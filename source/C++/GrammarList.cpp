/*	Class:			GrammarList
 *	Parent class:	List
 *	Purpose:		Storing grammar items
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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

#include "GrammarItem.cpp"
#include "InputOutput.cpp"
#include "List.h"
#include "WordEndingResultType.cpp"

class GrammarList : private List
	{
	friend class WordItem;

	// Private constructed variables

	bool isCheckingGrammarNeeded_ = false;

	GrammarItem *firstFeminineProperNounEndingGrammarItem_ = NULL;
	GrammarItem *firstMasculineProperNounEndingGrammarItem_ = NULL;
	GrammarItem *firstFeminineSingularNounEndingGrammarItem_ = NULL;
	GrammarItem *firstMasculineSingularNounEndingGrammarItem_ = NULL;
	GrammarItem *firstPluralNounEndingGrammarItem_ = NULL;
	GrammarItem *firstMergedWordGrammarItem_ = NULL;
	GrammarItem *firstChineseExclusiveNounGrammarItem_ = NULL;
	GrammarItem *firstSentenceGrammarItem_ = NULL;
	GrammarItem *firstStatementGrammarItem_ = NULL;
	GrammarItem *firstQuestionGrammarItem_ = NULL;


	// Private functions

	void deleteGrammarList( GrammarItem *searchGrammarItem )
		{
		GrammarItem *deleteGrammarItem;

		while( searchGrammarItem != NULL )
			{
			deleteGrammarItem = searchGrammarItem;
			searchGrammarItem = searchGrammarItem->nextGrammarItem();
			delete deleteGrammarItem;
			}
		}

	bool isText( size_t endPosition, char *sentenceString )
		{
		bool isText = false;
		size_t currentPosition = 0;

		if( sentenceString != NULL )
			{
			// Check for text string
			while( ++currentPosition < endPosition )
				{
				if( sentenceString[currentPosition] == SYMBOL_DOUBLE_QUOTE &&
				sentenceString[currentPosition - 1] != SYMBOL_BACK_SLASH )
					isText = !isText;
				}
			}

		return isText;
		}

	bool isWordEnding( unsigned short grammarParameter )
		{
		return ( grammarParameter == WORD_FEMININE_PROPER_NOUN_ENDING ||
				grammarParameter == WORD_MASCULINE_PROPER_NOUN_ENDING ||
				grammarParameter == WORD_FEMININE_SINGULAR_NOUN_ENDING ||
				grammarParameter == WORD_MASCULINE_SINGULAR_NOUN_ENDING ||
				grammarParameter == WORD_PLURAL_NOUN_ENDING ||
				grammarParameter == WORD_MERGED_WORD ||
				// Typical for Chinese
				grammarParameter == WORD_CHINESE_EXCLUSIVE_NOUN );
		}

	GrammarItem *firstActiveGrammarItem()
		{
		return (GrammarItem *)firstActiveItem();
		}

	GrammarItem *firstWordEndingGrammarItem( unsigned short grammarParameter )
		{
		switch( grammarParameter )
			{
			case WORD_FEMININE_PROPER_NOUN_ENDING:
				return firstFeminineProperNounEndingGrammarItem_;

			case WORD_MASCULINE_PROPER_NOUN_ENDING:
				return firstMasculineProperNounEndingGrammarItem_;

			case WORD_FEMININE_SINGULAR_NOUN_ENDING:
				return firstFeminineSingularNounEndingGrammarItem_;

			case WORD_MASCULINE_SINGULAR_NOUN_ENDING:
				return firstMasculineSingularNounEndingGrammarItem_;

			case WORD_PLURAL_NOUN_ENDING:
				return firstPluralNounEndingGrammarItem_;

			case WORD_MERGED_WORD:
				return firstMergedWordGrammarItem_;

			case WORD_CHINESE_EXCLUSIVE_NOUN:
				return firstChineseExclusiveNounGrammarItem_;
			}

		return NULL;
		}


	protected:
	// Constructor

	GrammarList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( WORD_GRAMMAR_LIST_SYMBOL, "GrammarList", globalVariables, inputOutput, myWordItem );
		}

	~GrammarList()
		{
		deleteGrammarList( firstActiveGrammarItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class GrammarList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class GrammarList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class GrammarList has replaced items." );

		deleteGrammarList( (GrammarItem *)firstDeletedItem() );
		}


	// Protected functions

	void markAsChoiceEnd()
		{
		bool hasFound = false;
		unsigned int currentSentenceItemNr = globalVariables()->currentSentenceItemNr;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();

		while( !hasFound &&
		searchGrammarItem != NULL )
			{
			if( searchGrammarItem->itemNr() == currentSentenceItemNr )
				{
				hasFound = true;
				searchGrammarItem->isChoiceEnd = true;
				}

			searchGrammarItem = searchGrammarItem->nextGrammarItem();
			}
		}

	void markAsOptionEnd()
		{
		bool hasFound = false;
		unsigned int currentSentenceItemNr = globalVariables()->currentSentenceItemNr;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();

		while( !hasFound &&
		searchGrammarItem != NULL )
			{
			if( searchGrammarItem->itemNr() == currentSentenceItemNr )
				{
				hasFound = true;
				searchGrammarItem->isOptionEnd = true;
				}

			searchGrammarItem = searchGrammarItem->nextGrammarItem();
			}
		}

	bool hasMergedWords()
		{
		return ( firstMergedWordGrammarItem_ != NULL );
		}

	bool isCheckingGrammarNeeded()
		{
		return isCheckingGrammarNeeded_;
		}

	signed char checkForDuplicateGrammarDefinition()
		{
		bool isIdentical;
		char *definitionGrammarString;
		char *grammarString;
		GrammarItem *currentGrammarItem;
		GrammarItem *definitionGrammarItem = NULL;
		GrammarItem *duplicateDefinitionGrammarItem = NULL;
		GrammarItem *duplicateGrammarItem;
		GrammarItem *nextGrammarItem;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDuplicateGrammarDefinition";

		while( searchGrammarItem != NULL &&
		duplicateDefinitionGrammarItem == NULL )
			{
			if( searchGrammarItem->isDefinitionStart() )
				{
				if( searchGrammarItem->hasCurrentCreationSentenceNr() )
					definitionGrammarItem = searchGrammarItem;
				else
					{
					if( definitionGrammarItem != NULL )
						{
						grammarString = searchGrammarItem->grammarString();
						definitionGrammarString = definitionGrammarItem->grammarString();

						if( grammarString != NULL &&
						definitionGrammarString != NULL &&
						strcmp( definitionGrammarString, grammarString ) == 0 )
							{
							searchGrammarItem->isGrammarItemInUse = true;
							duplicateDefinitionGrammarItem = searchGrammarItem;
							definitionGrammarItem->nextDefinitionGrammarItem = searchGrammarItem;
							}
						}
					}
				}

			searchGrammarItem = searchGrammarItem->nextGrammarItem();
			}

		if( definitionGrammarItem == NULL )
			return startError( functionNameString, "I couldn't find the last grammar definition word" );

		if( duplicateDefinitionGrammarItem != NULL )
			{
			do	{
				isIdentical = true;
				currentGrammarItem = definitionGrammarItem;
				duplicateGrammarItem = duplicateDefinitionGrammarItem;

				do	{
					if( currentGrammarItem->isIdentical( duplicateGrammarItem ) )
						{
						nextGrammarItem = currentGrammarItem->nextGrammarItem();
						currentGrammarItem = nextGrammarItem;

						nextGrammarItem = duplicateGrammarItem->nextGrammarItem();
						duplicateGrammarItem = nextGrammarItem;
						}
					else
						isIdentical = false;
					}
				while( isIdentical &&
				currentGrammarItem != NULL &&
				duplicateGrammarItem != NULL );

				if( isIdentical &&
				currentGrammarItem == NULL &&
				duplicateGrammarItem == NULL )
					return startError( functionNameString, "I found a duplicate grammar definition" );
				}
			while( ( duplicateDefinitionGrammarItem = duplicateDefinitionGrammarItem->nextDefinitionGrammarItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char checkGrammar()
		{
		unsigned short currentWordTypeNr = ( NUMBER_OF_WORD_TYPES - 1 );
		unsigned short grammarWordTypeNr;
		GrammarItem *currentGrammarItem;
		WordItem *currentLangugeWordItem = myWordItem();
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkGrammar";

		if( ( currentGrammarItem = firstActiveGrammarItem() ) == NULL )
			return startError( functionNameString, "I couldn't find any grammar item" );

		isCheckingGrammarNeeded_ = false;

		do	{
			if( currentGrammarItem->isDefinitionStart() )
				{
				if( currentGrammarItem->isNewStart() &&
				currentGrammarItem->isUserDefinedWord() &&
				( grammarWordTypeNr = currentGrammarItem->grammarWordTypeNr() ) <= currentWordTypeNr )
					{
					if( grammarWordTypeNr < currentWordTypeNr )
						{
						if( grammarWordTypeNr + 1 == currentWordTypeNr )
							{
							if( inputOutput()->writeInterfaceText( INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START, currentWordTypeNr, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE, currentLangugeWordItem->anyWordTypeString(), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) != RESULT_OK )
								return addError( functionNameString, "I failed to write the 'grammar word type definition missing' interface notification" );

							currentWordTypeNr = grammarWordTypeNr;
							}
						else
							{
							if( inputOutput()->writeInterfaceText( INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START, ( grammarWordTypeNr + 1 ), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO, currentWordTypeNr, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE, currentLangugeWordItem->anyWordTypeString(), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) != RESULT_OK )
								return addError( functionNameString, "I failed to write the 'grammar word type definitions missing' interface notification" );

							currentWordTypeNr = grammarWordTypeNr;
							}
						}

					currentWordTypeNr--;
					}

				if( !currentGrammarItem->isGrammarItemInUse &&
				!currentGrammarItem->isGrammarStart() &&
				!currentGrammarItem->isUndefinedWord() &&
				!currentGrammarItem->isUserDefinedWord() &&
				!isWordEnding( currentGrammarItem->grammarParameter() ) &&

				inputOutput()->writeInterfaceText( INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_START, currentGrammarItem->grammarString(), INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_MIDDLE, currentLangugeWordItem->anyWordTypeString(), INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_END ) != RESULT_OK )
					return addError( functionNameString, "I failed to write the 'grammar definition is not used' interface notification" );
				}
			else
				{
				if( currentGrammarItem->definitionGrammarItem == NULL )
					{
					sprintf( errorString, "Grammar word \"%s\" in \"%s\" is used, but not defined", currentGrammarItem->grammarString(), currentLangugeWordItem->anyWordTypeString() );
					return startError( functionNameString, errorString );
					}
				}
			}
		while( ( currentGrammarItem = currentGrammarItem->nextGrammarItem() ) != NULL );

		return RESULT_OK;
		}

	signed char linkLaterDefinedGrammarWords()
		{
		char *definitionGrammarString;
		char *grammarString;
		GrammarItem *currentGrammarItem;
		GrammarItem *definitionGrammarItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "linkLaterDefinedGrammarWords";

		if( ( definitionGrammarItem = firstActiveGrammarItem() ) == NULL )
			return startError( functionNameString, "I couldn't find any grammar item" );

		if( ( definitionGrammarString = definitionGrammarItem->grammarString() ) == NULL )
			return startError( functionNameString, "The grammar string of the grammar definition word is undefined" );

		currentGrammarItem = definitionGrammarItem;

		while( ( currentGrammarItem = currentGrammarItem->nextGrammarItem() ) != NULL )
			{
			if( ( grammarString = currentGrammarItem->grammarString() ) == NULL )
				return startError( functionNameString, "The grammar string of the grammar definition word is undefined" );

			if( currentGrammarItem->definitionGrammarItem == NULL &&
			!currentGrammarItem->isDefinitionStart() &&
			strcmp( definitionGrammarString, grammarString ) == 0 )
				{
				definitionGrammarItem->isGrammarItemInUse = true;
				currentGrammarItem->definitionGrammarItem = definitionGrammarItem;
				}
			}

		return RESULT_OK;
		}

	signed char expandMergedWordsInReadSentence( char *readUserSentenceString )
		{
		size_t previousPosition;
		size_t readUserSentenceStringLength;
		size_t startPosition;
		char *foundString;
		GrammarItem *expandMergedWordGrammarItem = firstMergedWordGrammarItem_;
		GrammarItem *searchMergedWordGrammarItem;
		char tempString[SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "expandMergedWordsInReadSentence";

		if( readUserSentenceString == NULL )
			return startError( functionNameString, "The given read user sentence string is undefined" );

		if( ( readUserSentenceStringLength = strlen( readUserSentenceString ) ) == 0 )
			return startError( functionNameString, "The given read user sentence string is empty" );

		if( expandMergedWordGrammarItem == NULL )
			return startError( functionNameString, "No grammar merged word definition was found" );

		do	{
			if( expandMergedWordGrammarItem->isDefinitionStart() )
				{
				if( ( searchMergedWordGrammarItem = expandMergedWordGrammarItem->nextDefinitionGrammarItem ) == NULL )
					return startError( functionNameString, "I found a merged word definition without replacing merged word definition" );

				previousPosition = 0;
				startPosition = 0;

				while( startPosition < readUserSentenceStringLength &&
				( foundString = strstr( &readUserSentenceString[startPosition], searchMergedWordGrammarItem->grammarString() ) ) != NULL )
					{
					startPosition += ( foundString - &readUserSentenceString[startPosition] );

					// First position,
					if( startPosition == 0 ||

					( startPosition > 0 &&
					// or is preceded by a space character
					isspace( readUserSentenceString[startPosition - 1] ) ) )
						{
						if( isText( ( startPosition - previousPosition ), &readUserSentenceString[previousPosition] ) )
							// Skip match in text string
							startPosition++;
						else
							{
							previousPosition = startPosition;

							strcpy( tempString, expandMergedWordGrammarItem->grammarString() );
							strcat( tempString, &readUserSentenceString[startPosition + strlen( searchMergedWordGrammarItem->grammarString() )] );
							strcpy( &readUserSentenceString[startPosition], tempString );

							readUserSentenceStringLength = strlen( readUserSentenceString );
							}
						}
					else
						// Skip match halfway word
						startPosition++;
					}
				}
			}
		while( ( expandMergedWordGrammarItem = expandMergedWordGrammarItem->nextWordEndingGrammarItem() ) != NULL );

		return RESULT_OK;
		}

	signed char shrinkMergedWordsInWriteSentence()
		{
		size_t previousPosition;
		size_t startPosition;
		size_t writtenSentenceStringLength;
		char *foundString;
		GrammarItem *searchMergedWordGrammarItem = firstMergedWordGrammarItem_;
		GrammarItem *shrinkMergedWordGrammarItem;
		char tempString[SENTENCE_STRING_LENGTH];
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "shrinkMergedWordsInWriteSentence";

		if( ( writtenSentenceString = globalVariables()->writtenSentenceString ) == NULL ||
		( writtenSentenceStringLength = strlen( writtenSentenceString ) ) == 0 )
			return startError( functionNameString, "The write sentence string is empty" );

		if( searchMergedWordGrammarItem == NULL )
			return startError( functionNameString, "No grammar merged word definition was found" );

		do	{
			if( searchMergedWordGrammarItem->isDefinitionStart() )
				{
				if( ( shrinkMergedWordGrammarItem = searchMergedWordGrammarItem->nextDefinitionGrammarItem ) == NULL )
					return startError( functionNameString, "I found a merged word definition without replacing merged word definition" );

				previousPosition = 0;
				startPosition = 0;

				// Search for merged word
				while( startPosition < writtenSentenceStringLength &&
				( foundString = strstr( &writtenSentenceString[startPosition], searchMergedWordGrammarItem->grammarString() ) ) != NULL )
					{
					startPosition += ( foundString - &writtenSentenceString[startPosition] );

					// First position,
					if( startPosition == 0 ||

					( startPosition > 0 &&
					// or is preceded by a space character
					isspace( writtenSentenceString[startPosition - 1] ) ) )
						{
						if( isText( ( startPosition - previousPosition ), &writtenSentenceString[previousPosition] ) )
							// Skip match in text string
							startPosition++;
						else
							{
							previousPosition = startPosition;

							strcpy( tempString, shrinkMergedWordGrammarItem->grammarString() );
							strcat( tempString, &writtenSentenceString[startPosition + strlen( searchMergedWordGrammarItem->grammarString() )] );
							strcpy( &writtenSentenceString[startPosition], tempString );

							writtenSentenceStringLength = strlen( writtenSentenceString );
							}
						}
					else
						// Skip match halfway word
						startPosition++;
					}
				}
			}
		while( ( searchMergedWordGrammarItem = searchMergedWordGrammarItem->nextWordEndingGrammarItem() ) != NULL );

		return RESULT_OK;
		}

	char *grammarStringInList( unsigned short wordTypeNr )
		{
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != NULL )
			{
			if( searchGrammarItem->isDefinitionStart() &&
			searchGrammarItem->isNewStart() &&
			searchGrammarItem->isUserDefinedWord() &&
			searchGrammarItem->grammarWordTypeNr() == wordTypeNr )
				return searchGrammarItem->grammarString();

			searchGrammarItem = searchGrammarItem->nextGrammarItem();
			}

		return NULL;
		}

	GrammarItem *firstReadingGrammarItem()
		{
		return firstSentenceGrammarItem_;
		}

	GrammarItem *firstWritingGrammarItem( bool isQuestion )
		{
		return ( isQuestion &&
				firstQuestionGrammarItem_ != NULL ?
				firstQuestionGrammarItem_ : firstStatementGrammarItem_ );
		}

	GrammarResultType createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		GrammarItem *createdGrammarItem;
		GrammarResultType grammarResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createGrammarItem";

		if( wordTypeNr < NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startGrammarResultError( functionNameString, "The given word type number is out of bounds" );

		createdGrammarItem = new GrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem, globalVariables(), inputOutput(), this, myWordItem() );
		grammarResult.grammarItem = createdGrammarItem;

		if( addItemToList( QUERY_ACTIVE_CHAR, createdGrammarItem ) != RESULT_OK )
			return addGrammarResultError( functionNameString, "I failed to add a grammar item" );

		isCheckingGrammarNeeded_ = true;

		if( createdGrammarItem->isDefinitionStart() )
			{
			switch( grammarParameter )
				{
				case WORD_FEMININE_PROPER_NOUN_ENDING:
					firstFeminineProperNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case WORD_MASCULINE_PROPER_NOUN_ENDING:
					firstMasculineProperNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case WORD_FEMININE_SINGULAR_NOUN_ENDING:
					firstFeminineSingularNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case WORD_MASCULINE_SINGULAR_NOUN_ENDING:
					firstMasculineSingularNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case WORD_PLURAL_NOUN_ENDING:
					firstPluralNounEndingGrammarItem_ = createdGrammarItem;
					break;

				case WORD_MERGED_WORD:
					firstMergedWordGrammarItem_ = createdGrammarItem;
					break;

				case WORD_CHINESE_EXCLUSIVE_NOUN:
					firstChineseExclusiveNounGrammarItem_ = createdGrammarItem;
					break;

				case GRAMMAR_SENTENCE:
					firstSentenceGrammarItem_ = createdGrammarItem;
					break;

				case GRAMMAR_STATEMENT:
					firstStatementGrammarItem_ = createdGrammarItem;
					break;

				case GRAMMAR_QUESTION:
					firstQuestionGrammarItem_ = createdGrammarItem;
					break;
				}
			}

		return grammarResult;
		}

	GrammarResultType findGrammar( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		GrammarItem *foundGrammarItem = NULL;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();
		GrammarResultType grammarResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findGrammar";

		if( grammarString == NULL )
			return startGrammarResultError( functionNameString, "The given grammar string is undefined" );

		do	{
			if( searchGrammarItem->grammarString() == NULL )
				return startGrammarResultError( functionNameString, "I found a grammar word without grammar string" );

			if( searchGrammarItem->isDefinitionStart() &&

			( isIgnoringGrammarParameter ||
			searchGrammarItem->grammarParameter() == grammarParameter ) &&

			grammarStringLength == strlen( searchGrammarItem->grammarString() ) &&
			strncmp( grammarString, searchGrammarItem->grammarString(), grammarStringLength ) == 0 )
				foundGrammarItem = searchGrammarItem;
			}
		while( foundGrammarItem == NULL &&
		( searchGrammarItem = searchGrammarItem->nextGrammarItem() ) != NULL );

		grammarResult.grammarItem = foundGrammarItem;
		return grammarResult;
		}

	WordEndingResultType analyzeWordEnding( unsigned short grammarParameter, size_t searchWordStringLength, char *searchWordString )
		{
		size_t replacingWordStringLength;
		size_t wordEndingStringLength;
		size_t wordLengthDifference;
		char *replacingWordEndingString;
		char *wordEndingString;
		GrammarItem *replacingWordEndingGrammarItem;
		GrammarItem *searchGrammarItem;
		WordEndingResultType wordEndingResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "analyzeWordEnding";

		if( searchWordString == NULL )
			return startWordEndingResultError( functionNameString, "The given search word string is undefined" );

		if( !isWordEnding( grammarParameter ) )
			return startWordEndingResultError( functionNameString, "The given grammar parameter is no word ending parameter" );

		// The given type of word ending is defined for the current language
		if( ( searchGrammarItem = firstWordEndingGrammarItem( grammarParameter ) ) != NULL )
			{
			if( searchWordStringLength == 0 )
				searchWordStringLength = strlen( searchWordString );

			if( searchWordStringLength > 0 )
				{
				do	{
					if( searchGrammarItem->isDefinitionStart() &&
					( wordEndingString = searchGrammarItem->itemString() ) != NULL &&
					( wordEndingStringLength = strlen( wordEndingString ) ) > 0 )
						{
						replacingWordStringLength = 0;

						if( ( replacingWordEndingGrammarItem = searchGrammarItem->nextDefinitionGrammarItem ) == NULL )
							replacingWordEndingString = NULL;
						else
							{
							if( ( replacingWordEndingString = replacingWordEndingGrammarItem->grammarString() ) != NULL )
								replacingWordStringLength = strlen( replacingWordEndingString );
							}

						if( ( wordLengthDifference = ( searchWordStringLength - wordEndingStringLength ) ) >= 0 &&
						strncmp( &searchWordString[wordLengthDifference], wordEndingString, wordEndingStringLength ) == 0 )
							{
							wordEndingResult.hasFoundWordEnding = true;
							wordEndingResult.wordStringLength = ( wordLengthDifference + replacingWordStringLength );

							strcpy( wordEndingResult.wordString, EMPTY_STRING );
							strncat( wordEndingResult.wordString, searchWordString, wordLengthDifference );

							if( replacingWordEndingString != NULL )
								strcat( wordEndingResult.wordString, replacingWordEndingString );
							}
						}
					}
				while( !wordEndingResult.hasFoundWordEnding &&
				( searchGrammarItem = searchGrammarItem->nextWordEndingGrammarItem() ) != NULL );
				}
			}

		return wordEndingResult;
		}
	};

/*************************************************************************
 *	"The voice of the Lord echoes above the sea.
 *	The God of glory thunders.
 *	The Lord thunders over the mighty sea." (Psalm 29:3)
 *************************************************************************/
