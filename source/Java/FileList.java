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

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.IOException;

class FileList extends List
	{
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

	private FileResultType createFileItem( boolean isInfoFile, boolean isTestFile, String readFileNameString, String writeFileNameString, BufferedReader readFile, BufferedWriter writeFile )
		{
		FileResultType fileResult = new FileResultType();

		if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
			{
			if( ( fileResult.createdFileItem = new FileItem( isInfoFile, isTestFile, readFileNameString, writeFileNameString, readFile, writeFile, this, myWordItem() ) ) != null )
				{
				if( addItemToList( Constants.QUERY_ACTIVE_CHAR, fileResult.createdFileItem ) != Constants.RESULT_OK )
					addError( 1, null, "I failed to add an active file item" );
				}
			else
				startError( 1, null, "I failed to create a file item" );
			}
		else
			startError( 1, null, "The current item number is undefined" );

		fileResult.result = CommonVariables.result;
		return fileResult;
		}

	private FileItem firstActiveFileItem()
		{
		return (FileItem)firstActiveItem();
		}


	// Constructor / deconstructor

	protected FileList( WordItem myWordItem )
		{
		initializeListVariables( Constants.ADMIN_FILE_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected boolean isShowingLine()
		{
		FileItem searchItem = firstActiveFileItem();

		while( searchItem != null )
			{
			if( searchItem.isInfoFile() ||
			searchItem.isTestFile() )
				return false;

			searchItem = searchItem.nextFileItem();
			}

		return true;
		}

	protected boolean isCurrentlyTesting()
		{
		FileItem searchItem = firstActiveFileItem();

		while( searchItem != null )
			{
			if( searchItem.isTestFile() )
				return true;

			searchItem = searchItem.nextFileItem();
			}

		return false;
		}

	protected boolean isCurrentFileTestFile()
		{
		FileItem currentFileItem = firstActiveFileItem();
		return ( currentFileItem == null ? false : currentFileItem.isTestFile() );
		}

	protected int currentFileSentenceNr()
		{
		FileItem currentFileItem = firstActiveFileItem();
		return ( currentFileItem == null ? Constants.NO_SENTENCE_NR : currentFileItem.activeSentenceNr() );
		}

	protected byte closeCurrentFile( FileItem closeFileItem )
		{
		FileItem currentFileItem = firstActiveFileItem();

		if( currentFileItem != null )
			{
			// Check to be sure to close the right file
			if( currentFileItem == closeFileItem )
				{
				try	{
					if( currentFileItem.readFile() != null )
						{
						currentFileItem.readFile().close();
						currentFileItem.clearReadFile();
						}
					}
				catch( IOException exception )
					{
					return startError( 1, null, "I couldn't close read file: \"" + currentFileItem.readFileNameString() + "\"" );
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
					return startError( 1, null, "I couldn't close write file: \"" + currentFileItem.writeFileNameString() + "\"" );
					}

				if( deleteItem( currentFileItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to delete a file item" );
				}
			else
				return startError( 1, null, "The given file item isn't the current file" );
			}
		else
			return startError( 1, null, "There is no file to close" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		FileItem searchItem = firstActiveFileItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeFileItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a file item in the database" );
				}

			searchItem = searchItem.nextFileItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected FileResultType openFile( boolean isAddingSubPath, boolean isInfoFile, boolean isTestFile, boolean isReportingErrorIfFileDoesNotExist, String defaultSubPathString, String fileNameString, String testOutputFileSubPathString, String testReferenceFileSubPathString )
		{
		FileResultType fileResult = new FileResultType();
		BufferedReader readFile = null;
		BufferedWriter writeFile = null;
		StringBuffer readFileNameStringBuffer = new StringBuffer();
		StringBuffer referenceFileNameStringBuffer = new StringBuffer();
		StringBuffer writeFileNameStringBuffer = new StringBuffer();

		if( defaultSubPathString != null )
			{
			if( fileNameString != null )
				{
				if( fileNameString.length() > 0 )
					{
					// Skip absolute path
					if( fileNameString.charAt( 0 ) != Constants.SYMBOL_SLASH &&
					fileNameString.charAt( 0 ) != Constants.SYMBOL_BACK_SLASH &&
					fileNameString.indexOf( Constants.DOUBLE_COLON_STRING ) < 0 )
						{
						readFileNameStringBuffer.append( CommonVariables.currentPathStringBuffer );
						referenceFileNameStringBuffer.append( CommonVariables.currentPathStringBuffer );
						writeFileNameStringBuffer.append( CommonVariables.currentPathStringBuffer );

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

								addError( 1, null, "I failed to create a file item" );
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
								startSystemError( 1, null, "You are probably trying to start this Java application still being packed in a Zip file.\nYou need to unpack the Zip file to start this Java application" );
							else
								startError( 1, null, "I couldn't open " + ( readFile == null ? "file for reading: \"" + readFileNameStringBuffer + "\"" : ( testReferenceFileSubPathString != null && fileResult.referenceFile == null ? ( "reference file for reading: \"" + referenceFileNameStringBuffer + "\"" ) : ( testOutputFileSubPathString == null ? "an unknown file" : ( " file for writing: \"" + writeFileNameStringBuffer + "\"" ) ) ) ) );
							}
						}
					}
				else
					startError( 1, null, "The copied file name string buffer is empty" );
				}
			else
				startError( 1, null, "The given file name string is undefined" );
			}
		else
			startError( 1, null, "The given default subpath string is undefined" );

		fileResult.result = CommonVariables.result;
		return fileResult;
		}

	protected BufferedReader currentReadFile()
		{
		FileItem currentFileItem = firstActiveFileItem();
		return ( currentFileItem == null ? null : currentFileItem.readFile() );
		}

	protected BufferedWriter currentWriteFile()
		{
		FileItem currentFileItem = firstActiveFileItem();
		return ( currentFileItem == null ? null : currentFileItem.writeFile() );
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *	Praise the name of the Lord!
 *	Praise him, you who serve the Lord,
 *				you who serve in the house of the Lord,
 *				in the courts of the house of our God." (Psalm 135:1-2)
 *************************************************************************/
