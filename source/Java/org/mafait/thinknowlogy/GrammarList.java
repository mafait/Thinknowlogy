/*	Class:			GrammarList
 *	Parent class:	List
 *	Purpose:		Storing grammar items
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

package org.mafait.thinknowlogy;

class GrammarList extends List
	{
	// Private constructed variables

	private boolean isCheckingGrammarNeeded_ = false;

	private GrammarItem firstFeminineProperNounEndingGrammarItem_ = null;
	private GrammarItem firstMasculineProperNounEndingGrammarItem_ = null;
	private GrammarItem firstFeminineSingularNounEndingGrammarItem_ = null;
	private GrammarItem firstMasculineSingularNounEndingGrammarItem_ = null;
	private GrammarItem firstPluralNounEndingGrammarItem_ = null;
	private GrammarItem firstMergedWordGrammarItem_ = null;
	private GrammarItem firstChineseExclusiveNounGrammarItem_ = null;
	private GrammarItem firstSentenceGrammarItem_ = null;
	private GrammarItem firstStatementGrammarItem_ = null;
	private GrammarItem firstQuestionGrammarItem_ = null;


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
		return ( grammarParameter == Constants.WORD_FEMININE_PROPER_NOUN_ENDING ||
				grammarParameter == Constants.WORD_MASCULINE_PROPER_NOUN_ENDING ||
				grammarParameter == Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING ||
				grammarParameter == Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING ||
				grammarParameter == Constants.WORD_PLURAL_NOUN_ENDING ||
				grammarParameter == Constants.WORD_MERGED_WORD ||
				// Typical for Chinese
				grammarParameter == Constants.WORD_CHINESE_EXCLUSIVE_NOUN );
		}

	private GrammarItem firstActiveGrammarItem()
		{
		return (GrammarItem)firstActiveItem();
		}

	private GrammarItem firstWordEndingGrammarItem( short grammarParameter )
		{
		switch( grammarParameter )
			{
			case Constants.WORD_FEMININE_PROPER_NOUN_ENDING:
				return firstFeminineProperNounEndingGrammarItem_;

			case Constants.WORD_MASCULINE_PROPER_NOUN_ENDING:
				return firstMasculineProperNounEndingGrammarItem_;

			case Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING:
				return firstFeminineSingularNounEndingGrammarItem_;

			case Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING:
				return firstMasculineSingularNounEndingGrammarItem_;

			case Constants.WORD_PLURAL_NOUN_ENDING:
				return firstPluralNounEndingGrammarItem_;

			case Constants.WORD_MERGED_WORD:
				return firstMergedWordGrammarItem_;

			case Constants.WORD_CHINESE_EXCLUSIVE_NOUN:
				return firstChineseExclusiveNounGrammarItem_;
			}

		return null;
		}


	// Constructor

	protected GrammarList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_GRAMMAR_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void markAsChoiceEnd()
		{
		boolean hasFound = false;
		int currentSentenceItemNr = GlobalVariables.currentSentenceItemNr;
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		while( !hasFound &&
		searchGrammarItem != null )
			{
			if( searchGrammarItem.itemNr() == currentSentenceItemNr )
				{
				hasFound = true;
				searchGrammarItem.isChoiceEnd = true;
				}

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}
		}

	protected void markAsOptionEnd()
		{
		boolean hasFound = false;
		int currentSentenceItemNr = GlobalVariables.currentSentenceItemNr;
		GrammarItem searchGrammarItem = firstActiveGrammarItem();

		while( !hasFound &&
		searchGrammarItem != null )
			{
			if( searchGrammarItem.itemNr() == currentSentenceItemNr )
				{
				hasFound = true;
				searchGrammarItem.isOptionEnd = true;
				}

			searchGrammarItem = searchGrammarItem.nextGrammarItem();
			}
		}

	protected boolean hasMergedWords()
		{
		return ( firstMergedWordGrammarItem_ != null );
		}

	protected boolean isCheckingGrammarNeeded()
		{
		return isCheckingGrammarNeeded_;
		}

	protected byte checkForDuplicateGrammarDefinition()
		{
		boolean isIdentical;
		String definitionGrammarString;
		String grammarString;
		GrammarItem currentGrammarItem;
		GrammarItem definitionGrammarItem = null;
		GrammarItem duplicateDefinitionGrammarItem = null;
		GrammarItem duplicateGrammarItem;
		GrammarItem nextGrammarItem;
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
					if( definitionGrammarItem != null )
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
			return startError( 1, "I couldn't find the last grammar definition word" );

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
						currentGrammarItem = nextGrammarItem;

						nextGrammarItem = duplicateGrammarItem.nextGrammarItem();
						duplicateGrammarItem = nextGrammarItem;
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
					return startError( 1, "I found a duplicate grammar definition" );
				}
			while( ( duplicateDefinitionGrammarItem = duplicateDefinitionGrammarItem.nextDefinitionGrammarItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte checkGrammar()
		{
		short currentWordTypeNr = ( Constants.NUMBER_OF_WORD_TYPES - 1 );
		short grammarWordTypeNr;
		GrammarItem currentGrammarItem;
		WordItem currentLangugeWordItem = myWordItem();

		if( ( currentGrammarItem = firstActiveGrammarItem() ) == null )
			return startError( 1, "I couldn't find any grammar item" );

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
							if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) != Constants.RESULT_OK )
								return addError( 1, "I failed to write the 'grammar word type definition missing' interface notification" );

							currentWordTypeNr = grammarWordTypeNr;
							}
						else
							{
							if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START, ( grammarWordTypeNr + 1 ), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO, currentWordTypeNr, Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) != Constants.RESULT_OK )
								return addError( 1, "I failed to write the 'grammar word type definitions missing' interface notification" );

							currentWordTypeNr = grammarWordTypeNr;
							}
						}

					currentWordTypeNr--;
					}

				if( !currentGrammarItem.isGrammarItemInUse &&
				!currentGrammarItem.isGrammarStart() &&
				!currentGrammarItem.isUndefinedWord() &&
				!currentGrammarItem.isUserDefinedWord() &&
				!isWordEnding( currentGrammarItem.grammarParameter() ) &&

				InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_START, currentGrammarItem.grammarString(), Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_MIDDLE, currentLangugeWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_END ) != Constants.RESULT_OK )
					return addError( 1, "I failed to write the 'grammar definition is not used' interface notification" );
				}
			else
				{
				if( currentGrammarItem.definitionGrammarItem == null )
					return startError( 1, "Grammar word \"" + currentGrammarItem.grammarString() + "\" in " + currentLangugeWordItem.anyWordTypeString() + " is used, but not defined" );
				}
			}
		while( ( currentGrammarItem = currentGrammarItem.nextGrammarItem() ) != null );

		return Constants.RESULT_OK;
		}

	protected byte linkLaterDefinedGrammarWords()
		{
		String definitionGrammarString;
		String grammarString;
		GrammarItem currentGrammarItem;
		GrammarItem definitionGrammarItem;

		if( ( definitionGrammarItem = firstActiveGrammarItem() ) == null )
			return startError( 1, "I couldn't find any grammar item" );

		if( ( definitionGrammarString = definitionGrammarItem.grammarString() ) == null )
			return startError( 1, "The grammar string of the grammar definition word is undefined" );

		currentGrammarItem = definitionGrammarItem;

		while( ( currentGrammarItem = currentGrammarItem.nextGrammarItem() ) != null )
			{
			if( ( grammarString = currentGrammarItem.grammarString() ) == null )
				return startError( 1, "The grammar string of the grammar definition word is undefined" );

			if( currentGrammarItem.definitionGrammarItem == null &&
			!currentGrammarItem.isDefinitionStart() &&
			definitionGrammarString.equals( grammarString ) )
				{
				definitionGrammarItem.isGrammarItemInUse = true;
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
		GrammarItem expandMergedWordGrammarItem = firstMergedWordGrammarItem_;
		GrammarItem searchMergedWordGrammarItem;

		if( readUserSentenceStringBuffer == null )
			return startError( 1, "The given read user sentence string buffer is undefined" );

		GlobalVariables.readUserSentenceStringBuffer = readUserSentenceStringBuffer;

		if( ( readUserSentenceStringBufferLength = readUserSentenceStringBuffer.length() ) == 0 )
			return startError( 1, "The given read user sentence string buffer is empty" );

		if( expandMergedWordGrammarItem == null )
			return startError( 1, "No grammar merged word definition was found" );

		do	{
			if( expandMergedWordGrammarItem.isDefinitionStart() )
				{
				if( ( searchMergedWordGrammarItem = expandMergedWordGrammarItem.nextDefinitionGrammarItem ) == null )
					return startError( 1, "I found a merged word definition without replacing merged word definition" );

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
							GlobalVariables.readUserSentenceStringBuffer = new StringBuffer( readUserSentenceStringBuffer.substring( 0, startPosition ) + expandMergedWordGrammarItem.grammarString() + readUserSentenceStringBuffer.substring( startPosition + searchMergedWordGrammarItem.grammarString().length() ) );

							readUserSentenceStringBuffer = GlobalVariables.readUserSentenceStringBuffer;
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
		GrammarItem searchMergedWordGrammarItem = firstMergedWordGrammarItem_;
		GrammarItem shrinkMergedWordGrammarItem;
		StringBuffer writtenSentenceStringBuffer;

		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		( writtenSentenceStringBufferLength = writtenSentenceStringBuffer.length() ) == 0 )
			return startError( 1, "The write sentence string buffer is empty" );

		if( searchMergedWordGrammarItem == null )
			return startError( 1, "No grammar merged word definition was found" );

		do	{
			if( searchMergedWordGrammarItem.isDefinitionStart() )
				{
				if( ( shrinkMergedWordGrammarItem = searchMergedWordGrammarItem.nextDefinitionGrammarItem ) == null )
					return startError( 1, "I found a merged word definition without replacing merged word definition" );

				previousPosition = 0;
				startPosition = 0;

				// Search for merged word
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
							GlobalVariables.writtenSentenceStringBuffer = new StringBuffer( writtenSentenceStringBuffer.substring( 0, startPosition ) + shrinkMergedWordGrammarItem.grammarString() + writtenSentenceStringBuffer.substring( startPosition + searchMergedWordGrammarItem.grammarString().length() ) );

							writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer;
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

	protected GrammarItem firstReadingGrammarItem()
		{
		return firstSentenceGrammarItem_;
		}

	protected GrammarItem firstWritingGrammarItem( boolean isQuestion )
		{
		return ( isQuestion &&
				firstQuestionGrammarItem_ != null ?
				firstQuestionGrammarItem_ : firstStatementGrammarItem_ );
		}

	protected GrammarResultType createGrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short wordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem definitionGrammarItem )
		{
		GrammarItem createdGrammarItem;
		GrammarResultType grammarResult = new GrammarResultType();

		if( wordTypeNr < Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startGrammarResultError( 1, "The given word type number is out of bounds" );

		createdGrammarItem = new GrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem, this, myWordItem() );
		grammarResult.grammarItem = createdGrammarItem;

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, createdGrammarItem ) != Constants.RESULT_OK )
			return addGrammarResultError( 1, "I failed to add a grammar item" );

		isCheckingGrammarNeeded_ = true;

		if( createdGrammarItem.isDefinitionStart() )
			{
			switch( grammarParameter )
				{
				case Constants.WORD_FEMININE_PROPER_NOUN_ENDING:
					firstFeminineProperNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case Constants.WORD_MASCULINE_PROPER_NOUN_ENDING:
					firstMasculineProperNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING:
					firstFeminineSingularNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING:
					firstMasculineSingularNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case Constants.WORD_PLURAL_NOUN_ENDING:
					firstPluralNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case Constants.WORD_MERGED_WORD:
					firstMergedWordGrammarItem_ = createdGrammarItem;
					break;

				case Constants.WORD_CHINESE_EXCLUSIVE_NOUN:
					firstChineseExclusiveNounGrammarItem_ = createdGrammarItem;
					break;

				case Constants.GRAMMAR_SENTENCE:
					firstSentenceGrammarItem_ = createdGrammarItem;
					break;

				case Constants.GRAMMAR_STATEMENT:
					firstStatementGrammarItem_ = createdGrammarItem;
					break;

				case Constants.GRAMMAR_QUESTION:
					firstQuestionGrammarItem_ = createdGrammarItem;
					break;
				}
			}

		return grammarResult;
		}

	protected GrammarResultType findGrammar( boolean isIgnoringGrammarParameter, short grammarParameter, int grammarStringLength, String grammarString )
		{
		GrammarItem foundGrammarItem = null;
		GrammarItem searchGrammarItem = firstActiveGrammarItem();
		GrammarResultType grammarResult = new GrammarResultType();

		if( grammarString == null )
			return startGrammarResultError( 1, "The given grammar string is undefined" );

		do	{
			if( searchGrammarItem.grammarString() == null )
				return startGrammarResultError( 1, "I found a grammar word without grammar string" );

			if( searchGrammarItem.isDefinitionStart() &&

			( isIgnoringGrammarParameter ||
			searchGrammarItem.grammarParameter() == grammarParameter ) &&

			grammarStringLength == searchGrammarItem.grammarString().length() &&
			grammarString.startsWith( searchGrammarItem.grammarString() ) )
				foundGrammarItem = searchGrammarItem;
			}
		while( foundGrammarItem == null &&
		( searchGrammarItem = searchGrammarItem.nextGrammarItem() ) != null );

		grammarResult.grammarItem = foundGrammarItem;
		return grammarResult;
		}

	protected WordEndingResultType analyzeWordEnding( short grammarParameter, int searchWordStringLength, String searchWordString )
		{
		int chineseRadicalValue;
		int replacingWordStringLength;
		int wordEndingStringLength;
		int wordLengthDifference;
		String replacingWordEndingString;
		String wordEndingString;
		GrammarItem replacingWordEndingGrammarItem;
		GrammarItem searchGrammarItem;
		WordEndingResultType wordEndingResult = new WordEndingResultType();

		if( searchWordString == null )
			return startWordEndingResultError( 1, "The given search word string is undefined" );

		if( !isWordEnding( grammarParameter ) )
			return startWordEndingResultError( 1, "The given grammar parameter is no word ending parameter" );

		// The given type of word ending is defined for the current language
		if( ( searchGrammarItem = firstWordEndingGrammarItem( grammarParameter ) ) != null )
			{
			if( searchWordStringLength == 0 )
				searchWordStringLength = searchWordString.length();

			if( searchWordStringLength > 0 )
				{
				do	{
					if( searchGrammarItem.isDefinitionStart() &&
					( wordEndingString = searchGrammarItem.itemString() ) != null &&
					( wordEndingStringLength = wordEndingString.length() ) > 0 )
						{
						chineseRadicalValue = 0;
						replacingWordStringLength = 0;

						if( ( replacingWordEndingGrammarItem = searchGrammarItem.nextDefinitionGrammarItem ) == null )
							replacingWordEndingString = null;
						else
							{
							if( ( replacingWordEndingString = replacingWordEndingGrammarItem.grammarString() ) != null )
								{
								// Typical for Chinese
								if( Character.isDigit( wordEndingString.charAt( 0 ) ) )
									{
									try	{
										// Compare value of given radical with start value
										if( ( chineseRadicalValue = searchWordString.codePointAt( 0 ) ) >= Integer.parseInt( wordEndingString ) &&
										// Compare value of given radical with end value
										chineseRadicalValue <= Integer.parseInt( replacingWordEndingString ) )
											// Value of given radical is within range
											wordEndingResult.hasFoundWordEnding = true;
										}
									catch( NumberFormatException exception )
										{
										// Radical not found. Don't throw exception.
										}
									}
								else
									replacingWordStringLength = replacingWordEndingString.length();
								}
							}

						// Not Chinese radical
						if( chineseRadicalValue == 0 &&
						( wordLengthDifference = ( searchWordStringLength - wordEndingStringLength ) ) >= 0 &&
						searchWordString.substring( wordLengthDifference ).startsWith( wordEndingString ) )
							{
							wordEndingResult.hasFoundWordEnding = true;
							wordEndingResult.wordStringLength = ( wordLengthDifference + replacingWordStringLength );
							wordEndingResult.wordString = searchWordString.substring( 0, wordLengthDifference ) + ( replacingWordEndingString == null ? Constants.EMPTY_STRING : replacingWordEndingString );
							}
						}
					}
				while( !wordEndingResult.hasFoundWordEnding &&
				( searchGrammarItem = searchGrammarItem.nextWordEndingGrammarItem() ) != null );
				}
			}

		return wordEndingResult;
		}
	}

/*************************************************************************
 *	"The voice of the Lord echoes above the sea.
 *	The God of glory thunders.
 *	The Lord thunders over the mighty sea." (Psalm 29:3)
 *************************************************************************/
