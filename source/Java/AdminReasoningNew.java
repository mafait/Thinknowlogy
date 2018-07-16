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

class AdminReasoningNew
	{
	// Private constructed variables

	private boolean hasSkippedHiddenSpanishSpecification_;

	private SpecificationItem adjustedSpanishQuestionSecondarySpecificationItem_;

	private WordItem adjustedQuestionWordItem_;

	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


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
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem archivedPrimarySpecificationItem;
		SpecificationItem exclusiveSecondarySpecificationItem;
		SpecificationItem questionPrimarySpecificationItem;
		SpecificationItem questionSecondarySpecificationItem;
		WordItem collectionWordItem;
		WordItem compoundGeneralizationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem primarySpecificationWordItem;
		BoolResultType boolResult = new BoolResultType();
		CreateAndAssignResultType createAndAssignResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		hasFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
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
													GlobalVariables.nUserSpecificationWords == 1 ) );
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();

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

						askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( adminItem_.hasFemaleUserSpecificationWord(), userSpecificationItem.hasNonCompoundSpecificationCollection(), isArchivedAssignment, isArchivedUserAssignment, true, isUserSpecificationWordSpanishAmbiguous, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != null && archivedPrimarySpecificationItem.isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), anotherPrimarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask Spanish questions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
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
									return adminItem_.addError( 1, moduleNameString_, "I failed to ask a Spanish question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
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
																					adjustedSpanishQuestionSecondarySpecificationItem_ != null ? adjustedSpanishQuestionSecondarySpecificationItem_ : exclusiveSecondarySpecificationItem );

											// Ask a specification substitution question
											// See paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, questionPrimarySpecificationItem, null, questionSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );

											// Adjusted question
											if( isPrimarySpecificationWordSpanishAmbiguous &&
											adjustedSpanishQuestionSecondarySpecificationItem_ == null &&
											createAndAssignResult.createdSpecificationItem == null )
												adjustedSpanishQuestionSecondarySpecificationItem_ = exclusiveSecondarySpecificationItem;
											}
										else
											hasSkippedAskingQuestion = true;
										}
									else
										{
										if( isUserGeneralizationWord &&
										hasExclusiveSecondarySpecificationCompoundCollection &&
										adjustedQuestionWordItem_ == null &&
										adjustedSpanishQuestionSecondarySpecificationItem_ == null )
											{
											if( primarySpecificationWordItem.hasCollectionNr( userSpecificationCollectionNr ) &&
											exclusiveSecondarySpecificationWordItem.nonCompoundCollectionNr() > Constants.NO_COLLECTION_NR &&

											// Find question to be adjusted by compound collection
											( boolResult = generalizationWordItem.findQuestionToBeAdjustedByCompoundCollection( exclusiveSecondarySpecificationCollectionNr, primarySpecificationWordItem ) ).result != Constants.RESULT_OK )
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
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an invalid assumption generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

					if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't create a question part based on current user question" );

					if( ( primarySpecificationItem = userGeneralizationWordItem.suggestiveQuestionAssumptionSecondarySpecificationItem() ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( secondarySpecificationItem = primarySpecificationWordItem.firstNonQuestionSpecificationItem() ) != null &&
					( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
					( correctedSpecificationItem = userGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationItem.specificationCollectionNr(), secondarySpecificationWordItem ) ) != null )
						{
						// Correct invalid assumption by opposite question
						// For asking questions in general, see design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, userGeneralizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to correct an invalid assumption generalization word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );

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

	private byte drawPossessiveReversibleConclusions( SpecificationItem currentSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean isActiveUserAssignment = false;
		boolean isAssignment;
		boolean isExclusiveGeneralizationAssignment;
		boolean isExclusiveSpecification;
		boolean isForcingNewSpanishJustification = false;
		boolean isOlderExistingSpecification;
		boolean isOlderSecondarySpecification;
		boolean isOlderUserSpecification;
		boolean isPossessive;
		boolean isPossessiveUserSpecification = false;
		boolean isSecondaryArchivedAssignment;
		boolean isSecondaryUserAssignment;
		boolean isSelfGenerated;
		boolean isSpanishCurrentLanguage;
		boolean isSpanishSpecificationFemaleWord = false;
		boolean isUniqueUserRelation;
		boolean isUserGeneralizationWord;
		short generalizationWordTypeNr;
		short specificationWordTypeNr;
		short relationWordTypeNr;
		int nonCompoundSpanishSpecificationCollectionNr;
		int spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int relationContextNr;
		int specificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem currentJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem olderJustificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem firstUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem reversedUserSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem spanishMasculineSpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem currentContextWordItem;
		WordItem firstContextWordItem;
		WordItem spanishMasculineSpecificationWordItem = null;
		WordItem specificationWordItem;
		WordItem userGeneralizationWordItem = null;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult = new JustificationResultType();

		if( currentSecondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( firstContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
				{
				isPossessiveUserSpecification = userSpecificationItem.isPossessive();
				userGeneralizationWordItem = userSpecificationItem.generalizationWordItem();

				if( isPossessiveUserSpecification )
					{
					// Typical for Spanish
					if( isSpanishCurrentLanguage &&
					adminItem_.hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore() &&
					( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null &&
					( nonCompoundSpanishSpecificationCollectionNr = userSpecificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
					( existingSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, Constants.NO_QUESTION_PARAMETER, nonCompoundSpanishSpecificationCollectionNr, userSpecificationWordItem ) ) != null &&
					// Justification doesn't exist in specification
					existingSpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null )
						{
						// Additional justification (Possessive - Spanish - User)
						if( ( justificationResult = generalizationWordItem.addJustification( false, true, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, userSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
							return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

						if( existingSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the assumption specification" );
						}
					}
				else
					{
					if( userSpecificationItem.isActiveAssignment() )
						isActiveUserAssignment = true;
					}
				}

			// Do for all selected specifications
			do	{
				isOlderSecondarySpecification = currentSecondarySpecificationItem.isOlderItem();
				isPossessive = currentSecondarySpecificationItem.isPossessive();
				isSelfGenerated = currentSecondarySpecificationItem.isSelfGenerated();
				generalizationWordTypeNr = currentSecondarySpecificationItem.generalizationWordTypeNr();
				specificationWordTypeNr = currentSecondarySpecificationItem.specificationWordTypeNr();
				relationWordTypeNr = currentSecondarySpecificationItem.relationWordTypeNr();
				specificationCollectionNr = currentSecondarySpecificationItem.specificationCollectionNr();
				relationContextNr = currentSecondarySpecificationItem.relationContextNr();

				if( ( specificationWordItem = currentSecondarySpecificationItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The secondary specification item has no specification word" );

				if( isPossessive )
					{
					if( isSelfGenerated &&
					userSpecificationItem != null )
						{
						if( isUserGeneralizationWord ||
						!currentSecondarySpecificationItem.hasSpecificationCollection() )
							{
							if( !isSpanishCurrentLanguage ||
							!currentSecondarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
								{
								currentContextWordItem = firstContextWordItem;

								// Do for all context words
								do	{
									if( currentContextWordItem != generalizationWordItem &&
									( existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, specificationWordItem, generalizationWordItem ) ) != null )
										{
										// Found user specification, force self-generated assumption
										if( existingSpecificationItem.isUserSpecification() &&
										( tempSpecificationItem = currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, true, specificationWordItem, generalizationWordItem ) ) != null )
											existingSpecificationItem = tempSpecificationItem;

										if( relationContextNr > Constants.NO_CONTEXT_NR &&
										existingSpecificationItem.hasCurrentCreationSentenceNr() &&
										existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
										// Relation context exists in word
										currentContextWordItem.hasContextInWord( relationContextNr, specificationWordItem ) &&
										// Justification doesn't exist in specification
//										existingSpecificationItem.secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == null &&
										// Justification doesn't exist in specification

										( ( olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ) ) == null ||
										olderJustificationItem.secondaryGeneralizationWordItem() != generalizationWordItem ) )
											{
											if( ( justificationResult = currentContextWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, currentSecondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null &&
											currentContextWordItem.attachJustificationInWord( createdJustificationItem, existingSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the given assumption specification" );
											}
										}
									}
								while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
								}
							}
						else
							{
							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							isActiveUserAssignment &&
							isOlderSecondarySpecification &&
							currentSecondarySpecificationItem.hasNonCompoundSpecificationCollection() &&
							// Justification doesn't exist in specification
							currentSecondarySpecificationItem.secondarySpecificationJustificationItem( userSpecificationItem ) == null &&

							// Draw Spanish non-compound possessive reversible conclusion
							( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, userSpecificationItem, null, generalizationWordItem, specificationWordItem, userGeneralizationWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated possessive reversible specification from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				else	// Non-possessive
					{
					isExclusiveSpecification = currentSecondarySpecificationItem.isExclusiveSpecification();
					isSecondaryArchivedAssignment = currentSecondarySpecificationItem.isArchivedAssignment();
					isSecondaryUserAssignment = currentSecondarySpecificationItem.isUserAssignment();

					isExclusiveGeneralizationAssignment = ( isSecondaryUserAssignment &&
															currentSecondarySpecificationItem.isExclusiveGeneralization() );

					currentContextWordItem = firstContextWordItem;

					// Typical for Spanish
					if( isSpanishCurrentLanguage )
						{
						isSpanishSpecificationFemaleWord = specificationWordItem.isFemale();
						spanishMasculineSpecificationWordItem = ( isSpanishSpecificationFemaleWord ? specificationWordItem.masculineCollectionWordItem() : null );
						}

					// Do for all context words
					do	{
						if( relationContextNr > Constants.NO_CONTEXT_NR &&
						currentContextWordItem != generalizationWordItem &&

						( !isSpanishCurrentLanguage ||
						!isSecondaryArchivedAssignment ) &&

						// Relation context exists in word
						currentContextWordItem.hasContextInWord( relationContextNr, specificationWordItem ) )
							{
							existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, isSecondaryArchivedAssignment, false, true, specificationWordItem, generalizationWordItem );

							isOlderExistingSpecification = ( existingSpecificationItem != null &&
															existingSpecificationItem.isOlderItem() );

							if( !isSecondaryUserAssignment ||
							!hasGeneralizationWordCurrentlyConfirmedSpecification )
								{
								if( isExclusiveSpecification )
									{
									// Specification doesn't exist
									if( existingSpecificationItem == null &&
									// Draw possessive reversible conclusion
									( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem, null, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated possessive reversible specification from current context word \"" + currentContextWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									firstUserSpecificationItem = currentContextWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

									isOlderUserSpecification = ( existingSpecificationItem == null &&
																firstUserSpecificationItem != null );

									reversedUserSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, specificationWordItem, currentContextWordItem );

									// User specification doesn't exist
									if( isOlderUserSpecification ||
									firstUserSpecificationItem == null ||

									// Typical for Spanish
									( isSpanishCurrentLanguage &&
									reversedUserSpecificationItem != null &&
									currentSecondarySpecificationItem != reversedUserSpecificationItem &&

									( ( !isOlderExistingSpecification &&
									reversedUserSpecificationItem.isOlderItem() ) ||

									( isOlderExistingSpecification &&
									!reversedUserSpecificationItem.isOlderItem() &&
									currentSecondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ) )
										{
										if( existingSpecificationItem == null ||

										( currentSecondarySpecificationItem == reversedUserSpecificationItem &&
										// Justification doesn't exist in specification
										existingSpecificationItem.secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == null ) )
											{
											isAssignment = ( isExclusiveGeneralizationAssignment ||
															isSecondaryArchivedAssignment );

											isUniqueUserRelation = ( isSecondaryArchivedAssignment &&
																	isSecondaryUserAssignment &&
																	adminItem_.isUniqueUserRelation() );

											if( isSpanishCurrentLanguage )
												{
												spanishAmbiguousSpecificationCollectionNr = ( !isSpanishSpecificationFemaleWord &&
																							reversedUserSpecificationItem == null ? specificationCollectionNr :

																							( existingSpecificationItem != null &&

																							( existingSpecificationItem.wasHiddenSpanishSpecification() ||

																							( !isSpanishSpecificationFemaleWord &&
																							currentSecondarySpecificationItem.hasCompoundSpecificationCollection() &&
																							existingSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ? existingSpecificationItem.specificationCollectionNr() : Constants.NO_COLLECTION_NR ) );

												isForcingNewSpanishJustification = ( spanishAmbiguousSpecificationCollectionNr > Constants.NO_COLLECTION_NR );
												}

											// Draw possessive reversible conclusion
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isSecondaryArchivedAssignment, false, false, false, false, isForcingNewSpanishJustification, false, false, true, isUniqueUserRelation, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem.updatedSpecificationItem(), null, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with generalization word \"" + currentContextWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

											if( currentSecondarySpecificationItem.isReplacedItem() )
												currentSecondarySpecificationItem = currentSecondarySpecificationItem.updatedSpecificationItem();

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;

											if( !isSpanishCurrentLanguage &&
											!isOlderUserSpecification &&
											isSelfGenerated &&
											( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? createAndAssignResult.foundSpecificationItem : createdSpecificationItem ) ) != null &&
											createdOrFoundSpecificationItem.hasNonCompoundSpecificationCollection() &&

											( isUserGeneralizationWord ||
											!createdOrFoundSpecificationItem.isCircular( generalizationWordItem ) ) )
												{
												if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, createdOrFoundSpecificationItem.updatedSpecificationItem(), null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null &&
												generalizationWordItem.attachJustificationInWord( createdJustificationItem, currentSecondarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the given assumption specification" );
												}

											if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
												{
												// Created, not found
												if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == null )
													{
													// Typical for Spanish
													if( isSpanishCurrentLanguage &&
													!isOlderUserSpecification &&
													( obsoleteSpecificationItem = currentContextWordItem.noRelationContextSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : specificationWordItem ) ) ) != null )
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
														// Additional justification. Test file: "Complex (19 - strange)"
														if( ( justificationResult = currentContextWordItem.addJustification( false, false, true, ( foundSpecificationItem.hasAssumptionLevel() ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, foundSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
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
										else
											{
											if( currentSecondarySpecificationItem == reversedUserSpecificationItem )
												{
												if( isUserGeneralizationWord &&
												( olderJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ) ) != null &&
												( currentJustificationItem = existingSpecificationItem.currentJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != null &&
												( primarySpecificationItem = currentJustificationItem.primarySpecificationItem() ) != null &&
												( secondarySpecificationItem = olderJustificationItem.secondarySpecificationItem() ) != null &&
												( foundSpecificationItem = currentJustificationItem.secondarySpecificationItem() ) != null &&
												!foundSpecificationItem.isOlderItem() )
													{
													// Additional justification
													if( ( justificationResult = currentContextWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, primarySpecificationItem, null, secondarySpecificationItem, null, foundSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
														return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

													if( foundSpecificationItem.changeFirstJustification( true, createdJustificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the created specification" );
													}
												}
											else
												{
												// Typical for Spanish
												if( isSpanishCurrentLanguage &&
												reversedUserSpecificationItem != null )
													{
													if( isOlderSecondarySpecification )
														{
														if( currentSecondarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&

														( existingSpecificationItem == firstUserSpecificationItem ||

														( firstUserSpecificationItem == null &&
														currentSecondarySpecificationItem.isUserSpecification() ) ) &&

														// Justification doesn't exist in specification
														reversedUserSpecificationItem.secondarySpecificationJustificationItem( currentSecondarySpecificationItem ) == null )
															{
															// Additional Spanish justification (self-generated conclusion). Test file: "Complejo (19 - extraño)"
															if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, ( currentSecondarySpecificationItem.hasAssumptionLevel() ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ), Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, currentSecondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

															if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
																return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

															if( reversedUserSpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the created specification" );
															}
														}
													else
														{
														if( !isOlderExistingSpecification &&
														spanishMasculineSpecificationWordItem != null &&
														( spanishMasculineSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != null &&
														( obsoleteJustificationItem = spanishMasculineSpecificationItem.secondarySpecificationJustificationItem( reversedUserSpecificationItem ) ) != null )
															{
															if( ( justificationResult = currentContextWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, currentSecondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
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
									else
										{
										if( isOlderExistingSpecification &&
										!isOlderSecondarySpecification &&
										existingSpecificationItem != null &&
										userGeneralizationWordItem != null &&
										userSpecificationItem != null &&
										currentSecondarySpecificationItem == reversedUserSpecificationItem &&
										existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
										!userGeneralizationWordItem.hasCurrentlyConfirmedSpecification() &&
										userSpecificationItem.hasNonCompoundSpecificationCollection() &&
										// Justification doesn't exist in specification
										currentSecondarySpecificationItem.secondarySpecificationJustificationItem( existingSpecificationItem ) == null )
											{
											// Additional justification (User). Test file: "Complex (13)"
											if( ( justificationResult = generalizationWordItem.addJustification( false, true, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, null, null, existingSpecificationItem, null, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

											if( currentSecondarySpecificationItem.attachJustificationToSpecification( createdJustificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the current secondary specification" );
											}
										}

									// Typical for Spanish
									if( isSpanishCurrentLanguage )
										{
										if( isSpanishSpecificationFemaleWord )
											{
											if( ( existingSpecificationItem == null &&
											spanishMasculineSpecificationWordItem != null &&
											( nonCompoundSpanishSpecificationCollectionNr = spanishMasculineSpecificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&

											( ( spanishMasculineSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) == null ||
											spanishMasculineSpecificationItem.isSelfGenerated() ) ) &&

											// Draw Spanish possessive reversible conclusion
											( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, true, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, nonCompoundSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem, null, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish possessive reversible conclusion with generalization word \"" + currentContextWordItem.anyWordTypeString() + "\" and masculine specification word \"" + spanishMasculineSpecificationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											if( existingSpecificationItem != null &&
											currentContextWordItem.isUserGeneralizationWord &&
											existingSpecificationItem.hasCompoundSpecificationCollection() &&
											currentSecondarySpecificationItem.isSelfGeneratedConclusion() &&
											( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&

											// Add Spanish non-compound possessive reversible conclusion
											( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, nonCompoundSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, currentSecondarySpecificationItem, null, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish non-compound possessive reversible conclusion with generalization word \"" + currentContextWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
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

	private byte drawProperNounPartOfConclusions( boolean hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, boolean isArchivedAssignment, boolean isCheckingForArchivedAssignment, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem spanishRelationWordItem )
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
			if( !isArchivedAssignment &&

			( primarySpecificationItem.isUserSpecification() ||
			generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation() ) &&

			( tempSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null )
				primarySpecificationItem = tempSpecificationItem;
			}

		if( primarySpecificationItem != null &&
		( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
			{
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

					if( partOfSpecificationItem == null ||
					primarySpecificationItem.assumptionLevel() < partOfSpecificationItem.assumptionLevel() ||

					( primarySpecificationItem.assumptionLevel() == partOfSpecificationItem.assumptionLevel() &&

					( !primarySpecificationItem.hasRelationContext() ||
					primarySpecificationItem.isPossessive() ||
					// Typical for Spanish
					!partOfSpecificationItem.hasPossessivePrimarySpecificationJustification() ) ) ||

					// Typical for Spanish
					( primarySpecificationItem.hasRelationContext() &&

					( ( partOfSpecificationItem.isConcludedAssumption() &&

					( primarySpecificationItem.isPossessive() ||
					generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() ) ) ||

					( !partOfSpecificationItem.hasAssumptionLevel() &&
					primarySpecificationItem.isPossessive() &&
					primarySpecificationItem.isOlderItem() ) ) ) )
						{
						isPrimarySpecificationArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

						// Typical for Spanish
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
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, false, true, false, false, false, false, false, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, partOfDefinitionSpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ) ).result != Constants.RESULT_OK )
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
							( assumptionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, spanishRelationWordItem ) ) != null )
								{
								// Additional justification (unhidden)
								// Typical for Spanish. Test files: "Complejo (13)" and "Complejo (18)"
								if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, primarySpecificationItem, null, oppositePossessiveDefinitionSpecificationItem, null, assumptionSpecificationItem.firstJustificationItem() ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add an unhidden Spanish justification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
									return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

								// Copy specification
								if( ( createAndAssignResult = generalizationWordItem.copySpecificationItem( assumptionSpecificationItem.specificationCollectionNr(), createdJustificationItem, assumptionSpecificationItem ) ).result != Constants.RESULT_OK )
									return adminItem_.startError( 1, moduleNameString_, "I failed to copy the assumption specification with the created justification item" );

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
		adjustedQuestionWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

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
		adjustedSpanishQuestionSecondarySpecificationItem_ = null;
		adjustedQuestionWordItem_ = null;
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
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem.suggestiveQuestionAssumptionSecondarySpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
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

		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
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

		if( !generalizationWordItem.isProperNounWord() )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item isn't a proper noun" );

		// Active assignments
		if( ( secondarySpecificationItem = generalizationWordItem.firstSelectedRelationSpecificationItem( true, false ) ) != null &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions with assignments" );

		// Archived assignments
		if( ( secondarySpecificationItem = generalizationWordItem.firstSelectedRelationSpecificationItem( true, true ) ) != null &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions with assignments" );

		// Specifications
		if( ( secondarySpecificationItem = generalizationWordItem.firstSelectedRelationSpecificationItem( false, false ) ) != null &&
		drawPossessiveReversibleConclusions( secondarySpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions with specifications" );

		return Constants.RESULT_OK;
		}

	protected byte drawProperNounPartOfConclusions( boolean hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, boolean isArchivedAssignment, WordItem generalizationProperNounWordItem, WordItem specificationWordItem, WordItem spanishRelationWordItem )
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
					drawProperNounPartOfConclusions( hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, isArchivedAssignment, true, currentProperNounWordItem, specificationWordItem, spanishRelationWordItem ) != Constants.RESULT_OK )
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

	protected WordItem adjustedQuestionWordItem()
		{
		return adjustedQuestionWordItem_;
		}
	};

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
