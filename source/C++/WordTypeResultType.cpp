/*	Class:		WordTypeResultType
 *	Purpose:	To return word type variables,
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

#ifndef WORDTYPERESULTTYPE
#define WORDTYPERESULTTYPE 1
#include "Item.h"
// Class declarations
class WordTypeItem;

class WordTypeResultType
	{
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class Item;
	friend class List;
	friend class WordTypeList;

	protected:
	// Protected variables

	signed char result = RESULT_OK;
	WordTypeItem *wordTypeItem = NULL;
	};
#endif

/*************************************************************************
 *	"We thank you, O God!
 *	We give thanks because you are near.
 *	People everywhere tell your wonderful deeds." (Psalm 75:1)
 *************************************************************************/
