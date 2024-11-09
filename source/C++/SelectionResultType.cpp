/*	Class:		SelectionResultType
 *	Purpose:	To return selection variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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
// Class declarations
class SelectionItem;

class SelectionResultType
	{
	friend class AdminImperative;
	friend class Item;
	friend class List;
	friend class ScoreList;

	protected:
	// Protected variables

	signed char result = RESULT_OK;
	SelectionItem *selectionItem = NULL;
	};

/*************************************************************************
 *	"For the angel of the Lord is a guard;
 *	he surrounds and defends all who fear him." (Psalm 34:7)
 *************************************************************************/
