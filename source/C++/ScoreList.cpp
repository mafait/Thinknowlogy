/*	Class:			ScoreList
 *	Parent class:	List
 *	Purpose:		To temporarily store score items
 *	Version:		Thinknowlogy 2018r1 (ShangDi 上帝)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

#include "List.h"
#include "ScoreItem.cpp"
#include "SelectionResultType.cpp"

class ScoreList : private List
	{
	friend class AdminItem;

	// Private functions

	void deleteScoreList( ScoreItem *searchScoreItem )
		{
		ScoreItem *deleteScoreItem;

		while( searchScoreItem != NULL )
			{
			deleteScoreItem = searchScoreItem;
			searchScoreItem = searchScoreItem->nextScoreItem();
			delete deleteScoreItem;
			}
		}

	bool hasEqualScore( unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, unsigned int bestOldSatisfiedScore, unsigned int bestNewSatisfiedScore, unsigned int bestOldDissatisfiedScore, unsigned int bestNewDissatisfiedScore, unsigned int bestOldNotBlockingScore, unsigned int bestNewNotBlockingScore, unsigned int bestOldBlockingScore, unsigned int bestNewBlockingScore )
		{
		bool isEqualSatisfiedScore = ( oldSatisfiedScore == bestOldSatisfiedScore &&
										newSatisfiedScore == bestNewSatisfiedScore );
		bool isEqualDissatisfiedScore = ( oldDissatisfiedScore == bestOldDissatisfiedScore &&
										newDissatisfiedScore == bestNewDissatisfiedScore );
		bool isEqualNotBlockingScore = ( oldNotBlockingScore == bestOldNotBlockingScore &&
										newNotBlockingScore == bestNewNotBlockingScore );
		bool isEqualBlockingScore = ( oldBlockingScore == bestOldBlockingScore &&
										newBlockingScore == bestNewBlockingScore );

		return ( isEqualSatisfiedScore &&
				isEqualDissatisfiedScore &&
				isEqualNotBlockingScore &&
				isEqualBlockingScore );
		}

	signed char markAction( SelectionItem *markSelectionReference )
		{
		ScoreItem *searchScoreItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "MarkAction";

		if( markSelectionReference == NULL )
			return startError( functionNameString, "The given score item is undefined" );

		while( searchScoreItem != NULL )
			{
			if( searchScoreItem->referenceSelectionItem == markSelectionReference )
				// Mark action
				searchScoreItem->isMarked = true;

			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		return RESULT_OK;
		}

	signed char disableAction( bool isIncludingMarkedActions, SelectionItem *disableItem )
		{
		ScoreItem *searchScoreItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "DisableAction";

		if( disableItem == NULL )
			return startError( functionNameString, "The given score item is undefined" );

		while( searchScoreItem != NULL )
			{
			if( ( isIncludingMarkedActions &&
			searchScoreItem->isMarked ) ||
			searchScoreItem->referenceSelectionItem == disableItem )
				{
				// Clear action
				searchScoreItem->isMarked = false;
				// Clear check
				searchScoreItem->isChecked = false;
				}

			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		return RESULT_OK;
		}

	ScoreItem *firstActiveScoreItem()
		{
		return (ScoreItem *)firstActiveItem();
		}

	ScoreItem *nextScoreListItem()
		{
		return (ScoreItem *)nextListItem();
		}

	BoolResultType getBestScore( bool isCummulative, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, unsigned int bestOldSatisfiedScore, unsigned int bestNewSatisfiedScore, unsigned int bestOldDissatisfiedScore, unsigned int bestNewDissatisfiedScore, unsigned int bestOldNotBlockingScore, unsigned int bestNewNotBlockingScore, unsigned int bestOldBlockingScore, unsigned int bestNewBlockingScore )
		{
		bool isEqualSatisfiedScore = ( oldSatisfiedScore == bestOldSatisfiedScore &&
										newSatisfiedScore == bestNewSatisfiedScore );
		bool isEqualDissatisfiedScore = ( oldDissatisfiedScore == bestOldDissatisfiedScore &&
										newDissatisfiedScore == bestNewDissatisfiedScore );
		bool isEqualBlockingScore = ( oldBlockingScore == bestOldBlockingScore &&
										newBlockingScore == bestNewBlockingScore );

		double satisfiedScore = (double)oldSatisfiedScore + (double)newSatisfiedScore;
		double dissatisfiedScore = (double)oldDissatisfiedScore + (double)newDissatisfiedScore;
		double notBlockingScore = (double)oldNotBlockingScore + (double)newNotBlockingScore;
		double blockingScore = (double)oldBlockingScore + (double)newBlockingScore;

		double bestSatisfiedScore = (double)bestOldSatisfiedScore + (double)bestNewSatisfiedScore;
		double bestDissatisfiedScore = (double)bestOldDissatisfiedScore + (double)bestNewDissatisfiedScore;
		double bestNotBlockingScore = (double)bestOldNotBlockingScore + (double)bestNewNotBlockingScore;
		double bestBlockingScore = (double)bestOldBlockingScore + (double)bestNewBlockingScore;

		bool isHigherSatisfiedScore = ( satisfiedScore > bestSatisfiedScore );
		bool isSuperiorSatisfiedScore = ( isHigherSatisfiedScore &&
										satisfiedScore > bestDissatisfiedScore );
		bool isLowerDissatisfiedScore = ( dissatisfiedScore < bestDissatisfiedScore );
		bool isHigherDissatisfiedScore = ( dissatisfiedScore > bestDissatisfiedScore );
		bool isLowerNotBlockingScore = ( notBlockingScore < bestNotBlockingScore );
		bool isLowerBlockingScore = ( blockingScore < bestBlockingScore );
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getBestScore";

		if( solveStrategyParameter != NO_SOLVE_STRATEGY_PARAMETER &&
		solveStrategyParameter != WORD_PARAMETER_ADJECTIVE_DEFENSIVE &&
		solveStrategyParameter != WORD_PARAMETER_ADJECTIVE_EXCLUSIVE )
			return startBoolResultError( functionNameString, "The given solve strategy parameter is invalid" );

		// Has better score
		boolResult.booleanValue =	( ( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER &&
									( isHigherSatisfiedScore ||
									( isEqualSatisfiedScore &&
									isLowerDissatisfiedScore ) ) ) ||

								( solveStrategyParameter == WORD_PARAMETER_ADJECTIVE_DEFENSIVE &&
									( isLowerDissatisfiedScore ||
									( isEqualDissatisfiedScore &&
									isHigherSatisfiedScore ) ) ) ||

								( solveStrategyParameter == WORD_PARAMETER_ADJECTIVE_EXCLUSIVE &&
									// Has no dissatisfied score and superior satisfied score
									( ( isSuperiorSatisfiedScore &&
									( isCummulative ||
									dissatisfiedScore == NO_SCORE ) ) ||

									// Has no old satisfied score and has new satisfied score and higher dissatisfied score
									( isHigherDissatisfiedScore &&
									( isCummulative ||
									( oldSatisfiedScore == NO_SCORE &&
									newSatisfiedScore > NO_SCORE ) ) ) ) ) ||

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

	ScoreList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		initializeListVariables( ADMIN_SCORE_LIST_SYMBOL, "ScoreList", globalVariables, inputOutput, myWordItem );
		}

	~ScoreList()
		{
		deleteScoreList( firstActiveScoreItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class ScoreList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class ScoreList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class ScoreList has replaced items." );

		deleteScoreList( (ScoreItem *)firstDeletedItem() );
		}


	// Protected virtual functions

	bool isTemporaryList()
		{
		return true;
		}


	// Protected functions

	unsigned int nPossibilities()
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		unsigned int nItems = 0;
		ScoreItem *searchScoreItem = firstActiveScoreItem();

		while( searchScoreItem != NULL &&
		searchScoreItem->assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchScoreItem->assignmentLevel() == currentAssignmentLevel )
				nItems++;

			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		return nItems;
		}

	signed char changeAction( SelectionItem *actionSelectionItem )
		{
		ScoreItem *searchScoreItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changeAction";

		if( actionSelectionItem == NULL )
			return startError( functionNameString, "The given action selection item is undefined" );

		while( searchScoreItem != NULL )
			{
			// All new created scores with assignment level higher than zero
			if( searchScoreItem->isChecked )
				{
				searchScoreItem->isChecked = false;
				searchScoreItem->referenceSelectionItem = actionSelectionItem;
				}

			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		return RESULT_OK;
		}

	signed char createScoreItem( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createScoreItem";

		if( addItemToList( QUERY_ACTIVE_CHAR, new ScoreItem( isChecked, globalVariables()->currentAssignmentLevel, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add an active score item" );

		return RESULT_OK;
		}

	signed char deleteScores()
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		ScoreItem *searchScoreItem = firstActiveScoreItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteScores";

		while( searchScoreItem != NULL &&
		searchScoreItem->assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchScoreItem->assignmentLevel() == currentAssignmentLevel )
				{
				if( deleteItem( searchScoreItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to delete an active item" );

				searchScoreItem = nextScoreListItem();
				}
			else
				searchScoreItem = searchScoreItem->nextScoreItem();
			}

		return RESULT_OK;
		}

	ScoreItem *firstPossibility()
		{
		ScoreItem *firstScoreItem;

		if( ( firstScoreItem = firstActiveScoreItem() ) != NULL )
			return firstScoreItem->firstPossibilityScoreItem();

		return NULL;
		}

	BoolResultType checkScores( bool isInverted, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore )
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		unsigned int checkOldSatisfiedScore = ( isInverted ? oldDissatisfiedScore : oldSatisfiedScore );
		unsigned int checkNewSatisfiedScore = ( isInverted ? newDissatisfiedScore : newSatisfiedScore );
		unsigned int checkOldDissatisfiedScore = ( isInverted ? oldSatisfiedScore : oldDissatisfiedScore );
		unsigned int checkNewDissatisfiedScore = ( isInverted ? newSatisfiedScore : newDissatisfiedScore );
		ScoreItem *searchScoreItem = firstActiveScoreItem();
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkScores";

		if( checkOldSatisfiedScore <= NO_SCORE &&
		checkNewSatisfiedScore <= NO_SCORE &&
		checkOldDissatisfiedScore <= NO_SCORE &&
		checkNewDissatisfiedScore <= NO_SCORE &&

		oldNotBlockingScore <= NO_SCORE &&
		newNotBlockingScore <= NO_SCORE &&
		oldBlockingScore <= NO_SCORE &&
		newBlockingScore <= NO_SCORE )
			return startBoolResultError( functionNameString, "None of the given scores has a value parameter" );

		while( searchScoreItem != NULL &&
		searchScoreItem->assignmentLevel() >= currentAssignmentLevel )
			{
			if( searchScoreItem->assignmentLevel() == currentAssignmentLevel )
				{
				// All new created (=empty) scores
				if( !searchScoreItem->hasOldSatisfiedScore() &&
				!searchScoreItem->hasNewSatisfiedScore() &&
				!searchScoreItem->hasOldDissatisfiedScore() &&
				!searchScoreItem->hasNewDissatisfiedScore() &&
				!searchScoreItem->hasOldNotBlockingScore() &&
				!searchScoreItem->hasNewNotBlockingScore() &&
				!searchScoreItem->hasOldBlockingScore() &&
				!searchScoreItem->hasNewBlockingScore() )
					{
					// Has found score
					boolReturnResult.booleanValue = true;

					searchScoreItem->oldSatisfiedScore = checkOldSatisfiedScore;
					searchScoreItem->newSatisfiedScore = checkNewSatisfiedScore;
					searchScoreItem->oldDissatisfiedScore = checkOldDissatisfiedScore;
					searchScoreItem->newDissatisfiedScore = checkNewDissatisfiedScore;
					searchScoreItem->oldNotBlockingScore = oldNotBlockingScore;
					searchScoreItem->newNotBlockingScore = newNotBlockingScore;
					searchScoreItem->oldBlockingScore = oldBlockingScore;
					searchScoreItem->newBlockingScore = newBlockingScore;
					}
				else
					{
					if( searchScoreItem->isMarked )
						{
						if( ( boolCheckResult = getBestScore( false, solveStrategyParameter, checkOldSatisfiedScore, checkNewSatisfiedScore, checkOldDissatisfiedScore, checkNewDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, searchScoreItem->oldSatisfiedScore, searchScoreItem->newSatisfiedScore, searchScoreItem->oldDissatisfiedScore, searchScoreItem->newDissatisfiedScore, searchScoreItem->oldNotBlockingScore, searchScoreItem->newNotBlockingScore, searchScoreItem->oldBlockingScore, searchScoreItem->newBlockingScore ) ).result != RESULT_OK )
							return addBoolResultError( functionNameString, "I failed to get the best score" );

						// Has better score
						if( boolCheckResult.booleanValue )
							{
							searchScoreItem->oldSatisfiedScore = checkOldSatisfiedScore;
							searchScoreItem->newSatisfiedScore = checkNewSatisfiedScore;
							searchScoreItem->oldDissatisfiedScore = checkOldDissatisfiedScore;
							searchScoreItem->newDissatisfiedScore = checkNewDissatisfiedScore;
							searchScoreItem->oldNotBlockingScore = oldNotBlockingScore;
							searchScoreItem->newNotBlockingScore = newNotBlockingScore;
							searchScoreItem->oldBlockingScore = oldBlockingScore;
							searchScoreItem->newBlockingScore = newBlockingScore;
							}

						// Has found score
						boolReturnResult.booleanValue = true;
						}
					}
				}

			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		return boolReturnResult;
		}

	BoolResultType findScore( bool isPreparingSort, SelectionItem *findScoreItem )
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		ScoreItem *searchScoreItem = firstActiveScoreItem();
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findScore";

		if( findScoreItem == NULL )
			return startBoolResultError( functionNameString, "The given score item is undefined" );

		while( searchScoreItem != NULL &&
		searchScoreItem->assignmentLevel() >= currentAssignmentLevel )
			{
			if( ( currentAssignmentLevel == NO_ASSIGNMENT_LEVEL ||
			searchScoreItem->isChecked ) &&

			searchScoreItem->assignmentLevel() == currentAssignmentLevel &&
			searchScoreItem->referenceSelectionItem == findScoreItem )
				{
				// Has found score
				boolResult.booleanValue = true;
				searchScoreItem->isMarked = isPreparingSort;
				}

			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		return boolResult;
		}

	SelectionResultType getBestSelection( bool isCurrentlyTesting, unsigned short solveStrategyParameter )
		{
		bool hasBetterScore = false;
		bool isCummulate = false;
		unsigned int bestOldSatisfiedScore = NO_SCORE;
		unsigned int bestNewSatisfiedScore = NO_SCORE;
		unsigned int bestOldDissatisfiedScore = ( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER ? MAX_SCORE : NO_SCORE );
		unsigned int bestNewDissatisfiedScore = bestOldDissatisfiedScore;
		unsigned int bestOldNotBlockingScore = MAX_SCORE;
		unsigned int bestNewNotBlockingScore = MAX_SCORE;
		unsigned int bestOldBlockingScore = MAX_SCORE;
		unsigned int bestNewBlockingScore = MAX_SCORE;
		unsigned int localOldSatisfiedScore;
		unsigned int localNewSatisfiedScore;
		unsigned int localOldDissatisfiedScore;
		unsigned int localNewDissatisfiedScore;
		unsigned int localOldNotBlockingScore;
		unsigned int localNewNotBlockingScore;
		unsigned int localOldBlockingScore;
		unsigned int localNewBlockingScore;
		unsigned int nBestLosingScores = MAX_NUMBER;
		unsigned int nBestWinningScores = 0;
		unsigned int nLocalLosingScores;
		unsigned int nLocalWinningScores;
		unsigned int nRandomEntries = 0;
		ScoreItem *localScoreItem;
		ScoreItem *searchScoreItem = firstActiveScoreItem();
		SelectionItem *bestActionSelectionItem = NULL;
		BoolResultType boolResult;
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getBestSelection";

		while( searchScoreItem != NULL )
			{
			// Clear all marked actions
			searchScoreItem->isMarked = false;
			// Set all checks
			searchScoreItem->isChecked = true;
			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		searchScoreItem = firstActiveScoreItem();

		while( searchScoreItem != NULL )
			{
			if( searchScoreItem->isChecked )
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
				localScoreItem = firstActiveScoreItem();

				while( localScoreItem != NULL )
					{
					if( localScoreItem->referenceSelectionItem == searchScoreItem->referenceSelectionItem )
						{
						// Already processed
						localScoreItem->isChecked = false;

						if( localScoreItem->newSatisfiedScore == WINNING_SCORE )
							nLocalWinningScores++;
						else
							{
							if( localScoreItem->newDissatisfiedScore == WINNING_SCORE )
								nLocalLosingScores++;
							else
								{
								if( ( boolResult = getBestScore( false, solveStrategyParameter, localScoreItem->oldSatisfiedScore, localScoreItem->newSatisfiedScore, localScoreItem->oldDissatisfiedScore, localScoreItem->newDissatisfiedScore, localScoreItem->oldNotBlockingScore, localScoreItem->newNotBlockingScore, localScoreItem->oldBlockingScore, localScoreItem->newBlockingScore, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore ) ).result != RESULT_OK )
									return addSelectionResultError( functionNameString, "I failed to get the best local score" );

								// Has better score
								if( boolResult.booleanValue )
									{
									localOldSatisfiedScore = localScoreItem->oldSatisfiedScore;
									localNewSatisfiedScore = localScoreItem->newSatisfiedScore;
									localOldDissatisfiedScore = localScoreItem->oldDissatisfiedScore;
									localNewDissatisfiedScore = localScoreItem->newDissatisfiedScore;
									localOldNotBlockingScore = localScoreItem->oldNotBlockingScore;
									localNewNotBlockingScore = localScoreItem->newNotBlockingScore;
									localOldBlockingScore = localScoreItem->oldBlockingScore;
									localNewBlockingScore = localScoreItem->newBlockingScore;
									}
								}
							}
						}

					localScoreItem = localScoreItem->nextScoreItem();
					}

				if( nLocalWinningScores == nBestWinningScores &&
				nLocalLosingScores == nBestLosingScores )
					{
					if( ( boolResult = getBestScore( false, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) ).result != RESULT_OK )
						return addSelectionResultError( functionNameString, "I failed to get the best score" );

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

					if( searchScoreItem->referenceSelectionItem != bestActionSelectionItem )
						{
						if( bestActionSelectionItem != NULL &&
						// Previous best action
						disableAction( true, bestActionSelectionItem ) != RESULT_OK )
							return addSelectionResultError( functionNameString, "I failed to disable the best action" );

						// Current action
						if( markAction( searchScoreItem->referenceSelectionItem ) != RESULT_OK )
							return addSelectionResultError( functionNameString, "I failed to mark an action" );
						}

					bestActionSelectionItem = searchScoreItem->referenceSelectionItem;
					}
				else
					{
					if( searchScoreItem->referenceSelectionItem != bestActionSelectionItem )
						{
						if( hasEqualScore( localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) )
							{
							// Current action
							if( markAction( searchScoreItem->referenceSelectionItem ) != RESULT_OK )
								return addSelectionResultError( functionNameString, "I failed to mark an action" );

							// Found the same best score with different action
							isCummulate = true;
							}
						else
							{
							// Previous best action
							if( disableAction( false, searchScoreItem->referenceSelectionItem ) != RESULT_OK )
								return addSelectionResultError( functionNameString, "I failed to disable an action" );
							}
						}
					}
				}

			searchScoreItem = searchScoreItem->nextScoreItem();
			}

		// Found the same best score with different action
		if( isCummulate )
			{
			bestOldSatisfiedScore = NO_SCORE;
			bestNewSatisfiedScore = NO_SCORE;
			bestOldDissatisfiedScore = ( solveStrategyParameter == NO_SOLVE_STRATEGY_PARAMETER ? MAX_SCORE : NO_SCORE );
			bestNewDissatisfiedScore = bestOldDissatisfiedScore;
			bestOldNotBlockingScore = MAX_SCORE;
			bestNewNotBlockingScore = MAX_SCORE;
			bestOldBlockingScore = MAX_SCORE;
			bestNewBlockingScore = MAX_SCORE;

			bestActionSelectionItem = NULL;

			searchScoreItem = firstActiveScoreItem();

			while( searchScoreItem != NULL )
				{
				// Copy the checks
				searchScoreItem->isChecked = searchScoreItem->isMarked;
				// Clear all marked actions
				searchScoreItem->isMarked = false;
				searchScoreItem = searchScoreItem->nextScoreItem();
				}

			searchScoreItem = firstActiveScoreItem();

			// Search only for new scores of best actions
			while( searchScoreItem != NULL )
				{
				if( searchScoreItem->isChecked )
					{
					localOldSatisfiedScore = NO_SCORE;
					localNewSatisfiedScore = NO_SCORE;
					localOldDissatisfiedScore = NO_SCORE;
					localNewDissatisfiedScore = NO_SCORE;
					localOldNotBlockingScore = NO_SCORE;
					localNewNotBlockingScore = NO_SCORE;
					localOldBlockingScore = NO_SCORE;
					localNewBlockingScore = NO_SCORE;

					localScoreItem = firstActiveScoreItem();

					while( localScoreItem != NULL )
						{
						if( localScoreItem->referenceSelectionItem == searchScoreItem->referenceSelectionItem )
							{
							// Clear processed check
							localScoreItem->isChecked = false;

							// Don't add winning or losing scores
							if( localScoreItem->newSatisfiedScore != WINNING_SCORE &&
							localScoreItem->newDissatisfiedScore != WINNING_SCORE &&

							// Only add if solve strategy isn't EXCLUSIVE-OFFENSIVE or it has no old satisfied score and it has new satisfied score or it has no dissatisfied score
							( solveStrategyParameter != WORD_PARAMETER_ADJECTIVE_EXCLUSIVE ||

							( !localScoreItem->hasOldSatisfiedScore() &&
							localScoreItem->hasNewSatisfiedScore() ) ||

							( !localScoreItem->hasOldDissatisfiedScore() &&
							!localScoreItem->hasNewDissatisfiedScore() ) ) )
								{
								if( MAX_SCORE - localOldSatisfiedScore <= localScoreItem->oldSatisfiedScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the old satisfied cummulate score" );

								if( MAX_SCORE - localNewSatisfiedScore <= localScoreItem->newSatisfiedScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the new satisfied cummulate score" );

								if( MAX_SCORE - localOldDissatisfiedScore <= localScoreItem->oldDissatisfiedScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the old dissatisfied cummulate score" );

								if( MAX_SCORE - localNewDissatisfiedScore <= localScoreItem->newDissatisfiedScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the new dissatisfied cummulate score" );

								if( MAX_SCORE - localOldNotBlockingScore <= localScoreItem->oldNotBlockingScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the old not-blocking cummulate score" );

								if( MAX_SCORE - localNewNotBlockingScore <= localScoreItem->newNotBlockingScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the new not-blocking cummulate score" );

								if( MAX_SCORE - localOldBlockingScore <= localScoreItem->oldBlockingScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the old blocking cummulate score" );

								if( MAX_SCORE - localNewBlockingScore <= localScoreItem->newBlockingScore )
									return startSelectionResultSystemError( functionNameString, "Overflow of the new blocking cummulate score" );

								localOldSatisfiedScore += localScoreItem->oldSatisfiedScore;
								localNewSatisfiedScore += localScoreItem->newSatisfiedScore;
								localOldDissatisfiedScore += localScoreItem->oldDissatisfiedScore;
								localNewDissatisfiedScore += localScoreItem->newDissatisfiedScore;
								localOldNotBlockingScore += localScoreItem->oldNotBlockingScore;
								localNewNotBlockingScore += localScoreItem->newNotBlockingScore;
								localOldBlockingScore += localScoreItem->oldBlockingScore;
								localNewBlockingScore += localScoreItem->newBlockingScore;
								}
							}

						localScoreItem = localScoreItem->nextScoreItem();
						}

					if( ( boolResult = getBestScore( true, solveStrategyParameter, localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) ).result != RESULT_OK )
						return addSelectionResultError( functionNameString, "I failed to get the best local score" );

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

						if( searchScoreItem->referenceSelectionItem != bestActionSelectionItem )
							{
							if( bestActionSelectionItem != NULL &&
							// Previous best action
							disableAction( true, bestActionSelectionItem ) != RESULT_OK )
								return addSelectionResultError( functionNameString, "I failed to disable the best action" );

							// Current action
							if( markAction( searchScoreItem->referenceSelectionItem ) != RESULT_OK )
								return addSelectionResultError( functionNameString, "I failed to mark an action" );
							}

						bestActionSelectionItem = searchScoreItem->referenceSelectionItem;
						}
					else
						{
						if( searchScoreItem->referenceSelectionItem != bestActionSelectionItem )
							{
							if( hasEqualScore( localOldSatisfiedScore, localNewSatisfiedScore, localOldDissatisfiedScore, localNewDissatisfiedScore, localOldNotBlockingScore, localNewNotBlockingScore, localOldBlockingScore, localNewBlockingScore, bestOldSatisfiedScore, bestNewSatisfiedScore, bestOldDissatisfiedScore, bestNewDissatisfiedScore, bestOldNotBlockingScore, bestNewNotBlockingScore, bestOldBlockingScore, bestNewBlockingScore ) )
								{
								// Current action
								if( markAction( searchScoreItem->referenceSelectionItem ) != RESULT_OK )
									return addSelectionResultError( functionNameString, "I failed to mark an action" );

								nRandomEntries++;
								}
							else
								{
								// Previous best action
								if( disableAction( false, searchScoreItem->referenceSelectionItem ) != RESULT_OK )
									return addSelectionResultError( functionNameString, "I failed to disable an action" );
								}
							}
						}
					}

				searchScoreItem = searchScoreItem->nextScoreItem();
				}

			// Skip random during testing. It would create different test results
			if( !isCurrentlyTesting &&
			// Found more than one the same best cummulate score with different action
			nRandomEntries > 1 )
				{
				// More than one equal possibilities. So, use random
				nRandomEntries = ( rand() % nRandomEntries ) + 1;

				searchScoreItem = firstActiveScoreItem();

				while( searchScoreItem != NULL &&
				nRandomEntries > 0 )
					{
					if( searchScoreItem->isMarked )
						{
						if( --nRandomEntries > 0 )
							{
							localScoreItem = firstActiveScoreItem();

							while( localScoreItem != NULL )
								{
								if( localScoreItem->referenceSelectionItem == searchScoreItem->referenceSelectionItem )
									// Clear all marked actions
									localScoreItem->isMarked = false;

								localScoreItem = localScoreItem->nextScoreItem();
								}
							}
						else
							bestActionSelectionItem = searchScoreItem->referenceSelectionItem;
						}

					searchScoreItem = searchScoreItem->nextScoreItem();
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
