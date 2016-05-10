/*	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		To store context items
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

#include "ContextItem.cpp"
#include "List.h"

class ContextList : private List
	{
	friend class WordItem;

	// Private functions

	bool hasContext( bool isCompoundCollectionSpanishAmbiguous, unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr &&
				searchItem->specificationWordItem() == specificationWordItem &&
				searchItem->isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	protected:
	// Constructor / deconstructor

	ContextList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_CONTEXT_LIST_SYMBOL, "ContextList", commonVariables, myWordItem );
		}

	~ContextList()
		{
		ContextItem *deleteItem;
		ContextItem *searchItem = firstActiveContextItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextContextItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class ContextList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class ContextList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class ContextList has replaced items." );

		searchItem = (ContextItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextContextItem();
			delete deleteItem;
			}
		}


	// Protected functions

	bool hasContext( unsigned int contextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	bool hasContext( unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr &&
				searchItem->specificationWordItem() == specificationWordItem )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	bool hasContextCurrentlyBeenUpdated( unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->hasCurrentCreationSentenceNr() &&
				searchItem->contextNr() == contextNr &&
				searchItem->specificationWordItem() == specificationWordItem )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	bool isContextSubset( unsigned int subsetContextNr, unsigned int fullSetContextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( fullSetContextNr > NO_CONTEXT_NR &&
		subsetContextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == subsetContextNr &&
				hasContext( fullSetContextNr, searchItem->specificationWordItem() ) )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	unsigned int contextNr( WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchItem != NULL )
			{
			if( searchItem->specificationWordItem() == specificationWordItem )
				return searchItem->contextNr();

			searchItem = searchItem->nextContextItem();
			}

		return NO_CONTEXT_NR;
		}

	unsigned int contextNr( bool isCompoundCollectionSpanishAmbiguous, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchItem != NULL )
			{
			if( searchItem->specificationWordItem() == specificationWordItem &&
			searchItem->isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous )
				return searchItem->contextNr();

			searchItem = searchItem->nextContextItem();
			}

		return NO_CONTEXT_NR;
		}

	unsigned int highestContextNr()
		{
		unsigned int highestContextNr = NO_CONTEXT_NR;
		ContextItem *searchItem = firstActiveContextItem();

		while( searchItem != NULL )
			{
			if( searchItem->contextNr() > highestContextNr )
				highestContextNr = searchItem->contextNr();

			searchItem = searchItem->nextContextItem();
			}

		return highestContextNr;
		}

	ResultType addContext( bool isCompoundCollectionSpanishAmbiguous, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addContext";

		if( contextNr > NO_CONTEXT_NR )
			{
			if( !hasContext( isCompoundCollectionSpanishAmbiguous, contextNr, specificationWordItem ) )
				{
				if( contextWordTypeNr > WORD_TYPE_UNDEFINED &&
				contextWordTypeNr < NUMBER_OF_WORD_TYPES )
					{
					if( specificationWordItem == NULL ||

					( specificationWordTypeNr > WORD_TYPE_UNDEFINED &&
					specificationWordTypeNr < NUMBER_OF_WORD_TYPES ) )
						{
						if( commonVariables()->currentItemNr < MAX_ITEM_NR )
							{
							if( addItemToList( QUERY_ACTIVE_CHAR, new ContextItem( isCompoundCollectionSpanishAmbiguous, contextWordTypeNr, ( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? WORD_TYPE_NOUN_SINGULAR : specificationWordTypeNr ), contextNr, specificationWordItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to add an active context item" );
							}
						else
							return startError( functionNameString, NULL, "The current item number is undefined" );
						}
					else
						return startError( functionNameString, NULL, "The given specification word type is undefined or out of bounds" );
					}
				else
					return startError( functionNameString, NULL, "The given context word type is undefined or out of bounds" );
				}
			}
		else
			return startError( functionNameString, NULL, "The given context number is undefined" );

		return RESULT_OK;
		}

	ResultType checkWordItemForUsage( WordItem *unusedWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->specificationWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, "The specification word item is still in use" );

				searchItem = searchItem->nextContextItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given unused word item is undefined" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		ContextItem *searchItem = firstActiveContextItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeContextItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a context item in the database" );
				}

			searchItem = searchItem->nextContextItem();
			}

		return RESULT_OK;
		}
*/
	ContextItem *firstActiveContextItem()
		{
		return (ContextItem *)firstActiveItem();
		}

	ContextItem *contextItem( unsigned int contextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr )
					return searchItem;

				searchItem = searchItem->nextContextItem();
				}
			}

		return NULL;
		}

	ContextItem *contextItem( WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchItem != NULL )
			{
			if( searchItem->specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem->nextContextItem();
			}

		return NULL;
		}

	ContextItem *contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();
		WordItem *anyWordItem = myWordItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( nContextWords > 0 )
			{
			while( searchItem != NULL )
				{
				if( searchItem->specificationWordItem() == specificationWordItem &&
				searchItem->isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous &&
				anyWordItem->nContextWordsInAllWords( searchItem->contextNr(), specificationWordItem ) == nContextWords )
					return searchItem;

				searchItem = searchItem->nextContextItem();
				}
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"O Lord my God, you have performed many wonders for us.
 *	Your plans for us are too numerous to list.
 *	You have no equal.
 *	I have tried to recite all your wonderful deeds,
 *	I would never come to the end of them." (Psalm 40:5)
 *************************************************************************/
