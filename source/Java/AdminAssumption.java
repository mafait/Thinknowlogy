/*
 *	Class:			AdminAssumption
 *	Supports class:	AdminItem
 *	Purpose:		To create assumptions autonomously
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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
	private boolean isAssumption_;

	private int lastCreatedRelationContextNr_;

	private SpecificationItem foundPossessiveSpecificationItem_;
	private SpecificationItem foundOppositePossessiveSpecificationItem_;
	private SpecificationItem possessiveSpecificationItem_;
	private SpecificationItem secondarySpecificationItem_;

	private WordItem generalizationAssumptionWordItem_;
	private WordItem oppositePossessiveSpecificationWordItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private static boolean isUniqueRelation( short justificationTypeNr )
		{
		return ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );
		}

	private byte addMissingJustification( short justificationTypeNr, SpecificationItem assumptionSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		JustificationResultType justificationResult;
		JustificationItem createdJustificationItem;

		if( assumptionSpecificationItem != null )
			{
			if( secondarySpecificationItem != null )
				{
				if( generalizationWordItem != null )
					{
					if( ( justificationResult = generalizationWordItem.addJustification( false, justificationTypeNr, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, primarySpecificationItem, null, secondarySpecificationItem, null, null ) ).result == Constants.RESULT_OK )
						{
						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
							{
							if( generalizationWordItem.attachJustification( createdJustificationItem, assumptionSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach the created justification to the given assumption specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a justification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given assumption specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte correctSuggestiveAssumptionsByOppositeQuestion( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem oldCorrectedAssumptionByOppositeQuestionJustificationItem;
		SpecificationItem createdSpecificationItem;
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
						if( oldSuggestiveQuestionAssumptionAssignmentItem.isAssignment() )
							{
							if( ( oldSuggestiveSpecificationWordItem = oldSuggestiveQuestionAssumptionAssignmentItem.specificationWordItem() ) != null )
								{
								if( secondarySpecificationItem.isAssignment() )
									{
									// Correct old question
									if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, oldSuggestiveQuestionAssumptionAssignmentItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
										{
										if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
											{
											// Correct new question
											if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, isNegative, false, isPossessive, false, false, false, secondarySpecificationItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result == Constants.RESULT_OK )
												{
												if( ( oldCorrectedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem.oldCorrectedAssumptionByOppositeQuestionJustificationItem() ) != null )
													{
													if( ( secondarySpecificationWordItem = oldCorrectedAssumptionByOppositeQuestionJustificationItem.secondarySpecificationWordItem() ) != null )
														{
														// Correct generalization assumption
														if( ( primarySpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != null )
															{
															if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
																{
																if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
																	{
																	if( foundSpecificationItem.attachJustificationToSpecification( oldCorrectedAssumptionByOppositeQuestionJustificationItem ) != Constants.RESULT_OK )
																		return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach the old corrected assumption by opposite question justification item to the corrected generalization assumption" );
																	}
																else
																	return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the corrected generalization assumption" );
																}
															else
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a specification substitution assumption" );
															}
														else
															return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any specification substitution assumption justification item" );
														}
													else
														return myWordItem_.startErrorInItem( 1, moduleNameString_, "The old corrected assumption by opposite question justification item has no secondary specification word" );
													}
												else
													return myWordItem_.startErrorInItem( 1, moduleNameString_, "I could find the old corrected assumption by opposite question justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any specification for the old suggestive question assumption" );
										}
									else
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to ask a question about a corrected assumption by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + oldSuggestiveSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is no assignment" );
								}
							else
								return myWordItem_.startErrorInItem( 1, moduleNameString_, "The old suggestive question assumption specification item has no specification word item" );
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The old suggestive question assumption assignment item is no assignment" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the old suggestive question assumption specification item" );
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

	private byte findPossessiveSpecifications( int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem definitionWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundPossessiveSpecificationWord = true;
		int possessiveSpecificationCollectionNr;
		SpecificationItem possessiveSpecificationItem;
		SpecificationItem previousPossessiveSpecificationItem = null;
		WordItem possessiveSpecificationWordItem;
		WordItem previousPossessiveSpecificationWordItem;
		WordItem higherLevelPossessiveSpecificationWordItem = null;

		foundPossessiveSpecificationItem_ = null;
		foundOppositePossessiveSpecificationItem_ = null;

		if( generalizationWordItem != null )
			{
			if( definitionWordItem != null )
				{
				if( specificationWordItem != null )
					{
					// First try to find a possessive specification on a higher level
					if( ( possessiveSpecificationItem = definitionWordItem.firstNonQuestionSpecificationItem() ) != null )
						{
						do	{
							if( !possessiveSpecificationItem.isExclusiveSpecification() &&
							possessiveSpecificationItem.isPossessive() &&
							possessiveSpecificationItem.isSpecificationNoun() &&
							( possessiveSpecificationWordItem = possessiveSpecificationItem.specificationWordItem() ) != null )
								{
								if( possessiveSpecificationWordItem != higherLevelPossessiveSpecificationWordItem &&
								possessiveSpecificationWordItem.firstActiveAssignmentOrSpecificationItem( false, true, false, false, Constants.NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, specificationWordItem ) != null )
									{
									if( higherLevelPossessiveSpecificationWordItem == null )
										higherLevelPossessiveSpecificationWordItem = possessiveSpecificationWordItem;
									else
										hasFoundPossessiveSpecificationWord = false;
									}
								}
							}
						while( ( possessiveSpecificationItem = possessiveSpecificationItem.nextSelectedSpecificationItem() ) != null );

						if( hasFoundPossessiveSpecificationWord &&
						( possessiveSpecificationItem = definitionWordItem.firstNonQuestionSpecificationItem() ) != null )
							{
							if( possessiveSpecificationItem.hasSpecificationCollection() )
								{
								if( higherLevelPossessiveSpecificationWordItem == null )
									higherLevelPossessiveSpecificationWordItem = specificationWordItem;

								// Try to find the opposite of the possessive specification
								do	{
									if( !possessiveSpecificationItem.isExclusiveSpecification() &&
									!possessiveSpecificationItem.isPartOf() &&
									possessiveSpecificationItem.isPossessive() &&
									( possessiveSpecificationWordItem = possessiveSpecificationItem.specificationWordItem() ) != null )
										{
										if( possessiveSpecificationWordItem != generalizationWordItem )
											{
											if( possessiveSpecificationWordItem.isSingularNoun() &&
											previousPossessiveSpecificationItem != null &&
											( previousPossessiveSpecificationWordItem = previousPossessiveSpecificationItem.specificationWordItem() ) != null )
												{
												possessiveSpecificationCollectionNr = possessiveSpecificationItem.specificationCollectionNr();

												if( previousPossessiveSpecificationWordItem != possessiveSpecificationWordItem &&
												higherLevelPossessiveSpecificationWordItem.hasCollectionNr( possessiveSpecificationCollectionNr ) &&
												previousPossessiveSpecificationItem.specificationCollectionNr() == possessiveSpecificationCollectionNr )
													foundOppositePossessiveSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? previousPossessiveSpecificationItem : possessiveSpecificationItem );

												foundPossessiveSpecificationItem_ = ( higherLevelPossessiveSpecificationWordItem == possessiveSpecificationWordItem ? possessiveSpecificationItem : previousPossessiveSpecificationItem );
												}

											previousPossessiveSpecificationItem = possessiveSpecificationItem;
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "The specification word of the possessive specification item is the same as the given generalization word" );
										}
									}
								while( ( possessiveSpecificationItem = possessiveSpecificationItem.nextSelectedSpecificationItem() ) != null );
								}
							}
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given definition word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

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
				if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result == Constants.RESULT_OK )
					{
					if( generalizationWordItem.hasCorrectedAssumptionByKnowledge() &&
					specificationResult.createdJustificationItem != null &&
					( olderGeneralizationAssumptionBySpecificationItem = specificationResult.foundSpecificationItem ) != null )
						{
						if( ( olderGeneralizationAssumptionBySpecificationJustificationItem = olderGeneralizationAssumptionBySpecificationItem.olderGeneralizationAssumptionBySpecificationJustificationItem() ) != null )
							{
							if( generalizationWordItem.replaceJustification( false, olderGeneralizationAssumptionBySpecificationJustificationItem, specificationResult.createdJustificationItem, olderGeneralizationAssumptionBySpecificationItem ) != Constants.RESULT_OK )
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item" );
							}
						}
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminAssumption( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		hasFoundSuggestiveQuestionAssumption_ = false;
		isAssumption_ = false;

		lastCreatedRelationContextNr_ = Constants.NO_CONTEXT_NR;

		foundPossessiveSpecificationItem_ = null;
		foundOppositePossessiveSpecificationItem_ = null;
		possessiveSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

		generalizationAssumptionWordItem_ = null;
		oppositePossessiveSpecificationWordItem_ = null;

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

	protected void initializeAdminAssumptionVariables()
		{
		hasFoundSuggestiveQuestionAssumption_ = false;
		isAssumption_ = false;

		secondarySpecificationItem_ = null;
		generalizationAssumptionWordItem_ = null;
		}

	protected boolean hasFoundSuggestiveQuestionAssumption()
		{
		return hasFoundSuggestiveQuestionAssumption_;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isOppositePossessiveSpecificationCollectedWithItself;
		boolean isPossessiveSpecificationCollectedWithItself;
		boolean hasRelationContext = ( relationWordItem != null );
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem = null;
		WordItem currentWordItem;
		WordItem possessiveSpecificationWordItem = null;

		possessiveSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

		oppositePossessiveSpecificationWordItem_ = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				userSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

				if( userSpecificationItem != null &&

				( hasRelationContext ||

				( !userSpecificationItem.isOlderItem() &&

				( !isPossessive ||
				!userSpecificationItem.isExclusiveSpecification() ) ) ) )
					{
					// Initially not all words are linked. So, search all words.
					if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
						{
						do	{
							if( currentWordItem.isNoun() &&
							currentWordItem != specificationWordItem )
								{
								if( findPossessiveSpecifications( generalizationContextNr, specificationContextNr, generalizationWordItem, currentWordItem, specificationWordItem ) == Constants.RESULT_OK )
									{
									if( foundPossessiveSpecificationItem_ != null )
										{
										possessiveSpecificationItem_ = foundPossessiveSpecificationItem_;

										if( ( possessiveSpecificationWordItem = possessiveSpecificationItem_.specificationWordItem() ) != null )
											{
											if( ( primarySpecificationItem = possessiveSpecificationWordItem.firstActiveAssignmentOrSpecificationItem( false, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, specificationWordItem ) ) != null )
												{
												isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem.isCollectedWithItself();

												if( ( !userSpecificationItem.hasRelationContext() &&
												generalizationWordItem.hasAnsweredQuestion( primarySpecificationItem.specificationCollectionNr() ) ) ||

												( isPossessiveSpecificationCollectedWithItself &&
												!userSpecificationItem.isActiveAssignment() &&
												userSpecificationItem.specificationWordItem() == possessiveSpecificationWordItem ) )
													{
													// Skip making exclusive specification substitution assumption
													if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem );

													if( existingUserSpecificationItem == null ||

													// Confirmed specification without relation word
													( relationWordItem != null &&

													( isPossessiveSpecificationCollectedWithItself ||
													!existingUserSpecificationItem.hasRelationContext() ) ) )
														{
														existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, possessiveSpecificationWordItem );

														// Make an exclusive specification substitution assumption - normal
														// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
														if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, ( hasRelationContext ? relationWordTypeNr : Constants.WORD_TYPE_UNDEFINED ), Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, possessiveSpecificationWordItem, relationWordItem ) ).result == Constants.RESULT_OK )
															{
															if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
																createdOrFoundSpecificationItem = specificationResult.foundSpecificationItem;
															else
																createdOrFoundSpecificationItem = createdSpecificationItem;

															if( createdOrFoundSpecificationItem != null )
																{
																if( isArchivedAssignment )
																	secondarySpecificationItem_ = generalizationWordItem.firstAssignmentItem( false, false, true, isPossessive, createdOrFoundSpecificationItem.questionParameter(), createdOrFoundSpecificationItem.relationContextNr(), createdOrFoundSpecificationItem.specificationWordItem() );
																else
																	secondarySpecificationItem_ = createdOrFoundSpecificationItem.updatedSpecificationItem();

																if( hasRelationContext &&
																createdSpecificationItem != null &&
																existingSpecificationItem != null &&
																existingSpecificationItem.hasRelationContext() &&
																existingSpecificationItem.isSelfGeneratedAssumption() &&
																// If cross-collected afterwards
																existingSpecificationItem.relationContextNr() != createdSpecificationItem.relationContextNr() &&
																( firstJustificationItem = existingSpecificationItem.firstJustificationItem() ) != null )
																	{
																	if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
																		{
																		if( generalizationWordItem.attachJustification( firstJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach the justifications of the existing specification to the created assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	else
																		return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the existing specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return myWordItem_.startErrorInItem( 1, moduleNameString_, "No specification item was created or found" );
															}
														else
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										}

									// First opposite possessive specification
									if( !CommonVariables.hasShownWarning &&
									possessiveSpecificationWordItem != null &&
									foundOppositePossessiveSpecificationItem_ != null &&
									( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveSpecificationItem_.specificationWordItem() ) != null )
										{
										if( secondarySpecificationItem_ == null )
											{
											secondarySpecificationItem_ = userSpecificationItem;

											// Find another primary definition
											if( !hasRelationContext ||
											isPossessive )
												anotherPrimarySpecificationItem = possessiveSpecificationWordItem.firstAssignmentOrSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );
											}

										existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

										if( secondarySpecificationItem_ != null &&

										( !secondarySpecificationItem_.isOlderItem() ||
										secondarySpecificationItem_.isSelfGenerated() ) &&

										( existingSpecificationItem == null ||
										existingSpecificationItem.hasRelationContext() ||
										existingSpecificationItem.isOlderItem() ) &&

										// Skip if user specification exists
										generalizationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == null )
											{
											isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_.isCollectedWithItself();
											existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( false, false, isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_ );

											// Make an exclusive specification substitution assumption - opposite
											// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
											if( ( specificationResult = addSelfGeneratedSpecification( false, false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, ( hasRelationContext ? relationWordTypeNr : Constants.WORD_TYPE_UNDEFINED ), Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, foundOppositePossessiveSpecificationItem_, ( isOppositePossessiveSpecificationCollectedWithItself ? null : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationCollectedWithItself ? userSpecificationItem : secondarySpecificationItem_ ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result == Constants.RESULT_OK )
												{
												if( !isPossessive )
													{
													if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
														{
														createdJustificationItem = specificationResult.createdJustificationItem;

														if( createdJustificationItem != null &&
														createdJustificationItem.isActiveItem() &&
														!generalizationWordItem.isJustificationInUse( createdJustificationItem ) )
															{
															if( generalizationWordItem.replaceOrDeleteJustification( createdJustificationItem ) != Constants.RESULT_OK )
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														{
														if( hasRelationContext &&
														existingSpecificationItem != null &&
														existingSpecificationItem.hasRelationContext() &&
														!existingSpecificationItem.isReplacedOrDeletedItem() &&
														// If cross-collected afterwards
														existingSpecificationItem.relationContextNr() != createdSpecificationItem.relationContextNr() &&
														( firstJustificationItem = existingSpecificationItem.firstJustificationItem() ) != null )
															{
															if( generalizationWordItem.replaceOrDeleteSpecification( existingSpecificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
																{
																if( generalizationWordItem.attachJustification( firstJustificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
																	return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach the created possessive reversible assumption justification to the created assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															else
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace the existing specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and opposite possessive specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
											}
										}
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find possessive specifications in word \"" + currentWordItem.anyWordTypeString() + "\"" );
								}
							}
						while( !CommonVariables.hasShownWarning &&
						( currentWordItem = currentWordItem.nextWordItem() ) != null );

						if( !CommonVariables.hasShownWarning &&
						hasRelationContext &&
						secondarySpecificationItem_ != null )
							{
							if( makePossessiveConditionalSpecificationAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem_, userSpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );
					}
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short specificQuestionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( adminItem_.isReasoningWordType( generalizationWordTypeNr ) )
			{
			if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
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
									currentGeneralizationWordItem.isNoun() &&
									!currentGeneralizationWordItem.hasCollection() &&
									!currentGeneralizationWordItem.hasPossessiveButNonQuestionSpecification() &&
									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
										{
										// Get secondary specification for justification
										if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
											{
											if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME )
												{
												if( makeGeneralizationAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, currentGeneralizationWordItem ) != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											{
											// Get secondary question specification for justification
											if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, specificQuestionParameter, specificationWordItem ) ) != null )
												{
												if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, false, isArchivedAssignment, false, isNegative, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, Constants.NO_CONTEXT_NR, generalizationWordItem, currentGeneralizationWordItem, null ) != Constants.RESULT_OK )
													myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											}
										}
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
								}
							while( !CommonVariables.hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
							}
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word type isn't a noun" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word type isn't a reasoning word type" );

		return Constants.RESULT_OK;
		}

	protected byte makeGeneralizationAssumptionBySpecification( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean hasGeneralizationWordConfirmedSpecification;
		boolean isGeneralizationCollectedWithItself;
		boolean isSpecificationCollectedWithItself;
		GeneralizationItem currentGeneralizationItem;
		JustificationItem createdJustificationItem;
		JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem userSpecificationItem = null;
		WordItem currentGeneralizationWordItem;

		if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( generalizationWordItem != null )
				{
				if( specificationWordItem != null )
					{
					if( generalizationWordItem.isUserGeneralizationWord )
						userSpecificationItem = adminItem_.userSpecificationItem();

					if( generalizationWordItem != specificationWordItem &&
					( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem() ) != null )
						{
						hasGeneralizationWordConfirmedSpecification = generalizationWordItem.hasConfirmedSpecification();
						isGeneralizationCollectedWithItself = generalizationWordItem.isCollectedWithItself();
						isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

						do	{
							if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
								{
								if( currentGeneralizationWordItem != generalizationWordItem &&
								currentGeneralizationWordItem.isNoun() &&

								( ( !isPossessive &&
								currentGeneralizationWordItem.hasNonPossessiveAndNonQuestionPartOfSpecification() ) ||

								( !currentGeneralizationWordItem.hasCollection() &&
								!currentGeneralizationWordItem.hasPossessiveButNonQuestionSpecification() ) ) )
									{
									existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

									// Skip if concluded conclusion exists
									if( ( existingSpecificationItem == null ||
									existingSpecificationItem != userSpecificationItem ) &&

									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
										{
										// Get secondary specification for justification
										if( userSpecificationItem != null &&
										!userSpecificationItem.isPossessive() &&

										( isSpecificationCollectedWithItself ||
										userSpecificationItem.specificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() ) )
											secondarySpecificationItem = userSpecificationItem;
										else
											secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, ( isSpecificationCollectedWithItself ? Constants.NO_CONTEXT_NR : lastCreatedRelationContextNr_ ), specificationWordItem );

										if( secondarySpecificationItem != null &&

										( !isSpecificationCollectedWithItself ||
										existingSpecificationItem == null ||
										generalizationWordItem.isUserRelationWord ||
										!secondarySpecificationItem.isHiddenSpecification() ) )
											{
											if( primarySpecificationItem.isSpecificationGeneralization() )
												{
												if( primarySpecificationItem.isPartOf() &&
												// Skip creating an assumption, if a conclusion exists
												generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, isNegative, false, false, currentGeneralizationWordItem ) == null )
													{
													// Draw a "part of" generalization conclusion
													// Not yet explained in the design
													if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem.generalizationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ) ).result == Constants.RESULT_OK )
														{
														if( ( hasGeneralizationWordConfirmedSpecification ||
														!secondarySpecificationItem.hasAssumptionLevel() ) &&

														( createdJustificationItem = specificationResult.createdJustificationItem ) != null &&
														( specificationSubstitutionPartOfAssumptionJustificationItem = generalizationWordItem.specificationSubstitutionPartOfAssumptionJustificationItem( primarySpecificationItem ) ) != null )
															{
															if( generalizationWordItem.updateJustificationInSpecifications( false, false, false, specificationSubstitutionPartOfAssumptionJustificationItem, createdJustificationItem ) == Constants.RESULT_OK )
																{
																if( isSpecificationCollectedWithItself &&
																specificationSubstitutionPartOfAssumptionJustificationItem.isActiveItem() &&
																!generalizationWordItem.isJustificationInUse( specificationSubstitutionPartOfAssumptionJustificationItem ) )
																	{
																	if( generalizationWordItem.replaceOrDeleteJustification( specificationSubstitutionPartOfAssumptionJustificationItem ) != Constants.RESULT_OK )
																		return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the specification substitution part-of assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to update a justification item by a conclusion justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated specification substitution specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											}
										}
									}
								}
							else
								return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
							}
						while( !CommonVariables.hasShownWarning &&
						( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
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
						if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, generalizationAssumptionWordItem_, null ).result != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a generalization assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );

		return Constants.RESULT_OK;
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		SpecificationResultType specificationResult;
		int currentSpecificationCollectionNr;
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
									if( ( specificationResult = generalizationWordItem.findRelatedSpecification( true, isAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem, null ) ).result == Constants.RESULT_OK )
										{
										if( specificationResult.relatedSpecificationItem != null )
											negativeQuestionSpecificationWordItem = specificationResult.relatedSpecificationItem.specificationWordItem();

										// Draw an indirectly answered question conclusion
										// Not yet explained in the design
										if( ( specificationResult = addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, indirectPrimarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
											{
											if( negativeQuestionSpecificationWordItem != null &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null &&
											( commonWordItem = currentQuestionSpecificationWordItem.commonWordItem( currentSpecificationCollectionNr ) ) != null )
												{
												if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, commonWordItem ) ) != null &&
												( anotherPrimarySpecificationItem = commonWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, false, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != null )
													{
													// Draw a negative assumption
													// Not yet explained in the design
													if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, null, generalizationWordItem, negativeQuestionSpecificationWordItem, null ).result != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a negative conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + negativeQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}

											currentQuestionSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true );
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentQuestionSpecificationWordItem.anyWordTypeString() + "\"" );
										}
									else
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a related specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" that is collected with itself" );
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
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

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
						( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstSpecificationGeneralizationItem() ) != null )
							{
							do	{
								if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
									{
									if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null &&
									( anotherPrimarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != null )
										{
										// Make an only option left assumption
										// Not yet explained in the design
										if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw an only option left conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
								}
							while( !CommonVariables.hasShownWarning &&
							( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
							}
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item has no specification collection" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, SpecificationItem collectedWithItselfSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isOppositePossessiveSpecificationCollectedWithItself;
		boolean isPossessiveReversibleAssumption;
		boolean isPossessiveSpecificationCollectedWithItself;
		short assumptionJustificationTypeNr;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem feminineSpecificationWordItem;
		WordItem possessiveSpecificationWordItem;

		if( adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
			{
			if( secondarySpecificationItem != null )
				{
				if( generalizationWordItem != null )
					{
					if( specificationWordItem != null )
						{
						if( relationWordItem != null )
							{
							// Skip if global variables are not assigned
							if( possessiveSpecificationItem_ != null &&
							secondarySpecificationItem_ != null &&
							( possessiveSpecificationWordItem = possessiveSpecificationItem_.specificationWordItem() ) != null )
								{
								isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem.isCollectedWithItself();

								if( isPossessiveSpecificationCollectedWithItself ||
								possessiveSpecificationItem_.specificationCollectionNr() == secondarySpecificationItem.specificationCollectionNr() )
									{
									isPossessiveReversibleAssumption = ( secondarySpecificationItem.specificationWordItem() == possessiveSpecificationWordItem );

									if( ( ( !isPossessiveSpecificationCollectedWithItself &&
									!possessiveSpecificationWordItem.isUserSpecificationWord &&
									// Skip if possessive specification exists
									relationWordItem.secondarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) == null ) ||

									( isPossessiveSpecificationCollectedWithItself &&
									!secondarySpecificationItem.isOlderItem() &&
									collectedWithItselfSpecificationItem != null &&
									collectedWithItselfSpecificationItem.isAssignment() ) ) &&

									// Skip if user specification exists
									relationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, specificationContextNr, possessiveSpecificationWordItem ) == null )
										{
										// Make a possessive conditional specification assumption - normal
										// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
										if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, isExclusiveSpecification, false, isNegative, isPartOf, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, possessiveSpecificationItem_.specificationCollectionNr(), Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, ( isPossessive ? possessiveSpecificationItem_ : null ), null, ( isPossessiveSpecificationCollectedWithItself ? collectedWithItselfSpecificationItem : secondarySpecificationItem ), null, relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) ).result == Constants.RESULT_OK )
											{
											if( isPossessive &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
												{
												if( adminItem_.drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, !isPossessive, false, createdSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdSpecificationItem.relationContextNr(), relationWordItem, possessiveSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );
												}
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveSpecificationWordItem.anyWordTypeString() + "\"" );
										}

									if( !CommonVariables.hasShownWarning )
										{
										if( oppositePossessiveSpecificationWordItem_ != null )
											{
											isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_.isCollectedWithItself();

											isPossessiveReversibleAssumption = ( secondarySpecificationItem.specificationWordItem() == oppositePossessiveSpecificationWordItem_ );
											assumptionJustificationTypeNr = ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );

											if( ( existingSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isNegative, isPossessive, false, oppositePossessiveSpecificationWordItem_ ) ) == null &&
											( tempSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ) != null )
												{
												if( collectedWithItselfSpecificationItem != null &&
												!tempSpecificationItem.hasCurrentCreationSentenceNr() &&
												!tempSpecificationItem.hasNewInformation() &&
												tempSpecificationItem.specificationCollectionNr() == collectedWithItselfSpecificationItem.specificationCollectionNr() )
													existingSpecificationItem = tempSpecificationItem;
												}

											// Skip if assumption already exists as user specification
											if( ( existingSpecificationItem == null ||
											!existingSpecificationItem.hasRelationContext() ) &&

											( ( !isOppositePossessiveSpecificationCollectedWithItself &&
											// Skip if already exists as self-generated assumption
											relationWordItem.secondarySpecificationJustificationItem( assumptionJustificationTypeNr, secondarySpecificationItem ) == null ) ||

											( isOppositePossessiveSpecificationCollectedWithItself &&
											collectedWithItselfSpecificationItem != null ) ) )
												{
												// Make a possessive conditional specification assumption - opposite
												// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
												if( ( specificationResult = addSelfGeneratedSpecification( false, false, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, assumptionJustificationTypeNr, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, ( isPossessiveReversibleAssumption ? null : possessiveSpecificationItem_ ), null, ( isOppositePossessiveSpecificationCollectedWithItself && collectedWithItselfSpecificationItem != null ? collectedWithItselfSpecificationItem : secondarySpecificationItem ), null, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ) ).result == Constants.RESULT_OK )
													{
													if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
														{
														foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

														if( !isPossessiveReversibleAssumption &&
														foundSpecificationItem.isOlderItem() )
															{
															if( secondarySpecificationItem.isSelfGeneratedAssumption() &&
															!secondarySpecificationItem.isHiddenSpecification() &&
															secondarySpecificationItem.oppositePossessiveConditionalSpecificationAssumptionJustificationItem( foundSpecificationItem ) == null )
																{
																if( addMissingJustification( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem, possessiveSpecificationItem_, foundSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
																	return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add missing an opposite possessive conditional specification assumption justification to an assumption specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}

															assumptionSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isNegative, !isPossessive, true, oppositePossessiveSpecificationWordItem_ );

															if( assumptionSpecificationItem != null &&
															!assumptionSpecificationItem.isHiddenSpecification() &&
															assumptionSpecificationItem.possessiveReversibleAssumptionJustificationItem( foundSpecificationItem ) == null )
																{
																if( addMissingJustification( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, assumptionSpecificationItem, null, foundSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
																	return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a missing possessive reversible assumption justification to an assumption specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														}

													// Check for feminine specification word
													if( isOppositePossessiveSpecificationCollectedWithItself &&
													oppositePossessiveSpecificationWordItem_.isMasculineWord() &&
													// Select feminine specification word
													( feminineSpecificationWordItem = oppositePossessiveSpecificationWordItem_.feminineCollectionWordItem() ) != null )
														{
														if( ( isPossessive &&
														generalizationWordItem.hasFeminineProperNameEnding() ) ||

														( !isPossessive &&
														relationWordItem.hasFeminineProperNameEnding() ) )
															{
															if( addSelfGeneratedSpecification( false, false, false, isExclusiveSpecification, false, isNegative, isPartOf, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, possessiveSpecificationItem_, null, collectedWithItselfSpecificationItem, null, relationWordItem, feminineSpecificationWordItem, generalizationWordItem ).result != Constants.RESULT_OK )
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a feminine possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + feminineSpecificationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												else
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
												}
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "The opposite possessive specification item or opposite possessive specification word item is undefined" );
										}
									}
								}
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		int secondarySpecificationCollectionNr;
		SpecificationResultType specificationResult;

		if( secondarySpecificationItem != null )
			{
			if( generalizationWordItem != null )
				{
				if( relationWordItem != null )
					{
					secondarySpecificationCollectionNr = secondarySpecificationItem.specificationCollectionNr();

					// Check if specification doesn't exist yet
					if( specificationWordItem.isCollectedWithItself() ||
					generalizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, isNegative, isPossessive, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) == null )
						{
						// Check if a related specification exists
						if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, null ) ).result == Constants.RESULT_OK )
							{
							if( specificationResult.relatedSpecificationItem == null )
								{
								if( generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion() )
									{
									if( correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to correct suggestive assumptions by opposite question in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								else
									{
									// Make a suggestive question assumption
									// Not yet explained in the design
									hasFoundSuggestiveQuestionAssumption_ = true;

									if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a suggestive question assumption to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find out if a specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" is related to the secondary specification" );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given relation word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSelfGeneratedSpecification( boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkipAdditionalConclusionOrAskQuestion, boolean isSpecificationGeneralization, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasFoundSelfGeneratedAssumption;
		boolean hasFoundRelationContext = false;
		boolean hasRelationContext = ( relationWordItem != null );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		short justificationTypeNr = assumptionJustificationTypeNr;
		JustificationItem createdJustificationItem = null;
		JustificationItem foundJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;

		isAssumption_ = adminItem_.isAssumption( assumptionJustificationTypeNr );
		lastCreatedRelationContextNr_ = Constants.NO_CONTEXT_NR;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				// Check assumption
				if( isAssumption_ &&
				adminItem_.assumptionGrade( assumptionJustificationTypeNr ) == 0 &&

				( primarySpecificationItem == null ||
				!primarySpecificationItem.hasAssumptionLevel() ) &&

				( anotherPrimarySpecificationItem == null ||
				!anotherPrimarySpecificationItem.hasAssumptionLevel() ) &&

				( secondarySpecificationItem == null ||
				!secondarySpecificationItem.hasAssumptionLevel() ) )
					{
					// Is conclusion
					if( adminItem_.isConclusion( conclusionJustificationTypeNr ) )
						{
						isAssumption_ = false;
						justificationTypeNr = conclusionJustificationTypeNr;
						}
					else
						myWordItem_.startErrorInItem( 1, moduleNameString_, "The given conclusion justification type isn't a conclusion" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( isAssumption_ ||
					questionParameter > Constants.NO_QUESTION_PARAMETER ||

					// Check conclusion justifications
					( ( primarySpecificationItem == null ||
					!primarySpecificationItem.isQuestion() ) &&

					( secondarySpecificationItem == null ||
					!secondarySpecificationItem.isQuestion() ) ) )
						{
						if( ( justificationResult = generalizationWordItem.addJustification( isForcingNewJustification, justificationTypeNr, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, null ) ).result == Constants.RESULT_OK )
							{
							createdJustificationItem = justificationResult.createdJustificationItem;
							foundJustificationItem = justificationResult.foundJustificationItem;

							if( ( createdOrFoundJustificationItem = ( createdJustificationItem == null ? foundJustificationItem : createdJustificationItem ) ) != null )
								{
								if( ( specificationResult = adminItem_.addSpecification( isAssignment, false, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation( justificationTypeNr ), prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result == Constants.RESULT_OK )
									{
									if( !CommonVariables.hasShownWarning )
										{
										createdSpecificationItem = specificationResult.createdSpecificationItem;

										if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
											hasFoundRelationContext = foundSpecificationItem.hasRelationContext();

										if( !isPossessive &&
										justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
										!specificationWordItem.isCollectedWithItself() )
											{
											secondarySpecificationItem_ = ( foundSpecificationItem != null &&

																			( !foundSpecificationItem.isOlderItem() ||
																			myWordItem_.nContextWordsInAllWords( foundSpecificationItem.relationContextNr(), specificationWordItem ) > CommonVariables.nUserRelationWords ) ? foundSpecificationItem : createdSpecificationItem );
											}

										if( createdSpecificationItem == null )
											{
											if( foundSpecificationItem != null )
												{
												hasFoundSelfGeneratedAssumption = foundSpecificationItem.isSelfGeneratedAssumption();

												if( hasFoundRelationContext &&
												hasFoundSelfGeneratedAssumption &&
												generalizationWordItem.isUserGeneralizationWord &&
												justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION )
													{
													// Check for confirmation of at least one relation word
													if( generalizationWordItem.checkForConfirmedJustifications( secondarySpecificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for confirmed justifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												if( CommonVariables.result == Constants.RESULT_OK )
													{
													if( foundSpecificationItem.isSelfGenerated() )
														{
														if( createdJustificationItem == null )
															{
															if( foundSpecificationItem.hasCurrentCreationSentenceNr() &&
															!foundSpecificationItem.hasFoundJustification( false, foundJustificationItem ) )
																{
																// Change the justification of the found specification by the found justification
																if( foundSpecificationItem.changeFirstJustification( foundJustificationItem ) != Constants.RESULT_OK )
																	myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to change the first justification of the found justification by a conclusion justification" );
																}
															}
														else
															{
															// A justification has been created, but the self-generated specification already exists
															// So, the justification needs to be added or replaced separately
															if( hasFoundSelfGeneratedAssumption ||

															( !isAssumption_ &&
															foundSpecificationItem.isSelfGeneratedConclusion() ) )
																{
																// Attach created justification to the found specification
																if( generalizationWordItem.attachJustification( createdJustificationItem, foundSpecificationItem ) == Constants.RESULT_OK )
																	{
																	if( isAssumption_ &&
																	isGeneralizationProperName &&
																	!isPossessive &&

																	( isNegative ||

																	( hasRelationContext &&
																	generalizationWordItem.isUserRelationWord ) ||

																	// Assumption will be concluded
																	( !hasRelationContext &&
																	!generalizationWordItem.hasFoundAnswerToQuestion() ) ) )
																		{
																		if( foundSpecificationItem.isHiddenSpecification() )
																			// Clear sentence write buffer of hidden specification
																			foundSpecificationItem.lastWrittenSentenceStringBuffer = null;
																		else
																			{
																			if( generalizationWordItem.recalculateAssumptions() != Constants.RESULT_OK )
																				myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to recalculate assumptions of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																			}
																		}
																	}
																else
																	myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														}
													else
														{
														if( createdJustificationItem != null &&
														!generalizationWordItem.isJustificationInUse( createdJustificationItem ) )
															myWordItem_.startErrorInItem( 1, moduleNameString_, "A justification has been created, but the specification already exists" );
														}
													}
												}
											else
												myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find or create a specification" );
											}
										else
											{
											lastCreatedRelationContextNr_ = createdSpecificationItem.relationContextNr();

											if( !hasRelationContext ||
											!hasFoundRelationContext ||
											generalizationWordItem.hasConfirmedSpecification() )
												{
												// Check self-generated specification for integrity
												if( generalizationWordItem.writeSelectedSpecification( true, true, false, false, Constants.NO_ANSWER_PARAMETER, createdSpecificationItem ) == Constants.RESULT_OK )
													{
													if( CommonVariables.writeSentenceStringBuffer != null &&
													CommonVariables.writeSentenceStringBuffer.length() > 0 )
														{
														if( hasRelationContext &&
														isGeneralizationProperName )
															{
															if( adminItem_.collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, lastCreatedRelationContextNr_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
																myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );
															}
														}
													else
														myWordItem_.startErrorInItem( 1, moduleNameString_, "Integrity error! I couldn't write the self-generated specification with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
													}
												else
													myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the self-generated specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to check the writing integrity" );
												}
											else
												{
												if( foundSpecificationItem != null &&
												foundSpecificationItem.relationContextNr() == lastCreatedRelationContextNr_ )
													myWordItem_.startErrorInItem( 1, moduleNameString_, "Integrity error! A specification was created in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" while this specification was found as well" );
												}
											}

										if( CommonVariables.result == Constants.RESULT_OK &&
										!isSkipAdditionalConclusionOrAskQuestion &&
										adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) &&

										( createdSpecificationItem != null ||

										( createdJustificationItem != null &&
										createdJustificationItem.isActiveItem() ) ) )
											{
											if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption_, false, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) == Constants.RESULT_OK )
												{
												if( isUniqueUserRelation &&
												createdSpecificationItem != null )
													{
													// Draw a unique relation conclusion
													// Not yet explained in the design
													if( addSelfGeneratedSpecification( false, false, isEveryGeneralization, isExclusiveSpecification, false, !isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, createdSpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								else
									myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a specification" );
								}
							else
								myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find or create a justification" );
							}
						else
							myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a justification" );
						}
					else
						myWordItem_.startErrorInItem( 1, moduleNameString_, "The primary specification item or secondary specification item of this conclusion, is a question or an assumption, which can't be a justification for a conclusion" );
					}
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.createdJustificationItem = createdJustificationItem;
		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"He is my loving ally and my fortress,
 *	my tower of safety, my rescuer.
 *	He is my shield, and I take refuge in him.
 *	He makes the nations submit to me." (Psalm 144:2)
 *************************************************************************/
