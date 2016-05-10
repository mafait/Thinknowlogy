/*	Class:			ListCleanup
 *	Supports class:	List
 *	Purpose:		To cleanup obsolete items in the lists
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

#include "List.h"

class ListCleanup
	{
	friend class List;

	// Private constructible variables

	CommonVariables *commonVariables_;
	List *myList_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void clearReplacedInfo( Item *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->clearReplacingItem();

			if( searchItem->hasCurrentReplacedSentenceNr() )
				searchItem->clearReplacedSentenceNr();

			searchItem = searchItem->nextItem;
			}
		}

	void setCurrentItemNr( Item *searchItem )
		{
		unsigned int tempItemNr;
		unsigned int currentItemNr = commonVariables_->currentItemNr;

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() &&
			( tempItemNr = searchItem->itemNr() ) > currentItemNr )
				currentItemNr = tempItemNr;

			searchItem = searchItem->nextItem;
			}

		commonVariables_->currentItemNr = currentItemNr;
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
						return myList_->addError( functionNameString, moduleNameString_, "I failed to decrement the active sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "I have found an item with an active sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->inactiveSentenceNr() >= startSentenceNr )
				{
				if( searchItem->inactiveSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementInactiveSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to decrement the inactive sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "I have found an item with an inactive sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->originalSentenceNr() >= startSentenceNr )
				{
				if( searchItem->originalSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementOriginalSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to decrement the original sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "I have found an item with an original sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->creationSentenceNr() >= startSentenceNr )
				{
				if( searchItem->creationSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementCreationSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to decrement the creation sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "I have found an item with a creation sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->archivedSentenceNr() >= startSentenceNr )
				{
				if( searchItem->archivedSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementArchivedSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to decrement the archived sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "I have found an item with an archived sentence number equal to the given start sentence number ", startSentenceNr );
				}

			if( searchItem->replacedSentenceNr() >= startSentenceNr )
				{
				if( searchItem->replacedSentenceNr() > startSentenceNr )
					{
					if( searchItem->decrementReplacedSentenceNr() != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to decrement the replaced sentence number of an item" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "I have found an item with a replaced sentence number equal to the given start sentence number ", startSentenceNr );
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
						return myList_->addError( functionNameString, moduleNameString_, "I failed to decrement the item number of an item with a certain offset" );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "I have found an item number equal to the given start item number" );
				}

			searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	ListCleanup( CommonVariables *commonVariables, List *myList )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myList_ = myList;
		strcpy( moduleNameString_, "ListCleanup" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myList_ == NULL )
			strcpy( errorString, "The given my list is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myList_ != NULL &&
			myList_->myWordItem() != NULL )
				myList_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void clearReplacedInfo()
		{
		Item *searchItem;

		if( ( searchItem = myList_->firstActiveItem() ) != NULL )
			clearReplacedInfo( searchItem );

		if( ( searchItem = myList_->firstInactiveItem() ) != NULL )
			clearReplacedInfo( searchItem );

		if( ( searchItem = myList_->firstArchivedItem() ) != NULL )
			clearReplacedInfo( searchItem );
		}

	void setCurrentItemNr()
		{
		Item *searchItem;

		if( ( searchItem = myList_->firstActiveItem() ) != NULL )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_->firstInactiveItem() ) != NULL )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_->firstArchivedItem() ) != NULL )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_->firstReplacedItem() ) != NULL )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_->firstDeletedItem() ) != NULL )
			setCurrentItemNr( searchItem );
		}

	unsigned int highestInUseSentenceNrInList( bool isIncludingDeletedItems, unsigned int highestSentenceNr )
		{
		unsigned int tempSentenceNr;
		unsigned int highestInUseSentenceNr = NO_SENTENCE_NR;
		Item *searchItem;

		if( ( searchItem = myList_->firstActiveItem() ) != NULL )
			highestInUseSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem );

		if( ( searchItem = myList_->firstInactiveItem() ) != NULL &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( searchItem = myList_->firstArchivedItem() ) != NULL &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( searchItem = myList_->firstReplacedItem() ) != NULL &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( isIncludingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		return highestInUseSentenceNr;
		}

	ResultType decrementSentenceNrs( unsigned int startSentenceNr )
		{
		Item *searchItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrs";

		if( startSentenceNr > NO_SENTENCE_NR )
			{
			if( ( searchItem = myList_->firstActiveItem() ) != NULL )
				decrementSentenceNrs( startSentenceNr, searchItem );

			if( commonVariables_->result == RESULT_OK &&
			( searchItem = myList_->firstInactiveItem() ) != NULL )
				decrementSentenceNrs( startSentenceNr, searchItem );

			if( commonVariables_->result == RESULT_OK &&
			( searchItem = myList_->firstArchivedItem() ) != NULL )
				decrementSentenceNrs( startSentenceNr, searchItem );

			if( commonVariables_->result == RESULT_OK &&
			( searchItem = myList_->firstReplacedItem() ) != NULL )
				decrementSentenceNrs( startSentenceNr, searchItem );

			if( commonVariables_->result == RESULT_OK &&
			( searchItem = myList_->firstDeletedItem() ) != NULL )
				decrementSentenceNrs( startSentenceNr, searchItem );
			}
		else
			return myList_->startError( functionNameString, moduleNameString_, "The given start sentence number is undefined" );

		return RESULT_OK;
		}

	ResultType decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		Item *searchItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRange";

		if( decrementSentenceNr >= NO_SENTENCE_NR &&
		decrementSentenceNr < MAX_SENTENCE_NR )
			{
			if( startDecrementItemNr > NO_ITEM_NR )
				{
				if( decrementOffset > 0 )
					{
					if( ( searchItem = myList_->firstActiveItem() ) != NULL )
						decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

					if( commonVariables_->result == RESULT_OK &&
					( searchItem = myList_->firstInactiveItem() ) != NULL )
						decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

					if( commonVariables_->result == RESULT_OK &&
					( searchItem = myList_->firstArchivedItem() ) != NULL )
						decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

					if( commonVariables_->result == RESULT_OK &&
					( searchItem = myList_->firstReplacedItem() ) != NULL )
						decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

					if( commonVariables_->result == RESULT_OK &&
					( searchItem = myList_->firstDeletedItem() ) != NULL )
						decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );
					}
				else
					return myList_->startError( functionNameString, moduleNameString_, "The given decrement offset is undefined" );
				}
			else
				return myList_->startError( functionNameString, moduleNameString_, "The given start item number is undefined" );
			}
		else
			return myList_->startError( functionNameString, moduleNameString_, "The given decrement sentence number is undefined" );

		// Clear error to be able to restart after justification error
		return RESULT_OK;
		}

	ResultType deleteSentences( unsigned int lowestSentenceNr )
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
						if( myList_->deleteItem( searchItem ) == RESULT_OK )
							searchItem = myList_->nextListItem();
						else
							return myList_->addError( functionNameString, moduleNameString_, "I failed to delete an active item" );
						}
					else
						{
						if( searchItem->hasInactiveSentenceNr() )
							{
							if( myList_->inactivateItem( searchItem ) == RESULT_OK )
								searchItem = myList_->nextListItem();
							else
								return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate an active item" );
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
						if( myList_->deleteItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to delete an inactive item" );
						}
					else
						{
						if( myList_->activateItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to activate an inactive item" );
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
						if( myList_->deleteItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to delete an archived item" );
						}
					else
						{
						searchItem->clearArchivedSentenceNr();

						if( searchItem->wasActiveBefore() )
							{
							if( myList_->activateItem( searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, "I failed to activate an archived item" );
							}
						else
							{
							if( myList_->inactivateItem( searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate an archived item" );
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
						if( myList_->deleteItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to delete a replaced item" );
						}
					else
						{
//						searchItem->clearReplacingItem();
//						searchItem->clearReplacedSentenceNr();

						if( searchItem->wasActiveBefore() )
							{
							if( myList_->activateItem( searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, "I failed to activate a replaced item" );
							}
						else
							{
							if( searchItem->wasInactiveBefore() )
								{
								if( myList_->inactivateItem( searchItem ) != RESULT_OK )
									return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate a replaced item" );
								}
							else
								{
								if( myList_->archiveItem( searchItem ) != RESULT_OK )
									return myList_->addError( functionNameString, moduleNameString_, "I failed to archive a replaced item" );
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
			return myList_->startError( functionNameString, moduleNameString_, "The given lowest sentence number is undefined" );

		return RESULT_OK;
		}

	ResultType redoCurrentSentence()
		{
		Item *searchItem = myList_->firstActiveItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoCurrentSentence";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentInactiveSentenceNr() )
				{
				if( myList_->inactivateItem( searchItem ) == RESULT_OK )
					searchItem = myList_->nextListItem();
				else
					return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate an active item" );
				}
			else
				{
				if( searchItem->hasCurrentArchivedSentenceNr() )
					{
					if( myList_->archiveItem( searchItem ) == RESULT_OK )
						searchItem = myList_->nextListItem();
					else
						return myList_->addError( functionNameString, moduleNameString_, "I failed to archive an active item" );
					}
				else
					searchItem = searchItem->nextItem;
				}
			}

		searchItem = myList_->firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( myList_->activateItem( searchItem ) == RESULT_OK )
					searchItem = myList_->nextListItem();
				else
					return myList_->addError( functionNameString, moduleNameString_, "I failed to activate an inactive item" );
				}
			else
				{
				if( searchItem->hasCurrentArchivedSentenceNr() )
					{
					if( myList_->archiveItem( searchItem ) == RESULT_OK )
						searchItem = myList_->nextListItem();
					else
						return myList_->addError( functionNameString, moduleNameString_, "I failed to archive an inactive item" );
					}
				else
					searchItem = searchItem->nextItem;
				}
			}

		searchItem = myList_->firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( myList_->activateItem( searchItem ) == RESULT_OK )
					searchItem = myList_->nextListItem();
				else
					return myList_->addError( functionNameString, moduleNameString_, "I failed to activate an archived item" );
				}
			else
				{
				if( searchItem->hasCurrentInactiveSentenceNr() )
					{
					if( myList_->inactivateItem( searchItem ) == RESULT_OK )
						searchItem = myList_->nextListItem();
					else
						return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate an archived item" );
					}
				else
					searchItem = searchItem->nextItem;
				}
			}

		searchItem = myList_->firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( myList_->activateItem( searchItem ) == RESULT_OK )
					searchItem = myList_->nextListItem();
				else
					return myList_->addError( functionNameString, moduleNameString_, "I failed to activate a replaced item" );
				}
			else
				{
				if( searchItem->hasCurrentInactiveSentenceNr() )
					{
					if( myList_->inactivateItem( searchItem ) == RESULT_OK )
						searchItem = myList_->nextListItem();
					else
						return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate a replaced item" );
					}
				else
					{
					if( searchItem->hasCurrentArchivedSentenceNr() )
						{
						if( myList_->archiveItem( searchItem ) == RESULT_OK )
							searchItem = myList_->nextListItem();
						else
							return myList_->addError( functionNameString, moduleNameString_, "I failed to archive a replaced item" );
						}
					else
						searchItem = searchItem->nextItem;
					}
				}
			}

		searchItem = myList_->firstDeletedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentOriginalSentenceNr() )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( myList_->activateItem( searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to activate a deleted item" );
					}
				else
					{
					if( searchItem->wasInactiveBefore() )
						{
						if( myList_->inactivateItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate a deleted item" );
						}
					else
						{
						if( myList_->archiveItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate a deleted item" );
						}
					}

				searchItem = myList_->nextListItem();
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
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( searchItem->hasCurrentCreationSentenceNr() )
					{
					if( myList_->deleteItem( searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to delete an active item" );
					}
				else
					{
					if( searchItem->hasInactiveSentenceNr() &&
					searchItem->hasCurrentActiveSentenceNr() &&
					searchItem->inactiveSentenceNr() < searchItem->activeSentenceNr() )
						{
						if( myList_->inactivateItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate an active item" );
						}
					else
						{
						// Only assignments can be archived
						if( isAssignment )
							{
							if( myList_->archiveItem( searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, "I failed to archive an active item" );
							}
						else
							{
							if( myList_->replaceItem( searchItem ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, "I failed to replace an active item" );
							}
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
			if( searchItem->hasCurrentInactiveSentenceNr() )
				{
				if( searchItem->hasCurrentCreationSentenceNr() )
					{
					if( myList_->deleteItem( searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to delete an inactive item" );
					}
				else
					{
					if( searchItem->hasActiveSentenceNr() &&
					searchItem->activeSentenceNr() < searchItem->inactiveSentenceNr() )
						{
						if( myList_->activateItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to activate an inactive item" );
						}
					else
						{
						if( myList_->archiveItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to archive an inactive item" );
						}
					}

				searchItem = myList_->nextListItem();
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentArchivedSentenceNr() )
				{
				if( searchItem->hasCurrentCreationSentenceNr() )
					{
					if( myList_->deleteItem( searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to delete an archived item" );
					}
				else
					{
					if( searchItem->wasActiveBefore() )
						{
						if( myList_->activateItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to activate an archived item" );
						}
					else
						{
						if( myList_->inactivateItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate an archived item" );
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
			if( searchItem->hasCurrentReplacedSentenceNr() )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( myList_->activateItem( searchItem ) != RESULT_OK )
						return myList_->addError( functionNameString, moduleNameString_, "I failed to activate a replaced item" );
					}
				else
					{
					if( searchItem->wasInactiveBefore() )
						{
						if( myList_->inactivateItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to inactivate a replaced item" );
						}
					else
						{
						if( myList_->archiveItem( searchItem ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, "I failed to archive a replaced item" );
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
 *	"The winds blows, and we are gone -
 *	as though we had never been here.
 *	But the love of the Lord remains forever
 *	with those who fear him." (Psalm 103:16-17)
 *************************************************************************/
