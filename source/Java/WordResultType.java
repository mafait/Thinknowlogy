/*
 *	Class:			WordResultType
 *	Purpose:		To return word variables of a method
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

class WordResultType
	{
	// Protected variables

	protected byte result;

	protected boolean hasFoundDifferentParameter;

	protected WordItem createdWordItem;
	protected WordItem foundWordItem;

	protected WordTypeItem createdWordTypeItem;
	protected WordTypeItem foundWordTypeItem;

	// Constructor / deconstructor

	WordResultType()
		{
		result = Constants.RESULT_OK;

		hasFoundDifferentParameter = false;

		createdWordItem = null;
		foundWordItem = null;

		createdWordTypeItem = null;
		foundWordTypeItem = null;
		}
	};

/*************************************************************************
 *	"I take joy in doing your will, my God,
 *	for your instructions are written on my heart." (Psalm 40:8)
 *************************************************************************/
