/*	Class:		ContextResultType
 *	Purpose:	To return context variables,
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

class ContextResultType
	{
	// Protected variables

	protected byte result;

	protected boolean isAmbiguousRelationContext;

	protected int contextNr;
	protected int copiedRelationContextNr;

	protected SpecificationItem confirmedAllRelationWordsOfSpecificationItem;

	// Constructor

	ContextResultType()
		{
		result = Constants.RESULT_OK;

		isAmbiguousRelationContext = false;

		contextNr = Constants.NO_CONTEXT_NR;
		copiedRelationContextNr = Constants.NO_CONTEXT_NR;

		confirmedAllRelationWordsOfSpecificationItem = null;
		}
	};

/*************************************************************************
 *	"The Lord is my shepherd;
 *	I have all that I need.
 *	He lets me rest in green meadows;
 *	he leads me beside peaceful steams." (Psalm 23:1-2)
 *************************************************************************/
