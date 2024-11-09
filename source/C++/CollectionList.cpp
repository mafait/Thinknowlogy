﻿/*	Class:			CollectionList
 *	Parent class:	List
 *	Purpose:		Storing collection items
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

#include "CollectionItem.cpp"
#include "List.h"

class CollectionList : private List
	{
	friend class WordItem;

	// Private constructed variables

	bool isMarkedAsCollectionWord_ = false;


	// Private functions

	void deleteCollectionList( CollectionItem *searchCollectionItem )
		{
		CollectionItem *deleteCollectionItem;

		while( searchCollectionItem != NULL )
			{
			deleteCollectionItem = searchCollectionItem;
			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			delete deleteCollectionItem;
			}
		}

	CollectionItem *firstActiveCollectionItem()
		{
		return (CollectionItem *)firstActiveItem();
		}

	protected:
	// Constructor

	CollectionList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( WORD_COLLECTION_LIST_SYMBOL, "CollectionList", globalVariables, inputOutput, myWordItem );
		}

	~CollectionList()
		{
		deleteCollectionList( firstActiveCollectionItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class CollectionList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class CollectionList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class CollectionList has replaced items." );

		deleteCollectionList( (CollectionItem *)firstDeletedItem() );
		}


	// Protected functions

	void addToCollectionWordQuickAccessList()
		{
		WordItem *lastCollectionWordItem = globalVariables()->lastCollectionWordItem;
		WordItem *_myWordItem = myWordItem();

		if( lastCollectionWordItem == NULL )
			globalVariables()->firstCollectionWordItem = _myWordItem;
		else
			// Word order is important: Add word at end of collection word list
			lastCollectionWordItem->nextCollectionWordItem = _myWordItem;

		globalVariables()->lastCollectionWordItem = _myWordItem;
		}

	bool hasCollectionNr( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool hasCollectionCurrentlyBeenUpdated( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr &&
				searchCollectionItem->hasCurrentCreationSentenceNr() )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isCompoundCollection( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isExclusiveCollection( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->isExclusiveSpecification() &&
				searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isNonExclusiveCollection( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( !searchCollectionItem->isExclusiveSpecification() &&
				searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	bool isNonCompoundCollection( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( !searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return false;
		}

	unsigned short highestCollectionOrderNr( unsigned int collectionNr )
		{
		unsigned short highestCollectionOrderNr = NO_ORDER_NR;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr &&
				searchCollectionItem->collectionOrderNr() > highestCollectionOrderNr )
					highestCollectionOrderNr = searchCollectionItem->collectionOrderNr();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return highestCollectionOrderNr;
		}

	unsigned int collectionNr()
		{
		CollectionItem *firstCollectionItem = firstActiveCollectionItem();

		return ( firstCollectionItem == NULL ? NO_COLLECTION_NR : firstCollectionItem->collectionNr() );
		}

	unsigned int collectionNr( WordItem *commonWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->commonWordItem() == commonWordItem )
				return searchCollectionItem->collectionNr();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return NO_COLLECTION_NR;
		}

	unsigned int compoundCollectionNr()
		{
		unsigned int foundCompoundCollectionNr = NO_COLLECTION_NR;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->isCompoundGeneralization() )
				foundCompoundCollectionNr = searchCollectionItem->collectionNr();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return foundCompoundCollectionNr;
		}

	unsigned int nonCompoundCollectionNr()
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( !searchCollectionItem->isCompoundGeneralization() )
				return searchCollectionItem->collectionNr();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return NO_COLLECTION_NR;
		}

	unsigned int nonCompoundCollectionNr( unsigned int compoundCollectionNr )
		{
		unsigned int nonCompoundCollectionNr;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();
		WordItem *collectionWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == compoundCollectionNr &&
				( collectionWordItem = searchCollectionItem->collectionWordItem() ) != NULL &&
				( nonCompoundCollectionNr = collectionWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR )
					return nonCompoundCollectionNr;

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NO_COLLECTION_NR;
		}

	unsigned int collectionNrByCompoundGeneralizationWord( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		unsigned int lastFoundCollectionNr = NO_COLLECTION_NR;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->isExclusiveSpecification() &&
			searchCollectionItem->isMatchingCollectionWordTypeNr( collectionWordTypeNr ) &&
			searchCollectionItem->compoundGeneralizationWordItem() == compoundGeneralizationWordItem )
				lastFoundCollectionNr = searchCollectionItem->collectionNr();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return lastFoundCollectionNr;
		}

	unsigned int highestCollectionNr()
		{
		unsigned int highestCollectionNr = NO_COLLECTION_NR;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->collectionNr() > highestCollectionNr )
				highestCollectionNr = searchCollectionItem->collectionNr();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return highestCollectionNr;
		}

	signed char checkWordItemForUsage( WordItem *unusedWordItem )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, "The given unused word item is undefined" );

		while( searchCollectionItem != NULL &&
		searchCollectionItem->checkWordItemForUsage( unusedWordItem ) == RESULT_OK )
			searchCollectionItem = searchCollectionItem->nextCollectionItem();

		return globalVariables()->result;
		}

	WordItem *collectionWordItem( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr )
					return searchCollectionItem->collectionWordItem();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *commonWordItem( unsigned int collectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->collectionNr() == collectionNr )
					return searchCollectionItem->commonWordItem();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *compoundGeneralizationWordItem( unsigned int compoundCollectionNr )
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		if( compoundCollectionNr > NO_COLLECTION_NR )
			{
			while( searchCollectionItem != NULL )
				{
				if( searchCollectionItem->isCompoundGeneralization() &&
				searchCollectionItem->collectionNr() == compoundCollectionNr )
					return searchCollectionItem->compoundGeneralizationWordItem();

				searchCollectionItem = searchCollectionItem->nextCollectionItem();
				}
			}

		return NULL;
		}

	WordItem *feminineCollectionWordItem()
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->hasFemaleCollectionWord() )
				return searchCollectionItem->collectionWordItem();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return NULL;
		}

	WordItem *masculineCollectionWordItem()
		{
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != NULL )
			{
			if( searchCollectionItem->hasMaleCollectionWord() )
				return searchCollectionItem->collectionWordItem();

			searchCollectionItem = searchCollectionItem->nextCollectionItem();
			}

		return NULL;
		}

	BoolResultType findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem )
		{
		bool isCollected = false;
		CollectionItem *searchCollectionItem = firstActiveCollectionItem();
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findCollection";

		if( commonWordItem == NULL )
			return startBoolResultError( functionNameString, "The given collected word is undefined" );

		if( collectionWordItem == NULL )
			return startBoolResultError( functionNameString, "The given common word is undefined" );

		if( ( searchCollectionItem = firstActiveCollectionItem() ) != NULL )
			{
			do	{
				if( searchCollectionItem->collectionWordItem() == collectionWordItem &&

				( isAllowingDifferentCommonWord ||
				searchCollectionItem->commonWordItem() == commonWordItem ) )
					isCollected = true;
				}
			while( !isCollected &&
			( searchCollectionItem = searchCollectionItem->nextCollectionItem() ) != NULL );
			}

		boolResult.booleanValue = isCollected;
		return boolResult;
		}

	CollectionResultType createCollectionItem( bool isCheckingForSameCollectionNumber, bool isExclusiveSpecification, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int _collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem )
		{
		unsigned short collectionOrderNr;
		unsigned int foundCollectionNr;
		WordItem *_myWordItem = myWordItem();
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createCollectionItem";

		if( collectionWordTypeNr <= NO_WORD_TYPE_NR ||
		collectionWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startCollectionResultError( functionNameString, "The given collection word type number is undefined or out of bounds: ", collectionWordTypeNr );

		if( collectionWordItem == NULL )
			return startCollectionResultError( functionNameString, "The given collection word is undefined" );

		if( commonWordItem == NULL )
			return startCollectionResultError( functionNameString, "The given common word is undefined" );

		if( collectionWordItem == _myWordItem )
			return startCollectionResultError( functionNameString, "The given collection word is the same as my word" );

		if( isCheckingForSameCollectionNumber &&
		( foundCollectionNr = _myWordItem->collectionNr( commonWordItem ) ) > NO_COLLECTION_NR &&

			( _collectionNr == NO_COLLECTION_NR ||
			foundCollectionNr == _collectionNr ) &&

		collectionWordItem->collectionNr( commonWordItem ) == foundCollectionNr )
			return startCollectionResultError( functionNameString, "Both the given collection word and my word are already collected through the same collection number" );

		if( collectionWordTypeNr == WORD_TYPE_NOUN_PLURAL )
			collectionWordTypeNr = WORD_TYPE_NOUN_SINGULAR;

		// Typical for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !_myWordItem->hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( functionNameString, "I don't have the requested word type number: ", collectionWordTypeNr );

		// Typical for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !collectionWordItem->hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( functionNameString, "The collection word item doesn't have the requested word type number: ", collectionWordTypeNr );

		if( _collectionNr == NO_COLLECTION_NR )
			{
			if( ( _collectionNr = _myWordItem->highestCollectionNrInCollectionWords() ) >= MAX_COLLECTION_NR )
				return startCollectionResultSystemError( functionNameString, "Collection number overflow" );

			collectionResult.createdCollectionNr = ++_collectionNr;
			}

		// Each collection comes in pairs
		if( ( collectionOrderNr = _myWordItem->highestCollectionOrderNrInCollectionWords( _collectionNr ) ) >= MAX_ORDER_NR - 1 )
			return startCollectionResultSystemError( functionNameString, "Collection order number overflow" );

		if( !isMarkedAsCollectionWord_ )
			{
			isMarkedAsCollectionWord_ = true;
			addToCollectionWordQuickAccessList();
			}

		if( addItemToList( QUERY_ACTIVE_CHAR, new CollectionItem( isExclusiveSpecification, ++collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, _collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, globalVariables(), inputOutput(), this, _myWordItem ) ) != RESULT_OK )
			return addCollectionResultError( functionNameString, "I failed to add a collection item" );

		return collectionResult;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lords of lords.
 *	His faithful love endures forever." (Psalm 136:3)
 *************************************************************************/
