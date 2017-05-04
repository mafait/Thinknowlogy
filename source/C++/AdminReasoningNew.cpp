/*	Class:			AdminReasoningNew
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (new design)
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

class AdminReasoningNew
	{
	friend class AdminItem;

	// Private constructed variables

	SpecificationItem *adjustedSpanishQuestionSecondarySpecificationItem_;
	SpecificationItem *adjustedSpanishQuestionSpecificationItem_;

	WordItem *adjustedQuestionWordItem_;

	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;


	// Private functions

	signed char askQuestions( bool isAllowingSpanishPossessiveSpecification, bool isArchivedUserAssignment, unsigned int userSpecificationCollectionNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *userSpecificationWordItem )
		{
		bool hasExclusiveSecondarySpecificationCompoundCollection;
		bool hasFeminineOrMasculineProperNameEnding;
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
		SpecificationItem *adjustedQuestionCompoundSpecificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *archivedPrimarySpecificationItem;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		WordItem *collectionWordItem;
		WordItem *compoundGeneralizationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askQuestions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		hasFeminineOrMasculineProperNameEnding = generalizationWordItem->hasFeminineOrMasculineProperNameEnding();
		hasUserSpecificationCompoundCollection = userSpecificationItem->hasSpecificationCompoundCollection();
		hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
		isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
		isPossessiveUserSpecification = userSpecificationItem->isPossessive();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
		isUsefulUserSpecificationWithoutRelation = ( hasUserSpecificationCompoundCollection &&
													!hasUserSpecificationRelationContext &&
													isUserGeneralizationWord &&

													( isExclusiveUserSpecification ||
													isPossessiveUserSpecification ||
													commonVariables_->nUserSpecificationWords == 1 ) );
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();

		do	{
			if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL )
				{
				hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
				isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();
				isPossessivePrimarySpecification = primarySpecificationItem->isPossessive();

				if( isPossessivePrimarySpecification )
					{
					if( !hasFeminineOrMasculineProperNameEnding )
						{
						archivedPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, true, NO_QUESTION_PARAMETER, primarySpecificationWordItem );

						// Typically for the Spanish language
						if( anotherPrimarySpecificationItem != NULL &&
						// Specification with user specification collection doesn't exist
						generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) == NULL &&

						askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( adminItem_->hasFemaleUserSpecificationWord(), userSpecificationItem->hasSpecificationNonCompoundCollection(), isArchivedAssignment, isArchivedUserAssignment, true, isUserSpecificationWordSpanishAmbiguous, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != NULL && archivedPrimarySpecificationItem->isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), anotherPrimarySpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask Spanish questions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
						}
					}
				else
					{
					hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasSpecificationCompoundCollection();

					if( ( !hasFeminineOrMasculineProperNameEnding ||
					// If gender is known, skip gender-related question parts
					hasPrimarySpecificationCompoundCollection ) &&

					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( exclusiveSecondarySpecificationItem = primarySpecificationWordItem->firstExclusiveNounSpecificationItem() ) != NULL )
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
							if( isPrimaryUserSpecification )
								{
								if( !isPrimarySpecificationWordSpanishAmbiguous &&
								primarySpecificationItem->isUserAssignment() )
									alternativePrimarySpecificationItem = adminItem_->simpleUserSpecificationItem();
								}
							else
								{
								if( !hasUserSpecificationRelationContext &&
								generalizationWordItem->hasConfirmedSpecificationButNoRelation() )
									alternativePrimarySpecificationItem = userSpecificationItem;
								}
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
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a Spanish question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
								}
							else
								{
								if( isPrimarySpecificationSelfGeneratedAssumption &&
								( alternativePrimarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, primarySpecificationWordItem, NULL ) ) != NULL )
									{
									// Skip if conclusion is not older
									if( !alternativePrimarySpecificationItem->isOlderItem() )
										alternativePrimarySpecificationItem = NULL;
									}
								}
							}

						do	{
							hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem->hasSpecificationCompoundCollection();

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
								!generalizationWordItem->hasJustification( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, ( alternativePrimarySpecificationItem == NULL ? primarySpecificationItem : alternativePrimarySpecificationItem ), NULL, exclusiveSecondarySpecificationItem ) ) ) &&

								( collectionWordItem = exclusiveSecondarySpecificationWordItem->collectionWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL )
									{
									// Adjusted question
									if( alternativePrimarySpecificationItem != NULL ||

									( hasPrimarySpecificationCompoundCollection &&
									!isUserSpecificationWordSpanishAmbiguous &&

									( adjustedQuestionWordItem_ != NULL ||
									isCompoundGeneralizationWordSpanishAmbiguous ) ) ||

									( isUserSpecificationWordSpanishAmbiguous &&
									compoundGeneralizationWordItem != primarySpecificationWordItem &&

									( !hasPrimarySpecificationRelationContext ||
									// Same specification but without relation context doesn't exists. Spanish test file: "Esta información es más específico"
									generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationWordItem ) == NULL ) ) ||

									generalizationWordItem->hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationItem ) )
										{
										// Avoid asking invalid questions:
										// 1) Check for existing knowledge with same specification collection
										if( generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, exclusiveSecondarySpecificationWordItem ) == NULL &&
										// 2) Check for existing negative specification. Test file: "Skip invalid questions (1)"
										generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, NO_QUESTION_PARAMETER, collectionWordItem, NULL ) == NULL &&
										// 3) Check for existing user specification. Test file: "Skip invalid questions (2)"
										generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, collectionWordItem ) == NULL )
											{
											// Ask a specification substitution question
											// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( alternativePrimarySpecificationItem == NULL ? primarySpecificationItem : alternativePrimarySpecificationItem ), NULL, ( isPrimarySpecificationWordSpanishAmbiguous && isUserGeneralizationWord && adjustedSpanishQuestionSecondarySpecificationItem_ != NULL ? adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem ), NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );

											// Adjusted question
											if( isPrimarySpecificationWordSpanishAmbiguous &&
											createAndAssignResult.createdSpecificationItem == NULL &&
											hasExclusiveSecondarySpecificationCompoundCollection &&
											isUserGeneralizationWord )
												{
												if( adjustedSpanishQuestionSecondarySpecificationItem_ == NULL )
													adjustedSpanishQuestionSecondarySpecificationItem_ = exclusiveSecondarySpecificationItem;
												else
													{
													if( adjustedQuestionWordItem_ == NULL )
														adjustedSpanishQuestionSpecificationItem_ = generalizationWordItem->firstActiveAssignmentOrSpecificationItem( isArchivedAssignment, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationWordItem );
													else
														{
														if( adjustedSpanishQuestionSpecificationItem_ != NULL )
															{
															// Write adjusted compound question
															if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, adjustedSpanishQuestionSpecificationItem_ ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

															adjustedSpanishQuestionSpecificationItem_ = NULL;
															}
														}
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
										adjustedQuestionWordItem_ == NULL )
											{
											if( adjustedSpanishQuestionSecondarySpecificationItem_ == NULL )
												{
												if( exclusiveSecondarySpecificationWordItem->nonCompoundCollectionNr( exclusiveSecondarySpecificationItem->specificationWordTypeNr() ) > NO_COLLECTION_NR &&
												// Find question to be adjusted
												findQuestionToBeAdjustedByCompoundCollection( isArchivedAssignment, exclusiveSecondarySpecificationCollectionNr, adjustedSpanishQuestionSpecificationItem_, generalizationWordItem, primarySpecificationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );
												}
											else
												{
												if( isPrimarySpecificationWordSpanishAmbiguous )
													{
													// Adjust Spanish question
													while( ( adjustedQuestionCompoundSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL ) ) != NULL &&
													adjustedQuestionCompoundSpecificationItem != adjustedSpanishQuestionSpecificationItem_ &&
													adjustedQuestionCompoundSpecificationItem->isOlderItem() )
														{
														if( generalizationWordItem->replaceOrDeleteSpecification( false, adjustedQuestionCompoundSpecificationItem, adjustedSpanishQuestionSpecificationItem_ ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a question part" );
														}

													adjustedQuestionWordItem_ = generalizationWordItem;
													}
												}
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "askQuestionsAboutSpecificSituations";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( userGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user generalization word item is undefined" );

		if( userSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification word item is undefined" );

		if( userSpecificationItem->isExclusiveSpecification() )
			{
			// Question part doesn't exist
			if( !userGeneralizationWordItem->hasJustification( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, userSpecificationItem, NULL, NULL ) &&

			// Ask question after more specific non-exclusive user specification
			// Not yet explained in the design
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, userGeneralizationWordItem, userSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about the current user sentence" );
			}
		else
			{
			if( userSpecificationItem->isPossessive() )
				{
				if( anotherPrimarySpecificationItem != NULL &&
				!userGeneralizationWordItem->hasFeminineOrMasculineProperNameEnding() &&

				askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( false, false, isArchivedAssignment, false, isSpanishCurrentLanguage, false, userSpecificationItem->specificationCollectionNr(), userSpecificationItem, anotherPrimarySpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with possessive user specification as primary specification" );
				}
			else
				{
				if( userGeneralizationWordItem->hasCorrectedAssumptionByOppositeQuestion() &&
				( secondarySpecificationItem = userSpecificationWordItem->firstNonCompoundUserSpecificationItem() ) != NULL &&
				( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Question part doesn't exist
				!userGeneralizationWordItem->hasJustification( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, userSpecificationItem, NULL, secondarySpecificationItem ) )
					{
					// Create question part based on current user question
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, secondarySpecificationItem, NULL, userGeneralizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an invalid assumption generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create a question part based on current user question" );

					if( ( primarySpecificationItem = userGeneralizationWordItem->olderSuggestiveQuestionAssumptionSecondarySpecificationItem() ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( secondarySpecificationItem = primarySpecificationWordItem->firstNonQuestionSpecificationItem() ) != NULL &&
					// Question part doesn't exist
					!userGeneralizationWordItem->hasJustification( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, NULL, secondarySpecificationItem ) &&
					( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
					( correctedSpecificationItem = userGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, secondarySpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != NULL )
						{
						// Correct invalid assumption by opposite question
						// Not yet explained in the design
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, userGeneralizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct an invalid assumption generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the corrected suggestive question assumption" );

						if( userGeneralizationWordItem->replaceOrDeleteSpecification( false, correctedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\"" );

						if( ( obsoleteJustificationItem = userGeneralizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, createdSpecificationItem ) ) != NULL &&
						// Remove justification from corrected assumption
						userGeneralizationWordItem->replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete an obsolete justification item" );

						// Clean-up obsolete justifications
						if( userGeneralizationWordItem->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\"" );
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "askQuestionsWithQuestionAsPrimarySpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem->firstExclusiveNounSpecificationItem() ) != NULL )
			{
			do	{
				if( ( !isSpanishCurrentLanguage ||
				primarySpecificationItem->isSelfGeneratedQuestion() ) &&

				( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
				( secondarySpecificationItem = primarySpecificationWordItem->firstNonCompoundUserSpecificationItem() ) != NULL &&
				// Question part doesn't exist
				!generalizationWordItem->hasJustification( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, NULL, secondarySpecificationItem ) &&

				( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL )
					{
					isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

					// Ask a specification substitution question
					// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with a question as primary specification in generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
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
		bool isOlderPrimarySpecification;
		bool isUserGeneralizationWord;
		bool isOlderExistingPrimarySpecificationSpanishAmbiguous = false;
		unsigned short generalizationWordTypeNr;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *existingPrimarySpecificationItem = NULL;
		WordItem *anotherPrimarySpecificationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( anotherPrimarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given another primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Question about generalization word
		if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem->specificationWordItem() ) != NULL &&
		( exclusiveSecondarySpecificationItem = anotherPrimarySpecificationWordItem->firstExclusiveNounSpecificationItem() ) != NULL )
			{
			hasFoundAnswerToQuestion = generalizationWordItem->hasFoundAnswerToQuestion();
			hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasSpecificationCompoundCollection();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();
			primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr();

			if( isSpanishCurrentLanguage &&
			( existingPrimarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, NO_QUESTION_PARAMETER, primarySpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL ) ) != NULL &&
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
					if( !generalizationWordItem->hasJustification( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem ) &&
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

						( hasPrimarySpecificationCompoundCollection &&
						!hasUserSpecificationNonCompoundCollection &&
						existingPrimarySpecificationItem == NULL ) ||

						( !hasFoundAnswerToQuestion &&
						!isArchivedUserAssignment &&
						isOlderExistingPrimarySpecificationSpanishAmbiguous ) ||

						( hasFemaleUserSpecificationWord &&

						( !isOlderPrimarySpecification ||
						userSpecificationCollectionNr == NO_COLLECTION_NR ) ) ) &&

						( compoundGeneralizationWordItem == anotherPrimarySpecificationWordItem ||
						anotherPrimarySpecificationWordItem->hasCollectionNr( exclusiveSecondarySpecificationCollectionNr ) ) ) ) &&

						// Negative specification doesn't exist
						generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, false, NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL ) == NULL ) )
							{
							// Ask a specification substitution question
							// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
							if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( userSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification word item is undefined" );

		// Question about relation word
		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem->firstExclusiveNounSpecificationItem() ) != NULL )
			{
			do	{
				if( !generalizationWordItem->hasJustification( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, userSpecificationItem, NULL, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL &&

				// Ask a specification substitution question
				// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveNounSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char findQuestionToBeAdjustedByCompoundCollection( bool isArchivedAssignment, unsigned int exclusiveSecondarySpecificationCollectionNr, SpecificationItem *replacingSpecificationItem, WordItem *generalizationWordItem, WordItem *primarySpecificationWordItem )
		{
		SpecificationItem *adjustedQuestionCompoundSpecificationItem;
		SpecificationItem *lastAdjustedQuestionCompoundSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findQuestionToBeAdjustedByCompoundCollection";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification word item is undefined" );

		if( exclusiveSecondarySpecificationCollectionNr <= NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given exclusive secondary specification collection number is undefined" );

				// Check for adjusted compound question
		while( ( adjustedQuestionCompoundSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL ) ) != NULL &&
		adjustedQuestionCompoundSpecificationItem->isOlderItem() &&
		// No definition specification exists
		primarySpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, adjustedQuestionCompoundSpecificationItem->specificationWordItem() ) == NULL )
			{
			if( generalizationWordItem->replaceOrDeleteSpecification( false, adjustedQuestionCompoundSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a question part" );

			lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem;
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != NULL )
			{
			// Write adjusted compound question
			if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, lastAdjustedQuestionCompoundSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			adjustedQuestionWordItem_ = generalizationWordItem;
			}

		return RESULT_OK;
		}

	SpecificationItem *getAnotherPrimarySpecificationItem( WordItem *userSpecificationWordItem )
		{
		SpecificationItem *possessiveDefinitionSpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		WordItem *currentPossessiveNounWordItem;
		WordItem *possessiveSpecificationWordItem;
		WordItem *higherLevelPossessiveSpecificationWordItem = NULL;

		if( userSpecificationWordItem != NULL &&
		( currentPossessiveNounWordItem = commonVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			// Do for all possessive noun words
			do	{
				// First, try to find a possessive specification on a higher level
				if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
					{
					do	{
						if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
						possessiveSpecificationWordItem->firstSpecificationItem( false, false, NO_QUESTION_PARAMETER, userSpecificationWordItem ) != NULL )
							higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveSpecificationWordItem == NULL &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
						anotherPrimarySpecificationItem = ( higherLevelPossessiveSpecificationWordItem == NULL || higherLevelPossessiveSpecificationWordItem == possessiveDefinitionSpecificationItem->specificationWordItem() ? possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() : possessiveDefinitionSpecificationItem );
					}
				}
			while( anotherPrimarySpecificationItem == NULL &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );
			}

		return anotherPrimarySpecificationItem;
		}


	protected:
	// Constructor

	AdminReasoningNew( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		adjustedSpanishQuestionSecondarySpecificationItem_ = NULL;
		adjustedSpanishQuestionSpecificationItem_ = NULL;

		adjustedQuestionWordItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminReasoningNew" );

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

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeAdminReasoningNewVariables()
		{
		adjustedSpanishQuestionSecondarySpecificationItem_ = NULL;
		adjustedSpanishQuestionSpecificationItem_ = NULL;

		adjustedQuestionWordItem_ = NULL;
		}

	signed char askQuestions()
		{
		bool isAllowingSpanishPossessiveSpecification;
		bool isArchivedUserAssignment;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem = NULL;
		WordItem *currentTouchedProperNameWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "askQuestions";

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );

		isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
		userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
		userSpecificationWordItem = userSpecificationItem->specificationWordItem();

		if( userSpecificationItem->isGeneralizationProperName() )
			{
			anotherPrimarySpecificationItem = getAnotherPrimarySpecificationItem( userSpecificationWordItem );

			if( askQuestionsAboutSpecificSituations( isArchivedUserAssignment, isSpanishCurrentLanguage, anotherPrimarySpecificationItem, userSpecificationItem, userSpecificationItem->generalizationWordItem(), userSpecificationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about specific situations" );
			}

		if( ( currentTouchedProperNameWordItem = commonVariables_->firstTouchedProperNameWordItem ) != NULL )
			{
			// Do for all proper name words touched during current sentence
			do	{
				isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
															currentTouchedProperNameWordItem->isUserRelationWord );

				if( ( primarySpecificationItem = currentTouchedProperNameWordItem->firstPrimarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL &&
				askQuestions( isAllowingSpanishPossessiveSpecification, isArchivedUserAssignment, userSpecificationCollectionNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, currentTouchedProperNameWordItem, userSpecificationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper name word \"", currentTouchedProperNameWordItem->anyWordTypeString(), "\"" );

				if( askQuestionsWithQuestionAsPrimarySpecification( isSpanishCurrentLanguage, currentTouchedProperNameWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper name word \"", currentTouchedProperNameWordItem->anyWordTypeString(), "\" with a question as primary specification" );
				}
			while( ( currentTouchedProperNameWordItem = currentTouchedProperNameWordItem->nextTouchedProperNameWordItem ) != NULL );
			}

		return RESULT_OK;
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
