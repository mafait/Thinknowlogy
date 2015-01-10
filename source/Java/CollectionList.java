/*
 *	Class:			CollectionList
 *	Parent class:	List
 *	Purpose:		To store collection items
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class CollectionList extends List
	{
	// Private methods

	private CollectionItem firstActiveCollectionItem()
		{
		return (CollectionItem)firstActiveItem();
		}


	// Constructor / deconstructor

	protected CollectionList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_COLLECTION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected boolean hasCollectionNr( int collectionNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionNr() == collectionNr )
					return true;

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean hasCollectionNr( int collectionNr, WordItem commonWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionNr() == collectionNr &&
				searchItem.commonWordItem() == commonWordItem )
					return true;

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean hasCollection( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionNr() == collectionNr &&
				searchItem.collectionWordItem() == collectionWordItem &&
				searchItem.commonWordItem() == commonWordItem )
					return true;

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean hasNonExclusiveCollection( int collectionNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( !searchItem.isExclusiveSpecification() &&
				searchItem.collectionNr() == collectionNr )
					return true;

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean isCollectedWithItself()
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		while( searchItem != null )
			{
			if( searchItem.commonWordItem() == myWordItem() )
				return true;

			searchItem = searchItem.nextCollectionItem();
			}

		return false;
		}

	protected boolean isExclusiveCollection( int collectionNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionNr() == collectionNr )
					return searchItem.isExclusiveSpecification();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.isCompoundGeneralization() &&
				searchItem.collectionNr() == collectionNr )
					return true;

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected boolean isCompoundCollection( int collectionNr, WordItem commonWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR &&
		commonWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isCompoundGeneralization() &&
				searchItem.collectionNr() == collectionNr &&
				searchItem.commonWordItem() == commonWordItem )
					return true;

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return false;
		}

	protected short highestCollectionOrderNr( int collectionNr )
		{
		short highestCollectionOrderNr = Constants.NO_ORDER_NR;
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionNr() == collectionNr &&
				searchItem.collectionOrderNr() > highestCollectionOrderNr )
					highestCollectionOrderNr = searchItem.collectionOrderNr();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return highestCollectionOrderNr;
		}

	protected short collectionOrderNr( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		while( searchItem != null )
			{
			if( searchItem.collectionNr() == collectionNr &&
			searchItem.collectionWordItem() == collectionWordItem &&
			searchItem.commonWordItem() == commonWordItem )
				return searchItem.collectionOrderNr();

			searchItem = searchItem.nextCollectionItem();
			}

		return Constants.NO_ORDER_NR;
		}

	protected int collectionNr( short collectionWordTypeNr, WordItem commonWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		commonWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.commonWordItem() == commonWordItem &&
				searchItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchItem.collectionNr();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int compoundCollectionNr( short collectionWordTypeNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.WORD_TYPE_UNDEFINED )
			{
			while( searchItem != null )
				{
				if( searchItem.isCompoundGeneralization() &&
				searchItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchItem.collectionNr();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int nonCompoundCollectionNr( short collectionWordTypeNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.WORD_TYPE_UNDEFINED )
			{
			while( searchItem != null )
				{
				if( !searchItem.isCompoundGeneralization() &&
				searchItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) )
					return searchItem.collectionNr();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int nonCompoundCollectionNr( int compoundCollectionNr )
		{
		int nonCompoundCollectionNr;
		CollectionItem searchItem = firstActiveCollectionItem();
		WordItem collectionWordItem;

		if( compoundCollectionNr > Constants.NO_CONTEXT_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionNr() == compoundCollectionNr &&
				( collectionWordItem = searchItem.collectionWordItem() ) != null )
					{
					if( ( nonCompoundCollectionNr = collectionWordItem.nonCompoundCollectionNr( searchItem.collectionWordTypeNr() ) ) > Constants.NO_COLLECTION_NR )
						return nonCompoundCollectionNr;
					}

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int collectionNrByCompoundGeneralizationWord( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		compoundGeneralizationWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isMatchingCollectionWordTypeNr( collectionWordTypeNr ) &&
				searchItem.compoundGeneralizationWordItem() == compoundGeneralizationWordItem )
					return searchItem.collectionNr();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	protected int highestCollectionNr()
		{
		int highestCollectionNr = Constants.NO_COLLECTION_NR;
		CollectionItem searchItem = firstActiveCollectionItem();

		while( searchItem != null )
			{
			if( searchItem.collectionNr() > highestCollectionNr )
				highestCollectionNr = searchItem.collectionNr();

			searchItem = searchItem.nextCollectionItem();
			}

		return highestCollectionNr;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( unusedWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionWordItem() == unusedWordItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The collected word item is still in use" );

				if( searchItem.commonWordItem() == unusedWordItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The common word item is still in use" );

				if( searchItem.compoundGeneralizationWordItem() == unusedWordItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The compound word item is still in use" );

				searchItem = searchItem.nextCollectionItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte createCollectionItem( boolean isExclusiveSpecification, short collectionOrderNr, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem, String collectionString )
		{
		if( collectionWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		collectionWordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
				{
				if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new CollectionItem( isExclusiveSpecification, collectionOrderNr, collectionWordTypeNr, commonWordTypeNr, collectionNr, collectionWordItem, commonWordItem, compoundGeneralizationWordItem, collectionString, this, myWordItem() ) ) != Constants.RESULT_OK )
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active collection item" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given collected word type number is undefined or out of bounds" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDataBase()
		{
		// Not fully implemented yet
		CollectionItem searchItem = firstActiveCollectionItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeCollectionItemInFutureDataBase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store a collection item in the database" );
				}

			searchItem = searchItem.nextCollectionItem();
			}

		searchItem = firstReplacedCollectionItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeCollectionItemInFutureDataBase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced collection item in the database" );
				}

			searchItem = searchItem.nextCollectionItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected CollectionResultType findCollection( boolean isAllowingDifferentCommonWord, WordItem collectionWordItem, WordItem commonWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		CollectionItem searchItem = firstActiveCollectionItem();

		if( commonWordItem != null )
			{
			if( collectionWordItem != null )
				{
				if( ( searchItem = firstActiveCollectionItem() ) != null )
					{
					do	{
						if( searchItem.collectionWordItem() == collectionWordItem &&

						( searchItem.commonWordItem() == commonWordItem ||

						( isAllowingDifferentCommonWord &&
						collectionWordItem.hasCollectionNr( searchItem.collectionNr() ) ) ) )
							collectionResult.isCollected = true;
						}
					while( !collectionResult.isCollected &&
					( searchItem = searchItem.nextCollectionItem() ) != null );
					}
				}
			else
				startError( 1, null, myWordItem().anyWordTypeString(), "The given common word is undefined" );
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given collected word is undefined" );

		collectionResult.result = CommonVariables.result;
		return collectionResult;
		}

	protected WordItem collectionWordItem( int compoundCollectionNr, WordItem notThisCommonWordItem )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.isCompoundGeneralization() &&
				searchItem.collectionNr() == compoundCollectionNr &&
				searchItem.commonWordItem() != notThisCommonWordItem )
					return searchItem.collectionWordItem();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return null;
		}

	protected WordItem commonWordItem( int collectionNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.collectionNr() == collectionNr )
					return searchItem.commonWordItem();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return null;
		}

	protected WordItem compoundGeneralizationWordItem( int compoundCollectionNr )
		{
		CollectionItem searchItem = firstActiveCollectionItem();

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.isCompoundGeneralization() &&
				searchItem.collectionNr() == compoundCollectionNr )
					return searchItem.compoundGeneralizationWordItem();

				searchItem = searchItem.nextCollectionItem();
				}
			}

		return null;
		}
	};

/*************************************************************************
 *
 *	"Give thanks to the Lords of lords.
 *	His faithful love endures forever." (Psalm 136:3)
 *
 *************************************************************************/
