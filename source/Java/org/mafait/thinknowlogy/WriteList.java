/*	Class:			WriteList
 *	Parent class:	List
 *	Purpose:		Temporarily storing write items
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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

class WriteList extends List
	{
	// Constructor

	protected WriteList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_WRITE_LIST_SYMBOL, myWordItem );
		}


	// Protected virtual methods

	@Override
	protected boolean isTemporaryList()
		{
		return true;
		}


	// Protected methods

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
	}

/*************************************************************************
 *	God says, "At the time I have planned,
 *	I will bring justice against the wicked.
 *	When the earth quakes and its people live in turmoil,
 *	I am the one that keeps its foundations firm." (Psalm 75:2-3)
 *************************************************************************/
