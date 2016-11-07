/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
 *	Version:		Thinknowlogy 2016r2 (Restyle)
 *************************************************************************/
/*	Copyright (C) 2009-2016, Menno Mafait. Your suggestions, modifications,
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

class WordSpecification
	{
	// Private constructed variables

	private boolean hasConfirmedSpecification_;
	private boolean hasCorrectedAssumptionByKnowledge_;
	private boolean hasCorrectedAssumptionByOppositeQuestion_;
	private boolean hasDisplayedMoreSpecificRelatedQuestion_;
	private boolean isConfirmedExclusive_;
	private boolean isMyWordTouchedDuringCurrentSentence_;
	private boolean isNonExclusiveSpecification_;
	private boolean isOnlyCheckingForConflicts_;
	private boolean isSameQuestionFromUser_;
	private boolean isSimilarOrRelatedQuestion_;

	private int spanishCompoundSpecificationCollectionNr_;
	private int userSpecificationCollectionNr_;

	private SpecificationItem confirmedReplacedSpecificationItem_;
	private SpecificationItem confirmedSpecificationButNoRelationReplacedSpecificationItem_;
	private SpecificationItem conflictingSpecificationItem_;
	private SpecificationItem correctedAssumptionReplacedSpecificationItem_;
	private SpecificationItem firstConfirmedReplacedSpanishSpecificationItem_;
	private SpecificationItem replacedAssignmentItem_;

	private WordItem spanishCompoundSpecificationWordItem_;


	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private void addToTouchedDuringCurrentSentenceWordList()
		{
		WordItem tempWordItem;
		WordItem lastTouchedWordItem = CommonVariables.firstTouchedWordItem;

		if( lastTouchedWordItem == null )
			CommonVariables.firstTouchedWordItem = myWordItem_;
		else
			{
			// Word order is important: Add word at end of temporary touched word list
			while( ( tempWordItem = lastTouchedWordItem.nextTouchedDuringCurrentSentenceWordItem ) != null )
				lastTouchedWordItem = tempWordItem;

			lastTouchedWordItem.nextTouchedDuringCurrentSentenceWordItem = myWordItem_;
			}
		}

	private byte addGeneralization( boolean isRelation, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem specificationWordItem )
		{
		GeneralizationResultType generalizationResult;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( generalizationResult = specificationWordItem.findGeneralization( isRelation, myWordItem_ ) ).result != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a generalization item" );

		if( !generalizationResult.hasFoundGeneralization )
			{
			// Specification word of a generalization word: noun
			if( specificationWordItem.createGeneralizationItem( isRelation, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a generalization item" );
			}

		return Constants.RESULT_OK;
		}

	private byte checkSpecificationWordForConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean hasFoundCompoundConflict = false;
		int nonCompoundSpecificationCollectionNr;
		int compoundSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		SpecificationItem anotherConflictingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem tempConflictingSpecificationItem;
		SpecificationItem pastTenseAssignmentItem = null;

		conflictingSpecificationItem_ = null;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// Derive conflict
		if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

		if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) == null )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			// Check for looping
			if( isSpecificationWordSpanishAmbiguous ||
			( conflictingSpecificationItem_ = specificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, myWordItem_ ) ) == null )
				{
				// Check for past tense
				foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem != null &&
				foundSpecificationItem.isArchivedAssignment() != isArchivedAssignment )
					conflictingSpecificationItem_ = foundSpecificationItem;
				else
					{
					if( !isSpecificationWordSpanishAmbiguous &&
					( compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
						{
						// Check for negative
						foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, specificationWordItem );

						if( foundSpecificationItem != null &&
						!specificationWordItem.isUserSpecificationWord &&
						foundSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
							hasFoundCompoundConflict = true;

						if( !isNegative &&
						foundSpecificationItem != null &&

						( hasFoundCompoundConflict ||
						compoundSpecificationCollectionNr != specificationCollectionNr ||
						foundSpecificationItem.isUserSpecification() ) )
							{
							// First check other conflict
							if( ( anotherConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, null ) ) != null )
								{
								// Write conflict
								if( anotherConflictingSpecificationItem.writeSpecificationConflict() != Constants.RESULT_OK )
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
								tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, null );

								if( tempConflictingSpecificationItem == null )
									{
									tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, compoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, null );

									if( tempConflictingSpecificationItem != null &&
									!tempConflictingSpecificationItem.isOlderItem() &&
									tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem )
										conflictingSpecificationItem_ = tempConflictingSpecificationItem;
									}
								else
									{
									if( tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem &&
									!tempConflictingSpecificationItem.isHiddenSpanishSpecification() )
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
		!conflictingSpecificationItem_.hasSpecificationBeenWrittenAsConflict &&

		( specificationCollectionNr == Constants.NO_COLLECTION_NR ||
		!specificationWordItem.hasNonExclusiveCollection( specificationCollectionNr ) ) )
			{
			if( !isExclusiveSpecification &&
			specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			!conflictingSpecificationItem_.isOlderItem() )
				{
				// Sentence in conflict with itself
				if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				// Write conflict
				if( conflictingSpecificationItem_.writeSpecificationConflict() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the conflicting specification" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte checkUserQuestion( boolean hasRelationContext, boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem foundQuestionSpecificationItem, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem relatedSpecificationItem;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( foundQuestionSpecificationItem != null &&
		foundQuestionSpecificationItem.isAnsweredQuestion() &&
		foundQuestionSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification &&
		foundQuestionSpecificationItem.relationContextNr() == relationContextNr )
			CommonVariables.isQuestionAlreadyAnswered = true;

		if( foundQuestionSpecificationItem == null ||
		!foundQuestionSpecificationItem.isQuestion() )
			{
			if( ( specificationResult = findRelatedSpecification( false, false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related question" );

			if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null &&
			!isExclusiveSpecification &&
			!isSpecificationGeneralization &&
			!specificationWordItem.hasNonExclusiveCollection( specificationCollectionNr ) )
				{
				if( relatedSpecificationItem.isOlderItem() )
					{
					if( hasRelationContext )
						{
						if( correctedAssumptionReplacedSpecificationItem_ != null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "The corrected assumption replaced specification item already assigned" );

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, true, true, relatedSpecificationItem.isNegative(), relatedSpecificationItem.isPossessive(), true, Constants.NO_QUESTION_PARAMETER, relatedSpecificationItem.specificationCollectionNr(), relatedSpecificationItem.relationContextNr(), relatedSpecificationItem.specificationWordItem() ) ) != null )
							{
							if( myWordItem_.writeUpdatedSpecification( false, false, false, true, false, false, correctedAssumptionReplacedSpecificationItem_ ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );

							hasCorrectedAssumptionByOppositeQuestion_ = true;
							}
						}
					}
				else
					{
					if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte checkUserSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isValueSpecification, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundRelationContext;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		WordItem currentLanguageWordItem;
		WordItem currentSpecificationWordItem;
		WordTypeItem pluralNounWordTypeItem;
		String unknownPluralNounString;

		if( foundSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given found specification item is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( foundSpecificationItem.isValueSpecification() != isValueSpecification )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "Value specification conflict! Specification word \"" + specificationWordItem.anyWordTypeString() + "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );

		if( !hasRelationContext &&
		!isAssignment &&
		!isExclusiveSpecification &&
		foundSpecificationItem.isExclusiveSpecification() &&
		foundSpecificationItem.isUserSpecification() )
			{
			if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );

			isNonExclusiveSpecification_ = true;
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
							if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
							}
						}
					else
						{
						if( !isInactiveAssignment )
							{
							if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific related specification" );
							}
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			if( !CommonVariables.hasDisplayedWarning )
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

						if( !foundSpecificationItem.isHiddenSpanishSpecification() )
							{
							if( myWordItem_.writeSelectedSpecification( false, true, foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the confirmed self-generated specification" );

							if( CommonVariables.writtenSentenceStringBuffer == null ||
							CommonVariables.writtenSentenceStringBuffer.length() == 0 )
								return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the confirmed self-generated specification" );

							if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( foundSpecificationItem.isSelfGeneratedAssumption() ? ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NO_RELATION ) : ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NO_RELATION ) ) ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a confirmation interface text" );

							if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a confirmed specification" );
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
							foundSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( hasFoundRelationContext, false, false, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, foundSpecificationItem.relationContextNr(), specificationWordItem );
						}

					if( foundSpecificationItem != null )
						{
						// Confirmation: Replace a self-generated by a user-entered specification
						if( foundSpecificationItem.isSelfGenerated() &&

						( hasRelationContext ||
						!foundSpecificationItem.isExclusiveSpecification() ) )
							{
							if( foundSpecificationItem.isExclusiveSpecification() )
								isConfirmedExclusive_ = true;

							if( foundSpecificationItem.isSelfGenerated() )
								hasConfirmedSpecification_ = true;

							confirmedReplacedSpecificationItem_ = foundSpecificationItem;

							if( !foundSpecificationItem.isHiddenSpanishSpecification() )
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
								if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START, pluralNounWordTypeItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a notification text" );

								confirmedReplacedSpecificationItem_ = foundSpecificationItem;
								currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

								if( currentLanguageWordItem != null &&
								( currentSpecificationWordItem = CommonVariables.firstSpecificationWordItem ) != null &&
								( unknownPluralNounString = currentLanguageWordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) ) != null )
									{
									// Do for all specification words
									do	{
										if( currentSpecificationWordItem.clearLastWrittenSentenceStringWithUnknownPluralNoun( unknownPluralNounString, specificationWordItem ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to clear the last written sentence string with specification word \"" + specificationWordItem.anyWordTypeString() + "\" in generalization word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
										}
									while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
									}
								}
							}
						}
					}
				}
			}

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

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// Skip if definition (singular noun) is Spanish ambiguous
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
			if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != Constants.RESULT_OK )
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
					if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			if( isQuestion &&
			foundSpecificationItem == null )
				foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

			if( foundSpecificationItem == null )
				{
				// Check current assignment or specification (without this relation)
				if( isQuestion )
					foundSpecificationItem = myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem == null )
					foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem );

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
							if( ( specificationResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

							relatedSpecificationItem = specificationResult.relatedSpecificationItem;

							if( hasRelationContext &&
							relatedSpecificationItem != null &&
							!relatedSpecificationItem.hasRelationContext() &&
							relatedSpecificationItem.isSelfGeneratedAssumption() )
								{
								// Assumption needs to be corrected
								if( correctedAssumptionReplacedSpecificationItem_ != null )
									return myWordItem_.startErrorInWord( 1, moduleNameString_, "The corrected assumption specification item already assigned" );

								if( myWordItem_.writeUpdatedSpecification( false, false, true, false, false, false, relatedSpecificationItem ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );

								hasCorrectedAssumptionByKnowledge_ = true;
								correctedAssumptionReplacedSpecificationItem_ = relatedSpecificationItem;
								}
							else
								{
								if( !isPossessive &&
								!isQuestion &&
								myWordItem_.isUserGeneralizationWord &&

								( relatedSpecificationItem == null ||

								( ( !relatedSpecificationItem.hasRelationContext() ||
								relatedSpecificationItem.specificationWordItem() != specificationWordItem ) &&

								( !specificationWordItem.isNounWordSpanishAmbiguous() ||
								!relatedSpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ) &&

								// Skip checking for conflicts if a question with the current specification word exists
								myWordItem_.firstSpecificationItem( false, false, Constants.WORD_PARAMETER_SINGULAR_VERB_IS, specificationWordItem ) == null )
									{
									if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
									}
								}
							}
						}

					if( !CommonVariables.hasDisplayedWarning &&
					!hasRelationContext &&
					isAssignment &&
					!isInactiveAssignment &&
					!isArchivedAssignment &&
					!isNegative &&
					!isValueSpecification &&
					specificationContextNr == Constants.NO_CONTEXT_NR &&
					!myWordItem_.isLanguageWord() &&
					myWordItem_.isNounWordType( specificationWordTypeNr ) &&
					// Negative specification doesn't exist
					myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == null )
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
						if( foundWordTypeItem.hasFeminineOrMasculineWordEnding() )
							{
							hasFeminineSingularNounEnding = foundWordTypeItem.hasFeminineWordEnding();

							if( !myWordItem_.isCorrectIndefiniteArticle( ( hasFeminineSingularNounEnding ? Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ), generalizationWordTypeNr ) )
								{
								foundWordTypeItem.clearIndefiniteArticleParameter();

								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_SINGULAR_NOUN_TO_BE, myWordItem_.anyWordTypeString(), ( hasFeminineSingularNounEnding ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a singular noun expected to be feminine or masculine" );

								CommonVariables.hasDisplayedArticleNotification = true;
								}
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
					if( myWordItem_.writeSelectedSpecification( false, true, foundSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					if( CommonVariables.writtenSentenceStringBuffer == null ||
					CommonVariables.writtenSentenceStringBuffer.length() == 0 )
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write a sentence with an assumption about the relation" );

					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a missing relation" );

					if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					// Force 'I know' notification
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the 'I know' interface notification" );
					}
				}

			if( !CommonVariables.hasDisplayedWarning )
				{
				if( isQuestion )
					{
					if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the user question" );

					// Detect self-generated question
					if( findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting self-generated question" );

					// Detect user question
					if( findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting user question" );

					isOnlyCheckingForConflicts_ = true;
					}
				else
					{
					if( foundSpecificationItem != null )
						{
						if( checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isValueSpecification, specificationWordTypeNr, specificationCollectionNr, relationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check the user specification" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte displayAssumptionsThatAreNotHiddenAnymore()
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
						if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I have found an undefined generalization word" );

						if( ( hiddenSpecificationItem = currentGeneralizationWordItem.firstUnhiddenSpecificationItem( currentContextItem.contextNr() ) ) != null )
							{
							if( currentGeneralizationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, hiddenSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an assumption that is not hidden anymore" );
							}
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
					}
				}
			while( ( currentContextItem = currentContextItem.nextContextItem() ) != null );
			}

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
			if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write 'I had the same question before' interface notification" );

			isSameQuestionFromUser_ = true;
			}

		if( ( currentWordItem = CommonVariables.firstWordItem ) == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The first word item is undefined" );

		// Do for all words
		// Only checking specification words is not sufficient
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
				if( isSameQuestion )
					{
					if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? Constants.INTERFACE_QUESTION_I_HAD_THE_SAME_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about the same question" );

					if( foundQuestionSpecificationItem.isUserQuestion() )
						isSameQuestionFromUser_ = true;
					}
				else
					{
					if( myWordItem_.writeSelectedSpecification( false, false, foundQuestionSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the found specification" );

						if( CommonVariables.writtenSentenceStringBuffer == null ||
						CommonVariables.writtenSentenceStringBuffer.length() == 0 )
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the found specification" );

					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE ) : ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE ) ) ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a related question" );

					if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

					isSimilarOrRelatedQuestion_ = true;
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
						hasDisplayedMoreSpecificRelatedQuestion_ = true;

						// Skip displaying announcement if question is in conflict with itself
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
					if( hasDisplayedMoreSpecificRelatedQuestion_ &&
					isConflictingQuestion )
						{
						if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem createdSpecificationItem )
		{
		if( obsoleteSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem.isAssignment() )
			{
			if( replacedAssignmentItem_ == null &&
			firstConfirmedReplacedSpanishSpecificationItem_ != null &&
			myWordItem_.nContextWordsInAllWords( obsoleteSpecificationItem.relationContextNr(), obsoleteSpecificationItem.specificationWordItem() ) == CommonVariables.nUserRelationWords )
				{
				if( myWordItem_.replaceOrDeleteSpecification( true, firstConfirmedReplacedSpanishSpecificationItem_, obsoleteSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a confirmed specification" );
				}

			replacedAssignmentItem_ = obsoleteSpecificationItem;
			}
		else
			{
			if( myWordItem_.replaceOrDeleteSpecification( false, obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a specification" );

			if( firstConfirmedReplacedSpanishSpecificationItem_ == null &&
			obsoleteSpecificationItem.isReplacedItem() )
				firstConfirmedReplacedSpanishSpecificationItem_ = obsoleteSpecificationItem;
			}

		return Constants.RESULT_OK;
		}

	private byte replaceObsoleteSpecification( SpecificationItem createdSpecificationItem, SpecificationItem foundUserSpecificationItem, SpecificationItem obsoleteAssumptionSpecificationItem, SpecificationItem obsoleteConcludedAssumptionSpecificationItem, SpecificationItem obsoleteNotCollectedSpecificationItem, SpecificationItem obsoleteHiddenSpanishSpecificationItem, SpecificationItem obsoleteUserSpecificationItem )
		{
		if( createdSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given created specification is undefined" );

		if( confirmedReplacedSpecificationItem_ != null )
			{
			if( replaceOrDeleteSpecification( confirmedReplacedSpecificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a confirmed specification" );

			if( confirmedReplacedSpecificationItem_.isReplacedItem() )
				{
				if( myWordItem_.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace obsolete justification items" );
				}

			if( foundUserSpecificationItem != null &&
			!foundUserSpecificationItem.isReplacedOrDeletedItem() )
				{
				if( replaceOrDeleteSpecification( foundUserSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a duplicate user specification" );
				}
			}

		if( confirmedSpecificationButNoRelationReplacedSpecificationItem_ != null )
			{
			if( myWordItem_.confirmSpecificationButNotItsRelation( confirmedSpecificationButNoRelationReplacedSpecificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to confirm a specification, but no relation(s)" );
			}

		if( correctedAssumptionReplacedSpecificationItem_ != null )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a corrected specification" );

			correctedAssumptionReplacedSpecificationItem_ = null;
			}

		if( obsoleteAssumptionSpecificationItem != null )
			{
			if( myWordItem_.attachJustificationInWord( obsoleteAssumptionSpecificationItem.firstJustificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the first justification of the found assumption specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteAssumptionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found assumption specification" );
			}

		if( obsoleteConcludedAssumptionSpecificationItem != null )
			{
			if( replaceOrDeleteSpecification( obsoleteConcludedAssumptionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found replacing user specification" );
			}

		if( obsoleteNotCollectedSpecificationItem != null )
			{
			if( replaceOrDeleteSpecification( obsoleteNotCollectedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found replacing user specification" );

			if( myWordItem_.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace obsolete justification items" );
			}

		if( obsoleteHiddenSpanishSpecificationItem != null )
			{
			if( createdSpecificationItem.attachJustificationToSpecification( obsoleteHiddenSpanishSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found replaced hidden specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteHiddenSpanishSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found replaced hidden specification" );
			}

		if( obsoleteUserSpecificationItem != null )
			{
			if( replaceOrDeleteSpecification( obsoleteUserSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found replacing user specification" );
			}

		return Constants.RESULT_OK;
		}

	private byte writeConfirmedSpecification( SpecificationItem writeSpecificationItem )
		{
		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( myWordItem_.writeSelectedSpecification( false, true, writeSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

		if( CommonVariables.writtenSentenceStringBuffer == null ||
		CommonVariables.writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given write specification" );

		if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( writeSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : Constants.INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ) ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a confirmation" );

		if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

		return Constants.RESULT_OK;
		}

	private byte writeMoreSpecificSpecification( SpecificationItem olderSpecificationItem )
		{
		if( olderSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given older specification item is undefined" );

		if( !olderSpecificationItem.isOlderItem() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given older specification item isn't old" );

		if( myWordItem_.writeSelectedSpecification( false, !olderSpecificationItem.isExclusiveSpecification(), olderSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the given older specification" );

		if( CommonVariables.writtenSentenceStringBuffer == null ||
		CommonVariables.writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given older specification" );

		if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( olderSpecificationItem.isQuestion() ? ( olderSpecificationItem.isSelfGenerated() ? Constants.INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : Constants.INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem.isSelfGenerated() ? ( olderSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : Constants.INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface listing text" );

		if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );

		return Constants.RESULT_OK;
		}

	private SpecificationResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean isFirstRelatedSpecification = true;
		boolean isLastRelatedSpecification = false;
		int currentSpecificationSentenceNr;
		int currentSpecificationItemNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		int highestItemNr = Constants.NO_ITEM_NR;
		SpecificationItem currentSpecificationItem = ( isCheckingAllQuestions ? myWordItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) : myWordItem_.firstSelectedSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) );
		WordItem currentSpecificationWordItem;

		if( specificationWordItem == null )
			return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( currentSpecificationItem != null &&
		specificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem.relatedSpecificationWordItem( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) != null )
					{
					currentSpecificationSentenceNr = currentSpecificationItem.creationSentenceNr();
					currentSpecificationItemNr = currentSpecificationItem.itemNr();

					if( currentSpecificationWordItem == specificationWordItem &&
					currentSpecificationItem.relationContextNr() == relationContextNr )
						{
						// Found the given specification item
						isFirstRelatedSpecification = false;

						if( currentSpecificationSentenceNr > highestSentenceNr ||

						( currentSpecificationSentenceNr == highestSentenceNr &&
						currentSpecificationItemNr > highestItemNr ) )
							{
							isLastRelatedSpecification = true;

							highestSentenceNr = currentSpecificationSentenceNr;
							highestItemNr = currentSpecificationItemNr;
							}
						}
					else
						{
						if( currentSpecificationSentenceNr > highestSentenceNr ||

						( currentSpecificationSentenceNr == highestSentenceNr &&
						currentSpecificationItemNr > highestItemNr ) )
							{
							isLastRelatedSpecification = false;

							highestSentenceNr = currentSpecificationSentenceNr;
							highestItemNr = currentSpecificationItemNr;
							}

						// If the specification has no relation, select the oldest one (the first of a series)
						if( !hasRelationContext ||
						specificationResult.relatedSpecificationItem == null ||

						( currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
						currentSpecificationWordItem != specificationWordItem ) )
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

		return specificationResult;
		}


	// Constructor

	protected WordSpecification( WordItem myWordItem )
		{
		String errorString = null;

		hasConfirmedSpecification_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasCorrectedAssumptionByOppositeQuestion_ = false;
		hasDisplayedMoreSpecificRelatedQuestion_ = false;
		isConfirmedExclusive_ = false;
		isMyWordTouchedDuringCurrentSentence_ = false;
		isNonExclusiveSpecification_ = false;
		isOnlyCheckingForConflicts_ = false;
		isSameQuestionFromUser_ = false;
		isSimilarOrRelatedQuestion_ = false;

		spanishCompoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		confirmedReplacedSpecificationItem_ = null;
		confirmedSpecificationButNoRelationReplacedSpecificationItem_ = null;
		conflictingSpecificationItem_ = null;
		correctedAssumptionReplacedSpecificationItem_ = null;
		firstConfirmedReplacedSpanishSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		spanishCompoundSpecificationWordItem_ = null;

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
		hasDisplayedMoreSpecificRelatedQuestion_ = false;
		isMyWordTouchedDuringCurrentSentence_ = false;
		isOnlyCheckingForConflicts_ = false;

		spanishCompoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		firstConfirmedReplacedSpanishSpecificationItem_ = null;
		spanishCompoundSpecificationWordItem_ = null;
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

	protected boolean hasDisplayedMoreSpecificRelatedQuestion()
		{
		return hasDisplayedMoreSpecificRelatedQuestion_;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		conflictingSpecificationItem_ = null;

		if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );

		if( conflictingSpecificationItem_ != null &&
		conflictingSpecificationItem_.isOlderItem() &&

		( isNegative ||
		!conflictingSpecificationItem_.isNegative() ||
		conflictingSpecificationItem_.hasSpecificationCompoundCollection() ) )
			{
			// Opposite negative of conflicting specification word
			if( checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, !isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, conflictingSpecificationItem_.specificationWordItem() ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for the opposite negative specification conflict" );
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecificationInWord( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasCopiedRelationContext = ( copiedRelationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasFoundSpecificationWithDifferentRelationContext = false;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelfGenerated = ( firstJustificationItem != null );
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUserRelationWord = myWordItem_.isUserRelationWord;
		int nonCompoundCollectionNr;
		int originalSentenceNr;
		int compoundCollectionNr = Constants.NO_COLLECTION_NR;
		int generalizationCollectionNr = Constants.NO_COLLECTION_NR;
		SpecificationItem createdSpecificationItem;
		SpecificationItem replacingSpecificationItem;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem foundUserSpecificationItem = null;
		SpecificationItem obsoleteAssumptionSpecificationItem = null;
		SpecificationItem obsoleteConcludedAssumptionSpecificationItem = null;
		SpecificationItem obsoleteNotCollectedSpecificationItem = null;
		SpecificationItem obsoleteHiddenSpanishSpecificationItem = null;
		SpecificationItem obsoleteUserSpecificationItem = null;

		isConfirmedExclusive_ = false;
		isNonExclusiveSpecification_ = false;
		isSameQuestionFromUser_ = false;

		if( !isMyWordTouchedDuringCurrentSentence_ )
			{
			isMyWordTouchedDuringCurrentSentence_ = true;
			addToTouchedDuringCurrentSentenceWordList();
			}

		confirmedReplacedSpecificationItem_ = null;
		confirmedSpecificationButNoRelationReplacedSpecificationItem_ = null;
		correctedAssumptionReplacedSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		// Check generalization word type
		if( !myWordItem_.hasWordType( true, generalizationWordTypeNr ) )
			return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		// Check relation variables
		if( ( hasRelationContext ||
		relationWordTypeNr != Constants.NO_WORD_TYPE_NR ||
		relationWordItem != null ) &&

		( ( !hasRelationContext &&
		// Exception: Conditions have no relation context
		!isConditional ) ||

		relationWordItem == null ||
		!relationWordItem.hasWordType( false, relationWordTypeNr ) ) )
			return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The relation variables aren't matching" );

		if( specificationWordItem != null )
			{
			// Check specification word type
			if( !specificationWordItem.hasWordType( true, specificationWordTypeNr ) &&
			// New created language
			!myWordItem_.isLanguageWord() )
				return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification word type" );

			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			if( !isSpecificationWordSpanishAmbiguous )
				generalizationCollectionNr = myWordItem_.collectionNr( generalizationWordTypeNr, specificationWordItem );

			// Collect definition specification without specification collection
			if( !isExclusiveSpecification &&
			!isQuestion &&
			!specificationWordItem.hasCollection() &&
			myWordItem_.isNounWordType( generalizationWordTypeNr ) &&
			myWordItem_.isNounWordType( specificationWordTypeNr ) )
				{
				if( myWordItem_.addCollectionByGeneralization( isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to add a collection by generalization of specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
				}

			if( hasSpecificationCollection )
				{
				if( !specificationWordItem.hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification collection" );
				}
			else
				{
				if( !isSelfGenerated &&
				userSpecificationCollectionNr_ > Constants.NO_COLLECTION_NR )
					// Use previous user specification collection
					specificationCollectionNr = userSpecificationCollectionNr_;
				else
					{
					compoundCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr );
					nonCompoundCollectionNr = specificationWordItem.nonCompoundCollectionNr( specificationWordTypeNr );

					if( nonCompoundCollectionNr > Constants.NO_COLLECTION_NR &&

					( ( compoundCollectionNr == Constants.NO_COLLECTION_NR &&
					// Not a definition sentence with the same generalization and specification word
					!specificationWordItem.isUserGeneralizationWord ) ||

					// Non-exclusive specification collection
					( !isExclusiveSpecification &&
					!isQuestion &&

					( !isSpecificationWordSpanishAmbiguous ||

					( hasRelationContext &&

					( nContextRelations > 1 ||
					!specificationWordItem.isUserSpecificationWord ) ) ) ) ) )
						specificationCollectionNr = nonCompoundCollectionNr;
					else
						specificationCollectionNr = compoundCollectionNr;
					}
				}

			if( !isSelfGenerated &&
			myWordItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				if( checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check a user specification or question with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

				if( isGeneralizationProperName ||

				( !isAssignment &&
				!isEveryGeneralization ) )
					userSpecificationCollectionNr_ = specificationCollectionNr;
				}

			if( !CommonVariables.hasDisplayedWarning )
				{
				// Try to find specification
				// Start with finding user specification
				if( ( foundUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, true, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == null )
					{
					if( isSelfGenerated )
						{
						if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, false, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : ( hasCopiedRelationContext ? copiedRelationContextNr : relationContextNr ) ), specificationWordItem ) ) == null )
							{
							// Not found
							if( hasRelationContext &&
							!isQuestion &&
							( replacingSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, relationContextNr, specificationWordItem ) ) != null )
								{
								if( ( isSpecificationWordSpanishAmbiguous ||
								compoundCollectionNr > Constants.NO_COLLECTION_NR ) &&

								!replacingSpecificationItem.hasSpecificationCollection() &&
								replacingSpecificationItem.isSelfGenerated() )
									obsoleteNotCollectedSpecificationItem = replacingSpecificationItem;
								else
									{
									if( isSpecificationWordSpanishAmbiguous &&
									replacingSpecificationItem.hasRelationContext() )
										{
										if( replacingSpecificationItem.isOlderItem() &&
										replacingSpecificationItem.isHiddenSpanishSpecification() )
											obsoleteHiddenSpanishSpecificationItem = replacingSpecificationItem;
										}
									else
										{
										if( ( !isSpecificationWordSpanishAmbiguous ||
										hasSpecificationCollection ) &&

										( isUserRelationWord &&
										replacingSpecificationItem.isSelfGenerated() &&
										replacingSpecificationItem.hasSpecificationNonCompoundCollection() &&
										// Exclude archived assignment
										!replacingSpecificationItem.hasCurrentCreationSentenceNr() ) )
											{
											if( confirmedReplacedSpecificationItem_ != null )
												return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The confirmed replaced specification item is already defined" );

											if( myWordItem_.writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, isSpecificationWordSpanishAmbiguous, false, false, false, !isSpecificationWordSpanishAmbiguous, replacingSpecificationItem ) != Constants.RESULT_OK )
												return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an assumption, which will have an added relation" );

											confirmedReplacedSpecificationItem_ = replacingSpecificationItem;
											}
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
								foundSpecificationItem.isOlderItem() )
									{
									if( firstJustificationItem.isConclusionJustification() )
										{
										if( foundSpecificationItem.isSelfGeneratedAssumption() &&

										( !hasRelationContext ||
										foundSpecificationItem.hasSpecificationCompoundCollection() ||

										( isUserRelationWord &&
										foundSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null &&

										( isSpecificationWordSpanishAmbiguous ||
										foundSpecificationItem.hasOnlyOneRelationWord() ) ) ) &&

										( ( !isSpecificationWordSpanishAmbiguous &&
										// Conclusion doesn't exist
										myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, null ) == null ) ||

										// Typically for the Spanish language
										( isSpecificationWordSpanishAmbiguous &&
										!foundSpecificationItem.isHiddenSpanishSpecification() ) ) )
											{
											if( foundSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
												return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

											if( isUserRelationWord ||
											foundSpecificationItem.hasCurrentActiveSentenceNr() ||
											!foundSpecificationItem.hasSpecificationCompoundCollection() )
												{
												if( myWordItem_.writeUpdatedSpecification( true, false, false, false, true, false, foundSpecificationItem ) != Constants.RESULT_OK )
													return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a concluded assumption" );
												}
											}
										}
									else
										{
										if( hasRelationContext &&
										!isUserRelationWord &&
										!isSpecificationWordSpanishAmbiguous &&
										foundSpecificationItem.isConcludedAssumption() )
											obsoleteConcludedAssumptionSpecificationItem = foundSpecificationItem;
										}
									}
								}
							else
								{
								if( hasRelationContext &&
								foundSpecificationItem.hasRelationContext() )
									{
									hasFoundSpecificationWithDifferentRelationContext = true;

									if( !isSpecificationWordSpanishAmbiguous &&
									firstJustificationItem != null &&

									( ( foundSpecificationItem.isSelfGeneratedAssumption() &&
									firstJustificationItem.isOppositePossessiveConditionalSpecificationAssumption() ) ||

									( foundSpecificationItem.isSelfGeneratedConclusion() &&
									firstJustificationItem.isPossessiveReversibleConclusion() ) ) )
										obsoleteAssumptionSpecificationItem = foundSpecificationItem;
									}
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

							if( isGeneralizationProperName &&
							specificationCollectionNr > Constants.NO_COLLECTION_NR &&

							( ( isAssignment &&
							confirmedReplacedSpecificationItem_ != null &&
							foundUserSpecificationItem.isUserAssignment() ) ||

							!foundUserSpecificationItem.hasSpecificationCollection() ) )
								// Prepare foundUserSpecificationItem for confirmation
								foundUserSpecificationItem = myWordItem_.firstSpecificationItem( isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );
							else
								foundUserSpecificationItem = null;
							}
						else	// Typically for compound collections in Spanish
							{
							if( spanishCompoundSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR ||
							spanishCompoundSpecificationWordItem_ == null )
								{
								if( ( specificationCollectionNr = myWordItem_.highestCollectionNrInAllWords() ) >= Constants.MAX_COLLECTION_NR )
									return myWordItem_.startSpecificationResultSystemErrorInWord( 1, moduleNameString_, "Collection number overflow" );

								spanishCompoundSpecificationCollectionNr_ = ++specificationCollectionNr;
								spanishCompoundSpecificationWordItem_ = specificationWordItem;
								}
							else
								{
								specificationCollectionNr = spanishCompoundSpecificationCollectionNr_;

								if( spanishCompoundSpecificationWordItem_.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
									return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + spanishCompoundSpecificationWordItem_.anyWordTypeString() + "\" with word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								if( specificationWordItem.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishCompoundSpecificationWordItem_, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
									return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + specificationWordItem.anyWordTypeString() + "\" with word \"" + spanishCompoundSpecificationWordItem_.anyWordTypeString() + "\"" );
								}
							}
						}
					else
						{
						if( foundUserSpecificationItem.hasGeneralizationCollection() &&
						foundUserSpecificationItem.isSpecificationWordSpanishAmbiguous() )
							specificationCollectionNr = foundUserSpecificationItem.generalizationCollectionNr();
						}
					}
				}

			// Add generalization items
			if( hasRelationContext ||
			foundSpecificationItem == null )
				{
				// Specification word
				if( addGeneralization( false, generalizationWordTypeNr, specificationWordTypeNr, specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to add a specification word to the generalizations" );

				// Relation word
				if( relationWordItem != null )
					{
					if( addGeneralization( true, generalizationWordTypeNr, relationWordTypeNr, relationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to add a relation word to the generalizations" );
					}
				}
			}

		if( !CommonVariables.hasDisplayedWarning )
			{
			if( foundSpecificationItem == null ||

			// Exceptions
			isNonExclusiveSpecification_ ||
			confirmedReplacedSpecificationItem_ != null ||
			correctedAssumptionReplacedSpecificationItem_ != null ||
			obsoleteConcludedAssumptionSpecificationItem != null ||

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

			// User specification has copied relation context
			( hasCopiedRelationContext &&
			isSelfGenerated &&
			foundSpecificationItem.isUserSpecification() ) ||

			// Accept different relation context (e.g. ambiguous specification)
			( hasFoundSpecificationWithDifferentRelationContext &&
			!isArchivedAssignment &&
			isSelfGenerated ) )
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
					// Check with other replacements if this one is unique
					foundSpecificationItem != confirmedReplacedSpecificationItem_ &&
					foundSpecificationItem != confirmedSpecificationButNoRelationReplacedSpecificationItem_ &&
					foundSpecificationItem != correctedAssumptionReplacedSpecificationItem_ &&
					foundSpecificationItem != obsoleteAssumptionSpecificationItem &&
					foundSpecificationItem != obsoleteConcludedAssumptionSpecificationItem &&
					foundSpecificationItem != obsoleteNotCollectedSpecificationItem &&
					foundSpecificationItem != obsoleteHiddenSpanishSpecificationItem )
						obsoleteUserSpecificationItem = foundSpecificationItem;
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
				if( ( specificationResult = myWordItem_.createSpecificationItem( false, false, false, false, false, isConditional, ( hasCorrectedAssumptionByKnowledge_ || hasCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveSpecification, false, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, Constants.NO_ASSIGNMENT_LEVEL, assumptionLevel, CommonVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? Constants.NO_WORD_TYPE_NR : relationWordTypeNr ), generalizationCollectionNr, specificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to create a specification item" );

				if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
					return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "I couldn't create a specification item" );

				// Now replaced obsolete specifications by the created specification
				if( replaceObsoleteSpecification( createdSpecificationItem, foundUserSpecificationItem, obsoleteAssumptionSpecificationItem, obsoleteConcludedAssumptionSpecificationItem, obsoleteNotCollectedSpecificationItem, obsoleteHiddenSpanishSpecificationItem, obsoleteUserSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete specification" );

				if( hasCopiedRelationContext &&
				firstJustificationItem != null &&
				foundSpecificationItem != null )
					{
					if( !firstJustificationItem.isOlderItem() &&

					( isSpecificationWordSpanishAmbiguous ||
					foundSpecificationItem.hasCurrentCreationSentenceNr() ) )
						{
						if( firstJustificationItem.changeAttachedJustification( foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
							return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
						}

					if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) == Constants.RESULT_OK )
						{
						if( myWordItem_.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
							return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace obsolete justification items" );
						}
					else
						return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );
					}

				if( !isNegative &&
				!isPossessive &&
				!isQuestion &&
				!isSimilarOrRelatedQuestion_ &&
				specificationWordItem != null &&

				( !isSpecificationWordSpanishAmbiguous ||
				!createdSpecificationItem.isHiddenSpanishSpecification() ) )
					{
					if( myWordItem_.findAnswersToQuestions( specificationWordItem.compoundCollectionNr( specificationWordTypeNr ), createdSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find answers to questions" );

					if( !isSelfGenerated &&
					!myWordItem_.isFemaleOrMale() )
						{
						if( specificationWordItem.isFemale() )
							{
							if( myWordItem_.markWordAsFemale() != Constants.RESULT_OK )
								return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as female" );
							}
						else
							{
							if( specificationWordItem.isMale() )
								{
								if( myWordItem_.markWordAsMale() != Constants.RESULT_OK )
									return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as male" );

								if( !isAssignment &&
								isGeneralizationProperName &&
								myWordItem_.isSpanishCurrentLanguage() )
									{
									if( displayAssumptionsThatAreNotHiddenAnymore() != Constants.RESULT_OK )
										return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to display assumptions that are not hidden anymore" );
									}
								}
							}
						}
					}
				}
			}

		specificationResult.foundSpecificationItem = foundSpecificationItem;
		specificationResult.replacedAssignmentItem = replacedAssignmentItem_;

		return specificationResult;
		}

	protected SpecificationResultType findRelatedSpecification( boolean isCheckingRelationContext, SpecificationItem searchSpecificationItem )
		{
		if( searchSpecificationItem == null )
			return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The given search specification item is undefined" );

		return findRelatedSpecification( false, isCheckingRelationContext, false, false, searchSpecificationItem.isAssignment(), searchSpecificationItem.isInactiveAssignment(), searchSpecificationItem.isArchivedAssignment(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPossessive(), searchSpecificationItem.questionParameter(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.specificationWordItem() );
		}

	protected SpecificationResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( isIncludingAssignments )
			{
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			specificationResult.relatedSpecificationItem == null )
				{
				// Archived assignments
				if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification as well" );
				}
			}

		// Specifications
		if( specificationResult.relatedSpecificationItem == null )
			{
			if( ( specificationResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );
			}

		return specificationResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
