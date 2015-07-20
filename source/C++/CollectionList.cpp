/*
 *	Class:			CollectionList
 *	Parent class:	List
 *	Purpose:		To store collection items
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

#include "CollectionItem.cpp"
#include "List.h"

class CollectionList : private List
	{
	friend class WordCollection;
	friend class WordItem;

	// Private functions

	CollectionItem *firstActiveCollectionItem()
		{
		return (CollectionItem *)firstActiveItem();
		}

	protected:
	// Constructor / deconstructor

	CollectionList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_COLLECTION_LIST_SYMBOL, "CollectionList", commonVariables, myWordItem );
		}

	~CollectionList()
		{
		CollectionItem *deleteItem;
		CollectionItem *searchItem = firstActiveCollectionItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextCollectionItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class CollectionList has inactive items." );

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class CollectionList has archived items." );

		searchItem = (CollectionItem *)firstReplacedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextCollectionItem();
			delete deleteItem;
			}

		searchItem = (CollectionItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextCollectionItem();
			delete deleteItem;
			}
		}


	// Protected functions

	bool hasCollectionNr( unsigned int collectionNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionNr() == collectionNr )
					return true;

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool hasCollectionNr( unsigned int collectionNr, WordItem *commonWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionNr() == collectionNr &&
				searchItem->commonWordItem() == commonWordItem )
					return true;

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool hasCollection( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionNr() == collectionNr &&
				searchItem->collectionWordItem() == collectionWordItem &&
				searchItem->commonWordItem() == commonWordItem )
					return true;

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool hasNonExclusiveCollection( unsigned int collectionNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isExclusiveSpecification() &&
				searchItem->collectionNr() == collectionNr )
					return true;

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isCompoundCollection( unsigned int collectionNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isCompoundGeneralization() &&
				searchItem->collectionNr() == collectionNr )
					return true;

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isCompoundCollection( unsigned int collectionNr, WordItem *commonWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isCompoundGeneralization() &&
				searchItem->collectionNr() == collectionNr &&
				searchItem->commonWordItem() == commonWordItem )
					return true;

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isNonCompoundCollection( unsigned int collectionNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isCompoundGeneralization() &&
				searchItem->collectionNr() == collectionNr )
					return true;

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return false;
		}

	unsigned short highestCollectionOrderNr( unsigned int collectionNr )
		{
		unsigned short highestCollectionOrderNr = NO_ORDER_NR;
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionNr() == collectionNr &&
				searchItem->collectionOrderNr() > highestCollectionOrderNr )
					highestCollectionOrderNr = searchItem->collectionOrderNr();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return highestCollectionOrderNr;
		}

	unsigned short collectionOrderNr( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		while( searchItem != NULL )
			{
			if( searchItem->collectionNr() == collectionNr &&
			searchItem->collectionWordItem() == collectionWordItem &&
			searchItem->commonWordItem() == commonWordItem )
				return searchItem->collectionOrderNr();

			searchItem = searchItem->nextCollectionItem();
			}

		return NO_ORDER_NR;
		}

	unsigned int collectionNr( unsigned short collectionWordTypeNr, WordItem *commonWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > WORD_TYPE_UNDEFINED &&
		commonWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->commonWordItem() == commonWordItem &&
				searchItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchItem->collectionNr();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int compoundCollectionNr( unsigned short collectionWordTypeNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > WORD_TYPE_UNDEFINED )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isCompoundGeneralization() &&
				searchItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchItem->collectionNr();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int nonCompoundCollectionNr( unsigned short collectionWordTypeNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > WORD_TYPE_UNDEFINED )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isCompoundGeneralization() &&
				searchItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchItem->collectionNr();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int nonCompoundCollectionNr( unsigned int compoundCollectionNr )
		{
		unsigned int nonCompoundCollectionNr;
		CollectionItem *searchItem = firstActiveCollectionItem();
		WordItem *collectionWordItem;

		if( compoundCollectionNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionNr() == compoundCollectionNr &&
				( collectionWordItem = searchItem->collectionWordItem() ) != NULL )
					{
					if( ( nonCompoundCollectionNr = collectionWordItem->nonCompoundCollectionNr( searchItem->collectionWordTypeNr() ) ) > NO_COLLECTION_NR )
						return nonCompoundCollectionNr;
					}

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int collectionNrByCompoundGeneralizationWord( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > WORD_TYPE_UNDEFINED &&
		compoundGeneralizationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) &&
				searchItem->compoundGeneralizationWordItem() == compoundGeneralizationWordItem )
					return searchItem->collectionNr();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int highestCollectionNr()
		{
		unsigned int highestCollectionNr = NO_COLLECTION_NR;
		CollectionItem *searchItem = firstActiveCollectionItem();

		while( searchItem != NULL )
			{
			if( searchItem->collectionNr() > highestCollectionNr )
				highestCollectionNr = searchItem->collectionNr();

			searchItem = searchItem->nextCollectionItem();
			}

		return highestCollectionNr;
		}

	ResultType checkWordItemForUsage( WordItem *unusedWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The collected word item is still in use" );

				if( searchItem->commonWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The common word item is still in use" );

				if( searchItem->compoundGeneralizationWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The compound word item is still in use" );

				searchItem = searchItem->nextCollectionItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given unused word item is undefined" );

		return RESULT_OK;
		}

	ResultType createCollectionItem( bool isExclusiveSpecification, unsigned short collectionOrderNr, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createCollectionItem";

		if( collectionWordTypeNr > WORD_TYPE_UNDEFINED &&
		collectionWordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( commonVariables()->currentItemNr < MAX_ITEM_NR )
				{
				if( addItemToList( QUERY_ACTIVE_CHAR, new CollectionItem( isExclusiveSpecification, collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
					return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active collection item" );
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given collected word type number is undefined or out of bounds" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		CollectionItem *searchItem = firstActiveCollectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeCollectionItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store a collection item in the database" );
				}

			searchItem = searchItem->nextCollectionItem();
			}

		searchItem = firstReplacedCollectionItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeCollectionItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced collection item in the database" );
				}

			searchItem = searchItem->nextCollectionItem();
			}

		return RESULT_OK;
		}
*/
	CollectionResultType findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		CollectionResultType collectionResult;
		CollectionItem *searchItem = firstActiveCollectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findCollection";

		if( commonWordItem != NULL )
			{
			if( collectionWordItem != NULL )
				{
				if( ( searchItem = firstActiveCollectionItem() ) != NULL )
					{
					do	{
						if( searchItem->collectionWordItem() == collectionWordItem &&

						( searchItem->commonWordItem() == commonWordItem ||

						( isAllowingDifferentCommonWord &&
						collectionWordItem->hasCollectionNr( searchItem->collectionNr() ) ) ) )
							collectionResult.isCollected = true;
						}
					while( !collectionResult.isCollected &&
					( searchItem = searchItem->nextCollectionItem() ) != NULL );
					}
				}
			else
				startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given common word is undefined" );
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given collected word is undefined" );

		collectionResult.result = commonVariables()->result;
		return collectionResult;
		}

	WordItem *collectionWordItem( unsigned int compoundCollectionNr, WordItem *notThisCommonWordItem )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		notThisCommonWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionNr() == compoundCollectionNr &&
				searchItem->commonWordItem() != notThisCommonWordItem )
					return searchItem->collectionWordItem();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *commonWordItem( unsigned int collectionNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->collectionNr() == collectionNr )
					return searchItem->commonWordItem();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *compoundGeneralizationWordItem( unsigned int compoundCollectionNr )
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		if( compoundCollectionNr > NO_COLLECTION_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isCompoundGeneralization() &&
				searchItem->collectionNr() == compoundCollectionNr )
					return searchItem->compoundGeneralizationWordItem();

				searchItem = searchItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *feminineCollectionWordItem()
		{
		CollectionItem *searchItem = firstActiveCollectionItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasFemaleCollectionWord() )
				return searchItem->collectionWordItem();

			searchItem = searchItem->nextCollectionItem();
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lords of lords.
 *	His faithful love endures forever." (Psalm 136:3)
 *************************************************************************/
