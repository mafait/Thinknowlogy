/*
 *	Class:			JustificationResultType
 *	Purpose:		To return justification variables of a function
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
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

#include "Item.h"
// Class declarations needed by some compilers
class JustificationItem;

class JustificationResultType
	{
	friend class AdminAssumption;
	friend class AdminReasoning;
	friend class JustificationList;
	friend class SpecificationList;
	friend class WordItem;
	protected:
	// Protected variables

	ResultType result;
	unsigned short foundOrderNr;

	JustificationItem *createdJustificationItem;
	JustificationItem *foundJustificationItem;

	protected:
	// Constructor / deconstructor

	JustificationResultType()
		{
		result = RESULT_OK;

		foundOrderNr = NO_ORDER_NR;

		createdJustificationItem = NULL;
		foundJustificationItem = NULL;
		}
	};

/*************************************************************************
 *	"He renews my strength.
 *	He guides me along right paths,
 *	bringing honor to his name." (Psalm 23:3)
 *************************************************************************/
