/*
 *	Class:			ScoreList
 *	Parent class:	List
 *	Purpose:		To temporarily store score items
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

// Score List header

#include "ScoreList.h"

	// Private deconstructor functions

	void ScoreList::deleteScoreList( ScoreItem *searchItem )
		{
		ScoreItem *deleteItem;

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextScoreItem();
			delete deleteItem;
			}
		}

	// Private functions

	ResultType ScoreList::markAction( SelectionItem *markSelectionReference )
		{
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "MarkAction";

		if( markSelectionReference != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->referenceSelectionItem == markSelectionReference )
					searchItem->isMarked = true;		// Set mark

				searchItem = searchItem->nextScoreItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "The given score item is undefined" );

		return RESULT_OK;
		}

	ResultType ScoreList::disableAction( bool isIncludingMarkedActions, SelectionItem *disableItem )
		{
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "DisableAction";

		if( disableItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( ( isIncludingMarkedActions &&
				searchItem->isMarked ) ||
				searchItem->referenceSelectionItem == disableItem )
					{
					searchItem->isMarked = false;	// Clear mark
					searchItem->isChecked = false;	// Clear check
					}

				searchItem = searchItem->nextScoreItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "The given score item is undefined" );

		return RESULT_OK;
		}

	ResultType ScoreList::getBestScore( bool cummulative, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, unsigned int bestOldSatisfiedScore, unsigned int bestNewSatisfiedScore, unsigned int bestOldDissatisfiedScore, unsigned int bestNewDissatisfiedScore, unsigned int bestOldNotBlockingScore, unsigned int bestNewNotBlockingScore, unsigned int bestOldBlockingScore, unsigned int bestNewBlockingScore )
		{
		bool equalSatisfiedScore = ( oldSatisfiedScore == bestOldSatisfiedScore &&
										newSatisfiedScore == bestNewSatisfiedScore );
		bool equalDissatisfiedScore = ( oldDissatisfiedScore == bestOldDissatisfiedScore &&
										newDissatisfiedScore == bestNewDissatisfiedScore );
		bool equalNotBlockingScore = ( oldNotBlockingScore == bestOldNotBlockingScore &&
										newNotBlockingScore == bestNewNotBlockingScore );
		bool equalBlockingScore = ( oldBlockingScore == bestOldBlockingScore &&
										newBlockingScore == bestNewBlockingScore );

		double satisfiedScore = (double)oldSatisfiedScore + (double)newSatisfiedScore;
		double dissatisfiedScore = (double)oldDissatisfiedScore + (double)newDissatisfiedScore;
		double notBlockingScore = (double)oldNotBlockingScore + (double)newNotBlockingScore;
		double blockingScore = (double)oldBlockingScore + (double)newBlockingScore;

		double bestSatisfiedScore = (double)bestOldSatisfiedScore + (double)bestNewSatisfiedScore;
		double bestDissatisfiedScore = (double)bestOldDissatisfiedScore + (double)bestNewDissatisfiedScore;
		double bestNotBlockingScore = (double)bestOldNotBlockingScore + (double)bestNewNotBlockingScore;
		double bestBlockingScore = (double)bestOldBlockingScore + (double)bestNewBlockingScore;

		bool higherSatisfiedScore = ( satisfiedScore > bestSatisfiedScore );
		bool superiorSatisfiedScore = ( higherSatisfiedScore &&
										satisfiedScore > bestDissatisfiedScore );
		bool lowerDissatisfiedScore = ( dissatisfiedScore < bestDissatisfiedScore );
		bool higherDissatisfiedScore = ( dissatisfiedScore > bestDissatisfiedScore );
		bool lowerNotBlockingScore = ( notBlockingScore < bestNotBlockingScore );
		bool lowerBlockingScore = ( blockingScore < bestBlockingScore );
		char functionNameString[FUNCTION_NAME_LENGTH] = "getBestScore";

		hasBetterScore_ = false;
		hasEqualScore_ = ( equalSatisfiedScore &&
						equalDissatisfiedScore &&
						equalNotBlockingScore &&
						equalBlockingScore );

		if( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER ||
		solveStrategyParameter == WORD_PARAMETER_ADJECTIVE_DEFENSIVE ||
		solveStrategyParameter == WORD_PARAMETER_ADJECTIVE_EXCLUSIVE )
			{
			hasBetterScore_ =	// Get best satisfying strategy,
							( ( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER &&
								( higherSatisfiedScore ||
								( equalSatisfiedScore &&
								lowerDissatisfiedScore ) ) ) ||

							( solveStrategyParameter == WORD_PARAMETER_ADJECTIVE_DEFENSIVE &&
								( lowerDissatisfiedScore ||
								( equalDissatisfiedScore &&
								higherSatisfiedScore ) ) ) ||

							( solveStrategyParameter == WORD_PARAMETER_ADJECTIVE_EXCLUSIVE &&
								// Has no dissatisfied score and superior satisfied score
								( ( superiorSatisfiedScore &&
								( cummulative ||
								dissatisfiedScore == NO_SCORE ) ) ||

								// Has no old satisfied score and has new satisfied score and higher dissatisfied score
								( higherDissatisfiedScore &&
								( cummulative ||
								( oldSatisfiedScore == NO_SCORE &&
								newSatisfiedScore > NO_SCORE ) ) ) ) ) ||

							// else if equal satisfying strategy,
							( equalSatisfiedScore &&
							equalDissatisfiedScore &&

							// Get lowest blocking score, else if equal blocking score, Get lowest not blocking score
							( lowerBlockingScore ||
							( equalBlockingScore &&
							lowerNotBlockingScore ) ) ) );
			}
		else
			return startError( functionNameString, NULL, NULL, "The given solve strategy parameter isn't implemented" );

		return RESULT_OK;
		}

	ScoreItem *ScoreList::firstActiveScoreItem()
		{
		return (ScoreItem *)firstActiveItem();
		}

	ScoreItem *ScoreList::nextScoreListItem()
		{
		return (ScoreItem *)nextListItem();
		}


	// Constructor / deconstructor

	ScoreList::ScoreList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		hasBetterScore_ = false;
		hasEqualScore_ = false;
		hasFoundScore_ = false;

		initializeListVariables( ADMIN_SCORE_LIST_SYMBOL, "ScoreList", commonVariables, myWordItem );
		}

	ScoreList::~ScoreList()
		{
		deleteScoreList( firstActiveScoreItem() );
		deleteScoreList( (ScoreItem *)firstReplacedItem() );
		deleteScoreList( (ScoreItem *)firstDeletedItem() );
		}


	// Protected virtual functions

	bool ScoreList::isTemporaryList()
		{
		return true;
		}


	// Protected functions

	bool ScoreList::hasFoundScore()
		{
		return hasFoundScore_;
		}

	unsigned int ScoreList::nPossibilities()
		{
		unsigned int nItems = 0;
		ScoreItem *searchItem = firstActiveScoreItem();

		while( searchItem != NULL &&
		searchItem->assignmentLevel() >= commonVariables()->currentAssignmentLevel )
			{
			if( searchItem->assignmentLevel() == commonVariables()->currentAssignmentLevel )
				nItems++;

			searchItem = searchItem->nextScoreItem();
			}

		return nItems;
		}

	ResultType ScoreList::changeAction( SelectionItem *actionSelectionItem )
		{
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeAction";

		if( actionSelectionItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isChecked )		// All new created scores with assignment level higher than zero
					{
					searchItem->isChecked = false;
					searchItem->referenceSelectionItem = actionSelectionItem;
					}

				searchItem = searchItem->nextScoreItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "The given action selection item is undefined" );

		return RESULT_OK;
		}

	ResultType ScoreList::checkSelectionItemForUsage( SelectionItem *unusedSelectionItem )
		{
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSelectionItemForUsage";

		if( unusedSelectionItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->referenceSelectionItem == unusedSelectionItem )
					return startError( functionNameString, NULL, NULL, "The reference selection item is still in use" );

				searchItem = searchItem->nextScoreItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "The given unused justification item is undefined" );

		return RESULT_OK;
		}

	ResultType ScoreList::findScore( bool prepareSort, SelectionItem *findScoreItem )
		{
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findScore";

		hasFoundScore_ = false;

		if( findScoreItem != NULL )
			{
			while( searchItem != NULL &&
			searchItem->assignmentLevel() >= commonVariables()->currentAssignmentLevel )
				{
				if( searchItem->assignmentLevel() == commonVariables()->currentAssignmentLevel )
					{
					if( ( searchItem->isChecked ||
					commonVariables()->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL ) &&
					searchItem->referenceSelectionItem == findScoreItem )
						{
						hasFoundScore_ = true;
						searchItem->isMarked = prepareSort;
						}
					}

				searchItem = searchItem->nextScoreItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "The given score item is undefined" );

		return RESULT_OK;
		}

	ResultType ScoreList::deleteScores()
		{
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteScores";

		while( searchItem != NULL &&
		searchItem->assignmentLevel() >= commonVariables()->currentAssignmentLevel )
			{
			if( searchItem->assignmentLevel() == commonVariables()->currentAssignmentLevel )
				{
				if( deleteItem( false, searchItem ) == RESULT_OK )
					searchItem = nextScoreListItem();
				else
					return addError( functionNameString, NULL, NULL, "I failed to delete an active item" );
				}
			else
				searchItem = searchItem->nextScoreItem();
			}

		return RESULT_OK;
		}

	ResultType ScoreList::checkScores( bool isInverted, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore )
		{
		unsigned int checkOldSatisfiedScore = ( isInverted ? oldDissatisfiedScore : oldSatisfiedScore );
		unsigned int checkNewSatisfiedScore = ( isInverted ? newDissatisfiedScore : newSatisfiedScore );
		unsigned int checkOldDissatisfiedScore = ( isInverted ? oldSatisfiedScore : oldDissatisfiedScore );
		unsigned int checkNewDissatisfiedScore = ( isInverted ? newSatisfiedScore : newDissatisfiedScore );
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkScores";

		hasFoundScore_ = false;

		if( checkOldSatisfiedScore > NO_SCORE ||
		checkNewSatisfiedScore > NO_SCORE ||
		checkOldDissatisfiedScore > NO_SCORE ||
		checkNewDissatisfiedScore > NO_SCORE ||

		oldNotBlockingScore > NO_SCORE ||
		newNotBlockingScore > NO_SCORE ||
		oldBlockingScore > NO_SCORE ||
		newBlockingScore > NO_SCORE )
			{
			while( searchItem != NULL &&
			searchItem->assignmentLevel() >= commonVariables()->currentAssignmentLevel )
				{
				if( searchItem->assignmentLevel() == commonVariables()->currentAssignmentLevel )
					{
					if( !searchItem->hasOldSatisfiedScore() &&
					!searchItem->hasNewSatisfiedScore() &&
					!searchItem->hasOldDissatisfiedScore() &&
					!searchItem->hasNewDissatisfiedScore() &&
					!searchItem->hasOldNotBlockingScore() &&
					!searchItem->hasNewNotBlockingScore() &&
					!searchItem->hasOldBlockingScore() &&
					!searchItem->hasNewBlockingScore() )	// All new created (=empty) scores
						{
						hasFoundScore_ = true;

						searchItem->oldSatisfiedScore = checkOldSatisfiedScore;
						searchItem->newSatisfiedScore = checkNewSatisfiedScore;
						searchItem->oldDissatisfiedScore = checkOldDissatisfiedScore;
						searchItem->newDissatisfiedScore = checkNewDissatisfiedScore;
						searchItem->oldNotBlockingScore = oldNotBlockingScore;
						searchItem->newNotBlockingScore = newNotBlockingScore;
						searchItem->oldBlockingScore = oldBlockingScore;
						searchItem->newBlockingScore = newBlockingScore;
						}
					else
						{
						if( searchItem->isMarked )
							{
							if( getBestScore( false, solveStrategyParameter, checkOldSatisfiedScore, checkNewSatisfiedScore, checkOldDissatisfiedScore, checkNewDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, searchItem->oldSatisfiedScore, searchItem->newSatisfiedScore, searchItem->oldDissatisfiedScore, searchItem->newDissatisfiedScore, searchItem->oldNotBlockingScore, searchItem->newNotBlockingScore, searchItem->oldBlockingScore, searchItem->newBlockingScore ) == RESULT_OK )
								{
								hasFoundScore_ = true;

								if( hasBetterScore_ )
									{
									searchItem->oldSatisfiedScore = checkOldSatisfiedScore;
									searchItem->newSatisfiedScore = checkNewSatisfiedScore;
									searchItem->oldDissatisfiedScore = checkOldDissatisfiedScore;
									searchItem->newDissatisfiedScore = checkNewDissatisfiedScore;
									searchItem->oldNotBlockingScore = oldNotBlockingScore;
									searchItem->newNotBlockingScore = newNotBlockingScore;
									searchItem->oldBlockingScore = oldBlockingScore;
									searchItem->newBlockingScore = newBlockingScore;
									}
								}
							else
								return addError( functionNameString, NULL, NULL, "I failed to get the best score" );
							}
						}
					}

				searchItem = searchItem->nextScoreItem();
				}
			}
		else
			return startError( functionNameString, NULL, NULL, "None of the given scores has a value parameter" );

		return RESULT_OK;
		}

	ResultType ScoreList::createScoreItem( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createScoreItem";

		if( commonVariables()->currentItemNr < MAX_ITEM_NR )
			{
			if( addItemToList( QUERY_ACTIVE_CHAR, new ScoreItem( isChecked, commonVariables()->currentAssignmentLevel, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
				return addError( functionNameString, NULL, NULL, "I failed to add an active score item" );
			}
		else
			return startError( functionNameString, NULL, NULL, "The current item number is undefined" );

		return RESULT_OK;
		}

	SelectionResultType ScoreList::getBestAction( unsigned short solveStrategyParameter )
		{
		SelectionResultType selectionResult;
		bool cummulate = false;
		unsigned int nRandomEntries = 0;
		unsigned int nLocalLosingScores;
		unsigned int nLocalWinningScores;
		unsigned int nBestWinningScores = 0;
		unsigned int nBestLosingScores = MAX_NUMBER;
		unsigned int localOldSatisfiedScore;
		unsigned int localNewSatisfiedScore;
		unsigned int localOldDissatisfiedScore;
		unsigned int localNewDissatisfiedScore;
		unsigned int localOldNotBlockingScore;
		unsigned int localNewNotBlockingScore;
		unsigned int localOldBlockingScore;
		unsigned int localNewBlockingScore;
		unsigned int bestOldSatisfiedScore = NO_SCORE;
		unsigned int bestNewSatisfiedScore = NO_SCORE;
		unsigned int bestOldDissatisfiedScore = ( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER ? MAX_SCORE : NO_SCORE );
		unsigned int bestNewDissatisfiedScore = bestOldDissatisfiedScore;
		unsigned int bestOldNotBlockingScore = MAX_SCORE;
		unsigned int bestNewNotBlockingScore = MAX_SCORE;
		unsigned int bestOldBlockingScore = MAX_SCORE;
		unsigned int bestNewBlockingScore = MAX_SCORE;
		ScoreItem *localSearchItem;
		ScoreItem *searchItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "getBestAction";

		hasEqualScore_ = false;
		hasBetterScore_ = false;

		while( searchItem != NULL )
			{
			searchItem->isMarked = false;	// Clear all marks
			searchItem->isChecked = true;	// Set all checks
			searchItem = searchItem->nextScoreItem();
			}

		searchItem = firstActiveScoreItem();

		while( commonVariables()->result == RESULT_OK &&
		searchItem != NULL )
			{
			if( searchItem->isChecked )
				{
				nLocalLosingScores = 0;
				nLocalWinningScores = 0;
				localOldSatisfiedScore = NO_SCORE;
				localNewSatisfiedScore = NO_SCORE;
				localOldDissatisfiedScore = ( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER ? MAX_SCORE : NO_SCORE );
				localNewDissatisfiedScore = localOldDissatisfiedScore;
				localOldNotBlockingScore = MAX_SCORE;
				localNewNotBlockingScore = MAX_SCORE;
				localOldBlockingScore = MAX_SCORE;
				localNewBlockingScore = MAX_SCORE;
				localSearchItem = firstActiveScoreItem();

				while( commonVariables()->result == RESULT_OK &&
				localSearchItem != NULL )
					{
					if( localSearchItem->referenceSelectionItem == searchItem->referenceSelectionItem )
						{
						localSearchItem->isChecked = false;		// Is handled

						if( localSearchItem->newSatisfiedScore == WINNING_SCORE )
							nLocalWinningScores++;
						else
							{
							if( localSearchItem->newDissatisfiedScore == WINNING_SCORE )
								nLocalLosingScores++;
							else
								{
								if( getBestScore( false, solveStrategyParameter, localSearchItem->oldSatisfiedScore, localSearchItem->newSatisfiedScore, localSearchItem->oldDissatisfiedScore, localSearchItem->newDissatisfiedScore, localSearchItem->oldNotBlockingScore, localSearchItem->newNotBlockingScore, localSearchItem->oldBlockingScore, localSearchItem->newBlockingScore, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore ) == RESULT_OK )
									{
									if( hasBetterScore_ )
										{
										localOldSatisfiedScore = localSearchItem->oldSatisfiedScore;
										localNewSatisfiedScore = localSearchItem->newSatisfiedScore;
										localOldDissatisfiedScore = localSearchItem->oldDissatisfiedScore;
										localNewDissatisfiedScore = localSearchItem->newDissatisfiedScore;
										localOldNotBlockingScore = localSearchItem->oldNotBlockingScore;
										localNewNotBlockingScore = localSearchItem->newNotBlockingScore;
										localOldBlockingScore = localSearchItem->oldBlockingScore;
										localNewBlockingScore = localSearchItem->newBlockingScore;
										}
									}
								else
									addError( functionNameString, NULL, NULL, "I failed to get the best local score" );
								}
							}
						}

					localSearchItem = localSearchItem->nextScoreItem();
					}

				hasEqualScore_ = ( nLocalWinningScores == nBestWinningScores &&
				nLocalLosingScores == nBestLosingScores );

				if( hasEqualScore_ )
					{
					if( getBestScore( false, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) != RESULT_OK )
						addError( functionNameString, NULL, NULL, "I failed to get the best score" );
					}
				else
					// Get highest number of winning scores, else if equal: Get lowest number of losing scores
					hasBetterScore_ = ( nLocalWinningScores > nBestWinningScores ||

										( nLocalWinningScores == nBestWinningScores &&
										nLocalLosingScores < nBestLosingScores ) );

				if( commonVariables()->result == RESULT_OK )
					{
					if( hasBetterScore_ )
						{
						cummulate = false;
						nBestWinningScores = nLocalWinningScores;
						nBestLosingScores = nLocalLosingScores;

						bestOldSatisfiedScore = localOldSatisfiedScore;
						bestNewSatisfiedScore = localNewSatisfiedScore;
						bestOldDissatisfiedScore = localOldDissatisfiedScore;
						bestNewDissatisfiedScore = localNewDissatisfiedScore;
						bestOldNotBlockingScore = localOldNotBlockingScore;
						bestNewNotBlockingScore = localNewNotBlockingScore;
						bestOldBlockingScore = localOldBlockingScore;
						bestNewBlockingScore = localNewBlockingScore;

						if( searchItem->referenceSelectionItem != selectionResult.bestActionItem )
							{
							if( selectionResult.bestActionItem != NULL )
								{
								if( disableAction( true, selectionResult.bestActionItem ) != RESULT_OK )	// Previous best action
									addError( functionNameString, NULL, NULL, "I failed to disable the best action" );
								}

							if( commonVariables()->result == RESULT_OK )
								{
								if( markAction( searchItem->referenceSelectionItem ) != RESULT_OK )	// Current action
									addError( functionNameString, NULL, NULL, "I failed to mark an action" );
								}
							}

						selectionResult.bestActionItem = searchItem->referenceSelectionItem;
						}
					else
						{
						if( searchItem->referenceSelectionItem != selectionResult.bestActionItem )
							{
							if( hasEqualScore_ )
								{
								if( markAction( searchItem->referenceSelectionItem ) == RESULT_OK )	// Current action
									cummulate = true;	// Found the same best score with different action
								else
									addError( functionNameString, NULL, NULL, "I failed to mark an action" );
								}
							else
								{
								if( disableAction( false, searchItem->referenceSelectionItem ) != RESULT_OK )		// Previous best action
									addError( functionNameString, NULL, NULL, "I failed to disable an action" );
								}
							}
						}
					}
				}

			searchItem = searchItem->nextScoreItem();
			}

		if( commonVariables()->result == RESULT_OK &&
		cummulate )		// Found the same best score with different action
			{
			bestOldSatisfiedScore = NO_SCORE;
			bestNewSatisfiedScore = NO_SCORE;
			bestOldDissatisfiedScore = ( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER ? MAX_SCORE : NO_SCORE );
			bestNewDissatisfiedScore = bestOldDissatisfiedScore;
			bestOldNotBlockingScore = MAX_SCORE;
			bestNewNotBlockingScore = MAX_SCORE;
			bestOldBlockingScore = MAX_SCORE;
			bestNewBlockingScore = MAX_SCORE;

			selectionResult.bestActionItem = NULL;

			searchItem = firstActiveScoreItem();

			while( searchItem != NULL )
				{
				searchItem->isChecked = searchItem->isMarked;	// Copy the checks
				searchItem->isMarked = false;					// Clear all marks
				searchItem = searchItem->nextScoreItem();
				}

			searchItem = firstActiveScoreItem();

			while( commonVariables()->result == RESULT_OK &&
			searchItem != NULL )	// Search only for new scores of best actions
				{
				if( searchItem->isChecked )
					{
					localOldSatisfiedScore = NO_SCORE;
					localNewSatisfiedScore = NO_SCORE;
					localOldDissatisfiedScore = NO_SCORE;
					localNewDissatisfiedScore = NO_SCORE;
					localOldNotBlockingScore = NO_SCORE;
					localNewNotBlockingScore = NO_SCORE;
					localOldBlockingScore = NO_SCORE;
					localNewBlockingScore = NO_SCORE;

					localSearchItem = firstActiveScoreItem();

					while( localSearchItem != NULL )
						{
						if( localSearchItem->referenceSelectionItem == searchItem->referenceSelectionItem )
							{
							localSearchItem->isChecked = false;		// Clear handled check

							// Don't add winning or losing scores
							if( localSearchItem->newSatisfiedScore != WINNING_SCORE &&
							localSearchItem->newDissatisfiedScore != WINNING_SCORE &&

							// Only add if solve strategy isn't EXCLUSIVE-OFFENSIVE or it has no old satisfied score and it has new satisfied score or it has no dissatisfied score
							( solveStrategyParameter != WORD_PARAMETER_ADJECTIVE_EXCLUSIVE ||

							( !localSearchItem->hasOldSatisfiedScore() &&
							localSearchItem->hasNewSatisfiedScore() ) ||

							( !localSearchItem->hasOldDissatisfiedScore() &&
							!localSearchItem->hasNewDissatisfiedScore() ) ) )
								{
								if( MAX_SCORE - localOldSatisfiedScore > localSearchItem->oldSatisfiedScore )
									{
									if( MAX_SCORE - localNewSatisfiedScore > localSearchItem->newSatisfiedScore )
										{
										if( MAX_SCORE - localOldDissatisfiedScore > localSearchItem->oldDissatisfiedScore )
											{
											if( MAX_SCORE - localNewDissatisfiedScore > localSearchItem->newDissatisfiedScore )
												{
												if( MAX_SCORE - localOldNotBlockingScore > localSearchItem->oldNotBlockingScore )
													{
													if( MAX_SCORE - localNewNotBlockingScore > localSearchItem->newNotBlockingScore )
														{
														if( MAX_SCORE - localOldBlockingScore > localSearchItem->oldBlockingScore )
															{
															if( MAX_SCORE - localNewBlockingScore > localSearchItem->newBlockingScore )
																{
																localOldSatisfiedScore += localSearchItem->oldSatisfiedScore;
																localNewSatisfiedScore += localSearchItem->newSatisfiedScore;
																localOldDissatisfiedScore += localSearchItem->oldDissatisfiedScore;
																localNewDissatisfiedScore += localSearchItem->newDissatisfiedScore;
																localOldNotBlockingScore += localSearchItem->oldNotBlockingScore;
																localNewNotBlockingScore += localSearchItem->newNotBlockingScore;
																localOldBlockingScore += localSearchItem->oldBlockingScore;
																localNewBlockingScore += localSearchItem->newBlockingScore;
																}
															else
																startSystemError( functionNameString, NULL, NULL, "Overflow of the new blocking cummulate score" );
															}
														else
															startSystemError( functionNameString, NULL, NULL, "Overflow of the old blocking cummulate score" );
														}
													else
														startSystemError( functionNameString, NULL, NULL, "Overflow of the new not-blocking cummulate score" );
													}
												else
													startSystemError( functionNameString, NULL, NULL, "Overflow of the old not-blocking cummulate score" );
												}
											else
												startSystemError( functionNameString, NULL, NULL, "Overflow of the new dissatisfied cummulate score" );
											}
										else
											startSystemError( functionNameString, NULL, NULL, "Overflow of the old dissatisfied cummulate score" );
										}
									else
										startSystemError( functionNameString, NULL, NULL, "Overflow of the new satisfied cummulate score" );
									}
								else
									startSystemError( functionNameString, NULL, NULL, "Overflow of the old satisfied cummulate score" );
								}
							}

						localSearchItem = localSearchItem->nextScoreItem();
						}

					if( getBestScore( true, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) == RESULT_OK )
						{
						if( hasBetterScore_ )
							{
							nRandomEntries = 1;

							bestOldSatisfiedScore = localOldSatisfiedScore;
							bestNewSatisfiedScore = localNewSatisfiedScore;
							bestOldDissatisfiedScore = localOldDissatisfiedScore;
							bestNewDissatisfiedScore = localNewDissatisfiedScore;
							bestOldNotBlockingScore = localOldNotBlockingScore;
							bestNewNotBlockingScore = localNewNotBlockingScore;
							bestOldBlockingScore = localOldBlockingScore;
							bestNewBlockingScore = localNewBlockingScore;

							if( searchItem->referenceSelectionItem != selectionResult.bestActionItem )
								{
								if( selectionResult.bestActionItem != NULL )
									{
									if( disableAction( true, selectionResult.bestActionItem ) != RESULT_OK )	// Previous best action
										addError( functionNameString, NULL, NULL, "I failed to disable the best action" );
									}

								if( commonVariables()->result == RESULT_OK )
									{
									if( markAction( searchItem->referenceSelectionItem ) != RESULT_OK )	// Current action
										addError( functionNameString, NULL, NULL, "I failed to mark an action" );
									}
								}

							selectionResult.bestActionItem = searchItem->referenceSelectionItem;
							}
						else
							{
							if( searchItem->referenceSelectionItem != selectionResult.bestActionItem )
								{
								if( hasEqualScore_ )
									{
									if( markAction( searchItem->referenceSelectionItem ) == RESULT_OK )	// Current action
										nRandomEntries++;
									else
										addError( functionNameString, NULL, NULL, "I failed to mark an action" );
									}
								else
									{
									if( disableAction( false, searchItem->referenceSelectionItem ) != RESULT_OK )		// Previous best action
										addError( functionNameString, NULL, NULL, "I failed to disable an action" );
									}
								}
							}
						}
					else
						addError( functionNameString, NULL, NULL, "I failed to get the best local score" );
					}

				searchItem = searchItem->nextScoreItem();
				}

			if( commonVariables()->result == RESULT_OK &&
			nRandomEntries > 1 )	// Found more than one the same best cummulate score with different action
				{
				nRandomEntries = ( rand() % nRandomEntries ) + 1;	// More than one equal possibilities. So, use random

				searchItem = firstActiveScoreItem();

				while( searchItem != NULL &&
				nRandomEntries > 0 )
					{
					if( searchItem->isMarked )
						{
						if( --nRandomEntries > 0 )
							{
							localSearchItem = firstActiveScoreItem();

							while( localSearchItem != NULL )
								{
								if( localSearchItem->referenceSelectionItem == searchItem->referenceSelectionItem )
									localSearchItem->isMarked = false;		// Clear check to handled

								localSearchItem = localSearchItem->nextScoreItem();
								}
							}
						else
							selectionResult.bestActionItem = searchItem->referenceSelectionItem;
						}

					searchItem = searchItem->nextScoreItem();
					}
				}
			}

		selectionResult.result = commonVariables()->result;
		return selectionResult;
		}

	ScoreItem *ScoreList::firstPossibility()
		{
		ScoreItem *firstScoreItem;

		if( ( firstScoreItem = firstActiveScoreItem() ) != NULL )
			return firstScoreItem->firstPossibilityItem();

		return NULL;
		}

/*************************************************************************
 *
 *	"He gives food to those who fear him;
 *	he always remembers his convenant." (Psalm 111:5)
 *
 *************************************************************************/
