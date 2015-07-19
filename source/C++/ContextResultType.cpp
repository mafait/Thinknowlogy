/*
 *	Class:			ContextResultType
 *	Purpose:		To return context variables of a function
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

#include "Constants.h"
#include "SpecificationItem.cpp"

class ContextResultType
	{
	friend class AdminConclusion;
	friend class AdminContext;
	friend class AdminItem;
	friend class AdminReadSentence;
	friend class AdminSpecification;
	protected:
	// Protected variables

	ResultType result;

	bool isAmbiguousRelationContext;
	bool isExclusiveContext;

	unsigned int contextNr;

	SpecificationItem *conclusionSpecificationItem;

	protected:
	// Constructor / deconstructor

	ContextResultType()
		{
		result = RESULT_OK;

		isAmbiguousRelationContext = false;
		isExclusiveContext = false;

		contextNr = NO_CONTEXT_NR;

		conclusionSpecificationItem = NULL;
		}
	};

/*************************************************************************
 *	"The Lord is my shepherd;
 *	I've all that I need.
 *	He lets me rest in green meadows;
 *	he leads me beside peaceful steams." (Psalm 23:1-2)
 *************************************************************************/
