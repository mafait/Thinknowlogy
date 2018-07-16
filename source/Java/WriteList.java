/*	Class:			WriteList
 *	Parent class:	List
 *	Purpose:		To temporarily store write items
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

class WriteList extends List
	{
	// Constructor

	protected WriteList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_WRITE_LIST_SYMBOL, "WriteList", myWordItem );
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return true;
		}


	// Protected methods

	protected byte checkGrammarItemForUsage( GrammarItem unusedGrammarItem )
		{
		WriteItem searchWriteItem = firstActiveWriteItem();

		if( unusedGrammarItem == null )
			return startError( 1, "The given unused grammar item is undefined" );

		while( searchWriteItem != null )
			{
			if( searchWriteItem.startOfChoiceOrOptionGrammarItem() == unusedGrammarItem )
				return startError( 1, "The start of choice or option grammar item is still in use" );

			searchWriteItem = searchWriteItem.nextWriteItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte createWriteItem( boolean isSkipped, short grammarLevel, GrammarItem startOfChoiceOrOptionGrammarItem )
		{
		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new WriteItem( isSkipped, grammarLevel, startOfChoiceOrOptionGrammarItem, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add a write item" );

		return Constants.RESULT_OK;
		}

	protected WriteItem firstActiveWriteItem()
		{
		return (WriteItem)firstActiveItem();
		}
	};

/*************************************************************************
 *	God says, "At the time I have planned,
 *	I will bring justice against the wicked.
 *	When the earth quakes and its people live in turmoil,
 *	I am the one that keeps its foundations firm." (Psalm 75:2-3)
 *************************************************************************/
