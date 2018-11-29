/*	Class:		List
 *	Purpose:	Base class to store the items of the knowledge structure
 *	Version:	Thinknowlogy 2018r4 (New Science)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

package org.mafait.thinknowlogy;

class List
	{
	// Private constructed variables

	private int highestSentenceNrInList_ = Constants.NO_SENTENCE_NR;

	private Item activeList_ = null;
	private Item inactiveList_ = null;
	private Item archivedList_ = null;
	private Item replacedList_ = null;
	private Item deletedList_ = null;

	private Item nextListItem_ = null;

	// Private initialized variables

	private char listChar_ = Constants.QUERY_NO_LIST_CHAR;

	private WordItem myWordItem_ = null;

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

	private Item firstDeletedItem()
		{
		return deletedList_;
		}


	// Private cleanup methods

	private static int highestCurrentSentenceItemNr( int currentSentenceNr, Item searchItem )
		{
		int highestItemNr = Constants.NO_ITEM_NR;
		int tempItemNr;

		while( searchItem != null )
			{
			if( searchItem.creationSentenceNr() == currentSentenceNr &&
			( tempItemNr = searchItem.itemNr() ) > highestItemNr )
				highestItemNr = tempItemNr;

			searchItem = searchItem.nextItem;
			}

		return highestItemNr;
		}

	private static int highestFoundSentenceNrInList( int maxSentenceNr, Item searchItem )
		{
		int highestFoundSentenceNr = Constants.NO_SENTENCE_NR;
		int tempSentenceNr;

		while( searchItem != null &&
		highestFoundSentenceNr < maxSentenceNr )
			{
			tempSentenceNr = searchItem.activeSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem.inactiveSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem.archivedSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem.replacedSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			searchItem = searchItem.nextItem;
			}

		return highestFoundSentenceNr;
		}

	private byte decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset, Item searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.creationSentenceNr() == decrementSentenceNr &&
			searchItem.itemNr() >= startDecrementItemNr )
				{
				if( searchItem.itemNr() <= startDecrementItemNr )
					return startError( 1, "I found an item number equal to the given start item number" );

				if( searchItem.decrementItemNr( decrementOffset ) != Constants.RESULT_OK )
					return addError( 1, "I failed to decrement the item number of an item with a certain offset" );
				}

			searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	private byte decrementSentenceNrs( int startSentenceNr, Item searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.activeSentenceNr() >= startSentenceNr )
				{
				if( searchItem.activeSentenceNr() <= startSentenceNr )
					return startError( 1, "I found an item with an active sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementActiveSentenceNr() != Constants.RESULT_OK )
					return addError( 1, "I failed to decrement the active sentence number of an item" );
				}

			if( searchItem.inactiveSentenceNr() >= startSentenceNr )
				{
				if( searchItem.inactiveSentenceNr() <= startSentenceNr )
					return startError( 1, "I found an item with an inactive sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementInactiveSentenceNr() != Constants.RESULT_OK )
					return addError( 1, "I failed to decrement the inactive sentence number of an item" );
				}

			if( searchItem.originalSentenceNr() >= startSentenceNr )
				{
				if( searchItem.originalSentenceNr() <= startSentenceNr )
					return startError( 1, "I found an item with an original sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementOriginalSentenceNr() != Constants.RESULT_OK )
					return addError( 1, "I failed to decrement the original sentence number of an item" );
				}

			if( searchItem.creationSentenceNr() >= startSentenceNr )
				{
				if( searchItem.creationSentenceNr() <= startSentenceNr )
					return startError( 1, "I found an item with a creation sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementCreationSentenceNr() != Constants.RESULT_OK )
					return addError( 1, "I failed to decrement the creation sentence number of an item" );
				}

			if( searchItem.archivedSentenceNr() >= startSentenceNr )
				{
				if( searchItem.archivedSentenceNr() <= startSentenceNr )
					return startError( 1, "I found an item with an archived sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementArchivedSentenceNr() != Constants.RESULT_OK )
					return addError( 1, "I failed to decrement the archived sentence number of an item" );
				}

			if( searchItem.replacedSentenceNr() >= startSentenceNr )
				{
				if( searchItem.replacedSentenceNr() <= startSentenceNr )
					return startError( 1, "I found an item with a replaced sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementReplacedSentenceNr() != Constants.RESULT_OK )
					return addError( 1, "I failed to decrement the replaced sentence number of an item" );
				}

			searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	private byte removeItemFromList( Item removeItem )
		{
		if( removeItem == null )
			return startError( 1, "The given remove item is undefined" );

		if( removeItem.myList() != this )
			return startError( 1, "The given remove item doesn't belong to my list" );

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
					return startError( 1, "The given remove item has an unknown status character" );
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


	// Private database connection methods

/*
	private byte storeChangesInFutureDatabase( Item searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() &&
			searchItem.storeChangesInFutureDatabase() != Constants.RESULT_OK )
				return addError( 1, "I failed to store the changes of an item in the database" );

			searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}
*/


	// Private query methods

	private static void clearQuerySelections( Item searchItem )
		{
		while( searchItem != null )
			{
			searchItem.isSelectedByQuery = false;
			searchItem.isSelectedByJustificationQuery = false;

			searchItem = searchItem.nextItem;
			}
		}

	private static void itemQuery( boolean isSelectingOnFind, boolean isReferenceQuery, int querySentenceNr, int queryItemNr, Item queryItem )
		{
		while( queryItem != null )
			{
			if( ( isReferenceQuery &&
			queryItem.hasReferenceItemById( querySentenceNr, queryItemNr ) ) ||

			( !isReferenceQuery &&

			( ( querySentenceNr == Constants.NO_SENTENCE_NR &&
			queryItemNr == Constants.NO_SENTENCE_NR ) ||

			( querySentenceNr > Constants.NO_SENTENCE_NR &&
			queryItemNr == Constants.NO_SENTENCE_NR &&
			queryItem.hasSentenceNr( querySentenceNr ) ) ||

			( queryItem.itemNr() == queryItemNr &&

			( querySentenceNr == Constants.NO_SENTENCE_NR ||
			queryItem.creationSentenceNr() == querySentenceNr ) ) ) ) )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					GlobalVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void listQuery( boolean isSelectingOnFind, Item queryItem )
		{
		while( queryItem != null )
			{
			if( isSelectingOnFind )
				{
				if( !queryItem.isSelectedByQuery )
					{
					GlobalVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void parameterQuery( boolean isSelectingOnFind, int queryParameter, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.hasParameter( queryParameter ) )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					GlobalVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void wordQuery( boolean isSelectingOnFind, Item queryItem )
		{
		while( queryItem != null )
			{
			if( isSelectingOnFind )
				{
				if( !queryItem.isSelectedByQuery )
					{
					GlobalVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private static void wordTypeQuery( boolean isSelectingOnFind, short queryWordTypeNr, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.hasWordType( queryWordTypeNr ) )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					GlobalVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}
		}

	private byte displayQueryResult( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth, Item queryItem )
		{
		while( queryItem != null )
			{
			if( queryItem.isSelectedByQuery ||
			queryItem.isSelectedByJustificationQuery )
				{
				if( isOnlyDisplayingWords )
					queryItem.displayWords( isReturnQueryToPosition, queryWordTypeNr );
				else
					{
					if( isOnlyDisplayingWordReferences )
						queryItem.displayWordReferences( isReturnQueryToPosition );
					else
						{
						if( isOnlyDisplayingStrings )
							queryItem.displayString( isReturnQueryToPosition );
						else
							{
							if( InputOutput.writeText( true, promptTypeNr, queryWidth, queryItem.itemToStringBuffer( queryWordTypeNr ) ) != Constants.RESULT_OK )
								return addError( 1, "I failed to display the info of an active item" );
							}
						}
					}
				}

			queryItem = queryItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	private byte stringQuery( boolean isSelectingOnFind, String wordString, Item queryItem )
		{
		boolean hasFoundString;
		String itemString;
		BoolResultType boolResult;

		while( queryItem != null )
			{
			hasFoundString = false;

			if( ( itemString = queryItem.itemString() ) != null )
				{
				if( ( boolResult = compareStrings( wordString, itemString ) ).result != Constants.RESULT_OK )
					return addError( 1, "I failed to compare two strings" );

				hasFoundString = boolResult.booleanValue;
				}

			if( hasFoundString )
				{
				if( isSelectingOnFind &&
				!queryItem.isSelectedByQuery )
					{
					GlobalVariables.hasFoundQuery = true;
					queryItem.isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	private byte wordReferenceQuery( boolean isSelectingOnFind, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString, Item queryItem )
		{
		BoolResultType boolResult;

		while( queryItem != null )
			{
			if( ( boolResult = queryItem.findMatchingWordReferenceString( wordReferenceNameString ) ).result != Constants.RESULT_OK )
				return addError( 1, "I failed to check the word references" );

			// Matching string
			if( boolResult.booleanValue )
				{
				if( queryItem.isSelectedByQuery )
					{
					if( isSelectingAttachedJustifications )
						queryItem.selectingAttachedJustifications( isSelectingJustificationSpecifications );
					}
				else
					{
					if( isSelectingOnFind )
						{
						GlobalVariables.hasFoundQuery = true;
						queryItem.isSelectedByQuery = true;
						}
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem.isSelectedByQuery )
					queryItem.isSelectedByQuery = false;
				}

			queryItem = queryItem.nextItem;
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected List()
		{
		}


	// Protected error methods

	protected byte addError( int methodLevel, String errorString )
		{
		InputOutput.displayError( listChar_, this.getClass().getName(), this.getClass().getSuperclass().getName(), ( myWordItem_ == null || myWordItem_.isAdminWord() ? null : myWordItem_.anyWordTypeString() ), ( methodLevel + 1 ), errorString );
		return GlobalVariables.result;
		}

	protected byte startError( int methodLevel, String errorString )
		{
		addError( ( methodLevel + 1 ), errorString );

		GlobalVariables.result = Constants.RESULT_ERROR;
		return Constants.RESULT_ERROR;
		}

	protected byte startSystemError( int methodLevel, String errorString )
		{
		addError( ( methodLevel + 1 ), errorString );

		GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;

		return Constants.RESULT_SYSTEM_ERROR;
		}

	protected BoolResultType addBoolResultError( int methodLevel, String errorString )
		{
		BoolResultType interpretationResult = new BoolResultType();

		interpretationResult.result = addError( ( methodLevel + 1 ), errorString );
		return interpretationResult;
		}

	protected BoolResultType startBoolResultError( int methodLevel, String errorString )
		{
		BoolResultType boolResult = new BoolResultType();

		boolResult.result = startError( ( methodLevel + 1 ), errorString );
		return boolResult;
		}

	protected CollectionResultType addCollectionResultError( int methodLevel, String errorString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		collectionResult.result = addError( ( methodLevel + 1 ), errorString );
		return collectionResult;
		}

	protected CollectionResultType startCollectionResultError( int methodLevel, String errorString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		collectionResult.result = startError( ( methodLevel + 1 ), errorString );
		return collectionResult;
		}

	protected CollectionResultType startCollectionResultSystemError( int methodLevel, String errorString )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		collectionResult.result = startSystemError( ( methodLevel + 1 ), errorString );
		return collectionResult;
		}

	protected CreateAndAssignResultType addCreateAndAssignResultError( int methodLevel, String errorString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		createAndAssignResult.result = addError( ( methodLevel + 1 ), errorString );
		return createAndAssignResult;
		}

	protected CreateAndAssignResultType startCreateAndAssignResultError( int methodLevel, String errorString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		createAndAssignResult.result = startError( ( methodLevel + 1 ), errorString );
		return createAndAssignResult;
		}

	protected DuplicateResultType addDuplicateResultError( int methodLevel, String errorString )
		{
		DuplicateResultType duplicateResult = new DuplicateResultType();

		duplicateResult.result = addError( ( methodLevel + 1 ), errorString );
		return duplicateResult;
		}

	protected DuplicateResultType startDuplicateResultError( int methodLevel, String errorString )
		{
		DuplicateResultType duplicateResult = new DuplicateResultType();

		duplicateResult.result = startError( ( methodLevel + 1 ), errorString );
		return duplicateResult;
		}

	protected FileResultType addFileResultError( int methodLevel, String errorString )
		{
		FileResultType fileResult = new FileResultType();

		fileResult.result = addError( ( methodLevel + 1 ), errorString );
		return fileResult;
		}

	protected FileResultType startFileResultError( int methodLevel, String errorString )
		{
		FileResultType fileResult = new FileResultType();

		fileResult.result = startError( ( methodLevel + 1 ), errorString );
		return fileResult;
		}

	protected GrammarResultType addGrammarResultError( int methodLevel, String errorString )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		grammarResult.result = addError( ( methodLevel + 1 ), errorString );
		return grammarResult;
		}

	protected GrammarResultType startGrammarResultError( int methodLevel, String errorString )
		{
		GrammarResultType grammarResult = new GrammarResultType();

		grammarResult.result = startError( ( methodLevel + 1 ), errorString );
		return grammarResult;
		}

	protected JustificationResultType addJustificationResultError( int methodLevel, String errorString )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		justificationResult.result = addError( ( methodLevel + 1 ), errorString );
		return justificationResult;
		}

	protected JustificationResultType startJustificationResultError( int methodLevel, String errorString )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		justificationResult.result = startError( ( methodLevel + 1 ), errorString );
		return justificationResult;
		}

	protected SelectionResultType addSelectionResultError( int methodLevel, String errorString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		selectionResult.result = addError( ( methodLevel + 1 ), errorString );
		return selectionResult;
		}

	protected SelectionResultType startSelectionResultSystemError( int methodLevel, String errorString )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		selectionResult.result = startSystemError( ( methodLevel + 1 ), errorString );
		return selectionResult;
		}

	protected ShortResultType startShortResultError( int methodLevel, String errorString )
		{
		ShortResultType shortResult = new ShortResultType();

		shortResult.result = startError( ( methodLevel + 1 ), errorString );
		return shortResult;
		}

	protected WordEndingResultType startWordEndingResultError( int methodLevel, String errorString )
		{
		WordEndingResultType wordEndingResult = new WordEndingResultType();

		wordEndingResult.result = startError( ( methodLevel + 1 ), errorString );
		return wordEndingResult;
		}

	protected WordResultType addWordResultError( int methodLevel, String errorString )
		{
		WordResultType wordResult = new WordResultType();

		wordResult.result = addError( ( methodLevel + 1 ), errorString );
		return wordResult;
		}

	protected WordResultType startWordResultError( int methodLevel, String errorString )
		{
		WordResultType wordResult = new WordResultType();

		wordResult.result = startError( ( methodLevel + 1 ), errorString );
		return wordResult;
		}

	protected WordTypeResultType addWordTypeResultError( int methodLevel, String errorString )
		{
		WordTypeResultType wordTypeResult = new WordTypeResultType();

		wordTypeResult.result = addError( ( methodLevel + 1 ), errorString );
		return wordTypeResult;
		}

	protected WordTypeResultType startWordTypeResultError( int methodLevel, String errorString )
		{
		WordTypeResultType wordTypeResult = new WordTypeResultType();

		wordTypeResult.result = startError( ( methodLevel + 1 ), errorString );
		return wordTypeResult;
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return false;
		}


	// Protected common methods

	protected void initializeListVariables( char listChar, String classNameString, WordItem myWordItem )
		{
		// Private initialized variables

		listChar_ = listChar;

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == null )
			startSystemError( 1, "The given my word item is undefined" );
		}

	protected void deleteTemporaryList()
		{
		Item searchItem = deletedList_;

		// ReadList may have inactive items
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

		// Temporary lists have active items
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

	protected boolean hasActiveItems()
		{
		return ( activeList_ != null );
		}

	protected boolean isAdminList()
		{
		return Character.isUpperCase( listChar_ );
		}

	protected boolean isAssignmentList()
		{
		return ( listChar_ == Constants.WORD_ASSIGNMENT_LIST_SYMBOL );
		}

	protected int highestSentenceNrInList()
		{
		return highestSentenceNrInList_;
		}

	protected char listChar()
		{
		return listChar_;
		}

	protected byte addItemToList( char statusChar, Item newItem )
		{
		int creationSentenceNr;
		Item previousSearchItem = null;
		Item searchItem;

		if( newItem == null )
			return startError( 1, "The given new item is undefined" );

		if( newItem.myList() != this )
			return startError( 1, "The given new item doesn't belong to my list" );

		if( newItem.nextItem != null )
			return startError( 1, "The given new item seems to be a part of a list" );

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
				newItem.setDeletedStatus();
				searchItem = deletedList_;
				break;

			default:
				return startError( 1, "The given status character is unknown" );
			}

		creationSentenceNr = newItem.creationSentenceNr();

		// Sort item in list
		if( statusChar == Constants.QUERY_DELETED_CHAR )
			{
			while( searchItem != null &&

			// 1) Descending creationSentenceNr
			( searchItem.creationSentenceNr() > creationSentenceNr ||

			// 2) Ascending itemNr
			( searchItem.creationSentenceNr() == creationSentenceNr &&
			searchItem.itemNr() < newItem.itemNr() ) ) )
				{
				previousSearchItem = searchItem;
				searchItem = searchItem.nextItem;
				}
			}
		else
			{
			if( creationSentenceNr > highestSentenceNrInList_ )
				{
				highestSentenceNrInList_ = creationSentenceNr;

				if( myWordItem_ != null )
					myWordItem_.setHighestSentenceNr( creationSentenceNr );
				}

			while( searchItem != null &&
			!newItem.isSorted( searchItem ) )
				{
				previousSearchItem = searchItem;
				searchItem = searchItem.nextItem;
				}
			}

		if( searchItem != null &&
		// Check on duplicates
		searchItem.creationSentenceNr() == creationSentenceNr &&
		// for integrity
		searchItem.itemNr() == newItem.itemNr() )
			return startError( 1, "I found another item with the same identification" );

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
					return startError( 1, "The given status character is unknown" );
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
			return startError( 1, "The given activate item is undefined" );

		if( activateItem.statusChar() == Constants.QUERY_ACTIVE_CHAR )
			return startError( 1, "The given activate item is already an active item" );

		if( removeItemFromList( activateItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to remove an item from the archive list" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, activateItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an item to the active list" );

		if( isAssignmentList() &&
		GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
			GlobalVariables.isAssignmentChanged = true;

		return Constants.RESULT_OK;
		}

	protected byte inactivateItem( Item inactivateItem )
		{
		if( inactivateItem == null )
			return startError( 1, "The given inactivate item is undefined" );

		if( inactivateItem.statusChar() == Constants.QUERY_INACTIVE_CHAR )
			return startError( 1, "The given inactivate item is already an inactive item" );

		if( listChar_ != Constants.ADMIN_READ_LIST_SYMBOL &&
		listChar_ != Constants.WORD_ASSIGNMENT_LIST_SYMBOL )
			return startError( 1, "Only assignments and read items can be inactived" );

		if( removeItemFromList( inactivateItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to remove an item from the archive list" );

		if( addItemToList( Constants.QUERY_INACTIVE_CHAR, inactivateItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an item to the inactive list" );

		if( isAssignmentList() &&
		GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
			GlobalVariables.isAssignmentChanged = true;

		return Constants.RESULT_OK;
		}

	protected byte archiveItem( Item archiveItem )
		{
		if( archiveItem == null )
			return startError( 1, "The given archive item is undefined" );

		if( archiveItem.statusChar() == Constants.QUERY_ARCHIVED_CHAR )
			return startError( 1, "The given archive item is already an archived item" );

		if( !isAssignmentList() )
			return startError( 1, "Only assignments can be archived" );

		if( removeItemFromList( archiveItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to remove an item from a list" );

		archiveItem.previousStatusChar = archiveItem.statusChar();

		if( addItemToList( Constants.QUERY_ARCHIVED_CHAR, archiveItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an item to the archived list" );

		if( GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
			GlobalVariables.isAssignmentChanged = true;

		return Constants.RESULT_OK;
		}

	protected byte replaceItem( Item replaceItem )
		{
		if( replaceItem == null )
			return startError( 1, "The given replace item is undefined" );

		if( replaceItem.statusChar() == Constants.QUERY_REPLACED_CHAR )
			return startError( 1, "The given replace item is already a replaced item" );

		if( removeItemFromList( replaceItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to remove an item from a list" );

		replaceItem.previousStatusChar = replaceItem.statusChar();

		if( addItemToList( Constants.QUERY_REPLACED_CHAR, replaceItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an item to the replaced list" );

		return Constants.RESULT_OK;
		}

	protected byte deleteItem( Item deleteItem )
		{
		if( removeItemFromList( deleteItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to remove an item from a list" );

		deleteItem.previousStatusChar = deleteItem.statusChar();

		if( deleteItem.statusChar() == Constants.QUERY_DELETED_CHAR )
			return startError( 1, "The given delete item is already a deleted item" );

		if( addItemToList( Constants.QUERY_DELETED_CHAR, deleteItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an item to the deleted list" );

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItemsInList()
		{
		int nDeletedItems = 0;
		int removeSentenceNr;
		int removeStartItemNr;
		Item removeItem = deletedList_;

		if( removeItem != null )
			{
			if( GlobalVariables.nDeletedItems != 0 ||
			GlobalVariables.removeSentenceNr > Constants.NO_SENTENCE_NR ||
			GlobalVariables.removeStartItemNr > Constants.NO_ITEM_NR )
				return startError( 1, "There is already a range of deleted items" );

			removeSentenceNr = removeItem.creationSentenceNr();
			removeStartItemNr = removeItem.itemNr();

			do	{
				// Disconnect deleted list from item
				deletedList_ = deletedList_.nextItem;

				removeItem.checkForUsage();

				// Disconnect item from the deleted list
				removeItem.nextItem = null;
				removeItem = deletedList_;
				nDeletedItems++;
				}
			while( removeItem != null &&
			// Same sentence number
			removeItem.creationSentenceNr() == removeSentenceNr &&
			// Ascending item number
			removeItem.itemNr() == removeStartItemNr + nDeletedItems );

			GlobalVariables.nDeletedItems = nDeletedItems;
			GlobalVariables.removeSentenceNr = removeSentenceNr;
			GlobalVariables.removeStartItemNr = removeStartItemNr;
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

	protected Item nextListItem()
		{
		return nextListItem_;
		}

	protected WordItem myWordItem()
		{
		return myWordItem_;
		}

	protected BoolResultType compareStrings( String searchString, String sourceString )
		{
		boolean hasFoundMatchingString = true;
		boolean isStop;
		int searchStringPosition = 0;
		int sourceStringPosition = 0;
		BoolResultType boolResult = new BoolResultType();

		if( searchString == null )
			return startBoolResultError( 1, "The given search string is undefined" );

		if( sourceString == null )
			return startBoolResultError( 1, "The given source string is undefined" );

		if( searchString == sourceString )
			return startBoolResultError( 1, "The given strings are the same string" );

		while( hasFoundMatchingString &&
		searchStringPosition < searchString.length() &&
		sourceStringPosition < sourceString.length() )
			{
			if( searchString.charAt( searchStringPosition ) == Constants.SYMBOL_QUESTION_MARK ||
			searchString.charAt( searchStringPosition ) == sourceString.charAt( sourceStringPosition ) )
				{
				searchStringPosition++;
				sourceStringPosition++;
				}
			else
				{
				if( searchString.charAt( searchStringPosition ) == Constants.SYMBOL_ASTERISK )
					{
					if( ++searchStringPosition < searchString.length() )
						{
						isStop = false;

						while( !isStop &&
						sourceStringPosition < sourceString.length() )
							{
							if( searchString.charAt( searchStringPosition ) == sourceString.charAt( sourceStringPosition ) )
								{
								// Check remaining strings
								if( ( boolResult = compareStrings( searchString.substring( searchStringPosition ), sourceString.substring( sourceStringPosition ) ) ).result != Constants.RESULT_OK )
									return addBoolResultError( 1, "I failed to compare the remaining strings" );

								if( hasFoundMatchingString )
									{
									isStop = true;
									searchStringPosition++;
									}
								else
									// Reset indicator
									hasFoundMatchingString = true;

								sourceStringPosition++;
								}
							else
								// Skip source characters if not equal
								sourceStringPosition++;
							}
						}
					else
						// Empty source string after asterisk
						sourceStringPosition = sourceString.length();
					}
				else
					hasFoundMatchingString = false;
				}
			}

		if( hasFoundMatchingString &&
		sourceStringPosition == sourceString.length() )
			{
			// Check search string for extra asterisks
			while( searchStringPosition < searchString.length() &&
			searchString.charAt( searchStringPosition ) == Constants.SYMBOL_ASTERISK )
				// Skip extra asterisks
				searchStringPosition++;
			}

		if( searchStringPosition < searchString.length() ||
		sourceStringPosition < sourceString.length() )
			hasFoundMatchingString = false;

		boolResult.booleanValue = hasFoundMatchingString;
		return boolResult;
		}


	// Protected cleanup methods

	protected int highestCurrentSentenceItemNrInList( int currentSentenceNr, int globalHighestItemNr )
		{
		int localHighestItemNr = Constants.NO_ITEM_NR;
		int tempItemNr;
		Item searchItem;

		if( ( searchItem = firstActiveItem() ) != null )
			localHighestItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem );

		if( ( searchItem = firstInactiveItem() ) != null &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( ( searchItem = firstArchivedItem() ) != null &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( ( searchItem = firstReplacedItem() ) != null &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( ( searchItem = firstDeletedItem() ) != null &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( localHighestItemNr >= globalHighestItemNr )
			return localHighestItemNr;

		return globalHighestItemNr;
		}

	protected int highestFoundSentenceNrInList( boolean isIncludingDeletedItems, int globalHighestFoundSentenceNr, int maxSentenceNr )
		{
		int localHighestFoundSentenceNr = Constants.NO_SENTENCE_NR;
		int tempSentenceNr;
		Item searchItem;

		if( ( searchItem = firstActiveItem() ) != null )
			localHighestFoundSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem );

		if( ( searchItem = firstInactiveItem() ) != null &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( ( searchItem = firstArchivedItem() ) != null &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( ( searchItem = firstReplacedItem() ) != null &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( isIncludingDeletedItems &&
		( searchItem = firstDeletedItem() ) != null &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( localHighestFoundSentenceNr >= globalHighestFoundSentenceNr )
			return localHighestFoundSentenceNr;

		return globalHighestFoundSentenceNr;
		}

	protected byte decrementItemNrRangeInList( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		Item searchItem;

		if( decrementSentenceNr < Constants.NO_SENTENCE_NR )
			return startError( 1, "The given decrement sentence number is undefined" );

		if( startDecrementItemNr <= Constants.NO_ITEM_NR )
			return startError( 1, "The given start item number is undefined" );

		if( decrementOffset <= 0 )
			return startError( 1, "The given decrement offset is undefined" );

		if( ( searchItem = firstActiveItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstInactiveItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstArchivedItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstReplacedItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		// Clear error to be able to restart after error
		return Constants.RESULT_OK;
		}

	protected byte decrementSentenceNrsInList( int startSentenceNr )
		{
		Item searchItem;

		if( startSentenceNr <= Constants.NO_SENTENCE_NR )
			return startError( 1, "The given start sentence number is undefined" );

		if( highestSentenceNrInList_ == startSentenceNr )
			highestSentenceNrInList_--;

		if( ( searchItem = firstActiveItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstInactiveItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstArchivedItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstReplacedItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		return Constants.RESULT_OK;
		}

	protected byte deleteSentencesInList( int lowestSentenceNr )
		{
		boolean isResultOK = ( GlobalVariables.result == Constants.RESULT_OK );
		Item searchItem = firstActiveItem();

		if( lowestSentenceNr <= Constants.NO_SENTENCE_NR )
			return startError( 1, "The given lowest sentence number is undefined" );

		while( searchItem != null )
			{
			if( searchItem.activeSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete an active item" );

					searchItem = nextListItem_;
					}
				else
					{
					if( searchItem.wasInactiveBefore() )
						{
						if( inactivateItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to inactivate an active item" );

						searchItem = nextListItem_;
						}
					else
						searchItem = searchItem.nextItem;
					}
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.inactiveSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete an inactive item" );
					}
				else
					{
					if( activateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to activate an inactive item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.archivedSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete an archived item" );
					}
				else
					{
					searchItem.clearArchivedSentenceNr();

					if( searchItem.wasActiveBefore() )
						{
						if( activateItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to activate an archived item" );
						}
					else
						{
						if( !searchItem.wasInactiveBefore() )
							return startError( 1, "I don't know what to do with an archived item that wasn't active before, nor inactive" );

						if( inactivateItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to inactivate an archived item" );
						}
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.replacedSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete a replaced item" );
					}
				else
					{
					if( searchItem.wasActiveBefore() )
						{
						if( activateItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to activate a replaced item" );
						}
					else
						{
						if( searchItem.wasInactiveBefore() )
							{
							if( inactivateItem( searchItem ) != Constants.RESULT_OK )
								return addError( 1, "I failed to inactivate a replaced item" );
							}
						else
							{
							if( !searchItem.wasArchivedBefore() )
								return startError( 1, "I don't know what to do with a replaced item that wasn't inactive before, nor archived" );

							if( archiveItem( searchItem ) != Constants.RESULT_OK )
								return addError( 1, "I failed to archive a replaced item" );
							}
						}
					}

				searchItem = nextListItem_;
				}
			else
				{
				if( !isResultOK &&
				searchItem.replacedSentenceNr() == lowestSentenceNr - 1 )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete a replaced item" );

					searchItem = nextListItem_;
					}
				else
					searchItem = searchItem.nextItem;
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte redoCurrentSentenceInList()
		{
		Item searchItem = firstActiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentInactiveSentenceNr() )
				{
				if( inactivateItem( searchItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to inactivate an active item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem.hasCurrentArchivedSentenceNr() )
					{
					if( archiveItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to archive an active item" );

					searchItem = nextListItem_;
					}
				else
					{
					if( searchItem.hasCurrentReplacedSentenceNr() )
						{
						if( replaceItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to replace an active item" );

						searchItem = nextListItem_;
						}
					else
						searchItem = searchItem.nextItem;
					}
				}
			}

		searchItem = firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( activateItem( searchItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to activate an inactive item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem.hasCurrentArchivedSentenceNr() )
					{
					if( archiveItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to archive an inactive item" );

					searchItem = nextListItem_;
					}
				else
					searchItem = searchItem.nextItem;
				}
			}

		searchItem = firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( activateItem( searchItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to activate an archived item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem.hasCurrentInactiveSentenceNr() )
					{
					if( inactivateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to inactivate an archived item" );

					searchItem = nextListItem_;
					}
				else
					searchItem = searchItem.nextItem;
				}
			}

		searchItem = firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( activateItem( searchItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to activate a replaced item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem.hasCurrentInactiveSentenceNr() )
					{
					if( inactivateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to inactivate a replaced item" );

					searchItem = nextListItem_;
					}
				else
					{
					if( searchItem.hasCurrentArchivedSentenceNr() )
						{
						if( archiveItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to archive a replaced item" );

						searchItem = nextListItem_;
						}
					else
						searchItem = searchItem.nextItem;
					}
				}
			}

		searchItem = firstDeletedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( activateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to activate a deleted item" );
					}
				else
					{
					if( searchItem.wasInactiveBefore() )
						{
						if( inactivateItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to inactivate a deleted item" );
						}
					else
						{
						if( !searchItem.wasArchivedBefore() )
							return startError( 1, "I don't know what to do with a deleted item that wasn't inactive before, nor archived" );

						if( archiveItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to inactivate a deleted item" );
						}
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	protected byte undoCurrentSentenceInList()
		{
		Item searchItem = firstActiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( searchItem.hasCurrentCreationSentenceNr() )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete an active item" );
					}
				else
					{
					if( !searchItem.hasInactiveSentenceNr() )
						return startError( 1, "I don't know what to do with an active item that isn't current, and wasn't inactive" );

					if( inactivateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to inactivate an active item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentInactiveSentenceNr() )
				{
				if( searchItem.hasCurrentCreationSentenceNr() )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete an inactive item" );
					}
				else
					{
					if( !searchItem.hasActiveSentenceNr() )
						return startError( 1, "I don't know what to do with this inactive item isn't current, and wasn't active" );

					if( activateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to activate an inactive item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentArchivedSentenceNr() )
				{
				if( searchItem.hasCurrentCreationSentenceNr() )
					{
					if( deleteItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete an archived item" );
					}
				else
					{
					if( searchItem.wasActiveBefore() )
						{
						if( activateItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to activate an archived item" );
						}
					else
						{
						if( !searchItem.wasInactiveBefore() )
							return startError( 1, "I don't know what to do with this archived item that wasn't inactive before, nor archived" );

						if( inactivateItem( searchItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to inactivate an archived item" );
						}
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentReplacedSentenceNr() )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( activateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to activate a replaced item" );
					}
				else
					{
					if( !searchItem.wasInactiveBefore() )
						return startError( 1, "I don't know what to do with this replaced item that wasn't active before, nor inactive" );

					if( inactivateItem( searchItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to inactivate a replaced item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}


	// Protected database connection methods
/*
	protected byte storeChangesInFutureDatabaseInList()
		{
		Item searchItem;

		if( ( searchItem = firstActiveItem() ) != null )
			storeChangesInFutureDatabase( searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstInactiveItem() ) != null )
			storeChangesInFutureDatabase( searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstArchivedItem() ) != null )
			storeChangesInFutureDatabase( searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstReplacedItem() ) != null )
			storeChangesInFutureDatabase( searchItem );

		return GlobalVariables.result;
		}
*/

	// Protected query methods

	protected void clearQuerySelectionsInList()
		{
		Item searchItem;

		if( ( searchItem = firstActiveItem() ) != null )
			clearQuerySelections( searchItem );

		if( ( searchItem = firstInactiveItem() ) != null )
			clearQuerySelections( searchItem );

		if( ( searchItem = firstArchivedItem() ) != null )
			clearQuerySelections( searchItem );

		if( ( searchItem = firstReplacedItem() ) != null )
			clearQuerySelections( searchItem );
		}

	protected void countQueryInList()
		{
		Item searchItem = firstActiveItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				GlobalVariables.nActiveQueryItems++;

			searchItem = searchItem.nextItem;
			}

		searchItem = firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				GlobalVariables.nInactiveQueryItems++;

			searchItem = searchItem.nextItem;
			}

		searchItem = firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				GlobalVariables.nArchivedQueryItems++;

			searchItem = searchItem.nextItem;
			}

		searchItem = firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.isSelectedByQuery )
				GlobalVariables.nReplacedQueryItems++;

			searchItem = searchItem.nextItem;
			}
		}

	protected void itemQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != null )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );
		}

	protected void listQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, StringBuffer queryListStringBuffer )
		{
		boolean isSelectingOnFindAndListIncluded;
		boolean isListIncludedInQuery = isIncludingThisList( queryListStringBuffer );
		Item searchItem;

		if( isSelectingOnFind ||
		!isListIncludedInQuery )
			{
			isSelectingOnFindAndListIncluded = ( isSelectingOnFind &&
												isListIncludedInQuery );

			if( isSelectingActiveItems &&
			( searchItem = firstActiveItem() ) != null )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );

			if( isSelectingInactiveItems &&
			( searchItem = firstInactiveItem() ) != null )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );

			if( isSelectingArchivedItems &&
			( searchItem = firstArchivedItem() ) != null )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );

			if( isSelectingReplacedItems &&
			( searchItem = firstReplacedItem() ) != null )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );
			}
		}

	protected void parameterQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, int queryParameter )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != null )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );
		}

	protected void wordQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != null )
			wordQuery( isSelectingOnFind, searchItem );
		}

	protected void wordTypeQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, short queryWordTypeNr )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != null )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );
		}

	protected byte displayQueryResultInList( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		Item searchItem;

		if( ( searchItem = firstActiveItem() ) != null )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstInactiveItem() ) != null )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstArchivedItem() ) != null )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		( searchItem = firstReplacedItem() ) != null )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		return GlobalVariables.result;
		}

	protected byte stringQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, String queryString )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != null )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != null )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != null )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != null )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		return GlobalVariables.result;
		}

	protected byte wordReferenceQueryInList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		Item searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( GlobalVariables.result == Constants.RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != null )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		return GlobalVariables.result;
		}
	};

/*************************************************************************
 *	"But he rescues the poor from trouble
 *	and increases their families like flocks of sheep." (Psalm 107:41)
 *************************************************************************/
