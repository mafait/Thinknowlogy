/*	Class:			AdminWrite
 *	Supports class:	AdminItem
 *	Purpose:		To write selected specifications as sentences
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"
#include "ReadWordResultType.cpp"

class AdminWrite
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_;
	bool isFirstSelfGeneratedAssumption_;
	bool isFirstSelfGeneratedConclusion_;
	bool isFirstSelfGeneratedQuestion_;
	bool isFirstUserQuestion_;
	bool isFirstUserSpecifications_;
	bool isWritingSeparator_;

	SpecificationItem *previousPrimarySpecificationItem_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;


	// Private functions

	signed char displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence()
		{
		ReadItem *currentReadItem = adminItem_->firstActiveReadItem();
		char *readWordTypeString;
		char writeString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence";

		do	{
			if( !currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence &&
			( readWordTypeString = currentReadItem->readWordTypeString() ) != NULL )
				{
				sprintf( writeString, "%s (position: %u, word type: %s, word parameter: %u, grammar parameter: %u)", readWordTypeString, currentReadItem->wordOrderNr(), adminItem_->wordTypeNameString( currentReadItem->wordTypeNr() ), currentReadItem->wordParameter(), currentReadItem->grammarParameter );

				if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INDENTED, writeString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification sentence" );
				}
			}
		while( ( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		return RESULT_OK;
		}

	signed char writeJustificationSpecification( SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool isFirstJustificationType = true;
		JustificationItem *firstJustificationItem;
		JustificationItem *currentJustificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecification";

		if( selfGeneratedSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );

		if( ( firstJustificationItem = selfGeneratedSpecificationItem->firstJustificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );

		if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );

		isWritingSeparator_ = true;

		generalizationWordItem->clearJustificationHasBeenWritten();
		currentJustificationItem = firstJustificationItem;

		do	{
			if( writeJustificationType( isFirstJustificationType, currentJustificationItem, selfGeneratedSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification type of a specification" );

			isFirstJustificationType = false;
			}
		while( ( currentJustificationItem = currentJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != NULL );

		return RESULT_OK;
		}

	signed char writeJustificationSpecifications( bool isWritingPrimarySpecification, bool isWritingExtraSeparator, JustificationItem *startJustificationItem )
		{
		bool isExclusiveSecondarySpecification;
		bool isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion;
		bool isOnlyWritingPrimarySpecification;
		bool isQuestionJustification;
		bool isFirstTime = true;
		bool isStop = false;
		JustificationItem *currentJustificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *previousSecondarySpecificationItem = NULL;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecifications";

		if( startJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start justification item is undefined" );

		currentJustificationItem = startJustificationItem;

		do	{
			isExclusiveSecondarySpecification = false;
			isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = false;
			isOnlyWritingPrimarySpecification = false;
			isQuestionJustification = currentJustificationItem->isQuestionJustification();

			primarySpecificationItem = currentJustificationItem->primarySpecificationItem();

			if( ( secondarySpecificationItem = currentJustificationItem->secondarySpecificationItem() ) != NULL )
				{
				if( secondarySpecificationItem->isExclusiveSpecification() )
					isExclusiveSecondarySpecification = true;
				}

			if( !isQuestionJustification &&
			primarySpecificationItem != NULL &&
			primarySpecificationItem->hasRelationContext() )
				{
				isWritingPrimarySpecification = true;
				isOnlyWritingPrimarySpecification = ( currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() != NULL );

				if( primarySpecificationItem->isSpecificationWordSpanishAmbiguous() &&
				primarySpecificationItem->isPossessiveReversibleConclusion() &&
				primarySpecificationItem->hasSpecificationNonCompoundCollection() &&
				primarySpecificationItem->isPossessive() &&

				( currentJustificationItem->isSpecificationSubstitutionAssumption() ||
				currentJustificationItem->isExclusiveSpecificationSubstitutionAssumption() ) &&

				primarySpecificationItem->hasOnlyOneRelationWord() )
					{
					isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = true;
					isWritingSeparator_ = false;
					}
				}

			if( !isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion )
				{
				if( isWritingPrimarySpecification ||

				( ( !isQuestionJustification &&

				( primarySpecificationItem == NULL ||

				( isFirstTime &&
				primarySpecificationItem != previousPrimarySpecificationItem_ ) ) ) ||

				( !isExclusiveSecondarySpecification &&
				primarySpecificationItem != NULL &&
				secondarySpecificationItem != NULL &&
				previousPrimarySpecificationItem_ != NULL &&
				!primarySpecificationItem->isCorrectedAssumption() &&
				secondarySpecificationItem != previousSecondarySpecificationItem &&
				primarySpecificationItem->specificationCollectionNr() == previousPrimarySpecificationItem_->specificationCollectionNr() &&

				// Skip possessive secondary specifications without relation
				( !secondarySpecificationItem->isPossessive() ||
				secondarySpecificationItem->hasRelationContext() ) ) ) )
					{
					if( isWritingExtraSeparator &&
					inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_JUSTIFICATION_AND ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an extra justification string" );

					if( writeSpecificationJustifications( isOnlyWritingPrimarySpecification, isWritingPrimarySpecification, currentJustificationItem, startJustificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the current justification specification" );

					isWritingSeparator_ = true;
					isFirstTime = false;
					isWritingPrimarySpecification = false;
					currentJustificationItem->hasJustificationBeenWritten = true;

					previousPrimarySpecificationItem_ = primarySpecificationItem;
					previousSecondarySpecificationItem = secondarySpecificationItem;

					if( secondarySpecificationItem != NULL &&
					primarySpecificationItem != NULL &&
					primarySpecificationItem->isExclusiveSpecification() &&
					primarySpecificationItem->isQuestion() &&
					( generalizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( generalizationWordItem->writeRelatedJustificationSpecifications( currentJustificationItem->justificationTypeNr(), secondarySpecificationItem->specificationCollectionNr() ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the related justification specifications" );

						previousPrimarySpecificationItem_ = NULL;
						}
					}
				else
					isStop = true;
				}
			}
		while( !isStop &&
		( currentJustificationItem = currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() ) != NULL );

		return RESULT_OK;
		}

	signed char writeJustificationType( bool isFirstJustificationType, JustificationItem *writeJustificationItem, SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool isExclusiveSpecification;
		bool isNegative;
		bool isNegativeAssumptionOrConclusion;
		bool isPossessive;
		bool isSelfGenerated;
		bool isWritingPrimarySpecificationOrExtraSeparator;
		bool hasNonExclusiveCollection = false;
		bool isFirstTime = true;
		unsigned short assumptionLevel;
		unsigned short specificationWordTypeNr;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		JustificationItem *currentJustificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *currentPrimarySpecificationItem;
		SpecificationItem *currentSecondarySpecificationItem;
		SpecificationItem *firstSecondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationType";

		previousPrimarySpecificationItem_ = NULL;

		if( writeJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write justification is undefined" );

		if( selfGeneratedSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );

		if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );

		if( isFirstJustificationType &&
		inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification sentence" );

		if( isWritingSeparator_ &&
		inputOutput_->writeInterfaceText( false, !isFirstJustificationType, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isFirstJustificationType ? INTERFACE_JUSTIFICATION_BECAUSE : INTERFACE_JUSTIFICATION_AND ) ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification start string" );

		if( writeJustificationSpecifications( true, false, writeJustificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification specifications" );

		isNegativeAssumptionOrConclusion = writeJustificationItem->isNegativeAssumptionOrConclusion();
		specificationCollectionNr = selfGeneratedSpecificationItem->specificationCollectionNr();

		if( ( firstSecondarySpecificationItem = writeJustificationItem->secondarySpecificationItem() ) != NULL )
			{
			if( isNegativeAssumptionOrConclusion &&
			firstSecondarySpecificationItem->hasSpecificationNonExclusiveCollection() )
				hasNonExclusiveCollection = true;
			}

		if( ( !isNegativeAssumptionOrConclusion ||
		hasNonExclusiveCollection ||

		( !writeJustificationItem->hasAnotherPrimarySpecification() &&
		writeJustificationItem->isPrimarySpecificationWordSpanishAmbiguous() ) ) &&

		( currentSpecificationItem = generalizationWordItem->firstSpecificationItem( false, selfGeneratedSpecificationItem->isAssignment(), selfGeneratedSpecificationItem->isInactiveAssignment(), selfGeneratedSpecificationItem->isArchivedAssignment(), selfGeneratedSpecificationItem->questionParameter() ) ) != NULL )
			{
			isExclusiveSpecification = selfGeneratedSpecificationItem->isExclusiveSpecification();
			isNegative = selfGeneratedSpecificationItem->isNegative();
			isPossessive = selfGeneratedSpecificationItem->isPossessive();
			isSelfGenerated = selfGeneratedSpecificationItem->isSelfGenerated();

			assumptionLevel = selfGeneratedSpecificationItem->assumptionLevel();
			specificationWordTypeNr = selfGeneratedSpecificationItem->specificationWordTypeNr();

			generalizationContextNr = selfGeneratedSpecificationItem->generalizationContextNr();
			specificationContextNr = selfGeneratedSpecificationItem->specificationContextNr();
			relationContextNr = selfGeneratedSpecificationItem->relationContextNr();

			do	{
				// Self-generated
				currentJustificationItem = currentSpecificationItem->firstJustificationItem();

				if( currentJustificationItem != NULL &&
				!currentJustificationItem->hasJustificationBeenWritten &&
				currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) )
					{
					currentPrimarySpecificationItem = currentJustificationItem->primarySpecificationItem();
					currentSecondarySpecificationItem = currentJustificationItem->secondarySpecificationItem();

					isWritingPrimarySpecificationOrExtraSeparator = ( currentPrimarySpecificationItem == NULL ||
																	// Suggestive assumption corrected by opposite question
																	currentPrimarySpecificationItem->isUserQuestion() ||

																	( currentSecondarySpecificationItem != NULL &&

																	( currentSecondarySpecificationItem->isGeneralizationProperName() ||

																	( !currentPrimarySpecificationItem->hasRelationContext() &&
																	currentPrimarySpecificationItem->isGeneralizationProperName() &&
																	!selfGeneratedSpecificationItem->isQuestion() ) ||

																	( isFirstTime &&
																	firstSecondarySpecificationItem != NULL &&
																	firstSecondarySpecificationItem->specificationCollectionNr() != currentSecondarySpecificationItem->specificationCollectionNr() ) ) ) );

					if( writeJustificationSpecifications( isWritingPrimarySpecificationOrExtraSeparator, isWritingPrimarySpecificationOrExtraSeparator, currentJustificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the current justification specifications" );

					isFirstTime = false;
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSelfGeneratedSpecification( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedSpecification";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated specifications" );

		// Active assignments
		if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated current-tense assignments" );

		// Inactive assignments
		if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated past-tense assignments" );

		// Archived assignments
		if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated archived assignments" );

		return RESULT_OK;
		}

	signed char writeSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		bool isCurrentSpecificationWordSpanishAmbiguous;
		bool isForcingResponseNotBeingFirstSpecification;
		bool isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion;
		bool isSelfGenerated;
		bool hasAnyChangeBeenMadeByThisSentence = adminItem_->hasAnyChangeBeenMadeByThisSentence();
		bool hasHeaderBeenWritten = false;
		SpecificationItem *currentSpecificationItem;
		WordItem *singleRelationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecification";

		if( writeWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = writeWordItem->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != NULL )
			{
			do	{
				isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = false;

				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem->isSpecificationWordSpanishAmbiguous();
				isSelfGenerated = currentSpecificationItem->isSelfGenerated();

				if( isCurrentSpecificationWordSpanishAmbiguous &&
				currentSpecificationItem->isPossessiveReversibleConclusion() &&
				currentSpecificationItem->hasSpecificationNonCompoundCollection() &&
				( singleRelationWordItem = currentSpecificationItem->singleRelationWordItem() ) != NULL )
					{
					if( singleRelationWordItem->isMale() )
						isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion = true;
					}

				// Typically for the Spanish language
				if( !isHiddenSingleRelationNonCompoundPossessiveReversibleConclusion &&

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

				( !isCurrentSpecificationWordSpanishAmbiguous ||
				// Typically for the Spanish language
				!currentSpecificationItem->isHiddenSpanishSpecification() ) &&

				// Filter on current or updated sentences
				( !isWritingCurrentSentenceOnly ||
				// If nothing has changed, it will result in the notification: "I know".
				!hasAnyChangeBeenMadeByThisSentence ||
				currentSpecificationItem->hasNewInformation() ||

				// Typically for the Spanish language
				( isCurrentSpecificationWordSpanishAmbiguous &&
				currentSpecificationItem->wasHiddenSpanishSpecification() ) ) )
					{
					isForcingResponseNotBeingFirstSpecification = ( isAssignment &&
																	isSelfGenerated &&
																	currentSpecificationItem->isExclusiveSpecification() );

					if( writeWordItem->writeSelectedSpecification( false, false, false, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a selected specification" );

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
							if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isWritingSelfGeneratedConclusions ? INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? INTERFACE_LISTING_MY_ASSUMPTIONS : INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isSelfGenerated ? INTERFACE_LISTING_MY_INFORMATION : INTERFACE_LISTING_YOUR_INFORMATION ) : INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write my conclusions header" );

							hasHeaderBeenWritten = false;
							}

						if( isWritingJustification )
							{
							if( isSelfGenerated &&
							writeJustificationSpecification( currentSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification line" );
							}
						else
							{
							if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a sentence" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSpecificationJustifications( bool isOnlyWritingPrimarySpecification, bool isWritingPrimarySpecification, JustificationItem *writeJustificationItem, JustificationItem *startJustificationItem )
		{
		bool isWritingCurrentSpecificationWordOnly;
		JustificationItem *currentJustificationItem = startJustificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem;
		SpecificationItem *currentPrimarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *primarySpecificationItem = NULL;
		WordItem *feminineOrMasculineProperNameEndingWordItem;
		WordItem *generalizationWordItem;
		WordItem *primaryGeneralizationWordItem = NULL;
		WordItem *lastWrittenFeminineOrMasculineProperNameEndingWordItem = NULL;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecificationJustifications";

		if( writeJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write justification item is undefined" );

		if( isWritingPrimarySpecification )
			{
			if( ( primarySpecificationItem = writeJustificationItem->primarySpecificationItem() ) != NULL )
				{
				if( ( primaryGeneralizationWordItem = primarySpecificationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given primary specification item is undefined" );

				if( primarySpecificationItem->isHiddenSpanishSpecification() )
					{
					sprintf( errorString, "\nThe primary specification item is a hidden specification:\n\tSpecificationItem: %s;\n\tJustificationItem: %s.\n", primarySpecificationItem->itemToString( NO_WORD_TYPE_NR ), writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( primaryGeneralizationWordItem->writeJustificationSpecification( ( !primarySpecificationItem->isExclusiveSpecification() && !primarySpecificationItem->isPossessive() ), primarySpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the definition justification specification" );
					}
				}

			if( !isOnlyWritingPrimarySpecification &&
			( anotherPrimarySpecificationItem = writeJustificationItem->anotherPrimarySpecificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = anotherPrimarySpecificationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given another primary specification item is undefined" );

				if( anotherPrimarySpecificationItem->isHiddenSpanishSpecification() )
					{
					sprintf( errorString, "\nThe another primary specification item is a hidden specification:\n\tSpecificationItem: %s;\n\tJustificationItem: %s.\n", anotherPrimarySpecificationItem->itemToString( NO_WORD_TYPE_NR ), writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( generalizationWordItem->writeJustificationSpecification( false, anotherPrimarySpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the another definition justification specification" );
					}
				}
			}

		if( !isOnlyWritingPrimarySpecification )
			{
			if( ( secondarySpecificationItem = writeJustificationItem->secondarySpecificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = secondarySpecificationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given secondary specification item is undefined" );

				if( secondarySpecificationItem->isHiddenSpanishSpecification() )
					{
					sprintf( errorString, "\nThe secondary specification item is a hidden specification:\n\tSpecificationItem: %s;\n\tJustificationItem: %s.\n", secondarySpecificationItem->itemToString( NO_WORD_TYPE_NR ), writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					isWritingCurrentSpecificationWordOnly = ( secondarySpecificationItem->isPartOf() ||

															( !secondarySpecificationItem->isExclusiveSpecification() &&
															secondarySpecificationItem->isGeneralizationProperName() &&
															!secondarySpecificationItem->isPossessive() ) );

					if( generalizationWordItem->writeJustificationSpecification( isWritingCurrentSpecificationWordOnly, secondarySpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the secondary justification specification" );
					}
				}

			if( ( anotherSecondarySpecificationItem = writeJustificationItem->anotherSecondarySpecificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = anotherSecondarySpecificationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given another secondary specification item is undefined" );

				if( anotherSecondarySpecificationItem->isHiddenSpanishSpecification() )
					{
					sprintf( errorString, "\nThe another secondary specification item is a hidden specification:\n\tSpecificationItem: %s;\n\tJustificationItem: %s.\n", anotherSecondarySpecificationItem->itemToString( NO_WORD_TYPE_NR ), writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( generalizationWordItem->writeJustificationSpecification( false, anotherSecondarySpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the another secondary justification specification" );
					}
				}

			// Now display list of feminine/masculine assumptions
			if( isWritingPrimarySpecification &&
			currentJustificationItem != NULL &&
			writeJustificationItem->hasFeminineOrMasculineProperNameEnding() )
				{
				do	{
					if( currentJustificationItem->hasFeminineOrMasculineProperNameEnding() )
						{
						currentPrimarySpecificationItem = currentJustificationItem->primarySpecificationItem();

						if( ( feminineOrMasculineProperNameEndingWordItem = ( currentPrimarySpecificationItem != NULL && currentPrimarySpecificationItem->isGeneralizationProperName() && currentPrimarySpecificationItem->isPossessive() ? currentPrimarySpecificationItem->generalizationWordItem() : currentJustificationItem->generalizationWordItem() ) ) == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the feminine or masculine proper name ending word" );

						if( feminineOrMasculineProperNameEndingWordItem != lastWrittenFeminineOrMasculineProperNameEndingWordItem )
							{
							if( inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_WRITE_INDENTED, INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING, feminineOrMasculineProperNameEndingWordItem->anyWordTypeString(), ( feminineOrMasculineProperNameEndingWordItem->hasFeminineProperNameEnding() ? INTERFACE_JUSTIFICATION_FEMININE_PROPER_NAME_ENDING : INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NAME_ENDING ) ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification sentence start string" );

							lastWrittenFeminineOrMasculineProperNameEndingWordItem = feminineOrMasculineProperNameEndingWordItem;
							}
						}
					}
				while( ( currentJustificationItem = currentJustificationItem->attachedJustificationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char writeUserQuestions( bool isWritingCurrentSentenceOnly, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUserQuestions";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions without assignments" );

		// Active assignments
		if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions with current-tense assignments" );

		// Inactive assignments
		if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions with past-tense assignments" );

		// Archived assignments
		if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, false, false, false, true, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions with archived assignments" );

		return RESULT_OK;
		}

	signed char writeUserSpecifications( bool isWritingCurrentSentenceOnly, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUserSpecifications";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered specifications" );

		// Active assignments
		if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered current-tense assignments" );

		// Inactive assignments
		if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered past-tense assignments" );

		// Archived assignments
		if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, true, false, false, false, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user-entered archived assignments" );

		return RESULT_OK;
		}

	signed char writeSelectedSpecificationInfo( WordItem *writeWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecificationInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				// Respond with active specifications
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( false, false, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active specifications" );

				// Respond with active specification questions
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( false, false, false, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active specification questions" );

				// Respond with active assignments
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active assignments" );

				// Respond with active assignment questions
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, false, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active assignment questions" );

				// Respond with inactive assignments
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, true, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write inactive assignments" );

				// Respond with inactive assignment questions
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, true, false, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write inactive assignment questions" );

				// Respond with archived assignments
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, true, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archived assignments" );

				// Respond with archive assignment questions
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, true, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archive assignment questions" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSelectedRelationInfo( WordItem *writeWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedRelationInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstRelationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				// Respond with active related specifications
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( false, false, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related specifications" );

				// Respond with active related specification questions
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( false, false, false, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related specification questions" );

				// Respond with active related assignments
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related assignments" );

				// Respond with active related assignment questions
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, false, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related assignment assignments" );

				// Respond with inactive related assignments
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, true, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active related assignments" );

				// Respond with inactive related assignment questions
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, true, false, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write inactive related assignment questions" );

				// Respond with archive related assignments
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, true, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archive related assignment" );

				// Respond with archive related assignment questions
				if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, true, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archive related assignment questions" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextRelationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

	AdminWrite( AdminItem *adminItem, CommonVariables *commonVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;
		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;
		isWritingSeparator_ = false;

		previousPrimarySpecificationItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminWrite" );

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( commonVariables != NULL )
				commonVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( commonVariables_ = commonVariables ) == NULL )
				strcpy( errorString, "The given common variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
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

	signed char answerQuestions()
		{
		WordItem *currentTouchedProperNameWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "answerQuestions";

		if( commonVariables_->isQuestionAlreadyAnswered &&
		inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'question is already answered' interface notification" );

		if( ( currentTouchedProperNameWordItem = commonVariables_->firstTouchedProperNameWordItem ) != NULL )
			{
			// Do for all proper name words touched during current sentence
			do	{
				if( currentTouchedProperNameWordItem->findAnswerToNewUserQuestion() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find an answer to new user questions of word \"", currentTouchedProperNameWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedProperNameWordItem = currentTouchedProperNameWordItem->nextTouchedProperNameWordItem ) != NULL );
			}

		if( !commonVariables_->hasFoundAnswerToQuestion &&
		!commonVariables_->isQuestionAlreadyAnswered &&
		inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I don't know the answer to this question' interface notification" );

		return RESULT_OK;
		}

	signed char checkIntegrityOfStoredUserSentence( char *readUserSentenceString )
		{
		bool hasFoundPluralQuestionVerb = false;
		bool haveAllWordsPassed = true;
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
				if( inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, EMPTY_STRING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START, readUserSentenceString, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS : INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE ) ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, EMPTY_STRING, ( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ ? INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE_DUE_TO_WORDS : INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE ) ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}

			if( hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ )
				{
				if( displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to display the words that didn't pass the integrity check" );

				if( strlen( commonVariables_->writtenSentenceString ) > 0 )
					{
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, commonVariables_->writtenSentenceString, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				}

			if( adminItem_->isSystemStartingUp() &&
			commonVariables_->hasDisplayedWarning )
				return adminItem_->startError( functionNameString, moduleNameString_, "An integrity error occurred during startup" );
			}

		return RESULT_OK;
		}

	signed char markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem *userSpecificationItem )
		{
		ReadWordResultType readWordResult;
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

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( ( currentReadItem = adminItem_->firstActiveReadItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any read words" );

		if( ( generalizationWordItem = userSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( generalizationWordItem->writeSelectedSpecification( true, false, userSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the user specification of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

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

						if( ( readWordResult = adminItem_->readWordFromString( false, false, false, wordPosition, readWordTypeStringLength, writtenUserSentenceString ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a word from the written string" );

						if( readWordResult.wordLength > 0 &&
						!currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence )
							{
							if( readWordTypeStringLength == readWordResult.wordLength &&
							strncmp( &writtenUserSentenceString[wordPosition], readWordTypeString, readWordResult.wordLength ) == 0 )
								{
								hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = true;
								currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;

								if( lastFoundWordOrderNr == NO_ORDER_NR ||
								lastFoundWordOrderNr + 1 == currentReadItem->wordOrderNr() )
									{
									wordPosition = readWordResult.nextWordPosition;
									lastFoundWordOrderNr = currentReadItem->wordOrderNr();
									startNewSpecificationReadItem = currentReadItem->nextReadItem();
									}
								}
							else
								{
								// Skip checking on linked conjunctions.
								// Example: "Guest is a user and has no password."
								if( currentReadItem->isLinkedGeneralizationConjunction() ||

								// Skip checking on grammar conjunctions.
								// Example: "Expert is a user and his password is expert123."
								currentReadItem->isSentenceConjunction() ||

								// Skip checking on extra comma in sentence that isn't written.
								// See grammar file for: '( symbolComma )'
								// Example: "A creature is an animal, fungus, human-being, micro-organism, or plant."
								currentReadItem->isSymbol() ||

								// In French, a singular adjective is given ('appelée'), but a plural adjective is found ('appelés').
								// Example, given: "Pascal a un parent, appelée Amélie.", but found:
								// "Pascal a 2 parent [pluriel de «parent» est inconnue], appelés Olivier et Amélie."
								// Skip checking on this adjective.
								( currentReadItem->isAdjectiveCalledSingularFeminineOrMasculine() &&
								adminItem_->nContextWordsInContextWords( userSpecificationItem->relationContextNr(), userSpecificationItem->specificationWordItem() ) > 1 ) ||

								// Skip checking if indefinite article doesn't match with noun.
								// In that case, a warning will be displayed.
								( currentReadItem->isArticle() &&

								( adminItem_->hasFoundDifferentParameter() ||
								commonVariables_->hasDisplayedArticleNotification ||
								// And skip when an indefinite article is missing (because of a plural noun)
								currentReadItem->grammarParameter == GRAMMAR_GENERALIZATION_SPECIFICATION ) ) )
									// Skip until implemented
									currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;

								if( startNewSpecificationReadItem == NULL )
									startNewSpecificationReadItem = currentReadItem;
								}
							}
						}
					}
				while( readWordResult.wordLength > 0 &&
				( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

				wordPosition = readWordResult.nextWordPosition;
				currentReadItem = startNewSpecificationReadItem;
				}
			while( currentReadItem != NULL &&
			readWordResult.nextWordPosition < writtenUserSentenceStringLength );
			}

		return RESULT_OK;
		}

	signed char writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions )
		{
		WordItem *currentTouchedProperNameWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;

		if( ( currentTouchedProperNameWordItem = commonVariables_->firstTouchedProperNameWordItem ) != NULL )
			{
			// Do for all proper name words touched during current sentence
			do	{
				if( writeInfoAboutWord( true, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, false, false, currentTouchedProperNameWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write selg-generated info of word \"", currentTouchedProperNameWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedProperNameWordItem = currentTouchedProperNameWordItem->nextTouchedProperNameWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSelfGeneratedInfo( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		if( isWritingSelfGeneratedConclusions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, true, false, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated conclusions" );

		if( isWritingSelfGeneratedAssumptions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, true, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated assumptions" );

		if( isWritingSelfGeneratedQuestions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, false, true, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated questions" );

		return RESULT_OK;
		}

	signed char writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoAboutWord";

		if( isWritingUserSpecifications &&
		writeUserSpecifications( isWritingCurrentSentenceOnly, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user specifications" );

		if( isWritingUserQuestions &&
		writeUserQuestions( isWritingCurrentSentenceOnly, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions" );

		if( isWritingSelfGeneratedConclusions ||
		isWritingSelfGeneratedAssumptions ||
		isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the self-generated info" );
			}

		if( isWritingSpecificationInfo &&
		!writeWordItem->isNounWordSpanishAmbiguous() &&
		writeSelectedSpecificationInfo( writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write selected specification info" );

		if( isWritingRelatedInfo &&
		writeSelectedRelationInfo( writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write selected related info" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Give thanks to the Lord, for he is good!
 *	His faithful love endures forever." (Psalm 107:1)
 *************************************************************************/
