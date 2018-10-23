/*	Class:			FileList
 *	Parent class:	List
 *	Purpose:		To store file items
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.IOException;

class FileList extends List
	{
	// Private methods

	private static boolean doesFileNameContainExtension( String fileNameString )
		{
		int position;

		if( fileNameString != null )
			{
			position = fileNameString.length();

			while( position > 0 &&
			fileNameString.charAt( position - 1 ) != Constants.SYMBOL_SLASH &&
			fileNameString.charAt( position - 1 ) != Constants.SYMBOL_BACK_SLASH )
				{
				if( fileNameString.charAt( --position ) == Constants.SYMBOL_COLON )
					return true;
				}
			}

		return false;
		}

	private FileItem firstActiveFileItem()
		{
		return (FileItem)firstActiveItem();
		}

	private FileResultType createFileItem( boolean isInfoFile, boolean isTestFile, String readFileNameString, String writeFileNameString, BufferedReader readFile, BufferedWriter writeFile )
		{
		FileResultType fileResult = new FileResultType();

		if( ( fileResult.createdFileItem = new FileItem( isInfoFile, isTestFile, readFileNameString, writeFileNameString, readFile, writeFile, this, myWordItem() ) ) == null )
			return startFileResultError( 1, "I failed to create a file item" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, fileResult.createdFileItem ) != Constants.RESULT_OK )
			return addFileResultError( 1, "I failed to add a file item" );

		return fileResult;
		}


	// Constructor

	protected FileList( WordItem myWordItem )
		{
		initializeListVariables( Constants.ADMIN_FILE_LIST_SYMBOL, "FileList", myWordItem );
		}


	// Protected methods

	protected boolean isCurrentFileTestFile()
		{
		FileItem currentFileItem = firstActiveFileItem();
		return ( currentFileItem == null ? false : currentFileItem.isTestFile() );
		}

	protected boolean isCurrentlyTesting()
		{
		FileItem searchFileItem = firstActiveFileItem();

		while( searchFileItem != null )
			{
			if( searchFileItem.isTestFile() )
				return true;

			searchFileItem = searchFileItem.nextFileItem();
			}

		return false;
		}

	protected boolean isDisplayingLine()
		{
		FileItem searchFileItem = firstActiveFileItem();

		while( searchFileItem != null )
			{
			if( searchFileItem.isInfoFile() ||
			searchFileItem.isTestFile() )
				return false;

			searchFileItem = searchFileItem.nextFileItem();
			}

		return true;
		}

	protected int currentFileSentenceNr()
		{
		FileItem currentFileItem = firstActiveFileItem();
		return ( currentFileItem == null ? Constants.NO_SENTENCE_NR : currentFileItem.activeSentenceNr() );
		}

	protected byte closeCurrentFile( FileItem closeFileItem )
		{
		FileItem currentFileItem = firstActiveFileItem();

		if( currentFileItem == null )
			return startError( 1, "There is no file to close" );

		// Check to be sure to close the right file
		if( currentFileItem != closeFileItem )
			return startError( 1, "The given file item isn't the current file" );

		try	{
			if( currentFileItem.readFile() != null )
				{
				currentFileItem.readFile().close();
				currentFileItem.clearReadFile();
				}
			}
		catch( IOException exception )
			{
			return startError( 1, "IOException: I couldn't close read file: \"" + currentFileItem.readFileNameString() + "\"" );
			}

		try	{
			if( currentFileItem.writeFile() != null )
				{
				currentFileItem.writeFile().close();
				currentFileItem.clearWriteFile();
				}
			}
		catch( IOException exception )
			{
			return startError( 1, "IOException: I couldn't close write file: \"" + currentFileItem.writeFileNameString() + "\"" );
			}

		if( deleteItem( currentFileItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to delete a file item" );

		return Constants.RESULT_OK;
		}

	protected BufferedReader currentReadFile()
		{
		FileItem currentFileItem;

		return ( ( currentFileItem = firstActiveFileItem() ) != null ?
				currentFileItem.readFile() : null );
		}

	protected BufferedWriter currentWriteFile()
		{
		FileItem currentFileItem;

		return ( ( currentFileItem = firstActiveFileItem() ) != null ?
				currentFileItem.writeFile() : null );
		}

	protected FileResultType openFile( boolean isAddingSubPath, boolean isInfoFile, boolean isTestFile, boolean isReportingErrorIfFileDoesNotExist, String defaultSubPathString, String fileNameString, String testOutputFileSubPathString, String testReferenceFileSubPathString )
		{
		BufferedReader readFile = null;
		BufferedWriter writeFile = null;
		StringBuffer readFileNameStringBuffer = new StringBuffer();
		StringBuffer referenceFileNameStringBuffer = new StringBuffer();
		StringBuffer writeFileNameStringBuffer = new StringBuffer();
		FileResultType fileResult = new FileResultType();

		if( defaultSubPathString == null )
			return startFileResultError( 1, "The given default subpath string is undefined" );

		if( fileNameString == null )
			return startFileResultError( 1, "The given file name string is undefined" );

		if( fileNameString.length() == 0 )
			return startFileResultError( 1, "The copied file name string buffer is empty" );

		// Skip absolute path
		if( fileNameString.charAt( 0 ) != Constants.SYMBOL_SLASH &&
		fileNameString.charAt( 0 ) != Constants.SYMBOL_BACK_SLASH &&
		fileNameString.indexOf( Constants.DOUBLE_COLON_STRING ) < 0 )
			{
			readFileNameStringBuffer.append( GlobalVariables.currentPathStringBuffer );
			referenceFileNameStringBuffer.append( GlobalVariables.currentPathStringBuffer );
			writeFileNameStringBuffer.append( GlobalVariables.currentPathStringBuffer );

			if( isAddingSubPath &&
			// File name doesn't contains sub-path
			fileNameString.indexOf( defaultSubPathString ) < 0 )
				{
				readFileNameStringBuffer.append( defaultSubPathString );

				if( testReferenceFileSubPathString != null )
					referenceFileNameStringBuffer.append( testReferenceFileSubPathString );

				if( testOutputFileSubPathString != null )
					writeFileNameStringBuffer.append( testOutputFileSubPathString );
				}
			}

		readFileNameStringBuffer.append( fileNameString );
		referenceFileNameStringBuffer.append( fileNameString );
		writeFileNameStringBuffer.append( fileNameString );

		if( !doesFileNameContainExtension( fileNameString ) )
			{
			readFileNameStringBuffer.append( Constants.FILE_EXTENSION_STRING );
			referenceFileNameStringBuffer.append( Constants.FILE_EXTENSION_STRING );
			writeFileNameStringBuffer.append( Constants.FILE_EXTENSION_STRING );
			}

		try	{
			// If not exists, try to open the file from the file system
			readFile = new BufferedReader( new InputStreamReader( new FileInputStream( readFileNameStringBuffer.toString() ), Constants.FILE_UTF_8_ENCODING_STRING ) );

			if( testReferenceFileSubPathString != null )
				fileResult.referenceFile = new BufferedReader( new InputStreamReader( new FileInputStream( referenceFileNameStringBuffer.toString() ), Constants.FILE_UTF_8_ENCODING_STRING ) );

			if( testOutputFileSubPathString != null )
				{
				writeFile = new BufferedWriter( new OutputStreamWriter( new FileOutputStream( writeFileNameStringBuffer.toString() ), Constants.FILE_UTF_8_ENCODING_STRING ) );
				writeFile.write( Constants.FILE_UTF_8_BOM_STRING );
				}

			// Skip creating file item if opening files for comparing
			if( testReferenceFileSubPathString == null )
				{
				if( ( fileResult = createFileItem( isInfoFile, isTestFile, readFileNameStringBuffer.toString(), ( testOutputFileSubPathString == null ? null : writeFileNameStringBuffer.toString() ), readFile, writeFile ) ).result != Constants.RESULT_OK )
					{
					if( fileResult.createdFileItem != null )
						closeCurrentFile( fileResult.createdFileItem );

					return addFileResultError( 1, "I failed to create a file item" );
					}
				}
			else
				fileResult.outputFile = readFile;
			}

		catch( IOException exception )
			{
			if( isReportingErrorIfFileDoesNotExist )
				{
				// The startup file is the first file to be read when this Java application is still packed in a Zip file
				if( fileNameString.equals( Constants.FILE_STARTUP_NAME_STRING ) )
					return startFileResultError( 1, "You are probably trying to start this Java application still being packed in a Zip file.\nYou need to unpack the Zip file to start this Java application" );

				return startFileResultError( 1, "I couldn't open " + ( readFile == null ? "file for reading: \"" + readFileNameStringBuffer + "\"" : ( testReferenceFileSubPathString != null && fileResult.referenceFile == null ? ( "reference file for reading: \"" + referenceFileNameStringBuffer + "\"" ) : ( testOutputFileSubPathString == null ? "an unknown file" : ( " file for writing: \"" + writeFileNameStringBuffer + "\"" ) ) ) ) );
				}
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
