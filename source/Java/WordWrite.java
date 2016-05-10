/*	Class:			WordWrite
 *	Supports class:	WordItem
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

class WordWrite
	{
	// Private constructible variables

	private WordItem myWordItem_;
	private String moduleNameString_;

	// Constructor / deconstructor

	protected WordWrite( WordItem myWordItem )
		{
		String errorString = null;

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

	protected byte writeJustificationSpecification( boolean isWritingCurrentSpecificationWordOnly, SpecificationItem justificationSpecificationItem )
		{
		if( justificationSpecificationItem != null )
			{
			if( !justificationSpecificationItem.isReplacedItem() )
				{
				if( writeSelectedSpecification( false, false, true, true, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, justificationSpecificationItem ) == Constants.RESULT_OK )
					{
					if( CommonVariables.writtenSentenceStringBuffer != null &&
					CommonVariables.writtenSentenceStringBuffer.length() > 0 )
						{
						if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE_INDENTED, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a justification sentence" );
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the selected specification with justification" );
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected specification with justification" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given justification specification item is a replaced item" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given justification specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedSpecificationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		SpecificationItem currentSpecificationItem;

		if( writeWordItem != null )
			{
			if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != null )
				{
				do	{
					if( currentSpecificationItem.specificationWordItem() == writeWordItem &&
					!currentSpecificationItem.isHiddenSpanishSpecification() )
						{
						if( writeSelectedSpecification( false, false, false, false, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.writtenSentenceStringBuffer != null &&
							CommonVariables.writtenSentenceStringBuffer.length() > 0 )
								{
								if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WRITE, ( isQuestion ? Constants.INTERFACE_LISTING_SPECIFICATION_QUESTIONS : Constants.INTERFACE_LISTING_SPECIFICATIONS ) ) == Constants.RESULT_OK )
									{
									if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a related header" );
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected specification" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedRelationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isQuestion, WordItem writeWordItem )
		{
		SpecificationItem currentSpecificationItem = null;

		if( writeWordItem != null )
			{
			if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != null )
				{
				do	{
					if( writeWordItem.hasContextInWord( currentSpecificationItem.relationContextNr(), currentSpecificationItem.specificationWordItem() ) &&
					!currentSpecificationItem.isHiddenSpanishSpecification() &&
					myWordItem_.firstRelationSpecificationItem( isInactiveAssignment, isArchivedAssignment, currentSpecificationItem.isPossessive(), currentSpecificationItem.questionParameter(), writeWordItem ) != null )
						{
						if( writeSelectedSpecification( false, false, false, false, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.writtenSentenceStringBuffer != null &&
							CommonVariables.writtenSentenceStringBuffer.length() > 0 )
								{
								if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isQuestion ? Constants.INTERFACE_LISTING_RELATED_QUESTIONS : Constants.INTERFACE_LISTING_RELATED_INFORMATION ) ) == Constants.RESULT_OK )
									{
									if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a related header" );
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a selected specification" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedSpecification( boolean isAdjustedAssumption, boolean isCheckingUserSentenceForIntegrity, boolean isForcingResponseNotBeingAssignment, boolean isForcingResponseNotBeingFirstSpecification, boolean isWritingCurrentSentenceOnly, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, SpecificationItem writeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		boolean hasRelationContext;
		boolean hasSpecificationCompoundCollection;
		boolean isAssignment;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isOlderItem;
		boolean isQuestion;
		boolean isSpecificationGeneralization;
		boolean isWritingSentenceWithOneSpecificationOnly;
		boolean hasAssignment = false;
		boolean isCombinedSpecification = false;
		boolean isCombinedOldAndNewSpecification = false;
		boolean isFirstRelatedSpecification = false;
		boolean isLastCompoundSpecification = false;
		boolean isLastRelatedSpecification = false;
		boolean isSelfGeneratedDefinitionConclusion = false;
		int relationContextNr;
		SpecificationItem firstAssignmentItem;
		SpecificationItem relatedSpecificationItem = null;
		WordItem specificationWordItem;
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

		CommonVariables.learnedFromUserStringBuffer = new StringBuffer();
		CommonVariables.writtenSentenceStringBuffer = new StringBuffer();

		if( writeSpecificationItem != null )
			{
			if( !writeSpecificationItem.isHiddenSpanishSpecification() )
				{
				hasRelationContext = writeSpecificationItem.hasRelationContext();
				hasSpecificationCompoundCollection = writeSpecificationItem.hasSpecificationCompoundCollection();
				isAssignment = writeSpecificationItem.isAssignment();
				isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
				isNegative = writeSpecificationItem.isNegative();
				isPossessive = writeSpecificationItem.isPossessive();
				isOlderItem = writeSpecificationItem.isOlderItem();
				isQuestion = writeSpecificationItem.isQuestion();
				isSpecificationGeneralization = writeSpecificationItem.isSpecificationGeneralization();

				relationContextNr = writeSpecificationItem.relationContextNr();

				if( ( specificationWordItem = writeSpecificationItem.specificationWordItem() ) != null )
					{
					if( !isWritingCurrentSpecificationWordOnly &&
					answerParameter == Constants.NO_ANSWER_PARAMETER )
						{
						if( ( specificationResult = myWordItem_.findRelatedSpecification( true, writeSpecificationItem ) ).result == Constants.RESULT_OK )
							{
							isFirstRelatedSpecification = specificationResult.isFirstRelatedSpecification;
							isLastRelatedSpecification = specificationResult.isLastRelatedSpecification;
							relatedSpecificationItem = specificationResult.relatedSpecificationItem;

							if( relatedSpecificationItem != null &&
							relatedSpecificationItem.isOlderItem() )
								{
								if( !hasAssignment &&
								!hasSpecificationCompoundCollection &&
								isWritingCurrentSentenceOnly )
									isCombinedSpecification = true;

								if( !isOlderItem )
									isCombinedOldAndNewSpecification = true;
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );
						}

					if( !isAssignment &&
					( firstAssignmentItem = myWordItem_.firstAssignmentItem( true, true, true, isPossessive, writeSpecificationItem.questionParameter(), relationContextNr, specificationWordItem ) ) != null )
						{
						hasAssignment = true;

						if( isExclusiveSpecification &&
						!firstAssignmentItem.hasRelationContext() &&
						firstAssignmentItem.isArchivedAssignment() )
							isForcingResponseNotBeingAssignment = true;
						}

					if( hasSpecificationCompoundCollection &&
					!isFirstRelatedSpecification &&
					isLastRelatedSpecification )
						isLastCompoundSpecification = true;

					if( !isSpecificationGeneralization &&
					writeSpecificationItem.isGeneralizationNoun() &&
					writeSpecificationItem.isSelfGeneratedConclusion() )
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
				writeSpecificationItem.isConditional() ||
				writeSpecificationItem.isCorrectedAssumption() ) &&

				( isCombinedOldAndNewSpecification ||
				isFirstRelatedSpecification ||
				isForcingResponseNotBeingFirstSpecification ||
				relatedSpecificationItem == null ||

				( !isAdjustedAssumption &&
				isNegative &&
				!isQuestion &&
				writeSpecificationItem.assumptionLevel() != relatedSpecificationItem.assumptionLevel() ) ) ) )
					{
					isWritingSentenceWithOneSpecificationOnly = ( isWritingCurrentSpecificationWordOnly &&
																writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer != null );

					// To increase performance, use the last written sentence if still up-to-date
					if( answerParameter == Constants.NO_ANSWER_PARAMETER &&
					writeSpecificationItem.lastWrittenSentenceStringBuffer != null &&

					( isWritingSentenceWithOneSpecificationOnly ||

					// Question
					( isQuestion &&

					( !isAdjustedAssumption ||
					writeSpecificationItem.hasCurrentActiveSentenceNr() ) ) ||

					// No relation context
					( !hasRelationContext &&

					( ( isExclusiveSpecification &&

					( !isAdjustedAssumption ||
					!hasSpecificationCompoundCollection ) ) ||

					( ( !isAdjustedAssumption ||
					!isQuestion ) &&

					myWordItem_.isUserGeneralizationWord ) ) ) ||

					// Relation context
					( hasRelationContext &&
					!myWordItem_.hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) ) ) )
						{
						// Use the last written sentence
						if( isCheckingUserSentenceForIntegrity )
							CommonVariables.writtenUserSentenceStringBuffer = new StringBuffer( writeSpecificationItem.lastWrittenSentenceStringBuffer );
						else
							CommonVariables.writtenSentenceStringBuffer = new StringBuffer( isWritingSentenceWithOneSpecificationOnly ? writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer : writeSpecificationItem.lastWrittenSentenceStringBuffer );

						}
					else
						{
						// To increase performance, use the last written sentence of a related specification
						if( isOlderItem &&
						!isQuestion &&
						relatedSpecificationItem != null &&
						relatedSpecificationItem.lastWrittenSentenceStringBuffer != null )
							// Use the last written sentence of a related specification
							CommonVariables.writtenSentenceStringBuffer = relatedSpecificationItem.lastWrittenSentenceStringBuffer;
						else
							{
							if( currentLanguageWordItem != null )
								{
								if( myWordItem_.selectGrammarToWriteSentence( isCheckingUserSentenceForIntegrity, isWritingCurrentSpecificationWordOnly, answerParameter, Constants.NO_GRAMMAR_LEVEL, currentLanguageWordItem.firstGrammarItem(), writeSpecificationItem ) == Constants.RESULT_OK )
									{
									// Under certain conditions, the last written sentence will be stored, in order to be re-used if needed
									if( answerParameter == Constants.NO_ANSWER_PARAMETER &&
									CommonVariables.writtenSentenceStringBuffer != null &&
									CommonVariables.writtenSentenceStringBuffer.length() > 0 &&

									( ( ( !isAssignment ||
									!isPossessive )&&

									writeSpecificationItem.isUserSpecification() ) ||

									( !isNegative &&

									( isOlderItem ||
									!writeSpecificationItem.isGeneralizationAssignment() ) ) ) )
										{
										if( isWritingCurrentSpecificationWordOnly )
											{
											if( writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer == null &&
											writeSpecificationItem.nInvolvedSpecificationWords( false ) > 1 )
												writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer = new StringBuffer( CommonVariables.writtenSentenceStringBuffer );
											}
										else
											writeSpecificationItem.lastWrittenSentenceStringBuffer = new StringBuffer( CommonVariables.writtenSentenceStringBuffer );
										}
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to select the grammar to write a sentence" );
								}
							else
								return myWordItem_.startErrorInWord( 1, moduleNameString_, "The current language word item is undefined" );
							}
						}

					if( currentLanguageWordItem != null &&
					writeSpecificationItem.userNr() != CommonVariables.currentUserNr &&
					CommonVariables.writtenSentenceStringBuffer != null &&
					CommonVariables.writtenSentenceStringBuffer.length() > 0 )
						CommonVariables.learnedFromUserStringBuffer = new StringBuffer( currentLanguageWordItem.interfaceString( Constants.INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_START ) + myWordItem_.userNameString( writeSpecificationItem.userNr() ) + currentLanguageWordItem.interfaceString( Constants.INTERFACE_JUSTIFICATION_LEARNED_FROM_USER_END ) );
					}
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is hidden" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeUpdatedSpecification( boolean isAdjustedSpecification, boolean isConcludedAssumption, boolean isCorrectedAssumptionByKnowledge, boolean isCorrectedAssumptionByOppositeQuestion, SpecificationItem writeSpecificationItem )
		{
		boolean isExclusiveSpecification;
		boolean wasHiddenSpanishSpecification;

		if( writeSpecificationItem != null )
			{
			isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();
			wasHiddenSpanishSpecification = writeSpecificationItem.wasHiddenSpanishSpecification();

			if( writeSelectedSpecification( true, false, true, isExclusiveSpecification, false, false, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem ) == Constants.RESULT_OK )
				{
				if( CommonVariables.writtenSentenceStringBuffer != null &&
				CommonVariables.writtenSentenceStringBuffer.length() > 0 )
					{
					if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isCorrectedAssumptionByKnowledge ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE : ( isCorrectedAssumptionByOppositeQuestion ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_QUESTION : ( isAdjustedSpecification ? ( writeSpecificationItem.isQuestion() ? Constants.INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS : ( isConcludedAssumption || writeSpecificationItem.isConcludedAssumption() ? ( wasHiddenSpanishSpecification ? Constants.INTERFACE_LISTING_MY_HIDDEN_ASSUMPTIONS_THAT_ARE_CONCLUDED : Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONCLUDED ) : ( wasHiddenSpanishSpecification ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_NOT_HIDDEN_ANYMORE : Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_ADJUSTED ) ) ) : ( writeSpecificationItem.isSelfGenerated() ? Constants.INTERFACE_LISTING_MY_QUESTIONS_THAT_ARE_ANSWERED : Constants.INTERFACE_LISTING_YOUR_QUESTIONS_THAT_ARE_ANSWERED ) ) ) ) ) == Constants.RESULT_OK )
						{
						if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) == Constants.RESULT_OK )
							{
							if( !isCorrectedAssumptionByOppositeQuestion )
								{
								if( myWordItem_.recalculateAssumptionsInAllTouchedWords() != Constants.RESULT_OK )
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to recalculate the assumptions in all touched words" );
								}
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a sentence" );
						}
					else
						return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a header" );
					}
				else
					{
					if( isExclusiveSpecification )
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't write the given specification item" );
					}
				}
			else
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the given specification item" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Tremble, O earth, at the presence of the Lord,
 *	at the presence of the God of Jacob.
 *	He turned the rock into a pool of water;
 *	yes, a spring of water flowed from solid rock." (Psalm 114:7-8)
 *************************************************************************/
