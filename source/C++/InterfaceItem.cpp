﻿/*	Class:			InterfaceItem
 *	Parent class:	Item
 *	Purpose:		Storing info about the user-interface messages
 *					in the available languages
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

	// Private initialized variables

	unsigned short interfaceParameter_ = NO_INTERFACE_PARAMETER;

	char *interfaceString_ = NULL;


	protected:
	// Constructor

	InterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "InterfaceItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		interfaceParameter_ = interfaceParameter;

		// Checking private initialized variables

		if( interfaceString != NULL )
			{
			if( interfaceStringLength < SENTENCE_STRING_LENGTH &&
			strlen( interfaceString ) < SENTENCE_STRING_LENGTH )
				{
				if( ( interfaceString_ = new char[interfaceStringLength + 1] ) != NULL )
					{
					strcpy( interfaceString_, EMPTY_STRING );
					strncat( interfaceString_, interfaceString, interfaceStringLength );
					}
				else
					startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the interface string" );
				}
			else
				startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given interface string is too long" );
			}
		else
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given interface string is undefined" );
		}

	~InterfaceItem()
		{
		if( interfaceString_ != NULL )
			delete interfaceString_;
		}


	// Protected virtual functions

	virtual void displayString( bool isReturnQueryToPosition )
		{
		statusString[0] = statusChar();

		if( interfaceString_ != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables()->queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( globalVariables()->queryString, interfaceString_ );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( interfaceParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				interfaceParameter_ > NO_INTERFACE_PARAMETER ) );
		}

	virtual char *itemString()
		{
		return interfaceString_;
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( interfaceParameter_ > NO_INTERFACE_PARAMETER )
			{
			sprintf( tempString, "%cinterfaceParameter:%u", QUERY_SEPARATOR_CHAR, interfaceParameter_ );
			strcat( queryString, tempString );
			}

		if( interfaceString_ != NULL )
			{
			sprintf( tempString, "%cinterfaceString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, interfaceString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
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
