/*
 *	Class:			WordQuestion
 *	Supports class:	WordItem
 *	Purpose:		To answer questions about this word
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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

class WordQuestion
	{
	// Private constructible variables

	private boolean hasFoundAnswerToQuestion_;
	private boolean hasFoundDeeperPositiveAnswer_;
	private boolean hasFoundNothingInThisWord_;
	private boolean hasFoundSpecificationGeneralizationAnswer_;
	private boolean isNegativeAnswer_;

	private SpecificationItem uncertainAboutAnswerRelationSpecificationItem_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte findAnswerToQuestion( SpecificationItem questionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		boolean hasRelationContext;
		boolean isAssignment;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isNegativeAnswer = false;
		boolean isPositiveAnswer = false;
		boolean isUncertainAboutRelation = false;
		short questionAssumptionLevel;
		short answerAssumptionLevel;
		int specificationCollectionNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		SpecificationItem answerSpecificationItem = null;
		WordItem specificationWordItem;

		if( questionSpecificationItem != null )
			{
			hasRelationContext = questionSpecificationItem.hasRelationContext();
			isAssignment = questionSpecificationItem.isAssignment();
			isExclusiveSpecification = questionSpecificationItem.isExclusiveSpecification();
			isNegative = questionSpecificationItem.isNegative();
			isPossessive = questionSpecificationItem.isPossessive();

			specificationCollectionNr = questionSpecificationItem.specificationCollectionNr();
			generalizationContextNr = questionSpecificationItem.generalizationContextNr();
			specificationContextNr = questionSpecificationItem.specificationContextNr();
			relationContextNr = questionSpecificationItem.relationContextNr();
			specificationWordItem = questionSpecificationItem.specificationWordItem();

			// Find correct answer
			if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == null )
				{
				// Find answer with different relation context
				if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ) == null )
					{
					// Find negative answer
					if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, !isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) == null )
						{
						// Find opposite possessive answer
						if( ( answerSpecificationItem = myWordItem_.bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isAssignment, isNegative, !isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
							isNegativeAnswer = true;
						}
					else
						isNegativeAnswer = true;
					}
				else
					{
					// Find out if the relation context of the question is a subset of the relation context of the answer
					if( hasRelationContext &&
					!answerSpecificationItem.hasRelationContext() )
						isUncertainAboutRelation = true;
					else
						isNegativeAnswer = true;
					}
				}
			else
				{
				if( ( !isExclusiveSpecification &&
				answerSpecificationItem.isExclusiveSpecification() ) ||

				answerSpecificationItem.specificationWordItem() != specificationWordItem )
					// Has different specification word
					isNegativeAnswer = true;
				else
					{
					// Only confirm the answer with 'yes' if the answer is at least as reliable as the question
					if( !isExclusiveSpecification &&
					// If a specification question is mapped to an assignment question, the answer is negative
					answerSpecificationItem.isRelatedQuestion( isExclusiveSpecification, specificationCollectionNr, relationContextNr ) )
						{
						if( ( specificationResult = questionSpecificationItem.getAssumptionLevel() ).result == Constants.RESULT_OK )
							{
							questionAssumptionLevel = specificationResult.assumptionLevel;

							if( ( specificationResult = answerSpecificationItem.getAssumptionLevel() ).result == Constants.RESULT_OK )
								{
								answerAssumptionLevel = specificationResult.assumptionLevel;

								if( answerAssumptionLevel <= questionAssumptionLevel )
									isPositiveAnswer = true;
								}
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to get the answer assumption level" );
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to get the question assumption level" );
						}
					}

				if( ( isPositiveAnswer ||
				isNegativeAnswer ) &&

				!hasRelationContext &&
				answerSpecificationItem.isAssignment() &&
				answerSpecificationItem.hasRelationContext() )
					{
					// Ambiguity: Missing relation context
					if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_AMBIGUOUS_QUESTION_MISSING_RELATION ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification about ambiguity" );
					}
				}

			if( answerSpecificationItem != null &&
			// Ignore suggestive assumptions
			answerSpecificationItem.isOlderItem() &&
			!answerSpecificationItem.isHiddenSpecification() )
				{
				if( writeAnswerToQuestion( isNegativeAnswer, isPositiveAnswer, isUncertainAboutRelation, answerSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte findAlternativeAnswerToQuestion( SpecificationItem questionSpecificationItem )
		{
		boolean isAssignment;
		boolean isNegative;
		boolean isPossessive;
		int generalizationCollectionNr;
		int specificationCollectionNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem specificationWordItem;

		hasFoundSpecificationGeneralizationAnswer_ = false;

		if( questionSpecificationItem != null )
			{
			if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( questionSpecificationItem.isAssignment(), questionSpecificationItem.isInactiveAssignment(), questionSpecificationItem.isArchivedAssignment(), false ) ) != null )
				{
				isAssignment = ( questionSpecificationItem.isAssignment() ||
								questionSpecificationItem.hasRelationContext() );

				isNegative = questionSpecificationItem.isNegative();
				isPossessive = questionSpecificationItem.isPossessive();
				generalizationCollectionNr = questionSpecificationItem.generalizationCollectionNr();
				specificationCollectionNr = questionSpecificationItem.specificationCollectionNr();
				generalizationContextNr = questionSpecificationItem.generalizationContextNr();
				specificationContextNr = questionSpecificationItem.specificationContextNr();
				relationContextNr = questionSpecificationItem.relationContextNr();
				specificationWordItem = questionSpecificationItem.specificationWordItem();

				do	{
					if( currentSpecificationItem.isOlderItem() )
						{
						if( currentSpecificationItem.isSpecificationGeneralization() )
							{
							if( writeAnswerToQuestion( true, false, false, currentSpecificationItem ) == Constants.RESULT_OK )
								hasFoundSpecificationGeneralizationAnswer_ = true;
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
							}
						else
							{
							if( relationContextNr == Constants.NO_CONTEXT_NR ||
							uncertainAboutAnswerRelationSpecificationItem_ == null )
								{
								if( hasFoundSpecificationGeneralizationAnswer_ ||
								currentSpecificationItem.isRelatedSpecification( isNegative, isPossessive, generalizationCollectionNr, specificationCollectionNr, relationContextNr ) )
									{
									if( writeAnswerToQuestion( true, false, false, currentSpecificationItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
									}
								else
									{
									if( questionSpecificationItem.isSpecificationGeneralization() &&
									( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null )
										{
										if( currentSpecificationWordItem.bestMatchingSpecificationWordSpecificationItem( false, isAssignment, true, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) != null )
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
				( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte findAlternativeAnswerToQuestionInOtherWords( boolean hasFoundSpecificationGeneralizationAnswer, SpecificationItem questionSpecificationItem )
		{
		boolean isAssignment;
		boolean isArchivedAssignment;
		boolean isNegative;
		boolean isPossessive;
		short generalizationWordTypeNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		SpecificationItem foundSpecificationItem;
		WordItem currentWordItem;
		WordItem specificationWordItem;

		if( questionSpecificationItem != null )
			{
			isAssignment = questionSpecificationItem.isAssignment();
			isArchivedAssignment = questionSpecificationItem.isArchivedAssignment();
			isNegative = questionSpecificationItem.isNegative();
			isPossessive = questionSpecificationItem.isPossessive();
			generalizationWordTypeNr = questionSpecificationItem.generalizationWordTypeNr();
			generalizationContextNr = questionSpecificationItem.generalizationContextNr();
			specificationContextNr = questionSpecificationItem.specificationContextNr();
			relationContextNr = questionSpecificationItem.relationContextNr();
			specificationWordItem = questionSpecificationItem.specificationWordItem();

			if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
				{
				// Do for all words for an alternative answer
				do	{
					if( currentWordItem != myWordItem_ &&
					( foundSpecificationItem = currentWordItem.bestMatchingSpecificationWordSpecificationItem( false, isAssignment, isArchivedAssignment, isNegative, isPossessive, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
						{
						if( hasFoundSpecificationGeneralizationAnswer ||

						( foundSpecificationItem.isRelatedSpecification( isNegative, isPossessive, generalizationWordTypeNr ) &&
						!foundSpecificationItem.isHiddenSpecification() ) )
							{
							if( currentWordItem.writeAnswerToQuestion( !hasFoundDeeperPositiveAnswer_, hasFoundDeeperPositiveAnswer_, false, foundSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
							}
						}
					}
				while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The first word item is undefined" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte findAnswerToNewUserQuestion( SpecificationItem questionSpecificationItem )
		{
		hasFoundDeeperPositiveAnswer_ = false;
		isNegativeAnswer_ = false;
		uncertainAboutAnswerRelationSpecificationItem_ = null;

		CommonVariables.hasFoundAnswerToQuestion = false;

		if( questionSpecificationItem != null )
			{
			if( questionSpecificationItem.isQuestion() )
				{
				if( findAnswerToQuestion( questionSpecificationItem ) == Constants.RESULT_OK )
					{
					if( !CommonVariables.hasFoundAnswerToQuestion )
						{
						// Check collections for an alternative current-tense assignment answer to the question
						if( findAlternativeAnswerToQuestion( questionSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an alternative answer to a question" );
						}

					if( isNegativeAnswer_ ||
					hasFoundSpecificationGeneralizationAnswer_ ||
					uncertainAboutAnswerRelationSpecificationItem_ != null ||

					( !CommonVariables.hasFoundAnswerToQuestion &&
					questionSpecificationItem.isAssignment() ) )
						{
						// Check other words for an alternative answer to the question
						if( findAlternativeAnswerToQuestionInOtherWords( hasFoundSpecificationGeneralizationAnswer_, questionSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an alternative answer to a question" );
						}

					if( CommonVariables.hasFoundAnswerToQuestion &&
					uncertainAboutAnswerRelationSpecificationItem_ == null )
						{
						if( markQuestionAsAnswered( false, questionSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a question specification as been answered" );
						}
					else
						{
						if( myWordItem_.firstNonQuestionSpecificationItem() == null )
							{
							// I don't know anything at all about this word
							if( Presentation.writeInterfaceText( hasFoundNothingInThisWord_, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_START, ( questionSpecificationItem.isSpecificationGeneralization() ? questionSpecificationItem.activeSpecificationWordTypeString() : questionSpecificationItem.activeGeneralizationWordTypeString() ), Constants.INTERFACE_QUESTION_I_DONT_KNOW_ANYTHING_ABOUT_WORD_END ) == Constants.RESULT_OK )
								{
								hasFoundNothingInThisWord_ = true;
								CommonVariables.hasFoundAnswerToQuestion = true;
								}
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an interface notification" );
							}
						else
							{
							// A specification exists, but it isn't a question
							if( uncertainAboutAnswerRelationSpecificationItem_ != null )
								{
								if( myWordItem_.writeSelectedSpecification( true, true, false, false, Constants.NO_ANSWER_PARAMETER, uncertainAboutAnswerRelationSpecificationItem_ ) == Constants.RESULT_OK )
									{
									if( CommonVariables.writeSentenceStringBuffer != null &&
									CommonVariables.writeSentenceStringBuffer.length() > 0 )
										{
										if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_LISTING_I_ONLY_KNOW ) == Constants.RESULT_OK )
											{
											if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
											}
										else
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a header" );
										}
									else
										return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the selected answer to a question" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected answer to a question" );
								}
							}
						}
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an answer to a question" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item isn't a question" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte findAnswersToQuestions( boolean isAssignment, boolean isArchivedAssignment, int compoundSpecificationCollectionNr, SpecificationItem answerSpecificationItem )
		{
		boolean isAnswerPossessive;
		boolean isAnswerNegative;
		boolean isAssignmentQuestionMarkedAsAnswered;
		int answerGeneralizationCollectionNr;
		int answerSpecificationCollectionNr;
		SpecificationItem currentQuestionSpecificationItem;
		WordItem answerSpecificationWordItem;

		if( answerSpecificationItem != null )
			{
			if( !answerSpecificationItem.isQuestion() )
				{
				if( ( currentQuestionSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAssignment, false, isArchivedAssignment, true ) ) != null )
					{
					isAnswerPossessive = answerSpecificationItem.isPossessive();
					isAnswerNegative = answerSpecificationItem.isNegative();

					answerGeneralizationCollectionNr = answerSpecificationItem.generalizationCollectionNr();
					answerSpecificationCollectionNr = answerSpecificationItem.specificationCollectionNr();

					answerSpecificationWordItem = answerSpecificationItem.specificationWordItem();

					do	{
						// To avoid answering questions that are created during this sentence
						if( currentQuestionSpecificationItem.isOlderItem() &&
						currentQuestionSpecificationItem.isRelatedSpecification( isAnswerNegative, isAnswerPossessive, answerGeneralizationCollectionNr, answerSpecificationCollectionNr, compoundSpecificationCollectionNr, answerSpecificationWordItem ) )
							{
							isAssignmentQuestionMarkedAsAnswered = ( isAssignment ? false : ( myWordItem_.firstAnsweredQuestionAssignmentItem( isArchivedAssignment, currentQuestionSpecificationItem.isNegative(), currentQuestionSpecificationItem.isPossessive(), currentQuestionSpecificationItem.questionParameter(), currentQuestionSpecificationItem.relationContextNr(), currentQuestionSpecificationItem.specificationWordItem() ) != null ) );

							if( markQuestionAsAnswered( !isAssignmentQuestionMarkedAsAnswered, currentQuestionSpecificationItem ) == Constants.RESULT_OK )
								currentQuestionSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAssignment, false, isArchivedAssignment, true );
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a related question as been answered" );
							}
						else
							currentQuestionSpecificationItem = currentQuestionSpecificationItem.nextSelectedSpecificationItem();
						}
					while( currentQuestionSpecificationItem != null );
					}
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is a question" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte markQuestionAsAnswered( SpecificationItem questionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem answeredQuestionSpecificationItem;

		if( questionSpecificationItem != null )
			{
			if( !questionSpecificationItem.isAnsweredQuestion() )
				{
				if( questionSpecificationItem.isAssignment() )
					{
					if( ( specificationResult = myWordItem_.copySpecificationItem( questionSpecificationItem.isInactiveAssignment(), questionSpecificationItem.isArchivedAssignment(), true, questionSpecificationItem.isExclusiveSpecification(), questionSpecificationItem.assignmentLevel(), questionSpecificationItem.generalizationCollectionNr(), questionSpecificationItem.specificationCollectionNr(), questionSpecificationItem.relationContextNr(), questionSpecificationItem.firstJustificationItem(), questionSpecificationItem ) ).result == Constants.RESULT_OK )
						{
						if( ( answeredQuestionSpecificationItem = specificationResult.createdSpecificationItem ) != null )
							{
							if( myWordItem_.replaceOrDeleteSpecification( questionSpecificationItem, answeredQuestionSpecificationItem ) == Constants.RESULT_OK )
								// Get the specification of this assignment question
								questionSpecificationItem = myWordItem_.firstSpecificationItem( questionSpecificationItem.isPossessive(), questionSpecificationItem.questionParameter(), questionSpecificationItem.specificationWordItem() );
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace an answered question assignment" );
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't create an answered question assignment item" );
						}
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to copy a question assignment to an answered question assignment" );
					}

				if( questionSpecificationItem != null )
					{
					if( ( specificationResult = myWordItem_.copySpecificationItem( questionSpecificationItem.isInactiveAssignment(), questionSpecificationItem.isArchivedAssignment(), true, questionSpecificationItem.isExclusiveSpecification(), questionSpecificationItem.assignmentLevel(), questionSpecificationItem.generalizationCollectionNr(), questionSpecificationItem.specificationCollectionNr(), questionSpecificationItem.relationContextNr(), questionSpecificationItem.firstJustificationItem(), questionSpecificationItem ) ).result == Constants.RESULT_OK )
						{
						if( ( answeredQuestionSpecificationItem = specificationResult.createdSpecificationItem ) != null )
							{
							if( myWordItem_.replaceOrDeleteSpecification( questionSpecificationItem, answeredQuestionSpecificationItem ) == Constants.RESULT_OK )
								hasFoundAnswerToQuestion_ = true;
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace an answered question specification" );
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't create an answered question specification item" );
						}
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to copy a question specification to an answered question specification" );
					}
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is already answered" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte markQuestionAsAnswered( boolean isShowingAnsweredQuestion, SpecificationItem questionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		SpecificationItem relatedSpecificationItem;

		if( questionSpecificationItem != null )
			{
			if( questionSpecificationItem.isQuestion() )
				{
				if( isShowingAnsweredQuestion )
					{
					if( myWordItem_.writeUpdatedSpecification( false, false, false, questionSpecificationItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the answered question" );
					}

				// First mark related question parts as answered
				do	{
					if( ( specificationResult = myWordItem_.findRelatedSpecification( false, questionSpecificationItem ) ).result == Constants.RESULT_OK )
						{
						if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null )
							{
							if( markQuestionAsAnswered( relatedSpecificationItem ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a related question as been answered" );
							}
						}
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related answered question part" );
					}
				while( relatedSpecificationItem != null );

				// Now mark the given question part as answered
				if( markQuestionAsAnswered( questionSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to mark a question as been answered" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item isn't a question" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given question specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private SpecificationItem firstActiveNewUserQuestion()
		{
		SpecificationItem firstSpecificationItem;
		SpecificationItem firstNewUserQuestion = null;

		// Try to find an assignment first
		if( ( firstSpecificationItem = myWordItem_.firstQuestionAssignmentItem() ) != null )
			firstNewUserQuestion = firstSpecificationItem.firstNewUserQuestion();

		// If not found, try to find a specification
		if( firstNewUserQuestion == null &&
		( firstSpecificationItem = myWordItem_.firstActiveQuestionSpecificationItem() ) != null )
			return firstSpecificationItem.firstNewUserQuestion();

		return firstNewUserQuestion;
		}


	// Constructor / deconstructor

	protected WordQuestion( WordItem myWordItem )
		{
		String errorString = null;

		hasFoundAnswerToQuestion_ = false;
		hasFoundDeeperPositiveAnswer_ = false;
		hasFoundNothingInThisWord_ = false;
		hasFoundSpecificationGeneralizationAnswer_ = false;
		isNegativeAnswer_ = false;

		uncertainAboutAnswerRelationSpecificationItem_ = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void clearFoundAnswerToQuestion()
		{
		hasFoundAnswerToQuestion_ = false;
		}

	protected boolean hasFoundAnswerToQuestion()
		{
		return hasFoundAnswerToQuestion_;
		}

	protected byte findAnswersToQuestions( int compoundSpecificationCollectionNr, SpecificationItem answerSpecificationItem )
		{
		if( findAnswersToQuestions( true, false, compoundSpecificationCollectionNr, answerSpecificationItem ) == Constants.RESULT_OK )
			{
			if( findAnswersToQuestions( true, true, compoundSpecificationCollectionNr, answerSpecificationItem ) == Constants.RESULT_OK )
				{
				if( findAnswersToQuestions( false, false, compoundSpecificationCollectionNr, answerSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a posssible specification user question and mark it as been answered" );
				}
			else
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a posssible archived assignment user question and mark it as been answered" );
			}
		else
			return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a posssible active assignment user question and mark it as been answered" );

		return Constants.RESULT_OK;
		}

	protected byte findAnswerToNewUserQuestion()
		{
		SpecificationItem questionSpecificationItem;

		hasFoundNothingInThisWord_ = false;

		if( ( questionSpecificationItem = firstActiveNewUserQuestion() ) != null )
			{
			do	{
				if( findAnswerToNewUserQuestion( questionSpecificationItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an answer to a question" );
				}
			while( ( questionSpecificationItem = ( questionSpecificationItem.isDeletedItem() ? firstActiveNewUserQuestion() : questionSpecificationItem.nextNewUserQuestion() ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeAnswerToQuestion( boolean isNegativeAnswer, boolean isPositiveAnswer, boolean isUncertainAboutRelation, SpecificationItem answerSpecificationItem )
		{
		short answerParameter;
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

		if( answerSpecificationItem != null )
			{
			if( answerSpecificationItem.isOlderItem() )
				{
				if( !answerSpecificationItem.hasAlreadyBeenWrittenAsAnswer )
					{
					if( !answerSpecificationItem.isHiddenSpecification() )
						{
						if( isPositiveAnswer )
							{
							if( currentLanguageWordItem != null )
								{
								if( myWordItem_.selectGrammarToWriteSentence( false, Constants.WORD_PARAMETER_INTERJECTION_YES, Constants.NO_GRAMMAR_LEVEL, currentLanguageWordItem.startOfGrammarItem(), answerSpecificationItem ) == Constants.RESULT_OK )
									answerSpecificationItem.hasAlreadyBeenWrittenAsAnswer = true;
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to select the grammar for an answer" );
								}
							else
								return myWordItem_.startErrorInWord( 1, moduleNameString_, "The current language word item is undefined" );
							}
						else
							{
							// Neutral or negative answer
							if( isUncertainAboutRelation )
								{
								if( uncertainAboutAnswerRelationSpecificationItem_ == null )
									uncertainAboutAnswerRelationSpecificationItem_ = answerSpecificationItem;
								else
									return myWordItem_.startErrorInWord( 1, moduleNameString_, "The uncertain about relation specification item is already assigned" );
								}
							else
								{
								answerParameter = ( isNegativeAnswer &&
													CommonVariables.isFirstAnswerToQuestion ? Constants.WORD_PARAMETER_INTERJECTION_NO : Constants.NO_ANSWER_PARAMETER );

								if( myWordItem_.writeSelectedSpecification( true, true, false, answerSpecificationItem.isNegative(), answerParameter, answerSpecificationItem ) == Constants.RESULT_OK )
									answerSpecificationItem.hasAlreadyBeenWrittenAsAnswer = true;
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected answer to a question" );
								}
							}

						if( !isUncertainAboutRelation &&
						CommonVariables.writeSentenceStringBuffer != null &&
						CommonVariables.writeSentenceStringBuffer.length() > 0 )
							{
							// Show notification 'My answer:'
							if( ( CommonVariables.hasShownMessage &&
							CommonVariables.isFirstAnswerToQuestion ) ||

							// Show notification 'I am not sure, but:'
							( !isPositiveAnswer &&
							answerSpecificationItem.hasAssumptionLevel() ) )
								{
								if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( answerSpecificationItem.hasAssumptionLevel() ? Constants.INTERFACE_LISTING_I_AM_NOT_SURE_BUT : Constants.INTERFACE_LISTING_MY_ANSWER ) ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a listing header" );
								}

							if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) == Constants.RESULT_OK )
								{
								if( isNegativeAnswer )
									isNegativeAnswer_ = true;

								CommonVariables.hasFoundAnswerToQuestion = true;
								CommonVariables.isFirstAnswerToQuestion = false;
								}
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write an answer to a question" );
							}
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is hidden" );
					}
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item isn't old" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given answer specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType findQuestionToBeAdjustedByCompoundCollection( boolean isNegative, boolean isPossessive, short questionParameter, int specificationCompoundCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		WordItem currentCollectionWordItem;
		WordItem currentWordItem;

		if( specificationWordItem != null )
			{
			if( specificationCompoundCollectionNr > Constants.NO_COLLECTION_NR )
				{
				if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
					{
					// Do for all words
					do	{
						if( ( currentCollectionWordItem = currentWordItem.collectionWordItem( specificationCompoundCollectionNr, specificationWordItem ) ) != null )
							{
							if( ( specificationResult.adjustedQuestionSpecificationItem = myWordItem_.bestMatchingRelationContextNrSpecificationItem( false, false, false, true, true, isNegative, isPossessive, questionParameter, specificationCompoundCollectionNr, relationContextNr, currentCollectionWordItem ) ) != null )
								{
								if( myWordItem_.replaceOrDeleteSpecification( specificationResult.adjustedQuestionSpecificationItem, null ) != Constants.RESULT_OK )
									myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to replace or delete a question part" );
								}
							}
						}
					while( CommonVariables.result == Constants.RESULT_OK &&
					( currentWordItem = currentWordItem.nextWordItem() ) != null );
					}
				else
					myWordItem_.startErrorInWord( 1, moduleNameString_, "The first word item is undefined" );
				}
			else
				myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification compound collection number is undefined" );
			}
		else
			myWordItem_.startErrorInWord( 1, moduleNameString_, "The given specification word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}
	};

/*************************************************************************
 *	"I will sing a new song to you, O God!
 *	I will sing your praises with a ten-stringed harp." (Psalm 144:9)
 *************************************************************************/
