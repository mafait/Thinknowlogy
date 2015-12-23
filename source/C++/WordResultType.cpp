/*
 *	Class:			WordResultType
 *	Purpose:		To return word variables of a function
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

#include "Item.h"
// Some compilers need these class declarations
class WordTypeItem;

class WordResultType
	{
	friend class AdminAuthorization;
	friend class AdminItem;
	friend class AdminLanguage;
	friend class AdminReadCreateWords;
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class WordItem;
	friend class WordList;
	friend class WordSpecification;
	friend class WordType;
	friend class WordTypeItem;
	friend class WordTypeList;
	protected:
	// Protected variables

	ResultType result;

	bool hasFoundDifferentParameter;

	WordItem *createdWordItem;
	WordItem *foundWordItem;

	WordTypeItem *createdWordTypeItem;
	WordTypeItem *foundWordTypeItem;

	protected:
	// Constructor / deconstructor

	WordResultType()
		{
		result = RESULT_OK;

		hasFoundDifferentParameter = false;

		createdWordItem = NULL;
		foundWordItem = NULL;

		createdWordTypeItem = NULL;
		foundWordTypeItem = NULL;
		}
	};

/*************************************************************************
 *	"I take joy in doing your will, my God,
 *	for your instructions are written on my heart." (Psalm 40:8)
 *************************************************************************/
