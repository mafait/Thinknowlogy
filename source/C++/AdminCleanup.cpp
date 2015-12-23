/*
 *	Class:			AdminCleanup
 *	Supports class:	AdminItem
 *	Purpose:		To cleanup obsolete items
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

#include "AdminItem.h"
#include "ReadList.cpp"
#include "WordList.cpp"
#include "WordTypeItem.cpp"

class AdminCleanup
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundAnyChangeMadeByThisSentence_;
	bool wasUndoOrRedo_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void decrementCurrentSentenceNr()
		{
		unsigned int firstSentenceNrOfCurrentUser = adminItem_->firstSentenceNrOfCurrentUser();

		if( commonVariables_->currentSentenceNr >= firstSentenceNrOfCurrentUser )
			{
			getHighestInUseSentenceNr( false, false, --commonVariables_->currentSentenceNr );

			if( commonVariables_->highestInUseSentenceNr < commonVariables_->currentSentenceNr )
				commonVariables_->currentSentenceNr = ( commonVariables_->highestInUseSentenceNr > firstSentenceNrOfCurrentUser ? commonVariables_->highestInUseSentenceNr : firstSentenceNrOfCurrentUser );

			// Necessary after changing current sentence number
			setCurrentItemNr();

			if( commonVariables_->currentItemNr == NO_ITEM_NR )
				{
				if( commonVariables_->currentSentenceNr > NO_SENTENCE_NR )
					{
					commonVariables_->currentSentenceNr--;
					// Necessary after changing current sentence number
					setCurrentItemNr();
					}
				}
			}
		else
			commonVariables_->isDontIncrementCurrentSentenceNr = true;
		}

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

	void getHighestInUseSentenceNr( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		unsigned short adminListNr = 0;
		WordList *wordList;

		commonVariables_->highestInUseSentenceNr = NO_SENTENCE_NR;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );

		// Admin lists
		while( commonVariables_->highestInUseSentenceNr < highestSentenceNr &&
		adminListNr < NUMBER_OF_ADMIN_LISTS )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL &&

			( isIncludingTemporaryLists ||
			!adminItem_->adminListArray[adminListNr]->isTemporaryList() ) )
				adminItem_->adminListArray[adminListNr]->getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			adminListNr++;
			}
		}

	unsigned int highestSentenceNr()
		{
		unsigned int tempSentenceNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;
		WordList *wordList;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			highestSentenceNr = wordList->highestSentenceNrInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL &&
			( tempSentenceNr = adminItem_->adminListArray[adminListNr]->highestSentenceNrInList() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;
			}

		return highestSentenceNr;
		}

	ResultType decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRange";

		if( commonVariables_->currentSentenceNr == decrementSentenceNr &&
		commonVariables_->currentItemNr > startDecrementItemNr )
			commonVariables_->currentItemNr -= decrementOffset;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to decrement item number range in my word list" );
			}

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			// Admin lists
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return RESULT_OK;
		}

	ResultType decrementSentenceNrs( unsigned int startSentenceNr )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->decrementSentenceNrsInWordList( startSentenceNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in my word list" );
			}

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			// Admin lists
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->decrementSentenceNrsInList( startSentenceNr ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return RESULT_OK;
		}

	ResultType deleteUnusedWordTypes( bool isInactiveItems, bool isDeletingAllActiveWordTypes )
		{
		ReadResultType readResult;
		unsigned short nReadWordReferences;
		char *pluralNounString;
		WordTypeItem *unusedWordTypeItem;
		WordTypeItem *singularNounWordTypeItem;
		ReadItem *previousReadItem = NULL;
		ReadItem *unusedReadItem = ( isInactiveItems ? adminItem_->firstInactiveReadItem() : adminItem_->firstActiveReadItem() );
		WordItem *unusedReadWordItem;
		ReadList *readList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedWordTypes";

		if( ( readList = adminItem_->readList ) != NULL )
			{
			while( unusedReadItem != NULL )
				{
				if( isInactiveItems ||
				isDeletingAllActiveWordTypes ||

				( previousReadItem != NULL &&
				!unusedReadItem->isSingularNoun() &&
				// More word types for this word number
				previousReadItem->wordOrderNr() == unusedReadItem->wordOrderNr() ) )
					{
					// Skip text
					if( ( unusedReadWordItem = unusedReadItem->readWordItem() ) != NULL )
						{
						if( ( unusedWordTypeItem = unusedReadItem->activeReadWordTypeItem() ) != NULL )
							{
							if( unusedWordTypeItem->hasCurrentCreationSentenceNr() )
								{
								if( !isDeletingAllActiveWordTypes &&
								// Wrong assumption: This noun isn't plural but singular
								unusedReadItem->isPluralNoun() &&
								unusedReadWordItem->isUserDefinedWord() &&
								( pluralNounString = unusedWordTypeItem->itemString() ) != NULL &&
								( singularNounWordTypeItem = unusedReadWordItem->activeWordTypeItem( WORD_TYPE_NOUN_SINGULAR ) ) != NULL )
									{
									if( singularNounWordTypeItem->createNewWordTypeString( pluralNounString ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a new word string for a singular noun word type of an active read word" );
									}

								if( ( readResult = readList->getNumberOfReadWordReferences( unusedReadItem->wordTypeNr(), unusedReadWordItem ) ).result == RESULT_OK )
									{
									if( ( nReadWordReferences = readResult.nReadWordReferences ) >= 1 )
										{
										unusedReadItem->isUnusedReadItem = true;

										if( nReadWordReferences == 1 )
											{
											if( unusedReadWordItem->deleteWordType( unusedReadItem->wordTypeNr() ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an unused word type item" );
											}
										}
									else
										return adminItem_->startError( functionNameString, moduleNameString_, "I have found an invalid number of read word references" );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the number of read word references" );
								}
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the word type of an active read word" );
						}
					}

				previousReadItem = unusedReadItem;
				unusedReadItem = unusedReadItem->nextReadItem();
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The read list isn't created yet" );

		return RESULT_OK;
		}

	ResultType deleteUnusedWordTypes( bool isDeletingAllActiveWordTypes )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedWordTypes";

		// From active read items
		if( deleteUnusedWordTypes( false, isDeletingAllActiveWordTypes ) == RESULT_OK )
			{
			// From inactive read items
			if( deleteUnusedWordTypes( true, isDeletingAllActiveWordTypes ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the active unused word types" );
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the inactive unused word types" );

		return RESULT_OK;
		}

	ResultType redoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentence";

		// In words
		if( redoCurrentSentenceInAllWords() == RESULT_OK )
			{
			// Admin lists
			for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( adminItem_->adminListArray[adminListNr] != NULL )
					{
					if( adminItem_->adminListArray[adminListNr]->redoCurrentSentenceInList() != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to redo the current sentence" );
					}
				}
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to redo the current sentence in my word list" );

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
			return adminItem_->startError( functionNameString, moduleNameString_, "The first word item is undefined" );

		return RESULT_OK;
		}

	ResultType removeFirstRangeOfDeletedItems()
		{
		unsigned short adminListNr = 0;
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItems";

		commonVariables_->nDeletedItems = 0;
		commonVariables_->removeSentenceNr = NO_SENTENCE_NR;
		commonVariables_->removeStartItemNr = NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->removeFirstRangeOfDeletedItemsInWordList() != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove the first deleted items in my word list" );
			}

		// Admin lists
		while( adminListNr < NUMBER_OF_ADMIN_LISTS &&
		commonVariables_->nDeletedItems == 0 )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->removeFirstRangeOfDeletedItemsInList() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to remove the first deleted items" );
				}

			adminListNr++;
			}

		return RESULT_OK;
		}

	ResultType rollbackDeletedRedoInfo()
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "rollbackDeletedRedoInfo";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->rollbackDeletedRedoInfoInWordList() != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the current redo info of my words in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				adminItem_->adminListArray[adminListNr]->rollbackDeletedRedoInfoInList();
			}

		return RESULT_OK;
		}

	ResultType undoCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoCurrentSentence";

		// In words
		if( undoCurrentSentenceInAllWords() == RESULT_OK )
			{
			// Admin lists
			for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( adminItem_->adminListArray[adminListNr] != NULL )
					{
					if( adminItem_->adminListArray[adminListNr]->undoCurrentSentenceInList() != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to undo the current sentence" );
					}
				}
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to undo the current sentence in my word list" );

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
			return adminItem_->startError( functionNameString, moduleNameString_, "The first word item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminCleanup( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundAnyChangeMadeByThisSentence_ = false;
		wasUndoOrRedo_ = false;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminCleanup" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void checkForChangesMadeByThisSentence()
		{
		hasFoundAnyChangeMadeByThisSentence_ = true;

		getHighestInUseSentenceNr( false, false, commonVariables_->currentSentenceNr );

		if( commonVariables_->highestInUseSentenceNr < commonVariables_->currentSentenceNr )
			hasFoundAnyChangeMadeByThisSentence_ = false;
		}

	void deleteRollbackInfo()
		{
		WordList *wordList;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->deleteRollbackInfoInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				adminItem_->adminListArray[adminListNr]->deleteRollbackInfoInList();
			}
		}

	void setCurrentItemNr()
		{
		WordList *wordList;

		commonVariables_->currentItemNr = NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->setCurrentItemNrInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				adminItem_->adminListArray[adminListNr]->setCurrentItemNrInList();
			}
		}

	bool hasFoundAnyChangeMadeByThisSentence()
		{
		return hasFoundAnyChangeMadeByThisSentence_;
		}

	bool wasUndoOrRedo()
		{
		return wasUndoOrRedo_;
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
				// Previous deleted sentence is may be empty
				previousRemoveSentenceNr != commonVariables_->removeSentenceNr )
					{
					getHighestInUseSentenceNr( true, true, previousRemoveSentenceNr );

					// All items of this sentence are deleted
					if( commonVariables_->highestInUseSentenceNr < previousRemoveSentenceNr )
						{
						// So, decrement all higher sentence numbers
						if( decrementSentenceNrs( previousRemoveSentenceNr ) == RESULT_OK )
							{
							if( commonVariables_->removeSentenceNr > previousRemoveSentenceNr )
								commonVariables_->removeSentenceNr--;

							if( commonVariables_->currentSentenceNr >= previousRemoveSentenceNr )
								decrementCurrentSentenceNr();
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number" );
						}
					}

				if( commonVariables_->nDeletedItems > 0 )
					{
					if( decrementItemNrRange( commonVariables_->removeSentenceNr, commonVariables_->removeStartItemNr, commonVariables_->nDeletedItems ) == RESULT_OK )
						previousRemoveSentenceNr = commonVariables_->removeSentenceNr;
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to decrement item number range" );
					}
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove the first deleted items" );
			}
		while( commonVariables_->nDeletedItems > 0 );

//		commonVariables_->presentation->clearStatus();

		return RESULT_OK;
		}

	ResultType deleteAllTemporaryLists()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAllTemporaryLists";

		wasUndoOrRedo_ = false;

		// Read list is a temporary list
		adminItem_->deleteTemporaryReadList();
		// Score list is a temporary list
		adminItem_->deleteTemporaryScoreList();
		// Response lists are temporary lists
		deleteWriteListsInAllWords();

		if( cleanupDeletedItems() != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to cleanup the deleted items" );

		return RESULT_OK;
		}

	ResultType deleteUnusedInterpretations( bool isDeletingAllActiveWordTypes )
		{
		ReferenceResultType referenceResult;
		unsigned short adminListNr;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		WordItem *unusedWordItem;
		ReadItem *unusedReadItem = adminItem_->firstActiveReadItem();
		ReadList *readList;
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedInterpretations";

		if( ( readList = adminItem_->readList ) != NULL )
			{
			if( ( wordList = adminItem_->wordList ) != NULL )
				{
				if( deleteUnusedWordTypes( isDeletingAllActiveWordTypes ) == RESULT_OK )
					{
					// Active read items
					while( unusedReadItem != NULL )
						{
						if( unusedReadItem->isUnusedReadItem ||
						unusedReadItem->wordOrderNr() == previousWordOrderNr )
							{
							previousWordOrderNr = unusedReadItem->wordOrderNr();
							unusedWordItem = unusedReadItem->readWordItem();

							if( readList->deleteItem( false, unusedReadItem ) == RESULT_OK )
								{
								// Skip text
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
											// Check the selection lists for a reference to this word
											if( ( referenceResult = adminItem_->adminListArray[adminListNr]->findWordReference( unusedWordItem ) ).result != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to find a reference to an active word \"", unusedWordItem->anyWordTypeString(), "\" in one of my lists" );
											}

										adminListNr++;
										}

									if( !referenceResult.hasFoundWordReference )
										{
										if( wordList->deleteItem( false, unusedWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete a word item" );
										}
									}

								unusedReadItem = readList->nextReadListItem();
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an active read item" );
							}
						else
							{
							previousWordOrderNr = unusedReadItem->wordOrderNr();
							unusedReadItem = unusedReadItem->nextReadItem();
							}
						}

					// Inactive read items
					while( ( unusedReadItem = adminItem_->firstInactiveReadItem() ) != NULL )
						{
						if( readList->deleteItem( false, unusedReadItem ) == RESULT_OK )
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
											return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to find a reference to an inactive word \"", unusedWordItem->anyWordTypeString(), "\" in one of my lists" );
										}

									adminListNr++;
									}

								if( !referenceResult.hasFoundWordReference )
									{
									if( wordList->deleteItem( false, unusedWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete a word item" );
									}
								}
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an unused (inactive) read word" );
						}
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the unused word types" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The word list isn't created yet" );
			}

		return RESULT_OK;
		}

	ResultType deleteSentences( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete sentences in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->deleteSentencesInList( isAvailableForRollback, lowestSentenceNr ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to delete sentences in a list" );
				}
			}

		if( cleanupDeletedItems() != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to cleanup the deleted items" );

		return RESULT_OK;
		}

	ResultType undoLastSentence()
		{
		unsigned int previousSentenceNr = commonVariables_->currentSentenceNr;
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoLastSentence";

		// Remove the deleted read items of this undo sentence
		if( deleteAllTemporaryLists() == RESULT_OK )
			{
			// No sentences found to undo
			if( commonVariables_->currentSentenceNr <= adminItem_->firstSentenceNrOfCurrentUser() )
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_UNDO ) == RESULT_OK )
					{
					if( previousSentenceNr == commonVariables_->currentSentenceNr )
						commonVariables_->isDontIncrementCurrentSentenceNr = true;
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'no sentences to undo' interface notification" );
				}
			else
				{
				if( previousSentenceNr > NO_SENTENCE_NR &&
				previousSentenceNr == commonVariables_->currentSentenceNr )
					{
					commonVariables_->currentSentenceNr--;
					// Necessary after changing current sentence number
					setCurrentItemNr();
					}

				if( undoCurrentSentence() == RESULT_OK )
					{
					if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, commonVariables_->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) == RESULT_OK )
						{
						wasUndoOrRedo_ = true;
						commonVariables_->isDontIncrementCurrentSentenceNr = true;
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I have undone' interface notification" );
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to undo the current sentence" );
				}
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all temporary lists" );

		return RESULT_OK;
		}

	ResultType redoLastUndoneSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoLastUndoneSentence";

		// Remove the deleted read items of this redo sentence
		if( deleteAllTemporaryLists() == RESULT_OK )
			{
			if( commonVariables_->currentSentenceNr > highestSentenceNr() )
				{
				// Roll-back deleted sentence. Process this sentence as a command, not as a sentence
				rollbackDeletedRedoInfo();

				if( redoCurrentSentence() == RESULT_OK )
					{
					if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, commonVariables_->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) == RESULT_OK )
						wasUndoOrRedo_ = true;
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I have redone' interface notification" );
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to redo the current sentence" );
				}
			else
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'no sentences to redo' interface notification" );
				}
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all temporary lists" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"How great is the Lord,
 *	how deserving of praise,
 *	in the city of our God,
 *	which sits on his holy mountain!" (Psalm 48:1)
 *************************************************************************/
