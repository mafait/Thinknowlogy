/*	Class:			GrammarList
 *	Parent class:	List
 *	Purpose:		To store grammar items
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
 *************************************************************************/
/*	Copyright (C) 2009-2016, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at http://mafait.org/contact/
 *************************************************************************/
/*	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this program; if not, write to the Free Software Foundation, Inc.,
 *	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/

class GrammarList extends List
	{
	// Private constructible variables

	private boolean isNeededToCheckGrammar_;

	private GrammarItem firstGrammarItem_;
	private GrammarItem firstFeminineProperNameEndingGrammarItem_;
	private GrammarItem firstMasculineProperNameEndingGrammarItem_;
	private GrammarItem firstFeminineSingularNounEndingGrammarItem_;
	private GrammarItem firstMasculineSingularNounEndingGrammarItem_;
	private GrammarItem firstPluralNounEndingGrammarItem_;
	private GrammarItem firstMergedWordGrammarItem_;


	// Private methods

	private static boolean isText( int endPosition, String sentenceString )
		{
		boolean isText = false;
		int currentPosition = 0;

		if( sentenceString != null )
			{
			// Check for text string
			while( ++currentPosition < endPosition )
				{
				if( sentenceString.charAt( currentPosition ) == Constants.SYMBOL_DOUBLE_QUOTE &&
				sentenceString.charAt( currentPosition - 1 ) != Constants.SYMBOL_BACK_SLASH )
					isText = !isText;
				}
			}

		return isText;
		}

	private static boolean isWordEnding( short grammarParameter )
		{
		return ( grammarParameter == Constants.WORD_FEMININE_PROPER_NAME_ENDING ||
				grammarParameter == Constants.WORD_MASCULINE_PROPER_NAME_ENDING ||
				grammarParameter == Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING ||
				grammarParameter == Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING ||
				grammarParameter == Constants.WORD_PLURAL_NOUN_ENDING ||
				grammarParameter == Constants.WORD_MERGED_WORD );
		}

	private GrammarResultType comparePluralEndingOfWord( int searchWordStringLength, int replacingWordStringLength, String searchWordString, String searchWordEndingString, String replacingWordEndingString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		int tempWordLength;
		int searchWordEndingStringLength;

		if( searchWordStringLength > 0 )
			{
			if( searchWordString != null )
				{
				if( searchWordEndingString != null )
					{
					searchWordEndingStringLength = searchWordEndingString.length();
					tempWordLength = ( searchWordStringLength - searchWordEndingStringLength );
					grammarResult.singularNounWordStringLength = ( searchWordStringLength + replacingWordStringLength - searchWordEndingStringLength );

					if( tempWordLength >= 0 &&
					grammarResult.singularNounWordStringLength > 0 )
						{
						if( searchWordString.substring( tempWordLength ).startsWith( searchWordEndingString ) )
							{
							grammarResult.hasFoundWordEnding = true;
							grammarResult.singularNounWordString = searchWordString.substring( 0, tempWordLength ) + ( replacingWordEndingString == null ? Constants.EMPTY_STRING : replacingWordEndingString );
							}
						}
					}
				else
					startError( 1, null, "The given search word ending string is undefined" );
				}
			else
				startError( 1, null, "The given search word string is undefined" );
			}
		else
			startError( 1, null, "The given search word string length is undefined" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	private GrammarItem firstActiveGrammarItem()
		{
		return (GrammarItem)firstActiveItem();
		}

	private GrammarItem firstWordEndingGrammarItem( short grammarParameter )
		{
		switch( grammarParameter )
			{
			case Constants.WORD_FEMININE_PROPER_NAME_ENDING:
				return firstFeminineProperNameEndingGrammarItem_;

			case Constants.WORD_MASCULINE_PROPER_NAME_ENDING:
				return firstMasculineProperNameEndingGrammarItem_;

			case Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING:
				return firstFeminineSingularNounEndingGrammarItem_;

			case Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING:
				return firstMasculineSingularNounEndingGrammarItem_;

			case Constants.WORD_PLURAL_NOUN_ENDING:
				return firstPluralNounEndingGrammarItem_;

			case Constants.WORD_MERGED_WORD:
				return firstMergedWordGrammarItem_;
			}

		return null;
		}


	// Constructor / deconstructor

	protected GrammarList( WordItem myWordItem )
		{
		isNeededToCheckGrammar_ = false;

		firstGrammarItem_ = null;
		firstFeminineProperNameEndingGrammarItem_ = null;
		firstMasculineProperNameEndingGrammarItem_ = null;
		firstFeminineSingularNounEndingGrammarItem_ = null;
		firstMasculineSingularNounEndingGrammarItem_ = null;
		firstPluralNounEndingGrammarItem_ = null;
		firstMergedWordGrammarItem_ = null;

		initializeListVariables( Constants.WORD_GRAMMAR_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void markAsOptionEnd()
		{
		boolean hasFound = false;
		int currentItemNr = CommonVariables.currentItemNr;
		GrammarItem searchItem = firstActiveGrammarItem();

		while( searchItem != null &&
		!hasFound )
			{
			if( searchItem.hasCurrentCreationSentenceNr() &&
			searchItem.itemNr() == currentItemNr )
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

	protected boolean isNeededToCheckGrammar()
		{
		return isNeededToCheckGrammar_;
		}

	protected boolean hasMergedWords()
		{
		return ( firstMergedWordGrammarItem_ != null );
		}

	protected GrammarResultType analyzeWordEnding( short grammarParameter, int searchWordStringLength, String searchWordString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		String replacingWordEndingString = null;
		GrammarItem replacingWordEndingGrammarItem;
		GrammarItem searchItem = firstWordEndingGrammarItem( grammarParameter );

		if( isWordEnding( grammarParameter ) )
			{
			// The given type of word ending is defined for the current language
			if( searchItem != null )
				{
				if( searchWordString != null )
					{
					if( searchWordStringLength == 0 )
						searchWordStringLength = searchWordString.length();

					do	{
						if( searchItem.isDefinitionStart() )
							{
							replacingWordEndingGrammarItem = searchItem.nextDefinitionGrammarItem;
							replacingWordEndingString = ( replacingWordEndingGrammarItem == null ? null : replacingWordEndingGrammarItem.grammarString() );

							if( ( grammarResult = comparePluralEndingOfWord( searchWordStringLength, ( replacingWordEndingString == null ? 0 : replacingWordEndingString.length() ), searchWordString, searchItem.itemString(), replacingWordEndingString ) ).result != Constants.RESULT_OK )
								addError( 1, null, "I failed to find the plural ending of an undefined word type" );
							}
						}
					while( CommonVariables.result == Constants.RESULT_OK &&
					!grammarResult.hasFoundWordEnding &&
					( searchItem = searchItem.nextWordEndingGrammarItem() ) != null );
					}
				else
					startError( 1, null, "The given search word string is undefined" );
				}
			}
		else
			startError( 1, null, "The given grammar parameter is not a word ending parameter" );

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
						startError( 1, null, "I have found a duplicate grammar definition" );
					}
				while( CommonVariables.result == Constants.RESULT_OK &&
				( duplicateDefinitionGrammarItem = duplicateDefinitionGrammarItem.nextDefinitionGrammarItem ) != null );
				}
			}
		else
			startError( 1, null, "I couldn't find the last grammar definition word" );

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
						isNeededToCheckGrammar_ = true;

						if( grammarResult.createdGrammarItem.isDefinitionStart() )
							{
							switch( grammarParameter )
								{
								case Constants.WORD_FEMININE_PROPER_NAME_ENDING:
									firstFeminineProperNameEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case Constants.WORD_MASCULINE_PROPER_NAME_ENDING:
									firstMasculineProperNameEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING:
									firstFeminineSingularNounEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING:
									firstMasculineSingularNounEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case Constants.WORD_PLURAL_NOUN_ENDING:
									firstPluralNounEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case Constants.WORD_MERGED_WORD:
									firstMergedWordGrammarItem_ = grammarResult.createdGrammarItem;
									break;
								}

							if( grammarResult.createdGrammarItem.isGrammarStart() )
								firstGrammarItem_ = grammarResult.createdGrammarItem;
							}
						}
					else
						addError( 1, null, "I failed to add an active grammar item" );
					}
				else
					startError( 1, null, "I failed to create a grammar item" );
				}
			else
				startError( 1, null, "The current item number is undefined" );
			}
		else
			startError( 1, null, "The given collected word type number is undefined or out of bounds" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	protected GrammarResultType findGrammarItem( boolean ignoreValue, short grammarParameter, int grammarStringLength, String grammarString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		GrammarItem searchItem = firstActiveGrammarItem();

		if( grammarString != null )
			{
			while( CommonVariables.result == Constants.RESULT_OK &&
			searchItem != null &&
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
					startError( 1, null, "I have found a grammar word without grammar string" );
				}
			}
		else
			startError( 1, null, "The given grammar string is undefined" );

		grammarResult.result = CommonVariables.result;
		return grammarResult;
		}

	protected byte checkGrammar()
		{
		short grammarWordTypeNr;
		short currentWordTypeNr = ( Constants.NUMBER_OF_WORD_TYPES - 1 );
		GrammarItem currentGrammarItem;
		WordItem currentLangugeWordItem = myWordItem();

		if( ( currentGrammarItem = firstActiveGrammarItem() ) != null )
			{
			isNeededToCheckGrammar_ = false;

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
								if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == Constants.RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( 1, null, "I failed to write the 'grammar word type definition missing' interface notification" );
								}
							else
								{
								if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START, ( grammarWordTypeNr + 1 ), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == Constants.RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( 1, null, "I failed to write the 'grammar word type definitions missing' interface notification" );
								}
							}

						currentWordTypeNr--;
						}

					if( !currentGrammarItem.isGrammarItemInUse &&
					!currentGrammarItem.isGrammarStart() &&
					!currentGrammarItem.isUndefinedWord() &&
					!currentGrammarItem.isUserDefinedWord() &&
					!isWordEnding( currentGrammarItem.grammarParameter() ) )
						{
						if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_START, currentGrammarItem.grammarString(), Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_END ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to write the 'grammar definition is not used' interface notification" );
						}
					}
				else
					{
					if( currentGrammarItem.definitionGrammarItem == null )
						return startError( 1, null, "Grammar word \"" + currentGrammarItem.grammarString() + "\" in " + currentLangugeWordItem.anyWordTypeString() + " is used, but not defined" );
					}
				}
			while( ( currentGrammarItem = currentGrammarItem.nextGrammarItem() ) != null );
			}
		else
			return startError( 1, null, "I couldn't find any grammar item" );

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
					return startError( 1, null, "The definition grammar item is still in use" );

				if( searchItem.nextDefinitionGrammarItem == unusedGrammarItem )
					return startError( 1, null, "The next definition grammar item is still in use" );

				searchItem = searchItem.nextGrammarItem();
				}
			}
		else
			return startError( 1, null, "The given unused grammar item is undefined" );

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
						return startError( 1, null, "The grammar string of the grammar definition word is undefined" );
					}
				}
			else
				return startError( 1, null, "The grammar string of the grammar definition word is undefined" );
			}
		else
			return startError( 1, null, "I couldn't find any grammar item" );

		return Constants.RESULT_OK;
		}

	protected byte expandMergedWordsInReadSentence( StringBuffer readUserSentenceStringBuffer )
		{
		int offset;
		int previousPosition;
		int readUserSentenceStringBufferLength;
		int startPosition;
		GrammarItem searchMergedWordGrammarItem;
		GrammarItem expandMergedWordGrammarItem = firstMergedWordGrammarItem_;

		if( readUserSentenceStringBuffer != null )
			{
			CommonVariables.readUserSentenceStringBuffer = readUserSentenceStringBuffer;

			if( ( readUserSentenceStringBufferLength = readUserSentenceStringBuffer.length() ) > 0 )
				{
				if( expandMergedWordGrammarItem != null )
					{
					do	{
						if( expandMergedWordGrammarItem.isDefinitionStart() )
							{
							if( ( searchMergedWordGrammarItem = expandMergedWordGrammarItem.nextDefinitionGrammarItem ) != null )
								{
								previousPosition = 0;
								startPosition = 0;

								while( startPosition < readUserSentenceStringBufferLength &&
								( offset = readUserSentenceStringBuffer.substring( startPosition ).indexOf( searchMergedWordGrammarItem.grammarString() ) ) >= 0 )
									{
									startPosition += offset;

									// First position,
									if( startPosition == 0 ||

									( startPosition > 0 &&
									// or is preceded by a space character
									Character.isWhitespace( readUserSentenceStringBuffer.charAt( startPosition - 1 ) ) ) )
										{
										if( isText( offset, readUserSentenceStringBuffer.substring( previousPosition ) ) )
											// Skip match in text string
											startPosition++;
										else
											{
											previousPosition = startPosition;
											CommonVariables.readUserSentenceStringBuffer = new StringBuffer( readUserSentenceStringBuffer.substring( 0, startPosition ) + expandMergedWordGrammarItem.grammarString() + readUserSentenceStringBuffer.substring( startPosition + searchMergedWordGrammarItem.grammarString().length() ) );

											readUserSentenceStringBuffer = CommonVariables.readUserSentenceStringBuffer;
											readUserSentenceStringBufferLength = readUserSentenceStringBuffer.length();
											}
										}
									else
										// Skip match halfway word
										startPosition++;
									}
								}
							else
								return startError( 1, null, "I found a compound word definition without replacing compound word definition" );
							}
						}
					while( ( expandMergedWordGrammarItem = expandMergedWordGrammarItem.nextWordEndingGrammarItem() ) != null );
					}
				else
					return startError( 1, null, "No grammar compound word definition was found" );
				}
			else
				return startError( 1, null, "The given read user sentence string buffer is empty" );
			}
		else
			return startError( 1, null, "The given read user sentence string buffer is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte shrinkMergedWordsInWriteSentence()
		{
		int offset;
		int previousPosition;
		int startPosition;
		int writtenSentenceStringBufferLength;
		StringBuffer writtenSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer;
		GrammarItem shrinkMergedWordGrammarItem;
		GrammarItem searchMergedWordGrammarItem = firstMergedWordGrammarItem_;

		if( writtenSentenceStringBuffer != null &&
		( writtenSentenceStringBufferLength = writtenSentenceStringBuffer.length() ) > 0 )
			{
			if( searchMergedWordGrammarItem != null )
				{
				do	{
					if( searchMergedWordGrammarItem.isDefinitionStart() )
						{
						if( ( shrinkMergedWordGrammarItem = searchMergedWordGrammarItem.nextDefinitionGrammarItem ) != null )
							{
							previousPosition = 0;
							startPosition = 0;

							// Search for compound word
							while( startPosition < writtenSentenceStringBufferLength &&
							( offset = writtenSentenceStringBuffer.substring( startPosition ).indexOf( searchMergedWordGrammarItem.grammarString() ) ) >= 0 )
								{
								startPosition += offset;

								// First position,
								if( startPosition == 0 ||

								( startPosition > 0 &&
								// or is preceded by a space character
								Character.isWhitespace( writtenSentenceStringBuffer.charAt( startPosition - 1 ) ) ) )
									{
									if( isText( offset, writtenSentenceStringBuffer.substring( previousPosition ) ) )
										// Skip match in text string
										startPosition++;
									else
										{
										previousPosition = startPosition;
										CommonVariables.writtenSentenceStringBuffer = new StringBuffer( writtenSentenceStringBuffer.substring( 0, startPosition ) + shrinkMergedWordGrammarItem.grammarString() + writtenSentenceStringBuffer.substring( startPosition + searchMergedWordGrammarItem.grammarString().length() ) );

										writtenSentenceStringBuffer = CommonVariables.writtenSentenceStringBuffer;
										writtenSentenceStringBufferLength = writtenSentenceStringBuffer.length();
										}
									}
								else
									// Skip match halfway word
									startPosition++;
								}
							}
						else
							return startError( 1, null, "I found a compound word definition without replacing compound word definition" );
						}
					}
				while( ( searchMergedWordGrammarItem = searchMergedWordGrammarItem.nextWordEndingGrammarItem() ) != null );
				}
			else
				return startError( 1, null, "No grammar compound word definition was found" );
			}
		else
			return startError( 1, null, "The write sentence string buffer is empty" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		GrammarItem searchItem = firstActiveGrammarItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeGrammarItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a grammar item in the database" );
				}

			searchItem = searchItem.nextGrammarItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected GrammarItem firstGrammarItem()
		{
		return firstGrammarItem_;
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
 *	"The voice of the Lord echoes above the sea.
 *	The God of glory thunders.
 *	The Lord thunders over the mighty sea." (Psalm 29:3)
 *************************************************************************/
