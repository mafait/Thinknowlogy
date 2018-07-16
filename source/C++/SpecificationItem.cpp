/*	Class:			SpecificationItem
 *	Purpose:		To store info about the specification structure
 *					of a word
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

#ifndef SPECIFICATIONITEM
#define SPECIFICATIONITEM 1
#include "CreateAndAssignResultType.cpp"
#include "InputOutput.cpp"
#include "List.h"
#include "ShortResultType.cpp"
#include "SpecificationResultType.cpp"

class SpecificationItem : private Item
	{
	friend class AdminImperative;
	friend class AdminReadFile;
	friend class AdminReasoningNew;
	friend class AdminReasoningOld;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class JustificationItem;
	friend class JustificationList;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordWrite;

	// Private initialized variables

	bool isAnsweredQuestion_;
	bool isCharacteristicFor_;
	bool isConcludedAssumption_;
	bool isConditional_;
	bool isCorrectedAssumption_;
	bool isEveryGeneralization_;
	bool isExclusiveGeneralization_;
	bool isExclusiveSpecification_;
	bool isLanguageWord_;
	bool isNegative_;
	bool isPartOf_;
	bool isPossessive_;
	bool isSpecific_;
	bool isSpecificationGeneralization_;
	bool isUncountableGeneralizationNoun_;
	bool isUniqueUserRelation_;
	bool isValueSpecification_;

	unsigned short assignmentLevel_;
	unsigned short assumptionLevel_;
	unsigned short languageNr_;
	unsigned short prepositionParameter_;
	unsigned short questionParameter_;

	unsigned short generalizationWordTypeNr_;
	unsigned short specificationWordTypeNr_;
	unsigned short relationWordTypeNr_;

	unsigned int generalizationCollectionNr_;
	unsigned int specificationCollectionNr_;

	unsigned int generalizationContextNr_;
	unsigned int specificationContextNr_;
	unsigned int relationContextNr_;

	unsigned int nContextRelations_;

	JustificationItem *firstJustificationItem_;

	WordItem *specificationWordItem_;

	char *specificationString_;

	char storedSentenceString_[MAX_SENTENCE_STRING_LENGTH];
	char storedSentenceWithOnlyOneSpecificationString_[MAX_SENTENCE_STRING_LENGTH];


	// Private constructed variables

	bool hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_;
	bool hasSpecificationBeenWrittenAsConflict_;

	unsigned short specificationStringWriteLevel_;


	// Private assignment functions

	SpecificationItem *nextAssignmentItemWithAssignmentLevel( unsigned short currentAssignmentLevel )
		{
		SpecificationItem *nextAssignmentItem = nextSpecificationItem();

		if( nextAssignmentItem != NULL &&
		nextAssignmentItem->assignmentLevel() == currentAssignmentLevel )
			return nextAssignmentItem;

		return NULL;
		}


	// Private specification functions

	bool hasContextMaleWordInContextWords()
		{
		WordItem *currentContextWordItem;
		WordItem *thisWordItem = myWordItem();

		if( relationContextNr_ > NO_CONTEXT_NR &&
		specificationWordItem_ != NULL &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != thisWordItem &&
				currentContextWordItem->isMale() &&
				currentContextWordItem->hasContextInWord( relationContextNr_, specificationWordItem_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return false;
		}

	bool hasUserSpecificationWordItem()
		{
		return ( specificationWordItem_ != NULL &&
				specificationWordItem_->isUserSpecificationWord );
		}

	bool isPrimarySpecificationCandidateOfQuestion( bool isAllowingSpanishPossessiveSpecification )
		{
		return ( !isExclusiveSpecification_ &&
				!isNegative_ &&
				!isPartOf_ &&

				( !isPossessive_ ||
				isAllowingSpanishPossessiveSpecification ) &&

				questionParameter_ == NO_QUESTION_PARAMETER &&
				!isHiddenSpanishSpecification() );
		}

	signed char recalculateAssumptionsInContextWords( unsigned int contextNr, WordItem *specificationWordItem )
		{
		WordItem *currentContextWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptionsInContextWords";

		if( contextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			do	{
				if( currentContextWordItem->hasContextInWord( contextNr, specificationWordItem ) &&
				currentContextWordItem->recalculateAssumptionsInWord() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to recalculate the assumptions of word \"", currentContextWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	WordItem *relationContextWordItemInContextWords( WordItem *previousContextWordItem )
		{
		WordItem *currentContextWordItem;

		if( relationContextNr_ > NO_CONTEXT_NR &&
		// Do for all (remaining) context words
		( currentContextWordItem = ( previousContextWordItem == NULL ? globalVariables()->firstContextWordItem : previousContextWordItem->nextContextWordItem ) ) != NULL )
			{
			do	{
				if( currentContextWordItem->hasContextInWord( relationContextNr_, specificationWordItem_ ) )
					return currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return NULL;
		}


	protected:
	// Protected constructed variables

	bool hasSpecificationBeenWrittenAsAnswer;

	SpecificationItem *replacingSpecificationItem;


	// Constructor

	SpecificationItem( bool isAnsweredQuestion, bool isCharacteristicFor, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isLanguageWord, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		size_t specificationStringLength;

		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, "SpecificationItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		isAnsweredQuestion_ = isAnsweredQuestion;
		isCharacteristicFor_ = isCharacteristicFor;
		isConcludedAssumption_ = isConcludedAssumption;
		isConditional_ = isConditional;
		isCorrectedAssumption_ = isCorrectedAssumption;
		isEveryGeneralization_ = isEveryGeneralization;
		isExclusiveGeneralization_ = isExclusiveGeneralization;
		isExclusiveSpecification_ = isExclusiveSpecification;
		isLanguageWord_ = isLanguageWord;
		isNegative_ = isNegative;
		isPartOf_ = isPartOf;
		isPossessive_ = isPossessive;
		isSpecific_ = isSpecific;
		isSpecificationGeneralization_ = isSpecificationGeneralization;
		isUncountableGeneralizationNoun_ = isUncountableGeneralizationNoun;
		isUniqueUserRelation_ = isUniqueUserRelation;
		isValueSpecification_ = isValueSpecification;

		assignmentLevel_ = assignmentLevel;
		assumptionLevel_ = assumptionLevel;
		languageNr_ = languageNr;
		prepositionParameter_ = prepositionParameter;
		questionParameter_ = questionParameter;

		generalizationWordTypeNr_ = generalizationWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;
		relationWordTypeNr_ = relationWordTypeNr;

		generalizationCollectionNr_ = generalizationCollectionNr;
		specificationCollectionNr_ = specificationCollectionNr;

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nContextRelations_ = nContextRelations;

		firstJustificationItem_ = firstJustificationItem;

		specificationWordItem_ = specificationWordItem;

		specificationString_ = NULL;

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


		// Private constructed variables

		hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = false;
		hasSpecificationBeenWrittenAsConflict_ = false;

		specificationStringWriteLevel_ = NO_WRITE_LEVEL;

		// Protected constructed variables

		hasSpecificationBeenWrittenAsAnswer = false;

		replacingSpecificationItem = NULL;

		strcpy( storedSentenceString_, ( storedSentenceString == NULL ? EMPTY_STRING : storedSentenceString ) );
		strcpy( storedSentenceWithOnlyOneSpecificationString_, ( storedSentenceWithOnlyOneSpecificationString == NULL ? EMPTY_STRING : storedSentenceWithOnlyOneSpecificationString ) );
		}

	~SpecificationItem()
		{
		if( specificationString_ != NULL )
			delete specificationString_;
		}


	// Protected virtual functions

	virtual void checkForUsage()
		{
		myWordItem()->checkSpecificationForUsageOfInvolvedWords( this );
		}

	virtual void selectingAttachedJustifications( bool isSelectingJustificationSpecifications )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			searchJustificationItem->isSelectedByQuery = true;

			if( isSelectingJustificationSpecifications )
				searchJustificationItem->selectingJustificationSpecifications();

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}
		}

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
		char *wordString;

		statusString[0] = statusChar();

		if( specificationWordItem_ != NULL &&
		( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables()->queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( globalVariables()->queryString, wordString );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( prepositionParameter_ == queryParameter ||
				questionParameter_ == queryParameter ||
				generalizationCollectionNr_ == queryParameter ||
				specificationCollectionNr_ == queryParameter ||
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nContextRelations_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( prepositionParameter_ > NO_PREPOSITION_PARAMETER ||
				questionParameter_ > NO_QUESTION_PARAMETER ||
				generalizationCollectionNr_ > NO_COLLECTION_NR ||
				specificationCollectionNr_ > NO_COLLECTION_NR ||
				generalizationContextNr_ > NO_CONTEXT_NR ||
				specificationContextNr_ > NO_CONTEXT_NR ||
				relationContextNr_ > NO_CONTEXT_NR ||
				nContextRelations_ > 0 ) ) );
		}

	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( firstJustificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : firstJustificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : firstJustificationItem_->itemNr() == queryItemNr ) ) ||

				( specificationWordItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : specificationWordItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : specificationWordItem_->itemNr() == queryItemNr ) ) ||

				( replacingSpecificationItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : replacingSpecificationItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : replacingSpecificationItem->itemNr() == queryItemNr ) ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( generalizationWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr	||
				relationWordTypeNr_ == queryWordTypeNr	);
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		SpecificationItem *nextSortSpecificationItem = (SpecificationItem *)nextSortItem;

		return ( nextSortItem != NULL &&

				// 1) Assignment needs descending assignmentLevel_
				( assignmentLevel_ > nextSortSpecificationItem->assignmentLevel_ ||

				// 2) Question and specification needs descending creationSentenceNr()
				( assignmentLevel_ == nextSortSpecificationItem->assignmentLevel_ &&
				creationSentenceNr() > nextSortSpecificationItem->creationSentenceNr() ) ) );
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		char *languageNameString = myWordItem()->languageNameString( languageNr_ );
		char *generalizationWordTypeString = myWordItem()->wordTypeNameString( generalizationWordTypeNr_ );
		char *queryString;
		char *relationWordTypeString = ( relationWordTypeNr_ == NO_WORD_TYPE_NR ? NULL : myWordItem()->wordTypeNameString( relationWordTypeNr_ ) );
		char *specificationWordTypeString = myWordItem()->wordTypeNameString( specificationWordTypeNr_ );
		char *wordString;

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( languageNr_ > NO_LANGUAGE_NR )
			{
			if( languageNameString == NULL )
				sprintf( tempString, "%clanguageNr:%u", QUERY_SEPARATOR_CHAR, languageNr_ );
			else
				sprintf( tempString, "%clanguage:%s", QUERY_SEPARATOR_CHAR, languageNameString );

			strcat( queryString, tempString );
			}

		if( isAnsweredQuestion_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isAnsweredQuestion" );
			}

		if( isCharacteristicFor_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isCharacteristicFor" );
			}

		if( isConcludedAssumption_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isConcludedAssumption" );
			}

		if( isConditional_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isConditional" );
			}

		if( isCorrectedAssumption_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isCorrectedAssumption" );
			}

		if( isEveryGeneralization_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isEveryGeneralization" );
			}

		if( isExclusiveGeneralization_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isExclusiveGeneralization" );
			}

		if( isExclusiveSpecification_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isExclusiveSpecification" );
			}

		if( isLanguageWord_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isLanguageWord" );
			}

		if( isNegative_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isNegative" );
			}

		if( isPartOf_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isPartOf" );
			}

		if( isPossessive_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isPossessive" );
			}

		if( isSpecific_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSpecific" );
			}

		if( isSpecificationGeneralization_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSpecificationGeneralization" );
			}

		if( isUncountableGeneralizationNoun_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isUncountableGeneralizationNoun" );
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

		if( assignmentLevel_ > NO_ASSIGNMENT_LEVEL )
			{
			sprintf( tempString, "%cassignmentLevel:%u", QUERY_SEPARATOR_CHAR, assignmentLevel_ );
			strcat( queryString, tempString );
			}

		if( assumptionLevel_ > NO_ASSUMPTION_LEVEL )
			{
			sprintf( tempString, "%cassumptionLevel:%u", QUERY_SEPARATOR_CHAR, assumptionLevel_ );
			strcat( queryString, tempString );
			}

		if( specificationStringWriteLevel_ > NO_WRITE_LEVEL )
			{
			sprintf( tempString, "%cspecificationStringWriteLevel:%u", QUERY_SEPARATOR_CHAR, specificationStringWriteLevel_ );
			strcat( queryString, tempString );
			}

		if( prepositionParameter_ > NO_PREPOSITION_PARAMETER )
			{
			sprintf( tempString, "%cprepositionParameter:%u", QUERY_SEPARATOR_CHAR, prepositionParameter_ );
			strcat( queryString, tempString );
			}

		if( questionParameter_ > NO_QUESTION_PARAMETER )
			{
			sprintf( tempString, "%cquestionParameter:%u", QUERY_SEPARATOR_CHAR, questionParameter_ );
			strcat( queryString, tempString );
			}

		if( generalizationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%cgeneralizationCollectionNr:%u", QUERY_SEPARATOR_CHAR, generalizationCollectionNr_ );
			strcat( queryString, tempString );
			}

		if( generalizationWordTypeString == NULL )
			sprintf( tempString, "%cgeneralizationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );
		else
			sprintf( tempString, "%cgeneralizationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, generalizationWordTypeString, QUERY_WORD_TYPE_CHAR, generalizationWordTypeNr_ );

		strcat( queryString, tempString );

		if( generalizationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cgeneralizationContextNr:%u", QUERY_SEPARATOR_CHAR, generalizationContextNr_ );
			strcat( queryString, tempString );
			}

		if( specificationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%cspecificationCollectionNr:%u", QUERY_SEPARATOR_CHAR, specificationCollectionNr_ );
			strcat( queryString, tempString );
			}

		if( specificationWordTypeString == NULL )
			sprintf( tempString, "%cspecificationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );
		else
			sprintf( tempString, "%cspecificationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, specificationWordTypeString, QUERY_WORD_TYPE_CHAR, specificationWordTypeNr_ );

		strcat( queryString, tempString );

		if( specificationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cspecificationContextNr:%u", QUERY_SEPARATOR_CHAR, specificationContextNr_ );
			strcat( queryString, tempString );
			}

		if( specificationWordItem_ != NULL )
			{
			sprintf( tempString, "%cspecificationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, specificationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, specificationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = specificationWordItem_->wordTypeString( true, specificationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( relationWordTypeString != NULL )
			{
			sprintf( tempString, "%crelationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, relationWordTypeString, QUERY_WORD_TYPE_CHAR, relationWordTypeNr_ );
			strcat( queryString, tempString );
			}

		if( relationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%crelationContextNr:%u", QUERY_SEPARATOR_CHAR, relationContextNr_ );
			strcat( queryString, tempString );
			}

		if( nContextRelations_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cnContextRelations:%u", QUERY_SEPARATOR_CHAR, nContextRelations_ );
			strcat( queryString, tempString );
			}

		if( firstJustificationItem_ != NULL )
			{
			sprintf( tempString, "%cfirstJustificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, firstJustificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, firstJustificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( replacingSpecificationItem != NULL )
			{
			sprintf( tempString, "%creplacingSpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, replacingSpecificationItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, replacingSpecificationItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( specificationString_ != NULL )
			{
			sprintf( tempString, "%cspecificationString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, specificationString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		if( strlen( storedSentenceString_ ) > 0 )
			{
			sprintf( tempString, "%cstoredSentenceString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, storedSentenceString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		if( strlen( storedSentenceWithOnlyOneSpecificationString_ ) > 0 )
			{
			sprintf( tempString, "%cstoredSentenceWithOnlyOneSpecificationString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, storedSentenceWithOnlyOneSpecificationString_, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}

	virtual BoolResultType findMatchingWordReferenceString( char *queryString )
		{
		BoolResultType boolResult;

		if( specificationWordItem_ != NULL )
			return specificationWordItem_->findMatchingWordReferenceString( queryString );

		return boolResult;
		}


	// Protected assignment functions

	bool isAssignment()
		{
		return myList()->isAssignmentList();
		}

	bool isUserAssignment()
		{
		return ( isAssignment() &&
				isUserSpecification() );
		}

	bool isActiveAssignment()
		{
		return ( isAssignment() &&
				isActiveItem() );
		}

	bool isInactiveAssignment()
		{
		return ( isAssignment() &&
				isInactiveItem() );
		}

	bool isArchivedAssignment()
		{
		return ( isAssignment() &&
				isArchivedItem() );
		}

	unsigned short assignmentLevel()
		{
		return assignmentLevel_;
		}

	unsigned short assumptionLevel()
		{
		return assumptionLevel_;
		}

	unsigned short languageNr()
		{
		return languageNr_;
		}

	unsigned int nRelationContextWords()
		{
		return myWordItem()->nContextWords( relationContextNr_, specificationWordItem_ );
		}

	SpecificationItem *assignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, bool isQuestion )
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isQuestion() == isQuestion &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel );
			}

		return NULL;
		}

	SpecificationItem *assignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, unsigned short questionParameter )
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? ( assignmentLevel_ == currentAssignmentLevel ? this : NULL ) : nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->questionParameter_ == questionParameter &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel );
			}

		return NULL;
		}


	// Protected question functions

	bool isAnsweredQuestion()
		{
		return isAnsweredQuestion_;
		}

	bool isQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER );
		}

	bool isUserQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == NULL );
		}

	unsigned short questionParameter()
		{
		return questionParameter_;
		}


	// Protected specification functions

	void clearReplacingInfo()
		{
		if( hasCurrentReplacedSentenceNr() ||

		( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->hasCurrentOrNewerCreationSentenceNr() ) )
			{
			replacingSpecificationItem = NULL;
			clearReplacedSentenceNr();
			}
		}

	void clearStoredSentenceString()
		{
		strcpy( storedSentenceString_, EMPTY_STRING );
		}

	void initializeSpecificationVariables()
		{
		// Private constructed variables

		hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = false;
		hasSpecificationBeenWrittenAsConflict_ = false;

		// Protected constructed variables

		hasSpecificationBeenWrittenAsAnswer = false;
		}

	void incrementSpanishAssumptionLevel()
		{
		if( assumptionLevel_ < MAX_LEVEL )
			assumptionLevel_++;
		}

	void storeWrittenSentenceString()
		{
		strcpy( storedSentenceString_, globalVariables()->writtenSentenceString );
		}

	void storedWrittenSentenceStringWithOneSpecificationOnly()
		{
		strcpy( storedSentenceWithOnlyOneSpecificationString_, globalVariables()->writtenSentenceString );
		}

	bool hasAssumptionLevel()
		{
		return ( assumptionLevel_ > NO_ASSUMPTION_LEVEL );
		}

	bool hasCompoundSpecificationCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->isCompoundCollection( specificationCollectionNr_ ) );
		}

	bool hasJustification( JustificationItem *referenceJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( referenceJustificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem == referenceJustificationItem )
					return true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool hasJustificationOfSameType( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		unsigned int primarySpecificationCollectionNr = ( primarySpecificationItem == NULL ? NO_COLLECTION_NR : primarySpecificationItem->specificationCollectionNr() );
		unsigned int secondarySpecificationCollectionNr = ( secondarySpecificationItem == NULL ? NO_COLLECTION_NR : secondarySpecificationItem->specificationCollectionNr() );
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->primarySpecificationCollectionNr() == primarySpecificationCollectionNr &&
			searchJustificationItem->secondarySpecificationCollectionNr() == secondarySpecificationCollectionNr )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasGeneralizationCollection()
		{
		return ( generalizationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasNewInformation()
		{
		return ( !isOlderItem() ||
				// Has been inactivated during this sentence
				hasCurrentInactiveSentenceNr() ||
				// Has been archived during this sentence
				hasCurrentArchivedSentenceNr() ||
				hasRelationContextCurrentlyBeenUpdated() );
		}

	bool hasNonCompoundSpecificationCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->isNonCompoundCollection( specificationCollectionNr_ ) );
		}

	bool hasNonExclusiveSpecificationCollection()
		{
		return ( specificationWordItem_ != NULL &&
				specificationWordItem_->hasNonExclusiveCollection( specificationCollectionNr_ ) );
		}

	bool hasOnlyOneRelationWord()
		{
		return ( myWordItem()->nContextWords( relationContextNr_, specificationWordItem_ ) == 1 );
		}

	bool hasPossessivePrimarySpecificationJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( !searchJustificationItem->hasPossessivePrimarySpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasPrimarySpecificationJustification( SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem )
					return true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool hasSpecificationContext()
		{
		return ( specificationContextNr_ > NO_CONTEXT_NR );
		}

	bool hasRelationContext()
		{
		return ( relationContextNr_ > NO_CONTEXT_NR );
		}

	bool hasRelationContextCurrentlyBeenUpdated()
		{
		WordItem *currentContextWordItem;
		WordItem *thisWordItem = myWordItem();

		if( relationContextNr_ > NO_CONTEXT_NR &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != thisWordItem &&
				currentContextWordItem->hasContextCurrentlyBeenUpdatedInWord( relationContextNr_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return false;
		}

	bool hasSpecificationBeenWrittenAsAdjustedNegativeAssumption()
		{
		return hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_;
		}

	bool hasSpecificationBeenWrittenAsConflict()
		{
		return hasSpecificationBeenWrittenAsConflict_;
		}

	bool hasSpecificationCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool isAnsweredSelfGeneratedQuestion()
		{
		return ( isAnsweredQuestion_ &&
				firstJustificationItem_ != NULL );
		}

	bool isCharacteristicFor()
		{
		return isCharacteristicFor_;
		}

	bool isCircular( WordItem *secondaryGeneralizationWordItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( secondaryGeneralizationWordItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( !searchJustificationItem->isOlderItem() &&
				searchJustificationItem->isPossessiveReversibleAssumption() &&
				searchJustificationItem->secondaryGeneralizationWordItem() == secondaryGeneralizationWordItem )
					return true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool isConcludedAssumption()
		{
		return isConcludedAssumption_;
		}

	bool isConditional()
		{
		return isConditional_;
		}

	bool isCorrectedAssumption()
		{
		return isCorrectedAssumption_;
		}

	bool isCorrectSpecificationArticle( bool isDefiniteArticle, unsigned short articleParameter )
		{
		bool isSelfGeneratedSpecification;
		SpecificationItem *searchSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *foundWordItem = NULL;

		if( ( searchSpecificationItem = myWordItem()->firstSpecificationItem( isAnsweredQuestion_, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != NULL )
			{
			isSelfGeneratedSpecification = isSelfGenerated();

			do	{
				if( ( currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ : searchSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGeneratedSpecification, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, relationContextNr_ ) ) ) != NULL &&
				!currentSpecificationWordItem->isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
					foundWordItem = currentSpecificationWordItem;
				}
			while( foundWordItem == NULL &&
			( searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion_ ) ) != NULL );

			return ( foundWordItem == NULL ? true : ( isDefiniteArticle ? foundWordItem->isCorrectDefiniteArticle( articleParameter, specificationWordTypeNr_ ) : foundWordItem->isCorrectIndefiniteArticle( articleParameter, specificationWordTypeNr_ ) ) );
			}

		return false;
		}

	bool isCorrectArticleOfFirstUnwrittenProperNounRelationWord( unsigned short definiteArticleParameter )
		{
		WordItem *firstProperNounUnwrittenRelationWordItem;

		return ( ( firstProperNounUnwrittenRelationWordItem = firstUnwrittenRelationWordItem( WORD_TYPE_PROPER_NOUN, NULL ) ) != NULL &&
				firstProperNounUnwrittenRelationWordItem->isProperNounPrecededByDefiniteArticle( definiteArticleParameter ) );
		}

	bool isExclusiveNonPossessiveGeneralizationAssignment()
		{
		return ( isAssignment() &&
				isExclusiveNonPossessiveGeneralization() );
		}

	bool isExclusiveNonPossessiveGeneralization()
		{
		return ( isExclusiveGeneralization_ &&
				!isPossessive_ );
		}

	bool isEveryGeneralization()
		{
		return isEveryGeneralization_;
		}

	bool isExclusiveGeneralization()
		{
		return isExclusiveGeneralization_;
		}

	bool isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	bool isGeneralizationNoun()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR ||
				generalizationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isGeneralizationProperNoun()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_PROPER_NOUN );
		}

	bool isHiddenSpanishSpecification()
		{
		return ( isSpecificationWordSpanishAmbiguous() &&
				hasNonCompoundSpecificationCollection() &&
				!isNegative_ &&
				isSelfGenerated() &&
				isGeneralizationProperNoun() &&

				// Non-possessive specification not in a male word
				( ( !isPossessive_ &&
				!myWordItem()->isMale() ) ||

				// Possessive specification without male relation context
				( isPossessive_ &&
				relationContextNr_ > NO_CONTEXT_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->compoundCollectionNr() > NO_COLLECTION_NR &&
				!hasContextMaleWordInContextWords() ) ) );
		}

	bool isMatchingGeneralizationContextNr( bool isAllowingEmptyRelationContext, unsigned int generalizationContextNr )
		{
		if( isAllowingEmptyRelationContext ||
		// Empty subset
		generalizationContextNr == NO_CONTEXT_NR ||
		// Same set
		generalizationContextNr_ == generalizationContextNr )
			return true;

		return myWordItem()->isContextSubsetInContextWords( generalizationContextNr_, generalizationContextNr );
		}

	bool isMatchingRelationContextNr( bool isAllowingEmptyRelationContext, unsigned int relationContextNr )
		{
		// Empty subset
		if( relationContextNr == NO_CONTEXT_NR ||
		// Same set
		relationContextNr_ == relationContextNr ||

		( isAllowingEmptyRelationContext &&
		relationContextNr_ == NO_CONTEXT_NR ) )
			return true;

		return myWordItem()->isContextSubsetInContextWords( relationContextNr_, relationContextNr );
		}

	bool isNegative()
		{
		return isNegative_;
		}

	bool isPartOf()
		{
		return isPartOf_;
		}

	bool isPossessive()
		{
		return isPossessive_;
		}

	bool isPossessiveReversibleConclusion()
		{
		return ( firstJustificationItem_ != NULL &&
				firstJustificationItem_->isPossessiveReversibleConclusion() );
		}

	bool isRelatedSpecification( bool isExclusiveSpecification, bool isPossessive, WordItem *specificationWordItem )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isPossessive_ == isPossessive &&
				relationContextNr_ > NO_CONTEXT_NR &&
				specificationWordItem != NULL &&
				specificationWordItem_ == specificationWordItem );
		}

	bool isRelatedSpecification( bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		return ( isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				specificationWordItem != NULL &&
				specificationWordItem_ == specificationWordItem &&
				isMatchingRelationContextNr( true, relationContextNr ) );
		}

	bool isRelatedSpecification( unsigned int specificationCollectionNr, unsigned int compoundSpecificationCollectionNr, WordItem *specificationWordItem )
		{
		return ( ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr ) ||

				( compoundSpecificationCollectionNr > NO_COLLECTION_NR &&

				( specificationCollectionNr_ == compoundSpecificationCollectionNr ||
				specificationWordItem_ == specificationWordItem ) ) );
		}

	bool isSelfGeneratedAssumption()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( !isConcludedAssumption_ &&
		questionParameter_ == NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != NULL )
			{
			if( assumptionLevel_ > NO_ASSUMPTION_LEVEL )
				return true;

			while( searchJustificationItem != NULL )
				{
				// At least one justification is an assumption
				if( searchJustificationItem->isAssumptionJustification() )
					return true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return false;
		}

	bool isSelfGeneratedConclusion()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( questionParameter_ == NO_QUESTION_PARAMETER &&
		firstJustificationItem_ != NULL )
			{
			if( isConcludedAssumption_ )
				return true;

			while( searchJustificationItem != NULL )
				{
				// At least one justification is an assumption. So, it is no conclusion
				if( searchJustificationItem->isAssumptionJustification() )
					return false;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}

			return true;
			}

		return false;
		}

	bool isSelfGeneratedQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER &&
				firstJustificationItem_ != NULL );
		}

	bool isSelfGenerated()
		{
		return ( firstJustificationItem_ != NULL );
		}

	bool isSpecific()
		{
		return isSpecific_;
		}

	bool isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	bool isSpecificationNoun()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR ||
				specificationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isSpecificationNumeral()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isSpecificationPluralNoun()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isSpecificationSingularNoun()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}

	bool isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > NO_WRITE_LEVEL );
		}

	bool isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				myWordItem()->nContextWords( relationContextNr_, specificationWordItem_ ) > 1 );
		}

	bool isSpecificationWordSpanishAmbiguous()
		{
		return ( specificationWordItem_ != NULL &&
				specificationWordItem_->isNounWordSpanishAmbiguous() );
		}

	bool isTriggeringExecutionOfSelections()
		{
		return ( isAssignment() ||
				generalizationWordTypeNr_ == WORD_TYPE_VERB_SINGULAR );
		}

	bool isUncountableGeneralizationNoun()
		{
		return isUncountableGeneralizationNoun_;
		}

	bool isUniqueUserRelation()
		{
		return isUniqueUserRelation_;
		}

	bool isUserSpecification()
		{
		return ( questionParameter_ == NO_QUESTION_PARAMETER &&
				firstJustificationItem_ == NULL );
		}

	bool isValueSpecification()
		{
		return isValueSpecification_;
		}

	bool wasHiddenSpanishSpecification()
		{
		JustificationItem *attachedJustificationItem;

		return ( relationContextNr_ > NO_CONTEXT_NR &&
				// Self-generated
				firstJustificationItem_ != NULL &&
				isOlderItem() &&
				strlen( storedSentenceString_ ) == 0 &&
				isSpecificationWordSpanishAmbiguous() &&
				!isHiddenSpanishSpecification() &&

				( isSelfGeneratedAssumption() ||

				( !isConcludedAssumption_ &&
				firstJustificationItem_->hasPossessiveSecondarySpecification() ) ||

				( isConcludedAssumption_ &&

				( firstJustificationItem_->hasNonPossessivePrimarySpecification() ||

				( firstJustificationItem_->isPossessiveReversibleConclusion() &&
				( attachedJustificationItem = firstJustificationItem_->attachedJustificationItem() ) != NULL &&
				attachedJustificationItem->hasNonPossessivePrimarySpecification() ) ) ) ) );
		}

	unsigned short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	unsigned short highestAttachedJustificationOrderNr( unsigned short justificationTypeNr )
		{
		unsigned short highestOrderNr = NO_ORDER_NR;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->orderNr > highestOrderNr &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchJustificationItem->orderNr;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return highestOrderNr;
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

	unsigned int generalizationCollectionNr()
		{
		return generalizationCollectionNr_;
		}

	unsigned int specificationCollectionNr()
		{
		return specificationCollectionNr_;
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

	unsigned int nInvolvedSpecificationWords()
		{
		unsigned int myCreationSentenceNr = creationSentenceNr();
		unsigned int nInvolvedSpecificationWords = 0;
		SpecificationItem *searchSpecificationItem = myWordItem()->firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem == this ||
			searchSpecificationItem->creationSentenceNr() == myCreationSentenceNr )
				{
				if( !searchSpecificationItem->hasUserSpecificationWordItem() )
					return 0;

				nInvolvedSpecificationWords++;
				}

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem();
			}

		return nInvolvedSpecificationWords;
		}

	signed char attachJustificationToSpecification( JustificationItem *attachJustificationItem )
		{
		bool hasCorrectedAssumptionByKnowledge;
		JustificationItem *firstJustificationItem;
		SpecificationItem *createdSpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "attachJustificationToSpecification";

		if( attachJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given attached justification item is undefined" );

		if( !attachJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given attached justification item isn't active" );

		if( isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "I am replaced or deleted" );

		if( ( firstJustificationItem = firstJustificationItem_ ) == NULL )
			return startError( functionNameString, NULL, "I have no justification items" );

		if( hasCurrentCreationSentenceNr() )
			{
			hasCorrectedAssumptionByKnowledge = myWordItem()->hasCorrectedAssumptionByKnowledge();

			if( changeFirstJustification( !hasCorrectedAssumptionByKnowledge, attachJustificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to change my first justification item" );

			if( !hasCorrectedAssumptionByKnowledge &&
			attachJustificationItem->attachJustification( firstJustificationItem, this ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to attach the first justification item of myself to the given attached justification item" );
			}
		else
			{
			// Copy specification
			if( ( createAndAssignResult = myWordItem()->copySpecificationItem( specificationCollectionNr_, attachJustificationItem, this ) ).result != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to copy myself with a different first justification item" );

			if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
				return startError( functionNameString, NULL, "I couldn't create a specification" );

			if( attachJustificationItem->attachJustification( firstJustificationItem, createdSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to attach the first justification item of the created specification item, to the given attached justification item of the created specification item" );

			if( myWordItem()->replaceOrDeleteSpecification( this, createdSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to replace or delete a specification" );
			}

		return RESULT_OK;
		}

	signed char changeFirstJustification( bool isCheckingForDuplicates, JustificationItem *replacingJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changeFirstJustification";

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing justification item is undefined" );

		if( !replacingJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given replacing justification item isn't active" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( isCheckingForDuplicates &&
		checkJustification( replacingJustificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to check the given replacing justification item" );

		firstJustificationItem_ = replacingJustificationItem;

		return RESULT_OK;
		}

	signed char checkForDeletedJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDeletedJustification";

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->isDeletedItem() )
				{
				strcpy( errorString, "\nI found a deleted justification item in a replaced specification item:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, NULL, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return RESULT_OK;
		}

	signed char checkForReplacedOrDeletedJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForReplacedOrDeletedJustification";

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->isReplacedOrDeletedItem() )
				{
				strcpy( errorString, "\nI found a replaced or deleted justification item:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, NULL, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return RESULT_OK;
		}

	signed char checkJustification( JustificationItem *referenceJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		SpecificationItem *referencePrimarySpecificationItem;
		SpecificationItem *referenceAnotherPrimarySpecificationItem;
		SpecificationItem *referenceSecondarySpecificationItem;
		SpecificationItem *referenceAnotherSecondarySpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkJustification";

		if( referenceJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given reference justification item is undefined" );

		if( searchJustificationItem == NULL )
			return startError( functionNameString, NULL, "I have no justification items" );

		referencePrimarySpecificationItem = referenceJustificationItem->primarySpecificationItem();
		referenceAnotherPrimarySpecificationItem = referenceJustificationItem->anotherPrimarySpecificationItem();
		referenceSecondarySpecificationItem = referenceJustificationItem->secondarySpecificationItem();
		referenceAnotherSecondarySpecificationItem = referenceJustificationItem->anotherSecondarySpecificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem == referenceJustificationItem )
				return startError( functionNameString, NULL, "The given reference justification item is already one of my attached justification items" );

			if( searchJustificationItem->hasJustification( referencePrimarySpecificationItem, referenceAnotherPrimarySpecificationItem, referenceSecondarySpecificationItem, referenceAnotherSecondarySpecificationItem ) )
				return startError( functionNameString, NULL, "I already have a justificiation item like the given attached justification item" );

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return RESULT_OK;
		}

	signed char collectGeneralization( bool isExclusiveGeneralization, unsigned int generalizationCollectionNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectGeneralization";

		if( generalizationCollectionNr <= NO_COLLECTION_NR )
			return startError( functionNameString, NULL, "The given generalization collection number is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( isExclusiveGeneralization )
			isExclusiveGeneralization_ = true;

		if( generalizationCollectionNr_ > NO_COLLECTION_NR )
			return startError( functionNameString, NULL, "The generalization is already collected" );

		generalizationCollectionNr_ = generalizationCollectionNr;

		return RESULT_OK;
		}

	signed char collectSpecification( bool isExclusiveSpecification, unsigned int specificationCollectionNr )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectSpecification";

		if( specificationCollectionNr <= NO_COLLECTION_NR )
			return startError( functionNameString, NULL, "The given specification collection number is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( isExclusiveSpecification )
			isExclusiveSpecification_ = true;

		if( specificationCollectionNr_ > NO_COLLECTION_NR )
			return startError( functionNameString, NULL, "The specification is already collected" );

		specificationCollectionNr_ = specificationCollectionNr;

		return RESULT_OK;
		}

	signed char markAsConcludedAssumption()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markAsConcludedAssumption";

		if( !isSelfGeneratedAssumption() &&
		!myWordItem()->isUserRelationWord )
			return startError( functionNameString, NULL, "I am not a self-generated assumption" );

		isConcludedAssumption_ = true;
		assumptionLevel_ = NO_ASSUMPTION_LEVEL;

		// Clear stored sentence buffer to lose the uncertainty word written in the sentence
		clearStoredSentenceString();

		return RESULT_OK;
		}

	signed char markSpecificationStringAsWritten()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markSpecificationStringAsWritten";

		if( globalVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, "Current write word level overflow" );

		if( specificationStringWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, "My write level is already assigned" );

		specificationStringWriteLevel_ = ++globalVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	signed char recalculateAssumption()
		{
		bool hasRelationContext = ( relationContextNr_ > NO_CONTEXT_NR );
		bool isAdjustedSpecification = false;
		bool isOlderAssumption;
		unsigned short previousAssumptionLevel = assumptionLevel_;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumption";

		if( firstJustificationItem_ != NULL &&
		firstJustificationItem_->hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition() )
			{
			if( markAsConcludedAssumption() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to mark myself as a concluded assumption" );

			isAdjustedSpecification = true;
			}
		else
			{
			isOlderAssumption = isOlderItem();

			if( specificationCollectionNr_ == NO_COLLECTION_NR ||

			// Avoid corrected assumption to be concluded
			( !isCorrectedAssumption_ &&

			( !isOlderAssumption ||
			// Avoid older corrected assumption to be concluded
			!myWordItem()->hasCorrectedAssumptionByKnowledge() ) ) )
				{
				if( calculateAssumptionLevel().result != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to recalculate my assumption level" );

				if( isOlderAssumption &&

				( assumptionLevel_ != previousAssumptionLevel ||
				// Typical for Spanish
				assumptionLevel_ == NO_ASSUMPTION_LEVEL ) &&

				// Avoid updates of high uncertainty mapping,
				// from 'maybe' of one level to 'maybe' of another level
				( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
				assumptionLevel_ < NUMBER_OF_ASSUMPTION_LEVELS ) )
					{
					if( assumptionLevel_ == NO_ASSUMPTION_LEVEL )
						{
						if( markAsConcludedAssumption() != RESULT_OK )
							return addError( functionNameString, NULL, "After recalculation, I failed to mark myself as a concluded assumption" );

						if( ( !hasRelationContext &&
						previousAssumptionLevel > NO_ASSUMPTION_LEVEL ) ||

						( isPossessive_ &&
						assumptionLevel_ != previousAssumptionLevel ) )
							isAdjustedSpecification = true;
						}
					else
						{
						if( ( !isPossessive_ &&
						previousAssumptionLevel > NO_ASSUMPTION_LEVEL ) ||

						( hasRelationContext &&
						hasCompoundSpecificationCollection() &&
						!isSpecificationWordSpanishAmbiguous() ) )
							isAdjustedSpecification = true;
						}
					}
				}
			}

		if( isAdjustedSpecification )
			{
			if( isNegative_ &&
			assumptionLevel_ > NO_ASSUMPTION_LEVEL )
				hasSpecificationBeenWrittenAsAdjustedNegativeAssumption_ = true;

			// Write adjusted specification
			if( myWordItem()->writeUpdatedSpecification( true, false, false, false, wasHiddenSpanishSpecification(), this ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to write an adjusted specification" );

			if( specificationCollectionNr_ > NO_COLLECTION_NR )
				{
				if( hasRelationContext )
					{
					if( recalculateAssumptionsInContextWords( relationContextNr_, specificationWordItem_ ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to recalculate the assumptions of words with my relation context" );
					}
				else
					{
					if( myWordItem()->recalculateAssumptionsInWord() != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to recalculate the assumptions of my word" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char writeSpecificationConflict( bool isQuestion )
		{
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationConflict";

		if( myWordItem()->writeSelectedSpecification( false, false, this ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write the conflicting specification" );

		if( ( writtenSentenceString = globalVariables()->writtenSentenceString ) == NULL ||
		strlen( writtenSentenceString ) == 0 )
			return startError( functionNameString, NULL, "The written sentence string is empty" );

		if( inputOutput()->writeInterfaceText( true, ( isQuestion ? INPUT_OUTPUT_PROMPT_WRITE : INPUT_OUTPUT_PROMPT_WARNING ), ( isOlderItem() ? ( isQuestion ? INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_DEFINITION : INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_DEFINITION ) : INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write an interface warning" );

		if( inputOutput()->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables()->learnedFromUserString ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write the conflict sentence" );

		hasSpecificationBeenWrittenAsConflict_ = true;

		return RESULT_OK;
		}

	char *specificationString()
		{
		return specificationString_;
		}

	char *activeGeneralizationWordTypeString()
		{
		return myWordItem()->activeWordTypeString( generalizationWordTypeNr_ );
		}

	char *activeSpecificationWordTypeString()
		{
		return ( specificationWordItem_ == NULL ? NULL : specificationWordItem_->activeWordTypeString( specificationWordTypeNr_ ) );
		}

	char *storedSentenceString()
		{
		return storedSentenceString_;
		}

	char *storedSentenceWithOnlyOneSpecificationString()
		{
		return storedSentenceWithOnlyOneSpecificationString_;
		}

	JustificationItem *currentJustificationItem( unsigned short justificationTypeNr )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( !searchJustificationItem->isOlderItem() &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *differentAssumptionLevelSpecificationSubstitutionJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		unsigned short primaryAssumptionLevel;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != NULL &&
		secondarySpecificationItem != NULL )
			{
			primaryAssumptionLevel = primarySpecificationItem->assumptionLevel();

			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isSpecificationSubstitutionAssumption() &&
				searchJustificationItem->isOlderItem() &&
				searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem &&
				searchJustificationItem->primarySpecificationAssumptionLevel() != primaryAssumptionLevel )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *firstJustificationItem()
		{
		return firstJustificationItem_;
		}

	JustificationItem *firstJustificationItem( unsigned short justificationTypeNr )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *olderJustificationItem( unsigned short justificationTypeNr )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isOlderItem() &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *previousJustificationItem( JustificationItem *currentJustificationItem )
		{
		JustificationItem *attachedJustificationItem;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( currentJustificationItem != NULL &&
		currentJustificationItem != firstJustificationItem_ )
			{
			while( ( attachedJustificationItem = searchJustificationItem->attachedJustificationItem() ) != currentJustificationItem )
				searchJustificationItem = attachedJustificationItem;

			return searchJustificationItem;
			}

		return NULL;
		}

	JustificationItem *primaryOrSecondarySpecificationJustificationItem( SpecificationItem *primaryOrSecondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( primaryOrSecondarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->primarySpecificationItem() == primaryOrSecondarySpecificationItem ||
				searchJustificationItem->secondarySpecificationItem() == primaryOrSecondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *secondarySpecificationJustificationItem( SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( secondarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *secondarySpecificationWithoutRelationContextJustificationItem( WordItem *secondarySpecificationWordItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( secondarySpecificationWordItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isPossessiveReversibleConclusionWithoutRelationContext() )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *exclusiveSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isExclusiveSpecification() &&
			!searchSpecificationItem->isNegative() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nonPossessiveDefinitionSpecificationItem( bool isIncludingAdjectives, bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isPossessive() &&

			( isIncludingAdjectives ||
			searchSpecificationItem->isSpecificationNoun() ) &&

			!searchSpecificationItem->isSpecificationGeneralization() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *possessiveSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPossessive() &&
			searchSpecificationItem->hasSpecificationCollection() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *primarySpecificationCandidateOfQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification, bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isPrimarySpecificationCandidateOfQuestion( isAllowingSpanishPossessiveSpecification ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *relationSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasRelationContext() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *selectedRelationSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasRelationContext() &&
			!searchSpecificationItem->isQuestion() &&
			!searchSpecificationItem->isHiddenSpanishSpecification() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *specificationItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, bool isQuestion )
		{
		unsigned short currentLanguageNr = globalVariables()->currentLanguageNr;
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isQuestion() == isQuestion &&

			( isLanguageWord_ ||
			// Skip other languages
			searchSpecificationItem->languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *specificationItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, unsigned short questionParameter )
		{
		unsigned short currentLanguageNr = globalVariables()->currentLanguageNr;
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->questionParameter_ == questionParameter &&

			( isLanguageWord_ ||
			// Skip other languages
			searchSpecificationItem->languageNr_ == currentLanguageNr ) &&

			( isIncludingAnsweredQuestions ||
			// Skip answered questions
			!searchSpecificationItem->isAnsweredQuestion_ ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nextExclusiveNounSpecificationItem()
		{
		return exclusiveSpecificationItem( false );
		}

	SpecificationItem *nextExclusiveSpecificationItem()
		{
		return exclusiveSpecificationItem( false );
		}

	SpecificationItem *nextNonPossessiveDefinitionSpecificationItem( bool isIncludingAdjectives )
		{
		return nonPossessiveDefinitionSpecificationItem( isIncludingAdjectives, false );
		}

	SpecificationItem *nextPossessiveSpecificationItem()
		{
		return possessiveSpecificationItem( false );
		}

	SpecificationItem *nextPrimarySpecificationCandidateOfQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification )
		{
		return primarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, false );
		}

	SpecificationItem *nextRelationSpecificationItem()
		{
		return relationSpecificationItem( false );
		}

	SpecificationItem *nextSelectedRelationSpecificationItem()
		{
		return selectedRelationSpecificationItem( false );
		}

	SpecificationItem *nextSelectedSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, isQuestion() ) : specificationItem( false, false, isQuestion() ) );
		}

	SpecificationItem *nextSelectedQuestionParameterSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, questionParameter_ ) : specificationItem( false, false, questionParameter_ ) );
		}

	SpecificationItem *nextSelectedQuestionParameterSpecificationItem( bool isIncludingAnsweredQuestions )
		{
		return ( isAssignment() ? assignmentItem( isIncludingAnsweredQuestions, false, questionParameter_ ) : specificationItem( isIncludingAnsweredQuestions, false, questionParameter_ ) );
		}

	SpecificationItem *nextSpecificationItem()
		{
		return (SpecificationItem *)nextItem;
		}

	SpecificationItem *recentlyAnsweredQuestionSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = specificationItem( true, isIncludingThisItem, questionParameter_ );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->hasCurrentCreationSentenceNr() &&
			searchSpecificationItem->isAnsweredQuestion_ )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( true );
			}

		return NULL;
		}

	SpecificationItem *updatedSpecificationItem()
		{
		SpecificationItem *updatedSpecificationItem;
		SpecificationItem *searchSpecificationItem = this;

		while( ( updatedSpecificationItem = searchSpecificationItem->replacingSpecificationItem ) != NULL )
			searchSpecificationItem = updatedSpecificationItem;

		return searchSpecificationItem;
		}

	WordItem *collectedWithItselfCommonWordItem()
		{
		WordItem *commonWordItem;

		if( specificationWordItem_ != NULL &&
		( commonWordItem = specificationWordItem_->commonWordItem( specificationCollectionNr_ ) ) != NULL &&
		commonWordItem->isNounWordSpanishAmbiguous() )
			return commonWordItem;

		return NULL;
		}

	WordItem *firstUnwrittenRelationWordItem( unsigned short wordTypeNr, WordItem *previousWordItem )
		{
		// Start with previous word
		WordItem *currentRelationWordItem = previousWordItem;

		// Check all relation words
		while( ( currentRelationWordItem = relationContextWordItemInContextWords( currentRelationWordItem ) ) != NULL )
			{
			if( !currentRelationWordItem->isRelationWordTypeAlreadyWritten( wordTypeNr ) )
				return currentRelationWordItem;
			}

		return NULL;
		}

	WordItem *generalizationWordItem()
		{
		return myWordItem();
		}

	WordItem *singleRelationWordItem()
		{
		WordItem *firstRelationWordItem;

		// Check all relation words
		if( ( firstRelationWordItem = relationContextWordItemInContextWords( NULL ) ) != NULL &&
		// Found another relation word
		relationContextWordItemInContextWords( firstRelationWordItem ) != NULL )
			return NULL;

		return firstRelationWordItem;
		}

	WordItem *relatedSpecificationWordItem( bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr )
		{
		if( ( isIgnoringExclusive ||
		isExclusiveSpecification_ == isExclusiveSpecification ) &&

		( isIgnoringNegative ||
		isNegative_ == isNegative ) &&

		isPossessive_ == isPossessive &&
		specificationCollectionNr_ == specificationCollectionNr &&
		generalizationContextNr_ == generalizationContextNr &&

		( ( !isCheckingRelationContext &&
		!isHiddenSpanishSpecification() ) ||

		relationContextNr_ == relationContextNr ) )
			return specificationWordItem_;

		return NULL;
		}

	WordItem *relatedSpecificationWordItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr )
		{
		if( isExclusiveSpecification_ == isExclusiveSpecification &&
		isNegative_ == isNegative &&
		isPossessive_ == isPossessive &&
		isSelfGenerated() == isSelfGeneratedSpecification &&
		specificationCollectionNr > NO_COLLECTION_NR &&
		specificationCollectionNr_ == specificationCollectionNr &&
		generalizationContextNr_ == generalizationContextNr &&
		relationContextNr_ == relationContextNr &&

		( assumptionLevel_ == assumptionLevel ||
		isQuestion() ) &&

		( specificationWordTypeNr == NO_WORD_TYPE_NR ||
		specificationWordTypeNr_ == specificationWordTypeNr ) )
			return specificationWordItem_;

		return NULL;
		}

	WordItem *specificationWordItem()
		{
		return specificationWordItem_;
		}

	ShortResultType calculateAssumptionLevel()
		{
		bool isSpanishCurrentLanguage;
		bool _isSpecificationWordSpanishAmbiguous = false;
		unsigned short highestAssumptionLevel;
		unsigned short lowestAssumptionLevel = MAX_LEVEL;
		unsigned int nJustificationRelationWords;
		unsigned int nRelationWords;
		unsigned int tempAssumptionLevel;
		JustificationItem *currentJustificationItem;
		JustificationItem *nextJustificationItem = firstJustificationItem_;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "calculateAssumptionLevel";

		if( !isSpecificationGeneralization_ &&
		isSelfGeneratedAssumption() )
			{
			isSpanishCurrentLanguage = myWordItem()->isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				_isSpecificationWordSpanishAmbiguous = isSpecificationWordSpanishAmbiguous();

			nRelationWords = nRelationContextWords();

			do	{
				highestAssumptionLevel = NO_ASSUMPTION_LEVEL;
				nJustificationRelationWords = 0;
				currentJustificationItem = nextJustificationItem;

				if( currentJustificationItem != NULL )
					{
					do	{
						nJustificationRelationWords += currentJustificationItem->nJustificationContextRelations( relationContextNr_, nRelationWords );

						if( currentJustificationItem->isAssumptionJustification() )
							{
							if( ( shortResult = currentJustificationItem->getCombinedAssumptionLevel() ).result != RESULT_OK )
								return addShortResultError( functionNameString, NULL, NULL, "I failed to get the combined assumption level" );

							// Add combined assumption level and justification grade
							if( ( tempAssumptionLevel = ( shortResult.shortValue + currentJustificationItem->justificationAssumptionGrade() ) ) > highestAssumptionLevel )
								{
								if( tempAssumptionLevel >= MAX_LEVEL )
									return startShortResultSystemError( functionNameString, "Assumption level overflow" );

								highestAssumptionLevel = (unsigned short)tempAssumptionLevel;
								}
							}
						}
					while( ( currentJustificationItem = currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );
					}

				if( highestAssumptionLevel < lowestAssumptionLevel &&

				( nJustificationRelationWords == nRelationWords ||

				( highestAssumptionLevel <= assumptionLevel_ &&
				nJustificationRelationWords > nRelationWords ) ) &&

				( !isSpanishCurrentLanguage ||
				// Typical for Spanish
				isNegative_ ||
				// No assumption level
				assumptionLevel_ == NO_ASSUMPTION_LEVEL ||

				// First set of justifications
				( lowestAssumptionLevel == MAX_LEVEL &&
				highestAssumptionLevel == NO_ASSUMPTION_LEVEL ) ||

				// Assumption level
				( !_isSpecificationWordSpanishAmbiguous &&
				assumptionLevel_ + 2 > highestAssumptionLevel ) ) )
					lowestAssumptionLevel = highestAssumptionLevel;
				}
			while( ( nextJustificationItem = nextJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != NULL );

			if( lowestAssumptionLevel < MAX_LEVEL &&
			// Assumption level has changed
			lowestAssumptionLevel != assumptionLevel_ &&

			( lowestAssumptionLevel > NO_ASSUMPTION_LEVEL ||
			isOlderItem() ) )
				{
				assumptionLevel_ = lowestAssumptionLevel;

				if( assumptionLevel_ > NO_ASSUMPTION_LEVEL &&
				isOlderItem() )
					// Clear stored sentence buffer to lose the previous assumption level word
					clearStoredSentenceString();
				}
			}

		shortResult.shortValue = assumptionLevel_;
		return shortResult;
		}
	};
#endif

/*************************************************************************
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his covenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *************************************************************************/
