/*	Class:			GrammarList
 *	Parent class:	List
 *	Purpose:		To store grammar items
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#include "GrammarItem.cpp"
#include "InputOutput.cpp"
#include "List.h"
#include "WordEndingResultType.cpp"

class GrammarList : private List
	{
	friend class WordItem;

	// Private constructed variables

	bool isCheckingGrammarNeeded_;

	GrammarItem *firstGrammarItem_;
	GrammarItem *firstFeminineProperNameEndingGrammarItem_;
	GrammarItem *firstMasculineProperNameEndingGrammarItem_;
	GrammarItem *firstFeminineSingularNounEndingGrammarItem_;
	GrammarItem *firstMasculineSingularNounEndingGrammarItem_;
	GrammarItem *firstPluralNounEndingGrammarItem_;
	GrammarItem *firstMergedWordGrammarItem_;


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
		return ( grammarParameter == WORD_FEMININE_PROPER_NAME_ENDING ||
				grammarParameter == WORD_MASCULINE_PROPER_NAME_ENDING ||
				grammarParameter == WORD_FEMININE_SINGULAR_NOUN_ENDING ||
				grammarParameter == WORD_MASCULINE_SINGULAR_NOUN_ENDING ||
				grammarParameter == WORD_PLURAL_NOUN_ENDING ||
				grammarParameter == WORD_MERGED_WORD );
		}

	GrammarItem *firstActiveGrammarItem()
		{
		return (GrammarItem *)firstActiveItem();
		}

	GrammarItem *firstWordEndingGrammarItem( unsigned short grammarParameter )
		{
		switch( grammarParameter )
			{
			case WORD_FEMININE_PROPER_NAME_ENDING:
				return firstFeminineProperNameEndingGrammarItem_;

			case WORD_MASCULINE_PROPER_NAME_ENDING:
				return firstMasculineProperNameEndingGrammarItem_;

			case WORD_FEMININE_SINGULAR_NOUN_ENDING:
				return firstFeminineSingularNounEndingGrammarItem_;

			case WORD_MASCULINE_SINGULAR_NOUN_ENDING:
				return firstMasculineSingularNounEndingGrammarItem_;

			case WORD_PLURAL_NOUN_ENDING:
				return firstPluralNounEndingGrammarItem_;

			case WORD_MERGED_WORD:
				return firstMergedWordGrammarItem_;
			}

		return NULL;
		}

	WordEndingResultType comparePluralWordEnding( size_t searchWordStringLength, size_t replacingWordStringLength, char *searchWordString, char *searchWordEndingString, char *replacingWordEndingString )
		{
		WordEndingResultType wordEndingResult;
		size_t tempWordLength;
		size_t searchWordEndingStringLength;
		char functionNameString[FUNCTION_NAME_LENGTH] = "comparePluralWordEnding";

		if( searchWordStringLength <= 0 )
			return startWordEndingResultError( functionNameString, "The given search word string length is undefined" );

		if( searchWordString == NULL )
			return startWordEndingResultError( functionNameString, "The given search word string is undefined" );

		if( searchWordEndingString == NULL )
			return startWordEndingResultError( functionNameString, "The given search word ending string is undefined" );

		searchWordEndingStringLength = strlen( searchWordEndingString );
		tempWordLength = ( searchWordStringLength - searchWordEndingStringLength );
		wordEndingResult.singularNounWordStringLength = ( searchWordStringLength + replacingWordStringLength - searchWordEndingStringLength );

		if( tempWordLength >= 0 &&
		wordEndingResult.singularNounWordStringLength > 0 )
			{
			if( strncmp( &searchWordString[tempWordLength], searchWordEndingString, searchWordEndingStringLength ) == 0 )
				{
				wordEndingResult.hasFoundWordEnding = true;

				strcpy( wordEndingResult.singularNounWordString, EMPTY_STRING );
				strncat( wordEndingResult.singularNounWordString, searchWordString, tempWordLength );

				if( replacingWordEndingString != NULL )
					strcat( wordEndingResult.singularNounWordString, replacingWordEndingString );
				}
			}

		return wordEndingResult;
		}


	protected:
	// Constructor

	GrammarList( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		isCheckingGrammarNeeded_ = false;

		firstGrammarItem_ = NULL;
		firstFeminineProperNameEndingGrammarItem_ = NULL;
		firstMasculineProperNameEndingGrammarItem_ = NULL;
		firstFeminineSingularNounEndingGrammarItem_ = NULL;
		firstMasculineSingularNounEndingGrammarItem_ = NULL;
		firstPluralNounEndingGrammarItem_ = NULL;
		firstMergedWordGrammarItem_ = NULL;

		initializeListVariables( WORD_GRAMMAR_LIST_SYMBOL, "GrammarList", commonVariables, inputOutput, myWordItem );
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

	void markAsOptionEnd()
		{
		bool hasFound = false;
		unsigned int currentItemNr = commonVariables()->currentItemNr;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != NULL &&
		!hasFound )
			{
			if( searchGrammarItem->hasCurrentCreationSentenceNr() &&
			searchGrammarItem->itemNr() == currentItemNr )
				{
				hasFound = true;
				searchGrammarItem->isOptionEnd = true;
				}

			searchGrammarItem = searchGrammarItem->nextGrammarItem();
			}
		}

	void markAsChoiceEnd( unsigned int choiceEndItemNr )
		{
		bool hasFound = false;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();

		while( searchGrammarItem != NULL &&
		!hasFound )
			{
			if( searchGrammarItem->hasCurrentCreationSentenceNr() &&
			searchGrammarItem->itemNr() == choiceEndItemNr )
				{
				hasFound = true;
				searchGrammarItem->isChoiceEnd = true;
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
		char *grammarString;
		char *definitionGrammarString;
		GrammarItem *currentGrammarItem;
		GrammarItem *duplicateGrammarItem;
		GrammarItem *nextGrammarItem;
		GrammarItem *definitionGrammarItem = NULL;
		GrammarItem *duplicateDefinitionGrammarItem = NULL;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateGrammarDefinition";

		while( searchGrammarItem != NULL &&
		duplicateDefinitionGrammarItem == NULL )
			{
			if( searchGrammarItem->isDefinitionStart() )
				{
				if( searchGrammarItem->hasCurrentCreationSentenceNr() )
					definitionGrammarItem = searchGrammarItem;
				else
					{
					if( definitionGrammarItem != NULL &&
					searchGrammarItem->creationSentenceNr() < commonVariables()->currentSentenceNr )
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

						currentGrammarItem = ( nextGrammarItem != NULL &&
												nextGrammarItem->hasCurrentCreationSentenceNr() ? nextGrammarItem : NULL );

						nextGrammarItem = duplicateGrammarItem->nextGrammarItem();

						duplicateGrammarItem = ( nextGrammarItem != NULL &&
												nextGrammarItem->creationSentenceNr() == duplicateDefinitionGrammarItem->creationSentenceNr() ? nextGrammarItem : NULL );
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
		unsigned short grammarWordTypeNr;
		unsigned short currentWordTypeNr = ( NUMBER_OF_WORD_TYPES - 1 );
		GrammarItem *currentGrammarItem;
		WordItem *currentLangugeWordItem = myWordItem();
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammar";

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

	signed char checkGrammarItemForUsage( GrammarItem *unusedGrammarItem )
		{
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammarItemForUsage";

		if( unusedGrammarItem == NULL )
			return startError( functionNameString, "The given unused grammar item is undefined" );

		while( searchGrammarItem != NULL )
			{
			if( searchGrammarItem->definitionGrammarItem == unusedGrammarItem )
				return startError( functionNameString, "The definition grammar item is still in use" );

			if( searchGrammarItem->nextDefinitionGrammarItem == unusedGrammarItem )
				return startError( functionNameString, "The next definition grammar item is still in use" );

			searchGrammarItem = searchGrammarItem->nextGrammarItem();
			}

		return RESULT_OK;
		}

	signed char linkLaterDefinedGrammarWords()
		{
		char *definitionGrammarString;
		char *grammarString;
		GrammarItem *currentGrammarItem;
		GrammarItem *definitionGrammarItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "linkLaterDefinedGrammarWords";

		if( ( definitionGrammarItem = firstActiveGrammarItem() ) == NULL )
			return startError( functionNameString, "I couldn't find any grammar item" );

		if( ( definitionGrammarString = definitionGrammarItem->grammarString() ) == NULL )
			return startError( functionNameString, "The grammar string of the grammar definition word is undefined" );

		currentGrammarItem = definitionGrammarItem;

		while( ( currentGrammarItem = currentGrammarItem->nextGrammarItem() ) != NULL )
			{
			if( ( grammarString = currentGrammarItem->grammarString() ) == NULL )
				return startError( functionNameString, "The grammar string of the grammar definition word is undefined" );

			if( currentGrammarItem->definitionGrammarItem == NULL )
				{
				if( !currentGrammarItem->isDefinitionStart() &&
				strcmp( definitionGrammarString, grammarString ) == 0 )
					{
					definitionGrammarItem->isGrammarItemInUse = true;
					currentGrammarItem->definitionGrammarItem = definitionGrammarItem;
					}
				}
			else
				{
				if( currentGrammarItem->definitionGrammarItem == definitionGrammarItem->nextDefinitionGrammarItem )
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
		GrammarItem *searchMergedWordGrammarItem;
		GrammarItem *expandMergedWordGrammarItem = firstMergedWordGrammarItem_;
		char tempString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "expandMergedWordsInReadSentence";

		if( readUserSentenceString == NULL )
			return startError( functionNameString, "The given read user sentence string is undefined" );

		if( ( readUserSentenceStringLength = strlen( readUserSentenceString ) ) == 0 )
			return startError( functionNameString, "The given read user sentence string is empty" );

		if( expandMergedWordGrammarItem == NULL )
			return startError( functionNameString, "No grammar compound word definition was found" );

		do	{
			if( expandMergedWordGrammarItem->isDefinitionStart() )
				{
				if( ( searchMergedWordGrammarItem = expandMergedWordGrammarItem->nextDefinitionGrammarItem ) == NULL )
					return startError( functionNameString, "I found a compound word definition without replacing compound word definition" );

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
		char *writtenSentenceString = commonVariables()->writtenSentenceString;
		GrammarItem *shrinkMergedWordGrammarItem;
		GrammarItem *searchMergedWordGrammarItem = firstMergedWordGrammarItem_;
		char tempString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "shrinkMergedWordsInWriteSentence";

		if( ( writtenSentenceStringLength = strlen( writtenSentenceString ) ) == 0 )
			return startError( functionNameString, "The write sentence string is empty" );

		if( searchMergedWordGrammarItem == NULL )
			return startError( functionNameString, "No grammar compound word definition was found" );

		do	{
			if( searchMergedWordGrammarItem->isDefinitionStart() )
				{
				if( ( shrinkMergedWordGrammarItem = searchMergedWordGrammarItem->nextDefinitionGrammarItem ) == NULL )
					return startError( functionNameString, "I found a compound word definition without replacing compound word definition" );

				previousPosition = 0;
				startPosition = 0;

				// Search for compound word
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

	GrammarItem *firstGrammarItem()
		{
		return firstGrammarItem_;
		}

	GrammarResultType createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		GrammarResultType grammarResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGrammarItem";

		if( wordTypeNr < NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startGrammarResultError( functionNameString, "The given word type number is out of bounds" );

		if( ( grammarResult.grammarItem = new GrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem, commonVariables(), inputOutput(), this, myWordItem() ) ) == NULL )
			return startGrammarResultError( functionNameString, "I failed to create a grammar item" );

		if( addItemToList( QUERY_ACTIVE_CHAR, grammarResult.grammarItem ) != RESULT_OK )
			return addGrammarResultError( functionNameString, "I failed to add an active grammar item" );

		isCheckingGrammarNeeded_ = true;

		if( grammarResult.grammarItem->isDefinitionStart() )
			{
			switch( grammarParameter )
				{
				case WORD_FEMININE_PROPER_NAME_ENDING:
					firstFeminineProperNameEndingGrammarItem_ = grammarResult.grammarItem;
					break;

				case WORD_MASCULINE_PROPER_NAME_ENDING:
					firstMasculineProperNameEndingGrammarItem_ = grammarResult.grammarItem;
					break;

				case WORD_FEMININE_SINGULAR_NOUN_ENDING:
					firstFeminineSingularNounEndingGrammarItem_ = grammarResult.grammarItem;
					break;

				case WORD_MASCULINE_SINGULAR_NOUN_ENDING:
					firstMasculineSingularNounEndingGrammarItem_ = grammarResult.grammarItem;
					break;

				case WORD_PLURAL_NOUN_ENDING:
					firstPluralNounEndingGrammarItem_ = grammarResult.grammarItem;
					break;

				case WORD_MERGED_WORD:
					firstMergedWordGrammarItem_ = grammarResult.grammarItem;
					break;
				}

			if( grammarResult.grammarItem->isGrammarStart() )
				firstGrammarItem_ = grammarResult.grammarItem;
			}

		return grammarResult;
		}

	GrammarResultType findGrammar( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		GrammarResultType grammarResult;
		GrammarItem *foundGrammarItem = NULL;
		GrammarItem *searchGrammarItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findGrammar";

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
		WordEndingResultType wordEndingResult;
		char *replacingWordEndingString = NULL;
		GrammarItem *replacingWordEndingGrammarItem;
		GrammarItem *searchGrammarItem = firstWordEndingGrammarItem( grammarParameter );
		char functionNameString[FUNCTION_NAME_LENGTH] = "analyzeWordEnding";

		if( !isWordEnding( grammarParameter ) )
			return startWordEndingResultError( functionNameString, "The given grammar parameter is not a word ending parameter" );

		// The given type of word ending is defined for the current language
		if( searchGrammarItem != NULL )
			{
			if( searchWordString == NULL )
				return startWordEndingResultError( functionNameString, "The given search word string is undefined" );

			if( searchWordStringLength == 0 )
				searchWordStringLength = strlen( searchWordString );

			do	{
				if( searchGrammarItem->isDefinitionStart() )
					{
					replacingWordEndingGrammarItem = searchGrammarItem->nextDefinitionGrammarItem;
					replacingWordEndingString = ( replacingWordEndingGrammarItem == NULL ? NULL : replacingWordEndingGrammarItem->grammarString() );

					if( ( wordEndingResult = comparePluralWordEnding( searchWordStringLength, ( replacingWordEndingString == NULL ? 0 : strlen( replacingWordEndingString ) ), searchWordString, searchGrammarItem->itemString(), replacingWordEndingString ) ).result != RESULT_OK )
						return addWordEndingResultError( functionNameString, "I failed to find the plural ending of an undefined word type" );
					}
				}
			while( !wordEndingResult.hasFoundWordEnding &&
			( searchGrammarItem = searchGrammarItem->nextWordEndingGrammarItem() ) != NULL );
			}

		return wordEndingResult;
		}
	};

/*************************************************************************
 *	"The voice of the Lord echoes above the sea.
 *	The God of glory thunders.
 *	The Lord thunders over the mighty sea." (Psalm 29:3)
 *************************************************************************/
