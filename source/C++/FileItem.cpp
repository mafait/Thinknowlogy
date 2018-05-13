/*	Class:			FileItem
 *	Parent class:	Item
 *	Purpose:		To store info about the opened files
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

#include "Item.h"

class FileItem : private Item
	{
	friend class AdminReadFile;
	friend class FileList;

	// Private initialized variables

	bool isInfoFile_;
	bool isTestFile_;

	char *readFileNameString_;
	char *writeFileNameString_;

	FILE *readFile_;
	FILE *writeFile_;

	protected:
	// Constructor

	FileItem( bool isInfoFile, bool isTestFile, char *readFileNameString, char *writeFileNameString, FILE *readFile, FILE *writeFile, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		size_t fileNameStringLength;

		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "FileItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		isInfoFile_ = isInfoFile;
		isTestFile_ = isTestFile;

		readFileNameString_ = NULL;
		writeFileNameString_ = NULL;

		readFile_ = readFile;
		writeFile_ = writeFile;

		if( readFile_ != NULL )
			{
			if( readFileNameString != NULL )
				{
				if( ( fileNameStringLength = strlen( readFileNameString ) ) < MAX_SENTENCE_STRING_LENGTH )
					{
					if( ( readFileNameString_ = new char[fileNameStringLength + 1] ) != NULL )
						strcpy( readFileNameString_, readFileNameString );
					else
						startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the read file name string" );
					}
				else
					startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given read file name string is too long" );
				}

			if( writeFileNameString != NULL )
				{
				if( ( fileNameStringLength = strlen( writeFileNameString ) ) < MAX_SENTENCE_STRING_LENGTH )
					{
					if( ( writeFileNameString_ = new char[fileNameStringLength + 1] ) != NULL )
						strcpy( writeFileNameString_, writeFileNameString );
					else
						startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the write file name string" );
					}
				else
					startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given write file name string is too long" );
				}
			}
		else
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given read file is undefined" );
		}

	~FileItem()
		{
		if( readFileNameString_ != NULL )
			delete readFileNameString_;
		if( writeFileNameString_ != NULL )
			delete writeFileNameString_;
		if( readFile_ != NULL )
			fclose( readFile_ );
		if( writeFile_ != NULL )
			fclose( writeFile_ );
		}


	// Protected virtual functions

	virtual void displayString( bool isReturnQueryToPosition )
		{
		statusString[0] = statusChar();

		if( readFileNameString_ != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables()->queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( globalVariables()->queryString, readFileNameString_ );
			}
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.

		// Always add at the beginning of the list
		return true;
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( isInfoFile_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isInfoFile" );
			}

		if( isTestFile_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isTestFile" );
			}

		if( readFileNameString_ != NULL )
			{
			sprintf( tempString, "%creadFileNameString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, readFileNameString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		if( writeFileNameString_ != NULL )
			{
			sprintf( tempString, "%cwriteFileNameString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, writeFileNameString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected functions

	void clearReadFile()
		{
		readFile_ = NULL;
		}

	void clearWriteFile()
		{
		writeFile_ = NULL;
		}

	bool isInfoFile()
		{
		return isInfoFile_;
		}

	bool isTestFile()
		{
		return isTestFile_;
		}

	char *readFileNameString()
		{
		return readFileNameString_;
		}

	char *writeFileNameString()
		{
		return writeFileNameString_;
		}

	FILE *readFile()
		{
		return readFile_;
		}

	FILE *writeFile()
		{
		return writeFile_;
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
