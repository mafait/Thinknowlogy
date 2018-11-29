/*	Class:			WordList
 *	Parent class:	List
 *	Purpose:		To store word items
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

class WordList extends List
	{
	// Private common methods

	private WordItem firstActiveWordItem()
		{
		return (WordItem)firstActiveItem();
		}


	// Constructor

	protected WordList( WordItem myWordItem )
		{
		initializeListVariables( Constants.ADMIN_WORD_LIST_SYMBOL, "WordList", myWordItem );
		}


	// Protected cleanup methods

	protected void decrementItemNrRangeInWordList( int decrementSentenceNr, int decrementItemNr, int decrementOffset )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			// Efficiency: Only select words with decrement sentence number or higher
			if( searchWordItem.highestSentenceNrInWord() >= decrementSentenceNr &&
			searchWordItem.decrementItemNrRangeInWord( decrementSentenceNr, decrementItemNr, decrementOffset ) != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to decrement item numbers in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void decrementSentenceNrsInWordList( int startSentenceNr )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			// Efficiency: Only select words with start sentence number or higher
			if( searchWordItem.highestSentenceNrInWord() >= startSentenceNr &&
			searchWordItem.decrementSentenceNrsInWord( startSentenceNr ) != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to decrement the sentence numbers from the current sentence number in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void deleteSentencesInWordList( int lowestSentenceNr )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			// Efficiency: Only select words with lowest sentence number or higher
			if( searchWordItem.highestSentenceNrInWord() >= lowestSentenceNr &&
			searchWordItem.deleteSentencesInWord( lowestSentenceNr ) != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to delete sentences in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void removeFirstRangeOfDeletedItemsInWordList()
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null &&
		GlobalVariables.nDeletedItems == 0 )
			{
			if( searchWordItem.removeFirstRangeOfDeletedItemsInWord() != Constants.RESULT_OK )
				// This method can be called during an error situation. So, the result isn't returned
				addError( 1, "I failed to remove the first deleted items in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected int highestCurrentSentenceItemNrInWordList( int currentSentenceNr, int currentSentenceItemNr )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			// Efficiency: Only select words with current sentence number or higher
			if( searchWordItem.highestSentenceNrInWord() >= currentSentenceNr )
				currentSentenceItemNr = searchWordItem.highestCurrentSentenceItemNrInWord( currentSentenceNr, currentSentenceItemNr );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return currentSentenceItemNr;
		}

	protected int highestFoundSentenceNrInWordList( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int maxSentenceNr )
		{
		int highestFoundSentenceNr = Constants.NO_SENTENCE_NR;
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null &&
		highestFoundSentenceNr < maxSentenceNr )
			{
			// Efficiency: Only select words with higher sentence number
			if( searchWordItem.highestSentenceNrInWord() > highestFoundSentenceNr )
				highestFoundSentenceNr = searchWordItem.highestFoundSentenceNrInWord( isIncludingDeletedItems, isIncludingTemporaryLists, highestFoundSentenceNr, maxSentenceNr );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return highestFoundSentenceNr;
		}


	// Protected database connection methods
/*
	protected byte storeChangesInFutureDatabaseInWordList()
		{
		WordItem searchWordItem = firstActiveWordItem();

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

	protected byte redoCurrentSentenceInWordList()
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			if( searchWordItem.redoCurrentSentenceInWord() != Constants.RESULT_OK )
				return addError( 1, "I failed to redo the current sentence in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte undoCurrentSentenceInWordList()
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			if( searchWordItem.undoCurrentSentenceInWord() != Constants.RESULT_OK )
				return addError( 1, "I failed to undo the current sentence in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}


	// Protected query methods

	protected void countQueryInWordList()
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			searchWordItem.countQuery();
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void clearQuerySelectionsInWordList()
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			searchWordItem.clearQuerySelections();
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void itemQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			searchWordItem.itemQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void listQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, StringBuffer queryListStringBuffer )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			searchWordItem.listQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListStringBuffer );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void parameterQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, int queryParameter )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			if( isSelectingOnFind &&
			searchWordItem.hasParameter( queryParameter ) )
				searchWordItem.isSelectedByQuery = true;

			searchWordItem.parameterQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected void wordTypeQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, short queryWordTypeNr )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			searchWordItem.wordTypeQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr );
			searchWordItem = searchWordItem.nextWordItem();
			}
		}

	protected byte displayQueryResultInWordList( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			if( searchWordItem.displayQueryResultInWord( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
				return addError( 1, "I failed to display the query result in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte stringQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, String queryString )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			if( searchWordItem.stringQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryString ) != Constants.RESULT_OK )
				return addError( 1, "I failed to query strings in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte wordQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, String wordNameString )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			if( searchWordItem.wordQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, wordNameString ) != Constants.RESULT_OK )
				return addError( 1, "I failed to query a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQueryInWordList( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		WordItem searchWordItem = firstActiveWordItem();

		while( searchWordItem != null )
			{
			if( searchWordItem.wordReferenceQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != Constants.RESULT_OK )
				return addError( 1, "I failed to query word references in a word" );

			searchWordItem = searchWordItem.nextWordItem();
			}

		return Constants.RESULT_OK;
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
