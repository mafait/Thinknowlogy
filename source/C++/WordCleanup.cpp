/*
 *	Class:			WordCleanup
 *	Supports class:	WordItem
 *	Purpose:		To cleanup obsolete items
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

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

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

	void getHighestInUseSentenceNr( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, bool isLanguageWord, unsigned int highestSentenceNr )
		{
		unsigned short wordListNr = 0;

		while( wordListNr < NUMBER_OF_WORD_LISTS &&
		commonVariables_->highestInUseSentenceNr < highestSentenceNr )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST ) ) &&

			// Skip temporary lists
			( isIncludingTemporaryLists ||
			!myWordItem_->wordListArray[wordListNr]->isTemporaryList() ) )
				myWordItem_->wordListArray[wordListNr]->getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			wordListNr++;
			}
		}

	void setCurrentItemNr( bool isLanguageWord )
		{
		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST ) ) )
				myWordItem_->wordListArray[wordListNr]->setCurrentItemNrInList();
			}
		}

	ResultType decrementItemNrRange( bool isLanguageWord, unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRange";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST ) ) )
				{
				if( myWordItem_->wordListArray[wordListNr]->decrementItemNrRangeInList( decrementSentenceNr, decrementItemNr, decrementOffset ) != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return RESULT_OK;
		}

	ResultType decrementSentenceNrs( bool isLanguageWord, unsigned int startSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST ) ) )
				{
				if( myWordItem_->wordListArray[wordListNr]->decrementSentenceNrsInList( startSentenceNr ) != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return RESULT_OK;
		}

	ResultType deleteSentences( bool isAvailableForRollback, bool isLanguageWord, unsigned int lowestSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		for( unsigned short wordListNr = 0; wordListNr < NUMBER_OF_WORD_LISTS; wordListNr++ )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST ) ) )
				{
				if( myWordItem_->wordListArray[wordListNr]->deleteSentencesInList( isAvailableForRollback, lowestSentenceNr ) != RESULT_OK )
					return myWordItem_->addErrorInWord( myWordItem_->wordListChar( wordListNr ), functionNameString, moduleNameString_, "I failed to delete sentences in one of my lists" );
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

	ResultType removeFirstRangeOfDeletedItems( bool isLanguageWord )
		{
		unsigned short wordListNr = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItems";

		while( wordListNr < NUMBER_OF_WORD_LISTS &&
		commonVariables_->nDeletedItems == 0 )
			{
			if( myWordItem_->wordListArray[wordListNr] != NULL &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != WORD_GRAMMAR_LIST &&
			wordListNr != WORD_INTERFACE_LIST ) ) )
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "rollbackDeletedRedoInfo";

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
	};

/*************************************************************************
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his convenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *************************************************************************/
