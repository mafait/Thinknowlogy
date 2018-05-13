/*	Class:		ConditionResultType
 *	Purpose:	To return selection condition variables,
 *				as the result of a method call
 *	Version:	Thinknowlogy 2018r1 (ShangDi 上帝)
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

class ConditionResultType
	{
	// Protected variables

	protected byte result;

	protected boolean isConditionPartSatisfied;

	protected int oldSatisfiedScore;
	protected int newSatisfiedScore;
	protected int oldDissatisfiedScore;
	protected int newDissatisfiedScore;
	protected int oldNotBlockingScore;
	protected int newNotBlockingScore;
	protected int oldBlockingScore;
	protected int newBlockingScore;

	// Constructor

	ConditionResultType()
		{
		result = Constants.RESULT_OK;

		isConditionPartSatisfied = false;

		oldSatisfiedScore = Constants.NO_SCORE;
		newSatisfiedScore = Constants.NO_SCORE;
		oldDissatisfiedScore = Constants.NO_SCORE;
		newDissatisfiedScore = Constants.NO_SCORE;
		oldNotBlockingScore = Constants.NO_SCORE;
		newNotBlockingScore = Constants.NO_SCORE;
		oldBlockingScore = Constants.NO_SCORE;
		newBlockingScore = Constants.NO_SCORE;
		}
	};

/*************************************************************************
 *	"The Lords protects them
 *	and keeps them alive.
 *	and rescues them from their enemies.
 *	The Lord nurses them when they are sick
 *	and restores them to health." (Psalm 41:2-3)
 *************************************************************************/
