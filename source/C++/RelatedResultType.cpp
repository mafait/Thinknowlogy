/*	Class:		RelatedResultType
 *	Purpose:	To return related specification variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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
class SpecificationItem;

class RelatedResultType
	{
	friend class AdminReasoning;
	friend class AdminWrite;
	friend class Item;
	friend class JustificationList;
	friend class WordItem;
	friend class WordQuestion;
	friend class WordWrite;
	friend class WordSpecification;

	protected:
	// Protected variables

	signed char result = RESULT_OK;

	bool isFirstRelatedSpecification = false;
	bool isLastRelatedSpecification = false;

	SpecificationItem *relatedSpecificationItem = NULL;
	};

/*************************************************************************
 *	"He gives food to those who fear him;
 *	he always remembers his covenant." (Psalm 111:5)
 *************************************************************************/
