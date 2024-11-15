﻿/*	Class:			AdminWrite
 *	Supports class:	AdminItem
 *	Purpose:		Writing selected specifications as sentences
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

package org.mafait.thinknowlogy;

class AdminWrite
	{
	// Private constructed variables

	private boolean hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;
	private boolean isFirstSelfGeneratedAssumption_ = false;
	private boolean isFirstSelfGeneratedConclusion_ = false;
	private boolean isFirstSelfGeneratedQuestion_ = false;
	private boolean isFirstUserSpecifications_ = false;

	private JustificationItem lastWrittenJustificationItem_ = null;

	private final String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private byte displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence()
		{
		ReadItem currentReadItem = adminItem_.firstActiveReadItem();
		String readWordTypeString;

		do	{
			if( !currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence &&
			( readWordTypeString = currentReadItem.readWordTypeString() ) != null &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INDENTED, ( readWordTypeString + " (position: " + currentReadItem.wordOrderNr() + ", word type: " + WordItem.wordTypeNameString( currentReadItem.wordTypeNr() ) + ", word parameter: " + currentReadItem.wordParameter() + ", grammar parameter: " + currentReadItem.grammarParameter + ")" ) ) != Constants.RESULT_OK )
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

		generalizationWordItem.clearJustificationHasBeenWritten();

		lastWrittenJustificationItem_ = null;
		currentJustificationItem = firstJustificationItem;

		do	{
			if( writeJustificationType( isFirstJustificationType, currentJustificationItem, selfGeneratedSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write a justification type of a specification" );

			isFirstJustificationType = false;
			}
		while( ( currentJustificationItem = currentJustificationItem.nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != null );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationSpecifications( boolean isWritingPrimarySpecification, boolean isWritingSeparator, JustificationItem startJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean hasPrimarySpecificationCompoundCollection;
		boolean hasPrimarySpecificationRelationWord;
		boolean isExclusiveSecondarySpecification;
		boolean isFirstTime = true;
		boolean isForcingSeparator = false;
		boolean isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification;
		boolean isOnlyWritingPrimarySpecification;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		boolean isPossessivePrimarySpecification;
		boolean isQuestionJustification;
		boolean isStop = false;
		JustificationItem currentJustificationItem;
		SpecificationItem previousPrimarySpecificationItem = ( lastWrittenJustificationItem_ == null ? null : lastWrittenJustificationItem_.primarySpecificationItem() );
		SpecificationItem previousSecondarySpecificationItem = null;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;

		if( startJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given start justification item is undefined" );

		currentJustificationItem = startJustificationItem;

		do	{
			isExclusiveSecondarySpecification = false;
			isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification = false;
			isOnlyWritingPrimarySpecification = false;
			isQuestionJustification = currentJustificationItem.isQuestionJustification();

			if( ( primarySpecificationItem = currentJustificationItem.primarySpecificationItem() ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The primary specification of the current justification is undefined" );

			hasPrimarySpecificationCompoundCollection = primarySpecificationItem.hasCompoundSpecificationCollection();
			hasPrimarySpecificationRelationWord = primarySpecificationItem.hasRelationWord();
			isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();
			isPossessivePrimarySpecification = primarySpecificationItem.isPossessive();

			// No secondary specification
			if( ( secondarySpecificationItem = currentJustificationItem.secondarySpecificationItem() ) == null )
				{
				if( !isWritingPrimarySpecification )
					// Test files: "reasoning\Collect afterwards - Past tense",
					//				"reasoning\Collect afterwards - Uncertainty"
					{
					isForcingSeparator = true;
					isWritingPrimarySpecification = true;
					}
				}
			else	// Secondary specification
				{
				if( secondarySpecificationItem.isExclusiveSpecification() &&

					// Question
					( ( isQuestionJustification &&
					!isWritingPrimarySpecification ) ||

					// Test file: "reasoning\family\Justification report"
					( hasPrimarySpecificationCompoundCollection &&
					currentJustificationItem.isSpecificationSubstitutionAssumptionOrConclusion() ) ) )
					isExclusiveSecondarySpecification = true;
				}

			// No question
			if( ( !isQuestionJustification &&

				( hasPrimarySpecificationRelationWord ||
				isForcingSeparator ) ) ||

			// Question
			( isQuestionJustification &&
			isPossessivePrimarySpecification ) )
				{
				if( currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() != null )
					isOnlyWritingPrimarySpecification = true;

				// Typical for Spanish
				if( isPrimarySpecificationWordSpanishAmbiguous &&
				!hasPrimarySpecificationCompoundCollection &&
				isPossessivePrimarySpecification &&
				primarySpecificationItem.hasOnlyOneRelationWord() &&
				// Spanish test file: "Reto científico"
				!primarySpecificationItem.isArchivedAssignment() &&
				primarySpecificationItem.isSelfGeneratedConclusion() &&
				!currentJustificationItem.isSpecificationSubstitutionPartOfAssumptionOrConclusion() )
					isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification = true;
				}

			if( isWritingPrimarySpecification ||

			( !isExclusiveSecondarySpecification &&
			previousPrimarySpecificationItem != null &&
			secondarySpecificationItem != previousSecondarySpecificationItem ) ||

			// Test file: "reasoning\family\Justification report"
			( isFirstTime &&
			!isQuestionJustification &&
			primarySpecificationItem != previousPrimarySpecificationItem ) )
				{
				if( isFirstTime &&

					( isForcingSeparator ||
					isWritingSeparator ) &&

				// Write word to screen / output file
				InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_JUSTIFICATION_AND ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write a word to the screen / output file" );

				// Spanish test file: "razonamiento\familia\Que haya confirmado al menos una palabra de relación"
				if( isQuestionJustification &&
				hasPrimarySpecificationRelationWord &&
				!isOnlyWritingPrimarySpecification &&
				!isPrimarySpecificationWordSpanishAmbiguous &&
				// Next justification with same type and order number exists
				currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() != null )
					isOnlyWritingPrimarySpecification = true;

				if( writeSpecificationJustifications( isOnlyWritingPrimarySpecification, ( isWritingPrimarySpecification && !isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification ), isWritingPrimarySpecification, currentJustificationItem, startJustificationItem, selfGeneratedSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the current justification specification" );

				isFirstTime = false;

				if( !isOnlyWritingPrimarySpecification )
					isWritingPrimarySpecification = false;

				currentJustificationItem.hasJustificationBeenWritten = true;
				lastWrittenJustificationItem_ = currentJustificationItem;

				previousPrimarySpecificationItem = primarySpecificationItem;
				previousSecondarySpecificationItem = secondarySpecificationItem;
				}
			else
				isStop = true;
			}
		while( !isStop &&
		( currentJustificationItem = currentJustificationItem.nextJustificationItemWithSameTypeAndOrderNr() ) != null );

		return Constants.RESULT_OK;
		}

	private byte writeJustificationType( boolean isFirstJustificationType, JustificationItem writeJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isSelfGeneratedQuestion;
		boolean isSpecificationGeneralization;
		boolean isWritingPrimarySpecificationOrSeparator;
		short assumptionLevel;
		short selectedJustificationTypeNr;
		short specificationWordTypeNr;
		int generalizationContextNr;
		int relationCollectionNr;
		int specificationCollectionNr;
		JustificationItem currentJustificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem currentPrimarySpecificationItem;
		SpecificationItem currentSecondarySpecificationItem;
		WordItem generalizationWordItem;
		WordItem relationWordItem;

		if( writeJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given write justification is undefined" );

		if( selfGeneratedSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given self-generated specification item is undefined" );

		if( ( generalizationWordItem = selfGeneratedSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );

		if( isFirstJustificationType )
			{
			if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, GlobalVariables.writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification sentence" );

			// Write text to screen / output file
			if( InputOutput.writeInterfaceText( false, false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_JUSTIFICATION_BECAUSE ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write a text to the screen / output file" );
			}

		// Write justifications of this specification
		if( writeJustificationSpecifications( true, !isFirstJustificationType, writeJustificationItem, selfGeneratedSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification specifications" );

		if( !writeJustificationItem.isNegativeAssumptionOrConclusion() &&
		// Find a related specification
		( currentSpecificationItem = generalizationWordItem.firstSpecificationItem( false, selfGeneratedSpecificationItem.isAssignment(), selfGeneratedSpecificationItem.isInactiveAssignment(), selfGeneratedSpecificationItem.isArchivedAssignment(), selfGeneratedSpecificationItem.questionParameter() ) ) != null &&
		// Found a related specification
		currentSpecificationItem != selfGeneratedSpecificationItem )
			{
			isExclusiveSpecification = selfGeneratedSpecificationItem.isExclusiveSpecification();
			isNegative = selfGeneratedSpecificationItem.isNegative();
			isPossessive = selfGeneratedSpecificationItem.isPossessive();
			isSpecificationGeneralization = selfGeneratedSpecificationItem.isSpecificationGeneralization();
			assumptionLevel = selfGeneratedSpecificationItem.assumptionLevel();
			relationCollectionNr = selfGeneratedSpecificationItem.relationCollectionNr();
			specificationCollectionNr = selfGeneratedSpecificationItem.specificationCollectionNr();
			specificationWordTypeNr = selfGeneratedSpecificationItem.specificationWordTypeNr();
			generalizationContextNr = selfGeneratedSpecificationItem.generalizationContextNr();
			relationWordItem = selfGeneratedSpecificationItem.relationWordItem();

			do	{
				if( currentSpecificationItem != selfGeneratedSpecificationItem &&
				currentSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, true, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, relationWordItem ) != null &&
				( currentPrimarySpecificationItem = writeJustificationItem.primarySpecificationItem() ) != null )
					{
					isSelfGeneratedQuestion = ( ( currentSecondarySpecificationItem = writeJustificationItem.secondarySpecificationItem() ) != null &&
												selfGeneratedSpecificationItem.isQuestion() );

													// Test file: "conflict\family\No conflict on multiple related specifications"
					selectedJustificationTypeNr = ( isPossessive ? Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION :

													// Test files: "reasoning\Collect afterwards - Past tense",
													//				"reasoning\Collect afterwards - Uncertainty"
													( isSpecificationGeneralization ? Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION :

													// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
													( currentSecondarySpecificationItem == null &&
													currentSpecificationItem.isCorrectedSpecification() ? Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION :

													( isSelfGeneratedQuestion &&
													!currentPrimarySpecificationItem.isPossessive() ? Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION :

													// Test files: "conflict\family\No conflict on multiple related specifications",
													//				"reasoning\Basic reasoning",
													//				"reasoning\Scientific challenge",
													//				"reasoning\family\Justification report",
													//				"reasoning\family\Simple family definition - Justification report",
													//				"reasoning\family\This information is more specific (non-exclusive)"
																										Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) ) ) );

					if( lastWrittenJustificationItem_ != null &&
					// Find related justification in current specification
					( currentJustificationItem = ( isExclusiveSpecification ? currentSpecificationItem.firstJustificationItem( selectedJustificationTypeNr, lastWrittenJustificationItem_.justificationOrderNr ) :
																				// Test file: "conflict\family\No conflict on multiple related specifications"
																				currentSpecificationItem.firstJustificationItem( selectedJustificationTypeNr ) ) ) != null &&

						( !currentJustificationItem.hasPrimarySpecificationRelationWord() ||
						// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
						currentSecondarySpecificationItem == null ) )
						{
						// Test file: "conflict\family\No conflict on multiple related specifications"
						isWritingPrimarySpecificationOrSeparator = ( !isExclusiveSpecification &&
																	currentSecondarySpecificationItem != null );

						// Write related justification specifications
						if( writeJustificationSpecifications( isWritingPrimarySpecificationOrSeparator, isWritingPrimarySpecificationOrSeparator, currentJustificationItem, null ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write related justification specifications" );
						}
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
		boolean hasCurrentSpecificationAssumptionLevel;
		boolean hasCurrentSpecificationRelationWord;
		boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore;
		boolean hasHeaderBeenWritten = false;
		boolean isCurrentSpecificationSelfGenerated;
		boolean isCurrentSpecificationWordSpanishAmbiguous;
		boolean isHiddenSpanishSpecification;
		boolean isOnlyOptionLeftAssignment;
		SpecificationItem currentSpecificationItem;
		WordItem singleRelationWordItem;
		StringBuffer writtenSentenceStringBuffer;

		if( writeWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given write word item is undefined" );

		hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = writeWordItem.hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore;

		if( ( currentSpecificationItem = writeWordItem.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != null )
			{
			do	{
				hasCurrentSpecificationAssumptionLevel = currentSpecificationItem.hasAssumptionLevel();
				hasCurrentSpecificationRelationWord = currentSpecificationItem.hasRelationWord();
				isCurrentSpecificationSelfGenerated = currentSpecificationItem.isSelfGeneratedSpecification();
				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem.isSpecificationWordSpanishAmbiguous();
				isHiddenSpanishSpecification = false;

				// Typical for Spanish
				if( isCurrentSpecificationWordSpanishAmbiguous &&
				hasCurrentSpecificationRelationWord &&
				isCurrentSpecificationSelfGenerated &&
				currentSpecificationItem.isPossessive() &&
				currentSpecificationItem.isFirstJustificationReversibleAssumptionOrConclusion() )
					{
					// Self-generated conclusion
					if( ( ( singleRelationWordItem = currentSpecificationItem.singleRelationWordItem() ) != null &&
					!hasCurrentSpecificationAssumptionLevel &&
					currentSpecificationItem.hasNonCompoundSpecificationCollection() &&
					singleRelationWordItem.isMasculineWord() ) ||

					// Spanish test file: "razonamiento\familia\Complejo (16)"
					( singleRelationWordItem == null &&
					hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore &&
					hasCurrentSpecificationAssumptionLevel ) )
						isHiddenSpanishSpecification = true;
					}

				if( !isHiddenSpanishSpecification &&

				// User specifications
				( ( isWritingUserSpecifications &&
				currentSpecificationItem.isUserSpecification() ) ||

				// User questions
				( isWritingUserQuestions &&
				currentSpecificationItem.isUserQuestion() ) ||

				// Self-generated conclusions
				( ( ( isWritingSelfGeneratedConclusions &&
				!hasCurrentSpecificationAssumptionLevel &&
				isCurrentSpecificationSelfGenerated ) ||

				// Self-generated assumptions
				( isWritingSelfGeneratedAssumptions &&
				hasCurrentSpecificationAssumptionLevel &&
				isCurrentSpecificationSelfGenerated ) ||

				// Self-generated questions
				( isWritingSelfGeneratedQuestions &&
				currentSpecificationItem.isSelfGeneratedQuestion() ) ) &&

						// Filter on current or updated sentences
						( ( ( !isWritingCurrentSentenceOnly ||
						currentSpecificationItem.hasUpdatedWriteInformation() ) &&

							( !isCurrentSpecificationWordSpanishAmbiguous ||
							// Typical for Spanish: Don't write hidden specifications
							!currentSpecificationItem.isHiddenSpanishSpecification() ) ) ||

						// Typical for Spanish
						// Spanish test file: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente - Juan es masculino"
						( isCurrentSpecificationWordSpanishAmbiguous &&
						currentSpecificationItem.wasHiddenSpanishSpecification() ) ) ) ) )
					{
					isOnlyOptionLeftAssignment = ( isAssignment &&
													isCurrentSpecificationSelfGenerated &&
													currentSpecificationItem.isExclusiveSpecification() );

					if( writeWordItem.writeSelectedSpecification( false, false, isOnlyOptionLeftAssignment, false, isWritingCurrentSentenceOnly, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) != Constants.RESULT_OK )
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
										hasHeaderBeenWritten = true;
									}
								}
							}

						if( hasHeaderBeenWritten )
							{
							// Write header to screen / output file
							if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isCurrentSpecificationSelfGenerated ? ( isWritingSelfGeneratedConclusions ? Constants.INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS : Constants.INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isCurrentSpecificationSelfGenerated ? Constants.INTERFACE_LISTING_MY_INFORMATION : Constants.INTERFACE_LISTING_YOUR_INFORMATION ) : Constants.INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write a header to the screen / output file" );

							hasHeaderBeenWritten = false;
							}

						if( isWritingJustification )
							{
							if( writeJustificationSpecification( currentSpecificationItem ) != Constants.RESULT_OK )
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

	private byte writeSpecificationJustifications( boolean isOnlyWritingPrimarySpecification, boolean isWritingPrimarySpecification, boolean isWritingAdditionalDefinitionSpecification, JustificationItem writeJustificationItem, JustificationItem startJustificationItem, SpecificationItem selfGeneratedSpecificationItem )
		{
		boolean isPossessiveSelfGeneratedSpecificationItem;
		boolean isWritingCurrentSpecificationWordOnly;
		JustificationItem currentJustificationItem = startJustificationItem;
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem additionalProperNounSpecificationItem;
		SpecificationItem primarySpecificationItem = null;
		SpecificationItem secondarySpecificationItem;
		WordItem feminineOrMasculineProperNounEndingWordItem;
		WordItem generalizationWordItem;
		WordItem primaryGeneralizationWordItem = null;
		WordItem lastWrittenFeminineOrMasculineProperNounEndingWordItem = null;

		if( writeJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given write justification item is undefined" );

		if( currentJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given start justification item is undefined" );

		if( isWritingPrimarySpecification &&
		( primarySpecificationItem = writeJustificationItem.primarySpecificationItem() ) != null )
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
				if( primaryGeneralizationWordItem.writeJustificationSpecificationInWord( ( !primarySpecificationItem.isExclusiveSpecification() && !primarySpecificationItem.isPossessive() ), primarySpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the definition justification specification" );
				}
			}

		if( !isOnlyWritingPrimarySpecification &&
		isWritingAdditionalDefinitionSpecification &&
		( additionalDefinitionSpecificationItem = writeJustificationItem.additionalDefinitionSpecificationItem() ) != null )
			{
			if( ( generalizationWordItem = additionalDefinitionSpecificationItem.generalizationWordItem() ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given additional definition specification item is undefined" );

			if( additionalDefinitionSpecificationItem.isHiddenSpanishSpecification() )
				{
				if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe additional definition specification item is a hidden specification:\n\tSpecificationItem: " + additionalDefinitionSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + writeJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
					return adminItem_.startError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( generalizationWordItem.writeJustificationSpecificationInWord( false, additionalDefinitionSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the another definition justification specification" );
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
					isWritingCurrentSpecificationWordOnly = ( writeJustificationItem.isSpecificationSubstitutionPartOfAssumptionOrConclusion() ||

															// Test file: "conflict\family\No conflict on multiple related specifications"
															( isWritingPrimarySpecification &&
															writeJustificationItem.isOppositePossessiveSpecificationAssumption() &&
															!secondarySpecificationItem.hasRelationWord() &&
															!secondarySpecificationItem.isExclusiveSpecification() &&
															!secondarySpecificationItem.isPossessive() ) );

					if( generalizationWordItem.writeJustificationSpecificationInWord( isWritingCurrentSpecificationWordOnly, secondarySpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the secondary justification specification" );
					}
				}

			if( ( additionalProperNounSpecificationItem = writeJustificationItem.additionalProperNounSpecificationItem() ) != null )
				{
				if( ( generalizationWordItem = additionalProperNounSpecificationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given additional proper noun specification item is undefined" );

				if( additionalProperNounSpecificationItem.isHiddenSpanishSpecification() )
					{
					if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nThe additional proper noun specification item is a hidden specification:\n\tSpecificationItem: " + additionalProperNounSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + writeJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
						return adminItem_.startError( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( generalizationWordItem.writeJustificationSpecificationInWord( false, additionalProperNounSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the additional proper noun justification specification" );
					}
				}

			// Now display list of feminine / masculine assumptions
			if( selfGeneratedSpecificationItem != null &&
			writeJustificationItem.hasFeminineOrMasculineProperNounEnding() )
				{
				isPossessiveSelfGeneratedSpecificationItem = selfGeneratedSpecificationItem.isPossessive();

				do	{
					if( currentJustificationItem.hasFeminineOrMasculineProperNounEnding() )
						{
						feminineOrMasculineProperNounEndingWordItem = ( isPossessiveSelfGeneratedSpecificationItem ? selfGeneratedSpecificationItem.singleRelationWordItem() :
																													currentJustificationItem.generalizationWordItem() );

						if( feminineOrMasculineProperNounEndingWordItem == null )
							return adminItem_.startError( 1, moduleNameString_, "I couldn't find the feminine or masculine proper noun ending word" );

						if( !feminineOrMasculineProperNounEndingWordItem.hasFeminineOrMasculineProperNounEnding() )
							return adminItem_.startError( 1, moduleNameString_, "Word \"" + feminineOrMasculineProperNounEndingWordItem.anyWordTypeString() + "\" has no feminine or masculine proper noun ending" );

						if( feminineOrMasculineProperNounEndingWordItem != lastWrittenFeminineOrMasculineProperNounEndingWordItem )
							{
							// Write justification sentence start string to screen / output file
							if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WRITE_INDENTED, Constants.INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NOUN_ENDING, feminineOrMasculineProperNounEndingWordItem.anyWordTypeString(), ( feminineOrMasculineProperNounEndingWordItem.hasFeminineProperNounEnding() ? Constants.INTERFACE_JUSTIFICATION_FEMININE_PROPER_NOUN_ENDING : Constants.INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NOUN_ENDING ) ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write the justification sentence start string to the screen / output file" );

							lastWrittenFeminineOrMasculineProperNounEndingWordItem = feminineOrMasculineProperNounEndingWordItem;
							}
						}
					}
				while( ( currentJustificationItem = currentJustificationItem.attachedJustificationItem() ) != null );
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

		// Archived assignments
		if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user-entered archived assignments" );

		// Inactive assignments
		if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user-entered past-tense assignments" );

		// Active assignments
		if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user-entered current-tense assignments" );

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
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

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
			// Do for all generalization relation words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

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
		isFirstUserSpecifications_ = true;
		}

	protected byte answerQuestions()
		{
		WordItem currentTouchedWordItem;

		// Write 'question is already answered' interface notification to screen / output file
		if( GlobalVariables.isQuestionAlreadyAnswered &&
		InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'question is already answered' interface notification to the screen / output file" );

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			// Do for all words touched during current sentence
			do	{
				// Find answer to new user questions
				if( currentTouchedWordItem.findAnswerToNewUserQuestion() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find an answer to new user questions of word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

		// Didn't find any answer
		if( !GlobalVariables.hasFoundAnswerToQuestion &&
		!GlobalVariables.isConflictingQuestion &&
			// Write text to screen / output file
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
			if( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ &&
			readUserSentenceString != null )
				{
				// Write interface warning to screen / output file
				if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, Constants.EMPTY_STRING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START, readUserSentenceString, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning to the screen / output file" );

				if( displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to display the words that didn't pass the integrity check" );

				if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
				writtenSentenceStringBuffer.length() > 0 &&
				// Write interface warning to screen / output file
				InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, writtenSentenceStringBuffer.toString(), Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning to the screen / output file" );
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
		int offset = 0;
		int readUserSentenceStringLength = ( readUserSentenceString == null ? 0 : readUserSentenceString.length() );
		int readWordTypeStringLength;
		int wordPosition = 0;
		int writtenUserSentenceStringBufferLength;
		String readWordTypeString;
		StringBuffer writtenUserSentenceStringBuffer;
		ReadItem currentReadItem;
		ReadItem startNewSpecificationReadItem = null;
		WordItem generalizationWordItem;
		ReadWordResultType readWordResult;

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
											// The read user string and written user string are equal
											readUserSentenceString.substring( 1 ).compareTo( writtenUserSentenceStringBuffer.toString().substring( 1 ) ) == 0 );

			do	{
				do	{
					if( isEqualReadAndWriteUserString )
						currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;
					else
						{
						if( ( readWordTypeString = currentReadItem.readWordTypeString() ) != null )
							{
							readWordTypeStringLength = readWordTypeString.length();

							if( ( readWordResult = adminItem_.readWordFromString( false, false, false, false, readWordTypeStringLength, writtenUserSentenceStringBuffer.substring( wordPosition ) ) ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to read a word from the written string" );

							offset = readWordResult.offset;

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
										wordPosition += offset;
										lastFoundWordOrderNr = currentReadItem.wordOrderNr();
										startNewSpecificationReadItem = currentReadItem.nextReadItem();
										}
									}
								else
									{
									if( currentReadItem.isSkippingIntegrityCheckWords() ||

									// Typical for Chinese
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

				wordPosition += offset;
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
			// Do for all words touched during current sentence
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
		// Write user specifications
		writeUserSpecifications( isWritingCurrentSentenceOnly, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user specifications" );

		if( isWritingUserQuestions &&
		// Write user questions
		writeUserQuestions( isWritingCurrentSentenceOnly, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write user questions" );

		if( ( isWritingSelfGeneratedConclusions ||
		isWritingSelfGeneratedAssumptions ||
		isWritingSelfGeneratedQuestions ) &&

		// Write self-generated info
		writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated info" );

		if( isWritingSpecificationInfo &&
		!writeWordItem.isNounWordSpanishAmbiguous() &&
		// Write selected specification info
		writeSelectedSpecificationInfo( writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write selected specification info" );

		if( isWritingRelatedInfo &&
		// Write selected relation info
		writeSelectedRelationInfo( writeWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write selected related info" );

		return Constants.RESULT_OK;
		}
	}

/*************************************************************************
 *	"Give thanks to the Lord, for he is good!
 *	His faithful love endures forever." (Psalm 107:1)
 *************************************************************************/
