/*
 *	Class:			InterfaceList
 *	Parent class:	List
 *	Purpose:		To store interface items
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

#include "InterfaceItem.cpp"
#include "List.h"
#include "WordItem.h"

class InterfaceList : private List
	{
	friend class WordItem;

	protected:
	// Constructor / deconstructor

	InterfaceList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_INTERFACE_LIST_SYMBOL, "InterfaceList", commonVariables, myWordItem );
		}

	~InterfaceList()
		{
		InterfaceItem *deleteItem;
		InterfaceItem *searchItem = firstActiveInterfaceItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextInterfaceItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class InterfaceList has inactive items." );

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class InterfaceList has archived items." );

		searchItem = (InterfaceItem *)firstReplacedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextInterfaceItem();
			delete deleteItem;
			}

		searchItem = (InterfaceItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextInterfaceItem();
			delete deleteItem;
			}
		}


	// Protected functions

	ResultType checkInterface( unsigned short interfaceParameter, char *interfaceString )
		{
		ReferenceResultType referenceResult;
		InterfaceItem *searchItem = firstActiveInterfaceItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkInterface";

		if( interfaceString != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->interfaceParameter() != interfaceParameter )
					{
					if( searchItem->interfaceString() != NULL )
						{
						if( ( referenceResult = compareStrings( interfaceString, searchItem->interfaceString() ) ).result == RESULT_OK )
							{
							if( !referenceResult.hasFoundMatchingStrings )
								searchItem = searchItem->nextInterfaceItem();
							else
								return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given interface string already exists" );
							}
						else
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to compare two interface strings" );
						}
					else
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I've found an undefined interface string" );
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given interface parameter already exists" );
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given interface string is undefined" );

		return RESULT_OK;
		}

	ResultType createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createInterfaceItem";

		if( commonVariables()->currentItemNr < MAX_ITEM_NR )
			{
			if( addItemToList( QUERY_ACTIVE_CHAR, new InterfaceItem( interfaceParameter, interfaceStringLength, interfaceString, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
				return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active interface item" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		InterfaceItem *searchItem = firstActiveInterfaceItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeInterfaceItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store an interface item in the database" );
				}

			searchItem = searchItem->nextInterfaceItem();
			}

		searchItem = firstReplacedInterfaceItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeInterfaceItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced interface item in the database" );
				}

			searchItem = searchItem->nextInterfaceItem();
			}

		return RESULT_OK;
		}
*/
	InterfaceItem *firstActiveInterfaceItem()
		{
		return (InterfaceItem *)firstActiveItem();
		}

	const char *interfaceString( unsigned short interfaceParameter )
		{
		InterfaceItem *searchItem = firstActiveInterfaceItem();

		while( searchItem != NULL )
			{
			if( searchItem->interfaceParameter() == interfaceParameter )
				return searchItem->interfaceString();

			searchItem = searchItem->nextInterfaceItem();
			}

		return INTERFACE_STRING_NOT_AVAILABLE;
		}
	};

/*************************************************************************
 *	"I will exalt you, my God and King,
 *	and praise your name forever and ever." (Psalm 145:1)
 *************************************************************************/
