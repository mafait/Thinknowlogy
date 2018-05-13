/*	Class:		RelatedResultType
 *	Purpose:	To return related specification variables,
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

#include "Item.h"
// Class declarations
class SpecificationItem;

class RelatedResultType
	{
	friend class AdminReasoningOld;
	friend class Item;
	friend class JustificationList;
	friend class WordItem;
	friend class WordQuestion;
	friend class WordWrite;
	friend class WordSpecification;

	protected:
	// Protected variables

	signed char result;
	bool isFirstRelatedSpecification;
	bool isLastRelatedSpecification;
	SpecificationItem *relatedSpecificationItem;

	protected:
	// Constructor

	RelatedResultType()
		{
		result = RESULT_OK;
		isFirstRelatedSpecification = false;
		isLastRelatedSpecification = false;
		relatedSpecificationItem = NULL;
		}
	};

/*************************************************************************
 *	"He gives food to those who fear him;
 *	he always remembers his covenant." (Psalm 111:5)
 *************************************************************************/
