/*
 *	Class:			WriteList
 *	Parent class:	List
 *	Purpose:		To temporarily store write items
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
	friend class WordWriteSentence;

	protected:
	// Constructor / deconstructor

	WriteList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_WRITE_LIST_SYMBOL, "WriteList", commonVariables, myWordItem );
		}

	~WriteList()
		{
		WriteItem *deleteItem;
		WriteItem *searchItem = firstActiveWriteItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWriteItem();
			delete deleteItem;
			}

		searchItem = (WriteItem *)firstReplacedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWriteItem();
			delete deleteItem;
			}

		searchItem = (WriteItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWriteItem();
			delete deleteItem;
			}
		}


	// Protected virtual functions

	virtual bool isTemporaryList()
		{
		return true;
		}


	// Protected functions

	ResultType checkGrammarItemForUsage( GrammarItem *unusedGrammarItem )
		{
		WriteItem *searchItem = firstActiveWriteItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkGrammarItemForUsage";

		if( unusedGrammarItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->startOfChoiceOrOptionGrammarItem() == unusedGrammarItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The start of choice or option grammar item is still in use" );

				searchItem = searchItem->nextWriteItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given unused grammar item is undefined" );

		return RESULT_OK;
		}

	ResultType createWriteItem( bool isSkipped, unsigned short grammarLevel, GrammarItem *startOfChoiceOrOptionGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWriteItem";

		if( commonVariables()->currentItemNr < MAX_ITEM_NR )
			{
			if( addItemToList( QUERY_ACTIVE_CHAR, new WriteItem( isSkipped, grammarLevel, startOfChoiceOrOptionGrammarItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
				return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active write item" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );

		return RESULT_OK;
		}

	WriteItem *firstActiveWriteItem()
		{
		return (WriteItem *)firstActiveItem();
		}
	};

/*************************************************************************
 *	"God says, "At the time I have planned,
 *	I will bring justice against the wicked.
 *	When the earth quackes and its people live in turmoil,
 *	I am the one that keeps its foundations firm." (Psalm 75:2-3)
 *************************************************************************/
