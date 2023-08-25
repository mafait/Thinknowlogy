/*	Class:			FileItem
 *	Parent class:	Item
 *	Purpose:		Storing info about the opened files
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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

package org.mafait.thinknowlogy;

import java.io.BufferedReader;
import java.io.BufferedWriter;

class FileItem extends Item
	{
	// Private initialized variables

	private boolean isInfoFile_ = false;
	private boolean isTestFile_ = false;

	private String readFileNameString_ = null;
	private String writeFileNameString_ = null;

	private BufferedReader readFile_ = null;
	private BufferedWriter writeFile_ = null;

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

	@Override
	protected void displayString( boolean isReturnQueryToPosition )
		{
		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( readFileNameString_ != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( readFileNameString_ );
			}
		}

	@Override
	protected boolean isSorted( Item _nextSortItem )
		{
		// This is a virtual method. Therefore, the given variables are unreferenced.

		// Always add at the beginning of the list
		return true;
		}

	@Override
	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

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
	}

/*************************************************************************
 *	"He spoke, and the winds rose,
 *	stirring up the waves." (Psalm 107:25)
 *************************************************************************/
