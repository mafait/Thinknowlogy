/*
 *	Class:			ReadResultType
 *	Purpose:		To return read word variables of a function
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

#ifndef READRESULTTYPE
#define READRESULTTYPE 1
#include "Item.h"
// Some compilers need these class declarations
class ReadItem;

class ReadResultType
	{
	friend class AdminCleanup;
	friend class AdminItem;
	friend class AdminReadCreateWords;
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class AdminWriteSpecification;
	friend class ReadList;
	protected:
	// Protected variables

	ResultType result;

	bool hasFoundGrammarDefinition;
	bool hasFoundMatchingWordType;
	bool hasFoundMoreInterpretations;
	bool hasCreatedAllReadWords;

	unsigned short nReadWordReferences;

	size_t startWordPosition;
	size_t nextWordPosition;
	size_t wordLength;

	ReadItem *createdReadItem;

	protected:
	// Constructor / deconstructor

	ReadResultType()
		{
		result = RESULT_OK;

		hasFoundGrammarDefinition = false;
		hasFoundMatchingWordType = false;
		hasFoundMoreInterpretations = false;
		hasCreatedAllReadWords = false;

		nReadWordReferences = 0;

		startWordPosition = 0;
		nextWordPosition = 0;
		wordLength = 0;

		createdReadItem = NULL;
		}
	};
#endif

/*************************************************************************
 *	"As a deer longs for streams of water,
 *	so I long for you, O God.
 *	I thirst for God, the living God.
 *	When can I go and stand before him?" (Psalm 42:1-2)
 *************************************************************************/
