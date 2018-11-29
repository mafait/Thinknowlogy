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

package org.mafait.thinknowlogy;

class AdminReasoningNew
	{
	// Private constructed variables

	private boolean hasSkippedHiddenSpanishSpecification_ = false;
	private boolean hasSuggestiveQuestionAssumption_ = false;

	private SpecificationItem adjustedSpanishQuestionSecondarySpecificationItem_ = null;
	private SpecificationItem firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = null;
	private SpecificationItem secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = null;
	private SpecificationItem secondaryDefinitionSpecificationItem_ = null;

	private WordItem adjustedQuestionWordItem_ = null;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private byte askQuestions( boolean isAllowingSpanishPossessiveSpecification, boolean isArchivedUserAssignment, boolean isCheckingForGapInKnowledge, int userSpecificationCollectionNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem userSpecificationWordItem )
		{
		boolean hasExclusiveSecondarySpecificationCompoundCollection;
		boolean hasFeminineOrMasculineProperNounEnding;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasPrimarySpecificationRelationContext;
		boolean hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection;
		boolean hasSkippedAskingQuestion;
		boolean hasUserSpecificationCompoundCollection;
		boolean hasUserSpecificationRelationContext;
		boolean isArchivedAssignment;
		boolean isCompoundGeneralizationWordSpanishAmbiguous;
		boolean isExclusiveUserSpecification;
		boolean isPossessivePrimarySpecification;
		boolean isPossessiveUserSpecification;
		boolean isPrimaryOldUserSpecification;
		boolean isPrimarySpecificationSelfGeneratedAssumption;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		boolean isPrimaryUserSpecification;
		boolean isUsefulUserSpecificationWithoutRelation;
		boolean isUserGeneralizationWord;
		boolean isUserSpecificationWordSpanishAmbiguous;
		short generalizationWordTypeNr;
		int exclusiveSecondarySpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem archivedPrimarySpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem exclusiveSecondarySpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem questionPrimarySpecificationItem;
		SpecificationItem questionSecondarySpecificationItem;
		WordItem collectionWordItem;
		WordItem compoundGeneralizationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem primarySpecificationWordItem;
		BoolResultType boolResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		hasFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

		hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
		hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
		isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
		isPossessiveUserSpecification = userSpecificationItem.isPossessive();
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
		isUsefulUserSpecificationWithoutRelation = ( hasUserSpecificationCompoundCollection &&
													!hasUserSpecificationRelationContext &&
													isUserGeneralizationWord &&

													( isExclusiveUserSpecification ||
													isPossessiveUserSpecification ||
													GlobalVariables.nUserSpecificationWords == 1 ) );

		do	{
			if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null )
				{
				hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
				isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();
				isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();

				if( isPossessivePrimarySpecification )
					{
					if( !hasFeminineOrMasculineProperNounEnding )
						{
						archivedPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, false, primarySpecificationWordItem );

						// Typical for Spanish
						if( anotherPrimarySpecificationItem != null &&
						// Specification with user specification collection doesn't exist
						generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) == null &&
						// Ask questions about generalization word with possessive primary specification
						askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( adminItem_.hasFemaleUserSpecificationWord(), !hasUserSpecificationCompoundCollection, isArchivedAssignment, isArchivedUserAssignment, true, isUserSpecificationWordSpanishAmbiguous, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != null && archivedPrimarySpecificationItem.isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), anotherPrimarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask Spanish questions about word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
						}
					}
				else
					{
					hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();

					if( ( !hasFeminineOrMasculineProperNounEnding ||
					hasPrimarySpecificationCompoundCollection ||

					( !hasPrimarySpecificationRelationContext &&
					primarySpecificationItem == userSpecificationItem ) ) &&

					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( exclusiveSecondarySpecificationItem = primarySpecificationWordItem.firstExclusiveSpecificationItem() ) != null )
						{
						hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection = generalizationWordItem.hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();
						hasSkippedAskingQuestion = false;
						isPrimarySpecificationSelfGeneratedAssumption = primarySpecificationItem.isSelfGeneratedAssumption();
						isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();
						isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();
						isPrimaryOldUserSpecification = ( isPrimaryUserSpecification &&
														primarySpecificationItem.isOlderItem() );

						generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();

						alternativePrimarySpecificationItem = null;

						if( isUserGeneralizationWord )
							{
							if( !hasUserSpecificationRelationContext &&
							!isPrimaryUserSpecification &&
							generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation() )
								alternativePrimarySpecificationItem = userSpecificationItem;
							}
						else
							{
							if( isPossessiveUserSpecification )
								{
								if( isUserSpecificationWordSpanishAmbiguous &&
								// Number of relation words of user specification item
								userSpecificationItem.nContextRelations() > 1 &&
								// Specification with exclusive secondary specification collection, or user specification word, doesn't exist
								generalizationWordItem.firstUserSpecificationItem( false, false, exclusiveSecondarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, userSpecificationItem.specificationWordItem() ) == null &&

								askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( isArchivedAssignment, generalizationWordTypeNr, userSpecificationItem, generalizationWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to ask a Spanish question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
								}
							else
								{
								if( isPrimarySpecificationSelfGeneratedAssumption )
									alternativePrimarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, primarySpecificationWordItem, null );
								}
							}

						do	{
							hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem.hasCompoundSpecificationCollection();

							if( ( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
							( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null )
								{
								isCompoundGeneralizationWordSpanishAmbiguous = ( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem.compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null &&
																				compoundGeneralizationWordItem.isNounWordSpanishAmbiguous() );

								if( ( ( hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection ||

								( !isPrimaryOldUserSpecification &&

								( ( hasUserSpecificationCompoundCollection &&
								isUsefulUserSpecificationWithoutRelation &&
								adjustedQuestionWordItem_ == null ) ||

								( !isUsefulUserSpecificationWithoutRelation &&

								( isPrimaryUserSpecification ||

								( hasExclusiveSecondarySpecificationCompoundCollection &&
								hasUserSpecificationRelationContext ) ) ) ) ) ||

								// Adjusted question
								( adjustedQuestionWordItem_ != null &&
								hasPrimarySpecificationCompoundCollection &&

								( !isUserSpecificationWordSpanishAmbiguous ||
								compoundGeneralizationWordItem != primarySpecificationWordItem ) ) ) &&

								( ( isCompoundGeneralizationWordSpanishAmbiguous &&
								!isUserSpecificationWordSpanishAmbiguous ) ||

								// Question part doesn't exist
								!generalizationWordItem.hasJustification( ( alternativePrimarySpecificationItem == null ? primarySpecificationItem : alternativePrimarySpecificationItem ), null, exclusiveSecondarySpecificationItem ) ) ) &&

								( collectionWordItem = exclusiveSecondarySpecificationWordItem.collectionWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null )
									{
									// Adjusted question
									if( alternativePrimarySpecificationItem != null ||

									( hasPrimarySpecificationCompoundCollection &&
									!isUserSpecificationWordSpanishAmbiguous &&

									( adjustedQuestionWordItem_ != null ||

									( hasPrimarySpecificationRelationContext &&
									isCompoundGeneralizationWordSpanishAmbiguous ) ) ) ||

									( isUserSpecificationWordSpanishAmbiguous &&
									compoundGeneralizationWordItem != primarySpecificationWordItem &&

									( !hasPrimarySpecificationRelationContext ||
									// Same specification but without relation context doesn't exists. Spanish test file: "Esta información es más específico"
									generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, primarySpecificationWordItem ) == null ) ) ||

									// Has possibly a gap in the knowledge
									( isCheckingForGapInKnowledge &&

									( !primarySpecificationItem.isOlderItem() ||
									userSpecificationItem.isQuestion() ) &&

									generalizationWordItem.hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationItem ) ) )
										{
										// Avoid asking invalid questions:
										// 1) Check for existing knowledge with same specification collection
										if( generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, exclusiveSecondarySpecificationWordItem ) == null &&
										// 2) Check for existing negative specification. Test file: "Skip invalid questions (1)"
										generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, true, false, collectionWordItem, null ) == null &&
										// 3) Check for existing user specification. Test file: "Skip invalid questions (2)"
										generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, collectionWordItem ) == null )
											{
											questionPrimarySpecificationItem = ( alternativePrimarySpecificationItem == null ? primarySpecificationItem : alternativePrimarySpecificationItem );
											questionSecondarySpecificationItem = ( isPrimarySpecificationWordSpanishAmbiguous &&
																					isUserGeneralizationWord &&
																					adjustedSpanishQuestionSecondarySpecificationItem_ != null ?
																						adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem );

											// Ask a specification substitution question
											// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, questionPrimarySpecificationItem, null, questionSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );

											// Typical for Spanish
											if( isPrimarySpecificationWordSpanishAmbiguous )
												{
												if( adjustedSpanishQuestionSecondarySpecificationItem_ == null &&
												createAndAssignResult.createdSpecificationItem == null )
													// Adjusted question
													adjustedSpanishQuestionSecondarySpecificationItem_ = exclusiveSecondarySpecificationItem;
												}
											else
												{
												if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
												foundSpecificationItem.isCorrectedAssumption() &&
												!primarySpecificationItem.hasCurrentCreationSentenceNr() )
													{
													// Create justification to corrected assumption by opposite suggestive question
													if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, foundSpecificationItem, null, exclusiveSecondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
														return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

													// Copy specification
													if( ( createAndAssignResult = generalizationWordItem.copySpecificationItem( primarySpecificationItem.specificationCollectionNr(), createdJustificationItem, primarySpecificationItem ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to copy the assumption specification with the created justification item" );

													if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
														return adminItem_.startError( 1, moduleNameString_, "I couldn't copy a corrected assumption by opposite suggestive question" );

													// Replace primary specification
													if( generalizationWordItem.replaceOrDeleteSpecification( primarySpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the primary specification by the created specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ == null )
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
										adjustedQuestionWordItem_ == null &&
										primarySpecificationWordItem.hasCollectionNr( userSpecificationCollectionNr ) &&
										exclusiveSecondarySpecificationWordItem.nonCompoundCollectionNr() > Constants.NO_COLLECTION_NR )
											{
											// Find question to be adjusted by compound collection
											if( ( boolResult = generalizationWordItem.findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationCollectionNr, primarySpecificationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );

											if( boolResult.booleanValue )
												adjustedQuestionWordItem_ = generalizationWordItem;
											}
										}
									}
								}
							}
						while( !hasSkippedAskingQuestion &&
						( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveNounSpecificationItem() ) != null );
						}
					}
				}
			}
		while( ( primarySpecificationItem = primarySpecificationItem.nextPrimarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null );

		return Constants.RESULT_OK;
		}

	private byte askQuestionsAboutSpecificSituations( boolean isArchivedAssignment, boolean isSpanishCurrentLanguage, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem userSpecificationItem, WordItem userGeneralizationWordItem, WordItem userSpecificationWordItem )
		{
		JustificationItem obsoleteJustificationItem;
		SpecificationItem correctedSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( userGeneralizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user generalization word item is undefined" );

		if( userSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification word item is undefined" );

		if( userSpecificationItem.isExclusiveSpecification() )
			{
			// Question part doesn't exist
			if( !userGeneralizationWordItem.hasJustification( userSpecificationItem, null, null ) &&

			// Ask question after more specific non-exclusive user specification
			// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, userGeneralizationWordItem, userSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about the current user sentence" );
			}
		else
			{
			if( userSpecificationItem.isPossessive() )
				{
				if( anotherPrimarySpecificationItem != null &&
				!userGeneralizationWordItem.hasFeminineOrMasculineProperNounEnding() &&
				// Ask questions about generalization word with possessive primary specification
				askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( false, false, isArchivedAssignment, false, isSpanishCurrentLanguage, false, userSpecificationItem.specificationCollectionNr(), userSpecificationItem, anotherPrimarySpecificationItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with possessive user specification as primary specification" );
				}
			else
				{
				if( userGeneralizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
				( secondarySpecificationItem = userSpecificationWordItem.firstNonCompoundUserSpecificationItem() ) != null &&
				( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
				// Question part doesn't exist
				!userGeneralizationWordItem.hasJustification( userSpecificationItem, null, secondarySpecificationItem ) )
					{
					// Create question part based on current user question
					// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an invalid assumption word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't create a question part based on current user question" );

					if( ( primarySpecificationItem = userGeneralizationWordItem.suggestiveQuestionAssumptionSecondarySpecificationItem() ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( secondarySpecificationItem = primarySpecificationWordItem.firstNonQuestionSpecificationItem() ) != null &&
					( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
					( correctedSpecificationItem = userGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationItem.specificationCollectionNr(), secondarySpecificationWordItem ) ) != null )
						{
						// Correct invalid assumption by opposite suggestive question
						// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to correct an invalid assumption word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
							return adminItem_.startError( 1, moduleNameString_, "I couldn't create the corrected suggestive question assumption" );

						if( userGeneralizationWordItem.replaceOrDeleteSpecification( correctedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid assumption specification in word \"" + userGeneralizationWordItem.anyWordTypeString() + "\"" );

						if( ( obsoleteJustificationItem = userGeneralizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, createdSpecificationItem ) ) != null &&
						// Remove justification from corrected assumption
						userGeneralizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete an obsolete justification item" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestionsWithQuestionAsPrimarySpecification( boolean isSpanishCurrentLanguage, WordItem generalizationWordItem )
		{
		boolean isArchivedAssignment;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem() ) != null )
			{
			do	{
				if( ( !isSpanishCurrentLanguage ||
				primarySpecificationItem.isSelfGeneratedQuestion() ) &&

				( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
				( secondarySpecificationItem = primarySpecificationWordItem.firstNonCompoundUserSpecificationItem() ) != null &&
				// Question part doesn't exist
				!generalizationWordItem.hasJustification( primarySpecificationItem, null, secondarySpecificationItem ) &&

				( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
					{
					isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

					// Ask a specification substitution question
					// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with a question as primary specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem.nextExclusiveNounSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( boolean hasFemaleUserSpecificationWord, boolean hasUserSpecificationNonCompoundCollection, boolean isArchivedAssignment, boolean isArchivedUserAssignment, boolean isSpanishCurrentLanguage, boolean isUserSpecificationWordSpanishAmbiguous, int userSpecificationCollectionNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasFoundAnswerToQuestion;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean isOlderExistingPrimarySpecificationSpanishAmbiguous = false;
		boolean isOlderPrimarySpecification;
		boolean isUserGeneralizationWord;
		short generalizationWordTypeNr;
		int exclusiveSecondarySpecificationCollectionNr;
		int primarySpecificationCollectionNr;
		SpecificationItem exclusiveSecondarySpecificationItem;
		SpecificationItem existingPrimarySpecificationItem = null;
		WordItem anotherPrimarySpecificationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem compoundGeneralizationWordItem = null;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( anotherPrimarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given another primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		// Question about generalization word
		if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem.specificationWordItem() ) != null &&
		( exclusiveSecondarySpecificationItem = anotherPrimarySpecificationWordItem.firstExclusiveSpecificationItem() ) != null )
			{
			hasFoundAnswerToQuestion = generalizationWordItem.hasFoundAnswerToQuestion();
			hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();
			primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

			if( isSpanishCurrentLanguage &&
			( existingPrimarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, primarySpecificationCollectionNr, null ) ) != null &&
			existingPrimarySpecificationItem.isOlderItem() &&
			existingPrimarySpecificationItem.isHiddenSpanishSpecification() )
				isOlderExistingPrimarySpecificationSpanishAmbiguous = true;

			if( !isOlderExistingPrimarySpecificationSpanishAmbiguous ||

			// The rest of this condition is typical for the Spanish language
			isUserSpecificationWordSpanishAmbiguous ||

			( hasFemaleUserSpecificationWord &&
			!primarySpecificationItem.isSelfGeneratedAssumption() ) )
				{
				do	{
					// Current question part doesn't exist
					if( !generalizationWordItem.hasJustification( primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem ) &&
					( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
					( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null )
						{
						if( isSpanishCurrentLanguage )
							compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem.compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr );

						if( ( !isSpanishCurrentLanguage &&
						// User specification doesn't exist
						generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) == null ) ||

						// The rest of this condition is typical for the Spanish language
						( isSpanishCurrentLanguage &&

						( ( compoundGeneralizationWordItem == null &&

						( hasFemaleUserSpecificationWord ||
						hasPrimarySpecificationCompoundCollection ) ) ||

						( ( ( !isOlderPrimarySpecification &&
						isUserGeneralizationWord ) ||

						( hasFemaleUserSpecificationWord &&
						!isOlderPrimarySpecification ) ||

						( hasPrimarySpecificationCompoundCollection &&
						!hasUserSpecificationNonCompoundCollection &&
						existingPrimarySpecificationItem == null ) ||

						( !hasFoundAnswerToQuestion &&
						!isArchivedUserAssignment &&
						isOlderExistingPrimarySpecificationSpanishAmbiguous ) ) &&

						( compoundGeneralizationWordItem == anotherPrimarySpecificationWordItem ||
						anotherPrimarySpecificationWordItem.hasCollectionNr( exclusiveSecondarySpecificationCollectionNr ) ) ) ) &&

						// Negative specification doesn't exist
						generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, null ) == null ) )
							{
							// Ask a specification substitution question
							// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
							if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveNounSpecificationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( boolean isArchivedAssignment, short generalizationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem userSpecificationWordItem )
		{
		SpecificationItem exclusiveSecondarySpecificationItem;
		WordItem exclusiveSecondarySpecificationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( userSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification word item is undefined" );

		// Question about relation word
		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem.firstExclusiveSpecificationItem() ) != null )
			{
			do	{
				if( !generalizationWordItem.hasJustification( userSpecificationItem, null, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null &&

				// Ask a specification substitution question
				// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveNounSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem createdNewSpecificationItem;
		SpecificationItem createdOldSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem negativeSpecificationItem;
		SpecificationItem oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem definitionSpecificationWordItem;
		WordItem oldSuggestiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( !secondarySpecificationItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is no assignment" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		// Previously, a suggestive question assumption was made.
		// However, now a question with the opposite specification is asked.
		// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem.suggestiveQuestionAssumptionSecondarySpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		// Replace created old specification by created new specification in justifications of involved word
		if( generalizationWordItem.updateSpecificationOfJustificationsInWord( true, createdOldSpecificationItem, createdNewSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to update a specification in justifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ != null &&
		( definitionSpecificationWordItem = firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_.specificationWordItem() ) != null )
			{
			// Add justification to corrected assumption by opposite suggestive question (First - Old)
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem.firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != null &&
			addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdOldSpecificationItem, null, definitionSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the first corrected assumption specification" );

			// Add justification to corrected assumption by opposite suggestive question (First - New)
			if( ( definitionSpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != null &&
			addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdNewSpecificationItem, null, definitionSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add another specification substitution justification to the first corrected assumption specification" );
			}

		if( secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ != null &&
		( definitionSpecificationWordItem = secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_.specificationWordItem() ) != null )
			{
			// Add justification to corrected assumption by opposite suggestive question (Second - Old)
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem.firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != null &&
			addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdOldSpecificationItem, null, definitionSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the second corrected assumption specification" );

			// Add justification to corrected assumption by opposite suggestive question (Second - New)
			if( ( definitionSpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, false, false, definitionSpecificationWordItem ) ) != null &&
			addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_, createdNewSpecificationItem, null, definitionSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add another specification substitution justification to the second corrected assumption specification" );
			}

		// Replace negative assumption
		if( ( negativeSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, isPossessive, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
			{
			// Write negative assumption
			if( generalizationWordItem.writeUpdatedSpecification( false, false, true, false, false, negativeSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			// Replace corrected negative assumption
			if( generalizationWordItem.replaceOrDeleteSpecification( negativeSpecificationItem, secondarySpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative specification" );
			}

		return Constants.RESULT_OK;
		}

	private byte drawCompoundSpecificationSubstitutionConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isSpanishCurrentLanguage;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		WordItem compoundGeneralizationWordItem = null;
		WordItem currentSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonPossessiveDefinitionSpecificationItem( false ) ) != null )
			{
			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage ||
			( compoundGeneralizationWordItem = specificationWordItem.compoundGeneralizationWordItem( primarySpecificationItem.specificationCollectionNr() ) ) != null )
				{
				do	{
					// Select non-possessive definition
					if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
					currentSpecificationWordItem != generalizationWordItem &&
					compoundGeneralizationWordItem != currentSpecificationWordItem &&
					!currentSpecificationItem.isExclusiveSpecification() &&
					// Typical for Spanish
					currentSpecificationItem.isUserSpecification() &&
					currentSpecificationItem.hasNonCompoundSpecificationCollection() )
						{
						// Draw a compound specification substitution conclusion
						// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
						//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( !isSpanishCurrentLanguage &&
						( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
						// Make generalization assumption
						makeGeneralizationAssumption( false, isArchivedAssignment, specificationWordTypeNr, generalizationWordTypeNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, currentSpecificationItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + specificationWordItem.anyWordTypeString() + "\" and specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem.nextNonPossessiveDefinitionSpecificationItem( false ) ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte drawReversibleConclusions( boolean hasCreatedSpecification, SpecificationItem currentSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		boolean hasNonCompoundSecondarySpecificationCollection;
		boolean hasNonCompoundUserSpecificationCollection = false;
		boolean hasUserGeneralizationWordCurrentlyConfirmedSpecification = false;
		boolean hasUserSpecificationAssumptionLevel = false;
		boolean hasUserSpecificationRelationContext = false;
		boolean isActiveUserAssignment = false;
		boolean isArchivedAssignment;
		boolean isArchivedUserAssignment = false;
		boolean isAssignment;
		boolean isExclusiveGeneralizationAssignment;
		boolean isExclusiveSecondarySpecification;
		boolean isIncrementingOrderNr;
		boolean isOlderAlternativeSpecification;
		boolean isOlderExistingSpecification;
		boolean isOlderSecondarySpecification;
		boolean isOlderUserSpecification;
		boolean isPossessiveSecondarySpecification;
		boolean isPossessiveUserSpecification = false;
		boolean isSecondaryArchivedAssignment;
		boolean isSecondarySelfGeneratedAssumption;
		boolean isSecondarySelfGeneratedSpecification;
		boolean isSecondarySpecificationWordSpanishAmbiguous = false;
		boolean isSecondaryUserAssignment;
		boolean isSpanishCurrentLanguage;
		boolean isSpanishSpecificationFemaleWord = false;
		boolean isUniqueUserRelation;
		boolean isUserExistingSpecification;
		boolean isUserGeneralizationWord;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		short secondaryGeneralizationWordTypeNr;
		short secondarySpecificationWordTypeNr;
		short secondaryRelationWordTypeNr;
		int foundRelationContextNr;
		int nonCompoundSpanishSpecificationCollectionNr;
		int relationContextNr;
		int secondaryRelationContextNr;
		int secondarySpecificationCollectionNr;
		int spanishAmbiguousRelationContextNr = Constants.NO_CONTEXT_NR;
		int spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int userSpecificationCollectionNr;
		JustificationItem assumptionJustificationItem;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem olderJustificationItem;
		SpecificationItem alternativeSpecificationItem;
		SpecificationItem assignmentSpecificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem collectedSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem firstUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem spanishDefinitionSpecificationItem;
		SpecificationItem spanishMasculineSpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem unhiddenSpanishSpecificationItem = null;
		SpecificationItem userSpecificationItem;
		WordItem currentContextWordItem;
		WordItem firstContextWordItem;
		WordItem oppositePossessiveDefinitionSpecificationWordItem;
		WordItem spanishFeminineSpecificationWordItem = null;
		WordItem spanishMasculineSpecificationWordItem = null;
		WordItem secondarySpecificationWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( currentSecondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( firstContextWordItem = GlobalVariables.firstContextWordItem ) != null &&
		( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
		( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			secondarySpecificationWordTypeNr = currentSecondarySpecificationItem.specificationWordTypeNr();

			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();

			hasNonCompoundUserSpecificationCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			hasUserGeneralizationWordCurrentlyConfirmedSpecification = userGeneralizationWordItem.hasCurrentlyConfirmedSpecification();
			hasUserSpecificationAssumptionLevel = userSpecificationItem.hasAssumptionLevel();
			hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			if( isPossessiveUserSpecification )
				{
				if( generalizationWordItem.isUserGeneralizationWord )
					{
					// Typical for Chinese: Skip possessive inactive assignment
					if( !userSpecificationItem.isInactiveAssignment() )
						{
						currentContextWordItem = firstContextWordItem;

						// Do for all context words
						do	{
							if( currentContextWordItem.isUserRelationWord )
								{
								if( ( existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, false, userSpecificationWordItem, generalizationWordItem ) ) == null ||

								( existingSpecificationItem.isSelfGenerated() &&
								// Justification doesn't exist in specification
								existingSpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null ) )
									{
									// Typical for Spanish
									if( isSpanishCurrentLanguage ||

									( !hasNonCompoundUserSpecificationCollection &&
									existingSpecificationItem == null ) ||

									( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
									// Self-generated conclusion with different relation word
									currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, null ) != null ) ||

									( existingSpecificationItem != null &&
									// Conclusion doesn't exist
									currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, null ) == null ) ||

									( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
									generalizationWordItem.firstActiveQuestionSpecificationItem() != null ) &&

									// Assumption doesn't exist
									currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, userSpecificationWordItem, null ) == null ) )
										{
										isAssignment = false;
										isArchivedAssignment = false;

										// Typical for Spanish
										if( isSpanishCurrentLanguage )
											{
											spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
											spanishAmbiguousRelationContextNr = Constants.NO_CONTEXT_NR;
											unhiddenSpanishSpecificationItem = null;

											if( hasNonCompoundUserSpecificationCollection )
												{
												if( !currentContextWordItem.hasNegativeSpecification() ||
												currentContextWordItem.hasFeminineProperNounEnding() )
													spanishAmbiguousSpecificationCollectionNr = userSpecificationCollectionNr;
												}
											else
												{
												if( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
												existingSpecificationItem == null &&
												( unhiddenSpanishSpecificationItem = currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, null ) ) != null &&
												unhiddenSpanishSpecificationItem.isSelfGeneratedConclusion() )
													spanishAmbiguousRelationContextNr = unhiddenSpanishSpecificationItem.relationContextNr();
												}
											}

										// Similar specification already exists as assignment
										// So, make this conclusion an assignment too
										if( existingSpecificationItem == null &&
										( assignmentSpecificationItem = currentContextWordItem.firstAssignmentItem( false, false, Constants.NO_CONTEXT_NR, userSpecificationWordItem ) ) != null )
											{
											isAssignment = true;
											isArchivedAssignment = assignmentSpecificationItem.isArchivedAssignment();
											}

										// Draw reversible user conclusion
										if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, spanishAmbiguousRelationContextNr, null, null, userSpecificationItem, null, currentContextWordItem, userSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible user conclusion in word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );

										// Typical for Spanish
										if( isUserSpecificationWordSpanishAmbiguous )
											{
											if( spanishAmbiguousRelationContextNr == Constants.NO_CONTEXT_NR )
												{
												if( currentContextWordItem.hasFeminineProperNounEnding() &&
												( spanishFeminineSpecificationWordItem = userSpecificationWordItem.feminineCollectionWordItem() ) != null &&
												( spanishDefinitionSpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, false, spanishFeminineSpecificationWordItem ) ) != null &&
												currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, spanishDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, spanishFeminineSpecificationWordItem ) == null &&

												// Draw Spanish feminine reversible user conclusion
												adminItem_.addSelfGeneratedSpecification( true, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, userSpecificationItem, spanishDefinitionSpecificationItem, currentContextWordItem, spanishFeminineSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish feminine reversible user conclusion in word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + spanishFeminineSpecificationWordItem.anyWordTypeString() + "\"" );
												}
											else
												{
												// Typical for Spanish. Test files: "Complejo (12)"
												if( unhiddenSpanishSpecificationItem != null &&
												( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
												( olderJustificationItem = unhiddenSpanishSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) != null &&
												( assumptionSpecificationItem = currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, userSpecificationWordItem, null ) ) != null )
													{
													// Add a relation context to the assumption specification
													if( generalizationWordItem.addContext( Constants.WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, assumptionSpecificationItem.relationContextNr(), Constants.NO_COLLECTION_NR, userSpecificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

													// Add reversible justification to the assumption specification
													if( addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, assumptionSpecificationItem, null, null, olderJustificationItem.secondarySpecificationItem(), currentContextWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the assumption specification" );

													// Add reversible justification to the created specification
													if( addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, createdSpecificationItem, null, null, olderJustificationItem.secondarySpecificationItem(), currentContextWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the created specification" );

													// Replace conclusion specification
													if( currentContextWordItem.replaceOrDeleteSpecification( unhiddenSpanishSpecificationItem.updatedSpecificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace the unhidden Spanish specification by the created specification in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										else
											{
											if( existingSpecificationItem != null &&
											( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
											( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
											!foundSpecificationItem.isReplacedItem() &&
											// Replace an assumption specification by a conclusion specification in justifications of involved word
											currentContextWordItem.updateSpecificationOfJustificationsInWord( false, foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in a justification in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								}
							}
						while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
						}
					}
				else
					{
					if( isSpanishCurrentLanguage &&
					// Draw proper noun part-of conclusions
					drawProperNounPartOfConclusions( false, false, false, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a proper noun 'part of' conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			else	// Not possessive user specification
				{
				if( userSpecificationItem.isActiveAssignment() )
					isActiveUserAssignment = true;
				else
					{
					if( userSpecificationItem.isArchivedAssignment() )
						isArchivedUserAssignment = true;
					}

				if( !isUserGeneralizationWord &&
				hasUserSpecificationRelationContext &&
				( existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, userSpecificationWordItem, userGeneralizationWordItem ) ) != null &&
				// Justification doesn't exist in specification
				existingSpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null &&

				// Older specification without specification collection
				( !existingSpecificationItem.hasSpecificationCollection() ||
				existingSpecificationItem.isFirstJustificiationReversibleAssumption() ) )
					{
					// Draw reversible user conclusion
					if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, userSpecificationItem, null, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible user conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );

					// Cleanup unreferenced justifications
					if( isActiveUserAssignment &&
					existingSpecificationItem.isConcludedAssumption() &&
					generalizationWordItem.cleanupUnreferencedJustifications() != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to clean up unreferenced justifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}

			// Do for all selected specifications
			do	{
				hasNonCompoundSecondarySpecificationCollection = currentSecondarySpecificationItem.hasNonCompoundSpecificationCollection();
				isOlderSecondarySpecification = currentSecondarySpecificationItem.isOlderItem();
				isPossessiveSecondarySpecification = currentSecondarySpecificationItem.isPossessive();
				isSecondaryArchivedAssignment = currentSecondarySpecificationItem.isArchivedAssignment();
				isSecondarySelfGeneratedAssumption = currentSecondarySpecificationItem.isSelfGeneratedAssumption();
				isSecondarySelfGeneratedSpecification = currentSecondarySpecificationItem.isSelfGenerated();
				isSecondarySpecificationWordSpanishAmbiguous = currentSecondarySpecificationItem.isSpecificationWordSpanishAmbiguous();
				secondaryGeneralizationWordTypeNr = currentSecondarySpecificationItem.generalizationWordTypeNr();
				secondarySpecificationWordTypeNr = currentSecondarySpecificationItem.specificationWordTypeNr();
				secondaryRelationWordTypeNr = currentSecondarySpecificationItem.relationWordTypeNr();
				secondarySpecificationCollectionNr = currentSecondarySpecificationItem.specificationCollectionNr();
				secondaryRelationContextNr = currentSecondarySpecificationItem.relationContextNr();

				if( ( secondarySpecificationWordItem = currentSecondarySpecificationItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The secondary specification item has no specification word" );

				if( !isOlderSecondarySpecification &&
				( obsoleteSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isPossessiveSecondarySpecification, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != null &&
				obsoleteSpecificationItem.isOlderItem() &&
				obsoleteSpecificationItem.isSelfGenerated() &&
				// Skip all assignments
				generalizationWordItem.firstAssignmentItem( isPossessiveSecondarySpecification, false, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) == null )
					{
					// Write assumption/conclusion without relation word
					if( generalizationWordItem.writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					// Replace assumption/conclusion without relation word
					if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, currentSecondarySpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
					}

				if( isPossessiveSecondarySpecification )
					{
					if( isSecondarySelfGeneratedSpecification )
						{
						if( isUserGeneralizationWord ||
						secondarySpecificationCollectionNr == Constants.NO_COLLECTION_NR )
							{
							if( !isSpanishCurrentLanguage ||
							!isSecondarySpecificationWordSpanishAmbiguous )
								{
								currentContextWordItem = firstContextWordItem;

								// Do for all context words
								do	{
									if( currentContextWordItem != generalizationWordItem &&
									( existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, secondarySpecificationWordItem, generalizationWordItem ) ) != null )
										{
										// Found user specification, force self-generated assumption
										if( existingSpecificationItem.isUserSpecification() &&
										( tempSpecificationItem = currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, true, secondarySpecificationWordItem, generalizationWordItem ) ) != null )
											existingSpecificationItem = tempSpecificationItem;

										currentSecondarySpecificationItem = currentSecondarySpecificationItem.updatedSpecificationItem();

										if( hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation )
											{
											if( currentContextWordItem.isUserRelationWord &&
											( obsoleteJustificationItem = currentSecondarySpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, currentContextWordItem ) ) != null )
												{
												isIncrementingOrderNr = ( currentSecondarySpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, generalizationWordItem ) == null );

												if( ( justificationResult = generalizationWordItem.addJustification( false, false, isIncrementingOrderNr, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, userSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible assumption justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't find or create a reversible assumption justification" );

												if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, currentSecondarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete reversible assumption justification of the current secondary specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											{
											if( existingSpecificationItem.hasCurrentCreationSentenceNr() &&
											existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
											existingSpecificationItem.secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == null &&
											// Relation context exists in word
											currentContextWordItem.hasContextInWord( secondaryRelationContextNr, secondarySpecificationWordItem ) &&

											// Justification doesn't exist in specification
											( ( olderJustificationItem = existingSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ) ) == null ||
											olderJustificationItem.secondaryGeneralizationWordItem() != generalizationWordItem ) &&

											// Add reversible justification
											addJustificationToSpecification( false, false, false, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, existingSpecificationItem, null, null, currentSecondarySpecificationItem, currentContextWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
											}
										}
									}
								while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
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
									currentSecondarySpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null &&
									// Draw Spanish non-compound reversible conclusion
									adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, secondaryRelationWordTypeNr, secondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, userSpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible specification from word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									if( !hasNonCompoundSecondarySpecificationCollection &&
									hasUserGeneralizationWordCurrentlyConfirmedSpecification &&
									!hasUserSpecificationAssumptionLevel &&
									secondarySpecificationWordItem == userSpecificationWordItem &&
									currentSecondarySpecificationItem.hasOnlyOneRelationWord() &&
									!currentSecondarySpecificationItem.isConcludedAssumption() )
										{
										if( currentSecondarySpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to mark the current secondary specification as concluded assumption" );

										// Write concluded assumption
										if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, currentSecondarySpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to write a concluded assumption" );
										}
									}
								}

							// Typical for Spanish
							if( hasNonCompoundSecondarySpecificationCollection &&
							hasUserSpecificationRelationContext &&
							!isPossessiveUserSpecification &&
							isUserSpecificationWordSpanishAmbiguous &&
							// Specification doesn't exist
							generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, userSpecificationWordItem ) == null &&

							// Draw reversible conclusion
							adminItem_.addSelfGeneratedSpecification( false, false, isArchivedUserAssignment, false, false, false, false, true, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, userSpecificationItem, null, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible user conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );
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
								currentContextWordItem.isUserRelationWord &&
								currentContextWordItem.hasNegativeSpecification() &&
								// Draw a specification substitution conclusion or ask a question
								adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, false, false, Constants.NO_QUESTION_PARAMETER, secondaryGeneralizationWordTypeNr, secondarySpecificationWordTypeNr, secondaryRelationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
							}
						}
					}
				else	// Not possessive current secondary specification
					{
					isExclusiveSecondarySpecification = currentSecondarySpecificationItem.isExclusiveSpecification();
					isSecondaryUserAssignment = currentSecondarySpecificationItem.isUserAssignment();

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
							currentSecondarySpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null )
								{
								// Add Spanish reversible user justification
								if( addJustificationToSpecification( false, false, hasNonCompoundSecondarySpecificationCollection, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, null, null, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish reversible user justification to the current secondary specification" );

								currentSecondarySpecificationItem = currentSecondarySpecificationItem.updatedSpecificationItem();
								}
							}
						else
							{
							if( isPossessiveUserSpecification &&
							isSecondarySelfGeneratedAssumption )
								{
								if( ( hasUserGeneralizationWordCurrentlyConfirmedSpecification ||

								( ( existingSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, secondarySpecificationWordItem, null ) ) != null &&
								// Justification doesn't exist in specification
								existingSpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null ) ) &&

								// Conclusion doesn't exist
								generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem, userGeneralizationWordItem ) == null )
									{
									relationContextNr = Constants.NO_CONTEXT_NR;
									conclusionSpecificationItem = null;

									if( hasNonCompoundSecondarySpecificationCollection )
										{
										if( hasUserGeneralizationWordCurrentlyConfirmedSpecification )
											{
											if( ( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem, null ) ) == null )
												{
												if( ( foundRelationContextNr = userGeneralizationWordItem.contextNr( secondarySpecificationWordItem ) ) > Constants.NO_CONTEXT_NR &&
												adminItem_.nContextWords( foundRelationContextNr, secondarySpecificationWordItem ) > 1 )
													{
													if( ( contextResult = adminItem_.getRelationContext( false, false, false, false, generalizationWordItem, secondarySpecificationWordItem, userGeneralizationWordItem, null ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to get a relation context" );

													relationContextNr = contextResult.contextNr;
													}
												}
											else
												relationContextNr = conclusionSpecificationItem.relationContextNr();
											}
										else
											{
											if( ( foundRelationContextNr = userGeneralizationWordItem.contextNr( secondarySpecificationWordItem ) ) > Constants.NO_CONTEXT_NR )
												relationContextNr = foundRelationContextNr;
											}
										}

									// Draw reversible user conclusion
									if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, hasUserGeneralizationWordCurrentlyConfirmedSpecification, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, secondaryRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, relationContextNr, null, null, userSpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user conclusion from word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

									if( conclusionSpecificationItem != null &&
									// Confirmed all relation words
									conclusionSpecificationItem.nRelationContextWords() == currentSecondarySpecificationItem.nRelationContextWords() &&
									// Replace assumption by conclusion
									generalizationWordItem.replaceOrDeleteSpecification( currentSecondarySpecificationItem, conclusionSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
									}
								}
							}
						}

					isExclusiveGeneralizationAssignment = ( isSecondaryUserAssignment &&
															currentSecondarySpecificationItem.isExclusiveGeneralization() );
					currentContextWordItem = firstContextWordItem;

					// Typical for Spanish
					if( isSpanishCurrentLanguage )
						{
						isSpanishSpecificationFemaleWord = secondarySpecificationWordItem.isFemale();
						spanishMasculineSpecificationWordItem = ( isSpanishSpecificationFemaleWord ? secondarySpecificationWordItem.masculineCollectionWordItem() : null );

						if( !isSecondarySpecificationWordSpanishAmbiguous &&
						spanishMasculineSpecificationWordItem != null &&
						generalizationWordItem.firstAssignmentOrSpecificationItem( false, isSecondaryArchivedAssignment, true, false, false, spanishMasculineSpecificationWordItem ) == null &&
						( primarySpecificationItem = spanishMasculineSpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, secondarySpecificationWordItem ) ) != null &&

						// Draw negative Spanish conclusion
						adminItem_.drawNegativeConclusion( isSecondaryArchivedAssignment, Constants.WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, ( isActiveUserAssignment && currentSecondarySpecificationItem == userSpecificationItem ? adminItem_.simpleUserSpecificationItem() : currentSecondarySpecificationItem ), null, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}

					// Do for all context words
					do	{
						if( currentContextWordItem != generalizationWordItem &&
						// Relation context exists in word
						currentContextWordItem.hasContextInWord( secondaryRelationContextNr, secondarySpecificationWordItem ) )
							{
							isOlderExistingSpecification = false;
							isUserExistingSpecification = false;

							if( ( existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, isSecondaryArchivedAssignment, false, true, secondarySpecificationWordItem, generalizationWordItem ) ) != null )
								{
								isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
								isUserExistingSpecification = existingSpecificationItem.isUserSpecification();
								}

							if( isExclusiveSecondarySpecification )
								{
								// Specification doesn't exist
								if( existingSpecificationItem == null &&
								// Draw exclusive reversible conclusion
								adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, secondaryRelationWordTypeNr, secondarySpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem, null, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible specification from current context word \"" + currentContextWordItem.anyWordTypeString() + "\" to specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								firstUserSpecificationItem = currentContextWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

								isOlderUserSpecification = ( existingSpecificationItem == null &&
															firstUserSpecificationItem != null );

								alternativeSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, secondarySpecificationWordItem, currentContextWordItem );

								isOlderAlternativeSpecification = ( alternativeSpecificationItem != null &&
																	alternativeSpecificationItem.isOlderItem() );

								if( isOlderUserSpecification ||
								firstUserSpecificationItem == null ||

								// Typical for Spanish
								( isSpanishCurrentLanguage &&
								currentSecondarySpecificationItem != alternativeSpecificationItem &&

								( !isOlderExistingSpecification ||
								!isOlderAlternativeSpecification ) ) )
									{
									if( existingSpecificationItem == null ||

									( currentSecondarySpecificationItem == alternativeSpecificationItem &&
									// Justification doesn't exist in specification
									existingSpecificationItem.secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == null ) )
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
																	adminItem_.isUniqueUserRelation() );

											if( isSpanishCurrentLanguage )
												spanishAmbiguousSpecificationCollectionNr = ( !isSpanishSpecificationFemaleWord &&
																							alternativeSpecificationItem == null ? secondarySpecificationCollectionNr :

																							( existingSpecificationItem != null &&

																							( existingSpecificationItem.wasHiddenSpanishSpecification() ||

																							( !hasNonCompoundSecondarySpecificationCollection &&
																							!isSpanishSpecificationFemaleWord &&
																							existingSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ?
																								existingSpecificationItem.specificationCollectionNr() : Constants.NO_COLLECTION_NR ) );

											// Draw reversible conclusion
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isSecondaryArchivedAssignment, false, false, false, false, false, false, false, true, isUniqueUserRelation, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, secondaryRelationWordTypeNr, secondarySpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem, null, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion in word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
											foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
											currentSecondarySpecificationItem = currentSecondarySpecificationItem.updatedSpecificationItem();

											if( !isSpanishCurrentLanguage &&
											!isOlderUserSpecification &&
											isSecondarySelfGeneratedSpecification &&
											( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem.updatedSpecificationItem() : createdSpecificationItem ) ) != null &&
											// Justification doesn't exist in specification
											currentSecondarySpecificationItem.secondarySpecificationJustificationItem( createdOrFoundSpecificationItem ) == null &&
											createdOrFoundSpecificationItem.hasNonCompoundSpecificationCollection() &&
											// Add reversible user justification
											addJustificationToSpecification( false, currentContextWordItem.isUserRelationWord, false, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, null, null, createdOrFoundSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

											if( createdSpecificationItem != null )
												{
												// Created, not found
												if( foundSpecificationItem == null )
													{
													// Typical for Spanish
													if( isSpanishCurrentLanguage &&
													!isOlderUserSpecification &&
													( obsoleteSpecificationItem = currentContextWordItem.noRelationContextSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : secondarySpecificationWordItem ) ) ) != null )
														{
														// Write an assumption without relation word
														if( currentContextWordItem.writeUpdatedSpecification( true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

														// Replace an assumption without relation word
														if( currentContextWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
														}
													}
												else	// Created and found
													{
													if( isOlderUserSpecification &&
													!isExclusiveGeneralizationAssignment )
														{
														// Add reversible justification. Test file: "Complex (19 - strange)"
														if( addJustificationToSpecification( false, false, false, true, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, createdSpecificationItem, null, null, foundSpecificationItem, currentContextWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the found specification" );

														if( currentContextWordItem.updateSpecificationOfJustificationsOfInvolvedWords( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
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
											( olderJustificationItem = existingSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ) ) != null &&
											( assumptionJustificationItem = existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != null &&
											( primarySpecificationItem = assumptionJustificationItem.primarySpecificationItem() ) != null &&
											( secondarySpecificationItem = olderJustificationItem.secondarySpecificationItem() ) != null &&
											( foundSpecificationItem = assumptionJustificationItem.secondarySpecificationItem() ) != null &&
											!foundSpecificationItem.isOlderItem() &&
											// Justification doesn't exist in specification
											foundSpecificationItem.secondarySpecificationJustificationItem( secondarySpecificationItem ) == null &&

											// Add opposite possessive justification
											addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, foundSpecificationItem, primarySpecificationItem, null, secondarySpecificationItem, currentContextWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the found specification" );
											}
										else
											{
											// Typical for Spanish
											if( isSpanishCurrentLanguage &&
											alternativeSpecificationItem != null )
												{
												if( isOlderSecondarySpecification )
													{
													if( isSecondarySpecificationWordSpanishAmbiguous &&
													!hasNonCompoundSecondarySpecificationCollection &&
													// Justification doesn't exist in specification
													alternativeSpecificationItem.secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == null &&

													// Add Spanish justification (self-generated conclusion)
													addJustificationToSpecification( false, false, false, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, alternativeSpecificationItem, null, null, currentSecondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the found specification" );
													}
												else
													{
													if( !isOlderExistingSpecification &&
													spanishMasculineSpecificationWordItem != null &&
													( spanishMasculineSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != null &&
													( obsoleteJustificationItem = spanishMasculineSpecificationItem.secondarySpecificationJustificationItem( alternativeSpecificationItem ) ) != null )
														{
														if( ( justificationResult = currentContextWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, currentSecondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

														if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
															return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

														if( currentContextWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, spanishMasculineSpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption justification by a conclusion justification in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										}
									}

								if( hasNonCompoundSecondarySpecificationCollection &&
								isSecondarySelfGeneratedSpecification &&
								!isSpanishCurrentLanguage &&
								alternativeSpecificationItem != null &&
								// Secondary specification word is same as user specification word
								secondarySpecificationWordItem == userSpecificationWordItem &&

								( ( isSecondarySelfGeneratedAssumption &&
								// Justification doesn't exist in specification
								currentSecondarySpecificationItem != alternativeSpecificationItem ) ||

								( !isSecondarySelfGeneratedAssumption &&
								currentSecondarySpecificationItem == alternativeSpecificationItem ) ) &&

								// Make a possessive conditional specification assumption
								adminItem_.makePossessiveConditionalSpecificationAssumption( isSecondaryArchivedAssignment, false, true, Constants.WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, secondaryRelationWordTypeNr, ( isSecondarySelfGeneratedAssumption ? alternativeSpecificationItem : currentSecondarySpecificationItem ), null, currentContextWordItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption during the drawing of a reversible conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								// Alternative specification is same as current secondary specification
								if( currentSecondarySpecificationItem == alternativeSpecificationItem )
									{
									if( isOlderExistingSpecification &&
									isSecondarySelfGeneratedSpecification &&
									existingSpecificationItem != null )
										{
										if( existingSpecificationItem.hasCompoundSpecificationCollection() )
											{
											if( isUserExistingSpecification &&
											isSecondarySelfGeneratedAssumption &&
											!isSecondarySpecificationWordSpanishAmbiguous &&
											// Justification doesn't exist in specification
											currentSecondarySpecificationItem.secondarySpecificationJustificationItem( existingSpecificationItem ) == null &&
											// Add reversible justification
											addJustificationToSpecification( false, false, true, isSpanishCurrentLanguage, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, null, null, existingSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the current secondary specification" );
											}
										else
											{
											if( !hasCreatedSpecification &&
											isOlderSecondarySpecification &&
											firstUserSpecificationItem == null &&
											// Secondary specification word is same as user specification word
											secondarySpecificationWordItem == userSpecificationWordItem &&
											// Justification doesn't exist in specification
											currentSecondarySpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null &&
											( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, secondarySpecificationWordItem, userGeneralizationWordItem ) ) != null )
												{
												// Add reversible user justification
												if( addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, null, null, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

												// Add context
												if( userGeneralizationWordItem.addContext( Constants.WORD_TYPE_PROPER_NOUN, secondarySpecificationWordTypeNr, currentSecondarySpecificationItem.relationContextNr(), Constants.NO_COLLECTION_NR, secondarySpecificationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( collectedSpecificationItem = userGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, currentSecondarySpecificationItem.specificationCollectionNr(), null ) ) != null &&
												( primarySpecificationItem = adminItem_.definitionSpecificationItem( secondarySpecificationWordItem ) ) != null )
													{
													// Add collected opposite possessive justification
													if( addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, currentSecondarySpecificationItem, primarySpecificationItem, null, collectedSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a collected opposite possessive justification to the current secondary specification" );

													// Add opposite possessive conclusion justification
													if( addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, collectedSpecificationItem, primarySpecificationItem, null, conclusionSpecificationItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
													}
												}
											}
										}
									}
								else	// Alternative is different from current secondary
									{
									if( hasNonCompoundSecondarySpecificationCollection )
										{
										if( alternativeSpecificationItem != null )
											{
											// Secondary specification word is same as user specification word
											if( secondarySpecificationWordItem == userSpecificationWordItem )
												{
												if( isSpanishCurrentLanguage )
													{
													// Typical for Spanish
													if( currentSecondarySpecificationItem.hasOnlyOneRelationWord() &&
													// Replace assumption by conclusion
													generalizationWordItem.replaceOrDeleteSpecification( currentSecondarySpecificationItem, alternativeSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace a Spanish assumption specification by a conclusion specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( !isOlderExistingSpecification &&
													alternativeSpecificationItem.isSelfGeneratedAssumption() &&
													( oppositePossessiveDefinitionSpecificationWordItem = adminItem_.oppositePossessiveDefinitionSpecificationWordItem( secondarySpecificationWordItem ) ) != null &&
													( foundSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null &&
													foundSpecificationItem.secondarySpecificationJustificationItem( alternativeSpecificationItem ) != null &&

													// Replace assumption justification by conclusion justification of involved word
													currentContextWordItem.updateSpecificationOfJustificationsInWord( false, alternativeSpecificationItem, currentSecondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in the justification items in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
													}
												}
											else	// Secondary specification word is different from user specification word
												{
												if( ( assumptionJustificationItem = alternativeSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) ) != null &&
												( secondarySpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, false, userSpecificationWordItem, generalizationWordItem ) ) != null &&
												// Justification doesn't exist in specification
												alternativeSpecificationItem.secondarySpecificationJustificationItem( secondarySpecificationItem ) == null &&
												// Add opposite possessive conclusion justification
												addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, alternativeSpecificationItem, assumptionJustificationItem.primarySpecificationItem(), null, secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
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
											if( generalizationWordItem.replaceOrDeleteSpecification( ( isSecondarySelfGeneratedAssumption ? currentSecondarySpecificationItem : alternativeSpecificationItem ), ( isSecondarySelfGeneratedAssumption ? alternativeSpecificationItem : currentSecondarySpecificationItem ) ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption specification by a conclusion specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											currentSecondarySpecificationItem = currentSecondarySpecificationItem.updatedSpecificationItem();
											}
										}
									}

								// Typical for Spanish
								if( isSpanishCurrentLanguage )
									{
									if( isSpanishSpecificationFemaleWord &&
									spanishMasculineSpecificationWordItem != null )
										{
										if( existingSpecificationItem == null )
											{
											if( ( nonCompoundSpanishSpecificationCollectionNr = spanishMasculineSpecificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&

											( ( spanishMasculineSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) == null ||
											spanishMasculineSpecificationItem.isSelfGenerated() ) &&

											// Draw Spanish reversible conclusion
											adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, nonCompoundSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem, null, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish reversible conclusion in word \"" + currentContextWordItem.anyWordTypeString() + "\" and with masculine specification word \"" + spanishMasculineSpecificationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											if( hasGeneralizationWordCurrentlyConfirmedSpecification &&
											isOlderExistingSpecification &&
											!hasUserSpecificationAssumptionLevel &&
											secondarySpecificationWordItem == userSpecificationWordItem &&
											existingSpecificationItem.isSelfGeneratedAssumption() )
												{
												if( existingSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to mark the existing specification as concluded assumption" );

												// Write concluded assumption
												if( currentContextWordItem.writeUpdatedSpecification( true, false, false, false, false, existingSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to write a concluded assumption in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

												if( ( spanishMasculineSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != null )
													{
													if( spanishMasculineSpecificationItem.isHiddenSpanishSpecification() )
														{
														if( !spanishMasculineSpecificationItem.hasCurrentCreationSentenceNr() &&
														// Draw Spanish reversible conclusion (hidden)
														adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, userSpecificationItem, null, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish reversible conclusion in word \"" + currentContextWordItem.anyWordTypeString() + "\" and with masculine specification word \"" + spanishMasculineSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														if( !isActiveUserAssignment &&
														spanishMasculineSpecificationItem.nReversibleConclusionJustifications() == spanishMasculineSpecificationItem.nRelationContextWords() )
															{
															if( spanishMasculineSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to mark the Spanish masculine specification as concluded assumption" );

															// Write concluded assumption
															if( currentContextWordItem.writeUpdatedSpecification( true, false, false, false, false, spanishMasculineSpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to write a concluded assumption in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												}
											}
										}
									else
										{
										if( existingSpecificationItem != null &&
										currentContextWordItem.isUserGeneralizationWord &&
										existingSpecificationItem.hasCompoundSpecificationCollection() &&
										currentSecondarySpecificationItem.isSelfGeneratedConclusion() &&
										( nonCompoundSpanishSpecificationCollectionNr = secondarySpecificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&

										// Add Spanish non-compound reversible conclusion
										adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, secondaryRelationWordTypeNr, secondarySpecificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, nonCompoundSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem, null, currentContextWordItem, secondarySpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish non-compound reversible conclusion in word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
										}
									}

								if( isOlderExistingSpecification &&
								isUserExistingSpecification &&

								( hasNonCompoundUserSpecificationCollection ||
								isActiveUserAssignment ||
								isPossessiveUserSpecification ||
								currentSecondarySpecificationItem != alternativeSpecificationItem ) &&

								isSecondarySelfGeneratedAssumption &&
								!currentSecondarySpecificationItem.isOlderItem() &&
								// Justification doesn't exist in specification
								currentSecondarySpecificationItem.secondarySpecificationJustificationItem( existingSpecificationItem ) == null &&

								// Add reversible user justification
								addJustificationToSpecification( false, false, true, false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, currentSecondarySpecificationItem, null, null, existingSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );
								}
							}
						}
					while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
					}
				}
			while( ( currentSecondarySpecificationItem = currentSecondarySpecificationItem.nextSelectedRelationSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawProperNounPartOfConclusions( boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, boolean isArchivedAssignment, boolean isCheckingForArchivedAssignment, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem spanishRelationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		boolean hasPrimarySpecificationRelationContext;
		boolean isPrimarySpecificationArchivedAssignment;
		boolean isPossessivePrimarySpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		short assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		short partOfAssumptionLevel;
		short primaryAssumptionLevel;
		JustificationItem createdJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem partOfDefinitionSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem spanishUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem currentPossessiveNounWordItem;
		WordItem oppositePossessiveDefinitionSpecificationWordItem;
		WordItem spanishAmbiguousSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		if( ( primarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null ) ) == null ||
		isSpecificationWordSpanishAmbiguous )
			{
			primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isCheckingForArchivedAssignment, false, false, false, specificationWordItem );

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&

			( primarySpecificationItem == null ||
			primarySpecificationItem.isHiddenSpanishSpecification() ) )
				{
				if( spanishRelationWordItem == null )
					{
					// Try to find self-generated question, as alternative to hidden specification
					if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, true, specificationWordItem ) ) != null &&
					!primarySpecificationItem.isExclusiveSpecification() )
						primarySpecificationItem = null;
					}
				else
					{
					if( ( spanishUserSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
					spanishUserSpecificationItem.isPossessive() )
						primarySpecificationItem = spanishUserSpecificationItem;
					else
						primarySpecificationItem = spanishRelationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, true, specificationWordItem, generalizationWordItem );
					}

				if( primarySpecificationItem != null &&
				primarySpecificationItem.isHiddenSpanishSpecification() )
					{
					hasSkippedHiddenSpanishSpecification_ = true;
					primarySpecificationItem = null;
					}
				}
			}
		else
			{
			if( ( primarySpecificationItem.isUserSpecification() ||
			generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation() ) &&

			( tempSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null )
				primarySpecificationItem = tempSpecificationItem;
			}

		if( primarySpecificationItem != null &&
		( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
			{
			hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
			isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();
			isPrimarySpecificationArchivedAssignment = primarySpecificationItem.isArchivedAssignment();
			primaryAssumptionLevel = primarySpecificationItem.assumptionLevel();

			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();

			// Typical for Spanish
			if( adminItem_.isSpanishCurrentLanguage() &&
			specificationWordItem.isFemale() &&
			!adminItem_.isUserQuestion() )
				spanishAmbiguousSpecificationWordItem = specificationWordItem.masculineCollectionWordItem();

			// Do for all possessive noun words
			do	{
				if( ( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem.partOfSpecificationItem( specificationWordItem ) ) == null &&
				spanishAmbiguousSpecificationWordItem != null )
					partOfDefinitionSpecificationItem = currentPossessiveNounWordItem.partOfSpecificationItem( spanishAmbiguousSpecificationWordItem );

				if( partOfDefinitionSpecificationItem != null &&
				// Justification doesn't exist in word
				!generalizationWordItem.hasJustification( primarySpecificationItem, null, partOfDefinitionSpecificationItem ) )
					{
					partOfSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, currentPossessiveNounWordItem );
					partOfAssumptionLevel = ( partOfSpecificationItem != null ? partOfSpecificationItem.assumptionLevel() : Constants.NO_ASSUMPTION_LEVEL );

					if( partOfSpecificationItem == null ||
					primaryAssumptionLevel < partOfAssumptionLevel ||

					( primaryAssumptionLevel == partOfAssumptionLevel &&

					( !hasPrimarySpecificationRelationContext ||
					isPossessivePrimarySpecification ||
					// Typical for Spanish
					!partOfSpecificationItem.hasPossessivePrimarySpecificationJustification() ) ) ||

					// Typical for Spanish
					( hasPrimarySpecificationRelationContext &&

					( ( partOfSpecificationItem.isConcludedAssumption() &&

					( isPossessivePrimarySpecification ||
					hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation ) ) ||

					( isPossessivePrimarySpecification &&
					partOfAssumptionLevel == Constants.NO_ASSUMPTION_LEVEL &&
					primarySpecificationItem.isOlderItem() ) ) ) )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous )
							assumptionLevel = ( hasPrimarySpecificationRelationContext &&
												isPossessivePrimarySpecification &&

												// Primary specification will become an assumption
												// Forcing 'part of' assumption instead conclusion
												( ( partOfSpecificationItem == null &&
												!primarySpecificationItem.hasCurrentCreationSentenceNr() ) ||

												// Primary specification isn't recalculated
												// Forcing 'part of' assumption instead conclusion
												( primaryAssumptionLevel == Constants.NO_ASSUMPTION_LEVEL &&
												primarySpecificationItem.isSelfGeneratedAssumption() ) ) ? 1 : Constants.NO_ASSUMPTION_LEVEL );

						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the challenge document, or http://mafait.org/block1/ and http://mafait.org/block2/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, false, true, false, false, false, false, false, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, partOfDefinitionSpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
							{
							foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

							if( foundSpecificationItem.isSelfGeneratedAssumption() &&
							// Assumption level of hidden Spanish specification isn't calculated. So, calculate now.
							// Recalculate unhidden Spanish assumption
							foundSpecificationItem.recalculateAssumption() != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to calculate an unhidden Spanish assumption" );

							// Unhidden Spanish assumption
							if( primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
							( definitionSpecificationItem = adminItem_.definitionSpecificationItem( currentPossessiveNounWordItem ) ) != null &&
							( oppositePossessiveDefinitionSpecificationWordItem = adminItem_.oppositePossessiveDefinitionSpecificationWordItem( specificationWordItem ) ) != null &&
							( assumptionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, oppositePossessiveDefinitionSpecificationWordItem, spanishRelationWordItem ) ) != null )
								{
								// Add justification (unhidden)
								// Typical for Spanish. Test files: "Complejo (13)" and "Complejo (18)"
								if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, primarySpecificationItem, null, definitionSpecificationItem, null, assumptionSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
									return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

								// Copy specification
								if( ( createAndAssignResult = generalizationWordItem.copySpecificationItem( assumptionSpecificationItem.specificationCollectionNr(), createdJustificationItem, assumptionSpecificationItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to copy the assumption specification with the created justification item" );

								if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
									return adminItem_.startError( 1, moduleNameString_, "I couldn't copy the assumption specification" );

								if( generalizationWordItem.replaceOrDeleteSpecification( assumptionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace the copied assumption specification" );
								}
							}
						}
					}
				}
			while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private static SpecificationItem getAnotherPrimarySpecificationItem( WordItem userSpecificationWordItem )
		{
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem possessiveDefinitionSpecificationItem;
		WordItem currentPossessiveNounWordItem;
		WordItem higherLevelPossessiveSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;

		if( userSpecificationWordItem != null &&
		( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
			{
			// Do for all possessive noun words
			do	{
				// First, try to find a possessive specification on a higher level
				if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
					{
					do	{
						if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null &&
						possessiveSpecificationWordItem.firstSpecificationItem( false, false, false, userSpecificationWordItem ) != null )
							higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveSpecificationWordItem == null &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
						anotherPrimarySpecificationItem = ( higherLevelPossessiveSpecificationWordItem == null ||
															higherLevelPossessiveSpecificationWordItem == possessiveDefinitionSpecificationItem.specificationWordItem() ?
																possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() : possessiveDefinitionSpecificationItem );
					}
				}
			while( anotherPrimarySpecificationItem == null &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );
			}

		return anotherPrimarySpecificationItem;
		}


	// Constructor

	protected AdminReasoningNew( AdminItem adminItem )
		{
		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminReasoningNewVariables()
		{
		hasSkippedHiddenSpanishSpecification_ = false;
		hasSuggestiveQuestionAssumption_ = false;

		adjustedSpanishQuestionSecondarySpecificationItem_ = null;
		firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = null;
		secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationItem_ = null;
		secondaryDefinitionSpecificationItem_ = null;

		adjustedQuestionWordItem_ = null;
		}

	protected boolean hasSuggestiveQuestionAssumption()
		{
		return hasSuggestiveQuestionAssumption_;
		}

	protected byte addJustificationToSpecification( boolean hasFeminineOrMasculineProperNounEnding, boolean isAdvancedAttaching, boolean isForcingNewJustification, boolean isIncrementingOrderNr, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, SpecificationItem destinationSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		short justificationTypeNr;
		JustificationItem createdJustificationItem;
		JustificationResultType justificationResult;

		if( destinationSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given destination specification item is undefined" );

		if( destinationSpecificationItem.isUserSpecification() )
			return adminItem_.startError( 1, moduleNameString_, "The given destination specification item is a user specification" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationItem == null &&
		destinationSpecificationItem.specificationWordItem() != secondarySpecificationItem.specificationWordItem() )
			return adminItem_.startError( 1, moduleNameString_, "The given destination specification has a different specification word than the given secondary specification" );

		justificationTypeNr = ( secondarySpecificationItem.hasAssumptionLevel() ? assumptionJustificationTypeNr : conclusionJustificationTypeNr );

		if( ( justificationResult = generalizationWordItem.addJustification( hasFeminineOrMasculineProperNounEnding, isForcingNewJustification, isIncrementingOrderNr, justificationTypeNr, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

		if( isAdvancedAttaching )
			{
			if( destinationSpecificationItem.attachJustificationToSpecificationAdvanced( createdJustificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the destination specification" );
			}
		else
			{
			if( destinationSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the destination specification" );
			}

		return Constants.RESULT_OK;
		}

	protected byte askQuestions( boolean isCheckingForGapInKnowledge )
		{
		boolean isAllowingSpanishPossessiveSpecification;
		boolean isArchivedUserAssignment;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		int userSpecificationCollectionNr;
		SpecificationItem anotherPrimarySpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem currentTouchedWordItem;
		WordItem userSpecificationWordItem;

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
		userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
		userSpecificationWordItem = userSpecificationItem.specificationWordItem();

		if( userSpecificationItem.isGeneralizationProperNoun() )
			{
			anotherPrimarySpecificationItem = getAnotherPrimarySpecificationItem( userSpecificationWordItem );

			if( askQuestionsAboutSpecificSituations( isArchivedUserAssignment, isSpanishCurrentLanguage, anotherPrimarySpecificationItem, userSpecificationItem, userSpecificationItem.generalizationWordItem(), userSpecificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about specific situations" );
			}

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
															currentTouchedWordItem.isUserRelationWord );

				if( ( primarySpecificationItem = currentTouchedWordItem.firstPrimarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null &&
				askQuestions( isAllowingSpanishPossessiveSpecification, isArchivedUserAssignment, isCheckingForGapInKnowledge, userSpecificationCollectionNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, currentTouchedWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper noun word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );

				if( askQuestionsWithQuestionAsPrimarySpecification( isSpanishCurrentLanguage, currentTouchedWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper noun word \"" + currentTouchedWordItem.anyWordTypeString() + "\" with a question as primary specification" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, int generalizationContextNr, WordItem generalizationWordItem )
		{
		boolean hasFoundAssignment;
		boolean hasMoreOptionsLeft = false;
		SpecificationItem currentAssignmentItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem firstNegativeOnlyOptionLeftAssignmentItem = null;
		SpecificationItem firstOnlyOptionLeftSpecificationItem = null;
		SpecificationItem foundOnlyOptionLeftSpecificationItem = null;
		WordItem specificationWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given specification collection number is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

			do	{
				if( currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
				( currentAssignmentItem = generalizationWordItem.firstSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != null )
					{
					hasFoundAssignment = false;
					specificationWordItem = currentSpecificationItem.specificationWordItem();

					if( currentAssignmentItem.isNegative() &&
					firstNegativeOnlyOptionLeftAssignmentItem == null )
						firstNegativeOnlyOptionLeftAssignmentItem = currentAssignmentItem;

					do	{
						if( currentAssignmentItem.isNegative() &&
						currentAssignmentItem.specificationCollectionNr() == specificationCollectionNr &&
						currentAssignmentItem.specificationWordItem() == specificationWordItem )
							hasFoundAssignment = true;
						}
					while( !hasFoundAssignment &&
					( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );

					if( !hasFoundAssignment )
						{
						if( foundOnlyOptionLeftSpecificationItem == null )
							// Found the first option left
							foundOnlyOptionLeftSpecificationItem = currentSpecificationItem;
						else
							{
							hasMoreOptionsLeft = true;
							// Clear the found option
							foundOnlyOptionLeftSpecificationItem = null;
							}
						}
					}
				}
			while( !hasMoreOptionsLeft &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );

			if( firstNegativeOnlyOptionLeftAssignmentItem != null &&
			foundOnlyOptionLeftSpecificationItem != null &&
			// Draw an only option left conclusion
			adminItem_.addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawReversibleConclusions( boolean hasCreatedSpecification, WordItem generalizationWordItem )
		{
		SpecificationItem secondarySpecificationItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem.isProperNounWord() )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item isn't a proper noun" );

		// Active assignments
		if( ( secondarySpecificationItem = generalizationWordItem.firstSelectedRelationSpecificationItem( true, false ) ) != null &&
		drawReversibleConclusions( hasCreatedSpecification, secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with active assignments" );

		// Archived assignments
		if( ( secondarySpecificationItem = generalizationWordItem.firstSelectedRelationSpecificationItem( true, true ) ) != null &&
		drawReversibleConclusions( hasCreatedSpecification, secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with archived assignments" );

		// Specifications
		if( ( secondarySpecificationItem = generalizationWordItem.firstSelectedRelationSpecificationItem( false, false ) ) != null &&
		drawReversibleConclusions( hasCreatedSpecification, secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with specifications" );

		return Constants.RESULT_OK;
		}

	protected byte drawProperNounPartOfConclusions( boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, boolean isArchivedAssignment, WordItem generalizationProperNounWordItem, WordItem specificationWordItem, WordItem spanishRelationWordItem )
		{
		WordItem currentProperNounWordItem;

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationProperNounWordItem == null ||

		// Try all proper nouns again. Maybe the specification isn't hidden anymore
		( hasSkippedHiddenSpanishSpecification_ &&
		!generalizationProperNounWordItem.isUserGeneralizationWord ) )
			{
			if( ( currentProperNounWordItem = GlobalVariables.firstUserProperNounWordItem ) != null )
				{
				// Do for all user proper noun words
				do	{
					if( currentProperNounWordItem != spanishRelationWordItem &&
					drawProperNounPartOfConclusions( hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, isArchivedAssignment, true, currentProperNounWordItem, specificationWordItem, spanishRelationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a proper noun 'part of' conclusion in proper noun word \"" + currentProperNounWordItem.anyWordTypeString() + "\"" );
					}
				while( ( currentProperNounWordItem = currentProperNounWordItem.nextUserProperNounWordItem ) != null );
				}
			}
		else
			{
			if( drawProperNounPartOfConclusions( false, isArchivedAssignment, isArchivedAssignment, generalizationProperNounWordItem, specificationWordItem, spanishRelationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a proper noun 'part of' conclusion in word \"" + generalizationProperNounWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isArchivedAssignment, boolean isSelfGenerated, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		short assumptionLevel;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem = null;
		WordItem currentGeneralizationWordItem;

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != null &&
				!currentSpecificationItem.hasSpecificationCollection() )
					{
					assumptionLevel = ( secondarySpecificationItem.assumptionLevel() < currentSpecificationItem.assumptionLevel() ?
											currentSpecificationItem.assumptionLevel() : secondarySpecificationItem.assumptionLevel() );

					// Collect generalization words of a specification-generalization definition
					if( adminItem_.collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

					currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

					if( currentSpecificationItem.hasCurrentCreationSentenceNr() &&
					// Draw a specification-generalization conclusion (using current specification)
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, currentSpecificationItem.isExclusiveSpecification(), false, currentSpecificationItem.isNegative(), currentSpecificationItem.isPartOf(), currentSpecificationItem.isPossessive(), false, false, false, true, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, currentSpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSpecificationItem, null, currentSpecificationItem.specificationWordItem(), currentSpecificationItem.generalizationWordItem(), null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the current specification" );

					if( !isSelfGenerated &&
					// Draw a specification-generalization conclusion (using secondary specification)
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, secondarySpecificationItem.isExclusiveSpecification(), false, secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPartOf(), secondarySpecificationItem.isPossessive(), false, false, false, true, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, secondarySpecificationItem.specificationWordItem(), secondarySpecificationItem.generalizationWordItem(), null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the secondary specification" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		int currentSpecificationCollectionNr;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem indirectPrimarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordItem commonWordItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem negativeQuestionSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( userSpecificationItem.hasSpecificationCollection() &&
		( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != null )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
				indirectPrimarySpecificationItem.hasCompoundSpecificationCollection() &&
				( currentSpecificationCollectionNr = currentQuestionSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
					{
					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem.findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

					// Draw an indirectly answered question conclusion
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, indirectPrimarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
					( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
					( negativeQuestionSpecificationWordItem = relatedSpecificationItem.specificationWordItem() ) != null &&
					( commonWordItem = currentQuestionSpecificationWordItem.commonWordItem( currentSpecificationCollectionNr ) ) != null )
						{
						if( commonWordItem.isNounWordSpanishAmbiguous() )
							{
							if( generalizationWordItem.replaceOrDeleteSpecification( currentQuestionSpecificationItem.updatedSpecificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace the answered question by the created specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, commonWordItem ) ) != null &&
							( anotherPrimarySpecificationItem = commonWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, currentQuestionSpecificationWordItem ) ) != null &&
							// Draw a negative assumption
							adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, null, generalizationWordItem, negativeQuestionSpecificationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
							}
						}

					currentQuestionSpecificationItem = adminItem_.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
					}
				else
					currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem();
				}
			while( currentQuestionSpecificationItem != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeGeneralizationAssumption( boolean hasCurrentlyCorrectedAssumptionByKnowledge, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isEveryGeneralization;
		JustificationItem olderGeneralizationAssumptionBySpecificationJustificationItem;
		SpecificationItem olderGeneralizationAssumptionBySpecificationItem;
		CreateAndAssignResultType createAndAssignResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( !primarySpecificationItem.isPartOf() &&
		secondarySpecificationItem.hasCurrentCreationSentenceNr() &&
		// Skip if user specification already exists
		generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) == null &&
		// Skip if justification with primary specification already exists
		generalizationWordItem.primarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem ) == null )
			{
			isEveryGeneralization = ( generalizationWordTypeNr != Constants.WORD_TYPE_PROPER_NOUN );

			// Make a generalization assumption
			// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
			if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			if( ( secondaryDefinitionSpecificationItem_ = createAndAssignResult.createdSpecificationItem ) == null &&
			hasCurrentlyCorrectedAssumptionByKnowledge &&
			( olderGeneralizationAssumptionBySpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
			( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != null &&
			// Replace justification
			generalizationWordItem.replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, createAndAssignResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a reasoning word type" );

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a noun" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( !currentGeneralizationWordItem.hasCollection() &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null )
					{
					// Get secondary specification for justification
					if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null )
						{
						if( !primarySpecificationItem.isSelfGeneratedAssumption() &&
						secondarySpecificationItem.hasCurrentCreationSentenceNr() &&
						// Make generalization assumption
						makeGeneralizationAssumption( false, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						// Get secondary question specification for justification
						if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, true, specificationWordItem ) ) != null &&
						// Draw a specification substitution conclusion or ask a question
						adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		short generalizationWordTypeNr;
		short specificationWordTypeNr;
		short nOptionsLeft = 0;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem primarySpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem onlyOptionLeftSpecificationWordItem = null;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( !userSpecificationItem.hasSpecificationCollection() )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification collection" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != null )
			{
			do	{
				if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, currentQuestionSpecificationWordItem ) == null )
					{
					nOptionsLeft++;
					onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
					}
				}
			while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem() ) != null );

			if( nOptionsLeft == 1 &&
			onlyOptionLeftSpecificationWordItem != null &&
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
				{
				generalizationWordTypeNr = userSpecificationItem.generalizationWordTypeNr();
				specificationWordTypeNr = userSpecificationItem.specificationWordTypeNr();

				// Do for all generalization noun specification words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != null &&
					( anotherPrimarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != null &&
					// Make an only option left assumption
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				while( !GlobalVariables.hasDisplayedWarning &&
				( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		int secondarySpecificationCollectionNr;
		RelatedResultType relatedResult = new RelatedResultType();

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

		// Check if specification doesn't exist yet
		if( generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == null )
			{
			if( secondarySpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
			// Check if a related specification exists
			( relatedResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );

			if( relatedResult.relatedSpecificationItem == null )
				{
				if( generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() )
					{
					if( correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					// Typical for Spanish:
					// Skip making suggestive assumption if specification word is Spanish ambiguous,
					// and if user question has more than one generalization words
					if( !secondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ||
					GlobalVariables.nUserGeneralizationWords == 1 )
						{
						hasSuggestiveQuestionAssumption_ = true;

						// Make a suggestive question assumption
						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a suggestive question assumption to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected CompoundResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		boolean isArchivedAssignment;
		short generalizationWordTypeNr;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem secondarySpecificationWordItem;
		CompoundResultType compoundResult = new CompoundResultType();
		RelatedResultType relatedResult;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( specificationWordItem == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startCompoundResultError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( foundSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, false, false, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != null )
					{
					if( !foundSpecificationItem.isPartOf() )
						{
						isArchivedAssignment = foundSpecificationItem.isArchivedAssignment();

						// Definition
						if( ( generalizationWordTypeNr = foundSpecificationItem.generalizationWordTypeNr() ) != Constants.WORD_TYPE_PROPER_NOUN &&
						secondaryDefinitionSpecificationItem_ != null &&
						foundSpecificationItem.hasCompoundSpecificationCollection() &&
						( secondarySpecificationWordItem = secondaryDefinitionSpecificationItem_.specificationWordItem() ) != null &&
						// Make an exclusive specification substitution definition assumption
						// See paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, foundSpecificationItem.isEveryGeneralization(), false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, null, secondaryDefinitionSpecificationItem_, null, currentGeneralizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to make an exclusive specification substitution definition assumption about word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

						// Draw a compound specification substitution conclusion
						if( adminItem_.userSpecificationItem() != null &&
						drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, foundSpecificationItem.isExclusiveSpecification(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, foundSpecificationItem, currentGeneralizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						// Draw a specification substitution conclusion or ask a question
						if( adminItem_.userSpecificationItem() != null &&
						adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem.hasAssumptionLevel(), isArchivedAssignment, foundSpecificationItem.isExclusiveSpecification(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
						}

					if( !GlobalVariables.hasDisplayedWarning &&
					foundSpecificationItem.hasSpecificationCollection() )
						{
						if( ( relatedResult = currentGeneralizationWordItem.findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );

						if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null )
							{
							// Draw a specification substitution conclusion or ask a question
							if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.hasAssumptionLevel(), relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem.specificationWordItem(), null ) != Constants.RESULT_OK )
								return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( !GlobalVariables.hasDisplayedWarning &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return compoundResult;
		}

	protected WordItem adjustedQuestionWordItem()
		{
		return adjustedQuestionWordItem_;
		}
	};

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
