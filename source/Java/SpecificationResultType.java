/*
 *	Class:		SpecificationResultType
 *	Purpose:	To return specification item variables of a method
 *	Version:	Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

class SpecificationResultType
	{
	// Protected variables

	protected byte result;

	protected boolean isFirstRelatedSpecification;
	protected boolean isLastRelatedSpecification;

	protected short assignmentOrderNr;
	protected short assignmentParameter;
	protected short assumptionLevel;
	protected short combinedAssumptionLevel;

	protected JustificationItem createdJustificationItem;

	protected SpecificationItem adjustedQuestionSpecificationItem;
	protected SpecificationItem createdSpecificationItem;
	protected SpecificationItem foundSpecificationItem;
	protected SpecificationItem relatedSpecificationItem;
	protected SpecificationItem replacedAssignmentItem;

	protected WordItem compoundGeneralizationWordItem;

	// Constructor / deconstructor

	SpecificationResultType()
		{
		result = Constants.RESULT_OK;

		isFirstRelatedSpecification = false;
		isLastRelatedSpecification = false;

		assignmentOrderNr = Constants.NO_ORDER_NR;
		assignmentParameter = Constants.NO_ASSIGNMENT_PARAMETER;
		assumptionLevel = Constants.NO_ASSIGNMENT_LEVEL;
		combinedAssumptionLevel = Constants.NO_ASSIGNMENT_LEVEL;

		createdJustificationItem = null;

		adjustedQuestionSpecificationItem = null;
		createdSpecificationItem = null;
		foundSpecificationItem = null;
		relatedSpecificationItem = null;
		replacedAssignmentItem = null;

		compoundGeneralizationWordItem = null;
		}
	};

/*************************************************************************
 *	"Even when I walk
 *	through the darkest valley,
 *	I will not be afraid,
 *	for you are close beside me.
 *	Your rod and your staff
 *	protect and conform me." (Psalm 23:4)
 *************************************************************************/
