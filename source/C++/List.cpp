/*	Class:		List
 *	Purpose:	Base class to store the items of the knowledge structure
 *	Version:	Thinknowlogy 2016r2 (Restyle)
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

#include "CollectionResultType.cpp"
#include "FileResultType.cpp"
#include "ListCleanup.cpp"
#include "ListQuery.cpp"
#include "ReadResultType.cpp"
#include "SpecificationResultType.cpp"

	// Private functions

	bool List::isAssignmentOrSpecificationList()
		{
		return ( listChar_ == WORD_ASSIGNMENT_LIST_SYMBOL ||
				listChar_ == WORD_SPECIFICATION_LIST_SYMBOL );
		}

	bool List::isIncludingThisList( char *queryListString )
		{
		unsigned short index = 0;

		if( queryListString != NULL &&
		strlen( queryListString ) > 0 )
			{
			while( index < strlen( queryListString ) &&
			queryListString[index] != listChar_ )
				index++;

			if( index == strlen( queryListString ) )
				return false;
			}

		return true;
		}

	ResultType List::removeItemFromList( Item *removeItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeItemFromList";

		if( removeItem == NULL )
			return startError( functionNameString, NULL, "The given remove item is undefined" );

		if( removeItem->myList() != this )
			return startError( functionNameString, NULL, "The given remove item doesn't belong to my list" );

		// First item in list
		if( removeItem->previousItem == NULL )
			{
			switch( removeItem->statusChar() )
				{
				case QUERY_ACTIVE_CHAR:
					activeList_ = removeItem->nextItem;
					break;

				case QUERY_INACTIVE_CHAR:
					inactiveList_ = removeItem->nextItem;
					break;

				case QUERY_ARCHIVED_CHAR:
					archivedList_ = removeItem->nextItem;
					break;

				case QUERY_REPLACED_CHAR:
					replacedList_ = removeItem->nextItem;
					break;

				case QUERY_DELETED_CHAR:
					deletedList_ = removeItem->nextItem;
					break;

				default:
					return startError( functionNameString, NULL, "The given remove item has an unknown status character" );
				}

			if( removeItem->nextItem != NULL )
				removeItem->nextItem->previousItem = NULL;
			}
		else
			{
			removeItem->previousItem->nextItem = removeItem->nextItem;

			if( removeItem->nextItem != NULL )
				removeItem->nextItem->previousItem = removeItem->previousItem;
			}

		// Remember next item
		nextListItem_ = removeItem->nextItem;

		// Disconnect item from the list
		removeItem->previousItem = NULL;
		removeItem->nextItem = NULL;

		return RESULT_OK;
		}


	// Constructor

	List::List()
		{
		// Private constructed variables

		listChar_ = QUERY_NO_LIST_CHAR;

		activeList_ = NULL;
		inactiveList_ = NULL;
		archivedList_ = NULL;
		replacedList_ = NULL;
		deletedList_ = NULL;

		nextListItem_ = NULL;

		listCleanup_ = NULL;
		listQuery_ = NULL;

		myWordItem_ = NULL;
		commonVariables_ = NULL;

		strcpy( classNameString_, EMPTY_STRING );
		strcpy( superClassNameString_, "List" );
		}

	List::~List()
		{
		if( listCleanup_ != NULL )
			delete listCleanup_;

		if( listQuery_ != NULL )
			delete listQuery_;
		}


	// Protected error functions

	CollectionResultType List::addCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CollectionResultType collectionResult;

		collectionResult.result = addError( functionNameString, moduleNameString, errorString );
		return collectionResult;
		}

	CollectionResultType List::startCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		CollectionResultType collectionResult;

		collectionResult.result = startError( functionNameString, moduleNameString, errorString );
		return collectionResult;
		}

	FileResultType List::addFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		FileResultType fileResult;

		fileResult.result = addError( functionNameString, moduleNameString, errorString );
		return fileResult;
		}

	FileResultType List::startFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		FileResultType fileResult;

		fileResult.result = startError( functionNameString, moduleNameString, errorString );
		return fileResult;
		}

	FileResultType List::startFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		FileResultType fileResult;

		fileResult.result = startError( functionNameString, moduleNameString, errorString1, errorString2, errorString3 );
		return fileResult;
		}

	FileResultType List::startFileResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		FileResultType fileResult;

		fileResult.result = startSystemError( functionNameString, moduleNameString, errorString );
		return fileResult;
		}

	GeneralizationResultType List::startGeneralizationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		GeneralizationResultType generalizationResult;

		generalizationResult.result = startError( functionNameString, moduleNameString, errorString );
		return generalizationResult;
		}

	GrammarResultType List::addGrammarResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		GrammarResultType grammarResult;

		grammarResult.result = addError( functionNameString, moduleNameString, errorString );
		return grammarResult;
		}

	GrammarResultType List::startGrammarResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		GrammarResultType grammarResult;

		grammarResult.result = startError( functionNameString, moduleNameString, errorString );
		return grammarResult;
		}

	JustificationResultType List::addJustificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		JustificationResultType justificationResult;

		justificationResult.result = addError( functionNameString, moduleNameString, errorString );
		return justificationResult;
		}

	JustificationResultType List::startJustificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		JustificationResultType justificationResult;

		justificationResult.result = startError( functionNameString, moduleNameString, errorString );
		return justificationResult;
		}

	ReadResultType List::addReadResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ReadResultType readResult;

		readResult.result = addError( functionNameString, moduleNameString, errorString );
		return readResult;
		}

	ReadResultType List::startReadResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		ReadResultType readResult;

		readResult.result = startError( functionNameString, moduleNameString, errorString );
		return readResult;
		}

	ResultType List::addError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		if( commonVariables_ != NULL &&
		commonVariables_->presentation != NULL )
			commonVariables_->presentation->displayError( listChar_, ( moduleNameString == NULL ? classNameString_ : moduleNameString ), ( moduleNameString == NULL ? superClassNameString_ : NULL ), ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), functionNameString, errorString );
		else
			fprintf( stderr, "\nClass:\t%s\nSubclass:\t%s\nFunction:\t%s\nError:\t\t%s.\n", classNameString_, superClassNameString_, functionNameString, errorString );

		return ( commonVariables_ == NULL ? RESULT_ERROR : commonVariables_->result );
		}

	ResultType List::startError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, errorString );

		if( commonVariables_ != NULL )
		commonVariables_->result = RESULT_ERROR;
		return RESULT_ERROR;
		}

	ResultType List::startError( const char *functionNameString, const char *moduleNameString, const char *_errorString, unsigned int errorSentenceNr )
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		sprintf( errorString, "%s%u", _errorString, errorSentenceNr );
		return startError( functionNameString, moduleNameString, errorString );
		}

	ResultType List::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		sprintf( errorString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, moduleNameString, errorString );
		}

	ResultType List::startSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, errorString );

		if( commonVariables_ != NULL )
		commonVariables_->result = RESULT_SYSTEM_ERROR;

		return RESULT_SYSTEM_ERROR;
		}

	SelectionResultType List::addSelectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		SelectionResultType selectionResult;

		selectionResult.result = addError( functionNameString, moduleNameString, errorString );
		return selectionResult;
		}

	SelectionResultType List::startSelectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		SelectionResultType selectionResult;

		selectionResult.result = startError( functionNameString, moduleNameString, errorString );
		return selectionResult;
		}

	SelectionResultType List::startSystemSelectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		SelectionResultType selectionResult;

		selectionResult.result = startSystemError( functionNameString, moduleNameString, errorString );
		return selectionResult;
		}

	SpecificationResultType List::addSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		SpecificationResultType specificationResult;

		specificationResult.result = addError( functionNameString, moduleNameString, errorString );
		return specificationResult;
		}

	SpecificationResultType List::startSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		SpecificationResultType specificationResult;

		specificationResult.result = startError( functionNameString, moduleNameString, errorString );
		return specificationResult;
		}

	StringResultType List::addStringResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		StringResultType stringResult;

		stringResult.result = addError( functionNameString, moduleNameString, errorString );
		return stringResult;
		}

	StringResultType List::startStringResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		StringResultType stringResult;

		stringResult.result = startError( functionNameString, moduleNameString, errorString );
		return stringResult;
		}

	WordResultType List::addWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		WordResultType wordResult;

		wordResult.result = addError( functionNameString, moduleNameString, errorString );
		return wordResult;
		}

	WordResultType List::startWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		WordResultType wordResult;

		wordResult.result = startError( functionNameString, moduleNameString, errorString );
		return wordResult;
		}


	// Protected virtual functions

	bool List::isTemporaryList()
		{
		return false;
		}


	// Protected common functions

	void List::initializeListVariables( char listChar, const char *classNameString, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		listChar_ = listChar;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( classNameString == NULL )
			strcpy( errorString, "The given my word is undefined" );
		else
			{
			if( classNameString != NULL )
				strcpy( classNameString_, classNameString );
			else
				strcpy( errorString, "The given class name string is undefined" );
			}

		// Always create the list cleanup module
		if( ( listCleanup_ = new ListCleanup( commonVariables_, this ) ) == NULL )
			strcpy( errorString, "I failed to create my list cleanup module" );

		if( strlen( errorString ) > 0 )
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, errorString );
		}

	void List::deleteTemporaryList()
		{
		Item *searchItem = deletedList_;

		if( replacedList_ != NULL )
			{
			// Move replaced list to deleted list
			if( searchItem == NULL )
				deletedList_ = replacedList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem->tailOfList();
				searchItem->nextItem = replacedList_;
				}

			replacedList_ = NULL;
			}

		if( archivedList_ != NULL )
			{
			// Move archived list to deleted list
			if( searchItem == NULL )
				deletedList_ = archivedList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem->tailOfList();
				searchItem->nextItem = archivedList_;
				}

			archivedList_ = NULL;
			}

		if( inactiveList_ != NULL )
			{
			// Move inactive list to deleted list
			if( searchItem == NULL )
				deletedList_ = inactiveList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem->tailOfList();
				searchItem->nextItem = inactiveList_;
				}

			inactiveList_ = NULL;
			}

		if( activeList_ != NULL )
			{
			// Move active list to deleted list
			if( searchItem == NULL )
				deletedList_ = activeList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem->tailOfList();
				searchItem->nextItem = activeList_;
				}

			activeList_ = NULL;
			}
		}

	bool List::hasItems()
		{
		return ( activeList_ != NULL ||
				inactiveList_ != NULL ||
				archivedList_ != NULL ||
				replacedList_ != NULL );
		}

	bool List::isAdminList()
		{
		return ( isupper( listChar_ ) != 0 );
		}

	bool List::isAssignmentList()
		{
		return ( listChar_ == WORD_ASSIGNMENT_LIST_SYMBOL );
		}

	char List::listChar()
		{
		return listChar_;
		}

	ResultType List::addItemToList( char statusChar, Item *newItem )
		{
		unsigned int newCreationSentenceNr;
		Item *searchItem;
		Item *previousSearchItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addItemToList";

		if( newItem == NULL )
			return startError( functionNameString, NULL, "The given new item is undefined" );

		if( newItem->myList() != this )
			return startError( functionNameString, NULL, "The given new item doesn't belong to my list" );

		if( newItem->nextItem != NULL )
			return startError( functionNameString, NULL, "The given new item seems to be a part of a list" );

		switch( statusChar )
			{
			case QUERY_ACTIVE_CHAR:
				newItem->setActiveSentenceNr();
				newItem->setActiveStatus();
				searchItem = activeList_;
				break;

			case QUERY_INACTIVE_CHAR:
				newItem->setInactiveSentenceNr();
				newItem->setInactiveStatus();
				searchItem = inactiveList_;
				break;

			case QUERY_ARCHIVED_CHAR:
				newItem->setArchivedSentenceNr();
				newItem->setArchivedStatus();
				searchItem = archivedList_;
				break;

			case QUERY_REPLACED_CHAR:
				newItem->setReplacedSentenceNr();
				newItem->setReplacedStatus();
				searchItem = replacedList_;
				break;

			case QUERY_DELETED_CHAR:
				newItem->setDeletedSentenceNr();
				newItem->setDeletedStatus();
				searchItem = deletedList_;
				break;

			default:
				return startError( functionNameString, NULL, "The given status character is unknown" );
			}

		// Sort item in list
		if( statusChar == QUERY_DELETED_CHAR )
			{
			newCreationSentenceNr = newItem->creationSentenceNr();

			while( searchItem != NULL &&

			// 1) Descending creationSentenceNr
			( searchItem->creationSentenceNr() > newCreationSentenceNr ||

			// 2) Ascending itemNr
			( searchItem->creationSentenceNr() == newCreationSentenceNr &&
			searchItem->itemNr() < newItem->itemNr() ) ) )
				{
				previousSearchItem = searchItem;
				searchItem = searchItem->nextItem;
				}
			}
		else
			{
			while( searchItem != NULL &&
			!newItem->isSorted( searchItem ) )
				{
				previousSearchItem = searchItem;
				searchItem = searchItem->nextItem;
				}
			}

		if( searchItem != NULL &&
		// Check on duplicates
		searchItem->creationSentenceNr() == newItem->creationSentenceNr() &&
		// for integrity
		searchItem->itemNr() == newItem->itemNr() )
			return startError( functionNameString, NULL, "I have found an active item with the same identification" );

		newItem->previousItem = previousSearchItem;

		// First item in list
		if( previousSearchItem == NULL )
			{
			switch( statusChar )
				{
				case QUERY_ACTIVE_CHAR:
					if( activeList_ != NULL )
						activeList_->previousItem = newItem;

					newItem->nextItem = activeList_;
					activeList_ = newItem;
					break;

				case QUERY_INACTIVE_CHAR:
					if( inactiveList_ != NULL )
						inactiveList_->previousItem = newItem;

					newItem->nextItem = inactiveList_;
					inactiveList_ = newItem;
					break;

				case QUERY_ARCHIVED_CHAR:
					if( archivedList_ != NULL )
						archivedList_->previousItem = newItem;

					newItem->nextItem = archivedList_;
					archivedList_ = newItem;
					break;

				case QUERY_REPLACED_CHAR:
					if( replacedList_ != NULL )
						replacedList_->previousItem = newItem;

					newItem->nextItem = replacedList_;
					replacedList_ = newItem;
					break;

				case QUERY_DELETED_CHAR:
					if( deletedList_ != NULL )
						deletedList_->previousItem = newItem;

					newItem->nextItem = deletedList_;
					deletedList_ = newItem;
					break;

				default:
					return startError( functionNameString, NULL, "The given status character is unknown" );
				}
			}
		else
			{
			if( searchItem != NULL )
				searchItem->previousItem = newItem;

			newItem->nextItem = previousSearchItem->nextItem;
			previousSearchItem->nextItem = newItem;
			}

		return RESULT_OK;
		}

	ResultType List::activateItem( Item *activateItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "activateItem";

		if( activateItem == NULL )
			return startError( functionNameString, NULL, "The given activate item is undefined" );

		if( activateItem->statusChar() == QUERY_ACTIVE_CHAR )
			return startError( functionNameString, NULL, "The given activate item is already an active item" );

		if( removeItemFromList( activateItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to remove an item from the archive list" );

		if( addItemToList( QUERY_ACTIVE_CHAR, activateItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to add an item to the active list" );

		if( isAssignmentList() &&
		commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
			commonVariables_->isAssignmentChanged = true;

		return RESULT_OK;
		}

	ResultType List::inactivateItem( Item *inactivateItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateItem";

		if( inactivateItem == NULL )
			return startError( functionNameString, NULL, "The given inactivate item is undefined" );

		if( inactivateItem->statusChar() == QUERY_INACTIVE_CHAR )
			return startError( functionNameString, NULL, "The given inactivate item is already an inactive item" );

		if( !isAssignmentList() &&
		listChar_ != ADMIN_READ_LIST_SYMBOL )
			return startError( functionNameString, NULL, "Only assignments, Guide by Grammar items and read items can be inactived" );

		if( removeItemFromList( inactivateItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to remove an item from the archive list" );

		if( addItemToList( QUERY_INACTIVE_CHAR, inactivateItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to add an item to the inactive list" );

		if( isAssignmentList() &&
		commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
			commonVariables_->isAssignmentChanged = true;

		return RESULT_OK;
		}

	ResultType List::archiveItem( Item *archiveItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "archiveItem";

		if( archiveItem == NULL )
			return startError( functionNameString, NULL, "The given archive item is undefined" );

		if( archiveItem->statusChar() == QUERY_ARCHIVED_CHAR )
			return startError( functionNameString, NULL, "The given archive item is already an archived item" );

		if( !isAssignmentList() )
			return startError( functionNameString, NULL, "Only assignments can be archived" );

		if( removeItemFromList( archiveItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to remove an item from a list" );

		archiveItem->previousStatusChar = archiveItem->statusChar();

		if( addItemToList( QUERY_ARCHIVED_CHAR, archiveItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to add an item to the archived list" );

		if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
			commonVariables_->isAssignmentChanged = true;

		return RESULT_OK;
		}

	ResultType List::replaceItem( Item *replaceItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceItem";

		if( replaceItem == NULL )
			return startError( functionNameString, NULL, "The given replace item is undefined" );

		if( replaceItem->statusChar() == QUERY_REPLACED_CHAR )
			return startError( functionNameString, NULL, "The given replace item is already a replaced item" );

		if( removeItemFromList( replaceItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to remove an item from a list" );

		replaceItem->previousStatusChar = replaceItem->statusChar();

		if( addItemToList( QUERY_REPLACED_CHAR, replaceItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to add an item to the replaced list" );

		return RESULT_OK;
		}

	ResultType List::deleteItem( Item *deleteItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteItem";

		if( removeItemFromList( deleteItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to remove an item from a list" );

		deleteItem->previousStatusChar = deleteItem->statusChar();

		if( deleteItem->statusChar() == QUERY_DELETED_CHAR )
			return startError( functionNameString, NULL, "The given delete item is already a deleted item" );

		if( addItemToList( QUERY_DELETED_CHAR, deleteItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to add an item to the deleted list" );

		return RESULT_OK;
		}

	ResultType List::deleteActiveItemsWithCurrentSentenceNr()
		{
		Item *searchItem = activeList_;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteActiveItemsWithCurrentSentenceNr";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( searchItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to delete an active item" );

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType List::removeFirstRangeOfDeletedItemsInList()
		{
		Item *removeItem = deletedList_;
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItemsInList";

		if( removeItem != NULL )
			{
			if( commonVariables_->nDeletedItems != 0 ||
			commonVariables_->removeSentenceNr != 0 ||
			commonVariables_->removeStartItemNr != 0 )
				return startError( functionNameString, NULL, "There is already a range of deleted items" );

			commonVariables_->removeSentenceNr = removeItem->creationSentenceNr();
			commonVariables_->removeStartItemNr = removeItem->itemNr();

			do	{
				// Disconnect deleted list from item
				deletedList_ = deletedList_->nextItem;

				if( removeItem->checkForUsage() != RESULT_OK )
					addError( functionNameString, NULL, "I failed to check an item for its usage" );

				// Disconnect item from the deleted list
				removeItem->nextItem = NULL;
				delete removeItem;
				removeItem = deletedList_;
				commonVariables_->nDeletedItems++;
				}
			while( removeItem != NULL &&
			// Same sentence number
			removeItem->creationSentenceNr() == commonVariables_->removeSentenceNr &&
			// Ascending item number
			removeItem->itemNr() == commonVariables_->removeStartItemNr + commonVariables_->nDeletedItems );
			}

		return RESULT_OK;
		}

	Item *List::firstActiveItem()
		{
		return activeList_;
		}

	Item *List::firstInactiveItem()
		{
		return inactiveList_;
		}

	Item *List::firstArchivedItem()
		{
		return archivedList_;
		}

	Item *List::firstReplacedItem()
		{
		return replacedList_;
		}

	Item *List::firstDeletedItem()
		{
		return deletedList_;
		}

	Item *List::nextListItem()
		{
		return nextListItem_;
		}

	CommonVariables *List::commonVariables()
		{
		return commonVariables_;
		}

	WordItem *List::myWordItem()
		{
		return myWordItem_;
		}


	// Protected cleanup functions

	void List::clearReplacingInfoInList()
		{
		listCleanup_->clearReplacingInfo();
		}

	void List::getHighestInUseSentenceNrInList( bool isIncludingDeletedItems, unsigned int highestSentenceNr )
		{
		unsigned int tempSentenceNr = listCleanup_->highestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

		if( tempSentenceNr > commonVariables_->highestInUseSentenceNr )
			commonVariables_->highestInUseSentenceNr = tempSentenceNr;
		}

	void List::setCurrentItemNrInList()
		{
		listCleanup_->setCurrentItemNr();
		}

	ResultType List::decrementItemNrRangeInList( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		return listCleanup_->decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset );
		}

	ResultType List::deleteSentencesInList( unsigned int lowestSentenceNr )
		{
		return listCleanup_->deleteSentences( lowestSentenceNr );
		}

	ResultType List::decrementSentenceNrsInList( unsigned int startSentenceNr )
		{
		return listCleanup_->decrementSentenceNrs( startSentenceNr );
		}

	ResultType List::undoCurrentSentenceInList()
		{
		return listCleanup_->undoCurrentSentence();
		}

	ResultType List::redoCurrentSentenceInList()
		{
		return listCleanup_->redoCurrentSentence();
		}


	// Protected query functions

	void List::countQueryInList()
		{
		if( listQuery_ != NULL )
			listQuery_->countQuery();
		}

	void List::clearQuerySelectionsInList()
		{
		if( listQuery_ != NULL )
			listQuery_->clearQuerySelections();
		}

	ResultType List::displayQueryResultInList( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		if( listQuery_ != NULL )
			return listQuery_->displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth );

		// In case the first query doesn't include admin lists,
		// the admin list query module isn't created yet
		return RESULT_OK;
		}

	ResultType List::itemQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, "I failed to create my list query module" );

		listQuery_->itemQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );
		return RESULT_OK;
		}

	ResultType List::listQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString )
		{
		bool isListIncludedInQuery = isIncludingThisList( queryListString );
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQueryInList";

		if( isSelectingOnFind ||
		!isListIncludedInQuery )
			{
			if( listQuery_ == NULL &&
			// Create supporting module
			( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
				return startError( functionNameString, NULL, "I failed to create my list query module" );

			listQuery_->listQuery( ( isSelectingOnFind && isListIncludedInQuery ), isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems );
			}

		return RESULT_OK;
		}

	ResultType List::wordTypeQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, "I failed to create my list query module" );

		listQuery_->wordTypeQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr );
		return RESULT_OK;
		}

	ResultType List::parameterQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, "I failed to create my list query module" );

		listQuery_->parameterQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter );
		return RESULT_OK;
		}

	ResultType List::wordQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, "I failed to create my list query module" );

		listQuery_->wordQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems );
		return RESULT_OK;
		}

	ResultType List::wordReferenceQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, "I failed to create my list query module" );

		return listQuery_->wordReferenceQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, ( isSelectingAttachedJustifications && isAssignmentOrSpecificationList() ), ( isSelectingJustificationSpecifications && isAssignmentOrSpecificationList() ), wordReferenceNameString );
		}

	ResultType List::stringQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, "I failed to create my list query module" );

		return listQuery_->stringQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString );
		}

	StringResultType List::compareStrings( char *searchString, char *sourceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareStrings";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startStringResultError( functionNameString, NULL, "I failed to create my list query module" );

		return listQuery_->compareStrings( searchString, sourceString );
		}

/*************************************************************************
 *	"But he rescues the poor from trouble
 *	and increases their families like flocks of sheep." (Psalm 107:41)
 *************************************************************************/
