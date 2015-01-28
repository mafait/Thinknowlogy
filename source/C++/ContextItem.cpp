/*
 *	Class:			ContextItem
 *	Parent class:	Item
 *	Purpose:		To store the context info of a word
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

#ifndef CONTEXTITEM
#define CONTEXTITEM 1
#include "WordItem.h"

class ContextItem : private Item
	{
	friend class AdminAssumption;
	friend class AdminContext;
	friend class AdminSpecification;
	friend class ContextList;
	friend class WordItem;

	// Private loadable variables

	bool isQuestion_;

	unsigned short contextWordTypeNr_;
	unsigned short specificationWordTypeNr_;

	unsigned int contextNr_;

	WordItem *specificationWordItem_;


	protected:
	// Constructor / deconstructor

	ContextItem( bool isQuestion, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "ContextItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		isQuestion_ = isQuestion;

		contextWordTypeNr_ = contextWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;

		contextNr_ = contextNr;

		specificationWordItem_ = specificationWordItem;
		}


	// Protected virtual functions

	virtual void showWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( specificationWordItem_ != NULL &&
		( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			if( !isActiveItem() )	// Show status if not active
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, wordString );
			}
		}

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( contextNr_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				contextNr_ > NO_CONTEXT_NR ) );
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( specificationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : specificationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : specificationWordItem_->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( contextWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr );
		}

	virtual ReferenceResultType findMatchingWordReferenceString( char *queryString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( specificationWordItem_ != NULL )
			{
			if( ( referenceResult = specificationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find a matching word reference string for the specification word" );
			}

		return referenceResult;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *wordString;
		char *contextWordTypeString = myWordItem()->wordTypeNameString( contextWordTypeNr_ );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );
		Item::toString( queryWordTypeNr );

		if( isQuestion_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isQuestion" );
			}

		if( contextWordTypeString == NULL )
			sprintf( tempString, "%ccontextWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, contextWordTypeNr_ );
		else
			sprintf( tempString, "%ccontextWordType:%s%c%u", QUERY_SEPARATOR_CHAR, contextWordTypeString, QUERY_WORD_TYPE_CHAR, contextWordTypeNr_ );

		strcat( commonVariables()->queryString, tempString );

		if( contextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%ccontextNr:%u", QUERY_SEPARATOR_CHAR, contextNr_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationWordTypeNr_ > WORD_TYPE_UNDEFINED )
			{
			if( specificationWordTypeString == NULL )
				sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
			else
				sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

			strcat( commonVariables()->queryString, tempString );
			}

		if( specificationWordItem_ != NULL )
			{
			sprintf( tempString, "%cspecificationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, specificationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, specificationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );

			if( ( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( commonVariables()->queryString, tempString );
				}
			}

		return commonVariables()->queryString;
		}


	// Protected functions

	bool isQuestion()
		{
		return isQuestion_;
		}

	unsigned short contextWordTypeNr()
		{
		return contextWordTypeNr_;
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
