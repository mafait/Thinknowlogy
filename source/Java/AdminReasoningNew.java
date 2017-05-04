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

class AdminReasoningNew
	{
	// Private constructed variables

	private SpecificationItem adjustedSpanishQuestionSecondarySpecificationItem_;
	private SpecificationItem adjustedSpanishQuestionSpecificationItem_;

	private WordItem adjustedQuestionWordItem_;

	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


	// Private methods

	private byte askQuestions( boolean isAllowingSpanishPossessiveSpecification, boolean isArchivedUserAssignment, int userSpecificationCollectionNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem userSpecificationWordItem )
		{
		boolean hasExclusiveSecondarySpecificationCompoundCollection;
		boolean hasFeminineOrMasculineProperNameEnding;
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
		SpecificationItem adjustedQuestionCompoundSpecificationItem;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem archivedPrimarySpecificationItem;
		SpecificationItem exclusiveSecondarySpecificationItem;
		WordItem collectionWordItem;
		WordItem compoundGeneralizationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		hasFeminineOrMasculineProperNameEnding = generalizationWordItem.hasFeminineOrMasculineProperNameEnding();
		hasUserSpecificationCompoundCollection = userSpecificationItem.hasSpecificationCompoundCollection();
		hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
		isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
		isPossessiveUserSpecification = userSpecificationItem.isPossessive();
		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
		isUsefulUserSpecificationWithoutRelation = ( hasUserSpecificationCompoundCollection &&
													!hasUserSpecificationRelationContext &&
													isUserGeneralizationWord &&

													( isExclusiveUserSpecification ||
													isPossessiveUserSpecification ||
													CommonVariables.nUserSpecificationWords == 1 ) );
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();

		do	{
			if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null )
				{
				hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
				isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();
				isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();

				if( isPossessivePrimarySpecification )
					{
					if( !hasFeminineOrMasculineProperNameEnding )
						{
						archivedPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, Constants.NO_QUESTION_PARAMETER, primarySpecificationWordItem );

						// Typically for the Spanish language
						if( anotherPrimarySpecificationItem != null &&
						// Specification with user specification collection doesn't exist
						generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) == null &&

						askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( adminItem_.hasFemaleUserSpecificationWord(), userSpecificationItem.hasSpecificationNonCompoundCollection(), isArchivedAssignment, isArchivedUserAssignment, true, isUserSpecificationWordSpanishAmbiguous, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != null && archivedPrimarySpecificationItem.isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), anotherPrimarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask Spanish questions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
						}
					}
				else
					{
					hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasSpecificationCompoundCollection();

					if( ( !hasFeminineOrMasculineProperNameEnding ||
					// If gender is known, skip gender-related question parts
					hasPrimarySpecificationCompoundCollection ) &&

					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( exclusiveSecondarySpecificationItem = primarySpecificationWordItem.firstExclusiveNounSpecificationItem() ) != null )
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
							if( isPrimaryUserSpecification )
								{
								if( !isPrimarySpecificationWordSpanishAmbiguous &&
								primarySpecificationItem.isUserAssignment() )
									alternativePrimarySpecificationItem = adminItem_.simpleUserSpecificationItem();
								}
							else
								{
								if( !hasUserSpecificationRelationContext &&
								generalizationWordItem.hasConfirmedSpecificationButNoRelation() )
									alternativePrimarySpecificationItem = userSpecificationItem;
								}
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
									return adminItem_.addError( 1, moduleNameString_, "I failed to ask a Spanish question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
								}
							else
								{
								if( isPrimarySpecificationSelfGeneratedAssumption &&
								( alternativePrimarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, primarySpecificationWordItem, null ) ) != null )
									{
									// Skip if conclusion is not older
									if( !alternativePrimarySpecificationItem.isOlderItem() )
										alternativePrimarySpecificationItem = null;
									}
								}
							}

						do	{
							hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem.hasSpecificationCompoundCollection();

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
								!generalizationWordItem.hasJustification( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, ( alternativePrimarySpecificationItem == null ? primarySpecificationItem : alternativePrimarySpecificationItem ), null, exclusiveSecondarySpecificationItem ) ) ) &&

								( collectionWordItem = exclusiveSecondarySpecificationWordItem.collectionWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null )
									{
									// Adjusted question
									if( alternativePrimarySpecificationItem != null ||

									( hasPrimarySpecificationCompoundCollection &&
									!isUserSpecificationWordSpanishAmbiguous &&

									( adjustedQuestionWordItem_ != null ||
									isCompoundGeneralizationWordSpanishAmbiguous ) ) ||

									( isUserSpecificationWordSpanishAmbiguous &&
									compoundGeneralizationWordItem != primarySpecificationWordItem &&

									( !hasPrimarySpecificationRelationContext ||
									// Same specification but without relation context doesn't exists. Spanish test file: "Esta información es más específico"
									generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationWordItem ) == null ) ) ||

									generalizationWordItem.hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationItem ) )
										{
										// Avoid asking invalid questions:
										// 1) Check for existing knowledge with same specification collection
										if( generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, exclusiveSecondarySpecificationWordItem ) == null &&
										// 2) Check for existing negative specification. Test file: "Skip invalid questions (1)"
										generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, Constants.NO_QUESTION_PARAMETER, collectionWordItem, null ) == null &&
										// 3) Check for existing user specification. Test file: "Skip invalid questions (2)"
										generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, collectionWordItem ) == null )
											{
											// Ask a specification substitution question
											// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( alternativePrimarySpecificationItem == null ? primarySpecificationItem : alternativePrimarySpecificationItem ), null, ( isPrimarySpecificationWordSpanishAmbiguous && isUserGeneralizationWord && adjustedSpanishQuestionSecondarySpecificationItem_ != null ? adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem ), null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );

											// Adjusted question
											if( isPrimarySpecificationWordSpanishAmbiguous &&
											createAndAssignResult.createdSpecificationItem == null &&
											hasExclusiveSecondarySpecificationCompoundCollection &&
											isUserGeneralizationWord )
												{
												if( adjustedSpanishQuestionSecondarySpecificationItem_ == null )
													adjustedSpanishQuestionSecondarySpecificationItem_ = exclusiveSecondarySpecificationItem;
												else
													{
													if( adjustedQuestionWordItem_ == null )
														adjustedSpanishQuestionSpecificationItem_ = generalizationWordItem.firstActiveAssignmentOrSpecificationItem( isArchivedAssignment, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationWordItem );
													else
														{
														if( adjustedSpanishQuestionSpecificationItem_ != null )
															{
															// Write adjusted compound question
															if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, adjustedSpanishQuestionSpecificationItem_ ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

															adjustedSpanishQuestionSpecificationItem_ = null;
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
										adjustedQuestionWordItem_ == null )
											{
											if( adjustedSpanishQuestionSecondarySpecificationItem_ == null )
												{
												if( exclusiveSecondarySpecificationWordItem.nonCompoundCollectionNr( exclusiveSecondarySpecificationItem.specificationWordTypeNr() ) > Constants.NO_COLLECTION_NR &&
												// Find question to be adjusted
												findQuestionToBeAdjustedByCompoundCollection( isArchivedAssignment, exclusiveSecondarySpecificationCollectionNr, adjustedSpanishQuestionSpecificationItem_, generalizationWordItem, primarySpecificationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );
												}
											else
												{
												if( isPrimarySpecificationWordSpanishAmbiguous )
													{
													// Adjust Spanish question
													while( ( adjustedQuestionCompoundSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null ) ) != null &&
													adjustedQuestionCompoundSpecificationItem != adjustedSpanishQuestionSpecificationItem_ &&
													adjustedQuestionCompoundSpecificationItem.isOlderItem() )
														{
														if( generalizationWordItem.replaceOrDeleteSpecification( false, adjustedQuestionCompoundSpecificationItem, adjustedSpanishQuestionSpecificationItem_ ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a question part" );
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
			if( !userGeneralizationWordItem.hasJustification( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, userSpecificationItem, null, null ) &&

			// Ask question after more specific non-exclusive user specification
			// Not yet explained in the design
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, userGeneralizationWordItem, userSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about the current user sentence" );
			}
		else
			{
			if( userSpecificationItem.isPossessive() )
				{
				if( anotherPrimarySpecificationItem != null &&
				!userGeneralizationWordItem.hasFeminineOrMasculineProperNameEnding() &&

				askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( false, false, isArchivedAssignment, false, isSpanishCurrentLanguage, false, userSpecificationItem.specificationCollectionNr(), userSpecificationItem, anotherPrimarySpecificationItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with possessive user specification as primary specification" );
				}
			else
				{
				if( userGeneralizationWordItem.hasCorrectedAssumptionByOppositeQuestion() &&
				( secondarySpecificationItem = userSpecificationWordItem.firstNonCompoundUserSpecificationItem() ) != null &&
				( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
				// Question part doesn't exist
				!userGeneralizationWordItem.hasJustification( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, userSpecificationItem, null, secondarySpecificationItem ) )
					{
					// Create question part based on current user question
					// Not yet explained in the design
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an invalid assumption generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't create a question part based on current user question" );

					if( ( primarySpecificationItem = userGeneralizationWordItem.olderSuggestiveQuestionAssumptionSecondarySpecificationItem() ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( secondarySpecificationItem = primarySpecificationWordItem.firstNonQuestionSpecificationItem() ) != null &&
					// Question part doesn't exist
					!userGeneralizationWordItem.hasJustification( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, null, secondarySpecificationItem ) &&
					( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
					( correctedSpecificationItem = userGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != null )
						{
						// Correct invalid assumption by opposite question
						// Not yet explained in the design
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to correct an invalid assumption generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
							return adminItem_.startError( 1, moduleNameString_, "I couldn't create the corrected suggestive question assumption" );

						if( userGeneralizationWordItem.replaceOrDeleteSpecification( false, correctedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\"" );

						if( ( obsoleteJustificationItem = userGeneralizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, createdSpecificationItem ) ) != null &&
						// Remove justification from corrected assumption
						userGeneralizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete an obsolete justification item" );

						// Clean-up obsolete justifications
						if( userGeneralizationWordItem.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete obsolete justifications in generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\"" );
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

		if( ( primarySpecificationItem = generalizationWordItem.firstExclusiveNounSpecificationItem() ) != null )
			{
			do	{
				if( ( !isSpanishCurrentLanguage ||
				primarySpecificationItem.isSelfGeneratedQuestion() ) &&

				( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
				( secondarySpecificationItem = primarySpecificationWordItem.firstNonCompoundUserSpecificationItem() ) != null &&
				// Question part doesn't exist
				!generalizationWordItem.hasJustification( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, null, secondarySpecificationItem ) &&

				( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
					{
					isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

					// Ask a specification substitution question
					// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with a question as primary specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
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
		boolean isOlderPrimarySpecification;
		boolean isUserGeneralizationWord;
		boolean isOlderExistingPrimarySpecificationSpanishAmbiguous = false;
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
		( exclusiveSecondarySpecificationItem = anotherPrimarySpecificationWordItem.firstExclusiveNounSpecificationItem() ) != null )
			{
			hasFoundAnswerToQuestion = generalizationWordItem.hasFoundAnswerToQuestion();
			hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasSpecificationCompoundCollection();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();
			primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

			if( isSpanishCurrentLanguage &&
			( existingPrimarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, false, false, Constants.NO_QUESTION_PARAMETER, primarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null ) ) != null &&
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
					if( !generalizationWordItem.hasJustification( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem ) &&
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

						( hasPrimarySpecificationCompoundCollection &&
						!hasUserSpecificationNonCompoundCollection &&
						existingPrimarySpecificationItem == null ) ||

						( !hasFoundAnswerToQuestion &&
						!isArchivedUserAssignment &&
						isOlderExistingPrimarySpecificationSpanishAmbiguous ) ||

						( hasFemaleUserSpecificationWord &&

						( !isOlderPrimarySpecification ||
						userSpecificationCollectionNr == Constants.NO_COLLECTION_NR ) ) ) &&

						( compoundGeneralizationWordItem == anotherPrimarySpecificationWordItem ||
						anotherPrimarySpecificationWordItem.hasCollectionNr( exclusiveSecondarySpecificationCollectionNr ) ) ) ) &&

						// Negative specification doesn't exist
						generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null ) == null ) )
							{
							// Ask a specification substitution question
							// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
							if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
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
		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem.firstExclusiveNounSpecificationItem() ) != null )
			{
			do	{
				if( !generalizationWordItem.hasJustification( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, userSpecificationItem, null, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null &&

				// Ask a specification substitution question
				// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveNounSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte findQuestionToBeAdjustedByCompoundCollection( boolean isArchivedAssignment, int exclusiveSecondarySpecificationCollectionNr, SpecificationItem replacingSpecificationItem, WordItem generalizationWordItem, WordItem primarySpecificationWordItem )
		{
		SpecificationItem adjustedQuestionCompoundSpecificationItem;
		SpecificationItem lastAdjustedQuestionCompoundSpecificationItem = null;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( primarySpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification word item is undefined" );

		if( exclusiveSecondarySpecificationCollectionNr <= Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given exclusive secondary specification collection number is undefined" );

				// Check for adjusted compound question
		while( ( adjustedQuestionCompoundSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null ) ) != null &&
		adjustedQuestionCompoundSpecificationItem.isOlderItem() &&
		// No definition specification exists
		primarySpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, adjustedQuestionCompoundSpecificationItem.specificationWordItem() ) == null )
			{
			if( generalizationWordItem.replaceOrDeleteSpecification( false, adjustedQuestionCompoundSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a question part" );

			lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem;
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != null )
			{
			// Write adjusted compound question
			if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, lastAdjustedQuestionCompoundSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			adjustedQuestionWordItem_ = generalizationWordItem;
			}

		return Constants.RESULT_OK;
		}

	private static SpecificationItem getAnotherPrimarySpecificationItem( WordItem userSpecificationWordItem )
		{
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem = null;
		WordItem currentPossessiveNounWordItem;
		WordItem possessiveSpecificationWordItem;
		WordItem higherLevelPossessiveSpecificationWordItem = null;

		if( userSpecificationWordItem != null &&
		( currentPossessiveNounWordItem = CommonVariables.firstPossessiveNounWordItem ) != null )
			{
			// Do for all possessive noun words
			do	{
				// First, try to find a possessive specification on a higher level
				if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
					{
					do	{
						if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null &&
						possessiveSpecificationWordItem.firstSpecificationItem( false, false, Constants.NO_QUESTION_PARAMETER, userSpecificationWordItem ) != null )
							higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveSpecificationWordItem == null &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
						anotherPrimarySpecificationItem = ( higherLevelPossessiveSpecificationWordItem == null || higherLevelPossessiveSpecificationWordItem == possessiveDefinitionSpecificationItem.specificationWordItem() ? possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() : possessiveDefinitionSpecificationItem );
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
		// Private constructed variables

		adjustedSpanishQuestionSecondarySpecificationItem_ = null;
		adjustedSpanishQuestionSpecificationItem_ = null;

		adjustedQuestionWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminReasoningNewVariables()
		{
		adjustedSpanishQuestionSecondarySpecificationItem_ = null;
		adjustedSpanishQuestionSpecificationItem_ = null;

		adjustedQuestionWordItem_ = null;
		}

	protected byte askQuestions()
		{
		boolean isAllowingSpanishPossessiveSpecification;
		boolean isArchivedUserAssignment;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		int userSpecificationCollectionNr;
		SpecificationItem primarySpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem = null;
		WordItem currentTouchedProperNameWordItem;
		WordItem userSpecificationWordItem;

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
		userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
		userSpecificationWordItem = userSpecificationItem.specificationWordItem();

		if( userSpecificationItem.isGeneralizationProperName() )
			{
			anotherPrimarySpecificationItem = getAnotherPrimarySpecificationItem( userSpecificationWordItem );

			if( askQuestionsAboutSpecificSituations( isArchivedUserAssignment, isSpanishCurrentLanguage, anotherPrimarySpecificationItem, userSpecificationItem, userSpecificationItem.generalizationWordItem(), userSpecificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about specific situations" );
			}

		if( ( currentTouchedProperNameWordItem = CommonVariables.firstTouchedProperNameWordItem ) != null )
			{
			// Do for all proper name words touched during current sentence
			do	{
				isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
															currentTouchedProperNameWordItem.isUserRelationWord );

				if( ( primarySpecificationItem = currentTouchedProperNameWordItem.firstPrimarySpecificationCandidateOfQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null &&
				askQuestions( isAllowingSpanishPossessiveSpecification, isArchivedUserAssignment, userSpecificationCollectionNr, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, currentTouchedProperNameWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper name word \"" + currentTouchedProperNameWordItem.anyWordTypeString() + "\"" );

				if( askQuestionsWithQuestionAsPrimarySpecification( isSpanishCurrentLanguage, currentTouchedProperNameWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper name word \"" + currentTouchedProperNameWordItem.anyWordTypeString() + "\" with a question as primary specification" );
				}
			while( ( currentTouchedProperNameWordItem = currentTouchedProperNameWordItem.nextTouchedProperNameWordItem ) != null );
			}

		return Constants.RESULT_OK;
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
