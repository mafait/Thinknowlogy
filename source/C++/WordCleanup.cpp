/*
 *	Class:			WordCleanup
 *	Supports class:	WordItem
 *	Purpose:		To cleanup unnecessary items
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

#include "List.h"
#include "WordItem.h"

class WordCleanup
	{
	friend class WordItem;

	// Private constructible variables

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	protected:
	// Constructor / deconstructor

	WordCleanup( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordCleanup" );

		if( commonVariables_ != NULL )
			{
		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void deleteRollbackInfo()
		{
		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				myWordItem_->wordListArray[wordListNr]->deleteRollbackInfoInList();
			}
		}

	void getHighestInUseSentenceNr( bool isIncludingDeletedItems, bool isIncludingLanguageAssignments, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		bool isGrammarLanguage = myWordItem_->isLanguageWord();
		unsigned short wordListNr = 0;

		while( wordListNr < NUMBER_OF_WORD_LISTS &&
		commonVariables_->highestInUseSentenceNr < highestSentenceNr )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL &&

			( !isGrammarLanguage ||
			isIncludingLanguageAssignments ||
			wordListNr != WORD_ASSIGNMENT_LIST ) &&

			( isIncludingTemporaryLists ||
			!myWordItem_->wordListArray[wordListNr]->isTemporaryList() ) )
				myWordItem_->wordListArray[wordListNr]->getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			wordListNr++;
			}
		}

	void setCurrentItemNr()
		{
		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				myWordItem_->wordListArray[wordListNr]->setCurrentItemNrInList();
			}
		}

	ResultType decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRange";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				{
				if( myWordItem_->wordListArray[wordListNr]->decrementItemNrRangeInList( decrementSentenceNr, decrementItemNr, decrementOffset ) != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return RESULT_OK;
		}

	ResultType decrementSentenceNrs( unsigned int startSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				{
				if( myWordItem_->wordListArray[wordListNr]->decrementSentenceNrsInList( startSentenceNr ) != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return RESULT_OK;
		}

	ResultType deleteSentences( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				{
				if( myWordItem_->wordListArray[wordListNr]->deleteSentencesInList( isAvailableForRollback, lowestSentenceNr ) != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to delete sentences in one of my lists" );
				}
			}

		return RESULT_OK;
		}

	ResultType removeFirstRangeOfDeletedItems()
		{
		unsigned short wordListNr = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItems";

		while( wordListNr < NUMBER_OF_WORD_LISTS &&
		commonVariables_->nDeletedItems == 0 )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				{
				if( myWordItem_->wordListArray[wordListNr]->removeFirstRangeOfDeletedItemsInList() != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to remove the first deleted items" );
				}

			wordListNr++;
			}

		return RESULT_OK;
		}

	ResultType rollbackDeletedRedoInfo()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				{
				if( myWordItem_->wordListArray[wordListNr]->rollbackDeletedRedoInfoInList() != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to rollback the deleted redo info in one of my lists" );
				}
			}

		return RESULT_OK;
		}

	ResultType undoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoCurrentSentence";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				{
				if( myWordItem_->wordListArray[wordListNr]->undoCurrentSentenceInList() != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to undo the current sentence" );
				}
			}

		return RESULT_OK;
		}

	ResultType redoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentence";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL )
				{
				if( myWordItem_->wordListArray[wordListNr]->redoCurrentSentenceInList() != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to redo the current sentence" );
				}
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his convenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *
 *************************************************************************/
