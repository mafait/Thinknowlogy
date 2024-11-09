/*	Class:		ConditionResultType
 *	Purpose:	To return selection condition variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

#include "Constants.h"

class ConditionResultType
	{
	friend class AdminImperative;
	friend class Item;

	protected:
	// Protected variables

	signed char result = RESULT_OK;

	bool isConditionPartSatisfied = false;

	unsigned int oldSatisfiedScore = NO_SCORE;
	unsigned int newSatisfiedScore = NO_SCORE;
	unsigned int oldDissatisfiedScore = NO_SCORE;
	unsigned int newDissatisfiedScore = NO_SCORE;
	unsigned int oldNotBlockingScore = NO_SCORE;
	unsigned int newNotBlockingScore = NO_SCORE;
	unsigned int oldBlockingScore = NO_SCORE;
	unsigned int newBlockingScore = NO_SCORE;
	};

/*************************************************************************
 *	"The Lords protects them
 *	and keeps them alive.
 *	and rescues them from their enemies.
 *	The Lord nurses them when they are sick
 *	and restores them to health." (Psalm 41:2-3)
 *************************************************************************/
