/*
 *	Class:			ListQuery
 *	Supports class:	List
 *	Purpose:		To process queries
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

	// Private constructible variables

	CommonVariables *commonVariables_;
	List *myList_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void itemQuery( bool isSelectOnFind, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( ( isReferenceQuery &&
			queryItem->hasFoundReferenceItemById( querySentenceNr, queryItemNr ) ) ||

			( !isReferenceQuery &&
			( querySentenceNr == NO_SENTENCE_NR ||
			querySentenceNr == queryItem->creationSentenceNr() ) &&

			( queryItemNr == NO_SENTENCE_NR ||
			queryItemNr == queryItem->itemNr() ) ) )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void listQuery( bool isSelectOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectOnFind )
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

	void wordTypeQuery( bool isSelectOnFind, unsigned short queryWordTypeNr, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasFoundWordType( queryWordTypeNr ) )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void parameterQuery( bool isSelectOnFind, unsigned int queryParameter, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasFoundParameter( queryParameter ) )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void wordQuery( bool isSelectOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectOnFind )
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
			searchItem = searchItem->nextItem;
			}
		}

	ResultType wordReferenceQuery( bool isSelectOnFind, char *wordReferenceNameString, Item *queryItem )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQuery";

		while( queryItem != NULL )
			{
			if( ( referenceResult = queryItem->findMatchingWordReferenceString( wordReferenceNameString ) ).result == RESULT_OK )
				{
				if( referenceResult.hasFoundMatchingStrings )
					{
					if( isSelectOnFind &&
					!queryItem->isSelectedByQuery )
						{
						commonVariables_->hasFoundQuery = true;
						queryItem->isSelectedByQuery = true;
						}
					}
				else
					{
					if( !isSelectOnFind &&
					queryItem->isSelectedByQuery )
						queryItem->isSelectedByQuery = false;
					}

				queryItem = queryItem->nextItem;
				}
			else
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to check the word references" );
			}

		return RESULT_OK;
		}

	ResultType stringQuery( bool isSelectOnFind, char *wordString, Item *queryItem )
		{
		ReferenceResultType referenceResult;
		bool hasFoundString;
		char *itemString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQuery";

		while( queryItem != NULL )
			{
			hasFoundString = false;

			if( ( itemString = queryItem->itemString() ) != NULL )
				{
				if( ( referenceResult = compareStrings( wordString, itemString ) ).result == RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						hasFoundString = true;
					}
				else
					return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to compare two strings" );
				}

			if( !hasFoundString &&
			( itemString = queryItem->extraItemString() ) != NULL )
				{
				if( ( referenceResult = compareStrings( wordString, itemString ) ).result == RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						hasFoundString = true;
					}
				else
					return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to compare two strings" );
				}

			if( hasFoundString )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType showQueryResult( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, Item *queryItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "showQueryResult";

		while( queryItem != NULL )
			{
			if( queryItem->isSelectedByQuery )
				{
				if( isOnlyShowingWords )
					queryItem->showWords( isReturnQueryToPosition, queryWordTypeNr );
				else
					{
					if( isOnlyShowingWordReferences )
						queryItem->showWordReferences( isReturnQueryToPosition );
					else
						{
						if( isOnlyShowingStrings )
							queryItem->showString( isReturnQueryToPosition );
						else
							{
							if( commonVariables_->presentation->writeText( true, promptTypeNr, queryWidth, queryItem->toString( queryWordTypeNr ) ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to show the info of an active item" );
							}
						}
					}
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

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
				myList_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), errorString );
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

	void itemQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		Item *searchItem;

		if( isSelectActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );
		}

	void listQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems )
		{
		Item *searchItem;

		if( isSelectActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			listQuery( isSelectOnFind, searchItem );

		if( isSelectInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			listQuery( isSelectOnFind, searchItem );

		if( isSelectArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			listQuery( isSelectOnFind, searchItem );

		if( isSelectReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			listQuery( isSelectOnFind, searchItem );

		if( isSelectDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			listQuery( isSelectOnFind, searchItem );
		}

	void wordTypeQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned short queryWordTypeNr )
		{
		Item *searchItem;

		if( isSelectActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, searchItem );

		if( isSelectInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, searchItem );

		if( isSelectArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, searchItem );

		if( isSelectReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, searchItem );

		if( isSelectDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, searchItem );
		}

	void parameterQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned int queryParameter )
		{
		Item *searchItem;

		if( isSelectActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			parameterQuery( isSelectOnFind, queryParameter, searchItem );

		if( isSelectInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			parameterQuery( isSelectOnFind, queryParameter, searchItem );

		if( isSelectArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			parameterQuery( isSelectOnFind, queryParameter, searchItem );

		if( isSelectReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			parameterQuery( isSelectOnFind, queryParameter, searchItem );

		if( isSelectDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			parameterQuery( isSelectOnFind, queryParameter, searchItem );
		}

	void wordQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems )
		{
		Item *searchItem;

		if( isSelectActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			wordQuery( isSelectOnFind, searchItem );

		if( isSelectInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			wordQuery( isSelectOnFind, searchItem );

		if( isSelectArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			wordQuery( isSelectOnFind, searchItem );

		if( isSelectReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			wordQuery( isSelectOnFind, searchItem );

		if( isSelectDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			wordQuery( isSelectOnFind, searchItem );
		}

	ReferenceResultType compareStrings( char *searchString, char *sourceString )
		{
		ReferenceResultType referenceResult;
		bool isStop;
		size_t searchStringPosition = 0;
		size_t sourceStringPosition = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareStrings";

		if( searchString != NULL )
			{
			if( sourceString != NULL )
				{
				if( searchString != sourceString )
					{
					referenceResult.hasFoundMatchingStrings = true;

					while( commonVariables_->result == RESULT_OK &&
					referenceResult.hasFoundMatchingStrings &&
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
											if( ( referenceResult = compareStrings( &searchString[searchStringPosition], &sourceString[sourceStringPosition] ) ).result == RESULT_OK )
												{
												if( referenceResult.hasFoundMatchingStrings )
													{
													isStop = true;
													searchStringPosition++;
													}
												else
													// Reset indicator
													referenceResult.hasFoundMatchingStrings = true;

												sourceStringPosition++;
												}
											else
												myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to compare the remaining strings" );
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
								referenceResult.hasFoundMatchingStrings = false;
							}
						}

					if( commonVariables_->result == RESULT_OK )
						{
						if( referenceResult.hasFoundMatchingStrings &&
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
							referenceResult.hasFoundMatchingStrings = false;
						}
					}
				else
					myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given strings are the same string" );
				}
			else
				myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given source string is undefined" );
			}
		else
			myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given search string is undefined" );

		referenceResult.result = commonVariables_->result;
		return referenceResult;
		}

	ResultType wordReferenceQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordReferenceNameString )
		{
		Item *searchItem;

		if( isSelectActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			wordReferenceQuery( isSelectOnFind, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			wordReferenceQuery( isSelectOnFind, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			wordReferenceQuery( isSelectOnFind, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			wordReferenceQuery( isSelectOnFind, wordReferenceNameString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			wordReferenceQuery( isSelectOnFind, wordReferenceNameString, searchItem );

		return commonVariables_->result;
		}

	ResultType stringQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordString )
		{
		Item *searchItem;

		if( isSelectActiveItems &&
		( searchItem = myList_->firstActiveItem() ) != NULL )
			stringQuery( isSelectOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectInactiveItems &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			stringQuery( isSelectOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectArchivedItems &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			stringQuery( isSelectOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectReplacedItems &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			stringQuery( isSelectOnFind, wordString, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		isSelectDeletedItems &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			stringQuery( isSelectOnFind, wordString, searchItem );

		return commonVariables_->result;
		}

	ResultType showQueryResult( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		Item *searchItem;

		if( ( searchItem = myList_->firstActiveItem() ) != NULL )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstInactiveItem() ) != NULL )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstArchivedItem() ) != NULL )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstReplacedItem() ) != NULL )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( commonVariables_->result == RESULT_OK &&
		( searchItem = myList_->firstDeletedItem() ) != NULL )
			showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		return commonVariables_->result;
		}
	};

/*************************************************************************
 *	"Your name, O Lord, endures forever;
 *	your name, O Lord, is known to every generation." (Psalm 135:13)
 *************************************************************************/
