/*	Class:			GeneralizationList
 *	Parent class:	List
 *	Purpose:		To store generalization items
 *	Version:		Thinknowlogy 2018r1 (ShangDi 上帝)
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

#include "GeneralizationItem.cpp"
#include "List.h"

class GeneralizationList : private List
	{
	friend class WordItem;

	// Private functions

	void deleteGeneralizationList( GeneralizationItem *searchGeneralizationItem )
		{
		GeneralizationItem *deleteGeneralizationItem;

		while( searchGeneralizationItem != NULL )
			{
			deleteGeneralizationItem = searchGeneralizationItem;
			searchGeneralizationItem = searchGeneralizationItem->nextGeneralizationItem();
			delete deleteGeneralizationItem;
			}
		}

	protected:
	// Constructor

	GeneralizationList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( WORD_GENERALIZATION_LIST_SYMBOL, "GeneralizationList", globalVariables, inputOutput, myWordItem );
		}

	~GeneralizationList()
		{
		deleteGeneralizationList( firstActiveGeneralizationItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class GeneralizationList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class GeneralizationList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class GeneralizationList has replaced items." );

		deleteGeneralizationList( (GeneralizationItem *)firstDeletedItem() );
		}


	// Protected functions

	signed char checkWordItemForUsage( WordItem *unusedWordItem )
		{
		GeneralizationItem *searchGeneralizationItem = firstActiveGeneralizationItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, "The given unused word item is undefined" );

		while( searchGeneralizationItem != NULL )
			{
			if( searchGeneralizationItem->generalizationWordItem() == unusedWordItem )
				return startError( functionNameString, "The generalization word item is still in use" );

			searchGeneralizationItem = searchGeneralizationItem->nextGeneralizationItem();
			}

		return RESULT_OK;
		}

	signed char createGeneralizationItem( bool isLanguageWord, bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createGeneralizationItem";

		if( generalizationWordTypeNr <= NO_WORD_TYPE_NR &&
		generalizationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given generalization word type number is undefined or out of bounds: ", generalizationWordTypeNr );

		if( addItemToList( QUERY_ACTIVE_CHAR, new GeneralizationItem( isLanguageWord, isRelation, globalVariables()->currentLanguageNr, specificationWordTypeNr, generalizationWordTypeNr, generalizationWordItem, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an active generalization item" );

		return RESULT_OK;
		}

	GeneralizationItem *firstActiveGeneralizationItem()
		{
		return (GeneralizationItem *)firstActiveItem();
		}

	GeneralizationItem *firstGeneralizationItem( bool isOnlySelectingNoun, bool isOnlySelectingCurrentLanguage, bool isRelation )
		{
		GeneralizationItem *firstGeneralizationItem = firstActiveGeneralizationItem();

		return ( firstGeneralizationItem == NULL ? NULL : firstGeneralizationItem->generalizationItem( true, isOnlySelectingCurrentLanguage, isOnlySelectingNoun, isRelation ) );
		}

	BoolResultType findGeneralization( bool isRelation, WordItem *generalizationWordItem )
		{
		bool hasFoundGeneralization = false;
		GeneralizationItem *searchGeneralizationItem = firstActiveGeneralizationItem();
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findGeneralization";

		if( generalizationWordItem == NULL )
			return startBoolResultError( functionNameString, "The given generalization word item is undefined" );

		while( searchGeneralizationItem != NULL &&
		!hasFoundGeneralization )
			{
			if( searchGeneralizationItem->isRelation() == isRelation &&
			searchGeneralizationItem->generalizationWordItem() == generalizationWordItem )
				hasFoundGeneralization = true;
			else
				searchGeneralizationItem = searchGeneralizationItem->nextGeneralizationItem();
			}

		boolResult.booleanValue = hasFoundGeneralization;
		return boolResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord, for the Lord is good;
 *	celebrate his lovely name with music." (Psalm 135:3)
 *************************************************************************/
