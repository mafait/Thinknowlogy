/*	Class:			ListQuery
 *	Supports class:	List
 *	Purpose:		To process queries
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

#include "List.h"
#include "Presentation.cpp"

class ListQuery
	{
	friend class List;

	// Private constructed variables

	CommonVariables *commonVariables_;
	List *myList_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void itemQuery( bool isSelectingOnFind, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( ( isReferenceQuery &&
			queryItem->hasReferenceItemById( querySentenceNr, queryItemNr ) ) ||

			( !isReferenceQuery &&

			( ( querySentenceNr == NO_SENTENCE_NR &&
			queryItemNr == NO_SENTENCE_NR ) ||

			( querySentenceNr > NO_SENTENCE_NR &&
			queryItemNr == NO_SENTENCE_NR &&
			queryItem->hasSentenceNr( querySentenceNr ) ) ||

			( queryItem->itemNr() == queryItemNr &&

			( querySentenceNr == NO_SENTENCE_NR ||
			queryItem->creationSentenceNr() == querySentenceNr ) ) ) ) )
				{
				if( isSelectingOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void listQuery( bool isSelectingOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectingOnFind )
				{
				if( !queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void wordTypeQuery( bool isSelectingOnFind, unsigned short queryWordTypeNr, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasWordType( queryWordTypeNr ) )
				{
				if( isSelectingOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void parameterQuery( bool isSelectingOnFind, unsigned int queryParameter, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasParameter( queryParameter ) )
				{
				if( isSelectingOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void wordQuery( bool isSelectingOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectingOnFind )
				{
				if( !queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void clearQuerySelections( Item *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->isSelectedByQuery = false;
			searchItem->isSelectedByJustificationQuery = false;

			searchItem = searchItem->nextItem;
			}
		}

	ResultType displayQueryResult( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, Item *queryItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "displayQueryResult";

		while( queryItem != NULL )
			{
			if( queryItem->isSelectedByQuery ||
			queryItem->isSelectedByJustificationQuery )
				{
				if( isOnlyDisplayingWords )
					queryItem->displayWords( isReturnQueryToPosition, queryWordTypeNr );
				else
					{
					if( isOnlyDisplayingWordReferences )
						queryItem->displayWordReferences( isReturnQueryToPosition );
					else
						{
						if( isOnlyDisplayingStrings )
							queryItem->displayString( isReturnQueryToPosition );
						else
							{
							if( commonVariables_->presentation->writeText( true, promptTypeNr, queryWidth, queryItem->toString( queryWordTypeNr ) ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, "I failed to display the info of an active item" );
							}
						}
					}
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType stringQuery( bool isSelectingOnFind, char *wordString, Item *queryItem )
		{
		StringResultType stringResult;
		bool hasFoundString;
		char *itemString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQuery";

		while( queryItem != NULL )
			{
			hasFoundString = false;

			if( ( itemString = queryItem->itemString() ) != NULL )
				{
				if( ( stringResult = compareStrings( wordString, itemString ) ).result != RESULT_OK )
					return myList_->addError( functionNameString, moduleNameString_, "I failed to compare two strings" );

				if( stringResult.hasFoundMatchingStrings )
					hasFoundString = true;
				}

			if( !hasFoundString &&
			( itemString = queryItem->virtualGuideByGrammarString() ) != NULL )
				{
				if( ( stringResult = compareStrings( wordString, itemString ) ).result != RESULT_OK )
					return myList_->addError( functionNameString, moduleNameString_, "I failed to compare a Guide by Grammar string" );

				if( stringResult.hasFoundMatchingStrings )
					hasFoundString = true;
				}

			if( hasFoundString )
				{
				if( isSelectingOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType wordReferenceQuery( bool isSelectingOnFind, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString, Item *queryItem )
		{
		StringResultType stringResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQuery";

		while( queryItem != NULL )
			{
			if( ( stringResult = queryItem->findMatchingWordReferenceString( wordReferenceNameString ) ).result != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, "I failed to check the word references" );

			if( stringResult.hasFoundMatchingStrings )
				{
				if( queryItem->isSelectedByQuery )
					{
					if( isSelectingAttachedJustifications )
						queryItem->selectingAttachedJustifications( isSelectingJustificationSpecifications );
					}
				else
					{
					if( isSelectingOnFind )
						{
						commonVariables_->hasFoundQuery = true;
						queryItem->isSelectedByQuery = true;
						}
					}
				}
			else
				{
				if( !isSelectingOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

	ListQuery( CommonVariables *commonVariables, List *myList )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myList_ = myList;
		strcpy( moduleNameString_, "ListQuery" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myList_ == NULL )
			strcpy( errorString, "The given my list is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myList_ != NULL &&
			myList_->myWordItem() != NULL )
				myList_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void countQuery()
		{
		Item *searchItem = myList_->firstActiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nActiveQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nInactiveQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nArchivedQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nReplacedQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstDeletedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nDeletedQueryItems++;

			searchItem = searchItem->nextItem;
			}
		}

	void clearQuerySelections()
		{
		Item *searchItem;

		if( ( searchItem = myList_->firstActiveItem() ) != NULL )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_->firstInactiveItem() ) != NULL )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_->firstArchivedItem() ) != NULL )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_->firstReplacedItem() ) != NULL )
			clearQuerySelections( searchItem );

		if( ( searchItem = myList_->firstDeletedItem() ) != NULL )
			clearQuerySelections( searchItem );
		}

	void itemQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );
		}

	void listQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			listQuery( isSelectingOnFind, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			listQuery( isSelectingOnFind, searchItem );
		}

	void wordTypeQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );
		}

	void parameterQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );
		}

	void wordQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );
		}

	ResultType displayQueryResult( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		Item *searchItem;

		if( ( searchItem = myList_->firstActiveItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		return commonVariables_->result;
		}

	ResultType stringQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordString )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			stringQuery( isSelectingOnFind, wordString, searchItem );

		return commonVariables_->result;
		}

	ResultType wordReferenceQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectingDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		return commonVariables_->result;
		}

	StringResultType compareStrings( char *searchString, char *sourceString )
		{
		StringResultType stringResult;
		bool isStop;
		size_t searchStringPosition = 0;
		size_t sourceStringPosition = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareStrings";

		if( searchString == NULL )
			return myList_->startStringResultError( functionNameString, moduleNameString_, "The given search string is undefined" );

		if( sourceString == NULL )
			return myList_->startStringResultError( functionNameString, moduleNameString_, "The given source string is undefined" );

		if( searchString == sourceString )
			return myList_->startStringResultError( functionNameString, moduleNameString_, "The given strings are the same string" );

		stringResult.hasFoundMatchingStrings = true;

		while( stringResult.hasFoundMatchingStrings &&
		searchStringPosition < strlen( searchString ) &&
		sourceStringPosition < strlen( sourceString ) )
			{
			if( searchString[searchStringPosition] == SYMBOL_QUESTION_MARK ||
			searchString[searchStringPosition] == sourceString[sourceStringPosition] )
				{
				searchStringPosition++;
				sourceStringPosition++;
				}
			else
				{
				if( searchString[searchStringPosition] == SYMBOL_ASTERISK )
					{
					if( ++searchStringPosition < strlen( searchString ) )
						{
						isStop = false;

						while( !isStop &&
						sourceStringPosition < strlen( sourceString ) )
							{
							if( searchString[searchStringPosition] == sourceString[sourceStringPosition] )
								{
								// Check remaining strings
								if( ( stringResult = compareStrings( &searchString[searchStringPosition], &sourceString[sourceStringPosition] ) ).result != RESULT_OK )
									return myList_->addStringResultError( functionNameString, moduleNameString_, "I failed to compare the remaining strings" );

								if( stringResult.hasFoundMatchingStrings )
									{
									isStop = true;
									searchStringPosition++;
									}
								else
									// Reset indicator
									stringResult.hasFoundMatchingStrings = true;

								sourceStringPosition++;
								}
							else
								// Skip source characters if not equal
								sourceStringPosition++;
							}
						}
					else
						// Empty source string after asterisk
						sourceStringPosition = strlen( sourceString );
					}
				else
					stringResult.hasFoundMatchingStrings = false;
				}
			}

		if( stringResult.hasFoundMatchingStrings &&
		sourceStringPosition == strlen( sourceString ) )
			{
			// Check search string for extra asterisks
			while( searchStringPosition < strlen( searchString ) &&
			searchString[searchStringPosition] == SYMBOL_ASTERISK )
				// Skip extra asterisks
				searchStringPosition++;
			}

		if( searchStringPosition < strlen( searchString ) ||
		sourceStringPosition < strlen( sourceString ) )
			stringResult.hasFoundMatchingStrings = false;

		return stringResult;
		}
	};

/*************************************************************************
 *	"Your name, O Lord, endures forever;
 *	your name, O Lord, is known to every generation." (Psalm 135:13)
 *************************************************************************/
