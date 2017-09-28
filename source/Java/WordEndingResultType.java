/*	Class:		WordEndingResultType
 *	Purpose:	To return word ending variables,
 *				as the result of a method call
 *	Version:	Thinknowlogy 2017r2 (Science as it should be)
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

class WordEndingResultType
	{
	// Protected variables

	protected byte result;

	protected boolean hasFoundWordEnding;

	protected int singularNounWordStringLength;

	protected String singularNounWordString;

	// Constructor

	WordEndingResultType()
		{
		result = Constants.RESULT_OK;

		hasFoundWordEnding = false;

		singularNounWordStringLength = 0;

		singularNounWordString = null;
		}
	};

/*************************************************************************
 *	"Let them praise your great and awesome name.
 *	Your name is holy!" (Psalm 99:3)
 *************************************************************************/
