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

#ifndef SCORELIST
#define SCORELIST 1

#include "List.h"
#include "ScoreItem.cpp"
#include "SelectionResultType.cpp"

class ScoreList : private List
	{
	friend class AdminItem;
	friend class AdminSolve;
	friend class WordItem;

	// Private constructible variables

	bool hasBetterScore_;
	bool hasEqualScore_;
	bool hasFoundScore_;

	// Private functions

	ResultType markAction( SelectionItem *markSelectionReference );
	ResultType disableAction( bool isIncludingMarkedActions, SelectionItem *disableItem );
	ResultType getBestScore( bool cummulative, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, unsigned int bestOldSatisfiedScore, unsigned int bestNewSatisfiedScore, unsigned int bestOldDissatisfiedScore, unsigned int bestNewDissatisfiedScore, unsigned int bestOldNotBlockingScore, unsigned int bestNewNotBlockingScore, unsigned int bestOldBlockingScore, unsigned int bestNewBlockingScore );

	ScoreItem *firstActiveScoreItem();
	ScoreItem *nextScoreListItem();


	protected:
	// Constructor / deconstructor

	ScoreList( CommonVariables *commonVariables, WordItem *myWordItem );
	~ScoreList();


	// Protected virtual functions

	virtual bool isTemporaryList();


	// Protected functions

	bool hasFoundScore();

	unsigned int nPossibilities();

	ResultType changeAction( SelectionItem *actionSelectionItem );
	ResultType checkSelectionItemForUsage( SelectionItem *unusedSelectionItem );
	ResultType findScore( bool prepareSort, SelectionItem *findScoreItem );

	ResultType deleteScores();

	ResultType checkScores( bool isInverted, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore );
	ResultType createScoreItem( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem );

	SelectionResultType getBestAction( unsigned short solveStrategyParameter );

	ScoreItem *firstPossibility();
	};
#endif

/*************************************************************************
 *	"Those who look to him for help will be radiant with joy;
 *	no shadow of shame wil darken their faces." (Psalm 34:5)
 *************************************************************************/
