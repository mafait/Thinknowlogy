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

package org.mafait.thinknowlogy;

class CollectionList extends List
	{
	// Private constructed variables

	private boolean isMarkedAsCollectionWord_ = false;


	// Private methods

	private CollectionItem firstActiveCollectionItem()
		{
		return (CollectionItem)firstActiveItem();
		}

	// Constructor

	protected CollectionList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_COLLECTION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void addToCollectionWordQuickAccessList()
		{
		WordItem lastCollectionWordItem = GlobalVariables.lastCollectionWordItem;
		WordItem _myWordItem = myWordItem();

		if( lastCollectionWordItem == null )
			GlobalVariables.firstCollectionWordItem = _myWordItem;
		else
			// Word order is important: Add word at end of collection word list
			lastCollectionWordItem.nextCollectionWordItem = _myWordItem;

		GlobalVariables.lastCollectionWordItem = _myWordItem;
		}

	protected boolean hasCollectionNr( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean hasCollectionCurrentlyBeenUpdated( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr &&
				searchCollectionItem.hasCurrentCreationSentenceNr() )
					return true;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.isCompoundGeneralization() &&
				searchCollectionItem.collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean isExclusiveCollection( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.isExclusiveSpecification() &&
				searchCollectionItem.collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean isNonExclusiveCollection( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( !searchCollectionItem.isExclusiveSpecification() &&
				searchCollectionItem.collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean isNonCompoundCollection( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( !searchCollectionItem.isCompoundGeneralization() &&
				searchCollectionItem.collectionNr() == collectionNr )
					return true;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected short highestCollectionOrderNr( int collectionNr )
		{
		short highestCollectionOrderNr = Constants.NO_ORDER_NR;
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr &&
				searchCollectionItem.collectionOrderNr() > highestCollectionOrderNr )
					highestCollectionOrderNr = searchCollectionItem.collectionOrderNr();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return highestCollectionOrderNr;
		}

	protected int collectionNr()
		{
		CollectionItem firstCollectionItem = firstActiveCollectionItem();

		return ( firstCollectionItem == null ? Constants.NO_COLLECTION_NR : firstCollectionItem.collectionNr() );
		}

	protected int collectionNr( WordItem commonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.commonWordItem() == commonWordItem )
				return searchCollectionItem.collectionNr();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int compoundCollectionNr()
		{
		int foundCompoundCollectionNr = Constants.NO_COLLECTION_NR;
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.isCompoundGeneralization() )
				foundCompoundCollectionNr = searchCollectionItem.collectionNr();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return foundCompoundCollectionNr;
		}

	protected int nonCompoundCollectionNr()
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( !searchCollectionItem.isCompoundGeneralization() )
				return searchCollectionItem.collectionNr();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int nonCompoundCollectionNr( int compoundCollectionNr )
		{
		int nonCompoundCollectionNr;
		CollectionItem searchCollectionItem = firstActiveCollectionItem();
		WordItem collectionWordItem;

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == compoundCollectionNr &&
				( collectionWordItem = searchCollectionItem.collectionWordItem() ) != null &&
				( nonCompoundCollectionNr = collectionWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR )
					return nonCompoundCollectionNr;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNrByCompoundGeneralizationWord( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int lastFoundCollectionNr = Constants.NO_COLLECTION_NR;
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.isExclusiveSpecification() &&
			searchCollectionItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) &&
			searchCollectionItem.compoundGeneralizationWordItem() == compoundGeneralizationWordItem )
				lastFoundCollectionNr = searchCollectionItem.collectionNr();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return lastFoundCollectionNr;
		}

	protected int highestCollectionNr()
		{
		int highestCollectionNr = Constants.NO_COLLECTION_NR;
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.collectionNr() > highestCollectionNr )
				highestCollectionNr = searchCollectionItem.collectionNr();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return highestCollectionNr;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( unusedWordItem == null )
			return startError( 1, "The given unused word item is undefined" );

		while( searchCollectionItem != null &&
		searchCollectionItem.checkWordItemForUsage( unusedWordItem ) == Constants.RESULT_OK )
			searchCollectionItem = searchCollectionItem.nextCollectionItem();

		return GlobalVariables.result;
		}

	protected WordItem collectionWordItem( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr )
					return searchCollectionItem.collectionWordItem();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return null;
		}

	protected WordItem commonWordItem( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr )
					return searchCollectionItem.commonWordItem();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return null;
		}

	protected WordItem compoundGeneralizationWordItem( int compoundCollectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.isCompoundGeneralization() &&
				searchCollectionItem.collectionNr() == compoundCollectionNr )
					return searchCollectionItem.compoundGeneralizationWordItem();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return null;
		}

	protected WordItem feminineCollectionWordItem()
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.hasFemaleCollectionWord() )
				return searchCollectionItem.collectionWordItem();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return null;
		}

	protected WordItem masculineCollectionWordItem()
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.hasMaleCollectionWord() )
				return searchCollectionItem.collectionWordItem();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return null;
		}

	protected BoolResultType findCollection( boolean isAllowingDifferentCommonWord, WordItem collectionWordItem, WordItem commonWordItem )
		{
		boolean isCollected = false;
		CollectionItem searchCollectionItem = firstActiveCollectionItem();
		BoolResultType boolResult = new BoolResultType();

		if( commonWordItem == null )
			return startBoolResultError( 1, "The given collected word is undefined" );

		if( collectionWordItem == null )
			return startBoolResultError( 1, "The given common word is undefined" );

		if( ( searchCollectionItem = firstActiveCollectionItem() ) != null )
			{
			do	{
				if( searchCollectionItem.collectionWordItem() == collectionWordItem &&

				( isAllowingDifferentCommonWord ||
				searchCollectionItem.commonWordItem() == commonWordItem ) )
					isCollected = true;
				}
			while( !isCollected &&
			( searchCollectionItem = searchCollectionItem.nextCollectionItem() ) != null );
			}

		boolResult.booleanValue = isCollected;
		return boolResult;
		}

	protected CollectionResultType createCollectionItem( boolean isCheckingForSameCollectionNumber, boolean isExclusiveSpecification, short collectionWordTypeNr, short commonWordTypeNr, int _collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem )
		{
		short collectionOrderNr;
		int foundCollectionNr;
		WordItem _myWordItem = myWordItem();
		CollectionResultType collectionResult = new CollectionResultType();

		if( collectionWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		collectionWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startCollectionResultError( 1, "The given collection word type number is undefined or out of bounds: " + collectionWordTypeNr );

		if( collectionWordItem == null )
			return startCollectionResultError( 1, "The given collection word is undefined" );

		if( commonWordItem == null )
			return startCollectionResultError( 1, "The given common word is undefined" );

		if( collectionWordItem == _myWordItem )
			return startCollectionResultError( 1, "The given collection word is the same as my word" );

		if( isCheckingForSameCollectionNumber &&
		( foundCollectionNr = _myWordItem.collectionNr( commonWordItem ) ) > Constants.NO_COLLECTION_NR &&

			( _collectionNr == Constants.NO_COLLECTION_NR ||
			foundCollectionNr == _collectionNr ) &&

		collectionWordItem.collectionNr( commonWordItem ) == foundCollectionNr )
			return startCollectionResultError( 1, "Both the given collection word and my word are already collected through the same collection number" );

		if( collectionWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL )
			collectionWordTypeNr = Constants.WORD_TYPE_NOUN_SINGULAR;

		// Typical for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !_myWordItem.hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( 1, "I don't have the requested word type number: " + collectionWordTypeNr );

		// Typical for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !collectionWordItem.hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( 1, "The collection word item doesn't have the requested word type number: " + collectionWordTypeNr );

		if( _collectionNr == Constants.NO_COLLECTION_NR )
			{
			if( ( _collectionNr = WordItem.highestCollectionNrInCollectionWords() ) >= Constants.MAX_COLLECTION_NR )
				return startCollectionResultSystemError( 1, "Collection number overflow" );

			collectionResult.createdCollectionNr = ++_collectionNr;
			}

		// Each collection comes in pairs
		if( ( collectionOrderNr = WordItem.highestCollectionOrderNrInCollectionWords( _collectionNr ) ) >= Constants.MAX_ORDER_NR - 1 )
			return startCollectionResultSystemError( 1, "Collection order number overflow" );

		if( !isMarkedAsCollectionWord_ )
			{
			isMarkedAsCollectionWord_ = true;
			addToCollectionWordQuickAccessList();
			}

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new CollectionItem( isExclusiveSpecification, ++collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, _collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, this, _myWordItem ) ) != Constants.RESULT_OK )
			return addCollectionResultError( 1, "I failed to add a collection item" );

		return collectionResult;
		}
	}

/*************************************************************************
 *	"Give thanks to the Lords of lords.
 *	His faithful love endures forever." (Psalm 136:3)
 *************************************************************************/
