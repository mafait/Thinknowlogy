/*	Class:			AdminSolve
 *	Supports class:	AdminItem
 *	Purpose:		Trying to solve (= to assign) words according to the
 *					given selections
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
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

	// Private constructible variables

	bool canWordBeSolved_;
	bool hasFoundPossibility_;
	bool hasFoundScoringAssignment_;
	bool isConditionSatisfied_;

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

	SpecificationItem *comparisonAssignmentItem_;
	SpecificationItem *firstComparisonAssignmentItem_;
	SpecificationItem *secondComparisonAssignmentItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isNumeralString( char *checkString )
		{
		size_t stringLength;
		size_t position = 0;

		if( checkString != NULL &&
		( stringLength = strlen( checkString ) ) > 0 )
			{
			while( position < stringLength &&
			isdigit( checkString[position] ) )
				position++;

			return ( position == stringLength );
			}

		return false;
		}

	ResultType createNewAssignmentLevelInAllWords()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all active words
			do	currentWordItem->createNewAssignmentLevelInWord();
			while( commonVariables_->result == RESULT_OK &&
			( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return commonVariables_->result;
		}

	ResultType deleteAssignmentLevelInAllWords()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all active words
			do	currentWordItem->deleteAssignmentLevelInWord();
			while( commonVariables_->result == RESULT_OK &&
			( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return commonVariables_->result;
		}

	ResultType getComparisonAssignment( bool isNumeralRelation, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getComparisonAssignment";

		comparisonAssignmentItem_ = NULL;

		if( specificationWordItem != NULL )
			{
			if( isNumeralRelation )
				comparisonAssignmentItem_ = specificationWordItem->firstActiveNumeralAssignmentItem();
			else
				{
				if( relationWordItem == NULL )
					comparisonAssignmentItem_ = specificationWordItem->firstActiveStringAssignmentItem();
				else
					{
					if( specificationWordItem->isNounHead() )
						comparisonAssignmentItem_ = relationWordItem->lastActiveNonQuestionAssignmentItem();
					else
						{
						if( specificationWordItem->isNounTail() )
							comparisonAssignmentItem_ = relationWordItem->firstNonQuestionActiveAssignmentItem();
						else
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		return RESULT_OK;
		}

	ResultType backTrackConditionScorePaths( bool isAddingScores, bool isInverted, bool isAllowingDuplicates, bool isPreparingSort, unsigned short executionLevel, unsigned short solveStrategyParameter, unsigned int conditionSentenceNr )
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

		if( ( conditionList = adminItem_->conditionList ) != NULL )
			{
			conditionList->clearConditionChecksForSolving( MAX_LEVEL, conditionSentenceNr );

			do	{
				isWaitingForNewLevel = false;
				isWaitingForNewStart = false;
				handledSelectionLevel = MAX_LEVEL;
				previousSelectionLevel = NO_SELECTION_LEVEL;
				unhandledSelectionLevel = MAX_LEVEL;
				previousConditionSelectionItem = NULL;

				if( ( conditionSelectionItem = conditionList->firstConditionSelectionItem( conditionSentenceNr ) ) != NULL )
					{
					if( isAddingScores )
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

								// Second brance on the same level
								if( selectionLevel == previousSelectionLevel )
									{
									if( !isWaitingForNewLevel )
										{
										if( handledSelectionLevel == MAX_LEVEL )
											{
											handledSelectionLevel = selectionLevel;

											// This is a new brance
											if( previousConditionSelectionItem->isConditionCheckedForSolving )
												conditionSelectionItem->isConditionCheckedForSolving = true;
											else
												{
												// The previous brance was a new brance and this one is unhandled
												isWaitingForNewLevel = true;
												unhandledSelectionLevel = selectionLevel;
												previousConditionSelectionItem->isConditionCheckedForSolving = true;
												}
											}
										else
											{
											isWaitingForNewLevel = true;

											// This brance isn't handled yet
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

					if( previousSelectionLevel == executionLevel )
						{
						// All branches on same level are done and there are brances on a higher level unhandled,
						// so start again with the handled branches (by clearing their checks) until all paths are handled
						if( unhandledSelectionLevel < MAX_LEVEL &&
						handledSelectionLevel < unhandledSelectionLevel )
							conditionList->clearConditionChecksForSolving( unhandledSelectionLevel, conditionSentenceNr );

						if( isAddingScores )
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
								if( scoreList->checkScores( isInverted, solveStrategyParameter, oldSatisfiedScore_, newSatisfiedScore_, oldDissatisfiedScore_, newDissatisfiedScore_, oldNotBlockingScore_, newNotBlockingScore_, oldBlockingScore_, newBlockingScore_ ) == RESULT_OK )
									hasFoundScore = scoreList->hasFoundScore();
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the scores" );
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
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't reach the given execution level ", executionLevel, ". The highest reached level was ", handledSelectionLevel );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the first item of the condition with sentence number ", conditionSentenceNr );
				}
			while( unhandledSelectionLevel < MAX_LEVEL );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The condition list isn't created yet" );

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
				if( ( selectionResult = actionList->findFirstExecutionItem( solveWordItem ) ).result == RESULT_OK )
					currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first action execution selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
				}
			}
		else
			{
			if( ( alternativeList = adminItem_->alternativeList ) != NULL )
				{
				if( ( selectionResult = alternativeList->findFirstExecutionItem( solveWordItem ) ).result == RESULT_OK )
					currentExecutionSelectionItem_ = selectionResult.firstExecutionItem;
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first alternative execution selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
				}
			}

		if( currentExecutionSelectionItem_ != NULL )
			{
			do	{
				if( ( specificationWordItem = currentExecutionSelectionItem_->specificationWordItem() ) != NULL )
					{
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

					if( currentExecutionSelectionItem_->findNextExecutionSelectionItem( solveWordItem ) == RESULT_OK )
						currentExecutionSelectionItem_ = currentExecutionSelectionItem_->nextExecutionItem();
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the next execution selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the current execution selection item is undefined" );
				}
			while( !canWordBeSolved_ );
			}

		currentExecutionSelectionItem_ = originalExecutionSelectionItem;

		return RESULT_OK;
		}

	ResultType canWordBeSolved( WordItem *solveWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "canWordBeSolved";

		if( canWordBeSolved( true, solveWordItem ) == RESULT_OK )
			{
			if( !canWordBeSolved_ &&
			!commonVariables_->hasShownWarning )
				{
				if( canWordBeSolved( false, solveWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a word can be solved by an alternative action" );
				}
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a word can be solved by an action" );

		return RESULT_OK;
		}

	ResultType checkComparison( SelectionItem *conditionSelectionItem )
		{
		bool isNegative;
		bool isNumeralRelation;
		int comparisonResult = 0;
		int firstNumeral = 0;
		int secondNumeral = 0;
		char *firstString = NULL;
		char *secondString = NULL;
		WordItem *generalizationWordItem;
		WordItem *firstSpecificationWordItem;
		WordItem *relationWordItem;
		WordItem *secondSpecificationWordItem;
		WordItem *specificationWordItem;
		WordItem *comparisonAssignmentSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkComparison";

		isConditionSatisfied_ = false;

		if( conditionSelectionItem != NULL )
			{
			if( ( generalizationWordItem = conditionSelectionItem->generalizationWordItem() ) != NULL )
				{
				if( ( specificationWordItem = conditionSelectionItem->specificationWordItem() ) != NULL )
					{
					isNegative = conditionSelectionItem->isNegative();
					relationWordItem = conditionSelectionItem->relationWordItem();

					// First part
					if( conditionSelectionItem->isFirstComparisonPart() )
						{
						if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) == RESULT_OK )
							{
							// Allow the second part of the comparison to be checked
							isConditionSatisfied_ = true;

							firstComparisonAssignmentItem_ = comparisonAssignmentItem_;
							secondComparisonAssignmentItem_ = NULL;
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed get the first comparison assignment" );
						}
					else
						{
						isNumeralRelation = conditionSelectionItem->isNumeralRelation();

						if( !isNumeralRelation &&
						// Second part
						conditionSelectionItem->specificationString() == NULL )
							{
							if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) == RESULT_OK )
								{
								secondComparisonAssignmentItem_ = comparisonAssignmentItem_;

								firstSpecificationWordItem = ( firstComparisonAssignmentItem_ == NULL ? NULL : firstComparisonAssignmentItem_->specificationWordItem() );
								secondSpecificationWordItem = ( secondComparisonAssignmentItem_ == NULL ? NULL : secondComparisonAssignmentItem_->specificationWordItem() );

								firstString = ( firstSpecificationWordItem == NULL ? NULL : firstSpecificationWordItem->anyWordTypeString() );
								secondString = ( secondSpecificationWordItem == NULL ? NULL : secondSpecificationWordItem->anyWordTypeString() );
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed get a comparison assignment" );
							}
						else
							{
							// Numeral or specification string
							if( getComparisonAssignment( isNumeralRelation, specificationWordItem, relationWordItem ) == RESULT_OK )
								{
								if( comparisonAssignmentItem_ != NULL )
									comparisonAssignmentSpecificationWordItem = comparisonAssignmentItem_->specificationWordItem();

								firstString = ( comparisonAssignmentItem_ == NULL ? NULL : ( isNumeralRelation ? ( comparisonAssignmentSpecificationWordItem == NULL ? NULL : comparisonAssignmentSpecificationWordItem->anyWordTypeString() ) : comparisonAssignmentItem_->specificationString() ) );
								secondString = ( isNumeralRelation ? ( relationWordItem == NULL ? NULL : relationWordItem->anyWordTypeString() ) : conditionSelectionItem->specificationString() );
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed get the first comparison assignment" );
							}

						if( firstString == NULL ||
						secondString == NULL )
							comparisonResult = ( firstString == NULL && secondString == NULL ? 0 : ( firstString == NULL ? -1 : 1 ) );
						else
							{
							if( isNumeralString( firstString ) &&
							isNumeralString( secondString ) )
								{
								sscanf( firstString, "%d", &firstNumeral );
								sscanf( secondString, "%d", &secondNumeral );

								comparisonResult = ( firstNumeral == secondNumeral ? 0 : ( firstNumeral < secondNumeral ? -1 : 1 ) );
								}
							else
								comparisonResult = strcmp( firstString, secondString );
							}

						if( generalizationWordItem->isAdjectiveComparisonLess() )
							isConditionSatisfied_ = ( comparisonResult < 0 ? !isNegative : isNegative );
						else
							{
							if( generalizationWordItem->isAdjectiveComparisonEqual() )
								isConditionSatisfied_ = ( comparisonResult == 0 ? !isNegative : isNegative );
							else
								{
								if( generalizationWordItem->isAdjectiveComparisonMore() )
									isConditionSatisfied_ = ( comparisonResult > 0 ? !isNegative : isNegative );
								else
									return adminItem_->startError( functionNameString, moduleNameString_, "Word \"", generalizationWordItem->anyWordTypeString(), "\" isn't comparison word" );
								}
							}
						}
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the given condition selection item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given condition selection item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given condition selection item is undefined" );

		return RESULT_OK;
		}

	ResultType checkForOddOrEven( SelectionItem *conditionSelectionItem )
		{
		bool isNegative;
		unsigned int nAssignments;
		unsigned int relationContextNr;
		WordItem *generalizationWordItem;
		WordItem *specificationWordItem;
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForOddOrEven";

		isConditionSatisfied_ = false;

		if( conditionSelectionItem != NULL )
			{
			if( ( generalizationWordItem = conditionSelectionItem->generalizationWordItem() ) != NULL )
				{
				if( ( specificationWordItem = conditionSelectionItem->specificationWordItem() ) != NULL )
					{
					isNegative = conditionSelectionItem->isNegative();
					relationContextNr = conditionSelectionItem->relationContextNr();

					if( ( relationWordItem = adminItem_->contextWordItemInAllWords( relationContextNr, specificationWordItem, NULL ) ) != NULL )
						{
						if( specificationWordItem->isNounNumber() )
							{
							nAssignments = relationWordItem->nActiveAssignments();

							if( generalizationWordItem->isAdjectiveOdd() )
								isConditionSatisfied_ = ( nAssignments % 2 == 1 ? !isNegative : isNegative );
							else
								{
								if( generalizationWordItem->isAdjectiveEven() )
									isConditionSatisfied_ = ( nAssignments % 2 == 0 ? !isNegative : isNegative );
								else
									return adminItem_->startError( functionNameString, moduleNameString_, "Word \"", generalizationWordItem->anyWordTypeString(), "\" isn't about odd or even" );
								}
							}
						else
							{
							if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, relationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					else
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the relation word" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the given condition selection item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given condition selection item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given condition selection item is undefined" );

		return RESULT_OK;
		}

	ResultType createScore( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createScore";

		if( adminItem_->scoreList == NULL )
			{
			// Create list
			if( ( adminItem_->scoreList = new ScoreList( commonVariables_, adminItem_ ) ) != NULL )
				{
				commonVariables_->adminScoreList = adminItem_->scoreList;
				adminItem_->adminListArray[ADMIN_SCORE_LIST] = adminItem_->scoreList;
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "I failed to create the admin solve score list" );
			}

		if( adminItem_->scoreList->createScoreItem( isChecked, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a score item" );

		return RESULT_OK;
		}

	ResultType findScoringAssignment( bool isBlocking, WordItem *generalizationWordItem )
		{
		SpecificationItem *currentAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findScoringAssignment";

		hasFoundScoringAssignment_ = false;

		if( generalizationWordItem != NULL )
			{
			if( ( currentAssignmentItem = generalizationWordItem->firstNonQuestionActiveAssignmentItem() ) != NULL )
				{
				do	{
					if( !currentAssignmentItem->isNegative() )
						{
						hasFoundScoringAssignment_ = true;

						if( isBlocking )
							{
							if( currentAssignmentItem->isOlderItem() )
								oldBlockingScore_++;
							else
								newBlockingScore_++;
							}
						else
							{
							if( currentAssignmentItem->isOlderItem() )
								oldNotBlockingScore_++;
							else
								newNotBlockingScore_++;
							}
						}
					}
				while( !hasFoundScoringAssignment_ &&
				( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType findScoringAssignment( bool isPossessive, bool isSatisfiedScore, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *currentAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findScoringAssignment";

		hasFoundScoringAssignment_ = false;

		if( generalizationWordItem != NULL )
			{
			if( specificationWordItem != NULL )
				{
				if( ( currentAssignmentItem = generalizationWordItem->firstNonQuestionActiveAssignmentItem() ) != NULL )
					{
					do	{
						if( currentAssignmentItem->isRelatedSpecification( false, isPossessive, relationContextNr, specificationWordItem ) )
							{
							hasFoundScoringAssignment_ = true;

							if( isSatisfiedScore )
								{
								if( currentAssignmentItem->isOlderItem() )
									oldSatisfiedScore_++;
								else
									newSatisfiedScore_++;
								}
							else
								{
								if( currentAssignmentItem->isOlderItem() )
									oldDissatisfiedScore_++;
								else
									newDissatisfiedScore_++;
								}
							}
						}
					while( !hasFoundScoringAssignment_ &&
					( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
					}
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return RESULT_OK;
		}

	ResultType checkConditionByValue( bool isNegative, bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isSatisfiedScore;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkConditionByValue";

		isConditionSatisfied_ = true;

		if( specificationWordItem != NULL )
			{
			if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
				{
				do	{
					foundAssignmentItem = specificationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, isPossessive, currentSpecificationItem->relationContextNr(), currentSpecificationItem->specificationWordItem() );
					isSatisfiedScore = ( isNegative == ( foundAssignmentItem == NULL || foundAssignmentItem->isNegative() ) );

					if( findScoringAssignment( isPossessive, isSatisfiedScore, currentSpecificationItem->relationContextNr(), generalizationWordItem, currentSpecificationItem->specificationWordItem() ) == RESULT_OK )
						{
						if( hasFoundScoringAssignment_ != isSatisfiedScore )
							isConditionSatisfied_ = false;
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a scoring assignment" );
					}
				while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		return RESULT_OK;
		}

	ResultType calculateScorePaths( bool isInverted, bool isAllowingDuplicates, bool isPreparingSort, unsigned short solveStrategyParameter, SelectionItem *referenceSelectionItem )
		{
		bool isAddLocalScores;
		bool isOriginalFoundPossibility;
		bool hasFoundScore = false;
		unsigned int localOldSatisfiedScore;
		unsigned int localNewSatisfiedScore;
		unsigned int localOldDissatisfiedScore;
		unsigned int localNewDissatisfiedScore;
		unsigned int localOldNotBlockingScore;
		unsigned int localNewNotBlockingScore;
		unsigned int localOldBlockingScore;
		unsigned int localNewBlockingScore;
		unsigned int oldSatisfiedScore = oldSatisfiedScore_;
		unsigned int newSatisfiedScore = newSatisfiedScore_;
		unsigned int oldDissatisfiedScore = oldDissatisfiedScore_;
		unsigned int newDissatisfiedScore = newDissatisfiedScore_;
		unsigned int oldNotBlockingScore = oldNotBlockingScore_;
		unsigned int newNotBlockingScore = newNotBlockingScore_;
		unsigned int oldBlockingScore = oldBlockingScore_;
		unsigned int newBlockingScore = newBlockingScore_;
		WordItem *generalizationWordItem;
		WordItem *specificationWordItem;
		ScoreList *scoreList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "calculateScorePaths";

		if( referenceSelectionItem != NULL )
			{
			if( ( generalizationWordItem = referenceSelectionItem->generalizationWordItem() ) != NULL )
				{
				oldSatisfiedScore_ = NO_SCORE;
				newSatisfiedScore_ = NO_SCORE;
				oldDissatisfiedScore_ = NO_SCORE;
				newDissatisfiedScore_ = NO_SCORE;
				oldNotBlockingScore_ = NO_SCORE;
				newNotBlockingScore_ = NO_SCORE;
				oldBlockingScore_ = NO_SCORE;
				newBlockingScore_ = NO_SCORE;

				if( checkCondition( referenceSelectionItem ).result == RESULT_OK )
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
					!generalizationWordItem->isWordCheckedForSolving )
						{
						if( referenceSelectionItem->isAssignedOrClear() )
							{
							if( !referenceSelectionItem->isNegative() )
								{
								// Word has no active assignments
								if( generalizationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
									{
									isOriginalFoundPossibility = hasFoundPossibility_;

									if( findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) == RESULT_OK )
										{
										if( hasFoundPossibility_ )
											isAddLocalScores = false;
										else
											hasFoundPossibility_ = isOriginalFoundPossibility;
										}
									else
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a possibility to solve condition word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						else
							{
							if( ( specificationWordItem = referenceSelectionItem->specificationWordItem() ) != NULL )
								{
								if( !referenceSelectionItem->isNegative() &&
								!specificationWordItem->isWordCheckedForSolving &&
								// Word has no active assignments
								generalizationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
									{
									if( canWordBeSolved( specificationWordItem ) == RESULT_OK )
										{
										if( canWordBeSolved_ )
											{
											isOriginalFoundPossibility = hasFoundPossibility_;

											if( findPossibilityToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) == RESULT_OK )
												{
												if( hasFoundPossibility_ )
													isAddLocalScores = false;
												else
													hasFoundPossibility_ = isOriginalFoundPossibility;
												}
											else
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a possibility to solve condition word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											scoreList = adminItem_->scoreList;

											if( isAllowingDuplicates ||
											scoreList == NULL )
												hasFoundScore = false;
											else
												{
												if( scoreList->findScore( isPreparingSort, referenceSelectionItem ) == RESULT_OK )
													hasFoundScore = scoreList->hasFoundScore();
												else
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a score item" );
												}

											if( !hasFoundScore )
												{
												if( createScore( ( commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL ), NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, referenceSelectionItem ) == RESULT_OK )
													hasFoundPossibility_ = true;
												else
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create an empty solve score" );
												}
											}
										}
									else
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if specification word \"", specificationWordItem->anyWordTypeString(), "\" can be solved" );
									}
								}
							else
								return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the given score item is undefined" );
							}
						}

					if( !commonVariables_->hasShownWarning &&
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
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the condition of the item with sentence number ", referenceSelectionItem->activeSentenceNr(), " and item number ", referenceSelectionItem->itemNr() );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given score item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given reference selection item is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminSolve( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		canWordBeSolved_ = false;
		hasFoundPossibility_ = false;
		hasFoundScoringAssignment_ = false;
		isConditionSatisfied_ = false;

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
		comparisonAssignmentItem_ = NULL;
		firstComparisonAssignmentItem_ = NULL;
		secondComparisonAssignmentItem_ = NULL;

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

	ResultType findPossibilityToSolveWord( bool isAddingScores, bool isAllowingDuplicates, bool isInverted, bool isPreparingSort, unsigned short solveStrategyParameter, WordItem *solveWordItem )
		{
		SelectionResultType selectionResult;
		SelectionItem *originalExecutionSelectionItem;
		SelectionList *actionList;
		SelectionList *alternativeList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossibilityToSolveWord";

		hasFoundPossibility_ = false;

		if( solveWordItem != NULL )
			{
			if( !solveWordItem->isWordCheckedForSolving )
				{
				if( canWordBeSolved( true, solveWordItem ) == RESULT_OK )
					{
					solveWordItem->isWordCheckedForSolving = true;

					if( canWordBeSolved_ &&
					( actionList = adminItem_->actionList ) != NULL )
						{
						originalExecutionSelectionItem = currentExecutionSelectionItem_;

						if( ( selectionResult = actionList->findFirstExecutionItem( solveWordItem ) ).result == RESULT_OK )
							{
							if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != NULL )
								{
								do	{
									if( backTrackConditionScorePaths( isAddingScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_->selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_->activeSentenceNr() ) == RESULT_OK )
										{
										if( currentExecutionSelectionItem_->findNextExecutionSelectionItem( solveWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the next action selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
										}
									else
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to back-fire the condition score paths for the action with sentence number ", currentExecutionSelectionItem_->activeSentenceNr() );
									}
								while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_->nextExecutionItem() ) != NULL );
								}

							currentExecutionSelectionItem_ = originalExecutionSelectionItem;
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first action selection item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
						}

					if( !commonVariables_->hasShownWarning )
						{
						if( canWordBeSolved( false, solveWordItem ) == RESULT_OK )
							{
							if( canWordBeSolved_ &&
							( alternativeList = adminItem_->alternativeList ) != NULL )
								{
								originalExecutionSelectionItem = currentExecutionSelectionItem_;

								if( ( selectionResult = alternativeList->findFirstExecutionItem( solveWordItem ) ).result == RESULT_OK )
									{
									if( ( currentExecutionSelectionItem_ = selectionResult.firstExecutionItem ) != NULL )
										{
										do	{
											if( backTrackConditionScorePaths( isAddingScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem_->selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem_->activeSentenceNr() ) == RESULT_OK )
												{
												if( currentExecutionSelectionItem_->findNextExecutionSelectionItem( solveWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the next alternative item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
												}
											else
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to back-fire the condition score paths for the alternative with sentence number ", currentExecutionSelectionItem_->activeSentenceNr() );
											}
										while( ( currentExecutionSelectionItem_ = currentExecutionSelectionItem_->nextExecutionItem() ) != NULL );
										}

									currentExecutionSelectionItem_ = originalExecutionSelectionItem;
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the first alternative item with solve word \"", solveWordItem->anyWordTypeString(), "\"" );
								}

							solveWordItem->isWordCheckedForSolving = false;
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if the given word \"", solveWordItem->anyWordTypeString(), "\" can be solved by alternative" );
						}
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if the given word \"", solveWordItem->anyWordTypeString(), "\" can be solved by action" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The given solve word \"", solveWordItem->anyWordTypeString(), "\" is already checked" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given solve word is undefined" );

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
		ScoreItem *possibilityItem;
		WordItem *predefinedNounSolveLevelWordItem;
		WordItem *predefinedNounSolveMethodWordItem;
		WordItem *predefinedNounSolveStrategyWordItem;
		ScoreList *scoreList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "solveWord";

		if( solveWordItem != NULL )
			{
			if( ( predefinedNounSolveMethodWordItem = adminItem_->predefinedNounSolveMethodWordItem() ) != NULL )
				{
				if( ( predefinedNounSolveStrategyWordItem = adminItem_->predefinedNounSolveStrategyWordItem() ) != NULL )
					{
					if( currentSolveProgress_ == 0 )
						{
						if( ( predefinedNounSolveLevelWordItem = adminItem_->predefinedNounSolveLevelWordItem() ) == NULL )
							solveLevel_ = NO_SOLVE_LEVEL;
						else
							{
							if( ( specificationResult = predefinedNounSolveLevelWordItem->getAssignmentOrderNr() ).result == RESULT_OK )
								solveLevel_ = specificationResult.assignmentOrderNr;
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve level" );
							}
						}

					// Word has no active assignments
					if( solveWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
						{
						if( commonVariables_->currentAssignmentLevel <= solveLevel_ )
							{
							if( adminItem_->assignSpecification( predefinedNounSolveMethodWordItem, adminItem_->predefinedAdjectiveBusyWordItem() ) == RESULT_OK )
								{
								if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL &&
								( scoreList = adminItem_->scoreList ) != NULL )
									{
									// Make sure no scores are left at the start
									if( scoreList->deleteScores() != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the admin score list" );
									}

								isInverted = ( predefinedNounSolveMethodWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, adminItem_->predefinedAdjectiveInvertedWordItem() ) != NULL );

								if( ( specificationResult = predefinedNounSolveStrategyWordItem->getAssignmentWordParameter() ).result == RESULT_OK )
									{
									if( findPossibilityToSolveWord( true, ( commonVariables_->currentAssignmentLevel == solveLevel_ ), isInverted, ( commonVariables_->currentAssignmentLevel + 1 < solveLevel_ ), specificationResult.assignmentParameter, solveWordItem ) == RESULT_OK )
										{
										if( hasFoundPossibility_ )
											{
											if( commonVariables_->currentAssignmentLevel < solveLevel_ )
												{
												if( ( scoreList = adminItem_->scoreList ) != NULL )
													{
													nPossibilities = scoreList->nPossibilities();
													solveProgressStep = ( ( endSolveProgress - currentSolveProgress_ ) / nPossibilities );

													if( solveLevel_ > 1 )
														commonVariables_->presentation->startProgress( currentSolveProgress_, MAX_PROGRESS, INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_START, solveLevel_, INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_END );

													if( ( possibilityItem = scoreList->firstPossibility() ) != NULL )
														{
														do	{
															// Copy solve action of NO_ASSIGNMENT_LEVEL to higher levels
															if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
																actionSelectionItem = possibilityItem->scoreReference();

															if( createNewAssignmentLevelInAllWords() == RESULT_OK )
																{
																commonVariables_->currentAssignmentLevel++;

																if( adminItem_->assignSelectionSpecification( possibilityItem->scoreReference() ) == RESULT_OK )
																	{
																	tempEndSolveProgress = currentSolveProgress_ + solveProgressStep;

																	if( adminItem_->executeSelection( ( currentSolveProgress_ + solveProgressStep / 2L ), actionSelectionItem ) == RESULT_OK )
																		{
																		// Word has active assignments
																		if( solveWordItem->firstNonQuestionActiveAssignmentItem() != NULL )
																			{
																			isInverted = ( predefinedNounSolveMethodWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, adminItem_->predefinedAdjectiveInvertedWordItem() ) != NULL );

																			if( !isInverted &&
																			commonVariables_->currentAssignmentLevel < solveLevel_ )
																				{
																				if( scoreList->deleteScores() == RESULT_OK )
																					// Don't solve any deeper if there is a winning score
																					solveLevel_ = commonVariables_->currentAssignmentLevel;
																				else
																					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the scores with an assignment level higher than ", commonVariables_->currentAssignmentLevel );
																				}

																			// Create winning or losing score
																			if( createScore( false, NO_SCORE, ( isInverted ? NO_SCORE : WINNING_SCORE ), NO_SCORE, ( isInverted ? WINNING_SCORE : NO_SCORE ), NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, actionSelectionItem ) == RESULT_OK )
																				{
																				currentSolveProgress_ = tempEndSolveProgress;

																				if( solveLevel_ > 1 )
																					commonVariables_->presentation->showProgress( currentSolveProgress_ );
																				}
																			else
																				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a winning or losing score of solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );
																			}
																		else
																			{
																			if( solveWord( tempEndSolveProgress, solveWordItem, actionSelectionItem ) == RESULT_OK )
																				{
																				if( commonVariables_->currentAssignmentLevel == 1 )
																					scoreList->changeAction( actionSelectionItem );
																				}
																			else
																				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );
																			}

																		if( deleteAssignmentLevelInAllWords() == RESULT_OK )
																			{
																			commonVariables_->currentAssignmentLevel--;
																			possibilityItem = possibilityItem->nextPossibilityItem();

																			if( ++possibilityNumber <= nPossibilities )
																				{
																				if( possibilityItem != NULL &&
																				possibilityNumber == nPossibilities )
																					return adminItem_->startError( functionNameString, moduleNameString_, "I have found more possibility items than number of possibilities" );
																				}
																			else
																				{
																				if( possibilityItem == NULL )
																					return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the next possibility item before the number of possibilities is reached" );
																				}
																			}
																		else
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the assignments of level ", commonVariables_->currentAssignmentLevel );
																		}
																	else
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute a selection during the solving of word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );
																	}
																else
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a selection specifcation at assignment level: ", commonVariables_->currentAssignmentLevel );
																}
															else
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a new assignment level: ", commonVariables_->currentAssignmentLevel );
															}
														while( possibilityItem != NULL );

														if( nPossibilities > 1 ||
														// Higher level has possibilities
														commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL )
															{
															if( scoreList->deleteScores() != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the scores with assignment level ", commonVariables_->currentAssignmentLevel );
															}
														}
													else
														return adminItem_->startError( functionNameString, moduleNameString_, "I failed to get the first possibility item at assignment level ", commonVariables_->currentAssignmentLevel );
													}
												else
													return adminItem_->startError( functionNameString, moduleNameString_, "The solve scores list isn't created yet at assignment level ", commonVariables_->currentAssignmentLevel );
												}
											else
												{
												currentSolveProgress_ = endSolveProgress;

												if( solveLevel_ > 1 )
													commonVariables_->presentation->showProgress( currentSolveProgress_ );
												}

											if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
												{
												if( ( scoreList = adminItem_->scoreList ) != NULL )
													{
													if( ( specificationResult = predefinedNounSolveStrategyWordItem->getAssignmentWordParameter() ).result == RESULT_OK )
														{
														if( ( selectionResult = scoreList->getBestAction( adminItem_->isCurrentlyTesting(), specificationResult.assignmentParameter ) ).result == RESULT_OK )
															{
															if( ( actionSelectionItem = selectionResult.bestActionItem ) != NULL )
																{
																if( adminItem_->assignSelectionSpecification( actionSelectionItem ) == RESULT_OK )
																	{
																	if( adminItem_->assignSpecification( predefinedNounSolveMethodWordItem, adminItem_->predefinedAdjectiveDoneWordItem() ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the done flag to the solve method at assignment level ", commonVariables_->currentAssignmentLevel );
																	}
																else
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a selection specification at assignment level ", commonVariables_->currentAssignmentLevel );
																}
															else
																return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the best action selection item" );
															}
														else
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the best action of solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );
														}
													else
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve strategy at assignment level ", commonVariables_->currentAssignmentLevel );
													}
												else
													return adminItem_->startError( functionNameString, moduleNameString_, "The solve scores list isn't created yet" );
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
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a possibility to solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve strategy at assignment level ", commonVariables_->currentAssignmentLevel );
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the busy flag to the solve method at assignment level ", commonVariables_->currentAssignmentLevel );
							}
						else
							return adminItem_->startError( functionNameString, moduleNameString_, "The given assignment level of ", commonVariables_->currentAssignmentLevel, " is higher than the given solve level ", solveLevel_ );
						}
					else
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_START, solveWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_END ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The predefined solve strategy noun word item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "The predefined solve-method noun word item is undefined" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given solve word is undefined" );

		return RESULT_OK;
		}

	SelectionResultType checkCondition( SelectionItem *conditionSelectionItem )
		{
		SelectionResultType selectionResult;
		bool isPossessive;
		bool isNegative;
		WordItem *generalizationWordItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkCondition";

		isConditionSatisfied_ = false;

		if( conditionSelectionItem != NULL )
			{
			if( ( generalizationWordItem = conditionSelectionItem->generalizationWordItem() ) != NULL )
				{
				if( ( specificationWordItem = conditionSelectionItem->specificationWordItem() ) != NULL )
					{
					isPossessive = conditionSelectionItem->isPossessive();
					isNegative = conditionSelectionItem->isNegative();

					if( conditionSelectionItem->isAssignedOrClear() )
						{
						if( specificationWordItem->isAdjectiveEmpty() )
							{
							// Adjective "clear"
							if( findScoringAssignment( !isNegative, generalizationWordItem ) == RESULT_OK )
								isConditionSatisfied_ = ( hasFoundScoringAssignment_ ? isNegative : !isNegative );
							else
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a scoring assignment" );
							}
						else
							{
							// Adjective "assigned"
							if( findScoringAssignment( isNegative, generalizationWordItem ) == RESULT_OK )
								isConditionSatisfied_ = ( hasFoundScoringAssignment_ ? !isNegative : isNegative );
							else
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a scoring assignment" );
							}
						}
					else
						{
						if( generalizationWordItem->isAdjectiveComparison() )
							{
							if( checkComparison( conditionSelectionItem ) != RESULT_OK )
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to check a comparison" );
							}
						else
							{
							if( generalizationWordItem->isAdjectiveOddOrEven() )
								{
								if( checkForOddOrEven( conditionSelectionItem ) != RESULT_OK )
									adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for odd or even" );
								}
							else
								{
								if( conditionSelectionItem->isValueSpecification() )
									{
									if( checkConditionByValue( isNegative, isPossessive, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the condition of a specification by value" );
									}
								else
									{
									if( findScoringAssignment( isPossessive, !isNegative, conditionSelectionItem->relationContextNr(), generalizationWordItem, specificationWordItem ) == RESULT_OK )
										{
										if( hasFoundScoringAssignment_ != isNegative )
											isConditionSatisfied_ = true;
										}
									else
										adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a scoring assignment" );
									}
								}
							}
						}
					}
				else
					adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the given condition selection item is undefined" );
				}
			else
				adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given condition selection item is undefined" );
			}
		else
			adminItem_->startError( functionNameString, moduleNameString_, "The given condition selection item is undefined" );

		selectionResult.isConditionSatisfied = isConditionSatisfied_;
		selectionResult.result = commonVariables_->result;
		return selectionResult;
		}
	};

/*************************************************************************
 *	"Give thanks to the God of gods.
 *	His faithful love endures forever." (Psalm 136:2)
 *************************************************************************/
