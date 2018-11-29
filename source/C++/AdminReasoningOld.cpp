/*	Class:			AdminReasoningOld
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (old design)
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

	unsigned int postponeNegativeConclusionSpecificationCollectionNr_ = NO_COLLECTION_NR;

	SpecificationItem *alternativeSpanishAnotherPrimarySpecificationItem_ = NULL;
	SpecificationItem *oppositePossessiveDefinitionSpecificationItem_ = NULL;
	SpecificationItem *possessiveDefinitionSpecificationItem_ = NULL;
	SpecificationItem *postponeNegativeConclusionAnotherPrimarySpecificationItem_ = NULL;
	SpecificationItem *postponeNegativeConclusionPrimarySpecificationItem_ = NULL;
	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;

	WordItem *feminineSpecificationWordItem_ = NULL;
	WordItem *oppositePossessiveSpecificationWordItem_ = NULL;

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminReasoningOld";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;


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
						// Possessive (relation)
				return ( ( isPossessive &&
						relationWordItem != NULL &&

						( ( isFemaleSpecificationWord &&
						relationWordItem->hasFeminineProperNounEnding() ) ||

						( isMaleSpecificationWord &&
						relationWordItem->hasMasculineProperNounEnding() ) ) ) ||

						// Not possessive (generalization)
						( !isPossessive &&

						( ( isFemaleSpecificationWord &&
						generalizationWordItem->hasFeminineProperNounEnding() ) ||

						( isMaleSpecificationWord &&

						( generalizationWordItem->hasMasculineProperNounEnding() ||

						// Typical for Spanish
						( specificationWordItem->isNounWordSpanishAmbiguous() &&
						generalizationWordItem->hasFeminineProperNounEnding() ) ) ) ) ) );
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
		SpecificationItem *conclusionSpecificationItem;
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
			!generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&

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
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
							if( ( isPrimaryUserSpecification ||
							primarySpecificationItem->hasRelationContext() ||
							generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) == NULL ) &&

							adminItem_->askQuestions( true ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions" );
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
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

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
						if( relatedSpecificationItem->isOlderItem() &&
						relatedSpecificationItem->isSelfGeneratedAssumption() &&
						( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, relatedSpecificationItem->specificationWordItem(), userSpecificationItem->generalizationWordItem() ) ) != NULL )
							relatedSpecificationItem = conclusionSpecificationItem;

						if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, relatedSpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

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
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ?
																							previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ?
																				possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
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
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

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

		spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ = NULL;
		spanishAmbiguousCompoundPrimarySpecificationItem_ = NULL;

		feminineSpecificationWordItem_ = NULL;
		}

	signed char checkForUnprocessedNegativeConclusion()
		{
		char errorString[ERROR_STRING_LENGTH];
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

	signed char drawNegativeConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasAnotherSecondarySpecificationWithCompoundCollection = false;
		bool hasCurrentlyMoreSpecificNonExclusiveSpecification;
		bool hasFoundOldSpanishUserSpecification = false;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasRelatedSpecificationsInSameSentence = false;
		bool isCompoundUserSpecification = false;
		bool isHiddenSpanishPrimarySpecification;
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

		hasCurrentlyMoreSpecificNonExclusiveSpecification = ( primarySpecificationItem == existingPositiveSpecificationItem &&
															primarySpecificationItem->isExclusiveSpecification() );
		isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

		secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr();

		if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem->negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != NULL )
			foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem->primarySpecificationItem();

		if( ( hasCurrentlyMoreSpecificNonExclusiveSpecification ||
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
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a specification without specification collection in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				secondarySpecificationItem = createdSpecificationItem;

				if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, secondarySpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an updated specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
						anotherSecondarySpecificationItem = ( !userSpecificationItem->isSpecificationWordSpanishAmbiguous() ?
																userSpecificationItem : primarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, collectionWordItem ) );
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

					if( !hasFoundOldSpanishUserSpecification &&
					secondarySpecificationCollectionNr != anotherPrimarySpecificationCollectionNr &&
					secondarySpecificationItem->hasCompoundSpecificationCollection() &&
					( anotherSecondarySpecificationItem = secondarySpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, NULL ) ) == NULL &&
					( secondarySpecificationCommonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL &&
					secondarySpecificationCommonWordItem != primarySpecificationWordItem )
						anotherSecondarySpecificationItem = secondarySpecificationCommonWordItem->firstSpecificationItem( false, false, false, secondarySpecificationWordItem );
					}
				}

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
					anotherPrimarySpecificationCollectionNr > NO_COLLECTION_NR &&
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

			// User specification is selected
			( isUserSpecificationSelected &&

			( ( !primarySpecificationItem->hasRelationContext() &&

			( existingNegativeSpecificationItem == NULL ||
			existingNegativeSpecificationItem->isOlderItem() ) ) ||

			( isHiddenSpanishPrimarySpecification &&
			existingNegativeSpecificationItem != NULL &&

			( existingNegativeSpecificationItem->hasNonCompoundSpecificationCollection() ||

			( existingNegativeSpecificationItem->isOlderItem() &&
			secondarySpecificationItem->hasRelationContext() ) ) ) ) ) )
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
							negativeConclusionAnotherSecondarySpecificationItem = ( secondarySpecificationCollectionNr == userSpecificationItem->specificationCollectionNr() ?
																						anotherSecondarySpecificationItem : NULL );
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
						if( hasCurrentlyMoreSpecificNonExclusiveSpecification )
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
				if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, primarySpecificationItem->isExclusiveSpecification(), false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAnotherPrimarySpecificationItem, negativeConclusionSecondarySpecificationItem, negativeConclusionAnotherSecondarySpecificationItem, generalizationWordItem, anotherPrimarySpecificationWordItem, NULL ) ).result != RESULT_OK )
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
									negativeAssumptionJustificationItem = foundSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION );

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
					if( generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge() )
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

		return RESULT_OK;
		}

	signed char drawNegativeConclusionsFromAnsweredQuestions( bool hasCurrentlyMoreSpecificNonExclusiveSpecification, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
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
				if( hasCurrentlyMoreSpecificNonExclusiveSpecification ||
				generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, answeredQuestionSpecificationWordItem ) == NULL )
					{
					if( ( currentJustificationItem = answeredQuestionSpecificationItem->firstJustificationItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The answered question specification item has no justification" );

					// Do for all justifications
					do	{
						if( !currentJustificationItem->hasPossessivePrimarySpecification() &&
						// Draw negative conclusion from answered question
						drawNegativeConclusion( answeredQuestionSpecificationItem->isArchivedAssignment(), answeredQuestionSpecificationItem->generalizationWordTypeNr(), answeredQuestionSpecificationItem->specificationWordTypeNr(), answeredQuestionSpecificationItem->generalizationContextNr(), answeredQuestionSpecificationItem->specificationContextNr(), currentJustificationItem->primarySpecificationItem(), currentJustificationItem->secondarySpecificationItem(), userSpecificationItem, NULL, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
					}
				}
			}
		while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem->recentlyAnsweredQuestionSpecificationItem( false ) ) != NULL );

		return RESULT_OK;
		}

	signed char drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned int nonCompoundSpecificationCollectionNr;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *simpleUserSpecificationItem;
		WordItem *collectionWordItem;
		WordItem *commonWordItem = NULL;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSimpleNegativeSpanishConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( !specificationWordItem->isNounWordSpanishAmbiguous() &&
		( simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL &&
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
			// Ask specification substitution question, or draw negative conclusion
			askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem, anotherPrimarySpecificationItem, generalizationWordItem, userSpecificationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCurrentlyCorrectedAssumptionByKnowledge;
		bool hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		bool hasCurrentSpecificationCollection;
		bool hasCurrentSpecificationCompoundCollection;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasGeneralizationWordMasculineProperNounEnding;
		bool hasPrimarySpecificationNonCompoundCollection;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasUserRelationContext = false;
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
		bool isUserQuestion = false;
		bool isUserSpecificationWord;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned short currentSpecificationWordTypeNr;
		unsigned short selectedSpecificationWordTypeNr;
		unsigned short selectedRelationWordTypeNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *tempJustification;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *alternativeSpanishAnotherPrimarySpecificationItem;
		SpecificationItem *archivedAssignmentItem;
		SpecificationItem *bestMatchingRelationSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedAnotherPrimarySpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *spanishAmbiguousSpecificationItem = NULL;
		SpecificationItem *userSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *selectedRelationWordItem;
		WordItem *userSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
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

		hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge();

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
			{
			hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
			hasUserRelationContext = userSpecificationItem->hasRelationContext();
			isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserQuestion = userSpecificationItem->isQuestion();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
			userSpecificationWordItem = userSpecificationItem->specificationWordItem();

			isIncludingAdjectives = ( !isExclusiveSpecification &&
									!hasCurrentlyCorrectedAssumptionByKnowledge &&
									!userSpecificationItem->isQuestion() );
			}

		if( ( currentSpecificationItem = specificationWordItem->firstNonPossessiveDefinitionSpecificationItem( isIncludingAdjectives ) ) != NULL )
			{
			hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();

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
				spanishAmbiguousSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );

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
						if( currentSpecificationItem->isUserSpecification() &&
						generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, false, false, currentSpecificationCollectionNr, currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
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
								foundPrimarySpecificationItem->hasNewInformation() &&

								( !hasUserSpecificationRelationContext ||
								foundPrimarySpecificationItem->hasNonCompoundSpecificationCollection() ) ) ||

								( !isUserGeneralizationWord &&

								( hasRelationWord ||
								generalizationWordItem->isOlderItem() ) &&

								oppositePossessiveDefinitionSpecificationItem_ != NULL &&
								oppositePossessiveDefinitionSpecificationItem_->specificationCollectionNr() == userSpecificationCollectionNr ) )
									{
									selectedRelationWordTypeNr = ( isIncludingRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR );
									selectedRelationWordItem = ( isIncludingRelationWord ? relationWordItem : NULL );
									selectedPrimarySpecificationItem = ( isFoundPrimaryHiddenSpanishSpecification ? userSpecificationItem : foundPrimarySpecificationItem );
									selectedAnotherPrimarySpecificationItem = ( isFoundPrimaryHiddenSpanishSpecification ? oppositePossessiveDefinitionSpecificationItem_ : NULL );

									// Make a feminine or masculine proper noun ending assumption
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAnotherPrimarySpecificationItem, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

									if( !isUserSpecificationWordSpanishAmbiguous &&
									( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
										{
										// Draw missing negative conclusions
										if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
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
							isPossessiveUserSpecification ) ) ) ) ) ) )
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
								hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ||
								adminItem_->hasSuggestiveQuestionAssumption() ) )
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
								currentSpecificationItem->hasAssumptionLevel() ||

								( relationWordItem != NULL &&
								!isSpanishCurrentLanguage &&
								foundPrimarySpecificationItem->hasRelationContext() &&
								relationWordItem->hasCurrentlyConfirmedSpecification() ) ) &&

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
										alternativePrimarySpecificationItem = ( !isSpecificationWordSpanishAmbiguous ? ( hasRelationWord ? userSpecificationItem : foundPrimarySpecificationItem ) :
																					alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) );

										if( ( isSpecificationWordSpanishAmbiguous &&
										primarySpecificationItem->isHiddenSpanishSpecification() ) ||

										( existingSpecificationItem != NULL &&
										alternativePrimarySpecificationItem != NULL &&
										alternativePrimarySpecificationItem->specificationWordItem() == specificationWordItem &&

										( ( isSpecificationWordSpanishAmbiguous &&
										( existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ||

										( primarySpecificationItem->hasRelationContext() &&
										generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) != NULL ) ) ) ||

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
											existingSpecificationItem != NULL )
												{
												if( ( primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, NO_CONTEXT_NR, userSpecificationWordItem ) ) != NULL &&
												// Check if justification already exists
												existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) )
													{
													if( primarySpecificationItem == alternativePrimarySpecificationItem )
														{
														if( ( tempJustification = foundPrimarySpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != NULL &&
														primarySpecificationItem != tempJustification->primarySpecificationItem() )
															primarySpecificationItem = tempJustification->primarySpecificationItem();
														}
													else
														primarySpecificationItem = alternativePrimarySpecificationItem;
													}
												}
											else
												primarySpecificationItem = alternativePrimarySpecificationItem;
											}
										else
											{
											if( isPossessiveUserSpecification &&
											primarySpecificationItem->hasAssumptionLevel() &&
											( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, currentSpecificationWordItem ) ) != NULL &&
											existingSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL )
												// Skip if similar justification already exists, without relation word
												primarySpecificationItem = NULL;
											}
										}

									if( primarySpecificationItem != NULL &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||

									( ( currentSpecificationCollectionNr == NO_COLLECTION_NR ||
									existingSpecificationItem != NULL ) &&

									primarySpecificationItem->isPossessive() ) ) &&

									// No existing specification
									( ( existingSpecificationItem == NULL &&

									( !hasUserRelationContext ||
									isUserQuestion ||
									currentSpecificationItem->isUserSpecification() ||
									!currentSpecificationItem->hasSpecificationCollection() ) ) ||

									// Existing specification
									( existingSpecificationItem != NULL &&
									!existingSpecificationItem->hasRelationContext() &&
									existingSpecificationItem->isSelfGenerated() &&
									// Skip uncertain justifications
									( primarySpecificationItem->assumptionLevel() + currentSpecificationItem->assumptionLevel() ) <= existingSpecificationItem->assumptionLevel() ) ) )
										{
										archivedAssignmentItem = NULL;

										if( isArchivedUserAssignment &&
										userSpecificationItem != NULL &&
										userSpecificationItem->isGeneralizationNoun() )
											{
											isArchivedAssignment = true;
											archivedAssignmentItem = userSpecificationItem;
											}

										selectedSecondarySpecificationItem = ( archivedAssignmentItem == NULL ? currentSpecificationItem : archivedAssignmentItem );

										// Make a compound specification substitution assumption, which is actually an uncertain conclusion
										// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
										//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
										if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, isSpecificationWordSpanishAmbiguous, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, alternativeSpanishAnotherPrimarySpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

										if( createAndAssignResult.createdJustificationItem != NULL &&
										( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
										!foundSpecificationItem->isReplacedItem() &&
										!foundSpecificationItem->hasCurrentCreationSentenceNr() &&
										// Cleanup unreferenced justifications
										generalizationWordItem->cleanupUnreferencedJustifications() != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to clean up unreferenced justifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									{
									if( ( !isExclusiveSpecification &&
									bestMatchingRelationSpecificationItem == NULL &&

									( !isSpecificationWordSpanishAmbiguous ||

									( !isFoundPrimaryHiddenSpanishSpecification &&
									spanishAmbiguousSpecificationItem != NULL &&
									spanishAmbiguousSpecificationItem->hasCurrentCreationSentenceNr() ) ) &&

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
										!currentSpecificationItem->hasCompoundSpecificationCollection() ) ) ) )
											{
											selectedSpecificationWordTypeNr = ( currentSpecificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? WORD_TYPE_NOUN_SINGULAR : currentSpecificationWordTypeNr );

											// Draw a compound specification substitution conclusion
											// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
											//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, selectedSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
												{
												if( hasCurrentlyCorrectedAssumptionByKnowledge )
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
														primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
														// Draw simple negative Spanish conclusions
														drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														if( !createdSpecificationItem->hasCompoundSpecificationCollection() &&
														// Make a specification substitution 'part of' assumption
														makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, currentSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
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

							( hasCurrentlyCorrectedAssumptionByKnowledge ||
							isUserSpecificationWordSpanishAmbiguous ) &&

							// Make generalization assumption by generalization
							adminItem_->makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption by generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
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
		unsigned short selectedRelationWordTypeNr;
		unsigned int nonCompoundSpanishSpecificationCollectionNr;
		unsigned int relationContextNr;
		JustificationItem *olderJustificationItem;
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
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedAnotherPrimarySpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *commonWordItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *feminineSpecificationWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
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
								// Draw a specification substitution conclusion or ask a question
								if( drawSpecificationSubstitutionConclusionOrAskQuestion( true, isArchivedAssignment, isExclusiveSpecification, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
									adminItem_->makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, NO_CONTEXT_NR, definitionSpecificationItem, primarySpecificationItem, generalizationWordItem, commonWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", commonWordItem->anyWordTypeString(), "\"" );

									selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR );

									// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, foundPossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a normal exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

									createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
									foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

									if( ( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? foundSpecificationItem : createdSpecificationItem ) ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "No specification item was created or found" );

									secondarySpecificationItem = createdOrFoundSpecificationItem->updatedSpecificationItem();

									if( createdSpecificationItem != NULL &&
									isPossessiveSpecificationWordSpanishAmbiguous )
										{
										isReplaceHiddenSpecification = true;

										if( spanishAmbiguousCompoundPrimarySpecificationItem_ != NULL )
											primarySpecificationItem = spanishAmbiguousCompoundPrimarySpecificationItem_;

										if( spanishAmbiguousCompoundAnotherPrimarySpecificationItem_ != NULL )
											anotherPrimarySpecificationItem = spanishAmbiguousCompoundAnotherPrimarySpecificationItem_;
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
						adminItem_->addSelfGeneratedSpecification( false, primarySpecificationItem->isAssignment(), false, false, false, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentPossessiveNounWordItem->anyWordTypeString(), "\"" );
						}

					// First opposite possessive specification
					if( !globalVariables_->hasDisplayedWarning &&
					primarySpecificationItem != NULL &&
					possessiveDefinitionSpecificationWordItem != NULL &&
					foundOppositePossessiveDefinitionSpecificationItem != NULL &&
					( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( isNegative ||
						createdOrFoundSpecificationItem == NULL ||
						primarySpecificationItem->hasSpecificationContext() )
							{
							secondarySpecificationItem = primarySpecificationItem;

							// Find another primary definition
							if( !hasRelationWord ||
							isPossessive )
								anotherPrimarySpecificationItem = possessiveDefinitionSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, specificationWordItem );
							}

						if( !isHiddenPrimarySpecification &&
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

							( existingSpecificationItem->isSelfGeneratedAssumption() ||

							( existingSpecificationItem->isHiddenSpanishSpecification() &&
							relationWordItem != NULL &&
							relationWordItem->hasCurrentlyAnsweredSelfGeneratedQuestion() ) ) &&

							( hasRelationWordConfirmedSpecification ||
							!hasRelationWordFeminineOrMasculineProperNounEnding ||
							anotherPrimarySpecificationItem != NULL ) ) ||

							( secondarySpecificationItem->hasRelationContext() &&

							( !secondarySpecificationItem->isSelfGeneratedConclusion() ||
							secondarySpecificationItem->isHiddenSpanishSpecification() ) &&

							( !isHiddenSecondarySpecification ||

							( primarySpecificationItem->isUserSpecification() &&
							!generalizationWordItem->hasCurrentlyMoreSpecificSpecification() ) ||

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
								selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR );
								primarySpecificationItem = primarySpecificationItem->updatedSpecificationItem();
								selectedPrimarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ?
																		( isHiddenSecondarySpecification ? primarySpecificationItem : secondarySpecificationItem ) :
																											foundOppositePossessiveDefinitionSpecificationItem );
								selectedAnotherPrimarySpecificationItem = ( !isReplaceHiddenSpecification &&
																			isOppositePossessiveSpecificationWordSpanishAmbiguous ? NULL : anotherPrimarySpecificationItem );
								selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? foundOppositePossessiveDefinitionSpecificationItem : secondarySpecificationItem );

								// Make an exclusive specification substitution assumption (opposite)
								// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, foundOppositePossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAnotherPrimarySpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result != RESULT_OK )
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
												adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, anotherPrimarySpecificationItem, NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine opposite exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
												}

											if( ( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, false, anotherPrimarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
											compoundSpecificationItem != primarySpecificationItem )
												{
												// Make a compound specification exclusive specification substitution assumption (opposite)
												// See paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
												if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, compoundSpecificationItem, ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), NULL, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

												if( feminineSpecificationWordItem_ != NULL &&
												adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR ), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNounEnding ? anotherPrimarySpecificationItem : compoundSpecificationItem ), NULL, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ).result != RESULT_OK )
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
											// Found, but not created
											if( createdSpecificationItem == NULL )
												{
												secondarySpecificationItem = foundSpecificationItem->updatedSpecificationItem();

												if( hasRelationWord &&
												anotherPrimarySpecificationItem != NULL &&
												primarySpecificationItem->isUserSpecification() &&
												( additionalSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != NULL &&
												additionalSpecificationItem->hasOnlyOneRelationWord() &&

												// Add exclusive specification substitution justification
												adminItem_->addJustificationToSpecification( false, false, false, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, additionalSpecificationItem, anotherPrimarySpecificationItem, NULL, primarySpecificationItem, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an exclusive specification substitution justification to the additional specification" );
												}
											}
										}
									}
								else	// Not possessive
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

													if( ( olderJustificationItem = existingSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) == NULL )
														olderJustificationItem = existingSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

													if( olderJustificationItem != NULL &&
													createdSpecificationItem->attachJustificationToSpecificationAdvanced( olderJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach an older justification item to the created assumption" );
													}
												else
													{
													if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										else
											{
											if( relationWordItem != NULL &&
											existingSpecificationItem->isReplacedItem() &&
											( conclusionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, NO_QUESTION_PARAMETER, createdSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
											!conclusionSpecificationItem->isOlderItem() &&

											// Add opposite possessive justification
											adminItem_->addJustificationToSpecification( false, true, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, createdSpecificationItem, foundPossessiveDefinitionSpecificationItem, NULL, conclusionSpecificationItem, generalizationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive justification to the created specification" );
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
				// Draw reversible conclusions
				if( adminItem_->drawReversibleConclusions( true, relationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				if( primarySpecificationItem->isSpecificationWordSpanishAmbiguous() )
					{
					if( primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
					( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, specificationWordItem->compoundCollectionNr(), primarySpecificationItem->relationContextNr(), specificationWordItem ) ) != NULL )
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

				// Make a possessive conditional specification assumption
				if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, ( isHiddenPrimarySpecification ? adminItem_->userSpecificationItem() : primarySpecificationItem ), generalizationWordItem, relationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption during the making an exclusive specification substitution assumption in word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
				}
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
		bool isReversibleAssumption;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isRelationWordUserWordGeneralization;
		unsigned short assumptionJustificationTypeNr;
		unsigned short assumptionLevel = NO_ASSUMPTION_LEVEL;
		unsigned short selectedAssumptionJustificationTypeNr;
		unsigned int hiddenSpanishSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int relationContextNr;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *selfGeneratedConclusionSpecificationItem = NULL;
		SpecificationItem *userSpecificationItem;
		WordItem *alternativeSecondarySpecificationWordItem;
		WordItem *feminineSpecificationWordItem = NULL;
		WordItem *masculineSpecificationWordItem;
		WordItem *possessiveSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
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
					isReversibleAssumption = ( secondarySpecificationWordItem == possessiveSpecificationWordItem );

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

					selectedAssumptionJustificationTypeNr = ( isReversibleAssumption ? JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );
					selectedPrimarySpecificationItem = ( isPossessive ? possessiveDefinitionSpecificationItem_ : NULL );
					selectedSecondarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem : secondarySpecificationItem );

					// Make a possessive conditional specification assumption (normal)
					// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
					if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, isForcingNewJustification, false, false, !isPossessive, false, false, false, false, false, assumptionLevel, selectedAssumptionJustificationTypeNr, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", possessiveSpecificationWordItem->anyWordTypeString(), "\"" );
					}

				if( !globalVariables_->hasDisplayedWarning )
					{
					if( oppositePossessiveSpecificationWordItem_ == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );

					if( alternativeSecondarySpecificationItem != NULL )
						{
						// Try to find a self-generated conclusion first
						if( ( selfGeneratedConclusionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveSpecificationWordItem_, NULL ) ) != NULL &&
						selfGeneratedConclusionSpecificationItem->isHiddenSpanishSpecification() &&
						alternativeSecondarySpecificationItem->isUserSpecification() )
							selfGeneratedConclusionSpecificationItem = NULL;

						// If not found, try to find a self-generated assumption
						if( selfGeneratedConclusionSpecificationItem == NULL &&
						( assumptionSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != NULL )
							{
							hasSameSpecificationCollection = ( assumptionSpecificationItem->specificationCollectionNr() == alternativeSecondarySpecificationItem->specificationCollectionNr() );

							if( !assumptionSpecificationItem->hasCurrentCreationSentenceNr() &&
							!assumptionSpecificationItem->hasNewInformation() &&

							( generalizationWordItem->hasCurrentlyConfirmedSpecification() == hasSameSpecificationCollection ||
							alternativeSecondarySpecificationItem->isPossessive() ) )
								selfGeneratedConclusionSpecificationItem = assumptionSpecificationItem;
							}
						}

					isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveSpecificationWordItem_->isNounWordSpanishAmbiguous();
					feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ?
														oppositePossessiveSpecificationWordItem_->feminineCollectionWordItem() : oppositePossessiveSpecificationWordItem_->feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem->hasFeminineProperNounEnding() : relationWordItem->hasFeminineProperNounEnding() ) );

					if( feminineSpecificationWordItem != NULL )
						{
						if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
							anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_->firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
						else
							feminineSpecificationWordItem = NULL;
						}

					if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					alternativeSecondarySpecificationItem != NULL &&
					alternativeSecondarySpecificationItem->isUserSpecification() &&

					isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
						hasFeminineOrMasculineProperNounEnding = true;

					hasAlternativeSecondarySpecificationCompoundCollection = ( alternativeSecondarySpecificationItem != NULL &&
																				alternativeSecondarySpecificationItem->hasCompoundSpecificationCollection() );

					isReversibleAssumption = ( secondarySpecificationWordItem == oppositePossessiveSpecificationWordItem_ );

					assumptionJustificationTypeNr = ( isReversibleAssumption ? JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION :

													// Typical for Spanish
													( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
													!hasAlternativeSecondarySpecificationCompoundCollection &&
													!isPossessive ) ? JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION : JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

					if( ( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
					// Typical for Spanish
					isPossessive ||
					existingSpecificationItem == NULL ||
					!existingSpecificationItem->isActiveItem() ||
					!existingSpecificationItem->isHiddenSpanishSpecification() ||
					secondarySpecificationItem->isSelfGenerated() ) &&

					( ( ( !generalizationWordItem->isOlderItem() ||
					selfGeneratedConclusionSpecificationItem == NULL ||

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
					isReversibleAssumption ) &&

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
					!existingUserSpecificationItem->isOlderItem() ) ) ) )
						{
						// Typical for Spanish
						if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
						existingSpecificationItem != NULL &&

						( existingSpecificationItem->isUserSpecification() ||
						existingSpecificationItem->isHiddenSpanishSpecification() ) )
							hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_->specificationCollectionNr();

						selectedAssumptionJustificationTypeNr = ( assumptionJustificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ?
																	secondarySpecificationItem->assumptionLevel() : NO_ASSUMPTION_LEVEL );
						selectedPrimarySpecificationItem = ( isReversibleAssumption ? NULL :
															( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
															alternativeSecondarySpecificationItem != NULL ?
																alternativeSecondarySpecificationItem->updatedSpecificationItem() : possessiveDefinitionSpecificationItem_ ) );
						selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ?
																( hasFeminineOrMasculineProperNounEnding &&
																!isPossessive ? anotherPrimarySpecificationItem : possessiveDefinitionSpecificationItem_ ) : secondarySpecificationItem );

						// Make a possessive conditional specification assumption (opposite)
						// See paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, selectedAssumptionJustificationTypeNr, assumptionJustificationTypeNr, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", oppositePossessiveSpecificationWordItem_->anyWordTypeString(), "\"" );

						// Test file: "I know (11)"
						// Add reversible user justification
						if( isPossessive &&
						isReversibleAssumption &&
						feminineSpecificationWordItem != NULL &&
						secondarySpecificationItem->isOlderItem() &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
						foundSpecificationItem->secondarySpecificationJustificationItem( selectedSecondarySpecificationItem ) == NULL &&
						adminItem_->addJustificationToSpecification( false, true, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, foundSpecificationItem, NULL, NULL, selectedSecondarySpecificationItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the found specification" );

						if( alternativeSecondarySpecificationItem != NULL &&
						feminineSpecificationWordItem != NULL &&
						( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem->specificationWordItem() ) != NULL &&

						( ( isPossessive ||
						!secondarySpecificationItem->isSelfGeneratedAssumption() ||
						generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, secondarySpecificationWordItem, relationWordItem ) == NULL ) &&

						( possessiveSpecificationWordItem == secondarySpecificationWordItem ||

						( !isPossessiveSpecificationWordSpanishAmbiguous &&
						secondarySpecificationItem->isUserSpecification() ) ) ) &&

						( ( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem ) ) == NULL ||
						existingSpecificationItem->isSelfGeneratedAssumption() ) )
							{
							// Make an opposite possessive feminine proper noun ending assumption
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, isExclusiveSpecification, false, false, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( existingSpecificationItem == NULL ? NO_CONTEXT_NR : existingSpecificationItem->relationContextNr() ), ( isOppositePossessiveSpecificationWordSpanishAmbiguous || possessiveSpecificationWordItem == alternativeSecondarySpecificationWordItem ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), ( !isPossessive && isOppositePossessiveSpecificationWordSpanishAmbiguous && oppositePossessiveDefinitionSpecificationItem_ != NULL ? oppositePossessiveDefinitionSpecificationItem_ : possessiveDefinitionSpecificationItem_ ), anotherPrimarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
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
											// Test Spanish test file: "Complejo (19 - mixed)"
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
										( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&

										// Add opposite possessive justification
										adminItem_->addJustificationToSpecification( true, false, true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, createdSpecificationItem, userSpecificationItem, possessiveDefinitionSpecificationItem_, anotherPrimarySpecificationItem, relationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive justification to the created specification" );
										}
									}
								}
							else
								feminineSpecificationWordItem_ = feminineSpecificationWordItem;
							}
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char makeSpecificationSubstitutionPartOfAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasCurrentlyCorrectedAssumptionByKnowledge;
		bool hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
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
			hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge();
			hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
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

					( ( !hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&

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
								existingSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == NULL ) )
									secondarySpecificationItem = tempSpecificationItem;
								}
							}

						if( secondarySpecificationItem != NULL &&
						primarySpecificationItem->isExclusiveSpecification() &&
						// Make generalization assumption
						adminItem_->makeGeneralizationAssumption( hasCurrentlyCorrectedAssumptionByKnowledge, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( !globalVariables_->hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	SpecificationItem *definitionSpecificationItem( WordItem *specificationWordItem )
		{
		return ( possessiveDefinitionSpecificationItem_ != NULL &&
				possessiveDefinitionSpecificationItem_->specificationWordItem() == specificationWordItem ? possessiveDefinitionSpecificationItem_ : oppositePossessiveDefinitionSpecificationItem_ );
		}

	WordItem *oppositePossessiveDefinitionSpecificationWordItem( WordItem *specificationWordItem )
		{
		if( possessiveDefinitionSpecificationItem_ != NULL &&
		oppositePossessiveDefinitionSpecificationItem_ != NULL )
			{
			if( possessiveDefinitionSpecificationItem_->specificationWordItem() == specificationWordItem )
				return oppositePossessiveDefinitionSpecificationItem_->specificationWordItem();

			if( oppositePossessiveDefinitionSpecificationItem_->specificationWordItem() == specificationWordItem )
				return possessiveDefinitionSpecificationItem_->specificationWordItem();
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"Praise him for his mighty works;
 *	praise his unequaled greatness!" (Psalm 150:2)
 *************************************************************************/
