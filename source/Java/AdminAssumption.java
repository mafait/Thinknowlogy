/*
 *	Class:			AdminAssumption
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously make grammar-based assumptions
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

class AdminAssumption
	{
	// Private constructible variables

	private boolean hasFoundSuggestiveQuestionAssumption_;

	private SpecificationItem foundPossessiveDefinitionSpecificationItem_;
	private SpecificationItem foundOppositePossessiveDefinitionSpecificationItem_;
	private SpecificationItem lastFoundOppositePossessiveDefinitionSpecificationItem_;
	private SpecificationItem possessiveDefinitionSpecificationItem_;
	private SpecificationItem secondarySpecificationItem_;

	private WordItem generalizationAssumptionWordItem_;
	private WordItem oppositePossessiveSpecificationWordItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static boolean isFeminineOrMasculineProperNameEnding( boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isFemaleSpecificationWord;
		boolean isMaleSpecificationWord;

		if( generalizationWordItem != null &&
		specificationWordItem != null )
			{
			isFemaleSpecificationWord = specificationWordItem.isFemale();
			isMaleSpecificationWord = specificationWordItem.isMale();

			if( isFemaleSpecificationWord ||
			isMaleSpecificationWord )
				{
				if( specificationWordItem.isNounWordCollectedWithItself() )
					{
					if( ( isPossessive &&
					relationWordItem != null &&
					relationWordItem.hasFeminineProperNameEnding() ) ||

					( !isPossessive &&
					generalizationWordItem.hasFeminineProperNameEnding() ) )
						return true;
					}
				else
					{
					// Possessive (relation)
					if( ( isPossessive &&
					relationWordItem != null &&

					( ( isFemaleSpecificationWord &&
					relationWordItem.hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					relationWordItem.hasMasculineProperNameEnding() ) ) ) ||

					// Non-possessive (generalization)
					( !isPossessive &&

					( ( isFemaleSpecificationWord &&
					generalizationWordItem.hasFeminineProperNameEnding() ) ||

					( isMaleSpecificationWord &&
					generalizationWordItem.hasMasculineProperNameEnding() ) ) ) )
						return true;
					}
				}
			}

		return false;
		}

	private byte correctSuggestiveAssumptionsByOppositeQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem correctedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem createdNewSpecificationItem;
		SpecificationItem createdOldSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem oldSuggestiveQuestionAssumptionAssignmentItem;
		WordItem oldSuggestiveSpecificationWordItem;
		WordItem secondarySpecificationWordItem;

		if( secondarySpecificationItem != null )
			{
			if( generalizationWordItem != null )
				{
				if( specificationWordItem != null )
					{
					// Previously, a suggestive question assumption was made.
					// However, now a question with the opposite specification is asked.
					// Instead of assuming this new specification, a question is generated to ask which of both specifications is true.
					if( ( oldSuggestiveQuestionAssumptionAssignmentItem = generalizationWordItem.oldSuggestiveQuestionAssumptionSecondarySpecificationItem() ) != null )
						{
						if( oldSuggestiveQuestionAssumptionAssignmentItem.isActiveAssignment() )
							{
							if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) != null )
								{
								if( secondarySpecificationItem.isActiveAssignment() )
									{
									// Correct old question
									if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, oldSuggestiveQuestionAssumptionAssignmentItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
										{
										if( ( createdOldSpecificationItem = specificationResult.createdSpecificationItem ) != null )
											{
											// Correct new question
											if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, secondarySpecificationItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result == Constants.RESULT_OK )
												{
												if( ( createdNewSpecificationItem = specificationResult.createdSpecificationItem ) != null )
													{
													if( ( correctedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem.correctedAssumptionByOppositeQuestionJustificationItem() ) != null )
														{
														// Change primary user specification by primary corrected question specification
														if( correctedAssumptionByOppositeQuestionJustificationItem.changePrimarySpecification( createdNewSpecificationItem ) == Constants.RESULT_OK )
															{
															if( ( secondarySpecificationWordItem = correctedAssumptionByOppositeQuestionJustificationItem.secondarySpecificationWordItem() ) != null )
																{
																// Correct generalization assumption
																if( ( primarySpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != null )
																	{
																	if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdOldSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
																		{
																		if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
																			{
																			if( foundSpecificationItem.attachJustificationToSpecification( correctedAssumptionByOppositeQuestionJustificationItem ) != Constants.RESULT_OK )
																				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );
																			}
																		else
																			return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the corrected generalization assumption" );
																		}
																	else
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a specification substitution assumption" );
																	}
																else
																	return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );
																}
															else
																return adminItem_.startErrorInItem( 1, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );
															}
														else
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to change the primary specification of the corrected assumption by opposite question justification item" );
														}
													else
														return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the old corrected assumption by opposite question justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't create the new suggestive question assumption" );
												}
											else
												return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
											}
										else
											return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't create the old suggestive question assumption" );
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								else
									return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is no assignment" );
								}
							else
								return adminItem_.startErrorInItem( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte findPossessiveSpecifications( WordItem generalizationWordItem, WordItem definitionWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundPossessiveSpecificationWord = true;
		int possessiveSpecificationCollectionNr;
		SpecificationItem possessiveDefinitionSpecificationItem;
		SpecificationItem previousPossessiveSpecificationItem = null;
		WordItem possessiveSpecificationWordItem;
		WordItem previousPossessiveSpecificationWordItem;
		WordItem higherLevelPossessiveSpecificationWordItem = null;

		foundPossessiveDefinitionSpecificationItem_ = null;
		foundOppositePossessiveDefinitionSpecificationItem_ = null;

		if( generalizationWordItem != null )
			{
			if( definitionWordItem != null )
				{
				if( specificationWordItem != null )
					{
					// First try to find a possessive specification on a higher level
					if( ( possessiveDefinitionSpecificationItem = definitionWordItem.firstNonQuestionSpecificationItem() ) != null )
						{
						do	{
							if( !possessiveDefinitionSpecificationItem.isExclusiveSpecification() &&
							possessiveDefinitionSpecificationItem.isPossessive() &&
							possessiveDefinitionSpecificationItem.isSpecificationNoun() &&
							( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
								{
								if( possessiveSpecificationWordItem != higherLevelPossessiveSpecificationWordItem &&
								possessiveSpecificationWordItem.firstSpecificationItem( false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) != null )
									{
									if( higherLevelPossessiveSpecificationWordItem == null )
										higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
									else
										hasFoundPossessiveSpecificationWord = false;
									}
								}
							}
						while( ( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextSelectedSpecificationItem() ) != null );

						if( hasFoundPossessiveSpecificationWord &&
						( possessiveDefinitionSpecificationItem = definitionWordItem.firstNonQuestionSpecificationItem() ) != null )
							{
							if( possessiveDefinitionSpecificationItem.hasSpecificationCollection() )
								{
								if( higherLevelPossessiveSpecificationWordItem == null )
									higherLevelPossessiveSpecificationWordItem = specificationWordItem;

								// Try to find the opposite of the possessive specification
								do	{
									if( !possessiveDefinitionSpecificationItem.isExclusiveSpecification() &&
									!possessiveDefinitionSpecificationItem.isPartOf() &&
									possessiveDefinitionSpecificationItem.isPossessive() &&
									( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem.specificationWordItem() ) != null )
										{
										if( possessiveSpecificationWordItem != generalizationWordItem )
											{
											if( possessiveSpecificationWordItem.isSingularNoun() &&
											previousPossessiveSpecificationItem != null &&
											( previousPossessiveSpecificationWordItem = previousPossessiveSpecificationItem.specificationWordItem() ) != null )
												{
												possessiveSpecificationCollectionNr = possessiveDefinitionSpecificationItem.specificationCollectionNr();

												if( previousPossessiveSpecificationWordItem != possessiveSpecificationWordItem &&
												higherLevelPossessiveSpecificationWordItem.hasCollectionNr( possessiveSpecificationCollectionNr ) &&
												previousPossessiveSpecificationItem.specificationCollectionNr() == possessiveSpecificationCollectionNr )
													{
													foundOppositePossessiveDefinitionSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveDefinitionSpecificationItem );
													lastFoundOppositePossessiveDefinitionSpecificationItem_ = foundOppositePossessiveDefinitionSpecificationItem_;
													}

												foundPossessiveDefinitionSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveDefinitionSpecificationItem : previousPossessiveSpecificationItem );
												}

											previousPossessiveSpecificationItem = possessiveDefinitionSpecificationItem;
											}
										else
											return adminItem_.startErrorInItem( 1, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );
										}
									}
								while( ( possessiveDefinitionSpecificationItem = possessiveDefinitionSpecificationItem.nextSelectedSpecificationItem() ) != null );
								}
							}
						}
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given definition word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte makeGeneralizationAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem olderGeneralizationAssumptionBySpecificationJustificationItem;
		SpecificationItem olderGeneralizationAssumptionBySpecificationItem;

		if( generalizationWordItem != null )
			{
			// Skip if already exists
			if( generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) == null )
				{
				// Make a generalization assumption
				// Not yet explained in the design
				if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result == Constants.RESULT_OK )
					{
					if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() &&
					specificationResult.createdJustificationItem != null &&
					( olderGeneralizationAssumptionBySpecificationItem = specificationResult.foundSpecificationItem ) != null )
						{
						if( ( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ) ) != null )
							{
							if( generalizationWordItem.replaceJustification( olderGeneralizationAssumptionBySpecificationJustificationItem, specificationResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != Constants.RESULT_OK )
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item" );
							}
						}
					}
				else
					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminAssumption( AdminItem adminItem )
		{
		String errorString = null;

		hasFoundSuggestiveQuestionAssumption_ = false;

		foundPossessiveDefinitionSpecificationItem_ = null;
		foundOppositePossessiveDefinitionSpecificationItem_ = null;
		lastFoundOppositePossessiveDefinitionSpecificationItem_ = null;
		possessiveDefinitionSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

		generalizationAssumptionWordItem_ = null;
		oppositePossessiveSpecificationWordItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void initializeAdminAssumptionVariables()
		{
		hasFoundSuggestiveQuestionAssumption_ = false;

		lastFoundOppositePossessiveDefinitionSpecificationItem_ = null;
		secondarySpecificationItem_ = null;
		generalizationAssumptionWordItem_ = null;
		}

	protected boolean hasFoundSuggestiveQuestionAssumption()
		{
		return hasFoundSuggestiveQuestionAssumption_;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasFeminineOrMasculineProperNameEnding;
		boolean hasSecondarySpecificationNonCompoundCollection;
		boolean isHiddenSecondarySpecification;
		boolean isOppositePossessiveSpecificationCollectedWithItself;
		boolean isPossessiveSpecificationCollectedWithItself;
		boolean isSpecificationCollectedWithItself;
		boolean hasRelationWord = ( relationWordItem != null );
		int compoundCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem olderPossessiveReversibleConclusionJustificationItem;
		JustificationItem replacingJustificationItem;
		SpecificationItem compoundSecondarySpecificationItem;
		SpecificationItem compoundSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem primarySpecificationItem = null;
		SpecificationItem anotherPrimarySpecificationItem = null;
		WordItem currentWordItem;
		WordItem possessiveSpecificationWordItem = null;

		possessiveDefinitionSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

		oppositePossessiveSpecificationWordItem_ = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

				if( secondarySpecificationItem != null &&

				( hasRelationWord ||

				( !secondarySpecificationItem.isOlderItem() &&

				( !isPossessive ||
				!secondarySpecificationItem.isExclusiveSpecification() ) ) ) )
					{
					// Initially not all words are linked. So, search all words.
					if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
						{
						isHiddenSecondarySpecification = secondarySpecificationItem.isHiddenSpecification();
						isSpecificationCollectedWithItself = specificationWordItem.isNounWordCollectedWithItself();

						do	{
							if( currentWordItem.isNoun() &&
							currentWordItem != specificationWordItem )
								{
								if( findPossessiveSpecifications( generalizationWordItem, currentWordItem, specificationWordItem ) == Constants.RESULT_OK )
									{
									if( foundPossessiveDefinitionSpecificationItem_ != null )
										{
										possessiveDefinitionSpecificationItem_ = foundPossessiveDefinitionSpecificationItem_;

										if( ( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem() ) != null )
											{
											if( ( primarySpecificationItem = possessiveSpecificationWordItem.firstSpecificationItem( isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
												{
												isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem.isNounWordCollectedWithItself();

												if( ( !secondarySpecificationItem.hasRelationContext() &&
												generalizationWordItem.hasAnsweredQuestion() ) ||

												( isPossessiveSpecificationCollectedWithItself &&
												secondarySpecificationItem.specificationWordItem() == possessiveSpecificationWordItem ) )
													{
													if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a normal specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );

													if( existingUserSpecificationItem == null ||

													// Confirmed specification without relation word
													( hasRelationWord &&

													( !existingUserSpecificationItem.hasRelationContext() ||
													!existingUserSpecificationItem.isOlderItem() ) ) )
														{
														hasFeminineOrMasculineProperNameEnding = false;

														if( isPossessiveSpecificationCollectedWithItself &&
														isFeminineOrMasculineProperNameEnding( isPossessive, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) )
															hasFeminineOrMasculineProperNameEnding = true;

														// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
														if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationWord ? relationWordTypeNr : Constants.WORD_TYPE_UNDEFINED ), foundPossessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) ).result == Constants.RESULT_OK )
															{
															createdSpecificationItem = specificationResult.createdSpecificationItem;
															foundSpecificationItem = specificationResult.foundSpecificationItem;

															if( ( createdOrFoundSpecificationItem = ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ) ) != null )
																{
																if( isArchivedAssignment )
																	secondarySpecificationItem_ = generalizationWordItem.firstAssignmentItem( false, false, true, isPossessive, createdOrFoundSpecificationItem.questionParameter(), createdOrFoundSpecificationItem.relationContextNr(), createdOrFoundSpecificationItem.specificationWordItem() );
																else
																	secondarySpecificationItem_ = createdOrFoundSpecificationItem.updatedSpecificationItem();

																if( hasRelationWord &&
																createdSpecificationItem != null &&
																foundSpecificationItem != null &&
																foundSpecificationItem.hasRelationContext() &&
																// Cross-collected afterwards
																foundSpecificationItem.relationContextNr() != createdSpecificationItem.relationContextNr() &&
																( createdJustificationItem = specificationResult.createdJustificationItem ) != null &&
																( obsoleteJustificationItem = generalizationWordItem.olderPrimarySpecificationJustificationItem( false, true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_COLLECTION_NR, null ) ) != null )
																	{
																	if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
																		{
																		if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null )
																			{
																			// Move justifications from the found specification to the created specification
																			if( createdSpecificationItem.changeFirstJustification( firstJustificationItem ) == Constants.RESULT_OK )
																				{
																				if( generalizationWordItem.replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																				}
																			else
																				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to change the first justification item of the created specification item" );
																			}
																		else
																			return adminItem_.startErrorInItem( 1, moduleNameString_, "The found specification item has no first justification item" );
																		}
																	else
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the obsolete justification item by the created justification item" );
																	}
																}
															else
																return adminItem_.startErrorInItem( 1, moduleNameString_, "No specification item was created or found" );
															}
														else
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a normal exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										}

									// First opposite possessive specification
									if( !CommonVariables.hasShownWarning &&
									possessiveSpecificationWordItem != null &&
									foundOppositePossessiveDefinitionSpecificationItem_ != null &&
									( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveDefinitionSpecificationItem_.specificationWordItem() ) != null )
										{
										if( isNegative ||
										specificationContextNr > Constants.NO_CONTEXT_NR ||
										secondarySpecificationItem_ == null )
											{
											secondarySpecificationItem_ = secondarySpecificationItem;

											// Find another primary definition
											if( !hasRelationWord ||
											isPossessive )
												anotherPrimarySpecificationItem = possessiveSpecificationWordItem.firstAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );
											}

										if( secondarySpecificationItem_ != null &&

										( !secondarySpecificationItem_.isOlderItem() ||
										secondarySpecificationItem_.isSelfGenerated() ) )
											{
											existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

											if( ( ( hasRelationWord &&
											isPossessive ) ||

											existingSpecificationItem == null ||
											!existingSpecificationItem.hasRelationContext() ||

											( !isHiddenSecondarySpecification &&
											secondarySpecificationItem_.hasRelationContext() &&
											!secondarySpecificationItem_.isSelfGeneratedConclusion() ) ) &&

											// Skip if user specification exists
											generalizationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == null )
												{
												hasFeminineOrMasculineProperNameEnding = false;
												hasSecondarySpecificationNonCompoundCollection = false;
												isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_.isNounWordCollectedWithItself();

												if( isOppositePossessiveSpecificationCollectedWithItself &&
												isPossessive &&
												isFeminineOrMasculineProperNameEnding( false, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) )
													{
													hasFeminineOrMasculineProperNameEnding = true;

													if( secondarySpecificationItem.hasSpecificationNonCompoundCollection() )
														{
														hasSecondarySpecificationNonCompoundCollection = true;
														anotherPrimarySpecificationItem = oppositePossessiveSpecificationWordItem_.firstAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ );
														}
													}

												// Make an exclusive specification substitution assumption (opposite)
												// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
												if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.WORD_TYPE_UNDEFINED ), foundOppositePossessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundOppositePossessiveDefinitionSpecificationItem_, ( isOppositePossessiveSpecificationCollectedWithItself ? null : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationCollectedWithItself ? secondarySpecificationItem : secondarySpecificationItem_ ), ( hasSecondarySpecificationNonCompoundCollection ? anotherPrimarySpecificationItem : null ), generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result == Constants.RESULT_OK )
													{
													createdSpecificationItem = specificationResult.createdSpecificationItem;
													foundSpecificationItem = specificationResult.foundSpecificationItem;

													if( isPossessive )
														{
														if( isOppositePossessiveSpecificationCollectedWithItself )
															{
															if( anotherPrimarySpecificationItem != null &&
															( compoundSecondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, false, true, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
																{
																if( !hasRelationWord &&
																adminItem_.isPossessiveUserSpecification() )
																	{
																	if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( hasFeminineOrMasculineProperNameEnding, false, false, false, isNegative, false, !isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, relationWordTypeNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw an opposite specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																else
																	{
																	// Make a compound specification exclusive specification substitution assumption (opposite)
																	// Not yet explained in the design
																	if( adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationWord ? relationWordTypeNr : Constants.WORD_TYPE_UNDEFINED ), Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, foundOppositePossessiveDefinitionSpecificationItem_, null, compoundSecondarySpecificationItem, null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ).result != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a compound exclusive specification substitution assumption that is collected with itself in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															}
														else
															{
															if( foundSpecificationItem == null )
																secondarySpecificationItem_ = createdSpecificationItem;
															else
																{
																if( createdSpecificationItem == null )
																	secondarySpecificationItem_ = foundSpecificationItem.updatedSpecificationItem();
																else
																	{
																	if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null )
																		{
																		// Move justifications from the found specification to the created specification
																		if( createdSpecificationItem.changeFirstJustification( firstJustificationItem ) == Constants.RESULT_OK )
																			{
																			if( generalizationWordItem.replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the found specification by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																			}
																		else
																			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to change the first justification item of the created specification item" );
																		}
																	else
																		return adminItem_.startErrorInItem( 1, moduleNameString_, "The found specification item has no first justification item" );
																	}
																}
															}
														}
													else
														{
														if( createdSpecificationItem == null )
															{
															if( ( obsoleteJustificationItem = generalizationWordItem.obsoleteJustificationItem() ) == null )
																{
																if( foundSpecificationItem != null &&
																specificationResult.createdJustificationItem == null &&
																( replacingJustificationItem = generalizationWordItem.olderSecondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem_ ) ) != null &&
																( obsoleteJustificationItem = generalizationWordItem.olderSecondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, secondarySpecificationItem_ ) ) != null )
																	{
																	if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, replacingJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace an obsolete justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																{
																if( generalizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
																	return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															{
															if( hasRelationWord &&
															existingSpecificationItem != null &&
															existingSpecificationItem.hasRelationContext() &&

															( !createdSpecificationItem.isHiddenSpecification() ||
															existingSpecificationItem.isHiddenSpecification() ||

															( isSpecificationCollectedWithItself &&
															existingSpecificationItem.specificationCollectionNr() != createdSpecificationItem.specificationCollectionNr() ) ) &&

															( existingSpecificationItem.isSelfGeneratedAssumption() ||

															( isOppositePossessiveSpecificationCollectedWithItself &&
															// Cross-collected afterwards
															existingSpecificationItem.relationContextNr() != createdSpecificationItem.relationContextNr() ) ) )
																{
																if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
																	{
																	if( isOppositePossessiveSpecificationCollectedWithItself )
																		{
																		if( ( olderPossessiveReversibleConclusionJustificationItem = existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) != null )
																			{
																			if( generalizationWordItem.attachJustification( olderPossessiveReversibleConclusionJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach the created possessive reversible assumption justification to the created assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																			}
																		}
																	else
																		{
																		if( ( firstJustificationItem = existingSpecificationItem.firstJustificationItem() ) != null )
																			{
																			// Attach all justification items of the existing specification to the created specification
																			if( createdSpecificationItem.changeFirstJustification( firstJustificationItem ) != Constants.RESULT_OK )
																				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to change the first justification item of the created specification" );
																			}
																		else
																			return adminItem_.startErrorInItem( 1, moduleNameString_, "The existing specification item has no first justification item" );
																		}
																	}
																else
																	return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the existing specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														}
													}
												else
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an opposite exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
												}
											}
										}
									}
								else
									return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find possessive specifications in word \"" + currentWordItem.anyWordTypeString() + "\"" );
								}
							}
						while( !CommonVariables.hasShownWarning &&
						( currentWordItem = currentWordItem.nextWordItem() ) != null );

						if( !CommonVariables.hasShownWarning &&
						hasRelationWord &&
						secondarySpecificationItem_ != null )
							{
							if( secondarySpecificationItem.isSpecificationWordCollectedWithItself() &&
							secondarySpecificationItem.hasSpecificationNonCompoundCollection() &&
							( compoundCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
								{
								if( ( compoundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, true, false, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, compoundCollectionNr, secondarySpecificationItem.relationContextNr(), specificationWordItem ) ) != null )
									secondarySpecificationItem = compoundSpecificationItem;
								}

							if( makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem_, ( isHiddenSecondarySpecification ? adminItem_.userSpecificationItem() : secondarySpecificationItem ), generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );
					}
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		short questionParameter = ( isGeneralizationProperName ? Constants.WORD_PARAMETER_SINGULAR_VERB_IS : Constants.NO_QUESTION_PARAMETER );
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
				{
				if( generalizationWordItem != null )
					{
					if( specificationWordItem != null )
						{
						if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
							{
							do	{
								if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
									{
									if( currentGeneralizationWordItem != generalizationWordItem &&
									!currentGeneralizationWordItem.hasCollection() &&
									!currentGeneralizationWordItem.hasPossessiveButNonQuestionSpecification() &&
									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
										{
										// Get secondary specification for justification
										if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
											{
											if( isGeneralizationProperName )
												{
												if( makeGeneralizationAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											{
											// Get secondary question specification for justification
											if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != null )
												{
												if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isArchivedAssignment, false, isNegative, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, Constants.NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, null ) != Constants.RESULT_OK )
													adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									}
								else
									return adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an undefined generalization word" );
								}
							while( !CommonVariables.hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
							}
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word is undefined" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word type isn't a noun" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word type isn't a reasoning word type" );

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		SpecificationResultType specificationResult;
		int currentSpecificationCollectionNr;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem indirectPrimarySpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		WordItem commonWordItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem negativeQuestionSpecificationWordItem = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( userSpecificationItem != null )
					{
					if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) ) != null )
						{
						do	{
							if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
							( currentSpecificationCollectionNr = currentQuestionSpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
								{
								if( ( indirectPrimarySpecificationItem = currentQuestionSpecificationWordItem.firstActiveAssignmentOrSpecificationItem( false, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
									{
									if( ( specificationResult = generalizationWordItem.findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ).result == Constants.RESULT_OK )
										{
										if( specificationResult.relatedSpecificationItem != null )
											negativeQuestionSpecificationWordItem = specificationResult.relatedSpecificationItem.specificationWordItem();

										// Draw an indirectly answered question conclusion
										// Not yet explained in the design
										if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, indirectPrimarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
											{
											if( negativeQuestionSpecificationWordItem != null &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null &&
											( commonWordItem = currentQuestionSpecificationWordItem.commonWordItem( currentSpecificationCollectionNr ) ) != null )
												{
												if( commonWordItem.isNounWordCollectedWithItself() )
													{
													if( generalizationWordItem.replaceOrDeleteSpecification( currentQuestionSpecificationItem.updatedSpecificationItem(), createdSpecificationItem ) == Constants.RESULT_OK )
														{
														while( ( obsoleteJustificationItem = generalizationWordItem.obsoleteJustificationItem() ) != null )
															{
															if( generalizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
																return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the answered question by the created specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, commonWordItem ) ) != null &&
													( anotherPrimarySpecificationItem = commonWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != null )
														{
														// Draw a negative assumption
														// Not yet explained in the design
														if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, null, generalizationWordItem, negativeQuestionSpecificationWordItem, null ).result != Constants.RESULT_OK )
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												}

											currentQuestionSpecificationItem = adminItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
											}
										else
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is collected with itself" );
									}
								else
									currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem();
								}
							else
								currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem();
							}
						while( currentQuestionSpecificationItem != null );
						}
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		short nOptionsLeft = 0;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem currentQuestionSpecificationWordItem;
		WordItem onlyOptionLeftSpecificationWordItem = null;

		if( userSpecificationItem != null )
			{
			if( generalizationWordItem != null )
				{
				if( userSpecificationItem.hasSpecificationCollection() )
					{
					if( ( currentQuestionSpecificationItem = generalizationWordItem.firstSelectedSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != null )
						{
						do	{
							if( ( currentQuestionSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null )
								{
								if( generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isPossessive, Constants.NO_QUESTION_PARAMETER, currentQuestionSpecificationWordItem ) == null )
									{
									nOptionsLeft++;
									onlyOptionLeftSpecificationWordItem = currentQuestionSpecificationWordItem;
									}
								}
							}
						while( ( currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem() ) != null );

						if( nOptionsLeft == 1 &&
						onlyOptionLeftSpecificationWordItem != null &&
						( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
							{
							do	{
								if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
									{
									if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null &&
									( anotherPrimarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != null )
										{
										// Make an only option left assumption
										// Not yet explained in the design
										if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									return adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an undefined generalization word" );
								}
							while( !CommonVariables.hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
							}
						}
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item has no specification collection" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasGeneralizationWordConfirmedSpecification;
		boolean hasSameSpecificationCollection;
		boolean isOppositePossessiveSpecificationCollectedWithItself;
		boolean isPossessiveReversibleAssumption;
		boolean isPossessiveSpecificationCollectedWithItself;
		boolean hasFeminineOrMasculineProperNameEnding = false;
		boolean isArchivedAssignmentAssumption = false;
		boolean isForcingNewJustification = false;
		short assumptionJustificationTypeNr;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem possessiveSpecificationWordItem;
		WordItem feminineSpecificationWordItem = null;

		if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( secondarySpecificationItem != null )
				{
				if( generalizationWordItem != null )
					{
					if( relationWordItem != null )
						{
						// Skip if global variables are not assigned
						if( possessiveDefinitionSpecificationItem_ != null &&
						secondarySpecificationItem_ != null &&
						( possessiveSpecificationWordItem = possessiveDefinitionSpecificationItem_.specificationWordItem() ) != null )
							{
							isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem.isNounWordCollectedWithItself();

							if( isPossessiveSpecificationCollectedWithItself ||
							possessiveDefinitionSpecificationItem_.specificationCollectionNr() == secondarySpecificationItem.specificationCollectionNr() )
								{
								existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );
								existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );

								// Skip if possessive user specification exists
								if( ( existingUserSpecificationItem == null &&
								!possessiveSpecificationWordItem.isUserSpecificationWord &&

								( !isPossessiveSpecificationCollectedWithItself ||

								( existingSpecificationItem == null &&
								relationWordItem.isUserGeneralizationWord ) ) &&

								// Skip if possessive specification exists
								relationWordItem.olderSecondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == null ) ||

								( isPossessiveSpecificationCollectedWithItself &&
								alternativeSecondarySpecificationItem != null &&
								!alternativeSecondarySpecificationItem.isOlderItem() &&
								alternativeSecondarySpecificationItem.isActiveAssignment() &&

								// Skip if possessive user specification exists
								( existingUserSpecificationItem == null ||

								( existingUserSpecificationItem.isOlderItem() &&
								existingUserSpecificationItem.hasSpecificationCompoundCollection() ) ) ) )
									{
									isPossessiveReversibleAssumption = ( secondarySpecificationItem.specificationWordItem() == possessiveSpecificationWordItem );

									// Make a possessive conditional specification assumption (normal)
									// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
									if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, isNegative, isPartOf, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveDefinitionSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessive ? possessiveDefinitionSpecificationItem_ : null ), null, ( isPossessiveSpecificationCollectedWithItself ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), null, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result == Constants.RESULT_OK )
										{
										if( !isPossessive &&
										existingSpecificationItem != null &&
										( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
											{
											existingSpecificationItem = existingSpecificationItem.updatedSpecificationItem();

											if( createdSpecificationItem.assumptionLevel() < existingSpecificationItem.assumptionLevel() )
												{
												if( relationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the existing specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );
									}

								if( !CommonVariables.hasShownWarning )
									{
									existingSpecificationItem = null;

									if( oppositePossessiveSpecificationWordItem_ != null )
										{
										isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_.isNounWordCollectedWithItself();
										isPossessiveReversibleAssumption = ( secondarySpecificationItem.specificationWordItem() == oppositePossessiveSpecificationWordItem_ );
										assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

										if( alternativeSecondarySpecificationItem != null &&
										// Try to find a self-generated conclusion first
										( existingSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, false, isNegative, isPossessive, false, oppositePossessiveSpecificationWordItem_, null ) ) == null &&
										// If not found, try to find a self-generated assumption
										( assumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != null )
											{
											hasGeneralizationWordConfirmedSpecification = generalizationWordItem.hasConfirmedSpecification();
											hasSameSpecificationCollection = ( assumptionSpecificationItem.specificationCollectionNr() == alternativeSecondarySpecificationItem.specificationCollectionNr() );

											if( !assumptionSpecificationItem.hasCurrentCreationSentenceNr() &&
											!assumptionSpecificationItem.hasNewInformation() &&

											( ( hasGeneralizationWordConfirmedSpecification &&
											hasSameSpecificationCollection ) ||

											( !hasGeneralizationWordConfirmedSpecification &&
											!hasSameSpecificationCollection ) ||

											alternativeSecondarySpecificationItem.isPossessive() ) )
												existingSpecificationItem = assumptionSpecificationItem;
											}

										// Skip if assumption already exists as user specification
										if( ( existingSpecificationItem == null ||
										!existingSpecificationItem.hasRelationContext() ||
										!generalizationWordItem.isOlderItem() ) &&

										( ( !isOppositePossessiveSpecificationCollectedWithItself &&
										// Skip if already exists as self-generated assumption
										relationWordItem.olderSecondarySpecificationJustificationItem( assumptionJustificationTypeNr, secondarySpecificationItem ) == null ) ||

										( isOppositePossessiveSpecificationCollectedWithItself &&
										alternativeSecondarySpecificationItem != null &&
										// Skip if already exists as self-generated assumption
										relationWordItem.olderSecondarySpecificationJustificationItem( assumptionJustificationTypeNr, alternativeSecondarySpecificationItem ) == null ) ) )
											{
											if( isOppositePossessiveSpecificationCollectedWithItself &&

											( alternativeSecondarySpecificationItem == null ||
											alternativeSecondarySpecificationItem.isUserSpecification() ) &&

											isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) )
												hasFeminineOrMasculineProperNameEnding = true;

											// Make a possessive conditional specification assumption (opposite)
											// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
											if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, false, false, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, assumptionJustificationTypeNr, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, ( isPossessiveReversibleAssumption ? null : possessiveDefinitionSpecificationItem_ ), null, ( isOppositePossessiveSpecificationCollectedWithItself && alternativeSecondarySpecificationItem != null ? alternativeSecondarySpecificationItem : secondarySpecificationItem ), null, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result == Constants.RESULT_OK )
												{
												if( !isOppositePossessiveSpecificationCollectedWithItself &&
												!isPossessiveReversibleAssumption &&
												( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
													{
													if( foundSpecificationItem.isOlderItem() &&
													( assumptionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, isNegative, !isPossessive, true, oppositePossessiveSpecificationWordItem_, relationWordItem ) ) != null )
														{
														if( assumptionSpecificationItem.secondarySpecificationJustificationItem( foundSpecificationItem ) == null )
															{
															// Add missing justification
															if( ( justificationResult = generalizationWordItem.addJustification( false, false, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, null, null, foundSpecificationItem, null, null ) ).result == Constants.RESULT_OK )
																{
																if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
																	{
																	if( generalizationWordItem.attachJustification( createdJustificationItem, assumptionSpecificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach the created justification to the given assumption specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a justification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}

												if( alternativeSecondarySpecificationItem != null )
													{
													if( isOppositePossessiveSpecificationCollectedWithItself )
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_.feminineCollectionWordItem();
													else
														feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_.feminineOrMasculineCommonWordItem( isPossessive ? generalizationWordItem.hasFeminineProperNameEnding() : relationWordItem.hasFeminineProperNameEnding() );

													if( feminineSpecificationWordItem != null &&
													isFeminineOrMasculineProperNameEnding( isPossessive, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ) )
														{
														existingSpecificationItem = null;

														if( isOppositePossessiveSpecificationCollectedWithItself )
															existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, feminineSpecificationWordItem, generalizationWordItem );

														if( existingSpecificationItem == null ||

														// Skip if assumption under certain conditions already exists
														( !existingSpecificationItem.isOlderItem() &&
														!secondarySpecificationItem.isHiddenSpecification() ) )
															{
															if( isArchivedAssignment &&
															relationWordItem.isUserGeneralizationWord )
																isArchivedAssignmentAssumption = true;

															if( isOppositePossessiveSpecificationCollectedWithItself &&
															isPossessive &&
															( tempSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != null )
																{
																if( tempSpecificationItem.isOlderItem() )
																	isForcingNewJustification = true;
																}

															// Make an opposite possessive feminine proper name ending assumption
															// Not yet explained in the design
															if( adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignmentAssumption, isArchivedAssignmentAssumption, false, isExclusiveSpecification, isForcingNewJustification, isNegative, isPartOf, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, possessiveDefinitionSpecificationItem_, null, alternativeSecondarySpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ).result == Constants.RESULT_OK )
																{
																if( ( obsoleteJustificationItem = generalizationWordItem.obsoleteJustificationItem() ) != null )
																	{
																	if( generalizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace an obsolete justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												}
											else
												return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
											}
										}
									else
										return adminItem_.startErrorInItem( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );
									}
								}
							}
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );

		return Constants.RESULT_OK;
		}

	protected byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean hasGeneralizationWordConfirmedSpecification;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean isGeneralizationCollectedWithItself;
		boolean isGeneralizationWordUserGeneralizationWord;
		boolean isSpecificationCollectedWithItself;
		GeneralizationItem currentGeneralizationItem;
		JustificationItem createdJustificationItem;
		JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem;
		SpecificationItem alternativeAnotherPrimarySpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem primaryPartOfSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem simpleUserSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem feminineCollectionWordItem;
		WordItem userSpecificationWordItem;

		if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( generalizationWordItem != null )
				{
				if( specificationWordItem != null )
					{
					if( generalizationWordItem != specificationWordItem &&
					( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
						{
						hasGeneralizationWordConfirmedSpecification = generalizationWordItem.hasConfirmedSpecification();
						hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
						isGeneralizationCollectedWithItself = generalizationWordItem.isNounWordCollectedWithItself();
						isGeneralizationWordUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
						isSpecificationCollectedWithItself = specificationWordItem.isNounWordCollectedWithItself();

						simpleUserSpecificationItem = adminItem_.simpleUserSpecificationItem();

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&

								( ( !isPossessive &&
								currentGeneralizationWordItem.hasNonPossessiveAndNonQuestionPartOfSpecification() ) ||

								( !currentGeneralizationWordItem.hasCollection() &&
								!currentGeneralizationWordItem.hasPossessiveButNonQuestionSpecification() ) ) )
									{
									existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

									// Skip if concluded conclusion exists
									if( ( existingSpecificationItem == null ||
									existingSpecificationItem != simpleUserSpecificationItem ) &&

									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
										{
										alternativeAnotherPrimarySpecificationItem = null;
										secondarySpecificationItem = null;

										// Get secondary specification for justification
										if( isGeneralizationWordUserGeneralizationWord &&
										simpleUserSpecificationItem != null &&
										!simpleUserSpecificationItem.isPossessive() &&

										( ( isSpecificationCollectedWithItself &&
										simpleUserSpecificationItem.specificationWordItem() == primarySpecificationItem.specificationWordItem() ) ||

										simpleUserSpecificationItem.specificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() ) )
											secondarySpecificationItem = simpleUserSpecificationItem;
										else
											{
											if( isSpecificationCollectedWithItself &&
											hasGeneralizationWordFeminineProperNameEnding &&
											( feminineCollectionWordItem = specificationWordItem.feminineCollectionWordItem() ) != null )
												secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, feminineCollectionWordItem );

											if( secondarySpecificationItem == null )
												{
												// Try to find self-generated conclusion first
												if( ( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, null ) ) == null )
													// Otherwise, try to find self-generated assumption
													tempSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );

												if( tempSpecificationItem != null &&

												( existingSpecificationItem == null ||
												tempSpecificationItem.isSelfGeneratedConclusion() ||
												// Skip assumption with relation word
												existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) == null ) )
													secondarySpecificationItem = tempSpecificationItem;
												}

											if( secondarySpecificationItem == null )
												{
												if( hasGeneralizationWordFeminineProperNameEnding &&
												!isGeneralizationWordUserGeneralizationWord &&
												simpleUserSpecificationItem != null &&
												simpleUserSpecificationItem.isSpecificationWordCollectedWithItself() &&
												simpleUserSpecificationItem.isPossessive() &&
												( userSpecificationWordItem = simpleUserSpecificationItem.specificationWordItem() ) != null )
													{
													if( ( tempSpecificationItem = userSpecificationWordItem.firstSpecificationItem( isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null )
														{
														primarySpecificationItem = simpleUserSpecificationItem;
														secondarySpecificationItem = tempSpecificationItem;
														}
													}
												}
											else
												{
												if( isSpecificationCollectedWithItself &&
												secondarySpecificationItem.isHiddenSpecification() )
													{
													secondarySpecificationItem = adminItem_.alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, false, true, isNegative, !isPossessive, simpleUserSpecificationItem, generalizationWordItem, specificationWordItem );
													alternativeAnotherPrimarySpecificationItem = adminItem_.alternativeAnotherPrimarySpecificationItem();
													}
												}
											}

										if( secondarySpecificationItem != null &&

										( !isSpecificationCollectedWithItself ||
										existingSpecificationItem == null ||
										!secondarySpecificationItem.isHiddenSpecification() ) )
											{
											if( primarySpecificationItem.isSpecificationGeneralization() )
												{
												if( primarySpecificationItem.isPartOf() &&
												// If a conclusion exists, skip creating an assumption
												generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, isNegative, false, false, currentGeneralizationWordItem, null ) == null )
													{
													// Make a substitution 'part of' assumption
													// Not yet explained in the design
													if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem.generalizationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, alternativeAnotherPrimarySpecificationItem, primarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ) ).result == Constants.RESULT_OK )
														{
														if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != null &&
														( specificationSubstitutionPartOfAssumptionJustificationItem = generalizationWordItem.specificationSubstitutionPartOfAssumptionJustificationItem( primarySpecificationItem ) ) != null )
															{
															primaryPartOfSpecificationItem = specificationSubstitutionPartOfAssumptionJustificationItem.primarySpecificationItem();

															if( hasGeneralizationWordConfirmedSpecification ||
															specificationSubstitutionPartOfAssumptionJustificationItem.hasPrimaryAnsweredQuestion() ||

															( primaryPartOfSpecificationItem != null &&
															secondarySpecificationItem.assumptionLevel() < primaryPartOfSpecificationItem.assumptionLevel() ) )
																{
																if( generalizationWordItem.updateJustificationInSpecifications( false, specificationSubstitutionPartOfAssumptionJustificationItem, createdJustificationItem ) == Constants.RESULT_OK )
																	{
																	if( !specificationSubstitutionPartOfAssumptionJustificationItem.isReplacedOrDeletedItem() &&
																	!generalizationWordItem.isJustificationInUse( specificationSubstitutionPartOfAssumptionJustificationItem ) )
																		{
																		if( generalizationWordItem.replaceOrDeleteJustification( specificationSubstitutionPartOfAssumptionJustificationItem ) != Constants.RESULT_OK )
																			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the specification substitution 'part of' assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	}
																else
																	return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to update a justification item by a conclusion justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														}
													else
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated specification substitution specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( !isGeneralizationCollectedWithItself ||

												( primarySpecificationItem.isExclusiveSpecification() &&
												secondarySpecificationItem.isExclusiveSpecification() &&
												!secondarySpecificationItem.isOlderItem() ) )
													{
													if( makeGeneralizationAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) == Constants.RESULT_OK )
														generalizationAssumptionWordItem_ = currentGeneralizationWordItem;
													else
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										}
									}
								}
							else
								return adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an undefined generalization word" );
							}
						while( !CommonVariables.hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
						}

					// Definition specification
					if( generalizationWordTypeNr != Constants.WORD_TYPE_PROPER_NAME &&
					generalizationAssumptionWordItem_ != null &&
					generalizationWordItem != generalizationAssumptionWordItem_ &&
					( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
					( secondarySpecificationItem = specificationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != null )
						{
						// Make an exclusive specification substitution assumption
						// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
						if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, generalizationAssumptionWordItem_, null ).result != Constants.RESULT_OK )
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult;
		int secondarySpecificationCollectionNr;
		SpecificationItem existingSpecificationItem;

		if( secondarySpecificationItem != null )
			{
			if( generalizationWordItem != null )
				{
				if( relationWordItem != null )
					{
					secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();
					existingSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

					// Check if specification doesn't exist yet
					if( existingSpecificationItem == null ||
					existingSpecificationItem.isHiddenSpecification() )
						{
						// Check if a related specification exists
						if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result == Constants.RESULT_OK )
							{
							if( specificationResult.relatedSpecificationItem == null )
								{
								if( generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() )
									{
									if( correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, secondarySpecificationItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									// Make a suggestive question assumption
									// Not yet explained in the design
									hasFoundSuggestiveQuestionAssumption_ = true;

									if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						else
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );
						}
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationItem lastFoundOppositePossessiveDefinitionSpecificationItem()
		{
		return lastFoundOppositePossessiveDefinitionSpecificationItem_;
		}
	};

/*************************************************************************
 *	"He is my loving ally and my fortress,
 *	my tower of safety, my rescuer.
 *	He is my shield, and I take refuge in him.
 *	He makes the nations submit to me." (Psalm 144:2)
 *************************************************************************/
