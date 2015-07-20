/*
 *	Class:			GrammarItem
 *	Parent class:	Item
 *	Purpose:		To store info about the grammar of a language, which
 *					will be used for reading as well as writing sentences
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

#ifndef GRAMMARITEM
#define GRAMMARITEM 1
#include "WordItem.h"

class GrammarItem : private Item
	{
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class GrammarList;
	friend class ReadItem;
	friend class ReadList;
	friend class WordWriteSentence;
	friend class WordWriteWords;
	friend class WriteItem;

	// Private loadable variables

	bool isDefinitionStart_;
	bool isNewStart_;
	bool isOptionStart_;
	bool isChoiceStart_;
	bool isSkipOptionForWriting_;

	unsigned short grammarParameter_;
	unsigned short grammarWordTypeNr_;

	char *grammarString_;


	protected:
	// Protected constructible variables

	bool isOptionEnd;
	bool isChoiceEnd;
	bool isGrammarItemInUse;

	GrammarItem *nextDefinitionGrammarItem;

	char *guideByGrammarString;


	// Protected loadable variables

	GrammarItem *definitionGrammarItem;


	// Constructor / deconstructor

	GrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short grammarWordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *_grammarDefinitionWordItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "GrammarItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		isDefinitionStart_ = isDefinitionStart;
		isNewStart_ = isNewStart;
		isOptionStart_ = isOptionStart;
		isChoiceStart_ = isChoiceStart;
		isSkipOptionForWriting_ = isSkipOptionForWriting;

		grammarParameter_ = grammarParameter;
		grammarWordTypeNr_ = grammarWordTypeNr;

		grammarString_ = NULL;

		if( grammarString != NULL )
			{
			if( grammarStringLength < MAX_SENTENCE_STRING_LENGTH &&
			strlen( grammarString ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( grammarString_ = new char[grammarStringLength + 1] ) != NULL )
					{
					strncpy( grammarString_, grammarString, grammarStringLength );
					grammarString_[grammarStringLength] = NULL_CHAR;
					}
				else
					startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the grammar string" );
				}
			else
				startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given grammar string is too long" );
			}
		else
			startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given grammar string is undefined" );


		// Protected constructible variables

		isOptionEnd = false;
		isChoiceEnd = false;
		isGrammarItemInUse = false;

		nextDefinitionGrammarItem = NULL;

		guideByGrammarString = NULL;

		// Protected loadable variables

		definitionGrammarItem = _grammarDefinitionWordItem;
		}

	~GrammarItem()
		{
		if( grammarString_ != NULL )
			delete grammarString_;
		if( guideByGrammarString != NULL )
			delete guideByGrammarString;
		}


	// Protected virtual functions

	virtual void showString( bool isReturnQueryToPosition )
		{
		char statusString[2] = SPACE_STRING;
		statusString[0] = statusChar();

		if( grammarString_ != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, grammarString_ );
			}

		if( guideByGrammarString != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, guideByGrammarString );
			}
		}

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( grammarParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				grammarParameter_ > NO_GRAMMAR_PARAMETER ) );
		}

	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( definitionGrammarItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : definitionGrammarItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : definitionGrammarItem->itemNr() == queryItemNr ) ) ||

				( nextDefinitionGrammarItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : nextDefinitionGrammarItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : nextDefinitionGrammarItem->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( grammarWordTypeNr_ == queryWordTypeNr );
		}

	virtual ResultType checkForUsage()
		{
		return myWordItem()->checkGrammarForUsageInWord( this );
		}

	virtual char *itemString()
		{
		return grammarString_;
		}

	virtual char *extraItemString()
		{
		return guideByGrammarString;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *grammarWordTypeString = myWordItem()->wordTypeNameString( grammarWordTypeNr_ );
		Item::toString( queryWordTypeNr );

		if( isDefinitionStart_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isDefinitionStart" );
			}

		if( isNewStart_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isNewStart" );
			}

		if( isOptionStart_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isOptionStart" );
			}

		if( isOptionEnd )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isOptionEnd" );
			}

		if( isChoiceStart_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isChoiceStart" );
			}

		if( isChoiceEnd )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isChoiceEnd" );
			}

		if( isSkipOptionForWriting_ )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isSkipOptionForWriting" );
			}
/*
		if( isGrammarItemInUse )
			{
			strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
			strcat( commonVariables()->queryString, "isGrammarItemInUse" );
			}
*/
		if( grammarParameter_ > NO_GRAMMAR_PARAMETER )
			{
			sprintf( tempString, "%cgrammarParameter:%u", QUERY_SEPARATOR_CHAR, grammarParameter_ );
			strcat( commonVariables()->queryString, tempString );
			}

		if( grammarWordTypeNr_ > WORD_TYPE_UNDEFINED )
			{
			if( grammarWordTypeString == NULL )
				sprintf( tempString, "%cgrammarWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, grammarWordTypeNr_ );
			else
				sprintf( tempString, "%cgrammarWordType:%s", QUERY_SEPARATOR_CHAR, grammarWordTypeString );

			strcat( commonVariables()->queryString, tempString );
			}

		if( grammarString_ != NULL )
			{
			sprintf( tempString, "%cgrammarString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, grammarString_, QUERY_STRING_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( guideByGrammarString != NULL )
			{
			sprintf( tempString, "%cguideByGrammarString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, guideByGrammarString, QUERY_STRING_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( definitionGrammarItem != NULL )
			{
			sprintf( tempString, "%cdefinitionGrammarItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, definitionGrammarItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, definitionGrammarItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( nextDefinitionGrammarItem != NULL )
			{
			sprintf( tempString, "%cnextDefinitionGrammarItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, nextDefinitionGrammarItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, nextDefinitionGrammarItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		return commonVariables()->queryString;
		}


	// Protected functions

	bool isDefinitionStart()
		{
		return isDefinitionStart_;
		}

	bool isNewStart()
		{
		return isNewStart_;
		}

	bool isOptionStart()
		{
		return isOptionStart_;
		}

	bool isChoiceStart()
		{
		return isChoiceStart_;
		}

	bool isSkipOptionForWriting()
		{
		return isSkipOptionForWriting_;
		}

	bool isWordEnding()
		{
		return ( grammarParameter_ == WORD_FEMININE_PROPER_NAME_ENDING ||
				grammarParameter_ == WORD_MASCULINE_PROPER_NAME_ENDING ||
				grammarParameter_ == WORD_FEMININE_SINGULAR_NOUN_ENDING ||
				grammarParameter_ == WORD_MASCULINE_SINGULAR_NOUN_ENDING ||
				grammarParameter_ == WORD_PLURAL_NOUN_ENDING );
		}

	bool isGrammarDefinition()
		{
		return ( grammarParameter_ >= GRAMMAR_SENTENCE );
		}

	bool isPredefinedWord()
		{
		return ( grammarWordTypeNr_ > WORD_TYPE_UNDEFINED &&
				grammarWordTypeNr_ < WORD_TYPE_TEXT &&

				grammarParameter_ > NO_GRAMMAR_PARAMETER );
		}

	bool isUserDefinedWord()
		{
		return ( grammarWordTypeNr_ > WORD_TYPE_UNDEFINED &&
				grammarParameter_ == NO_GRAMMAR_PARAMETER );
		}

	bool isGrammarStart()
		{
		return ( grammarParameter_ == GRAMMAR_SENTENCE );
		}

	bool hasWordType()
		{
		return ( grammarWordTypeNr_ > WORD_TYPE_UNDEFINED );
		}

	bool isSymbol()
		{
		return ( grammarWordTypeNr_ == WORD_TYPE_SYMBOL );
		}

	bool isNumeral()
		{
		return ( grammarWordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isSingularNoun()
		{
		return ( grammarWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}

	bool isPluralNoun()
		{
		return ( grammarWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isText()
		{
		return ( grammarWordTypeNr_ == WORD_TYPE_TEXT );
		}

	bool isUndefinedWord()
		{
		// Last item in the list
		return ( nextItem == NULL );
		}

	bool isIdentical( GrammarItem *checkGrammarItem )
		{
		return ( checkGrammarItem != NULL &&

				isNewStart_ == checkGrammarItem->isNewStart() &&
				isOptionStart_ == checkGrammarItem->isOptionStart() &&
				isOptionEnd == checkGrammarItem->isOptionEnd &&
				isChoiceStart_ == checkGrammarItem->isChoiceStart() &&
				isChoiceEnd == checkGrammarItem->isChoiceEnd &&
				grammarParameter_ == checkGrammarItem->grammarParameter() &&
				grammarWordTypeNr_ == checkGrammarItem->grammarWordTypeNr() &&
				itemNr() == checkGrammarItem->itemNr() &&

				grammarString_ != NULL &&
				checkGrammarItem->grammarString() != NULL &&
				strcmp( grammarString_, checkGrammarItem->grammarString() ) == 0 );
		}

	bool isUniqueGrammarDefinition( GrammarItem *checkGrammarItem )
		{
		char *checkGrammarString;
		GrammarItem *searchItem = this;

		if( checkGrammarItem != NULL &&
		( checkGrammarString = checkGrammarItem->grammarString() ) != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isDefinitionStart_ &&
				searchItem != checkGrammarItem &&
				strcmp( searchItem->grammarString_, checkGrammarString ) == 0 )
					return false;

				searchItem = searchItem->nextGrammarItem();
				}
			}

		return true;
		}

	unsigned short adjectiveParameter()
		{
		if( isAdjectiveParameter( grammarParameter_ ) )
			return grammarParameter_;

		return NO_ADJECTIVE_PARAMETER;
		}

	unsigned short definiteArticleParameter()
		{
		if( isDefiniteArticleParameter( grammarParameter_ ) )
			return grammarParameter_;

		return NO_DEFINITE_ARTICLE_PARAMETER;
		}

	unsigned short indefiniteArticleParameter()
		{
		if( isIndefiniteArticleParameter( grammarParameter_ ) )
			return grammarParameter_;

		return NO_INDEFINITE_ARTICLE_PARAMETER;
		}

	unsigned short grammarParameter()
		{
		return grammarParameter_;
		}

	unsigned short grammarWordTypeNr()
		{
		return grammarWordTypeNr_;
		}

	char *grammarString()
		{
		return grammarString_;
		}

	GrammarItem *nextGrammarItem()
		{
		return (GrammarItem *)nextItem;
		}

	GrammarItem *nextWordEndingGrammarItem()
		{
		GrammarItem *nextEndingGrammarItem = nextGrammarItem();

		return ( nextEndingGrammarItem == NULL || nextEndingGrammarItem->grammarParameter() != grammarParameter_ ? NULL : nextEndingGrammarItem );
		}
	};
#endif

/*************************************************************************
 *	"For the Lord is good.
 *	His unfailing love continues forever,
 *	and his faithfulness continues to each generation." (Psalm 100:5)
 *************************************************************************/
