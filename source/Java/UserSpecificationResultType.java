/*	Class:		UserSpecificationResultType
 *	Purpose:	To return user specification variables,
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

class UserSpecificationResultType
	{
	// Protected variables

	protected byte result;

	protected boolean isConfirmedExclusive;
	protected boolean isNonExclusiveSpecification;
	protected boolean isSameQuestionFromUser;
	protected boolean isSimilarOrRelatedQuestion;

	protected SpecificationItem confirmedReplacedSpecificationItem;
	protected SpecificationItem confirmedSpecificationButNoRelationReplacedSpecificationItem;

	// Constructor

	UserSpecificationResultType()
		{
		result = Constants.RESULT_OK;

		isConfirmedExclusive = false;
		isNonExclusiveSpecification = false;
		isSameQuestionFromUser = false;
		isSimilarOrRelatedQuestion = false;

		confirmedReplacedSpecificationItem = null;
		confirmedSpecificationButNoRelationReplacedSpecificationItem = null;
		}
	};

/*************************************************************************
 *	"The winds blows, and we are gone -
 *	as though we had never been here.
 *	But the love of the Lord remains forever
 *	with those who fear him." (Psalm 103:16-17)
 *************************************************************************/
