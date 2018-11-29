/*	Class:			AdminReasoningNew
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (redesign)
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
#include "GeneralizationItem.cpp"

class AdminReasoningNew
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasSkippedHiddenSpanishSpecification_ = false;
	bool hasSuggestiveQuestionAssumption_ = false;

	SpecificationItem *adjustedSpanishQuestionSecondarySpecificationItem_ = NULL;
	SpecificationItem *firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = NULL;
	SpecificationItem *secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = NULL;
	SpecificationItem *secondaryDefinitionSpecificationItem_ = NULL;

	WordItem *adjustedQuestionWordItem_ = NULL;

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminReasoningNew";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;


	// Private functions

	signed char askQuestions( bool isAllowingSpanishPossessiveSpecification, bool isArchivedUserAssignment, bool isCheckingForGapInKnowledge, unsigned int userSpecificationCollectionNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *userSpecificationWordItem )
		{
		bool hasExclusiveSecondarySpecificationCompoundCollection;
		bool hasFeminineOrMasculineProperNounEnding;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasPrimarySpecificationRelationContext;
		bool hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection;
		bool hasSkippedAskingQuestion;
		bool hasUserSpecificationCompoundCollection;
		bool hasUserSpecificationRelationContext;
		bool isArchivedAssignment;
		bool isCompoundGeneralizationWordSpanishAmbiguous;
		bool isExclusiveUserSpecification;
		bool isPossessivePrimarySpecification;
		bool isPossessiveUserSpecification;
		bool isPrimaryOldUserSpecification;
		bool isPrimarySpecificationSelfGeneratedAssumption;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		bool isPrimaryUserSpecification;
		bool isUsefulUserSpecificationWithoutRelation;
		bool isUserGeneralizationWord;
		bool isUserSpecificationWordSpanishAmbiguous;
		unsigned short generalizationWordTypeNr;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *archivedPrimarySpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *questionPrimarySpecificationItem;
		SpecificationItem *questionSecondarySpecificationItem;
		WordItem *collectionWordItem;
		WordItem *compoundGeneralizationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		BoolResultType boolResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		hasFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

		hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
		hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
		isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
		isPossessiveUserSpecification = userSpecificationItem->isPossessive();
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
		isUsefulUserSpecificationWithoutRelation = ( hasUserSpecificationCompoundCollection &&
													!hasUserSpecificationRelationContext &&
													isUserGeneralizationWord &&

													( isExclusiveUserSpecification ||
													isPossessiveUserSpecification ||
													globalVariables_->nUserSpecificationWords == 1 ) );

		do	{
			if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL )
				{
				hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
				isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();
				isPossessivePrimarySpecification = primarySpecificationItem->isPossessive();

				if( isPossessivePrimarySpecification )
					{
					if( !hasFeminineOrMasculineProperNounEnding )
						{
						archivedPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, true, false, primarySpecificationWordItem );

						// Typical for Spanish
						if( anotherPrimarySpecificationItem != NULL &&
						// Specification with user specification collection doesn't exist
						generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) == NULL &&
						// Ask questions about generalization word with possessive primary specification
						askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( adminItem_->hasFemaleUserSpecificationWord(), !hasUserSpecificationCompoundCollection, isArchivedAssignment, isArchivedUserAssignment, true, isUserSpecificationWordSpanishAmbiguous, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != NULL && archivedPrimarySpecificationItem->isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), anotherPrimarySpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask Spanish questions about word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
						}
					}
				else
					{
					hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();

					if( ( !hasFeminineOrMasculineProperNounEnding ||
					hasPrimarySpecificationCompoundCollection ||

					( !hasPrimarySpecificationRelationContext &&
					primarySpecificationItem == userSpecificationItem ) ) &&

					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( exclusiveSecondarySpecificationItem = primarySpecificationWordItem->firstExclusiveSpecificationItem() ) != NULL )
						{
						hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection = generalizationWordItem->hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();
						hasSkippedAskingQuestion = false;
						isPrimarySpecificationSelfGeneratedAssumption = primarySpecificationItem->isSelfGeneratedAssumption();
						isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();
						isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();
						isPrimaryOldUserSpecification = ( isPrimaryUserSpecification &&
														primarySpecificationItem->isOlderItem() );

						generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();

						alternativePrimarySpecificationItem = NULL;

						if( isUserGeneralizationWord )
							{
							if( !hasUserSpecificationRelationContext &&
							!isPrimaryUserSpecification &&
							generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation() )
								alternativePrimarySpecificationItem = userSpecificationItem;
							}
						else
							{
							if( isPossessiveUserSpecification )
								{
								if( isUserSpecificationWordSpanishAmbiguous &&
								// Number of relation words of user specification item
								userSpecificationItem->nContextRelations() > 1 &&
								// Specification with exclusive secondary specification collection, or user specification word, doesn't exist
								generalizationWordItem->firstUserSpecificationItem( false, false, exclusiveSecondarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, userSpecificationItem->specificationWordItem() ) == NULL &&

								askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( isArchivedAssignment, generalizationWordTypeNr, userSpecificationItem, generalizationWordItem, userSpecificationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a Spanish question about word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
								}
							else
								{
								if( isPrimarySpecificationSelfGeneratedAssumption )
									alternativePrimarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, primarySpecificationWordItem, NULL );
								}
							}

						do	{
							hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem->hasCompoundSpecificationCollection();

							if( ( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
							( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL )
								{
								isCompoundGeneralizationWordSpanishAmbiguous = ( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem->compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL &&
																				compoundGeneralizationWordItem->isNounWordSpanishAmbiguous() );

								if( ( ( hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection ||

								( !isPrimaryOldUserSpecification &&

								( ( hasUserSpecificationCompoundCollection &&
								isUsefulUserSpecificationWithoutRelation &&
								adjustedQuestionWordItem_ == NULL ) ||

								( !isUsefulUserSpecificationWithoutRelation &&

								( isPrimaryUserSpecification ||

								( hasExclusiveSecondarySpecificationCompoundCollection &&
								hasUserSpecificationRelationContext ) ) ) ) ) ||

								// Adjusted question
								( adjustedQuestionWordItem_ != NULL &&
								hasPrimarySpecificationCompoundCollection &&

								( !isUserSpecificationWordSpanishAmbiguous ||
								compoundGeneralizationWordItem != primarySpecificationWordItem ) ) ) &&

								( ( isCompoundGeneralizationWordSpanishAmbiguous &&
								!isUserSpecificationWordSpanishAmbiguous ) ||

								// Question part doesn't exist
								!generalizationWordItem->hasJustification( ( alternativePrimarySpecificationItem == NULL ? primarySpecificationItem : alternativePrimarySpecificationItem ), NULL, exclusiveSecondarySpecificationItem ) ) ) &&

								( collectionWordItem = exclusiveSecondarySpecificationWordItem->collectionWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL )
									{
									// Adjusted question
									if( alternativePrimarySpecificationItem != NULL ||

									( hasPrimarySpecificationCompoundCollection &&
									!isUserSpecificationWordSpanishAmbiguous &&

									( adjustedQuestionWordItem_ != NULL ||

									( hasPrimarySpecificationRelationContext &&
									isCompoundGeneralizationWordSpanishAmbiguous ) ) ) ||

									( isUserSpecificationWordSpanishAmbiguous &&
									compoundGeneralizationWordItem != primarySpecificationWordItem &&

									( !hasPrimarySpecificationRelationContext ||
									// Same specification but without relation context doesn't exists. Spanish test file: "Esta información es más específico"
									generalizationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, primarySpecificationWordItem ) == NULL ) ) ||

									// Has possibly a gap in the knowledge
									( isCheckingForGapInKnowledge &&

									( !primarySpecificationItem->isOlderItem() ||
									userSpecificationItem->isQuestion() ) &&

									generalizationWordItem->hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationItem ) ) )
										{
										// Avoid asking invalid questions:
										// 1) Check for existing knowledge with same specification collection
										if( generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, exclusiveSecondarySpecificationWordItem ) == NULL &&
										// 2) Check for existing negative specification. Test file: "Skip invalid questions (1)"
										generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, true, false, collectionWordItem, NULL ) == NULL &&
										// 3) Check for existing user specification. Test file: "Skip invalid questions (2)"
										generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, collectionWordItem ) == NULL )
											{
											questionPrimarySpecificationItem = ( alternativePrimarySpecificationItem == NULL ? primarySpecificationItem : alternativePrimarySpecificationItem );
											questionSecondarySpecificationItem = ( isPrimarySpecificationWordSpanishAmbiguous &&
																					isUserGeneralizationWord &&
																					adjustedSpanishQuestionSecondarySpecificationItem_ != NULL ?
																						adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem );

											// Ask a specification substitution question
											// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, questionPrimarySpecificationItem, NULL, questionSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );

											// Typical for Spanish
											if( isPrimarySpecificationWordSpanishAmbiguous )
												{
												if( adjustedSpanishQuestionSecondarySpecificationItem_ == NULL &&
												createAndAssignResult.createdSpecificationItem == NULL )
													// Adjusted question
													adjustedSpanishQuestionSecondarySpecificationItem_ = exclusiveSecondarySpecificationItem;
												}
											else
												{
												if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
												foundSpecificationItem->isCorrectedAssumption() &&
												!primarySpecificationItem->hasCurrentCreationSentenceNr() )
													{
													// Create justification to corrected assumption by opposite suggestive question
													if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, foundSpecificationItem, NULL, exclusiveSecondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

													// Copy specification
													if( ( createAndAssignResult = generalizationWordItem->copySpecificationItem( primarySpecificationItem->specificationCollectionNr(), createdJustificationItem, primarySpecificationItem ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy the assumption specification with the created justification item" );

													if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't copy a corrected assumption by opposite suggestive question" );

													// Replace primary specification
													if( generalizationWordItem->replaceOrDeleteSpecification( primarySpecificationItem, createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the primary specification by the created specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ == NULL )
														firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = createdSpecificationItem;
													else
														secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = createdSpecificationItem;

													primarySpecificationItem = createdSpecificationItem;
													}
												}
											}
										else
											hasSkippedAskingQuestion = true;
										}
									else
										{
										if( isUserGeneralizationWord &&
										hasExclusiveSecondarySpecificationCompoundCollection &&
										adjustedQuestionWordItem_ == NULL &&
										primarySpecificationWordItem->hasCollectionNr( userSpecificationCollectionNr ) &&
										exclusiveSecondarySpecificationWordItem->nonCompoundCollectionNr() > NO_COLLECTION_NR )
											{
											// Find question to be adjusted by compound collection
											if( ( boolResult = generalizationWordItem->findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationCollectionNr, primarySpecificationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );

											if( boolResult.booleanValue )
												adjustedQuestionWordItem_ = generalizationWordItem;
											}
										}
									}
								}
							}
						while( !hasSkippedAskingQuestion &&
						( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveNounSpecificationItem() ) != NULL );
						}
					}
				}
			}
		while( ( primarySpecificationItem = primarySpecificationItem->nextPrimarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL );

		return RESULT_OK;
		}

	signed char askQuestionsAboutSpecificSituations( bool isArchivedAssignment, bool isSpanishCurrentLanguage, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *userGeneralizationWordItem, WordItem *userSpecificationWordItem )
		{
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *correctedSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestionsAboutSpecificSituations";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( userGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user generalization word item is undefined" );

		if( userSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification word item is undefined" );

		if( userSpecificationItem->isExclusiveSpecification() )
			{
			// Question part doesn't exist
			if( !userGeneralizationWordItem->hasJustification( userSpecificationItem, NULL, NULL ) &&

			// Ask question after more specific non-exclusive user specification
			// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, userGeneralizationWordItem, userSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about the current user sentence" );
			}
		else
			{
			if( userSpecificationItem->isPossessive() )
				{
				if( anotherPrimarySpecificationItem != NULL &&
				!userGeneralizationWordItem->hasFeminineOrMasculineProperNounEnding() &&
				// Ask questions about generalization word with possessive primary specification
				askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( false, false, isArchivedAssignment, false, isSpanishCurrentLanguage, false, userSpecificationItem->specificationCollectionNr(), userSpecificationItem, anotherPrimarySpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with possessive user specification as primary specification" );
				}
			else
				{
				if( userGeneralizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
				( secondarySpecificationItem = userSpecificationWordItem->firstNonCompoundUserSpecificationItem() ) != NULL &&
				( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Question part doesn't exist
				!userGeneralizationWordItem->hasJustification( userSpecificationItem, NULL, secondarySpecificationItem ) )
					{
					// Create question part based on current user question
					// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, secondarySpecificationItem, NULL, userGeneralizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an invalid assumption word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create a question part based on current user question" );

					if( ( primarySpecificationItem = userGeneralizationWordItem->suggestiveQuestionAssumptionSecondarySpecificationItem() ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( secondarySpecificationItem = primarySpecificationWordItem->firstNonQuestionSpecificationItem() ) != NULL &&
					( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
					( correctedSpecificationItem = userGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, secondarySpecificationItem->specificationCollectionNr(), secondarySpecificationWordItem ) ) != NULL )
						{
						// Correct invalid assumption by opposite suggestive question
						// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, userGeneralizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct an invalid assumption word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the corrected suggestive question assumption" );

						if( userGeneralizationWordItem->replaceOrDeleteSpecification( correctedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid assumption specification in word \"", userGeneralizationWordItem->anyWordTypeString(), "\"" );

						if( ( obsoleteJustificationItem = userGeneralizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, createdSpecificationItem ) ) != NULL &&
						// Remove justification from corrected assumption
						userGeneralizationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an obsolete justification item" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char askQuestionsWithQuestionAsPrimarySpecification( bool isSpanishCurrentLanguage, WordItem *generalizationWordItem )
		{
		bool isArchivedAssignment;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestionsWithQuestionAsPrimarySpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem() ) != NULL )
			{
			do	{
				if( ( !isSpanishCurrentLanguage ||
				primarySpecificationItem->isSelfGeneratedQuestion() ) &&

				( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
				( secondarySpecificationItem = primarySpecificationWordItem->firstNonCompoundUserSpecificationItem() ) != NULL &&
				// Question part doesn't exist
				!generalizationWordItem->hasJustification( primarySpecificationItem, NULL, secondarySpecificationItem ) &&

				( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
					{
					isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

					// Ask a specification substitution question
					// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with a question as primary specification in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem->nextExclusiveNounSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( bool hasFemaleUserSpecificationWord, bool hasUserSpecificationNonCompoundCollection, bool isArchivedAssignment, bool isArchivedUserAssignment, bool isSpanishCurrentLanguage, bool isUserSpecificationWordSpanishAmbiguous, unsigned int userSpecificationCollectionNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasFoundAnswerToQuestion;
		bool hasPrimarySpecificationCompoundCollection;
		bool isOlderExistingPrimarySpecificationSpanishAmbiguous = false;
		bool isOlderPrimarySpecification;
		bool isUserGeneralizationWord;
		unsigned short generalizationWordTypeNr;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *existingPrimarySpecificationItem = NULL;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( anotherPrimarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Question about generalization word
		if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem() ) != NULL &&
		( exclusiveSecondarySpecificationItem = anotherPrimarySpecificationWordItem->firstExclusiveSpecificationItem() ) != NULL )
			{
			hasFoundAnswerToQuestion = generalizationWordItem->hasFoundAnswerToQuestion();
			hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();
			primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr();

			if( isSpanishCurrentLanguage &&
			( existingPrimarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, primarySpecificationCollectionNr, NULL ) ) != NULL &&
			existingPrimarySpecificationItem->isOlderItem() &&
			existingPrimarySpecificationItem->isHiddenSpanishSpecification() )
				isOlderExistingPrimarySpecificationSpanishAmbiguous = true;

			if( !isOlderExistingPrimarySpecificationSpanishAmbiguous ||

			// The rest of this condition is typical for the Spanish language
			isUserSpecificationWordSpanishAmbiguous ||

			( hasFemaleUserSpecificationWord &&
			!primarySpecificationItem->isSelfGeneratedAssumption() ) )
				{
				do	{
					// Current question part doesn't exist
					if( !generalizationWordItem->hasJustification( primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem ) &&
					( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
					( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL )
						{
						if( isSpanishCurrentLanguage )
							compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem->compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr );

						if( ( !isSpanishCurrentLanguage &&
						// User specification doesn't exist
						generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) == NULL ) ||

						// The rest of this condition is typical for the Spanish language
						( isSpanishCurrentLanguage &&

						( ( compoundGeneralizationWordItem == NULL &&

						( hasFemaleUserSpecificationWord ||
						hasPrimarySpecificationCompoundCollection ) ) ||

						( ( ( !isOlderPrimarySpecification &&
						isUserGeneralizationWord ) ||

						( hasFemaleUserSpecificationWord &&
						!isOlderPrimarySpecification ) ||

						( hasPrimarySpecificationCompoundCollection &&
						!hasUserSpecificationNonCompoundCollection &&
						existingPrimarySpecificationItem == NULL ) ||

						( !hasFoundAnswerToQuestion &&
						!isArchivedUserAssignment &&
						isOlderExistingPrimarySpecificationSpanishAmbiguous ) ) &&

						( compoundGeneralizationWordItem == anotherPrimarySpecificationWordItem ||
						anotherPrimarySpecificationWordItem->hasCollectionNr( exclusiveSecondarySpecificationCollectionNr ) ) ) ) &&

						// Negative specification doesn't exist
						generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, NULL ) == NULL ) )
							{
							// Ask a specification substitution question
							// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
							if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveNounSpecificationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *userSpecificationWordItem )
		{
		SpecificationItem *exclusiveSecondarySpecificationItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( userSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification word item is undefined" );

		// Question about relation word
		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem->firstExclusiveSpecificationItem() ) != NULL )
			{
			do	{
				if( !generalizationWordItem->hasJustification( userSpecificationItem, NULL, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL &&

				// Ask a specification substitution question
				// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveNounSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *createdNewSpecificationItem;
		SpecificationItem *createdOldSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *negativeSpecificationItem;
		SpecificationItem *oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem *definitionSpecificationWordItem;
		WordItem *oldSuggestiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByOppositeSuggestiveQuestion";

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
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem->suggestiveQuestionAssumptionSecondarySpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem->isActiveAssignment() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, oldSuggestiveQuestionAssumptionAssignmentItem, NULL, generalizationWordItem, oldSuggestiveSpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oldSuggestiveSpecificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		// Replace created old specification by created new specification in justifications of involved word
		if( generalizationWordItem->updateSpecificationOfJustificationsInWord( true, createdOldSpecificationItem, createdNewSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update a specification in justifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ != NULL &&
		( definitionSpecificationWordItem = firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_->specificationWordItem() ) != NULL )
			{
			// Add justification to corrected assumption by opposite suggestive question (First - Old)
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem->firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != NULL &&
			addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdOldSpecificationItem, NULL, definitionSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the first corrected assumption specification" );

			// Add justification to corrected assumption by opposite suggestive question (First - New)
			if( ( definitionSpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != NULL &&
			addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdNewSpecificationItem, NULL, definitionSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add another specification substitution justification to the first corrected assumption specification" );
			}

		if( secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ != NULL &&
		( definitionSpecificationWordItem = secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_->specificationWordItem() ) != NULL )
			{
			// Add justification to corrected assumption by opposite suggestive question (Second - Old)
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem->firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != NULL &&
			addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdOldSpecificationItem, NULL, definitionSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the second corrected assumption specification" );

			// Add justification to corrected assumption by opposite suggestive question (Second - New)
			if( ( definitionSpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != NULL &&
			addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdNewSpecificationItem, NULL, definitionSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add another specification substitution justification to the second corrected assumption specification" );
			}

		// Replace negative assumption
		if( ( negativeSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, isPossessive, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
			{
			// Write negative assumption
			if( generalizationWordItem->writeUpdatedSpecification( false, false, true, false, false, negativeSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			// Replace corrected negative assumption
			if( generalizationWordItem->replaceOrDeleteSpecification( negativeSpecificationItem, secondarySpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative specification" );
			}

		return RESULT_OK;
		}

	signed char drawCompoundSpecificationSubstitutionConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isSpanishCurrentLanguage;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		WordItem *currentSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonPossessiveDefinitionSpecificationItem( false ) ) != NULL )
			{
			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage ||
			( compoundGeneralizationWordItem = specificationWordItem->compoundGeneralizationWordItem( primarySpecificationItem->specificationCollectionNr() ) ) != NULL )
				{
				do	{
					// Select non-possessive definition
					if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
					currentSpecificationWordItem != generalizationWordItem &&
					compoundGeneralizationWordItem != currentSpecificationWordItem &&
					!currentSpecificationItem->isExclusiveSpecification() &&
					// Typical for Spanish
					currentSpecificationItem->isUserSpecification() &&
					currentSpecificationItem->hasNonCompoundSpecificationCollection() )
						{
						// Draw a compound specification substitution conclusion
						// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
						//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( !isSpanishCurrentLanguage &&
						( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
						// Make generalization assumption
						makeGeneralizationAssumption( false, isArchivedAssignment, specificationWordTypeNr, generalizationWordTypeNr, NO_CONTEXT_NR, createdSpecificationItem, currentSpecificationItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", specificationWordItem->anyWordTypeString(), "\" and specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem->nextNonPossessiveDefinitionSpecificationItem( false ) ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char drawReversibleConclusions( bool hasCreatedSpecification, SpecificationItem *currentSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		bool hasNonCompoundSecondarySpecificationCollection;
		bool hasNonCompoundUserSpecificationCollection = false;
		bool hasUserGeneralizationWordCurrentlyConfirmedSpecification = false;
		bool hasUserSpecificationAssumptionLevel = false;
		bool hasUserSpecificationRelationContext = false;
		bool isActiveUserAssignment = false;
		bool isArchivedAssignment;
		bool isArchivedUserAssignment = false;
		bool isAssignment;
		bool isExclusiveGeneralizationAssignment;
		bool isExclusiveSecondarySpecification;
		bool isIncrementingOrderNr;
		bool isOlderAlternativeSpecification;
		bool isOlderExistingSpecification;
		bool isOlderSecondarySpecification;
		bool isOlderUserSpecification;
		bool isPossessiveSecondarySpecification;
		bool isPossessiveUserSpecification = false;
		bool isSecondaryArchivedAssignment;
		bool isSecondarySelfGeneratedAssumption;
		bool isSecondarySelfGeneratedSpecification;
		bool isSecondarySpecificationWordSpanishAmbiguous = false;
		bool isSecondaryUserAssignment;
		bool isSpanishCurrentLanguage;
		bool isSpanishSpecificationFemaleWord = false;
		bool isUniqueUserRelation;
		bool isUserExistingSpecification;
		bool isUserGeneralizationWord;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned short secondaryGeneralizationWordTypeNr;
		unsigned short secondarySpecificationWordTypeNr;
		unsigned short secondaryRelationWordTypeNr;
		unsigned int foundRelationContextNr;
		unsigned int nonCompoundSpanishSpecificationCollectionNr;
		unsigned int relationContextNr;
		unsigned int secondaryRelationContextNr;
		unsigned int secondarySpecificationCollectionNr;
		unsigned int spanishAmbiguousRelationContextNr = NO_CONTEXT_NR;
		unsigned int spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int userSpecificationCollectionNr;
		JustificationItem *assumptionJustificationItem;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *olderJustificationItem;
		SpecificationItem *alternativeSpecificationItem;
		SpecificationItem *assignmentSpecificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *collectedSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *firstUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *spanishDefinitionSpecificationItem;
		SpecificationItem *spanishMasculineSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *unhiddenSpanishSpecificationItem = NULL;
		SpecificationItem *userSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *firstContextWordItem;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem;
		WordItem *spanishFeminineSpecificationWordItem = NULL;
		WordItem *spanishMasculineSpecificationWordItem = NULL;
		WordItem *secondarySpecificationWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawReversibleConclusions";

		if( currentSecondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( firstContextWordItem = globalVariables_->firstContextWordItem ) != NULL &&
		( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
		( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			secondarySpecificationWordTypeNr = currentSecondarySpecificationItem->specificationWordTypeNr();

			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();

			hasNonCompoundUserSpecificationCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			hasUserGeneralizationWordCurrentlyConfirmedSpecification = userGeneralizationWordItem->hasCurrentlyConfirmedSpecification();
			hasUserSpecificationAssumptionLevel = userSpecificationItem->hasAssumptionLevel();
			hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			if( isPossessiveUserSpecification )
				{
				if( generalizationWordItem->isUserGeneralizationWord )
					{
					// Typical for Chinese: Skip possessive inactive assignment
					if( !userSpecificationItem->isInactiveAssignment() )
						{
						currentContextWordItem = firstContextWordItem;

						// Do for all context words
						do	{
							if( currentContextWordItem->isUserRelationWord )
								{
								if( ( existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, false, userSpecificationWordItem, generalizationWordItem ) ) == NULL ||

								( existingSpecificationItem->isSelfGenerated() &&
								// Justification doesn't exist in specification
								existingSpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL ) )
									{
									// Typical for Spanish
									if( isSpanishCurrentLanguage ||

									( !hasNonCompoundUserSpecificationCollection &&
									existingSpecificationItem == NULL ) ||

									( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
									// Self-generated conclusion with different relation word
									currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, NULL ) != NULL ) ||

									( existingSpecificationItem != NULL &&
									// Conclusion doesn't exist
									currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, NULL ) == NULL ) ||

									( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
									generalizationWordItem->firstActiveQuestionSpecificationItem() != NULL ) &&

									// Assumption doesn't exist
									currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, userSpecificationWordItem, NULL ) == NULL ) )
										{
										isAssignment = false;
										isArchivedAssignment = false;

										// Typical for Spanish
										if( isSpanishCurrentLanguage )
											{
											spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
											spanishAmbiguousRelationContextNr = NO_CONTEXT_NR;
											unhiddenSpanishSpecificationItem = NULL;

											if( hasNonCompoundUserSpecificationCollection )
												{
												if( !currentContextWordItem->hasNegativeSpecification() ||
												currentContextWordItem->hasFeminineProperNounEnding() )
													spanishAmbiguousSpecificationCollectionNr = userSpecificationCollectionNr;
												}
											else
												{
												if( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
												existingSpecificationItem == NULL &&
												( unhiddenSpanishSpecificationItem = currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, NULL ) ) != NULL &&
												unhiddenSpanishSpecificationItem->isSelfGeneratedConclusion() )
													spanishAmbiguousRelationContextNr = unhiddenSpanishSpecificationItem->relationContextNr();
												}
											}

										// Similar specification already exists as assignment
										// So, make this conclusion an assignment too
										if( existingSpecificationItem == NULL &&
										( assignmentSpecificationItem = currentContextWordItem->firstAssignmentItem( false, false, NO_CONTEXT_NR, userSpecificationWordItem ) ) != NULL )
											{
											isAssignment = true;
											isArchivedAssignment = assignmentSpecificationItem->isArchivedAssignment();
											}

										// Draw reversible user conclusion
										if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, spanishAmbiguousRelationContextNr, NULL, NULL, userSpecificationItem, NULL, currentContextWordItem, userSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible user conclusion in word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );

										// Typical for Spanish
										if( isUserSpecificationWordSpanishAmbiguous )
											{
											if( spanishAmbiguousRelationContextNr == NO_CONTEXT_NR )
												{
												if( currentContextWordItem->hasFeminineProperNounEnding() &&
												( spanishFeminineSpecificationWordItem = userSpecificationWordItem->feminineCollectionWordItem() ) != NULL &&
												( spanishDefinitionSpecificationItem = userSpecificationWordItem->firstSpecificationItem( false, false, false, spanishFeminineSpecificationWordItem ) ) != NULL &&
												currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, NO_QUESTION_PARAMETER, spanishDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, spanishFeminineSpecificationWordItem ) == NULL &&

												// Draw Spanish feminine reversible user conclusion
												adminItem_->addSelfGeneratedSpecification( true, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, userSpecificationItem, spanishDefinitionSpecificationItem, currentContextWordItem, spanishFeminineSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish feminine reversible user conclusion in word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", spanishFeminineSpecificationWordItem->anyWordTypeString(), "\"" );
												}
											else
												{
												// Typical for Spanish. Test files: "Complejo (12)"
												if( unhiddenSpanishSpecificationItem != NULL &&
												( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
												( olderJustificationItem = unhiddenSpanishSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) != NULL &&
												( assumptionSpecificationItem = currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, userSpecificationWordItem, NULL ) ) != NULL )
													{
													// Add a relation context to the assumption specification
													if( generalizationWordItem->addContext( WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, assumptionSpecificationItem->relationContextNr(), NO_COLLECTION_NR, userSpecificationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

													// Add reversible justification to the assumption specification
													if( addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, assumptionSpecificationItem, NULL, NULL, olderJustificationItem->secondarySpecificationItem(), currentContextWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the assumption specification" );

													// Add reversible justification to the created specification
													if( addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, createdSpecificationItem, NULL, NULL, olderJustificationItem->secondarySpecificationItem(), currentContextWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the created specification" );

													// Replace conclusion specification
													if( currentContextWordItem->replaceOrDeleteSpecification( unhiddenSpanishSpecificationItem->updatedSpecificationItem(), createdSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the unhidden Spanish specification by the created specification in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
													}
												}
											}
										else
											{
											if( existingSpecificationItem != NULL &&
											( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
											( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
											!foundSpecificationItem->isReplacedItem() &&
											// Replace an assumption specification by a conclusion specification in justifications of involved word
											currentContextWordItem->updateSpecificationOfJustificationsInWord( false, foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in a justification in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								}
							}
						while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
						}
					}
				else
					{
					if( isSpanishCurrentLanguage &&
					// Draw proper noun part-of conclusions
					drawProperNounPartOfConclusions( false, false, false, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a proper noun 'part of' conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			else	// Not possessive user specification
				{
				if( userSpecificationItem->isActiveAssignment() )
					isActiveUserAssignment = true;
				else
					{
					if( userSpecificationItem->isArchivedAssignment() )
						isArchivedUserAssignment = true;
					}

				if( !isUserGeneralizationWord &&
				hasUserSpecificationRelationContext &&
				( existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, userSpecificationWordItem, userGeneralizationWordItem ) ) != NULL &&
				// Justification doesn't exist in specification
				existingSpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL &&

				// Older specification without specification collection
				( !existingSpecificationItem->hasSpecificationCollection() ||
				existingSpecificationItem->isFirstJustificiationReversibleAssumption() ) )
					{
					// Draw reversible user conclusion
					if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, userSpecificationItem, NULL, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible user conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );

					// Cleanup unreferenced justifications
					if( isActiveUserAssignment &&
					existingSpecificationItem->isConcludedAssumption() &&
					generalizationWordItem->cleanupUnreferencedJustifications() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to clean up unreferenced justifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}

			// Do for all selected specifications
			do	{
				hasNonCompoundSecondarySpecificationCollection = currentSecondarySpecificationItem->hasNonCompoundSpecificationCollection();
				isOlderSecondarySpecification = currentSecondarySpecificationItem->isOlderItem();
				isPossessiveSecondarySpecification = currentSecondarySpecificationItem->isPossessive();
				isSecondaryArchivedAssignment = currentSecondarySpecificationItem->isArchivedAssignment();
				isSecondarySelfGeneratedAssumption = currentSecondarySpecificationItem->isSelfGeneratedAssumption();
				isSecondarySelfGeneratedSpecification = currentSecondarySpecificationItem->isSelfGenerated();
				isSecondarySpecificationWordSpanishAmbiguous = currentSecondarySpecificationItem->isSpecificationWordSpanishAmbiguous();
				secondaryGeneralizationWordTypeNr = currentSecondarySpecificationItem->generalizationWordTypeNr();
				secondarySpecificationWordTypeNr = currentSecondarySpecificationItem->specificationWordTypeNr();
				secondaryRelationWordTypeNr = currentSecondarySpecificationItem->relationWordTypeNr();
				secondarySpecificationCollectionNr = currentSecondarySpecificationItem->specificationCollectionNr();
				secondaryRelationContextNr = currentSecondarySpecificationItem->relationContextNr();

				if( ( secondarySpecificationWordItem = currentSecondarySpecificationItem->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The secondary specification item has no specification word" );

				if( !isOlderSecondarySpecification &&
				( obsoleteSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isPossessiveSecondarySpecification, NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != NULL &&
				obsoleteSpecificationItem->isOlderItem() &&
				obsoleteSpecificationItem->isSelfGenerated() &&
				// Skip all assignments
				generalizationWordItem->firstAssignmentItem( isPossessiveSecondarySpecification, false, NO_CONTEXT_NR, secondarySpecificationWordItem ) == NULL )
					{
					// Write assumption/conclusion without relation word
					if( generalizationWordItem->writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					// Replace assumption/conclusion without relation word
					if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, currentSecondarySpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
					}

				if( isPossessiveSecondarySpecification )
					{
					if( isSecondarySelfGeneratedSpecification )
						{
						if( isUserGeneralizationWord ||
						secondarySpecificationCollectionNr == NO_COLLECTION_NR )
							{
							if( !isSpanishCurrentLanguage ||
							!isSecondarySpecificationWordSpanishAmbiguous )
								{
								currentContextWordItem = firstContextWordItem;

								// Do for all context words
								do	{
									if( currentContextWordItem != generalizationWordItem &&
									( existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, secondarySpecificationWordItem, generalizationWordItem ) ) != NULL )
										{
										// Found user specification, force self-generated assumption
										if( existingSpecificationItem->isUserSpecification() &&
										( tempSpecificationItem = currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, true, secondarySpecificationWordItem, generalizationWordItem ) ) != NULL )
											existingSpecificationItem = tempSpecificationItem;

										currentSecondarySpecificationItem = currentSecondarySpecificationItem->updatedSpecificationItem();

										if( hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation )
											{
											if( currentContextWordItem->isUserRelationWord &&
											( obsoleteJustificationItem = currentSecondarySpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, currentContextWordItem ) ) != NULL )
												{
												isIncrementingOrderNr = ( currentSecondarySpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, generalizationWordItem ) == NULL );

												if( ( justificationResult = generalizationWordItem->addJustification( false, false, isIncrementingOrderNr, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, userSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible assumption justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find or create a reversible assumption justification" );

												if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, currentSecondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete reversible assumption justification of the current secondary specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										else
											{
											if( existingSpecificationItem->hasCurrentCreationSentenceNr() &&
											existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
											existingSpecificationItem->secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == NULL &&
											// Relation context exists in word
											currentContextWordItem->hasContextInWord( secondaryRelationContextNr, secondarySpecificationWordItem ) &&

											// Justification doesn't exist in specification
											( ( olderJustificationItem = existingSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ) ) == NULL ||
											olderJustificationItem->secondaryGeneralizationWordItem() != generalizationWordItem ) &&

											// Add reversible justification
											addJustificationToSpecification( false, false, false, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, existingSpecificationItem, NULL, NULL, currentSecondarySpecificationItem, currentContextWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
											}
										}
									}
								while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
								}
							}
						else
							{
							// Typical for Spanish
							if( isOlderSecondarySpecification )
								{
								if( isActiveUserAssignment )
									{
									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									hasNonCompoundSecondarySpecificationCollection &&
									// Justification doesn't exist in specification
									currentSecondarySpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL &&
									// Draw Spanish non-compound reversible conclusion
									adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, secondaryRelationWordTypeNr, secondarySpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, userSpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible specification from word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( !hasNonCompoundSecondarySpecificationCollection &&
									hasUserGeneralizationWordCurrentlyConfirmedSpecification &&
									!hasUserSpecificationAssumptionLevel &&
									secondarySpecificationWordItem == userSpecificationWordItem &&
									currentSecondarySpecificationItem->hasOnlyOneRelationWord() &&
									!currentSecondarySpecificationItem->isConcludedAssumption() )
										{
										if( currentSecondarySpecificationItem->markAsConcludedAssumption() != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the current secondary specification as concluded assumption" );

										// Write concluded assumption
										if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, currentSecondarySpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a concluded assumption" );
										}
									}
								}

							// Typical for Spanish
							if( hasNonCompoundSecondarySpecificationCollection &&
							hasUserSpecificationRelationContext &&
							!isPossessiveUserSpecification &&
							isUserSpecificationWordSpanishAmbiguous &&
							// Specification doesn't exist
							generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, NO_CONTEXT_NR, userSpecificationWordItem ) == NULL &&

							// Draw reversible conclusion
							adminItem_->addSelfGeneratedSpecification( false, false, isArchivedUserAssignment, false, false, false, false, true, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, userSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, userSpecificationItem, NULL, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible user conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					else	// User specification
						{
						if( !hasNonCompoundSecondarySpecificationCollection &&
						!isOlderSecondarySpecification )
							{
							currentContextWordItem = firstContextWordItem;

							// Do for all context words
							do	{
								if( currentContextWordItem != generalizationWordItem &&
								currentContextWordItem->isUserRelationWord &&
								currentContextWordItem->hasNegativeSpecification() &&
								// Draw a specification substitution conclusion or ask a question
								adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( false, false, false, NO_QUESTION_PARAMETER, secondaryGeneralizationWordTypeNr, secondarySpecificationWordTypeNr, secondaryRelationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
							}
						}
					}
				else	// Not possessive current secondary specification
					{
					isExclusiveSecondarySpecification = currentSecondarySpecificationItem->isExclusiveSpecification();
					isSecondaryUserAssignment = currentSecondarySpecificationItem->isUserAssignment();

					if( hasUserSpecificationRelationContext &&
					isSecondarySelfGeneratedSpecification &&
					// Secondary specification word is same as user specification word
					secondarySpecificationWordItem == userSpecificationWordItem )
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage )
							{
							if( ( isUserGeneralizationWord ||
							hasNonCompoundSecondarySpecificationCollection ) &&

							// Justification doesn't exist in specification
							currentSecondarySpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL )
								{
								// Add Spanish reversible user justification
								if( addJustificationToSpecification( false, false, hasNonCompoundSecondarySpecificationCollection, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, NULL, NULL, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish reversible user justification to the current secondary specification" );

								currentSecondarySpecificationItem = currentSecondarySpecificationItem->updatedSpecificationItem();
								}
							}
						else
							{
							if( isPossessiveUserSpecification &&
							isSecondarySelfGeneratedAssumption )
								{
								if( ( hasUserGeneralizationWordCurrentlyConfirmedSpecification ||

								( ( existingSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, secondarySpecificationWordItem, NULL ) ) != NULL &&
								// Justification doesn't exist in specification
								existingSpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL ) ) &&

								// Conclusion doesn't exist
								generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem, userGeneralizationWordItem ) == NULL )
									{
									relationContextNr = NO_CONTEXT_NR;
									conclusionSpecificationItem = NULL;

									if( hasNonCompoundSecondarySpecificationCollection )
										{
										if( hasUserGeneralizationWordCurrentlyConfirmedSpecification )
											{
											if( ( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem, NULL ) ) == NULL )
												{
												if( ( foundRelationContextNr = userGeneralizationWordItem->contextNr( secondarySpecificationWordItem ) ) > NO_CONTEXT_NR &&
												adminItem_->nContextWords( foundRelationContextNr, secondarySpecificationWordItem ) > 1 )
													{
													if( ( contextResult = adminItem_->getRelationContext( false, false, false, false, generalizationWordItem, secondarySpecificationWordItem, userGeneralizationWordItem, NULL ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get a relation context" );

													relationContextNr = contextResult.contextNr;
													}
												}
											else
												relationContextNr = conclusionSpecificationItem->relationContextNr();
											}
										else
											{
											if( ( foundRelationContextNr = userGeneralizationWordItem->contextNr( secondarySpecificationWordItem ) ) > NO_CONTEXT_NR )
												relationContextNr = foundRelationContextNr;
											}
										}

									// Draw reversible user conclusion
									if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, hasUserGeneralizationWordCurrentlyConfirmedSpecification, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, secondaryRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, relationContextNr, NULL, NULL, userSpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user conclusion from word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

									if( conclusionSpecificationItem != NULL &&
									// Confirmed all relation words
									conclusionSpecificationItem->nRelationContextWords() == currentSecondarySpecificationItem->nRelationContextWords() &&
									// Replace assumption by conclusion
									generalizationWordItem->replaceOrDeleteSpecification( currentSecondarySpecificationItem, conclusionSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
									}
								}
							}
						}

					isExclusiveGeneralizationAssignment = ( isSecondaryUserAssignment &&
															currentSecondarySpecificationItem->isExclusiveGeneralization() );
					currentContextWordItem = firstContextWordItem;

					// Typical for Spanish
					if( isSpanishCurrentLanguage )
						{
						isSpanishSpecificationFemaleWord = secondarySpecificationWordItem->isFemale();
						spanishMasculineSpecificationWordItem = ( isSpanishSpecificationFemaleWord ? secondarySpecificationWordItem->masculineCollectionWordItem() : NULL );

						if( !isSecondarySpecificationWordSpanishAmbiguous &&
						spanishMasculineSpecificationWordItem != NULL &&
						generalizationWordItem->firstAssignmentOrSpecificationItem( false, isSecondaryArchivedAssignment, true, false, false, spanishMasculineSpecificationWordItem ) == NULL &&
						( primarySpecificationItem = spanishMasculineSpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, secondarySpecificationWordItem ) ) != NULL &&

						// Draw negative Spanish conclusion
						adminItem_->drawNegativeConclusion( isSecondaryArchivedAssignment, WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, ( isActiveUserAssignment && currentSecondarySpecificationItem == userSpecificationItem ? adminItem_->simpleUserSpecificationItem() : currentSecondarySpecificationItem ), NULL, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}

					// Do for all context words
					do	{
						if( currentContextWordItem != generalizationWordItem &&
						// Relation context exists in word
						currentContextWordItem->hasContextInWord( secondaryRelationContextNr, secondarySpecificationWordItem ) )
							{
							isOlderExistingSpecification = false;
							isUserExistingSpecification = false;

							if( ( existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, isSecondaryArchivedAssignment, false, true, secondarySpecificationWordItem, generalizationWordItem ) ) != NULL )
								{
								isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
								isUserExistingSpecification = existingSpecificationItem->isUserSpecification();
								}

							if( isExclusiveSecondarySpecification )
								{
								// Specification doesn't exist
								if( existingSpecificationItem == NULL &&
								// Draw exclusive reversible conclusion
								adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, secondaryRelationWordTypeNr, secondarySpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem, NULL, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible specification from current context word \"", currentContextWordItem->anyWordTypeString(), "\" to specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								firstUserSpecificationItem = currentContextWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem );

								isOlderUserSpecification = ( existingSpecificationItem == NULL &&
															firstUserSpecificationItem != NULL );

								alternativeSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, secondarySpecificationWordItem, currentContextWordItem );

								isOlderAlternativeSpecification = ( alternativeSpecificationItem != NULL &&
																	alternativeSpecificationItem->isOlderItem() );

								if( isOlderUserSpecification ||
								firstUserSpecificationItem == NULL ||

								// Typical for Spanish
								( isSpanishCurrentLanguage &&
								currentSecondarySpecificationItem != alternativeSpecificationItem &&

								( !isOlderExistingSpecification ||
								!isOlderAlternativeSpecification ) ) )
									{
									if( existingSpecificationItem == NULL ||

									( currentSecondarySpecificationItem == alternativeSpecificationItem &&
									// Justification doesn't exist in specification
									existingSpecificationItem->secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == NULL ) )
										{
										if( !isSecondarySpecificationWordSpanishAmbiguous ||
										// Typical for Spanish
										!hasGeneralizationWordCurrentlyConfirmedSpecification ||
										!isSecondaryUserAssignment )
											{
											isAssignment = ( isExclusiveGeneralizationAssignment ||
															isSecondaryArchivedAssignment );

											isUniqueUserRelation = ( isSecondaryArchivedAssignment &&
																	isSecondaryUserAssignment &&
																	adminItem_->isUniqueUserRelation() );

											if( isSpanishCurrentLanguage )
												spanishAmbiguousSpecificationCollectionNr = ( !isSpanishSpecificationFemaleWord &&
																							alternativeSpecificationItem == NULL ? secondarySpecificationCollectionNr :

																							( existingSpecificationItem != NULL &&

																							( existingSpecificationItem->wasHiddenSpanishSpecification() ||

																							( !hasNonCompoundSecondarySpecificationCollection &&
																							!isSpanishSpecificationFemaleWord &&
																							existingSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ?
																								existingSpecificationItem->specificationCollectionNr() : NO_COLLECTION_NR ) );

											// Draw reversible conclusion
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isSecondaryArchivedAssignment, false, false, false, false, false, false, false, true, isUniqueUserRelation, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, secondaryRelationWordTypeNr, secondarySpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem, NULL, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion in word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
											foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
											currentSecondarySpecificationItem = currentSecondarySpecificationItem->updatedSpecificationItem();

											if( !isSpanishCurrentLanguage &&
											!isOlderUserSpecification &&
											isSecondarySelfGeneratedSpecification &&
											( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? foundSpecificationItem->updatedSpecificationItem() : createdSpecificationItem ) ) != NULL &&
											// Justification doesn't exist in specification
											currentSecondarySpecificationItem->secondarySpecificationJustificationItem( createdOrFoundSpecificationItem ) == NULL &&
											createdOrFoundSpecificationItem->hasNonCompoundSpecificationCollection() &&
											// Add reversible user justification
											addJustificationToSpecification( false, currentContextWordItem->isUserRelationWord, false, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, NULL, NULL, createdOrFoundSpecificationItem, generalizationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

											if( createdSpecificationItem != NULL )
												{
												// Created, not found
												if( foundSpecificationItem == NULL )
													{
													// Typical for Spanish
													if( isSpanishCurrentLanguage &&
													!isOlderUserSpecification &&
													( obsoleteSpecificationItem = currentContextWordItem->noRelationContextSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : secondarySpecificationWordItem ) ) ) != NULL )
														{
														// Write an assumption without relation word
														if( currentContextWordItem->writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", currentContextWordItem->anyWordTypeString(), "\"" );

														// Replace an assumption without relation word
														if( currentContextWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
														}
													}
												else	// Created and found
													{
													if( isOlderUserSpecification &&
													!isExclusiveGeneralizationAssignment )
														{
														// Add reversible justification. Test file: "Complex (19 - strange)"
														if( addJustificationToSpecification( false, false, false, true, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, createdSpecificationItem, NULL, NULL, foundSpecificationItem, currentContextWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the found specification" );

														if( currentContextWordItem->updateSpecificationOfJustificationsOfInvolvedWords( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"", currentContextWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										}
									else
										{
										if( currentSecondarySpecificationItem == alternativeSpecificationItem )
											{
											if( isUserGeneralizationWord &&
											( olderJustificationItem = existingSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ) ) != NULL &&
											( assumptionJustificationItem = existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != NULL &&
											( primarySpecificationItem = assumptionJustificationItem->primarySpecificationItem() ) != NULL &&
											( secondarySpecificationItem = olderJustificationItem->secondarySpecificationItem() ) != NULL &&
											( foundSpecificationItem = assumptionJustificationItem->secondarySpecificationItem() ) != NULL &&
											!foundSpecificationItem->isOlderItem() &&
											// Justification doesn't exist in specification
											foundSpecificationItem->secondarySpecificationJustificationItem( secondarySpecificationItem ) == NULL &&

											// Add opposite possessive justification
											addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, foundSpecificationItem, primarySpecificationItem, NULL, secondarySpecificationItem, currentContextWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the found specification" );
											}
										else
											{
											// Typical for Spanish
											if( isSpanishCurrentLanguage &&
											alternativeSpecificationItem != NULL )
												{
												if( isOlderSecondarySpecification )
													{
													if( isSecondarySpecificationWordSpanishAmbiguous &&
													!hasNonCompoundSecondarySpecificationCollection &&
													// Justification doesn't exist in specification
													alternativeSpecificationItem->secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == NULL &&

													// Add Spanish justification (self-generated conclusion)
													addJustificationToSpecification( false, false, false, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, alternativeSpecificationItem, NULL, NULL, currentSecondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the found specification" );
													}
												else
													{
													if( !isOlderExistingSpecification &&
													spanishMasculineSpecificationWordItem != NULL &&
													( spanishMasculineSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != NULL &&
													( obsoleteJustificationItem = spanishMasculineSpecificationItem->secondarySpecificationJustificationItem( alternativeSpecificationItem ) ) != NULL )
														{
														if( ( justificationResult = currentContextWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, currentSecondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", currentContextWordItem->anyWordTypeString(), "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
															return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

														if( currentContextWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem, spanishMasculineSpecificationItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption justification by a conclusion justification in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										}
									}

								if( hasNonCompoundSecondarySpecificationCollection &&
								isSecondarySelfGeneratedSpecification &&
								!isSpanishCurrentLanguage &&
								alternativeSpecificationItem != NULL &&
								// Secondary specification word is same as user specification word
								secondarySpecificationWordItem == userSpecificationWordItem &&

								( ( isSecondarySelfGeneratedAssumption &&
								// Justification doesn't exist in specification
								currentSecondarySpecificationItem != alternativeSpecificationItem ) ||

								( !isSecondarySelfGeneratedAssumption &&
								currentSecondarySpecificationItem == alternativeSpecificationItem ) ) &&

								// Make a possessive conditional specification assumption
								adminItem_->makePossessiveConditionalSpecificationAssumption( isSecondaryArchivedAssignment, false, true, WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, secondaryRelationWordTypeNr, ( isSecondarySelfGeneratedAssumption ? alternativeSpecificationItem : currentSecondarySpecificationItem ), NULL, currentContextWordItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a possessive conditional specification assumption during the drawing of a reversible conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								// Alternative specification is same as current secondary specification
								if( currentSecondarySpecificationItem == alternativeSpecificationItem )
									{
									if( isOlderExistingSpecification &&
									isSecondarySelfGeneratedSpecification &&
									existingSpecificationItem != NULL )
										{
										if( existingSpecificationItem->hasCompoundSpecificationCollection() )
											{
											if( isUserExistingSpecification &&
											isSecondarySelfGeneratedAssumption &&
											!isSecondarySpecificationWordSpanishAmbiguous &&
											// Justification doesn't exist in specification
											currentSecondarySpecificationItem->secondarySpecificationJustificationItem( existingSpecificationItem ) == NULL &&
											// Add reversible justification
											addJustificationToSpecification( false, false, true, isSpanishCurrentLanguage, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, NULL, NULL, existingSpecificationItem, generalizationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the current secondary specification" );
											}
										else
											{
											if( !hasCreatedSpecification &&
											isOlderSecondarySpecification &&
											firstUserSpecificationItem == NULL &&
											// Secondary specification word is same as user specification word
											secondarySpecificationWordItem == userSpecificationWordItem &&
											// Justification doesn't exist in specification
											currentSecondarySpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL &&
											( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, secondarySpecificationWordItem, userGeneralizationWordItem ) ) != NULL )
												{
												// Add reversible user justification
												if( addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, NULL, NULL, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

												// Add context
												if( userGeneralizationWordItem->addContext( WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, currentSecondarySpecificationItem->relationContextNr(), NO_COLLECTION_NR, secondarySpecificationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( collectedSpecificationItem = userGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, currentSecondarySpecificationItem->specificationCollectionNr(), NULL ) ) != NULL &&
												( primarySpecificationItem = adminItem_->definitionSpecificationItem( secondarySpecificationWordItem ) ) != NULL )
													{
													// Add collected opposite possessive justification
													if( addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, currentSecondarySpecificationItem, primarySpecificationItem, NULL, collectedSpecificationItem, generalizationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a collected opposite possessive justification to the current secondary specification" );

													// Add opposite possessive conclusion justification
													if( addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, collectedSpecificationItem, primarySpecificationItem, NULL, conclusionSpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
													}
												}
											}
										}
									}
								else	// Alternative is different from current secondary
									{
									if( hasNonCompoundSecondarySpecificationCollection )
										{
										if( alternativeSpecificationItem != NULL )
											{
											// Secondary specification word is same as user specification word
											if( secondarySpecificationWordItem == userSpecificationWordItem )
												{
												if( isSpanishCurrentLanguage )
													{
													// Typical for Spanish
													if( currentSecondarySpecificationItem->hasOnlyOneRelationWord() &&
													// Replace assumption by conclusion
													generalizationWordItem->replaceOrDeleteSpecification( currentSecondarySpecificationItem, alternativeSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a Spanish assumption specification by a conclusion specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}
												else
													{
													if( !isOlderExistingSpecification &&
													alternativeSpecificationItem->isSelfGeneratedAssumption() &&
													( oppositePossessiveDefinitionSpecificationWordItem = adminItem_->oppositePossessiveDefinitionSpecificationWordItem( secondarySpecificationWordItem ) ) != NULL &&
													( foundSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL &&
													foundSpecificationItem->secondarySpecificationJustificationItem( alternativeSpecificationItem ) != NULL &&

													// Replace assumption justification by conclusion justification of involved word
													currentContextWordItem->updateSpecificationOfJustificationsInWord( false, alternativeSpecificationItem, currentSecondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in the justification items in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
													}
												}
											else	// Secondary specification word is different from user specification word
												{
												if( ( assumptionJustificationItem = alternativeSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) ) != NULL &&
												( secondarySpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, false, userSpecificationWordItem, generalizationWordItem ) ) != NULL &&
												// Justification doesn't exist in specification
												alternativeSpecificationItem->secondarySpecificationJustificationItem( secondarySpecificationItem ) == NULL &&
												// Add opposite possessive conclusion justification
												addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, alternativeSpecificationItem, assumptionJustificationItem->primarySpecificationItem(), NULL, secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
												}
											}
										}
									else
										{
										if( isOlderAlternativeSpecification &&
										!isOlderExistingSpecification &&
										isOlderSecondarySpecification )
											{
											// Replace assumption by conclusion
											if( generalizationWordItem->replaceOrDeleteSpecification( ( isSecondarySelfGeneratedAssumption ? currentSecondarySpecificationItem : alternativeSpecificationItem ), ( isSecondarySelfGeneratedAssumption ? alternativeSpecificationItem : currentSecondarySpecificationItem ) ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption specification by a conclusion specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											currentSecondarySpecificationItem = currentSecondarySpecificationItem->updatedSpecificationItem();
											}
										}
									}

								// Typical for Spanish
								if( isSpanishCurrentLanguage )
									{
									if( isSpanishSpecificationFemaleWord &&
									spanishMasculineSpecificationWordItem != NULL )
										{
										if( existingSpecificationItem == NULL )
											{
											if( ( nonCompoundSpanishSpecificationCollectionNr = spanishMasculineSpecificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&

											( ( spanishMasculineSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) == NULL ||
											spanishMasculineSpecificationItem->isSelfGenerated() ) &&

											// Draw Spanish reversible conclusion
											adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, nonCompoundSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem, NULL, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish reversible conclusion in word \"", currentContextWordItem->anyWordTypeString(), "\" and with masculine specification word \"", spanishMasculineSpecificationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											if( hasGeneralizationWordCurrentlyConfirmedSpecification &&
											isOlderExistingSpecification &&
											!hasUserSpecificationAssumptionLevel &&
											secondarySpecificationWordItem == userSpecificationWordItem &&
											existingSpecificationItem->isSelfGeneratedAssumption() )
												{
												if( existingSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the existing specification as concluded assumption" );

												// Write concluded assumption
												if( currentContextWordItem->writeUpdatedSpecification( true, false, false, false, false, existingSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a concluded assumption in word \"", currentContextWordItem->anyWordTypeString(), "\"" );

												if( ( spanishMasculineSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != NULL )
													{
													if( spanishMasculineSpecificationItem->isHiddenSpanishSpecification() )
														{
														if( !spanishMasculineSpecificationItem->hasCurrentCreationSentenceNr() &&
														// Draw Spanish reversible conclusion (hidden)
														adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, userSpecificationItem, NULL, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish reversible conclusion in word \"", currentContextWordItem->anyWordTypeString(), "\" and with masculine specification word \"", spanishMasculineSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														if( !isActiveUserAssignment &&
														spanishMasculineSpecificationItem->nReversibleConclusionJustifications() == spanishMasculineSpecificationItem->nRelationContextWords() )
															{
															if( spanishMasculineSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark the Spanish masculine specification as concluded assumption" );

															// Write concluded assumption
															if( currentContextWordItem->writeUpdatedSpecification( true, false, false, false, false, spanishMasculineSpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a concluded assumption in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
															}
														}
													}
												}
											}
										}
									else
										{
										if( existingSpecificationItem != NULL &&
										currentContextWordItem->isUserGeneralizationWord &&
										existingSpecificationItem->hasCompoundSpecificationCollection() &&
										currentSecondarySpecificationItem->isSelfGeneratedConclusion() &&
										( nonCompoundSpanishSpecificationCollectionNr = secondarySpecificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&

										// Add Spanish non-compound reversible conclusion
										adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, secondaryRelationWordTypeNr, secondarySpecificationWordTypeNr, WORD_TYPE_PROPER_NOUN, nonCompoundSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem, NULL, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish non-compound reversible conclusion in word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
										}
									}

								if( isOlderExistingSpecification &&
								isUserExistingSpecification &&

								( hasNonCompoundUserSpecificationCollection ||
								isActiveUserAssignment ||
								isPossessiveUserSpecification ||
								currentSecondarySpecificationItem != alternativeSpecificationItem ) &&

								isSecondarySelfGeneratedAssumption &&
								!currentSecondarySpecificationItem->isOlderItem() &&
								// Justification doesn't exist in specification
								currentSecondarySpecificationItem->secondarySpecificationJustificationItem( existingSpecificationItem ) == NULL &&

								// Add reversible user justification
								addJustificationToSpecification( false, false, true, false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, NULL, NULL, existingSpecificationItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );
								}
							}
						}
					while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
					}
				}
			while( ( currentSecondarySpecificationItem = currentSecondarySpecificationItem->nextSelectedRelationSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusions( bool hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, bool isArchivedAssignment, bool isCheckingForArchivedAssignment, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *spanishRelationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		bool hasPrimarySpecificationRelationContext;
		bool isPrimarySpecificationArchivedAssignment;
		bool isPossessivePrimarySpecification;
		bool isSpecificationWordSpanishAmbiguous;
		unsigned short assumptionLevel = NO_ASSUMPTION_LEVEL;
		unsigned short partOfAssumptionLevel;
		unsigned short primaryAssumptionLevel;
		JustificationItem *createdJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *partOfDefinitionSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *spanishUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem;
		WordItem *spanishAmbiguousSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawProperNounPartOfConclusions";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		if( ( primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL ) ) == NULL ||
		isSpecificationWordSpanishAmbiguous )
			{
			primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isCheckingForArchivedAssignment, false, false, false, specificationWordItem );

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&

			( primarySpecificationItem == NULL ||
			primarySpecificationItem->isHiddenSpanishSpecification() ) )
				{
				if( spanishRelationWordItem == NULL )
					{
					// Try to find self-generated question, as alternative to hidden specification
					if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, true, specificationWordItem ) ) != NULL &&
					!primarySpecificationItem->isExclusiveSpecification() )
						primarySpecificationItem = NULL;
					}
				else
					{
					if( ( spanishUserSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
					spanishUserSpecificationItem->isPossessive() )
						primarySpecificationItem = spanishUserSpecificationItem;
					else
						primarySpecificationItem = spanishRelationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, true, specificationWordItem, generalizationWordItem );
					}

				if( primarySpecificationItem != NULL &&
				primarySpecificationItem->isHiddenSpanishSpecification() )
					{
					hasSkippedHiddenSpanishSpecification_ = true;
					primarySpecificationItem = NULL;
					}
				}
			}
		else
			{
			if( ( primarySpecificationItem->isUserSpecification() ||
			generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation() ) &&

			( tempSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL )
				primarySpecificationItem = tempSpecificationItem;
			}

		if( primarySpecificationItem != NULL &&
		( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
			isPossessivePrimarySpecification = primarySpecificationItem->isPossessive();
			isPrimarySpecificationArchivedAssignment = primarySpecificationItem->isArchivedAssignment();
			primaryAssumptionLevel = primarySpecificationItem->assumptionLevel();

			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();

			// Typical for Spanish
			if( adminItem_->isSpanishCurrentLanguage() &&
			specificationWordItem->isFemale() &&
			!adminItem_->isUserQuestion() )
				spanishAmbiguousSpecificationWordItem = specificationWordItem->masculineCollectionWordItem();

			// Do for all possessive noun words
			do	{
				if( ( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem->partOfSpecificationItem( specificationWordItem ) ) == NULL &&
				spanishAmbiguousSpecificationWordItem != NULL )
					partOfDefinitionSpecificationItem = currentPossessiveNounWordItem->partOfSpecificationItem( spanishAmbiguousSpecificationWordItem );

				if( partOfDefinitionSpecificationItem != NULL &&
				// Justification doesn't exist in word
				!generalizationWordItem->hasJustification( primarySpecificationItem, NULL, partOfDefinitionSpecificationItem ) )
					{
					partOfSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, currentPossessiveNounWordItem );
					partOfAssumptionLevel = ( partOfSpecificationItem != NULL ? partOfSpecificationItem->assumptionLevel() : NO_ASSUMPTION_LEVEL );

					if( partOfSpecificationItem == NULL ||
					primaryAssumptionLevel < partOfAssumptionLevel ||

					( primaryAssumptionLevel == partOfAssumptionLevel &&

					( !hasPrimarySpecificationRelationContext ||
					isPossessivePrimarySpecification ||
					// Typical for Spanish
					!partOfSpecificationItem->hasPossessivePrimarySpecificationJustification() ) ) ||

					// Typical for Spanish
					( hasPrimarySpecificationRelationContext &&

					( ( partOfSpecificationItem->isConcludedAssumption() &&

					( isPossessivePrimarySpecification ||
					hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation ) ) ||

					( isPossessivePrimarySpecification &&
					partOfAssumptionLevel == NO_ASSUMPTION_LEVEL &&
					primarySpecificationItem->isOlderItem() ) ) ) )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous )
							assumptionLevel = ( hasPrimarySpecificationRelationContext &&
												isPossessivePrimarySpecification &&

												// Primary specification will become an assumption
												// Forcing 'part of' assumption instead conclusion
												( ( partOfSpecificationItem == NULL &&
												!primarySpecificationItem->hasCurrentCreationSentenceNr() ) ||

												// Primary specification isn't recalculated
												// Forcing 'part of' assumption instead conclusion
												( primaryAssumptionLevel == NO_ASSUMPTION_LEVEL &&
												primarySpecificationItem->isSelfGeneratedAssumption() ) ) ? 1 : NO_ASSUMPTION_LEVEL );

						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the challenge document, or http://mafait.org/block1/ and http://mafait.org/block2/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, false, true, false, false, false, false, false, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
							{
							foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

							if( foundSpecificationItem->isSelfGeneratedAssumption() &&
							// Assumption level of hidden Spanish specification isn't calculated. So, calculate now.
							// Recalculate unhidden Spanish assumption
							foundSpecificationItem->recalculateAssumption() != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to calculate an unhidden Spanish assumption" );

							// Unhidden Spanish assumption
							if( primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
							( definitionSpecificationItem = adminItem_->definitionSpecificationItem( currentPossessiveNounWordItem ) ) != NULL &&
							( oppositePossessiveDefinitionSpecificationWordItem = adminItem_->oppositePossessiveDefinitionSpecificationWordItem( specificationWordItem ) ) != NULL &&
							( assumptionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, oppositePossessiveDefinitionSpecificationWordItem, spanishRelationWordItem ) ) != NULL )
								{
								// Add justification (unhidden)
								// Typical for Spanish. Test files: "Complejo (13)" and "Complejo (18)"
								if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, primarySpecificationItem, NULL, definitionSpecificationItem, NULL, assumptionSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

								// Copy specification
								if( ( createAndAssignResult = generalizationWordItem->copySpecificationItem( assumptionSpecificationItem->specificationCollectionNr(), createdJustificationItem, assumptionSpecificationItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy the assumption specification with the created justification item" );

								if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't copy the assumption specification" );

								if( generalizationWordItem->replaceOrDeleteSpecification( assumptionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the copied assumption specification" );
								}
							}
						}
					}
				}
			while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	SpecificationItem *getAnotherPrimarySpecificationItem( WordItem *userSpecificationWordItem )
		{
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *higherLevelPossessiveSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;

		if( userSpecificationWordItem != NULL &&
		( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			// Do for all possessive noun words
			do	{
				// First, try to find a possessive specification on a higher level
				if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
					{
					do	{
						if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
						possessiveSpecificationWordItem->firstSpecificationItem( false, false, false, userSpecificationWordItem ) != NULL )
							higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveSpecificationWordItem == NULL &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
						anotherPrimarySpecificationItem = ( higherLevelPossessiveSpecificationWordItem == NULL ||
															higherLevelPossessiveSpecificationWordItem == possessiveDefinitionSpecificationItem->specificationWordItem() ?
																possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() : possessiveDefinitionSpecificationItem );
					}
				}
			while( anotherPrimarySpecificationItem == NULL &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );
			}

		return anotherPrimarySpecificationItem;
		}


	protected:
	// Constructor

	AdminReasoningNew( AdminItem *adminItem, GlobalVariables *globalVariables )
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

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeAdminReasoningNewVariables()
		{
		hasSkippedHiddenSpanishSpecification_ = false;
		hasSuggestiveQuestionAssumption_ = false;

		adjustedSpanishQuestionSecondarySpecificationItem_ = NULL;
		firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = NULL;
		secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = NULL;
		secondaryDefinitionSpecificationItem_ = NULL;

		adjustedQuestionWordItem_ = NULL;
		}

	bool hasSuggestiveQuestionAssumption()
		{
		return hasSuggestiveQuestionAssumption_;
		}

	signed char addJustificationToSpecification( bool hasFeminineOrMasculineProperNounEnding, bool isAdvancedAttaching, bool isForcingNewJustification, bool isIncrementingOrderNr, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, SpecificationItem *destinationSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned short justificationTypeNr;
		JustificationItem *createdJustificationItem;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addJustificationToSpecification";

		if( destinationSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given destination specification item is undefined" );

		if( destinationSpecificationItem->isUserSpecification() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given destination specification item is a user specification" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationItem == NULL &&
		destinationSpecificationItem->specificationWordItem() != secondarySpecificationItem->specificationWordItem() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given destination specification has a different specification word than the given secondary specification" );

		justificationTypeNr = ( secondarySpecificationItem->hasAssumptionLevel() ? assumptionJustificationTypeNr : conclusionJustificationTypeNr );

		if( ( justificationResult = generalizationWordItem->addJustification( hasFeminineOrMasculineProperNounEnding, isForcingNewJustification, isIncrementingOrderNr, justificationTypeNr, NO_ORDER_NR, globalVariables_->currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

		if( isAdvancedAttaching )
			{
			if( destinationSpecificationItem->attachJustificationToSpecificationAdvanced( createdJustificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the destination specification" );
			}
		else
			{
			if( destinationSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the destination specification" );
			}

		return RESULT_OK;
		}

	signed char askQuestions( bool isCheckingForGapInKnowledge )
		{
		bool isAllowingSpanishPossessiveSpecification;
		bool isArchivedUserAssignment;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *currentTouchedWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestions";

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );

		isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
		userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
		userSpecificationWordItem = userSpecificationItem->specificationWordItem();

		if( userSpecificationItem->isGeneralizationProperNoun() )
			{
			anotherPrimarySpecificationItem = getAnotherPrimarySpecificationItem( userSpecificationWordItem );

			if( askQuestionsAboutSpecificSituations( isArchivedUserAssignment, isSpanishCurrentLanguage, anotherPrimarySpecificationItem, userSpecificationItem, userSpecificationItem->generalizationWordItem(), userSpecificationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about specific situations" );
			}

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
															currentTouchedWordItem->isUserRelationWord );

				if( ( primarySpecificationItem = currentTouchedWordItem->firstPrimarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL &&
				askQuestions( isAllowingSpanishPossessiveSpecification, isArchivedUserAssignment, isCheckingForGapInKnowledge, userSpecificationCollectionNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, currentTouchedWordItem, userSpecificationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );

				if( askQuestionsWithQuestionAsPrimarySpecification( isSpanishCurrentLanguage, currentTouchedWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\" with a question as primary specification" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
			}

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
			adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawReversibleConclusions( bool hasCreatedSpecification, WordItem *generalizationWordItem )
		{
		SpecificationItem *secondarySpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawReversibleConclusions";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem->isProperNounWord() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item isn't a proper noun" );

		// Active assignments
		if( ( secondarySpecificationItem = generalizationWordItem->firstSelectedRelationSpecificationItem( true, false ) ) != NULL &&
		drawReversibleConclusions( hasCreatedSpecification, secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with active assignments" );

		// Archived assignments
		if( ( secondarySpecificationItem = generalizationWordItem->firstSelectedRelationSpecificationItem( true, true ) ) != NULL &&
		drawReversibleConclusions( hasCreatedSpecification, secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with archived assignments" );

		// Specifications
		if( ( secondarySpecificationItem = generalizationWordItem->firstSelectedRelationSpecificationItem( false, false ) ) != NULL &&
		drawReversibleConclusions( hasCreatedSpecification, secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with specifications" );

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusions( bool hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, bool isArchivedAssignment, WordItem *generalizationProperNounWordItem, WordItem *specificationWordItem, WordItem *spanishRelationWordItem )
		{
		WordItem *currentProperNounWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawProperNounPartOfConclusions";

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationProperNounWordItem == NULL ||

		// Try all proper nouns again. Maybe the specification isn't hidden anymore
		( hasSkippedHiddenSpanishSpecification_ &&
		!generalizationProperNounWordItem->isUserGeneralizationWord ) )
			{
			if( ( currentProperNounWordItem = globalVariables_->firstUserProperNounWordItem ) != NULL )
				{
				// Do for all user proper noun words
				do	{
					if( currentProperNounWordItem != spanishRelationWordItem &&
					drawProperNounPartOfConclusions( hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, isArchivedAssignment, true, currentProperNounWordItem, specificationWordItem, spanishRelationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a proper noun 'part of' conclusion in proper noun word \"", currentProperNounWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentProperNounWordItem = currentProperNounWordItem->nextUserProperNounWordItem ) != NULL );
				}
			}
		else
			{
			if( drawProperNounPartOfConclusions( false, isArchivedAssignment, isArchivedAssignment, generalizationProperNounWordItem, specificationWordItem, spanishRelationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a proper noun 'part of' conclusion in word \"", generalizationProperNounWordItem->anyWordTypeString(), "\"" );
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

				( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != NULL &&
				!currentSpecificationItem->hasSpecificationCollection() )
					{
					assumptionLevel = ( secondarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ?
											currentSpecificationItem->assumptionLevel() : secondarySpecificationItem->assumptionLevel() );

					// Collect generalization words of a specification-generalization definition
					if( adminItem_->collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

					currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

					if( currentSpecificationItem->hasCurrentCreationSentenceNr() &&
					// Draw a specification-generalization conclusion (using current specification)
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, currentSpecificationItem->isExclusiveSpecification(), false, currentSpecificationItem->isNegative(), currentSpecificationItem->isPartOf(), currentSpecificationItem->isPossessive(), false, false, false, true, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, currentSpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSpecificationItem, NULL, currentSpecificationItem->specificationWordItem(), currentSpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the current specification" );

					if( !isSelfGenerated &&
					// Draw a specification-generalization conclusion (using secondary specification)
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, secondarySpecificationItem->isExclusiveSpecification(), false, secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPartOf(), secondarySpecificationItem->isPossessive(), false, false, false, true, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, secondarySpecificationItem->specificationWordItem(), secondarySpecificationItem->generalizationWordItem(), NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the secondary specification" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
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
		SpecificationItem *relatedSpecificationItem;
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
				indirectPrimarySpecificationItem->hasCompoundSpecificationCollection() &&
				( currentSpecificationCollectionNr = currentQuestionSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
					{
					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem->findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem->questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

					// Draw an indirectly answered question conclusion
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, indirectPrimarySpecificationItem, NULL, userSpecificationItem, NULL, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentQuestionSpecificationWordItem->anyWordTypeString(), "\"" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
					( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
					( negativeQuestionSpecificationWordItem = relatedSpecificationItem->specificationWordItem() ) != NULL &&
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
							adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, NULL, generalizationWordItem, negativeQuestionSpecificationWordItem, NULL ).result != RESULT_OK )
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

	signed char makeGeneralizationAssumption( bool hasCurrentlyCorrectedAssumptionByKnowledge, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isEveryGeneralization;
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

		if( !primarySpecificationItem->isPartOf() &&
		secondarySpecificationItem->hasCurrentCreationSentenceNr() &&
		// Skip if user specification already exists
		generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) == NULL &&
		// Skip if justification with primary specification already exists
		generalizationWordItem->primarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem ) == NULL )
			{
			isEveryGeneralization = ( generalizationWordTypeNr != WORD_TYPE_PROPER_NOUN );

			// Make a generalization assumption
			// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
			if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			if( ( secondaryDefinitionSpecificationItem_ = createAndAssignResult.createdSpecificationItem ) == NULL &&
			hasCurrentlyCorrectedAssumptionByKnowledge &&
			( olderGeneralizationAssumptionBySpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
			( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != NULL &&
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
						// Draw a specification substitution conclusion or ask a question
						adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, false, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( !globalVariables_->hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned short generalizationWordTypeNr;
		unsigned short specificationWordTypeNr;
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
				generalizationWordTypeNr = userSpecificationItem->generalizationWordTypeNr();
				specificationWordTypeNr = userSpecificationItem->specificationWordTypeNr();

				// Do for all generalization noun specification words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != NULL &&
					( anotherPrimarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != NULL &&
					// Make an only option left assumption
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, NULL, generalizationWordItem, onlyOptionLeftSpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
				if( generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() )
					{
					if( correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem->questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					// Typical for Spanish:
					// Skip making suggestive assumption if specification word is Spanish ambiguous,
					// and if user question has more than one generalization words
					if( !secondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ||
					globalVariables_->nUserGeneralizationWords == 1 )
						{
						hasSuggestiveQuestionAssumption_ = true;

						// Make a suggestive question assumption
						if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question assumption to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	CompoundResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		bool isArchivedAssignment;
		unsigned short generalizationWordTypeNr;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *secondarySpecificationWordItem;
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
					if( !foundSpecificationItem->isPartOf() )
						{
						isArchivedAssignment = foundSpecificationItem->isArchivedAssignment();

						// Definition
						if( ( generalizationWordTypeNr = foundSpecificationItem->generalizationWordTypeNr() ) != WORD_TYPE_PROPER_NOUN &&
						secondaryDefinitionSpecificationItem_ != NULL &&
						foundSpecificationItem->hasCompoundSpecificationCollection() &&
						( secondarySpecificationWordItem = secondaryDefinitionSpecificationItem_->specificationWordItem() ) != NULL &&
						// Make an exclusive specification substitution definition assumption
						// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, foundSpecificationItem->isEveryGeneralization(), false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, NULL, secondaryDefinitionSpecificationItem_, NULL, currentGeneralizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution definition assumption about word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

						// Draw a compound specification substitution conclusion
						if( adminItem_->userSpecificationItem() != NULL &&
						drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, foundSpecificationItem->isExclusiveSpecification(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, foundSpecificationItem, currentGeneralizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						// Draw a specification substitution conclusion or ask a question
						if( adminItem_->userSpecificationItem() != NULL &&
						adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem->hasAssumptionLevel(), isArchivedAssignment, foundSpecificationItem->isExclusiveSpecification(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
						}

					if( !globalVariables_->hasDisplayedWarning &&
					foundSpecificationItem->hasSpecificationCollection() )
						{
						if( ( relatedResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );

						if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL )
							{
							// Draw a specification substitution conclusion or ask a question
							if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->hasAssumptionLevel(), relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem->specificationWordItem(), NULL ) != RESULT_OK )
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

	WordItem *adjustedQuestionWordItem()
		{
		return adjustedQuestionWordItem_;
		}
	};

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
