/*
 *	Class:			AdminReasoning
 *	Supports class:	AdminItem
 *	Purpose:		To support grammar-based reasoning, and to
 *					autonomously ask grammar-based questions
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

class AdminReasoning
	{
	// Private constructible variables

	private boolean hasAdjustedQuestion_;
	private boolean hasFoundConflictWithUserSpecification_;

	private SpecificationItem alternativeAnotherPrimarySpecificationItem_;
	private SpecificationItem adjustedQuestionSecondarySpecificationItem_;
	private SpecificationItem adjustedQuestionSpecificationItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte checkForConflictWithUserSpecification( WordItem generalizationWordItem, WordItem primarySpecificationWordItem, WordItem questionSpecificationWordItem )
		{
		SpecificationItem currentSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem currentSpecificationWordItem;

		hasFoundConflictWithUserSpecification_ = false;

		if( generalizationWordItem != null )
			{
			if( primarySpecificationWordItem != null )
				{
				if( questionSpecificationWordItem != null )
					{
					if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem() ) != null )
						{
						do	{
							if( !currentSpecificationItem.isExclusiveSpecification() &&
							!currentSpecificationItem.isNegative() &&
							!currentSpecificationItem.isPossessive() &&
							// Current sentence
							!currentSpecificationItem.isOlderItem() &&
							// User sentence
							!currentSpecificationItem.isSelfGenerated() &&
							( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
								{
								foundSpecificationItem = currentSpecificationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, questionSpecificationWordItem );

								if( ( foundSpecificationItem == null ||

								( !foundSpecificationItem.isExclusiveSpecification() &&
								!foundSpecificationItem.isNegative() ) ) )
									hasFoundConflictWithUserSpecification_ = true;
								}
							}
						while( !hasFoundConflictWithUserSpecification_ &&
						( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
						}
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given question specification word item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given primary specification word item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte processJustification( boolean hasFoundRelationContext, boolean hasRelationWord, short justificationTypeNr, JustificationItem createdOrFoundJustificationItem, SpecificationItem foundSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isAttachingJustification = true;
		boolean isDeletingCreatedJustification = false;
		JustificationResultType justificationResult;
		JustificationItem obsoleteJustificationItem;
		JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem;
		JustificationItem existingJustificationItem = null;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( createdOrFoundJustificationItem != null )
			{
			if( foundSpecificationItem != null )
				{
				if( generalizationWordItem != null )
					{
					if( primarySpecificationItem != null &&
					secondarySpecificationItem != null )
						{
						secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem();

						switch( justificationTypeNr )
							{
							case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
								if( relationWordItem != null )
									{
									if( secondarySpecificationItem.isSelfGeneratedAssumption() )
										{
										// Skip if justification with self-generated assumption is created,
										// while justification with self-generated conclusion already exists
										if( relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPossessive(), false, secondarySpecificationItem.specificationWordItem(), generalizationWordItem ) != null )
											isDeletingCreatedJustification = true;
										}
									else
										{
										if( secondarySpecificationItem.isSelfGeneratedConclusion() &&
										( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPossessive(), true, secondarySpecificationWordItem, generalizationWordItem ) ) != null )
											{
											if( ( existingJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != null )
												{
												if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) == Constants.RESULT_OK )
													isAttachingJustification = false;
												else
													return adminItem_.addError( 1, moduleNameString_, "I failed to replace an existing justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											{
											if( !foundSpecificationItem.isOlderItem() &&
											foundSpecificationItem.isHiddenSpecification() &&
											generalizationWordItem.isUserGeneralizationWord &&
											createdOrFoundJustificationItem.hasFeminineOrMasculineProperNameEnding() )
												isAttachingJustification = false;
											}
										}
									}

								break;

							case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( hasFoundRelationContext &&
								hasRelationWord &&
								generalizationWordItem.isUserGeneralizationWord &&
								// Confirmation
								secondarySpecificationItem.isUserSpecification() )
									{
									if( ( existingJustificationItem = generalizationWordItem.olderComplexJustificationItem( true, justificationTypeNr, secondarySpecificationItem.specificationCollectionNr(), primarySpecificationItem ) ) == null )
										{
										if( ( obsoleteJustificationItem = generalizationWordItem.possessiveReversibleAssumptionJustificationItem( secondarySpecificationWordItem ) ) != null )
											{
											if( ( justificationResult = generalizationWordItem.addJustification( false, false, justificationTypeNr, obsoleteJustificationItem.orderNr, obsoleteJustificationItem.originalSentenceNr(), null, null, secondarySpecificationItem, null, null ) ).result == Constants.RESULT_OK )
												{
												if( justificationResult.createdJustificationItem != null )
													{
													if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, justificationResult.createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification item without primary specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													return adminItem_.startError( 1, moduleNameString_, "I couldn't find or create a justification without primary specification" );
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification without primary specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											// Skip if justification already exists as an opposite possessive conditional specification assumption
											if( secondarySpecificationItem.isGeneralizationProperName() &&
											generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) != null )
												isDeletingCreatedJustification = true;
											}
										}
									else
										{
										// Replace existing justification by the created justification
										if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) == Constants.RESULT_OK )
											isAttachingJustification = false;
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace an existing justification by the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									}

								break;

							case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( !hasRelationWord )
									{
									if( ( existingJustificationItem = foundSpecificationItem.differentAssumptionLevelSpecificationSubstitutionJustificationItem( primarySpecificationItem, secondarySpecificationItem ) ) == null )
										{
										if( ( obsoleteJustificationItem = foundSpecificationItem.answeredQuestionJustificationItem() ) != null )
											{
											if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) == Constants.RESULT_OK )
												isAttachingJustification = false;
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification item without primary specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										}
									else
										{
										isAttachingJustification = false;

										if( primarySpecificationItem.assumptionLevel() < existingJustificationItem.primarySpecificationAssumptionLevel() )
											{
											// Replace less certain existing justification by the created justification
											if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											isDeletingCreatedJustification = true;
										}
									}

								break;

							case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
								if( !hasRelationWord &&
								( specificationSubstitutionPartOfAssumptionJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) != null )
									{
									if( specificationSubstitutionPartOfAssumptionJustificationItem.hasHiddenPrimarySpecification() )
										{
										if( specificationSubstitutionPartOfAssumptionJustificationItem.changePrimarySpecification( primarySpecificationItem ) == Constants.RESULT_OK )
											isDeletingCreatedJustification = true;
										else
											return adminItem_.addError( 1, moduleNameString_, "I failed to replace a hidden primary specification of the attached justification item of the found specification substitution 'part of' assumption justification item" );
										}
									}

								break;

							case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
								if( hasAdjustedQuestion_ &&
								!foundSpecificationItem.isReplacedItem() &&

								( primarySpecificationItem.hasSpecificationNonCompoundCollection() ||
								!primarySpecificationItem.isSpecificationWordCollectedWithItself() ) )
									{
									// Remove obsolete justifications from adjusted compound question
									if( ( specificationResult = generalizationWordItem.copySpecificationItem( foundSpecificationItem.isInactiveAssignment(), foundSpecificationItem.isArchivedAssignment(), foundSpecificationItem.isAnsweredQuestion(), foundSpecificationItem.isExclusiveSpecification(), foundSpecificationItem.assignmentLevel(), foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), foundSpecificationItem.relationContextNr(), createdOrFoundJustificationItem, foundSpecificationItem ) ).result == Constants.RESULT_OK )
										{
										if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
											{
											if( generalizationWordItem.replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
												isAttachingJustification = false;
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete a specification" );
											}
										else
											return adminItem_.addError( 1, moduleNameString_, "I couldn't create the adjusted question specification" );
										}
									else
										return adminItem_.addError( 1, moduleNameString_, "I failed to copy a specification with a different first justification item" );
									}
							}
						}

					if( isDeletingCreatedJustification )
						{
						if( generalizationWordItem.replaceOrDeleteJustification( createdOrFoundJustificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						{
						if( isAttachingJustification )
							{
							// Attach created justification to the found specification
							if( generalizationWordItem.attachJustification( createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given found specification item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given created or found justification item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminReasoning( AdminItem adminItem )
		{
		String errorString = null;

		hasAdjustedQuestion_ = false;
		hasFoundConflictWithUserSpecification_ = false;

		alternativeAnotherPrimarySpecificationItem_ = null;
		adjustedQuestionSecondarySpecificationItem_ = null;
		adjustedQuestionSpecificationItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void initializeAdminReasoningVariables()
		{
		hasAdjustedQuestion_ = false;
		hasFoundConflictWithUserSpecification_ = false;

		adjustedQuestionSecondarySpecificationItem_ = null;
		adjustedQuestionSpecificationItem_ = null;
		}

	protected byte askSpecificationSubstitutionQuestion( boolean isArchivedAssignment, boolean isExclusiveSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasCorrectedAssumptionByOppositeQuestion;
		boolean isPrimarySpecificationCollectedWithItself;
		boolean isSecondarySpecificationCompoundCollection;
		boolean isBlockingCommonWordOfCompoundCollection = false;
		int nonCompoundSpecificationCollectionNr;
		int secondaryRelationContextNr;
		int secondarySpecificationCollectionNr;
		SpecificationItem adjustedQuestionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( primarySpecificationItem != null )
			{
			if( secondarySpecificationItem != null )
				{
				if( generalizationWordItem != null )
					{
					if( specificationWordItem != null )
						{
						if( ( secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
							{
							if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
								{
								hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();
								isPrimarySpecificationCollectedWithItself = primarySpecificationItem.isSpecificationWordCollectedWithItself();

								nonCompoundSpecificationCollectionNr = secondarySpecificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );
								secondaryRelationContextNr = secondarySpecificationItem.relationContextNr();
								isSecondarySpecificationCompoundCollection = secondarySpecificationWordItem.isCompoundCollection( secondarySpecificationCollectionNr, specificationWordItem );

								foundSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Skip if already exists as specification
								if( hasCorrectedAssumptionByOppositeQuestion ||
								isPrimarySpecificationCollectedWithItself ||
								foundSpecificationItem == null ||

								( isSecondarySpecificationCompoundCollection &&
								foundSpecificationItem.isSelfGeneratedAssumption() ) )
									{
									if( generalizationWordItem.isUserGeneralizationWord )
										{
										if( isSecondarySpecificationCompoundCollection &&
										nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR )
											{
											// Check for conflicts
											if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, secondaryRelationContextNr, secondarySpecificationWordItem ) == Constants.RESULT_OK )
												{
												// Check for adjusted compound question
												if( ( specificationResult = generalizationWordItem.findQuestionToBeAdjustedByCompoundCollection( false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, secondarySpecificationCollectionNr, secondaryRelationContextNr, adjustedQuestionSpecificationItem_, specificationWordItem ) ).result == Constants.RESULT_OK )
													{
													if( ( adjustedQuestionSpecificationItem = specificationResult.adjustedQuestionSpecificationItem ) != null )
														{
														// Write adjusted compound question
														if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, adjustedQuestionSpecificationItem ) == Constants.RESULT_OK )
															hasAdjustedQuestion_ = true;
														else
															return adminItem_.addError( 1, moduleNameString_, "I failed to write an adjusted compound question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													return adminItem_.addError( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to check for a specification conflict in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											if( checkForConflictWithUserSpecification( generalizationWordItem, specificationWordItem, secondarySpecificationWordItem ) == Constants.RESULT_OK )
												{
												if( hasFoundConflictWithUserSpecification_ )
													isBlockingCommonWordOfCompoundCollection = true;
												else
													{
													if( nonCompoundSpecificationCollectionNr == Constants.NO_COLLECTION_NR )
														nonCompoundSpecificationCollectionNr = adminItem_.nonCompoundCollectionNrInAllWords( secondarySpecificationCollectionNr );

													if( generalizationWordItem.firstUserSpecificationItem( false, false, secondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) != null ||

													( nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
													generalizationWordItem.firstUserSpecificationItem( false, false, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) != null ) )
														isBlockingCommonWordOfCompoundCollection = true;
													}
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to check for a conflict with the user specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										}

									if( !CommonVariables.hasShownWarning &&
									!isBlockingCommonWordOfCompoundCollection &&

									( isExclusiveSpecification ||
									hasCorrectedAssumptionByOppositeQuestion ||
									secondarySpecificationItem.isExclusiveSpecification() ) )
										{
										alternativeAnotherPrimarySpecificationItem_ = null;

										if( isPrimarySpecificationCollectedWithItself &&
										primarySpecificationItem.isHiddenSpecification() )
											primarySpecificationItem = alternativeJustificationSpecificationItem( generalizationWordItem.hasFeminineProperNameEnding(), true, false, false, false, adminItem_.userSpecificationItem(), generalizationWordItem, specificationWordItem );

										if( primarySpecificationItem != null &&

										( hasCorrectedAssumptionByOppositeQuestion ||
										!primarySpecificationItem.isOlderItem() ||
										primarySpecificationItem.isSelfGenerated() ) )
											{
											// Ask a specification substitution question
											// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
											if( ( specificationResult = addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, false, primarySpecificationItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem_, ( isPrimarySpecificationCollectedWithItself && adjustedQuestionSecondarySpecificationItem_ != null ? adjustedQuestionSecondarySpecificationItem_ : secondarySpecificationItem ), null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
												{
												if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
													{
													if( isPrimarySpecificationCollectedWithItself &&
													specificationResult.foundSpecificationItem != null &&
													!primarySpecificationItem.isPossessive() &&
													primarySpecificationItem.isUserSpecification() &&
													secondarySpecificationItem.isUserSpecification() )
														{
														adjustedQuestionSecondarySpecificationItem_ = secondarySpecificationItem;
														adjustedQuestionSpecificationItem_ = specificationResult.foundSpecificationItem.updatedSpecificationItem();
														}
													}
												else
													{
													// Has created question part
													// Change specification to question part
													if( foundSpecificationItem != null &&
													secondarySpecificationWordItem != specificationWordItem )
														{
														if( generalizationWordItem.replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											else
												return adminItem_.addError( 1, moduleNameString_, "I failed to ask a self-generated specification substitution question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								}
							else
								return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word item" );
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification collection" );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNameEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkippingAdditionalConclusionOrAskQuestion, boolean isSpecificationGeneralization, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasFoundRelationContext = false;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isAssumption = adminItem_.isAssumption( assumptionJustificationTypeNr );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		short justificationTypeNr = assumptionJustificationTypeNr;
		int createdRelationContextNr;
		JustificationItem createdJustificationItem = null;
		JustificationItem foundJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				// Check assumption
				if( isAssumption &&
				!hasFeminineOrMasculineProperNameEnding &&
				adminItem_.assumptionGrade( ( anotherPrimarySpecificationItem != null ), hasFeminineOrMasculineProperNameEnding, ( primarySpecificationItem != null && primarySpecificationItem.isGeneralizationProperName() && primarySpecificationItem.isPossessive() ), ( primarySpecificationItem != null && primarySpecificationItem.isQuestion() ), assumptionJustificationTypeNr ) == 0 &&

				( primarySpecificationItem == null ||

				( !primarySpecificationItem.hasAssumptionLevel() &&
				!primarySpecificationItem.isSelfGeneratedAssumption() ) ) &&

				( anotherPrimarySpecificationItem == null ||

				( !anotherPrimarySpecificationItem.hasAssumptionLevel() &&
				!anotherPrimarySpecificationItem.isSelfGeneratedAssumption() ) ) &&

				( secondarySpecificationItem == null ||

				( !secondarySpecificationItem.hasAssumptionLevel() &&
				!secondarySpecificationItem.isSelfGeneratedAssumption() ) ) )
					{
					// Conclusion
					if( adminItem_.isConclusion( conclusionJustificationTypeNr ) )
						{
						isAssumption = false;
						justificationTypeNr = conclusionJustificationTypeNr;
						}
					else
						adminItem_.startError( 1, moduleNameString_, "The given conclusion justification type isn't a conclusion" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( ( justificationResult = generalizationWordItem.addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, justificationTypeNr, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, null ) ).result == Constants.RESULT_OK )
						{
						createdJustificationItem = justificationResult.createdJustificationItem;
						foundJustificationItem = justificationResult.foundJustificationItem;

						if( ( createdOrFoundJustificationItem = ( createdJustificationItem == null ? foundJustificationItem : createdJustificationItem ) ) != null )
							{
							if( ( specificationResult = adminItem_.addSpecification( isAssignment, false, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION || justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION ), prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result == Constants.RESULT_OK )
								{
								if( !CommonVariables.hasShownWarning )
									{
									createdSpecificationItem = specificationResult.createdSpecificationItem;

									if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
										hasFoundRelationContext = foundSpecificationItem.hasRelationContext();

									if( createdSpecificationItem == null )
										{
										if( foundSpecificationItem != null )
											{
											if( foundSpecificationItem.isSelfGenerated() )
												{
												// A justification has been created, but the self-generated specification already exists
												// So, the justification needs to be added or replaced separately
												if( ( createdJustificationItem == null &&
												!foundSpecificationItem.hasFoundJustification( false, foundJustificationItem ) ) ||

												( createdJustificationItem != null &&

												( !isAssumption ||
												foundSpecificationItem.isConcludedAssumption() ||
												foundSpecificationItem.isSelfGeneratedAssumption() ||

												( secondarySpecificationItem != null &&
												secondarySpecificationItem.isUserAssignment() &&

												( foundSpecificationItem.hasCurrentCreationSentenceNr() ||

												adminItem_.nContextWordsInAllWords( foundSpecificationItem.relationContextNr(), specificationWordItem ) == 1 ) ) ) ) )
													{
													if( processJustification( hasFoundRelationContext, hasRelationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) == Constants.RESULT_OK )
														{
														if( hasRelationWord &&
														isAssumption &&
														!isPossessive &&
														generalizationWordItem.isUserRelationWord &&
														!specificationWordItem.isNounWordCollectedWithItself() )
															{
															if( generalizationWordItem.recalculateAssumptionsInWord() != Constants.RESULT_OK )
																adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														adminItem_.addError( 1, moduleNameString_, "I failed to process the created justification" );
													}
												}
											else
												adminItem_.startError( 1, moduleNameString_, "I found a user specification, whilst trying to create a self-generated specification" );
											}
										else
											adminItem_.startError( 1, moduleNameString_, "I couldn't find or create a specification" );
										}
									else
										{
										createdRelationContextNr = createdSpecificationItem.relationContextNr();

										if( !hasRelationWord ||
										!hasFoundRelationContext ||
										generalizationWordItem.hasConfirmedSpecification() )
											{
											if( hasRelationWord &&
											isGeneralizationProperName )
												{
												if( adminItem_.collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, createdRelationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
													adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );
												}

											if( !createdSpecificationItem.isHiddenSpecification() )
												{
												// Check self-generated specification for integrity
												if( generalizationWordItem.writeSelectedSpecification( true, true, false, false, Constants.NO_ANSWER_PARAMETER, createdSpecificationItem ) == Constants.RESULT_OK )
													{
													if( CommonVariables.writeSentenceStringBuffer == null ||
													CommonVariables.writeSentenceStringBuffer.length() == 0 )
														adminItem_.startError( 1, moduleNameString_, "Integrity error! I couldn't write the self-generated specification with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
													}
												else
													adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to check the writing integrity" );
												}
											}
										else
											{
											if( foundSpecificationItem != null &&
											foundSpecificationItem.relationContextNr() == createdRelationContextNr )
												adminItem_.startError( 1, moduleNameString_, "Integrity error! A specification was created in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" while this specification was found as well" );
											}
										}

									if( CommonVariables.result == Constants.RESULT_OK &&
									!isNegative &&
									!isPartOf &&
									!isSkippingAdditionalConclusionOrAskQuestion &&
									adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) &&

									( createdSpecificationItem != null ||
									createdJustificationItem != null ) )
										{
										if( isPossessive )
											{
											if( isUniqueUserRelation &&
											createdSpecificationItem != null )
												{
												// Draw a unique relation conclusion
												// Not yet explained in the design
												if( addSelfGeneratedSpecification( false, false, false, isEveryGeneralization, isExclusiveSpecification, false, true, false, true, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, createdSpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
													adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											{
											if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, false, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
												adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								}
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to create a specification" );
							}
						else
							adminItem_.startError( 1, moduleNameString_, "I couldn't find or create a justification" );
						}
					else
						adminItem_.addError( 1, moduleNameString_, "I failed to add a justification" );
					}
				}
			else
				adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.createdJustificationItem = createdJustificationItem;
		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationItem alternativeAnotherPrimarySpecificationItem()
		{
		return alternativeAnotherPrimarySpecificationItem_;
		}

	protected SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNameEnding, boolean isCheckingForPossessiveSpecification, boolean isCheckingForQuestion, boolean isNegative, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem oppositeSpecificationItem;
		SpecificationItem possessiveSpecificationItem;
		SpecificationItem questionSpecificationItem;
		WordItem userGeneralizationWordItem;
		WordItem userSpecificationWordItem;

		alternativeAnotherPrimarySpecificationItem_ = null;

		if( userSpecificationItem != null &&
		generalizationWordItem != null &&
		specificationWordItem != null )
			{
			if( generalizationWordItem.isUserRelationWord &&
			userSpecificationItem.isPossessive() )
				return userSpecificationItem;

			if( !hasGeneralizationWordFeminineProperNameEnding &&
			( userGeneralizationWordItem = userSpecificationItem.generalizationWordItem() ) != null )
				{
				// Try to find opposite specification
				oppositeSpecificationItem = userGeneralizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

				if( oppositeSpecificationItem != null &&
				!oppositeSpecificationItem.isHiddenSpecification() )
					return oppositeSpecificationItem;

				if( isCheckingForPossessiveSpecification &&
				( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
					{
					possessiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, true, Constants.NO_QUESTION_PARAMETER, userSpecificationWordItem );

					// Try to find a possessive specification
					if( possessiveSpecificationItem != null &&
					!possessiveSpecificationItem.isHiddenSpecification() )
						{
						alternativeAnotherPrimarySpecificationItem_ = adminItem_.oppositePossessiveDefinitionSpecificationItem();
						return possessiveSpecificationItem;
						}
					}

				if( isCheckingForQuestion &&
				// Try to find question
				( questionSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, specificationWordItem ) ) != null )
					return questionSpecificationItem;
				}
			}

		return null;
		}
	};

/*************************************************************************
 *	"Praise the Lord!
 *
 *	Praise God in his sanctuary;
 *	praise him in his mighty heaven!" (Psalm 150:1)
 *************************************************************************/
