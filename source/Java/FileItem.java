/*
 *	Class:			FileItem
 *	Parent class:	Item
 *	Purpose:		To store info about the opened files
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

import java.io.BufferedReader;

class FileItem extends Item
	{
	// Private loadable variables

	private boolean isInfoFile_;

	private String readFileNameString_;

	private BufferedReader readFile_;

	// Constructor / deconstructor

	protected FileItem( boolean isInfoFile, String readFileNameString, BufferedReader readFile, List myList, WordItem myWordItem )
		{

		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

		isInfoFile_ = isInfoFile;

		readFileNameString_ = readFileNameString;
		readFile_ = readFile;

		if( readFile_ == null )
			startSystemErrorInItem( 1, null, null, "The given read file is undefined" );
		}


	// Protected virtual methods

	protected void showString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( readFileNameString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			if( !isActiveItem() )	// Show status when not active
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( readFileNameString_ );
			}
		}

	protected boolean isSorted( Item nextSortItem )
		{
		// This is a virtual method. Therefore the given variables are unreferenced

		return true;	// Add at the beginning of the list
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		baseToStringBuffer( queryWordTypeNr );

		if( isInfoFile_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isInfoFile" );

		if( readFileNameString_ != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "readFileNameString:" + Constants.QUERY_STRING_START_CHAR + readFileNameString_ + Constants.QUERY_STRING_END_CHAR );


		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

	protected void clearReadFile()
		{
		readFile_ = null;
		}

	protected boolean isInfoFile()
		{
		return isInfoFile_;
		}

	protected String readFileNameString()
		{
		return readFileNameString_;
		}

	protected BufferedReader readFile()
		{
		return readFile_;
		}

	protected FileItem nextFileItem()
		{
		return (FileItem)nextItem;
		}
	};

/*************************************************************************
 *
 *	"He spoke, and the winds rose,
 *	stirring up the waves." (Psalm 107:25)
 *
 *************************************************************************/
