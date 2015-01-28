/*
 *	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
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

class WordSpecification
	{
	// Private constructible variables

	private boolean hasConfirmedSpecification_;
	private boolean hasCorrectedAssumptionByKnowledge_;
	private boolean hasCorrectedAssumptionByOppositeQuestion_;
	private boolean hasShownMoreSpecificRelatedQuestion_;
	private boolean isConfirmedAssignment_;
	private boolean isConfirmedExclusive_;
	private boolean isNonExclusiveSpecification_;
	private boolean isOnlyCheckingForConflicts_;
	private boolean isSameQuestionFromUser_;
	private boolean isSimilarOrRelatedQuestion_;
	private boolean isStopDetectingQuestion_;

	private int userSpecificationCollectionNr_;

	private SpecificationItem confirmedReplacedSpecificationItem_;
	private SpecificationItem confirmedSpecificationButNoRelationReplacedSpecificationItem_;
	private SpecificationItem conflictingSpecificationItem_;
	private SpecificationItem correctedAssumptionReplacedSpecificationItem_;
	private SpecificationItem replacedAssignmentItem_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte addGeneralization( boolean isAddSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		GeneralizationResultType generalizationResult;

		if( specificationWordItem != null )
			{
			if( isAddSpecification )
				{
				if( ( generalizationResult = specificationWordItem.findGeneralization( false, myWordItem_ ) ).result == Constants.RESULT_OK )
					{
					if( !generalizationResult.hasFoundGeneralization )
						{
						// Specification word of a generalization word: noun
						if( specificationWordItem.createGeneralization( false, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a generalization item" );
						}
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a generalization item" );
				}

			if( relationWordItem != null )
				{
				if( ( generalizationResult = relationWordItem.findGeneralization( true, myWordItem_ ) ).result == Constants.RESULT_OK )
					{
					if( !generalizationResult.hasFoundGeneralization )
						{
						// Relation word of a generalization word: proper noun
						if( relationWordItem.createGeneralization( true, relationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a relation generalization item" );
						}
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a relation generalization item" );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkSpecificationWordForConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult;
		boolean isSpecificationCollectedWithItself;
		boolean hasFoundCompoundConflict = false;
		int nonCompoundSpecificationCollectionNr;
		int compoundSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem tempConflictingSpecificationItem;
		SpecificationItem pastTenseAssignmentItem = null;

		conflictingSpecificationItem_ = null;

		if( specificationWordItem != null )
			{
			// Derive conflict
			if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) ).result == Constants.RESULT_OK )
				{
				if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) == null )
					{
					isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

					// Check looping
					if( isSpecificationCollectedWithItself ||
					( conflictingSpecificationItem_ = specificationWordItem.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, myWordItem_ ) ) == null )
						{
						// Check past tense
						foundSpecificationItem = myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem );

						if( foundSpecificationItem != null &&
						foundSpecificationItem.isArchivedAssignment() != isArchivedAssignment )
							conflictingSpecificationItem_ = foundSpecificationItem;
						else
							{
							if( !isSpecificationCollectedWithItself &&
							( compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
								{
								// Check negative
								foundSpecificationItem = myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

								if( foundSpecificationItem != null &&
								!specificationWordItem.isUserSpecificationWord &&
								foundSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
									hasFoundCompoundConflict = true;

								if( !isNegative &&
								foundSpecificationItem != null &&

								( hasFoundCompoundConflict ||
								foundSpecificationItem.isUserSpecification() ||

								( compoundSpecificationCollectionNr != specificationCollectionNr &&
								foundSpecificationItem.hasSpecificationCompoundCollection() ) ) )
									{
									// First check other conflict
									if( ( conflictingSpecificationItem_ = myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, null ) ) != null )
										{
										// Write conflict
										if( writeSpecificationConflict( isExclusiveSpecification, specificationCollectionNr, conflictingSpecificationItem_ ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the conflicting specification" );
										}

									conflictingSpecificationItem_ = foundSpecificationItem;
									}
								else
									{
									// Check for existing compound specification
									// No self-generated question available for this specification
									if( ( nonCompoundSpecificationCollectionNr = myWordItem_.nonCompoundCollectionNrInAllWords( compoundSpecificationCollectionNr ) ) > Constants.NO_COLLECTION_NR )
										{
										tempConflictingSpecificationItem = myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, nonCompoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, null );

										if( tempConflictingSpecificationItem == null )
											{
											tempConflictingSpecificationItem = myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, null );

											if( tempConflictingSpecificationItem != null &&
											!tempConflictingSpecificationItem.isOlderItem() &&
											tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem )
												conflictingSpecificationItem_ = tempConflictingSpecificationItem;
											}
										else
											{
											if( tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem )
												conflictingSpecificationItem_ = tempConflictingSpecificationItem;
											}
										}
									}
								}
							}
						}
					}
				else
					{
					// Find possible past tense assignment of relatedSpecificationItem
					pastTenseAssignmentItem = myWordItem_.firstNonQuestionAssignmentItem( false, false, true, isNegative, isPossessive, relationContextNr, relatedSpecificationItem.specificationWordItem() );

					conflictingSpecificationItem_ = ( pastTenseAssignmentItem == null ? relatedSpecificationItem : pastTenseAssignmentItem );
					}

				if( !hasFoundCompoundConflict &&
				conflictingSpecificationItem_ != null &&
				!conflictingSpecificationItem_.hasAlreadyBeenWrittenAsConflict &&

				( specificationCollectionNr == Constants.NO_COLLECTION_NR ||
				specificationWordItem.isExclusiveCollection( specificationCollectionNr ) ) )
					{
					// Write conflict
					if( writeSpecificationConflict( isExclusiveSpecification, specificationCollectionNr, conflictingSpecificationItem_ ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the conflicting specification" );
					}
				}
			else
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkUserQuestion( boolean hasRelationContext, boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem foundQuestionSpecificationItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult;
		SpecificationItem relatedSpecificationItem;

		if( specificationWordItem != null )
			{
			if( foundQuestionSpecificationItem != null &&
			foundQuestionSpecificationItem.isAnsweredQuestion() &&
			foundQuestionSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification &&
			foundQuestionSpecificationItem.relationContextNr() == relationContextNr )
				CommonVariables.isQuestionAlreadyAnswered = true;

			if( foundQuestionSpecificationItem == null ||
			!foundQuestionSpecificationItem.isQuestion() )
				{
				if( ( specificationResult = findRelatedSpecification( false, false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) ).result == Constants.RESULT_OK )
					{
					if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null &&
					!isExclusiveSpecification &&
					!isSpecificationGeneralization &&
					specificationWordItem.isExclusiveCollection( specificationCollectionNr ) )
						{
						if( relatedSpecificationItem.isOlderItem() )
							{
							if( hasRelationContext )
								{
								if( correctedAssumptionReplacedSpecificationItem_ == null )
									{
									if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, true, true, relatedSpecificationItem.isNegative(), relatedSpecificationItem.isPossessive(), true, Constants.NO_QUESTION_PARAMETER, relatedSpecificationItem.specificationCollectionNr(), relatedSpecificationItem.relationContextNr(), relatedSpecificationItem.specificationWordItem() ) ) != null )
										{
										if( myWordItem_.writeUpdatedSpecification( false, false, true, correctedAssumptionReplacedSpecificationItem_ ) == Constants.RESULT_OK )
											hasCorrectedAssumptionByOppositeQuestion_ = true;
										else
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );
										}
									}
								else
									return myWordItem_.startErrorInWord( 1, moduleNameString_, "The corrected assumption replaced specification item already assigned" );
								}
							}
						else
							{
							if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related question" );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkUserSpecification( boolean hasRelationContext, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isValueSpecification, int specificationCollectionNr, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem specificationWordItem )
		{
		if( foundSpecificationItem != null )
			{
			if( specificationWordItem != null )
				{
				if( foundSpecificationItem.isValueSpecification() == isValueSpecification )
					{
					if( !isAssignment &&
					!isExclusiveSpecification &&
					foundSpecificationItem.isExclusiveSpecification() &&
					foundSpecificationItem.isUserSpecification() )
						{
						if( writeMoreSpecificSpecification( foundSpecificationItem ) == Constants.RESULT_OK )
							isNonExclusiveSpecification_ = true;
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );
						}
					else
						{
						if( !isSelection &&
						hasRelationContext &&
						foundSpecificationItem.isOlderItem() &&
						!foundSpecificationItem.hasRelationContext() )
							{
							if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific related specification" );
							}

						if( foundSpecificationItem.hasRelationContext() &&
						foundSpecificationItem.relationContextNr() != relationContextNr &&
						foundSpecificationItem.isInactiveAssignment() == isInactiveAssignment &&
						foundSpecificationItem.isArchivedAssignment() == isArchivedAssignment &&

						( !hasRelationContext ||
						// Found specification has more than one relation words
						myWordItem_.nContextWordsInAllWords( foundSpecificationItem.relationContextNr(), specificationWordItem ) > 1 ) )
							{
							if( myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, relationContextNr, specificationWordItem ) == null )
								{
								if( myWordItem_.writeSelectedSpecification( true, foundSpecificationItem ) == Constants.RESULT_OK )
									{
									if( CommonVariables.writeSentenceStringBuffer != null &&
									CommonVariables.writeSentenceStringBuffer.length() > 0 )
										{
										if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( foundSpecificationItem.isSelfGeneratedAssumption() ? ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION ) : ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION ) ) ) == Constants.RESULT_OK )
											{
											if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) == Constants.RESULT_OK )
												{
												if( !hasRelationContext )
													confirmedSpecificationButNoRelationReplacedSpecificationItem_ = foundSpecificationItem;
												}
											else
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a confirmed specification" );
											}
										else
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a confirmation interface text" );
										}
									else
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the confirmed self-generated specification" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the confirmed self-generated specification" );
								}
							}
						else
							{
							if( hasRelationContext &&
							!foundSpecificationItem.isOlderItem() &&
							foundSpecificationItem.relationContextNr() == relationContextNr )
								foundSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, true, true, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, relationContextNr, specificationWordItem );

							if( foundSpecificationItem != null &&

							// Confirmation: Replace a self-generated by a user-entered specification
							( ( foundSpecificationItem.isSelfGenerated() &&
							!foundSpecificationItem.isArchivedAssignment() ) ||

							( !hasRelationContext &&
							foundSpecificationItem.hasRelationContext() &&
							myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, true, true, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, relationContextNr, specificationWordItem ) != null ) ) )
								{
								if( myWordItem_.writeSelectedSpecification( true, foundSpecificationItem ) == Constants.RESULT_OK )
									{
									if( CommonVariables.writeSentenceStringBuffer != null &&
									CommonVariables.writeSentenceStringBuffer.length() > 0 )
										{
										if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( foundSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : Constants.INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ) ) == Constants.RESULT_OK )
											{
											if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) == Constants.RESULT_OK )
												{
												confirmedReplacedSpecificationItem_ = foundSpecificationItem;

												if( foundSpecificationItem.isAssignment() )
													isConfirmedAssignment_ = true;

												if( foundSpecificationItem.isExclusiveSpecification() )
													isConfirmedExclusive_ = true;

												if( foundSpecificationItem.isSelfGenerated() )
													hasConfirmedSpecification_ = true;
												}
											else
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );
											}
										else
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification" );
										}
									else
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the found specification" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the found specification" );
								}
							}
						}
					}
				else
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "Value specification conflict! Specification word \"" + specificationWordItem.anyWordTypeString() + "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given found specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkUserSpecificationOrQuestion( boolean hasRelationContext, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isValueSpecification, short assumptionLevel, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		GeneralizationResultType generalizationResult = new GeneralizationResultType();
		SpecificationResultType specificationResult;
		boolean hasFeminineProperNameEnding;
		boolean hasMasculineProperNameEnding;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;

		hasCorrectedAssumptionByOppositeQuestion_ = false;
		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;

		if( specificationWordItem != null )
			{
			// Skip if definition (singular noun) is collected with itself
			if( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
			specificationWordItem != myWordItem_ )
				{
				// Check specification in opposite direction
				if( ( generalizationResult = myWordItem_.findGeneralization( false, specificationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find out if word \"" + specificationWordItem.anyWordTypeString() + "\" is one of my generalization words" );
				}

			// If user specification is looping
			if( generalizationResult.hasFoundGeneralization )
				{
				if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification looping conflict" );
				}
			else
				{
				// Check current assignment, specification or question (with relation)
				if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( true, true, true, true, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isQuestion ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) != null )
					{
					if( !isQuestion &&
					isArchivedAssignment != foundSpecificationItem.isArchivedAssignment() )
						{
						if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
						}
					}

				if( isQuestion &&
				foundSpecificationItem == null )
					foundSpecificationItem = myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

				if( foundSpecificationItem == null )
					{
					// Check current assignment or specification (without this relation)
					if( isQuestion )
						foundSpecificationItem = myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

					if( foundSpecificationItem == null )
						foundSpecificationItem = myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, false, false, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem );

					if( !isQuestion &&
					generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME )
						{
						if( !isExclusiveSpecification &&	// Exclusive specifications are not conflicting
						!isPossessive &&					// Possessive specifications are not conflicting
						foundSpecificationItem == null )
							{
							hasFeminineProperNameEnding = myWordItem_.hasFeminineProperNameEnding();
							hasMasculineProperNameEnding = myWordItem_.hasMasculineProperNameEnding();

							if( !myWordItem_.isFeminineOrMasculineWord() &&
							hasFeminineProperNameEnding != specificationWordItem.isFeminineWord() &&
							hasMasculineProperNameEnding != specificationWordItem.isMasculineWord() )
								{
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NAME_TO_BE, myWordItem_.anyWordTypeString(), ( hasFeminineProperNameEnding ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about unexpected feminine or masculine proper name" );
								}

							if( specificationCollectionNr > Constants.NO_COLLECTION_NR )
								{
								if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) ).result == Constants.RESULT_OK )
									{
									relatedSpecificationItem = specificationResult.relatedSpecificationItem;

									if( hasRelationContext &&
									relatedSpecificationItem != null &&
									!relatedSpecificationItem.hasRelationContext() &&
									relatedSpecificationItem.isSelfGeneratedAssumption() )
										{
										// Assumption needs to be corrected
										if( correctedAssumptionReplacedSpecificationItem_ == null )
											{
											if( myWordItem_.writeUpdatedSpecification( false, true, false, relatedSpecificationItem ) == Constants.RESULT_OK )
												{
												hasCorrectedAssumptionByKnowledge_ = true;
												correctedAssumptionReplacedSpecificationItem_ = relatedSpecificationItem;
												}
											else
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );
											}
										else
											return myWordItem_.startErrorInWord( 1, moduleNameString_, "The corrected assumption specification item already assigned" );
										}
									else
										{
										if( !isPossessive &&

										( relatedSpecificationItem == null ||
										!relatedSpecificationItem.hasRelationContext() ||
										relatedSpecificationItem.specificationWordItem() != specificationWordItem ) )
											{
											if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
											}
										}
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );
								}
							}

						if( !CommonVariables.hasShownWarning &&
						!hasRelationContext &&
						isAssignment &&
						!isInactiveAssignment &&
						!isArchivedAssignment &&
						!isNegative &&
						!isValueSpecification &&
						specificationContextNr == Constants.NO_CONTEXT_NR &&
						!myWordItem_.isLanguageWord() &&
						myWordItem_.isSingularOrPluralNoun( specificationWordTypeNr ) &&
						// Negative specification doesn't exist
						myWordItem_.firstNonQuestionAssignmentOrSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) == null )
							{
							if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MISSING_RELATION ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning about ambiguity" );
							}
						}
					}
				else
					{
					if( !hasRelationContext &&
					isAssignment &&
					!isNegative &&
					generalizationContextNr == Constants.NO_CONTEXT_NR &&
					specificationContextNr == Constants.NO_CONTEXT_NR &&
					foundSpecificationItem.hasRelationContext() )
						{
						if( myWordItem_.writeSelectedSpecification( true, foundSpecificationItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.writeSentenceStringBuffer != null &&
							CommonVariables.writeSentenceStringBuffer.length() > 0 )
								{
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN ) == Constants.RESULT_OK )
									{
									if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) == Constants.RESULT_OK )
										CommonVariables.hasShownMessage = false;
									else
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification with an assumption about the relation" );
								}
							else
								return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write a sentence with an assumption about the relation" );
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );
						}
					}

				if( !CommonVariables.hasShownWarning )
					{
					if( isQuestion )
						{
						if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem, specificationString ) == Constants.RESULT_OK )
							{
							if( !isStopDetectingQuestion_ )
								{
								// Detect self-generated question
								if( findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) == Constants.RESULT_OK )
									{
									// Detect user question
									if( findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) == Constants.RESULT_OK )
										isOnlyCheckingForConflicts_ = true;
									else
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting user question" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting self-generated question" );
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the user question" );
						}
					else
						{
						if( foundSpecificationItem != null )
							{
							if( checkUserSpecification( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isValueSpecification, specificationCollectionNr, relationContextNr, foundSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the user specification" );
							}
						}
					}
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte findSameSimilarOrRelatedQuestion( boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelfGenerated, boolean isSpecificationGeneralization, short assumptionLevel, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr )
		{
		boolean hasFoundQuestion;
		boolean isUserSpecificationWord;
		boolean hasFoundOlderSpecification = false;
		boolean hasFoundSpecificationRelationContext = false;
		boolean hasOneSpecificationWord = ( CommonVariables.nUserSpecificationWords == 1 );
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isConflictingQuestion = false;
		boolean isRelatedQuestion = false;
		boolean isSameAssignment = false;
		boolean isSameExclusiveSpecification = false;
		boolean isSameRelationContext = false;
		boolean isSameQuestion = true;
		boolean isSimilarQuestion = false;
		SpecificationItem tempSpecificationItem;
		SpecificationItem foundQuestionSpecificationItem = null;
		WordItem currentWordItem;

		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;

		if( !hasOneSpecificationWord &&
		!isOnlyCheckingForConflicts_ &&
		!isSelfGenerated &&
		( tempSpecificationItem = myWordItem_.sameUserQuestionSpecificationItem( questionParameter ) ) != null )
			{
			if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) == Constants.RESULT_OK )
				isSameQuestionFromUser_ = true;
			else
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification" );
			}

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			do	{
				hasFoundQuestion = false;
				isUserSpecificationWord = currentWordItem.isUserSpecificationWord;

				if( ( tempSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, currentWordItem ) ) != null )
					{
					hasFoundQuestion = true;

					if( foundQuestionSpecificationItem == null ||
					foundQuestionSpecificationItem.isExclusiveSpecification() != tempSpecificationItem.isExclusiveSpecification() )
						{
						foundQuestionSpecificationItem = tempSpecificationItem;

						hasFoundOlderSpecification = foundQuestionSpecificationItem.isOlderItem();
						hasFoundSpecificationRelationContext = foundQuestionSpecificationItem.hasRelationContext();
						isSameAssignment = ( foundQuestionSpecificationItem.isAssignment() == isAssignment );
						isSameRelationContext = ( foundQuestionSpecificationItem.relationContextNr() == relationContextNr );
						isSameExclusiveSpecification = ( foundQuestionSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification );
						}
					}

				if( hasFoundQuestion ||
				isUserSpecificationWord )
					{
					if( hasFoundQuestion &&
					isUserSpecificationWord )
						{
						isSimilarQuestion = true;

						if( foundQuestionSpecificationItem != null )
							{
							if( !hasFoundSpecificationRelationContext ||
							isSameRelationContext )
								{
								if( !isSelfGenerated &&

								( ( isSameRelationContext &&
								!isSameAssignment ) ||

								// Found user question with different exclusive specification
								foundQuestionSpecificationItem.isExclusiveSpecification() != isExclusiveSpecification ||
								// Found user question with different assumption level
								foundQuestionSpecificationItem.assumptionLevel() != assumptionLevel ) )
									isSameQuestion = false;
								else
									{
									if( !hasFoundSpecificationRelationContext &&
									!isExclusiveSpecification &&

									( !hasFoundOlderSpecification ||
									hasRelationContext ) )
										isConflictingQuestion = true;
									}
								}
							else
								{
								if( myWordItem_.isContextSubsetInAllWords( relationContextNr, foundQuestionSpecificationItem.relationContextNr() ) )
									isConflictingQuestion = true;
								else
									{
									isRelatedQuestion = true;
									isSameQuestion = false;
									}
								}
							}
						}
					else
						{
						if( isUserSpecificationWord ||

						// Found question has specification words that aren't in the user question
						( hasSpecificationCollection &&
						isExclusiveSpecification &&
						currentWordItem.isCompoundCollection( specificationCollectionNr ) ) )
							isSameQuestion = false;

						if( isSpecificationGeneralization ||

						// Prefer similar over related if both occur
						( ( !hasFoundSpecificationRelationContext ||
						!isSameRelationContext ) &&

						foundQuestionSpecificationItem != null ) )
							{
							if( !isSameAssignment ||
							hasSpecificationCollection )
								isSimilarQuestion = true;
							else
								isConflictingQuestion = true;
							}
						}
					}
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );

			if( foundQuestionSpecificationItem != null )
				{
				if( !isConflictingQuestion &&
				!isOnlyCheckingForConflicts_ &&
				!isSameQuestionFromUser_ &&

				( isExclusiveSpecification ||
				isSameExclusiveSpecification ) )
					{
					if( isExclusiveSpecification &&
					!foundQuestionSpecificationItem.isExclusiveSpecification() )
						isStopDetectingQuestion_ = true;

					if( isSameQuestion )
						{
						if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? Constants.INTERFACE_QUESTION_I_HAD_THE_SAME_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) ) == Constants.RESULT_OK )
							{
							if( foundQuestionSpecificationItem.isUserQuestion() )
								isSameQuestionFromUser_ = true;
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification" );
						}
					else
						{
						if( myWordItem_.writeSelectedSpecification( false, foundQuestionSpecificationItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.writeSentenceStringBuffer != null &&
							CommonVariables.writeSentenceStringBuffer.length() > 0 )
								{
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE ) : ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE ) ) ) == Constants.RESULT_OK )
									{
									if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) == Constants.RESULT_OK )
										isSimilarOrRelatedQuestion_ = true;
									else
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification" );
								}
							else
								return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the found specification" );
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the found specification" );
						}
					}
				else
					{
					if( hasFoundOlderSpecification )
						{
						if( !isOnlyCheckingForConflicts_ &&

						( !isSameExclusiveSpecification ||
						!isSameRelationContext ) )
							{
							hasShownMoreSpecificRelatedQuestion_ = true;

							// Skip showing announcement if question is in conflict with itself
							if( hasOneSpecificationWord ||
							isExclusiveSpecification )
								{
								if( writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a more specific related question" );
								}
							}
						}
					else
						{
						if( hasShownMoreSpecificRelatedQuestion_ &&
						isConflictingQuestion )
							{
							if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The first word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem createdSpecificationItem )
		{
		if( obsoleteSpecificationItem != null )
			{
			if( obsoleteSpecificationItem.isAssignment() )
				{
				if( replacedAssignmentItem_ == null )
					replacedAssignmentItem_ = obsoleteSpecificationItem;
				else
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "The archive assignment item is already defined" );
				}
			else
				{
				if( myWordItem_.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a specification" );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given obsolete specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationConflict( boolean isExclusiveSpecification, int specificationCollectionNr, SpecificationItem conflictingSpecificationItem )
		{
		boolean isInConflictWithItself = false;
		WordItem generalizationWordItem;

		if( conflictingSpecificationItem != null )
			{
			if( ( generalizationWordItem = conflictingSpecificationItem.generalizationWordItem() ) != null )
				{
				if( generalizationWordItem.writeSelectedSpecification( false, conflictingSpecificationItem ) == Constants.RESULT_OK )
					{
					if( CommonVariables.writeSentenceStringBuffer != null &&
					CommonVariables.writeSentenceStringBuffer.length() > 0 )
						{
						if( !isExclusiveSpecification &&
						specificationCollectionNr > Constants.NO_COLLECTION_NR &&
						!conflictingSpecificationItem.isOlderItem() )
							isInConflictWithItself = true;

						if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, ( isInConflictWithItself ? Constants.INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_ITSELF : ( conflictingSpecificationItem.isOlderItem() ? Constants.INTERFACE_LISTING_CONFLICT : Constants.INTERFACE_LISTING_SENTENCE_DOESNT_LEAVE_ANY_OPTION_OPEN ) ) ) == Constants.RESULT_OK )
							{
							if( !isInConflictWithItself )
								{
								if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) == Constants.RESULT_OK )
									conflictingSpecificationItem.hasAlreadyBeenWrittenAsConflict = true;
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the conflict sentence" );
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the conflicting specification" );
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the conflicting specification" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The generalization word item of the conflicting specification item is undefined" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given conflicting specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeMoreSpecificSpecification( SpecificationItem olderSpecificationItem )
		{
		if( olderSpecificationItem != null )
			{
			if( olderSpecificationItem.isOlderItem() )
				{
				if( myWordItem_.writeSelectedSpecification( !olderSpecificationItem.isExclusiveSpecification(), olderSpecificationItem ) == Constants.RESULT_OK )
					{
					if( CommonVariables.writeSentenceStringBuffer != null &&
					CommonVariables.writeSentenceStringBuffer.length() > 0 )
						{
						if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( olderSpecificationItem.isQuestion() ? ( olderSpecificationItem.isSelfGenerated() ? Constants.INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : Constants.INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem.isSelfGenerated() ? ( olderSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : Constants.INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) == Constants.RESULT_OK )
							{
							if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface listing text" );
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given older specification" );
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the given older specification" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given older specification item isn't old" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given older specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private SpecificationResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean isFirstRelatedSpecification = true;
		boolean isLastRelatedSpecification = false;
		int currentSentenceNr;
		int currentItemNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		int highestItemNr = Constants.NO_ITEM_NR;
		String currentSpecificationString;
		SpecificationItem currentSpecificationItem = ( isCheckingAllQuestions ? myWordItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) : myWordItem_.firstSelectedSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) );

		if( specificationWordItem != null ||
		specificationString != null )
			{
			if( currentSpecificationItem != null &&
			specificationCollectionNr > Constants.NO_COLLECTION_NR )
				{
				do	{
					if( currentSpecificationItem.isRelatedSpecification( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
						{
						currentSentenceNr = currentSpecificationItem.creationSentenceNr();
						currentItemNr = currentSpecificationItem.itemNr();
						currentSpecificationString = currentSpecificationItem.specificationString();

						if( currentSpecificationItem.relationContextNr() == relationContextNr &&
						currentSpecificationItem.specificationWordItem() == specificationWordItem &&

						( specificationString == null ||
						currentSpecificationString == null ||
						currentSpecificationString.equals( specificationString ) ) )
							{
							// Found the given specification item or specification string
							isFirstRelatedSpecification = false;

							if( currentSentenceNr > highestSentenceNr ||

							( currentSentenceNr == highestSentenceNr &&
							currentItemNr > highestItemNr ) )
								{
								isLastRelatedSpecification = true;

								highestSentenceNr = currentSentenceNr;
								highestItemNr = currentItemNr;
								}
							}
						else
							{
							if( currentSentenceNr > highestSentenceNr ||

							( currentSentenceNr == highestSentenceNr &&
							currentItemNr > highestItemNr ) )
								{
								isLastRelatedSpecification = false;

								highestSentenceNr = currentSentenceNr;
								highestItemNr = currentItemNr;
								}

							// If the specification has no relation, select the oldest one (the first of a series)
							if( !hasRelationContext ||
							specificationResult.relatedSpecificationItem == null )
								{
								specificationResult.isFirstRelatedSpecification = isFirstRelatedSpecification;
								specificationResult.relatedSpecificationItem = currentSpecificationItem;
								}
							}
						}
					}
				while( ( currentSpecificationItem = ( isCheckingAllQuestions ? currentSpecificationItem.nextSelectedSpecificationItem() : currentSpecificationItem.nextSelectedSpecificationItem() ) ) != null );

				specificationResult.isLastRelatedSpecification = isLastRelatedSpecification;
				}
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item and given specification string are undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}


	// Constructor / deconstructor

	protected WordSpecification( WordItem myWordItem )
		{
		String errorString = null;

		hasConfirmedSpecification_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasCorrectedAssumptionByOppositeQuestion_ = false;
		hasShownMoreSpecificRelatedQuestion_ = false;
		isConfirmedAssignment_ = false;
		isConfirmedExclusive_ = false;
		isNonExclusiveSpecification_ = false;
		isOnlyCheckingForConflicts_ = false;
		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;
		isStopDetectingQuestion_ = false;

		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		confirmedReplacedSpecificationItem_ = null;
		confirmedSpecificationButNoRelationReplacedSpecificationItem_ = null;
		conflictingSpecificationItem_ = null;
		correctedAssumptionReplacedSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void initializeWordSpecificationVariables()
		{
		hasConfirmedSpecification_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasShownMoreSpecificRelatedQuestion_ = false;
		isOnlyCheckingForConflicts_ = false;
		isStopDetectingQuestion_ = false;

		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		}

	protected boolean hasConfirmedSpecification()
		{
		return hasConfirmedSpecification_;
		}

	protected boolean hasCorrectedAssumption()
		{
		return ( hasCorrectedAssumptionByKnowledge_ ||
				hasCorrectedAssumptionByOppositeQuestion_ );
		}

	protected boolean hasCorrectedAssumptionByKnowledge()
		{
		return hasCorrectedAssumptionByKnowledge_;
		}

	protected boolean hasCorrectedAssumptionByOppositeQuestion()
		{
		return hasCorrectedAssumptionByOppositeQuestion_;
		}

	protected int userSpecificationCollectionNr()
		{
		return userSpecificationCollectionNr_;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		conflictingSpecificationItem_ = null;

		if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) == Constants.RESULT_OK )
			{
			if( conflictingSpecificationItem_ != null &&
			conflictingSpecificationItem_.isOlderItem() )
				{
				// Opposite negative of conflicting specification word
				if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, !isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, conflictingSpecificationItem_.specificationWordItem(), specificationString ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for the opposite negative specification conflict" );
				}
			}
		else
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, boolean isUserSentenceMixOfFeminineAndMasculineSpecificationWords, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isSpecificationCollectedWithItself;
		boolean hasFoundSpecificationWithDifferentRelationContext = false;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean isExclusiveGeneralizationCollection = false;
		boolean isGeneralizationNoun = myWordItem_.isNoun();
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelfGenerated = ( firstJustificationItem != null );
		int nonCompoundCollectionNr;
		int originalSentenceNr;
		int compoundCollectionNr = Constants.NO_COLLECTION_NR;
		int generalizationCollectionNr = Constants.NO_COLLECTION_NR;
		int nFoundRelationContextWords = 0;
		int nRelationContextWords = 0;
		JustificationItem confirmedJustificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundUserSpecificationItem;
		SpecificationItem foundReplacedSpecificationItem = null;
		SpecificationItem foundSpecificationItem = null;

		isConfirmedAssignment_ = false;
		isConfirmedExclusive_ = false;
		isNonExclusiveSpecification_ = false;
		isSameQuestionFromUser_ = false;

		myWordItem_.isWordTouchedDuringCurrentSentence = true;

		confirmedReplacedSpecificationItem_ = null;
		confirmedSpecificationButNoRelationReplacedSpecificationItem_ = null;
		correctedAssumptionReplacedSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		// Check generalization word type
		if( myWordItem_.hasWordType( generalizationWordTypeNr ) ||
		// Skip if plural noun is unknown
		generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL )
			{
			// Check relation variables
			if( ( !hasRelationContext &&
			relationWordTypeNr == Constants.WORD_TYPE_UNDEFINED &&
			relationWordItem == null ) ||

			( ( ( hasRelationContext &&
			relationWordTypeNr > Constants.WORD_TYPE_UNDEFINED ) ||

			// Skip numeral
			relationWordTypeNr == Constants.WORD_TYPE_NUMERAL ) &&

			relationWordItem != null &&
			relationWordItem.hasWordType( relationWordTypeNr ) ) )
				{
				// Specification string
				if( specificationWordItem == null )
					specificationCollectionNr = myWordItem_.collectionNr( specificationWordTypeNr, null );
				else
					{
					if( specificationWordItem.hasWordType( specificationWordTypeNr ) ||
					// Skip if plural noun is unknown
					specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL )
						{
						isSpecificationCollectedWithItself = specificationWordItem.isCollectedWithItself();

						if( !isSpecificationCollectedWithItself )
							generalizationCollectionNr = myWordItem_.collectionNr( generalizationWordTypeNr, specificationWordItem );

						// Collect definition specification without specification collection
						if( !isExclusiveSpecification &&
						isGeneralizationNoun &&
						!isQuestion &&
						specificationWordItem.isNoun() &&
						!specificationWordItem.hasCollection() )
							{
							if( myWordItem_.addCollectionByGeneralization( isExclusiveSpecification, isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ).result != Constants.RESULT_OK )
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to add a collection by generalization of specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						specificationCollectionNr == Constants.NO_COLLECTION_NR )
							{
							compoundCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr );
							nonCompoundCollectionNr = specificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );

							if( ( !hasRelationContext &&
							isPossessive ) ||

							( nonCompoundCollectionNr > Constants.NO_COLLECTION_NR &&

							( ( compoundCollectionNr == Constants.NO_COLLECTION_NR &&
							// Not a definition sentence with the same generalization and specification word
							specificationWordItem != myWordItem_ ) ||

							// Non-exclusive specification collection
							( !isExclusiveSpecification &&
							!isNegative &&
							!isQuestion &&

							( !isSpecificationCollectedWithItself ||

							// If specification is collected with itself
							myWordItem_.isFeminineWord() ||

							( hasRelationContext &&
							!isAssignment &&

							( isUserSentenceMixOfFeminineAndMasculineSpecificationWords ||
							!specificationWordItem.isUserSpecificationWord ) ) ) ) ) ) )
								specificationCollectionNr = nonCompoundCollectionNr;
							else
								specificationCollectionNr = compoundCollectionNr;
							}

						if( !isSelfGenerated &&
						myWordItem_.isReasoningWordType( generalizationWordTypeNr ) )
							{
							if( checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) == Constants.RESULT_OK )
								{
								if( !isAssignment &&
								isConfirmedAssignment_ )
									isAssignment = true;

								if( !isExclusiveSpecification &&
								generalizationCollectionNr > Constants.NO_COLLECTION_NR &&
								myWordItem_.isExclusiveCollection( generalizationCollectionNr ) )
									isExclusiveGeneralizationCollection = true;

								userSpecificationCollectionNr_ = specificationCollectionNr;
								}
							else
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check a user specification or question with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}

				if( CommonVariables.result == Constants.RESULT_OK &&
				!CommonVariables.hasShownWarning )
					{
					if( specificationWordItem != null )
						{
						foundUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem );

						if( isSelfGenerated )
							{
							if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) != null )
								{
								if( hasRelationContext &&
								foundUserSpecificationItem == null &&
								foundSpecificationItem.hasRelationContext() &&
								foundSpecificationItem.relationContextNr() != relationContextNr )
									{
									hasFoundSpecificationWithDifferentRelationContext = true;
									nRelationContextWords = myWordItem_.nContextWordsInAllWords( relationContextNr, specificationWordItem );
									nFoundRelationContextWords = myWordItem_.nContextWordsInAllWords( foundSpecificationItem.relationContextNr(), specificationWordItem );
									}
								}
							}
						else
							foundSpecificationItem = foundUserSpecificationItem;
						}

					if( foundSpecificationItem == null ||

					// Exceptions
					isNonExclusiveSpecification_ ||
					confirmedReplacedSpecificationItem_ != null ||
					correctedAssumptionReplacedSpecificationItem_ != null ||

					// Overwrite non-conditional specification by conditional one
					( isConditional &&
					!foundSpecificationItem.isConditional() ) ||

					// Overwrite non-exclusive specification by exclusive one
					( isExclusiveSpecification &&
					!foundSpecificationItem.isExclusiveSpecification() ) ||

					// Overwrite user questions by non-exclusive one
					( !isAssignment &&
					isQuestion &&
					!isSameQuestionFromUser_ &&
					!isSelfGenerated &&
					foundSpecificationItem.isExclusiveSpecification() ) ||

					// Overwrite self-generated assignment by user assignment
					( isAssignment &&
					!isSelfGenerated &&
					foundSpecificationItem.isSelfGenerated() ) ||

					// Add a self-generated specification without relation if this specification only exists with relation
					( isSelfGenerated &&
					!hasRelationContext &&
					foundSpecificationItem.hasRelationContext() &&
					foundSpecificationItem.isSelfGeneratedAssumption() ) ||

					// Accept different relation context (e.g. ambiguous specification)
					( hasFoundSpecificationWithDifferentRelationContext &&

					( ( hasConfirmedSpecification_ &&
					nRelationContextWords == CommonVariables.nUserRelationWords ) ||

					( myWordItem_.isUserRelationWord &&
					nFoundRelationContextWords < nRelationContextWords ) ||

					( firstJustificationItem != null &&
					firstJustificationItem.isPossessiveReversibleConclusion() ) ) ) )
						{
						if( foundSpecificationItem != null )
							{
							if( !isConditional &&
							foundSpecificationItem.isConditional() )
								isConditional = true;

							if( !isExclusiveSpecification &&
							!isQuestion &&
							foundSpecificationItem.isExclusiveSpecification() )
								isExclusiveSpecification = true;

							if( !isConfirmedAssignment_ &&
							!isQuestion &&
							!isSelfGenerated &&
							foundSpecificationItem.isUserSpecification() &&
							foundSpecificationItem != confirmedReplacedSpecificationItem_ &&
							foundSpecificationItem != correctedAssumptionReplacedSpecificationItem_ )
								foundReplacedSpecificationItem = foundSpecificationItem;
							}

						if( isExclusiveSpecification )
							{
							if( isNonExclusiveSpecification_ )
								isExclusiveSpecification = false;
							}
						else
							{
							if( isConfirmedExclusive_ ||
							isExclusiveGeneralizationCollection ||

							( isConditional &&

							( generalizationCollectionNr > Constants.NO_COLLECTION_NR ||
							specificationCollectionNr > Constants.NO_COLLECTION_NR ) ) )
								isExclusiveSpecification = true;
							}

						originalSentenceNr = ( hasRelationContext ||
											isQuestion ||
											foundSpecificationItem == null ||

											( foundSpecificationItem.hasRelationContext() &&
											firstJustificationItem != null &&
											firstJustificationItem.isExclusiveSpecificationSubstitutionAssumption() ) ? CommonVariables.currentSentenceNr : foundSpecificationItem.originalSentenceNr() );

						// Create the actual specification
						if( ( specificationResult = myWordItem_.createSpecification( false, false, false, false, false, isConditional, ( hasCorrectedAssumptionByKnowledge_ || hasCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveSpecification, false, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, Constants.NO_ASSIGNMENT_LEVEL, assumptionLevel, CommonVariables.currentGrammarLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? Constants.WORD_TYPE_UNDEFINED : relationWordTypeNr ), generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, null ) ).result == Constants.RESULT_OK )
							{
							if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
								{
								if( !isPossessive &&
								!isQuestion &&
								!isSelection &&
								hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords &&
								specificationWordItem != null &&
								!myWordItem_.isFeminineOrMasculineWord() )
									{
									if( specificationWordItem.isFeminineWord() )
										{
										if( myWordItem_.markWordAsFeminine() != Constants.RESULT_OK )
											myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark my word as feminine" );
										}
									else
										{
										if( specificationWordItem.isMasculineWord() )
											{
											if( myWordItem_.markWordAsMasculine() != Constants.RESULT_OK )
												myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark my word as masculine" );
											}
										}
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								confirmedReplacedSpecificationItem_ != null )
									{
									if( replaceOrDeleteSpecification( confirmedReplacedSpecificationItem_, createdSpecificationItem ) == Constants.RESULT_OK )
										{
										confirmedJustificationItem = confirmedReplacedSpecificationItem_.firstJustificationItem();

										if( confirmedJustificationItem != null &&
										confirmedJustificationItem.isActiveItem() &&
										!myWordItem_.isJustificationInUse( confirmedJustificationItem ) )
											{
											if( myWordItem_.replaceOrDeleteJustification( confirmedJustificationItem ) != Constants.RESULT_OK )
												myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a confirmed justification item" );
											}
										}
									else
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a confirmed specification" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								confirmedSpecificationButNoRelationReplacedSpecificationItem_ != null )
									{
									if( myWordItem_.confirmSpecificationButNoRelation( confirmedSpecificationButNoRelationReplacedSpecificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to confirm a specification, but no relation(s)" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								correctedAssumptionReplacedSpecificationItem_ != null )
									{
									if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) == Constants.RESULT_OK )
										correctedAssumptionReplacedSpecificationItem_ = null;
									else
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a corrected specification" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								foundReplacedSpecificationItem != null )
									{
									if( replaceOrDeleteSpecification( foundReplacedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a found specification" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								!isNegative &&
								!isPossessive &&
								!isQuestion &&
								!isSimilarOrRelatedQuestion_ &&
								specificationWordItem != null )
									{
									if( myWordItem_.findAnswersToQuestions( specificationWordItem.compoundCollectionNr( specificationWordTypeNr ), createdSpecificationItem ) != Constants.RESULT_OK )
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find answers to questions" );
									}
								}
							else
								myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't create a specification item" );
							}
						else
							myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a specification item" );
						}

					if( CommonVariables.result == Constants.RESULT_OK &&
					specificationWordItem != null &&

					( ( isSelfGenerated &&
					relationWordItem != null ) ||

					foundSpecificationItem == null ) )
						{
						if( addGeneralization( ( ( !isSelfGenerated && isAssignment ) || ( isSelfGenerated && isPossessive ) || relationWordItem == null ), generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to add a generalization" );
						}
					}
				}
			else
				myWordItem_.startErrorInWord( 1, moduleNameString_, "The relation variables aren't matching" );
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		specificationResult.foundSpecificationItem = foundSpecificationItem;
		specificationResult.replacedAssignmentItem = replacedAssignmentItem_;
		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isCheckingRelationContext, SpecificationItem searchSpecificationItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( searchSpecificationItem != null )
			return findRelatedSpecification( false, isCheckingRelationContext, false, false, searchSpecificationItem.isAssignment(), searchSpecificationItem.isInactiveAssignment(), searchSpecificationItem.isArchivedAssignment(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPossessive(), searchSpecificationItem.questionParameter(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.specificationWordItem(), searchSpecificationItem.specificationString() );

		specificationResult.result = myWordItem_.startErrorInWord( 1, moduleNameString_, "The given search specification item is undefined" );
		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( isIncludingAssignments )
			{
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) ).result == Constants.RESULT_OK )
				{
				if( isIncludingArchivedAssignments &&
				specificationResult.relatedSpecificationItem == null )
					{
					// Archived assignments
					if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
						myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification as well" );
					}
				}
			else
				myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related assignment" );
			}

		// Specifications
		if( CommonVariables.result == Constants.RESULT_OK &&
		specificationResult.relatedSpecificationItem == null )
			{
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
				myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );
			}

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
