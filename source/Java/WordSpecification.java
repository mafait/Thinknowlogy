/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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
	private boolean hasConfirmedSpecificationButNoRelation_;
	private boolean hasCorrectedAssumptionByKnowledge_;
	private boolean hasCorrectedAssumptionByOppositeQuestion_;
	private boolean hasDisplayedMoreSpecificSpecification_;
	private boolean hasDisplayedMoreSpecificNonExclusiveSpecification_;
	private boolean hasDisplayedMoreSpecificQuestion_;
	private boolean isThisProperNameWordTouchedDuringCurrentSentence_;
	private boolean isOnlyCheckingForConflicts_;

	private int spanishCompoundSpecificationCollectionNr_;
	private int userSpecificationCollectionNr_;

	private SpecificationItem correctedAssumptionReplacedSpecificationItem_;
	private SpecificationItem firstConfirmedReplacedSpanishSpecificationItem_;
	private SpecificationItem replacedAssignmentItem_;

	private WordItem spanishCompoundSpecificationWordItem_;


	// Private initialized variables

	private String moduleNameString_;

	private WordItem myWordItem_;


	// Private methods

	private void addToTouchedDuringCurrentSentenceProperNameWordList()
		{
		WordItem tempProperNameWordItem;
		WordItem lastTouchedProperNameWordItem = CommonVariables.firstTouchedProperNameWordItem;

		if( lastTouchedProperNameWordItem == null )
			CommonVariables.firstTouchedProperNameWordItem = myWordItem_;
		else
			{
			// Word order is important: Add word at end of temporary touched word list
			while( ( tempProperNameWordItem = lastTouchedProperNameWordItem.nextTouchedProperNameWordItem ) != null )
				lastTouchedProperNameWordItem = tempProperNameWordItem;

			lastTouchedProperNameWordItem.nextTouchedProperNameWordItem = myWordItem_;
			}
		}

	private static int nonCompoundCollectionNrInCollectionWords( int compoundCollectionNr )
		{
		int nonCompoundCollectionNr;
		WordItem currentCollectionWordItem;

		if( compoundCollectionNr > Constants.NO_COLLECTION_NR &&
		( currentCollectionWordItem = CommonVariables.firstCollectionWordItem ) != null )
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

	private byte checkUserQuestion( boolean hasRelationContext, boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem foundQuestionSpecificationItem, WordItem specificationWordItem )
		{
		SpecificationItem relatedSpecificationItem;
		RelatedResultType relatedResult = new RelatedResultType();

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
			if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			( relatedResult = findRelatedSpecification( false, false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
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

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, true, true, relatedSpecificationItem.isNegative(), relatedSpecificationItem.isPossessive(), true, Constants.NO_QUESTION_PARAMETER, relatedSpecificationItem.specificationCollectionNr(), relatedSpecificationItem.relationContextNr(), relatedSpecificationItem.specificationWordItem() ) ) != null )
							{
							if( myWordItem_.writeUpdatedSpecification( false, false, false, true, false, correctedAssumptionReplacedSpecificationItem_ ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );

							hasCorrectedAssumptionByOppositeQuestion_ = true;
							}
						}
					}
				else
					{
					if( relatedSpecificationItem.hasSpecificationCompoundCollection() &&
					InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte collectSpecifications( boolean isExclusiveGeneralization, boolean isQuestion, short collectionWordTypeNr, short commonWordTypeNr, WordItem generalizationWordItem, WordItem collectionWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();
		int specificationCollectionNr;
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;
		WordItem foundGeneralizationWordItem;

		if( collectionWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given collection word item is undefined" );

		if( ( currentGeneralizationItem = myWordItem_.firstNounSpecificationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != collectionWordItem )
					{
					if( ( collectionResult = currentGeneralizationWordItem.collectSpecifications( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem ) ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to add a specification collection by exclusive specification in word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

					foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem;

					if( foundGeneralizationWordItem != null &&
					( specificationCollectionNr = collectionWordItem.collectionNr( collectionWordTypeNr, currentGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR &&
					// Collect by generalization
					foundGeneralizationWordItem.collectSpecificationsInWord( isExclusiveGeneralization, isQuestion, specificationCollectionNr ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to collect specifications in word \"" + foundGeneralizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( collectionResult.foundGeneralizationWordItem == null &&
			( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
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
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I found an undefined generalization word" );

						if( ( hiddenSpecificationItem = currentGeneralizationWordItem.firstUnhiddenSpecificationItem( currentContextItem.contextNr() ) ) != null &&
						currentGeneralizationWordItem.writeUpdatedSpecification( true, false, false, false, false, hiddenSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an assumption that is not hidden anymore" );
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
					}
				}
			while( ( currentContextItem = currentContextItem.nextContextItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte processPreviouslyUnknownPluralNoun( WordItem specificationWordItem )
		{
		WordItem currentLanguageWordItem;
		WordItem currentSpecificationWordItem;
		WordTypeItem pluralNounWordTypeItem;
		String unknownPluralNounString;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		pluralNounWordTypeItem = specificationWordItem.activeWordTypeItem( Constants.WORD_TYPE_NOUN_PLURAL );

		if( pluralNounWordTypeItem != null &&
		// Skip announcement for new occurrences
		!pluralNounWordTypeItem.isOlderItem() )
			{
			if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START, pluralNounWordTypeItem.itemString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a notification text" );

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
			myWordItem_.nContextWordsInContextWords( obsoleteSpecificationItem.relationContextNr(), obsoleteSpecificationItem.specificationWordItem() ) == CommonVariables.nUserRelationWords &&
			myWordItem_.replaceOrDeleteSpecification( true, firstConfirmedReplacedSpanishSpecificationItem_, obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a confirmed specification" );

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

	private byte replaceObsoleteSpecification( SpecificationItem confirmedReplacedSpecificationItem, SpecificationItem confirmedSpecificationButNoRelationReplacedSpecificationItem, SpecificationItem createdSpecificationItem, SpecificationItem foundUserSpecificationItem, SpecificationItem obsoleteAssumptionSpecificationItem, SpecificationItem obsoleteConcludedAssumptionSpecificationItem, SpecificationItem obsoleteHiddenSpanishSpecificationItem, SpecificationItem obsoleteNotCollectedSpecificationItem, SpecificationItem obsoleteUserSpecificationItem )
		{
		if( createdSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given created specification is undefined" );

		if( confirmedReplacedSpecificationItem != null &&
		!createdSpecificationItem.isHiddenSpanishSpecification() )
			{
			if( replaceOrDeleteSpecification( confirmedReplacedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a confirmed replaced specification" );

			if( foundUserSpecificationItem == null )
				{
				// Clean-up obsolete justifications
				if( myWordItem_.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete obsolete justifications" );
				}
			else
				{
				if( !foundUserSpecificationItem.isReplacedOrDeletedItem() &&
				replaceOrDeleteSpecification( foundUserSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a duplicate user specification" );
				}
			}

		if( confirmedSpecificationButNoRelationReplacedSpecificationItem != null &&
		myWordItem_.confirmSpecificationButNotItsRelation( confirmedSpecificationButNoRelationReplacedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to confirm a specification, but no relation(s)" );

		if( correctedAssumptionReplacedSpecificationItem_ != null )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a corrected assumption replaced specification" );

			correctedAssumptionReplacedSpecificationItem_ = null;
			}

		if( obsoleteAssumptionSpecificationItem != null )
			{
			if( myWordItem_.attachJustificationInWord( obsoleteAssumptionSpecificationItem.firstJustificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the first justification of the found assumption specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteAssumptionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete assumption specification" );
			}

		if( obsoleteConcludedAssumptionSpecificationItem != null &&
		replaceOrDeleteSpecification( obsoleteConcludedAssumptionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete concluded assumption specification" );

		if( obsoleteHiddenSpanishSpecificationItem != null )
			{
			if( myWordItem_.attachJustificationInWord( obsoleteHiddenSpanishSpecificationItem.firstJustificationItem(), createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found replaced hidden specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteHiddenSpanishSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete hidden Spanish specification" );
			}

		if( obsoleteNotCollectedSpecificationItem != null )
			{
			if( replaceOrDeleteSpecification( obsoleteNotCollectedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete not-collected specification" );

			// Clean-up obsolete justifications
			if( myWordItem_.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete obsolete justifications" );
			}

		if( obsoleteUserSpecificationItem != null &&
		replaceOrDeleteSpecification( obsoleteUserSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete an obsolete user specification" );

		return Constants.RESULT_OK;
		}

	private byte writeConfirmedSpecification( short interfaceParameter, SpecificationItem writeSpecificationItem )
		{
		if( interfaceParameter <= Constants.NO_INTERFACE_PARAMETER )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given interface parameter is undefined" );

		if( writeSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		if( myWordItem_.writeSelectedSpecification( false, true, writeSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

		if( CommonVariables.writtenSentenceStringBuffer == null ||
		CommonVariables.writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given write specification" );

		if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a confirmation" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
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

		if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( olderSpecificationItem.isQuestion() ? ( olderSpecificationItem.isSelfGenerated() ? Constants.INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : Constants.INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem.isSelfGenerated() ? ( olderSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : Constants.INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface listing text" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );

		return Constants.RESULT_OK;
		}

	private RelatedResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		boolean isFirstRelatedSpecification = true;
		boolean isLastRelatedSpecification = false;
		int currentSpecificationSentenceNr;
		int currentSpecificationItemNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		int highestItemNr = Constants.NO_ITEM_NR;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		RelatedResultType relatedResult = new RelatedResultType();

		if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
			return myWordItem_.startRelatedResultErrorInWord( 1, moduleNameString_, "The given specification collection number is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startRelatedResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		currentSpecificationItem = ( isCheckingAllQuestions ? myWordItem_.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) :
															myWordItem_.firstSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) );

		if( currentSpecificationItem != null )
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
						relatedResult.relatedSpecificationItem == null ||

						( currentSpecificationItem.specificationCollectionNr() == specificationCollectionNr &&
						currentSpecificationWordItem != specificationWordItem ) )
							{
							relatedResult.isFirstRelatedSpecification = isFirstRelatedSpecification;
							relatedResult.relatedSpecificationItem = currentSpecificationItem;
							}
						}
					}
				}
			while( ( currentSpecificationItem = ( isCheckingAllQuestions ? currentSpecificationItem.nextSelectedSpecificationItem() : currentSpecificationItem.nextSelectedSpecificationItem() ) ) != null );

			relatedResult.isLastRelatedSpecification = isLastRelatedSpecification;
			}

		return relatedResult;
		}

	private SpecificationResultType checkSpecificationWordForConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationProperName, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		boolean isSpecificationWordSpanishAmbiguous;
		boolean hasFoundCompoundConflict = false;
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		int nonCompoundSpecificationCollectionNr;
		int compoundSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		SpecificationItem anotherConflictingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem tempConflictingSpecificationItem;
		SpecificationItem conflictingSpecificationItem = null;
		SpecificationItem pastTenseAssignmentItem = null;
		RelatedResultType relatedResult = new RelatedResultType();
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( specificationWordItem == null )
			return myWordItem_.startSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// Derive conflict
		if( hasSpecificationCollection &&
		( relatedResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

		if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) == null )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			// Check for looping
			if( isSpecificationWordSpanishAmbiguous ||
			( conflictingSpecificationItem = specificationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, myWordItem_ ) ) == null )
				{
				// Check for past tense
				foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem != null &&
				foundSpecificationItem.isArchivedAssignment() != isArchivedAssignment )
					conflictingSpecificationItem = foundSpecificationItem;
				else
					{
					if( !isSpecificationWordSpanishAmbiguous &&
					( compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr ) ) > Constants.NO_COLLECTION_NR )
						{
						// Check for negative
						foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, specificationWordItem );

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
							if( ( anotherConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, null ) ) != null &&
							// Write conflict
							anotherConflictingSpecificationItem.writeSpecificationConflict() != Constants.RESULT_OK )
								return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write another conflicting specification" );

							conflictingSpecificationItem = foundSpecificationItem;
							}
						else
							{
							// Check for existing compound specification
							// No self-generated question available for this specification
							if( ( nonCompoundSpecificationCollectionNr = nonCompoundCollectionNrInCollectionWords( compoundSpecificationCollectionNr ) ) > Constants.NO_COLLECTION_NR )
								{
								tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, null );

								if( tempConflictingSpecificationItem == null )
									{
									tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, Constants.NO_CONTEXT_NR, relationContextNr, null );

									if( tempConflictingSpecificationItem != null &&
									!tempConflictingSpecificationItem.isOlderItem() &&
									tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem )
										conflictingSpecificationItem = tempConflictingSpecificationItem;
									}
								else
									{
									if( tempConflictingSpecificationItem.specificationWordItem() != specificationWordItem &&
									!tempConflictingSpecificationItem.isHiddenSpanishSpecification() )
										conflictingSpecificationItem = tempConflictingSpecificationItem;
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
			conflictingSpecificationItem = ( pastTenseAssignmentItem == null ? relatedSpecificationItem : pastTenseAssignmentItem );
			}

		if( !hasFoundCompoundConflict &&
		!isExclusiveSpecification &&
		conflictingSpecificationItem != null &&
		!conflictingSpecificationItem.hasSpecificationBeenWrittenAsConflict() )
			{
			if( conflictingSpecificationItem.isOlderItem() )
				{
				if( ( !hasSpecificationCollection ||
				isGeneralizationProperName ) &&

				// Write conflicting specification
				conflictingSpecificationItem.writeSpecificationConflict() != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the conflicting specification" );
				}
			else
				{
				if( hasSpecificationCollection &&
				!conflictingSpecificationItem.hasSpecificationNonExclusiveCollection() &&

				// Sentence in conflict with itself
				InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
					return myWordItem_.addSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			}

		specificationResult.specificationItem = conflictingSpecificationItem;
		return specificationResult;
		}

	private UserSpecificationResultType checkUserSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationProperName, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isValueSpecification, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundRelationContext;
		boolean hasRelationContext = ( relationContextNr > Constants.NO_CONTEXT_NR );
		JustificationItem nextJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem predecessorObsoleteJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		if( foundSpecificationItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given found specification item is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( foundSpecificationItem.isValueSpecification() != isValueSpecification )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "Value specification conflict! Specification word \"" + specificationWordItem.anyWordTypeString() + "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );

		if( !hasRelationContext &&
		!isAssignment &&
		!isExclusiveSpecification &&
		foundSpecificationItem.isExclusiveSpecification() &&
		foundSpecificationItem.isUserSpecification() )
			{
			if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );

			hasDisplayedMoreSpecificNonExclusiveSpecification_ = true;
			userSpecificationResult.isNonExclusiveSpecification = true;

			if( ( relatedResult = findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find the related part of a more specific specification" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
			( obsoleteJustificationItem = myWordItem_.primarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, relatedSpecificationItem ) ) != null &&
			( secondarySpecificationItem = obsoleteJustificationItem.secondarySpecificationItem() ) != null &&
			( assumptionSpecificationItem = myWordItem_.firstAssignmentOrSpecificationItem( true, true, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationItem.specificationWordItem() ) ) != null &&
			( predecessorObsoleteJustificationItem = assumptionSpecificationItem.firstJustificationItem() ) != null )
				{
				while( ( nextJustificationItem = predecessorObsoleteJustificationItem.attachedJustificationItem() ) != obsoleteJustificationItem )
					predecessorObsoleteJustificationItem = nextJustificationItem;

				if( predecessorObsoleteJustificationItem != obsoleteJustificationItem )
					{
					if( ( justificationResult = myWordItem_.copyJustification( true, predecessorObsoleteJustificationItem.primarySpecificationItem(), predecessorObsoleteJustificationItem.secondarySpecificationItem(), null, predecessorObsoleteJustificationItem ) ).result != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to copyh the predecessor obsolete justification item" );

					if( justificationResult.createdJustificationItem == null )
						return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The created justification item is undefined" );

					if( myWordItem_.replaceJustification( predecessorObsoleteJustificationItem, justificationResult.createdJustificationItem, assumptionSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete justification item" );

					if( myWordItem_.replaceJustification( obsoleteJustificationItem, null, assumptionSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete justification item" );
					}
				}
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
						specificationWordItem.isMale() != relationWordItem.isMale() &&

						// Check for specification conflict
						checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
						}
					else
						{
						if( !isInactiveAssignment )
							{
							hasDisplayedMoreSpecificSpecification_ = true;

							if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific related specification" );
							}
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
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
				CommonVariables.nUserRelationWords < myWordItem_.nContextWordsInContextWords( foundSpecificationItem.relationContextNr(), specificationWordItem ) ) )
					{
					// Has new relation context
					if( hasRelationContext ||
					myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, relationContextNr, specificationWordItem ) == null )
						{
						if( !hasRelationContext )
							{
							hasConfirmedSpecificationButNoRelation_ = true;
							userSpecificationResult.confirmedSpecificationButNoRelationReplacedSpecificationItem = foundSpecificationItem;
							}

						if( !foundSpecificationItem.isHiddenSpanishSpecification() &&
						writeConfirmedSpecification( ( foundSpecificationItem.isSelfGeneratedAssumption() ? ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NO_RELATION ) : ( hasRelationContext ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NO_RELATION ) ), foundSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the confirmed specification" );
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
							// Has already confirmed another specification
							if( hasConfirmedSpecification_ &&
							foundSpecificationItem.hasRelationContext() &&
							foundSpecificationItem.hasSpecificationNonCompoundCollection() &&
							foundSpecificationItem.relationContextNr() != relationContextNr &&
							( obsoleteJustificationItem = myWordItem_.secondarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, foundSpecificationItem ) ) != null &&

							// Replace justification of confirmed specification
							myWordItem_.replaceJustification( obsoleteJustificationItem, null, foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete justification item" );

							if( foundSpecificationItem.isExclusiveSpecification() )
								userSpecificationResult.isConfirmedExclusive = true;

							if( foundSpecificationItem.isSelfGenerated() )
								hasConfirmedSpecification_ = true;

							userSpecificationResult.confirmedReplacedSpecificationItem = foundSpecificationItem;

							if( !foundSpecificationItem.isHiddenSpanishSpecification() &&
							writeConfirmedSpecification( ( foundSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : Constants.INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ), foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the confirmed specification" );
							}

						if( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						foundSpecificationItem.specificationWordTypeNr() == Constants.WORD_TYPE_NOUN_SINGULAR )
							{
							if( processPreviouslyUnknownPluralNoun( specificationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to process a previously unknown plural noun" );

							userSpecificationResult.confirmedReplacedSpecificationItem = foundSpecificationItem;
							}
						}
					}
				}
			}

		return userSpecificationResult;
		}

	private UserSpecificationResultType checkUserSpecificationOrQuestion( boolean hasRelationContext, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationProperName, boolean isNegative, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isValueSpecification, short assumptionLevel, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFeminineSingularNounEnding;
		boolean isSingularNounGeneralizationWord = ( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR );
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordTypeItem foundWordTypeItem;
		WordTypeItem singularNounWordTypeItem;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();
		BoolResultType boolResult = new BoolResultType();
		RelatedResultType relatedResult;

		hasCorrectedAssumptionByOppositeQuestion_ = false;

		if( specificationWordItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// Skip if definition (singular noun) is Spanish ambiguous
		if( isSingularNounGeneralizationWord &&
		specificationWordItem != myWordItem_ &&
		// Check specification in opposite direction
		( boolResult = myWordItem_.findGeneralization( false, specificationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find out if word \"" + specificationWordItem.anyWordTypeString() + "\" is one of my generalization words" );

		// Has found generalization
		// User specification is looping
		if( boolResult.booleanValue )
			{
			if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification looping conflict" );
			}
		else
			{
			// Check current assignment, specification or question (with relation)
			if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( true, true, true, true, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isQuestion ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) != null )
				{
				if( !isQuestion &&
				isArchivedAssignment != foundSpecificationItem.isArchivedAssignment() &&

				// Check for specification conflict
				checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
				}

			if( isQuestion &&
			foundSpecificationItem == null )
				foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

			if( foundSpecificationItem == null )
				{
				// Check current assignment or specification (without this relation)
				if( isQuestion )
					foundSpecificationItem = myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem == null )
					foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, generalizationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem );

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
						myWordItem_.hasMasculineProperNameEnding() != specificationWordItem.isMale() &&
						InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NAME_TO_BE, myWordItem_.anyWordTypeString(), ( myWordItem_.hasFeminineProperNameEnding() ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a proper name expected to be feminine or masculine" );

						if( specificationCollectionNr > Constants.NO_COLLECTION_NR )
							{
							if( ( relatedResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

							relatedSpecificationItem = relatedResult.relatedSpecificationItem;

							if( hasRelationContext &&
							relatedSpecificationItem != null &&
							!relatedSpecificationItem.hasRelationContext() &&
							relatedSpecificationItem.isSelfGeneratedAssumption() )
								{
								// Assumption needs to be corrected
								if( correctedAssumptionReplacedSpecificationItem_ != null )
									return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The corrected assumption specification item already assigned" );

								if( myWordItem_.writeUpdatedSpecification( false, false, true, false, false, relatedSpecificationItem ) != Constants.RESULT_OK )
									return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );

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
									// Check for specification conflict
									if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
										return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
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
					myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == null &&
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

							CommonVariables.hasDisplayedArticleNotification = true;
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
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					if( CommonVariables.writtenSentenceStringBuffer == null ||
					CommonVariables.writtenSentenceStringBuffer.length() == 0 )
						return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "I couldn't write a sentence with an assumption about the relation" );

					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a missing relation" );

					if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					// Force 'I know' notification
					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the 'I know' interface notification" );
					}
				}

			if( !CommonVariables.hasDisplayedWarning )
				{
				if( isQuestion )
					{
					if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check the user question" );

					// Find self-generated question
					if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) ).result != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting self-generated question" );

					if( !isOnlyCheckingForConflicts_ &&
					CommonVariables.nUserSpecificationWords > 1 &&
					myWordItem_.sameUserQuestionSpecificationItem( questionParameter ) != null )
						{
						if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write 'I had the same question before' interface notification" );

						userSpecificationResult.isSameQuestionFromUser = true;
						}
					else
						{
						// Find user question
						if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) ).result != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting user question" );
						}

					isOnlyCheckingForConflicts_ = true;
					}
				else
					{
					if( foundSpecificationItem == null )
						{
						if( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL )
							{
							// Find singular word type
							singularNounWordTypeItem = specificationWordItem.activeWordTypeItem( Constants.WORD_TYPE_NOUN_SINGULAR );

							if( singularNounWordTypeItem != null &&
							// Skip currently created singular noun
							singularNounWordTypeItem.isOlderItem() &&
							processPreviouslyUnknownPluralNoun( specificationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to process a previously unknown plural noun" );
							}
						}
					else
						{
						if( ( userSpecificationResult = checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, isSelection, isValueSpecification, specificationWordTypeNr, specificationCollectionNr, relationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check the user specification" );
						}
					}
				}
			}

		return userSpecificationResult;
		}

	private UserSpecificationResultType findSameSimilarOrRelatedQuestion( boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isOnlyCheckingForConflicts, boolean isPossessive, boolean isSelfGenerated, boolean isSpecificationGeneralization, short assumptionLevel, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr )
		{
		boolean hasFoundQuestion;
		boolean isUserSpecificationWord;
		boolean hasFoundOlderSpecification = false;
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
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem foundQuestionSpecificationItem = null;
		WordItem currentWordItem;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		if( ( currentWordItem = CommonVariables.firstWordItem ) == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The first word item is undefined" );

		// Do for all words, including predefined words
		do	{
			hasFoundQuestion = false;
			isUserSpecificationWord = currentWordItem.isUserSpecificationWord;

			if( ( currentQuestionSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, currentWordItem ) ) != null )
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
							if( myWordItem_.isContextSubsetInContextWords( relationContextNr, foundQuestionSpecificationItem.relationContextNr() ) )
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

					// Prefer similar over related, if both occur
					( foundQuestionSpecificationItem != null &&

					( !hasFoundSpecificationRelationContext ||
					!isSameRelationContext ) ) )
						{
						if( !hasRelationContext ||
						hasSpecificationCollection ||
						!isSameAssignment )
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

					if( CommonVariables.writtenSentenceStringBuffer == null ||
					CommonVariables.writtenSentenceStringBuffer.length() == 0 )
						return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "I couldn't write the found specification" );

					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE ) : ( isSimilarQuestion && !isRelatedQuestion ? Constants.INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE : Constants.INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE ) ) ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a related question" );

					if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

					userSpecificationResult.isSimilarOrRelatedQuestion = true;
					}
				}
			else
				{
				if( hasFoundOlderSpecification )
					{
					if( !isOnlyCheckingForConflicts &&

					( !isSameExclusiveSpecification ||
					!isSameRelationContext ) )
						{
						hasDisplayedMoreSpecificQuestion_ = true;

						// Skip displaying announcement if question is in conflict with itself
						if( isExclusiveSpecification ||
						CommonVariables.nUserSpecificationWords == 1 )
							{
							if( writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a more specific related question" );
							}
						}
					}
				else
					{
					if( hasDisplayedMoreSpecificQuestion_ &&
					isConflictingQuestion &&
					InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}

		return userSpecificationResult;
		}


	// Constructor

	protected WordSpecification( WordItem myWordItem )
		{
		// Private constructed variables

		hasConfirmedSpecification_ = false;
		hasConfirmedSpecificationButNoRelation_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasCorrectedAssumptionByOppositeQuestion_ = false;
		hasDisplayedMoreSpecificSpecification_ = false;
		hasDisplayedMoreSpecificNonExclusiveSpecification_ = false;
		hasDisplayedMoreSpecificQuestion_ = false;
		isThisProperNameWordTouchedDuringCurrentSentence_ = false;
		isOnlyCheckingForConflicts_ = false;

		spanishCompoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		correctedAssumptionReplacedSpecificationItem_ = null;
		firstConfirmedReplacedSpanishSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		spanishCompoundSpecificationWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given my word item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeWordSpecificationVariables()
		{
		hasConfirmedSpecification_ = false;
		hasConfirmedSpecificationButNoRelation_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasDisplayedMoreSpecificSpecification_ = false;
		hasDisplayedMoreSpecificNonExclusiveSpecification_ = false;
		hasDisplayedMoreSpecificQuestion_ = false;
		isThisProperNameWordTouchedDuringCurrentSentence_ = false;
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

	protected boolean hasConfirmedSpecificationButNoRelation()
		{
		return hasConfirmedSpecificationButNoRelation_;
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

	protected boolean hasDisplayedMoreSpecificSpecification()
		{
		return hasDisplayedMoreSpecificSpecification_;
		}

	protected boolean hasDisplayedMoreSpecificNonExclusiveSpecification()
		{
		return hasDisplayedMoreSpecificNonExclusiveSpecification_;
		}

	protected boolean hasDisplayedMoreSpecificQuestion()
		{
		return hasDisplayedMoreSpecificQuestion_;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isGeneralizationProperName, boolean isNegative, boolean isPossessive, short specificationWordTypeNr, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationItem conflictingSpecificationItem;
		SpecificationResultType specificationResult;

		if( ( specificationResult = checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );

		conflictingSpecificationItem = specificationResult.specificationItem;

		if( conflictingSpecificationItem != null &&
		conflictingSpecificationItem.isOlderItem() &&

		( isNegative ||
		!conflictingSpecificationItem.isNegative() ||
		conflictingSpecificationItem.hasSpecificationCompoundCollection() ) &&

		// Opposite negative of conflicting specification word
		checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, !isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, conflictingSpecificationItem.specificationWordItem() ).result != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to check for the opposite negative specification conflict" );

		return Constants.RESULT_OK;
		}

	protected CreateAndAssignResultType addSpecificationInWord( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
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
		SpecificationItem createdSpecificationItem;
		SpecificationItem replacingSpecificationItem;
		SpecificationItem confirmedReplacedSpecificationItem = null;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem foundUserSpecificationItem = null;
		SpecificationItem obsoleteAssumptionSpecificationItem = null;
		SpecificationItem obsoleteConcludedAssumptionSpecificationItem = null;
		SpecificationItem obsoleteNotCollectedSpecificationItem = null;
		SpecificationItem obsoleteHiddenSpanishSpecificationItem = null;
		SpecificationItem obsoleteUserSpecificationItem = null;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		if( !isThisProperNameWordTouchedDuringCurrentSentence_ )
			{
			isThisProperNameWordTouchedDuringCurrentSentence_ = true;
			addToTouchedDuringCurrentSentenceProperNameWordList();
			}

		correctedAssumptionReplacedSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		// Check generalization word type
		if( !myWordItem_.hasWordType( true, generalizationWordTypeNr ) )
			return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		// Check relation variables
		if( ( hasRelationContext ||
		relationWordTypeNr != Constants.NO_WORD_TYPE_NR ||
		relationWordItem != null ) &&

		( ( !hasRelationContext &&
		// Exception: Conditions have no relation context
		!isConditional ) ||

		relationWordItem == null ||
		!relationWordItem.hasWordType( false, relationWordTypeNr ) ) )
			return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The relation variables aren't matching" );

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
			collectSpecifications( isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect myself with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

			if( hasSpecificationCollection )
				{
				if( !specificationWordItem.hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification collection" );
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
					// Skip definition sentence with the same generalization and specification word
					!specificationWordItem.isUserGeneralizationWord ) ||

					( !isQuestion &&

					// Non-exclusive specification collection
					( ( !isExclusiveSpecification &&

					( !isSpecificationWordSpanishAmbiguous ||

					( hasRelationContext &&

					( nContextRelations > 1 ||
					!specificationWordItem.isUserSpecificationWord ) ) ) ) ||

					// Exclusive specification collection
					( isExclusiveSpecification &&
					!hasRelationContext &&
					!isPossessive &&
					!isSelfGenerated &&
					compoundCollectionNr > Constants.NO_COLLECTION_NR &&
					CommonVariables.nUserSpecificationWords > 2 ) ) ) ) )
						specificationCollectionNr = nonCompoundCollectionNr;
					else
						specificationCollectionNr = compoundCollectionNr;
					}
				}

			if( !isSelfGenerated &&
			myWordItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&

				( !isAssignment ||
				isGeneralizationProperName ) )
					userSpecificationCollectionNr_ = specificationCollectionNr;

				if( ( userSpecificationResult = checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to check a user specification or question with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

				confirmedReplacedSpecificationItem = userSpecificationResult.confirmedReplacedSpecificationItem;
				}

			if( !CommonVariables.hasDisplayedWarning )
				{
				// Try to find specification
				// Start with finding user specification
				if( ( foundUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, true, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == null )
					{
					if( isSelfGenerated )
						{
						if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isAssignment && !isArchivedAssignment ? Constants.NO_CONTEXT_NR : ( hasCopiedRelationContext ? copiedRelationContextNr : relationContextNr ) ), specificationWordItem ) ) == null )
							{
							// Not found
							if( hasRelationContext &&
							!isQuestion &&
							( replacingSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, relationContextNr, specificationWordItem ) ) != null )
								{
								if( !hasCopiedRelationContext &&
								compoundCollectionNr > Constants.NO_COLLECTION_NR &&
								!replacingSpecificationItem.hasSpecificationCollection() &&
								replacingSpecificationItem.isSelfGenerated() )
									obsoleteNotCollectedSpecificationItem = replacingSpecificationItem;
								else
									{
									if( isSpecificationWordSpanishAmbiguous &&
									replacingSpecificationItem.hasRelationContext() )
										{
										if( !isPossessive &&
										replacingSpecificationItem.isSelfGeneratedConclusion() &&
										replacingSpecificationItem.isHiddenSpanishSpecification() )
											obsoleteHiddenSpanishSpecificationItem = replacingSpecificationItem;
										}
									else
										{
										if( isUserRelationWord &&

										( !isSpecificationWordSpanishAmbiguous ||
										hasSpecificationCollection ) &&

										replacingSpecificationItem.isSelfGenerated() )
											{
											if( replacingSpecificationItem.relationContextNr() == relationContextNr )
												foundSpecificationItem = replacingSpecificationItem;
											else
												{
												// Find any specification
												replacingSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem );

												if( replacingSpecificationItem != null &&
												!replacingSpecificationItem.hasRelationContext() )
													{
													if( confirmedReplacedSpecificationItem != null )
														return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The confirmed replaced specification item is already defined" );

													if( myWordItem_.writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, isSpecificationWordSpanishAmbiguous, false, false, !isSpecificationWordSpanishAmbiguous, replacingSpecificationItem ) != Constants.RESULT_OK )
														return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to write an assumption, which will have an added relation" );

													confirmedReplacedSpecificationItem = replacingSpecificationItem;
													}
												}
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

										( relationWordItem == null ||

										( relationWordItem.isOlderItem() &&

										( foundSpecificationItem.hasSpecificationCompoundCollection() ||

										( isUserRelationWord &&
										foundSpecificationItem.olderJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != null &&

										( isSpecificationWordSpanishAmbiguous ||
										foundSpecificationItem.hasOnlyOneRelationWord() ) ) ) &&

										( ( !isSpecificationWordSpanishAmbiguous &&
										// Conclusion doesn't exist
										myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, null ) == null ) ||

										// Typically for the Spanish language
										( isSpecificationWordSpanishAmbiguous &&
										!foundSpecificationItem.isHiddenSpanishSpecification() ) ) ) ) )
											{
											if( foundSpecificationItem.markAsConcludedAssumption( false ) != Constants.RESULT_OK )
												return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

											if( isUserRelationWord ||
											foundSpecificationItem.isPartOf() ||

											( foundSpecificationItem.isNegative() &&
											!myWordItem_.hasAnsweredSelfGeneratedQuestion() ) )
												{
												if( myWordItem_.writeUpdatedSpecification( true, false, false, false, false, foundSpecificationItem ) != Constants.RESULT_OK )
													return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to write a concluded assumption" );
												}
											}
										}
									else
										{
										if( hasRelationContext &&
										!isSpecificationWordSpanishAmbiguous )
											{
											if( isPossessive )
												{
												if( !isUserRelationWord &&
												foundSpecificationItem.isConcludedAssumption() )
													obsoleteConcludedAssumptionSpecificationItem = foundSpecificationItem;
												}
											else
												{
												if( isUserRelationWord &&
												relationWordItem != null &&
												!relationWordItem.isOlderItem() &&
												!foundSpecificationItem.hasAssumptionLevel() &&
												!foundSpecificationItem.hasSpecificationCompoundCollection() )
													// Conclusion becomes assumption
													// No specification will be created
													foundSpecificationItem.incrementAssumptionLevel();
												}
											}
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
							confirmedReplacedSpecificationItem != null &&
							foundUserSpecificationItem.isUserAssignment() ) ||

							!foundUserSpecificationItem.hasSpecificationCollection() ) )
								// Prepare foundUserSpecificationItem for confirmation
								foundUserSpecificationItem = myWordItem_.firstSpecificationItem( isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );
							else
								{
								if( !hasRelationContext ||
								!isQuestion ||
								!foundUserSpecificationItem.hasRelationContext() ||
								foundUserSpecificationItem.relationContextNr() == relationContextNr )
									foundUserSpecificationItem = null;
								else
									{
									// Question with different relation context
									foundSpecificationItem = null;
									foundUserSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

									if( foundUserSpecificationItem != null &&
									foundUserSpecificationItem.assumptionLevel() != assumptionLevel )
										foundUserSpecificationItem = null;
									}
								}
							}
						else	// Typically for compound collections in Spanish
							{
							if( spanishCompoundSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR ||
							spanishCompoundSpecificationWordItem_ == null )
								{
								if( ( specificationCollectionNr = myWordItem_.highestCollectionNrInCollectionWords() ) >= Constants.MAX_COLLECTION_NR )
									return myWordItem_.startCreateAndAssignResultSystemErrorInWord( 1, moduleNameString_, "Collection number overflow" );

								spanishCompoundSpecificationCollectionNr_ = ++specificationCollectionNr;
								spanishCompoundSpecificationWordItem_ = specificationWordItem;
								}
							else
								{
								specificationCollectionNr = spanishCompoundSpecificationCollectionNr_;

								if( spanishCompoundSpecificationWordItem_.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + spanishCompoundSpecificationWordItem_.anyWordTypeString() + "\" with word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								if( specificationWordItem.addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishCompoundSpecificationWordItem_, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect word \"" + specificationWordItem.anyWordTypeString() + "\" with word \"" + spanishCompoundSpecificationWordItem_.anyWordTypeString() + "\"" );
								}
							}
						}
					else
						{
						if( foundUserSpecificationItem.isSpecificationWordSpanishAmbiguous() )
							specificationCollectionNr = myWordItem_.collectionNr( Constants.WORD_TYPE_NOUN_SINGULAR, myWordItem_ );
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

		if( !CommonVariables.hasDisplayedWarning )
			{
			if( foundSpecificationItem == null ||

			// Exceptions
			confirmedReplacedSpecificationItem != null ||
			correctedAssumptionReplacedSpecificationItem_ != null ||
			obsoleteConcludedAssumptionSpecificationItem != null ||
			userSpecificationResult.isNonExclusiveSpecification ||

			// Overwrite non-conditional specification by conditional one
			( isConditional &&
			!foundSpecificationItem.isConditional() ) ||

			// Overwrite non-exclusive specification by exclusive one
			( isExclusiveSpecification &&
			!foundSpecificationItem.isExclusiveSpecification() ) ||

			// Overwrite user questions by non-exclusive one
			( !isAssignment &&
			isQuestion &&
			!isSelfGenerated &&
			!userSpecificationResult.isSameQuestionFromUser &&
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
					foundSpecificationItem != confirmedReplacedSpecificationItem &&
					foundSpecificationItem.isUserSpecification() )
						obsoleteUserSpecificationItem = foundSpecificationItem;
					}

				if( isExclusiveSpecification )
					{
					if( userSpecificationResult.isNonExclusiveSpecification )
						isExclusiveSpecification = false;
					}
				else
					{
					if( userSpecificationResult.isConfirmedExclusive )
						isExclusiveSpecification = true;
					}

				originalSentenceNr = ( hasRelationContext ||
									isQuestion ||
									foundSpecificationItem == null ? CommonVariables.currentSentenceNr : foundSpecificationItem.originalSentenceNr() );

				// Create the actual specification
				if( ( createAndAssignResult = myWordItem_.createSpecificationItem( false, false, false, false, false, isConditional, ( hasCorrectedAssumptionByKnowledge_ || hasCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, Constants.NO_ASSIGNMENT_LEVEL, assumptionLevel, CommonVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? Constants.NO_WORD_TYPE_NR : relationWordTypeNr ), specificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to create a specification item" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
					return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I couldn't create a specification item" );

				// Now replaced obsolete specifications by the created specification
				if( replaceObsoleteSpecification( confirmedReplacedSpecificationItem, userSpecificationResult.confirmedSpecificationButNoRelationReplacedSpecificationItem, createdSpecificationItem, foundUserSpecificationItem, obsoleteAssumptionSpecificationItem, obsoleteConcludedAssumptionSpecificationItem, obsoleteHiddenSpanishSpecificationItem, obsoleteNotCollectedSpecificationItem, obsoleteUserSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete specification" );

				if( hasCopiedRelationContext &&
				firstJustificationItem != null &&
				foundSpecificationItem != null &&

				( isPossessive ||
				foundSpecificationItem.isSelfGeneratedAssumption() ||
				foundSpecificationItem.isUserSpecification() ) )
					{
					if( !firstJustificationItem.isOlderItem() &&

					( isSpecificationWordSpanishAmbiguous ||
					foundSpecificationItem.hasCurrentCreationSentenceNr() ||
					foundSpecificationItem.isConcludedAssumption() ) )
						{
						if( firstJustificationItem.changeAttachedJustification( foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
							return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
						}

					if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );

					if( foundSpecificationItem.isSelfGeneratedAssumption() &&
					!foundSpecificationItem.isSpecificationWordSpanishAmbiguous() &&

					// Clean-up obsolete justifications
					myWordItem_.replaceOrDeleteObsoleteJustifications() != Constants.RESULT_OK )
						return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace or delete obsolete justifications" );
					}

				if( !isNegative &&
				!isPossessive &&
				!isQuestion &&
				specificationWordItem != null &&
				!userSpecificationResult.isSimilarOrRelatedQuestion &&

				( !isSpecificationWordSpanishAmbiguous ||
				!createdSpecificationItem.isHiddenSpanishSpecification() ) )
					{
					if( myWordItem_.findAnswersToQuestions( specificationWordItem.compoundCollectionNr( specificationWordTypeNr ), createdSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to find answers to questions" );

					if( !isSelfGenerated &&
					!myWordItem_.isFemaleOrMale() )
						{
						if( specificationWordItem.isFemale() )
							{
							if( myWordItem_.markWordAsFemale() != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as female" );
							}
						else
							{
							if( specificationWordItem.isMale() )
								{
								if( myWordItem_.markWordAsMale() != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as male" );

								if( !isAssignment &&
								isGeneralizationProperName &&
								myWordItem_.isSpanishCurrentLanguage() &&
								displayAssumptionsThatAreNotHiddenAnymore() != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to display assumptions that are not hidden anymore" );
								}
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

		return findRelatedSpecification( false, isCheckingRelationContext, false, false, searchSpecificationItem.isAssignment(), searchSpecificationItem.isInactiveAssignment(), searchSpecificationItem.isArchivedAssignment(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPossessive(), searchSpecificationItem.questionParameter(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationContextNr(), searchSpecificationItem.relationContextNr(), searchSpecificationItem.specificationWordItem() );
		}

	protected RelatedResultType findRelatedSpecification( boolean isCheckingAllQuestions, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		RelatedResultType relatedResult = new RelatedResultType();

		if( isIncludingAssignments )
			{
			if( ( relatedResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			relatedResult.relatedSpecificationItem == null &&
			// Archived assignments
			( relatedResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification as well" );
			}

		// Specifications
		if( relatedResult.relatedSpecificationItem == null &&
		( relatedResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

		return relatedResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
