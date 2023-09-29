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

package org.mafait.thinknowlogy;

class AdminReasoning
	{
	// Private constructed variables

	private boolean hasSuggestiveQuestionAssumption_ = false;

	private SpecificationItem adjustedSpanishQuestionSecondarySpecificationItem_ = null;
	private SpecificationItem alternativeSpanishAdditionalDefinitionSpecificationItem_ = null;
	private SpecificationItem oppositePossessiveDefinitionSpecificationItem_ = null;
	private SpecificationItem possessiveDefinitionSpecificationItem_ = null;

	private WordItem feminineSpecificationWordItem_ = null;
	private WordItem firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;
	private WordItem secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private static boolean isFeminineOrMasculineProperNounEnding( boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isFeminineSpecificationWord;
		boolean isMasculineSpecificationWord;

		if( generalizationWordItem != null &&
		specificationWordItem != null )
			{
			isFeminineSpecificationWord = specificationWordItem.isFeminineWord();
			isMasculineSpecificationWord = specificationWordItem.isMasculineWord();

			if( isFeminineSpecificationWord ||
			isMasculineSpecificationWord )
						// Possessive (relation)
				return ( ( isPossessive &&
						relationWordItem != null &&

						( ( isFeminineSpecificationWord &&
						relationWordItem.hasFeminineProperNounEnding() ) ||

						( isMasculineSpecificationWord &&
						relationWordItem.hasMasculineProperNounEnding() ) ) ) ||

						// Non-possessive (generalization)
						( !isPossessive &&

						( ( isFeminineSpecificationWord &&
						generalizationWordItem.hasFeminineProperNounEnding() ) ||

						( isMasculineSpecificationWord &&

						( generalizationWordItem.hasMasculineProperNounEnding() ||

						// Typical for Spanish
						( specificationWordItem.isNounWordSpanishAmbiguous() &&
						generalizationWordItem.hasFeminineProperNounEnding() ) ) ) ) ) );
			}

		return false;
		}

	private byte askQuestions( SpecificationItem userSpecificationItem )
		{
		boolean isAllowingSpanishPossessiveSpecification;
		boolean isArchivedUserAssignment;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		int userSpecificationCollectionNr;
		SpecificationItem additionalDefinitionSpecificationItem = null;
		WordItem currentTouchedWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
		userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
		userSpecificationWordItem = userSpecificationItem.specificationWordItem();

		if( userSpecificationItem.isGeneralizationProperNoun() &&
		( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null )
			{
			additionalDefinitionSpecificationItem = getAdditionalDefinitionSpecificationItem( userSpecificationWordItem );

			if( userSpecificationItem.isExclusiveSpecification() )
				{
				// Question part doesn't exist yet
				if( !userGeneralizationWordItem.hasJustification( userSpecificationItem, null, null ) &&
				// Ask question after more specific non-exclusive user specification
				adminItem_.addSelfGeneratedSpecification( false, isArchivedUserAssignment, isArchivedUserAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, userGeneralizationWordItem, userSpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about the current user sentence" );
				}
			else
				{
				if( userSpecificationItem.isPossessive() )
					{
					if( !userGeneralizationWordItem.hasFeminineOrMasculineProperNounEnding() &&

					( ( isSpanishCurrentLanguage &&
					userSpecificationItem.isSpecificationWordSpanishAmbiguous() ) ||

					!userGeneralizationWordItem.hasActiveQuestionWithCompoundCollection() ) &&

					// Ask questions about generalization word with possessive primary specification
					askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( isArchivedUserAssignment, true, isSpanishCurrentLanguage, userSpecificationItem.specificationCollectionNr(), userSpecificationItem, additionalDefinitionSpecificationItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about word \"" + userGeneralizationWordItem.anyWordTypeString() + "\" with possessive user specification as primary specification" );
					}
				else
					{
					if( userGeneralizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
					// Avoid idle call
					userGeneralizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( false ) != null &&
					// Correct suggestive assumptions by opposite suggestive question
					correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( userSpecificationItem, userGeneralizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question in word \"" + userGeneralizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			// Do for all words touched during current sentence
			do	{
				// Typical for Spanish
				isAllowingSpanishPossessiveSpecification = ( isSpanishCurrentLanguage &&
															currentTouchedWordItem.isUserRelationWord );

				if( currentTouchedWordItem.firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) != null )
					{
					// Ask questions
					if( askQuestions( isAllowingSpanishPossessiveSpecification, userSpecificationCollectionNr, additionalDefinitionSpecificationItem, userSpecificationItem, currentTouchedWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper noun word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );

					if( additionalDefinitionSpecificationItem != null &&
					// Avoid idle call
					currentTouchedWordItem.firstExclusiveSpecificationItem( false ) != null &&
					// Ask questions with question as primary specification
					askQuestionsWithQuestionAsPrimarySpecification( userSpecificationItem, currentTouchedWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions about proper noun word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestions( boolean isAllowingSpanishPossessiveSpecification, int userSpecificationCollectionNr, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasExclusiveSecondarySpecificationCompoundCollection;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasPrimarySpecificationRelationContext;
		boolean hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection;
		boolean hasSkippedAskingQuestion;
		boolean hasUserSpecificationCompoundCollection;
		boolean hasUserSpecificationRelationContext;
		boolean isArchivedAssignment;
		boolean isCompoundGeneralizationWordSpanishAmbiguous;
		boolean isOlderPrimarySpecification;
		boolean isOlderUserSpecification;
		boolean isPossessivePrimarySpecification;
		boolean isPossessiveUserSpecification;
		boolean isPrimarySameAsUserSpecification;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		boolean isPrimaryUserSpecification;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isUserGeneralizationWord;
		boolean isUserQuestion;
		boolean isUserSpecificationWordSpanishAmbiguous;
		short generalizationWordTypeNr;
		int exclusiveSecondarySpecificationCollectionNr;
		int primarySpecificationCollectionNr;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem archivedPrimarySpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem exclusiveSecondarySpecificationItem;
		SpecificationItem exclusiveSpecificationItem;
		SpecificationItem existingQuestionSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem questionPrimarySpecificationItem;
		SpecificationItem questionSecondarySpecificationItem;
		WordItem collectionWordItem;
		WordItem compoundGeneralizationWordItem;
		WordItem exclusiveSpecificationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem primarySpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem.firstCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
			isOlderUserSpecification = userSpecificationItem.isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserQuestion = userSpecificationItem.isQuestion();
			isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();

			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			do	{
				if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null )
					{
					hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
					isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();
					isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();
					isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
					isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();
					primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

					if( isPossessivePrimarySpecification )
						{
						if( !hasGeneralizationWordFeminineOrMasculineProperNounEnding )
							{
							archivedPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, false, primarySpecificationWordItem );

							// Typical for Spanish
							// Specification with user specification collection doesn't exist yet
							if( generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) == null &&
							// Ask questions about generalization word with possessive primary specification
							askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( isArchivedAssignment, isPossessiveUserSpecification, true, userSpecificationCollectionNr, ( archivedPrimarySpecificationItem != null && archivedPrimarySpecificationItem.isArchivedAssignment() ? archivedPrimarySpecificationItem : primarySpecificationItem ), additionalDefinitionSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to ask Spanish questions about word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
							}
						}
					else
						{
						hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();

						if( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
						hasPrimarySpecificationCompoundCollection ||
						isPrimarySpecificationWordSpanishAmbiguous ||

						( isPrimarySameAsUserSpecification &&
						!hasPrimarySpecificationRelationContext ) ) &&

						( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
						( exclusiveSecondarySpecificationItem = primarySpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
							{
							hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection = generalizationWordItem.hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();
							hasSkippedAskingQuestion = false;

							isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
							isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();

							generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();

							alternativePrimarySpecificationItem = null;

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
									userSpecificationItem.nContextRelations() > 1 &&
									( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
									// Skip invalid questions / similar specification doesn't exist yet
									generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, exclusiveSecondarySpecificationCollectionNr, null ) == null &&
									// Ask Spanish question about word relation word with possessive primary specification
									askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( isArchivedAssignment, generalizationWordTypeNr, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to ask a Spanish question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with possessive primary specification" );
									}
								else
									{
									if( primarySpecificationItem.isSelfGeneratedAssumption() )
										alternativePrimarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, primarySpecificationWordItem, null );
									}
								}

							do	{
								hasExclusiveSecondarySpecificationCompoundCollection = exclusiveSecondarySpecificationItem.hasCompoundSpecificationCollection();

								if( ( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null )
									{
									exclusiveSpecificationItem = null;
									foundSpecificationItem = null;

									if( ( exclusiveSpecificationItem = exclusiveSecondarySpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
										{
										// Do for all exclusive specifications of this word (including adjectives)
										do	{
											if( ( exclusiveSpecificationWordItem = exclusiveSpecificationItem.specificationWordItem() ) != null )
												foundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, exclusiveSpecificationWordItem );
											}
										while( foundSpecificationItem == null &&
										( exclusiveSpecificationItem = exclusiveSpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
										}

									if( foundSpecificationItem == null )
										{
										if( ( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
											{
											isCompoundGeneralizationWordSpanishAmbiguous = ( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem.compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null &&
																							compoundGeneralizationWordItem.isNounWordSpanishAmbiguous() );

											existingQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem );

											// Useful user specification without relation
											if( ( ( hasPrimarySpecificationCompoundCollection &&
											isPrimaryUserSpecification &&
											!exclusiveSecondarySpecificationItem.isSpecificationAdjective() &&

											( !isUserSpecificationWordSpanishAmbiguous ||

											// Typical for Spanish
											( isPrimarySameAsUserSpecification &&
											!hasPrimarySpecificationRelationContext &&
											!exclusiveSecondarySpecificationWordItem.isFeminineWord() ) ) ) ||

											// No useful user specification without relation
											( !hasUserSpecificationCompoundCollection &&
											!hasPrimarySpecificationRelationContext &&
											!isOlderPrimarySpecification &&

											( ( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
											hasPrimarySpecificationCompoundCollection ) ||

											( isPrimaryUserSpecification &&
											exclusiveSecondarySpecificationItem.isUserSpecification() ) ) ) ||

											( hasExclusiveSecondarySpecificationCompoundCollection &&
											hasUserSpecificationRelationContext &&

											( ( hasPrimarySpecificationCompoundCollection &&

											( hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
											!exclusiveSecondarySpecificationWordItem.isFeminineWord() ) ) ||

											( !isUserGeneralizationWord &&

											( !isPossessiveUserSpecification ||

											( !isUserSpecificationWordSpanishAmbiguous &&
											primarySpecificationWordItem.isUserSpecificationWord ) ) ) ) ) ||

											// Added additional justification
											( !hasExclusiveSecondarySpecificationCompoundCollection &&
											!hasPrimarySpecificationRelationContext &&
											hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection &&

											( ( existingQuestionSpecificationItem == null &&
											!hasUserSpecificationCompoundCollection ) ||

											( existingQuestionSpecificationItem != null &&
											!existingQuestionSpecificationItem.hasSecondarySpecificationCompoundCollectionJustification() ) ) ) ||

											( hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection &&

											// Corrected assumption by opposite suggestive question
											( hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ||
											// Avoid adding non-compound justification to compound question
											primarySpecificationCollectionNr > Constants.NO_COLLECTION_NR ) ) ) &&

											( ( isCompoundGeneralizationWordSpanishAmbiguous &&
											!isUserSpecificationWordSpanishAmbiguous ) ||

											// Typical for Spanish
											// Question part doesn't exist yet
											!generalizationWordItem.hasJustification( ( alternativePrimarySpecificationItem == null ? primarySpecificationItem : alternativePrimarySpecificationItem ), null, exclusiveSecondarySpecificationItem ) ) )
												{
												// Adjusted question
												if( alternativePrimarySpecificationItem != null ||

												( hasPrimarySpecificationCompoundCollection &&
												hasPrimarySpecificationRelationContext &&
												isCompoundGeneralizationWordSpanishAmbiguous &&
												!isUserSpecificationWordSpanishAmbiguous ) ||

												( isUserSpecificationWordSpanishAmbiguous &&
												compoundGeneralizationWordItem != primarySpecificationWordItem &&

												( !hasPrimarySpecificationRelationContext ||
												// Same specification but without relation context doesn't exist
												// Spanish test file: "Esta información es más específico"
												generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, primarySpecificationWordItem ) == null ) ) ||

												// Has possibly a gap in the knowledge
												( ( !isOlderPrimarySpecification ||
												isUserQuestion ) &&

												( !hasUserSpecificationCompoundCollection ||

												// Negative specification doesn't exist yet
												( generalizationWordItem.firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == null &&
												generalizationWordItem.hasPossiblyGapInKnowledge( exclusiveSecondarySpecificationCollectionNr, primarySpecificationCollectionNr, primarySpecificationItem ) ) ) ) )
													{
													questionPrimarySpecificationItem = ( alternativePrimarySpecificationItem == null ? primarySpecificationItem :
																																		alternativePrimarySpecificationItem );
													questionSecondarySpecificationItem = ( isPrimarySpecificationWordSpanishAmbiguous &&
																						isUserGeneralizationWord &&
																						adjustedSpanishQuestionSecondarySpecificationItem_ != null ? adjustedSpanishQuestionSecondarySpecificationItem_ :
																																					exclusiveSecondarySpecificationItem );

													if( ( collectionWordItem = exclusiveSecondarySpecificationWordItem.collectionWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null &&
													// Avoid asking invalid questions:
													// 1) Check for existing knowledge with same specification collection
													generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, exclusiveSecondarySpecificationWordItem ) == null &&
													// 2) Check for existing negative specification
													generalizationWordItem.firstSpecificationItem( true, false, false, exclusiveSecondarySpecificationWordItem ) == null &&
													// 3) Check for existing negative specification. Test file: "Complex (3)"
													generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, true, false, collectionWordItem, null ) == null &&

													( existingQuestionSpecificationItem == null ||

													( ( !isSpanishCurrentLanguage ||
													// Typical for Spanish
													// Avoid almost duplicate justifications
													!hasPrimarySpecificationRelationContext ||
													!isPossessiveUserSpecification ) &&

													// Justification doesn't exist yet in specification
													( !existingQuestionSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( questionPrimarySpecificationItem, questionSecondarySpecificationItem ) &&
													// Spanish test file: "Que haya confirmado al menos una palabra de relación"
													// Justification doesn't exist yet in word
													generalizationWordItem.primaryAndSecondarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, questionPrimarySpecificationItem, questionSecondarySpecificationItem ) == null ) ) ) )
														{
														// Ask a specification substitution question
														if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, questionPrimarySpecificationItem, null, questionSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );

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
															if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
															createdOrFoundSpecificationItem.isCorrectedSpecification() )
																{
																if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ == null )
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
													primarySpecificationWordItem.hasCollectionNr( userSpecificationCollectionNr ) &&
													exclusiveSecondarySpecificationWordItem.nonCompoundCollectionNr() > Constants.NO_COLLECTION_NR &&
													// Question exists
													generalizationWordItem.firstSpecificationItem( false, false, true, exclusiveSecondarySpecificationWordItem ) != null &&
													// Find question to be adjusted by compound collection
													findQuestionToBeAdjustedByCompoundCollection( generalizationWordItem, primarySpecificationWordItem, userSpecificationItem, exclusiveSecondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection about word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );
													}
												}
											}
										}
									else
										{
										// Test file: "Skip invalid questions (1)"
										if( generalizationWordItem != exclusiveSecondarySpecificationWordItem && 
										// Specification doesn't exist yet
										generalizationWordItem.firstSpecificationItem( false, false, false, exclusiveSecondarySpecificationWordItem ) == null &&
										// Make exclusive specification substitution assumption
										adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, exclusiveSecondarySpecificationItem, exclusiveSpecificationItem, foundSpecificationItem, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							while( !hasSkippedAskingQuestion &&
							( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
							}
						}
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem.nextCandidateForQuestionSpecificationItem( isAllowingSpanishPossessiveSpecification ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestionsAboutGeneralizationWordWithPossessivePrimarySpecification( boolean isArchivedAssignment, boolean isPossessiveUserSpecification, boolean isSpanishCurrentLanguage, int userSpecificationCollectionNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasMultipleUserGeneralizationWords = false;
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasPrimarySpecificationRelationContext;
		boolean isOlderPrimarySpecification;
		boolean isUserGeneralizationWord;
		short generalizationWordTypeNr;
		int exclusiveSecondarySpecificationCollectionNr;
		SpecificationItem exclusiveSecondarySpecificationItem;
		SpecificationItem existingQuestionSpecificationItem;
		WordItem additionalDefinitionSpecificationWordItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem compoundGeneralizationWordItem = null;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given additional definition specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		// Question about generalization word
		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem.specificationWordItem() ) != null &&
		( exclusiveSecondarySpecificationItem = additionalDefinitionSpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
			{
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();
			hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();

			if( isSpanishCurrentLanguage )
				hasMultipleUserGeneralizationWords = ( GlobalVariables.nUserGeneralizationWords > 1 );

			do	{
				// Current question part doesn't exist yet
				if( !generalizationWordItem.hasJustification( primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationCollectionNr = exclusiveSecondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null &&

				// No answered questions
				( ( ( existingQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS ) ) == null &&
				isUserGeneralizationWord &&
				hasPrimarySpecificationRelationContext &&
				// User specification doesn't exist yet
				generalizationWordItem.firstUserSpecificationItem( false, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) == null ) ||

				// Typical for Spanish
				( isSpanishCurrentLanguage &&

				( ( isUserGeneralizationWord ||

				( isOlderPrimarySpecification &&
				!isPossessiveUserSpecification ) ||

				( hasMultipleUserGeneralizationWords ||

				( hasPrimarySpecificationCompoundCollection &&
				!isPossessiveUserSpecification ) ) ) &&

				( exclusiveSecondarySpecificationItem.isSelfGeneratedSpecification() ||

				( ( compoundGeneralizationWordItem = exclusiveSecondarySpecificationWordItem.compoundGeneralizationWordItem( exclusiveSecondarySpecificationCollectionNr ) ) != null &&
				!compoundGeneralizationWordItem.isNounWordSpanishAmbiguous() ) ) &&

				// Positive specification doesn't exist yet
				generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, null ) == null &&
				// Negative specification doesn't exist yet
				generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, exclusiveSecondarySpecificationCollectionNr, null ) == null &&

				( existingQuestionSpecificationItem == null ||
				// Justification has at least the same assumption level
				existingQuestionSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, null ) ) ) ) ) &&

				// Ask a specification substitution question
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte askQuestionsWithQuestionAsPrimarySpecification( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean hasUserSpecificationCompoundCollection;
		boolean isArchivedAssignment;
		boolean isFeminineOrMasculineUserSpecificationWord;
		boolean isPossessiveUserSpecification;
		boolean isUserRelationWord;
		SpecificationItem existingSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem( false ) ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
			{
			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			isFeminineOrMasculineUserSpecificationWord = userSpecificationWordItem.isFeminineOrMasculineWord();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();

			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( ( hasUserSpecificationCompoundCollection ||
				primarySpecificationItem.hasCompoundSpecificationCollection() ) &&

				( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
				( secondarySpecificationItem = primarySpecificationWordItem.firstNonCompoundUserSpecificationItem() ) != null &&
				( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
				// Question part doesn't exist yet as specification
				generalizationWordItem.firstSpecificationItem( false, false, false, secondarySpecificationWordItem ) == null &&

				( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
				isUserRelationWord ) ||

				// Skip adjective specification words of user generalization word
				!secondarySpecificationItem.isSpecificationAdjective() ) &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == null ||
				// Justification doesn't exist yet in question
				!existingSpecificationItem.hasSecondarySpecificationJustification( secondarySpecificationItem ) ) &&

				( !isFeminineOrMasculineUserSpecificationWord ||
				!isPossessiveUserSpecification ||
				// Typical for Spanish
				existingSpecificationItem != null ) )
					{
					isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

					// Ask a specification substitution question
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( primarySpecificationItem = primarySpecificationItem.nextExclusiveSpecificationItem( false ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte askSpanishQuestionAboutRelationWordWithPossessivePrimarySpecification( boolean isArchivedAssignment, short generalizationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationItem exclusiveSecondarySpecificationItem;
		WordItem exclusiveSecondarySpecificationWordItem;
		WordItem userSpecificationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( exclusiveSecondarySpecificationItem = userSpecificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( !generalizationWordItem.hasJustification( userSpecificationItem, null, exclusiveSecondarySpecificationItem ) &&
				( exclusiveSecondarySpecificationWordItem = exclusiveSecondarySpecificationItem.specificationWordItem() ) != null &&
				// Ask a specification substitution question
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, exclusiveSecondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, exclusiveSecondarySpecificationItem, null, generalizationWordItem, exclusiveSecondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + exclusiveSecondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSecondarySpecificationItem = exclusiveSecondarySpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByKnowledge( SpecificationItem createdSpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem createdSpecificationWordItem )
		{
		SpecificationItem obsoleteSpecificationItem;
		WordItem userSpecificationWordItem;

		if( createdSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given created specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( createdSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given created specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( obsoleteSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, createdSpecificationWordItem ) ) != null )
			{
			// Write invalid positive assumption by knowledge
			if( generalizationWordItem.writeUpdatedSpecification( false, true, false, false, false, false, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid positive assumption by knowledge about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			// Replace invalid positive assumption by knowledge
			if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid positive assumption by knowledge in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		if( ( obsoleteSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, userSpecificationWordItem ) ) != null )
			{
			// Write invalid user assumption by knowledge
			if( generalizationWordItem.writeUpdatedSpecification( false, true, false, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid user assumption by knowledge about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			// Replace invalid user assumption by knowledge
			if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, userSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid user assumption by knowledge in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		int obsoleteSpecificationCollectionNr;
		SpecificationItem obsoleteSpecificationItem;
		WordItem obsoleteSpecificationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		// Replace all invalid assumptions by opposite suggestive question
		if( ( obsoleteSpecificationItem = generalizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( false ) ) != null )
			{
			do	{
				if( ( obsoleteSpecificationCollectionNr = obsoleteSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
				( obsoleteSpecificationWordItem = obsoleteSpecificationItem.specificationWordItem() ) != null &&
				obsoleteSpecificationWordItem.isExclusiveCollection( obsoleteSpecificationCollectionNr ) )
					{
					// Write invalid assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem.writeUpdatedSpecification( false, false, true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid assumption by opposite suggestive question (obsolete) in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					// Replace invalid assumption by opposite suggestive question (obsolete)
					if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, userSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid assumption by opposite suggestive question (obsolete) in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					obsoleteSpecificationItem = generalizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( false );
					}
				else
					obsoleteSpecificationItem = obsoleteSpecificationItem.nextOlderNonPossessiveNonQuestionSpecificationItem();
				}
			while( obsoleteSpecificationItem != null );
			}

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem createdNewSpecificationItem;
		SpecificationItem createdOldSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem oldSuggestiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is no assignment" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		// Previously, a suggestive question assumption was made.
		// However, now a question with the opposite specification is asked.
		// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
		if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem.suggestiveQuestionAssumptionSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );

		if( !oldSuggestiveQuestionAssumptionAssignmentItem.isActiveAssignment() )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );

		if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );

		// Correct old question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, oldSuggestiveQuestionAssumptionAssignmentItem, null, null, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdOldSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );

		// Correct new question
		if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite suggestive question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

		if( ( createdNewSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't create the new suggestive question assumption" );

		// Replace created old specification by created new specification in justifications of involved word
		if( generalizationWordItem.updateSpecificationOfJustificationsInWord( createdOldSpecificationItem, createdNewSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to update a specification in justifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		if( firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != null )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem.firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add first old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the first old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );

			if( ( definitionSpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, false, false, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add first new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdNewSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the first new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );
			}

		if( secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ != null )
			{
			if( ( definitionSpecificationItem = oldSuggestiveSpecificationWordItem.firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add second old created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the second old created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );

			if( ( definitionSpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ ) ) != null &&
			// Add second new created specification justification to corrected assumption (by opposite suggestive question)
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdNewSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add the second new created specification justification to corrected assumption (by opposite suggestive question) about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeAdjectiveConclusions( boolean isArchivedAssignment, short generalizationWordTypeNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem primarySpecificationWordItem;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( !primarySpecificationItem.isSpecificationAdjective() )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no adjective specification" );

		if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = primarySpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( secondarySpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, primarySpecificationWordItem ) ) != null &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == null ||

				// User specification doesn't exist yet
				( existingSpecificationItem.isSelfGeneratedSpecification() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem, null ) ) ) &&

				// Draw negative adjective conclusion
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean isPrimarySameAsSecondarySpecification;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		int additionalDefinitionSpecificationCollectionNr;
		SpecificationItem additionalProperNounSpecificationItem = null;
		SpecificationItem negativeConclusionPrimarySpecificationItem = null;
		SpecificationItem negativeConclusionAdditionalDefinitionSpecificationItem = null;
		SpecificationItem negativeConclusionSecondarySpecificationItem = null;
		SpecificationItem negativeDefinitionSpecificationItem = null;
		WordItem additionalDefinitionSpecificationWordItem;
		WordItem primarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( additionalDefinitionSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given additional definition specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( additionalDefinitionSpecificationWordItem = additionalDefinitionSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given additional definition specification item has no specification word item" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();
		isPrimarySameAsSecondarySpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );

		additionalDefinitionSpecificationCollectionNr = additionalDefinitionSpecificationItem.specificationCollectionNr();

		if( ( negativeDefinitionSpecificationItem = secondarySpecificationWordItem.firstSpecificationItem( true, false, false, additionalDefinitionSpecificationWordItem ) ) != null &&
		!isPrimarySpecificationWordSpanishAmbiguous )
			{
			// Test file: "Simple family definition - John and Anna - Paul, Joe and Laura"
			negativeConclusionPrimarySpecificationItem = secondarySpecificationItem;
			negativeConclusionSecondarySpecificationItem = negativeDefinitionSpecificationItem;
			}
		else
			{
			if( isPrimarySpecificationWordSpanishAmbiguous &&
			negativeDefinitionSpecificationItem != null )
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

				if( negativeConclusionSecondarySpecificationItem == null )
					// Typical for Spanish
					negativeConclusionSecondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, additionalDefinitionSpecificationCollectionNr, null );
				}
			}

		if( negativeConclusionSecondarySpecificationItem == null )
			{
			// Typical for Spanish
			// Spanish test file: "Reto científico"
			if( generalizationWordItem.firstSpecificationItem( false, false, true, additionalDefinitionSpecificationWordItem ) == null &&
			// Ask a specification substitution question
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, null, negativeConclusionAdditionalDefinitionSpecificationItem, null, generalizationWordItem, additionalDefinitionSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + additionalDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
			}
		else
			{
			// Draw negative conclusion
			if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, additionalDefinitionSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, negativeConclusionPrimarySpecificationItem, negativeConclusionAdditionalDefinitionSpecificationItem, negativeConclusionSecondarySpecificationItem, null, generalizationWordItem, additionalDefinitionSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + additionalDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeDefinitionConclusions( boolean isArchivedAssignment, short generalizationWordTypeNr, short negativeSpecificationWordTypeNr, WordItem generalizationWordItem, WordItem negativeSpecificationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem definitionSpecificationItem = null;
		SpecificationItem existingSpecificationItem;
		SpecificationItem positiveAdjectiveDefinitionSpecificationItem;
		SpecificationItem positiveSpecificationItem;
		SpecificationItem negativeAdjectiveDefinitionSpecificationItem;
		SpecificationItem secondAdjectiveDefinitionSpecificationItem;
		WordItem oppositeSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( negativeSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given negative specification word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( positiveAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem.firstAdjectiveSpecificationItem( false ) ) != null )
			{
			// Try to find negative first
			if( ( secondAdjectiveDefinitionSpecificationItem = generalizationWordItem.firstAdjectiveSpecificationItem( true ) ) != null ||
			( secondAdjectiveDefinitionSpecificationItem = generalizationWordItem.firstAdjectiveSpecificationItem( false ) ) != null )
				{
				// Draw simple negative adjective conclusion
				if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, null, secondAdjectiveDefinitionSpecificationItem, null, negativeSpecificationWordItem, generalizationWordItem, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative adjective conclusion about generalization word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				definitionSpecificationItem = createAndAssignResult.createdSpecificationItem;
				}

			if( ( positiveAdjectiveDefinitionSpecificationItem = generalizationWordItem.firstAdjectiveSpecificationItem( false ) ) != null &&
			( negativeAdjectiveDefinitionSpecificationItem = negativeSpecificationWordItem.firstAdjectiveSpecificationItem( true ) ) != null &&

			// Specification doesn't exist yet
			( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == null ||

			// Justification doesn't exist yet in specification
			( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveAdjectiveDefinitionSpecificationItem, negativeAdjectiveDefinitionSpecificationItem ) &&
			// Justification has at least the same assumption level
			existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveAdjectiveDefinitionSpecificationItem, null, negativeAdjectiveDefinitionSpecificationItem, null ) ) ) &&

			// Draw another simple negative adjective conclusion
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveAdjectiveDefinitionSpecificationItem, null, negativeAdjectiveDefinitionSpecificationItem, null, generalizationWordItem, negativeSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw another simple negative adjective conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );

			if( definitionSpecificationItem != null &&
			( oppositeSpecificationWordItem = generalizationWordItem.collectionWordItem( definitionSpecificationItem.specificationCollectionNr() ) ) != null &&
			( positiveSpecificationItem = oppositeSpecificationWordItem.firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != null )
				{
				// Specification doesn't exist yet
				if( ( ( existingSpecificationItem = oppositeSpecificationWordItem.firstSpecificationItem( true, false, false, generalizationWordItem ) ) == null ||

				// Justification doesn't exist yet in specification
				( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, null, definitionSpecificationItem, null ) ) ) &&

				// Draw opposite simple negative conclusion
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveSpecificationItem, null, definitionSpecificationItem, null, oppositeSpecificationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw an opposite simple negative conclusion about generalization word \"" + oppositeSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				// Test file: "James was the father of Peter (before family definition)"
				if( existingSpecificationItem != null &&
				// Recalculate assumption level of existing specification
				existingSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the existing specification" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte drawNegativeExclusiveConclusions( boolean isArchivedAssignment, short generalizationWordTypeNr, SpecificationItem negativePrimarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem exclusiveSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( negativePrimarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given negative primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( exclusiveSpecificationItem = specificationWordItem.firstExclusiveSpecificationItem( true ) ) != null )
			{
			// Do for all exclusive specifications of this word (including adjectives)
			do	{
				if( ( secondarySpecificationWordItem = exclusiveSpecificationItem.specificationWordItem() ) != null &&
				// Draw exclusive negative compound conclusion
				adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, exclusiveSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, negativePrimarySpecificationItem, null, exclusiveSpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw an exclusive negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
				}
			while( ( exclusiveSpecificationItem = exclusiveSpecificationItem.nextExclusiveSpecificationItem( true ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawOnlyOptionLeftProperNounConclusion( SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasCreatedSpecification = false;
		boolean isArchivedAssignment;
		boolean hasFoundMoreMatches = false;
		int currentSpecificationCollectionNr;
		SpecificationItem currentSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem foundDefinitionSpecificationItem = null;
		SpecificationItem negativeSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem foundDefinitionSpecificationWordItem;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) != null )
			{
			do	{
				if( currentSpecificationItem.isExclusiveSpecification() &&
				( currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
				( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				( negativeSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != null &&
				( definitionSpecificationItem = specificationWordItem.firstExclusiveSpecificationItem( false ) ) != null )
					{
					do	{
						if( definitionSpecificationItem.specificationWordItem() != currentSpecificationWordItem &&
						// Positive specification doesn't exist yet
						generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, null ) == null &&
						definitionSpecificationItem.specificationCollectionNr() == currentSpecificationCollectionNr )
							{
							if( foundDefinitionSpecificationItem == null )
								foundDefinitionSpecificationItem = definitionSpecificationItem;
							else
								hasFoundMoreMatches = true;
							}
						}
					while( !hasFoundMoreMatches &&
					( definitionSpecificationItem = definitionSpecificationItem.nextExclusiveSpecificationItem( false ) ) != null );

					if( !hasFoundMoreMatches &&
					foundDefinitionSpecificationItem != null &&
					( foundDefinitionSpecificationWordItem = foundDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						isArchivedAssignment = primarySpecificationItem.isArchivedAssignment();

						// Draw an only option left conclusion
						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, primarySpecificationItem.generalizationWordTypeNr(), primarySpecificationItem.specificationWordTypeNr(), primarySpecificationItem.relationWordTypeNr(), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, foundDefinitionSpecificationItem, negativeSpecificationItem, null, generalizationWordItem, foundDefinitionSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						hasCreatedSpecification = true;
						}
					}
				}
			while( !hasCreatedSpecification &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawProperNounPartOfConclusions( boolean isSpanishCurrentLanguage, SpecificationItem primarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasPrimarySpecificationRelationContext;
		boolean hasUserSpecificationCompoundCollection;
		boolean isOlderPrimarySpecification;
		boolean isOlderUserSpecification;
		boolean isPossessivePrimarySpecification;
		boolean isPossessiveUserSpecification;
		boolean isPrimarySpecificationArchivedAssignment;
		boolean isPrimaryUserSpecification;
		boolean isUserGeneralizationWord;
		boolean isUserRelationWord;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem partOfDefinitionSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		WordItem currentPossessiveNounWordItem;
		WordItem spanishMasculineSpecificationWordItem = null;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The specification word of the given user specification item is undefined" );

		if( ( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
			{
			hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();
			isPrimarySpecificationArchivedAssignment = primarySpecificationItem.isArchivedAssignment();
			isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();

			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			isOlderUserSpecification = userSpecificationItem.isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();

			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			if( hasPrimarySpecificationRelationContext &&
			isOlderPrimarySpecification &&
			!isPossessiveUserSpecification &&
			!isPrimaryUserSpecification &&
			primarySpecificationItem.hasAssumptionLevel() &&
			primarySpecificationItem.hasCurrentCreationSentenceNr() &&
			// Recalculate assumption level of primary specification
			primarySpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the primary specification" );

			// Typical for Spanish
			if( isSpanishCurrentLanguage &&
			specificationWordItem.isFeminineWord() &&
			!adminItem_.isUserQuestion() )
				spanishMasculineSpecificationWordItem = specificationWordItem.masculineCollectionWordItem();

			// Do for all possessive noun words
			do	{
				if( ( partOfDefinitionSpecificationItem = currentPossessiveNounWordItem.partOfSpecificationItem( specificationWordItem ) ) == null &&
				spanishMasculineSpecificationWordItem != null )
					partOfDefinitionSpecificationItem = currentPossessiveNounWordItem.partOfSpecificationItem( spanishMasculineSpecificationWordItem );

				if( partOfDefinitionSpecificationItem != null &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, currentPossessiveNounWordItem ) ) == null ||

				( ( !isOlderPrimarySpecification ||
				// Typical for Spanish
				!isUserRelationWord ||
				!existingSpecificationItem.isSelfGeneratedAssumption() ||
				!primarySpecificationItem.isSelfGeneratedConclusion() ) &&

				// Justification doesn't exist yet in word
				!generalizationWordItem.hasJustification( primarySpecificationItem, null, partOfDefinitionSpecificationItem ) &&
				// Justification has at least the same assumption level
				existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, partOfDefinitionSpecificationItem, null ) ) ) )
					{
					createdOrFoundSpecificationItem = existingSpecificationItem;

					if( ( ( !isOlderPrimarySpecification &&

					( !isSpanishCurrentLanguage ||
					// Typical for Spanish
					!isUserRelationWord ||
					existingSpecificationItem == null ||
					isPrimaryUserSpecification ) ) ||

					( !isOlderUserSpecification &&

					( !isPossessiveUserSpecification ||
					!isUserRelationWord ||

					// Typical for Spanish
					// Spanish test file: "Complejo (2)"
					( hasUserSpecificationCompoundCollection &&
					primarySpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == null ) ) ) ) &&

					( !isUserGeneralizationWord ||
					userSpecificationItem.hasRelationContext() ||

					( hasPrimarySpecificationRelationContext &&
					existingSpecificationItem != null &&
					!existingSpecificationItem.hasCurrentCreationSentenceNr() ) ||

					// Not Spanish
					( !isSpanishCurrentLanguage &&

					( hasPrimarySpecificationRelationContext ||
					existingSpecificationItem == null ||
					existingSpecificationItem.hasCurrentCreationSentenceNr() ) ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&
					!hasPrimarySpecificationRelationContext ) ) )
						{
						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the included challenge document,
						// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, partOfDefinitionSpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
						}

					if( createdOrFoundSpecificationItem != null &&
					!isPossessivePrimarySpecification &&

					( ( isPossessiveUserSpecification &&
					userSpecificationWordItem == specificationWordItem ) ||

					( !isPossessiveUserSpecification &&
					!isSpanishCurrentLanguage ) ) )
						{
						selectedPrimarySpecificationItem = ( isPossessiveUserSpecification ? userSpecificationItem :
																							userGeneralizationWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) );

						if( selectedPrimarySpecificationItem != null &&
						// Justification doesn't exist yet in specification
						!createdOrFoundSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
						// Justification has at least the same assumption level
						createdOrFoundSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, null, partOfDefinitionSpecificationItem, null ) &&
						// Draw a 'part of' generalization conclusion
						// See Block 1 and Block 2 of the included challenge document,
						// or https://www.mafait.org/block_1 and https://www.mafait.org/block_2
						adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationArchivedAssignment, isPrimarySpecificationArchivedAssignment, false, false, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, partOfDefinitionSpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification substitution 'part of' specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawPossessiveReversibleConclusions( boolean isActiveUserAssignment, SpecificationItem givenSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasNonCompoundSpecificationCollection;
		boolean hasNonCompoundUserSpecificationCollection;
		boolean hasUserSpecificationAssumptionLevel;
		boolean hasUserSpecificationRelationContext;
		boolean isArchivedAssignment;
		boolean isAssignment;
		boolean isExclusiveGeneralizationAssignment;
		boolean isExclusiveSpecification;
		boolean isExistingUserSpecification;
		boolean isOlderAlternativeSpecification;
		boolean isOlderExistingSpecification;
		boolean isOlderSpecification;
		boolean isOlderUserSpecification;
		boolean isPossessiveUserSpecification;
		boolean isSelfGeneratedAssumption;
		boolean isSelfGeneratedSpecification;
		boolean isSpanishCurrentLanguage;
		boolean isSpanishSpecificationFemaleWord = false;
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUniqueUserRelation;
		boolean isUserAssignment;
		boolean isUserAssignmentSpecification;
		boolean isUserGeneralizationWord;
		boolean isUserSpecification;
		short relationWordTypeNr;
		short specificationWordTypeNr;
		int createdRelationContextNr;
		int relationContextNr;
		int spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int specificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem foundJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem additionalSpecificationItem;
		SpecificationItem additionalUserSpecificationItem;
		SpecificationItem alternativeSpecificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem collectedSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentExistingSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem firstUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem oppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem partOfDefinitionSpecificationItem;
		SpecificationItem partOfSpecificationItem;
		SpecificationItem possessiveDefinitionSpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem spanishMasculineSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem additionalGeneralizationWordItem;
		WordItem commonWordItem;
		WordItem currentContextWordItem;
		WordItem oppositePossessiveDefinitionSpecificationWordItem = null;
		WordItem currentRelationContextWordItem;
		WordItem possessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveNounWordItem;
		WordItem spanishMasculineSpecificationWordItem = null;
		WordItem specificationWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;
		JustificationResultType justificationResult;

		if( givenSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( relationContextNr = givenSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = adminItem_.firstContextWordItem( relationContextNr ) ) != null &&
		( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
		( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			hasNonCompoundUserSpecificationCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			hasUserSpecificationAssumptionLevel = userSpecificationItem.hasAssumptionLevel();
			hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			isUserAssignmentSpecification = userSpecificationItem.isAssignment();

			hasNonCompoundSpecificationCollection = givenSpecificationItem.hasNonCompoundSpecificationCollection();
			isArchivedAssignment = givenSpecificationItem.isArchivedAssignment();
			isExclusiveSpecification = givenSpecificationItem.isExclusiveSpecification();
			isOlderSpecification = givenSpecificationItem.isOlderItem();
			isSelfGeneratedAssumption = givenSpecificationItem.isSelfGeneratedAssumption();
			isSelfGeneratedSpecification = givenSpecificationItem.isSelfGeneratedSpecification();
			isUserAssignment = givenSpecificationItem.isUserAssignment();
			isUserSpecification = givenSpecificationItem.isUserSpecification();
			relationWordTypeNr = givenSpecificationItem.relationWordTypeNr();
			specificationWordTypeNr = givenSpecificationItem.specificationWordTypeNr();
			specificationCollectionNr = givenSpecificationItem.specificationCollectionNr();

			isExclusiveGeneralizationAssignment = ( isUserAssignment &&
													givenSpecificationItem.isExclusiveGeneralization() );

			if( ( specificationWordItem = givenSpecificationItem.specificationWordItem() ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The secondary specification item has no specification word" );

			// Compound generalization word
			if( ( possessiveNounWordItem = specificationWordItem.compoundGeneralizationWordItem( specificationCollectionNr ) ) != null )
				{
				// Find possessive specifications
				if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"" + possessiveNounWordItem.anyWordTypeString() + "\"" );

				if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != null )
					oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem.specificationWordItem();

				if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != null )
					possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem();
				}

			// Typical for Spanish
			if( isSpanishCurrentLanguage )
				{
				isSpecificationWordSpanishAmbiguous = givenSpecificationItem.isSpecificationWordSpanishAmbiguous();
				isSpanishSpecificationFemaleWord = specificationWordItem.isFeminineWord();

				if( isSpanishSpecificationFemaleWord )
					spanishMasculineSpecificationWordItem = specificationWordItem.masculineCollectionWordItem();
				}

			// Do for all context words with secondary relation context
			do	{
				isOlderExistingSpecification = false;
				isExistingUserSpecification = false;

				if( ( existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, true, specificationWordItem, generalizationWordItem ) ) != null )
					{
					isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
					isExistingUserSpecification = existingSpecificationItem.isUserSpecification();
					}

				if( isExclusiveSpecification )
					{
					// Specification doesn't exist yet
					if( existingSpecificationItem == null &&
					// Draw possessive reversible conclusion
					adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, currentContextWordItem, specificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with current context word \"" + currentContextWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					firstUserSpecificationItem = currentContextWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

					isOlderUserSpecification = ( existingSpecificationItem == null &&
												firstUserSpecificationItem != null );

					alternativeSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, specificationWordItem, currentContextWordItem );

					isOlderAlternativeSpecification = ( alternativeSpecificationItem != null &&
														alternativeSpecificationItem.isOlderItem() );

					if( isOlderUserSpecification ||
					firstUserSpecificationItem == null ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&
					givenSpecificationItem != alternativeSpecificationItem &&

					( !isOlderExistingSpecification ||
					!isOlderAlternativeSpecification ) ) )
						{
						currentExistingSpecificationItem = currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, true, true, Constants.NO_QUESTION_PARAMETER, specificationWordItem.nonCompoundCollectionNr(), specificationWordItem );

						if( existingSpecificationItem == null ||

						( givenSpecificationItem == alternativeSpecificationItem &&
						// Justification doesn't exist yet in specification
						!existingSpecificationItem.hasPrimarySpecificationJustification( givenSpecificationItem ) &&

						( !isSpanishCurrentLanguage ||
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null, null ) ) &&

						( currentExistingSpecificationItem == null ||
						// Justification has at least the same assumption level
						currentExistingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null, null ) ||

						( isOlderExistingSpecification &&
						existingSpecificationItem.hasRelationContextCurrentlyBeenUpdated() ) ) ) )
							{
							if( !isSpecificationWordSpanishAmbiguous ||

							// Typical for Spanish
							( alternativeSpecificationItem == null &&
							firstUserSpecificationItem != null ) ||

							// Typical for Spanish
							( ( !isUserAssignment &&

							( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							isUserSpecification ||
							currentExistingSpecificationItem != null ) ) ||

							( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
							isUserSpecification &&

							( currentExistingSpecificationItem == null ||
							!currentExistingSpecificationItem.hasCurrentCreationSentenceNr() ) ) ) )
								{
								isAssignment = ( isArchivedAssignment ||
												isExclusiveGeneralizationAssignment );

								isUniqueUserRelation = ( isArchivedAssignment &&
														isUserAssignment &&
														adminItem_.isUniqueUserRelation() );

								if( isSpanishCurrentLanguage )
									spanishAmbiguousSpecificationCollectionNr = ( !isSpanishSpecificationFemaleWord &&
																				alternativeSpecificationItem == null &&
																				firstUserSpecificationItem == null ? specificationCollectionNr :
																				existingSpecificationItem != null ? existingSpecificationItem.specificationCollectionNr() :
																													Constants.NO_COLLECTION_NR );

								// Draw reversible conclusion
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, true, isUniqueUserRelation, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, currentContextWordItem, specificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion about word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
								foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
								createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;

								givenSpecificationItem = givenSpecificationItem.updatedSpecificationItem();

								if( !isOlderUserSpecification &&
								!isSpanishCurrentLanguage &&
								isSelfGeneratedSpecification &&

								( hasNonCompoundUserSpecificationCollection ||
								isActiveUserAssignment ||
								isOlderSpecification ||
								!isUserGeneralizationWord ||
								foundSpecificationItem == null ||
								!foundSpecificationItem.isOlderItem() ||
								givenSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != null ) &&

								createdOrFoundSpecificationItem != null &&
								createdOrFoundSpecificationItem.hasNonCompoundSpecificationCollection() &&
								// Justification doesn't exist yet in specification
								!givenSpecificationItem.hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
								// Justification has at least the same assumption level
								givenSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null, null ) )
									{
									// Draw reversible conclusion
									if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, currentContextWordItem ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

									if( isActiveUserAssignment &&
									isUserGeneralizationWord &&
									!isOlderSpecification &&
									createdSpecificationItem == null &&
									!createdOrFoundSpecificationItem.isOlderItem() &&
									( additionalUserSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
										{
										// Test file: "John - Anna (before family definition)"
										// Justification doesn't exist yet in specification
										if( !givenSpecificationItem.hasPrimarySpecificationJustification( additionalUserSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, null, null, null ) &&
										// Add user reversible justification to current secondary specification
										givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalUserSpecificationItem, null, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a user reversible justification to the current secondary specification" );

										// Test file: "John - Anna (before family definition)"
										if( ( additionalGeneralizationWordItem = adminItem_.firstContextWordItem( additionalUserSpecificationItem.relationContextNr() ) ) != null &&
										( additionalSpecificationItem = additionalGeneralizationWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
										// Justification doesn't exist yet in specification
										!givenSpecificationItem.hasPrimarySpecificationJustification( additionalSpecificationItem ) &&
										// Justification has at least the same assumption level
										givenSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, null, null, null ) &&
										// Add assumption reversible justification to current secondary specification
										givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, additionalSpecificationItem, null, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add an assumption reversible justification to the current secondary specification" );
										}
									}

								if( createdSpecificationItem != null )
									{
									// Created, not found
									if( foundSpecificationItem == null )
										{
										if( isUserGeneralizationWord )
											{
											if( specificationWordItem == userSpecificationWordItem )
												{
												if( possessiveDefinitionSpecificationWordItem != null &&
												( assumptionSpecificationItem = currentContextWordItem.firstSpecificationItem( false, true, false, possessiveDefinitionSpecificationWordItem ) ) != null &&
												( secondarySpecificationItem = specificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != null &&
												// Justification doesn't exist yet in specification
												!assumptionSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( createdSpecificationItem, secondarySpecificationItem ) &&
												// Justification has at least the same assumption level
												assumptionSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, secondarySpecificationItem, null ) &&
												// Add specification substitution justification to additional specification
												assumptionSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );
												}
											else
												{
												if( ( primarySpecificationItem = currentContextWordItem.firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != null &&
												// Skip archived user assignment
												primarySpecificationItem.hasRelationContext() &&
												( secondarySpecificationItem = userSpecificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
												// Justification doesn't exist yet in specification
												!createdSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
												// Justification has at least the same assumption level
												createdSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem, null ) &&
												// Add specification substitution justification to created specification
												createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the created specification" );
												}
											}

										if( isSpanishCurrentLanguage )
											{
											// Typical for Spanish
											if( ( obsoleteSpecificationItem = currentContextWordItem.noRelationContextSpecificationItem( true, true, ( isSpanishSpecificationFemaleWord ? spanishMasculineSpecificationWordItem : specificationWordItem ) ) ) == null )
												{
												if( isUserAssignmentSpecification )
													{
													// Spanish test file: "Complejo (19 - extraño)"
													if( ( secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, userSpecificationWordItem, currentContextWordItem ) ) != null &&
													( commonWordItem = userSpecificationWordItem.commonWordItem( createdSpecificationItem.specificationCollectionNr() ) ) != null &&
													( primarySpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != null &&
													// Justification doesn't exist yet in specification
													!createdSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
													// Justification has at least the same assumption level
													createdSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, secondarySpecificationItem, null ) &&
													// Add opposite possessive conclusion justification to created specification
													createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the created specification" );
													}
												else
													{
													// Justification doesn't exist yet in specification
													if( ( secondarySpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
													!createdSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, secondarySpecificationItem ) &&
													// Justification has at least the same assumption level
													createdSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, null, secondarySpecificationItem, null ) &&
													// Add exclusive user specification substitution assumption justification to created specification
													createdSpecificationItem.addJustificationToSpecification( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, userSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add an exclusive user specification substitution assumption justification to the created specification" );
													}
												}
											else
												{
												// Write an assumption without relation word
												if( isUserSpecification &&
												currentContextWordItem.writeUpdatedSpecification( true, false, false, false, true, false, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to write an assumption without relation word of word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

												// Replace an assumption without relation word
												if( currentContextWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
												}
											}
										}
									else	// Created and found
										{
										if( !isOlderUserSpecification )
											{
											if( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
											!isOlderSpecification &&
											( foundJustificationItem = givenSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) != null &&
											( primarySpecificationItem = foundJustificationItem.primarySpecificationItem() ) != null &&
											( secondarySpecificationItem = foundJustificationItem.secondarySpecificationItem() ) != null &&
											// Justification doesn't exist yet in specification
											!createdSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											createdSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, secondarySpecificationItem, null ) )
												{
												// Add opposite possessive conclusion justification to created specification
												if( createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, primarySpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the created specification" );

												// "Test file: "Complex (11)
												if( ( createdRelationContextNr = createdSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
												( currentRelationContextWordItem = adminItem_.firstContextWordItem( createdRelationContextNr ) ) != null )
													{
													// Do for all context words with the created relation context
													do	{
														// Remove obsolete assumption justifications from 'part of' specifications in relation context word
														if( currentRelationContextWordItem.removeObsoleteAssumptionJustificationsFromPartSpecificationsInWord() != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to remove obsolete assumption justifications from 'part of' specifications in relation context word \"" + currentRelationContextWordItem.anyWordTypeString() + "\"" );
														}
													while( ( currentRelationContextWordItem = currentRelationContextWordItem._nextContextWordItem( createdRelationContextNr ) ) != null );
													}
												}

											if( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
											isSelfGeneratedSpecification ) &&

											( commonWordItem = specificationWordItem.commonWordItem( specificationCollectionNr ) ) != null &&
											( partOfSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, commonWordItem ) ) != null &&
											partOfSpecificationItem.isSelfGeneratedSpecification() &&
											// Justification doesn't exist yet in part-of specification
											!partOfSpecificationItem.hasPrimarySpecificationJustification( createdSpecificationItem ) &&
											( partOfDefinitionSpecificationItem = commonWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
											// Justification has at least the same assumption level
											partOfSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, partOfDefinitionSpecificationItem, null ) &&
											// Add part-of justification to part-of specification
											partOfSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, partOfDefinitionSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a part-of justification to a part-of specification" );
											}
										}

									if( isOlderUserSpecification &&
									!isExclusiveGeneralizationAssignment )
										{
										// Typical for Spanish
										// Spanish test file: "Complejo (19 - extraño)"
										if( isSpecificationWordSpanishAmbiguous )
											foundSpecificationItem = currentContextWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

										if( foundSpecificationItem != null )
											{
											// Test file: "Complex (19 - strange)"
											// Add reversible justification
											if( createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, " failed to add a reversible justification to the created specification" );

											// Update specification of justifications of involved words, except for current context word
											if( currentContextWordItem.updateSpecificationOfJustificationsOfInvolvedWords( false, foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in the justification of the involved words of word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

											if( possessiveNounWordItem == null &&
											// Common word
											( possessiveNounWordItem = specificationWordItem.commonWordItem( specificationCollectionNr ) ) != null )
												{
												// Find possessive specifications
												if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in word \"" + possessiveNounWordItem.anyWordTypeString() + "\"" );

												if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != null )
													oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem.specificationWordItem();

												if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != null )
													possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem();
												}

											if( possessiveDefinitionSpecificationItem != null &&
											oppositePossessiveDefinitionSpecificationWordItem != null &&
											// Make an opposite possessive specification assumption
											adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationItem, null, createdSpecificationItem, null, currentContextWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"" + currentContextWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								else
									{
									if( isOlderSpecification &&
									isUserAssignmentSpecification &&
									existingSpecificationItem == null &&
									foundSpecificationItem != null &&
									// Common word
									( possessiveNounWordItem = specificationWordItem.commonWordItem( specificationCollectionNr ) ) != null )
										{
										// Find possessive specifications
										if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"" + possessiveNounWordItem.anyWordTypeString() + "\"" );

										if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != null &&
										( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem.specificationWordItem() ) != null &&
										oppositePossessiveDefinitionSpecificationWordItem != specificationWordItem &&
										( secondarySpecificationItem = currentContextWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null )
											{
											// Recalculate assumption level of found specification
											if( foundSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );

											// Justification doesn't exist yet in specification
											if( !foundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( oppositePossessiveDefinitionSpecificationItem, secondarySpecificationItem ) &&
											// Justification has at least the same assumption level
											foundSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, null, secondarySpecificationItem, null ) &&
											// Add opposite possessive justification to found specification
											foundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive justification to the found specification" );
											}
										}
									}
								}
							}
						else
							{
							// Typical for Spanish
							if( isSpanishCurrentLanguage &&
							alternativeSpecificationItem != null &&
							givenSpecificationItem != alternativeSpecificationItem )
								{
								if( isOlderSpecification )
									{
									if( isSpecificationWordSpanishAmbiguous &&
									!hasNonCompoundSpecificationCollection &&
									// Justification doesn't exist yet in specification
									!alternativeSpecificationItem.hasPrimarySpecificationJustification( givenSpecificationItem ) &&
									// Add Spanish reversible justification to alternative specification
									alternativeSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish reversible justification to the alternative specification" );
									}
								else
									{
									if( !isOlderExistingSpecification &&
									spanishMasculineSpecificationWordItem != null &&
									( spanishMasculineSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, spanishMasculineSpecificationWordItem, generalizationWordItem ) ) != null &&
									// Justification exists in specification
									( obsoleteJustificationItem = spanishMasculineSpecificationItem.primarySpecificationJustificationItem( alternativeSpecificationItem ) ) != null )
										{
										// Create justification
										if( ( justificationResult = currentContextWordItem.createJustificationItem( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, givenSpecificationItem, null, null, null, null ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to create a justification item to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

										if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
											return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

										if( currentContextWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption justification by a conclusion justification in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							}
						}

					if( hasNonCompoundSpecificationCollection &&
					isSelfGeneratedSpecification &&
					!isSpanishCurrentLanguage &&
					alternativeSpecificationItem != null &&
					existingSpecificationItem == userSpecificationItem &&
					// Make an opposite possessive specification assumption
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, false, true, Constants.WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, relationWordTypeNr, ( isSelfGeneratedAssumption ? alternativeSpecificationItem : givenSpecificationItem ), null, userSpecificationItem, currentContextWordItem, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption during the drawing of a reversible conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					// Alternative specification is same as current secondary specification
					if( givenSpecificationItem == alternativeSpecificationItem )
						{
						if( isOlderExistingSpecification &&
						isSelfGeneratedSpecification &&
						existingSpecificationItem != null &&
						existingSpecificationItem.hasNonCompoundSpecificationCollection() )
							{
							if( isOlderSpecification &&
							firstUserSpecificationItem == null &&
							// Secondary specification word is same as user specification word
							specificationWordItem == userSpecificationWordItem &&
							// Justification doesn't exist yet in specification
							!givenSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
							( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, specificationWordItem, userGeneralizationWordItem ) ) != null )
								{
								// Add reversible user justification
								if( givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );

								givenSpecificationItem = givenSpecificationItem.updatedSpecificationItem();

								// Add context
								if( userGeneralizationWordItem.addContext( Constants.WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, givenSpecificationItem.relationContextNr(), Constants.NO_COLLECTION_NR, specificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + userGeneralizationWordItem.anyWordTypeString() + "\"" );

								if( ( collectedSpecificationItem = userGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, givenSpecificationItem.specificationCollectionNr(), null ) ) != null &&
								// Common word
								( possessiveNounWordItem = specificationWordItem.commonWordItem( specificationCollectionNr ) ) != null )
									{
									// Find possessive specifications
									if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"" + possessiveNounWordItem.anyWordTypeString() + "\"" );

									if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != null &&
									!collectedSpecificationItem.isOlderItem() &&
									// Justification doesn't exist yet in specification
									!givenSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( oppositePossessiveDefinitionSpecificationItem, collectedSpecificationItem ) &&
									// Add collected opposite possessive justification
									givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, oppositePossessiveDefinitionSpecificationItem, null, collectedSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a collected opposite possessive justification to the current secondary specification" );

									if( ( possessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem ) != null &&
									// Justification doesn't exist yet in specification
									!collectedSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( possessiveDefinitionSpecificationItem, conclusionSpecificationItem ) &&
									// Add opposite possessive conclusion justification to collected specification
									collectedSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, possessiveDefinitionSpecificationItem, null, conclusionSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the collected specification" );
									}
								}
							}
						}
					else	// Alternative is different from current secondary
						{
						if( hasNonCompoundSpecificationCollection )
							{
							if( alternativeSpecificationItem != null &&
							// Secondary specification word is same as user specification word
							specificationWordItem == userSpecificationWordItem &&
							!alternativeSpecificationItem.isReplacedItem() )
								{
								if( isSpanishCurrentLanguage )
									{
									// Typical for Spanish
									// Replace assumption by conclusion
									if( generalizationWordItem.replaceOrDeleteSpecification( givenSpecificationItem, alternativeSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace a Spanish assumption specification by a conclusion specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									if( !isOlderExistingSpecification &&
									alternativeSpecificationItem.isSelfGeneratedAssumption() &&
									// Common word
									( possessiveNounWordItem = specificationWordItem.commonWordItem( specificationCollectionNr ) ) != null )
										{
										// Find possessive specifications
										if( ( findSpecificationResult = findPossessiveSpecifications( possessiveNounWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in possessive noun word \"" + possessiveNounWordItem.anyWordTypeString() + "\"" );

										if( ( oppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem ) != null &&
										( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem.specificationWordItem() ) != null &&
										( foundSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null &&
										foundSpecificationItem.hasSecondarySpecificationJustification( alternativeSpecificationItem ) &&
										// Replace assumption justification by conclusion justification of involved word
										currentContextWordItem.updateSpecificationOfJustificationsInWord( alternativeSpecificationItem, givenSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in the justification items in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
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
							generalizationWordItem.replaceOrDeleteSpecification( ( isSelfGeneratedAssumption ? givenSpecificationItem : alternativeSpecificationItem ), ( isSelfGeneratedAssumption ? alternativeSpecificationItem : givenSpecificationItem ) ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption specification by a conclusion specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}

					// Typical for Spanish
					if( isSpanishCurrentLanguage )
						{
						if( isSpanishSpecificationFemaleWord &&
						spanishMasculineSpecificationWordItem != null )
							{
							spanishMasculineSpecificationItem = currentContextWordItem.firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem );

							if( existingSpecificationItem == null ||

							( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
							hasUserSpecificationRelationContext ) &&

							!givenSpecificationItem.hasCurrentCreationSentenceNr() &&
							givenSpecificationItem.isSelfGeneratedSpecification() ) )
								{
								if( spanishMasculineSpecificationItem == null ||
								existingSpecificationItem == userSpecificationItem ||

								( !isPossessiveUserSpecification &&
								isUserGeneralizationWord ) ||

								( !spanishMasculineSpecificationItem.isOlderItem() &&
								spanishMasculineSpecificationItem.isHiddenSpanishSpecification() &&
								// Justification doesn't exist yet in specification
								!spanishMasculineSpecificationItem.hasPrimarySpecificationJustification( givenSpecificationItem ) &&
								// Justification has at least the same assumption level
								spanishMasculineSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, givenSpecificationItem, null, null, null ) ) )
									{
									givenSpecificationItem = givenSpecificationItem.updatedSpecificationItem();

									// Draw Spanish reversible conclusion
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, spanishMasculineSpecificationWordItem.nonCompoundCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"" + currentContextWordItem.anyWordTypeString() + "\" and with masculine specification word \"" + spanishMasculineSpecificationWordItem.anyWordTypeString() + "\"" );

									if( !isUserGeneralizationWord &&
									existingSpecificationItem == userSpecificationItem &&
									( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
									// Justification doesn't exist yet in question
									!createdOrFoundSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
									// Add opposite possessive justification
									createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the created or found specification" );
									}
								}
							else
								{
								if( hasGeneralizationWordCurrentlyConfirmedSpecification &&
								isOlderExistingSpecification &&
								!hasUserSpecificationAssumptionLevel &&
								specificationWordItem == userSpecificationWordItem &&
								spanishMasculineSpecificationItem != null &&
								spanishMasculineSpecificationItem.isHiddenSpanishSpecification() &&
								!spanishMasculineSpecificationItem.hasCurrentCreationSentenceNr() &&
								existingSpecificationItem.isSelfGeneratedAssumption() &&
								// Draw Spanish reversible conclusion (hidden)
								adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, currentContextWordItem, spanishMasculineSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a hidden Spanish reversible conclusion about word \"" + currentContextWordItem.anyWordTypeString() + "\" and with masculine specification word \"" + spanishMasculineSpecificationWordItem.anyWordTypeString() + "\"" );
								}
							}
						else
							{
							if( existingSpecificationItem != null &&
							currentContextWordItem.isUserGeneralizationWord &&
							existingSpecificationItem.hasCompoundSpecificationCollection() &&
							givenSpecificationItem.isSelfGeneratedConclusion() &&
							// Add Spanish non-compound reversible conclusion
							adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, specificationWordItem.nonCompoundCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, givenSpecificationItem, null, null, null, currentContextWordItem, specificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish non-compound reversible conclusion about word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}
						}

					if( isExistingUserSpecification &&
					isOlderExistingSpecification &&
					!isOlderSpecification &&
					isSelfGeneratedAssumption &&
					// Justification doesn't exist yet in specification
					!givenSpecificationItem.hasPrimarySpecificationJustification( existingSpecificationItem ) &&
					// Add reversible user justification
					givenSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, null, null ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the current secondary specification" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( relationContextNr ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawReversibleConclusions( boolean isAssignment, boolean isArchivedAssignment, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasCurrentSpecificationCurrentCreationSentenceNr;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation;
		boolean hasNonCompoundCurrentSpecificationCollection;
		boolean hasUserGeneralizationWordCurrentlyConfirmedSpecification = false;
		boolean hasUserSpecificationCompoundCollection;
		boolean hasUserSpecificationRelationContext = false;
		boolean isActiveUserAssignment = false;
		boolean isArchivedUserAssignment = false;
		boolean isCurrentSpecificationSelfGeneratedAssumption;
		boolean isCurrentSpecificationSelfGeneratedSpecification;
		boolean isCurrentSpecificationWordSpanishAmbiguous = false;
		boolean isOlderCurrentSpecification;
		boolean isPossessiveUserSpecification = false;
		boolean isSpanishCurrentLanguage;
		boolean isUserGeneralizationWord;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		short generalizationWordTypeNr;
		short specificationWordTypeNr;
		short relationWordTypeNr;
		int currentRelationContextNr;
		int foundRelationContextNr;
		int selectedRelationContextNr;
		int specificationCollectionNr;
		int userSpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem currentContextWordItem;
		WordItem currentSpecificationWordItem;
		WordItem firstContextWordItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) ) != null &&
		( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null &&
		( firstContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			specificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();

			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();

			hasUserGeneralizationWordCurrentlyConfirmedSpecification = userGeneralizationWordItem.hasCurrentlyConfirmedSpecification();
			hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
			hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
			isActiveUserAssignment = userSpecificationItem.isActiveAssignment();
			isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			if( isPossessiveUserSpecification )
				{
				if( isUserGeneralizationWord &&
				// Typical for Chinese: Skip possessive inactive assignment
				!userSpecificationItem.isInactiveAssignment() &&
				// Draw user reversible conclusion
				drawUserReversibleConclusions( currentSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw a user reversible conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			else	// Non-possessive user specification
				{
				if( !isUserGeneralizationWord &&
				hasUserSpecificationRelationContext &&
				( existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, userSpecificationWordItem, userGeneralizationWordItem ) ) != null &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&

				// Older specification without specification collection
				( !existingSpecificationItem.hasSpecificationCollection() ||
				existingSpecificationItem.isFirstJustificationReversibleAssumption() ||
				// Typical for Spanish
				// Spanish test file: "Complejo (13)"
				existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null ) )
					{
					// Draw reversible user conclusion
					if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible user conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );

					// Cleanup unreferenced justifications
					if( isActiveUserAssignment &&
					existingSpecificationItem.hasCurrentCreationSentenceNr() &&
					generalizationWordItem.cleanupUnreferencedJustifications() != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to clean up unreferenced justifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}

			// Do for all selected specifications
			do	{
				hasCurrentSpecificationCurrentCreationSentenceNr = currentSpecificationItem.hasCurrentCreationSentenceNr();
				hasNonCompoundCurrentSpecificationCollection = currentSpecificationItem.hasNonCompoundSpecificationCollection();
				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem.isSpecificationWordSpanishAmbiguous();
				isOlderCurrentSpecification = currentSpecificationItem.isOlderItem();
				isCurrentSpecificationSelfGeneratedAssumption = currentSpecificationItem.isSelfGeneratedAssumption();
				isCurrentSpecificationSelfGeneratedSpecification = currentSpecificationItem.isSelfGeneratedSpecification();
				generalizationWordTypeNr = currentSpecificationItem.generalizationWordTypeNr();
				specificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();
				relationWordTypeNr = currentSpecificationItem.relationWordTypeNr();
				currentRelationContextNr = currentSpecificationItem.relationContextNr();
				specificationCollectionNr = currentSpecificationItem.specificationCollectionNr();

				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The secondary specification item has no specification word" );

				if( currentSpecificationItem.isPossessive() )
					{
					if( isCurrentSpecificationSelfGeneratedSpecification )
						{
						if( isUserGeneralizationWord ||
						specificationCollectionNr == Constants.NO_COLLECTION_NR )
							{
							if( !isSpanishCurrentLanguage ||
							!isCurrentSpecificationWordSpanishAmbiguous )
								{
								currentContextWordItem = firstContextWordItem;

								// Do for all context words
								do	{
									if( currentContextWordItem != generalizationWordItem &&
									( existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, currentSpecificationWordItem, generalizationWordItem ) ) != null )
										{
										// Found user specification, force self-generated assumption
										if( existingSpecificationItem.isUserSpecification() &&
										( tempSpecificationItem = currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, true, currentSpecificationWordItem, generalizationWordItem ) ) != null )
											existingSpecificationItem = tempSpecificationItem;

										if( hasGeneralizationWordCurrentlyConfirmedSpecificationAndAtLeastOneRelation )
											{
											if( currentContextWordItem.isUserRelationWord &&
											( obsoleteJustificationItem = currentSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentContextWordItem ) ) != null )
												{
												// Create justification
												if( ( justificationResult = generalizationWordItem.createJustificationItem( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, userSpecificationItem, null, null, null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to create a reversible assumption justification item to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't find or create a reversible assumption justification" );

												if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete reversible assumption justification of the current secondary specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											{
											if( existingSpecificationItem.hasCurrentCreationSentenceNr() &&
											existingSpecificationItem.hasNonCompoundSpecificationCollection() &&

											( !hasCurrentSpecificationCurrentCreationSentenceNr ||
											!isOlderCurrentSpecification ) &&

											// Relation context exists in word
											currentContextWordItem.hasContextInWord( currentRelationContextNr ) &&
											// Justification doesn't exist yet in specification
											!existingSpecificationItem.hasPrimarySpecificationJustification( currentSpecificationItem ) &&

											// Justification has at least the same assumption level
											( existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentSpecificationItem, null, null, null ) ||
											// Force assumption to be less certain
											// Test files: "Complex (18)", "Complex (19 - mixed)" and "Complex (19 - strange)"
											existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != null ) &&

											// Add reversible justification
											existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, currentSpecificationItem, null, null ) != Constants.RESULT_OK )
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
							if( isSpanishCurrentLanguage &&
							hasNonCompoundCurrentSpecificationCollection )
								{
								if( isOlderCurrentSpecification &&
								isActiveUserAssignment &&
								// Justification doesn't exist yet in specification
								!currentSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
								// Draw Spanish non-compound reversible conclusion
								adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible specification from word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

								if( !isPossessiveUserSpecification &&
								isUserSpecificationWordSpanishAmbiguous &&
								// Specification doesn't exist yet with user specification collection
								generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, userSpecificationWordItem ) == null )
									{
									// Draw Spanish reversible conclusion
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, isArchivedUserAssignment, false, false, false, false, false, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, userSpecificationWordItem, userGeneralizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish reversible conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );

									if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
									// Specification doesn't exist yet with without specification collection
									( obsoleteSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, userSpecificationWordItem ) ) != null &&
									// Replace specification without specification collection with specification with specification
									// Spanish test file: "Juan - Ana (antes de la definición de la familia)"
									generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace a specification without specification collection with a specification with specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
								currentContextWordItem.isUserRelationWord &&
								currentContextWordItem.hasNegativeSpecification() &&
								// Draw a specification substitution conclusion or ask a question
								drawSpecificationSubstitutionConclusionOrAskQuestion( false, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentContextWordItem, currentSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
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
							!currentSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
							// Add Spanish reversible user justification
							currentSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish reversible user justification to the current secondary specification" );
							}
						else
							{
							if( isPossessiveUserSpecification &&
							isCurrentSpecificationSelfGeneratedAssumption )
								{
								if( ( hasUserGeneralizationWordCurrentlyConfirmedSpecification ||

								( ( existingSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, currentSpecificationWordItem, null ) ) != null &&
								// Justification doesn't exist yet in specification
								!existingSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) ) &&

								// Conclusion doesn't exist yet
								generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, userGeneralizationWordItem ) == null )
									{
									selectedRelationContextNr = Constants.NO_CONTEXT_NR;
									conclusionSpecificationItem = null;

									if( hasNonCompoundCurrentSpecificationCollection )
										{
										if( hasUserGeneralizationWordCurrentlyConfirmedSpecification )
											{
											if( ( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, currentSpecificationWordItem, null ) ) == null )
												{
												if( ( foundRelationContextNr = userGeneralizationWordItem.contextNr( currentSpecificationWordItem ) ) > Constants.NO_CONTEXT_NR &&
												adminItem_.nContextWords( foundRelationContextNr ) > 1 )
													{
													// Get relation context
													if( ( contextResult = adminItem_.getRelationContext( false, false, false, false, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem, null ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to get a relation context" );

													selectedRelationContextNr = contextResult.contextNr;
													}
												}
											else
												selectedRelationContextNr = conclusionSpecificationItem.relationContextNr();
											}
										else
											selectedRelationContextNr = userGeneralizationWordItem.contextNr( currentSpecificationWordItem );
										}

									// Draw reversible user conclusion
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, relationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedRelationContextNr, userSpecificationItem, null, null, null, generalizationWordItem, currentSpecificationWordItem, userGeneralizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

									if( conclusionSpecificationItem == null )
										{
										if( createAndAssignResult.foundSpecificationItem != null &&
										( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
										// Add reversible justification to current secondary specification
										currentSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the current secondary specification" );
										}
									else
										{
										conclusionSpecificationItem = conclusionSpecificationItem.updatedSpecificationItem();

										// Confirmed all relation words
										if( conclusionSpecificationItem.nRelationContextWords() == currentSpecificationItem.nRelationContextWords() &&
										// Replace assumption by conclusion
										generalizationWordItem.replaceOrDeleteSpecification( currentSpecificationItem, conclusionSpecificationItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace an assumption without relation word" );
										}
									}
								}
							else
								{
								// Test file: "Complex (17)"
								if( isOlderCurrentSpecification &&
								( obsoleteSpecificationItem = generalizationWordItem.noRelationContextSpecificationItem( false, true, currentSpecificationWordItem ) ) != null &&
								// Replace obsolete assumption specification by conclusion
								generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, currentSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete assumption specification by a conclusion in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						}

					currentContextWordItem = firstContextWordItem;
					currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

					if( currentSpecificationItem.hasRelationContext() &&
					// Draw possessive reversible conclusions
					drawPossessiveReversibleConclusions( isActiveUserAssignment, currentSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw possessive reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem.isReplacedOrDeletedItem() ?
												// First specification
												generalizationWordItem.firstNonQuestionSpecificationItem( true, isAssignment, isArchivedAssignment ) :
												// Next specification
												currentSpecificationItem.nextNonQuestionNonRelationSpecificationItem() ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte drawUserReversibleConclusions( SpecificationItem currentSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasNonCompoundUserSpecificationCollection = false;
		boolean isArchivedAssignment;
		boolean isAssignment;
		boolean isSpanishCurrentLanguage;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		int spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int userSpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem assignmentSpecificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem assumptionSpecificationWordItem;
		WordItem commonWordItem;
		WordItem currentContextWordItem;
		WordItem spanishFeminineSpecificationWordItem;
		WordItem spanishMasculineSpecificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( currentSecondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first secondary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null &&
		( userSpecificationItem = adminItem_.userSpecificationItem() ) != null &&
		( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();

			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();

			hasNonCompoundUserSpecificationCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			// Do for all context words
			do	{
				if( currentContextWordItem.isUserRelationWord &&

				// Specification doesn't exist yet
				( ( existingSpecificationItem = currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, false, userSpecificationWordItem, generalizationWordItem ) ) == null ||

				( existingSpecificationItem.isSelfGeneratedSpecification() &&
				// Justification doesn't exist yet in specification
				!existingSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) ) )
					{
					conclusionSpecificationItem = currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, userSpecificationWordItem, null );

					if( isSpanishCurrentLanguage ||

					( !hasNonCompoundUserSpecificationCollection &&
					existingSpecificationItem == null ) ||

					( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
					// Self-generated conclusion with different relation word
					conclusionSpecificationItem != null ) ||

					( existingSpecificationItem != null &&
					// Conclusion doesn't exist yet
					conclusionSpecificationItem == null ) ||

					( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
					generalizationWordItem.firstActiveQuestionSpecificationItem() != null ) &&

					// Assumption doesn't exist yet
					currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, userSpecificationWordItem, null ) == null ) )
						{
						isAssignment = false;
						isArchivedAssignment = false;
						assignmentSpecificationItem = null;
						spanishAmbiguousSpecificationCollectionNr = Constants.NO_COLLECTION_NR;

						// Typical for Spanish
						if( isSpanishCurrentLanguage &&
						hasNonCompoundUserSpecificationCollection &&

						( !currentContextWordItem.hasNegativeSpecification() ||
						currentContextWordItem.hasFeminineProperNounEnding() ) )
							spanishAmbiguousSpecificationCollectionNr = userSpecificationCollectionNr;

						// Similar specification already exists as assignment
						// So, make this conclusion an assignment too
						if( existingSpecificationItem == null &&
						( assignmentSpecificationItem = currentContextWordItem.firstAssignmentItem( false, false, Constants.NO_CONTEXT_NR, userSpecificationWordItem ) ) != null )
							{
							isAssignment = true;
							isArchivedAssignment = assignmentSpecificationItem.isArchivedAssignment();
							}

						// Draw reversible user conclusion
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, spanishAmbiguousSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, currentContextWordItem, userSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible user conclusion about word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + userSpecificationWordItem.anyWordTypeString() + "\"" );

						// Typical for Spanish
						if( isUserSpecificationWordSpanishAmbiguous )
							{
							if( currentContextWordItem.hasFeminineProperNounEnding() &&
							( spanishFeminineSpecificationWordItem = userSpecificationWordItem.feminineCollectionWordItem() ) != null &&
							( definitionSpecificationItem = userSpecificationWordItem.firstSpecificationItem( false, false, false, spanishFeminineSpecificationWordItem ) ) != null &&
							currentContextWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, definitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, spanishFeminineSpecificationWordItem ) == null &&
							// Draw Spanish feminine exclusive specification substitution assumption
							adminItem_.addSelfGeneratedSpecification( true, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, currentContextWordItem, spanishFeminineSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish exclusive specification substitution assumption about word \"" + currentContextWordItem.anyWordTypeString() + "\" with specification word \"" + spanishFeminineSpecificationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
								{
								if( existingSpecificationItem == null )
									{							// Assignment specification
									foundSpecificationItem = ( assignmentSpecificationItem != null ? assignmentSpecificationItem :
																// Found specification
																( createAndAssignResult.foundSpecificationItem != null ? createAndAssignResult.foundSpecificationItem :
																// User specification
																currentContextWordItem.firstUserSpecificationItem( false, false, createdSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, userSpecificationWordItem ) ) );

									if( foundSpecificationItem == null )
										{
										if( hasNonCompoundUserSpecificationCollection )
											{
											// Test file: "Complex (3)"
											// Draw only option left conclusion
											if( drawOnlyOptionLeftProperNounConclusion( createdSpecificationItem, currentContextWordItem, userSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only-option-left proper noun conclusion" );


											if( ( commonWordItem = userSpecificationWordItem.commonWordItem( userSpecificationCollectionNr ) ) != null &&
											( assumptionSpecificationWordItem = userSpecificationWordItem.collectionWordItem( userSpecificationCollectionNr ) ) != null &&
											( definitionSpecificationItem = commonWordItem.firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != null &&
											( assumptionSpecificationItem = currentContextWordItem.firstSpecificationItem( false, true, false, assumptionSpecificationWordItem ) ) != null &&
											// Justification doesn't exist yet in specification
											!assumptionSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( definitionSpecificationItem, createdSpecificationItem ) &&
											// Add opposite possessive assumption justification to assumption specification
											assumptionSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, definitionSpecificationItem, null, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive assumption justification to an assumption specification" );
											}
										}
									else
										{
										if( foundSpecificationItem.hasOnlyOneRelationWord() &&
										// Add reversible user justification to created specification
										createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the created specification" );
										}
									}
								else
									{
									if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
										{
										if( foundSpecificationItem.hasCompoundSpecificationCollection() )
											{
											// Replace an assumption specification by a conclusion specification in justifications of involved word
											if( currentContextWordItem.updateSpecificationOfJustificationsInWord( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to update the specifications in a justification in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

											// Add reversible user justification to created specification
											if( foundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, null ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible user justification to the created specification" );

											// Typical for Spanish
											// Spanish test file: "Complejo (15)"
											if( isSpanishCurrentLanguage &&
											( spanishMasculineSpecificationWordItem = userSpecificationWordItem.masculineCollectionWordItem() ) != null &&
											( assumptionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, true, false, spanishMasculineSpecificationWordItem ) ) != null &&
											!assumptionSpecificationItem.isHiddenSpanishSpecification() &&
											// Add Spanish reversible justification to assumption specification
											assumptionSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdSpecificationItem, null, null ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish reversible justification to the assumption specification" );
											}
										else
											{
											if( ( obsoleteJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, generalizationWordItem ) ) != null )
												{
												// Test file: "You have confirmed at least one relation word"
												// Create justification
												if( ( justificationResult = currentContextWordItem.createJustificationItem( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, userSpecificationItem, null, null, null, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to create a justification item to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );

												if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
													return adminItem_.startError( 1, moduleNameString_, "I couldn't create an additional justification" );

												// Replace negative assumption justification
												if( currentContextWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace a negative assumption justification in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									}
								}
							}
						}
					}
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private FindSpecificationResultType findPossessiveSpecifications( WordItem possessiveNounWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundPossessiveSpecificationWord = true;
		int possessiveSpecificationCollectionNr;
		SpecificationItem foundOppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem foundPossessiveDefinitionSpecificationItem = null;
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem previousPossessiveSpecificationItem = null;
		WordItem higherLevelPossessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;
		FindSpecificationResultType findSpecificationResult = new FindSpecificationResultType();

		if( possessiveNounWordItem == null )
			return adminItem_.startFindSpecificationResultError( 1, moduleNameString_, "The given possessive noun word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startFindSpecificationResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		// First try to find a possessive specification on a higher level
		if( ( possessiveDefinitionSpecificationItem = possessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
			{
			do	{
				if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null &&
				possessiveSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) != null )
					{
					if( higherLevelPossessiveDefinitionSpecificationWordItem == null )
						higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
					else
						hasFoundPossessiveSpecificationWord = false;
					}
				}
			while( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

			if( hasFoundPossessiveSpecificationWord &&
			( possessiveDefinitionSpecificationItem = possessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
				{
				if( higherLevelPossessiveDefinitionSpecificationWordItem == null )
					higherLevelPossessiveDefinitionSpecificationWordItem = specificationWordItem;

				// Try to find the opposite of the possessive specification
				do	{
					if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
						{
						if( previousPossessiveSpecificationItem != null )
							{
							possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem.specificationCollectionNr();

							if( higherLevelPossessiveDefinitionSpecificationWordItem.hasCollectionNr( possessiveSpecificationCollectionNr ) )
								foundOppositePossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem :
																																												possessiveDefinitionSpecificationItem );

							foundPossessiveDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem :
																																									previousPossessiveSpecificationItem );
							}

						previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
						}
					}
				while( foundPossessiveDefinitionSpecificationItem == null &&
				( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );
				}
			}

		findSpecificationResult.foundPossessiveDefinitionSpecificationItem = foundPossessiveDefinitionSpecificationItem;
		findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem = foundOppositePossessiveDefinitionSpecificationItem;

		return findSpecificationResult;
		}

	private byte findQuestionToBeAdjustedByCompoundCollection( WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem, SpecificationItem definitionSpecificationItem )
		{
		boolean hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		int definitionSpecificationCollectionNr;
		SpecificationItem adjustedQuestionCompoundSpecificationItem;
		SpecificationItem lastAdjustedQuestionCompoundSpecificationItem = null;
		SpecificationItem replacingSpecificationItem = null;
		WordItem commonWordItem;
		WordItem currentCollectionWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( definitionSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given definition specification item is undefined" );

		if( ( definitionSpecificationCollectionNr = definitionSpecificationItem.specificationCollectionNr() ) <= Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given definition specification item has no specification collection" );

		if( ( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
			{
			hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem.hasCurrentlyMoreSpecificSpecification();

			// Do for all collection words
			do	{
				if( ( commonWordItem = currentCollectionWordItem.commonWordItem( definitionSpecificationCollectionNr ) ) != null &&
				( adjustedQuestionCompoundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, currentCollectionWordItem ) ) != null )
					{
					if( commonWordItem == specificationWordItem )
						{
						// Skip updating question if the old justification has no relation word,
						// while the current user specification has a relation word.
						// Test file: "This information is more specific"
						if( !hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
						// Justification doesn't exist yet in specification
						!adjustedQuestionCompoundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) )
							{
							// Create adjusted question
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, adjustedQuestionCompoundSpecificationItem.isInactiveAssignment(), adjustedQuestionCompoundSpecificationItem.isArchivedAssignment(), false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, adjustedQuestionCompoundSpecificationItem.generalizationWordTypeNr(), adjustedQuestionCompoundSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, currentCollectionWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create an adjusted question about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );

							replacingSpecificationItem = createAndAssignResult.foundSpecificationItem;
							}
						}
					else
						{
						if( generalizationWordItem.replaceOrDeleteSpecification( adjustedQuestionCompoundSpecificationItem, ( replacingSpecificationItem == null || replacingSpecificationItem.isReplacedOrDeletedItem() ? null : replacingSpecificationItem ) ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a question part" );

						lastAdjustedQuestionCompoundSpecificationItem = adjustedQuestionCompoundSpecificationItem.updatedSpecificationItem();
						}
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		if( lastAdjustedQuestionCompoundSpecificationItem != null &&
		// Write last instance of adjusted compound question
		generalizationWordItem.writeUpdatedSpecification( true, false, false, true, false, false, false, lastAdjustedQuestionCompoundSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted compound question" );

		return Constants.RESULT_OK;
		}

	private byte makeOppositePossessiveSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasExistingSpecificationCurrentCreationSentenceNr = false;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		boolean hasFeminineOrMasculineProperNounEnding = false;
		boolean hasRelationWordConfirmedSpecification;
		boolean hasRelationWordMasculineProperNounEnding = false;
		boolean hasSameSpecificationCollection;
		boolean isAlternativeSecondaryUserSpecification = false;
		boolean isExistingHiddenSpanishSpecification = false;
		boolean isExistingUserSpecification = false;
		boolean isOlderCurrentExistingSpecification = false;
		boolean isOlderExistingSpecification = false;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isRelationWordUserWordGeneralization;
		boolean isSecondaryUserSpecification;
		short currentExistingAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		short selectedJustificationTypeNr;
		int createAssumptionLevel;
		int hiddenSpanishSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int relationContextNr;
		SpecificationItem additionalDefinitionSpecificationItem = null;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentExistingSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem feminineSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem selectedSpecificationItem = null;
		SpecificationItem tempSpecificationItem;
		WordItem alternativeSecondarySpecificationWordItem;
		WordItem currentContextWordItem;
		WordItem feminineSpecificationWordItem = null;
		WordItem oppositePossessiveDefinitionSpecificationWordItem;
		WordItem possessiveDefinitionSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( oppositePossessiveDefinitionSpecificationItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item is undefined" );

		if( possessiveDefinitionSpecificationItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The possessive definition specification item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( oppositePossessiveDefinitionSpecificationWordItem = oppositePossessiveDefinitionSpecificationItem_.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification item has no specification word" );

		if( ( possessiveDefinitionSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The The possessive definition specification item has no specification word item" );

		isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();

		if( !isPossessiveSpecificationWordSpanishAmbiguous &&
		secondarySpecificationItem.hasCompoundSpecificationCollection() )
			return adminItem_.startError( 1, moduleNameString_, "Some preconditions are not met" );

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
		hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem.hasCurrentlyMoreSpecificSpecification();
		hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();
		isRelationWordUserWordGeneralization = relationWordItem.isUserGeneralizationWord;

		isSecondaryUserSpecification = secondarySpecificationItem.isUserSpecification();

		if( alternativeSecondarySpecificationItem != null )
			isAlternativeSecondaryUserSpecification = alternativeSecondarySpecificationItem.isUserSpecification();

		if( ( existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, !isPossessive, possessiveDefinitionSpecificationWordItem ) ) != null )
			{
			hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem.hasCurrentCreationSentenceNr();
			isExistingHiddenSpanishSpecification = existingSpecificationItem.isHiddenSpanishSpecification();
			isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
			isExistingUserSpecification = existingSpecificationItem.isUserSpecification();
			}

		if( isPossessive )
			{
			selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;

			selectedPrimarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? possessiveDefinitionSpecificationItem_ :
																								oppositePossessiveDefinitionSpecificationItem_ );

			selectedSecondarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																									secondarySpecificationItem );
			}
		else
			{
			selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION;

			selectedPrimarySpecificationItem = ( isPossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																								secondarySpecificationItem );
			selectedSecondarySpecificationItem = null;

			}

		// User specification doesn't exist
		if( ( ( existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) == null &&
		alternativeSecondarySpecificationItem != null &&
		!possessiveDefinitionSpecificationWordItem.isUserSpecificationWord &&

		( ( isPossessive &&
		!isPossessiveSpecificationWordSpanishAmbiguous &&
		!isOlderExistingSpecification &&
		isRelationWordUserWordGeneralization &&
		existingSpecificationItem != null &&
		// Justification doesn't exist yet in specification
		!existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ||

		// Typical for Spanish
		( isPossessiveSpecificationWordSpanishAmbiguous &&
		isRelationWordUserWordGeneralization &&
		existingSpecificationItem == null ) ) ) ||

		// Typical for Spanish
		( isPossessiveSpecificationWordSpanishAmbiguous &&
		alternativeSecondarySpecificationItem != null &&

		( existingUserSpecificationItem != null ||

		( isAlternativeSecondaryUserSpecification &&

		( existingSpecificationItem == null ||
		!existingSpecificationItem.hasNonCompoundSpecificationCollection() ||

		// Justification doesn't exist yet in specification
		( !existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ) ) ) &&

		// User specification doesn't exist yet
		( ( existingUserSpecificationItem == null &&

		( existingSpecificationItem == null ||

		( ( hasExistingSpecificationCurrentCreationSentenceNr ||
		!alternativeSecondarySpecificationItem.isOlderItem() ) &&

		// Justification doesn't exist yet in specification
//		!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ) ) ||

		// User specification exists
		( existingUserSpecificationItem != null &&
		existingUserSpecificationItem.isOlderItem() &&
		existingUserSpecificationItem.hasCompoundSpecificationCollection() &&

		( hasGeneralizationWordCurrentlyConfirmedSpecification ||

		( isRelationWordUserWordGeneralization &&
		existingSpecificationItem != null &&
		// Justification doesn't exist yet in specification
		!existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
		// Justification has at least the same assumption level
		existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, selectedJustificationTypeNr, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) ) ) ) ) &&

		( ( !isPossessive &&
		existingUserSpecificationItem != null ) ||

		alternativeSecondarySpecificationItem.isActiveAssignment() ||

		// User specification doesn't exist
		( //existingUserSpecificationItem == null &&
		!generalizationWordItem.hasActiveQuestionWithCompoundCollection() &&

		// Not hidden
		( !isExistingHiddenSpanishSpecification ||

		// Hidden
		( isOlderExistingSpecification &&
		hasExistingSpecificationCurrentCreationSentenceNr ) ) ) ) ) )
			{
			// Make an opposite possessive specification assumption (normal)
			if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isExclusiveSpecification, false, false, !isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
			}
		else
			{
			if( isPossessiveSpecificationWordSpanishAmbiguous &&
			// Draw Spanish proper noun part-of conclusions in proper noun words
			drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
			}

		if( existingSpecificationItem != null &&
		alternativeSecondarySpecificationItem != null &&
		alternativeSecondarySpecificationItem.isPossessive() &&
		!existingSpecificationItem.hasCurrentCreationSentenceNr() &&
		existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
		// Avoid idle call
		possessiveDefinitionSpecificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null &&
		// Draw a specification substitution conclusion or ask a question
		drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, relationWordItem, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + relationWordItem.anyWordTypeString() + "\"" );

		if( existingSpecificationItem != null ||
		alternativeSecondarySpecificationItem != null )
			{
			if( alternativeSecondarySpecificationItem != null )
				{
				if( ( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem.specificationWordItem() ) != null &&
				// Avoid idle call
				alternativeSecondarySpecificationWordItem.firstNegativeSpecificationItem() != null &&
				( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, alternativeSecondarySpecificationWordItem ) ) != null &&
				!foundSpecificationItem.isHiddenSpanishSpecification() &&
				// Draw simple negative conclusions
				drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, alternativeSecondarySpecificationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				// Try to find user specification or self-generated conclusion first
				if( ( selectedSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem, null ) ) != null &&
				isAlternativeSecondaryUserSpecification &&
				selectedSpecificationItem.isHiddenSpanishSpecification() )
					selectedSpecificationItem = null;

				// If not found, try to find a self-generated assumption
				if( selectedSpecificationItem == null &&
				( assumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null )
					{
					hasSameSpecificationCollection = ( assumptionSpecificationItem.specificationCollectionNr() == alternativeSecondarySpecificationItem.specificationCollectionNr() );

					if( !assumptionSpecificationItem.hasCurrentCreationSentenceNr() &&
					!assumptionSpecificationItem.hasNewInformation() &&

					( hasGeneralizationWordCurrentlyConfirmedSpecification == hasSameSpecificationCollection ||
					alternativeSecondarySpecificationItem.isPossessive() ) )
						selectedSpecificationItem = assumptionSpecificationItem;
					}
				}

			isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();
			feminineSpecificationWordItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationWordItem.feminineCollectionWordItem() :
																										oppositePossessiveDefinitionSpecificationWordItem.feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem.hasFeminineProperNounEnding() : relationWordItem.hasFeminineProperNounEnding() ) );

			if( feminineSpecificationWordItem != null )
				{
				if( isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
					additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, false, feminineSpecificationWordItem );
				else
					feminineSpecificationWordItem = null;
				}

			if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
			isAlternativeSecondaryUserSpecification &&
			isFeminineOrMasculineProperNounEnding( isPossessive, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) )
				hasFeminineOrMasculineProperNounEnding = true;

			if( ( ( selectedSpecificationItem == null ||

			( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
			!hasGeneralizationWordCurrentlyMoreSpecificSpecification &&
			!isRelationWordUserWordGeneralization &&
			existingSpecificationItem != null &&
			existingSpecificationItem.isSelfGeneratedSpecification() &&

			( additionalDefinitionSpecificationItem != null ||
			!selectedSpecificationItem.hasRelationContext() ||

			( selectedSpecificationItem.isSelfGeneratedSpecification() &&
			relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) == null ) ) ) ||

			( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
			isRelationWordUserWordGeneralization &&
			selectedSpecificationItem.hasOnlyOneRelationWord() &&
			!selectedSpecificationItem.isHiddenSpanishSpecification() ) ||

			// Relation context doesn't exist yet
			( generalizationWordItem.hasContextInWord( selectedSpecificationItem.relationContextNr() ) &&
			// Try to find user specification or self-generated conclusion first
			relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, isPossessive, true, oppositePossessiveDefinitionSpecificationWordItem, null ) != null ) ||

			( isRelationWordUserWordGeneralization &&
			selectedSpecificationItem.isSelfGeneratedSpecification() &&
			relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) != null ) ||

			( isOppositePossessiveSpecificationWordSpanishAmbiguous &&

			( ( relationContextNr = selectedSpecificationItem.relationContextNr() ) == Constants.NO_CONTEXT_NR ||

			( !isRelationWordUserWordGeneralization &&
			selectedSpecificationItem.isUserSpecification() &&
			// Relation context doesn't exist
			!generalizationWordItem.hasContextInWord( relationContextNr ) ) ) ) ) &&

			( isOppositePossessiveSpecificationWordSpanishAmbiguous ||

			( ( relationWordItem.secondarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == null ||

			( feminineSpecificationWordItem != null &&
			// Skip if feminine specification exists as user specification
			relationWordItem.firstUserSpecificationItem( false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, feminineSpecificationWordItem ) == null ) ) ) ) ) &&

			( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
			// Typical for Spanish
			!isPossessive ||
			isRelationWordUserWordGeneralization ||
			additionalDefinitionSpecificationItem == null ||
			generalizationWordItem.firstUserSpecificationItem( false, isPossessive, additionalDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, null ) != null ) )
				{
				// Typical for Spanish
				if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
				existingSpecificationItem != null &&

				( isExistingUserSpecification ||
				isExistingHiddenSpanishSpecification ) )
					hiddenSpanishSpecificationCollectionNr = possessiveDefinitionSpecificationItem_.specificationCollectionNr();

				if( ( currentExistingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, false, isPossessive, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null )
					{
					isOlderCurrentExistingSpecification = currentExistingSpecificationItem.isOlderItem();
					currentExistingAssumptionLevel = currentExistingSpecificationItem.assumptionLevel();
					}

				selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
				selectedPrimarySpecificationItem = possessiveDefinitionSpecificationItem_;
				selectedAdditionalDefinitionSpecificationItem = null;
				selectedSecondarySpecificationItem = ( isOppositePossessiveSpecificationWordSpanishAmbiguous ? alternativeSecondarySpecificationItem :
																												secondarySpecificationItem );

				createAssumptionLevel = adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null );

				if( ( alternativeSecondarySpecificationItem == null ||

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

				( existingSpecificationItem == null ||
				currentExistingSpecificationItem == null ||

				// Spanish test file: "Español ambigua - Luisa - Pablo - Luisa"
				( additionalDefinitionSpecificationItem == null &&
				currentExistingSpecificationItem.hasOnlyOneRelationWord() ) ||

				// Test file: "Que haya confirmado al menos una palabra de relación"
				( ( !hasRelationWordConfirmedSpecification &&

				( !isOlderExistingSpecification ||
				!isPossessive ||
				currentExistingAssumptionLevel >= createAssumptionLevel ) ) ||

				( ( !existingSpecificationItem.isSelfGeneratedAssumption() ||
				currentExistingSpecificationItem.hasCurrentCreationSentenceNr() ) &&

				( ( !hasFeminineOrMasculineProperNounEnding &&
				currentExistingAssumptionLevel >= createAssumptionLevel ) ||

				existingSpecificationItem.isSelfGeneratedSpecification() ||

				( isOlderCurrentExistingSpecification &&
				!hasFeminineOrMasculineProperNounEnding &&
				!alternativeSecondarySpecificationItem.isOlderItem() ) ) ) ) ) ) ) &&

				( currentExistingSpecificationItem == null ||

				( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
				currentExistingSpecificationItem.isHiddenSpanishSpecification() ) ||

				// Justification doesn't exist yet in specification
				!currentExistingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) ) )
					{
					if( !isPossessive &&
					additionalDefinitionSpecificationItem == null &&
					relationWordItem.hasMasculineProperNounEnding() &&
					( tempSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null )
						{
						hasRelationWordMasculineProperNounEnding = true;
						selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
						selectedPrimarySpecificationItem = selectedSecondarySpecificationItem;
						selectedAdditionalDefinitionSpecificationItem = possessiveDefinitionSpecificationItem_;
						selectedSecondarySpecificationItem = tempSpecificationItem;
						}

					// Make an assumption
					if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an assumption from generalization word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

					if( isPossessive )
						{
						// Typical for Spanish
						if( hasRelationWordConfirmedSpecification &&
						( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null &&
						( relationContextNr = foundSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
						( currentContextWordItem = adminItem_.firstContextWordItem( relationContextNr ) ) != null )
							{
							// Do for all context words with relation context
							do	{
								if( currentContextWordItem != generalizationWordItem &&
								( selectedSecondarySpecificationItem = currentContextWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != null &&
								// Justification doesn't exist yet in specification
								!foundSpecificationItem.hasSecondarySpecificationJustification( selectedSecondarySpecificationItem ) &&
								// Justification has at least the same assumption level
								foundSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
								// Add Spanish opposite possessive conclusion justification to found specification
								foundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a Spanish opposite possessive conclusion justification to the found specification" );
								}
							while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( relationContextNr ) ) != null );
							}
						}
					else
						{
						if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
							{
							if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) == null )
								{
								// Typical for Spanish
								// Spanish test file: "Reto científico"
								if( isRelationWordUserWordGeneralization &&
								hiddenSpanishSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
								additionalDefinitionSpecificationItem == null &&
								relationWordItem.hasFeminineProperNounEnding() &&
								( feminineSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstNegativeSpecificationItem() ) != null &&
								( feminineSpecificationWordItem = feminineSpecificationItem.specificationWordItem() ) != null )
									{
									// Make feminine assumption
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, hiddenSpanishSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine assumption from generalization word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );

									if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
									// Specification doesn't exist yet
									( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, true, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem.hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null, null ) &&
									// Add reversible justification to existing specification
									existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the existing specification" );
									}
								}
							else
								{
								if( isOlderExistingSpecification &&
								!isOppositePossessiveSpecificationWordSpanishAmbiguous &&
								// Justification has at least the same assumption level
								foundSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null, null ) &&
								// Add reversible justification
								createdSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, foundSpecificationItem, null, null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a reversible justification to the found specification" );
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
					secondarySpecificationItem.isHiddenSpanishSpecification() &&
					currentExistingSpecificationItem != null &&
					!currentExistingSpecificationItem.isHiddenSpanishSpecification() &&
					// Draw a specification substitution conclusion or ask a question
					drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}

				if( alternativeSecondarySpecificationItem != null &&
				feminineSpecificationWordItem != null &&

				( ( isPossessive ||
				!secondarySpecificationItem.isSelfGeneratedAssumption() ||
				generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, secondarySpecificationWordItem, relationWordItem ) == null ) &&

				( possessiveDefinitionSpecificationWordItem == secondarySpecificationWordItem ||

				( !isPossessiveSpecificationWordSpanishAmbiguous &&
				isSecondaryUserSpecification ) ) ) &&

				( alternativeSecondarySpecificationWordItem = alternativeSecondarySpecificationItem.specificationWordItem() ) != null )
					{
					existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, feminineSpecificationWordItem, generalizationWordItem );

					selectedPrimarySpecificationItem = ( isPossessive &&
														existingSpecificationItem == null &&
														userSpecificationItem.assumptionLevel() < secondarySpecificationItem.assumptionLevel() ? userSpecificationItem :

														( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
														possessiveDefinitionSpecificationWordItem == alternativeSecondarySpecificationWordItem ? alternativeSecondarySpecificationItem :
																																				secondarySpecificationItem ) );
					selectedAdditionalDefinitionSpecificationItem = ( !isPossessive &&
																	isOppositePossessiveSpecificationWordSpanishAmbiguous ? oppositePossessiveDefinitionSpecificationItem_ :
																															possessiveDefinitionSpecificationItem_ );
					selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;

					if( existingSpecificationItem == null ||

					( !selectedPrimarySpecificationItem.isPossessive() &&
					// Justification doesn't exist yet in specification
					!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) )
						{
						relationContextNr = ( existingSpecificationItem == null ? Constants.NO_CONTEXT_NR : existingSpecificationItem.relationContextNr() );

						// Make a feminine possessive exclusive specification substitution assumption
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, isExclusiveSpecification, false, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, relationContextNr, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine possessive exclusive specification substitution assumption with generalization word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );

						if( isPossessive )
							{
							if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&

							// Specification doesn't exist yet
							( ( existingSpecificationItem = generalizationWordItem.firstNonExclusiveSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) == null ||

							// Justification doesn't exist yet in specification
							( !existingSpecificationItem.hasPrimarySpecificationJustification( createdOrFoundSpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, createdOrFoundSpecificationItem, null, null, null ) ) ) &&
							// Draw possessive reversible conclusion
							adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, null, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							feminineSpecificationWordItem_ = feminineSpecificationWordItem;

							// Typical for Spanish
							// Spanish test file: "Juan - Ana (antes de la definición de la familia)"
							if( isOppositePossessiveSpecificationWordSpanishAmbiguous &&
							existingSpecificationItem != null &&
							!existingSpecificationItem.isOlderItem() &&
							( selectedPrimarySpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, userSpecificationWordItem ) ) != null &&
							// Justification doesn't exist yet in specification
							!existingSpecificationItem.hasPrimarySpecificationJustification( selectedPrimarySpecificationItem ) &&
							// Justification has at least the same assumption level
							existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) &&
							// Add possessive exclusive specification substitution justification to existing specification
							existingSpecificationItem.addJustificationToSpecification( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a possessive exclusive specification substitution assumption justification to the existing specification" );
							}
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNounEnding, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationItem possessiveSpecificationItem;
		WordItem userSpecificationWordItem;

		alternativeSpanishAdditionalDefinitionSpecificationItem_ = null;

		if( userSpecificationItem != null )
			{
			if( generalizationWordItem.isUserRelationWord &&
			userSpecificationItem.isPossessive() )
				return userSpecificationItem;

			if( !hasGeneralizationWordFeminineProperNounEnding &&
			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null &&
			// Try to find a possessive specification
			( possessiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, true, false, userSpecificationWordItem ) ) != null )
				{
				alternativeSpanishAdditionalDefinitionSpecificationItem_ = possessiveDefinitionSpecificationItem_;
				return possessiveSpecificationItem;
				}
			}

		return null;
		}

	private static SpecificationItem getAdditionalDefinitionSpecificationItem( WordItem userSpecificationWordItem )
		{
		SpecificationItem additionalDefinitionSpecificationItem = null;
		SpecificationItem possessiveDefinitionSpecificationItem;
		WordItem currentPossessiveNounWordItem;
		WordItem higherLevelPossessiveDefinitionSpecificationWordItem = null;
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
							higherLevelPossessiveDefinitionSpecificationWordItem = possessiveSpecificationWordItem;
						}
					while( higherLevelPossessiveDefinitionSpecificationWordItem == null &&
					( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() ) != null );

					// Try to find the opposite of the possessive specification
					if( ( possessiveDefinitionSpecificationItem = currentPossessiveNounWordItem.firstPossessiveSpecificationItem() ) != null )
						additionalDefinitionSpecificationItem = ( higherLevelPossessiveDefinitionSpecificationWordItem == null ||
																higherLevelPossessiveDefinitionSpecificationWordItem == possessiveDefinitionSpecificationItem.specificationWordItem() ? possessiveDefinitionSpecificationItem.nextPossessiveSpecificationItem() :
																																														possessiveDefinitionSpecificationItem );
					}
				}
			while( additionalDefinitionSpecificationItem == null &&
			( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );
			}

		return additionalDefinitionSpecificationItem;
		}


	// Constructor

	protected AdminReasoning( AdminItem adminItem )
		{
		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminReasoningVariables()
		{
		hasSuggestiveQuestionAssumption_ = false;

		adjustedSpanishQuestionSecondarySpecificationItem_ = null;

		feminineSpecificationWordItem_ = null;
		firstCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;
		secondCorrectedAssumptionByOppositeSuggestiveQuestionSpecificationWordItem_ = null;
		}

	protected byte askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean hasNonCompoundSecondarySpecificationCollection;
		boolean hasUserSpecificationRelationContext;
		boolean isAdjectiveSecondarySpecification;
		boolean isCommonWordWordSpanishAmbiguous = false;
		boolean isExclusiveSecondarySpecification;
		boolean isFeminineUserSpecificationWord = false;
		boolean isHiddenSpanishPrimarySpecification;
		boolean isMasculineUserSpecificationWord = false;
		boolean isPrimarySameAsUserSpecification;
		boolean isPrimaryUserSpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		int compoundSpecificationCollectionNr;
		int nonCompoundSpecificationCollectionNr;
		int secondarySpecificationCollectionNr;
		int specificationCollectionNr;
		int userSpecificationCollectionNr;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem firstNonCompoundCollectionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordItem collectionWordItem = null;
		WordItem commonWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		RelatedResultType relatedResult;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr() ) == Constants.NO_COLLECTION_NR )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification collection" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );

		if( ( commonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationCollectionNr ) ) != null )
			isCommonWordWordSpanishAmbiguous = commonWordItem.isNounWordSpanishAmbiguous();

		if( commonWordItem == null ||
		( specificationCollectionNr = commonWordItem.collectionNr( specificationWordItem ) ) == Constants.NO_COLLECTION_NR )
			specificationCollectionNr = secondarySpecificationCollectionNr;

		if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		relatedSpecificationItem = relatedResult.relatedSpecificationItem;

		isHiddenSpanishPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();
		isPrimarySameAsUserSpecification = ( primarySpecificationItem == userSpecificationItem );
		isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();

		hasNonCompoundSecondarySpecificationCollection = secondarySpecificationItem.hasNonCompoundSpecificationCollection();
		isAdjectiveSecondarySpecification = secondarySpecificationItem.isSpecificationAdjective();
		isExclusiveSecondarySpecification = secondarySpecificationItem.isExclusiveSpecification();

		hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
		hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
		hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();
		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
		userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

		// Current question part isn't related to an answer or a similar question
		if( isCommonWordWordSpanishAmbiguous &&
		commonWordItem != null &&
		relatedSpecificationItem == null &&
		!userSpecificationItem.isPossessive() &&
		( nonCompoundSpecificationCollectionNr = commonWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR )
			{
			if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

			relatedSpecificationItem = relatedResult.relatedSpecificationItem;
			}

		if( relatedSpecificationItem == null )
			{
			if( !hasUserSpecificationRelationContext &&
			!isPrimarySameAsUserSpecification &&
			!userSpecificationItem.isExclusiveSpecification() &&

			( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null &&

			( isAdjectiveSecondarySpecification ||
			!userSpecificationWordItem.isNounWordSpanishAmbiguous() ||

			( ( compoundSpecificationCollectionNr = secondarySpecificationWordItem.compoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
			secondarySpecificationWordItem.compoundGeneralizationWordItem( compoundSpecificationCollectionNr ) != userSpecificationWordItem ) ) &&

			( GlobalVariables.nUserSpecificationWords == 1 ||
			// Typical for Spanish
			userSpecificationWordItem.isNounWordSpanishAmbiguous() ) )
				{
				if( userSpecificationWordItem.isFeminineWord() )
					isFeminineUserSpecificationWord = true;

				if( userSpecificationWordItem.isMasculineWord() )
					isMasculineUserSpecificationWord = true;
				}

			if( isFeminineUserSpecificationWord ||
			isMasculineUserSpecificationWord ||

			// Specification is Spanish ambiguous
			( isSpecificationWordSpanishAmbiguous &&
			!hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&

			( hasGeneralizationWordFeminineProperNounEnding ||
			userSpecificationItem.isQuestion() ) ) )
				{
				}
			else
				{
				// Skip invalid questions (step 1)
				if( ( !hasGeneralizationWordFeminineOrMasculineProperNounEnding ||
				primarySpecificationItem.hasCompoundSpecificationCollection() ||

				( isPrimarySameAsUserSpecification &&
				!secondarySpecificationWordItem.isFeminineOrMasculineWord() ) ) &&

				( !isSpecificationWordSpanishAmbiguous ||
				// Typical for Spanish
				hasNonCompoundSecondarySpecificationCollection ||
				secondarySpecificationWordItem.compoundCollectionNr() == secondarySpecificationCollectionNr ) )
					{
					if( isSpecificationWordSpanishAmbiguous )
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem );
					else
						collectionWordItem = secondarySpecificationWordItem.collectionWordItem( secondarySpecificationCollectionNr );

					// Skip invalid questions (step 2)
					if( ( collectionWordItem != null &&

					( isPrimaryUserSpecification ||

					( primarySpecificationItem.hasRelationContext() ||
					generalizationWordItem.primaryAndSecondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, primarySpecificationItem, secondarySpecificationItem ) == null ) ) &&

					generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, collectionWordItem ) == null ) ||

					( isSpecificationWordSpanishAmbiguous &&

					( ( !isPrimaryUserSpecification &&

					( existingSpecificationItem == null ||
					existingSpecificationItem.isHiddenSpanishSpecification() ) ) ||

					( isPrimaryUserSpecification &&
					primarySpecificationItem.isExclusiveSpecification() ) ) ) )
						{
						if( ( isExclusiveSecondarySpecification ||
						isPrimarySameAsUserSpecification ||
						hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ) &&

						// Ask questions
						askQuestions( userSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions" );

						if( !hasUserSpecificationRelationContext &&
						isAdjectiveSecondarySpecification &&
						isUserGeneralizationWord &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Typical for Spanish
						// Spanish test file: "Esta información es más específico (no exclusiva)"
						!isExclusiveSecondarySpecification ) &&

						// Question doesn't exist yet as specification
						generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, secondarySpecificationWordItem ) == null &&
						// Justification doesn't exist yet in word
						generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, secondarySpecificationItem ) == null &&
						// Ask adjective question
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, Constants.WORD_TYPE_ADJECTIVE, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to ask an adjective question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					{
					// Test files: "Scientific challenge" and "John - Anna (before family definition)"
					// Definition specification
					if( userSpecificationItem.isGeneralizationNoun() &&
					primarySpecificationItem.hasRelationContext() &&
					primarySpecificationItem.isOlderItem() &&
					primarySpecificationItem.isSelfGeneratedSpecification() &&

					// Question part doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) ) == null ||
					// Justification doesn't exist yet in question
					!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) ) &&

					// Ask a specification substitution question
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, primarySpecificationItem.generalizationWordTypeNr(), secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask a question with a question as primary specification about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}
		else
			{
			if( isExclusiveSecondarySpecification )
				{
				if( !isCommonWordWordSpanishAmbiguous &&
				( firstNonCompoundCollectionSpecificationItem = generalizationWordItem.firstNonCompoundCollectionSpecificationItem( secondarySpecificationCollectionNr ) ) != null )
					relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

				// Typical for Spanish
				if( isCommonWordWordSpanishAmbiguous &&
				isSpecificationWordSpanishAmbiguous &&
				isPrimaryUserSpecification &&
				GlobalVariables.nUserSpecificationWords == 1 &&
				!primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
				primarySpecificationItem.specificationCollectionNr() != secondarySpecificationCollectionNr )
					{
					// Ask Spanish ambiguous questions
					// Spanish test file: "Español ambigua - Ana es un padre - Luisa es un hijo"
					if( hasGeneralizationWordFeminineOrMasculineProperNounEnding &&
					// Question part doesn't exist yet
					generalizationWordItem.firstSpecificationItem( false, false, true, secondarySpecificationWordItem ) == null &&
					askQuestions( userSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to ask questions" );
					}
				else
					{
					if( hasNonCompoundSecondarySpecificationCollection ||
					secondarySpecificationCollectionNr == userSpecificationCollectionNr ||

					( secondarySpecificationItem.isUserSpecification() &&
					secondarySpecificationWordItem.compoundCollectionNr() == secondarySpecificationCollectionNr ) )
						{
						if( relatedSpecificationItem.isOlderItem() &&
						relatedSpecificationItem.isSelfGeneratedAssumption() &&
						( conclusionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, false, false, false, relatedSpecificationItem.specificationWordItem(), userSpecificationItem.generalizationWordItem() ) ) != null )
							relatedSpecificationItem = conclusionSpecificationItem;

						if( !isHiddenSpanishPrimarySpecification &&
						!primarySpecificationItem.hasRelationContext() &&
						// Negative specification doesn't exist
						generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, true, false, false, secondarySpecificationWordItem ) == null &&
						// Draw negative conclusion
						drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte drawCompoundSpecificationSubstitutionConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem selectedPrimarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasCurrentCompoundSpecificationCollection;
		boolean hasCurrentNonCompoundSpecificationCollection;
		boolean hasCurrentSpecificationCollection;
		boolean hasCurrentSpecificationCurrentCreationSentenceNr;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasGeneralizationWordFeminineOrMasculineProperNounEnding;
		boolean hasPrimarySpecificationRelationContext;
		boolean isAdjectiveCurrentSpecification;
		boolean isExclusiveCurrentSpecification;
		boolean isExclusivePrimarySpecification;
		boolean isExclusiveUserSpecification = false;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isOlderPrimarySpecification;
		boolean isPrimaryEveryGeneralization;
		boolean isPrimaryUserAssignment;
		boolean isPrimaryUserQuestion;
		boolean isSelectedExclusiveSpecification;
		boolean isSpanishCurrentLanguage;
		short currentSpecificationWordTypeNr;
		short selectedJustificationTypeNr;
		int currentSpecificationCollectionNr;
		int primarySpecificationCollectionNr;
		SpecificationItem adjectiveSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem exclusiveSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem previousUserSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem selectedCurrentSpecificationItem;
		WordItem compoundGeneralizationWordItem;
		WordItem currentSpecificationWordItem;
		WordItem previousUserSpecificationWordItem;
		WordItem primarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item has no specification word item" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() ) != null )
			{
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();

			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
			hasGeneralizationWordFeminineOrMasculineProperNounEnding = generalizationWordItem.hasFeminineOrMasculineProperNounEnding();

			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
			isExclusivePrimarySpecification = primarySpecificationItem.isExclusiveSpecification();
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			isPrimaryEveryGeneralization = primarySpecificationItem.isEveryGeneralization();
			isPrimaryUserAssignment = primarySpecificationItem.isUserAssignment();
			isPrimaryUserQuestion = primarySpecificationItem.isUserQuestion();
			primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr();

			compoundGeneralizationWordItem = specificationWordItem.compoundGeneralizationWordItem( primarySpecificationCollectionNr );

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem &&
				currentSpecificationWordItem != compoundGeneralizationWordItem )
					{
					hasCurrentCompoundSpecificationCollection = currentSpecificationItem.hasCompoundSpecificationCollection();
					hasCurrentNonCompoundSpecificationCollection = currentSpecificationItem.hasNonCompoundSpecificationCollection();
					isAdjectiveCurrentSpecification = currentSpecificationItem.isSpecificationAdjective();
					isExclusiveCurrentSpecification = currentSpecificationItem.isExclusiveSpecification();
					currentSpecificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();

					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR );
					isSelectedExclusiveSpecification = ( hasCurrentSpecificationCollection &&
														isExclusiveSpecification &&
														currentSpecificationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR );

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
					generalizationWordItem.firstSpecificationItem( false, false, true, currentSpecificationWordItem ) == null ) &&

					// Specification doesn't exist yet
					generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, null ) == null ) ||

					// Non-exclusive definition specification
					( ( !isGeneralizationProperNoun &&

					( ( hasCurrentNonCompoundSpecificationCollection &&
					!isExclusiveCurrentSpecification ) ||

					// Exclusive compound definition specification
					( ( hasCurrentCompoundSpecificationCollection &&
					isExclusiveCurrentSpecification &&
					compoundGeneralizationWordItem != null &&
					currentSpecificationWordItem.isFeminineOrMasculineWord() ) ||

					// Older definition specification (without compound specification collection)
					( !hasCurrentCompoundSpecificationCollection &&

					( !isExclusiveCurrentSpecification ||
					isSelectedExclusiveSpecification ) &&

					currentSpecificationItem.isOlderItem() ) ) ) ) &&

					// User specification doesn't exist yet
					generalizationWordItem.firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) )
						{
						selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;

						if( selectedPrimarySpecificationItem == null )
							selectedPrimarySpecificationItem = ( !isArchivedAssignment &&
																isPrimaryUserAssignment ? userSpecificationItem :
																							primarySpecificationItem );

						selectedCurrentSpecificationItem = ( isArchivedAssignment &&
															!hasPrimarySpecificationRelationContext ? userSpecificationItem :
																										currentSpecificationItem );

						if( isAdjectiveCurrentSpecification )
							{
							if( compoundGeneralizationWordItem == null )
								{
								if( isExclusiveCurrentSpecification )
									{
									// Create adjective question
									isSelectedExclusiveSpecification = true;
									questionParameter = Constants.WORD_PARAMETER_SINGULAR_VERB_IS;
									selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION;
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
										primarySpecificationCollectionNr > Constants.NO_COLLECTION_NR )
											// Test file: "Scientific challenge"
											isSelectedExclusiveSpecification = true;
										}
									}
								}
							else
								{
								if( !isGeneralizationProperNoun &&
								// Specification doesn't exist yet
								generalizationWordItem.firstSpecificationItem( false, false, false, currentSpecificationWordItem ) == null )
									// Create exclusive adjective specification
									isSelectedExclusiveSpecification = true;
								}
							}

						hasCurrentSpecificationCurrentCreationSentenceNr = currentSpecificationItem.hasCurrentCreationSentenceNr();

						if( !hasCurrentSpecificationCurrentCreationSentenceNr ||
						isSelectedExclusiveSpecification ||

						( !isExclusiveUserSpecification &&
						!isSpanishCurrentLanguage ) )
							{
							if( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, questionParameter, currentSpecificationWordItem ) ) != null &&
							!existingSpecificationItem.isExclusiveSpecification() &&
							isExclusiveUserSpecification &&
							isSelectedExclusiveSpecification )
								// Typical for Spanish
								// Spanish test file: "Reto científico"
								existingSpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem( currentSpecificationWordItem );

							// Specification doesn't exist yet
							if( existingSpecificationItem == null ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedCurrentSpecificationItem ) )
								{
								// Draw specification substitution conclusion
								if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isPrimaryEveryGeneralization, false, isSelectedExclusiveSpecification, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, selectedCurrentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

								if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null )
									{
									if( ( isGeneralizationProperNoun ||
									!createdOrFoundSpecificationItem.isExclusiveSpecification() ) &&

									// Test file: "Scientific challenge"
									specificationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, currentSpecificationCollectionNr, null ) != null &&
									// Draw simple negative conclusions
									drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									if( !isGeneralizationProperNoun )
										{
										if( isSelectedExclusiveSpecification )
											{
											if( hasCurrentNonCompoundSpecificationCollection &&
											hasCurrentSpecificationCurrentCreationSentenceNr &&
											!isSpanishCurrentLanguage &&
											// Make generalization assumption definition
											adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, generalizationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, currentSpecificationItem, null, specificationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption definition about generalization word \"" + specificationWordItem.anyWordTypeString() + "\" and specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											if( !hasCurrentNonCompoundSpecificationCollection &&
											selectedPrimarySpecificationItem.hasSpecificationCollection() &&
											( adjectiveSpecificationItem = userSpecificationWordItem.firstAdjectiveSpecificationItem( false ) ) != null &&
											( exclusiveSpecificationItem = currentSpecificationWordItem.firstAdjectiveSpecificationItem( false ) ) != null )
												{
												if( createdOrFoundSpecificationItem.isOlderItem() &&
												// Make user generalization assumption definition
												( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.generalizationWordTypeNr(), exclusiveSpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, exclusiveSpecificationItem, null, adjectiveSpecificationItem, null, userSpecificationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make a user generalization assumption definition about generalization word \"" + userSpecificationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

												// Find previous user specification
												if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, userSpecificationItem ) ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

												if( !currentSpecificationItem.isReplacedItem() &&
												( previousUserSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
												( previousUserSpecificationWordItem = previousUserSpecificationItem.specificationWordItem() ) != null &&
												( adjectiveSpecificationItem = previousUserSpecificationWordItem.firstAdjectiveSpecificationItem( false ) ) != null &&
												( exclusiveSpecificationItem = currentSpecificationWordItem.firstAdjectiveSpecificationItem( false ) ) != null &&

												// Specification doesn't exist yet
												( ( existingSpecificationItem = previousUserSpecificationWordItem.firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) == null ||
												// Justification doesn't exist yet in specification
												!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( exclusiveSpecificationItem, adjectiveSpecificationItem ) ) )
													{
													// Make previous user generalization assumption definition
													if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, previousUserSpecificationItem.generalizationWordTypeNr(), exclusiveSpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, exclusiveSpecificationItem, null, adjectiveSpecificationItem, null, previousUserSpecificationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make a previous user generalization assumption definition about generalization word \"" + previousUserSpecificationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

													if( ( secondarySpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
													// Add specification substitution justification to created or found specification
													createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, previousUserSpecificationItem, null, secondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the created or found specification" );
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
						( selectedPrimarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, true, questionParameter, ( compoundGeneralizationWordItem == null ? specificationWordItem.compoundCollectionNr() : primarySpecificationItem.specificationCollectionNr() ), specificationWordItem ) ) != null )
							{
							// Ask adjective question
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to ask an adjective question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

							if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null &&
							( obsoleteSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, false, currentSpecificationWordItem ) ) != null )
								{
								// Write invalid assumption by opposite suggestive question (obsolete)
								if( generalizationWordItem.writeUpdatedSpecification( false, false, true, false, false, true, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid assumption by opposite suggestive question (obsolete) in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								// Replace obsolete adjective assumption by created question
								if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace the obsolete assumption specification by the created question in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						}
					}
				}
			while( ( currentSpecificationItem = ( currentSpecificationItem.isReplacedOrDeletedItem() ?
												// First specification
												specificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() :
												// Next specification
												currentSpecificationItem.nextNonNegativeNonPossessiveDefinitionSpecificationItem() ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawExclusiveNegativeUserConclusions( boolean isArchivedAssignment, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		SpecificationItem previousUserSpecificationItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( currentSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given current specification word item is undefined" );

		if( previousSpecificationWordItem != null &&
		( previousUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, previousSpecificationWordItem ) ) != null )
			{
			// Negative specification doesn't exist yet
			if( previousSpecificationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) == null &&
			// Draw negative conclusion
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, previousUserSpecificationItem, null, null, null, previousSpecificationWordItem, currentSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative user conclusion about generalization word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

			// Negative specification doesn't exist yet
			if( currentSpecificationWordItem.firstSpecificationItem( true, false, false, previousSpecificationWordItem ) == null &&
			// Draw another negative conclusion
			adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, currentSpecificationWordItem, previousSpecificationWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with specification word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawNegativeUserDefinitionConclusions( boolean isArchivedAssignment, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem adjectiveSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		WordItem currentGeneralizationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( adjectiveSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given adjective specification word item is undefined" );

		if( currentSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given current specification word item is undefined" );

		// Negative specification doesn't exist yet
		if( generalizationWordItem.firstSpecificationItem( true, false, false, adjectiveSpecificationWordItem ) == null )
			{
			// Draw adjective negative conclusion
			if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_ADJECTIVE, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, null, null, generalizationWordItem, adjectiveSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw another negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + adjectiveSpecificationWordItem.anyWordTypeString() + "\"" );

			if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
			// Get first generalization specification noun word
			( currentGeneralizationItem = adjectiveSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null &&
					currentGeneralizationWordItem != generalizationWordItem )
						{
						// Negative definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != null &&
						// Draw negative generalization conclusion (negative definition)
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative generalization conclusion with negative definition about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

						// Positive definition
						if( ( definitionSpecificationItem = currentGeneralizationWordItem.firstNonExclusiveSpecificationItem( false, false, false, adjectiveSpecificationWordItem ) ) != null )
							{
							// Negative conclusion doesn't exist yet
							if( generalizationWordItem.firstNonExclusiveSpecificationItem( true, false, false, currentGeneralizationWordItem ) == null &&
							// Draw negative generalization conclusion (positive definition)
							adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, definitionSpecificationItem, null, createdOrFoundSpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative generalization conclusion with positive definition about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

							// Draw negative generalization conclusion (reversed, positive definition)
							if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, definitionSpecificationItem, null, createdOrFoundSpecificationItem, null, currentGeneralizationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversed negative generalization conclusion with positive definition about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte drawOnlyOptionLeftNounConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, int generalizationContextNr, WordItem generalizationWordItem )
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
				if( currentSpecificationItem.isExclusiveSpecification() &&
				currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
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
			adminItem_.addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, false, false, true, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left noun conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawProperNounPartOfConclusionsInProperNounWords( boolean isArchivedAssignment, WordItem specificationWordItem, WordItem spanishRelationWordItem )
		{
		boolean hasUserSpecificationCollection;
		boolean hasUserSpecificationRelationContext;
		boolean isPossessiveUserSpecification;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserSpecificationWordSpanishAmbiguous;
		SpecificationItem primarySpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem currentProperNounWordItem;

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find the user specification item" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		hasUserSpecificationCollection = userSpecificationItem.hasSpecificationCollection();
		hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
		isPossessiveUserSpecification = userSpecificationItem.isPossessive();
		isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();

		if( ( currentProperNounWordItem = GlobalVariables.firstUserDefinedProperNounWordItem ) != null )
			{
			// Do for all user-defined proper noun words
			do	{
				if( currentProperNounWordItem != spanishRelationWordItem )
					{
					if( ( primarySpecificationItem = currentProperNounWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null ) ) == null ||
					isSpecificationWordSpanishAmbiguous )
						{
						primarySpecificationItem = currentProperNounWordItem.firstAssignmentOrSpecificationItem( false, true, false, false, false, specificationWordItem );

						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&

						( primarySpecificationItem == null ||
						primarySpecificationItem.isHiddenSpanishSpecification() ) )
							{
							if( spanishRelationWordItem == null )
								{
								// Try to find self-generated question, as alternative to hidden specification
								if( ( tempSpecificationItem = currentProperNounWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, true, specificationWordItem ) ) != null &&
								tempSpecificationItem.isExclusiveSpecification() )
									primarySpecificationItem = tempSpecificationItem;
								}
							else
								{
								// Typical for Spanish
								if( isPossessiveUserSpecification &&
								!isUserSpecificationWordSpanishAmbiguous &&
								currentProperNounWordItem.hasContextInWord( userSpecificationItem.relationContextNr() ) )
									primarySpecificationItem = userSpecificationItem;
								else
									primarySpecificationItem = spanishRelationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, true, specificationWordItem, currentProperNounWordItem );
								}

							if( primarySpecificationItem != null &&
							primarySpecificationItem.isHiddenSpanishSpecification() )
								primarySpecificationItem = null;
							}
						}
					else
						{
						// Typical for Spanish
						if( isSpanishCurrentLanguage &&
						primarySpecificationItem.isUserSpecification() &&
						( tempSpecificationItem = currentProperNounWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null )
							primarySpecificationItem = tempSpecificationItem;
						}

					if( primarySpecificationItem != null &&

					( !hasUserSpecificationCollection ||
					!hasUserSpecificationRelationContext ||
					isUserSpecificationWordSpanishAmbiguous ||
					spanishRelationWordItem != null ||
					primarySpecificationItem == userSpecificationItem ||
					primarySpecificationItem.hasCurrentCreationSentenceNr() ) &&

					// Avoid idle call
					GlobalVariables.firstPossessiveNounWordItem != null &&
					// Draw proper noun part-of conclusions
					drawProperNounPartOfConclusions( isSpanishCurrentLanguage, primarySpecificationItem, userSpecificationItem, currentProperNounWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions in proper noun word \"" + currentProperNounWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentProperNounWordItem = currentProperNounWordItem.nextUserDefinedProperNounWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawReversibleConclusions( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( !generalizationWordItem.isProperNounWord() )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item isn't a proper noun" );

		if( generalizationWordItem.firstNonQuestionSpecificationItem( true, true, false ) != null )
			{
			// Active assignments
			if( drawReversibleConclusions( true, false, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with active assignments" );
			}
		else
			{
			if( generalizationWordItem.firstNonQuestionSpecificationItem( true, true, true ) != null &&
			// Archived assignments
			drawReversibleConclusions( true, true, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with archived assignments" );
			}

		// Avoid idle call
		if( generalizationWordItem.firstNonQuestionSpecificationItem( true, false, false ) != null &&
		// Specifications
		drawReversibleConclusions( false, false, userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions with specifications" );

		return Constants.RESULT_OK;
		}

	protected byte drawSimpleNegativeConclusions( boolean isArchivedAssignment, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation;
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge;
		boolean hasUserSpecificationCurrentCreationSentenceNr;
		boolean hasUserSpecificationRelationContext;
		boolean isDrawingCompoundNegativeConclusions;
		boolean isDrawingNegativeUserConclusion;
		boolean isEveryGeneralization;
		boolean isOlderUserSpecification;
		boolean isPossessiveUserSpecification;
		boolean isProperNounArchivedAssignment;
		boolean isSelfGeneratedPrimarySpecification;
		boolean isUserAssignment;
		boolean isUserRelationWord;
		short generalizationWordTypeNr;
		short negativeSpecificationWordTypeNr;
		short userSpecificationWordTypeNr;
		int compoundCollectionNr = Constants.NO_COLLECTION_NR;
		int userSpecificationCollectionNr;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem negativeSpecificationItem;
		SpecificationItem negativeDefinitionSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem positiveSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem properNounSpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		WordItem collectionWordItem = null;
		WordItem currentGeneralizationWordItem;
		WordItem negativeSpecificationWordItem;
		WordItem reversedNegativeSpecificationWordItem;
		WordItem userSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( negativeDefinitionSpecificationItem = specificationWordItem.firstNegativeSpecificationItem() ) != null &&
		( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != null &&
		!primarySpecificationItem.isHiddenSpanishSpecification() )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation = generalizationWordItem.hasCurrentlyConfirmedSpecificationButNoRelation();
			hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem.hasCurrentlyCorrectedAssumptionByKnowledge();
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			isEveryGeneralization = primarySpecificationItem.isEveryGeneralization();
			generalizationWordTypeNr = primarySpecificationItem.generalizationWordTypeNr();
			isSelfGeneratedPrimarySpecification = primarySpecificationItem.isSelfGeneratedSpecification();

			isUserAssignment = userSpecificationItem.isAssignment();

			if( ( simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem() ) != null )
				userSpecificationItem = simpleUserSpecificationItem;

			hasUserSpecificationCurrentCreationSentenceNr = userSpecificationItem.hasCurrentCreationSentenceNr();
			hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
			isOlderUserSpecification = userSpecificationItem.isOlderItem();
			isPossessiveUserSpecification = userSpecificationItem.isPossessive();
			userSpecificationWordTypeNr = userSpecificationItem.specificationWordTypeNr();
			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();

			isDrawingNegativeUserConclusion = ( !isOlderUserSpecification &&
												userSpecificationItem.isGeneralizationProperNoun() &&
												// If the user specification is possessive, the user generalization word must be opposite, and otherwise
												userSpecificationItem.isPossessive() == isUserRelationWord &&
												!userSpecificationItem.isQuestion() );

			isDrawingCompoundNegativeConclusions = ( isDrawingNegativeUserConclusion &&
													( compoundCollectionNr = userSpecificationWordItem.compoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
													( collectionWordItem = userSpecificationWordItem.collectionWordItem( compoundCollectionNr ) ) != null );

			do	{
				if( ( negativeSpecificationWordItem = negativeDefinitionSpecificationItem.specificationWordItem() ) != null &&

				( !primarySpecificationItem.isExclusiveSpecification() ||
				negativeDefinitionSpecificationItem.isSpecificationAdjective() ) &&

				// Positive specification doesn't exist
				( generalizationWordItem.firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) == null ||
				hasGeneralizationWordCurrentlyCorrectedAssumptionByKnowledge ||
				negativeSpecificationWordItem.isNounWordSpanishAmbiguous() ) )
					{
					negativeSpecificationWordTypeNr = negativeDefinitionSpecificationItem.specificationWordTypeNr();

					if( isDrawingNegativeUserConclusion &&

					( isPossessiveUserSpecification ||
					// No answered question with this specification word
					generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, false, true, false, false, true, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, negativeSpecificationWordItem ) == null ) )
						{
						negativeSpecificationItem = ( userSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE ? negativeSpecificationWordItem.firstSpecificationItem( true, false, false, userSpecificationWordItem ) :
																														userSpecificationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) );

						if( negativeSpecificationItem != null &&
							
						// Negative specification doesn't exist
						( ( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == null &&

						( negativeSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE ||
						userSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ||

						// Negative specification exists
						( existingSpecificationItem != null &&

						( isUserAssignment ||
						negativeSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE ||

						( !hasGeneralizationWordCurrentlyConfirmedSpecificationButNoRelation &&

						( !hasUserSpecificationRelationContext ||

						( !hasGeneralizationWordCurrentlyConfirmedSpecification &&
						existingSpecificationItem.hasPrimaryNounSpecificationJustification() ) ) ) ) &&

						// Justification doesn't exist yet in specification
						!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, negativeSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, negativeSpecificationItem, null ) ) ) &&

						// Draw negative user conclusion
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, negativeSpecificationItem, null, generalizationWordItem, negativeSpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );
						}

					obsoleteSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, negativeDefinitionSpecificationItem.specificationCollectionNr(), null );

					if( ( hasUserSpecificationCurrentCreationSentenceNr ||
					isUserRelationWord ) &&

					// Negative specification doesn't exist
					( ( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) == null &&

					( !isDrawingNegativeUserConclusion ||
					isSelfGeneratedPrimarySpecification ||
					obsoleteSpecificationItem != null ) ) ||

					// Negative specification exists
					( existingSpecificationItem != null &&

					( !isDrawingNegativeUserConclusion ||
					!existingSpecificationItem.isOlderItem() ||
					existingSpecificationItem.hasPrimaryNounSpecificationJustification() ) &&

					// Justification doesn't exist yet in specification
					!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, negativeDefinitionSpecificationItem ) &&
					// Justification has at least the same assumption level
					existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, negativeDefinitionSpecificationItem, null ) ) ) )
						{
						// Draw simple negative conclusion
						if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, negativeDefinitionSpecificationItem, null, generalizationWordItem, negativeSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a simple negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );

						if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null )
							{
							if( obsoleteSpecificationItem != null &&
							createdOrFoundSpecificationItem.isCorrectedSpecification() )
								{
								// Correcting invalidated assumption by knowledge
								if( correctSuggestiveAssumptionsByKnowledge( createdOrFoundSpecificationItem, userSpecificationItem, generalizationWordItem, negativeSpecificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to correct invalid assumptions by knowledge about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								// Definition specification
								if( isEveryGeneralization )
									{
									if( negativeSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
									// Negative definition doesn't exist yet
									negativeSpecificationWordItem.firstSpecificationItem( true, false, false, generalizationWordItem ) == null &&
									// Draw negative definition conclusions
									drawNegativeDefinitionConclusions( isArchivedAssignment, generalizationWordTypeNr, negativeSpecificationWordTypeNr, generalizationWordItem, negativeSpecificationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative definition conclusions" );

									if( ( definitionSpecificationItem = negativeSpecificationWordItem.firstSpecificationItem( true, false, false, generalizationWordItem ) ) == null )
										definitionSpecificationItem = createdOrFoundSpecificationItem;

									// Get first generalization specification proper noun word
									if( ( currentGeneralizationItem = generalizationWordItem.firstProperNounSpecificationGeneralizationItem() ) != null )
										{
										// Do for all generalization specification proper noun words
										do	{
											if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
												return adminItem_.addError( 1, moduleNameString_, "I found an undefined generalization word" );

											if( ( positiveSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, negativeSpecificationWordItem ) ) != null &&

											( ( existingSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationWordItem ) ) == null ||

											// Justification doesn't exist yet in specification
											( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
											// Justification has at least the same assumption level
											existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, null, definitionSpecificationItem, null ) ) ) )
												{
												isProperNounArchivedAssignment = positiveSpecificationItem.isArchivedAssignment();

												// Draw negative conclusion
												if( adminItem_.addSelfGeneratedSpecification( false, isProperNounArchivedAssignment, isProperNounArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, generalizationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveSpecificationItem, null, definitionSpecificationItem, null, currentGeneralizationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										while( ( currentGeneralizationItem = currentGeneralizationItem.nextProperNounSpecificationGeneralizationItem() ) != null );
										}

									// Get first generalization specification noun word
									if( ( currentGeneralizationItem = generalizationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
										{
										// Do for all generalization specification noun words
										do	{
											if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
												return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

											if( ( positiveSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( false, false, false, negativeSpecificationWordItem ) ) != null &&
											!positiveSpecificationItem.isExclusiveSpecification() &&

											// Specification doesn't exist yet
											( ( existingSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, generalizationWordItem ) ) == null ||

											// Justification doesn't exist yet in specification
											( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, definitionSpecificationItem ) &&
											// Justification has at least the same assumption level
											existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, null, definitionSpecificationItem, null ) ) ) &&

											// Draw negative adjective conclusion
											adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveSpecificationItem, null, definitionSpecificationItem, null, currentGeneralizationWordItem, generalizationWordItem, null ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with specification word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( ( reversedNegativeSpecificationWordItem = negativeSpecificationWordItem.collectionWordItem( definitionSpecificationItem.specificationCollectionNr() ) ) != null &&
											( negativeSpecificationItem = currentGeneralizationWordItem.firstSpecificationItem( true, false, false, reversedNegativeSpecificationWordItem ) ) != null &&
											( positiveSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, reversedNegativeSpecificationWordItem ) ) != null &&

											// Specification doesn't exist yet
											( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentGeneralizationWordItem ) ) == null ||

											// Justification doesn't exist yet in specification
											( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( positiveSpecificationItem, negativeSpecificationItem) &&
											// Justification has at least the same assumption level
											existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, positiveSpecificationItem, null, negativeSpecificationItem, null ) ) ) &&

											// Draw negative adjective conclusion
											adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, positiveSpecificationItem, null, negativeSpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative adjective conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
											}
										while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
										}
									}
								else	// Proper noun specification
									{
									if( isUserRelationWord &&
									existingSpecificationItem != null &&
									( properNounSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, userSpecificationWordItem ) ) != null &&
									!properNounSpecificationItem.isHiddenSpanishSpecification() &&
									// Another reverse negative definition
									( definitionSpecificationItem = userSpecificationWordItem.firstSpecificationItem( true, false, false, negativeSpecificationWordItem ) ) != null &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( properNounSpecificationItem, definitionSpecificationItem ) &&
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, properNounSpecificationItem, null, definitionSpecificationItem, null ) &&
									// Draw another reverse simple negative conclusion
									adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, negativeSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, properNounSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, negativeSpecificationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw another reverse simple negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								}

							// Test file: "Knowledge of organisms (1)"
							if( isDrawingNegativeUserConclusion &&
							userSpecificationCollectionNr != compoundCollectionNr &&
							!primarySpecificationItem.hasAssumptionLevel() &&
							// Avoid idle call
							negativeSpecificationWordItem.firstExclusiveSpecificationItem( true ) != null &&
							// Draw negative exclusive conclusions
							drawNegativeExclusiveConclusions( isArchivedAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, negativeSpecificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative exclusive conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with negative specification word \"" + negativeSpecificationWordItem.anyWordTypeString() + "\"" );
							}

						if( isDrawingCompoundNegativeConclusions &&
						collectionWordItem != null &&
						// Positive specification doesn't exist
						generalizationWordItem.firstSpecificationItem( false, false, false, collectionWordItem ) == null &&
						( definitionSpecificationItem = userSpecificationWordItem.firstSpecificationItem( true, false, false, collectionWordItem ) ) != null &&

						// Specification doesn't exist yet
						( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, collectionWordItem ) ) == null ||

						// Justification doesn't exist yet in specification
						( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) &&
						// Justification has at least the same assumption level
						existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, definitionSpecificationItem, null ) ) ) &&

						// Draw compound negative conclusion
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isEveryGeneralization, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, userSpecificationItem, null, definitionSpecificationItem, null, generalizationWordItem, collectionWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw a compound negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( negativeDefinitionSpecificationItem = negativeDefinitionSpecificationItem.nextNegativeSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isArchivedAssignment, boolean isSelfGenerated, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		short assumptionLevel;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem = null;
		SpecificationItem foundSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem primaryGeneralizationWordItem;
		WordItem primarySpecificationWordItem;

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( isSelfGenerated ||
				currentGeneralizationWordItem != generalizationWordItem ) &&

				( currentSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem ) ) != null )
					{
					assumptionLevel = ( primarySpecificationItem.assumptionLevel() < currentSpecificationItem.assumptionLevel() ? currentSpecificationItem.assumptionLevel() :
																																		primarySpecificationItem.assumptionLevel() );

					// Collect generalization words of a specification-generalization definition
					if( adminItem_.collectGeneralizationWordWithPreviousOne( false, false, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

					currentSpecificationItem = currentSpecificationItem.updatedSpecificationItem();

					if( currentSpecificationItem.hasCurrentCreationSentenceNr() &&
					// Specification-generalization doesn't exist yet

					( ( foundSpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, currentGeneralizationWordItem ) ) == null ||
					foundSpecificationItem.isUserSpecification() ) &&

					// Draw a specification-generalization conclusion (using current specification)
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, true, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, currentSpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentSpecificationItem, null, null, null, specificationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the current specification" );

					if( !isSelfGenerated &&
					currentSpecificationItem.hasGeneralizationCollection() &&
					( primaryGeneralizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					// Specification-generalization doesn't exist yet
					primarySpecificationWordItem.firstSpecificationItem( false, false, false, primaryGeneralizationWordItem ) == null &&
					// Draw a specification-generalization conclusion (using primary specification)
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, true, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, primarySpecificationItem.generalizationWordTypeNr(), Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, null, null, primarySpecificationWordItem, primaryGeneralizationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated specification-generalization substitution specification, using the primary specification" );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isArchivedAssignment, boolean isExclusiveSpecification, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasCurrentlyCorrectedAssumptionByKnowledge;
		boolean hasCurrentSpecificationCollection;
		boolean hasCurrentSpecificationCompoundCollection;
		boolean hasGeneralizationWordActiveQuestionWithCompoundCollection;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean hasGeneralizationWordMasculineProperNounEnding;
		boolean hasPrimarySpecificationNewInformation;
		boolean hasPrimarySpecificationNonCompoundCollection;
		boolean hasPrimarySpecificationRelationContext;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasUserSpecificationCompoundCollection = false;
		boolean hasUserSpecificationNonCompoundCollection = false;
		boolean hasUserSpecificationRelationContext = false;
		boolean isAdjectiveCurrentSpecification;
		boolean isArchivedUserAssignment = false;
		boolean isCurrentUserSpecification;
		boolean isDefinitionSpanishAmbiguous;
		boolean isExclusiveCurrentSpecification;
		boolean isExclusiveUserSpecification = false;
		boolean isFeminineOrMasculineGeneralizationWord;
		boolean isFoundPrimaryHiddenSpanishSpecification;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isOlderCurrentSpecification;
		boolean isOlderUserSpecification = false;
		boolean isPossessiveUserSpecification = false;
		boolean isPossessivePrimarySpecification;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isSpanishNegativeConclusion = false;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserAssignment = false;
		boolean isUserAdjectiveSpecification = false;
		boolean isUserDefinitionSpecification = false;
		boolean isUserGeneralizationWord;
		boolean isUserQuestion = false;
		boolean isUserRelationWord;
		boolean isUserSpecificationWordSpanishAmbiguous = false;
		short currentSpecificationWordTypeNr;
		short selectedRelationWordTypeNr;
		int compoundSpecificationCollectionNr;
		int currentSpecificationCollectionNr;
		int nonCompoundSpecificationCollectionNr;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem tempJustificationItem;
		SpecificationItem alternativePrimarySpecificationItem;
		SpecificationItem alternativeSpanishAdditionalDefinitionSpecificationItem;
		SpecificationItem archivedAssignmentItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem feminineOrMasculineSpecificationItem;
		SpecificationItem foundPrimarySpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem questionSpecificationItem;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem collectionWordItem;
		WordItem currentSpecificationWordItem;
		WordItem oppositeSpecificationWordItem;
		WordItem selectedRelationWordItem;
		WordItem userSpecificationWordItem = null;
		CreateAndAssignResultType createAndAssignResult;

		if( !adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word type isn't a generalization reasoning word type" );

		if( !adminItem_.isSpecificationReasoningWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a specification reasoning type" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() ) != null )
			{
			hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem.hasCurrentlyCorrectedAssumptionByKnowledge();
			hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem.hasActiveQuestionWithCompoundCollection();
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
			hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem.hasMasculineProperNounEnding();
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem.isFeminineOrMasculineWord();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isUserRelationWord = generalizationWordItem.isUserRelationWord;

			isDefinitionSpanishAmbiguous = ( generalizationWordItem == specificationWordItem );

			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr();

			if( ( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
				{
				hasUserSpecificationCompoundCollection = userSpecificationItem.hasCompoundSpecificationCollection();
				hasUserSpecificationNonCompoundCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
				hasUserSpecificationRelationContext = userSpecificationItem.hasRelationContext();
				isArchivedUserAssignment = userSpecificationItem.isArchivedAssignment();
				isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();
				isOlderUserSpecification = userSpecificationItem.isOlderItem();
				isPossessiveUserSpecification = userSpecificationItem.isPossessive();
				isUserAssignment = userSpecificationItem.isAssignment();
				isUserAdjectiveSpecification = userSpecificationItem.isSpecificationAdjective();
				isUserDefinitionSpecification = userSpecificationItem.isGeneralizationNoun();
				isUserQuestion = userSpecificationItem.isQuestion();
				isUserSpecificationWordSpanishAmbiguous = userSpecificationItem.isSpecificationWordSpanishAmbiguous();
				userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
				userSpecificationWordItem = userSpecificationItem.specificationWordItem();
				}

			do	{
				// Select non-possessive definition
				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				currentSpecificationWordItem != generalizationWordItem )
					{
					hasCurrentSpecificationCompoundCollection = currentSpecificationItem.hasCompoundSpecificationCollection();
					isCurrentUserSpecification = currentSpecificationItem.isUserSpecification();
					isAdjectiveCurrentSpecification = currentSpecificationItem.isSpecificationAdjective();
					isExclusiveCurrentSpecification = currentSpecificationItem.isExclusiveSpecification();
					isOlderCurrentSpecification = currentSpecificationItem.isOlderItem();
					currentSpecificationWordTypeNr = currentSpecificationItem.specificationWordTypeNr();
					currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
					hasCurrentSpecificationCollection = ( currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR );

					if( !hasRelationWord ||
					!isPossessiveUserSpecification ||
					( foundPrimarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, false, specificationWordItem, relationWordItem ) ) == null )
						foundPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, isQuestion, specificationWordItem );

					if( foundPrimarySpecificationItem == null )
						{
						// Check for specification conflict
						if( !hasUserSpecificationRelationContext &&
						isCurrentUserSpecification &&
						currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
						!currentSpecificationItem.isSpecificationWordSpanishAmbiguous() &&
						// Check for specification conflict
						generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, false, false, isSpecificationWordSpanishAmbiguous, currentSpecificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.relationContextNr(), currentSpecificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to check for a specification conflict in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						hasPrimarySpecificationNewInformation = foundPrimarySpecificationItem.hasNewInformation();
						hasPrimarySpecificationNonCompoundCollection = foundPrimarySpecificationItem.hasNonCompoundSpecificationCollection();
						isFoundPrimaryHiddenSpanishSpecification = foundPrimarySpecificationItem.isHiddenSpanishSpecification();

						// Specification doesn't exist yet
						if( ( existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) == null )
							existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, isArchivedAssignment, false, isArchivedAssignment, false, false, isQuestion, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, currentSpecificationWordItem );

						if( existingSpecificationItem != null &&
						!isQuestion &&
						!existingSpecificationItem.isExclusiveSpecification() )
							isSpanishNegativeConclusion = true;

						if( isExclusiveCurrentSpecification &&
						!isFeminineOrMasculineGeneralizationWord &&
						userSpecificationItem != null &&
						userSpecificationWordItem != null &&

						// Matching feminine word
						( ( hasGeneralizationWordFeminineProperNounEnding &&
						currentSpecificationWordItem.isFeminineWord() ) ||

						// Matching masculine word
						( hasGeneralizationWordMasculineProperNounEnding &&
						currentSpecificationWordItem.isMasculineWord() ) ) &&

						( feminineOrMasculineSpecificationItem = currentSpecificationWordItem.firstFeminineOrMasculineSpecificationItem() ) != null &&
						// Select correct compound word to avoid assumption to be made if a question should be asked
						currentSpecificationWordItem.compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem )
							{
							selectedRelationWordItem = ( feminineOrMasculineSpecificationItem.isSpecificationAdjective() ? null : relationWordItem );

							if( existingSpecificationItem == null ||
							// Justification doesn't exist yet in specification
							!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( foundPrimarySpecificationItem, currentSpecificationItem ) ||

							( hasCurrentSpecificationCompoundCollection &&
							selectedRelationWordItem != null &&
							!selectedRelationWordItem.hasContextInWord( existingSpecificationItem.relationContextNr() ) ) )
								{
								selectedPrimarySpecificationItem = foundPrimarySpecificationItem;
								selectedAdditionalDefinitionSpecificationItem = null;

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
								foundPrimarySpecificationItem.hasRelationContext() ) ) ) ) ||

								// No non-compound specification collection
								( !hasPrimarySpecificationNonCompoundCollection &&
								!hasUserSpecificationRelationContext &&
								currentSpecificationWordItem.collectionNrByCompoundGeneralizationWordInWord( true, Constants.WORD_TYPE_NOUN_SINGULAR, userSpecificationWordItem ) == Constants.NO_COLLECTION_NR ) ) ) ||

								// Typical for Spanish
								( isFoundPrimaryHiddenSpanishSpecification &&
								// Justification doesn't exist yet in specification
								generalizationWordItem.foundJustificationItem( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem ) == null ) ) ) ||

								// Not user generalization word
								( !isUserGeneralizationWord &&
								// Non-compound specification collection
								hasPrimarySpecificationNonCompoundCollection &&
								!isFoundPrimaryHiddenSpanishSpecification &&
								!isSpecificationWordSpanishAmbiguous &&

								( existingSpecificationItem == null ||

								( isUserAssignment &&
								existingSpecificationItem.hasSpecificationCollection() &&
								!existingSpecificationItem.hasCurrentCreationSentenceNr() ) ||

								( existingSpecificationItem.hasSpecificationCollection() &&

								( isPossessiveUserSpecification ||
								// Justification doesn't exist yet in specification
								!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, currentSpecificationItem ) ) &&

								( ( hasRelationWord &&
								existingSpecificationItem.hasRelationContext() &&
								// Relation word is not covered by justification
								existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem ) == null ) ||

								// Justification has at least the same assumption level
								existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, null ) ) ) ) &&

								( !isPossessiveUserSpecification ||

								( ( existingSpecificationItem != null &&

								( !hasCurrentSpecificationCompoundCollection ||
								existingSpecificationItem.hasCurrentCreationSentenceNr() ) ) ||

								( selectedRelationWordItem != null &&

								( !selectedRelationWordItem.hasCurrentlyConfirmedSpecification() ||
								// Justification doesn't exist yet
								generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, currentSpecificationItem ) == null ) ) ) ) ) )
									{
									selectedRelationWordTypeNr = ( selectedRelationWordItem == null ? Constants.NO_WORD_TYPE_NR : Constants.WORD_TYPE_PROPER_NOUN );

									// Make a feminine or masculine proper noun ending assumption
									if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, foundPrimarySpecificationItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

									if( !isUserSpecificationWordSpanishAmbiguous &&
									createAndAssignResult.createdSpecificationItem != null &&
									// Draw a specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}

						if( userSpecificationItem != null &&

						( ( existingSpecificationItem == null &&

						( hasCurrentSpecificationCollection ||
						!foundPrimarySpecificationItem.hasSpecificationCollection() ) ) ||

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
								userSpecificationWordItem != null &&

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

								!foundPrimarySpecificationItem.isSelfGeneratedQuestion() ) ) &&

								( isAdjectiveCurrentSpecification ||
								isSpanishCurrentLanguage ||

								( isUserGeneralizationWord &&

								( ( hasGeneralizationWordCurrentlyConfirmedSpecification ||
								!hasUserSpecificationRelationContext ) &&

								// Negative specification doesn't exist
								userSpecificationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) == null &&

								// Question part doesn't exist yet
								( ( questionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, true, currentSpecificationWordItem ) ) == null ||

								// Question to be adjusted
								( questionSpecificationItem.isOlderItem() &&

								( hasGeneralizationWordCurrentlyConfirmedSpecification ||
								// Justification doesn't exist yet in question
								!questionSpecificationItem.hasPrimarySpecificationJustification( foundPrimarySpecificationItem ) ) ) ) ) ) ||

								( foundPrimarySpecificationItem.isSelfGeneratedSpecification() &&
								!foundPrimarySpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
								// Question part doesn't exist yet
								generalizationWordItem.firstSpecificationItem( false, false, true, currentSpecificationWordItem ) == null ) ) ) ||

								// Specification is Spanish ambiguous
								( isSpecificationWordSpanishAmbiguous &&

								( hasPrimarySpecificationNonCompoundCollection ||
								!isExclusiveCurrentSpecification ||
								isSpanishNegativeConclusion ||
								isUserGeneralizationWord ) &&

								( !foundPrimarySpecificationItem.isHiddenSpanishSpecification() ||

								( isUserRelationWord &&
								userSpecificationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) != null ) ) ) ) &&

								// Ask specification substitution question, or draw negative conclusion
								askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, ( currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE && specificationWordItem.isUserSpecificationWord ? userSpecificationItem : foundPrimarySpecificationItem ), currentSpecificationItem, userSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								// Make an assumption or draw a conclusion
								if( isGeneralizationProperNoun &&
								currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&

								( existingSpecificationItem != null ||
								!foundPrimarySpecificationItem.isCorrectedSpecification() ) )
									{
									alternativeSpanishAdditionalDefinitionSpecificationItem = null;
									primarySpecificationItem = null;

									if( isSpecificationWordSpanishAmbiguous )
										{
										if( currentSpecificationWordItem != specificationWordItem )
											primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem );
										}
									else
										{
										if( ( primarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, specificationWordItem, null ) ) == null ||
										primarySpecificationItem.isUserAssignment() )
											primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem );
										}

									if( primarySpecificationItem != null )
										{
										alternativePrimarySpecificationItem = ( !isSpecificationWordSpanishAmbiguous ?
																				( hasRelationWord ? userSpecificationItem :
																									foundPrimarySpecificationItem ) :
																									alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNounEnding, userSpecificationItem, generalizationWordItem ) );

										if( ( existingSpecificationItem != null &&
										alternativePrimarySpecificationItem != null &&
										alternativePrimarySpecificationItem.specificationWordItem() == specificationWordItem &&

										( ( !isSpecificationWordSpanishAmbiguous &&
										!existingSpecificationItem.hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) ) ||

										// Typical for Spanish
										( isSpecificationWordSpanishAmbiguous &&

										( existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) ||

										( isPossessiveUserSpecification &&
										primarySpecificationItem.hasRelationContext() &&
										generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) != null ) ) ) ) ) ||

										// Typical for Spanish
										( isSpecificationWordSpanishAmbiguous &&
										primarySpecificationItem.isHiddenSpanishSpecification() ) )
											{
											if( isSpecificationWordSpanishAmbiguous &&
											alternativePrimarySpecificationItem != null )
												alternativeSpanishAdditionalDefinitionSpecificationItem = alternativeSpanishAdditionalDefinitionSpecificationItem_;

											// Typical for Spanish
											if( isSpecificationWordSpanishAmbiguous &&
											!isArchivedUserAssignment &&
											isUserRelationWord &&
											alternativeSpanishAdditionalDefinitionSpecificationItem != null &&
											existingSpecificationItem != null )
												{
												if( ( primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, userSpecificationWordItem ) ) != null &&
												// Check if justification already exists
												existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) )
													{
													if( primarySpecificationItem == alternativePrimarySpecificationItem )
														{
														if( ( tempJustificationItem = foundPrimarySpecificationItem.firstSecondaryJustificationItem( true ) ) != null )
															{
															primarySpecificationItem = tempJustificationItem.secondarySpecificationItem();

															if( existingSpecificationItem.isOlderItem() &&
															// Justification already exists yet in specification. Try to find a non-compound specification
															existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) &&
															( nonCompoundSpecificationCollectionNr = alternativeSpanishAdditionalDefinitionSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
															( oppositeSpecificationWordItem = alternativeSpanishAdditionalDefinitionSpecificationItem.specificationWordItem() ) != null &&
															( tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, false, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, oppositeSpecificationWordItem ) ) != null &&
															!tempSpecificationItem.isHiddenSpanishSpecification() )
																primarySpecificationItem = tempSpecificationItem;
															}
														}
													else
														primarySpecificationItem = alternativePrimarySpecificationItem;
													}
												else
													{
													if( !existingSpecificationItem.hasPrimarySpecificationJustification( alternativePrimarySpecificationItem ) &&
													// Justification has at least the same assumption level
													existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, alternativePrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, currentSpecificationItem, null ) )
														primarySpecificationItem = alternativePrimarySpecificationItem;
													}
												}
											else
												{
												primarySpecificationItem = alternativePrimarySpecificationItem;

												if( existingSpecificationItem != null &&
												primarySpecificationItem != null )
													{
													if( isUserGeneralizationWord )
														{
														// Test file: "Laura - John and Anna - Joe"
														if( existingSpecificationItem.hasCurrentCreationSentenceNr() &&
														primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
														primarySpecificationItem.isSelfGeneratedAssumption() &&
														existingSpecificationItem.assumptionLevel() > primarySpecificationItem.assumptionLevel() &&
														// Recalculate assumption level of existing specification
														existingSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the existing specification" );
														}
													else
														{
														// Test files: "Complex (12)" and "Complex (15)"
														if( !existingSpecificationItem.hasCurrentCreationSentenceNr() &&
														primarySpecificationItem.isOlderItem() &&
														primarySpecificationItem.assumptionLevel() > existingSpecificationItem.assumptionLevel() &&
														// Recalculate assumption level of primary specification
														primarySpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the primary specification" );
														}
													}
												}
											}
										}

									hasPrimarySpecificationRelationContext = ( primarySpecificationItem == null ? false : primarySpecificationItem.hasRelationContext() );
									isPossessivePrimarySpecification = ( primarySpecificationItem == null ? false : primarySpecificationItem.isPossessive() );
									archivedAssignmentItem = null;

									if( isArchivedUserAssignment &&
									isUserDefinitionSpecification )
										{
										isArchivedAssignment = true;
										archivedAssignmentItem = userSpecificationItem;
										}

									selectedPrimarySpecificationItem = ( !isPossessiveUserSpecification ||

																		( primarySpecificationItem != null &&

																		( !primarySpecificationItem.isSelfGeneratedAssumption() ||
																		!primarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ? primarySpecificationItem :
																																				// Typical for Spanish
																																				userSpecificationItem );
									selectedSecondarySpecificationItem = ( archivedAssignmentItem == null ? currentSpecificationItem :
																											archivedAssignmentItem );

									if( primarySpecificationItem != null &&
									selectedPrimarySpecificationItem != null &&
									generalizationWordItem.foundJustificationItem( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem ) == null &&

									// No existing specification
									( ( existingSpecificationItem == null &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||

									// Typical for Spanish
									( foundPrimarySpecificationItem.isHiddenSpanishSpecification() &&
									!isCurrentUserSpecification ) ) &&

									( isUserQuestion ||
									isCurrentUserSpecification ||

									( ( !hasUserSpecificationRelationContext ||
									currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR ) &&

									( isArchivedAssignment ||
									isUserGeneralizationWord ||
									isUserRelationWord ||
									primarySpecificationItem.isOlderItem() ) ) ) ) ||

									// Existing specification
									( existingSpecificationItem != null &&
									existingSpecificationItem.isSelfGeneratedSpecification() &&
									!selectedPrimarySpecificationItem.isActiveAssignment() &&

									( ( hasPrimarySpecificationRelationContext &&
									isPossessivePrimarySpecification ) ||

									!existingSpecificationItem.hasRelationContext() ||

									( isUserRelationWord &&
									existingSpecificationItem.isOlderItem() ) ) &&

									( isPossessivePrimarySpecification ||

									// Typical for Spanish
									( ( ( isUserRelationWord &&
									primarySpecificationItem != selectedPrimarySpecificationItem ) ||

									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) &&

									( primarySpecificationItem == foundPrimarySpecificationItem ||
									!existingSpecificationItem.hasSpecificationCollection() ) &&

									( primarySpecificationItem.hasCurrentCreationSentenceNr() ||

									( !primarySpecificationItem.hasCompoundSpecificationCollection() &&

									( isUserAdjectiveSpecification ||
									existingSpecificationItem.hasCurrentCreationSentenceNr() ||

									( hasUserSpecificationRelationContext &&
									isUserRelationWord ) ) ) ) &&

									( !primarySpecificationItem.isCorrectedSpecification() ||
									!primarySpecificationItem.isSelfGeneratedAssumption() ) &&

									( hasPrimarySpecificationRelationContext ||
									!existingSpecificationItem.isArchivedAssignment() ) ) ) &&

									// Justification doesn't exist yet in specification
									!existingSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) &&

									// Skip uncertain justifications
									( ( !isPossessivePrimarySpecification &&
									compoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
									primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
									// Justification doesn't exist yet in specification
									!existingSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) ) ||

									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ) ) )
										{
										selectedRelationWordItem = ( existingSpecificationItem != null &&
																	existingSpecificationItem.hasRelationContext() &&
																	existingSpecificationItem.hasCurrentCreationSentenceNr() ? relationWordItem :

																	// Test file: "John - Anna (before family definition)"
																	( isExclusiveUserSpecification &&
																	!isSpanishCurrentLanguage &&
																	currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
																	selectedPrimarySpecificationItem.hasOnlyOneRelationWord() ? adminItem_.firstContextWordItem( selectedPrimarySpecificationItem.relationContextNr() ) : null ) );

										selectedRelationWordTypeNr = ( selectedRelationWordItem == null ? Constants.NO_WORD_TYPE_NR : Constants.WORD_TYPE_PROPER_NOUN );

										// Make a compound specification substitution assumption
										if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, alternativeSpanishAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, selectedRelationWordItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

										if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null )
											{
											// Test file: "Complex (3)"
											if( !hasRelationWord &&
											existingSpecificationItem != null &&
											!createdOrFoundSpecificationItem.isOlderItem() &&
											primarySpecificationItem.hasCompoundSpecificationCollection() &&
											primarySpecificationItem.isUserSpecification() &&
											// Remove obsolete assumption justifications from currently created or found specification
											createdOrFoundSpecificationItem.removeObsoleteAssumptionJustifications( false, false ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to remove obsolete assumption justifications from the currently created or found specification" );

											// Typical for Spanish
											// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
											if( isSpanishCurrentLanguage &&
											hasUserSpecificationNonCompoundCollection &&
											hasUserSpecificationRelationContext &&
											!isPossessiveUserSpecification &&
											!isUserSpecificationWordSpanishAmbiguous &&
											( selectedPrimarySpecificationItem = adminItem_.simpleUserSpecificationItem() ) != null &&
											( oppositeSpecificationWordItem = currentSpecificationWordItem.collectionWordItem( createdOrFoundSpecificationItem.specificationCollectionNr() ) ) != null &&
											( collectionWordItem = specificationWordItem.collectionWordItem( userSpecificationCollectionNr ) ) != null &&
											( selectedAdditionalDefinitionSpecificationItem = collectionWordItem.firstSpecificationItem( false, false, false, currentSpecificationWordItem ) ) != null &&
											// Draw negative Spanish assumption
											adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, true, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, createdOrFoundSpecificationItem, null, generalizationWordItem, oppositeSpecificationWordItem, null ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a negative Spanish assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oppositeSpecificationWordItem.anyWordTypeString() + "\"" );
											}
										}

									// Typical for Spanish
									if( isSpecificationWordSpanishAmbiguous &&
									isUserGeneralizationWord &&
									hasUserSpecificationCompoundCollection &&
									!isOlderUserSpecification &&
									primarySpecificationItem != null &&

									( isArchivedUserAssignment ||

									( currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
									existingSpecificationItem != null &&
									existingSpecificationItem.isOlderItem() ) ) &&

									// Draw a specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, null ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									if( ( primarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, specificationWordItem ) ) != null &&

									// Non-exclusive
									( ( !isExclusiveSpecification &&

									( !isSpecificationWordSpanishAmbiguous ||
									!isFoundPrimaryHiddenSpanishSpecification ) &&

									// Skip if already exists as user specification
									generalizationWordItem.firstUserSpecificationItem( false, false, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null &&

									// Skip if new user specification has relation context, while the old one did not
									// Test file: "I know (10)"
									( !isGeneralizationProperNoun ||
									!primarySpecificationItem.hasRelationContext() ||
									generalizationWordItem.noRelationContextSpecificationItem( false, false, specificationWordItem ) == null ) ) ||

									// Exclusive
									( isExclusiveSpecification &&

									( isGeneralizationProperNoun ||
									!hasCurrentSpecificationCompoundCollection ||
									currentSpecificationWordItem.isFeminineOrMasculineWord() ) &&

									// Exclusive conclusion doesn't exist
									generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, false, true, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, currentSpecificationWordItem ) == null ) ) )
										{
										createdSpecificationItem = null;
										createdOrFoundSpecificationItem = existingSpecificationItem;

										if( existingSpecificationItem == null ||

										// Justification doesn't exist yet in specification
										( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, currentSpecificationItem ) &&

										// Test file: "Complex (15)"
										( !primarySpecificationItem.isSelfGeneratedConclusion() ||
										generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, null ) == null ) &&

										// Justification has at least the same assumption level
										existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, currentSpecificationItem, null ) ) )
											{
											// Draw a compound specification substitution conclusion
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isEveryGeneralization(), false, isExclusiveSpecification, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

											if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
											createdOrFoundSpecificationItem.hasCurrentCreationSentenceNr() &&
											( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null &&
											// Remove obsolete assumption justifications from created or found specification
											createdOrFoundSpecificationItem.removeObsoleteAssumptionJustifications( false, false ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to remove obsolete assumption justifications from the older created or found specification" );
											}

										if( isGeneralizationProperNoun &&
										createdOrFoundSpecificationItem != null )
											{
											// Adjective
											if( currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE )
												{
												if( ( !hasUserSpecificationRelationContext ||
												createdOrFoundSpecificationItem.isOlderItem() ||
												( tempSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, primarySpecificationItem.specificationCollectionNr(), null ) ) == null ||
												!tempSpecificationItem.hasCompoundSpecificationCollection() ) &&

												// Draw negative adjective conclusions
												drawNegativeAdjectiveConclusions( isArchivedAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK)
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative adjective conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( !isUserAssignment &&
												userSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
												createdSpecificationItem != null &&

												( hasUserSpecificationNonCompoundCollection ||
												!hasUserSpecificationRelationContext ||
												isUserSpecificationWordSpanishAmbiguous ) &&

												// Avoid idle call
												generalizationWordItem.firstSpecificationItem( false, false, isArchivedUserAssignment, true ) != null &&
												// Make indirectly answered question assumption
												makeIndirectlyAnsweredQuestionAssumption( isArchivedUserAssignment, generalizationWordTypeNr, createdOrFoundSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}

											if( ( !isPossessiveUserSpecification ||
											primarySpecificationItem.isSelfGeneratedAssumption() ) &&

											primarySpecificationItem.assumptionLevel() <= createdOrFoundSpecificationItem.assumptionLevel() )
												{
												// Avoid creating duplicate negative justifications
												if( ( !hasUserSpecificationRelationContext ||
												( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, Constants.NO_QUESTION_PARAMETER, userSpecificationCollectionNr, specificationWordItem ) ) == null ||
												!tempSpecificationItem.isOlderItem() ) &&

												// Avoid idle call
												specificationWordItem.firstNegativeSpecificationItem() != null &&
												// Draw simple negative conclusions
												drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												{
												if( hasUserSpecificationCompoundCollection &&
												isPossessiveUserSpecification &&
												isUserRelationWord &&
												existingSpecificationItem != null &&
												userSpecificationWordItem != null )
													{
													definitionSpecificationItem = ( specificationWordItem == userSpecificationWordItem ? currentSpecificationItem :
																																		// Typical for Spanish
																																		// Spanish test file: "Complejo (3)"
																																		userSpecificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, currentSpecificationWordItem ) );
													// Test file: "Complex (17)"
													existingSpecificationItem = existingSpecificationItem.updatedSpecificationItem();

													// Justification doesn't exist yet in specification
													if( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( userSpecificationItem, definitionSpecificationItem ) &&
													// Justification has at least the same assumption level
													existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, definitionSpecificationItem, null ) &&
													// Add specification substitution justification to existing specification
													existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, definitionSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the existing specification" );
													}
												}

											if( hasCurrentlyCorrectedAssumptionByKnowledge &&
											createdSpecificationItem != null &&
											// Find obsolete specification (incorrect assumption that has been corrected)
											( obsoleteSpecificationItem = generalizationWordItem.firstSpecificationItem( true, false, false, currentSpecificationWordItem ) ) != null )
												{
												// Write invalid assumption by knowledge
												if( generalizationWordItem.writeUpdatedSpecification( false, true, false, false, false, false, false, obsoleteSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to write an invalid assumption by knowledge about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												// Replace incorrect assumption
												if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace a corrected specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
							currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
							existingSpecificationItem != null &&
							!existingSpecificationItem.isExclusiveSpecification() &&
							!existingSpecificationItem.isReplacedOrDeletedItem() &&
							existingSpecificationItem.isSelfGeneratedSpecification() &&
							!foundPrimarySpecificationItem.hasRelationContext() &&
							foundPrimarySpecificationItem.isSelfGeneratedSpecification() &&
							( definitionSpecificationItem = currentSpecificationWordItem.firstNonExclusiveSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
							// Justification doesn't exist yet in specification
							!foundPrimarySpecificationItem.hasPrimaryAndSecondarySpecificationJustification( existingSpecificationItem, definitionSpecificationItem ) &&
							// Add specification substitution justification to found primary specification
							foundPrimarySpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, existingSpecificationItem, null, definitionSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the found primary specification" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextNonNegativeNonPossessiveDefinitionSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUncountableGeneralizationNoun, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasExistingSpecificationCurrentCreationSentenceNr;
		boolean hasExistingSpecificationRelationContext;
		boolean hasFeminineOrMasculineProperNounEnding;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasGeneralizationWordCurrentlyMoreSpecificSpecification;
		boolean hasGeneralizationWordMasculineProperNounEnding;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasRelationWordConfirmedSpecification = false;
		boolean hasRelationWordFeminineOrMasculineProperNounEnding = false;
		boolean hasRelationWordMasculineProperNounEnding;
		boolean hasReplacedHiddenSpecification;
		boolean hasSecondarySpecificationRelationContext;
		boolean isExistingHiddenSpanishSpecification;
		boolean isHiddenPrimarySpecification;
		boolean isHiddenSecondarySpecification;
		boolean isOlderExistingSpecification;
		boolean isOlderPrimarySpecification;
		boolean isOppositePossessiveSpecificationWordSpanishAmbiguous;
		boolean isPossessiveSpecificationWordSpanishAmbiguous;
		boolean isPrimarySameAsSecondarySpecification;
		boolean isPrimarySpecificationAssignment;
		boolean isPrimaryUserSpecification;
		boolean isRelationWordUserWordGeneralization = false;
		boolean isSelectedSecondaryUserSpecification;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isUserRelationWord;
		short prepositionParameter;
		short selectedJustificationTypeNr;
		short selectedRelationWordTypeNr;
		int relationContextNr;
		int specificationCollectionNr;
		JustificationItem olderJustificationItem;
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem additionalSpecificationItem;
		SpecificationItem compoundSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentExistingSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundOppositePossessiveDefinitionSpecificationItem;
		SpecificationItem foundPossessiveDefinitionSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem possessiveSpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem = null;
		SpecificationItem selectedPrimarySpecificationItem;
		SpecificationItem selectedAdditionalDefinitionSpecificationItem;
		SpecificationItem selectedSecondarySpecificationItem;
		SpecificationItem selectedAdditionalProperNounSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem commonWordItem;
		WordItem currentPossessiveNounWordItem;
		WordItem feminineSpecificationWordItem;
		WordItem oppositePossessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveDefinitionSpecificationWordItem = null;
		WordItem possessiveSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		FindSpecificationResultType findSpecificationResult;

		oppositePossessiveDefinitionSpecificationItem_ = null;
		possessiveDefinitionSpecificationItem_ = null;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, relationWordItem );

		// Typical for Spanish
		if( isSpanishCurrentLanguage &&
		primarySpecificationItem != null &&
		relationWordItem != null &&
		specificationWordItem.isNounWordSpanishAmbiguous() &&
		primarySpecificationItem.hasCompoundSpecificationCollection() &&
		primarySpecificationItem.isSelfGeneratedSpecification() &&
		( specificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
		( tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, specificationCollectionNr, specificationWordItem ) ) != null &&
		( relationContextNr = tempSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR &&
		relationWordItem.hasContextInWord( relationContextNr ) )
			// Spanish non-compound specification
			primarySpecificationItem = tempSpecificationItem;

		if( primarySpecificationItem != null )
			{
			isOlderPrimarySpecification = primarySpecificationItem.isOlderItem();
			isPrimarySpecificationAssignment = primarySpecificationItem.isAssignment();
			isPrimaryUserSpecification = primarySpecificationItem.isUserSpecification();

			if( ( !isOlderPrimarySpecification ||
			isPrimarySpecificationAssignment ||

			( relationWordItem != null &&
			primarySpecificationItem.isSelfGeneratedSpecification() ) ) &&

			( currentPossessiveNounWordItem = GlobalVariables.firstPossessiveNounWordItem ) != null )
				{
				hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
				hasGeneralizationWordCurrentlyMoreSpecificSpecification = generalizationWordItem.hasCurrentlyMoreSpecificSpecification();
				hasGeneralizationWordMasculineProperNounEnding = generalizationWordItem.hasMasculineProperNounEnding();
				isUserRelationWord = generalizationWordItem.isUserRelationWord;

				isHiddenPrimarySpecification = primarySpecificationItem.isHiddenSpanishSpecification();

				if( relationWordItem != null )
					{
					hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();
					hasRelationWordFeminineOrMasculineProperNounEnding = relationWordItem.hasFeminineOrMasculineProperNounEnding();
					isRelationWordUserWordGeneralization = relationWordItem.isUserGeneralizationWord;
					}

				// Do for all possessive noun words
				do	{
					if( currentPossessiveNounWordItem != specificationWordItem &&
					currentPossessiveNounWordItem != generalizationWordItem )
						{
						// Find possessive specifications
						if( ( findSpecificationResult = findPossessiveSpecifications( currentPossessiveNounWordItem, specificationWordItem) ).result != Constants.RESULT_OK)
							return adminItem_.addError( 1, moduleNameString_, "I failed to find possessive specifications in word \"" + currentPossessiveNounWordItem.anyWordTypeString() + "\"" );

						hasReplacedHiddenSpecification = false;
						additionalDefinitionSpecificationItem = null;
						createdOrFoundSpecificationItem = null;
						foundOppositePossessiveDefinitionSpecificationItem = findSpecificationResult.foundOppositePossessiveDefinitionSpecificationItem;
						foundPossessiveDefinitionSpecificationItem = findSpecificationResult.foundPossessiveDefinitionSpecificationItem;

						if( foundPossessiveDefinitionSpecificationItem != null &&
						( possessiveDefinitionSpecificationWordItem = foundPossessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
							{
							oppositePossessiveDefinitionSpecificationItem_ = foundOppositePossessiveDefinitionSpecificationItem;
							possessiveDefinitionSpecificationItem_ = foundPossessiveDefinitionSpecificationItem;

							if( !isPossessive &&
							( possessiveSpecificationItem = possessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null )
								{
								isPossessiveSpecificationWordSpanishAmbiguous = possessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();

								if( primarySpecificationItem != null &&

								( ( !primarySpecificationItem.hasRelationContext() &&
								generalizationWordItem.hadOnceAnsweredSelfGeneratedQuestion() ) ||

								( isPossessiveSpecificationWordSpanishAmbiguous &&
								primarySpecificationItem.specificationWordItem() == possessiveDefinitionSpecificationWordItem ) ) )
									{
									// Draw Spanish specification substitution conclusion
									if( drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									if( ( existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) ) == null ||

									( hasRelationWord &&

									// Confirmed specification without relation word
									( !existingUserSpecificationItem.hasRelationContext() ||
									!existingUserSpecificationItem.hasSpecificationCollection() ) ) )
										{
										hasFeminineOrMasculineProperNounEnding = false;

										if( isPossessiveSpecificationWordSpanishAmbiguous &&
										isFeminineOrMasculineProperNounEnding( isPossessive, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) )
											hasFeminineOrMasculineProperNounEnding = true;

										if( !hasRelationWord &&
										( possessiveSpecificationWordItem = possessiveSpecificationItem.specificationWordItem() ) != null &&
										( commonWordItem = possessiveSpecificationWordItem.commonWordItem( possessiveSpecificationItem.specificationCollectionNr() ) ) != null &&
										commonWordItem != generalizationWordItem &&
										commonWordItem.isFeminineOrMasculineWord() &&
										!commonWordItem.isNounWordSpanishAmbiguous() &&
										( definitionSpecificationItem = commonWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, false, false, false, possessiveSpecificationWordItem ) ) != null &&
										// Make generalization assumption
										adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, definitionSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + commonWordItem.anyWordTypeString() + "\"" );

										// User specification doesn't exist yet
										if( generalizationWordItem.firstUserSpecificationItem( !isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationWordItem ) == null )
											{
											selectedSecondarySpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem );

											// Specification doesn't exist yet
											if( ( currentExistingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, isPossessive, false, possessiveDefinitionSpecificationWordItem ) ) == null ||

											// No relation context
											( ( relationWordItem == null &&
											// Test files: "I know (11)" and "Paul is a boy - Laura is a girl"
											possessiveSpecificationItem.isSelfGeneratedSpecification() ) ||

											// Relation context
											( relationWordItem != null &&

											// Spanish test file: "Complejo (8)"
											( isPossessiveSpecificationWordSpanishAmbiguous ||
											!relationWordItem.hasContextInWord( currentExistingSpecificationItem.relationContextNr() ) ||

											( currentExistingSpecificationItem.isOlderItem() &&
											!currentExistingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, selectedSecondarySpecificationItem ) &&

											( ( currentExistingSpecificationItem.isSelfGeneratedSpecification() &&
											currentExistingSpecificationItem.hasOnlyOneRelationWord() ) ||

											( isPrimaryUserSpecification &&
											currentExistingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) != null ) ) ) ) ) ) )
												{
												prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
												selectedRelationWordTypeNr = Constants.NO_WORD_TYPE_NR;
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

												if( hasRelationWord )
													{
													prepositionParameter = Constants.WORD_PARAMETER_PREPOSITION_OF;
													selectedRelationWordTypeNr = relationWordTypeNr;
													}

												if( selectedSecondarySpecificationItem != null )
													{
													// Make specification substitution assumption
													if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, prepositionParameter, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, selectedSecondarySpecificationItem, null, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to make a specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

													if( ( createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem ) != null &&
													// Justification doesn't exist yet in specification
													!createdOrFoundSpecificationItem.hasPrimarySpecificationJustification( userSpecificationItem ) &&
													// Add user specification substitution justification to created or found specification
													createdOrFoundSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, userSpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a user specification substitution justification to the created or found specification" );
													}
												else
													{
													// Typical for Spanish
													if( isSpanishCurrentLanguage )
														{
														if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, prepositionParameter, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, selectedRelationWordTypeNr, foundPossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, possessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to make a selected assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

														createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
														}
													}

												// Typical for Spanish
												if( hasRelationWord &&
												isPrimaryUserSpecification &&
												existingUserSpecificationItem == null &&
												createdOrFoundSpecificationItem != null &&
												createdOrFoundSpecificationItem.hasCurrentCreationSentenceNr() &&
												createdOrFoundSpecificationItem.isOlderItem() &&
												createdOrFoundSpecificationItem.isHiddenSpanishSpecification() &&
												!primarySpecificationItem.isActiveAssignment() &&
												// Add user specification to justifications of hidden Spanish specification
												createdOrFoundSpecificationItem.addUserSpecificationToSpanishJustifications( primarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to add the user specification to justifications of the hidden Spanish specification" );
												}
											else
												createdOrFoundSpecificationItem = currentExistingSpecificationItem;

											if( createdOrFoundSpecificationItem != null )
												{
												secondarySpecificationItem = createdOrFoundSpecificationItem.updatedSpecificationItem();

												// Typical for Spanish
												if( isPossessiveSpecificationWordSpanishAmbiguous )
													{
													hasReplacedHiddenSpecification = true;

													// Spanish test file: "Que haya confirmado al menos una palabra de relación"
													// Draw simple negative conclusions
													if( drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem, generalizationWordItem, primarySpecificationItem.specificationWordItem() ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
							primarySpecificationItem != null &&
							( foundSpecificationItem = currentPossessiveNounWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
							// Uncountable generalization noun. Example: "Water ..."
							adminItem_.addSelfGeneratedSpecification( false, isPrimarySpecificationAssignment, false, false, false, false, false, false, false, false, false, false, false, isUncountableGeneralizationNoun, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, currentPossessiveNounWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make a substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentPossessiveNounWordItem.anyWordTypeString() + "\"" );
							}

						// First opposite possessive specification
						if( primarySpecificationItem != null &&
						possessiveDefinitionSpecificationWordItem != null &&
						foundPossessiveDefinitionSpecificationItem != null &&
						foundOppositePossessiveDefinitionSpecificationItem != null &&
						( oppositePossessiveDefinitionSpecificationWordItem = foundOppositePossessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
							{
							if( createdOrFoundSpecificationItem == null ||
							primarySpecificationItem.hasSpecificationContext() )
								{
								secondarySpecificationItem = primarySpecificationItem;

								// Find additional definition definition
								if( !hasRelationWord ||
								isPossessive )
									additionalDefinitionSpecificationItem = possessiveDefinitionSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, specificationWordItem );
								}

							if( !isHiddenPrimarySpecification &&
							secondarySpecificationItem != null )
								{
								hasSecondarySpecificationRelationContext = secondarySpecificationItem.hasRelationContext();

								if( ( !hasSecondarySpecificationRelationContext ||
								!secondarySpecificationItem.isUserAssignment() ||
								secondarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) &&

								( !isOlderPrimarySpecification ||
								primarySpecificationItem.isSelfGeneratedSpecification() ) )
									{
									hasExistingSpecificationCurrentCreationSentenceNr = false;
									hasExistingSpecificationRelationContext = false;
									isExistingHiddenSpanishSpecification = false;
									isOlderExistingSpecification = false;
									isHiddenSecondarySpecification = secondarySpecificationItem.isHiddenSpanishSpecification();

									if( ( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, isArchivedAssignment, isNegative, !isPossessive, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
									// Typical for Spanish
									!isHiddenSecondarySpecification &&
									hasGeneralizationWordCurrentlyConfirmedSpecification &&
									existingSpecificationItem.hasCompoundSpecificationCollection() &&
									existingSpecificationItem.hasOnlyOneRelationWord() &&
									( tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, !isPossessive, false, foundOppositePossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != null )
										existingSpecificationItem = tempSpecificationItem;

									if( existingSpecificationItem != null )
										{
										hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem.hasCurrentCreationSentenceNr();
										hasExistingSpecificationRelationContext = existingSpecificationItem.hasRelationContext();
										isExistingHiddenSpanishSpecification = existingSpecificationItem.isHiddenSpanishSpecification();
										isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
										}

									isOppositePossessiveSpecificationWordSpanishAmbiguous = oppositePossessiveDefinitionSpecificationWordItem.isNounWordSpanishAmbiguous();
									isPrimarySameAsSecondarySpecification = ( primarySpecificationItem == secondarySpecificationItem );
									primarySpecificationItem = primarySpecificationItem.updatedSpecificationItem();
									selectedPrimarySpecificationItem = foundPossessiveDefinitionSpecificationItem;
									selectedAdditionalDefinitionSpecificationItem = ( !hasReplacedHiddenSpecification &&
																					isOppositePossessiveSpecificationWordSpanishAmbiguous ? null : additionalDefinitionSpecificationItem );

									selectedSecondarySpecificationItem = ( isHiddenSecondarySpecification ? primarySpecificationItem :
																											secondarySpecificationItem );

									isSelectedSecondaryUserSpecification = selectedSecondarySpecificationItem.isUserSpecification();

									if( existingSpecificationItem == null ||

									( ( relationWordItem == null ||

									// Justification doesn't exist yet in specification
									( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&

									( isSelectedSecondaryUserSpecification ||
									existingSpecificationItem.highestReversibleJustificationAssumptionLevel() > existingSpecificationItem.assumptionLevel() ||
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ) ||

									// Relation context doesn't exist yet
									( !relationWordItem.hasContextInWord( existingSpecificationItem.relationContextNr() ) &&

									( isOppositePossessiveSpecificationWordSpanishAmbiguous ||
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ) ) &&

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
									!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) ) &&

									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ||

									// Non-possessive
									( !isPossessive &&
									!isUserRelationWord &&

									( hasExistingSpecificationCurrentCreationSentenceNr ||
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ||

									( isPrimarySpecificationAssignment &&

									( !isHiddenSecondarySpecification ||

									secondarySpecificationItem.nRelationContextWords() < primarySpecificationItem.nRelationContextWords() ) ) ||

									( existingSpecificationItem.hasOnlyOneRelationWord() &&

									( secondarySpecificationItem.hasOnlyOneRelationWord() ||
									existingSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ) ) ) &&

									( ( hasRelationWord &&
									existingSpecificationItem.isSelfGeneratedAssumption() &&

									( hasRelationWordConfirmedSpecification ||
									!hasRelationWordFeminineOrMasculineProperNounEnding ||
									isPrimarySpecificationAssignment ||
									additionalDefinitionSpecificationItem != null ) ) ||

									( hasSecondarySpecificationRelationContext &&

									( isHiddenSecondarySpecification ||
									!secondarySpecificationItem.isSelfGeneratedConclusion() ) &&

									( !hasGeneralizationWordCurrentlyMoreSpecificSpecification ||
									!isHiddenSecondarySpecification ||
									// Typical for Spanish
									// Self-generated assignment
									isPrimarySpecificationAssignment ) &&

									( !isOlderExistingSpecification ||

									( hasExistingSpecificationRelationContext &&
									hasGeneralizationWordCurrentlyConfirmedSpecification ) ||

									( isPrimarySameAsSecondarySpecification &&
									additionalDefinitionSpecificationItem == null ) ) ) ) &&

									( selectedAdditionalDefinitionSpecificationItem == null ||
									// Justification has at least the same assumption level
									existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null ) ) ) )
										{
										selectedRelationWordTypeNr = ( hasRelationWord ? relationWordTypeNr : Constants.NO_WORD_TYPE_NR );

										createdSpecificationItem = existingSpecificationItem;
										foundSpecificationItem = existingSpecificationItem;

										if( !isOppositePossessiveSpecificationWordSpanishAmbiguous ||
										!hasRelationWord ||
										isPrimaryUserSpecification ||
										!isPossessive ||
										existingSpecificationItem == null ||
										!existingSpecificationItem.hasOnlyOneRelationWord() ||
										GlobalVariables.nUserGeneralizationWords > 1 ||
										!selectedSecondarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
											{
											hasRelationWordMasculineProperNounEnding = false;
											selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;

											if( hasRelationWord &&
											isPossessive &&
											generalizationWordItem.hasMasculineProperNounEnding() &&
											( tempSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null )
												{
												hasRelationWordMasculineProperNounEnding = true;
												selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
												selectedPrimarySpecificationItem = primarySpecificationItem;
												selectedAdditionalDefinitionSpecificationItem = foundPossessiveDefinitionSpecificationItem;
												selectedSecondarySpecificationItem = tempSpecificationItem;
												}

											// Make an opposite possessive specification assumption
											if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( hasRelationWordMasculineProperNounEnding, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, foundPossessiveDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

											createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
											foundSpecificationItem = createAndAssignResult.foundSpecificationItem;
											}

										if( relationWordItem != null )
											{
											if( isPossessive )
												{
												if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
													{
													if( primarySpecificationItem.isSelfGeneratedSpecification() &&
													( additionalDefinitionSpecificationItem = oppositePossessiveDefinitionSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, Constants.NO_CONTEXT_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != null )
														{
														hasFeminineOrMasculineProperNounEnding = false;

														if( isFeminineOrMasculineProperNounEnding( false, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ) )
															{
															hasFeminineOrMasculineProperNounEnding = true;
															feminineSpecificationWordItem = null;

															if( feminineSpecificationWordItem_ != null )
																feminineSpecificationWordItem = oppositePossessiveDefinitionSpecificationWordItem.feminineCollectionWordItem();

															if( !hasGeneralizationWordMasculineProperNounEnding &&
															feminineSpecificationWordItem != null &&

															( !isOlderPrimarySpecification ||
															// Specification doesn't exist yet
															( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, feminineSpecificationWordItem ) ) == null ||

															// Justification doesn't exist yet in specification
															( !existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, additionalDefinitionSpecificationItem ) &&
															// Justification has at least the same assumption level
															existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, null ) ) ) &&

															// Make exclusive Spanish feminine specification substitution assumption
															adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, foundOppositePossessiveDefinitionSpecificationItem, additionalDefinitionSpecificationItem, null, generalizationWordItem, feminineSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to make an exclusive Spanish feminine specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
															}

														if( ( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, false, true, false, additionalDefinitionSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
														compoundSpecificationItem != primarySpecificationItem )
															{
															if( hasFeminineOrMasculineProperNounEnding )
																{
																selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION;
																selectedPrimarySpecificationItem = compoundSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = foundOppositePossessiveDefinitionSpecificationItem;
																selectedSecondarySpecificationItem = additionalDefinitionSpecificationItem;
																}
															else
																{
																selectedJustificationTypeNr = Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION;
																selectedPrimarySpecificationItem = foundPossessiveDefinitionSpecificationItem;
																selectedAdditionalDefinitionSpecificationItem = null;
																selectedSecondarySpecificationItem = compoundSpecificationItem;
																}

															// Make a compound specification exclusive specification substitution assumption (opposite)
															if( adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectedJustificationTypeNr, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, selectedPrimarySpecificationItem, selectedAdditionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, null, generalizationWordItem, oppositePossessiveDefinitionSpecificationWordItem, relationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is Spanish ambiguous in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );

															if( feminineSpecificationWordItem_ != null &&
															adminItem_.addSelfGeneratedSpecification( true, false, false, false, false, false, false, isNegative, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, selectedRelationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( hasFeminineOrMasculineProperNounEnding ? compoundSpecificationItem : foundOppositePossessiveDefinitionSpecificationItem ), ( hasFeminineOrMasculineProperNounEnding ? foundOppositePossessiveDefinitionSpecificationItem : null ), ( hasFeminineOrMasculineProperNounEnding ? additionalDefinitionSpecificationItem : compoundSpecificationItem ), null, generalizationWordItem, feminineSpecificationWordItem_, relationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to make a feminine compound exclusive specification substitution assumption that is Spanish ambiguous in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + feminineSpecificationWordItem_.anyWordTypeString() + "\"" );
															}
														}
													}
												else
													{
													if( foundSpecificationItem == null )
														secondarySpecificationItem = createdSpecificationItem;
													else
														{
														if( additionalDefinitionSpecificationItem == null )
															{
															// Possessive
															if( ( additionalSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, isArchivedAssignment, false, true, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( selectedPrimarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
															// Add opposite possessive conclusion justification to additional specification
															additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, selectedPrimarySpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the additional specification" );
															}
														else
															{
															// Found, but not created
															if( ( additionalSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, true, true, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != null &&
															( selectedSecondarySpecificationItem = specificationWordItem.firstSpecificationItem( false, false, false, possessiveDefinitionSpecificationWordItem ) ) != null &&
															// Justification doesn't exist yet in specification
															!additionalSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, selectedSecondarySpecificationItem ) &&
															// Justification has at least the same assumption level
															additionalSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
															// Add specification substitution justification to additional specification
															additionalSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, primarySpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification substitution justification to the additional specification" );
															}
														}
													}
												}
											else	// Non-possessive
												{
												if( existingSpecificationItem != null )
													{
													if( createdSpecificationItem == null )
														{
														if( foundSpecificationItem != null &&
														foundSpecificationItem.isOlderItem() &&
														existingSpecificationItem.isReplacedItem() &&
														( tempSpecificationItem = relationWordItem.firstSpecificationItem( false, false, false, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
														tempSpecificationItem.isSelfGeneratedConclusion() )
															{
															existingSpecificationItem = existingSpecificationItem.updatedSpecificationItem();

															// Draw reversible conclusion
															if( adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, existingSpecificationItem, null, null, null, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
															}

														if( existingSpecificationItem.isReplacedItem() )
															{
															if( !primarySpecificationItem.isUserAssignment() &&
															( tempSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, false, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null &&
															tempSpecificationItem.isSelfGeneratedSpecification() &&
															// Add opposite possessive assumption justification to existing specification
															tempSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, null, primarySpecificationItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive assumption justification to the existing specification" );
															}
														else
															{
															if( hasExistingSpecificationCurrentCreationSentenceNr &&
															!isOlderExistingSpecification )
																{
																if( isPrimarySpecificationAssignment )
																	{
																	if( !isSelectedSecondaryUserSpecification &&
																	!selectedSecondarySpecificationItem.isOlderItem() )
																		{
																		// Test file: "John - Anna (before family definition)"
																		// Remove obsolete assumption justifications from existing specification
																		if( existingSpecificationItem.removeObsoleteAssumptionJustifications( false, false ) != Constants.RESULT_OK )
																			return adminItem_.addError( 1, moduleNameString_, "I failed to remove obsolete assumption justifications from the existing specification" );

																		if( ( additionalSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, false, possessiveDefinitionSpecificationWordItem, relationWordItem ) ) != null &&
																		// Justification doesn't exist yet in specification
																		!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( foundPossessiveDefinitionSpecificationItem, additionalSpecificationItem ) &&
																		// Add additional opposite possessive conclusion justification to existing specification
																		existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, null, additionalSpecificationItem ) != Constants.RESULT_OK )
																			return adminItem_.addError( 1, moduleNameString_, "I failed to add an additional opposite possessive conclusion justification to the existing specification" );
																		}
																	}
																else
																	{
																	if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
																	( selectedSecondarySpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, possessiveDefinitionSpecificationWordItem, generalizationWordItem ) ) != null &&

																	( !selectedSecondarySpecificationItem.isOlderItem() ||
																	selectedSecondarySpecificationItem.isUserSpecification() ) &&

																	// Justification doesn't exist yet in specification
																	!existingSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( foundPossessiveDefinitionSpecificationItem, selectedSecondarySpecificationItem ) &&
																	// Justification has at least the same assumption level
																	existingSpecificationItem.assumptionLevel() >= adminItem_.justificationAssumptionLevel( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, null, selectedSecondarySpecificationItem, null ) &&
																	// Add opposite possessive conclusion justification to existing specification
																	existingSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, foundPossessiveDefinitionSpecificationItem, null, selectedSecondarySpecificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addError( 1, moduleNameString_, "I failed to add an opposite possessive conclusion justification to the existing specification" );
																	}
																}
															}
														}
													else
														{
														if( hasExistingSpecificationRelationContext )
															{
															if( !existingSpecificationItem.isReplacedOrDeletedItem() )
																{
																if( isOppositePossessiveSpecificationWordSpanishAmbiguous )
																	{
																	// Replace existing specification by created one
																	// Created specification was hidden
																	if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing Spanish specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

																	// Typical for Spanish
																	if( ( olderJustificationItem = existingSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) != null &&
																	// Attach older justification to created specification
																	createdSpecificationItem.attachJustificationToSpecification( olderJustificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addError( 1, moduleNameString_, "I failed to attach an older justification item to the created assumption" );
																	}
																else
																	{
																	// Replace existing specification by created one
																	if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addError( 1, moduleNameString_, "I failed to replace the existing specification in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															}
														else
															{
															// Test file: "Complex (19 - mixed)"
															if( !isOppositePossessiveSpecificationWordSpanishAmbiguous &&
															( foundSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, Constants.NO_COLLECTION_NR, oppositePossessiveDefinitionSpecificationWordItem ) ) != null &&
															// Draw reversible conclusion
															adminItem_.addSelfGeneratedSpecification( false, false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NOUN, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NOUN, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, relationWordItem, oppositePossessiveDefinitionSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to draw a reversible conclusion about word \"" + relationWordItem.anyWordTypeString() + "\" with specification word \"" + oppositePossessiveDefinitionSpecificationWordItem.anyWordTypeString() + "\"" );
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
				while( ( currentPossessiveNounWordItem = currentPossessiveNounWordItem.nextPossessiveNounWordItem ) != null );

				// No conflict found
				if( !GlobalVariables.hasDisplayedWarning &&
				primarySpecificationItem != null &&
				secondarySpecificationItem != null &&
				relationWordItem != null )
					{
					// Draw reversible conclusions
					if( drawReversibleConclusions( userSpecificationItem, relationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + relationWordItem.anyWordTypeString() + "\"" );

					if( isSpanishCurrentLanguage &&
					primarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&
					primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
					( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, false, specificationWordItem.compoundCollectionNr(), primarySpecificationItem.relationContextNr(), specificationWordItem ) ) != null )
						primarySpecificationItem = compoundSpecificationItem;

					selectedAdditionalProperNounSpecificationItem = ( isHiddenPrimarySpecification ? userSpecificationItem :
																									primarySpecificationItem );

					if( ( ( isRelationWordUserWordGeneralization ||
					primarySpecificationItem == userSpecificationItem ) &&

					( isSpanishCurrentLanguage ||
					!secondarySpecificationItem.hasCompoundSpecificationCollection() ) ) &&

					// Make an opposite possessive specification assumption (based on secondary specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, selectedAdditionalProperNounSpecificationItem, userSpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on secondary specification) during the making an exclusive specification substitution assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

					if( existingSpecificationItem != null &&
					primarySpecificationItem == secondarySpecificationItem &&
					primarySpecificationItem.hasCompoundSpecificationCollection() &&

					( ( !isSpanishCurrentLanguage &&
					isPossessive &&

					( !isRelationWordUserWordGeneralization ||
					!existingSpecificationItem.hasCurrentCreationSentenceNr() ) &&

					( isUserRelationWord ||
					existingSpecificationItem.isSelfGeneratedAssumption() ) ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&
					!isPossessive &&
					!existingSpecificationItem.isOlderItem() &&
					existingSpecificationItem.isHiddenSpanishSpecification() ) ) &&

					// Make an opposite possessive specification assumption (based on existing specification)
					makeOppositePossessiveSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, existingSpecificationItem, ( isHiddenPrimarySpecification ? userSpecificationItem : primarySpecificationItem ), userSpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to make an opposite possessive specification assumption (based on existing specification) during the making an exclusive specification substitution assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, SpecificationItem adjectiveSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		short specificationWordTypeNr;
		int currentSpecificationCollectionNr;
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem negativeDefinitionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem primarySpecificationWordItem;
		WordItem relatedSpecificationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		RelatedResultType relatedResult;

		if( adjectiveSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given adjective specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( !adjectiveSpecificationItem.isSpecificationAdjective() )
			return adminItem_.startError( 1, moduleNameString_, "The given adjective specification has no adjective specification" );

		if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, isArchivedAssignment, true ) ) != null )
			{
			do	{
				if( !currentQuestionSpecificationItem.hasCurrentCreationSentenceNr() &&
				( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				currentQuestionSpecificationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) != null )
					{
					specificationWordTypeNr = currentQuestionSpecificationItem.specificationWordTypeNr();
					currentSpecificationCollectionNr = currentQuestionSpecificationItem.specificationCollectionNr();
					createdOrFoundSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem );

					// Search first for related specification, before add a new one
					if( ( relatedResult = generalizationWordItem.findRelatedSpecification( true, isArchivedAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), specificationWordTypeNr, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a related specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is Spanish ambiguous" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
					relatedSpecificationItem != currentQuestionSpecificationItem &&
					( relatedSpecificationWordItem = relatedSpecificationItem.specificationWordItem() ) != null &&
					( primarySpecificationItem = generalizationWordItem.firstOlderNonPossessiveNonQuestionSpecificationItem( isArchivedAssignment ) ) != null &&
					( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
					( additionalDefinitionSpecificationItem = primarySpecificationWordItem.firstSpecificationItem( false, false, false, currentQuestionSpecificationWordItem ) ) != null &&
					( secondarySpecificationItem = relatedSpecificationWordItem.firstAdjectiveSpecificationItem( false ) ) != null )
						{
						// Typical for Spanish
						if( primarySpecificationItem.isSpecificationWordSpanishAmbiguous() )
							{
							// Draw Spanish specification substitution conclusion or ask a question
							if( drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, currentQuestionSpecificationWordItem, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to draw a Spanish specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							// Draw an indirectly answered question conclusion
							if( ( createAndAssignResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, adjectiveSpecificationItem, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );

							createdOrFoundSpecificationItem = createAndAssignResult.createdOrFoundSpecificationItem;
							}
						}

					if( createdOrFoundSpecificationItem != null &&
					currentQuestionSpecificationItem.isSpecificationWordSpanishAmbiguous() )
						{
						// Spanish test file: "Preguntas respondidas indirectamente - Juan es masculino"
						if( ( negativeDefinitionSpecificationItem = currentQuestionSpecificationWordItem.firstNegativeSpecificationItem() ) != null &&
						!negativeDefinitionSpecificationItem.hasSpecificationCollection() &&
						// Draw simple Spanish negative conclusions
						drawSimpleNegativeConclusions( isArchivedAssignment, adjectiveSpecificationItem, generalizationWordItem, currentQuestionSpecificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative Spanish conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						// Draw Spanish proper noun part-of conclusions in proper noun words
						if( drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, currentQuestionSpecificationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );

						// Find answers to questions
						// Missed a question question to answer
						if( generalizationWordItem.findAnswersToQuestions( currentSpecificationCollectionNr, createdOrFoundSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find answers to questions in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentQuestionSpecificationItem = ( currentQuestionSpecificationItem.isReplacedOrDeletedItem() ?
														// First question specification
														adminItem_.firstSpecificationItem( false, false, isArchivedAssignment, true ) :
														// Next question specification
														currentQuestionSpecificationItem.nextSelectedSpecificationItem() ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		short nOptionsLeft = 0;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem additionalDefinitionSpecificationItem;
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

		if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != null )
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
			// Get first generalization specification noun word
			( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
				{
				// Do for all generalization specification noun words
				do	{
					if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
						return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

					if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, currentGeneralizationWordItem ) ) != null &&
					( additionalDefinitionSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, false, onlyOptionLeftSpecificationWordItem ) ) != null &&
					// Make an only option left assumption
					adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, additionalDefinitionSpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw an only option left conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasGeneralizationWordFeminineProperNounEnding;
		boolean isChineseCurrentLanguage;
		boolean isFeminineOrMasculineGeneralizationWord;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem feminineCollectionWordItem;

		if( !adminItem_.isNounWordType( specificationWordTypeNr ) )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word type isn't a noun" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem == specificationWordItem )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item and specification word item are the same" );

		// Get first noun word specification generalization
		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			hasGeneralizationWordFeminineProperNounEnding = generalizationWordItem.hasFeminineProperNounEnding();
			isChineseCurrentLanguage = adminItem_.isChineseCurrentLanguage();
			isFeminineOrMasculineGeneralizationWord = generalizationWordItem.isFeminineOrMasculineWord();
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			if( userSpecificationItem == null )
				return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != generalizationWordItem &&
				// Get primary specification for justification
				( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null )
					{
					if( ( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||
					!currentGeneralizationWordItem.hasCollection() ||

					// Typical for Chinese
					( isChineseCurrentLanguage &&
					generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
					primarySpecificationItem.hasCompoundSpecificationCollection() &&

					( ( foundSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) == null ||
					foundSpecificationItem.hasSpecificationCollection() ) ) ) &&

					// Specification doesn't exist yet
					( ( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, currentGeneralizationWordItem ) ) == null ||
					// Skip if concluded conclusion already exists
					existingSpecificationItem != userSpecificationItem ) )
						{
						alternativeSpanishAdditionalDefinitionSpecificationItem_ = null;
						secondarySpecificationItem = null;

						// Get secondary specification for justification
						if( isUserGeneralizationWord &&
						!userSpecificationItem.isPossessive() &&

						( ( isSpecificationWordSpanishAmbiguous &&
						userSpecificationItem.specificationWordItem() == primarySpecificationItem.specificationWordItem() ) ||

						userSpecificationItem.specificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() ) )
							secondarySpecificationItem = userSpecificationItem;
						else
							{
							if( isSpecificationWordSpanishAmbiguous &&
							hasGeneralizationWordFeminineProperNounEnding &&
							( feminineCollectionWordItem = specificationWordItem.feminineCollectionWordItem() ) != null )
								secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, feminineCollectionWordItem );

							if( secondarySpecificationItem == null &&
							( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, false, specificationWordItem ) ) != null &&

							( existingSpecificationItem == null ||
							foundSpecificationItem.isSelfGeneratedConclusion() ) )
								secondarySpecificationItem = foundSpecificationItem;
							}

						if( !isGeneralizationProperNoun &&
						existingSpecificationItem == null &&
						secondarySpecificationItem != null &&
						primarySpecificationItem.isExclusiveSpecification() &&

						( !isFeminineOrMasculineGeneralizationWord ||
						secondarySpecificationItem.hasNonCompoundSpecificationCollection() ) &&

						// Skip if user specification already exists
						generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, currentGeneralizationWordItem ) == null &&
						// Skip if justification with primary specification already exists
						generalizationWordItem.primaryAndSecondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, primarySpecificationItem, secondarySpecificationItem ) == null &&
						// Make generalization assumption
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, !isGeneralizationProperNoun, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		RelatedResultType relatedResult = new RelatedResultType();

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		// Check if a related specification exists
		( relatedResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );

		if( relatedResult.relatedSpecificationItem == null )
			{
			if( generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() )
				{
				if( correctSuggestiveAssumptionsByOppositeSuggestiveQuestion( isArchivedAssignment, isNegative, isPossessive, primarySpecificationItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite suggestive question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			else
				{
				// Typical for Spanish:
				// Skip making suggestive assumption if specification word is Spanish ambiguous,
				// and if user question has more than one generalization words
				if( !primarySpecificationItem.isSpecificationWordSpanishAmbiguous() ||
				GlobalVariables.nUserGeneralizationWords == 1 )
					{
					hasSuggestiveQuestionAssumption_ = true;

					// Make a suggestive question assumption
					if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, false, isNegative, false, isPossessive, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a suggestive question assumption to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected CompoundResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, SpecificationItem userSpecificationItem, WordItem specificationWordItem )
		{
		boolean hasNonCompoundUserSpecificationCollection;
		boolean hasUserSpecificationCollection;
		boolean isArchivedAssignment;
		boolean isExclusiveSpecification;
		boolean isExclusiveUserSpecification;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		short generalizationWordTypeNr;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem secondaryDefinitionSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem relatedSpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;
		CompoundResultType compoundResult = new CompoundResultType();
		RelatedResultType relatedResult;

		if( userSpecificationItem == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startCompoundResultError( 1, moduleNameString_, "The given user specification item has no specification word" );

		if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
			{
			hasNonCompoundUserSpecificationCollection = userSpecificationItem.hasNonCompoundSpecificationCollection();
			hasUserSpecificationCollection = userSpecificationItem.hasSpecificationCollection();
			isExclusiveUserSpecification = userSpecificationItem.isExclusiveSpecification();

			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startCompoundResultError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( foundSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
					{
					if( foundSpecificationItem.isPartOf() )
						{
						if( compoundResult.compoundGeneralizationWordItem == null )
							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
						}
					else
						{
						isArchivedAssignment = foundSpecificationItem.isArchivedAssignment();
						isExclusiveSpecification = foundSpecificationItem.isExclusiveSpecification();
						generalizationWordTypeNr = foundSpecificationItem.generalizationWordTypeNr();

						// Definition
						if( isExclusiveSpecification &&
						generalizationWordTypeNr != Constants.WORD_TYPE_PROPER_NOUN &&
						( secondaryDefinitionSpecificationItem = specificationWordItem.firstNonCollectedSpecificationItem( true, currentGeneralizationWordItem ) ) != null &&
						( secondarySpecificationWordItem = secondaryDefinitionSpecificationItem.specificationWordItem() ) != null &&

						( !isSpanishCurrentLanguage ||
						// Typical for Spanish
						foundSpecificationItem.hasCompoundSpecificationCollection() ) &&

						// Make a specification substitution definition assumption
						adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, foundSpecificationItem.isEveryGeneralization(), false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundSpecificationItem, null, secondaryDefinitionSpecificationItem, null, currentGeneralizationWordItem, secondarySpecificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to make a specification substitution definition assumption about word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

						if( ( hasUserSpecificationCollection ||
						isExclusiveSpecification ) &&

						// Avoid idle call
						specificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null &&
						// Draw a compound specification substitution conclusion
						drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, foundSpecificationItem, null, userSpecificationItem, currentGeneralizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to draw a compound specification substitution conclusion about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( ( ( ( !isExclusiveSpecification ||

						( isSpanishCurrentLanguage &&
						// Avoid idle call
						specificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null ) ) &&

						foundSpecificationItem.isOlderItem() ) ||

						// Typical for Spanish
						// Test file: "Scientific challenge"
						( isExclusiveSpecification &&
						generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN ) ||

						// Test file: "John has 2 sons and a daughter (before family definition)"
						( isExclusiveSpecification &&
						isExclusiveUserSpecification &&
						foundSpecificationItem.hasNonCompoundSpecificationCollection() &&
						userSpecificationItem.hasCompoundSpecificationCollection() ) ) &&

						// Draw a specification substitution conclusion or ask a question
						drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentGeneralizationWordItem, specificationWordItem, null ) != Constants.RESULT_OK )
							return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

						if( isExclusiveSpecification &&
						foundSpecificationItem.isOlderItem() )
							{
							if( hasNonCompoundUserSpecificationCollection &&
							foundSpecificationItem.hasNonCompoundSpecificationCollection() )
								{
								if( ( relatedResult = currentGeneralizationWordItem.findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to find out if a specification in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );

								if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
								( relatedSpecificationWordItem = relatedSpecificationItem.specificationWordItem() ) != null )
									{
									// Avoid idle call
									if( relatedSpecificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null &&
									// Draw a specification substitution conclusion or ask a question
									drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentGeneralizationWordItem, relatedSpecificationWordItem, null ) != Constants.RESULT_OK )
										return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to draw a related specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

									// Make a related specification substitution 'part of' assumption
									if( makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, relatedSpecificationWordItem, userSpecificationWordItem.collectionWordItem( userSpecificationItem.specificationCollectionNr() ) ) != Constants.RESULT_OK )
										return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"" + relatedSpecificationWordItem.anyWordTypeString() + "\"" );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									// Make a Spanish specification substitution 'part of' assumption
									makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem, specificationWordItem, userSpecificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCompoundResultError( 1, moduleNameString_, "I failed to make a Spanish specification substitution 'part of' assumption in specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}

							compoundResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
							}
						}
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return compoundResult;
		}
	}

/*************************************************************************
 *	"In my desperation I prayed, and the Lord listened;
 *	he saved me from all my troubles." (Psalm 34:6)
 *************************************************************************/
