/*	Class:			WordWrite
 *	Supports class:	WordItem
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

#include "SpecificationItem.cpp"

class WordWrite
	{
	friend class WordItem;

	// Private constructed variables

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];

	protected:
	// Constructor

	WordWrite( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordWrite" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType writeJustificationSpecification( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecification";

		if( justificationSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given justification specification item is undefined" );

		if( justificationSpecificationItem->isReplacedOrDeletedItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given justification specification item is a replaced or deleted item" );

		if( writeSelectedSpecification( false, false, true, true, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, justificationSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification with justification" );

		if( strlen( commonVariables_->writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the selected specification with justification" );

		if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE_INDENTED, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a justification sentence" );

		return RESULT_OK;
		}

	ResultType writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		SpecificationItem *currentSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecificationInfo";

		if( writeWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != NULL )
			{
			do	{
				if( currentSpecificationItem->specificationWordItem() == writeWordItem &&
				!currentSpecificationItem->isHiddenSpanishSpecification() )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification" );

					if( strlen( commonVariables_->writtenSentenceString ) > 0 )
						{
						if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_WRITE, ( isQuestion ? INTERFACE_LISTING_SPECIFICATION_QUESTIONS : INTERFACE_LISTING_SPECIFICATIONS ) ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a related header" );

						if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		SpecificationItem *currentSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedRelationInfo";

		if( writeWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != NULL )
			{
			do	{
				if( writeWordItem->hasContextInWord( currentSpecificationItem->relationContextNr(), currentSpecificationItem->specificationWordItem() ) &&
				myWordItem_->firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, currentSpecificationItem->isPossessive(), currentSpecificationItem->questionParameter(), writeWordItem ) != NULL &&
				!currentSpecificationItem->isHiddenSpanishSpecification() )
					{
					if( writeSelectedSpecification( false, false, false, false, false, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification" );

					if( strlen( commonVariables_->writtenSentenceString ) > 0 )
						{
						if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( isQuestion ? INTERFACE_LISTING_RELATED_QUESTIONS : INTERFACE_LISTING_RELATED_INFORMATION ) ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a related header" );

						if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType writeSelectedSpecification( bool isAdjustedAssumption, bool isCheckingUserSentenceForIntegrity, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		bool hasRelationContext;
		bool hasSpecificationCompoundCollection;
		bool isAssignment;
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPossessive;
		bool isOlderItem;
		bool isQuestion;
		bool isSpecificationGeneralization;
		bool isUserSpecification;
		bool isWritingSentenceWithOneSpecificationOnly;
		bool hasAssignment = false;
		bool isCombinedSpecification = false;
		bool isCombinedOldAndNewSpecification = false;
		bool isFirstRelatedSpecification = false;
		bool isLastCompoundSpecification = false;
		bool isLastRelatedSpecification = false;
		bool isSelfGeneratedDefinitionConclusion = false;
		unsigned int relationContextNr;
		size_t writtenSentenceStringLength;
		SpecificationItem *firstAssignmentItem;
		SpecificationItem *relatedSpecificationItem = NULL;
		WordItem *specificationWordItem;
		WordItem *currentLanguageWordItem = commonVariables_->currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecification";

		strcpy( commonVariables_->learnedFromUserString, EMPTY_STRING );
		strcpy( commonVariables_->writtenSentenceString, EMPTY_STRING );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( writeSpecificationItem->isHiddenSpanishSpecification() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is hidden" );

		hasRelationContext = writeSpecificationItem->hasRelationContext();
		hasSpecificationCompoundCollection = writeSpecificationItem->hasSpecificationCompoundCollection();
		isAssignment = writeSpecificationItem->isAssignment();
		isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();
		isNegative = writeSpecificationItem->isNegative();
		isPossessive = writeSpecificationItem->isPossessive();
		isOlderItem = writeSpecificationItem->isOlderItem();
		isQuestion = writeSpecificationItem->isQuestion();
		isSpecificationGeneralization = writeSpecificationItem->isSpecificationGeneralization();
		isUserSpecification = writeSpecificationItem->isUserSpecification();

		relationContextNr = writeSpecificationItem->relationContextNr();

		if( ( specificationWordItem = writeSpecificationItem->specificationWordItem() ) != NULL )
			{
			if( !isWritingCurrentSpecificationWordOnly &&
			answerParameter == NO_ANSWER_PARAMETER )
				{
				if( ( specificationResult = myWordItem_->findRelatedSpecification( true, writeSpecificationItem ) ).result != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

				isFirstRelatedSpecification = specificationResult.isFirstRelatedSpecification;
				isLastRelatedSpecification = specificationResult.isLastRelatedSpecification;
				relatedSpecificationItem = specificationResult.relatedSpecificationItem;

				if( relatedSpecificationItem != NULL &&
				relatedSpecificationItem->isOlderItem() )
					{
					if( !hasAssignment &&
					!hasSpecificationCompoundCollection &&
					isWritingCurrentSentenceOnly )
						isCombinedSpecification = true;

					if( !isOlderItem )
						isCombinedOldAndNewSpecification = true;
					}
				}

			if( !isAssignment &&
			( firstAssignmentItem = myWordItem_->firstAssignmentItem( true, true, true, isPossessive, writeSpecificationItem->questionParameter(), relationContextNr, specificationWordItem ) ) != NULL )
				{
				hasAssignment = true;

				if( isExclusiveSpecification &&
				!firstAssignmentItem->hasRelationContext() &&
				firstAssignmentItem->isArchivedAssignment() )
					isForcingResponseNotBeingAssignment = true;
				}

			if( hasSpecificationCompoundCollection &&
			!isFirstRelatedSpecification &&
			isLastRelatedSpecification )
				isLastCompoundSpecification = true;

			if( !isSpecificationGeneralization &&
			writeSpecificationItem->isGeneralizationNoun() &&
			writeSpecificationItem->isSelfGeneratedConclusion() )
				isSelfGeneratedDefinitionConclusion = true;
			}

		if( isCombinedSpecification ||

		// Self-generated
		( isSelfGeneratedDefinitionConclusion &&

		( !isExclusiveSpecification ||
		isLastCompoundSpecification ||
		isForcingResponseNotBeingFirstSpecification ||

		( !hasSpecificationCompoundCollection &&
		isFirstRelatedSpecification ) ) ) ||

		// User specification
		( !isSelfGeneratedDefinitionConclusion &&

		( !hasAssignment ||
		isForcingResponseNotBeingAssignment ||
		writeSpecificationItem->isConditional() ||
		writeSpecificationItem->isCorrectedAssumption() ) &&

		( isCombinedOldAndNewSpecification ||
		isFirstRelatedSpecification ||
		isForcingResponseNotBeingFirstSpecification ||
		relatedSpecificationItem == NULL ||

		( !isAdjustedAssumption &&
		isNegative &&
		!isQuestion &&
		writeSpecificationItem->assumptionLevel() != relatedSpecificationItem->assumptionLevel() ) ) ) )
			{
			isWritingSentenceWithOneSpecificationOnly = ( isWritingCurrentSpecificationWordOnly &&
														strlen( writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString ) > 0 );

			// To increase performance, use the last written sentence if still up-to-date
			if( answerParameter == NO_ANSWER_PARAMETER &&
			strlen( writeSpecificationItem->lastWrittenSentenceString ) > 0 &&

			( isWritingSentenceWithOneSpecificationOnly ||

			// Question
			( isQuestion &&

			( !isAdjustedAssumption ||
			writeSpecificationItem->hasCurrentActiveSentenceNr() ) ) ||

			// No relation context
			( !hasRelationContext &&

			( ( isExclusiveSpecification &&
			!isQuestion ) ||

			( isUserSpecification &&

			( !isWritingCurrentSpecificationWordOnly ||

			( !hasSpecificationCompoundCollection &&
			writeSpecificationItem->nInvolvedSpecificationWords( false ) == 1 ) ) ) ||

			( ( !isQuestion ||
			!isAdjustedAssumption ) &&

			myWordItem_->isUserGeneralizationWord ) ) ) ||

			// Relation context
			( hasRelationContext &&
			!myWordItem_->hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) ) ) )
				{
				// Use the last written sentence
				if( isCheckingUserSentenceForIntegrity )
					strcpy( commonVariables_->writtenUserSentenceString, writeSpecificationItem->lastWrittenSentenceString );
				else
					strcpy( commonVariables_->writtenSentenceString, ( isWritingSentenceWithOneSpecificationOnly ? writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString : writeSpecificationItem->lastWrittenSentenceString ) );
				}
			else
				{
				// To increase performance, use the last written sentence of a related specification
				if( isOlderItem &&

				( !isQuestion ||
				!hasSpecificationCompoundCollection ) &&

				relatedSpecificationItem != NULL &&
				relatedSpecificationItem->lastWrittenSentenceString != NULL &&
				strlen( relatedSpecificationItem->lastWrittenSentenceString ) > 0 )
					// Use the last written sentence of a related specification
					strcpy( commonVariables_->writtenSentenceString, relatedSpecificationItem->lastWrittenSentenceString );
				else
					{
					if( currentLanguageWordItem == NULL )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The current language word item is undefined" );

					if( myWordItem_->selectGrammarToWriteSentence( isCheckingUserSentenceForIntegrity, isWritingCurrentSpecificationWordOnly, answerParameter, NO_GRAMMAR_LEVEL, currentLanguageWordItem->firstGrammarItem(), writeSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to select the grammar to write a sentence" );

					// Under certain conditions, the last written sentence will be stored, in order to be re-used if needed
					if( answerParameter == NO_ANSWER_PARAMETER &&
					strlen( commonVariables_->writtenSentenceString ) > 0 &&

					( !isNegative ||
					isUserSpecification ) &&

					( isOlderItem ||
					!isPossessive ||
					!writeSpecificationItem->isGeneralizationAssignment() ) )
						{
						if( isWritingCurrentSpecificationWordOnly )
							{
							if( strlen( writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString ) == 0 &&
							writeSpecificationItem->nInvolvedSpecificationWords( false ) > 1 )
								strcpy( writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString, commonVariables_->writtenSentenceString );
							}
						else
							strcpy( writeSpecificationItem->lastWrittenSentenceString, commonVariables_->writtenSentenceString );
						}
					}
				}

			if( currentLanguageWordItem != NULL &&
			writeSpecificationItem->userNr() != commonVariables_->currentUserNr &&
			( writtenSentenceStringLength = strlen( commonVariables_->writtenSentenceString ) ) > 0 )
				{
				strcpy( commonVariables_->learnedFromUserString, currentLanguageWordItem->interfaceString( INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_START ) );
				strcat( commonVariables_->learnedFromUserString, myWordItem_->userNameString( writeSpecificationItem->userNr() ) );
				strcat( commonVariables_->learnedFromUserString, currentLanguageWordItem->interfaceString( INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_END ) );
				}
			}

		return RESULT_OK;
		}

	ResultType writeUpdatedSpecification( bool isAdjustedSpecification, bool isConcludedSpanishAmbiguousAssumption, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, bool isRecalculatingAssumptionsInAllTouchedWords, bool isReplacedBySpecificationWithRelation, SpecificationItem *writeSpecificationItem )
		{
		bool isExclusiveSpecification;
		bool wasHiddenSpanishSpecification;
		unsigned short interfaceParameter;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUpdatedSpecification";

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();
		wasHiddenSpanishSpecification = writeSpecificationItem->wasHiddenSpanishSpecification();

		if( writeSelectedSpecification( true, false, true, isExclusiveSpecification, false, false, NO_ANSWER_PARAMETER, writeSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the given specification item" );

		if( strlen( commonVariables_->writtenSentenceString ) > 0 )
			{
			interfaceParameter = ( isCorrectedAssumptionByKnowledge ? INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE :
									( isCorrectedAssumptionByOppositeQuestion ? INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_QUESTION :
									( isReplacedBySpecificationWithRelation ? ( writeSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_MY_EARLIER_ASSUMPTIONS_THAT_HAVE_RELATION_WORDS_NOW : INTERFACE_LISTING_MY_EARLIER_CONCLUSIONS_THAT_HAVE_RELATION_WORDS_NOW ) :
									( isAdjustedSpecification ? ( writeSpecificationItem->isQuestion() ? INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS :
									( isConcludedSpanishAmbiguousAssumption || writeSpecificationItem->isConcludedAssumption() ?
									( wasHiddenSpanishSpecification ? INTERFACE_LISTING_MY_HIDDEN_ASSUMPTIONS_THAT_ARE_CONCLUDED : INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONCLUDED ) :
									( wasHiddenSpanishSpecification ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_NOT_HIDDEN_ANYMORE : INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_ADJUSTED ) ) ) :
									( writeSpecificationItem->isSelfGenerated() ? INTERFACE_LISTING_MY_QUESTIONS_THAT_ARE_ANSWERED : INTERFACE_LISTING_YOUR_QUESTIONS_THAT_ARE_ANSWERED ) ) ) ) );

			if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a header" );

			if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );

			if( isRecalculatingAssumptionsInAllTouchedWords )
				{
				if( myWordItem_->recalculateAssumptionsInAllTouchedWords() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to recalculate the assumptions in all touched words" );
				}
			}
		else
			{
			if( isExclusiveSpecification )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given specification item" );
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Tremble, O earth, at the presence of the Lord,
 *	at the presence of the God of Jacob.
 *	He turned the rock into a pool of water;
 *	yes, a spring of water flowed from solid rock." (Psalm 114:7-8)
 *************************************************************************/
