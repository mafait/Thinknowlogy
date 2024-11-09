/*	Class:		ReadWordResultType
 *	Purpose:	To return read word variables,
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

#ifndef READWORDRESULTTYPE
#define READWORDRESULTTYPE 1
#include "Item.h"

class ReadWordResultType
	{
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminWrite;
	friend class Item;

	protected:
	// Protected variables

	signed char result = RESULT_OK;

	bool hasFoundGrammarDefinition = false;

	size_t offset = 0;
	size_t startWordPosition = 0;
	size_t wordLength = 0;
	};
#endif

/*************************************************************************
 *	"As a deer longs for streams of water,
 *	so I long for you, O God.
 *	I thirst for God, the living God.
 *	When can I go and stand before him?" (Psalm 42:1-2)
 *************************************************************************/
