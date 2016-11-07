/*	Class:			AdminSelection
 *	Supports class:	AdminItem
 *	Purpose:		To process selections
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

#include "AdminItem.h"
#include "SelectionList.h"

class AdminSelection
	{
	friend class AdminItem;

	// Private constructed variables

	SelectionItem *firstSelectionItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType removeDuplicateSelection()
		{
		SelectionList *actionList;
		SelectionList *alternativeList;
		SelectionList *conditionList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeDuplicateSelection";

		if( ( conditionList = adminItem_->conditionList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The condition list isn't created yet" );

		if( ( actionList = adminItem_->actionList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The action list isn't created yet" );

		if( conditionList->deleteActiveItemsWithCurrentSentenceNr() != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove the condition of a selection" );

		if( actionList->deleteActiveItemsWithCurrentSentenceNr() != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove the action of a selection" );

		if( ( alternativeList = adminItem_->alternativeList ) != NULL )
			{
			if( alternativeList->deleteActiveItemsWithCurrentSentenceNr() != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove the alternative of a selection" );
			}

		return RESULT_OK;
		}

	SelectionItem *firstCondition()
		{
		SelectionList *conditionList;

		if( ( conditionList = adminItem_->conditionList ) != NULL )
			return conditionList->firstActiveSelectionItem();

		return NULL;
		}


	protected:
	// Constructor

	AdminSelection( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		firstSelectionItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminSelection" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType checkForDuplicateSelection()
		{
		SelectionResultType selectionResult;
		bool hasFoundDuplicateSelection = false;
		unsigned int duplicateConditionSentenceNr;
		SelectionList *actionList;
		SelectionList *alternativeList;
		SelectionList *conditionList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateSelection";

		if( ( conditionList = adminItem_->conditionList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The condition list isn't created yet" );

		if( ( actionList = adminItem_->actionList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The action list isn't created yet" );

		if( ( selectionResult = conditionList->checkDuplicateCondition() ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check if the condition selection part is duplicate" );

		if( ( duplicateConditionSentenceNr = selectionResult.duplicateConditionSentenceNr ) > NO_SENTENCE_NR )
			{
			if( ( selectionResult = actionList->checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check if the action selection part is duplicate" );

			if( selectionResult.hasFoundDuplicateSelection )
				{
				if( ( alternativeList = adminItem_->alternativeList ) == NULL )
					hasFoundDuplicateSelection = true;
				else
					{
					if( ( selectionResult = alternativeList->checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check if the alternative selection part is duplicate" );

					if( selectionResult.hasFoundDuplicateSelection )
						hasFoundDuplicateSelection = true;
					}
				}

			if( hasFoundDuplicateSelection )
				{
				if( removeDuplicateSelection() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove a duplicate selection" );
				}
			}

		return RESULT_OK;
		}

	ResultType createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionPart";

		if( generalizationWordItem == NULL &&
		specificationString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word or specification string is undefined" );

		switch( selectionListNr )
			{
			case ADMIN_CONDITION_LIST:
				if( adminItem_->conditionList == NULL )
					{
					// Create list
					if( ( adminItem_->conditionList = new SelectionList( ADMIN_CONDITION_LIST_SYMBOL, commonVariables_, adminItem_ ) ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to create an admin condition list" );

					commonVariables_->adminConditionList = adminItem_->conditionList;
					adminItem_->adminListArray[ADMIN_CONDITION_LIST] = adminItem_->conditionList;
					}

				if( ( selectionResult = adminItem_->conditionList->createSelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin condition list" );

				if( firstSelectionItem_ == NULL )
					firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;

				break;

			case ADMIN_ACTION_LIST:
				if( adminItem_->actionList == NULL )
					{
					// Create list
					if( ( adminItem_->actionList = new SelectionList( ADMIN_ACTION_LIST_SYMBOL, commonVariables_, adminItem_ ) ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to create an admin action list" );

					commonVariables_->adminActionList = adminItem_->actionList;
					adminItem_->adminListArray[ADMIN_ACTION_LIST] = adminItem_->actionList;
					}

				if( ( selectionResult = adminItem_->actionList->createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin action list" );

				if( firstSelectionItem_ == NULL )
					firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;

				break;

			case ADMIN_ALTERNATIVE_LIST:
				if( adminItem_->alternativeList == NULL )
					{
					// Create list
					if( ( adminItem_->alternativeList = new SelectionList( ADMIN_ALTERNATIVE_LIST_SYMBOL, commonVariables_, adminItem_ ) ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to create an admin alternative list" );

					commonVariables_->adminAlternativeList = adminItem_->alternativeList;
					adminItem_->adminListArray[ADMIN_ALTERNATIVE_LIST] = adminItem_->alternativeList;
					}

				if( ( selectionResult = adminItem_->alternativeList->createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin alternative list" );

				if( firstSelectionItem_ == NULL )
					firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;

				break;

			default:
				return adminItem_->startError( functionNameString, moduleNameString_, "The given list number is invalid: ", selectionListNr );
			}

		return RESULT_OK;
		}

	ResultType executeSelection( unsigned int endSolveProgress, SelectionItem *actionSelectionItem )
		{
		SelectionResultType selectionResult;
		bool hasDoneLastExecution;
		bool isInitializeVariables;
		bool isNewStart;
		bool isSatisfied;
		bool isWaitingForNewStart;
		bool isWaitingForNewLevel;
		bool isWaitingForExecution;
		unsigned short executionLevel;
		unsigned short executionListNr;
		unsigned short selectionLevel;
		unsigned short nSelectionExecutions = 0;
		unsigned int executionSentenceNr;
		WordItem *generalizationWordItem;
		SelectionItem *conditionSelectionItem;
		SelectionItem *executionSelectionItem;
		SelectionList *actionList;
		SelectionList *alternativeList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeSelection";

		do	{
			hasDoneLastExecution = false;
			commonVariables_->isAssignmentChanged = false;

			if( ( conditionSelectionItem = firstCondition() ) != NULL )
				{
				isSatisfied = false;
				isWaitingForNewStart = false;
				isWaitingForNewLevel = false;
				isWaitingForExecution = false;
				executionLevel = conditionSelectionItem->selectionLevel();
				executionSentenceNr = conditionSelectionItem->activeSentenceNr();

				do	{
					if( conditionSelectionItem == NULL ||
					executionSentenceNr != conditionSelectionItem->activeSentenceNr() )
						{
						executionListNr = ( isSatisfied ? ADMIN_ACTION_LIST : ADMIN_ALTERNATIVE_LIST );
						executionSelectionItem = NULL;

						if( isSatisfied )
							{
							if( ( actionList = adminItem_->actionList ) != NULL )
								executionSelectionItem = actionList->executionStartEntry( executionLevel, executionSentenceNr );
							}
						else
							{
							if( ( alternativeList = adminItem_->alternativeList ) != NULL )
								executionSelectionItem = alternativeList->executionStartEntry( executionLevel, executionSentenceNr );
							}

						if( executionSelectionItem != NULL )
							{
							isInitializeVariables = true;

							do	{
								if( adminItem_->executeImperative( isInitializeVariables, executionListNr, executionSelectionItem->imperativeVerbParameter(), NO_WORD_PARAMETER, executionSelectionItem->specificationWordTypeNr(), endSolveProgress, executionSelectionItem->specificationString(), executionSelectionItem->generalizationWordItem(), executionSelectionItem->specificationWordItem(), NULL, NULL, executionSelectionItem, actionSelectionItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute an imperative" );

								isInitializeVariables = false;
								}
							while( !adminItem_->hasRequestedRestart() &&
							!commonVariables_->hasDisplayedWarning &&
							( executionSelectionItem = executionSelectionItem->nextExecutionItem( executionLevel, executionSentenceNr ) ) != NULL );
							}

						// Found new condition
						if( conditionSelectionItem != NULL )
							{
							isSatisfied = false;
							isWaitingForNewStart = false;
							isWaitingForNewLevel = false;
							isWaitingForExecution = false;
							executionLevel = conditionSelectionItem->selectionLevel();
							}
						}

					if( conditionSelectionItem == NULL )
						hasDoneLastExecution = true;
					else
						{
						isNewStart = conditionSelectionItem->isNewStart();
						selectionLevel = conditionSelectionItem->selectionLevel();

						if( isNewStart &&
						isWaitingForNewStart &&
						// Found new start
						executionLevel == selectionLevel )
							isWaitingForNewStart = false;

						if( isWaitingForNewLevel &&
						// Found new level
						executionLevel != selectionLevel )
							isWaitingForNewLevel = false;

						if( !isWaitingForNewStart &&
						!isWaitingForNewLevel &&
						!isWaitingForExecution )
							{
							if( !isNewStart &&
							!isSatisfied &&
							executionLevel == selectionLevel )
								// Skip checking of this condition part and wait for a new start to come on this level
								isWaitingForNewStart = true;
							else
								{
								if( isNewStart &&
								isSatisfied &&
								executionLevel == selectionLevel )
									// Skip checking of this condition part and wait for a new level to come
									isWaitingForNewLevel = true;
								else
									{
									if( executionLevel != selectionLevel &&
									isSatisfied != conditionSelectionItem->isAction() )
										// Skip checking of this condition and wait for the next condition sentence number to come
										isWaitingForExecution = true;
									else
										{
										generalizationWordItem = conditionSelectionItem->generalizationWordItem();

										if( generalizationWordItem == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined condition word" );

										if( ( selectionResult = generalizationWordItem->checkSelectionCondition( conditionSelectionItem ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the condition of a selection in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

										isSatisfied = selectionResult.isConditionSatisfied;
										executionLevel = selectionLevel;
										executionSentenceNr = conditionSelectionItem->activeSentenceNr();
										}
									}
								}
							}

						conditionSelectionItem = conditionSelectionItem->nextSelectionItem();
						}
					}
				while( !hasDoneLastExecution &&
				!adminItem_->hasRequestedRestart() &&
				!commonVariables_->hasDisplayedWarning );
				}
			}
		while( !adminItem_->hasRequestedRestart() &&
		!commonVariables_->hasDisplayedWarning &&
		commonVariables_->isAssignmentChanged &&
		++nSelectionExecutions < MAX_SELECTION_EXECUTIONS );

		if( commonVariables_->isAssignmentChanged &&
		nSelectionExecutions == MAX_SELECTION_EXECUTIONS )
			return adminItem_->startError( functionNameString, moduleNameString_, "There is probably an endless loop in the selections" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Praise his name with dancing,
 *	accompanied by tambourine and harp.
 *	For the Lord delights in his people;
 *	he crowns the humble with victory." (Psalm 149:3-4)
 *************************************************************************/
