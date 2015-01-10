/*
 *	Class:			GrammarList
 *	Parent class:	List
 *	Purpose:		To store grammar items
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

class GrammarList extends List
	{
	// Private constructible variables

	private boolean isCheckingGrammarNeeded_;

	private GrammarItem firstWordEndingGrammarItem_;
	private GrammarItem startOfGrammarItem_;


	// Private methods

	private GrammarResultType comparePluralEndingOfWord( int originalWordStringLength, int replacingWordStringLength, String originalWordString, String originalWordEndingString, String replacingWordEndingString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		int tempWordLength;
		int originalWordEndingStringLength;

		if( originalWordStringLength > 0 )
			{
			if( originalWordString != null )
				{
				if( originalWordEndingString != null )
					{
					originalWordEndingStringLength = originalWordEndingString.length();
					tempWordLength = ( originalWordStringLength - originalWordEndingStringLength );
					grammarResult.singularNounWordStringLength = ( originalWordStringLength + replacingWordStringLength - originalWordEndingStringLength );

					if( tempWordLength >= 0 &&
					grammarResult.singularNounWordStringLength > 0 )
						{
						if( originalWordString.substring( tempWordLength ).startsWith( originalWordEndingString ) )
							{
							grammarResult.hasFoundWordEnding = true;
							grammarResult.singularNounWordString = originalWordString.substring( 0, tempWordLength ) + ( replacingWordEndingString == null ? Constants.EMPTY_STRING : replacingWordEndingString );
							}
						}
					}
				else
					startError( 1, null, myWordItem().anyWordTypeString(), "The given original word ending string is undefined" );
				}
			else
				startError( 1, null, myWordItem().anyWordTypeString(), "The given original word string is undefined" );
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given original word string length is undefined" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	private GrammarItem firstWordEndingGrammarItem( short grammarParameter )
		{
		GrammarItem searchItem = firstWordEndingGrammarItem_;

		while( searchItem != null )
			{
			if( searchItem.grammarParameter() == grammarParameter )
				return searchItem;

			searchItem = searchItem.nextGrammarItem();
			}

		return null;
		}


	// Constructor / deconstructor

	protected GrammarList( WordItem myWordItem )
		{
		isCheckingGrammarNeeded_ = false;
		firstWordEndingGrammarItem_ = null;
		startOfGrammarItem_ = null;

		initializeListVariables( Constants.WORD_GRAMMAR_LANGUAGE_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void markAsOptionEnd()
		{
		boolean hasFound = false;
		GrammarItem searchItem = firstActiveGrammarItem();

		while( searchItem != null &&
		!hasFound )
			{
			if( searchItem.hasCurrentCreationSentenceNr() &&
			searchItem.itemNr() == CommonVariables.currentItemNr )
				{
				hasFound = true;
				searchItem.isOptionEnd = true;
				}

			searchItem = searchItem.nextGrammarItem();
			}
		}

	protected void markAsChoiceEnd( int choiceEndItemNr )
		{
		boolean hasFound = false;
		GrammarItem searchItem = firstActiveGrammarItem();

		while( searchItem != null &&
		!hasFound )
			{
			if( searchItem.hasCurrentCreationSentenceNr() &&
			searchItem.itemNr() == choiceEndItemNr )
				{
				hasFound = true;
				searchItem.isChoiceEnd = true;
				}

			searchItem = searchItem.nextGrammarItem();
			}
		}

	protected boolean isCheckingGrammarNeeded()
		{
		return isCheckingGrammarNeeded_;
		}

	protected short guideByGrammarStringWordTypeNr( String guideByGrammarString )
		{
		GrammarItem searchItem = firstActiveGrammarItem();

		if( guideByGrammarString != null )
			{
			while( searchItem != null )
				{
				if( searchItem.guideByGrammarString != null &&
				guideByGrammarString.equals( searchItem.guideByGrammarString ) )
					return searchItem.grammarWordTypeNr();

				searchItem = searchItem.nextGrammarItem();
				}
			}

		return Constants.WORD_TYPE_UNDEFINED;
		}

	protected GrammarResultType checkOnWordEnding( short grammarParameter, int originalWordStringLength, String originalWordString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		String replacingWordEndingString = null;
		GrammarItem originalWordEndingGrammarItem;
		GrammarItem replacingWordEndingGrammarItem;

		if( grammarParameter > Constants.NO_GRAMMAR_PARAMETER )
			{
			if( originalWordString != null )
				{
				if( originalWordStringLength == 0 )
					originalWordStringLength = originalWordString.length();

				if( ( originalWordEndingGrammarItem = firstWordEndingGrammarItem( grammarParameter ) ) != null )
					{
					do	{
						if( originalWordEndingGrammarItem.isDefinitionStart() )
							{
							replacingWordEndingGrammarItem = originalWordEndingGrammarItem.nextDefinitionGrammarItem;
							replacingWordEndingString = ( replacingWordEndingGrammarItem == null ? null : replacingWordEndingGrammarItem.grammarString() );

							if( ( grammarResult = comparePluralEndingOfWord( originalWordStringLength, ( replacingWordEndingString == null ? 0 : replacingWordEndingString.length() ), originalWordString, originalWordEndingGrammarItem.itemString(), replacingWordEndingString ) ).result != Constants.RESULT_OK )
								addError( 1, null, myWordItem().anyWordTypeString(), "I failed to find the plural ending of an undefined word type" );
							}
						}
					while( CommonVariables.result == Constants.RESULT_OK &&
					!grammarResult.hasFoundWordEnding &&
					( originalWordEndingGrammarItem = originalWordEndingGrammarItem.nextWordEndingGrammarItem() ) != null );
					}
				}
			else
				startError( 1, null, myWordItem().anyWordTypeString(), "The given original word string is undefined" );
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given grammar parameter is undefined" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	protected GrammarResultType createGrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		if( wordTypeNr >= Constants.WORD_TYPE_UNDEFINED &&
		wordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
				{
				if( ( grammarResult.createdGrammarItem = new GrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem, this, myWordItem() ) ) != null )
					{
					if( addItemToList( Constants.QUERY_ACTIVE_CHAR, grammarResult.createdGrammarItem ) == Constants.RESULT_OK )
						{
						isCheckingGrammarNeeded_ = true;

						if( grammarResult.createdGrammarItem.isDefinitionStart() )
							{
							if( grammarResult.createdGrammarItem.isWordEnding() )
								firstWordEndingGrammarItem_ = grammarResult.createdGrammarItem;

							if( grammarResult.createdGrammarItem.isGrammarStart() )
								startOfGrammarItem_ = grammarResult.createdGrammarItem;
							}
						}
					else
						addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active grammar item" );
					}
				else
					startError( 1, null, myWordItem().anyWordTypeString(), "I failed to create a grammar item" );
				}
			else
				startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given collected word type number is undefined or out of bounds" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	protected GrammarResultType findGrammarItem( boolean ignoreValue, short grammarParameter, int grammarStringLength, String grammarString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		GrammarItem searchItem = firstActiveGrammarItem();

		if( grammarString != null )
			{
			while( searchItem != null &&
			grammarResult.foundGrammarItem == null )
				{
				if( searchItem.grammarString() != null )
					{
					if( searchItem.isDefinitionStart() &&

					( ignoreValue ||
					searchItem.grammarParameter() == grammarParameter ) &&

					grammarStringLength == searchItem.grammarString().length() &&
					grammarString.startsWith( searchItem.grammarString() ) )
						grammarResult.foundGrammarItem = searchItem;
					else
						searchItem = searchItem.nextGrammarItem();
					}
				else
					startError( 1, null, myWordItem().anyWordTypeString(), "I found a grammar word without grammar string" );
				}
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given grammar string is undefined" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	protected GrammarResultType checkForDuplicateGrammarDefinition()
		{
		GrammarResultType grammarResult = new GrammarResultType();
		boolean isIdentical;
		String grammarString;
		String definitionGrammarString;
		GrammarItem currentGrammarItem;
		GrammarItem duplicateGrammarItem;
		GrammarItem nextGrammarItem;
		GrammarItem definitionGrammarItem = null;
		GrammarItem duplicateDefinitionGrammarItem = null;
		GrammarItem searchItem = firstActiveGrammarItem();

		while( searchItem != null &&
		duplicateDefinitionGrammarItem == null )
			{
			if( searchItem.isDefinitionStart() )
				{
				if( searchItem.hasCurrentCreationSentenceNr() )
					definitionGrammarItem = searchItem;
				else
					{
					if( definitionGrammarItem != null &&
					searchItem.creationSentenceNr() < CommonVariables.currentSentenceNr )
						{
						grammarString = searchItem.grammarString();
						definitionGrammarString = definitionGrammarItem.grammarString();

						if( grammarString != null &&
						definitionGrammarString != null &&
						definitionGrammarString.equals( grammarString ) )
							{
							searchItem.isGrammarItemInUse = true;
							duplicateDefinitionGrammarItem = searchItem;
							definitionGrammarItem.nextDefinitionGrammarItem = searchItem;
							}
						}
					}
				}

			searchItem = searchItem.nextGrammarItem();
			}

		if( definitionGrammarItem != null )
			{
			if( duplicateDefinitionGrammarItem != null )
				{
				do	{
					isIdentical = true;
					currentGrammarItem = definitionGrammarItem;
					duplicateGrammarItem = duplicateDefinitionGrammarItem;

					do	{
						if( currentGrammarItem.isIdentical( duplicateGrammarItem ) )
							{
							nextGrammarItem = currentGrammarItem.nextGrammarItem();

							currentGrammarItem = ( nextGrammarItem != null &&
													nextGrammarItem.hasCurrentCreationSentenceNr() ? nextGrammarItem : null );

							nextGrammarItem = duplicateGrammarItem.nextGrammarItem();

							duplicateGrammarItem = ( nextGrammarItem != null &&
													nextGrammarItem.creationSentenceNr() == duplicateDefinitionGrammarItem.creationSentenceNr() ? nextGrammarItem : null );
							}
						else
							isIdentical = false;
						}
					while( isIdentical &&
					currentGrammarItem != null &&
					duplicateGrammarItem != null );

					if( isIdentical &&
					currentGrammarItem == null &&
					duplicateGrammarItem == null )
						startError( 1, null, myWordItem().anyWordTypeString(), "I found a duplicate grammar definition" );
					}
				while( CommonVariables.result == Constants.RESULT_OK &&
				( duplicateDefinitionGrammarItem = duplicateDefinitionGrammarItem.nextDefinitionGrammarItem ) != null );
				}
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find the last grammar definition word" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	protected byte checkGrammar()
		{
		short grammarWordTypeNr;
		short currentWordTypeNr = ( Constants.NUMBER_OF_WORD_TYPES - 1 );
		GrammarItem currentGrammarItem;

		if( ( currentGrammarItem = firstActiveGrammarItem() ) != null )
			{
			isCheckingGrammarNeeded_ = false;

			do	{
				if( currentGrammarItem.isDefinitionStart() )
					{
					if( currentGrammarItem.isNewStart() &&
					currentGrammarItem.isUserDefinedWord() &&
					( grammarWordTypeNr = currentGrammarItem.grammarWordTypeNr() ) <= currentWordTypeNr )
						{
						if( grammarWordTypeNr < currentWordTypeNr )
							{
							if( grammarWordTypeNr + 1 == currentWordTypeNr )
								{
								if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE, myWordItem().anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == Constants.RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an interface notification" );
								}
							else
								{
								if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START, ( grammarWordTypeNr + 1 ), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE, myWordItem().anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == Constants.RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an interface notification" );
								}
							}

						currentWordTypeNr--;
						}

					if( !currentGrammarItem.isGrammarItemInUse &&
					!currentGrammarItem.isGrammarStart() &&
					!currentGrammarItem.isUndefinedWord() &&
					!currentGrammarItem.isUserDefinedWord() &&
					!currentGrammarItem.isWordEnding() )
						{
						if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_START, currentGrammarItem.grammarString(), Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_MIDDLE, myWordItem().anyWordTypeString(), Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_END ) != Constants.RESULT_OK )
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an interface notification" );
						}
					}
				else
					{
					if( currentGrammarItem.definitionGrammarItem == null )
						return startError( 1, null, myWordItem().anyWordTypeString(), "Grammar word \"" + currentGrammarItem.grammarString() + "\" in " + myWordItem().anyWordTypeString() + " is used, but not defined" );
					}
				}
			while( ( currentGrammarItem = currentGrammarItem.nextGrammarItem() ) != null );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find any grammar item" );

		return Constants.RESULT_OK;
		}

	protected byte checkGrammarItemForUsage( GrammarItem unusedGrammarItem )
		{
		GrammarItem searchItem = firstActiveGrammarItem();

		if( unusedGrammarItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.definitionGrammarItem == unusedGrammarItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The definition grammar item is still in use" );

				if( searchItem.nextDefinitionGrammarItem == unusedGrammarItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The next definition grammar item is still in use" );

				searchItem = searchItem.nextGrammarItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused grammar item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte linkLaterDefinedGrammarWords()
		{
		String definitionGrammarString;
		String grammarString;
		GrammarItem currentGrammarItem;
		GrammarItem definitionGrammarItem;

		if( ( definitionGrammarItem = firstActiveGrammarItem() ) != null )
			{
			if( ( definitionGrammarString = definitionGrammarItem.grammarString() ) != null )
				{
				currentGrammarItem = definitionGrammarItem;

				while( ( currentGrammarItem = currentGrammarItem.nextGrammarItem() ) != null )
					{
					if( ( grammarString = currentGrammarItem.grammarString() ) != null )
						{
						if( currentGrammarItem.definitionGrammarItem == null )
							{
							if( !currentGrammarItem.isDefinitionStart() &&
							definitionGrammarString.equals( grammarString ) )
								{
								definitionGrammarItem.isGrammarItemInUse = true;
								currentGrammarItem.definitionGrammarItem = definitionGrammarItem;
								}
							}
						else
							{
							if( currentGrammarItem.definitionGrammarItem == definitionGrammarItem.nextDefinitionGrammarItem )
								currentGrammarItem.definitionGrammarItem = definitionGrammarItem;
							}
						}
					else
						return startError( 1, null, myWordItem().anyWordTypeString(), "The grammar string of the grammar definition word is undefined" );
					}
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The grammar string of the grammar definition word is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find any grammar item" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDataBase()
		{
		// Not fully implemented yet
		GrammarItem searchItem = firstActiveGrammarItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeGrammarItemInFutureDataBase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store a grammar item in the database" );
				}

			searchItem = searchItem.nextGrammarItem();
			}

		searchItem = firstReplacedGrammarItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeGrammarItemInFutureDataBase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced grammar item in the database" );
				}

			searchItem = searchItem.nextGrammarItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected GrammarItem firstActiveGrammarItem()
		{
		return (GrammarItem)firstActiveItem();
		}

	protected GrammarItem startOfGrammarItem()
		{
		return startOfGrammarItem_;
		}

	protected String grammarStringInList( short wordTypeNr )
		{
		GrammarItem searchItem = firstActiveGrammarItem();

		while( searchItem != null )
			{
			if( searchItem.isDefinitionStart() &&
			searchItem.isNewStart() &&
			searchItem.isUserDefinedWord() &&
			searchItem.grammarWordTypeNr() == wordTypeNr )
				return searchItem.grammarString();

			searchItem = searchItem.nextGrammarItem();
			}

		return null;
		}
	};

/*************************************************************************
 *
 *	"The voice of the Lord echoes above the sea.
 *	The God of glory thunders.
 *	The Lord thunders over the mighty sea." (Psalm 29:3)
 *
 *************************************************************************/
