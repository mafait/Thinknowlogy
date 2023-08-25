﻿/*	Class:		ShortResultType
 *	Purpose:	To return specification variables,
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

#ifndef SHORTRESULTTYPE
#define SHORTRESULTTYPE 1
#include "Constants.h"

class ShortResultType
	{
	friend class AdminImperative;
	friend class AdminReadCreateWords;
	friend class AdminReadSentence;
	friend class Item;
	friend class JustificationItem;
	friend class List;
	friend class ReadList;

	protected:
	// Protected variables

	signed char result = RESULT_OK;
	unsigned short shortValue = 0;
	};
#endif

/*************************************************************************
 *	"I will sing of your love and justice, O Lord.
 *	I will praise you with songs." (Psalm 101:1)
 *************************************************************************/
