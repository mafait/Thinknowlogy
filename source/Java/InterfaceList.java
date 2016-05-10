/*	Class:			InterfaceList
 *	Parent class:	List
 *	Purpose:		To store interface items
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

class InterfaceList extends List
	{

	// Private methods

	private InterfaceItem firstActiveInterfaceItem()
		{
		return (InterfaceItem)firstActiveItem();
		}

	// Constructor / deconstructor

	protected InterfaceList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_INTERFACE_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected byte checkInterface( short interfaceParameter, String interfaceString )
		{
		ReferenceResultType referenceResult;
		InterfaceItem searchItem = firstActiveInterfaceItem();

		if( interfaceString != null )
			{
			while( searchItem != null )
				{
				if( searchItem.interfaceParameter() != interfaceParameter )
					{
					if( searchItem.interfaceString() != null )
						{
						if( ( referenceResult = compareStrings( interfaceString, searchItem.interfaceString() ) ).result == Constants.RESULT_OK )
							{
							if( !referenceResult.hasFoundMatchingStrings )
								searchItem = searchItem.nextInterfaceItem();
							else
								return startError( 1, null, "The given interface string already exists" );
							}
						else
							return addError( 1, null, "I failed to compare two interface strings" );
						}
					else
						return startError( 1, null, "I have found an undefined interface string" );
					}
				else
					return startError( 1, null, "The given interface parameter already exists" );
				}
			}
		else
			return startError( 1, null, "The given interface string is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte createInterfaceItem( short interfaceParameter, int interfaceStringLength, String interfaceString )
		{
		if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
			{
			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new InterfaceItem( interfaceParameter, interfaceStringLength, interfaceString, this, myWordItem() ) ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to add an active interface item" );
			}
		else
			return startError( 1, null, "The current item number is undefined" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		InterfaceItem searchItem = firstActiveInterfaceItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeInterfaceItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store an interface item in the database" );
				}

			searchItem = searchItem.nextInterfaceItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected String interfaceString( short interfaceParameter )
		{
		InterfaceItem searchItem = firstActiveInterfaceItem();

		while( searchItem != null )
			{
			if( searchItem.interfaceParameter() == interfaceParameter )
				return searchItem.interfaceString();

			searchItem = searchItem.nextInterfaceItem();
			}

		return Constants.INTERFACE_STRING_NOT_AVAILABLE;
		}
	};

/*************************************************************************
 *	"I will exalt you, my God and King,
 *	and praise your name forever and ever." (Psalm 145:1)
 *************************************************************************/
