/*	Class:			ContextItem
 *	Parent class:	Item
 *	Purpose:		To store the context info of a word
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#ifndef CONTEXTITEM
#define CONTEXTITEM 1
#include "WordItem.h"

class ContextItem : private Item
	{
	friend class AdminSpecification;
	friend class ContextList;
	friend class WordItem;
	friend class WordSpecification;

	// Private initialized variables

	bool isCompoundCollectionSpanishAmbiguous_;

	unsigned short contextWordTypeNr_;
	unsigned short specificationWordTypeNr_;

	unsigned int contextNr_;

	WordItem *specificationWordItem_;


	protected:
	// Constructor

	ContextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem, CommonVariables *commonVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "ContextItem", commonVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		isCompoundCollectionSpanishAmbiguous_ = isCompoundCollectionSpanishAmbiguous;

		contextWordTypeNr_ = contextWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;

		contextNr_ = contextNr;

		specificationWordItem_ = specificationWordItem;
		}


	// Protected virtual functions

	virtual void displayWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;

		statusString[0] = statusChar();

		if( specificationWordItem_ != NULL &&
		( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( contextNr_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				contextNr_ > NO_CONTEXT_NR ) );
		}

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( specificationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : specificationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : specificationWordItem_->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( contextWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr );
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *wordString;
		char *queryString;
		char *contextWordTypeString = myWordItem()->wordTypeNameString( contextWordTypeNr_ );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

		if( contextWordTypeString == NULL )
			sprintf( tempString, "%ccontextWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, contextWordTypeNr_ );
		else
			sprintf( tempString, "%ccontextWordType:%s%c%u", QUERY_SEPARATOR_CHAR, contextWordTypeString, QUERY_WORD_TYPE_CHAR, contextWordTypeNr_ );

		strcat( queryString, tempString );

		if( isCompoundCollectionSpanishAmbiguous_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isCompoundCollectionSpanishAmbiguous" );
			}

		if( contextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%ccontextNr:%u", QUERY_SEPARATOR_CHAR, contextNr_ );
			strcat( queryString, tempString );
			}

		if( specificationWordTypeNr_ > NO_WORD_TYPE_NR )
			{
			if( specificationWordTypeString == NULL )
				sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
			else
				sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

			strcat( queryString, tempString );
			}

		if( specificationWordItem_ != NULL )
			{
			sprintf( tempString, "%cspecificationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, specificationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, specificationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
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

		if( specificationWordItem_ != NULL )
			return specificationWordItem_->findMatchingWordReferenceString( queryString );

		return boolResult;
		}


	// Protected functions

	bool isCompoundCollectionSpanishAmbiguous()
		{
		return isCompoundCollectionSpanishAmbiguous_;
		}

	unsigned short contextWordTypeNr()
		{
		return contextWordTypeNr_;
		}

	unsigned short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
		}

	unsigned int contextNr()
		{
		return contextNr_;
		}

	ContextItem *nextContextItem()
		{
		return (ContextItem *)nextItem;
		}

	WordItem *specificationWordItem()
		{
		return specificationWordItem_;
		}
	};
#endif

/*************************************************************************
 *	"Enter his gates with thanksgiving;
 *	go into his courts with praise.
 *	Give thanks to him and praise his name." (Psalm 100:4)
 *************************************************************************/
