/*
 *	Class:			AdminAssumption
 *	Supports class:	AdminItem
 *	Purpose:		To create assumptions autonomously
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

class AdminAssumption
	{
	// Private constructible variables

	private boolean hasFoundSuggestiveQuestionAssumption_;
	private boolean isAssumption_;

	private short justificationTypeNr_;

	private int lastCreatedRelationContextNr_;

	private SpecificationItem foundPossessiveSpecificationItem_;
	private SpecificationItem foundOppositePossessiveSpecificationItem_;
	private SpecificationItem possessiveSpecificationItem_;
	private SpecificationItem secondarySpecificationItem_;

	private WordItem generalizationAssumptionWordItem_;
	private WordItem oppositePossessiveSpecificationWordItem_;
	private WordItem possessiveSpecificationWordItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private static boolean isUniqueRelation( short justificationTypeNr )
		{
		return ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );
		}

	private static boolean isSpecificationGeneralizationJustification( short justificationTypeNr )
		{
		return ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION ||

				justificationTypeNr == Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION ||
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION );
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
									if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, isNegative, false, isPossessive, false, false, false, oldSuggestiveQuestionAssumptionAssignmentItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, oldSuggestiveQuestionAssumptionAssignmentItem, null, generalizationWordItem, oldSuggestiveSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
										{
										if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
											{
											// Correct new question
											if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, isNegative, false, isPossessive, false, false, false, secondarySpecificationItem.assumptionLevel(), Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION, Constants.NO_PREPOSITION_PARAMETER, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result == Constants.RESULT_OK )
												{
												if( ( oldCorrectedAssumptionByOppositeQuestionJustificationItem = generalizationWordItem.oldCorrectedAssumptionByOppositeQuestionJustificationItem() ) != null )
													{
													if( ( secondarySpecificationWordItem = oldCorrectedAssumptionByOppositeQuestionJustificationItem.secondarySpecificationWordItem() ) != null )
														{
														// Correct generalization assumption
														if( ( primarySpecificationItem = specificationWordItem.firstSpecificationItem( isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != null )
															{
															if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, secondarySpecificationWordItem, null ) ).result == Constants.RESULT_OK )
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
						while( ( possessiveSpecificationItem = possessiveSpecificationItem.nextNonQuestionSpecificationItem() ) != null );

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
								while( ( possessiveSpecificationItem = possessiveSpecificationItem.nextNonQuestionSpecificationItem() ) != null );
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
			// Skip when already exists
			if( generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) == null )
				{
				// Make a generalization assumption
				// Not yet explained in the design
				if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ) ).result == Constants.RESULT_OK )
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

		justificationTypeNr_ = Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION;

		lastCreatedRelationContextNr_ = Constants.NO_CONTEXT_NR;

		foundPossessiveSpecificationItem_ = null;
		foundOppositePossessiveSpecificationItem_ = null;
		possessiveSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

		generalizationAssumptionWordItem_ = null;
		oppositePossessiveSpecificationWordItem_ = null;
		possessiveSpecificationWordItem_ = null;

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

		justificationTypeNr_ = Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION;

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
		SpecificationItem createdOrFoundSpecificationItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem userSpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem = null;
		WordItem currentWordItem;

		possessiveSpecificationItem_ = null;
		secondarySpecificationItem_ = null;

		oppositePossessiveSpecificationWordItem_ = null;
		possessiveSpecificationWordItem_ = null;

		if( generalizationWordItem != null )
			{
			if( specificationWordItem != null )
				{
				userSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

				if( userSpecificationItem != null &&

				( hasRelationContext ||
				generalizationWordItem.hasConfirmedSpecificationButNoRelation() ||

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

										if( ( possessiveSpecificationWordItem_ = possessiveSpecificationItem_.specificationWordItem() ) != null )
											{
											if( ( primarySpecificationItem = possessiveSpecificationWordItem_.firstActiveAssignmentOrSpecificationItem( false, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, specificationWordItem ) ) != null )
												{
												isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem_.isCollectedWithItself();

												if( ( !userSpecificationItem.hasRelationContext() &&
												generalizationWordItem.hasAnsweredQuestion( primarySpecificationItem.specificationCollectionNr() ) ) ||

												( isPossessiveSpecificationCollectedWithItself &&
												!userSpecificationItem.isActiveAssignment() &&
												userSpecificationItem.specificationWordItem() == possessiveSpecificationWordItem_ ) )
													{
													// Skip making exclusive specification substitution assumption
													if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( true, false, isArchivedAssignment, isExclusiveSpecification, isNegative, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, possessiveSpecificationWordItem_, relationWordItem ) != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, possessiveSpecificationWordItem_ );

													if( existingUserSpecificationItem == null ||

													// Confirmed specification without relation word
													( relationWordItem != null &&

													( isPossessiveSpecificationCollectedWithItself ||
													!existingUserSpecificationItem.hasRelationContext() ) ) )
														{
														// Make an exclusive specification substitution assumption - normal
														// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
														if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, possessiveSpecificationWordItem_, relationWordItem ) ).result == Constants.RESULT_OK )
															{
															if( ( createdOrFoundSpecificationItem = ( specificationResult.createdSpecificationItem == null ? specificationResult.foundSpecificationItem : specificationResult.createdSpecificationItem ) ) != null )
																{
																if( isArchivedAssignment )
																	secondarySpecificationItem_ = generalizationWordItem.firstAssignmentItem( false, false, true, isPossessive, createdOrFoundSpecificationItem.questionParameter(), createdOrFoundSpecificationItem.relationContextNr(), createdOrFoundSpecificationItem.specificationWordItem() );
																else
																	secondarySpecificationItem_ = createdOrFoundSpecificationItem.updatedSpecificationItem();
																}
															else
																return myWordItem_.startErrorInItem( 1, moduleNameString_, "No specification item was created nor found" );
															}
														else
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and possessive specification word \"" + possessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										}

									// First opposite possessive specification
									if( !CommonVariables.hasShownWarning &&
									possessiveSpecificationWordItem_ != null &&
									foundOppositePossessiveSpecificationItem_ != null &&
									( oppositePossessiveSpecificationWordItem_ = foundOppositePossessiveSpecificationItem_.specificationWordItem() ) != null )
										{
										isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_.isCollectedWithItself();

										if( secondarySpecificationItem_ == null )
											{
											secondarySpecificationItem_ = userSpecificationItem;

											// Find another primary definition
											if( !hasRelationContext ||
											isPossessive )
												anotherPrimarySpecificationItem = possessiveSpecificationWordItem_.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );
											}

										if( secondarySpecificationItem_ != null &&

										// Skip when already exists
										( !secondarySpecificationItem_.isOlderItem() ||
										secondarySpecificationItem_.isSelfGenerated() ) &&

										// Skip when user specification exists
										generalizationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, oppositePossessiveSpecificationWordItem_ ) == null )
											{
											// Make an exclusive specification substitution assumption - opposite
											// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
											if( ( specificationResult = addSelfGeneratedSpecification( false, false, false, false, isNegative, false, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, relationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, foundOppositePossessiveSpecificationItem_, ( isOppositePossessiveSpecificationCollectedWithItself ? null : anotherPrimarySpecificationItem ), ( isOppositePossessiveSpecificationCollectedWithItself ? userSpecificationItem : secondarySpecificationItem_ ), null, generalizationWordItem, oppositePossessiveSpecificationWordItem_, relationWordItem ) ).result == Constants.RESULT_OK )
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
									( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
										{
										// Get secondary specification for justification
										if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
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
											if( ( secondarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, specificQuestionParameter, specificationWordItem ) ) != null )
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
									existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem );

									// Skip if concluded conclusion exists
									if( ( existingSpecificationItem == null ||
									existingSpecificationItem != userSpecificationItem ) &&

									// Get primary specification for justification
									( primarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) ) != null )
										{
										// Get secondary specification for justification
										if( userSpecificationItem != null &&
										!userSpecificationItem.isPossessive() &&

										( isSpecificationCollectedWithItself ||
										userSpecificationItem.specificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() ) )
											secondarySpecificationItem = userSpecificationItem;
										else
											secondarySpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, isArchivedAssignment, false, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, ( isSpecificationCollectedWithItself ? Constants.NO_CONTEXT_NR : lastCreatedRelationContextNr_ ), specificationWordItem );

										if( secondarySpecificationItem != null &&

										( !isSpecificationCollectedWithItself ||
										existingSpecificationItem == null ||
										generalizationWordItem.isUserRelationWord ||
										!secondarySpecificationItem.isHiddenSpecification() ) )
											{
											if( primarySpecificationItem.isSpecificationGeneralization() )
												{
												if( primarySpecificationItem.isPartOf() &&
												// Skip creating an assumption, when a conclusion already exists
												generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( true, true, isNegative, false, false, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) == null )
													{
													// When collected afterwards
													if( !secondarySpecificationItem.hasAssumptionLevel() &&
													!secondarySpecificationItem.isCorrectedAssumption() &&
													secondarySpecificationItem.isSelfGeneratedAssumption() )
														{
														if( secondarySpecificationItem.recalculateAssumption() != Constants.RESULT_OK )
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to recalculate the secondary specification item" );
														}

													// Draw a "part of" generalization conclusion
													// Not yet explained in the design
													if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, isNegative, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, currentGeneralizationItem.generalizationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, secondarySpecificationItem, null, primarySpecificationItem, null, generalizationWordItem, currentGeneralizationWordItem, null ) ).result == Constants.RESULT_OK )
														{
														if( !secondarySpecificationItem.hasAssumptionLevel() &&
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
					( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
					( secondarySpecificationItem = specificationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, generalizationAssumptionWordItem_ ) ) != null )
						{
						// Make an exclusive specification substitution assumption
						// See design: paragraph 2.4.1 in the theory document or http://mafait.org/theory_2_4_1/
						if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, null, secondarySpecificationItem, null, generalizationWordItem, generalizationAssumptionWordItem_, null ).result != Constants.RESULT_OK )
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
									if( ( specificationResult = generalizationWordItem.findRelatedSpecification( true, isAssignment, isInactiveAssignment, isArchivedAssignment, true, false, false, currentQuestionSpecificationItem.questionParameter(), currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem, null ) ).result == Constants.RESULT_OK )
										{
										if( specificationResult.relatedSpecificationItem != null )
											negativeQuestionSpecificationWordItem = specificationResult.relatedSpecificationItem.specificationWordItem();

										// Draw an indirectly answered question conclusion
										// Not yet explained in the design
										if( ( specificationResult = addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, indirectPrimarySpecificationItem, null, userSpecificationItem, null, generalizationWordItem, currentQuestionSpecificationWordItem, null ) ).result == Constants.RESULT_OK )
											{
											if( negativeQuestionSpecificationWordItem != null &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null &&
											( commonWordItem = currentQuestionSpecificationWordItem.commonWordItem( currentSpecificationCollectionNr ) ) != null )
												{
												if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, false, false, Constants.NO_QUESTION_PARAMETER, commonWordItem ) ) != null &&
												( anotherPrimarySpecificationItem = commonWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, false, false, currentSpecificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentQuestionSpecificationWordItem ) ) != null )
													{
													// Draw a negative assumption
													// Not yet explained in the design
													if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, currentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, createdSpecificationItem, null, generalizationWordItem, negativeQuestionSpecificationWordItem, null ).result != Constants.RESULT_OK )
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
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given user specfication item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specfication word item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		short nOptionsLeft = 0;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem currentSpecificationWordItem;
		WordItem onlyOptionLeftSpecificationWordItem = null;

		if( userSpecificationItem != null )
			{
			if( generalizationWordItem != null )
				{
				if( userSpecificationItem.hasSpecificationCollection() )
					{
					if( ( currentSpecificationItem = generalizationWordItem.firstSelectedSpecificationItem( isArchivedAssignment, false, isArchivedAssignment, true ) ) != null )
						{
						do	{
							if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
								{
								if( generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, true, isPossessive, Constants.NO_QUESTION_PARAMETER, currentSpecificationWordItem ) == null )
									{
									nOptionsLeft++;
									onlyOptionLeftSpecificationWordItem = currentSpecificationWordItem;
									}
								}
							}
						while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );

						if( nOptionsLeft == 1 &&
						onlyOptionLeftSpecificationWordItem != null &&
						( currentGeneralizationItem = onlyOptionLeftSpecificationWordItem.firstSpecificationGeneralizationItem() ) != null )
							{
							do	{
								if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
									{
									if( ( primarySpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, currentGeneralizationWordItem ) ) != null &&
									( anotherPrimarySpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, onlyOptionLeftSpecificationWordItem ) ) != null )
										{
										// Make an only option left assumption
										// Not yet explained in the design
										if( adminItem_.addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, true, false, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION, Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, userSpecificationItem.generalizationWordTypeNr(), userSpecificationItem.specificationWordTypeNr(), Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, primarySpecificationItem, anotherPrimarySpecificationItem, userSpecificationItem, null, generalizationWordItem, onlyOptionLeftSpecificationWordItem, null ).result != Constants.RESULT_OK )
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

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isOppositePossessiveSpecificationCollectedWithItself;
		boolean isPossessiveReversibleAssumption;
		boolean isPossessiveSpecificationCollectedWithItself;
		boolean isUserSpecificationOlderItem = false;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingAssumptionSpecificationItem;
		SpecificationItem existingUserSpecificationItem;

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
							if( possessiveSpecificationItem_ != null &&
							possessiveSpecificationWordItem_ != null &&
							// Skip if global variable secondarySpecificationItem_ is not assigned
							secondarySpecificationItem_ != null )
								{
								isPossessiveSpecificationCollectedWithItself = possessiveSpecificationWordItem_.isCollectedWithItself();

								if( isPossessiveSpecificationCollectedWithItself ||
								possessiveSpecificationItem_.specificationCollectionNr() == secondarySpecificationItem.specificationCollectionNr() )
									{
									if( userSpecificationItem != null &&
									userSpecificationItem.isOlderItem() )
										isUserSpecificationOlderItem = true;

									existingAssumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, !isPossessive, Constants.NO_QUESTION_PARAMETER, possessiveSpecificationWordItem_, generalizationWordItem );
									existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( isNegative, !isPossessive, Constants.NO_COLLECTION_NR, specificationContextNr, possessiveSpecificationWordItem_ );

									// Skip if user specification exists
									if( existingUserSpecificationItem == null &&

									( ( !isPossessiveSpecificationCollectedWithItself &&
									!possessiveSpecificationWordItem_.isUserSpecificationWord &&

									( !isUserSpecificationOlderItem ||
									existingAssumptionSpecificationItem == null ||
									!existingAssumptionSpecificationItem.isOlderItem() ) ) ||

									( isPossessiveSpecificationCollectedWithItself &&
									userSpecificationItem != null &&
									userSpecificationItem.isAssignment() ) ) )
										{
										// Make a possessive conditional specification assumption - normal
										// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
										if( ( specificationResult = addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, isExclusiveSpecification, isNegative, isPartOf, !isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_BACK_FIRED_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, secondarySpecificationItem.specificationWordTypeNr(), generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, ( isPossessive ? possessiveSpecificationItem_ : null ), null, ( isPossessiveSpecificationCollectedWithItself ? userSpecificationItem : secondarySpecificationItem ), null, relationWordItem, possessiveSpecificationWordItem_, generalizationWordItem ) ).result == Constants.RESULT_OK )
											{
											if( isPossessive &&
											( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
												{
												if( adminItem_.drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, !isPossessive, false, createdSpecificationItem.assumptionLevel(), relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, specificationContextNr, createdSpecificationItem.relationContextNr(), relationWordItem, possessiveSpecificationWordItem_, generalizationWordItem ) != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a possessive reversible conclusion from generalization word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + possessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
												}
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption about relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + possessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
										}

									if( !CommonVariables.hasShownWarning )
										{
										if( oppositePossessiveSpecificationWordItem_ != null )
											{
											isOppositePossessiveSpecificationCollectedWithItself = oppositePossessiveSpecificationWordItem_.isCollectedWithItself();
											existingAssumptionSpecificationItem = relationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, oppositePossessiveSpecificationWordItem_, generalizationWordItem );
											existingUserSpecificationItem = relationWordItem.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, oppositePossessiveSpecificationWordItem_ );

											if( isOppositePossessiveSpecificationCollectedWithItself &&
											userSpecificationItem != null )
												secondarySpecificationItem = userSpecificationItem;

											// Skip when assumption already exists as user specification
											if( ( existingUserSpecificationItem == null ||
											!existingUserSpecificationItem.hasRelationContext() ) &&

											( isOppositePossessiveSpecificationCollectedWithItself ||
											!isUserSpecificationOlderItem ||
											existingAssumptionSpecificationItem == null ||
											!existingAssumptionSpecificationItem.isOlderItem() ) )
												{
												isPossessiveReversibleAssumption = ( oppositePossessiveSpecificationWordItem_ == secondarySpecificationItem.specificationWordItem() );

												// Make a possessive conditional specification assumption - opposite
												// See design: paragraph 2.4.2 in the theory document or http://mafait.org/theory_2_4_2/
												if( addSelfGeneratedSpecification( false, false, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, ( isPossessiveReversibleAssumption ? Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION : Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ), Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, relationWordTypeNr, Constants.WORD_TYPE_NOUN_SINGULAR, generalizationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationContextNr, generalizationContextNr, ( isPossessiveReversibleAssumption ? null : possessiveSpecificationItem_ ), null, ( isOppositePossessiveSpecificationCollectedWithItself && userSpecificationItem != null ? userSpecificationItem : secondarySpecificationItem ), null, relationWordItem, oppositePossessiveSpecificationWordItem_, generalizationWordItem ).result != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a possessive conditional specification assumption from relation word \"" + relationWordItem.anyWordTypeString() + "\" to specification word \"" + oppositePossessiveSpecificationWordItem_.anyWordTypeString() + "\"" );
												}
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "The opposite possessive specification word item is undefined" );
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
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given secondary specification item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word type isn't a noun" );

		return Constants.RESULT_OK;
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult;

		if( secondarySpecificationItem != null )
			{
			if( generalizationWordItem != null )
				{
				if( relationWordItem != null )
					{
					// Check if specification doesn't exist yet
					if( specificationWordItem.isCollectedWithItself() ||
					generalizationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, false, isNegative, isPossessive, secondarySpecificationItem.specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) == null )
						{
						// Check if a related specification exists
						if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, secondarySpecificationItem.specificationCollectionNr(), generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, null ) ).result == Constants.RESULT_OK )
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

									if( addSelfGeneratedSpecification( isArchivedAssignment, isArchivedAssignment, false, false, isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, secondarySpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
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

	protected SpecificationResultType addSelfGeneratedSpecification( boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isForcingNewJustification, boolean isSkipAdditionalConclusionOrAskQuestion, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasFoundSelfGeneratedAssumption;
		boolean hasFoundOlderItem = false;
		boolean hasFoundRelationContext = false;
		boolean hasRelationContext = ( relationWordItem != null );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		JustificationItem createdJustificationItem = null;
		JustificationItem foundJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;

		isAssumption_ = adminItem_.isAssumption( assumptionJustificationTypeNr );
		justificationTypeNr_ = assumptionJustificationTypeNr;
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
						justificationTypeNr_ = conclusionJustificationTypeNr;
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
						if( ( justificationResult = generalizationWordItem.addJustification( isForcingNewJustification, justificationTypeNr_, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, null ) ).result == Constants.RESULT_OK )
							{
							createdJustificationItem = justificationResult.createdJustificationItem;
							foundJustificationItem = justificationResult.foundJustificationItem;

							if( ( createdOrFoundJustificationItem = ( createdJustificationItem == null ? foundJustificationItem : createdJustificationItem ) ) != null )
								{
								if( ( specificationResult = adminItem_.addSpecification( isAssignment, false, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecificationGeneralizationJustification( justificationTypeNr_ ), isUniqueRelation( justificationTypeNr_ ), prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result == Constants.RESULT_OK )
									{
									if( !CommonVariables.hasShownWarning )
										{
										createdSpecificationItem = specificationResult.createdSpecificationItem;

										if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
											{
											hasFoundOlderItem = foundSpecificationItem.isOlderItem();
											hasFoundRelationContext = foundSpecificationItem.hasRelationContext();
											}

										if( !isPossessive &&
										justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
										!specificationWordItem.isCollectedWithItself() )
											secondarySpecificationItem_ = ( !hasFoundOlderItem && foundSpecificationItem != null ? foundSpecificationItem : createdSpecificationItem );

										if( createdSpecificationItem == null )
											{
											if( foundSpecificationItem != null )
												{
												hasFoundSelfGeneratedAssumption = foundSpecificationItem.isSelfGeneratedAssumption();

												if( hasFoundRelationContext &&
												hasFoundSelfGeneratedAssumption &&
												justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION )
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
															if( hasFoundOlderItem &&
															!foundSpecificationItem.hasFoundJustification( foundJustificationItem ) )
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
																	if( isGeneralizationProperName &&
																	!isPossessive &&

																	( isAssumption_ ||
																	isPartOf ) &&

																	( hasRelationContext ||
																	isNegative ||
																	!generalizationWordItem.hasFoundAnswerToQuestion() ) )
																		{
																		if( generalizationWordItem.recalculateAssumptions( false ) != Constants.RESULT_OK )
																			myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to recalculate assumptions of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
															{
															if( generalizationWordItem.replaceOrDeleteJustification( createdJustificationItem ) != Constants.RESULT_OK )
																myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
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
													if( addSelfGeneratedSpecification( false, false, isEveryGeneralization, isExclusiveSpecification, !isNegative, false, isPossessive, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, createdSpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
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
 *
 *	"He is my loving ally and my fortress,
 *	my tower of safety, my rescuer.
 *	He is my shield, and I take refuge in him.
 *	He makes the nations submit to me." (Psalm 144:2)
 *
 *************************************************************************/
