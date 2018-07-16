/*	Class:			AdminReasoningNew
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (redesign)
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

class AdminReasoningNew
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasSkippedHiddenSpanishSpecification_;

	SpecificationItem *adjustedSpanishQuestionSecondarySpecificationItem_;

	WordItem *adjustedQuestionWordItem_;

	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH];

	AdminItem *adminItem_;
	GlobalVariables *globalVariables_;


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
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *archivedPrimarySpecificationItem;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *questionPrimarySpecificationItem;
		SpecificationItem *questionSecondarySpecificationItem;
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

						// Typical for Spanish
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
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a Spanish question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
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
																					adjustedSpanishQuestionSecondarySpecificationItem_ != NULL ? adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem );

											// Ask a specification substitution question
											// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, questionPrimarySpecificationItem, NULL, questionSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question in word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );

											// Adjusted question
											if( isPrimarySpecificationWordSpanishAmbiguous &&
											adjustedSpanishQuestionSecondarySpecificationItem_ == NULL &&
											createAndAssignResult.createdSpecificationItem == NULL )
												adjustedSpanishQuestionSecondarySpecificationItem_ = exclusiveSecondarySpecificationItem;
											}
										else
											hasSkippedAskingQuestion = true;
										}
									else
										{
										if( isUserGeneralizationWord &&
										hasExclusiveSecondarySpecificationCompoundCollection &&
										adjustedQuestionWordItem_ == NULL &&
										adjustedSpanishQuestionSecondarySpecificationItem_ == NULL )
											{
											if( primarySpecificationWordItem->hasCollectionNr( userSpecificationCollectionNr ) &&
											exclusiveSecondarySpecificationWordItem->nonCompoundCollectionNr() > NO_COLLECTION_NR &&

											// Find question to be adjusted by compound collection
											( boolResult = generalizationWordItem->findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationCollectionNr, primarySpecificationWordItem ) ).result != RESULT_OK )
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

					if( ( primarySpecificationItem = userGeneralizationWordItem->suggestiveQuestionAssumptionSecondarySpecificationItem() ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( secondarySpecificationItem = primarySpecificationWordItem->firstNonQuestionSpecificationItem() ) != NULL &&
					( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
					( correctedSpecificationItem = userGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, secondarySpecificationItem->specificationCollectionNr(), secondarySpecificationWordItem ) ) != NULL )
						{
						// Correct invalid assumption by opposite question
						// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, userGeneralizationWordItem, secondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct an invalid assumption generalization word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );

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

	signed char drawPossessiveReversibleConclusions( SpecificationItem *currentSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool isActiveUserAssignment = false;
		bool isAssignment;
		bool isExclusiveGeneralizationAssignment;
		bool isExclusiveSpecification;
		bool isForcingNewSpanishJustification = false;
		bool isOlderExistingSpecification;
		bool isOlderSecondarySpecification;
		bool isOlderUserSpecification;
		bool isPossessive;
		bool isPossessiveUserSpecification = false;
		bool isSecondaryArchivedAssignment;
		bool isSecondaryUserAssignment;
		bool isSelfGenerated;
		bool isSpanishCurrentLanguage;
		bool isSpanishSpecificationFemaleWord = false;
		bool isUniqueUserRelation;
		bool isUserGeneralizationWord;
		unsigned short generalizationWordTypeNr;
		unsigned short specificationWordTypeNr;
		unsigned short relationWordTypeNr;
		unsigned int nonCompoundSpanishSpecificationCollectionNr;
		unsigned int spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int relationContextNr;
		unsigned int specificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *currentJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *olderJustificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *firstUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *reversedUserSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *spanishMasculineSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *firstContextWordItem;
		WordItem *spanishMasculineSpecificationWordItem = NULL;
		WordItem *specificationWordItem;
		WordItem *userGeneralizationWordItem = NULL;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawPossessiveReversibleConclusions";

		if( currentSecondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( firstContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
				{
				isPossessiveUserSpecification = userSpecificationItem->isPossessive();
				userGeneralizationWordItem = userSpecificationItem->generalizationWordItem();

				if( isPossessiveUserSpecification )
					{
					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					adminItem_->hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore() &&
					( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL &&
					( nonCompoundSpanishSpecificationCollectionNr = userSpecificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&
					( existingSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, NO_QUESTION_PARAMETER, nonCompoundSpanishSpecificationCollectionNr, userSpecificationWordItem ) ) != NULL &&
					// Justification doesn't exist in specification
					existingSpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL )
						{
						// Additional justification (Possessive - Spanish - User)
						if( ( justificationResult = generalizationWordItem->addJustification( false, true, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, userSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

						if( existingSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the assumption specification" );
						}
					}
				else
					{
					if( userSpecificationItem->isActiveAssignment() )
						isActiveUserAssignment = true;
					}
				}

			// Do for all selected specifications
			do	{
				isOlderSecondarySpecification = currentSecondarySpecificationItem->isOlderItem();
				isPossessive = currentSecondarySpecificationItem->isPossessive();
				isSelfGenerated = currentSecondarySpecificationItem->isSelfGenerated();
				generalizationWordTypeNr = currentSecondarySpecificationItem->generalizationWordTypeNr();
				specificationWordTypeNr = currentSecondarySpecificationItem->specificationWordTypeNr();
				relationWordTypeNr = currentSecondarySpecificationItem->relationWordTypeNr();
				specificationCollectionNr = currentSecondarySpecificationItem->specificationCollectionNr();
				relationContextNr = currentSecondarySpecificationItem->relationContextNr();

				if( ( specificationWordItem = currentSecondarySpecificationItem->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The secondary specification item has no specification word" );

				if( isPossessive )
					{
					if( isSelfGenerated &&
					userSpecificationItem != NULL )
						{
						if( isUserGeneralizationWord ||
						!currentSecondarySpecificationItem->hasSpecificationCollection() )
							{
							if( !isSpanishCurrentLanguage ||
							!currentSecondarySpecificationItem->isSpecificationWordSpanishAmbiguous() )
								{
								currentContextWordItem = firstContextWordItem;

								// Do for all context words
								do	{
									if( currentContextWordItem != generalizationWordItem &&
									( existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, specificationWordItem, generalizationWordItem ) ) != NULL )
										{
										// Found user specification, force self-generated assumption
										if( existingSpecificationItem->isUserSpecification() &&
										( tempSpecificationItem = currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, true, specificationWordItem, generalizationWordItem ) ) != NULL )
											existingSpecificationItem = tempSpecificationItem;

										if( relationContextNr > NO_CONTEXT_NR &&
										existingSpecificationItem->hasCurrentCreationSentenceNr() &&
										existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
										// Relation context exists in word
										currentContextWordItem->hasContextInWord( relationContextNr, specificationWordItem ) &&
										// Justification doesn't exist in specification
//										existingSpecificationItem->secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == NULL &&
										// Justification doesn't exist in specification

										( ( olderJustificationItem = existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ) ) == NULL ||
										olderJustificationItem->secondaryGeneralizationWordItem() != generalizationWordItem ) )
											{
											if( ( justificationResult = currentContextWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, currentSecondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", currentContextWordItem->anyWordTypeString(), "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL &&
											currentContextWordItem->attachJustificationInWord( createdJustificationItem, existingSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the given assumption specification" );
											}
										}
									}
								while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
								}
							}
						else
							{
							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							isActiveUserAssignment &&
							isOlderSecondarySpecification &&
							currentSecondarySpecificationItem->hasNonCompoundSpecificationCollection() &&
							// Justification doesn't exist in specification
							currentSecondarySpecificationItem->secondarySpecificationJustificationItem( userSpecificationItem ) == NULL &&

							// Draw Spanish non-compound possessive reversible conclusion
							( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, userSpecificationItem, NULL, generalizationWordItem, specificationWordItem, userGeneralizationWordItem ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				else	// Non-possessive
					{
					isExclusiveSpecification = currentSecondarySpecificationItem->isExclusiveSpecification();
					isSecondaryArchivedAssignment = currentSecondarySpecificationItem->isArchivedAssignment();
					isSecondaryUserAssignment = currentSecondarySpecificationItem->isUserAssignment();

					isExclusiveGeneralizationAssignment = ( isSecondaryUserAssignment &&
															currentSecondarySpecificationItem->isExclusiveGeneralization() );

					currentContextWordItem = firstContextWordItem;

					// Typical for Spanish
					if( isSpanishCurrentLanguage )
						{
						isSpanishSpecificationFemaleWord = specificationWordItem->isFemale();
						spanishMasculineSpecificationWordItem = ( isSpanishSpecificationFemaleWord ? specificationWordItem->masculineCollectionWordItem() : NULL );
						}

					// Do for all context words
					do	{
						if( relationContextNr > NO_CONTEXT_NR &&
						currentContextWordItem != generalizationWordItem &&

						( !isSpanishCurrentLanguage ||
						!isSecondaryArchivedAssignment ) &&

						// Relation context exists in word
						currentContextWordItem->hasContextInWord( relationContextNr, specificationWordItem ) )
							{
							existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, isSecondaryArchivedAssignment, false, true, specificationWordItem, generalizationWordItem );

							isOlderExistingSpecification = ( existingSpecificationItem != NULL &&
															existingSpecificationItem->isOlderItem() );

							if( !isSecondaryUserAssignment ||
							!hasGeneralizationWordCurrentlyConfirmedSpecification )
								{
								if( isExclusiveSpecification )
									{
									// Specification doesn't exist
									if( existingSpecificationItem == NULL &&
									// Draw possessive reversible conclusion
									( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem, NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated possessive reversible specification from current context word \"", currentContextWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									firstUserSpecificationItem = currentContextWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );

									isOlderUserSpecification = ( existingSpecificationItem == NULL &&
																firstUserSpecificationItem != NULL );

									reversedUserSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, specificationWordItem, currentContextWordItem );

									// User specification doesn't exist
									if( isOlderUserSpecification ||
									firstUserSpecificationItem == NULL ||

									// Typical for Spanish
									( isSpanishCurrentLanguage &&
									reversedUserSpecificationItem != NULL &&
									currentSecondarySpecificationItem != reversedUserSpecificationItem &&

									( ( !isOlderExistingSpecification &&
									reversedUserSpecificationItem->isOlderItem() ) ||

									( isOlderExistingSpecification &&
									!reversedUserSpecificationItem->isOlderItem() &&
									currentSecondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) )
										{
										if( existingSpecificationItem == NULL ||

										( currentSecondarySpecificationItem == reversedUserSpecificationItem &&
										// Justification doesn't exist in specification
										existingSpecificationItem->secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == NULL ) )
											{
											isAssignment = ( isExclusiveGeneralizationAssignment ||
															isSecondaryArchivedAssignment );

											isUniqueUserRelation = ( isSecondaryArchivedAssignment &&
																	isSecondaryUserAssignment &&
																	adminItem_->isUniqueUserRelation() );

											if( isSpanishCurrentLanguage )
												{
												spanishAmbiguousSpecificationCollectionNr = ( !isSpanishSpecificationFemaleWord &&
																							reversedUserSpecificationItem == NULL ? specificationCollectionNr :

																							( existingSpecificationItem != NULL &&

																							( existingSpecificationItem->wasHiddenSpanishSpecification() ||

																							( !isSpanishSpecificationFemaleWord &&
																							currentSecondarySpecificationItem->hasCompoundSpecificationCollection() &&
																							existingSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ? existingSpecificationItem->specificationCollectionNr() : NO_COLLECTION_NR ) );

												isForcingNewSpanishJustification = ( spanishAmbiguousSpecificationCollectionNr > NO_COLLECTION_NR );
												}

											// Draw possessive reversible conclusion
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isSecondaryArchivedAssignment, false, false, false, false, isForcingNewSpanishJustification, false, false, true, isUniqueUserRelation, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem->updatedSpecificationItem(), NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with generalization word \"", currentContextWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

											if( currentSecondarySpecificationItem->isReplacedItem() )
												currentSecondarySpecificationItem = currentSecondarySpecificationItem->updatedSpecificationItem();

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;

											if( !isSpanishCurrentLanguage &&
											!isOlderUserSpecification &&
											isSelfGenerated &&
											( createdOrFoundSpecificationItem = ( createdSpecificationItem == NULL ? createAndAssignResult.foundSpecificationItem : createdSpecificationItem ) ) != NULL &&
											createdOrFoundSpecificationItem->hasNonCompoundSpecificationCollection() &&

											( isUserGeneralizationWord ||
											!createdOrFoundSpecificationItem->isCircular( generalizationWordItem ) ) )
												{
												if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, createdOrFoundSpecificationItem->updatedSpecificationItem(), NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL &&
												generalizationWordItem->attachJustificationInWord( createdJustificationItem, currentSecondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the given assumption specification" );
												}

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
												{
												// Created, not found
												if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == NULL )
													{
													// Typical for Spanish
													if( isSpanishCurrentLanguage &&
													!isOlderUserSpecification &&
													( obsoleteSpecificationItem = currentContextWordItem->noRelationContextSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : specificationWordItem ) ) ) != NULL )
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
										else
											{
											if( currentSecondarySpecificationItem == reversedUserSpecificationItem )
												{
												if( isUserGeneralizationWord &&
												( olderJustificationItem = existingSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ) ) != NULL &&
												( currentJustificationItem = existingSpecificationItem->currentJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != NULL &&
												( primarySpecificationItem = currentJustificationItem->primarySpecificationItem() ) != NULL &&
												( secondarySpecificationItem = olderJustificationItem->secondarySpecificationItem() ) != NULL &&
												( foundSpecificationItem = currentJustificationItem->secondarySpecificationItem() ) != NULL &&
												!foundSpecificationItem->isOlderItem() )
													{
													// Additional justification
													if( ( justificationResult = currentContextWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, foundSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", currentContextWordItem->anyWordTypeString(), "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
														return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

													if( foundSpecificationItem->changeFirstJustification( true, createdJustificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the created specification" );
													}
												}
											else
												{
												// Typical for Spanish
												if( isSpanishCurrentLanguage &&
												reversedUserSpecificationItem != NULL )
													{
													if( isOlderSecondarySpecification )
														{
														if( currentSecondarySpecificationItem->isSpecificationWordSpanishAmbiguous() &&

														( existingSpecificationItem == firstUserSpecificationItem ||

														( firstUserSpecificationItem == NULL &&
														currentSecondarySpecificationItem->isUserSpecification() ) ) &&

														// Justification doesn't exist in specification
														reversedUserSpecificationItem->secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == NULL )
															{
															// Additional Spanish justification (self-generated conclusion). Test file: "Complejo (19 - extraño)"
															if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, ( currentSecondarySpecificationItem->hasAssumptionLevel() ? JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, currentSecondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

															if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
																return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

															if( reversedUserSpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the created specification" );
															}
														}
													else
														{
														if( !isOlderExistingSpecification &&
														spanishMasculineSpecificationWordItem != NULL &&
														( spanishMasculineSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != NULL &&
														( obsoleteJustificationItem = spanishMasculineSpecificationItem->secondarySpecificationJustificationItem( reversedUserSpecificationItem ) ) != NULL )
															{
															if( ( justificationResult = currentContextWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, currentSecondarySpecificationItem, NULL, NULL ) ).result != RESULT_OK )
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
									else
										{
										if( isOlderExistingSpecification &&
										!isOlderSecondarySpecification &&
										existingSpecificationItem != NULL &&
										userGeneralizationWordItem != NULL &&
										userSpecificationItem != NULL &&
										currentSecondarySpecificationItem == reversedUserSpecificationItem &&
										existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
										!userGeneralizationWordItem->hasCurrentlyConfirmedSpecification() &&
										userSpecificationItem->hasNonCompoundSpecificationCollection() &&
										// Justification doesn't exist in specification
										currentSecondarySpecificationItem->secondarySpecificationJustificationItem( existingSpecificationItem ) == NULL )
											{
											// Additional justification (User). Test file: "Complex (13)"
											if( ( justificationResult = generalizationWordItem->addJustification( false, true, false, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, NULL, NULL, existingSpecificationItem, NULL, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
												return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

											if( currentSecondarySpecificationItem->attachJustificationToSpecification( createdJustificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach the created justification to the current secondary specification" );
											}
										}

									// Typical for Spanish
									if( isSpanishCurrentLanguage )
										{
										if( isSpanishSpecificationFemaleWord )
											{
											if( ( existingSpecificationItem == NULL &&
											spanishMasculineSpecificationWordItem != NULL &&
											( nonCompoundSpanishSpecificationCollectionNr = spanishMasculineSpecificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&

											( ( spanishMasculineSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) == NULL ||
											spanishMasculineSpecificationItem->isSelfGenerated() ) ) &&

											// Draw Spanish possessive reversible conclusion
											( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, nonCompoundSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem, NULL, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish possessive reversible conclusion with generalization word \"", currentContextWordItem->anyWordTypeString(), "\" and masculine specification word \"", spanishMasculineSpecificationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											if( existingSpecificationItem != NULL &&
											currentContextWordItem->isUserGeneralizationWord &&
											existingSpecificationItem->hasCompoundSpecificationCollection() &&
											currentSecondarySpecificationItem->isSelfGeneratedConclusion() &&
											( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&

											// Add Spanish non-compound possessive reversible conclusion
											( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, nonCompoundSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NULL, NULL, currentSecondarySpecificationItem, NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish non-compound possessive reversible conclusion with generalization word \"", currentContextWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
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
			if( !isArchivedAssignment &&

			( primarySpecificationItem->isUserSpecification() ||
			generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation() ) &&

			( tempSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL )
				primarySpecificationItem = tempSpecificationItem;
			}

		if( primarySpecificationItem != NULL &&
		( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
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

					if( partOfSpecificationItem == NULL ||
					primarySpecificationItem->assumptionLevel() < partOfSpecificationItem->assumptionLevel() ||

					( primarySpecificationItem->assumptionLevel() == partOfSpecificationItem->assumptionLevel() &&

					( !primarySpecificationItem->hasRelationContext() ||
					primarySpecificationItem->isPossessive() ||
					// Typical for Spanish
					!partOfSpecificationItem->hasPossessivePrimarySpecificationJustification() ) ) ||

					// Typical for Spanish
					( primarySpecificationItem->hasRelationContext() &&

					( ( partOfSpecificationItem->isConcludedAssumption() &&

					( primarySpecificationItem->isPossessive() ||
					generalizationWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() ) ) ||

					( !partOfSpecificationItem->hasAssumptionLevel() &&
					primarySpecificationItem->isPossessive() &&
					primarySpecificationItem->isOlderItem() ) ) ) )
						{
						isPrimarySpecificationArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

						// Typical for Spanish
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
							( assumptionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, spanishRelationWordItem ) ) != NULL )
								{
								// Additional justification (unhidden)
								// Typical for Spanish. Test files: "Complejo (13)" and "Complejo (18)"
								if( ( justificationResult = generalizationWordItem->addJustification( false, false, false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, NO_ORDER_NR, globalVariables_->currentSentenceNr, primarySpecificationItem, NULL, oppositePossessiveDefinitionSpecificationItem, NULL, assumptionSpecificationItem->firstJustificationItem() ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

								// Copy specification
								if( ( createAndAssignResult = generalizationWordItem->copySpecificationItem( assumptionSpecificationItem->specificationCollectionNr(), createdJustificationItem, assumptionSpecificationItem ) ).result != RESULT_OK )
									return adminItem_->startError( functionNameString, moduleNameString_, "I failed to copy the assumption specification with the created justification item" );

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
		adjustedQuestionWordItem_ = NULL;
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
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem->suggestiveQuestionAssumptionSecondarySpecificationItem() ) == NULL )
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

		// Active assignments
		if( ( secondarySpecificationItem = generalizationWordItem->firstSelectedRelationSpecificationItem( true, false ) ) != NULL &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions with assignments" );

		// Archived assignments
		if( ( secondarySpecificationItem = generalizationWordItem->firstSelectedRelationSpecificationItem( true, true ) ) != NULL &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions with assignments" );

		// Specifications
		if( ( secondarySpecificationItem = generalizationWordItem->firstSelectedRelationSpecificationItem( false, false ) ) != NULL &&
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
