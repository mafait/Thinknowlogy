/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		Creating specification structures
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
 *************************************************************************/
/*	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, eit	her version 2 of the License, or
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

package org.mafait.thinknowlogy;

class WordSpecification
	{
	// Private constructed variables

	private boolean hasCurrentlyConfirmedSpecification_ = false;
	private boolean hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = false;
	private boolean hasCurrentlyConfirmedSpecificationButNoRelation_ = false;
	private boolean hasCurrentlyCorrectedAssumptionByKnowledge_ = false;
	private boolean hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ = false;
	private boolean hasCurrentlyMoreSpecificSpecification_ = false;
	private boolean isOnlyCheckingForConflicts_ = false;
	private boolean isWordTouchedDuringCurrentSentence_ = false;

	private int compoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
	private int userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

	private SpecificationItem correctedAssumptionReplacedSpecificationItem_ = null;
	private SpecificationItem previousConflictingSpecificationItem_ = null;
	private SpecificationItem replacedAssignmentItem_ = null;

	private WordItem spanishPreviousSpecificationWordItem_ = null;

	private final String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private WordItem myWordItem_;


	// Private methods

	private static boolean hasOnlyEitherFeminineOrMasculineSpecificationWords( boolean isFeminineWord )
		{
		WordItem currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = GlobalVariables.firstSpecificationWordItem ) != null )
			{
			// Do for all words touched during current sentence
			do	{
				if( currentSpecificationWordItem.isUserSpecificationWord &&
				// Found difference
				currentSpecificationWordItem.isFeminineWord() != isFeminineWord )
					return false;
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
			}

		return true;
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

	private byte checkUserQuestion( boolean hasRelationWord, boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, SpecificationItem foundQuestionSpecificationItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationItem relatedSpecificationItem;
		RelatedResultType relatedResult;

		if( foundQuestionSpecificationItem != null &&
		foundQuestionSpecificationItem.isAnsweredQuestion() &&
		foundQuestionSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification &&
		foundQuestionSpecificationItem.relationCollectionNr() == relationCollectionNr &&
		foundQuestionSpecificationItem.relationWordItem() == relationWordItem )
			GlobalVariables.isQuestionAlreadyAnswered = true;

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&

		( foundQuestionSpecificationItem == null ||
		!foundQuestionSpecificationItem.isQuestion() ) )
			{
			if( ( relatedResult = findRelatedSpecification( false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related question" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
			!isExclusiveSpecification &&
			!isSpecificationGeneralization )
				{
				if( relatedSpecificationItem.isOlderItem() )
					{
					if( hasRelationWord )
						{
						if( correctedAssumptionReplacedSpecificationItem_ != null )
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "The corrected assumption replaced specification item already assigned" );

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_.firstSelfGeneratedCheckSpecificationItem( true, true, relatedSpecificationItem.isNegative(), relatedSpecificationItem.isPossessive(), true, Constants.NO_QUESTION_PARAMETER, relatedSpecificationItem.specificationCollectionNr(), relatedSpecificationItem.relationCollectionNr(), relatedSpecificationItem.specificationWordItem(), relatedSpecificationItem.relationWordItem() ) ) != null )
							{
							// Write assumption that needs to be corrected
							if( myWordItem_.writeUpdatedSpecification( false, false, true, false, false, false, false, correctedAssumptionReplacedSpecificationItem_ ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a corrected assumption" );

							hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ = true;
							}
						}
					}
				else
					{
					if( relatedSpecificationItem.hasCompoundSpecificationCollection() )
						{
						// Current question in conflict with definition
						if( writeSpecificationInConflictWithDefinition( true, relatedSpecificationItem.specificationWordItem(), specificationWordItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write that the current question is in conflict with a definition" );

						GlobalVariables.isConflictingQuestion = true;
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte collectSimpleSpecifications( short collectionWordTypeNr, short commonWordTypeNr, WordItem collectionWordItem )
		{
		int specificationCollectionNr;
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;
		WordItem foundGeneralizationWordItem = null;
		CollectionResultType collectionResult;

		if( collectionWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given collection word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = myWordItem_.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return myWordItem_.startSystemErrorInWord( 1, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != collectionWordItem &&
				// Avoid idle call
				currentGeneralizationWordItem.firstExclusiveSpecificationItem( true ) != null )
					{
					// Collect specification words exclusively
					if( ( collectionResult = currentGeneralizationWordItem.collectExclusiveSpecificationWords( collectionWordTypeNr, commonWordTypeNr, myWordItem_, collectionWordItem ) ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to exclusively collect specification words in generalization word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

					if( ( foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem ) != null &&
					( specificationCollectionNr = collectionWordItem.collectionNr( currentGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR &&
					// Collect by generalization
					foundGeneralizationWordItem.collectSpecificationsInWord( false, false, specificationCollectionNr ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to collect specifications in word \"" + foundGeneralizationWordItem.anyWordTypeString() + "\"" );
					}
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
			( unknownPluralNounString = WordItem.interfaceString( Constants.INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) ) != null )
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

	private byte replaceOrDeleteSpecification( SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		if( obsoleteSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem.isAssignment() )
			replacedAssignmentItem_ = obsoleteSpecificationItem;
		else
			{
			if( myWordItem_.replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a specification" );
			}

		return Constants.RESULT_OK;
		}

	private byte replaceObsoleteSpecification( boolean isConfirmedSpecificationButNoneOfItsRelations, SpecificationItem confirmedSpecificationItem, SpecificationItem createdSpecificationItem, SpecificationItem obsoleteSpecificationItem )
		{
		if( createdSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given created specification is undefined" );

		if( confirmedSpecificationItem != null &&
		// Replace confirmed specification
		replaceOrDeleteSpecification( confirmedSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete the confirmed specification" );

		if( isConfirmedSpecificationButNoneOfItsRelations &&
		myWordItem_.confirmSpecificationButNotItsRelation( createdSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to confirm a specification, but not its relation(s)" );

		if( correctedAssumptionReplacedSpecificationItem_ != null )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a corrected assumption replaced specification" );

			correctedAssumptionReplacedSpecificationItem_ = null;
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

		if( myWordItem_.writeSelectedSpecification( false, false, olderSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the given older specification" );

		if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
		writtenSentenceStringBuffer.length() == 0 )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given older specification" );

		if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( olderSpecificationItem.isQuestion() ? ( olderSpecificationItem.isSelfGeneratedSpecification() ? Constants.INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : Constants.INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem.isSelfGeneratedSpecification() ? ( olderSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : Constants.INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : Constants.INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface listing text" );

		if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationInConflictWithDefinition( boolean isQuestion, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem definitionSpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( previousSpecificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given previous specification word item is undefined" );

		if( currentSpecificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given current specification word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = currentSpecificationWordItem.firstNounSpecificationGeneralizationItem() ) != null )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return myWordItem_.startSystemErrorInWord( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( definitionSpecificationItem = currentGeneralizationWordItem.firstExclusiveSpecificationItem( currentSpecificationWordItem ) ) != null &&
				currentGeneralizationWordItem.firstSpecificationItem( false, false, false, previousSpecificationWordItem ) != null &&
				// Write conflicting specification
				definitionSpecificationItem.writeSpecificationConflict( isQuestion, false ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an exclusive conflicting specification" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextNounSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private RelatedResultType findRelatedSpecification( boolean isCheckingRelationContext, boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isFirstRelatedSpecification = true;
		boolean isLastRelatedSpecification = false;
		int currentSpecificationSentenceNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		RelatedResultType relatedResult = new RelatedResultType();

		if( specificationWordItem == null )
			return myWordItem_.startRelatedResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) ) != null )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem.relatedSpecificationWordItem( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, relationWordItem ) ) != null )
					{
					currentSpecificationSentenceNr = currentSpecificationItem.creationSentenceNr();

					if( currentSpecificationWordItem == specificationWordItem &&
					currentSpecificationItem.relationCollectionNr() == relationCollectionNr )
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

	private UserSpecificationResultType checkUserSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationWordSpanishAmbiguous, boolean isValueSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, SpecificationItem foundSpecificationItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundExclusiveSpecification;
		boolean hasFoundRelationWord;
		boolean hasRelationWord = ( relationWordItem != null );
		int foundRelationCollectionNr;
		JustificationItem obsoleteJustificationItem;
		JustificationItem predecessorObsoleteJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem oldUserSpecificationItem = null;
		SpecificationItem relatedSpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem userSpecificationItem;
		WordItem foundRelationWordItem;
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		if( foundSpecificationItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given found specification item is undefined" );

		if( specificationWordItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		if( foundSpecificationItem.isValueSpecification() != isValueSpecification )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "Value specification conflict! Specification word \"" + specificationWordItem.anyWordTypeString() + "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );

		hasFoundExclusiveSpecification = foundSpecificationItem.isExclusiveSpecification();

		// No relation word
		if( !hasRelationWord &&
		hasFoundExclusiveSpecification &&
		!isAssignment &&
		!isExclusiveSpecification )
			{
			if( writeMoreSpecificSpecification( foundSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );

			userSpecificationResult.isNonExclusiveSpecification = true;

			if( ( relatedResult = findRelatedSpecification( false, foundSpecificationItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find the related part of a more specific specification" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
			( obsoleteJustificationItem = myWordItem_.primarySpecificationJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, relatedSpecificationItem ) ) != null &&
			( secondarySpecificationItem = obsoleteJustificationItem.secondarySpecificationItem() ) != null &&
			( assumptionSpecificationItem = myWordItem_.firstAssignmentOrSpecificationItem( true, true, false, false, false, secondarySpecificationItem.specificationWordItem() ) ) != null &&
			( predecessorObsoleteJustificationItem = assumptionSpecificationItem.firstJustificationItem() ) != null )
				{
				// Copy justification
				if( ( justificationResult = myWordItem_.copyJustificationItem( predecessorObsoleteJustificationItem.primarySpecificationItem(), predecessorObsoleteJustificationItem.secondarySpecificationItem(), null, predecessorObsoleteJustificationItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to copy the predecessor obsolete justification item" );

				if( justificationResult.createdJustificationItem == null )
					return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The created justification item is undefined" );

				// Replace predecessor justification
				if( myWordItem_.replaceJustification( predecessorObsoleteJustificationItem, justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace a predecessor obsolete justification item" );

				// Replace obsolete justification
				if( myWordItem_.replaceJustification( obsoleteJustificationItem, null ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete justification item" );
				}
			}
		else
			{
			foundRelationWordItem = foundSpecificationItem.relationWordItem();
			hasFoundRelationWord = ( foundRelationWordItem != null );

			// Relation word
			if( hasRelationWord &&
			foundSpecificationItem.isOlderItem() )
				{
				if( foundSpecificationItem.isArchivedAssignment() == isArchivedAssignment &&
				// Check if negative specification exists
				myWordItem_.firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, specificationWordItem ) == null )
					{
					if( hasFoundRelationWord &&
					isPossessive &&
					relationWordItem != null &&
					specificationWordItem.isFeminineWord() != relationWordItem.isFeminineWord() &&
					specificationWordItem.isMasculineWord() != relationWordItem.isMasculineWord() &&
					// Check for specification conflict
					checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );

					if( !isInactiveAssignment &&

						( !hasFoundRelationWord ||

						( !foundSpecificationItem.isExclusiveGeneralization() &&
						// An self-generated assumption was found,
						// but there is also a user specification without relation word
						( oldUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != null &&
						!oldUserSpecificationItem.hasRelationWord() ) ) )
						{
						if( writeMoreSpecificSpecification( oldUserSpecificationItem == null ? foundSpecificationItem : oldUserSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an notification about a more specific related specification" );

						hasCurrentlyMoreSpecificSpecification_ = true;
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			// No conflict found
			if( !GlobalVariables.hasDisplayedWarning )
				{
				foundRelationCollectionNr = foundSpecificationItem.relationCollectionNr();

				if( foundRelationWordItem != null &&

					( ( !hasRelationWord &&
					foundRelationCollectionNr == Constants.NO_COLLECTION_NR ) ||

					// Current user specification has less relation words than the found specification
					( foundRelationCollectionNr > Constants.NO_COLLECTION_NR &&
					foundRelationCollectionNr != relationCollectionNr &&
					GlobalVariables.nUserRelationWords < WordItem.nRelationWords( foundRelationCollectionNr, foundRelationWordItem ) ) ) )
					{
					if( !foundSpecificationItem.isHiddenSpanishSpecification() )
						{
						if( hasRelationWord )
							{
							hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = true;

							if( writeConfirmedSpecification( ( foundSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : Constants.INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION ), foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the confirmed specification" );
							}
						else
							{
							if( myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, specificationWordItem ) == null )
								{
								hasCurrentlyConfirmedSpecificationButNoRelation_ = true;
								userSpecificationResult.isConfirmedSpecificationButNoneOfItsRelations = true;
								}
							}
						}
					}
				else
					{
					if( hasRelationWord )
						{
						if( !hasFoundRelationWord ||

						( ( relationCollectionNr == Constants.NO_COLLECTION_NR ||
						foundRelationCollectionNr == relationCollectionNr ) &&

						!foundSpecificationItem.isOlderItem() ) )
							// Confirmed relation words
							foundSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( hasFoundRelationWord, false, isNegative, isPossessive, true, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, foundRelationCollectionNr, specificationWordItem, foundRelationWordItem );
						}

					if( foundSpecificationItem == null )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						hasCurrentlyConfirmedSpecification_ &&
						( foundSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, true, specificationWordItem, null ) ) != null &&
						( userSpecificationItem = myWordItem_.firstSpecificationItem( false, isPossessive, false, specificationWordItem ) ) != null )
							{
							// Write confirmed specification
							if( writeConfirmedSpecification( Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_CONFIRMED, foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write another confirmed Spanish specification" );

							if( replaceOrDeleteSpecification( foundSpecificationItem, userSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to replace or delete another confirmed Spanish specification" );
							}
						}
					else
						{
						// Confirmation: Replace a self-generated by a user-entered specification
						if( foundSpecificationItem.isSelfGeneratedSpecification() &&

							( foundRelationWordItem == null ||
							foundRelationWordItem == relationWordItem ||
							foundRelationWordItem.isUserRelationWord ) &&

						( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN ||
						// Typical for Spanish
						// Test file: "reasoning\family\I know (5)"
						!myWordItem_.isNounWordSpanishAmbiguous() ) )
							{
							hasCurrentlyConfirmedSpecification_ = true;
							GlobalVariables.hasConfirmedAnySpecification = true;

							if( !foundSpecificationItem.isHiddenSpanishSpecification() &&
							// Write confirmed specification
							writeConfirmedSpecification( ( foundSpecificationItem.isSelfGeneratedAssumption() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_CONFIRMED : Constants.INTERFACE_LISTING_MY_CONCLUSIONS_THAT_HAVE_BEEN_CONFIRMED ), foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write the confirmed specification" );

							userSpecificationResult.confirmedSpecificationItem = foundSpecificationItem;
							}

						if( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||

						( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						foundSpecificationItem.isSingularNounSpecification() ) )
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

	private UserSpecificationResultType checkUserSpecificationOrQuestion( boolean hasRelationWord, boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isSpecificationWordSpanishAmbiguous, boolean isValueSpecification, short assumptionLevel, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, int specificationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFeminineSingularNounEnding;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSingularNounGeneralizationWord = ( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR );
		SpecificationItem foundSpecificationItem;
		SpecificationItem relatedSpecificationItem;
		WordTypeItem foundWordTypeItem;
		WordTypeItem singularNounWordTypeItem;
		StringBuffer writtenSentenceStringBuffer;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ = false;

		if( specificationWordItem == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		// Check current assignment, specification or question (with relation)
		if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationSpecificationItem( true, true, true, true, true, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) != null &&
		!isArchivedAssignment &&
		!isQuestion &&
		foundSpecificationItem.isArchivedAssignment() &&
		// Check for specification conflict
		// Test file: "Past tense - John was the father of Paul - John is a man"
		checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
			return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );

		if( isQuestion &&
		foundSpecificationItem == null )
			foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

		if( foundSpecificationItem == null )
			{
			// Check current assignment or specification (without this relation)
			if( isQuestion )
				foundSpecificationItem = myWordItem_.firstUserSpecificationItem( isNegative, isPossessive, Constants.NO_COLLECTION_NR, specificationWordItem );
			else
				foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, specificationWordItem, null );

			if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN )
				{
				// Exclusive specifications are not conflicting
				if( !isExclusiveSpecification &&
				// Possessive specifications are not conflicting
				!isPossessive &&
				foundSpecificationItem == null )
					{
					if( !isNegative &&
					!myWordItem_.isFeminineOrMasculineWord() &&
					myWordItem_.hasFeminineProperNounEnding() != specificationWordItem.isFeminineWord() &&
					myWordItem_.hasMasculineProperNounEnding() != specificationWordItem.isMasculineWord() &&
					InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NOUN_TO_BE, myWordItem_.anyWordTypeString(), ( myWordItem_.hasFeminineProperNounEnding() ? Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : Constants.INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != Constants.RESULT_OK )
						return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about a proper noun expected to be feminine or masculine" );

					if( specificationCollectionNr > Constants.NO_COLLECTION_NR )
						{
						if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
							return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

						relatedSpecificationItem = relatedResult.relatedSpecificationItem;

						if( hasRelationWord &&
						relatedSpecificationItem != null &&
						relatedSpecificationItem.isSelfGeneratedAssumption() &&
						specificationWordItem.isExclusiveCollection( specificationCollectionNr ) )
							{
							// Assumption needs to be corrected
							if( correctedAssumptionReplacedSpecificationItem_ != null )
								return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The corrected assumption specification item already assigned" );

							// Write assumption that needs to be corrected
							if( myWordItem_.writeUpdatedSpecification( false, true, false, false, false, false, false, relatedSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write an assumption that needs to be corrected" );

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
							checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check for a specification conflict" );
							}
						}
					}

				if( !hasRelationWord &&
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
				if( !isNegative &&
				isSingularNounGeneralizationWord &&
				!myWordItem_.isFeminineOrMasculineWord() &&
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
			if( !hasRelationWord &&
			isAssignment &&
			generalizationContextNr == Constants.NO_CONTEXT_NR &&
			foundSpecificationItem.hasRelationWord() )
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

		if( isQuestion )
			{
			if( checkUserQuestion( hasRelationWord, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check the user question" );

			// Find self-generated question
			if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationCollectionNr, relationWordItem ) ).result != Constants.RESULT_OK )
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
				if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationCollectionNr, relationWordItem ) ).result != Constants.RESULT_OK )
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
				// Process previously unknown plural noun
				processPreviouslyUnknownPluralNoun( specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to process a previously unknown plural noun" );
				}
			else
				{
				if( ( userSpecificationResult = checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationWordSpanishAmbiguous, isValueSpecification, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, foundSpecificationItem, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to check the user specification" );
				}
			}

		return userSpecificationResult;
		}

	private UserSpecificationResultType findSameSimilarOrRelatedQuestion( boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isOnlyCheckingForConflicts, boolean isPossessive, boolean isSelfGenerated, boolean isSpecificationGeneralization, short assumptionLevel, short questionParameter, int specificationCollectionNr, int relationCollectionNr, WordItem relationWordItem )
		{
		boolean hasFoundOlderSpecification = false;
		boolean hasFoundQuestion;
		boolean hasFoundSpecificationRelationWord = false;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isConflictingQuestion = false;
		boolean isRelatedQuestion = false;
		boolean isSameAssignment = false;
		boolean isSameExclusiveSpecification = false;
		boolean isSameRelation = false;
		boolean isSameQuestion = true;
		boolean isSimilarQuestion = false;
		boolean isUserSpecificationWord;
		SpecificationItem currentQuestionSpecificationItem;
		SpecificationItem foundQuestionSpecificationItem = null;
		SpecificationItem tempSpecificationItem;
		WordItem currentWordItem;
		WordItem tempSpecificationWordItem;
		StringBuffer writtenSentenceStringBuffer;
		UserSpecificationResultType userSpecificationResult = new UserSpecificationResultType();

		if( ( currentWordItem = GlobalVariables.firstWordItem ) == null )
			return myWordItem_.startUserSpecificationResultErrorInWord( 1, moduleNameString_, "The first word item is undefined" );

		// Do for all words, including predefined words
		do	{
			hasFoundQuestion = false;
			isUserSpecificationWord = currentWordItem.isUserSpecificationWord;

			if( ( currentQuestionSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, currentWordItem ) ) == null )
				{
				// Find similar or more specific alternative question
				if( ( currentQuestionSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( !isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, currentWordItem ) ) == null &&
				// Find alternative question without specification collection
				( tempSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, Constants.NO_COLLECTION_NR, currentWordItem ) ) != null &&
				( tempSpecificationWordItem = tempSpecificationItem.specificationWordItem() ) != null &&
				tempSpecificationWordItem.isUserSpecificationWord )
					currentQuestionSpecificationItem = tempSpecificationItem;
				}
			else
				{
				if( hasRelationWord &&
				currentQuestionSpecificationItem.relationWordItem() != relationWordItem &&
				// Test file: "You had same-similar-related question before (with relation)"
				( tempSpecificationItem = myWordItem_.bestMatchingRelationSpecificationItem( true, true, true, true, true, isNegative, isPossessive, ( questionParameter > Constants.NO_QUESTION_PARAMETER ), specificationCollectionNr, relationCollectionNr, currentWordItem, relationWordItem ) ) != null &&
				tempSpecificationItem.hasRelationWord() )
					currentQuestionSpecificationItem = tempSpecificationItem;
				}

			if( currentQuestionSpecificationItem != null )
				{
				hasFoundQuestion = true;

				if( foundQuestionSpecificationItem == null ||
				// Similar or more specific alternative question
				foundQuestionSpecificationItem.isExclusiveSpecification() != currentQuestionSpecificationItem.isExclusiveSpecification() ||

				// Prefer question with user specification word
				( hasRelationWord &&
				( tempSpecificationWordItem = currentQuestionSpecificationItem.specificationWordItem() ) != null &&
				tempSpecificationWordItem.isUserSpecificationWord ) )
					{
					foundQuestionSpecificationItem = currentQuestionSpecificationItem;

					hasFoundOlderSpecification = foundQuestionSpecificationItem.isOlderItem();
					hasFoundSpecificationRelationWord = foundQuestionSpecificationItem.hasRelationWord();
					isSameAssignment = ( foundQuestionSpecificationItem.isAssignment() == isAssignment );
					isSameExclusiveSpecification = ( foundQuestionSpecificationItem.isExclusiveSpecification() == isExclusiveSpecification );
					isSameRelation = ( foundQuestionSpecificationItem.relationCollectionNr() == relationCollectionNr &&
										foundQuestionSpecificationItem.relationWordItem() == relationWordItem &&
										( tempSpecificationWordItem = foundQuestionSpecificationItem.specificationWordItem() ) != null &&
										tempSpecificationWordItem.isUserSpecificationWord );
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

					if( !hasFoundSpecificationRelationWord ||
					isSameRelation )
						{
						if( !isSelfGenerated &&

						( ( isSameRelation &&
						!isSameAssignment ) ||

						// Found user question with different exclusive specification
						foundQuestionSpecificationItem.isExclusiveSpecification() != isExclusiveSpecification ||
						// Found user question with different assumption level
						foundQuestionSpecificationItem.assumptionLevel() != assumptionLevel ) )
							isSameQuestion = false;
						else
							{
							if( !hasFoundSpecificationRelationWord &&
							!isExclusiveSpecification &&

								( !hasFoundOlderSpecification ||
								hasRelationWord ) )
								isConflictingQuestion = true;
							}
						}
					else
						{
						if( foundQuestionSpecificationItem.relationWordItem() == relationWordItem )
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
					( ( !hasFoundSpecificationRelationWord ||
					!isSameRelation ) &&

					foundQuestionSpecificationItem != null ) )
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
					}
				}
			else
				{
				if( hasFoundOlderSpecification &&
				!isOnlyCheckingForConflicts &&
				// Skip displaying announcement if question is in conflict with itself
				GlobalVariables.nUserSpecificationWords == 1 &&
				// Write more specific specification
				writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addUserSpecificationResultErrorInWord( 1, moduleNameString_, "I failed to write a more specific related question" );
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
		isOnlyCheckingForConflicts_ = false;
		isWordTouchedDuringCurrentSentence_ = false;

		compoundSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = Constants.NO_COLLECTION_NR;

		previousConflictingSpecificationItem_ = null;

		spanishPreviousSpecificationWordItem_ = null;
		}

	protected void addMyWordToTouchedDuringCurrentSentenceList()
		{
		WordItem lastTouchedWordItem;
		WordItem tempTouchedWordItem;

		if( !isWordTouchedDuringCurrentSentence_ )
			{
			lastTouchedWordItem = GlobalVariables.firstTouchedWordItem;

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
				hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ );
		}

	protected boolean hasCurrentlyCorrectedAssumptionByKnowledge()
		{
		return hasCurrentlyCorrectedAssumptionByKnowledge_;
		}

	protected boolean hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion()
		{
		return hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_;
		}

	protected boolean hasCurrentlyMoreSpecificSpecification()
		{
		return hasCurrentlyMoreSpecificSpecification_;
		}

	protected int userSpecificationCollectionNr()
		{
		return userSpecificationCollectionNr_;
		}

	protected byte checkForSpecificationConflict( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isUserSpecificationWordSpanishAmbiguous, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isSpecificationWordSpanishAmbiguous;
		int compoundSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int nonCompoundSpecificationCollectionNr;
		SpecificationItem anotherConflictingSpecificationItem;
		SpecificationItem conflictingSpecificationItem = null;
		SpecificationItem foundSpecificationItem;
		SpecificationItem negativeConflictingSpecificationItem = null;
		SpecificationItem previousConflictingSpecificationItem = null;
		SpecificationItem relatedSpecificationItem = null;
		SpecificationItem tempConflictingSpecificationItem;
		WordItem collectionWordItem;
		WordItem relatedSpecificationWordItem;
		RelatedResultType relatedResult;

		if( specificationWordItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		// Typical for Spanish
		if( previousConflictingSpecificationItem_ == null ||

		( ( collectionWordItem = specificationWordItem.collectionWordItem( specificationCollectionNr ) ) != null &&
		!collectionWordItem.isNounWordSpanishAmbiguous() ) )
			{
			if( ( negativeConflictingSpecificationItem = myWordItem_.firstAssignmentOrSpecificationItem( !isNegative, isPossessive, relationCollectionNr, specificationWordItem, relationWordItem ) ) != null &&
			negativeConflictingSpecificationItem.isOlderItem() )
				{
				// Write conflicting negative specification
				if( negativeConflictingSpecificationItem.writeSpecificationConflict( false, true ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the conflicting negative specification" );

				previousConflictingSpecificationItem = negativeConflictingSpecificationItem;
				}

			// Derive conflict
			if( hasSpecificationCollection )
				{
				if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, Constants.NO_CONTEXT_NR, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

				relatedSpecificationItem = relatedResult.relatedSpecificationItem;
				}
			}

		if( relatedSpecificationItem == null )
			{
			// Check for past tense
			if( ( foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, false, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != null &&
			foundSpecificationItem.isArchivedAssignment() != isArchivedAssignment )
				conflictingSpecificationItem = foundSpecificationItem;
			else
				{
				if( !isSpecificationWordSpanishAmbiguous &&
				( compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr() ) > Constants.NO_COLLECTION_NR )
					{
					if( !isNegative &&
					!isUserSpecificationWordSpanishAmbiguous &&
					( foundSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, true, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != null &&
					compoundSpecificationCollectionNr != specificationCollectionNr )
						{
						// First check other conflict
						if( ( anotherConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, null ) ) != null &&
						// Write conflicting specification
						anotherConflictingSpecificationItem.writeSpecificationConflict( false, true ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write another conflicting specification" );
						}
					else
						{
						if( foundSpecificationItem != null &&
						foundSpecificationItem.isUserSpecification() )
							conflictingSpecificationItem = foundSpecificationItem;
						else
							{
							// Check for existing compound specification
							// No self-generated question available for this specification
							if( ( nonCompoundSpecificationCollectionNr = nonCompoundCollectionNrInCollectionWords( compoundSpecificationCollectionNr ) ) > Constants.NO_COLLECTION_NR &&
							( tempConflictingSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, null ) ) != null )
								conflictingSpecificationItem = tempConflictingSpecificationItem;
							}
						}
					}
				}
			}
		else
			{
			if( ( conflictingSpecificationItem = ( isNegative ?
														myWordItem_.bestMatchingSpecificationWordSpecificationItem( true, true, false, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, null ) :
														relatedSpecificationItem ) ) != null &&
			// No conflict found yet
			previousConflictingSpecificationItem_ == null )
				{
				if( !relatedSpecificationItem.hasRelationWord() &&
				conflictingSpecificationItem.isUserSpecification() )
					{
					// Opposite negative of conflicting specification word
					if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, !isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, Constants.NO_CONTEXT_NR, conflictingSpecificationItem.specificationWordItem(), conflictingSpecificationItem.relationWordItem() ) ).result != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
					relatedSpecificationItem.isOlderItem() &&
					( relatedSpecificationWordItem = relatedSpecificationItem.specificationWordItem() ) != null &&
					relatedSpecificationWordItem.isUserSpecificationWord )
						{
						previousConflictingSpecificationItem = relatedSpecificationItem;

						// Write opposite negative conflicting specification
						if( relatedSpecificationItem.writeSpecificationConflict( false, true ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an opposite negative conflicting specification" );
						}
					}
				else
					{
					if( isNegative &&
					// Write conflicting negative specification
					relatedSpecificationItem.writeSpecificationConflict( false, true ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a negative conflicting specification" );
					}
				}
			}

		if( conflictingSpecificationItem != null )
			{
			if( conflictingSpecificationItem.isOlderItem() )
				{
				if( !isNegative &&
				conflictingSpecificationItem != negativeConflictingSpecificationItem &&
				conflictingSpecificationItem != previousConflictingSpecificationItem_ &&
				// Typical for Spanish: Manual conflict tests
				!conflictingSpecificationItem.isHiddenSpanishSpecification() &&

					( previousConflictingSpecificationItem == null ||
					!previousConflictingSpecificationItem.isSpecificationWordSpanishAmbiguous() ||
					GlobalVariables.nUserRelationWords > 0 ) )
					{
					// Write conflicting specification
					if( conflictingSpecificationItem.writeSpecificationConflict( false, true ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a conflicting specification" );

					previousConflictingSpecificationItem_ = conflictingSpecificationItem;
					}
				}
			else
				{
				// Sentence can only be in conflict with itself if it has at least 2 specification words
				if( !conflictingSpecificationItem.hasNonExclusiveSpecificationCollection() &&
				conflictingSpecificationItem.isUserSpecification() &&
				// Current sentence in conflict with definition
				writeSpecificationInConflictWithDefinition( false, conflictingSpecificationItem.specificationWordItem(), specificationWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write that the current sentence is in conflict with a definition" );
				}
			}

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

	protected CreateAndAssignResultType addSpecificationInWord( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nEnteredRelationWords, JustificationItem firstJustificationItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		boolean hasFoundDifferentRelationWords = false;
		boolean hasFoundExclusiveSpecification;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isConfirmedSpecificationButNoneOfItsRelations = false;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isNonExclusiveSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationWordUserGeneralizationWord;
		boolean isSameQuestionFromUser = false;
		boolean isSelfGenerated = ( firstJustificationItem != null );
		boolean isSpecificationFeminineWord;
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUserSpecificationWord;
		boolean isUserRelationWord = myWordItem_.isUserRelationWord;
		int compoundSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int nonCompoundSpecificationCollectionNr;
		int originalSentenceNr;
		JustificationItem attachedJustificationItem;
		JustificationItem foundJustificationItem;
		JustificationItem tempJustificationItem;
		SpecificationItem confirmedSpecificationItem = null;
		SpecificationItem createdSpecificationItem = null;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem = null;
		SpecificationItem foundUserSpecificationItem;
		SpecificationItem obsoleteSpecificationItem = null;
		SpecificationItem relationOppositeSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem existingRelationWordItem;
		WordItem foundRelationWordItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		UserSpecificationResultType userSpecificationResult;

		correctedAssumptionReplacedSpecificationItem_ = null;
		replacedAssignmentItem_ = null;

		// Check generalization word type
		if( !myWordItem_.hasWordType( true, generalizationWordTypeNr ) )
			return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		// Check relation variables:
		// 1) If relation context is defined,
		if( ( hasRelationWord ||
		// 2) or relation word type is defined
		relationWordTypeNr != Constants.NO_WORD_TYPE_NR ||
		// 3) or relation word item is defined
		relationWordItem != null ) &&

		( relationWordItem == null ||
		!relationWordItem.hasWordType( false, relationWordTypeNr ) ) )
			return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The relation variables aren't matching" );

		isRelationWordUserGeneralizationWord = ( relationWordItem == null ? false : relationWordItem.isUserGeneralizationWord );

		if( !isWordTouchedDuringCurrentSentence_ )
			addMyWordToTouchedDuringCurrentSentenceList();

		if( specificationWordItem != null )
			{
			// Check specification word type
			if( !specificationWordItem.hasWordType( true, specificationWordTypeNr ) &&
			// New created language
			!myWordItem_.isLanguageWord() )
				return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification word type" );

			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();
			isUserSpecificationWord = specificationWordItem.isUserSpecificationWord;

			// Collect definition specification without specification collection
			if( !isExclusiveSpecification &&
			!isQuestion &&
			generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
			!specificationWordItem.hasCollection() &&

				// Collect new singular noun specification words
				( ( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
				!specificationWordItem.isOlderItem() ) ||

				// Collect both predefined adjectives 'feminine' and 'masculine'
				specificationWordItem.isAdjectiveFeminineOrMasculine() ) &&

			// Collect simple specifications
			collectSimpleSpecifications( specificationWordTypeNr, generalizationWordTypeNr, specificationWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect simple specifications, with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

			if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
				{
				if( !isSelfGenerated &&
				userSpecificationCollectionNr_ > Constants.NO_COLLECTION_NR )
					// Use previous user specification collection
					specificationCollectionNr = userSpecificationCollectionNr_;
				else
					{
					compoundSpecificationCollectionNr = specificationWordItem.compoundCollectionNr();

					specificationCollectionNr = ( ( ( nonCompoundSpecificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&

													( ( compoundSpecificationCollectionNr == Constants.NO_COLLECTION_NR &&
													// Skip definition sentence with the same generalization and specification word
													!specificationWordItem.isUserGeneralizationWord ) ||

													( !isQuestion &&
													// Non-exclusive specification collection
													!isExclusiveSpecification &&

														( !isSpecificationWordSpanishAmbiguous ||

														( hasRelationWord &&

															( !isUserSpecificationWord ||
															nEnteredRelationWords > 1 ) ) ) ) ) ) ? nonCompoundSpecificationCollectionNr :
																									compoundSpecificationCollectionNr );

					// User specification / question
					if( !isSelfGenerated &&
					specificationCollectionNr != userSpecificationCollectionNr_ )
						// Remember this user specification collection
						userSpecificationCollectionNr_ = specificationCollectionNr;
					}
				}
			else
				{
				if( !specificationWordItem.hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The given specification word doesn't have the given specification collection" );
				}

			// User specification / question
			if( !isSelfGenerated &&
			Item.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				// Check user specification / question
				if( ( userSpecificationResult = checkUserSpecificationOrQuestion( hasRelationWord, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, isSpecificationWordSpanishAmbiguous, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to check a user specification or user question with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

				isConfirmedSpecificationButNoneOfItsRelations = userSpecificationResult.isConfirmedSpecificationButNoneOfItsRelations;
				isNonExclusiveSpecification = userSpecificationResult.isNonExclusiveSpecification;
				isSameQuestionFromUser = userSpecificationResult.isSameQuestionFromUser;

				// Confirmed specification
				confirmedSpecificationItem = userSpecificationResult.confirmedSpecificationItem;
				}

			// Try to find specification
			// Start with finding user specification
			if( ( foundUserSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, ( isGeneralizationProperNoun && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? Constants.NO_COLLECTION_NR : relationCollectionNr ), specificationWordItem, ( isAssignment ? null : relationWordItem ) ) ) == null )
				{
				// User specification
				if( firstJustificationItem == null )
					{
					if( !isAssignment &&
					!isQuestion )
						// Obsolete user specification without specification
						// Test file: "reasoning\family\John - Anna (before family definition)"
						// Spanish test file: "razonamiento\Reto científico"
						obsoleteSpecificationItem = myWordItem_.firstSelfGeneratedCheckSpecificationItem( false, false, isNegative, isPossessive, false, questionParameter, Constants.NO_COLLECTION_NR, relationCollectionNr, specificationWordItem, relationWordItem );
					}
				else	// Self-generated
					{
					if( ( foundSpecificationItem = myWordItem_.bestMatchingRelationSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationCollectionNr, ( isAssignment ? Constants.NO_COLLECTION_NR : relationCollectionNr ), specificationWordItem, ( isAssignment ? null : relationWordItem ) ) ) == null )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						// Spanish test files: "razonamiento\familia\Complejo (5)",
						//						"razonamiento\familia\Complejo (6)",
						//						"razonamiento\familia\Complejo (11)",
						//						"razonamiento\familia\Complejo (19 - mezclado)"
						!isNegative &&
						isPossessive &&
						// Try to find alternative
						( tempSpecificationItem = myWordItem_.bestMatchingRelationSpecificationItem( false, false, false, true, specificationCollectionNr, specificationWordItem ) ) != null &&
						tempSpecificationItem.isSelfGeneratedConclusion() )
							foundSpecificationItem = tempSpecificationItem;
						}
					else
						{
						if( isExclusiveSpecification &&
						!foundSpecificationItem.isExclusiveSpecification() &&
						myWordItem_.firstExclusiveSpecificationItem( specificationWordItem ) != null )
							return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "An exclusive specification was requested. But I selected a non-exclusive specification instead" );
						}

					// Not found specification
					if( foundSpecificationItem == null )
						{
						if( !isQuestion &&
						( existingSpecificationItem = myWordItem_.bestMatchingRelationSpecificationItem( isArchivedAssignment, isNegative, isPossessive, specificationWordItem ) ) != null )
							{
							if( relationWordItem == null )
								{
								if( isNegative )
									// Test file: "reasoning\Scientific challenge"
									foundSpecificationItem = existingSpecificationItem;
								}
							else
								{
								if( !isSpecificationWordSpanishAmbiguous ||
								// Typical for Spanish
								!existingSpecificationItem.isHiddenSpanishSpecification() )
									{
									if( ( existingRelationWordItem = existingSpecificationItem.relationWordItem() ) != null &&
									// Typical for Spanish
									isSpecificationWordSpanishAmbiguous )
										{
										// Non-possessive
										if( !isPossessive )
											{
											if( existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
											!existingSpecificationItem.hasOnlyOneRelationWord() )
												// Spanish test files: "razonamiento\familia\Complejo (2)",
												//						"razonamiento\familia\Complejo (12)",
												//						"razonamiento\familia\Complejo (15)"
												foundSpecificationItem = existingSpecificationItem;
											}
										else	// Possessive
											{
											// Efficiency
											if( isArchivedAssignment ||
											// Spanish test files: "conflicto\familia\José es una madre",
											//						"razonamiento\familia\Complejo (9)",
											//						"razonamiento\familia\Complejo (15)",
											//						"razonamiento\familia\Complejo (19 - extraño)",
											//						"razonamiento\familia\Complejo (20)",
											//						among others
											relationCollectionNr > Constants.NO_COLLECTION_NR ||

											( existingSpecificationItem.isOlderItem() &&

												// Same relation word
												( ( existingRelationWordItem == relationWordItem &&
												!existingSpecificationItem.hasPrimaryActiveAssignmentJustification() &&
												( foundJustificationItem = existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) != null &&

													// Spanish test files: "razonamiento\familia\Complejo (16)",
													//						"razonamiento\familia\Juan - Ana (antes de la definición de la familia)",
													//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
													//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa"
													//						"razonamiento\familia\Paz es un hijo - Juan y Ana"
													( foundJustificationItem.isOlderItem() ||

													// Spanish test files: "razonamiento\familia\Complejo (6)",
													//						"razonamiento\familia\Complejo (11)",
													//						"razonamiento\familia\José tiene 2 padres - José es hijo"
													( ( attachedJustificationItem = foundJustificationItem.attachedJustificationItem() ) != null &&
													!attachedJustificationItem.isReversibleAssumptionOrConclusion() ) ) ) ||

												// Different relation word
												// Spanish test files: "conflicto\familia\José es una madre",
												//						"razonamiento\familia\Complejo (6)",
												//						"razonamiento\familia\Complejo (9)",
												//						"razonamiento\familia\Complejo (11)",
												//						"razonamiento\familia\Complejo (12)",
												//						"razonamiento\familia\Complejo (13)",
												//						"razonamiento\familia\Complejo (14)",
												//						among others
												( existingRelationWordItem != relationWordItem &&
												!existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) )
												foundSpecificationItem = existingSpecificationItem;
											}
										}	
									else
										{
										// Not user specification word
										// Test file: "reasoning\family\John - Anna (before family definition)"
										// Spanish test files: "razonamiento\familia\Complejo (18)",
										//						"razonamiento\familia\Complejo (19 - mezclado)",
										//						"razonamiento\familia\Complejo (19 - extraño)",
										if( ( !isUserSpecificationWord &&
										isRelationWordUserGeneralizationWord &&
										existingRelationWordItem != null ) ||

										// User specification word
										// Test file: "reasoning\family\John - Anna (before family definition)"
										( isUserSpecificationWord &&
										existingSpecificationItem.hasRelationCollection() ) )
											foundSpecificationItem = existingSpecificationItem;
										else
											{
											// Test file: "reasoning\family\Complex (9)",
											//				"reasoning\family\Complex (16)",
											//				"reasoning\family\I know (11)",
											//				"reasoning\family\John is a father - Paul has 2 parents",
											//				among others
											if( ( !isSpecificationWordSpanishAmbiguous ||

											// Typical for Spanish
											// Spanish test file: "razonamiento\familia\Sé (11)"
											!isPossessive ||

											// Spanish test file: "razonamiento\familia\Complejo (10)",
											//						"razonamiento\familia\Complejo (19 - mezclado)",
											//						"razonamiento\familia\Complejo (19 - extraño)",
											//						"razonamiento\familia\Mis conclusiones que se confirman (sin relación)",
											//						"razonamiento\familia\Mis suposiciones que se confirman (Paz, José y Luisa)"
											!isUserRelationWord ||

											// Spanish test file: "razonamiento\familia\Sé (11)"
											isUserSpecificationWord ) &&

											myWordItem_.noRelationWordSpecificationItem( isPossessive, true, specificationWordItem ) != null )
												{
												if( confirmedSpecificationItem != null )
													return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "The confirmed specification item is already defined" );

												// Write an assumption that has relation words now
												if( myWordItem_.writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, false, false, false, true, false, false, existingSpecificationItem ) != Constants.RESULT_OK )
													return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to write an assumption that has relation words now" );

												// Avoid replacing by specification with higher assumption level
												if( existingSpecificationItem.assumptionLevel() >= assumptionLevel )
													confirmedSpecificationItem = existingSpecificationItem;
												}
											}
										}
									}
								}
							}
						}
					else	// Found specification
						{
						if( isPossessive &&
						!isSpecificationWordSpanishAmbiguous &&
						hasCurrentlyConfirmedSpecification_ &&
						foundSpecificationItem.isSelfGeneratedConclusion() &&
						// Current user assignment
						myWordItem_.firstAssignmentItem( false, false, false ) != null )
							{
							// Conclusion becomes assumption
							// Test file: "reasoning\family\Joe has 2 parents - Joe is a child",
							//				"reasoning\family\Joe is a child - Joe has 2 parents",
							//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
							if( firstJustificationItem.changeAttachedJustification( foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to change the attached justification item of the given first justification item" );

							obsoleteSpecificationItem = foundSpecificationItem;
							foundSpecificationItem = null;
							}
						else
							{
							if( relationWordItem != null &&
							foundSpecificationItem.isOlderItem() &&
							( foundRelationWordItem = foundSpecificationItem.relationWordItem() ) != null &&
							( foundJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null )
								{
								relationOppositeSpecificationItem = relationWordItem.firstSpecificationItem( isNegative, !isPossessive, isQuestion, specificationWordItem );

								// No relation collection
								if( ( relationCollectionNr == Constants.NO_COLLECTION_NR &&
								isRelationWordUserGeneralizationWord &&
								relationOppositeSpecificationItem != null &&

									// Same relation word
									// Test files: "reasoning\family\Complex (15)",
									//				"reasoning\family\Complex (16)",
									//				"reasoning\family\I know (3)",
									//				"reasoning\family\I know (10)",
									//				among others
									( ( foundRelationWordItem == relationWordItem &&
									foundSpecificationItem.hasRelationCollection() &&
									relationOppositeSpecificationItem.hasRelationWord() &&
									relationOppositeSpecificationItem.isUserSpecification() ) ||

									// Different relation word
									( foundRelationWordItem != relationWordItem &&

										// Non-possessive
										// Test files: "reasoning\family\Avoid duplicate context",
										//				"reasoning\family\Complex (7 - Becky)",
										//				"reasoning\family\Complex (7 - Claudia)",
										//				"reasoning\family\I know (4)",
										//				"reasoning\family\I know (3)",
										//				"reasoning\family\I know (10)",
										//				among others
										( ( !isPossessive &&
										!isSpecificationWordSpanishAmbiguous &&
										relationOppositeSpecificationItem.isUserSpecification() ) ||

										// Possessive
										// Test file: "Anna was the mother of Paul, Joe and Laura"
										( isPossessive &&
										isArchivedAssignment ) ) ) ) ) ||

								// Relation collection
								// Test file: "reasoning\family\Complex (4)"
								( relationCollectionNr > Constants.NO_COLLECTION_NR &&
								isPossessive &&
								foundRelationWordItem != relationWordItem &&
								!isSpecificationWordSpanishAmbiguous &&
								!isRelationWordUserGeneralizationWord &&
								relationOppositeSpecificationItem == null &&
								foundJustificationItem.isOlderItem() &&
								foundJustificationItem.isOppositePossessiveSpecificationAssumption() ) )
									hasFoundDifferentRelationWords = true;
								}
							}
						}
					}
				}
			else
				{
				if( isSpecificationGeneralization &&
				// Spanish ambiguous specification collection
				myWordItem_ == specificationWordItem )
					// Spanish test file: "razonamiento\Recopilar más tarde - Cada padre es un padre y cada madre es un padre"
					specificationCollectionNr = foundUserSpecificationItem.generalizationCollectionNr();
				else
					{
					if( !isSelfGenerated ||
					!foundUserSpecificationItem.isExclusiveSpecification() )
						{
						foundSpecificationItem = foundUserSpecificationItem;

						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						hasRelationWord &&
						isQuestion &&
						foundUserSpecificationItem.hasRelationWord() )
							// Don't create question
							// Spanish test file: "respuestas a las preguntas\familia\	Relación pregunta es subconjunto de relación de respuesta"
							foundSpecificationItem = null;
						else
							{
							if( confirmedSpecificationItem != null &&
							!confirmedSpecificationItem.hasRelationWord() &&
							foundSpecificationItem.hasRelationWord() )
								{
								// Test file: "reasoning\family\Complex (3)"
								// Replace confirmed specification without relation word by found specification
								if( replaceOrDeleteSpecification( confirmedSpecificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace the confirmed specification without relation word by the found specification" );

								confirmedSpecificationItem = null;
								}
							else
								{
								// Typical for Spanish
								if( hasRelationWord &&
								!isPossessive &&
								!isSelfGenerated &&
								specificationCollectionNr > Constants.NO_COLLECTION_NR &&
								!foundSpecificationItem.hasSpecificationCollection() )
									{
									// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
									confirmedSpecificationItem = foundUserSpecificationItem;
									spanishPreviousSpecificationWordItem_ = specificationWordItem;
									}
								}
							}
						}
					else
						{
						if( compoundSpecificationCollectionNr_ == Constants.NO_COLLECTION_NR )
							{
							if( isExclusiveSpecification )
								{
								if( ( specificationCollectionNr = WordItem.highestCollectionNrInCollectionWords() ) >= Constants.MAX_COLLECTION_NR )
									return myWordItem_.startCreateAndAssignResultSystemErrorInWord( 1, moduleNameString_, "Collection number overflow" );

								// Test files (incrementing specificationCollectionNr): "reasoning/The only option left - Boat",
								//														"reasoning/The only option left - Option"
								// and Spanish test files
								compoundSpecificationCollectionNr_ = ++specificationCollectionNr;

								// Typical for Spanish
								if( spanishPreviousSpecificationWordItem_ == null )
									spanishPreviousSpecificationWordItem_ = specificationWordItem;
								}
							else
								// Typical for Spanish
								foundSpecificationItem = myWordItem_.firstSpecificationItem( true, false, false, specificationWordItem );
							}
						else
							{
							// Typical for Spanish
							if( spanishPreviousSpecificationWordItem_ != null &&
							spanishPreviousSpecificationWordItem_ != specificationWordItem &&
							// Avoid looping
							specificationWordItem.collectionNr( myWordItem_ ) != compoundSpecificationCollectionNr_ )
								{
								specificationCollectionNr = compoundSpecificationCollectionNr_;

								// Each collection comes in pairs
								if( spanishPreviousSpecificationWordItem_.createCollectionItem( true, isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect previous word \"" + spanishPreviousSpecificationWordItem_.anyWordTypeString() + "\" with curent word \"" + specificationWordItem.anyWordTypeString() + "\"" );

								if( specificationWordItem.createCollectionItem( false, isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishPreviousSpecificationWordItem_, myWordItem_, myWordItem_ ).result != Constants.RESULT_OK )
									return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to collect current word \"" + specificationWordItem.anyWordTypeString() + "\" with previous word \"" + spanishPreviousSpecificationWordItem_.anyWordTypeString() + "\"" );
								}
							}
						}
					}
				}

			// Add generalization items
			if( hasRelationWord ||
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

		hasFoundExclusiveSpecification = ( foundSpecificationItem == null ? false : foundSpecificationItem.isExclusiveSpecification() );

		// No conflict found
		if( !GlobalVariables.hasDisplayedWarning &&

			// Not found
			( foundSpecificationItem == null ||

			// Accept different relation words (e.g. ambiguous specification)
			// Test files: "reasoning\family\Anna was the mother of Paul, Joe and Laura",
			//				"reasoning\family\Avoid duplicate context",
			//				"reasoning\family\Complex (4)",
			//				among others
			hasFoundDifferentRelationWords ||

			// Test files: "conflict\family\Question in conflict with itself (with knowledge)",
			//				"reasoning\family\This information is more specific (non-exclusive)"
			isNonExclusiveSpecification ||

			// Test files: "reasoning\Knowledge of organisms (2)",
			//				"reasoning\family\Complex (4)",
			//				"reasoning\family\Complex (5)",
			//				"reasoning\family\Complex (6)",
			//				"reasoning\family\Complex (9)",
			//				"reasoning\family\Complex (18)",
			//				"reasoning\family\I know (10)",
			//				"reasoning\family\I know (11)",
			//				"reasoning\family\My assumptions that are confirmed (Paul and John)"
			confirmedSpecificationItem != null ||

			// Overwrite non-exclusive specification by exclusive one
			// Test files: "ambiguity\Boston",
			//				"ambiguity\Duplicates",
			//				"question answering\family\Same and similar questions",
			//				"question answering\family\You had same-similar question before (without relation)",
			//				"reasoning\John is a parent of Paul - John is a parent of Joe",
			//				"reasoning\Knowledge of organisms (1)",
			//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
			( !hasFoundExclusiveSpecification &&
			isExclusiveSpecification ) ||

			// Overwrite user questions by non-exclusive one
			// Test files: "question answering\This question is more specific (without relation)",
			//				"question answering\family\Same and similar questions",
			//				"question answering\family\This question is more specific (with relation)",
			//				"question answering\family\You had same-similar question before (without relation)"
			( hasFoundExclusiveSpecification &&
			!isAssignment &&
			isQuestion &&
			!isSelfGenerated &&
			!isSameQuestionFromUser ) ||

			// Overwrite non-conditional specification by conditional one
			// Test file: "programming\Connect-Four - Display information about the set"
			( isConditional &&
			!foundSpecificationItem.isConditional() ) ||

			( !isExclusiveSpecification &&
			relationCollectionNr > Constants.NO_COLLECTION_NR &&
			relationWordItem != null &&
			( foundRelationWordItem = foundSpecificationItem.relationWordItem() ) != null &&
			foundRelationWordItem != relationWordItem &&

				// User specification
				( ( foundSpecificationItem.isUserSpecification() &&

					// Test file: "reasoning\John is a parent of Paul - Joe has 2 parents"
					( isSelfGenerated ||

					// Test file: "reasoning\Paul has a parent, called John - Paul has a parent, called Anna"
					( isPossessive &&
					myWordItem_.bestMatchingRelationSpecificationItem( false, false, isNegative, isPossessive, specificationWordItem, relationWordItem ) == null ) ) ) ||

				// Self-generated specification
				( firstJustificationItem != null &&
				!firstJustificationItem.isOlderItem() &&
				foundSpecificationItem.isOlderItem() &&
				( foundJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null &&

					// Non-possessive
					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				"reasoning\family\Complex (4)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				among others
					( ( !isPossessive &&
					!isSpecificationWordSpanishAmbiguous &&
					myWordItem_.bestMatchingRelationSpecificationItem( false, false, isNegative, isPossessive, specificationWordItem, relationWordItem ) == null ) ||

					// Possessive
					( isPossessive &&
					// Relation opposite specification
					( relationOppositeSpecificationItem = relationWordItem.firstSpecificationItem( isNegative, false, isQuestion, specificationWordItem ) ) != null &&

						( ( !isSpecificationWordSpanishAmbiguous &&
						firstJustificationItem.isReversibleAssumptionOrConclusion() &&
						relationOppositeSpecificationItem.isSelfGeneratedSpecification() &&

							// Relation opposite specification is not older
							( ( !relationOppositeSpecificationItem.isOlderItem() &&

								// Relation word is not user generalization word
								// Test files: "reasoning\family\Complex (20)",
								//				"reasoning\family\John - Laura - John and Anna",
								//				"reasoning\family\John is a parent - Paul - Laura",
								//				"reasoning\family\John is the father - Paul - Laura"
								( ( !isRelationWordUserGeneralizationWord &&
								!foundSpecificationItem.hasRelationCollection() ) ||

								// Relation word is user generalization word
								( isRelationWordUserGeneralizationWord &&
								foundJustificationItem.isOlderItem() &&

									// Found justification is not current
									// Test files: "reasoning\family\Complex (11)",
									//				"reasoning\family\Laura - Paul - John and Anna",
									//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul",
									//				"reasoning\family\Paul - Joe - John and Anna"
									( ( !foundJustificationItem.hasCurrentCreationSentenceNr() &&
									foundSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ||

									// Found justification is current
									( foundJustificationItem.hasCurrentCreationSentenceNr() &&

										// Test files: "reasoning\family\Complex (7 - Becky)",
										//				"reasoning\family\Complex (7 - Claudia)",
										//				"reasoning\family\Complex (18)",
										//				"reasoning\family\Complex (19 - mixed)",
										//				"reasoning\family\Complex (19 - strange)",
										//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
										( ( foundJustificationItem.isOppositePossessiveSpecificationAssumption() &&
										foundRelationWordItem.firstAdjectiveSpecificationItem( false, false ) != null ) ||

										// Reversible assumption or conclusion
										// Test file: "reasoning\family\Complex (2)"
										( foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == null &&
										relationWordItem.firstAdjectiveSpecificationItem( false, false ) == null &&
										( tempJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) != null &&
										!tempJustificationItem.hasCurrentCreationSentenceNr() ) ) ) ) ) ) ) ||

							// Relation opposite specification is older
							// Test files: "reasoning\family\Complex (4)",
							//				"reasoning\family\Complex (5)",
							//				"reasoning\family\Complex (6)",
							//				"reasoning\family\John - Laura - John and Anna"
							( relationOppositeSpecificationItem.isOlderItem() &&
							foundSpecificationItem.hasOnlyOneRelationWord() &&
							foundSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) ||

						// Typical for Spanish
						( isSpecificationWordSpanishAmbiguous &&

							// Not hidden Spanish specification
							( ( !foundSpecificationItem.isHiddenSpanishSpecification() &&

								// Relation opposite self-generated specification
								// Spanish test files: "razonamiento\familia\Complejo (6)",
								//						"razonamiento\familia\Complejo (11)",
								//						"razonamiento\familia\Complejo (20)"
								( ( relationOppositeSpecificationItem.isSelfGeneratedSpecification() &&
								!foundSpecificationItem.hasRelationCollection() ) ||

								// Relation opposite user specification
								( relationOppositeSpecificationItem.isUserSpecification() &&
								// Spanish test files: "razonamiento\familia\Sé (3)",
								//						"razonamiento\familia\Mis suposiciones que se confirman (Paz, José y Luisa)",
								//						"razonamiento\familia\Paz es el hijo Juan y Ana"
								foundSpecificationItem.hasCompoundSpecificationCollection() &&
								!foundSpecificationItem.hasRelationCollection() ) ) ||

							// Hidden Spanish specification
							( foundSpecificationItem.isHiddenSpanishSpecification() &&
							// Opposite possessive specification assumption
							firstJustificationItem.hasSecondaryUserSpecification() &&

								// Spanish test file: "razonamiento\familia\Paz - Ana - José - Luisa - Juan"
								( !relationOppositeSpecificationItem.isHiddenSpanishSpecification() ||

								// Spanish test files: "razonamiento\familia\Complejo (14)",
								//						"razonamiento\familia\Sé (11)",
								//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
								( !isRelationWordUserGeneralizationWord &&
								!foundSpecificationItem.hasCurrentCreationSentenceNr() &&
								foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != null &&
								!relationOppositeSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) ) ) ) ) ) ) ) ) ) ) )
			{
			if( !isQuestion &&
			!isSelfGenerated &&
			foundSpecificationItem != null )
				{
				if( hasFoundExclusiveSpecification )
					// Test files: "programming\Connect-Four - Column is full",
					//				"programming\Connect-Four - Display information about the set",
					//				"programming\Connect-Four - Eager to win",
					//				"programming\Tower of Hanoi (even number of discs)",
					//				"programming\Tower of Hanoi (odd number of discs)"
					isExclusiveSpecification = true;

				if( foundSpecificationItem != confirmedSpecificationItem )
					// Test files: "ambiguity\Boston",
					//				"ambiguity\Duplicates",
					//				"conflict\family\Question in conflict with itself (with knowledge)",
					//				"programming\Connect-Four - Display information about the set",
					//				"reasoning\John is a parent of Paul - John is a parent of Joe",
					//				"reasoning\Paul has a parent, called John - Paul has a parent, called Anna",
					//				"reasoning\family\Complex (15)",
					//				among others
					obsoleteSpecificationItem = foundSpecificationItem;
				}

			if( isExclusiveSpecification &&
			isNonExclusiveSpecification )
				// Test files: "conflict\family\Question in conflict with itself (with knowledge)",
				//				"reasoning\family\This information is more specific (non-exclusive)"
				isExclusiveSpecification = false;

			originalSentenceNr = ( hasRelationWord ||
									isQuestion ||
									foundSpecificationItem == null ? GlobalVariables.currentSentenceNr :
																	foundSpecificationItem.originalSentenceNr() );

			// Create the actual specification
			if( ( createAndAssignResult = myWordItem_.createSpecificationItem( false, false, false, false, isCharacteristicFor, isConditional, ( hasCurrentlyCorrectedAssumptionByKnowledge_ || hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ ), isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, Constants.NO_ASSIGNMENT_LEVEL, assumptionLevel, GlobalVariables.currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? Constants.NO_WORD_TYPE_NR : relationWordTypeNr ), specificationCollectionNr, ( isAssignment ? Constants.NO_COLLECTION_NR : relationCollectionNr ), generalizationContextNr, Constants.NO_CONTEXT_NR, ( isAssignment ? Constants.NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, ( isAssignment ? 0 : nEnteredRelationWords ), firstJustificationItem, specificationWordItem, ( isAssignment ? null : relationWordItem ), specificationString, null, null ) ).result != Constants.RESULT_OK )
				return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to create a specification item" );

			if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
				return myWordItem_.startCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I couldn't create a specification item" );

			// Test files: "reasoning\family\Anna - John - Paul, Joe and Laura",
			//				"reasoning\family\Complex (1)",
			//				"reasoning\family\Complex (9)",
			//				"reasoning\family\Complex (10)",
			//				"reasoning\family\Complex (16)",
			//				among others
			if( ( isConfirmedSpecificationButNoneOfItsRelations ||

			// Test files: "ambiguity\Boston",
			//				"ambiguity\Duplicates",
			//				"conflict\family\Question in conflict with itself (with knowledge)",
			//				"programming\Connect-Four - Display information about the set",
			//				"reasoning\John is a parent of Paul - John is a parent of Joe",
			//				"reasoning\Paul has a parent, called John - Paul has a parent, called Anna",
			//				"reasoning\family\Complex (15)",
			//				among others
			obsoleteSpecificationItem != null ||

			// Test files: "reasoning\family\Correcting invalidated assumption (by knowledge)",
			//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
			correctedAssumptionReplacedSpecificationItem_ != null ||

			( confirmedSpecificationItem != null &&

				// Efficiency
				( !isSpecificationWordSpanishAmbiguous ||

				// Typical for Spanish
				// Spanish test files: "conflicto\familia\José es una madre",
				//						"respuestas a las preguntas\familia\Contestador inteligente de preguntas",
				//						"respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)",
				//						"respuestas a las preguntas\familia\Tenías mismo-similar-relacionado pregunta antes (con relación)",
				//						"razonamiento\Reto científico",
				//						"razonamiento\familia\Complejo (1)",
				//						"razonamiento\familia\Complejo (2)",
				//						among others
				!createdSpecificationItem.isHiddenSpanishSpecification() ) ) ) &&

			// Now replace obsolete specifications by the created specification
			replaceObsoleteSpecification( isConfirmedSpecificationButNoneOfItsRelations, confirmedSpecificationItem, createdSpecificationItem, obsoleteSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace an obsolete specification" );

			if( isGeneralizationProperNoun &&
			firstJustificationItem != null &&
			foundSpecificationItem != null &&

				// Non-possessive
				// Test file: "reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
				( ( !isPossessive &&
				foundSpecificationItem.isSelfGeneratedAssumption() &&
				firstJustificationItem.isSpecificationSubstitutionAssumptionOrConclusion() ) ||

				// Possessive
				( isPossessive &&

					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (4)",
					//				"reasoning\family\Complex (7 - Becky)",
					//				"reasoning\family\Complex (7 - Claudia)",
					//				among others
					( ( ( foundJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null &&
					foundJustificationItem.isOlderItem() ) ||

					// Typical for Spanish
					// Spanish test files: "razonamiento\familia\Complejo (6)",
					//						"razonamiento\familia\Complejo (11)",
					//						"razonamiento\familia\Juan - Paz - Luisa",
					( isSpecificationWordSpanishAmbiguous &&
					foundSpecificationItem.isSelfGeneratedConclusion() ) ) ) ) )
				{
				if( firstJustificationItem != foundSpecificationItem.firstJustificationItem() )
					{
					if( isPossessive &&
					foundSpecificationItem.hasCurrentCreationSentenceNr() )
						{
						// Test files: "reasoning\family\Complex (2)",
						//				"reasoning\family\Complex (7 - Becky)",
						//				"reasoning\family\Complex (7 - Claudia)",
						//				"reasoning\family\Complex (11)",
						//				"reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)",
						//				among others
						if( createdSpecificationItem.changeFirstJustification( true, foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
							return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace the justifications of the created specification by the justifications of the found specification" );
						}
					else
						{
						// Change attached justification
						// Test files: "reasoning\family\Complex (4)",
						//				"reasoning\family\Complex (5)",
						//				"reasoning\family\Complex (6)",
						//				"reasoning\family\John - Laura - John and Anna",
						//				"reasoning\family\John is a parent - Paul - Laura",
						//				"reasoning\family\John is the father - Paul - Laura",
						if( firstJustificationItem.changeAttachedJustification( foundSpecificationItem.firstJustificationItem() ) != Constants.RESULT_OK )
							return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
						}
					}

				// Replace or delete found specification with copied relation context
				// Test files: "reasoning\family\Complex (2)",
				//				"reasoning\family\Complex (4)",
				//				"reasoning\family\Complex (5)",
				//				"reasoning\family\Complex (6)",
				//				among others
				if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );
				}

			if( !isNegative &&
			!isPossessive &&
			!isQuestion &&
			specificationWordItem != null &&

				( !isSpecificationWordSpanishAmbiguous ||
				// Typical for Spanish
				!createdSpecificationItem.isHiddenSpanishSpecification() ) )
				{
				if( !isAssignment &&
				// Find answers to questions
				// Check user assignments after the assignment is created. Class: AdminSpecification, method: addSpecification()
				myWordItem_.findAnswersToQuestions( specificationWordItem.compoundCollectionNr(), createdSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to find answers to questions" );

				if( !isSelfGenerated &&
				!myWordItem_.isFeminineOrMasculineWord() &&
				specificationWordItem.isFeminineOrMasculineWord() )
					{
					isSpecificationFeminineWord = specificationWordItem.isFeminineWord();

					// Definition specification
					if( !isExclusiveSpecification ||
					hasOnlyEitherFeminineOrMasculineSpecificationWords( isSpecificationFeminineWord ) )
						{
						if( isSpecificationFeminineWord )
							{
							if( myWordItem_.markWordAsFeminine() != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as feminine" );
							}
						else
							{
							if( myWordItem_.markWordAsMasculine() != Constants.RESULT_OK )
								return myWordItem_.addCreateAndAssignResultErrorInWord( 1, moduleNameString_, "I failed to mark my word as masculine" );
							}
						}
					}
				}
			}

		if( ( createAndAssignResult.createdOrFoundSpecificationItem = createdSpecificationItem ) == null )
			createAndAssignResult.createdOrFoundSpecificationItem = foundSpecificationItem;

		createAndAssignResult.foundSpecificationItem = foundSpecificationItem;
		createAndAssignResult.replacedAssignmentItem = replacedAssignmentItem_;
		return createAndAssignResult;
		}

	protected RelatedResultType findRelatedSpecification( boolean isCheckingRelationContext, SpecificationItem searchSpecificationItem )
		{
		if( searchSpecificationItem == null )
			return myWordItem_.startRelatedResultErrorInWord( 1, moduleNameString_, "The given search specification item is undefined" );

		return findRelatedSpecification( isCheckingRelationContext, false, false, searchSpecificationItem.isAssignment(), searchSpecificationItem.isInactiveAssignment(), searchSpecificationItem.isArchivedAssignment(), searchSpecificationItem.isExclusiveSpecification(), searchSpecificationItem.isNegative(), searchSpecificationItem.isPossessive(), searchSpecificationItem.questionParameter(), searchSpecificationItem.specificationWordTypeNr(), searchSpecificationItem.specificationCollectionNr(), searchSpecificationItem.relationCollectionNr(), searchSpecificationItem.generalizationContextNr(), searchSpecificationItem.specificationWordItem(), searchSpecificationItem.relationWordItem() );
		}

	protected RelatedResultType findRelatedSpecification( boolean isIgnoringExclusive, boolean isIgnoringNegative, boolean isIncludingAssignments, boolean isIncludingArchivedAssignments, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short questionParameter, short specificationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		RelatedResultType relatedResult = new RelatedResultType();

		if( isIncludingAssignments )
			{
			if( ( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			relatedResult.relatedSpecificationItem == null &&
			// Archived assignments
			( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
				return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification as well" );
			}

		// Specifications
		if( relatedResult.relatedSpecificationItem == null &&
		( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
			return myWordItem_.addRelatedResultErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );

		return relatedResult;
		}
	}

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
