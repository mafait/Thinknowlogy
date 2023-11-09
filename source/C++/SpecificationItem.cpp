/*	Class:			SpecificationItem
 *	Purpose:		Storing info about the specification structure of a word
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

#ifndef SPECIFICATIONITEM
#define SPECIFICATIONITEM 1
#include "CreateAndAssignResultType.cpp"
#include "GeneralizationItem.cpp"
#include "InputOutput.cpp"
#include "List.h"
#include "ShortResultType.cpp"
#include "SpecificationResultType.cpp"

class SpecificationItem : private Item
	{
	friend class AdminImperative;
	friend class AdminReadFile;
	friend class AdminReasoning;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class SpecificationList;
	friend class WordItem;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordWrite;

	// Private initialized variables

	bool isAnsweredQuestion_ = false;
	bool isCharacteristicFor_ = false;
	bool isConditional_ = false;
	bool isCorrectedSpecification_ = false;
	bool isEveryGeneralization_ = false;
	bool isExclusiveGeneralization_ = false;
	bool isExclusiveSpecification_ = false;
	bool isLanguageWord_ = false;
	bool isNegative_ = false;
	bool isPartOf_ = false;
	bool isPossessive_ = false;
	bool isSpecific_ = false;
	bool isSpecificationGeneralization_ = false;
	bool isUncountableGeneralizationNoun_ = false;
	bool isUniqueUserRelation_ = false;
	bool isValueSpecification_ = false;

	unsigned short assignmentLevel_ = NO_ASSIGNMENT_LEVEL;
	unsigned short assumptionLevel_ = NO_ASSUMPTION_LEVEL;
	unsigned short languageNr_ = NO_LANGUAGE_NR;
	unsigned short prepositionParameter_ = NO_PREPOSITION_PARAMETER;
	unsigned short questionParameter_ = NO_QUESTION_PARAMETER;

	unsigned short generalizationWordTypeNr_ = NO_WORD_TYPE_NR;
	unsigned short specificationWordTypeNr_ = NO_WORD_TYPE_NR;
	unsigned short relationWordTypeNr_ = NO_WORD_TYPE_NR;

	unsigned int generalizationCollectionNr_ = NO_COLLECTION_NR;
	unsigned int specificationCollectionNr_ = NO_COLLECTION_NR;

	unsigned int generalizationContextNr_ = NO_CONTEXT_NR;
	unsigned int specificationContextNr_ = NO_CONTEXT_NR;
	unsigned int relationContextNr_ = NO_CONTEXT_NR;

	unsigned int nContextRelations_ = 0;

	JustificationItem *firstJustificationItem_ = NULL;

	WordItem *specificationWordItem_ = NULL;

	char *specificationString_ = NULL;

	char storedSentenceString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;
	char storedSentenceWithOnlyOneSpecificationString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;

	// Private constructed variables

	unsigned short specificationStringWriteLevel_ = NO_WRITE_LEVEL;


	// Private assignment functions

	SpecificationItem *nextAssignmentItemWithAssignmentLevel( unsigned short currentAssignmentLevel )
		{
		SpecificationItem *nextAssignmentItem;

		return ( ( nextAssignmentItem = nextSpecificationItem() ) != NULL &&
				nextAssignmentItem->assignmentLevel() == currentAssignmentLevel ? nextAssignmentItem : NULL );
		}


	// Private justification functions

	bool hasAtLeastOneAssumptionJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasAssumptionLevel() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasFoundCircularReasoning()
		{
		bool hasFoundOnlyUserSpecifications = true;
		bool hasFoundOtherJustificationTypes = false;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( !hasFoundOtherJustificationTypes &&
		searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isReversibleAssumptionOrConclusion() )
				{
				if( searchJustificationItem->primarySelfGeneratedSpecification() != NULL )
					hasFoundOnlyUserSpecifications = false;
				}
			else
				hasFoundOtherJustificationTypes = true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		if( !hasFoundOtherJustificationTypes &&
		!hasFoundOnlyUserSpecifications )
			return true;

		return false;
		}

	bool hasPrimaryAssignmentJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isPrimaryAssignmentSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	signed char addJustificationsToContextSpecifications()
		{
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *oppositeSpecificationItem;
		SpecificationItem *reversibleSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *partOfSpecificationWordItem;
		WordItem *oppositeSpecificationWordItem;
		WordItem *_myWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addJustificationsToContextSpecifications";

		if( specificationWordItem_ == NULL )
			return startError( functionNameString, NULL, "I have no specification word" );

		if( ( currentContextWordItem = _myWordItem->firstContextWordItem( relationContextNr_ ) ) != NULL )
			{
			partOfSpecificationWordItem = specificationWordItem_->commonWordItem( specificationCollectionNr_ );

			// Do for all context words with my relation context number
			do	{
				if( ( reversibleSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive_, specificationWordItem_, _myWordItem ) ) != NULL &&
				!reversibleSpecificationItem->isHiddenSpanishSpecification() )
					{
					if( !hasOnlyOneRelationWord() &&
					reversibleSpecificationItem->isSelfGeneratedSpecification() )
						{
						// Justification doesn't exist yet in reversible specification
						if( !reversibleSpecificationItem->hasPrimarySpecificationJustification( this ) &&
						// Justification has at least the same assumption level
						reversibleSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, this, NULL, NULL, NULL ) &&
						// Add non-possessive reversible justification to myself
						reversibleSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, this, NULL, NULL ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to add a non-possessive reversible justification to myself" );

						reversibleSpecificationItem = reversibleSpecificationItem->updatedSpecificationItem();

						if( partOfSpecificationWordItem != NULL )
							{
							if( isPossessive_ )
								{
								if( ( partOfSpecificationItem = currentContextWordItem->partOfSpecificationItem( partOfSpecificationWordItem ) ) != NULL &&
								// Justification doesn't exist yet in part-of specification
								!partOfSpecificationItem->hasPrimarySpecificationJustification( reversibleSpecificationItem ) &&
								( definitionSpecificationItem = partOfSpecificationWordItem->partOfSpecificationItem( specificationWordItem_ ) ) != NULL &&
								// Justification has at least the same assumption level
								partOfSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, NULL, definitionSpecificationItem, NULL ) &&
								// Add non-possessive part-of justification to part-of specification
								partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, NULL, definitionSpecificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to add a non-possessive part-of justification to a part-of specification" );
								}
							else
								{
								if( ( partOfSpecificationItem = _myWordItem->partOfSpecificationItem( partOfSpecificationWordItem ) ) != NULL &&
								// Justification doesn't exist yet in part-of specification
								!partOfSpecificationItem->hasPrimarySpecificationJustification( this ) &&
								( definitionSpecificationItem = partOfSpecificationWordItem->partOfSpecificationItem( specificationWordItem_ ) ) != NULL &&
								// Justification has at least the same assumption level
								partOfSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, NULL, definitionSpecificationItem, NULL ) &&
								// Add non-possessive part-of justification to part-of specification
								partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, NULL, definitionSpecificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to add a non-possessive part-of justification to a part-of specification" );
								}
							}
						}

					if( !isPossessive_ &&
					!reversibleSpecificationItem->hasOnlyOneRelationWord() &&
					// I don't have this reversible justification
					!hasPrimarySpecificationJustification( reversibleSpecificationItem ) &&
					// I have no better assumption level
					assumptionLevel_ >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, NULL, NULL, NULL ) &&
					// Add possessive reversible justification to myself
					addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, NULL, NULL ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add a possessive reversible justification to myself" );

					if( currentContextWordItem->hasContextInWord( relationContextNr_ ) &&
					// Recalculate assumption levels of specifications in context word
					currentContextWordItem->recalculateAssumptionLevelsInWord() != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to recalculate the assumption levels of specifications in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
					}

				if( isPossessive_ &&
				partOfSpecificationWordItem != NULL )
					{
					if( ( partOfSpecificationItem = currentContextWordItem->partOfSpecificationItem( partOfSpecificationWordItem ) ) != NULL &&
					// Part-of specification has no similar justification
					partOfSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, _myWordItem ) == NULL &&
					// Justification doesn't exist yet in part-of specification
					!partOfSpecificationItem->hasPrimarySpecificationJustification( this ) &&
					( definitionSpecificationItem = partOfSpecificationWordItem->partOfSpecificationItem( specificationWordItem_ ) ) != NULL &&
					// Justification has at least the same assumption level
					partOfSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, NULL, definitionSpecificationItem, NULL ) &&
					// Add possessive part-of justification to part-of specification
					partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, NULL, definitionSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add a possessive part-of justification to a part-of specification" );

					if( !specificationWordItem_->isNounWordSpanishAmbiguous() &&
					( oppositeSpecificationWordItem = specificationWordItem_->collectionWordItem( specificationCollectionNr_ ) ) != NULL &&
					( oppositeSpecificationItem = _myWordItem->firstSpecificationItem( false, false, false, oppositeSpecificationWordItem ) ) != NULL &&
					( definitionSpecificationItem = partOfSpecificationWordItem->firstSpecificationItem( false, true, false, specificationWordItem_ ) ) != NULL &&
					// Justification doesn't exist yet in part-of specification
					!oppositeSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, this ) &&
					// Justification has at least the same assumption level
					oppositeSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, this, NULL ) &&
					// Add opposite possessive assumption justification to opposite specification
					oppositeSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, this ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add an opposite possessive assumption justification to the opposite specification" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->_nextContextWordItem( relationContextNr_ ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char processChangedAssumption( unsigned short previousAssumptionLevel )
		{
		bool _hasRelationContext = ( relationContextNr_ > NO_CONTEXT_NR );
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *_myWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "processChangedAssumption";

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I am a user specification" );

		if( specificationWordItem_ == NULL )
			return startError( functionNameString, NULL, "I have no specification word" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "I am a replaced or deleted specification" );

		// Quick condition check
		if( ( !_hasRelationContext ||
		assumptionLevel_ <= previousAssumptionLevel ||
		assumptionLevel_ >= NUMBER_OF_ASSUMPTION_LEVELS ) &&

		// Within defined number of assumption levels
		( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
		assumptionLevel_ < NUMBER_OF_ASSUMPTION_LEVELS ) &&

		( assumptionLevel_ == NO_ASSUMPTION_LEVEL ||
		!hasNewInformation() ) &&

		( isPossessive_ ||
		assumptionLevel_ == NO_ASSUMPTION_LEVEL ||

		( !_hasRelationContext &&
		!isSpecificationWordSpanishAmbiguous() ) ||

		!_myWordItem->isUserGeneralizationWord ||
		// Relation won't be expanded
		// Test file: "Complex (7 - Claudia)"
		globalVariables()->nUserRelationWords == 1 ||

		( ( !hasPrimaryAssignmentJustification() ||
		firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == NULL ) &&

		!hasOnlyOneRelationWord() ) ) &&

		( isAssignment() ||
		// Skip writing update of specification part of assignment
		_myWordItem->firstAssignmentItem( isPossessive_, false, NO_CONTEXT_NR, specificationWordItem_ ) == NULL ) &&

		// Write adjusted specification
		_myWordItem->writeUpdatedSpecification( true, false, false, false, false, false, false, this ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write an adjusted specification" );

		if( _hasRelationContext )
			{
			// Add justifications to context specifications
			if( addJustificationsToContextSpecifications() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to add justifications to context specifications" );
			}
		else
			{
			// Definition specification
			if( generalizationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR &&
			// Get first generalization specification proper noun word
			( currentGeneralizationItem = specificationWordItem_->firstProperNounSpecificationGeneralizationItem() ) != NULL )
				{
				// Do for all generalization specification proper noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return addError( functionNameString, NULL, "I found an undefined generalization word" );

					// Recalculate assumption levels of specifications in proper noun word
					if( currentGeneralizationWordItem->recalculateAssumptionLevelsInWord() != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to recalculate the assumption levels of specifications in proper noun word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextProperNounSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}


	// Private specification functions

	bool hasContextMaleWordInContextWords()
		{
		WordItem *currentContextWordItem;
		WordItem *_myWordItem = myWordItem();

		if( relationContextNr_ > NO_CONTEXT_NR &&
		specificationWordItem_ != NULL &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != _myWordItem &&
				currentContextWordItem->isMasculineWord() &&
				currentContextWordItem->hasContextInWord( relationContextNr_ ) )
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

	bool isCandidateForQuestion( bool isAllowingSpanishPossessiveSpecification )
		{
		return ( !isExclusiveSpecification_ &&
				!isNegative_ &&
				!isPartOf_ &&

				( !isPossessive_ ||
				isAllowingSpanishPossessiveSpecification ) &&

				questionParameter_ == NO_QUESTION_PARAMETER &&
				!isHiddenSpanishSpecification() );
		}

	WordItem *relationContextWordItemInContextWords( WordItem *previousContextWordItem )
		{
		WordItem *currentContextWordItem;

		if( relationContextNr_ > NO_CONTEXT_NR &&
		// Do for all (remaining) context words
		( currentContextWordItem = ( previousContextWordItem == NULL ? globalVariables()->firstContextWordItem : previousContextWordItem->nextContextWordItem ) ) != NULL )
			{
			do	{
				if( currentContextWordItem->hasContextInWord( relationContextNr_ ) )
					return currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return NULL;
		}

	WordItem *relationContextWordNotCoveredByJustifications()
		{
		bool hasFoundContextInJustification = false;
		JustificationItem *searchJustificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentContextWordItem;

		if( relationContextNr_ > NO_CONTEXT_NR &&
		firstJustificationItem_ != NULL &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem->hasContextInWord( relationContextNr_ ) )
					{
					hasFoundContextInJustification = false;
					searchJustificationItem = firstJustificationItem_;

					while( !hasFoundContextInJustification &&
					searchJustificationItem != NULL )
						{
						// Check primary specification
						if( ( ( primarySpecificationItem = searchJustificationItem->primarySpecificationItem() ) != NULL &&
						primarySpecificationItem->hasRelationContext() &&

						( primarySpecificationItem->generalizationWordItem() == currentContextWordItem ||
						currentContextWordItem->hasContextInWord( primarySpecificationItem->relationContextNr() ) ) ) ||

						// Check secondary specification
						( ( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL &&
						secondarySpecificationItem->hasRelationContext() &&

						( secondarySpecificationItem->generalizationWordItem() == currentContextWordItem ||
						currentContextWordItem->hasContextInWord( secondarySpecificationItem->relationContextNr() ) ) ) )
							hasFoundContextInJustification = true;

						searchJustificationItem = searchJustificationItem->attachedJustificationItem();
						}

					if( !hasFoundContextInJustification )
						// This relation context word is not covered by justifications
						return currentContextWordItem;
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		// Justifications are covering given relation context
		return NULL;
		}

	protected:
	// Protected constructed variables

	bool hasAdjustedSpecificationBeenWritten = false;
	bool hasSpecificationBeenWrittenAsAnswer = false;

	SpecificationItem *replacingSpecificationItem = NULL;


	// Constructor

	SpecificationItem( bool isAnsweredQuestion, bool isCharacteristicFor, bool isConditional, bool isCorrectedSpecification, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isLanguageWord, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		size_t specificationStringLength;

		initializeItemVariables( originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, "SpecificationItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		isAnsweredQuestion_ = isAnsweredQuestion;
		isCharacteristicFor_ = isCharacteristicFor;
		isConditional_ = isConditional;
		isCorrectedSpecification_ = isCorrectedSpecification;
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

		if( specificationString != NULL )
			{
			if( ( specificationStringLength = strlen( specificationString ) ) < SENTENCE_STRING_LENGTH )
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

		if( storedSentenceString != NULL )
			strcpy( storedSentenceString_, storedSentenceString );

		if( storedSentenceString != NULL )
			strcpy( storedSentenceWithOnlyOneSpecificationString_, storedSentenceWithOnlyOneSpecificationString );
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
		WordItem *_myWordItem = myWordItem();
		char *languageNameString = _myWordItem->languageNameString( languageNr_ );
		char *generalizationWordTypeString = _myWordItem->wordTypeNameString( generalizationWordTypeNr_ );
		char *queryString;
		char *relationWordTypeString = ( relationWordTypeNr_ == NO_WORD_TYPE_NR ? NULL : _myWordItem->wordTypeNameString( relationWordTypeNr_ ) );
		char *specificationWordTypeString = _myWordItem->wordTypeNameString( specificationWordTypeNr_ );
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

		if( isConditional_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isConditional" );
			}

		if( isCorrectedSpecification_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isCorrectedSpecification" );
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


	// Protected common functions

	bool hasAssumptionLevel()
		{
		return ( assumptionLevel_ > NO_ASSUMPTION_LEVEL );
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
		return myWordItem()->nContextWords( relationContextNr_ );
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

	SpecificationItem *assignmentItem( bool isIncludingAnsweredQuestions, bool isIncludingThisItem, bool isQuestion )
		{
		unsigned short currentAssignmentLevel = globalVariables()->currentAssignmentLevel;
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ?
														( assignmentLevel_ == currentAssignmentLevel ? this : NULL ) :
														nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

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
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ?
														( assignmentLevel_ == currentAssignmentLevel ? this : NULL ) :
														nextAssignmentItemWithAssignmentLevel( currentAssignmentLevel ) );

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


	// Protected justification functions

	bool hasAdditionalDefinitionSpecificationJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasAdditionalDefinitionSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
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

	bool hasJustificationWithAdditionalDefinitionSpecification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->additionalDefinitionSpecificationItem() != NULL )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasPrimaryNounSpecificationJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isPrimaryNounSpecification() )
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

	bool hasPrimaryAndSecondarySpecificationJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasSecondarySpecificationJustification( SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasSecondarySpecificationCompoundCollectionJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasCompoundCollectionSecondarySpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool isFirstJustificationReversibleAssumptionOrConclusion()
		{
		return ( firstJustificationItem_ != NULL &&
				firstJustificationItem_->isReversibleAssumptionOrConclusion() );
		}

	bool isFirstJustificationReversibleAssumption()
		{
		return ( firstJustificationItem_ != NULL &&
				firstJustificationItem_->isReversibleAssumptionOrConclusion() &&
				firstJustificationItem_->hasAssumptionLevel() );
		}

	unsigned int highestReversibleJustificationAssumptionLevel()
		{
		unsigned int currentAssumptionLevel;
		unsigned int highestAssumptionLevel = NO_ASSUMPTION_LEVEL;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isReversibleAssumptionOrConclusion() &&
			( currentAssumptionLevel = searchJustificationItem->justificationAssumptionLevel() ) > highestAssumptionLevel )
				highestAssumptionLevel = currentAssumptionLevel;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return highestAssumptionLevel;
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

	signed char checkForCircularReasoning()
		{
		bool hasFoundCircularReasoning = false;
		bool hasFoundOnlyUserSpecifications = true;
		bool hasFoundOtherJustificationTypes = false;
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		SpecificationItem *primarySelfGeneratedSpecification;
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForCircularReasoning";

		while( !hasFoundOtherJustificationTypes &&
		searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->isReversibleAssumptionOrConclusion() )
				{
				if( ( primarySelfGeneratedSpecification = searchJustificationItem->primarySelfGeneratedSpecification() ) != NULL )
					{
					hasFoundOnlyUserSpecifications = false;

					if( primarySelfGeneratedSpecification->hasFoundCircularReasoning() )
						hasFoundCircularReasoning = true;
					}
				}
			else
				hasFoundOtherJustificationTypes = true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		if( hasFoundCircularReasoning &&
		!hasFoundOtherJustificationTypes &&
		!hasFoundOnlyUserSpecifications )
			{
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcpy( errorString, "\nI found a case of circular reasoning:\n\tSpecificationItem: " );
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char checkForDeletedJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDeletedJustification";

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->isDeletedItem() )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found a deleted justification item in a replaced specification item:\n\tSpecificationItem: " );
				strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
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
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForReplacedOrDeletedJustification";

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->isReplacedOrDeletedItem() )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found a replaced or deleted justification item:\n\tSpecificationItem: " );
				strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, NULL, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return RESULT_OK;
		}

	signed char checkForJustificationsCoveringRelationContext()
		{
		WordItem *currentContextWordItem;
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForJustificationsCoveringRelationContext";

		if( relationContextNr_ == NO_CONTEXT_NR )
			return startError( functionNameString, NULL, "I have no relation context" );

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I am a user specification. So, I have no justifications" );

		if( globalVariables()->firstContextWordItem == NULL )
			return startError( functionNameString, NULL, "I could not find any context word, while checking myself for justifications covering a relation context" );

		if( ( currentContextWordItem = relationContextWordNotCoveredByJustifications() ) != NULL )
			{
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcpy( errorString, "\nI found a specification missing justifications covering context word: \"" );
			strcat( errorString, currentContextWordItem->anyWordTypeString() );
			strcat( errorString, "\";\n\tRelation context number: " );
			sprintf( tempString, "%d", relationContextNr_ );
			strcat( errorString, tempString );
			strcat( errorString, ";\n\tSpecificationItem: " );
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char checkJustification( JustificationItem *referenceJustificationItem )
		{
		bool hasFeminineOrMasculineProperNounEnding;
		unsigned short justificationTypeNr;
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		SpecificationItem *referencePrimarySpecificationItem;
		SpecificationItem *referenceAdditionalDefinitionSpecificationItem;
		SpecificationItem *referenceSecondarySpecificationItem;
//		SpecificationItem *referenceAdditionalProperNounSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkJustification";

		if( referenceJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given reference justification item is undefined" );

		if( searchJustificationItem == NULL )
			return startError( functionNameString, NULL, "I have no justification items" );

		hasFeminineOrMasculineProperNounEnding = referenceJustificationItem->hasFeminineOrMasculineProperNounEnding();
		justificationTypeNr = referenceJustificationItem->justificationTypeNr();
		referencePrimarySpecificationItem = referenceJustificationItem->primarySpecificationItem();
		referenceAdditionalDefinitionSpecificationItem = referenceJustificationItem->additionalDefinitionSpecificationItem();
		referenceSecondarySpecificationItem = referenceJustificationItem->secondarySpecificationItem();
//		referenceAdditionalProperNounSpecificationItem = referenceJustificationItem->additionalProperNounSpecificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem == referenceJustificationItem )
				return startError( functionNameString, NULL, "The given reference justification item is already one of my attached justification items" );

			if( searchJustificationItem->hasJustification( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, referencePrimarySpecificationItem, referenceAdditionalDefinitionSpecificationItem, referenceSecondarySpecificationItem ) )
				return startError( functionNameString, NULL, "I already have a justification item like the given attached justification item" );

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return RESULT_OK;
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

	JustificationItem *firstJustificationItem( unsigned short justificationTypeNr, unsigned short justificationOrderNr )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->justificationOrderNr == justificationOrderNr )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *firstJustificationItem( unsigned short justificationTypeNr, WordItem *primaryGeneralizationWordItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->primaryGeneralizationWordItem() == primaryGeneralizationWordItem )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *firstReplacedPrimarySpecificationJustificationItem()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isReplacedPrimarySpecification() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *firstReplacedSecondarySpecificationJustificationItem()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isReplacedSecondarySpecification() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *firstSecondaryJustificationItem( bool isPossessiveSecondarySpecification )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isPossessiveSecondarySpecification() == isPossessiveSecondarySpecification )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return NULL;
		}

	JustificationItem *primarySpecificationJustificationItem( SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		if( primarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}
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
		// Protected constructed variables

		hasAdjustedSpecificationBeenWritten = false;
		hasSpecificationBeenWrittenAsAnswer = false;
		}

	void storeWrittenSentenceString()
		{
		strcpy( storedSentenceString_, globalVariables()->writtenSentenceString );
		}

	void storedWrittenSentenceStringWithOneSpecificationOnly()
		{
		strcpy( storedSentenceWithOnlyOneSpecificationString_, globalVariables()->writtenSentenceString );
		}

	bool hasCompoundSpecificationCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->isCompoundCollection( specificationCollectionNr_ ) );
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

				( relationContextNr_ > NO_CONTEXT_NR &&
				hasRelationContextCurrentlyBeenUpdated() ) );
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
				specificationWordItem_->isNonExclusiveCollection( specificationCollectionNr_ ) );
		}

	bool hasOnlyOneRelationWord()
		{
		return ( myWordItem()->nContextWords( relationContextNr_ ) == 1 );
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
		WordItem *_myWordItem = myWordItem();

		if( relationContextNr_ > NO_CONTEXT_NR &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != _myWordItem &&
				currentContextWordItem->hasContextCurrentlyBeenUpdatedInWord( relationContextNr_ ) )
					return true;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return false;
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

	bool isConditional()
		{
		return isConditional_;
		}

	bool isCorrectedSpecification()
		{
		return isCorrectedSpecification_;
		}

	bool isCorrectSpecificationArticle( bool isDefiniteArticle, bool isWritingCurrentSpecificationWordOnly, unsigned short articleParameter )
		{
		bool isSelfGenerated;
		SpecificationItem *searchSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *foundWordItem = NULL;

		if( isWritingCurrentSpecificationWordOnly )
			foundWordItem = specificationWordItem_;
		else
			{
			if( ( searchSpecificationItem = myWordItem()->firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ ) ) != NULL )
				{
				isSelfGenerated = isSelfGeneratedSpecification();

				do	{
					if( ( currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ : searchSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGenerated, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, generalizationContextNr_, relationContextNr_ ) ) ) != NULL &&
					!currentSpecificationWordItem->isSpecificationWordTypeAlreadyWritten( specificationWordTypeNr_ ) )
						foundWordItem = currentSpecificationWordItem;
					}
				while( foundWordItem == NULL &&
				( searchSpecificationItem = searchSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion_ ) ) != NULL );
				}
			}

		return ( foundWordItem == NULL ? true : ( isDefiniteArticle ? foundWordItem->isCorrectDefiniteArticle( articleParameter, specificationWordTypeNr_ ) : foundWordItem->isCorrectIndefiniteArticle( articleParameter, specificationWordTypeNr_ ) ) );
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
				isGeneralizationProperNoun() &&
				isSelfGeneratedSpecification() &&

				// Non-possessive specification not in a male word
				( ( !isPossessive_ &&
		!myWordItem()->hasMasculineProperNounEnding() &&
				!myWordItem()->isMasculineWord() ) ||

				// Possessive specification without male relation context
				( isPossessive_ &&
				relationContextNr_ > NO_CONTEXT_NR &&
				specificationWordItem_ != NULL &&
				specificationWordItem_->compoundCollectionNr() > NO_COLLECTION_NR &&
				!hasContextMaleWordInContextWords() ) ) );
		}

	bool isMatchingGeneralizationContextNr( bool isAllowingEmptyRelationContext, unsigned int generalizationContextNr )
		{
		return ( isAllowingEmptyRelationContext ||
				// Empty subset
				generalizationContextNr == NO_CONTEXT_NR ||
				// Same set
				generalizationContextNr_ == generalizationContextNr ? true : myWordItem()->isContextSubsetInContextWords( generalizationContextNr_, generalizationContextNr ) );
		}

	bool isMatchingRelationContextNr( bool isAllowingEmptyRelationContext, unsigned int relationContextNr )
		{		// Empty subset
		return ( relationContextNr == NO_CONTEXT_NR ||
				// Same set
				relationContextNr_ == relationContextNr ||

				( isAllowingEmptyRelationContext &&
				relationContextNr_ == NO_CONTEXT_NR ) ? true : myWordItem()->isContextSubsetInContextWords( relationContextNr_, relationContextNr ) );
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

	bool isRelatedSpecification( unsigned int answerSpecificationCollectionNr, unsigned int questionSpecificationCollectionNr, WordItem *specificationWordItem )
		{
		return ( ( specificationCollectionNr_ > NO_COLLECTION_NR &&
				specificationCollectionNr_ == answerSpecificationCollectionNr ) ||

				( questionSpecificationCollectionNr > NO_COLLECTION_NR &&

				( specificationCollectionNr_ == questionSpecificationCollectionNr ||
				specificationWordItem_ == specificationWordItem ) ) );
		}

	bool isSelfGeneratedAssumption()
		{
		if( !isSpecificationGeneralization_ &&
		// Not a question
		questionParameter_ == NO_QUESTION_PARAMETER &&
		// Self-generated
		firstJustificationItem_ != NULL )
			{
			if( assumptionLevel_ == NO_ASSUMPTION_LEVEL )
				// Calculate assumption level
				calculateAssumptionLevel();

			return ( assumptionLevel_ > NO_ASSUMPTION_LEVEL );
			}

		return false;
		}

	bool isSelfGeneratedConclusion()
		{
		return ( !isSpecificationGeneralization_ &&
				// No assumption level
				assumptionLevel_ == NO_ASSUMPTION_LEVEL &&
				// Not a question
				questionParameter_ == NO_QUESTION_PARAMETER &&
				// Self-generated
				firstJustificationItem_ != NULL );
		}

	bool isSelfGeneratedConclusionOrAssumption()
		{
		return ( !isSpecificationGeneralization_ &&
				// Not a question
				questionParameter_ == NO_QUESTION_PARAMETER &&
				// Self-generated
				firstJustificationItem_ != NULL );
		}

	bool isSelfGeneratedQuestion()
		{
		return ( questionParameter_ > NO_QUESTION_PARAMETER &&
				firstJustificationItem_ != NULL );
		}

	bool isSelfGeneratedSpecification()
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

	bool isSpecificationAdjective()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_ADJECTIVE );
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
				myWordItem()->nContextWords( relationContextNr_ ) > 1 );
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
		return ( relationContextNr_ > NO_CONTEXT_NR &&
				// Self-generated specification
				firstJustificationItem_ != NULL &&
				isSpecificationWordSpanishAmbiguous() &&
				isOlderItem() &&
				!isHiddenSpanishSpecification() &&
				strlen( storedSentenceString_ ) == 0 &&

				( isSelfGeneratedAssumption() ||
				firstJustificationItem_->isPossessivePrimarySpecification() ) );
		}

	unsigned short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
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

	signed char addJustificationToSpecification( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *createdJustificationItem;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addJustificationToSpecification";

		if( isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "I am replaced or deleted" );

		if( isUserSpecification() )
			return startError( functionNameString, NULL, "I am a user specification" );

		if( primarySpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given primary specification item is undefined" );

		if( primarySpecificationItem == this )
			return startError( functionNameString, NULL, "The given primary specification item is the same as me" );

		if( additionalDefinitionSpecificationItem == this )
			return startError( functionNameString, NULL, "The given additional definition specification item is the same as me" );

		if( secondarySpecificationItem == this )
			return startError( functionNameString, NULL, "The given secondary specification item is the same as me" );

		// Create justification
		if( ( justificationResult = myWordItem()->createJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, NO_ORDER_NR, globalVariables()->currentSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to create a justification item to my word" );

		if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
			return startError( functionNameString, NULL, "I couldn't create an additional justification" );

		if( attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to attach the created justification to myself" );

		return RESULT_OK;
		}

	signed char addUserSpecificationToSpanishJustifications( SpecificationItem *userSpecificationItem )
		{
		JustificationItem *currentJustificationItem = firstJustificationItem_;
		SpecificationItem *selfGeneratedSpecificationItem;
		WordItem *secondaryGeneralizationWordItem;
		WordItem *_myWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addUserSpecificationToSpanishJustifications";

		if( userSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given user specification item is undefined" );

		if( specificationWordItem_ == NULL )
			return startError( functionNameString, NULL, "I have no specification word item" );

		do	{
			if( ( secondaryGeneralizationWordItem = currentJustificationItem->secondaryGeneralizationWordItem() ) != NULL &&
			( selfGeneratedSpecificationItem = secondaryGeneralizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, !isPossessive_, true, specificationWordItem_, _myWordItem ) ) != NULL &&
			!selfGeneratedSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
			// Add user specification reversible justification to Spanish primary specification
			selfGeneratedSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to add a user specification reversible justification to the primary specification" );

			currentJustificationItem = currentJustificationItem->attachedJustificationItem();
			}
		while( currentJustificationItem != NULL );

		return RESULT_OK;
		}

	signed char attachJustificationToSpecification( JustificationItem *attachJustificationItem )
		{
		bool _hasRelationContext = ( relationContextNr_ > NO_CONTEXT_NR );
		bool _isQuestion = isQuestion();
		JustificationItem *lastAttachedJustificationItem;
		JustificationItem *obsoleteJustificationItem = NULL;
		JustificationItem *originalFirstJustificationItem;
		JustificationItem *tempJustificationItem;
		SpecificationItem *attachedPrimarySpecificationItem;
		SpecificationItem *attachedSecondarySpecificationItem;
		SpecificationItem *createdSpecificationItem;
		WordItem *_myWordItem = myWordItem();
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "attachJustificationToSpecification";

		if( attachJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given attached justification item is undefined" );

		if( !attachJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given attached justification item isn't active" );

		if( isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "I am replaced or deleted" );

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I have no justification items" );

		if( ( attachedPrimarySpecificationItem = attachJustificationItem->primarySpecificationItem() ) != NULL &&
		!attachedPrimarySpecificationItem->isPossessive() &&
		attachedPrimarySpecificationItem->hasRelationContext() )
			obsoleteJustificationItem = firstJustificationItem_->primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem->specificationWordItem() );
		else
			{
			// Typical for Spanish
			if( ( _isQuestion ||
			isSpecificationWordSpanishAmbiguous() ) &&

			( attachedSecondarySpecificationItem = attachJustificationItem->secondarySpecificationItem() ) != NULL &&
			attachedSecondarySpecificationItem->hasCompoundSpecificationCollection() &&
			!attachedSecondarySpecificationItem->isPossessive() &&
			attachedSecondarySpecificationItem->isUserSpecification() )
				obsoleteJustificationItem = firstJustificationItem_->obsoleteSpanishJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
			}

		if( _hasRelationContext &&
		obsoleteJustificationItem != NULL &&
		// Replace obsolete justification item
		_myWordItem->replaceJustification( obsoleteJustificationItem, attachJustificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to replace the obsolete justification item" );

		if( obsoleteJustificationItem == NULL )
			{
			// Find a justification item with current sentence number, but without attached justification item
			lastAttachedJustificationItem = firstJustificationItem_;

			while( ( tempJustificationItem = lastAttachedJustificationItem->attachedJustificationItem() ) != NULL )
				lastAttachedJustificationItem = tempJustificationItem;

			if( lastAttachedJustificationItem->hasCurrentCreationSentenceNr() )
				{
				if( lastAttachedJustificationItem->attachJustification( attachJustificationItem, this ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to attach a justification item" );

				if( !_hasRelationContext &&
				!isNegative_ &&
				!_isQuestion &&
				assumptionLevel_ > NO_ASSUMPTION_LEVEL &&
				// Recalculate my assumption level
				recalculateAssumptionLevel( false ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to recalculate my assumption level" );
				}
			else
				{
				if( attachJustificationItem->attachedJustificationItem() != NULL )
					return startError( functionNameString, NULL, "The given attached justification has a justification attached" );

				if( hasCurrentCreationSentenceNr() )
					{
					originalFirstJustificationItem = firstJustificationItem_;

					if( changeFirstJustification( true, attachJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to change my first justification item" );

					if( attachJustificationItem->attachJustification( originalFirstJustificationItem, this ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to attach the first justification item of myself to the given attached justification item" );
					}
				else
					{
					// Create copy of myself with different first justification item
					if( ( createAndAssignResult = _myWordItem->createSpecificationItem( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isCharacteristicFor_, isConditional(), isCorrectedSpecification_, isEveryGeneralization_, isExclusiveGeneralization_, isExclusiveSpecification_, isNegative_, isPartOf_, isPossessive_, isSpecific_, isSpecificationGeneralization_, isUncountableGeneralizationNoun_, isUniqueUserRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, languageNr(), prepositionParameter(), questionParameter(), generalizationWordTypeNr(), specificationWordTypeNr(), relationWordTypeNr(), specificationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nContextRelations_, attachJustificationItem, specificationWordItem_, specificationString_, storedSentenceString_, storedSentenceWithOnlyOneSpecificationString_ ) ).result != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to create a copy of myself with a different first justification item" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
						return startError( functionNameString, NULL, "I couldn't create a specification" );

					if( attachJustificationItem->attachJustification( firstJustificationItem_, createdSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to attach the first justification item of the created specification item, to the given attached justification item of the created specification item" );

					if( _myWordItem->replaceOrDeleteSpecification( this, createdSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to replace or delete a specification" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char calculateAssumptionLevel()
		{
		bool hasFoundReversibleAssumptionOrConclusion;
		bool _hasRelationContext = ( relationContextNr_ > NO_CONTEXT_NR );
		bool isCheckingRelationWords;
		bool isBlockingHighestAssumptionLevel;
		bool isForcingHighestAssumptionLevel;
		bool isOlderSpecification = isOlderItem();
		bool isReversibleAssumptionOrConclusion;
		bool _isSpecificationWordSpanishAmbiguous = false;
		unsigned short highestAssumptionLevel;
		unsigned short lowestAssumptionLevel = MAX_LEVEL;
		unsigned int nJustificationRelationWords;
		unsigned int nRelationWords;
		unsigned int justificationAssumptionLevel;
		JustificationItem *currentJustificationItem = firstJustificationItem_;
		JustificationItem *searchJustificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "calculateAssumptionLevel";

		if( isSpecificationGeneralization_ )
			return startError( functionNameString, NULL, "I am a specification-generalization" );

		if( questionParameter_ > NO_QUESTION_PARAMETER )
			return startError( functionNameString, NULL, "I am a question" );

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I am a user specification" );

		if( assumptionLevel_ > NO_ASSUMPTION_LEVEL ||

		( ( _hasRelationContext ||
		!isOlderSpecification ||
		!hasCurrentCreationSentenceNr() ) &&

		hasAtLeastOneAssumptionJustification() ) )
			{
			hasFoundReversibleAssumptionOrConclusion = ( firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != NULL );
			_isSpecificationWordSpanishAmbiguous = isSpecificationWordSpanishAmbiguous();
			nRelationWords = nRelationContextWords();

			do	{
				isCheckingRelationWords = true;
				isBlockingHighestAssumptionLevel = false;
				isForcingHighestAssumptionLevel = false;
				highestAssumptionLevel = NO_ASSUMPTION_LEVEL;
				nJustificationRelationWords = 0;
				searchJustificationItem = currentJustificationItem;

				do	{
					if( _hasRelationContext )
						nJustificationRelationWords += searchJustificationItem->nJustificationContextRelations( relationContextNr_, nRelationWords );

					// Calculate assumption level
					if( ( justificationAssumptionLevel = searchJustificationItem->justificationAssumptionLevel() ) > MAX_LEVEL )
						return startSystemError( functionNameString, NULL, "Assumption level overflow" );

					if( isCheckingRelationWords )
						{
						if( justificationAssumptionLevel > highestAssumptionLevel )
							{
							if( hasFoundReversibleAssumptionOrConclusion &&
							justificationAssumptionLevel > assumptionLevel_ )
								{
								isReversibleAssumptionOrConclusion = searchJustificationItem->isReversibleAssumptionOrConclusion();

								// Opposite possessive specification assumption, or Exclusive specification substitution assumption
								if( ( !isReversibleAssumptionOrConclusion &&
								isOlderSpecification &&

								( assumptionLevel_ == NO_ASSUMPTION_LEVEL ||
								highestAssumptionLevel > NO_ASSUMPTION_LEVEL ||
								searchJustificationItem->isExclusiveSpecificationSubstitutionAssumption() ) ) ||

								// Reversible assumption
								( isReversibleAssumptionOrConclusion &&
								highestAssumptionLevel == NO_ASSUMPTION_LEVEL &&
								// Typical for Spanish
								!_isSpecificationWordSpanishAmbiguous &&
								hasCompoundSpecificationCollection() ) )
									isForcingHighestAssumptionLevel = true;
								else
									{
									if( searchJustificationItem->isExclusiveSpecificationSubstitutionAssumption() )
										isBlockingHighestAssumptionLevel = true;
									}
								}
							else
								{
								// Typical for Spanish
								// Spanish test file: "Complejo (19 - extraño)"
								if( isOlderSpecification &&
								justificationAssumptionLevel < assumptionLevel_ &&
								searchJustificationItem->hasCurrentCreationSentenceNr() &&
								searchJustificationItem->isExclusiveSpecificationSubstitutionAssumption() &&
								searchJustificationItem->isPossessivePrimarySpecification() &&
								searchJustificationItem->isOlderItem() )
									isBlockingHighestAssumptionLevel = true;
								}

							highestAssumptionLevel = (unsigned short)justificationAssumptionLevel;
							}
						}
					else
						{
						if( justificationAssumptionLevel < highestAssumptionLevel )
							highestAssumptionLevel = (unsigned short)justificationAssumptionLevel;
						}

					if( !_hasRelationContext )
						isCheckingRelationWords = false;
					}
				while( ( searchJustificationItem = searchJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );

				if( highestAssumptionLevel < lowestAssumptionLevel &&

				( isForcingHighestAssumptionLevel ||

				( nJustificationRelationWords > nRelationWords &&

				// Test files: "Complex (18)", "Complex (19 - mixed)" and "Complex (19 - strange)"
				( !hasFoundReversibleAssumptionOrConclusion ||
				highestAssumptionLevel <= assumptionLevel_ ) ) ||

				( !isBlockingHighestAssumptionLevel &&
				nJustificationRelationWords == nRelationWords &&

				( isNegative_ ||
				// No assumption level
				assumptionLevel_ == NO_ASSUMPTION_LEVEL ||

				// First set of justifications
				( ( lowestAssumptionLevel == MAX_LEVEL &&
				highestAssumptionLevel < assumptionLevel_ ) ||

				// Assumption level
				( !_isSpecificationWordSpanishAmbiguous &&
				assumptionLevel_ + 2 > highestAssumptionLevel ) ) ) ) ) )
					lowestAssumptionLevel = highestAssumptionLevel;
				}
			while( ( currentJustificationItem = currentJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != NULL );

			if( lowestAssumptionLevel < MAX_LEVEL &&
			// Assumption level has changed
			lowestAssumptionLevel != assumptionLevel_ )
				{
				assumptionLevel_ = lowestAssumptionLevel;

				// Clear stored sentence buffer to lose the previous assumption level word
				clearStoredSentenceString();
				}
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

	signed char markAsAnsweredQuestion()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markAsAnsweredQuestion";

		if( hasCurrentCreationSentenceNr() )
			isAnsweredQuestion_ = true;
		else
			{
			if( myWordItem()->copyAndReplaceSpecification( true, isExclusiveGeneralization_, generalizationCollectionNr_, specificationCollectionNr_, firstJustificationItem_, this ).result != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to copy and replace myself as an answered question" );
			}

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

	signed char recalculateAssumptionLevel( bool isSituationStable )
		{
		bool hasChangedAssumptionLevel;
		bool _hasRelationContext = ( relationContextNr_ > NO_CONTEXT_NR );
		bool isOlderSpecification = isOlderItem();
		unsigned short previousAssumptionLevel = assumptionLevel_;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptionLevel";

		// Calculate assumption level
		if( calculateAssumptionLevel() != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to calculate my assumption level" );

		if( hasCurrentCreationSentenceNr() )
			{
			hasChangedAssumptionLevel = ( previousAssumptionLevel != assumptionLevel_ );

			if( ( ( isOlderSpecification &&
			!isNegative_ ) ||

			( isSituationStable &&

			// No relation context
			( ( !_hasRelationContext &&

			( isNegative_ ||

			// Test file: "John - Anna (before family definition)"
			( !hasChangedAssumptionLevel &&
			isPartOf_ ) ) ) ||

			// Relation context
			( _hasRelationContext &&
			hasNonCompoundSpecificationCollection() &&

			( isPossessive_ ||
			myWordItem()->isUserRelationWord ||
			firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) == NULL ) ) ) ) ) &&

			// Remove obsolete assumption justifications
			removeObsoleteAssumptionJustifications( isOlderSpecification, isSituationStable ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to remove obsolete assumption justifications from this assumption" );

			if( hasChangedAssumptionLevel &&
			isOlderSpecification &&
			// Process changed assumption level
			processChangedAssumption( previousAssumptionLevel ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to process the changed assumption level" );
			}

		return RESULT_OK;
		}

	signed char removeObsoleteAssumptionJustifications( bool isOnlySelectingOlderJustifications, bool isSituationStable )
		{
		bool hasConfirmedAnySpecification = globalVariables()->hasConfirmedAnySpecification;
		bool _hasOnlyOneRelationWord;
		bool _hasRelationContext = ( relationContextNr_ > NO_CONTEXT_NR );
		bool isIncludingNegativeAssumptionOrConclusion;
		bool isIncludingReversibleAssumptionOrConclusion;
		bool isIncludingSpecificationSubstitutionAssumptionOrConclusion;
		JustificationItem *attachedJustificationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *currentJustificationItem = firstJustificationItem_;
		JustificationItem *previousJustificationItem = NULL;
		JustificationItem *previousPreviousJustificationItem = NULL;
		SpecificationItem *userAssignmentItem;
		WordItem *_myWordItem = myWordItem();
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeObsoleteAssumptionJustifications";

		if( questionParameter_ > NO_QUESTION_PARAMETER )
			return startError( functionNameString, NULL, "I am a question" );

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I am a user specification" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		isIncludingNegativeAssumptionOrConclusion = ( ( assumptionLevel_ == NO_ASSUMPTION_LEVEL &&

													( !hasConfirmedAnySpecification ||
													!isOnlySelectingOlderJustifications ||
													// Test file: "My assumptions that are confirmed (John)"
													!firstJustificationItem_->hasPrimarySpecificationCurrentCreationSentenceNr() ) ) ||

													( !hasConfirmedAnySpecification &&
													// Typical for Spanish
													// Spanish test file: "Complejo (13)"
													firstJustificationItem_->justificationAssumptionLevel() == assumptionLevel_ ) );

		isIncludingReversibleAssumptionOrConclusion = ( isSituationStable ||
														!hasNewInformation() ||
														// Typical for Spanish
														isHiddenSpanishSpecification() );

		isIncludingSpecificationSubstitutionAssumptionOrConclusion = ( !hasConfirmedAnySpecification ||

																		( isSituationStable &&
																		specificationCollectionNr_ == NO_COLLECTION_NR ) );

		_hasOnlyOneRelationWord = ( _hasRelationContext &&
									isSituationStable &&
									hasOnlyOneRelationWord() );

		do	{
			if( currentJustificationItem->isObsoleteAssumptionJustification(_hasOnlyOneRelationWord, isIncludingNegativeAssumptionOrConclusion, isIncludingReversibleAssumptionOrConclusion, isIncludingSpecificationSubstitutionAssumptionOrConclusion, isOnlySelectingOlderJustifications, assumptionLevel_) )
				{
				attachedJustificationItem = currentJustificationItem->attachedJustificationItem();

				if( previousJustificationItem == NULL ||
				previousJustificationItem->hasCurrentCreationSentenceNr() )
					{
					if( currentJustificationItem->hasCurrentCreationSentenceNr() )
						{
						// Disconnect current justification item
						if( currentJustificationItem->changeAttachedJustification( NULL ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change the attached justification item of the current justification item" );
						}
					else
						{
						// Replace current justification
						if( _myWordItem->replaceOrDeleteJustification( currentJustificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace the current justification item" );
						}

					if( previousJustificationItem == NULL )
						{
						if( attachedJustificationItem != NULL &&
						// Change first justification item
						changeFirstJustification( !currentJustificationItem->isReplacedItem(), attachedJustificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change my first justification item" );
						}
					else
						{
						// Remove current justification item
						if( previousJustificationItem->changeAttachedJustification( attachedJustificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change the attached justification item of the previous justification item" );
						}

					currentJustificationItem = attachedJustificationItem;
					}
				else
					{
					if( previousPreviousJustificationItem == NULL ||
					previousPreviousJustificationItem->hasCurrentCreationSentenceNr() )
						{
						// Copy previous justification
						if( ( justificationResult = _myWordItem->copyJustificationItem( previousJustificationItem->primarySpecificationItem(), previousJustificationItem->secondarySpecificationItem(), attachedJustificationItem, previousJustificationItem ) ).result != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to copy the previous justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
							return startError( functionNameString, NULL, "The created justification item is undefined" );

						if( previousPreviousJustificationItem == NULL )
							{
							// Change first justification item to created justification
							if( changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to change my first justification item to the created justification" );
							}
						else
							{
							// Change attached justification item of previous previous justification to created justification
							if( previousPreviousJustificationItem->changeAttachedJustification( createdJustificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to change the attached justification item of the previous previous justification item to the created justification" );
							}

						// Replace previous justification
						if( _myWordItem->replaceOrDeleteJustification( previousJustificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace the previous justification item" );

						// Replace current justification
						if( _myWordItem->replaceOrDeleteJustification( currentJustificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace the current justification item" );

						// Reset variables
						previousPreviousJustificationItem = NULL;
						previousJustificationItem = NULL;
						currentJustificationItem = firstJustificationItem_;
						}
					else
						{
						previousPreviousJustificationItem = previousJustificationItem;
						previousJustificationItem = currentJustificationItem;
						currentJustificationItem = currentJustificationItem->attachedJustificationItem();
						}
					}
				}
			else
				{
				if( _hasRelationContext &&
				!hasAdjustedSpecificationBeenWritten &&
				!isIncludingNegativeAssumptionOrConclusion &&
				!isIncludingReversibleAssumptionOrConclusion &&
				currentJustificationItem->isSpecificationSubstitutionAssumptionOrConclusion() &&
				!currentJustificationItem->hasPrimarySpecificationRelationContext() &&
				( userAssignmentItem = _myWordItem->firstAssignmentItem( false, false, NO_CONTEXT_NR, currentJustificationItem->primarySpecificationWordItem() ) ) != NULL )
					{
					// Copy current justification
					if( ( justificationResult = _myWordItem->copyJustificationItem( userAssignmentItem, currentJustificationItem->secondarySpecificationItem(), currentJustificationItem->attachedJustificationItem(), currentJustificationItem ) ).result != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to copy the previous justification item" );

					if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
						return startError( functionNameString, NULL, "The created justification item is undefined" );

					// Replace specification without relation context, by assignment specification
					if( _myWordItem->replaceJustification( currentJustificationItem, createdJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to replace a specification without relation context, by an assignment specification" );

					// Start from scratch
					previousPreviousJustificationItem = NULL;
					previousJustificationItem = NULL;
					currentJustificationItem = firstJustificationItem_;
					}
				else
					{
					previousPreviousJustificationItem = previousJustificationItem;
					previousJustificationItem = currentJustificationItem;
					currentJustificationItem = currentJustificationItem->attachedJustificationItem();
					}
				}
			}
		while( currentJustificationItem != NULL );

		// Typical for Spanish
		// Spanish test file: "Complejo (12)"
		if( _hasRelationContext &&
		isOnlySelectingOlderJustifications &&
		isPossessive_ &&
		!isSituationStable &&
		isHiddenSpanishSpecification() &&
		// (Re)calculate assumption level
		calculateAssumptionLevel() != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to (re)calculate my assumption level" );

		return RESULT_OK;
		}

	signed char writeSpecificationConflict( bool isQuestion, bool isWritingCurrentSpecificationWordOnly )
		{
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationConflict";

		if( myWordItem()->writeSelectedSpecification( false, isWritingCurrentSpecificationWordOnly, this ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write a conflicting specification" );

		if( ( writtenSentenceString = globalVariables()->writtenSentenceString ) == NULL ||
		strlen( writtenSentenceString ) == 0 )
			return startError( functionNameString, NULL, "The written sentence string is empty" );

		if( inputOutput()->writeInterfaceText( true, ( isQuestion ? INPUT_OUTPUT_PROMPT_WRITE : INPUT_OUTPUT_PROMPT_WARNING ), ( isOlderItem() ? ( isQuestion ? INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_DEFINITION : INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_DEFINITION ) : INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write an interface warning" );

		if( inputOutput()->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables()->learnedFromUserString ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to write the conflict sentence" );

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
		return ( specificationWordItem_ != NULL ?
				specificationWordItem_->activeWordTypeString( specificationWordTypeNr_ ) : NULL );
		}

	char *storedSentenceString()
		{
		return storedSentenceString_;
		}

	char *storedSentenceWithOnlyOneSpecificationString()
		{
		return storedSentenceWithOnlyOneSpecificationString_;
		}

	JustificationItem *firstJustificationItem()
		{
		return firstJustificationItem_;
		}

	SpecificationItem *candidateForQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification, bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isCandidateForQuestion( isAllowingSpanishPossessiveSpecification ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *exclusiveSpecificationItem( bool isIncludingAdjectives, bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isExclusiveSpecification() &&
			!searchSpecificationItem->isAnsweredQuestion() &&

			( isIncludingAdjectives ||
			!searchSpecificationItem->isSpecificationAdjective() ) )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *olderNonPossessiveNonQuestionSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isOlderItem() &&
			!searchSpecificationItem->isPossessive() &&
			!searchSpecificationItem->isQuestion() &&
			searchSpecificationItem->hasSpecificationCollection() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nonNegativeNonPossessiveDefinitionSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isNegative() &&
			!searchSpecificationItem->isPossessive() &&
			!searchSpecificationItem->isSpecificationGeneralization() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nonQuestionSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( ( isAllowingEmptyRelationContext ||
			searchSpecificationItem->hasRelationContext() ) &&

			!searchSpecificationItem->isQuestion() &&
			!searchSpecificationItem->isHiddenSpanishSpecification() )
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

	SpecificationItem *negativeSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->isNegative() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nextExclusiveSpecificationItem( bool isIncludingAdjectives )
		{
		return exclusiveSpecificationItem( isIncludingAdjectives, false );
		}

	SpecificationItem *nextOlderNonPossessiveNonQuestionSpecificationItem()
		{
		return olderNonPossessiveNonQuestionSpecificationItem( false );
		}

	SpecificationItem *nextNegativeSpecificationItem()
		{
		return negativeSpecificationItem( false );
		}

	SpecificationItem *nextNonNegativeNonPossessiveDefinitionSpecificationItem()
		{
		return nonNegativeNonPossessiveDefinitionSpecificationItem( false );
		}

	SpecificationItem *nextPossessiveSpecificationItem()
		{
		return possessiveSpecificationItem( false );
		}

	SpecificationItem *nextCandidateForQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification )
		{
		return candidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification, false );
		}

	SpecificationItem *nextNonQuestionNonRelationSpecificationItem()
		{
		return nonQuestionSpecificationItem( false, false );
		}

	SpecificationItem *nextSelectedSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, isQuestion() ) :
									specificationItem( false, false, isQuestion() ) );
		}

	SpecificationItem *nextSelectedQuestionParameterSpecificationItem()
		{
		return ( isAssignment() ? assignmentItem( false, false, questionParameter_ ) :
									specificationItem( false, false, questionParameter_ ) );
		}

	SpecificationItem *nextSelectedQuestionParameterSpecificationItem( bool isIncludingAnsweredQuestions )
		{
		return ( isAssignment() ? assignmentItem( isIncludingAnsweredQuestions, false, questionParameter_ ) :
									specificationItem( isIncludingAnsweredQuestions, false, questionParameter_ ) );
		}

	SpecificationItem *nextSpecificationItem()
		{
		return (SpecificationItem *)nextItem;
		}

	SpecificationItem *updatedSpecificationItem()
		{
		SpecificationItem *updatedSpecificationItem;
		SpecificationItem *searchSpecificationItem = this;

		while( ( updatedSpecificationItem = searchSpecificationItem->replacingSpecificationItem ) != NULL )
			searchSpecificationItem = updatedSpecificationItem;

		return searchSpecificationItem;
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

	WordItem *relatedSpecificationWordItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool _isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				isSelfGeneratedSpecification() == _isSelfGeneratedSpecification &&
				specificationCollectionNr > NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				relationContextNr_ == relationContextNr &&

				( assumptionLevel_ == assumptionLevel ||
				questionParameter_ > NO_QUESTION_PARAMETER ) &&

				( specificationWordTypeNr == NO_WORD_TYPE_NR ||
				specificationWordTypeNr_ == specificationWordTypeNr ) ? specificationWordItem_ : NULL );
		}

	WordItem *relatedSpecificationWordItem( bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr )
		{
		return ( isPossessive_ == isPossessive &&

				( isIgnoringExclusive ||
				isExclusiveSpecification_ == isExclusiveSpecification ) &&

				( isIgnoringNegative ||
				isNegative_ == isNegative ) &&

				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				isMatchingWordType( specificationWordTypeNr_, specificationWordTypeNr ) &&

				( relationContextNr_ == relationContextNr ||

				( !isCheckingRelationContext &&
				!isHiddenSpanishSpecification() ) ) ? specificationWordItem_ : NULL );
		}

	WordItem *singleRelationWordItem()
		{
		WordItem *firstRelationWordItem;

				// First relation word
		return ( ( firstRelationWordItem = relationContextWordItemInContextWords( NULL ) ) != NULL &&
				// No another relation word
				relationContextWordItemInContextWords( firstRelationWordItem ) == NULL ? firstRelationWordItem : NULL );
		}

	WordItem *specificationWordItem()
		{
		return specificationWordItem_;
		}
	};
#endif

/*************************************************************************
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his covenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *************************************************************************/
