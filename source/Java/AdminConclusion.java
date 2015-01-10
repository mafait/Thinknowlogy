/*
 *	Class:			AdminConclusion
 *	Supports class:	AdminItem
 *	Purpose:		To create conclusions and questions autonomously
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class AdminConclusion
	{
	// Private constructible variables

	private boolean hasCreatedPossessiveReversibleConclusion_;
	private boolean hasFoundAdjustedQuestion_;
	private boolean hasFoundConflictWithUserSpecification_;
	private boolean hasFoundSpecificationWordCollectedWithItself_;

	private short questionParameter_;

	private int generalizationSingleContextNr_;
	private int missingNegativeConclusionSpecificationCollectionNr_;

	private SpecificationItem lastCreatedSpecificationSubstitutionConclusionSpecificationItem_;
	private SpecificationItem missingNegativeConclusionPrimarySpecificationItem_;
	private SpecificationItem missingNegativeConclusionAnotherPrimarySpecificationItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte askSpecificationSubstitutionQuestion( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean isSecondarySpecificationCompoundCollection;
		boolean isBlockingCommonWordOfCompoundCollection = false;
		int nonCompoundSpecificationCollectionNr;
		int secondaryRelationContextNr;
		int secondarySpecificationCollectionNr;
		SpecificationItem adjustedQuestionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem obsoleteSpecificationItem;
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
								questionParameter_ = Constants.WORD_PARAMETER_SINGULAR_VERB_IS;

								nonCompoundSpecificationCollectionNr = secondarySpecificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );
								secondaryRelationContextNr = secondarySpecificationItem.relationContextNr();
								isSecondarySpecificationCompoundCollection = secondarySpecificationWordItem.isCompoundCollection( secondarySpecificationCollectionNr, specificationWordItem );

								foundSpecificationItem = generalizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, isArchivedAssignment, isArchivedAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem );

								// Skip if already exists as specification
								if( foundSpecificationItem == null ||
								generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() ||

								( isSecondarySpecificationCompoundCollection &&
								foundSpecificationItem.isSelfGeneratedAssumption() ) )
									{
									if( isSecondarySpecificationCompoundCollection &&
									nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR )
										{
										// Check for conflicts
										if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, false, false, specificationWordTypeNr, nonCompoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, secondaryRelationContextNr, secondarySpecificationWordItem, secondarySpecificationItem.specificationString() ) == Constants.RESULT_OK )
											{
											// Check for adjusted compound question
											if( ( specificationResult = generalizationWordItem.findQuestionToBeAdjustedByCompoundCollection( false, false, questionParameter_, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, secondaryRelationContextNr, specificationWordItem ) ).result == Constants.RESULT_OK )
												{
												if( ( adjustedQuestionSpecificationItem = specificationResult.adjustedQuestionSpecificationItem ) != null )
													{
													// Write adjusted compound question
													if( generalizationWordItem.writeUpdatedSpecification( true, false, false, adjustedQuestionSpecificationItem ) == Constants.RESULT_OK )
														hasFoundAdjustedQuestion_ = true;
													else
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an adjusted compound question" );
													}
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a question to be adjusted by a compound collection in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" by compound collection" );
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for a specification conflict in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
													nonCompoundSpecificationCollectionNr = myWordItem_.nonCompoundCollectionNrInAllWords( secondarySpecificationCollectionNr );

												if( generalizationWordItem.firstUserSpecificationItem( false, false, secondarySpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) != null ||

												( nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
												generalizationWordItem.firstUserSpecificationItem( false, false, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, null ) != null ) )
													isBlockingCommonWordOfCompoundCollection = true;
												}
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for a conflict with the user specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}

									if( !CommonVariables.hasShownWarning &&
									!isBlockingCommonWordOfCompoundCollection )
										{
										// Ask a specification substitution question
										// See design: paragraph 2.5.1 in the theory document or http://mafait.org/theory_2_5_1/
										if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, false, false, false, (short)( primarySpecificationItem.assumptionLevel() + secondarySpecificationItem.assumptionLevel() ), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter_, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
											{
											if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
												{
												if( hasFoundAdjustedQuestion_ )
													{
													// Replace obsolete justifications from adjusted compound question
													if( generalizationWordItem.replaceObsoleteJustificationsOfAdjustedQuestion( specificationResult.createdJustificationItem, specificationResult.foundSpecificationItem ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to archive obsolete justifications from an adjusted compound question" );
													}
												}
											else	// Has created question part
												{
												if( secondarySpecificationWordItem != specificationWordItem &&
												( obsoleteSpecificationItem = generalizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, isArchivedAssignment, isInactiveAssignment, isArchivedAssignment, false, false, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem ) ) != null )
													{
													if( generalizationWordItem.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace an invalid assumption specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							else
								return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item has no specfication word item" );
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item has no specfication collection" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specfication word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given primary specfication item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegativeConclusion, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasGeneralizationWordMasculineProperNameEnding;
		int nonCompoundSpecificationCollectionNr;
		int secondarySpecificationCollectionNr;
		int specificationCollectionNr;
		SpecificationItem firstNonCompoundCollectionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
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

									if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, ( isInactiveAssignment || isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
										{
										// Current question part isn't related to an answer or a similar question
										if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) == null &&
										commonWordItem.isCollectedWithItself() )
											{
											nonCompoundSpecificationCollectionNr = commonWordItem.nonCompoundCollectionNr( specificationWordTypeNr );

											if( nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
											nonCompoundSpecificationCollectionNr != specificationCollectionNr )
												{
												if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, ( isInactiveAssignment || isArchivedAssignment ), isInactiveAssignment, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
													relatedSpecificationItem = specificationResult.relatedSpecificationItem;
												else
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is collected with itself" );
												}
											}

										if( relatedSpecificationItem == null )
											{
											hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
											hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem.hasMasculineProperNameEnding();

											// Negative feminine conclusion
											if( ( hasGeneralizationWordMasculineProperNameEnding &&
											secondarySpecificationWordItem.isFeminineWord() ) ||

											// Negative masculine conclusion
											( hasGeneralizationWordFeminineProperNameEnding &&
											secondarySpecificationWordItem.isMasculineWord() ) ||

											// Specification is collected with itself
											( specificationWordItem.isCollectedWithItself() &&
											!generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() &&
											generalizationWordItem.isUserGeneralizationWord &&
											!adminItem_.isUserSentencePossessive() ) )
												{
												missingNegativeConclusionSpecificationCollectionNr_ = secondarySpecificationCollectionNr;
												missingNegativeConclusionPrimarySpecificationItem_ = primarySpecificationItem;
												missingNegativeConclusionAnotherPrimarySpecificationItem_ = secondarySpecificationItem;
												}
											else
												{
												// Skip invalid questions
												if( !isNegativeConclusion &&

												( primarySpecificationItem.hasSpecificationCompoundCollection() ||

												( !hasGeneralizationWordFeminineProperNameEnding &&
												!hasGeneralizationWordMasculineProperNameEnding ) ) )
													{
													if( askSpecificationSubstitutionQuestion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a specification substitution question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										else
											{
											if( !commonWordItem.isCollectedWithItself() &&
											secondarySpecificationItem.hasSpecificationCompoundCollection() &&
											( firstNonCompoundCollectionSpecificationItem = generalizationWordItem.firstNonCompoundCollectionSpecificationItem( secondarySpecificationCollectionNr ) ) != null )
												relatedSpecificationItem = firstNonCompoundCollectionSpecificationItem;

											if( missingNegativeConclusionAnotherPrimarySpecificationItem_ != null &&
											missingNegativeConclusionAnotherPrimarySpecificationItem_.specificationCollectionNr() == secondarySpecificationItem.specificationCollectionNr() )
												{
												if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, missingNegativeConclusionPrimarySpecificationItem_, missingNegativeConclusionAnotherPrimarySpecificationItem_, relatedSpecificationItem, generalizationWordItem ) == Constants.RESULT_OK )
													{
													missingNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
													missingNegativeConclusionPrimarySpecificationItem_ = null;
													missingNegativeConclusionAnotherPrimarySpecificationItem_ = null;
													}
												else
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
												}

											if( drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, secondarySpecificationItem, relatedSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + secondarySpecificationWordItem.anyWordTypeString() + "\"" );
											}
										}
									else
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the common word of the given specfication collection number" );
								}
							else
								return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item has no specfication word item" );
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item has no specfication collection" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specfication word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given primary specfication item is undefined" );

		return Constants.RESULT_OK;
		}

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
							!currentSpecificationItem.isOlderItem() &&			// Current sentence
							!currentSpecificationItem.isSelfGenerated() &&		// User sentence
							( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
								{
								foundSpecificationItem = currentSpecificationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, questionSpecificationWordItem );

								if( ( foundSpecificationItem == null ||

								( !foundSpecificationItem.isExclusiveSpecification() &&
								!foundSpecificationItem.isNegative() ) ) )
									hasFoundConflictWithUserSpecification_ = true;
								}
							}
						while( !hasFoundConflictWithUserSpecification_ &&
						( currentSpecificationItem = currentSpecificationItem.nextNonQuestionSpecificationItem() ) != null );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given question specification word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given primary specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte drawNegativeConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isPrimaryAndUserSameSpecification;
		boolean isSelectingUserSpecification = false;
		boolean isUserSpecificationCollectedWithItself = false;
		int anotherPrimarySpecificationCollectionNr;
		JustificationItem createdJustificationItem;
		JustificationItem negativeAssumptionOrConclusionJustificationItem;
		JustificationItem questionJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingPositiveSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem foundNegativeAssumptionOrConclusionPrimarySpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem = null;
		SpecificationItem userSpecificationItem = null;
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
						if( ( anotherPrimarySpecificationCollectionNr = anotherPrimarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
							{
							if( ( anotherPrimarySpecificationWordItem = anotherPrimarySpecificationItem.specificationWordItem() ) != null )
								{
								existingPositiveSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, false, false, Constants.NO_QUESTION_PARAMETER, anotherPrimarySpecificationWordItem );
								foundNegativeAssumptionOrConclusionPrimarySpecificationItem = null;

								if( ( negativeAssumptionOrConclusionJustificationItem = generalizationWordItem.negativeAssumptionOrConclusionJustificationItem( anotherPrimarySpecificationItem ) ) != null )
									foundNegativeAssumptionOrConclusionPrimarySpecificationItem = negativeAssumptionOrConclusionJustificationItem.primarySpecificationItem();

								// Skip when negative conclusion already exists
								if( ( !primarySpecificationItem.isUserSpecification() ||
								!secondarySpecificationItem.isOlderItem() ) &&

								// No positive specification exists with this specification word
								( existingPositiveSpecificationItem == null ||
								existingPositiveSpecificationItem.isHiddenSpecification() ) &&

								// Prefer primary specification with the least uncertainty
								( !primarySpecificationItem.hasSpecificationCollection() ||
								foundNegativeAssumptionOrConclusionPrimarySpecificationItem == null ||
								primarySpecificationItem.assumptionLevel() < foundNegativeAssumptionOrConclusionPrimarySpecificationItem.assumptionLevel() ) &&

								// No question exists with this specification word
								generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, anotherPrimarySpecificationWordItem ) == null )
									{
									if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null )
										{
										isPrimaryAndUserSameSpecification = ( primarySpecificationItem.specificationWordItem() == secondarySpecificationWordItem );

										if( !isPrimaryAndUserSameSpecification )
											anotherSecondarySpecificationItem = secondarySpecificationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, false, false, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null );

										if( generalizationWordItem.isUserGeneralizationWord &&
										( userSpecificationItem = adminItem_.userSpecificationItem() ) != null )
											{
											if( ( userSpecificationWordItem = userSpecificationItem.specificationWordItem() ) != null )
												{
												// Force conclusion when user specification that is collected with itself
												if( userSpecificationWordItem.isCollectedWithItself() )
													isUserSpecificationCollectedWithItself = true;

												if( anotherSecondarySpecificationItem != null &&
												userSpecificationItem.hasSpecificationCompoundCollection() &&
												userSpecificationWordItem.hasCollectionNr( anotherPrimarySpecificationCollectionNr ) )
													isSelectingUserSpecification = true;
												}
											}

										// Draw a negative conclusion
										// Not yet explained in the design
										if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, primarySpecificationItem.isExclusiveSpecification(), true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isUserSpecificationCollectedWithItself ? Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION : Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, anotherPrimarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, ( isPrimaryAndUserSameSpecification || primarySpecificationItem.isHiddenSpecification() ? null : primarySpecificationItem ), anotherPrimarySpecificationItem, ( isSelectingUserSpecification ? userSpecificationItem : secondarySpecificationItem ), ( isSelectingUserSpecification ? null : anotherSecondarySpecificationItem ), generalizationWordItem, anotherPrimarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
											{
											createdJustificationItem = specificationResult.createdJustificationItem;

											// Found specification
											if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
												{
												if( createdJustificationItem != null )
													{
													if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
														{
														foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

														if( negativeAssumptionOrConclusionJustificationItem == null )
															{
															if( foundSpecificationItem.isSelfGeneratedAssumption() )
																{
																if( createdJustificationItem.isConclusionJustification() )
																	{
																	// Replace justification of negative conclusion / assumption
																	if( foundSpecificationItem.markAsConcludedAssumption( false ) != Constants.RESULT_OK )
																		return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
																	}
																}
															else
																{
																if( !isUserSpecificationCollectedWithItself )
																	{
																	if( generalizationWordItem.replaceOrDeleteJustification( createdJustificationItem ) != Constants.RESULT_OK )
																		return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the created justification" );
																	}
																}
															}
														else
															{
															if( !foundSpecificationItem.isConcludedAssumption() )
																{
																// Replace justification of negative conclusion / assumption
																if( generalizationWordItem.replaceJustification( false, negativeAssumptionOrConclusionJustificationItem, createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
																	return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a negative assumption or conclusion justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														}
													else
														return myWordItem_.startErrorInItem( 1, moduleNameString_, "The found specfication item is undefined" );
													}
												}
											else	// Created specification
												{
												// Replace corrected negative conclusion
												if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() )
													{
													if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, false, createdSpecificationItem ) ).result == Constants.RESULT_OK )
														{
														if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null )
															{
															if( generalizationWordItem.replaceOrDeleteSpecification( relatedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a corrected negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created negative conclusion" );
													}
												else
													{
													// Replace question justification
													if( anotherSecondarySpecificationItem != null &&
													anotherPrimarySpecificationItem.hasSpecificationCompoundCollection() )
														{
														if( ( questionJustificationItem = generalizationWordItem.questionJustificationItem( anotherPrimarySpecificationItem ) ) == null )
															{
															if( isSelectingUserSpecification )
																{
																if( generalizationWordItem.changeJustificationOfNegativeAssumptions( userSpecificationItem ) != Constants.RESULT_OK )
																	return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a question justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															{
															if( generalizationWordItem.replaceJustification( false, questionJustificationItem, createdJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a question justification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												}
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + anotherPrimarySpecificationWordItem.anyWordTypeString() + "\"" );
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given related specfication item has no specfication word item" );
									}
								}
							else
								return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item has no specfication word item" );
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item has no specfication collection" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specfication item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given another primary specfication item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given primary specfication item is undefined" );

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
				if( adminItem_.addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, secondarySpecificationItem.isExclusiveSpecification(), secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPartOf(), secondarySpecificationItem.isPossessive(), false, false, false, assumptionLevel, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, secondarySpecificationItem.generalizationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, secondarySpecificationItem.specificationWordItem(), secondarySpecificationItem.generalizationWordItem(), null ).result != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated specification generalization substitution specification" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The generalization word and/or specification word of the given primary specification item isn't a noun" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminConclusion( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		hasCreatedPossessiveReversibleConclusion_ = false;
		hasFoundAdjustedQuestion_ = false;
		hasFoundConflictWithUserSpecification_ = false;
		hasFoundSpecificationWordCollectedWithItself_ = false;

		questionParameter_ = Constants.NO_QUESTION_PARAMETER;

		generalizationSingleContextNr_ = Constants.NO_CONTEXT_NR;
		missingNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = null;
		missingNegativeConclusionPrimarySpecificationItem_ = null;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = null;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
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
		hasCreatedPossessiveReversibleConclusion_ = false;
		hasFoundAdjustedQuestion_ = false;
		hasFoundSpecificationWordCollectedWithItself_ = false;

		generalizationSingleContextNr_ = Constants.NO_CONTEXT_NR;
		missingNegativeConclusionSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = null;
		missingNegativeConclusionPrimarySpecificationItem_ = null;
		missingNegativeConclusionAnotherPrimarySpecificationItem_ = null;
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
									if( generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, false, false, Constants.NO_QUESTION_PARAMETER, answeredQuestionSpecificationWordItem ) == null )
										{
										if( ( currentJustificationItem = answeredQuestionSpecificationItem.firstJustificationItem() ) != null )
											{
											do	{	// Do for all justifications
												if( !currentJustificationItem.hasPrimaryQuestion() )
													{
													if( drawNegativeConclusion( answeredQuestionSpecificationItem.isArchivedAssignment(), answeredQuestionSpecificationItem.generalizationWordTypeNr(), answeredQuestionSpecificationItem.specificationWordTypeNr(), answeredQuestionSpecificationItem.generalizationContextNr(), answeredQuestionSpecificationItem.specificationContextNr(), currentJustificationItem.primarySpecificationItem(), currentJustificationItem.secondarySpecificationItem(), userSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											while( ( currentJustificationItem = currentJustificationItem.attachedJustificationItem() ) != null );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "The answered question specification item has no justification" );
										}
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "The answered question specification item has no specification word" );
								}
							}
						while( ( answeredQuestionSpecificationItem = answeredQuestionSpecificationItem.recentlyAnsweredQuestionSpecificationItem( false ) ) != null );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any answered question specification" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item has no specification word" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item is undefined" );

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
				if( ( currentSpecificationItem = generalizationWordItem.firstSelectedSpecificationItem( false, false, false, false ) ) != null )
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
									foundOnlyOptionLeftSpecificationItem = currentSpecificationItem;	// Found the first option left
								else
									{
									hasFoundMoreOptionsLeft = true;					// There are more options left
									foundOnlyOptionLeftSpecificationItem = null;	// So, clear the found option
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
						if( adminItem_.addSelfGeneratedSpecification( true, isArchivedAssignment, false, true, false, false, foundOnlyOptionLeftSpecificationItem.isPossessive(), false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, firstOnlyOptionLeftSpecificationItem.generalizationWordTypeNr(), firstOnlyOptionLeftSpecificationItem.specificationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, firstOnlyOptionLeftSpecificationItem, null, firstNegativeOnlyOptionLeftAssignmentItem, null, generalizationWordItem, foundOnlyOptionLeftSpecificationItem.specificationWordItem(), null ).result != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification collection number is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte drawPossessiveReversibleConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUniqueUserRelation, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		SpecificationResultType specificationResult;
		boolean isConclusionAssignment;
		boolean isSpecificationCollectedWithItself;
		boolean hasFoundSecondaryArchivedAssignment = false;
		boolean isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		boolean isAssumption = ( userAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL );
		int nContextRelations;
		int foundRelationContextNr;
		int multipleRelationContextNr;
		int generalizationContextNr = Constants.NO_CONTEXT_NR;
		GeneralizationItem currentGeneralizationItem;
		JustificationItem obsoleteAssumptionJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingConclusionSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem collectedWithItselfCommonWordItem;
		WordItem currentGeneralizationWordItem;

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			if( generalizationWordItem != null )
				{
				if( specificationWordItem != null )
					{
					if( relationWordItem != null )
						{
						if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME &&
						adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
							{
							isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

							if( ( foundRelationContextNr = generalizationWordItem.contextNrInWord( specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
								{
								// Check on single entry context
								if( ( nContextRelations = myWordItem_.nContextWordsInAllWords( foundRelationContextNr, specificationWordItem ) ) == 1 )
									{
									if( isSpecificationCollectedWithItself )
										{
										if( generalizationSingleContextNr_ == Constants.NO_CONTEXT_NR )
											{
											multipleRelationContextNr = generalizationWordItem.contextNrInWord( specificationWordItem );

											if( !hasCreatedPossessiveReversibleConclusion_ &&

											( multipleRelationContextNr == Constants.NO_COLLECTION_NR ||
											multipleRelationContextNr == foundRelationContextNr ) )
												{
												if( ( generalizationContextNr = adminItem_.highestContextNr() ) < Constants.MAX_CONTEXT_NR )
													generalizationSingleContextNr_ = ++generalizationContextNr;
												else
													return myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
												}
											else
												// Found context is single context
												generalizationContextNr = foundRelationContextNr;
											}
										else
											generalizationContextNr = generalizationSingleContextNr_;
										}
									}
								else
									{
									// Multiple entry context
									if( nContextRelations > 1 )
										{
										// Find correct generalization context, which will become the relation context
										if( ( contextResult = adminItem_.getRelationContext( false, isExclusiveSpecification, isNegative, !isPossessive, false, Constants.NO_QUESTION_PARAMETER, nContextRelations, relationWordItem, specificationWordItem, generalizationWordItem, null ) ).result == Constants.RESULT_OK )
											generalizationContextNr = contextResult.contextNr;
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to get the relation context number" );
										}
									}
								}

							existingConclusionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, generalizationWordItem );

							// Conclusion doesn't exist yet as user specification
							if( ( existingConclusionSpecificationItem == null ||
							// Allow replacing exclusive generalization conclusion
							existingConclusionSpecificationItem.isInactiveAssignment() ||
							// Allow existing conclusion to change its relation context
							existingConclusionSpecificationItem.isSelfGenerated() ) &&

							// Find the non-reversible specification (active assignments)
							( secondarySpecificationItem = generalizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, true, false, false, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
								{
								if( !secondarySpecificationItem.isOlderItem() ||
								secondarySpecificationItem.assumptionLevel() != userAssumptionLevel ||
								myWordItem_.hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) )
									{
									// Archived assignments
									if( ( tempSpecificationItem = generalizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
										{
										if( secondarySpecificationItem.isAssignment() )
											hasFoundSecondaryArchivedAssignment = true;
										else
											secondarySpecificationItem = tempSpecificationItem;
										}

									// Check if user specification exists
									userSpecificationItem = relationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationWordItem );

									if( userSpecificationItem == null ||
									// Ambiguous specification: different tense (archived means: past tense)
									userSpecificationItem.isArchivedAssignment() != hasFoundSecondaryArchivedAssignment )
										{
										isConclusionAssignment = ( ( isAssignment &&
																	isAssumption ) ||

																	( !isAssumption &&

																	( ( isAssignment &&
																	userSpecificationItem == null ) ||

																	secondarySpecificationItem.hasExclusiveGeneralizationCollection() ) ) );

										// Draw a possessive reversible conclusion
										// See design: paragraph 2.3.2 in the theory document or http://mafait.org/theory_2_3_2/
										if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( isConclusionAssignment, isArchivedAssignment, false, isExclusiveSpecification, isNegative, false, !isPossessive, isUniqueUserRelation, isSpecificationCollectedWithItself, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, null, null, secondarySpecificationItem, null, relationWordItem, specificationWordItem, generalizationWordItem ) ).result == Constants.RESULT_OK )
											{
											hasCreatedPossessiveReversibleConclusion_ = true;

											if( contextResult.replaceContextNr == Constants.NO_CONTEXT_NR )
												{
												if( isPossessive &&
												( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
													{
													if( adminItem_.makePossessiveConditionalSpecificationAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, createdSpecificationItem, null, generalizationWordItem, specificationWordItem, relationWordItem ) == Constants.RESULT_OK )
														{
														if( ( collectedWithItselfCommonWordItem = createdSpecificationItem.collectedWithItselfCommonWordItem() ) != null )
															{
															if( ( currentGeneralizationItem = collectedWithItselfCommonWordItem.firstSpecificationGeneralizationItem() ) != null )
																{
																foundSpecificationItem = null;

																do	{
																	if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
																		{
																		if( ( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isConclusionAssignment, isInactiveAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, collectedWithItselfCommonWordItem ) ) != null )
																			{
																			// Draw a "part of" generalization conclusion
																			// Not yet explained in the design
																			if( foundSpecificationItem.isPartOf() )
																				{
																				if( adminItem_.addSelfGeneratedSpecification( isConclusionAssignment, isArchivedAssignment, false, false, isNegative, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, foundSpecificationItem, null, relationWordItem, currentGeneralizationWordItem, null ).result != Constants.RESULT_OK )
																					myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated reversible part-of specification to generalization word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
																				}
																			}
																		}
																	else
																		return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
																	}
																while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
																}
															}
														}
													else
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
													{
													foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

													if( foundSpecificationItem.isSelfGeneratedAssumption() )
														{
														if( relationWordItem.replaceOlderSpecificationByConcludedAssumption( contextResult.replaceContextNr, foundSpecificationItem ) == Constants.RESULT_OK )
															{
															if( ( obsoleteAssumptionJustificationItem = relationWordItem.obsoleteAssumptionJustificationItem() ) != null )
																{
																if( relationWordItem.replaceOrDeleteJustification( obsoleteAssumptionJustificationItem ) != Constants.RESULT_OK )
																	return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace older conclusion specifications in relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find an older specification to replace" );
												}
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated possessive reversible specification from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							}
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specfication word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given relation context number is undefined" );

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
					if( ( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem() ) != null )
						{
						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&
								currentGeneralizationItem.generalizationWordTypeNr() == generalizationWordTypeNr &&
								( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, false, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
									{
									if( !foundSpecificationItem.hasSpecificationCollection() &&
									!foundSpecificationItem.isExclusiveSpecification() )
										{
										assumptionLevel = ( foundSpecificationItem.assumptionLevel() > secondarySpecificationItem.assumptionLevel() ? foundSpecificationItem.assumptionLevel() : secondarySpecificationItem.assumptionLevel() );

										if( adminItem_.collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
											{
											if( drawSpecificationGeneralizationConclusion( assumptionLevel, foundSpecificationItem ) == Constants.RESULT_OK )
												{
												if( drawSpecificationGeneralizationConclusion( assumptionLevel, secondarySpecificationItem ) != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification generalization conclusion about the given sentence" );
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification generalization conclusion about an earlier sentence" );
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );
										}
									}
								}
							else
								return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
							}
						while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean hasCorrectedAssumptionByKnowledge;
		boolean hasCorrectedAssumptionByOppositeQuestion;
		boolean hasGeneralizationWordActiveQuestionWithCompoundCollection;
		boolean hasGeneralizationWordFeminineProperNameEnding;
		boolean hasGeneralizationWordMasculineProperNameEnding;
		boolean isExclusiveCurrentSpecification;
		boolean isGeneralizationFeminineOrMasculineWord;
		boolean isSpecificationCollectedWithItself;
		boolean isUserSpecificationWord;
		boolean isAssignment = ( isInactiveAssignment || isArchivedAssignment );
		boolean isDefinitionSpecification = adminItem_.isSingularOrPluralNoun( generalizationWordTypeNr );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		boolean isNegativeConclusion = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		int currentRelationContextNr;
		int currentSpecificationCollectionNr;
		JustificationItem obsoleteAssumptionJustificationItem;
		SpecificationItem bestMatchingRelationSpecificationItem;
		SpecificationItem collectedWithItselfSpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem firstFeminineOrMasculineSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem primarySpecificationItem;
		WordItem currentSpecificationWordItem;
		String currentSpecificationString;

		if( adminItem_.isReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
				{
				if( generalizationWordItem != null )
					{
					if( specificationWordItem != null )
						{
						hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();
						isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

						// Skip definition sentence with the same generalization and specification word
						if( !hasFoundSpecificationWordCollectedWithItself_ ||
						!hasCorrectedAssumptionByKnowledge )
							{
							if( adminItem_.makeGeneralizationAssumptionBySpecification( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
								{
								if( isSpecificationCollectedWithItself )
									hasFoundSpecificationWordCollectedWithItself_ = true;
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption by specification word \"" + specificationWordItem.anyWordTypeString() + "\" in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}

						if( ( currentSpecificationItem = specificationWordItem.firstNonQuestionSpecificationItem() ) != null )
							{
							hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();

							hasGeneralizationWordActiveQuestionWithCompoundCollection = generalizationWordItem.hasActiveQuestionWithCompoundCollection();
							hasGeneralizationWordFeminineProperNameEnding = generalizationWordItem.hasFeminineProperNameEnding();
							hasGeneralizationWordMasculineProperNameEnding = generalizationWordItem.hasMasculineProperNameEnding();

							isGeneralizationFeminineOrMasculineWord = generalizationWordItem.isFeminineOrMasculineWord();
							isUserSpecificationWord = specificationWordItem.isUserSpecificationWord;

							do	{
								if( currentSpecificationItem.isSpecificationNoun() &&
								!currentSpecificationItem.isSpecificationGeneralization() &&
								currentSpecificationItem.isNegative() == isNegative &&
								currentSpecificationItem.isPossessive() == isPossessive &&
								( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
									{
									questionParameter_ = Constants.NO_QUESTION_PARAMETER;

									isExclusiveCurrentSpecification = currentSpecificationItem.isExclusiveSpecification();
									currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
									currentRelationContextNr = currentSpecificationItem.relationContextNr();

									currentSpecificationString = currentSpecificationItem.specificationString();

									if( ( bestMatchingRelationSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, false, isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) != null )
										{
										if( !isQuestion )
											isNegativeConclusion = true;
										}

									if( ( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( ( isAssignment || hasCorrectedAssumptionByOppositeQuestion ), isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) == null )
										{
										// Check for more conflicts
										if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), currentRelationContextNr, currentSpecificationWordItem, currentSpecificationString ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for a specification conflict in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									else
										{
										if( !hasGeneralizationWordActiveQuestionWithCompoundCollection &&
										isExclusiveCurrentSpecification &&
										!isGeneralizationFeminineOrMasculineWord &&

										// Matching feminine word
										( ( hasGeneralizationWordFeminineProperNameEnding &&
										currentSpecificationWordItem.isFeminineWord() ) ||

										// Matching masculine word
										( hasGeneralizationWordMasculineProperNameEnding &&
										currentSpecificationWordItem.isMasculineWord() ) ) &&

										// Select correct compound word
										currentSpecificationWordItem.compoundGeneralizationWordItem( currentSpecificationCollectionNr ) != specificationWordItem &&
										// Skip creating feminine or masculine proper name ending assumption if conclusion already exists
										generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, false, false, Constants.NO_QUESTION_PARAMETER, currentSpecificationWordItem ) == null )
											{
											if( ( firstFeminineOrMasculineSpecificationItem = currentSpecificationWordItem.firstFeminineOrMasculineSpecificationItem() ) != null )
												{
												// Make a feminine or masculine proper name ending assumption
												// Not yet explained in the design
												if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION, Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, foundSpecificationItem, currentSpecificationItem, firstFeminineOrMasculineSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
													{
													if( specificationResult.createdSpecificationItem != null &&
													// Has missed drawing some negative conclusions
													missingNegativeConclusionSpecificationCollectionNr_ == currentSpecificationCollectionNr )
														{
														// Draw missing negative conclusions
														if( drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a feminine or masculine substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
												}
											else
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find a feminine or masculine specification item in current specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
											}

										if( !isExclusiveCurrentSpecification ||
										bestMatchingRelationSpecificationItem == null )
											{
											if( isGeneralizationProperName ||
											currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR )
												{
												if( ( isExclusiveSpecification &&
												isGeneralizationProperName ) ||

												( !isExclusiveSpecification &&
												isExclusiveCurrentSpecification ) )
													{
													if( isGeneralizationProperName &&
													currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR )
														{
														// Ask a question or draw a negative conclusion
														if( !isNegative &&
														!isPossessive &&
														!specificationWordItem.isUserGeneralizationWord &&

														( isNegativeConclusion ||

														// Skip invalid compound feminine/masculine questions,
														// but allow those questions to be adjusted
														( ( !isExclusiveCurrentSpecification ||

														( !foundSpecificationItem.isQuestion() &&
														!currentSpecificationWordItem.isUserSpecificationWord ) ) &&

														( adminItem_.hasFoundSuggestiveQuestionAssumption() ||
														generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() ||

														( ( !isSpecificationCollectedWithItself ||
														!isUserSpecificationWord ) &&

														( !hasGeneralizationWordActiveQuestionWithCompoundCollection ||
														foundSpecificationItem.hasSpecificationCollection() ||
														adminItem_.isExclusiveUserSpecification() ||
														// When specification is collected with itself
														currentSpecificationWordItem.compoundCollectionNr( specificationWordTypeNr ) == Constants.NO_COLLECTION_NR ) ) ) ) ) )
															{
															if( askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegativeConclusion, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, foundSpecificationItem, currentSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a specification substitution question or to draw a negative conclusion in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												else	// Make an assumption or draw a conclusion
													{
													if( isAssumption ||
													currentSpecificationItem.hasAssumptionLevel() )
														{
														if( isArchivedAssignment )
															primarySpecificationItem = foundSpecificationItem;
														else
															{
															if( isSpecificationCollectedWithItself ||
															( primarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isNegative, isPossessive, questionParameter, adminItem_.mostRecentRelationContextNr(), specificationWordItem ) ) == null )
																primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isNegative, isPossessive, questionParameter, specificationWordItem );
															}

														if( primarySpecificationItem != null )
															{
															if( ( !isSpecificationCollectedWithItself ||

															( !foundSpecificationItem.isOlderItem() &&

															( CommonVariables.isUserQuestion ||
															foundSpecificationItem.isUserSpecification() ||

															( currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR &&
															foundSpecificationItem.isHiddenSpecification() &&
															generalizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, isNegative, isPossessive, specificationWordItem.compoundCollectionNr( specificationWordTypeNr ), generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null ) == null ) ) ) ) &&

															// Skip if already exists as user specification
															generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null )
																{
																// Make a compound specification substitution assumption, which is actually an uncertain conclusion
																// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
																//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
																if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, false, isSpecificationCollectedWithItself, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
																	{
																	if( ( obsoleteAssumptionJustificationItem = generalizationWordItem.obsoleteAssumptionJustificationItem() ) != null )
																		{
																		if( generalizationWordItem.replaceOrDeleteJustification( obsoleteAssumptionJustificationItem ) != Constants.RESULT_OK )
																			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an obsolete assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	}
																else
																	return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a suggestive question specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															return myWordItem_.startErrorInItem( 1, moduleNameString_, "The primary specification item is undefined" );
														}
													else
														{
														collectedWithItselfSpecificationItem = ( isSpecificationCollectedWithItself ? generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) : null );

														if( currentSpecificationItem.isSpecificationNoun() &&

														( ( !isExclusiveSpecification &&

														( collectedWithItselfSpecificationItem == null ||
														!collectedWithItselfSpecificationItem.isOlderItem() ) &&

														// Skip if already exists as user specification
														generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) ||

														// Check if exclusive conclusion already exists
														( isExclusiveSpecification &&
														generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) == null ) ) )
															{
															if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
																{
																if( !primarySpecificationItem.isPartOf() )
																	{
																	if( lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ != null &&
																	lastCreatedSpecificationSubstitutionConclusionSpecificationItem_.generalizationWordItem() == generalizationWordItem )
																		currentSpecificationCollectionNr = lastCreatedSpecificationSubstitutionConclusionSpecificationItem_.specificationCollectionNr();

																	// Draw a compound specification substitution conclusion
																	// See design:	- paragraph 2.3.1 in the theory document or http://mafait.org/theory_2_3_1/,
																	//				- paragraph 2.3.1.1 in the theory document or http://mafait.org/theory_2_3_1_1/
																	if( ( specificationResult = adminItem_.addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, primarySpecificationItem.isEveryGeneralization(), isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, currentSpecificationItem.specificationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), Constants.NO_CONTEXT_NR, primarySpecificationItem, null, currentSpecificationItem, null, generalizationWordItem, currentSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
																		{
																		if( ( lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ = specificationResult.createdSpecificationItem ) != null )
																			{
																			if( hasCorrectedAssumptionByKnowledge )
																				{
																				if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, false, lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ ) ).result == Constants.RESULT_OK )
																					{
																					if( specificationResult.relatedSpecificationItem != null )
																						{
																						// Replace incorrect assumption
																						if( generalizationWordItem.replaceOrDeleteSpecification( specificationResult.relatedSpecificationItem, lastCreatedSpecificationSubstitutionConclusionSpecificationItem_ ) != Constants.RESULT_OK )
																							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace a corrected specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																						}
																					}
																				else
																					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the created specification" );
																				}
																			else
																				{
																				if( !isGeneralizationProperName )
																					{
																					if( adminItem_.makeGeneralizationAssumptionBySpecification( isArchivedAssignment, isNegative, isPossessive, currentSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption in generalization word \"" + specificationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
																					}
																				}
																			}
																		}
																	else
																		return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find a primary specification item of a conclusion or question" );
															}
														}
													}

												if( !CommonVariables.hasShownWarning &&
												currentSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

												( !isExclusiveCurrentSpecification ||

												( isDefinitionSpecification &&
												isExclusiveSpecification ) ) )
													{
													if( adminItem_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, isNegative, isPossessive, questionParameter_, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption by generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( !isExclusiveSpecification &&
												isGeneralizationProperName &&
												!isPossessive &&
												currentSpecificationItem.isUserSpecification() )
													{
													if( generalizationWordItem.checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, currentSpecificationCollectionNr, currentSpecificationItem.generalizationContextNr(), currentSpecificationItem.specificationContextNr(), currentRelationContextNr, currentSpecificationWordItem, currentSpecificationString ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for a specification conflict in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										}
									}
								}
							while( ( currentSpecificationItem = currentSpecificationItem.nextNonQuestionSpecificationItem() ) != null );
							}
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specfication word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word type isn't a reasoning type" );

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
								if( ( foundSpecificationItem = currentGeneralizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, true, true, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
									{
									if( drawSpecificationSubstitutionConclusionOrAskQuestion( foundSpecificationItem.hasAssumptionLevel(), foundSpecificationItem.isInactiveAssignment(), foundSpecificationItem.isArchivedAssignment(), foundSpecificationItem.isExclusiveSpecification(), isNegative, isPartOf, isPossessive, Constants.NO_QUESTION_PARAMETER, foundSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, specificationWordItem, null ) == Constants.RESULT_OK )
										{
										if( !CommonVariables.hasShownWarning )
											{
											if( ( specificationResult = currentGeneralizationWordItem.findRelatedSpecification( false, false, foundSpecificationItem ) ).result == Constants.RESULT_OK )
												{
												if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null )
													{
													if( drawSpecificationSubstitutionConclusionOrAskQuestion( relatedSpecificationItem.hasAssumptionLevel(), relatedSpecificationItem.isInactiveAssignment(), relatedSpecificationItem.isArchivedAssignment(), relatedSpecificationItem.isExclusiveSpecification(), isNegative, isPartOf, isPossessive, relatedSpecificationItem.questionParameter(), relatedSpecificationItem.generalizationWordTypeNr(), specificationWordTypeNr, generalizationContextNr, specificationContextNr, currentGeneralizationWordItem, relatedSpecificationItem.specificationWordItem(), null ) == Constants.RESULT_OK )
														specificationResult.compoundGeneralizationWordItem = currentGeneralizationWordItem;
													else
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" is related to the found specification" );
											}
										}
									else
										myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a specification substitution conclusion or question for generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							else
								myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
							}
						while( CommonVariables.result == Constants.RESULT_OK &&
						!CommonVariables.hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
						}
					}
				while( CommonVariables.result == Constants.RESULT_OK &&
				!CommonVariables.hasShownWarning &&
				previousSpecificationItem != null );	// Do until no more conclusions or questions are created
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}
	};

/*************************************************************************
 *
 *	"He forgives all my sins and heals all my diseases.
 *	He redeems me from death and crowns me with love and
 *	tender mercies. He fills my life with good things.
 *	My youth is renewed like the eagle's!" (Psalm 103:3-5)
 *
 *************************************************************************/
