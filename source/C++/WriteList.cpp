/*	Class:			WriteList
 *	Parent class:	List
 *	Purpose:		To temporarily store write items
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

#include "List.h"
#include "WriteItem.cpp"

class WriteList : private List
	{
	friend class WordItem;

	// Private functions

	void deleteWriteList( WriteItem *searchWriteItem )
		{
		WriteItem *deleteWriteItem;

		while( searchWriteItem != NULL )
			{
			deleteWriteItem = searchWriteItem;
			searchWriteItem = searchWriteItem->nextWriteItem();
			delete deleteWriteItem;
			}
		}

	protected:
	// Constructor

	WriteList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( WORD_WRITE_LIST_SYMBOL, "WriteList", globalVariables, inputOutput, myWordItem );
		}

	~WriteList()
		{
		deleteWriteList( firstActiveWriteItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WriteList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class WriteList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class WriteList has replaced items." );

		deleteWriteList( (WriteItem *)firstDeletedItem() );
		}


	// Protected virtual functions

	virtual bool isTemporaryList()
		{
		return true;
		}


	// Protected functions

	signed char createWriteItem( bool isSkipped, unsigned short grammarLevel, GrammarItem *startOfChoiceOrOptionGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createWriteItem";

		if( addItemToList( QUERY_ACTIVE_CHAR, new WriteItem( isSkipped, grammarLevel, startOfChoiceOrOptionGrammarItem, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add a write item" );

		return RESULT_OK;
		}

	WriteItem *firstActiveWriteItem()
		{
		return (WriteItem *)firstActiveItem();
		}
	};

/*************************************************************************
 *	God says, "At the time I have planned,
 *	I will bring justice against the wicked.
 *	When the earth quakes and its people live in turmoil,
 *	I am the one that keeps its foundations firm." (Psalm 75:2-3)
 *************************************************************************/
