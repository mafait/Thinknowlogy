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

class AdminSolve
	{
	// Private constructed variables

	private boolean canWordBeSolved_;
	private boolean hasFoundPossibility_;

	private short solveLevel_;

	private int oldSatisfiedScore_;
	private int newSatisfiedScore_;
	private int oldDissatisfiedScore_;
	private int newDissatisfiedScore_;
	private int oldNotBlockingScore_;
	private int newNotBlockingScore_;
	private int oldBlockingScore_;
	private int newBlockingScore_;

	private int currentSolveProgress_;

	private SelectionItem currentExecutionSelectionItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static byte createNewAssignmentLevelInAllWords()
		{
		WordItem currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = CommonVariables.firstAssignmentWordItem ) != null )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem.createNewAssignmentLevelInWord() == Constants.RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem.nextAssignmentWordItem ) != null );
			}

		return CommonVariables.result;
		}

	private static byte deleteAssignmentLevelInAllWords()
		{
		WordItem currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = CommonVariables.firstAssignmentWordItem ) != null )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem.deleteAssignmentLevelInWord() == Constants.RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem.nextAssignmentWordItem ) != null );
			}

		return CommonVariables.result;
		}

	private byte backTrackConditionScorePaths( boolean isInitializeScores, boolean isInverted, boolean isAllowingDuplicates, boolean isPreparingSort, short executionLevel, short solveStrategyParameter, int conditionSentenceNr )
		{
		boolean isNewStart;
		boolean isWaitingForNewLevel;
		boolean isWaitingForNewStart;
		boolean hasFoundScore = false;
		short selectionLevel;
		short handledSelectionLevel;
		short previousSelectionLevel;
		short unhandledSelectionLevel;
		SelectionItem conditionSelectionItem;
		SelectionItem previousConditionSelectionItem;
		ScoreList scoreList;
		SelectionList conditionList;

		if( ( conditionList = adminItem_.conditionList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The condition list isn't created yet" );

		conditionList.clearConditionChecksForSolving( Constants.MAX_LEVEL, conditionSentenceNr );

		do	{
			isWaitingForNewLevel = false;
			isWaitingForNewStart = false;
			handledSelectionLevel = Constants.MAX_LEVEL;
			previousSelectionLevel = Constants.NO_SELECTION_LEVEL;
			unhandledSelectionLevel = Constants.MAX_LEVEL;
			previousConditionSelectionItem = null;

			if( ( conditionSelectionItem = conditionList.firstConditionSelectionItem( conditionSentenceNr ) ) == null )
				return adminItem_.startError( 1, moduleNameString_, "I couldn't get the first item of the condition with sentence number " + conditionSentenceNr );

			if( isInitializeScores )
				{
				oldSatisfiedScore_ = Constants.NO_SCORE;
				newSatisfiedScore_ = Constants.NO_SCORE;
				oldDissatisfiedScore_ = Constants.NO_SCORE;
				newDissatisfiedScore_ = Constants.NO_SCORE;
				oldNotBlockingScore_ = Constants.NO_SCORE;
				newNotBlockingScore_ = Constants.NO_SCORE;
				oldBlockingScore_ = Constants.NO_SCORE;
				newBlockingScore_ = Constants.NO_SCORE;
				}

			do	{
				isNewStart = conditionSelectionItem.isNewStart();
				selectionLevel = conditionSelectionItem.selectionLevel();

				if( conditionSelectionItem.isConditionCheckedForSolving )
					isWaitingForNewStart = true;
				else
					{
					if( isNewStart &&
					// Not first start item
					previousConditionSelectionItem != null )
						{
						isWaitingForNewStart = false;

						// Second branch on the same level
						if( selectionLevel == previousSelectionLevel )
							{
							if( !isWaitingForNewLevel )
								{
								if( handledSelectionLevel == Constants.MAX_LEVEL )
									{
									handledSelectionLevel = selectionLevel;

									// This is a new branch
									if( previousConditionSelectionItem.isConditionCheckedForSolving )
										conditionSelectionItem.isConditionCheckedForSolving = true;
									else
										{
										// The previous branch was a new branch and this one is unhandled
										isWaitingForNewLevel = true;
										unhandledSelectionLevel = selectionLevel;
										previousConditionSelectionItem.isConditionCheckedForSolving = true;
										}
									}
								else
									{
									isWaitingForNewLevel = true;

									// This branch isn't handled yet
									if( unhandledSelectionLevel == Constants.MAX_LEVEL )
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
						if( calculateScorePaths( isInverted, isAllowingDuplicates, isPreparingSort, solveStrategyParameter, conditionSelectionItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to calculate the score paths" );
						}
					}

				previousSelectionLevel = selectionLevel;

				if( isNewStart )
					previousConditionSelectionItem = conditionSelectionItem;
				}
			while( ( conditionSelectionItem = conditionSelectionItem.nextConditionItem( executionLevel, conditionSentenceNr ) ) != null );

			if( previousSelectionLevel != executionLevel )
				return adminItem_.startError( 1, moduleNameString_, "I couldn't reach the given execution level " + executionLevel + ". The highest reached level was " + handledSelectionLevel );

			// All branches on same level are done and there are branchs on a higher level unhandled,
			// so start again with the handled branches (by clearing their checks) until all paths are handled
			if( unhandledSelectionLevel < Constants.MAX_LEVEL &&
			handledSelectionLevel < unhandledSelectionLevel )
				conditionList.clearConditionChecksForSolving( unhandledSelectionLevel, conditionSentenceNr );

			if( isInitializeScores )
				{
				scoreList = adminItem_.scoreList;

				if( scoreList == null ||

				( oldSatisfiedScore_ == Constants.NO_SCORE &&
				newSatisfiedScore_ == Constants.NO_SCORE &&
				oldDissatisfiedScore_ == Constants.NO_SCORE &&
				newDissatisfiedScore_ == Constants.NO_SCORE &&
				oldNotBlockingScore_ == Constants.NO_SCORE &&
				newNotBlockingScore_ == Constants.NO_SCORE &&
				oldBlockingScore_ == Constants.NO_SCORE &&
				newBlockingScore_ == Constants.NO_SCORE ) )
					hasFoundScore = false;
				else
					{
					if( scoreList.checkScores( isInverted, solveStrategyParameter, oldSatisfiedScore_, newSatisfiedScore_, oldDissatisfiedScore_, newDissatisfiedScore_, oldNotBlockingScore_, newNotBlockingScore_, oldBlockingScore_, newBlockingScore_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to check the scores" );

					hasFoundScore = scoreList.hasFoundScore();
					}

				if( !hasFoundScore &&
				isAllowingDuplicates &&
				CommonVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL )
					{
					if( createScore( ( CommonVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL ), oldSatisfiedScore_, newSatisfiedScore_, oldDissatisfiedScore_, newDissatisfiedScore_, oldNotBlockingScore_, newNotBlockingScore_, oldBlockingScore_, newBlockingScore_, null ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to create an empty solve score" );
					}
				}
			}
		while( unhandledSelectionLevel < Constants.MAX_LEVEL );

		return Constants.RESULT_OK;
		}

	private byte canWordBeSolved( boolean isAction, WordItem solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem originalExecutionSelectionItem = currentExecutionSelectionItem_;
		WordItem specificationWordItem;
		SelectionList actionList;
		SelectionList alternativeList;

		canWordBeSolved_ = false;

		if( isAction )
			{
			if( ( actionList = adminItem_.actionList ) != null )
				{
				if( ( selectionResult = actionList.findFirstExecutionItem( solveWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find the first action execution selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );

				currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				}
			}
		else
			{
			if( ( alternativeList = adminItem_.alternativeList ) != null )
				{
				if( ( selectionResult = alternativeList.findFirstExecutionItem( solveWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find the first alternative execution selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );

				currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				}
			}

		if( currentExecutionSelectionItem_ != null )
			{
			do	{
				if( ( specificationWordItem = currentExecutionSelectionItem_.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The specification word of the current execution selection item is undefined" );

				if( specificationWordItem.firstNonQuestionActiveAssignmentItem() == null )
					{
					if( currentExecutionSelectionItem_.isValueSpecification() )
						{
						if( canWordBeSolved( specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find out if value specification word \"" + specificationWordItem.anyWordTypeString() + "\" can be solved" );
						}
					else
						{
						if( !currentExecutionSelectionItem_.isNegative() )
							canWordBeSolved_ = true;
						}
					}
				else
					// Word has active assignments
					canWordBeSolved_ = true;

				if( currentExecutionSelectionItem_.findNextExecutionSelectionItem( solveWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find the next execution selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );

				currentExecutionSelectionItem_ = currentExecutionSelectionItem_.nextExecutionItem();
				}
			while( !canWordBeSolved_ );
			}

		currentExecutionSelectionItem_ = originalExecutionSelectionItem;

		return Constants.RESULT_OK;
		}

	private byte canWordBeSolved( WordItem solveWordItem )
		{
		if( canWordBeSolved( true, solveWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a word can be solved by an action" );

		if( !canWordBeSolved_ &&
		!CommonVariables.hasDisplayedWarning )
			{
			if( canWordBeSolved( false, solveWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a word can be solved by an alternative action" );
			}

		return Constants.RESULT_OK;
		}

	private byte createScore( boolean isChecked, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, SelectionItem referenceSelectionItem )
		{
		if( adminItem_.scoreList == null )
			{
			// Create list
			if( ( adminItem_.scoreList = new ScoreList( adminItem_ ) ) == null )
				return adminItem_.startError( 1, moduleNameString_, "I failed to create the admin solve score list" );

			CommonVariables.adminScoreList = adminItem_.scoreList;
			adminItem_.adminListArray[Constants.ADMIN_SCORE_LIST] = adminItem_.scoreList;
			}

		if( adminItem_.scoreList.createScoreItem( isChecked, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to create a score item" );

		return Constants.RESULT_OK;
		}

	private byte calculateScorePaths( boolean isInverted, boolean isAllowingDuplicates, boolean isPreparingSort, short solveStrategyParameter, SelectionItem conditionSelectionItem )
		{
		SelectionResultType selectionResult;
		boolean isAddingLocalScores;
		boolean isOriginalFoundPossibility;
		boolean hasFoundScore = false;
		WordItem generalizationWordItem;
		WordItem specificationWordItem;
		ScoreList scoreList;

		if( conditionSelectionItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given condition selection item is undefined" );

		if( ( generalizationWordItem = conditionSelectionItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given score item is undefined" );

		if( ( selectionResult = generalizationWordItem.checkSelectionCondition( conditionSelectionItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to check the condition of a selection in word word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		isAddingLocalScores = true;

		if( !selectionResult.isConditionSatisfied &&
		!generalizationWordItem.isWordCheckedForSolving )
			{
			if( conditionSelectionItem.isAssignedOrClear() )
				{
				if( !conditionSelectionItem.isNegative() &&
				// Word has no active assignments
				generalizationWordItem.firstNonQuestionActiveAssignmentItem() == null )
					{
					isOriginalFoundPossibility = hasFoundPossibility_;

					if( findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a possibility to solve condition word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					if( hasFoundPossibility_ )
						isAddingLocalScores = false;
					else
						hasFoundPossibility_ = isOriginalFoundPossibility;
					}
				}
			else
				{
				if( ( specificationWordItem = conditionSelectionItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The specification word of the given score item is undefined" );

				if( !conditionSelectionItem.isNegative() &&
				!specificationWordItem.isWordCheckedForSolving &&
				// Word has no active assignments
				generalizationWordItem.firstNonQuestionActiveAssignmentItem() == null )
					{
					if( canWordBeSolved( specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find out if specification word \"" + specificationWordItem.anyWordTypeString() + "\" can be solved" );

					if( canWordBeSolved_ )
						{
						isOriginalFoundPossibility = hasFoundPossibility_;

						if( findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find a possibility to solve condition word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( hasFoundPossibility_ )
							isAddingLocalScores = false;
						else
							hasFoundPossibility_ = isOriginalFoundPossibility;
						}
					else
						{
						scoreList = adminItem_.scoreList;

						if( isAllowingDuplicates ||
						scoreList == null )
							hasFoundScore = false;
						else
							{
							if( scoreList.findScore( isPreparingSort, conditionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to find a score item" );

							hasFoundScore = scoreList.hasFoundScore();
							}

						if( !hasFoundScore )
							{
							if( createScore( ( CommonVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL ), Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, conditionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create an empty solve score" );

							hasFoundPossibility_ = true;
							}
						}
					}
				}
			}

		if( !CommonVariables.hasDisplayedWarning &&
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

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminSolve( AdminItem adminItem )
		{
		String errorString = null;

		canWordBeSolved_ = false;
		hasFoundPossibility_ = false;

		solveLevel_ = Constants.NO_SOLVE_LEVEL;

		oldSatisfiedScore_ = Constants.NO_SCORE;
		newSatisfiedScore_ = Constants.NO_SCORE;
		oldDissatisfiedScore_ = Constants.NO_SCORE;
		newDissatisfiedScore_ = Constants.NO_SCORE;
		oldNotBlockingScore_ = Constants.NO_SCORE;
		newNotBlockingScore_ = Constants.NO_SCORE;
		oldBlockingScore_ = Constants.NO_SCORE;
		newBlockingScore_ = Constants.NO_SCORE;

		currentSolveProgress_ = 0;

		currentExecutionSelectionItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void clearCurrentSolveProgress()
		{
		currentSolveProgress_ = 0;
		}

	protected byte findPossibilityToSolveWord( boolean isInitializeScores, boolean isAllowingDuplicates, boolean isInverted, boolean isPreparingSort, short solveStrategyParameter, WordItem solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem originalExecutionSelectionItem;
		SelectionList actionList;
		SelectionList alternativeList;

		hasFoundPossibility_ = false;

		if( solveWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given solve word is undefined" );

		if( solveWordItem.isWordCheckedForSolving )
			return adminItem_.startError( 1, moduleNameString_, "The given solve word \"" + solveWordItem.anyWordTypeString() + "\" is already checked" );

		if( canWordBeSolved( true, solveWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find out if the given word \"" + solveWordItem.anyWordTypeString() + "\" can be solved by action" );

		solveWordItem.isWordCheckedForSolving = true;

		if( canWordBeSolved_ &&
		( actionList = adminItem_.actionList ) != null )
			{
			originalExecutionSelectionItem = currentExecutionSelectionItem_;

			if( ( selectionResult = actionList.findFirstExecutionItem( solveWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find the first action selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );

			if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != null )
				{
				do	{
					if( backTrackConditionScorePaths( isInitializeScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_.selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_.activeSentenceNr() ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to back-fire the condition score paths for the action with sentence number " + currentExecutionSelectionItem_.activeSentenceNr() );

					if( currentExecutionSelectionItem_.findNextExecutionSelectionItem( solveWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find the next action selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
					}
				while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_.nextExecutionItem() ) != null );
				}

			currentExecutionSelectionItem_ = originalExecutionSelectionItem;
			}

		if( !CommonVariables.hasDisplayedWarning )
			{
			if( canWordBeSolved( false, solveWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if the given word \"" + solveWordItem.anyWordTypeString() + "\" can be solved by alternative" );

			if( canWordBeSolved_ &&
			( alternativeList = adminItem_.alternativeList ) != null )
				{
				originalExecutionSelectionItem = currentExecutionSelectionItem_;

				if( ( selectionResult = alternativeList.findFirstExecutionItem( solveWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find the first alternative item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );

				if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != null )
					{
					do	{
						if( backTrackConditionScorePaths( isInitializeScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_.selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_.activeSentenceNr() ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to back-fire the condition score paths for the alternative with sentence number " + currentExecutionSelectionItem_.activeSentenceNr() );

						if( currentExecutionSelectionItem_.findNextExecutionSelectionItem( solveWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find the next alternative item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
						}
					while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_.nextExecutionItem() ) != null );
					}

				currentExecutionSelectionItem_ = originalExecutionSelectionItem;
				}

			solveWordItem.isWordCheckedForSolving = false;
			}

		return Constants.RESULT_OK;
		}

	protected byte solveWord( int endSolveProgress, WordItem solveWordItem, SelectionItem actionSelectionItem )
		{
		SelectionResultType selectionResult;
		SpecificationResultType specificationResult;
		boolean isInverted = false;
		int nPossibilities;
		int possibilityNumber = 0;
		int solveProgressStep;
		int tempEndSolveProgress;
		ScoreItem possibilityScoreItem;
		WordItem predefinedNounSolveLevelWordItem;
		WordItem predefinedNounSolveMethodWordItem;
		WordItem predefinedNounSolveStrategyWordItem;
		ScoreList scoreList;

		if( solveWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given solve word is undefined" );

		if( ( predefinedNounSolveMethodWordItem = adminItem_.predefinedNounSolveMethodWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The predefined solve-method noun word item is undefined" );

		if( ( predefinedNounSolveStrategyWordItem = adminItem_.predefinedNounSolveStrategyWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The predefined solve strategy noun word item is undefined" );

		if( currentSolveProgress_ == 0 )
			{
			if( ( predefinedNounSolveLevelWordItem = adminItem_.predefinedNounSolveLevelWordItem() ) == null )
				solveLevel_ = Constants.NO_SOLVE_LEVEL;
			else
				{
				if( ( specificationResult = predefinedNounSolveLevelWordItem.getAssignmentOrderNr() ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve level" );

				solveLevel_ = specificationResult.assignmentOrderNr;
				}
			}

		// Word has no active assignments
		if( solveWordItem.firstNonQuestionActiveAssignmentItem() == null )
			{
			if( CommonVariables.currentAssignmentLevel > solveLevel_ )
				return adminItem_.startError( 1, moduleNameString_, "The given assignment level of " + CommonVariables.currentAssignmentLevel + " is higher than the given solve level " + solveLevel_ );

			if( adminItem_.assignSpecification( predefinedNounSolveMethodWordItem, adminItem_.predefinedAdjectiveBusyWordItem() ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to assign the busy flag to the solve method at assignment level " + CommonVariables.currentAssignmentLevel );

			if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL &&
			( scoreList = adminItem_.scoreList ) != null )
				{
				// Make sure no scores are left at the start
				if( scoreList.deleteScores() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete the admin score list" );
				}

			isInverted = ( predefinedNounSolveMethodWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, Constants.NO_CONTEXT_NR, adminItem_.predefinedAdjectiveInvertedWordItem() ) != null );

			if( ( specificationResult = predefinedNounSolveStrategyWordItem.getAssignmentWordParameter() ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve strategy at assignment level " + CommonVariables.currentAssignmentLevel );

			if( findPossibilityToSolveWord( true, ( CommonVariables.currentAssignmentLevel == solveLevel_ ), isInverted, ( CommonVariables.currentAssignmentLevel + 1 < solveLevel_ ), specificationResult.assignmentParameter, solveWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find a possibility to solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );

			if( hasFoundPossibility_ )
				{
				if( CommonVariables.currentAssignmentLevel < solveLevel_ )
					{
					if( ( scoreList = adminItem_.scoreList ) == null )
						return adminItem_.startError( 1, moduleNameString_, "The solve scores list isn't created yet at assignment level " + CommonVariables.currentAssignmentLevel );

					nPossibilities = scoreList.nPossibilities();
					solveProgressStep = ( ( endSolveProgress - currentSolveProgress_ ) / nPossibilities );

					if( solveLevel_ > 1 )
						Presentation.startProgress( currentSolveProgress_, Constants.MAX_PROGRESS, Constants.INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_START, solveLevel_, Constants.INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_END );

					if( ( possibilityScoreItem = scoreList.firstPossibility() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I failed to get the first possibility item at assignment level " + CommonVariables.currentAssignmentLevel );

					do	{
						// Copy solve action of Constants.NO_ASSIGNMENT_LEVEL to higher levels
						if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
							actionSelectionItem = possibilityScoreItem.referenceSelectionItem;

						if( createNewAssignmentLevelInAllWords() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to create a new assignment level: " + CommonVariables.currentAssignmentLevel );

						CommonVariables.currentAssignmentLevel++;

						if( adminItem_.assignSelectionSpecification( possibilityScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to assign a selection specifcation at assignment level: " + CommonVariables.currentAssignmentLevel );

						tempEndSolveProgress = currentSolveProgress_ + solveProgressStep;

						if( adminItem_.executeSelection( (int)( currentSolveProgress_ + solveProgressStep / 2L ), actionSelectionItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to execute a selection during the solving of word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );

						// Word has active assignments
						if( solveWordItem.firstNonQuestionActiveAssignmentItem() != null )
							{
							isInverted = ( predefinedNounSolveMethodWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, Constants.NO_CONTEXT_NR, adminItem_.predefinedAdjectiveInvertedWordItem() ) != null );

							if( !isInverted &&
							CommonVariables.currentAssignmentLevel < solveLevel_ )
								{
								if( scoreList.deleteScores() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to delete the scores with an assignment level higher than " + CommonVariables.currentAssignmentLevel );

								// Don't solve any deeper if there is a winning score
								solveLevel_ = CommonVariables.currentAssignmentLevel;
								}

							// Create winning or losing score
							if( createScore( false, Constants.NO_SCORE, ( isInverted ? Constants.NO_SCORE : Constants.WINNING_SCORE ), Constants.NO_SCORE, ( isInverted ? Constants.WINNING_SCORE : Constants.NO_SCORE ), Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, actionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create a winning or losing score of solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );

							currentSolveProgress_ = tempEndSolveProgress;

							if( solveLevel_ > 1 )
								Presentation.displayProgress( currentSolveProgress_ );
							}
						else
							{
							if( solveWord( tempEndSolveProgress, solveWordItem, actionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );

							if( CommonVariables.currentAssignmentLevel == 1 )
								scoreList.changeAction( actionSelectionItem );
							}

						if( deleteAssignmentLevelInAllWords() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete the assignments of level " + CommonVariables.currentAssignmentLevel );

						CommonVariables.currentAssignmentLevel--;
						possibilityScoreItem = possibilityScoreItem.nextPossibilityScoreItem();

						if( ++possibilityNumber <= nPossibilities )
							{
							if( possibilityScoreItem != null &&
							possibilityNumber == nPossibilities )
								return adminItem_.startError( 1, moduleNameString_, "I have found more possibility items than number of possibilities" );
							}
						else
							{
							if( possibilityScoreItem == null )
								return adminItem_.startError( 1, moduleNameString_, "I couldn't get the next possibility item before the number of possibilities is reached" );
							}
						}
					while( possibilityScoreItem != null );

					if( nPossibilities > 1 ||
					// Higher level has possibilities
					CommonVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL )
						{
						if( scoreList.deleteScores() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete the scores with assignment level " + CommonVariables.currentAssignmentLevel );
						}
					}
				else
					{
					currentSolveProgress_ = endSolveProgress;

					if( solveLevel_ > 1 )
						Presentation.displayProgress( currentSolveProgress_ );
					}

				if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
					{
					if( ( scoreList = adminItem_.scoreList ) == null )
						return adminItem_.startError( 1, moduleNameString_, "The solve scores list isn't created yet" );

					if( ( specificationResult = predefinedNounSolveStrategyWordItem.getAssignmentWordParameter() ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve strategy at assignment level " + CommonVariables.currentAssignmentLevel );

					if( ( selectionResult = scoreList.getBestAction( adminItem_.isCurrentlyTesting(), specificationResult.assignmentParameter ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get the best action of solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );

					if( ( actionSelectionItem = selectionResult.bestActionItem ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't get the best action selection item" );

					if( adminItem_.assignSelectionSpecification( actionSelectionItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to assign a selection specification at assignment level " + CommonVariables.currentAssignmentLevel );

					if( adminItem_.assignSpecification( predefinedNounSolveMethodWordItem, adminItem_.predefinedAdjectiveDoneWordItem() ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to assign the done flag to the solve method at assignment level " + CommonVariables.currentAssignmentLevel );
					}
				}
			else
				{
				if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
					{
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_START, solveWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_END ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_START, solveWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Give thanks to the God of gods.
 *	His faithful love endures forever." (Psalm 136:2)
 *************************************************************************/
