/*
 *	Class:		List
 *	Purpose:	Base class to store the items of the knowledge structure
 *	Version:	Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

class List
	{
	// Private constructible variables

	private char listChar_;

	private Item activeList_;
	private Item inactiveList_;
	private Item archivedList_;
	private Item replacedList_;
	private Item deletedList_;

	private Item nextListItem_;

	private ListCleanup listCleanup_;
	private ListQuery listQuery_;

	private WordItem myWordItem_;


	// Private methods

	private boolean isIncludingThisList( StringBuffer queryListStringBuffer )
		{
		short index = 0;

		if( queryListStringBuffer != null &&
		queryListStringBuffer.length() > 0 )
			{
			while( index < queryListStringBuffer.length() &&
			queryListStringBuffer.charAt( index ) != listChar_ )
				index++;

			if( index == queryListStringBuffer.length() )
				return false;
			}

		return true;
		}

	private static Item tailOfList( Item searchItem )
		{
		Item nextItem;

		while( searchItem != null &&
		( nextItem = searchItem.nextItem ) != null )
			searchItem = nextItem;

		return searchItem;
		}


	// Constructor / deconstructor

	protected List()
		{
		// Private constructible variables

		listChar_ = Constants.QUERY_NO_LIST_CHAR;

		activeList_ = null;
		inactiveList_ = null;
		archivedList_ = null;
		replacedList_ = null;
		deletedList_ = null;

		nextListItem_ = null;

		listCleanup_ = null;
		listQuery_ = null;

		myWordItem_ = null;
		}


	// Protected error methods

	protected byte addError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		Presentation.showError( listChar_, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), wordNameString, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}

	protected byte startError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;

		return Constants.RESULT_ERROR;
		}

	protected byte startSystemError( int methodLevel, String moduleNameString, String wordNameString, String errorString )
		{
		addError( ( methodLevel + 1 ), moduleNameString, wordNameString, errorString );

		CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;

		return Constants.RESULT_SYSTEM_ERROR;
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return false;
		}

	protected ReferenceResultType findWordReference( WordItem referenceWordItem )
		{
		// This is a virtual method. Therefore the given variables are unreferenced
		return new ReferenceResultType();
		}


	// Protected common methods

	protected void initializeListVariables( char listChar, WordItem myWordItem )
		{
		listChar_ = listChar;

		myWordItem_ = myWordItem;

		if( myWordItem_ != null )
			{
			// Always create the list cleanup module
			if( ( listCleanup_ = new ListCleanup( this ) ) == null )
				startSystemError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list cleanup module" );
			}
		else
			startSystemError( 1, null, null, "The given my word is undefined" );
		}

	protected void deleteTemporaryList()
		{
		Item searchItem = deletedList_;

		if( replacedList_ != null )
			{
			// Move replaced list to deleted list
			if( deletedList_ == null )
				deletedList_ = replacedList_;
			else
				{
				searchItem = tailOfList( searchItem );
				searchItem.nextItem = replacedList_;
				}

			replacedList_ = null;
			}

		if( archivedList_ != null )
			{
			// Move archived list to deleted list
			if( deletedList_ == null )
				deletedList_ = archivedList_;
			else
				{
				searchItem = tailOfList( searchItem );
				searchItem.nextItem = archivedList_;
				}

			archivedList_ = null;
			}

		if( inactiveList_ != null )
			{
			// Move inactive list to deleted list
			if( deletedList_ == null )
				deletedList_ = inactiveList_;
			else
				{
				searchItem = tailOfList( searchItem );
				searchItem.nextItem = inactiveList_;
				}

			inactiveList_ = null;
			}

		if( activeList_ != null )
			{
			// Move active list to deleted list
			if( deletedList_ == null )
				deletedList_ = activeList_;
			else
				{
				searchItem = tailOfList( searchItem );
				searchItem.nextItem = activeList_;
				}

			activeList_ = null;
			}
		}

	protected boolean hasItems()
		{
		return ( activeList_ != null ||
				inactiveList_ != null ||
				archivedList_ != null ||
				replacedList_ != null );
		}

	protected boolean isAdminList()
		{
		return Character.isUpperCase( listChar_ );
		}

	protected boolean isAssignmentList()
		{
		return ( listChar_ == Constants.WORD_ASSIGNMENT_LIST_SYMBOL );
		}

	protected boolean isReadList()
		{
		return ( listChar_ == Constants.ADMIN_READ_LIST_SYMBOL );
		}

	protected int highestSentenceNrInList()
		{
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		Item searchItem = activeList_;

		while( searchItem != null )
			{
			if( searchItem.creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem.creationSentenceNr();

			searchItem = searchItem.nextItem;
			}

		searchItem = inactiveList_;

		while( searchItem != null )
			{
			if( searchItem.creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem.creationSentenceNr();

			searchItem = searchItem.nextItem;
			}

		searchItem = archivedList_;

		while( searchItem != null )
			{
			if( searchItem.creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem.creationSentenceNr();

			searchItem = searchItem.nextItem;
			}

		searchItem = replacedList_;

		while( searchItem != null )
			{
			if( searchItem.creationSentenceNr() > highestSentenceNr )
				highestSentenceNr = searchItem.creationSentenceNr();

			searchItem = searchItem.nextItem;
			}

		return highestSentenceNr;
		}

	protected char listChar()
		{
		return listChar_;
		}

	protected byte addItemToList( char statusChar, Item newItem )
		{
		int newCreationSentenceNr;
		Item searchItem;
		Item previousSearchItem = null;

		if( newItem != null )
			{
			if( newItem.myList() == this )
				{
				if( newItem.nextItem == null )
					{
					switch( statusChar )
						{
						case Constants.QUERY_ACTIVE_CHAR:
							newItem.setActiveSentenceNr();
							newItem.setActiveStatus();
							searchItem = activeList_;
							break;

						case Constants.QUERY_INACTIVE_CHAR:
							newItem.setInactiveSentenceNr();
							newItem.setInactiveStatus();
							searchItem = inactiveList_;
							break;

						case Constants.QUERY_ARCHIVED_CHAR:
							newItem.setArchivedSentenceNr();
							newItem.setArchivedStatus();
							searchItem = archivedList_;
							break;

						case Constants.QUERY_REPLACED_CHAR:
							newItem.setReplacedSentenceNr();
							newItem.setReplacedStatus();
							searchItem = replacedList_;
							break;

						case Constants.QUERY_DELETED_CHAR:
							newItem.setDeletedSentenceNr();
							newItem.setDeletedStatus();
							searchItem = deletedList_;
							break;

						default:
							return startError( 1, null, myWordItem_.anyWordTypeString(), "The given status character is unknown" );
						}

					// Sort item in list
					if( statusChar == Constants.QUERY_DELETED_CHAR )
						{
						newCreationSentenceNr = newItem.creationSentenceNr();

						while( searchItem != null &&

						// 1) Descending creationSentenceNr
						( searchItem.creationSentenceNr() > newCreationSentenceNr ||

						// 2) Ascending itemNr
						( searchItem.creationSentenceNr() == newCreationSentenceNr &&
						searchItem.itemNr() < newItem.itemNr() ) ) )
							{
							previousSearchItem = searchItem;
							searchItem = searchItem.nextItem;
							}
						}
					else
						{
						while( searchItem != null &&
						!newItem.isSorted( searchItem ) )
							{
							previousSearchItem = searchItem;
							searchItem = searchItem.nextItem;
							}
						}

					if( searchItem == null ||
					searchItem.creationSentenceNr() != newItem.creationSentenceNr() ||	// Check on duplicates
					searchItem.itemNr() != newItem.itemNr() )								// for integrity
						{
						newItem.previousItem = previousSearchItem;

						if( previousSearchItem == null )	// First item in list
							{
							switch( statusChar )
								{
								case Constants.QUERY_ACTIVE_CHAR:
									if( activeList_ != null )
										activeList_.previousItem = newItem;

									newItem.nextItem = activeList_;
									activeList_ = newItem;
									break;

								case Constants.QUERY_INACTIVE_CHAR:
									if( inactiveList_ != null )
										inactiveList_.previousItem = newItem;

									newItem.nextItem = inactiveList_;
									inactiveList_ = newItem;
									break;

								case Constants.QUERY_ARCHIVED_CHAR:
									if( archivedList_ != null )
										archivedList_.previousItem = newItem;

									newItem.nextItem = archivedList_;
									archivedList_ = newItem;
									break;

								case Constants.QUERY_REPLACED_CHAR:
									if( replacedList_ != null )
										replacedList_.previousItem = newItem;

									newItem.nextItem = replacedList_;
									replacedList_ = newItem;
									break;

								case Constants.QUERY_DELETED_CHAR:
									if( deletedList_ != null )
										deletedList_.previousItem = newItem;

									newItem.nextItem = deletedList_;
									deletedList_ = newItem;
									break;

								default:
									return startError( 1, null, myWordItem_.anyWordTypeString(), "The given status character is unknown" );
								}
							}
						else
							{
							if( searchItem != null )
								searchItem.previousItem = newItem;

							newItem.nextItem = previousSearchItem.nextItem;
							previousSearchItem.nextItem = newItem;
							}
						}
					else
						return startError( 1, null, myWordItem_.anyWordTypeString(), "I found an active item with the same identification" );
					}
				else
					return startError( 1, null, myWordItem_.anyWordTypeString(), "The given new item seems to be a part of a list" );
				}
			else
				return startError( 1, null, myWordItem_.anyWordTypeString(), "The given new item doesn't belong to my list" );
			}
		else
			return startError( 1, null, myWordItem_.anyWordTypeString(), "The given new item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte activateItem( boolean isSkipTest, Item activateItem )
		{
		if( activateItem != null )
			{
			if( activateItem.statusChar() != Constants.QUERY_ACTIVE_CHAR )
				{
				if( isSkipTest ||
				!activateItem.hasActiveSentenceNr() ||
				activateItem.hasCurrentActiveSentenceNr() ||
				activateItem.hasCurrentInactiveSentenceNr() )
					{
					if( removeItemFromList( activateItem ) == Constants.RESULT_OK )
						{
						if( addItemToList( Constants.QUERY_ACTIVE_CHAR, activateItem ) == Constants.RESULT_OK )
							{
							if( isAssignmentList() &&
							CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
								CommonVariables.isAssignmentChanged = true;
							}
						else
							return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to add an item to the active list" );
						}
					else
						return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to remove an item from the archive list" );
					}
				else
					return startError( 1, null, myWordItem_.anyWordTypeString(), "The active sentence number of the given archived item is already assigned" );
				}
			else
				return startError( 1, null, myWordItem_.anyWordTypeString(), "The given activate item is already an active item" );
			}
		else
			return startError( 1, null, myWordItem_.anyWordTypeString(), "The given activate item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte inactivateItem( boolean isSkipTest, Item inactivateItem )
		{
		if( inactivateItem != null )
			{
			if( inactivateItem.statusChar() != Constants.QUERY_INACTIVE_CHAR )
				{
				if( isAssignmentList() ||
				isReadList() )
					{
					if( isSkipTest ||
					!inactivateItem.hasInactiveSentenceNr() ||
					inactivateItem.hasCurrentActiveSentenceNr() ||
					inactivateItem.hasCurrentInactiveSentenceNr() )
						{
						if( removeItemFromList( inactivateItem ) == Constants.RESULT_OK )
							{
							if( addItemToList( Constants.QUERY_INACTIVE_CHAR, inactivateItem ) == Constants.RESULT_OK )
								{
								if( isAssignmentList() &&
								CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
									CommonVariables.isAssignmentChanged = true;
								}
							else
								return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to add an item to the inactive list" );
							}
						else
							return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to remove an item from the archive list" );
						}
					else
						return startError( 1, null, myWordItem_.anyWordTypeString(), "The inactive sentence number of the given archived item is already assigned" );
					}
				else
					return startError( 1, null, myWordItem_.anyWordTypeString(), "Only assignments, Guide by Grammar items and read items can be inactived" );
				}
			else
				return startError( 1, null, myWordItem_.anyWordTypeString(), "The given inactivate item is already an inactive item" );
			}
		else
			return startError( 1, null, myWordItem_.anyWordTypeString(), "The given inactivate item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte archiveItem( Item archiveItem )
		{
		if( archiveItem != null )
			{
			if( archiveItem.statusChar() != Constants.QUERY_ARCHIVED_CHAR )
				{
				if( isAssignmentList() )
					{
					if( removeItemFromList( archiveItem ) == Constants.RESULT_OK )
						{
						archiveItem.previousStatusChar = archiveItem.statusChar();

						if( addItemToList( Constants.QUERY_ARCHIVED_CHAR, archiveItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
								CommonVariables.isAssignmentChanged = true;
							}
						else
							return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to add an item to the archived list" );
						}
					else
						return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to remove an item from a list" );
					}
				else
					return startError( 1, null, myWordItem_.anyWordTypeString(), "Only assignments can be archived" );
				}
			else
				return startError( 1, null, myWordItem_.anyWordTypeString(), "The given archive item is already an archived item" );
			}
		else
			return startError( 1, null, myWordItem_.anyWordTypeString(), "The given archive item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte replaceItem( Item replaceItem )
		{
		if( replaceItem != null )
			{
			if( replaceItem.statusChar() != Constants.QUERY_REPLACED_CHAR )
				{
				if( removeItemFromList( replaceItem ) == Constants.RESULT_OK )
					{
					replaceItem.previousStatusChar = replaceItem.statusChar();

					if( addItemToList( Constants.QUERY_REPLACED_CHAR, replaceItem ) != Constants.RESULT_OK )
						return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to add an item to the replaced list" );
					}
				else
					return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to remove an item from a list" );
				}
			else
				return startError( 1, null, myWordItem_.anyWordTypeString(), "The given replace item is already a replaced item" );
			}
		else
			return startError( 1, null, myWordItem_.anyWordTypeString(), "The given replace item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte deleteItem( boolean isAvailableForRollback, Item deleteItem )
		{
		if( removeItemFromList( deleteItem ) == Constants.RESULT_OK )
			{
			if( deleteItem.statusChar() != Constants.QUERY_DELETED_CHAR )
				{
				if( addItemToList( Constants.QUERY_DELETED_CHAR, deleteItem ) == Constants.RESULT_OK )
					deleteItem.isAvailableForRollbackAfterDelete = isAvailableForRollback;
				else
					return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to add an item to the deleted list" );
				}
			else
				return startError( 1, null, myWordItem_.anyWordTypeString(), "The given delete item is already a deleted item" );
			}
		else
			return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to remove an item from a list" );

		return Constants.RESULT_OK;
		}

	protected byte deleteActiveItemsWithCurrentSentenceNr()
		{
		Item searchItem = activeList_;

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
					searchItem = nextListItem_;
				else
					return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to delete an active item" );
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItemsInList()
		{
		Item removeItem = deletedList_;
		Item previousRemoveItem = null;

		if( CommonVariables.nDeletedItems == 0 &&
		CommonVariables.removeSentenceNr == 0 &&
		CommonVariables.removeStartItemNr == 0 )
			{
			while( removeItem != null &&										// Skip items that must be kept for rollback
			removeItem.isAvailableForRollbackAfterDelete )						// and items of current sentence (if wanted)
				{
				previousRemoveItem = removeItem;
				removeItem = removeItem.nextItem;
				}

			if( removeItem != null )											// Found items to remove
				{
				CommonVariables.removeSentenceNr = removeItem.creationSentenceNr();
				CommonVariables.removeStartItemNr = removeItem.itemNr();

				do	{
					if( previousRemoveItem == null )
						deletedList_ = deletedList_.nextItem;					// Disconnect deleted list from item
					else
						previousRemoveItem.nextItem = removeItem.nextItem;	// Disconnect deleted list from item

					if( removeItem.checkForUsage() == Constants.RESULT_OK )
						{
						removeItem.nextItem = null;							// Disconnect item from the deleted list
						removeItem = ( previousRemoveItem == null ? deletedList_ : previousRemoveItem.nextItem );
						CommonVariables.nDeletedItems++;
						}
					else
						return addError( 1, null, myWordItem_.anyWordTypeString(), "I failed to check an item for its usage" );
					}
				while( removeItem != null &&
				removeItem.creationSentenceNr() == CommonVariables.removeSentenceNr &&							// Same sentence number
				removeItem.itemNr() == CommonVariables.removeStartItemNr + CommonVariables.nDeletedItems );	// Ascending item number
				}
			}
		else
			return startError( 1, null, myWordItem_.anyWordTypeString(), "There is already a range of deleted items" );

		return Constants.RESULT_OK;
		}

	protected byte removeItemFromList( Item removeItem )
		{
		if( removeItem != null )
			{
			if( removeItem.myList() == this )
				{
				if( removeItem.previousItem == null )		// First item in list
					{
					switch( removeItem.statusChar() )
						{
						case Constants.QUERY_ACTIVE_CHAR:
							activeList_ = removeItem.nextItem;
							break;

						case Constants.QUERY_INACTIVE_CHAR:
							inactiveList_ = removeItem.nextItem;
							break;

						case Constants.QUERY_ARCHIVED_CHAR:
							archivedList_ = removeItem.nextItem;
							break;

						case Constants.QUERY_REPLACED_CHAR:
							replacedList_ = removeItem.nextItem;
							break;

						case Constants.QUERY_DELETED_CHAR:
							deletedList_ = removeItem.nextItem;
							break;

						default:
							return startError( 1, null, myWordItem_.anyWordTypeString(), "The given remove item has an unknown status character" );
						}

					if( removeItem.nextItem != null )
						removeItem.nextItem.previousItem = null;
					}
				else
					{
					removeItem.previousItem.nextItem = removeItem.nextItem;

					if( removeItem.nextItem != null )
						removeItem.nextItem.previousItem = removeItem.previousItem;
					}

				nextListItem_ = removeItem.nextItem;		// Remember next item

				// Disconnect item from the list
				removeItem.previousItem = null;
				removeItem.nextItem = null;
				}
			else
				return startError( 1, null, myWordItem_.anyWordTypeString(), "The given remove item doesn't belong to my list" );
			}
		else
			return startError( 1, null, myWordItem_.anyWordTypeString(), "The given remove item is undefined" );

		return Constants.RESULT_OK;
		}

	protected Item firstActiveItem()
		{
		return activeList_;
		}

	protected Item firstInactiveItem()
		{
		return inactiveList_;
		}

	protected Item firstArchivedItem()
		{
		return archivedList_;
		}

	protected Item firstReplacedItem()
		{
		return replacedList_;
		}

	protected Item firstDeletedItem()
		{
		return deletedList_;
		}

	protected Item nextListItem()
		{
		return nextListItem_;
		}

	protected WordItem myWordItem()
		{
		return myWordItem_;
		}


	// Protected cleanup methods

	protected void deleteRollbackInfoInList()
		{
		listCleanup_.deleteRollbackInfo();
		}

	protected void getHighestInUseSentenceNrInList( boolean isIncludingDeletedItems, int highestSentenceNr )
		{
		int tempSentenceNr;

		if( ( tempSentenceNr = listCleanup_.highestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr ) ) > CommonVariables.highestInUseSentenceNr )
			CommonVariables.highestInUseSentenceNr = tempSentenceNr;
		}

	protected void setCurrentItemNrInList()
		{
		listCleanup_.setCurrentItemNr();
		}

	protected byte decrementItemNrRangeInList( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		return listCleanup_.decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset );
		}

	protected byte deleteSentencesInList( boolean isAvailableForRollback, int lowestSentenceNr )
		{
		return listCleanup_.deleteSentences( isAvailableForRollback, lowestSentenceNr );
		}

	protected byte decrementSentenceNrsInList( int startSentenceNr )
		{
		return listCleanup_.decrementSentenceNrs( startSentenceNr );
		}

	protected byte rollbackDeletedRedoInfoInList()
		{
		return listCleanup_.rollbackDeletedRedoInfo();
		}

	protected byte undoCurrentSentenceInList()
		{
		return listCleanup_.undoCurrentSentence();
		}

	protected byte redoCurrentSentenceInList()
		{
		return listCleanup_.redoCurrentSentence();
		}


	// Protected query methods

	protected void countQueryInList()
		{
		if( listQuery_ != null )
			listQuery_.countQuery();
		}

	protected void clearQuerySelectionsInList()
		{
		if( listQuery_ != null )
			listQuery_.clearQuerySelections();
		}

	protected ReferenceResultType compareStrings( String searchString, String sourceString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( listQuery_ != null ||
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) != null )
			return listQuery_.compareStrings( searchString, sourceString );

		referenceResult.result = startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );
		return referenceResult;
		}

	protected byte itemQueryInList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );

		listQuery_.itemQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );
		return Constants.RESULT_OK;
		}

	protected byte listQueryInList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, StringBuffer queryListStringBuffer )
		{
		boolean isListIncludedInQuery = isIncludingThisList( queryListStringBuffer );

		if( isSelectOnFind ||
		!isListIncludedInQuery )
			{
			if( listQuery_ == null &&
			// Create supporting module
			( listQuery_ = new ListQuery( this ) ) == null )
				return startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );

			listQuery_.listQuery( ( isSelectOnFind && isListIncludedInQuery ), isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems );
			}

		return Constants.RESULT_OK;
		}

	protected byte wordTypeQueryInList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, short queryWordTypeNr )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );

		listQuery_.wordTypeQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr );
		return Constants.RESULT_OK;
		}

	protected byte parameterQueryInList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, int queryParameter )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );

		listQuery_.parameterQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter );
		return Constants.RESULT_OK;
		}

	protected byte wordQueryInList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );

		listQuery_.wordQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems );
		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQueryInList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordReferenceNameString )
		{
		if( listQuery_ != null ||
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) != null )
			return listQuery_.wordReferenceQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString );

		return startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );
		}

	protected byte stringQueryInList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String queryString )
		{
		if( listQuery_ != null ||
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) != null )
			return listQuery_.stringQuery( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString );

		return startError( 1, null, myWordItem_.anyWordTypeString(), "I failed to create my list query module" );
		}

	protected byte showQueryResultInList( boolean showOnlyWords, boolean showOnlyWordReferences, boolean showOnlyStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		if( listQuery_ != null )
			return listQuery_.showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth );

		return startError( 1, null, myWordItem_.anyWordTypeString(), "My list query module isn't created yet" );
		}
	};

/*************************************************************************
 *	"But he rescues the poor from trouble
 *	and increases their families like flocks of sheep." (Psalm 107:41)
 *************************************************************************/
