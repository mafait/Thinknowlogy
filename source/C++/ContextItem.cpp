/*	Class:			ContextItem
 *	Parent class:	Item
 *	Purpose:		To store the context info of a word
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

#include "WordItem.h"

class ContextItem : private Item
	{
	friend class AdminSpecification;
	friend class ContextList;
	friend class WordItem;

	// Private initialized variables

	unsigned short contextWordTypeNr_ = NO_WORD_TYPE_NR;
	unsigned short specificationWordTypeNr_ = NO_WORD_TYPE_NR;

	unsigned int contextNr_ = NO_CONTEXT_NR;
	unsigned int spanishAmbiguousCollectionNr_ = NO_COLLECTION_NR;

	WordItem *specificationWordItem_ = NULL;


	protected:
	// Constructor

	ContextItem( unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "ContextItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		contextWordTypeNr_ = contextWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;

		contextNr_ = contextNr;
		spanishAmbiguousCollectionNr_ = spanishAmbiguousCollectionNr;

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
			if( globalVariables()->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables()->queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( globalVariables()->queryString, wordString );
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
		WordItem *thisWordItem = myWordItem();
		char *wordString;
		char *queryString;
		char *contextWordTypeString = thisWordItem->wordTypeNameString( contextWordTypeNr_ );
		char *specificationWordTypeString = thisWordItem->wordTypeNameString( specificationWordTypeNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( contextWordTypeString == NULL )
			sprintf( tempString, "%ccontextWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, contextWordTypeNr_ );
		else
			sprintf( tempString, "%ccontextWordType:%s%c%u", QUERY_SEPARATOR_CHAR, contextWordTypeString, QUERY_WORD_TYPE_CHAR, contextWordTypeNr_ );

		strcat( queryString, tempString );

		if( contextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%ccontextNr:%u", QUERY_SEPARATOR_CHAR, contextNr_ );
			strcat( queryString, tempString );
			}

		if( spanishAmbiguousCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%cspanishAmbiguousCollectionNr:%u", QUERY_SEPARATOR_CHAR, spanishAmbiguousCollectionNr_ );
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

	unsigned int spanishAmbiguousCollectionNr()
		{
		return spanishAmbiguousCollectionNr_;
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

/*************************************************************************
 *	"Enter his gates with thanksgiving;
 *	go into his courts with praise.
 *	Give thanks to him and praise his name." (Psalm 100:4)
 *************************************************************************/
