/*
 *	Class:			AdminSelection
 *	Supports class:	AdminItem
 *	Purpose:		To process selections
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

	// Private constructible variables

	SelectionItem *firstSelectionItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType removeDuplicateSelection()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeDuplicateSelection";

		if( adminItem_->conditionList != NULL )
			{
			if( adminItem_->actionList != NULL )
				{
				if( adminItem_->conditionList->deleteActiveItemsWithCurrentSentenceNr() == RESULT_OK )
					{
					if( adminItem_->actionList->deleteActiveItemsWithCurrentSentenceNr() == RESULT_OK )
						{
						if( adminItem_->alternativeList != NULL )
							{
							if( adminItem_->alternativeList->deleteActiveItemsWithCurrentSentenceNr() != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to remove the alternative of a selection" );
							}
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to remove the action of a selection" );
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to remove the condition of a selection" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The action list isn't created yet" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The condition list isn't created yet" );

		return RESULT_OK;
		}

	SelectionItem *firstCondition()
		{
		if( adminItem_->conditionList != NULL )
			return adminItem_->conditionList->firstActiveSelectionItem();

		return NULL;
		}


	protected:
	// Constructor / deconstructor

	AdminSelection( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		firstSelectionItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminSelection" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateSelection";

		if( adminItem_->conditionList != NULL )
			{
			if( adminItem_->actionList != NULL )
				{
				if( ( selectionResult = adminItem_->conditionList->checkDuplicateCondition() ).result == RESULT_OK )
					{
					if( ( duplicateConditionSentenceNr = selectionResult.duplicateConditionSentenceNr ) > NO_SENTENCE_NR )
						{
						if( ( selectionResult = adminItem_->actionList->checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result == RESULT_OK )
							{
							if( selectionResult.hasFoundDuplicateSelection )
								{
								if( adminItem_->alternativeList == NULL )
									hasFoundDuplicateSelection = true;
								else
									{
									if( ( selectionResult = adminItem_->alternativeList->checkDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result == RESULT_OK )
										{
										if( selectionResult.hasFoundDuplicateSelection )
											hasFoundDuplicateSelection = true;
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check if the alternative selection part is duplicate" );
									}
								}

							if( hasFoundDuplicateSelection )
								{
								if( removeDuplicateSelection() != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to remove a duplicate selection" );
								}
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check if the action selection part is duplicate" );
						}
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check if the condition selection part is duplicate" );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The action list isn't created yet" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The condition list isn't created yet" );

		return RESULT_OK;
		}

	ResultType createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short prepositionParameter, unsigned short specificationWordParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionPart";

		if( generalizationWordItem != NULL ||
		specificationString != NULL )
			{
			switch( selectionListNr )
				{
				case ADMIN_CONDITION_LIST:
					if( adminItem_->conditionList == NULL )
						{
						// Create list
						if( ( adminItem_->conditionList = new SelectionList( ADMIN_CONDITION_LIST_SYMBOL, commonVariables_, myWordItem_ ) ) != NULL )
							{
							commonVariables_->adminConditionList = adminItem_->conditionList;
							adminItem_->adminListArray[ADMIN_CONDITION_LIST] = adminItem_->conditionList;
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to create an admin condition list" );
						}

					if( ( selectionResult = adminItem_->conditionList->createSelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == RESULT_OK )
						{
						if( firstSelectionItem_ == NULL )
							firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin condition list" );

					break;

				case ADMIN_ACTION_LIST:
					if( adminItem_->actionList == NULL )
						{
						// Create list
						if( ( adminItem_->actionList = new SelectionList( ADMIN_ACTION_LIST_SYMBOL, commonVariables_, myWordItem_ ) ) != NULL )
							{
							commonVariables_->adminActionList = adminItem_->actionList;
							adminItem_->adminListArray[ADMIN_ACTION_LIST] = adminItem_->actionList;
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to create an admin action list" );
						}

					if( ( selectionResult = adminItem_->actionList->createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == RESULT_OK )
						{
						if( firstSelectionItem_ == NULL )
							firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin action list" );

					break;

				case ADMIN_ALTERNATIVE_LIST:
					if( adminItem_->alternativeList == NULL )
						{
						// Create list
						if( ( adminItem_->alternativeList = new SelectionList( ADMIN_ALTERNATIVE_LIST_SYMBOL, commonVariables_, myWordItem_ ) ) != NULL )
							{
							commonVariables_->adminAlternativeList = adminItem_->alternativeList;
							adminItem_->adminListArray[ADMIN_ALTERNATIVE_LIST] = adminItem_->alternativeList;
							}
						else
							return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to create an admin alternative list" );
						}

					if( ( selectionResult = adminItem_->alternativeList->createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == RESULT_OK )
						{
						if( firstSelectionItem_ == NULL )
							firstSelectionItem_ = selectionResult.lastCreatedSelectionItem;
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a copy of a temporary generalization noun selection item in the admin alternative list" );

					break;

				default:
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given list number is invalid: ", selectionListNr );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given generalization word or specification string is undefined" );

		return RESULT_OK;
		}

	ResultType executeSelection( unsigned int endSolveProgress, SelectionItem *actionSelectionItem )
		{
		SelectionResultType selectionResult;
		bool isNewStart;
		bool isSatisfied;
		bool isWaitingForNewStart;
		bool isWaitingForNewLevel;
		bool waitForExecution;
		bool doneLastExecution;
		bool isInitializeVariables;
		unsigned short executionLevel;
		unsigned short executionListNr;
		unsigned short selectionLevel;
		unsigned short nSelectionExecutions = 0;
		unsigned int executionSentenceNr;
		WordItem *conditionWordItem;
		SelectionItem *conditionSelectionItem;
		SelectionItem *executionSelectionItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeSelection";

		do	{
			doneLastExecution = false;
			commonVariables_->isAssignmentChanged = false;

			if( ( conditionSelectionItem = firstCondition() ) != NULL )
				{
				isSatisfied = false;
				isWaitingForNewStart = false;
				isWaitingForNewLevel = false;
				waitForExecution = false;
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
							if( adminItem_->actionList != NULL )
								executionSelectionItem = adminItem_->actionList->executionStartEntry( executionLevel, executionSentenceNr );
							}
						else
							{
							if( adminItem_->alternativeList != NULL )
								executionSelectionItem = adminItem_->alternativeList->executionStartEntry( executionLevel, executionSentenceNr );
							}

						if( executionSelectionItem != NULL )
							{
							isInitializeVariables = true;

							do	{
								if( adminItem_->executeImperative( isInitializeVariables, executionListNr, executionSelectionItem->imperativeParameter(), executionSelectionItem->specificationWordParameter(), executionSelectionItem->specificationWordTypeNr(), endSolveProgress, executionSelectionItem->specificationString(), executionSelectionItem->generalizationWordItem(), executionSelectionItem->specificationWordItem(), NULL, NULL, executionSelectionItem, actionSelectionItem ) == RESULT_OK )
									isInitializeVariables = false;
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute an imperative" );
								}
							while( !adminItem_->hasRequestedRestart() &&
							!commonVariables_->hasShownWarning &&
							( executionSelectionItem = executionSelectionItem->nextExecutionItem( executionLevel, executionSentenceNr ) ) != NULL );
							}

						if( conditionSelectionItem != NULL )	// Found new condition
							{
							isSatisfied = false;
							isWaitingForNewStart = false;
							isWaitingForNewLevel = false;
							waitForExecution = false;
							executionLevel = conditionSelectionItem->selectionLevel();
							}
						}

					if( conditionSelectionItem == NULL )
						doneLastExecution = true;
					else
						{
						isNewStart = conditionSelectionItem->isNewStart();
						selectionLevel = conditionSelectionItem->selectionLevel();

						if( isNewStart &&
						isWaitingForNewStart &&
						executionLevel == selectionLevel )	// Found new start
							isWaitingForNewStart = false;

						if( isWaitingForNewLevel &&
						executionLevel != selectionLevel )	// Found new level
							isWaitingForNewLevel = false;

						if( !isWaitingForNewStart &&
						!isWaitingForNewLevel &&
						!waitForExecution )
							{
							if( !isNewStart &&
							!isSatisfied &&
							executionLevel == selectionLevel )
								isWaitingForNewStart = true;	// Skip checking of this condition part and wait for a new start to come on this level
							else
								{
								if( isNewStart &&
								isSatisfied &&
								executionLevel == selectionLevel )
									isWaitingForNewLevel = true;		// Skip checking of this condition part and wait for a new level to come
								else
									{
									if( executionLevel != selectionLevel &&
									isSatisfied != conditionSelectionItem->isAction() )
										waitForExecution = true;	// Skip checking of this condition and wait for the next condition sentence number to come
									else
										{
										conditionWordItem = conditionSelectionItem->generalizationWordItem();

										if( conditionWordItem != NULL )
											{
											if( ( selectionResult = adminItem_->checkCondition( conditionSelectionItem ) ).result == RESULT_OK )
												{
												isSatisfied = selectionResult.isConditionSatisfied;
												executionLevel = selectionLevel;
												executionSentenceNr = conditionSelectionItem->activeSentenceNr();
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check the condition of word \"", conditionWordItem->anyWordTypeString(), "\"" );
											}
										else
											return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined condition word" );
										}
									}
								}
							}

						conditionSelectionItem = conditionSelectionItem->nextSelectionItem();
						}
					}
				while( !doneLastExecution &&
				!adminItem_->hasRequestedRestart() &&
				!commonVariables_->hasShownWarning );
				}
			}
		while( !adminItem_->hasRequestedRestart() &&
		!commonVariables_->hasShownWarning &&
		commonVariables_->isAssignmentChanged &&
		++nSelectionExecutions < MAX_SELECTION_EXECUTIONS );

		if( commonVariables_->isAssignmentChanged &&
		nSelectionExecutions == MAX_SELECTION_EXECUTIONS )
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "There is probably an endless loop in the selections" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Praise his name with dancing,
 *	accompanied by tambourine and harp.
 *	For the Lord delights in his people;
 *	he crowns the humble with victory." (Psalm 149:3-4)
 *************************************************************************/
