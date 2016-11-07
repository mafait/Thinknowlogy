/*	Class:		GrammarResultType
 *	Purpose:	To return grammar variables,
 *				as the result of a function call
 *	Version:	Thinknowlogy 2016r2 (Restyle)
 *************************************************************************/
/*	Copyright (C) 2009-2016, Menno Mafait. Your suggestions, modifications,
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
// Some compilers need these class declarations
class GrammarItem;

class GrammarResultType
	{
	friend class AdminReadCreateWords;
	friend class AdminReadGrammar;
	friend class GrammarList;
	friend class Item;
	friend class List;
	friend class WordItem;
	friend class WordType;

	protected:
	// Protected variables

	ResultType result;

	bool hasFoundWordEnding;

	size_t singularNounWordStringLength;

	GrammarItem *createdGrammarItem;
	GrammarItem *foundGrammarItem;

	char singularNounWordString[MAX_WORD_LENGTH];

	protected:
	// Constructor

	GrammarResultType()
		{
		result = RESULT_OK;

		hasFoundWordEnding = false;

		singularNounWordStringLength = 0;

		createdGrammarItem = NULL;
		foundGrammarItem = NULL;

		strcpy( singularNounWordString, EMPTY_STRING );
		}
	};

/*************************************************************************
 *	"Sing praises to God, sing praises;
 *	sing praises to our King, sing praises.
 *	For God is the King over all the earth.
 *	Praise him with a psalm!" (Psalm 47:6-7)
 *************************************************************************/
