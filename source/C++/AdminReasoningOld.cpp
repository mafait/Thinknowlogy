/*	Class:			AdminReasoningOld
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (old design)
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#include "AdminItem.h"
#include "FindSpecificationResultType.cpp"
#include "GeneralizationItem.cpp"

class AdminReasoningOld
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasSuggestiveQuestionAssumption_;

	unsigned int postponeNegativeConclusionSpecificationCollectionNr_;

	SpecificationItem *alternativeAnotherPrimarySpecificationItem_;
	SpecificationItem *possessiveDefinitionSpecificationItem_;
	SpecificationItem *oppositePossessiveDefinitionSpecificationItem_;
	SpecificationItem *postponeNegativeConclusionPrimarySpecificationItem_;
	SpecificationItem *postponeNegativeConclusionAnotherPrimarySpecificationItem_;
	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem_;
	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
	SpecificationItem *spanishQuestionPrimarySpecificationItem_;

	WordItem *feminineSpecificationWordItem_;
	WordItem *generalizationAssumptionWordItem_;
	WordItem *oppositePossessiveSpecificationWordItem_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;


	// Private functions

	void clearPostponedNegativeConclusionVariables()
		{
		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;
		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		}

	bool isFeminineOrMasculineProperNameEnding( bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isFemaleSpecificationWord;
		bool isMaleSpecificationWord;

		if( generalizationWordItem != NULL &&
		specificationWordItem != NULL )
			{
			isFemaleSpecificationWord = specificationWordItem->isFemale();
			isMaleSpecificationWord = specificationWordItem->isMale();

			if( isFemaleSpecificationWord ||
			isMaleSpecificationWord )
				{
				if( specificationWordItem->isNounWordSpanishAmbiguous() )
					{
					if( ( isPossessive &&
					relationWordItem != NULL &&
					relationWordItem->hasFeminineProperNameEnding() ) ||

					( !isPossessive &&
					generalizationWordItem->hasFeminineProperNameEnding() ) )
						return true;
					}
				else
					{
					// Possessive (relation)
					if( ( isPossessive &&
					relationWordItem != NULL &&

					( ( isFemaleSpecificationWord &&
					relationWordItem->hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					relationWordItem->hasMasculineProperNameEnding() ) ) ) ||

					// Non-possessive (generalization)
					( !isPossessive &&

					( ( isFemaleSpecificationWord &&
					generalizationWordItem->hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					generalizationWordItem->hasMasculineProperNameEnding() ) ) ) )
						return true;
					}
				}
			}

		return false;
		}

	signed char askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, bool isNegativeConclusion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool isCommonWordWordSpanishAmbiguous;
		bool isHiddenSpanishPrimarySpecification;
		bool isSpecificationWordSpanishAmbiguous;
		bool isFemaleUserSpecificationWord = false;
		bool isMaleUserSpecificationWord = false;
		unsigned int anotherPrimarySpecificationCollectionNr;
		unsigned int nonCompoundCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *firstNonCompoundCollectionSpecificationItem;
		SpecificationItem *questionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem = NULL;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *commonWordItem;
		WordItem *userSpecificationWordItem;
		WordItem *collectionWordItem = NULL;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askSpecificationSubstitutionQuestionOrDrawNegativeConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( anotherPrimarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem->specificationCollectionNr() ) == NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification collection" );

		if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item has no specification word item" );

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );

		if( ( commonWordItem = anotherPrimarySpecificationWordItem->commonWordItem( anotherPrimarySpecificationCollectionNr ) ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the common word of the given specification collection number" );

		if( ( specificationCollectionNr = commonWordItem->collectionNr( specificationWordTypeNr, specificationWordItem ) ) == NO_COLLECTION_NR )
			specificationCollectionNr = anotherPrimarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		isCommonWordWordSpanishAmbiguous = commonWordItem->isNounWordSpanishAmbiguous();
		isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();
		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		relatedSpecificationItem == NULL &&
		!userSpecificationItem->isPossessive() &&
		( nonCompoundCollectionNr = commonWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
			{
			if( nonCompoundCollectionNr != specificationCollectionNr )
				{
				if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, nonCompoundCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

				relatedSpecificationItem = relatedResult.relatedSpecificationItem;
				}
			}

		if( relatedSpecificationItem == NULL )
			{
			hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
			hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

			if( userSpecificationItem != primarySpecificationItem &&
			!userSpecificationItem->hasRelationContext() &&
			!userSpecificationItem->isExclusiveSpecification() &&
			commonVariables_->nUserSpecificationWords == 1 &&
			( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
				{
				if( userSpecificationWordItem->isFemale() )
					isFemaleUserSpecificationWord = true;

				if( userSpecificationWordItem->isMale() )
					isMaleUserSpecificationWord = true;
				}

			if( isFemaleUserSpecificationWord ||
			isMaleUserSpecificationWord ||

			// Specification is Spanish ambiguous
			( isSpecificationWordSpanishAmbiguous &&
			!generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() &&

			( hasGeneralizationWordFeminineProperNameEnding ||
			userSpecificationItem->isQuestion() ) ) )
				{
				if( isFemaleUserSpecificationWord ||
				isMaleUserSpecificationWord )
					{
					if( anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() &&

					( anotherPrimarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ||

					( isFemaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem->isMale() ) ||

					( isMaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem->isFemale() ) ) &&

					// Draw negative conclusion
					drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					if( !isHiddenSpanishPrimarySpecification ||
					!isSpecificationWordSpanishAmbiguous )
						{
						postponeNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;
						postponeNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
						postponeNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
						}
					}
				}
			else
				{
				// Skip invalid questions (step 1)
				if( ( ( !hasGeneralizationWordFeminineProperNameEnding &&
				!hasGeneralizationWordMasculineProperNameEnding ) ||

				primarySpecificationItem->hasSpecificationCompoundCollection() ) &&

				( !isSpecificationWordSpanishAmbiguous ||
				anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ||
				anotherPrimarySpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, specificationWordItem );
					else
						{
						if( !isNegativeConclusion )
							collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr );
						}

					// Skip invalid questions (step 2)
					if( ( !isNegativeConclusion &&
					collectionWordItem != NULL &&
					generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, collectionWordItem ) == NULL ) ||

					( isSpecificationWordSpanishAmbiguous &&

					( generalizationWordItem->isUserGeneralizationWord ||
					existingSpecificationItem == NULL ||
					existingSpecificationItem->isHiddenSpanishSpecification() ) ) )
						{
						if( isSpecificationWordSpanishAmbiguous &&
						!primarySpecificationItem->isExclusiveSpecification() &&
						primarySpecificationItem->isUserSpecification() )
							{
							postponeNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;
							postponeNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
							postponeNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
							}
						else
							{
							if( primarySpecificationItem->hasRelationContext() ||
							primarySpecificationItem->isUserSpecification() ||
							generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) == NULL )
								{
								if( adminItem_->askQuestions() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions" );
								}
							}
						}
					}
				}
			}
		else
			{
			if( anotherPrimarySpecificationItem->isExclusiveSpecification() )
				{
				if( !isCommonWordWordSpanishAmbiguous &&
				( firstNonCompoundCollectionSpecificationItem = generalizationWordItem->firstNonCompoundCollectionSpecificationItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
					relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

				if( postponeNegativeConclusionSpecificationCollectionNr_ == anotherPrimarySpecificationCollectionNr &&
				// Draw postponed negative conclusion
				drawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, postponeNegativeConclusionPrimarySpecificationItem_, postponeNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				clearPostponedNegativeConclusionVariables();

				// Typically for the Spanish language
				if( isCommonWordWordSpanishAmbiguous &&
				isSpecificationWordSpanishAmbiguous &&
				// specificationWordItem == commonWordItem &&
				commonVariables_->nUserSpecificationWords == 1 &&
				!primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
				primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr &&

				( primarySpecificationItem->isUserSpecification() ||
				generalizationWordItem->isUserRelationWord ) )
					{
					questionSpecificationItem = generalizationWordItem->firstActiveAssignmentOrSpecificationItem( isArchivedAssignment, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem );

					if( ( questionSpecificationItem == NULL ||
					questionSpecificationItem->isOlderItem() ) &&

					generalizationWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem ) == NULL )
						{
						if( adminItem_->askQuestions() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions" );

						// Clean-up obsolete justifications
						if( generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					{
					if( anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ||
					anotherPrimarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ||

					( !relatedSpecificationItem->isHiddenSpanishSpecification() &&
					anotherPrimarySpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ) )
						{
						if( drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( isHiddenSpanishPrimarySpecification &&
						specificationWordItem == commonWordItem &&
						anotherPrimarySpecificationWordItem == commonWordItem &&
						userSpecificationItem != relatedSpecificationItem &&
						primarySpecificationItem != relatedSpecificationItem &&
						!relatedSpecificationItem->hasRelationContext() &&
						relatedSpecificationItem->isSelfGenerated() )
							{
							spanishAmbiguousCompoundPrimarySpecificationItem_ = relatedSpecificationItem;
							spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByOppositeQuestion( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem *createdNewSpecificationItem;
		SpecificationItem *createdOldSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem *oldSuggestiveSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "correctSuggestiveAssumptionsByOppositeQuestion";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( !secondarySpecificationItem->isActiveAssignment() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is no assignment" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Previously, a suggestive question assumption was made.
		// However, now a question with the opposite specification is asked.
		// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem->olderSuggestiveQuestionAssumptionSecondarySpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem->isActiveAssignment() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, oldSuggestiveQuestionAssumptionAssignmentItem, NULL, generalizationWordItem, oldSuggestiveSpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oldSuggestiveSpecificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		if( ( correctedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem->correctedAssumptionByOppositeQuestionJustificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the old corrected assumption by opposite question justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		// Change primary user specification by primary corrected question specification
		if( correctedAssumptionByOppositeQuestionJustificationItem->changePrimarySpecification( createdNewSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the primary specification of the corrected assumption by opposite question justification item" );

		if( ( secondarySpecificationWordItem = correctedAssumptionByOppositeQuestionJustificationItem->secondarySpecificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );

		// Correct generalization assumption
		if( ( primarySpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, false, NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );

		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdOldSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution assumption" );

		if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the corrected generalization assumption" );

		if( foundSpecificationItem->attachJustificationToSpecification( correctedAssumptionByOppositeQuestionJustificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );

		return RESULT_OK;
		}

	signed char drawNegativeConclusion( bool isArchivedAssignment, bool isSpecificationWordSpanishAmbiguous, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasDisplayedMoreSpecificNonExclusiveSpecification;
		bool isHiddenSpanishPrimarySpecification;
		bool isPrimarySameAsSecondarySpecification;
		bool isUserGeneralizationWord;
		bool hasAnotherSecondarySpecificationWithCompoundCollection = false;
		bool hasFoundOldSpanishUserSpecification = false;
		bool hasRelatedSpecificationsInSameSentence = false;
		bool isCompoundUserSpecification = false;
		bool isPostponingNegativeConclusion = false;
		bool isUserSpecificationSelected = false;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned int nonCompoundCollectionNr;
		unsigned int secondarySpecificationCollectionNr;
		unsigned int anotherPrimarySpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *createdJustificationItem;
		JustificationItem *negativeAssumptionJustificationItem;
		JustificationItem *negativeAssumptionOrConclusionJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingNegativeSpecificationItem;
		SpecificationItem *existingPositiveSpecificationItem;
		SpecificationItem *existingQuestionSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *negativeArchivedAssignmentItem;
		SpecificationItem *negativeConclusionPrimarySpecificationItem;
		SpecificationItem *negativeConclusionAnotherPrimarySpecificationItem;
		SpecificationItem *negativeConclusionSecondarySpecificationItem;
		SpecificationItem *negativeConclusionAnotherSecondarySpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *foundNegativeAssumptionOrConclusionPrimarySpecificationItem = NULL;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem = NULL;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *collectionWordItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		WordItem *secondarySpecificationCommonWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationItem->isPossessive() ||
		primarySpecificationItem->isQuestion() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item shouldn't be possessive, nor a question" );

		if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		if( anotherPrimarySpecificationItem == NULL )
			anotherPrimarySpecificationWordItem = primarySpecificationWordItem;
		else
			{
			anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem->specificationCollectionNr();
			anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem();
			}

		if( anotherPrimarySpecificationWordItem == NULL ||
		( userSpecificationItem = adminItem_->userSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );

		// If exists, replace self-generated assumption for conclusion
		if( primarySpecificationItem->isSelfGeneratedAssumption() &&
		( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem->isArchivedAssignment(), primarySpecificationItem->isNegative(), primarySpecificationItem->isPossessive(), false, primarySpecificationWordItem, NULL ) ) != NULL )
			primarySpecificationItem = tempSpecificationItem;

		existingPositiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

		hasDisplayedMoreSpecificNonExclusiveSpecification = ( primarySpecificationItem == existingPositiveSpecificationItem &&
															primarySpecificationItem->isExclusiveSpecification() );
		isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

		secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

		if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != NULL )
			foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem->primarySpecificationItem();

		if( ( hasDisplayedMoreSpecificNonExclusiveSpecification ||
		// No positive specification exists with this specification word
		existingPositiveSpecificationItem == NULL ||
		existingPositiveSpecificationItem->isHiddenSpanishSpecification() ) &&

		// Prefer primary specification with the least uncertainty
		( foundNegativeAssumptionOrConclusionPrimarySpecificationItem == NULL ||
		!primarySpecificationItem->hasSpecificationCollection() ||
		primarySpecificationItem->assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem->assumptionLevel() ) &&

		// Select correct compound specification collection when Spanish ambiguous
		( !isSpecificationWordSpanishAmbiguous ||
		generalizationWordItem->isUserRelationWord ||
		anotherPrimarySpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ||

		( anotherPrimarySpecificationItem != NULL &&
		anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ) ) &&

		( ( existingQuestionSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) ) == NULL ||

		( isUserGeneralizationWord &&
		generalizationWordItem->isOlderItem() &&
		// Skip adjusted question
		!existingQuestionSpecificationItem->isOlderItem() ) ) )
			{
			if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

			isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

			if( isPrimarySameAsSecondarySpecification )
				{
				if( primarySpecificationItem->hasSpecificationCompoundCollection() &&
				primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
					{
					if( ( collectionWordItem = collectionWordItemInCollectionWords( anotherPrimarySpecificationCollectionNr, primarySpecificationWordItem ) ) != NULL &&
					( anotherSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, collectionWordItem ) ) == NULL &&
					( anotherSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == NULL )
						anotherSecondarySpecificationItem = ( !userSpecificationItem->isSpecificationWordSpanishAmbiguous() ? userSpecificationItem : primarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, collectionWordItem ) );
					}
				else
					{
					if( commonVariables_->nUserSpecificationWords > 1 &&
					commonVariables_->nUserRelationWords == 0 &&
					userSpecificationItem->isSpecificationWordSpanishAmbiguous() )
						hasRelatedSpecificationsInSameSentence = true;
					}
				}
			else
				{
				if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
					{
					if( isHiddenSpanishPrimarySpecification &&
					anotherPrimarySpecificationItem != NULL &&
					anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() &&
					anotherPrimarySpecificationItem->isSelfGenerated() &&
					generalizationWordItem->isUserRelationWord &&
					( collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
						{
						if( ( nonCompoundCollectionNr = collectionWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) == NO_COLLECTION_NR )
							nonCompoundCollectionNr = anotherPrimarySpecificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );

						if( nonCompoundCollectionNr > NO_COLLECTION_NR &&
						( tempSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, nonCompoundCollectionNr, generalizationContextNr, NO_CONTEXT_NR, NULL ) ) != NULL )
							{
							hasFoundOldSpanishUserSpecification = true;
							secondarySpecificationItem = tempSpecificationItem;
							}
						}

					if( !hasFoundOldSpanishUserSpecification )
						{
						if( ( anotherSecondarySpecificationItem = secondarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, NULL ) ) == NULL )
							{
							if( secondarySpecificationItem->hasSpecificationCompoundCollection() &&
							secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
							( secondarySpecificationCommonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL &&
							secondarySpecificationCommonWordItem != primarySpecificationWordItem )
								anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, secondarySpecificationWordItem );
							}
						else
							{
							if( !anotherSecondarySpecificationItem->isExclusiveSpecification() &&
							( collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
								{
								if( ( tempSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, collectionWordItem ) ) == NULL )
									isPostponingNegativeConclusion = true;
								else
									secondarySpecificationItem = tempSpecificationItem;

								anotherSecondarySpecificationItem = NULL;
								}
							}
						}
					}
				}

			if( !isPostponingNegativeConclusion )
				{
				if( isUserGeneralizationWord &&
				( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( userSpecificationWordItem->isNounWordSpanishAmbiguous() )
						isUserSpecificationWordSpanishAmbiguous = true;

					if( !isHiddenSpanishPrimarySpecification ||

					( !userSpecificationItem->isHiddenSpanishSpecification() &&

					( isUserSpecificationWordSpanishAmbiguous ||
					userSpecificationItem->isPossessive() ) ) )
						{
						isUserSpecificationSelected = true;

						if( !isPrimarySameAsSecondarySpecification &&
						anotherSecondarySpecificationItem != NULL &&
						userSpecificationItem->hasSpecificationCompoundCollection() &&
						userSpecificationWordItem->hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
							isCompoundUserSpecification = true;
						}
					else
						{
						if( isSpecificationWordSpanishAmbiguous &&
						!anotherPrimarySpecificationWordItem->isNounWordSpanishAmbiguous() &&
						( simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL )
							{
							isUserSpecificationSelected = true;
							userSpecificationItem = simpleUserSpecificationItem;

							if( isHiddenSpanishPrimarySpecification &&
							anotherSecondarySpecificationItem != NULL &&
							!anotherSecondarySpecificationItem->isGeneralizationProperName() &&
							anotherPrimarySpecificationCollectionNr == anotherSecondarySpecificationItem->specificationCollectionNr() )
								hasAnotherSecondarySpecificationWithCompoundCollection = true;
							}
						}

					if( isUserSpecificationSelected &&
					userSpecificationItem->isPossessive() )
						oppositePossessiveDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
					}

				existingNegativeSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

				if( hasFoundOldSpanishUserSpecification ||
				isCompoundUserSpecification ||

				( ( existingNegativeSpecificationItem == NULL ||

				( !isHiddenSpanishPrimarySpecification &&

				( existingNegativeSpecificationItem->isSelfGeneratedAssumption() ||
				primarySpecificationItem->hasSpecificationCompoundCollection() ) &&

				( existingNegativeSpecificationItem->isOlderItem() ||
				existingNegativeSpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) &&

				generalizationWordItem->primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, primarySpecificationItem ) == NULL ) ||

				( isUserSpecificationSelected &&
				isUserGeneralizationWord &&

				( ( !primarySpecificationItem->hasRelationContext() &&

				( existingNegativeSpecificationItem == NULL ||
				existingNegativeSpecificationItem->isOlderItem() ) ) ||

				( isHiddenSpanishPrimarySpecification &&
				existingNegativeSpecificationItem != NULL &&

				( existingNegativeSpecificationItem->isOlderItem() ||
				existingNegativeSpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) ) )
					{
					if( isHiddenSpanishPrimarySpecification )
						{
						if( isUserSpecificationSelected )
							{
							if( hasAnotherSecondarySpecificationWithCompoundCollection )
								{
								negativeConclusionPrimarySpecificationItem = anotherPrimarySpecificationItem;
								negativeConclusionAnotherPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem;
								negativeConclusionSecondarySpecificationItem = userSpecificationItem;
								negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ? anotherSecondarySpecificationItem : NULL );
								}
							else
								{
								if( ( isUserSpecificationWordSpanishAmbiguous &&
								userSpecificationItem->hasSpecificationNonCompoundCollection() ) ||

								( anotherPrimarySpecificationItem != NULL &&
								anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() ) )
									{
									negativeConclusionPrimarySpecificationItem = anotherPrimarySpecificationItem;
									negativeConclusionAnotherPrimarySpecificationItem = NULL;
									negativeConclusionSecondarySpecificationItem = secondarySpecificationItem;
									negativeConclusionAnotherSecondarySpecificationItem = anotherSecondarySpecificationItem;
									}
								else
									{
									negativeConclusionPrimarySpecificationItem = userSpecificationItem;
									negativeConclusionAnotherPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem;
									negativeConclusionSecondarySpecificationItem = anotherPrimarySpecificationItem;
									negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationItem->isHiddenSpanishSpecification() ? userSpecificationItem : secondarySpecificationItem );
									}
								}
							}
						else
							{
							negativeConclusionPrimarySpecificationItem = anotherPrimarySpecificationItem;
							negativeConclusionAnotherPrimarySpecificationItem = oppositePossessiveDefinitionSpecificationItem;
							negativeConclusionSecondarySpecificationItem = secondarySpecificationItem;
							negativeConclusionAnotherSecondarySpecificationItem = anotherSecondarySpecificationItem;
							}
						}
					else	// Not isHiddenSpanishPrimarySpecification
						{
						if( !isPrimarySameAsSecondarySpecification &&
						isUserSpecificationSelected &&
						primarySpecificationItem->isSelfGenerated() &&
						primarySpecificationItem->hasSpecificationCompoundCollection() &&
						!userSpecificationItem->isQuestion() &&
						userSpecificationItem->hasSpecificationCompoundCollection() )
							compoundGeneralizationWordItem = anotherPrimarySpecificationWordItem->compoundGeneralizationWordItem( anotherPrimarySpecificationCollectionNr );

						if( compoundGeneralizationWordItem != NULL &&
						compoundGeneralizationWordItem->isNounWordSpanishAmbiguous() )
							{
							negativeConclusionPrimarySpecificationItem = compoundGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
							negativeConclusionAnotherPrimarySpecificationItem = NULL;
							negativeConclusionSecondarySpecificationItem = anotherSecondarySpecificationItem;
							negativeConclusionAnotherSecondarySpecificationItem = NULL;
							}
						else
							{
							if( hasDisplayedMoreSpecificNonExclusiveSpecification )
								{
								negativeConclusionPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? anotherSecondarySpecificationItem : generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, secondarySpecificationCommonWordItem ) );
								negativeConclusionAnotherPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? NULL : anotherSecondarySpecificationItem );
								negativeConclusionSecondarySpecificationItem = userSpecificationItem;
								negativeConclusionAnotherSecondarySpecificationItem = NULL;
								}
							else
								{
								negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
								negativeConclusionAnotherPrimarySpecificationItem = ( hasRelatedSpecificationsInSameSentence ? NULL : anotherPrimarySpecificationItem );
								negativeConclusionSecondarySpecificationItem = ( isCompoundUserSpecification ? userSpecificationItem : ( isPrimarySameAsSecondarySpecification ? ( hasRelatedSpecificationsInSameSentence ? anotherPrimarySpecificationItem : ( primarySpecificationItem != userSpecificationItem && !userSpecificationItem->hasRelationContext() && userSpecificationItem->specificationCollectionNr() == anotherPrimarySpecificationCollectionNr ? userSpecificationItem : anotherSecondarySpecificationItem ) ) : secondarySpecificationItem ) );
								negativeConclusionAnotherSecondarySpecificationItem = ( isCompoundUserSpecification || isPrimarySameAsSecondarySpecification ? NULL : anotherSecondarySpecificationItem );
								}
							}
						}

					// Draw negative conclusion
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, primarySpecificationItem->isExclusiveSpecification(), false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationWordSpanishAmbiguous && primarySpecificationItem->isSelfGeneratedAssumption() ? JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", anotherPrimarySpecificationWordItem->anyWordTypeString(), "\"" );

					createdJustificationItem = createAndAssignResult.createdJustificationItem;

					// Found specification
					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
						{
						if( createdJustificationItem != NULL )
							{
							// Cleanup after found specification
							if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == NULL )
								return adminItem_->startError( functionNameString, moduleNameString_, "The found specification item is undefined" );

							if( negativeAssumptionOrConclusionJustificationItem == NULL )
								{
								if( foundSpecificationItem->isSelfGeneratedAssumption() )
									{
									if( createdJustificationItem->isConclusionJustification() )
										{
										if( foundSpecificationItem->markAsConcludedAssumption( false ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

										if( isArchivedAssignment )
											{
											if( ( negativeArchivedAssignmentItem = generalizationWordItem->firstAssignmentItem( false, false, true, true, false, true, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == NULL )
												return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a past tense negative assumption assignment" );

											if( negativeArchivedAssignmentItem->isSelfGeneratedAssumption() &&
											// Also mark the past tense negative assumption assignment as concluded
											negativeArchivedAssignmentItem->markAsConcludedAssumption( false ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark a past tense negative assumption assignment as concluded" );
											}
										}
									}
								else
									{
									if( generalizationWordItem->isJustificationInUse( createdJustificationItem ) )
										{
										// Check for negative assumption justification
										negativeAssumptionJustificationItem = foundSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION );

										if( negativeAssumptionJustificationItem != NULL &&
										negativeAssumptionJustificationItem->primarySpecificationItem() == negativeConclusionPrimarySpecificationItem &&
										generalizationWordItem->replaceJustification( negativeAssumptionJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									else
										{
										if( generalizationWordItem->replaceOrDeleteJustification( createdJustificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the created justification" );
										}
									}
								}
							else
								{
								// Replace justification of negative conclusion / assumption
								if( generalizationWordItem->replaceJustification( negativeAssumptionOrConclusionJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}
					else
						{
						// Created specification
						// Replace corrected negative conclusion
						if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() )
							{
							if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created negative conclusion" );

							relatedSpecificationItem = relatedResult.relatedSpecificationItem;

							if( relatedSpecificationItem != NULL &&
							relatedSpecificationItem->isOlderItem() &&
							generalizationWordItem->replaceOrDeleteSpecification( false, relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( isCompoundUserSpecification &&
							!isPrimarySameAsSecondarySpecification &&
							anotherSecondarySpecificationItem != NULL &&
							// Check for answered question justification
							createdSpecificationItem->answeredQuestionJustificationItem() == NULL &&
							generalizationWordItem->changeJustificationOfNegativeAssumptions( userSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a question justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFeminineProperNameEnding;
		bool isAssignment;
		bool isConclusionAssignment;
		bool isForcingNewJustification;
		bool isUserGeneralizationWord;
		bool isSecondarySpecificationWordSpanishAmbiguous;
		bool isAssumption = ( userAssumptionLevel > NO_ASSUMPTION_LEVEL );
		bool isReplacingObsoleteJustifications = false;
		unsigned int foundRelationContextNr;
		unsigned int nContextRelations;
		unsigned int generalizationContextNr = NO_CONTEXT_NR;
		unsigned int spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
		GeneralizationItem *currentGeneralizationItem;
		JustificationItem *conclusionJustificationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *collectedSpecificationItem;
		SpecificationItem *confirmedAllRelationWordsOfSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *justificationSecondarySpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *collectedWithItselfCommonWordItem;
		WordItem *commonWordItem;
		WordItem *currentGeneralizationWordItem;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( relationContextNr <= NO_CONTEXT_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation context number is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
		existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, !isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

		if( existingSpecificationItem == NULL ||
		// Allow existing conclusion specification to change its relation context
		existingSpecificationItem->isSelfGenerated() )
			{
			isSecondarySpecificationWordSpanishAmbiguous = secondarySpecificationWordItem->isNounWordSpanishAmbiguous();

			if( isPossessive &&
			isUserGeneralizationWord &&
			!isSecondarySpecificationWordSpanishAmbiguous &&

			( existingSpecificationItem == NULL ||
			commonVariables_->nUserRelationWords > 1 ) &&

			( foundRelationContextNr = generalizationWordItem->contextNr( specificationWordItem ) ) > NO_CONTEXT_NR )
				{
				if( ( nContextRelations = adminItem_->nContextWordsInContextWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
					{
					if( ( contextResult = adminItem_->getGeneralizationContext( isArchivedAssignment, false, false, false, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the generalization context number" );

					generalizationContextNr = contextResult.contextNr;
					}
				else
					generalizationContextNr = foundRelationContextNr;
				}

			confirmedAllRelationWordsOfSpecificationItem = contextResult.confirmedAllRelationWordsOfSpecificationItem;
			existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Find the non-reversible specification (active assignments)
			if( isSecondarySpecificationWordSpanishAmbiguous )
				secondarySpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );
			else
				secondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Skip if possessive user specification exists
			if( secondarySpecificationItem != NULL &&
			secondarySpecificationItem->hasNewInformation() &&

			// User specification doesn't exist
			( ( existingUserSpecificationItem == NULL &&

			( secondarySpecificationItem->isUserSpecification() ||
			relationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, secondarySpecificationItem ) == NULL ) ) ||

			// User specification exists
			( existingUserSpecificationItem != NULL &&

			( ( !isSecondarySpecificationWordSpanishAmbiguous &&

			( isPossessive ||
			existingUserSpecificationItem->isAssignment() ) ) ||

			// Typically for the Spanish language
			( isSecondarySpecificationWordSpanishAmbiguous &&

			( !existingUserSpecificationItem->hasRelationContext() ||
			generalizationWordItem->hasDisplayedMoreSpecificSpecification() ) ) ) ) ) )
				{
				hasFeminineProperNameEnding = ( isPossessive &&
												isSecondarySpecificationWordSpanishAmbiguous &&
												relationWordItem->hasFeminineProperNameEnding() );

				isConclusionAssignment = ( ( isArchivedAssignment &&
											isAssumption ) ||

											( !isAssumption &&

											( isArchivedAssignment ||
											secondarySpecificationItem->isExclusiveNonPossessiveGeneralizationAssignment() ) ) );

				isAssignment = ( isConclusionAssignment ||

								( existingUserSpecificationItem != NULL &&
								existingUserSpecificationItem->isAssignment() ) );

				// Typically for the Spanish language
				isForcingNewJustification = ( ( isSecondarySpecificationWordSpanishAmbiguous ||
											secondarySpecificationItem->isUserAssignment() ||

											( !isArchivedAssignment &&
											isPossessive &&
											isUserGeneralizationWord &&
											existingSpecificationItem != NULL &&
											secondarySpecificationItem->isUserSpecification() ) ) &&

											secondarySpecificationItem->hasSpecificationCollection() );

				// This condition is typical for the Spanish language
				if( isSecondarySpecificationWordSpanishAmbiguous &&
				secondarySpecificationWordItem == specificationWordItem &&

				( !isPossessive ||
				!relationWordItem->isMale() ) )
					spanishAmbiguousSpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

				// Draw a possessive reversible conclusion
				// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
				if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasFeminineProperNameEnding, isAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, NULL, NULL, secondarySpecificationItem, NULL, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

				if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
					foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

				if( confirmedAllRelationWordsOfSpecificationItem == NULL )
					{
					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
						{
						if( existingSpecificationItem != NULL &&
						foundSpecificationItem != NULL )
							{
							if( existingSpecificationItem->isConcludedAssumption() )
								isReplacingObsoleteJustifications = true;
							else
								{
								if( !isSecondarySpecificationWordSpanishAmbiguous &&
								!existingSpecificationItem->isReplacedItem() &&
								existingSpecificationItem->isSelfGeneratedAssumption() &&
								foundSpecificationItem->isSelfGeneratedConclusion() &&
								foundSpecificationItem->hasSpecificationCompoundCollection() )
									{
									if( relationWordItem->replaceOrDeleteSpecification( false, existingSpecificationItem, foundSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption specification by a conclusion in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

									isReplacingObsoleteJustifications = true;
									}
								}

							if( isReplacingObsoleteJustifications &&
							// Clean-up obsolete justifications
							relationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						{
						if( !isPossessive &&
						foundSpecificationItem == NULL &&
						secondarySpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) == NULL &&
						( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, specificationWordItem, NULL ) ) != NULL &&
						( conclusionJustificationItem = conclusionSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != NULL &&
						( justificationSecondarySpecificationItem = conclusionJustificationItem->secondarySpecificationItem() ) != NULL )
							{
							// Additional justification
							if( ( justificationResult = generalizationWordItem->addJustification( false, false, conclusionSpecificationItem->hasSpecificationCollection(), JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, justificationSecondarySpecificationItem, NULL, secondarySpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

							if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
								return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

							if( secondarySpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the assumption specification" );
							}
						}

					if( isPossessive &&
					( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? foundSpecificationItem : createdSpecificationItem ) ) != NULL )
						{
						if( !createdOrFoundSpecificationItem->hasSpecificationCompoundCollection() &&
						makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, NULL, generalizationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( isSecondarySpecificationWordSpanishAmbiguous &&
						( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem->collectedWithItselfCommonWordItem() ) != NULL )
							{
							if( !isArchivedAssignment &&
							// Reversed specification doesn't already exist
							generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, true, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) == NULL &&

							// Generate the reversed specification
							drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem->assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdOrFoundSpecificationItem->relationContextNr(), relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

							if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
								{
								do	{
									if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

									if( ( partOfSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) ) != NULL &&
									partOfSpecificationItem->isPartOf() &&

									( !createdOrFoundSpecificationItem->hasAssumptionLevel() ||
									// Skip if 'part of' conclusion exists
									relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, currentGeneralizationWordItem, NULL ) == NULL ) )
										{
										// Draw a 'part of' generalization conclusion
										// Not yet explained in the design
										if( adminItem_->addSelfGeneratedSpecification( false, isConclusionAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, partOfSpecificationItem, NULL, relationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
										}
									}
								while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
								}

							if( relationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) == NULL &&
							( secondarySpecificationItem = collectedWithItselfCommonWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_QUESTION_PARAMETER, createdOrFoundSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&

							drawNegativeConclusion( isArchivedAssignment, false, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationItem, NULL, createdOrFoundSpecificationItem, NULL, relationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about relation word \"", relationWordItem->anyWordTypeString(), "\"" );
							}
						}

					if( isUserGeneralizationWord &&
					specificationWordItem->isUserSpecificationWord )
						{
						// Typically for the Spanish language
						if( isSecondarySpecificationWordSpanishAmbiguous )
							{
							if( ( existingSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, !isPossessive, true, specificationWordItem, NULL ) ) != NULL &&
							existingSpecificationItem->specificationCollectionNr() != spanishAmbiguousSpecificationCollectionNr )
								{
								if( foundSpecificationItem == NULL )
									{
									if( existingSpecificationItem->isHiddenSpanishSpecification() )
										{
										// Add context to hidden specification
										if( generalizationWordItem->addContext( false, relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem->relationContextNr(), specificationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

										// Not hidden anymore after addition of context
										if( !existingSpecificationItem->isHiddenSpanishSpecification() &&
										relationWordItem->writeUpdatedSpecification( true, false, false, false, false, existingSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the hidden assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									{
									if( secondarySpecificationItem != NULL )
										{
										// Additional justification
										if( ( justificationResult = relationWordItem->addJustification( false, true, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, secondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

										if( existingSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the existing specification" );
										}
									}
								}
							}
						else
							{
							if( foundSpecificationItem == NULL )
								{
								if( isPossessive &&
								createdSpecificationItem != NULL &&
								( assumptionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, NULL ) ) != NULL )
									{
									// Additional justification (assumption)
									if( ( justificationResult = relationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, secondarySpecificationItem, NULL, assumptionSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

									if( assumptionSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the assumption specification" );

									if( generalizationWordItem->addContext( false, relationWordTypeNr, specificationWordTypeNr, assumptionSpecificationItem->relationContextNr(), specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									collectedSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, false, false, NO_QUESTION_PARAMETER, assumptionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NULL );

									if( collectedSpecificationItem != NULL &&
									( firstJustificationItem = collectedSpecificationItem->firstJustificationItem() ) != NULL )
										{
										// Additional justification (another)
										if( ( justificationResult = relationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, firstJustificationItem->primarySpecificationItem(), NULL, collectedSpecificationItem, NULL, assumptionSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

										if( assumptionSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification (again) of the assumption specification" );

										// Additional justification (collected)
										if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, firstJustificationItem->primarySpecificationItem(), NULL, createdSpecificationItem, NULL, collectedSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

										if( collectedSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the collected specification" );
										}
									}
								}
							else
								{
								// Typically for the Spanish language
								if( secondarySpecificationItem != NULL &&
								secondarySpecificationItem->hasSpecificationCompoundCollection() &&
								foundSpecificationItem->isSelfGeneratedAssumption() &&
								( commonWordItem = specificationWordItem->commonWordItem( foundSpecificationItem->specificationCollectionNr() ) ) != NULL &&
								( assumptionSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, true, NO_QUESTION_PARAMETER, commonWordItem, generalizationWordItem ) ) != NULL &&
								assumptionSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) != NULL )
									{
									// Additional justification (Spanish)
									if( ( justificationResult = relationWordItem->addJustification( false, true, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, secondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

									if( assumptionSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the assumption specification" );
									}
								}
							}

						if( makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					{
					if( existingSpecificationItem != NULL )
						{
						confirmedAllRelationWordsOfSpecificationItem = confirmedAllRelationWordsOfSpecificationItem->updatedSpecificationItem();

						if( relationWordItem->replaceOrDeleteSpecification( false, existingSpecificationItem, confirmedAllRelationWordsOfSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

						// Clean-up obsolete justifications
						if( relationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char drawSpecificationGeneralizationConclusion( unsigned short assumptionLevel, SpecificationItem *secondarySpecificationItem )
		{
		bool isArchivedAssignment;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( !secondarySpecificationItem->isGeneralizationNoun() ||
		!secondarySpecificationItem->isSpecificationNoun() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );

		isArchivedAssignment = secondarySpecificationItem->isArchivedAssignment();

		// Draw a specification generalization conclusion
		// Not yet explained in the design
		if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationItem->isExclusiveSpecification(), false, secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPartOf(), secondarySpecificationItem->isPossessive(), false, false, true, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, secondarySpecificationItem->specificationWordItem(), secondarySpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );

		return RESULT_OK;
		}

	signed char makeGeneralizationAssumption( bool hasCorrectedAssumptionByKnowledge, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		JustificationItem *olderGeneralizationAssumptionBySpecificationJustificationItem;
		SpecificationItem *olderGeneralizationAssumptionBySpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumption";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Skip if user specification already exists
		if( generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) == NULL &&

		// Skip if justification with primary specification already exists
		( generalizationWordItem->primarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem ) == NULL ||
		// Skip if justification with secondary specification already exists
		generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, secondarySpecificationItem ) == NULL ) )
			{
			// Make a generalization assumption
			// Not yet explained in the design
			if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME ? false : primarySpecificationItem->isEveryGeneralization() ), false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			if( hasCorrectedAssumptionByKnowledge &&
			createAndAssignResult.createdJustificationItem != NULL &&
			( olderGeneralizationAssumptionBySpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
			( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != NULL &&

			generalizationWordItem->replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, createAndAssignResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		unsigned short questionParameter = ( isGeneralizationProperName ? WORD_PARAMETER_SINGULAR_VERB_IS : NO_QUESTION_PARAMETER );
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumptionByGeneralization";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a reasoning word type" );

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a noun" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				!currentGeneralizationWordItem->hasCollection() &&
				!currentGeneralizationWordItem->hasPossessiveButNonQuestionSpecification() &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
					{
					// Get secondary specification for justification
					if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
						{
						if( isGeneralizationProperName &&
						!primarySpecificationItem->isSelfGeneratedAssumption() &&
						!secondarySpecificationItem->isOlderItem() &&

						// Make generalization assumption
						makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						// Get secondary question specification for justification
						if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, questionParameter, specificationWordItem ) ) != NULL &&
						drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( !commonVariables_->hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makePossessiveConditionalSpecificationAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *alternativeSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool hasAnotherPrimarySpecificationCompoundCollection;
		bool hasSameSpecificationCollection;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveReversibleAssumption;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isRelationWordUserWordGeneralization;
		bool hasFeminineOrMasculineProperNameEnding = false;
		unsigned short assumptionJustificationTypeNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *foundJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *selfGeneratedConclusionSpecificationItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		WordItem *secondaryGeneralizationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *feminineSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makePossessiveConditionalSpecificationAssumption";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( possessiveDefinitionSpecificationItem_ != NULL &&
		!possessiveDefinitionSpecificationItem_->isDeletedItem() &&
		( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem() ) != NULL )
			{
			isPossessiveSpecificationWordSpanishAmbiguous = possessiveSpecificationWordItem->isNounWordSpanishAmbiguous();

			if( isPossessiveSpecificationWordSpanishAmbiguous ||
			!secondarySpecificationItem->hasSpecificationCompoundCollection() )
				{
				isRelationWordUserWordGeneralization = relationWordItem->isUserGeneralizationWord;

				existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, possessiveSpecificationWordItem );
				existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveSpecificationWordItem );

				// Skip if possessive user specification exists
				if( ( existingUserSpecificationItem == NULL &&
				!possessiveSpecificationWordItem->isUserSpecificationWord &&

				( !isPossessiveSpecificationWordSpanishAmbiguous ||

				// Typically for the Spanish language
				( isRelationWordUserWordGeneralization &&
				existingSpecificationItem == NULL ) ) &&

				// Skip if possessive specification exists
				relationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == NULL ) ||

				// Typically for the Spanish language
				( isPossessiveSpecificationWordSpanishAmbiguous &&
				alternativeSecondarySpecificationItem != NULL &&
				!alternativeSecondarySpecificationItem->isOlderItem() &&
				alternativeSecondarySpecificationItem->isActiveAssignment() &&

				// Skip if possessive user specification exists
				( existingUserSpecificationItem == NULL ||

				( existingUserSpecificationItem->isOlderItem() &&
				existingUserSpecificationItem->hasSpecificationCompoundCollection() ) ) ) )
					{
					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == possessiveSpecificationWordItem );

					// Make a possessive conditional specification assumption (normal)
					// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, isExclusiveSpecification, false, false, false, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isPossessive ? possessiveDefinitionSpecificationItem_ : NULL ), NULL, ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), NULL, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
						{
						createdOrFoundSpecificationItem = createAndAssignResult.foundSpecificationItem;

						if( isPossessive &&
						!isRelationWordUserWordGeneralization &&
						existingSpecificationItem != NULL &&
						existingSpecificationItem->isSelfGeneratedAssumption() &&
						( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != NULL &&
						( foundJustificationItem = existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != NULL )
							{
							secondaryGeneralizationWordItem = foundJustificationItem->secondaryGeneralizationWordItem();

							if( secondaryGeneralizationWordItem != NULL &&
							secondaryGeneralizationWordItem != generalizationWordItem &&
							( foundJustificationItem = relationWordItem->possessiveReversibleAssumptionJustificationItem( secondaryGeneralizationWordItem, secondarySpecificationWordItem ) ) != NULL )
								{
								// Additional justification
								if( ( justificationResult = relationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, possessiveDefinitionSpecificationItem_, NULL, foundJustificationItem->secondarySpecificationItem(), NULL, existingSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

								if( existingSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
								}
							}
						}
					else
						{
						createdOrFoundSpecificationItem = createdSpecificationItem;

						if( !isPossessive &&
						existingSpecificationItem != NULL )
							{
							existingSpecificationItem = existingSpecificationItem->updatedSpecificationItem();

							if( !createdSpecificationItem->isHiddenSpanishSpecification() &&
							createdSpecificationItem->assumptionLevel() < existingSpecificationItem->assumptionLevel() &&
							relationWordItem->replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
							}
						}

					if( !isPossessive &&
					createdOrFoundSpecificationItem != NULL &&

					( createdSpecificationItem != NULL ||

					( secondarySpecificationItem->secondarySpecificationJustificationItem( createdOrFoundSpecificationItem ) == NULL &&
					!secondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) &&

					!generalizationWordItem->hasCorrectedAssumptionByKnowledge() &&
					// Typically for the Spanish language
					!secondarySpecificationItem->isActiveAssignment() )
						{
						// Additional justification
						if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, createdOrFoundSpecificationItem, NULL, secondarySpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

						secondarySpecificationItem = secondarySpecificationItem->updatedSpecificationItem();

						if( secondarySpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the secondary specification" );
						}
					}

				if( !commonVariables_->hasDisplayedWarning )
					{
					if( oppositePossessiveSpecificationWordItem_ == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );

					if( alternativeSecondarySpecificationItem != NULL &&
					// Try to find a self-generated conclusion first
					( selfGeneratedConclusionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveSpecificationWordItem_, NULL ) ) == NULL &&
					// If not found, try to find a self-generated assumption
					( assumptionSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != NULL )
						{
						hasSameSpecificationCollection = ( assumptionSpecificationItem->specificationCollectionNr() == alternativeSecondarySpecificationItem->specificationCollectionNr() );

						if( !assumptionSpecificationItem->hasCurrentCreationSentenceNr() &&
						!assumptionSpecificationItem->hasNewInformation() &&

						( generalizationWordItem->hasConfirmedSpecification() == hasSameSpecificationCollection ||
						alternativeSecondarySpecificationItem->isPossessive() ) )
							selfGeneratedConclusionSpecificationItem = assumptionSpecificationItem;
						}

					isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_->isNounWordSpanishAmbiguous();

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
						feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_->feminineCollectionWordItem();
					else
						feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_->feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem->hasFeminineProperNameEnding() : relationWordItem->hasFeminineProperNameEnding() );

					if( feminineSpecificationWordItem != NULL )
						{
						if( isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
							anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, feminineSpecificationWordItem );
						else
							feminineSpecificationWordItem = NULL;
						}

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

					( alternativeSecondarySpecificationItem == NULL ||
					alternativeSecondarySpecificationItem->isUserSpecification() ) &&

					isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
						hasFeminineOrMasculineProperNameEnding = true;

					hasAnotherPrimarySpecificationCompoundCollection = ( alternativeSecondarySpecificationItem != NULL &&
																		alternativeSecondarySpecificationItem->hasSpecificationCompoundCollection() );
					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == oppositePossessiveSpecificationWordItem_ );

					assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION :
													( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
													!hasAnotherPrimarySpecificationCompoundCollection &&
													!hasFeminineOrMasculineProperNameEnding &&
													isRelationWordUserWordGeneralization ) ? JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

					if( ( selfGeneratedConclusionSpecificationItem == NULL ||
					!generalizationWordItem->isOlderItem() ||

					( isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem->isSelfGenerated() ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!selfGeneratedConclusionSpecificationItem->hasRelationContext() ) ) &&

					( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!secondarySpecificationItem->isConcludedAssumption() &&

					( relationWordItem->secondarySpecificationJustificationItem( true, assumptionJustificationTypeNr, secondarySpecificationItem ) == NULL ||

					( feminineSpecificationWordItem != NULL &&

					( hasAnotherPrimarySpecificationCompoundCollection ||
					isPossessiveReversibleAssumption ) &&

					// Skip if feminine specification exists as user specification
					relationWordItem->firstUserSpecificationItem( false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, feminineSpecificationWordItem ) == NULL ) ) ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					alternativeSecondarySpecificationItem != NULL &&
					// Skip if already exists as self-generated assumption
					relationWordItem->primarySpecificationJustificationItem( true, assumptionJustificationTypeNr, alternativeSecondarySpecificationItem ) == NULL ) ) )
						{
						// Make a possessive conditional specification assumption (opposite)
						// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, secondarySpecificationItem->assumptionLevel(), assumptionJustificationTypeNr, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isPossessiveReversibleAssumption ? NULL : ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : possessiveDefinitionSpecificationItem_ ) ), NULL, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( hasFeminineOrMasculineProperNameEnding && !isPossessive ? anotherPrimarySpecificationItem : possessiveDefinitionSpecificationItem_ ) : secondarySpecificationItem ), NULL, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

						if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
							{
							foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

							if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
							( assumptionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, !isPossessive, true, oppositePossessiveSpecificationWordItem_, relationWordItem ) ) != NULL )
								{
								// Additional justifications (Generalization word)
								if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, foundSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL &&
								generalizationWordItem->attachJustificationInWord( createdJustificationItem, assumptionSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the given assumption specification" );

								// Additional justification (Relation word)
								if( ( justificationResult = relationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, assumptionSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", relationWordItem->anyWordTypeString(), "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL &&
								foundSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the found specification" );
								}
							else
								{
								if( isPossessiveSpecificationWordSpanishAmbiguous &&
								// Clean-up obsolete justifications
								relationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
								}
							}

						if( alternativeSecondarySpecificationItem != NULL &&
						feminineSpecificationWordItem != NULL )
							{
							if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, isPossessive, NO_QUESTION_PARAMETER, feminineSpecificationWordItem, generalizationWordItem ) ) == NULL ||

							( existingSpecificationItem->isSelfGeneratedAssumption() &&

							( !secondarySpecificationItem->isHiddenSpanishSpecification() ||

							( !relationWordItem->hasConfirmedSpecification() &&
							alternativeSecondarySpecificationItem->hasSpecificationCollection() ) ) ) )
								{
								// Make an opposite possessive feminine proper name ending assumption
								// Not yet explained in the design
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( existingSpecificationItem == NULL ? NO_CONTEXT_NR : existingSpecificationItem->relationContextNr() ), alternativeSecondarySpecificationItem, ( !isPossessive && isOppositePossessiveSpecificationWordSpanishAmbiguous && oppositePossessiveDefinitionSpecificationItem_ != NULL ? oppositePossessiveDefinitionSpecificationItem_ : possessiveDefinitionSpecificationItem_ ), anotherPrimarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );

								if( !isPossessive )
									{
									feminineSpecificationWordItem_ = feminineSpecificationWordItem;

									if( !isArchivedAssignment &&
									( createdOrFoundSpecificationItem = ( createAndAssignResult.createdSpecificationItem == NULL ? createAndAssignResult.foundSpecificationItem : createAndAssignResult.createdSpecificationItem ) ) != NULL &&

									( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
									!createdOrFoundSpecificationItem->isReplacedItem() ) &&

									// Generate the reversed specification
									drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem->assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, createdOrFoundSpecificationItem->specificationContextNr(), createdOrFoundSpecificationItem->relationContextNr(), relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char makeSpecificationSubstitutionPartOfAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordConfirmedSpecification;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool isGeneralizationProperName;
		bool isGeneralizationWordSpanishAmbiguous;
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		GeneralizationItem *currentGeneralizationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *primaryPartOfSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *collectionWordItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *feminineCollectionWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSpecificationSubstitutionPartOfAssumption";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem != specificationWordItem &&
		( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();
			hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();
			hasGeneralizationWordConfirmedSpecification = generalizationWordItem->hasConfirmedSpecification();
			hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
			isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
			isGeneralizationWordSpanishAmbiguous = generalizationWordItem->isNounWordSpanishAmbiguous();
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem();

			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&

				( currentGeneralizationWordItem->hasNonPossessiveAndNonQuestionPartOfSpecification() ||

				( ( !currentGeneralizationWordItem->hasCollection() ||

				( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
				!hasCorrectedAssumptionByOppositeQuestion ) ) &&

				!currentGeneralizationWordItem->hasPossessiveButNonQuestionSpecification() ) ) )
					{
					existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

					// Skip if concluded conclusion exists
					if( ( existingSpecificationItem == NULL ||
					existingSpecificationItem != simpleUserSpecificationItem ) &&

					// Get primary specification for justification
					( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
						{
						alternativeAnotherPrimarySpecificationItem_ = NULL;
						secondarySpecificationItem = NULL;

						// Get secondary specification for justification
						if( isUserGeneralizationWord &&
						simpleUserSpecificationItem != NULL &&
						!simpleUserSpecificationItem->isPossessive() &&

						( ( isSpecificationWordSpanishAmbiguous &&
						simpleUserSpecificationItem->specificationWordItem() == primarySpecificationItem->specificationWordItem() ) ||

						simpleUserSpecificationItem->specificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() ) )
							secondarySpecificationItem = simpleUserSpecificationItem;
						else
							{
							if( isSpecificationWordSpanishAmbiguous &&
							hasGeneralizationWordFeminineProperNameEnding &&
							( feminineCollectionWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL )
								secondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, feminineCollectionWordItem );

							if( secondarySpecificationItem == NULL )
								{
								// Try to find self-generated conclusion first
								if( ( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL ) ) == NULL )
									// Otherwise, try to find self-generated assumption
									tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );

								if( tempSpecificationItem != NULL &&

								( existingSpecificationItem == NULL ||
								tempSpecificationItem->isSelfGeneratedConclusion() ||
								// Skip assumption with relation word
								existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == NULL ) )
									secondarySpecificationItem = tempSpecificationItem;
								}

							if( secondarySpecificationItem == NULL )
								{
								if( hasGeneralizationWordFeminineProperNameEnding &&
								!isUserGeneralizationWord &&
								simpleUserSpecificationItem != NULL &&
								simpleUserSpecificationItem->isSpecificationWordSpanishAmbiguous() &&
								simpleUserSpecificationItem->isPossessive() &&
								( userSpecificationWordItem = simpleUserSpecificationItem->specificationWordItem() ) != NULL &&
								( tempSpecificationItem = userSpecificationWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != NULL )
									{
									primarySpecificationItem = simpleUserSpecificationItem;
									secondarySpecificationItem = tempSpecificationItem;
									}
								}
							else
								{
								// Typically for the Spanish language
								if( isSpecificationWordSpanishAmbiguous &&
								secondarySpecificationItem->isHiddenSpanishSpecification() &&
								( secondarySpecificationItem = alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, false, true, false, true, simpleUserSpecificationItem, generalizationWordItem, specificationWordItem ) ) != NULL )
									{
									if( secondarySpecificationItem->isQuestion() )
										spanishQuestionPrimarySpecificationItem_ = secondarySpecificationItem;
									else
										{
										if( !secondarySpecificationItem->hasAssumptionLevel() &&
										secondarySpecificationItem->isSelfGeneratedAssumption() )
											secondarySpecificationItem->incrementAssumptionLevel();
										}
									}
								}
							}

						if( secondarySpecificationItem != NULL &&

						( !isSpecificationWordSpanishAmbiguous ||
						existingSpecificationItem == NULL ||
						!secondarySpecificationItem->isHiddenSpanishSpecification() ) )
							{
							if( primarySpecificationItem->isSpecificationGeneralization() )
								{
								if( isGeneralizationProperName &&
								primarySpecificationItem->isPartOf() &&
								// Skip creating an assumption, if a conclusion exists
								generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem, NULL ) == NULL )
									{
									// Make a substitution 'part of' assumption
									// Not yet explained in the design
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationItem, alternativeAnotherPrimarySpecificationItem_, primarySpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									if( ( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != NULL &&
									( specificationSubstitutionPartOfAssumptionJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, primarySpecificationItem ) ) != NULL )
										{
										primaryPartOfSpecificationItem = specificationSubstitutionPartOfAssumptionJustificationItem->primarySpecificationItem();

										if( hasGeneralizationWordConfirmedSpecification ||
										specificationSubstitutionPartOfAssumptionJustificationItem->hasPrimaryAnsweredQuestion() ||

										( primaryPartOfSpecificationItem != NULL &&
										primaryPartOfSpecificationItem != secondarySpecificationItem &&
										secondarySpecificationItem->assumptionLevel() < primaryPartOfSpecificationItem->assumptionLevel() ) )
											{
											if( generalizationWordItem->updateJustificationInSpecifications( false, specificationSubstitutionPartOfAssumptionJustificationItem, createdJustificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update a justification item by a conclusion justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( !specificationSubstitutionPartOfAssumptionJustificationItem->isReplacedOrDeletedItem() &&
											!generalizationWordItem->isJustificationInUse( specificationSubstitutionPartOfAssumptionJustificationItem ) &&

											generalizationWordItem->replaceOrDeleteJustification( specificationSubstitutionPartOfAssumptionJustificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the specification substitution 'part of' assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								}
							else
								{
								if( primarySpecificationItem->isExclusiveSpecification() &&

								( !isGeneralizationWordSpanishAmbiguous ||
								secondarySpecificationItem->isExclusiveSpecification() ) &&

								( !primarySpecificationItem->hasSpecificationCompoundCollection() ||

								// Opposite specification doesn't exist
								( ( collectionWordItem = specificationWordItem->collectionWordItem( primarySpecificationItem->specificationCollectionNr() ) ) != NULL &&
								generalizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, collectionWordItem ) == NULL ) ) )
									{
									// Make generalization assumption
									if( makeGeneralizationAssumption( hasCorrectedAssumptionByKnowledge, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

									generalizationAssumptionWordItem_ = currentGeneralizationWordItem;
									}
								}
							}
						}
					}
				}
			while( !commonVariables_->hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		// Definition specification
		if( generalizationWordTypeNr != WORD_TYPE_PROPER_NAME &&
		generalizationAssumptionWordItem_ != NULL &&
		generalizationWordItem != generalizationAssumptionWordItem_ &&
		specificationWordItem != generalizationAssumptionWordItem_ &&
		( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
		( secondarySpecificationItem = specificationWordItem->firstAssignmentOrSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != NULL &&
		// User specification already exists
		generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) == NULL &&

		// Make an exclusive specification substitution assumption
		// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
		adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, primarySpecificationItem->isEveryGeneralization(), false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, generalizationAssumptionWordItem_, NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		return RESULT_OK;
		}

	SpecificationItem *alternativeJustificationSpecificationItem( bool hasGeneralizationWordFeminineProperNameEnding, bool isCheckingForPossessiveSpecification, bool isCheckingForQuestion, bool isNegative, bool isPossessive, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *oppositeSpecificationItem;
		SpecificationItem *possessiveSpecificationItem;
		SpecificationItem *questionSpecificationItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;

		alternativeAnotherPrimarySpecificationItem_ = NULL;

		if( userSpecificationItem == NULL ||
		generalizationWordItem == NULL ||
		specificationWordItem == NULL )
			return NULL;

		if( generalizationWordItem->isUserRelationWord &&
		userSpecificationItem->isPossessive() )
			return userSpecificationItem;

		if( !hasGeneralizationWordFeminineProperNameEnding &&
		( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL )
			{
			// Try to find opposite specification
			oppositeSpecificationItem = userGeneralizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

			if( oppositeSpecificationItem != NULL &&
			!oppositeSpecificationItem->isHiddenSpanishSpecification() )
				return oppositeSpecificationItem;

			if( isCheckingForPossessiveSpecification &&
			( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
				{
				possessiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, true, NO_QUESTION_PARAMETER, userSpecificationWordItem );

				// Try to find a possessive specification
				if( possessiveSpecificationItem != NULL &&
				!possessiveSpecificationItem->isHiddenSpanishSpecification() )
					{
					alternativeAnotherPrimarySpecificationItem_ = oppositePossessiveDefinitionSpecificationItem_;
					return possessiveSpecificationItem;
					}
				}

			if( isCheckingForQuestion &&
			// Try to find question
			( questionSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, false, WORD_PARAMETER_SINGULAR_VERB_IS, specificationWordItem ) ) != NULL )
				return questionSpecificationItem;
			}

		return NULL;
		}

	WordItem *collectionWordItemInCollectionWords( unsigned int compoundCollectionNr, WordItem *commonWordItem )
		{
		WordItem *currentCollectionWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL &&
		( currentCollectionWordItem = commonVariables_->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem != commonWordItem &&
				currentCollectionWordItem->hasCollectionNr( compoundCollectionNr, commonWordItem ) )
					return currentCollectionWordItem;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return NULL;
		}

	FindSpecificationResultType findPossessiveSpecifications( WordItem *generalizationWordItem, WordItem *possessiveNounWordItem, WordItem *specificationWordItem )
		{
		bool hasFoundPossessiveSpecificationWord = true;
		unsigned int possessiveSpecificationCollectionNr;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		SpecificationItem *foundPossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *previousPossessiveSpecificationItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		WordItem *previousPossessiveSpecificationWordItem;
		WordItem *higherLevelPossessiveSpecificationWordItem = NULL;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossessiveSpecifications";

		if( generalizationWordItem == NULL )
			return adminItem_->startFindSpecificationResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( possessiveNounWordItem == NULL )
			return adminItem_->startFindSpecificationResultError( functionNameString, moduleNameString_, "The given possessive noun word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startFindSpecificationResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// First try to find a possessive specification on a higher level
		if( ( possessiveDefinitionSpecificationItem = possessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
			{
			do	{
				if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
				possessiveSpecificationWordItem != higherLevelPossessiveSpecificationWordItem &&
				possessiveSpecificationWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, specificationWordItem ) != NULL )
					{
					if( higherLevelPossessiveSpecificationWordItem == NULL )
						higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
					else
						hasFoundPossessiveSpecificationWord = false;
					}
				}
			while( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );

			if( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
				{
				if( higherLevelPossessiveSpecificationWordItem == NULL )
					higherLevelPossessiveSpecificationWordItem = specificationWordItem;

				// Try to find the opposite of the possessive specification
				do	{
					if( !possessiveDefinitionSpecificationItem->isPartOf() &&
					( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( possessiveSpecificationWordItem == generalizationWordItem )
							return adminItem_->startFindSpecificationResultError( functionNameString, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );

						if( previousPossessiveSpecificationItem != NULL &&
						( previousPossessiveSpecificationWordItem = previousPossessiveSpecificationItem->specificationWordItem() ) != NULL )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem->specificationCollectionNr();

							if( previousPossessiveSpecificationWordItem != possessiveSpecificationWordItem &&
							higherLevelPossessiveSpecificationWordItem->hasCollectionNr( possessiveSpecificationCollectionNr ) &&
							previousPossessiveSpecificationItem->specificationCollectionNr() == possessiveSpecificationCollectionNr )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
							}

						previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
						}
					}
				while( foundPossessiveDefinitionSpecificationItem == NULL &&
				foundOppositePossessiveDefinitionSpecificationItem == NULL &&
				( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );
				}
			}

		findSpecificationResult.foundPossessiveDefinitionSpecificationItem = foundPossessiveDefinitionSpecificationItem;
		findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem = foundOppositePossessiveDefinitionSpecificationItem;
		return findSpecificationResult;
		}


	protected:
	// Constructor

	AdminReasoningOld( AdminItem *adminItem, CommonVariables *commonVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasSuggestiveQuestionAssumption_ = false;

		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		alternativeAnotherPrimarySpecificationItem_ = NULL;
		possessiveDefinitionSpecificationItem_ = NULL;
		oppositePossessiveDefinitionSpecificationItem_ = NULL;
		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
		spanishQuestionPrimarySpecificationItem_ = NULL;

		feminineSpecificationWordItem_ = NULL;
		generalizationAssumptionWordItem_ = NULL;
		oppositePossessiveSpecificationWordItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminReasoningOld" );

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( commonVariables != NULL )
				commonVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( commonVariables_ = commonVariables ) == NULL )
				strcpy( errorString, "The given common variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeAdminReasoningOldVariables()
		{
		clearPostponedNegativeConclusionVariables();

		hasSuggestiveQuestionAssumption_ = false;

		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
		spanishQuestionPrimarySpecificationItem_ = NULL;

		feminineSpecificationWordItem_ = NULL;
		generalizationAssumptionWordItem_ = NULL;
		}

	signed char checkForUnprocessedNegativeConclusion()
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForUnprocessedNegativeConclusion";

		if( postponeNegativeConclusionSpecificationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( errorString, "\nI found an unprocessed negative conclusion: postponeNegativeConclusionSpecificationCollectionNr_: %u;\n\tpostponeNegativeConclusionPrimarySpecificationItem_: %s;\n\tpostponeNegativeConclusionAnotherPrimarySpecificationItem_: %s.\n", postponeNegativeConclusionSpecificationCollectionNr_, ( postponeNegativeConclusionPrimarySpecificationItem_ == NULL ? EMPTY_STRING : postponeNegativeConclusionPrimarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) ), ( postponeNegativeConclusionAnotherPrimarySpecificationItem_ == NULL ? EMPTY_STRING : postponeNegativeConclusionAnotherPrimarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) ) );

			if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char drawNegativeConclusionsFromAnsweredQuestions( bool hasDisplayedMoreSpecificNonExclusiveSpecification, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		JustificationItem *currentJustificationItem;
		SpecificationItem *answeredQuestionSpecificationItem;
		WordItem *answeredQuestionSpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusionsFromAnsweredQuestions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( answeredQuestionSpecificationItem = generalizationWordItem->firstRecentlyAnsweredQuestionSpecificationItem( WORD_PARAMETER_SINGULAR_VERB_IS ) ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any answered question specification" );

		do	{
			if( userSpecificationWordItem->hasCollectionNr( answeredQuestionSpecificationItem->specificationCollectionNr() ) )
				{
				if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no specification word" );

				// Check if already exists as positive specification
				if( hasDisplayedMoreSpecificNonExclusiveSpecification ||
				generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == NULL )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem->firstJustificationItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem->hasPrimaryQuestion() &&
						!currentJustificationItem->hasPossessivePrimarySpecification() &&

						// Draw negative conclusion from answered question
						drawNegativeConclusion( answeredQuestionSpecificationItem->isArchivedAssignment(), false, answeredQuestionSpecificationItem->generalizationWordTypeNr(), answeredQuestionSpecificationItem->specificationWordTypeNr(), answeredQuestionSpecificationItem->generalizationContextNr(), answeredQuestionSpecificationItem->specificationContextNr(), currentJustificationItem->primarySpecificationItem(), currentJustificationItem->secondarySpecificationItem(), userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
					}
				}
			}
		while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem->recentlyAnsweredQuestionSpecificationItem( false ) ) != NULL );

		return RESULT_OK;
		}

	signed char drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem )
		{
		bool hasFoundAssignment;
		bool hasMoreOptionsLeft = false;
		SpecificationItem *currentAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstNegativeOnlyOptionLeftAssignmentItem = NULL;
		SpecificationItem *firstOnlyOptionLeftSpecificationItem = NULL;
		SpecificationItem *foundOnlyOptionLeftSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawOnlyOptionLeftConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationCollectionNr <= NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification collection number is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

			do	{
				if( currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
				( currentAssignmentItem = generalizationWordItem->firstSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != NULL )
					{
					hasFoundAssignment = false;
					specificationWordItem = currentSpecificationItem->specificationWordItem();

					if( currentAssignmentItem->isNegative() &&
					firstNegativeOnlyOptionLeftAssignmentItem == NULL )
						firstNegativeOnlyOptionLeftAssignmentItem = currentAssignmentItem;

					do	{
						if( currentAssignmentItem->isNegative() &&
						currentAssignmentItem->specificationCollectionNr() == specificationCollectionNr &&
						currentAssignmentItem->specificationWordItem() == specificationWordItem )
							hasFoundAssignment = true;
						}
					while( !hasFoundAssignment &&
					( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );

					if( !hasFoundAssignment )
						{
						if( foundOnlyOptionLeftSpecificationItem == NULL )
							// Found the first option left
							foundOnlyOptionLeftSpecificationItem = currentSpecificationItem;
						else
							{
							hasMoreOptionsLeft = true;
							// Clear the found option
							foundOnlyOptionLeftSpecificationItem = NULL;
							}
						}
					}
				}
			while( !hasMoreOptionsLeft &&
			( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

			if( firstNegativeOnlyOptionLeftAssignmentItem != NULL &&
			foundOnlyOptionLeftSpecificationItem != NULL )
				{
				// Draw an only option left conclusion
				// Not yet explained in the design
				if( adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *feminineSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion" );

		if( isPossessive &&
		!isArchivedAssignment &&
		specificationWordItem->isNounWordSpanishAmbiguous() &&
		relationWordItem->hasFeminineProperNameEnding() &&
		( feminineSpecificationWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL )
			{
			if( drawPossessiveReversibleConclusion( false, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, feminineSpecificationWordItem, relationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion" );
			}

		return RESULT_OK;
		}

	signed char drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned int nonCompoundCollectionNr;
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem();
		WordItem *collectionWordItem;
		WordItem *commonWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSimpleNegativeSpanishConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( simpleUserSpecificationItem != NULL &&
		!specificationWordItem->isNounWordSpanishAmbiguous() &&
		( userSpecificationWordItem = simpleUserSpecificationItem->specificationWordItem() ) != NULL &&
		( nonCompoundCollectionNr = specificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
			{
			if( !userSpecificationWordItem->isNounWordSpanishAmbiguous() &&
			( collectionWordItem = specificationWordItem->collectionWordItem( nonCompoundCollectionNr ) ) != NULL )
				{
				if( collectionWordItem->isNounWordSpanishAmbiguous() )
					anotherPrimarySpecificationItem = collectionWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, collectionWordItem );
				else
					{
					userSpecificationCollectionNr = simpleUserSpecificationItem->specificationCollectionNr();

					if( userSpecificationCollectionNr > NO_COLLECTION_NR &&
					( commonWordItem = userSpecificationWordItem->commonWordItem( userSpecificationCollectionNr ) ) != NULL )
						{
						if( commonWordItem->isNounWordSpanishAmbiguous() )
							anotherPrimarySpecificationItem = commonWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, false, false, true, NO_QUESTION_PARAMETER, nonCompoundCollectionNr, NO_CONTEXT_NR, collectionWordItem );
						}
					}

				if( anotherPrimarySpecificationItem != NULL &&
				anotherPrimarySpecificationItem->hasSpecificationCollection() &&

				askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isPossessive, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned short assumptionLevel;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
					{
					if( !currentSpecificationItem->hasSpecificationCollection() &&
					!currentSpecificationItem->isExclusiveSpecification() )
						{
						assumptionLevel = ( secondarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ? currentSpecificationItem->assumptionLevel() : secondarySpecificationItem->assumptionLevel() );

						// Collect generalization words of a specification-generalization definition
						if( adminItem_->collectGeneralizationWordWithPreviousOne( false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

						if( currentSpecificationItem->hasCurrentCreationSentenceNr() &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );

						if( !isSelfGenerated &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isMakingPartOfAssumption, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool hasCurrentSpecificationCollection;
		bool hasCurrentSpecificationCompoundCollection;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordFeminineProperNameEnding;
		bool hasGeneralizationWordMasculineProperNameEnding;
		bool hasPrimarySpecificationNonCompoundCollection;
		bool isDefinitionSpanishAmbiguous;
		bool isExclusiveCurrentSpecification;
		bool isFemaleOrMaleGeneralizationWord;
		bool isFoundPrimaryHiddenSpanishSpecification;
		bool isGeneralizationWordUserRelationWord;
		bool isIncludingRelationWord;
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		bool isUserSpecificationWord;
		bool hasUserSpecificationRelationContext = false;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		bool isExclusiveUserSpecification = false;
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		bool isIncludingAdjectives = false;
		bool isNegativeConclusion = false;
		bool isPossessiveUserSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned short currentSpecificationWordTypeNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int relationContextNr;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *alternativeAnotherPrimarySpecificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *archivedAssignmentItem;
		SpecificationItem *bestMatchingRelationSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *collectedWithItselfSpecificationItem = NULL;
		WordItem *currentSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( specificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
		!adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a specification reasoning word type" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( isMakingPartOfAssumption &&
		makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
			{
			hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			isIncludingAdjectives = ( !isExclusiveSpecification &&
									!isQuestion &&
									!hasCorrectedAssumptionByKnowledge &&
									!userSpecificationItem->isQuestion() );
			}

		if( ( currentSpecificationItem = specificationWordItem->firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != NULL )
			{
			hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();

			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem->hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem->hasFeminineProperNameEnding();
			hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem->hasMasculineProperNameEnding();

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			isFemaleOrMaleGeneralizationWord = generalizationWordItem->isFemaleOrMale();
			isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isUserSpecificationWord = specificationWordItem->isUserSpecificationWord;

			if( isSpecificationWordSpanishAmbiguous )
				collectedWithItselfSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem );

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem )
					{
					isExclusiveCurrentSpecification = currentSpecificationItem->isExclusiveSpecification();
					currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();
					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > NO_COLLECTION_NR );

					if( ( bestMatchingRelationSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, isAssignment, isInactiveAssignment, isArchivedAssignment, false, false, questionParameter, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) ) != NULL )
						{
						if( !isQuestion &&
						!bestMatchingRelationSpecificationItem->isExclusiveSpecification() )
							isNegativeConclusion = true;
						}

					if( !hasRelationWord ||
					!isPossessiveUserSpecification ||
					( foundPrimarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, questionParameter, specificationWordItem, relationWordItem ) ) == NULL )
						foundPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( ( isAssignment || hasCorrectedAssumptionByOppositeQuestion ), isArchivedAssignment, false, false, questionParameter, specificationWordItem );

					if( foundPrimarySpecificationItem == NULL )
						{
						// Check for specification conflict
						if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, false, false, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a specification conflict in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem->isHiddenSpanishSpecification();

						if( !commonVariables_->hasDisplayedWarning &&
						isExclusiveCurrentSpecification &&
						( feminineOrMasculineSpecificationItem = currentSpecificationWordItem->firstFeminineOrMasculineSpecificationItem() ) != NULL )
							{
							isIncludingRelationWord = ( hasRelationWord &&
														feminineOrMasculineSpecificationItem->hasSpecificationCollection() &&

														( isFoundPrimaryHiddenSpanishSpecification ||
														foundPrimarySpecificationItem->hasNewInformation() ) );

							if( ( isIncludingRelationWord ||

							( !hasRelationWord &&
							!isPossessiveUserSpecification ) ||

							( isPossessiveUserSpecification &&

							( !isUserGeneralizationWord ||
							foundPrimarySpecificationItem->isOlderItem() ) ) ) &&

							// Matching feminine word
							( ( ( hasGeneralizationWordFeminineProperNameEnding &&
							currentSpecificationWordItem->isFemale() ) ||

							// Matching masculine word
							( hasGeneralizationWordMasculineProperNameEnding &&
							currentSpecificationWordItem->isMale() ) ) &&

							( ( isFoundPrimaryHiddenSpanishSpecification &&
							isUserGeneralizationWord ) ||

							( !isFoundPrimaryHiddenSpanishSpecification &&

							( ( !isUserSpecificationWordSpanishAmbiguous &&
							!hasGeneralizationWordActiveQuestionWithCompoundCollection ) ||

							( !isPossessiveUserSpecification &&
							foundPrimarySpecificationItem->hasRelationContext() ) ) ) ) &&

							// Select correct compound word to avoid assumption to be made if a question should be asked
							currentSpecificationWordItem->compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem ) )
								{
								if( !isFemaleOrMaleGeneralizationWord )
									{
									if( isIncludingRelationWord ||

									( isUserGeneralizationWord &&

									( !hasUserSpecificationRelationContext ||
									foundPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ) ) ||

									( ( hasRelationWord ||
									generalizationWordItem->isOlderItem() ) &&

									oppositePossessiveDefinitionSpecificationItem_ != NULL &&
									oppositePossessiveDefinitionSpecificationItem_->specificationCollectionNr() == userSpecificationCollectionNr ) )
										{
										// Make a feminine or masculine proper name ending assumption
										// Not yet explained in the design
										if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem_ : NULL ), currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : NULL ) ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

										if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
											{
											if( !feminineOrMasculineSpecificationItem->hasSpecificationCompoundCollection() &&
											// Clean-up obsolete justifications
											generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											if( !isUserSpecificationWordSpanishAmbiguous &&

											( postponeNegativeConclusionSpecificationCollectionNr_ == NO_COLLECTION_NR ||
											postponeNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr ) )
												{
												if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
													{
													firstJustificationItem = foundSpecificationItem->firstJustificationItem();

													if( firstJustificationItem != NULL &&
													( secondarySpecificationItem = firstJustificationItem->secondarySpecificationItem() ) != NULL )
														{
														// Additional justification
														if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, secondarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
															return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

														if( createdSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
														}
													}

												// Draw missing negative conclusions
												if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, false, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									else
										clearPostponedNegativeConclusionVariables();
									}

								if( !isArchivedAssignment &&
								isIncludingRelationWord &&
								bestMatchingRelationSpecificationItem != NULL &&
								!foundPrimarySpecificationItem->isHiddenSpanishSpecification() &&
								( relationContextNr = bestMatchingRelationSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&

								drawPossessiveReversibleConclusion( false, false, false, false, NO_ASSUMPTION_LEVEL, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
								}
							}

						if( ( bestMatchingRelationSpecificationItem == NULL &&

						( hasCurrentSpecificationCollection ||
						!foundPrimarySpecificationItem->hasSpecificationCollection() ) ) ||

						( !isExclusiveCurrentSpecification &&

						( isGeneralizationProperName ||

						( hasCurrentSpecificationCollection &&
						isDefinitionSpanishAmbiguous ) ) ) ||

						( isDefinitionSpanishAmbiguous &&
						hasCurrentSpecificationCollection &&
						isExclusiveCurrentSpecification &&
						!isUserSpecificationWord &&
						!currentSpecificationItem->isOlderItem() ) )
							{
							hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem->hasSpecificationNonCompoundCollection();

							if( ( !isExclusiveSpecification &&
							isExclusiveCurrentSpecification ) ||

							( isGeneralizationProperName &&

							( ( isExclusiveSpecification &&
							hasCurrentSpecificationCollection ) ||

							( isSpecificationWordSpanishAmbiguous &&
							!isFoundPrimaryHiddenSpanishSpecification &&

							( ( isExclusiveSpecification &&

							( isGeneralizationWordUserRelationWord ||
							isPossessiveUserSpecification ) ) ||

							( hasPrimarySpecificationNonCompoundCollection &&
							isGeneralizationWordUserRelationWord &&
							isPossessiveUserSpecification ) ) ) ) ) )
								{
								hasCurrentSpecificationCompoundCollection = ( currentSpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) > NO_COLLECTION_NR );

								// Ask a question or draw a negative conclusion
								if( isGeneralizationProperName &&
								hasCurrentSpecificationCollection &&
								userSpecificationItem != NULL &&

								// Specification isn't Spanish ambiguous
								( ( !isSpecificationWordSpanishAmbiguous &&

								( !hasCurrentSpecificationCompoundCollection ||
								!hasGeneralizationWordActiveQuestionWithCompoundCollection ||
								isExclusiveUserSpecification ||
								isUserSpecificationWordSpanishAmbiguous ||
								foundPrimarySpecificationItem->hasSpecificationCollection() ) ) ||

								// Specification is Spanish ambiguous
								( isSpecificationWordSpanishAmbiguous &&

								( hasPrimarySpecificationNonCompoundCollection ||
								!isExclusiveCurrentSpecification ||
								isNegativeConclusion ||

								( isUserGeneralizationWord &&
								!isExclusiveUserSpecification &&
								currentSpecificationItem->hasSpecificationNonCompoundCollection() ) ) ) ||

								// Question
								hasCorrectedAssumptionByOppositeQuestion ||
								hasSuggestiveQuestionAssumption_ ) )
									{
									if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							else
								{
								currentSpecificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();

								// Make an assumption or draw a conclusion
								if( ( isAssumption ||
								currentSpecificationItem->hasAssumptionLevel() ) &&

								currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE )
									{
									alternativeAnotherPrimarySpecificationItem = NULL;
									primarySpecificationItem = NULL;

									if( isSpecificationWordSpanishAmbiguous )
										primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem );
									else
										{
										primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL );

										if( primarySpecificationItem == NULL ||
										primarySpecificationItem ->isUserAssignment() )
											primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );
										}

									existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem );

									if( primarySpecificationItem != NULL )
										{
										alternativePrimarySpecificationItem = ( hasRelationWord || isSpecificationWordSpanishAmbiguous ? alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) : NULL );

										if( ( isSpecificationWordSpanishAmbiguous &&
										primarySpecificationItem->isHiddenSpanishSpecification() ) ||

										( existingSpecificationItem != NULL &&
										alternativePrimarySpecificationItem != NULL &&
										alternativePrimarySpecificationItem->specificationWordItem() == specificationWordItem &&

										( ( isSpecificationWordSpanishAmbiguous &&
										existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ) ||

										( !isSpecificationWordSpanishAmbiguous &&
										!existingSpecificationItem->hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&

										( isUserGeneralizationWord ||
										!specificationWordItem->isCollectionSpanishAmbiguous( alternativePrimarySpecificationItem->specificationCollectionNr() ) ) ) ) ) )
											{
											if( alternativePrimarySpecificationItem != NULL )
												alternativeAnotherPrimarySpecificationItem = alternativeAnotherPrimarySpecificationItem_;

											if( isSpecificationWordSpanishAmbiguous &&
											isGeneralizationWordUserRelationWord &&
											alternativeAnotherPrimarySpecificationItem != NULL &&
											existingSpecificationItem != NULL &&
											userSpecificationItem != NULL &&
											primarySpecificationItem->isHiddenSpanishSpecification() &&
											!userSpecificationItem->isArchivedAssignment() )
												{
												primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, NO_QUESTION_PARAMETER, userSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, userSpecificationItem->specificationWordItem() );

												if( primarySpecificationItem != NULL &&
												// Check if justification already exists
												existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) )
													primarySpecificationItem = alternativePrimarySpecificationItem;
												}
											else
												primarySpecificationItem = alternativePrimarySpecificationItem;
											}
										else
											{
											if( userSpecificationItem != NULL &&
											userSpecificationItem->isPossessive() &&
											primarySpecificationItem->hasAssumptionLevel() )
												{
												existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, currentSpecificationWordItem );

												if( existingSpecificationItem != NULL &&
												existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL )
													// Skip if similar justification already exists, without relation word
													primarySpecificationItem = NULL;
												}
											}
										}

									if( primarySpecificationItem != NULL &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||

									( currentSpecificationCollectionNr == NO_COLLECTION_NR &&

									( primarySpecificationItem->isOlderItem() ||
									primarySpecificationItem->isPossessive() ) ) ) &&

									// Skip if already exists as user specification
									( existingSpecificationItem == NULL ||
									existingSpecificationItem->isSelfGenerated() ) )
										{
										archivedAssignmentItem = NULL;

										if( userSpecificationItem != NULL &&
										userSpecificationItem->isArchivedAssignment() &&
										userSpecificationItem->isGeneralizationNoun() )
											{
											isAssignment = true;
											isArchivedAssignment = true;
											archivedAssignmentItem = userSpecificationItem;
											}

										// Make a compound specification substitution assumption, which is actually an uncertain conclusion
										// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
										//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
										if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem, ( archivedAssignmentItem == NULL ? currentSpecificationItem : archivedAssignmentItem ), NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

										if( createAndAssignResult.createdSpecificationItem == NULL &&

										( createAndAssignResult.createdJustificationItem != NULL ||
										!primarySpecificationItem->hasRelationContext() ||
										foundPrimarySpecificationItem->isOlderItem() ) &&

										// Clean-up obsolete justifications
										generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									{
									if( ( !isExclusiveSpecification &&
									bestMatchingRelationSpecificationItem == NULL &&

									( !isSpecificationWordSpanishAmbiguous ||

									( !isFoundPrimaryHiddenSpanishSpecification &&
									collectedWithItselfSpecificationItem != NULL &&
									collectedWithItselfSpecificationItem->hasCurrentCreationSentenceNr() ) ) &&

									// Skip if already exists as user specification
									generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) ||

									// Check if exclusive conclusion already exists
									( isExclusiveSpecification &&
									generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isAssignment, isArchivedAssignment, false, false, true, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) )
										{
										primarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

										if( primarySpecificationItem != NULL &&
										primarySpecificationItem->isHiddenSpanishSpecification() &&
										( primarySpecificationItem = alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) ) != NULL )
											alternativeAnotherPrimarySpecificationItem = alternativeAnotherPrimarySpecificationItem_;

										if( primarySpecificationItem == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );

										if( !primarySpecificationItem->isPartOf() &&

										( isGeneralizationProperName ||

										( primarySpecificationItem->isUserSpecification() &&

										( !isExclusiveSpecification ||
										currentSpecificationWordItem->isFemaleOrMale() ||
										!currentSpecificationItem->hasSpecificationCompoundCollection() ||

										// Typically for the Spanish language
										( isSpanishCurrentLanguage &&

										( generalizationWordItem->isFemaleOrMale() ||
										currentSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, generalizationWordItem ) != NULL ) ) ) ) ) )
											{
											// Draw a compound specification substitution conclusion
											// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
											//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, primarySpecificationItem->isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, ( currentSpecificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? WORD_TYPE_NOUN_SINGULAR : currentSpecificationWordTypeNr ), NO_WORD_TYPE_NR, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
												{
												if( hasCorrectedAssumptionByKnowledge )
													{
													if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created specification" );

													if( relatedResult.relatedSpecificationItem != NULL &&
													// Replace incorrect assumption
													generalizationWordItem->replaceOrDeleteSpecification( false, relatedResult.relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													if( isGeneralizationProperName )
														{
														if( isSpanishCurrentLanguage &&
														!hasRelationWord &&
														hasCurrentSpecificationCollection &&
														!isExclusiveSpecification &&
														!isSpecificationWordSpanishAmbiguous &&
														isUserGeneralizationWord &&
														primarySpecificationItem->hasSpecificationNonCompoundCollection() &&

														// Draw simple negative Spanish conclusions
														drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														if( makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										}
									}
								}

							if( !isExclusiveCurrentSpecification &&
							isGeneralizationProperName &&
							hasCurrentSpecificationCollection &&

							makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption by generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFeminineOrMasculineProperNameEnding;
		bool hasGeneralizationWordConfirmedSpecification;
		bool isGeneralizationWordUserRelationWord;
		bool isHiddenPrimarySpecification;
		bool isHiddenSecondarySpecification;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isReplaceHiddenSpecification;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasRelationWordConfirmedSpecification = false;
		bool hasRelationWordFeminineOrMasculineProperNameEnding = false;
		unsigned int compoundCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *olderJustificationItem;
		SpecificationItem *additionalSecondarySpecificationItem;
		SpecificationItem *additionalSpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *compoundSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem;
		SpecificationItem *foundPossessiveDefinitionSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *possessiveSpecificationItem = NULL;
		SpecificationItem *secondarySpecificationItem = NULL;
		WordItem *commonWordItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *feminineSpecificationWordItem;
		WordItem *possessiveSpecificationWordItem;
		WordItem *spanishQuestionGeneralizationWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		FindSpecificationResultType findSpecificationResult;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		possessiveDefinitionSpecificationItem_ = NULL;
		oppositePossessiveDefinitionSpecificationItem_ = NULL;
		oppositePossessiveSpecificationWordItem_ = NULL;

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

		if( primarySpecificationItem != NULL &&

		( hasRelationWord ||

		( !primarySpecificationItem->isOlderItem() &&

		( !isPossessive ||
		!primarySpecificationItem->isExclusiveSpecification() ) ) ) &&

		( currentPossessiveNounWordItem = commonVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			hasGeneralizationWordConfirmedSpecification = generalizationWordItem->hasConfirmedSpecification();
			isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
			isHiddenPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();

			if( relationWordItem != NULL )
				{
				hasRelationWordConfirmedSpecification = relationWordItem->hasConfirmedSpecification();
				hasRelationWordFeminineOrMasculineProperNameEnding = relationWordItem->hasFeminineOrMasculineProperNameEnding();
				}

			// Do for all possessive noun words
			do	{
				if( currentPossessiveNounWordItem != specificationWordItem )
					{
					if( ( findSpecificationResult = findPossessiveSpecifications( generalizationWordItem, currentPossessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in word \"", currentPossessiveNounWordItem->anyWordTypeString(), "\"" );

					isReplaceHiddenSpecification = false;
					anotherPrimarySpecificationItem = NULL;
					createdOrFoundSpecificationItem = NULL;
					foundOppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem;
					foundPossessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem;

					if( foundPossessiveDefinitionSpecificationItem != NULL &&
					( possessiveDefinitionSpecificationWordItem = foundPossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						oppositePossessiveDefinitionSpecificationItem_ = foundOppositePossessiveDefinitionSpecificationItem;
						possessiveDefinitionSpecificationItem_ = foundPossessiveDefinitionSpecificationItem;

						if( ( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem->firstSpecificationItem( isPossessive, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
							{
							isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();

							if( primarySpecificationItem != NULL &&

							( ( !primarySpecificationItem->hasRelationContext() &&
							generalizationWordItem->hasAnsweredSelfGeneratedQuestion() ) ||

							( isPossessiveSpecificationWordSpanishAmbiguous &&
							primarySpecificationItem->specificationWordItem() == possessiveDefinitionSpecificationWordItem ) ) )
								{
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, false, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								existingUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem );

								if( ( existingUserSpecificationItem == NULL &&

								( !isPossessiveSpecificationWordSpanishAmbiguous ||
								!isNegative ) ) ||

								( hasRelationWord &&
								existingUserSpecificationItem != NULL &&

								// Confirmed specification without relation word
								( !existingUserSpecificationItem->hasRelationContext() ||
								!existingUserSpecificationItem->isOlderItem() ||
								// Confirmed only a few of the entered relation words
								adminItem_->nContextWordsInContextWords( existingUserSpecificationItem->relationContextNr(), possessiveDefinitionSpecificationWordItem ) < commonVariables_->nUserRelationWords ) ) )
									{
									hasFeminineOrMasculineProperNameEnding = false;

									if( isPossessiveSpecificationWordSpanishAmbiguous &&
									isFeminineOrMasculineProperNameEnding( isPossessive, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) )
										hasFeminineOrMasculineProperNameEnding = true;

									if( !hasRelationWord &&
									!isNegative &&
									( possessiveSpecificationWordItem = possessiveSpecificationItem->specificationWordItem() ) != NULL )
										{
										commonWordItem = possessiveSpecificationWordItem->commonWordItem( possessiveSpecificationItem->specificationCollectionNr() );

										if( commonWordItem != NULL &&
										commonWordItem != generalizationWordItem &&
										commonWordItem->isFemaleOrMale() &&
										!commonWordItem->isNounWordSpanishAmbiguous() &&
										// Specification doesn't exist yet
										generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, commonWordItem ) == NULL &&
										( definitionSpecificationItem = commonWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, possessiveSpecificationWordItem ) ) != NULL )
											{
											// Make generalization assumption
											if( makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, NO_CONTEXT_NR, definitionSpecificationItem, primarySpecificationItem, generalizationWordItem, commonWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", commonWordItem->anyWordTypeString(), "\"" );
											}
										}

									// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), foundPossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a normal exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

									createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
									foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

									if( ( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? foundSpecificationItem : createdSpecificationItem ) ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "No specification item was created or found" );

									secondarySpecificationItem = createdOrFoundSpecificationItem->updatedSpecificationItem();

									if( createdSpecificationItem != NULL )
										{
										if( isPossessiveSpecificationWordSpanishAmbiguous )
											{
											isReplaceHiddenSpecification = true;

											if( spanishAmbiguousCompoundPrimarySpecificationItem_ != NULL )
												primarySpecificationItem = spanishAmbiguousCompoundPrimarySpecificationItem_;

											if( spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ != NULL )
												anotherPrimarySpecificationItem = spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
											}
										else
											{
											if( hasRelationWord &&
											foundSpecificationItem != NULL &&
											( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != NULL )
												{
												if( foundSpecificationItem->isSelfGeneratedAssumption() &&
												( obsoleteJustificationItem = generalizationWordItem->olderComplexJustificationItem( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_COLLECTION_NR, NULL ) ) != NULL )
													{
													if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the obsolete justification item by the created justification item" );

													if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "The found specification item has no first justification item" );

													// Attach all justification items of the found specification to the created specification
													if( createdSpecificationItem->changeFirstJustification( firstJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification item of the created specification, attaching all justification items of the found specification" );

													if( generalizationWordItem->replaceOrDeleteSpecification( false, foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													if( foundSpecificationItem->isSelfGeneratedConclusion() &&
													( firstJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != NULL )
														{
														if( ( additionalSecondarySpecificationItem = firstJustificationItem->secondarySpecificationItem() ) != NULL )
															{
															// Additional justification
															if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, additionalSecondarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

															if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
																return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

															if( createdSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
															}
														else
															return adminItem_->startError( functionNameString, moduleNameString_, "The additional secondary specification item is undefined" );
														}
													}
												}
											}
										}
									}
								}
							}
						}

					// First opposite possessive specification
					if( !commonVariables_->hasDisplayedWarning &&
					possessiveDefinitionSpecificationWordItem != NULL &&
					foundOppositePossessiveDefinitionSpecificationItem != NULL &&
					( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( isNegative ||
						specificationContextNr > NO_CONTEXT_NR ||
						createdOrFoundSpecificationItem == NULL )
							{
							secondarySpecificationItem = primarySpecificationItem;

							// Find another primary definition
							if( !hasRelationWord ||
							isPossessive )
								anotherPrimarySpecificationItem = possessiveDefinitionSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );
							}

						if( !isHiddenPrimarySpecification &&
						primarySpecificationItem != NULL &&
						secondarySpecificationItem != NULL &&

						( !primarySpecificationItem->isOlderItem() ||
						primarySpecificationItem->isSelfGenerated() ) )
							{
							isHiddenSecondarySpecification = secondarySpecificationItem->isHiddenSpanishSpecification();
							existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, !isPossessive, NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

							if( ( existingSpecificationItem == NULL ||
							!existingSpecificationItem->hasRelationContext() ||

							( ( hasRelationWord &&
							isGeneralizationWordUserRelationWord &&
							isPossessive &&
							existingSpecificationItem->isSelfGeneratedAssumption() &&

							( hasRelationWordConfirmedSpecification ||
							!hasRelationWordFeminineOrMasculineProperNameEnding ||
							anotherPrimarySpecificationItem != NULL ) ) ||

							( !isHiddenPrimarySpecification &&
							secondarySpecificationItem->hasRelationContext() &&
							!secondarySpecificationItem->isSelfGeneratedConclusion() &&

							( !isHiddenSecondarySpecification ||
							// Typically for the Spanish language
							primarySpecificationItem->isAssignment() ||
							primarySpecificationItem->isUserSpecification() ) &&

							( hasGeneralizationWordConfirmedSpecification ||
							!existingSpecificationItem->isOlderItem() ||

							( anotherPrimarySpecificationItem == NULL &&

							( hasGeneralizationWordConfirmedSpecification ||
							primarySpecificationItem == secondarySpecificationItem ||
							existingSpecificationItem->isSelfGeneratedAssumption() ) ) ||

							( relationWordItem != NULL &&
							!relationWordItem->isOlderItem() ) ) ) ) ) &&

							// User specification doesn't exist
							generalizationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == NULL &&
							// No justification exist as an opposite possessive conditional specification assumption
							generalizationWordItem->secondarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == NULL )
								{
								isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_->isNounWordSpanishAmbiguous();

								// Make an exclusive specification substitution assumption (opposite)
								// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), foundOppositePossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( isHiddenSecondarySpecification ? primarySpecificationItem : secondarySpecificationItem ) : foundOppositePossessiveDefinitionSpecificationItem ), ( !isReplaceHiddenSpecification && isOppositePossessiveSpecificationWordSpanishAmbiguous ? NULL : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? foundOppositePossessiveDefinitionSpecificationItem : secondarySpecificationItem ), NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

								if( isPossessive )
									{
									if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
										{
										if( hasRelationWord &&
										primarySpecificationItem->isSelfGenerated() &&
										( anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_->firstAssignmentOrSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) ) != NULL )
											{
											hasFeminineOrMasculineProperNameEnding = false;
											feminineSpecificationWordItem = NULL;

											if( isFeminineOrMasculineProperNameEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) )
												{
												hasFeminineOrMasculineProperNameEnding = true;

												if( feminineSpecificationWordItem_ == NULL )
													{
													if( existingSpecificationItem == NULL )
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_->feminineCollectionWordItem();
													}
												else
													feminineSpecificationWordItem = feminineSpecificationWordItem_;

												if( feminineSpecificationWordItem != NULL &&
												( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, anotherPrimarySpecificationItem, NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
												}

											if( ( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, NO_QUESTION_PARAMETER, anotherPrimarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
											compoundSpecificationItem != primarySpecificationItem )
												{
												// Make a compound specification exclusive specification substitution assumption (opposite)
												// Not yet explained in the design
												if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, isNegative, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, compoundSpecificationItem, ( hasFeminineOrMasculineProperNameEnding ? foundOppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNameEnding ? anotherPrimarySpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

												if( feminineSpecificationWordItem_ != NULL &&
												isFeminineOrMasculineProperNameEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) &&
												( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNameEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNameEnding ? foundOppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNameEnding ? anotherPrimarySpecificationItem : compoundSpecificationItem ), NULL, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine compound exclusive specification substitution assumption that is Spanish ambiguous in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", feminineSpecificationWordItem_->anyWordTypeString(), "\"" );
												}
											}
										}
									else
										{
										if( foundSpecificationItem == NULL )
											secondarySpecificationItem = createdSpecificationItem;
										else
											{
											if( createdSpecificationItem == NULL )
												{
												// Found, but not created
												secondarySpecificationItem = foundSpecificationItem;

												if( anotherPrimarySpecificationItem != NULL &&
												primarySpecificationItem->isUserSpecification() &&
												( additionalSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != NULL )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_ORDER_NR, commonVariables_->currentSentenceNr, anotherPrimarySpecificationItem, NULL, primarySpecificationItem, NULL, additionalSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

													if( additionalSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the found specification" );

													if( existingSpecificationItem != NULL &&
													!existingSpecificationItem->isOlderItem() &&

													// Clean-up obsolete justifications
													generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											else
												{
												// Found and created
												if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "The found specification item has no first justification item" );

												if( foundSpecificationItem->isSelfGeneratedAssumption() )
													{
													// Attach all justification items of the found specification to the created specification
													if( createdSpecificationItem->changeFirstJustification( firstJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification item of the created specification, attaching all justification items of the found specification" );

													if( generalizationWordItem->replaceOrDeleteSpecification( false, foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													// Test file: "Complex (12)"
													// Adjusted context. Needs to be rewritten
													strcpy( primarySpecificationItem->lastWrittenSentenceString, EMPTY_STRING );

													// Replace the secondary specification of a justification
													if( ( obsoleteJustificationItem = primarySpecificationItem->secondarySpecificationJustificationItem( foundSpecificationItem ) ) != NULL )
														{
														if( ( justificationResult = generalizationWordItem->copyJustification( false, obsoleteJustificationItem->primarySpecificationItem(), createdSpecificationItem, obsoleteJustificationItem->attachedJustificationItem(), obsoleteJustificationItem ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy an obsolete justification item with a different secondary specification item" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
															return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't copy an obsolete justification" );

														// Replace obsolete justification
														if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, primarySpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}

													if( ( additionalSecondarySpecificationItem = firstJustificationItem->secondarySpecificationItem() ) != NULL )
														{
														// Additional justification
														if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, additionalSecondarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
															return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

														if( createdSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
														}
													else
														return adminItem_->startError( functionNameString, moduleNameString_, "The additional secondary specification item is undefined" );
													}
												}
											}
										}
									}
								else
									{
									if( createdSpecificationItem == NULL )
										{
										if( foundSpecificationItem == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any specification item" );

										if( foundSpecificationItem->isOlderItem() )
											{
											if( createAndAssignResult.createdJustificationItem != NULL &&
											!foundSpecificationItem->hasCurrentCreationSentenceNr() &&
											!foundSpecificationItem->isReplacedOrDeletedItem() &&

											// Clean-up obsolete justifications
											generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											if( spanishQuestionPrimarySpecificationItem_ == NULL )
												{
												if( relationWordItem != NULL &&
												( additionalSecondarySpecificationItem = relationWordItem->firstUserSpecificationItem( false, isPossessive, primarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NULL ) ) != NULL &&
												additionalSecondarySpecificationItem->hasRelationContext() &&
												!additionalSecondarySpecificationItem->isSpecificationWordSpanishAmbiguous() )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, commonVariables_->currentSentenceNr, NULL, NULL, additionalSecondarySpecificationItem, NULL, foundSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

													if( foundSpecificationItem->changeFirstJustification( createdJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the found specification" );

													if( ( shortResult = foundSpecificationItem->recalculateAssumptionLevel() ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );

													if( shortResult.shortValue == NO_ASSUMPTION_LEVEL &&
													// Force conclusion
													foundSpecificationItem->markAsConcludedAssumption( false ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
													}
												}
											else
												{
												if( foundSpecificationItem->wasHiddenSpanishSpecification() &&
												( spanishQuestionGeneralizationWordItem = spanishQuestionPrimarySpecificationItem_->generalizationWordItem() ) != NULL &&

												// Replace a question in a Spanish justification,
												// by a specification that is not hidden anymore
												spanishQuestionGeneralizationWordItem->updateSpecificationsInJustificationInWord( true, spanishQuestionPrimarySpecificationItem_, foundSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the justification items in generalization word \"", spanishQuestionGeneralizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									else
										{
										if( hasRelationWord &&
										existingSpecificationItem != NULL &&
										existingSpecificationItem->hasRelationContext() &&
										!existingSpecificationItem->isReplacedOrDeletedItem() )
											{
											if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
												{
												if( ( !createdSpecificationItem->isHiddenSpanishSpecification() ||
												existingSpecificationItem->isHiddenSpanishSpecification() ) &&

												( existingSpecificationItem->hasSpecificationNonCompoundCollection() ||

												( existingSpecificationItem->hasSpecificationCompoundCollection() &&
												!existingSpecificationItem->hasOnlyOneRelationWord() ) ) )
													{
													// Created specification was hidden
													if( generalizationWordItem->replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												if( ( olderJustificationItem = existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) == NULL )
													olderJustificationItem = existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

												if( olderJustificationItem != NULL &&
												generalizationWordItem->attachJustificationInWord( olderJustificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach an older justification item to the created assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											else
												{
												if( !generalizationWordItem->hasDisplayedMoreSpecificSpecification() )
													{
													if( generalizationWordItem->replaceOrDeleteSpecification( false, existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( ( firstJustificationItem = existingSpecificationItem->firstJustificationItem() ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "The existing specification item has no first justification item" );

													// Attach all justification items of the existing specification to the created specification
													if( createdSpecificationItem->changeFirstJustification( firstJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification item of the created specification, attaching all justification items of the existing specification" );
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			while( !commonVariables_->hasDisplayedWarning &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );

			if( !commonVariables_->hasDisplayedWarning &&
			primarySpecificationItem != NULL &&
			secondarySpecificationItem != NULL &&
			relationWordItem != NULL )
				{
				if( primarySpecificationItem->isSpecificationWordSpanishAmbiguous() )
					{
					if( primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
					( compoundCollectionNr = specificationWordItem->compoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR &&
					( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, compoundCollectionNr, primarySpecificationItem->relationContextNr(), specificationWordItem ) ) != NULL )
						primarySpecificationItem = compoundSpecificationItem;
					}
				else
					{
					if( existingSpecificationItem != NULL &&
					primarySpecificationItem == secondarySpecificationItem &&
					primarySpecificationItem->hasSpecificationCompoundCollection() &&
					!existingSpecificationItem->isSpecificationWordSpanishAmbiguous() &&
					relationWordItem->isUserGeneralizationWord )
						secondarySpecificationItem = existingSpecificationItem;
					}

				if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, ( isHiddenPrimarySpecification ? adminItem_->userSpecificationItem() : primarySpecificationItem ), generalizationWordItem, relationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem )
		{
		unsigned int currentSpecificationCollectionNr;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *indirectPrimarySpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		WordItem *commonWordItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *negativeQuestionSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( userSpecificationItem->hasSpecificationCollection() &&
		( currentQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != NULL )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL &&
				( currentSpecificationCollectionNr = currentQuestionSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
					{
					if( ( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem->firstActiveAssignmentOrSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
						{
						// Search first for related specification, before add a new one
						if( ( relatedResult = generalizationWordItem->findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem->questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, specificationWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

						negativeQuestionSpecificationWordItem = ( relatedResult.relatedSpecificationItem == NULL ? NULL : relatedResult.relatedSpecificationItem->specificationWordItem() );

						// Draw an indirectly answered question conclusion
						// Not yet explained in the design
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, indirectPrimarySpecificationItem, NULL, userSpecificationItem, NULL, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentQuestionSpecificationWordItem->anyWordTypeString(), "\"" );

						if( negativeQuestionSpecificationWordItem != NULL &&
						( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
						( commonWordItem = currentQuestionSpecificationWordItem->commonWordItem( currentSpecificationCollectionNr ) ) != NULL )
							{
							if( commonWordItem->isNounWordSpanishAmbiguous() )
								{
								if( generalizationWordItem->replaceOrDeleteSpecification( false, currentQuestionSpecificationItem->updatedSpecificationItem(), createdSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the answered question by the created specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								// Clean-up obsolete justifications
								if( generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, commonWordItem ) ) != NULL &&
								( anotherPrimarySpecificationItem = commonWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != NULL &&
								// Draw a negative assumption
								// Not yet explained in the design
								( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, NULL, generalizationWordItem, negativeQuestionSpecificationWordItem, NULL ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeQuestionSpecificationWordItem->anyWordTypeString(), "\"" );
								}
							}

						currentQuestionSpecificationItem = adminItem_->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
						}
					else
						currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem();
					}
				else
					currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem();
				}
			while( currentQuestionSpecificationItem != NULL );
			}

		return RESULT_OK;
		}

	signed char makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned short nOptionsLeft = 0;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *onlyOptionLeftSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeOnlyOptionLeftAssumption";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( !userSpecificationItem->hasSpecificationCollection() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification collection" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != NULL )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, NO_QUESTION_PARAMETER, currentQuestionSpecificationWordItem ) == NULL )
						{
						nOptionsLeft++;
						onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
						}
					}
				}
			while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

			if( nOptionsLeft == 1 &&
			onlyOptionLeftSpecificationWordItem != NULL &&
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
				{
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != NULL &&
					( anotherPrimarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, isPossessive, NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != NULL )
						{
						// Make an only option left assumption
						// Not yet explained in the design
						if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem, onlyOptionLeftSpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				while( !commonVariables_->hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		unsigned int secondarySpecificationCollectionNr;
		SpecificationItem *existingSpecificationItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();
		existingSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

		// Check if specification doesn't exist yet
		if( existingSpecificationItem == NULL ||
		existingSpecificationItem->isHiddenSpanishSpecification() )
			{
			if( secondarySpecificationCollectionNr > NO_COLLECTION_NR &&
			// Check if a related specification exists
			( relatedResult = generalizationWordItem->findRelatedSpecification( false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the secondary specification" );

			if( relatedResult.relatedSpecificationItem == NULL )
				{
				if( generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() )
					{
					if( correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem->questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					// Typically for the Spanish language:
					// Skip making suggestive assumption if specification word is Spanish ambiguous,
					// and if user question has more than one generalization words
					if( !secondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ||
					commonVariables_->nUserGeneralizationWords == 1 )
						{
						// Make a suggestive question assumption
						// Not yet explained in the design
						hasSuggestiveQuestionAssumption_ = true;

						if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	CompoundResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		RelatedResultType relatedResult;
		CompoundResultType compoundResult;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *previousSpecificationItem = NULL;
		SpecificationItem *lastCreatedSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( specificationWordItem == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		do	{
			if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( true ) ) != NULL )
				{
				previousSpecificationItem = lastCreatedSpecificationItem;

				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

					if( ( foundSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
						{
						if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem->hasAssumptionLevel(), foundSpecificationItem->isInactiveAssignment(), foundSpecificationItem->isArchivedAssignment(), foundSpecificationItem->isExclusiveSpecification(), true, NO_QUESTION_PARAMETER, foundSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( !commonVariables_->hasDisplayedWarning &&
						foundSpecificationItem->hasSpecificationCollection() )
							{
							if( ( relatedResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
								return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );

							if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL )
								{
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->hasAssumptionLevel(), relatedSpecificationItem->isInactiveAssignment(), relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), true, relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem->specificationWordItem(), NULL ) != RESULT_OK )
									return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

								compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
								}
							}
						}
					}
				while( !commonVariables_->hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
				}
			}
		while( !commonVariables_->hasDisplayedWarning &&
		// Do until no more conclusions or questions are created
		previousSpecificationItem != NULL );

		return compoundResult;
		}
	};

/*************************************************************************
 *	"Praise him for his mighty works;
 *	praise his unequaled greatness!" (Psalm 150:2)
 *************************************************************************/
