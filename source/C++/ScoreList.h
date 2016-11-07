/*	Class:			ScoreList
 *	Parent class:	List
 *	Purpose:		To temporarily store score items
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

// Score List header

#include "List.h"
#include "ScoreItem.cpp"
#include "SelectionResultType.cpp"

class ScoreList : private List
	{
	friend class AdminItem;
	friend class AdminSolve;
	friend class WordItem;

	// Private constructed variables

	bool hasBetterScore_;
	bool hasEqualScore_;
	bool hasFoundScore_;

	// Private functions

	ResultType markAction( SelectionItem *markSelectionReference );
	ResultType disableAction( bool isIncludingMarkedActions, SelectionItem *disableItem );
	ResultType getBestScore( bool isCummulative, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, unsigned int bestOldSatisfiedScore, unsigned int bestNewSatisfiedScore, unsigned int bestOldDissatisfiedScore, unsigned int bestNewDissatisfiedScore, unsigned int bestOldNotBlockingScore, unsigned int bestNewNotBlockingScore, unsigned int bestOldBlockingScore, unsigned int bestNewBlockingScore );

	ScoreItem *firstActiveScoreItem();
	ScoreItem *nextScoreListItem();


	protected:
	// Constructor

	ScoreList( CommonVariables *commonVariables, WordItem *myWordItem );
	~ScoreList();


	// Protected virtual functions

	virtual bool isTemporaryList();


	// Protected functions

	bool hasFoundScore();

	unsigned int nPossibilities();

	ResultType changeAction( SelectionItem *actionSelectionItem );
	ResultType checkSelectionItemForUsage( SelectionItem *unusedSelectionItem );
	ResultType findScore( bool isPreparingSort, SelectionItem *findScoreItem );

	ResultType deleteScores();

	ResultType checkScores( bool isInverted, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore );
	ResultType createScoreItem( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem );

	SelectionResultType getBestAction( bool isCurrentlyTesting, unsigned short solveStrategyParameter );

	ScoreItem *firstPossibility();
	};

/*************************************************************************
 *	"Those who look to him for help will be radiant with joy;
 *	no shadow of shame wil darken their faces." (Psalm 34:5)
 *************************************************************************/
