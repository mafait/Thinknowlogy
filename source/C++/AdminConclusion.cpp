/*	Class:			AdminConclusion
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously draw grammar-based conclusions
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"

class AdminConclusion
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasFoundConflictWithUserSpecification_;
	bool isSpanishAmbiguousCompoundSpecification_;

	unsigned int postponeNegativeConclusionSpecificationCollectionNr_;

	SpecificationItem *alternativeAnotherPrimarySpecificationItem_;
	SpecificationItem *adjustedQuestionSecondarySpecificationItem_;
	SpecificationItem *adjustedQuestionSpecificationItem_;
	SpecificationItem *postponeNegativeConclusionPrimarySpecificationItem_;
	SpecificationItem *postponeNegativeConclusionAnotherPrimarySpecificationItem_;
	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem_;
	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;

	WordItem *adjustedQuestionWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void clearPostponedNegativeConclusionVariables()
		{
		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;
		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		}

	ResultType askSpecificationSubstitutionQuestion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *existingQuestionSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		bool hasCorrectedAssumptionByOppositeQuestion;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		bool isSecondarySpecificationCompoundCollection;
		bool isUserGeneralizationWord;
		bool isBlockingCommonWordOfCompoundCollection = false;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int secondaryRelationContextNr;
		unsigned int secondarySpecificationCollectionNr;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *adjustedQuestionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askSpecificationSubstitutionQuestion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr() ) == NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification collection" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();
		isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

		nonCompoundSpecificationCollectionNr = secondarySpecificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );
		secondaryRelationContextNr = secondarySpecificationItem->relationContextNr();
		isSecondarySpecificationCompoundCollection = secondarySpecificationWordItem->isCompoundCollection( secondarySpecificationCollectionNr, specificationWordItem );

		foundSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

		// Skip if already exists as specification
		if( hasCorrectedAssumptionByOppositeQuestion ||
		isPrimarySpecificationWordSpanishAmbiguous ||
		foundSpecificationItem == NULL ||

		( isSecondarySpecificationCompoundCollection &&
		foundSpecificationItem->isSelfGeneratedAssumption() ) )
			{
			if( isUserGeneralizationWord )
				{
				if( isPrimarySpecificationWordSpanishAmbiguous &&
				adjustedQuestionSpecificationItem_ == NULL &&
				existingQuestionSpecificationItem != NULL &&
				existingQuestionSpecificationItem->hasSpecificationCompoundCollection() &&
				existingQuestionSpecificationItem->specificationCollectionNr() != secondarySpecificationCollectionNr )
					adjustedQuestionSpecificationItem_ = generalizationWordItem->firstSpecificationItem( false, false, existingQuestionSpecificationItem->questionParameter(), secondarySpecificationWordItem );

				if( isSecondarySpecificationCompoundCollection &&

				( adjustedQuestionSpecificationItem_ != NULL ||

				( !isPrimarySpecificationWordSpanishAmbiguous &&
				nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR ) ) )
					{
					// Check for adjusted compound question
					if( ( specificationResult = generalizationWordItem->findQuestionToBeAdjustedByCompoundCollection( false, false, WORD_PARAMETER_SINGULAR_VERB_IS, ( isPrimarySpecificationWordSpanishAmbiguous && existingQuestionSpecificationItem != NULL ? existingQuestionSpecificationItem->specificationCollectionNr() : secondarySpecificationCollectionNr ), secondaryRelationContextNr, adjustedQuestionSpecificationItem_, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );

					if( ( adjustedQuestionSpecificationItem = specificationResult.adjustedQuestionSpecificationItem ) != NULL )
						{
						if( adjustedQuestionWordItem_ != NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "The adjusted question word item is already defined" );

						// Write adjusted compound question
						if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, adjustedQuestionSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						adjustedQuestionWordItem_ = generalizationWordItem;
						}
					}
				else
					{
					if( checkForConflictWithUserSpecification( generalizationWordItem, specificationWordItem, secondarySpecificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a conflict with the user specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					if( hasFoundConflictWithUserSpecification_ )
						isBlockingCommonWordOfCompoundCollection = true;
					else
						{
						if( nonCompoundSpecificationCollectionNr == NO_COLLECTION_NR )
							nonCompoundSpecificationCollectionNr = adminItem_->nonCompoundCollectionNrInAllWords( secondarySpecificationCollectionNr );

						if( generalizationWordItem->firstUserSpecificationItem( false, false, secondarySpecificationCollectionNr, NO_CONTEXT_NR, NULL ) != NULL ||

						( nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR &&
						generalizationWordItem->firstUserSpecificationItem( false, false, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) != NULL ) )
							isBlockingCommonWordOfCompoundCollection = true;
						}
					}
				}

			if( !commonVariables_->hasDisplayedWarning &&
			!isBlockingCommonWordOfCompoundCollection &&

			( isExclusiveSpecification ||
			hasCorrectedAssumptionByOppositeQuestion ||
			secondarySpecificationItem->isExclusiveSpecification() ) )
				{
				alternativeAnotherPrimarySpecificationItem_ = NULL;

				if( isPrimarySpecificationWordSpanishAmbiguous &&
				primarySpecificationItem->isHiddenSpanishSpecification() )
					primarySpecificationItem = alternativeJustificationSpecificationItem( generalizationWordItem->hasFeminineProperNameEnding(), true, false, false, false, adminItem_->userSpecificationItem(), generalizationWordItem, specificationWordItem );

				if( primarySpecificationItem != NULL &&

				( hasCorrectedAssumptionByOppositeQuestion ||
				!primarySpecificationItem->isOlderItem() ||
				primarySpecificationItem->isSelfGenerated() ) )
					{
					// Ask a specification substitution question
					// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, primarySpecificationItem->assumptionLevel(), JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem_, ( isPrimarySpecificationWordSpanishAmbiguous && isUserGeneralizationWord && adjustedQuestionSecondarySpecificationItem_ != NULL ? adjustedQuestionSecondarySpecificationItem_ : secondarySpecificationItem ), NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a self-generated specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

					if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
						{
						if( isPrimarySpecificationWordSpanishAmbiguous &&
						isUserGeneralizationWord &&
						adjustedQuestionSecondarySpecificationItem_ == NULL &&
						specificationResult.foundSpecificationItem != NULL &&
						!primarySpecificationItem->isPossessive() &&
						secondarySpecificationItem->hasSpecificationCompoundCollection() )
							{
							adjustedQuestionSecondarySpecificationItem_ = secondarySpecificationItem;
							adjustedQuestionSpecificationItem_ = specificationResult.foundSpecificationItem->updatedSpecificationItem();
							}
						}
					else
						{
						// Has created question part
						// Change specification to question part
						if( foundSpecificationItem != NULL &&
						secondarySpecificationWordItem != specificationWordItem )
							{
							if( generalizationWordItem->replaceOrDeleteSpecification( false, foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

							if( createdSpecificationItem->isCorrectedAssumption() &&
							( obsoleteJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, createdSpecificationItem ) ) != NULL )
								{
								// Remove justification from corected assumption
								if( generalizationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an obsolete justification item" );
								}
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	ResultType askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegativeConclusion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
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

		if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		isCommonWordWordSpanishAmbiguous = commonWordItem->isNounWordSpanishAmbiguous();
		isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();
		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		relatedSpecificationItem = specificationResult.relatedSpecificationItem;

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		relatedSpecificationItem == NULL &&
		!userSpecificationItem->isPossessive() &&
		( nonCompoundCollectionNr = commonWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
			{
			if( nonCompoundCollectionNr != specificationCollectionNr )
				{
				if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, nonCompoundCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

				relatedSpecificationItem = specificationResult.relatedSpecificationItem;
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

			// Negative feminine conclusion
			if( isFemaleUserSpecificationWord ||
			isMaleUserSpecificationWord ||

			( hasGeneralizationWordMasculineProperNameEnding &&
			anotherPrimarySpecificationWordItem->isFemale() ) ||

			// Negative masculine conclusion
			( hasGeneralizationWordFeminineProperNameEnding &&
			anotherPrimarySpecificationWordItem->isMale() ) ||

			// Specification is Spanish ambiguous
			( isSpecificationWordSpanishAmbiguous &&
			!generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion() &&

			( hasGeneralizationWordFeminineProperNameEnding ||
			adminItem_->isUserQuestion() ) ) )
				{
				if( isFemaleUserSpecificationWord ||
				isMaleUserSpecificationWord )
					{
					if( anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() &&

					( anotherPrimarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ||

					( isFemaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem->isMale() ) ||

					( isMaleUserSpecificationWord &&
					anotherPrimarySpecificationWordItem->isFemale() ) ) )
						{
						if( drawNegativeConclusion( isArchivedAssignment, isSpecificationWordSpanishAmbiguous, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
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
							collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr, NULL );
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
								if( askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, NULL, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

								if( !primarySpecificationItem->hasSpecificationCollection() )
									{
									if( generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an obsolete justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
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

				if( postponeNegativeConclusionSpecificationCollectionNr_ == anotherPrimarySpecificationCollectionNr )
					{
					if( drawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, postponeNegativeConclusionPrimarySpecificationItem_, postponeNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}

				clearPostponedNegativeConclusionVariables();

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
						if( askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, questionSpecificationItem, primarySpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );
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

						if( isSpanishAmbiguousCompoundSpecification_ )
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

	ResultType checkForConflictWithUserSpecification( WordItem *generalizationWordItem, WordItem *primarySpecificationWordItem, WordItem *questionSpecificationWordItem )
		{
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForConflictWithUserSpecification";

		hasFoundConflictWithUserSpecification_ = false;

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification word item is undefined" );

		if( questionSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given question specification word item is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			do	{
				if( !currentSpecificationItem->isExclusiveSpecification() &&
				!currentSpecificationItem->isNegative() &&
				!currentSpecificationItem->isPossessive() &&
				// Current sentence
				!currentSpecificationItem->isOlderItem() &&
				// User sentence
				!currentSpecificationItem->isSelfGenerated() &&
				( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
					{
					foundSpecificationItem = currentSpecificationWordItem->firstAssignmentOrSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, questionSpecificationWordItem );

					if( ( foundSpecificationItem == NULL ||

					( !foundSpecificationItem->isExclusiveSpecification() &&
					!foundSpecificationItem->isNegative() ) ) )
						hasFoundConflictWithUserSpecification_ = true;
					}
				}
			while( !hasFoundConflictWithUserSpecification_ &&
			( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType drawNegativeConclusion( bool isArchivedAssignment, bool isSpecificationWordSpanishAmbiguous, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationResultType specificationResult;
		bool isGeneralizationWordUserRelationWord;
		bool isHiddenSpanishPrimarySpecification;
		bool isPrimarySameAsSecondarySpecification;
		bool isUserGeneralizationWord;
		bool hasAnotherSecondarySpecificationWithCompoundCollection = false;
		bool hasFoundOldSpanishUserSpecification = false;
		bool hasRelatedSpecificationsInSameSentence = false;
		bool isCompoundUserSpecification = false;
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

		isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;
		isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();

		secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

		existingPositiveSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

		if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != NULL )
			foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem->primarySpecificationItem();

		// No positive specification exists with this specification word
		if( ( existingPositiveSpecificationItem == NULL ||
		existingPositiveSpecificationItem->isHiddenSpanishSpecification() ) &&

		// Prefer primary specification with the least uncertainty
		( foundNegativeAssumptionOrConclusionPrimarySpecificationItem == NULL ||
		!primarySpecificationItem->hasSpecificationCollection() ||
		primarySpecificationItem->assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem->assumptionLevel() ) &&

		// Select correct compound specification collection when Spanish ambiguous
		( !isSpecificationWordSpanishAmbiguous ||
		isGeneralizationWordUserRelationWord ||
		anotherPrimarySpecificationWordItem->compoundCollectionNr( specificationWordTypeNr ) == anotherPrimarySpecificationCollectionNr ||

		( anotherPrimarySpecificationItem != NULL &&
		anotherPrimarySpecificationItem->hasSpecificationNonCompoundCollection() ) ) &&

		// No question exists with this specification word
		generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) == NULL )
			{
			if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

			isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			if( isPrimarySameAsSecondarySpecification )
				{
				if( primarySpecificationItem->hasSpecificationCompoundCollection() &&
				primarySpecificationItem->specificationCollectionNr() != anotherPrimarySpecificationCollectionNr )
					{
					collectionWordItem = collectionWordItemInAllWords( anotherPrimarySpecificationCollectionNr, primarySpecificationWordItem );

					if( collectionWordItem != NULL &&
					( anotherSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, collectionWordItem ) ) == NULL )
						{
						if( ( anotherSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == NULL )
							{
							if( userSpecificationItem->isSpecificationWordSpanishAmbiguous() )
								anotherSecondarySpecificationItem = primarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, collectionWordItem );
							else
								anotherSecondarySpecificationItem = userSpecificationItem;
							}
						}
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
					isGeneralizationWordUserRelationWord &&
					anotherPrimarySpecificationItem != NULL &&
					anotherPrimarySpecificationItem->hasSpecificationCompoundCollection() &&
					anotherPrimarySpecificationItem->isSelfGenerated() &&
					( collectionWordItem = anotherPrimarySpecificationWordItem->collectionWordItem( anotherPrimarySpecificationCollectionNr, NULL ) ) != NULL )
						{
						if( ( nonCompoundCollectionNr = collectionWordItem->nonCompoundCollectionNr( specificationWordTypeNr ) ) == NO_COLLECTION_NR )
							nonCompoundCollectionNr = anotherPrimarySpecificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );

						if( nonCompoundCollectionNr > NO_COLLECTION_NR &&
						( tempSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, nonCompoundCollectionNr, generalizationContextNr, NO_CONTEXT_NR, NULL ) ) != NULL )
							{
							hasFoundOldSpanishUserSpecification = true;
							secondarySpecificationItem = tempSpecificationItem;
							}
						}

					if( !hasFoundOldSpanishUserSpecification &&
					( anotherSecondarySpecificationItem = secondarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, NULL ) ) == NULL )
						{
						if( secondarySpecificationItem->hasSpecificationCompoundCollection() &&
						secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
						( secondarySpecificationCommonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL )
							{
							if( secondarySpecificationCommonWordItem != primarySpecificationWordItem )
								anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, secondarySpecificationWordItem );
							}
						}
					}
				}

			if( isUserGeneralizationWord )
				{
				if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
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
						oppositePossessiveDefinitionSpecificationItem = adminItem_->oppositePossessiveDefinitionSpecificationItem();
					}
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

			( isUserGeneralizationWord &&
			isUserSpecificationSelected &&

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
						isSpanishAmbiguousCompoundSpecification_ = true;

						negativeConclusionPrimarySpecificationItem = compoundGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
						negativeConclusionAnotherPrimarySpecificationItem = NULL;
						negativeConclusionSecondarySpecificationItem = anotherSecondarySpecificationItem;
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

				// Draw a negative negativeConclusion
				// Not yet explained in the design
				if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isExclusiveSpecification(), false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationWordSpanishAmbiguous && primarySpecificationItem->isSelfGeneratedAssumption() ? JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, NULL ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", anotherPrimarySpecificationWordItem->anyWordTypeString(), "\"" );

				createdJustificationItem = specificationResult.createdJustificationItem;

				// Found specification
				if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
					{
					if( createdJustificationItem != NULL )
						{
						// Cleanup after found specification
						if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "The found specification item is undefined" );

						if( negativeAssumptionOrConclusionJustificationItem == NULL )
							{
							if( foundSpecificationItem->isSelfGeneratedAssumption() )
								{
								if( createdJustificationItem->isConclusionJustification() )
									{
									if( foundSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

									if( isArchivedAssignment )
										{
										if( ( negativeArchivedAssignmentItem = generalizationWordItem->firstAssignmentItem( false, false, true, true, false, true, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, anotherPrimarySpecificationWordItem ) ) == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a past tense negative assumption assignment" );

										// Also mark the past tense negative assumption assignment as concluded
										if( negativeArchivedAssignmentItem->markAsConcludedAssumption() != RESULT_OK )
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
									negativeAssumptionJustificationItem->primarySpecificationItem() == negativeConclusionPrimarySpecificationItem )
										{
										if( generalizationWordItem->replaceJustification( negativeAssumptionJustificationItem, createdJustificationItem, foundSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
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
						if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created negative conclusion" );

						relatedSpecificationItem = specificationResult.relatedSpecificationItem;

						if( relatedSpecificationItem != NULL &&
						relatedSpecificationItem->isOlderItem() )
							{
							if( generalizationWordItem->replaceOrDeleteSpecification( false, relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						{
						if( isCompoundUserSpecification &&
						!isPrimarySameAsSecondarySpecification &&
						anotherSecondarySpecificationItem != NULL &&
						// Check for answered question justification
						createdSpecificationItem->answeredQuestionJustificationItem() == NULL )
							{
							if( generalizationWordItem->changeJustificationOfNegativeAssumptions( userSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a question justification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	ResultType drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
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
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *collectedWithItselfCommonWordItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( relationContextNr == NO_CONTEXT_NR )
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

			if( isUserGeneralizationWord &&
			!isSecondarySpecificationWordSpanishAmbiguous &&

			( existingSpecificationItem == NULL ||
			commonVariables_->nUserRelationWords > 1 ) &&

			( foundRelationContextNr = generalizationWordItem->contextNr( specificationWordItem ) ) > NO_CONTEXT_NR )
				{
				if( ( nContextRelations = adminItem_->nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
					{
					if( ( contextResult = adminItem_->getRelationContext( isArchivedAssignment, !isPossessive, false, false, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the relation context number" );

					generalizationContextNr = contextResult.contextNr;
					}
				else
					generalizationContextNr = foundRelationContextNr;
				}

			conclusionSpecificationItem = contextResult.conclusionSpecificationItem;
			existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Find the non-reversible specification (active assignments)
			if( isSecondarySpecificationWordSpanishAmbiguous )
				secondarySpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );
			else
				secondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, secondarySpecificationWordItem );

			// Skip if possessive user specification exists
			if( secondarySpecificationItem != NULL &&

			( !isSecondarySpecificationWordSpanishAmbiguous ||
			secondarySpecificationItem->hasSpecificationNonCompoundCollection() ||
			existingUserSpecificationItem == NULL ||
			!existingUserSpecificationItem->hasRelationContext() ) )
				{
				if( ( existingSpecificationItem == NULL ||
				secondarySpecificationItem->isUserSpecification() ||
				relationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, secondarySpecificationItem ) == NULL ) &&

				secondarySpecificationItem->hasNewInformation() )
					{
					hasFeminineProperNameEnding = ( isPossessive &&
													isSecondarySpecificationWordSpanishAmbiguous &&
													relationWordItem->hasFeminineProperNameEnding() );

					isConclusionAssignment = ( ( isArchivedAssignment &&
												isAssumption ) ||

												( !isAssumption &&

												( isArchivedAssignment ||
												secondarySpecificationItem->hasExclusiveGeneralizationCollection() ) ) );

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

					// Typically for the Spanish language
					if( isSecondarySpecificationWordSpanishAmbiguous &&
					secondarySpecificationWordItem == specificationWordItem &&

					( !isPossessive ||
					!relationWordItem->isMale() ) )
						spanishAmbiguousSpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

					// Draw a possessive reversible conclusion
					// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
					if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( hasFeminineProperNameEnding, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, isUniqueUserRelation, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, NULL, NULL, secondarySpecificationItem, NULL, relationWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

					if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != NULL )
						foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

					if( conclusionSpecificationItem == NULL )
						{
						if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == NULL )
							{
							if( existingSpecificationItem != NULL &&
							foundSpecificationItem != NULL )
								{
								if( existingSpecificationItem->isConcludedAssumption() )
									isReplacingObsoleteJustifications = true;
								else
									{
									if( !isSecondarySpecificationWordSpanishAmbiguous &&
									existingSpecificationItem->isSelfGeneratedAssumption() &&
									foundSpecificationItem->isSelfGeneratedConclusion() &&
									foundSpecificationItem->hasSpecificationCompoundCollection() )
										{
										if( relationWordItem->replaceOrDeleteSpecification( false, existingSpecificationItem, foundSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption specification by a conclusion in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

										isReplacingObsoleteJustifications = true;
										}
									}

								if( isReplacingObsoleteJustifications )
									{
									if( relationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace obsolete justification items in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						else
							{
							if( !isPossessive &&
							secondarySpecificationItem->isUserSpecification() &&
							createdSpecificationItem->isOlderItem() &&
							createdSpecificationItem->isSelfGeneratedAssumption() &&
							createdSpecificationItem->hasOnlyOneRelationWord() )
								{
								if( createdSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

								if( relationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, createdSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the concluded assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
								}
							}

						if( isPossessive &&
						( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? foundSpecificationItem : createdSpecificationItem ) ) != NULL )
							{
							if( !createdOrFoundSpecificationItem->hasSpecificationCompoundCollection() )
								{
								if( adminItem_->makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, NULL, generalizationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}

							if( isSecondarySpecificationWordSpanishAmbiguous &&
							( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem->collectedWithItselfCommonWordItem() ) != NULL )
								{
								// If the reversed specification doesn't already exist
								if( !isArchivedAssignment &&
								generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, true, NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) == NULL )
									{
									// Generate the reversed specification
									if( drawPossessiveReversibleConclusion( false, isExclusiveSpecification, false, false, createdOrFoundSpecificationItem->assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdOrFoundSpecificationItem->relationContextNr(), relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}

								if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
									{
									do	{
										if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );

										partOfSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem );

										if( partOfSpecificationItem != NULL &&
										partOfSpecificationItem->isPartOf() &&

										( !createdOrFoundSpecificationItem->hasAssumptionLevel() ||
										// Skip if 'part of' conclusion exists
										relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, currentGeneralizationWordItem, NULL ) == NULL ) )
											{
											// Draw a 'part of' generalization conclusion
											// Not yet explained in the design
											if( adminItem_->addSelfGeneratedSpecification( false, isConclusionAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, partOfSpecificationItem, NULL, relationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
											}
										}
									while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
									}

								if( relationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) == NULL &&
								( secondarySpecificationItem = collectedWithItselfCommonWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, createdOrFoundSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
									{
									if( drawNegativeConclusion( isArchivedAssignment, false, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationItem, NULL, createdOrFoundSpecificationItem, NULL, relationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about relation word \"", relationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}

						if( isUserGeneralizationWord &&
						specificationWordItem->isUserSpecificationWord )
							{
							if( isSecondarySpecificationWordSpanishAmbiguous )
								{
								existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );

								if( existingSpecificationItem != NULL &&
								existingSpecificationItem->isHiddenSpanishSpecification() &&
								existingSpecificationItem->specificationCollectionNr() != spanishAmbiguousSpecificationCollectionNr )
									{
									// Add context to hidden specification
									if( generalizationWordItem->addContext( false, relationWordTypeNr, specificationWordTypeNr, existingSpecificationItem->relationContextNr(), specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									// If not hidden anymore
									if( !existingSpecificationItem->isHiddenSpanishSpecification() )
										{
										if( relationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, existingSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the hidden assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}

							if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else
						{
						if( foundSpecificationItem != NULL &&
						foundSpecificationItem->isSelfGeneratedAssumption() )
							{
							if( foundSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the replacing specification as concluded assumption in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

							// Write adjusted specification
							if( relationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, foundSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

							if( ( firstJustificationItem = conclusionSpecificationItem->firstJustificationItem() ) == NULL )
								return adminItem_->startError( functionNameString, moduleNameString_, "The conclusion specification item has no justifications" );

							if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != NULL )
								{
								// Attach older conclusion justifications to the created conclusion justification item
								if( createdJustificationItem->changeAttachedJustification( firstJustificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach older conclusion justification items to the created conclusion justification item" );

								if( relationWordItem->replaceOrDeleteSpecification( false, conclusionSpecificationItem, foundSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a specification in relation word \"", relationWordItem->anyWordTypeString(), "\"" );

								firstJustificationItem = foundSpecificationItem->firstJustificationItem();

								if( firstJustificationItem != NULL &&
								!firstJustificationItem->isPossessiveReversibleConclusion() )
									{
									if( firstJustificationItem->changeAttachedJustification( createdJustificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete obsolete assumption justification items in relation word \"", relationWordItem->anyWordTypeString(), "\"" );
									}

								if( relationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	ResultType drawSpecificationGeneralizationConclusion( unsigned short assumptionLevel, SpecificationItem *secondarySpecificationItem )
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
		if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, secondarySpecificationItem->isExclusiveSpecification(), false, secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPartOf(), secondarySpecificationItem->isPossessive(), false, false, true, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, secondarySpecificationItem->specificationWordItem(), secondarySpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );

		return RESULT_OK;
		}

	WordItem *collectionWordItemInAllWords( unsigned int compoundCollectionNr, WordItem *commonWordItem )
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


	protected:
	// Constructor

	AdminConclusion( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundConflictWithUserSpecification_ = false;
		isSpanishAmbiguousCompoundSpecification_ = false;

		postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

		alternativeAnotherPrimarySpecificationItem_ = NULL;
		adjustedQuestionSecondarySpecificationItem_ = NULL;
		adjustedQuestionSpecificationItem_ = NULL;
		postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
		postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;

		adjustedQuestionWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminConclusion" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeAdminConclusionVariables()
		{
		clearPostponedNegativeConclusionVariables();

		hasFoundConflictWithUserSpecification_ = false;
		isSpanishAmbiguousCompoundSpecification_ = false;

		adjustedQuestionSecondarySpecificationItem_ = NULL;
		adjustedQuestionSpecificationItem_ = NULL;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;

		adjustedQuestionWordItem_ = NULL;
		}

	bool hasUnprocessedNegativeConclusion()
		{
		return ( postponeNegativeConclusionSpecificationCollectionNr_ > NO_COLLECTION_NR );
		}

	ResultType drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
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
				if( generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == NULL )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem->firstJustificationItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem->hasPrimaryQuestion() &&
						!currentJustificationItem->hasPossessivePrimarySpecification() )
							{
							if( drawNegativeConclusion( answeredQuestionSpecificationItem->isArchivedAssignment(), false, answeredQuestionSpecificationItem->generalizationWordTypeNr(), answeredQuestionSpecificationItem->specificationWordTypeNr(), answeredQuestionSpecificationItem->generalizationContextNr(), answeredQuestionSpecificationItem->specificationContextNr(), currentJustificationItem->primarySpecificationItem(), currentJustificationItem->secondarySpecificationItem(), userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
					}
				}
			}
		while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem->recentlyAnsweredQuestionSpecificationItem( false ) ) != NULL );

		return RESULT_OK;
		}

	ResultType drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem )
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

		if( specificationCollectionNr == NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification collection number is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

			do	{
				if( currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
				( currentAssignmentItem = generalizationWordItem->firstSelectedSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != NULL )
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
				if( adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	ResultType drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
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

	ResultType drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
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
			( collectionWordItem = specificationWordItem->collectionWordItem( nonCompoundCollectionNr, NULL ) ) != NULL )
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
				anotherPrimarySpecificationItem->hasSpecificationCollection() )
					{
					if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		return RESULT_OK;
		}

	ResultType drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isPossessive, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
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
					return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem ) ) != NULL )
					{
					if( !currentSpecificationItem->hasSpecificationCollection() &&
					!currentSpecificationItem->isExclusiveSpecification() )
						{
						assumptionLevel = ( secondarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ? currentSpecificationItem->assumptionLevel() : secondarySpecificationItem->assumptionLevel() );

						if( adminItem_->collectGeneralizationWordWithPreviousOne( false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

						if( currentSpecificationItem->hasCurrentCreationSentenceNr() )
							{
							if( drawSpecificationGeneralizationConclusion( assumptionLevel, currentSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );
							}

						if( drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isMakingPartOfAssumption, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
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
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		bool isExclusiveUserSpecification = false;
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		bool isNegativeConclusion = false;
		bool isPossessiveUserSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned int currentSpecificationCollectionNr;
		unsigned int relationContextNr;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		SpecificationItem *alternativeAnotherPrimarySpecificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *archivedAssignmentItem;
		SpecificationItem *bestMatchingRelationSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *collectedWithItselfSpecificationItem = NULL;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a reasoning type" );

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( isMakingPartOfAssumption )
			{
			if( adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
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

			oppositePossessiveDefinitionSpecificationItem = adminItem_->oppositePossessiveDefinitionSpecificationItem();

			if( isSpecificationWordSpanishAmbiguous )
				collectedWithItselfSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, specificationWordItem );

			if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
				{
				isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
				isPossessiveUserSpecification = userSpecificationItem->isPossessive();
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
				userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
				}

			do	{
				// Select non-possessive definition
				if( !currentSpecificationItem->isPossessive() &&
				currentSpecificationItem->isSpecificationNoun() &&
				!currentSpecificationItem->isSpecificationGeneralization() &&
				( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
					{
					if( currentSpecificationWordItem != generalizationWordItem )
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
							if( generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
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

								if( ( !hasRelationWord ||
								isIncludingRelationWord ||
								isPossessiveUserSpecification ) &&

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
										oppositePossessiveDefinitionSpecificationItem == NULL ) ||

										( oppositePossessiveDefinitionSpecificationItem != NULL &&
										oppositePossessiveDefinitionSpecificationItem->specificationCollectionNr() == userSpecificationCollectionNr ) )
											{
											// Make a feminine or masculine proper name ending assumption
											// Not yet explained in the design
											if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem : NULL ), currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : NULL ) ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

											if( specificationResult.createdSpecificationItem == NULL )
												{
												if( generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justification items in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											else
												{
												if( !isUserSpecificationWordSpanishAmbiguous &&

												// Has missed drawing some negative conclusions
												( postponeNegativeConclusionSpecificationCollectionNr_ == NO_COLLECTION_NR ||
												postponeNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr ) )
													{
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
									( relationContextNr = bestMatchingRelationSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR )
										{
										if( drawPossessiveReversibleConclusion( false, false, false, false, NO_ASSUMPTION_LEVEL, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
										}
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
									adminItem_->hasSuggestiveQuestionAssumption() ) )
										{
										if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isExclusiveSpecification, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									{
									// Make an assumption or draw a conclusion
									if( isAssumption ||
									currentSpecificationItem->hasAssumptionLevel() )
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
											if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem, ( archivedAssignmentItem == NULL ? currentSpecificationItem : archivedAssignmentItem ), NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

											if( specificationResult.createdSpecificationItem == NULL &&

											( specificationResult.createdJustificationItem != NULL ||
											!primarySpecificationItem->hasRelationContext() ||
											foundPrimarySpecificationItem->isOlderItem() ) )
												{
												if( generalizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete compound justification items in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
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
											primarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, isArchivedAssignment, false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

											if( primarySpecificationItem != NULL &&
											primarySpecificationItem->isHiddenSpanishSpecification() &&
											( primarySpecificationItem = alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, false, false, userSpecificationItem, generalizationWordItem, specificationWordItem ) ) != NULL )
												alternativeAnotherPrimarySpecificationItem = alternativeAnotherPrimarySpecificationItem_;

											if( primarySpecificationItem == NULL )
												return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );

											if( !primarySpecificationItem->isPartOf() )
												{
												// Draw a compound specification substitution conclusion
												// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
												//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
												if( ( specificationResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, primarySpecificationItem->isEveryGeneralization(), isExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

												if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
													{
													if( generalizationWordItem->hasCorrectedAssumptionByKnowledge() )
														{
														if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, createdSpecificationItem ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the created specification" );

														if( specificationResult.relatedSpecificationItem != NULL )
															{
															// Replace incorrect assumption
															if( generalizationWordItem->replaceOrDeleteSpecification( false, specificationResult.relatedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
															}
														}
													else
														{
														if( isGeneralizationProperName )
															{
															if( !hasRelationWord &&
															hasCurrentSpecificationCollection &&
															!isExclusiveSpecification &&
															!isSpecificationWordSpanishAmbiguous &&
															isUserGeneralizationWord &&
															primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
															adminItem_->isSpanishCurrentLanguage() )
																{
																if( drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														else
															{
															if( adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
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
								hasCurrentSpecificationCollection )
									{
									if( adminItem_->makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption by generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType findSpecificationResult;
		SpecificationResultType returnSpecificationResult;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *previousSpecificationItem = NULL;
		SpecificationItem *lastCreatedSpecificationItem = NULL;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( specificationWordItem == NULL )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		do	{
			if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( true ) ) != NULL )
				{
				previousSpecificationItem = lastCreatedSpecificationItem;

				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );

					if( ( foundSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
						{
						if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem->hasAssumptionLevel(), foundSpecificationItem->isInactiveAssignment(), foundSpecificationItem->isArchivedAssignment(), foundSpecificationItem->isExclusiveSpecification(), true, NO_QUESTION_PARAMETER, foundSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( !commonVariables_->hasDisplayedWarning )
							{
							if( ( findSpecificationResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
								return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to find out if a specification in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );

							if( ( relatedSpecificationItem = findSpecificationResult.relatedSpecificationItem ) != NULL )
								{
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->hasAssumptionLevel(), relatedSpecificationItem->isInactiveAssignment(), relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), true, relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem->specificationWordItem(), NULL ) != RESULT_OK )
									return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

								returnSpecificationResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
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

		return returnSpecificationResult;
		}

	SpecificationItem *alternativeAnotherPrimarySpecificationItem()
		{
		return alternativeAnotherPrimarySpecificationItem_;
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
					alternativeAnotherPrimarySpecificationItem_ = adminItem_->oppositePossessiveDefinitionSpecificationItem();
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

	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem()
		{
		return spanishAmbiguousCompoundPrimarySpecificationItem_;
		}

	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem()
		{
		return spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
		}

	WordItem *adjustedQuestionWordItem()
		{
		return adjustedQuestionWordItem_;
		}
	};

/*************************************************************************
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *************************************************************************/
