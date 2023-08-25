﻿/*	Class:			WriteItem
 *	Parent class:	Item
 *	Purpose:		Temporarily storing info about a word
 *					during the process of writing a sentence
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

#include "GrammarItem.cpp"

class WriteItem : private Item
	{
	friend class WriteList;
	friend class WordWrite;

	// Private initialized variables

	unsigned short grammarLevel_ = NO_GRAMMAR_LEVEL;

	GrammarItem *startOfChoiceOrOptionGrammarItem_ = NULL;


	protected:
	// Protected initialized variables

	bool isSkipped = false;


	// Constructor

	WriteItem( bool _isSkipped, unsigned short grammarLevel, GrammarItem *startOfChoiceOrOptionGrammarItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WriteItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		grammarLevel_ = grammarLevel;

		// Protected initialized variables

		isSkipped = _isSkipped;

		// Checking private initialized variables

		if( ( startOfChoiceOrOptionGrammarItem_ = startOfChoiceOrOptionGrammarItem ) == NULL )
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given start of grammar choice or option grammar item is undefined" );
		}

	~WriteItem()
		{
		}


	// Protected virtual functions

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( startOfChoiceOrOptionGrammarItem_ == NULL ? false :
				( querySentenceNr == NO_SENTENCE_NR ? true : startOfChoiceOrOptionGrammarItem_->creationSentenceNr() == querySentenceNr ) &&
				( queryItemNr == NO_ITEM_NR ? true : startOfChoiceOrOptionGrammarItem_->itemNr() == queryItemNr ) );
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( isSkipped )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSkipped" );
			}

		sprintf( tempString, "%cgrammarLevel:%u", QUERY_SEPARATOR_CHAR, grammarLevel_ );
		strcat( queryString, tempString );

		if( startOfChoiceOrOptionGrammarItem_ != NULL )
			{
			sprintf( tempString, "%cstartOfChoiceOrOptionGrammarItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, startOfChoiceOrOptionGrammarItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, startOfChoiceOrOptionGrammarItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected functions

	unsigned short grammarLevel()
		{
		return grammarLevel_;
		}

	GrammarItem *startOfChoiceOrOptionGrammarItem()
		{
		return startOfChoiceOrOptionGrammarItem_;
		}

	WriteItem *nextWriteItem()
		{
		return (WriteItem *)nextItem;
		}
	};

/*************************************************************************
 *	"Those who are wise will take all this to heart;
 *	they will see in our history the faithful love
 *	of the Lord." (Psalm 107:43)
 *************************************************************************/
