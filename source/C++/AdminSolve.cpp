/*	Class:			AdminSolve
 *	Supports class:	AdminItem
 *	Purpose:		Trying to solve (= to assign) words according to the
 *					given selections
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
#include "ScoreList.h"
#include "SelectionList.h"

class AdminSolve
	{
	friend class AdminItem;

	// Private constructed variables

	bool canWordBeSolved_;
	bool hasFoundPossibility_;

	unsigned short solveLevel_;

	unsigned int oldSatisfiedScore_;
	unsigned int newSatisfiedScore_;
	unsigned int oldDissatisfiedScore_;
	unsigned int newDissatisfiedScore_;
	unsigned int oldNotBlockingScore_;
	unsigned int newNotBlockingScore_;
	unsigned int oldBlockingScore_;
	unsigned int newBlockingScore_;

	unsigned int currentSolveProgress_;

	SelectionItem *currentExecutionSelectionItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType createNewAssignmentLevelInAllWords()
		{
		WordItem *currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = commonVariables_->firstAssignmentWordItem ) != NULL )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem->createNewAssignmentLevelInWord() == RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem->nextAssignmentWordItem ) != NULL );
			}

		return commonVariables_->result;
		}

	ResultType deleteAssignmentLevelInAllWords()
		{
		WordItem *currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = commonVariables_->firstAssignmentWordItem ) != NULL )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem->deleteAssignmentLevelInWord() == RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem->nextAssignmentWordItem ) != NULL );
			}

		return commonVariables_->result;
		}

	ResultType backTrackConditionScorePaths( bool isInitializeScores, bool isInverted, bool isAllowingDuplicates, bool isPreparingSort, unsigned short executionLevel, unsigned short solveStrategyParameter, unsigned int conditionSentenceNr )
		{
		bool isNewStart;
		bool isWaitingForNewLevel;
		bool isWaitingForNewStart;
		bool hasFoundScore = false;
		unsigned short selectionLevel;
		unsigned short handledSelectionLevel;
		unsigned short previousSelectionLevel;
		unsigned short unhandledSelectionLevel;
		SelectionItem *conditionSelectionItem;
		SelectionItem *previousConditionSelectionItem;
		ScoreList *scoreList;
		SelectionList *conditionList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "backTrackConditionScorePaths";

		if( ( conditionList = adminItem_->conditionList ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The condition list isn't created yet" );

		conditionList->clearConditionChecksForSolving( MAX_LEVEL, conditionSentenceNr );

		do	{
			isWaitingForNewLevel = false;
			isWaitingForNewStart = false;
			handledSelectionLevel = MAX_LEVEL;
			previousSelectionLevel = NO_SELECTION_LEVEL;
			unhandledSelectionLevel = MAX_LEVEL;
			previousConditionSelectionItem = NULL;

			if( ( conditionSelectionItem = conditionList->firstConditionSelectionItem( conditionSentenceNr ) ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the first item of the condition with sentence number ", conditionSentenceNr );

			if( isInitializeScores )
				{
				oldSatisfiedScore_ = NO_SCORE;
				newSatisfiedScore_ = NO_SCORE;
				oldDissatisfiedScore_ = NO_SCORE;
				newDissatisfiedScore_ = NO_SCORE;
				oldNotBlockingScore_ = NO_SCORE;
				newNotBlockingScore_ = NO_SCORE;
				oldBlockingScore_ = NO_SCORE;
				newBlockingScore_ = NO_SCORE;
				}

			do	{
				isNewStart = conditionSelectionItem->isNewStart();
				selectionLevel = conditionSelectionItem->selectionLevel();

				if( conditionSelectionItem->isConditionCheckedForSolving )
					isWaitingForNewStart = true;
				else
					{
					if( isNewStart &&
					// Not first start item
					previousConditionSelectionItem != NULL )
						{
						isWaitingForNewStart = false;

						// Second branch on the same level
						if( selectionLevel == previousSelectionLevel )
							{
							if( !isWaitingForNewLevel )
								{
								if( handledSelectionLevel == MAX_LEVEL )
									{
									handledSelectionLevel = selectionLevel;

									// This is a new branch
									if( previousConditionSelectionItem->isConditionCheckedForSolving )
										conditionSelectionItem->isConditionCheckedForSolving = true;
									else
										{
										// The previous branch was a new branch and this one is unhandled
										isWaitingForNewLevel = true;
										unhandledSelectionLevel = selectionLevel;
										previousConditionSelectionItem->isConditionCheckedForSolving = true;
										}
									}
								else
									{
									isWaitingForNewLevel = true;

									// This branch isn't handled yet
									if( unhandledSelectionLevel == MAX_LEVEL )
										unhandledSelectionLevel = selectionLevel;
									}
								}
							}
						else
							// New start on a new level
							isWaitingForNewLevel = false;
						}

					if( !isWaitingForNewLevel &&
					!isWaitingForNewStart )
						{
						if( calculateScorePaths( isInverted, isAllowingDuplicates, isPreparingSort, solveStrategyParameter, conditionSelectionItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to calculate the score paths" );
						}
					}

				previousSelectionLevel = selectionLevel;

				if( isNewStart )
					previousConditionSelectionItem = conditionSelectionItem;
				}
			while( ( conditionSelectionItem = conditionSelectionItem->nextConditionItem( executionLevel, conditionSentenceNr ) ) != NULL );

			if( previousSelectionLevel != executionLevel )
				return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't reach the given execution level ", executionLevel, ". The highest reached level was ", handledSelectionLevel );

			// All branches on same level are done and there are branchs on a higher level unhandled,
			// so start again with the handled branches (by clearing their checks) until all paths are handled
			if( unhandledSelectionLevel < MAX_LEVEL &&
			handledSelectionLevel < unhandledSelectionLevel )
				conditionList->clearConditionChecksForSolving( unhandledSelectionLevel, conditionSentenceNr );

			if( isInitializeScores )
				{
				scoreList = adminItem_->scoreList;

				if( scoreList == NULL ||

				( oldSatisfiedScore_ == NO_SCORE &&
				newSatisfiedScore_ == NO_SCORE &&
				oldDissatisfiedScore_ == NO_SCORE &&
				newDissatisfiedScore_ == NO_SCORE &&
				oldNotBlockingScore_ == NO_SCORE &&
				newNotBlockingScore_ == NO_SCORE &&
				oldBlockingScore_ == NO_SCORE &&
				newBlockingScore_ == NO_SCORE ) )
					hasFoundScore = false;
				else
					{
					if( scoreList->checkScores( isInverted, solveStrategyParameter, oldSatisfiedScore_, newSatisfiedScore_, oldDissatisfiedScore_, newDissatisfiedScore_, oldNotBlockingScore_, newNotBlockingScore_, oldBlockingScore_, newBlockingScore_ ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the scores" );

					hasFoundScore = scoreList->hasFoundScore();
					}

				if( !hasFoundScore &&
				isAllowingDuplicates &&
				commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL )
					{
					if( createScore( ( commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL ), oldSatisfiedScore_, newSatisfiedScore_, oldDissatisfiedScore_, newDissatisfiedScore_, oldNotBlockingScore_, newNotBlockingScore_, oldBlockingScore_, newBlockingScore_, NULL ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create an empty solve score" );
					}
				}
			}
		while( unhandledSelectionLevel < MAX_LEVEL );

		return RESULT_OK;
		}

	ResultType canWordBeSolved( bool isAction, WordItem *solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem *originalExecutionSelectionItem = currentExecutionSelectionItem_;
		WordItem *specificationWordItem;
		SelectionList *actionList;
		SelectionList *alternativeList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "canWordBeSolved";

		canWordBeSolved_ = false;

		if( isAction )
			{
			if( ( actionList = adminItem_->actionList ) != NULL )
				{
				if( ( selectionResult = actionList->findFirstExecutionItem( solveWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first action execution selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );

				currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				}
			}
		else
			{
			if( ( alternativeList = adminItem_->alternativeList ) != NULL )
				{
				if( ( selectionResult = alternativeList->findFirstExecutionItem( solveWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first alternative execution selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );

				currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				}
			}

		if( currentExecutionSelectionItem_ != NULL )
			{
			do	{
				if( ( specificationWordItem = currentExecutionSelectionItem_->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the current execution selection item is undefined" );

				if( specificationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
					{
					if( currentExecutionSelectionItem_->isValueSpecification() )
						{
						if( canWordBeSolved( specificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if value specification word \"", specificationWordItem->anyWordTypeString(), "\" can be solved" );
						}
					else
						{
						if( !currentExecutionSelectionItem_->isNegative() )
							canWordBeSolved_ = true;
						}
					}
				else
					// Word has active assignments
					canWordBeSolved_ = true;

				if( currentExecutionSelectionItem_->findNextExecutionSelectionItem( solveWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the next execution selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );

				currentExecutionSelectionItem_ = currentExecutionSelectionItem_->nextExecutionItem();
				}
			while( !canWordBeSolved_ );
			}

		currentExecutionSelectionItem_ = originalExecutionSelectionItem;

		return RESULT_OK;
		}

	ResultType canWordBeSolved( WordItem *solveWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "canWordBeSolved";

		if( canWordBeSolved( true, solveWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a word can be solved by an action" );

		if( !canWordBeSolved_ &&
		!commonVariables_->hasDisplayedWarning )
			{
			if( canWordBeSolved( false, solveWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a word can be solved by an alternative action" );
			}

		return RESULT_OK;
		}

	ResultType createScore( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createScore";

		if( adminItem_->scoreList == NULL )
			{
			// Create list
			if( ( adminItem_->scoreList = new ScoreList( commonVariables_, adminItem_ ) ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "I failed to create the admin solve score list" );

			commonVariables_->adminScoreList = adminItem_->scoreList;
			adminItem_->adminListArray[ADMIN_SCORE_LIST] = adminItem_->scoreList;
			}

		if( adminItem_->scoreList->createScoreItem( isChecked, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a score item" );

		return RESULT_OK;
		}

	ResultType calculateScorePaths( bool isInverted, bool isAllowingDuplicates, bool isPreparingSort, unsigned short solveStrategyParameter, SelectionItem *conditionSelectionItem )
		{
		SelectionResultType selectionResult;
		bool isAddingLocalScores;
		bool isOriginalFoundPossibility;
		bool hasFoundScore = false;
		WordItem *generalizationWordItem;
		WordItem *specificationWordItem;
		ScoreList *scoreList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "calculateScorePaths";

		if( conditionSelectionItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given condition selection item is undefined" );

		if( ( generalizationWordItem = conditionSelectionItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given score item is undefined" );

		if( ( selectionResult = generalizationWordItem->checkSelectionCondition( conditionSelectionItem ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the condition of a selection in word word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		isAddingLocalScores = true;

		if( !selectionResult.isConditionSatisfied &&
		!generalizationWordItem->isWordCheckedForSolving )
			{
			if( conditionSelectionItem->isAssignedOrClear() )
				{
				if( !conditionSelectionItem->isNegative() &&
				// Word has no active assignments
				generalizationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
					{
					isOriginalFoundPossibility = hasFoundPossibility_;

					if( findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a possibility to solve condition word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					if( hasFoundPossibility_ )
						isAddingLocalScores = false;
					else
						hasFoundPossibility_ = isOriginalFoundPossibility;
					}
				}
			else
				{
				if( ( specificationWordItem = conditionSelectionItem->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the given score item is undefined" );

				if( !conditionSelectionItem->isNegative() &&
				!specificationWordItem->isWordCheckedForSolving &&
				// Word has no active assignments
				generalizationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
					{
					if( canWordBeSolved( specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if specification word \"", specificationWordItem->anyWordTypeString(), "\" can be solved" );

					if( canWordBeSolved_ )
						{
						isOriginalFoundPossibility = hasFoundPossibility_;

						if( findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a possibility to solve condition word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( hasFoundPossibility_ )
							isAddingLocalScores = false;
						else
							hasFoundPossibility_ = isOriginalFoundPossibility;
						}
					else
						{
						scoreList = adminItem_->scoreList;

						if( isAllowingDuplicates ||
						scoreList == NULL )
							hasFoundScore = false;
						else
							{
							if( scoreList->findScore( isPreparingSort, conditionSelectionItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a score item" );

							hasFoundScore = scoreList->hasFoundScore();
							}

						if( !hasFoundScore )
							{
							if( createScore( ( commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL ), NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, conditionSelectionItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create an empty solve score" );

							hasFoundPossibility_ = true;
							}
						}
					}
				}
			}

		if( !commonVariables_->hasDisplayedWarning &&
		isAddingLocalScores )
			{
			oldSatisfiedScore_ += selectionResult.oldSatisfiedScore;
			newSatisfiedScore_ += selectionResult.newSatisfiedScore;
			oldDissatisfiedScore_ += selectionResult.oldDissatisfiedScore;
			newDissatisfiedScore_ += selectionResult.newDissatisfiedScore;
			oldNotBlockingScore_ += selectionResult.oldNotBlockingScore;
			newNotBlockingScore_ += selectionResult.newNotBlockingScore;
			oldBlockingScore_ += selectionResult.oldBlockingScore;
			newBlockingScore_ += selectionResult.newBlockingScore;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

	AdminSolve( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		canWordBeSolved_ = false;
		hasFoundPossibility_ = false;

		solveLevel_ = NO_SOLVE_LEVEL;

		oldSatisfiedScore_ = NO_SCORE;
		newSatisfiedScore_ = NO_SCORE;
		oldDissatisfiedScore_ = NO_SCORE;
		newDissatisfiedScore_ = NO_SCORE;
		oldNotBlockingScore_ = NO_SCORE;
		newNotBlockingScore_ = NO_SCORE;
		oldBlockingScore_ = NO_SCORE;
		newBlockingScore_ = NO_SCORE;

		currentSolveProgress_ = 0;

		currentExecutionSelectionItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminSolve" );

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

	void clearCurrentSolveProgress()
		{
		currentSolveProgress_ = 0;
		}

	ResultType findPossibilityToSolveWord( bool isInitializeScores, bool isAllowingDuplicates, bool isInverted, bool isPreparingSort, unsigned short solveStrategyParameter, WordItem *solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem *originalExecutionSelectionItem;
		SelectionList *actionList;
		SelectionList *alternativeList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossibilityToSolveWord";

		hasFoundPossibility_ = false;

		if( solveWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given solve word is undefined" );

		if( solveWordItem->isWordCheckedForSolving )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given solve word \"", solveWordItem->anyWordTypeString(), "\" is already checked" );

		if( canWordBeSolved( true, solveWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if the given word \"", solveWordItem->anyWordTypeString(), "\" can be solved by action" );

		solveWordItem->isWordCheckedForSolving = true;

		if( canWordBeSolved_ &&
		( actionList = adminItem_->actionList ) != NULL )
			{
			originalExecutionSelectionItem = currentExecutionSelectionItem_;

			if( ( selectionResult = actionList->findFirstExecutionItem( solveWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first action selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );

			if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != NULL )
				{
				do	{
					if( backTrackConditionScorePaths( isInitializeScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_->selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_->activeSentenceNr() ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to back-fire the condition score paths for the action with sentence number ", currentExecutionSelectionItem_->activeSentenceNr() );

					if( currentExecutionSelectionItem_->findNextExecutionSelectionItem( solveWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the next action selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_->nextExecutionItem() ) != NULL );
				}

			currentExecutionSelectionItem_ = originalExecutionSelectionItem;
			}

		if( !commonVariables_->hasDisplayedWarning )
			{
			if( canWordBeSolved( false, solveWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if the given word \"", solveWordItem->anyWordTypeString(), "\" can be solved by alternative" );

			if( canWordBeSolved_ &&
			( alternativeList = adminItem_->alternativeList ) != NULL )
				{
				originalExecutionSelectionItem = currentExecutionSelectionItem_;

				if( ( selectionResult = alternativeList->findFirstExecutionItem( solveWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first alternative item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );

				if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != NULL )
					{
					do	{
						if( backTrackConditionScorePaths( isInitializeScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_->selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_->activeSentenceNr() ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to back-fire the condition score paths for the alternative with sentence number ", currentExecutionSelectionItem_->activeSentenceNr() );

						if( currentExecutionSelectionItem_->findNextExecutionSelectionItem( solveWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the next alternative item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
						}
					while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_->nextExecutionItem() ) != NULL );
					}

				currentExecutionSelectionItem_ = originalExecutionSelectionItem;
				}

			solveWordItem->isWordCheckedForSolving = false;
			}

		return RESULT_OK;
		}

	ResultType solveWord( unsigned int endSolveProgress, WordItem *solveWordItem, SelectionItem *actionSelectionItem )
		{
		SelectionResultType selectionResult;
		SpecificationResultType specificationResult;
		bool isInverted = false;
		unsigned int nPossibilities;
		unsigned int possibilityNumber = 0;
		unsigned int solveProgressStep;
		unsigned int tempEndSolveProgress;
		ScoreItem *possibilityScoreItem;
		WordItem *predefinedNounSolveLevelWordItem;
		WordItem *predefinedNounSolveMethodWordItem;
		WordItem *predefinedNounSolveStrategyWordItem;
		ScoreList *scoreList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "solveWord";

		if( solveWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given solve word is undefined" );

		if( ( predefinedNounSolveMethodWordItem = adminItem_->predefinedNounSolveMethodWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The predefined solve-method noun word item is undefined" );

		if( ( predefinedNounSolveStrategyWordItem = adminItem_->predefinedNounSolveStrategyWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The predefined solve strategy noun word item is undefined" );

		if( currentSolveProgress_ == 0 )
			{
			if( ( predefinedNounSolveLevelWordItem = adminItem_->predefinedNounSolveLevelWordItem() ) == NULL )
				solveLevel_ = NO_SOLVE_LEVEL;
			else
				{
				if( ( specificationResult = predefinedNounSolveLevelWordItem->getAssignmentOrderNr() ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve level" );

				solveLevel_ = specificationResult.assignmentOrderNr;
				}
			}

		// Word has no active assignments
		if( solveWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
			{
			if( commonVariables_->currentAssignmentLevel > solveLevel_ )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given assignment level of ", commonVariables_->currentAssignmentLevel, " is higher than the given solve level ", solveLevel_ );

			if( adminItem_->assignSpecification( predefinedNounSolveMethodWordItem, adminItem_->predefinedAdjectiveBusyWordItem() ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the busy flag to the solve method at assignment level ", commonVariables_->currentAssignmentLevel );

			if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL &&
			( scoreList = adminItem_->scoreList ) != NULL )
				{
				// Make sure no scores are left at the start
				if( scoreList->deleteScores() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the admin score list" );
				}

			isInverted = ( predefinedNounSolveMethodWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, adminItem_->predefinedAdjectiveInvertedWordItem() ) != NULL );

			if( ( specificationResult = predefinedNounSolveStrategyWordItem->getAssignmentWordParameter() ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve strategy at assignment level ", commonVariables_->currentAssignmentLevel );

			if( findPossibilityToSolveWord( true, ( commonVariables_->currentAssignmentLevel == solveLevel_ ), isInverted, ( commonVariables_->currentAssignmentLevel + 1 < solveLevel_ ), specificationResult.assignmentParameter, solveWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a possibility to solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

			if( hasFoundPossibility_ )
				{
				if( commonVariables_->currentAssignmentLevel < solveLevel_ )
					{
					if( ( scoreList = adminItem_->scoreList ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The solve scores list isn't created yet at assignment level ", commonVariables_->currentAssignmentLevel );

					nPossibilities = scoreList->nPossibilities();
					solveProgressStep = ( ( endSolveProgress - currentSolveProgress_ ) / nPossibilities );

					if( solveLevel_ > 1 )
						commonVariables_->presentation->startProgress( currentSolveProgress_, MAX_PROGRESS, INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_START, solveLevel_, INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_END );

					if( ( possibilityScoreItem = scoreList->firstPossibility() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to get the first possibility item at assignment level ", commonVariables_->currentAssignmentLevel );

					do	{
						// Copy solve action of NO_ASSIGNMENT_LEVEL to higher levels
						if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
							actionSelectionItem = possibilityScoreItem->referenceSelectionItem;

						if( createNewAssignmentLevelInAllWords() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a new assignment level: ", commonVariables_->currentAssignmentLevel );

						commonVariables_->currentAssignmentLevel++;

						if( adminItem_->assignSelectionSpecification( possibilityScoreItem->referenceSelectionItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a selection specifcation at assignment level: ", commonVariables_->currentAssignmentLevel );

						tempEndSolveProgress = currentSolveProgress_ + solveProgressStep;

						if( adminItem_->executeSelection( ( currentSolveProgress_ + solveProgressStep / 2L ), actionSelectionItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute a selection during the solving of word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

						// Word has active assignments
						if( solveWordItem->firstNonQuestionActiveAssignmentItem() != NULL )
							{
							isInverted = ( predefinedNounSolveMethodWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, adminItem_->predefinedAdjectiveInvertedWordItem() ) != NULL );

							if( !isInverted &&
							commonVariables_->currentAssignmentLevel < solveLevel_ )
								{
								if( scoreList->deleteScores() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the scores with an assignment level higher than ", commonVariables_->currentAssignmentLevel );

								// Don't solve any deeper if there is a winning score
								solveLevel_ = commonVariables_->currentAssignmentLevel;
								}

							// Create winning or losing score
							if( createScore( false, NO_SCORE, ( isInverted ? NO_SCORE : WINNING_SCORE ), NO_SCORE, ( isInverted ? WINNING_SCORE : NO_SCORE ), NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, actionSelectionItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a winning or losing score of solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

							currentSolveProgress_ = tempEndSolveProgress;

							if( solveLevel_ > 1 )
								commonVariables_->presentation->displayProgress( currentSolveProgress_ );
							}
						else
							{
							if( solveWord( tempEndSolveProgress, solveWordItem, actionSelectionItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

							if( commonVariables_->currentAssignmentLevel == 1 )
								scoreList->changeAction( actionSelectionItem );
							}

						if( deleteAssignmentLevelInAllWords() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the assignments of level ", commonVariables_->currentAssignmentLevel );

						commonVariables_->currentAssignmentLevel--;
						possibilityScoreItem = possibilityScoreItem->nextPossibilityScoreItem();

						if( ++possibilityNumber <= nPossibilities )
							{
							if( possibilityScoreItem != NULL &&
							possibilityNumber == nPossibilities )
								return adminItem_->startError( functionNameString, moduleNameString_, "I have found more possibility items than number of possibilities" );
							}
						else
							{
							if( possibilityScoreItem == NULL )
								return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the next possibility item before the number of possibilities is reached" );
							}
						}
					while( possibilityScoreItem != NULL );

					if( nPossibilities > 1 ||
					// Higher level has possibilities
					commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL )
						{
						if( scoreList->deleteScores() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the scores with assignment level ", commonVariables_->currentAssignmentLevel );
						}
					}
				else
					{
					currentSolveProgress_ = endSolveProgress;

					if( solveLevel_ > 1 )
						commonVariables_->presentation->displayProgress( currentSolveProgress_ );
					}

				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
					{
					if( ( scoreList = adminItem_->scoreList ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The solve scores list isn't created yet" );

					if( ( specificationResult = predefinedNounSolveStrategyWordItem->getAssignmentWordParameter() ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve strategy at assignment level ", commonVariables_->currentAssignmentLevel );

					if( ( selectionResult = scoreList->getBestAction( adminItem_->isCurrentlyTesting(), specificationResult.assignmentParameter ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the best action of solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

					if( ( actionSelectionItem = selectionResult.bestActionItem ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the best action selection item" );

					if( adminItem_->assignSelectionSpecification( actionSelectionItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a selection specification at assignment level ", commonVariables_->currentAssignmentLevel );

					if( adminItem_->assignSpecification( predefinedNounSolveMethodWordItem, adminItem_->predefinedAdjectiveDoneWordItem() ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the done flag to the solve method at assignment level ", commonVariables_->currentAssignmentLevel );
					}
				}
			else
				{
				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
					{
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_START, solveWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_END ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}
		else
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_START, solveWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_END ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Give thanks to the God of gods.
 *	His faithful love endures forever." (Psalm 136:2)
 *************************************************************************/
