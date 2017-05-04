/*	Class:			WordList
 *	Parent class:	List
 *	Purpose:		To store word items
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

class WordList extends List
	{
	// Private cleanup methods

	private void deleteSentencesInWordList( int lowestSentenceNr, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.deleteSentencesInWord( lowestSentenceNr ) != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to delete sentences in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private void decrementSentenceNrsInWordList( int startSentenceNr, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.decrementSentenceNrsInWord( startSentenceNr ) != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to decrement the sentence numbers from the current sentence number in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private void decrementItemNrRangeInWordList( int decrementSentenceNr, int decrementItemNr, int decrementOffset, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.decrementItemNrRangeInWord( decrementSentenceNr, decrementItemNr, decrementOffset ) != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to decrement item numbers in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private static void getHighestInUseSentenceNrInWordList( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr, WordItem searchWordItem )
		{
		while( searchWordItem != null &&
		CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			{
			searchWordItem.getHighestInUseSentenceNrInWord( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private void removeFirstRangeOfDeletedItemsInWordList( WordItem searchWordItem )
		{
		while( searchWordItem != null &&
		CommonVariables.nDeletedItems == 0 )
			{
			if( searchWordItem.removeFirstRangeOfDeletedItemsInWord() != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to remove the first deleted items in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private static void setCurrentItemNrInWordList( WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			searchWordItem.setCurrentItemNrInWord();
			searchWordItem = searchWordItem.nextWordItem();
			}
		}


	// Protected database connection methods
/*
	private byte storeChangesInFutureDatabaseInWordList( WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			// Do for all words
			if( searchWordItem.storeChangesInFutureDatabaseInWord() != Constants.RESULT_OK )
				return addError( 1, "I failed to store changes of a word item in the database" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}
*/

	// Protected imperative methods

	private byte redoCurrentSentenceInWordList( WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.redoCurrentSentenceInWord() != Constants.RESULT_OK )
				return addError( 1, "I failed to redo the current sentence in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	private byte undoCurrentSentenceInWordList( WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.undoCurrentSentenceInWord() != Constants.RESULT_OK )
				return addError( 1, "I failed to undo the current sentence in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}


	// Private query methods

	private static void countQueryInWordList( WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			searchWordItem.countQuery();
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private static void clearQuerySelectionsInWordList( WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			searchWordItem.clearQuerySelections();
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private static void itemQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			searchWordItem.itemQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private static void listQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			searchWordItem.listQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private static void parameterQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( isSelectingOnFind &&
			searchWordItem.hasParameter( queryParameter ) )
				searchWordItem.isSelectedByQuery = true;

			searchWordItem.parameterQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private static void wordTypeQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			searchWordItem.wordTypeQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	private byte displayQueryResultInWordList( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.displayQueryResultInWord( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
				return addError( 1, "I failed to display the query result in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	private byte stringQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.stringQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != Constants.RESULT_OK )
				return addError( 1, "I failed to query strings in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	private byte wordQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordNameString, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.wordQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString ) != Constants.RESULT_OK )
				return addError( 1, "I failed to query a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	private byte wordReferenceQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString, WordItem searchWordItem )
		{
		while( searchWordItem != null )
			{
			if( searchWordItem.wordReferenceQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != Constants.RESULT_OK )
				return addError( 1, "I failed to query word references in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}


	// Private common methods

	private WordItem firstActiveWordItem()
		{
		return (WordItem)firstActiveItem();
		}

	private WordItem firstDeletedWordItem()
		{
		return (WordItem)firstDeletedItem();
		}


	// Constructor

	protected WordList( WordItem myWordItem )
		{
		initializeListVariables( Constants.ADMIN_WORD_LIST_SYMBOL, "WordList", myWordItem );
		}


	// Protected cleanup methods

	protected void decrementItemNrRangeInWordList( int decrementSentenceNr, int decrementItemNr, int decrementOffset )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchWordItem );
		}

	protected void decrementSentenceNrsInWordList( int startSentenceNr )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			decrementSentenceNrsInWordList( startSentenceNr, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			decrementSentenceNrsInWordList( startSentenceNr, searchWordItem );
		}

	protected void deleteSentencesInWordList( int lowestSentenceNr )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			deleteSentencesInWordList( lowestSentenceNr, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			deleteSentencesInWordList( lowestSentenceNr, searchWordItem );
		}

	protected void getHighestInUseSentenceNrInWordList( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchWordItem );

		if( isIncludingDeletedItems &&
		( searchWordItem = firstDeletedWordItem() ) != null &&
		CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchWordItem );
		}

	protected void removeFirstRangeOfDeletedItemsInWordList()
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			removeFirstRangeOfDeletedItemsInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			removeFirstRangeOfDeletedItemsInWordList( searchWordItem );
		}

	protected void setCurrentItemNrInWordList()
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			setCurrentItemNrInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			setCurrentItemNrInWordList( searchWordItem );
		}


	// Protected database connection methods
/*
	protected byte storeChangesInFutureDatabaseInWordList()
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			storeChangesInFutureDatabaseInWordList( searchWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != null )
			storeChangesInFutureDatabaseInWordList( searchWordItem );

		return CommonVariables.result;
		}
*/

	// Protected imperative methods

	protected byte redoCurrentSentenceInWordList()
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			redoCurrentSentenceInWordList( searchWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != null )
			redoCurrentSentenceInWordList( searchWordItem );

		return CommonVariables.result;
		}

	protected byte undoCurrentSentenceInWordList()
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			undoCurrentSentenceInWordList( searchWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != null )
			undoCurrentSentenceInWordList( searchWordItem );

		return CommonVariables.result;
		}


	// Protected query methods

	protected void countQueryInWordList()
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			countQueryInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			countQueryInWordList( searchWordItem );
		}

	protected void clearQuerySelectionsInWordList()
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			clearQuerySelectionsInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			clearQuerySelectionsInWordList( searchWordItem );
		}

	protected void itemQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchWordItem );
		}

	protected void listQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer, searchWordItem );
		}

	protected void parameterQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchWordItem );
		}

	protected void wordTypeQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != null )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchWordItem );
		}

	protected byte displayQueryResultInWordList( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			displayQueryResultInWordList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != null )
			displayQueryResultInWordList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchWordItem );

		return CommonVariables.result;
		}

	protected byte stringQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != null )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchWordItem );

		return CommonVariables.result;
		}

	protected byte wordQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordNameString )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != null )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchWordItem );

		return CommonVariables.result;
		}

	protected byte wordReferenceQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		WordItem searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != null )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchWordItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != null )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchWordItem );

		return CommonVariables.result;
		}


	// Protected read word methods

	protected WordResultType createWordItem( boolean isLanguageWord, short wordParameter )
		{
		WordResultType wordResult = new WordResultType();

		// Create word
		if( ( wordResult.createdWordItem = new WordItem( isLanguageWord, wordParameter, this ) ) == null )
			return startWordResultError( 1, "I failed to create a word item" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, wordResult.createdWordItem ) != Constants.RESULT_OK )
			return addWordResultError( 1, "I failed to add an active word item" );

		return wordResult;
		}
	};

/*************************************************************************
 *	"They share freely and give generously to those in need.
 *	Their good deeds will be remembered forever.
 *	They will have influence and honor." (Psalm 112:9)
 *************************************************************************/
