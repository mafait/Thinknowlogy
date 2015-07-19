/*
 *	Class:		List
 *	Purpose:	Base class to store the items of the knowledge structure
 *	Version:	Thinknowlogy 2015r1beta (Corazón)
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

#include "ListCleanup.cpp"
#include "ListQuery.cpp"

	// Private functions

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

	Item *List::tailOfList( Item *searchItem )
		{
		Item *nextItem;

		while( searchItem != NULL &&
		( nextItem = searchItem->nextItem ) != NULL )
			searchItem = nextItem;

		return searchItem;
		}


	// Constructor / deconstructor

	List::List()
		{
		// Private constructible variables

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

	ResultType List::addError( const char *functionNameString, const char *moduleNameString, const char *wordNameString, const char *errorString )
		{
		if( commonVariables_ != NULL &&
		commonVariables_->presentation != NULL )
			commonVariables_->presentation->showError( listChar_, ( moduleNameString == NULL ? classNameString_ : moduleNameString ), ( moduleNameString == NULL ? superClassNameString_ : NULL ), wordNameString, functionNameString, errorString );
		else
			fprintf( stderr, "\nClass:\t%s\nSubclass:\t%s\nFunction:\t%s\nError:\t\t%s.\n", classNameString_, superClassNameString_, functionNameString, errorString );

		return ( commonVariables_ == NULL ? RESULT_ERROR : commonVariables_->result );
		}

	ResultType List::startError( const char *functionNameString, const char *moduleNameString, const char *wordNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, wordNameString, errorString );

		if( commonVariables_ != NULL )
		commonVariables_->result = RESULT_ERROR;

		return RESULT_ERROR;
		}

	ResultType List::startError( const char *functionNameString, const char *moduleNameString, const char *wordNameString, const char *_errorString, unsigned int errorSentenceNr )
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		sprintf( errorString, "%s%u", _errorString, errorSentenceNr );
		return startError( functionNameString, moduleNameString, wordNameString, errorString );
		}

	ResultType List::startError( const char *functionNameString, const char *moduleNameString, const char *wordNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		sprintf( errorString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, moduleNameString, wordNameString, errorString );
		}

	ResultType List::startSystemError( const char *functionNameString, const char *moduleNameString, const char *wordNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, wordNameString, errorString );

		if( commonVariables_ != NULL )
		commonVariables_->result = RESULT_SYSTEM_ERROR;

		return RESULT_SYSTEM_ERROR;
		}


	// Protected virtual functions

	bool List::isTemporaryList()
		{
		return false;
		}

	ReferenceResultType List::findWordReference( WordItem *referenceWordItem )
		{
		// This is a virtual function. Therefore the given variables are unreferenced
		ReferenceResultType referenceResult;
		return referenceResult;
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
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, ( myWordItem_ == NULL ? NULL : myWordItem_->anyWordTypeString() ), errorString );
		}

	void List::deleteTemporaryList()
		{
		Item *searchItem = deletedList_;

		if( replacedList_ != NULL )
			{
			// Move replaced list to deleted list
			if( deletedList_ == NULL )
				deletedList_ = replacedList_;
			else
				{
				searchItem = tailOfList( searchItem );
				searchItem->nextItem = replacedList_;
				}

			replacedList_ = NULL;
			}

		if( archivedList_ != NULL )
			{
			// Move archived list to deleted list
			if( deletedList_ == NULL )
				deletedList_ = archivedList_;
			else
				{
				searchItem = tailOfList( searchItem );
				searchItem->nextItem = archivedList_;
				}

			archivedList_ = NULL;
			}

		if( inactiveList_ != NULL )
			{
			// Move inactive list to deleted list
			if( deletedList_ == NULL )
				deletedList_ = inactiveList_;
			else
				{
				searchItem = tailOfList( searchItem );
				searchItem->nextItem = inactiveList_;
				}

			inactiveList_ = NULL;
			}

		if( activeList_ != NULL )
			{
			// Move active list to deleted list
			if( deletedList_ == NULL )
				deletedList_ = activeList_;
			else
				{
				searchItem = tailOfList( searchItem );
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

	bool List::isReadList()
		{
		return ( listChar_ == ADMIN_READ_LIST_SYMBOL );
		}

	unsigned int List::highestSentenceNrInList()
		{
		unsigned int highestSentenceNr = NO_SENTENCE_NR;
		Item *searchItem = activeList_;

		while( searchItem != NULL )
			{
			if( searchItem->creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem->creationSentenceNr();

			searchItem = searchItem->nextItem;
			}

		searchItem = inactiveList_;

		while( searchItem != NULL )
			{
			if( searchItem->creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem->creationSentenceNr();

			searchItem = searchItem->nextItem;
			}

		searchItem = archivedList_;

		while( searchItem != NULL )
			{
			if( searchItem->creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem->creationSentenceNr();

			searchItem = searchItem->nextItem;
			}

		searchItem = replacedList_;

		while( searchItem != NULL )
			{
			if( searchItem->creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem->creationSentenceNr();

			searchItem = searchItem->nextItem;
			}

		return highestSentenceNr;
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

		if( newItem != NULL )
			{
			if( newItem->myList() == this )
				{
				if( newItem->nextItem == NULL )
					{
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
							return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given status character is unknown" );
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

					if( searchItem == NULL ||
					// Check on duplicates
					searchItem->creationSentenceNr() != newItem->creationSentenceNr() ||
					// for integrity
					searchItem->itemNr() != newItem->itemNr() )
						{
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
									return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given status character is unknown" );
								}
							}
						else
							{
							if( searchItem != NULL )
								searchItem->previousItem = newItem;

							newItem->nextItem = previousSearchItem->nextItem;
							previousSearchItem->nextItem = newItem;
							}
						}
					else
						return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I've found an active item with the same identification" );
					}
				else
					return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given new item seems to be a part of a list" );
				}
			else
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given new item doesn't belong to my list" );
			}
		else
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given new item is undefined" );

		return RESULT_OK;
		}

	ResultType List::activateItem( Item *activateItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "activateItem";

		if( activateItem != NULL )
			{
			if( activateItem->statusChar() != QUERY_ACTIVE_CHAR )
				{
				if( activateItem->creationSentenceNr() == activateItem->activeSentenceNr() )
					{
					if( removeItemFromList( activateItem ) == RESULT_OK )
						{
						if( addItemToList( QUERY_ACTIVE_CHAR, activateItem ) == RESULT_OK )
							{
							if( isAssignmentList() &&
							commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
								commonVariables_->isAssignmentChanged = true;
							}
						else
							return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to add an item to the active list" );
						}
					else
						return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to remove an item from the archive list" );
					}
				else
					return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The active sentence number of the given archived item is already assigned" );
				}
			else
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given activate item is already an active item" );
			}
		else
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given activate item is undefined" );

		return RESULT_OK;
		}

	ResultType List::inactivateItem( Item *inactivateItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "inactivateItem";

		if( inactivateItem != NULL )
			{
			if( inactivateItem->statusChar() != QUERY_INACTIVE_CHAR )
				{
				if( isAssignmentList() ||
				isReadList() )
					{
					if( !inactivateItem->hasInactiveSentenceNr() ||
					inactivateItem->creationSentenceNr() == inactivateItem->activeSentenceNr() )
						{
						if( removeItemFromList( inactivateItem ) == RESULT_OK )
							{
							if( addItemToList( QUERY_INACTIVE_CHAR, inactivateItem ) == RESULT_OK )
								{
								if( isAssignmentList() &&
								commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
									commonVariables_->isAssignmentChanged = true;
								}
							else
								return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to add an item to the inactive list" );
							}
						else
							return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to remove an item from the archive list" );
						}
					else
						return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The inactive sentence number of the given archived item is already assigned" );
					}
				else
					return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "Only assignments, Guide by Grammar items and read items can be inactived" );
				}
			else
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given inactivate item is already an inactive item" );
			}
		else
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given inactivate item is undefined" );

		return RESULT_OK;
		}

	ResultType List::archiveItem( Item *archiveItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "archiveItem";

		if( archiveItem != NULL )
			{
			if( archiveItem->statusChar() != QUERY_ARCHIVED_CHAR )
				{
				if( isAssignmentList() )
					{
					if( removeItemFromList( archiveItem ) == RESULT_OK )
						{
						archiveItem->previousStatusChar = archiveItem->statusChar();

						if( addItemToList( QUERY_ARCHIVED_CHAR, archiveItem ) == RESULT_OK )
							{
							if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
								commonVariables_->isAssignmentChanged = true;
							}
						else
							return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to add an item to the archived list" );
						}
					else
						return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to remove an item from a list" );
					}
				else
					return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "Only assignments can be archived" );
				}
			else
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given archive item is already an archived item" );
			}
		else
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given archive item is undefined" );

		return RESULT_OK;
		}

	ResultType List::replaceItem( Item *replaceItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceItem";

		if( replaceItem != NULL )
			{
			if( replaceItem->statusChar() != QUERY_REPLACED_CHAR )
				{
				if( removeItemFromList( replaceItem ) == RESULT_OK )
					{
					replaceItem->previousStatusChar = replaceItem->statusChar();

					if( addItemToList( QUERY_REPLACED_CHAR, replaceItem ) != RESULT_OK )
						return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to add an item to the replaced list" );
					}
				else
					return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to remove an item from a list" );
				}
			else
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given replace item is already a replaced item" );
			}
		else
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given replace item is undefined" );

		return RESULT_OK;
		}

	ResultType List::deleteItem( bool isAvailableForRollback, Item *deleteItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteItem";

		if( removeItemFromList( deleteItem ) == RESULT_OK )
			{
			if( deleteItem->statusChar() != QUERY_DELETED_CHAR )
				{
				if( addItemToList( QUERY_DELETED_CHAR, deleteItem ) == RESULT_OK )
					deleteItem->isAvailableForRollbackAfterDelete = isAvailableForRollback;
				else
					return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to add an item to the deleted list" );
				}
			else
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given delete item is already a deleted item" );
			}
		else
			return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to remove an item from a list" );

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
				if( deleteItem( false, searchItem ) == RESULT_OK )
					searchItem = nextListItem_;
				else
					return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to delete an active item" );
				}
			else
				searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType List::removeFirstRangeOfDeletedItemsInList()
		{
		Item *removeItem = deletedList_;
		Item *previousRemoveItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItemsInList";

		if( commonVariables_->nDeletedItems == 0 &&
		commonVariables_->removeSentenceNr == 0 &&
		commonVariables_->removeStartItemNr == 0 )
			{
			// Skip items that must be kept for rollback
			while( removeItem != NULL &&
			// and items of current sentence (if wanted)
			removeItem->isAvailableForRollbackAfterDelete )
				{
				previousRemoveItem = removeItem;
				removeItem = removeItem->nextItem;
				}

			// Found items to remove
			if( removeItem != NULL )
				{
				commonVariables_->removeSentenceNr = removeItem->creationSentenceNr();
				commonVariables_->removeStartItemNr = removeItem->itemNr();

				do	{
					if( previousRemoveItem == NULL )
						// Disconnect deleted list from item
						deletedList_ = deletedList_->nextItem;
					else
						// Disconnect deleted list from item
						previousRemoveItem->nextItem = removeItem->nextItem;

					if( removeItem->checkForUsage() == RESULT_OK )
						{
						// Disconnect item from the deleted list
						removeItem->nextItem = NULL;
						delete removeItem;
						removeItem = ( previousRemoveItem == NULL ? deletedList_ : previousRemoveItem->nextItem );
						commonVariables_->nDeletedItems++;
						}
					else
						return addError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to check an item for its usage" );
					}
				while( removeItem != NULL &&
				// Same sentence number
				removeItem->creationSentenceNr() == commonVariables_->removeSentenceNr &&
				// Ascending item number
				removeItem->itemNr() == commonVariables_->removeStartItemNr + commonVariables_->nDeletedItems );
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "There is already a range of deleted items" );

		return RESULT_OK;
		}

	ResultType List::removeItemFromList( Item *removeItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeItemFromList";

		if( removeItem != NULL )
			{
			if( removeItem->myList() == this )
				{
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
							return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given remove item has an unknown status character" );
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
				}
			else
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given remove item doesn't belong to my list" );
			}
		else
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "The given remove item is undefined" );

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

	WordItem *List::myWordItem()
		{
		return myWordItem_;
		}

	CommonVariables *List::commonVariables()
		{
		return commonVariables_;
		}


	// Protected cleanup functions

	void List::deleteRollbackInfoInList()
		{
		listCleanup_->deleteRollbackInfo();
		}

	void List::getHighestInUseSentenceNrInList( bool isIncludingDeletedItems, unsigned int highestSentenceNr )
		{
		unsigned int tempSentenceNr;

		if( ( tempSentenceNr = listCleanup_->highestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr ) ) > commonVariables_->highestInUseSentenceNr )
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

	ResultType List::deleteSentencesInList( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		return listCleanup_->deleteSentences( isAvailableForRollback, lowestSentenceNr );
		}

	ResultType List::decrementSentenceNrsInList( unsigned int startSentenceNr )
		{
		return listCleanup_->decrementSentenceNrs( startSentenceNr );
		}

	ResultType List::rollbackDeletedRedoInfoInList()
		{
		return listCleanup_->rollbackDeletedRedoInfo();
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

	ReferenceResultType List::compareStrings( char *searchString, char *sourceString )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareStrings";

		if( listQuery_ != NULL ||
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) != NULL )
			return listQuery_->compareStrings( searchString, sourceString );

		referenceResult.result = startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );
		return referenceResult;
		}

	ResultType List::itemQueryInList( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );

		listQuery_->itemQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );
		return RESULT_OK;
		}

	ResultType List::listQueryInList( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *queryListString )
		{
		bool isListIncludedInQuery = isIncludingThisList( queryListString );
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQueryInList";

		if( isSelectOnFind ||
		!isListIncludedInQuery )
			{
			if( listQuery_ == NULL &&
			// Create supporting module
			( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
				return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );

			listQuery_->listQuery( ( isSelectOnFind && isListIncludedInQuery ), isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems );
			}

		return RESULT_OK;
		}

	ResultType List::wordTypeQueryInList( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned short queryWordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );

		listQuery_->wordTypeQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr );
		return RESULT_OK;
		}

	ResultType List::parameterQueryInList( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned int queryParameter )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );

		listQuery_->parameterQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter );
		return RESULT_OK;
		}

	ResultType List::wordQueryInList( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQueryInList";

		if( listQuery_ == NULL &&
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) == NULL )
			return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );

		listQuery_->wordQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems );
		return RESULT_OK;
		}

	ResultType List::wordReferenceQueryInList( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordReferenceNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQueryInList";

		if( listQuery_ != NULL ||
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) != NULL )
			return listQuery_->wordReferenceQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString );

		return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );
		}

	ResultType List::stringQueryInList( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *queryString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQueryInList";

		if( listQuery_ != NULL ||
		// Create supporting module
		( listQuery_ = new ListQuery( commonVariables_, this ) ) != NULL )
			return listQuery_->stringQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString );

		return startError( functionNameString, NULL, myWordItem_->anyWordTypeString(), "I failed to create my list query module" );
		}

	ResultType List::showQueryResultInList( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		if( listQuery_ != NULL )
			return listQuery_->showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth );

		// In case the first query doesn't include admin lists,
		// the admin list query module isn't created yet
		return RESULT_OK;
		}

/*************************************************************************
 *	"But he rescues the poor from trouble
 *	and increases their families like flocks of sheep." (Psalm 107:41)
 *************************************************************************/
