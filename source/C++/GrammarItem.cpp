/*	Class:			GrammarItem
 *	Parent class:	Item
 *	Purpose:		To store info about the grammar of a language, which
 *					will be used for reading as well as writing sentences
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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

#ifndef GRAMMARITEM
#define GRAMMARITEM 1
#include "WordItem.h"

class GrammarItem : private Item
	{
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class GrammarList;
	friend class ReadItem;
	friend class ReadList;
	friend class WordWrite;
	friend class WriteItem;

	// Private initialized variables

	bool isDefinitionStart_ = false;
	bool isNewStart_ = false;
	bool isOptionStart_ = false;
	bool isChoiceStart_ = false;
	bool isSkipOptionForWriting_ = false;

	unsigned short grammarParameter_ = NO_GRAMMAR_PARAMETER;
	unsigned short grammarWordTypeNr_ = NO_WORD_TYPE_NR;

	char *grammarString_ = NULL;


	protected:
	// Protected constructed variables

	bool isOptionEnd;
	bool isChoiceEnd;
	bool isGrammarItemInUse;

	GrammarItem *nextDefinitionGrammarItem;


	// Protected initialized variables

	GrammarItem *definitionGrammarItem;


	// Constructor

	GrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short grammarWordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *_definitionGrammarItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "GrammarItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		isDefinitionStart_ = isDefinitionStart;
		isNewStart_ = isNewStart;
		isOptionStart_ = isOptionStart;
		isChoiceStart_ = isChoiceStart;
		isSkipOptionForWriting_ = isSkipOptionForWriting;

		grammarParameter_ = grammarParameter;
		grammarWordTypeNr_ = grammarWordTypeNr;

		// Checking private initialized variables

		if( grammarString != NULL )
			{
			if( grammarStringLength < MAX_SENTENCE_STRING_LENGTH &&
			strlen( grammarString ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( grammarString_ = new char[grammarStringLength + 1] ) != NULL )
					{
					strcpy( grammarString_, EMPTY_STRING );
					strncat( grammarString_, grammarString, grammarStringLength );
					}
				else
					startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the grammar string" );
				}
			else
				startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given grammar string is too long" );
			}
		else
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given grammar string is undefined" );


		// Protected constructed variables

		isOptionEnd = false;
		isChoiceEnd = false;
		isGrammarItemInUse = false;

		nextDefinitionGrammarItem = NULL;

		// Protected initialized variables

		definitionGrammarItem = _definitionGrammarItem;
		}

	~GrammarItem()
		{
		if( grammarString_ != NULL )
			delete grammarString_;
		}


	// Protected virtual functions

	virtual void checkForUsage()
		{
		myWordItem()->checkGrammarForUsageInWord( this );
		}

	virtual void displayString( bool isReturnQueryToPosition )
		{
		char *queryString;

		statusString[0] = statusChar();

		queryString = globalVariables()->queryString;

		if( grammarString_ != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( queryString, grammarString_ );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( grammarParameter_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&
				grammarParameter_ > NO_GRAMMAR_PARAMETER ) );
		}

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( definitionGrammarItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : definitionGrammarItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : definitionGrammarItem->itemNr() == queryItemNr ) ) ||

				( nextDefinitionGrammarItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : nextDefinitionGrammarItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : nextDefinitionGrammarItem->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( grammarWordTypeNr_ == queryWordTypeNr );
		}

	virtual char *itemString()
		{
		return grammarString_;
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;
		char *grammarWordTypeString = myWordItem()->wordTypeNameString( grammarWordTypeNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( isDefinitionStart_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isDefinitionStart" );
			}

		if( isNewStart_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isNewStart" );
			}

		if( isOptionStart_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isOptionStart" );
			}

		if( isOptionEnd )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isOptionEnd" );
			}

		if( isChoiceStart_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isChoiceStart" );
			}

		if( isChoiceEnd )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isChoiceEnd" );
			}

		if( isSkipOptionForWriting_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSkipOptionForWriting" );
			}
/*
		if( isGrammarItemInUse )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isGrammarItemInUse" );
			}
*/
		if( grammarParameter_ > NO_GRAMMAR_PARAMETER )
			{
			sprintf( tempString, "%cgrammarParameter:%u", QUERY_SEPARATOR_CHAR, grammarParameter_ );
			strcat( queryString, tempString );
			}

		if( grammarWordTypeNr_ > NO_WORD_TYPE_NR )
			{
			if( grammarWordTypeString == NULL )
				sprintf( tempString, "%cgrammarWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, grammarWordTypeNr_ );
			else
				sprintf( tempString, "%cgrammarWordType:%s", QUERY_SEPARATOR_CHAR, grammarWordTypeString );

			strcat( queryString, tempString );
			}

		if( grammarString_ != NULL )
			{
			sprintf( tempString, "%cgrammarString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, grammarString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		if( definitionGrammarItem != NULL )
			{
			sprintf( tempString, "%cdefinitionGrammarItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, definitionGrammarItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, definitionGrammarItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( nextDefinitionGrammarItem != NULL )
			{
			sprintf( tempString, "%cnextDefinitionGrammarItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, nextDefinitionGrammarItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, nextDefinitionGrammarItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
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

	bool isGrammarDefinition()
		{
		return ( grammarParameter_ >= GRAMMAR_SENTENCE );
		}

	bool isUserDefinedWord()
		{
		return ( grammarWordTypeNr_ > NO_WORD_TYPE_NR &&
				grammarParameter_ == NO_GRAMMAR_PARAMETER );
		}

	bool isGrammarStart()
		{
		return ( grammarParameter_ == GRAMMAR_SENTENCE );
		}

	bool isUndefinedWord()
		{
		// Last item in the list
		return ( nextItem == NULL );
		}

	bool isIdentical( GrammarItem *checkGrammarItem )
		{
		return ( checkGrammarItem != NULL &&

				checkGrammarItem->isNewStart() == isNewStart_ &&
				checkGrammarItem->isOptionStart() == isOptionStart_ &&
				checkGrammarItem->isOptionEnd == isOptionEnd &&
				checkGrammarItem->isChoiceStart() == isChoiceStart_ &&
				checkGrammarItem->isChoiceEnd == isChoiceEnd &&
				checkGrammarItem->grammarParameter() == grammarParameter_ &&
				checkGrammarItem->grammarWordTypeNr() == grammarWordTypeNr_ &&
				checkGrammarItem->itemNr() == itemNr() &&

				grammarString_ != NULL &&
				checkGrammarItem->grammarString() != NULL &&
				strcmp( grammarString_, checkGrammarItem->grammarString() ) == 0 );
		}

	bool isUsefulGrammarDefinition( bool isAssignment, bool isArchivedAssignment, bool isChineseCurrentLanguage, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization )
		{		// Statement
		return ( ( !isQuestion &&

				( isArchivedAssignment ||
				grammarParameter_ < GRAMMAR_STATEMENT_ASSIGNMENT ||
				grammarParameter_ > GRAMMAR_STATEMENT_SPECIFICATION_GENERALIZATION ||

				( isSpecificationGeneralization &&
				grammarParameter_ == GRAMMAR_STATEMENT_SPECIFICATION_GENERALIZATION ) ||

				( !isSpecificationGeneralization &&

				( ( !isAssignment &&
				grammarParameter_ == GRAMMAR_STATEMENT_SPECIFICATION ) ||

				( isAssignment &&

				( isPossessive ||
				isChineseCurrentLanguage ||
				grammarParameter_ == GRAMMAR_STATEMENT_ASSIGNMENT ) ) ) ) ) ) ||

				// Question
				( isQuestion &&

				( grammarParameter_ < GRAMMAR_QUESTION_SPECIFICATION ||
				grammarParameter_ > GRAMMAR_QUESTION_SPECIFICATION_GENERALIZATION ||

				( !isSpecificationGeneralization &&

				( isAssignment ||
				grammarParameter_ == GRAMMAR_QUESTION_SPECIFICATION ) ) ||

				( isSpecificationGeneralization &&
				grammarParameter_ == GRAMMAR_QUESTION_SPECIFICATION_GENERALIZATION ) ) ) );
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
		GrammarItem *nextEndingGrammarItem;

		return ( ( nextEndingGrammarItem = nextGrammarItem() ) != NULL &&
				nextEndingGrammarItem->grammarParameter() == grammarParameter_ ? nextEndingGrammarItem : NULL );
		}
	};
#endif

/*************************************************************************
 *	"For the Lord is good.
 *	His unfailing love continues forever,
 *	and his faithfulness continues to each generation." (Psalm 100:5)
 *************************************************************************/
