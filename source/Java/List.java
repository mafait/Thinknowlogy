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

class List
	{
	// Private constructed variables

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

	private boolean isAssignmentOrSpecificationList()
		{
		return ( listChar_ == Constants.WORD_ASSIGNMENT_LIST_SYMBOL ||
				listChar_ == Constants.WORD_SPECIFICATION_LIST_SYMBOL );
		}

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

	private byte removeItemFromList( Item removeItem )
		{
		if( removeItem == null )
			return startError( 1, null, "The given remove item is undefined" );

		if( removeItem.myList() != this )
			return startError( 1, null, "The given remove item doesn't belong to my list" );

		// First item in list
		if( removeItem.previousItem == null )
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
					return startError( 1, null, "The given remove item has an unknown status character" );
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

		// Remember next item
		nextListItem_ = removeItem.nextItem;

		// Disconnect item from the list
		removeItem.previousItem = null;
		removeItem.nextItem = null;

		return Constants.RESULT_OK;
		}


	// Constructor

	protected List()
		{
		// Private constructed variables

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

	protected CollectionResultType addCollectionResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		collectionResult.result = addError( 1, moduleNameString, errorString );
		return collectionResult;
		}

	protected CollectionResultType startCollectionResultError( int methodLevel, String moduleNameString, String errorString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		collectionResult.result = startError( 1, moduleNameString, errorString );
		return collectionResult;
		}

	protected FileResultType addFileResultError( int methodLevel, String moduleNameString, String errorString )
		{
		FileResultType fileResult = new FileResultType();

		fileResult.result = addError( 1, moduleNameString, errorString );
		return fileResult;
		}

	protected FileResultType startFileResultError( int methodLevel, String moduleNameString, String errorString )
		{
		FileResultType fileResult = new FileResultType();

		fileResult.result = startError( 1, moduleNameString, errorString );
		return fileResult;
		}
	protected FileResultType startFileResultSystemError( int methodLevel, String moduleNameString, String errorString )
		{
		FileResultType fileResult = new FileResultType();

		fileResult.result = startSystemError( 1, moduleNameString, errorString );
		return fileResult;
		}

	protected GeneralizationResultType startGeneralizationResultError( int methodLevel, String moduleNameString, String errorString )
		{
		GeneralizationResultType generalizationResult = new GeneralizationResultType();

		generalizationResult.result = startError( 1, moduleNameString, errorString );
		return generalizationResult;
		}

	protected GrammarResultType addGrammarResultError( int methodLevel, String moduleNameString, String errorString )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		grammarResult.result = addError( 1, moduleNameString, errorString );
		return grammarResult;
		}

	protected GrammarResultType startGrammarResultError( int methodLevel, String moduleNameString, String errorString )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		grammarResult.result = startError( 1, moduleNameString, errorString );
		return grammarResult;
		}

	protected JustificationResultType addJustificationResultError( int methodLevel, String moduleNameString, String errorString )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		justificationResult.result = addError( 1, moduleNameString, errorString );
		return justificationResult;
		}

	protected JustificationResultType startJustificationResultError( int methodLevel, String moduleNameString, String errorString )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		justificationResult.result = startError( 1, moduleNameString, errorString );
		return justificationResult;
		}

	protected ReadResultType addReadResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ReadResultType readResult = new ReadResultType();

		readResult.result = addError( 1, moduleNameString, errorString );
		return readResult;
		}

	protected ReadResultType startReadResultError( int methodLevel, String moduleNameString, String errorString )
		{
		ReadResultType readResult = new ReadResultType();

		readResult.result = startError( 1, moduleNameString, errorString );
		return readResult;
		}

	protected byte addError( int methodLevel, String moduleNameString, String errorString )
		{
		Presentation.displayError( listChar_, ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), ( myWordItem_ == null || myWordItem_.isAdminWord() ? null : myWordItem_.anyWordTypeString() ), ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}

	protected byte startError( int methodLevel, String moduleNameString, String errorString )
		{
		addError( ( methodLevel + 1 ), moduleNameString, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;
		return Constants.RESULT_ERROR;
		}

	protected byte startSystemError( int methodLevel, String moduleNameString, String errorString )
		{
		addError( ( methodLevel + 1 ), moduleNameString, errorString );

		CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;

		return Constants.RESULT_SYSTEM_ERROR;
		}

	protected SelectionResultType addSelectionResultError( int methodLevel, String moduleNameString, String errorString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		selectionResult.result = addError( 1, moduleNameString, errorString );
		return selectionResult;
		}

	protected SelectionResultType startSelectionResultError( int methodLevel, String moduleNameString, String errorString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		selectionResult.result = startError( 1, moduleNameString, errorString );
		return selectionResult;
		}

	protected SelectionResultType startSystemSelectionResultError( int methodLevel, String moduleNameString, String errorString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		selectionResult.result = startSystemError( 1, moduleNameString, errorString );
		return selectionResult;
		}

	protected SpecificationResultType addSpecificationResultError( int methodLevel, String moduleNameString, String errorString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		specificationResult.result = addError( 1, moduleNameString, errorString );
		return specificationResult;
		}

	protected SpecificationResultType startSpecificationResultError( int methodLevel, String moduleNameString, String errorString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		specificationResult.result = startError( 1, moduleNameString, errorString );
		return specificationResult;
		}

	protected StringResultType addStringResultError( int methodLevel, String moduleNameString, String errorString )
		{
		StringResultType stringResult = new StringResultType();

		stringResult.result = addError( 1, moduleNameString, errorString );
		return stringResult;
		}

	protected StringResultType startStringResultError( int methodLevel, String moduleNameString, String errorString )
		{
		StringResultType stringResult = new StringResultType();

		stringResult.result = startError( 1, moduleNameString, errorString );
		return stringResult;
		}

	protected WordResultType addWordResultError( int methodLevel, String moduleNameString, String errorString )
		{
		WordResultType wordResult = new WordResultType();

		wordResult.result = addError( 1, moduleNameString, errorString );
		return wordResult;
		}

	protected WordResultType startWordResultError( int methodLevel, String moduleNameString, String errorString )
		{
		WordResultType wordResult = new WordResultType();

		wordResult.result = startError( 1, moduleNameString, errorString );
		return wordResult;
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return false;
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
				startSystemError( 1, null, "I failed to create my list cleanup module" );
			}
		else
			startSystemError( 1, null, "The given my word is undefined" );
		}

	protected void deleteTemporaryList()
		{
		Item searchItem = deletedList_;

		if( replacedList_ != null )
			{
			// Move replaced list to deleted list
			if( searchItem == null )
				deletedList_ = replacedList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem.tailOfList();
				searchItem.nextItem = replacedList_;
				}

			replacedList_ = null;
			}

		if( archivedList_ != null )
			{
			// Move archived list to deleted list
			if( searchItem == null )
				deletedList_ = archivedList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem.tailOfList();
				searchItem.nextItem = archivedList_;
				}

			archivedList_ = null;
			}

		if( inactiveList_ != null )
			{
			// Move inactive list to deleted list
			if( searchItem == null )
				deletedList_ = inactiveList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem.tailOfList();
				searchItem.nextItem = inactiveList_;
				}

			inactiveList_ = null;
			}

		if( activeList_ != null )
			{
			// Move active list to deleted list
			if( searchItem == null )
				deletedList_ = activeList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem.tailOfList();
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

	protected char listChar()
		{
		return listChar_;
		}

	protected byte addItemToList( char statusChar, Item newItem )
		{
		int newCreationSentenceNr;
		Item searchItem;
		Item previousSearchItem = null;

		if( newItem == null )
			return startError( 1, null, "The given new item is undefined" );

		if( newItem.myList() != this )
			return startError( 1, null, "The given new item doesn't belong to my list" );

		if( newItem.nextItem != null )
			return startError( 1, null, "The given new item seems to be a part of a list" );

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
				return startError( 1, null, "The given status character is unknown" );
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

		if( searchItem != null &&
		// Check on duplicates
		searchItem.creationSentenceNr() == newItem.creationSentenceNr() &&
		// for integrity
		searchItem.itemNr() == newItem.itemNr() )
			return startError( 1, null, "I have found an active item with the same identification" );

		newItem.previousItem = previousSearchItem;

		// First item in list
		if( previousSearchItem == null )
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
					return startError( 1, null, "The given status character is unknown" );
				}
			}
		else
			{
			if( searchItem != null )
				searchItem.previousItem = newItem;

			newItem.nextItem = previousSearchItem.nextItem;
			previousSearchItem.nextItem = newItem;
			}

		return Constants.RESULT_OK;
		}

	protected byte activateItem( Item activateItem )
		{
		if( activateItem == null )
			return startError( 1, null, "The given activate item is undefined" );

		if( activateItem.statusChar() == Constants.QUERY_ACTIVE_CHAR )
			return startError( 1, null, "The given activate item is already an active item" );

		if( removeItemFromList( activateItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to remove an item from the archive list" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, activateItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to add an item to the active list" );

		if( isAssignmentList() &&
		CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
			CommonVariables.isAssignmentChanged = true;

		return Constants.RESULT_OK;
		}

	protected byte inactivateItem( Item inactivateItem )
		{
		if( inactivateItem == null )
			return startError( 1, null, "The given inactivate item is undefined" );

		if( inactivateItem.statusChar() == Constants.QUERY_INACTIVE_CHAR )
			return startError( 1, null, "The given inactivate item is already an inactive item" );

		if( !isAssignmentList() &&
		listChar_ != Constants.ADMIN_READ_LIST_SYMBOL )
			return startError( 1, null, "Only assignments, Guide by Grammar items and read items can be inactived" );

		if( removeItemFromList( inactivateItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to remove an item from the archive list" );

		if( addItemToList( Constants.QUERY_INACTIVE_CHAR, inactivateItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to add an item to the inactive list" );

		if( isAssignmentList() &&
		CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
			CommonVariables.isAssignmentChanged = true;

		return Constants.RESULT_OK;
		}

	protected byte archiveItem( Item archiveItem )
		{
		if( archiveItem == null )
			return startError( 1, null, "The given archive item is undefined" );

		if( archiveItem.statusChar() == Constants.QUERY_ARCHIVED_CHAR )
			return startError( 1, null, "The given archive item is already an archived item" );

		if( !isAssignmentList() )
			return startError( 1, null, "Only assignments can be archived" );

		if( removeItemFromList( archiveItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to remove an item from a list" );

		archiveItem.previousStatusChar = archiveItem.statusChar();

		if( addItemToList( Constants.QUERY_ARCHIVED_CHAR, archiveItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to add an item to the archived list" );

		if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
			CommonVariables.isAssignmentChanged = true;

		return Constants.RESULT_OK;
		}

	protected byte replaceItem( Item replaceItem )
		{
		if( replaceItem == null )
			return startError( 1, null, "The given replace item is undefined" );

		if( replaceItem.statusChar() == Constants.QUERY_REPLACED_CHAR )
			return startError( 1, null, "The given replace item is already a replaced item" );

		if( removeItemFromList( replaceItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to remove an item from a list" );

		replaceItem.previousStatusChar = replaceItem.statusChar();

		if( addItemToList( Constants.QUERY_REPLACED_CHAR, replaceItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to add an item to the replaced list" );

		return Constants.RESULT_OK;
		}

	protected byte deleteItem( Item deleteItem )
		{
		if( removeItemFromList( deleteItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to remove an item from a list" );

		deleteItem.previousStatusChar = deleteItem.statusChar();

		if( deleteItem.statusChar() == Constants.QUERY_DELETED_CHAR )
			return startError( 1, null, "The given delete item is already a deleted item" );

		if( addItemToList( Constants.QUERY_DELETED_CHAR, deleteItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to add an item to the deleted list" );

		return Constants.RESULT_OK;
		}

	protected byte deleteActiveItemsWithCurrentSentenceNr()
		{
		Item searchItem = activeList_;

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( searchItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to delete an active item" );

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItemsInList()
		{
		Item removeItem = deletedList_;

		if( removeItem != null )
			{
			if( CommonVariables.nDeletedItems != 0 ||
			CommonVariables.removeSentenceNr != 0 ||
			CommonVariables.removeStartItemNr != 0 )
				return startError( 1, null, "There is already a range of deleted items" );

			CommonVariables.removeSentenceNr = removeItem.creationSentenceNr();
			CommonVariables.removeStartItemNr = removeItem.itemNr();

			do	{
				// Disconnect deleted list from item
				deletedList_ = deletedList_.nextItem;

				if( removeItem.checkForUsage() != Constants.RESULT_OK )
					addError( 1, null, "I failed to check an item for its usage" );

				// Disconnect item from the deleted list
				removeItem.nextItem = null;
				removeItem = deletedList_;
				CommonVariables.nDeletedItems++;
				}
			while( removeItem != null &&
			// Same sentence number
			removeItem.creationSentenceNr() == CommonVariables.removeSentenceNr &&
			// Ascending item number
			removeItem.itemNr() == CommonVariables.removeStartItemNr + CommonVariables.nDeletedItems );
			}

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

	protected void clearReplacingInfoInList()
		{
		listCleanup_.clearReplacingInfo();
		}

	protected void getHighestInUseSentenceNrInList( boolean isIncludingDeletedItems, int highestSentenceNr )
		{
		int tempSentenceNr = listCleanup_.highestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

		if( tempSentenceNr > CommonVariables.highestInUseSentenceNr )
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

	protected byte deleteSentencesInList( int lowestSentenceNr )
		{
		return listCleanup_.deleteSentences( lowestSentenceNr );
		}

	protected byte decrementSentenceNrsInList( int startSentenceNr )
		{
		return listCleanup_.decrementSentenceNrs( startSentenceNr );
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

	protected byte displayQueryResultInList( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		if( listQuery_ != null )
			return listQuery_.displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth );

		// In case the first query doesn't include admin lists,
		// the admin list query module isn't created yet
		return Constants.RESULT_OK;
		}

	protected byte itemQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, "I failed to create my list query module" );

		listQuery_.itemQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );
		return Constants.RESULT_OK;
		}

	protected byte listQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer )
		{
		boolean isListIncludedInQuery = isIncludingThisList( queryListStringBuffer );

		if( isSelectingOnFind ||
		!isListIncludedInQuery )
			{
			if( listQuery_ == null &&
			// Create supporting module
			( listQuery_ = new ListQuery( this ) ) == null )
				return startError( 1, null, "I failed to create my list query module" );

			listQuery_.listQuery( ( isSelectingOnFind && isListIncludedInQuery ), isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems );
			}

		return Constants.RESULT_OK;
		}

	protected byte wordTypeQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, "I failed to create my list query module" );

		listQuery_.wordTypeQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr );
		return Constants.RESULT_OK;
		}

	protected byte parameterQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, "I failed to create my list query module" );

		listQuery_.parameterQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter );
		return Constants.RESULT_OK;
		}

	protected byte wordQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, "I failed to create my list query module" );

		listQuery_.wordQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems );
		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, "I failed to create my list query module" );

		return listQuery_.wordReferenceQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, ( isSelectingAttachedJustifications && isAssignmentOrSpecificationList() ), ( isSelectingJustificationSpecifications && isAssignmentOrSpecificationList() ), wordReferenceNameString );
		}

	protected byte stringQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startError( 1, null, "I failed to create my list query module" );

		return listQuery_.stringQuery( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString );
		}

	protected StringResultType compareStrings( String searchString, String sourceString )
		{
		if( listQuery_ == null &&
		// Create supporting module
		( listQuery_ = new ListQuery( this ) ) == null )
			return startStringResultError( 1, null, "I failed to create my list query module" );

		return listQuery_.compareStrings( searchString, sourceString );
		}
	};

/*************************************************************************
 *	"But he rescues the poor from trouble
 *	and increases their families like flocks of sheep." (Psalm 107:41)
 *************************************************************************/
