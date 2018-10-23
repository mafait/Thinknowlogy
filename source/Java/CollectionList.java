/*	Class:			CollectionList
 *	Parent class:	List
 *	Purpose:		To store collection items
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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
		initializeListVariables( Constants.WORD_COLLECTION_LIST_SYMBOL, "CollectionList", myWordItem );
		}


	// Protected methods

	protected void addToCollectionWordQuickAccessList()
		{
		WordItem lastCollectionWordItem = GlobalVariables.lastCollectionWordItem;

		if( lastCollectionWordItem == null )
			GlobalVariables.firstCollectionWordItem = myWordItem();
		else
			// Word order is important: Add word at end of collection word list
			lastCollectionWordItem.nextCollectionWordItem = myWordItem();

		GlobalVariables.lastCollectionWordItem = myWordItem();
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

	protected boolean hasCollectionNr( int collectionNr, WordItem commonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr &&
				searchCollectionItem.commonWordItem() == commonWordItem )
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

	protected boolean isCollectionSpanishAmbiguous( int collectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr &&
				searchCollectionItem.collectionWordItem() == searchCollectionItem.commonWordItem() )
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

	protected int collectionNr( short collectionWordTypeNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.NO_WORD_TYPE_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionWordTypeNr() == collectionWordTypeNr )
					return searchCollectionItem.collectionNr();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNr( WordItem commonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( commonWordItem != null )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.commonWordItem() == commonWordItem )
					return searchCollectionItem.collectionNr();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
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

		if( compoundCollectionNr > Constants.NO_CONTEXT_NR )
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

	protected int collectionNrByCompoundGeneralizationWord( boolean isExclusiveSpecification, short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.NO_WORD_TYPE_NR &&
		compoundGeneralizationWordItem != null )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.isExclusiveSpecification() == isExclusiveSpecification &&
				searchCollectionItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) &&
				searchCollectionItem.compoundGeneralizationWordItem() == compoundGeneralizationWordItem )
					return searchCollectionItem.collectionNr();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
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

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.collectionWordItem() == unusedWordItem )
				return startError( 1, "The collected word item is still in use" );

			if( searchCollectionItem.commonWordItem() == unusedWordItem )
				return startError( 1, "The common word item is still in use" );

			if( searchCollectionItem.compoundGeneralizationWordItem() == unusedWordItem )
				return startError( 1, "The compound word item is still in use" );

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return Constants.RESULT_OK;
		}

	protected WordItem collectionWordItem( int compoundCollectionNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		// notThisCommonWordItem might be undefined

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == compoundCollectionNr )
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

	protected CollectionResultType createCollection( boolean isExclusiveSpecification, short collectionWordTypeNr, short commonWordTypeNr, int _collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem )
		{
		short collectionOrderNr;
		WordItem generalizationWordItem = myWordItem();
		CollectionResultType collectionResult = new CollectionResultType();

		if( collectionWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		collectionWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startCollectionResultError( 1, "The given collected word type number is undefined or out of bounds: " + collectionWordTypeNr );

		if( collectionWordItem == null )
			return startCollectionResultError( 1, "The given collected word is undefined" );

		if( commonWordItem == null )
			return startCollectionResultError( 1, "The given common word is undefined" );

		if( collectionWordItem == myWordItem() )
			return startCollectionResultError( 1, "The given collected word is the same as my word" );

		if( collectionWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL )
			collectionWordTypeNr = Constants.WORD_TYPE_NOUN_SINGULAR;

		// Typical for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !generalizationWordItem.hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( 1, "I don't have the requested word type number: " + collectionWordTypeNr );

		// Typical for French: To accept noun 'fils', variable 'isAllowingDifferentNoun' is set to true
		if( !collectionWordItem.hasWordType( true, collectionWordTypeNr ) )
			return startCollectionResultError( 1, "The collection word item doesn't have the requested word type number: " + collectionWordTypeNr );

		if( _collectionNr == Constants.NO_COLLECTION_NR )
			{
			if( ( _collectionNr = generalizationWordItem.highestCollectionNrInCollectionWords() ) >= Constants.MAX_COLLECTION_NR )
				return startCollectionResultSystemError( 1, "Collection number overflow" );

			collectionResult.createdCollectionNr = ++_collectionNr;
			}

		// A collection comes in pairs
		if( ( collectionOrderNr = generalizationWordItem.highestCollectionOrderNrInCollectionWords( _collectionNr ) ) >= Constants.MAX_ORDER_NR - 1 )
			return startCollectionResultSystemError( 1, "Collection order number overflow" );

		if( !isMarkedAsCollectionWord_ )
			{
			isMarkedAsCollectionWord_ = true;
			addToCollectionWordQuickAccessList();
			}

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new CollectionItem( isExclusiveSpecification, ++collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, _collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addCollectionResultError( 1, "I failed to add a collection item" );

		return collectionResult;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lords of lords.
 *	His faithful love endures forever." (Psalm 136:3)
 *************************************************************************/
