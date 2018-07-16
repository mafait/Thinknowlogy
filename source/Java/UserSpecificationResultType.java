/*	Class:		UserSpecificationResultType
 *	Purpose:	To return user specification variables,
 *				as the result of a method call
 *	Version:	Thinknowlogy 2018r2 (Natural Intelligence)
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

class UserSpecificationResultType
	{
	// Protected variables

	protected byte result;

	protected boolean isConfirmedExclusive;
	protected boolean isConfirmedSpecificationButNotItsRelation;
	protected boolean isNonExclusiveSpecification;
	protected boolean isSameQuestionFromUser;
	protected boolean isSimilarOrRelatedQuestion;

	protected SpecificationItem confirmedSpecificationItem;

	// Constructor

	UserSpecificationResultType()
		{
		result = Constants.RESULT_OK;

		isConfirmedExclusive = false;
		isConfirmedSpecificationButNotItsRelation = false;
		isNonExclusiveSpecification = false;
		isSameQuestionFromUser = false;
		isSimilarOrRelatedQuestion = false;

		confirmedSpecificationItem = null;
		}
	};

/*************************************************************************
 *	"The winds blows, and we are gone -
 *	as though we had never been here.
 *	But the love of the Lord remains forever
 *	with those who fear him." (Psalm 103:16-17)
 *************************************************************************/
