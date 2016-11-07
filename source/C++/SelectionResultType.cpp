/*	Class:		SelectionResultType
 *	Purpose:	To return selection variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2016r2 (Restyle)
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

#ifndef SELECTIONRESULTTYPE
#define SELECTIONRESULTTYPE 1
#include "Item.h"
// Some compilers need these class declarations
class SelectionItem;

class SelectionResultType
	{
	friend class AdminSelection;
	friend class AdminSolve;
	friend class Item;
	friend class List;
	friend class ScoreList;
	friend class SelectionList;
	friend class WordSelectionCondition;

	protected:
	// Protected variables

	ResultType result;

	bool hasFoundDuplicateSelection;
	bool isConditionSatisfied;

	unsigned int duplicateConditionSentenceNr;

	unsigned int oldSatisfiedScore;
	unsigned int newSatisfiedScore;
	unsigned int oldDissatisfiedScore;
	unsigned int newDissatisfiedScore;
	unsigned int oldNotBlockingScore;
	unsigned int newNotBlockingScore;
	unsigned int oldBlockingScore;
	unsigned int newBlockingScore;

	SelectionItem *bestActionItem;
	SelectionItem *firstExecutionItem;
	SelectionItem *lastCreatedSelectionItem;

	protected:
	// Constructor

	SelectionResultType()
		{
		result = RESULT_OK;

		hasFoundDuplicateSelection = false;
		isConditionSatisfied = false;

		duplicateConditionSentenceNr = NO_SENTENCE_NR;

		oldSatisfiedScore = NO_SCORE;
		newSatisfiedScore = NO_SCORE;
		oldDissatisfiedScore = NO_SCORE;
		newDissatisfiedScore = NO_SCORE;
		oldNotBlockingScore = NO_SCORE;
		newNotBlockingScore = NO_SCORE;
		oldBlockingScore = NO_SCORE;
		newBlockingScore = NO_SCORE;

		bestActionItem = NULL;
		firstExecutionItem = NULL;
		lastCreatedSelectionItem = NULL;
		}
	};
#endif

/*************************************************************************
 *	"The Lords protects them
 *	and keeps them alive.
 *	and rescues them from their enemies.
 *	The Lord nurses them when they are sick
 *	and restores them to health." (Psalm 41:2-3)
 *************************************************************************/
