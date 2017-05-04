/*	Class:			ScoreList
 *	Parent class:	List
 *	Purpose:		To temporarily store score items
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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


class ScoreList extends List
	{
	// Private methods

	private static boolean hasEqualScore( int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, int bestOldSatisfiedScore, int bestNewSatisfiedScore, int bestOldDissatisfiedScore, int bestNewDissatisfiedScore, int bestOldNotBlockingScore, int bestNewNotBlockingScore, int bestOldBlockingScore, int bestNewBlockingScore )
		{
		boolean isEqualSatisfiedScore = ( oldSatisfiedScore == bestOldSatisfiedScore &&
										newSatisfiedScore == bestNewSatisfiedScore );
		boolean isEqualDissatisfiedScore = ( oldDissatisfiedScore == bestOldDissatisfiedScore &&
										newDissatisfiedScore == bestNewDissatisfiedScore );
		boolean isEqualNotBlockingScore = ( oldNotBlockingScore == bestOldNotBlockingScore &&
										newNotBlockingScore == bestNewNotBlockingScore );
		boolean isEqualBlockingScore = ( oldBlockingScore == bestOldBlockingScore &&
										newBlockingScore == bestNewBlockingScore );

		return( isEqualSatisfiedScore &&
				isEqualDissatisfiedScore &&
				isEqualNotBlockingScore &&
				isEqualBlockingScore );
		}

	private byte markAction( SelectionItem markSelectionReference )
		{
		ScoreItem searchScoreItem = firstActiveScoreItem();

		if( markSelectionReference == null )
			return startError( 1, "The given score item is undefined" );

		while( searchScoreItem != null )
			{
			if( searchScoreItem.referenceSelectionItem == markSelectionReference )
				// Mark action
				searchScoreItem.isMarked = true;

			searchScoreItem = searchScoreItem.nextScoreItem();
			}

		return Constants.RESULT_OK;
		}

	private byte disableAction( boolean isIncludingMarkedActions, SelectionItem disableItem )
		{
		ScoreItem searchScoreItem = firstActiveScoreItem();

		if( disableItem == null )
			return startError( 1, "The given score item is undefined" );

		while( searchScoreItem != null )
			{
			if( ( isIncludingMarkedActions &&
			searchScoreItem.isMarked ) ||
			searchScoreItem.referenceSelectionItem == disableItem )
				{
				// Clear action
				searchScoreItem.isMarked = false;
				// Clear check
				searchScoreItem.isChecked = false;
				}

			searchScoreItem = searchScoreItem.nextScoreItem();
			}

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

	private BoolResultType getBestScore( boolean isCummulative, short solveStrategyParameter, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, int bestOldSatisfiedScore, int bestNewSatisfiedScore, int bestOldDissatisfiedScore, int bestNewDissatisfiedScore, int bestOldNotBlockingScore, int bestNewNotBlockingScore, int bestOldBlockingScore, int bestNewBlockingScore )
		{
		boolean isEqualSatisfiedScore = ( oldSatisfiedScore == bestOldSatisfiedScore &&
										newSatisfiedScore == bestNewSatisfiedScore );
		boolean isEqualDissatisfiedScore = ( oldDissatisfiedScore == bestOldDissatisfiedScore &&
										newDissatisfiedScore == bestNewDissatisfiedScore );
		boolean isEqualBlockingScore = ( oldBlockingScore == bestOldBlockingScore &&
										newBlockingScore == bestNewBlockingScore );

		double satisfiedScore = (double)oldSatisfiedScore + (double)newSatisfiedScore;
		double dissatisfiedScore = (double)oldDissatisfiedScore + (double)newDissatisfiedScore;
		double notBlockingScore = (double)oldNotBlockingScore + (double)newNotBlockingScore;
		double blockingScore = (double)oldBlockingScore + (double)newBlockingScore;

		double bestSatisfiedScore = (double)bestOldSatisfiedScore + (double)bestNewSatisfiedScore;
		double bestDissatisfiedScore = (double)bestOldDissatisfiedScore + (double)bestNewDissatisfiedScore;
		double bestNotBlockingScore = (double)bestOldNotBlockingScore + (double)bestNewNotBlockingScore;
		double bestBlockingScore = (double)bestOldBlockingScore + (double)bestNewBlockingScore;

		boolean isHigherSatisfiedScore = ( satisfiedScore > bestSatisfiedScore );
		boolean isSuperiorSatisfiedScore = ( isHigherSatisfiedScore &&
										satisfiedScore > bestDissatisfiedScore );
		boolean isLowerDissatisfiedScore = ( dissatisfiedScore < bestDissatisfiedScore );
		boolean isHigherDissatisfiedScore = ( dissatisfiedScore > bestDissatisfiedScore );
		boolean isLowerNotBlockingScore = ( notBlockingScore < bestNotBlockingScore );
		boolean isLowerBlockingScore = ( blockingScore < bestBlockingScore );
		BoolResultType boolResult = new BoolResultType();

		if( solveStrategyParameter != Constants.NO_SOLVE_STRATEGY_PARAMETER &&
		solveStrategyParameter != Constants.WORD_PARAMETER_ADJECTIVE_DEFENSIVE &&
		solveStrategyParameter != Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE )
			return startBoolResultError( 1, "The given solve strategy parameter is invalid" );

		// Has better score
		boolResult.booleanValue =	( ( solveStrategyParameter == Constants.NO_SOLVE_STRATEGY_PARAMETER &&
									( isHigherSatisfiedScore ||
									( isEqualSatisfiedScore &&
									isLowerDissatisfiedScore ) ) ) ||

								( solveStrategyParameter == Constants.WORD_PARAMETER_ADJECTIVE_DEFENSIVE &&
									( isLowerDissatisfiedScore ||
									( isEqualDissatisfiedScore &&
									isHigherSatisfiedScore ) ) ) ||

								( solveStrategyParameter == Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE &&
									// Has no dissatisfied score and superior satisfied score
									( ( isSuperiorSatisfiedScore &&
									( isCummulative ||
									dissatisfiedScore == Constants.NO_SCORE ) ) ||

									// Has no old satisfied score and has new satisfied score and higher dissatisfied score
									( isHigherDissatisfiedScore &&
									( isCummulative ||
									( oldSatisfiedScore == Constants.NO_SCORE &&
									newSatisfiedScore > Constants.NO_SCORE ) ) ) ) ) ||

								// else if equal satisfying strategy,
								( isEqualSatisfiedScore &&
								isEqualDissatisfiedScore &&

								// Get lowest blocking score, else if equal blocking score, Get lowest not blocking score
								( isLowerBlockingScore ||
								( isEqualBlockingScore &&
								isLowerNotBlockingScore ) ) ) );

		return boolResult;
		}


	// Constructor

	protected ScoreList( WordItem myWordItem )
		{
		// Private constructed variables

		initializeListVariables( Constants.ADMIN_SCORE_LIST_SYMBOL, "ScoreList", myWordItem );
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return true;
		}


	// Protected methods

	protected int nPossibilities()
		{
		short currentAssignmentLevel = CommonVariables.currentAssignmentLevel;
		int nItems = 0;
		ScoreItem searchScoreItem = firstActiveScoreItem();

		while( searchScoreItem != null &&
		searchScoreItem.assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchScoreItem.assignmentLevel() == currentAssignmentLevel )
				nItems++;

			searchScoreItem = searchScoreItem.nextScoreItem();
			}

		return nItems;
		}

	protected byte changeAction( SelectionItem actionSelectionItem )
		{
		ScoreItem searchScoreItem = firstActiveScoreItem();

		if( actionSelectionItem == null )
			return startError( 1, "The given action selection item is undefined" );

		while( searchScoreItem != null )
			{
			// All new created scores with assignment level higher than zero
			if( searchScoreItem.isChecked )
				{
				searchScoreItem.isChecked = false;
				searchScoreItem.referenceSelectionItem = actionSelectionItem;
				}

			searchScoreItem = searchScoreItem.nextScoreItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte createScoreItem( boolean isChecked, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, SelectionItem referenceSelectionItem )
		{
		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new ScoreItem( isChecked, CommonVariables.currentAssignmentLevel, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add an active score item" );

		return Constants.RESULT_OK;
		}

	protected byte deleteScores()
		{
		short currentAssignmentLevel = CommonVariables.currentAssignmentLevel;
		ScoreItem searchScoreItem = firstActiveScoreItem();

		while( searchScoreItem != null &&
		searchScoreItem.assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchScoreItem.assignmentLevel() == currentAssignmentLevel )
				{
				if( deleteItem( searchScoreItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to delete an active item" );

				searchScoreItem = nextScoreListItem();
				}
			else
				searchScoreItem = searchScoreItem.nextScoreItem();
			}

		return Constants.RESULT_OK;
		}

	protected ScoreItem firstPossibility()
		{
		ScoreItem firstScoreItem;

		if( ( firstScoreItem = firstActiveScoreItem() ) != null )
			return firstScoreItem.firstPossibilityScoreItem();

		return null;
		}

	protected BoolResultType checkScores( boolean isInverted, short solveStrategyParameter, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore )
		{
		short currentAssignmentLevel = CommonVariables.currentAssignmentLevel;
		int checkOldSatisfiedScore = ( isInverted ? oldDissatisfiedScore : oldSatisfiedScore );
		int checkNewSatisfiedScore = ( isInverted ? newDissatisfiedScore : newSatisfiedScore );
		int checkOldDissatisfiedScore = ( isInverted ? oldSatisfiedScore : oldDissatisfiedScore );
		int checkNewDissatisfiedScore = ( isInverted ? newSatisfiedScore : newDissatisfiedScore );
		ScoreItem searchScoreItem = firstActiveScoreItem();
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult = new BoolResultType();

		if( checkOldSatisfiedScore <= Constants.NO_SCORE &&
		checkNewSatisfiedScore <= Constants.NO_SCORE &&
		checkOldDissatisfiedScore <= Constants.NO_SCORE &&
		checkNewDissatisfiedScore <= Constants.NO_SCORE &&

		oldNotBlockingScore <= Constants.NO_SCORE &&
		newNotBlockingScore <= Constants.NO_SCORE &&
		oldBlockingScore <= Constants.NO_SCORE &&
		newBlockingScore <= Constants.NO_SCORE )
			return startBoolResultError( 1, "None of the given scores has a value parameter" );

		while( searchScoreItem != null &&
		searchScoreItem.assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchScoreItem.assignmentLevel() == currentAssignmentLevel )
				{
				// All new created (=empty) scores
				if( !searchScoreItem.hasOldSatisfiedScore() &&
				!searchScoreItem.hasNewSatisfiedScore() &&
				!searchScoreItem.hasOldDissatisfiedScore() &&
				!searchScoreItem.hasNewDissatisfiedScore() &&
				!searchScoreItem.hasOldNotBlockingScore() &&
				!searchScoreItem.hasNewNotBlockingScore() &&
				!searchScoreItem.hasOldBlockingScore() &&
				!searchScoreItem.hasNewBlockingScore() )
					{
					// Has found score
					boolReturnResult.booleanValue = true;

					searchScoreItem.oldSatisfiedScore = checkOldSatisfiedScore;
					searchScoreItem.newSatisfiedScore = checkNewSatisfiedScore;
					searchScoreItem.oldDissatisfiedScore = checkOldDissatisfiedScore;
					searchScoreItem.newDissatisfiedScore = checkNewDissatisfiedScore;
					searchScoreItem.oldNotBlockingScore = oldNotBlockingScore;
					searchScoreItem.newNotBlockingScore = newNotBlockingScore;
					searchScoreItem.oldBlockingScore = oldBlockingScore;
					searchScoreItem.newBlockingScore = newBlockingScore;
					}
				else
					{
					if( searchScoreItem.isMarked )
						{
						if( ( boolCheckResult = getBestScore( false, solveStrategyParameter, checkOldSatisfiedScore, checkNewSatisfiedScore, checkOldDissatisfiedScore, checkNewDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, searchScoreItem.oldSatisfiedScore, searchScoreItem.newSatisfiedScore, searchScoreItem.oldDissatisfiedScore, searchScoreItem.newDissatisfiedScore, searchScoreItem.oldNotBlockingScore, searchScoreItem.newNotBlockingScore, searchScoreItem.oldBlockingScore, searchScoreItem.newBlockingScore ) ).result != Constants.RESULT_OK )
							return addBoolResultError( 1, "I failed to get the best score" );

						// Has better score
						if( boolCheckResult.booleanValue )
							{
							searchScoreItem.oldSatisfiedScore = checkOldSatisfiedScore;
							searchScoreItem.newSatisfiedScore = checkNewSatisfiedScore;
							searchScoreItem.oldDissatisfiedScore = checkOldDissatisfiedScore;
							searchScoreItem.newDissatisfiedScore = checkNewDissatisfiedScore;
							searchScoreItem.oldNotBlockingScore = oldNotBlockingScore;
							searchScoreItem.newNotBlockingScore = newNotBlockingScore;
							searchScoreItem.oldBlockingScore = oldBlockingScore;
							searchScoreItem.newBlockingScore = newBlockingScore;
							}

						// Has found score
						boolReturnResult.booleanValue = true;
						}
					}
				}

			searchScoreItem = searchScoreItem.nextScoreItem();
			}

		return boolReturnResult;
		}

	protected BoolResultType findScore( boolean isPreparingSort, SelectionItem findScoreItem )
		{
		BoolResultType boolResult = new BoolResultType();
		short currentAssignmentLevel = CommonVariables.currentAssignmentLevel;
		ScoreItem searchScoreItem = firstActiveScoreItem();

		if( findScoreItem == null )
			return startBoolResultError( 1, "The given score item is undefined" );

		while( searchScoreItem != null &&
		searchScoreItem.assignmentLevel() >= currentAssignmentLevel )
			{
			if( ( currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL ||
			searchScoreItem.isChecked ) &&

			searchScoreItem.assignmentLevel() == currentAssignmentLevel &&
			searchScoreItem.referenceSelectionItem == findScoreItem )
				{
				// Has found score
				boolResult.booleanValue = true;
				searchScoreItem.isMarked = isPreparingSort;
				}

			searchScoreItem = searchScoreItem.nextScoreItem();
			}

		return boolResult;
		}

	protected SelectionResultType getBestSelection( boolean isCurrentlyTesting, short solveStrategyParameter )
		{
		SelectionResultType selectionResult = new SelectionResultType();
		boolean hasBetterScore = false;
		boolean isCummulate = false;
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
		ScoreItem localScoreItem;
		ScoreItem searchScoreItem = firstActiveScoreItem();
		SelectionItem bestActionSelectionItem = null;
		BoolResultType boolResult;

		while( searchScoreItem != null )
			{
			// Clear all marked actions
			searchScoreItem.isMarked = false;
			// Set all checks
			searchScoreItem.isChecked = true;
			searchScoreItem = searchScoreItem.nextScoreItem();
			}

		searchScoreItem = firstActiveScoreItem();

		while( searchScoreItem != null )
			{
			if( searchScoreItem.isChecked )
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
				localScoreItem = firstActiveScoreItem();

				while( localScoreItem != null )
					{
					if( localScoreItem.referenceSelectionItem == searchScoreItem.referenceSelectionItem )
						{
						// Already processed
						localScoreItem.isChecked = false;

						if( localScoreItem.newSatisfiedScore == Constants.WINNING_SCORE )
							nLocalWinningScores++;
						else
							{
							if( localScoreItem.newDissatisfiedScore == Constants.WINNING_SCORE )
								nLocalLosingScores++;
							else
								{
								if( ( boolResult = getBestScore( false, solveStrategyParameter, localScoreItem.oldSatisfiedScore, localScoreItem.newSatisfiedScore, localScoreItem.oldDissatisfiedScore, localScoreItem.newDissatisfiedScore, localScoreItem.oldNotBlockingScore, localScoreItem.newNotBlockingScore, localScoreItem.oldBlockingScore, localScoreItem.newBlockingScore, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore ) ).result != Constants.RESULT_OK )
									return addSelectionResultError( 1, "I failed to get the best local score" );

								// Has better score
								if( boolResult.booleanValue )
									{
									localOldSatisfiedScore = localScoreItem.oldSatisfiedScore;
									localNewSatisfiedScore = localScoreItem.newSatisfiedScore;
									localOldDissatisfiedScore = localScoreItem.oldDissatisfiedScore;
									localNewDissatisfiedScore = localScoreItem.newDissatisfiedScore;
									localOldNotBlockingScore = localScoreItem.oldNotBlockingScore;
									localNewNotBlockingScore = localScoreItem.newNotBlockingScore;
									localOldBlockingScore = localScoreItem.oldBlockingScore;
									localNewBlockingScore = localScoreItem.newBlockingScore;
									}
								}
							}
						}

					localScoreItem = localScoreItem.nextScoreItem();
					}

				if( nLocalWinningScores == nBestWinningScores &&
				nLocalLosingScores == nBestLosingScores )
					{
					if( ( boolResult = getBestScore( false, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) ).result != Constants.RESULT_OK )
						return addSelectionResultError( 1, "I failed to get the best score" );

					// Has better score
					hasBetterScore = boolResult.booleanValue;
					}
				else
					{
					// Get highest number of winning scores, else if equal: Get lowest number of losing scores
					hasBetterScore = ( nLocalWinningScores > nBestWinningScores ||

										( nLocalWinningScores == nBestWinningScores &&
										nLocalLosingScores < nBestLosingScores ) );
					}

				if( hasBetterScore )
					{
					isCummulate = false;
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

					if( searchScoreItem.referenceSelectionItem != bestActionSelectionItem )
						{
						if( bestActionSelectionItem != null &&
						// Previous best action
						disableAction( true, bestActionSelectionItem ) != Constants.RESULT_OK )
							return addSelectionResultError( 1, "I failed to disable the best action" );

						// Current action
						if( markAction( searchScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
							return addSelectionResultError( 1, "I failed to mark an action" );
						}

					bestActionSelectionItem = searchScoreItem.referenceSelectionItem;
					}
				else
					{
					if( searchScoreItem.referenceSelectionItem != bestActionSelectionItem )
						{
						if( hasEqualScore( localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) )
							{
							// Current action
							if( markAction( searchScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
								return addSelectionResultError( 1, "I failed to mark an action" );

							// Found the same best score with different action
							isCummulate = true;
							}
						else
							{
							// Previous best action
							if( disableAction( false, searchScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
								return addSelectionResultError( 1, "I failed to disable an action" );
							}
						}
					}
				}

			searchScoreItem = searchScoreItem.nextScoreItem();
			}

		// Found the same best score with different action
		if( isCummulate )
			{
			bestOldSatisfiedScore = Constants.NO_SCORE;
			bestNewSatisfiedScore = Constants.NO_SCORE;
			bestOldDissatisfiedScore = ( solveStrategyParameter == Constants.NO_SOLVE_STRATEGY_PARAMETER ? Constants.MAX_SCORE : Constants.NO_SCORE );
			bestNewDissatisfiedScore = bestOldDissatisfiedScore;
			bestOldNotBlockingScore = Constants.MAX_SCORE;
			bestNewNotBlockingScore = Constants.MAX_SCORE;
			bestOldBlockingScore = Constants.MAX_SCORE;
			bestNewBlockingScore = Constants.MAX_SCORE;

			bestActionSelectionItem = null;

			searchScoreItem = firstActiveScoreItem();

			while( searchScoreItem != null )
				{
				// Copy the checks
				searchScoreItem.isChecked = searchScoreItem.isMarked;
				// Clear all marked actions
				searchScoreItem.isMarked = false;
				searchScoreItem = searchScoreItem.nextScoreItem();
				}

			searchScoreItem = firstActiveScoreItem();

			// Search only for new scores of best actions
			while( searchScoreItem != null )
				{
				if( searchScoreItem.isChecked )
					{
					localOldSatisfiedScore = Constants.NO_SCORE;
					localNewSatisfiedScore = Constants.NO_SCORE;
					localOldDissatisfiedScore = Constants.NO_SCORE;
					localNewDissatisfiedScore = Constants.NO_SCORE;
					localOldNotBlockingScore = Constants.NO_SCORE;
					localNewNotBlockingScore = Constants.NO_SCORE;
					localOldBlockingScore = Constants.NO_SCORE;
					localNewBlockingScore = Constants.NO_SCORE;

					localScoreItem = firstActiveScoreItem();

					while( localScoreItem != null )
						{
						if( localScoreItem.referenceSelectionItem == searchScoreItem.referenceSelectionItem )
							{
							// Clear processed check
							localScoreItem.isChecked = false;

							// Don't add winning or losing scores
							if( localScoreItem.newSatisfiedScore != Constants.WINNING_SCORE &&
							localScoreItem.newDissatisfiedScore != Constants.WINNING_SCORE &&

							// Only add if solve strategy isn't EXCLUSIVE-OFFENSIVE or it has no old satisfied score and it has new satisfied score or it has no dissatisfied score
							( solveStrategyParameter != Constants.WORD_PARAMETER_ADJECTIVE_EXCLUSIVE ||

							( !localScoreItem.hasOldSatisfiedScore() &&
							localScoreItem.hasNewSatisfiedScore() ) ||

							( !localScoreItem.hasOldDissatisfiedScore() &&
							!localScoreItem.hasNewDissatisfiedScore() ) ) )
								{
								if( Constants.MAX_SCORE - localOldSatisfiedScore <= localScoreItem.oldSatisfiedScore )
									return startSelectionResultSystemError( 1, "Overflow of the old satisfied cummulate score" );

								if( Constants.MAX_SCORE - localNewSatisfiedScore <= localScoreItem.newSatisfiedScore )
									return startSelectionResultSystemError( 1, "Overflow of the new satisfied cummulate score" );

								if( Constants.MAX_SCORE - localOldDissatisfiedScore <= localScoreItem.oldDissatisfiedScore )
									return startSelectionResultSystemError( 1, "Overflow of the old dissatisfied cummulate score" );

								if( Constants.MAX_SCORE - localNewDissatisfiedScore <= localScoreItem.newDissatisfiedScore )
									return startSelectionResultSystemError( 1, "Overflow of the new dissatisfied cummulate score" );

								if( Constants.MAX_SCORE - localOldNotBlockingScore <= localScoreItem.oldNotBlockingScore )
									return startSelectionResultSystemError( 1, "Overflow of the old not-blocking cummulate score" );

								if( Constants.MAX_SCORE - localNewNotBlockingScore <= localScoreItem.newNotBlockingScore )
									return startSelectionResultSystemError( 1, "Overflow of the new not-blocking cummulate score" );

								if( Constants.MAX_SCORE - localOldBlockingScore <= localScoreItem.oldBlockingScore )
									return startSelectionResultSystemError( 1, "Overflow of the old blocking cummulate score" );

								if( Constants.MAX_SCORE - localNewBlockingScore <= localScoreItem.newBlockingScore )
									return startSelectionResultSystemError( 1, "Overflow of the new blocking cummulate score" );

								localOldSatisfiedScore += localScoreItem.oldSatisfiedScore;
								localNewSatisfiedScore += localScoreItem.newSatisfiedScore;
								localOldDissatisfiedScore += localScoreItem.oldDissatisfiedScore;
								localNewDissatisfiedScore += localScoreItem.newDissatisfiedScore;
								localOldNotBlockingScore += localScoreItem.oldNotBlockingScore;
								localNewNotBlockingScore += localScoreItem.newNotBlockingScore;
								localOldBlockingScore += localScoreItem.oldBlockingScore;
								localNewBlockingScore += localScoreItem.newBlockingScore;
								}
							}

						localScoreItem = localScoreItem.nextScoreItem();
						}

					if( ( boolResult = getBestScore( true, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) ).result != Constants.RESULT_OK )
						return addSelectionResultError( 1, "I failed to get the best local score" );

					// Has better score
					if( boolResult.booleanValue )
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

						if( searchScoreItem.referenceSelectionItem != bestActionSelectionItem )
							{
							if( bestActionSelectionItem != null &&
							// Previous best action
							disableAction( true, bestActionSelectionItem ) != Constants.RESULT_OK )
								return addSelectionResultError( 1, "I failed to disable the best action" );

							// Current action
							if( markAction( searchScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
								return addSelectionResultError( 1, "I failed to mark an action" );
							}

						bestActionSelectionItem = searchScoreItem.referenceSelectionItem;
						}
					else
						{
						if( searchScoreItem.referenceSelectionItem != bestActionSelectionItem )
							{
							if( hasEqualScore( localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) )
								{
								// Current action
								if( markAction( searchScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
									return addSelectionResultError( 1, "I failed to mark an action" );

								nRandomEntries++;
								}
							else
								{
								// Previous best action
								if( disableAction( false, searchScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
									return addSelectionResultError( 1, "I failed to disable an action" );
								}
							}
						}
					}

				searchScoreItem = searchScoreItem.nextScoreItem();
				}

			// Skip random during testing. It would create different test results
			if( !isCurrentlyTesting &&
			// Found more than one the same best cummulate score with different action
			nRandomEntries > 1 )
				{
				// More than one equal possibilities. So, use random
				nRandomEntries = (int)( nRandomEntries * Math.random() ) + 1;

				searchScoreItem = firstActiveScoreItem();

				while( searchScoreItem != null &&
				nRandomEntries > 0 )
					{
					if( searchScoreItem.isMarked )
						{
						if( --nRandomEntries > 0 )
							{
							localScoreItem = firstActiveScoreItem();

							while( localScoreItem != null )
								{
								if( localScoreItem.referenceSelectionItem == searchScoreItem.referenceSelectionItem )
									// Clear all marked actions
									localScoreItem.isMarked = false;

								localScoreItem = localScoreItem.nextScoreItem();
								}
							}
						else
							bestActionSelectionItem = searchScoreItem.referenceSelectionItem;
						}

					searchScoreItem = searchScoreItem.nextScoreItem();
					}
				}
			}

		selectionResult.selectionItem = bestActionSelectionItem;
		return selectionResult;
		}
	};

/*************************************************************************
 *	"All he does is just and good,
 *	and all his commandments are trustworthy." (Psalm 111:7)
 *************************************************************************/
