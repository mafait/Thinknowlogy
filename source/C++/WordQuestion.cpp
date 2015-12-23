/*
 *	Class:			WordQuestion
 *	Supports class:	WordItem
 *	Purpose:		To answer questions about this word
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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

class WordQuestion
	{
	friend class WordItem;

	// Private constructible variables

	bool hasFoundAnswerToQuestion_;
	bool hasFoundDeeperPositiveAnswer_;
	bool hasFoundNothingInThisWord_;
	bool hasFoundSpecificationGeneralizationAnswer_;
	bool isNegativeAnswer_;

	SpecificationItem *uncertainAboutAnswerRelationSpecificationItem_;

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType findAnswerToQuestion( SpecificationItem *questionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		bool hasRelationContext;
		bool isAssignment;
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPossessive;
		bool isNegativeAnswer = false;
		bool isPositiveAnswer = false;
		bool isUncertainAboutRelation = false;
		unsigned short questionAssumptionLevel;
		unsigned short answerAssumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *answerSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswerToQuestion";

		if( questionSpecificationItem != NULL )
			{
			hasRelationContext = questionSpecificationItem->hasRelationContext();
			isAssignment = questionSpecificationItem->isAssignment();
			isExclusiveSpecification = questionSpecificationItem->isExclusiveSpecification();
			isNegative = questionSpecificationItem->isNegative();
			isPossessive = questionSpecificationItem->isPossessive();

			specificationCollectionNr = questionSpecificationItem->specificationCollectionNr();
			generalizationContextNr = questionSpecificationItem->generalizationContextNr();
			specificationContextNr = questionSpecificationItem->specificationContextNr();
			relationContextNr = questionSpecificationItem->relationContextNr();
			specificationWordItem = questionSpecificationItem->specificationWordItem();

			// Find correct answer
			if( ( answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == NULL )
				{
				// Find answer with different relation context
				if( ( answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, specificationWordItem ) ) == NULL )
					{
					// Find negative answer
					if( ( answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, !isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == NULL )
						{
						// Find opposite possessive answer
						if( ( answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, isNegative, !isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
							isNegativeAnswer = true;
						}
					else
						isNegativeAnswer = true;
					}
				else
					{
					// Find out if the relation context of the question is a subset of the relation context of the answer
					if( hasRelationContext &&
					!answerSpecificationItem->hasRelationContext() )
						isUncertainAboutRelation = true;
					else
						isNegativeAnswer = true;
					}
				}
			else
				{
				if( ( !isExclusiveSpecification &&
				answerSpecificationItem->isExclusiveSpecification() ) ||

				answerSpecificationItem->specificationWordItem() != specificationWordItem )
					// Has different specification word
					isNegativeAnswer = true;
				else
					{
					// Only confirm the answer with 'yes' if the answer is at least as reliable as the question
					if( !isExclusiveSpecification &&
					// If a specification question is mapped to an assignment question, the answer is negative
					answerSpecificationItem->isRelatedQuestion( isExclusiveSpecification, specificationCollectionNr, relationContextNr ) )
						{
						if( ( specificationResult = questionSpecificationItem->getAssumptionLevel() ).result == RESULT_OK )
							{
							questionAssumptionLevel = specificationResult.assumptionLevel;

							if( ( specificationResult = answerSpecificationItem->getAssumptionLevel() ).result == RESULT_OK )
								{
								answerAssumptionLevel = specificationResult.assumptionLevel;

								if( answerAssumptionLevel <= questionAssumptionLevel )
									isPositiveAnswer = true;
								}
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to get the answer assumption level" );
							}
						else
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to get the question assumption level" );
						}
					}

				if( ( isPositiveAnswer ||
				isNegativeAnswer ) &&

				!hasRelationContext &&
				answerSpecificationItem->isAssignment() &&
				answerSpecificationItem->hasRelationContext() )
					{
					// Ambiguity: Missing relation context
					if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_QUESTION_MISSING_RELATION ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'ambiguous question missing relation' interface notification" );
					}
				}

			if( answerSpecificationItem != NULL &&
			// Ignore suggestive assumptions
			answerSpecificationItem->isOlderItem() &&
			!answerSpecificationItem->isHiddenSpecification() )
				{
				if( writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		return RESULT_OK;
		}

	ResultType findAlternativeAnswerToQuestion( SpecificationItem *questionSpecificationItem )
		{
		bool isAssignment;
		bool isNegative;
		bool isPossessive;
		unsigned int generalizationCollectionNr;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAlternativeAnswerToQuestion";

		hasFoundSpecificationGeneralizationAnswer_ = false;

		if( questionSpecificationItem != NULL )
			{
			if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( questionSpecificationItem->isAssignment(), questionSpecificationItem->isInactiveAssignment(), questionSpecificationItem->isArchivedAssignment(), false ) ) != NULL )
				{
				isAssignment = ( questionSpecificationItem->isAssignment() ||
								questionSpecificationItem->hasRelationContext() );

				isNegative = questionSpecificationItem->isNegative();
				isPossessive = questionSpecificationItem->isPossessive();
				generalizationCollectionNr = questionSpecificationItem->generalizationCollectionNr();
				specificationCollectionNr = questionSpecificationItem->specificationCollectionNr();
				generalizationContextNr = questionSpecificationItem->generalizationContextNr();
				specificationContextNr = questionSpecificationItem->specificationContextNr();
				relationContextNr = questionSpecificationItem->relationContextNr();
				specificationWordItem = questionSpecificationItem->specificationWordItem();

				do	{
					if( currentSpecificationItem->isOlderItem() )
						{
						if( currentSpecificationItem->isSpecificationGeneralization() )
							{
							if( writeAnswerToQuestion( true, false, false, currentSpecificationItem ) == RESULT_OK )
								hasFoundSpecificationGeneralizationAnswer_ = true;
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
							}
						else
							{
							if( relationContextNr == NO_CONTEXT_NR ||
							uncertainAboutAnswerRelationSpecificationItem_ == NULL )
								{
								if( hasFoundSpecificationGeneralizationAnswer_ ||
								currentSpecificationItem->isRelatedSpecification( isNegative, isPossessive, generalizationCollectionNr, specificationCollectionNr, relationContextNr ) )
									{
									if( writeAnswerToQuestion( true, false, false, currentSpecificationItem ) != RESULT_OK )
										return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
									}
								else
									{
									if( questionSpecificationItem->isSpecificationGeneralization() &&
									( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL )
										{
										if( currentSpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( false, isAssignment, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != NULL )
											{
											hasFoundDeeperPositiveAnswer_ = true;
											hasFoundSpecificationGeneralizationAnswer_ = true;
											}
										}
									}
								}
							}
						}
					}
				while( !hasFoundSpecificationGeneralizationAnswer_ &&
				( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		return RESULT_OK;
		}

	ResultType findAlternativeAnswerToQuestionInOtherWords( bool hasFoundSpecificationGeneralizationAnswer, SpecificationItem *questionSpecificationItem )
		{
		bool isAssignment;
		bool isArchivedAssignment;
		bool isNegative;
		bool isPossessive;
		unsigned short generalizationWordTypeNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentWordItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAlternativeAnswerToQuestionInOtherWords";

		if( questionSpecificationItem != NULL )
			{
			isAssignment = questionSpecificationItem->isAssignment();
			isArchivedAssignment = questionSpecificationItem->isArchivedAssignment();
			isNegative = questionSpecificationItem->isNegative();
			isPossessive = questionSpecificationItem->isPossessive();
			generalizationWordTypeNr = questionSpecificationItem->generalizationWordTypeNr();
			generalizationContextNr = questionSpecificationItem->generalizationContextNr();
			specificationContextNr = questionSpecificationItem->specificationContextNr();
			relationContextNr = questionSpecificationItem->relationContextNr();
			specificationWordItem = questionSpecificationItem->specificationWordItem();

			if( ( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
				{
				// Do for all words for an alternative answer
				do	{
					if( currentWordItem != myWordItem_ &&
					( foundSpecificationItem = currentWordItem->bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isArchivedAssignment, isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
						{
						if( hasFoundSpecificationGeneralizationAnswer ||

						( foundSpecificationItem->isRelatedSpecification( isNegative, isPossessive, generalizationWordTypeNr ) &&
						!foundSpecificationItem->isHiddenSpecification() ) )
							{
							if( currentWordItem->writeAnswerToQuestion( !hasFoundDeeperPositiveAnswer_, hasFoundDeeperPositiveAnswer_, false, foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
							}
						}
					}
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The first word item is undefined" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		return RESULT_OK;
		}

	ResultType findAnswerToNewUserQuestion( SpecificationItem *questionSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswerToNewUserQuestion";
		hasFoundDeeperPositiveAnswer_ = false;
		isNegativeAnswer_ = false;
		uncertainAboutAnswerRelationSpecificationItem_ = NULL;

		commonVariables_->hasFoundAnswerToQuestion = false;

		if( questionSpecificationItem != NULL )
			{
			if( questionSpecificationItem->isQuestion() )
				{
				if( findAnswerToQuestion( questionSpecificationItem ) == RESULT_OK )
					{
					if( !commonVariables_->hasFoundAnswerToQuestion )
						{
						// Check collections for an alternative current-tense assignment answer to the question
						if( findAlternativeAnswerToQuestion( questionSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an alternative answer to a question" );
						}

					if( isNegativeAnswer_ ||
					hasFoundSpecificationGeneralizationAnswer_ ||
					uncertainAboutAnswerRelationSpecificationItem_ != NULL ||

					( !commonVariables_->hasFoundAnswerToQuestion &&
					questionSpecificationItem->isAssignment() ) )
						{
						// Check other words for an alternative answer to the question
						if( findAlternativeAnswerToQuestionInOtherWords( hasFoundSpecificationGeneralizationAnswer_, questionSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an alternative answer to a question" );
						}

					if( commonVariables_->hasFoundAnswerToQuestion &&
					uncertainAboutAnswerRelationSpecificationItem_ == NULL )
						{
						if( markQuestionAsAnswered( false, questionSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a question specification as been answered" );
						}
					else
						{
						if( myWordItem_->firstNonQuestionSpecificationItem() == NULL )
							{
							// I don't know anything at all about this word
							if( commonVariables_->presentation->writeInterfaceText( hasFoundNothingInThisWord_, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_START, ( questionSpecificationItem->isSpecificationGeneralization() ? questionSpecificationItem->activeSpecificationWordTypeString() : questionSpecificationItem->activeGeneralizationWordTypeString() ), INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_END ) == RESULT_OK )
								{
								hasFoundNothingInThisWord_ = true;
								commonVariables_->hasFoundAnswerToQuestion = true;
								}
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'I don't know anything about word' interface notification" );
							}
						else
							{
							// A specification exists, but it isn't a question
							if( uncertainAboutAnswerRelationSpecificationItem_ != NULL )
								{
								if( myWordItem_->writeSelectedSpecification( true, true, false, false, NO_ANSWER_PARAMETER, uncertainAboutAnswerRelationSpecificationItem_ ) == RESULT_OK )
									{
									if( strlen( commonVariables_->writeSentenceString ) > 0 )
										{
										if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_LISTING_I_ONLY_KNOW ) == RESULT_OK )
											{
											if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
												return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
											}
										else
											return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a header" );
										}
									else
										return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the selected answer to a question" );
									}
								else
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected answer to a question" );
								}
							}
						}
					}
				else
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an answer to a question" );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item isn't a question" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		return RESULT_OK;
		}

	ResultType findAnswersToQuestions( bool isAssignment, bool isArchivedAssignment, unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
		{
		bool isAnswerPossessive;
		bool isAnswerNegative;
		bool isShowingAnsweredQuestion;
		unsigned int answerGeneralizationCollectionNr;
		unsigned int answerSpecificationCollectionNr;
		SpecificationItem *currentQuestionSpecificationItem;
		WordItem *answerSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswersToQuestions";

		if( answerSpecificationItem != NULL )
			{
			if( !answerSpecificationItem->isQuestion() )
				{
				if( ( currentQuestionSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAssignment, false, isArchivedAssignment, true ) ) != NULL )
					{
					isAnswerPossessive = answerSpecificationItem->isPossessive();
					isAnswerNegative = answerSpecificationItem->isNegative();

					answerGeneralizationCollectionNr = answerSpecificationItem->generalizationCollectionNr();
					answerSpecificationCollectionNr = answerSpecificationItem->specificationCollectionNr();

					answerSpecificationWordItem = answerSpecificationItem->specificationWordItem();

					do	{
						// To avoid answering questions that are created during this sentence
						if( currentQuestionSpecificationItem->isOlderItem() &&
						currentQuestionSpecificationItem->isRelatedSpecification( isAnswerNegative, isAnswerPossessive, answerGeneralizationCollectionNr, answerSpecificationCollectionNr, compoundSpecificationCollectionNr, answerSpecificationWordItem ) )
							{
							isShowingAnsweredQuestion = ( isAssignment ? true : ( myWordItem_->firstAnsweredQuestionAssignmentItem( isArchivedAssignment, currentQuestionSpecificationItem->isNegative(), currentQuestionSpecificationItem->isPossessive(), currentQuestionSpecificationItem->questionParameter(), currentQuestionSpecificationItem->relationContextNr(), currentQuestionSpecificationItem->specificationWordItem() ) == NULL ) );

							if( markQuestionAsAnswered( isShowingAnsweredQuestion, currentQuestionSpecificationItem ) == RESULT_OK )
								currentQuestionSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAssignment, false, isArchivedAssignment, true );
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a related question as been answered" );
							}
						else
							currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem();
						}
					while( currentQuestionSpecificationItem != NULL );
					}
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is a question" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is undefined" );

		return RESULT_OK;
		}

	ResultType markQuestionAsAnswered( SpecificationItem *questionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *answeredQuestionSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "markQuestionAsAnswered";

		if( questionSpecificationItem != NULL )
			{
			if( !questionSpecificationItem->isAnsweredQuestion() )
				{
				if( questionSpecificationItem->isAssignment() )
					{
					if( ( specificationResult = myWordItem_->copySpecificationItem( questionSpecificationItem->isInactiveAssignment(), questionSpecificationItem->isArchivedAssignment(), true, questionSpecificationItem->isExclusiveSpecification(), questionSpecificationItem->assignmentLevel(), questionSpecificationItem->generalizationCollectionNr(), questionSpecificationItem->specificationCollectionNr(), questionSpecificationItem->relationContextNr(), questionSpecificationItem->firstJustificationItem(), questionSpecificationItem ) ).result == RESULT_OK )
						{
						if( ( answeredQuestionSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
							{
							if( myWordItem_->replaceOrDeleteSpecification( questionSpecificationItem, answeredQuestionSpecificationItem ) == RESULT_OK )
								// Get the specification of this assignment question
								questionSpecificationItem = myWordItem_->firstSpecificationItem( questionSpecificationItem->isPossessive(), questionSpecificationItem->isSpecificationGeneralization(), questionSpecificationItem->questionParameter(), questionSpecificationItem->specificationWordItem() );
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace an answered question assignment" );
							}
						else
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't create an answered question assignment item" );
						}
					else
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to copy a question assignment to an answered question assignment" );
					}

				if( questionSpecificationItem != NULL )
					{
					if( ( specificationResult = myWordItem_->copySpecificationItem( questionSpecificationItem->isInactiveAssignment(), questionSpecificationItem->isArchivedAssignment(), true, questionSpecificationItem->isExclusiveSpecification(), questionSpecificationItem->assignmentLevel(), questionSpecificationItem->generalizationCollectionNr(), questionSpecificationItem->specificationCollectionNr(), questionSpecificationItem->relationContextNr(), questionSpecificationItem->firstJustificationItem(), questionSpecificationItem ) ).result == RESULT_OK )
						{
						if( ( answeredQuestionSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
							{
							if( myWordItem_->replaceOrDeleteSpecification( questionSpecificationItem, answeredQuestionSpecificationItem ) == RESULT_OK )
								hasFoundAnswerToQuestion_ = true;
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace an answered question specification" );
							}
						else
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't create an answered question specification item" );
						}
					else
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to copy a question specification to an answered question specification" );
					}
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is already answered" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		return RESULT_OK;
		}

	ResultType markQuestionAsAnswered( bool isShowingAnsweredQuestion, SpecificationItem *questionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *relatedSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "markQuestionAsAnswered";

		if( questionSpecificationItem != NULL )
			{
			if( questionSpecificationItem->isQuestion() )
				{
				if( isShowingAnsweredQuestion )
					{
					if( myWordItem_->writeUpdatedSpecification( false, false, false, false, questionSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the answered question" );
					}

				// First mark related question parts as answered
				do	{
					if( ( specificationResult = myWordItem_->findRelatedSpecification( false, questionSpecificationItem ) ).result == RESULT_OK )
						{
						if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL )
							{
							if( markQuestionAsAnswered( relatedSpecificationItem ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a related question as been answered" );
							}
						}
					else
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related answered question part" );
					}
				while( relatedSpecificationItem != NULL );

				// Now mark the given question part as answered
				if( markQuestionAsAnswered( questionSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a question as been answered" );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item isn't a question" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		return RESULT_OK;
		}

	SpecificationItem *firstActiveNewUserQuestion()
		{
		SpecificationItem *firstSpecificationItem;
		SpecificationItem *firstNewUserQuestion = NULL;

		// Try to find an assignment first
		if( ( firstSpecificationItem = myWordItem_->firstQuestionAssignmentItem() ) != NULL )
			firstNewUserQuestion = firstSpecificationItem->firstNewUserQuestion();

		// If not found, try to find a specification
		if( firstNewUserQuestion == NULL &&
		( firstSpecificationItem = myWordItem_->firstActiveQuestionSpecificationItem() ) != NULL )
			return firstSpecificationItem->firstNewUserQuestion();

		return firstNewUserQuestion;
		}


	protected:
	// Constructor / deconstructor

	WordQuestion( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundAnswerToQuestion_ = false;
		hasFoundDeeperPositiveAnswer_ = false;
		hasFoundNothingInThisWord_ = false;
		hasFoundSpecificationGeneralizationAnswer_ = false;
		isNegativeAnswer_ = false;

		uncertainAboutAnswerRelationSpecificationItem_ = NULL;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordQuestion" );

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

	void clearFoundAnswerToQuestion()
		{
		hasFoundAnswerToQuestion_ = false;
		}

	bool hasFoundAnswerToQuestion()
		{
		return hasFoundAnswerToQuestion_;
		}

	ResultType findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswersToQuestions";

		if( findAnswersToQuestions( true, false, compoundSpecificationCollectionNr, answerSpecificationItem ) == RESULT_OK )
			{
			if( findAnswersToQuestions( true, true, compoundSpecificationCollectionNr, answerSpecificationItem ) == RESULT_OK )
				{
				if( findAnswersToQuestions( false, false, compoundSpecificationCollectionNr, answerSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a posssible specification user question and mark it as been answered" );
				}
			else
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a posssible archived assignment user question and mark it as been answered" );
			}
		else
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a posssible active assignment user question and mark it as been answered" );

		return RESULT_OK;
		}

	ResultType findAnswerToNewUserQuestion()
		{
		SpecificationItem *questionSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findAnswerToNewUserQuestion";

		hasFoundNothingInThisWord_ = false;

		if( ( questionSpecificationItem = firstActiveNewUserQuestion() ) != NULL )
			{
			do	{
				if( findAnswerToNewUserQuestion( questionSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an answer to a question" );
				}
			while( ( questionSpecificationItem = ( questionSpecificationItem->isReplacedOrDeletedItem() ? firstActiveNewUserQuestion() : questionSpecificationItem->nextNewUserQuestion() ) ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem )
		{
		unsigned short answerParameter;
		WordItem *currentLanguageWordItem = commonVariables_->currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeAnswerToQuestion";

		if( answerSpecificationItem != NULL )
			{
			if( answerSpecificationItem->isOlderItem() )
				{
				if( !answerSpecificationItem->hasAlreadyBeenWrittenAsAnswer )
					{
					if( !answerSpecificationItem->isHiddenSpecification() )
						{
						if( isPositiveAnswer )
							{
							if( currentLanguageWordItem != NULL )
								{
								if( myWordItem_->selectGrammarToWriteSentence( false, WORD_PARAMETER_INTERJECTION_YES, NO_GRAMMAR_LEVEL, currentLanguageWordItem->startOfGrammarItem(), answerSpecificationItem ) == RESULT_OK )
									answerSpecificationItem->hasAlreadyBeenWrittenAsAnswer = true;
								else
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to select the grammar for an answer" );
								}
							else
								return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The current language word item is undefined" );
							}
						else
							{
							// Neutral or negative answer
							if( isUncertainAboutRelation )
								{
								if( uncertainAboutAnswerRelationSpecificationItem_ == NULL )
									uncertainAboutAnswerRelationSpecificationItem_ = answerSpecificationItem;
								else
									return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The uncertain about relation specification item is already assigned" );
								}
							else
								{
								answerParameter = ( isNegativeAnswer &&
													commonVariables_->isFirstAnswerToQuestion ? WORD_PARAMETER_INTERJECTION_NO : NO_ANSWER_PARAMETER );

								if( myWordItem_->writeSelectedSpecification( true, true, false, answerSpecificationItem->isNegative(), answerParameter, answerSpecificationItem ) == RESULT_OK )
									answerSpecificationItem->hasAlreadyBeenWrittenAsAnswer = true;
								else
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected answer to a question" );
								}
							}

						if( !isUncertainAboutRelation &&
						strlen( commonVariables_->writeSentenceString ) > 0 )
							{
							// Show notification 'My answer:'
							if( ( commonVariables_->hasShownMessage &&
							commonVariables_->isFirstAnswerToQuestion ) ||

							// Show notification 'I am not sure, but:'
							( !isPositiveAnswer &&
							answerSpecificationItem->hasAssumptionLevel() ) )
								{
								if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( answerSpecificationItem->hasAssumptionLevel() ? INTERFACE_LISTING_I_AM_NOT_SURE_BUT : INTERFACE_LISTING_MY_ANSWER ) ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a listing header" );
								}

							if( commonVariables_->presentation->writeText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString, commonVariables_->learnedFromUserString ) == RESULT_OK )
								{
								if( isNegativeAnswer )
									isNegativeAnswer_ = true;

								commonVariables_->hasFoundAnswerToQuestion = true;
								commonVariables_->isFirstAnswerToQuestion = false;
								}
							else
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
							}
						}
					else
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is hidden" );
					}
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item isn't old" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is undefined" );

		return RESULT_OK;
		}

	SpecificationResultType findQuestionToBeAdjustedByCompoundCollection( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCompoundCollectionNr, unsigned int relationContextNr, SpecificationItem *replacingSpecificationItem, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem *adjustedQuestionSpecificationItem;
		WordItem *currentCollectionWordItem;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findQuestionToBeAdjustedByCompoundCollection";

		if( specificationWordItem != NULL )
			{
			if( specificationCompoundCollectionNr > NO_COLLECTION_NR )
				{
				if( ( currentWordItem = commonVariables_->lastPredefinedWordItem ) != NULL )
					{
					// Do for all words
					do	{
						if( ( currentCollectionWordItem = currentWordItem->collectionWordItem( specificationCompoundCollectionNr, specificationWordItem ) ) != NULL )
							{
							if( ( adjustedQuestionSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( false, false, false, true, true, isNegative, isPossessive, questionParameter, specificationCompoundCollectionNr, relationContextNr, currentCollectionWordItem ) ) != NULL )
								{
								if( adjustedQuestionSpecificationItem->isOlderItem() )
									{
									if( myWordItem_->replaceOrDeleteSpecification( adjustedQuestionSpecificationItem, replacingSpecificationItem ) == RESULT_OK )
										specificationResult.adjustedQuestionSpecificationItem = adjustedQuestionSpecificationItem;
									else
										myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a question part" );
									}
								}
							}
						}
					while( commonVariables_->result == RESULT_OK &&
					( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
					}
				else
					myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The first word item is undefined" );
				}
			else
				myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification compound collection number is undefined" );
			}
		else
			myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		specificationResult.result = commonVariables_->result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"I will sing a new song to you, O God!
 *	I will sing your praises with a ten-stringed harp." (Psalm 144:9)
 *************************************************************************/
