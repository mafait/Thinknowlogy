/*	Class:			MultipleWordItem
 *	Parent class:	Item
 *	Purpose:		Storing info about multiple words
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

#include "WordItem.h"

class MultipleWordItem : private Item
	{
	friend class MultipleWordList;

	// Private initialized variables

	unsigned short nWordParts_ = 0;
	unsigned short wordTypeLanguageNr_ = NO_LANGUAGE_NR;
	unsigned short wordTypeNr_ = NO_WORD_TYPE_NR;

	WordItem *multipleWordItem_ = NULL;


	protected:
	// Constructor

	MultipleWordItem( unsigned short nWordParts, unsigned short wordTypeLanguageNr, unsigned short wordTypeNr, WordItem *multipleWordItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "MultipleWordItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		nWordParts_ = nWordParts;
		wordTypeLanguageNr_ = wordTypeLanguageNr;
		wordTypeNr_ = wordTypeNr;

		// Checking private initialized variables

		if( ( multipleWordItem_ = multipleWordItem ) == NULL )
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given multiple word item is undefined" );
		}


	// Protected virtual functions

	virtual void displayWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;

		statusString[0] = statusChar();

		if( multipleWordItem_ != NULL &&
		( wordString = multipleWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables()->queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( globalVariables()->queryString, wordString );
			}
		}

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( multipleWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : multipleWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : multipleWordItem_->itemNr() == queryItemNr ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		WordItem *_myWordItem = myWordItem();
		char *languageNameString = _myWordItem->languageNameString( wordTypeLanguageNr_ );
		char *queryString;
		char *wordString;
		char *wordTypeString = _myWordItem->wordTypeNameString( wordTypeNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		sprintf( tempString, "%cnWordParts:%u", QUERY_SEPARATOR_CHAR, nWordParts_ );
		strcat( queryString, tempString );

		if( wordTypeLanguageNr_ > NO_LANGUAGE_NR )
			{
			if( languageNameString == NULL )
				sprintf( tempString, "%cwordTypeLanguageNr:%u", QUERY_SEPARATOR_CHAR, wordTypeLanguageNr_ );
			else
				sprintf( tempString, "%cwordTypeLanguageNr:%s", QUERY_SEPARATOR_CHAR, languageNameString );

			strcat( queryString, tempString );
			}

		if( wordTypeString == NULL )
			sprintf( tempString, "%cwordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );
		else
			sprintf( tempString, "%cwordType:%s%c%u", QUERY_SEPARATOR_CHAR, wordTypeString, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );

		strcat( queryString, tempString );

		if( multipleWordItem_ != NULL )
			{
			sprintf( tempString, "%cmultipleWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, multipleWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, multipleWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = multipleWordItem_->wordTypeString( true, wordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		return queryString;
		}

	virtual BoolResultType findMatchingWordReferenceString( char *queryString )
		{
		BoolResultType boolResult;

		if( multipleWordItem_ != NULL )
			return multipleWordItem_->findMatchingWordReferenceString( queryString );

		return boolResult;
		}


	// Protected functions

	unsigned short nWordParts()
		{
		return nWordParts_;
		}
/*
	unsigned short wordTypeLanguageNr()
		{
		return wordTypeLanguageNr_;
		}
*/
	unsigned short wordTypeNr()
		{
		return wordTypeNr_;
		}

	MultipleWordItem *nextMultipleWordItem()
		{
		return (MultipleWordItem *)nextItem;
		}

	WordItem *multipleWordItem()
		{
		return multipleWordItem_;
		}
	};

/*************************************************************************
 *	"The Lord is my light and my salvation -
 *	so why should I be afraid?
 *	The Lord is my fortress, protecting me from danger,light and my salvation -
 *	so why should I tremble?" (Psalm 27:1)
 *************************************************************************/
