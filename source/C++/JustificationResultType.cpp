/*
 *	Class:			JustificationResultType
 *	Purpose:		To return justification variables of a function
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

#include "Item.h"
// Class declarations needed by some compilers
class JustificationItem;

class JustificationResultType
	{
	friend class AdminAssumption;
	friend class JustificationList;
	friend class SpecificationList;
	friend class WordItem;
	protected:
	// Protected variables

	ResultType result;

	JustificationItem *createdJustificationItem;
	JustificationItem *foundJustificationItem;

	protected:
	// Constructor / deconstructor

	JustificationResultType()
		{
		result = RESULT_OK;

		createdJustificationItem = NULL;
		foundJustificationItem = NULL;
		}
	};

/*************************************************************************
 *
 *	"He renews my strength.
 *	He guides me along right paths,
 *	bringing honor to his name." (Psalm 23:3)
 *
 *************************************************************************/
