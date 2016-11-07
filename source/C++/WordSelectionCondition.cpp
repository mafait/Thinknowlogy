/*	Class:			WordSelectionCondition
 *	Supports class:	AdminItem
 *	Purpose:		To check the condition of selections
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

#include "SelectionItem.cpp"
#include "SpecificationItem.cpp"

class WordSelectionCondition
	{
	friend class WordItem;

	// Private constructed variables

	bool hasFoundScoringAssignment_;
	bool isConditionSatisfied_;

	unsigned int oldSatisfiedScore_;
	unsigned int newSatisfiedScore_;
	unsigned int oldDissatisfiedScore_;
	unsigned int newDissatisfiedScore_;
	unsigned int oldNotBlockingScore_;
	unsigned int newNotBlockingScore_;
	unsigned int oldBlockingScore_;
	unsigned int newBlockingScore_;

	SpecificationItem *comparisonAssignmentItem_;
	SpecificationItem *firstComparisonAssignmentItem_;
	SpecificationItem *secondComparisonAssignmentItem_;

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool hasFoundScoringAssignment( bool isBlocking )
		{
		SpecificationItem *currentAssignmentItem;

		if( ( currentAssignmentItem = myWordItem_->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			do	{
				if( !currentAssignmentItem->isNegative() )
					{
					if( isBlocking )
						{
						if( currentAssignmentItem->isOlderItem() )
							oldBlockingScore_++;
						else
							newBlockingScore_++;
						}
					else
						{
						if( currentAssignmentItem->isOlderItem() )
							oldNotBlockingScore_++;
						else
							newNotBlockingScore_++;
						}

					return true;
					}
				}
			while( ( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return false;
		}

	bool isNumeralString( char *checkString )
		{
		size_t stringLength;
		size_t position = 0;

		if( checkString != NULL &&
		( stringLength = strlen( checkString ) ) > 0 )
			{
			while( position < stringLength &&
			isdigit( checkString[position] ) )
				position++;

			return ( position == stringLength );
			}

		return false;
		}

	ResultType getComparisonAssignment( bool isNumeralRelation, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getComparisonAssignment";

		comparisonAssignmentItem_ = NULL;

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( isNumeralRelation )
			comparisonAssignmentItem_ = specificationWordItem->firstActiveNumeralAssignmentItem();
		else
			{
			if( relationWordItem == NULL )
				comparisonAssignmentItem_ = specificationWordItem->firstActiveStringAssignmentItem();
			else
				{
				if( specificationWordItem->isNounHead() )
					comparisonAssignmentItem_ = relationWordItem->lastActiveNonQuestionAssignmentItem();
				else
					{
					if( specificationWordItem->isNounTail() )
						comparisonAssignmentItem_ = relationWordItem->firstNonQuestionActiveAssignmentItem();
					else
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	ResultType checkComparison( bool isFirstComparisonPart, bool isNegative, bool isNumeralRelation, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		int comparisonResult = 0;
		int firstNumeral = 0;
		int secondNumeral = 0;
		char *firstString = NULL;
		char *secondString = NULL;
		WordItem *firstSpecificationWordItem;
		WordItem *secondSpecificationWordItem;
		WordItem *comparisonAssignmentSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkComparison";

		isConditionSatisfied_ = false;

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// First part
		if( isFirstComparisonPart )
			{
			if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed get the first comparison assignment" );

			// Allow the second part of the comparison to be checked
			isConditionSatisfied_ = true;

			firstComparisonAssignmentItem_ = comparisonAssignmentItem_;
			secondComparisonAssignmentItem_ = NULL;
			}
		else
			{
			if( !isNumeralRelation &&
			// Second part
			specificationString == NULL )
				{
				if( getComparisonAssignment( false, specificationWordItem, relationWordItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed get a comparison assignment" );

				secondComparisonAssignmentItem_ = comparisonAssignmentItem_;

				firstSpecificationWordItem = ( firstComparisonAssignmentItem_ == NULL ? NULL : firstComparisonAssignmentItem_->specificationWordItem() );
				secondSpecificationWordItem = ( secondComparisonAssignmentItem_ == NULL ? NULL : secondComparisonAssignmentItem_->specificationWordItem() );

				firstString = ( firstSpecificationWordItem == NULL ? NULL : firstSpecificationWordItem->anyWordTypeString() );
				secondString = ( secondSpecificationWordItem == NULL ? NULL : secondSpecificationWordItem->anyWordTypeString() );
				}
			else
				{
				// Numeral or specification string
				if( getComparisonAssignment( isNumeralRelation, specificationWordItem, relationWordItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed get the first comparison assignment" );

				if( comparisonAssignmentItem_ != NULL )
					comparisonAssignmentSpecificationWordItem = comparisonAssignmentItem_->specificationWordItem();

				firstString = ( comparisonAssignmentItem_ == NULL ? NULL : ( isNumeralRelation ? ( comparisonAssignmentSpecificationWordItem == NULL ? NULL : comparisonAssignmentSpecificationWordItem->anyWordTypeString() ) : comparisonAssignmentItem_->specificationString() ) );
				secondString = ( isNumeralRelation ? ( relationWordItem == NULL ? NULL : relationWordItem->anyWordTypeString() ) : specificationString );
				}

			if( firstString == NULL ||
			secondString == NULL )
				comparisonResult = ( firstString == NULL && secondString == NULL ? 0 : ( firstString == NULL ? -1 : 1 ) );
			else
				{
				if( isNumeralString( firstString ) &&
				isNumeralString( secondString ) )
					{
					sscanf( firstString, "%d", &firstNumeral );
					sscanf( secondString, "%d", &secondNumeral );

					comparisonResult = ( firstNumeral == secondNumeral ? 0 : ( firstNumeral < secondNumeral ? -1 : 1 ) );
					}
				else
					comparisonResult = strcmp( firstString, secondString );
				}

			if( myWordItem_->isAdjectiveComparisonLess() )
				isConditionSatisfied_ = ( comparisonResult < 0 ? !isNegative : isNegative );
			else
				{
				if( myWordItem_->isAdjectiveComparisonEqual() )
					isConditionSatisfied_ = ( comparisonResult == 0 ? !isNegative : isNegative );
				else
					{
					if( !myWordItem_->isAdjectiveComparisonMore() )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Word \"", myWordItem_->anyWordTypeString(), "\" isn't comparison word" );

					isConditionSatisfied_ = ( comparisonResult > 0 ? !isNegative : isNegative );
					}
				}
			}

		return RESULT_OK;
		}

	ResultType checkForOddOrEven( bool isNegative, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		unsigned int nAssignments;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForOddOrEven";

		isConditionSatisfied_ = false;

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( specificationWordItem->isNounNumber() )
			{
			nAssignments = relationWordItem->nActiveAssignments();

			if( myWordItem_->isAdjectiveOdd() )
				isConditionSatisfied_ = ( nAssignments % 2 == 1 ? !isNegative : isNegative );
			else
				{
				if( !myWordItem_->isAdjectiveEven() )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "Word \"", myWordItem_->anyWordTypeString(), "\" isn't about odd or even" );

				isConditionSatisfied_ = ( nAssignments % 2 == 0 ? !isNegative : isNegative );
				}
			}
		else
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	ResultType findScoringAssignment( bool isPossessive, bool isSatisfiedScore, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *currentAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findScoringAssignment";

		hasFoundScoringAssignment_ = false;

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentAssignmentItem = myWordItem_->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			do	{
				if( currentAssignmentItem->isRelatedSpecification( false, isPossessive, relationContextNr, specificationWordItem ) )
					{
					hasFoundScoringAssignment_ = true;

					if( isSatisfiedScore )
						{
						if( currentAssignmentItem->isOlderItem() )
							oldSatisfiedScore_++;
						else
							newSatisfiedScore_++;
						}
					else
						{
						if( currentAssignmentItem->isOlderItem() )
							oldDissatisfiedScore_++;
						else
							newDissatisfiedScore_++;
						}
					}
				}
			while( !hasFoundScoringAssignment_ &&
			( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType checkConditionByValue( bool isNegative, bool isPossessive, WordItem *specificationWordItem )
		{
		bool isSatisfiedScore;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkConditionByValue";

		isConditionSatisfied_ = true;

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			do	{
				foundAssignmentItem = specificationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, isPossessive, currentSpecificationItem->relationContextNr(), currentSpecificationItem->specificationWordItem() );
				isSatisfiedScore = ( isNegative == ( foundAssignmentItem == NULL || foundAssignmentItem->isNegative() ) );

				if( findScoringAssignment( isPossessive, isSatisfiedScore, currentSpecificationItem->relationContextNr(), currentSpecificationItem->specificationWordItem() ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a scoring assignment" );

				if( hasFoundScoringAssignment_ != isSatisfiedScore )
					isConditionSatisfied_ = false;
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

	WordSelectionCondition( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundScoringAssignment_ = false;
		isConditionSatisfied_ = false;

		oldSatisfiedScore_ = NO_SCORE;
		newSatisfiedScore_ = NO_SCORE;
		oldDissatisfiedScore_ = NO_SCORE;
		newDissatisfiedScore_ = NO_SCORE;
		oldNotBlockingScore_ = NO_SCORE;
		newNotBlockingScore_ = NO_SCORE;
		oldBlockingScore_ = NO_SCORE;
		newBlockingScore_ = NO_SCORE;

		comparisonAssignmentItem_ = NULL;
		firstComparisonAssignmentItem_ = NULL;
		secondComparisonAssignmentItem_ = NULL;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordSelectionCondition" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	SelectionResultType checkSelectionCondition( SelectionItem *conditionSelectionItem )
		{
		SelectionResultType selectionResult;
		bool isNegative;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSelectionCondition";

		isConditionSatisfied_ = false;

		oldSatisfiedScore_ = NO_SCORE;
		newSatisfiedScore_ = NO_SCORE;
		oldDissatisfiedScore_ = NO_SCORE;
		newDissatisfiedScore_ = NO_SCORE;
		oldNotBlockingScore_ = NO_SCORE;
		newNotBlockingScore_ = NO_SCORE;
		oldBlockingScore_ = NO_SCORE;
		newBlockingScore_ = NO_SCORE;

		if( conditionSelectionItem == NULL )
			return myWordItem_->startSelectionResultErrorInWord( functionNameString, moduleNameString_, "The given condition selection item is undefined" );

		if( ( specificationWordItem = conditionSelectionItem->specificationWordItem() ) == NULL )
			return myWordItem_->startSelectionResultErrorInWord( functionNameString, moduleNameString_, "The given condition selection item has no specification word" );

		isNegative = conditionSelectionItem->isNegative();

		if( conditionSelectionItem->isAssignedOrClear() )
			{
			if( specificationWordItem->isAdjectiveEmpty() )
				// Adjective "clear"
				isConditionSatisfied_ = ( hasFoundScoringAssignment( !isNegative ) ? isNegative : !isNegative );
			else
				// Adjective "assigned"
				isConditionSatisfied_ = ( hasFoundScoringAssignment( isNegative ) ? !isNegative : isNegative );
			}
		else
			{
			if( myWordItem_->isAdjectiveComparison() )
				{
				if( checkComparison( conditionSelectionItem->isFirstComparisonPart(), isNegative, conditionSelectionItem->isNumeralRelation(), specificationWordItem, conditionSelectionItem->relationWordItem(), conditionSelectionItem->specificationString() ) != RESULT_OK )
					return myWordItem_->addSelectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to check a comparison" );
				}
			else
				{
				if( myWordItem_->isAdjectiveOddOrEven() )
					{
					if( checkForOddOrEven( isNegative, specificationWordItem, conditionSelectionItem->relationWordItem() ) != RESULT_OK )
						return myWordItem_->addSelectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for odd or even" );
					}
				else
					{
					if( conditionSelectionItem->isValueSpecification() )
						{
						if( checkConditionByValue( isNegative, conditionSelectionItem->isPossessive(), specificationWordItem ) != RESULT_OK )
							return myWordItem_->addSelectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the condition of a specification by value" );
						}
					else
						{
						if( findScoringAssignment( conditionSelectionItem->isPossessive(), !isNegative, conditionSelectionItem->relationContextNr(), specificationWordItem ) != RESULT_OK )
							return myWordItem_->addSelectionResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a scoring assignment" );

						if( hasFoundScoringAssignment_ != isNegative )
							isConditionSatisfied_ = true;
						}
					}
				}
			}

		selectionResult.isConditionSatisfied = isConditionSatisfied_;

		selectionResult.oldSatisfiedScore = oldSatisfiedScore_;
		selectionResult.newSatisfiedScore = newSatisfiedScore_;
		selectionResult.oldDissatisfiedScore = oldDissatisfiedScore_;
		selectionResult.newDissatisfiedScore = newDissatisfiedScore_;
		selectionResult.oldNotBlockingScore = oldNotBlockingScore_;
		selectionResult.newNotBlockingScore = newNotBlockingScore_;
		selectionResult.oldBlockingScore = oldBlockingScore_;
		selectionResult.newBlockingScore = newBlockingScore_;

		return selectionResult;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *
 *	Praise God in his sanctuary;
 *	praise him in his mighty heaven!" (Psalm 150:1)
 *************************************************************************/
