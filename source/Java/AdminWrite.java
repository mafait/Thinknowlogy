/*	Class:			AdminWrite
 *	Supports class:	AdminItem
 *	Purpose:		To write selected specifications as sentences
 *	Version:		Thinknowlogy 2018r1 (ShangDi 上帝)
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

class AdminWrite
	{
	// Private constructed variables

	private boolean hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_;
	private boolean isFirstSelfGeneratedAssumption_;
	private boolean isFirstSelfGeneratedConclusion_;
	private boolean isFirstSelfGeneratedQuestion_;
	private boolean isFirstUserQuestion_;
	private boolean isFirstUserSpecifications_;
	private boolean isWritingSeparator_;

	private SpecificationItem previousPrimarySpecificationItem_;


	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


	// Private methods

	private byte displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence()
		{
		ReadItem currentReadItem = adminItem_.firstActiveReadItem();
		String readWordTypeString;

		do	{
			if( !currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence &&
			( readWordTypeString = currentReadItem.readWordTypeString() ) != null &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INDENTED, ( readWordTypeString + " (position: " + currentReadItem.wordOrderNr() + ", word type: " + adminItem_.wordTypeNameString( currentReadItem.wordTypeNr() ) + ", word parameter: " + currentReadItem.wordParameter() + ", grammar parameter: " + currentReadItem.grammarParameter + ")" ) ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write a justification sentence" );
			}
		while( ( currentReadItem = currentReadItem.nextReadItem() ) != null );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationSpecification( SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean isFirstJustificationType = true;
		JustificationItem currentJustificationItem;
		JustificationItem firstJustificationItem;
		WordItem generalizationWordItem;

		if( selfGeneratedSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given self-generated specification item is undefined" );

		if( ( firstJustificationItem = selfGeneratedSpecificationItem.firstJustificationItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );

		if( ( generalizationWordItem = selfGeneratedSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );

		isWritingSeparator_ = true;

		generalizationWordItem.clearJustificationHasBeenWritten();
		currentJustificationItem = firstJustificationItem;

		do	{
			if( writeJustificationType( isFirstJustificationType, currentJustificationItem, selfGeneratedSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write a justification type of a specification" );

			isFirstJustificationType = false;
			}
		while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != null );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationSpecifications( boolean isWritingPrimarySpecification, boolean isWritingExtraSeparator, JustificationItem startJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean isExclusiveSecondarySpecification;
		boolean isFirstTime = true;
		boolean isHidingAlmostDuplicateSpanishSpecification;
		boolean isOnlyWritingPrimarySpecification;
		boolean isQuestionJustification;
		boolean isStop = false;
		JustificationItem currentJustificationItem;
		SpecificationItem previousSecondarySpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		WordItem generalizationWordItem;

		if( startJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given start justification item is undefined" );

		currentJustificationItem = startJustificationItem;

		do	{
			isExclusiveSecondarySpecification = false;
			isHidingAlmostDuplicateSpanishSpecification = false;
			isOnlyWritingPrimarySpecification = false;
			isQuestionJustification = currentJustificationItem.isQuestionJustification();

			primarySpecificationItem = currentJustificationItem.primarySpecificationItem();

			if( ( secondarySpecificationItem = currentJustificationItem.secondarySpecificationItem() ) != null &&
			secondarySpecificationItem.isExclusiveSpecification() )
				isExclusiveSecondarySpecification = true;

			if( !isQuestionJustification &&
			primarySpecificationItem != null &&
			primarySpecificationItem.hasRelationContext() )
				{
				isWritingPrimarySpecification = true;
				isOnlyWritingPrimarySpecification = ( currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() != null );

				if( primarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&
				primarySpecificationItem.hasNonCompoundSpecificationCollection() &&
				primarySpecificationItem.isPossessive() &&
				primarySpecificationItem.isSelfGeneratedConclusion() &&
				!primarySpecificationItem.isConcludedAssumption() &&
				primarySpecificationItem.hasOnlyOneRelationWord() )
					{
					isHidingAlmostDuplicateSpanishSpecification = true;
					isWritingSeparator_ = false;
					}
				}

			if( !isHidingAlmostDuplicateSpanishSpecification )
				{
				if( isWritingPrimarySpecification ||

				( ( !isQuestionJustification &&

				( primarySpecificationItem == null ||

				( isFirstTime &&
				primarySpecificationItem != previousPrimarySpecificationItem_ ) ) ) ||

				( !isExclusiveSecondarySpecification &&
				primarySpecificationItem != null &&
				secondarySpecificationItem != null &&
				previousPrimarySpecificationItem_ != null &&
				!primarySpecificationItem.isCorrectedAssumption() &&
				secondarySpecificationItem != previousSecondarySpecificationItem &&
				primarySpecificationItem.specificationCollectionNr() == previousPrimarySpecificationItem_.specificationCollectionNr() &&

				// Skip possessive secondary specifications without relation
				( !secondarySpecificationItem.isPossessive() ||
				secondarySpecificationItem.hasRelationContext() ) ) ) )
					{
					if( isWritingExtraSeparator &&
					InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_JUSTIFICATION_AND ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write an extra justification string" );

					if( writeSpecificationJustifications( isOnlyWritingPrimarySpecification, isWritingPrimarySpecification, currentJustificationItem, startJustificationItem, selfGeneratedSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the current justification specification" );

					isWritingSeparator_ = true;
					isFirstTime = false;
					isWritingPrimarySpecification = false;
					currentJustificationItem.hasJustificationBeenWritten = true;

					previousPrimarySpecificationItem_ = primarySpecificationItem;
					previousSecondarySpecificationItem = secondarySpecificationItem;

					if( secondarySpecificationItem != null &&
					primarySpecificationItem != null &&
					primarySpecificationItem.isSelfGeneratedQuestion() &&

					( currentJustificationItem.isQuestionJustification() ||
					// Correct invalid assumption by opposite question
					currentJustificationItem.isSpecificationSubstitutionAssumption() ) &&

					( generalizationWordItem = primarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( generalizationWordItem.writeRelatedJustificationSpecifications( currentJustificationItem.justificationTypeNr(), secondarySpecificationItem.specificationCollectionNr() ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the related justification specifications" );

						previousPrimarySpecificationItem_ = null;
						}
					}
				else
					isStop = true;
				}
			}
		while( !isStop &&
		( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationType( boolean isFirstJustificationType, JustificationItem writeJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean hasNonExclusiveCollection = false;
		boolean isExclusiveSpecification;
		boolean isFirstTime = true;
		boolean isNegative;
		boolean isNegativeAssumptionOrConclusion;
		boolean isPossessive;
		boolean isSelfGenerated;
		boolean isWritingPrimarySpecificationOrExtraSeparator;
		short assumptionLevel;
		short specificationWordTypeNr;
		int generalizationContextNr;
		int relationContextNr;
		int specificationCollectionNr;
		JustificationItem currentJustificationItem;
		SpecificationItem currentPrimarySpecificationItem;
		SpecificationItem currentSecondarySpecificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem firstSecondarySpecificationItem;
		WordItem generalizationWordItem;

		previousPrimarySpecificationItem_ = null;

		if( writeJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given write justification is undefined" );

		if( selfGeneratedSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given self-generated specification item is undefined" );

		if( ( generalizationWordItem = selfGeneratedSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );

		if( isFirstJustificationType &&
		InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, GlobalVariables.writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification sentence" );

		if( isWritingSeparator_ &&
		InputOutput.writeInterfaceText( false, !isFirstJustificationType, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isFirstJustificationType ? Constants.INTERFACE_JUSTIFICATION_BECAUSE : Constants.INTERFACE_JUSTIFICATION_AND ) ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification start string" );

		if( writeJustificationSpecifications( true, false, writeJustificationItem, selfGeneratedSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification specifications" );

		isNegativeAssumptionOrConclusion = writeJustificationItem.isNegativeAssumptionOrConclusion();
		specificationCollectionNr = selfGeneratedSpecificationItem.specificationCollectionNr();

		if( ( firstSecondarySpecificationItem = writeJustificationItem.secondarySpecificationItem() ) != null &&
		isNegativeAssumptionOrConclusion &&
		firstSecondarySpecificationItem.hasNonExclusiveSpecificationCollection() )
			hasNonExclusiveCollection = true;

		if( ( !isNegativeAssumptionOrConclusion ||
		hasNonExclusiveCollection ||

		( !writeJustificationItem.hasAnotherPrimarySpecification() &&
		writeJustificationItem.isPrimarySpecificationWordSpanishAmbiguous() ) ) &&

		( currentSpecificationItem = generalizationWordItem.firstSpecificationItem( false, selfGeneratedSpecificationItem.isAssignment(), selfGeneratedSpecificationItem.isInactiveAssignment(), selfGeneratedSpecificationItem.isArchivedAssignment(), selfGeneratedSpecificationItem.questionParameter() ) ) != null )
			{
			isExclusiveSpecification = selfGeneratedSpecificationItem.isExclusiveSpecification();
			isNegative = selfGeneratedSpecificationItem.isNegative();
			isPossessive = selfGeneratedSpecificationItem.isPossessive();
			isSelfGenerated = selfGeneratedSpecificationItem.isSelfGenerated();
			assumptionLevel = selfGeneratedSpecificationItem.assumptionLevel();
			specificationWordTypeNr = selfGeneratedSpecificationItem.specificationWordTypeNr();
			generalizationContextNr = selfGeneratedSpecificationItem.generalizationContextNr();
			relationContextNr = selfGeneratedSpecificationItem.relationContextNr();

			do	{
				// Self-generated
				currentJustificationItem = currentSpecificationItem.firstJustificationItem();

				if( currentJustificationItem != null &&
				!currentJustificationItem.hasJustificationBeenWritten &&
				currentSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr ) != null )
					{
					currentPrimarySpecificationItem = currentJustificationItem.primarySpecificationItem();
					currentSecondarySpecificationItem = currentJustificationItem.secondarySpecificationItem();

					isWritingPrimarySpecificationOrExtraSeparator = ( currentPrimarySpecificationItem == null ||
																	// Suggestive assumption corrected by opposite question
																	currentPrimarySpecificationItem.isUserQuestion() ||

																	( currentSecondarySpecificationItem != null &&

																	( currentSecondarySpecificationItem.isGeneralizationProperNoun() ||

																	( !currentPrimarySpecificationItem.hasRelationContext() &&
																	currentPrimarySpecificationItem.isGeneralizationProperNoun() &&
																	!selfGeneratedSpecificationItem.isQuestion() ) ||

																	( isFirstTime &&
																	firstSecondarySpecificationItem != null &&
																	firstSecondarySpecificationItem.specificationCollectionNr() != currentSecondarySpecificationItem.specificationCollectionNr() ) ) ) );

					if( writeJustificationSpecifications( isWritingPrimarySpecificationOrExtraSeparator, isWritingPrimarySpecificationOrExtraSeparator, currentJustificationItem, null ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the current justification specifications" );

					isFirstTime = false;
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte writeSelfGeneratedSpecification( boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions, WordItem writeWordItem )
		{
		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated specifications" );

		// Active assignments
		if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated current-tense assignments" );

		// Inactive assignments
		if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated past-tense assignments" );

		// Archived assignments
		if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated archived assignments" );

		return Constants.RESULT_OK;
		}

	private byte writeSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, WordItem writeWordItem )
		{
		boolean hasAnyChangeBeenMadeByThisSentence;
		boolean hasHeaderBeenWritten = false;
		boolean isCurrentSpecificationWordSpanishAmbiguous;
		boolean isForcingResponseNotBeingFirstSpecification;
		boolean isHiddenSpanishSpecification;
		boolean isSelfGenerated;
		SpecificationItem currentSpecificationItem;
		WordItem singleRelationWordItem;
		StringBuffer writtenSentenceStringBuffer;

		if( writeWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = writeWordItem.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != null )
			{
			hasAnyChangeBeenMadeByThisSentence = adminItem_.hasAnyChangeBeenMadeByThisSentence();

			do	{
				isHiddenSpanishSpecification = false;

				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem.isSpecificationWordSpanishAmbiguous();
				isSelfGenerated = currentSpecificationItem.isSelfGenerated();

				if( isCurrentSpecificationWordSpanishAmbiguous &&
				currentSpecificationItem.hasRelationContext() &&

				( ( !isWritingJustification &&

				( ( currentSpecificationItem.hasNonCompoundSpecificationCollection() &&
				currentSpecificationItem.isOlderItem() &&
				currentSpecificationItem.isSelfGeneratedAssumption() &&
				adminItem_.hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore() &&

				( currentSpecificationItem.isPossessiveReversibleConclusion() ||

				( currentSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) != null &&
				currentSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ) == null ) ) ) ||

				( currentSpecificationItem.hasCompoundSpecificationCollection() &&
				currentSpecificationItem.isPossessiveReversibleConclusion() &&
				currentSpecificationItem.wasHiddenSpanishSpecification() ) ) ) ||

				( currentSpecificationItem.hasNonCompoundSpecificationCollection() &&
				!currentSpecificationItem.isConcludedAssumption() &&
				currentSpecificationItem.isPossessiveReversibleConclusion() &&
				( singleRelationWordItem = currentSpecificationItem.singleRelationWordItem() ) != null &&
				singleRelationWordItem.isMale() ) ) )
					isHiddenSpanishSpecification = true;

				// Typically for Spanish
				if( !isHiddenSpanishSpecification &&

				// Conclusions
				( ( isWritingSelfGeneratedConclusions &&
				currentSpecificationItem.isSelfGeneratedConclusion() &&

				// Skip recently concluded assumptions with no new information
				( !currentSpecificationItem.isConcludedAssumption() ||
				!currentSpecificationItem.isOlderItem() ||
				!currentSpecificationItem.hasNewInformation() ) ) ||

				// Assumptions
				( isWritingSelfGeneratedAssumptions &&
				currentSpecificationItem.isSelfGeneratedAssumption() ) ||

				// Questions
				( isWritingSelfGeneratedQuestions &&
				currentSpecificationItem.isSelfGeneratedQuestion() ) ||

				( ( isWritingUserSpecifications &&
				currentSpecificationItem.isUserSpecification() &&
				!currentSpecificationItem.isCorrectedAssumption() ) ||

				( isWritingUserQuestions &&
				currentSpecificationItem.isUserQuestion() ) ) ) &&

				( !isCurrentSpecificationWordSpanishAmbiguous ||
				// Typically for Spanish
				!currentSpecificationItem.isHiddenSpanishSpecification() ) &&

				// Filter on current or updated sentences
				( !isWritingCurrentSentenceOnly ||
				// If nothing has changed, it will result in the notification: "I know".
				!hasAnyChangeBeenMadeByThisSentence ||
				currentSpecificationItem.hasNewInformation() ||

				// Typically for Spanish
				( isCurrentSpecificationWordSpanishAmbiguous &&
				currentSpecificationItem.wasHiddenSpanishSpecification() ) ) )
					{
					isForcingResponseNotBeingFirstSpecification = ( isAssignment &&
																	isSelfGenerated &&
																	currentSpecificationItem.isExclusiveSpecification() );

					if( writeWordItem.writeSelectedSpecification( false, false, false, isForcingResponseNotBeingFirstSpecification, false, isWritingCurrentSentenceOnly, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write a selected specification" );

					if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
					writtenSentenceStringBuffer.length() > 0 )
						{
						if( isWritingSelfGeneratedConclusions )
							{
							if( isFirstSelfGeneratedConclusion_ )
								{
								hasHeaderBeenWritten = true;
								isFirstSelfGeneratedConclusion_ = false;
								}
							}
						else
							{
							if( isWritingSelfGeneratedAssumptions )
								{
								if( isFirstSelfGeneratedAssumption_ )
									{
									hasHeaderBeenWritten = true;
									isFirstSelfGeneratedAssumption_ = false;
									}
								}
							else
								{
								if( isWritingSelfGeneratedQuestions )
									{
									if( isFirstSelfGeneratedQuestion_ )
										{
										hasHeaderBeenWritten = true;
										isFirstSelfGeneratedQuestion_ = false;
										}
									}
								else
									{
									if( isWritingUserSpecifications )
										{
										if( isFirstUserSpecifications_ )
											{
											hasHeaderBeenWritten = true;
											isFirstUserSpecifications_ = false;
											}
										}
									else
										{
										if( isWritingUserQuestions &&
										isFirstUserQuestion_ )
											{
											hasHeaderBeenWritten = true;
											isFirstUserQuestion_ = false;
											}
										}
									}
								}
							}

						if( hasHeaderBeenWritten )
							{
							if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isWritingSelfGeneratedConclusions ? Constants.INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS : Constants.INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isSelfGenerated ? Constants.INTERFACE_LISTING_MY_INFORMATION : Constants.INTERFACE_LISTING_YOUR_INFORMATION ) : Constants.INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write my conclusions header" );

							hasHeaderBeenWritten = false;
							}

						if( isWritingJustification )
							{
							if( isSelfGenerated &&
							writeJustificationSpecification( currentSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write a justification line" );
							}
						else
							{
							if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write a sentence" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte writeSpecificationJustifications( boolean isOnlyWritingPrimarySpecification, boolean isWritingPrimarySpecification, JustificationItem writeJustificationItem, JustificationItem startJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean isPossessiveSelfGeneratedSpecificationItem;
		boolean isWritingCurrentSpecificationWordOnly;
		JustificationItem currentJustificationItem = startJustificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem;
		SpecificationItem currentPrimarySpecificationItem;
		SpecificationItem primarySpecificationItem = null;
		SpecificationItem secondarySpecificationItem;
		WordItem feminineOrMasculineProperNounEndingWordItem;
		WordItem generalizationWordItem;
		WordItem primaryGeneralizationWordItem = null;
		WordItem lastWrittenFeminineOrMasculineProperNounEndingWordItem = null;

		if( writeJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given write justification item is undefined" );

		if( isWritingPrimarySpecification )
			{
			if( ( primarySpecificationItem = writeJustificationItem.primarySpecificationItem() ) != null )
				{
				if( ( primaryGeneralizationWordItem = primarySpecificationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given primary specification item is undefined" );

				if( primarySpecificationItem.isHiddenSpanishSpecification() )
					{
					if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe primary specification item is a hidden specification:\n\tSpecificationItem: " + primarySpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + writeJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
						return adminItem_.startError( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( primaryGeneralizationWordItem.writeJustificationSpecification( ( !primarySpecificationItem.isExclusiveSpecification() && !primarySpecificationItem.isPossessive() ), primarySpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the definition justification specification" );
					}
				}

			if( !isOnlyWritingPrimarySpecification &&
			( anotherPrimarySpecificationItem = writeJustificationItem.anotherPrimarySpecificationItem() ) != null )
				{
				if( ( generalizationWordItem = anotherPrimarySpecificationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given another primary specification item is undefined" );

				if( anotherPrimarySpecificationItem.isHiddenSpanishSpecification() )
					{
					if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe another primary specification item is a hidden specification:\n\tSpecificationItem: " + anotherPrimarySpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + writeJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
						return adminItem_.startError( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( generalizationWordItem.writeJustificationSpecification( false, anotherPrimarySpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the another definition justification specification" );
					}
				}
			}

		if( !isOnlyWritingPrimarySpecification )
			{
			if( ( secondarySpecificationItem = writeJustificationItem.secondarySpecificationItem() ) != null )
				{
				if( ( generalizationWordItem = secondarySpecificationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given secondary specification item is undefined" );

				if( secondarySpecificationItem.isHiddenSpanishSpecification() )
					{
					if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe secondary specification item is a hidden specification:\n\tSpecificationItem: " + secondarySpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + writeJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
						return adminItem_.startError( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					isWritingCurrentSpecificationWordOnly = ( secondarySpecificationItem.isPartOf() ||

															( !secondarySpecificationItem.isExclusiveSpecification() &&
															secondarySpecificationItem.isGeneralizationProperNoun() &&
															!secondarySpecificationItem.isPossessive() ) );

					if( generalizationWordItem.writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, secondarySpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the secondary justification specification" );
					}
				}

			if( ( anotherSecondarySpecificationItem = writeJustificationItem.anotherSecondarySpecificationItem() ) != null )
				{
				if( ( generalizationWordItem = anotherSecondarySpecificationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given another secondary specification item is undefined" );

				if( anotherSecondarySpecificationItem.isHiddenSpanishSpecification() )
					{
					if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe another secondary specification item is a hidden specification:\n\tSpecificationItem: " + anotherSecondarySpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + writeJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
						return adminItem_.startError( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( !writeJustificationItem.isPossessiveReversibleAssumption() ||
					// Typically for Spanish
					writeJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() == null )
						{
						if( generalizationWordItem.writeJustificationSpecification( false, anotherSecondarySpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the another secondary justification specification" );
						}
					}
				}

			// Now display list of feminine/masculine assumptions
			if( currentJustificationItem != null &&
			selfGeneratedSpecificationItem != null &&
			writeJustificationItem.hasFeminineOrMasculineProperNounEnding() )
				{
				if( ( isWritingPrimarySpecification &&
				!writeJustificationItem.isPossessiveReversibleAssumption() ) ||

				// Typically for Spanish
				writeJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() == null )
					{
					isPossessiveSelfGeneratedSpecificationItem = selfGeneratedSpecificationItem.isPossessive();

					do	{
						if( currentJustificationItem.hasFeminineOrMasculineProperNounEnding() )
							{
							currentPrimarySpecificationItem = currentJustificationItem.primarySpecificationItem();
							feminineOrMasculineProperNounEndingWordItem = ( isPossessiveSelfGeneratedSpecificationItem ? ( selfGeneratedSpecificationItem.hasOnlyOneRelationWord() ? selfGeneratedSpecificationItem.singleRelationWordItem() : currentPrimarySpecificationItem.generalizationWordItem() ) : currentJustificationItem.generalizationWordItem() );

							if( feminineOrMasculineProperNounEndingWordItem == null )
								return adminItem_.startError( 1, moduleNameString_, "I couldn't find the feminine or masculine proper noun ending word" );

							if( !feminineOrMasculineProperNounEndingWordItem.hasFeminineOrMasculineProperNounEnding() )
								return adminItem_.startError( 1, moduleNameString_, "Word \"" + feminineOrMasculineProperNounEndingWordItem.anyWordTypeString() + "\" has no feminine or masculine proper noun ending" );

							if( feminineOrMasculineProperNounEndingWordItem != lastWrittenFeminineOrMasculineProperNounEndingWordItem )
								{
								if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED, Constants.INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NOUN_ENDING, feminineOrMasculineProperNounEndingWordItem.anyWordTypeString(), ( feminineOrMasculineProperNounEndingWordItem.hasFeminineProperNounEnding() ? Constants.INTERFACE_JUSTIFICATION_FEMININE_PROPER_NOUN_ENDING : Constants.INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NOUN_ENDING ) ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification sentence start string" );

								lastWrittenFeminineOrMasculineProperNounEndingWordItem = feminineOrMasculineProperNounEndingWordItem;
								}
							}
						}
					while( ( currentJustificationItem = currentJustificationItem.attachedJustificationItem() ) != null );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte writeUserQuestions( boolean isWritingCurrentSentenceOnly, WordItem writeWordItem )
		{
		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user questions without assignments" );

		// Active assignments
		if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user questions with current-tense assignments" );

		// Inactive assignments
		if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user questions with past-tense assignments" );

		// Archived assignments
		if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user questions with archived assignments" );

		return Constants.RESULT_OK;
		}

	private byte writeUserSpecifications( boolean isWritingCurrentSentenceOnly, WordItem writeWordItem )
		{
		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user-entered specifications" );

		// Active assignments
		if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user-entered current-tense assignments" );

		// Inactive assignments
		if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user-entered past-tense assignments" );

		// Archived assignments
		if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user-entered archived assignments" );

		return Constants.RESULT_OK;
		}

	private byte writeSelectedSpecificationInfo( WordItem writeWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( ( currentGeneralizationItem = writeWordItem.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			// Do for all generalization specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I found an undefined generalization word" );

				// Respond with active specifications
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( false, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active specifications" );

				// Respond with active assignments
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active assignments" );

				// Respond with inactive assignments
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, true, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write inactive assignments" );

				// Respond with archived assignments
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write archived assignments" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte writeSelectedRelationInfo( WordItem writeWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( ( currentGeneralizationItem = writeWordItem.firstRelationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I found an undefined generalization word" );

				// Respond with active related specifications
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active related specifications" );

				// Respond with active related specification questions
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( false, false, false, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active related specification questions" );

				// Respond with active related assignments
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active related assignments" );

				// Respond with active related assignment questions
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, false, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active related assignment assignments" );

				// Respond with inactive related assignments
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, true, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active related assignments" );

				// Respond with inactive related assignment questions
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, true, false, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write inactive related assignment questions" );

				// Respond with archive related assignments
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, true, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write archive related assignment" );

				// Respond with archive related assignment questions
				if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, true, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write archive related assignment questions" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextRelationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminWrite( AdminItem adminItem )
		{
		// Private constructed variables

		hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;
		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;
		isWritingSeparator_ = false;

		previousPrimarySpecificationItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminWriteVariables()
		{
		hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;
		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;
		}

	protected byte answerQuestions()
		{
		WordItem currentTouchedWordItem;

		if( GlobalVariables.isQuestionAlreadyAnswered &&
		InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'question is already answered' interface notification" );

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				if( currentTouchedWordItem.findAnswerToNewUserQuestion() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find an answer to new user questions of word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

		if( !GlobalVariables.hasFoundAnswerToQuestion &&
		!GlobalVariables.isQuestionAlreadyAnswered &&
		InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I don't know the answer to this question' interface notification" );

		return Constants.RESULT_OK;
		}

	protected byte checkIntegrityOfStoredUserSentence( String readUserSentenceString )
		{
		boolean haveAllWordsPassed = true;
		ReadItem currentReadItem;
		StringBuffer writtenSentenceStringBuffer;

		if( ( currentReadItem = adminItem_.firstActiveReadItem() ) != null )
			{
			do	{
				if( !currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence )
					haveAllWordsPassed = false;
				}
			while( haveAllWordsPassed &&
			( currentReadItem = currentReadItem.nextReadItem() ) != null );
			}

		if( !haveAllWordsPassed )
			{
			if( readUserSentenceString != null &&

			( adminItem_.isCurrentlyTesting() ||
			adminItem_.isSystemStartingUp() ) )
				{
				if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, Constants.EMPTY_STRING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START, readUserSentenceString, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS : Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE ) ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, Constants.EMPTY_STRING, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE_DUE_TO_WORDS : Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE ) ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}

			if( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ )
				{
				if( displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to display the words that didn't pass the integrity check" );

				if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
				writtenSentenceStringBuffer.length() > 0 &&
				InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, writtenSentenceStringBuffer.toString(), Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}

			if( adminItem_.isSystemStartingUp() )
				return adminItem_.startError( 1, moduleNameString_, "An integrity error occurred during startup" );
			}

		return Constants.RESULT_OK;
		}

	protected byte prepareIntegrityCheckOfStoredUserSentence( boolean isChineseCurrentLanguage, SpecificationItem userSpecificationItem, String readUserSentenceString )
		{
		boolean isEqualReadAndWriteUserString;
		short lastFoundWordOrderNr = Constants.NO_ORDER_NR;
		int readUserSentenceStringLength = ( readUserSentenceString == null ? 0 : readUserSentenceString.length() );
		int readWordTypeStringLength;
		int wordPosition = 0;
		int writtenUserSentenceStringBufferLength;
		String readWordTypeString;
		StringBuffer writtenUserSentenceStringBuffer;
		ReadItem currentReadItem;
		ReadItem startNewSpecificationReadItem = null;
		WordItem generalizationWordItem;
		ReadWordResultType readWordResult = new ReadWordResultType();

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( ( currentReadItem = adminItem_.firstActiveReadItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any read words" );

		if( ( generalizationWordItem = userSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( generalizationWordItem.writeSelectedSpecification( true, false, userSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the user specification of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		if( ( writtenUserSentenceStringBuffer = GlobalVariables.writtenUserSentenceStringBuffer ) != null &&
		( writtenUserSentenceStringBufferLength = writtenUserSentenceStringBuffer.length() ) > 0 )
			{
			isEqualReadAndWriteUserString = ( readUserSentenceStringLength > 0 &&
											readUserSentenceString != null &&

											( readUserSentenceString.charAt( 0 ) == writtenUserSentenceStringBuffer.charAt( 0 ) ||
											Character.toLowerCase( readUserSentenceString.charAt( 0 ) ) == writtenUserSentenceStringBuffer.charAt( 0 ) ) &&

											( readUserSentenceStringLength == 1 ||
											// The read user string and written user string are equal
											readUserSentenceString.substring( 1 ).compareTo( writtenUserSentenceStringBuffer.toString().substring( 1 ) ) == 0 ) );

			do	{
				do	{
					if( isEqualReadAndWriteUserString )
						currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;
					else
						{
						if( ( readWordTypeString = currentReadItem.readWordTypeString() ) == null )
							{
							// Skip text string (or hidden word)
							currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;
							// Avoid looping
							readWordResult.offset = ( writtenUserSentenceStringBufferLength - wordPosition );
							}
						else
							{
							readWordTypeStringLength = readWordTypeString.length();

							if( ( readWordResult = adminItem_.readWordFromString( false, false, false, false, readWordTypeStringLength, writtenUserSentenceStringBuffer.substring( wordPosition ) ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to read a word from the written string" );

							if( !currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence )
								{
								if( readWordTypeStringLength == readWordResult.wordLength &&
								writtenUserSentenceStringBuffer.substring( wordPosition ).startsWith( readWordTypeString ) )
									{
									hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = true;
									currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;

									if( lastFoundWordOrderNr == Constants.NO_ORDER_NR ||
									lastFoundWordOrderNr + 1 == currentReadItem.wordOrderNr() )
										{
										wordPosition += readWordResult.offset;
										lastFoundWordOrderNr = currentReadItem.wordOrderNr();
										startNewSpecificationReadItem = currentReadItem.nextReadItem();
										}
									}
								else
									{
									if( currentReadItem.isSkippingIntegrityCheckWords() ||

									// Typically for Chinese
									( isChineseCurrentLanguage &&
									currentReadItem.isSkippingChineseIntegrityCheckWords() ) )
										// Skip until implemented
										currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;

									if( startNewSpecificationReadItem == null )
										startNewSpecificationReadItem = currentReadItem;
									}
								}
							}
						}
					}
				while( ( currentReadItem = currentReadItem.nextReadItem() ) != null );

				wordPosition += readWordResult.offset;
				currentReadItem = startNewSpecificationReadItem;
				}
			while( currentReadItem != null &&
			wordPosition < writtenUserSentenceStringBufferLength );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions )
		{
		WordItem currentTouchedWordItem;

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				if( writeInfoAboutWord( true, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, false, false, currentTouchedWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated info of word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions, WordItem writeWordItem )
		{
		// Self-generated conclusions
		if( isWritingSelfGeneratedConclusions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, true, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated conclusions" );

		// Self-generated assumptions
		if( isWritingSelfGeneratedAssumptions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, true, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated assumptions" );

		// Self-generated questions
		if( isWritingSelfGeneratedQuestions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, false, true, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated questions" );

		return Constants.RESULT_OK;
		}

	protected byte writeInfoAboutWord( boolean isWritingCurrentSentenceOnly, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, boolean isWritingSpecificationInfo, boolean isWritingRelatedInfo, WordItem writeWordItem )
		{
		if( isWritingUserSpecifications &&
		writeUserSpecifications( isWritingCurrentSentenceOnly, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user specifications" );

		if( isWritingUserQuestions &&
		writeUserQuestions( isWritingCurrentSentenceOnly, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user questions" );

		if( isWritingSelfGeneratedConclusions ||
		isWritingSelfGeneratedAssumptions ||
		isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated info" );
			}

		if( isWritingSpecificationInfo &&
		!writeWordItem.isNounWordSpanishAmbiguous() &&
		writeSelectedSpecificationInfo( writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write selected specification info" );

		if( isWritingRelatedInfo &&
		writeSelectedRelationInfo( writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write selected related info" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lord, for he is good!
 *	His faithful love endures forever." (Psalm 107:1)
 *************************************************************************/
