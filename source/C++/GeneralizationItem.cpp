/*	Class:			GeneralizationItem
 *	Parent class:	Item
 *	Purpose:		To store info about generalizations of a word,
 *					which are the "parents" of that word,
 *					and is the opposite direction of its specifications
 *	Version:		Thinknowlogy 2018r1 (ShangDi 上帝)
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

#ifndef GENERALIZATIONITEM
#define GENERALIZATIONITEM 1
#include "WordItem.h"

class GeneralizationItem : private Item
	{
	friend class AdminReadFile;
	friend class AdminReasoningOld;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class ContextList;
	friend class GeneralizationList;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordSpecification;

	// Private initialized variables

	bool isLanguageWord_;
	bool isRelation_;

	unsigned short languageNr_;
	unsigned short specificationWordTypeNr_;
	unsigned short generalizationWordTypeNr_;

	WordItem *generalizationWordItem_;


	// Private functions

	bool isNoun()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR ||
				generalizationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}


	protected:
	// Constructor

	GeneralizationItem( bool isLanguageWord, bool isRelation, unsigned short languageNr, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "GeneralizationItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		isLanguageWord_ = isLanguageWord;
		isRelation_ = isRelation;

		languageNr_ = languageNr;

		specificationWordTypeNr_ = specificationWordTypeNr;
		generalizationWordTypeNr_ = generalizationWordTypeNr;

		// Checking private initialized variables

		if( ( generalizationWordItem_ = generalizationWordItem ) == NULL )
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given generalization word item is undefined" );
		}


	// Protected virtual functions

	virtual void displayWordReferences( bool isReturnQueryToPosition )
		{
		char *wordString;

		statusString[0] = statusChar();

		if( generalizationWordItem_ != NULL &&
		( wordString = generalizationWordItem_->wordTypeString( true, generalizationWordTypeNr_ ) ) != NULL )
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
		return ( generalizationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : generalizationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : generalizationWordItem_->itemNr() == queryItemNr ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( specificationWordTypeNr_ == queryWordTypeNr ||
				generalizationWordTypeNr_ == queryWordTypeNr );
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *generalizationWordTypeString = myWordItem()->wordTypeNameString( generalizationWordTypeNr_ );
		char *languageNameString = myWordItem()->languageNameString( languageNr_ );
		char *queryString;
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );
		char *wordString;

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( isLanguageWord_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isLanguageWord" );
			}

		if( isRelation_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isRelation" );
			}

		if( languageNr_ > NO_LANGUAGE_NR )
			{
			if( languageNameString == NULL )
				sprintf( tempString, "%clanguageNr:%u", QUERY_SEPARATOR_CHAR, languageNr_ );
			else
				sprintf( tempString, "%clanguage:%s", QUERY_SEPARATOR_CHAR, languageNameString );

			strcat( queryString, tempString );
			}

		if( specificationWordTypeString == NULL )
			sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
		else
			sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

		strcat( queryString, tempString );

		if( generalizationWordTypeString == NULL )
			sprintf( tempString, "%cgeneralizationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );
		else
			sprintf( tempString, "%cgeneralizationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, generalizationWordTypeString, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );

		strcat( queryString, tempString );

		if( generalizationWordItem_ != NULL )
			{
			sprintf( tempString, "%cgeneralizationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, generalizationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, generalizationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = generalizationWordItem_->wordTypeString( true, generalizationWordTypeNr_ ) ) != NULL )
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

		if( generalizationWordItem_ != NULL )
			return generalizationWordItem_->findMatchingWordReferenceString( queryString );

		return boolResult;
		}


	// Protected functions

	bool isRelation()
		{
		return isRelation_;
		}

	GeneralizationItem *generalizationItem( bool isIncludingThisItem, bool isOnlySelectingCurrentLanguage, bool isOnlySelectingNoun, bool isRelation )
		{
		unsigned short currentLanguageNr = globalVariables()->currentLanguageNr;
		GeneralizationItem *searchGeneralizationItem = ( isIncludingThisItem ? this : nextGeneralizationItem() );

		while( searchGeneralizationItem != NULL )
			{
			if( searchGeneralizationItem->isRelation_ == isRelation &&

			( !isOnlySelectingCurrentLanguage ||
			searchGeneralizationItem->languageNr_ == currentLanguageNr ) &&

			( !isOnlySelectingNoun ||
			searchGeneralizationItem->isNoun() ) )
				return searchGeneralizationItem;

			searchGeneralizationItem = searchGeneralizationItem->nextGeneralizationItem();
			}

		return NULL;
		}

	GeneralizationItem *nextGeneralizationItem()
		{
		return (GeneralizationItem *)nextItem;
		}

	GeneralizationItem *nextNounSpecificationGeneralizationItem()
		{
		return generalizationItem( false, false, true, false );
		}

	GeneralizationItem *nextSpecificationGeneralizationItem()
		{
		return generalizationItem( false, false, false, false );
		}

	GeneralizationItem *nextRelationGeneralizationItem()
		{
		return generalizationItem( false, false, false, true );
		}

	WordItem *generalizationWordItem()
		{
		return generalizationWordItem_;
		}
	};
#endif

/*************************************************************************
 *	"Give thanks to him who made the heavens so skillfully.
 *	His faithful love endures forever." (Psalm 136:5)
 *************************************************************************/
