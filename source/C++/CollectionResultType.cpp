/*
 *	Class:			CollectionResultType
 *	Purpose:		To return word type variables of a function
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

#include <string.h>
#include "Constants.h"

// Class declarations needed by some compilers
class WordItem;

class CollectionResultType
	{
	friend class AdminCollection;
	friend class AdminItem;
	friend class AdminSpecification;
	friend class CollectionList;
	friend class WordCollection;
	friend class WordItem;
	friend class WordSpecification;
	protected:
	// Protected variables

	ResultType result;

	bool isAmbiguousCollection;
	bool isCollected;

	unsigned int createdCollectionNr;
	unsigned int foundRelationContextNr;

	WordItem *foundGeneralizationWordItem;

	protected:
	// Constructor / deconstructor

	CollectionResultType()
		{
		result = RESULT_OK;

		isAmbiguousCollection = false;
		isCollected = false;

		createdCollectionNr = NO_COLLECTION_NR;
		foundRelationContextNr = NO_CONTEXT_NR;

		foundGeneralizationWordItem = NULL;
		}
	};

/*************************************************************************
 *
 *	"Oh, the joys of those who are kind to the poor!
 *	The Lord rescues them when they are in trouble." (Psalm 41:1)
 *
 *************************************************************************/
