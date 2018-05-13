/*	Class:		BoolResultType
 *	Purpose:	To return word parameter variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2018r1 (ShangDi 上帝)
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

#include "Constants.h"

class BoolResultType
	{
	friend class AdminImperative;
	friend class AdminReadCreateWords;
	friend class AdminReadSentence;
	friend class AdminReasoningNew;
	friend class AdminSpecification;
	friend class CollectionItem;
	friend class CollectionList;
	friend class ContextItem;
	friend class GeneralizationItem;
	friend class GeneralizationList;
	friend class InterfaceList;
	friend class Item;
	friend class List;
	friend class MultipleWordItem;
	friend class ReadItem;
	friend class ReadList;
	friend class ScoreList;
	friend class SelectionItem;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordSpecification;
	friend class WordTypeList;

	protected:
	// Protected variables

	signed char result;
	bool booleanValue;

	protected:
	// Constructor

	BoolResultType()
		{
		result = RESULT_OK;
		booleanValue = false;
		}
	};

/*************************************************************************
 *	"How great is the Lord,
 *	how deserving of praise,
 *	in the city of our God,
 *	which sits on his holy mountain!" (Psalm 48:1)
 *************************************************************************/
