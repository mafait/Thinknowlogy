/*	Class:			SpecificationItem
 *	Purpose:		Storing info about the specification structure of a word
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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
	unsigned int relationCollectionNr_ = NO_COLLECTION_NR;

	unsigned int generalizationContextNr_ = NO_CONTEXT_NR;
	unsigned int specificationContextNr_ = NO_CONTEXT_NR;
	unsigned int relationContextNr_ = NO_CONTEXT_NR;

	unsigned int nEnteredRelationWords_ = 0;

	JustificationItem *firstJustificationItem_ = NULL;

	WordItem *specificationWordItem_ = NULL;
	WordItem *relationWordItem_ = NULL;

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

	signed char calculateAssumptionLevel()
		{
		bool _hasRelationWord = ( relationWordItem_ != NULL );
		bool isBlockingHighestAssumptionLevel = false;
		bool isCheckingRelationWords;
		bool isForcingHighestAssumptionLevel;
		bool isOlderSpecification = isOlderItem();
		bool isReversibleAssumptionOrConclusion;
		bool _isSpecificationWordSpanishAmbiguous = isSpecificationWordSpanishAmbiguous();
		unsigned short highestAssumptionLevel;
		unsigned short lowestAssumptionLevel = MAX_LEVEL;
		unsigned int nJustificationRelationWords;
		unsigned int _nRelationWords = nRelationWords();
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

		do	{
			isCheckingRelationWords = true;
			isForcingHighestAssumptionLevel = false;
			highestAssumptionLevel = NO_ASSUMPTION_LEVEL;
			nJustificationRelationWords = 0;
			searchJustificationItem = currentJustificationItem;

			do	{
				if( _hasRelationWord )
					nJustificationRelationWords += searchJustificationItem->nJustificationRelationWords( relationCollectionNr_, _nRelationWords, relationWordItem_ );

				// Calculate assumption level
				if( ( justificationAssumptionLevel = searchJustificationItem->justificationAssumptionLevel() ) > MAX_LEVEL )
					return startSystemError( functionNameString, NULL, "Assumption level overflow" );

				if( isCheckingRelationWords )
					{
					if( justificationAssumptionLevel > highestAssumptionLevel )
						{
						if( justificationAssumptionLevel > assumptionLevel_ )
							{
							isReversibleAssumptionOrConclusion = searchJustificationItem->isReversibleAssumptionOrConclusion();

							// Opposite possessive specification assumption, or Exclusive specification substitution assumption
							if( ( !isReversibleAssumptionOrConclusion &&
							isOlderSpecification &&

								// Test files: "reasoning\family\Complex (13)",
								//				"reasoning\family\Complex (18)",
								//				"reasoning\family\Complex (19 - mixed)",
								//				"reasoning\family\Complex (19 - strange)"
								( searchJustificationItem->isExclusiveSpecificationSubstitutionAssumption() ||

								// Typical for Spanish
								// Spanish test files: "conflicto\familia\José es una madre",
								//						"razonamiento\familia\Complejo (12)",
								//						"razonamiento\familia\Complejo (13)",
								//						"razonamiento\familia\Complejo (14)",
								//						"razonamiento\familia\Complejo (15)",
								//						among others
								assumptionLevel_ == NO_ASSUMPTION_LEVEL ) ) ||

							// Reversible assumption
							// Test files: "reasoning\family\Complex (13)",
							//				"reasoning\family\Complex (18)",
							//				"reasoning\family\Complex (19 - mixed)",
							//				"reasoning\family\Complex (19 - strange)"
							( isReversibleAssumptionOrConclusion &&
							assumptionLevel_ > NO_ASSUMPTION_LEVEL &&
							searchJustificationItem == currentJustificationItem &&
							hasCompoundSpecificationCollection() ) )
								isForcingHighestAssumptionLevel = true;
							}
						else
							{
							// Typical for Spanish
							// Spanish test files: "razonamiento\familia\Complejo (19 - extraño)",
							//						"razonamiento\familia\Paz - José - Juan y Ana"
							if( isOlderSpecification &&
							justificationAssumptionLevel < assumptionLevel_ &&
							relationCollectionNr_ > NO_COLLECTION_NR &&
							searchJustificationItem->isExclusiveSpecificationSubstitutionAssumption() &&
							searchJustificationItem->hasCurrentCreationSentenceNr() &&
							searchJustificationItem->isPossessivePrimarySpecification() )
								isBlockingHighestAssumptionLevel = true;
							}

						highestAssumptionLevel = (unsigned short)justificationAssumptionLevel;
						}
					}
				else
					{
					// Test files: "reasoning\family\Complex (1)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				"reasoning\family\Complex (9)",
					//				"reasoning\family\Complex (15)",
					//				"reasoning\family\I know (11)",
					//				among others
					if( justificationAssumptionLevel < highestAssumptionLevel )
						highestAssumptionLevel = (unsigned short)justificationAssumptionLevel;
					}

				// Test files: "reasoning\family\Avoid duplicate context",
				//				"reasoning\family\Complex (1)",
				//				"reasoning\family\Complex (4)",
				//				"reasoning\family\Complex (5)",
				//				"reasoning\family\Complex (6)",
				//				"reasoning\family\Complex (9)",
				//				among others
				if( !_hasRelationWord )
					isCheckingRelationWords = false;
				}
			while( !isBlockingHighestAssumptionLevel &&
			( searchJustificationItem = searchJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );

			if( !isBlockingHighestAssumptionLevel &&
			highestAssumptionLevel < lowestAssumptionLevel &&

				// Reversible assumption
				// Test files: "reasoning\family\Complex (13)",
				//				"reasoning\family\Complex (18)",
				//				"reasoning\family\Complex (19 - mixed)",
				//				"reasoning\family\Complex (19 - strange)"
				( isForcingHighestAssumptionLevel ||

				// Test files: "conflict\family\Joe is a mother",
				//				"reasoning\family\Complex (14)",
				//				"reasoning\family\Joe is a son of John and Anna",
				//				"reasoning\family\My assumptions that are confirmed (Paul and John)",
				//				"reasoning\family\Paul - John and Anna - Joe - Laura",
				//				"reasoning\family\You have confirmed at least one relation word"
				( nJustificationRelationWords > _nRelationWords &&
				highestAssumptionLevel <= assumptionLevel_ ) ||

				( nJustificationRelationWords == _nRelationWords &&

					// No assumption level
					// Test files: "reasoning\family\I know (11)",
					//				"reasoning\family\Joe has 2 parents - Joe is a child",
					//				"reasoning\family\Joe is a child - Joe has 2 parents",
					//				"reasoning\family\Joe is a child - John and Anna",
					//				among others
					( assumptionLevel_ == NO_ASSUMPTION_LEVEL ||

					// Assumption level
					// Test files: "conflict\family\Joe is a mother",
					//				"reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				"reasoning\family\Complex (4)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				among others
						( !_isSpecificationWordSpanishAmbiguous &&
					assumptionLevel_ + 2 > highestAssumptionLevel ) ||

					// Typical for Spanish
					// Spanish test file: "razonamiento\familia\Complejo (13)"
					isNegative_ ||

					// Typical for Spanish
					// First set of justifications
					// Spanish test file: "conflicto\familia\José es una madre",
					//						"razonamiento\familia\Complejo (1)",
					//						"razonamiento\familia\Complejo (6)",
					//						"razonamiento\familia\Complejo (9)",
					//						"razonamiento\familia\Complejo (11)",
					//						among others
					highestAssumptionLevel < assumptionLevel_ ) ) ) )
				lowestAssumptionLevel = highestAssumptionLevel;
			}
		while( !isBlockingHighestAssumptionLevel &&
		( currentJustificationItem = currentJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem_ ) ) != NULL );

		if( !isBlockingHighestAssumptionLevel &&
		lowestAssumptionLevel < MAX_LEVEL &&
		// Assumption level has changed
		lowestAssumptionLevel != assumptionLevel_ )
			{
			assumptionLevel_ = lowestAssumptionLevel;

			// Clear stored sentence buffer to lose the previous assumption level word
			clearStoredSentenceString();
			}

		return RESULT_OK;
		}

	unsigned short highestJustificationAssumptionLevel()
		{
		unsigned short highestAssumptionLevel = NO_ASSUMPTION_LEVEL;
		unsigned short searchAssumptionLevel;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( ( searchAssumptionLevel = searchJustificationItem->justificationAssumptionLevel() ) > highestAssumptionLevel )
				highestAssumptionLevel = searchAssumptionLevel;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return highestAssumptionLevel;
		}

	signed char addJustificationsToSpecificationsOfRelationWords( WordItem *currentRelationWordItem, WordItem *partOfSpecificationWordItem )
		{
		SpecificationItem *additionalSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *reversibleSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *oppositeSpecificationWordItem;
		WordItem *_myWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addJustificationsToSpecificationsOfRelationWords";

		if( currentRelationWordItem == NULL )
			return startError( functionNameString, NULL, "The given current relation word item is undefined" );

		if( specificationWordItem_ == NULL )
			return startError( functionNameString, NULL, "I have no specification word" );

		if( ( reversibleSpecificationItem = currentRelationWordItem->bestMatchingRelationSpecificationItem( true, true, false, !isPossessive_, specificationWordItem_, _myWordItem ) ) != NULL &&
		!reversibleSpecificationItem->isHiddenSpanishSpecification() )
			{
			if( !hasOnlyOneRelationWord() &&
			reversibleSpecificationItem->isSelfGeneratedSpecification() )
				{
				if( ( reversibleSpecificationItem->isOlderItem() ||
				_myWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() ) &&

				// Justification doesn't exist yet in reversible specification
				!reversibleSpecificationItem->hasPrimarySpecificationJustification( this ) &&
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
						if( ( partOfSpecificationItem = currentRelationWordItem->firstPartOfSpecificationItem( partOfSpecificationWordItem ) ) != NULL &&
						// Justification doesn't exist yet in part-of specification
						!partOfSpecificationItem->hasPrimarySpecificationJustification( reversibleSpecificationItem ) &&
						( definitionSpecificationItem = partOfSpecificationWordItem->firstPartOfSpecificationItem( specificationWordItem_ ) ) != NULL &&
						// Justification has at least the same assumption level
						partOfSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, NULL, definitionSpecificationItem, NULL ) &&
						// Add non-possessive part-of justification to part-of specification
						partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, reversibleSpecificationItem, NULL, definitionSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to add a non-possessive part-of justification to a part-of specification" );
						}
					else
						{
						if( ( partOfSpecificationItem = _myWordItem->firstPartOfSpecificationItem( partOfSpecificationWordItem ) ) != NULL &&
						// Justification doesn't exist yet in part-of specification
						!partOfSpecificationItem->hasPrimarySpecificationJustification( this ) &&
						( definitionSpecificationItem = partOfSpecificationWordItem->firstPartOfSpecificationItem( specificationWordItem_ ) ) != NULL &&
						// Justification has at least the same assumption level
						partOfSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, NULL, definitionSpecificationItem, NULL ) &&
						// Skip if specification exists without relation word
						_myWordItem->bestMatchingRelationSpecificationItem( true, false, false, false, false, false, false, false, specificationCollectionNr_, NO_COLLECTION_NR, specificationWordItem_, NULL ) == this &&
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

			if( reversibleSpecificationItem->assumptionLevel() < assumptionLevel_ &&

				// Test files: "reasoning\family\Complex (13)",
				//				"reasoning\family\Complex (18)",
				//				"reasoning\family\Complex (19 - mixed)",
				//				"reasoning\family\Complex (19 - strange)"
				( hasCompoundSpecificationCollection() ||

				// Test files: "reasoning\family\Complex (14)",
				//				"reasoning\family\John - Anna (before family definition)",
				//				"reasoning\family\John is the father - Paul - Laura"
				!reversibleSpecificationItem->hasOnlyOneRelationWord() ) &&

			// Recalculate assumption levels of specifications in context word
			currentRelationWordItem->recalculateAssumptionLevelsInWord( false ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to recalculate the assumption levels of specifications in word \"", currentRelationWordItem->anyWordTypeString(), "\"" );
			}

		if( isPossessive_ &&
		partOfSpecificationWordItem != NULL )
			{
			// Part-of
			if( ( partOfSpecificationItem = currentRelationWordItem->firstPartOfSpecificationItem( partOfSpecificationWordItem ) ) != NULL &&
			// Part-of specification has no similar justification
			partOfSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, _myWordItem ) == NULL &&
			// Justification doesn't exist yet in part-of specification
			!partOfSpecificationItem->hasPrimarySpecificationJustification( this ) &&
			( definitionSpecificationItem = partOfSpecificationWordItem->firstPartOfSpecificationItem( specificationWordItem_ ) ) != NULL &&
			// Justification has at least the same assumption level
			partOfSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, NULL, definitionSpecificationItem, NULL ) &&
			// Add possessive part-of justification to part-of specification
			partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, this, NULL, definitionSpecificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to add a possessive part-of justification to a part-of specification" );

			if( !specificationWordItem_->isNounWordSpanishAmbiguous() &&
			( oppositeSpecificationWordItem = specificationWordItem_->collectionWordItem( specificationCollectionNr_ ) ) != NULL )
				{
				// Add justification to additional specification
				if( ( definitionSpecificationItem = partOfSpecificationWordItem->firstSpecificationItem( false, true, false, specificationWordItem_ ) ) != NULL )
					{
					// My word
					if( !_myWordItem->hasCurrentlyConfirmedSpecification() &&
					( additionalSpecificationItem = _myWordItem->firstSpecificationItem( false, false, false, oppositeSpecificationWordItem ) ) != NULL &&
					// Justification doesn't exist yet in part-of specification
					!additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, this ) &&
					// Justification has at least the same assumption level
					additionalSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, this, NULL ) &&
					// Add opposite possessive assumption justification to additional specification of my word
					additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, this ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add an opposite possessive assumption justification to an additional specification of my word" );

					// Current relation word
					if( ( additionalSpecificationItem = currentRelationWordItem->firstSpecificationItem( false, true, false, oppositeSpecificationWordItem ) ) != NULL &&
					// Justification doesn't exist yet in part-of specification
					!additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, this ) &&
					// Justification has at least the same assumption level
					additionalSpecificationItem->assumptionLevel() >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, this, NULL ) &&
					// Add opposite possessive assumption justification to additional specification of current relation word
					additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, this ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add an opposite possessive assumption justification to an additional specification of the current relation word" );
					}

				// Add justification to me
				if( ( definitionSpecificationItem = partOfSpecificationWordItem->firstSpecificationItem( false, true, false, oppositeSpecificationWordItem ) ) != NULL )
					{
					// My word
					if( ( secondarySpecificationItem = _myWordItem->bestMatchingRelationSpecificationItem( false, false, false, false, oppositeSpecificationWordItem, currentRelationWordItem ) ) != NULL &&
					// Justification doesn't exist yet in part-of specification
					!hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, secondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					assumptionLevel_ >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
					// Add opposite possessive assumption justification of my word to me
					addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add an opposite possessive assumption justification of my word to me" );

					// Current relation word
					if( !currentRelationWordItem->hasCurrentlyConfirmedSpecification() &&
					( secondarySpecificationItem = currentRelationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, oppositeSpecificationWordItem, _myWordItem ) ) != NULL &&
					// Justification doesn't exist yet in part-of specification
					!hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, secondarySpecificationItem ) &&
					assumptionLevel_ >= Item::justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
					// Add opposite possessive assumption justification of current relation word to me
					addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add an opposite possessive assumption justification of current relation word to me" );
					}
				}
			}

		return RESULT_OK;
		}


	// Private specification functions

	bool hasContextMasculineWordInRelationWords()
		{
		WordItem *currentCollectionWordItem;

		if( relationWordItem_ != NULL &&
		relationWordItem_->isMasculineWord() )
			return true;

		if( relationCollectionNr_ > NO_COLLECTION_NR &&
		( currentCollectionWordItem = myWordItem()->firstCollectionWordItem( relationCollectionNr_ ) ) != NULL )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem->isMasculineWord() )
					return true;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr_ ) ) != NULL );
			}

		return false;
		}

	bool hasSecondaryActiveAssignmentSpecification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isSecondaryAssignmentSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
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

	bool isRelationWordCoveredByJustifications( WordItem *givenRelationWordItem )
		{
		bool hasFoundRelationWordInJustification = false;
		unsigned int relationCollectionNr;
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;

		if( givenRelationWordItem != NULL )
			{
			while( !hasFoundRelationWordInJustification &&
			searchJustificationItem != NULL )
				{
				// Check primary specification
				if( ( ( primarySpecificationItem = searchJustificationItem->primarySpecificationItem() ) != NULL &&
				primarySpecificationItem->hasRelationWord() &&

					( primarySpecificationItem->generalizationWordItem() == givenRelationWordItem ||
					primarySpecificationItem->relationWordItem() == givenRelationWordItem ||

					( ( relationCollectionNr = primarySpecificationItem->relationCollectionNr() ) > NO_COLLECTION_NR &&
					givenRelationWordItem->hasCollectionNr( relationCollectionNr ) ) ) ) ||

				// Check secondary specification
				( ( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL &&
				secondarySpecificationItem->hasRelationWord() &&

					( secondarySpecificationItem->generalizationWordItem() == givenRelationWordItem ||
					secondarySpecificationItem->relationWordItem() == givenRelationWordItem ||

					( ( relationCollectionNr = secondarySpecificationItem->relationCollectionNr() ) > NO_COLLECTION_NR &&
					givenRelationWordItem->hasCollectionNr( relationCollectionNr ) ) ) ) )
					hasFoundRelationWordInJustification = true;

				searchJustificationItem = searchJustificationItem->attachedJustificationItem();
				}

			if( !hasFoundRelationWordInJustification )
				// At least one relation word is not covered by justifications
				return false;
			}

		// Relation words are covered by justifications
		return true;
		}

	WordItem *relationWordNotCoveredByJustifications()
		{
		WordItem *currentCollectionWordItem;

		if( relationWordItem_ != NULL &&
		firstJustificationItem_ != NULL )
			{
			if( !isRelationWordCoveredByJustifications( relationWordItem_ ) )
				// Relation word is not covered by justifications
				return relationWordItem_;

			if( relationCollectionNr_ > NO_COLLECTION_NR &&
			( currentCollectionWordItem = myWordItem()->firstCollectionWordItem( relationCollectionNr_ ) ) != NULL )
				{
				// Do for all collection words
				do	{
					if( !isRelationWordCoveredByJustifications( currentCollectionWordItem ) )
						// Relation word is not covered by justifications
						return currentCollectionWordItem;
					}
				while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr_ ) ) != NULL );
				}
			}

		// Relation words are covered by justifications
		return NULL;
		}

	protected:
	// Protected constructed variables

	bool hasSpecificationBeenWrittenAsAnswer = false;

	SpecificationItem *replacingSpecificationItem = NULL;


	// Constructor

	SpecificationItem( bool isAnsweredQuestion, bool isCharacteristicFor, bool isConditional, bool isCorrectedSpecification, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isLanguageWord, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
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
		relationCollectionNr_ = relationCollectionNr;

		generalizationContextNr_ = generalizationContextNr;
		specificationContextNr_ = specificationContextNr;
		relationContextNr_ = relationContextNr;

		nEnteredRelationWords_ = nEnteredRelationWords;

		firstJustificationItem_ = firstJustificationItem;

		specificationWordItem_ = specificationWordItem;
		relationWordItem_ = relationWordItem;

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
				relationCollectionNr_ == queryParameter ||
				generalizationContextNr_ == queryParameter ||
				specificationContextNr_ == queryParameter ||
				relationContextNr_ == queryParameter ||
				nEnteredRelationWords_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

					( prepositionParameter_ > NO_PREPOSITION_PARAMETER ||
					questionParameter_ > NO_QUESTION_PARAMETER ||
					generalizationCollectionNr_ > NO_COLLECTION_NR ||
					specificationCollectionNr_ > NO_COLLECTION_NR ||
					relationCollectionNr_ > NO_COLLECTION_NR ||
					generalizationContextNr_ > NO_CONTEXT_NR ||
					specificationContextNr_ > NO_CONTEXT_NR ||
					relationContextNr_ > NO_CONTEXT_NR ||
					nEnteredRelationWords_ > 0 ) ) );
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

		if( relationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( tempString, "%crelationCollectionNr:%u", QUERY_SEPARATOR_CHAR, relationCollectionNr_ );
			strcat( queryString, tempString );
			}

		if( relationWordTypeNr_ > NO_WORD_TYPE_NR )
			{
			if( relationWordTypeString == NULL )
				sprintf( tempString, "%crelationWordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, relationWordTypeNr_ );
			else
				sprintf( tempString, "%crelationWordType:%s%c%u", QUERY_SEPARATOR_CHAR, relationWordTypeString, QUERY_WORD_TYPE_CHAR, relationWordTypeNr_ );
			}

		strcat( queryString, tempString );

		if( relationContextNr_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%crelationContextNr:%u", QUERY_SEPARATOR_CHAR, relationContextNr_ );
			strcat( queryString, tempString );
			}

		if( relationWordItem_ != NULL )
			{
			sprintf( tempString, "%crelationWord%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, relationWordItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, relationWordItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );

			if( ( wordString = relationWordItem_->wordTypeString( true, relationWordTypeNr_ ) ) != NULL )
				{
				sprintf( tempString, "%c%s%c", QUERY_WORD_REFERENCE_START_CHAR, wordString, QUERY_WORD_REFERENCE_END_CHAR );
				strcat( queryString, tempString );
				}
			}

		if( nEnteredRelationWords_ > NO_CONTEXT_NR )
			{
			sprintf( tempString, "%cnEnteredRelationWords:%u", QUERY_SEPARATOR_CHAR, nEnteredRelationWords_ );
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

	unsigned int nRelationWords()
		{
		return myWordItem()->nRelationWords( relationCollectionNr_, relationWordItem_ );
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
			if( searchJustificationItem->hasAdditionalDefinitionSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasOnlyOneJustificationType( unsigned short justificationTypeNr )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() != justificationTypeNr )
				return false;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return true;
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
			if( searchJustificationItem->hasSecondaryCompoundCollectionSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasSecondaryUserSpecificationJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isOppositePossessiveSpecificationAssumption() &&
			searchJustificationItem->hasSecondaryUserSpecification() )
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

	unsigned int nJustificationItems()
		{
		unsigned int nItems = 0;
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			nItems++;
			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return nItems;
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

	signed char checkForJustificationsCoveringRelationWords()
		{
		WordItem *notCoveredRelationWord;
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForJustificationsCoveringRelationWords";

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I am a user specification. So, I have no justifications" );

		if( globalVariables()->firstCollectionWordItem == NULL )
			return startError( functionNameString, NULL, "I could not find any collection word, while checking myself for justifications covering relation words" );

		if( ( notCoveredRelationWord = relationWordNotCoveredByJustifications() ) != NULL )
			{
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcpy( errorString, "\nI found a specification missing justifications covering relation word: \"" );
			strcat( errorString, notCoveredRelationWord->anyWordTypeString() );
			strcat( errorString, "\";\n\tRelation collection number: " );
			sprintf( tempString, "%d", relationCollectionNr_ );
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

	signed char removeRemainingObsoleteAssumptionJustifications()
		{
		JustificationItem *previousJustificationItem = NULL;
		JustificationItem *searchJustificationItem = firstJustificationItem_;
		WordItem *_myWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeRemainingObsoleteAssumptionJustifications";

		if( questionParameter_ == NO_QUESTION_PARAMETER &&
		assumptionLevel_ < ASSUMPTION_LEVEL_MAYBE )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isOlderItem() &&
				assumptionLevel_ < searchJustificationItem->justificationAssumptionLevel() )
					{
					if( previousJustificationItem == NULL )
						{
						if( hasCurrentCreationSentenceNr() )
							{
							firstJustificationItem_ = searchJustificationItem->attachedJustificationItem();

							// Change attached justification
							if( searchJustificationItem->changeAttachedJustification( NULL ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to disconnect the attached justification item of the search justification item" );

							if( _myWordItem->replaceOrDeleteJustification( searchJustificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to replace an obsolete justification" );

							searchJustificationItem = firstJustificationItem_;
							}
						else
							{
							previousJustificationItem = searchJustificationItem;
							searchJustificationItem = searchJustificationItem->attachedJustificationItem();
							}
						}
					else	// Specification is not current
						{
						if( previousJustificationItem->hasCurrentCreationSentenceNr() )
							{
							// Change attached justification
							if( previousJustificationItem->changeAttachedJustification( searchJustificationItem->attachedJustificationItem() ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to disconnect the attached justification item of the search justification item" );

							if( searchJustificationItem->hasCurrentCreationSentenceNr() &&
							// Change attached justification
							searchJustificationItem->changeAttachedJustification( NULL ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to disconnect the attached justification item of the search justification item" );

							if( _myWordItem->replaceOrDeleteJustification( searchJustificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to replace an obsolete justification" );

							previousJustificationItem = NULL;
							searchJustificationItem = firstJustificationItem_;
							}
						else	// Previous justification is not current
							{
							previousJustificationItem = searchJustificationItem;
							searchJustificationItem = searchJustificationItem->attachedJustificationItem();
							}
						}
					}
				else
					{
					previousJustificationItem = searchJustificationItem;
					searchJustificationItem = searchJustificationItem->attachedJustificationItem();
					}
				}
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
		return ( firstJustificationItem_ == NULL &&
				questionParameter_ > NO_QUESTION_PARAMETER );
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
		hasSpecificationBeenWrittenAsAnswer = false;
		}

	void storeWrittenSentenceString()
		{
		strcpy( storedSentenceString_, globalVariables()->writtenSentenceString );
		}

	void storeWrittenSentenceStringWithOneSpecificationOnly()
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
		return ( relationCollectionNr_ == NO_COLLECTION_NR && 
				relationWordItem_ != NULL );
		}

	bool hasPrimaryActiveAssignmentJustification()
		{
		JustificationItem *searchJustificationItem = firstJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isPrimaryActiveAssignmentSpecification() )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem();
			}

		return false;
		}

	bool hasRelationWord()
		{
		return ( relationWordItem_ != NULL );
		}

	bool hasRelationCollection()
		{
		return ( relationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasRelationCollectionCurrentlyBeenUpdated()
		{
		WordItem *currentCollectionWordItem;

		if( relationCollectionNr_ > NO_COLLECTION_NR &&
		( currentCollectionWordItem = myWordItem()->firstCollectionWordItem( relationCollectionNr_ ) ) != NULL )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem->hasCollectionCurrentlyBeenUpdated( relationCollectionNr_ ) )
					return true;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr_ ) ) != NULL );
			}

		return false;
		}

	bool hasSpecificationCollection()
		{
		return ( specificationCollectionNr_ > NO_COLLECTION_NR );
		}

	bool hasSpecificationContext()
		{
		return ( specificationContextNr_ > NO_CONTEXT_NR );
		}

	bool hasUpdatedInformation()
		{
		return ( !isOlderItem() ||

				( relationCollectionNr_ > NO_COLLECTION_NR &&
				hasRelationCollectionCurrentlyBeenUpdated() ) );
		}

	bool hasUpdatedWriteInformation()
		{
		JustificationItem *attachedJustificationItem;

		return ( !isOlderItem() ||

				( relationWordItem_ != NULL &&

					// Has been inactivated during this sentence
					// Test file: "ambiguity\US presidents"
					( hasCurrentInactiveSentenceNr() ||

					// Has been archived during this sentence
					// Test file: "ambiguity\US presidents"
					hasCurrentArchivedSentenceNr() ||

					( relationCollectionNr_ > NO_COLLECTION_NR &&
					hasCurrentCreationSentenceNr() &&

						// Test files: "reasoning\family\Complex (12)",
						//				"reasoning\family\Complex (14)",
						//				"reasoning\family\Complex (16)",
						//				"reasoning\family\Complex (20)",
						//				"reasoning\family\I know (11)",
						//				among others
						( ( strlen( storedSentenceString_ ) == 0 &&
						!hasOnlyOneJustificationType( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) ||

						( hasRelationCollectionCurrentlyBeenUpdated() &&
						firstJustificationItem_ != NULL &&

							( ( !isSpecificationWordSpanishAmbiguous() &&

								// Test files: "question answering\family\Indirectly answered questions (with confirmation)",
								//				"reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
								//				"reasoning\family\Complex (11)",
								//				"reasoning\family\Complex (18)",
								//				"reasoning\family\Correcting invalidated assumption (by knowledge)",
								//				"reasoning\family\I know (3)",
								//				"reasoning\family\I know (7)",
								//				among others
								( isPossessive_ ||

								// Test files: "question answering\family\Indirectly answered questions (with confirmation)",
								//				"reasoning\family\Anna - John - Paul, Joe and Laura",
								//				"reasoning\family\Anna was the mother of Paul, Joe and Laura",
								//				"reasoning\family\Anna was the mother of Paul, Joe and Laura",
								//				"reasoning\family\Complex (4)",
								//				"reasoning\family\Complex (6)",
								//				"reasoning\family\Complex (15)",
								//				among others
								firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == NULL ||

								( !relationWordItem_->isUserGeneralizationWord &&

									// Test files: "reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
									//				"reasoning\family\Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura",
									//				"reasoning\family\Complex (3)",
									//				"reasoning\family\Complex (4)",
									//				"reasoning\family\Complex (5)",
									//				"reasoning\family\Complex (6)",
									//				among others
									( ( !firstJustificationItem_->isOlderItem() &&
									!firstJustificationItem_->isOppositePossessiveSpecificationAssumption() ) ||

									// Test files: "reasoning\family\Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura",
									//				"reasoning\family\Complex (18)",
									//				"reasoning\family\Complex (19 - mixed)",
									//				"reasoning\family\Complex (19 - strange)"
									//				"reasoning\family\Complex (21)",
									//				"reasoning\family\I know (11)",
									//				among others
									( ( attachedJustificationItem = firstJustificationItem_->attachedJustificationItem() ) != NULL &&
									attachedJustificationItem->isReversibleAssumptionOrConclusion() ) ) ) ) ) ||

							// Typical for Spanish
							( isSpecificationWordSpanishAmbiguous() &&

								// Spanish test files: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)",
								//						"razonamiento\familia\Complejo (19 - extraño)"
								( !isPossessive_ ||

								( !firstJustificationItem_->isOlderItem() &&

									// Spanish test files: "razonamiento\familia\Complejo (3)",
									//						"razonamiento\familia\Complejo (6)",
									//						"razonamiento\familia\Complejo (7 - Becky)",
									//						"razonamiento\familia\Complejo (7 - Claudia)",
									//						among others
									( !firstJustificationItem_->isReversibleAssumptionOrConclusion() ||

									// Spanish test files: "razonamiento\familia\Complejo (9)",
									//						"razonamiento\familia\Complejo (13)",
									//						"razonamiento\familia\Complejo (15)",
									//						"razonamiento\familia\Sé (3)",
									//						among others
									( !hasSecondaryActiveAssignmentSpecification() &&
									!firstJustificationItem_->isPrimaryActiveAssignmentSpecification() ) ) ) ) ) ) ) ) ) ) ) );
		}

	bool isAdjectiveSpecification()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_ADJECTIVE );
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
					if( ( currentSpecificationWordItem = ( searchSpecificationItem == this ? specificationWordItem_ :
														searchSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification_, isNegative_, isPossessive_, isSelfGenerated, assumptionLevel_, specificationWordTypeNr_, specificationCollectionNr_, relationCollectionNr_, generalizationContextNr_, relationWordItem_ ) ) ) != NULL &&
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

	bool isHiddenSpanishSpecification()
		{
		WordItem *_myWordItem;

		return ( isSpecificationWordSpanishAmbiguous() &&
				generalizationWordTypeNr_ == WORD_TYPE_PROPER_NOUN &&
				!isNegative_ &&
				// Self-generated specification
				firstJustificationItem_ != NULL &&
				hasNonCompoundSpecificationCollection() &&

					// Non-possessive specification not in a male word
					( ( !isPossessive_ &&
					( _myWordItem = myWordItem() ) != NULL &&
					!_myWordItem->isMasculineWord() &&
					!_myWordItem->hasMasculineProperNounEnding() ) ||

					// Possessive specification without male relation context
					( isPossessive_ &&
					relationWordItem_ != NULL &&
					specificationWordItem_ != NULL &&
					specificationWordItem_->compoundCollectionNr() > NO_COLLECTION_NR &&
					!hasContextMasculineWordInRelationWords() ) ) );
		}

	bool isMatchingRelation( bool isAllowingEmptyRelation, unsigned int relationCollectionNr, WordItem *relationWordItem )
		{		// Empty subset
		return ( relationWordItem == NULL ||

				// Same set
				( relationCollectionNr_ == relationCollectionNr &&
				relationWordItem_ == relationWordItem ) ||

				// No relation word
				( relationWordItem_ == NULL &&
				isAllowingEmptyRelation ) ||

				// Relation word
				( relationWordItem_ != NULL &&

					( ( relationCollectionNr > NO_COLLECTION_NR &&
					relationWordItem->hasCollectionNr( relationCollectionNr ) &&
					relationWordItem_->hasCollectionNr( relationCollectionNr ) ) ||

					( relationCollectionNr_ > NO_COLLECTION_NR &&
					relationWordItem->hasCollectionNr( relationCollectionNr_ ) &&
					relationWordItem_->hasCollectionNr( relationCollectionNr_ ) ) ) ) );
		}

	bool isNegative()
		{
		return isNegative_;
		}

	bool isNounGeneralization()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR ||
				generalizationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isNounSpecification()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR ||
				specificationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isNumeralSpecification()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isPartOf()
		{
		return isPartOf_;
		}

	bool isPossessive()
		{
		return isPossessive_;
		}

	bool isPluralNounSpecification()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isProperNounGeneralization()
		{
		return ( generalizationWordTypeNr_ == WORD_TYPE_PROPER_NOUN );
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
			if( assumptionLevel_ == NO_ASSUMPTION_LEVEL &&

			//Avoid idle calls
				( relationWordItem_ != NULL ||
				!isOlderItem() ) &&

			hasAtLeastOneAssumptionJustification() )
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

	bool isSingularNounSpecification()
		{
		return ( specificationWordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}

	bool isSpecific()
		{
		return isSpecific_;
		}

	bool isSpecificationGeneralization()
		{
		return isSpecificationGeneralization_;
		}

	bool isSpecificationStringAlreadyWritten()
		{
		return ( specificationStringWriteLevel_ > NO_WRITE_LEVEL );
		}

	bool isSpecificationWithStaticAmiguity()
		{
		return ( isExclusiveSpecification_ &&
				nRelationWords() > 1 );
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
				// Self-generated specification
		return ( firstJustificationItem_ != NULL &&
				relationWordItem_ != NULL &&
				// Not written yet (as unhidden Spanish specification)
				strlen( storedSentenceString_ ) == 0 &&
				isSpecificationWordSpanishAmbiguous() &&
				isOlderItem() &&
				!isHiddenSpanishSpecification() &&

					// Spanish test files: "conflicto\familia\José es una madre",
					//						"razonamiento\familia\Complejo (9)",
					//						"razonamiento\familia\Complejo (13)",
					//						"razonamiento\familia\Complejo (14)",
					//						"razonamiento\familia\Complejo (16)",
					//						"razonamiento\familia\Complejo (18)",
					//						among others
					( assumptionLevel_ > NO_ASSUMPTION_LEVEL ||
					// Spanish test file: "razonamiento\familia\Mis conclusiones que se confirman (José tiene 2 padres)"
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

	unsigned int relationCollectionNr()
		{
		return relationCollectionNr_;
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

	unsigned int nEnteredRelationWords()
		{
		return nEnteredRelationWords_;
		}

	unsigned int nInvolvedSpecificationWords()
		{
		unsigned int myCreationSentenceNr = creationSentenceNr();
		unsigned int nInvolvedSpecificationWords = 0;
		SpecificationItem *searchSpecificationItem = myWordItem()->firstSpecificationItem( false, isAssignment(), isInactiveAssignment(), isArchivedAssignment(), questionParameter_ );

		while( searchSpecificationItem != NULL )
			{
			if( searchSpecificationItem->creationSentenceNr() == myCreationSentenceNr )
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
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addUserSpecificationToSpanishJustifications";

		if( userSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given user specification item is undefined" );

		if( specificationWordItem_ == NULL )
			return startError( functionNameString, NULL, "I have no specification word item" );

		do	{
			if( ( secondaryGeneralizationWordItem = currentJustificationItem->secondaryGeneralizationWordItem() ) != NULL &&
			( selfGeneratedSpecificationItem = secondaryGeneralizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, !isPossessive_, true, specificationWordItem_, NULL ) ) != NULL &&
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
		bool _hasRelationWord = ( relationWordItem_ != NULL );
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

		if( ( attachedPrimarySpecificationItem = attachJustificationItem->primarySpecificationItem() ) != NULL )
			{
			if( attachedPrimarySpecificationItem->hasRelationWord() &&
			!attachedPrimarySpecificationItem->isPossessive() )
				obsoleteJustificationItem = firstJustificationItem_->primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem->specificationWordItem() );
			else
				{
				// Typical for Spanish
				if( isSpecificationWordSpanishAmbiguous() &&
				_hasRelationWord &&
				( attachedSecondarySpecificationItem = attachJustificationItem->secondarySpecificationItem() ) != NULL &&
				attachedSecondarySpecificationItem->hasCompoundSpecificationCollection() &&
				!attachedSecondarySpecificationItem->isPossessive() &&
				attachedSecondarySpecificationItem->isUserSpecification() )
					obsoleteJustificationItem = firstJustificationItem_->obsoleteSpanishJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
				}
			}

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

				// Test files: "reasoning\family\Complex (3)",
				//				"reasoning\family\Laura - Joe - John and Anna - Paul",
				//				"reasoning\family\Laura - Joe - Paul - John and Anna"
				if( !_hasRelationWord &&
				specificationCollectionNr_ == NO_COLLECTION_NR &&
				!isOlderItem() &&
				attachJustificationItem->justificationAssumptionLevel() < assumptionLevel_ &&
				attachJustificationItem->isSpecificationSubstitutionAssumptionOrConclusion() &&
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
					if( ( createAndAssignResult = _myWordItem->createSpecificationItem( isAssignment(), isInactiveAssignment(), isArchivedAssignment(), isAnsweredQuestion_, isCharacteristicFor_, isConditional(), isCorrectedSpecification_, isEveryGeneralization_, isExclusiveGeneralization_, isExclusiveSpecification_, isNegative_, isPartOf_, isPossessive_, isSpecific_, isSpecificationGeneralization_, isUncountableGeneralizationNoun_, isUniqueUserRelation_, isValueSpecification_, assignmentLevel_, assumptionLevel_, languageNr(), prepositionParameter(), questionParameter(), generalizationWordTypeNr(), specificationWordTypeNr(), relationWordTypeNr(), specificationCollectionNr_, relationCollectionNr_, generalizationContextNr_, specificationContextNr_, relationContextNr_, originalSentenceNr(), activeSentenceNr(), inactiveSentenceNr(), archivedSentenceNr(), nEnteredRelationWords_, attachJustificationItem, specificationWordItem_, relationWordItem_, specificationString_, storedSentenceString_, storedSentenceWithOnlyOneSpecificationString_ ) ).result != RESULT_OK )
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
		else
			{
			if( _hasRelationWord &&
			// Replace obsolete justification item
			_myWordItem->replaceJustification( obsoleteJustificationItem, attachJustificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to replace the obsolete justification item" );
			}

		return RESULT_OK;
		}

	signed char changeSpecification( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, unsigned int newRelationCollectionNr, JustificationItem *newFirstJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changeSpecification";

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		isAnsweredQuestion_ = isNewAnsweredQuestion;
		isExclusiveGeneralization_ = isNewExclusiveGeneralization;
		generalizationCollectionNr_ = newGeneralizationCollectionNr;
		specificationCollectionNr_ = newSpecificationCollectionNr;
		relationCollectionNr_ = newRelationCollectionNr;
		firstJustificationItem_ = newFirstJustificationItem;

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
			if( myWordItem()->changeSpecification( true, isExclusiveGeneralization_, generalizationCollectionNr_, specificationCollectionNr_, relationCollectionNr_, firstJustificationItem_, this ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to change myself into an answered question" );
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
		bool _hasRelationWord = ( relationWordItem_ != NULL );
		bool isOlderSpecification = isOlderItem();
		unsigned short previousAssumptionLevel = assumptionLevel_;
		JustificationItem *attachedJustificationItem;
		SpecificationItem *partOfSpecificationItem;
		WordItem *currentCollectionWordItem;
		WordItem *_myWordItem = myWordItem();
		WordItem *partOfSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptionLevel";

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I am a user specification" );

		if( specificationWordItem_ == NULL )
			return startError( functionNameString, NULL, "I have no specification word" );

		if( isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "I am a replaced or deleted specification" );

		// Calculate assumption level
		if( calculateAssumptionLevel() != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to calculate my assumption level" );

		if( hasCurrentCreationSentenceNr() )
			{
			hasChangedAssumptionLevel = ( previousAssumptionLevel != assumptionLevel_ );

			if( ( ( isOlderSpecification &&
			!isNegative_ ) ||

			( isSituationStable &&

				// No relation word
				// Test file: "reasoning\family\John - Anna (before family definition)"
				( ( !_hasRelationWord &&
				!hasChangedAssumptionLevel &&
				isPartOf_ &&
				highestJustificationAssumptionLevel() > assumptionLevel_ ) ||

				// Relation word
				( _hasRelationWord &&
				hasNonCompoundSpecificationCollection() &&

					// Non-possessive
					// Test files: "reasoning\family\Complex (3)",
					//				"reasoning\family\Complex (12)",
					//				"reasoning\family\John - Anna (before family definition)"
					( ( !isPossessive_ &&
					_myWordItem->isUserRelationWord ) ||

					// Possessive
					// Test files: "reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (19 - strange)",
					//				"reasoning\family\John - Laura - John and Anna"
					( !hasChangedAssumptionLevel &&
					isPossessive_ ) ) ) ) ) ) &&

			// Remove obsolete assumption justifications
			removeObsoleteAssumptionJustifications( isSituationStable ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to remove obsolete assumption justifications from this specification" );

			if( hasChangedAssumptionLevel &&
			isOlderSpecification &&

				// Only one relation word
				( ( relationCollectionNr_ == NO_COLLECTION_NR &&

					// Adjusted specifications
					( ( ( assumptionLevel_ < NUMBER_OF_ASSUMPTION_LEVELS ||

					// Test files: "reasoning\family\Complex (13)",
					//				"reasoning\family\Complex (18)",
					//				"reasoning\family\Complex (19 - mixed)",
					//				"reasoning\family\Complex (19 - strange)"
					previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ) &&

					// Skip writing update of specification part of assignment
					_myWordItem->firstAssignmentItem( isPossessive_, false, specificationWordItem_ ) == NULL ) ||

					// Past-tense specifications
					// Test files: "conflict\family\John was the father of Paul (before family definition)",
					//				"question answering\family\Just a few questions (1)",
					//				"reasoning\family\James was the father of Peter (before family definition)"
					isAssignment() ) ) ||

				// Relation word collection
				( relationCollectionNr_ > NO_COLLECTION_NR &&
				assumptionLevel_ < previousAssumptionLevel &&
				assumptionLevel_ < NUMBER_OF_ASSUMPTION_LEVELS &&

					( ( !hasRelationCollectionCurrentlyBeenUpdated() &&

						// Test files: "question answering\family\Intelligent answering of questions",
						//				"reasoning\family\Avoid duplicate context",
						//				"reasoning\family\Complex (1)",
						//				"reasoning\family\Complex (3)",
						//				among others
						( !firstJustificationItem_->isPrimaryActiveAssignmentSpecification() ||

						// Test file: "reasoning\family\I know (3)"
						( ( attachedJustificationItem = firstJustificationItem_->attachedJustificationItem() ) != NULL &&
						attachedJustificationItem->isOppositePossessiveSpecificationAssumption() ) ) ) ||

					// Typical for Spanish: previously hidden Spanish ambiguous specifications
					// Spanish test files: "conflicto\familia\José es una madre",
					//						"razonamiento\familia\Complejo (6)",
					//						"razonamiento\familia\Complejo (9)",
					//						"razonamiento\familia\Complejo (11)",
					//						"razonamiento\familia\Complejo (13)",
					//						among others
					assumptionLevel_ == NO_ASSUMPTION_LEVEL ) ) ) &&

				// Write adjusted specification
				_myWordItem->writeUpdatedSpecification( true, false, false, false, false, false, false, this ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to write an adjusted specification" );

			if( relationWordItem_ != NULL &&

				// Changed assumption level
				( hasChangedAssumptionLevel ||
				_myWordItem->isUserGeneralizationWord ) &&

			( partOfSpecificationWordItem = specificationWordItem_->commonWordItem( specificationCollectionNr_ ) ) != NULL )
				{
				// Only one relation word
				if( relationCollectionNr_ == NO_COLLECTION_NR )
					{
					// Test files: "question answering\family\Indirectly answered questions (with confirmation)",
					//				"question answering\family\Negative and alternative answers",
					//				"reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
					//				"reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				among others
					if( isPossessive_ &&
					// Add justifications to specifications of my relation word
					addJustificationsToSpecificationsOfRelationWords( relationWordItem_, partOfSpecificationWordItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to add justifications to the specifications of my relation word" );
					}
				else	// Relation collection
					{
					if( ( currentCollectionWordItem = _myWordItem->firstCollectionWordItem( relationCollectionNr_ ) ) != NULL )
						{
						// Do for all collection words
						do	{
							// Add justifications to specifications of current relation word
							if( addJustificationsToSpecificationsOfRelationWords( currentCollectionWordItem, partOfSpecificationWordItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to add justifications to the specifications of current relation word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Luisa - Juan y Ana - José"
							if( _myWordItem->hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore &&
							!currentCollectionWordItem->isUserGeneralizationWord &&
							!specificationWordItem_->isUserSpecificationWord &&
							( partOfSpecificationItem = currentCollectionWordItem->firstSpecificationItem( false, false, false, partOfSpecificationWordItem ) ) != NULL &&
							// Recalculate assumption level of part-of specification in current collection word
							partOfSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to recalculate assumption level of part-of specification in current collection word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );
							}
						while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr_ ) ) != NULL );
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char removeObsoleteAssumptionJustifications( bool isSituationStable )
		{
		bool hasConfirmedAnySpecification = globalVariables()->hasConfirmedAnySpecification;
		bool isIncludingExclusiveSpecificationSubstitutionAssumption;
		bool isIncludingOppositePossessiveSpecificationAssumption;
		bool isIncludingReversibleAssumptionOrConclusion;
		bool isIncludingSpecificationSubstitutionAssumptionOrConclusion;
		bool isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion;
		bool isOlderSpecification = isOlderItem();
		JustificationItem *attachedJustificationItem;
		JustificationItem *copiedJustificationItem;
		JustificationItem *currentJustificationItem = firstJustificationItem_;
		JustificationItem *mostRecentCurrentJustificationItem = NULL;
		JustificationItem *oldJustificationItem;
		JustificationItem *previousJustificationItem = NULL;
		JustificationItem *previousPreviousJustificationItem = NULL;
		WordItem *_myWordItem = myWordItem();
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "removeObsoleteAssumptionJustifications";

		if( questionParameter_ > NO_QUESTION_PARAMETER )
			return startError( functionNameString, NULL, "I am a question" );

		if( firstJustificationItem_ == NULL )
			return startError( functionNameString, NULL, "I am a user specification" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

																// Test files: "reasoning\family\Complex (18)",
																//				"reasoning\family\Complex (19 - mixed)",
																//				"reasoning\family\Complex (19 - strange)"
		isIncludingExclusiveSpecificationSubstitutionAssumption = ( isSituationStable ||

																	// Spanish test files: "razonamiento\familia\Mis suposiciones que se confirman (Juan y Ana)",
																	//						"razonamiento\familia\Que haya confirmado al menos una palabra de relación"
																	relationCollectionNr_ == NO_COLLECTION_NR );

																// Test files: "reasoning\family\Complex (1)",
																//				"reasoning\family\John - Laura - John and Anna",
																//				"reasoning\family\My assumptions that are confirmed (Paul, Joe and Laura)"
		isIncludingOppositePossessiveSpecificationAssumption = ( ( isSituationStable &&
																relationCollectionNr_ > NO_COLLECTION_NR ) ||

																// Typical for Spanish
																// Spanish test files: "razonamiento\familia\Complejo (1)",
																//						"razonamiento\familia\Complejo (14)",
																//						"razonamiento\familia\Mis suposiciones que se confirman (hijos)",
																//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa"
																isSpecificationWordSpanishAmbiguous() );

		isIncludingReversibleAssumptionOrConclusion = ( isSituationStable &&
														// Test file: "reasoning\family\Complex (20)"
														!firstJustificationItem_->isOlderItem() );

																		// Test files: "reasoning\family\Complex (3)",
																		//				"reasoning\family\Correcting invalidated assumption (by knowledge)",
																		//				"reasoning\family\Laura - Joe - John and Anna - Paul",
																		//				"reasoning\family\Laura - Joe - Paul - John and Anna"
		isIncludingSpecificationSubstitutionAssumptionOrConclusion = ( !hasConfirmedAnySpecification ||

																		// Test files: "reasoning\family\Complex (20)"
																		//				"reasoning\family\My assumptions that are confirmed (Paul, Joe and Laura)"
																		( isSituationStable &&
																		relationCollectionNr_ > NO_COLLECTION_NR ) );

		isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion = ( !hasConfirmedAnySpecification &&
																			!firstJustificationItem_->isOlderItem() );

		if( isIncludingExclusiveSpecificationSubstitutionAssumption ||
		isIncludingOppositePossessiveSpecificationAssumption ||
//		isIncludingReversibleAssumptionOrConclusion ||
		isIncludingSpecificationSubstitutionAssumptionOrConclusion /*||
		isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion*/ )
			{
			do	{
				if( currentJustificationItem->isObsoleteAssumptionJustification( isIncludingOppositePossessiveSpecificationAssumption, isIncludingReversibleAssumptionOrConclusion, isIncludingSpecificationSubstitutionAssumptionOrConclusion, isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion, isOlderSpecification, assumptionLevel_ ) )
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

							if( ( copiedJustificationItem = justificationResult.createdJustificationItem ) == NULL )
								return startError( functionNameString, NULL, "The copied justification item is undefined" );

							if( previousPreviousJustificationItem == NULL )
								{
								// Change first justification item to created justification
								if( changeFirstJustification( true, copiedJustificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to change my first justification item to the created justification" );
								}
							else
								{
								// Change attached justification item of previous previous justification to created justification
								if( previousPreviousJustificationItem->changeAttachedJustification( copiedJustificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to change the attached justification item of the previous previous justification item to the created justification" );
								}

							// Replace previous justification
							if( _myWordItem->replaceOrDeleteJustification( previousJustificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to replace the previous justification item" );

							// Replace current justification
							if( _myWordItem->replaceOrDeleteJustification( currentJustificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to replace the current justification item" );

							// Reset variables
							mostRecentCurrentJustificationItem = NULL;
							previousPreviousJustificationItem = NULL;
							previousJustificationItem = NULL;
							currentJustificationItem = firstJustificationItem_;
							}
						else
							{
							while( mostRecentCurrentJustificationItem != NULL &&
							( oldJustificationItem = mostRecentCurrentJustificationItem->attachedJustificationItem() ) != NULL &&
							oldJustificationItem != currentJustificationItem )
								{
								// Copy old justification
								if( ( justificationResult = _myWordItem->copyJustificationItem( oldJustificationItem->primarySpecificationItem(), oldJustificationItem->secondarySpecificationItem(), oldJustificationItem->attachedJustificationItem(), oldJustificationItem ) ).result != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to copy the previous justification item" );

								if( ( copiedJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									return startError( functionNameString, NULL, "The copied justification item is undefined" );

								// Connect copied justification to most recent current justification
								if( mostRecentCurrentJustificationItem->changeAttachedJustification( copiedJustificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to disconnect the attached justification item of the search justification item" );

								// Remove obsolete justification
								if( _myWordItem->replaceOrDeleteJustification( oldJustificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, "I failed to replace an obsolete justification" );

								// Copy and replace next old justification
								mostRecentCurrentJustificationItem = copiedJustificationItem;
								}

							// Reset variables
							mostRecentCurrentJustificationItem = NULL;
							previousPreviousJustificationItem = NULL;
							previousJustificationItem = NULL;
							currentJustificationItem = firstJustificationItem_;
							}
						}
					}
				else
					{
					previousPreviousJustificationItem = previousJustificationItem;
					previousJustificationItem = currentJustificationItem;

					if( currentJustificationItem->hasCurrentCreationSentenceNr() )
						mostRecentCurrentJustificationItem = currentJustificationItem;

					currentJustificationItem = currentJustificationItem->attachedJustificationItem();
					}
				}
			while( currentJustificationItem != NULL );
			}

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
				!searchSpecificationItem->isAdjectiveSpecification() ) )
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

	SpecificationItem *nonNegativeNonPosessiveDefinitionSpecificationItem( bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( !searchSpecificationItem->isNegative() &&
			!searchSpecificationItem->isSpecificationGeneralization() &&
			// Typical for Chinese
			// Chinese test files: "Boiling point",
			//						"Condensation point"
			!searchSpecificationItem->isPossessive() )
				return searchSpecificationItem;

			searchSpecificationItem = searchSpecificationItem->nextSpecificationItem();
			}

		return NULL;
		}

	SpecificationItem *nonQuestionSpecificationItem( bool isAllowingEmptyRelation, bool isIncludingThisItem )
		{
		SpecificationItem *searchSpecificationItem = ( isIncludingThisItem ? this : nextSpecificationItem() );

		while( searchSpecificationItem != NULL )
			{
			if( ( isAllowingEmptyRelation ||
			searchSpecificationItem->hasRelationWord() ) &&

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

	SpecificationItem *nextNonNegativeNonPosessiveDefinitionSpecificationItem()
		{
		return nonNegativeNonPosessiveDefinitionSpecificationItem( false );
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

	WordItem *firstUnwrittenRelationWordItem( unsigned short wordTypeNr, WordItem *previousRelationWordItem )
		{
		WordItem *currentCollectionWordItem;

		if( relationWordItem_ != NULL )
			{
			if( relationCollectionNr_ > NO_COLLECTION_NR &&
			( currentCollectionWordItem = ( previousRelationWordItem == NULL ?
												myWordItem()->firstCollectionWordItem( relationCollectionNr_ ) :
												previousRelationWordItem ) ) != NULL )
				{
				// Do for all collection words
				do	{
					if( !currentCollectionWordItem->isRelationWordTypeAlreadyWritten( wordTypeNr ) )
						return currentCollectionWordItem;
					}
				while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr_ ) ) != NULL );
				}

			if( relationWordItem_ != previousRelationWordItem &&
			!relationWordItem_->isRelationWordTypeAlreadyWritten( wordTypeNr ) )
				return relationWordItem_;
			}

		return NULL;
		}

	WordItem *generalizationWordItem()
		{
		return myWordItem();
		}

	WordItem *mostRecentRelationWord()
		{
		WordItem *currentCollectionWordItem;
		WordItem *previousCollectionWordItem;

		if( relationCollectionNr_ > NO_COLLECTION_NR &&
		( currentCollectionWordItem = myWordItem()->firstCollectionWordItem( relationCollectionNr_ ) ) != NULL )
			{
			// Do for all collection words
			do	previousCollectionWordItem = currentCollectionWordItem;
			while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( relationCollectionNr_ ) ) != NULL );

			return previousCollectionWordItem;
			}

		return relationWordItem_;
		}

	WordItem *relationCollectionWordItem()
		{
		if( relationCollectionNr_ > NO_COLLECTION_NR &&
		relationWordItem_ != NULL )
			return relationWordItem_->collectionWordItem( relationCollectionNr_ );

		// If relation collection doesn't exist, return relation word
		return relationWordItem_;
		}

	WordItem *relationWordItem()
		{
		return relationWordItem_;
		}

	WordItem *relatedSpecificationWordItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool _isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, WordItem *relationWordItem )
		{
		return ( isExclusiveSpecification_ == isExclusiveSpecification &&
				isNegative_ == isNegative &&
				isPossessive_ == isPossessive &&
				isSelfGeneratedSpecification() == _isSelfGeneratedSpecification &&
				specificationCollectionNr > NO_COLLECTION_NR &&
				specificationCollectionNr_ == specificationCollectionNr &&
				relationCollectionNr_ == relationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				relationWordItem_ == relationWordItem &&

					( ( assumptionLevel_ == assumptionLevel &&

						( specificationWordTypeNr == NO_WORD_TYPE_NR ||
						specificationWordTypeNr_ == specificationWordTypeNr ) &&

						// Typical for Spanish
						// Spanish test file: "Reto científico"
						!isHiddenSpanishSpecification() ) ||

					// Test files: "conflict\family\Question in conflict with itself (with knowledge)",
					//				"reasoning\Scientific challenge",
					//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)",
					//				"reasoning\family\John - Anna (before family definition)",
					//				"reasoning\family\This information is more specific (non-exclusive)"
					questionParameter_ > NO_QUESTION_PARAMETER ) ? specificationWordItem_ : NULL );
		}

	WordItem *relatedSpecificationWordItem( bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, WordItem *relationWordItem )
		{
		return ( isPossessive_ == isPossessive &&

					( isIgnoringExclusive ||
					isExclusiveSpecification_ == isExclusiveSpecification ) &&

					( isIgnoringNegative ||
					isNegative_ == isNegative ) &&

				specificationCollectionNr_ == specificationCollectionNr &&
				generalizationContextNr_ == generalizationContextNr &&
				isMatchingWordType( specificationWordTypeNr_, specificationWordTypeNr ) &&

					( !isCheckingRelationContext ||

					( relationCollectionNr_ == relationCollectionNr &&
					relationWordItem_ == relationWordItem ) ) ? specificationWordItem_ : NULL );
		}

	WordItem *singleRelationWordItem()
		{
		return ( relationCollectionNr_ == NO_COLLECTION_NR ? relationWordItem_ : NULL );
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
