/*	Class:			AdminCleanup
 *	Supports class:	AdminItem
 *	Purpose:		To cleanup obsolete items
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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

	// Private constructed variables

	bool hasAnyChangeBeenMadeByThisSentence_;
	bool wasUndoOrRedoCommand_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		List *currentAdminList;
		WordList *wordList;

		if( commonVariables_->currentSentenceNr == decrementSentenceNr &&
		commonVariables_->currentItemNr > startDecrementItemNr )
			commonVariables_->currentItemNr -= decrementOffset;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				currentAdminList->decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset );
			}
		}

	void decrementSentenceNrs( unsigned int startSentenceNr )
		{
		List *currentAdminList;
		WordList *wordList;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->decrementSentenceNrsInWordList( startSentenceNr );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				currentAdminList->decrementSentenceNrsInList( startSentenceNr );
			}
		}

	void rebuildQuickAccessListss()
		{
		WordItem *currentWordItem;

		commonVariables_->firstAssignmentWordItem = NULL;
		commonVariables_->firstContextWordItem = NULL;
		commonVariables_->firstCollectionWordItem = NULL;
		commonVariables_->firstSpecificationWordItem = NULL;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->rebuildQuickAccessLists();
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		}

	void removeFirstRangeOfDeletedItems()
		{
		unsigned short adminListNr = 0;
		List *currentAdminList;
		WordList *wordList;

		commonVariables_->nDeletedItems = 0;
		commonVariables_->removeSentenceNr = NO_SENTENCE_NR;
		commonVariables_->removeStartItemNr = NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->removeFirstRangeOfDeletedItemsInWordList();

		// Admin lists
		while( adminListNr < NUMBER_OF_ADMIN_LISTS &&
		commonVariables_->nDeletedItems == 0 )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				currentAdminList->removeFirstRangeOfDeletedItemsInList();

			adminListNr++;
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

	ResultType deleteAllWordTypesOfCurrentSentence( bool isActiveItems )
		{
		ReadItem *currentReadItem = ( isActiveItems ? adminItem_->firstActiveReadItem() : adminItem_->firstInactiveReadItem() );
		WordItem *currentWordItem;
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAllWordTypesOfCurrentSentence";

		if( ( wordList = adminItem_->wordList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The word list isn't created yet" );

		while( currentReadItem != NULL )
			{
			currentWordItem = currentReadItem->readWordItem();

			// Skip text
			if( currentWordItem != NULL &&
			!currentWordItem->isDeletedItem() )
				{
				if( currentWordItem->deleteAllWordTypesOfCurrentSentence() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all word types of the current sentence in word \"", currentWordItem->anyWordTypeString(), "\"" );

				if( !currentWordItem->hasAnyWordType() )
					{
					if( wordList->deleteItem( currentWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				}

			currentReadItem = currentReadItem->nextReadItem();
			}

		return RESULT_OK;
		}

	ResultType deleteUnusedWordsAndWordTypes( bool isActiveItems )
		{
		ReadResultType readResult;
		bool isSameWordOrderNr;
		unsigned short nReadWordReferences;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		WordTypeItem *currentWordTypeItem;
		ReadItem *currentReadItem = ( isActiveItems ? adminItem_->firstActiveReadItem() : adminItem_->firstInactiveReadItem() );
		ReadList *readList;
		WordItem *currentReadWordItem;
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedWordsAndWordTypes";

		if( ( readList = adminItem_->readList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The read list isn't created yet" );

		if( ( wordList = adminItem_->wordList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The word list isn't created yet" );

		while( currentReadItem != NULL )
			{
			isSameWordOrderNr = ( currentReadItem->wordOrderNr() == previousWordOrderNr );
			previousWordOrderNr = currentReadItem->wordOrderNr();

			// Skip text
			if( ( currentReadWordItem = currentReadItem->readWordItem() ) != NULL )
				{
				if( !isActiveItems ||

				// More word types for this word number
				( isSameWordOrderNr &&
				!currentReadItem->isSingularNoun() ) )
					{
					if( ( currentWordTypeItem = currentReadItem->activeReadWordTypeItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the word type of an active read word" );

					if( currentWordTypeItem->hasCurrentCreationSentenceNr() )
						{
						if( ( readResult = readList->getNumberOfReadWordReferences( currentReadItem->wordTypeNr(), currentReadWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the number of read word references" );

						if( ( nReadWordReferences = readResult.nReadWordReferences ) < 1 )
							return adminItem_->startError( functionNameString, moduleNameString_, "I have found an invalid number of read word references" );

						if( nReadWordReferences == 1 )
							{
							if( currentReadWordItem->deleteWordType( currentWordTypeItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an unused word type item" );

							if( !currentReadWordItem->hasAnyWordType() &&
							!currentReadWordItem->isDeletedItem() &&
							currentReadWordItem->hasCurrentCreationSentenceNr() )
								{
								if( wordList->deleteItem( currentReadWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an unused word item" );
								}
							}
						}
					}

				if( !isActiveItems ||
				isSameWordOrderNr )
					{
					if( readList->deleteItem( currentReadItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an active read item" );

					currentReadItem = readList->nextReadListItem();
					}
				else
					currentReadItem = currentReadItem->nextReadItem();
				}
			else
				currentReadItem = currentReadItem->nextReadItem();
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

	AdminCleanup( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasAnyChangeBeenMadeByThisSentence_ = false;
		wasUndoOrRedoCommand_ = false;

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

		hasAnyChangeBeenMadeByThisSentence_ = ( highestInUseSentenceNr >= currentSentenceNr );
		}

	void cleanupDeletedItems()
		{
		unsigned int firstSentenceNrOfCurrentUser;
		unsigned int startRemoveSentenceNr = NO_SENTENCE_NR;

		if( ( commonVariables_->hasDisplayedWarning ||
		commonVariables_->result != RESULT_OK ) &&

		!adminItem_->hasClosedFileDueToError() )
			deleteSentences( commonVariables_->currentSentenceNr );

		do	{
			removeFirstRangeOfDeletedItems();

			if( commonVariables_->nDeletedItems > 0 )
				{
				decrementItemNrRange( commonVariables_->removeSentenceNr, commonVariables_->removeStartItemNr, commonVariables_->nDeletedItems );
				startRemoveSentenceNr = commonVariables_->removeSentenceNr;
				}
			}
		while( commonVariables_->nDeletedItems > 0 );

		if( commonVariables_->hasDisplayedWarning )
			commonVariables_->hasDisplayedWarning = false;
		else
			{
			if( startRemoveSentenceNr > NO_SENTENCE_NR &&
			// Previous deleted sentence might be empty
			startRemoveSentenceNr != commonVariables_->removeSentenceNr &&
			// All items of this sentence are deleted
			getHighestInUseSentenceNr( true, true, startRemoveSentenceNr ) < startRemoveSentenceNr )
				{
				// So, decrement all higher sentence numbers
				decrementSentenceNrs( startRemoveSentenceNr );

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
			}
		}

	void clearTemporaryAdminLists()
		{
		wasUndoOrRedoCommand_ = false;

		adminItem_->deleteTemporaryReadList();
		adminItem_->deleteTemporaryScoreList();
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

	void deleteSentences( unsigned int lowestSentenceNr )
		{
		List *currentAdminList;
		WordList *wordList;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->deleteSentencesInWordList( lowestSentenceNr );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_->adminListArray[adminListNr] ) != NULL )
				currentAdminList->deleteSentencesInList( lowestSentenceNr );
			}

		rebuildQuickAccessListss();
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

	bool hasAnyChangeBeenMadeByThisSentence()
		{
		return hasAnyChangeBeenMadeByThisSentence_;
		}

	bool wasUndoOrRedoCommand()
		{
		return wasUndoOrRedoCommand_;
		}

	ResultType deleteAllWordTypesOfCurrentSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAllWordTypesOfCurrentSentence";

		if( deleteAllWordTypesOfCurrentSentence( true ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all word types of the active read list" );

		if( deleteAllWordTypesOfCurrentSentence( false ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete all word types of the inactive read list" );

		return RESULT_OK;
		}

	ResultType deleteUnusedWordsAndWordTypes()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedWordsAndWordTypes";

		// Active read items
		if( deleteUnusedWordsAndWordTypes( true ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete unused word types in the active read list" );

		// Inactive read items
		if( deleteUnusedWordsAndWordTypes( false ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete unused word types in the inactive read list" );

		return RESULT_OK;
		}

	ResultType redoLastUndoneSentence()
		{
		List *currentAdminList;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoLastUndoneSentence";

		wasUndoOrRedoCommand_ = true;

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

			if( ( currentWordItem = commonVariables_->firstWordItem ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The first word item is undefined" );

			// Do for all words
			do	{
				if( currentWordItem->redoCurrentSentence() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to redo the current sentence in word \"", currentWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

			if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, commonVariables_->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I have redone' interface notification" );

			rebuildQuickAccessListss();
			}
		else
			{
			// No sentences found to redo
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'no sentences to redo' interface notification" );

			decrementCurrentSentenceNr();
			}

		return RESULT_OK;
		}

	ResultType undoLastSentence()
		{
		unsigned int firstSentenceNrOfCurrentUser = adminItem_->firstSentenceNrOfCurrentUser();
		List *currentAdminList;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoLastSentence";

		wasUndoOrRedoCommand_ = true;

		// Remove the deleted read items of this undo sentence
		if( commonVariables_->currentSentenceNr > firstSentenceNrOfCurrentUser )
			{
			decrementCurrentSentenceNr();

			if( ( currentWordItem = commonVariables_->firstWordItem ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The first word item is undefined" );

			// Do for all words
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

			rebuildQuickAccessListss();
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
