/*	Class:			FileList
 *	Parent class:	List
 *	Purpose:		To store file items
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

#ifndef FILELIST
#define FILELIST 1
#include "FileItem.cpp"
#include "FileResultType.cpp"
#include "List.h"

class FileList : private List
	{
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

	FileResultType createFileItem( bool isInfoFile, bool isTestFile, char *readFileNameString, char *writeFileNameString, FILE *readFile, FILE *writeFile )
		{
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createFileItem";

		if( commonVariables()->currentItemNr < MAX_ITEM_NR )
			{
			if( ( fileResult.createdFileItem = new FileItem( isInfoFile, isTestFile, readFileNameString, writeFileNameString, readFile, writeFile, commonVariables(), this, myWordItem() ) ) != NULL )
				{
				if( addItemToList( QUERY_ACTIVE_CHAR, fileResult.createdFileItem ) != RESULT_OK )
					addError( functionNameString, NULL, "I failed to add an active file item" );
				}
			else
				startError( functionNameString, NULL, "I failed to create a file item" );
			}
		else
			startError( functionNameString, NULL, "The current item number is undefined" );

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

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class FileList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class FileList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class FileList has replaced items." );

		searchItem = (FileItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextFileItem();
			delete deleteItem;
			}
		}


	// Protected functions

	bool isShowingLine()
		{
		FileItem *searchItem = firstActiveFileItem();

		while( searchItem != NULL )
			{
			if( searchItem->isInfoFile() ||
			searchItem->isTestFile() )
				return false;

			searchItem = searchItem->nextFileItem();
			}

		return true;
		}

	bool isCurrentlyTesting()
		{
		FileItem *searchItem = firstActiveFileItem();

		while( searchItem != NULL )
			{
			if( searchItem->isTestFile() )
				return true;

			searchItem = searchItem->nextFileItem();
			}

		return false;
		}

	bool isCurrentFileTestFile()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? false : currentFileItem->isTestFile() );
		}

	unsigned int currentFileSentenceNr()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? NO_SENTENCE_NR : currentFileItem->activeSentenceNr() );
		}

	ResultType closeCurrentFile( FileItem *closeFileItem )
		{
		FileItem *currentFileItem = firstActiveFileItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "closeCurrentFile";

		if( currentFileItem != NULL )
			{
			// Check to be sure to close the right file
			if( currentFileItem == closeFileItem )
				{
					if( currentFileItem->readFile() != NULL )
						{
						fclose( currentFileItem->readFile() );
						currentFileItem->clearReadFile();
						}
					if( currentFileItem->writeFile() != NULL )
						{
						fclose( currentFileItem->writeFile() );
						currentFileItem->clearWriteFile();
						}

				if( deleteItem( currentFileItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to delete a file item" );
				}
			else
				return startError( functionNameString, NULL, "The given file item isn't the current file" );
			}
		else
			return startError( functionNameString, NULL, "There is no file to close" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		FileItem *searchItem = firstActiveFileItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeFileItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a file item in the database" );
				}

			searchItem = searchItem->nextFileItem();
			}

		return RESULT_OK;
		}
*/
	FileResultType openFile( bool isAddingSubPath, bool isInfoFile, bool isTestFile, bool isReportingErrorIfFileDoesNotExist, const char *defaultSubPathString, const char *fileNameString, const char *testOutputFileSubPathString, const char *testReferenceFileSubPathString )
		{
		FileResultType fileResult;
		FILE *readFile;
		FILE *writeFile = NULL;
		char readFileNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char referenceFileNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char writeFileNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "openFile";

		if( defaultSubPathString != NULL )
			{
			if( fileNameString != NULL )
				{
				if( strlen( fileNameString ) > 0 )
					{
					if( isAddingSubPath &&
					// Skip absolute path
					fileNameString[0] != SYMBOL_SLASH &&
					fileNameString[0] != SYMBOL_BACK_SLASH &&
					!doesFileNameContainSubPath( fileNameString, defaultSubPathString ) )
						{
						strcat( readFileNameString, defaultSubPathString );

						if( testReferenceFileSubPathString != NULL )
							strcat( referenceFileNameString, testReferenceFileSubPathString );

						if( testOutputFileSubPathString != NULL )
							strcat( writeFileNameString, testOutputFileSubPathString );
						}

					strcat( readFileNameString, fileNameString );
					strcat( referenceFileNameString, fileNameString );
					strcat( writeFileNameString, fileNameString );

					if( !doesFileNameContainExtension( fileNameString ) )
						{
						strcat( readFileNameString, FILE_EXTENSION_STRING );
						strcat( referenceFileNameString, FILE_EXTENSION_STRING );
						strcat( writeFileNameString, FILE_EXTENSION_STRING );
						}

					if( ( readFile = fopen( readFileNameString, FILE_ATTRIBUTES_READ_TEXT_UTF_8 ) ) != NULL )
						{
						if( testReferenceFileSubPathString != NULL &&
						( fileResult.referenceFile = fopen( referenceFileNameString, FILE_ATTRIBUTES_READ_TEXT_UTF_8 ) ) == NULL )
							{
							if( isReportingErrorIfFileDoesNotExist )
								startError( functionNameString, NULL, "I couldn't open reference file for reading: \"", referenceFileNameString, "\"" );
							}

						if( testOutputFileSubPathString != NULL )
							{
							if( ( writeFile = fopen( writeFileNameString, FILE_ATTRIBUTES_WRITE_TEXT_UTF_8 ) ) == NULL )
								{
								if( isReportingErrorIfFileDoesNotExist )
									startError( functionNameString, NULL, "I failed to open file for writing: \"", writeFileNameString, "\"" );
								}
#ifndef _MSC_VER
							else
								// For compilers other than MS Visual Studio
								// Write UTF-8 BOM string
								fprintf( writeFile, "%c%c%c", FILE_UTF_8_BOM_CHAR_1, FILE_UTF_8_BOM_CHAR_2, FILE_UTF_8_BOM_CHAR_3 );
#endif
							}

						// Skip creating file item if opening files for comparing
						if( testReferenceFileSubPathString == NULL )
							{
							if( ( fileResult = createFileItem( isInfoFile, isTestFile, readFileNameString, ( testOutputFileSubPathString == NULL ? NULL : writeFileNameString ), readFile, writeFile ) ).result != RESULT_OK )
								{
								if( fileResult.createdFileItem != NULL )
									closeCurrentFile( fileResult.createdFileItem );

								addError( functionNameString, NULL, "I failed to create a file item" );
								}
							}
						else
							fileResult.outputFile = readFile;
						}
					else
						{
						if( isReportingErrorIfFileDoesNotExist )
							startError( functionNameString, NULL, "I couldn't open file for reading: \"", readFileNameString, "\"" );
						}
					}
				else
					startError( functionNameString, NULL, "The copied file name string is empty" );
				}
			else
				startError( functionNameString, NULL, "The given file name string is undefined" );
			}
		else
			startError( functionNameString, NULL, "The given default subpath string is undefined" );

		fileResult.result = commonVariables()->result;
		return fileResult;
		}

	FILE *currentReadFile()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? NULL : currentFileItem->readFile() );
		}

	FILE *currentWriteFile()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? NULL : currentFileItem->writeFile() );
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
