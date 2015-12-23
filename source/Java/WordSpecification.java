/*
 *	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
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

class WordSpecification
	{
	// Private constructible variables

	private boolean hasConfirmedSpecification_;
	private boolean hasCorrectedAssumptionByKnowledge_;
	private boolean hasCorrectedAssumptionByOppositeQuestion_;
	private boolean hasShownMoreSpecificRelatedQuestion_;
	private boolean isConfirmedExclusive_;
	private boolean isNonExclusiveSpecification_;
	private boolean isOnlyCheckingForConflicts_;
	private boolean isSameQuestionFromUser_;
	private boolean isSimilarOrRelatedQuestion_;
	private boolean isStopDetectingQuestion_;

	private int compoundSpecificationCollectionNr_;
	private int userSpecificationCollectionNr_;

	private SpecificationItem confirmedReplacedSpecificationItem_;
	private SpecificationItem confirmedSpecificationButNoRelationReplacedSpecificationItem_;
	private SpecificationItem conflictingSpecificationItem_;
	private SpecificationItem correctedAssumptionReplacedSpecificationItem_;
	private SpecificationItem replacedAssignmentItem_;

	private WordItem compoundSpecificationWordItem_;


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
						if( specificationWordItem.createGeneralizationItem( false, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != Constants.RESULT_OK )
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
						// Relation word of a generalization word: proper name
						if( relationWordItem.createGeneralizationItem( true, relationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != Constants.RESULT_OK )
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

	private byte checkSpecificationWordForConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
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
			if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result == Constants.RESULT_OK )
				{
				if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) == null )
					{
					isSpecificationCollectedWithItself = specificationWordItem.isNounWordCollectedWithItself();

					// Check for looping
					if( isSpecificationCollectedWithItself ||
					( conflictingSpecificationItem_ = specificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, myWordItem_ ) ) == null )
						{
						// Check for past tense
						foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem );

						if( foundSpecificationItem != null &&
						foundSpecificationItem.isArchivedAssignment() != isArchivedAssignment )
							conflictingSpecificationItem_ = foundSpecificationItem;
						else
							{
							if( !isSpecificationCollectedWithItself &&
							( compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
								{
								// Check for negative
								foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

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
									if( ( conflictingSpecificationItem_ = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, null ) ) != null )
										{
										// Write conflict
										if( writeSpecificationConflict( isExclusiveSpecification, specificationCollectionNr, conflictingSpecificationItem_ ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write another conflicting specification" );
										}

									conflictingSpecificationItem_ = foundSpecificationItem;
									}
								else
									{
									// Check for existing compound specification
									// No self-generated question available for this specification
									if( ( nonCompoundSpecificationCollectionNr = myWordItem_.nonCompoundCollectionNrInAllWords( compoundSpecificationCollectionNr ) ) > Constants.NO_COLLECTION_NR )
										{
										tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, nonCompoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, null );

										if( tempConflictingSpecificationItem == null )
											{
											tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, null );

											if( tempConflictingSpecificationItem != null &&
											!tempConflictingSpecificationItem.isOlderItem() &&
											tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem )
												conflictingSpecificationItem_ = tempConflictingSpecificationItem;
											}
										else
											{
											if( tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem &&
											!tempConflictingSpecificationItem.isHiddenSpecification() )
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
					// Find for possible past tense assignment of relatedSpecificationItem
					pastTenseAssignmentItem = myWordItem_.firstNonQuestionAssignmentItem( false, false, true, isNegative, isPossessive, relationContextNr, relatedSpecificationItem.specificationWordItem() );
					conflictingSpecificationItem_ = ( pastTenseAssignmentItem == null ? relatedSpecificationItem : pastTenseAssignmentItem );
					}

				if( !hasFoundCompoundConflict &&
				conflictingSpecificationItem_ != null &&
				!conflictingSpecificationItem_.hasAlreadyBeenWrittenAsConflict &&

				( specificationCollectionNr == Constants.NO_COLLECTION_NR ||
				!specificationWordItem.hasNonExclusiveCollection( specificationCollectionNr ) ) )
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

	private byte checkUserQuestion( boolean hasRelationContext, boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem foundQuestionSpecificationItem, WordItem specificationWordItem )
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
				if( ( specificationResult = findRelatedSpecification( false, false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result == Constants.RESULT_OK )
					{
					if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null &&
					!isExclusiveSpecification &&
					!isSpecificationGeneralization &&
					!specificationWordItem.hasNonExclusiveCollection( specificationCollectionNr ) )
						{
						if( relatedSpecificationItem.isOlderItem() )
							{
							if( hasRelationContext )
								{
								if( correctedAssumptionReplacedSpecificationItem_ == null )
									{
									if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, true, true, relatedSpecificationItem.isNegative(), relatedSpecificationItem.isPossessive(), true, Constants.NO_QUESTION_PARAMETER, relatedSpecificationItem.specificationCollectionNr(), relatedSpecificationItem.relationContextNr(), relatedSpecificationItem.specificationWordItem() ) ) != null )
										{
										if( myWordItem_.writeUpdatedSpecification( false, false, false, true, correctedAssumptionReplacedSpecificationItem_ ) == Constants.RESULT_OK )
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

	private byte checkUserSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isValueSpecification, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundRelationContext;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		WordItem currentLanguageWordItem;
		WordItem currentWordItem;
		WordTypeItem pluralNounWordTypeItem;
		String unknownPluralNounString;

		if( foundSpecificationItem != null )
			{
			if( specificationWordItem != null )
				{
				if( foundSpecificationItem.isValueSpecification() == isValueSpecification )
					{
					if( !hasRelationContext &&
					!isAssignment &&
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
						foundSpecificationItem.isOlderItem() )
							{
							if( foundSpecificationItem.isArchivedAssignment() == isArchivedAssignment &&
							// Check if negative specification exists
							myWordItem_.firstAssignmentOrSpecificationItem( true, true, true, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) == null )
								{
								if( foundSpecificationItem.hasRelationContext() )
									{
									if( isPossessive &&
									relationWordItem != null &&
									specificationWordItem.isFemale() != relationWordItem.isFemale() &&
									specificationWordItem.isMale() != relationWordItem.isMale() )
										{
										if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
										}
									}
								else
									{
									if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific related specification" );
									}
								}
							else
								{
								if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
								}
							}

						if( !CommonVariables.hasShownWarning )
							{
							if( foundSpecificationItem.hasRelationContext() &&
							foundSpecificationItem.relationContextNr() != relationContextNr &&
							foundSpecificationItem.isInactiveAssignment() == isInactiveAssignment &&
							foundSpecificationItem.isArchivedAssignment() == isArchivedAssignment &&

							( !hasRelationContext ||
							// Current user specification has less relation words than the found specification
							CommonVariables.nUserRelationWords < myWordItem_.nContextWordsInAllWords( foundSpecificationItem.relationContextNr(), specificationWordItem ) ) )
								{
								// Has new relation context
								if( hasRelationContext ||
								myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, relationContextNr, specificationWordItem ) == null )
									{
									if( !hasRelationContext )
										confirmedSpecificationButNoRelationReplacedSpecificationItem_ = foundSpecificationItem;

									if( !foundSpecificationItem.isHiddenSpecification() )
										{
										if( myWordItem_.writeSelectedSpecification( true, foundSpecificationItem ) == Constants.RESULT_OK )
											{
											if( CommonVariables.writeSentenceStringBuffer != null &&
											CommonVariables.writeSentenceStringBuffer.length() > 0 )
												{
												if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( foundSpecificationItem.isSelfGeneratedAssumption() ? ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION ) : ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION ) ) ) == Constants.RESULT_OK )
													{
													if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
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
								}
							else
								{
								if( hasRelationContext )
									{
									hasFoundRelationContext = foundSpecificationItem.hasRelationContext();

									if( !hasFoundRelationContext ||

									( !foundSpecificationItem.isOlderItem() &&
									foundSpecificationItem.relationContextNr() == relationContextNr ) )
										// Confirmed relation words
										foundSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( hasFoundRelationContext, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, foundSpecificationItem.relationContextNr(), specificationWordItem );
									}

								if( foundSpecificationItem != null )
									{
									// Confirmation: Replace a self-generated by a user-entered specification
									if( foundSpecificationItem.isSelfGenerated() &&
									!myWordItem_.isNounWordCollectedWithItself() )
										{
										if( foundSpecificationItem.isExclusiveSpecification() )
											isConfirmedExclusive_ = true;

										if( foundSpecificationItem.isSelfGenerated() )
											hasConfirmedSpecification_ = true;

										confirmedReplacedSpecificationItem_ = foundSpecificationItem;

										if( !foundSpecificationItem.isHiddenSpecification() )
											{
											if( writeConfirmedSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the confirmed specification" );
											}
										}

									if( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
									foundSpecificationItem.specificationWordTypeNr() == Constants.WORD_TYPE_NOUN_SINGULAR &&
									( pluralNounWordTypeItem = specificationWordItem.activeWordTypeItem( Constants.WORD_TYPE_NOUN_PLURAL ) ) != null )
										{
										// Skip announcement for new occurrences
										if( !pluralNounWordTypeItem.isOlderItem() )
											{
											// Found unknown plural specification word
											if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START, pluralNounWordTypeItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END ) == Constants.RESULT_OK )
												{
												confirmedReplacedSpecificationItem_ = foundSpecificationItem;

												if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null &&
												( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null &&
												( unknownPluralNounString = currentLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) ) != null )
													{
													// Do for all words
													do	{
														if( currentWordItem.clearLastWrittenSentenceStringWithUnknownPluralNounInAllWords( unknownPluralNounString, specificationWordItem ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to clear the last written sentence string with specification word \"" + specificationWordItem.anyWordTypeString() + "\" in generalization word \"" + currentWordItem.anyWordTypeString() + "\"" );
														}
													while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
													}
												}
											else
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a notification text" );
											}
										}
									}
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

	private byte checkUserSpecificationOrQuestion( boolean hasRelationContext, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isValueSpecification, short assumptionLevel, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		GeneralizationResultType generalizationResult = new GeneralizationResultType();
		SpecificationResultType specificationResult;
		boolean hasFeminineSingularNounEnding;
		boolean isSingularNounGeneralizationWord = ( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR );
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordTypeItem foundWordTypeItem;

		hasCorrectedAssumptionByOppositeQuestion_ = false;
		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;

		if( specificationWordItem != null )
			{
			// Skip if definition (singular noun) is collected with itself
			if( isSingularNounGeneralizationWord &&
			specificationWordItem != myWordItem_ )
				{
				// Check specification in opposite direction
				if( ( generalizationResult = myWordItem_.findGeneralization( false, specificationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find out if word \"" + specificationWordItem.anyWordTypeString() + "\" is one of my generalization words" );
				}

			// If user specification is looping
			if( generalizationResult.hasFoundGeneralization )
				{
				if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
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
						if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
						}
					}

				if( isQuestion &&
				foundSpecificationItem == null )
					foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

				if( foundSpecificationItem == null )
					{
					// Check current assignment or specification (without this relation)
					if( isQuestion )
						foundSpecificationItem = myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

					if( foundSpecificationItem == null )
						foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem );

					if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME )
						{
						// Exclusive specifications are not conflicting
						if( !isExclusiveSpecification &&
						// Possessive specifications are not conflicting
						!isPossessive &&
						foundSpecificationItem == null )
							{
							if( !myWordItem_.isFemaleOrMale() &&
							myWordItem_.hasFeminineProperNameEnding() != specificationWordItem.isFemale() &&
							myWordItem_.hasMasculineProperNameEnding() != specificationWordItem.isMale() )
								{
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NAME_TO_BE, myWordItem_.anyWordTypeString(), ( myWordItem_.hasFeminineProperNameEnding() ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a proper name expected to be feminine or masculine" );
								}

							if( specificationCollectionNr > Constants.NO_COLLECTION_NR )
								{
								if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result == Constants.RESULT_OK )
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
											if( myWordItem_.writeUpdatedSpecification( false, false, true, false, relatedSpecificationItem ) == Constants.RESULT_OK )
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
										myWordItem_.isUserGeneralizationWord &&

										( relatedSpecificationItem == null ||

										( ( !relatedSpecificationItem.hasRelationContext() ||
										relatedSpecificationItem.specificationWordItem() != specificationWordItem ) &&

										( !specificationWordItem.isNounWordCollectedWithItself() ||
										!relatedSpecificationItem.isSpecificationWordCollectedWithItself() ) ) ) )
											{
											if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
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
						myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) == null )
							{
							if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MISSING_RELATION ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning about ambiguity" );
							}
						}
					else
						{
						if( isSingularNounGeneralizationWord &&
						!myWordItem_.isFemaleOrMale() &&
						( foundWordTypeItem = myWordItem_.activeWordTypeItem( Constants.WORD_TYPE_NOUN_SINGULAR ) ) != null )
							{
							hasFeminineSingularNounEnding = foundWordTypeItem.hasFeminineWordEnding();

							if( ( hasFeminineSingularNounEnding ||
							foundWordTypeItem.hasMasculineWordEnding() ) &&

							!myWordItem_.isCorrectIndefiniteArticle( ( hasFeminineSingularNounEnding ? Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ), generalizationWordTypeNr ) )
								{
								// Remove indefinite article parameter from word type item
								if( foundWordTypeItem.setIndefiniteArticleParameter( Constants.NO_INDEFINITE_ARTICLE_PARAMETER ).result == Constants.RESULT_OK )
									{
									if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_SINGULAR_NOUN_TO_BE, myWordItem_.anyWordTypeString(), ( hasFeminineSingularNounEnding ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) == Constants.RESULT_OK )
										CommonVariables.hasShownArticleNotification = true;
									else
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a singular noun expected to be feminine or masculine" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about an indefinite article expected to be feminine or masculine" );
								}
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
									if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) == Constants.RESULT_OK )
										CommonVariables.hasShownMessage = false;
									else
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a missing relation" );
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
						if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem ) == Constants.RESULT_OK )
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
							if( checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isValueSpecification, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
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
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write 'I had the same question before' interface notification" );
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
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about the same question" );
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
									if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) == Constants.RESULT_OK )
										isSimilarOrRelatedQuestion_ = true;
									else
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a related question" );
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
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "The replaced assignment item is already defined" );
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

	private byte showAssumptionsThatAreNotHiddenAnymore()
		{
		ContextItem currentContextItem;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem hiddenSpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( ( currentContextItem = myWordItem_.firstActiveContextItem() ) != null )
			{
			// Do for all context items in my word
			do	{
				if( ( currentGeneralizationItem = myWordItem_.firstGeneralizationItem() ) != null )
					{
					// Check all involved words
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
							{
							if( ( hiddenSpecificationItem = currentGeneralizationWordItem.firstUnHiddenSpecificationItem( currentContextItem.contextNr() ) ) != null )
								{
								if( currentGeneralizationWordItem.writeUpdatedSpecification( true, false, false, false, hiddenSpecificationItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an assumption that is not hidden anymore" );
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I have found an undefined generalization word" );
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
					}
				}
			while( ( currentContextItem = currentContextItem.nextContextItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte writeConfirmedSpecification( SpecificationItem writeSpecificationItem )
		{
		if( writeSpecificationItem != null )
			{
			if( myWordItem_.writeSelectedSpecification( true, writeSpecificationItem ) == Constants.RESULT_OK )
				{
				if( CommonVariables.writeSentenceStringBuffer != null &&
				CommonVariables.writeSentenceStringBuffer.length() > 0 )
					{
					if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( writeSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : Constants.INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ) ) == Constants.RESULT_OK )
						{
						if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );
						}
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a confirmation" );
					}
				else
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given write specification" );
				}
			else
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

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
								if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) == Constants.RESULT_OK )
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
							if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
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

	private SpecificationResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean isFirstRelatedSpecification = true;
		boolean isLastRelatedSpecification = false;
		int currentSentenceNr;
		int currentItemNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		int highestItemNr = Constants.NO_ITEM_NR;
		SpecificationItem currentSpecificationItem = ( isCheckingAllQuestions ? myWordItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) : myWordItem_.firstSelectedSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) );

		if( specificationWordItem != null )
			{
			if( currentSpecificationItem != null &&
			specificationCollectionNr > Constants.NO_COLLECTION_NR )
				{
				do	{
					if( currentSpecificationItem.isRelatedSpecification( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
						{
						currentSentenceNr = currentSpecificationItem.creationSentenceNr();
						currentItemNr = currentSpecificationItem.itemNr();

						if( currentSpecificationItem.relationContextNr() == relationContextNr &&
						currentSpecificationItem.specificationWordItem() == specificationWordItem )
							{
							// Found the given specification item
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
							specificationResult.relatedSpecificationItem == null ||

							( currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
							currentSpecificationItem.specificationWordItem() != specificationWordItem ) )
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
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

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
		isConfirmedExclusive_ = false;
		isNonExclusiveSpecification_ = false;
		isOnlyCheckingForConflicts_ = false;
		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;
		isStopDetectingQuestion_ = false;

		compoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		confirmedReplacedSpecificationItem_ = null;
		confirmedSpecificationButNoRelationReplacedSpecificationItem_ = null;
		conflictingSpecificationItem_ = null;
		correctedAssumptionReplacedSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		compoundSpecificationWordItem_ = null;

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

		compoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		compoundSpecificationWordItem_ = null;
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

	protected boolean hasShownMoreSpecificRelatedQuestion()
		{
		return hasShownMoreSpecificRelatedQuestion_;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		conflictingSpecificationItem_ = null;

		if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) == Constants.RESULT_OK )
			{
			if( conflictingSpecificationItem_ != null &&
			conflictingSpecificationItem_.isOlderItem() )
				{
				// Opposite negative of conflicting specification word
				if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, !isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, conflictingSpecificationItem_.specificationWordItem() ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for the opposite negative specification conflict" );
				}
			}
		else
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isSpecificationCollectedWithItself;
		boolean hasCopiedRelationContext = ( copiedRelationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasFoundSpecificationWithDifferentRelationContext = false;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelfGenerated = ( firstJustificationItem != null );
		int nonCompoundCollectionNr;
		int originalSentenceNr;
		int compoundCollectionNr = Constants.NO_COLLECTION_NR;
		int generalizationCollectionNr = Constants.NO_COLLECTION_NR;
		SpecificationItem createdSpecificationItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem foundReplacedHiddenSpecificationItem = null;
		SpecificationItem foundReplacedUserSpecificationItem = null;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem foundUserSpecificationItem = null;

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
				if( specificationWordItem != null )
					{
					if( specificationWordItem.hasWordType( specificationWordTypeNr ) ||
					// Skip if plural noun is unknown
					specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||
					// Newly created language
					myWordItem_.isLanguageWord() )
						{
						isSpecificationCollectedWithItself = specificationWordItem.isNounWordCollectedWithItself();

						if( !isSpecificationCollectedWithItself )
							generalizationCollectionNr = myWordItem_.collectionNr( generalizationWordTypeNr, specificationWordItem );

						// Collect definition specification without specification collection
						if( !isExclusiveSpecification &&
						!isQuestion &&
						!specificationWordItem.hasCollection() &&
						myWordItem_.isSingularOrPluralNoun( generalizationWordTypeNr ) &&
						myWordItem_.isSingularOrPluralNoun( specificationWordTypeNr ) )
							{
							if( myWordItem_.addCollectionByGeneralization( isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ) != Constants.RESULT_OK )
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to add a collection by generalization of specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}

						if( CommonVariables.result == Constants.RESULT_OK )
							{
							if( hasSpecificationCollection )
								{
								if( !specificationWordItem.hasCollectionNr( specificationCollectionNr ) )
									myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification collection" );
								}
							else
								{
								if( !isSelfGenerated &&
								userSpecificationCollectionNr_ > Constants.NO_COLLECTION_NR )
									// Use previous user specification collection
									specificationCollectionNr = userSpecificationCollectionNr_;
								else
									{
									compoundCollectionNr = specificationWordItem.lastCompoundCollectionNr( specificationWordTypeNr );
									nonCompoundCollectionNr = specificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );

									if( nonCompoundCollectionNr > Constants.NO_COLLECTION_NR &&

									( ( compoundCollectionNr == Constants.NO_COLLECTION_NR &&
									// Not a definition sentence with the same generalization and specification word
									!specificationWordItem.isUserGeneralizationWord ) ||

									// Non-exclusive specification collection
									( !isExclusiveSpecification &&
									!isQuestion &&

									( !isSpecificationCollectedWithItself ||

									( hasRelationContext &&

									( nContextRelations > 1 ||
									!specificationWordItem.isUserSpecificationWord ) ) ) ) ) )
										specificationCollectionNr = nonCompoundCollectionNr;
									else
										specificationCollectionNr = compoundCollectionNr;
									}
								}
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!isSelfGenerated &&
						myWordItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
							{
							if( checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, relationWordItem ) == Constants.RESULT_OK )
								{
								if( isGeneralizationProperName ||

								( !isAssignment &&
								!isEveryGeneralization ) )
									userSpecificationCollectionNr_ = specificationCollectionNr;
								}
							else
								myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check a user specification or question with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!CommonVariables.hasShownWarning )
							{
							// Try to find specification
							// Start with finding user specification
							if( ( foundUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, true, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == null )
								{
								if( isSelfGenerated )
									{
									if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : ( hasCopiedRelationContext ? copiedRelationContextNr : relationContextNr ) ), specificationWordItem ) ) == null )
										{
										if( isSpecificationCollectedWithItself &&
										hasSpecificationCollection &&
										!isQuestion &&
										( tempSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, relationContextNr, specificationWordItem ) ) != null )
											{
											if( tempSpecificationItem.hasRelationContext() )
												{
												if( tempSpecificationItem.isHiddenSpecification() )
													foundReplacedHiddenSpecificationItem = tempSpecificationItem;
												}
											else
												{
												if( myWordItem_.isUserRelationWord &&
												tempSpecificationItem.isSelfGenerated() &&
												tempSpecificationItem.hasSpecificationNonCompoundCollection() )
													{
													if( confirmedReplacedSpecificationItem_ == null )
														{
														if( myWordItem_.writeUpdatedSpecification( true, true, false, false, tempSpecificationItem ) == Constants.RESULT_OK )
															confirmedReplacedSpecificationItem_ = tempSpecificationItem;
														else
															myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an assumption, which will have an added relation" );
														}
													else
														myWordItem_.startErrorInWord( 1, moduleNameString_, "The confirmed replaced specification item is already defined" );
													}
												}
											}
										}
									else
										{
										if( foundSpecificationItem.relationContextNr() == relationContextNr )
											{
											if( isGeneralizationProperName &&
											firstJustificationItem != null &&
											firstJustificationItem.isConclusionJustification() &&
											foundSpecificationItem.isOlderItem() &&
											foundSpecificationItem.isSelfGeneratedAssumption() &&
											!foundSpecificationItem.isHiddenSpecification() &&
											// Conclusion doesn't exists
											myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, null ) == null )
												{
												if( foundSpecificationItem.markAsConcludedAssumption() == Constants.RESULT_OK )
													{
													if( myWordItem_.writeUpdatedSpecification( true, true, false, false, foundSpecificationItem ) != Constants.RESULT_OK )
														myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a concluded assumption" );
													}
												else
													myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );
												}
											}
										else
											{
											if( hasRelationContext &&
											foundSpecificationItem.hasRelationContext() )
												hasFoundSpecificationWithDifferentRelationContext = true;
											}
										}
									}
								}
							else
								{
								if( foundUserSpecificationItem.isSpecificationGeneralization() == isSpecificationGeneralization )
									{
									if( !isSelfGenerated ||
									foundUserSpecificationItem.hasRelationContext() ||
									!foundUserSpecificationItem.isExclusiveSpecification() ||
									foundUserSpecificationItem.isOlderItem() )
										{
										foundSpecificationItem = foundUserSpecificationItem;

										if( isAssignment &&
										specificationCollectionNr > Constants.NO_COLLECTION_NR &&
										confirmedReplacedSpecificationItem_ != null &&
										foundUserSpecificationItem.isUserAssignment() )
											// Prepare foundUserSpecificationItem for confirmation
											foundUserSpecificationItem = myWordItem_.firstSpecificationItem( isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );
										else
											foundUserSpecificationItem = null;
										}
									else
										{
										if( compoundSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR ||
										compoundSpecificationWordItem_ == null )
											{
											if( ( specificationCollectionNr = myWordItem_.highestCollectionNrInAllWords() ) < Constants.MAX_COLLECTION_NR )
												{
												compoundSpecificationCollectionNr_ = ++specificationCollectionNr;
												compoundSpecificationWordItem_ = specificationWordItem;
												}
											else
												myWordItem_.startSystemErrorInWord( 1, moduleNameString_, "Collection number overflow" );
											}
										else
											{
											specificationCollectionNr = compoundSpecificationCollectionNr_;

											if( compoundSpecificationWordItem_.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result == Constants.RESULT_OK )
												{
												if( specificationWordItem.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, compoundSpecificationWordItem_, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
													myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + specificationWordItem.anyWordTypeString() + "\" with word \"" + compoundSpecificationWordItem_.anyWordTypeString() + "\"" );
												}
											else
												myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + compoundSpecificationWordItem_.anyWordTypeString() + "\" with word \"" + specificationWordItem.anyWordTypeString() + "\"" );
											}
										}
									}
								else
									specificationCollectionNr = foundUserSpecificationItem.generalizationCollectionNr();
								}
							}
						}
					else
						myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification word type" );
					}

				if( CommonVariables.result == Constants.RESULT_OK &&
				!CommonVariables.hasShownWarning )
					{
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

					// Accept different relation context (e.g. ambiguous specification)
					( hasFoundSpecificationWithDifferentRelationContext &&
					firstJustificationItem != null &&

					( hasConfirmedSpecification_ ||
					firstJustificationItem.isPossessiveReversibleConclusion() ||

					( hasCopiedRelationContext &&
					!firstJustificationItem.isOlderItem() ) ) ) )
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

							if( !isQuestion &&
							!isSelfGenerated &&
							foundSpecificationItem.isUserSpecification() &&
							foundSpecificationItem != confirmedReplacedSpecificationItem_ &&
							foundSpecificationItem != correctedAssumptionReplacedSpecificationItem_ )
								foundReplacedUserSpecificationItem = foundSpecificationItem;
							}

						if( isExclusiveSpecification )
							{
							if( isNonExclusiveSpecification_ )
								isExclusiveSpecification = false;
							}
						else
							{
							if( isConfirmedExclusive_ )
								isExclusiveSpecification = true;
							}

						originalSentenceNr = ( hasRelationContext ||
											isQuestion ||
											foundSpecificationItem == null ? CommonVariables.currentSentenceNr : foundSpecificationItem.originalSentenceNr() );

						// Create the actual specification
						if( ( specificationResult = myWordItem_.createSpecificationItem( false, false, false, false, false, isConditional, ( hasCorrectedAssumptionByKnowledge_ || hasCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveSpecification, false, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, Constants.NO_ASSIGNMENT_LEVEL, assumptionLevel, CommonVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? Constants.WORD_TYPE_UNDEFINED : relationWordTypeNr ), generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, null ) ).result == Constants.RESULT_OK )
							{
							if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
								{
								if( confirmedReplacedSpecificationItem_ != null )
									{
									if( replaceOrDeleteSpecification( confirmedReplacedSpecificationItem_, createdSpecificationItem ) == Constants.RESULT_OK )
										{
										if( myWordItem_.replaceOrDeleteObsoleteJustification() == Constants.RESULT_OK )
											{
											if( foundUserSpecificationItem != null )
												{
												if( replaceOrDeleteSpecification( foundUserSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
													myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a duplicate user specification" );
												}
											}
										else
											myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete justification item" );
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
								foundReplacedHiddenSpecificationItem != null )
									{
									if( createdSpecificationItem.attachJustificationToSpecification( foundReplacedHiddenSpecificationItem.firstJustificationItem() ) == Constants.RESULT_OK )
										{
										if( replaceOrDeleteSpecification( foundReplacedHiddenSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
											myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found replacing hidden specification" );
										}
									else
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found replacing hidden specification to the created specification" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								foundReplacedUserSpecificationItem != null )
									{
									if( replaceOrDeleteSpecification( foundReplacedUserSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found replacing user specification" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								hasCopiedRelationContext &&
								firstJustificationItem != null &&
								foundSpecificationItem != null )
									{
									if( firstJustificationItem.changeAttachedJustification( foundSpecificationItem.firstJustificationItem() ) == Constants.RESULT_OK )
										{
										if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
											myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );
										}
									else
										myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								!isNegative &&
								!isPossessive &&
								!isQuestion &&
								!isSimilarOrRelatedQuestion_ &&
								specificationWordItem != null &&
								!createdSpecificationItem.isHiddenSpecification() )
									{
									if( myWordItem_.findAnswersToQuestions( specificationWordItem.compoundCollectionNr( specificationWordTypeNr ), createdSpecificationItem ) == Constants.RESULT_OK )
										{
										if( !isSelfGenerated &&
										!myWordItem_.isFemaleOrMale() )
											{
											if( specificationWordItem.isFemale() )
												{
												if( myWordItem_.markWordAsFemale() != Constants.RESULT_OK )
													myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark my word as female" );
												}
											else
												{
												if( specificationWordItem.isMale() )
													{
													if( myWordItem_.markWordAsMale() == Constants.RESULT_OK )
														{
														if( !isAssignment &&
														isGeneralizationProperName &&
														CommonVariables.currentLanguageWordItem != null &&
														// Has current language hidden specifications
														CommonVariables.currentLanguageWordItem.isNounWordCollectedWithItself() )
															{
															if( showAssumptionsThatAreNotHiddenAnymore() != Constants.RESULT_OK )
																myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to show assumptions that are not hidden anymore" );
															}
														}
													else
														myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark my word as male" );
													}
												}
											}
										}
									else
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
			return findRelatedSpecification( false, isCheckingRelationContext, false, false, searchSpecificationItem.isAssignment(), searchSpecificationItem.isInactiveAssignment(), searchSpecificationItem.isArchivedAssignment(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPossessive(), searchSpecificationItem.questionParameter(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.specificationWordItem() );

		specificationResult.result = myWordItem_.startErrorInWord( 1, moduleNameString_, "The given search specification item is undefined" );
		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( isIncludingAssignments )
			{
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result == Constants.RESULT_OK )
				{
				if( isIncludingArchivedAssignments &&
				specificationResult.relatedSpecificationItem == null )
					{
					// Archived assignments
					if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
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
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
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
