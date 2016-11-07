/*	Class:			SelectionItem
 *	Parent class:	Item
 *	Purpose:		To store the selection structure
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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

#ifndef SELECTIONITEM
#define SELECTIONITEM 1
#include "WordItem.h"

class SelectionItem : private Item
	{
	friend class AdminImperative;
	friend class AdminSelection;
	friend class AdminSolve;
	friend class AdminSpecification;
	friend class ScoreItem;
	friend class SelectionList;
	friend class WordSelectionCondition;

	// Private constructed variables

	SelectionItem *nextExecutionItem_;


	// Private initialized variables

	bool isAction_;
	bool isAssignedOrClear_;
	bool isInactive_;
	bool isArchived_;
	bool isFirstComparisonPart_;
	bool isNewStart_;
	bool isNegative_;
	bool isPossessive_;
	bool isSpecificationGeneralization_;
	bool isUniqueUserRelation_;
	bool isValueSpecification_;

	unsigned short assumptionLevel_;
	unsigned short selectionLevel_;
	unsigned short imperativeVerbParameter_;
	unsigned short prepositionParameter_;

	unsigned short generalizationWordTypeNr_;
	unsigned short specificationWordTypeNr_;
	unsigned short relationWordTypeNr_;

	unsigned int generalizationContextNr_;
	unsigned int specificationContextNr_;
	unsigned int relationContextNr_;

	unsigned int nContextRelations_;

	WordItem *generalizationWordItem_;
	WordItem *specificationWordItem_;
	WordItem *relationWordItem_;

	char *specificationString_;


	protected:
	// Protected constructed variables

	bool isConditionCheckedForSolving;


	// Constructor

	SelectionItem( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		size_t specificationStringLength;

		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "SelectionItem", commonVariables, myList, myWordItem );

		// Private constructed variables

		nextExecutionItem_ = NULL;

		// Private initialized variables

		isAction_ = isAction;
		isAssignedOrClear_ = isAssignedOrClear;
		isNewStart_ = isNewStart;

		isInactive_ = isInactiveAssignment;
		isArchived_ = isArchivedAssignment;

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

		specificationString_ = NULL;

		generalizationWordItem_ = generalizationWordItem;
		specificationWordItem_ = specificationWordItem;
		relationWordItem_ = relationWordItem;


		// Protected constructed variables

		isConditionCheckedForSolving = false;

		if( specificationString != NULL )
			{
			if( ( specificationStringLength = strlen( specificationString ) ) < MAX_SENTENCE_STRING_LENGTH )
				{
				if( ( specificationString_ = new char[specificationStringLength + 1] ) != NULL )
					strcpy( specificationString_, specificationString );
				else
					startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create a specification string" );
				}
			else
				startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given specification string is too long" );
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
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, specificationString_ );
			}
		}

	virtual void displayWordReferences( bool isReturnQueryToPosition )
		{
		char *queryString;
		char *wordString;

		statusString[0] = statusChar();

		queryString = commonVariables()->queryString;

		if( generalizationWordItem_ != NULL &&
		( wordString = generalizationWordItem_->wordTypeString( true, generalizationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}

		if( specificationWordItem_ != NULL &&
		( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery ||
			strlen( queryString ) > 0 )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( queryString, wordString );
			}

		if( relationWordItem_ != NULL &&
		( wordString = relationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( commonVariables()->hasFoundQuery ||
			strlen( queryString ) > 0 )
				strcat( queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( queryString, statusString );

			commonVariables()->hasFoundQuery = true;
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

	virtual ResultType checkForUsage()
		{
		return myWordItem()->checkSelectionForUsageInWord( this );
		}

	virtual StringResultType findMatchingWordReferenceString( char *queryString )
		{
		StringResultType stringResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		if( generalizationWordItem_ != NULL )
			{
			if( ( stringResult = generalizationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				return addStringResultError( functionNameString, NULL, specificationString_, "I failed to find a matching word reference string for the generalization word" );
			}

		if( !stringResult.hasFoundMatchingStrings &&
		specificationWordItem_ != NULL )
			{
			if( ( stringResult = specificationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				return addStringResultError( functionNameString, NULL, specificationString_, "I failed to find a matching word reference string for the specification word" );
			}

		if( !stringResult.hasFoundMatchingStrings &&
		relationWordItem_ != NULL )
			{
			if( ( stringResult = relationWordItem_->findMatchingWordReferenceString( queryString ) ).result != RESULT_OK )
				return addStringResultError( functionNameString, NULL, specificationString_, "I failed to find a matching word reference string for the relation word" );
			}

		return stringResult;
		}

	virtual char *itemString()
		{
		return specificationString_;
		}

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *queryString;
		char *wordString;
		char *generalizationWordTypeString = myWordItem()->wordTypeNameString( generalizationWordTypeNr_ );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );
		char *relationWordTypeString = myWordItem()->wordTypeNameString( relationWordTypeNr_ );

		Item::toString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

		if( isAction_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isAction" );
			}

		if( isAssignedOrClear_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isAssignedOrClear" );
			}

		if( isNewStart_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isNewStart" );
			}

		if( isInactive_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isInactiveAssignment" );
			}

		if( isArchived_ )
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


	// Protected functions

	bool isNumeralRelation()
		{
		return ( relationWordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isAction()
		{
		return isAction_;
		}

	bool isAssignedOrClear()
		{
		return isAssignedOrClear_;
		}

	bool isNewStart()
		{
		return isNewStart_;
		}

	bool isInactiveAssignment()
		{
		return isInactive_;
		}

	bool isArchivedAssignment()
		{
		return isArchived_;
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

	ResultType findNextExecutionSelectionItem( WordItem *solveGeneralizationWordItem )
		{
		return findNextExecutionSelectionItem( false, solveGeneralizationWordItem );
		}

	ResultType findNextExecutionSelectionItem( bool isIncludingThisItem, WordItem *solveWordItem )
		{
		SelectionItem *searchSelectionItem = ( isIncludingThisItem ? this : nextSelectionItem() );
		char functionNameString[FUNCTION_NAME_LENGTH] = "findNextExecutionSelectionItem";

		nextExecutionItem_ = NULL;

		if( solveWordItem == NULL )
			return startError( functionNameString, NULL, specificationString_, "The given solve word item is undefined" );

		while( searchSelectionItem != NULL &&
		nextExecutionItem_ == NULL )
			{
			if( searchSelectionItem->generalizationWordItem_ == solveWordItem &&
			searchSelectionItem->specificationWordItem_ != NULL )
				nextExecutionItem_ = searchSelectionItem;
			else
				searchSelectionItem = searchSelectionItem->nextSelectionItem();
			}

		return RESULT_OK;
		}

	char *specificationString()
		{
		return specificationString_;
		}

	SelectionItem *nextExecutionItem()
		{
		return nextExecutionItem_;
		}

	SelectionItem *nextSelectionItem()
		{
		return (SelectionItem *)nextItem;
		}

	SelectionItem *nextConditionItem( unsigned short executionLevel, unsigned int conditionSentenceNr )
		{
		if( nextSelectionItem() != NULL &&
		nextSelectionItem()->selectionLevel_ <= executionLevel &&
		nextSelectionItem()->creationSentenceNr() == conditionSentenceNr )
			return nextSelectionItem();

		return NULL;
		}

	SelectionItem *nextExecutionItem( unsigned short executionLevel, unsigned int executionSentenceNr )
		{
		if( nextSelectionItem() != NULL &&
		nextSelectionItem()->selectionLevel_ == executionLevel &&
		nextSelectionItem()->creationSentenceNr() == executionSentenceNr )
			return nextSelectionItem();

		return NULL;
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
