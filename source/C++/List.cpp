﻿/*	Class:		List
 *	Purpose:	Base class to store the items of the knowledge structure
 *	Version:	Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

#include "FileResultType.cpp"
#include "InputOutput.cpp"
#include "List.h"
#include "SelectionResultType.cpp"
#include "ShortResultType.cpp"
#include "CreateAndAssignResultType.cpp"
#include "WordEndingResultType.cpp"

	// Private functions

	bool List::isIncludingThisList( char *queryListString )
		{
		unsigned short index = 0;

		if( queryListString != NULL &&
		strlen( queryListString ) > 0 )
			{
			while( index < strlen( queryListString ) &&
			queryListString[index] != listChar_ )
				index++;

			if( index == strlen( queryListString ) )
				return false;
			}

		return true;
		}

	Item *List::firstDeletedItem()
		{
		return deletedList_;
		}


	// Private cleanup functions

	unsigned int List::highestCurrentSentenceItemNr( unsigned int currentSentenceNr, Item *searchItem )
		{
		unsigned int highestItemNr = NO_ITEM_NR;
		unsigned int tempItemNr;

		while( searchItem != NULL )
			{
			if( searchItem->creationSentenceNr() == currentSentenceNr &&
			( tempItemNr = searchItem->itemNr() ) > highestItemNr )
				highestItemNr = tempItemNr;

			searchItem = searchItem->nextItem;
			}

		return highestItemNr;
		}

	unsigned int List::highestFoundSentenceNrInList( unsigned int maxSentenceNr, Item *searchItem )
		{
		unsigned int highestFoundSentenceNr = NO_SENTENCE_NR;
		unsigned int tempSentenceNr;

		while( searchItem != NULL &&
		highestFoundSentenceNr < maxSentenceNr )
			{
			tempSentenceNr = searchItem->activeSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem->inactiveSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem->archivedSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem->replacedSentenceNr();

			if( tempSentenceNr > highestFoundSentenceNr &&
			tempSentenceNr <= maxSentenceNr )
				highestFoundSentenceNr = tempSentenceNr;

			searchItem = searchItem->nextItem;
			}

		return highestFoundSentenceNr;
		}

	signed char List::decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset, Item *searchItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementItemNrRange";

		while( searchItem != NULL )
			{
			if( searchItem->creationSentenceNr() == decrementSentenceNr &&
			searchItem->itemNr() >= startDecrementItemNr )
				{
				if( searchItem->itemNr() <= startDecrementItemNr )
					return startError( functionNameString, "I found an item number equal to - or lower than - the given start item number" );

				if( searchItem->decrementItemNr( decrementOffset ) != RESULT_OK )
					return addError( functionNameString, "I failed to decrement the item number of an item with a certain offset" );
				}

			searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	signed char List::decrementSentenceNrs( unsigned int startSentenceNr, Item *searchItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementSentenceNrs";

		while( searchItem != NULL )
			{
			if( searchItem->activeSentenceNr() >= startSentenceNr )
				{
				if( searchItem->activeSentenceNr() <= startSentenceNr )
					return startError( functionNameString, "I found an item with an active sentence number equal to the given start sentence number ", startSentenceNr );

				if( searchItem->decrementActiveSentenceNr() != RESULT_OK )
					return addError( functionNameString, "I failed to decrement the active sentence number of an item" );
				}

			if( searchItem->inactiveSentenceNr() >= startSentenceNr )
				{
				if( searchItem->inactiveSentenceNr() <= startSentenceNr )
					return startError( functionNameString, "I found an item with an inactive sentence number equal to the given start sentence number ", startSentenceNr );

				if( searchItem->decrementInactiveSentenceNr() != RESULT_OK )
					return addError( functionNameString, "I failed to decrement the inactive sentence number of an item" );
				}

			if( searchItem->originalSentenceNr() >= startSentenceNr )
				{
				if( searchItem->originalSentenceNr() <= startSentenceNr )
					return startError( functionNameString, "I found an item with an original sentence number equal to the given start sentence number ", startSentenceNr );

				if( searchItem->decrementOriginalSentenceNr() != RESULT_OK )
					return addError( functionNameString, "I failed to decrement the original sentence number of an item" );
				}

			if( searchItem->creationSentenceNr() >= startSentenceNr )
				{
				if( searchItem->creationSentenceNr() <= startSentenceNr )
					return startError( functionNameString, "I found an item with a creation sentence number equal to the given start sentence number ", startSentenceNr );

				if( searchItem->decrementCreationSentenceNr() != RESULT_OK )
					return addError( functionNameString, "I failed to decrement the creation sentence number of an item" );
				}

			if( searchItem->archivedSentenceNr() >= startSentenceNr )
				{
				if( searchItem->archivedSentenceNr() <= startSentenceNr )
					return startError( functionNameString, "I found an item with an archived sentence number equal to the given start sentence number ", startSentenceNr );

				if( searchItem->decrementArchivedSentenceNr() != RESULT_OK )
					return addError( functionNameString, "I failed to decrement the archived sentence number of an item" );
				}

			if( searchItem->replacedSentenceNr() >= startSentenceNr )
				{
				if( searchItem->replacedSentenceNr() <= startSentenceNr )
					return startError( functionNameString, "I found an item with a replaced sentence number equal to the given start sentence number ", startSentenceNr );

				if( searchItem->decrementReplacedSentenceNr() != RESULT_OK )
					return addError( functionNameString, "I failed to decrement the replaced sentence number of an item" );
				}

			searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	signed char List::removeItemFromList( Item *removeItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeItemFromList";

		if( removeItem == NULL )
			return startError( functionNameString, "The given remove item is undefined" );

		if( removeItem->myList() != this )
			return startError( functionNameString, "The given remove item doesn't belong to my list" );

		// First item in list
		if( removeItem->previousItem == NULL )
			{
			switch( removeItem->statusChar() )
				{
				case QUERY_ACTIVE_CHAR:
					activeList_ = removeItem->nextItem;
					break;

				case QUERY_INACTIVE_CHAR:
					inactiveList_ = removeItem->nextItem;
					break;

				case QUERY_ARCHIVED_CHAR:
					archivedList_ = removeItem->nextItem;
					break;

				case QUERY_REPLACED_CHAR:
					replacedList_ = removeItem->nextItem;
					break;

				case QUERY_DELETED_CHAR:
					deletedList_ = removeItem->nextItem;
					break;

				default:
					return startError( functionNameString, "The given remove item has an unknown status character" );
				}

			if( removeItem->nextItem != NULL )
				removeItem->nextItem->previousItem = NULL;
			}
		else
			{
			removeItem->previousItem->nextItem = removeItem->nextItem;

			if( removeItem->nextItem != NULL )
				removeItem->nextItem->previousItem = removeItem->previousItem;
			}

		// Remember next item
		nextListItem_ = removeItem->nextItem;

		// Disconnect item from the list
		removeItem->previousItem = NULL;
		removeItem->nextItem = NULL;

		return RESULT_OK;
		}


	// Private database connection functions

/*
	signed char List::storeChangesInFutureDatabase( Item *searchItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() &&
			searchItem->storeChangesInFutureDatabase() != RESULT_OK )
				return addError( functionNameString, "I failed to store the changes of an item in the database" );

			searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}
*/


	// Private query functions

	void List::clearQuerySelections( Item *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->isSelectedByQuery = false;
			searchItem->isSelectedByJustificationQuery = false;

			searchItem = searchItem->nextItem;
			}
		}

	void List::itemQuery( bool isSelectingOnFind, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, Item *queryItem )
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
					globalVariables_->hasFoundQuery = true;
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

	void List::listQuery( bool isSelectingOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectingOnFind )
				{
				if( !queryItem->isSelectedByQuery )
					{
					globalVariables_->hasFoundQuery = true;
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

	void List::parameterQuery( bool isSelectingOnFind, unsigned int queryParameter, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasParameter( queryParameter ) )
				{
				if( isSelectingOnFind &&
				!queryItem->isSelectedByQuery )
					{
					globalVariables_->hasFoundQuery = true;
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

	void List::wordQuery( bool isSelectingOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectingOnFind )
				{
				if( !queryItem->isSelectedByQuery )
					{
					globalVariables_->hasFoundQuery = true;
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

	void List::wordTypeQuery( bool isSelectingOnFind, unsigned short queryWordTypeNr, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasWordType( queryWordTypeNr ) )
				{
				if( isSelectingOnFind &&
				!queryItem->isSelectedByQuery )
					{
					globalVariables_->hasFoundQuery = true;
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

	signed char List::displayQueryResult( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, Item *queryItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displayQueryResult";

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
							if( inputOutput_->writeText( true, promptTypeNr, queryWidth, queryItem->itemToString( queryWordTypeNr ) ) != RESULT_OK )
								return addError( functionNameString, "I failed to display the info of an active item" );
							}
						}
					}
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}

	signed char List::stringQuery( bool isSelectingOnFind, char *wordString, Item *queryItem )
		{
		bool hasFoundString;
		char *itemString;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "stringQuery";

		while( queryItem != NULL )
			{
			hasFoundString = false;

			if( ( itemString = queryItem->itemString() ) != NULL )
				{
				if( ( boolResult = compareStrings( wordString, itemString ) ).result != RESULT_OK )
					return addError( functionNameString, "I failed to compare two strings" );

				hasFoundString = boolResult.booleanValue;
				}

			if( hasFoundString )
				{
				if( isSelectingOnFind &&
				!queryItem->isSelectedByQuery )
					{
					globalVariables_->hasFoundQuery = true;
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

	signed char List::wordReferenceQuery( bool isSelectingOnFind, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString, Item *queryItem )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "wordReferenceQuery";

		while( queryItem != NULL )
			{
			if( ( boolResult = queryItem->findMatchingWordReferenceString( wordReferenceNameString ) ).result != RESULT_OK )
				return addError( functionNameString, "I failed to check the word references" );

			// Matching string
			if( boolResult.booleanValue )
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
						globalVariables_->hasFoundQuery = true;
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


	// Constructor

	List::List()
		{
		}

	List::~List(){}


	// Protected error functions

	signed char List::addError( const char *functionNameString, const char *errorString )
		{
		if( inputOutput_ == NULL )
			fprintf( stderr, "\nClass:\t%s\nParent class:\t%s\nFunction:\t%s\nError:\t\t%s.\n", classNameString_, parentClassNameString_, functionNameString, errorString );
		else
			inputOutput_->displayError( listChar_, classNameString_, parentClassNameString_, ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), functionNameString, errorString );

		return ( globalVariables_ == NULL ? RESULT_ERROR : globalVariables_->result );
		}

	signed char List::addError( const char *functionNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		char errorString[ERROR_STRING_LENGTH];
		sprintf( errorString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, errorString );
		}

	signed char List::startError( const char *functionNameString, const char *errorString )
		{
		addError( functionNameString, errorString );

		if( globalVariables_ != NULL )
		globalVariables_->result = RESULT_ERROR;
		return RESULT_ERROR;
		}

	signed char List::startError( const char *functionNameString, const char *_errorString, unsigned int errorSentenceNr )
		{
		char errorString[ERROR_STRING_LENGTH];
		sprintf( errorString, "%s%u", _errorString, errorSentenceNr );
		return startError( functionNameString, errorString );
		}

	signed char List::startError( const char *functionNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		char errorString[ERROR_STRING_LENGTH];
		sprintf( errorString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, errorString );
		}

	signed char List::startSystemError( const char *functionNameString, const char *errorString )
		{
		addError( functionNameString, errorString );

		if( globalVariables_ != NULL )
		globalVariables_->result = RESULT_SYSTEM_ERROR;

		return RESULT_SYSTEM_ERROR;
		}

	BoolResultType List::addBoolResultError( const char *functionNameString, const char *errorString )
		{
		BoolResultType interpretationResult;

		interpretationResult.result = addError( functionNameString, errorString );
		return interpretationResult;
		}

	BoolResultType List::startBoolResultError( const char *functionNameString, const char *errorString )
		{
		BoolResultType boolResult;

		boolResult.result = startError( functionNameString, errorString );
		return boolResult;
		}

	CollectionResultType List::addCollectionResultError( const char *functionNameString, const char *errorString )
		{
		CollectionResultType collectionResult;

		collectionResult.result = addError( functionNameString, errorString );
		return collectionResult;
		}

	CollectionResultType List::startCollectionResultError( const char *functionNameString, const char *errorString )
		{
		CollectionResultType collectionResult;

		collectionResult.result = startError( functionNameString, errorString );
		return collectionResult;
		}

	CollectionResultType List::startCollectionResultError( const char *functionNameString, const char *errorString, unsigned int number )
		{
		CollectionResultType collectionResult;

		collectionResult.result = startError( functionNameString, errorString, number );
		return collectionResult;
		}

	CollectionResultType List::startCollectionResultSystemError( const char *functionNameString, const char *errorString )
		{
		CollectionResultType collectionResult;

		collectionResult.result = startSystemError( functionNameString, errorString );
		return collectionResult;
		}

	CreateAndAssignResultType List::addCreateAndAssignResultError( const char *functionNameString, const char *errorString )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = addError( functionNameString, errorString );
		return createAndAssignResult;
		}

	CreateAndAssignResultType List::startCreateAndAssignResultError( const char *functionNameString, const char *errorString )
		{
		CreateAndAssignResultType createAndAssignResult;

		createAndAssignResult.result = startError( functionNameString, errorString );
		return createAndAssignResult;
		}

	DuplicateResultType List::addDuplicateResultError( const char *functionNameString, const char *errorString )
		{
		DuplicateResultType duplicateResult;

		duplicateResult.result = addError( functionNameString, errorString );
		return duplicateResult;
		}

	DuplicateResultType List::startDuplicateResultError( const char *functionNameString, const char *errorString )
		{
		DuplicateResultType duplicateResult;

		duplicateResult.result = startError( functionNameString, errorString );
		return duplicateResult;
		}

	FileResultType List::addFileResultError( const char *functionNameString, const char *errorString )
		{
		FileResultType fileResult;

		fileResult.result = addError( functionNameString, errorString );
		return fileResult;
		}

	FileResultType List::startFileResultError( const char *functionNameString, const char *errorString )
		{
		FileResultType fileResult;

		fileResult.result = startError( functionNameString, errorString );
		return fileResult;
		}

	FileResultType List::startFileResultError( const char *functionNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		FileResultType fileResult;

		fileResult.result = startError( functionNameString, errorString1, errorString2, errorString3 );
		return fileResult;
		}

	GrammarResultType List::addGrammarResultError( const char *functionNameString, const char *errorString )
		{
		GrammarResultType grammarResult;

		grammarResult.result = addError( functionNameString, errorString );
		return grammarResult;
		}

	GrammarResultType List::startGrammarResultError( const char *functionNameString, const char *errorString )
		{
		GrammarResultType grammarResult;

		grammarResult.result = startError( functionNameString, errorString );
		return grammarResult;
		}

	JustificationResultType List::addJustificationResultError( const char *functionNameString, const char *errorString )
		{
		JustificationResultType justificationResult;

		justificationResult.result = addError( functionNameString, errorString );
		return justificationResult;
		}

	JustificationResultType List::startJustificationResultError( const char *functionNameString, const char *errorString )
		{
		JustificationResultType justificationResult;

		justificationResult.result = startError( functionNameString, errorString );
		return justificationResult;
		}

	SelectionResultType List::addSelectionResultError( const char *functionNameString, const char *errorString )
		{
		SelectionResultType selectionResult;

		selectionResult.result = addError( functionNameString, errorString );
		return selectionResult;
		}

	SelectionResultType List::startSelectionResultSystemError( const char *functionNameString, const char *errorString )
		{
		SelectionResultType selectionResult;

		selectionResult.result = startSystemError( functionNameString, errorString );
		return selectionResult;
		}

	ShortResultType List::startShortResultError( const char *functionNameString, const char *errorString )
		{
		ShortResultType shortResult;

		shortResult.result = startError( functionNameString, errorString );
		return shortResult;
		}

	WordEndingResultType List::startWordEndingResultError( const char *functionNameString, const char *errorString )
		{
		WordEndingResultType wordEndingResult;

		wordEndingResult.result = startError( functionNameString, errorString );
		return wordEndingResult;
		}

	WordResultType List::addWordResultError( const char *functionNameString, const char *errorString )
		{
		WordResultType wordResult;

		wordResult.result = addError( functionNameString, errorString );
		return wordResult;
		}

	WordResultType List::startWordResultError( const char *functionNameString, const char *errorString )
		{
		WordResultType wordResult;

		wordResult.result = startError( functionNameString, errorString );
		return wordResult;
		}

	WordTypeResultType List::addWordTypeResultError( const char *functionNameString, const char *errorString )
		{
		WordTypeResultType wordTypeResult;

		wordTypeResult.result = addError( functionNameString, errorString );
		return wordTypeResult;
		}

	WordTypeResultType List::startWordTypeResultError( const char *functionNameString, const char *errorString )
		{
		WordTypeResultType wordTypeResult;

		wordTypeResult.result = startError( functionNameString, errorString );
		return wordTypeResult;
		}


	// Protected virtual functions

	bool List::isTemporaryList()
		{
		return false;
		}


	// Protected common functions

	void List::initializeListVariables( char listChar, const char *classNameString, GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private initialized variables

		listChar_ = listChar;

		// Checking private initialized variables

		if( classNameString == NULL )
			strcpy( errorString, "The given class name string is undefined" );
		else
			strcpy( classNameString_, classNameString );

		if( ( globalVariables_ = globalVariables ) == NULL )
			strcpy( errorString, "The given global variables is undefined" );

		if( ( inputOutput_ = inputOutput ) == NULL )
			strcpy( errorString, "The given input-output is undefined" );

		if( ( myWordItem_ = myWordItem ) == NULL )
			strcpy( errorString, "The given my word item is undefined" );

		if( strlen( errorString ) > 0 )
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
		}

	void List::deleteTemporaryList()
		{
		Item *searchItem = deletedList_;

		// ReadList may have inactive items
		if( inactiveList_ != NULL )
			{
			// Move inactive list to deleted list
			if( searchItem == NULL )
				deletedList_ = inactiveList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem->tailOfList();
				searchItem->nextItem = inactiveList_;
				}

			inactiveList_ = NULL;
			}

		// Temporary lists have active items
		if( activeList_ != NULL )
			{
			// Move active list to deleted list
			if( searchItem == NULL )
				deletedList_ = activeList_;
			else
				{
				// Get the tail of the deleted list
				searchItem = searchItem->tailOfList();
				searchItem->nextItem = activeList_;
				}

			activeList_ = NULL;
			}
		}

	bool List::hasActiveItems()
		{
		return ( activeList_ != NULL );
		}

	bool List::isAdminList()
		{
		return ( isupper( listChar_ ) != 0 );
		}

	bool List::isAssignmentList()
		{
		return ( listChar_ == WORD_ASSIGNMENT_LIST_SYMBOL );
		}

	unsigned int List::highestSentenceNrInList()
		{
		return highestSentenceNrInList_;
		}

	char List::listChar()
		{
		return listChar_;
		}

	signed char List::activateItem( Item *activateItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "activateItem";

		if( activateItem == NULL )
			return startError( functionNameString, "The given activate item is undefined" );

		if( activateItem->statusChar() == QUERY_ACTIVE_CHAR )
			return startError( functionNameString, "The given activate item is already an active item" );

		if( removeItemFromList( activateItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to remove an item from the archive list" );

		if( addItemToList( QUERY_ACTIVE_CHAR, activateItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an item to the active list" );

		if( isAssignmentList() &&
		globalVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
			globalVariables_->isAssignmentChanged = true;

		return RESULT_OK;
		}

	signed char List::addItemToList( char statusChar, Item *newItem )
		{
		unsigned int creationSentenceNr;
		Item *previousSearchItem = NULL;
		Item *searchItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addItemToList";

		if( newItem == NULL )
			return startError( functionNameString, "The given new item is undefined" );

		if( newItem->myList() != this )
			return startError( functionNameString, "The given new item doesn't belong to my list" );

		if( newItem->nextItem != NULL )
			return startError( functionNameString, "The given new item seems to be a part of a list" );

		switch( statusChar )
			{
			case QUERY_ACTIVE_CHAR:
				newItem->setActiveSentenceNr();
				newItem->setActiveStatus();
				searchItem = activeList_;
				break;

			case QUERY_INACTIVE_CHAR:
				newItem->setInactiveSentenceNr();
				newItem->setInactiveStatus();
				searchItem = inactiveList_;
				break;

			case QUERY_ARCHIVED_CHAR:
				newItem->setArchivedSentenceNr();
				newItem->setArchivedStatus();
				searchItem = archivedList_;
				break;

			case QUERY_REPLACED_CHAR:
				newItem->setReplacedSentenceNr();
				newItem->setReplacedStatus();
				searchItem = replacedList_;
				break;

			case QUERY_DELETED_CHAR:
				newItem->setDeletedStatus();
				searchItem = deletedList_;
				break;

			default:
				return startError( functionNameString, "The given status character is unknown" );
			}

		creationSentenceNr = newItem->creationSentenceNr();

		// Sort item in list
		if( statusChar == QUERY_DELETED_CHAR )
			{
			while( searchItem != NULL &&

			// 1) Descending creationSentenceNr
			( searchItem->creationSentenceNr() > creationSentenceNr ||

			// 2) Ascending itemNr
			( searchItem->creationSentenceNr() == creationSentenceNr &&
			searchItem->itemNr() < newItem->itemNr() ) ) )
				{
				previousSearchItem = searchItem;
				searchItem = searchItem->nextItem;
				}
			}
		else
			{
			if( creationSentenceNr > highestSentenceNrInList_ )
				{
				highestSentenceNrInList_ = creationSentenceNr;

				if( myWordItem_ != NULL )
					myWordItem_->setHighestSentenceNr( creationSentenceNr );
				}

			while( searchItem != NULL &&
			!newItem->isSorted( searchItem ) )
				{
				previousSearchItem = searchItem;
				searchItem = searchItem->nextItem;
				}
			}

		if( searchItem != NULL &&
		// Check on duplicates
		searchItem->creationSentenceNr() == creationSentenceNr &&
		// for integrity
		searchItem->itemNr() == newItem->itemNr() )
			return startError( functionNameString, "I found another item with the same identification" );

		newItem->previousItem = previousSearchItem;

		// First item in list
		if( previousSearchItem == NULL )
			{
			switch( statusChar )
				{
				case QUERY_ACTIVE_CHAR:
					if( activeList_ != NULL )
						activeList_->previousItem = newItem;

					newItem->nextItem = activeList_;
					activeList_ = newItem;
					break;

				case QUERY_INACTIVE_CHAR:
					if( inactiveList_ != NULL )
						inactiveList_->previousItem = newItem;

					newItem->nextItem = inactiveList_;
					inactiveList_ = newItem;
					break;

				case QUERY_ARCHIVED_CHAR:
					if( archivedList_ != NULL )
						archivedList_->previousItem = newItem;

					newItem->nextItem = archivedList_;
					archivedList_ = newItem;
					break;

				case QUERY_REPLACED_CHAR:
					if( replacedList_ != NULL )
						replacedList_->previousItem = newItem;

					newItem->nextItem = replacedList_;
					replacedList_ = newItem;
					break;

				case QUERY_DELETED_CHAR:
					if( deletedList_ != NULL )
						deletedList_->previousItem = newItem;

					newItem->nextItem = deletedList_;
					deletedList_ = newItem;
					break;

				default:
					return startError( functionNameString, "The given status character is unknown" );
				}
			}
		else
			{
			if( searchItem != NULL )
				searchItem->previousItem = newItem;

			newItem->nextItem = previousSearchItem->nextItem;
			previousSearchItem->nextItem = newItem;
			}

		return RESULT_OK;
		}

	signed char List::archiveItem( Item *archiveItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "archiveItem";

		if( archiveItem == NULL )
			return startError( functionNameString, "The given archive item is undefined" );

		if( archiveItem->statusChar() == QUERY_ARCHIVED_CHAR )
			return startError( functionNameString, "The given archive item is already an archived item" );

		if( !isAssignmentList() )
			return startError( functionNameString, "Only assignments can be archived" );

		if( removeItemFromList( archiveItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to remove an item from a list" );

		archiveItem->previousStatusChar = archiveItem->statusChar();

		if( addItemToList( QUERY_ARCHIVED_CHAR, archiveItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an item to the archived list" );

		if( globalVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
			globalVariables_->isAssignmentChanged = true;

		return RESULT_OK;
		}

	signed char List::deleteItem( Item *deleteItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteItem";

		if( removeItemFromList( deleteItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to remove an item from a list" );

		deleteItem->previousStatusChar = deleteItem->statusChar();

		if( deleteItem->statusChar() == QUERY_DELETED_CHAR )
			return startError( functionNameString, "The given delete item is already a deleted item" );

		if( addItemToList( QUERY_DELETED_CHAR, deleteItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an item to the deleted list" );

		return RESULT_OK;
		}

	signed char List::inactivateItem( Item *inactivateItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "inactivateItem";

		if( inactivateItem == NULL )
			return startError( functionNameString, "The given inactivate item is undefined" );

		if( inactivateItem->statusChar() == QUERY_INACTIVE_CHAR )
			return startError( functionNameString, "The given inactivate item is already an inactive item" );

		if( listChar_ != ADMIN_READ_LIST_SYMBOL &&
		listChar_ != WORD_ASSIGNMENT_LIST_SYMBOL )
			return startError( functionNameString, "Only assignments and read items can be inactived" );

		if( removeItemFromList( inactivateItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to remove an item from the archive list" );

		if( addItemToList( QUERY_INACTIVE_CHAR, inactivateItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an item to the inactive list" );

		if( isAssignmentList() &&
		globalVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
			globalVariables_->isAssignmentChanged = true;

		return RESULT_OK;
		}

	signed char List::removeFirstRangeOfDeletedItemsInList()
		{
		unsigned int nDeletedItems = 0;
		unsigned int removeSentenceNr;
		unsigned int removeStartItemNr;
		Item *removeItem = deletedList_;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeFirstRangeOfDeletedItemsInList";

		if( removeItem != NULL )
			{
			if( globalVariables_->nDeletedItems != 0 ||
			globalVariables_->removeSentenceNr > NO_SENTENCE_NR ||
			globalVariables_->removeStartItemNr > NO_ITEM_NR )
				return startError( functionNameString, "There is already a range of deleted items" );

			removeSentenceNr = removeItem->creationSentenceNr();
			removeStartItemNr = removeItem->itemNr();

			do	{
				// Disconnect deleted list from item
				deletedList_ = deletedList_->nextItem;

				removeItem->checkForUsage();

				// Disconnect item from the deleted list
				removeItem->nextItem = NULL;
				delete removeItem;

				removeItem = deletedList_;
				nDeletedItems++;
				}
			while( removeItem != NULL &&
			// Same sentence number
			removeItem->creationSentenceNr() == removeSentenceNr &&
			// Ascending item number
			removeItem->itemNr() == removeStartItemNr + nDeletedItems );

			globalVariables_->nDeletedItems = nDeletedItems;
			globalVariables_->removeSentenceNr = removeSentenceNr;
			globalVariables_->removeStartItemNr = removeStartItemNr;
			}

		return RESULT_OK;
		}

	signed char List::replaceItem( Item *replaceItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceItem";

		if( replaceItem == NULL )
			return startError( functionNameString, "The given replace item is undefined" );

		if( replaceItem->statusChar() == QUERY_REPLACED_CHAR )
			return startError( functionNameString, "The given replace item is already a replaced item" );

		if( removeItemFromList( replaceItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to remove an item from a list" );

		replaceItem->previousStatusChar = replaceItem->statusChar();

		if( addItemToList( QUERY_REPLACED_CHAR, replaceItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an item to the replaced list" );

		return RESULT_OK;
		}

	Item *List::firstActiveItem()
		{
		return activeList_;
		}

	Item *List::firstInactiveItem()
		{
		return inactiveList_;
		}

	Item *List::firstArchivedItem()
		{
		return archivedList_;
		}

	Item *List::firstReplacedItem()
		{
		return replacedList_;
		}

	Item *List::nextListItem()
		{
		return nextListItem_;
		}

	GlobalVariables *List::globalVariables()
		{
		return globalVariables_;
		}

	InputOutput *List::inputOutput()
		{
		return inputOutput_;
		}

	WordItem *List::myWordItem()
		{
		return myWordItem_;
		}

	BoolResultType List::compareStrings( char *searchString, char *sourceString )
		{
		bool hasFoundMatchingString = true;
		bool isStop;
		size_t searchStringPosition = 0;
		size_t sourceStringPosition = 0;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "compareStrings";

		if( searchString == NULL )
			return startBoolResultError( functionNameString, "The given search string is undefined" );

		if( sourceString == NULL )
			return startBoolResultError( functionNameString, "The given source string is undefined" );

		if( searchString == sourceString )
			return startBoolResultError( functionNameString, "The given strings are the same string" );

		while( hasFoundMatchingString &&
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
								if( ( boolResult = compareStrings( &searchString[searchStringPosition], &sourceString[sourceStringPosition] ) ).result != RESULT_OK )
									return addBoolResultError( functionNameString, "I failed to compare the remaining strings" );

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
						sourceStringPosition = strlen( sourceString );
					}
				else
					hasFoundMatchingString = false;
				}
			}

		if( hasFoundMatchingString &&
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
			hasFoundMatchingString = false;

		boolResult.booleanValue = hasFoundMatchingString;
		return boolResult;
		}


	// Protected cleanup functions

	unsigned int List::highestCurrentSentenceItemNrInList( unsigned int currentSentenceNr, unsigned int globalHighestItemNr )
		{
		unsigned int localHighestItemNr = NO_ITEM_NR;
		unsigned int tempItemNr;
		Item *searchItem;

		if( ( searchItem = firstActiveItem() ) != NULL )
			localHighestItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem );

		if( ( searchItem = firstInactiveItem() ) != NULL &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( ( searchItem = firstArchivedItem() ) != NULL &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( ( searchItem = firstReplacedItem() ) != NULL &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( ( searchItem = firstDeletedItem() ) != NULL &&
		( tempItemNr = highestCurrentSentenceItemNr( currentSentenceNr, searchItem ) ) > localHighestItemNr )
			localHighestItemNr = tempItemNr;

		if( localHighestItemNr >= globalHighestItemNr )
			return localHighestItemNr;

		return globalHighestItemNr;
		}

	unsigned int List::highestFoundSentenceNrInList( bool isIncludingDeletedItems, unsigned int globalHighestFoundSentenceNr, unsigned int maxSentenceNr )
		{
		unsigned int localHighestFoundSentenceNr = NO_SENTENCE_NR;
		unsigned int tempSentenceNr;
		Item *searchItem;

		if( ( searchItem = firstActiveItem() ) != NULL )
			localHighestFoundSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem );

		if( ( searchItem = firstInactiveItem() ) != NULL &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( ( searchItem = firstArchivedItem() ) != NULL &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( ( searchItem = firstReplacedItem() ) != NULL &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( isIncludingDeletedItems &&
		( searchItem = firstDeletedItem() ) != NULL &&
		( tempSentenceNr = highestFoundSentenceNrInList( maxSentenceNr, searchItem ) ) > localHighestFoundSentenceNr )
			localHighestFoundSentenceNr = tempSentenceNr;

		if( localHighestFoundSentenceNr >= globalHighestFoundSentenceNr )
			return localHighestFoundSentenceNr;

		return globalHighestFoundSentenceNr;
		}

	signed char List::decrementItemNrRangeInList( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		Item *searchItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementItemNrRangeInList";

		if( decrementSentenceNr < NO_SENTENCE_NR )
			return startError( functionNameString, "The given decrement sentence number is undefined" );

		if( startDecrementItemNr <= NO_ITEM_NR )
			return startError( functionNameString, "The given start item number is undefined" );

		if( decrementOffset <= 0 )
			return startError( functionNameString, "The given decrement offset is undefined" );

		if( ( searchItem = firstActiveItem() ) != NULL )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstInactiveItem() ) != NULL )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstArchivedItem() ) != NULL )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstReplacedItem() ) != NULL )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstDeletedItem() ) != NULL )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		// Clear error to be able to restart after error
		return RESULT_OK;
		}

	signed char List::decrementSentenceNrsInList( unsigned int startSentenceNr )
		{
		Item *searchItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "decrementSentenceNrsInList";

		if( startSentenceNr <= NO_SENTENCE_NR )
			return startError( functionNameString, "The given start sentence number is undefined" );

		if( highestSentenceNrInList_ == startSentenceNr )
			highestSentenceNrInList_--;

		if( ( searchItem = firstActiveItem() ) != NULL )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstInactiveItem() ) != NULL )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstArchivedItem() ) != NULL )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstReplacedItem() ) != NULL )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstDeletedItem() ) != NULL )
			decrementSentenceNrs( startSentenceNr, searchItem );

		return RESULT_OK;
		}

	signed char List::deleteSentencesInList( unsigned int lowestSentenceNr )
		{
		bool isResultOK = ( globalVariables_->result == RESULT_OK );
		Item *searchItem = firstActiveItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteSentencesInList";

		if( lowestSentenceNr <= NO_SENTENCE_NR )
			return startError( functionNameString, "The given lowest sentence number is undefined" );

		while( searchItem != NULL )
			{
			if( searchItem->activeSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem->creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete an active item" );

					searchItem = nextListItem_;
					}
				else
					{
					if( searchItem->wasInactiveBefore() )
						{
						if( inactivateItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to inactivate an active item" );

						searchItem = nextListItem_;
						}
					else
						searchItem = searchItem->nextItem;
					}
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->inactiveSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem->creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete an inactive item" );
					}
				else
					{
					if( activateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to activate an inactive item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->archivedSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem->creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete an archived item" );
					}
				else
					{
					searchItem->clearArchivedSentenceNr();

					if( searchItem->wasActiveBefore() )
						{
						if( activateItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to activate an archived item" );
						}
					else
						{
						if( !searchItem->wasInactiveBefore() )
							return startError( functionNameString, "I don't know what to do with an archived item that wasn't active before, nor inactive" );

						if( inactivateItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to inactivate an archived item" );
						}
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->replacedSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem->creationSentenceNr() >= lowestSentenceNr )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete a replaced item" );
					}
				else
					{
					if( searchItem->wasActiveBefore() )
						{
						if( activateItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to activate a replaced item" );
						}
					else
						{
						if( searchItem->wasInactiveBefore() )
							{
							if( inactivateItem( searchItem ) != RESULT_OK )
								return addError( functionNameString, "I failed to inactivate a replaced item" );
							}
						else
							{
							if( !searchItem->wasArchivedBefore() )
								return startError( functionNameString, "I don't know what to do with a replaced item that wasn't inactive before, nor archived" );

							if( archiveItem( searchItem ) != RESULT_OK )
								return addError( functionNameString, "I failed to archive a replaced item" );
							}
						}
					}

				searchItem = nextListItem_;
				}
			else
				{
				if( !isResultOK &&
				searchItem->replacedSentenceNr() == lowestSentenceNr - 1 )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete a replaced item" );

					searchItem = nextListItem_;
					}
				else
					searchItem = searchItem->nextItem;
				}
			}

		return RESULT_OK;
		}

	signed char List::redoCurrentSentenceInList()
		{
		Item *searchItem = firstActiveItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "redoCurrentSentenceInList";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentInactiveSentenceNr() )
				{
				if( inactivateItem( searchItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to inactivate an active item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem->hasCurrentArchivedSentenceNr() )
					{
					if( archiveItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to archive an active item" );

					searchItem = nextListItem_;
					}
				else
					{
					if( searchItem->hasCurrentReplacedSentenceNr() )
						{
						if( replaceItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to replace an active item" );

						searchItem = nextListItem_;
						}
					else
						searchItem = searchItem->nextItem;
					}
				}
			}

		searchItem = firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( activateItem( searchItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to activate an inactive item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem->hasCurrentArchivedSentenceNr() )
					{
					if( archiveItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to archive an inactive item" );

					searchItem = nextListItem_;
					}
				else
					searchItem = searchItem->nextItem;
				}
			}

		searchItem = firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( activateItem( searchItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to activate an archived item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem->hasCurrentInactiveSentenceNr() )
					{
					if( inactivateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to inactivate an archived item" );

					searchItem = nextListItem_;
					}
				else
					searchItem = searchItem->nextItem;
				}
			}

		searchItem = firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( activateItem( searchItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to activate a replaced item" );

				searchItem = nextListItem_;
				}
			else
				{
				if( searchItem->hasCurrentInactiveSentenceNr() )
					{
					if( inactivateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to inactivate a replaced item" );

					searchItem = nextListItem_;
					}
				else
					{
					if( searchItem->hasCurrentArchivedSentenceNr() )
						{
						if( archiveItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to archive a replaced item" );

						searchItem = nextListItem_;
						}
					else
						searchItem = searchItem->nextItem;
					}
				}
			}

		searchItem = firstDeletedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( activateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to activate a deleted item" );
					}
				else
					{
					if( searchItem->wasInactiveBefore() )
						{
						if( inactivateItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to inactivate a deleted item" );
						}
					else
						{
						if( !searchItem->wasArchivedBefore() )
							return startError( functionNameString, "I don't know what to do with a deleted item that wasn't inactive before, nor archived" );

						if( archiveItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to inactivate a deleted item" );
						}
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}

	signed char List::undoCurrentSentenceInList()
		{
		Item *searchItem = firstActiveItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "undoCurrentSentenceInList";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentActiveSentenceNr() )
				{
				if( searchItem->hasCurrentCreationSentenceNr() )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete an active item" );
					}
				else
					{
					if( !searchItem->hasInactiveSentenceNr() )
						return startError( functionNameString, "I don't know what to do with an active item that isn't current, and wasn't inactive" );

					if( inactivateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to inactivate an active item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentInactiveSentenceNr() )
				{
				if( searchItem->hasCurrentCreationSentenceNr() )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete an inactive item" );
					}
				else
					{
					if( !searchItem->hasActiveSentenceNr() )
						return startError( functionNameString, "I don't know what to do with this inactive item isn't current, and wasn't active" );

					if( activateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to activate an inactive item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentArchivedSentenceNr() )
				{
				if( searchItem->hasCurrentCreationSentenceNr() )
					{
					if( deleteItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to delete an archived item" );
					}
				else
					{
					if( searchItem->wasActiveBefore() )
						{
						if( activateItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to activate an archived item" );
						}
					else
						{
						if( !searchItem->wasInactiveBefore() )
							return startError( functionNameString, "I don't know what to do with this archived item that wasn't inactive before, nor archived" );

						if( inactivateItem( searchItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to inactivate an archived item" );
						}
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		searchItem = firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentReplacedSentenceNr() )
				{
				if( searchItem->wasActiveBefore() )
					{
					if( activateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to activate a replaced item" );
					}
				else
					{
					if( !searchItem->wasInactiveBefore() )
						return startError( functionNameString, "I don't know what to do with this replaced item that wasn't active before, nor inactive" );

					if( inactivateItem( searchItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to inactivate a replaced item" );
					}

				searchItem = nextListItem_;
				}
			else
				searchItem = searchItem->nextItem;
			}

		return RESULT_OK;
		}


	// Protected database connection functions
/*
	signed char List::storeChangesInListInFutureDatabase()
		{
		Item *searchItem;

		if( ( searchItem = firstActiveItem() ) != NULL )
			storeChangesInFutureDatabase( searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstInactiveItem() ) != NULL )
			storeChangesInFutureDatabase( searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstArchivedItem() ) != NULL )
			storeChangesInFutureDatabase( searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstReplacedItem() ) != NULL )
			storeChangesInFutureDatabase( searchItem );

		return globalVariables_->result;
		}
*/

	// Protected query functions

	void List::clearQuerySelectionsInList()
		{
		Item *searchItem;

		if( ( searchItem = firstActiveItem() ) != NULL )
			clearQuerySelections( searchItem );

		if( ( searchItem = firstInactiveItem() ) != NULL )
			clearQuerySelections( searchItem );

		if( ( searchItem = firstArchivedItem() ) != NULL )
			clearQuerySelections( searchItem );

		if( ( searchItem = firstReplacedItem() ) != NULL )
			clearQuerySelections( searchItem );
		}

	void List::countQueryInList()
		{
		Item *searchItem = firstActiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				globalVariables_->nActiveQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				globalVariables_->nInactiveQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				globalVariables_->nArchivedQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				globalVariables_->nReplacedQueryItems++;

			searchItem = searchItem->nextItem;
			}
		}

	void List::itemQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != NULL )
			itemQuery( isSelectingOnFind, isReferenceQuery, querySentenceNr, queryItemNr, searchItem );
		}

	void List::listQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString )
		{
		bool isSelectingOnFindAndListIncluded;
		bool isListIncludedInQuery = isIncludingThisList( queryListString );
		Item *searchItem;

		if( isSelectingOnFind ||
		!isListIncludedInQuery )
			{
			isSelectingOnFindAndListIncluded = ( isSelectingOnFind &&
												isListIncludedInQuery );

			if( isSelectingActiveItems &&
			( searchItem = firstActiveItem() ) != NULL )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );

			if( isSelectingInactiveItems &&
			( searchItem = firstInactiveItem() ) != NULL )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );

			if( isSelectingArchivedItems &&
			( searchItem = firstArchivedItem() ) != NULL )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );

			if( isSelectingReplacedItems &&
			( searchItem = firstReplacedItem() ) != NULL )
				listQuery( isSelectingOnFindAndListIncluded, searchItem );
			}
		}

	void List::parameterQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != NULL )
			parameterQuery( isSelectingOnFind, queryParameter, searchItem );
		}

	void List::wordQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != NULL )
			wordQuery( isSelectingOnFind, searchItem );
		}

	void List::wordTypeQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );

		if( isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != NULL )
			wordTypeQuery( isSelectingOnFind, queryWordTypeNr, searchItem );
		}

	signed char List::displayQueryResultInList( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		Item *searchItem;

		if( ( searchItem = firstActiveItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstInactiveItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstArchivedItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		( searchItem = firstReplacedItem() ) != NULL )
			displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, searchItem );

		return globalVariables_->result;
		}

	signed char List::stringQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryString )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != NULL )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != NULL )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != NULL )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != NULL )
			stringQuery( isSelectingOnFind, queryString, searchItem );

		return globalVariables_->result;
		}

	signed char List::wordReferenceQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		Item *searchItem;

		if( isSelectingActiveItems &&
		( searchItem = firstActiveItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		isSelectingInactiveItems &&
		( searchItem = firstInactiveItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		isSelectingArchivedItems &&
		( searchItem = firstArchivedItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		if( globalVariables_->result == RESULT_OK &&
		isSelectingReplacedItems &&
		( searchItem = firstReplacedItem() ) != NULL )
			wordReferenceQuery( isSelectingOnFind, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString, searchItem );

		return globalVariables_->result;
		}

/*************************************************************************
 *	"But he rescues the poor from trouble
 *	and increases their families like flocks of sheep." (Psalm 107:41)
 *************************************************************************/
