/*
 *	Class:			GeneralizationList
 *	Parent class:	List
 *	Purpose:		To store generalization items
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

class GeneralizationList extends List
	{
	// Constructor / deconstructor

	protected GeneralizationList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_GENERALIZATION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected GeneralizationResultType findGeneralization( boolean isRelation, WordItem generalizationWordItem )
		{
		GeneralizationResultType generalizationResult = new GeneralizationResultType();
		GeneralizationItem searchItem = firstActiveGeneralizationItem();

		if( generalizationWordItem != null )
			{
			while( searchItem != null &&
			!generalizationResult.hasFoundGeneralization )
				{
				if( searchItem.isRelation() == isRelation &&
				searchItem.generalizationWordItem() == generalizationWordItem )
					generalizationResult.hasFoundGeneralization = true;
				else
					searchItem = searchItem.nextGeneralizationItem();
				}
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "The given generalization word item is undefined" );

		generalizationResult.result = CommonVariables.result;
		return generalizationResult;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		GeneralizationItem searchItem = firstActiveGeneralizationItem();

		if( unusedWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.generalizationWordItem() == unusedWordItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The generalization word item is still in use" );

				searchItem = searchItem.nextGeneralizationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte createGeneralizationItem( boolean isRelation, short specificationWordTypeNr, short generalizationWordTypeNr, WordItem generalizationWordItem )
		{
		if( generalizationWordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		generalizationWordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
				{
				if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new GeneralizationItem( isRelation, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active generalization item" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given generalization word type number is undefined or out of bounds" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		GeneralizationItem searchItem = firstActiveGeneralizationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeGeneralizationItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store a generalization item in the database" );
				}

			searchItem = searchItem.nextGeneralizationItem();
			}

		searchItem = firstReplacedGeneralizationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeGeneralizationItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced generalization item in the database" );
				}

			searchItem = searchItem.nextGeneralizationItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected GeneralizationItem firstActiveGeneralizationItem()
		{
		return (GeneralizationItem)firstActiveItem();
		}

	protected GeneralizationItem firstGeneralizationItem( boolean isOnlySelectingNoun, boolean isRelation )
		{
		GeneralizationItem firstGeneralizationItem = firstActiveGeneralizationItem();

		return ( firstGeneralizationItem == null ? null : firstGeneralizationItem.getGeneralizationItem( true, isOnlySelectingNoun, isRelation ) );
		}
	};

/*************************************************************************
 *	"Praise the Lord, for the Lord is good;
 *	celebrate his lovely name with music." (Psalm 135:3)
 *************************************************************************/
