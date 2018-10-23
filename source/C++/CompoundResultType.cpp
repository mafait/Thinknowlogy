/*	Class:		CompoundResultType
 *	Purpose:	To return specification variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2018r3 (Deep Magic)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

#include "WordItem.h"

class CompoundResultType
	{
	friend class AdminReasoningNew;
	friend class AdminSpecification;
	friend class Item;

	protected:
	// Protected variables

	signed char result = RESULT_OK;
	WordItem *compoundGeneralizationWordItem = NULL;
	};

/*************************************************************************
 *	"Oh, the joys of those who trust the Lord,
 *	who have no confidence of the proud
 *	or in those who worship idols." (Psalm 40:4)
 *************************************************************************/
