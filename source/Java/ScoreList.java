/*
 *	Class:			ScoreList
 *	Parent class:	List
 *	Purpose:		To temporarily store score items
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

// Score List header

class ScoreList extends List
	{
	// Private constructible variables

	private boolean hasBetterScore_;
	private boolean hasEqualScore_;
	private boolean hasFoundScore_;

	// Private methods

	private byte markAction( SelectionItem markSelectionReference )
		{
		ScoreItem searchItem = firstActiveScoreItem();

		if( markSelectionReference != null )
			{
			while( searchItem != null )
				{
				if( searchItem.referenceSelectionItem == markSelectionReference )
					searchItem.isMarked = true;		// Set mark

				searchItem = searchItem.nextScoreItem();
				}
			}
		else
			return startError( 1, null, null, "The given score item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte disableAction( boolean isIncludingMarkedActions, SelectionItem disableItem )
		{
		ScoreItem searchItem = firstActiveScoreItem();

		if( disableItem != null )
			{
			while( searchItem != null )
				{
				if( ( isIncludingMarkedActions &&
				searchItem.isMarked ) ||
				searchItem.referenceSelectionItem == disableItem )
					{
					searchItem.isMarked = false;	// Clear mark
					searchItem.isChecked = false;	// Clear check
					}

				searchItem = searchItem.nextScoreItem();
				}
			}
		else
			return startError( 1, null, null, "The given score item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte getBestScore( boolean cummulative, short solveStrategyParameter, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, int bestOldSatisfiedScore, int bestNewSatisfiedScore, int bestOldDissatisfiedScore, int bestNewDissatisfiedScore, int bestOldNotBlockingScore, int bestNewNotBlockingScore, int bestOldBlockingScore, int bestNewBlockingScore )
		{
		boolean equalSatisfiedScore = ( oldSatisfiedScore == bestOldSatisfiedScore &&
										newSatisfiedScore == bestNewSatisfiedScore );
		boolean equalDissatisfiedScore = ( oldDissatisfiedScore == bestOldDissatisfiedScore &&
										newDissatisfiedScore == bestNewDissatisfiedScore );
		boolean equalNotBlockingScore = ( oldNotBlockingScore == bestOldNotBlockingScore &&
										newNotBlockingScore == bestNewNotBlockingScore );
		boolean equalBlockingScore = ( oldBlockingScore == bestOldBlockingScore &&
										newBlockingScore == bestNewBlockingScore );

		double satisfiedScore = (double)oldSatisfiedScore + (double)newSatisfiedScore;
		double dissatisfiedScore = (double)oldDissatisfiedScore + (double)newDissatisfiedScore;
		double notBlockingScore = (double)oldNotBlockingScore + (double)newNotBlockingScore;
		double blockingScore = (double)oldBlockingScore + (double)newBlockingScore;

		double bestSatisfiedScore = (double)bestOldSatisfiedScore + (double)bestNewSatisfiedScore;
		double bestDissatisfiedScore = (double)bestOldDissatisfiedScore + (double)bestNewDissatisfiedScore;
		double bestNotBlockingScore = (double)bestOldNotBlockingScore + (double)bestNewNotBlockingScore;
		double bestBlockingScore = (double)bestOldBlockingScore + (double)bestNewBlockingScore;

		boolean higherSatisfiedScore = ( satisfiedScore > bestSatisfiedScore );
		boolean superiorSatisfiedScore = ( higherSatisfiedScore &&
										satisfiedScore > bestDissatisfiedScore );
		boolean lowerDissatisfiedScore = ( dissatisfiedScore < bestDissatisfiedScore );
		boolean higherDissatisfiedScore = ( dissatisfiedScore > bestDissatisfiedScore );
		boolean lowerNotBlockingScore = ( notBlockingScore < bestNotBlockingScore );
		boolean lowerBlockingScore = ( blockingScore < bestBlockingScore );

		hasBetterScore_ = false;
		hasEqualScore_ = ( equalSatisfiedScore &&
						equalDissatisfiedScore &&
						equalNotBlockingScore &&
						equalBlockingScore );

		if( solveStrategyParameter == Constants.NO_SOLVE_STRATEGY_PARAMETER ||
		solveStrategyParameter == Constants.WORD_PARAMETER_ADJECTIVE_DEFENSIVE ||
		solveStrategyParameter == Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE )
			{
			hasBetterScore_ =	// Get best satisfying strategy,
							( ( solveStrategyParameter == Constants.NO_SOLVE_STRATEGY_PARAMETER &&
								( higherSatisfiedScore ||
								( equalSatisfiedScore &&
								lowerDissatisfiedScore ) ) ) ||

							( solveStrategyParameter == Constants.WORD_PARAMETER_ADJECTIVE_DEFENSIVE &&
								( lowerDissatisfiedScore ||
								( equalDissatisfiedScore &&
								higherSatisfiedScore ) ) ) ||

							( solveStrategyParameter == Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE &&
								// Has no dissatisfied score and superior satisfied score
								( ( superiorSatisfiedScore &&
								( cummulative ||
								dissatisfiedScore == Constants.NO_SCORE ) ) ||

								// Has no old satisfied score and has new satisfied score and higher dissatisfied score
								( higherDissatisfiedScore &&
								( cummulative ||
								( oldSatisfiedScore == Constants.NO_SCORE &&
								newSatisfiedScore > Constants.NO_SCORE ) ) ) ) ) ||

							// else if equal satisfying strategy,
							( equalSatisfiedScore &&
							equalDissatisfiedScore &&

							// Get lowest blocking score, else if equal blocking score, Get lowest not blocking score
							( lowerBlockingScore ||
							( equalBlockingScore &&
							lowerNotBlockingScore ) ) ) );
			}
		else
			return startError( 1, null, null, "The given solve strategy parameter isn't implemented" );

		return Constants.RESULT_OK;
		}

	private ScoreItem firstActiveScoreItem()
		{
		return (ScoreItem)firstActiveItem();
		}

	private ScoreItem nextScoreListItem()
		{
		return (ScoreItem)nextListItem();
		}


	// Constructor / deconstructor

	protected ScoreList( WordItem myWordItem )
		{
		hasBetterScore_ = false;
		hasEqualScore_ = false;
		hasFoundScore_ = false;

		initializeListVariables( Constants.ADMIN_SCORE_LIST_SYMBOL, myWordItem );
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return true;
		}


	// Protected methods

	protected boolean hasFoundScore()
		{
		return hasFoundScore_;
		}

	protected int nPossibilities()
		{
		int nItems = 0;
		ScoreItem searchItem = firstActiveScoreItem();

		while( searchItem != null &&
		searchItem.assignmentLevel() >= CommonVariables.currentAssignmentLevel )
			{
			if( searchItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
				nItems++;

			searchItem = searchItem.nextScoreItem();
			}

		return nItems;
		}

	protected byte changeAction( SelectionItem actionSelectionItem )
		{
		ScoreItem searchItem = firstActiveScoreItem();

		if( actionSelectionItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isChecked )		// All new created scores with assignment level higher than zero
					{
					searchItem.isChecked = false;
					searchItem.referenceSelectionItem = actionSelectionItem;
					}

				searchItem = searchItem.nextScoreItem();
				}
			}
		else
			return startError( 1, null, null, "The given action selection item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkSelectionItemForUsage( SelectionItem unusedSelectionItem )
		{
		ScoreItem searchItem = firstActiveScoreItem();

		if( unusedSelectionItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.referenceSelectionItem == unusedSelectionItem )
					return startError( 1, null, null, "The reference selection item is still in use" );

				searchItem = searchItem.nextScoreItem();
				}
			}
		else
			return startError( 1, null, null, "The given unused justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte findScore( boolean prepareSort, SelectionItem findScoreItem )
		{
		ScoreItem searchItem = firstActiveScoreItem();

		hasFoundScore_ = false;

		if( findScoreItem != null )
			{
			while( searchItem != null &&
			searchItem.assignmentLevel() >= CommonVariables.currentAssignmentLevel )
				{
				if( searchItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
					{
					if( ( searchItem.isChecked ||
					CommonVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL ) &&
					searchItem.referenceSelectionItem == findScoreItem )
						{
						hasFoundScore_ = true;
						searchItem.isMarked = prepareSort;
						}
					}

				searchItem = searchItem.nextScoreItem();
				}
			}
		else
			return startError( 1, null, null, "The given score item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte deleteScores()
		{
		ScoreItem searchItem = firstActiveScoreItem();

		while( searchItem != null &&
		searchItem.assignmentLevel() >= CommonVariables.currentAssignmentLevel )
			{
			if( searchItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
				{
				if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
					searchItem = nextScoreListItem();
				else
					return addError( 1, null, null, "I failed to delete an active item" );
				}
			else
				searchItem = searchItem.nextScoreItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkScores( boolean isInverted, short solveStrategyParameter, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore )
		{
		int checkOldSatisfiedScore = ( isInverted ? oldDissatisfiedScore : oldSatisfiedScore );
		int checkNewSatisfiedScore = ( isInverted ? newDissatisfiedScore : newSatisfiedScore );
		int checkOldDissatisfiedScore = ( isInverted ? oldSatisfiedScore : oldDissatisfiedScore );
		int checkNewDissatisfiedScore = ( isInverted ? newSatisfiedScore : newDissatisfiedScore );
		ScoreItem searchItem = firstActiveScoreItem();

		hasFoundScore_ = false;

		if( checkOldSatisfiedScore > Constants.NO_SCORE ||
		checkNewSatisfiedScore > Constants.NO_SCORE ||
		checkOldDissatisfiedScore > Constants.NO_SCORE ||
		checkNewDissatisfiedScore > Constants.NO_SCORE ||

		oldNotBlockingScore > Constants.NO_SCORE ||
		newNotBlockingScore > Constants.NO_SCORE ||
		oldBlockingScore > Constants.NO_SCORE ||
		newBlockingScore > Constants.NO_SCORE )
			{
			while( searchItem != null &&
			searchItem.assignmentLevel() >= CommonVariables.currentAssignmentLevel )
				{
				if( searchItem.assignmentLevel() == CommonVariables.currentAssignmentLevel )
					{
					if( !searchItem.hasOldSatisfiedScore() &&
					!searchItem.hasNewSatisfiedScore() &&
					!searchItem.hasOldDissatisfiedScore() &&
					!searchItem.hasNewDissatisfiedScore() &&
					!searchItem.hasOldNotBlockingScore() &&
					!searchItem.hasNewNotBlockingScore() &&
					!searchItem.hasOldBlockingScore() &&
					!searchItem.hasNewBlockingScore() )	// All new created (=empty) scores
						{
						hasFoundScore_ = true;

						searchItem.oldSatisfiedScore = checkOldSatisfiedScore;
						searchItem.newSatisfiedScore = checkNewSatisfiedScore;
						searchItem.oldDissatisfiedScore = checkOldDissatisfiedScore;
						searchItem.newDissatisfiedScore = checkNewDissatisfiedScore;
						searchItem.oldNotBlockingScore = oldNotBlockingScore;
						searchItem.newNotBlockingScore = newNotBlockingScore;
						searchItem.oldBlockingScore = oldBlockingScore;
						searchItem.newBlockingScore = newBlockingScore;
						}
					else
						{
						if( searchItem.isMarked )
							{
							if( getBestScore( false, solveStrategyParameter, checkOldSatisfiedScore, checkNewSatisfiedScore, checkOldDissatisfiedScore, checkNewDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, searchItem.oldSatisfiedScore, searchItem.newSatisfiedScore, searchItem.oldDissatisfiedScore, searchItem.newDissatisfiedScore, searchItem.oldNotBlockingScore, searchItem.newNotBlockingScore, searchItem.oldBlockingScore, searchItem.newBlockingScore ) == Constants.RESULT_OK )
								{
								hasFoundScore_ = true;

								if( hasBetterScore_ )
									{
									searchItem.oldSatisfiedScore = checkOldSatisfiedScore;
									searchItem.newSatisfiedScore = checkNewSatisfiedScore;
									searchItem.oldDissatisfiedScore = checkOldDissatisfiedScore;
									searchItem.newDissatisfiedScore = checkNewDissatisfiedScore;
									searchItem.oldNotBlockingScore = oldNotBlockingScore;
									searchItem.newNotBlockingScore = newNotBlockingScore;
									searchItem.oldBlockingScore = oldBlockingScore;
									searchItem.newBlockingScore = newBlockingScore;
									}
								}
							else
								return addError( 1, null, null, "I failed to get the best score" );
							}
						}
					}

				searchItem = searchItem.nextScoreItem();
				}
			}
		else
			return startError( 1, null, null, "None of the given scores has a value parameter" );

		return Constants.RESULT_OK;
		}

	protected byte createScoreItem( boolean isChecked, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, SelectionItem referenceSelectionItem )
		{
		if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
			{
			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new ScoreItem( isChecked, CommonVariables.currentAssignmentLevel, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem, this, myWordItem() ) ) != Constants.RESULT_OK )
				return addError( 1, null, null, "I failed to add an active score item" );
			}
		else
			return startError( 1, null, null, "The current item number is undefined" );

		return Constants.RESULT_OK;
		}

	protected SelectionResultType getBestAction( short solveStrategyParameter )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		boolean cummulate = false;
		int nRandomEntries = 0;
		int nLocalLosingScores;
		int nLocalWinningScores;
		int nBestWinningScores = 0;
		int nBestLosingScores = Constants.MAX_NUMBER;
		int localOldSatisfiedScore;
		int localNewSatisfiedScore;
		int localOldDissatisfiedScore;
		int localNewDissatisfiedScore;
		int localOldNotBlockingScore;
		int localNewNotBlockingScore;
		int localOldBlockingScore;
		int localNewBlockingScore;
		int bestOldSatisfiedScore = Constants.NO_SCORE;
		int bestNewSatisfiedScore = Constants.NO_SCORE;
		int bestOldDissatisfiedScore = ( solveStrategyParameter == Constants.NO_SOLVE_STRATEGY_PARAMETER ? Constants.MAX_SCORE : Constants.NO_SCORE );
		int bestNewDissatisfiedScore = bestOldDissatisfiedScore;
		int bestOldNotBlockingScore = Constants.MAX_SCORE;
		int bestNewNotBlockingScore = Constants.MAX_SCORE;
		int bestOldBlockingScore = Constants.MAX_SCORE;
		int bestNewBlockingScore = Constants.MAX_SCORE;
		ScoreItem localSearchItem;
		ScoreItem searchItem = firstActiveScoreItem();

		hasEqualScore_ = false;
		hasBetterScore_ = false;

		while( searchItem != null )
			{
			searchItem.isMarked = false;	// Clear all marks
			searchItem.isChecked = true;	// Set all checks
			searchItem = searchItem.nextScoreItem();
			}

		searchItem = firstActiveScoreItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		searchItem != null )
			{
			if( searchItem.isChecked )
				{
				nLocalLosingScores = 0;
				nLocalWinningScores = 0;
				localOldSatisfiedScore = Constants.NO_SCORE;
				localNewSatisfiedScore = Constants.NO_SCORE;
				localOldDissatisfiedScore = ( solveStrategyParameter == Constants.NO_SOLVE_STRATEGY_PARAMETER ? Constants.MAX_SCORE : Constants.NO_SCORE );
				localNewDissatisfiedScore = localOldDissatisfiedScore;
				localOldNotBlockingScore = Constants.MAX_SCORE;
				localNewNotBlockingScore = Constants.MAX_SCORE;
				localOldBlockingScore = Constants.MAX_SCORE;
				localNewBlockingScore = Constants.MAX_SCORE;
				localSearchItem = firstActiveScoreItem();

				while( CommonVariables.result == Constants.RESULT_OK &&
				localSearchItem != null )
					{
					if( localSearchItem.referenceSelectionItem == searchItem.referenceSelectionItem )
						{
						localSearchItem.isChecked = false;		// Is handled

						if( localSearchItem.newSatisfiedScore == Constants.WINNING_SCORE )
							nLocalWinningScores++;
						else
							{
							if( localSearchItem.newDissatisfiedScore == Constants.WINNING_SCORE )
								nLocalLosingScores++;
							else
								{
								if( getBestScore( false, solveStrategyParameter, localSearchItem.oldSatisfiedScore, localSearchItem.newSatisfiedScore, localSearchItem.oldDissatisfiedScore, localSearchItem.newDissatisfiedScore, localSearchItem.oldNotBlockingScore, localSearchItem.newNotBlockingScore, localSearchItem.oldBlockingScore, localSearchItem.newBlockingScore, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore ) == Constants.RESULT_OK )
									{
									if( hasBetterScore_ )
										{
										localOldSatisfiedScore = localSearchItem.oldSatisfiedScore;
										localNewSatisfiedScore = localSearchItem.newSatisfiedScore;
										localOldDissatisfiedScore = localSearchItem.oldDissatisfiedScore;
										localNewDissatisfiedScore = localSearchItem.newDissatisfiedScore;
										localOldNotBlockingScore = localSearchItem.oldNotBlockingScore;
										localNewNotBlockingScore = localSearchItem.newNotBlockingScore;
										localOldBlockingScore = localSearchItem.oldBlockingScore;
										localNewBlockingScore = localSearchItem.newBlockingScore;
										}
									}
								else
									addError( 1, null, null, "I failed to get the best local score" );
								}
							}
						}

					localSearchItem = localSearchItem.nextScoreItem();
					}

				hasEqualScore_ = ( nLocalWinningScores == nBestWinningScores &&
				nLocalLosingScores == nBestLosingScores );

				if( hasEqualScore_ )
					{
					if( getBestScore( false, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) != Constants.RESULT_OK )
						addError( 1, null, null, "I failed to get the best score" );
					}
				else
					// Get highest number of winning scores, else if equal: Get lowest number of losing scores
					hasBetterScore_ = ( nLocalWinningScores > nBestWinningScores ||

										( nLocalWinningScores == nBestWinningScores &&
										nLocalLosingScores < nBestLosingScores ) );

				if( CommonVariables.result == Constants.RESULT_OK )
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

						if( searchItem.referenceSelectionItem != selectionResult.bestActionItem )
							{
							if( selectionResult.bestActionItem != null )
								{
								if( disableAction( true, selectionResult.bestActionItem ) != Constants.RESULT_OK )	// Previous best action
									addError( 1, null, null, "I failed to disable the best action" );
								}

							if( CommonVariables.result == Constants.RESULT_OK )
								{
								if( markAction( searchItem.referenceSelectionItem ) != Constants.RESULT_OK )	// Current action
									addError( 1, null, null, "I failed to mark an action" );
								}
							}

						selectionResult.bestActionItem = searchItem.referenceSelectionItem;
						}
					else
						{
						if( searchItem.referenceSelectionItem != selectionResult.bestActionItem )
							{
							if( hasEqualScore_ )
								{
								if( markAction( searchItem.referenceSelectionItem ) == Constants.RESULT_OK )	// Current action
									cummulate = true;	// Found the same best score with different action
								else
									addError( 1, null, null, "I failed to mark an action" );
								}
							else
								{
								if( disableAction( false, searchItem.referenceSelectionItem ) != Constants.RESULT_OK )		// Previous best action
									addError( 1, null, null, "I failed to disable an action" );
								}
							}
						}
					}
				}

			searchItem = searchItem.nextScoreItem();
			}

		if( CommonVariables.result == Constants.RESULT_OK &&
		cummulate )		// Found the same best score with different action
			{
			bestOldSatisfiedScore = Constants.NO_SCORE;
			bestNewSatisfiedScore = Constants.NO_SCORE;
			bestOldDissatisfiedScore = ( solveStrategyParameter == Constants.NO_SOLVE_STRATEGY_PARAMETER ? Constants.MAX_SCORE : Constants.NO_SCORE );
			bestNewDissatisfiedScore = bestOldDissatisfiedScore;
			bestOldNotBlockingScore = Constants.MAX_SCORE;
			bestNewNotBlockingScore = Constants.MAX_SCORE;
			bestOldBlockingScore = Constants.MAX_SCORE;
			bestNewBlockingScore = Constants.MAX_SCORE;

			selectionResult.bestActionItem = null;

			searchItem = firstActiveScoreItem();

			while( searchItem != null )
				{
				searchItem.isChecked = searchItem.isMarked;	// Copy the checks
				searchItem.isMarked = false;					// Clear all marks
				searchItem = searchItem.nextScoreItem();
				}

			searchItem = firstActiveScoreItem();

			while( CommonVariables.result == Constants.RESULT_OK &&
			searchItem != null )	// Search only for new scores of best actions
				{
				if( searchItem.isChecked )
					{
					localOldSatisfiedScore = Constants.NO_SCORE;
					localNewSatisfiedScore = Constants.NO_SCORE;
					localOldDissatisfiedScore = Constants.NO_SCORE;
					localNewDissatisfiedScore = Constants.NO_SCORE;
					localOldNotBlockingScore = Constants.NO_SCORE;
					localNewNotBlockingScore = Constants.NO_SCORE;
					localOldBlockingScore = Constants.NO_SCORE;
					localNewBlockingScore = Constants.NO_SCORE;

					localSearchItem = firstActiveScoreItem();

					while( localSearchItem != null )
						{
						if( localSearchItem.referenceSelectionItem == searchItem.referenceSelectionItem )
							{
							localSearchItem.isChecked = false;		// Clear handled check

							// Don't add winning or losing scores
							if( localSearchItem.newSatisfiedScore != Constants.WINNING_SCORE &&
							localSearchItem.newDissatisfiedScore != Constants.WINNING_SCORE &&

							// Only add if solve strategy isn't EXCLUSIVE-OFFENSIVE or it has no old satisfied score and it has new satisfied score or it has no dissatisfied score
							( solveStrategyParameter != Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE ||

							( !localSearchItem.hasOldSatisfiedScore() &&
							localSearchItem.hasNewSatisfiedScore() ) ||

							( !localSearchItem.hasOldDissatisfiedScore() &&
							!localSearchItem.hasNewDissatisfiedScore() ) ) )
								{
								if( Constants.MAX_SCORE - localOldSatisfiedScore > localSearchItem.oldSatisfiedScore )
									{
									if( Constants.MAX_SCORE - localNewSatisfiedScore > localSearchItem.newSatisfiedScore )
										{
										if( Constants.MAX_SCORE - localOldDissatisfiedScore > localSearchItem.oldDissatisfiedScore )
											{
											if( Constants.MAX_SCORE - localNewDissatisfiedScore > localSearchItem.newDissatisfiedScore )
												{
												if( Constants.MAX_SCORE - localOldNotBlockingScore > localSearchItem.oldNotBlockingScore )
													{
													if( Constants.MAX_SCORE - localNewNotBlockingScore > localSearchItem.newNotBlockingScore )
														{
														if( Constants.MAX_SCORE - localOldBlockingScore > localSearchItem.oldBlockingScore )
															{
															if( Constants.MAX_SCORE - localNewBlockingScore > localSearchItem.newBlockingScore )
																{
																localOldSatisfiedScore += localSearchItem.oldSatisfiedScore;
																localNewSatisfiedScore += localSearchItem.newSatisfiedScore;
																localOldDissatisfiedScore += localSearchItem.oldDissatisfiedScore;
																localNewDissatisfiedScore += localSearchItem.newDissatisfiedScore;
																localOldNotBlockingScore += localSearchItem.oldNotBlockingScore;
																localNewNotBlockingScore += localSearchItem.newNotBlockingScore;
																localOldBlockingScore += localSearchItem.oldBlockingScore;
																localNewBlockingScore += localSearchItem.newBlockingScore;
																}
															else
																startSystemError( 1, null, null, "Overflow of the new blocking cummulate score" );
															}
														else
															startSystemError( 1, null, null, "Overflow of the old blocking cummulate score" );
														}
													else
														startSystemError( 1, null, null, "Overflow of the new not-blocking cummulate score" );
													}
												else
													startSystemError( 1, null, null, "Overflow of the old not-blocking cummulate score" );
												}
											else
												startSystemError( 1, null, null, "Overflow of the new dissatisfied cummulate score" );
											}
										else
											startSystemError( 1, null, null, "Overflow of the old dissatisfied cummulate score" );
										}
									else
										startSystemError( 1, null, null, "Overflow of the new satisfied cummulate score" );
									}
								else
									startSystemError( 1, null, null, "Overflow of the old satisfied cummulate score" );
								}
							}

						localSearchItem = localSearchItem.nextScoreItem();
						}

					if( getBestScore( true, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) == Constants.RESULT_OK )
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

							if( searchItem.referenceSelectionItem != selectionResult.bestActionItem )
								{
								if( selectionResult.bestActionItem != null )
									{
									if( disableAction( true, selectionResult.bestActionItem ) != Constants.RESULT_OK )	// Previous best action
										addError( 1, null, null, "I failed to disable the best action" );
									}

								if( CommonVariables.result == Constants.RESULT_OK )
									{
									if( markAction( searchItem.referenceSelectionItem ) != Constants.RESULT_OK )	// Current action
										addError( 1, null, null, "I failed to mark an action" );
									}
								}

							selectionResult.bestActionItem = searchItem.referenceSelectionItem;
							}
						else
							{
							if( searchItem.referenceSelectionItem != selectionResult.bestActionItem )
								{
								if( hasEqualScore_ )
									{
									if( markAction( searchItem.referenceSelectionItem ) == Constants.RESULT_OK )	// Current action
										nRandomEntries++;
									else
										addError( 1, null, null, "I failed to mark an action" );
									}
								else
									{
									if( disableAction( false, searchItem.referenceSelectionItem ) != Constants.RESULT_OK )		// Previous best action
										addError( 1, null, null, "I failed to disable an action" );
									}
								}
							}
						}
					else
						addError( 1, null, null, "I failed to get the best local score" );
					}

				searchItem = searchItem.nextScoreItem();
				}

			if( CommonVariables.result == Constants.RESULT_OK &&
			nRandomEntries > 1 )	// Found more than one the same best cummulate score with different action
				{
				nRandomEntries = (int)( nRandomEntries * Math.random() ) + 1;	// More than one equal possibilities. So, use random

				searchItem = firstActiveScoreItem();

				while( searchItem != null &&
				nRandomEntries > 0 )
					{
					if( searchItem.isMarked )
						{
						if( --nRandomEntries > 0 )
							{
							localSearchItem = firstActiveScoreItem();

							while( localSearchItem != null )
								{
								if( localSearchItem.referenceSelectionItem == searchItem.referenceSelectionItem )
									localSearchItem.isMarked = false;		// Clear check to handled

								localSearchItem = localSearchItem.nextScoreItem();
								}
							}
						else
							selectionResult.bestActionItem = searchItem.referenceSelectionItem;
						}

					searchItem = searchItem.nextScoreItem();
					}
				}
			}

		selectionResult.result = CommonVariables.result;
		return selectionResult;
		}

	protected ScoreItem firstPossibility()
		{
		ScoreItem firstScoreItem;

		if( ( firstScoreItem = firstActiveScoreItem() ) != null )
			return firstScoreItem.firstPossibilityItem();

		return null;
		}
	};

/*************************************************************************
 *	"He gives food to those who fear him;
 *	he always remembers his convenant." (Psalm 111:5)
 *************************************************************************/
