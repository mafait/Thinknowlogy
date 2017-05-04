/*	Class:		CreateAndAssignResultType
 *	Purpose:	To return specification variables,
 *				as the result of a method call
 *	Version:	Thinknowlogy 2017r1 (Bursts of Laughter)
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

class CreateAndAssignResultType
	{
	// Protected variables

	protected byte result;

	protected JustificationItem createdJustificationItem;

	protected SpecificationItem createdSpecificationItem;
	protected SpecificationItem foundSpecificationItem;
	protected SpecificationItem replacedAssignmentItem;

	// Constructor

	CreateAndAssignResultType()
		{
		result = Constants.RESULT_OK;

		createdJustificationItem = null;

		createdSpecificationItem = null;
		foundSpecificationItem = null;
		replacedAssignmentItem = null;
		}
	};

/*************************************************************************
 *	"Surely your goodness and unfailing love will pursue me
 *	all the days of my life,
 *	and I will live in the house of the Lord
 *	forever." (Psalm 23:6)
 *************************************************************************/