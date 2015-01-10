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

#include "AdminItem.h"
#include "GeneralizationItem.cpp"
#include "Presentation.cpp"
#include "ReadItem.cpp"
#include "SpecificationItem.cpp"

class AdminWriteSpecification
	{
	friend class AdminItem;

	// Private constructible variables

	bool hasFoundAnyWordPassingGrammarIntegrityCheck_;

	bool isFirstSelfGeneratedAssumption_;
	bool isFirstSelfGeneratedConclusion_;
	bool isFirstSelfGeneratedQuestion_;
	bool isFirstUserQuestion_;
	bool isFirstUserSpecifications_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool haveAllWordsPassed()
		{
		ReadItem *currentReadItem = adminItem_->firstActiveReadItem();

		do	{
			if( !currentReadItem->hasWordPassedGrammarIntegrityCheck &&
			currentReadItem->readWordTypeString() != NULL )		// Skip hidden word types
				return false;
			}
		while( ( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		return true;
		}

	ResultType showWordsThatDidntPass()
		{
		ReadItem *currentReadItem = adminItem_->firstActiveReadItem();
		char *readWordTypeString;
		char writeString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "showWordsThatDidntPass";

		do	{
			if( !currentReadItem->hasWordPassedGrammarIntegrityCheck &&
			( readWordTypeString = currentReadItem->readWordTypeString() ) != NULL )
				{
				sprintf( writeString, "%s (position: %u, word type: %s, word parameter: %u, grammar parameter: %u)", readWordTypeString, currentReadItem->wordOrderNr(), adminItem_->wordTypeNameString( currentReadItem->wordTypeNr() ), currentReadItem->wordParameter(), currentReadItem->grammarParameter );

				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_JUSTIFICATION_SENTENCE_START ) == RESULT_OK )
					{
					if( commonVariables_->presentation->writeDiacriticalText( false, false, PRESENTATION_PROMPT_WARNING, writeString ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a justification sentence" );
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the justification sentence start string" );
				}
			}
		while( ( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		return RESULT_OK;
		}

	ResultType markWordsPassingGrammarIntegrityCheck()
		{
		ReadResultType readResult;
		bool hasFound;
		unsigned short currentOrderNr;
		unsigned short lastFoundWordOrderNr = NO_ORDER_NR;
		size_t writeSentenceStringLength;
		size_t readWordTypeStringLength;
		size_t wordPosition = 0;
		char *readWordTypeString;
		ReadItem *currentReadItem;
		ReadItem *startOfCurentOrderNrReadItem;
		ReadItem *startNewSpecificationReadItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordsPassingGrammarIntegrityCheck";

		if( ( currentReadItem = adminItem_->firstActiveReadItem() ) != NULL )
			{
			if( ( writeSentenceStringLength = strlen( commonVariables_->writeSentenceString ) ) > 0 )
				{
				do	{
					do	{
						if( ( readWordTypeString = currentReadItem->readWordTypeString() ) != NULL )	// Skip hidden word types
							{
							readWordTypeStringLength = strlen( readWordTypeString );

							if( ( readResult = adminItem_->readWordFromString( false, false, wordPosition, readWordTypeStringLength, commonVariables_->writeSentenceString ) ).result == RESULT_OK )
								{
								if( readResult.wordLength > 0 )
									{
									hasFound = false;
									currentOrderNr = currentReadItem->wordOrderNr();
									startOfCurentOrderNrReadItem = currentReadItem;

									do	{
										if( !hasFound )
											{
											if( !currentReadItem->hasWordPassedGrammarIntegrityCheck &&
											readWordTypeStringLength == readResult.wordLength &&
											strncmp( &commonVariables_->writeSentenceString[wordPosition], readWordTypeString, readResult.wordLength ) == 0 )
												{
												hasFound = true;
												hasFoundAnyWordPassingGrammarIntegrityCheck_ = true;
												currentReadItem->hasWordPassedGrammarIntegrityCheck = true;

												if( lastFoundWordOrderNr == NO_ORDER_NR ||
												lastFoundWordOrderNr + 1 == currentReadItem->wordOrderNr() )
													{
													wordPosition = readResult.nextWordPosition;
													lastFoundWordOrderNr = currentReadItem->wordOrderNr();
													startNewSpecificationReadItem = currentReadItem->nextReadItem();
													}
												}
											else
												{
												// Skip generalization conjunctions. Example: "John has 2 sons and a daughter."
												if( currentReadItem->isGeneralizationSpecification() ||

												// Skip linked conjunctions. Example: "Guest is a user and has no password."
												currentReadItem->isLinkedGeneralizationConjunction() ||

												// Skip grammar conjunctions
												currentReadItem->isSentenceConjunction() ||

												// Skip extra comma in sentence that isn't written. See grammar file for: '( symbolComma )'
												currentReadItem->isSymbol() ||

												// Skip text until it is implemented
												currentReadItem->isText() ||

												// Skip if an article or adjective has a different parameter
												( adminItem_->hasFoundDifferentParameter() &&

												( currentReadItem->isAdjective() ||
												currentReadItem->isArticle() ) ) )
													// Skip until implemented
													currentReadItem->hasWordPassedGrammarIntegrityCheck = true;
												}
											}

										currentReadItem = currentReadItem->nextReadItem();
										}
									while( currentReadItem != NULL &&
									currentReadItem->wordOrderNr() == currentOrderNr );

									if( !hasFound &&
									startNewSpecificationReadItem == NULL )
										startNewSpecificationReadItem = startOfCurentOrderNrReadItem;
									}
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a word from the written string" );
							}
						else
							currentReadItem = currentReadItem->nextReadItem();
						}
					while( readResult.wordLength > 0 &&
					currentReadItem != NULL );

					if( startNewSpecificationReadItem != NULL &&
					currentReadItem == NULL )
						{
						currentReadItem = startNewSpecificationReadItem;
						wordPosition = readResult.nextWordPosition;
						}
					}
				while( currentReadItem != NULL &&
				readResult.nextWordPosition < writeSentenceStringLength );
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The write sentence string is empty" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find any read words" );

		return RESULT_OK;
		}

	ResultType writeSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, char *storeAndRetrieveIntegrityString, WordItem *writeWordItem )
		{
		bool hasMatchingSpecificationCollection;
		bool isCurrentSpecificationCollectedWithItself;
		bool isExclusiveSpecification;
		bool isForcingResponseNotBeingFirstSpecification;
		bool isHiddenExclusiveSpecificationSubstitutionAssumption;
		bool isPossessive;
		bool isSelfGenerated;
		bool isSelfGeneratedAssumption;
		bool isWritingCurrentSpecificationWordOnly;
		bool hasHeaderBeenWritten = false;
		bool isIntegrityCheck = ( storeAndRetrieveIntegrityString != NULL );
		unsigned int currentSpecificationCollectionNr;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		JustificationItem *firstJustificationItem;
		SpecificationItem *currentSpecificationItem;
		SpecificationItem *firstRelationSpecificationItem;
		WordItem *specificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSpecification";

		strcpy( commonVariables_->writeSentenceString, EMPTY_STRING );

		if( writeWordItem != NULL )
			{
			userSpecificationCollectionNr = writeWordItem->userSpecificationCollectionNr();

			if( ( currentSpecificationItem = writeWordItem->firstSelectedSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, ( isWritingUserQuestions || isWritingSelfGeneratedQuestions ) ) ) != NULL )
				{
				do	{
					isHiddenExclusiveSpecificationSubstitutionAssumption = false;

					isExclusiveSpecification = currentSpecificationItem->isExclusiveSpecification();
					isPossessive = currentSpecificationItem->isPossessive();
					isSelfGenerated = currentSpecificationItem->isSelfGenerated();
					isSelfGeneratedAssumption = currentSpecificationItem->isSelfGeneratedAssumption();

					currentSpecificationCollectionNr = currentSpecificationItem->specificationCollectionNr();
					firstJustificationItem = currentSpecificationItem->firstJustificationItem();
					specificationWordItem = currentSpecificationItem->specificationWordItem();

					hasMatchingSpecificationCollection = ( currentSpecificationCollectionNr == NO_COLLECTION_NR ||
															currentSpecificationCollectionNr == userSpecificationCollectionNr );

					isCurrentSpecificationCollectedWithItself = ( specificationWordItem != NULL &&
																specificationWordItem->isCollectedWithItself() );

					firstRelationSpecificationItem = writeWordItem->firstRelationSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, currentSpecificationItem->isNegative(), isPossessive, NO_QUESTION_PARAMETER, specificationWordItem );

					if( firstJustificationItem != NULL &&
					specificationWordItem != NULL &&

					// Hide exclusive specification substitution assumption
					( ( firstJustificationItem->isExclusiveSpecificationSubstitutionAssumption() &&

					( ( isCurrentSpecificationCollectedWithItself &&
					!firstJustificationItem->hasHiddenPrimarySpecification() ) ||

					( !isWritingJustification &&
					!currentSpecificationItem->hasRelationContext() &&
					firstRelationSpecificationItem != NULL &&
					firstRelationSpecificationItem->assumptionLevel() <= currentSpecificationItem->assumptionLevel() ) ) ) ||

					// Hide extra possessive reversible conclusion with only one relation context
					( isCurrentSpecificationCollectedWithItself &&
					firstJustificationItem->isPossessiveReversibleConclusion() &&
					firstJustificationItem->orderNr > NO_ORDER_NR &&

					( firstJustificationItem->hasSecondaryAssignment() ||

					( !isPossessive &&
					firstJustificationItem->hasSecondaryUserSpecification() ) ) &&

					myWordItem_->nContextWordsInAllWords( currentSpecificationItem->relationContextNr(), specificationWordItem ) == 1 &&

					// Uncertainty starts with more than one relation word
					( commonVariables_->nUserRelationWords > 1 ||

					// Justification of older sentence has multiple relation context
					( firstJustificationItem->isOlderItem() &&
					firstJustificationItem->hasSecondarySpecificationWithMultipleRelationContext() ) ) ) ) )
						isHiddenExclusiveSpecificationSubstitutionAssumption = true;

					// Filter on self-generated specifications
					if( !isHiddenExclusiveSpecificationSubstitutionAssumption &&

					// Conclusions
					( ( isWritingSelfGeneratedConclusions &&
					currentSpecificationItem->isSelfGeneratedConclusion() &&

					// Skip recently concluded assumptions with no new information
					( !currentSpecificationItem->isConcludedAssumption() ||
					!currentSpecificationItem->isOlderItem() ||
					!currentSpecificationItem->hasNewInformation() ) ) ||

					// Assumptions
					( isSelfGeneratedAssumption &&
					isWritingSelfGeneratedAssumptions ) ||

					// Questions
					( isWritingSelfGeneratedQuestions &&
					currentSpecificationItem->isSelfGeneratedQuestion() ) ||

					// Filter on user-entered specifications
					( ( !isIntegrityCheck ||
					hasMatchingSpecificationCollection ) &&

					( ( isWritingUserSpecifications &&
					currentSpecificationItem->isUserSpecification() &&
					!currentSpecificationItem->isCorrectedAssumption() ) ||

					( isWritingUserQuestions &&
					currentSpecificationItem->isUserQuestion() ) ) ) ) &&

					// Filter on current or updated sentences
					( !isWritingCurrentSentenceOnly ||
					!adminItem_->hasFoundAnyChangeDuringThisSentence() ||	// If nothing has changed, it will result in the notification: "I know".
					currentSpecificationItem->hasNewInformation() ) )
						{
						isForcingResponseNotBeingFirstSpecification = ( isAssignment &&
																		isExclusiveSpecification &&

																		( isIntegrityCheck ||
																		isSelfGenerated ) );

						isWritingCurrentSpecificationWordOnly = ( !isIntegrityCheck &&
																!currentSpecificationItem->isSpecificationGeneralization() &&
																currentSpecificationItem->nContextRelations() > 1 );

						if( writeWordItem->writeSelectedSpecification( false, isForcingResponseNotBeingFirstSpecification, isWritingCurrentSentenceOnly, isWritingCurrentSpecificationWordOnly, NO_ANSWER_PARAMETER, currentSpecificationItem ) == RESULT_OK )
							{
							if( strlen( commonVariables_->writeSentenceString ) > 0 )
								{
								if( isIntegrityCheck )
									{
									if( markWordsPassingGrammarIntegrityCheck() != RESULT_OK )
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to mark the words that are passing the integrity check" );
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
										if( commonVariables_->presentation->writeInterfaceText( true, PRESENTATION_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isWritingSelfGeneratedConclusions ? INTERFACE_LISTING_MY_CONCLUSIONS : ( isWritingSelfGeneratedAssumptions ? INTERFACE_LISTING_MY_ASSUMPTIONS : INTERFACE_LISTING_MY_QUESTIONS ) ) : ( isWritingUserSpecifications ? ( isSelfGenerated ? INTERFACE_LISTING_MY_INFORMATION : INTERFACE_LISTING_YOUR_INFORMATION ) : INTERFACE_LISTING_YOUR_QUESTIONS ) ) ) == RESULT_OK )
											hasHeaderBeenWritten = false;
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write my conclusions header" );
										}

									if( isWritingJustification )
										{
										if( isSelfGenerated )
											{
											if( adminItem_->writeJustificationSpecification( commonVariables_->writeSentenceString, currentSpecificationItem ) != RESULT_OK )
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a justification line" );
											}
										}
									else
										{
										if( commonVariables_->presentation->writeDiacriticalText( PRESENTATION_PROMPT_WRITE, commonVariables_->writeSentenceString ) != RESULT_OK )
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a sentence" );
										}
									}
								}
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a selected specification" );
						}
					}
				while( ( !isIntegrityCheck ||
				!adminItem_->hasPassedGrammarIntegrityCheck() ) &&

				( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given write word item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelfGeneratedSpecification( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedSpecification";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, NULL, writeWordItem ) == RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, NULL, writeWordItem ) == RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, NULL, writeWordItem ) == RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, isWritingJustification, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, NULL, writeWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write self-generated archived assignments" );
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write self-generated past-tense assignments" );
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write self-generated current-tense assignments" );
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write self-generated specifications" );

		return RESULT_OK;
		}

	ResultType writeUserQuestions( bool isWritingCurrentSentenceOnly, char *storeAndRetrieveIntegrityString, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUserQuestions";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) == RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) == RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) == RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, false, false, false, true, false, storeAndRetrieveIntegrityString, writeWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user questions with archived assignments" );
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user questions with past-tense assignments" );
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user questions with current-tense assignments" );
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user questions without assignments" );

		return RESULT_OK;
		}

	ResultType writeUserSpecifications( bool isWritingCurrentSentenceOnly, char *storeAndRetrieveIntegrityString, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeUserSpecifications";

		// Specifications
		if( writeSpecification( false, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) == RESULT_OK )
			{
			// Active assignments
			if( writeSpecification( true, false, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) == RESULT_OK )
				{
				// Inactive assignments
				if( writeSpecification( true, true, false, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) == RESULT_OK )
					{
					// Archived assignments
					if( writeSpecification( true, false, true, isWritingCurrentSentenceOnly, false, true, false, false, false, false, storeAndRetrieveIntegrityString, writeWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user-entered archived assignments" );
					}
				else
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user-entered past-tense assignments" );
				}
			else
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user-entered current-tense assignments" );
			}
		else
			return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user-entered specifications" );

		return RESULT_OK;
		}

	ResultType writeSelectedSpecificationInfo( WordItem *writeWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "isWritingSpecificationInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstSpecificationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					// Respond with active specifications
					if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( false, false, false, false, writeWordItem ) == RESULT_OK )
						{
						// Respond with active specification questions
						if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( false, false, false, true, writeWordItem ) == RESULT_OK )
							{
							// Respond with active assignments
							if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, false, false, writeWordItem ) == RESULT_OK )
								{
								// Respond with active assignment questions
								if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, false, true, writeWordItem ) == RESULT_OK )
									{
									// Respond with inactive assignments
									if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, true, false, false, writeWordItem ) == RESULT_OK )
										{
										// Respond with inactive assignment questions
										if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, true, false, true, writeWordItem ) == RESULT_OK )
											{
											// Respond with archived assignments
											if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, true, false, writeWordItem ) == RESULT_OK )
												{
												// Respond with archive assignment questions
												if( currentGeneralizationWordItem->writeSelectedSpecificationInfo( true, false, true, true, writeWordItem ) != RESULT_OK )
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write archive assignment questions" );
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write archived assignments" );
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write inactive assignment questions" );
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write inactive assignments" );
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active assignment questions" );
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active assignments" );
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active specification questions" );
						}
					else
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active specifications" );
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	ResultType writeSelectedRelationInfo( WordItem *writeWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "isWritingRelatedInfo";

		if( ( currentGeneralizationItem = writeWordItem->firstRelationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL )
					{
					if( currentGeneralizationItem->isRelation() )
						{
						// Respond with active related specifications
						if( currentGeneralizationWordItem->writeSelectedRelationInfo( false, false, false, false, writeWordItem ) == RESULT_OK )
							{
							// Respond with active related specification questions
							if( currentGeneralizationWordItem->writeSelectedRelationInfo( false, false, false, true, writeWordItem ) == RESULT_OK )
								{
								// Respond with active related assignments
								if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, false, false, writeWordItem ) == RESULT_OK )
									{
									// Respond with active related assignment questions
									if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, false, true, writeWordItem ) == RESULT_OK )
										{
										// Respond with inactive related assignments
										if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, true, false, false, writeWordItem ) == RESULT_OK )
											{
											// Respond with inactive related assignment questions
											if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, true, false, true, writeWordItem ) == RESULT_OK )
												{
												// Respond with archive related assignments
												if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, true, false, writeWordItem ) == RESULT_OK )
													{
													// Respond with archive related assignment questions
													if( currentGeneralizationWordItem->writeSelectedRelationInfo( true, false, true, true, writeWordItem ) != RESULT_OK )
														return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write archive related assignment questions" );
													}
												else
													return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write archive related assignment" );
												}
											else
												return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write inactive related assignment questions" );
											}
										else
											return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active related assignments" );
										}
									else
										return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active related assignment assignments" );
									}
								else
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active related assignments" );
								}
							else
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active related specification questions" );
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write active related specifications" );
						}
					}
				else
					return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I found an undefined generalization word" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextRelationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminWriteSpecification( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundAnyWordPassingGrammarIntegrityCheck_ = false;

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminWriteSpecification" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeAdminWriteVariables()
		{
		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;
		isFirstUserQuestion_ = true;
		isFirstUserSpecifications_ = true;
		}

	ResultType answerQuestions()
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "answerQuestions";

		if( commonVariables_->isQuestionAlreadyAnswered )
			{
			if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_QUESTION_IS_ALREADY_ANSWERED ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
			}

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words for an answer
			do	{
				if( currentWordItem->isWordTouchedDuringCurrentSentence )
					{
					if( currentWordItem->findAnswerToNewUserQuestion() != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find an answer to new questions of the user about word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

			if( commonVariables_->isUserQuestion &&
			!commonVariables_->hasFoundAnswerToQuestion &&
			!commonVariables_->isQuestionAlreadyAnswered )
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_QUESTION_I_DONT_KNOW_THE_ANSWER_TO_THIS_QUESTION ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
				}
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );

		return RESULT_OK;
		}

	ResultType checkForStoreAndRetrieveIntegrity( bool isQuestion, char *storeAndRetrieveIntegrityString )
		{
		ReadItem *currentReadItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForStoreAndRetrieveIntegrity";

		hasFoundAnyWordPassingGrammarIntegrityCheck_ = false;

		if( ( currentReadItem = adminItem_->firstActiveReadItem() ) != NULL )
			{
			// Skip selections until writing of selection is implemented
			if( !currentReadItem->isSelection() &&
			// Skip compound questions
			!currentReadItem->isPluralQuestionVerb() &&
			// Skip imperatives until writing of imperatives is implemented
			!currentReadItem->isImperative() )
				{
				adminItem_->checkForChangesDuringThisSentence();

				// Skip known knowledge (no changes)
				if( adminItem_->hasFoundAnyChangeDuringThisSentence() )
					{
					do	{	// Select all generalization words in a sentence
						if( currentReadItem->isGeneralizationWord() )
							{
							if( writeInfoAboutWord( false, !isQuestion, false, false, isQuestion, false, false, false, storeAndRetrieveIntegrityString, currentReadItem->readWordItem() ) != RESULT_OK )
								return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write info about word \"", currentReadItem->readWordItem()->anyWordTypeString(), "\"" );
							}
						}
					while( ( currentReadItem = currentReadItem->nextReadItem() ) != NULL );
					}

				if( currentReadItem == NULL &&		// Sentence isn't skipped (selections, compound questions and imperatives)
				!haveAllWordsPassed() )
					{
					if( adminItem_->isSystemStartingUp() &&
					storeAndRetrieveIntegrityString != NULL )
						{
						if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, EMPTY_STRING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_START, storeAndRetrieveIntegrityString, ( hasFoundAnyWordPassingGrammarIntegrityCheck_ ? INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE_DUE_TO_WORDS : INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_SENTENCE ) ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						{
						if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_STORE_OR_RETRIEVE, EMPTY_STRING, ( hasFoundAnyWordPassingGrammarIntegrityCheck_ ? INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE_DUE_TO_WORDS : INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_THIS_SENTENCE ) ) != RESULT_OK )
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}

					if( hasFoundAnyWordPassingGrammarIntegrityCheck_ )
						{
						if( showWordsThatDidntPass() == RESULT_OK )
							{
							if( strlen( commonVariables_->writeSentenceString ) > 0 )
								{
								if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_WARNING, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_START, commonVariables_->writeSentenceString, INTERFACE_SENTENCE_ERROR_GRAMMAR_INTEGRITY_I_RETRIEVED_FROM_MY_SYSTEM_END ) != RESULT_OK )
									return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface warning" );
								}
							}
						else
							return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to show the words that didn't pass" );
						}

					if( adminItem_->isSystemStartingUp() &&
					commonVariables_->hasShownWarning )
						return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "An integrity error occurred during startup" );
					}
				}
			}

		return RESULT_OK;
		}

	ResultType writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions )
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		isFirstSelfGeneratedAssumption_ = true;
		isFirstSelfGeneratedConclusion_ = true;
		isFirstSelfGeneratedQuestion_ = true;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words for self-generated info
			do	{
				if( currentWordItem->isWordTouchedDuringCurrentSentence )
					{
					if( writeInfoAboutWord( true, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, false, isWritingSelfGeneratedQuestions, false, false, NULL, currentWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write info about word \"", currentWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The first word item is undefined" );

		return RESULT_OK;
		}

	ResultType writeSelfGeneratedInfo( bool isWritingCurrentSentenceOnly, bool isWritingJustification, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		if( isWritingSelfGeneratedConclusions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, true, false, false, writeWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write self-generated conclusions" );
			}

		if( isWritingSelfGeneratedAssumptions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, true, false, writeWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write self-generated assumptions" );
			}

		if( isWritingSelfGeneratedQuestions )
			{
			if( writeSelfGeneratedSpecification( isWritingCurrentSentenceOnly, isWritingJustification, false, false, true, writeWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write self-generated questions" );
			}

		return RESULT_OK;
		}

	ResultType writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, char *storeAndRetrieveIntegrityString, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoAboutWord";

		if( isWritingUserSpecifications )
			{
			if( writeUserSpecifications( isWritingCurrentSentenceOnly, storeAndRetrieveIntegrityString, writeWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user specifications" );
			}

		if( isWritingUserQuestions )
			{
			if( writeUserQuestions( isWritingCurrentSentenceOnly, storeAndRetrieveIntegrityString, writeWordItem ) != RESULT_OK )
				return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write user questions" );
			}

		// Skip integrity check for self-generated info, because it checks only
		// because it only checks needs to check the current user sentence
		if( storeAndRetrieveIntegrityString == NULL )
			{
			if( isWritingSelfGeneratedConclusions ||
			isWritingSelfGeneratedAssumptions ||
			isWritingSelfGeneratedQuestions )
				{
				if( writeSelfGeneratedInfo( isWritingCurrentSentenceOnly, false, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions, writeWordItem ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the self-generated info" );
				}

			if( isWritingSpecificationInfo )
				{
				if( writeSelectedSpecificationInfo( writeWordItem ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write selected specification info" );
				}

			if( isWritingRelatedInfo )
				{
				if( writeSelectedRelationInfo( writeWordItem ) != RESULT_OK )
					return myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write selected related info" );
				}
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"Give thanks to the Lord, for he is good!
 *	His faithful love endures forever." (Psalm 107:1)
 *
 *************************************************************************/
