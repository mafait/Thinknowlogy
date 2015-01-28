/*
 *	Class:			SelectionList
 *	Parent class:	List
 *	Purpose:		To store selection items
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

// Selection List header

#ifndef SELECTIONLIST
#define SELECTIONLIST 1

#include "List.h"
#include "SelectionItem.cpp"
#include "SelectionResultType.cpp"

class SelectionList : private List
	{
	friend class AdminItem;
	friend class AdminReadSentence;
	friend class AdminSelection;
	friend class AdminSolve;
	friend class WordItem;

	// Private selection functions

	unsigned int getLowerSentenceNr( unsigned int duplicateSentenceNr );


	protected:
	// Constructor / deconstructor

	SelectionList( char _listChar, CommonVariables *commonVariables, WordItem *myWordItem );
	~SelectionList();


	// Protected virtual functions

	virtual ReferenceResultType findWordReference( WordItem *referenceWordItem );


	// Protected functions

	void clearConditionChecksForSolving( unsigned short selectionLevel, unsigned int conditionSentenceNr );

	ResultType checkSelectionItemForUsage( SelectionItem *unusedSelectionItem );
	ResultType checkWordItemForUsage( WordItem *unusedWordItem );
//	ResultType storeChangesInFutureDatabase();

	SelectionResultType checkDuplicateCondition();
	SelectionResultType checkDuplicateSelectionPart( unsigned int duplicateConditionSentenceNr );
	SelectionResultType createSelectionItem( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short imperativeParameter, unsigned short prepositionParameter, unsigned short specificationWordParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );
	SelectionResultType findFirstExecutionItem( WordItem *solveWordItem );

	SelectionItem *executionStartEntry( unsigned short executionLevel, unsigned int executionSentenceNr );

	SelectionItem *firstActiveSelectionItem();
	SelectionItem *firstConditionSelectionItem( unsigned int conditionSentenceNr );
	};
#endif

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
