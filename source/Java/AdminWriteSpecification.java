/*
 *	Class:			AdminWriteSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To write selected specifications as sentences
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class AdminWriteSpecification
	{
	// Private constructible variables

	private boolean hasFoundAnyWordPassingGrammarIntegrityCheck_;

	private boolean isFirstSelfGeneratedAssumption_;
	private boolean isFirstSelfGeneratedConclusion_;
	private boolean isFirstSelfGeneratedQuestion_;
	private boolean isFirstUserQuestion_;
	private boolean isFirstUserSpecifications_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private boolean haveAllWordsPassed()
		{
		ReadItem currentReadItem = adminItem_.firstActiveReadItem();

		do	{
			if( !currentReadItem.hasWordPassedGrammarIntegrityCheck &&
			currentReadItem.readWordTypeString() != null )		// Skip hidden word types
				return false;
			}
		while( ( currentReadItem = currentReadItem.nextReadItem() ) != null );

		return true;
		}

	private byte showWordsThatDidntPass()
		{
		ReadItem currentReadItem = adminItem_.firstActiveReadItem();
		String readWordTypeString;
		String writeString;

		do	{
			if( !currentReadItem.hasWordPassedGrammarIntegrityCheck &&
			( readWordTypeString = currentReadItem.readWordTypeString() ) != null )
				{
				writeString = ( readWordTypeString + " (position: " + currentReadItem.wordOrderNr() + ", word type: " + adminItem_.wordTypeNameString( currentReadItem.wordTypeNr() ) + ", word parameter: " + currentReadItem.wordParameter() + ", grammar parameter: " + currentReadItem.grammarParameter + ")" );

				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_JUSTIFICATION_SENTENCE_START ) == Constants.RESULT_OK )
					{
					if( Presentation.writeDiacriticalText( false, false, Constants.PRESENTATION_PROMPT_WARNING, writeString ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a justification sentence" );
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the justification sentence start string" );
				}
			}
		while( ( currentReadItem = currentReadItem.nextReadItem() ) != null );

		return Constants.RESULT_OK;
		}

	private byte markWordsPassingGrammarIntegrityCheck()
		{
		ReadResultType readResult = new ReadResultType();
		boolean hasFound;
		short currentOrderNr;
		short lastFoundWordOrderNr = Constants.NO_ORDER_NR;
		int writeSentenceStringBufferLength;
		int readWordTypeStringLength;
		int wordPosition = 0;
		String readWordTypeString;
		ReadItem currentReadItem;
		ReadItem startOfCurentOrderNrReadItem;
		ReadItem startNewSpecificationReadItem = null;

		if( ( currentReadItem = adminItem_.firstActiveReadItem() ) != null )
			{
			if( CommonVariables.writeSentenceStringBuffer != null &&
			( writeSentenceStringBufferLength = CommonVariables.writeSentenceStringBuffer.length() ) > 0 )
				{
				do	{
					do	{
						if( ( readWordTypeString = currentReadItem.readWordTypeString() ) != null )	// Skip hidden word types
							{
							readWordTypeStringLength = readWordTypeString.length();

							if( ( readResult = adminItem_.readWordFromString( false, false, wordPosition, readWordTypeStringLength, CommonVariables.writeSentenceStringBuffer.toString() ) ).result == Constants.RESULT_OK )
								{
								if( readResult.wordLength > 0 )
									{
									hasFound = false;
									currentOrderNr = currentReadItem.wordOrderNr();
									startOfCurentOrderNrReadItem = currentReadItem;

									do	{
										if( !hasFound )
											{
											if( !currentReadItem.hasWordPassedGrammarIntegrityCheck &&
											readWordTypeStringLength == readResult.wordLength &&
											CommonVariables.writeSentenceStringBuffer.substring( wordPosition ).startsWith( readWordTypeString ) )
												{
												hasFound = true;
												hasFoundAnyWordPassingGrammarIntegrityCheck_ = true;
												currentReadItem.hasWordPassedGrammarIntegrityCheck = true;

												if( lastFoundWordOrderNr == Constants.NO_ORDER_NR ||
												lastFoundWordOrderNr + 1 == currentReadItem.wordOrderNr() )
													{
													wordPosition = readResult.nextWordPosition;
													lastFoundWordOrderNr = currentReadItem.wordOrderNr();
													startNewSpecificationReadItem = currentReadItem.nextReadItem();
													}
												}
											else
												{
												// Skip generalization conjunctions. Example: "John has 2 sons and a daughter."
												if( currentReadItem.isGeneralizationSpecification() ||

												// Skip linked conjunctions. Example: "Guest is a user and has no password."
												currentReadItem.isLinkedGeneralizationConjunction() ||

												// Skip grammar conjunctions
												currentReadItem.isSentenceConjunction() ||

												// Skip extra comma in sentence that isn't written. See grammar file for: '( symbolComma )'
												currentReadItem.isSymbol() ||

												// Skip text until it is implemented
												currentReadItem.isText() ||

												// Skip if an article or adjective has a different parameter
												( adminItem_.hasFoundDifferentParameter() &&

												( currentReadItem.isAdjective() ||
												currentReadItem.isArticle() ) ) )
													// Skip until implemented
													currentReadItem.hasWordPassedGrammarIntegrityCheck = true;
												}
											}

										currentReadItem = currentReadItem.nextReadItem();
										}
									while( currentReadItem != null &&
									currentReadItem.wordOrderNr() == currentOrderNr );

									if( !hasFound &&
									startNewSpecificationReadItem == null )
										startNewSpecificationReadItem = startOfCurentOrderNrReadItem;
									}
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a word from the written string" );
							}
						else
							currentReadItem = currentReadItem.nextReadItem();
						}
					while( readResult.wordLength > 0 &&
					currentReadItem != null );

					if( startNewSpecificationReadItem != null &&
					currentReadItem == null )
						{
						currentReadItem = startNewSpecificationReadItem;
						wordPosition = readResult.nextWordPosition;
						}
					}
				while( currentReadItem != null &&
				readResult.nextWordPosition < writeSentenceStringBufferLength );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The write sentence string buffer is empty" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find any read words" );

		return Constants.RESULT_OK;
		}

	private byte writeSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, String storeAndRetrieveIntegrityString, WordItem writeWordItem )
		{
		boolean hasMatchingSpecificationCollection;
		boolean isCurrentSpecificationCollectedWithItself;
		boolean isExclusiveSpecification;
		boolean isForcingResponseNotBeingFirstSpecification;
		boolean isHiddenExclusiveSpecificationSubstitutionAssumption;
		boolean isPossessive;
		boolean isSelfGenerated;
		boolean isSelfGeneratedAssumption;
		boolean isWritingCurrentSpecificationWordOnly;
		boolean hasHeaderBeenWritten = false;
		boolean isIntegrityCheck = ( storeAndRetrieveIntegrityString != null );
		int currentSpecificationCollectionNr;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		JustificationItem firstJustificationItem;
		SpecificationItem currentSpecificationItem;
		SpecificationItem firstRelationSpecificationItem;
		WordItem specificationWordItem;

		CommonVariables.writeSentenceStringBuffer = new StringBuffer( Constants.EMPTY_STRING );

		if( writeWordItem != null )
			{
			userSpecificationCollectionNr = writeWordItem.userSpecificationCollectionNr();

			if( ( currentSpecificationItem = writeWordItem.firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != null )
				{
				do	{
					isHiddenExclusiveSpecificationSubstitutionAssumption = false;

					isExclusiveSpecification = currentSpecificationItem.isExclusiveSpecification();
					isPossessive = currentSpecificationItem.isPossessive();
					isSelfGenerated = currentSpecificationItem.isSelfGenerated();
					isSelfGeneratedAssumption = currentSpecificationItem.isSelfGeneratedAssumption();

					currentSpecificationCollectionNr = currentSpecificationItem.specificationCollectionNr();
					firstJustificationItem = currentSpecificationItem.firstJustificationItem();
					specificationWordItem = currentSpecificationItem.specificationWordItem();

					hasMatchingSpecificationCollection = ( currentSpecificationCollectionNr == Constants.NO_COLLECTION_NR ||
															currentSpecificationCollectionNr == userSpecificationCollectionNr );

					isCurrentSpecificationCollectedWithItself = ( specificationWordItem != null &&
																specificationWordItem.isCollectedWithItself() );

					firstRelationSpecificationItem = writeWordItem.firstRelationSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, currentSpecificationItem.isNegative(), isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

					if( firstJustificationItem != null &&
					specificationWordItem != null &&

					// Hide exclusive specification substitution assumption
					( ( firstJustificationItem.isExclusiveSpecificationSubstitutionAssumption() &&

					( ( isCurrentSpecificationCollectedWithItself &&
					!firstJustificationItem.hasHiddenPrimarySpecification() ) ||

					( !isWritingJustification &&
					!currentSpecificationItem.hasRelationContext() &&
					firstRelationSpecificationItem != null &&
					firstRelationSpecificationItem.assumptionLevel() <= currentSpecificationItem.assumptionLevel() ) ) ) ||

					// Hide extra possessive reversible conclusion with only one relation context
					( isCurrentSpecificationCollectedWithItself &&
					firstJustificationItem.isPossessiveReversibleConclusion() &&
					firstJustificationItem.orderNr > Constants.NO_ORDER_NR &&

					( firstJustificationItem.hasSecondaryAssignment() ||

					( !isPossessive &&
					firstJustificationItem.hasSecondaryUserSpecification() ) ) &&

					myWordItem_.nContextWordsInAllWords( currentSpecificationItem.relationContextNr(), specificationWordItem ) == 1 &&

					// Uncertainty starts with more than one relation word
					( CommonVariables.nUserRelationWords > 1 ||

					// Justification of older sentence has multiple relation context
					( firstJustificationItem.isOlderItem() &&
					firstJustificationItem.hasSecondarySpecificationWithMultipleRelationContext() ) ) ) ) )
						isHiddenExclusiveSpecificationSubstitutionAssumption = true;

					// Filter on self-generated specifications
					if( !isHiddenExclusiveSpecificationSubstitutionAssumption &&

					// Conclusions
					( ( isWritingSelfGeneratedConclusions &&
					currentSpecificationItem.isSelfGeneratedConclusion() &&

					// Skip recently concluded assumptions with no new information
					( !currentSpecificationItem.isConcludedAssumption() ||
					!currentSpecificationItem.isOlderItem() ||
					!currentSpecificationItem.hasNewInformation() ) ) ||

					// Assumptions
					( isSelfGeneratedAssumption &&
					isWritingSelfGeneratedAssumptions ) ||

					// Questions
					( isWritingSelfGeneratedQuestions &&
					currentSpecificationItem.isSelfGeneratedQuestion() ) ||

					// Filter on user-entered specifications
					( ( !isIntegrityCheck ||
					hasMatchingSpecificationCollection ) &&

					( ( isWritingUserSpecifications &&
					currentSpecificationItem.isUserSpecification() &&
					!currentSpecificationItem.isCorrectedAssumption() ) ||

					( isWritingUserQuestions &&
					currentSpecificationItem.isUserQuestion() ) ) ) ) &&

					// Filter on current or updated sentences
					( !isWritingCurrentSentenceOnly ||
					!adminItem_.hasFoundAnyChangeDuringThisSentence() ||	// If nothing has changed, it will result in the notification: "I know".
					currentSpecificationItem.hasNewInformation() ) )
						{
						isForcingResponseNotBeingFirstSpecification = ( isAssignment &&
																		isExclusiveSpecification &&

																		( isIntegrityCheck ||
																		isSelfGenerated ) );

						isWritingCurrentSpecificationWordOnly = ( !isIntegrityCheck &&
																!currentSpecificationItem.isSpecificationGeneralization() &&
																currentSpecificationItem.nContextRelations() > 1 );

						if( writeWordItem.writeSelectedSpecification( false, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, Constants.NO_ANSWER_PARAMETER, currentSpecificationItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.writeSentenceStringBuffer != null &&
							CommonVariables.writeSentenceStringBuffer.length() > 0 )
								{
								if( isIntegrityCheck )
									{
									if( markWordsPassingGrammarIntegrityCheck() != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to mark the words that are passing the integrity check" );
									}
								else
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
										if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isWritingSelfGeneratedConclusions ? Constants.INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? Constants.INTERFACE_LISTING_MY_ASSUMPTIONS : Constants.INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isSelfGenerated ? Constants.INTERFACE_LISTING_MY_INFORMATION : Constants.INTERFACE_LISTING_YOUR_INFORMATION ) : Constants.INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) == Constants.RESULT_OK )
											hasHeaderBeenWritten = false;
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write my conclusions header" );
										}

									if( isWritingJustification )
										{
										if( isSelfGenerated )
											{
											if( adminItem_.writeJustificationSpecification( CommonVariables.writeSentenceStringBuffer.toString(), currentSpecificationItem ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a justification line" );
											}
										}
									else
										{
										if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a sentence" );
										}
									}
								}
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a selected specification" );
						}
					}
				while( ( !isIntegrityCheck ||
				!adminItem_.hasPassedGrammarIntegrityCheck() ) &&

				( currentSpecificationItem = currentSpecificationItem.nextSelectedSpecificationItem() ) != null );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given write word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte writeSelfGeneratedSpecification( boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions, WordItem writeWordItem )
		{
		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, null, writeWordItem ) == Constants.RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, null, writeWordItem ) == Constants.RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, null, writeWordItem ) == Constants.RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, null, writeWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write self-generated archived assignments" );
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write self-generated past-tense assignments" );
				}
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write self-generated current-tense assignments" );
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write self-generated specifications" );

		return Constants.RESULT_OK;
		}

	private byte writeUserQuestions( boolean isWritingCurrentSentenceOnly, String storeAndRetrieveIntegrityString, WordItem writeWordItem )
		{
		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) == Constants.RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) == Constants.RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) == Constants.RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user questions with archived assignments" );
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user questions with past-tense assignments" );
				}
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user questions with current-tense assignments" );
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user questions without assignments" );

		return Constants.RESULT_OK;
		}

	private byte writeUserSpecifications( boolean isWritingCurrentSentenceOnly, String storeAndRetrieveIntegrityString, WordItem writeWordItem )
		{
		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) == Constants.RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) == Constants.RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) == Constants.RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user-entered archived assignments" );
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user-entered past-tense assignments" );
				}
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user-entered current-tense assignments" );
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user-entered specifications" );

		return Constants.RESULT_OK;
		}

	private byte writeSelectedSpecificationInfo( WordItem writeWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( ( currentGeneralizationItem = writeWordItem.firstSpecificationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					// Respond with active specifications
					if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( false, false, false, false, writeWordItem ) == Constants.RESULT_OK )
						{
						// Respond with active specification questions
						if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( false, false, false, true, writeWordItem ) == Constants.RESULT_OK )
							{
							// Respond with active assignments
							if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, false, false, writeWordItem ) == Constants.RESULT_OK )
								{
								// Respond with active assignment questions
								if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, false, true, writeWordItem ) == Constants.RESULT_OK )
									{
									// Respond with inactive assignments
									if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, true, false, false, writeWordItem ) == Constants.RESULT_OK )
										{
										// Respond with inactive assignment questions
										if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, true, false, true, writeWordItem ) == Constants.RESULT_OK )
											{
											// Respond with archived assignments
											if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, true, false, writeWordItem ) == Constants.RESULT_OK )
												{
												// Respond with archive assignment questions
												if( currentGeneralizationWordItem.writeSelectedSpecificationInfo( true, false, true, true, writeWordItem ) != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write archive assignment questions" );
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write archived assignments" );
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write inactive assignment questions" );
										}
									else
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write inactive assignments" );
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active assignment questions" );
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active assignments" );
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active specification questions" );
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active specifications" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte writeSelectedRelationInfo( WordItem writeWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( ( currentGeneralizationItem = writeWordItem.firstRelationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null )
					{
					if( currentGeneralizationItem.isRelation() )
						{
						// Respond with active related specifications
						if( currentGeneralizationWordItem.writeSelectedRelationInfo( false, false, false, false, writeWordItem ) == Constants.RESULT_OK )
							{
							// Respond with active related specification questions
							if( currentGeneralizationWordItem.writeSelectedRelationInfo( false, false, false, true, writeWordItem ) == Constants.RESULT_OK )
								{
								// Respond with active related assignments
								if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, false, false, writeWordItem ) == Constants.RESULT_OK )
									{
									// Respond with active related assignment questions
									if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, false, true, writeWordItem ) == Constants.RESULT_OK )
										{
										// Respond with inactive related assignments
										if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, true, false, false, writeWordItem ) == Constants.RESULT_OK )
											{
											// Respond with inactive related assignment questions
											if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, true, false, true, writeWordItem ) == Constants.RESULT_OK )
												{
												// Respond with archive related assignments
												if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, true, false, writeWordItem ) == Constants.RESULT_OK )
													{
													// Respond with archive related assignment questions
													if( currentGeneralizationWordItem.writeSelectedRelationInfo( true, false, true, true, writeWordItem ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write archive related assignment questions" );
													}
												else
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write archive related assignment" );
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write inactive related assignment questions" );
											}
										else
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active related assignments" );
										}
									else
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active related assignment assignments" );
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active related assignments" );
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active related specification questions" );
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write active related specifications" );
						}
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined generalization word" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextRelationGeneralizationItem() ) != null );
			}

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminWriteSpecification( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		hasFoundAnyWordPassingGrammarIntegrityCheck_ = false;

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void initializeAdminWriteVariables()
		{
		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;
		}

	protected byte answerQuestions()
		{
		WordItem currentWordItem;

		if( CommonVariables.isQuestionAlreadyAnswered )
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
			}

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words for an answer
			do	{
				if( currentWordItem.isWordTouchedDuringCurrentSentence )
					{
					if( currentWordItem.findAnswerToNewUserQuestion() != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find an answer to new questions of the user about word \"" + currentWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );

			if( CommonVariables.isUserQuestion &&
			!CommonVariables.hasFoundAnswerToQuestion &&
			!CommonVariables.isQuestionAlreadyAnswered )
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkForStoreAndRetrieveIntegrity( boolean isQuestion, String storeAndRetrieveIntegrityString )
		{
		ReadItem currentReadItem;

		hasFoundAnyWordPassingGrammarIntegrityCheck_ = false;

		if( ( currentReadItem = adminItem_.firstActiveReadItem() ) != null )
			{
			// Skip selections until writing of selection is implemented
			if( !currentReadItem.isSelection() &&
			// Skip compound questions
			!currentReadItem.isPluralQuestionVerb() &&
			// Skip imperatives until writing of imperatives is implemented
			!currentReadItem.isImperative() )
				{
				adminItem_.checkForChangesDuringThisSentence();

				// Skip known knowledge (no changes)
				if( adminItem_.hasFoundAnyChangeDuringThisSentence() )
					{
					do	{	// Select all generalization words in a sentence
						if( currentReadItem.isGeneralizationWord() )
							{
							if( writeInfoAboutWord( false, !isQuestion, false, false, isQuestion, false, false, false, storeAndRetrieveIntegrityString, currentReadItem.readWordItem() ) != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write info about word \"" + currentReadItem.readWordItem().anyWordTypeString() + "\"" );
							}
						}
					while( ( currentReadItem = currentReadItem.nextReadItem() ) != null );
					}

				if( currentReadItem == null &&		// Sentence isn't skipped (selections, compound questions and imperatives)
				!haveAllWordsPassed() )
					{
					if( adminItem_.isSystemStartingUp() &&
					storeAndRetrieveIntegrityString != null )
						{
						if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, Constants.EMPTY_STRING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START, storeAndRetrieveIntegrityString, ( hasFoundAnyWordPassingGrammarIntegrityCheck_ ? Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS : Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE ) ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, Constants.EMPTY_STRING, ( hasFoundAnyWordPassingGrammarIntegrityCheck_ ? Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE_DUE_TO_WORDS : Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE ) ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning" );
						}

					if( hasFoundAnyWordPassingGrammarIntegrityCheck_ )
						{
						if( showWordsThatDidntPass() == Constants.RESULT_OK )
							{
							if( CommonVariables.writeSentenceStringBuffer != null &&
							CommonVariables.writeSentenceStringBuffer.length() > 0 )
								{
								if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, CommonVariables.writeSentenceStringBuffer.toString(), Constants.INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning" );
								}
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to show the words that didn't pass" );
						}

					if( adminItem_.isSystemStartingUp() &&
					CommonVariables.hasShownWarning )
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "An integrity error occurred during startup" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions )
		{
		WordItem currentWordItem;
		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words for self-generated info
			do	{
				if( currentWordItem.isWordTouchedDuringCurrentSentence )
					{
					if( writeInfoAboutWord( true, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, false, false, null, currentWordItem ) != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write info about word \"" + currentWordItem.anyWordTypeString() + "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingCurrentSentenceOnly, boolean isWritingJustification, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions, WordItem writeWordItem )
		{
		if( isWritingSelfGeneratedConclusions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, true, false, false, writeWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write self-generated conclusions" );
			}

		if( isWritingSelfGeneratedAssumptions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, true, false, writeWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write self-generated assumptions" );
			}

		if( isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, false, true, writeWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write self-generated questions" );
			}

		return Constants.RESULT_OK;
		}

	protected byte writeInfoAboutWord( boolean isWritingCurrentSentenceOnly, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, boolean isWritingSpecificationInfo, boolean isWritingRelatedInfo, String storeAndRetrieveIntegrityString, WordItem writeWordItem )
		{
		if( isWritingUserSpecifications )
			{
			if( writeUserSpecifications( isWritingCurrentSentenceOnly, storeAndRetrieveIntegrityString, writeWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user specifications" );
			}

		if( isWritingUserQuestions )
			{
			if( writeUserQuestions( isWritingCurrentSentenceOnly, storeAndRetrieveIntegrityString, writeWordItem ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write user questions" );
			}

		// Skip integrity check for self-generated info, because it checks only
		// because it only checks needs to check the current user sentence
		if( storeAndRetrieveIntegrityString == null )
			{
			if( isWritingSelfGeneratedConclusions ||
			isWritingSelfGeneratedAssumptions ||
			isWritingSelfGeneratedQuestions )
				{
				if( writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write the self-generated info" );
				}

			if( isWritingSpecificationInfo )
				{
				if( writeSelectedSpecificationInfo( writeWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write selected specification info" );
				}

			if( isWritingRelatedInfo )
				{
				if( writeSelectedRelationInfo( writeWordItem ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write selected related info" );
				}
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"Give thanks to the Lord, for he is good!
 *	His faithful love endures forever." (Psalm 107:1)
 *
 *************************************************************************/
