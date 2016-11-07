/*	Class:			GeneralizationList
 *	Parent class:	List
 *	Purpose:		To store generalization items
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

class GeneralizationList extends List
	{
	// Constructor

	protected GeneralizationList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_GENERALIZATION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected GeneralizationResultType findGeneralization( boolean isRelation, WordItem generalizationWordItem )
		{
		GeneralizationResultType generalizationResult = new GeneralizationResultType();
		GeneralizationItem searchGeneralizationItem = firstActiveGeneralizationItem();

		if( generalizationWordItem == null )
			return startGeneralizationResultError( 1, null, "The given generalization word item is undefined" );

		while( searchGeneralizationItem != null &&
		!generalizationResult.hasFoundGeneralization )
			{
			if( searchGeneralizationItem.isRelation() == isRelation &&
			searchGeneralizationItem.generalizationWordItem() == generalizationWordItem )
				generalizationResult.hasFoundGeneralization = true;
			else
				searchGeneralizationItem = searchGeneralizationItem.nextGeneralizationItem();
			}

		return generalizationResult;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		GeneralizationItem searchGeneralizationItem = firstActiveGeneralizationItem();

		if( unusedWordItem == null )
			return startError( 1, null, "The given unused word item is undefined" );

		while( searchGeneralizationItem != null )
			{
			if( searchGeneralizationItem.generalizationWordItem() == unusedWordItem )
				return startError( 1, null, "The generalization word item is still in use" );

			searchGeneralizationItem = searchGeneralizationItem.nextGeneralizationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte createGeneralizationItem( boolean isLanguageWord, boolean isRelation, short specificationWordTypeNr, short generalizationWordTypeNr, WordItem generalizationWordItem )
		{
		if( generalizationWordTypeNr <= Constants.NO_WORD_TYPE_NR &&
		generalizationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, null, "The given generalization word type number is undefined or out of bounds: " + generalizationWordTypeNr );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new GeneralizationItem( isLanguageWord, isRelation, CommonVariables.currentLanguageNr, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to add an active generalization item" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		GeneralizationItem searchGeneralizationItem = firstActiveGeneralizationItem();

		while( searchGeneralizationItem != null )
			{
			if( searchGeneralizationItem.hasCurrentCreationSentenceNr() )
				{
				if( searchGeneralizationItem.storeGeneralizationItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a generalization item in the database" );
				}

			searchGeneralizationItem = searchGeneralizationItem.nextGeneralizationItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected GeneralizationItem firstActiveGeneralizationItem()
		{
		return (GeneralizationItem)firstActiveItem();
		}

	protected GeneralizationItem firstGeneralizationItem( boolean isOnlySelectingNoun, boolean isOnlySelectingCurrentLanguage, boolean isRelation )
		{
		GeneralizationItem firstGeneralizationItem = firstActiveGeneralizationItem();

		return ( firstGeneralizationItem == null ? null : firstGeneralizationItem.getGeneralizationItem( true, isOnlySelectingCurrentLanguage, isOnlySelectingNoun, isRelation ) );
		}
	};

/*************************************************************************
 *	"Praise the Lord, for the Lord is good;
 *	celebrate his lovely name with music." (Psalm 135:3)
 *************************************************************************/
