/*
 *	Class:			ListCleanup
 *	Supports class:	List
 *	Purpose:		To cleanup unnecessary items in the lists
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

class ListCleanup
	{
	friend class List;

	// Private constructible variables

	CommonVariables *commonVariables_;
	List *myList_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void setCurrentItemNr( Item *searchItem )
		{
		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() &&
			searchItem->itemNr() > commonVariables_->currentItemNr )
				commonVariables_->currentItemNr = searchItem->itemNr();

			searchItem = searchItem->nextItem;
			}
		}

	unsigned int highestInUseSentenceNrInList( unsigned int highestSentenceNr, Item *searchItem )
		{
		unsigned int tempSentenceNr;
		unsigned int highestInUseSentenceNr = NO_SENTENCE_NR;

		while( searchItem != NULL &&
		highestInUseSentenceNr < highestSentenceNr )
			{
			tempSentenceNr = searchItem->activeSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem->inactiveSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem->archivedSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem->replacedSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			searchItem = searchItem->nextItem;
			}

		return highestInUseSentenceNr;
		}

	ResultType decrementSentenceNrs( unsigned int startSentenceNr, Item *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		while( searchItem != NULL )
			{
			if( searchItem->activeSentenceNr() >= startSentenceNr )
				{
				if( searchItem->activeSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementActiveSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to decrement the active sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I found an item with an active sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->inactiveSentenceNr() >= startSentenceNr )
				{
				if( searchItem->inactiveSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementInactiveSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to decrement the inactive sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I found an item with an inactive sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->originalSentenceNr() >= startSentenceNr )
				{
				if( searchItem->originalSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementOriginalSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to decrement the original sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I found an item with an original sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->creationSentenceNr() >= startSentenceNr )
				{
				if( searchItem->creationSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementCreationSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to decrement the creation sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I found an item with a creation sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->archivedSentenceNr() >= startSentenceNr )
				{
				if( searchItem->archivedSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementArchivedSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to decrement the archived sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I found an item with an archived sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->replacedSentenceNr() >= startSentenceNr )
				{
				if( searchItem->replacedSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementReplacedSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to decrement the replaced sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I found an item with a replaced sentence number equal to the given start sentence number ", startSentenceNr );
				}

			searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset, Item *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRange";

		while( searchItem != NULL )
			{
			if( searchItem->creationSentenceNr() == decrementSentenceNr &&
			searchItem->itemNr() >= startDecrementItemNr )
				{
				if( searchItem->itemNr() > startDecrementItemNr )
					{
					if( searchItem->decrementItemNr( decrementOffset ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to decrement the item number of an item with a certain offset" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I found an item number equal to the given start item number" );
				}

			searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	ListCleanup( CommonVariables *commonVariables, List *myList )
		{
		commonVariables_ = commonVariables;
		myList_ = myList;
		strcpy( moduleNameString_, "ListCleanup" );

		if( commonVariables_ != NULL )
			{
			if( myList_ == NULL )
				{
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given my list is undefined.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME );
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				}
			}
		else
			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given common variables is undefined.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME );
		}


	// Protected functions

	void setCurrentItemNr()
		{
		setCurrentItemNr( myList_->firstActiveItem() );
		setCurrentItemNr( myList_->firstInactiveItem() );
		setCurrentItemNr( myList_->firstArchivedItem() );
		setCurrentItemNr( myList_->firstReplacedItem() );
		setCurrentItemNr( myList_->firstDeletedItem() );
		}

	void deleteRollbackInfo()
		{
		Item *searchItem = myList_->firstDeletedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isAvailableForRollbackAfterDelete &&
			searchItem->deletedSentenceNr() >= commonVariables_->currentSentenceNr )
				searchItem->isAvailableForRollbackAfterDelete = false;

			searchItem = searchItem->nextItem;
			}
		}

	unsigned int highestInUseSentenceNrInList( bool isIncludingDeletedItems, unsigned int highestSentenceNr )
		{
		unsigned int tempSentenceNr;
		unsigned int highestInUseSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_->firstActiveItem() );

		if( ( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_->firstInactiveItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_->firstArchivedItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_->firstReplacedItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( isIncludingDeletedItems &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_->firstDeletedItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		return highestInUseSentenceNr;
		}

	ResultType decrementSentenceNrs( unsigned int startSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		if( startSentenceNr > NO_SENTENCE_NR )
			{
			if( decrementSentenceNrs( startSentenceNr, myList_->firstActiveItem() ) == RESULT_OK )
				{
				if( decrementSentenceNrs( startSentenceNr, myList_->firstInactiveItem() ) == RESULT_OK )
					{
					if( decrementSentenceNrs( startSentenceNr, myList_->firstArchivedItem() ) == RESULT_OK )
						{
						if( decrementSentenceNrs( startSentenceNr, myList_->firstReplacedItem() ) == RESULT_OK )
							return decrementSentenceNrs( startSentenceNr, myList_->firstDeletedItem() );
						}
					}
				}
			}
		else
			return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given start sentence number is undefined" );

		return RESULT_OK;
		}

	ResultType decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRange";

		if( decrementSentenceNr >= NO_SENTENCE_NR &&
		decrementSentenceNr < MAX_SENTENCE_NR )
			{
			if( startDecrementItemNr > NO_ITEM_NR )
				{
				if( decrementOffset > 0 )
					{
					if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_->firstActiveItem() ) == RESULT_OK )
						{
						if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_->firstInactiveItem() ) == RESULT_OK )
							{
							if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_->firstArchivedItem() ) == RESULT_OK )
								{
								if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_->firstReplacedItem() ) == RESULT_OK )
									return decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_->firstDeletedItem() );
								}
							}
						}
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given decrement offset is undefined" );
				}
			else
				return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given start item number is undefined" );
			}
		else
			return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given decrement sentence number is undefined" );

		return RESULT_OK;
		}

	ResultType deleteSentences( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		Item *searchItem = myList_->firstActiveItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		if( lowestSentenceNr > NO_SENTENCE_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->activeSentenceNr() >= lowestSentenceNr )
					{
					if( searchItem->creationSentenceNr() >= lowestSentenceNr )
						{
						if( myList_->deleteItem( isAvailableForRollback, searchItem ) == RESULT_OK )
							searchItem = myList_->nextListItem();
						else
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to delete an active item" );
						}
					else
						{
						if( searchItem->hasInactiveSentenceNr() )
							{
							if( myList_->inactivateItem( false, searchItem ) == RESULT_OK )
								searchItem = myList_->nextListItem();
							else
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate an active item" );
							}
						else
							searchItem = searchItem->nextItem;
						}
					}
				else
					searchItem = searchItem->nextItem;
				}

			searchItem = myList_->firstInactiveItem();

			while( searchItem != NULL )
				{
				if( searchItem->inactiveSentenceNr() >= lowestSentenceNr )
					{
					if( searchItem->creationSentenceNr() >= lowestSentenceNr )
						{
						if( myList_->deleteItem( isAvailableForRollback, searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to archive an inactive item" );
						}
					else
						{
						if( myList_->activateItem( false, searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate an inactive item" );
						}

					searchItem = myList_->nextListItem();
					}
				else
					searchItem = searchItem->nextItem;
				}

			searchItem = myList_->firstArchivedItem();

			while( searchItem != NULL )
				{
				if( searchItem->archivedSentenceNr() >= lowestSentenceNr )
					{
					if( searchItem->creationSentenceNr() >= lowestSentenceNr )
						{
						if( myList_->deleteItem( isAvailableForRollback, searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to delete an archived item" );
						}
					else
						{
						searchItem->clearArchivedSentenceNr();

						if( searchItem->wasActiveBefore() )
							{
							if( myList_->activateItem( true, searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate an archived item" );
							}
						else
							{
							if( myList_->inactivateItem( true, searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate an archived item" );
							}
						}

					searchItem = myList_->nextListItem();
					}
				else
					searchItem = searchItem->nextItem;
				}

			searchItem = myList_->firstReplacedItem();

			while( searchItem != NULL )
				{
				if( searchItem->replacedSentenceNr() >= lowestSentenceNr )
					{
					if( searchItem->creationSentenceNr() >= lowestSentenceNr )
						{
						if( myList_->deleteItem( isAvailableForRollback, searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to delete a replaced item" );
						}
					else
						{
						searchItem->clearReplacedSentenceNr();

						if( searchItem->wasActiveBefore() )
							{
							if( myList_->activateItem( true, searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate a replaced item" );
							}
						else
							{
							if( searchItem->wasInactiveBefore() )
								{
								if( myList_->inactivateItem( true, searchItem ) != RESULT_OK )
									return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate a replaced item" );
								}
							else
								{
								if( myList_->archiveItem( searchItem ) != RESULT_OK )
									return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to archive a replaced item" );
								}
							}
						}

					searchItem = myList_->nextListItem();
					}
				else
					searchItem = searchItem->nextItem;
				}
			}
		else
			return myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given lowest sentence number is undefined" );

		return RESULT_OK;
		}

	ResultType rollbackDeletedRedoInfo()
		{
		Item *searchItem = myList_->firstDeletedItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "rollbackDeletedRedoInfo";

		while( searchItem != NULL )
			{
			if( searchItem->isAvailableForRollbackAfterDelete &&
			searchItem->hasCurrentDeletedSentenceNr() )
				{
				searchItem->isAvailableForRollbackAfterDelete = false;

				if( myList_->removeItemFromList( searchItem ) == RESULT_OK )
					{
					if( searchItem->hasInactiveSentenceNr() )
						{
						if( searchItem->hasArchivedSentenceNr() )
							{
							if( myList_->addItemToList( QUERY_ARCHIVED_CHAR, searchItem ) == RESULT_OK )
								searchItem = myList_->nextListItem();
							else
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to add an item to the archive list" );
							}
						else
							{
							if( myList_->addItemToList( QUERY_INACTIVE_CHAR, searchItem ) == RESULT_OK )
								searchItem = myList_->nextListItem();
							else
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to add an item to the inactive list" );
							}
						}
					else
						{
						if( myList_->addItemToList( QUERY_ACTIVE_CHAR, searchItem ) == RESULT_OK )
							searchItem = myList_->nextListItem();
						else
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to add an item to the active list" );
						}
					}
				else
					return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to remove an item from the deleted list" );
				}
			else
				searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType undoCurrentSentence()
		{
		bool isAssignment = myList_->isAssignmentList();
		Item *searchItem = myList_->firstActiveItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoCurrentSentence";

		while( searchItem != NULL )
			{
			if( searchItem->activeSentenceNr() >= commonVariables_->currentSentenceNr )
				{
				if( searchItem->hasInactiveSentenceNr() &&
				searchItem->hasCurrentActiveSentenceNr() &&
				searchItem->inactiveSentenceNr() < searchItem->activeSentenceNr() )
					{
					if( myList_->inactivateItem( false, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate an active item" );
					}
				else
					{
					if( isAssignment )
						{
						if( myList_->archiveItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to archive an active item" );
						}
					else
						{
						if( myList_->replaceItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to replace an active item" );
						}
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->inactiveSentenceNr() >= commonVariables_->currentSentenceNr )
				{
				if( searchItem->hasActiveSentenceNr() &&
				searchItem->hasCurrentInactiveSentenceNr() &&
				searchItem->activeSentenceNr() < searchItem->inactiveSentenceNr() )
					{
					if( myList_->activateItem( false, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate an inactive item" );
					}
				else
					{
					if( myList_->archiveItem( searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to archive an inactive item" );
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstArchivedItem();

		while( searchItem != NULL )
			{
			if( ( searchItem->archivedSentenceNr() == commonVariables_->currentSentenceNr &&
			searchItem->creationSentenceNr() < commonVariables_->currentSentenceNr ) ||

			( searchItem->archivedSentenceNr() + 1 == commonVariables_->currentSentenceNr &&
			searchItem->creationSentenceNr() + 1 == commonVariables_->currentSentenceNr ) )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( myList_->activateItem( true, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate an archived item" );
					}
				else
					{
					if( myList_->inactivateItem( true, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate an archived item" );
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstReplacedItem();

		while( searchItem != NULL )
			{
			if( ( searchItem->replacedSentenceNr() == commonVariables_->currentSentenceNr &&
			searchItem->creationSentenceNr() < commonVariables_->currentSentenceNr ) ||

			( searchItem->replacedSentenceNr() + 1 == commonVariables_->currentSentenceNr &&
			searchItem->creationSentenceNr() + 1 == commonVariables_->currentSentenceNr ) )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( myList_->activateItem( true, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate a replaced item" );
					}
				else
					{
					if( searchItem->wasInactiveBefore() )
						{
						if( myList_->inactivateItem( true, searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate a replaced item" );
						}
					else
						{
						if( myList_->archiveItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to archive a replaced item" );
						}
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType redoCurrentSentence()
		{
		Item *searchItem = myList_->firstActiveItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentence";

		while( searchItem != NULL )
			{
			if( ( searchItem->hasCurrentInactiveSentenceNr() ||
			searchItem->hasCurrentArchivedSentenceNr() ||
			searchItem->hasCurrentReplacedSentenceNr() ) &&

			searchItem->activeSentenceNr() < commonVariables_->currentSentenceNr )
				{
				if( searchItem->inactiveSentenceNr() > searchItem->archivedSentenceNr() )
					{
					if( myList_->inactivateItem( false, searchItem ) == RESULT_OK )
						searchItem = myList_->nextListItem();
					else
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate an active item" );
					}
				else
					{
					if( searchItem->hasCurrentArchivedSentenceNr() )
						{
						if( myList_->archiveItem( searchItem ) == RESULT_OK )
							searchItem = myList_->nextListItem();
						else
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to archive an active item" );
						}
					else
						searchItem = searchItem->nextItem;
					}
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstInactiveItem();

		while( searchItem != NULL )
			{
			if( ( searchItem->hasCurrentActiveSentenceNr() ||
			searchItem->hasCurrentArchivedSentenceNr() ||
			searchItem->hasCurrentReplacedSentenceNr() ) &&

			searchItem->inactiveSentenceNr() < commonVariables_->currentSentenceNr )
				{
				if( searchItem->inactiveSentenceNr() > searchItem->archivedSentenceNr() )
					{
					if( myList_->activateItem( false, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate an inactive item" );
					}
				else
					{
					if( myList_->archiveItem( searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to archive an inactive item" );
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( myList_->activateItem( true, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate an archived item" );
					}
				else
					{
					if( myList_->inactivateItem( true, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate an archived item" );
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( myList_->activateItem( true, searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to activate a replaced item" );
					}
				else
					{
					if( searchItem->wasInactiveBefore() )
						{
						if( myList_->inactivateItem( true, searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate a replaced item" );
						}
					else
						{
						if( myList_->archiveItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to inactivate a replaced item" );
						}
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"The winds blows, and we are gone -
 *	as though we had never been here.
 *	But the love of the Lord remains forever
 *	with those who fear him." (Psalm 103:16-17)
 *
 *************************************************************************/
