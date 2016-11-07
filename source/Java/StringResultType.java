/*	Class:		StringResultType
 *	Purpose:	To return string variables,
 *				as the result of a method call
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

class StringResultType
	{
	// Protected variables

	protected byte result;

	protected boolean hasFoundMatchingStrings;

	// Constructor

	StringResultType()
		{
		result = Constants.RESULT_OK;

		hasFoundMatchingStrings = false;
		}
	};

/*************************************************************************
 *	"Everywhere - from east to west -
 *	praise the name of the Lord.
 *	For the Lord is high above the nations;
 *	his glory is higher than the heavens." (Psalm 113:3-4)
 *************************************************************************/
