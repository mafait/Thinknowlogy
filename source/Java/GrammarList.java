/*	Class:			GrammarList
 *	Parent class:	List
 *	Purpose:		To store grammar items
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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
	// Private constructed variables

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

		if( searchWordStringLength <= 0 )
			return startGrammarResultError( 1, null, "The given search word string length is undefined" );

		if( searchWordString == null )
			return startGrammarResultError( 1, null, "The given search word string is undefined" );

		if( searchWordEndingString == null )
			return startGrammarResultError( 1, null, "The given search word ending string is undefined" );

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


	// Constructor

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
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != null &&
		!hasFound )
			{
			if( searchGrammarItem.hasCurrentCreationSentenceNr() &&
			searchGrammarItem.itemNr() == currentItemNr )
				{
				hasFound = true;
				searchGrammarItem.isOptionEnd = true;
				}

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}
		}

	protected void markAsChoiceEnd( int choiceEndItemNr )
		{
		boolean hasFound = false;
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != null &&
		!hasFound )
			{
			if( searchGrammarItem.hasCurrentCreationSentenceNr() &&
			searchGrammarItem.itemNr() == choiceEndItemNr )
				{
				hasFound = true;
				searchGrammarItem.isChoiceEnd = true;
				}

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}
		}

	protected boolean hasMergedWords()
		{
		return ( firstMergedWordGrammarItem_ != null );
		}

	protected boolean isNeededToCheckGrammar()
		{
		return isNeededToCheckGrammar_;
		}

	protected GrammarResultType analyzeWordEnding( short grammarParameter, int searchWordStringLength, String searchWordString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		String replacingWordEndingString = null;
		GrammarItem replacingWordEndingGrammarItem;
		GrammarItem searchGrammarItem = firstWordEndingGrammarItem( grammarParameter );

		if( !isWordEnding( grammarParameter ) )
			return startGrammarResultError( 1, null, "The given grammar parameter is not a word ending parameter" );

		// The given type of word ending is defined for the current language
		if( searchGrammarItem != null )
			{
			if( searchWordString == null )
				return startGrammarResultError( 1, null, "The given search word string is undefined" );

			if( searchWordStringLength == 0 )
				searchWordStringLength = searchWordString.length();

			do	{
				if( searchGrammarItem.isDefinitionStart() )
					{
					replacingWordEndingGrammarItem = searchGrammarItem.nextDefinitionGrammarItem;
					replacingWordEndingString = ( replacingWordEndingGrammarItem == null ? null : replacingWordEndingGrammarItem.grammarString() );

					if( ( grammarResult = comparePluralEndingOfWord( searchWordStringLength, ( replacingWordEndingString == null ? 0 : replacingWordEndingString.length() ), searchWordString, searchGrammarItem.itemString(), replacingWordEndingString ) ).result != Constants.RESULT_OK )
						return addGrammarResultError( 1, null, "I failed to find the plural ending of an undefined word type" );
					}
				}
			while( !grammarResult.hasFoundWordEnding &&
			( searchGrammarItem = searchGrammarItem.nextWordEndingGrammarItem() ) != null );
			}

		return grammarResult;
		}

	protected GrammarResultType createGrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		if( wordTypeNr < Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startGrammarResultError( 1, null, "The given word type number is out of bounds" );

		if( ( grammarResult.createdGrammarItem = new GrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem, this, myWordItem() ) ) == null )
			return startGrammarResultError( 1, null, "I failed to create a grammar item" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, grammarResult.createdGrammarItem ) != Constants.RESULT_OK )
			return addGrammarResultError( 1, null, "I failed to add an active grammar item" );

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

		return grammarResult;
		}

	protected GrammarResultType findGrammar( boolean isIgnoringGrammarParameter, short grammarParameter, int grammarStringLength, String grammarString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		if( grammarString == null )
			return startGrammarResultError( 1, null, "The given grammar string is undefined" );

		do	{
			if( searchGrammarItem.grammarString() == null )
				return startGrammarResultError( 1, null, "I have found a grammar word without grammar string" );

			if( searchGrammarItem.isDefinitionStart() &&

			( isIgnoringGrammarParameter ||
			searchGrammarItem.grammarParameter() == grammarParameter ) &&

			grammarStringLength == searchGrammarItem.grammarString().length() &&
			grammarString.startsWith( searchGrammarItem.grammarString() ) )
				grammarResult.foundGrammarItem = searchGrammarItem;
			}
		while( grammarResult.foundGrammarItem == null &&
		( searchGrammarItem = searchGrammarItem.nextGrammarItem() ) != null );

		return grammarResult;
		}

	protected byte checkForDuplicateGrammarDefinition()
		{
		boolean isIdentical;
		String grammarString;
		String definitionGrammarString;
		GrammarItem currentGrammarItem;
		GrammarItem duplicateGrammarItem;
		GrammarItem nextGrammarItem;
		GrammarItem definitionGrammarItem = null;
		GrammarItem duplicateDefinitionGrammarItem = null;
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != null &&
		duplicateDefinitionGrammarItem == null )
			{
			if( searchGrammarItem.isDefinitionStart() )
				{
				if( searchGrammarItem.hasCurrentCreationSentenceNr() )
					definitionGrammarItem = searchGrammarItem;
				else
					{
					if( definitionGrammarItem != null &&
					searchGrammarItem.creationSentenceNr() < CommonVariables.currentSentenceNr )
						{
						grammarString = searchGrammarItem.grammarString();
						definitionGrammarString = definitionGrammarItem.grammarString();

						if( grammarString != null &&
						definitionGrammarString != null &&
						definitionGrammarString.equals( grammarString ) )
							{
							searchGrammarItem.isGrammarItemInUse = true;
							duplicateDefinitionGrammarItem = searchGrammarItem;
							definitionGrammarItem.nextDefinitionGrammarItem = searchGrammarItem;
							}
						}
					}
				}

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}

		if( definitionGrammarItem == null )
			return startError( 1, null, "I couldn't find the last grammar definition word" );

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
					return startError( 1, null, "I have found a duplicate grammar definition" );
				}
			while( ( duplicateDefinitionGrammarItem = duplicateDefinitionGrammarItem.nextDefinitionGrammarItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte checkGrammar()
		{
		short grammarWordTypeNr;
		short currentWordTypeNr = ( Constants.NUMBER_OF_WORD_TYPES - 1 );
		GrammarItem currentGrammarItem;
		WordItem currentLangugeWordItem = myWordItem();

		if( ( currentGrammarItem = firstActiveGrammarItem() ) == null )
			return startError( 1, null, "I couldn't find any grammar item" );

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
							if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to write the 'grammar word type definition missing' interface notification" );

							currentWordTypeNr = grammarWordTypeNr;
							}
						else
							{
							if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START, ( grammarWordTypeNr + 1 ), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to write the 'grammar word type definitions missing' interface notification" );

							currentWordTypeNr = grammarWordTypeNr;
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

		return Constants.RESULT_OK;
		}

	protected byte checkGrammarItemForUsage( GrammarItem unusedGrammarItem )
		{
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		if( unusedGrammarItem == null )
			return startError( 1, null, "The given unused grammar item is undefined" );

		while( searchGrammarItem != null )
			{
			if( searchGrammarItem.definitionGrammarItem == unusedGrammarItem )
				return startError( 1, null, "The definition grammar item is still in use" );

			if( searchGrammarItem.nextDefinitionGrammarItem == unusedGrammarItem )
				return startError( 1, null, "The next definition grammar item is still in use" );

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte linkLaterDefinedGrammarWords()
		{
		String definitionGrammarString;
		String grammarString;
		GrammarItem currentGrammarItem;
		GrammarItem definitionGrammarItem;

		if( ( definitionGrammarItem = firstActiveGrammarItem() ) == null )
			return startError( 1, null, "I couldn't find any grammar item" );

		if( ( definitionGrammarString = definitionGrammarItem.grammarString() ) == null )
			return startError( 1, null, "The grammar string of the grammar definition word is undefined" );

		currentGrammarItem = definitionGrammarItem;

		while( ( currentGrammarItem = currentGrammarItem.nextGrammarItem() ) != null )
			{
			if( ( grammarString = currentGrammarItem.grammarString() ) == null )
				return startError( 1, null, "The grammar string of the grammar definition word is undefined" );

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

		if( readUserSentenceStringBuffer == null )
			return startError( 1, null, "The given read user sentence string buffer is undefined" );

		CommonVariables.readUserSentenceStringBuffer = readUserSentenceStringBuffer;

		if( ( readUserSentenceStringBufferLength = readUserSentenceStringBuffer.length() ) == 0 )
			return startError( 1, null, "The given read user sentence string buffer is empty" );

		if( expandMergedWordGrammarItem == null )
			return startError( 1, null, "No grammar compound word definition was found" );

		do	{
			if( expandMergedWordGrammarItem.isDefinitionStart() )
				{
				if( ( searchMergedWordGrammarItem = expandMergedWordGrammarItem.nextDefinitionGrammarItem ) == null )
					return startError( 1, null, "I have found a compound word definition without replacing compound word definition" );

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
			}
		while( ( expandMergedWordGrammarItem = expandMergedWordGrammarItem.nextWordEndingGrammarItem() ) != null );

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

		if( writtenSentenceStringBuffer == null ||
		( writtenSentenceStringBufferLength = writtenSentenceStringBuffer.length() ) == 0 )
			return startError( 1, null, "The write sentence string buffer is empty" );

		if( searchMergedWordGrammarItem == null )
			return startError( 1, null, "No grammar compound word definition was found" );

		do	{
			if( searchMergedWordGrammarItem.isDefinitionStart() )
				{
				if( ( shrinkMergedWordGrammarItem = searchMergedWordGrammarItem.nextDefinitionGrammarItem ) == null )
					return startError( 1, null, "I have found a compound word definition without replacing compound word definition" );

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
			}
		while( ( searchMergedWordGrammarItem = searchMergedWordGrammarItem.nextWordEndingGrammarItem() ) != null );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != null )
			{
			if( searchGrammarItem.hasCurrentCreationSentenceNr() )
				{
				if( searchGrammarItem.storeGrammarItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a grammar item in the database" );
				}

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected GrammarItem firstGrammarItem()
		{
		return firstGrammarItem_;
		}

	protected String guideByGrammarString( GrammarItem startGrammarItem )
		{
		GrammarItem searchGrammarItem = startGrammarItem;

		while( searchGrammarItem != null &&
		searchGrammarItem.nextDefinitionGrammarItem != null )
			searchGrammarItem = searchGrammarItem.nextDefinitionGrammarItem;

		if( searchGrammarItem != null &&
		searchGrammarItem.guideByGrammarString != null )
			return searchGrammarItem.guideByGrammarString;

		return null;
		}

	protected String grammarStringInList( short wordTypeNr )
		{
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != null )
			{
			if( searchGrammarItem.isDefinitionStart() &&
			searchGrammarItem.isNewStart() &&
			searchGrammarItem.isUserDefinedWord() &&
			searchGrammarItem.grammarWordTypeNr() == wordTypeNr )
				return searchGrammarItem.grammarString();

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}

		return null;
		}
	};

/*************************************************************************
 *	"The voice of the Lord echoes above the sea.
 *	The God of glory thunders.
 *	The Lord thunders over the mighty sea." (Psalm 29:3)
 *************************************************************************/
