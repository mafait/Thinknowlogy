/*
 *	Class:			WordWrite
 *	Supports class:	WordItem
 *	Purpose:		To write selected specifications as sentences
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

#include "Presentation.cpp"
#include "SpecificationItem.cpp"

class WordWrite
	{
	friend class WordItem;

	// Private constructible variables

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];

	protected:
	// Constructor / deconstructor

	WordWrite( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordWrite" );

		if( commonVariables_ != NULL )
			{
		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

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

		if( justificationSpecificationItem != NULL )
			{
			if( !justificationSpecificationItem->isReplacedItem() )
				{
				if( writeSelectedSpecification( false, true, true, false, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, justificationSpecificationItem ) == RESULT_OK )
					{
					if( strlen( commonVariables_->writeSentenceString ) > 0 )
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WRITE, INTERFACE_JUSTIFICATION_SENTENCE_START ) == RESULT_OK )
							{
							if( commonVariables_->presentation->writeDiacriticalText( true, false, PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a justification sentence" );
							}
						else
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the justification sentence start string" );
						}
					else
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the selected specification with justification" );
					}
				else
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification with justification" );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given justification specification item is a replaced item" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given justification specification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem )
		{
		SpecificationItem *currentSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecificationInfo";

		if( writeWordItem != NULL )
			{
			if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != NULL )
				{
				do	{
					if( currentSpecificationItem->specificationWordItem() == writeWordItem )
						{
						if( writeSelectedSpecification( false, false, true, false, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) == RESULT_OK )
							{
							if( strlen( commonVariables_->writeSentenceString ) > 0 )
								{
								if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_WRITE, ( isQuestion ? INTERFACE_LISTING_SPECIFICATION_QUESTIONS : INTERFACE_LISTING_SPECIFICATIONS ) ) == RESULT_OK )
									{
									if( commonVariables_->presentation->writeDiacriticalText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString ) != RESULT_OK )
										return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );
									}
								else
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a related header" );
								}
							}
						else
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write word item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isLanguageWord, bool isQuestion, WordItem *writeWordItem )
		{
		unsigned short currentGrammarLanguageNr = commonVariables_->currentGrammarLanguageNr;
		SpecificationResultType specificationResult;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *currentSpecificationItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "isWritingRelatedInfo";

		if( writeWordItem != NULL )
			{
			if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != NULL )
				{
				do	{
					if( ( isLanguageWord ||
					currentSpecificationItem->grammarLanguageNr() == currentGrammarLanguageNr ) &&

					writeWordItem->hasContextInWord( currentSpecificationItem->relationContextNr(), currentSpecificationItem->specificationWordItem() ) )
						{
						if( ( specificationResult = myWordItem_->findAssignmentByRelationWord( true, isInactiveAssignment, isArchivedAssignment, currentSpecificationItem->isPossessive(), currentSpecificationItem->questionParameter(), writeWordItem ) ).result == RESULT_OK )
							{
							foundAssignmentItem = specificationResult.foundSpecificationItem;

							if( isQuestion ||

							( isAssignment &&
							foundAssignmentItem != NULL ) ||

							( !isAssignment &&
							foundAssignmentItem == NULL ) )
								{
								if( writeSelectedSpecification( false, false, false, false, false, NO_ANSWER_PARAMETER, currentSpecificationItem ) == RESULT_OK )
									{
									if( strlen( commonVariables_->writeSentenceString ) > 0 )
										{
										if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( isQuestion ? INTERFACE_LISTING_RELATED_QUESTIONS : INTERFACE_LISTING_RELATED_INFORMATION ) ) == RESULT_OK )
											{
											if( commonVariables_->presentation->writeDiacriticalText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString ) != RESULT_OK )
												return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );
											}
										else
											return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a related header" );
										}
									}
								else
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected specification" );
								}
							}
						else
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an assignment by relation context" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write word item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelectedSpecification( bool isAdjustedAssumption, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		bool hasRelationContext;
		bool hasSpecificationCompoundCollection;
		bool isAssignment;
		bool isFirstRelatedSpecification;
		bool isPossessive;
		bool isOlderItem;
		bool isSpecificationGeneralization;
		bool isWritingSentenceWithOneSpecificationOnly;
		bool hasAssignment = false;
		bool isCombinedSpecification = false;
		bool isCombinedOldAndNewSpecification = false;
		bool isLastCompoundSpecification = false;
		bool isSelfGeneratedDefinitionConclusion = false;
		unsigned int relationContextNr;
		SpecificationItem *foundRelatedSpecificationItem;
		WordItem *specificationWordItem;
		WordItem *currentGrammarLanguageWordItem = commonVariables_->currentGrammarLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelectedSpecification";

		strcpy( commonVariables_->writeSentenceString, EMPTY_STRING );

		if( writeSpecificationItem != NULL )
			{
			if( ( specificationResult = myWordItem_->findRelatedSpecification( true, writeSpecificationItem ) ).result == RESULT_OK )
				{
				isFirstRelatedSpecification = specificationResult.isFirstRelatedSpecification;
				foundRelatedSpecificationItem = specificationResult.relatedSpecificationItem;

				hasRelationContext = writeSpecificationItem->hasRelationContext();
				hasSpecificationCompoundCollection = writeSpecificationItem->hasSpecificationCompoundCollection();
				isAssignment = writeSpecificationItem->isAssignment();
				isPossessive = writeSpecificationItem->isPossessive();
				isOlderItem = writeSpecificationItem->isOlderItem();
				isSpecificationGeneralization = writeSpecificationItem->isSpecificationGeneralization();

				relationContextNr = writeSpecificationItem->relationContextNr();

				specificationWordItem = writeSpecificationItem->specificationWordItem();

				if( !isAssignment &&
				myWordItem_->firstAssignmentItem( true, true, true, isPossessive, writeSpecificationItem->questionParameter(), relationContextNr, specificationWordItem ) != NULL )
					hasAssignment = true;

				if( foundRelatedSpecificationItem != NULL &&
				foundRelatedSpecificationItem->isOlderItem() )
					{
					if( !hasAssignment &&
					!hasSpecificationCompoundCollection &&
					isWritingCurrentSentenceOnly )
						isCombinedSpecification = true;

					if( !isOlderItem )
						isCombinedOldAndNewSpecification = true;
					}

				if( hasSpecificationCompoundCollection &&
				!isFirstRelatedSpecification &&
				specificationResult.isLastRelatedSpecification )
					isLastCompoundSpecification = true;

				if( !isSpecificationGeneralization &&
				writeSpecificationItem->isGeneralizationNoun() &&
				writeSpecificationItem->isSelfGeneratedConclusion() )
					isSelfGeneratedDefinitionConclusion = true;

				if( isCombinedSpecification ||

				// Self-generated
				( isSelfGeneratedDefinitionConclusion &&

				( isLastCompoundSpecification ||
				isForcingResponseNotBeingFirstSpecification ||

				( !hasSpecificationCompoundCollection &&
				isFirstRelatedSpecification ) ) ) ||

				// User specification
				( !isSelfGeneratedDefinitionConclusion &&

				( !hasAssignment ||
				isAssignment ||
				isForcingResponseNotBeingAssignment ||
				writeSpecificationItem->isConditional() ||
				writeSpecificationItem->isCorrectedAssumption() ) &&

				( isCombinedOldAndNewSpecification ||
				isFirstRelatedSpecification ||
				isForcingResponseNotBeingFirstSpecification ||
				foundRelatedSpecificationItem == NULL ||

				( !isAdjustedAssumption &&
				!writeSpecificationItem->isQuestion() &&
				writeSpecificationItem->assumptionLevel() != foundRelatedSpecificationItem->assumptionLevel() ) ) ) )
					{
					isWritingSentenceWithOneSpecificationOnly = ( isWritingCurrentSpecificationWordOnly &&
																strlen( writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString ) > 0 );

					// To increase performance,
					// use the last written sentence if still up-to-date
					if( answerParameter == NO_ANSWER_PARAMETER &&
					strlen( writeSpecificationItem->lastWrittenSentenceString ) > 0 &&

					( isWritingSentenceWithOneSpecificationOnly ||
					writeSpecificationItem->isUserQuestion() ||

					( myWordItem_->isUserGeneralizationWord &&
					writeSpecificationItem->isSelfGeneratedAssumption() &&
					!myWordItem_->hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) ) ||

					// No relation context
					( !hasRelationContext &&

					( !isWritingCurrentSpecificationWordOnly ||
					writeSpecificationItem->hasExclusiveSpecificationCollection() ) ) ||

					// Relation context
					( hasRelationContext &&

					( ( isOlderItem &&
					!myWordItem_->hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) ) ||

					( !isWritingCurrentSpecificationWordOnly &&
					myWordItem_->nContextWordsInAllWords( relationContextNr, specificationWordItem ) == 1 ) ) ) ) )
						strcpy( commonVariables_->writeSentenceString, ( isWritingSentenceWithOneSpecificationOnly ? writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString : writeSpecificationItem->lastWrittenSentenceString ) );
					else
						{
						if( currentGrammarLanguageWordItem != NULL )
							{
							if( myWordItem_->selectGrammarToWriteSentence( isWritingCurrentSpecificationWordOnly, answerParameter, NO_GRAMMAR_LEVEL, currentGrammarLanguageWordItem->startOfGrammarItem(), writeSpecificationItem ) == RESULT_OK )
								{
								// Under certain conditions, the last written sentence will be stored, to be re-used again if needed
								if( answerParameter == NO_ANSWER_PARAMETER &&
								!writeSpecificationItem->hasHiddenSpecificationWord() &&
								strlen( commonVariables_->writeSentenceString ) > 0 &&

								// Any "part of" specification
								( writeSpecificationItem->isPartOf() ||

								// Any user question
								writeSpecificationItem->isUserQuestion() ||

								// Any non-exclusive non-specification-generalization specification
								( !isSpecificationGeneralization &&
								!writeSpecificationItem->isExclusiveSpecification() &&
								!writeSpecificationItem->isNegative() ) ||

								( !hasRelationContext &&

								// Any older self-generated specification without relation context
								( ( isOlderItem &&
								!writeSpecificationItem->isNegative() &&
								writeSpecificationItem->isSelfGenerated() ) ||

								// Any user specification
								( !isAssignment &&
								writeSpecificationItem->isUserSpecification() ) ) ) ||

								// Any generalization assignment with relation context
								( hasRelationContext &&
								!writeSpecificationItem->isGeneralizationAssignment() ) ) )
									{
									if( !isWritingCurrentSpecificationWordOnly )
										strcpy( writeSpecificationItem->lastWrittenSentenceString, commonVariables_->writeSentenceString );

									if( isWritingCurrentSpecificationWordOnly &&
									strlen( writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString ) == 0 &&
									writeSpecificationItem->nInvolvedSpecificationWords( true, false ) > 1 )
										strcpy( writeSpecificationItem->lastWrittenSentenceWithOneSpecificationOnlyString, commonVariables_->writeSentenceString );
									}
								}
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to select the grammar to write a sentence" );
							}
						else
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The current language word item is undefined" );
						}
					}
				}
			else
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeUpdatedSpecification( bool isAdjustedSpecification, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, SpecificationItem *writeSpecificationItem )
		{
		bool isExclusiveSpecification;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUpdatedSpecification";

		if( writeSpecificationItem != NULL )
			{
			isExclusiveSpecification = writeSpecificationItem->isExclusiveSpecification();

			if( writeSelectedSpecification( true, true, isExclusiveSpecification, false, false, NO_ANSWER_PARAMETER, writeSpecificationItem ) == RESULT_OK )
				{
				if( strlen( commonVariables_->writeSentenceString ) > 0 )
					{
					if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( isCorrectedAssumptionByKnowledge ? INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE : ( isCorrectedAssumptionByOppositeQuestion ? INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_QUESTION : ( isAdjustedSpecification ? ( writeSpecificationItem->isQuestion() ? INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS : ( writeSpecificationItem->isConcludedAssumption() ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONCLUDED : INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_ADJUSTED ) ) : ( writeSpecificationItem->isSelfGenerated() ? INTERFACE_LISTING_MY_QUESTIONS_THAT_ARE_ANSWERED : INTERFACE_LISTING_YOUR_QUESTIONS_THAT_ARE_ANSWERED ) ) ) ) ) == RESULT_OK )
						{
						if( commonVariables_->presentation->writeDiacriticalText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );
						}
					else
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a header" );
					}
				else
					{
					if( isExclusiveSpecification )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given specification item" );
					}
				}
			else
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the given specification item" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Tremble, O earth, at the presence of the Lord,
 *	at the presence of the God of Jacob.
 *	He turned the rock into a pool of water;
 *	yes, a spring of water flowed from solid rock." (Psalm 114:7-8)
 *************************************************************************/
