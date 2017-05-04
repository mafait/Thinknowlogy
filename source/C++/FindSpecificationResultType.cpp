/*	Class:		FindSpecificationResultType
 *	Purpose:	To return possessive specification variables,
 *				as the result of a function call
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

#include "WordItem.h"

class FindSpecificationResultType
	{
	friend class AdminReasoningOld;
	friend class Item;

	protected:
	// Protected variables

	signed char result;

	SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem;
	SpecificationItem *foundPossessiveDefinitionSpecificationItem;

	protected:
	// Constructor

	FindSpecificationResultType()
		{
		result = RESULT_OK;

		foundOppositePossessiveDefinitionSpecificationItem = NULL;
		foundPossessiveDefinitionSpecificationItem = NULL;
		}
	};

/*************************************************************************
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *************************************************************************/
