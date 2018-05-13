/*	Class:			WordQuestion
 *	Supports class:	WordItem
 *	Purpose:		To answer questions about this word
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

#include "SpecificationItem.cpp"

class WordQuestion
	{
	friend class WordItem;

	// Private constructed variables

	bool hasCurrentlyAnsweredSelfGeneratedQuestion_;
	bool hasFoundAnswerToQuestion_;
	bool hasFoundDeeperPositiveAnswer_;
	bool hasFoundNoAnswerInThisWord_;
	bool hasFoundSpecificationGeneralizationAnswer_;
	bool isNegativeAnswer_;

	SpecificationItem *uncertainAboutAnswerRelationSpecificationItem_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH];

	GlobalVariables *globalVariables_;
	InputOutput *inputOutput_;
	WordItem *myWordItem_;


	// Private functions

	signed char findAnswerToQuestion( SpecificationItem *questionSpecificationItem )
		{
		bool hasRelationContext;
		bool isAssignment;
		bool isExclusiveSpecification;
		bool isNegative;
		bool isNegativeAnswer = false;
		bool isPositiveAnswer = false;
		bool isPossessive;
		bool isUncertainAboutRelation = false;
		unsigned int generalizationContextNr;
		unsigned int relationContextNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *answerSpecificationItem = NULL;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAnswerToQuestion";

		if( questionSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		hasRelationContext = questionSpecificationItem->hasRelationContext();
		isAssignment = questionSpecificationItem->isAssignment();
		isExclusiveSpecification = questionSpecificationItem->isExclusiveSpecification();
		isNegative = questionSpecificationItem->isNegative();
		isPossessive = questionSpecificationItem->isPossessive();

		generalizationContextNr = questionSpecificationItem->generalizationContextNr();
		relationContextNr = questionSpecificationItem->relationContextNr();
		specificationCollectionNr = questionSpecificationItem->specificationCollectionNr();
		specificationWordItem = questionSpecificationItem->specificationWordItem();

		// Find correct answer
		answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

		if( isAssignment &&
		answerSpecificationItem != NULL &&
		answerSpecificationItem->isHiddenSpanishSpecification() )
			// Skip hidden specification as answer
			answerSpecificationItem = NULL;

		if( answerSpecificationItem == NULL )
			{
			// Find answer with different relation context
			if( ( answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, specificationWordItem ) ) == NULL )
				{
				// Find negative answer
				if( ( answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, !isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) == NULL )
					{
					// Find opposite possessive answer
					if( ( answerSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, isNegative, !isPossessive, NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != NULL )
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
				answerSpecificationItem->assumptionLevel() <= questionSpecificationItem->assumptionLevel() &&
				// If a specification question is mapped to an assignment question, the answer is negative
				answerSpecificationItem->isRelatedQuestion( isExclusiveSpecification, specificationCollectionNr, relationContextNr ) )
					isPositiveAnswer = true;
				}

			if( ( isPositiveAnswer ||
			isNegativeAnswer ) &&

			!hasRelationContext &&
			answerSpecificationItem->isAssignment() &&
			answerSpecificationItem->hasRelationContext() &&

			// Ambiguity: Missing relation context
			inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_QUESTION_MISSING_RELATION ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'ambiguous question missing relation' interface notification" );
			}

		if( answerSpecificationItem != NULL &&
		// Ignore suggestive assumptions
		answerSpecificationItem->isOlderItem() &&
		!answerSpecificationItem->isHiddenSpanishSpecification() &&

		writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );

		return RESULT_OK;
		}

	signed char findAlternativeAnswerToQuestion( SpecificationItem *questionSpecificationItem )
		{
		bool isAssignment;
		bool isNegative;
		bool isPossessive;
		unsigned int generalizationContextNr;
		unsigned int relationContextNr;
		unsigned int specificationCollectionNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAlternativeAnswerToQuestion";

		hasFoundSpecificationGeneralizationAnswer_ = false;

		if( questionSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( questionSpecificationItem->isAssignment(), questionSpecificationItem->isInactiveAssignment(), questionSpecificationItem->isArchivedAssignment(), false ) ) != NULL )
			{
			isAssignment = ( questionSpecificationItem->isAssignment() ||
							questionSpecificationItem->hasRelationContext() );

			isNegative = questionSpecificationItem->isNegative();
			isPossessive = questionSpecificationItem->isPossessive();
			generalizationContextNr = questionSpecificationItem->generalizationContextNr();
			relationContextNr = questionSpecificationItem->relationContextNr();
			specificationCollectionNr = questionSpecificationItem->specificationCollectionNr();
			specificationWordItem = questionSpecificationItem->specificationWordItem();

			do	{
				if( currentSpecificationItem->isOlderItem() )
					{
					if( currentSpecificationItem->isSpecificationGeneralization() )
						{
						if( writeAnswerToQuestion( true, false, false, currentSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );

						hasFoundSpecificationGeneralizationAnswer_ = true;
						}
					else
						{
						if( relationContextNr == NO_CONTEXT_NR ||
						uncertainAboutAnswerRelationSpecificationItem_ == NULL )
							{
							if( hasFoundSpecificationGeneralizationAnswer_ ||
							currentSpecificationItem->isRelatedSpecification( isNegative, isPossessive, specificationCollectionNr, relationContextNr ) )
								{
								if( writeAnswerToQuestion( true, false, false, currentSpecificationItem ) != RESULT_OK )
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
								}
							else
								{
								if( questionSpecificationItem->isSpecificationGeneralization() &&
								( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
								currentSpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) != NULL )
									{
									hasFoundDeeperPositiveAnswer_ = true;
									hasFoundSpecificationGeneralizationAnswer_ = true;
									}
								}
							}
						}
					}
				}
			while( !hasFoundSpecificationGeneralizationAnswer_ &&
			( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char findAlternativeAnswerToQuestionInOtherWords( bool hasFoundSpecificationGeneralizationAnswer, SpecificationItem *questionSpecificationItem )
		{
		bool isAssignment;
		bool isArchivedAssignment;
		bool isNegative;
		bool isPossessive;
		bool isSpecificationWordSpanishAmbiguous;
		unsigned short generalizationWordTypeNr;
		unsigned int generalizationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAlternativeAnswerToQuestionInOtherWords";

		if( questionSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		isAssignment = questionSpecificationItem->isAssignment();
		isArchivedAssignment = questionSpecificationItem->isArchivedAssignment();
		isNegative = questionSpecificationItem->isNegative();
		isPossessive = questionSpecificationItem->isPossessive();
		generalizationWordTypeNr = questionSpecificationItem->generalizationWordTypeNr();
		generalizationContextNr = questionSpecificationItem->generalizationContextNr();
		relationContextNr = questionSpecificationItem->relationContextNr();
		specificationWordItem = questionSpecificationItem->specificationWordItem();

		if( ( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The first specification word is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		// Do for all specification words
		do	{
			if( currentSpecificationWordItem != myWordItem_ &&
			( foundSpecificationItem = currentSpecificationWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isArchivedAssignment, isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != NULL &&

			( hasFoundSpecificationGeneralizationAnswer ||

			( ( !isSpecificationWordSpanishAmbiguous ||
			!foundSpecificationItem->isHiddenSpanishSpecification() ) &&

			( foundSpecificationItem->isRelatedSpecification( isNegative, isPossessive, generalizationWordTypeNr ) ) ) ) )
				{
				if( currentSpecificationWordItem->writeAnswerToQuestion( !hasFoundDeeperPositiveAnswer_, hasFoundDeeperPositiveAnswer_, false, foundSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
				}
			}
		while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );

		return RESULT_OK;
		}

	signed char findAnswerToNewUserQuestion( SpecificationItem *questionSpecificationItem )
		{
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAnswerToNewUserQuestion";

		hasFoundDeeperPositiveAnswer_ = false;
		isNegativeAnswer_ = false;
		uncertainAboutAnswerRelationSpecificationItem_ = NULL;

		globalVariables_->hasFoundAnswerToQuestion = false;

		if( questionSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		if( !questionSpecificationItem->isQuestion() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item isn't a question" );

		if( findAnswerToQuestion( questionSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an answer to a question" );

		if( !globalVariables_->hasFoundAnswerToQuestion &&
		// Check collections for an alternative current-tense assignment answer to the question
		findAlternativeAnswerToQuestion( questionSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an alternative answer to a question" );

		if( isNegativeAnswer_ ||
		hasFoundSpecificationGeneralizationAnswer_ ||
		uncertainAboutAnswerRelationSpecificationItem_ != NULL ||

		( !globalVariables_->hasFoundAnswerToQuestion &&
		questionSpecificationItem->isAssignment() ) )
			{
			// Check other words for an alternative answer to the question
			if( findAlternativeAnswerToQuestionInOtherWords( hasFoundSpecificationGeneralizationAnswer_, questionSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find an alternative answer to a question" );
			}

		if( globalVariables_->hasFoundAnswerToQuestion &&
		uncertainAboutAnswerRelationSpecificationItem_ == NULL )
			{
			if( markQuestionAsAnswered( false, questionSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a question specification as been answered" );
			}
		else
			{
			// No knowledge available about my word
			if( myWordItem_->firstNonQuestionSpecificationItem() == NULL )
				{
				// I don't know anything at all about this word
				if( inputOutput_->writeInterfaceText( hasFoundNoAnswerInThisWord_, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_START, ( questionSpecificationItem->isSpecificationGeneralization() ? questionSpecificationItem->activeSpecificationWordTypeString() : questionSpecificationItem->activeGeneralizationWordTypeString() ), INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_END ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'I don't know anything about word' interface notification" );

				hasFoundNoAnswerInThisWord_ = true;
				globalVariables_->hasFoundAnswerToQuestion = true;
				}
			else
				{
				// A specification exists, but it isn't a question
				if( uncertainAboutAnswerRelationSpecificationItem_ != NULL )
					{
					if( myWordItem_->writeSelectedSpecification( false, false, true, true, false, false, false, NO_ANSWER_PARAMETER, uncertainAboutAnswerRelationSpecificationItem_ ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected answer to a question" );

					if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
					strlen( writtenSentenceString ) == 0 )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the selected answer to a question" );

					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_LISTING_I_ONLY_KNOW ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a header" );

					if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char findAnswersToQuestions( bool isAssignment, bool isArchivedAssignment, unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
		{
		bool isAnswerPossessive;
		bool isAnswerNegative;
		bool isDisplayingAnsweredQuestion;
		unsigned int answerSpecificationCollectionNr;
		SpecificationItem *currentQuestionSpecificationItem;
		WordItem *answerSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAnswersToQuestions";

		if( answerSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is undefined" );

		if( answerSpecificationItem->isQuestion() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is a question" );

		if( ( !answerSpecificationItem->isExclusiveSpecification() ||
		globalVariables_->nUserSpecificationWords == 1 ) &&

		( currentQuestionSpecificationItem = myWordItem_->firstSpecificationItem( isAssignment, false, isArchivedAssignment, true ) ) != NULL )
			{
			isAnswerPossessive = answerSpecificationItem->isPossessive();
			isAnswerNegative = answerSpecificationItem->isNegative();
			answerSpecificationCollectionNr = answerSpecificationItem->specificationCollectionNr();
			answerSpecificationWordItem = answerSpecificationItem->specificationWordItem();

			do	{
				// To avoid answering questions that are created during this sentence
				if( currentQuestionSpecificationItem->isOlderItem() &&
				currentQuestionSpecificationItem->isRelatedSpecification( isAnswerNegative, isAnswerPossessive, answerSpecificationCollectionNr, compoundSpecificationCollectionNr, answerSpecificationWordItem ) )
					{
					isDisplayingAnsweredQuestion = ( isAssignment ? true : ( myWordItem_->firstAnsweredQuestionAssignmentItem( isArchivedAssignment, currentQuestionSpecificationItem->isNegative(), currentQuestionSpecificationItem->isPossessive(), currentQuestionSpecificationItem->questionParameter(), currentQuestionSpecificationItem->relationContextNr(), currentQuestionSpecificationItem->specificationWordItem() ) == NULL ) );

					if( markQuestionAsAnswered( isDisplayingAnsweredQuestion, currentQuestionSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a related question as been answered" );

					currentQuestionSpecificationItem = myWordItem_->firstSpecificationItem( isAssignment, false, isArchivedAssignment, true );
					}
				else
					currentQuestionSpecificationItem = currentQuestionSpecificationItem->nextSelectedSpecificationItem();
				}
			while( currentQuestionSpecificationItem != NULL );
			}

		return RESULT_OK;
		}

	signed char markQuestionAsAnswered( SpecificationItem *questionSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markQuestionAsAnswered";

		if( questionSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		if( questionSpecificationItem->isAnsweredQuestion() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is already answered" );

		if( questionSpecificationItem->isAssignment() )
			{
			if( myWordItem_->copyAndReplaceSpecificationItem( true, questionSpecificationItem->isExclusiveGeneralization(), questionSpecificationItem->isExclusiveSpecification(), questionSpecificationItem->generalizationCollectionNr(), questionSpecificationItem->specificationCollectionNr(), questionSpecificationItem->firstJustificationItem(), questionSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to change a question assignment to an answered question assignment" );

			questionSpecificationItem = myWordItem_->firstSpecificationItem( questionSpecificationItem->isPossessive(), questionSpecificationItem->isSpecificationGeneralization(), questionSpecificationItem->questionParameter(), questionSpecificationItem->specificationWordItem() );
			}

		if( questionSpecificationItem != NULL )
			{
			if( myWordItem_->copyAndReplaceSpecificationItem( true, questionSpecificationItem->isExclusiveGeneralization(), questionSpecificationItem->isExclusiveSpecification(), questionSpecificationItem->generalizationCollectionNr(), questionSpecificationItem->specificationCollectionNr(), questionSpecificationItem->firstJustificationItem(), questionSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to change a question specification to an answered question specification" );

			hasFoundAnswerToQuestion_ = true;
			}

		return RESULT_OK;
		}

	signed char markQuestionAsAnswered( bool isDisplayingAnsweredQuestion, SpecificationItem *questionSpecificationItem )
		{
		SpecificationItem *relatedSpecificationItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markQuestionAsAnswered";

		if( questionSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item is undefined" );

		if( !questionSpecificationItem->isQuestion() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given question specification item isn't a question" );

		if( isDisplayingAnsweredQuestion )
			{
			if( questionSpecificationItem->isSelfGeneratedQuestion() )
				hasCurrentlyAnsweredSelfGeneratedQuestion_ = true;

			if( myWordItem_->writeUpdatedSpecification( false, false, false, false, false, false, questionSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the answered question" );

			if( !questionSpecificationItem->isSpecificationWordSpanishAmbiguous() &&
			myWordItem_->recalculateAssumptionsInWord() != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to recalculate my assumptions" );
			}

		if( questionSpecificationItem->hasSpecificationCollection() )
			{
			// First mark related question parts as answered
			do	{
				if( ( relatedResult = myWordItem_->findRelatedSpecification( false, questionSpecificationItem ) ).result != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related answered question part" );

				if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
				markQuestionAsAnswered( relatedSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a related question as been answered" );
				}
			while( relatedSpecificationItem != NULL );
			}

		// Now mark the given question part as answered
		if( markQuestionAsAnswered( questionSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to mark a question as been answered" );

		return RESULT_OK;
		}

	SpecificationItem *firstActiveNewUserQuestion()
		{
		SpecificationItem *firstNewUserQuestion = NULL;
		SpecificationItem *firstQuestionSpecificationItem;

		// Try to find an assignment first
		if( ( firstQuestionSpecificationItem = myWordItem_->firstQuestionAssignmentItem() ) != NULL )
			firstNewUserQuestion = firstQuestionSpecificationItem->firstNewUserQuestion();

		// If not found, try to find a specification
		if( firstNewUserQuestion == NULL &&
		( firstQuestionSpecificationItem = myWordItem_->firstActiveQuestionSpecificationItem() ) != NULL )
			return firstQuestionSpecificationItem->firstNewUserQuestion();

		return firstNewUserQuestion;
		}


	protected:
	// Constructor

	WordQuestion( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasCurrentlyAnsweredSelfGeneratedQuestion_ = false;
		hasFoundAnswerToQuestion_ = false;
		hasFoundDeeperPositiveAnswer_ = false;
		hasFoundNoAnswerInThisWord_ = false;
		hasFoundSpecificationGeneralizationAnswer_ = false;
		isNegativeAnswer_ = false;

		uncertainAboutAnswerRelationSpecificationItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "WordQuestion" );

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given my word item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				myWordItem_->startSystemErrorInWord( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeWordQuestionVariables()
		{
		hasCurrentlyAnsweredSelfGeneratedQuestion_ = false;
		hasFoundAnswerToQuestion_ = false;
		}

	bool hasCurrentlyAnsweredSelfGeneratedQuestion()
		{
		return hasCurrentlyAnsweredSelfGeneratedQuestion_;
		}

	bool hasFoundAnswerToQuestion()
		{
		return hasFoundAnswerToQuestion_;
		}

	signed char findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAnswersToQuestions";

		// Find answers in active assignments
		if( findAnswersToQuestions( true, false, compoundSpecificationCollectionNr, answerSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a posssible active assignment user question and mark it as been answered" );

		// Find answers in archived assignments
		if( findAnswersToQuestions( true, true, compoundSpecificationCollectionNr, answerSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a posssible archived assignment user question and mark it as been answered" );

		// Find answers in specifications
		if( findAnswersToQuestions( false, false, compoundSpecificationCollectionNr, answerSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a posssible specification user question and mark it as been answered" );

		return RESULT_OK;
		}

	signed char findAnswerToNewUserQuestion()
		{
		SpecificationItem *questionSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findAnswerToNewUserQuestion";

		hasFoundNoAnswerInThisWord_ = false;

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

	signed char writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem )
		{
		unsigned short answerParameter;
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeAnswerToQuestion";

		if( answerSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is undefined" );

		if( !answerSpecificationItem->isOlderItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item isn't old" );

		if( !answerSpecificationItem->hasSpecificationBeenWrittenAsAnswer )
			{
			if( answerSpecificationItem->isHiddenSpanishSpecification() )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given answer specification item is hidden" );

			if( isPositiveAnswer )
				{
				if( myWordItem_->writeSpecificationSentence( answerSpecificationItem->isAssignment(), answerSpecificationItem->isArchivedAssignment(), false, answerSpecificationItem->isPossessive(), false, answerSpecificationItem->isSpecificationGeneralization(), false, WORD_PARAMETER_INTERJECTION_YES, NO_GRAMMAR_LEVEL, myWordItem_->firstCurrentLanguageWritingGrammarItem( false ), answerSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer" );

				answerSpecificationItem->hasSpecificationBeenWrittenAsAnswer = true;
				}
			else
				{
				// Neutral or negative answer
				if( isUncertainAboutRelation )
					{
					if( uncertainAboutAnswerRelationSpecificationItem_ != NULL )
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The uncertain about relation specification item is already assigned" );

					uncertainAboutAnswerRelationSpecificationItem_ = answerSpecificationItem;
					}
				else
					{
					answerParameter = ( isNegativeAnswer &&
										globalVariables_->isFirstAnswerToQuestion ? WORD_PARAMETER_INTERJECTION_NO : NO_ANSWER_PARAMETER );

					if( myWordItem_->writeSelectedSpecification( false, false, true, true, false, false, answerSpecificationItem->isNegative(), answerParameter, answerSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a selected answer to a question" );

					answerSpecificationItem->hasSpecificationBeenWrittenAsAnswer = true;
					}
				}

			if( !isUncertainAboutRelation &&
			( writtenSentenceString = globalVariables_->writtenSentenceString ) != NULL &&
			strlen( writtenSentenceString ) > 0 )
				{
				if( globalVariables_->hasDisplayedMessage &&
				globalVariables_->isFirstAnswerToQuestion &&

				// Display notification 'My answer:'
				inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_LISTING_MY_ANSWER ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a listing header" );

				if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an answer to a question" );

				if( isNegativeAnswer )
					isNegativeAnswer_ = true;

				globalVariables_->hasFoundAnswerToQuestion = true;
				globalVariables_->isFirstAnswerToQuestion = false;
				}
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"I will sing a new song to you, O God!
 *	I will sing your praises with a ten-stringed harp." (Psalm 144:9)
 *************************************************************************/
