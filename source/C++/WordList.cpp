/*	Class:			WordList
 *	Parent class:	List
 *	Purpose:		To store word items
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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

#ifndef WORDLIST
#define WORDLIST 1
#include "List.h"
#include "WordItem.h"

class WordList : private List
	{
	friend class AdminCleanup;
	friend class AdminItem;
	friend class AdminQuery;
	friend class AdminReadCreateWords;

	// Private cleanup functions

	void deleteSentencesInWordList( unsigned int lowestSentenceNr, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentencesInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->deleteSentencesInWord( lowestSentenceNr ) != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, NULL, "I failed to delete sentences in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void decrementSentenceNrsInWordList( unsigned int startSentenceNr, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrsInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->decrementSentenceNrsInWord( startSentenceNr ) != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, NULL, "I failed to decrement the sentence numbers from the current sentence number in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRangeInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->decrementItemNrRangeInWord( decrementSentenceNr, decrementItemNr, decrementOffset ) != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, NULL, "I failed to decrement item numbers in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void getHighestInUseSentenceNrInWordList( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr, WordItem *searchWordItem )
		{
		while( searchWordItem != NULL &&
		commonVariables()->highestInUseSentenceNr < highestSentenceNr )
			{
			searchWordItem->getHighestInUseSentenceNrInWord( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );
			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void removeFirstRangeOfDeletedItemsInWordList( WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItemsInWordList";

		while( searchWordItem != NULL &&
		commonVariables()->nDeletedItems == 0 )
			{
			if( searchWordItem->removeFirstRangeOfDeletedItemsInWord() != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, NULL, "I failed to remove the first deleted items in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void setCurrentItemNrInWordList( WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			searchWordItem->setCurrentItemNrInWord();
			searchWordItem = searchWordItem->nextWordItem();
			}
		}


	// Private query functions

	void countQueryInWordList( WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			searchWordItem->countQuery();
			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void clearQuerySelectionsInWordList( WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			searchWordItem->clearQuerySelections();
			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	ResultType displayQueryResultInWordList( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "displayQueryResultInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->displayQueryResultInWord( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to display the query result in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	ResultType itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->itemQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to query item numbers in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	ResultType listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->listQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to query lists in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	ResultType parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( isSelectingOnFind &&
			searchWordItem->hasParameter( queryParameter ) )
				searchWordItem->isSelectedByQuery = true;

			if( searchWordItem->parameterQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to query parameters in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	ResultType stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->stringQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to query strings in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	ResultType wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->wordQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to query a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	ResultType wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->wordReferenceQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to query word references in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	ResultType wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->wordTypeQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to query word types in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}


	// Private common functions

	WordItem *firstActiveWordItem()
		{
		return (WordItem *)firstActiveItem();
		}

	WordItem *firstDeletedWordItem()
		{
		return (WordItem *)firstDeletedItem();
		}


	protected:
	// Constructor

	WordList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( ADMIN_WORD_LIST_SYMBOL, "WordList", commonVariables, myWordItem );
		}

	~WordList()
		{
		WordItem *deleteWordItem;
		WordItem *searchWordItem = firstActiveWordItem();

		while( searchWordItem != NULL )
			{
			deleteWordItem = searchWordItem;
			searchWordItem = searchWordItem->nextWordItem();
			delete deleteWordItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has replaced items." );

		searchWordItem = firstDeletedWordItem();

		while( searchWordItem != NULL )
			{
			deleteWordItem = searchWordItem;
			searchWordItem = searchWordItem->nextWordItem();
			delete deleteWordItem;
			}
		}


	// Protected cleanup functions

	void decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != NULL )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchWordItem );
		}

	void decrementSentenceNrsInWordList( unsigned int startSentenceNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			decrementSentenceNrsInWordList( startSentenceNr, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != NULL )
			decrementSentenceNrsInWordList( startSentenceNr, searchWordItem );
		}

	void deleteSentencesInWordList( unsigned int lowestSentenceNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			deleteSentencesInWordList( lowestSentenceNr, searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != NULL )
			deleteSentencesInWordList( lowestSentenceNr, searchWordItem );
		}

	void getHighestInUseSentenceNrInWordList( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchWordItem );

		if( isIncludingDeletedItems &&
		( searchWordItem = firstDeletedWordItem() ) != NULL &&
		commonVariables()->highestInUseSentenceNr < highestSentenceNr )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchWordItem );
		}

	void removeFirstRangeOfDeletedItemsInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			removeFirstRangeOfDeletedItemsInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != NULL )
			removeFirstRangeOfDeletedItemsInWordList( searchWordItem );
		}

	void setCurrentItemNrInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			setCurrentItemNrInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != NULL )
			setCurrentItemNrInWordList( searchWordItem );
		}


	// Protected database connection functions
/*
	ResultType storeChangesInFutureDatabase()
		{
		WordItem *searchWordItem = firstActiveWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchWordItem != NULL )
			{
			// Do for all words
			if( searchWordItem->storeWordItemInFutureDatabase() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to store a word item in the database" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}
*/

	// Protected query functions

	void countQueryInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			countQueryInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != NULL )
			countQueryInWordList( searchWordItem );
		}

	void clearQuerySelectionsInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			clearQuerySelectionsInWordList( searchWordItem );

		if( ( searchWordItem = firstDeletedWordItem() ) != NULL )
			clearQuerySelectionsInWordList( searchWordItem );
		}

	ResultType displayQueryResultInWordList( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			displayQueryResultInWordList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			displayQueryResultInWordList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchWordItem );

		return commonVariables()->result;
		}

	ResultType itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchWordItem );

		return commonVariables()->result;
		}

	ResultType listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString, searchWordItem );

		return commonVariables()->result;
		}

	ResultType parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchWordItem );

		return commonVariables()->result;
		}

	ResultType stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchWordItem );

		return commonVariables()->result;
		}

	ResultType wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchWordItem );

		return commonVariables()->result;
		}

	ResultType wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchWordItem );

		return commonVariables()->result;
		}

	ResultType wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchWordItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchWordItem = firstDeletedWordItem() ) != NULL )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchWordItem );

		return commonVariables()->result;
		}


	// Protected read word functions

	WordResultType createWordItem( bool isLanguageWord, unsigned short wordParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordItem";

		// Create word
		if( ( wordResult.createdWordItem = new WordItem( isLanguageWord, wordParameter, commonVariables(), this ) ) == NULL )
			return startWordResultError( functionNameString, NULL, "I failed to create a word item" );

		if( addItemToList( QUERY_ACTIVE_CHAR, wordResult.createdWordItem ) != RESULT_OK )
			return addWordResultError( functionNameString, NULL, "I failed to add an active word item" );

		return wordResult;
		}
	};
#endif

/*************************************************************************
 *	"They share freely and give generously to those in need.
 *	Their good deeds will be remembered forever.
 *	They will have influence and honor." (Psalm 112:9)
 *************************************************************************/
