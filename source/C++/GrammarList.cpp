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

#include "GrammarItem.cpp"
#include "List.h"
#include "Presentation.cpp"

class GrammarList : private List
	{
	friend class WordItem;

	// Private constructible variables

	bool isNeededToCheckGrammar_;

	GrammarItem *firstGrammarItem_;
	GrammarItem *firstFeminineProperNameEndingGrammarItem_;
	GrammarItem *firstMasculineProperNameEndingGrammarItem_;
	GrammarItem *firstFeminineSingularNounEndingGrammarItem_;
	GrammarItem *firstMasculineSingularNounEndingGrammarItem_;
	GrammarItem *firstPluralNounEndingGrammarItem_;
	GrammarItem *firstMergedWordGrammarItem_;


	// Private functions

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

	GrammarResultType comparePluralEndingOfWord( size_t searchWordStringLength, size_t replacingWordStringLength, char *searchWordString, char *searchWordEndingString, char *replacingWordEndingString )
		{
		GrammarResultType grammarResult;
		size_t tempWordLength;
		size_t searchWordEndingStringLength;
		char functionNameString[FUNCTION_NAME_LENGTH] = "comparePluralEndingOfWord";

		if( searchWordStringLength > 0 )
			{
			if( searchWordString != NULL )
				{
				if( searchWordEndingString != NULL )
					{
					searchWordEndingStringLength = strlen( searchWordEndingString );
					tempWordLength = ( searchWordStringLength - searchWordEndingStringLength );
					grammarResult.singularNounWordStringLength = ( searchWordStringLength + replacingWordStringLength - searchWordEndingStringLength );

					if( tempWordLength >= 0 &&
					grammarResult.singularNounWordStringLength > 0 )
						{
						if( strncmp( &searchWordString[tempWordLength], searchWordEndingString, searchWordEndingStringLength ) == 0 )
							{
							grammarResult.hasFoundWordEnding = true;

							strcpy( grammarResult.singularNounWordString, EMPTY_STRING );
							strncat( grammarResult.singularNounWordString, searchWordString, tempWordLength );

							if( replacingWordEndingString != NULL )
								strcat( grammarResult.singularNounWordString, replacingWordEndingString );
							}
						}
					}
				else
					startError( functionNameString, NULL, "The given search word ending string is undefined" );
				}
			else
				startError( functionNameString, NULL, "The given search word string is undefined" );
			}
		else
			startError( functionNameString, NULL, "The given search word string length is undefined" );

		grammarResult.result = commonVariables()->result;
		return grammarResult;
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


	protected:
	// Constructor / deconstructor

	GrammarList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		isNeededToCheckGrammar_ = false;

		firstGrammarItem_ = NULL;
		firstFeminineProperNameEndingGrammarItem_ = NULL;
		firstMasculineProperNameEndingGrammarItem_ = NULL;
		firstFeminineSingularNounEndingGrammarItem_ = NULL;
		firstMasculineSingularNounEndingGrammarItem_ = NULL;
		firstPluralNounEndingGrammarItem_ = NULL;
		firstMergedWordGrammarItem_ = NULL;

		initializeListVariables( WORD_GRAMMAR_LIST_SYMBOL, "GrammarList", commonVariables, myWordItem );
		}

	~GrammarList()
		{
		GrammarItem *deleteItem;
		GrammarItem *searchItem = firstActiveGrammarItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextGrammarItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class GrammarList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class GrammarList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class GrammarList has replaced items." );

		searchItem = (GrammarItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextGrammarItem();
			delete deleteItem;
			}
		}


	// Protected functions

	void markAsOptionEnd()
		{
		bool hasFound = false;
		unsigned int currentItemNr = commonVariables()->currentItemNr;
		GrammarItem *searchItem = firstActiveGrammarItem();

		while( searchItem != NULL &&
		!hasFound )
			{
			if( searchItem->hasCurrentCreationSentenceNr() &&
			searchItem->itemNr() == currentItemNr )
				{
				hasFound = true;
				searchItem->isOptionEnd = true;
				}

			searchItem = searchItem->nextGrammarItem();
			}
		}

	void markAsChoiceEnd( unsigned int choiceEndItemNr )
		{
		bool hasFound = false;
		GrammarItem *searchItem = firstActiveGrammarItem();

		while( searchItem != NULL &&
		!hasFound )
			{
			if( searchItem->hasCurrentCreationSentenceNr() &&
			searchItem->itemNr() == choiceEndItemNr )
				{
				hasFound = true;
				searchItem->isChoiceEnd = true;
				}

			searchItem = searchItem->nextGrammarItem();
			}
		}

	bool isNeededToCheckGrammar()
		{
		return isNeededToCheckGrammar_;
		}

	bool hasMergedWords()
		{
		return ( firstMergedWordGrammarItem_ != NULL );
		}

	GrammarResultType analyzeWordEnding( unsigned short grammarParameter, size_t searchWordStringLength, char *searchWordString )
		{
		GrammarResultType grammarResult;
		char *replacingWordEndingString = NULL;
		GrammarItem *replacingWordEndingGrammarItem;
		GrammarItem *searchItem = firstWordEndingGrammarItem( grammarParameter );
		char functionNameString[FUNCTION_NAME_LENGTH] = "analyzeWordEnding";

		if( isWordEnding( grammarParameter ) )
			{
			// The given type of word ending is defined for the current language
			if( searchItem != NULL )
				{
				if( searchWordString != NULL )
					{
					if( searchWordStringLength == 0 )
						searchWordStringLength = strlen( searchWordString );

					do	{
						if( searchItem->isDefinitionStart() )
							{
							replacingWordEndingGrammarItem = searchItem->nextDefinitionGrammarItem;
							replacingWordEndingString = ( replacingWordEndingGrammarItem == NULL ? NULL : replacingWordEndingGrammarItem->grammarString() );

							if( ( grammarResult = comparePluralEndingOfWord( searchWordStringLength, ( replacingWordEndingString == NULL ? 0 : strlen( replacingWordEndingString ) ), searchWordString, searchItem->itemString(), replacingWordEndingString ) ).result != RESULT_OK )
								addError( functionNameString, NULL, "I failed to find the plural ending of an undefined word type" );
							}
						}
					while( commonVariables()->result == RESULT_OK &&
					!grammarResult.hasFoundWordEnding &&
					( searchItem = searchItem->nextWordEndingGrammarItem() ) != NULL );
					}
				else
					startError( functionNameString, NULL, "The given search word string is undefined" );
				}
			}
		else
			startError( functionNameString, NULL, "The given grammar parameter is not a word ending parameter" );

		grammarResult.result = commonVariables()->result;
		return grammarResult;
		}

	GrammarResultType checkForDuplicateGrammarDefinition()
		{
		GrammarResultType grammarResult;
		bool isIdentical;
		char *grammarString;
		char *definitionGrammarString;
		GrammarItem *currentGrammarItem;
		GrammarItem *duplicateGrammarItem;
		GrammarItem *nextGrammarItem;
		GrammarItem *definitionGrammarItem = NULL;
		GrammarItem *duplicateDefinitionGrammarItem = NULL;
		GrammarItem *searchItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateGrammarDefinition";

		while( searchItem != NULL &&
		duplicateDefinitionGrammarItem == NULL )
			{
			if( searchItem->isDefinitionStart() )
				{
				if( searchItem->hasCurrentCreationSentenceNr() )
					definitionGrammarItem = searchItem;
				else
					{
					if( definitionGrammarItem != NULL &&
					searchItem->creationSentenceNr() < commonVariables()->currentSentenceNr )
						{
						grammarString = searchItem->grammarString();
						definitionGrammarString = definitionGrammarItem->grammarString();

						if( grammarString != NULL &&
						definitionGrammarString != NULL &&
						strcmp( definitionGrammarString, grammarString ) == 0 )
							{
							searchItem->isGrammarItemInUse = true;
							duplicateDefinitionGrammarItem = searchItem;
							definitionGrammarItem->nextDefinitionGrammarItem = searchItem;
							}
						}
					}
				}

			searchItem = searchItem->nextGrammarItem();
			}

		if( definitionGrammarItem != NULL )
			{
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
						startError( functionNameString, NULL, "I have found a duplicate grammar definition" );
					}
				while( commonVariables()->result == RESULT_OK &&
				( duplicateDefinitionGrammarItem = duplicateDefinitionGrammarItem->nextDefinitionGrammarItem ) != NULL );
				}
			}
		else
			startError( functionNameString, NULL, "I couldn't find the last grammar definition word" );

		grammarResult.result = commonVariables()->result;
		return grammarResult;
		}

	GrammarResultType createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem )
		{
		GrammarResultType grammarResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGrammarItem";

		if( wordTypeNr >= WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( commonVariables()->currentItemNr < MAX_ITEM_NR )
				{
				if( ( grammarResult.createdGrammarItem = new GrammarItem( isDefinitionStart, isNewStart, isOptionStart, isChoiceStart, isSkipOptionForWriting, wordTypeNr, grammarParameter, grammarStringLength, grammarString, definitionGrammarItem, commonVariables(), this, myWordItem() ) ) != NULL )
					{
					if( addItemToList( QUERY_ACTIVE_CHAR, grammarResult.createdGrammarItem ) == RESULT_OK )
						{
						isNeededToCheckGrammar_ = true;

						if( grammarResult.createdGrammarItem->isDefinitionStart() )
							{
							switch( grammarParameter )
								{
								case WORD_FEMININE_PROPER_NAME_ENDING:
									firstFeminineProperNameEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case WORD_MASCULINE_PROPER_NAME_ENDING:
									firstMasculineProperNameEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case WORD_FEMININE_SINGULAR_NOUN_ENDING:
									firstFeminineSingularNounEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case WORD_MASCULINE_SINGULAR_NOUN_ENDING:
									firstMasculineSingularNounEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case WORD_PLURAL_NOUN_ENDING:
									firstPluralNounEndingGrammarItem_ = grammarResult.createdGrammarItem;
									break;

								case WORD_MERGED_WORD:
									firstMergedWordGrammarItem_ = grammarResult.createdGrammarItem;
									break;
								}

							if( grammarResult.createdGrammarItem->isGrammarStart() )
								firstGrammarItem_ = grammarResult.createdGrammarItem;
							}
						}
					else
						addError( functionNameString, NULL, "I failed to add an active grammar item" );
					}
				else
					startError( functionNameString, NULL, "I failed to create a grammar item" );
				}
			else
				startError( functionNameString, NULL, "The current item number is undefined" );
			}
		else
			startError( functionNameString, NULL, "The given collected word type number is undefined or out of bounds" );

		grammarResult.result = commonVariables()->result;
		return grammarResult;
		}

	GrammarResultType findGrammarItem( bool ignoreValue, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString )
		{
		GrammarResultType grammarResult;
		GrammarItem *searchItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findGrammarItem";

		if( grammarString != NULL )
			{
			while( commonVariables()->result == RESULT_OK &&
			searchItem != NULL &&
			grammarResult.foundGrammarItem == NULL )
				{
				if( searchItem->grammarString() != NULL )
					{
					if( searchItem->isDefinitionStart() &&

					( ignoreValue ||
					searchItem->grammarParameter() == grammarParameter ) &&

					grammarStringLength == strlen( searchItem->grammarString() ) &&
					strncmp( grammarString, searchItem->grammarString(), grammarStringLength ) == 0 )
						grammarResult.foundGrammarItem = searchItem;
					else
						searchItem = searchItem->nextGrammarItem();
					}
				else
					startError( functionNameString, NULL, "I have found a grammar word without grammar string" );
				}
			}
		else
			startError( functionNameString, NULL, "The given grammar string is undefined" );

		grammarResult.result = commonVariables()->result;
		return grammarResult;
		}

	ResultType checkGrammar()
		{
		unsigned short grammarWordTypeNr;
		unsigned short currentWordTypeNr = ( NUMBER_OF_WORD_TYPES - 1 );
		GrammarItem *currentGrammarItem;
		WordItem *currentLangugeWordItem = myWordItem();
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammar";

		if( ( currentGrammarItem = firstActiveGrammarItem() ) != NULL )
			{
			isNeededToCheckGrammar_ = false;

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
								if( commonVariables()->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START, currentWordTypeNr, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE, currentLangugeWordItem->anyWordTypeString(), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( functionNameString, NULL, "I failed to write the 'grammar word type definition missing' interface notification" );
								}
							else
								{
								if( commonVariables()->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START, ( grammarWordTypeNr + 1 ), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO, currentWordTypeNr, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE, currentLangugeWordItem->anyWordTypeString(), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( functionNameString, NULL, "I failed to write the 'grammar word type definitions missing' interface notification" );
								}
							}

						currentWordTypeNr--;
						}

					if( !currentGrammarItem->isGrammarItemInUse &&
					!currentGrammarItem->isGrammarStart() &&
					!currentGrammarItem->isUndefinedWord() &&
					!currentGrammarItem->isUserDefinedWord() &&
					!isWordEnding( currentGrammarItem->grammarParameter() ) )
						{
						if( commonVariables()->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_START, currentGrammarItem->grammarString(), INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_MIDDLE, currentLangugeWordItem->anyWordTypeString(), INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_END ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to write the 'grammar definition is not used' interface notification" );
						}
					}
				else
					{
					if( currentGrammarItem->definitionGrammarItem == NULL )
						{
						sprintf( errorString, "Grammar word \"%s\" in \"%s\" is used, but not defined", currentGrammarItem->grammarString(), currentLangugeWordItem->anyWordTypeString() );
						return startError( functionNameString, NULL, errorString );
						}
					}
				}
			while( ( currentGrammarItem = currentGrammarItem->nextGrammarItem() ) != NULL );
			}
		else
			return startError( functionNameString, NULL, "I couldn't find any grammar item" );

		return RESULT_OK;
		}

	ResultType checkGrammarItemForUsage( GrammarItem *unusedGrammarItem )
		{
		GrammarItem *searchItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammarItemForUsage";

		if( unusedGrammarItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->definitionGrammarItem == unusedGrammarItem )
					return startError( functionNameString, NULL, "The definition grammar item is still in use" );

				if( searchItem->nextDefinitionGrammarItem == unusedGrammarItem )
					return startError( functionNameString, NULL, "The next definition grammar item is still in use" );

				searchItem = searchItem->nextGrammarItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given unused grammar item is undefined" );

		return RESULT_OK;
		}

	ResultType linkLaterDefinedGrammarWords()
		{
		char *definitionGrammarString;
		char *grammarString;
		GrammarItem *currentGrammarItem;
		GrammarItem *definitionGrammarItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "linkLaterDefinedGrammarWords";

		if( ( definitionGrammarItem = firstActiveGrammarItem() ) != NULL )
			{
			if( ( definitionGrammarString = definitionGrammarItem->grammarString() ) != NULL )
				{
				currentGrammarItem = definitionGrammarItem;

				while( ( currentGrammarItem = currentGrammarItem->nextGrammarItem() ) != NULL )
					{
					if( ( grammarString = currentGrammarItem->grammarString() ) != NULL )
						{
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
					else
						return startError( functionNameString, NULL, "The grammar string of the grammar definition word is undefined" );
					}
				}
			else
				return startError( functionNameString, NULL, "The grammar string of the grammar definition word is undefined" );
			}
		else
			return startError( functionNameString, NULL, "I couldn't find any grammar item" );

		return RESULT_OK;
		}

	ResultType expandMergedWordsInReadSentence( char *readUserSentenceString )
		{
		size_t previousPosition;
		size_t readUserSentenceStringLength;
		size_t startPosition;
		char *foundString;
		GrammarItem *searchMergedWordGrammarItem;
		GrammarItem *expandMergedWordGrammarItem = firstMergedWordGrammarItem_;
		char tempString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "expandMergedWordsInReadSentence";

		if( readUserSentenceString != NULL )
			{
			if( ( readUserSentenceStringLength = strlen( readUserSentenceString ) ) > 0 )
				{
				if( expandMergedWordGrammarItem != NULL )
					{
					do	{
						if( expandMergedWordGrammarItem->isDefinitionStart() )
							{
							if( ( searchMergedWordGrammarItem = expandMergedWordGrammarItem->nextDefinitionGrammarItem ) != NULL )
								{
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
							else
								return startError( functionNameString, NULL, "I found a compound word definition without replacing compound word definition" );
							}
						}
					while( ( expandMergedWordGrammarItem = expandMergedWordGrammarItem->nextWordEndingGrammarItem() ) != NULL );
					}
				else
					return startError( functionNameString, NULL, "No grammar compound word definition was found" );
				}
			else
				return startError( functionNameString, NULL, "The given read user sentence string is empty" );
			}
		else
			return startError( functionNameString, NULL, "The given read user sentence string is undefined" );

		return RESULT_OK;
		}

	ResultType shrinkMergedWordsInWriteSentence()
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

		if( ( writtenSentenceStringLength = strlen( writtenSentenceString ) ) > 0 )
			{
			if( searchMergedWordGrammarItem != NULL )
				{
				do	{
					if( searchMergedWordGrammarItem->isDefinitionStart() )
						{
						if( ( shrinkMergedWordGrammarItem = searchMergedWordGrammarItem->nextDefinitionGrammarItem ) != NULL )
							{
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
						else
							return startError( functionNameString, NULL, "I found a compound word definition without replacing compound word definition" );
						}
					}
				while( ( searchMergedWordGrammarItem = searchMergedWordGrammarItem->nextWordEndingGrammarItem() ) != NULL );
				}
			else
				return startError( functionNameString, NULL, "No grammar compound word definition was found" );
			}
		else
			return startError( functionNameString, NULL, "The write sentence string is empty" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		GrammarItem *searchItem = firstActiveGrammarItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeGrammarItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a grammar item in the database" );
				}

			searchItem = searchItem->nextGrammarItem();
			}

		return RESULT_OK;
		}
*/
	GrammarItem *firstGrammarItem()
		{
		return firstGrammarItem_;
		}

	char *grammarStringInList( unsigned short wordTypeNr )
		{
		GrammarItem *searchItem = firstActiveGrammarItem();

		while( searchItem != NULL )
			{
			if( searchItem->isDefinitionStart() &&
			searchItem->isNewStart() &&
			searchItem->isUserDefinedWord() &&
			searchItem->grammarWordTypeNr() == wordTypeNr )
				return searchItem->grammarString();

			searchItem = searchItem->nextGrammarItem();
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"The voice of the Lord echoes above the sea.
 *	The God of glory thunders.
 *	The Lord thunders over the mighty sea." (Psalm 29:3)
 *************************************************************************/
