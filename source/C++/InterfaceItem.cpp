/*
 *	Class:			InterfaceItem
 *	Parent class:	Item
 *	Purpose:		To store info about the interface messages
 *					in a certain language that can be shown to the user
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

#include "Item.h"

class InterfaceItem : private Item
	{
	friend class InterfaceList;

	// Private loadable variables

	unsigned short interfaceParameter_;

	char *interfaceString_;


	protected:
	// Constructor / deconstructor

	InterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "InterfaceItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		interfaceParameter_ = interfaceParameter;
		interfaceString_ = NULL;

		if( interfaceString != NULL )
			{
			if( interfaceStringLength < MAX_SENTENCE_STRING_LENGTH &&
			strlen( interfaceString ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( interfaceString_ = new char[interfaceStringLength + 1] ) != NULL )
					{
					strcpy( interfaceString_, EMPTY_STRING );
					strncat( interfaceString_, interfaceString, interfaceStringLength );
					}
				else
					startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the interface string" );
				}
			else
				startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given interface string is too long" );
			}
		else
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given interface string is undefined" );
		}

	~InterfaceItem()
		{
		if( interfaceString_ != NULL )
			delete interfaceString_;
		}


	// Protected virtual functions

	virtual void showString( bool isReturnQueryToPosition )
		{
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( interfaceString_ != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, interfaceString_ );
			}
		}

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( interfaceParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				interfaceParameter_ > NO_INTERFACE_PARAMETER ) );
		}

	virtual char *itemString()
		{
		return interfaceString_;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		Item::toString( queryWordTypeNr );

		if( interfaceParameter_ > NO_INTERFACE_PARAMETER )
			{
			sprintf( tempString, "%cinterfaceParameter:%u", QUERY_SEPARATOR_CHAR, interfaceParameter_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( interfaceString_ != NULL )
			{
			sprintf( tempString, "%cinterfaceString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, interfaceString_, QUERY_STRING_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		return commonVariables()->queryString;
		}


	// Protected functions

	unsigned short interfaceParameter()
		{
		return interfaceParameter_;
		}

	char *interfaceString()
		{
		return interfaceString_;
		}

	InterfaceItem *nextInterfaceItem()
		{
		return (InterfaceItem *)nextItem;
		}
	};

/*************************************************************************
 *	"O Lord, what are human beings that you should notice them,
 *	mere mortals that you should think about them?" (Psalm 144:3)
 *************************************************************************/
