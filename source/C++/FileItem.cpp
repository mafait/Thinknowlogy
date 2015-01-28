/*
 *	Class:			FileItem
 *	Parent class:	Item
 *	Purpose:		To store info about the opened files
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

class FileItem : private Item
	{
	friend class AdminReadFile;
	friend class FileList;

	// Private loadable variables

	bool isInfoFile_;

	char *readFileNameString_;

	FILE *readFile_;

	protected:
	// Constructor / deconstructor

	FileItem( bool isInfoFile, char *readFileNameString, FILE *readFile, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		size_t fileNameStringLength;

		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "FileItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		isInfoFile_ = isInfoFile;

		readFileNameString_ = NULL;
		readFile_ = readFile;

		if( readFile_ != NULL )
			{
			if( readFileNameString != NULL )
				{
				if( ( fileNameStringLength = strlen( readFileNameString ) ) < MAX_SENTENCE_STRING_LENGTH )
					{
					if( ( readFileNameString_ = new char[fileNameStringLength + 1] ) != NULL )
						strcpy( readFileNameString_, readFileNameString );
					else
						startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the read file name string" );
					}
				else
					startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given read file name string is too long" );
				}
			}
		else
			startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given read file is undefined" );
		}

	~FileItem()
		{
		if( readFileNameString_ != NULL )
			delete readFileNameString_;
		if( readFile_ != NULL )
			fclose( readFile_ );
		}


	// Protected virtual functions

	virtual void showString( bool isReturnQueryToPosition )
		{
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( readFileNameString_ != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status if not active
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, readFileNameString_ );
			}
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		// This is a virtual function. Therefore the given variables are unreferenced

		return true;	// Add at the beginning of the list
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		Item::toString( queryWordTypeNr );

		if( isInfoFile_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isInfoFile" );
			}

		if( readFileNameString_ != NULL )
			{
			sprintf( tempString, "%creadFileNameString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, readFileNameString_, QUERY_STRING_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}


		return commonVariables()->queryString;
		}


	// Protected functions

	void clearReadFile()
		{
		readFile_ = NULL;
		}

	bool isInfoFile()
		{
		return isInfoFile_;
		}

	char *readFileNameString()
		{
		return readFileNameString_;
		}

	FILE *readFile()
		{
		return readFile_;
		}

	FileItem *nextFileItem()
		{
		return (FileItem *)nextItem;
		}
	};

/*************************************************************************
 *	"He spoke, and the winds rose,
 *	stirring up the waves." (Psalm 107:25)
 *************************************************************************/
