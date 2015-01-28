/*
 *	Class:			ReferenceResultType
 *	Purpose:		To return string variables of a method
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

class ReferenceResultType
	{
	// Protected variables

	protected byte result;

	protected boolean hasFoundMatchingStrings;
	protected boolean hasFoundWordReference;

	// Constructor / deconstructor

	ReferenceResultType()
		{
		result = Constants.RESULT_OK;

		hasFoundMatchingStrings = false;
		hasFoundWordReference = false;
		}
	};

/*************************************************************************
 *	"Everywhere - from east to west -
 *	praise the name of the Lord.
 *	For the Lord is high above the nations;
 *	his glory is higher than the heavens." (Psalm 113:3-4)
 *************************************************************************/
