/*
 *	Class:			AdminCleanup
 *	Supports class:	AdminItem
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

#include "AdminItem.h"
#include "ReadList.cpp"
#include "WordList.cpp"
#include "WordTypeItem.cpp"

class AdminCleanup
	{
	friend class AdminItem;

	// Private constructible variables

	bool isDontIncrementCurrentSentenceNr_;
	bool hasFoundAnyChangeDuringThisSentence_;
	bool wasUndoOrRedo_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void deleteWriteListsInAllWords()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->deleteTemporaryWriteList();
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		}

	void getHighestInUseSentenceNr( bool isIncludingDeletedItems, bool isIncludingLanguageAssignments, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		unsigned short adminListNr = 0;

		commonVariables_->highestInUseSentenceNr = NO_SENTENCE_NR;

		if( adminItem_->wordList != NULL )							// Inside words
			adminItem_->wordList->getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, highestSentenceNr );

		while( commonVariables_->highestInUseSentenceNr < highestSentenceNr &&
		adminListNr < NUMBER_OF_ADMIN_LISTS )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL &&	// Inside admin lists

			( isIncludingTemporaryLists ||
			!adminItem_->adminListArray[adminListNr]->isTemporaryList() ) )
				adminItem_->adminListArray[adminListNr]->getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			adminListNr++;
			}
		}

	ResultType decrementSentenceNrs( unsigned int startSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		if( adminItem_->wordList != NULL )							// Inside words
			{
			if( adminItem_->wordList->decrementSentenceNrsInWordList( startSentenceNr ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in my word list" );
			}

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
				{
				if( adminItem_->adminListArray[adminListNr]->decrementSentenceNrsInList( startSentenceNr ) != RESULT_OK )
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return RESULT_OK;
		}

	ResultType decrementCurrentSentenceNr()
		{
		unsigned int firstUserSentenceNr = adminItem_->firstUserSentenceNr();
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementCurrentSentenceNr";

		if( commonVariables_->currentSentenceNr > NO_SENTENCE_NR )
			{
			if( commonVariables_->currentSentenceNr - 1 >= firstUserSentenceNr )		// Is my sentence
				{
				getHighestInUseSentenceNr( false, true, false, --commonVariables_->currentSentenceNr );

				if( commonVariables_->highestInUseSentenceNr < commonVariables_->currentSentenceNr )
					commonVariables_->currentSentenceNr = ( commonVariables_->highestInUseSentenceNr > firstUserSentenceNr ? commonVariables_->highestInUseSentenceNr : firstUserSentenceNr );

				// Necessary after decrement of current sentence number
				setCurrentItemNr();
				}
			else
				isDontIncrementCurrentSentenceNr_ = true;
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The current sentence number is undefined" );

		return RESULT_OK;
		}

	ResultType decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRange";

		if( commonVariables_->currentSentenceNr == decrementSentenceNr &&
		commonVariables_->currentItemNr > startDecrementItemNr )
			commonVariables_->currentItemNr -= decrementOffset;

		if( adminItem_->wordList != NULL )							// Inside words
			{
			if( adminItem_->wordList->decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to decrement item number range in my word list" );
			}

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
				{
				if( adminItem_->adminListArray[adminListNr]->decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != RESULT_OK )
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return RESULT_OK;
		}

	ResultType deleteUnusedWordTypes( bool isInactiveItems, bool isDeleteAllActiveWordTypes )
		{
		ReadResultType readResult;
		unsigned short nReadWordReferences;
		char *pluralNounString;
		WordTypeItem *unusedWordTypeItem;
		WordTypeItem *singularNounWordTypeItem;
		ReadItem *previousReadItem = NULL;
		ReadItem *unusedReadItem = ( isInactiveItems ? adminItem_->firstInactiveReadItem() : adminItem_->firstActiveReadItem() );
		WordItem *unusedReadWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedWordTypes";

		if( adminItem_->readList != NULL )
			{
			while( unusedReadItem != NULL )
				{
				if( isInactiveItems ||
				isDeleteAllActiveWordTypes ||

				( previousReadItem != NULL &&
				!unusedReadItem->isSingularNoun() &&
				previousReadItem->wordOrderNr() == unusedReadItem->wordOrderNr() ) )		// More word types for this word number
					{
					if( ( unusedReadWordItem = unusedReadItem->readWordItem() ) != NULL )	// Skip text
						{
						if( ( unusedWordTypeItem = unusedReadItem->activeReadWordTypeItem() ) != NULL )
							{
							if( unusedWordTypeItem->hasCurrentCreationSentenceNr() )
								{
								if( !isDeleteAllActiveWordTypes &&
								unusedReadItem->isPluralNoun() &&	// Wrong assumption: This noun isn't plural but singular
								unusedReadWordItem->isUserDefinedWord() &&
								( pluralNounString = unusedWordTypeItem->itemString() ) != NULL &&
								( singularNounWordTypeItem = unusedReadWordItem->activeWordTypeItem( false, WORD_TYPE_NOUN_SINGULAR ) ) != NULL )
									{
									if( singularNounWordTypeItem->createNewWordTypeString( pluralNounString ) != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a new word string for a singular noun word type of an active read word" );
									}

								if( ( readResult = adminItem_->readList->getNumberOfReadWordReferences( unusedReadItem->wordTypeNr(), unusedReadWordItem ) ).result == RESULT_OK )
									{
									if( ( nReadWordReferences = readResult.nReadWordReferences ) >= 1 )
										{
										unusedReadItem->isUnusedReadItem = true;

										if( nReadWordReferences == 1 )
											{
											if( unusedReadWordItem->deleteWordType( unusedReadItem->wordTypeNr() ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an unused word type item" );
											}
										}
									else
										return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an invalid number of read word references" );
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get the number of read word references" );
								}
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the word type of an active read word" );
						}
					}

				previousReadItem = unusedReadItem;
				unusedReadItem = unusedReadItem->nextReadItem();
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The read list isn't created yet" );

		return RESULT_OK;
		}

	ResultType deleteUnusedWordTypes( bool isDeleteAllActiveWordTypes )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedWordTypes";
		if( deleteUnusedWordTypes( false, isDeleteAllActiveWordTypes ) == RESULT_OK )		// From active read items
			{
			if( deleteUnusedWordTypes( true, isDeleteAllActiveWordTypes ) != RESULT_OK )	// From inactive read items
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the active unused word types" );
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the inactive unused word types" );

		return RESULT_OK;
		}

	ResultType rollbackDeletedRedoInfo()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "rollbackDeletedRedoInfo";

		if( adminItem_->wordList != NULL )							// Inside words
			{
			if( adminItem_->wordList->rollbackDeletedRedoInfoInWordList() != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the current redo info of my words in my word list" );
			}

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
				adminItem_->adminListArray[adminListNr]->rollbackDeletedRedoInfoInList();
			}

		return RESULT_OK;
		}

	ResultType removeFirstRangeOfDeletedItems()
		{
		unsigned short adminListNr = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItems";

		commonVariables_->nDeletedItems = 0;
		commonVariables_->removeSentenceNr = NO_SENTENCE_NR;
		commonVariables_->removeStartItemNr = NO_ITEM_NR;

		if( adminItem_->wordList != NULL )							// Inside words
			{
			if( adminItem_->wordList->removeFirstRangeOfDeletedItemsInWordList() != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to remove the first deleted items in my word list" );
			}

		while( adminListNr < NUMBER_OF_ADMIN_LISTS &&
		commonVariables_->nDeletedItems == 0 )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
				{
				if( adminItem_->adminListArray[adminListNr]->removeFirstRangeOfDeletedItemsInList() != RESULT_OK )
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to remove the first deleted items" );
				}

			adminListNr++;
			}

		return RESULT_OK;
		}

	ResultType undoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoCurrentSentence";

		if( undoCurrentSentenceInAllWords() == RESULT_OK )			// Inside words
			{
			for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
					{
					if( adminItem_->adminListArray[adminListNr]->undoCurrentSentenceInList() != RESULT_OK )
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to undo the current sentence" );
					}
				}
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to undo the current sentence in my word list" );

		return RESULT_OK;
		}

	ResultType undoCurrentSentenceInAllWords()
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoCurrentSentenceInAllWords";

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->undoCurrentSentence();
			while( commonVariables_->result == RESULT_OK &&
			( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );

		return RESULT_OK;
		}

	ResultType redoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentence";

		if( redoCurrentSentenceInAllWords() == RESULT_OK )			// Inside words
			{
			for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
					{
					if( adminItem_->adminListArray[adminListNr]->redoCurrentSentenceInList() != RESULT_OK )
						return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to redo the current sentence" );
					}
				}
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to redo the current sentence in my word list" );

		return RESULT_OK;
		}

	ResultType redoCurrentSentenceInAllWords()
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentenceInAllWords";

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->redoCurrentSentence();
			while( commonVariables_->result == RESULT_OK &&
			( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminCleanup( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		isDontIncrementCurrentSentenceNr_ = false;
		hasFoundAnyChangeDuringThisSentence_ = false;
		wasUndoOrRedo_ = false;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminCleanup" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void checkForChangesDuringThisSentence()
		{
		hasFoundAnyChangeDuringThisSentence_ = true;

		getHighestInUseSentenceNr( false, false, false, commonVariables_->currentSentenceNr );

		if( commonVariables_->highestInUseSentenceNr < commonVariables_->currentSentenceNr )
			hasFoundAnyChangeDuringThisSentence_ = false;
		else
			clearDontIncrementCurrentSentenceNr();		// Found new knowledge
		}

	void clearDontIncrementCurrentSentenceNr()
		{
		isDontIncrementCurrentSentenceNr_ = false;
		}

	void deleteRollbackInfo()
		{
		if( adminItem_->wordList != NULL )							// Inside words
			adminItem_->wordList->deleteRollbackInfoInWordList();

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
				adminItem_->adminListArray[adminListNr]->deleteRollbackInfoInList();
			}
		}

	void setCurrentItemNr()
		{
		commonVariables_->currentItemNr = NO_ITEM_NR;

		if( adminItem_->wordList != NULL )							// Inside words
			adminItem_->wordList->setCurrentItemNrInWordList();

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
				adminItem_->adminListArray[adminListNr]->setCurrentItemNrInList();
			}
		}

	bool isDontIncrementCurrentSentenceNr()
		{
		return isDontIncrementCurrentSentenceNr_;
		}

	bool hasFoundAnyChangeDuringThisSentence()
		{
		return hasFoundAnyChangeDuringThisSentence_;
		}

	bool wasUndoOrRedo()
		{
		return wasUndoOrRedo_;
		}

	unsigned int highestSentenceNr()
		{
		unsigned int tempSentenceNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;

		if( adminItem_->wordList != NULL )							// Inside words
			highestSentenceNr = adminItem_->wordList->highestSentenceNrInWordList();

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL &&	// Inside admin lists
			( tempSentenceNr = adminItem_->adminListArray[adminListNr]->highestSentenceNrInList() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;
			}

		return highestSentenceNr;
		}

	ResultType cleanupDeletedItems()
		{
		unsigned int previousRemoveSentenceNr = NO_SENTENCE_NR;
		char functionNameString[FUNCTION_NAME_LENGTH] = "cleanupDeletedItems";
/*
		if( !adminItem_->isSystemStartingUp() )
			commonVariables_->presentation->showStatus( INTERFACE_CONSOLE_I_AM_CLEANING_UP_DELETED_ITEMS );
*/
		do	{
			if( removeFirstRangeOfDeletedItems() == RESULT_OK )
				{
				if( previousRemoveSentenceNr > NO_SENTENCE_NR &&
				previousRemoveSentenceNr != commonVariables_->removeSentenceNr )		// Previous deleted sentence is may be empty
					{
					getHighestInUseSentenceNr( true, true, true, previousRemoveSentenceNr );

					if( commonVariables_->highestInUseSentenceNr < previousRemoveSentenceNr )	// All items of this sentence are deleted
						{																// So, decrement all higher sentence numbers
						if( decrementSentenceNrs( previousRemoveSentenceNr ) == RESULT_OK )
							{
							if( commonVariables_->removeSentenceNr > previousRemoveSentenceNr )
								commonVariables_->removeSentenceNr--;

							if( commonVariables_->currentSentenceNr >= previousRemoveSentenceNr )
								{
								if( decrementCurrentSentenceNr() != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to decrement the current sentence number" );
								}
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number" );
						}
					}

				if( commonVariables_->nDeletedItems > 0 )
					{
					if( decrementItemNrRange( commonVariables_->removeSentenceNr, commonVariables_->removeStartItemNr, commonVariables_->nDeletedItems ) == RESULT_OK )
						previousRemoveSentenceNr = commonVariables_->removeSentenceNr;
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to decrement item number range" );
					}
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to remove the first deleted items" );
			}
		while( commonVariables_->nDeletedItems > 0 );

//		commonVariables_->presentation->clearStatus();

		return RESULT_OK;
		}

	ResultType deleteAllTemporaryLists()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAllTemporaryLists";

		wasUndoOrRedo_ = false;

		adminItem_->deleteTemporaryReadList();				// Read list is a temporary list
		adminItem_->deleteTemporaryScoreList();				// Score list is a temporary list

		deleteWriteListsInAllWords();					// Response lists are temporary lists

		if( cleanupDeletedItems() != RESULT_OK )
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to cleanup the deleted items" );

		return RESULT_OK;
		}

	ResultType deleteUnusedInterpretations( bool isDeleteAllActiveWordTypes )
		{
		ReferenceResultType referenceResult;
		unsigned short adminListNr;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		WordItem *unusedWordItem;
		ReadItem *unusedReadItem = adminItem_->firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedInterpretations";

		if( adminItem_->readList != NULL )
			{
			if( adminItem_->wordList != NULL )
				{
				if( deleteUnusedWordTypes( isDeleteAllActiveWordTypes ) == RESULT_OK )	// From active read items
					{
					while( unusedReadItem != NULL )
						{
						if( unusedReadItem->isUnusedReadItem ||
						unusedReadItem->wordOrderNr() == previousWordOrderNr )
							{
							previousWordOrderNr = unusedReadItem->wordOrderNr();
							unusedWordItem = unusedReadItem->readWordItem();

							if( adminItem_->readList->deleteItem( false, unusedReadItem ) == RESULT_OK )
								{
								if( unusedWordItem != NULL &&	// Skip text
								!unusedWordItem->hasItems() &&
								!unusedWordItem->isDeletedItem() &&
								unusedWordItem->hasCurrentCreationSentenceNr() )
									{
									adminListNr = 0;
									referenceResult.hasFoundWordReference = false;

									while( !referenceResult.hasFoundWordReference &&
									adminListNr < NUMBER_OF_ADMIN_LISTS )
										{
										if( adminItem_->adminListArray[adminListNr] != NULL )
											{
											// Check the selection lists for a reference to this word
											if( ( referenceResult = adminItem_->adminListArray[adminListNr]->findWordReference( unusedWordItem ) ).result != RESULT_OK )
												return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to find a reference to an active word \"", unusedWordItem->anyWordTypeString(), "\" in one of my lists" );
											}

										adminListNr++;
										}

									if( !referenceResult.hasFoundWordReference )
										{
										if( adminItem_->wordList->deleteItem( false, unusedWordItem ) != RESULT_OK )
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete a word item" );
										}
									}

								unusedReadItem = adminItem_->readList->nextReadListItem();
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an active read item" );
							}
						else
							{
							previousWordOrderNr = unusedReadItem->wordOrderNr();
							unusedReadItem = unusedReadItem->nextReadItem();
							}
						}

					while( ( unusedReadItem = adminItem_->firstInactiveReadItem() ) != NULL )
						{
						if( adminItem_->readList->deleteItem( false, unusedReadItem ) == RESULT_OK )
							{
							unusedWordItem = unusedReadItem->readWordItem();

							if( unusedWordItem != NULL &&
							!unusedWordItem->hasItems() &&
							!unusedWordItem->isDeletedItem() &&
							unusedWordItem->hasCurrentCreationSentenceNr() )
								{
								adminListNr = 0;
								referenceResult.hasFoundWordReference = false;

								while( !referenceResult.hasFoundWordReference &&
								adminListNr < NUMBER_OF_ADMIN_LISTS )
									{
									if( adminItem_->adminListArray[adminListNr] != NULL )
										{
										// Check my lists for a reference to this word
										if( ( referenceResult = adminItem_->adminListArray[adminListNr]->findWordReference( unusedWordItem ) ).result != RESULT_OK )
											return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to find a reference to an inactive word \"", unusedWordItem->anyWordTypeString(), "\" in one of my lists" );
										}

									adminListNr++;
									}

								if( !referenceResult.hasFoundWordReference )
									{
									if( adminItem_->wordList->deleteItem( false, unusedWordItem ) != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete a word item" );
									}
								}
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete an unused (inactive) read word" );
						}
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the unused word types" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The word list isn't created yet" );
			}

		return RESULT_OK;
		}

	ResultType deleteSentences( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		if( adminItem_->wordList != NULL )							// Inside words
			{
			if( adminItem_->wordList->deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete sentences in my word list" );
			}

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )	// Inside admin lists
				{
				if( adminItem_->adminListArray[adminListNr]->deleteSentencesInList( isAvailableForRollback, lowestSentenceNr ) != RESULT_OK )
					return myWordItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to delete sentences in a list" );
				}
			}

		if( cleanupDeletedItems() != RESULT_OK )
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to cleanup the deleted items" );

		return RESULT_OK;
		}

	ResultType undoLastSentence()
		{
		bool skipUndo = false;
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoLastSentence";

		if( commonVariables_->currentSentenceNr > NO_SENTENCE_NR )
			{
			if( deleteAllTemporaryLists() == RESULT_OK )			// Deleted read words of the undo sentence
				{
				if( commonVariables_->currentSentenceNr > highestSentenceNr() )
					{
					if( commonVariables_->currentSentenceNr - 1 >= adminItem_->firstUserSentenceNr() )		// Is my sentence
						{
						commonVariables_->currentSentenceNr--;
						setCurrentItemNr();		// Necessary after decrement of current sentence number
						}
					else
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_UNDO_SENTENCE_OF_ANOTHER_USER ) == RESULT_OK )
							{
							skipUndo = true;
							isDontIncrementCurrentSentenceNr_ = true;
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification about another user" );
						}
					}

				if( !skipUndo )
					{
					// Roll-back Undo sentence. Handle it as a command, not as a sentence
					rollbackDeletedRedoInfo();

					if( undoCurrentSentence() == RESULT_OK )
						{
						if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, commonVariables_->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) == RESULT_OK )
							{
							wasUndoOrRedo_ = true;
							isDontIncrementCurrentSentenceNr_ = true;
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification about having undone a sentence" );
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to undo the current sentence" );
					}
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete all temporary lists" );
			}
		else
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_LEFT_TO_UNDO ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification about no sentences left to undo" );
			}

		return RESULT_OK;
		}

	ResultType redoLastUndoneSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoLastUndoneSentence";

		if( deleteAllTemporaryLists() == RESULT_OK )	// Deleted read words of the undo sentence
			{
			if( !isDontIncrementCurrentSentenceNr_ &&
			commonVariables_->currentSentenceNr > highestSentenceNr() )
				{
				rollbackDeletedRedoInfo();

				if( redoCurrentSentence() == RESULT_OK )
					{
					if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, commonVariables_->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) == RESULT_OK )
						wasUndoOrRedo_ = true;
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification about having redone a sentence" );
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to redo the current sentence" );
				}
			else
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification about no sentences to redo" );
				}
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete all temporary lists" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"How great is the Lord,
 *	how deserving of praise,
 *	in the city of our God,
 *	which sits on his holy mountain!" (Psalm 48:1)
 *
 *************************************************************************/
