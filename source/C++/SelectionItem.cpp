/*	Class:			SelectionItem
 *	Parent class:	Item
 *	Purpose:		To store the selection structure
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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

#ifndef SELECTIONITEM
#define SELECTIONITEM 1
#include "WordItem.h"

class SelectionItem : private Item
	{
	friend class AdminImperative;
	friend class AdminSpecification;
	friend class ScoreItem;
	friend class SelectionList;

	// Private initialized variables

	bool isAction_ = false;
	bool isAssignedOrEmpty_ = false;
	bool isInactiveAssignment_ = false;
	bool isArchivedAssignment_ = false;
	bool isFirstComparisonPart_ = false;
	bool isNewStart_ = false;
	bool isNegative_ = false;
	bool isPossessive_ = false;
	bool isSpecificationGeneralization_ = false;
	bool isUniqueUserRelation_ = false;
	bool isValueSpecification_ = false;

	unsigned short assumptionLevel_ = NO_ASSIGNMENT_LEVEL;
	unsigned short selectionLevel_ = NO_SELECTION_LEVEL;
	unsigned short imperativeVerbParameter_ = NO_IMPERATIVE_PARAMETER;
	unsigned short prepositionParameter_ = NO_PREPOSITION_PARAMETER;

	unsigned short generalizationWordTypeNr_ = NO_WORD_TYPE_NR;
	unsigned short specificationWordTypeNr_ = NO_WORD_TYPE_NR;
	unsigned short relationWordTypeNr_ = NO_WORD_TYPE_NR;

	unsigned int generalizationContextNr_ = NO_CONTEXT_NR;
	unsigned int specificationContextNr_ = NO_CONTEXT_NR;
	unsigned int relationContextNr_ = NO_CONTEXT_NR;

	unsigned int nContextRelations_ = 0;

	WordItem *generalizationWordItem_ = NULL;
	WordItem *specificationWordItem_ = NULL;
	WordItem *relationWordItem_ = NULL;

	char *specificationString_ = NULL;


	// Private functions

	SelectionItem *nextSelectionItem( bool isIncludingThisItem, WordItem *solveWordItem )
		{
		SelectionItem *searchSelectionItem = ( isIncludingThisItem ? this : nextSelectionItem() );

		if( solveWordItem != NULL )
			{
			while( searchSelectionItem != NULL )
				{
				if( searchSelectionItem->generalizationWordItem_ == solveWordItem &&
				searchSelectionItem->specificationWordItem_ != NULL )
					return searchSelectionItem;

				searchSelectionItem = searchSelectionItem->nextSelectionItem();
				}
			}

		return NULL;
		}


	protected:
	// Protected constructed variables

	bool isConditionCheckedForSolving = false;


	// Constructor

	SelectionItem( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		size_t specificationStringLength;

		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "SelectionItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		isAction_ = isAction;
		isAssignedOrEmpty_ = isAssignedOrClear;
		isNewStart_ = isNewStart;

		isInactiveAssignment_ = isInactiveAssignment;
		isArchivedAssignment_ = isArchivedAssignment;

		isFirstComparisonPart_ = isFirstComparisonPart;

		isNegative_ = isNegative;
		isPossessive_ = isPossessive;
		isSpecificationGeneralization_ = isSpecificationGeneralization;
		isUniqueUserRelation_ = isUniqueUserRelation;
		isValueSpecification_ = isValueSpecification;

		assumptionLevel_ = assumptionLevel;
		selectionLevel_ = selectionLevel;

		imperativeVerbParameter_ = imperativeVerbParameter;
		prepositionParameter_ = prepositionParameter;

		generalizationWordTypeNr_ = generalizationWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;
		relationWordTypeNr_ = relationWordTypeNr;

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nContextRelations_ = nContextRelations;

		generalizationWordItem_ = generalizationWordItem;
		specificationWordItem_ = specificationWordItem;
		relationWordItem_ = relationWordItem;

		if( specificationString != NULL )
			{
			if( ( specificationStringLength = strlen( specificationString ) ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( specificationString_ = new char[specificationStringLength + 1] ) != NULL )
					strcpy( specificationString_, specificationString );
				else
					startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create a specification string" );
				}
			else
				startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given specification string is too long" );
			}
		}

	~SelectionItem()
		{
		if( specificationString_ != NULL )
			delete specificationString_;
		}


	// Protected virtual functions

	virtual void displayString( bool isReturnQueryToPosition )
		{
		statusString[0] = statusChar();

		if( specificationString_ != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables()->queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( globalVariables()->queryString, specificationString_ );
			}
		}

	virtual void displayWordReferences( bool isReturnQueryToPosition )
		{
		char *queryString;
		char *wordString;

		statusString[0] = statusChar();

		queryString = globalVariables()->queryString;

		if( generalizationWordItem_ != NULL &&
		( wordString = generalizationWordItem_->wordTypeString( true, generalizationWordTypeNr_ ) ) != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}

		if( specificationWordItem_ != NULL &&
		( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( globalVariables()->hasFoundQuery ||
			strlen( queryString ) > 0 )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}

		if( relationWordItem_ != NULL &&
		( wordString = relationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( globalVariables()->hasFoundQuery ||
			strlen( queryString ) > 0 )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( selectionLevel_ == queryParameter ||
				imperativeVerbParameter_ == queryParameter ||
				prepositionParameter_ == queryParameter ||
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nContextRelations_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( selectionLevel_ > NO_SELECTION_LEVEL ||
				imperativeVerbParameter_ > NO_IMPERATIVE_PARAMETER ||
				prepositionParameter_ > NO_PREPOSITION_PARAMETER ||
				generalizationContextNr_ > NO_CONTEXT_NR ||
				specificationContextNr_ > NO_CONTEXT_NR ||
				relationContextNr_ > NO_CONTEXT_NR ||
				nContextRelations_ > 0 ) ) );
		}

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( generalizationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : generalizationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : generalizationWordItem_->itemNr() == queryItemNr ) ) ||

				( specificationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : specificationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : specificationWordItem_->itemNr() == queryItemNr ) ) ||

				( relationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : relationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : relationWordItem_->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( generalizationWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr ||
				relationWordTypeNr_ == queryWordTypeNr );
		}

	virtual char *itemString()
		{
		return specificationString_;
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;
		char *wordString;
		char *generalizationWordTypeString = myWordItem()->wordTypeNameString( generalizationWordTypeNr_ );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );
		char *relationWordTypeString = myWordItem()->wordTypeNameString( relationWordTypeNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( isAction_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isAction" );
			}

		if( isAssignedOrEmpty_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isAssignedOrClear" );
			}

		if( isNewStart_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isNewStart" );
			}

		if( isInactiveAssignment_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isInactiveAssignment" );
			}

		if( isArchivedAssignment_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isArchivedAssignment" );
			}

		if( isFirstComparisonPart_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isFirstComparisonPart" );
			}

		if( isNegative_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isNegative" );
			}

		if( isPossessive_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isPossessive" );
			}

		if( isSpecificationGeneralization_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSpecificationGeneralization" );
			}

		if( isUniqueUserRelation_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isUniqueUserRelation" );
			}

		if( isValueSpecification_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isValueSpecification" );
			}

		if( isConditionCheckedForSolving )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isConditionCheckedForSolving" );
			}

		if( assumptionLevel_ > NO_ASSUMPTION_LEVEL )
			{
			sprintf( tempString, "%cassumptionLevel:%u", QUERY_SEPARATOR_CHAR, assumptionLevel_ );
			strcat( queryString, tempString );
			}

		if( selectionLevel_ > NO_SELECTION_LEVEL )
			{
			sprintf( tempString, "%cselectionLevel:%u", QUERY_SEPARATOR_CHAR, selectionLevel_ );
			strcat( queryString, tempString );
			}

		if( imperativeVerbParameter_ > NO_IMPERATIVE_PARAMETER )
			{
			sprintf( tempString, "%cimperativeVerbParameter:%u", QUERY_SEPARATOR_CHAR, imperativeVerbParameter_ );
			strcat( queryString, tempString );
			}

		if( prepositionParameter_ > NO_PREPOSITION_PARAMETER )
			{
			sprintf( tempString, "%cprepositionParameter:%u", QUERY_SEPARATOR_CHAR, prepositionParameter_ );
			strcat( queryString, tempString );
			}

		if( generalizationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cgeneralizationContextNr:%u", QUERY_SEPARATOR_CHAR, generalizationContextNr_ );
			strcat( queryString, tempString );
			}

		if( generalizationWordTypeString == NULL )
			sprintf( tempString, "%cgeneralizationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );
		else
			sprintf( tempString, "%cgeneralizationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, generalizationWordTypeString, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );

		strcat( queryString, tempString );

		if( generalizationWordItem_ != NULL )
			{
			sprintf( tempString, "%cgeneralizationWordItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, generalizationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, generalizationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = generalizationWordItem_->wordTypeString( true, generalizationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( specificationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cspecificationContextNr:%u", QUERY_SEPARATOR_CHAR, specificationContextNr_ );
			strcat( queryString, tempString );
			}

		if( specificationWordTypeString == NULL )
			sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
		else
			sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

		strcat( queryString, tempString );

		if( specificationWordItem_ != NULL )
			{
			sprintf( tempString, "%cspecificationWordItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, specificationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, specificationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( relationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%crelationContextNr:%u", QUERY_SEPARATOR_CHAR, relationContextNr_ );
			strcat( queryString, tempString );
			}

		if( relationWordTypeString == NULL )
			sprintf( tempString, "%crelationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, relationWordTypeNr_ );
		else
			sprintf( tempString, "%crelationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, relationWordTypeString, QUERY_WORD_TYPE_CHAR, relationWordTypeNr_ );

		strcat( queryString, tempString );

		if( relationWordItem_ != NULL )
			{
			sprintf( tempString, "%crelationWordItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, relationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, relationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = relationWordItem_->wordTypeString( true, relationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( nContextRelations_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cnContextRelations:%u", QUERY_SEPARATOR_CHAR, nContextRelations_ );
			strcat( queryString, tempString );
			}

		if( specificationString_ != NULL )
			{
			sprintf( tempString, "%cspecificationString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, specificationString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}

	virtual BoolResultType findMatchingWordReferenceString( char *queryString )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findMatchingWordReferenceString";

		if( generalizationWordItem_ != NULL &&
		( boolResult = generalizationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
			return addBoolResultError( functionNameString, NULL, specificationString_, "I failed to find a matching word reference string for the generalization word" );

		// No matching string
		if( !boolResult.booleanValue &&
		specificationWordItem_ != NULL &&
		( boolResult = specificationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
			return addBoolResultError( functionNameString, NULL, specificationString_, "I failed to find a matching word reference string for the specification word" );

		// No matching string
		if( !boolResult.booleanValue &&
		relationWordItem_ != NULL &&
		( boolResult = relationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
			return addBoolResultError( functionNameString, NULL, specificationString_, "I failed to find a matching word reference string for the relation word" );

		return boolResult;
		}


	// Protected functions

	bool isNumeralRelation()
		{
		return ( relationWordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isAction()
		{
		return isAction_;
		}

	bool isAssignedOrEmpty()
		{
		return isAssignedOrEmpty_;
		}

	bool isNewStart()
		{
		return isNewStart_;
		}

	bool isInactiveAssignment()
		{
		return isInactiveAssignment_;
		}

	bool isArchivedAssignment()
		{
		return isArchivedAssignment_;
		}

	bool isFirstComparisonPart()
		{
		return isFirstComparisonPart_;
		}

	bool isNegative()
		{
		return isNegative_;
		}

	bool isPossessive()
		{
		return isPossessive_;
		}

	bool isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	bool isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
		}

	bool isValueSpecification()
		{
		return isValueSpecification_;
		}

	unsigned short assumptionLevel()
		{
		return assumptionLevel_;
		}

	unsigned short selectionLevel()
		{
		return selectionLevel_;
		}

	unsigned short imperativeVerbParameter()
		{
		return imperativeVerbParameter_;
		}

	unsigned short prepositionParameter()
		{
		return prepositionParameter_;
		}

	unsigned short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
		}

	unsigned short relationWordTypeNr()
		{
		return relationWordTypeNr_;
		}

	unsigned int generalizationContextNr()
		{
		return generalizationContextNr_;
		}

	unsigned int specificationContextNr()
		{
		return specificationContextNr_;
		}

	unsigned int relationContextNr()
		{
		return relationContextNr_;
		}

	unsigned int nContextRelations()
		{
		return nContextRelations_;
		}

	char *specificationString()
		{
		return specificationString_;
		}

	SelectionItem *firstSelectionItem( WordItem *solveWordItem )
		{
		return nextSelectionItem( true, solveWordItem );
		}

	SelectionItem *nextSelectionItem( WordItem *solveWordItem )
		{
		return nextSelectionItem( false, solveWordItem );
		}

	SelectionItem *nextSelectionItem()
		{
		return (SelectionItem *)nextItem;
		}

	SelectionItem *nextConditionItem( unsigned short executionLevel, unsigned int conditionSentenceNr )
		{
		SelectionItem *_nextSelectionItem;

		return ( ( _nextSelectionItem = nextSelectionItem() ) != NULL &&
				_nextSelectionItem->selectionLevel_ <= executionLevel &&
				_nextSelectionItem->creationSentenceNr() == conditionSentenceNr ? _nextSelectionItem : NULL );
		}

	SelectionItem *nextExecutionItem( unsigned short executionLevel, unsigned int executionSentenceNr )
		{
		SelectionItem *_nextSelectionItem;

		return ( ( _nextSelectionItem = nextSelectionItem() ) != NULL &&
				_nextSelectionItem->selectionLevel_ == executionLevel &&
				_nextSelectionItem->creationSentenceNr() == executionSentenceNr ? _nextSelectionItem : NULL );
		}

	WordItem *generalizationWordItem()
		{
		return generalizationWordItem_;
		}

	WordItem *specificationWordItem()
		{
		return specificationWordItem_;
		}

	WordItem *relationWordItem()
		{
		return relationWordItem_;
		}
	};
#endif

/*************************************************************************
 *	"Light shines in the darkness for the godly.
 *	They are generous, compassionate, and righteous." (Psalm 112:4)
 *************************************************************************/
