/*	Class:			WordList
 *	Parent class:	List
 *	Purpose:		To store word items
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

class WordList extends List
	{
	// Private cleanup methods

	private static void getHighestInUseSentenceNrInWordList( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr, WordItem searchItem )
		{
		while( searchItem != null &&
		CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			{
			searchItem.getHighestInUseSentenceNrInWord( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );
			searchItem = searchItem.nextWordItem();
			}
		}

	private static void setCurrentItemNrInWordList( WordItem searchItem )
		{
		while( searchItem != null )
			{
			searchItem.setCurrentItemNrInWord();
			searchItem = searchItem.nextWordItem();
			}
		}

	private byte deleteSentencesInWordList( int lowestSentenceNr, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.deleteSentencesInWord( lowestSentenceNr ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to delete sentences in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte removeFirstRangeOfDeletedItemsInWordList( WordItem searchItem )
		{
		while( searchItem != null &&
		CommonVariables.nDeletedItems == 0 )
			{
			if( searchItem.removeFirstRangeOfDeletedItemsInWord() == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to remove the first deleted items in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte decrementSentenceNrsInWordList( int startSentenceNr, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.decrementSentenceNrsInWord( startSentenceNr ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to decrement the sentence numbers from the current sentence number in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte decrementItemNrRangeInWordList( int decrementSentenceNr, int decrementItemNr, int decrementOffset, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.decrementItemNrRangeInWord( decrementSentenceNr, decrementItemNr, decrementOffset ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to decrement item numbers in a word" );
			}

		return Constants.RESULT_OK;
		}


	// Private query methods

	private static void countQueryInWordList( WordItem searchItem )
		{
		while( searchItem != null )
			{
			searchItem.countQuery();
			searchItem = searchItem.nextWordItem();
			}
		}

	private static void clearQuerySelectionsInWordList( WordItem searchItem )
		{
		while( searchItem != null )
			{
			searchItem.clearQuerySelections();
			searchItem = searchItem.nextWordItem();
			}
		}

	private byte wordQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordNameString, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.wordQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to query a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte itemQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.itemQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to query item numbers in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte listQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.listQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to query lists in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte wordTypeQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.wordTypeQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to query word types in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte parameterQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( isSelectingOnFind &&
			searchItem.hasFoundParameter( queryParameter ) )
				searchItem.isSelectedByQuery = true;

			if( searchItem.parameterQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to query parameters in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte wordReferenceQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.wordReferenceQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to query word references in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte stringQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.stringQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to query strings in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte showQueryResultInWordList( boolean isOnlyShowingWords, boolean isOnlyShowingWordReferences, boolean isOnlyShowingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.showQueryResultInWord( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, "I failed to show the query result in a word" );
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


	// Constructor / deconstructor

	protected WordList( WordItem myWordItem )
		{
		initializeListVariables( Constants.ADMIN_WORD_LIST_SYMBOL, myWordItem );
		}


	// Protected cleanup methods

	protected void getHighestInUseSentenceNrInWordList( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchItem );

		if( isIncludingDeletedItems &&
		( searchItem = firstDeletedWordItem() ) != null &&
		CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchItem );
		}

	protected void setCurrentItemNrInWordList()
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			setCurrentItemNrInWordList( searchItem );

		if( ( searchItem = firstDeletedWordItem() ) != null )
			setCurrentItemNrInWordList( searchItem );
		}

	protected byte decrementItemNrRangeInWordList( int decrementSentenceNr, int decrementItemNr, int decrementOffset )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchItem );

		return Constants.RESULT_OK;
		}

	protected byte decrementSentenceNrsInWordList( int startSentenceNr )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			decrementSentenceNrsInWordList( startSentenceNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			decrementSentenceNrsInWordList( startSentenceNr, searchItem );

		return Constants.RESULT_OK;
		}

	protected byte deleteSentencesInWordList( int lowestSentenceNr )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			deleteSentencesInWordList( lowestSentenceNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			deleteSentencesInWordList( lowestSentenceNr, searchItem );

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItemsInWordList()
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			removeFirstRangeOfDeletedItemsInWordList( searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			removeFirstRangeOfDeletedItemsInWordList( searchItem );

		return Constants.RESULT_OK;
		}


	// Protected database connection methods
/*
	protected byte storeChangesInFutureDatabase()
		{
		WordItem searchItem = firstActiveWordItem();

		while( searchItem != null )
			{
			// Do for all active words
			if( searchItem.storeWordItemInFutureDatabase() != Constants.RESULT_OK )
				return addError( 1, null, "I failed to store a word item in the database" );

			searchItem = searchItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}
*/

	// Protected query methods

	protected void countQueryInWordList()
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			countQueryInWordList( searchItem );

		if( ( searchItem = firstDeletedWordItem() ) != null )
			countQueryInWordList( searchItem );
		}

	protected void clearQuerySelectionsInWordList()
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			clearQuerySelectionsInWordList( searchItem );

		if( ( searchItem = firstDeletedWordItem() ) != null )
			clearQuerySelectionsInWordList( searchItem );
		}

	protected byte wordQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String wordNameString )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchItem );

		return CommonVariables.result;
		}

	protected byte itemQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		return CommonVariables.result;
		}

	protected byte listQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, StringBuffer queryListStringBuffer )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListStringBuffer, searchItem );

		return CommonVariables.result;
		}

	protected byte wordTypeQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, short queryWordTypeNr )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchItem );

		return CommonVariables.result;
		}

	protected byte parameterQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, int queryParameter )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchItem );

		return CommonVariables.result;
		}

	protected byte wordReferenceQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		return CommonVariables.result;
		}

	protected byte stringQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingDeletedItems, String queryString )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchItem );

		return CommonVariables.result;
		}

	protected byte showQueryResultInWordList( boolean isOnlyShowingWords, boolean isOnlyShowingWordReferences, boolean isOnlyShowingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		WordItem searchItem;

		if( ( searchItem = firstActiveWordItem() ) != null )
			showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != null )
			showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		return CommonVariables.result;
		}


	// Protected read word methods

	protected WordResultType createWordItem( boolean isLanguageWord, short wordParameter )
		{
		WordResultType wordResult = new WordResultType();

		if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
			{
			// Create word
			if( ( wordResult.createdWordItem = new WordItem( isLanguageWord, wordParameter, this ) ) != null )
				{
				if( addItemToList( Constants.QUERY_ACTIVE_CHAR, wordResult.createdWordItem ) != Constants.RESULT_OK )
					addError( 1, null, "I failed to add an active word item" );
				}
			else
				startError( 1, null, "I failed to create a word item" );
			}
		else
			startError( 1, null, "The current item number is undefined" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}
	};

/*************************************************************************
 *	"They share freely and give generously to those in need.
 *	Their good deeds will be remembered forever.
 *	They will have influence and honor." (Psalm 112:9)
 *************************************************************************/
