/*	Class:			InterfaceList
 *	Parent class:	List
 *	Purpose:		To store interface items
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#include "InterfaceItem.cpp"
#include "List.h"

class InterfaceList : private List
	{
	friend class WordItem;

	// Private functions

	void deleteInterfaceList( InterfaceItem *searchInterfaceItem )
		{
		InterfaceItem *deleteInterfaceItem;

		while( searchInterfaceItem != NULL )
			{
			deleteInterfaceItem = searchInterfaceItem;
			searchInterfaceItem = searchInterfaceItem->nextInterfaceItem();
			delete deleteInterfaceItem;
			}
		}

	InterfaceItem *firstActiveInterfaceItem()
		{
		return (InterfaceItem *)firstActiveItem();
		}

	protected:
	// Constructor

	InterfaceList( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( WORD_INTERFACE_LIST_SYMBOL, "InterfaceList", commonVariables, inputOutput, myWordItem );
		}

	~InterfaceList()
		{
		deleteInterfaceList( firstActiveInterfaceItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class InterfaceList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class InterfaceList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class InterfaceList has replaced items." );

		deleteInterfaceList( (InterfaceItem *)firstDeletedItem() );
		}


	// Protected functions

	signed char checkInterface( unsigned short interfaceParameter, char *interfaceString )
		{
		InterfaceItem *searchInterfaceItem = firstActiveInterfaceItem();
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkInterface";

		if( interfaceString == NULL )
			return startError( functionNameString, "The given interface string is undefined" );

		while( searchInterfaceItem != NULL )
			{
			if( searchInterfaceItem->interfaceParameter() == interfaceParameter )
				return startError( functionNameString, "The given interface parameter already exists" );

			if( searchInterfaceItem->interfaceString() == NULL )
				return startError( functionNameString, "I found an undefined interface string" );

			if( ( boolResult = compareStrings( interfaceString, searchInterfaceItem->interfaceString() ) ).result != RESULT_OK )
				return addError( functionNameString, "I failed to compare two interface strings" );

			// Matching string
			if( boolResult.booleanValue )
				return startError( functionNameString, "The given interface string already exists" );

			searchInterfaceItem = searchInterfaceItem->nextInterfaceItem();
			}

		return RESULT_OK;
		}

	signed char createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createInterfaceItem";

		if( addItemToList( QUERY_ACTIVE_CHAR, new InterfaceItem( interfaceParameter, interfaceStringLength, interfaceString, commonVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an active interface item" );

		return RESULT_OK;
		}

	const char *interfaceString( unsigned short interfaceParameter )
		{
		InterfaceItem *searchInterfaceItem = firstActiveInterfaceItem();

		while( searchInterfaceItem != NULL )
			{
			if( searchInterfaceItem->interfaceParameter() == interfaceParameter )
				return searchInterfaceItem->interfaceString();

			searchInterfaceItem = searchInterfaceItem->nextInterfaceItem();
			}

		return INTERFACE_STRING_NOT_AVAILABLE;
		}
	};

/*************************************************************************
 *	"I will exalt you, my God and King,
 *	and praise your name forever and ever." (Psalm 145:1)
 *************************************************************************/
