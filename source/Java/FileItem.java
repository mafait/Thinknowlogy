/*	Class:			FileItem
 *	Parent class:	Item
 *	Purpose:		To store info about the opened files
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

import java.io.BufferedReader;
import java.io.BufferedWriter;

class FileItem extends Item
	{
	// Private initialized variables

	private boolean isInfoFile_;
	private boolean isTestFile_;

	private String readFileNameString_;
	private String writeFileNameString_;

	private BufferedReader readFile_;
	private BufferedWriter writeFile_;

	// Constructor

	protected FileItem( boolean isInfoFile, boolean isTestFile, String readFileNameString, String writeFileNameString, BufferedReader readFile, BufferedWriter writeFile, List myList, WordItem myWordItem )
		{

		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		isInfoFile_ = isInfoFile;
		isTestFile_ = isTestFile;

		readFileNameString_ = readFileNameString;
		writeFileNameString_ = writeFileNameString;

		readFile_ = readFile;
		writeFile_ = writeFile;

		if( readFile_ == null )
			startSystemError( 1, null, null, "The given read file is undefined" );
		}


	// Protected virtual methods

	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( readFileNameString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( readFileNameString_ );
			}
		}

	protected boolean isSorted( Item nextSortItem )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.

		// Always add at the beginning of the list
		return true;
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( isInfoFile_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isInfoFile" );

		if( isTestFile_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isTestFile" );

		if( readFileNameString_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "readFileNameString:" + Constants.QUERY_STRING_START_CHAR + readFileNameString_ + Constants.QUERY_STRING_END_CHAR );

		if( writeFileNameString_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "writeFileNameString:" + Constants.QUERY_STRING_START_CHAR + writeFileNameString_ + Constants.QUERY_STRING_END_CHAR );

		return queryStringBuffer;
		}


	// Protected methods

	protected void clearReadFile()
		{
		readFile_ = null;
		}

	protected void clearWriteFile()
		{
		writeFile_ = null;
		}

	protected boolean isInfoFile()
		{
		return isInfoFile_;
		}

	protected boolean isTestFile()
		{
		return isTestFile_;
		}

	protected String readFileNameString()
		{
		return readFileNameString_;
		}

	protected String writeFileNameString()
		{
		return writeFileNameString_;
		}

	protected BufferedReader readFile()
		{
		return readFile_;
		}

	protected BufferedWriter writeFile()
		{
		return writeFile_;
		}

	protected FileItem nextFileItem()
		{
		return (FileItem)nextItem;
		}
	};

/*************************************************************************
 *	"He spoke, and the winds rose,
 *	stirring up the waves." (Psalm 107:25)
 *************************************************************************/
