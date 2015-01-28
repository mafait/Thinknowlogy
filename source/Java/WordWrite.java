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
				if( writeSelectedSpecification( false, true, true, false, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, justificationSpecificationItem ) == Constants.RESULT_OK )
					{
					if( CommonVariables.writeSentenceStringBuffer != null &&
					CommonVariables.writeSentenceStringBuffer.length() > 0 )
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WRITE, Constants.INTERFACE_JUSTIFICATION_SENTENCE_START ) == Constants.RESULT_OK )
							{
							if( Presentation.writeDiacriticalText( true, false, Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a justification sentence" );
							}
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write the justification sentence start string" );
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
					if( currentSpecificationItem.specificationWordItem() == writeWordItem )
						{
						if( writeSelectedSpecification( false, false, true, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.writeSentenceStringBuffer != null &&
							CommonVariables.writeSentenceStringBuffer.length() > 0 )
								{
								if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_WRITE, ( isQuestion ? Constants.INTERFACE_LISTING_SPECIFICATION_QUESTIONS : Constants.INTERFACE_LISTING_SPECIFICATIONS ) ) == Constants.RESULT_OK )
									{
									if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
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

	protected byte writeSelectedRelationInfo( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isLanguageWord, boolean isQuestion, WordItem writeWordItem )
		{
		short currentGrammarLanguageNr = CommonVariables.currentGrammarLanguageNr;
		SpecificationResultType specificationResult;
		SpecificationItem foundAssignmentItem;
		SpecificationItem currentSpecificationItem = null;

		if( writeWordItem != null )
			{
			if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, isQuestion ) ) != null )
				{
				do	{
					if( ( isLanguageWord ||
					currentSpecificationItem.grammarLanguageNr() == currentGrammarLanguageNr ) &&

					writeWordItem.hasContextInWord( currentSpecificationItem.relationContextNr(), currentSpecificationItem.specificationWordItem() ) )
						{
						if( ( specificationResult = myWordItem_.findAssignmentByRelationWord( true, isInactiveAssignment, isArchivedAssignment, currentSpecificationItem.isPossessive(), currentSpecificationItem.questionParameter(), writeWordItem ) ).result == Constants.RESULT_OK )
							{
							foundAssignmentItem = specificationResult.foundSpecificationItem;

							if( isQuestion ||

							( isAssignment &&
							foundAssignmentItem != null ) ||

							( !isAssignment &&
							foundAssignmentItem == null ) )
								{
								if( writeSelectedSpecification( false, false, false, false, false, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) == Constants.RESULT_OK )
									{
									if( CommonVariables.writeSentenceStringBuffer != null &&
									CommonVariables.writeSentenceStringBuffer.length() > 0 )
										{
										if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isQuestion ? Constants.INTERFACE_LISTING_RELATED_QUESTIONS : Constants.INTERFACE_LISTING_RELATED_INFORMATION ) ) == Constants.RESULT_OK )
											{
											if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
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
						else
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find an assignment by relation context" );
						}
					}
				while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeSelectedSpecification( boolean isAdjustedAssumption, boolean isForcingResponseNotBeingAssignment, boolean isForcingResponseNotBeingFirstSpecification, boolean isWritingCurrentSentenceOnly, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, SpecificationItem writeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		boolean hasRelationContext;
		boolean hasSpecificationCompoundCollection;
		boolean isAssignment;
		boolean isFirstRelatedSpecification;
		boolean isPossessive;
		boolean isOlderItem;
		boolean isSpecificationGeneralization;
		boolean isWritingSentenceWithOneSpecificationOnly;
		boolean hasAssignment = false;
		boolean isCombinedSpecification = false;
		boolean isCombinedOldAndNewSpecification = false;
		boolean isLastCompoundSpecification = false;
		boolean isSelfGeneratedDefinitionConclusion = false;
		int relationContextNr;
		SpecificationItem foundRelatedSpecificationItem;
		WordItem specificationWordItem;
		WordItem currentGrammarLanguageWordItem = CommonVariables.currentGrammarLanguageWordItem;

		CommonVariables.writeSentenceStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

		if( writeSpecificationItem != null )
			{
			if( ( specificationResult = myWordItem_.findRelatedSpecification( true, writeSpecificationItem ) ).result == Constants.RESULT_OK )
				{
				isFirstRelatedSpecification = specificationResult.isFirstRelatedSpecification;
				foundRelatedSpecificationItem = specificationResult.relatedSpecificationItem;

				hasRelationContext = writeSpecificationItem.hasRelationContext();
				hasSpecificationCompoundCollection = writeSpecificationItem.hasSpecificationCompoundCollection();
				isAssignment = writeSpecificationItem.isAssignment();
				isPossessive = writeSpecificationItem.isPossessive();
				isOlderItem = writeSpecificationItem.isOlderItem();
				isSpecificationGeneralization = writeSpecificationItem.isSpecificationGeneralization();

				relationContextNr = writeSpecificationItem.relationContextNr();

				specificationWordItem = writeSpecificationItem.specificationWordItem();

				if( !isAssignment &&
				myWordItem_.firstAssignmentItem( true, true, true, isPossessive, writeSpecificationItem.questionParameter(), relationContextNr, specificationWordItem ) != null )
					hasAssignment = true;

				if( foundRelatedSpecificationItem != null &&
				foundRelatedSpecificationItem.isOlderItem() )
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
				writeSpecificationItem.isGeneralizationNoun() &&
				writeSpecificationItem.isSelfGeneratedConclusion() )
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
				writeSpecificationItem.isConditional() ||
				writeSpecificationItem.isCorrectedAssumption() ) &&

				( isCombinedOldAndNewSpecification ||
				isFirstRelatedSpecification ||
				isForcingResponseNotBeingFirstSpecification ||
				foundRelatedSpecificationItem == null ||

				( !isAdjustedAssumption &&
				!writeSpecificationItem.isQuestion() &&
				writeSpecificationItem.assumptionLevel() != foundRelatedSpecificationItem.assumptionLevel() ) ) ) )
					{
					isWritingSentenceWithOneSpecificationOnly = ( isWritingCurrentSpecificationWordOnly &&
																writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer != null );

					// To increase performance,
					// use the last written sentence if still up-to-date
					if( answerParameter == Constants.NO_ANSWER_PARAMETER &&
					writeSpecificationItem.lastWrittenSentenceStringBuffer != null &&

					( isWritingSentenceWithOneSpecificationOnly ||
					writeSpecificationItem.isUserQuestion() ||

					( myWordItem_.isUserGeneralizationWord &&
					writeSpecificationItem.isSelfGeneratedAssumption() &&
					!myWordItem_.hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) ) ||

					// No relation context
					( !hasRelationContext &&

					( !isWritingCurrentSpecificationWordOnly ||
					writeSpecificationItem.hasExclusiveSpecificationCollection() ) ) ||

					// Relation context
					( hasRelationContext &&

					( ( isOlderItem &&
					!myWordItem_.hasContextCurrentlyBeenUpdatedInAllWords( relationContextNr, specificationWordItem ) ) ||

					( !isWritingCurrentSpecificationWordOnly &&
					myWordItem_.nContextWordsInAllWords( relationContextNr, specificationWordItem ) == 1 ) ) ) ) )
						CommonVariables.writeSentenceStringBuffer = new StringBuffer( isWritingSentenceWithOneSpecificationOnly ? writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer : writeSpecificationItem.lastWrittenSentenceStringBuffer );
					else
						{
						if( currentGrammarLanguageWordItem != null )
							{
							if( myWordItem_.selectGrammarToWriteSentence( isWritingCurrentSpecificationWordOnly, answerParameter, Constants.NO_GRAMMAR_LEVEL, currentGrammarLanguageWordItem.startOfGrammarItem(), writeSpecificationItem ) == Constants.RESULT_OK )
								{
								// Under certain conditions, the last written sentence will be stored, to be re-used again if needed
								if( answerParameter == Constants.NO_ANSWER_PARAMETER &&
								!writeSpecificationItem.hasHiddenSpecificationWord() &&
								CommonVariables.writeSentenceStringBuffer != null &&

								// Any "part of" specification
								( writeSpecificationItem.isPartOf() ||

								// Any user question
								writeSpecificationItem.isUserQuestion() ||

								// Any non-exclusive non-specification-generalization specification
								( !isSpecificationGeneralization &&
								!writeSpecificationItem.isExclusiveSpecification() &&
								!writeSpecificationItem.isNegative() ) ||

								( !hasRelationContext &&

								// Any older self-generated specification without relation context
								( ( isOlderItem &&
								!writeSpecificationItem.isNegative() &&
								writeSpecificationItem.isSelfGenerated() ) ||

								// Any user specification
								( !isAssignment &&
								writeSpecificationItem.isUserSpecification() ) ) ) ||

								// Any generalization assignment with relation context
								( hasRelationContext &&
								!writeSpecificationItem.isGeneralizationAssignment() ) ) )
									{
									if( !isWritingCurrentSpecificationWordOnly )
										writeSpecificationItem.lastWrittenSentenceStringBuffer = new StringBuffer( CommonVariables.writeSentenceStringBuffer );

									if( isWritingCurrentSpecificationWordOnly &&
									writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer == null &&
									writeSpecificationItem.nInvolvedSpecificationWords( true, false ) > 1 )
										writeSpecificationItem.lastWrittenSentenceWithOneSpecificationOnlyStringBuffer = new StringBuffer( CommonVariables.writeSentenceStringBuffer );
									}
								}
							else
								return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to select the grammar to write a sentence" );
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "The current language word item is undefined" );
						}
					}
				}
			else
				return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to find a related specification" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeUpdatedSpecification( boolean isAdjustedSpecification, boolean isCorrectedAssumptionByKnowledge, boolean isCorrectedAssumptionByOppositeQuestion, SpecificationItem writeSpecificationItem )
		{
		boolean isExclusiveSpecification;

		if( writeSpecificationItem != null )
			{
			isExclusiveSpecification = writeSpecificationItem.isExclusiveSpecification();

			if( writeSelectedSpecification( true, true, isExclusiveSpecification, false, false, Constants.NO_ANSWER_PARAMETER, writeSpecificationItem ) == Constants.RESULT_OK )
				{
				if( CommonVariables.writeSentenceStringBuffer != null &&
				CommonVariables.writeSentenceStringBuffer.length() > 0 )
					{
					if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isCorrectedAssumptionByKnowledge ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_KNOWLEDGE : ( isCorrectedAssumptionByOppositeQuestion ? Constants.INTERFACE_LISTING_MY_CORRECTED_ASSUMPTIONS_BY_OPPOSITE_QUESTION : ( isAdjustedSpecification ? ( writeSpecificationItem.isQuestion() ? Constants.INTERFACE_LISTING_MY_ADJUSTED_QUESTIONS : ( writeSpecificationItem.isConcludedAssumption() ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONCLUDED : Constants.INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_ADJUSTED ) ) : ( writeSpecificationItem.isSelfGenerated() ? Constants.INTERFACE_LISTING_MY_QUESTIONS_THAT_ARE_ANSWERED : Constants.INTERFACE_LISTING_YOUR_QUESTIONS_THAT_ARE_ANSWERED ) ) ) ) ) == Constants.RESULT_OK )
						{
						if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
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
