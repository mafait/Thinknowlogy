/*	Class:			InterfaceList
 *	Parent class:	List
 *	Purpose:		To store interface items
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

package org.mafait.thinknowlogy;

class InterfaceList extends List
	{
	// Private methods

	private InterfaceItem firstActiveInterfaceItem()
		{
		return (InterfaceItem)firstActiveItem();
		}

	// Constructor

	protected InterfaceList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_INTERFACE_LIST_SYMBOL, "InterfaceList", myWordItem );
		}


	// Protected methods

	protected byte checkInterface( short interfaceParameter, String interfaceString )
		{
		InterfaceItem searchInterfaceItem = firstActiveInterfaceItem();
		BoolResultType boolResult;

		if( interfaceString == null )
			return startError( 1, "The given interface string is undefined" );

		while( searchInterfaceItem != null )
			{
			if( searchInterfaceItem.interfaceParameter() == interfaceParameter )
				return startError( 1, "The given interface parameter already exists" );

			if( searchInterfaceItem.interfaceString() == null )
				return startError( 1, "I found an undefined interface string" );

			if( ( boolResult = compareStrings( interfaceString, searchInterfaceItem.interfaceString() ) ).result != Constants.RESULT_OK )
				return addError( 1, "I failed to compare two interface strings" );

			// Matching string
			if( boolResult.booleanValue )
				return startError( 1, "The given interface string already exists" );

			searchInterfaceItem = searchInterfaceItem.nextInterfaceItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte createInterfaceItem( short interfaceParameter, int interfaceStringLength, String interfaceString )
		{
		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new InterfaceItem( interfaceParameter, interfaceStringLength, interfaceString, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an interface item" );

		return Constants.RESULT_OK;
		}

	protected String interfaceString( short interfaceParameter )
		{
		InterfaceItem searchInterfaceItem = firstActiveInterfaceItem();

		while( searchInterfaceItem != null )
			{
			if( searchInterfaceItem.interfaceParameter() == interfaceParameter )
				return searchInterfaceItem.interfaceString();

			searchInterfaceItem = searchInterfaceItem.nextInterfaceItem();
			}

		return null;
		}
	};

/*************************************************************************
 *	"I will exalt you, my God and King,
 *	and praise your name forever and ever." (Psalm 145:1)
 *************************************************************************/
