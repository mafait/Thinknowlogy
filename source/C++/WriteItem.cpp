/*	Class:			WriteItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store info about a word
 *					during the process of writing a sentence
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

#include "GrammarItem.cpp"

class WriteItem : private Item
	{
	friend class WriteList;
	friend class WordWriteSentence;

	// Private loadable variables

	unsigned short grammarLevel_;

	GrammarItem *startOfChoiceOrOptionGrammarItem_;


	protected:
	// Protected loadable variables

	bool isSkipped;


	// Constructor / deconstructor

	WriteItem( bool _isSkipped, unsigned short grammarLevel, GrammarItem *startOfChoiceOrOptionGrammarItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WriteItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		grammarLevel_ = grammarLevel;

		// Protected loadable variables

		isSkipped = _isSkipped;

		if( ( startOfChoiceOrOptionGrammarItem_ = startOfChoiceOrOptionGrammarItem ) == NULL )
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given start of grammar choice or option grammar item is undefined" );
		}

	~WriteItem()
		{
		}


	// Protected virtual functions

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( startOfChoiceOrOptionGrammarItem_ == NULL ? false :
				( querySentenceNr == NO_SENTENCE_NR ? true : startOfChoiceOrOptionGrammarItem_->creationSentenceNr() == querySentenceNr ) &&
				( queryItemNr == NO_ITEM_NR ? true : startOfChoiceOrOptionGrammarItem_->itemNr() == queryItemNr ) );
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		Item::toString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

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
