/*
 *	Class:			MultipleWordItem
 *	Parent class:	Item
 *	Purpose:		To store info about multiple words
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

#include "WordItem.h"

class MultipleWordItem : private Item
	{
	friend class MultipleWordList;

	// Private loadable variables

	unsigned short wordTypeNr_;

	WordItem *multipleWordItem_;


	protected:
	// Constructor / deconstructor

	MultipleWordItem( unsigned short wordTypeNr, WordItem *multipleWordItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "MultipleWordItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		wordTypeNr_ = wordTypeNr;

		multipleWordItem_ = multipleWordItem;

		if( multipleWordItem_ == NULL )
			startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given multiple word item is undefined" );
		}


	// Protected virtual functions

	virtual void showWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( multipleWordItem_ != NULL &&
		( wordString = multipleWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status if not active
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( multipleWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : multipleWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : multipleWordItem_->itemNr() == queryItemNr ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	virtual ReferenceResultType findMatchingWordReferenceString( char *queryString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( multipleWordItem_ != NULL )
			{
			if( ( referenceResult = multipleWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find a matching word reference string for the multiple word word" );
			}

		return referenceResult;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *wordString;
		char *wordTypeString = myWordItem()->wordTypeNameString( wordTypeNr_ );

		Item::toString( queryWordTypeNr );

		if( wordTypeString == NULL )
			sprintf( tempString, "%cwordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );
		else
			sprintf( tempString, "%cwordType:%s%c%u", QUERY_SEPARATOR_CHAR, wordTypeString, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );

		strcat( commonVariables()->queryString, tempString );

		if( multipleWordItem_ != NULL )
			{
			sprintf( tempString, "%cmultipleWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, multipleWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, multipleWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );

			if( ( wordString = multipleWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( commonVariables()->queryString, tempString );
				}
			}

		return commonVariables()->queryString;
		}


	// Protected functions

	bool isSingularNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}

	unsigned short wordTypeNr()
		{
		return wordTypeNr_;
		}

	WordItem *multipleWordItem()
		{
		return multipleWordItem_;
		}

	MultipleWordItem *nextMultipleWordItem()
		{
		return (MultipleWordItem *)nextItem;
		}
	};

/*************************************************************************
 *	"The Lord is my light and my salvation -
 *	so why should I be afraid?
 *	The Lord is my fortress, protecting me from danger,light and my salvation -
 *	so why should I tremble?" (Psalm 27:1)
 *************************************************************************/
