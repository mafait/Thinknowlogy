/*	Class:			AdminCleanup
 *	Supports class:	AdminItem
 *	Purpose:		To cleanup obsolete items
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

#include "AdminItem.h"
#include "ReadList.cpp"
#include "WordList.cpp"
#include "WordTypeItem.cpp"

class AdminCleanup
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundAnyChangeMadeByThisSentence_;
	bool wasCurrentCommandUndoOrRedo_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void deleteWriteListsInAllWords()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all active words
			do	currentWordItem->deleteTemporaryWriteList();
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		}

	unsigned int getHighestInUseSentenceNr( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		unsigned short adminListNr = 0;
		List *currentAdminList;
		WordList *wordList;

		commonVariables_->highestInUseSentenceNr = NO_SENTENCE_NR;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );

		// Admin lists
		while( adminListNr < NUMBER_OF_ADMIN_LISTS &&
		highestSentenceNr > commonVariables_->highestInUseSentenceNr )
			{
			currentAdminList = adminItem_->adminListArray[adminListNr];

			if( currentAdminList != NULL &&

			( isIncludingTemporaryLists ||
			!currentAdminList->isTemporaryList() ) )
				currentAdminList->getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			adminListNr++;
			}

		return commonVariables_->highestInUseSentenceNr;
		}

	ResultType decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		List *currentAdminList;
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

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				{
				if( currentAdminList->decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return RESULT_OK;
		}

	ResultType decrementSentenceNrs( unsigned int startSentenceNr )
		{
		List *currentAdminList;
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->decrementSentenceNrsInWordList( startSentenceNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				{
				if( currentAdminList->decrementSentenceNrsInList( startSentenceNr ) != RESULT_OK )
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

	ResultType removeFirstRangeOfDeletedItems()
		{
		unsigned short adminListNr = 0;
		List *currentAdminList;
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
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				{
				if( currentAdminList->removeFirstRangeOfDeletedItemsInList() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to remove the first deleted items of an admin list" );
				}

			adminListNr++;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminCleanup( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundAnyChangeMadeByThisSentence_ = false;
		wasCurrentCommandUndoOrRedo_ = false;

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
		unsigned int currentSentenceNr = commonVariables_->currentSentenceNr;
		unsigned int highestInUseSentenceNr = getHighestInUseSentenceNr( false, false, currentSentenceNr );

		hasFoundAnyChangeMadeByThisSentence_ = ( highestInUseSentenceNr >= currentSentenceNr );
		}

	void clearAllTemporaryLists()
		{
		wasCurrentCommandUndoOrRedo_ = false;

		// Read list is a temporary list
		adminItem_->deleteTemporaryReadList();
		// Score list is a temporary list
		adminItem_->deleteTemporaryScoreList();
		// Response lists are temporary lists
		deleteWriteListsInAllWords();
		}

	void decrementCurrentSentenceNr()
		{
		if( commonVariables_->currentSentenceNr > NO_SENTENCE_NR )
			{
			commonVariables_->currentSentenceNr--;
			// Necessary after changing current sentence number
			setCurrentItemNr();
			}
		}

	void setCurrentItemNr()
		{
		List *currentAdminList;
		WordList *wordList;

		commonVariables_->currentItemNr = NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->setCurrentItemNrInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				currentAdminList->setCurrentItemNrInList();
			}
		}

	bool hasFoundAnyChangeMadeByThisSentence()
		{
		return hasFoundAnyChangeMadeByThisSentence_;
		}

	bool wasCurrentCommandUndoOrRedo()
		{
		return wasCurrentCommandUndoOrRedo_;
		}

	ResultType cleanupDeletedItems()
		{
		unsigned int firstSentenceNrOfCurrentUser;
		unsigned int startRemoveSentenceNr = NO_SENTENCE_NR;
		char functionNameString[FUNCTION_NAME_LENGTH] = "cleanupDeletedItems";
/*
		if( !adminItem_->isSystemStartingUp() )
			commonVariables_->presentation->showStatus( INTERFACE_CONSOLE_I_AM_CLEANING_UP_DELETED_ITEMS );
*/
		if( ( commonVariables_->hasShownWarning ||
		commonVariables_->result != RESULT_OK ) &&

		!adminItem_->hasClosedFileDueToError() )
			{
			if( deleteSentences( commonVariables_->currentSentenceNr ) == RESULT_OK )
				commonVariables_->hasShownWarning = false;
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the current sentence" );
			}

		do	{
			if( removeFirstRangeOfDeletedItems() == RESULT_OK )
				{
				if( commonVariables_->nDeletedItems > 0 )
					{
					if( decrementItemNrRange( commonVariables_->removeSentenceNr, commonVariables_->removeStartItemNr, commonVariables_->nDeletedItems ) == RESULT_OK )
						startRemoveSentenceNr = commonVariables_->removeSentenceNr;
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to decrement item number range" );
					}
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove the first deleted items" );
			}
		while( commonVariables_->nDeletedItems > 0 );

		if( startRemoveSentenceNr > NO_SENTENCE_NR &&
		// Previous deleted sentence might be empty
		startRemoveSentenceNr != commonVariables_->removeSentenceNr &&
		// All items of this sentence are deleted
		getHighestInUseSentenceNr( true, true, startRemoveSentenceNr ) < startRemoveSentenceNr )
			{
			// So, decrement all higher sentence numbers
			if( decrementSentenceNrs( startRemoveSentenceNr ) == RESULT_OK )
				{
				if( commonVariables_->currentSentenceNr >= startRemoveSentenceNr )
					{
					firstSentenceNrOfCurrentUser = adminItem_->firstSentenceNrOfCurrentUser();

					// First user sentence
					if( startRemoveSentenceNr == firstSentenceNrOfCurrentUser )
						decrementCurrentSentenceNr();
					else
						{
						commonVariables_->currentSentenceNr = getHighestInUseSentenceNr( false, false, commonVariables_->currentSentenceNr );
						// Necessary after changing current sentence number
						setCurrentItemNr();
						}
					}
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number" );
			}

//		commonVariables_->presentation->clearStatus();

		return RESULT_OK;
		}

	ResultType deleteUnusedInterpretations( bool isDeletingAllActiveWordTypes )
		{
		ReferenceResultType referenceResult;
		unsigned short adminListNr;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		List *currentAdminList;
		ReadItem *unusedReadItem = adminItem_->firstActiveReadItem();
		ReadList *readList;
		WordItem *unusedWordItem;
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

							if( readList->deleteItem( unusedReadItem ) == RESULT_OK )
								{
								// Skip text
								if( unusedWordItem != NULL &&
								!unusedWordItem->hasItems() &&
								!unusedWordItem->isDeletedItem() &&
								unusedWordItem->hasCurrentCreationSentenceNr() )
									{
									adminListNr = 0;
									referenceResult.hasFoundWordReference = false;

									while( adminListNr < NUMBER_OF_ADMIN_LISTS &&
									!referenceResult.hasFoundWordReference )
										{
										if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
											{
											// Check the selection lists for a reference to this word
											if( ( referenceResult = currentAdminList->findWordReference( unusedWordItem ) ).result != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to find a reference to an active word \"", unusedWordItem->anyWordTypeString(), "\" in one of my lists" );
											}

										adminListNr++;
										}

									if( !referenceResult.hasFoundWordReference )
										{
										if( wordList->deleteItem( unusedWordItem ) != RESULT_OK )
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
						if( readList->deleteItem( unusedReadItem ) == RESULT_OK )
							{
							unusedWordItem = unusedReadItem->readWordItem();

							if( unusedWordItem != NULL &&
							!unusedWordItem->hasItems() &&
							!unusedWordItem->isDeletedItem() &&
							unusedWordItem->hasCurrentCreationSentenceNr() )
								{
								adminListNr = 0;
								referenceResult.hasFoundWordReference = false;

								while( adminListNr < NUMBER_OF_ADMIN_LISTS &&
								!referenceResult.hasFoundWordReference )
									{
									if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
										{
										// Check my lists for a reference to this word
										if( ( referenceResult = currentAdminList->findWordReference( unusedWordItem ) ).result != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to find a reference to an inactive word \"", unusedWordItem->anyWordTypeString(), "\" in one of my lists" );
										}

									adminListNr++;
									}

								if( !referenceResult.hasFoundWordReference )
									{
									if( wordList->deleteItem( unusedWordItem ) != RESULT_OK )
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

	ResultType deleteSentences( unsigned int lowestSentenceNr )
		{
		List *currentAdminList;
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->deleteSentencesInWordList( lowestSentenceNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete sentences in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				{
				if( currentAdminList->deleteSentencesInList( lowestSentenceNr ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to delete sentences in a list" );
				}
			}

		return RESULT_OK;
		}

	ResultType redoLastUndoneSentence()
		{
		List *currentAdminList;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoLastUndoneSentence";

		wasCurrentCommandUndoOrRedo_ = true;

		if( getHighestInUseSentenceNr( true, false, commonVariables_->currentSentenceNr ) == commonVariables_->currentSentenceNr )
			{
			// Important: Redo admin lists first, and the words after that.
			// Because redoing admin words list might redo words.
			for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
					{
					if( !currentAdminList->isTemporaryList() &&
					currentAdminList->redoCurrentSentenceInList() != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to redo the current sentence" );
					}
				}

			if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
				{
				// Do for all active words
				do	{
					if( currentWordItem->redoCurrentSentence() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to redo the current sentence in word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

				if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, commonVariables_->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I have redone' interface notification" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The first word item is undefined" );
			}
		else
			{
			// No sentences found to redo
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) == RESULT_OK )
				decrementCurrentSentenceNr();
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'no sentences to redo' interface notification" );
			}

		return RESULT_OK;
		}

	ResultType undoLastSentence()
		{
		unsigned int firstSentenceNrOfCurrentUser = adminItem_->firstSentenceNrOfCurrentUser();
		List *currentAdminList;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoLastSentence";

		wasCurrentCommandUndoOrRedo_ = true;

		// Remove the deleted read items of this undo sentence
		if( commonVariables_->currentSentenceNr > firstSentenceNrOfCurrentUser )
			{
			decrementCurrentSentenceNr();

			if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
				{
				// Do for all active words
				do	{
					if( currentWordItem->undoCurrentSentence() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to undo the current sentence in word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

				// Admin lists
				for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
					{
					if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
						{
						if( !currentAdminList->isTemporaryList() &&
						currentAdminList->undoCurrentSentenceInList() != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString_, "I failed to undo the current sentence" );
						}
					}

				if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, commonVariables_->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I have undone' interface notification" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The first word item is undefined" );
			}
		else
			{
			// No sentences found to undo
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_UNDO ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'no sentences to undo' interface notification" );
			}

		if( commonVariables_->currentSentenceNr >= firstSentenceNrOfCurrentUser )
			decrementCurrentSentenceNr();

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"How great is the Lord,
 *	how deserving of praise,
 *	in the city of our God,
 *	which sits on his holy mountain!" (Psalm 48:1)
 *************************************************************************/
