/*	Class:			AdminReasoningNew
 *	Supports class:	AdminItem
 *	Purpose:		To provide autonomous reasoning (new design)
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
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

	private boolean hasSkippedHiddenSpanishSpecification_;

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
		BoolResultType boolResult = new BoolResultType();
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		hasFeminineOrMasculineProperNameEnding = generalizationWordItem.hasFeminineOrMasculineProperNameEnding();
		hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
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
						archivedPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, false, primarySpecificationWordItem );

						// Typically for the Spanish language
						if( anotherPrimarySpecificationItem != null &&
						// Specification with user specification collection doesn't exist
						generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) == null &&

						askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( adminItem_.hasFemaleUserSpecificationWord(), userSpecificationItem.hasNonCompoundSpecificationCollection(), isArchivedAssignment, isArchivedUserAssignment, true, isUserSpecificationWordSpanishAmbiguous, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != null && archivedPrimarySpecificationItem.isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), anotherPrimarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask Spanish questions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
						}
					}
				else
					{
					hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();

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
								generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation() )
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
									generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationWordItem ) == null ) ) ||

									// Has possibly a gap in the knowledge
									( ( !primarySpecificationItem.isOlderItem() ||
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
											// Ask a specification substitution question
											// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( alternativePrimarySpecificationItem == null ? primarySpecificationItem : alternativePrimarySpecificationItem ), null, ( isPrimarySpecificationWordSpanishAmbiguous && isUserGeneralizationWord && adjustedSpanishQuestionSecondarySpecificationItem_ != null ? adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem ), null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );

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
														adjustedSpanishQuestionSpecificationItem_ = generalizationWordItem.firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem );
													else
														{
														if( adjustedSpanishQuestionSpecificationItem_ != null )
															{
															// Write adjusted compound question
															if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, adjustedSpanishQuestionSpecificationItem_ ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted compound question in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

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
												if( primarySpecificationWordItem.hasCollectionNr( userSpecificationCollectionNr ) &&
												exclusiveSecondarySpecificationWordItem.nonCompoundCollectionNr( exclusiveSecondarySpecificationItem.specificationWordTypeNr() ) > Constants.NO_COLLECTION_NR &&

												// Find question to be adjusted by compound collection
												( boolResult = generalizationWordItem.findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationCollectionNr, primarySpecificationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );

												if( boolResult.booleanValue )
													adjustedQuestionWordItem_ = generalizationWordItem;
												}
											else
												{
												if( isPrimarySpecificationWordSpanishAmbiguous )
													{
													// Adjust Spanish question
													while( ( adjustedQuestionCompoundSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, exclusiveSecondarySpecificationCollectionNr, null ) ) != null &&
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
			if( !userGeneralizationWordItem.hasJustification( userSpecificationItem, null, null ) &&

			// Ask question after more specific non-exclusive user specification
			// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
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
				!userGeneralizationWordItem.hasJustification( userSpecificationItem, null, secondarySpecificationItem ) )
					{
					// Create question part based on current user question
					// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an invalid assumption generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't create a question part based on current user question" );

					if( ( primarySpecificationItem = userGeneralizationWordItem.olderSuggestiveQuestionAssumptionSecondarySpecificationItem() ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( secondarySpecificationItem = primarySpecificationWordItem.firstNonQuestionSpecificationItem() ) != null &&
					// Question part doesn't exist
					!userGeneralizationWordItem.hasJustification( primarySpecificationItem, null, secondarySpecificationItem ) &&
					( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
					( correctedSpecificationItem = userGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationItem.specificationCollectionNr(), secondarySpecificationWordItem ) ) != null )
						{
						// Correct invalid assumption by opposite question
						// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to correct an invalid assumption generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
							return adminItem_.startError( 1, moduleNameString_, "I couldn't create the corrected suggestive question assumption" );

						if( userGeneralizationWordItem.replaceOrDeleteSpecification( false, correctedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
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

		if( ( primarySpecificationItem = generalizationWordItem.firstExclusiveNounSpecificationItem() ) != null )
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
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
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
						generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, null ) == null ) )
							{
							// Ask a specification substitution question
							// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
							if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
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
		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem.firstExclusiveNounSpecificationItem() ) != null )
			{
			do	{
				if( !generalizationWordItem.hasJustification( userSpecificationItem, null, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null &&

				// Ask a specification substitution question
				// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveNounSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawPossessiveReversibleConclusions( SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean isOlderSecondarySpecification;
		boolean isOlderUserSpecification;
		boolean isPossessive;
		boolean isSpanishCurrentLanguage;
		boolean isSpecificationFemaleWord = false;
		boolean isSpanishUserAssignment = false;
		int nonCompoundSpanishSpecificationCollectionNr;
		int relationContextNr;
		int hiddenSpanishSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem createdJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem firstUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem masculineSpecificationItem;
		SpecificationItem nonCompoundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem existingSpanishUserSpecificationItem = null;
		SpecificationItem userSpecificationItem = null;
		WordItem currentContextWordItem;
		WordItem firstContextWordItem;
		WordItem specificationWordItem;
		WordItem masculineSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult = new JustificationResultType();

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( firstContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage &&
			( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
			userSpecificationItem.isAssignment() )
				isSpanishUserAssignment = true;

			do	{
				isPossessive = secondarySpecificationItem.isPossessive();

				if( !isPossessive )
					{
					isOlderSecondarySpecification = secondarySpecificationItem.isOlderItem();
					relationContextNr = secondarySpecificationItem.relationContextNr();
					currentContextWordItem = firstContextWordItem;

					if( ( specificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "The secondary specification item has no specification word" );

					if( isSpanishCurrentLanguage )
						{
						isSpecificationFemaleWord = specificationWordItem.isFemale();
						masculineSpecificationWordItem = ( isSpecificationFemaleWord ? specificationWordItem.masculineCollectionWordItem() : null );
						existingSpanishUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );
						}

					// Do for all context words
					do	{
						if( currentContextWordItem != generalizationWordItem &&
						// Relation context exists
						currentContextWordItem.hasContextInWord( relationContextNr, specificationWordItem ) )
							{
							existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, !isPossessive, specificationWordItem, generalizationWordItem );

							if( existingSpecificationItem != null ||
							currentContextWordItem.isUserGeneralizationWord ||
							currentContextWordItem.isUserRelationWord )
								{
								firstUserSpecificationItem = currentContextWordItem.firstUserSpecificationItem( false, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

								isOlderUserSpecification = ( existingSpecificationItem == null &&
															firstUserSpecificationItem != null &&
															firstUserSpecificationItem.isOlderItem() );

								// Specification doesn't exist
								// User specification doesn't exist
								if( ( isOlderUserSpecification ||
								firstUserSpecificationItem == null ||
								// More specific specification
								!firstUserSpecificationItem.hasRelationContext() ) &&

								( existingSpecificationItem == null ||

								// Justification doesn't exist
								( !currentContextWordItem.hasJustification( null, null, secondarySpecificationItem ) &&
								secondarySpecificationItem == generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, specificationWordItem, currentContextWordItem ) &&

								( existingSpanishUserSpecificationItem == null ||
								existingSpecificationItem.secondarySpecificationJustificationItem( existingSpanishUserSpecificationItem ) == null ) ) ) )
									{
									if( isSpanishCurrentLanguage )
										{
										hiddenSpanishSpecificationCollectionNr = Constants.NO_COLLECTION_NR;

										if( existingSpecificationItem == null )
											{
											if( isPossessive &&
											!generalizationWordItem.isOlderItem() &&
											secondarySpecificationItem.isUserSpecification() &&
											( nonCompoundSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
											nonCompoundSpecificationItem.isHiddenSpanishSpecification() )
												hiddenSpanishSpecificationCollectionNr = nonCompoundSpecificationItem.specificationCollectionNr();
											}
										else
											{
											if( existingSpecificationItem.wasHiddenSpanishSpecification() )
												hiddenSpanishSpecificationCollectionNr = existingSpecificationItem.specificationCollectionNr();
											}
										}

									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NAME, hiddenSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with relation word \"" + currentContextWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

									if( secondarySpecificationItem.isReplacedItem() )
										secondarySpecificationItem = secondarySpecificationItem.updatedSpecificationItem();

									if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
										{
										// Created, not found
										if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == null )
											{
											// Typically for the Spanish language
											if( isSpanishCurrentLanguage &&
											!isPossessive &&
											!isOlderUserSpecification &&
											( obsoleteSpecificationItem = currentContextWordItem.noRelationContextSpecificationItem( true, true, ( isSpecificationFemaleWord ? masculineSpecificationWordItem : specificationWordItem ) ) ) != null )
												{
												// Write an assumption without relation word
												if( currentContextWordItem.writeUpdatedSpecification( true, false, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

												// Replace an assumption without relation word
												if( currentContextWordItem.replaceOrDeleteSpecification( false, obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
												}
											}
										else	// Created and found
											{
											if( isOlderUserSpecification )
												{
												// Additional justification. Test file: "Complex (19 - strange)"
												if( ( justificationResult = currentContextWordItem.addJustification( false, false, true, ( foundSpecificationItem.hasAssumptionLevel() ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, foundSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												if( createdSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the created specification" );

												if( currentContextWordItem.updateSpecificationsInJustificationsOfInvolvedWords( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									}

								// Typically for the Spanish language
								if( isSpanishCurrentLanguage &&
								isSpecificationFemaleWord &&
								existingSpecificationItem == null &&
								masculineSpecificationWordItem != null &&
								!currentContextWordItem.hasContextInWord( relationContextNr, masculineSpecificationWordItem ) )
									{
									// Masculine specification doesn't exist
									if( ( ( masculineSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, !isPossessive, masculineSpecificationWordItem, generalizationWordItem ) ) == null &&
									!isOlderSecondarySpecification ) ||

									// Masculine specification exists
									( masculineSpecificationItem != null &&

									( isSpanishUserAssignment ||
									masculineSpecificationItem.isSelfGenerated() ) &&

									masculineSpecificationItem.secondarySpecificationJustificationItem( secondarySpecificationItem ) == null ) )
										{
										nonCompoundSpanishSpecificationCollectionNr = ( masculineSpecificationItem == null &&
																						secondarySpecificationItem.isUserSpecification() ? masculineSpecificationWordItem.nonCompoundCollectionNr( Constants.WORD_TYPE_NOUN_SINGULAR ) : Constants.NO_COLLECTION_NR );

										if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, true, false, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NAME, nonCompoundSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, currentContextWordItem, masculineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish possessive reversible conclusion with relation word \"" + currentContextWordItem.anyWordTypeString() + "\" and masculine specification word \"" + masculineSpecificationWordItem.anyWordTypeString() + "\"" );

										if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
										( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
										!foundSpecificationItem.isHiddenSpanishSpecification() )
											{
											// Additional Spanish justification (User)
											if( ( justificationResult = currentContextWordItem.addJustification( false, false, true, ( foundSpecificationItem.hasAssumptionLevel() ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, foundSpecificationItem, null, createdSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish justification to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

											if( createdSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
											}
										}
									else
										{
										if( userSpecificationItem != null &&
										( definitionSpecificationItem = masculineSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&

										// Add Spanish - Feminine / definition specification
										adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, true, false, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NAME, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, currentContextWordItem, specificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish possessive reversible conclusion with relation word \"" + currentContextWordItem.anyWordTypeString() + "\" and masculine specification word \"" + masculineSpecificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							}
						}
					while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
					}
				}
			while( ( secondarySpecificationItem = secondarySpecificationItem.nextNonExclusiveSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawProperNamePartOfConclusions( boolean hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, boolean isArchivedAssignment, boolean isCheckingForArchivedAssignment, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem spanishRelationWordItem )
		{
		boolean isPrimarySpecificationArchivedAssignment;
		boolean isSpecificationWordSpanishAmbiguous;
		short assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		JustificationItem createdJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem oppositePossessiveDefinitionSpecificationItem;
		SpecificationItem partOfDefinitionSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem spanishUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem currentPossessiveNounWordItem;
		WordItem possessiveDefinitionSpecificationWordItem;
		WordItem spanishAmbiguousSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		// Select primary specification
		primarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null );

		if( primarySpecificationItem == null ||
		isSpecificationWordSpanishAmbiguous )
			{
			primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isCheckingForArchivedAssignment, false, false, false, specificationWordItem );

			// Typically for the Spanish language
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

					if( primarySpecificationItem != null &&
					!primarySpecificationItem.hasRelationContext() )
						primarySpecificationItem = null;
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
			if( !isArchivedAssignment &&
			primarySpecificationItem.hasRelationContext() &&

			( primarySpecificationItem.isUserSpecification() ||
			generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation() ) &&

			( tempSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null )
				primarySpecificationItem = tempSpecificationItem;
			}

		if( primarySpecificationItem != null &&
		( currentPossessiveNounWordItem = CommonVariables.firstPossessiveNounWordItem ) != null )
			{
			// Typically for the Spanish language
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
				// Justification doesn't exist
				!generalizationWordItem.hasJustification( primarySpecificationItem, null, partOfDefinitionSpecificationItem ) )
					{
					partOfSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, currentPossessiveNounWordItem );

					if( partOfSpecificationItem == null ||
					primarySpecificationItem.assumptionLevel() < partOfSpecificationItem.assumptionLevel() ||

					( primarySpecificationItem.assumptionLevel() == partOfSpecificationItem.assumptionLevel() &&
					// Justification doesn't exist
					partOfSpecificationItem.primarySpecificationJustificationItem( primarySpecificationItem ) == null &&

					( !primarySpecificationItem.hasRelationContext() ||
					primarySpecificationItem.isPossessive() ||
					// Typically for the Spanish language
					!partOfSpecificationItem.hasPossessivePrimarySpecificationJustification() ) ) ||

					// Typically for the Spanish language
					( primarySpecificationItem.hasRelationContext() &&

					( ( partOfSpecificationItem.isConcludedAssumption() &&

					( primarySpecificationItem.isPossessive() ||
					generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() ) ) ||

					( !partOfSpecificationItem.hasAssumptionLevel() &&
					primarySpecificationItem.isPossessive() &&
					primarySpecificationItem.isOlderItem() &&
					primarySpecificationItem.isSelfGeneratedAssumption() ) ) ) )
						{
						isPrimarySpecificationArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

						// Typically for the Spanish language
						if( isSpecificationWordSpanishAmbiguous )
							assumptionLevel = ( primarySpecificationItem.hasRelationContext() &&
												primarySpecificationItem.isPossessive() &&

												// Primary specification will become an assumption
												// Forcing 'part of' assumption instead conclusion
												( ( partOfSpecificationItem == null &&
												!primarySpecificationItem.hasCurrentCreationSentenceNr() ) ||

												// Primary specification isn't recalculated
												// Forcing 'part of' assumption instead conclusion
												( !primarySpecificationItem.hasAssumptionLevel() &&
												primarySpecificationItem.isSelfGeneratedAssumption() ) ) ? 1 : Constants.NO_ASSUMPTION_LEVEL );

						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the challenge document, or http://mafait.org/block1/ and http://mafait.org/block2/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, true, false, false, false, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, partOfDefinitionSpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
							{
							foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

							if( foundSpecificationItem.isSelfGeneratedAssumption() &&
							// Recalculate unhidden Spanish assumption
							foundSpecificationItem.recalculateAssumption() != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate an unhidden Spanish assumption" );

							// Unhidden Spanish assumption
							if( spanishRelationWordItem != null &&
							primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
							( oppositePossessiveDefinitionSpecificationItem = adminItem_.oppositePossessiveDefinitionSpecificationItem() ) != null &&
							( possessiveDefinitionSpecificationWordItem = adminItem_.possessiveDefinitionSpecificationWordItem() ) != null &&
							( assumptionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, spanishRelationWordItem ) ) != null &&
							assumptionSpecificationItem.primarySpecificationJustificationItem( primarySpecificationItem ) == null &&
							// Feminine specification doesn't exist
							spanishRelationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, false, specificationWordItem.feminineCollectionWordItem(), generalizationWordItem ) == null )
								{
								// Additional justification (unhidden)
								if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, primarySpecificationItem, null, oppositePossessiveDefinitionSpecificationItem, null, assumptionSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
									return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

								if( ( createAndAssignResult = generalizationWordItem.createSpecificationItem( assumptionSpecificationItem.isAssignment(), assumptionSpecificationItem.isInactiveAssignment(), assumptionSpecificationItem.isArchivedAssignment(), assumptionSpecificationItem.isAnsweredQuestion(), assumptionSpecificationItem.isConcludedAssumption(), assumptionSpecificationItem.isConditional(), assumptionSpecificationItem.isCorrectedAssumption(), assumptionSpecificationItem.isEveryGeneralization(), assumptionSpecificationItem.isExclusiveGeneralization(), assumptionSpecificationItem.isExclusiveSpecification(), assumptionSpecificationItem.isNegative(), assumptionSpecificationItem.isPartOf(), assumptionSpecificationItem.isPossessive(), assumptionSpecificationItem.isSpecificationGeneralization(), assumptionSpecificationItem.isUniqueUserRelation(), assumptionSpecificationItem.isValueSpecification(), assumptionSpecificationItem.assignmentLevel(), assumptionSpecificationItem.assumptionLevel(), assumptionSpecificationItem.languageNr(), assumptionSpecificationItem.prepositionParameter(), assumptionSpecificationItem.questionParameter(), assumptionSpecificationItem.generalizationWordTypeNr(), assumptionSpecificationItem.specificationWordTypeNr(), assumptionSpecificationItem.relationWordTypeNr(), assumptionSpecificationItem.specificationCollectionNr(), assumptionSpecificationItem.generalizationContextNr(), assumptionSpecificationItem.specificationContextNr(), assumptionSpecificationItem.relationContextNr(), assumptionSpecificationItem.originalSentenceNr(), assumptionSpecificationItem.activeSentenceNr(), assumptionSpecificationItem.inactiveSentenceNr(), assumptionSpecificationItem.archivedSentenceNr(), assumptionSpecificationItem.nContextRelations(), createdJustificationItem, assumptionSpecificationItem.specificationWordItem(), assumptionSpecificationItem.specificationString(), assumptionSpecificationItem.storedSentenceStringBuffer(), assumptionSpecificationItem.storedSentenceWithOnlyOneSpecificationStringBuffer() ) ).result != Constants.RESULT_OK )
									return adminItem_.startError( 1, moduleNameString_, "I failed to create a copy of the assumption specification with the created justification item" );

								if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
									return adminItem_.startError( 1, moduleNameString_, "I couldn't copy the assumption specification" );

								if( generalizationWordItem.replaceOrDeleteSpecification( false, assumptionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
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
						possessiveSpecificationWordItem.firstSpecificationItem( false, false, false, userSpecificationWordItem ) != null )
							higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveSpecificationWordItem == null &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
						anotherPrimarySpecificationItem = ( higherLevelPossessiveSpecificationWordItem == null ||
															higherLevelPossessiveSpecificationWordItem == possessiveDefinitionSpecificationItem.specificationWordItem() ? possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() : possessiveDefinitionSpecificationItem );
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

		hasSkippedHiddenSpanishSpecification_ = false;

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
		hasSkippedHiddenSpanishSpecification_ = false;

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

	protected byte correctSuggestiveAssumptionsByOppositeQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		JustificationItem correctedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem createdNewSpecificationItem;
		SpecificationItem createdOldSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem oldSuggestiveSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
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
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem.olderSuggestiveQuestionAssumptionSecondarySpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, true, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		if( ( correctedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem.correctedAssumptionByOppositeQuestionJustificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old corrected assumption by opposite question justification item in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		// Change primary user specification by primary corrected question specification
		if( correctedAssumptionByOppositeQuestionJustificationItem.changePrimarySpecification( createdNewSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to change the primary specification of the corrected assumption by opposite question justification item" );

		if( ( secondarySpecificationWordItem = correctedAssumptionByOppositeQuestionJustificationItem.secondarySpecificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );

		// Correct generalization assumption
		if( ( primarySpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, false, false, secondarySpecificationWordItem ) ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );

		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution assumption" );

		if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the corrected generalization assumption" );

		if( foundSpecificationItem.attachJustificationToSpecification( correctedAssumptionByOppositeQuestionJustificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );

		return Constants.RESULT_OK;
		}

	protected byte drawPossessiveReversibleConclusions( WordItem generalizationWordItem )
		{
		SpecificationItem secondarySpecificationItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem.isProperNameWord() )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item isn't a proper name" );
/*
		// Assignments
		if( ( secondarySpecificationItem = generalizationWordItem.firstNonExclusiveSpecificationItem( true ) ) != null &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions with assignments" );
*/
		// Specifications
		if( ( secondarySpecificationItem = generalizationWordItem.firstNonExclusiveSpecificationItem( false ) ) != null &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions with specifications" );

		return Constants.RESULT_OK;
		}

	protected byte drawProperNamePartOfConclusions( boolean hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, boolean isArchivedAssignment, WordItem generalizationProperNameWordItem, WordItem specificationWordItem, WordItem spanishRelationWordItem )
		{
		WordItem currentProperNameWordItem;

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationProperNameWordItem == null ||

		// Try all proper names again. Maybe the specification isn't hidden anymore
		( hasSkippedHiddenSpanishSpecification_ &&
		!generalizationProperNameWordItem.isUserGeneralizationWord ) )
			{
			if( ( currentProperNameWordItem = CommonVariables.firstUserProperNameWordItem ) != null )
				{
				// Do for all user proper name words
				do	{
					if( currentProperNameWordItem != spanishRelationWordItem &&
					drawProperNamePartOfConclusions( hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, isArchivedAssignment, true, currentProperNameWordItem, specificationWordItem, spanishRelationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a proper name 'part of' conclusion in proper name word \"" + currentProperNameWordItem.anyWordTypeString() + "\"" );
					}
				while( ( currentProperNameWordItem = currentProperNameWordItem.nextUserProperNameWordItem ) != null );
				}
			}
		else
			{
			if( drawProperNamePartOfConclusions( false, isArchivedAssignment, isArchivedAssignment, generalizationProperNameWordItem, specificationWordItem, spanishRelationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a proper name 'part of' conclusion in word \"" + generalizationProperNameWordItem.anyWordTypeString() + "\"" );
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
