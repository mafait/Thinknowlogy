/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

		// Private constructed variables

	private boolean hasCurrentlyConfirmedSpecification_ = false;
	private boolean hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = false;
	private boolean hasCurrentlyConfirmedSpecificationButNoRelation_ = false;
	private boolean hasCurrentlyCorrectedAssumptionByKnowledge_ = false;
	private boolean hasCurrentlyCorrectedAssumptionByOppositeQuestion_ = false;
	private boolean hasCurrentlyMoreSpecificSpecification_ = false;
	private boolean hasCurrentlyMoreSpecificNonExclusiveSpecification_ = false;
	private boolean hasCurrentlyMoreSpecificQuestion_ = false;
	private boolean isOnlyCheckingForConflicts_ = false;
	private boolean isWordTouchedDuringCurrentSentence_ = false;

	private int spanishCompoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
	private int userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

	private SpecificationItem correctedAssumptionReplacedSpecificationItem_ = null;
	private SpecificationItem replacedAssignmentItem_ = null;

	private WordItem spanishCompoundSpecificationWordItem_ = null;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private WordItem myWordItem_;


	// Private methods

	private void addMyWordToTouchedDuringCurrentSentenceList()
		{
		WordItem lastTouchedWordItem = GlobalVariables.firstTouchedWordItem;
		WordItem tempTouchedWordItem;

		if( !isWordTouchedDuringCurrentSentence_ )
			{
			isWordTouchedDuringCurrentSentence_ = true;

			if( lastTouchedWordItem == null )
				GlobalVariables.firstTouchedWordItem = myWordItem_;
			else
				{
				// Word order is important: Add word at end of temporary touched word list
				while( ( tempTouchedWordItem = lastTouchedWordItem.nextTouchedWordItem ) != null )
					lastTouchedWordItem = tempTouchedWordItem;

				lastTouchedWordItem.nextTouchedWordItem = myWordItem_;
				}
			}
		}

	private static int nonCompoundCollectionNrInCollectionWords( int compoundCollectionNr )
		{
		int nonCompoundCollectionNr;
		WordItem currentCollectionWordItem;

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( ( nonCompoundCollectionNr = currentCollectionWordItem.nonCompoundCollectionNrInWord( compoundCollectionNr ) ) > Constants.NO_COLLECTION_NR )
					return nonCompoundCollectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		return Constants.NO_COLLECTION_NR;
		}

	private byte addGeneralization( boolean isRelation, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem specificationWordItem )
		{
		BoolResultType boolResult;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( boolResult = specificationWordItem.findGeneralization( isRelation, myWordItem_ ) ).result != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a generalization item" );

		// Generalization not found
		if( !boolResult.booleanValue &&
		// Specification word of a generalization word: noun
		specificationWordItem.createGeneralizationItem( isRelation, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a generalization item" );

		return Constants.RESULT_OK;
		}

	private byte checkUserQuestion( boolean hasRelationContext, boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, SpecificationItem foundQuestionSpecificationItem, WordItem specificationWordItem )
		{
		SpecificationItem relatedSpecificationItem;
		RelatedResultType relatedResult;

		if( foundQuestionSpecificationItem != null &&
		foundQuestionSpecificationItem.isAnsweredQuestion() &&
		foundQuestionSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification &&
		foundQuestionSpecificationItem.relationContextNr() == relationContextNr )
			GlobalVariables.isQuestionAlreadyAnswered = true;

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&

		( foundQuestionSpecificationItem == null ||
		!foundQuestionSpecificationItem.isQuestion() ) )
			{
			if( ( relatedResult = findRelatedSpecification( false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related question" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
			!isExclusiveSpecification &&
			!isSpecificationGeneralization )
				{
				if( relatedSpecificationItem.isOlderItem() )
					{
					if( hasRelationContext )
						{
						if( correctedAssumptionReplacedSpecificationItem_ != null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "The corrected assumption replaced specification item already assigned" );

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, true, relatedSpecificationItem.isNegative(), relatedSpecificationItem.isPossessive(), true, Constants.NO_QUESTION_PARAMETER, relatedSpecificationItem.specificationCollectionNr(), relatedSpecificationItem.relationContextNr(), relatedSpecificationItem.specificationWordItem() ) ) != null )
							{
							if( myWordItem_.writeUpdatedSpecification( false, false, true, false, false, correctedAssumptionReplacedSpecificationItem_ ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );

							hasCurrentlyCorrectedAssumptionByOppositeQuestion_ = true;
							}
						}
					}
				else
					{
					if( relatedSpecificationItem.hasCompoundSpecificationCollection() )
						{
						// Current question in conflict with definition
						if( writeSpecificationInConflictWithDefinition( true, specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write that the current question is in conflict with a definition" );

						GlobalVariables.isConflictingQuestion = true;
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte collectSpecifications( boolean isExclusiveGeneralization, boolean isQuestion, short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		int specificationCollectionNr;
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;
		WordItem foundGeneralizationWordItem = null;
		CollectionResultType collectionResult;

		if( collectionWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given collection word item is undefined" );

		if( ( currentGeneralizationItem = myWordItem_.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return myWordItem_.startSystemErrorInWord( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( collectionResult = currentGeneralizationWordItem.collectSpecifications( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to add a specification collection by exclusive specification in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

				if( ( foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem ) != null &&
				( specificationCollectionNr = collectionWordItem.collectionNr( currentGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR &&
				// Collect by generalization
				foundGeneralizationWordItem.collectSpecificationsInWord( isExclusiveGeneralization, isQuestion, specificationCollectionNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to collect specifications in word \"" + foundGeneralizationWordItem.anyWordTypeString() + "\"" );
				}
			while( foundGeneralizationWordItem == null &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte processPreviouslyUnknownPluralNoun( WordItem specificationWordItem )
		{
		WordItem currentSpecificationWordItem;
		WordTypeItem pluralNounWordTypeItem;
		String unknownPluralNounString;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( pluralNounWordTypeItem = specificationWordItem.activeWordTypeItem( Constants.WORD_TYPE_NOUN_PLURAL ) ) != null &&
		// Skip announcement for new occurrences
		!pluralNounWordTypeItem.isOlderItem() )
			{
			if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START, pluralNounWordTypeItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a notification text" );

			if( ( currentSpecificationWordItem = GlobalVariables.firstSpecificationWordItem ) != null &&
			( unknownPluralNounString = myWordItem_.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) ) != null )
				{
				// Do for all specification words
				do	{
					if( currentSpecificationWordItem.clearStoredSentenceStringWithUnknownPluralNoun( unknownPluralNounString, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to clear the stored sentence string with specification word \"" + specificationWordItem.anyWordTypeString() + "\" in word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
					}
				while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem createdSpecificationItem )
		{
		if( obsoleteSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem.isAssignment() )
			replacedAssignmentItem_ = obsoleteSpecificationItem;
		else
			{
			if( myWordItem_.replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a specification" );
			}

		return Constants.RESULT_OK;
		}

	private byte replaceObsoleteSpecification( boolean isConfirmedSpecificationButNotItsRelation, SpecificationItem confirmedSpecificationItem, SpecificationItem createdSpecificationItem, SpecificationItem obsoleteAssumptionSpecificationItem, SpecificationItem obsoleteHiddenSpanishSpecificationItem, SpecificationItem obsoleteSpecificationItem )
		{
		if( createdSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given created specification is undefined" );

		if( confirmedSpecificationItem != null &&
		// Replace confirmed specification
		replaceOrDeleteSpecification( confirmedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the confirmed specification" );

		if( isConfirmedSpecificationButNotItsRelation &&
		myWordItem_.confirmSpecificationButNotItsRelation( createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to confirm a specification, but not its relation(s)" );

		if( correctedAssumptionReplacedSpecificationItem_ != null )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a corrected assumption replaced specification" );

			correctedAssumptionReplacedSpecificationItem_ = null;
			}

		if( obsoleteAssumptionSpecificationItem != null )
			{
			if( !obsoleteAssumptionSpecificationItem.isConcludedAssumption() &&
			createdSpecificationItem.attachJustificationToSpecificationAdvanced( obsoleteAssumptionSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the first justification of the found assumption specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteAssumptionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete assumption specification" );
			}

		if( obsoleteHiddenSpanishSpecificationItem != null )
			{
			if( createdSpecificationItem.attachJustificationToSpecificationAdvanced( obsoleteHiddenSpanishSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found replaced hidden specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteHiddenSpanishSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete hidden Spanish specification" );
			}

		if( obsoleteSpecificationItem != null &&
		replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete specification" );

		return Constants.RESULT_OK;
		}

	private byte writeMoreSpecificSpecification( SpecificationItem olderSpecificationItem )
		{
		StringBuffer writtenSentenceStringBuffer;

		if( olderSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given older specification item is undefined" );

		if( !olderSpecificationItem.isOlderItem() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given older specification item isn't old" );

		if( myWordItem_.writeSelectedSpecification( false, !olderSpecificationItem.isExclusiveSpecification(), olderSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the given older specification" );

		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given older specification" );

		if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( olderSpecificationItem.isQuestion() ? ( olderSpecificationItem.isSelfGenerated() ? Constants.INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : Constants.INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem.isSelfGenerated() ? ( olderSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : Constants.INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface listing text" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationInConflictWithDefinition( boolean isQuestion, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem definitionSpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return myWordItem_.startSystemErrorInWord( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( definitionSpecificationItem = currentGeneralizationWordItem.firstExclusiveSpecificationItem( specificationWordItem ) ) != null &&

				( !isQuestion ||
				definitionSpecificationItem.hasNonCompoundSpecificationCollection() ) &&

				// Write conflicting specification
				definitionSpecificationItem.writeSpecificationConflict( isQuestion ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an exclusive conflicting specification" );
				}
			while( isQuestion &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private RelatedResultType findRelatedSpecification( boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		boolean isFirstRelatedSpecification = true;
		boolean isLastRelatedSpecification = false;
		int currentSpecificationSentenceNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		RelatedResultType relatedResult = new RelatedResultType();

		if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
			return myWordItem_.startRelatedResultErrorInWord( 1, moduleNameString_, "The given specification collection number is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startRelatedResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) ) != null )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem.relatedSpecificationWordItem( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr ) ) != null )
					{
					currentSpecificationSentenceNr = currentSpecificationItem.creationSentenceNr();

					if( currentSpecificationWordItem == specificationWordItem &&
					currentSpecificationItem.relationContextNr() == relationContextNr )
						{
						// Found the given specification item
						isFirstRelatedSpecification = false;

						if( currentSpecificationSentenceNr >= highestSentenceNr )
							{
							isLastRelatedSpecification = true;
							highestSentenceNr = currentSpecificationSentenceNr;
							}
						}
					else
						{
						if( currentSpecificationSentenceNr >= highestSentenceNr )
							{
							isLastRelatedSpecification = false;
							highestSentenceNr = currentSpecificationSentenceNr;
							}

						relatedResult.isFirstRelatedSpecification = isFirstRelatedSpecification;
						relatedResult.relatedSpecificationItem = currentSpecificationItem;
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );

			relatedResult.isLastRelatedSpecification = isLastRelatedSpecification;
			}

		return relatedResult;
		}

	private SpecificationResultType checkSpecificationWordForConflict( boolean isArchivedAssignment, boolean isGeneralizationProperNoun, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		boolean hasFoundCompoundConflict = false;
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isSpecificationWordSpanishAmbiguous;
		int compoundSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int nonCompoundSpecificationCollectionNr;
		SpecificationItem anotherConflictingSpecificationItem;
		SpecificationItem conflictingSpecificationItem = null;
		SpecificationItem foundSpecificationItem;
		SpecificationItem pastTenseAssignmentItem = null;
		SpecificationItem relatedSpecificationItem = null;
		SpecificationItem tempConflictingSpecificationItem;
		RelatedResultType relatedResult;
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( specificationWordItem == null )
			return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// Derive conflict
		if( hasSpecificationCollection )
			{
			if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

			relatedSpecificationItem = relatedResult.relatedSpecificationItem;
			}

		if( relatedSpecificationItem == null )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			// Check for looping
			if( isSpecificationWordSpanishAmbiguous ||
			( conflictingSpecificationItem = specificationWordItem.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, myWordItem_ ) ) == null )
				{
				// Check for past tense
				foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

				if( foundSpecificationItem != null &&
				foundSpecificationItem.isArchivedAssignment() != isArchivedAssignment )
					conflictingSpecificationItem = foundSpecificationItem;
				else
					{
					if( !isSpecificationWordSpanishAmbiguous &&
					( compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr() ) > Constants.NO_COLLECTION_NR )
						{
						// Check for negative
						foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, true, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

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
							if( ( anotherConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, null ) ) != null &&
							// Write conflict
							anotherConflictingSpecificationItem.writeSpecificationConflict( false ) != Constants.RESULT_OK )
								return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write another conflicting specification" );

							conflictingSpecificationItem = foundSpecificationItem;
							}
						else
							{
							// Check for existing compound specification
							// No self-generated question available for this specification
							if( ( nonCompoundSpecificationCollectionNr = nonCompoundCollectionNrInCollectionWords( compoundSpecificationCollectionNr ) ) > Constants.NO_COLLECTION_NR &&
							( tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, null ) ) != null &&
							tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem )
								conflictingSpecificationItem = tempConflictingSpecificationItem;
							}
						}
					}
				}
			}
		else
			{
			// Find for possible past tense assignment of relatedSpecificationItem
			pastTenseAssignmentItem = myWordItem_.firstNonQuestionAssignmentItem( false, false, true, isNegative, isPossessive, relatedSpecificationItem.specificationWordItem() );
			conflictingSpecificationItem = ( pastTenseAssignmentItem == null ? relatedSpecificationItem : pastTenseAssignmentItem );
			}

		if( !hasFoundCompoundConflict &&
		conflictingSpecificationItem != null &&
		!conflictingSpecificationItem.hasSpecificationBeenWrittenAsConflict() )
			{
			if( conflictingSpecificationItem.isOlderItem() )
				{
				if( ( !hasSpecificationCollection ||
				isGeneralizationProperNoun ) &&

				// Write conflicting specification
				conflictingSpecificationItem.writeSpecificationConflict( false ) != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the conflicting specification" );
				}
			else
				{
				// Sentence can only be in conflict with itself if it has at least 2 specification words
				if( GlobalVariables.nUserSpecificationWords > 1 &&
				!conflictingSpecificationItem.hasNonExclusiveSpecificationCollection() &&
				conflictingSpecificationItem.isUserSpecification() &&

				// Current sentence in conflict with definition
				writeSpecificationInConflictWithDefinition( false, specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write that the current sentence is in conflict with a definition" );
				}
			}

		specificationResult.specificationItem = conflictingSpecificationItem;
		return specificationResult;
		}

	private UserSpecificationResultType checkUserSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationProperNoun, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isSpecificationWordSpanishAmbiguous, boolean isValueSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundRelationContext;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean isFoundSpecificationExclusive;
		int foundRelationContextNr;
		JustificationItem nextJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem predecessorObsoleteJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem oldUserSpecificationItem = null;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem userSpecificationItem;
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		if( foundSpecificationItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given found specification item is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( foundSpecificationItem.isValueSpecification() != isValueSpecification )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "Value specification conflict! Specification word \"" + specificationWordItem.anyWordTypeString() + "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );

		isFoundSpecificationExclusive = foundSpecificationItem.isExclusiveSpecification();

		if( !hasRelationContext &&
		!isAssignment &&
		!isExclusiveSpecification &&
		isFoundSpecificationExclusive )
			{
			if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );

			hasCurrentlyMoreSpecificNonExclusiveSpecification_ = true;
			userSpecificationResult.isNonExclusiveSpecification = true;

			if( ( relatedResult = findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find the related part of a more specific specification" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
			( obsoleteJustificationItem = myWordItem_.primarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, relatedSpecificationItem ) ) != null &&
			( secondarySpecificationItem = obsoleteJustificationItem.secondarySpecificationItem() ) != null &&
			( assumptionSpecificationItem = myWordItem_.firstAssignmentOrSpecificationItem( true, true, false, false, false, secondarySpecificationItem.specificationWordItem() ) ) != null &&
			( predecessorObsoleteJustificationItem = assumptionSpecificationItem.firstJustificationItem() ) != null )
				{
				while( ( nextJustificationItem = predecessorObsoleteJustificationItem.attachedJustificationItem() ) != null &&
				nextJustificationItem != obsoleteJustificationItem )
					predecessorObsoleteJustificationItem = nextJustificationItem;

				if( ( justificationResult = myWordItem_.copyJustification( true, predecessorObsoleteJustificationItem.primarySpecificationItem(), predecessorObsoleteJustificationItem.secondarySpecificationItem(), null, predecessorObsoleteJustificationItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to copy the predecessor obsolete justification item" );

				if( justificationResult.createdJustificationItem == null )
					return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The created justification item is undefined" );

				if( myWordItem_.replaceJustification( predecessorObsoleteJustificationItem, justificationResult.createdJustificationItem, assumptionSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete justification item" );

				if( myWordItem_.replaceJustification( obsoleteJustificationItem, null, assumptionSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete justification item" );
				}
			}
		else
			{
			hasFoundRelationContext = foundSpecificationItem.hasRelationContext();

			if( !isSelection &&
			hasRelationContext &&
			foundSpecificationItem.isOlderItem() )
				{
				if( foundSpecificationItem.isArchivedAssignment() == isArchivedAssignment &&
				// Check if negative specification exists
				myWordItem_.firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, specificationWordItem ) == null )
					{
					if( hasFoundRelationContext )
						{
						if( isPossessive &&
						relationWordItem != null &&
						specificationWordItem.isFemale() != relationWordItem.isFemale() &&
						specificationWordItem.isMale() != relationWordItem.isMale() &&

						// Check for specification conflict
						checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
						}

					if( !isInactiveAssignment &&

					( !hasFoundRelationContext ||

					( !foundSpecificationItem.isExclusiveGeneralization() &&
					// An self-generated assumption was found,
					// but there is also a user specification without relation word
					( oldUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != null &&
					!oldUserSpecificationItem.hasRelationContext() ) ) )
						{
						if( writeMoreSpecificSpecification( oldUserSpecificationItem == null ? foundSpecificationItem : oldUserSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific related specification" );

						hasCurrentlyMoreSpecificSpecification_ = true;
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			if( !GlobalVariables.hasDisplayedWarning )
				{
				foundRelationContextNr = foundSpecificationItem.relationContextNr();

				if( hasFoundRelationContext &&
				foundRelationContextNr != relationContextNr &&

				( !hasRelationContext ||
				// Current user specification has less relation words than the found specification
				GlobalVariables.nUserRelationWords < myWordItem_.nContextWords( foundRelationContextNr, specificationWordItem ) ) )
					{
					if( !foundSpecificationItem.isHiddenSpanishSpecification() &&

					// Has new relation context
					( hasRelationContext ||
					myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, relationContextNr, specificationWordItem ) == null ) )
						{
						if( !hasRelationContext )
							{
							hasCurrentlyConfirmedSpecificationButNoRelation_ = true;
							userSpecificationResult.isConfirmedSpecificationButNotItsRelation = true;
							}
						else
							{
							hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = true;

							if( writeConfirmedSpecification( ( foundSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION ), foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the confirmed specification" );
							}
						}
					}
				else
					{
					if( hasRelationContext )
						{
						if( !hasFoundRelationContext ||

						( foundRelationContextNr == relationContextNr &&
						!foundSpecificationItem.isOlderItem() ) )
							// Confirmed relation words
							foundSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( hasFoundRelationContext, false, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, foundRelationContextNr, specificationWordItem );
						}

					if( foundSpecificationItem == null )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						hasCurrentlyConfirmedSpecification_ &&
						( foundSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, true, specificationWordItem, relationWordItem ) ) != null &&
						( userSpecificationItem = myWordItem_.firstSpecificationItem( false, isPossessive, false, specificationWordItem ) ) != null )
							{
							// Write confirmed specification
							if( writeConfirmedSpecification( Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED, foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write another confirmed Spanish specification" );

							if( replaceOrDeleteSpecification( foundSpecificationItem, userSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace or delete another confirmed Spanish specification" );
							}
						}
					else
						{
						// Confirmation: Replace a self-generated by a user-entered specification
						if( ( hasRelationContext ||
						!isFoundSpecificationExclusive ) &&

						foundSpecificationItem.isSelfGenerated() )
							{
							hasCurrentlyConfirmedSpecification_ = true;

							if( isFoundSpecificationExclusive )
								userSpecificationResult.isConfirmedExclusive = true;

							if( !foundSpecificationItem.isHiddenSpanishSpecification() &&
							// Write confirmed specification
							writeConfirmedSpecification( ( foundSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : Constants.INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ), foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the confirmed specification" );

							userSpecificationResult.confirmedSpecificationItem = foundSpecificationItem;
							}

						if( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||

						( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						foundSpecificationItem.isSpecificationSingularNoun() ) )
							{
							if( processPreviouslyUnknownPluralNoun( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ? myWordItem_ : specificationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to process a previously unknown plural noun" );

							userSpecificationResult.confirmedSpecificationItem = foundSpecificationItem;
							}
						}
					}
				}
			}

		return userSpecificationResult;
		}

	private UserSpecificationResultType checkUserSpecificationOrQuestion( boolean hasRelationContext, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationProperNoun, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isSpecificationWordSpanishAmbiguous, boolean isValueSpecification, short assumptionLevel, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFeminineSingularNounEnding;
		boolean isCheckingForLooping = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSingularNounGeneralizationWord = ( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR );
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordTypeItem foundWordTypeItem;
		WordTypeItem singularNounWordTypeItem;
		StringBuffer writtenSentenceStringBuffer;
		BoolResultType boolResult;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		hasCurrentlyCorrectedAssumptionByOppositeQuestion_ = false;

		if( specificationWordItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// Skip if definition (singular noun) is Spanish ambiguous
		if( isSingularNounGeneralizationWord &&
		specificationWordItem != myWordItem_ )
			{
			// Check specification in opposite direction
			if( ( boolResult = myWordItem_.findGeneralization( false, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find out if word \"" + specificationWordItem.anyWordTypeString() + "\" is one of my generalization words" );

			isCheckingForLooping = boolResult.booleanValue;
			}

		// Has found generalization
		// User specification is looping
		if( isCheckingForLooping )
			{
			if( checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification looping conflict" );
			}
		else
			{
			// Check current assignment, specification or question (with relation)
			if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( true, true, true, true, true, isNegative, isPossessive, isQuestion, specificationCollectionNr, ( isQuestion ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) != null )
				{
				if( !isQuestion &&
				isArchivedAssignment != foundSpecificationItem.isArchivedAssignment() &&
				// Check for specification conflict
				checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
				}

			if( isQuestion &&
			foundSpecificationItem == null )
				foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

			if( foundSpecificationItem == null )
				{
				// Check current assignment or specification (without this relation)
				if( isQuestion )
					foundSpecificationItem = myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );
				else
					foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem );

				if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN )
					{
					// Exclusive specifications are not conflicting
					if( !isExclusiveSpecification &&
					// Possessive specifications are not conflicting
					!isPossessive &&
					foundSpecificationItem == null )
						{
						if( !myWordItem_.isFemaleOrMale() &&
						myWordItem_.hasFeminineProperNounEnding() != specificationWordItem.isFemale() &&
						myWordItem_.hasMasculineProperNounEnding() != specificationWordItem.isMale() &&
						InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NOUN_TO_BE, myWordItem_.anyWordTypeString(), ( myWordItem_.hasFeminineProperNounEnding() ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a proper noun expected to be feminine or masculine" );

						if( specificationCollectionNr > Constants.NO_COLLECTION_NR )
							{
							if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

							relatedSpecificationItem = relatedResult.relatedSpecificationItem;

							if( hasRelationContext &&
							relatedSpecificationItem != null &&
							relatedSpecificationItem.isSelfGeneratedAssumption() &&
							specificationWordItem.isExclusiveCollection( specificationCollectionNr ) )
								{
								// Assumption needs to be corrected
								if( correctedAssumptionReplacedSpecificationItem_ != null )
									return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The corrected assumption specification item already assigned" );

								if( myWordItem_.writeUpdatedSpecification( false, true, false, false, false, relatedSpecificationItem ) != Constants.RESULT_OK )
									return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );

								hasCurrentlyCorrectedAssumptionByKnowledge_ = true;
								correctedAssumptionReplacedSpecificationItem_ = relatedSpecificationItem;
								}
							else
								{
								if( !isQuestion &&

								( relatedSpecificationItem == null ||

								( ( !relatedSpecificationItem.isOlderItem() ||
								specificationWordItem.isExclusiveCollection( specificationCollectionNr ) ) &&

								( !isSpecificationWordSpanishAmbiguous ||
								!relatedSpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ) &&

								// Skip when question with the current specification word exists
								myWordItem_.firstSpecificationItem( false, false, true, specificationWordItem ) == null &&

								// Check for specification conflict
								checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
									return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
								}
							}
						}

					if( !GlobalVariables.hasDisplayedWarning &&
					!hasRelationContext &&
					isAssignment &&
					!isArchivedAssignment &&
					!isValueSpecification &&
					specificationContextNr == Constants.NO_CONTEXT_NR &&
					// Write warning
					InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MISSING_RELATION ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface warning about ambiguity" );
					}
				else
					{
					if( isSingularNounGeneralizationWord &&
					!myWordItem_.isFemaleOrMale() &&
					( foundWordTypeItem = myWordItem_.activeWordTypeItem( Constants.WORD_TYPE_NOUN_SINGULAR ) ) != null &&
					foundWordTypeItem.hasFeminineOrMasculineWordEnding() )
						{
						hasFeminineSingularNounEnding = foundWordTypeItem.hasFeminineWordEnding();

						if( !myWordItem_.isCorrectIndefiniteArticle( ( hasFeminineSingularNounEnding ? Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : Constants.WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ), generalizationWordTypeNr ) )
							{
							foundWordTypeItem.clearIndefiniteArticleParameter();

							if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_SINGULAR_NOUN_TO_BE, myWordItem_.anyWordTypeString(), ( hasFeminineSingularNounEnding ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a singular noun expected to be feminine or masculine" );
							}
						}
					}
				}
			else
				{
				if( !hasRelationContext &&
				isAssignment &&
				generalizationContextNr == Constants.NO_CONTEXT_NR &&
				foundSpecificationItem.hasRelationContext() )
					{
					if( myWordItem_.writeSelectedSpecification( false, true, foundSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
					writtenSentenceStringBuffer.length() == 0 )
						return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "I couldn't write a sentence with an assumption about the relation" );

					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a missing relation" );

					if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					// Force 'I know' notification
					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the 'I know' interface notification" );
					}
				}

			if( !GlobalVariables.hasDisplayedWarning )
				{
				if( isQuestion )
					{
					if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check the user question" );

					// Find self-generated question
					if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationContextNr ) ).result != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting self-generated question" );

					if( !isOnlyCheckingForConflicts_ &&
					GlobalVariables.nUserSpecificationWords > 1 &&
					myWordItem_.sameUserQuestionSpecificationItem( questionParameter ) != null )
						{
						if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write 'I had the same question before' interface notification" );

						userSpecificationResult.isSameQuestionFromUser = true;
						}
					else
						{
						// Find user question
						if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationContextNr ) ).result != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting user question" );
						}

					isOnlyCheckingForConflicts_ = true;
					}
				else
					{
					if( foundSpecificationItem == null )
						{
						if( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						// Find singular word type
						( singularNounWordTypeItem = specificationWordItem.activeWordTypeItem( Constants.WORD_TYPE_NOUN_SINGULAR ) ) != null &&
						// Skip currently created singular noun
						singularNounWordTypeItem.isOlderItem() &&

						processPreviouslyUnknownPluralNoun( specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to process a previously unknown plural noun" );
						}
					else
						{
						if( ( userSpecificationResult = checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperNoun, isNegative, isPossessive, isSelection, isSpecificationWordSpanishAmbiguous, isValueSpecification, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, relationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check the user specification" );
						}
					}
				}
			}

		return userSpecificationResult;
		}

	private UserSpecificationResultType findSameSimilarOrRelatedQuestion( boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isOnlyCheckingForConflicts, boolean isPossessive, boolean isSelfGenerated, boolean isSpecificationGeneralization, short assumptionLevel, short questionParameter, int specificationCollectionNr, int relationContextNr )
		{
		boolean hasFoundOlderSpecification = false;
		boolean hasFoundQuestion;
		boolean hasFoundSpecificationRelationContext = false;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isConflictingQuestion = false;
		boolean isRelatedQuestion = false;
		boolean isSameAssignment = false;
		boolean isSameExclusiveSpecification = false;
		boolean isSameRelationContext = false;
		boolean isSameQuestion = true;
		boolean isSimilarQuestion = false;
		boolean isUserSpecificationWord;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem foundQuestionSpecificationItem = null;
		WordItem currentWordItem;
		StringBuffer writtenSentenceStringBuffer;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		if( ( currentWordItem = GlobalVariables.firstWordItem ) == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The first word item is undefined" );

		// Do for all words, including predefined words
		do	{
			hasFoundQuestion = false;
			isUserSpecificationWord = currentWordItem.isUserSpecificationWord;

			if( ( currentQuestionSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, currentWordItem ) ) != null )
				{
				hasFoundQuestion = true;

				if( foundQuestionSpecificationItem == null ||
				foundQuestionSpecificationItem.isExclusiveSpecification() != currentQuestionSpecificationItem.isExclusiveSpecification() )
					{
					foundQuestionSpecificationItem = currentQuestionSpecificationItem;

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
				isUserSpecificationWord &&
				foundQuestionSpecificationItem != null )
					{
					isSimilarQuestion = true;

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
						if( myWordItem_.isContextSubsetInContextWords( relationContextNr, foundQuestionSpecificationItem.relationContextNr() ) )
							isConflictingQuestion = true;
						else
							{
							isRelatedQuestion = true;
							isSameQuestion = false;
							}
						}
					}
				else
					{
					if( isUserSpecificationWord ||

					// Found question has specification words that aren't in the user question
					( hasSpecificationCollection &&
					isExclusiveSpecification ) )
						isSameQuestion = false;

					if( isSpecificationGeneralization ||

					// Prefer similar over related, if both occur
					( foundQuestionSpecificationItem != null &&

					( !hasFoundSpecificationRelationContext ||
					!isSameRelationContext ) ) )
						isSimilarQuestion = true;
					}
				}
			}
		while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );

		if( foundQuestionSpecificationItem != null )
			{
			if( !isConflictingQuestion &&
			!isOnlyCheckingForConflicts &&

			( isExclusiveSpecification ||
			isSameExclusiveSpecification ) )
				{
				if( isSameQuestion )
					{
					if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? Constants.INTERFACE_QUESTION_I_HAD_THE_SAME_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about the same question" );
					}
				else
					{
					if( myWordItem_.writeSelectedSpecification( false, false, foundQuestionSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the found specification" );

					if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
					writtenSentenceStringBuffer.length() == 0 )
						return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "I couldn't write the found specification" );

					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE ) : ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE ) ) ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a related question" );

					if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

					userSpecificationResult.isSimilarOrRelatedQuestion = true;
					}
				}
			else
				{
				if( hasFoundOlderSpecification &&
				!isOnlyCheckingForConflicts )
					{
					hasCurrentlyMoreSpecificQuestion_ = true;

					// Skip displaying announcement if question is in conflict with itself
					if( GlobalVariables.nUserSpecificationWords == 1 &&
					// Write more specific specification
					writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a more specific related question" );
					}
				}
			}

		return userSpecificationResult;
		}


	// Constructor

	protected WordSpecification( WordItem myWordItem )
		{
		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given my word item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeWordSpecificationVariables()
		{
		hasCurrentlyConfirmedSpecification_ = false;
		hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = false;
		hasCurrentlyConfirmedSpecificationButNoRelation_ = false;
		hasCurrentlyCorrectedAssumptionByKnowledge_ = false;
		hasCurrentlyMoreSpecificSpecification_ = false;
		hasCurrentlyMoreSpecificNonExclusiveSpecification_ = false;
		hasCurrentlyMoreSpecificQuestion_ = false;
		isOnlyCheckingForConflicts_ = false;
		isWordTouchedDuringCurrentSentence_ = false;

		spanishCompoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		spanishCompoundSpecificationWordItem_ = null;
		}

	protected boolean hasCurrentlyConfirmedSpecification()
		{
		return hasCurrentlyConfirmedSpecification_;
		}

	protected boolean hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation()
		{
		return hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_;
		}

	protected boolean hasCurrentlyConfirmedSpecificationButNoRelation()
		{
		return hasCurrentlyConfirmedSpecificationButNoRelation_;
		}

	protected boolean hasCurrentlyCorrectedAssumption()
		{
		return ( hasCurrentlyCorrectedAssumptionByKnowledge_ ||
				hasCurrentlyCorrectedAssumptionByOppositeQuestion_ );
		}

	protected boolean hasCurrentlyCorrectedAssumptionByKnowledge()
		{
		return hasCurrentlyCorrectedAssumptionByKnowledge_;
		}

	protected boolean hasCurrentlyCorrectedAssumptionByOppositeQuestion()
		{
		return hasCurrentlyCorrectedAssumptionByOppositeQuestion_;
		}

	protected boolean hasCurrentlyMoreSpecificSpecification()
		{
		return hasCurrentlyMoreSpecificSpecification_;
		}

	protected boolean hasCurrentlyMoreSpecificNonExclusiveSpecification()
		{
		return hasCurrentlyMoreSpecificNonExclusiveSpecification_;
		}

	protected boolean hasCurrentlyMoreSpecificQuestion()
		{
		return hasCurrentlyMoreSpecificQuestion_;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isGeneralizationProperNoun, boolean isNegative, boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem conflictingSpecificationItem;
		SpecificationResultType specificationResult;

		if( ( specificationResult = checkSpecificationWordForConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );

		conflictingSpecificationItem = specificationResult.specificationItem;

		if( conflictingSpecificationItem != null &&
		conflictingSpecificationItem.isOlderItem() &&

		( isNegative ||
		!conflictingSpecificationItem.isNegative() ||
		conflictingSpecificationItem.hasCompoundSpecificationCollection() ) &&

		// Opposite negative of conflicting specification word
		checkSpecificationWordForConflict( isArchivedAssignment, isGeneralizationProperNoun, !isNegative, isPossessive, specificationCollectionNr, relationContextNr, conflictingSpecificationItem.specificationWordItem() ).result != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for the opposite negative specification conflict" );

		return Constants.RESULT_OK;
		}

	protected byte writeConfirmedSpecification( short interfaceParameter, SpecificationItem writeSpecificationItem )
		{
		StringBuffer writtenSentenceStringBuffer;

		if( interfaceParameter <= Constants.NO_INTERFACE_PARAMETER )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given interface parameter is undefined" );

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( ( writtenSentenceStringBuffer = writeSpecificationItem.storedSentenceStringBuffer() ) == null )
			{
			if( myWordItem_.writeSelectedSpecification( false, true, writeSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

			writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer;
			}

		if( writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given write specification" );

		if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a confirmation" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

		return Constants.RESULT_OK;
		}

	protected CreateAndAssignResultType addSpecificationInWord( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		boolean hasCopiedRelationContext = ( copiedRelationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasExistingSpecificationRelationContext;
		boolean hasFoundSpecificationWithDifferentRelationContext = false;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean hasSpecificationCollection;
		boolean isConfirmedExclusive = false;
		boolean isConfirmedSpecificationButNotItsRelation = false;
		boolean isExistingHiddenSpanishSpecification;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isNonExclusiveSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSameQuestionFromUser = false;
		boolean isSelfGenerated = ( firstJustificationItem != null );
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUserRelationWord = myWordItem_.isUserRelationWord;
		int compoundSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int existingRelationContextNr;
		int nonCompoundSpecificationCollectionNr;
		int originalSentenceNr;
		JustificationItem duplicateJustificationItem;
		JustificationItem foundJustificationItem;
		SpecificationItem confirmedSpecificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem foundUserSpecificationItem;
		SpecificationItem noRelationContextSpecificationItem;
		SpecificationItem obsoleteAssumptionSpecificationItem = null;
		SpecificationItem obsoleteHiddenSpanishSpecificationItem = null;
		SpecificationItem obsoleteSpecificationItem = null;
		SpecificationItem tempSpecificationItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		UserSpecificationResultType userSpecificationResult;

		correctedAssumptionReplacedSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		// Check generalization word type
		if( !myWordItem_.hasWordType( true, generalizationWordTypeNr ) )
			return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		// Check relation variables:
		// 1) If relation context is defined,
		if( ( hasRelationContext ||
		// 2) or relation word type is defined
		relationWordTypeNr != Constants.NO_WORD_TYPE_NR ||
		// 3) or relation word item is defined
		relationWordItem != null ) &&

		( relationWordItem == null ||
		!relationWordItem.hasWordType( false, relationWordTypeNr ) ) )
			return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The relation variables aren't matching" );

		addMyWordToTouchedDuringCurrentSentenceList();

		if( specificationWordItem != null )
			{
			// Check specification word type
			if( !specificationWordItem.hasWordType( true, specificationWordTypeNr ) &&
			// New created language
			!myWordItem_.isLanguageWord() )
				return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification word type" );

			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			// Collect definition specification without specification collection
			if( !isExclusiveSpecification &&
			!isQuestion &&
			generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
			specificationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
			!specificationWordItem.isOlderItem() &&
			!specificationWordItem.hasCollection() &&

			// Collect specifications
			collectSpecifications( isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect myself with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

			if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
				{
				if( !isSelfGenerated &&
				userSpecificationCollectionNr_ > Constants.NO_COLLECTION_NR )
					// Use previous user specification collection
					specificationCollectionNr = userSpecificationCollectionNr_;
				else
					{
					compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr();
					nonCompoundSpecificationCollectionNr = specificationWordItem.nonCompoundCollectionNr();

					if( nonCompoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

					( ( compoundSpecificationCollectionNr == Constants.NO_COLLECTION_NR &&
					// Skip definition sentence with the same generalization and specification word
					!specificationWordItem.isUserGeneralizationWord ) ||

					( !isQuestion &&

					// Non-exclusive specification collection
					( !isExclusiveSpecification &&

					( !isSpecificationWordSpanishAmbiguous ||

					( hasRelationContext &&

					( nContextRelations > 1 ||
					!specificationWordItem.isUserSpecificationWord ) ) ) ) ) ) )
						specificationCollectionNr = nonCompoundSpecificationCollectionNr;
					else
						specificationCollectionNr = compoundSpecificationCollectionNr;
					}
				}
			else
				{
				if( !specificationWordItem.hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification collection" );
				}

			hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );

			if( !isSelfGenerated &&
			myWordItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				if( hasSpecificationCollection &&

				( !isAssignment ||
				isGeneralizationProperNoun ) )
					userSpecificationCollectionNr_ = specificationCollectionNr;

				if( ( userSpecificationResult = checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperNoun, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isSpecificationWordSpanishAmbiguous, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to check a user specification or question with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

				isConfirmedExclusive = userSpecificationResult.isConfirmedExclusive;
				isConfirmedSpecificationButNotItsRelation = userSpecificationResult.isConfirmedSpecificationButNotItsRelation;
				isNonExclusiveSpecification = userSpecificationResult.isNonExclusiveSpecification;
				isSameQuestionFromUser = userSpecificationResult.isSameQuestionFromUser;
				confirmedSpecificationItem = userSpecificationResult.confirmedSpecificationItem;
				}

			if( !GlobalVariables.hasDisplayedWarning )
				{
				// Try to find specification
				// Start with finding user specification
				if( ( foundUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == null )
					{
					// Self-generated
					if( firstJustificationItem == null )
						// Obsolete user specification without specification
						obsoleteSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, isNegative, isPossessive, false, questionParameter, Constants.NO_COLLECTION_NR, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem );
					else
						{
						if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationCollectionNr, ( isAssignment && !isArchivedAssignment ? Constants.NO_CONTEXT_NR : ( hasCopiedRelationContext ? copiedRelationContextNr : relationContextNr ) ), specificationWordItem ) ) == null &&
						// Try to find alternative
						!isArchivedAssignment &&
						( tempSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationCollectionNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
						tempSpecificationItem.isSelfGeneratedConclusion() )
							foundSpecificationItem = tempSpecificationItem;

						if( foundSpecificationItem == null )
							{
							// Not found
							if( !isQuestion &&
							relationWordItem != null &&
							( existingSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isNegative, isPossessive, specificationWordItem ) ) != null )
								{
								if( !hasCopiedRelationContext &&
								compoundSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
								// Existing specification has no specification collection
								!existingSpecificationItem.hasSpecificationCollection() )
									// Obsolete not-collected specification
									obsoleteSpecificationItem = existingSpecificationItem;
								else
									{
									hasExistingSpecificationRelationContext = existingSpecificationItem.hasRelationContext();
									isExistingHiddenSpanishSpecification = existingSpecificationItem.isHiddenSpanishSpecification();
									existingRelationContextNr = existingSpecificationItem.relationContextNr();

									if( isSpecificationWordSpanishAmbiguous &&
									hasExistingSpecificationRelationContext )
										{
										if( isPossessive )
											{
											if( !isExistingHiddenSpanishSpecification &&
											existingRelationContextNr == relationContextNr )
												{
												if( isUserRelationWord &&
												firstJustificationItem.isAssumptionJustification() )
													{
													// Forcing lower assumption level
													if( assumptionLevel > Constants.NO_ASSUMPTION_LEVEL )
														assumptionLevel--;

													// Older specification need to be copied
													obsoleteSpecificationItem = existingSpecificationItem;

													if( firstJustificationItem.changeAttachedJustification( existingSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
														return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to write an assumption, which has relation words now" );
													}
												else
													foundSpecificationItem = existingSpecificationItem;
												}
											}
										else
											{
											if( existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
											existingSpecificationItem.isSelfGeneratedConclusion() )
												obsoleteHiddenSpanishSpecificationItem = existingSpecificationItem;
											}
										}
									else
										{
										if( existingSpecificationItem.isUserSpecification() )
											{
											if( isUserRelationWord &&
											isSpecificationWordSpanishAmbiguous )
												// More specific specification
												foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, specificationWordItem, relationWordItem );
											}
										else
											{
											if( existingRelationContextNr == relationContextNr )
												foundSpecificationItem = existingSpecificationItem;
											else
												{
												if( !isSpecificationWordSpanishAmbiguous &&
												( noRelationContextSpecificationItem = myWordItem_.noRelationContextSpecificationItem( isPossessive, true, specificationWordItem ) ) != null )
													{
													if( confirmedSpecificationItem != null )
														return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The confirmed specification item is already defined" );

													if( myWordItem_.writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, false, false, hasRelationContext, false, noRelationContextSpecificationItem ) != Constants.RESULT_OK )
														return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to write an assumption, which has relation words now" );

													confirmedSpecificationItem = noRelationContextSpecificationItem;
													}
												}
											}
										}
									}
								}
							}
						else
							{
							if( !isSpecificationWordSpanishAmbiguous &&
							foundSpecificationItem.isConcludedAssumption() &&
							firstJustificationItem.isExclusiveSpecificationSubstitutionAssumption() &&

							( foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) != null ||
							!foundSpecificationItem.hasOnlyOneRelationWord() ) )
								{
								// Concluded assumption becomes self-generated assumption
								if( firstJustificationItem.changeAttachedJustification( foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to change the attached justification item of the given first justification item" );

								obsoleteSpecificationItem = foundSpecificationItem;
								foundSpecificationItem = null;
								}
							else
								{
								// Found specification has same relation context
								if( foundSpecificationItem.relationContextNr() == relationContextNr )
									{
									if( isGeneralizationProperNoun &&
									foundSpecificationItem.isOlderItem() )
										{
										if( firstJustificationItem.isConclusionJustification() )
											{
											if( foundSpecificationItem.isSelfGeneratedAssumption() &&

											( relationWordItem == null ||

											( isUserRelationWord &&
											!isSpecificationWordSpanishAmbiguous &&
											relationWordItem.isOlderItem() &&
											foundSpecificationItem.hasOnlyOneRelationWord() &&
											// Conclusion doesn't exist
											myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, null ) == null ) ) )
												{
												if( foundSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
													return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

												if( ( isUserRelationWord ||
												foundSpecificationItem.isPartOf() ||
												!myWordItem_.hasCurrentlyAnsweredSelfGeneratedQuestion() ) &&

												// Write concluded assumption
												myWordItem_.writeUpdatedSpecification( true, false, false, false, foundSpecificationItem.wasHiddenSpanishSpecification(), foundSpecificationItem ) != Constants.RESULT_OK )
													return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to write a concluded assumption" );
												}
											}
										else
											{
											if( hasRelationContext &&
											isPossessive &&
											!isUserRelationWord &&
											!isSpecificationWordSpanishAmbiguous &&
											!foundSpecificationItem.isConcludedAssumption() &&
											( foundJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != null &&
											( duplicateJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) ) != null &&
											foundJustificationItem.secondarySpecificationItem() == duplicateJustificationItem.secondarySpecificationItem() &&

											// Remove an almost duplicate justification. Different justification type
											myWordItem_.replaceJustification( duplicateJustificationItem, foundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
												return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to remove an almost duplicate justification in my word" );
											}
										}
									}
								else	// Different relation context
									{
									if( hasRelationContext &&
									foundSpecificationItem.hasRelationContext() )
										{
										hasFoundSpecificationWithDifferentRelationContext = true;

										if( !isSpecificationWordSpanishAmbiguous &&
										firstJustificationItem.isReversibleConclusion() &&
										foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) == null )
											obsoleteAssumptionSpecificationItem = foundSpecificationItem;
										}
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

							// Typical for Spanish
							if( isSpecificationWordSpanishAmbiguous &&
							hasRelationContext &&
							isQuestion &&
							foundUserSpecificationItem.hasRelationContext() &&
							// Question with different relation context
							foundUserSpecificationItem.relationContextNr() != relationContextNr )
								// Don't create question
								foundSpecificationItem = null;
							else
								{
								if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
								relationWordItem != null &&
								foundUserSpecificationItem.hasRelationContext() &&
								!foundUserSpecificationItem.hasSpecificationCollection() )
									confirmedSpecificationItem = foundUserSpecificationItem;
								}
							}
						else	// Typical for compound collections in Spanish
							{
							if( spanishCompoundSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR )
								{
								if( ( specificationCollectionNr = myWordItem_.highestCollectionNrInCollectionWords() ) >= Constants.MAX_COLLECTION_NR )
									return myWordItem_.startCreateAndAssignResultSystemErrorInWord( 1, moduleNameString_, "Collection number overflow" );

								spanishCompoundSpecificationCollectionNr_ = ++specificationCollectionNr;
								spanishCompoundSpecificationWordItem_ = specificationWordItem;
								}
							else
								{
								if( spanishCompoundSpecificationWordItem_ != null )
									{
									specificationCollectionNr = spanishCompoundSpecificationCollectionNr_;

									if( spanishCompoundSpecificationWordItem_.createCollection( isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
										return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + spanishCompoundSpecificationWordItem_.anyWordTypeString() + "\" with word \"" + specificationWordItem.anyWordTypeString() + "\"" );

									if( specificationWordItem.createCollection( isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishCompoundSpecificationWordItem_, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
										return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + specificationWordItem.anyWordTypeString() + "\" with word \"" + spanishCompoundSpecificationWordItem_.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					else
						{
						if( foundUserSpecificationItem.isSpecificationWordSpanishAmbiguous() )
							specificationCollectionNr = myWordItem_.collectionNr( myWordItem_ );
						}
					}
				}

			// Add generalization items
			if( hasRelationContext ||
			foundSpecificationItem == null )
				{
				// Specification word
				if( addGeneralization( false, generalizationWordTypeNr, specificationWordTypeNr, specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to add a specification word to the generalizations" );

				// Relation word
				if( relationWordItem != null &&
				addGeneralization( true, generalizationWordTypeNr, relationWordTypeNr, relationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to add a relation word to the generalizations" );
				}
			}

		if( !GlobalVariables.hasDisplayedWarning )
			{
			// Not found
			if( foundSpecificationItem == null ||
			// User specification has copied relation context
			hasCopiedRelationContext ||
			// Accept different relation context (e.g. ambiguous specification)
			hasFoundSpecificationWithDifferentRelationContext ||

			// Exceptions
			isNonExclusiveSpecification ||
			confirmedSpecificationItem != null ||

			// Overwrite non-exclusive specification by exclusive one
			( isExclusiveSpecification &&
			!foundSpecificationItem.isExclusiveSpecification() ) ||

			// Overwrite user questions by non-exclusive one
			( !isAssignment &&
			isQuestion &&
			!isSelfGenerated &&
			!isSameQuestionFromUser &&
			foundSpecificationItem.isExclusiveSpecification() ) ||

			// Overwrite non-conditional specification by conditional one
			( isConditional &&
			!foundSpecificationItem.isConditional() ) )
				{
				if( foundSpecificationItem != null )
					{
					if( !isExclusiveSpecification &&
					!isQuestion &&
					foundSpecificationItem.isExclusiveSpecification() )
						isExclusiveSpecification = true;

					if( !isQuestion &&
					!isSelfGenerated &&
					foundSpecificationItem != confirmedSpecificationItem )
						// Obsolete user specification
						obsoleteSpecificationItem = foundSpecificationItem;
					}

				if( isExclusiveSpecification )
					{
					if( isNonExclusiveSpecification )
						isExclusiveSpecification = false;
					}
				else
					{
					if( isConfirmedExclusive )
						isExclusiveSpecification = true;
					}

				originalSentenceNr = ( hasRelationContext ||
										isQuestion ||
										foundSpecificationItem == null ? ( obsoleteHiddenSpanishSpecificationItem == null ? GlobalVariables.currentSentenceNr : obsoleteHiddenSpanishSpecificationItem.creationSentenceNr() ) : foundSpecificationItem.originalSentenceNr() );

				// Create the actual specification
				if( ( createAndAssignResult = myWordItem_.createSpecificationItem( false, false, false, false, isCharacteristicFor, false, isConditional, ( hasCurrentlyCorrectedAssumptionByKnowledge_ || hasCurrentlyCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, Constants.NO_ASSIGNMENT_LEVEL, assumptionLevel, GlobalVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? Constants.NO_WORD_TYPE_NR : relationWordTypeNr ), specificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, null, null ) ).result != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to create a specification item" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
					return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I couldn't create a specification item" );

				// Now replace obsolete specifications by the created specification
				if( replaceObsoleteSpecification( isConfirmedSpecificationButNotItsRelation, confirmedSpecificationItem, createdSpecificationItem, obsoleteAssumptionSpecificationItem, obsoleteHiddenSpanishSpecificationItem, obsoleteSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete specification" );

				if( firstJustificationItem != null &&
				foundSpecificationItem != null &&

				( ( hasCopiedRelationContext &&

				( !foundSpecificationItem.hasSpecificationCollection() ||
				foundSpecificationItem.isSelfGeneratedAssumption() ) ) ||

				( isPossessive &&
				foundSpecificationItem.isSelfGeneratedConclusion() &&

				( hasCopiedRelationContext ||

				// Typical for Spanish
				( isSpecificationWordSpanishAmbiguous &&
				foundSpecificationItem.isConcludedAssumption() ) ) ) ) )
					{
					if( !firstJustificationItem.isOlderItem() &&
					firstJustificationItem != foundSpecificationItem.firstJustificationItem() )
						{
						if( isPossessive &&
						foundSpecificationItem.hasCurrentCreationSentenceNr() &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Typical for Spanish
						!foundSpecificationItem.isHiddenSpanishSpecification() ) )
							{
							// Change first justification
							if( createdSpecificationItem.changeFirstJustification( true, foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace the justifications of the created specification by the justifications of the found specification" );
							}
						else
							{
							// Change attached justification
							if( firstJustificationItem.changeAttachedJustification( foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
							}
						}

					if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );
					}

				if( !isNegative &&
				!isPossessive &&
				!isQuestion &&
				specificationWordItem != null &&

				( !isSpecificationWordSpanishAmbiguous ||
				!createdSpecificationItem.isHiddenSpanishSpecification() ) )
					{
					if( myWordItem_.findAnswersToQuestions( specificationWordItem.compoundCollectionNr(), createdSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to find answers to questions" );

					if( !isSelfGenerated &&

					( isGeneralizationProperNoun ||
					generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR ) &&

					!myWordItem_.isFemaleOrMale() )
						{
						if( specificationWordItem.isFemale() )
							{
							if( myWordItem_.markWordAsFemale() != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as female" );
							}
						else
							{
							if( specificationWordItem.isMale() &&
							myWordItem_.markWordAsMale() != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as male" );
							}
						}
					}
				}
			}

		createAndAssignResult.foundSpecificationItem = foundSpecificationItem;
		createAndAssignResult.replacedAssignmentItem = replacedAssignmentItem_;
		return createAndAssignResult;
		}

	protected RelatedResultType findRelatedSpecification( boolean isCheckingRelationContext, SpecificationItem searchSpecificationItem )
		{
		if( searchSpecificationItem == null )
			return myWordItem_.startRelatedResultErrorInWord( 1, moduleNameString_, "The given search specification item is undefined" );

		return findRelatedSpecification( isCheckingRelationContext, false, false, searchSpecificationItem.isAssignment(), searchSpecificationItem.isInactiveAssignment(), searchSpecificationItem.isArchivedAssignment(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPossessive(), searchSpecificationItem.questionParameter(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.specificationWordItem() );
		}

	protected RelatedResultType findRelatedSpecification( boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		RelatedResultType relatedResult = new RelatedResultType();

		if( isIncludingAssignments )
			{
			if( ( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			relatedResult.relatedSpecificationItem == null &&
			// Archived assignments
			( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification as well" );
			}

		// Specifications
		if( relatedResult.relatedSpecificationItem == null &&
		( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

		return relatedResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
