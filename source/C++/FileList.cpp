/*	Class:			FileList
 *	Parent class:	List
 *	Purpose:		To store file items
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

#include "FileItem.cpp"
#include "FileResultType.cpp"
#include "List.h"

class FileList : private List
	{
	friend class AdminItem;

	// Private functions

	void deleteFileList( FileItem *searchFileItem )
		{
		FileItem *deleteFileItem;

		while( searchFileItem != NULL )
			{
			deleteFileItem = searchFileItem;
			searchFileItem = searchFileItem->nextFileItem();
			delete deleteFileItem;
			}
		}

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

	FileItem *firstActiveFileItem()
		{
		return (FileItem *)firstActiveItem();
		}

	FileResultType createFileItem( bool isInfoFile, bool isTestFile, char *readFileNameString, char *writeFileNameString, FILE *readFile, FILE *writeFile )
		{
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createFileItem";

		if( ( fileResult.createdFileItem = new FileItem( isInfoFile, isTestFile, readFileNameString, writeFileNameString, readFile, writeFile, commonVariables(), inputOutput(), this, myWordItem() ) ) == NULL )
			return startFileResultError( functionNameString, "I failed to create a file item" );

		if( addItemToList( QUERY_ACTIVE_CHAR, fileResult.createdFileItem ) != RESULT_OK )
			return addFileResultError( functionNameString, "I failed to add an active file item" );

		return fileResult;
		}


	protected:
	// Constructor

	FileList( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( ADMIN_FILE_LIST_SYMBOL, "FileList", commonVariables, inputOutput, myWordItem );
		}

	~FileList()
		{
		deleteFileList( firstActiveFileItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class FileList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class FileList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class FileList has replaced items." );

		deleteFileList( (FileItem *)firstDeletedItem() );
		}


	// Protected functions

	bool isCurrentFileTestFile()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? false : currentFileItem->isTestFile() );
		}

	bool isCurrentlyTesting()
		{
		FileItem *searchFileItem = firstActiveFileItem();

		while( searchFileItem != NULL )
			{
			if( searchFileItem->isTestFile() )
				return true;

			searchFileItem = searchFileItem->nextFileItem();
			}

		return false;
		}

	bool isDisplayingLine()
		{
		FileItem *searchFileItem = firstActiveFileItem();

		while( searchFileItem != NULL )
			{
			if( searchFileItem->isInfoFile() ||
			searchFileItem->isTestFile() )
				return false;

			searchFileItem = searchFileItem->nextFileItem();
			}

		return true;
		}

	unsigned int currentFileSentenceNr()
		{
		FileItem *currentFileItem = firstActiveFileItem();
		return ( currentFileItem == NULL ? NO_SENTENCE_NR : currentFileItem->activeSentenceNr() );
		}

	signed char closeCurrentFile( FileItem *closeFileItem )
		{
		FileItem *currentFileItem = firstActiveFileItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "closeCurrentFile";

		if( currentFileItem == NULL )
			return startError( functionNameString, "There is no file to close" );

		// Check to be sure to close the right file
		if( currentFileItem != closeFileItem )
			return startError( functionNameString, "The given file item isn't the current file" );

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
			return addError( functionNameString, "I failed to delete a file item" );

		return RESULT_OK;
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

	FileResultType openFile( bool isAddingSubPath, bool isInfoFile, bool isTestFile, bool isReportingErrorIfFileDoesNotExist, const char *defaultSubPathString, const char *fileNameString, const char *testOutputFileSubPathString, const char *testReferenceFileSubPathString )
		{
		FileResultType fileResult;
		FILE *readFile;
		FILE *writeFile = NULL;
		char readFileNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char referenceFileNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char writeFileNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "openFile";

		if( defaultSubPathString == NULL )
			return startFileResultError( functionNameString, "The given default subpath string is undefined" );

		if( fileNameString == NULL )
			return startFileResultError( functionNameString, "The given file name string is undefined" );

		if( strlen( fileNameString ) == 0 )
			return startFileResultError( functionNameString, "The copied file name string is empty" );

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
					return startFileResultError( functionNameString, "I couldn't open reference file for reading: \"", referenceFileNameString, "\"" );
				}

			if( testOutputFileSubPathString != NULL )
				{
				if( ( writeFile = fopen( writeFileNameString, FILE_ATTRIBUTES_WRITE_TEXT_UTF_8 ) ) == NULL )
					{
					if( isReportingErrorIfFileDoesNotExist )
						return startFileResultError( functionNameString, "I failed to open file for writing: \"", writeFileNameString, "\"" );
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

					return addFileResultError( functionNameString, "I failed to create a file item" );
					}
				}
			else
				fileResult.outputFile = readFile;
			}
		else
			{
			if( isReportingErrorIfFileDoesNotExist )
				return startFileResultError( functionNameString, "I couldn't open file for reading: \"", readFileNameString, "\"" );
			}

		return fileResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	Praise the name of the Lord!
 *	Praise him, you who serve the Lord,
 *				you who serve in the house of the Lord,
 *				in the courts of the house of our God." (Psalm 135:1-2)
 *************************************************************************/
