/*	Class:			SelectionList
 *	Parent class:	List
 *	Purpose:		Storing selection items
 *	Version:		Thinknowlogy 2023 (Shaking tree)
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

#include "List.h"
#include "SelectionItem.cpp"

class SelectionList : private List
	{
	friend class AdminItem;
	friend class WordItem;

	// Private selection functions

	void deleteSelectionList( SelectionItem *searchSelectionItem )
		{
		SelectionItem *deleteSelectionItem;

		while( searchSelectionItem != NULL )
			{
			deleteSelectionItem = searchSelectionItem;
			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			delete deleteSelectionItem;
			}
		}

	unsigned int getLowerSentenceNr( unsigned int duplicateSentenceNr )
		{
		Item *searchWordItem = firstActiveItem();
		unsigned int lowerSentenceNr = NO_SENTENCE_NR;

		while( searchWordItem != NULL )
			{
			if( searchWordItem->activeSentenceNr() < duplicateSentenceNr &&
			searchWordItem->activeSentenceNr() > lowerSentenceNr )
				lowerSentenceNr = searchWordItem->activeSentenceNr();

			searchWordItem = searchWordItem->nextItem;
			}

		return lowerSentenceNr;
		}


	// Constructor

	SelectionList( char _listChar, GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( _listChar, "SelectionList", globalVariables, inputOutput, myWordItem );
		}

	~SelectionList()
		{
		deleteSelectionList( firstActiveSelectionItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class SelectionList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class SelectionList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class SelectionList has replaced items." );

		deleteSelectionList( (SelectionItem *)firstDeletedItem() );
		}


	// Protected functions

	void clearConditionChecksForSolving( unsigned short selectionLevel, unsigned int conditionSentenceNr )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != NULL )
			{
			if( searchSelectionItem->selectionLevel() < selectionLevel &&
			searchSelectionItem->activeSentenceNr() == conditionSentenceNr )
				searchSelectionItem->isConditionCheckedForSolving = false;

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}
		}

	signed char checkWordItemForUsage( WordItem *unusedWordItem )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, "The given unused word item is undefined" );

		while( searchSelectionItem != NULL &&
		searchSelectionItem->checkWordItemForUsage( unusedWordItem ) == RESULT_OK )
			searchSelectionItem = searchSelectionItem->nextSelectionItem();

		return globalVariables()->result;
		}

	signed char createSelectionItem( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createSelectionItem";

		if( generalizationWordTypeNr <= NO_WORD_TYPE_NR ||
		generalizationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given generalization word type number is undefined or out of bounds" );

		if( specificationWordTypeNr <= NO_WORD_TYPE_NR ||
		specificationWordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given specification word type number is undefined or out of bounds" );

		if( addItemToList( QUERY_ACTIVE_CHAR, new SelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add a selection item" );

		return RESULT_OK;
		}

	SelectionItem *executionStartEntry( unsigned short executionLevel, unsigned int executionSentenceNr )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != NULL &&
		searchSelectionItem->activeSentenceNr() >= executionSentenceNr )
			{
			if( searchSelectionItem->activeSentenceNr() == executionSentenceNr &&
			searchSelectionItem->selectionLevel() == executionLevel )
				return searchSelectionItem;

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return NULL;
		}

	SelectionItem *firstActiveSelectionItem()
		{
		return (SelectionItem *)firstActiveItem();
		}

	SelectionItem *firstSelectionItem( WordItem *solveWordItem )
		{
		SelectionItem *firstSelectionItem;

		return ( ( firstSelectionItem = firstActiveSelectionItem() ) != NULL ?
				firstSelectionItem->firstSelectionItem( solveWordItem ) : NULL );
		}

	SelectionItem *firstConditionSelectionItem( unsigned int conditionSentenceNr )
		{
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();

		while( searchSelectionItem != NULL &&
		searchSelectionItem->activeSentenceNr() >= conditionSentenceNr )
			{
			if( searchSelectionItem->activeSentenceNr() == conditionSentenceNr )
				return searchSelectionItem;

			searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return NULL;
		}

	DuplicateResultType checkForDuplicateCondition()
		{
		unsigned int duplicateConditionSentenceNr = globalVariables()->currentSentenceNr;
		DuplicateResultType duplicateResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDuplicateCondition";

		do	{
			if( ( duplicateConditionSentenceNr = getLowerSentenceNr( duplicateConditionSentenceNr ) ) > NO_SENTENCE_NR )
				{
				if( ( duplicateResult = checkForDuplicateSelectionPart( duplicateConditionSentenceNr ) ).result != RESULT_OK )
					return addDuplicateResultError( functionNameString, "I failed to check if the alternative selection part is duplicate" );

				duplicateConditionSentenceNr = duplicateResult.duplicateConditionSentenceNr;
				}
			}
		while( !duplicateResult.hasFoundDuplicateSelection &&
		duplicateConditionSentenceNr > NO_SENTENCE_NR );

		duplicateResult.duplicateConditionSentenceNr = duplicateConditionSentenceNr;
		return duplicateResult;
		}

	DuplicateResultType checkForDuplicateSelectionPart( unsigned int duplicateConditionSentenceNr )
		{
		bool hasFoundDuplicateSelection = true;
		char *currentSpecificationString;
		char *searchSpecificationString;
		SelectionItem *currentSelectionItem = NULL;
		SelectionItem *searchSelectionItem = firstActiveSelectionItem();
		DuplicateResultType duplicateResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDuplicateSelectionPart";

		if( duplicateConditionSentenceNr <= NO_SENTENCE_NR )
			return startDuplicateResultError( functionNameString, "The given duplicate sentence number is undefined" );

		if( duplicateConditionSentenceNr >= globalVariables()->currentSentenceNr )
			return startDuplicateResultError( functionNameString, "The given duplicate sentence number is equal or higher than the current sentence number" );

		while( searchSelectionItem != NULL &&
		searchSelectionItem->activeSentenceNr() >= duplicateConditionSentenceNr )
			{
			if( searchSelectionItem->activeSentenceNr() == duplicateConditionSentenceNr )
				{
				currentSelectionItem = firstActiveSelectionItem();

				do	{
					if( currentSelectionItem->isAction() == searchSelectionItem->isAction() &&
					currentSelectionItem->isAssignedOrEmpty() == searchSelectionItem->isAssignedOrEmpty() &&
					currentSelectionItem->isNegative() == searchSelectionItem->isNegative() &&
					currentSelectionItem->isNewStart() == searchSelectionItem->isNewStart() &&
					currentSelectionItem->selectionLevel() == searchSelectionItem->selectionLevel() &&
					currentSelectionItem->generalizationWordItem() == searchSelectionItem->generalizationWordItem() &&
					currentSelectionItem->specificationWordItem() == searchSelectionItem->specificationWordItem() )
						{
						currentSpecificationString = currentSelectionItem->specificationString();
						searchSpecificationString = searchSelectionItem->specificationString();

						if( currentSpecificationString != NULL &&
						searchSpecificationString != NULL )
							hasFoundDuplicateSelection = ( strcmp( currentSpecificationString, searchSpecificationString ) == 0 );
						else
							{
							if( currentSpecificationString != NULL ||
							searchSpecificationString != NULL )
								hasFoundDuplicateSelection = false;
							}

						if( hasFoundDuplicateSelection )
							{
							duplicateResult.duplicateConditionSentenceNr = searchSelectionItem->creationSentenceNr();

							currentSelectionItem = ( currentSelectionItem->nextSelectionItem() != NULL &&
													currentSelectionItem->nextSelectionItem()->hasCurrentCreationSentenceNr() ? currentSelectionItem->nextSelectionItem() : NULL );

							searchSelectionItem = ( searchSelectionItem->nextSelectionItem() != NULL &&
													searchSelectionItem->nextSelectionItem()->activeSentenceNr() == duplicateConditionSentenceNr ? searchSelectionItem->nextSelectionItem() : NULL );
							}
						else
							searchSelectionItem = NULL;
						}
					else
						{
						hasFoundDuplicateSelection = false;
						searchSelectionItem = NULL;
						}
					}
				while( hasFoundDuplicateSelection &&
				currentSelectionItem != NULL &&
				searchSelectionItem != NULL );
				}
			else
				searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		duplicateResult.hasFoundDuplicateSelection = hasFoundDuplicateSelection;
		return duplicateResult;
		}
	};

/*************************************************************************
 *	"Who can be compared with the Lord or God,
 *	who is enthroned on high?" (Psalm 113:5)
 *************************************************************************/
