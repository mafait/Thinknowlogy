/*	Class:			CollectionList
 *	Parent class:	List
 *	Purpose:		To store collection items
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

class CollectionList extends List
	{
	// Private constructed variables

	private boolean isMarkedAsCollectionWord_;

	// Private methods

	private CollectionItem firstActiveCollectionItem()
		{
		return (CollectionItem)firstActiveItem();
		}

	// Constructor

	protected CollectionList( WordItem myWordItem )
		{
		isMarkedAsCollectionWord_ = false;

		initializeListVariables( Constants.WORD_COLLECTION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void addToCollectionWordQuickAccessList()
		{
		WordItem tempWordItem;
		WordItem lastCollectionWordItem = CommonVariables.firstCollectionWordItem;

		if( lastCollectionWordItem == null )
			CommonVariables.firstCollectionWordItem = myWordItem();
		else
			{
			// Word order is important: Add word at end of collection word list
			while( ( tempWordItem = lastCollectionWordItem.nextCollectionWordItem ) != null )
				lastCollectionWordItem = tempWordItem;

			lastCollectionWordItem.nextCollectionWordItem = myWordItem();
			}
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

	protected boolean hasCollection( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == collectionNr &&
				searchCollectionItem.collectionWordItem() == collectionWordItem &&
				searchCollectionItem.commonWordItem() == commonWordItem )
					return true;

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean hasNonExclusiveCollection( int collectionNr )
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

	protected boolean isCompoundCollection( int collectionNr, WordItem commonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.isCompoundGeneralization() &&
				searchCollectionItem.collectionNr() == collectionNr &&
				searchCollectionItem.commonWordItem() == commonWordItem )
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

	protected short collectionOrderNr( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.collectionNr() == collectionNr &&
			searchCollectionItem.collectionWordItem() == collectionWordItem &&
			searchCollectionItem.commonWordItem() == commonWordItem )
				return searchCollectionItem.collectionOrderNr();

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return Constants.NO_ORDER_NR;
		}

	protected int collectionNr( short collectionWordTypeNr, WordItem commonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.NO_WORD_TYPE_NR &&
		commonWordItem != null )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.commonWordItem() == commonWordItem &&
				searchCollectionItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchCollectionItem.collectionNr();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int compoundCollectionNr( short collectionWordTypeNr )
		{
		int foundCompoundCollectionNr = Constants.NO_COLLECTION_NR;
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.NO_WORD_TYPE_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.isCompoundGeneralization() &&
				searchCollectionItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					foundCompoundCollectionNr = searchCollectionItem.collectionNr();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return foundCompoundCollectionNr;
		}

	protected int nonCompoundCollectionNr( short collectionWordTypeNr )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.NO_WORD_TYPE_NR )
			{
			while( searchCollectionItem != null )
				{
				if( !searchCollectionItem.isCompoundGeneralization() &&
				searchCollectionItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchCollectionItem.collectionNr();

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
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
				( collectionWordItem = searchCollectionItem.collectionWordItem() ) != null )
					{
					if( ( nonCompoundCollectionNr = collectionWordItem.nonCompoundCollectionNr( searchCollectionItem.collectionWordTypeNr() ) ) > Constants.NO_COLLECTION_NR )
						return nonCompoundCollectionNr;
					}

				searchCollectionItem = searchCollectionItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNrByCompoundGeneralizationWord( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.NO_WORD_TYPE_NR &&
		compoundGeneralizationWordItem != null )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) &&
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
			return startError( 1, null, "The given unused word item is undefined" );

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.collectionWordItem() == unusedWordItem )
				return startError( 1, null, "The collected word item is still in use" );

			if( searchCollectionItem.commonWordItem() == unusedWordItem )
				return startError( 1, null, "The common word item is still in use" );

			if( searchCollectionItem.compoundGeneralizationWordItem() == unusedWordItem )
				return startError( 1, null, "The compound word item is still in use" );

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte createCollectionItem( boolean isExclusiveSpecification, short collectionOrderNr, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem )
		{
		if( collectionWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		collectionWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, null, "The given collected word type number is undefined or out of bounds: " + collectionWordTypeNr );

		if( !isMarkedAsCollectionWord_ )
			{
			isMarkedAsCollectionWord_ = true;
			addToCollectionWordQuickAccessList();
			}

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new CollectionItem( isExclusiveSpecification, collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to add an active collection item" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		while( searchCollectionItem != null )
			{
			if( searchCollectionItem.hasCurrentCreationSentenceNr() )
				{
				if( searchCollectionItem.storeCollectionItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a collection item in the database" );
				}

			searchCollectionItem = searchCollectionItem.nextCollectionItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected CollectionResultType findCollection( boolean isAllowingDifferentCommonWord, WordItem collectionWordItem, WordItem commonWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		if( commonWordItem == null )
			return startCollectionResultError( 1, null, "The given collected word is undefined" );

		if( collectionWordItem == null )
			return startCollectionResultError( 1, null, "The given common word is undefined" );

		if( ( searchCollectionItem = firstActiveCollectionItem() ) != null )
			{
			do	{
				if( searchCollectionItem.collectionWordItem() == collectionWordItem &&

				( searchCollectionItem.commonWordItem() == commonWordItem ||

				( isAllowingDifferentCommonWord &&
				collectionWordItem.hasCollectionNr( searchCollectionItem.collectionNr() ) ) ) )
					collectionResult.isCollected = true;
				}
			while( !collectionResult.isCollected &&
			( searchCollectionItem = searchCollectionItem.nextCollectionItem() ) != null );
			}

		return collectionResult;
		}

	protected WordItem collectionWordItem( int compoundCollectionNr, WordItem notThisCommonWordItem )
		{
		CollectionItem searchCollectionItem = firstActiveCollectionItem();

		// notThisCommonWordItem might be undefined

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchCollectionItem != null )
				{
				if( searchCollectionItem.collectionNr() == compoundCollectionNr &&
				searchCollectionItem.commonWordItem() != notThisCommonWordItem )
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
	};

/*************************************************************************
 *	"Give thanks to the Lords of lords.
 *	His faithful love endures forever." (Psalm 136:3)
 *************************************************************************/
