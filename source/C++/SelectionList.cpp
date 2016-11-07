/*	Class:			SelectionList
 *	Parent class:	List
 *	Purpose:		To store selection items
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

#include "SelectionList.h"

	// Private selection functions

	unsigned int SelectionList::getLowerSentenceNr( unsigned int duplicateSentenceNr )
		{
		Item *searchWordItem = firstActiveItem();
		unsigned int lowerSentenceNr = NO_SENTENCE_NR;

		while( searchWordItem != NULL )
			{
			if( searchWordItem->activeSentenceNr() < duplicateSentenceNr &&
			searchWordItem->activeSentenceNr() > lowerSentenceNr )
				lowerSentenceNr = searchWordItem->activeSentenceNr();

			searchWordItem = searchWordItem->nextItem;
			}

		return lowerSentenceNr;
		}


	// Constructor

	SelectionList::SelectionList( char _listChar, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( _listChar, "SelectionList", commonVariables, myWordItem );
		}

	SelectionList::~SelectionList()
		{
		SelectionItem *deleteSelectionItem;
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != NULL )
			{
			deleteSelectionItem = searchSelectionItem;
			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			delete deleteSelectionItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class SelectionList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class SelectionList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class SelectionList has replaced items." );

		searchSelectionItem = (SelectionItem *)firstDeletedItem();

		while( searchSelectionItem != NULL )
			{
			deleteSelectionItem = searchSelectionItem;
			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			delete deleteSelectionItem;
			}
		}


	// Protected functions

	void SelectionList::clearConditionChecksForSolving( unsigned short selectionLevel, unsigned int conditionSentenceNr )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != NULL )
			{
			if( searchSelectionItem->selectionLevel() < selectionLevel &&
			searchSelectionItem->activeSentenceNr() == conditionSentenceNr )
				searchSelectionItem->isConditionCheckedForSolving = false;

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}
		}

	ResultType SelectionList::checkSelectionItemForUsage( SelectionItem *unusedSelectionItem )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSelectionItemForUsage";

		if( unusedSelectionItem == NULL )
			return startError( functionNameString, NULL, "The given unused justification item is undefined" );

		while( searchSelectionItem != NULL )
			{
			if( searchSelectionItem->nextExecutionItem() == unusedSelectionItem )
				return startError( functionNameString, NULL, "The reference selection item is still in use" );

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return RESULT_OK;
		}

	ResultType SelectionList::checkWordItemForUsage( WordItem *unusedWordItem )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, NULL, "The given unused word item is undefined" );

		while( searchSelectionItem != NULL )
			{
			if( searchSelectionItem->generalizationWordItem() == unusedWordItem )
				return startError( functionNameString, NULL, "The generalization word item is still in use" );

			if( searchSelectionItem->specificationWordItem() == unusedWordItem )
				return startError( functionNameString, NULL, "The specification word item is still in use" );

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return RESULT_OK;
		}
/*
	ResultType SelectionList::storeChangesInFutureDatabase()
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchSelectionItem != NULL )
			{
			if( searchSelectionItem->hasCurrentCreationSentenceNr() )
				{
				if( searchSelectionItem->storeFileItemInFutureDatabase( isCondition, isAction, isAlternative ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a selection item in the database" );
				}

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return RESULT_OK;
		}
*/
	SelectionResultType SelectionList::checkDuplicateCondition()
		{
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkDuplicateCondition";

		selectionResult.duplicateConditionSentenceNr = commonVariables()->currentSentenceNr;

		do	{
			if( ( selectionResult.duplicateConditionSentenceNr = getLowerSentenceNr( selectionResult.duplicateConditionSentenceNr ) ) > NO_SENTENCE_NR )
				{
				if( ( selectionResult = checkDuplicateSelectionPart( selectionResult.duplicateConditionSentenceNr ) ).result != RESULT_OK )
					return addSelectionResultError( functionNameString, NULL, "I failed to check if the alternative selection part is duplicate" );
				}
			}
		while( !selectionResult.hasFoundDuplicateSelection &&
		selectionResult.duplicateConditionSentenceNr > NO_SENTENCE_NR );

		return selectionResult;
		}

	SelectionResultType SelectionList::checkDuplicateSelectionPart( unsigned int duplicateConditionSentenceNr )
		{
		SelectionResultType selectionResult;
		SelectionItem *currentSelectionItem = NULL;
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkDuplicateSelectionPart";

		if( duplicateConditionSentenceNr <= NO_SENTENCE_NR )
			return startSelectionResultError( functionNameString, NULL, "The given duplicate sentence number is undefined" );

		if( duplicateConditionSentenceNr >= commonVariables()->currentSentenceNr )
			return startSelectionResultError( functionNameString, NULL, "The given duplicate sentence number is equal or higher than the current sentence number" );

		selectionResult.hasFoundDuplicateSelection = true;

		while( searchSelectionItem != NULL &&
		searchSelectionItem->activeSentenceNr() >= duplicateConditionSentenceNr )
			{
			if( searchSelectionItem->activeSentenceNr() == duplicateConditionSentenceNr )
				{
				currentSelectionItem = firstActiveSelectionItem();

				do	{
					if( currentSelectionItem->isAction() == searchSelectionItem->isAction() &&
					currentSelectionItem->isAssignedOrClear() == searchSelectionItem->isAssignedOrClear() &&
					currentSelectionItem->isNegative() == searchSelectionItem->isNegative() &&
					currentSelectionItem->isNewStart() == searchSelectionItem->isNewStart() &&
					currentSelectionItem->selectionLevel() == searchSelectionItem->selectionLevel() &&
					currentSelectionItem->generalizationWordItem() == searchSelectionItem->generalizationWordItem() &&
					currentSelectionItem->specificationWordItem() == searchSelectionItem->specificationWordItem() )
						{
						if( currentSelectionItem->specificationString() != NULL &&
						searchSelectionItem->specificationString() != NULL )
							selectionResult.hasFoundDuplicateSelection = ( strcmp( currentSelectionItem->specificationString(), searchSelectionItem->specificationString() ) == 0 );
						else
							{
							if( currentSelectionItem->specificationString() != NULL ||
							searchSelectionItem->specificationString() != NULL )
								selectionResult.hasFoundDuplicateSelection = false;
							}

						if( selectionResult.hasFoundDuplicateSelection )
							{
							currentSelectionItem = ( currentSelectionItem->nextSelectionItem() != NULL &&
													currentSelectionItem->nextSelectionItem()->hasCurrentCreationSentenceNr() ? currentSelectionItem->nextSelectionItem() : NULL );

							searchSelectionItem = ( searchSelectionItem->nextSelectionItem() != NULL &&
													searchSelectionItem->nextSelectionItem()->activeSentenceNr() == duplicateConditionSentenceNr ? searchSelectionItem->nextSelectionItem() : NULL );
							}
						else
							searchSelectionItem = NULL;
						}
					else
						{
						selectionResult.hasFoundDuplicateSelection = false;
						searchSelectionItem = NULL;
						}
					}
				while( selectionResult.hasFoundDuplicateSelection &&
				currentSelectionItem != NULL &&
				searchSelectionItem != NULL );
				}
			else
				searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return selectionResult;
		}

	SelectionResultType SelectionList::createSelectionItem( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionItem";

		if( generalizationWordTypeNr <= NO_WORD_TYPE_NR ||
		generalizationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startSelectionResultError( functionNameString, NULL, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= NO_WORD_TYPE_NR &&
		specificationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startSelectionResultError( functionNameString, NULL, "The given specification word type number is undefined or out of bounds" );

		if( ( selectionResult.lastCreatedSelectionItem = new SelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString, commonVariables(), this, myWordItem() ) ) == NULL )
			return startSelectionResultError( functionNameString, NULL, "I failed to create a selection item" );

		if( addItemToList( QUERY_ACTIVE_CHAR, selectionResult.lastCreatedSelectionItem ) != RESULT_OK )
			return addSelectionResultError( functionNameString, NULL, "I failed to add an active selection item" );

		return selectionResult;
		}

	SelectionResultType SelectionList::findFirstExecutionItem( WordItem *solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem *firstSelectionItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findFirstExecutionItem";

		if( ( firstSelectionItem = firstActiveSelectionItem() ) != NULL )
			{
			if( firstSelectionItem->findNextExecutionSelectionItem( true, solveWordItem ) != RESULT_OK )
				return addSelectionResultError( functionNameString, NULL, "I failed to find the first execution selection item" );

			selectionResult.firstExecutionItem = firstSelectionItem->nextExecutionItem();
			}

		return selectionResult;
		}

	SelectionItem *SelectionList::executionStartEntry( unsigned short executionLevel, unsigned int executionSentenceNr )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != NULL &&
		searchSelectionItem->activeSentenceNr() >= executionSentenceNr )
			{
			if( searchSelectionItem->activeSentenceNr() == executionSentenceNr &&
			searchSelectionItem->selectionLevel() == executionLevel )
				return searchSelectionItem;

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return NULL;
		}

	SelectionItem *SelectionList::firstActiveSelectionItem()
		{
		return (SelectionItem *)firstActiveItem();
		}

	SelectionItem *SelectionList::firstConditionSelectionItem( unsigned int conditionSentenceNr )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != NULL &&
		searchSelectionItem->activeSentenceNr() >= conditionSentenceNr )
			{
			if( searchSelectionItem->activeSentenceNr() == conditionSentenceNr )
				return searchSelectionItem;

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return NULL;
		}

/*************************************************************************
 *	"Who can be compared with the Lord or God,
 *	who is enthroned on high?" (Psalm 113:5)
 *************************************************************************/
