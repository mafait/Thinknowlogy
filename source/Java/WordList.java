/*
 *	Class:			WordList
 *	Parent class:	List
 *	Purpose:		To store word items
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
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

class WordList extends List
	{
	// Private cleanup methods

	private static void deleteRollbackInfoInWordList( WordItem searchItem )
		{
		while( searchItem != null )
			{
			searchItem.deleteRollbackInfo();
			searchItem = searchItem.nextWordItem();
			}
		}

	private static void getHighestInUseSentenceNrInWordList( boolean isIncludingDeletedItems, boolean isIncludingLanguageAssignments, boolean isIncludingTemporaryLists, int highestSentenceNr, WordItem searchItem )
		{
		while( searchItem != null &&
		CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			{
			searchItem.getHighestInUseSentenceNrInWord( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, highestSentenceNr );
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

	private static int highestSentenceNrInWordList( WordItem searchItem )
		{
		int tempSentenceNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;

		while( searchItem != null )
			{
			if( ( tempSentenceNr = searchItem.highestSentenceNr() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;

			searchItem = searchItem.nextWordItem();
			}

		return highestSentenceNr;
		}

	private byte deleteSentencesInWordList( boolean isAvailableForRollback, int lowestSentenceNr, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.deleteSentencesInWord( isAvailableForRollback, lowestSentenceNr ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to delete sentences in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte removeFirstRangeOfDeletedItemsInWordList( WordItem searchItem )
		{
		while( searchItem != null &&
		CommonVariables.nDeletedItems == 0 )
			{
			if( searchItem.removeFirstRangeOfDeletedItems() == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to remove the first deleted items in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte rollbackDeletedRedoInfoInWordList( WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.rollbackDeletedRedoInfo() == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to rollback a deleted item in a word" );
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
				return addError( 1, null, null, "I failed to decrement the sentence numbers from the current sentence number in a word" );
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
				return addError( 1, null, null, "I failed to decrement item numbers in a word" );
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

	private byte wordQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordNameString, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.wordQueryInWord( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordNameString ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to query a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte itemQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.itemQueryInWord( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to query item numbers in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte listQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, StringBuffer queryListStringBuffer, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.listQueryInWord( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to query lists in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte wordTypeQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, short queryWordTypeNr, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.wordTypeQueryInWord( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to query word types in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte parameterQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, int queryParameter, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( isSelectOnFind &&
			searchItem.hasFoundParameter( queryParameter ) )
				searchItem.isSelectedByQuery = true;

//			if( searchItem.isSelectedByQuery )
				{
				if( searchItem.parameterQueryInWord( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter ) != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to query parameters in a word" );
				}

			searchItem = searchItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	private byte wordReferenceQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordReferenceNameString, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.wordReferenceQueryInWord( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to query word references in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte stringQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String queryString, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.stringQueryInWord( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to query strings in a word" );
			}

		return Constants.RESULT_OK;
		}

	private byte showQueryResultInWordList( boolean showOnlyWords, boolean showOnlyWordReferences, boolean showOnlyStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth, WordItem searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.showQueryResultInWord( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) == Constants.RESULT_OK )
				searchItem = searchItem.nextWordItem();
			else
				return addError( 1, null, null, "I failed to show the query result in a word" );
			}

		return Constants.RESULT_OK;
		}


	// Private common methods

	private WordItem firstActiveWordItem()
		{
		return (WordItem)firstActiveItem();
		}

	private WordItem firstReplacedWordItem()
		{
		return (WordItem)firstReplacedItem();
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


	// Protected assignment methods

	protected byte createNewAssignmentLevelInWordList()
		{
		WordItem searchItem = firstActiveWordItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		searchItem != null )
			{
			searchItem.createNewAssignmentLevel();
			searchItem = searchItem.nextWordItem();
			}

		return CommonVariables.result;
		}


	// Protected cleanup methods

	protected void deleteRollbackInfoInWordList()
		{
		deleteRollbackInfoInWordList( firstActiveWordItem() );
		deleteRollbackInfoInWordList( firstReplacedWordItem() );
		}

	protected void getHighestInUseSentenceNrInWordList( boolean isIncludingDeletedItems, boolean isIncludingLanguageAssignments, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, highestSentenceNr, firstActiveWordItem() );

		if( CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			{
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, highestSentenceNr, firstReplacedWordItem() );

			if( isIncludingDeletedItems &&
			CommonVariables.highestInUseSentenceNr < highestSentenceNr )
				getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, highestSentenceNr, firstDeletedWordItem() );
			}
		}

	protected void setCurrentItemNrInWordList()
		{
		setCurrentItemNrInWordList( firstActiveWordItem() );
		setCurrentItemNrInWordList( firstReplacedWordItem() );
		setCurrentItemNrInWordList( firstDeletedWordItem() );
		}

	protected int highestSentenceNrInWordList()
		{
		int tempSentenceNr;
		int highestSentenceNr = highestSentenceNrInWordList( firstActiveWordItem() );

		if( ( tempSentenceNr = highestSentenceNrInWordList( firstReplacedWordItem() ) ) > highestSentenceNr )
			highestSentenceNr = tempSentenceNr;

		if( ( tempSentenceNr = highestSentenceNrInWordList( firstDeletedWordItem() ) ) > highestSentenceNr )
			highestSentenceNr = tempSentenceNr;

		return highestSentenceNr;
		}

	protected byte decrementItemNrRangeInWordList( int decrementSentenceNr, int decrementItemNr, int decrementOffset )
		{
		if( decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, firstActiveWordItem() ) == Constants.RESULT_OK &&
		decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte decrementSentenceNrsInWordList( int startSentenceNr )
		{
		if( decrementSentenceNrsInWordList( startSentenceNr, firstActiveWordItem() ) == Constants.RESULT_OK &&
		decrementSentenceNrsInWordList( startSentenceNr, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return decrementSentenceNrsInWordList( startSentenceNr, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte deleteSentencesInWordList( boolean isAvailableForRollback, int lowestSentenceNr )
		{
		if( deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr, firstActiveWordItem() ) == Constants.RESULT_OK &&
		deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte removeFirstRangeOfDeletedItemsInWordList()
		{
		if( removeFirstRangeOfDeletedItemsInWordList( firstActiveWordItem() ) == Constants.RESULT_OK &&
		removeFirstRangeOfDeletedItemsInWordList( firstReplacedWordItem() ) == Constants.RESULT_OK )
			return removeFirstRangeOfDeletedItemsInWordList( firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte rollbackDeletedRedoInfoInWordList()
		{
		if( rollbackDeletedRedoInfoInWordList( firstActiveWordItem() ) == Constants.RESULT_OK &&
		rollbackDeletedRedoInfoInWordList( firstReplacedWordItem() ) == Constants.RESULT_OK )
			return rollbackDeletedRedoInfoInWordList( firstDeletedWordItem() );

		return CommonVariables.result;
		}


	// Protected database connection methods
/*
	protected byte storeChangesInFutureDatabase()
		{
		// Not fully implemented yet
		WordItem searchItem = firstActiveWordItem();

		while( searchItem != null )
			{
			// Do for all active words
			if( searchItem.storeWordItemInFutureDatabase() != Constants.RESULT_OK )
				return addError( 1, null, null, "I failed to store a word item in the database" );

			searchItem = searchItem.nextWordItem();
			}

		searchItem = firstReplacedWordItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeWordItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced word item in the database" );
				}

			searchItem = searchItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}
*/

	// Protected query methods

	protected void countQueryInWordList()
		{
		countQueryInWordList( firstActiveWordItem() );
		countQueryInWordList( firstReplacedWordItem() );
		countQueryInWordList( firstDeletedWordItem() );
		}

	protected void clearQuerySelectionsInWordList()
		{
		clearQuerySelectionsInWordList( firstActiveWordItem() );
		clearQuerySelectionsInWordList( firstReplacedWordItem() );
		clearQuerySelectionsInWordList( firstDeletedWordItem() );
		}

	protected byte wordQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordNameString )
		{
		if( wordQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordNameString, firstActiveWordItem() ) == Constants.RESULT_OK &&
		wordQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordNameString, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return wordQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordNameString, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte itemQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		if( itemQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, firstActiveWordItem() ) == Constants.RESULT_OK &&
		itemQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return itemQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte listQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, StringBuffer queryListStringBuffer )
		{
		if( listQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer ) == Constants.RESULT_OK &&
		listQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer, firstActiveWordItem() ) == Constants.RESULT_OK &&
		listQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return listQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListStringBuffer, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte wordTypeQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, short queryWordTypeNr )
		{
		if( wordTypeQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr, firstActiveWordItem() ) == Constants.RESULT_OK &&
		wordTypeQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return wordTypeQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte parameterQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, int queryParameter )
		{
		if( parameterQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter, firstActiveWordItem() ) == Constants.RESULT_OK &&
		parameterQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return parameterQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte wordReferenceQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String wordReferenceNameString )
		{
		if( wordReferenceQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString, firstActiveWordItem() ) == Constants.RESULT_OK &&
		wordReferenceQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return wordReferenceQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte stringQueryInWordList( boolean isSelectOnFind, boolean isSelectActiveItems, boolean isSelectInactiveItems, boolean isSelectArchivedItems, boolean isSelectReplacedItems, boolean isSelectDeletedItems, String queryString )
		{
		if( stringQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString, firstActiveWordItem() ) == Constants.RESULT_OK &&
		stringQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return stringQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString, firstDeletedWordItem() );

		return CommonVariables.result;
		}

	protected byte showQueryResultInWordList( boolean showOnlyWords, boolean showOnlyWordReferences, boolean showOnlyStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		if( showQueryResultInWordList( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, firstActiveWordItem() ) == Constants.RESULT_OK &&
		showQueryResultInWordList( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, firstReplacedWordItem() ) == Constants.RESULT_OK )
			return showQueryResultInWordList( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, firstDeletedWordItem() );

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
					addError( 1, null, null, "I failed to add an active word item" );
				}
			else
				startError( 1, null, null, "I failed to create a word item" );
			}
		else
			startError( 1, null, null, "The current item number is undefined" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}
	};

/*************************************************************************
 *	"They share freely and give generously to those in need.
 *	Their good deeds will be remembered forever.
 *	They will have influence and honor." (Psalm 112:9)
 *************************************************************************/
