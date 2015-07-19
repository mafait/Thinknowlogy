/*
 *	Class:			ContextResultType
 *	Purpose:		To return context variables of a method
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

class ContextResultType
	{
	// Protected variables

	protected byte result;

	protected boolean isAmbiguousRelationContext;
	protected boolean isExclusiveContext;

	protected int contextNr;

	protected SpecificationItem conclusionSpecificationItem;

	// Constructor / deconstructor

	ContextResultType()
		{
		result = Constants.RESULT_OK;

		isAmbiguousRelationContext = false;
		isExclusiveContext = false;

		contextNr = Constants.NO_CONTEXT_NR;

		conclusionSpecificationItem = null;
		}
	};

/*************************************************************************
 *	"The Lord is my shepherd;
 *	I've all that I need.
 *	He lets me rest in green meadows;
 *	he leads me beside peaceful steams." (Psalm 23:1-2)
 *************************************************************************/
