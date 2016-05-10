/*	Class:			GeneralizationList
 *	Parent class:	List
 *	Purpose:		To store generalization items
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

#include "GeneralizationItem.cpp"
#include "List.h"

class GeneralizationList : private List
	{
	friend class WordItem;

	protected:
	// Constructor / deconstructor

	GeneralizationList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_GENERALIZATION_LIST_SYMBOL, "GeneralizationList", commonVariables, myWordItem );
		}

	~GeneralizationList()
		{
		GeneralizationItem *deleteItem;
		GeneralizationItem *searchItem = firstActiveGeneralizationItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextGeneralizationItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class GeneralizationList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class GeneralizationList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class GeneralizationList has replaced items." );

		searchItem = (GeneralizationItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextGeneralizationItem();
			delete deleteItem;
			}
		}


	// Protected functions

	GeneralizationResultType findGeneralization( bool isRelation, WordItem *generalizationWordItem )
		{
		GeneralizationResultType generalizationResult;
		GeneralizationItem *searchItem = firstActiveGeneralizationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findGeneralization";

		if( generalizationWordItem != NULL )
			{
			while( searchItem != NULL &&
			!generalizationResult.hasFoundGeneralization )
				{
				if( searchItem->isRelation() == isRelation &&
				searchItem->generalizationWordItem() == generalizationWordItem )
					generalizationResult.hasFoundGeneralization = true;
				else
					searchItem = searchItem->nextGeneralizationItem();
				}
			}
		else
			startError( functionNameString, NULL, "The given generalization word item is undefined" );

		generalizationResult.result = commonVariables()->result;
		return generalizationResult;
		}

	ResultType checkWordItemForUsage( WordItem *unusedWordItem )
		{
		GeneralizationItem *searchItem = firstActiveGeneralizationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->generalizationWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, "The generalization word item is still in use" );

				searchItem = searchItem->nextGeneralizationItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given unused word item is undefined" );

		return RESULT_OK;
		}

	ResultType createGeneralizationItem( bool isLanguageWord, bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGeneralizationItem";

		if( generalizationWordTypeNr > WORD_TYPE_UNDEFINED &&
		generalizationWordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( commonVariables()->currentItemNr < MAX_ITEM_NR )
				{
				if( addItemToList( QUERY_ACTIVE_CHAR, new GeneralizationItem( isLanguageWord, isRelation, commonVariables()->currentLanguageNr, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to add an active generalization item" );
				}
			else
				return startError( functionNameString, NULL, "The current item number is undefined" );
			}
		else
			return startError( functionNameString, NULL, "The given generalization word type number is undefined or out of bounds" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		GeneralizationItem *searchItem = firstActiveGeneralizationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeGeneralizationItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a generalization item in the database" );
				}

			searchItem = searchItem->nextGeneralizationItem();
			}

		return RESULT_OK;
		}
*/
	GeneralizationItem *firstActiveGeneralizationItem()
		{
		return (GeneralizationItem *)firstActiveItem();
		}

	GeneralizationItem *firstGeneralizationItem( bool isOnlySelectingNoun, bool isOnlySelectingCurrentLanguage, bool isRelation )
		{
		GeneralizationItem *firstGeneralizationItem = firstActiveGeneralizationItem();

		return ( firstGeneralizationItem == NULL ? NULL : firstGeneralizationItem->getGeneralizationItem( true, isOnlySelectingCurrentLanguage, isOnlySelectingNoun, isRelation ) );
		}
	};

/*************************************************************************
 *	"Praise the Lord, for the Lord is good;
 *	celebrate his lovely name with music." (Psalm 135:3)
 *************************************************************************/
