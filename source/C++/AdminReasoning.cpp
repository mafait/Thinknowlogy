/*	Class:			AdminReasoning
 *	Supports class:	AdminItem
 *	Purpose:		Providing autonomous reasoning
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

#include "AdminItem.h"
#include "FindSpecificationResultType.cpp"
#include "GeneralizationItem.cpp"

class AdminReasoning
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasSuggestiveQuestionAssumption_ = false;

	SpecificationItem *adjustedSpanishQuestionSecondarySpecificationItem_ = NULL;
	SpecificationItem *alternativeSpanishAdditionalDefinitionSpecificationItem_ = NULL;
	SpecificationItem *oppositePossessiveDefinitionSpecificationItem_ = NULL;
	SpecificationItem *possessiveDefinitionSpecificationItem_ = NULL;

	WordItem *feminineSpecificationWordItem_ = NULL;
	WordItem *firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;
	WordItem *secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminReasoning";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;


	// Private functions

	bool isFeminineOrMasculineProperNounEnding( bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isFeminineSpecificationWord;
		bool isMasculineSpecificationWord;

		if( generalizationWordItem != NULL &&
		specificationWordItem != NULL )
			{
			isFeminineSpecificationWord = specificationWordItem->isFeminineWord();
			isMasculineSpecificationWord = specificationWordItem->isMasculineWord();

			if( isFeminineSpecificationWord ||
			isMasculineSpecificationWord )
						// Possessive (relation)
				return ( ( isPossessive &&
						relationWordItem != NULL &&

						( ( isFeminineSpecificationWord &&
						relationWordItem->hasFeminineProperNounEnding() ) ||

						( isMasculineSpecificationWord &&
						relationWordItem->hasMasculineProperNounEnding() ) ) ) ||

						// Non-possessive (generalization)
						( !isPossessive &&

						( ( isFeminineSpecificationWord &&
						generalizationWordItem->hasFeminineProperNounEnding() ) ||

						( isMasculineSpecificationWord &&

						( generalizationWordItem->hasMasculineProperNounEnding() ||

						// Typical for Spanish
						( specificationWordItem->isNounWordSpanishAmbiguous() &&
						generalizationWordItem->hasFeminineProperNounEnding() ) ) ) ) ) );
			}

		return false;
		}

	signed char askQuestions( SpecificationItem *userSpecificationItem )
		{
		bool isAllowingSpanishPossessiveSpecification;
		bool isArchivedUserAssignment;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *additionalDefinitionSpecificationItem = NULL;
		WordItem *currentTouchedWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
		userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
		userSpecificationWordItem = userSpecificationItem->specificationWordItem();

		if( userSpecificationItem->isGeneralizationProperNoun() &&
		( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL )
			{
			additionalDefinitionSpecificationItem = getAdditionalDefinitionSpecificationItem( userSpecificationWordItem );

			if( userSpecificationItem->isExclusiveSpecification() )
				{
				// Question part doesn't exist yet
				if( !userGeneralizationWordItem->hasJustification( userSpecificationItem, NULL, NULL ) &&
				// Ask question after more specific non-exclusive user specification
				adminItem_->addSelfGeneratedSpecification( false, isArchivedUserAssignment, isArchivedUserAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem->generalizationWordTypeNr(), userSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, userGeneralizationWordItem, userSpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about the current user sentence" );
				}
			else
				{
				if( userSpecificationItem->isPossessive() )
					{
					if( !userGeneralizationWordItem->hasFeminineOrMasculineProperNounEnding() &&

					( ( isSpanishCurrentLanguage &&
					userSpecificationItem->isSpecificationWordSpanishAmbiguous() ) ||

					!userGeneralizationWordItem->hasActiveQuestionWithCompoundCollection() ) &&

					// Ask questions about generalization word with possessive primary specification
					askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( isArchivedUserAssignment, true, isSpanishCurrentLanguage, userSpecificationItem->specificationCollectionNr(), userSpecificationItem, additionalDefinitionSpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about word \"", userGeneralizationWordItem->anyWordTypeString(), "\" with possessive user specification as primary specification" );
					}
				else
					{
					if( userGeneralizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
					// Avoid idle call
					userGeneralizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( false ) != NULL &&
					// Correct suggestive assumptions by opposite suggestive question
					correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( userSpecificationItem, userGeneralizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question in word \"", userGeneralizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			// Do for all words touched during current sentence
			do	{
				// Typical for Spanish
				isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
															currentTouchedWordItem->isUserRelationWord );

				if( currentTouchedWordItem->firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) != NULL )
					{
					// Ask questions
					if( askQuestions( isAllowingSpanishPossessiveSpecification, userSpecificationCollectionNr, additionalDefinitionSpecificationItem, userSpecificationItem, currentTouchedWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );

					if( additionalDefinitionSpecificationItem != NULL &&
					// Avoid idle call
					currentTouchedWordItem->firstExclusiveSpecificationItem( false ) != NULL &&
					// Ask questions with question as primary specification
					askQuestionsWithQuestionAsPrimarySpecification( userSpecificationItem, currentTouchedWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions about proper noun word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askQuestions( bool isAllowingSpanishPossessiveSpecification, unsigned int userSpecificationCollectionNr, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasExclusiveSecondarySpecificationCompoundCollection;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasPrimarySpecificationRelationContext;
		bool hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection;
		bool hasSkippedAskingQuestion;
		bool hasUserSpecificationCompoundCollection;
		bool hasUserSpecificationRelationContext;
		bool isArchivedAssignment;
		bool isCompoundGeneralizationWordSpanishAmbiguous;
		bool isOlderPrimarySpecification;
		bool isOlderUserSpecification;
		bool isPossessivePrimarySpecification;
		bool isPossessiveUserSpecification;
		bool isPrimarySameAsUserSpecification;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		bool isPrimaryUserSpecification;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isUserGeneralizationWord;
		bool isUserQuestion;
		bool isUserSpecificationWordSpanishAmbiguous;
		unsigned short generalizationWordTypeNr;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *archivedPrimarySpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *exclusiveSpecificationItem;
		SpecificationItem *existingQuestionSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *questionPrimarySpecificationItem;
		SpecificationItem *questionSecondarySpecificationItem;
		WordItem *collectionWordItem;
		WordItem *compoundGeneralizationWordItem;
		WordItem *exclusiveSpecificationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem->firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
			isOlderUserSpecification = userSpecificationItem->isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserQuestion = userSpecificationItem->isQuestion();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();

			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			do	{
				if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL )
					{
					hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
					isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();
					isPossessivePrimarySpecification = primarySpecificationItem->isPossessive();
					isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
					isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();
					primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr();

					if( isPossessivePrimarySpecification )
						{
						if( !hasGeneralizationWordFeminineOrMasculineProperNounEnding )
							{
							archivedPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, true, false, primarySpecificationWordItem );

							// Typical for Spanish
							// Specification with user specification collection doesn't exist yet
							if( generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) == NULL &&
							// Ask questions about generalization word with possessive primary specification
							askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( isArchivedAssignment, isPossessiveUserSpecification, true, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != NULL && archivedPrimarySpecificationItem->isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), additionalDefinitionSpecificationItem, generalizationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask Spanish questions about word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
							}
						}
					else
						{
						hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();

						if( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
						hasPrimarySpecificationCompoundCollection ||
						isPrimarySpecificationWordSpanishAmbiguous ||

						( isPrimarySameAsUserSpecification &&
						!hasPrimarySpecificationRelationContext ) ) &&

						( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
						( exclusiveSecondarySpecificationItem = primarySpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
							{
							hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection = generalizationWordItem->hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();
							hasSkippedAskingQuestion = false;

							isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
							isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();

							generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();

							alternativePrimarySpecificationItem = NULL;

							if( isUserGeneralizationWord )
								{
								if( hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation &&
								!isPrimaryUserSpecification )
									alternativePrimarySpecificationItem = userSpecificationItem;
								}
							else
								{
								if( isPossessiveUserSpecification )
									{
									// Typical for Spanish
									if( isUserSpecificationWordSpanishAmbiguous &&
									// Number of relation words of user specification item
									userSpecificationItem->nContextRelations() > 1 &&
									( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
									// Skip invalid questions / similar specification doesn't exist yet
									generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, exclusiveSecondarySpecificationCollectionNr, NULL ) == NULL &&
									// Ask Spanish question about word relation word with possessive primary specification
									askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( isArchivedAssignment, generalizationWordTypeNr, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a Spanish question about word \"", generalizationWordItem->anyWordTypeString(), "\" with possessive primary specification" );
									}
								else
									{
									if( primarySpecificationItem->isSelfGeneratedAssumption() )
										alternativePrimarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, primarySpecificationWordItem, NULL );
									}
								}

							do	{
								hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem->hasCompoundSpecificationCollection();

								if( ( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL )
									{
									exclusiveSpecificationItem = NULL;
									foundSpecificationItem = NULL;

									if( ( exclusiveSpecificationItem = exclusiveSecondarySpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
										{
										// Do for all exclusive specifications of this word (including adjectives)
										do	{
											if( ( exclusiveSpecificationWordItem = exclusiveSpecificationItem->specificationWordItem() ) != NULL )
												foundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, exclusiveSpecificationWordItem );
											}
										while( foundSpecificationItem == NULL &&
										( exclusiveSpecificationItem = exclusiveSpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
										}

									if( foundSpecificationItem == NULL )
										{
										if( ( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
											{
											isCompoundGeneralizationWordSpanishAmbiguous = ( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem->compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL &&
																							compoundGeneralizationWordItem->isNounWordSpanishAmbiguous() );

											existingQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem );

											// Useful user specification without relation
											if( ( ( hasPrimarySpecificationCompoundCollection &&
											isPrimaryUserSpecification &&
											!exclusiveSecondarySpecificationItem->isSpecificationAdjective() &&

											( !isUserSpecificationWordSpanishAmbiguous ||

											// Typical for Spanish
											( isPrimarySameAsUserSpecification &&
											!hasPrimarySpecificationRelationContext &&
											!exclusiveSecondarySpecificationWordItem->isFeminineWord() ) ) ) ||

											// No useful user specification without relation
											( !hasUserSpecificationCompoundCollection &&
											!hasPrimarySpecificationRelationContext &&
											!isOlderPrimarySpecification &&

											( ( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
											hasPrimarySpecificationCompoundCollection ) ||

											( isPrimaryUserSpecification &&
											exclusiveSecondarySpecificationItem->isUserSpecification() ) ) ) ||

											( hasExclusiveSecondarySpecificationCompoundCollection &&
											hasUserSpecificationRelationContext &&

											( ( hasPrimarySpecificationCompoundCollection &&

											( hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
											!exclusiveSecondarySpecificationWordItem->isFeminineWord() ) ) ||

											( !isUserGeneralizationWord &&

											( !isPossessiveUserSpecification ||

											( !isUserSpecificationWordSpanishAmbiguous &&
											primarySpecificationWordItem->isUserSpecificationWord ) ) ) ) ) ||

											// Added additional justification
											( !hasExclusiveSecondarySpecificationCompoundCollection &&
											!hasPrimarySpecificationRelationContext &&
											hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection &&

											( ( existingQuestionSpecificationItem == NULL &&
											!hasUserSpecificationCompoundCollection ) ||

											( existingQuestionSpecificationItem != NULL &&
											!existingQuestionSpecificationItem->hasSecondarySpecificationCompoundCollectionJustification() ) ) ) ||

											( hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection &&

											// Corrected assumption by opposite suggestive question
											( hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ||
											// Avoid adding non-compound justification to compound question
											primarySpecificationCollectionNr > NO_COLLECTION_NR ) ) ) &&

											( ( isCompoundGeneralizationWordSpanishAmbiguous &&
											!isUserSpecificationWordSpanishAmbiguous ) ||

											// Typical for Spanish
											// Question part doesn't exist yet
											!generalizationWordItem->hasJustification( ( alternativePrimarySpecificationItem == NULL ? primarySpecificationItem : alternativePrimarySpecificationItem ), NULL, exclusiveSecondarySpecificationItem ) ) )
												{
												// Adjusted question
												if( alternativePrimarySpecificationItem != NULL ||

												( hasPrimarySpecificationCompoundCollection &&
												hasPrimarySpecificationRelationContext &&
												isCompoundGeneralizationWordSpanishAmbiguous &&
												!isUserSpecificationWordSpanishAmbiguous ) ||

												( isUserSpecificationWordSpanishAmbiguous &&
												compoundGeneralizationWordItem != primarySpecificationWordItem &&

												( !hasPrimarySpecificationRelationContext ||
												// Same specification but without relation context doesn't exist
												// Spanish test file: "Esta información es más específico"
												generalizationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, primarySpecificationWordItem ) == NULL ) ) ||

												// Has possibly a gap in the knowledge
												( ( !isOlderPrimarySpecification ||
												isUserQuestion ) &&

												( !hasUserSpecificationCompoundCollection ||

												// Negative specification doesn't exist yet
												( generalizationWordItem->firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == NULL &&
												generalizationWordItem->hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationCollectionNr, primarySpecificationItem ) ) ) ) )
													{
													questionPrimarySpecificationItem = ( alternativePrimarySpecificationItem == NULL ? primarySpecificationItem :
																																		alternativePrimarySpecificationItem );
													questionSecondarySpecificationItem = ( isPrimarySpecificationWordSpanishAmbiguous &&
																						isUserGeneralizationWord &&
																						adjustedSpanishQuestionSecondarySpecificationItem_ != NULL ? adjustedSpanishQuestionSecondarySpecificationItem_ :
																																					exclusiveSecondarySpecificationItem );

													if( ( collectionWordItem = exclusiveSecondarySpecificationWordItem->collectionWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL &&
													// Avoid asking invalid questions:
													// 1) Check for existing knowledge with same specification collection
													generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, exclusiveSecondarySpecificationWordItem ) == NULL &&
													// 2) Check for existing negative specification
													generalizationWordItem->firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == NULL &&
													// 3) Check for existing negative specification. Test file: "Complex (3)"
													generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, true, false, collectionWordItem, NULL ) == NULL &&

													( existingQuestionSpecificationItem == NULL ||

													( ( !isSpanishCurrentLanguage ||
													// Typical for Spanish
													// Avoid almost duplicate justifications
													!hasPrimarySpecificationRelationContext ||
													!isPossessiveUserSpecification ) &&

													// Justification doesn't exist yet in specification
													( !existingQuestionSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( questionPrimarySpecificationItem, questionSecondarySpecificationItem ) &&
													// Spanish test file: "Que haya confirmado al menos una palabra de relación"
													// Justification doesn't exist yet in word
													generalizationWordItem->primaryAndSecondarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, questionPrimarySpecificationItem, questionSecondarySpecificationItem ) == NULL ) ) ) )
														{
														// Ask a specification substitution question
														if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, questionPrimarySpecificationItem, NULL, questionSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );

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
															if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
															createdOrFoundSpecificationItem->isCorrectedSpecification() )
																{
																if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ == NULL )
																	firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = primarySpecificationWordItem;
																else
																	{
																	if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != primarySpecificationWordItem )
																		secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = primarySpecificationWordItem;
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
													hasUserSpecificationCompoundCollection &&
													!isOlderUserSpecification &&
													primarySpecificationWordItem->hasCollectionNr( userSpecificationCollectionNr ) &&
													exclusiveSecondarySpecificationWordItem->nonCompoundCollectionNr() > NO_COLLECTION_NR &&
													// Question exists
													generalizationWordItem->firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem ) != NULL &&
													// Find question to be adjusted by compound collection
													findQuestionToBeAdjustedByCompoundCollection( generalizationWordItem, primarySpecificationWordItem, userSpecificationItem, exclusiveSecondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a question to be adjusted by a compound collection about word \"", generalizationWordItem->anyWordTypeString(), "\" by compound collection" );
													}
												}
											}
										}
									else
										{
										// Test file: "Skip invalid questions (1)"
										if( generalizationWordItem != exclusiveSecondarySpecificationWordItem && 
										// Specification doesn't exist yet
										generalizationWordItem->firstSpecificationItem( false, false, false, exclusiveSecondarySpecificationWordItem ) == NULL &&
										// Make exclusive specification substitution assumption
										adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, exclusiveSecondarySpecificationItem, exclusiveSpecificationItem, foundSpecificationItem, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							while( !hasSkippedAskingQuestion &&
							( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
							}
						}
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem->nextCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( bool isArchivedAssignment, bool isPossessiveUserSpecification, bool isSpanishCurrentLanguage, unsigned int userSpecificationCollectionNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasMultipleUserGeneralizationWords = false;
		bool hasPrimarySpecificationCompoundCollection;
		bool hasPrimarySpecificationRelationContext;
		bool isOlderPrimarySpecification;
		bool isUserGeneralizationWord;
		unsigned short generalizationWordTypeNr;
		unsigned int exclusiveSecondarySpecificationCollectionNr;
		SpecificationItem *exclusiveSecondarySpecificationItem;
		SpecificationItem *existingQuestionSpecificationItem;
		WordItem *additionalDefinitionSpecificationWordItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given additional definition specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Question about generalization word
		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
		( exclusiveSecondarySpecificationItem = additionalDefinitionSpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
			{
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			hasPrimarySpecificationCompoundCollection = primarySpecificationItem->hasCompoundSpecificationCollection();
			hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();

			if( isSpanishCurrentLanguage )
				hasMultipleUserGeneralizationWords = ( globalVariables_->nUserGeneralizationWords > 1 );

			do	{
				// Current question part doesn't exist yet
				if( !generalizationWordItem->hasJustification( primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL &&

				// No answered questions
				( ( ( existingQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, false, WORD_PARAMETER_SINGULAR_VERB_IS ) ) == NULL &&
				isUserGeneralizationWord &&
				hasPrimarySpecificationRelationContext &&
				// User specification doesn't exist yet
				generalizationWordItem->firstUserSpecificationItem( false, false, userSpecificationCollectionNr, NO_CONTEXT_NR, NULL ) == NULL ) ||

				// Typical for Spanish
				( isSpanishCurrentLanguage &&

				( ( isUserGeneralizationWord ||

				( isOlderPrimarySpecification &&
				!isPossessiveUserSpecification ) ||

				( hasMultipleUserGeneralizationWords ||

				( hasPrimarySpecificationCompoundCollection &&
				!isPossessiveUserSpecification ) ) ) &&

				( exclusiveSecondarySpecificationItem->isSelfGeneratedSpecification() ||

				( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem->compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != NULL &&
				!compoundGeneralizationWordItem->isNounWordSpanishAmbiguous() ) ) &&

				// Positive specification doesn't exist yet
				generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, NULL ) == NULL &&
				// Negative specification doesn't exist yet
				generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, NULL ) == NULL &&

				( existingQuestionSpecificationItem == NULL ||
				// Justification has at least the same assumption level
				existingQuestionSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, NULL ) ) ) ) ) &&

				// Ask a specification substitution question
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askQuestionsWithQuestionAsPrimarySpecification( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool hasUserSpecificationCompoundCollection;
		bool isArchivedAssignment;
		bool isFeminineOrMasculineUserSpecificationWord;
		bool isPossessiveUserSpecification;
		bool isUserRelationWord;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askQuestionsWithQuestionAsPrimarySpecification";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem( false ) ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			isFeminineOrMasculineUserSpecificationWord = userSpecificationWordItem->isFeminineOrMasculineWord();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();

			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( ( hasUserSpecificationCompoundCollection ||
				primarySpecificationItem->hasCompoundSpecificationCollection() ) &&

				( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
				( secondarySpecificationItem = primarySpecificationWordItem->firstNonCompoundUserSpecificationItem() ) != NULL &&
				( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Question part doesn't exist yet as specification
				generalizationWordItem->firstSpecificationItem( false, false, false, secondarySpecificationWordItem ) == NULL &&

				( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
				isUserRelationWord ) ||

				// Skip adjective specification words of user generalization word
				!secondarySpecificationItem->isSpecificationAdjective() ) &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == NULL ||
				// Justification doesn't exist yet in question
				!existingSpecificationItem->hasSecondarySpecificationJustification( secondarySpecificationItem ) ) &&

				( !isFeminineOrMasculineUserSpecificationWord ||
				!isPossessiveUserSpecification ||
				// Typical for Spanish
				existingSpecificationItem != NULL ) )
					{
					isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

					// Ask a specification substitution question
					if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem->nextExclusiveSpecificationItem( false ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		SpecificationItem *exclusiveSecondarySpecificationItem;
		WordItem *exclusiveSecondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( !generalizationWordItem->hasJustification( userSpecificationItem, NULL, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Ask a specification substitution question
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, exclusiveSecondarySpecificationItem, NULL, generalizationWordItem, exclusiveSecondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", exclusiveSecondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByKnowledge( SpecificationItem *createdSpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *createdSpecificationWordItem )
		{
		SpecificationItem *obsoleteSpecificationItem;
		WordItem *userSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByKnowledge";

		if( createdSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given created specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( createdSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given created specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( obsoleteSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, createdSpecificationWordItem ) ) != NULL )
			{
			// Write invalid positive assumption by knowledge
			if( generalizationWordItem->writeUpdatedSpecification( false, true, false, false, false, false, false, obsoleteSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid positive assumption by knowledge about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			// Replace invalid positive assumption by knowledge
			if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid positive assumption by knowledge in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		if( ( obsoleteSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, userSpecificationWordItem ) ) != NULL )
			{
			// Write invalid user assumption by knowledge
			if( generalizationWordItem->writeUpdatedSpecification( false, true, false, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid user assumption by knowledge about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			// Replace invalid user assumption by knowledge
			if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, userSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid user assumption by knowledge in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned int obsoleteSpecificationCollectionNr;
		SpecificationItem *obsoleteSpecificationItem;
		WordItem *obsoleteSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByOppositeSuggestiveQuestion";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Replace all invalid assumptions by opposite suggestive question
		if( ( obsoleteSpecificationItem = generalizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( false ) ) != NULL )
			{
			do	{
				if( ( obsoleteSpecificationCollectionNr = obsoleteSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
				( obsoleteSpecificationWordItem = obsoleteSpecificationItem->specificationWordItem() ) != NULL &&
				obsoleteSpecificationWordItem->isExclusiveCollection( obsoleteSpecificationCollectionNr ) )
					{
					// Write invalid assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem->writeUpdatedSpecification( false, false, true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid assumption by opposite suggestive question (obsolete) in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					// Replace invalid assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, userSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an invalid assumption by opposite suggestive question (obsolete) in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					obsoleteSpecificationItem = generalizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( false );
					}
				else
					obsoleteSpecificationItem = obsoleteSpecificationItem->nextOlderNonPossessiveNonQuestionSpecificationItem();
				}
			while( obsoleteSpecificationItem != NULL );
			}

		return RESULT_OK;
		}

	signed char correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *createdNewSpecificationItem;
		SpecificationItem *createdOldSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem *oldSuggestiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "correctSuggestiveAssumptionsByOppositeSuggestiveQuestion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem->isActiveAssignment() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is no assignment" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Previously, a suggestive question assumption was made.
		// However, now a question with the opposite specification is asked.
		// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem->suggestiveQuestionAssumptionSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem->isActiveAssignment() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, oldSuggestiveQuestionAssumptionAssignmentItem, NULL, NULL, NULL, generalizationWordItem, oldSuggestiveSpecificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oldSuggestiveSpecificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		// Replace created old specification by created new specification in justifications of involved word
		if( generalizationWordItem->updateSpecificationOfJustificationsInWord( createdOldSpecificationItem, createdNewSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update a specification in justifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != NULL )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem->firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add first old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOldSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the first old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );

			if( ( definitionSpecificationItem = specificationWordItem->firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add first new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdNewSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the first new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );
			}

		if( secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != NULL )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem->firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add second old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOldSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the second old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );

			if( ( definitionSpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != NULL &&
			// Add second new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdNewSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the second new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawNegativeAdjectiveConclusions( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *primarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeAdjectiveConclusions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem->isSpecificationAdjective() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no adjective specification" );

		if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = primarySpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( secondarySpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, primarySpecificationWordItem ) ) != NULL &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == NULL ||

				// User specification doesn't exist yet
				( existingSpecificationItem->isSelfGeneratedSpecification() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) ) ) &&

				// Draw negative adjective conclusion
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawNegativeConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool isPrimarySameAsSecondarySpecification;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		unsigned int additionalDefinitionSpecificationCollectionNr;
		SpecificationItem *additionalProperNounSpecificationItem = NULL;
		SpecificationItem *negativeConclusionPrimarySpecificationItem = NULL;
		SpecificationItem *negativeConclusionAdditionalDefinitionSpecificationItem = NULL;
		SpecificationItem *negativeConclusionSecondarySpecificationItem = NULL;
		SpecificationItem *negativeDefinitionSpecificationItem = NULL;
		WordItem *additionalDefinitionSpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given additional definition specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given additional definition specification item has no specification word item" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();
		isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

		additionalDefinitionSpecificationCollectionNr = additionalDefinitionSpecificationItem->specificationCollectionNr();

		if( ( negativeDefinitionSpecificationItem = secondarySpecificationWordItem->firstSpecificationItem( true, false, false, additionalDefinitionSpecificationWordItem ) ) != NULL &&
		!isPrimarySpecificationWordSpanishAmbiguous )
			{
			// Test file: "Simple family definition - John and Anna - Paul, Joe and Laura"
			negativeConclusionPrimarySpecificationItem = secondarySpecificationItem;
			negativeConclusionSecondarySpecificationItem = negativeDefinitionSpecificationItem;
			}
		else
			{
			if( isPrimarySpecificationWordSpanishAmbiguous &&
			negativeDefinitionSpecificationItem != NULL )
				{
				// Typical for Spanish
				// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
				negativeConclusionPrimarySpecificationItem = secondarySpecificationItem;
				negativeConclusionSecondarySpecificationItem = negativeDefinitionSpecificationItem;
				}
			else
				{
				negativeConclusionPrimarySpecificationItem = primarySpecificationItem;
				negativeConclusionAdditionalDefinitionSpecificationItem = additionalDefinitionSpecificationItem;
				negativeConclusionSecondarySpecificationItem = ( isPrimarySameAsSecondarySpecification ? additionalProperNounSpecificationItem :
																										secondarySpecificationItem );

				if( negativeConclusionSecondarySpecificationItem == NULL )
					// Typical for Spanish
					negativeConclusionSecondarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, additionalDefinitionSpecificationCollectionNr, NULL );
				}
			}

		if( negativeConclusionSecondarySpecificationItem == NULL )
			{
			// Typical for Spanish
			// Spanish test file: "Reto científico"
			if( generalizationWordItem->firstSpecificationItem( false, false, true, additionalDefinitionSpecificationWordItem ) == NULL &&
			// Ask a specification substitution question
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, NULL, negativeConclusionAdditionalDefinitionSpecificationItem, NULL, generalizationWordItem, additionalDefinitionSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", additionalDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
			}
		else
			{
			// Draw negative conclusion
			if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAdditionalDefinitionSpecificationItem, negativeConclusionSecondarySpecificationItem, NULL, generalizationWordItem, additionalDefinitionSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", additionalDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawNegativeDefinitionConclusions( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short negativeSpecificationWordTypeNr, WordItem *generalizationWordItem, WordItem *negativeSpecificationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *definitionSpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *positiveAdjectiveDefinitionSpecificationItem;
		SpecificationItem *positiveSpecificationItem;
		SpecificationItem *negativeAdjectiveDefinitionSpecificationItem;
		SpecificationItem *secondAdjectiveDefinitionSpecificationItem;
		WordItem *oppositeSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeDefinitionConclusions";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( negativeSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given negative specification word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( positiveAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL )
			{
			// Try to find negative first
			if( ( secondAdjectiveDefinitionSpecificationItem = generalizationWordItem->firstAdjectiveSpecificationItem( true ) ) != NULL ||
			( secondAdjectiveDefinitionSpecificationItem = generalizationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL )
				{
				// Draw simple negative adjective conclusion
				if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, NULL, secondAdjectiveDefinitionSpecificationItem, NULL, negativeSpecificationWordItem, generalizationWordItem, NULL ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative adjective conclusion about generalization word \"", negativeSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				definitionSpecificationItem = createAndAssignResult.createdSpecificationItem;
				}

			if( ( positiveAdjectiveDefinitionSpecificationItem = generalizationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL &&
			( negativeAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem->firstAdjectiveSpecificationItem( true ) ) != NULL &&

			// Specification doesn't exist yet
			( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == NULL ||

			// Justification doesn't exist yet in specification
			( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveAdjectiveDefinitionSpecificationItem, negativeAdjectiveDefinitionSpecificationItem ) &&
			// Justification has at least the same assumption level
			existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveAdjectiveDefinitionSpecificationItem, NULL, negativeAdjectiveDefinitionSpecificationItem, NULL ) ) ) &&

			// Draw another simple negative adjective conclusion
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, NULL, negativeAdjectiveDefinitionSpecificationItem, NULL, generalizationWordItem, negativeSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw another simple negative adjective conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );

			if( definitionSpecificationItem != NULL &&
			( oppositeSpecificationWordItem = generalizationWordItem->collectionWordItem( definitionSpecificationItem->specificationCollectionNr() ) ) != NULL &&
			( positiveSpecificationItem = oppositeSpecificationWordItem->firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != NULL )
				{
				// Specification doesn't exist yet
				if( ( ( existingSpecificationItem = oppositeSpecificationWordItem->firstSpecificationItem( true, false, false, generalizationWordItem ) ) == NULL ||

				// Justification doesn't exist yet in specification
				( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL ) ) ) &&

				// Draw opposite simple negative conclusion
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL, oppositeSpecificationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an opposite simple negative conclusion about generalization word \"", oppositeSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				// Test file: "James was the father of Peter (before family definition)"
				if( existingSpecificationItem != NULL &&
				// Recalculate assumption level of existing specification
				existingSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the existing specification" );
				}
			}

		return RESULT_OK;
		}

	signed char drawNegativeExclusiveConclusions( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *negativePrimarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *exclusiveSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeExclusiveConclusions";

		if( negativePrimarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given negative primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( exclusiveSpecificationItem = specificationWordItem->firstExclusiveSpecificationItem( true ) ) != NULL )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( ( secondarySpecificationWordItem = exclusiveSpecificationItem->specificationWordItem() ) != NULL &&
				// Draw exclusive negative compound conclusion
				adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, exclusiveSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, negativePrimarySpecificationItem, NULL, exclusiveSpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an exclusive negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
				}
			while( ( exclusiveSpecificationItem = exclusiveSpecificationItem->nextExclusiveSpecificationItem( true ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawOnlyOptionLeftProperNounConclusion( SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCreatedSpecification = false;
		bool isArchivedAssignment;
		bool hasFoundMoreMatches = false;
		unsigned int currentSpecificationCollectionNr;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *foundDefinitionSpecificationItem = NULL;
		SpecificationItem *negativeSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *foundDefinitionSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawOnlyOptionLeftProperNounConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			do	{
				if( currentSpecificationItem->isExclusiveSpecification() &&
				( currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
				( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				( negativeSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != NULL &&
				( definitionSpecificationItem = specificationWordItem->firstExclusiveSpecificationItem( false ) ) != NULL )
					{
					do	{
						if( definitionSpecificationItem->specificationWordItem() != currentSpecificationWordItem &&
						// Positive specification doesn't exist yet
						generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NULL ) == NULL &&
						definitionSpecificationItem->specificationCollectionNr() == currentSpecificationCollectionNr )
							{
							if( foundDefinitionSpecificationItem == NULL )
								foundDefinitionSpecificationItem = definitionSpecificationItem;
							else
								hasFoundMoreMatches = true;
							}
						}
					while( !hasFoundMoreMatches &&
					( definitionSpecificationItem = definitionSpecificationItem->nextExclusiveSpecificationItem( false ) ) != NULL );

					if( !hasFoundMoreMatches &&
					foundDefinitionSpecificationItem != NULL &&
					( foundDefinitionSpecificationWordItem = foundDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						isArchivedAssignment = primarySpecificationItem->isArchivedAssignment();

						// Draw an only option left conclusion
						if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, primarySpecificationItem->generalizationWordTypeNr(), primarySpecificationItem->specificationWordTypeNr(), primarySpecificationItem->relationWordTypeNr(), NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, foundDefinitionSpecificationItem, negativeSpecificationItem, NULL, generalizationWordItem, foundDefinitionSpecificationWordItem, relationWordItem ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						hasCreatedSpecification = true;
						}
					}
				}
			while( !hasCreatedSpecification &&
			( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusions( bool isSpanishCurrentLanguage, SpecificationItem *primarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasPrimarySpecificationRelationContext;
		bool hasUserSpecificationCompoundCollection;
		bool isOlderPrimarySpecification;
		bool isOlderUserSpecification;
		bool isPossessivePrimarySpecification;
		bool isPossessiveUserSpecification;
		bool isPrimarySpecificationArchivedAssignment;
		bool isPrimaryUserSpecification;
		bool isUserGeneralizationWord;
		bool isUserRelationWord;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *partOfDefinitionSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *spanishMasculineSpecificationWordItem = NULL;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawProperNounPartOfConclusions";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The specification word of the given user specification item is undefined" );

		if( ( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
			{
			hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			isPossessivePrimarySpecification = primarySpecificationItem->isPossessive();
			isPrimarySpecificationArchivedAssignment = primarySpecificationItem->isArchivedAssignment();
			isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();

			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			isOlderUserSpecification = userSpecificationItem->isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();

			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			if( hasPrimarySpecificationRelationContext &&
			isOlderPrimarySpecification &&
			!isPossessiveUserSpecification &&
			!isPrimaryUserSpecification &&
			primarySpecificationItem->hasAssumptionLevel() &&
			primarySpecificationItem->hasCurrentCreationSentenceNr() &&
			// Recalculate assumption level of primary specification
			primarySpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the primary specification" );

			// Typical for Spanish
			if( isSpanishCurrentLanguage &&
			specificationWordItem->isFeminineWord() &&
			!adminItem_->isUserQuestion() )
				spanishMasculineSpecificationWordItem = specificationWordItem->masculineCollectionWordItem();

			// Do for all possessive noun words
			do	{
				if( ( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem->partOfSpecificationItem( specificationWordItem ) ) == NULL &&
				spanishMasculineSpecificationWordItem != NULL )
					partOfDefinitionSpecificationItem = currentPossessiveNounWordItem->partOfSpecificationItem( spanishMasculineSpecificationWordItem );

				if( partOfDefinitionSpecificationItem != NULL &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, currentPossessiveNounWordItem ) ) == NULL ||

				( ( !isOlderPrimarySpecification ||
				// Typical for Spanish
				!isUserRelationWord ||
				!existingSpecificationItem->isSelfGeneratedAssumption() ||
				!primarySpecificationItem->isSelfGeneratedConclusion() ) &&

				// Justification doesn't exist yet in word
				!generalizationWordItem->hasJustification( primarySpecificationItem, NULL, partOfDefinitionSpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL ) ) ) )
					{
					createdOrFoundSpecificationItem = existingSpecificationItem;

					if( ( ( !isOlderPrimarySpecification &&

					( !isSpanishCurrentLanguage ||
					// Typical for Spanish
					!isUserRelationWord ||
					existingSpecificationItem == NULL ||
					isPrimaryUserSpecification ) ) ||

					( !isOlderUserSpecification &&

					( !isPossessiveUserSpecification ||
					!isUserRelationWord ||

					// Typical for Spanish
					// Spanish test file: "Complejo (2)"
					( hasUserSpecificationCompoundCollection &&
					primarySpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == NULL ) ) ) ) &&

					( !isUserGeneralizationWord ||
					userSpecificationItem->hasRelationContext() ||

					( hasPrimarySpecificationRelationContext &&
					existingSpecificationItem != NULL &&
					!existingSpecificationItem->hasCurrentCreationSentenceNr() ) ||

					// Not Spanish
					( !isSpanishCurrentLanguage &&

					( hasPrimarySpecificationRelationContext ||
					existingSpecificationItem == NULL ||
					existingSpecificationItem->hasCurrentCreationSentenceNr() ) ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&
					!hasPrimarySpecificationRelationContext ) ) )
						{
						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the included challenge document,
						// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
						}

					if( createdOrFoundSpecificationItem != NULL &&
					!isPossessivePrimarySpecification &&

					( ( isPossessiveUserSpecification &&
					userSpecificationWordItem == specificationWordItem ) ||

					( !isPossessiveUserSpecification &&
					!isSpanishCurrentLanguage ) ) )
						{
						selectedPrimarySpecificationItem = ( isPossessiveUserSpecification ? userSpecificationItem :
																							userGeneralizationWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) );

						if( selectedPrimarySpecificationItem != NULL &&
						// Justification doesn't exist yet in specification
						!createdOrFoundSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
						// Justification has at least the same assumption level
						createdOrFoundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL ) &&
						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the included challenge document,
						// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
						adminItem_->addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawPossessiveReversibleConclusions( bool isActiveUserAssignment, SpecificationItem *givenSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasNonCompoundSpecificationCollection;
		bool hasNonCompoundUserSpecificationCollection;
		bool hasUserSpecificationAssumptionLevel;
		bool hasUserSpecificationRelationContext;
		bool isArchivedAssignment;
		bool isAssignment;
		bool isExclusiveGeneralizationAssignment;
		bool isExclusiveSpecification;
		bool isExistingUserSpecification;
		bool isOlderAlternativeSpecification;
		bool isOlderExistingSpecification;
		bool isOlderSpecification;
		bool isOlderUserSpecification;
		bool isPossessiveUserSpecification;
		bool isSelfGeneratedAssumption;
		bool isSelfGeneratedSpecification;
		bool isSpanishCurrentLanguage;
		bool isSpanishSpecificationFemaleWord = false;
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUniqueUserRelation;
		bool isUserAssignment;
		bool isUserAssignmentSpecification;
		bool isUserGeneralizationWord;
		bool isUserSpecification;
		unsigned short relationWordTypeNr;
		unsigned short specificationWordTypeNr;
		unsigned int createdRelationContextNr;
		unsigned int relationContextNr;
		unsigned int spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int specificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *foundJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *additionalSpecificationItem;
		SpecificationItem *additionalUserSpecificationItem;
		SpecificationItem *alternativeSpecificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *collectedSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentExistingSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *firstUserSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *oppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *partOfDefinitionSpecificationItem;
		SpecificationItem *partOfSpecificationItem;
		SpecificationItem *possessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *spanishMasculineSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *additionalGeneralizationWordItem;
		WordItem *commonWordItem;
		WordItem *currentContextWordItem;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem = NULL;
		WordItem *currentRelationContextWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveNounWordItem;
		WordItem *spanishMasculineSpecificationWordItem = NULL;
		WordItem *specificationWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawPossessiveReversibleConclusions";

		if( givenSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( relationContextNr = givenSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
		( currentContextWordItem = adminItem_->firstContextWordItem( relationContextNr ) ) != NULL &&
		( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
		( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			hasNonCompoundUserSpecificationCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			hasUserSpecificationAssumptionLevel = userSpecificationItem->hasAssumptionLevel();
			hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			isUserAssignmentSpecification = userSpecificationItem->isAssignment();

			hasNonCompoundSpecificationCollection = givenSpecificationItem->hasNonCompoundSpecificationCollection();
			isArchivedAssignment = givenSpecificationItem->isArchivedAssignment();
			isExclusiveSpecification = givenSpecificationItem->isExclusiveSpecification();
			isOlderSpecification = givenSpecificationItem->isOlderItem();
			isSelfGeneratedAssumption = givenSpecificationItem->isSelfGeneratedAssumption();
			isSelfGeneratedSpecification = givenSpecificationItem->isSelfGeneratedSpecification();
			isUserAssignment = givenSpecificationItem->isUserAssignment();
			isUserSpecification = givenSpecificationItem->isUserSpecification();
			relationWordTypeNr = givenSpecificationItem->relationWordTypeNr();
			specificationWordTypeNr = givenSpecificationItem->specificationWordTypeNr();
			specificationCollectionNr = givenSpecificationItem->specificationCollectionNr();

			isExclusiveGeneralizationAssignment = ( isUserAssignment &&
													givenSpecificationItem->isExclusiveGeneralization() );

			if( ( specificationWordItem = givenSpecificationItem->specificationWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The secondary specification item has no specification word" );

			// Compound generalization word
			if( ( possessiveNounWordItem = specificationWordItem->compoundGeneralizationWordItem( specificationCollectionNr ) ) != NULL )
				{
				// Find possessive specifications
				if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"", possessiveNounWordItem->anyWordTypeString(), "\"" );

				if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != NULL )
					oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem->specificationWordItem();

				if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != NULL )
					possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem();
				}

			// Typical for Spanish
			if( isSpanishCurrentLanguage )
				{
				isSpecificationWordSpanishAmbiguous = givenSpecificationItem->isSpecificationWordSpanishAmbiguous();
				isSpanishSpecificationFemaleWord = specificationWordItem->isFeminineWord();

				if( isSpanishSpecificationFemaleWord )
					spanishMasculineSpecificationWordItem = specificationWordItem->masculineCollectionWordItem();
				}

			// Do for all context words with secondary relation context
			do	{
				isOlderExistingSpecification = false;
				isExistingUserSpecification = false;

				if( ( existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, true, specificationWordItem, generalizationWordItem ) ) != NULL )
					{
					isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
					isExistingUserSpecification = existingSpecificationItem->isUserSpecification();
					}

				if( isExclusiveSpecification )
					{
					// Specification doesn't exist yet
					if( existingSpecificationItem == NULL &&
					// Draw possessive reversible conclusion
					adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with current context word \"", currentContextWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					firstUserSpecificationItem = currentContextWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );

					isOlderUserSpecification = ( existingSpecificationItem == NULL &&
												firstUserSpecificationItem != NULL );

					alternativeSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, specificationWordItem, currentContextWordItem );

					isOlderAlternativeSpecification = ( alternativeSpecificationItem != NULL &&
														alternativeSpecificationItem->isOlderItem() );

					if( isOlderUserSpecification ||
					firstUserSpecificationItem == NULL ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&
					givenSpecificationItem != alternativeSpecificationItem &&

					( !isOlderExistingSpecification ||
					!isOlderAlternativeSpecification ) ) )
						{
						currentExistingSpecificationItem = currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, true, true, NO_QUESTION_PARAMETER, specificationWordItem->nonCompoundCollectionNr(), specificationWordItem );

						if( existingSpecificationItem == NULL ||

						( givenSpecificationItem == alternativeSpecificationItem &&
						// Justification doesn't exist yet in specification
						!existingSpecificationItem->hasPrimarySpecificationJustification( givenSpecificationItem ) &&

						( !isSpanishCurrentLanguage ||
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL, NULL ) ) &&

						( currentExistingSpecificationItem == NULL ||
						// Justification has at least the same assumption level
						currentExistingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL, NULL ) ||

						( isOlderExistingSpecification &&
						existingSpecificationItem->hasRelationContextCurrentlyBeenUpdated() ) ) ) )
							{
							if( !isSpecificationWordSpanishAmbiguous ||

							// Typical for Spanish
							( alternativeSpecificationItem == NULL &&
							firstUserSpecificationItem != NULL ) ||

							// Typical for Spanish
							( ( !isUserAssignment &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							isUserSpecification ||
							currentExistingSpecificationItem != NULL ) ) ||

							( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
							isUserSpecification &&

							( currentExistingSpecificationItem == NULL ||
							!currentExistingSpecificationItem->hasCurrentCreationSentenceNr() ) ) ) )
								{
								isAssignment = ( isArchivedAssignment ||
												isExclusiveGeneralizationAssignment );

								isUniqueUserRelation = ( isArchivedAssignment &&
														isUserAssignment &&
														adminItem_->isUniqueUserRelation() );

								if( isSpanishCurrentLanguage )
									spanishAmbiguousSpecificationCollectionNr = ( !isSpanishSpecificationFemaleWord &&
																				alternativeSpecificationItem == NULL &&
																				firstUserSpecificationItem == NULL ? specificationCollectionNr :
																				existingSpecificationItem != NULL ? existingSpecificationItem->specificationCollectionNr() :
																													NO_COLLECTION_NR );

								// Draw reversible conclusion
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, true, isUniqueUserRelation, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion about word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
								createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;

								givenSpecificationItem = givenSpecificationItem->updatedSpecificationItem();

								if( !isOlderUserSpecification &&
								!isSpanishCurrentLanguage &&
								isSelfGeneratedSpecification &&

								( hasNonCompoundUserSpecificationCollection ||
								isActiveUserAssignment ||
								isOlderSpecification ||
								!isUserGeneralizationWord ||
								foundSpecificationItem == NULL ||
								!foundSpecificationItem->isOlderItem() ||
								givenSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != NULL ) &&

								createdOrFoundSpecificationItem != NULL &&
								createdOrFoundSpecificationItem->hasNonCompoundSpecificationCollection() &&
								// Justification doesn't exist yet in specification
								!givenSpecificationItem->hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
								// Justification has at least the same assumption level
								givenSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL, NULL ) )
									{
									// Draw reversible conclusion
									if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, currentContextWordItem ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

									if( isActiveUserAssignment &&
									isUserGeneralizationWord &&
									!isOlderSpecification &&
									createdSpecificationItem == NULL &&
									!createdOrFoundSpecificationItem->isOlderItem() &&
									( additionalUserSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
										{
										// Test file: "John - Anna (before family definition)"
										// Justification doesn't exist yet in specification
										if( !givenSpecificationItem->hasPrimarySpecificationJustification( additionalUserSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, NULL, NULL, NULL ) &&
										// Add user reversible justification to current secondary specification
										givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, NULL, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a user reversible justification to the current secondary specification" );

										// Test file: "John - Anna (before family definition)"
										if( ( additionalGeneralizationWordItem = adminItem_->firstContextWordItem( additionalUserSpecificationItem->relationContextNr() ) ) != NULL &&
										( additionalSpecificationItem = additionalGeneralizationWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) ) != NULL &&
										// Justification doesn't exist yet in specification
										!givenSpecificationItem->hasPrimarySpecificationJustification( additionalSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, NULL, NULL, NULL ) &&
										// Add assumption reversible justification to current secondary specification
										givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, NULL, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an assumption reversible justification to the current secondary specification" );
										}
									}

								if( createdSpecificationItem != NULL )
									{
									// Created, not found
									if( foundSpecificationItem == NULL )
										{
										if( isUserGeneralizationWord )
											{
											if( specificationWordItem == userSpecificationWordItem )
												{
												if( possessiveDefinitionSpecificationWordItem != NULL &&
												( assumptionSpecificationItem = currentContextWordItem->firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
												( secondarySpecificationItem = specificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
												// Justification doesn't exist yet in specification
												!assumptionSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( createdSpecificationItem, secondarySpecificationItem ) &&
												// Justification has at least the same assumption level
												assumptionSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
												// Add specification substitution justification to additional specification
												assumptionSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );
												}
											else
												{
												if( ( primarySpecificationItem = currentContextWordItem->firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != NULL &&
												// Skip archived user assignment
												primarySpecificationItem->hasRelationContext() &&
												( secondarySpecificationItem = userSpecificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
												// Justification doesn't exist yet in specification
												!createdSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
												// Justification has at least the same assumption level
												createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
												// Add specification substitution justification to created specification
												createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the created specification" );
												}
											}

										if( isSpanishCurrentLanguage )
											{
											// Typical for Spanish
											if( ( obsoleteSpecificationItem = currentContextWordItem->noRelationContextSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : specificationWordItem ) ) ) == NULL )
												{
												if( isUserAssignmentSpecification )
													{
													// Spanish test file: "Complejo (19 - extraño)"
													if( ( secondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, userSpecificationWordItem, currentContextWordItem ) ) != NULL &&
													( commonWordItem = userSpecificationWordItem->commonWordItem( createdSpecificationItem->specificationCollectionNr() ) ) != NULL &&
													( primarySpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != NULL &&
													// Justification doesn't exist yet in specification
													!createdSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
													// Justification has at least the same assumption level
													createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
													// Add opposite possessive conclusion justification to created specification
													createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the created specification" );
													}
												else
													{
													// Justification doesn't exist yet in specification
													if( ( secondarySpecificationItem = userSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
													!createdSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, secondarySpecificationItem ) &&
													// Justification has at least the same assumption level
													createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
													// Add exclusive user specification substitution assumption justification to created specification
													createdSpecificationItem->addJustificationToSpecification( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an exclusive user specification substitution assumption justification to the created specification" );
													}
												}
											else
												{
												// Write an assumption without relation word
												if( isUserSpecification &&
												currentContextWordItem->writeUpdatedSpecification( true, false, false, false, true, false, false, obsoleteSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an assumption without relation word of word \"", currentContextWordItem->anyWordTypeString(), "\"" );

												// Replace an assumption without relation word
												if( currentContextWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
												}
											}
										}
									else	// Created and found
										{
										if( !isOlderUserSpecification )
											{
											if( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
											!isOlderSpecification &&
											( foundJustificationItem = givenSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) != NULL &&
											( primarySpecificationItem = foundJustificationItem->primarySpecificationItem() ) != NULL &&
											( secondarySpecificationItem = foundJustificationItem->secondarySpecificationItem() ) != NULL &&
											// Justification doesn't exist yet in specification
											!createdSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											createdSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, secondarySpecificationItem, NULL ) )
												{
												// Add opposite possessive conclusion justification to created specification
												if( createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the created specification" );

												// "Test file: "Complex (11)
												if( ( createdRelationContextNr = createdSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
												( currentRelationContextWordItem = adminItem_->firstContextWordItem( createdRelationContextNr ) ) != NULL )
													{
													// Do for all context words with the created relation context
													do	{
														// Remove obsolete assumption justifications from 'part of' specifications in relation context word
														if( currentRelationContextWordItem->removeObsoleteAssumptionJustificationsFromPartSpecificationsInWord() != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove obsolete assumption justifications from 'part of' specifications in relation context word \"", currentRelationContextWordItem->anyWordTypeString(), "\"" );
														}
													while( ( currentRelationContextWordItem = currentRelationContextWordItem->_nextContextWordItem( createdRelationContextNr ) ) != NULL );
													}
												}

											if( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
											isSelfGeneratedSpecification ) &&

											( commonWordItem = specificationWordItem->commonWordItem( specificationCollectionNr ) ) != NULL &&
											( partOfSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, commonWordItem ) ) != NULL &&
											partOfSpecificationItem->isSelfGeneratedSpecification() &&
											// Justification doesn't exist yet in part-of specification
											!partOfSpecificationItem->hasPrimarySpecificationJustification( createdSpecificationItem ) &&
											( partOfDefinitionSpecificationItem = commonWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
											// Justification has at least the same assumption level
											partOfSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, partOfDefinitionSpecificationItem, NULL ) &&
											// Add part-of justification to part-of specification
											partOfSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, partOfDefinitionSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a part-of justification to a part-of specification" );
											}
										}

									if( isOlderUserSpecification &&
									!isExclusiveGeneralizationAssignment )
										{
										// Typical for Spanish
										// Spanish test file: "Complejo (19 - extraño)"
										if( isSpecificationWordSpanishAmbiguous )
											foundSpecificationItem = currentContextWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );

										if( foundSpecificationItem != NULL )
											{
											// Test file: "Complex (19 - strange)"
											// Add reversible justification
											if( createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, " failed to add a reversible justification to the created specification" );

											// Update specification of justifications of involved words, except for current context word
											if( currentContextWordItem->updateSpecificationOfJustificationsOfInvolvedWords( false, foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"", currentContextWordItem->anyWordTypeString(), "\"" );

											if( possessiveNounWordItem == NULL &&
											// Common word
											( possessiveNounWordItem = specificationWordItem->commonWordItem( specificationCollectionNr ) ) != NULL )
												{
												// Find possessive specifications
												if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in word \"", possessiveNounWordItem->anyWordTypeString(), "\"" );

												if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != NULL )
													oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem->specificationWordItem();

												if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != NULL )
													possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem();
												}

											if( possessiveDefinitionSpecificationItem != NULL &&
											oppositePossessiveDefinitionSpecificationWordItem != NULL &&
											// Make an opposite possessive specification assumption
											adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationItem, NULL, createdSpecificationItem, NULL, currentContextWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"", currentContextWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
											}
										}
									}
								else
									{
									if( isOlderSpecification &&
									isUserAssignmentSpecification &&
									existingSpecificationItem == NULL &&
									foundSpecificationItem != NULL &&
									// Common word
									( possessiveNounWordItem = specificationWordItem->commonWordItem( specificationCollectionNr ) ) != NULL )
										{
										// Find possessive specifications
										if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"", possessiveNounWordItem->anyWordTypeString(), "\"" );

										if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != NULL &&
										( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
										oppositePossessiveDefinitionSpecificationWordItem != specificationWordItem &&
										( secondarySpecificationItem = currentContextWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL )
											{
											// Recalculate assumption level of found specification
											if( foundSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );

											// Justification doesn't exist yet in specification
											if( !foundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( oppositePossessiveDefinitionSpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											foundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, NULL, secondarySpecificationItem, NULL ) &&
											// Add opposite possessive justification to found specification
											foundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive justification to the found specification" );
											}
										}
									}
								}
							}
						else
							{
							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							alternativeSpecificationItem != NULL &&
							givenSpecificationItem != alternativeSpecificationItem )
								{
								if( isOlderSpecification )
									{
									if( isSpecificationWordSpanishAmbiguous &&
									!hasNonCompoundSpecificationCollection &&
									// Justification doesn't exist yet in specification
									!alternativeSpecificationItem->hasPrimarySpecificationJustification( givenSpecificationItem ) &&
									// Add Spanish reversible justification to alternative specification
									alternativeSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish reversible justification to the alternative specification" );
									}
								else
									{
									if( !isOlderExistingSpecification &&
									spanishMasculineSpecificationWordItem != NULL &&
									( spanishMasculineSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != NULL &&
									// Justification exists in specification
									( obsoleteJustificationItem = spanishMasculineSpecificationItem->primarySpecificationJustificationItem( alternativeSpecificationItem ) ) != NULL )
										{
										// Create justification
										if( ( justificationResult = currentContextWordItem->createJustificationItem( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, givenSpecificationItem, NULL, NULL, NULL, NULL ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a justification item to word \"", currentContextWordItem->anyWordTypeString(), "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
											return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

										if( currentContextWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption justification by a conclusion justification in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						}

					if( hasNonCompoundSpecificationCollection &&
					isSelfGeneratedSpecification &&
					!isSpanishCurrentLanguage &&
					alternativeSpecificationItem != NULL &&
					existingSpecificationItem == userSpecificationItem &&
					// Make an opposite possessive specification assumption
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, false, true, WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, relationWordTypeNr, ( isSelfGeneratedAssumption ? alternativeSpecificationItem : givenSpecificationItem ), NULL, userSpecificationItem, currentContextWordItem, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption during the drawing of a reversible conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					// Alternative specification is same as current secondary specification
					if( givenSpecificationItem == alternativeSpecificationItem )
						{
						if( isOlderExistingSpecification &&
						isSelfGeneratedSpecification &&
						existingSpecificationItem != NULL &&
						existingSpecificationItem->hasNonCompoundSpecificationCollection() )
							{
							if( isOlderSpecification &&
							firstUserSpecificationItem == NULL &&
							// Secondary specification word is same as user specification word
							specificationWordItem == userSpecificationWordItem &&
							// Justification doesn't exist yet in specification
							!givenSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
							( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, specificationWordItem, userGeneralizationWordItem ) ) != NULL )
								{
								// Add reversible user justification
								if( givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

								givenSpecificationItem = givenSpecificationItem->updatedSpecificationItem();

								// Add context
								if( userGeneralizationWordItem->addContext( WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, givenSpecificationItem->relationContextNr(), NO_COLLECTION_NR, specificationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", userGeneralizationWordItem->anyWordTypeString(), "\"" );

								if( ( collectedSpecificationItem = userGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, givenSpecificationItem->specificationCollectionNr(), NULL ) ) != NULL &&
								// Common word
								( possessiveNounWordItem = specificationWordItem->commonWordItem( specificationCollectionNr ) ) != NULL )
									{
									// Find possessive specifications
									if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"", possessiveNounWordItem->anyWordTypeString(), "\"" );

									if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != NULL &&
									!collectedSpecificationItem->isOlderItem() &&
									// Justification doesn't exist yet in specification
									!givenSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( oppositePossessiveDefinitionSpecificationItem, collectedSpecificationItem ) &&
									// Add collected opposite possessive justification
									givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, NULL, collectedSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a collected opposite possessive justification to the current secondary specification" );

									if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != NULL &&
									// Justification doesn't exist yet in specification
									!collectedSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, conclusionSpecificationItem ) &&
									// Add opposite possessive conclusion justification to collected specification
									collectedSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, NULL, conclusionSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
									}
								}
							}
						}
					else	// Alternative is different from current secondary
						{
						if( hasNonCompoundSpecificationCollection )
							{
							if( alternativeSpecificationItem != NULL &&
							// Secondary specification word is same as user specification word
							specificationWordItem == userSpecificationWordItem &&
							!alternativeSpecificationItem->isReplacedItem() )
								{
								if( isSpanishCurrentLanguage )
									{
									// Typical for Spanish
									// Replace assumption by conclusion
									if( generalizationWordItem->replaceOrDeleteSpecification( givenSpecificationItem, alternativeSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a Spanish assumption specification by a conclusion specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( !isOlderExistingSpecification &&
									alternativeSpecificationItem->isSelfGeneratedAssumption() &&
									// Common word
									( possessiveNounWordItem = specificationWordItem->commonWordItem( specificationCollectionNr ) ) != NULL )
										{
										// Find possessive specifications
										if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"", possessiveNounWordItem->anyWordTypeString(), "\"" );

										if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != NULL &&
										( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
										( foundSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL &&
										foundSpecificationItem->hasSecondarySpecificationJustification( alternativeSpecificationItem ) &&
										// Replace assumption justification by conclusion justification of involved word
										currentContextWordItem->updateSpecificationOfJustificationsInWord( alternativeSpecificationItem, givenSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in the justification items in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							}
						else
							{
							if( isOlderAlternativeSpecification &&
							!isOlderExistingSpecification &&
							isOlderSpecification &&
							// Replace assumption by conclusion
							generalizationWordItem->replaceOrDeleteSpecification( ( isSelfGeneratedAssumption ? givenSpecificationItem : alternativeSpecificationItem ), ( isSelfGeneratedAssumption ? alternativeSpecificationItem : givenSpecificationItem ) ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption specification by a conclusion specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}

					// Typical for Spanish
					if( isSpanishCurrentLanguage )
						{
						if( isSpanishSpecificationFemaleWord &&
						spanishMasculineSpecificationWordItem != NULL )
							{
							spanishMasculineSpecificationItem = currentContextWordItem->firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem );

							if( existingSpecificationItem == NULL ||

							( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							hasUserSpecificationRelationContext ) &&

							!givenSpecificationItem->hasCurrentCreationSentenceNr() &&
							givenSpecificationItem->isSelfGeneratedSpecification() ) )
								{
								if( spanishMasculineSpecificationItem == NULL ||
								existingSpecificationItem == userSpecificationItem ||

								( !isPossessiveUserSpecification &&
								isUserGeneralizationWord ) ||

								( !spanishMasculineSpecificationItem->isOlderItem() &&
								spanishMasculineSpecificationItem->isHiddenSpanishSpecification() &&
								// Justification doesn't exist yet in specification
								!spanishMasculineSpecificationItem->hasPrimarySpecificationJustification( givenSpecificationItem ) &&
								// Justification has at least the same assumption level
								spanishMasculineSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, NULL, NULL, NULL ) ) )
									{
									givenSpecificationItem = givenSpecificationItem->updatedSpecificationItem();

									// Draw Spanish reversible conclusion
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, spanishMasculineSpecificationWordItem->nonCompoundCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"", currentContextWordItem->anyWordTypeString(), "\" and with masculine specification word \"", spanishMasculineSpecificationWordItem->anyWordTypeString(), "\"" );

									if( !isUserGeneralizationWord &&
									existingSpecificationItem == userSpecificationItem &&
									( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
									// Justification doesn't exist yet in question
									!createdOrFoundSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
									// Add opposite possessive justification
									createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the created or found specification" );
									}
								}
							else
								{
								if( hasGeneralizationWordCurrentlyConfirmedSpecification &&
								isOlderExistingSpecification &&
								!hasUserSpecificationAssumptionLevel &&
								specificationWordItem == userSpecificationWordItem &&
								spanishMasculineSpecificationItem != NULL &&
								spanishMasculineSpecificationItem->isHiddenSpanishSpecification() &&
								!spanishMasculineSpecificationItem->hasCurrentCreationSentenceNr() &&
								existingSpecificationItem->isSelfGeneratedAssumption() &&
								// Draw Spanish reversible conclusion (hidden)
								adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a hidden Spanish reversible conclusion about word \"", currentContextWordItem->anyWordTypeString(), "\" and with masculine specification word \"", spanishMasculineSpecificationWordItem->anyWordTypeString(), "\"" );
								}
							}
						else
							{
							if( existingSpecificationItem != NULL &&
							currentContextWordItem->isUserGeneralizationWord &&
							existingSpecificationItem->hasCompoundSpecificationCollection() &&
							givenSpecificationItem->isSelfGeneratedConclusion() &&
							// Add Spanish non-compound reversible conclusion
							adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, WORD_TYPE_PROPER_NOUN, specificationWordItem->nonCompoundCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, givenSpecificationItem, NULL, NULL, NULL, currentContextWordItem, specificationWordItem, generalizationWordItem ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish non-compound reversible conclusion about word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						}

					if( isExistingUserSpecification &&
					isOlderExistingSpecification &&
					!isOlderSpecification &&
					isSelfGeneratedAssumption &&
					// Justification doesn't exist yet in specification
					!givenSpecificationItem->hasPrimarySpecificationJustification( existingSpecificationItem ) &&
					// Add reversible user justification
					givenSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, NULL, NULL ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->_nextContextWordItem( relationContextNr ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawReversibleConclusions( bool isAssignment, bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasCurrentSpecificationCurrentCreationSentenceNr;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		bool hasNonCompoundCurrentSpecificationCollection;
		bool hasUserGeneralizationWordCurrentlyConfirmedSpecification = false;
		bool hasUserSpecificationCompoundCollection;
		bool hasUserSpecificationRelationContext = false;
		bool isActiveUserAssignment = false;
		bool isArchivedUserAssignment = false;
		bool isCurrentSpecificationSelfGeneratedAssumption;
		bool isCurrentSpecificationSelfGeneratedSpecification;
		bool isCurrentSpecificationWordSpanishAmbiguous = false;
		bool isOlderCurrentSpecification;
		bool isPossessiveUserSpecification = false;
		bool isSpanishCurrentLanguage;
		bool isUserGeneralizationWord;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned short generalizationWordTypeNr;
		unsigned short specificationWordTypeNr;
		unsigned short relationWordTypeNr;
		unsigned int currentRelationContextNr;
		unsigned int foundRelationContextNr;
		unsigned int selectedRelationContextNr;
		unsigned int specificationCollectionNr;
		unsigned int userSpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *firstContextWordItem;
		WordItem *userGeneralizationWordItem;
		WordItem *userSpecificationWordItem;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawReversibleConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) ) != NULL &&
		( userGeneralizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL &&
		( firstContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			specificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();

			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();

			hasUserGeneralizationWordCurrentlyConfirmedSpecification = userGeneralizationWordItem->hasCurrentlyConfirmedSpecification();
			hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
			hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
			isActiveUserAssignment = userSpecificationItem->isActiveAssignment();
			isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			if( isPossessiveUserSpecification )
				{
				if( isUserGeneralizationWord &&
				// Typical for Chinese: Skip possessive inactive assignment
				!userSpecificationItem->isInactiveAssignment() &&
				// Draw user reversible conclusion
				drawUserReversibleConclusions( currentSpecificationItem, generalizationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a user reversible conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			else	// Non-possessive user specification
				{
				if( !isUserGeneralizationWord &&
				hasUserSpecificationRelationContext &&
				( existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, userSpecificationWordItem, userGeneralizationWordItem ) ) != NULL &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&

				// Older specification without specification collection
				( !existingSpecificationItem->hasSpecificationCollection() ||
				existingSpecificationItem->isFirstJustificationReversibleAssumption() ||
				// Typical for Spanish
				// Spanish test file: "Complejo (13)"
				existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL ) )
					{
					// Draw reversible user conclusion
					if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible user conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );

					// Cleanup unreferenced justifications
					if( isActiveUserAssignment &&
					existingSpecificationItem->hasCurrentCreationSentenceNr() &&
					generalizationWordItem->cleanupUnreferencedJustifications() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to clean up unreferenced justifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}

			// Do for all selected specifications
			do	{
				hasCurrentSpecificationCurrentCreationSentenceNr = currentSpecificationItem->hasCurrentCreationSentenceNr();
				hasNonCompoundCurrentSpecificationCollection = currentSpecificationItem->hasNonCompoundSpecificationCollection();
				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem->isSpecificationWordSpanishAmbiguous();
				isOlderCurrentSpecification = currentSpecificationItem->isOlderItem();
				isCurrentSpecificationSelfGeneratedAssumption = currentSpecificationItem->isSelfGeneratedAssumption();
				isCurrentSpecificationSelfGeneratedSpecification = currentSpecificationItem->isSelfGeneratedSpecification();
				generalizationWordTypeNr = currentSpecificationItem->generalizationWordTypeNr();
				specificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();
				relationWordTypeNr = currentSpecificationItem->relationWordTypeNr();
				currentRelationContextNr = currentSpecificationItem->relationContextNr();
				specificationCollectionNr = currentSpecificationItem->specificationCollectionNr();

				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The secondary specification item has no specification word" );

				if( currentSpecificationItem->isPossessive() )
					{
					if( isCurrentSpecificationSelfGeneratedSpecification )
						{
						if( isUserGeneralizationWord ||
						specificationCollectionNr == NO_COLLECTION_NR )
							{
							if( !isSpanishCurrentLanguage ||
							!isCurrentSpecificationWordSpanishAmbiguous )
								{
								currentContextWordItem = firstContextWordItem;

								// Do for all context words
								do	{
									if( currentContextWordItem != generalizationWordItem &&
									( existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, currentSpecificationWordItem, generalizationWordItem ) ) != NULL )
										{
										// Found user specification, force self-generated assumption
										if( existingSpecificationItem->isUserSpecification() &&
										( tempSpecificationItem = currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, true, currentSpecificationWordItem, generalizationWordItem ) ) != NULL )
											existingSpecificationItem = tempSpecificationItem;

										if( hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation )
											{
											if( currentContextWordItem->isUserRelationWord &&
											( obsoleteJustificationItem = currentSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentContextWordItem ) ) != NULL )
												{
												// Create justification
												if( ( justificationResult = generalizationWordItem->createJustificationItem( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, userSpecificationItem, NULL, NULL, NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a reversible assumption justification item to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find or create a reversible assumption justification" );

												if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete reversible assumption justification of the current secondary specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										else
											{
											if( existingSpecificationItem->hasCurrentCreationSentenceNr() &&
											existingSpecificationItem->hasNonCompoundSpecificationCollection() &&

											( !hasCurrentSpecificationCurrentCreationSentenceNr ||
											!isOlderCurrentSpecification ) &&

											// Relation context exists in word
											currentContextWordItem->hasContextInWord( currentRelationContextNr ) &&
											// Justification doesn't exist yet in specification
											!existingSpecificationItem->hasPrimarySpecificationJustification( currentSpecificationItem ) &&

											// Justification has at least the same assumption level
											( existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentSpecificationItem, NULL, NULL, NULL ) ||
											// Force assumption to be less certain
											// Test files: "Complex (18)", "Complex (19 - mixed)" and "Complex (19 - strange)"
											existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != NULL ) &&

											// Add reversible justification
											existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentSpecificationItem, NULL, NULL ) != RESULT_OK )
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
							if( isSpanishCurrentLanguage &&
							hasNonCompoundCurrentSpecificationCollection )
								{
								if( isOlderCurrentSpecification &&
								isActiveUserAssignment &&
								// Justification doesn't exist yet in specification
								!currentSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
								// Draw Spanish non-compound reversible conclusion
								adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible specification from word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

								if( !isPossessiveUserSpecification &&
								isUserSpecificationWordSpanishAmbiguous &&
								// Specification doesn't exist yet with user specification collection
								generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, NO_CONTEXT_NR, userSpecificationWordItem ) == NULL )
									{
									// Draw Spanish reversible conclusion
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, isArchivedUserAssignment, false, false, false, false, false, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, userSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );

									if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
									// Specification doesn't exist yet with without specification collection
									( obsoleteSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, NO_COLLECTION_NR, NO_CONTEXT_NR, userSpecificationWordItem ) ) != NULL &&
									// Replace specification without specification collection with specification with specification
									// Spanish test file: "Juan - Ana (antes de la definición de la familia)"
									generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a specification without specification collection with a specification with specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					else	// User specification
						{
						if( !hasNonCompoundCurrentSpecificationCollection &&
						!isOlderCurrentSpecification )
							{
							currentContextWordItem = firstContextWordItem;

							// Do for all context words
							do	{
								if( currentContextWordItem != generalizationWordItem &&
								currentContextWordItem->isUserRelationWord &&
								currentContextWordItem->hasNegativeSpecification() &&
								// Draw a specification substitution conclusion or ask a question
								drawSpecificationSubstitutionConclusionOrAskQuestion( false, false, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, currentContextWordItem, currentSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
							}
						}
					}
				else	// Non-possessive
					{
					if( hasUserSpecificationRelationContext &&
					isCurrentSpecificationSelfGeneratedSpecification &&
					// Secondary specification word is same as user specification word
					currentSpecificationWordItem == userSpecificationWordItem )
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage )
							{
							if( hasUserSpecificationCompoundCollection &&

							( isUserGeneralizationWord ||
							hasNonCompoundCurrentSpecificationCollection ) &&

							// Justification doesn't exist yet in specification
							!currentSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Add Spanish reversible user justification
							currentSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish reversible user justification to the current secondary specification" );
							}
						else
							{
							if( isPossessiveUserSpecification &&
							isCurrentSpecificationSelfGeneratedAssumption )
								{
								if( ( hasUserGeneralizationWordCurrentlyConfirmedSpecification ||

								( ( existingSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, currentSpecificationWordItem, NULL ) ) != NULL &&
								// Justification doesn't exist yet in specification
								!existingSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) ) &&

								// Conclusion doesn't exist yet
								generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, userGeneralizationWordItem ) == NULL )
									{
									selectedRelationContextNr = NO_CONTEXT_NR;
									conclusionSpecificationItem = NULL;

									if( hasNonCompoundCurrentSpecificationCollection )
										{
										if( hasUserGeneralizationWordCurrentlyConfirmedSpecification )
											{
											if( ( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, NULL ) ) == NULL )
												{
												if( ( foundRelationContextNr = userGeneralizationWordItem->contextNr( currentSpecificationWordItem ) ) > NO_CONTEXT_NR &&
												adminItem_->nContextWords( foundRelationContextNr ) > 1 )
													{
													// Get relation context
													if( ( contextResult = adminItem_->getRelationContext( false, false, false, false, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem, NULL ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get a relation context" );

													selectedRelationContextNr = contextResult.contextNr;
													}
												}
											else
												selectedRelationContextNr = conclusionSpecificationItem->relationContextNr();
											}
										else
											selectedRelationContextNr = userGeneralizationWordItem->contextNr( currentSpecificationWordItem );
										}

									// Draw reversible user conclusion
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, relationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedRelationContextNr, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

									if( conclusionSpecificationItem == NULL )
										{
										if( createAndAssignResult.foundSpecificationItem != NULL &&
										( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
										// Add reversible justification to current secondary specification
										currentSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the current secondary specification" );
										}
									else
										{
										conclusionSpecificationItem = conclusionSpecificationItem->updatedSpecificationItem();

										// Confirmed all relation words
										if( conclusionSpecificationItem->nRelationContextWords() == currentSpecificationItem->nRelationContextWords() &&
										// Replace assumption by conclusion
										generalizationWordItem->replaceOrDeleteSpecification( currentSpecificationItem, conclusionSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an assumption without relation word" );
										}
									}
								}
							else
								{
								// Test file: "Complex (17)"
								if( isOlderCurrentSpecification &&
								( obsoleteSpecificationItem = generalizationWordItem->noRelationContextSpecificationItem( false, true, currentSpecificationWordItem ) ) != NULL &&
								// Replace obsolete assumption specification by conclusion
								generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, currentSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete assumption specification by a conclusion in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}

					currentContextWordItem = firstContextWordItem;
					currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

					if( currentSpecificationItem->hasRelationContext() &&
					// Draw possessive reversible conclusions
					drawPossessiveReversibleConclusions( isActiveUserAssignment, currentSpecificationItem, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw possessive reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem->isReplacedOrDeletedItem() ?
												// First specification
												generalizationWordItem->firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) :
												// Next specification
												currentSpecificationItem->nextNonQuestionNonRelationSpecificationItem() ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawUserReversibleConclusions( SpecificationItem *currentSecondarySpecificationItem, WordItem *generalizationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasNonCompoundUserSpecificationCollection = false;
		bool isArchivedAssignment;
		bool isAssignment;
		bool isSpanishCurrentLanguage;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned int spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int userSpecificationCollectionNr;
		JustificationItem *createdJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *assignmentSpecificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *assumptionSpecificationWordItem;
		WordItem *commonWordItem;
		WordItem *currentContextWordItem;
		WordItem *spanishFeminineSpecificationWordItem;
		WordItem *spanishMasculineSpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawUserReversibleConclusions";

		if( currentSecondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first secondary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentContextWordItem = globalVariables_->firstContextWordItem ) != NULL &&
		( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
		( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();

			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();

			hasNonCompoundUserSpecificationCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			// Do for all context words
			do	{
				if( currentContextWordItem->isUserRelationWord &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = currentContextWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, false, userSpecificationWordItem, generalizationWordItem ) ) == NULL ||

				( existingSpecificationItem->isSelfGeneratedSpecification() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) ) )
					{
					conclusionSpecificationItem = currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, NULL );

					if( isSpanishCurrentLanguage ||

					( !hasNonCompoundUserSpecificationCollection &&
					existingSpecificationItem == NULL ) ||

					( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
					// Self-generated conclusion with different relation word
					conclusionSpecificationItem != NULL ) ||

					( existingSpecificationItem != NULL &&
					// Conclusion doesn't exist yet
					conclusionSpecificationItem == NULL ) ||

					( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
					generalizationWordItem->firstActiveQuestionSpecificationItem() != NULL ) &&

					// Assumption doesn't exist yet
					currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, userSpecificationWordItem, NULL ) == NULL ) )
						{
						isAssignment = false;
						isArchivedAssignment = false;
						assignmentSpecificationItem = NULL;
						spanishAmbiguousSpecificationCollectionNr = NO_COLLECTION_NR;

						// Typical for Spanish
						if( isSpanishCurrentLanguage &&
						hasNonCompoundUserSpecificationCollection &&

						( !currentContextWordItem->hasNegativeSpecification() ||
						currentContextWordItem->hasFeminineProperNounEnding() ) )
							spanishAmbiguousSpecificationCollectionNr = userSpecificationCollectionNr;

						// Similar specification already exists as assignment
						// So, make this conclusion an assignment too
						if( existingSpecificationItem == NULL &&
						( assignmentSpecificationItem = currentContextWordItem->firstAssignmentItem( false, false, NO_CONTEXT_NR, userSpecificationWordItem ) ) != NULL )
							{
							isAssignment = true;
							isArchivedAssignment = assignmentSpecificationItem->isArchivedAssignment();
							}

						// Draw reversible user conclusion
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, currentContextWordItem, userSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible user conclusion about word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", userSpecificationWordItem->anyWordTypeString(), "\"" );

						// Typical for Spanish
						if( isUserSpecificationWordSpanishAmbiguous )
							{
							if( currentContextWordItem->hasFeminineProperNounEnding() &&
							( spanishFeminineSpecificationWordItem = userSpecificationWordItem->feminineCollectionWordItem() ) != NULL &&
							( definitionSpecificationItem = userSpecificationWordItem->firstSpecificationItem( false, false, false, spanishFeminineSpecificationWordItem ) ) != NULL &&
							currentContextWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, NO_QUESTION_PARAMETER, definitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, spanishFeminineSpecificationWordItem ) == NULL &&
							// Draw Spanish feminine exclusive specification substitution assumption
							adminItem_->addSelfGeneratedSpecification( true, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, currentContextWordItem, spanishFeminineSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish exclusive specification substitution assumption about word \"", currentContextWordItem->anyWordTypeString(), "\" with specification word \"", spanishFeminineSpecificationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
								{
								if( existingSpecificationItem == NULL )
									{							// Assignment specification
									foundSpecificationItem = ( assignmentSpecificationItem != NULL ? assignmentSpecificationItem :
																// Found specification
																( createAndAssignResult.foundSpecificationItem != NULL ? createAndAssignResult.foundSpecificationItem :
																// User specification
																currentContextWordItem->firstUserSpecificationItem( false, false, createdSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, userSpecificationWordItem ) ) );

									if( foundSpecificationItem == NULL )
										{
										if( hasNonCompoundUserSpecificationCollection )
											{
											// Test file: "Complex (3)"
											// Draw only option left conclusion
											if( drawOnlyOptionLeftProperNounConclusion( createdSpecificationItem, currentContextWordItem, userSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only-option-left proper noun conclusion" );


											if( ( commonWordItem = userSpecificationWordItem->commonWordItem( userSpecificationCollectionNr ) ) != NULL &&
											( assumptionSpecificationWordItem = userSpecificationWordItem->collectionWordItem( userSpecificationCollectionNr ) ) != NULL &&
											( definitionSpecificationItem = commonWordItem->firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != NULL &&
											( assumptionSpecificationItem = currentContextWordItem->firstSpecificationItem( false, true, false, assumptionSpecificationWordItem ) ) != NULL &&
											// Justification doesn't exist yet in specification
											!assumptionSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, createdSpecificationItem ) &&
											// Add opposite possessive assumption justification to assumption specification
											assumptionSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, NULL, createdSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive assumption justification to an assumption specification" );
											}
										}
									else
										{
										if( foundSpecificationItem->hasOnlyOneRelationWord() &&
										// Add reversible user justification to created specification
										createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the created specification" );
										}
									}
								else
									{
									if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
										{
										if( foundSpecificationItem->hasCompoundSpecificationCollection() )
											{
											// Replace an assumption specification by a conclusion specification in justifications of involved word
											if( currentContextWordItem->updateSpecificationOfJustificationsInWord( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to update the specifications in a justification in word \"", currentContextWordItem->anyWordTypeString(), "\"" );

											// Add reversible user justification to created specification
											if( foundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, NULL ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible user justification to the created specification" );

											// Typical for Spanish
											// Spanish test file: "Complejo (15)"
											if( isSpanishCurrentLanguage &&
											( spanishMasculineSpecificationWordItem = userSpecificationWordItem->masculineCollectionWordItem() ) != NULL &&
											( assumptionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem ) ) != NULL &&
											!assumptionSpecificationItem->isHiddenSpanishSpecification() &&
											// Add Spanish reversible justification to assumption specification
											assumptionSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, NULL, NULL ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish reversible justification to the assumption specification" );
											}
										else
											{
											if( ( obsoleteJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, generalizationWordItem ) ) != NULL )
												{
												// Test file: "You have confirmed at least one relation word"
												// Create justification
												if( ( justificationResult = currentContextWordItem->createJustificationItem( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, NO_ORDER_NR, globalVariables_->currentSentenceNr, userSpecificationItem, NULL, NULL, NULL, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a justification item to word \"", currentContextWordItem->anyWordTypeString(), "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
													return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create an additional justification" );

												// Replace negative assumption justification
												if( currentContextWordItem->replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a negative assumption justification in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}
								}
							}
						}
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	FindSpecificationResultType findPossessiveSpecifications( WordItem *possessiveNounWordItem, WordItem *specificationWordItem )
		{
		bool hasFoundPossessiveSpecificationWord = true;
		unsigned int possessiveSpecificationCollectionNr;
		SpecificationItem *foundOppositePossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *foundPossessiveDefinitionSpecificationItem = NULL;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		SpecificationItem *previousPossessiveSpecificationItem = NULL;
		WordItem *higherLevelPossessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findPossessiveSpecifications";

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
					if( higherLevelPossessiveDefinitionSpecificationWordItem == NULL )
						higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
					else
						hasFoundPossessiveSpecificationWord = false;
					}
				}
			while( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );

			if( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
				{
				if( higherLevelPossessiveDefinitionSpecificationWordItem == NULL )
					higherLevelPossessiveDefinitionSpecificationWordItem = specificationWordItem;

				// Try to find the opposite of the possessive specification
				do	{
					if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
						{
						if( previousPossessiveSpecificationItem != NULL )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem->specificationCollectionNr();

							if( higherLevelPossessiveDefinitionSpecificationWordItem->hasCollectionNr( possessiveSpecificationCollectionNr ) )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem :
																																												possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem :
																																									previousPossessiveSpecificationItem );
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

	signed char findQuestionToBeAdjustedByCompoundCollection( WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem, SpecificationItem *definitionSpecificationItem )
		{
		bool hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		unsigned int definitionSpecificationCollectionNr;
		SpecificationItem *adjustedQuestionCompoundSpecificationItem;
		SpecificationItem *lastAdjustedQuestionCompoundSpecificationItem = NULL;
		SpecificationItem *replacingSpecificationItem = NULL;
		WordItem *commonWordItem;
		WordItem *currentCollectionWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findQuestionToBeAdjustedByCompoundCollection";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( definitionSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given definition specification item is undefined" );

		if( ( definitionSpecificationCollectionNr = definitionSpecificationItem->specificationCollectionNr() ) <= NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given definition specification item has no specification collection" );

		if( ( currentCollectionWordItem = globalVariables_->firstCollectionWordItem ) != NULL )
			{
			hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem->hasCurrentlyMoreSpecificSpecification();

			// Do for all collection words
			do	{
				if( ( commonWordItem = currentCollectionWordItem->commonWordItem( definitionSpecificationCollectionNr ) ) != NULL &&
				( adjustedQuestionCompoundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, currentCollectionWordItem ) ) != NULL )
					{
					if( commonWordItem == specificationWordItem )
						{
						// Skip updating question if the old justification has no relation word,
						// while the current user specification has a relation word.
						// Test file: "This information is more specific"
						if( !hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
						// Justification doesn't exist yet in specification
						!adjustedQuestionCompoundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) )
							{
							// Create adjusted question
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, adjustedQuestionCompoundSpecificationItem->isInactiveAssignment(), adjustedQuestionCompoundSpecificationItem->isArchivedAssignment(), false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, adjustedQuestionCompoundSpecificationItem->generalizationWordTypeNr(), adjustedQuestionCompoundSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, currentCollectionWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create an adjusted question about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );

							replacingSpecificationItem = createAndAssignResult.foundSpecificationItem;
							}
						}
					else
						{
						if( generalizationWordItem->replaceOrDeleteSpecification( adjustedQuestionCompoundSpecificationItem, ( replacingSpecificationItem == NULL || replacingSpecificationItem->isReplacedOrDeletedItem() ? NULL : replacingSpecificationItem ) ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace or delete a question part" );

						lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem->updatedSpecificationItem();
						}
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != NULL &&
		// Write last instance of adjusted compound question
		generalizationWordItem->writeUpdatedSpecification( true, false, false, true, false, false, false, lastAdjustedQuestionCompoundSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an adjusted compound question" );

		return RESULT_OK;
		}

	signed char makeOppositePossessiveSpecificationAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *alternativeSecondarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool hasExistingSpecificationCurrentCreationSentenceNr = false;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		bool hasFeminineOrMasculineProperNounEnding = false;
		bool hasRelationWordConfirmedSpecification;
		bool hasRelationWordMasculineProperNounEnding = false;
		bool hasSameSpecificationCollection;
		bool isAlternativeSecondaryUserSpecification = false;
		bool isExistingHiddenSpanishSpecification = false;
		bool isExistingUserSpecification = false;
		bool isOlderCurrentExistingSpecification = false;
		bool isOlderExistingSpecification = false;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isRelationWordUserWordGeneralization;
		bool isSecondaryUserSpecification;
		unsigned short currentExistingAssumptionLevel = NO_ASSUMPTION_LEVEL;
		unsigned short selectedJustificationTypeNr;
		unsigned int createAssumptionLevel;
		unsigned int hiddenSpanishSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int relationContextNr;
		SpecificationItem *additionalDefinitionSpecificationItem = NULL;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentExistingSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *feminineSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *selectedSpecificationItem = NULL;
		SpecificationItem *tempSpecificationItem;
		WordItem *alternativeSecondarySpecificationWordItem;
		WordItem *currentContextWordItem;
		WordItem *feminineSpecificationWordItem = NULL;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem;
		WordItem *possessiveDefinitionSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeOppositePossessiveSpecificationAssumption";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( oppositePossessiveDefinitionSpecificationItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item is undefined" );

		if( possessiveDefinitionSpecificationItem_ == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The possessive definition specification item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item has no specification word" );

		if( ( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The The possessive definition specification item has no specification word item" );

		isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();

		if( !isPossessiveSpecificationWordSpanishAmbiguous &&
		secondarySpecificationItem->hasCompoundSpecificationCollection() )
			return adminItem_->startError( functionNameString, moduleNameString_, "Some preconditions are not met" );

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
		hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem->hasCurrentlyMoreSpecificSpecification();
		hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();
		isRelationWordUserWordGeneralization = relationWordItem->isUserGeneralizationWord;

		isSecondaryUserSpecification = secondarySpecificationItem->isUserSpecification();

		if( alternativeSecondarySpecificationItem != NULL )
			isAlternativeSecondaryUserSpecification = alternativeSecondarySpecificationItem->isUserSpecification();

		if( ( existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, !isPossessive, possessiveDefinitionSpecificationWordItem ) ) != NULL )
			{
			hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem->hasCurrentCreationSentenceNr();
			isExistingHiddenSpanishSpecification = existingSpecificationItem->isHiddenSpanishSpecification();
			isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
			isExistingUserSpecification = existingSpecificationItem->isUserSpecification();
			}

		if( isPossessive )
			{
			selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;

			selectedPrimarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? possessiveDefinitionSpecificationItem_ :
																								oppositePossessiveDefinitionSpecificationItem_ );

			selectedSecondarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																									secondarySpecificationItem );
			}
		else
			{
			selectedJustificationTypeNr = JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION;

			selectedPrimarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																								secondarySpecificationItem );
			selectedSecondarySpecificationItem = NULL;

			}

		// User specification doesn't exist
		if( ( ( existingUserSpecificationItem = relationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) == NULL &&
		alternativeSecondarySpecificationItem != NULL &&
		!possessiveDefinitionSpecificationWordItem->isUserSpecificationWord &&

		( ( isPossessive &&
		!isPossessiveSpecificationWordSpanishAmbiguous &&
		!isOlderExistingSpecification &&
		isRelationWordUserWordGeneralization &&
		existingSpecificationItem != NULL &&
		// Justification doesn't exist yet in specification
		!existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ||

		// Typical for Spanish
		( isPossessiveSpecificationWordSpanishAmbiguous &&
		isRelationWordUserWordGeneralization &&
		existingSpecificationItem == NULL ) ) ) ||

		// Typical for Spanish
		( isPossessiveSpecificationWordSpanishAmbiguous &&
		alternativeSecondarySpecificationItem != NULL &&

		( existingUserSpecificationItem != NULL ||

		( isAlternativeSecondaryUserSpecification &&

		( existingSpecificationItem == NULL ||
		!existingSpecificationItem->hasNonCompoundSpecificationCollection() ||

		// Justification doesn't exist yet in specification
		( !existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ) ) ) &&

		// User specification doesn't exist yet
		( ( existingUserSpecificationItem == NULL &&

		( existingSpecificationItem == NULL ||

		( ( hasExistingSpecificationCurrentCreationSentenceNr ||
		!alternativeSecondarySpecificationItem->isOlderItem() ) &&

		// Justification doesn't exist yet in specification
//		!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ) ) ||

		// User specification exists
		( existingUserSpecificationItem != NULL &&
		existingUserSpecificationItem->isOlderItem() &&
		existingUserSpecificationItem->hasCompoundSpecificationCollection() &&

		( hasGeneralizationWordCurrentlyConfirmedSpecification ||

		( isRelationWordUserWordGeneralization &&
		existingSpecificationItem != NULL &&
		// Justification doesn't exist yet in specification
		!existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) ) ) ) ) &&

		( ( !isPossessive &&
		existingUserSpecificationItem != NULL ) ||

		alternativeSecondarySpecificationItem->isActiveAssignment() ||

		// User specification doesn't exist
		( //existingUserSpecificationItem == NULL &&
		!generalizationWordItem->hasActiveQuestionWithCompoundCollection() &&

		// Not hidden
		( !isExistingHiddenSpanishSpecification ||

		// Hidden
		( isOlderExistingSpecification &&
		hasExistingSpecificationCurrentCreationSentenceNr ) ) ) ) ) )
			{
			// Make an opposite possessive specification assumption (normal)
			if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, false, false, !isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem->specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption about relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
			}
		else
			{
			if( isPossessiveSpecificationWordSpanishAmbiguous &&
			// Draw Spanish proper noun part-of conclusions in proper noun words
			drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
			}

		if( existingSpecificationItem != NULL &&
		alternativeSecondarySpecificationItem != NULL &&
		alternativeSecondarySpecificationItem->isPossessive() &&
		!existingSpecificationItem->hasCurrentCreationSentenceNr() &&
		existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
		// Avoid idle call
		possessiveDefinitionSpecificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL &&
		// Draw a specification substitution conclusion or ask a question
		drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", relationWordItem->anyWordTypeString(), "\"" );

		if( existingSpecificationItem != NULL ||
		alternativeSecondarySpecificationItem != NULL )
			{
			if( alternativeSecondarySpecificationItem != NULL )
				{
				if( ( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem->specificationWordItem() ) != NULL &&
				// Avoid idle call
				alternativeSecondarySpecificationWordItem->firstNegativeSpecificationItem() != NULL &&
				( foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, alternativeSecondarySpecificationWordItem ) ) != NULL &&
				!foundSpecificationItem->isHiddenSpanishSpecification() &&
				// Draw simple negative conclusions
				drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, alternativeSecondarySpecificationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				// Try to find user specification or self-generated conclusion first
				if( ( selectedSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem, NULL ) ) != NULL &&
				isAlternativeSecondaryUserSpecification &&
				selectedSpecificationItem->isHiddenSpanishSpecification() )
					selectedSpecificationItem = NULL;

				// If not found, try to find a self-generated assumption
				if( selectedSpecificationItem == NULL &&
				( assumptionSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL )
					{
					hasSameSpecificationCollection = ( assumptionSpecificationItem->specificationCollectionNr() == alternativeSecondarySpecificationItem->specificationCollectionNr() );

					if( !assumptionSpecificationItem->hasCurrentCreationSentenceNr() &&
					!assumptionSpecificationItem->hasNewInformation() &&

					( hasGeneralizationWordCurrentlyConfirmedSpecification == hasSameSpecificationCollection ||
					alternativeSecondarySpecificationItem->isPossessive() ) )
						selectedSpecificationItem = assumptionSpecificationItem;
					}
				}

			isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();
			feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationWordItem->feminineCollectionWordItem() :
																										oppositePossessiveDefinitionSpecificationWordItem->feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem->hasFeminineProperNounEnding() : relationWordItem->hasFeminineProperNounEnding() ) );

			if( feminineSpecificationWordItem != NULL )
				{
				if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
					additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
				else
					feminineSpecificationWordItem = NULL;
				}

			if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
			isAlternativeSecondaryUserSpecification &&
			isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) )
				hasFeminineOrMasculineProperNounEnding = true;

			if( ( ( selectedSpecificationItem == NULL ||

			( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
			!hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
			!isRelationWordUserWordGeneralization &&
			existingSpecificationItem != NULL &&
			existingSpecificationItem->isSelfGeneratedSpecification() &&

			( additionalDefinitionSpecificationItem != NULL ||
			!selectedSpecificationItem->hasRelationContext() ||

			( selectedSpecificationItem->isSelfGeneratedSpecification() &&
			relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) == NULL ) ) ) ||

			( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
			isRelationWordUserWordGeneralization &&
			selectedSpecificationItem->hasOnlyOneRelationWord() &&
			!selectedSpecificationItem->isHiddenSpanishSpecification() ) ||

			// Relation context doesn't exist yet
			( generalizationWordItem->hasContextInWord( selectedSpecificationItem->relationContextNr() ) &&
			// Try to find user specification or self-generated conclusion first
			relationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, true, oppositePossessiveDefinitionSpecificationWordItem, NULL ) != NULL ) ||

			( isRelationWordUserWordGeneralization &&
			selectedSpecificationItem->isSelfGeneratedSpecification() &&
			relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) != NULL ) ||

			( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

			( ( relationContextNr = selectedSpecificationItem->relationContextNr() ) == NO_CONTEXT_NR ||

			( !isRelationWordUserWordGeneralization &&
			selectedSpecificationItem->isUserSpecification() &&
			// Relation context doesn't exist
			!generalizationWordItem->hasContextInWord( relationContextNr ) ) ) ) ) &&

			( isOppositePossessiveSpecificationWordSpanishAmbiguous ||

			( ( relationWordItem->secondarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == NULL ||

			( feminineSpecificationWordItem != NULL &&
			// Skip if feminine specification exists as user specification
			relationWordItem->firstUserSpecificationItem( false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, feminineSpecificationWordItem ) == NULL ) ) ) ) ) &&

			( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
			// Typical for Spanish
			!isPossessive ||
			isRelationWordUserWordGeneralization ||
			additionalDefinitionSpecificationItem == NULL ||
			generalizationWordItem->firstUserSpecificationItem( false, isPossessive, additionalDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NULL ) != NULL ) )
				{
				// Typical for Spanish
				if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
				existingSpecificationItem != NULL &&

				( isExistingUserSpecification ||
				isExistingHiddenSpanishSpecification ) )
					hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_->specificationCollectionNr();

				if( ( currentExistingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL )
					{
					isOlderCurrentExistingSpecification = currentExistingSpecificationItem->isOlderItem();
					currentExistingAssumptionLevel = currentExistingSpecificationItem->assumptionLevel();
					}

				selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
				selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem_;
				selectedAdditionalDefinitionSpecificationItem = NULL;
				selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																												secondarySpecificationItem );

				createAssumptionLevel = adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL );

				if( ( alternativeSecondarySpecificationItem == NULL ||

				// Non-possessive
				( !isPossessive &&

				// Current existing specification exists
				( !isOlderCurrentExistingSpecification ||
				isSecondaryUserSpecification ||

				( !isExistingUserSpecification &&
				!isOppositePossessiveSpecificationWordSpanishAmbiguous &&
				currentExistingAssumptionLevel >= createAssumptionLevel ) ||

				( isExistingUserSpecification &&
				isRelationWordUserWordGeneralization &&
				!isOppositePossessiveSpecificationWordSpanishAmbiguous ) ) ) ||

				// Typical for Spanish
				( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

				( existingSpecificationItem == NULL ||
				currentExistingSpecificationItem == NULL ||

				// Spanish test file: "Español ambigua - Luisa - Pablo - Luisa"
				( additionalDefinitionSpecificationItem == NULL &&
				currentExistingSpecificationItem->hasOnlyOneRelationWord() ) ||

				// Test file: "Que haya confirmado al menos una palabra de relación"
				( ( !hasRelationWordConfirmedSpecification &&

				( !isOlderExistingSpecification ||
				!isPossessive ||
				currentExistingAssumptionLevel >= createAssumptionLevel ) ) ||

				( ( !existingSpecificationItem->isSelfGeneratedAssumption() ||
				currentExistingSpecificationItem->hasCurrentCreationSentenceNr() ) &&

				( ( !hasFeminineOrMasculineProperNounEnding &&
				currentExistingAssumptionLevel >= createAssumptionLevel ) ||

				existingSpecificationItem->isSelfGeneratedSpecification() ||

				( isOlderCurrentExistingSpecification &&
				!hasFeminineOrMasculineProperNounEnding &&
				!alternativeSecondarySpecificationItem->isOlderItem() ) ) ) ) ) ) ) &&

				( currentExistingSpecificationItem == NULL ||

				( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
				currentExistingSpecificationItem->isHiddenSpanishSpecification() ) ||

				// Justification doesn't exist yet in specification
				!currentExistingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) ) )
					{
					if( !isPossessive &&
					additionalDefinitionSpecificationItem == NULL &&
					relationWordItem->hasMasculineProperNounEnding() &&
					( tempSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL )
						{
						hasRelationWordMasculineProperNounEnding = true;
						selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
						selectedPrimarySpecificationItem = selectedSecondarySpecificationItem;
						selectedAdditionalDefinitionSpecificationItem = possessiveDefinitionSpecificationItem_;
						selectedSecondarySpecificationItem = tempSpecificationItem;
						}

					// Make an assumption
					if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an assumption from generalization word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

					if( isPossessive )
						{
						// Typical for Spanish
						if( hasRelationWordConfirmedSpecification &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL &&
						( relationContextNr = foundSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
						( currentContextWordItem = adminItem_->firstContextWordItem( relationContextNr ) ) != NULL )
							{
							// Do for all context words with relation context
							do	{
								if( currentContextWordItem != generalizationWordItem &&
								( selectedSecondarySpecificationItem = currentContextWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != NULL &&
								// Justification doesn't exist yet in specification
								!foundSpecificationItem->hasSecondarySpecificationJustification( selectedSecondarySpecificationItem ) &&
								// Justification has at least the same assumption level
								foundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
								// Add Spanish opposite possessive conclusion justification to found specification
								foundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a Spanish opposite possessive conclusion justification to the found specification" );
								}
							while( ( currentContextWordItem = currentContextWordItem->_nextContextWordItem( relationContextNr ) ) != NULL );
							}
						}
					else
						{
						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
							{
							if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == NULL )
								{
								// Typical for Spanish
								// Spanish test file: "Reto científico"
								if( isRelationWordUserWordGeneralization &&
								hiddenSpanishSpecificationCollectionNr > NO_COLLECTION_NR &&
								additionalDefinitionSpecificationItem == NULL &&
								relationWordItem->hasFeminineProperNounEnding() &&
								( feminineSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstNegativeSpecificationItem() ) != NULL &&
								( feminineSpecificationWordItem = feminineSpecificationItem->specificationWordItem() ) != NULL )
									{
									// Make feminine assumption
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, relationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine assumption from generalization word \"", relationWordItem->anyWordTypeString(), "\" to specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );

									if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
									// Specification doesn't exist yet
									( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL, NULL ) &&
									// Add reversible justification to existing specification
									existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
									}
								}
							else
								{
								if( isOlderExistingSpecification &&
								!isOppositePossessiveSpecificationWordSpanishAmbiguous &&
								// Justification has at least the same assumption level
								foundSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL, NULL ) &&
								// Add reversible justification
								createdSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, NULL, NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a reversible justification to the found specification" );
								}
							}
						}
					}
				else
					{
					// Typical for Spanish
					// Spanish test file: "Sé (10)"
					if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
					isRelationWordUserWordGeneralization &&
					secondarySpecificationItem->isHiddenSpanishSpecification() &&
					currentExistingSpecificationItem != NULL &&
					!currentExistingSpecificationItem->isHiddenSpanishSpecification() &&
					// Draw a specification substitution conclusion or ask a question
					drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}

				if( alternativeSecondarySpecificationItem != NULL &&
				feminineSpecificationWordItem != NULL &&

				( ( isPossessive ||
				!secondarySpecificationItem->isSelfGeneratedAssumption() ||
				generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, secondarySpecificationWordItem, relationWordItem ) == NULL ) &&

				( possessiveDefinitionSpecificationWordItem == secondarySpecificationWordItem ||

				( !isPossessiveSpecificationWordSpanishAmbiguous &&
				isSecondaryUserSpecification ) ) ) &&

				( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem->specificationWordItem() ) != NULL )
					{
					existingSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem );

					selectedPrimarySpecificationItem = ( isPossessive &&
														existingSpecificationItem == NULL &&
														userSpecificationItem->assumptionLevel() < secondarySpecificationItem->assumptionLevel() ? userSpecificationItem :

														( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
														possessiveDefinitionSpecificationWordItem == alternativeSecondarySpecificationWordItem ? alternativeSecondarySpecificationItem :
																																				secondarySpecificationItem ) );
					selectedAdditionalDefinitionSpecificationItem = ( !isPossessive &&
																	isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationItem_ :
																															possessiveDefinitionSpecificationItem_ );
					selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;

					if( existingSpecificationItem == NULL ||

					( !selectedPrimarySpecificationItem->isPossessive() &&
					// Justification doesn't exist yet in specification
					!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) )
						{
						relationContextNr = ( existingSpecificationItem == NULL ? NO_CONTEXT_NR : existingSpecificationItem->relationContextNr() );

						// Make a feminine possessive exclusive specification substitution assumption
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, relationContextNr, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine possessive exclusive specification substitution assumption with generalization word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );

						if( isPossessive )
							{
							if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&

							// Specification doesn't exist yet
							( ( existingSpecificationItem = generalizationWordItem->firstNonExclusiveSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) == NULL ||

							// Justification doesn't exist yet in specification
							( !existingSpecificationItem->hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, NULL, NULL, NULL ) ) ) &&
							// Draw possessive reversible conclusion
							adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, NULL, NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a possessive reversible conclusion with generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							feminineSpecificationWordItem_ = feminineSpecificationWordItem;

							// Typical for Spanish
							// Spanish test file: "Juan - Ana (antes de la definición de la familia)"
							if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
							existingSpecificationItem != NULL &&
							!existingSpecificationItem->isOlderItem() &&
							( selectedPrimarySpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != NULL &&
							// Justification doesn't exist yet in specification
							!existingSpecificationItem->hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) &&
							// Add possessive exclusive specification substitution justification to existing specification
							existingSpecificationItem->addJustificationToSpecification( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a possessive exclusive specification substitution assumption justification to the existing specification" );
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

		alternativeSpanishAdditionalDefinitionSpecificationItem_ = NULL;

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
				alternativeSpanishAdditionalDefinitionSpecificationItem_ = possessiveDefinitionSpecificationItem_;
				return possessiveSpecificationItem;
				}
			}

		return NULL;
		}

	SpecificationItem *getAdditionalDefinitionSpecificationItem( WordItem *userSpecificationWordItem )
		{
		SpecificationItem *additionalDefinitionSpecificationItem = NULL;
		SpecificationItem *possessiveDefinitionSpecificationItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *higherLevelPossessiveDefinitionSpecificationWordItem = NULL;
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
							higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveDefinitionSpecificationWordItem == NULL &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() ) != NULL );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem->firstPossessiveSpecificationItem() ) != NULL )
						additionalDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == NULL ||
																higherLevelPossessiveDefinitionSpecificationWordItem == possessiveDefinitionSpecificationItem->specificationWordItem() ? possessiveDefinitionSpecificationItem->nextPossessiveSpecificationItem() :
																																														possessiveDefinitionSpecificationItem );
					}
				}
			while( additionalDefinitionSpecificationItem == NULL &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );
			}

		return additionalDefinitionSpecificationItem;
		}


	protected:
	// Constructor

	AdminReasoning( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
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

	void initializeAdminReasoningVariables()
		{
		hasSuggestiveQuestionAssumption_ = false;

		adjustedSpanishQuestionSecondarySpecificationItem_ = NULL;

		feminineSpecificationWordItem_ = NULL;
		firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;
		secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = NULL;
		}

	signed char askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool hasNonCompoundSecondarySpecificationCollection;
		bool hasUserSpecificationRelationContext;
		bool isAdjectiveSecondarySpecification;
		bool isCommonWordWordSpanishAmbiguous = false;
		bool isExclusiveSecondarySpecification;
		bool isFeminineUserSpecificationWord = false;
		bool isHiddenSpanishPrimarySpecification;
		bool isMasculineUserSpecificationWord = false;
		bool isPrimarySameAsUserSpecification;
		bool isPrimaryUserSpecification;
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		unsigned int compoundSpecificationCollectionNr;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int secondarySpecificationCollectionNr;
		unsigned int specificationCollectionNr;
		unsigned int userSpecificationCollectionNr;
		SpecificationItem *conclusionSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *firstNonCompoundCollectionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordItem *collectionWordItem = NULL;
		WordItem *commonWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "askSpecificationSubstitutionQuestionOrDrawNegativeConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( secondarySpecificationCollectionNr = secondarySpecificationItem->specificationCollectionNr() ) == NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification collection" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( commonWordItem = secondarySpecificationWordItem->commonWordItem( secondarySpecificationCollectionNr ) ) != NULL )
			isCommonWordWordSpanishAmbiguous = commonWordItem->isNounWordSpanishAmbiguous();

		if( commonWordItem == NULL ||
		( specificationCollectionNr = commonWordItem->collectionNr( specificationWordItem ) ) == NO_COLLECTION_NR )
			specificationCollectionNr = secondarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationWordItem ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		isHiddenSpanishPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();
		isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
		isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();

		hasNonCompoundSecondarySpecificationCollection = secondarySpecificationItem->hasNonCompoundSpecificationCollection();
		isAdjectiveSecondarySpecification = secondarySpecificationItem->isSpecificationAdjective();
		isExclusiveSecondarySpecification = secondarySpecificationItem->isExclusiveSpecification();

		hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
		hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
		hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();
		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
		userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		commonWordItem != NULL &&
		relatedSpecificationItem == NULL &&
		!userSpecificationItem->isPossessive() &&
		( nonCompoundSpecificationCollectionNr = commonWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR )
			{
			if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, NO_QUESTION_PARAMETER, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, secondarySpecificationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

			relatedSpecificationItem = relatedResult.relatedSpecificationItem;
			}

		if( relatedSpecificationItem == NULL )
			{
			if( !hasUserSpecificationRelationContext &&
			!isPrimarySameAsUserSpecification &&
			!userSpecificationItem->isExclusiveSpecification() &&

			( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) != NULL &&

			( isAdjectiveSecondarySpecification ||
			!userSpecificationWordItem->isNounWordSpanishAmbiguous() ||

			( ( compoundSpecificationCollectionNr = secondarySpecificationWordItem->compoundCollectionNr() ) > NO_COLLECTION_NR &&
			secondarySpecificationWordItem->compoundGeneralizationWordItem( compoundSpecificationCollectionNr ) != userSpecificationWordItem ) ) &&

			( globalVariables_->nUserSpecificationWords == 1 ||
			// Typical for Spanish
			userSpecificationWordItem->isNounWordSpanishAmbiguous() ) )
				{
				if( userSpecificationWordItem->isFeminineWord() )
					isFeminineUserSpecificationWord = true;

				if( userSpecificationWordItem->isMasculineWord() )
					isMasculineUserSpecificationWord = true;
				}

			if( isFeminineUserSpecificationWord ||
			isMasculineUserSpecificationWord ||

			// Specification is Spanish ambiguous
			( isSpecificationWordSpanishAmbiguous &&
			!hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&

			( hasGeneralizationWordFeminineProperNounEnding ||
			userSpecificationItem->isQuestion() ) ) )
				{
				}
			else
				{
				// Skip invalid questions (step 1)
				if( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
				primarySpecificationItem->hasCompoundSpecificationCollection() ||

				( isPrimarySameAsUserSpecification &&
				!secondarySpecificationWordItem->isFeminineOrMasculineWord() ) ) &&

				( !isSpecificationWordSpanishAmbiguous ||
				// Typical for Spanish
				hasNonCompoundSecondarySpecificationCollection ||
				secondarySpecificationWordItem->compoundCollectionNr() == secondarySpecificationCollectionNr ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem );
					else
						collectionWordItem = secondarySpecificationWordItem->collectionWordItem( secondarySpecificationCollectionNr );

					// Skip invalid questions (step 2)
					if( ( collectionWordItem != NULL &&

					( isPrimaryUserSpecification ||

					( primarySpecificationItem->hasRelationContext() ||
					generalizationWordItem->primaryAndSecondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, secondarySpecificationItem ) == NULL ) ) &&

					generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, collectionWordItem ) == NULL ) ||

					( isSpecificationWordSpanishAmbiguous &&

					( ( !isPrimaryUserSpecification &&

					( existingSpecificationItem == NULL ||
					existingSpecificationItem->isHiddenSpanishSpecification() ) ) ||

					( isPrimaryUserSpecification &&
					primarySpecificationItem->isExclusiveSpecification() ) ) ) )
						{
						if( ( isExclusiveSecondarySpecification ||
						isPrimarySameAsUserSpecification ||
						hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ) &&

						// Ask questions
						askQuestions( userSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions" );

						if( !hasUserSpecificationRelationContext &&
						isAdjectiveSecondarySpecification &&
						isUserGeneralizationWord &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Typical for Spanish
						// Spanish test file: "Esta información es más específico (no exclusiva)"
						!isExclusiveSecondarySpecification ) &&

						// Question doesn't exist yet as specification
						generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, specificationCollectionNr, secondarySpecificationWordItem ) == NULL &&
						// Justification doesn't exist yet in word
						generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, secondarySpecificationItem ) == NULL &&
						// Ask adjective question
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, WORD_TYPE_ADJECTIVE, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask an adjective question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
						}
					}
				else
					{
					// Test files: "Scientific challenge" and "John - Anna (before family definition)"
					// Definition specification
					if( userSpecificationItem->isGeneralizationNoun() &&
					primarySpecificationItem->hasRelationContext() &&
					primarySpecificationItem->isOlderItem() &&
					primarySpecificationItem->isSelfGeneratedSpecification() &&

					// Question part doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == NULL ||
					// Justification doesn't exist yet in question
					!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) ) &&

					// Ask a specification substitution question
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem->generalizationWordTypeNr(), secondarySpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", secondarySpecificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}
		else
			{
			if( isExclusiveSecondarySpecification )
				{
				if( !isCommonWordWordSpanishAmbiguous &&
				( firstNonCompoundCollectionSpecificationItem = generalizationWordItem->firstNonCompoundCollectionSpecificationItem( secondarySpecificationCollectionNr ) ) != NULL )
					relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

				// Typical for Spanish
				if( isCommonWordWordSpanishAmbiguous &&
				isSpecificationWordSpanishAmbiguous &&
				isPrimaryUserSpecification &&
				globalVariables_->nUserSpecificationWords == 1 &&
				!primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
				primarySpecificationItem->specificationCollectionNr() != secondarySpecificationCollectionNr )
					{
					// Ask Spanish ambiguous questions
					// Spanish test file: "Español ambigua - Ana es un padre - Luisa es un hijo"
					if( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
					// Question part doesn't exist yet
					generalizationWordItem->firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) == NULL &&
					askQuestions( userSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask questions" );
					}
				else
					{
					if( hasNonCompoundSecondarySpecificationCollection ||
					secondarySpecificationCollectionNr == userSpecificationCollectionNr ||

					( secondarySpecificationItem->isUserSpecification() &&
					secondarySpecificationWordItem->compoundCollectionNr() == secondarySpecificationCollectionNr ) )
						{
						if( relatedSpecificationItem->isOlderItem() &&
						relatedSpecificationItem->isSelfGeneratedAssumption() &&
						( conclusionSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, relatedSpecificationItem->specificationWordItem(), userSpecificationItem->generalizationWordItem() ) ) != NULL )
							relatedSpecificationItem = conclusionSpecificationItem;

						if( !isHiddenSpanishPrimarySpecification &&
						!primarySpecificationItem->hasRelationContext() &&
						// Negative specification doesn't exist
						generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, true, false, false, secondarySpecificationWordItem ) == NULL &&
						// Draw negative conclusion
						drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char drawCompoundSpecificationSubstitutionConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *selectedPrimarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasCurrentCompoundSpecificationCollection;
		bool hasCurrentNonCompoundSpecificationCollection;
		bool hasCurrentSpecificationCollection;
		bool hasCurrentSpecificationCurrentCreationSentenceNr;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		bool hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		bool hasPrimarySpecificationRelationContext;
		bool isAdjectiveCurrentSpecification;
		bool isExclusiveCurrentSpecification;
		bool isExclusivePrimarySpecification;
		bool isExclusiveUserSpecification = false;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isOlderPrimarySpecification;
		bool isPrimaryEveryGeneralization;
		bool isPrimaryUserAssignment;
		bool isPrimaryUserQuestion;
		bool isSelectedExclusiveSpecification;
		bool isSpanishCurrentLanguage;
		unsigned short currentSpecificationWordTypeNr;
		unsigned short selectedJustificationTypeNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int primarySpecificationCollectionNr;
		SpecificationItem *adjectiveSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *exclusiveSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *previousUserSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *selectedCurrentSpecificationItem;
		WordItem *compoundGeneralizationWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *previousUserSpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() ) != NULL )
			{
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();

			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem->hasFeminineOrMasculineProperNounEnding();

			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
			isExclusivePrimarySpecification = primarySpecificationItem->isExclusiveSpecification();
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			isPrimaryEveryGeneralization = primarySpecificationItem->isEveryGeneralization();
			isPrimaryUserAssignment = primarySpecificationItem->isUserAssignment();
			isPrimaryUserQuestion = primarySpecificationItem->isUserQuestion();
			primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr();

			compoundGeneralizationWordItem = specificationWordItem->compoundGeneralizationWordItem( primarySpecificationCollectionNr );

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem &&
				currentSpecificationWordItem != compoundGeneralizationWordItem )
					{
					hasCurrentCompoundSpecificationCollection = currentSpecificationItem->hasCompoundSpecificationCollection();
					hasCurrentNonCompoundSpecificationCollection = currentSpecificationItem->hasNonCompoundSpecificationCollection();
					isAdjectiveCurrentSpecification = currentSpecificationItem->isSpecificationAdjective();
					isExclusiveCurrentSpecification = currentSpecificationItem->isExclusiveSpecification();
					currentSpecificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();

					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > NO_COLLECTION_NR );
					isSelectedExclusiveSpecification = ( hasCurrentSpecificationCollection &&
														isExclusiveSpecification &&
														currentSpecificationWordTypeNr == WORD_TYPE_NOUN_SINGULAR );

					if( ( !isSpanishCurrentLanguage &&
					// Adjective specification word
					isAdjectiveCurrentSpecification &&
					// Proper noun specification
					isGeneralizationProperNoun &&
					primarySpecificationWordItem == specificationWordItem &&

					( isPrimaryUserAssignment ||
					isPrimaryUserQuestion ||

					( !hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation &&
					!hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
					!isExclusiveSpecification ) ) &&

					// Add justification to question
					( isExclusiveCurrentSpecification ||
					// Specification part doesn't exist yet as question
					generalizationWordItem->firstSpecificationItem( false, false, true, currentSpecificationWordItem ) == NULL ) &&

					// Specification doesn't exist yet
					generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, NULL ) == NULL ) ||

					// Non-exclusive definition specification
					( ( !isGeneralizationProperNoun &&

					( ( hasCurrentNonCompoundSpecificationCollection &&
					!isExclusiveCurrentSpecification ) ||

					// Exclusive compound definition specification
					( ( hasCurrentCompoundSpecificationCollection &&
					isExclusiveCurrentSpecification &&
					compoundGeneralizationWordItem != NULL &&
					currentSpecificationWordItem->isFeminineOrMasculineWord() ) ||

					// Older definition specification (without compound specification collection)
					( !hasCurrentCompoundSpecificationCollection &&

					( !isExclusiveCurrentSpecification ||
					isSelectedExclusiveSpecification ) &&

					currentSpecificationItem->isOlderItem() ) ) ) ) &&

					// User specification doesn't exist yet
					generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL ) )
						{
						selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;

						if( selectedPrimarySpecificationItem == NULL )
							selectedPrimarySpecificationItem = ( !isArchivedAssignment &&
																isPrimaryUserAssignment ? userSpecificationItem :
																							primarySpecificationItem );

						selectedCurrentSpecificationItem = ( isArchivedAssignment &&
															!hasPrimarySpecificationRelationContext ? userSpecificationItem :
																										currentSpecificationItem );

						if( isAdjectiveCurrentSpecification )
							{
							if( compoundGeneralizationWordItem == NULL )
								{
								if( isExclusiveCurrentSpecification )
									{
									// Create adjective question
									isSelectedExclusiveSpecification = true;
									questionParameter = WORD_PARAMETER_SINGULAR_VERB_IS;
									selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION;
									}
								else
									{
									if( isExclusivePrimarySpecification )
										{
										if( isOlderPrimarySpecification )
											// Test file: "John has 2 sons and a daughter (before family definition)"
											isSelectedExclusiveSpecification = true;

										// Definition
										if( !isOlderPrimarySpecification &&
										!isSpanishCurrentLanguage &&
										primarySpecificationCollectionNr > NO_COLLECTION_NR )
											// Test file: "Scientific challenge"
											isSelectedExclusiveSpecification = true;
										}
									}
								}
							else
								{
								if( !isGeneralizationProperNoun &&
								// Specification doesn't exist yet
								generalizationWordItem->firstSpecificationItem( false, false, false, currentSpecificationWordItem ) == NULL )
									// Create exclusive adjective specification
									isSelectedExclusiveSpecification = true;
								}
							}

						hasCurrentSpecificationCurrentCreationSentenceNr = currentSpecificationItem->hasCurrentCreationSentenceNr();

						if( !hasCurrentSpecificationCurrentCreationSentenceNr ||
						isSelectedExclusiveSpecification ||

						( !isExclusiveUserSpecification &&
						!isSpanishCurrentLanguage ) )
							{
							if( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, questionParameter, currentSpecificationWordItem ) ) != NULL &&
							!existingSpecificationItem->isExclusiveSpecification() &&
							isExclusiveUserSpecification &&
							isSelectedExclusiveSpecification )
								// Typical for Spanish
								// Spanish test file: "Reto científico"
								existingSpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem( currentSpecificationWordItem );

							// Specification doesn't exist yet
							if( existingSpecificationItem == NULL ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedCurrentSpecificationItem ) )
								{
								// Draw specification substitution conclusion
								if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isPrimaryEveryGeneralization, false, isSelectedExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, selectedCurrentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

								if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL )
									{
									if( ( isGeneralizationProperNoun ||
									!createdOrFoundSpecificationItem->isExclusiveSpecification() ) &&

									// Test file: "Scientific challenge"
									specificationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, currentSpecificationCollectionNr, NULL ) != NULL &&
									// Draw simple negative conclusions
									drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									if( !isGeneralizationProperNoun )
										{
										if( isSelectedExclusiveSpecification )
											{
											if( hasCurrentNonCompoundSpecificationCollection &&
											hasCurrentSpecificationCurrentCreationSentenceNr &&
											!isSpanishCurrentLanguage &&
											// Make generalization assumption definition
											adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, specificationWordTypeNr, generalizationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdOrFoundSpecificationItem, NULL, currentSpecificationItem, NULL, specificationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption definition about generalization word \"", specificationWordItem->anyWordTypeString(), "\" and specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											if( !hasCurrentNonCompoundSpecificationCollection &&
											selectedPrimarySpecificationItem->hasSpecificationCollection() &&
											( adjectiveSpecificationItem = userSpecificationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL &&
											( exclusiveSpecificationItem = currentSpecificationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL )
												{
												if( createdOrFoundSpecificationItem->isOlderItem() &&
												// Make user generalization assumption definition
												( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, userSpecificationItem->generalizationWordTypeNr(), exclusiveSpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, exclusiveSpecificationItem, NULL, adjectiveSpecificationItem, NULL, userSpecificationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a user generalization assumption definition about generalization word \"", userSpecificationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

												// Find previous user specification
												if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, userSpecificationItem ) ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

												if( !currentSpecificationItem->isReplacedItem() &&
												( previousUserSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
												( previousUserSpecificationWordItem = previousUserSpecificationItem->specificationWordItem() ) != NULL &&
												( adjectiveSpecificationItem = previousUserSpecificationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL &&
												( exclusiveSpecificationItem = currentSpecificationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL &&

												// Specification doesn't exist yet
												( ( existingSpecificationItem = previousUserSpecificationWordItem->firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) == NULL ||
												// Justification doesn't exist yet in specification
												!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( exclusiveSpecificationItem, adjectiveSpecificationItem ) ) )
													{
													// Make previous user generalization assumption definition
													if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, previousUserSpecificationItem->generalizationWordTypeNr(), exclusiveSpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, exclusiveSpecificationItem, NULL, adjectiveSpecificationItem, NULL, previousUserSpecificationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a previous user generalization assumption definition about generalization word \"", previousUserSpecificationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

													if( ( secondarySpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
													// Add specification substitution justification to created or found specification
													createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, previousUserSpecificationItem, NULL, secondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the created or found specification" );
													}
												}
											}
										}
									}
								}
							}
						}
					else
						{
						// Corrected assumption by opposite suggestive question
						if( hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&
						isAdjectiveCurrentSpecification &&
						// Get exclusive question part
						( selectedPrimarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, true, questionParameter, ( compoundGeneralizationWordItem == NULL ? specificationWordItem->compoundCollectionNr() : primarySpecificationItem->specificationCollectionNr() ), specificationWordItem ) ) != NULL )
							{
							// Ask adjective question
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask an adjective question about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL &&
							( obsoleteSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, false, false, currentSpecificationWordItem ) ) != NULL )
								{
								// Write invalid assumption by opposite suggestive question (obsolete)
								if( generalizationWordItem->writeUpdatedSpecification( false, false, true, false, false, true, false, obsoleteSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid assumption by opposite suggestive question (obsolete) in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								// Replace obsolete adjective assumption by created question
								if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the obsolete assumption specification by the created question in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem->isReplacedOrDeletedItem() ?
												// First specification
												specificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() :
												// Next specification
												currentSpecificationItem->nextNonNegativeNonPossessiveDefinitionSpecificationItem() ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawExclusiveNegativeUserConclusions( bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		SpecificationItem *previousUserSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawExclusiveNegativeUserConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( currentSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current specification word item is undefined" );

		if( previousSpecificationWordItem != NULL &&
		( previousUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, previousSpecificationWordItem ) ) != NULL )
			{
			// Negative specification doesn't exist yet
			if( previousSpecificationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) == NULL &&
			// Draw negative conclusion
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, previousUserSpecificationItem, NULL, NULL, NULL, previousSpecificationWordItem, currentSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative user conclusion about generalization word \"", previousSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

			// Negative specification doesn't exist yet
			if( currentSpecificationWordItem->firstSpecificationItem( true, false, false, previousSpecificationWordItem ) == NULL &&
			// Draw another negative conclusion
			adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, currentSpecificationWordItem, previousSpecificationWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with specification word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawNegativeUserDefinitionConclusions( bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *adjectiveSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawNegativeUserDefinitionConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( adjectiveSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given adjective specification word item is undefined" );

		if( currentSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current specification word item is undefined" );

		// Negative specification doesn't exist yet
		if( generalizationWordItem->firstSpecificationItem( true, false, false, adjectiveSpecificationWordItem ) == NULL )
			{
			// Draw adjective negative conclusion
			if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_ADJECTIVE, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, NULL, NULL, generalizationWordItem, adjectiveSpecificationWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", adjectiveSpecificationWordItem->anyWordTypeString(), "\"" );

			if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
			// Get first generalization specification noun word
			( currentGeneralizationItem = adjectiveSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL &&
					currentGeneralizationWordItem != generalizationWordItem )
						{
						// Negative definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != NULL &&
						// Draw negative generalization conclusion (negative definition)
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative generalization conclusion with negative definition about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

						// Positive definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem->firstNonExclusiveSpecificationItem( false, false, false, adjectiveSpecificationWordItem ) ) != NULL )
							{
							// Negative conclusion doesn't exist yet
							if( generalizationWordItem->firstNonExclusiveSpecificationItem( true, false, false, currentGeneralizationWordItem ) == NULL &&
							// Draw negative generalization conclusion (positive definition)
							adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, definitionSpecificationItem, NULL, createdOrFoundSpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative generalization conclusion with positive definition about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

							// Draw negative generalization conclusion (reversed, positive definition)
							if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, definitionSpecificationItem, NULL, createdOrFoundSpecificationItem, NULL, currentGeneralizationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversed negative generalization conclusion with positive definition about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char drawOnlyOptionLeftNounConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem )
		{
		bool hasFoundAssignment;
		bool hasMoreOptionsLeft = false;
		SpecificationItem *currentAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstNegativeOnlyOptionLeftAssignmentItem = NULL;
		SpecificationItem *firstOnlyOptionLeftSpecificationItem = NULL;
		SpecificationItem *foundOnlyOptionLeftSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawOnlyOptionLeftNounConclusion";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationCollectionNr <= NO_COLLECTION_NR )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification collection number is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem->firstNonQuestionSpecificationItem() ) != NULL )
			{
			firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

			do	{
				if( currentSpecificationItem->isExclusiveSpecification() &&
				currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
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
			adminItem_->addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, false, true, false, false, foundOnlyOptionLeftSpecificationItem->isPossessive(), false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem->generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem->specificationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, NULL, firstNegativeOnlyOptionLeftAssignmentItem, NULL, generalizationWordItem, foundOnlyOptionLeftSpecificationItem->specificationWordItem(), NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char drawProperNounPartOfConclusionsInProperNounWords( bool isArchivedAssignment, WordItem *specificationWordItem, WordItem *spanishRelationWordItem )
		{
		bool hasUserSpecificationCollection;
		bool hasUserSpecificationRelationContext;
		bool isPossessiveUserSpecification;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserSpecificationWordSpanishAmbiguous;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *currentProperNounWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawProperNounPartOfConclusionsInProperNounWords";

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the user specification item" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		hasUserSpecificationCollection = userSpecificationItem->hasSpecificationCollection();
		hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
		isPossessiveUserSpecification = userSpecificationItem->isPossessive();
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();

		if( ( currentProperNounWordItem = globalVariables_->firstUserDefinedProperNounWordItem ) != NULL )
			{
			// Do for all user-defined proper noun words
			do	{
				if( currentProperNounWordItem != spanishRelationWordItem )
					{
					if( ( primarySpecificationItem = currentProperNounWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL ) ) == NULL ||
					isSpecificationWordSpanishAmbiguous )
						{
						primarySpecificationItem = currentProperNounWordItem->firstAssignmentOrSpecificationItem( false, true, false, false, false, specificationWordItem );

						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&

						( primarySpecificationItem == NULL ||
						primarySpecificationItem->isHiddenSpanishSpecification() ) )
							{
							if( spanishRelationWordItem == NULL )
								{
								// Try to find self-generated question, as alternative to hidden specification
								if( ( tempSpecificationItem = currentProperNounWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, true, specificationWordItem ) ) != NULL &&
								tempSpecificationItem->isExclusiveSpecification() )
									primarySpecificationItem = tempSpecificationItem;
								}
							else
								{
								// Typical for Spanish
								if( isPossessiveUserSpecification &&
								!isUserSpecificationWordSpanishAmbiguous &&
								currentProperNounWordItem->hasContextInWord( userSpecificationItem->relationContextNr() ) )
									primarySpecificationItem = userSpecificationItem;
								else
									primarySpecificationItem = spanishRelationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, true, specificationWordItem, currentProperNounWordItem );
								}

							if( primarySpecificationItem != NULL &&
							primarySpecificationItem->isHiddenSpanishSpecification() )
								primarySpecificationItem = NULL;
							}
						}
					else
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage &&
						primarySpecificationItem->isUserSpecification() &&
						( tempSpecificationItem = currentProperNounWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL )
							primarySpecificationItem = tempSpecificationItem;
						}

					if( primarySpecificationItem != NULL &&

					( !hasUserSpecificationCollection ||
					!hasUserSpecificationRelationContext ||
					isUserSpecificationWordSpanishAmbiguous ||
					spanishRelationWordItem != NULL ||
					primarySpecificationItem == userSpecificationItem ||
					primarySpecificationItem->hasCurrentCreationSentenceNr() ) &&

					// Avoid idle call
					globalVariables_->firstPossessiveNounWordItem != NULL &&
					// Draw proper noun part-of conclusions
					drawProperNounPartOfConclusions( isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, currentProperNounWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions in proper noun word \"", currentProperNounWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentProperNounWordItem = currentProperNounWordItem->nextUserDefinedProperNounWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawReversibleConclusions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawReversibleConclusions";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem->isProperNounWord() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item isn't a proper noun" );

		if( generalizationWordItem->firstNonQuestionSpecificationItem( true, true, false ) != NULL )
			{
			// Active assignments
			if( drawReversibleConclusions( true, false, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with active assignments" );
			}
		else
			{
			if( generalizationWordItem->firstNonQuestionSpecificationItem( true, true, true ) != NULL &&
			// Archived assignments
			drawReversibleConclusions( true, true, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with archived assignments" );
			}

		// Avoid idle call
		if( generalizationWordItem->firstNonQuestionSpecificationItem( true, false, false ) != NULL &&
		// Specifications
		drawReversibleConclusions( false, false, userSpecificationItem, generalizationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions with specifications" );

		return RESULT_OK;
		}

	signed char drawSimpleNegativeConclusions( bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge;
		bool hasUserSpecificationCurrentCreationSentenceNr;
		bool hasUserSpecificationRelationContext;
		bool isDrawingCompoundNegativeConclusions;
		bool isDrawingNegativeUserConclusion;
		bool isEveryGeneralization;
		bool isOlderUserSpecification;
		bool isPossessiveUserSpecification;
		bool isProperNounArchivedAssignment;
		bool isSelfGeneratedPrimarySpecification;
		bool isUserAssignment;
		bool isUserRelationWord;
		unsigned short generalizationWordTypeNr;
		unsigned short negativeSpecificationWordTypeNr;
		unsigned short userSpecificationWordTypeNr;
		unsigned int compoundCollectionNr = NO_COLLECTION_NR;
		unsigned int userSpecificationCollectionNr;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *negativeSpecificationItem;
		SpecificationItem *negativeDefinitionSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *positiveSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *properNounSpecificationItem;
		SpecificationItem *simpleUserSpecificationItem;
		WordItem *collectionWordItem = NULL;
		WordItem *currentGeneralizationWordItem;
		WordItem *negativeSpecificationWordItem;
		WordItem *reversedNegativeSpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSimpleNegativeConclusions";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( negativeDefinitionSpecificationItem = specificationWordItem->firstNegativeSpecificationItem() ) != NULL &&
		( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != NULL &&
		!primarySpecificationItem->isHiddenSpanishSpecification() )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem->hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge();
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			isEveryGeneralization = primarySpecificationItem->isEveryGeneralization();
			generalizationWordTypeNr = primarySpecificationItem->generalizationWordTypeNr();
			isSelfGeneratedPrimarySpecification = primarySpecificationItem->isSelfGeneratedSpecification();

			isUserAssignment = userSpecificationItem->isAssignment();

			if( ( simpleUserSpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL )
				userSpecificationItem = simpleUserSpecificationItem;

			hasUserSpecificationCurrentCreationSentenceNr = userSpecificationItem->hasCurrentCreationSentenceNr();
			hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
			isOlderUserSpecification = userSpecificationItem->isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem->isPossessive();
			userSpecificationWordTypeNr = userSpecificationItem->specificationWordTypeNr();
			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();

			isDrawingNegativeUserConclusion = ( !isOlderUserSpecification &&
												userSpecificationItem->isGeneralizationProperNoun() &&
												// If the user specification is possessive, the user generalization word must be opposite, and otherwise
												userSpecificationItem->isPossessive() == isUserRelationWord &&
												!userSpecificationItem->isQuestion() );

			isDrawingCompoundNegativeConclusions = ( isDrawingNegativeUserConclusion &&
													( compoundCollectionNr = userSpecificationWordItem->compoundCollectionNr() ) > NO_COLLECTION_NR &&
													( collectionWordItem = userSpecificationWordItem->collectionWordItem( compoundCollectionNr ) ) != NULL );

			do	{
				if( ( negativeSpecificationWordItem = negativeDefinitionSpecificationItem->specificationWordItem() ) != NULL &&

				( !primarySpecificationItem->isExclusiveSpecification() ||
				negativeDefinitionSpecificationItem->isSpecificationAdjective() ) &&

				// Positive specification doesn't exist
				( generalizationWordItem->firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) == NULL ||
				hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge ||
				negativeSpecificationWordItem->isNounWordSpanishAmbiguous() ) )
					{
					negativeSpecificationWordTypeNr = negativeDefinitionSpecificationItem->specificationWordTypeNr();

					if( isDrawingNegativeUserConclusion &&

					( isPossessiveUserSpecification ||
					// No answered question with this specification word
					generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, false, true, false, false, true, userSpecificationCollectionNr, NO_CONTEXT_NR, negativeSpecificationWordItem ) == NULL ) )
						{
						negativeSpecificationItem = ( userSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE ? negativeSpecificationWordItem->firstSpecificationItem( true, false, false, userSpecificationWordItem ) :
																														userSpecificationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) );

						if( negativeSpecificationItem != NULL &&
							
						// Negative specification doesn't exist
						( ( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == NULL &&

						( negativeSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE ||
						userSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ||

						// Negative specification exists
						( existingSpecificationItem != NULL &&

						( isUserAssignment ||
						negativeSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE ||

						( !hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation &&

						( !hasUserSpecificationRelationContext ||

						( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
						existingSpecificationItem->hasPrimaryNounSpecificationJustification() ) ) ) ) &&

						// Justification doesn't exist yet in specification
						!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, negativeSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, negativeSpecificationItem, NULL ) ) ) &&

						// Draw negative user conclusion
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, negativeSpecificationItem, NULL, generalizationWordItem, negativeSpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );
						}

					obsoleteSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, NO_QUESTION_PARAMETER, negativeDefinitionSpecificationItem->specificationCollectionNr(), NULL );

					if( ( hasUserSpecificationCurrentCreationSentenceNr ||
					isUserRelationWord ) &&

					// Negative specification doesn't exist
					( ( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == NULL &&

					( !isDrawingNegativeUserConclusion ||
					isSelfGeneratedPrimarySpecification ||
					obsoleteSpecificationItem != NULL ) ) ||

					// Negative specification exists
					( existingSpecificationItem != NULL &&

					( !isDrawingNegativeUserConclusion ||
					!existingSpecificationItem->isOlderItem() ||
					existingSpecificationItem->hasPrimaryNounSpecificationJustification() ) &&

					// Justification doesn't exist yet in specification
					!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, negativeDefinitionSpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, negativeDefinitionSpecificationItem, NULL ) ) ) )
						{
						// Draw simple negative conclusion
						if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, negativeDefinitionSpecificationItem, NULL, generalizationWordItem, negativeSpecificationWordItem, NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );

						if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL )
							{
							if( obsoleteSpecificationItem != NULL &&
							createdOrFoundSpecificationItem->isCorrectedSpecification() )
								{
								// Correcting invalidated assumption by knowledge
								if( correctSuggestiveAssumptionsByKnowledge( createdOrFoundSpecificationItem, userSpecificationItem, generalizationWordItem, negativeSpecificationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct invalid assumptions by knowledge about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								// Definition specification
								if( isEveryGeneralization )
									{
									if( negativeSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
									// Negative definition doesn't exist yet
									negativeSpecificationWordItem->firstSpecificationItem( true, false, false, generalizationWordItem ) == NULL &&
									// Draw negative definition conclusions
									drawNegativeDefinitionConclusions( isArchivedAssignment, generalizationWordTypeNr, negativeSpecificationWordTypeNr, generalizationWordItem, negativeSpecificationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative definition conclusions" );

									if( ( definitionSpecificationItem = negativeSpecificationWordItem->firstSpecificationItem( true, false, false, generalizationWordItem ) ) == NULL )
										definitionSpecificationItem = createdOrFoundSpecificationItem;

									// Get first generalization specification proper noun word
									if( ( currentGeneralizationItem = generalizationWordItem->firstProperNounSpecificationGeneralizationItem() ) != NULL )
										{
										// Do for all generalization specification proper noun words
										do	{
											if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
												return adminItem_->addError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

											if( ( positiveSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, negativeSpecificationWordItem ) ) != NULL &&

											( ( existingSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationWordItem ) ) == NULL ||

											// Justification doesn't exist yet in specification
											( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
											// Justification has at least the same assumption level
											existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL ) ) ) )
												{
												isProperNounArchivedAssignment = positiveSpecificationItem->isArchivedAssignment();

												// Draw negative conclusion
												if( adminItem_->addSelfGeneratedSpecification( false, isProperNounArchivedAssignment, isProperNounArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, generalizationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL, currentGeneralizationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										while( ( currentGeneralizationItem = currentGeneralizationItem->nextProperNounSpecificationGeneralizationItem() ) != NULL );
										}

									// Get first generalization specification noun word
									if( ( currentGeneralizationItem = generalizationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
										{
										// Do for all generalization specification noun words
										do	{
											if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
												return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

											if( ( positiveSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != NULL &&
											!positiveSpecificationItem->isExclusiveSpecification() &&

											// Specification doesn't exist yet
											( ( existingSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, generalizationWordItem ) ) == NULL ||

											// Justification doesn't exist yet in specification
											( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
											// Justification has at least the same assumption level
											existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL ) ) ) &&

											// Draw negative adjective conclusion
											adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveSpecificationItem, NULL, definitionSpecificationItem, NULL, currentGeneralizationWordItem, generalizationWordItem, NULL ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with specification word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( ( reversedNegativeSpecificationWordItem = negativeSpecificationWordItem->collectionWordItem( definitionSpecificationItem->specificationCollectionNr() ) ) != NULL &&
											( negativeSpecificationItem = currentGeneralizationWordItem->firstSpecificationItem( true, false, false, reversedNegativeSpecificationWordItem ) ) != NULL &&
											( positiveSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, reversedNegativeSpecificationWordItem ) ) != NULL &&

											// Specification doesn't exist yet
											( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == NULL ||

											// Justification doesn't exist yet in specification
											( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, negativeSpecificationItem) &&
											// Justification has at least the same assumption level
											existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, NULL, negativeSpecificationItem, NULL ) ) ) &&

											// Draw negative adjective conclusion
											adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, positiveSpecificationItem, NULL, negativeSpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
											}
										while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
										}
									}
								else	// Proper noun specification
									{
									if( isUserRelationWord &&
									existingSpecificationItem != NULL &&
									( properNounSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, userSpecificationWordItem ) ) != NULL &&
									!properNounSpecificationItem->isHiddenSpanishSpecification() &&
									// Another reverse negative definition
									( definitionSpecificationItem = userSpecificationWordItem->firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) != NULL &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( properNounSpecificationItem, definitionSpecificationItem ) &&
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, properNounSpecificationItem, NULL, definitionSpecificationItem, NULL ) &&
									// Draw another reverse simple negative conclusion
									adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, properNounSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, negativeSpecificationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw another reverse simple negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								}

							// Test file: "Knowledge of organisms (1)"
							if( isDrawingNegativeUserConclusion &&
							userSpecificationCollectionNr != compoundCollectionNr &&
							!primarySpecificationItem->hasAssumptionLevel() &&
							// Avoid idle call
							negativeSpecificationWordItem->firstExclusiveSpecificationItem( true ) != NULL &&
							// Draw negative exclusive conclusions
							drawNegativeExclusiveConclusions( isArchivedAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, negativeSpecificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative exclusive conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with negative specification word \"", negativeSpecificationWordItem->anyWordTypeString(), "\"" );
							}

						if( isDrawingCompoundNegativeConclusions &&
						collectionWordItem != NULL &&
						// Positive specification doesn't exist
						generalizationWordItem->firstSpecificationItem( false, false, false, collectionWordItem ) == NULL &&
						( definitionSpecificationItem = userSpecificationWordItem->firstSpecificationItem( true, false, false, collectionWordItem ) ) != NULL &&

						// Specification doesn't exist yet
						( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, collectionWordItem ) ) == NULL ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, definitionSpecificationItem, NULL ) ) ) &&

						// Draw compound negative conclusion
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, userSpecificationItem, NULL, definitionSpecificationItem, NULL, generalizationWordItem, collectionWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a compound negative conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( negativeDefinitionSpecificationItem = negativeDefinitionSpecificationItem->nextNegativeSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned short assumptionLevel;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *currentSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *primaryGeneralizationWordItem;
		WordItem *primarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != NULL )
					{
					assumptionLevel = ( primarySpecificationItem->assumptionLevel() < currentSpecificationItem->assumptionLevel() ? currentSpecificationItem->assumptionLevel() :
																																		primarySpecificationItem->assumptionLevel() );

					// Collect generalization words of a specification-generalization definition
					if( adminItem_->collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

					currentSpecificationItem = currentSpecificationItem->updatedSpecificationItem();

					if( currentSpecificationItem->hasCurrentCreationSentenceNr() &&
					// Specification-generalization doesn't exist yet

					( ( foundSpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, currentGeneralizationWordItem ) ) == NULL ||
					foundSpecificationItem->isUserSpecification() ) &&

					// Draw a specification-generalization conclusion (using current specification)
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, true, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, currentSpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, currentSpecificationItem, NULL, NULL, NULL, specificationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the current specification" );

					if( !isSelfGenerated &&
					currentSpecificationItem->hasGeneralizationCollection() &&
					( primaryGeneralizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					// Specification-generalization doesn't exist yet
					primarySpecificationWordItem->firstSpecificationItem( false, false, false, primaryGeneralizationWordItem ) == NULL &&
					// Draw a specification-generalization conclusion (using primary specification)
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, true, false, assumptionLevel, JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_PLURAL, primarySpecificationItem->generalizationWordTypeNr(), NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, NULL, NULL, primarySpecificationWordItem, primaryGeneralizationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the primary specification" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char drawSpecificationSubstitutionConclusionOrAskQuestion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCurrentlyCorrectedAssumptionByKnowledge;
		bool hasCurrentSpecificationCollection;
		bool hasCurrentSpecificationCompoundCollection;
		bool hasGeneralizationWordActiveQuestionWithCompoundCollection;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool hasGeneralizationWordMasculineProperNounEnding;
		bool hasPrimarySpecificationNewInformation;
		bool hasPrimarySpecificationNonCompoundCollection;
		bool hasPrimarySpecificationRelationContext;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasUserSpecificationCompoundCollection = false;
		bool hasUserSpecificationNonCompoundCollection = false;
		bool hasUserSpecificationRelationContext = false;
		bool isAdjectiveCurrentSpecification;
		bool isArchivedUserAssignment = false;
		bool isCurrentUserSpecification;
		bool isDefinitionSpanishAmbiguous;
		bool isExclusiveCurrentSpecification;
		bool isExclusiveUserSpecification = false;
		bool isFeminineOrMasculineGeneralizationWord;
		bool isFoundPrimaryHiddenSpanishSpecification;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isOlderCurrentSpecification;
		bool isOlderUserSpecification = false;
		bool isPossessiveUserSpecification = false;
		bool isPossessivePrimarySpecification;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isSpanishNegativeConclusion = false;
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserAssignment = false;
		bool isUserAdjectiveSpecification = false;
		bool isUserDefinitionSpecification = false;
		bool isUserGeneralizationWord;
		bool isUserQuestion = false;
		bool isUserRelationWord;
		bool isUserSpecificationWordSpanishAmbiguous = false;
		unsigned short currentSpecificationWordTypeNr;
		unsigned short selectedRelationWordTypeNr;
		unsigned int compoundSpecificationCollectionNr;
		unsigned int currentSpecificationCollectionNr;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *tempJustificationItem;
		SpecificationItem *alternativePrimarySpecificationItem;
		SpecificationItem *alternativeSpanishAdditionalDefinitionSpecificationItem;
		SpecificationItem *archivedAssignmentItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *feminineOrMasculineSpecificationItem;
		SpecificationItem *foundPrimarySpecificationItem;
		SpecificationItem *obsoleteSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *questionSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *tempSpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *collectionWordItem;
		WordItem *currentSpecificationWordItem;
		WordItem *oppositeSpecificationWordItem;
		WordItem *selectedRelationWordItem;
		WordItem *userSpecificationWordItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( !adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_->isSpecificationReasoningWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a specification reasoning type" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() ) != NULL )
			{
			hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge();
			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem->hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem->hasMasculineProperNounEnding();
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem->isFeminineOrMasculineWord();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem->isUserRelationWord;

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
			compoundSpecificationCollectionNr = specificationWordItem->compoundCollectionNr();

			if( ( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL )
				{
				hasUserSpecificationCompoundCollection = userSpecificationItem->hasCompoundSpecificationCollection();
				hasUserSpecificationNonCompoundCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
				hasUserSpecificationRelationContext = userSpecificationItem->hasRelationContext();
				isArchivedUserAssignment = userSpecificationItem->isArchivedAssignment();
				isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();
				isOlderUserSpecification = userSpecificationItem->isOlderItem();
				isPossessiveUserSpecification = userSpecificationItem->isPossessive();
				isUserAssignment = userSpecificationItem->isAssignment();
				isUserAdjectiveSpecification = userSpecificationItem->isSpecificationAdjective();
				isUserDefinitionSpecification = userSpecificationItem->isGeneralizationNoun();
				isUserQuestion = userSpecificationItem->isQuestion();
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem->isSpecificationWordSpanishAmbiguous();
				userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
				userSpecificationWordItem = userSpecificationItem->specificationWordItem();
				}

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				currentSpecificationWordItem != generalizationWordItem )
					{
					hasCurrentSpecificationCompoundCollection = currentSpecificationItem->hasCompoundSpecificationCollection();
					isCurrentUserSpecification = currentSpecificationItem->isUserSpecification();
					isAdjectiveCurrentSpecification = currentSpecificationItem->isSpecificationAdjective();
					isExclusiveCurrentSpecification = currentSpecificationItem->isExclusiveSpecification();
					isOlderCurrentSpecification = currentSpecificationItem->isOlderItem();
					currentSpecificationWordTypeNr = currentSpecificationItem->specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();
					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > NO_COLLECTION_NR );

					if( !hasRelationWord ||
					!isPossessiveUserSpecification ||
					( foundPrimarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, specificationWordItem, relationWordItem ) ) == NULL )
						foundPrimarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, isQuestion, specificationWordItem );

					if( foundPrimarySpecificationItem == NULL )
						{
						// Check for specification conflict
						if( !hasUserSpecificationRelationContext &&
						isCurrentUserSpecification &&
						currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
						!currentSpecificationItem->isSpecificationWordSpanishAmbiguous() &&
						// Check for specification conflict
						generalizationWordItem->checkForSpecificationConflict( isArchivedAssignment, false, false, isSpecificationWordSpanishAmbiguous, currentSpecificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem->relationContextNr(), currentSpecificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a specification conflict in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					else
						{
						hasPrimarySpecificationNewInformation = foundPrimarySpecificationItem->hasNewInformation();
						hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem->hasNonCompoundSpecificationCollection();
						isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem->isHiddenSpanishSpecification();

						// Specification doesn't exist yet
						if( ( existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) ) == NULL )
							existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, NO_COLLECTION_NR, NO_CONTEXT_NR, currentSpecificationWordItem );

						if( existingSpecificationItem != NULL &&
						!isQuestion &&
						!existingSpecificationItem->isExclusiveSpecification() )
							isSpanishNegativeConclusion = true;

						if( isExclusiveCurrentSpecification &&
						!isFeminineOrMasculineGeneralizationWord &&
						userSpecificationItem != NULL &&
						userSpecificationWordItem != NULL &&

						// Matching feminine word
						( ( hasGeneralizationWordFeminineProperNounEnding &&
						currentSpecificationWordItem->isFeminineWord() ) ||

						// Matching masculine word
						( hasGeneralizationWordMasculineProperNounEnding &&
						currentSpecificationWordItem->isMasculineWord() ) ) &&

						( feminineOrMasculineSpecificationItem = currentSpecificationWordItem->firstFeminineOrMasculineSpecificationItem() ) != NULL &&
						// Select correct compound word to avoid assumption to be made if a question should be asked
						currentSpecificationWordItem->compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem )
							{
							selectedRelationWordItem = ( feminineOrMasculineSpecificationItem->isSpecificationAdjective() ? NULL : relationWordItem );

							if( existingSpecificationItem == NULL ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( foundPrimarySpecificationItem, currentSpecificationItem ) ||

							( hasCurrentSpecificationCompoundCollection &&
							selectedRelationWordItem != NULL &&
							!selectedRelationWordItem->hasContextInWord( existingSpecificationItem->relationContextNr() ) ) )
								{
								selectedPrimarySpecificationItem = foundPrimarySpecificationItem;
								selectedAdditionalDefinitionSpecificationItem = NULL;

								if( isFoundPrimaryHiddenSpanishSpecification )
									{
									selectedPrimarySpecificationItem = userSpecificationItem;
									selectedAdditionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationItem_;
									}

								// User generalization word
								if( ( isUserGeneralizationWord &&
								hasPrimarySpecificationNewInformation &&

								( ( !isFoundPrimaryHiddenSpanishSpecification &&

								// Non-compound specification collection
								( ( hasPrimarySpecificationNonCompoundCollection &&

								( hasCurrentSpecificationCompoundCollection &&

								( !hasGeneralizationWordActiveQuestionWithCompoundCollection ||

								// Non-possessive user specification
								( !isPossessiveUserSpecification &&
								foundPrimarySpecificationItem->hasRelationContext() ) ) ) ) ||

								// No non-compound specification collection
								( !hasPrimarySpecificationNonCompoundCollection &&
								!hasUserSpecificationRelationContext &&
								currentSpecificationWordItem->collectionNrByCompoundGeneralizationWordInWord( true, WORD_TYPE_NOUN_SINGULAR, userSpecificationWordItem ) == NO_COLLECTION_NR ) ) ) ||

								// Typical for Spanish
								( isFoundPrimaryHiddenSpanishSpecification &&
								// Justification doesn't exist yet in specification
								generalizationWordItem->foundJustificationItem( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem ) == NULL ) ) ) ||

								// Not user generalization word
								( !isUserGeneralizationWord &&
								// Non-compound specification collection
								hasPrimarySpecificationNonCompoundCollection &&
								!isFoundPrimaryHiddenSpanishSpecification &&
								!isSpecificationWordSpanishAmbiguous &&

								( existingSpecificationItem == NULL ||

								( isUserAssignment &&
								existingSpecificationItem->hasSpecificationCollection() &&
								!existingSpecificationItem->hasCurrentCreationSentenceNr() ) ||

								( existingSpecificationItem->hasSpecificationCollection() &&

								( isPossessiveUserSpecification ||
								// Justification doesn't exist yet in specification
								!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, currentSpecificationItem ) ) &&

								( ( hasRelationWord &&
								existingSpecificationItem->hasRelationContext() &&
								// Relation word is not covered by justification
								existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem ) == NULL ) ||

								// Justification has at least the same assumption level
								existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, NULL ) ) ) ) &&

								( !isPossessiveUserSpecification ||

								( ( existingSpecificationItem != NULL &&

								( !hasCurrentSpecificationCompoundCollection ||
								existingSpecificationItem->hasCurrentCreationSentenceNr() ) ) ||

								( selectedRelationWordItem != NULL &&

								( !selectedRelationWordItem->hasCurrentlyConfirmedSpecification() ||
								// Justification doesn't exist yet
								generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, currentSpecificationItem ) == NULL ) ) ) ) ) )
									{
									selectedRelationWordTypeNr = ( selectedRelationWordItem == NULL ? NO_WORD_TYPE_NR : WORD_TYPE_PROPER_NOUN );

									// Make a feminine or masculine proper noun ending assumption
									if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem->prepositionParameter(), NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

									if( !isUserSpecificationWordSpanishAmbiguous &&
									createAndAssignResult.createdSpecificationItem != NULL &&
									// Draw a specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}

						if( userSpecificationItem != NULL &&

						( ( existingSpecificationItem == NULL &&

						( hasCurrentSpecificationCollection ||
						!foundPrimarySpecificationItem->hasSpecificationCollection() ) ) ||

						( !isExclusiveCurrentSpecification &&

						( isGeneralizationProperNoun ||

						( hasCurrentSpecificationCollection &&
						isDefinitionSpanishAmbiguous ) ) ) ||

						( isDefinitionSpanishAmbiguous &&
						hasCurrentSpecificationCollection &&
						!isOlderCurrentSpecification ) ) )
							{
							if( ( !isExclusiveSpecification &&
							isExclusiveCurrentSpecification ) ||

							( isGeneralizationProperNoun &&

							( ( isExclusiveSpecification &&
							hasCurrentSpecificationCollection &&
							isOlderCurrentSpecification ) ||

							( isSpecificationWordSpanishAmbiguous &&
							!isFoundPrimaryHiddenSpanishSpecification &&

							( isExclusiveSpecification &&

							( isUserRelationWord ||
							isPossessiveUserSpecification ) ) ) ) ) )
								{
								// Ask a question or draw a negative conclusion
								if( isGeneralizationProperNoun &&
								hasCurrentSpecificationCollection &&
								userSpecificationWordItem != NULL &&

								// Question
								( hasSuggestiveQuestionAssumption_ ||

								// Specification isn't Spanish ambiguous
								( !isSpecificationWordSpanishAmbiguous &&

								( !hasGeneralizationWordActiveQuestionWithCompoundCollection ||
								isExclusiveUserSpecification ||
								isUserSpecificationWordSpanishAmbiguous ||

								// Skip invalid questions
								( ( !hasGeneralizationWordFeminineProperNounEnding &&
								!hasGeneralizationWordMasculineProperNounEnding ) ||

								!foundPrimarySpecificationItem->isSelfGeneratedQuestion() ) ) &&

								( isAdjectiveCurrentSpecification ||
								isSpanishCurrentLanguage ||

								( isUserGeneralizationWord &&

								( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
								!hasUserSpecificationRelationContext ) &&

								// Negative specification doesn't exist
								userSpecificationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) == NULL &&

								// Question part doesn't exist yet
								( ( questionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, true, currentSpecificationWordItem ) ) == NULL ||

								// Question to be adjusted
								( questionSpecificationItem->isOlderItem() &&

								( hasGeneralizationWordCurrentlyConfirmedSpecification ||
								// Justification doesn't exist yet in question
								!questionSpecificationItem->hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ) ) ) ||

								( foundPrimarySpecificationItem->isSelfGeneratedSpecification() &&
								!foundPrimarySpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
								// Question part doesn't exist yet
								generalizationWordItem->firstSpecificationItem( false, false, true, currentSpecificationWordItem ) == NULL ) ) ) ||

								// Specification is Spanish ambiguous
								( isSpecificationWordSpanishAmbiguous &&

								( hasPrimarySpecificationNonCompoundCollection ||
								!isExclusiveCurrentSpecification ||
								isSpanishNegativeConclusion ||
								isUserGeneralizationWord ) &&

								( !foundPrimarySpecificationItem->isHiddenSpanishSpecification() ||

								( isUserRelationWord &&
								userSpecificationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) != NULL ) ) ) ) &&

								// Ask specification substitution question, or draw negative conclusion
								askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, ( currentSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE && specificationWordItem->isUserSpecificationWord ? userSpecificationItem : foundPrimarySpecificationItem ), currentSpecificationItem, userSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								// Make an assumption or draw a conclusion
								if( isGeneralizationProperNoun &&
								currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&

								( existingSpecificationItem != NULL ||
								!foundPrimarySpecificationItem->isCorrectedSpecification() ) )
									{
									alternativeSpanishAdditionalDefinitionSpecificationItem = NULL;
									primarySpecificationItem = NULL;

									if( isSpecificationWordSpanishAmbiguous )
										{
										if( currentSpecificationWordItem != specificationWordItem )
											primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );
										}
									else
										{
										if( ( primarySpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, NULL ) ) == NULL ||
										primarySpecificationItem->isUserAssignment() )
											primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );
										}

									if( primarySpecificationItem != NULL )
										{
										alternativePrimarySpecificationItem = ( !isSpecificationWordSpanishAmbiguous ?
																				( hasRelationWord ? userSpecificationItem :
																									foundPrimarySpecificationItem ) :
																									alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) );

										if( ( existingSpecificationItem != NULL &&
										alternativePrimarySpecificationItem != NULL &&
										alternativePrimarySpecificationItem->specificationWordItem() == specificationWordItem &&

										( ( !isSpecificationWordSpanishAmbiguous &&
										!existingSpecificationItem->hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) ) ||

										// Typical for Spanish
										( isSpecificationWordSpanishAmbiguous &&

										( existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ||

										( isPossessiveUserSpecification &&
										primarySpecificationItem->hasRelationContext() &&
										generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) != NULL ) ) ) ) ) ||

										// Typical for Spanish
										( isSpecificationWordSpanishAmbiguous &&
										primarySpecificationItem->isHiddenSpanishSpecification() ) )
											{
											if( isSpecificationWordSpanishAmbiguous &&
											alternativePrimarySpecificationItem != NULL )
												alternativeSpanishAdditionalDefinitionSpecificationItem = alternativeSpanishAdditionalDefinitionSpecificationItem_;

											// Typical for Spanish
											if( isSpecificationWordSpanishAmbiguous &&
											!isArchivedUserAssignment &&
											isUserRelationWord &&
											alternativeSpanishAdditionalDefinitionSpecificationItem != NULL &&
											existingSpecificationItem != NULL )
												{
												if( ( primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, NO_CONTEXT_NR, userSpecificationWordItem ) ) != NULL &&
												// Check if justification already exists
												existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) )
													{
													if( primarySpecificationItem == alternativePrimarySpecificationItem )
														{
														if( ( tempJustificationItem = foundPrimarySpecificationItem->firstSecondaryJustificationItem( true ) ) != NULL )
															{
															primarySpecificationItem = tempJustificationItem->secondarySpecificationItem();

															if( existingSpecificationItem->isOlderItem() &&
															// Justification already exists yet in specification. Try to find a non-compound specification
															existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) &&
															( nonCompoundSpecificationCollectionNr = alternativeSpanishAdditionalDefinitionSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR &&
															( oppositeSpecificationWordItem = alternativeSpanishAdditionalDefinitionSpecificationItem->specificationWordItem() ) != NULL &&
															( tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, oppositeSpecificationWordItem ) ) != NULL &&
															!tempSpecificationItem->isHiddenSpanishSpecification() )
																primarySpecificationItem = tempSpecificationItem;
															}
														}
													else
														primarySpecificationItem = alternativePrimarySpecificationItem;
													}
												else
													{
													if( !existingSpecificationItem->hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&
													// Justification has at least the same assumption level
													existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, alternativePrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, currentSpecificationItem, NULL ) )
														primarySpecificationItem = alternativePrimarySpecificationItem;
													}
												}
											else
												{
												primarySpecificationItem = alternativePrimarySpecificationItem;

												if( existingSpecificationItem != NULL &&
												primarySpecificationItem != NULL )
													{
													if( isUserGeneralizationWord )
														{
														// Test file: "Laura - John and Anna - Joe"
														if( existingSpecificationItem->hasCurrentCreationSentenceNr() &&
														primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
														primarySpecificationItem->isSelfGeneratedAssumption() &&
														existingSpecificationItem->assumptionLevel() > primarySpecificationItem->assumptionLevel() &&
														// Recalculate assumption level of existing specification
														existingSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the existing specification" );
														}
													else
														{
														// Test files: "Complex (12)" and "Complex (15)"
														if( !existingSpecificationItem->hasCurrentCreationSentenceNr() &&
														primarySpecificationItem->isOlderItem() &&
														primarySpecificationItem->assumptionLevel() > existingSpecificationItem->assumptionLevel() &&
														// Recalculate assumption level of primary specification
														primarySpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the primary specification" );
														}
													}
												}
											}
										}

									hasPrimarySpecificationRelationContext = ( primarySpecificationItem == NULL ? false : primarySpecificationItem->hasRelationContext() );
									isPossessivePrimarySpecification = ( primarySpecificationItem == NULL ? false : primarySpecificationItem->isPossessive() );
									archivedAssignmentItem = NULL;

									if( isArchivedUserAssignment &&
									isUserDefinitionSpecification )
										{
										isArchivedAssignment = true;
										archivedAssignmentItem = userSpecificationItem;
										}

									selectedPrimarySpecificationItem = ( !isPossessiveUserSpecification ||

																		( primarySpecificationItem != NULL &&

																		( !primarySpecificationItem->isSelfGeneratedAssumption() ||
																		!primarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ? primarySpecificationItem :
																																				// Typical for Spanish
																																				userSpecificationItem );
									selectedSecondarySpecificationItem = ( archivedAssignmentItem == NULL ? currentSpecificationItem :
																											archivedAssignmentItem );

									if( primarySpecificationItem != NULL &&
									selectedPrimarySpecificationItem != NULL &&
									generalizationWordItem->foundJustificationItem( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) == NULL &&

									// No existing specification
									( ( existingSpecificationItem == NULL &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||

									// Typical for Spanish
									( foundPrimarySpecificationItem->isHiddenSpanishSpecification() &&
									!isCurrentUserSpecification ) ) &&

									( isUserQuestion ||
									isCurrentUserSpecification ||

									( ( !hasUserSpecificationRelationContext ||
									currentSpecificationCollectionNr == NO_COLLECTION_NR ) &&

									( isArchivedAssignment ||
									isUserGeneralizationWord ||
									isUserRelationWord ||
									primarySpecificationItem->isOlderItem() ) ) ) ) ||

									// Existing specification
									( existingSpecificationItem != NULL &&
									existingSpecificationItem->isSelfGeneratedSpecification() &&
									!selectedPrimarySpecificationItem->isActiveAssignment() &&

									( ( hasPrimarySpecificationRelationContext &&
									isPossessivePrimarySpecification ) ||

									!existingSpecificationItem->hasRelationContext() ||

									( isUserRelationWord &&
									existingSpecificationItem->isOlderItem() ) ) &&

									( isPossessivePrimarySpecification ||

									// Typical for Spanish
									( ( ( isUserRelationWord &&
									primarySpecificationItem != selectedPrimarySpecificationItem ) ||

									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||
									!existingSpecificationItem->hasSpecificationCollection() ) &&

									( primarySpecificationItem->hasCurrentCreationSentenceNr() ||

									( !primarySpecificationItem->hasCompoundSpecificationCollection() &&

									( isUserAdjectiveSpecification ||
									existingSpecificationItem->hasCurrentCreationSentenceNr() ||

									( hasUserSpecificationRelationContext &&
									isUserRelationWord ) ) ) ) &&

									( !primarySpecificationItem->isCorrectedSpecification() ||
									!primarySpecificationItem->isSelfGeneratedAssumption() ) &&

									( hasPrimarySpecificationRelationContext ||
									!existingSpecificationItem->isArchivedAssignment() ) ) ) &&

									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) &&

									// Skip uncertain justifications
									( ( !isPossessivePrimarySpecification &&
									compoundSpecificationCollectionNr > NO_COLLECTION_NR &&
									primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) ) ||

									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ) ) )
										{
										selectedRelationWordItem = ( existingSpecificationItem != NULL &&
																	existingSpecificationItem->hasRelationContext() &&
																	existingSpecificationItem->hasCurrentCreationSentenceNr() ? relationWordItem :

																	// Test file: "John - Anna (before family definition)"
																	( isExclusiveUserSpecification &&
																	!isSpanishCurrentLanguage &&
																	currentSpecificationCollectionNr > NO_COLLECTION_NR &&
																	selectedPrimarySpecificationItem->hasOnlyOneRelationWord() ? adminItem_->firstContextWordItem( selectedPrimarySpecificationItem->relationContextNr() ) : NULL ) );

										selectedRelationWordTypeNr = ( selectedRelationWordItem == NULL ? NO_WORD_TYPE_NR : WORD_TYPE_PROPER_NOUN );

										// Make a compound specification substitution assumption
										if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

										if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL )
											{
											// Test file: "Complex (3)"
											if( !hasRelationWord &&
											existingSpecificationItem != NULL &&
											!createdOrFoundSpecificationItem->isOlderItem() &&
											primarySpecificationItem->hasCompoundSpecificationCollection() &&
											primarySpecificationItem->isUserSpecification() &&
											// Remove obsolete assumption justifications from currently created or found specification
											createdOrFoundSpecificationItem->removeObsoleteAssumptionJustifications( false, false ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove obsolete assumption justifications from the currently created or found specification" );

											// Typical for Spanish
											// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
											if( isSpanishCurrentLanguage &&
											hasUserSpecificationNonCompoundCollection &&
											hasUserSpecificationRelationContext &&
											!isPossessiveUserSpecification &&
											!isUserSpecificationWordSpanishAmbiguous &&
											( selectedPrimarySpecificationItem = adminItem_->simpleUserSpecificationItem() ) != NULL &&
											( oppositeSpecificationWordItem = currentSpecificationWordItem->collectionWordItem( createdOrFoundSpecificationItem->specificationCollectionNr() ) ) != NULL &&
											( collectionWordItem = specificationWordItem->collectionWordItem( userSpecificationCollectionNr ) ) != NULL &&
											( selectedAdditionalDefinitionSpecificationItem = collectionWordItem->firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) != NULL &&
											// Draw negative Spanish assumption
											adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, createdOrFoundSpecificationItem, NULL, generalizationWordItem, oppositeSpecificationWordItem, NULL ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a negative Spanish assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", oppositeSpecificationWordItem->anyWordTypeString(), "\"" );
											}
										}

									// Typical for Spanish
									if( isSpecificationWordSpanishAmbiguous &&
									isUserGeneralizationWord &&
									hasUserSpecificationCompoundCollection &&
									!isOlderUserSpecification &&
									primarySpecificationItem != NULL &&

									( isArchivedUserAssignment ||

									( currentSpecificationCollectionNr > NO_COLLECTION_NR &&
									existingSpecificationItem != NULL &&
									existingSpecificationItem->isOlderItem() ) ) &&

									// Draw a specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, NULL ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or question for generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( ( primarySpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, specificationWordItem ) ) != NULL &&

									// Non-exclusive
									( ( !isExclusiveSpecification &&

									( !isSpecificationWordSpanishAmbiguous ||
									!isFoundPrimaryHiddenSpanishSpecification ) &&

									// Skip if already exists as user specification
									generalizationWordItem->firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, NO_CONTEXT_NR, currentSpecificationWordItem ) == NULL &&

									// Skip if new user specification has relation context, while the old one did not
									// Test file: "I know (10)"
									( !isGeneralizationProperNoun ||
									!primarySpecificationItem->hasRelationContext() ||
									generalizationWordItem->noRelationContextSpecificationItem( false, false, specificationWordItem ) == NULL ) ) ||

									// Exclusive
									( isExclusiveSpecification &&

									( isGeneralizationProperNoun ||
									!hasCurrentSpecificationCompoundCollection ||
									currentSpecificationWordItem->isFeminineOrMasculineWord() ) &&

									// Exclusive conclusion doesn't exist
									generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( true, false, false, true, NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, currentSpecificationWordItem ) == NULL ) ) )
										{
										createdSpecificationItem = NULL;
										createdOrFoundSpecificationItem = existingSpecificationItem;

										if( existingSpecificationItem == NULL ||

										// Justification doesn't exist yet in specification
										( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, currentSpecificationItem ) &&

										// Test file: "Complex (15)"
										( !primarySpecificationItem->isSelfGeneratedConclusion() ||
										generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, NULL ) == NULL ) &&

										// Justification has at least the same assumption level
										existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, currentSpecificationItem, NULL ) ) )
											{
											// Draw a compound specification substitution conclusion
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem->isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, currentSpecificationItem->generalizationContextNr(), currentSpecificationItem->specificationContextNr(), NO_CONTEXT_NR, primarySpecificationItem, NULL, currentSpecificationItem, NULL, generalizationWordItem, currentSpecificationWordItem, NULL ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

											if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
											createdOrFoundSpecificationItem->hasCurrentCreationSentenceNr() &&
											( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL &&
											// Remove obsolete assumption justifications from created or found specification
											createdOrFoundSpecificationItem->removeObsoleteAssumptionJustifications( false, false ) != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove obsolete assumption justifications from the older created or found specification" );
											}

										if( isGeneralizationProperNoun &&
										createdOrFoundSpecificationItem != NULL )
											{
											// Adjective
											if( currentSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE )
												{
												if( ( !hasUserSpecificationRelationContext ||
												createdOrFoundSpecificationItem->isOlderItem() ||
												( tempSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, NO_QUESTION_PARAMETER, primarySpecificationItem->specificationCollectionNr(), NULL ) ) == NULL ||
												!tempSpecificationItem->hasCompoundSpecificationCollection() ) &&

												// Draw negative adjective conclusions
												drawNegativeAdjectiveConclusions( isArchivedAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem ) != RESULT_OK)
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative adjective conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( !isUserAssignment &&
												userSpecificationCollectionNr > NO_COLLECTION_NR &&
												createdSpecificationItem != NULL &&

												( hasUserSpecificationNonCompoundCollection ||
												!hasUserSpecificationRelationContext ||
												isUserSpecificationWordSpanishAmbiguous ) &&

												// Avoid idle call
												generalizationWordItem->firstSpecificationItem( false, false, isArchivedUserAssignment, true ) != NULL &&
												// Make indirectly answered question assumption
												makeIndirectlyAnsweredQuestionAssumption( isArchivedUserAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}

											if( ( !isPossessiveUserSpecification ||
											primarySpecificationItem->isSelfGeneratedAssumption() ) &&

											primarySpecificationItem->assumptionLevel() <= createdOrFoundSpecificationItem->assumptionLevel() )
												{
												// Avoid creating duplicate negative justifications
												if( ( !hasUserSpecificationRelationContext ||
												( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, NO_QUESTION_PARAMETER, userSpecificationCollectionNr, specificationWordItem ) ) == NULL ||
												!tempSpecificationItem->isOlderItem() ) &&

												// Avoid idle call
												specificationWordItem->firstNegativeSpecificationItem() != NULL &&
												// Draw simple negative conclusions
												drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											else
												{
												if( hasUserSpecificationCompoundCollection &&
												isPossessiveUserSpecification &&
												isUserRelationWord &&
												existingSpecificationItem != NULL &&
												userSpecificationWordItem != NULL )
													{
													definitionSpecificationItem = ( specificationWordItem == userSpecificationWordItem ? currentSpecificationItem :
																																		// Typical for Spanish
																																		// Spanish test file: "Complejo (3)"
																																		userSpecificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, currentSpecificationWordItem ) );
													// Test file: "Complex (17)"
													existingSpecificationItem = existingSpecificationItem->updatedSpecificationItem();

													// Justification doesn't exist yet in specification
													if( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) &&
													// Justification has at least the same assumption level
													existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, definitionSpecificationItem, NULL ) &&
													// Add specification substitution justification to existing specification
													existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, definitionSpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the existing specification" );
													}
												}

											if( hasCurrentlyCorrectedAssumptionByKnowledge &&
											createdSpecificationItem != NULL &&
											// Find obsolete specification (incorrect assumption that has been corrected)
											( obsoleteSpecificationItem = generalizationWordItem->firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != NULL )
												{
												// Write invalid assumption by knowledge
												if( generalizationWordItem->writeUpdatedSpecification( false, true, false, false, false, false, false, obsoleteSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an invalid assumption by knowledge about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												// Replace incorrect assumption
												if( generalizationWordItem->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a corrected specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
												}
											}
										}
									}
								}
							}
						else
							{
							if( isExclusiveCurrentSpecification &&
							isGeneralizationProperNoun &&
							!isPossessiveUserSpecification &&
							currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
							existingSpecificationItem != NULL &&
							!existingSpecificationItem->isExclusiveSpecification() &&
							!existingSpecificationItem->isReplacedOrDeletedItem() &&
							existingSpecificationItem->isSelfGeneratedSpecification() &&
							!foundPrimarySpecificationItem->hasRelationContext() &&
							foundPrimarySpecificationItem->isSelfGeneratedSpecification() &&
							( definitionSpecificationItem = currentSpecificationWordItem->firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
							// Justification doesn't exist yet in specification
							!foundPrimarySpecificationItem->hasPrimaryAndSecondarySpecificationJustification( existingSpecificationItem, definitionSpecificationItem ) &&
							// Add specification substitution justification to found primary specification
							foundPrimarySpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, NULL, definitionSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the found primary specification" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextNonNegativeNonPossessiveDefinitionSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isUncountableGeneralizationNoun, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasExistingSpecificationCurrentCreationSentenceNr;
		bool hasExistingSpecificationRelationContext;
		bool hasFeminineOrMasculineProperNounEnding;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		bool hasGeneralizationWordMasculineProperNounEnding;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasRelationWordConfirmedSpecification = false;
		bool hasRelationWordFeminineOrMasculineProperNounEnding = false;
		bool hasRelationWordMasculineProperNounEnding;
		bool hasReplacedHiddenSpecification;
		bool hasSecondarySpecificationRelationContext;
		bool isExistingHiddenSpanishSpecification;
		bool isHiddenPrimarySpecification;
		bool isHiddenSecondarySpecification;
		bool isOlderExistingSpecification;
		bool isOlderPrimarySpecification;
		bool isOppositePossessiveSpecificationWordSpanishAmbiguous;
		bool isPossessiveSpecificationWordSpanishAmbiguous;
		bool isPrimarySameAsSecondarySpecification;
		bool isPrimarySpecificationAssignment;
		bool isPrimaryUserSpecification;
		bool isRelationWordUserWordGeneralization = false;
		bool isSelectedSecondaryUserSpecification;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isUserRelationWord;
		unsigned short prepositionParameter;
		unsigned short selectedJustificationTypeNr;
		unsigned short selectedRelationWordTypeNr;
		unsigned int relationContextNr;
		unsigned int specificationCollectionNr;
		JustificationItem *olderJustificationItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *additionalSpecificationItem;
		SpecificationItem *compoundSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *currentExistingSpecificationItem;
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
		SpecificationItem *selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *selectedAdditionalProperNounSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *commonWordItem;
		WordItem *currentPossessiveNounWordItem;
		WordItem *feminineSpecificationWordItem;
		WordItem *oppositePossessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveDefinitionSpecificationWordItem = NULL;
		WordItem *possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		oppositePossessiveDefinitionSpecificationItem_ = NULL;
		possessiveDefinitionSpecificationItem_ = NULL;

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, relationWordItem );

		// Typical for Spanish
		if( isSpanishCurrentLanguage &&
		primarySpecificationItem != NULL &&
		relationWordItem != NULL &&
		specificationWordItem->isNounWordSpanishAmbiguous() &&
		primarySpecificationItem->hasCompoundSpecificationCollection() &&
		primarySpecificationItem->isSelfGeneratedSpecification() &&
		( specificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&
		( tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) != NULL &&
		( relationContextNr = tempSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR &&
		relationWordItem->hasContextInWord( relationContextNr ) )
			// Spanish non-compound specification
			primarySpecificationItem = tempSpecificationItem;

		if( primarySpecificationItem != NULL )
			{
			isOlderPrimarySpecification = primarySpecificationItem->isOlderItem();
			isPrimarySpecificationAssignment = primarySpecificationItem->isAssignment();
			isPrimaryUserSpecification = primarySpecificationItem->isUserSpecification();

			if( ( !isOlderPrimarySpecification ||
			isPrimarySpecificationAssignment ||

			( relationWordItem != NULL &&
			primarySpecificationItem->isSelfGeneratedSpecification() ) ) &&

			( currentPossessiveNounWordItem = globalVariables_->firstPossessiveNounWordItem ) != NULL )
				{
				hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
				hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem->hasCurrentlyMoreSpecificSpecification();
				hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem->hasMasculineProperNounEnding();
				isUserRelationWord = generalizationWordItem->isUserRelationWord;

				isHiddenPrimarySpecification = primarySpecificationItem->isHiddenSpanishSpecification();

				if( relationWordItem != NULL )
					{
					hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();
					hasRelationWordFeminineOrMasculineProperNounEnding = relationWordItem->hasFeminineOrMasculineProperNounEnding();
					isRelationWordUserWordGeneralization = relationWordItem->isUserGeneralizationWord;
					}

				// Do for all possessive noun words
				do	{
					if( currentPossessiveNounWordItem != specificationWordItem &&
					currentPossessiveNounWordItem != generalizationWordItem )
						{
						// Find possessive specifications
						if( ( findSpecificationResult = findPossessiveSpecifications( currentPossessiveNounWordItem, specificationWordItem) ).result != RESULT_OK)
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find possessive specifications in word \"", currentPossessiveNounWordItem->anyWordTypeString(), "\"" );

						hasReplacedHiddenSpecification = false;
						additionalDefinitionSpecificationItem = NULL;
						createdOrFoundSpecificationItem = NULL;
						foundOppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem;
						foundPossessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem;

						if( foundPossessiveDefinitionSpecificationItem != NULL &&
						( possessiveDefinitionSpecificationWordItem = foundPossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
							{
							oppositePossessiveDefinitionSpecificationItem_ = foundOppositePossessiveDefinitionSpecificationItem;
							possessiveDefinitionSpecificationItem_ = foundPossessiveDefinitionSpecificationItem;

							if( !isPossessive &&
							( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL )
								{
								isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();

								if( primarySpecificationItem != NULL &&

								( ( !primarySpecificationItem->hasRelationContext() &&
								generalizationWordItem->hadOnceAnsweredSelfGeneratedQuestion() ) ||

								( isPossessiveSpecificationWordSpanishAmbiguous &&
								primarySpecificationItem->specificationWordItem() == possessiveDefinitionSpecificationWordItem ) ) )
									{
									// Draw Spanish specification substitution conclusion
									if( drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( ( existingUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) == NULL ||

									( hasRelationWord &&

									// Confirmed specification without relation word
									( !existingUserSpecificationItem->hasRelationContext() ||
									!existingUserSpecificationItem->hasSpecificationCollection() ) ) )
										{
										hasFeminineOrMasculineProperNounEnding = false;

										if( isPossessiveSpecificationWordSpanishAmbiguous &&
										isFeminineOrMasculineProperNounEnding( isPossessive, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) )
											hasFeminineOrMasculineProperNounEnding = true;

										if( !hasRelationWord &&
										( possessiveSpecificationWordItem = possessiveSpecificationItem->specificationWordItem() ) != NULL &&
										( commonWordItem = possessiveSpecificationWordItem->commonWordItem( possessiveSpecificationItem->specificationCollectionNr() ) ) != NULL &&
										commonWordItem != generalizationWordItem &&
										commonWordItem->isFeminineOrMasculineWord() &&
										!commonWordItem->isNounWordSpanishAmbiguous() &&
										( definitionSpecificationItem = commonWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, possessiveSpecificationWordItem ) ) != NULL &&
										// Make generalization assumption
										adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, definitionSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, commonWordItem, NULL ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", commonWordItem->anyWordTypeString(), "\"" );

										// User specification doesn't exist yet
										if( generalizationWordItem->firstUserSpecificationItem( !isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) == NULL )
											{
											selectedSecondarySpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem );

											// Specification doesn't exist yet
											if( ( currentExistingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, isPossessive, false, possessiveDefinitionSpecificationWordItem ) ) == NULL ||

											// No relation context
											( ( relationWordItem == NULL &&
											// Test files: "I know (11)" and "Paul is a boy - Laura is a girl"
											possessiveSpecificationItem->isSelfGeneratedSpecification() ) ||

											// Relation context
											( relationWordItem != NULL &&

											// Spanish test file: "Complejo (8)"
											( isPossessiveSpecificationWordSpanishAmbiguous ||
											!relationWordItem->hasContextInWord( currentExistingSpecificationItem->relationContextNr() ) ||

											( currentExistingSpecificationItem->isOlderItem() &&
											!currentExistingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, selectedSecondarySpecificationItem ) &&

											( ( currentExistingSpecificationItem->isSelfGeneratedSpecification() &&
											currentExistingSpecificationItem->hasOnlyOneRelationWord() ) ||

											( isPrimaryUserSpecification &&
											currentExistingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != NULL ) ) ) ) ) ) )
												{
												prepositionParameter = NO_PREPOSITION_PARAMETER;
												selectedRelationWordTypeNr = NO_WORD_TYPE_NR;
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

												if( hasRelationWord )
													{
													prepositionParameter = WORD_PARAMETER_PREPOSITION_OF;
													selectedRelationWordTypeNr = relationWordTypeNr;
													}

												if( selectedSecondarySpecificationItem != NULL )
													{
													// Make specification substitution assumption
													if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, prepositionParameter, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

													if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != NULL &&
													// Justification doesn't exist yet in specification
													!createdOrFoundSpecificationItem->hasPrimarySpecificationJustification( userSpecificationItem ) &&
													// Add user specification substitution justification to created or found specification
													createdOrFoundSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a user specification substitution justification to the created or found specification" );
													}
												else
													{
													// Typical for Spanish
													if( isSpanishCurrentLanguage )
														{
														if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, prepositionParameter, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, foundPossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, possessiveSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a selected assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and possessive specification word \"", possessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

														createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
														}
													}

												// Typical for Spanish
												if( hasRelationWord &&
												isPrimaryUserSpecification &&
												existingUserSpecificationItem == NULL &&
												createdOrFoundSpecificationItem != NULL &&
												createdOrFoundSpecificationItem->hasCurrentCreationSentenceNr() &&
												createdOrFoundSpecificationItem->isOlderItem() &&
												createdOrFoundSpecificationItem->isHiddenSpanishSpecification() &&
												!primarySpecificationItem->isActiveAssignment() &&
												// Add user specification to justifications of hidden Spanish specification
												createdOrFoundSpecificationItem->addUserSpecificationToSpanishJustifications( primarySpecificationItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add the user specification to justifications of the hidden Spanish specification" );
												}
											else
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

											if( createdOrFoundSpecificationItem != NULL )
												{
												secondarySpecificationItem = createdOrFoundSpecificationItem->updatedSpecificationItem();

												// Typical for Spanish
												if( isPossessiveSpecificationWordSpanishAmbiguous )
													{
													hasReplacedHiddenSpecification = true;

													// Spanish test file: "Que haya confirmado al menos una palabra de relación"
													// Draw simple negative conclusions
													if( drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, primarySpecificationItem->specificationWordItem() ) != RESULT_OK )
														return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
							// Uncountable generalization noun. Example: "Water ..."
							adminItem_->addSelfGeneratedSpecification( false, isPrimarySpecificationAssignment, false, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, NULL, primarySpecificationItem, NULL, generalizationWordItem, currentPossessiveNounWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentPossessiveNounWordItem->anyWordTypeString(), "\"" );
							}

						// First opposite possessive specification
						if( primarySpecificationItem != NULL &&
						possessiveDefinitionSpecificationWordItem != NULL &&
						foundPossessiveDefinitionSpecificationItem != NULL &&
						foundOppositePossessiveDefinitionSpecificationItem != NULL &&
						( oppositePossessiveDefinitionSpecificationWordItem = foundOppositePossessiveDefinitionSpecificationItem->specificationWordItem() ) != NULL )
							{
							if( createdOrFoundSpecificationItem == NULL ||
							primarySpecificationItem->hasSpecificationContext() )
								{
								secondarySpecificationItem = primarySpecificationItem;

								// Find additional definition definition
								if( !hasRelationWord ||
								isPossessive )
									additionalDefinitionSpecificationItem = possessiveDefinitionSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, specificationWordItem );
								}

							if( !isHiddenPrimarySpecification &&
							secondarySpecificationItem != NULL )
								{
								hasSecondarySpecificationRelationContext = secondarySpecificationItem->hasRelationContext();

								if( ( !hasSecondarySpecificationRelationContext ||
								!secondarySpecificationItem->isUserAssignment() ||
								secondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) &&

								( !isOlderPrimarySpecification ||
								primarySpecificationItem->isSelfGeneratedSpecification() ) )
									{
									hasExistingSpecificationCurrentCreationSentenceNr = false;
									hasExistingSpecificationRelationContext = false;
									isExistingHiddenSpanishSpecification = false;
									isOlderExistingSpecification = false;
									isHiddenSecondarySpecification = secondarySpecificationItem->isHiddenSpanishSpecification();

									if( ( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
									// Typical for Spanish
									!isHiddenSecondarySpecification &&
									hasGeneralizationWordCurrentlyConfirmedSpecification &&
									existingSpecificationItem->hasCompoundSpecificationCollection() &&
									existingSpecificationItem->hasOnlyOneRelationWord() &&
									( tempSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, !isPossessive, false, foundOppositePossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL )
										existingSpecificationItem = tempSpecificationItem;

									if( existingSpecificationItem != NULL )
										{
										hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem->hasCurrentCreationSentenceNr();
										hasExistingSpecificationRelationContext = existingSpecificationItem->hasRelationContext();
										isExistingHiddenSpanishSpecification = existingSpecificationItem->isHiddenSpanishSpecification();
										isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
										}

									isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem->isNounWordSpanishAmbiguous();
									isPrimarySameAsSecondarySpecification = ( primarySpecificationItem == secondarySpecificationItem );
									primarySpecificationItem = primarySpecificationItem->updatedSpecificationItem();
									selectedPrimarySpecificationItem = foundPossessiveDefinitionSpecificationItem;
									selectedAdditionalDefinitionSpecificationItem = ( !hasReplacedHiddenSpecification &&
																					isOppositePossessiveSpecificationWordSpanishAmbiguous ? NULL : additionalDefinitionSpecificationItem );

									selectedSecondarySpecificationItem = ( isHiddenSecondarySpecification ? primarySpecificationItem :
																											secondarySpecificationItem );

									isSelectedSecondaryUserSpecification = selectedSecondarySpecificationItem->isUserSpecification();

									if( existingSpecificationItem == NULL ||

									( ( relationWordItem == NULL ||

									// Justification doesn't exist yet in specification
									( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&

									( isSelectedSecondaryUserSpecification ||
									existingSpecificationItem->highestReversibleJustificationAssumptionLevel() > existingSpecificationItem->assumptionLevel() ||
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ) ||

									// Relation context doesn't exist yet
									( !relationWordItem->hasContextInWord( existingSpecificationItem->relationContextNr() ) &&

									( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ) ) &&

									( !hasRelationWord ||
									!hasExistingSpecificationRelationContext ||
									isExistingHiddenSpanishSpecification ||

									( !isOlderPrimarySpecification &&
									isUserRelationWord &&

									( isOppositePossessiveSpecificationWordSpanishAmbiguous ||

									( isPossessive &&
									hasRelationWordConfirmedSpecification ) ) ) ||

									// Possessive
									( isPossessive &&
									!hasRelationWordConfirmedSpecification &&

									( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
									// Justification doesn't exist yet in specification
									!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) ) &&

									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ||

									// Non-possessive
									( !isPossessive &&
									!isUserRelationWord &&

									( hasExistingSpecificationCurrentCreationSentenceNr ||
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ||

									( isPrimarySpecificationAssignment &&

									( !isHiddenSecondarySpecification ||

									secondarySpecificationItem->nRelationContextWords() < primarySpecificationItem->nRelationContextWords() ) ) ||

									( existingSpecificationItem->hasOnlyOneRelationWord() &&

									( secondarySpecificationItem->hasOnlyOneRelationWord() ||
									existingSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ) ) ) &&

									( ( hasRelationWord &&
									existingSpecificationItem->isSelfGeneratedAssumption() &&

									( hasRelationWordConfirmedSpecification ||
									!hasRelationWordFeminineOrMasculineProperNounEnding ||
									isPrimarySpecificationAssignment ||
									additionalDefinitionSpecificationItem != NULL ) ) ||

									( hasSecondarySpecificationRelationContext &&

									( isHiddenSecondarySpecification ||
									!secondarySpecificationItem->isSelfGeneratedConclusion() ) &&

									( !hasGeneralizationWordCurrentlyMoreSpecificSpecification ||
									!isHiddenSecondarySpecification ||
									// Typical for Spanish
									// Self-generated assignment
									isPrimarySpecificationAssignment ) &&

									( !isOlderExistingSpecification ||

									( hasExistingSpecificationRelationContext &&
									hasGeneralizationWordCurrentlyConfirmedSpecification ) ||

									( isPrimarySameAsSecondarySpecification &&
									additionalDefinitionSpecificationItem == NULL ) ) ) ) &&

									( selectedAdditionalDefinitionSpecificationItem == NULL ||
									// Justification has at least the same assumption level
									existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL ) ) ) )
										{
										selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : NO_WORD_TYPE_NR );

										createdSpecificationItem = existingSpecificationItem;
										foundSpecificationItem = existingSpecificationItem;

										if( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
										!hasRelationWord ||
										isPrimaryUserSpecification ||
										!isPossessive ||
										existingSpecificationItem == NULL ||
										!existingSpecificationItem->hasOnlyOneRelationWord() ||
										globalVariables_->nUserGeneralizationWords > 1 ||
										!selectedSecondarySpecificationItem->isSpecificationWordSpanishAmbiguous() )
											{
											hasRelationWordMasculineProperNounEnding = false;
											selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;

											if( hasRelationWord &&
											isPossessive &&
											generalizationWordItem->hasMasculineProperNounEnding() &&
											( tempSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL )
												{
												hasRelationWordMasculineProperNounEnding = true;
												selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
												selectedPrimarySpecificationItem = primarySpecificationItem;
												selectedAdditionalDefinitionSpecificationItem = foundPossessiveDefinitionSpecificationItem;
												selectedSecondarySpecificationItem = tempSpecificationItem;
												}

											// Make an opposite possessive specification assumption
											if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, foundPossessiveDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
											foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
											}

										if( relationWordItem != NULL )
											{
											if( isPossessive )
												{
												if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
													{
													if( primarySpecificationItem->isSelfGeneratedSpecification() &&
													( additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem->firstAssignmentOrSpecificationItem( false, false, NO_CONTEXT_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL )
														{
														hasFeminineOrMasculineProperNounEnding = false;

														if( isFeminineOrMasculineProperNounEnding( false, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) )
															{
															hasFeminineOrMasculineProperNounEnding = true;
															feminineSpecificationWordItem = NULL;

															if( feminineSpecificationWordItem_ != NULL )
																feminineSpecificationWordItem = oppositePossessiveDefinitionSpecificationWordItem->feminineCollectionWordItem();

															if( !hasGeneralizationWordMasculineProperNounEnding &&
															feminineSpecificationWordItem != NULL &&

															( !isOlderPrimarySpecification ||
															// Specification doesn't exist yet
															( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) == NULL ||

															// Justification doesn't exist yet in specification
															( !existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, additionalDefinitionSpecificationItem ) &&
															// Justification has at least the same assumption level
															existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, NULL ) ) ) &&

															// Make exclusive Spanish feminine specification substitution assumption
															adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, NULL, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an exclusive Spanish feminine specification substitution assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and opposite possessive specification word \"", feminineSpecificationWordItem->anyWordTypeString(), "\"" );
															}

														if( ( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, false, additionalDefinitionSpecificationItem->specificationCollectionNr(), NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
														compoundSpecificationItem != primarySpecificationItem )
															{
															if( hasFeminineOrMasculineProperNounEnding )
																{
																selectedJustificationTypeNr = JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
																selectedPrimarySpecificationItem = compoundSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = foundOppositePossessiveDefinitionSpecificationItem;
																selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;
																}
															else
																{
																selectedJustificationTypeNr = JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
																selectedPrimarySpecificationItem = foundPossessiveDefinitionSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = NULL;
																selectedSecondarySpecificationItem = compoundSpecificationItem;
																}

															// Make a compound specification exclusive specification substitution assumption (opposite)
															if( adminItem_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, NULL, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );

															if( feminineSpecificationWordItem_ != NULL &&
															adminItem_->addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, generalizationWordTypeNr, WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : NULL ), ( hasFeminineOrMasculineProperNounEnding ? additionalDefinitionSpecificationItem : compoundSpecificationItem ), NULL, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ).result != RESULT_OK )
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
														if( additionalDefinitionSpecificationItem == NULL )
															{
															// Possessive
															if( ( additionalSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, isArchivedAssignment, false, true, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
															// Add opposite possessive conclusion justification to additional specification
															additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the additional specification" );
															}
														else
															{
															// Found, but not created
															if( ( additionalSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != NULL &&
															( selectedSecondarySpecificationItem = specificationWordItem->firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != NULL &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
															// Add specification substitution justification to additional specification
															additionalSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );
															}
														}
													}
												}
											else	// Non-possessive
												{
												if( existingSpecificationItem != NULL )
													{
													if( createdSpecificationItem == NULL )
														{
														if( foundSpecificationItem != NULL &&
														foundSpecificationItem->isOlderItem() &&
														existingSpecificationItem->isReplacedItem() &&
														( tempSpecificationItem = relationWordItem->firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
														tempSpecificationItem->isSelfGeneratedConclusion() )
															{
															existingSpecificationItem = existingSpecificationItem->updatedSpecificationItem();

															// Draw reversible conclusion
															if( adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, existingSpecificationItem, NULL, NULL, NULL, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
															}

														if( existingSpecificationItem->isReplacedItem() )
															{
															if( !primarySpecificationItem->isUserAssignment() &&
															( tempSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL &&
															tempSpecificationItem->isSelfGeneratedSpecification() &&
															// Add opposite possessive assumption justification to existing specification
															tempSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, NULL, primarySpecificationItem ) != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive assumption justification to the existing specification" );
															}
														else
															{
															if( hasExistingSpecificationCurrentCreationSentenceNr &&
															!isOlderExistingSpecification )
																{
																if( isPrimarySpecificationAssignment )
																	{
																	if( !isSelectedSecondaryUserSpecification &&
																	!selectedSecondarySpecificationItem->isOlderItem() )
																		{
																		// Test file: "John - Anna (before family definition)"
																		// Remove obsolete assumption justifications from existing specification
																		if( existingSpecificationItem->removeObsoleteAssumptionJustifications( false, false ) != RESULT_OK )
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to remove obsolete assumption justifications from the existing specification" );

																		if( ( additionalSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != NULL &&
																		// Justification doesn't exist yet in specification
																		!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( foundPossessiveDefinitionSpecificationItem, additionalSpecificationItem ) &&
																		// Add additional opposite possessive conclusion justification to existing specification
																		existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, NULL, additionalSpecificationItem ) != RESULT_OK )
																			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an additional opposite possessive conclusion justification to the existing specification" );
																		}
																	}
																else
																	{
																	if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
																	( selectedSecondarySpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != NULL &&

																	( !selectedSecondarySpecificationItem->isOlderItem() ||
																	selectedSecondarySpecificationItem->isUserSpecification() ) &&

																	// Justification doesn't exist yet in specification
																	!existingSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( foundPossessiveDefinitionSpecificationItem, selectedSecondarySpecificationItem ) &&
																	// Justification has at least the same assumption level
																	existingSpecificationItem->assumptionLevel() >= adminItem_->justificationAssumptionLevel( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, NULL, selectedSecondarySpecificationItem, NULL ) &&
																	// Add opposite possessive conclusion justification to existing specification
																	existingSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, NULL, selectedSecondarySpecificationItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the existing specification" );
																	}
																}
															}
														}
													else
														{
														if( hasExistingSpecificationRelationContext )
															{
															if( !existingSpecificationItem->isReplacedOrDeletedItem() )
																{
																if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
																	{
																	// Replace existing specification by created one
																	// Created specification was hidden
																	if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing Spanish specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

																	// Typical for Spanish
																	if( ( olderJustificationItem = existingSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) != NULL &&
																	// Attach older justification to created specification
																	createdSpecificationItem->attachJustificationToSpecification( olderJustificationItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach an older justification item to the created assumption" );
																	}
																else
																	{
																	// Replace existing specification by created one
																	if( generalizationWordItem->replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace the existing specification in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																	}
																}
															}
														else
															{
															// Test file: "Complex (19 - mixed)"
															if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
															( foundSpecificationItem = relationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, (unsigned int)NO_COLLECTION_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != NULL &&
															// Draw reversible conclusion
															adminItem_->addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, WORD_PARAMETER_PREPOSITION_OF, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != RESULT_OK )
																return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a reversible conclusion about word \"", relationWordItem->anyWordTypeString(), "\" with specification word \"", oppositePossessiveDefinitionSpecificationWordItem->anyWordTypeString(), "\"" );
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
					}
				while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem->nextPossessiveNounWordItem ) != NULL );

				// No conflict found
				if( !globalVariables_->hasDisplayedWarning &&
				primarySpecificationItem != NULL &&
				secondarySpecificationItem != NULL &&
				relationWordItem != NULL )
					{
					// Draw reversible conclusions
					if( drawReversibleConclusions( userSpecificationItem, relationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", relationWordItem->anyWordTypeString(), "\"" );

					if( isSpanishCurrentLanguage &&
					primarySpecificationItem->isSpecificationWordSpanishAmbiguous() &&
					primarySpecificationItem->hasNonCompoundSpecificationCollection() &&
					( compoundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, specificationWordItem->compoundCollectionNr(), primarySpecificationItem->relationContextNr(), specificationWordItem ) ) != NULL )
						primarySpecificationItem = compoundSpecificationItem;

					selectedAdditionalProperNounSpecificationItem = ( isHiddenPrimarySpecification ? userSpecificationItem :
																									primarySpecificationItem );

					if( ( ( isRelationWordUserWordGeneralization ||
					primarySpecificationItem == userSpecificationItem ) &&

					( isSpanishCurrentLanguage ||
					!secondarySpecificationItem->hasCompoundSpecificationCollection() ) ) &&

					// Make an opposite possessive specification assumption (based on secondary specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, selectedAdditionalProperNounSpecificationItem, userSpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on secondary specification) during the making an exclusive specification substitution assumption about word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

					if( existingSpecificationItem != NULL &&
					primarySpecificationItem == secondarySpecificationItem &&
					primarySpecificationItem->hasCompoundSpecificationCollection() &&

					( ( !isSpanishCurrentLanguage &&
					isPossessive &&

					( !isRelationWordUserWordGeneralization ||
					!existingSpecificationItem->hasCurrentCreationSentenceNr() ) &&

					( isUserRelationWord ||
					existingSpecificationItem->isSelfGeneratedAssumption() ) ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&
					!isPossessive &&
					!existingSpecificationItem->isOlderItem() &&
					existingSpecificationItem->isHiddenSpanishSpecification() ) ) &&

					// Make an opposite possessive specification assumption (based on existing specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, existingSpecificationItem, ( isHiddenPrimarySpecification ? userSpecificationItem : primarySpecificationItem ), userSpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on existing specification) during the making an exclusive specification substitution assumption about word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char makeIndirectlyAnsweredQuestionAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *adjectiveSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		unsigned short specificationWordTypeNr;
		unsigned int currentSpecificationCollectionNr;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *createdOrFoundSpecificationItem;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *negativeDefinitionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentQuestionSpecificationWordItem;
		WordItem *primarySpecificationWordItem;
		WordItem *relatedSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

		if( adjectiveSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given adjective specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( !adjectiveSpecificationItem->isSpecificationAdjective() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given adjective specification has no adjective specification" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, isArchivedAssignment, true ) ) != NULL )
			{
			do	{
				if( !currentQuestionSpecificationItem->hasCurrentCreationSentenceNr() &&
				( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL &&
				currentQuestionSpecificationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) != NULL )
					{
					specificationWordTypeNr = currentQuestionSpecificationItem->specificationWordTypeNr();
					currentSpecificationCollectionNr = currentQuestionSpecificationItem->specificationCollectionNr();
					createdOrFoundSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem );

					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem->findRelatedSpecification( true, isArchivedAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem->questionParameter(), specificationWordTypeNr, currentSpecificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a related specification in word \"", generalizationWordItem->anyWordTypeString(), "\" that is Spanish ambiguous" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
					relatedSpecificationItem != currentQuestionSpecificationItem &&
					( relatedSpecificationWordItem = relatedSpecificationItem->specificationWordItem() ) != NULL &&
					( primarySpecificationItem = generalizationWordItem->firstOlderNonPossessiveNonQuestionSpecificationItem( isArchivedAssignment ) ) != NULL &&
					( primarySpecificationWordItem = primarySpecificationItem->specificationWordItem() ) != NULL &&
					( additionalDefinitionSpecificationItem = primarySpecificationWordItem->firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem ) ) != NULL &&
					( secondarySpecificationItem = relatedSpecificationWordItem->firstAdjectiveSpecificationItem( false ) ) != NULL )
						{
						// Typical for Spanish
						if( primarySpecificationItem->isSpecificationWordSpanishAmbiguous() )
							{
							// Draw Spanish specification substitution conclusion or ask a question
							if( drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							// Draw an indirectly answered question conclusion
							if( ( createAndAssignResult = adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, adjectiveSpecificationItem, generalizationWordItem, currentQuestionSpecificationWordItem, NULL ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentQuestionSpecificationWordItem->anyWordTypeString(), "\"" );

							createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
							}
						}

					if( createdOrFoundSpecificationItem != NULL &&
					currentQuestionSpecificationItem->isSpecificationWordSpanishAmbiguous() )
						{
						// Spanish test file: "Preguntas respondidas indirectamente - Juan es masculino"
						if( ( negativeDefinitionSpecificationItem = currentQuestionSpecificationWordItem->firstNegativeSpecificationItem() ) != NULL &&
						!negativeDefinitionSpecificationItem->hasSpecificationCollection() &&
						// Draw simple Spanish negative conclusions
						drawSimpleNegativeConclusions( isArchivedAssignment, adjectiveSpecificationItem, generalizationWordItem, currentQuestionSpecificationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative Spanish conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						// Draw Spanish proper noun part-of conclusions in proper noun words
						if( drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, currentQuestionSpecificationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );

						// Find answers to questions
						// Missed a question question to answer
						if( generalizationWordItem->findAnswersToQuestions( currentSpecificationCollectionNr, createdOrFoundSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find answers to questions in generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentQuestionSpecificationItem = ( currentQuestionSpecificationItem->isReplacedOrDeletedItem() ?
														// First question specification
														adminItem_->firstSpecificationItem( false, false, isArchivedAssignment, true ) :
														// Next question specification
														currentQuestionSpecificationItem->nextSelectedSpecificationItem() ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeOnlyOptionLeftAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		unsigned short nOptionsLeft = 0;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
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

		if( ( currentQuestionSpecificationItem = generalizationWordItem->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != NULL )
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
			// Get first generalization specification noun word
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
						return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != NULL &&
					( additionalDefinitionSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != NULL &&
					// Make an only option left assumption
					adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, userSpecificationItem, NULL, generalizationWordItem, onlyOptionLeftSpecificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an only option left conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char makeSpecificationSubstitutionPartOfAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasGeneralizationWordFeminineProperNounEnding;
		bool isChineseCurrentLanguage;
		bool isFeminineOrMasculineGeneralizationWord;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *feminineCollectionWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeSpecificationSubstitutionPartOfAssumption";

		if( !adminItem_->isNounWordType( specificationWordTypeNr ) )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word type isn't a noun" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem == specificationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item and specification word item are the same" );

		// Get first noun word specification generalization
		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem->hasFeminineProperNounEnding();
			isChineseCurrentLanguage = adminItem_->isChineseCurrentLanguage();
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem->isFeminineOrMasculineWord();
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			if( userSpecificationItem == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != NULL )
					{
					if( ( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||
					!currentGeneralizationWordItem->hasCollection() ||

					// Typical for Chinese
					( isChineseCurrentLanguage &&
					generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
					primarySpecificationItem->hasCompoundSpecificationCollection() &&

					( ( foundSpecificationItem = specificationWordItem->firstNonQuestionSpecificationItem() ) == NULL ||
					foundSpecificationItem->hasSpecificationCollection() ) ) ) &&

					// Specification doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem ) ) == NULL ||
					// Skip if concluded conclusion already exists
					existingSpecificationItem != userSpecificationItem ) )
						{
						alternativeSpanishAdditionalDefinitionSpecificationItem_ = NULL;
						secondarySpecificationItem = NULL;

						// Get secondary specification for justification
						if( isUserGeneralizationWord &&
						!userSpecificationItem->isPossessive() &&

						( ( isSpecificationWordSpanishAmbiguous &&
						userSpecificationItem->specificationWordItem() == primarySpecificationItem->specificationWordItem() ) ||

						userSpecificationItem->specificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() ) )
							secondarySpecificationItem = userSpecificationItem;
						else
							{
							if( isSpecificationWordSpanishAmbiguous &&
							hasGeneralizationWordFeminineProperNounEnding &&
							( feminineCollectionWordItem = specificationWordItem->feminineCollectionWordItem() ) != NULL )
								secondarySpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, feminineCollectionWordItem );

							if( secondarySpecificationItem == NULL &&
							( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem ) ) != NULL &&

							( existingSpecificationItem == NULL ||
							foundSpecificationItem->isSelfGeneratedConclusion() ) )
								secondarySpecificationItem = foundSpecificationItem;
							}

						if( !isGeneralizationProperNoun &&
						existingSpecificationItem == NULL &&
						secondarySpecificationItem != NULL &&
						primarySpecificationItem->isExclusiveSpecification() &&

						( !isFeminineOrMasculineGeneralizationWord ||
						secondarySpecificationItem->hasNonCompoundSpecificationCollection() ) &&

						// Skip if user specification already exists
						generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, currentGeneralizationWordItem ) == NULL &&
						// Skip if justification with primary specification already exists
						generalizationWordItem->primaryAndSecondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem, secondarySpecificationItem ) == NULL &&
						// Make generalization assumption
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, !isGeneralizationProperNoun, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, primarySpecificationItem, NULL, secondarySpecificationItem, NULL, generalizationWordItem, currentGeneralizationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about generalization word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( specificationCollectionNr > NO_COLLECTION_NR &&
		// Check if a related specification exists
		( relatedResult = generalizationWordItem->findRelatedSpecification( false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", generalizationWordItem->anyWordTypeString(), "\" is related to the secondary specification" );

		if( relatedResult.relatedSpecificationItem == NULL )
			{
			if( generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() )
				{
				if( correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( isArchivedAssignment, isNegative, isPossessive, primarySpecificationItem->questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			else
				{
				// Typical for Spanish:
				// Skip making suggestive assumption if specification word is Spanish ambiguous,
				// and if user question has more than one generalization words
				if( !primarySpecificationItem->isSpecificationWordSpanishAmbiguous() ||
				globalVariables_->nUserGeneralizationWords == 1 )
					{
					hasSuggestiveQuestionAssumption_ = true;

					// Make a suggestive question assumption
					if( adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, primarySpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a suggestive question assumption to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		return RESULT_OK;
		}

	CompoundResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *specificationWordItem )
		{
		bool hasNonCompoundUserSpecificationCollection;
		bool hasUserSpecificationCollection;
		bool isArchivedAssignment;
		bool isExclusiveSpecification;
		bool isExclusiveUserSpecification;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		unsigned short generalizationWordTypeNr;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *secondaryDefinitionSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *relatedSpecificationWordItem;
		WordItem *secondarySpecificationWordItem;
		WordItem *userSpecificationWordItem;
		CompoundResultType compoundResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( userSpecificationItem == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem->specificationWordItem() ) == NULL )
			return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( true ) ) != NULL )
			{
			hasNonCompoundUserSpecificationCollection = userSpecificationItem->hasNonCompoundSpecificationCollection();
			hasUserSpecificationCollection = userSpecificationItem->hasSpecificationCollection();
			isExclusiveUserSpecification = userSpecificationItem->isExclusiveSpecification();

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startCompoundResultError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( foundSpecificationItem = currentGeneralizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
					{
					if( foundSpecificationItem->isPartOf() )
						{
						if( compoundResult.compoundGeneralizationWordItem == NULL )
							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
						}
					else
						{
						isArchivedAssignment = foundSpecificationItem->isArchivedAssignment();
						isExclusiveSpecification = foundSpecificationItem->isExclusiveSpecification();
						generalizationWordTypeNr = foundSpecificationItem->generalizationWordTypeNr();

						// Definition
						if( isExclusiveSpecification &&
						generalizationWordTypeNr != WORD_TYPE_PROPER_NOUN &&
						( secondaryDefinitionSpecificationItem = specificationWordItem->firstNonCollectedSpecificationItem( true, currentGeneralizationWordItem ) ) != NULL &&
						( secondarySpecificationWordItem = secondaryDefinitionSpecificationItem->specificationWordItem() ) != NULL &&

						( !isSpanishCurrentLanguage ||
						// Typical for Spanish
						foundSpecificationItem->hasCompoundSpecificationCollection() ) &&

						// Make a specification substitution definition assumption
						adminItem_->addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, foundSpecificationItem->isEveryGeneralization(), false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, foundSpecificationItem, NULL, secondaryDefinitionSpecificationItem, NULL, currentGeneralizationWordItem, secondarySpecificationWordItem, NULL ).result != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to make a specification substitution definition assumption about word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

						if( ( hasUserSpecificationCollection ||
						isExclusiveSpecification ) &&

						// Avoid idle call
						specificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL &&
						// Draw a compound specification substitution conclusion
						drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, foundSpecificationItem, NULL, userSpecificationItem, currentGeneralizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to draw a compound specification substitution conclusion about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( ( ( ( !isExclusiveSpecification ||

						( isSpanishCurrentLanguage &&
						// Avoid idle call
						specificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL ) ) &&

						foundSpecificationItem->isOlderItem() ) ||

						// Typical for Spanish
						// Test file: "Scientific challenge"
						( isExclusiveSpecification &&
						generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN ) ||

						// Test file: "John has 2 sons and a daughter (before family definition)"
						( isExclusiveSpecification &&
						isExclusiveUserSpecification &&
						foundSpecificationItem->hasNonCompoundSpecificationCollection() &&
						userSpecificationItem->hasCompoundSpecificationCollection() ) ) &&

						// Draw a specification substitution conclusion or ask a question
						drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, currentGeneralizationWordItem, specificationWordItem, NULL ) != RESULT_OK )
							return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

						if( isExclusiveSpecification &&
						foundSpecificationItem->isOlderItem() )
							{
							if( hasNonCompoundUserSpecificationCollection &&
							foundSpecificationItem->hasNonCompoundSpecificationCollection() )
								{
								if( ( relatedResult = currentGeneralizationWordItem->findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
									return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to find out if a specification in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" is related to the found specification" );

								if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
								( relatedSpecificationWordItem = relatedSpecificationItem->specificationWordItem() ) != NULL )
									{
									// Avoid idle call
									if( relatedSpecificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL &&
									// Draw a specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem->isArchivedAssignment(), relatedSpecificationItem->isExclusiveSpecification(), relatedSpecificationItem->questionParameter(), relatedSpecificationItem->generalizationWordTypeNr(), specificationWordTypeNr, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, currentGeneralizationWordItem, relatedSpecificationWordItem, NULL ) != RESULT_OK )
										return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to draw a related specification substitution conclusion or question for generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

									// Make a related specification substitution 'part of' assumption
									if( makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, relatedSpecificationWordItem, userSpecificationWordItem->collectionWordItem( userSpecificationItem->specificationCollectionNr() ) ) != RESULT_OK )
										return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"", relatedSpecificationWordItem->anyWordTypeString(), "\"" );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									// Make a Spanish specification substitution 'part of' assumption
									makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, specificationWordItem, userSpecificationWordItem ) != RESULT_OK )
										return adminItem_->addCompoundResultError( functionNameString, moduleNameString_, "I failed to make a Spanish specification substitution 'part of' assumption in specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}

							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return compoundResult;
		}
	};

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
