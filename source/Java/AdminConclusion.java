/*
 *	Class:			AdminConclusion
 *	Supports class:	AdminItem
 *	Purpose:		To autonomously draw grammar-based conclusions
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

class AdminConclusion
	{
	// Private constructible variables

	private boolean hasFoundSpecificationWordCollectedWithItself_;

	private int missingNegativeConclusionSpecificationCollectionNr_;

	private SpecificationItem lastCreatedSpecificationSubstitutionConclusionSpecificationItem_;
	private SpecificationItem missingNegativeConclusionPrimarySpecificationItem_;
	private SpecificationItem missingNegativeConclusionAnotherPrimarySpecificationItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegativeConclusion, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasGeneralizationWordMasculineProperNameEnding;
		boolean isCommonWordCollectedWithItself;
		boolean isSpecificationCollectedWithItself;
		boolean isPossessiveUserSpecification = adminItem_.isPossessiveUserSpecification();
		int nonCompoundSpecificationCollectionNr;
		int secondarySpecificationCollectionNr;
		int specificationCollectionNr;
		SpecificationItem firstNonCompoundCollectionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
		WordItem commonWordItem;
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
								if( ( commonWordItem = secondarySpecificationWordItem.commonWordItem( secondarySpecificationCollectionNr ) ) != null )
									{
									if( ( specificationCollectionNr = commonWordItem.collectionNr( specificationWordTypeNr, specificationWordItem ) ) == Constants.NO_COLLECTION_NR )
										specificationCollectionNr = secondarySpecificationCollectionNr;

									if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ).result == Constants.RESULT_OK )
										{
										isCommonWordCollectedWithItself = commonWordItem.isNounWordCollectedWithItself();
										isSpecificationCollectedWithItself = specificationWordItem.isNounWordCollectedWithItself();
										relatedSpecificationItem = specificationResult.relatedSpecificationItem;

										// Current question part isn't related to an answer or a similar question
										if( isCommonWordCollectedWithItself &&
										!isPossessiveUserSpecification &&
										relatedSpecificationItem == null &&
										( nonCompoundSpecificationCollectionNr = commonWordItem.nonCompoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
											{
											if( nonCompoundSpecificationCollectionNr != specificationCollectionNr )
												{
												if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ).result == Constants.RESULT_OK )
													relatedSpecificationItem = specificationResult.relatedSpecificationItem;
												else
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is collected with itself" );
												}
											}

										if( relatedSpecificationItem == null )
											{
											hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
											hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem.hasMasculineProperNameEnding();

											// Negative feminine conclusion
											if( ( hasGeneralizationWordMasculineProperNameEnding &&
											secondarySpecificationWordItem.isFemale() ) ||

											// Negative masculine conclusion
											( hasGeneralizationWordFeminineProperNameEnding &&
											secondarySpecificationWordItem.isMale() ) ||

											// Specification is collected with itself
											( isSpecificationCollectedWithItself &&
											!generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() &&

											( CommonVariables.isUserQuestion ||

											( hasGeneralizationWordFeminineProperNameEnding &&
											!secondarySpecificationWordItem.isNounWordCollectedWithItself() ) ) ) )
												{
												if( !isSpecificationCollectedWithItself ||
												missingNegativeConclusionSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR )
													{
													missingNegativeConclusionSpecificationCollectionNr_ = secondarySpecificationCollectionNr;
													missingNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
													missingNegativeConclusionAnotherPrimarySpecificationItem_ = secondarySpecificationItem;
													}
												}
											else
												{
												if( isSpecificationCollectedWithItself )
													existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

												// Skip invalid questions
												if( ( !isNegativeConclusion ||

												( isSpecificationCollectedWithItself &&

												( existingSpecificationItem == null ||
												existingSpecificationItem.isHiddenSpecification() ) ) ) &&

												( ( !hasGeneralizationWordFeminineProperNameEnding &&
												!hasGeneralizationWordMasculineProperNameEnding ) ||

												primarySpecificationItem.hasSpecificationCompoundCollection() ) )
													{
													if( adminItem_.askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										else
											{
											if( secondarySpecificationItem.isExclusiveSpecification() )
												{
												if( !isCommonWordCollectedWithItself &&
												( firstNonCompoundCollectionSpecificationItem = generalizationWordItem.firstNonCompoundCollectionSpecificationItem( secondarySpecificationCollectionNr ) ) != null )
													relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

												if( missingNegativeConclusionSpecificationCollectionNr_ == secondarySpecificationCollectionNr )
													{
													if( drawNegativeConclusion( isArchivedAssignment, false, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, missingNegativeConclusionPrimarySpecificationItem_, missingNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, generalizationWordItem ) == Constants.RESULT_OK )
														{
														missingNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
														missingNegativeConclusionPrimarySpecificationItem_ = null;
														missingNegativeConclusionAnotherPrimarySpecificationItem_ = null;
														}
													else
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												if( drawNegativeConclusion( isArchivedAssignment, isSpecificationCollectedWithItself, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									else
										return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the common word of the given specification collection number" );
								}
							else
								return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item has no specification word item" );
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item has no specification collection" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given primary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte drawNegativeConclusion( boolean isArchivedAssignment, boolean isSpecificationCollectedWithItself, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isGeneralizationWordUserGeneralizationWord;
		boolean isPrimarySameAsUserSpecification;
		boolean isPrimarySpecificationHidden;
		boolean isSelectingUserSpecification = false;
		boolean isUserSpecificationCollectedWithItself = false;
		int anotherPrimarySpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem negativeAssumptionOrConclusionJustificationItem;
		JustificationItem questionJustificationItem;
		SpecificationItem conclusionPrimarySpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingNegativeSpecificationItem;
		SpecificationItem existingPositiveSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem foundNegativeAssumptionOrConclusionPrimarySpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem = null;
		SpecificationItem lastFoundOppositePossessiveDefinitionSpecificationItem = null;
		SpecificationItem userSpecificationItem = null;
		WordItem primarySpecificationWordItem;
		WordItem anotherPrimarySpecificationWordItem;
		WordItem secondarySpecificationWordItem;
		WordItem userSpecificationWordItem;

		if( primarySpecificationItem != null )
			{
			if( anotherPrimarySpecificationItem != null )
				{
				if( secondarySpecificationItem != null )
					{
					if( generalizationWordItem != null )
						{
						if( !primarySpecificationItem.isPossessive() &&
						!primarySpecificationItem.isQuestion() )
							{
							if( ( primarySpecificationWordItem = primarySpecificationItem.specificationWordItem() ) != null )
								{
								if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
									{
									if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem.specificationWordItem() ) != null )
										{
										// If exists, replace self-generated assumption for conclusion
										if( primarySpecificationItem.isSelfGeneratedAssumption() &&
										( conclusionPrimarySpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, primarySpecificationItem.isArchivedAssignment(), primarySpecificationItem.isNegative(), primarySpecificationItem.isPossessive(), false, primarySpecificationWordItem, null ) ) != null )
											primarySpecificationItem = conclusionPrimarySpecificationItem;

										isPrimarySpecificationHidden = primarySpecificationItem.isHiddenSpecification();
										existingPositiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
										foundNegativeAssumptionOrConclusionPrimarySpecificationItem = null;

										if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != null )
											foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem.primarySpecificationItem();

										// No positive specification exists with this specification word
										if( ( existingPositiveSpecificationItem == null ||
										existingPositiveSpecificationItem.isHiddenSpecification() ) &&

										// Prefer primary specification with the least uncertainty
										( !primarySpecificationItem.hasSpecificationCollection() ||
										foundNegativeAssumptionOrConclusionPrimarySpecificationItem == null ||
										primarySpecificationItem.assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem.assumptionLevel() ) &&

										// No question exists with this specification word
										generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) == null )
											{
											if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
												{
												isPrimarySameAsUserSpecification = ( primarySpecificationWordItem == secondarySpecificationWordItem );
												isGeneralizationWordUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

												if( !isPrimarySameAsUserSpecification )
													anotherSecondarySpecificationItem = secondarySpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null );

												if( isGeneralizationWordUserGeneralizationWord &&
												( tempSpecificationItem = adminItem_.userSpecificationItem() ) != null )
													{
													if( ( userSpecificationWordItem = tempSpecificationItem.specificationWordItem() ) != null )
														{
														if( userSpecificationWordItem.isNounWordCollectedWithItself() )
															isUserSpecificationCollectedWithItself = true;

														if( !isPrimarySpecificationHidden ||

														( isUserSpecificationCollectedWithItself &&
														!tempSpecificationItem.isHiddenSpecification() ) )
															{
															userSpecificationItem = tempSpecificationItem;
															lastFoundOppositePossessiveDefinitionSpecificationItem = adminItem_.lastFoundOppositePossessiveDefinitionSpecificationItem();

															if( anotherSecondarySpecificationItem != null &&
															userSpecificationItem.hasSpecificationCompoundCollection() &&
															userSpecificationWordItem.hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
																isSelectingUserSpecification = true;
															}
														else
															{
															if( isSpecificationCollectedWithItself &&
															!anotherPrimarySpecificationWordItem.isNounWordCollectedWithItself() )
																userSpecificationItem = adminItem_.simpleUserSpecificationItem();
															}
														}
													}

												existingNegativeSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, true, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );

												if( isSelectingUserSpecification ||
												!primarySpecificationItem.hasRelationContext() ||

												( isPrimarySpecificationHidden &&
												existingNegativeSpecificationItem == null ) ||

												( ( !isPrimarySpecificationHidden ||

												( isGeneralizationWordUserGeneralizationWord &&

												( existingNegativeSpecificationItem == null ||
												!existingNegativeSpecificationItem.isConcludedAssumption() ) ) ) &&

												generalizationWordItem.olderPrimarySpecificationJustificationItem( false, false, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_COLLECTION_NR, primarySpecificationItem ) == null ) )
													{
													// Draw a negative conclusion
													// Not yet explained in the design
													if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isExclusiveSpecification(), false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isArchivedAssignment && isUserSpecificationCollectedWithItself ? Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, ( isPrimarySpecificationHidden ? ( userSpecificationItem == null ? anotherPrimarySpecificationItem : userSpecificationItem ) : ( isPrimarySameAsUserSpecification ? null : primarySpecificationItem ) ), ( isPrimarySpecificationHidden ? lastFoundOppositePossessiveDefinitionSpecificationItem : anotherPrimarySpecificationItem ), ( isPrimarySpecificationHidden && userSpecificationItem != null ? anotherPrimarySpecificationItem : ( isSelectingUserSpecification ? userSpecificationItem : secondarySpecificationItem ) ), ( isPrimarySpecificationHidden && userSpecificationItem != null ? secondarySpecificationItem : ( isSelectingUserSpecification ? null : anotherSecondarySpecificationItem ) ), generalizationWordItem, anotherPrimarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
														{
														createdJustificationItem = specificationResult.createdJustificationItem;

														// Found specification
														if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
															{
															if( createdJustificationItem != null )
																{
																// Cleanup after found specification
																if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
																	{
																	if( negativeAssumptionOrConclusionJustificationItem == null )
																		{
																		if( foundSpecificationItem.isSelfGeneratedAssumption() )
																			{
																			if( createdJustificationItem.isConclusionJustification() )
																				{
																				// Replace justification of negative conclusion / assumption
																				if( foundSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
																					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
																				}
																			}
																		else
																			{
																			if( !generalizationWordItem.isJustificationInUse( createdJustificationItem ) )
																				{
																				if( generalizationWordItem.replaceOrDeleteJustification( createdJustificationItem ) != Constants.RESULT_OK )
																					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the created justification" );
																				}
																			}
																		}
																	else
																		{
																		// Replace justification of negative conclusion / assumption
																		if( generalizationWordItem.replaceJustification( negativeAssumptionOrConclusionJustificationItem, createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
																			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	}
																else
																	return adminItem_.startErrorInItem( 1, moduleNameString_, "The found specification item is undefined" );
																}
															}
														else
															{
															// Created specification
															// Replace corrected negative conclusion
															if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() )
																{
																if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, createdSpecificationItem ) ).result == Constants.RESULT_OK )
																	{
																	relatedSpecificationItem = specificationResult.relatedSpecificationItem;

																	if( relatedSpecificationItem != null &&
																	relatedSpecificationItem.isOlderItem() )
																		{
																		if( generalizationWordItem.replaceOrDeleteSpecification( relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	}
																else
																	return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created negative conclusion" );
																}
															else
																{
																// Replace question justification
																if( anotherSecondarySpecificationItem != null &&
																anotherPrimarySpecificationItem.hasSpecificationCompoundCollection() )
																	{
																	if( ( questionJustificationItem = generalizationWordItem.olderSecondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, anotherPrimarySpecificationItem ) ) == null )
																		{
																		if( isSelectingUserSpecification )
																			{
																			if( generalizationWordItem.changeJustificationOfNegativeAssumptions( userSpecificationItem ) != Constants.RESULT_OK )
																				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a question justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																			}
																		}
																	else
																		{
																		if( generalizationWordItem.replaceJustification( questionJustificationItem, createdJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a question justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	}
																}
															}
														}
													else
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + anotherPrimarySpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												return adminItem_.startErrorInItem( 1, moduleNameString_, "The given related specification item has no specification word item" );
											}
										}
									else
										return adminItem_.startErrorInItem( 1, moduleNameString_, "The given another primary specification item has no specification word item" );
									}
								else
									return adminItem_.startErrorInItem( 1, moduleNameString_, "The given another primary specification item has no specification collection" );
								}
							else
								return adminItem_.startErrorInItem( 1, moduleNameString_, "The given primary specification item has no specification word item" );
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The given primary specification item shouldn't be possessive, nor a question, nor a hidden specification" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given another primary specification item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given primary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte drawPossessiveReversibleConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUniqueUserRelation, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem secondarySpecificationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		SpecificationResultType specificationResult;
		boolean isConclusionAssignment;
		boolean isForcingNewJustification;
		boolean isSecondarySpecificationWordCollectedWithItself;
		boolean hasFeminineProperNameEnding = false;
		boolean isAssumption = ( userAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL );
		int foundRelationContextNr;
		int nContextRelations;
		int generalizationContextNr = Constants.NO_CONTEXT_NR;
		int specificationCollectionNr = Constants.NO_COLLECTION_NR;
		GeneralizationItem currentGeneralizationItem;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem conclusionSpecificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem collectedWithItselfCommonWordItem;
		WordItem currentGeneralizationWordItem;

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			if( generalizationWordItem != null )
				{
				if( secondarySpecificationWordItem != null )
					{
					if( specificationWordItem != null )
						{
						if( relationWordItem != null )
							{
							existingSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

							if( existingSpecificationItem == null ||
							// Allow existing conclusion specification to change its relation context
							existingSpecificationItem.isSelfGenerated() )
								{
								if( ( foundRelationContextNr = generalizationWordItem.contextNr( specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
									{
									if( ( nContextRelations = adminItem_.nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) ) > 1 )
										{
										if( ( contextResult = adminItem_.getRelationContext( isArchivedAssignment, isNegative, !isPossessive, false, false, specificationWordTypeNr, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, null ) ).result == Constants.RESULT_OK )
											generalizationContextNr = contextResult.contextNr;
										else
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get the relation context number" );
										}
									else
										{
										if( existingSpecificationItem != null )
											generalizationContextNr = foundRelationContextNr;
										}
									}

								isSecondarySpecificationWordCollectedWithItself = secondarySpecificationWordItem.isNounWordCollectedWithItself();

								conclusionSpecificationItem = contextResult.conclusionSpecificationItem;
								existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Find the non-reversible specification (active assignments)
								if( isSecondarySpecificationWordCollectedWithItself )
									secondarySpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );
								else
									secondarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Skip if possessive user specification exists
								if( secondarySpecificationItem != null &&

								( existingUserSpecificationItem == null ||
								!existingUserSpecificationItem.hasRelationContext() ||
								// Allow exclusive assignments
								existingUserSpecificationItem.isExclusiveSpecification() ) )
									{
									if( secondarySpecificationItem.hasNewInformation() ||
									secondarySpecificationItem.assumptionLevel() != userAssumptionLevel )
										{
										if( isPossessive &&
										isSecondarySpecificationWordCollectedWithItself &&
										relationWordItem.hasFeminineProperNameEnding() )
											hasFeminineProperNameEnding = true;

										isConclusionAssignment = ( ( isArchivedAssignment &&
																	isAssumption ) ||

																	( !isAssumption &&

																	( isArchivedAssignment ||
																	secondarySpecificationItem.hasExclusiveGeneralizationCollection() ) ) );

										isForcingNewJustification = ( isSecondarySpecificationWordCollectedWithItself ||

																	( !isPossessive &&
																	existingSpecificationItem != null &&

																	( conclusionSpecificationItem != null ||
																	existingSpecificationItem.relationContextNr() != generalizationContextNr ||

																	( secondarySpecificationItem.isActiveAssignment() &&
																	existingSpecificationItem.secondarySpecificationJustificationItem( secondarySpecificationItem ) == null ) ) ) );

										if( isSecondarySpecificationWordCollectedWithItself &&
										secondarySpecificationWordItem == specificationWordItem &&

										( !isPossessive ||
										!relationWordItem.isMale() ) )
											specificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

										// Draw a possessive reversible conclusion
										// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
										if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( hasFeminineProperNameEnding, isConclusionAssignment, isArchivedAssignment, false, isExclusiveSpecification, isForcingNewJustification, isNegative, false, !isPossessive, isUniqueUserRelation, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, specificationCollectionNr, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, null, null, secondarySpecificationItem, null, relationWordItem, specificationWordItem, generalizationWordItem ) ).result == Constants.RESULT_OK )
											{
											if( conclusionSpecificationItem == null )
												{
												if( ( createdOrFoundSpecificationItem = ( specificationResult.createdSpecificationItem == null ? specificationResult.foundSpecificationItem : specificationResult.createdSpecificationItem ) ) != null )
													{
													if( isPossessive )
														{
														if( adminItem_.makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, false, true, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, createdOrFoundSpecificationItem, null, generalizationWordItem, relationWordItem ) == Constants.RESULT_OK )
															{
															if( ( collectedWithItselfCommonWordItem = createdOrFoundSpecificationItem.collectedWithItselfCommonWordItem() ) != null )
																{
																// If the reversed specification doesn't already exist
																if( generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) == null )
																	{
																	// Generate the reversed specification
																	if( adminItem_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isNegative, !isPossessive, false, createdOrFoundSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdOrFoundSpecificationItem.relationContextNr(), relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
																	}

																if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem.firstNounSpecificationGeneralizationItem() ) != null )
																	{
																	do	{
																		if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
																			{
																			if( ( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isConclusionAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) ) != null )
																				{
																				if( foundSpecificationItem.isPartOf() &&

																				( !createdOrFoundSpecificationItem.hasAssumptionLevel() ||
																				// Skip if 'part of' conclusion exists
																				relationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, false, false, currentGeneralizationWordItem, null ) == null ) )
																					{
																					// Draw a 'part of' generalization conclusion
																					// Not yet explained in the design
																					if( adminItem_.addSelfGeneratedSpecification( false, isConclusionAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdOrFoundSpecificationItem, null, foundSpecificationItem, null, relationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
																						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
																					}
																				}
																			}
																		else
																			return adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an undefined generalization word" );
																		}
																	while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
																	}

																if( ( primarySpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem, generalizationWordItem ) ) != null &&
																( secondarySpecificationItem = collectedWithItselfCommonWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, createdOrFoundSpecificationItem.specificationCollectionNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
																	{
																	if( drawNegativeConclusion( isArchivedAssignment, false, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, secondarySpecificationItem, createdOrFoundSpecificationItem, relationWordItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a negative conclusion about relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															}
														else
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														if( secondarySpecificationItem.isUserSpecification() &&
														createdOrFoundSpecificationItem.isOlderItem() &&
														createdOrFoundSpecificationItem.isSelfGeneratedAssumption() &&
														adminItem_.nContextWordsInAllWords( createdOrFoundSpecificationItem.relationContextNr(), specificationWordItem ) == 1 )
															{
															if( createdOrFoundSpecificationItem.markAsConcludedAssumption() == Constants.RESULT_OK )
																{
																if( relationWordItem.writeUpdatedSpecification( true, false, false, createdOrFoundSpecificationItem ) != Constants.RESULT_OK )
																	return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the concluded assumption in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
																}
															else
																return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
															}
														}

													if( generalizationWordItem.isUserGeneralizationWord &&
													specificationWordItem.isUserSpecificationWord )
														{
														if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, !isPossessive, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													return adminItem_.startErrorInItem( 1, moduleNameString_, "No specification item was found or created" );
												}
											else
												{
												if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
													{
													if( foundSpecificationItem.isSelfGeneratedAssumption() )
														{
														if( foundSpecificationItem.markAsConcludedAssumption() == Constants.RESULT_OK )
															{
															// Write adjusted specification
															if( relationWordItem.writeUpdatedSpecification( true, false, false, foundSpecificationItem ) == Constants.RESULT_OK )
																{
																if( ( firstJustificationItem = conclusionSpecificationItem.firstJustificationItem() ) != null )
																	{
																	if( ( createdJustificationItem = specificationResult.createdJustificationItem ) != null )
																		{
																		// Attach older conclusion justifications to the created conclusion justification item
																		if( createdJustificationItem.changeAttachedJustification( firstJustificationItem ) == Constants.RESULT_OK )
																			{
																			if( relationWordItem.replaceOrDeleteSpecification( conclusionSpecificationItem, foundSpecificationItem ) == Constants.RESULT_OK )
																				{
																				firstJustificationItem = foundSpecificationItem.firstJustificationItem();

																				if( firstJustificationItem != null &&
																				!firstJustificationItem.isPossessiveReversibleConclusion() )
																					{
																					if( firstJustificationItem.changeAttachedJustification( createdJustificationItem ) != Constants.RESULT_OK )
																						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach older conclusion justification items to the created conclusion justification item" );
																					}

																				while( ( obsoleteJustificationItem = relationWordItem.obsoleteJustificationItem() ) != null )
																					{
																					if( relationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
																						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
																					}
																				}
																			else
																				return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace or delete a specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
																			}
																		else
																			return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach older conclusion justification items to the created conclusion justification item" );
																		}
																	else
																		return adminItem_.startErrorInItem( 1, moduleNameString_, "The created conclusion justification item is undefined" );
																	}
																else
																	return adminItem_.startErrorInItem( 1, moduleNameString_, "The conclusion specification item has no justifications" );
																}
															else
																return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an adjusted specification in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
															}
														else
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to mark the replacing specification as concluded assumption in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find an older specification to replace" );
												}
											}
										else
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							}
						else
							return adminItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given relation context number is undefined" );

		return Constants.RESULT_OK;
		}

	private byte drawSpecificationGeneralizationConclusion( short assumptionLevel, SpecificationItem secondarySpecificationItem )
		{
		boolean isArchivedAssignment;

		if( secondarySpecificationItem != null )
			{
			if( secondarySpecificationItem.isGeneralizationNoun() &&
			secondarySpecificationItem.isSpecificationNoun() )
				{
				secondarySpecificationItem = secondarySpecificationItem.updatedSpecificationItem();
				isArchivedAssignment = secondarySpecificationItem.isArchivedAssignment();

				// Draw a specification generalization conclusion
				// Not yet explained in the design
				if( adminItem_.addSelfGeneratedSpecification( false, isArchivedAssignment, isArchivedAssignment, false, secondarySpecificationItem.isExclusiveSpecification(), false, secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPartOf(), secondarySpecificationItem.isPossessive(), false, false, true, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem.generalizationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, secondarySpecificationItem.specificationWordItem(), secondarySpecificationItem.generalizationWordItem(), null ).result != Constants.RESULT_OK )
					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminConclusion( AdminItem adminItem )
		{
		String errorString = null;

		hasFoundSpecificationWordCollectedWithItself_ = false;

		missingNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = null;
		missingNegativeConclusionPrimarySpecificationItem_ = null;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = null;

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

	protected void initializeAdminConclusionVariables()
		{
		hasFoundSpecificationWordCollectedWithItself_ = false;

		missingNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = null;
		missingNegativeConclusionPrimarySpecificationItem_ = null;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = null;
		}

	protected boolean hasFoundUnprocessedNegativeConclusion()
		{
		return ( missingNegativeConclusionSpecificationCollectionNr_ > Constants.NO_COLLECTION_NR );
		}

	protected byte drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		JustificationItem currentJustificationItem;
		SpecificationItem answeredQuestionSpecificationItem;
		WordItem answeredQuestionSpecificationWordItem;
		WordItem userSpecificationWordItem;

		if( userSpecificationItem != null )
			{
			if( generalizationWordItem != null )
				{
				if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
					{
					if( ( answeredQuestionSpecificationItem = generalizationWordItem.firstRecentlyAnsweredQuestionSpecificationItem( Constants.WORD_PARAMETER_SINGULAR_VERB_IS ) ) != null )
						{
						do	{
							if( userSpecificationWordItem.hasCollectionNr( answeredQuestionSpecificationItem.specificationCollectionNr() ) )
								{
								if( ( answeredQuestionSpecificationWordItem = answeredQuestionSpecificationItem.specificationWordItem() ) != null )
									{
									// Check if already exists as positive specification
									if( generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == null )
										{
										if( ( currentJustificationItem = answeredQuestionSpecificationItem.firstJustificationItem() ) != null )
											{
											// Do for all justifications
											do	{
												if( !currentJustificationItem.hasPrimaryQuestion() &&
												!currentJustificationItem.hasPossessivePrimarySpecification() )
													{
													if( drawNegativeConclusion( answeredQuestionSpecificationItem.isArchivedAssignment(), false, answeredQuestionSpecificationItem.generalizationWordTypeNr(), answeredQuestionSpecificationItem.specificationWordTypeNr(), answeredQuestionSpecificationItem.generalizationContextNr(), answeredQuestionSpecificationItem.specificationContextNr(), currentJustificationItem.primarySpecificationItem(), currentJustificationItem.secondarySpecificationItem(), userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											while( ( currentJustificationItem = currentJustificationItem.attachedJustificationItem() ) != null );
											}
										else
											return adminItem_.startErrorInItem( 1, moduleNameString_, "The answered question specification item has no justification" );
										}
									}
								else
									return adminItem_.startErrorInItem( 1, moduleNameString_, "The answered question specification item has no specification word" );
								}
							}
						while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem.recentlyAnsweredQuestionSpecificationItem( false ) ) != null );
						}
					else
						return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any answered question specification" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item has no specification word" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, WordItem generalizationWordItem )
		{
		boolean hasFoundAssignment;
		boolean hasFoundMoreOptionsLeft = false;
		SpecificationItem currentAssignmentItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem firstNegativeOnlyOptionLeftAssignmentItem = null;
		SpecificationItem firstOnlyOptionLeftSpecificationItem = null;
		SpecificationItem foundOnlyOptionLeftSpecificationItem = null;
		WordItem specificationWordItem;

		if( generalizationWordItem != null )
			{
			if( specificationCollectionNr > Constants.NO_COLLECTION_NR )
				{
				if( ( currentSpecificationItem = generalizationWordItem.firstNonQuestionSpecificationItem() ) != null )
					{
					firstOnlyOptionLeftSpecificationItem = currentSpecificationItem;

					do	{
						if( currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
						( currentAssignmentItem = generalizationWordItem.firstSelectedSpecificationItem( true, isInactiveAssignment, isArchivedAssignment, false ) ) != null )
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
									// There are more options left
									hasFoundMoreOptionsLeft = true;
									// So, clear the found option
									foundOnlyOptionLeftSpecificationItem = null;
									}
								}
							}
						}
					while( !hasFoundMoreOptionsLeft &&
					( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );

					if( firstNegativeOnlyOptionLeftAssignmentItem != null &&
					foundOnlyOptionLeftSpecificationItem != null )
						{
						// Draw an only option left conclusion
						// Not yet explained in the design
						if( adminItem_.addSelfGeneratedSpecification( false, true, isArchivedAssignment, false, true, false, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification collection number is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte drawPossessiveReversibleConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUniqueUserRelation, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		WordItem feminineSpecificationWordItem;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				if( relationWordItem != null )
					{
					if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, specificationWordItem, relationWordItem ) == Constants.RESULT_OK )
						{
						if( isPossessive &&
						specificationWordItem.isNounWordCollectedWithItself() &&
						relationWordItem.hasFeminineProperNameEnding() &&
						( feminineSpecificationWordItem = specificationWordItem.feminineCollectionWordItem() ) != null )
							{
							if( drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, feminineSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a feminine possessive reversible conclusion" );
							}
						}
					else
						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion" );
					}
				else
					return adminItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		short assumptionLevel;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem = null;
		WordItem currentGeneralizationWordItem;

		if( secondarySpecificationItem != null )
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
								( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
									{
									if( !foundSpecificationItem.hasSpecificationCollection() &&
									!foundSpecificationItem.isExclusiveSpecification() )
										{
										assumptionLevel = ( secondarySpecificationItem.assumptionLevel() < foundSpecificationItem.assumptionLevel() ? foundSpecificationItem.assumptionLevel() : secondarySpecificationItem.assumptionLevel() );

										if( adminItem_.collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
											{
											if( drawSpecificationGeneralizationConclusion( assumptionLevel, foundSpecificationItem ) == Constants.RESULT_OK )
												{
												if( drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != Constants.RESULT_OK )
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
												}
											else
												return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );
											}
										else
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );
										}
									}
								}
							else
								return adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an undefined generalization word" );
							}
						while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
						}
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

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasCorrectedAssumptionByKnowledge;
		boolean hasCorrectedAssumptionByOppositeQuestion;
		boolean hasGeneralizationWordActiveQuestionWithCompoundCollection;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasGeneralizationWordMasculineProperNameEnding;
		boolean isIncludingRelationWord;
		boolean isExclusiveCurrentSpecification;
		boolean isFemaleOrMaleGeneralizationWord;
		boolean isFoundPrimaryHiddenSpecification;
		boolean isGeneralizationWordUserGeneralizationWord;
		boolean isSpecificationCollectedWithItself;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		boolean isDefinitionSpecification = adminItem_.isSingularOrPluralNoun( generalizationWordTypeNr );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		boolean isGeneralizationWordUserRelationWord;
		boolean isNegativeConclusion = false;
		boolean isPossessiveUserSpecification = adminItem_.isPossessiveUserSpecification();
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		int currentRelationContextNr;
		int currentSpecificationCollectionNr;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem alternativeAnotherPrimarySpecificationItem;
		SpecificationItem alternativeJustificationSpecificationItem;
		SpecificationItem bestMatchingRelationSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem feminineOrMasculineSpecificationItem;
		SpecificationItem foundPrimarySpecificationItem;
		SpecificationItem lastFoundOppositePossessiveDefinitionSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem collectedWithItselfSpecificationItem = null;
		SpecificationItem userSpecificationItem = adminItem_.userSpecificationItem();
		WordItem currentSpecificationWordItem;

		if( adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
				{
				if( generalizationWordItem != null )
					{
					if( specificationWordItem != null )
						{
						hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();
						isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
						isSpecificationCollectedWithItself = specificationWordItem.isNounWordCollectedWithItself();
						isGeneralizationWordUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

						// Skip definition sentence with the same generalization and specification word
						if( !hasFoundSpecificationWordCollectedWithItself_ ||
						!hasCorrectedAssumptionByKnowledge )
							{
							if( adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
								{
								if( isSpecificationCollectedWithItself )
									hasFoundSpecificationWordCollectedWithItself_ = true;
								}
							else
								return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}

						if( ( currentSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) != null )
							{
							hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();

							hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem.hasActiveQuestionWithCompoundCollection();
							hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
							hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem.hasMasculineProperNameEnding();

							lastFoundOppositePossessiveDefinitionSpecificationItem = adminItem_.lastFoundOppositePossessiveDefinitionSpecificationItem();

							isFemaleOrMaleGeneralizationWord = generalizationWordItem.isFemaleOrMale();

							if( isSpecificationCollectedWithItself )
								collectedWithItselfSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

							do	{
								if( currentSpecificationItem.isSpecificationNoun() &&
								!currentSpecificationItem.isSpecificationGeneralization() &&
								currentSpecificationItem.isNegative() == isNegative &&
								currentSpecificationItem.isPossessive() == isPossessive &&
								( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
									{
									isExclusiveCurrentSpecification = currentSpecificationItem.isExclusiveSpecification();
									currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
									currentRelationContextNr = currentSpecificationItem.relationContextNr();

									if( ( bestMatchingRelationSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) != null )
										{
										if( !isQuestion &&
										!bestMatchingRelationSpecificationItem.isExclusiveSpecification() )
											isNegativeConclusion = true;
										}

									if( ( foundPrimarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( ( isAssignment || hasCorrectedAssumptionByOppositeQuestion ), isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == null )
										{
										// Check for more conflicts
										if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), currentRelationContextNr, currentSpecificationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for a specification conflict in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									else
										{
										isFoundPrimaryHiddenSpecification = foundPrimarySpecificationItem.isHiddenSpecification();

										if( !hasGeneralizationWordActiveQuestionWithCompoundCollection &&
										isExclusiveCurrentSpecification &&
										!isFemaleOrMaleGeneralizationWord &&

										( !isFoundPrimaryHiddenSpecification ||
										isGeneralizationWordUserGeneralizationWord ) &&

										// Matching feminine word
										( ( hasGeneralizationWordFeminineProperNameEnding &&
										currentSpecificationWordItem.isFemale() ) ||

										// Matching masculine word
										( hasGeneralizationWordMasculineProperNameEnding &&
										currentSpecificationWordItem.isMale() ) ) &&

										// Select correct compound word to avoid assumption to be made if a question should be asked
										currentSpecificationWordItem.compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem &&
										// Skip creating feminine or masculine proper name ending assumption if conclusion already exists
										generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, false, false, false, currentSpecificationWordItem, null ) == null )
											{
											if( ( feminineOrMasculineSpecificationItem = currentSpecificationWordItem.firstFeminineOrMasculineSpecificationItem() ) != null )
												{
												isIncludingRelationWord = ( hasRelationWord &&
																			feminineOrMasculineSpecificationItem.hasSpecificationCollection() );

												// Make a feminine or masculine proper name ending assumption
												// Not yet explained in the design
												if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION, Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION, foundPrimarySpecificationItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( isIncludingRelationWord ? relationWordTypeNr : Constants.WORD_TYPE_UNDEFINED ), Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, ( isFoundPrimaryHiddenSpecification ? userSpecificationItem : foundPrimarySpecificationItem ), ( isFoundPrimaryHiddenSpecification ? lastFoundOppositePossessiveDefinitionSpecificationItem : null ), currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, ( isIncludingRelationWord ? relationWordItem : null ) ) ).result == Constants.RESULT_OK )
													{
													if( isIncludingRelationWord &&
													( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
														{
														if( drawPossessiveReversibleConclusion( isArchivedAssignment, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, createdSpecificationItem.relationContextNr(), generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														if( specificationResult.foundSpecificationItem != null &&
														( obsoleteJustificationItem = generalizationWordItem.obsoleteJustificationItem() ) != null )
															{
															if( generalizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
																return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}

													if( !isSpecificationCollectedWithItself &&
													// Has missed drawing some negative conclusions
													missingNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr )
														{
														// Draw missing negative conclusions
														if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
												}
											else
												return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find a feminine or masculine specification item in current specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
											}

										if( !isExclusiveCurrentSpecification ||
										bestMatchingRelationSpecificationItem == null )
											{
											if( isGeneralizationProperName ||
											currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR )
												{
												if( ( !isExclusiveSpecification &&
												isExclusiveCurrentSpecification ) ||

												( isGeneralizationProperName &&

												( ( isExclusiveSpecification &&
												currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR ) ||

												( isSpecificationCollectedWithItself &&
												!isFoundPrimaryHiddenSpecification &&

												( ( isExclusiveSpecification &&

												( isGeneralizationWordUserRelationWord ||
												isPossessiveUserSpecification ) ) ||

												( isGeneralizationWordUserRelationWord &&
												isPossessiveUserSpecification &&
												foundPrimarySpecificationItem.hasSpecificationNonCompoundCollection() ) ) ) ) ) )
													{
													// Ask a question or draw a negative conclusion
													if( isGeneralizationProperName &&
													!isNegative &&
													!isPossessive &&
													currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

													( adminItem_.hasFoundSuggestiveQuestionAssumption() ||
													generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() ||

													( !isSpecificationCollectedWithItself &&

													( !hasGeneralizationWordActiveQuestionWithCompoundCollection ||
													foundPrimarySpecificationItem.hasSpecificationCollection() ||
													adminItem_.isExclusiveUserSpecification() ||
													// Specification collected with itself
													currentSpecificationWordItem.compoundCollectionNr( specificationWordTypeNr ) == Constants.NO_COLLECTION_NR ) ) ||

													// Skip invalid compound feminine/masculine questions,
													// but allow those questions to be adjusted
													( isSpecificationCollectedWithItself &&

													( !isExclusiveCurrentSpecification ||
													isNegativeConclusion ||

													( foundPrimarySpecificationItem.hasRelationContext() &&
													foundPrimarySpecificationItem.hasSpecificationNonCompoundCollection() ) ) ) ) )
														{
														if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isExclusiveSpecification, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundPrimarySpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													{
													// Make an assumption or draw a conclusion
													if( isAssumption ||
													currentSpecificationItem.hasAssumptionLevel() )
														{
														alternativeAnotherPrimarySpecificationItem = null;
														primarySpecificationItem = null;

														if( isArchivedAssignment )
															primarySpecificationItem = foundPrimarySpecificationItem;
														else
															{
															if( isSpecificationCollectedWithItself ||

															( primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) == null )
																primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );
															}

														if( primarySpecificationItem != null )
															{
															alternativeJustificationSpecificationItem = ( hasRelationWord || isSpecificationCollectedWithItself ? adminItem_.alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, isNegative, isPossessive, userSpecificationItem, generalizationWordItem, specificationWordItem ) : null );
															existingSpecificationItem = ( hasRelationWord ? generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, currentSpecificationWordItem ) : null );

															if( ( isSpecificationCollectedWithItself &&
															primarySpecificationItem.isHiddenSpecification() ) ||

															( existingSpecificationItem != null &&
															alternativeJustificationSpecificationItem != null &&
															alternativeJustificationSpecificationItem.specificationWordItem() == specificationWordItem &&
															existingSpecificationItem.primarySpecificationJustificationItem( primarySpecificationItem ) != null ) )
																{
																primarySpecificationItem = alternativeJustificationSpecificationItem;

																if( alternativeJustificationSpecificationItem != null )
																	alternativeAnotherPrimarySpecificationItem = adminItem_.alternativeAnotherPrimarySpecificationItem();
																}
															else
																{
																if( userSpecificationItem != null &&
																userSpecificationItem.isPossessive() &&
																primarySpecificationItem.hasAssumptionLevel() )
																	{
																	existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, currentSpecificationWordItem );

																	if( existingSpecificationItem != null &&
																	existingSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null )
																		// Skip if similar justification already exists, without relation word
																		primarySpecificationItem = null;
																	}
																}
															}

														if( primarySpecificationItem != null &&

														( !isSpecificationCollectedWithItself ||
														foundPrimarySpecificationItem.isUserSpecification() ||

														( currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR ||
														CommonVariables.isUserQuestion ) ) &&

														// Skip if already exists as user specification
														generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null )
															{
															// Make a compound specification substitution assumption, which is actually an uncertain conclusion
															// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
															//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
															if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, isSpecificationCollectedWithItself, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, alternativeAnotherPrimarySpecificationItem, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
																{
																if( specificationResult.foundSpecificationItem != null &&
																( obsoleteJustificationItem = generalizationWordItem.obsoleteJustificationItem() ) != null )
																	{
																	if( generalizationWordItem.replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a suggestive question specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														{
														if( ( !isExclusiveSpecification &&

														( !isSpecificationCollectedWithItself ||

														( collectedWithItselfSpecificationItem != null &&
														collectedWithItselfSpecificationItem.hasCurrentCreationSentenceNr() &&

														( !isFoundPrimaryHiddenSpecification ||
														CommonVariables.nUserRelationWords == 1 ) ) ) &&

														// Skip if already exists as user specification
														generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) ||

														// Check if exclusive conclusion already exists
														( isExclusiveSpecification &&
														generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isAssignment, isArchivedAssignment, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) )
															{
															primarySpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, isArchivedAssignment, isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

															if( primarySpecificationItem != null &&
															primarySpecificationItem.isHiddenSpecification() &&
															( primarySpecificationItem = adminItem_.alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, true, false, isNegative, isPossessive, userSpecificationItem, generalizationWordItem, specificationWordItem ) ) != null )
																alternativeAnotherPrimarySpecificationItem = adminItem_.alternativeAnotherPrimarySpecificationItem();

															if( primarySpecificationItem != null )
																{
																if( !primarySpecificationItem.isPartOf() )
																	{
																	if( lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ != null &&
																	lastCreatedSpecificationSubstitutionConclusionSpecificationItem_.generalizationWordItem() == generalizationWordItem )
																		currentSpecificationCollectionNr = lastCreatedSpecificationSubstitutionConclusionSpecificationItem_.specificationCollectionNr();

																	// Draw a compound specification substitution conclusion
																	// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
																	//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
																	if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, primarySpecificationItem.isEveryGeneralization(), isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem.specificationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
																		{
																		if( ( lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = specificationResult.createdSpecificationItem ) != null )
																			{
																			if( hasCorrectedAssumptionByKnowledge )
																				{
																				if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ ) ).result == Constants.RESULT_OK )
																					{
																					if( specificationResult.relatedSpecificationItem != null )
																						{
																						// Replace incorrect assumption
																						if( generalizationWordItem.replaceOrDeleteSpecification( specificationResult.relatedSpecificationItem, lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ ) != Constants.RESULT_OK )
																							return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a corrected specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																						}
																					}
																				else
																					return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created specification" );
																				}
																			else
																				{
																				if( !isGeneralizationProperName )
																					{
																					if( adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, isNegative, isPossessive, currentSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																						return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption in specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
																					}
																				}
																			}
																		}
																	else
																		return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );
															}
														}
													}

												if( !CommonVariables.hasShownWarning &&
												currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

												( !isExclusiveCurrentSpecification ||

												( isDefinitionSpecification &&
												isExclusiveSpecification ) ) )
													{
													if( adminItem_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption by generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( !isExclusiveSpecification &&
												isGeneralizationProperName &&
												!isPossessive &&
												currentSpecificationItem.isUserSpecification() )
													{
													if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), currentRelationContextNr, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for a specification conflict in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										}
									}
								}
							while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
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
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word type isn't a reasoning type" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( boolean isNegative, boolean isPartOf, boolean isPossessive, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem previousSpecificationItem = null;
		SpecificationItem lastCreatedSpecificationItem = null;
		WordItem currentGeneralizationWordItem;
		WordItem compoundGeneralizationWordItem = null;

		if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( specificationWordItem != null )
				{
				do	{
					if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem() ) != null )
						{
						previousSpecificationItem = lastCreatedSpecificationItem;

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
								{
								if( ( foundSpecificationItem = currentGeneralizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
									{
									if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem.hasAssumptionLevel(), foundSpecificationItem.isInactiveAssignment(), foundSpecificationItem.isArchivedAssignment(), foundSpecificationItem.isExclusiveSpecification(), isNegative, isPartOf, isPossessive, Constants.NO_QUESTION_PARAMETER, foundSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, null ) == Constants.RESULT_OK )
										{
										if( !CommonVariables.hasShownWarning )
											{
											if( ( specificationResult = currentGeneralizationWordItem.findRelatedSpecification( false, foundSpecificationItem ) ).result == Constants.RESULT_OK )
												{
												if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null )
													{
													if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.hasAssumptionLevel(), relatedSpecificationItem.isInactiveAssignment(), relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), isNegative, isPartOf, isPossessive, relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem.specificationWordItem(), null ) == Constants.RESULT_OK )
														compoundGeneralizationWordItem = currentGeneralizationWordItem;
													else
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );
											}
										}
									else
										adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							else
								adminItem_.startErrorInItem( 1, moduleNameString_, "I've found an undefined generalization word" );
							}
						while( CommonVariables.result == Constants.RESULT_OK &&
						!CommonVariables.hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
						}
					}
				while( CommonVariables.result == Constants.RESULT_OK &&
				!CommonVariables.hasShownWarning &&
				// Do until no more conclusions or questions are created
				previousSpecificationItem != null );
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );

		specificationResult.compoundGeneralizationWordItem = compoundGeneralizationWordItem;
		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *************************************************************************/
