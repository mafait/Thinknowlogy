/*	Class:			AdminWriteSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To write selected specifications as sentences
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"

class AdminWriteSpecification
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_;

	bool isFirstSelfGeneratedAssumption_;
	bool isFirstSelfGeneratedConclusion_;
	bool isFirstSelfGeneratedQuestion_;
	bool isFirstUserQuestion_;
	bool isFirstUserSpecifications_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType showWordsThatDidntPassIntegrityCheckOfStoredUserSentence()
		{
		ReadItem *currentReadItem = adminItem_->firstActiveReadItem();
		char *readWordTypeString;
		char writeString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "showWordsThatDidntPassIntegrityCheckOfStoredUserSentence";

		do	{
			if( !currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence &&
			( readWordTypeString = currentReadItem->readWordTypeString() ) != NULL )
				{
				sprintf( writeString, "%s (position: %u, word type: %s, word parameter: %u, grammar parameter: %u)", readWordTypeString, currentReadItem->wordOrderNr(), adminItem_->wordTypeNameString( currentReadItem->wordTypeNr() ), currentReadItem->wordParameter(), currentReadItem->grammarParameter );

				if( commonVariables_->presentation->writeDiacriticalText( PRESENTATION_PROMPT_WARNING_INDENTED, writeString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification sentence" );
				}
			}
		while( ( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		return RESULT_OK;
		}

	ResultType writeSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		bool isForcingResponseNotBeingFirstSpecification;
		bool isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion;
		bool isSelfGenerated;
		bool hasFoundAnyChangeMadeByThisSentence = adminItem_->hasFoundAnyChangeMadeByThisSentence();
		bool hasHeaderBeenWritten = false;
		SpecificationItem *currentSpecificationItem;
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecification";

		if( writeWordItem != NULL )
			{
			if( ( currentSpecificationItem = writeWordItem->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != NULL )
				{
				do	{
					isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = false;

					isSelfGenerated = currentSpecificationItem->isSelfGenerated();

					if( currentSpecificationItem->isSpecificationWordSpanishAmbiguous() &&
					currentSpecificationItem->isPossessiveReversibleConclusion() &&
					currentSpecificationItem->hasSpecificationNonCompoundCollection() &&
					adminItem_->nContextWordsInAllWords( currentSpecificationItem->relationContextNr(), currentSpecificationItem->specificationWordItem() ) == 1 )
						{
						relationWordItem = currentSpecificationItem->relationWordItem();

						if( relationWordItem != NULL &&
						relationWordItem->isMale() )
							isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = true;
						}

					// Skip hidden specifications
					if( !isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion &&
					!currentSpecificationItem->isHiddenSpanishSpecification() &&

					// Conclusions
					( ( isWritingSelfGeneratedConclusions &&
					currentSpecificationItem->isSelfGeneratedConclusion() &&

					// Skip recently concluded assumptions with no new information
					( !currentSpecificationItem->isConcludedAssumption() ||
					!currentSpecificationItem->isOlderItem() ||
					!currentSpecificationItem->hasNewInformation() ) ) ||

					// Assumptions
					( isWritingSelfGeneratedAssumptions &&
					currentSpecificationItem->isSelfGeneratedAssumption() ) ||

					// Questions
					( isWritingSelfGeneratedQuestions &&
					currentSpecificationItem->isSelfGeneratedQuestion() ) ||

					( ( isWritingUserSpecifications &&
					currentSpecificationItem->isUserSpecification() &&
					!currentSpecificationItem->isCorrectedAssumption() ) ||

					( isWritingUserQuestions &&
					currentSpecificationItem->isUserQuestion() ) ) ) &&

					// Filter on current or updated sentences
					( !isWritingCurrentSentenceOnly ||
					// If nothing has changed, it will result in the notification: "I know".
					!hasFoundAnyChangeMadeByThisSentence ||
					currentSpecificationItem->hasNewInformation() ||
					currentSpecificationItem->wasHiddenSpanishSpecification() ) )
						{
						isForcingResponseNotBeingFirstSpecification = ( isAssignment &&
																		isSelfGenerated &&
																		currentSpecificationItem->isExclusiveSpecification() );

						if( writeWordItem->writeSelectedSpecification( false, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) == RESULT_OK )
							{
							if( strlen( commonVariables_->writtenSentenceString ) > 0 )
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
									if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isWritingSelfGeneratedConclusions ? INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? INTERFACE_LISTING_MY_ASSUMPTIONS : INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isSelfGenerated ? INTERFACE_LISTING_MY_INFORMATION : INTERFACE_LISTING_YOUR_INFORMATION ) : INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) == RESULT_OK )
										hasHeaderBeenWritten = false;
									else
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write my conclusions header" );
									}

								if( isWritingJustification )
									{
									if( isSelfGenerated )
										{
										if( adminItem_->writeJustificationSpecification( currentSpecificationItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification line" );
										}
									}
								else
									{
									if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a sentence" );
									}
								}
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a selected specification" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write word item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelfGeneratedSpecification( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedSpecification";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) == RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) == RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) == RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated archived assignments" );
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated past-tense assignments" );
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated current-tense assignments" );
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated specifications" );

		return RESULT_OK;
		}

	ResultType writeUserQuestions( bool isWritingCurrentSentenceOnly, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUserQuestions";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) == RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) == RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) == RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions with archived assignments" );
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions with past-tense assignments" );
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions with current-tense assignments" );
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions without assignments" );

		return RESULT_OK;
		}

	ResultType writeUserSpecifications( bool isWritingCurrentSentenceOnly, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUserSpecifications";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) == RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) == RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) == RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered archived assignments" );
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered past-tense assignments" );
				}
			else
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered current-tense assignments" );
			}
		else
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered specifications" );

		return RESULT_OK;
		}

	ResultType writeSelectedSpecificationInfo( WordItem *writeWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecificationInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					// Respond with active specifications
					if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( false, false, false, false, writeWordItem ) == RESULT_OK )
						{
						// Respond with active specification questions
						if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( false, false, false, true, writeWordItem ) == RESULT_OK )
							{
							// Respond with active assignments
							if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, false, false, writeWordItem ) == RESULT_OK )
								{
								// Respond with active assignment questions
								if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, false, true, writeWordItem ) == RESULT_OK )
									{
									// Respond with inactive assignments
									if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, true, false, false, writeWordItem ) == RESULT_OK )
										{
										// Respond with inactive assignment questions
										if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, true, false, true, writeWordItem ) == RESULT_OK )
											{
											// Respond with archived assignments
											if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, true, false, writeWordItem ) == RESULT_OK )
												{
												// Respond with archive assignment questions
												if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, true, true, writeWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archive assignment questions" );
												}
											else
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archived assignments" );
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write inactive assignment questions" );
										}
									else
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write inactive assignments" );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active assignment questions" );
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active assignments" );
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active specification questions" );
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active specifications" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType writeSelectedRelationInfo( WordItem *writeWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedRelationInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstRelationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					// Respond with active related specifications
					if( currentGeneralizationWordItem->writeSelectedRelationInfo( false, false, false, false, writeWordItem ) == RESULT_OK )
						{
						// Respond with active related specification questions
						if( currentGeneralizationWordItem->writeSelectedRelationInfo( false, false, false, true, writeWordItem ) == RESULT_OK )
							{
							// Respond with active related assignments
							if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, false, false, writeWordItem ) == RESULT_OK )
								{
								// Respond with active related assignment questions
								if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, false, true, writeWordItem ) == RESULT_OK )
									{
									// Respond with inactive related assignments
									if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, true, false, false, writeWordItem ) == RESULT_OK )
										{
										// Respond with inactive related assignment questions
										if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, true, false, true, writeWordItem ) == RESULT_OK )
											{
											// Respond with archive related assignments
											if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, true, false, writeWordItem ) == RESULT_OK )
												{
												// Respond with archive related assignment questions
												if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, true, true, writeWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archive related assignment questions" );
												}
											else
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archive related assignment" );
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write inactive related assignment questions" );
										}
									else
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related assignments" );
									}
								else
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related assignment assignments" );
								}
							else
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related assignments" );
							}
						else
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related specification questions" );
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related specifications" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "I have found an undefined generalization word" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextRelationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminWriteSpecification( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminWriteSpecification" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeAdminWriteVariables()
		{
		hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;
		}

	ResultType answerQuestions()
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "answerQuestions";

		if( commonVariables_->isQuestionAlreadyAnswered )
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'question is already answered' interface notification" );
			}

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all active words for an answer
			do	{
				if( currentWordItem->isWordTouchedDuringCurrentSentence )
					{
					if( currentWordItem->findAnswerToNewUserQuestion() != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find an answer to new questions of the user about word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

			if( !commonVariables_->hasFoundAnswerToQuestion &&
			!commonVariables_->isQuestionAlreadyAnswered )
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I don't know the answer to this question' interface notification" );
				}
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The first word item is undefined" );

		return RESULT_OK;
		}

	ResultType checkIntegrityOfStoredUserSentence( char *readUserSentenceString )
		{
		bool haveAllWordsPassed = true;
		bool hasFoundPluralQuestionVerb = false;
		ReadItem *currentReadItem = adminItem_->firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkIntegrityOfStoredUserSentence";

		if( currentReadItem != NULL )
			{
			do	{
				if( !currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence )
					haveAllWordsPassed = false;

				if( currentReadItem->isPluralQuestionVerb() )
					hasFoundPluralQuestionVerb = true;
				}
			while( haveAllWordsPassed &&
			( currentReadItem = currentReadItem->nextReadItem() ) != NULL );
			}

		if( !haveAllWordsPassed &&
		// Skip plural questions until implemented
		!hasFoundPluralQuestionVerb )
			{
			if( readUserSentenceString != NULL &&

			( adminItem_->isCurrentlyTesting() ||
			adminItem_->isSystemStartingUp() ) )
				{
				if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, EMPTY_STRING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START, readUserSentenceString, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS : INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE ) ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, EMPTY_STRING, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE_DUE_TO_WORDS : INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE ) ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}

			if( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ )
				{
				if( showWordsThatDidntPassIntegrityCheckOfStoredUserSentence() == RESULT_OK )
					{
					if( strlen( commonVariables_->writtenSentenceString ) > 0 )
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, commonVariables_->writtenSentenceString, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				else
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to show the words that didn't pass the integrity check" );
				}

			if( adminItem_->isSystemStartingUp() &&
			commonVariables_->hasShownWarning )
				return adminItem_->startError( functionNameString, moduleNameString_, "An integrity error occurred during startup" );
			}

		return RESULT_OK;
		}

	ResultType markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem *userSpecificationItem )
		{
		ReadResultType readResult;
		unsigned short lastFoundWordOrderNr = NO_ORDER_NR;
		size_t writtenUserSentenceStringLength;
		size_t readWordTypeStringLength;
		size_t wordPosition = 0;
		char *readWordTypeString;
		char *writtenUserSentenceString;
		ReadItem *currentReadItem;
		ReadItem *startNewSpecificationReadItem = NULL;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordsPassingIntegrityCheckOfStoredUserSentence";

		if( userSpecificationItem != NULL )
			{
			if( ( currentReadItem = adminItem_->firstActiveReadItem() ) != NULL )
				{
				if( ( generalizationWordItem = userSpecificationItem->generalizationWordItem() ) != NULL )
					{
					if( generalizationWordItem->writeSelectedSpecification( true, false, userSpecificationItem ) == RESULT_OK )
						{
						writtenUserSentenceString = commonVariables_->writtenUserSentenceString;

						if( ( writtenUserSentenceStringLength = strlen( writtenUserSentenceString ) ) > 0 )
							{
							do	{
								do	{
									if( ( readWordTypeString = currentReadItem->readWordTypeString() ) == NULL )
										// Hidden word type
										currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;
									else
										{
										readWordTypeStringLength = strlen( readWordTypeString );

										if( ( readResult = adminItem_->readWordFromString( false, false, false, wordPosition, readWordTypeStringLength, writtenUserSentenceString ) ).result == RESULT_OK )
											{
											if( readResult.wordLength > 0 &&
											!currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence )
												{
												if( readWordTypeStringLength == readResult.wordLength &&
												strncmp( &writtenUserSentenceString[wordPosition], readWordTypeString, readResult.wordLength ) == 0 )
													{
													hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = true;
													currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;

													if( lastFoundWordOrderNr == NO_ORDER_NR ||
													lastFoundWordOrderNr + 1 == currentReadItem->wordOrderNr() )
														{
														wordPosition = readResult.nextWordPosition;
														lastFoundWordOrderNr = currentReadItem->wordOrderNr();
														startNewSpecificationReadItem = currentReadItem->nextReadItem();
														}
													}
												else
													{
													// Skip on linked conjunctions.
													// Example: "Guest is a user and has no password."
													if( currentReadItem->isLinkedGeneralizationConjunction() ||

													// Skip on grammar conjunctions.
													// Example: "Expert is a user and his password is expert123."
													currentReadItem->isSentenceConjunction() ||

													// Skip on extra comma in sentence that isn't written.
													// See grammar file for: '( symbolComma )'
													// Example: "A creature is an animal, fungus, human-being, micro-organism, or plant."
													currentReadItem->isSymbol() ||

													// A singular adjective is given, but a plural adjective is found.
													// Example, given: "Laurent a un parent, appel�e Am�lie.",
													// but found: "Laurent a 2 parent [pluriel de �parent� est inconnue], appel�s Olivier et Am�lie."
													( currentReadItem->isAdjectiveCalledSingularFeminineOrMasculine() &&
													adminItem_->nContextWordsInAllWords( userSpecificationItem->relationContextNr(), userSpecificationItem->specificationWordItem() ) > 1 ) ||

													// Skip if indefinite article doesn't match with noun.
													// In that case, a warning will be shown.
													( currentReadItem->isArticle() &&

													( adminItem_->hasFoundDifferentParameter() ||
													commonVariables_->hasShownArticleNotification ||
													// And skip when an indefinite article is missing (because of a plural noun)
													currentReadItem->grammarParameter == GRAMMAR_GENERALIZATION_SPECIFICATION ) ) )
														// Skip until implemented
														currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;

													if( startNewSpecificationReadItem == NULL )
														startNewSpecificationReadItem = currentReadItem;
													}
												}
											}
										else
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a word from the written string" );
										}
									}
								while( readResult.wordLength > 0 &&
								( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

								wordPosition = readResult.nextWordPosition;
								currentReadItem = startNewSpecificationReadItem;
								}
							while( readResult.nextWordPosition < writtenUserSentenceStringLength );
							}
						}
					else
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the user specification of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given user specification item is undefined" );
				}
			else
				return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any read words" );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions )
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;

		if( ( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
			{
			// Do for all active words for self-generated info
			do	{
				if( currentWordItem->isWordTouchedDuringCurrentSentence )
					{
					if( writeInfoAboutWord( true, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, false, false, currentWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write info about word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, "The last predefined word item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelfGeneratedInfo( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		if( isWritingSelfGeneratedConclusions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, true, false, false, writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated conclusions" );
			}

		if( isWritingSelfGeneratedAssumptions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, true, false, writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated assumptions" );
			}

		if( isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, false, true, writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated questions" );
			}

		return RESULT_OK;
		}

	ResultType writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoAboutWord";

		if( isWritingUserSpecifications )
			{
			if( writeUserSpecifications( isWritingCurrentSentenceOnly, writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user specifications" );
			}

		if( isWritingUserQuestions )
			{
			if( writeUserQuestions( isWritingCurrentSentenceOnly, writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions" );
			}

		if( isWritingSelfGeneratedConclusions ||
		isWritingSelfGeneratedAssumptions ||
		isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the self-generated info" );
			}

		if( isWritingSpecificationInfo &&
		!writeWordItem->isNounWordSpanishAmbiguous() )
			{
			if( writeSelectedSpecificationInfo( writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write selected specification info" );
			}

		if( isWritingRelatedInfo )
			{
			if( writeSelectedRelationInfo( writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write selected related info" );
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lord, for he is good!
 *	His faithful love endures forever." (Psalm 107:1)
 *************************************************************************/
