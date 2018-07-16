/*	Class:			WordList
 *	Parent class:	List
 *	Purpose:		To store word items
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

#include "List.h"
#include "WordItem.h"

class WordList : private List
	{
	friend class AdminItem;

	// Private cleanup functions

	void deleteWordList( WordItem *searchWordItem )
		{
		WordItem *deleteWordItem;

		while( searchWordItem != NULL )
			{
			deleteWordItem = searchWordItem;
			searchWordItem = searchWordItem->nextWordItem();
			delete deleteWordItem;
			}
		}

	void deleteSentencesInWordList( unsigned int lowestSentenceNr, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteSentencesInWordList";

		while( searchWordItem != NULL )
			{
			// Efficiency: Only select words with lowest sentence number or higher
			if( searchWordItem->highestSentenceNrInWord() >= lowestSentenceNr &&
			searchWordItem->deleteSentencesInWord( lowestSentenceNr ) != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, "I failed to delete sentences in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void decrementSentenceNrsInWordList( unsigned int startSentenceNr, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementSentenceNrsInWordList";

		while( searchWordItem != NULL )
			{
			// Efficiency: Only select words with start sentence number or higher
			if( searchWordItem->highestSentenceNrInWord() >= startSentenceNr &&
			searchWordItem->decrementSentenceNrsInWord( startSentenceNr ) != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, "I failed to decrement the sentence numbers from the current sentence number in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementItemNrRangeInWordList";

		while( searchWordItem != NULL )
			{
			// Efficiency: Only select words with decrement sentence number or higher
			if( searchWordItem->highestSentenceNrInWord() >= decrementSentenceNr &&
			searchWordItem->decrementItemNrRangeInWord( decrementSentenceNr, decrementItemNr, decrementOffset ) != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, "I failed to decrement item numbers in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void removeFirstRangeOfDeletedItemsInWordList( WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeFirstRangeOfDeletedItemsInWordList";

		while( searchWordItem != NULL &&
		globalVariables()->nDeletedItems == 0 )
			{
			if( searchWordItem->removeFirstRangeOfDeletedItemsInWord() != RESULT_OK )
				// This function can be called during an error situation. So, the result isn't returned
				addError( functionNameString, "I failed to remove the first deleted items in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	unsigned int highestCurrentSentenceItemNrInWordList( unsigned int currentSentenceNr, unsigned int currentSentenceItemNr, WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			// Efficiency: Only select words with current sentence number or higher
			if( searchWordItem->highestSentenceNrInWord() >= currentSentenceNr )
				currentSentenceItemNr = searchWordItem->highestCurrentSentenceItemNrInWord( currentSentenceNr, currentSentenceItemNr );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return currentSentenceItemNr;
		}

	unsigned int highestFoundSentenceNrInWordList( bool isIncludingTemporaryLists, unsigned int highestFoundSentenceNr, unsigned int maxSentenceNr, WordItem *searchWordItem )
		{
		while( searchWordItem != NULL &&
		highestFoundSentenceNr < maxSentenceNr )
			{
			// Efficiency: Only select words with higher sentence number
			if( searchWordItem->highestSentenceNrInWord() > highestFoundSentenceNr )
				highestFoundSentenceNr = searchWordItem->highestFoundSentenceNrInWord( isIncludingTemporaryLists, highestFoundSentenceNr, maxSentenceNr );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return highestFoundSentenceNr;
		}


	// Protected database connection functions
/*
	signed char storeChangesInFutureDatabaseInWordList( WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "storeChangesInFutureDatabaseInWordList";

		while( searchWordItem != NULL )
			{
			// Do for all words
			if( searchWordItem->storeChangesInFutureDatabaseInWord() != RESULT_OK )
				return addError( functionNameString, "I failed to store changes of a word item in the database" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}
*/

	// Protected imperative functions

	signed char redoCurrentSentenceInWordList( WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "redoCurrentSentenceInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->redoCurrentSentenceInWord() != RESULT_OK )
				return addError( functionNameString, "I failed to redo the current sentence in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	signed char undoCurrentSentenceInWordList( WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "undoCurrentSentenceInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->undoCurrentSentenceInWord() != RESULT_OK )
				return addError( functionNameString, "I failed to undo the current sentence in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
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

	void itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			searchWordItem->itemQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString, WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			searchWordItem->listQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListString );
			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter, WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			if( isSelectingOnFind &&
			searchWordItem->hasParameter( queryParameter ) )
				searchWordItem->isSelectedByQuery = true;

			searchWordItem->parameterQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter );
			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	void wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr, WordItem *searchWordItem )
		{
		while( searchWordItem != NULL )
			{
			searchWordItem->wordTypeQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr );
			searchWordItem = searchWordItem->nextWordItem();
			}
		}

	signed char displayQueryResultInWordList( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displayQueryResultInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->displayQueryResultInWord( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
				return addError( functionNameString, "I failed to display the query result in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	signed char stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryString, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "stringQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->stringQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryString ) != RESULT_OK )
				return addError( functionNameString, "I failed to query strings in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	signed char wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *wordNameString, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "wordQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->wordQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, wordNameString ) != RESULT_OK )
				return addError( functionNameString, "I failed to query a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}

	signed char wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString, WordItem *searchWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "wordReferenceQueryInWordList";

		while( searchWordItem != NULL )
			{
			if( searchWordItem->wordReferenceQueryInWord( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != RESULT_OK )
				return addError( functionNameString, "I failed to query word references in a word" );

			searchWordItem = searchWordItem->nextWordItem();
			}

		return RESULT_OK;
		}


	// Private common functions

	WordItem *firstActiveWordItem()
		{
		return (WordItem *)firstActiveItem();
		}


	protected:
	// Constructor

	WordList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( ADMIN_WORD_LIST_SYMBOL, "WordList", globalVariables, inputOutput, myWordItem );
		}

	~WordList()
		{
		deleteWordList( firstActiveWordItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class WordList has replaced items." );

		deleteWordList( (WordItem *)firstDeletedItem() );
		}


	// Protected cleanup functions

	void decrementItemNrRangeInWordList( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			decrementItemNrRangeInWordList( decrementSentenceNr, decrementItemNr, decrementOffset, searchWordItem );
		}

	void decrementSentenceNrsInWordList( unsigned int startSentenceNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			decrementSentenceNrsInWordList( startSentenceNr, searchWordItem );
		}

	void deleteSentencesInWordList( unsigned int lowestSentenceNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			deleteSentencesInWordList( lowestSentenceNr, searchWordItem );
		}

	void removeFirstRangeOfDeletedItemsInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			removeFirstRangeOfDeletedItemsInWordList( searchWordItem );
		}

	unsigned int highestCurrentSentenceItemNrInWordList( unsigned int currentSentenceNr, unsigned int currentSentenceItemNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			currentSentenceItemNr = highestCurrentSentenceItemNrInWordList( currentSentenceNr, currentSentenceItemNr, searchWordItem );

		return currentSentenceItemNr;
		}

	unsigned int highestFoundSentenceNrInWordList( bool isIncludingTemporaryLists, unsigned int maxSentenceNr )
		{
		unsigned int highestFoundSentenceNr = NO_SENTENCE_NR;
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			highestFoundSentenceNr = highestFoundSentenceNrInWordList( isIncludingTemporaryLists, highestFoundSentenceNr, maxSentenceNr, searchWordItem );

		return highestFoundSentenceNr;
		}


	// Protected database connection functions
/*
	signed char storeChangesInFutureDatabaseInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			storeChangesInFutureDatabaseInWordList( searchWordItem );

		return globalVariables()->result;
		}
*/

	// Protected imperative functions

	signed char redoCurrentSentenceInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			redoCurrentSentenceInWordList( searchWordItem );

		return globalVariables()->result;
		}

	signed char undoCurrentSentenceInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			undoCurrentSentenceInWordList( searchWordItem );

		return globalVariables()->result;
		}


	// Protected query functions

	void countQueryInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			countQueryInWordList( searchWordItem );
		}

	void clearQuerySelectionsInWordList()
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			clearQuerySelectionsInWordList( searchWordItem );
		}

	void itemQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr, searchWordItem );
		}

	void listQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListString, searchWordItem );
		}

	void parameterQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter, searchWordItem );
		}

	void wordTypeQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr, searchWordItem );
		}

	signed char displayQueryResultInWordList( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			displayQueryResultInWordList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchWordItem );

		return globalVariables()->result;
		}

	signed char stringQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryString, searchWordItem );

		return globalVariables()->result;
		}

	signed char wordQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *wordNameString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, wordNameString, searchWordItem );

		return globalVariables()->result;
		}

	signed char wordReferenceQueryInWordList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		WordItem *searchWordItem;

		if( ( searchWordItem = firstActiveWordItem() ) != NULL )
			wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchWordItem );

		return globalVariables()->result;
		}


	// Protected read word functions

	WordResultType createWordItem( bool isLanguageWord, unsigned short wordParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createWordItem";

		// Create word
		if( ( wordResult.createdWordItem = new WordItem( isLanguageWord, wordParameter, globalVariables(), inputOutput(), this ) ) == NULL )
			return startWordResultError( functionNameString, "I failed to create a word item" );

		if( addItemToList( QUERY_ACTIVE_CHAR, wordResult.createdWordItem ) != RESULT_OK )
			return addWordResultError( functionNameString, "I failed to add an active word item" );

		return wordResult;
		}
	};

/*************************************************************************
 *	"They share freely and give generously to those in need.
 *	Their good deeds will be remembered forever.
 *	They will have influence and honor." (Psalm 112:9)
 *************************************************************************/
