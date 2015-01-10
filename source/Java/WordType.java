/*
 *	Class:			WordType
 *	Supports class:	WordItem
 *	Purpose:		To create word type structures
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class WordType
	{
	// Private constructible variables

	private boolean hasFeminineWordEnding_;
	private boolean hasMasculineWordEnding_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte checkOnFeminineAndMasculineWordEnding( boolean isSingularNoun, String wordString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		WordItem currentGrammarLanguageWordItem;

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		if( wordString != null )
			{
			if( ( currentGrammarLanguageWordItem = CommonVariables.currentGrammarLanguageWordItem ) != null )
				{
				if( ( grammarResult = currentGrammarLanguageWordItem.checkOnWordEnding( ( isSingularNoun ? Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING : Constants.WORD_FEMININE_PROPER_NAME_ENDING ), 0, wordString ) ).result == Constants.RESULT_OK )
					{
					if( grammarResult.hasFoundWordEnding )
						{
						hasFeminineWordEnding_ = true;

						if( isSingularNoun )
							{
							if( myWordItem_.markWordAsFeminine() != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark my word as feminine" );
							}
						}
					else
						{
						if( ( grammarResult = currentGrammarLanguageWordItem.checkOnWordEnding( ( isSingularNoun ? Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING : Constants.WORD_MASCULINE_PROPER_NAME_ENDING ), 0, wordString ) ).result == Constants.RESULT_OK )
							{
							if( grammarResult.hasFoundWordEnding )
								{
								hasMasculineWordEnding_ = true;

								if( isSingularNoun )
									{
									if( myWordItem_.markWordAsMasculine() != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark my word as masculine" );
									}
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check on masculine word ending" );
						}
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check on feminine word ending" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The current grammar language word item is undefined" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given word string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected WordType( WordItem myWordItem )
		{
		String errorString = null;

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected WordResultType addWordType( boolean isMultipleWord, boolean isProperNamePrecededByDefiniteArticle, short adjectiveParameter, short definiteArticleParameter, short indefiniteArticleParameter, short wordTypeNr, int wordLength, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();
		boolean isSingularNoun;

		hasFeminineWordEnding_ = false;
		hasMasculineWordEnding_ = false;

		if( !myWordItem_.isAdminWord() )
			{
			if( myWordItem_.wordTypeList == null )
				{
				// Create list
				if( ( myWordItem_.wordTypeList = new WordTypeList( myWordItem_ ) ) != null )
					myWordItem_.wordListArray[Constants.WORD_TYPE_LIST] = myWordItem_.wordTypeList;
				else
					myWordItem_.startErrorInWord( 1, moduleNameString_, "I failed to create a word type list" );
				}
			else
				{
				// Check if word type already exists
				if( ( wordResult = findWordType( false, wordTypeNr, wordTypeString ) ).result != Constants.RESULT_OK )
					myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find the given word type" );
				}

			// Add word type if it doesn't exist yet
			if( CommonVariables.result == Constants.RESULT_OK &&
			wordResult.foundWordTypeItem == null )
				{
				isSingularNoun = ( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR );

				if( isSingularNoun ||

				( wordTypeNr == Constants.WORD_TYPE_PROPER_NAME &&
				!myWordItem_.isLanguageWord() ) )
					{
					if( checkOnFeminineAndMasculineWordEnding( isSingularNoun, wordTypeString ) != Constants.RESULT_OK )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the string on feminine or masculine word ending" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( ( wordResult = myWordItem_.wordTypeList.createWordTypeItem( hasFeminineWordEnding_, hasMasculineWordEnding_, isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordLength, wordTypeString ) ).result != Constants.RESULT_OK )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a word type item" );
					}
				}
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The admin word item cannot have word types" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}

	protected WordResultType findWordType( boolean isForcingToCheckAllLanguages, short wordTypeNr, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();
		int currentWordTypeStringLength;
		int wordTypeStringLength;
		String currentWordTypeString;
		WordTypeItem currentWordTypeItem;

		if( wordTypeString != null )
			{
			if( ( wordTypeStringLength = wordTypeString.length() ) > 0 )
				{
				if( ( currentWordTypeItem = myWordItem_.activeWordTypeItem( isForcingToCheckAllLanguages, wordTypeNr ) ) != null )
					{
					do	{
						if( ( currentWordTypeString = currentWordTypeItem.itemString() ) != null )		// Skip hidden word type
							{
							if( ( currentWordTypeStringLength = currentWordTypeString.length() ) > 0 )
								{
								if( wordTypeStringLength == currentWordTypeStringLength &&
								wordTypeString.equals( currentWordTypeString ) )
									{
									wordResult.foundWordItem = myWordItem_;
									wordResult.foundWordTypeItem = currentWordTypeItem;
									}
								}
							else
								myWordItem_.startErrorInWord( 1, moduleNameString_, "The active word type string is empty" );
							}
						}
					while( CommonVariables.result == Constants.RESULT_OK &&
					wordResult.foundWordItem == null &&
					( currentWordTypeItem = currentWordTypeItem.nextWordTypeItem( wordTypeNr ) ) != null );
					}
				}
			else
				myWordItem_.startErrorInWord( 1, moduleNameString_, "The given search word string is empty" );
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given search word string is undefined" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}

	protected WordResultType findWordTypeInAllWords( boolean isForcingToCheckAllLanguages, short wordTypeNr, String wordTypeString, WordItem previousWordItem )
		{
		WordResultType wordResult = new WordResultType();
		WordItem currentWordItem;

		if( ( currentWordItem = ( previousWordItem == null ? CommonVariables.firstWordItem : previousWordItem.nextWordItem() ) ) != null )
			{
			do	{
				if( ( wordResult = currentWordItem.findWordType( isForcingToCheckAllLanguages, wordTypeNr, wordTypeString ) ).result != Constants.RESULT_OK )
					myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a word type in word \"" + currentWordItem.anyWordTypeString() + "\"" );
				}
			while( wordResult.foundWordItem == null &&
			( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		wordResult.result = CommonVariables.result;
		return wordResult;
		}

	protected String wordTypeString( boolean isCheckingAllLanguages, short wordTypeNr )
		{
		String wordTypeString = ( myWordItem_.wordTypeList == null ? null : myWordItem_.wordTypeList.wordTypeString( isCheckingAllLanguages, wordTypeNr ) );

		if( wordTypeString == null )
			return ( Constants.QUERY_ITEM_START_STRING + myWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + myWordItem_.itemNr() + Constants.QUERY_ITEM_END_CHAR );

		return wordTypeString;
		}
	};

/*************************************************************************
 *
 *	"Let them praise your great and awesome name.
 *	Your name is holy!" (Psalm 99:3)
 *
 *************************************************************************/
