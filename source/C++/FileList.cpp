/*
 *	Class:			FileList
 *	Parent class:	List
 *	Purpose:		To store file items
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

#ifndef FILELIST
#define FILELIST 1
#include "FileItem.cpp"
#include "FileResultType.cpp"
#include "List.h"

class FileList : private List
	{
	friend class AdminImperative;
	friend class AdminReadSentence;
	friend class AdminItem;
	friend class AdminReadFile;

	// Private common functions

	bool doesFileNameContainSubPath( const char *fileNameString, const char *subPathString )
		{
		size_t position = 0;
		size_t fileNameStringLength;
		size_t subPathStringLength;
		size_t lengthDifference;

		if( fileNameString != NULL &&
		subPathString != NULL )
			{
			fileNameStringLength = strlen( fileNameString );
			subPathStringLength = strlen( subPathString );

			if( fileNameStringLength > subPathStringLength )
				{
				lengthDifference = ( fileNameStringLength - subPathStringLength );

				while( position < lengthDifference )
					{
					if( strncmp( &fileNameString[position], subPathString, subPathStringLength ) == 0 )
						return true;

					position++;
					}
				}
			}

		return false;
		}

	bool doesFileNameContainExtension( const char *fileNameString )
		{
		size_t position;

		if( fileNameString != NULL )
			{
			position = strlen( fileNameString );

			while( position > 0 &&
			fileNameString[position - 1] != SYMBOL_SLASH &&
			fileNameString[position - 1] != SYMBOL_BACK_SLASH )
				{
				if( fileNameString[--position] == SYMBOL_COLON )
					return true;
				}
			}

		return false;
		}

	FileResultType createFileItem( bool isInfoFile, char *readFileNameString, FILE *readFile )
		{
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createFileItem";

		if( commonVariables()->currentItemNr < MAX_ITEM_NR )
			{
			if( ( fileResult.createdFileItem = new FileItem( isInfoFile, readFileNameString, readFile, commonVariables(), this, myWordItem() ) ) != NULL )
				{
				if( addItemToList( QUERY_ACTIVE_CHAR, fileResult.createdFileItem ) != RESULT_OK )
					addError( functionNameString, NULL, NULL, "I failed to add an active file item" );
				}
			else
				startError( functionNameString, NULL, NULL, "I failed to create a file item" );
			}
		else
			startError( functionNameString, NULL, NULL, "The current item number is undefined" );

		fileResult.result = commonVariables()->result;
		return fileResult;
		}

	FileItem *firstActiveFileItem()
		{
		return (FileItem *)firstActiveItem();
		}


	protected:
	// Constructor / deconstructor

	FileList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( ADMIN_FILE_LIST_SYMBOL, "FileList", commonVariables, myWordItem );
		}

	~FileList()
		{
		FileItem *deleteItem;
		FileItem *searchItem = firstActiveFileItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextFileItem();
			delete deleteItem;
			}

		searchItem = (FileItem *)firstReplacedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextFileItem();
			delete deleteItem;
			}

		searchItem = (FileItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextFileItem();
			delete deleteItem;
			}
		}


	// Protected functions

	bool showLine()
		{
		FileItem *searchItem = firstActiveFileItem();

		while( searchItem != NULL )
			{
			if( searchItem->isInfoFile() )
				return false;

			searchItem = searchItem->nextFileItem();
			}

		return true;
		}

	ResultType closeCurrentFile( FileItem *closeFileItem )
		{
		FileItem *currentFileItem = firstActiveFileItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "closeCurrentFile";

		if( currentFileItem != NULL )
			{
			if( currentFileItem == closeFileItem )	// Check to be sure to close the right file
				{
					if( currentFileItem->readFile() != NULL )
						{
						fclose( currentFileItem->readFile() );
						currentFileItem->clearReadFile();
						}

				if( deleteItem( false, currentFileItem ) != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to delete a file item" );
				}
			else
				return startError( functionNameString, NULL, NULL, "The given file item isn't the current file" );
			}
		else
			return startError( functionNameString, NULL, NULL, "There is no file to close" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		// Not fully implemented yet
		FileItem *searchItem = firstActiveFileItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeFileItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store a file item in the database" );
				}

			searchItem = searchItem->nextFileItem();
			}

		searchItem = firstReplacedFileItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeFileItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced file item in the database" );
				}

			searchItem = searchItem->nextFileItem();
			}

		return RESULT_OK;
		}
*/
	FileResultType openFile( bool isAddingSubPath, bool isInfoFile, bool reportErrorIfFileDoesNotExist, const char *defaultSubPathString, const char *fileNameString )
		{
		FileResultType fileResult;
		FILE *readFile;
		char readFileNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "openFile";

		if( defaultSubPathString != NULL )
			{
			if( fileNameString != NULL )
				{
				if( strlen( fileNameString ) > 0 )
					{
					if( isAddingSubPath &&
					fileNameString[0] != SYMBOL_SLASH &&		// Skip absolute path
					fileNameString[0] != SYMBOL_BACK_SLASH &&
					!doesFileNameContainSubPath( fileNameString, defaultSubPathString ) )
						strcat( readFileNameString, defaultSubPathString );

					strcat( readFileNameString, fileNameString );

					if( !doesFileNameContainExtension( fileNameString ) )
						strcat( readFileNameString, FILE_EXTENSION_STRING );

					if( ( readFile = fopen( readFileNameString, FILE_ATTRIBUTES_READ_TEXT_UTF_8 ) ) != NULL )
						{
						if( ( fileResult = createFileItem( isInfoFile, readFileNameString, readFile ) ).result != RESULT_OK )
							{
							if( fileResult.createdFileItem != NULL )
								closeCurrentFile( fileResult.createdFileItem );

							addError( functionNameString, NULL, NULL, "I failed to create a file item" );
							}
						}
					else
						{
						if( reportErrorIfFileDoesNotExist )
							startError( functionNameString, NULL, NULL, "I couldn't open file for reading: \"", readFileNameString, "\"" );
						}
					}
				else
					startError( functionNameString, NULL, NULL, "The copied file name string is empty" );
				}
			else
				startError( functionNameString, NULL, NULL, "The given file name string is undefined" );
			}
		else
			startError( functionNameString, NULL, NULL, "The given default subpath string is undefined" );

		fileResult.result = commonVariables()->result;
		return fileResult;
		}

	char *currentReadFileNameString()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? NULL : currentFileItem->readFileNameString() );
		}

	FILE *currentReadFile()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? NULL : currentFileItem->readFile() );
		}
	};
#endif

/*************************************************************************
 *	"Praise the Lord!
 *	Praise the name of the Lord!
 *	Praise him, you who serve the Lord,
 *				you who serve in the house of the Lord,
 *				in the courts of the house of our God." (Psalm 135:1-2)
 *************************************************************************/
