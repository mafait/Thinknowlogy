/*	Class:		SpecificationResultType
 *	Purpose:	To return specification variables,
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

#include "Item.h"
#include "JustificationItem.h"

class SpecificationResultType
	{
	friend class AdminAssumption;
	friend class AdminAuthorization;
	friend class AdminConclusion;
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminLanguage;
	friend class AdminReadFile;
	friend class AdminSolve;
	friend class AdminSpecification;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class List;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordAssignment;
	friend class WordItem;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordWrite;
	friend class WordWriteWords;

	protected:
	// Protected variables

	ResultType result;

	bool isFirstRelatedSpecification;
	bool isLastRelatedSpecification;

	unsigned short assignmentOrderNr;
	unsigned short assignmentParameter;
	unsigned short assumptionLevel;
	unsigned short combinedAssumptionLevel;

	JustificationItem *createdJustificationItem;

	SpecificationItem *adjustedQuestionSpecificationItem;
	SpecificationItem *createdSpecificationItem;
	SpecificationItem *foundSpecificationItem;
	SpecificationItem *relatedSpecificationItem;
	SpecificationItem *replacedAssignmentItem;

	WordItem *compoundGeneralizationWordItem;

	protected:
	// Constructor

	SpecificationResultType()
		{
		result = RESULT_OK;

		isFirstRelatedSpecification = false;
		isLastRelatedSpecification = false;

		assignmentOrderNr = NO_ORDER_NR;
		assignmentParameter = NO_ASSIGNMENT_PARAMETER;
		assumptionLevel = NO_ASSIGNMENT_LEVEL;
		combinedAssumptionLevel = NO_ASSIGNMENT_LEVEL;

		createdJustificationItem = NULL;

		adjustedQuestionSpecificationItem = NULL;
		createdSpecificationItem = NULL;
		foundSpecificationItem = NULL;
		relatedSpecificationItem = NULL;
		replacedAssignmentItem = NULL;

		compoundGeneralizationWordItem = NULL;
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
