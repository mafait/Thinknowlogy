/*
 *	Class:			AdminSolve
 *	Supports class:	AdminItem
 *	Purpose:		Trying to solve (= to assign) words according to the
 *					given selections
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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
	// Private constructible variables

	private boolean canWordBeSolved_;
	private boolean hasFoundPossibility_;
	private boolean hasFoundScoringAssignment_;
	private boolean isConditionSatisfied_;

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

	private SpecificationItem comparisonAssignmentItem_;
	private SpecificationItem firstComparisonAssignmentItem_;
	private SpecificationItem secondComparisonAssignmentItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static void clearAllWordSolveChecksInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.isWordCheckedForSolving = false;
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		}

	private static boolean isNumeralString( String checkString )
		{
		int stringLength;
		int position = 0;

		if( checkString != null &&
		( stringLength = checkString.length() ) > 0 )
			{
			while( position < stringLength &&
			Character.isDigit( checkString.charAt( position ) ) )
				position++;

			return ( position == stringLength );
			}

		return false;
		}

	private byte getComparisonAssignment( boolean isNumeralRelation, WordItem specificationWordItem, WordItem relationWordItem )
		{
		comparisonAssignmentItem_ = null;

		if( specificationWordItem != null )
			{
			if( isNumeralRelation )
				comparisonAssignmentItem_ = specificationWordItem.firstActiveNumeralAssignmentItem();
			else
				{
				if( relationWordItem == null )
					comparisonAssignmentItem_ = specificationWordItem.firstActiveStringAssignmentItem();
				else
					{
					if( specificationWordItem.isNounHead() )
						comparisonAssignmentItem_ = relationWordItem.lastActiveNonQuestionAssignmentItem();
					else
						{
						if( specificationWordItem.isNounTail() )
							comparisonAssignmentItem_ = relationWordItem.firstNonQuestionActiveAssignmentItem();
						else
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private static byte deleteAssignmentLevelInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.deleteAssignmentLevelInWord();
			while( CommonVariables.result == Constants.RESULT_OK &&
			( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return CommonVariables.result;
		}

	private byte backTrackConditionScorePaths( boolean isAddingScores, boolean isInverted, boolean isAllowingDuplicates, boolean isPreparingSort, short executionLevel, short solveStrategyParameter, int conditionSentenceNr )
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

		if( ( conditionList = adminItem_.conditionList ) != null )
			{
			conditionList.clearConditionChecksForSolving( Constants.MAX_LEVEL, conditionSentenceNr );

			do	{
				isWaitingForNewLevel = false;
				isWaitingForNewStart = false;
				handledSelectionLevel = Constants.MAX_LEVEL;
				previousSelectionLevel = Constants.NO_SELECTION_LEVEL;
				unhandledSelectionLevel = Constants.MAX_LEVEL;
				previousConditionSelectionItem = null;

				if( ( conditionSelectionItem = conditionList.firstConditionSelectionItem( conditionSentenceNr ) ) != null )
					{
					if( isAddingScores )
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

								// Second brance on the same level
								if( selectionLevel == previousSelectionLevel )
									{
									if( !isWaitingForNewLevel )
										{
										if( handledSelectionLevel == Constants.MAX_LEVEL )
											{
											handledSelectionLevel = selectionLevel;

											// This is a new brance
											if( previousConditionSelectionItem.isConditionCheckedForSolving )
												conditionSelectionItem.isConditionCheckedForSolving = true;
											else
												{
												// The previous brance was a new brance and this one is unhandled
												isWaitingForNewLevel = true;
												unhandledSelectionLevel = selectionLevel;
												previousConditionSelectionItem.isConditionCheckedForSolving = true;
												}
											}
										else
											{
											isWaitingForNewLevel = true;

											// This brance isn't handled yet
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

					if( previousSelectionLevel == executionLevel )
						{
						// All branches on same level are done and there are brances on a higher level unhandled,
						// so start again with the handled branches (by clearing their checks) until all paths are handled
						if( unhandledSelectionLevel < Constants.MAX_LEVEL &&
						handledSelectionLevel < unhandledSelectionLevel )
							conditionList.clearConditionChecksForSolving( unhandledSelectionLevel, conditionSentenceNr );

						if( isAddingScores )
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
								if( scoreList.checkScores( isInverted, solveStrategyParameter, oldSatisfiedScore_, newSatisfiedScore_, oldDissatisfiedScore_, newDissatisfiedScore_, oldNotBlockingScore_, newNotBlockingScore_, oldBlockingScore_, newBlockingScore_ ) == Constants.RESULT_OK )
									hasFoundScore = scoreList.hasFoundScore();
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to check the scores" );
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
					else
						return adminItem_.startError( 1, moduleNameString_, "I couldn't reach the given execution level " + executionLevel + ". The highest reached level was " + handledSelectionLevel );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "I couldn't get the first item of the condition with sentence number " + conditionSentenceNr );
				}
			while( unhandledSelectionLevel < Constants.MAX_LEVEL );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The condition list isn't created yet" );

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
				if( ( selectionResult = actionList.findFirstExecutionItem( solveWordItem ) ).result == Constants.RESULT_OK )
					currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to find the first action execution selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
				}
			}
		else
			{
			if( ( alternativeList = adminItem_.alternativeList ) != null )
				{
				if( ( selectionResult = alternativeList.findFirstExecutionItem( solveWordItem ) ).result == Constants.RESULT_OK )
					currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to find the first alternative execution selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
				}
			}

		if( currentExecutionSelectionItem_ != null )
			{
			do	{
				if( ( specificationWordItem = currentExecutionSelectionItem_.specificationWordItem() ) != null )
					{
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

					if( currentExecutionSelectionItem_.findNextExecutionSelectionItem( solveWordItem ) == Constants.RESULT_OK )
						currentExecutionSelectionItem_ = currentExecutionSelectionItem_.nextExecutionItem();
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to find the next execution selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The specification word of the current execution selection item is undefined" );
				}
			while( !canWordBeSolved_ );
			}

		currentExecutionSelectionItem_ = originalExecutionSelectionItem;

		return Constants.RESULT_OK;
		}

	private byte canWordBeSolved( WordItem solveWordItem )
		{
		if( canWordBeSolved( true, solveWordItem ) == Constants.RESULT_OK )
			{
			if( !canWordBeSolved_ &&
			!CommonVariables.hasShownWarning )
				{
				if( canWordBeSolved( false, solveWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a word can be solved by an alternative action" );
				}
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a word can be solved by an action" );

		return Constants.RESULT_OK;
		}

	private byte checkComparison( SelectionItem conditionSelectionItem )
		{
		boolean isNegative;
		boolean isNumeralRelation;
		int comparisonResult = 0;
		int firstNumeral = 0;
		int secondNumeral = 0;
		String firstString = null;
		String secondString = null;
		WordItem generalizationWordItem;
		WordItem firstSpecificationWordItem;
		WordItem relationWordItem;
		WordItem secondSpecificationWordItem;
		WordItem specificationWordItem;
		WordItem comparisonAssignmentSpecificationWordItem = null;

		isConditionSatisfied_ = false;

		if( conditionSelectionItem != null )
			{
			if( ( generalizationWordItem = conditionSelectionItem.generalizationWordItem() ) != null )
				{
				if( ( specificationWordItem = conditionSelectionItem.specificationWordItem() ) != null )
					{
					isNegative = conditionSelectionItem.isNegative();
					relationWordItem = conditionSelectionItem.relationWordItem();

					// First part
					if( conditionSelectionItem.isFirstComparisonPart() )
						{
						if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) == Constants.RESULT_OK )
							{
							// Allow the second part of the comparison to be checked
							isConditionSatisfied_ = true;

							firstComparisonAssignmentItem_ = comparisonAssignmentItem_;
							secondComparisonAssignmentItem_ = null;
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed get the first comparison assignment" );
						}
					else
						{
						isNumeralRelation = conditionSelectionItem.isNumeralRelation();

						if( !isNumeralRelation &&
						// Second part
						conditionSelectionItem.specificationString() == null )
							{
							if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) == Constants.RESULT_OK )
								{
								secondComparisonAssignmentItem_ = comparisonAssignmentItem_;

								firstSpecificationWordItem = ( firstComparisonAssignmentItem_ == null ? null : firstComparisonAssignmentItem_.specificationWordItem() );
								secondSpecificationWordItem = ( secondComparisonAssignmentItem_ == null ? null : secondComparisonAssignmentItem_.specificationWordItem() );

								firstString = ( firstSpecificationWordItem == null ? null : firstSpecificationWordItem.anyWordTypeString() );
								secondString = ( secondSpecificationWordItem == null ? null : secondSpecificationWordItem.anyWordTypeString() );
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed get a comparison assignment" );
							}
						else
							{
							// Numeral or specification string
							if( getComparisonAssignment( isNumeralRelation, specificationWordItem, relationWordItem ) == Constants.RESULT_OK )
								{
								if( comparisonAssignmentItem_ != null )
									comparisonAssignmentSpecificationWordItem = comparisonAssignmentItem_.specificationWordItem();

								firstString = ( comparisonAssignmentItem_ == null ? null : ( isNumeralRelation ? ( comparisonAssignmentSpecificationWordItem == null ? null : comparisonAssignmentSpecificationWordItem.anyWordTypeString() ) : comparisonAssignmentItem_.specificationString() ) );
								secondString = ( isNumeralRelation ? ( relationWordItem == null ? null : relationWordItem.anyWordTypeString() ) : conditionSelectionItem.specificationString() );
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed get the first comparison assignment" );
							}

						if( firstString == null ||
						secondString == null )
							comparisonResult = ( firstString == null && secondString == null ? 0 : ( firstString == null ? -1 : 1 ) );
						else
							{
							if( isNumeralString( firstString ) &&
							isNumeralString( secondString ) )
								{
								firstNumeral = Integer.parseInt( firstString );
								secondNumeral = Integer.parseInt( secondString );

								comparisonResult = ( firstNumeral == secondNumeral ? 0 : ( firstNumeral < secondNumeral ? -1 : 1 ) );
								}
							else
								comparisonResult = firstString.compareTo( secondString );
							}

						if( generalizationWordItem.isAdjectiveComparisonLess() )
							isConditionSatisfied_ = ( comparisonResult < 0 ? !isNegative : isNegative );
						else
							{
							if( generalizationWordItem.isAdjectiveComparisonEqual() )
								isConditionSatisfied_ = ( comparisonResult == 0 ? !isNegative : isNegative );
							else
								{
								if( generalizationWordItem.isAdjectiveComparisonMore() )
									isConditionSatisfied_ = ( comparisonResult > 0 ? !isNegative : isNegative );
								else
									return adminItem_.startError( 1, moduleNameString_, "Word \"" + generalizationWordItem.anyWordTypeString() + "\" isn't comparison word" );
								}
							}
						}
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The specification word of the given condition selection item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given condition selection item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given condition selection item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkForOddOrEven( SelectionItem conditionSelectionItem )
		{
		boolean isNegative;
		int nAssignments;
		int relationContextNr;
		WordItem generalizationWordItem;
		WordItem specificationWordItem;
		WordItem relationWordItem;

		isConditionSatisfied_ = false;

		if( conditionSelectionItem != null )
			{
			if( ( generalizationWordItem = conditionSelectionItem.generalizationWordItem() ) != null )
				{
				if( ( specificationWordItem = conditionSelectionItem.specificationWordItem() ) != null )
					{
					isNegative = conditionSelectionItem.isNegative();
					relationContextNr = conditionSelectionItem.relationContextNr();

					if( ( relationWordItem = adminItem_.contextWordItemInAllWords( relationContextNr, specificationWordItem, null ) ) != null )
						{
						if( specificationWordItem.isNounNumber() )
							{
							nAssignments = relationWordItem.nActiveAssignments();

							if( generalizationWordItem.isAdjectiveOdd() )
								isConditionSatisfied_ = ( nAssignments % 2 == 1 ? !isNegative : isNegative );
							else
								{
								if( generalizationWordItem.isAdjectiveEven() )
									isConditionSatisfied_ = ( nAssignments % 2 == 0 ? !isNegative : isNegative );
								else
									return adminItem_.startError( 1, moduleNameString_, "Word \"" + generalizationWordItem.anyWordTypeString() + "\" isn't about odd or even" );
								}
							}
						else
							{
							if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, relationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "I couldn't find the relation word" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The specification word of the given condition selection item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given condition selection item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given condition selection item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte createScore( boolean isChecked, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, SelectionItem referenceSelectionItem )
		{
		if( adminItem_.scoreList == null )
			{
			// Create list
			if( ( adminItem_.scoreList = new ScoreList( adminItem_ ) ) != null )
				{
				CommonVariables.adminScoreList = adminItem_.scoreList;
				adminItem_.adminListArray[Constants.ADMIN_SCORE_LIST] = adminItem_.scoreList;
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "I failed to create the admin solve score list" );
			}

		if( adminItem_.scoreList.createScoreItem( isChecked, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to create a score item" );

		return Constants.RESULT_OK;
		}

	private byte findScoringAssignment( boolean isBlocking, WordItem generalizationWordItem )
		{
		SpecificationItem currentAssignmentItem;

		hasFoundScoringAssignment_ = false;

		if( generalizationWordItem != null )
			{
			if( ( currentAssignmentItem = generalizationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
				{
				do	{
					if( !currentAssignmentItem.isNegative() )
						{
						hasFoundScoringAssignment_ = true;

						if( isBlocking )
							{
							if( currentAssignmentItem.isOlderItem() )
								oldBlockingScore_++;
							else
								newBlockingScore_++;
							}
						else
							{
							if( currentAssignmentItem.isOlderItem() )
								oldNotBlockingScore_++;
							else
								newNotBlockingScore_++;
							}
						}
					}
				while( !hasFoundScoringAssignment_ &&
				( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte findScoringAssignment( boolean isPossessive, boolean isSatisfiedScore, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem currentAssignmentItem;

		hasFoundScoringAssignment_ = false;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( ( currentAssignmentItem = generalizationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
					{
					do	{
						if( currentAssignmentItem.isRelatedSpecification( false, isPossessive, relationContextNr, specificationWordItem ) )
							{
							hasFoundScoringAssignment_ = true;

							if( isSatisfiedScore )
								{
								if( currentAssignmentItem.isOlderItem() )
									oldSatisfiedScore_++;
								else
									newSatisfiedScore_++;
								}
							else
								{
								if( currentAssignmentItem.isOlderItem() )
									oldDissatisfiedScore_++;
								else
									newDissatisfiedScore_++;
								}
							}
						}
					while( !hasFoundScoringAssignment_ &&
					( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
					}
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkConditionByValue( boolean isNegative, boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isSatisfiedScore;
		SpecificationItem foundAssignmentItem;
		SpecificationItem currentSpecificationItem;

		isConditionSatisfied_ = true;

		if( specificationWordItem != null )
			{
			if( ( currentSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) != null )
				{
				do	{
					foundAssignmentItem = specificationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, isPossessive, currentSpecificationItem.relationContextNr(), currentSpecificationItem.specificationWordItem() );
					isSatisfiedScore = ( isNegative == ( foundAssignmentItem == null || foundAssignmentItem.isNegative() ) );

					if( findScoringAssignment( isPossessive, isSatisfiedScore, currentSpecificationItem.relationContextNr(), generalizationWordItem, currentSpecificationItem.specificationWordItem() ) == Constants.RESULT_OK )
						{
						if( hasFoundScoringAssignment_ != isSatisfiedScore )
							isConditionSatisfied_ = false;
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a scoring assignment" );
					}
				while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte calculateScorePaths( boolean isInverted, boolean isAllowingDuplicates, boolean isPreparingSort, short solveStrategyParameter, SelectionItem referenceSelectionItem )
		{
		boolean isAddLocalScores;
		boolean isOriginalFoundPossibility;
		boolean hasFoundScore = false;
		int localOldSatisfiedScore;
		int localNewSatisfiedScore;
		int localOldDissatisfiedScore;
		int localNewDissatisfiedScore;
		int localOldNotBlockingScore;
		int localNewNotBlockingScore;
		int localOldBlockingScore;
		int localNewBlockingScore;
		int oldSatisfiedScore = oldSatisfiedScore_;
		int newSatisfiedScore = newSatisfiedScore_;
		int oldDissatisfiedScore = oldDissatisfiedScore_;
		int newDissatisfiedScore = newDissatisfiedScore_;
		int oldNotBlockingScore = oldNotBlockingScore_;
		int newNotBlockingScore = newNotBlockingScore_;
		int oldBlockingScore = oldBlockingScore_;
		int newBlockingScore = newBlockingScore_;
		WordItem generalizationWordItem;
		WordItem specificationWordItem;
		ScoreList scoreList;

		if( referenceSelectionItem != null )
			{
			if( ( generalizationWordItem = referenceSelectionItem.generalizationWordItem() ) != null )
				{
				oldSatisfiedScore_ = Constants.NO_SCORE;
				newSatisfiedScore_ = Constants.NO_SCORE;
				oldDissatisfiedScore_ = Constants.NO_SCORE;
				newDissatisfiedScore_ = Constants.NO_SCORE;
				oldNotBlockingScore_ = Constants.NO_SCORE;
				newNotBlockingScore_ = Constants.NO_SCORE;
				oldBlockingScore_ = Constants.NO_SCORE;
				newBlockingScore_ = Constants.NO_SCORE;

				if( checkCondition( referenceSelectionItem ).result == Constants.RESULT_OK )
					{
					isAddLocalScores = true;

					localOldSatisfiedScore = oldSatisfiedScore_;
					localNewSatisfiedScore = newSatisfiedScore_;
					localOldDissatisfiedScore = oldDissatisfiedScore_;
					localNewDissatisfiedScore = newDissatisfiedScore_;
					localOldNotBlockingScore = oldNotBlockingScore_;
					localNewNotBlockingScore = newNotBlockingScore_;
					localOldBlockingScore = oldBlockingScore_;
					localNewBlockingScore = newBlockingScore_;

					oldSatisfiedScore_ = oldSatisfiedScore;
					newSatisfiedScore_ = newSatisfiedScore;
					oldDissatisfiedScore_ = oldDissatisfiedScore;
					newDissatisfiedScore_ = newDissatisfiedScore;
					oldNotBlockingScore_ = oldNotBlockingScore;
					newNotBlockingScore_ = newNotBlockingScore;
					oldBlockingScore_ = oldBlockingScore;
					newBlockingScore_ = newBlockingScore;

					if( !isConditionSatisfied_ &&
					!generalizationWordItem.isWordCheckedForSolving )
						{
						if( referenceSelectionItem.isAssignedOrClear() )
							{
							if( !referenceSelectionItem.isNegative() )
								{
								// Word has no active assignments
								if( generalizationWordItem.firstNonQuestionActiveAssignmentItem() == null )
									{
									isOriginalFoundPossibility = hasFoundPossibility_;

									if( adminItem_.findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) == Constants.RESULT_OK )
										{
										if( hasFoundPossibility_ )
											isAddLocalScores = false;
										else
											hasFoundPossibility_ = isOriginalFoundPossibility;
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to find a possibility to solve condition word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						else
							{
							if( ( specificationWordItem = referenceSelectionItem.specificationWordItem() ) != null )
								{
								if( !referenceSelectionItem.isNegative() &&
								!specificationWordItem.isWordCheckedForSolving &&
								// Word has no active assignments
								generalizationWordItem.firstNonQuestionActiveAssignmentItem() == null )
									{
									if( canWordBeSolved( specificationWordItem ) == Constants.RESULT_OK )
										{
										if( canWordBeSolved_ )
											{
											isOriginalFoundPossibility = hasFoundPossibility_;

											if( adminItem_.findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) == Constants.RESULT_OK )
												{
												if( hasFoundPossibility_ )
													isAddLocalScores = false;
												else
													hasFoundPossibility_ = isOriginalFoundPossibility;
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to find a possibility to solve condition word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											scoreList = adminItem_.scoreList;

											if( isAllowingDuplicates ||
											scoreList == null )
												hasFoundScore = false;
											else
												{
												if( scoreList.findScore( isPreparingSort, referenceSelectionItem ) == Constants.RESULT_OK )
													hasFoundScore = scoreList.hasFoundScore();
												else
													return adminItem_.addError( 1, moduleNameString_, "I failed to find a score item" );
												}

											if( !hasFoundScore )
												{
												if( createScore( ( CommonVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL ), Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, referenceSelectionItem ) == Constants.RESULT_OK )
													hasFoundPossibility_ = true;
												else
													return adminItem_.addError( 1, moduleNameString_, "I failed to create an empty solve score" );
												}
											}
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to find out if specification word \"" + specificationWordItem.anyWordTypeString() + "\" can be solved" );
									}
								}
							else
								return adminItem_.startError( 1, moduleNameString_, "The specification word of the given score item is undefined" );
							}
						}

					if( !CommonVariables.hasShownWarning &&
					isAddLocalScores )
						{
						oldSatisfiedScore_ += localOldSatisfiedScore;
						newSatisfiedScore_ += localNewSatisfiedScore;
						oldDissatisfiedScore_ += localOldDissatisfiedScore;
						newDissatisfiedScore_ += localNewDissatisfiedScore;
						oldNotBlockingScore_ += localOldNotBlockingScore;
						newNotBlockingScore_ += localNewNotBlockingScore;
						oldBlockingScore_ += localOldBlockingScore;
						newBlockingScore_ += localNewBlockingScore;
						}
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to check the condition of the item with sentence number " + referenceSelectionItem.activeSentenceNr() + " and item number " + referenceSelectionItem.itemNr() );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given score item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given reference selection item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminSolve( AdminItem adminItem )
		{
		String errorString = null;

		canWordBeSolved_ = false;
		hasFoundPossibility_ = false;
		hasFoundScoringAssignment_ = false;
		isConditionSatisfied_ = false;

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
		comparisonAssignmentItem_ = null;
		firstComparisonAssignmentItem_ = null;
		secondComparisonAssignmentItem_ = null;

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

	protected byte findPossibilityToSolveWord( boolean isAddingScores, boolean isAllowingDuplicates, boolean isInverted, boolean isPreparingSort, short solveStrategyParameter, WordItem solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem originalExecutionSelectionItem;
		SelectionList actionList;
		SelectionList alternativeList;

		hasFoundPossibility_ = false;

		if( solveWordItem != null )
			{
			if( !solveWordItem.isWordCheckedForSolving )
				{
				if( canWordBeSolved( true, solveWordItem ) == Constants.RESULT_OK )
					{
					solveWordItem.isWordCheckedForSolving = true;

					if( canWordBeSolved_ &&
					( actionList = adminItem_.actionList ) != null )
						{
						originalExecutionSelectionItem = currentExecutionSelectionItem_;

						if( ( selectionResult = actionList.findFirstExecutionItem( solveWordItem ) ).result == Constants.RESULT_OK )
							{
							if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != null )
								{
								do	{
									if( backTrackConditionScorePaths( isAddingScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_.selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_.activeSentenceNr() ) == Constants.RESULT_OK )
										{
										if( currentExecutionSelectionItem_.findNextExecutionSelectionItem( solveWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to find the next action selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to back-fire the condition score paths for the action with sentence number " + currentExecutionSelectionItem_.activeSentenceNr() );
									}
								while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_.nextExecutionItem() ) != null );
								}

							currentExecutionSelectionItem_ = originalExecutionSelectionItem;
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to find the first action selection item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
						}

					if( !CommonVariables.hasShownWarning )
						{
						if( canWordBeSolved( false, solveWordItem ) == Constants.RESULT_OK )
							{
							if( canWordBeSolved_ &&
							( alternativeList = adminItem_.alternativeList ) != null )
								{
								originalExecutionSelectionItem = currentExecutionSelectionItem_;

								if( ( selectionResult = alternativeList.findFirstExecutionItem( solveWordItem ) ).result == Constants.RESULT_OK )
									{
									if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != null )
										{
										do	{
											if( backTrackConditionScorePaths( isAddingScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_.selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_.activeSentenceNr() ) == Constants.RESULT_OK )
												{
												if( currentExecutionSelectionItem_.findNextExecutionSelectionItem( solveWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to find the next alternative item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to back-fire the condition score paths for the alternative with sentence number " + currentExecutionSelectionItem_.activeSentenceNr() );
											}
										while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_.nextExecutionItem() ) != null );
										}

									currentExecutionSelectionItem_ = originalExecutionSelectionItem;
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to find the first alternative item with solve word \"" + solveWordItem.anyWordTypeString() + "\"" );
								}

							solveWordItem.isWordCheckedForSolving = false;
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to find out if the given word \"" + solveWordItem.anyWordTypeString() + "\" can be solved by alternative" );
						}
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to find out if the given word \"" + solveWordItem.anyWordTypeString() + "\" can be solved by action" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given solve word \"" + solveWordItem.anyWordTypeString() + "\" is already checked" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given solve word is undefined" );

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
		ScoreItem possibilityItem;
		WordItem predefinedNounSolveLevelWordItem;
		WordItem predefinedNounSolveMethodWordItem;
		WordItem predefinedNounSolveStrategyWordItem;
		ScoreList scoreList;
		WordList wordList;

		if( solveWordItem != null )
			{
			if( ( predefinedNounSolveMethodWordItem = adminItem_.predefinedNounSolveMethodWordItem() ) != null )
				{
				if( ( predefinedNounSolveStrategyWordItem = adminItem_.predefinedNounSolveStrategyWordItem() ) != null )
					{
					if( currentSolveProgress_ == 0 )
						{
						if( ( predefinedNounSolveLevelWordItem = adminItem_.predefinedNounSolveLevelWordItem() ) == null )
							solveLevel_ = Constants.NO_SOLVE_LEVEL;
						else
							{
							if( ( specificationResult = predefinedNounSolveLevelWordItem.getAssignmentOrderNr() ).result == Constants.RESULT_OK )
								solveLevel_ = specificationResult.assignmentOrderNr;
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve level" );
							}
						}

					// Word has no active assignments
					if( solveWordItem.firstNonQuestionActiveAssignmentItem() == null )
						{
						if( CommonVariables.currentAssignmentLevel <= solveLevel_ )
							{
							if( adminItem_.assignSpecification( predefinedNounSolveMethodWordItem, adminItem_.predefinedAdjectiveBusyWordItem() ) == Constants.RESULT_OK )
								{
								if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
									{
									clearAllWordSolveChecksInAllWords();

									if( ( scoreList = adminItem_.scoreList ) != null )
										{
										// Make sure no scores are left at the start
										if( scoreList.deleteScores() != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to delete the admin score list" );
										}
									}

								isInverted = ( predefinedNounSolveMethodWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, Constants.NO_CONTEXT_NR, adminItem_.predefinedAdjectiveInvertedWordItem() ) != null );

								if( ( specificationResult = predefinedNounSolveStrategyWordItem.getAssignmentWordParameter() ).result == Constants.RESULT_OK )
									{
									if( findPossibilityToSolveWord( true, ( CommonVariables.currentAssignmentLevel == solveLevel_ ), isInverted, ( CommonVariables.currentAssignmentLevel + 1 < solveLevel_ ), specificationResult.assignmentParameter, solveWordItem ) == Constants.RESULT_OK )
										{
										if( hasFoundPossibility_ )
											{
											if( CommonVariables.currentAssignmentLevel < solveLevel_ )
												{
												if( ( scoreList = adminItem_.scoreList ) != null )
													{
													nPossibilities = scoreList.nPossibilities();
													solveProgressStep = ( ( endSolveProgress - currentSolveProgress_ ) / nPossibilities );

													if( solveLevel_ > 1 )
														Presentation.startProgress( currentSolveProgress_, Constants.MAX_PROGRESS, Constants.INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_START, solveLevel_, Constants.INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_END );

													if( ( wordList = adminItem_.wordList ) != null )
														{
														if( ( possibilityItem = scoreList.firstPossibility() ) != null )
															{
															do	{
																// Copy solve action of Constants.NO_ASSIGNMENT_LEVEL to higher levels
																if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
																	actionSelectionItem = possibilityItem.scoreReference();

																if( wordList.createNewAssignmentLevelInWordList() == Constants.RESULT_OK )
																	{
																	CommonVariables.currentAssignmentLevel++;

																	if( adminItem_.assignSelectionSpecification( possibilityItem.scoreReference() ) == Constants.RESULT_OK )
																		{
																		tempEndSolveProgress = currentSolveProgress_ + solveProgressStep;

																		if( adminItem_.executeSelection( (int)( currentSolveProgress_ + solveProgressStep / 2L ), actionSelectionItem ) == Constants.RESULT_OK )
																			{
																			// Word has active assignments
																			if( solveWordItem.firstNonQuestionActiveAssignmentItem() != null )
																				{
																				isInverted = ( predefinedNounSolveMethodWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, Constants.NO_CONTEXT_NR, adminItem_.predefinedAdjectiveInvertedWordItem() ) != null );

																				if( !isInverted &&
																				CommonVariables.currentAssignmentLevel < solveLevel_ )
																					{
																					if( scoreList.deleteScores() == Constants.RESULT_OK )
																						// Don't solve any deeper if there is a winning score
																						solveLevel_ = CommonVariables.currentAssignmentLevel;
																					else
																						return adminItem_.addError( 1, moduleNameString_, "I failed to delete the scores with an assignment level higher than " + CommonVariables.currentAssignmentLevel );
																					}

																				// Create winning or losing score
																				if( createScore( false, Constants.NO_SCORE, ( isInverted ? Constants.NO_SCORE : Constants.WINNING_SCORE ), Constants.NO_SCORE, ( isInverted ? Constants.WINNING_SCORE : Constants.NO_SCORE ), Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, actionSelectionItem ) == Constants.RESULT_OK )
																					{
																					currentSolveProgress_ = tempEndSolveProgress;

																					if( solveLevel_ > 1 )
																						Presentation.showProgress( currentSolveProgress_ );
																					}
																				else
																					return adminItem_.addError( 1, moduleNameString_, "I failed to create a winning or losing score of solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );
																				}
																			else
																				{
																				if( solveWord( tempEndSolveProgress, solveWordItem, actionSelectionItem ) == Constants.RESULT_OK )
																					{
																					if( CommonVariables.currentAssignmentLevel == 1 )
																						scoreList.changeAction( actionSelectionItem );
																					}
																				else
																					return adminItem_.addError( 1, moduleNameString_, "I failed to solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );
																				}

																			if( deleteAssignmentLevelInAllWords() == Constants.RESULT_OK )
																				{
																				CommonVariables.currentAssignmentLevel--;
																				possibilityItem = possibilityItem.nextPossibilityItem();

																				if( ++possibilityNumber <= nPossibilities )
																					{
																					if( possibilityItem != null &&
																					possibilityNumber == nPossibilities )
																						return adminItem_.startError( 1, moduleNameString_, "I have found more possibility items than number of possibilities" );
																					}
																				else
																					{
																					if( possibilityItem == null )
																						return adminItem_.startError( 1, moduleNameString_, "I couldn't get the next possibility item before the number of possibilities is reached" );
																					}
																				}
																			else
																				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the assignments of level " + CommonVariables.currentAssignmentLevel );
																			}
																		else
																			return adminItem_.addError( 1, moduleNameString_, "I failed to execute a selection during the solving of word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );
																		}
																	else
																		return adminItem_.addError( 1, moduleNameString_, "I failed to assign a selection specifcation at assignment level: " + CommonVariables.currentAssignmentLevel );
																	}
																else
																	return adminItem_.addError( 1, moduleNameString_, "I failed to create a new assignment level: " + CommonVariables.currentAssignmentLevel );
																}
															while( possibilityItem != null );

															if( nPossibilities > 1 ||
															// Higher level has possibilities
															CommonVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL )
																{
																if( scoreList.deleteScores() != Constants.RESULT_OK )
																	return adminItem_.addError( 1, moduleNameString_, "I failed to delete the scores with assignment level " + CommonVariables.currentAssignmentLevel );
																}
															}
														else
															return adminItem_.startError( 1, moduleNameString_, "I failed to get the first possibility item at assignment level " + CommonVariables.currentAssignmentLevel );
														}
													else
														return adminItem_.startError( 1, moduleNameString_, "The word list isn't created yet" );
													}
												else
													return adminItem_.startError( 1, moduleNameString_, "The solve scores list isn't created yet at assignment level " + CommonVariables.currentAssignmentLevel );
												}
											else
												{
												currentSolveProgress_ = endSolveProgress;

												if( solveLevel_ > 1 )
													Presentation.showProgress( currentSolveProgress_ );
												}

											if( CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
												{
												if( ( scoreList = adminItem_.scoreList ) != null )
													{
													if( ( specificationResult = predefinedNounSolveStrategyWordItem.getAssignmentWordParameter() ).result == Constants.RESULT_OK )
														{
														if( ( selectionResult = scoreList.getBestAction( adminItem_.isTesting(), specificationResult.assignmentParameter ) ).result == Constants.RESULT_OK )
															{
															if( ( actionSelectionItem = selectionResult.bestActionItem ) != null )
																{
																if( adminItem_.assignSelectionSpecification( actionSelectionItem ) == Constants.RESULT_OK )
																	{
																	if( adminItem_.assignSpecification( predefinedNounSolveMethodWordItem, adminItem_.predefinedAdjectiveDoneWordItem() ) != Constants.RESULT_OK )
																		return adminItem_.addError( 1, moduleNameString_, "I failed to assign the done flag to the solve method at assignment level " + CommonVariables.currentAssignmentLevel );
																	}
																else
																	return adminItem_.addError( 1, moduleNameString_, "I failed to assign a selection specification at assignment level " + CommonVariables.currentAssignmentLevel );
																}
															else
																return adminItem_.startError( 1, moduleNameString_, "I couldn't get the best action selection item" );
															}
														else
															return adminItem_.addError( 1, moduleNameString_, "I failed to get the best action of solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );
														}
													else
														return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve strategy at assignment level " + CommonVariables.currentAssignmentLevel );
													}
												else
													return adminItem_.startError( 1, moduleNameString_, "The solve scores list isn't created yet" );
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
										return adminItem_.addError( 1, moduleNameString_, "I failed to find a possibility to solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + CommonVariables.currentAssignmentLevel );
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve strategy at assignment level " + CommonVariables.currentAssignmentLevel );
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to assign the busy flag to the solve method at assignment level " + CommonVariables.currentAssignmentLevel );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The given assignment level of " + CommonVariables.currentAssignmentLevel + " is higher than the given solve level " + solveLevel_ );
						}
					else
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_START, solveWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_END ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The predefined solve strategy noun word item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The predefined solve-method noun word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given solve word is undefined" );

		return Constants.RESULT_OK;
		}

	protected SelectionResultType checkCondition( SelectionItem conditionSelectionItem )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		boolean isPossessive;
		boolean isNegative;
		WordItem generalizationWordItem;
		WordItem specificationWordItem;

		isConditionSatisfied_ = false;

		if( conditionSelectionItem != null )
			{
			if( ( generalizationWordItem = conditionSelectionItem.generalizationWordItem() ) != null )
				{
				if( ( specificationWordItem = conditionSelectionItem.specificationWordItem() ) != null )
					{
					isPossessive = conditionSelectionItem.isPossessive();
					isNegative = conditionSelectionItem.isNegative();

					if( conditionSelectionItem.isAssignedOrClear() )
						{
						if( specificationWordItem.isAdjectiveClear() )
							{
							// Adjective "clear"
							if( findScoringAssignment( !isNegative, generalizationWordItem ) == Constants.RESULT_OK )
								isConditionSatisfied_ = ( hasFoundScoringAssignment_ ? isNegative : !isNegative );
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to find a scoring assignment" );
							}
						else
							{
							// Adjective "assigned"
							if( findScoringAssignment( isNegative, generalizationWordItem ) == Constants.RESULT_OK )
								isConditionSatisfied_ = ( hasFoundScoringAssignment_ ? !isNegative : isNegative );
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to find a scoring assignment" );
							}
						}
					else
						{
						if( generalizationWordItem.isAdjectiveComparison() )
							{
							if( checkComparison( conditionSelectionItem ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to check a comparison" );
							}
						else
							{
							if( generalizationWordItem.isAdjectiveOddOrEven() )
								{
								if( checkForOddOrEven( conditionSelectionItem ) != Constants.RESULT_OK )
									adminItem_.addError( 1, moduleNameString_, "I failed to check for odd or even" );
								}
							else
								{
								if( conditionSelectionItem.isValueSpecification() )
									{
									if( checkConditionByValue( isNegative, isPossessive, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										adminItem_.addError( 1, moduleNameString_, "I failed to check the condition of a specification by value" );
									}
								else
									{
									if( findScoringAssignment( isPossessive, !isNegative, conditionSelectionItem.relationContextNr(), generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
										{
										if( hasFoundScoringAssignment_ != isNegative )
											isConditionSatisfied_ = true;
										}
									else
										adminItem_.addError( 1, moduleNameString_, "I failed to find a scoring assignment" );
									}
								}
							}
						}
					}
				else
					adminItem_.startError( 1, moduleNameString_, "The specification word of the given condition selection item is undefined" );
				}
			else
				adminItem_.startError( 1, moduleNameString_, "The generalization word of the given condition selection item is undefined" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given condition selection item is undefined" );

		selectionResult.isConditionSatisfied = isConditionSatisfied_;
		selectionResult.result = CommonVariables.result;
		return selectionResult;
		}
	};

/*************************************************************************
 *	"Give thanks to the God of gods.
 *	His faithful love endures forever." (Psalm 136:2)
 *************************************************************************/
