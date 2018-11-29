/*	Class:			ReadList
 *	Parent class:	List
 *	Purpose:		To temporarily store read items
 *	Version:		Thinknowlogy 2018r4 (New Science)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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
#include "ReadWordResultType.cpp"
#include "ShortResultType.cpp"

class ReadList : private List
	{
	friend class AdminItem;

	// Private constructed variables

	unsigned short lastActivatedWordOrderNr_ = NO_ORDER_NR;

	// Private functions

	void deleteReadList( ReadItem *searchReadItem )
		{
		ReadItem *deleteReadItem;

		while( searchReadItem != NULL )
			{
			deleteReadItem = searchReadItem;
			searchReadItem = searchReadItem->nextReadItem();
			delete deleteReadItem;
			}
		}

	bool hasFoundReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, WordItem *readWordItem )
		{
		ReadItem *searchReadItem = firstActiveReadItem();

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordOrderNr() == wordOrderNr &&
			searchReadItem->wordParameter() == wordParameter &&
			searchReadItem->wordTypeNr() == wordTypeNr &&
			searchReadItem->readWordItem() == readWordItem )
				return true;

			searchReadItem = searchReadItem->nextReadItem();
			}

		return false;
		}

	protected:
	// Constructor

	ReadList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( ADMIN_READ_LIST_SYMBOL, "ReadList", globalVariables, inputOutput, myWordItem );
		}

	~ReadList()
		{
		deleteReadList( firstActiveReadItem() );
		deleteReadList( firstInactiveReadItem() );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class ReadList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class ReadList has replaced items." );

		deleteReadList( (ReadItem *)firstDeletedItem() );
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

	signed char activateInactiveReadWords( unsigned short wordOrderNr )
		{
		ReadItem *searchReadItem = firstInactiveReadItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "activateInactiveReadWords";

		while( searchReadItem != NULL )
			{
			if( !searchReadItem->isPreposition() &&
			searchReadItem->wordOrderNr() > wordOrderNr )
				{
				if( activateItem( searchReadItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to activate an inactive item" );

				searchReadItem = nextReadListItem();
				}
			else
				searchReadItem = searchReadItem->nextReadItem();
			}

		return RESULT_OK;
		}

	signed char createReadItem( bool isUncountableGeneralizationNoun, unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *readString, WordItem *readWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createReadItem";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given read word type number is undefined or out of bounds" );

		if( hasFoundReadItem( wordOrderNr, wordParameter, wordTypeNr, readWordItem ) )
			return startError( functionNameString, "The given read item already exists" );

		if( addItemToList( QUERY_ACTIVE_CHAR, new ReadItem( isUncountableGeneralizationNoun, wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add a read item" );

		return RESULT_OK;
		}

	signed char deleteReadItemsWithNonMatchingMultipleWordPart( unsigned short wordOrderNr, char *sentenceString )
		{
		ReadItem *searchReadItem = firstActiveReadItem();
		WordItem *readWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteReadItemsWithNonMatchingMultipleWordPart";

		if( sentenceString == NULL )
			return startError( functionNameString, "The given sentence string is undefined" );

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordOrderNr() == wordOrderNr &&
			( readWordItem = searchReadItem->readWordItem() ) != NULL )
				{
				if( readWordItem->isMultipleWord() &&
				// No matching multiple word parts
				readWordItem->matchingMultipleWordParts( sentenceString ) == 0 )
					{
					if( deleteItem( searchReadItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete a read item" );

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

	signed char setGrammarParameter( bool isValid, unsigned short startWordOrderNr, unsigned short endWordOrderNr, GrammarItem *definitionGrammarItem )
		{
		bool hasFound = false;
		bool isMarked = true;
		unsigned short wordOrderNr;
		size_t definitionGrammarStringLength;
		char *definitionGrammarString;
		ReadItem *searchReadItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setGrammarParameter";

		if( endWordOrderNr <= NO_ORDER_NR )
			return startError( functionNameString, "The given end word order number is undefined" );

		if( startWordOrderNr >= endWordOrderNr )
			return startError( functionNameString, "The given start word order number is equal or higher than the given end word order number" );

		if( definitionGrammarItem == NULL )
			return startError( functionNameString, "The given grammar definition word item is undefined" );

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
							if( ( definitionGrammarStringLength = strlen( definitionGrammarString ) ) >= MAX_WORD_LENGTH )
								return startError( functionNameString, "The grammar definition string is too long" );

							if( searchReadItem->readString != NULL )
								delete searchReadItem->readString;

							if( ( searchReadItem->readString = new char[definitionGrammarStringLength + 1] ) == NULL )
								return startError( functionNameString, "I failed to create a grammar string" );

							strcpy( searchReadItem->readString, definitionGrammarString );
							}
						}
					}
				else
					searchReadItem->isMarkedBySetGrammarParameter = true;
				}

			searchReadItem = searchReadItem->nextReadItem();
			}

		if( !hasFound )
			return startError( functionNameString, "I couldn't find any item between the given word order numbers" );

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

	BoolResultType findMoreInterpretations()
		{
		ReadItem *activeReadItem = firstActiveReadItem();
		ReadItem *inactiveReadItem = firstInactiveReadItem();
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findMoreInterpretations";

		// Get last inactive item
		while( inactiveReadItem != NULL &&
		inactiveReadItem->nextReadItem() != NULL )
			inactiveReadItem = inactiveReadItem->nextReadItem();

		if( inactiveReadItem != NULL )
			{
			// Has found another interpretation
			boolResult.booleanValue = true;
			lastActivatedWordOrderNr_ = inactiveReadItem->wordOrderNr();

			if( activateItem( inactiveReadItem ) != RESULT_OK )
				return addBoolResultError( functionNameString, "I failed to active an inactive item" );

			// Clear grammar parameters of all active read items
			while( activeReadItem != NULL )
				{
				activeReadItem->grammarParameter = NO_GRAMMAR_PARAMETER;
				activeReadItem = activeReadItem->nextReadItem();
				}
			}

		return boolResult;
		}

	BoolResultType selectMatchingWordType( unsigned short currentWordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr )
		{
		bool hasFoundMatchingWordType = false;
		ReadItem *activeReadItem;
		ReadItem *currentReadItem = firstActiveReadItem();
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "selectMatchingWordType";

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
						hasFoundMatchingWordType = true;

						// Inactivate read items that don't match
						while( activeReadItem != currentReadItem )
							{
							if( inactivateItem( activeReadItem ) != RESULT_OK )
								return addBoolResultError( functionNameString, "I failed to inactive an active item" );

							activeReadItem = nextReadListItem();
							}
						}
					else
						currentReadItem = currentReadItem->nextReadItem();
					}
				}
			while( !hasFoundMatchingWordType &&
			currentReadItem != NULL &&
			// Only check this word position
			currentReadItem->wordOrderNr() == currentWordOrderNr + 1 &&
			currentReadItem->wordOrderNr() > lastActivatedWordOrderNr_ );
			}

		boolResult.booleanValue = hasFoundMatchingWordType;
		return boolResult;
		}

	ShortResultType getNumberOfReadWordReferences( unsigned short wordTypeNr, WordItem *readWordItem )
		{
		unsigned short nReadWordReferences = 0;
		ReadItem *searchReadItem = firstActiveReadItem();
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getNumberOfReadWordReferences";

		if( readWordItem == NULL )
			return startShortResultError( functionNameString, "The given read word is undefined" );

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordTypeNr() == wordTypeNr &&
			searchReadItem->readWordItem() == readWordItem )
				nReadWordReferences++;

			searchReadItem = searchReadItem->nextReadItem();
			}

		searchReadItem = firstInactiveReadItem();

		while( searchReadItem != NULL )
			{
			if( searchReadItem->wordTypeNr() == wordTypeNr &&
			searchReadItem->readWordItem() == readWordItem )
				nReadWordReferences++;

			searchReadItem = searchReadItem->nextReadItem();
			}

		shortResult.shortValue = nReadWordReferences;
		return shortResult;
		}
	};
#endif

/*************************************************************************
 *	"Everything he does reveals his glory and majesty.
 *	His righteousness never fails." (Psalm 111:3)
 *************************************************************************/
