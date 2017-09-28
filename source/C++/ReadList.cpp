/*	Class:			ReadList
 *	Parent class:	List
 *	Purpose:		To temporarily store read items
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
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

	unsigned short lastActivatedWordOrderNr_;

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

	protected:
	// Constructor

	ReadList( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		lastActivatedWordOrderNr_ = NO_ORDER_NR;

		initializeListVariables( ADMIN_READ_LIST_SYMBOL, "ReadList", commonVariables, inputOutput, myWordItem );
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

	bool isImperativeSentence()
		{
		unsigned short wordOrderNr;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		unsigned int nWords = 0;
		char *readWordString;
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

			while( searchReadItem != NULL )
				{
				if( ( wordOrderNr = searchReadItem->wordOrderNr() ) > previousWordOrderNr &&
				!searchReadItem->isText() &&
				( readWordString = searchReadItem->readWordTypeString() ) != NULL )
					{
					if( previousWordOrderNr > NO_ORDER_NR &&
					// End of string (colon, question mark, etc)
					searchReadItem->grammarParameter != GRAMMAR_SENTENCE )
						strcat( commonVariables()->writtenSentenceString, SPACE_STRING );

					previousWordOrderNr = wordOrderNr;
					strcat( commonVariables()->writtenSentenceString, readWordString );
					}

				searchReadItem = searchReadItem->nextReadItem();
				}

			return true;
			}

		return false;
		}

	signed char activateInactiveReadWords( unsigned short wordOrderNr )
		{
		ReadItem *searchReadItem = firstInactiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "activateInactiveReadWords";

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

	signed char createReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *readString, WordItem *readWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadItem";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given read word type number is undefined or out of bounds" );

		if( hasFoundReadItem( wordOrderNr, wordParameter, wordTypeNr, readString, readWordItem ) )
			return startError( functionNameString, "The given read item already exists" );

		if( addItemToList( QUERY_ACTIVE_CHAR, new ReadItem( wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem, commonVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an active read item" );

		return RESULT_OK;
		}

	signed char deleteReadItemsWithNonMatchingMultipleWordPart( unsigned short wordOrderNr, char *sentenceString )
		{
		ReadItem *searchReadItem = firstActiveReadItem();
		WordItem *readWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteReadItemsWithNonMatchingMultipleWordPart";

		if( sentenceString == NULL )
			return startError( functionNameString, "The given sentence string is undefined" );

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
						return addError( functionNameString, "I failed to delete an active read item" );

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
		char functionNameString[FUNCTION_NAME_LENGTH] = "setGrammarParameter";

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
							if( searchReadItem->readString != NULL )
								strcpy( searchReadItem->readString, definitionGrammarString );
							else
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMoreInterpretations";

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
		char functionNameString[FUNCTION_NAME_LENGTH] = "getNumberOfReadWordReferences";

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
