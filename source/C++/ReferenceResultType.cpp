/*
 *	Class:			ReferenceResultType
 *	Purpose:		To return string variables of a function
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

class ReferenceResultType
	{
	friend class AdminCleanup;
	friend class CollectionItem;
	friend class ContextItem;
	friend class GeneralizationItem;
	friend class InterfaceList;
	friend class Item;
	friend class List;
	friend class ListQuery;
	friend class MultipleWordItem;
	friend class ReadItem;
	friend class ReadList;
	friend class SelectionItem;
	friend class SelectionList;
	friend class SpecificationItem;
	friend class WordItem;
	friend class WordTypeList;
	protected:
	// Protected variables

	ResultType result;

	bool hasFoundMatchingStrings;
	bool hasFoundWordReference;

	protected:
	// Constructor / deconstructor

	ReferenceResultType()
		{
		result = RESULT_OK;

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
