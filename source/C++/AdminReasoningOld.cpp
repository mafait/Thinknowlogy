/*	Class:			AdminReasoningOld
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (old design)
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

#include "AdminItem.h"
#include "FindSpecificationResultType.cpp"
#include "GeneralizationItem.cpp"

class AdminReasoningOld
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasSuggestiveQuestionAssumption_;

	unsigned int postponeNegativeConclusionSpecificationCollectionNr_;

	SpecificationItem *alternativeSpanishAnotherPrimarySpecificationItem_;
	SpecificationItem *oppositePossessiveDefinitionSpecificationItem_;
	SpecificationItem *possessiveDefinitionSpecificationItem_;
	SpecificationItem *postponeNegativeConclusionAnotherPrimarySpecificationItem_;
	SpecificationItem *postponeNegativeConclusionPrimarySpecificationItem_;
	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem_;

	WordItem *feminineSpecificationWordItem_;
	WordItem *generalizationAssumptionWordItem_;
	WordItem *oppositePossessiveSpecificationWordItem_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH];

	AdminItem *adminItem_;
	GlobalVariables *globalVariables_;
	InputOutput *inputOutput_;


	// Private functions

	void clearPostponedNegativeConclusionVariables()
		{
		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		}

	bool isFeminineOrMasculineProperNounEnding( bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
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
					relationWordItem->hasFeminineProperNounEnding() ) ||

					( !isPossessive &&
					generalizationWordItem->hasFeminineProperNounEnding() ) )
						return true;
					}
				else
					{
					// Possessive (relation)
					if( ( isPossessive &&
					relationWordItem != NULL &&

					( ( isFemaleSpecificationWord &&
					relationWordItem->hasFeminineProperNounEnding() ) ||

					( isMaleSpecificationWord &&
					relationWordItem->hasMasculineProperNounEnding() ) ) ) ||

					// Non-possessive (generalization)
					( !isPossessive &&

					( ( isFemaleSpecificationWord &&
					generalizationWordItem->hasFeminineProperNounEnding() ) ||

					( isMaleSpecificationWord &&
					generalizationWordItem->hasMasculineProperNounEnding() ) ) ) )
						return true;
					}
				}
			}

		return false;
		}

	signed char askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, bool isNegativeConclusion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasGeneralizationWordMasculineProperNounEnding;
		bool isCommonWordWordSpanishAmbiguous;
		bool isFemaleUserSpecificationWord = false;
		bool isHiddenSpanishPrimarySpecification;
		bool isMaleUserSpecificationWord = false;
		bool isPrimaryUserSpecification;
		bool isSpecificationWordSpanishAmbiguous;
		unsigned int anotherPrimarySpecificationCollectionNr;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *anotherSecondarySpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *firstNonCompoundCollectionSpecificationItem;
		SpecificationItem *questionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *collectionWordItem = NULL;
		WordItem *commonWordItem;
		WordItem *userSpecificationWordItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSpecificationSubstitutionQuestionOrDrawNegativeConclusion";

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

		if( ( specificationCollectionNr = commonWordItem->collectionNr( specificationWordItem ) ) == NO_COLLECTION_NR )
			specificationCollectionNr = anotherPrimarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();
		isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();
		isCommonWordWordSpanishAmbiguous = commonWordItem->isNounWordSpanishAmbiguous();
		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		relatedSpecificationItem == NULL &&
		!userSpecificationItem->isPossessive() &&
		( nonCompoundSpecificationCollectionNr = commonWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR )
			{
			if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

			relatedSpecificationItem = relatedResult.relatedSpecificationItem;
			}

		if( relatedSpecificationItem == NULL )
			{
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem->hasMasculineProperNounEnding();

			if( userSpecificationItem != primarySpecificationItem &&
			!userSpecificationItem->hasRelationContext() &&
			!userSpecificationItem->isExclusiveSpecification() &&
			globalVariables_->nUserSpecificationWords == 1 &&
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

			( hasGeneralizationWordFeminineProperNounEnding ||
			userSpecificationItem->isQuestion() ) ) )
				{
				if( isFemaleUserSpecificationWord ||
				isMaleUserSpecificationWord )
					{
					if( anotherPrimarySpecificationItem->hasCompoundSpecificationCollection() &&

					( anotherPrimarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ||

					( isFemaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem->isMale() ) ||

					( isMaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem->isFemale() ) ) &&

					// Draw negative conclusion
					drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					if( !isHiddenSpanishPrimarySpecification )
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
				if( ( ( !hasGeneralizationWordFeminineProperNounEnding &&
				!hasGeneralizationWordMasculineProperNounEnding ) ||

				( primarySpecificationItem == userSpecificationItem &&
				!anotherPrimarySpecificationWordItem->isFemaleOrMale() ) ||

				primarySpecificationItem->hasCompoundSpecificationCollection() ) &&

				( !isSpecificationWordSpanishAmbiguous ||
				anotherPrimarySpecificationItem->hasNonCompoundSpecificationCollection() ||
				anotherPrimarySpecificationWordItem->compoundCollectionNr() == anotherPrimarySpecificationCollectionNr ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem );
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

					( existingSpecificationItem == NULL ||
					existingSpecificationItem->isHiddenSpanishSpecification() ||
					generalizationWordItem->isUserGeneralizationWord ) ) )
						{
						if( isSpecificationWordSpanishAmbiguous &&
						isPrimaryUserSpecification &&
						!primarySpecificationItem->isExclusiveSpecification() )
							{
							postponeNegativeConclusionSpecificationCollectionNr_ = anotherPrimarySpecificationCollectionNr;

							postponeNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
							postponeNegativeConclusionAnotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
							}
						else
							{
							if( isPrimaryUserSpecification ||
							primarySpecificationItem->hasRelationContext() ||
							generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) == NULL )
								{
								if( adminItem_->askQuestions( true ) != RESULT_OK )
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
				drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, postponeNegativeConclusionPrimarySpecificationItem_, postponeNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				clearPostponedNegativeConclusionVariables();

				// Typical for Spanish
				if( isCommonWordWordSpanishAmbiguous &&
				isSpecificationWordSpanishAmbiguous &&
				// specificationWordItem == commonWordItem &&
				globalVariables_->nUserSpecificationWords == 1 &&
				!primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
				primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr &&

				( isPrimaryUserSpecification ||
				generalizationWordItem->isUserRelationWord ) )
					{
					questionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, anotherPrimarySpecificationWordItem );

					if( ( questionSpecificationItem == NULL ||
					questionSpecificationItem->isOlderItem() ) &&

					generalizationWordItem->firstSpecificationItem( false, false, false, anotherPrimarySpecificationWordItem ) == NULL &&

					// Ask Spanish ambiguous questions
					adminItem_->askQuestions( false ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions" );
					}
				else
					{
					if( anotherPrimarySpecificationItem->hasNonCompoundSpecificationCollection() ||
					anotherPrimarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ||
					anotherPrimarySpecificationWordItem->compoundCollectionNr() == anotherPrimarySpecificationCollectionNr )
						{
						if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( isHiddenSpanishPrimarySpecification &&
						specificationWordItem == commonWordItem &&
						anotherPrimarySpecificationWordItem == commonWordItem &&
						userSpecificationItem != relatedSpecificationItem &&
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

	signed char drawNegativeConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasAnotherSecondarySpecificationWithCompoundCollection = false;
		bool hasDisplayedMoreSpecificNonExclusiveSpecification;
		bool hasFoundOldSpanishUserSpecification = false;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasRelatedSpecificationsInSameSentence = false;
		bool isCompoundUserSpecification = false;
		bool isHiddenSpanishPrimarySpecification;
		bool isPostponingNegativeConclusion = false;
		bool isPrimarySameAsSecondarySpecification;
		bool isUserGeneralizationWord;
		bool isUserSpecificationSelected = false;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned int anotherPrimarySpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int secondarySpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *negativeAssumptionJustificationItem;
		JustificationItem *negativeAssumptionOrConclusionJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingNegativeSpecificationItem;
		SpecificationItem *existingPositiveSpecificationItem;
		SpecificationItem *foundNegativeAssumptionOrConclusionPrimarySpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *negativeConclusionAnotherPrimarySpecificationItem;
		SpecificationItem *negativeConclusionPrimarySpecificationItem;
		SpecificationItem *negativeConclusionAnotherSecondarySpecificationItem;
		SpecificationItem *negativeConclusionSecondarySpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *collectionWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationCommonWordItem = NULL;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeConclusion";

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

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );

		// If exists, replace self-generated assumption for conclusion
		if( primarySpecificationItem->isSelfGeneratedAssumption() &&
		( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem->isArchivedAssignment(), primarySpecificationItem->isNegative(), primarySpecificationItem->isPossessive(), false, primarySpecificationWordItem, NULL ) ) != NULL )
			primarySpecificationItem = tempSpecificationItem;

		existingPositiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, anotherPrimarySpecificationWordItem );

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

		// No question exists
		generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, true, anotherPrimarySpecificationWordItem ) == NULL )
			{
			if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

			if( !secondarySpecificationItem->isOlderItem() &&
			secondarySpecificationItem->isSelfGeneratedAssumption() &&
			( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem, NULL ) ) != NULL )
				{
				// Add specification collection to specification
				if( ( createAndAssignResult = generalizationWordItem->copySpecificationItem( secondarySpecificationItem->specificationCollectionNr(), tempSpecificationItem->firstJustificationItem(), tempSpecificationItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy a specification without specification collection" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create a specification" );

				if( generalizationWordItem->replaceOrDeleteSpecification( tempSpecificationItem, createdSpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a specification without specification collection in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				secondarySpecificationItem = createdSpecificationItem;

				if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, secondarySpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an updated specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}

			hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();
			isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

			if( isPrimarySameAsSecondarySpecification )
				{
				if( hasPrimarySpecificationCompoundCollection &&
				primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
					{
					if( ( collectionWordItem = collectionWordItemInCollectionWords( anotherPrimarySpecificationCollectionNr, primarySpecificationWordItem ) ) != NULL &&
					( anotherSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, collectionWordItem ) ) == NULL )
						anotherSecondarySpecificationItem = ( !userSpecificationItem->isSpecificationWordSpanishAmbiguous() ? userSpecificationItem : primarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, collectionWordItem ) );
					}
				else
					{
					if( globalVariables_->nUserSpecificationWords > 1 &&
					globalVariables_->nUserRelationWords == 0 &&
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
					anotherPrimarySpecificationItem->hasCompoundSpecificationCollection() &&
					anotherPrimarySpecificationItem->isSelfGenerated() &&
					generalizationWordItem->isUserRelationWord &&
					( collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
						{
						if( ( nonCompoundSpecificationCollectionNr = collectionWordItem->nonCompoundCollectionNr() ) == NO_COLLECTION_NR )
							nonCompoundSpecificationCollectionNr = anotherPrimarySpecificationWordItem->nonCompoundCollectionNr();

						if( ( tempSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, isArchivedAssignment, false, false, nonCompoundSpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, NULL ) ) != NULL )
							{
							hasFoundOldSpanishUserSpecification = true;
							secondarySpecificationItem = tempSpecificationItem;
							}
						}

					if( !hasFoundOldSpanishUserSpecification )
						{
						if( ( anotherSecondarySpecificationItem = secondarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, NULL ) ) == NULL )
							{
							if( secondarySpecificationItem->hasCompoundSpecificationCollection() &&
							secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
							( secondarySpecificationCommonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL &&
							secondarySpecificationCommonWordItem != primarySpecificationWordItem )
								anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem->firstSpecificationItem( false, false, false, secondarySpecificationWordItem );
							}
						else
							{
							if( !anotherSecondarySpecificationItem->isExclusiveSpecification() &&
							( collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr ) ) != NULL )
								{
								isPostponingNegativeConclusion = true;
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

					( isUserSpecificationWordSpanishAmbiguous ||
					userSpecificationItem->isPossessive() ) )
						{
						isUserSpecificationSelected = true;

						if( !isPrimarySameAsSecondarySpecification &&
						anotherSecondarySpecificationItem != NULL &&
						userSpecificationItem->hasCompoundSpecificationCollection() &&
						userSpecificationWordItem->hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
							isCompoundUserSpecification = true;
						}
					else
						{
						if( !anotherPrimarySpecificationWordItem->isNounWordSpanishAmbiguous() &&
						( simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL )
							{
							isUserSpecificationSelected = true;
							userSpecificationItem = simpleUserSpecificationItem;

							// Hidden
							if( anotherSecondarySpecificationItem != NULL )
								hasAnotherSecondarySpecificationWithCompoundCollection = true;
							}
						}

					if( isUserSpecificationSelected &&
					userSpecificationItem->isPossessive() )
						oppositePossessiveDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
					}

				existingNegativeSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, true, false, false, anotherPrimarySpecificationWordItem );

				if( hasFoundOldSpanishUserSpecification ||
				isCompoundUserSpecification ||

				( ( existingNegativeSpecificationItem == NULL ||

				( !isHiddenSpanishPrimarySpecification &&

				( hasPrimarySpecificationCompoundCollection ||
				existingNegativeSpecificationItem->isSelfGeneratedAssumption() ) &&

				( existingNegativeSpecificationItem->isOlderItem() ||
				existingNegativeSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ) &&

				generalizationWordItem->primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, primarySpecificationItem ) == NULL ) ||

				( isUserSpecificationSelected &&

				( ( !primarySpecificationItem->hasRelationContext() &&

				( existingNegativeSpecificationItem == NULL ||
				existingNegativeSpecificationItem->isOlderItem() ) ) ||

				( isHiddenSpanishPrimarySpecification &&
				existingNegativeSpecificationItem != NULL &&

				( existingNegativeSpecificationItem->hasNonCompoundSpecificationCollection() ||

				( existingNegativeSpecificationItem->isOlderItem() &&
				secondarySpecificationItem->isSelfGeneratedAssumption() ) ) ) ) ) )
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
								userSpecificationItem->hasNonCompoundSpecificationCollection() ) ||

								( anotherPrimarySpecificationItem != NULL &&
								anotherPrimarySpecificationItem->hasCompoundSpecificationCollection() ) )
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
									negativeConclusionAnotherSecondarySpecificationItem = secondarySpecificationItem;
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
						if( hasPrimarySpecificationCompoundCollection &&
						!isPrimarySameAsSecondarySpecification &&
						isUserSpecificationSelected &&
						primarySpecificationItem->isSelfGenerated() &&
						!userSpecificationItem->isQuestion() &&
						userSpecificationItem->hasCompoundSpecificationCollection() )
							compoundGeneralizationWordItem = anotherPrimarySpecificationWordItem->compoundGeneralizationWordItem( anotherPrimarySpecificationCollectionNr );

						if( compoundGeneralizationWordItem != NULL &&
						compoundGeneralizationWordItem->isNounWordSpanishAmbiguous() )
							{
							negativeConclusionPrimarySpecificationItem = compoundGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, false, anotherPrimarySpecificationWordItem );
							negativeConclusionAnotherPrimarySpecificationItem = NULL;
							negativeConclusionSecondarySpecificationItem = anotherSecondarySpecificationItem;
							negativeConclusionAnotherSecondarySpecificationItem = NULL;
							}
						else
							{
							if( hasDisplayedMoreSpecificNonExclusiveSpecification )
								{
								negativeConclusionPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? anotherSecondarySpecificationItem : generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, secondarySpecificationCommonWordItem ) );
								negativeConclusionAnotherPrimarySpecificationItem = ( isUserSpecificationWordSpanishAmbiguous ? NULL : anotherSecondarySpecificationItem );
								negativeConclusionSecondarySpecificationItem = userSpecificationItem;
								negativeConclusionAnotherSecondarySpecificationItem = NULL;
								}
							else
								{
								negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
								negativeConclusionAnotherPrimarySpecificationItem = ( hasRelatedSpecificationsInSameSentence ? NULL : anotherPrimarySpecificationItem );

								negativeConclusionSecondarySpecificationItem = ( isCompoundUserSpecification ? userSpecificationItem :
																				( isPrimarySameAsSecondarySpecification ? ( hasRelatedSpecificationsInSameSentence ? anotherPrimarySpecificationItem :
																				( primarySpecificationItem != userSpecificationItem &&
																				!userSpecificationItem->hasRelationContext() &&
																				userSpecificationItem->specificationCollectionNr() == anotherPrimarySpecificationCollectionNr ? userSpecificationItem : anotherSecondarySpecificationItem ) ) : secondarySpecificationItem ) );

								negativeConclusionAnotherSecondarySpecificationItem = ( isCompoundUserSpecification ||
																						isPrimarySameAsSecondarySpecification ? NULL : anotherSecondarySpecificationItem );
								}
							}
						}

					// Draw negative conclusion
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, primarySpecificationItem->isExclusiveSpecification(), false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationWordSpanishAmbiguous && primarySpecificationItem->isSelfGeneratedAssumption() ? JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, NULL ) ).result != RESULT_OK )
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
									if( createdJustificationItem->isConclusionJustification() &&
									foundSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
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
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption justification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a negative conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created negative conclusion" );

							relatedSpecificationItem = relatedResult.relatedSpecificationItem;

							if( relatedSpecificationItem != NULL &&

							// Replace a corrected negative conclusion
							generalizationWordItem->replaceOrDeleteSpecification( relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected negative conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( isCompoundUserSpecification &&
							// Change justification of negative assumptions
							generalizationWordItem->changeJustificationOfNegativeAssumptions( userSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a question justification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFeminineProperNounEnding;
		bool isAssignment;
		bool isAssumption = ( userAssumptionLevel > NO_ASSUMPTION_LEVEL );
		bool isConclusionAssignment;
		bool isForcingNewJustification;
		bool isSecondarySpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		unsigned int foundRelationContextNr;
		unsigned int generalizationContextNr = NO_CONTEXT_NR;
		unsigned int nContextRelations;
		unsigned int spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *collectedSpecificationItem;
		SpecificationItem *confirmedAllRelationWordsOfSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *collectedWithItselfCommonWordItem;
		WordItem *masculineSpecificationWordItem;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawPossessiveReversibleConclusion";

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

		if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, specificationWordItem, generalizationWordItem ) ) == NULL ||
		// Allow existing conclusion specification to change its relation context
		existingSpecificationItem->isSelfGenerated() )
			{
			isSecondarySpecificationWordSpanishAmbiguous = secondarySpecificationWordItem->isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			if( isPossessive &&
			isUserGeneralizationWord &&
			!isSecondarySpecificationWordSpanishAmbiguous &&

			( existingSpecificationItem == NULL ||
			globalVariables_->nUserRelationWords > 1 ) &&

			( foundRelationContextNr = generalizationWordItem->contextNr( specificationWordItem ) ) > NO_CONTEXT_NR )
				{
				if( ( nContextRelations = adminItem_->nContextWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
					{
					if( ( contextResult = adminItem_->getGeneralizationContext( isArchivedAssignment, false, false, false, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get a generalization context" );

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
				secondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Skip if possessive user specification exists
			if( secondarySpecificationItem != NULL &&

			// User specification doesn't exist
			( ( existingUserSpecificationItem == NULL &&
			secondarySpecificationItem->isUserSpecification() ) ||

			// User specification exists
			( existingUserSpecificationItem != NULL &&

			( ( !isSecondarySpecificationWordSpanishAmbiguous &&

			( isPossessive ||
			existingUserSpecificationItem->isAssignment() ) ) ||

			// Typical for Spanish
			( isSecondarySpecificationWordSpanishAmbiguous &&
			!secondarySpecificationItem->isUserAssignment() &&

			( !existingUserSpecificationItem->hasRelationContext() ||
			generalizationWordItem->hasDisplayedMoreSpecificSpecification() ) ) ) ) ) &&

			secondarySpecificationItem->hasNewInformation() )
				{
				isForcingNewJustification = ( ( ( isSecondarySpecificationWordSpanishAmbiguous ||
											secondarySpecificationItem->isUserAssignment() ) &&

											secondarySpecificationItem->hasSpecificationCollection() ) ||

											( !isArchivedAssignment &&
											isPossessive &&
											existingSpecificationItem != NULL &&
											secondarySpecificationItem->hasNonCompoundSpecificationCollection() ) );

				if( !isForcingNewJustification &&
				// Draw possessive reversible conclusions
				adminItem_->drawPossessiveReversibleConclusions( generalizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				if( !isExclusiveSpecification &&
				!secondarySpecificationItem->isExclusiveGeneralization() )
					{
					hasFeminineProperNounEnding = ( isPossessive &&
													isSecondarySpecificationWordSpanishAmbiguous &&
													relationWordItem->hasFeminineProperNounEnding() );

					isConclusionAssignment = ( ( isArchivedAssignment &&
												isAssumption ) ||

												( !isAssumption &&
												isArchivedAssignment ) );

					isAssignment = ( isConclusionAssignment ||

									( existingUserSpecificationItem != NULL &&
									existingUserSpecificationItem->isAssignment() ) );

					// Typical for Spanish
					if( isSecondarySpecificationWordSpanishAmbiguous &&
					secondarySpecificationWordItem == specificationWordItem &&

					( !isPossessive ||
					!relationWordItem->isMale() ) )
						spanishAmbiguousSpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

					// Draw a possessive reversible conclusion
					// See paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasFeminineProperNounEnding, isAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, NULL, NULL, secondarySpecificationItem, ( hasFeminineProperNounEnding ? secondarySpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) : NULL ), relationWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

					if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
					foundSpecificationItem->isReplacedItem() )
						foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

					if( confirmedAllRelationWordsOfSpecificationItem == NULL )
						{
						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL &&
						existingSpecificationItem != NULL )
							{
							if( existingSpecificationItem->isConcludedAssumption() )
								{
								if( relationWordItem->replaceOrDeleteUnreferencedJustifications() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"", relationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								if( !isSecondarySpecificationWordSpanishAmbiguous &&
								foundSpecificationItem != NULL &&
								!existingSpecificationItem->isReplacedOrDeletedItem() &&
								existingSpecificationItem->isSelfGeneratedAssumption() &&
								foundSpecificationItem->isSelfGeneratedConclusion() &&
								foundSpecificationItem->hasCompoundSpecificationCollection() &&

								// Replace assumption by conclusion
								relationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, foundSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption specification by a conclusion in word \"", relationWordItem->anyWordTypeString(), "\"" );
								}
							}

						if( isPossessive &&
						( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? foundSpecificationItem : createdSpecificationItem ) ) != NULL )
							{
							if( createdOrFoundSpecificationItem->hasNonCompoundSpecificationCollection() &&
							makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, NULL, generalizationWordItem, relationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption during the drawing of a possessive reversible conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

							// Typical for Spanish
							if( isSecondarySpecificationWordSpanishAmbiguous &&
							( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem->collectedWithItselfCommonWordItem() ) != NULL )
								{
								if( relationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, true, false, false, collectedWithItselfCommonWordItem ) == NULL &&
								( secondarySpecificationItem = collectedWithItselfCommonWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, createdOrFoundSpecificationItem->specificationCollectionNr(), specificationWordItem ) ) != NULL &&

								drawNegativeConclusion( isArchivedAssignment, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationItem, NULL, createdOrFoundSpecificationItem, NULL, relationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about relation word \"", relationWordItem->anyWordTypeString(), "\"" );
								}
							}

						if( isUserGeneralizationWord )
							{
							if( specificationWordItem->isUserSpecificationWord )
								{
								// Typical for Spanish
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
												if( generalizationWordItem->addContext( relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem->relationContextNr(), NO_COLLECTION_NR, specificationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												// Not hidden anymore after addition of context
												if( relationWordItem->writeUpdatedSpecification( true, false, false, false, false, existingSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the hidden assumption in word \"", relationWordItem->anyWordTypeString(), "\"" );
												}
											else
												{
												if( createdSpecificationItem != NULL &&
												!existingSpecificationItem->hasCurrentCreationSentenceNr() &&

												// Replace an assumption without relation word
												relationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing assumption by the created conclusion" );
												}
											}
										else
											{
											if( secondarySpecificationItem != NULL &&
											existingSpecificationItem->secondarySpecificationJustificationItem( secondarySpecificationItem ) != NULL &&
											generalizationWordItem->hasDisplayedMoreSpecificSpecification() &&

											// Draw proper noun 'part of' conclusions
											adminItem_->drawProperNounPartOfConclusions( false, isArchivedAssignment, NULL, specificationWordItem, relationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
											}
										}
									}
								else
									{
									if( isPossessive )
										{
										if( createdSpecificationItem != NULL &&
										foundSpecificationItem == NULL &&
										( assumptionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, NULL ) ) != NULL )
											{
											// Additional justification (assumption)
											if( ( justificationResult = relationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, secondarySpecificationItem, NULL, assumptionSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", relationWordItem->anyWordTypeString(), "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
												return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

											if( assumptionSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the assumption specification" );

											if( generalizationWordItem->addContext( relationWordTypeNr, specificationWordTypeNr, assumptionSpecificationItem->relationContextNr(), NO_COLLECTION_NR, specificationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( ( collectedSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, assumptionSpecificationItem->specificationCollectionNr(), NULL ) ) != NULL &&
											( firstJustificationItem = collectedSpecificationItem->firstJustificationItem() ) != NULL )
												{
												// Additional justification (another)
												if( ( justificationResult = relationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, firstJustificationItem->primarySpecificationItem(), NULL, collectedSpecificationItem, NULL, assumptionSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", relationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												if( assumptionSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification (again) of the assumption specification" );

												// Additional justification (collected)
												if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, firstJustificationItem->primarySpecificationItem(), NULL, createdSpecificationItem, NULL, collectedSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												if( collectedSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the collected specification" );
												}
											}
										}
									else
										{
										if( adminItem_->isSpanishCurrentLanguage() &&
										createdSpecificationItem != NULL &&
										( masculineSpecificationWordItem = specificationWordItem->masculineCollectionWordItem() ) != NULL &&
										( obsoleteSpecificationItem = relationWordItem->noRelationContextSpecificationItem( true, true, masculineSpecificationWordItem ) ) != NULL )
											{
											// Write an assumption without relation word
											if( relationWordItem->writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", relationWordItem->anyWordTypeString(), "\"" );

											// Replace an assumption without relation word
											if( relationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
											}
										}
									}

								if( makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								if( createdSpecificationItem != NULL &&
								foundSpecificationItem != NULL &&
								( firstJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != NULL &&
								( secondarySpecificationItem = firstJustificationItem->secondarySpecificationItem() ) != NULL )
									{
									// Additional justification (Created Spanish copy)
									if( ( justificationResult = relationWordItem->addJustification( false, false, true, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, secondarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", relationWordItem->anyWordTypeString(), "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

									if( createdSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
									}
								}
							}
						}
					else
						{
						if( relationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, confirmedAllRelationWordsOfSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a specification in word \"", relationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char drawSpecificationGeneralizationConclusion( unsigned short assumptionLevel, SpecificationItem *secondarySpecificationItem )
		{
		bool isArchivedAssignment;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( !secondarySpecificationItem->isGeneralizationNoun() ||
		!secondarySpecificationItem->isSpecificationNoun() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );

		isArchivedAssignment = secondarySpecificationItem->isArchivedAssignment();

		// Draw a specification-generalization conclusion
		// Not yet explained in the design
		if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, secondarySpecificationItem->isExclusiveSpecification(), false, secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPartOf(), secondarySpecificationItem->isPossessive(), false, false, false, true, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, secondarySpecificationItem->specificationWordItem(), secondarySpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification" );

		return RESULT_OK;
		}

	signed char makeGeneralizationAssumption( bool hasCorrectedAssumptionByKnowledge, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		JustificationItem *olderGeneralizationAssumptionBySpecificationJustificationItem;
		SpecificationItem *olderGeneralizationAssumptionBySpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeGeneralizationAssumption";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( secondarySpecificationItem->hasCurrentCreationSentenceNr() &&
		// Skip if user specification already exists
		generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) == NULL &&
		// Skip if justification with primary specification already exists
		generalizationWordItem->primarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem ) == NULL )
			{
			// Make a generalization assumption
			// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
			if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, ( generalizationWordTypeNr != WORD_TYPE_PROPER_NOUN ), false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			if( hasCorrectedAssumptionByKnowledge &&
			( olderGeneralizationAssumptionBySpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
			( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != NULL &&

			// Replace justification
			generalizationWordItem->replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, createAndAssignResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeGeneralizationAssumptionByGeneralization";

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
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( !currentGeneralizationWordItem->hasCollection() &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != NULL )
					{
					// Get secondary specification for justification
					if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != NULL )
						{
						if( !primarySpecificationItem->isSelfGeneratedAssumption() &&
						secondarySpecificationItem->hasCurrentCreationSentenceNr() &&

						// Make generalization assumption
						makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						// Get secondary question specification for justification
						if( ( secondarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, true, specificationWordItem ) ) != NULL &&
						drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( !globalVariables_->hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makePossessiveConditionalSpecificationAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *alternativeSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool hasAlternativeSecondarySpecificationCompoundCollection;
		bool hasFeminineOrMasculineProperNounEnding = false;
		bool hasSameSpecificationCollection;
		bool isExistingHiddenSpanishSpecification = false;
		bool isForcingNewJustification = false;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveReversibleAssumption;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isRelationWordUserWordGeneralization;
		unsigned short assumptionJustificationTypeNr;
		unsigned short assumptionLevel = NO_ASSUMPTION_LEVEL;
		unsigned int hiddenSpanishSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int relationContextNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *foundJustificationItem;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *selfGeneratedConclusionSpecificationItem = NULL;
		SpecificationItem *userSpecificationItem;
		WordItem *feminineSpecificationWordItem = NULL;
		WordItem *masculineSpecificationWordItem;
		WordItem *possessiveSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makePossessiveConditionalSpecificationAssumption";

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
		( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem() ) != NULL )
			{
			isPossessiveSpecificationWordSpanishAmbiguous = possessiveSpecificationWordItem->isNounWordSpanishAmbiguous();

			if( isPossessiveSpecificationWordSpanishAmbiguous ||
			!secondarySpecificationItem->hasCompoundSpecificationCollection() )
				{
				isRelationWordUserWordGeneralization = relationWordItem->isUserGeneralizationWord;

				if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, !isPossessive, possessiveSpecificationWordItem ) ) != NULL &&
				existingSpecificationItem->isHiddenSpanishSpecification() )
					isExistingHiddenSpanishSpecification = true;

				existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveSpecificationWordItem );

				if( ( !isPossessiveSpecificationWordSpanishAmbiguous ||
				existingUserSpecificationItem != NULL ||
				existingSpecificationItem == NULL ||
				existingSpecificationItem->isHiddenSpanishSpecification() ||
				!existingSpecificationItem->hasSpecificationCollection() ||
				!existingSpecificationItem->hasOnlyOneRelationWord() ||
				secondarySpecificationItem->isSelfGenerated() ||
				!generalizationWordItem->hasCurrentlyConfirmedSpecification() ) &&

				// Skip if possessive user specification exists
				( ( existingUserSpecificationItem == NULL &&
				!possessiveSpecificationWordItem->isUserSpecificationWord &&

				( !isPossessiveSpecificationWordSpanishAmbiguous ||

				// Typical for Spanish
				( isRelationWordUserWordGeneralization &&
				existingSpecificationItem == NULL ) ) &&

				// Skip if possessive specification exists
				relationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == NULL ) ||

				// Typical for Spanish
				( isPossessiveSpecificationWordSpanishAmbiguous &&
				alternativeSecondarySpecificationItem != NULL &&
				!alternativeSecondarySpecificationItem->isOlderItem() &&

				( existingUserSpecificationItem != NULL ||
				alternativeSecondarySpecificationItem->isUserSpecification() ) &&

				( ( !isPossessive &&
				existingUserSpecificationItem != NULL ) ||

				( ( existingSpecificationItem == NULL ||
				!existingSpecificationItem->isConcludedAssumption() ) &&

				alternativeSecondarySpecificationItem->isActiveAssignment() ) ||

				( existingSpecificationItem != NULL &&
				existingUserSpecificationItem == NULL &&
				!generalizationWordItem->hasActiveQuestionWithCompoundCollection() &&

				// Not hidden
				( ( !isExistingHiddenSpanishSpecification &&
				!existingSpecificationItem->isOlderItem() ) ||

				// Hidden
				( isExistingHiddenSpanishSpecification &&
				existingSpecificationItem->hasCurrentCreationSentenceNr() &&
				existingSpecificationItem->isOlderItem() ) ) ) ) &&

				// Skip if possessive user specification exists
				( existingUserSpecificationItem == NULL ||

				( existingUserSpecificationItem->isOlderItem() &&
				existingUserSpecificationItem->hasCompoundSpecificationCollection() ) ) ) ) )
					{
					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == possessiveSpecificationWordItem );

					if( isPossessiveSpecificationWordSpanishAmbiguous &&
					!isPossessive &&
					existingSpecificationItem != NULL )
						{
						if( isExistingHiddenSpanishSpecification )
							{
							if( existingSpecificationItem->isSelfGeneratedAssumption() &&
							secondarySpecificationItem->isUserSpecification() )
								{
								// Force assumption
								assumptionLevel++;
								isForcingNewJustification = true;
								}
							}
						else
							isForcingNewJustification = true;
						}

					// Make a possessive conditional specification assumption (normal)
					// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, false, false, false, false, false, assumptionLevel, ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isPossessive ? possessiveDefinitionSpecificationItem_ : NULL ), NULL, ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), NULL, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );

					createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
					createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? createAndAssignResult.foundSpecificationItem : createdSpecificationItem );
					}

				if( !globalVariables_->hasDisplayedWarning )
					{
					if( oppositePossessiveSpecificationWordItem_ == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );

					if( alternativeSecondarySpecificationItem != NULL &&
					// Try to find a self-generated conclusion first
					( selfGeneratedConclusionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveSpecificationWordItem_, NULL ) ) == NULL &&
					// If not found, try to find a self-generated assumption
					( assumptionSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != NULL )
						{
						hasSameSpecificationCollection = ( assumptionSpecificationItem->specificationCollectionNr() == alternativeSecondarySpecificationItem->specificationCollectionNr() );

						if( !assumptionSpecificationItem->hasCurrentCreationSentenceNr() &&
						!assumptionSpecificationItem->hasNewInformation() &&

						( generalizationWordItem->hasCurrentlyConfirmedSpecification() == hasSameSpecificationCollection ||
						alternativeSecondarySpecificationItem->isPossessive() ) )
							selfGeneratedConclusionSpecificationItem = assumptionSpecificationItem;
						}

					isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_->isNounWordSpanishAmbiguous();
					feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveSpecificationWordItem_->feminineCollectionWordItem() : oppositePossessiveSpecificationWordItem_->feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem->hasFeminineProperNounEnding() : relationWordItem->hasFeminineProperNounEnding() ) );

					if( feminineSpecificationWordItem != NULL )
						{
						if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
							anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_->firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
						else
							feminineSpecificationWordItem = NULL;
						}

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

					( alternativeSecondarySpecificationItem == NULL ||
					alternativeSecondarySpecificationItem->isUserSpecification() ) &&

					isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
						hasFeminineOrMasculineProperNounEnding = true;

					hasAlternativeSecondarySpecificationCompoundCollection = ( alternativeSecondarySpecificationItem != NULL &&
																				alternativeSecondarySpecificationItem->hasCompoundSpecificationCollection() );

					isPossessiveReversibleAssumption = ( secondarySpecificationWordItem == oppositePossessiveSpecificationWordItem_ );

					assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION :

													// Typical for Spanish
													( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
													!hasAlternativeSecondarySpecificationCompoundCollection &&
													!isPossessive ) ? JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

					if( ( ( selfGeneratedConclusionSpecificationItem == NULL ||
					!generalizationWordItem->isOlderItem() ||

					( isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem->isSelfGenerated() &&
					relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) != NULL ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

					( ( relationContextNr = selfGeneratedConclusionSpecificationItem->relationContextNr() ) == NO_CONTEXT_NR ||

					( !isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem->isUserSpecification() &&
					// Relation context doesn't exist
					!generalizationWordItem->hasContextInWord( relationContextNr, oppositePossessiveSpecificationWordItem_ ) ) ) ) ) &&

					( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					!secondarySpecificationItem->isConcludedAssumption() &&

					( relationWordItem->secondarySpecificationJustificationItem( true, assumptionJustificationTypeNr, secondarySpecificationItem ) == NULL ||

					( feminineSpecificationWordItem != NULL &&

					( hasAlternativeSecondarySpecificationCompoundCollection ||
					isPossessiveReversibleAssumption ) &&

					// Skip if feminine specification exists as user specification
					relationWordItem->firstUserSpecificationItem( false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, feminineSpecificationWordItem ) == NULL ) ) ) ||

					( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					alternativeSecondarySpecificationItem != NULL &&
					// Skip if already exists as self-generated assumption
					relationWordItem->primarySpecificationJustificationItem( false, assumptionJustificationTypeNr, alternativeSecondarySpecificationItem ) == NULL ) ) ) &&

					( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
					!isPossessive ||
					anotherPrimarySpecificationItem == NULL ||

					( isRelationWordUserWordGeneralization &&
					selfGeneratedConclusionSpecificationItem == NULL ) ||

					( selfGeneratedConclusionSpecificationItem != NULL &&
					( relationContextNr = selfGeneratedConclusionSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
					!generalizationWordItem->hasContextInWord( relationContextNr, oppositePossessiveSpecificationWordItem_ ) ) ||

					( ( existingUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, isPossessive, anotherPrimarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NULL ) ) != NULL &&
					!existingUserSpecificationItem->isOlderItem() ) ) )
						{
						// Typical for Spanish
						if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
						existingSpecificationItem != NULL &&

						( existingSpecificationItem->isUserSpecification() ||
						existingSpecificationItem->isHiddenSpanishSpecification() ) )
							hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_->specificationCollectionNr();

						// Make a possessive conditional specification assumption (opposite)
						// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, ( assumptionJustificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ? secondarySpecificationItem->assumptionLevel() : NO_ASSUMPTION_LEVEL ), assumptionJustificationTypeNr, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isPossessiveReversibleAssumption ? NULL : ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : possessiveDefinitionSpecificationItem_ ) ), NULL, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( hasFeminineOrMasculineProperNounEnding && !isPossessive ? anotherPrimarySpecificationItem : possessiveDefinitionSpecificationItem_ ) : secondarySpecificationItem ), NULL, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

						if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == NULL )
							{
							if( selfGeneratedConclusionSpecificationItem == NULL &&
							// Created, not found
							( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
								{
								// Typical for Spanish
								if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
								secondarySpecificationItem->isUserSpecification() &&
								!createdSpecificationItem->isHiddenSpanishSpecification() &&
								( obsoleteSpecificationItem = relationWordItem->noRelationContextSpecificationItem( isPossessive, true, oppositePossessiveSpecificationWordItem_ ) ) != NULL )
									{
									// Write an assumption without relation word
									if( relationWordItem->writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", relationWordItem->anyWordTypeString(), "\"" );

									// Replace an assumption without relation word
									if( relationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
									}
								}
							}
						else	// Found
							{
							if( isPossessive &&
							feminineSpecificationWordItem == NULL &&
							foundSpecificationItem->hasNonCompoundSpecificationCollection() &&
							foundSpecificationItem->isOlderItem() &&
							!foundSpecificationItem->isHiddenSpanishSpecification() &&

							// Cleanup unreferenced justifications
							relationWordItem->replaceOrDeleteUnreferencedJustifications() != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"", relationWordItem->anyWordTypeString(), "\"" );
							}

						if( alternativeSecondarySpecificationItem != NULL &&
						feminineSpecificationWordItem != NULL )
							{
							if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem ) ) == NULL ||
							existingSpecificationItem->isSelfGeneratedAssumption() )
								{
								// Make an opposite possessive feminine proper noun ending assumption
								// Not yet explained in the design
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( existingSpecificationItem == NULL ? NO_CONTEXT_NR : existingSpecificationItem->relationContextNr() ), alternativeSecondarySpecificationItem, ( !isPossessive && isOppositePossessiveSpecificationWordSpanishAmbiguous && oppositePossessiveDefinitionSpecificationItem_ != NULL ? oppositePossessiveDefinitionSpecificationItem_ : possessiveDefinitionSpecificationItem_ ), anotherPrimarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );

								if( isPossessive )
									{
									if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
										{
										// Typical for Spanish
										if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
											{
											if( ( masculineSpecificationWordItem = feminineSpecificationWordItem->masculineCollectionWordItem() ) != NULL &&
											( obsoleteSpecificationItem = relationWordItem->noRelationContextSpecificationItem( true, true, masculineSpecificationWordItem ) ) != NULL )
												{
												// Write an assumption without relation word
												if( relationWordItem->writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", relationWordItem->anyWordTypeString(), "\"" );

												// Replace an assumption without relation word
												if( relationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
												}
											}
										else
											{
											if( alternativeSecondarySpecificationItem->isOlderItem() &&
											alternativeSecondarySpecificationItem->isSelfGeneratedAssumption() &&
											( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
												{
												// Additional justification (feminine)
												if( ( justificationResult = relationWordItem->addJustification( true, false, true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, userSpecificationItem, possessiveDefinitionSpecificationItem_, anotherPrimarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", relationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the feminine assumption specification" );
												}
											}
										}
									}
								else
									{
									feminineSpecificationWordItem_ = feminineSpecificationWordItem;

									if( !isArchivedAssignment &&
									( createdOrFoundSpecificationItem = ( createAndAssignResult.createdSpecificationItem == NULL ? createAndAssignResult.foundSpecificationItem : createAndAssignResult.createdSpecificationItem ) ) != NULL &&
									!createdOrFoundSpecificationItem->isReplacedItem() )
										{
										if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() )
											{
											// Draw possessive reversible conclusion
											if( drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem->assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, createdOrFoundSpecificationItem->specificationContextNr(), createdOrFoundSpecificationItem->relationContextNr(), relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											// Typical for Spanish. Test files: "Complejo (6)" and "Complejo (11)"
											if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
											existingSpecificationItem == NULL &&
											( conclusionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, false, feminineSpecificationWordItem, NULL ) ) != NULL &&
											conclusionSpecificationItem->hasSpecificationCollection() &&
											( foundJustificationItem = conclusionSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != NULL )
												{
												// Additional Spanish feminine conclusion justification
												if( ( justificationResult = relationWordItem->addJustification( false, true, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, foundJustificationItem->secondarySpecificationItem(), NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", relationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												if( createdOrFoundSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the found specification" );
												}
											}
										}
									}
								}
							else
								{
								if( isPossessive &&
								existingSpecificationItem->isUserSpecification() &&
								( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, feminineSpecificationWordItem, relationWordItem ) ) != NULL &&
								foundSpecificationItem->secondarySpecificationJustificationItem( existingSpecificationItem ) == NULL )
									{
									// Additional justification (User). Test file "John - Anna (before family definition)"
									if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, ( existingSpecificationItem->hasAssumptionLevel() ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, existingSpecificationItem, NULL, foundSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

									if( foundSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the found specification" );
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
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasSimpleUserSpecificationCompoundCollection = false;
		bool isChineseCurrentLanguage;
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *feminineCollectionWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeSpecificationSubstitutionPartOfAssumption";

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
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
			isChineseCurrentLanguage = adminItem_->isChineseCurrentLanguage();
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			if( ( simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL )
				hasSimpleUserSpecificationCompoundCollection = simpleUserSpecificationItem->hasCompoundSpecificationCollection();

			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != NULL )
					{
					hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();

					if( ( currentGeneralizationWordItem->hasPartOfSpecification() ||

					( ( !hasCorrectedAssumptionByOppositeQuestion &&

					( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||
					!currentGeneralizationWordItem->hasCollection() ||

					// Typical for Chinese
					( isChineseCurrentLanguage &&
					hasPrimarySpecificationCompoundCollection &&
					hasSimpleUserSpecificationCompoundCollection &&
					generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
					!generalizationWordItem->isOlderItem() ) ) ) ) ) &&

					( ( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem ) ) == NULL ||
					// Skip if concluded conclusion already exists
					existingSpecificationItem != simpleUserSpecificationItem ) )
						{
						alternativeSpanishAnotherPrimarySpecificationItem_ = NULL;
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
							hasGeneralizationWordFeminineProperNounEnding &&
							( feminineCollectionWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL )
								secondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, feminineCollectionWordItem );

							if( secondarySpecificationItem == NULL )
								{
								// Try to find self-generated conclusion first
								if( ( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL ) ) == NULL )
									// Otherwise, try to find self-generated assumption
									tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );

								if( tempSpecificationItem != NULL &&

								( existingSpecificationItem == NULL ||
								tempSpecificationItem->isSelfGeneratedConclusion() ||
								// Skip assumption with relation word
								existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == NULL ) )
									secondarySpecificationItem = tempSpecificationItem;
								}
							}

						if( secondarySpecificationItem != NULL &&
						primarySpecificationItem->isExclusiveSpecification() &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Typical for Spanish
						existingSpecificationItem == NULL ) )
							{
							// Make generalization assumption
							if( makeGeneralizationAssumption( hasCorrectedAssumptionByKnowledge, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

							generalizationAssumptionWordItem_ = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( !globalVariables_->hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		// Definition specification
		if( generalizationWordTypeNr != WORD_TYPE_PROPER_NOUN &&
		generalizationAssumptionWordItem_ != NULL &&
		generalizationWordItem != generalizationAssumptionWordItem_ &&
		specificationWordItem != generalizationAssumptionWordItem_ &&
		( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
		( secondarySpecificationItem = specificationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != NULL &&

		// Make an exclusive specification substitution assumption
		// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
		adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isEveryGeneralization(), false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, generalizationAssumptionWordItem_, NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		return RESULT_OK;
		}

	SpecificationItem *alternativeJustificationSpecificationItem( bool hasGeneralizationWordFeminineProperNounEnding, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationItem *possessiveSpecificationItem;
		WordItem *userSpecificationWordItem;

		alternativeSpanishAnotherPrimarySpecificationItem_ = NULL;

		if( userSpecificationItem != NULL )
			{
			if( generalizationWordItem->isUserRelationWord &&
			userSpecificationItem->isPossessive() )
				return userSpecificationItem;

			if( !hasGeneralizationWordFeminineProperNounEnding &&
			( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL &&
			// Try to find a possessive specification
			( possessiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, true, false, userSpecificationWordItem ) ) != NULL )
				{
				alternativeSpanishAnotherPrimarySpecificationItem_ = oppositePossessiveDefinitionSpecificationItem_;
				return possessiveSpecificationItem;
				}
			}

		return NULL;
		}

	WordItem *collectionWordItemInCollectionWords( unsigned int compoundCollectionNr, WordItem *commonWordItem )
		{
		WordItem *currentCollectionWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		commonWordItem != NULL &&
		( currentCollectionWordItem = globalVariables_->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem->hasCollectionNr( compoundCollectionNr, commonWordItem ) )
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
		SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *foundPossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		SpecificationItem *previousPossessiveSpecificationItem = NULL;
		WordItem *higherLevelPossessiveSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findPossessiveSpecifications";

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
				possessiveSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) != NULL )
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
					if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( possessiveSpecificationWordItem == generalizationWordItem )
							return adminItem_->startFindSpecificationResultError( functionNameString, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );

						if( previousPossessiveSpecificationItem != NULL )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem->specificationCollectionNr();

							if( higherLevelPossessiveSpecificationWordItem->hasCollectionNr( possessiveSpecificationCollectionNr ) )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
							}

						previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
						}
					}
				while( foundPossessiveDefinitionSpecificationItem == NULL &&
				( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );
				}
			}

		findSpecificationResult.foundPossessiveDefinitionSpecificationItem = foundPossessiveDefinitionSpecificationItem;
		findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem = foundOppositePossessiveDefinitionSpecificationItem;
		return findSpecificationResult;
		}


	protected:
	// Constructor

	AdminReasoningOld( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasSuggestiveQuestionAssumption_ = false;

		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		alternativeSpanishAnotherPrimarySpecificationItem_ = NULL;
		oppositePossessiveDefinitionSpecificationItem_ = NULL;
		possessiveDefinitionSpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;

		feminineSpecificationWordItem_ = NULL;
		generalizationAssumptionWordItem_ = NULL;
		oppositePossessiveSpecificationWordItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminReasoningOld" );

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

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

		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;

		feminineSpecificationWordItem_ = NULL;
		generalizationAssumptionWordItem_ = NULL;
		}

	signed char checkForUnprocessedNegativeConclusion()
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForUnprocessedNegativeConclusion";

		if( postponeNegativeConclusionSpecificationCollectionNr_ > NO_COLLECTION_NR )
			{
			sprintf( errorString, "\nI found an unprocessed negative conclusion: postponeNegativeConclusionSpecificationCollectionNr_: %u;\n\tpostponeNegativeConclusionPrimarySpecificationItem_: ", postponeNegativeConclusionSpecificationCollectionNr_ );

			if( postponeNegativeConclusionPrimarySpecificationItem_ != NULL )
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, postponeNegativeConclusionPrimarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );

			strcat( errorString, ";\n\tpostponeNegativeConclusionAnotherPrimarySpecificationItem_: " );

			if( postponeNegativeConclusionAnotherPrimarySpecificationItem_ != NULL )
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, postponeNegativeConclusionAnotherPrimarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );

			strcat( errorString, ".\n" );

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
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeConclusionsFromAnsweredQuestions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( answeredQuestionSpecificationItem = generalizationWordItem->firstRecentlyAnsweredQuestionSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any answered question specification" );

		do	{
			if( userSpecificationWordItem->hasCollectionNr( answeredQuestionSpecificationItem->specificationCollectionNr() ) )
				{
				if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no specification word" );

				// Check if already exists as positive specification
				if( hasDisplayedMoreSpecificNonExclusiveSpecification ||
				generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, answeredQuestionSpecificationWordItem ) == NULL )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem->firstJustificationItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem->hasPossessivePrimarySpecification() &&
						// Draw negative conclusion from answered question
						drawNegativeConclusion( answeredQuestionSpecificationItem->isArchivedAssignment(), answeredQuestionSpecificationItem->generalizationWordTypeNr(), answeredQuestionSpecificationItem->specificationWordTypeNr(), answeredQuestionSpecificationItem->generalizationContextNr(), answeredQuestionSpecificationItem->specificationContextNr(), currentJustificationItem->primarySpecificationItem(), currentJustificationItem->secondarySpecificationItem(), userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
					}
				}
			}
		while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem->recentlyAnsweredQuestionSpecificationItem( false ) ) != NULL );

		return RESULT_OK;
		}

	signed char drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem )
		{
		bool hasFoundAssignment;
		bool hasMoreOptionsLeft = false;
		SpecificationItem *currentAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstNegativeOnlyOptionLeftAssignmentItem = NULL;
		SpecificationItem *firstOnlyOptionLeftSpecificationItem = NULL;
		SpecificationItem *foundOnlyOptionLeftSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawOnlyOptionLeftConclusion";

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
			foundOnlyOptionLeftSpecificationItem != NULL &&

			// Draw an only option left conclusion
			// Not yet explained in the design
			adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *feminineSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawPossessiveReversibleConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion" );

		// Typical for Spanish
		if( isPossessive &&
		!isArchivedAssignment &&
		specificationWordItem->isNounWordSpanishAmbiguous() &&
		relationWordItem->hasFeminineProperNounEnding() &&
		( feminineSpecificationWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL &&

		// Draw possessive reversible feminine conclusion
		drawPossessiveReversibleConclusion( false, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, feminineSpecificationWordItem, relationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible feminine conclusion" );

		return RESULT_OK;
		}

	signed char drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned int nonCompoundSpecificationCollectionNr;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem();
		WordItem *collectionWordItem;
		WordItem *commonWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSimpleNegativeSpanishConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( simpleUserSpecificationItem != NULL &&
		!specificationWordItem->isNounWordSpanishAmbiguous() &&
		( userSpecificationWordItem = simpleUserSpecificationItem->specificationWordItem() ) != NULL &&
		( nonCompoundSpecificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&
		( collectionWordItem = specificationWordItem->collectionWordItem( nonCompoundSpecificationCollectionNr ) ) != NULL )
			{
			if( collectionWordItem->isNounWordSpanishAmbiguous() )
				anotherPrimarySpecificationItem = collectionWordItem->firstSpecificationItem( false, false, false, collectionWordItem );
			else
				{
				if( ( commonWordItem = userSpecificationWordItem->commonWordItem( simpleUserSpecificationItem->specificationCollectionNr() ) ) != NULL &&
				commonWordItem->isNounWordSpanishAmbiguous() )
					anotherPrimarySpecificationItem = commonWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, false, true, NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, collectionWordItem );
				}

			if( anotherPrimarySpecificationItem != NULL &&
			anotherPrimarySpecificationItem->hasSpecificationCollection() &&

			askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned short assumptionLevel;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != NULL )
					{
					if( !currentSpecificationItem->hasSpecificationCollection() )
						{
						assumptionLevel = ( secondarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ? currentSpecificationItem->assumptionLevel() : secondarySpecificationItem->assumptionLevel() );

						// Collect generalization words of a specification-generalization definition
						if( adminItem_->collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						if( currentSpecificationItem->isReplacedItem() )
							currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

						if( currentSpecificationItem->hasCurrentCreationSentenceNr() &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification-generalization conclusion about an earlier sentence" );

						if( !isSelfGenerated &&
						drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification-generalization conclusion about the given sentence" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isArchivedAssignment, bool isExclusiveSpecification, bool isMakingPartOfAssumption, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool hasCurrentSpecificationCollection;
		bool hasCurrentSpecificationCompoundCollection;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasGeneralizationWordMasculineProperNounEnding;
		bool hasPrimarySpecificationNonCompoundCollection;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasUserSpecificationRelationContext = false;
		bool isArchivedUserAssignment = false;
		bool isDefinitionSpanishAmbiguous;
		bool isExclusiveCurrentSpecification;
		bool isExclusiveUserSpecification = false;
		bool isFemaleOrMaleGeneralizationWord;
		bool isFoundPrimaryHiddenSpanishSpecification;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isGeneralizationWordUserRelationWord;
		bool isIncludingAdjectives = false;
		bool isIncludingRelationWord;
		bool isNegativeConclusion = false;
		bool isPossessiveUserSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		bool isUserSpecificationWord;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned short currentSpecificationWordTypeNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *alternativeSpanishAnotherPrimarySpecificationItem;
		SpecificationItem *archivedAssignmentItem;
		SpecificationItem *bestMatchingRelationSpecificationItem;
		SpecificationItem *collectedWithItselfSpecificationItem = NULL;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *currentSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
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
			isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			isIncludingAdjectives = ( !isExclusiveSpecification &&
									!hasCorrectedAssumptionByKnowledge &&
									!userSpecificationItem->isQuestion() );
			}

		if( ( currentSpecificationItem = specificationWordItem->firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != NULL )
			{
			hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();

			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem->hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem->hasMasculineProperNounEnding();

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			isFemaleOrMaleGeneralizationWord = generalizationWordItem->isFemaleOrMale();
			isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isUserSpecificationWord = specificationWordItem->isUserSpecificationWord;

			if( isSpecificationWordSpanishAmbiguous )
				collectedWithItselfSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem )
					{
					isExclusiveCurrentSpecification = currentSpecificationItem->isExclusiveSpecification();
					currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();
					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > NO_COLLECTION_NR );

					if( ( bestMatchingRelationSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) ) != NULL &&
					!isQuestion &&
					!bestMatchingRelationSpecificationItem->isExclusiveSpecification() )
						isNegativeConclusion = true;

					if( !hasRelationWord ||
					!isPossessiveUserSpecification ||
					( foundPrimarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, specificationWordItem, relationWordItem ) ) == NULL )
						foundPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, isQuestion, specificationWordItem );

					if( foundPrimarySpecificationItem == NULL )
						{
						// Check for specification conflict
						if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, false, false, currentSpecificationCollectionNr, currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a specification conflict in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem->isHiddenSpanishSpecification();

						if( !globalVariables_->hasDisplayedWarning &&
						isExclusiveCurrentSpecification &&
						( feminineOrMasculineSpecificationItem = currentSpecificationWordItem->firstFeminineOrMasculineSpecificationItem() ) != NULL )
							{
							isIncludingRelationWord = ( hasRelationWord &&
														feminineOrMasculineSpecificationItem->hasSpecificationCollection() &&

														( isFoundPrimaryHiddenSpanishSpecification ||
														foundPrimarySpecificationItem->hasNewInformation() ) );

							if( !isFemaleOrMaleGeneralizationWord &&

							( isIncludingRelationWord ||

							( !hasRelationWord &&
							!isPossessiveUserSpecification ) ||

							( isPossessiveUserSpecification &&

							( !isUserGeneralizationWord ||
							foundPrimarySpecificationItem->isOlderItem() ) ) ) &&

							// Matching feminine word
							( ( ( hasGeneralizationWordFeminineProperNounEnding &&
							currentSpecificationWordItem->isFemale() ) ||

							// Matching masculine word
							( hasGeneralizationWordMasculineProperNounEnding &&
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
								if( isIncludingRelationWord ||

								( isUserGeneralizationWord &&

								( !hasUserSpecificationRelationContext ||
								foundPrimarySpecificationItem->hasNonCompoundSpecificationCollection() ) ) ||

								( ( hasRelationWord ||
								generalizationWordItem->isOlderItem() ) &&

								oppositePossessiveDefinitionSpecificationItem_ != NULL &&
								oppositePossessiveDefinitionSpecificationItem_->specificationCollectionNr() == userSpecificationCollectionNr ) )
									{
									// Make a feminine or masculine proper noun ending assumption
									// Not yet explained in the design
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem_ : NULL ), currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : NULL ) ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

									if( !isUserSpecificationWordSpanishAmbiguous )
										{
										if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
											{
											if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
											( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL &&
											( secondarySpecificationItem = firstJustificationItem->secondarySpecificationItem() ) != NULL )
												{
												// Additional justification
												if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, secondarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
												}

											// Draw missing negative conclusions
											if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isArchivedAssignment, isExclusiveSpecification, false, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								else
									clearPostponedNegativeConclusionVariables();
								}
							}

						if( ( bestMatchingRelationSpecificationItem == NULL &&

						( hasCurrentSpecificationCollection ||
						!foundPrimarySpecificationItem->hasSpecificationCollection() ) ) ||

						( !isExclusiveCurrentSpecification &&

						( isGeneralizationProperNoun ||

						( hasCurrentSpecificationCollection &&
						isDefinitionSpanishAmbiguous ) ) ) ||

						( isDefinitionSpanishAmbiguous &&
						hasCurrentSpecificationCollection &&
						!isUserSpecificationWord &&
						!currentSpecificationItem->isOlderItem() ) )
							{
							hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem->hasNonCompoundSpecificationCollection();

							if( ( !isExclusiveSpecification &&
							isExclusiveCurrentSpecification ) ||

							( isGeneralizationProperNoun &&

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
								hasCurrentSpecificationCompoundCollection = ( currentSpecificationWordItem->compoundCollectionNr() > NO_COLLECTION_NR );

								// Ask a question or draw a negative conclusion
								if( isGeneralizationProperNoun &&
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
								currentSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ) ||

								// Question
								hasCorrectedAssumptionByOppositeQuestion ||
								hasSuggestiveQuestionAssumption_ ) )
									{
									if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
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
									alternativeSpanishAnotherPrimarySpecificationItem = NULL;
									primarySpecificationItem = NULL;

									if( isSpecificationWordSpanishAmbiguous )
										primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );
									else
										{
										primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL );

										if( primarySpecificationItem == NULL ||
										primarySpecificationItem ->isUserAssignment() )
											primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );
										}

									existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, currentSpecificationWordItem );

									if( primarySpecificationItem != NULL )
										{
										alternativePrimarySpecificationItem = ( hasRelationWord ||
																				isSpecificationWordSpanishAmbiguous ? alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) : NULL );

										if( ( isSpecificationWordSpanishAmbiguous &&
										primarySpecificationItem->isHiddenSpanishSpecification() ) ||

										( existingSpecificationItem != NULL &&
										alternativePrimarySpecificationItem != NULL &&
										alternativePrimarySpecificationItem->specificationWordItem() == specificationWordItem &&

										( ( isSpecificationWordSpanishAmbiguous &&
										existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ) ||

										( !isSpecificationWordSpanishAmbiguous &&
										!existingSpecificationItem->hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&
										!specificationWordItem->isCollectionSpanishAmbiguous( alternativePrimarySpecificationItem->specificationCollectionNr() ) ) ) ) )
											{
											if( alternativePrimarySpecificationItem != NULL )
												alternativeSpanishAnotherPrimarySpecificationItem = alternativeSpanishAnotherPrimarySpecificationItem_;

											if( !isArchivedUserAssignment &&
											isSpecificationWordSpanishAmbiguous &&
											isGeneralizationWordUserRelationWord &&
											alternativeSpanishAnotherPrimarySpecificationItem != NULL &&
											existingSpecificationItem != NULL &&
											userSpecificationItem != NULL )
												{
												if( ( primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, NO_CONTEXT_NR, userSpecificationItem->specificationWordItem() ) ) != NULL &&
												// Check if justification already exists
												existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) )
													primarySpecificationItem = alternativePrimarySpecificationItem;
												}
											else
												primarySpecificationItem = alternativePrimarySpecificationItem;
											}
										else
											{
											if( isPossessiveUserSpecification &&
											primarySpecificationItem->hasAssumptionLevel() &&
											( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, currentSpecificationWordItem ) ) != NULL &&
											existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL )
												// Skip if similar justification already exists, without relation word
												primarySpecificationItem = NULL;
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

										if( isArchivedUserAssignment &&
										userSpecificationItem != NULL &&
										userSpecificationItem->isGeneralizationNoun() )
											{
											isArchivedAssignment = true;
											archivedAssignmentItem = userSpecificationItem;
											}

										// Make a compound specification substitution assumption, which is actually an uncertain conclusion
										// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
										//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
										if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, alternativeSpanishAnotherPrimarySpecificationItem, ( archivedAssignmentItem == NULL ? currentSpecificationItem : archivedAssignmentItem ), NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

										if( createAndAssignResult.createdSpecificationItem == NULL &&

										( createAndAssignResult.createdJustificationItem != NULL ||
										foundPrimarySpecificationItem->isOlderItem() ) &&

										// Cleanup unreferenced justifications
										generalizationWordItem->replaceOrDeleteUnreferencedJustifications() != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
									generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, true, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) )
										{
										if( ( primarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, specificationWordItem ) ) != NULL &&

										( isGeneralizationProperNoun ||

										( primarySpecificationItem->isUserSpecification() &&

										( !isExclusiveSpecification ||
										currentSpecificationWordItem->isFemaleOrMale() ||
										!currentSpecificationItem->hasCompoundSpecificationCollection() ||

										// Typical for Spanish
										( isSpanishCurrentLanguage &&
										currentSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, false, generalizationWordItem ) != NULL ) ) ) ) )
											{
											// Draw a compound specification substitution conclusion
											// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
											//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, ( currentSpecificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? WORD_TYPE_NOUN_SINGULAR : currentSpecificationWordTypeNr ), NO_WORD_TYPE_NR, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
												{
												if( hasCorrectedAssumptionByKnowledge )
													{
													if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created specification" );

													if( relatedResult.relatedSpecificationItem != NULL &&
													// Replace incorrect assumption
													generalizationWordItem->replaceOrDeleteSpecification( relatedResult.relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													if( isGeneralizationProperNoun )
														{
														if( isSpanishCurrentLanguage &&
														!hasRelationWord &&
														hasCurrentSpecificationCollection &&
														!isSpecificationWordSpanishAmbiguous &&
														isUserGeneralizationWord &&
														primarySpecificationItem->hasNonCompoundSpecificationCollection() &&

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
							isGeneralizationProperNoun &&
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

	signed char makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isUncountableGeneralizationNoun, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFeminineOrMasculineProperNounEnding;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasRelationWordConfirmedSpecification = false;
		bool hasRelationWordFeminineOrMasculineProperNounEnding = false;
		bool isGeneralizationWordUserRelationWord;
		bool isHiddenPrimarySpecification;
		bool isHiddenSecondarySpecification;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isReplaceHiddenSpecification;
		unsigned int compoundCollectionNr;
		unsigned int nonCompoundSpanishSpecificationCollectionNr;
		unsigned int relationContextNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *olderJustificationItem;
		JustificationItem *previousJustificationItem;
		SpecificationItem *additionalSecondarySpecificationItem;
		SpecificationItem *additionalSpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *compoundSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem;
		SpecificationItem *foundPossessiveDefinitionSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *possessiveSpecificationItem = NULL;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem = NULL;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *commonWordItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *feminineSpecificationWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		oppositePossessiveDefinitionSpecificationItem_ = NULL;
		possessiveDefinitionSpecificationItem_ = NULL;

		oppositePossessiveSpecificationWordItem_ = NULL;

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, relationWordItem );


		// Typical for Spanish
		if( specificationWordItem->isNounWordSpanishAmbiguous() &&
		relationWordItem != NULL &&
		primarySpecificationItem != NULL &&
		primarySpecificationItem->hasCompoundSpecificationCollection() &&
		primarySpecificationItem->isSelfGenerated() &&
		( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&
		( tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, nonCompoundSpanishSpecificationCollectionNr, specificationWordItem ) ) != NULL &&
		( relationContextNr = tempSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
		relationWordItem->hasContextInWord( relationContextNr, specificationWordItem ) )
			// Spanish non-compound specification
			primarySpecificationItem = tempSpecificationItem;

		if( primarySpecificationItem != NULL &&

		( !primarySpecificationItem->isOlderItem() ||

		( relationWordItem != NULL &&

		( primarySpecificationItem->isSelfGenerated() ||
		!relationWordItem->isOlderItem() ) ) ) &&

		( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
			isHiddenPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();

			if( relationWordItem != NULL )
				{
				hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();
				hasRelationWordFeminineOrMasculineProperNounEnding = relationWordItem->hasFeminineOrMasculineProperNounEnding();
				}

			// Do for all possessive noun words
			do	{
				if( currentPossessiveNounWordItem != specificationWordItem &&
				currentPossessiveNounWordItem != generalizationWordItem )
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

						if( ( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem->firstSpecificationItem( isPossessive, false, false, specificationWordItem ) ) != NULL )
							{
							isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();

							if( primarySpecificationItem != NULL &&

							( ( !primarySpecificationItem->hasRelationContext() &&
							generalizationWordItem->hadOnceAnsweredSelfGeneratedQuestion() ) ||

							( isPossessiveSpecificationWordSpanishAmbiguous &&
							primarySpecificationItem->specificationWordItem() == possessiveDefinitionSpecificationWordItem ) ) )
								{
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( true, isArchivedAssignment, isExclusiveSpecification, false, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != RESULT_OK )
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
								!existingUserSpecificationItem->hasSpecificationCollection() ) ) )
									{
									hasFeminineOrMasculineProperNounEnding = false;

									if( isPossessiveSpecificationWordSpanishAmbiguous &&
									isFeminineOrMasculineProperNounEnding( isPossessive, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) )
										hasFeminineOrMasculineProperNounEnding = true;

									if( !hasRelationWord &&
									!isNegative &&
									( possessiveSpecificationWordItem = possessiveSpecificationItem->specificationWordItem() ) != NULL &&
									( commonWordItem = possessiveSpecificationWordItem->commonWordItem( possessiveSpecificationItem->specificationCollectionNr() ) ) != NULL &&
									commonWordItem != generalizationWordItem &&
									commonWordItem->isFemaleOrMale() &&
									!commonWordItem->isNounWordSpanishAmbiguous() &&
									( definitionSpecificationItem = commonWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, possessiveSpecificationWordItem ) ) != NULL &&

									// Make generalization assumption
									makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, NO_CONTEXT_NR, definitionSpecificationItem, primarySpecificationItem, generalizationWordItem, commonWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", commonWordItem->anyWordTypeString(), "\"" );

									// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), foundPossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
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
											foundSpecificationItem->isSelfGeneratedConclusion() &&
											( createdJustificationItem = createAndAssignResult.createdJustificationItem ) != NULL &&
											( firstJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != NULL )
												{
												if( ( additionalSecondarySpecificationItem = firstJustificationItem->secondarySpecificationItem() ) != NULL )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, additionalSecondarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

													if( createdSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
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
					else
						{
						if( hasRelationWord &&
						primarySpecificationItem != NULL &&
						( foundSpecificationItem = currentPossessiveNounWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) ) != NULL &&

						// Uncountable generalization noun. Example: "Water ...".
						( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, primarySpecificationItem->isAssignment(), false, false, false, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentPossessiveNounWordItem->anyWordTypeString(), "\"" );
						}

					// First opposite possessive specification
					if( !globalVariables_->hasDisplayedWarning &&
					possessiveDefinitionSpecificationWordItem != NULL &&
					foundOppositePossessiveDefinitionSpecificationItem != NULL &&
					( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( isNegative ||
						createdOrFoundSpecificationItem == NULL ||

						( primarySpecificationItem != NULL &&
						primarySpecificationItem->hasSpecificationContext() ) )
							{
							secondarySpecificationItem = primarySpecificationItem;

							// Find another primary definition
							if( !hasRelationWord ||
							isPossessive )
								anotherPrimarySpecificationItem = possessiveDefinitionSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, specificationWordItem );
							}

						if( !isHiddenPrimarySpecification &&
						primarySpecificationItem != NULL &&
						secondarySpecificationItem != NULL &&

						( !primarySpecificationItem->isOlderItem() ||
						primarySpecificationItem->isSelfGenerated() ) )
							{
							isHiddenSecondarySpecification = secondarySpecificationItem->isHiddenSpanishSpecification();
							existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, false, oppositePossessiveSpecificationWordItem_ );

							if( !isHiddenSecondarySpecification &&
							hasGeneralizationWordCurrentlyConfirmedSpecification &&
							existingSpecificationItem != NULL &&
							existingSpecificationItem->hasCompoundSpecificationCollection() &&
							existingSpecificationItem->hasOnlyOneRelationWord() &&
							( tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, !isPossessive, false, foundOppositePossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) ) != NULL )
								existingSpecificationItem = tempSpecificationItem;

							if( ( existingSpecificationItem == NULL ||
							!existingSpecificationItem->hasRelationContext() ||

							( ( hasRelationWord &&
							isGeneralizationWordUserRelationWord &&
							isPossessive &&
							existingSpecificationItem->isSelfGeneratedAssumption() &&

							( hasRelationWordConfirmedSpecification ||
							!hasRelationWordFeminineOrMasculineProperNounEnding ||
							anotherPrimarySpecificationItem != NULL ) ) ||

							( secondarySpecificationItem->hasRelationContext() &&
							!secondarySpecificationItem->isSelfGeneratedConclusion() &&

							( !isHiddenSecondarySpecification ||

							( primarySpecificationItem->isUserSpecification() &&
							!generalizationWordItem->hasDisplayedMoreSpecificSpecification() ) ||

							// Typical for Spanish
							// Self-generated assignment
							primarySpecificationItem->isAssignment() ) &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							!existingSpecificationItem->isOlderItem() ||

							( anotherPrimarySpecificationItem == NULL &&

							( primarySpecificationItem == secondarySpecificationItem ||
							existingSpecificationItem->isSelfGeneratedAssumption() ) ) ||

							( relationWordItem != NULL &&
							!relationWordItem->isOlderItem() ) ) ) ) ) &&

							// User specification doesn't exist
							generalizationWordItem->firstUserSpecificationItem( isNegative, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == NULL &&
							// No justification exist as an opposite possessive conditional specification assumption
							generalizationWordItem->secondarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == NULL )
								{
								isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_->isNounWordSpanishAmbiguous();

								if( primarySpecificationItem->isReplacedItem() )
									primarySpecificationItem = primarySpecificationItem->updatedSpecificationItem();

								// Make an exclusive specification substitution assumption (opposite)
								// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), foundOppositePossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? ( isHiddenSecondarySpecification ? primarySpecificationItem : secondarySpecificationItem ) : foundOppositePossessiveDefinitionSpecificationItem ), ( !isReplaceHiddenSpecification && isOppositePossessiveSpecificationWordSpanishAmbiguous ? NULL : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? foundOppositePossessiveDefinitionSpecificationItem : secondarySpecificationItem ), NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

								if( isPossessive )
									{
									if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
										{
										if( hasRelationWord &&
										primarySpecificationItem->isSelfGenerated() &&
										( anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) ) != NULL )
											{
											hasFeminineOrMasculineProperNounEnding = false;
											feminineSpecificationWordItem = NULL;

											if( isFeminineOrMasculineProperNounEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) )
												{
												hasFeminineOrMasculineProperNounEnding = true;

												if( feminineSpecificationWordItem_ == NULL )
													{
													if( existingSpecificationItem == NULL )
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_->feminineCollectionWordItem();
													}
												else
													feminineSpecificationWordItem = feminineSpecificationWordItem_;

												if( feminineSpecificationWordItem != NULL &&
												( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, anotherPrimarySpecificationItem, NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine opposite exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
												}

											if( ( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, false, anotherPrimarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
											compoundSpecificationItem != primarySpecificationItem )
												{
												// Make a compound specification exclusive specification substitution assumption (opposite)
												// See paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
												if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, compoundSpecificationItem, ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

												if( feminineSpecificationWordItem_ != NULL &&
												( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : compoundSpecificationItem ), NULL, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine compound exclusive specification substitution assumption that is Spanish ambiguous in word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", feminineSpecificationWordItem_->anyWordTypeString(), "\"" );
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
												secondarySpecificationItem = foundSpecificationItem->updatedSpecificationItem();

												if( anotherPrimarySpecificationItem != NULL &&
												relationWordItem != NULL &&
												relationWordItem->isOlderItem() &&
												!globalVariables_->hasDisplayedMessage &&
												primarySpecificationItem->isUserSpecification() &&
												( additionalSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != NULL )
													{
													// Additional justification
													if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, anotherPrimarySpecificationItem, NULL, primarySpecificationItem, NULL, additionalSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

													if( additionalSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the found specification" );
													}
												}
											else	// Found and created
												{
												if( !foundSpecificationItem->isDeletedItem() )
													{
													if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "The found specification item has no first justification item" );

													// Replace the secondary specification of a justification
													if( ( obsoleteJustificationItem = primarySpecificationItem->secondarySpecificationJustificationItem( foundSpecificationItem ) ) != NULL )
														{
														if( ( justificationResult = generalizationWordItem->copyJustification( false, obsoleteJustificationItem->primarySpecificationItem(), createdSpecificationItem, obsoleteJustificationItem->attachedJustificationItem(), obsoleteJustificationItem ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy an obsolete justification item with a different secondary specification item" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
															return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't copy an obsolete justification" );

														// Replace obsolete justification
														if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, primarySpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}

													if( ( additionalSecondarySpecificationItem = firstJustificationItem->secondarySpecificationItem() ) != NULL )
														{
														// Additional justification
														if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, additionalSecondarySpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
															return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

														if( createdSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
														}
													else
														return adminItem_->startError( functionNameString, moduleNameString_, "The additional secondary specification item is undefined" );
													}
												}
											}
										}
									}
								else	// Non-possessive
									{
									if( hasRelationWord &&
									createdSpecificationItem != NULL &&
									existingSpecificationItem != NULL )
										{
										if( existingSpecificationItem->hasRelationContext() )
											{
											if( !existingSpecificationItem->isReplacedOrDeletedItem() )
												{
												if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
													{
													// Created specification was hidden
													if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( ( olderJustificationItem = existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) == NULL )
														olderJustificationItem = existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

													if( olderJustificationItem != NULL )
														{
														if( generalizationWordItem->attachJustificationInWord( olderJustificationItem, createdSpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach an older justification item to the created assumption in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

														if( ( obsoleteJustificationItem = createdSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != NULL &&
														( previousJustificationItem = createdSpecificationItem->previousJustificationItem( obsoleteJustificationItem ) ) != NULL )
															{
															// Replace obsolete justification from created specification
															if( previousJustificationItem->changeAttachedJustification( obsoleteJustificationItem->attachedJustificationItem() ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the attached justification item of the previous justification item" );

															if( generalizationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												else
													{
													if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( !existingSpecificationItem->isConcludedAssumption() &&
													// Cleanup unreferenced justifications
													generalizationWordItem->replaceOrDeleteUnreferencedJustifications() != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete unreferenced justifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										else
											{
											if( relationWordItem != NULL &&
											existingSpecificationItem->isReplacedItem() &&
											( conclusionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, NO_QUESTION_PARAMETER, createdSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
											!conclusionSpecificationItem->isOlderItem() )
												{
												// Additional justification
												if( ( justificationResult = generalizationWordItem->addJustification( false, true, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, foundPossessiveDefinitionSpecificationItem, NULL, conclusionSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the created specification" );
												}
											}
										}
									else
										{
										if( primarySpecificationItem->isReplacedItem() &&
										( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
											{
											primarySpecificationItem = primarySpecificationItem->updatedSpecificationItem();

											// Additional Spanish justification (User). Test Spanish file: "Complejo (15)"
											if( ( justificationResult = generalizationWordItem->addJustification( false, true, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, userSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
												return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

											if( primarySpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the primary specification" );
											}
										}
									}
								}
							}
						}
					}
				}
			while( !globalVariables_->hasDisplayedWarning &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );

			if( !globalVariables_->hasDisplayedWarning &&
			primarySpecificationItem != NULL &&
			secondarySpecificationItem != NULL &&
			relationWordItem != NULL )
				{
				if( primarySpecificationItem->isSpecificationWordSpanishAmbiguous() )
					{
					if( primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
					( compoundCollectionNr = specificationWordItem->compoundCollectionNr() ) > NO_COLLECTION_NR &&
					( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, compoundCollectionNr, primarySpecificationItem->relationContextNr(), specificationWordItem ) ) != NULL )
						primarySpecificationItem = compoundSpecificationItem;
					}
				else
					{
					if( existingSpecificationItem != NULL &&
					primarySpecificationItem == secondarySpecificationItem &&
					primarySpecificationItem->hasCompoundSpecificationCollection() &&
					!existingSpecificationItem->isSpecificationWordSpanishAmbiguous() &&
					relationWordItem->isUserGeneralizationWord )
						secondarySpecificationItem = existingSpecificationItem;
					}

				// Draw possessive reversible conclusions
				if( adminItem_->drawPossessiveReversibleConclusions( relationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, ( isHiddenPrimarySpecification ? adminItem_->userSpecificationItem() : primarySpecificationItem ), generalizationWordItem, relationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption during the making an exclusive specification substitution assumption in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem )
		{
		unsigned int currentSpecificationCollectionNr;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *indirectPrimarySpecificationItem;
		SpecificationItem *primarySpecificationItem;
		WordItem *commonWordItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *negativeQuestionSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

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
				( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
				( currentSpecificationCollectionNr = currentQuestionSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
					{
					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem->findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem->questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

					// Draw an indirectly answered question conclusion
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, indirectPrimarySpecificationItem, NULL, userSpecificationItem, NULL, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentQuestionSpecificationWordItem->anyWordTypeString(), "\"" );

					if( relatedResult.relatedSpecificationItem != NULL &&
					( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
					( negativeQuestionSpecificationWordItem = relatedResult.relatedSpecificationItem->specificationWordItem() ) != NULL &&
					( commonWordItem = currentQuestionSpecificationWordItem->commonWordItem( currentSpecificationCollectionNr ) ) != NULL )
						{
						if( commonWordItem->isNounWordSpanishAmbiguous() )
							{
							if( generalizationWordItem->replaceOrDeleteSpecification( currentQuestionSpecificationItem->updatedSpecificationItem(), createdSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the answered question by the created specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, commonWordItem ) ) != NULL &&
							( anotherPrimarySpecificationItem = commonWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, currentQuestionSpecificationWordItem ) ) != NULL &&

							// Draw a negative assumption
							// Not yet explained in the design
							( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, NULL, generalizationWordItem, negativeQuestionSpecificationWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeQuestionSpecificationWordItem->anyWordTypeString(), "\"" );
							}
						}

					currentQuestionSpecificationItem = adminItem_->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
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
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *onlyOptionLeftSpecificationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeOnlyOptionLeftAssumption";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( !userSpecificationItem->hasSpecificationCollection() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification collection" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != NULL )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL &&
				generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, currentQuestionSpecificationWordItem ) == NULL )
					{
					nOptionsLeft++;
					onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
					}
				}
			while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

			if( nOptionsLeft == 1 &&
			onlyOptionLeftSpecificationWordItem != NULL &&
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
				{
				// Do for all generalization noun specification words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != NULL &&
					( anotherPrimarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != NULL &&

					// Make an only option left assumption
					// Not yet explained in the design
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem, onlyOptionLeftSpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				while( !globalVariables_->hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		unsigned int secondarySpecificationCollectionNr;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

		// Check if specification doesn't exist yet
		if( generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == NULL )
			{
			if( secondarySpecificationCollectionNr > NO_COLLECTION_NR &&
			// Check if a related specification exists
			( relatedResult = generalizationWordItem->findRelatedSpecification( false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the secondary specification" );

			if( relatedResult.relatedSpecificationItem == NULL )
				{
				if( generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() )
					{
					if( adminItem_->correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem->questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					// Typical for Spanish:
					// Skip making suggestive assumption if specification word is Spanish ambiguous,
					// and if user question has more than one generalization words
					if( !secondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ||
					globalVariables_->nUserGeneralizationWords == 1 )
						{
						// Make a suggestive question assumption
						// Not yet explained in the design
						hasSuggestiveQuestionAssumption_ = true;

						if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	SpecificationItem *oppositePossessiveDefinitionSpecificationItem()
		{
		return oppositePossessiveDefinitionSpecificationItem_;
		}

	WordItem *possessiveDefinitionSpecificationWordItem()
		{
		if( possessiveDefinitionSpecificationItem_ != NULL )
			return possessiveDefinitionSpecificationItem_->specificationWordItem();

		return NULL;
		}

	CompoundResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		CompoundResultType compoundResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( specificationWordItem == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( true ) ) != NULL )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( foundSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, false, false, NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
					{
					if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem->hasAssumptionLevel(), foundSpecificationItem->isArchivedAssignment(), foundSpecificationItem->isExclusiveSpecification(), true, NO_QUESTION_PARAMETER, foundSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, NULL ) != RESULT_OK )
						return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

					if( !globalVariables_->hasDisplayedWarning &&
					foundSpecificationItem->hasSpecificationCollection() )
						{
						if( ( relatedResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );

						if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL )
							{
							if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->hasAssumptionLevel(), relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), true, relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem->specificationWordItem(), NULL ) != RESULT_OK )
								return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( !globalVariables_->hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return compoundResult;
		}
	};

/*************************************************************************
 *	"Praise him for his mighty works;
 *	praise his unequaled greatness!" (Psalm 150:2)
 *************************************************************************/
