/*
 *	Class:			GeneralizationResultType
 *	Purpose:		To return generalization variables of a function
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

class GeneralizationResultType
	{
	friend class AdminCollection;
	friend class GeneralizationList;
	friend class WordItem;
	friend class WordSpecification;
	protected:
	// Protected variables

	ResultType result;
	bool hasFoundGeneralization;

	protected:
	// Constructor / deconstructor

	GeneralizationResultType()
		{
		result = RESULT_OK;
		hasFoundGeneralization = false;
		}
	};

/*************************************************************************
 *	"God has ascended with a mighty shout.
 *	The Lord has ascended with trumput blaring." (Psalm 47:5)
 *************************************************************************/
