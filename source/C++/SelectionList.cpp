/*
 *	Class:			SelectionList
 *	Parent class:	List
 *	Purpose:		To store selection items
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

#include "SelectionList.h"

	// Private selection functions

	unsigned int SelectionList::getLowerSentenceNr( unsigned int duplicateSentenceNr )
		{
		Item *searchItem = firstActiveItem();
		unsigned int lowerSentenceNr = NO_SENTENCE_NR;

		while( searchItem != NULL )
			{
			if( searchItem->activeSentenceNr() < duplicateSentenceNr &&
			searchItem->activeSentenceNr() > lowerSentenceNr )
				lowerSentenceNr = searchItem->activeSentenceNr();

			searchItem = searchItem->nextItem;
			}

		return lowerSentenceNr;
		}


	// Constructor / deconstructor

	SelectionList::SelectionList( char _listChar, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( _listChar, "SelectionList", commonVariables, myWordItem );
		}

	// Protected virtual functions

	SelectionList::~SelectionList()
		{
		SelectionItem *deleteItem;
		SelectionItem *searchItem = firstActiveSelectionItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSelectionItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class SelectionList has inactive items." );

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class SelectionList has archived items." );

		searchItem = (SelectionItem *)firstReplacedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSelectionItem();
			delete deleteItem;
			}

		searchItem = (SelectionItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextSelectionItem();
			delete deleteItem;
			}
		}


	// Protected virtual functions

	ReferenceResultType SelectionList::findWordReference( WordItem *referenceWordItem )
		{
		ReferenceResultType referenceResult;
		SelectionItem *searchItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordReference";

		while( commonVariables()->result == RESULT_OK &&
		searchItem != NULL &&
		!referenceResult.hasFoundWordReference )
			{
			if( ( referenceResult = searchItem->findWordReference( referenceWordItem ) ).result == RESULT_OK )
				searchItem = searchItem->nextSelectionItem();
			else
				addError( functionNameString, NULL, NULL, "I failed to check for a reference word item in an active selection item" );
			}

		return referenceResult;
		}


	// Protected functions

	void SelectionList::clearConditionChecksForSolving( unsigned short selectionLevel, unsigned int conditionSentenceNr )
		{
		SelectionItem *searchItem = firstActiveSelectionItem();

		while( searchItem != NULL )
			{
			if( searchItem->selectionLevel() < selectionLevel &&
			searchItem->activeSentenceNr() == conditionSentenceNr )
				searchItem->isConditionCheckedForSolving = false;

			searchItem = searchItem->nextSelectionItem();
			}
		}

	ResultType SelectionList::checkSelectionItemForUsage( SelectionItem *unusedSelectionItem )
		{
		SelectionItem *searchItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSelectionItemForUsage";

		if( unusedSelectionItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->nextExecutionItem() == unusedSelectionItem )
					return startError( functionNameString, NULL, NULL, "The reference selection item is still in use" );

				searchItem = searchItem->nextSelectionItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "The given unused justification item is undefined" );

		return RESULT_OK;
		}

	ResultType SelectionList::checkWordItemForUsage( WordItem *unusedWordItem )
		{
		SelectionItem *searchItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->generalizationWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, NULL, "The generalization word item is still in use" );

				if( searchItem->specificationWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, NULL, "The specification word item is still in use" );

				searchItem = searchItem->nextSelectionItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "The given unused word item is undefined" );

		return RESULT_OK;
		}
/*
	ResultType SelectionList::storeChangesInFutureDatabase()
		{
		SelectionItem *searchItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeFileItemInFutureDatabase( isCondition, isAction, isAlternative ) != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store a selection item in the database" );
				}

			searchItem = searchItem->nextSelectionItem();
			}

		searchItem = firstReplacedSelectionItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeFileItemInFutureDatabase( isCondition, isAction, isAlternative ) != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced selection item in the database" );
				}

			searchItem = searchItem->nextSelectionItem();
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
					addError( functionNameString, NULL, NULL, "I failed to check if the alternative selection part is duplicate" );
				}
			}
		while( commonVariables()->result == RESULT_OK &&
		!selectionResult.hasFoundDuplicateSelection &&
		selectionResult.duplicateConditionSentenceNr > NO_SENTENCE_NR );

		selectionResult.result = commonVariables()->result;
		return selectionResult;
		}

	SelectionResultType SelectionList::checkDuplicateSelectionPart( unsigned int duplicateConditionSentenceNr )
		{
		SelectionResultType selectionResult;
		SelectionItem *currentSelectionItem = NULL;
		SelectionItem *searchItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkDuplicateSelectionPart";

		if( duplicateConditionSentenceNr > NO_SENTENCE_NR )
			{
			if( duplicateConditionSentenceNr < commonVariables()->currentSentenceNr )
				{
				selectionResult.hasFoundDuplicateSelection = true;

				while( searchItem != NULL &&
				searchItem->activeSentenceNr() >= duplicateConditionSentenceNr )
					{
					if( searchItem->activeSentenceNr() == duplicateConditionSentenceNr )
						{
						currentSelectionItem = firstActiveSelectionItem();

						do	{
							if( currentSelectionItem->isAction() == searchItem->isAction() &&
							currentSelectionItem->isAssignedOrClear() == searchItem->isAssignedOrClear() &&
							currentSelectionItem->isNegative() == searchItem->isNegative() &&
							currentSelectionItem->isNewStart() == searchItem->isNewStart() &&
							currentSelectionItem->selectionLevel() == searchItem->selectionLevel() &&
							currentSelectionItem->generalizationWordItem() == searchItem->generalizationWordItem() &&
							currentSelectionItem->specificationWordItem() == searchItem->specificationWordItem() )
								{
								if( currentSelectionItem->specificationString() != NULL &&
								searchItem->specificationString() != NULL )
									selectionResult.hasFoundDuplicateSelection = ( strcmp( currentSelectionItem->specificationString(), searchItem->specificationString() ) == 0 );
								else
									{
									if( currentSelectionItem->specificationString() != NULL ||
									searchItem->specificationString() != NULL )
										selectionResult.hasFoundDuplicateSelection = false;
									}

								if( selectionResult.hasFoundDuplicateSelection )
									{
									currentSelectionItem = ( currentSelectionItem->nextSelectionItem() != NULL &&
															currentSelectionItem->nextSelectionItem()->hasCurrentCreationSentenceNr() ? currentSelectionItem->nextSelectionItem() : NULL );

									searchItem = ( searchItem->nextSelectionItem() != NULL &&
												searchItem->nextSelectionItem()->activeSentenceNr() == duplicateConditionSentenceNr ? searchItem->nextSelectionItem() : NULL );
									}
								else
									searchItem = NULL;
								}
							else
								{
								selectionResult.hasFoundDuplicateSelection = false;
								searchItem = NULL;
								}
							}
						while( selectionResult.hasFoundDuplicateSelection &&
						currentSelectionItem != NULL &&
						searchItem != NULL );
						}
					else
						searchItem = searchItem->nextSelectionItem();
					}
				}
			else
				startError( functionNameString, NULL, NULL, "The given duplicate sentence number is equal or higher than the current sentence number" );
			}
		else
			startError( functionNameString, NULL, NULL, "The given duplicate sentence number is undefined" );

		selectionResult.result = commonVariables()->result;
		return selectionResult;
		}

	SelectionResultType SelectionList::createSelectionItem( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short imperativeParameter, unsigned short prepositionParameter, unsigned short specificationWordParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionItem";

		if( generalizationWordTypeNr > WORD_TYPE_UNDEFINED &&
		generalizationWordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( specificationWordTypeNr > WORD_TYPE_UNDEFINED &&
			specificationWordTypeNr < NUMBER_OF_WORD_TYPES )
				{
				if( commonVariables()->currentItemNr < MAX_ITEM_NR )
					{
					if( ( selectionResult.lastCreatedSelectionItem = new SelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString, commonVariables(), this, myWordItem() ) ) != NULL )
						{
						if( addItemToList( QUERY_ACTIVE_CHAR, selectionResult.lastCreatedSelectionItem ) != RESULT_OK )
							addError( functionNameString, NULL, NULL, "I failed to add an active selection item" );
						}
					else
						startError( functionNameString, NULL, NULL, "I failed to create a selection item" );
					}
				else
					startError( functionNameString, NULL, NULL, "The current item number is undefined" );
				}
			else
				startError( functionNameString, NULL, NULL, "The given specification word type number is undefined or out of bounds" );
			}
		else
			startError( functionNameString, NULL, NULL, "The given generalization word type number is undefined or out of bounds" );

		selectionResult.result = commonVariables()->result;
		return selectionResult;
		}

	SelectionResultType SelectionList::findFirstExecutionItem( WordItem *solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem *firstSelectionItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findFirstExecutionItem";

		if( ( firstSelectionItem = firstActiveSelectionItem() ) != NULL )
			{
			if( firstSelectionItem->findNextExecutionSelectionItem( true, solveWordItem ) == RESULT_OK )
				selectionResult.firstExecutionItem = firstSelectionItem->nextExecutionItem();
			else
				addError( functionNameString, NULL, NULL, "I failed to find the first execution selection item" );
			}

		selectionResult.result = commonVariables()->result;
		return selectionResult;
		}

	SelectionItem *SelectionList::executionStartEntry( unsigned short executionLevel, unsigned int executionSentenceNr )
		{
		SelectionItem *searchItem = firstActiveSelectionItem();

		while( searchItem != NULL &&
		searchItem->activeSentenceNr() >= executionSentenceNr )
			{
			if( searchItem->activeSentenceNr() == executionSentenceNr &&
			searchItem->selectionLevel() == executionLevel )
				return searchItem;

			searchItem = searchItem->nextSelectionItem();
			}

		return NULL;
		}

	SelectionItem *SelectionList::firstActiveSelectionItem()
		{
		return (SelectionItem *)firstActiveItem();
		}

	SelectionItem *SelectionList::firstConditionSelectionItem( unsigned int conditionSentenceNr )
		{
		SelectionItem *searchItem = firstActiveSelectionItem();

		while( searchItem != NULL &&
		searchItem->activeSentenceNr() >= conditionSentenceNr )
			{
			if( searchItem->activeSentenceNr() == conditionSentenceNr )
				return searchItem;

			searchItem = searchItem->nextSelectionItem();
			}

		return NULL;
		}

/*************************************************************************
 *	"Who can be compared with the Lord or God,
 *	who is enthroned on high?" (Psalm 113:5)
 *************************************************************************/
