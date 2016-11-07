/*	Class:			AdminWriteSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To write selected specifications as sentences
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

class AdminWriteSpecification
	{
	// Private constructed variables

	private boolean hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_;

	private boolean isFirstSelfGeneratedAssumption_;
	private boolean isFirstSelfGeneratedConclusion_;
	private boolean isFirstSelfGeneratedQuestion_;
	private boolean isFirstUserQuestion_;
	private boolean isFirstUserSpecifications_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence()
		{
		ReadItem currentReadItem = adminItem_.firstActiveReadItem();
		String readWordTypeString;

		do	{
			if( !currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence &&
			( readWordTypeString = currentReadItem.readWordTypeString() ) != null )
				{
				if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WARNING_INDENTED, ( readWordTypeString + " (position: " + currentReadItem.wordOrderNr() + ", word type: " + adminItem_.wordTypeNameString( currentReadItem.wordTypeNr() ) + ", word parameter: " + currentReadItem.wordParameter() + ", grammar parameter: " + currentReadItem.grammarParameter + ")" ) ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write a justification sentence" );
				}
			}
		while( ( currentReadItem = currentReadItem.nextReadItem() ) != null );

		return Constants.RESULT_OK;
		}

	private byte writeSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, WordItem writeWordItem )
		{
		boolean isCurrentSpecificationWordSpanishAmbiguous;
		boolean isForcingResponseNotBeingFirstSpecification;
		boolean isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion;
		boolean isSelfGenerated;
		boolean hasAnyChangeBeenMadeByThisSentence = adminItem_.hasAnyChangeBeenMadeByThisSentence();
		boolean hasHeaderBeenWritten = false;
		SpecificationItem currentSpecificationItem;
		WordItem singleRelationWordItem;

		if( writeWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = writeWordItem.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != null )
			{
			do	{
				isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = false;

				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem.isSpecificationWordSpanishAmbiguous();
				isSelfGenerated = currentSpecificationItem.isSelfGenerated();

				if( isCurrentSpecificationWordSpanishAmbiguous &&
				currentSpecificationItem.isPossessiveReversibleConclusion() &&
				currentSpecificationItem.hasSpecificationNonCompoundCollection() &&
				( singleRelationWordItem = currentSpecificationItem.singleRelationWordItem() ) != null )
					{
					if( singleRelationWordItem.isMale() )
						isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = true;
					}

				// Typically for the Spanish language
				if( !isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion &&

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
				// Typically for the Spanish language
				!currentSpecificationItem.isHiddenSpanishSpecification() ) &&

				// Filter on current or updated sentences
				( !isWritingCurrentSentenceOnly ||
				// If nothing has changed, it will result in the notification: "I know".
				!hasAnyChangeBeenMadeByThisSentence ||
				currentSpecificationItem.hasNewInformation() ||

				// Typically for the Spanish language
				( isCurrentSpecificationWordSpanishAmbiguous &&
				currentSpecificationItem.wasHiddenSpanishSpecification() ) ) )
					{
					isForcingResponseNotBeingFirstSpecification = ( isAssignment &&
																	isSelfGenerated &&
																	currentSpecificationItem.isExclusiveSpecification() );

					if( writeWordItem.writeSelectedSpecification( false, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write a selected specification" );

					if( CommonVariables.writtenSentenceStringBuffer != null &&
					CommonVariables.writtenSentenceStringBuffer.length() > 0 )
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
							if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isWritingSelfGeneratedConclusions ? Constants.INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS : Constants.INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isSelfGenerated ? Constants.INTERFACE_LISTING_MY_INFORMATION : Constants.INTERFACE_LISTING_YOUR_INFORMATION ) : Constants.INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write my conclusions header" );

							hasHeaderBeenWritten = false;
							}

						if( isWritingJustification )
							{
							if( isSelfGenerated )
								{
								if( adminItem_.writeJustificationSpecification( currentSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to write a justification line" );
								}
							}
						else
							{
							if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write a sentence" );
							}
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
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );

				// Respond with active specifications
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( false, false, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active specifications" );

				// Respond with active specification questions
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( false, false, false, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active specification questions" );

				// Respond with active assignments
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active assignments" );

				// Respond with active assignment questions
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, false, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write active assignment questions" );

				// Respond with inactive assignments
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, true, false, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write inactive assignments" );

				// Respond with inactive assignment questions
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, true, false, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write inactive assignment questions" );

				// Respond with archived assignments
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, true, false, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write archived assignments" );

				// Respond with archive assignment questions
				if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, true, true, writeWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write archive assignment questions" );
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
					return adminItem_.startError( 1, moduleNameString_, "I have found an undefined generalization word" );

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

	protected AdminWriteSpecification( AdminItem adminItem )
		{
		String errorString = null;

		hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
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

		if( CommonVariables.isQuestionAlreadyAnswered )
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'question is already answered' interface notification" );
			}

		if( ( currentTouchedWordItem = CommonVariables.firstTouchedWordItem ) != null )
			{
			// Do for all words touched during current sentence
			do	{
				if( currentTouchedWordItem.findAnswerToNewUserQuestion() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find an answer to new user questions of word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedDuringCurrentSentenceWordItem ) != null );
			}

		if( !CommonVariables.hasFoundAnswerToQuestion &&
		!CommonVariables.isQuestionAlreadyAnswered )
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I don't know the answer to this question' interface notification" );
			}

		return Constants.RESULT_OK;
		}

	protected byte checkIntegrityOfStoredUserSentence( String readUserSentenceString )
		{
		boolean haveAllWordsPassed = true;
		boolean hasFoundPluralQuestionVerb = false;
		ReadItem currentReadItem = adminItem_.firstActiveReadItem();

		if( currentReadItem != null )
			{
			do	{
				if( !currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence )
					haveAllWordsPassed = false;

				if( currentReadItem.isPluralQuestionVerb() )
					hasFoundPluralQuestionVerb = true;
				}
			while( haveAllWordsPassed &&
			( currentReadItem = currentReadItem.nextReadItem() ) != null );
			}

		if( !haveAllWordsPassed &&
		// Skip plural questions until implemented
		!hasFoundPluralQuestionVerb )
			{
			if( readUserSentenceString != null &&

			( adminItem_.isCurrentlyTesting() ||
			adminItem_.isSystemStartingUp() ) )
				{
				if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, Constants.EMPTY_STRING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START, readUserSentenceString, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS : Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE ) ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, Constants.EMPTY_STRING, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE_DUE_TO_WORDS : Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE ) ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}

			if( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ )
				{
				if( displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to display the words that didn't pass the integrity check" );

				if( CommonVariables.writtenSentenceStringBuffer != null &&
				CommonVariables.writtenSentenceStringBuffer.length() > 0 )
					{
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, CommonVariables.writtenSentenceStringBuffer.toString(), Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
					}
				}

			if( adminItem_.isSystemStartingUp() &&
			CommonVariables.hasDisplayedWarning )
				return adminItem_.startError( 1, moduleNameString_, "An integrity error occurred during startup" );
			}

		return Constants.RESULT_OK;
		}

	protected byte markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem userSpecificationItem )
		{
		ReadResultType readResult = new ReadResultType();
		short lastFoundWordOrderNr = Constants.NO_ORDER_NR;
		int writtenUserSentenceStringBufferLength;
		int readWordTypeStringLength;
		int wordPosition = 0;
		String readWordTypeString;
		StringBuffer writtenUserSentenceStringBuffer;
		ReadItem currentReadItem;
		ReadItem startNewSpecificationReadItem = null;
		WordItem generalizationWordItem;

		if( userSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given user specification item is undefined" );

		if( ( currentReadItem = adminItem_.firstActiveReadItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "I couldn't find any read words" );

		if( ( generalizationWordItem = userSpecificationItem.generalizationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( generalizationWordItem.writeSelectedSpecification( true, false, userSpecificationItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the user specification of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		writtenUserSentenceStringBuffer = CommonVariables.writtenUserSentenceStringBuffer;

		if( writtenUserSentenceStringBuffer != null &&
		( writtenUserSentenceStringBufferLength = writtenUserSentenceStringBuffer.length() ) > 0 )
			{
			do	{
				do	{
					if( ( readWordTypeString = currentReadItem.readWordTypeString() ) == null )
						// Hidden word type
						currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;
					else
						{
						readWordTypeStringLength = readWordTypeString.length();

						if( ( readResult = adminItem_.readWordFromString( false, false, false, wordPosition, readWordTypeStringLength, writtenUserSentenceStringBuffer.toString() ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to read a word from the written string" );

						if( readResult.wordLength > 0 &&
						!currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence )
							{
							if( readWordTypeStringLength == readResult.wordLength &&
							writtenUserSentenceStringBuffer.substring( wordPosition ).startsWith( readWordTypeString ) )
								{
								hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = true;
								currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;

								if( lastFoundWordOrderNr == Constants.NO_ORDER_NR ||
								lastFoundWordOrderNr + 1 == currentReadItem.wordOrderNr() )
									{
									wordPosition = readResult.nextWordPosition;
									lastFoundWordOrderNr = currentReadItem.wordOrderNr();
									startNewSpecificationReadItem = currentReadItem.nextReadItem();
									}
								}
							else
								{
								// Skip on linked conjunctions.
								// Example: "Guest is a user and has no password."
								if( currentReadItem.isLinkedGeneralizationConjunction() ||

								// Skip on grammar conjunctions.
								// Example: "Expert is a user and his password is expert123."
								currentReadItem.isSentenceConjunction() ||

								// Skip on extra comma in sentence that isn't written.
								// See grammar file for: '( symbolComma )'
								// Example: "A creature is an animal, fungus, human-being, micro-organism, or plant."
								currentReadItem.isSymbol() ||

								// A singular adjective is given, but a plural adjective is found.
								// Example, given: "Laurent a un parent, appelée Amélie.",
								// but found: "Laurent a 2 parent [pluriel de «parent» est inconnue], appelés Olivier et Amélie."
								( currentReadItem.isAdjectiveCalledSingularFeminineOrMasculine() &&
								adminItem_.nContextWordsInAllWords( userSpecificationItem.relationContextNr(), userSpecificationItem.specificationWordItem() ) > 1 ) ||

								// Skip if indefinite article doesn't match with noun.
								// In that case, a warning will be displayed.
								( currentReadItem.isArticle() &&

								( adminItem_.hasFoundDifferentParameter() ||
								CommonVariables.hasDisplayedArticleNotification ||
								// And skip when an indefinite article is missing (because of a plural noun)
								currentReadItem.grammarParameter == Constants.GRAMMAR_GENERALIZATION_SPECIFICATION ) ) )
									// Skip until implemented
									currentReadItem.hasWordPassedIntegrityCheckOfStoredUserSentence = true;

								if( startNewSpecificationReadItem == null )
									startNewSpecificationReadItem = currentReadItem;
								}
							}
						}
					}
				while( readResult.wordLength > 0 &&
				( currentReadItem = currentReadItem.nextReadItem() ) != null );

				wordPosition = readResult.nextWordPosition;
				currentReadItem = startNewSpecificationReadItem;
				}
			while( currentReadItem != null &&
			readResult.nextWordPosition < writtenUserSentenceStringBufferLength );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions )
		{
		WordItem currentTouchedWordItem;

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;

		if( ( currentTouchedWordItem = CommonVariables.firstTouchedWordItem ) != null )
			{
			// Do for all words touched during current sentence
			do	{
				if( writeInfoAboutWord( true, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, false, false, currentTouchedWordItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write selg-generated info of word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedDuringCurrentSentenceWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions, WordItem writeWordItem )
		{
		if( isWritingSelfGeneratedConclusions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, true, false, false, writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated conclusions" );
			}

		if( isWritingSelfGeneratedAssumptions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, true, false, writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated assumptions" );
			}

		if( isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, false, true, writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write self-generated questions" );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeInfoAboutWord( boolean isWritingCurrentSentenceOnly, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, boolean isWritingSpecificationInfo, boolean isWritingRelatedInfo, WordItem writeWordItem )
		{
		if( isWritingUserSpecifications )
			{
			if( writeUserSpecifications( isWritingCurrentSentenceOnly, writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write user specifications" );
			}

		if( isWritingUserQuestions )
			{
			if( writeUserQuestions( isWritingCurrentSentenceOnly, writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write user questions" );
			}

		if( isWritingSelfGeneratedConclusions ||
		isWritingSelfGeneratedAssumptions ||
		isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the self-generated info" );
			}

		if( isWritingSpecificationInfo &&
		!writeWordItem.isNounWordSpanishAmbiguous() )
			{
			if( writeSelectedSpecificationInfo( writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write selected specification info" );
			}

		if( isWritingRelatedInfo )
			{
			if( writeSelectedRelationInfo( writeWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write selected related info" );
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lord, for he is good!
 *	His faithful love endures forever." (Psalm 107:1)
 *************************************************************************/
