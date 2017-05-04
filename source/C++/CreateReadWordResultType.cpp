/*	Class:		CreateReadWordResultType
 *	Purpose:	To return read word variables,
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

#include "Item.h"

class CreateReadWordResultType
	{
	friend class AdminReadCreateWords;
	friend class Item;

	protected:
	// Protected variables

	signed char result;

	bool hasCreatedReadWord;

	size_t nextTextStringPosition;

	protected:
	// Constructor

	CreateReadWordResultType()
		{
		result = RESULT_OK;

		hasCreatedReadWord = false;

		nextTextStringPosition = 0;
		}
	};

/*************************************************************************
 *	"The Lord is king!
 *	Let the nations tremble!
 *	He sits on his throne between the cherubim.
 *	Let the whole earth quake!" (Psalm 99:1)
 *************************************************************************/
