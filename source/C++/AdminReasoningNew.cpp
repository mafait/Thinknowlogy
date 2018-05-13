/*	Class:			AdminReasoningNew
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (new design)
 *	Version:		Thinknowlogy 2018r1 (ShangDi 上帝)
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

class AdminReasoningNew
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasSkippedHiddenSpanishSpecification_;

	SpecificationItem *adjustedSpanishQuestionSecondarySpecificationItem_;
	SpecificationItem *adjustedSpanishQuestionSpecificationItem_;

	WordItem *adjustedQuestionWordItem_;

	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH];

	AdminItem *adminItem_;
	GlobalVariables *globalVariables_;


	// Private functions

	signed char askQuestions( bool isAllowingSpanishPossessiveSpecification, bool isArchivedUserAssignment, unsigned int userSpecificationCollectionNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *userSpecificationWordItem )
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
		SpecificationItem *adjustedQuestionCompoundSpecificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *archivedPrimarySpecificationItem;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		WordItem *collectionWordItem;
		WordItem *compoundGeneralizationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		BoolResultType boolResult;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		hasFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
		hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
		hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
		isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
		isPossessiveUserSpecification = userSpecificationItem->isPossessive();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
		isUsefulUserSpecificationWithoutRelation = ( hasUserSpecificationCompoundCollection &&
													!hasUserSpecificationRelationContext &&
													isUserGeneralizationWord &&

													( isExclusiveUserSpecification ||
													isPossessiveUserSpecification ||
													globalVariables_->nUserSpecificationWords == 1 ) );
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();

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

						// Typically for Spanish
						if( anotherPrimarySpecificationItem != NULL &&
						// Specification with user specification collection doesn't exist
						generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) == NULL &&

						askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( adminItem_->hasFemaleUserSpecificationWord(), userSpecificationItem->hasNonCompoundSpecificationCollection(), isArchivedAssignment, isArchivedUserAssignment, true, isUserSpecificationWordSpanishAmbiguous, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != NULL && archivedPrimarySpecificationItem->isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), anotherPrimarySpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask Spanish questions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
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
								generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation() )
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
									generalizationWordItem->firstAssignmentOrSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationWordItem ) == NULL ) ) ||

									// Has possibly a gap in the knowledge
									( ( !primarySpecificationItem->isOlderItem() ||
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
											// Ask a specification substitution question
											// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( alternativePrimarySpecificationItem == NULL ? primarySpecificationItem : alternativePrimarySpecificationItem ), NULL, ( isPrimarySpecificationWordSpanishAmbiguous && isUserGeneralizationWord && adjustedSpanishQuestionSecondarySpecificationItem_ != NULL ? adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem ), NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );

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
														adjustedSpanishQuestionSpecificationItem_ = generalizationWordItem->firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem );
													else
														{
														if( adjustedSpanishQuestionSpecificationItem_ != NULL )
															{
															// Write adjusted compound question
															if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, adjustedSpanishQuestionSpecificationItem_ ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted compound question in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

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
												if( primarySpecificationWordItem->hasCollectionNr( userSpecificationCollectionNr ) &&
												exclusiveSecondarySpecificationWordItem->nonCompoundCollectionNr( exclusiveSecondarySpecificationItem->specificationWordTypeNr() ) > NO_COLLECTION_NR &&

												// Find question to be adjusted by compound collection
												( boolResult = generalizationWordItem->findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationCollectionNr, primarySpecificationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );

												if( boolResult.booleanValue )
													adjustedQuestionWordItem_ = generalizationWordItem;
												}
											else
												{
												if( isPrimarySpecificationWordSpanishAmbiguous )
													{
													// Adjust Spanish question
													while( ( adjustedQuestionCompoundSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationCollectionNr, NULL ) ) != NULL &&
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

				askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( false, false, isArchivedAssignment, false, isSpanishCurrentLanguage, false, userSpecificationItem->specificationCollectionNr(), userSpecificationItem, anotherPrimarySpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with possessive user specification as primary specification" );
				}
			else
				{
				if( userGeneralizationWordItem->hasCorrectedAssumptionByOppositeQuestion() &&
				( secondarySpecificationItem = userSpecificationWordItem->firstNonCompoundUserSpecificationItem() ) != NULL &&
				( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Question part doesn't exist
				!userGeneralizationWordItem->hasJustification( userSpecificationItem, NULL, secondarySpecificationItem ) )
					{
					// Create question part based on current user question
					// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, secondarySpecificationItem, NULL, userGeneralizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an invalid assumption generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create a question part based on current user question" );

					if( ( primarySpecificationItem = userGeneralizationWordItem->olderSuggestiveQuestionAssumptionSecondarySpecificationItem() ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( secondarySpecificationItem = primarySpecificationWordItem->firstNonQuestionSpecificationItem() ) != NULL &&
					// Question part doesn't exist
					!userGeneralizationWordItem->hasJustification( primarySpecificationItem, NULL, secondarySpecificationItem ) &&
					( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
					( correctedSpecificationItem = userGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, secondarySpecificationItem->specificationCollectionNr(), secondarySpecificationWordItem ) ) != NULL )
						{
						// Correct invalid assumption by opposite question
						// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, userGeneralizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct an invalid assumption generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the corrected suggestive question assumption" );

						if( userGeneralizationWordItem->replaceOrDeleteSpecification( false, correctedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
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

		if( ( primarySpecificationItem = generalizationWordItem->firstExclusiveNounSpecificationItem() ) != NULL )
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
		( exclusiveSecondarySpecificationItem = anotherPrimarySpecificationWordItem->firstExclusiveNounSpecificationItem() ) != NULL )
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
		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem->firstExclusiveNounSpecificationItem() ) != NULL )
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

	signed char drawPossessiveReversibleConclusions( SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool isOlderSecondarySpecification;
		bool isOlderUserSpecification;
		bool isPossessive;
		bool isSpanishCurrentLanguage;
		bool isSpanishUserAssignment = false;
		bool isSpecificationFemaleWord = false;
		unsigned int hiddenSpanishSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int nonCompoundSpanishSpecificationCollectionNr;
		unsigned int relationContextNr;
		JustificationItem *createdJustificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpanishUserSpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *firstUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *masculineSpecificationItem;
		SpecificationItem *nonCompoundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *userSpecificationItem = NULL;
		WordItem *currentContextWordItem;
		WordItem *firstContextWordItem;
		WordItem *specificationWordItem;
		WordItem *masculineSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawPossessiveReversibleConclusions";

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( firstContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage &&
			( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
			userSpecificationItem->isAssignment() )
				isSpanishUserAssignment = true;

			do	{
				isPossessive = secondarySpecificationItem->isPossessive();

				if( !isPossessive )
					{
					isOlderSecondarySpecification = secondarySpecificationItem->isOlderItem();
					relationContextNr = secondarySpecificationItem->relationContextNr();
					currentContextWordItem = firstContextWordItem;

					if( ( specificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "The secondary specification item has no specification word" );

					if( isSpanishCurrentLanguage )
						{
						isSpecificationFemaleWord = specificationWordItem->isFemale();
						masculineSpecificationWordItem = ( isSpecificationFemaleWord ? specificationWordItem->masculineCollectionWordItem() : NULL );
						existingSpanishUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );
						}

					// Do for all context words
					do	{
						if( currentContextWordItem != generalizationWordItem &&
						// Relation context exists
						currentContextWordItem->hasContextInWord( relationContextNr, specificationWordItem ) )
							{
							existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, !isPossessive, specificationWordItem, generalizationWordItem );

							if( existingSpecificationItem != NULL ||
							currentContextWordItem->isUserGeneralizationWord ||
							currentContextWordItem->isUserRelationWord )
								{
								firstUserSpecificationItem = currentContextWordItem->firstUserSpecificationItem( false, !isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );

								isOlderUserSpecification = ( existingSpecificationItem == NULL &&
															firstUserSpecificationItem != NULL &&
															firstUserSpecificationItem->isOlderItem() );

								// Specification doesn't exist
								// User specification doesn't exist
								if( ( isOlderUserSpecification ||
								firstUserSpecificationItem == NULL ||
								// More specific specification
								!firstUserSpecificationItem->hasRelationContext() ) &&

								( existingSpecificationItem == NULL ||

								// Justification doesn't exist
								( !currentContextWordItem->hasJustification( NULL, NULL, secondarySpecificationItem ) &&
								secondarySpecificationItem == generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, specificationWordItem, currentContextWordItem ) &&

								( existingSpanishUserSpecificationItem == NULL ||
								existingSpecificationItem->secondarySpecificationJustificationItem( existingSpanishUserSpecificationItem ) == NULL ) ) ) )
									{
									if( isSpanishCurrentLanguage )
										{
										hiddenSpanishSpecificationCollectionNr = NO_COLLECTION_NR;

										if( existingSpecificationItem == NULL )
											{
											if( isPossessive &&
											!generalizationWordItem->isOlderItem() &&
											secondarySpecificationItem->isUserSpecification() &&
											( nonCompoundSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
											nonCompoundSpecificationItem->isHiddenSpanishSpecification() )
												hiddenSpanishSpecificationCollectionNr = nonCompoundSpecificationItem->specificationCollectionNr();
											}
										else
											{
											if( existingSpecificationItem->wasHiddenSpanishSpecification() )
												hiddenSpanishSpecificationCollectionNr = existingSpecificationItem->specificationCollectionNr();
											}
										}

									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, !isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, hiddenSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with relation word \"", currentContextWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

									if( secondarySpecificationItem->isReplacedItem() )
										secondarySpecificationItem = secondarySpecificationItem->updatedSpecificationItem();

									if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
										{
										// Created, not found
										if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == NULL )
											{
											// Typically for Spanish
											if( isSpanishCurrentLanguage &&
											!isPossessive &&
											!isOlderUserSpecification &&
											( obsoleteSpecificationItem = currentContextWordItem->noRelationContextSpecificationItem( true, true, ( isSpecificationFemaleWord ? masculineSpecificationWordItem : specificationWordItem ) ) ) != NULL )
												{
												// Write an assumption without relation word
												if( currentContextWordItem->writeUpdatedSpecification( true, false, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", currentContextWordItem->anyWordTypeString(), "\"" );

												// Replace an assumption without relation word
												if( currentContextWordItem->replaceOrDeleteSpecification( false, obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
												}
											}
										else	// Created and found
											{
											if( isOlderUserSpecification )
												{
												// Additional justification. Test file: "Complex (19 - strange)"
												if( ( justificationResult = currentContextWordItem->addJustification( false, false, true, ( foundSpecificationItem->hasAssumptionLevel() ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, foundSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", currentContextWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the created specification" );

												if( currentContextWordItem->updateSpecificationsInJustificationsOfInvolvedWords( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"", currentContextWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}

								// Typically for Spanish
								if( isSpanishCurrentLanguage &&
								isSpecificationFemaleWord &&
								existingSpecificationItem == NULL &&
								masculineSpecificationWordItem != NULL &&
								!currentContextWordItem->hasContextInWord( relationContextNr, masculineSpecificationWordItem ) )
									{
									// Masculine specification doesn't exist
									if( ( ( masculineSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, !isPossessive, masculineSpecificationWordItem, generalizationWordItem ) ) == NULL &&
									!isOlderSecondarySpecification ) ||

									// Masculine specification exists
									( masculineSpecificationItem != NULL &&

									( isSpanishUserAssignment ||
									masculineSpecificationItem->isSelfGenerated() ) &&

									masculineSpecificationItem->secondarySpecificationJustificationItem( secondarySpecificationItem ) == NULL ) )
										{
										nonCompoundSpanishSpecificationCollectionNr = ( masculineSpecificationItem == NULL &&
																						secondarySpecificationItem->isUserSpecification() ? masculineSpecificationWordItem->nonCompoundCollectionNr( WORD_TYPE_NOUN_SINGULAR ) : NO_COLLECTION_NR );

										if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, !isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, nonCompoundSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, currentContextWordItem, masculineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish possessive reversible conclusion with relation word \"", currentContextWordItem->anyWordTypeString(), "\" and masculine specification word \"", masculineSpecificationWordItem->anyWordTypeString(), "\"" );

										if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
										( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
										!foundSpecificationItem->isHiddenSpanishSpecification() )
											{
											// Additional Spanish justification (User)
											if( ( justificationResult = currentContextWordItem->addJustification( false, false, true, ( foundSpecificationItem->hasAssumptionLevel() ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, foundSpecificationItem, NULL, createdSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish justification to word \"", currentContextWordItem->anyWordTypeString(), "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
												return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

											if( createdSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
											}
										}
									else
										{
										if( userSpecificationItem != NULL &&
										( definitionSpecificationItem = masculineSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&

										// Add Spanish - Feminine / definition specification
										adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, true, false, false, !isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish possessive reversible conclusion with relation word \"", currentContextWordItem->anyWordTypeString(), "\" and masculine specification word \"", masculineSpecificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						}
					while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
					}
				}
			while( ( secondarySpecificationItem = secondarySpecificationItem->nextNonExclusiveSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusions( bool hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, bool isArchivedAssignment, bool isCheckingForArchivedAssignment, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *spanishRelationWordItem )
		{
		bool isPrimarySpecificationArchivedAssignment;
		bool isSpecificationWordSpanishAmbiguous;
		unsigned short assumptionLevel = NO_ASSUMPTION_LEVEL;
		JustificationItem *createdJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem *partOfDefinitionSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *spanishUserSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem;
		WordItem *spanishAmbiguousSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawProperNounPartOfConclusions";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		// Select primary specification
		primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL );

		if( primarySpecificationItem == NULL ||
		isSpecificationWordSpanishAmbiguous )
			{
			primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isCheckingForArchivedAssignment, false, false, false, specificationWordItem );

			// Typically for Spanish
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

					if( primarySpecificationItem != NULL &&
					!primarySpecificationItem->hasRelationContext() )
						primarySpecificationItem = NULL;
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
			if( !isArchivedAssignment &&
			primarySpecificationItem->hasRelationContext() &&

			( primarySpecificationItem->isUserSpecification() ||
			generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation() ) &&

			( tempSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL )
				primarySpecificationItem = tempSpecificationItem;
			}

		if( primarySpecificationItem != NULL &&
		( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			// Typically for Spanish
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
				// Justification doesn't exist
				!generalizationWordItem->hasJustification( primarySpecificationItem, NULL, partOfDefinitionSpecificationItem ) )
					{
					partOfSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, currentPossessiveNounWordItem );

					if( partOfSpecificationItem == NULL ||
					primarySpecificationItem->assumptionLevel() < partOfSpecificationItem->assumptionLevel() ||

					( primarySpecificationItem->assumptionLevel() == partOfSpecificationItem->assumptionLevel() &&
					// Justification doesn't exist
					partOfSpecificationItem->primarySpecificationJustificationItem( primarySpecificationItem ) == NULL &&

					( !primarySpecificationItem->hasRelationContext() ||
					primarySpecificationItem->isPossessive() ||
					// Typically for Spanish
					!partOfSpecificationItem->hasPossessivePrimarySpecificationJustification() ) ) ||

					// Typically for Spanish
					( primarySpecificationItem->hasRelationContext() &&

					( ( partOfSpecificationItem->isConcludedAssumption() &&

					( primarySpecificationItem->isPossessive() ||
					generalizationWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() ) ) ||

					( !partOfSpecificationItem->hasAssumptionLevel() &&
					primarySpecificationItem->isPossessive() &&
					primarySpecificationItem->isOlderItem() &&
					primarySpecificationItem->isSelfGeneratedAssumption() ) ) ) )
						{
						isPrimarySpecificationArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

						// Typically for Spanish
						if( isSpecificationWordSpanishAmbiguous )
							assumptionLevel = ( primarySpecificationItem->hasRelationContext() &&
												primarySpecificationItem->isPossessive() &&

												// Primary specification will become an assumption
												// Forcing 'part of' assumption instead conclusion
												( ( partOfSpecificationItem == NULL &&
												!primarySpecificationItem->hasCurrentCreationSentenceNr() ) ||

												// Primary specification isn't recalculated
												// Forcing 'part of' assumption instead conclusion
												( !primarySpecificationItem->hasAssumptionLevel() &&
												primarySpecificationItem->isSelfGeneratedAssumption() ) ) ? 1 : NO_ASSUMPTION_LEVEL );

						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the challenge document, or http://mafait.org/block1/ and http://mafait.org/block2/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, false, true, false, false, false, false, false, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
							{
							foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

							if( foundSpecificationItem->isSelfGeneratedAssumption() &&
							// Recalculate unhidden Spanish assumption
							foundSpecificationItem->recalculateAssumption() != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate an unhidden Spanish assumption" );

							// Unhidden Spanish assumption
							if( spanishRelationWordItem != NULL &&
							primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
							( oppositePossessiveDefinitionSpecificationItem = adminItem_->oppositePossessiveDefinitionSpecificationItem() ) != NULL &&
							( possessiveDefinitionSpecificationWordItem = adminItem_->possessiveDefinitionSpecificationWordItem() ) != NULL &&
							( assumptionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, spanishRelationWordItem ) ) != NULL &&
							assumptionSpecificationItem->primarySpecificationJustificationItem( primarySpecificationItem ) == NULL &&
							// Feminine specification doesn't exist
							spanishRelationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, false, specificationWordItem->feminineCollectionWordItem(), generalizationWordItem ) == NULL )
								{
								// Additional justification (unhidden)
								if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, primarySpecificationItem, NULL, oppositePossessiveDefinitionSpecificationItem, NULL, assumptionSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

								if( ( createAndAssignResult = generalizationWordItem->createSpecificationItem( assumptionSpecificationItem->isAssignment(), assumptionSpecificationItem->isInactiveAssignment(), assumptionSpecificationItem->isArchivedAssignment(), assumptionSpecificationItem->isAnsweredQuestion(), assumptionSpecificationItem->isCharacteristicFor(), assumptionSpecificationItem->isConcludedAssumption(), assumptionSpecificationItem->isConditional(), assumptionSpecificationItem->isCorrectedAssumption(), assumptionSpecificationItem->isEveryGeneralization(), assumptionSpecificationItem->isExclusiveGeneralization(), assumptionSpecificationItem->isExclusiveSpecification(), assumptionSpecificationItem->isNegative(), assumptionSpecificationItem->isPartOf(), assumptionSpecificationItem->isPossessive(), assumptionSpecificationItem->isSpecific(), assumptionSpecificationItem->isSpecificationGeneralization(), assumptionSpecificationItem->isUncountableGeneralizationNoun(), assumptionSpecificationItem->isUniqueUserRelation(), assumptionSpecificationItem->isValueSpecification(), assumptionSpecificationItem->assignmentLevel(), assumptionSpecificationItem->assumptionLevel(), assumptionSpecificationItem->languageNr(), assumptionSpecificationItem->prepositionParameter(), assumptionSpecificationItem->questionParameter(), assumptionSpecificationItem->generalizationWordTypeNr(), assumptionSpecificationItem->specificationWordTypeNr(), assumptionSpecificationItem->relationWordTypeNr(), assumptionSpecificationItem->specificationCollectionNr(), assumptionSpecificationItem->generalizationContextNr(), assumptionSpecificationItem->specificationContextNr(), assumptionSpecificationItem->relationContextNr(), assumptionSpecificationItem->originalSentenceNr(), assumptionSpecificationItem->activeSentenceNr(), assumptionSpecificationItem->inactiveSentenceNr(), assumptionSpecificationItem->archivedSentenceNr(), assumptionSpecificationItem->nContextRelations(), createdJustificationItem, assumptionSpecificationItem->specificationWordItem(), assumptionSpecificationItem->specificationString(), assumptionSpecificationItem->storedSentenceString(), assumptionSpecificationItem->storedSentenceWithOnlyOneSpecificationString() ) ).result != RESULT_OK )
									return adminItem_->startError( functionNameString, moduleNameString_, "I failed to create a copy of the assumption specification with the created justification item" );

								if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't copy the assumption specification" );

								if( generalizationWordItem->replaceOrDeleteSpecification( false, assumptionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
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
															higherLevelPossessiveSpecificationWordItem == possessiveDefinitionSpecificationItem->specificationWordItem() ? possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() : possessiveDefinitionSpecificationItem );
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
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasSkippedHiddenSpanishSpecification_ = false;

		adjustedSpanishQuestionSecondarySpecificationItem_ = NULL;
		adjustedSpanishQuestionSpecificationItem_ = NULL;

		adjustedQuestionWordItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminReasoningNew" );

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
				askQuestions( isAllowingSpanishPossessiveSpecification, isArchivedUserAssignment, userSpecificationCollectionNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, currentTouchedWordItem, userSpecificationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );

				if( askQuestionsWithQuestionAsPrimarySpecification( isSpanishCurrentLanguage, currentTouchedWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\" with a question as primary specification" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
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
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByOppositeQuestion";

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
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, oldSuggestiveQuestionAssumptionAssignmentItem, NULL, generalizationWordItem, oldSuggestiveSpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oldSuggestiveSpecificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, secondarySpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		if( ( correctedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem->correctedAssumptionByOppositeQuestionJustificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the old corrected assumption by opposite question justification item in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		// Change primary user specification by primary corrected question specification
		if( correctedAssumptionByOppositeQuestionJustificationItem->changePrimarySpecification( createdNewSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the primary specification of the corrected assumption by opposite question justification item" );

		if( ( secondarySpecificationWordItem = correctedAssumptionByOppositeQuestionJustificationItem->secondarySpecificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );

		// Correct generalization assumption
		if( ( primarySpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, false, false, secondarySpecificationWordItem ) ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );

		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdOldSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution assumption" );

		if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the corrected generalization assumption" );

		if( foundSpecificationItem->attachJustificationToSpecification( correctedAssumptionByOppositeQuestionJustificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );

		return RESULT_OK;
		}

	signed char drawPossessiveReversibleConclusions( WordItem *generalizationWordItem )
		{
		SpecificationItem *secondarySpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawPossessiveReversibleConclusions";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem->isProperNounWord() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item isn't a proper noun" );
/*
		// Assignments
		if( ( secondarySpecificationItem = generalizationWordItem->firstNonExclusiveSpecificationItem( true ) ) != NULL &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions with assignments" );
*/
		// Specifications
		if( ( secondarySpecificationItem = generalizationWordItem->firstNonExclusiveSpecificationItem( false ) ) != NULL &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions with specifications" );

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusions( bool hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, bool isArchivedAssignment, WordItem *generalizationProperNounWordItem, WordItem *specificationWordItem, WordItem *spanishRelationWordItem )
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
					drawProperNounPartOfConclusions( hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, isArchivedAssignment, true, currentProperNounWordItem, specificationWordItem, spanishRelationWordItem ) != RESULT_OK )
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

	WordItem *adjustedQuestionWordItem()
		{
		return adjustedQuestionWordItem_;
		}
	};

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
