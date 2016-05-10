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

	void getHighestInUseSentenceNrInWordList( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr, WordItem *searchItem )
		{
		while( searchItem != NULL &&
		commonVariables()->highestInUseSentenceNr < highestSentenceNr )
			{
			searchItem->getHighestInUseSentenceNrInWord( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );
			searchItem = searchItem->nextWordItem();
			}
		}

	void setCurrentItemNrInWordList( WordItem *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->setCurrentItemNrInWord();
			searchItem = searchItem->nextWordItem();
			}
		}

	ResultType deleteSentencesInWordList( unsigned int lowestSentenceNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentencesInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->deleteSentencesInWord( lowestSentenceNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to delete sentences in a word" );
			}

		return RESULT_OK;
		}

	ResultType removeFirstRangeOfDeletedItemsInWordList( WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeFirstRangeOfDeletedItemsInWordList";

		while( searchItem != NULL &&
		commonVariables()->nDeletedItems == 0 )
			{
			if( searchItem->removeFirstRangeOfDeletedItemsInWord() == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to remove the first deleted items in a word" );
			}

		return RESULT_OK;
		}

	ResultType decrementSentenceNrsInWordList( unsigned int startSentenceNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementSentenceNrsInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->decrementSentenceNrsInWord( startSentenceNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to decrement the sentence numbers from the current sentence number in a word" );
			}

		return RESULT_OK;
		}

	ResultType decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNrRangeInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->decrementItemNrRangeInWord( decrementSentenceNr, decrementItemNr, decrementOffset ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to decrement item numbers in a word" );
			}

		return RESULT_OK;
		}


	// Private query functions

	void countQueryInWordList( WordItem *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->countQuery();
			searchItem = searchItem->nextWordItem();
			}
		}

	void clearQuerySelectionsInWordList( WordItem *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->clearQuerySelections();
			searchItem = searchItem->nextWordItem();
			}
		}

	ResultType wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->wordQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query a word" );
			}

		return RESULT_OK;
		}

	ResultType itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->itemQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query item numbers in a word" );
			}

		return RESULT_OK;
		}

	ResultType listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->listQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query lists in a word" );
			}

		return RESULT_OK;
		}

	ResultType wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->wordTypeQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query word types in a word" );
			}

		return RESULT_OK;
		}

	ResultType parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQueryInWordList";

		while( searchItem != NULL )
			{
			if( isSelectingOnFind &&
			searchItem->hasFoundParameter( queryParameter ) )
				searchItem->isSelectedByQuery = true;

			if( searchItem->parameterQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query parameters in a word" );
			}

		return RESULT_OK;
		}

	ResultType wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->wordReferenceQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query word references in a word" );
			}

		return RESULT_OK;
		}

	ResultType stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQueryInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->stringQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to query strings in a word" );
			}

		return RESULT_OK;
		}

	ResultType showQueryResultInWordList( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, WordItem *searchItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "showQueryResultInWordList";

		while( searchItem != NULL )
			{
			if( searchItem->showQueryResultInWord( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) == RESULT_OK )
				searchItem = searchItem->nextWordItem();
			else
				return addError( functionNameString, NULL, "I failed to show the query result in a word" );
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
	// Constructor / deconstructor

	WordList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( ADMIN_WORD_LIST_SYMBOL, "WordList", commonVariables, myWordItem );
		}

	~WordList()
		{
		WordItem *deleteItem;
		WordItem *searchItem = firstActiveWordItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has replaced items." );

		searchItem = firstDeletedWordItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordItem();
			delete deleteItem;
			}
		}


	// Protected cleanup functions

	void getHighestInUseSentenceNrInWordList( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchItem );

		if( isIncludingDeletedItems &&
		( searchItem = firstDeletedWordItem() ) != NULL &&
		commonVariables()->highestInUseSentenceNr < highestSentenceNr )
			getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr, searchItem );
		}

	void setCurrentItemNrInWordList()
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			setCurrentItemNrInWordList( searchItem );

		if( ( searchItem = firstDeletedWordItem() ) != NULL )
			setCurrentItemNrInWordList( searchItem );
		}

	ResultType decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchItem );

		return RESULT_OK;
		}

	ResultType decrementSentenceNrsInWordList( unsigned int startSentenceNr )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			decrementSentenceNrsInWordList( startSentenceNr, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			decrementSentenceNrsInWordList( startSentenceNr, searchItem );

		return RESULT_OK;
		}

	ResultType deleteSentencesInWordList( unsigned int lowestSentenceNr )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			deleteSentencesInWordList( lowestSentenceNr, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			deleteSentencesInWordList( lowestSentenceNr, searchItem );

		return RESULT_OK;
		}

	ResultType removeFirstRangeOfDeletedItemsInWordList()
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			removeFirstRangeOfDeletedItemsInWordList( searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			removeFirstRangeOfDeletedItemsInWordList( searchItem );

		return RESULT_OK;
		}


	// Protected database connection functions
/*
	ResultType storeChangesInFutureDatabase()
		{
		WordItem *searchItem = firstActiveWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			// Do for all active words
			if( searchItem->storeWordItemInFutureDatabase() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to store a word item in the database" );

			searchItem = searchItem->nextWordItem();
			}

		return RESULT_OK;
		}
*/

	// Protected query functions

	void countQueryInWordList()
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			countQueryInWordList( searchItem );

		if( ( searchItem = firstDeletedWordItem() ) != NULL )
			countQueryInWordList( searchItem );
		}

	void clearQuerySelectionsInWordList()
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			clearQuerySelectionsInWordList( searchItem );

		if( ( searchItem = firstDeletedWordItem() ) != NULL )
			clearQuerySelectionsInWordList( searchItem );
		}

	ResultType wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString, searchItem );

		return commonVariables()->result;
		}

	ResultType itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		return commonVariables()->result;
		}

	ResultType listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString, searchItem );

		return commonVariables()->result;
		}

	ResultType wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr, searchItem );

		return commonVariables()->result;
		}

	ResultType parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter, searchItem );

		return commonVariables()->result;
		}

	ResultType wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		return commonVariables()->result;
		}

	ResultType stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString, searchItem );

		return commonVariables()->result;
		}

	ResultType showQueryResultInWordList( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		WordItem *searchItem;

		if( ( searchItem = firstActiveWordItem() ) != NULL )
			showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables()->result == RESULT_OK &&
		( searchItem = firstDeletedWordItem() ) != NULL )
			showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		return commonVariables()->result;
		}


	// Protected read word functions

	WordResultType createWordItem( bool isLanguageWord, unsigned short wordParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordItem";

		if( commonVariables()->currentItemNr < MAX_ITEM_NR )
			{
			// Create word
			if( ( wordResult.createdWordItem = new WordItem( isLanguageWord, wordParameter, commonVariables(), this ) ) != NULL )
				{
				if( addItemToList( QUERY_ACTIVE_CHAR, wordResult.createdWordItem ) != RESULT_OK )
					addError( functionNameString, NULL, "I failed to add an active word item" );
				}
			else
				startError( functionNameString, NULL, "I failed to create a word item" );
			}
		else
			startError( functionNameString, NULL, "The current item number is undefined" );

		wordResult.result = commonVariables()->result;
		return wordResult;
		}
	};
#endif

/*************************************************************************
 *	"They share freely and give generously to those in need.
 *	Their good deeds will be remembered forever.
 *	They will have influence and honor." (Psalm 112:9)
 *************************************************************************/
