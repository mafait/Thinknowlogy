/*
 *	Class:			GrammarList
 *	Parent class:	List
 *	Purpose:		To store grammar items
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

	bool isCheckingGrammarNeeded_;

	GrammarItem *firstWordEndingGrammarItem_;
	GrammarItem *startOfGrammarItem_;


	// Private functions

	GrammarResultType comparePluralEndingOfWord( size_t originalWordStringLength, size_t replacingWordStringLength, char *originalWordString, char *originalWordEndingString, char *replacingWordEndingString )
		{
		GrammarResultType grammarResult;
		size_t tempWordLength;
		size_t originalWordEndingStringLength;
		char functionNameString[FUNCTION_NAME_LENGTH] = "comparePluralEndingOfWord";

		if( originalWordStringLength > 0 )
			{
			if( originalWordString != NULL )
				{
				if( originalWordEndingString != NULL )
					{
					originalWordEndingStringLength = strlen( originalWordEndingString );
					tempWordLength = ( originalWordStringLength - originalWordEndingStringLength );
					grammarResult.singularNounWordStringLength = ( originalWordStringLength + replacingWordStringLength - originalWordEndingStringLength );

					if( tempWordLength >= 0 &&
					grammarResult.singularNounWordStringLength > 0 )
						{
						if( strncmp( &originalWordString[tempWordLength], originalWordEndingString, originalWordEndingStringLength ) == 0 )
							{
							grammarResult.hasFoundWordEnding = true;

							strcpy( grammarResult.singularNounWordString, originalWordString );
							grammarResult.singularNounWordString[tempWordLength] = NULL_CHAR;

							if( replacingWordEndingString != NULL )
								strcat( grammarResult.singularNounWordString, replacingWordEndingString );
							}
						}
					}
				else
					startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given original word ending string is undefined" );
				}
			else
				startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given original word string is undefined" );
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given original word string length is undefined" );

		grammarResult.result = commonVariables()->result;
		return grammarResult;
		}

	GrammarItem *firstWordEndingGrammarItem( unsigned short grammarParameter )
		{
		GrammarItem *searchItem = firstWordEndingGrammarItem_;

		while( searchItem != NULL )
			{
			if( searchItem->grammarParameter() == grammarParameter )
				return searchItem;

			searchItem = searchItem->nextGrammarItem();
			}

		return NULL;
		}


	protected:
	// Constructor / deconstructor

	GrammarList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		isCheckingGrammarNeeded_ = false;
		firstWordEndingGrammarItem_ = NULL;
		startOfGrammarItem_ = NULL;

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

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class GrammarList has archived items." );

		searchItem = (GrammarItem *)firstReplacedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextGrammarItem();
			delete deleteItem;
			}

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
		GrammarItem *searchItem = firstActiveGrammarItem();

		while( searchItem != NULL &&
		!hasFound )
			{
			if( searchItem->hasCurrentCreationSentenceNr() &&
			searchItem->itemNr() == commonVariables()->currentItemNr )
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

	bool isCheckingGrammarNeeded()
		{
		return isCheckingGrammarNeeded_;
		}

	GrammarResultType checkOnWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString )
		{
		GrammarResultType grammarResult;
		char *replacingWordEndingString = NULL;
		GrammarItem *originalWordEndingGrammarItem;
		GrammarItem *replacingWordEndingGrammarItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkOnWordEnding";

		if( grammarParameter > NO_GRAMMAR_PARAMETER )
			{
			if( originalWordString != NULL )
				{
				if( originalWordStringLength == 0 )
					originalWordStringLength = strlen( originalWordString );

				if( ( originalWordEndingGrammarItem = firstWordEndingGrammarItem( grammarParameter ) ) != NULL )
					{
					do	{
						if( originalWordEndingGrammarItem->isDefinitionStart() )
							{
							replacingWordEndingGrammarItem = originalWordEndingGrammarItem->nextDefinitionGrammarItem;
							replacingWordEndingString = ( replacingWordEndingGrammarItem == NULL ? NULL : replacingWordEndingGrammarItem->grammarString() );

							if( ( grammarResult = comparePluralEndingOfWord( originalWordStringLength, ( replacingWordEndingString == NULL ? 0 : strlen( replacingWordEndingString ) ), originalWordString, originalWordEndingGrammarItem->itemString(), replacingWordEndingString ) ).result != RESULT_OK )
								addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find the plural ending of an undefined word type" );
							}
						}
					while( commonVariables()->result == RESULT_OK &&
					!grammarResult.hasFoundWordEnding &&
					( originalWordEndingGrammarItem = originalWordEndingGrammarItem->nextWordEndingGrammarItem() ) != NULL );
					}
				}
			else
				startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given original word string is undefined" );
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given grammar parameter is undefined" );

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
						isCheckingGrammarNeeded_ = true;

						if( grammarResult.createdGrammarItem->isDefinitionStart() )
							{
							if( grammarResult.createdGrammarItem->isWordEnding() )
								firstWordEndingGrammarItem_ = grammarResult.createdGrammarItem;

							if( grammarResult.createdGrammarItem->isGrammarStart() )
								startOfGrammarItem_ = grammarResult.createdGrammarItem;
							}
						}
					else
						addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active grammar item" );
					}
				else
					startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to create a grammar item" );
				}
			else
				startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given collected word type number is undefined or out of bounds" );

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
			while( searchItem != NULL &&
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
					startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I've found a grammar word without grammar string" );
				}
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given grammar string is undefined" );

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
						startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I've found a duplicate grammar definition" );
					}
				while( commonVariables()->result == RESULT_OK &&
				( duplicateDefinitionGrammarItem = duplicateDefinitionGrammarItem->nextDefinitionGrammarItem ) != NULL );
				}
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find the last grammar definition word" );

		grammarResult.result = commonVariables()->result;
		return grammarResult;
		}

	ResultType checkGrammar()
		{
		unsigned short grammarWordTypeNr;
		unsigned short currentWordTypeNr = ( NUMBER_OF_WORD_TYPES - 1 );
		GrammarItem *currentGrammarItem;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammar";

		if( ( currentGrammarItem = firstActiveGrammarItem() ) != NULL )
			{
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
								if( commonVariables()->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_START, currentWordTypeNr, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_MIDDLE, myWordItem()->anyWordTypeString(), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an interface notification" );
								}
							else
								{
								if( commonVariables()->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_START, ( grammarWordTypeNr + 1 ), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_TO, currentWordTypeNr, INTERFACE_GRAMMAR_WORD_TYPE_DEFINITIONS_MISSING_MIDDLE, myWordItem()->anyWordTypeString(), INTERFACE_GRAMMAR_WORD_TYPE_DEFINITION_MISSING_END ) == RESULT_OK )
									currentWordTypeNr = grammarWordTypeNr;
								else
									return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an interface notification" );
								}
							}

						currentWordTypeNr--;
						}

					if( !currentGrammarItem->isGrammarItemInUse &&
					!currentGrammarItem->isGrammarStart() &&
					!currentGrammarItem->isUndefinedWord() &&
					!currentGrammarItem->isUserDefinedWord() &&
					!currentGrammarItem->isWordEnding() )
						{
						if( commonVariables()->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_START, currentGrammarItem->grammarString(), INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_MIDDLE, myWordItem()->anyWordTypeString(), INTERFACE_GRAMMAR_DEFINITION_IS_NOT_USED_END ) != RESULT_OK )
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an interface notification" );
						}
					}
				else
					{
					if( currentGrammarItem->definitionGrammarItem == NULL )
						{
						sprintf( errorString, "Grammar word \"%s\" in \"%s\" is used, but not defined", currentGrammarItem->grammarString(), myWordItem()->anyWordTypeString() );
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), errorString );
						}
					}
				}
			while( ( currentGrammarItem = currentGrammarItem->nextGrammarItem() ) != NULL );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find any grammar item" );

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
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The definition grammar item is still in use" );

				if( searchItem->nextDefinitionGrammarItem == unusedGrammarItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The next definition grammar item is still in use" );

				searchItem = searchItem->nextGrammarItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given unused grammar item is undefined" );

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
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The grammar string of the grammar definition word is undefined" );
					}
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The grammar string of the grammar definition word is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find any grammar item" );

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
					return addError( functionNameString, NULL, NULL, "I failed to store a grammar item in the database" );
				}

			searchItem = searchItem->nextGrammarItem();
			}

		searchItem = firstReplacedGrammarItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeGrammarItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced grammar item in the database" );
				}

			searchItem = searchItem->nextGrammarItem();
			}

		return RESULT_OK;
		}
*/
	GrammarItem *firstActiveGrammarItem()
		{
		return (GrammarItem *)firstActiveItem();
		}

	GrammarItem *startOfGrammarItem()
		{
		return startOfGrammarItem_;
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
