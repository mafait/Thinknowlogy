/*	Class:			CollectionItem
 *	Parent class:	List
 *	Purpose:		To store collections of a word
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

#include "WordItem.h"

class CollectionItem : private Item
	{
	friend class CollectionList;

	// Private loadable variables

	bool isExclusiveSpecification_;

	unsigned short collectionOrderNr_;
	unsigned short collectionWordTypeNr_;
	unsigned short commonWordTypeNr_;

	unsigned int collectionNr_;

	WordItem *collectionWordItem_;
	WordItem *commonWordItem_;
	WordItem *compoundGeneralizationWordItem_;


	protected:
	// Constructor / deconstructor

	CollectionItem( bool isExclusiveSpecification, unsigned short collectionOrderNr, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "CollectionItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		isExclusiveSpecification_ = isExclusiveSpecification;

		collectionOrderNr_ = collectionOrderNr;
		collectionWordTypeNr_ = collectionWordTypeNr;
		commonWordTypeNr_ = commonWordTypeNr;

		collectionNr_ = collectionNr;

		collectionWordItem_ = collectionWordItem;
		commonWordItem_ = commonWordItem;
		compoundGeneralizationWordItem_ = compoundGeneralizationWordItem;
		}


	// Protected virtual functions

	virtual void showWordReferences( bool isReturnQueryToPosition )
		{
		char *queryString;
		char *wordString;

		statusString[0] = statusChar();

		queryString = commonVariables()->queryString;

		if( collectionWordItem_ != NULL &&
		( wordString = collectionWordItem_->wordTypeString( true, collectionWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}

		if( commonWordItem_ != NULL &&
		( wordString = commonWordItem_->wordTypeString( true, commonWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery ||
			strlen( queryString ) > 0 )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}

		if( compoundGeneralizationWordItem_ != NULL &&
		( wordString = compoundGeneralizationWordItem_->wordTypeString( true, commonWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery ||
			strlen( queryString ) > 0 )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}
		}

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( collectionOrderNr_ == queryParameter ||
				collectionNr_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( collectionOrderNr_ > NO_ORDER_NR ||
				collectionNr_ > NO_COLLECTION_NR ) ) );
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( collectionWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : collectionWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : collectionWordItem_->itemNr() == queryItemNr ) ) ||

				( commonWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : commonWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : commonWordItem_->itemNr() == queryItemNr ) ) ||

				( compoundGeneralizationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : compoundGeneralizationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : compoundGeneralizationWordItem_->itemNr() == queryItemNr ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( collectionWordTypeNr_ == queryWordTypeNr ||
				commonWordTypeNr_ == queryWordTypeNr );
		}

	virtual ReferenceResultType findMatchingWordReferenceString( char *queryString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( collectionWordItem_ != NULL )
			{
			if( ( referenceResult = collectionWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addError( functionNameString, NULL, "I failed to find a matching word reference string for the collected word item" );
			}

		if( commonVariables()->result == RESULT_OK &&
		commonWordItem_ != NULL )
			{
			if( ( referenceResult = commonWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addError( functionNameString, NULL, "I failed to find a matching word reference string for the common word item" );
			}

		if( commonVariables()->result == RESULT_OK &&
		compoundGeneralizationWordItem_ != NULL )
			{
			if( ( referenceResult = compoundGeneralizationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				addError( functionNameString, NULL, "I failed to find a matching word reference string for the compound word item" );
			}

		referenceResult.result = commonVariables()->result;
		return referenceResult;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *wordString;
		char *queryString;
		char *collectionWordTypeString = myWordItem()->wordTypeNameString( collectionWordTypeNr_ );
		char *commonWordTypeString = myWordItem()->wordTypeNameString( commonWordTypeNr_ );

		Item::toString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

		if( isExclusiveSpecification_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isExclusiveSpecification" );
			}

		if( collectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%ccollectionNr:%u", QUERY_SEPARATOR_CHAR, collectionNr_ );
			strcat( queryString, tempString );
			}

		if( collectionOrderNr_ > NO_ORDER_NR )
			{
			sprintf( tempString, "%ccollectionOrderNr:%u", QUERY_SEPARATOR_CHAR, collectionOrderNr_ );
			strcat( queryString, tempString );
			}

		if( collectionWordTypeString == NULL )
			sprintf( tempString, "%ccollectionWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, collectionWordTypeNr_ );
		else
			sprintf( tempString, "%ccollectionWordType:%s%c%u", QUERY_SEPARATOR_CHAR, collectionWordTypeString, QUERY_WORD_TYPE_CHAR, collectionWordTypeNr_ );

		strcat( queryString, tempString );

		if( collectionWordItem_ != NULL )
			{
			sprintf( tempString, "%ccollectionWordItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, collectionWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, collectionWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = collectionWordItem_->wordTypeString( true, collectionWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( commonWordTypeString == NULL )
			sprintf( tempString, "%ccommonWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, commonWordTypeNr_ );
		else
			sprintf( tempString, "%ccommonWordType:%s%c%u", QUERY_SEPARATOR_CHAR, commonWordTypeString, QUERY_WORD_TYPE_CHAR, commonWordTypeNr_ );

		strcat( queryString, tempString );

		if( commonWordItem_ != NULL )
			{
			sprintf( tempString, "%ccommonWordItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, commonWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, commonWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = commonWordItem_->wordTypeString( true, commonWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( compoundGeneralizationWordItem_ != NULL )
			{
			sprintf( tempString, "%ccompoundGeneralizationWordItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, compoundGeneralizationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, compoundGeneralizationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = compoundGeneralizationWordItem_->wordTypeString( true, commonWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		return queryString;
		}


	// Protected functions

	bool hasFemaleCollectionWord()
		{
		return ( collectionWordItem_ != NULL &&
				collectionWordItem_->isFemale() );
		}

	bool isCompoundGeneralization()
		{
		return ( compoundGeneralizationWordItem_ != NULL );
		}

	bool isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	bool isMatchingCollectionWordTypeNr( unsigned short collectionWordTypeNr )
		{
		return isMatchingWordType( collectionWordTypeNr_, collectionWordTypeNr );
		}

	unsigned short collectionOrderNr()
		{
		return collectionOrderNr_;
		}

	unsigned short collectionWordTypeNr()
		{
		return collectionWordTypeNr_;
		}

	unsigned short commonWordTypeNr()
		{
		return commonWordTypeNr_;
		}

	unsigned int collectionNr()
		{
		return collectionNr_;
		}

	WordItem *collectionWordItem()
		{
		return collectionWordItem_;
		}

	WordItem *commonWordItem()
		{
		return commonWordItem_;
		}

	WordItem *compoundGeneralizationWordItem()
		{
		return compoundGeneralizationWordItem_;
		}

	CollectionItem *nextCollectionItem()
		{
		return (CollectionItem *)nextItem;
		}
	};

/*************************************************************************
 *	"Let them praise to Lord for his great love
 *	and for the wonderful things he has done for them." (Psalm 107:8)
 *************************************************************************/
