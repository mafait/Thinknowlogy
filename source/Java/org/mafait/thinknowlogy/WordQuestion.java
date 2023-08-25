﻿/*	Class:			WordQuestion
 *	Supports class:	WordItem
 *	Purpose:		Answering questions about this word
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

package org.mafait.thinknowlogy;

class WordQuestion
	{
	// Private constructed variables

	private boolean hasFoundDeeperPositiveAnswer_ = false;
	private boolean hasFoundDifferentSpecificationCollection_ = false;
	private boolean hasFoundNoAnswerInThisWord_ = false;
	private boolean hasFoundSpecificationGeneralizationAnswer_ = false;
	private boolean isNegativeAnswer_ = false;

	private SpecificationItem uncertainAboutAnswerRelationSpecificationItem_ = null;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private WordItem myWordItem_;


	// Private methods

	private byte findAnswerToQuestion( SpecificationItem questionSpecificationItem )
		{
		boolean hasRelationContext;
		boolean isAssignment;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isNegativeAnswer = false;
		boolean isPositiveAnswer = false;
		boolean isPossessive;
		boolean isUncertainAboutRelation = false;
		int generalizationContextNr;
		int relationContextNr;
		int specificationCollectionNr;
		SpecificationItem answerSpecificationItem = null;
		WordItem specificationWordItem;

		if( questionSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		hasRelationContext = questionSpecificationItem.hasRelationContext();
		isAssignment = questionSpecificationItem.isAssignment();
		isExclusiveSpecification = questionSpecificationItem.isExclusiveSpecification();
		isNegative = questionSpecificationItem.isNegative();
		isPossessive = questionSpecificationItem.isPossessive();

		generalizationContextNr = questionSpecificationItem.generalizationContextNr();
		relationContextNr = questionSpecificationItem.relationContextNr();
		specificationCollectionNr = questionSpecificationItem.specificationCollectionNr();
		specificationWordItem = questionSpecificationItem.specificationWordItem();

		// Find answer
		if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ) == null &&
		// Try different specification collection
		( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != null )
			{
			if( answerSpecificationItem.isHiddenSpanishSpecification() )
				answerSpecificationItem = null;
			else
				hasFoundDifferentSpecificationCollection_ = true;
			}

		if( answerSpecificationItem == null )
			{
			// Find answer with different relation context
			if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ) == null )
				{
				// Find negative answer
				if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, !isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) == null )
					{
					// Find opposite possessive answer
					if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, isAssignment, isNegative, !isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != null )
						isNegativeAnswer = true;
					}
				else
					isNegativeAnswer = true;
				}
			else
				{
				if( answerSpecificationItem.hasRelationContext() )
					isNegativeAnswer = true;
				else
					isUncertainAboutRelation = true;
				}
			}
		else
			{
			if( answerSpecificationItem.specificationWordItem() != specificationWordItem ||

			( !isExclusiveSpecification &&

			( answerSpecificationItem.isExclusiveSpecification() ||

			( GlobalVariables.nUserSpecificationWords > 1 &&
			!answerSpecificationItem.isSpecificationGeneralization() ) ) ) )
				// Has different specification word
				isNegativeAnswer = true;
			else
				{
				// Only confirm the answer with 'yes' if the answer is at least as reliable as the question
				if( !isExclusiveSpecification &&
				answerSpecificationItem.assumptionLevel() <= questionSpecificationItem.assumptionLevel() &&
				answerSpecificationItem.specificationCollectionNr() == questionSpecificationItem.specificationCollectionNr() )
					isPositiveAnswer = true;
				}

			if( ( isPositiveAnswer ||
			isNegativeAnswer ) &&

			!hasRelationContext &&
			answerSpecificationItem.isAssignment() &&
			answerSpecificationItem.hasRelationContext() &&

			// Ambiguity: Missing relation context
			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_QUESTION_MISSING_RELATION ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the 'ambiguous question missing relation' interface notification" );
			}

		if( answerSpecificationItem != null &&
		// Ignore suggestive assumptions
		answerSpecificationItem.isOlderItem() &&
		// Write answer to question
		writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );

		return Constants.RESULT_OK;
		}

	private byte findAlternativeAnswerToQuestion( SpecificationItem questionSpecificationItem )
		{
		boolean isAssignment;
		boolean isNegative;
		boolean isPossessive;
		int generalizationContextNr;
		int relationContextNr;
		int specificationCollectionNr;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem specificationWordItem;

		hasFoundSpecificationGeneralizationAnswer_ = false;

		if( questionSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		if( ( currentSpecificationItem = myWordItem_.firstSpecificationItem( questionSpecificationItem.isAssignment(), questionSpecificationItem.isInactiveAssignment(), questionSpecificationItem.isArchivedAssignment(), false ) ) != null )
			{
			isAssignment = ( questionSpecificationItem.isAssignment() ||
							questionSpecificationItem.hasRelationContext() );

			isNegative = questionSpecificationItem.isNegative();
			isPossessive = questionSpecificationItem.isPossessive();
			generalizationContextNr = questionSpecificationItem.generalizationContextNr();
			relationContextNr = questionSpecificationItem.relationContextNr();
			specificationCollectionNr = questionSpecificationItem.specificationCollectionNr();
			specificationWordItem = questionSpecificationItem.specificationWordItem();

			do	{
				if( currentSpecificationItem.isOlderItem() )
					{
					if( currentSpecificationItem.isSpecificationGeneralization() )
						{
						if( writeAnswerToQuestion( true, false, false, currentSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );

						hasFoundSpecificationGeneralizationAnswer_ = true;
						}
					else
						{
						if( ( relationContextNr == Constants.NO_CONTEXT_NR ||
						uncertainAboutAnswerRelationSpecificationItem_ == null ) &&

						questionSpecificationItem.isSpecificationGeneralization() &&
						( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
						currentSpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) != null )
							{
							hasFoundDeeperPositiveAnswer_ = true;
							hasFoundSpecificationGeneralizationAnswer_ = true;
							}
						}
					}
				}
			while( !hasFoundSpecificationGeneralizationAnswer_ &&
			( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte findAlternativeAnswerToQuestionInOtherWords( boolean hasFoundSpecificationGeneralizationAnswer, SpecificationItem questionSpecificationItem )
		{
		boolean isAssignment;
		boolean isNegative;
		boolean isPossessive;
		boolean isSpecificationWordSpanishAmbiguous;
		short generalizationWordTypeNr;
		int generalizationContextNr;
		int relationContextNr;
		SpecificationItem foundSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem specificationWordItem;

		if( questionSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		isAssignment = questionSpecificationItem.isAssignment();
		isNegative = questionSpecificationItem.isNegative();
		isPossessive = questionSpecificationItem.isPossessive();
		generalizationWordTypeNr = questionSpecificationItem.generalizationWordTypeNr();
		generalizationContextNr = questionSpecificationItem.generalizationContextNr();
		relationContextNr = questionSpecificationItem.relationContextNr();

		if( ( specificationWordItem = questionSpecificationItem.specificationWordItem() ) == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item has no specification word" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		if( ( currentSpecificationWordItem = GlobalVariables.firstSpecificationWordItem ) == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The first specification word is undefined" );

		// Do for all specification words
		do	{
			if( currentSpecificationWordItem != myWordItem_ &&
			( foundSpecificationItem = currentSpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, isAssignment, true, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, relationContextNr, specificationWordItem ) ) != null &&

			( hasFoundSpecificationGeneralizationAnswer ||

			( ( !isSpecificationWordSpanishAmbiguous ||
			!foundSpecificationItem.isHiddenSpanishSpecification() ) &&

			// Related
			foundSpecificationItem.generalizationWordTypeNr() == generalizationWordTypeNr ) ) )
				{
				if( currentSpecificationWordItem.writeAnswerToQuestion( !hasFoundDeeperPositiveAnswer_, hasFoundDeeperPositiveAnswer_, false, foundSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
				}
			}
		while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );

		return Constants.RESULT_OK;
		}

	private byte findAnswerToNewUserQuestion( SpecificationItem questionSpecificationItem )
		{
		StringBuffer writtenSentenceStringBuffer;

		hasFoundDeeperPositiveAnswer_ = false;
		hasFoundDifferentSpecificationCollection_ = false;
		isNegativeAnswer_ = false;
		uncertainAboutAnswerRelationSpecificationItem_ = null;

		GlobalVariables.hasFoundAnswerToQuestion = false;

		if( questionSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		if( !questionSpecificationItem.isQuestion() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item isn't a question" );

		if( findAnswerToQuestion( questionSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an answer to a question" );

		if( !GlobalVariables.hasFoundAnswerToQuestion &&
		// Check collections for an alternative current-tense assignment answer to the question
		findAlternativeAnswerToQuestion( questionSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an alternative answer to a question" );

		if( isNegativeAnswer_ ||
		hasFoundSpecificationGeneralizationAnswer_ ||
		uncertainAboutAnswerRelationSpecificationItem_ != null ||

		( !GlobalVariables.hasFoundAnswerToQuestion &&
		questionSpecificationItem.isAssignment() ) )
			{
			// Check other words for an alternative answer to the question
			if( findAlternativeAnswerToQuestionInOtherWords( hasFoundSpecificationGeneralizationAnswer_, questionSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an alternative answer to a question" );
			}

		if( GlobalVariables.hasFoundAnswerToQuestion &&
		uncertainAboutAnswerRelationSpecificationItem_ == null )
			{
			if( markQuestionAsAnswered( false, questionSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a question specification as been answered" );
			}
		else
			{
			// No knowledge available about my word
			if( myWordItem_.firstNonQuestionSpecificationItem() == null )
				{
				if( !GlobalVariables.isConflictingQuestion )
					{
					// I don't know anything at all about this word
					if( InputOutput.writeInterfaceText( hasFoundNoAnswerInThisWord_, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_START, ( questionSpecificationItem.isSpecificationGeneralization() ? questionSpecificationItem.activeSpecificationWordTypeString() : questionSpecificationItem.activeGeneralizationWordTypeString() ), Constants.INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_END ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the 'I don't know anything about word' interface notification" );

					hasFoundNoAnswerInThisWord_ = true;
					GlobalVariables.hasFoundAnswerToQuestion = true;
					}
				}
			else
				{
				// A specification exists, but it isn't a question
				if( uncertainAboutAnswerRelationSpecificationItem_ != null )
					{
					if( myWordItem_.writeSelectedSpecification( false, true, true, false, false, false, Constants.NO_ANSWER_PARAMETER, uncertainAboutAnswerRelationSpecificationItem_ ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected answer to a question" );

					if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
					writtenSentenceStringBuffer.length() == 0 )
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the selected answer to a question" );

					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_LISTING_I_ONLY_KNOW ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a header" );

					if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte findAnswersToQuestions( boolean isAssignment, boolean isArchivedAssignment, int questionSpecificationCollectionNr, SpecificationItem answerSpecificationItem, SpecificationItem currentQuestionSpecificationItem )
		{
		int answerSpecificationCollectionNr;
		SpecificationItem questionAssignmentOrSpecificationItem;
		WordItem answerSpecificationWordItem;

		if( answerSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is undefined" );

		if( currentQuestionSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given first question specification item is undefined" );

		if( answerSpecificationItem.isQuestion() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is a question" );

		answerSpecificationCollectionNr = answerSpecificationItem.specificationCollectionNr();
		answerSpecificationWordItem = answerSpecificationItem.specificationWordItem();

		do	{
			if( currentQuestionSpecificationItem.isOlderItem() )
				{
				if( currentQuestionSpecificationItem.isAssignment() ||
				( questionAssignmentOrSpecificationItem = myWordItem_.firstAssignmentItem( false, true, Constants.NO_CONTEXT_NR, currentQuestionSpecificationItem.specificationWordItem() ) ) == null ||
				questionAssignmentOrSpecificationItem.creationSentenceNr() != currentQuestionSpecificationItem.creationSentenceNr() )
					questionAssignmentOrSpecificationItem = currentQuestionSpecificationItem;

				if( questionAssignmentOrSpecificationItem.isRelatedSpecification( answerSpecificationCollectionNr, questionSpecificationCollectionNr, answerSpecificationWordItem ) &&

				( answerSpecificationItem.isMatchingRelationContextNr( false, questionAssignmentOrSpecificationItem.relationContextNr() ) ||
				myWordItem_.hasCurrentlyCorrectedAssumptionByKnowledge() ) &&

				// Mark directly related question specification part as answered
				markQuestionAsAnswered( true, currentQuestionSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a related question as been answered" );
				}
			}																										// First question specification
		while( ( currentQuestionSpecificationItem = ( currentQuestionSpecificationItem.isReplacedOrDeletedItem() ? myWordItem_.firstSpecificationItem( isAssignment, false, isArchivedAssignment, true ) :
																													// Next question specification
																													currentQuestionSpecificationItem.nextSelectedSpecificationItem() ) ) != null );

		return Constants.RESULT_OK;
		}

	private byte markQuestionAsAnswered( SpecificationItem questionSpecificationItem )
		{
		if( questionSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		if( questionSpecificationItem.isAnsweredQuestion() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is already answered" );

		if( questionSpecificationItem.isAssignment() )
			{
			if( questionSpecificationItem.markAsAnsweredQuestion() != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a question assignment as an answered question" );

			questionSpecificationItem = myWordItem_.firstSpecificationItem( questionSpecificationItem.isPossessive(), questionSpecificationItem.isSpecificationGeneralization(), questionSpecificationItem.questionParameter(), questionSpecificationItem.specificationWordItem() );
			}

		if( questionSpecificationItem != null &&
		questionSpecificationItem.markAsAnsweredQuestion() != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a question specification as an answered question" );

		return Constants.RESULT_OK;
		}

	private byte markQuestionAsAnswered( boolean isDisplayingAnsweredQuestion, SpecificationItem questionSpecificationItem )
		{
		SpecificationItem relatedSpecificationItem;
		RelatedResultType relatedResult;

		if( questionSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		if( !questionSpecificationItem.isQuestion() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item isn't a question" );

		if( isDisplayingAnsweredQuestion )
			{
			if( myWordItem_.writeUpdatedSpecification( false, false, false, questionSpecificationItem.isExclusiveSpecification(), false, false, false, questionSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the answered question" );

			if( questionSpecificationItem.isUserQuestion() &&
			!myWordItem_.hasCurrentlyCorrectedAssumptionByKnowledge() &&
			// Recalculate assumption levels of my word
			myWordItem_.recalculateAssumptionLevelsInWord() != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to recalculate the assumption levels in my word" );
			}

		if( questionSpecificationItem.hasSpecificationCollection() )
			{
			// First mark related question parts as answered
			do	{
				if( ( relatedResult = myWordItem_.findRelatedSpecification( false, questionSpecificationItem ) ).result != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related answered question part" );

				if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null &&
				markQuestionAsAnswered( relatedSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a related question as been answered" );
				}
			while( relatedSpecificationItem != null );
			}

		// Now mark the given question part as answered
		if( markQuestionAsAnswered( questionSpecificationItem ) != Constants.RESULT_OK )
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a question as been answered" );

		return Constants.RESULT_OK;
		}

	private SpecificationItem firstActiveNewUserQuestion()
		{
		SpecificationItem firstQuestionSpecificationItem;

				// Try to find an assignment first
		return ( ( ( firstQuestionSpecificationItem = myWordItem_.firstQuestionAssignmentItem() ) != null &&
				!firstQuestionSpecificationItem.isOlderItem() ) ||

				// If not found, try to find a specification
				( ( firstQuestionSpecificationItem = myWordItem_.firstActiveQuestionSpecificationItem() ) != null &&
				!firstQuestionSpecificationItem.isOlderItem() ) ?
					firstQuestionSpecificationItem : null );
		}


	// Constructor

	protected WordQuestion( WordItem myWordItem )
		{
		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given my word item is undefined.\n" );
			}
		}


	// Protected methods

	protected byte findAnswersToQuestions( int questionSpecificationCollectionNr, SpecificationItem answerSpecificationItem )
		{
		SpecificationItem firstQuestionSpecificationItem;

		if( answerSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is undefined" );

		if( myWordItem_.isOlderItem() &&

		( !answerSpecificationItem.isExclusiveSpecification() ||
		GlobalVariables.nUserSpecificationWords == 1 ) )
			{
			// Find answers in active assignments
			if( ( firstQuestionSpecificationItem = myWordItem_.firstSpecificationItem( true, false, false, true ) ) != null &&
			findAnswersToQuestions( true, false, questionSpecificationCollectionNr, answerSpecificationItem, firstQuestionSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a posssible active assignment user question and mark it as been answered" );

			// Find answers in archived assignments
			if( ( firstQuestionSpecificationItem = myWordItem_.firstSpecificationItem( true, false, true, true ) ) != null &&
			findAnswersToQuestions( true, true, questionSpecificationCollectionNr, answerSpecificationItem, firstQuestionSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a posssible archived assignment user question and mark it as been answered" );

			// Find answers in specifications
			if( ( firstQuestionSpecificationItem = myWordItem_.firstSpecificationItem( false, false, false, true ) ) != null &&
			findAnswersToQuestions( false, false, questionSpecificationCollectionNr, answerSpecificationItem, firstQuestionSpecificationItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a posssible specification user question and mark it as been answered" );
			}

		return Constants.RESULT_OK;
		}

	protected byte findAnswerToNewUserQuestion()
		{
		SpecificationItem questionSpecificationItem;

		hasFoundNoAnswerInThisWord_ = false;

		if( ( questionSpecificationItem = firstActiveNewUserQuestion() ) != null )
			{
			do	{
				if( findAnswerToNewUserQuestion( questionSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an answer to a question" );
				}																						// First question specification
			while( ( questionSpecificationItem = ( questionSpecificationItem.isReplacedOrDeletedItem() ? firstActiveNewUserQuestion() :
																										// Next question specification
																										questionSpecificationItem.nextSelectedSpecificationItem() ) ) != null &&
			!questionSpecificationItem.isOlderItem() );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeAnswerToQuestion( boolean isNegativeAnswer, boolean isPositiveAnswer, boolean isUncertainAboutRelation, SpecificationItem answerSpecificationItem )
		{
		short answerParameter;
		StringBuffer writtenSentenceStringBuffer;

		if( answerSpecificationItem == null )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is undefined" );

		if( !answerSpecificationItem.isOlderItem() )
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item isn't old" );

		if( !answerSpecificationItem.hasSpecificationBeenWrittenAsAnswer )
			{
			if( answerSpecificationItem.isHiddenSpanishSpecification() )
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is hidden" );

			if( isPositiveAnswer )
				{
				if( myWordItem_.writeSpecificationSentence( answerSpecificationItem.isAssignment(), answerSpecificationItem.isArchivedAssignment(), false, answerSpecificationItem.isPossessive(), false, answerSpecificationItem.isSpecificationGeneralization(), false, Constants.WORD_PARAMETER_INTERJECTION_YES, Constants.NO_GRAMMAR_LEVEL, myWordItem_.firstCurrentLanguageWritingGrammarItem( false ), answerSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer" );

				answerSpecificationItem.hasSpecificationBeenWrittenAsAnswer = true;
				}
			else
				{
				// Neutral or negative answer
				if( isUncertainAboutRelation )
					{
					if( uncertainAboutAnswerRelationSpecificationItem_ != null )
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "The uncertain about relation specification item is already assigned" );

					uncertainAboutAnswerRelationSpecificationItem_ = answerSpecificationItem;
					}
				else
					{
					answerParameter = ( isNegativeAnswer &&
										GlobalVariables.isFirstAnswerToQuestion ? Constants.WORD_PARAMETER_INTERJECTION_NO : Constants.NO_ANSWER_PARAMETER );

					if( myWordItem_.writeSelectedSpecification( false, true, true, false, false, answerSpecificationItem.isNegative(), answerParameter, answerSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected answer to a question" );

					answerSpecificationItem.hasSpecificationBeenWrittenAsAnswer = true;
					}
				}

			if( !isUncertainAboutRelation &&
			( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) != null &&
			writtenSentenceStringBuffer.length() > 0 )
				{
				if( GlobalVariables.hasDisplayedMessage &&
				GlobalVariables.isFirstAnswerToQuestion &&
				// Display notification 'My answer:'
				InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_LISTING_MY_ANSWER ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a listing header" );

				if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );

				if( isNegativeAnswer )
					isNegativeAnswer_ = true;

				if( !hasFoundDifferentSpecificationCollection_ )
					GlobalVariables.hasFoundAnswerToQuestion = true;

				GlobalVariables.isFirstAnswerToQuestion = false;
				}
			}

		return Constants.RESULT_OK;
		}
	}

/*************************************************************************
 *	"I will sing a new song to you, O God!
 *	I will sing your praises with a ten-stringed harp." (Psalm 144:9)
 *************************************************************************/
