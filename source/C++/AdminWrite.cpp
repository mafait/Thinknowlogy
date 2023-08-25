/*	Class:			AdminWrite
 *	Supports class:	AdminItem
 *	Purpose:		Writing selected specifications as sentences
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"
#include "ReadWordResultType.cpp"

class AdminWrite
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = false;
	bool isFirstSelfGeneratedAssumption_ = false;
	bool isFirstSelfGeneratedConclusion_ = false;
	bool isFirstSelfGeneratedQuestion_ = false;
	bool isFirstUserSpecifications_ = false;

	JustificationItem *lastWrittenJustificationItem_ = NULL;

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminWrite";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;


	// Private functions

	signed char displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence()
		{
		ReadItem *currentReadItem = adminItem_->firstActiveReadItem();
		char *readWordTypeString;
		char writeString[SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displayWordsThatDidntPassIntegrityCheckOfStoredUserSentence";

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
		JustificationItem *currentJustificationItem;
		JustificationItem *firstJustificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeJustificationSpecification";

		if( selfGeneratedSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );

		if( ( firstJustificationItem = selfGeneratedSpecificationItem->firstJustificationItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The first justification of the given self-generated specification item is undefined" );

		if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );

		generalizationWordItem->clearJustificationHasBeenWritten();

		lastWrittenJustificationItem_ = NULL;
		currentJustificationItem = firstJustificationItem;

		do	{
			if( writeJustificationType( isFirstJustificationType, currentJustificationItem, selfGeneratedSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification type of a specification" );

			isFirstJustificationType = false;
			}
		while( ( currentJustificationItem = currentJustificationItem->nextJustificationItemWithDifferentTypeOrOrderNr( firstJustificationItem ) ) != NULL );

		return RESULT_OK;
		}

	signed char writeJustificationSpecifications( bool isWritingPrimarySpecification, bool isWritingSeparator, JustificationItem *startJustificationItem, SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool hasPrimarySpecificationRelationContext;
		bool isExclusiveSecondarySpecification;
		bool isFirstTime = true;
		bool isForcingSeparator = false;
		bool isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification;
		bool isOnlyWritingPrimarySpecification;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		bool isPossessivePrimarySpecification;
		bool isQuestionJustification;
		bool isStop = false;
		JustificationItem *currentJustificationItem;
		SpecificationItem *previousPrimarySpecificationItem = ( lastWrittenJustificationItem_ == NULL ? NULL : lastWrittenJustificationItem_->primarySpecificationItem() );
		SpecificationItem *previousSecondarySpecificationItem = NULL;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeJustificationSpecifications";

		if( startJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start justification item is undefined" );

		currentJustificationItem = startJustificationItem;

		do	{
			isExclusiveSecondarySpecification = false;
			isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification = false;
			isOnlyWritingPrimarySpecification = false;
			isQuestionJustification = currentJustificationItem->isQuestionJustification();

			if( ( secondarySpecificationItem = currentJustificationItem->secondarySpecificationItem() ) != NULL &&
			secondarySpecificationItem->isExclusiveSpecification() )
				isExclusiveSecondarySpecification = true;

			if( !isWritingPrimarySpecification &&
			secondarySpecificationItem == NULL )
				isForcingSeparator = true;

			if( ( primarySpecificationItem = currentJustificationItem->primarySpecificationItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The primary specification of the current justification is undefined" );

			hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
			isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();
			isPossessivePrimarySpecification = primarySpecificationItem->isPossessive();

			if( ( !isQuestionJustification &&

			( hasPrimarySpecificationRelationContext ||
			isForcingSeparator ) ) ||

			( isQuestionJustification &&
			isPossessivePrimarySpecification ) )
				{
				isWritingPrimarySpecification = true;

				isOnlyWritingPrimarySpecification = ( currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() != NULL );

				if( isPrimarySpecificationWordSpanishAmbiguous &&
				isPossessivePrimarySpecification &&
				!primarySpecificationItem->hasCompoundSpecificationCollection() &&
				primarySpecificationItem->isSelfGeneratedConclusion() &&
				primarySpecificationItem->hasOnlyOneRelationWord() &&
				!currentJustificationItem->isSpecificationSubstitutionPartOfAssumptionOrConclusion() )
					isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification = true;
				}

			if( isWritingPrimarySpecification ||

			( ( isFirstTime &&
			!isQuestionJustification &&
			primarySpecificationItem != previousPrimarySpecificationItem ) ||

			( !isExclusiveSecondarySpecification &&
			previousPrimarySpecificationItem != NULL &&
			secondarySpecificationItem != previousSecondarySpecificationItem ) ) )
				{
				if( isFirstTime &&

				( isWritingSeparator ||
				isForcingSeparator ) &&

				inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_JUSTIFICATION_AND ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an extra justification string" );

				if( isQuestionJustification &&
				hasPrimarySpecificationRelationContext &&
				!isPossessivePrimarySpecification &&
				!isPrimarySpecificationWordSpanishAmbiguous &&
				// Next justification with same type and order number exists
				currentJustificationItem->nextJustificationItemWithSameTypeAndOrderNr() != NULL )
					isOnlyWritingPrimarySpecification = true;

				if( writeSpecificationJustifications( isOnlyWritingPrimarySpecification, ( isWritingPrimarySpecification && !isHidingAlmostSimilarSpanishAmbiguousPrimarySpecification ), isWritingPrimarySpecification, currentJustificationItem, startJustificationItem, selfGeneratedSpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the current justification specification" );

				isFirstTime = false;

				if( !isOnlyWritingPrimarySpecification )
					isWritingPrimarySpecification = false;

				currentJustificationItem->hasJustificationBeenWritten = true;

				lastWrittenJustificationItem_ = currentJustificationItem;
				previousPrimarySpecificationItem = primarySpecificationItem;
				previousSecondarySpecificationItem = secondarySpecificationItem;

				if( primarySpecificationItem->isSelfGeneratedQuestion() &&

				( currentJustificationItem->isQuestionJustification() ||
				// Correct invalid assumption by opposite suggestive question
				primarySpecificationItem->isCorrectedSpecification() ) &&

				( generalizationWordItem = primarySpecificationItem->generalizationWordItem() ) != NULL )
					{
					if( generalizationWordItem->writeRelatedJustificationSpecifications( currentJustificationItem->justificationTypeNr(), currentJustificationItem->justificationOrderNr, currentJustificationItem->secondarySpecificationCollectionNr() ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the related justification specifications" );

					previousPrimarySpecificationItem = NULL;
					lastWrittenJustificationItem_ = NULL;
					}
				}
			else
				isStop = true;
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
		bool isNegativePrimarySpecification = false;
		bool isPossessive;
		bool isQuestion;
		bool isSelfGeneratedQuestion;
		bool isSpecificationGeneralization;
		bool isWritingPrimarySpecificationOrSeparator;
		unsigned short assumptionLevel;
		unsigned short selectedJustificationTypeNr;
		unsigned short specificationWordTypeNr;
		unsigned int generalizationContextNr;
		unsigned int relationContextNr;
		unsigned int specificationCollectionNr;
		JustificationItem *currentJustificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *currentPrimarySpecificationItem;
		SpecificationItem *currentSecondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeJustificationType";

		if( writeJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write justification is undefined" );

		if( selfGeneratedSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given self-generated specification item is undefined" );

		if( ( generalizationWordItem = selfGeneratedSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given self-generated specification item is undefined" );

		if( isFirstJustificationType )
			{
			if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, globalVariables_->writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification sentence" );

			if( inputOutput_->writeInterfaceText( false, false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_JUSTIFICATION_BECAUSE ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification start string" );
			}

		// Write justifications of this specification
		if( writeJustificationSpecifications( true, !isFirstJustificationType, writeJustificationItem, selfGeneratedSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification specifications" );

		// Find a related specification
		if( ( currentSpecificationItem = generalizationWordItem->firstSpecificationItem( false, selfGeneratedSpecificationItem->isAssignment(), selfGeneratedSpecificationItem->isInactiveAssignment(), selfGeneratedSpecificationItem->isArchivedAssignment(), selfGeneratedSpecificationItem->questionParameter() ) ) != NULL &&
		// Found a related specification
		currentSpecificationItem != selfGeneratedSpecificationItem )
			{
			isExclusiveSpecification = selfGeneratedSpecificationItem->isExclusiveSpecification();
			isNegativeAssumptionOrConclusion = writeJustificationItem->isNegativeAssumptionOrConclusion();

			if( isNegativeAssumptionOrConclusion &&
			!currentSpecificationItem->hasRelationContext() &&
			writeJustificationItem->isNegativePrimarySpecification() )
				isNegativePrimarySpecification = true;

			if( !isNegativeAssumptionOrConclusion ||

			( isNegativePrimarySpecification &&
			currentSpecificationItem->hasNonCompoundSpecificationCollection() ) )
				{
				isNegative = selfGeneratedSpecificationItem->isNegative();
				isPossessive = selfGeneratedSpecificationItem->isPossessive();
				isQuestion = selfGeneratedSpecificationItem->isQuestion();
				isSpecificationGeneralization = selfGeneratedSpecificationItem->isSpecificationGeneralization();
				assumptionLevel = selfGeneratedSpecificationItem->assumptionLevel();
				specificationCollectionNr = selfGeneratedSpecificationItem->specificationCollectionNr();
				specificationWordTypeNr = selfGeneratedSpecificationItem->specificationWordTypeNr();
				generalizationContextNr = selfGeneratedSpecificationItem->generalizationContextNr();
				relationContextNr = selfGeneratedSpecificationItem->relationContextNr();

				do	{
					if( lastWrittenJustificationItem_ != NULL &&
					currentSpecificationItem != selfGeneratedSpecificationItem &&
					currentSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, true, assumptionLevel, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, relationContextNr ) != NULL )
						{
						currentPrimarySpecificationItem = writeJustificationItem->primarySpecificationItem();

						isSelfGeneratedQuestion = ( ( currentSecondarySpecificationItem = writeJustificationItem->secondarySpecificationItem() ) != NULL &&
													selfGeneratedSpecificationItem->isQuestion() );

														// Test file: "No conflict on multiple related specifications"
						selectedJustificationTypeNr = ( isPossessive ? JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION :

														// Test files: "Collect afterwards - Past tense" and "Collect afterwards - Uncertainty"
														( isSpecificationGeneralization ? JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION :

														// Test file: "Correcting invalidated assumption (by opposite suggestive question)"
														( 
										currentSecondarySpecificationItem == NULL &&
										currentSpecificationItem->isCorrectedSpecification() ? lastWrittenJustificationItem_->justificationTypeNr() :
														// Test file: "This information is more specific (non-exclusive)"
														( isSelfGeneratedQuestion &&
														currentPrimarySpecificationItem != NULL &&
														!currentPrimarySpecificationItem->isPossessive() ? JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION :
																											JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) ) ) );

						// Find related justification in current specification
						currentJustificationItem = ( isExclusiveSpecification ? currentSpecificationItem->firstJustificationItem( selectedJustificationTypeNr, lastWrittenJustificationItem_->justificationOrderNr ) :
																				currentSpecificationItem->firstJustificationItem( selectedJustificationTypeNr ) );

						if( currentJustificationItem != NULL &&

						// Exclude question justification with primary specification that has relation context
						( !isQuestion ||
						// Suggestive question assumption
						currentSecondarySpecificationItem == NULL ||
						!currentJustificationItem->hasPrimarySpecificationRelationContext() ) )
							{
							isWritingPrimarySpecificationOrSeparator = ( ( currentSecondarySpecificationItem == NULL &&
																		currentJustificationItem->isSuggestiveQuestionAssumption() ) ||

																		( currentSecondarySpecificationItem != NULL &&

																		( currentSecondarySpecificationItem->isGeneralizationProperNoun() ||

																		( !isSelfGeneratedQuestion &&
																		currentPrimarySpecificationItem != NULL &&
																		currentPrimarySpecificationItem->isGeneralizationProperNoun() ) ) ) );

							// Write related justification specifications
							if( writeJustificationSpecifications( isWritingPrimarySpecificationOrSeparator, isWritingPrimarySpecificationOrSeparator, currentJustificationItem, NULL ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write related justification specifications" );
							}
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char writeSelfGeneratedSpecification( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelfGeneratedSpecification";

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
		bool hasCurrentSpecificationAssumptionLevel;
		bool hasCurrentSpecificationRelationContext;
		bool hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore;
		bool hasHeaderBeenWritten = false;
		bool isCurrentSpecificationSelfGenerated;
		bool isCurrentSpecificationWordSpanishAmbiguous;
		bool isHiddenSpanishSpecification;
		bool isOnlyOptionLeftAssignment;
		SpecificationItem *currentSpecificationItem;
		WordItem *singleRelationWordItem;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecification";

		if( writeWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write word item is undefined" );

		hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = writeWordItem->hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore;

		if( ( currentSpecificationItem = writeWordItem->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != NULL )
			{
			do	{
				hasCurrentSpecificationAssumptionLevel = currentSpecificationItem->hasAssumptionLevel();
				hasCurrentSpecificationRelationContext = currentSpecificationItem->hasRelationContext();
				isCurrentSpecificationSelfGenerated = currentSpecificationItem->isSelfGeneratedSpecification();
				isCurrentSpecificationWordSpanishAmbiguous = currentSpecificationItem->isSpecificationWordSpanishAmbiguous();
				isHiddenSpanishSpecification = false;

				if( isCurrentSpecificationWordSpanishAmbiguous &&
				hasCurrentSpecificationRelationContext &&
				currentSpecificationItem->isFirstJustificationReversibleAssumptionOrConclusion() )
					{
					if( ( currentSpecificationItem->isOlderItem() &&
					currentSpecificationItem->isSelfGeneratedAssumption() &&
					writeWordItem->hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore ) ||

					( currentSpecificationItem->isPossessive() &&
					currentSpecificationItem->hasNonCompoundSpecificationCollection() &&
					( singleRelationWordItem = currentSpecificationItem->singleRelationWordItem() ) != NULL &&
					singleRelationWordItem->isMasculineWord() ) )
						isHiddenSpanishSpecification = true;
					}

				if( !isHiddenSpanishSpecification &&

				// Conclusions
				( ( isWritingSelfGeneratedConclusions &&
				!hasCurrentSpecificationAssumptionLevel &&
				isCurrentSpecificationSelfGenerated &&

				( !hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore ||
				// Typical for Spanish
				!hasCurrentSpecificationRelationContext ||
				currentSpecificationItem->hasOnlyOneRelationWord() ) ) ||

				// Assumptions
				( isWritingSelfGeneratedAssumptions &&
				hasCurrentSpecificationAssumptionLevel &&
				isCurrentSpecificationSelfGenerated ) ||

				// Questions
				( isWritingSelfGeneratedQuestions &&
				currentSpecificationItem->isSelfGeneratedQuestion() ) ||

				( isWritingUserSpecifications &&
				currentSpecificationItem->isUserSpecification() ) ||

				( isWritingUserQuestions &&
				currentSpecificationItem->isUserQuestion() ) ) &&

				( !isCurrentSpecificationWordSpanishAmbiguous ||
				// Typical for Spanish
				!currentSpecificationItem->isHiddenSpanishSpecification() ) &&

				// Filter on current or updated sentences
				( !isWritingCurrentSentenceOnly ||
				currentSpecificationItem->hasNewInformation() ||

				// Typical for Spanish
				// Spanish test filea: "Complejo (15)", "Complejo (17)" and "Preguntas respondidas indirectamente - Juan es masculino"
				( isCurrentSpecificationWordSpanishAmbiguous &&
				currentSpecificationItem->wasHiddenSpanishSpecification() ) ) )
					{
					isOnlyOptionLeftAssignment = ( isAssignment &&
													isCurrentSpecificationSelfGenerated &&
													currentSpecificationItem->isExclusiveSpecification() );

					if( writeWordItem->writeSelectedSpecification( false, false, isOnlyOptionLeftAssignment, false, isWritingCurrentSentenceOnly, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a selected specification" );

					if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
					strlen( writtenSentenceString ) > 0 )
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
							if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isCurrentSpecificationSelfGenerated ? ( isWritingSelfGeneratedConclusions ? INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? INTERFACE_LISTING_MY_ASSUMPTIONS : INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isCurrentSpecificationSelfGenerated ? INTERFACE_LISTING_MY_INFORMATION : INTERFACE_LISTING_YOUR_INFORMATION ) : INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write my conclusions header" );

							hasHeaderBeenWritten = false;
							}

						if( isWritingJustification )
							{
							if( writeJustificationSpecification( currentSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a justification line" );
							}
						else
							{
							if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a sentence" );
							}
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSpecificationJustifications( bool isOnlyWritingPrimarySpecification, bool isWritingPrimarySpecification, bool isWritingAdditionalDefinitionSpecification, JustificationItem *writeJustificationItem, JustificationItem *startJustificationItem, SpecificationItem *selfGeneratedSpecificationItem )
		{
		bool isPossessiveSelfGeneratedSpecificationItem;
		bool isWritingCurrentSpecificationWordOnly;
		JustificationItem *currentJustificationItem = startJustificationItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *additionalProperNounSpecificationItem;
		SpecificationItem *primarySpecificationItem = NULL;
		SpecificationItem *secondarySpecificationItem;
		WordItem *feminineOrMasculineProperNounEndingWordItem;
		WordItem *generalizationWordItem;
		WordItem *primaryGeneralizationWordItem = NULL;
		WordItem *lastWrittenFeminineOrMasculineProperNounEndingWordItem = NULL;
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationJustifications";

		if( writeJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given write justification item is undefined" );

		if( currentJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start justification item is undefined" );

		if( isWritingPrimarySpecification &&
		( primarySpecificationItem = writeJustificationItem->primarySpecificationItem() ) != NULL )
			{
			if( ( primaryGeneralizationWordItem = primarySpecificationItem->generalizationWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given primary specification item is undefined" );

			if( primarySpecificationItem->isHiddenSpanishSpecification() )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nThe primary specification item is a hidden specification:\n\tSpecificationItem: " );
				strcat( errorString, primarySpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( primaryGeneralizationWordItem->writeJustificationSpecificationInWord( ( !primarySpecificationItem->isExclusiveSpecification() && !primarySpecificationItem->isPossessive() ), primarySpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the definition justification specification" );
				}
			}

		if( !isOnlyWritingPrimarySpecification &&
		isWritingAdditionalDefinitionSpecification &&
		( additionalDefinitionSpecificationItem = writeJustificationItem->additionalDefinitionSpecificationItem() ) != NULL )
			{
			if( ( generalizationWordItem = additionalDefinitionSpecificationItem->generalizationWordItem() ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given additional definition specification item is undefined" );

			if( additionalDefinitionSpecificationItem->isHiddenSpanishSpecification() )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nThe additional definition specification item is a hidden specification:\n\tSpecificationItem: " );
				strcat( errorString, additionalDefinitionSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			else
				{
				if( generalizationWordItem->writeJustificationSpecificationInWord( false, additionalDefinitionSpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the another definition justification specification" );
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
					// The result of this call is stored in a shared string. Hence, one call at the time.
					strcpy( errorString, "\nThe secondary specification item is a hidden specification:\n\tSpecificationItem: " );
					strcat( errorString, secondarySpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ";\n\tJustificationItem: " );
					strcat( errorString, writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ".\n" );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					isWritingCurrentSpecificationWordOnly = ( secondarySpecificationItem->isPartOf() ||

															( !secondarySpecificationItem->isExclusiveSpecification() &&
															secondarySpecificationItem->isGeneralizationProperNoun() &&
															!secondarySpecificationItem->isPossessive() ) );

					if( generalizationWordItem->writeJustificationSpecificationInWord( isWritingCurrentSpecificationWordOnly, secondarySpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the secondary justification specification" );
					}
				}

			if( ( additionalProperNounSpecificationItem = writeJustificationItem->additionalProperNounSpecificationItem() ) != NULL )
				{
				if( ( generalizationWordItem = additionalProperNounSpecificationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given additional proper noun specification item is undefined" );

				if( additionalProperNounSpecificationItem->isHiddenSpanishSpecification() )
					{
					// The result of this call is stored in a shared string. Hence, one call at the time.
					strcpy( errorString, "\nThe additional proper noun specification item is a hidden specification:\n\tSpecificationItem: " );
					strcat( errorString, additionalProperNounSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ";\n\tJustificationItem: " );
					strcat( errorString, writeJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ".\n" );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				else
					{
					if( generalizationWordItem->writeJustificationSpecificationInWord( false, additionalProperNounSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the additional proper noun justification specification" );
					}
				}

			// Now display list of feminine / masculine assumptions
			if( selfGeneratedSpecificationItem != NULL &&
			writeJustificationItem->hasFeminineOrMasculineProperNounEnding() )
				{
				isPossessiveSelfGeneratedSpecificationItem = selfGeneratedSpecificationItem->isPossessive();

				do	{
					if( currentJustificationItem->hasFeminineOrMasculineProperNounEnding() )
						{
						feminineOrMasculineProperNounEndingWordItem = ( isPossessiveSelfGeneratedSpecificationItem ? selfGeneratedSpecificationItem->singleRelationWordItem() :
																													currentJustificationItem->generalizationWordItem() );

						if( feminineOrMasculineProperNounEndingWordItem == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the feminine or masculine proper noun ending word" );

						if( !feminineOrMasculineProperNounEndingWordItem->hasFeminineOrMasculineProperNounEnding() )
							return adminItem_->startError( functionNameString, moduleNameString_, "Word \"", feminineOrMasculineProperNounEndingWordItem->anyWordTypeString(), "\" has no feminine or masculine proper noun ending" );

						if( feminineOrMasculineProperNounEndingWordItem != lastWrittenFeminineOrMasculineProperNounEndingWordItem )
							{
							if( inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_WRITE_INDENTED, INTERFACE_JUSTIFICATION_FEMININE_OR_MASCULINE_PROPER_NOUN_ENDING, feminineOrMasculineProperNounEndingWordItem->anyWordTypeString(), ( feminineOrMasculineProperNounEndingWordItem->hasFeminineProperNounEnding() ? INTERFACE_JUSTIFICATION_FEMININE_PROPER_NOUN_ENDING : INTERFACE_JUSTIFICATION_MASCULINE_PROPER_NOUN_ENDING ) ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the justification sentence start string" );

							lastWrittenFeminineOrMasculineProperNounEndingWordItem = feminineOrMasculineProperNounEndingWordItem;
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
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeUserQuestions";

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
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeUserSpecifications";

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
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedSpecificationInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			// Do for all generalization specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				// Respond with active specifications
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( false, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active specifications" );

				// Respond with active assignments
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write active assignments" );

				// Respond with inactive assignments
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, true, false, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write inactive assignments" );

				// Respond with archived assignments
				if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, true, writeWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write archived assignments" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSelectedRelationInfo( WordItem *writeWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelectedRelationInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstRelationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization relation words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

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

	AdminWrite( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

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
		isFirstUserSpecifications_ = true;
		}

	signed char answerQuestions()
		{
		WordItem *currentTouchedWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "answerQuestions";

		if( globalVariables_->isQuestionAlreadyAnswered &&
		inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'question is already answered' interface notification" );

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			// Do for all words touched during current sentence
			do	{
				if( currentTouchedWordItem->findAnswerToNewUserQuestion() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find an answer to new user questions of word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
			}

		if( !globalVariables_->hasFoundAnswerToQuestion &&
		!globalVariables_->isConflictingQuestion &&
		inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I don't know the answer to this question' interface notification" );

		return RESULT_OK;
		}

	signed char checkIntegrityOfStoredUserSentence( char *readUserSentenceString )
		{
		bool haveAllWordsPassed = true;
		ReadItem *currentReadItem;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkIntegrityOfStoredUserSentence";

		if( ( currentReadItem = adminItem_->firstActiveReadItem() ) != NULL )
			{
			do	{
				if( !currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence )
					haveAllWordsPassed = false;
				}
			while( haveAllWordsPassed &&
			( currentReadItem = currentReadItem->nextReadItem() ) != NULL );
			}

		if( !haveAllWordsPassed )
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

				if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
				strlen( writtenSentenceString ) > 0 &&
				inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, writtenSentenceString, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}

			if( adminItem_->isSystemStartingUp() )
				return adminItem_->startError( functionNameString, moduleNameString_, "An integrity error occurred during startup" );
			}

		return RESULT_OK;
		}

	signed char prepareIntegrityCheckOfStoredUserSentence( bool isChineseCurrentLanguage, SpecificationItem *userSpecificationItem, char *readUserSentenceString )
		{
		bool isEqualReadAndWriteUserString;
		unsigned short lastFoundWordOrderNr = NO_ORDER_NR;
		size_t offset = 0;
		size_t readUserSentenceStringLength = ( readUserSentenceString == NULL ? 0 : strlen( readUserSentenceString ) );
		size_t readWordTypeStringLength;
		size_t wordPosition = 0;
		size_t writtenUserSentenceStringLength;
		char *readWordTypeString;
		char *writtenUserSentenceString;
		ReadItem *currentReadItem;
		ReadItem *startNewSpecificationReadItem = NULL;
		WordItem *generalizationWordItem;
		ReadWordResultType readWordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "prepareIntegrityCheckOfStoredUserSentence";

		if( userSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given user specification item is undefined" );

		if( ( currentReadItem = adminItem_->firstActiveReadItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any read words" );

		if( ( generalizationWordItem = userSpecificationItem->generalizationWordItem() ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The generalization word of the given user specification item is undefined" );

		if( generalizationWordItem->writeSelectedSpecification( true, false, userSpecificationItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the user specification of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		if( ( writtenUserSentenceString = globalVariables_->writtenUserSentenceString ) != NULL &&
		( writtenUserSentenceStringLength = strlen( writtenUserSentenceString ) ) > 0 )
			{
			isEqualReadAndWriteUserString = ( readUserSentenceStringLength > 0 &&
											readUserSentenceString != NULL &&

											( readUserSentenceString[0] == writtenUserSentenceString[0] ||
											(char)tolower( readUserSentenceString[0] ) == writtenUserSentenceString[0] ) &&

											( readUserSentenceStringLength == 1 ||
											// The read user string and written user string are equal
											strcmp( &readUserSentenceString[1], &writtenUserSentenceString[1] ) == 0 ) );

			do	{
				do	{
					if( isEqualReadAndWriteUserString )
						currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;
					else
						{
						if( ( readWordTypeString = currentReadItem->readWordTypeString() ) != NULL )
							{
							readWordTypeStringLength = strlen( readWordTypeString );

							if( ( readWordResult = adminItem_->readWordFromString( false, false, false, readWordTypeStringLength, &writtenUserSentenceString[wordPosition] ) ).result != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a word from the written string" );

							offset = readWordResult.offset;

							if( !currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence )
								{
								if( readWordTypeStringLength == readWordResult.wordLength &&
								strncmp( &writtenUserSentenceString[wordPosition], readWordTypeString, readWordResult.wordLength ) == 0 )
									{
									hasFoundAnyWordPassingIntegrityCheckOfStoredUserSentence_ = true;
									currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;

									if( lastFoundWordOrderNr == NO_ORDER_NR ||
									lastFoundWordOrderNr + 1 == currentReadItem->wordOrderNr() )
										{
										wordPosition += offset;
										lastFoundWordOrderNr = currentReadItem->wordOrderNr();
										startNewSpecificationReadItem = currentReadItem->nextReadItem();
										}
									}
								else
									{
									if( currentReadItem->isSkippingIntegrityCheckWords() ||

									// Typical for Chinese
									( isChineseCurrentLanguage &&
									currentReadItem->isSkippingChineseIntegrityCheckWords() ) )
										// Skip until implemented
										currentReadItem->hasWordPassedIntegrityCheckOfStoredUserSentence = true;

									if( startNewSpecificationReadItem == NULL )
										startNewSpecificationReadItem = currentReadItem;
									}
								}
							}
						}
					}
				while( ( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

				wordPosition += offset;
				currentReadItem = startNewSpecificationReadItem;
				}
			while( currentReadItem != NULL &&
			wordPosition < writtenUserSentenceStringLength );
			}

		return RESULT_OK;
		}

	signed char writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions )
		{
		WordItem *currentTouchedWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelfGeneratedInfo";

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			// Do for all words touched during current sentence
			do	{
				if( writeInfoAboutWord( true, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, false, false, currentTouchedWordItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated info of word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char writeSelfGeneratedInfo( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSelfGeneratedInfo";

		// Self-generated conclusions
		if( isWritingSelfGeneratedConclusions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, true, false, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated conclusions" );

		// Self-generated assumptions
		if( isWritingSelfGeneratedAssumptions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, true, false, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated assumptions" );

		// Self-generated questions
		if( isWritingSelfGeneratedQuestions &&
		writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, false, true, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write self-generated questions" );

		return RESULT_OK;
		}

	signed char writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeInfoAboutWord";

		if( isWritingUserSpecifications &&
		writeUserSpecifications( isWritingCurrentSentenceOnly, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user specifications" );

		if( isWritingUserQuestions &&
		writeUserQuestions( isWritingCurrentSentenceOnly, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write user questions" );

		if( ( isWritingSelfGeneratedConclusions ||
		isWritingSelfGeneratedAssumptions ||
		isWritingSelfGeneratedQuestions ) &&

		writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the self-generated info" );

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
