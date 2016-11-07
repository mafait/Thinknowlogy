/*	Class:			ReadList
 *	Parent class:	List
 *	Purpose:		To temporarily store read items
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

#ifndef READLIST
#define READLIST 1
#include "List.h"
#include "ReadItem.cpp"
#include "ReadResultType.cpp"

class ReadList : private List
	{
	friend class AdminCleanup;
	friend class AdminItem;
	friend class AdminReadCreateWords;
	friend class AdminReadSentence;

	// Private constructed variables

	unsigned short lastActivatedWordOrderNr_;

	protected:
	// Constructor

	ReadList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		lastActivatedWordOrderNr_ = NO_ORDER_NR;

		initializeListVariables( ADMIN_READ_LIST_SYMBOL, "ReadList", commonVariables, myWordItem );
		}

	~ReadList()
		{
		ReadItem *deleteReadItem;
		ReadItem *searchReadItem = firstActiveReadItem();

		while( searchReadItem != NULL )
			{
			deleteReadItem = searchReadItem;
			searchReadItem = searchReadItem->nextReadItem();
			delete deleteReadItem;
			}

		searchReadItem = firstInactiveReadItem();

		while( searchReadItem != NULL )
			{
			deleteReadItem = searchReadItem;
			searchReadItem = searchReadItem->nextReadItem();
			delete deleteReadItem;
			}

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class ReadList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class ReadList has replaced items." );

		searchReadItem = (ReadItem *)firstDeletedItem();

		while( searchReadItem != NULL )
			{
			deleteReadItem = searchReadItem;
			searchReadItem = searchReadItem->nextReadItem();
			delete deleteReadItem;
			}
		}


	// Protected virtual functions

	bool isTemporaryList()
		{
		return true;
		}


	// Protected functions

	void clearLastActivatedWordOrderNr()
		{
		lastActivatedWordOrderNr_ = NO_ORDER_NR;
		}

	bool hasFoundReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, char *readString, WordItem *readWordItem )
		{
		ReadItem *searchReadItem = firstActiveReadItem();

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordOrderNr() == wordOrderNr &&
			searchReadItem->wordParameter() == wordParameter &&
			searchReadItem->wordTypeNr() == wordTypeNr &&

			( ( searchReadItem->readString == NULL &&
			readString == NULL ) ||

			( searchReadItem->readString != NULL &&
			readString != NULL &&
			strcmp( searchReadItem->readString, readString ) == 0 ) ) &&

			searchReadItem->readWordItem() == readWordItem )
				return true;

			searchReadItem = searchReadItem->nextReadItem();
			}

		return false;
		}

	bool isImperativeSentence()
		{
		unsigned short wordOrderNr;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		unsigned int nWords = 0;
		char *readWordString;
		char *writtenSentenceString;
		ReadItem *searchReadItem = firstActiveReadItem();
		ReadItem *startReadItem = NULL;

		while( searchReadItem != NULL )
			{
			if( ( nWords > 0 ||
			// Trigger
			searchReadItem->isSpecificationWord() ) &&

			// First appearance of new word
			( wordOrderNr = searchReadItem->wordOrderNr() ) > previousWordOrderNr )
				{
				nWords++;
				previousWordOrderNr = wordOrderNr;

				if( startReadItem == NULL )
					startReadItem = searchReadItem;
				}

			searchReadItem = searchReadItem->nextReadItem();
			}

		// Start creation of imperative sentence
		if( nWords > 2 )
			{
			previousWordOrderNr = NO_ORDER_NR;
			searchReadItem = startReadItem;
			strcpy( commonVariables()->writtenSentenceString, EMPTY_STRING );

			writtenSentenceString = commonVariables()->writtenSentenceString;

			while( searchReadItem != NULL )
				{
				if( ( wordOrderNr = searchReadItem->wordOrderNr() ) > previousWordOrderNr &&
				!searchReadItem->isText() &&
				( readWordString = searchReadItem->readWordTypeString() ) != NULL )
					{
					if( previousWordOrderNr > NO_ORDER_NR &&
					// End of string (colon, question mark, etc)
					searchReadItem->grammarParameter != GRAMMAR_SENTENCE )
						strcat( writtenSentenceString, SPACE_STRING );

					previousWordOrderNr = wordOrderNr;
					strcat( writtenSentenceString, readWordString );
					}

				searchReadItem = searchReadItem->nextReadItem();
				}

			return true;
			}

		return false;
		}

	ReadResultType createReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *readString, WordItem *readWordItem )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadItem";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startReadResultError( functionNameString, NULL, "The given read word type number is undefined or out of bounds" );

		if( ( readResult.createdReadItem = new ReadItem( wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem, commonVariables(), this, myWordItem() ) ) == NULL )
			return startReadResultError( functionNameString, NULL, "I failed to create a read item" );

		if( addItemToList( QUERY_ACTIVE_CHAR, readResult.createdReadItem ) != RESULT_OK )
			return addReadResultError( functionNameString, NULL, "I failed to add an active read item" );

		return readResult;
		}

	ReadResultType findMoreInterpretations()
		{
		ReadResultType readResult;
		ReadItem *activeReadItem = firstActiveReadItem();
		ReadItem *inactiveReadItem = firstInactiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMoreInterpretations";

		// Get last inactive item
		while( inactiveReadItem != NULL &&
		inactiveReadItem->nextReadItem() != NULL )
			inactiveReadItem = inactiveReadItem->nextReadItem();

		if( inactiveReadItem != NULL )
			{
			readResult.hasFoundMoreInterpretations = true;
			lastActivatedWordOrderNr_ = inactiveReadItem->wordOrderNr();

			if( activateItem( inactiveReadItem ) != RESULT_OK )
				return addReadResultError( functionNameString, NULL, "I failed to active an inactive item" );

			// Clear grammar parameters of all active read items
			while( activeReadItem != NULL )
				{
				activeReadItem->grammarParameter = NO_GRAMMAR_PARAMETER;
				activeReadItem = activeReadItem->nextReadItem();
				}
			}

		return readResult;
		}

	ReadResultType getNumberOfReadWordReferences( unsigned short wordTypeNr, WordItem *readWordItem )
		{
		ReadResultType readResult;
		ReadItem *searchReadItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "getNumberOfReadWordReferences";

		if( readWordItem == NULL )
			return startReadResultError( functionNameString, NULL, "The given read word is undefined" );

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordTypeNr() == wordTypeNr &&
			searchReadItem->readWordItem() == readWordItem )
				readResult.nReadWordReferences++;

			searchReadItem = searchReadItem->nextReadItem();
			}

		searchReadItem = firstInactiveReadItem();

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordTypeNr() == wordTypeNr &&
			searchReadItem->readWordItem() == readWordItem )
				readResult.nReadWordReferences++;

			searchReadItem = searchReadItem->nextReadItem();
			}

		return readResult;
		}

	ReadResultType selectMatchingWordType( unsigned short currentWordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr )
		{
		ReadResultType readResult;
		ReadItem *activeReadItem;
		ReadItem *currentReadItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "selectMatchingWordType";

		// Find current word position
		if( currentWordOrderNr > NO_ORDER_NR )
			{
			while( currentReadItem != NULL &&
			currentReadItem->wordOrderNr() <= currentWordOrderNr )
				currentReadItem = currentReadItem->nextReadItem();
			}

		if( ( activeReadItem = currentReadItem ) != NULL )
			{
			do	{
				if( currentReadItem != NULL )
					{
					if( currentReadItem->wordTypeNr() == wordTypeNr &&
					currentReadItem->wordParameter() == wordParameter )
						{
						readResult.hasFoundMatchingWordType = true;

						// Inactivate read items that don't match
						while( activeReadItem != currentReadItem )
							{
							if( inactivateItem( activeReadItem ) != RESULT_OK )
								return addReadResultError( functionNameString, NULL, "I failed to inactive an active item" );

							activeReadItem = nextReadListItem();
							}
						}
					else
						currentReadItem = currentReadItem->nextReadItem();
					}
				}
			while( !readResult.hasFoundMatchingWordType &&
			currentReadItem != NULL &&
			// Only check this word position
			currentReadItem->wordOrderNr() == currentWordOrderNr + 1 &&
			currentReadItem->wordOrderNr() > lastActivatedWordOrderNr_ );
			}

		return readResult;
		}

	ResultType activateInactiveReadWords( unsigned short wordOrderNr )
		{
		ReadItem *searchReadItem = firstInactiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "activateInactiveReadWords";

		while( searchReadItem != NULL )
			{
			if( !searchReadItem->isPreposition() &&
			searchReadItem->wordOrderNr() > wordOrderNr )
				{
				if( activateItem( searchReadItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to activate an inactive item" );

				searchReadItem = nextReadListItem();
				}
			else
				searchReadItem = searchReadItem->nextReadItem();
			}

		return RESULT_OK;
		}

	ResultType deleteReadItemsWithNonMatchingMultipleWordPart( unsigned short wordOrderNr, char *sentenceString )
		{
		ReadItem *searchReadItem = firstActiveReadItem();
		WordItem *readWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteReadItemsWithNonMatchingMultipleWordPart";

		if( sentenceString == NULL )
			return startError( functionNameString, NULL, "The given sentence string is undefined" );

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordOrderNr() == wordOrderNr &&
			( readWordItem = searchReadItem->readWordItem() ) != NULL )
				{
				if( readWordItem->isMultipleWord() &&
				// No matching multiple word parts
				readWordItem->matchingMultipleSingularNounWordParts( sentenceString ) == 0 )
					{
					if( deleteItem( searchReadItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to delete an active read item" );

					searchReadItem = nextReadListItem();
					}
				else
					searchReadItem = searchReadItem->nextReadItem();
				}
			else
				searchReadItem = searchReadItem->nextReadItem();
			}

		return RESULT_OK;
		}

	ResultType setGrammarParameter( bool isValid, unsigned short startWordOrderNr, unsigned short endWordOrderNr, GrammarItem *definitionGrammarItem )
		{
		bool hasFound = false;
		bool isMarked = true;
		unsigned short wordOrderNr;
		size_t definitionGrammarStringLength;
		char *definitionGrammarString;
		ReadItem *searchReadItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "setGrammarParameter";

		if( endWordOrderNr <= NO_ORDER_NR )
			return startError( functionNameString, NULL, "The given end word order number is undefined" );

		if( startWordOrderNr >= endWordOrderNr )
			return startError( functionNameString, NULL, "The given start word order number is equal or higher than the given end word order number" );

		if( definitionGrammarItem == NULL )
			return startError( functionNameString, NULL, "The given grammar definition word item is undefined" );

		if( isValid )
			{
			while( isMarked &&
			searchReadItem != NULL &&
			( wordOrderNr = searchReadItem->wordOrderNr() ) <= endWordOrderNr )
				{
				if( !searchReadItem->isMarkedBySetGrammarParameter &&
				wordOrderNr > startWordOrderNr &&
				wordOrderNr <= endWordOrderNr )
					isMarked = false;

				searchReadItem = searchReadItem->nextReadItem();
				}

			searchReadItem = firstActiveReadItem();
			}

		while( searchReadItem != NULL &&
		( wordOrderNr = searchReadItem->wordOrderNr() ) <= endWordOrderNr )
			{
			if( wordOrderNr > startWordOrderNr &&
			wordOrderNr <= endWordOrderNr )
				{
				hasFound = true;

				if( isValid )
					{
					if( isMarked ||
					definitionGrammarItem->grammarParameter() > searchReadItem->grammarParameter )
						{
						searchReadItem->isMarkedBySetGrammarParameter = false;
						searchReadItem->grammarParameter = definitionGrammarItem->grammarParameter();
						searchReadItem->definitionGrammarItem = definitionGrammarItem;

						if( searchReadItem->readString == NULL &&
						( definitionGrammarString = definitionGrammarItem->grammarString() ) != NULL )
							{
							if( searchReadItem->readString != NULL )
								strcpy( searchReadItem->readString, definitionGrammarString );
							else
								{
								if( ( definitionGrammarStringLength = strlen( definitionGrammarString ) ) >= MAX_WORD_LENGTH )
									return startError( functionNameString, NULL, "The grammar definition string is too long" );

								if( searchReadItem->readString != NULL )
									delete searchReadItem->readString;

								if( ( searchReadItem->readString = new char[definitionGrammarStringLength + 1] ) == NULL )
									return startError( functionNameString, NULL, "I failed to create a grammar string" );

								strcpy( searchReadItem->readString, definitionGrammarString );
								}
							}
						}
					}
				else
					searchReadItem->isMarkedBySetGrammarParameter = true;
				}

			searchReadItem = searchReadItem->nextReadItem();
			}

		if( !hasFound )
			return startError( functionNameString, NULL, "I couldn't find any item between the given word order numbers" );

		return RESULT_OK;
		}

	ReadItem *firstActiveReadItem()
		{
		return (ReadItem *)firstActiveItem();
		}

	ReadItem *firstInactiveReadItem()
		{
		return (ReadItem *)firstInactiveItem();
		}

	ReadItem *nextReadListItem()
		{
		return (ReadItem *)nextListItem();
		}
	};
#endif

/*************************************************************************
 *	"Everything he does reveals his glory and majesty.
 *	His righteousness never fails." (Psalm 111:3)
 *************************************************************************/
