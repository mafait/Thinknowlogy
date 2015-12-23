/*
 *	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
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

class AdminSpecification
	{
	// Private constructible variables

	private boolean hasAlreadyFoundRelationContext_;
	private boolean isArchivedAssignment_;

	private short doneSpecificationWordOrderNr_;
	private short linkedGeneralizationWordTypeNr_;

	private int userRelationContextNr_;

	private SpecificationItem simpleUserSpecificationItem_;
	private SpecificationItem userSpecificationItem_;

	private WordItem linkedGeneralizationWordItem_;

	private String previousSpecificationString_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private byte addUserSpecificationWithRelation( boolean isAction, boolean isAssignedOrClear, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short selectionLevel, short selectionListNr, short imperativeParameter, short specificationWordParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, ReadItem startRelationReadItem, ReadItem endRelationReadItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		boolean isFirstComparisonPart = ( selectionListNr == Constants.ADMIN_CONDITION_LIST );
		boolean isStop = false;
		short relationWordTypeNr;
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		ReadItem currentRelationReadItem = startRelationReadItem;
		SpecificationItem foundSpecificationItem;
		WordItem mostRecentContextWord;
		WordItem relationWordItem;
		WordItem previousRelationWordItem = null;

		if( startRelationReadItem != null )
			{
			if( endRelationReadItem != null )
				{
				if( generalizationWordItem != null )
					{
					if( specificationWordItem != null )
						{
						do	{
							if( currentRelationReadItem.isPreposition() )
								prepositionParameter = currentRelationReadItem.wordParameter();
							else
								{
								if( currentRelationReadItem.isRelationWord() )
									{
									if( ( relationWordItem = currentRelationReadItem.readWordItem() ) != null )
										{
										if( relationWordItem != generalizationWordItem )
											{
											if( relationWordItem != specificationWordItem )
												{
												relationWordTypeNr = currentRelationReadItem.wordTypeNr();

												if( previousRelationWordItem == null &&
												CommonVariables.nUserRelationWords == 1 &&
												( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != null )
													{
													if( foundSpecificationItem.hasRelationContext() &&
													( mostRecentContextWord = mostRecentContextWordInAllWords( foundSpecificationItem.relationContextNr() ) ) != null )
														{
														if( mostRecentContextWord != relationWordItem )
															previousRelationWordItem = mostRecentContextWord;
														}
													}

												if( previousRelationWordItem != null )
													{
													if( adminItem_.collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
													}

												if( selectionListNr == Constants.NO_LIST_NR )
													{
													if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, null, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ).result != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( ( relationContextNr = relationWordItem.contextNr( specificationWordItem ) ) == Constants.NO_CONTEXT_NR )
														{
														if( ( relationContextNr = adminItem_.highestContextNrInAllWords() ) < Constants.MAX_CONTEXT_NR )
															{
															if( relationWordItem.addContext( false, relationWordTypeNr, specificationWordTypeNr, ++relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
																return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
															}
														else
															return adminItem_.startSystemError( 1, moduleNameString_, "Context number overflow" );
														}

													if( adminItem_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem.isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) == Constants.RESULT_OK )
														isFirstComparisonPart = false;
													else
														return adminItem_.addError( 1, moduleNameString_, "I failed to create a selection part having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
													}

												previousRelationWordItem = relationWordItem;
												}
											else
												return adminItem_.startError( 1, moduleNameString_, "The relation word is the same as the specification word" );
											}
										else
											return adminItem_.startError( 1, moduleNameString_, "The relation word is the same as the generalization word" );
										}
									else
										return adminItem_.startError( 1, moduleNameString_, "I have found an undefined read word" );
									}
								}
							}
						while( !isStop &&
						currentRelationReadItem != endRelationReadItem &&
						( currentRelationReadItem = currentRelationReadItem.nextReadItem() ) != null );
						}
					else
						return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The given end relation read item is undefined" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given start relation read item is undefined" );

		return Constants.RESULT_OK;
		}

	private static WordItem mostRecentContextWordInAllWords( int contextNr )
		{
		ContextItem currentContextItem;
		ContextItem mostRecentContextItem = null;
		WordItem currentWordItem;
		WordItem mostRecentWordItem = null;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( currentContextItem = currentWordItem.contextItem( contextNr ) ) != null )
					{
					if( mostRecentContextItem == null ||
					currentContextItem.isMoreRecent( mostRecentContextItem ) )
						{
						mostRecentWordItem = currentWordItem;
						mostRecentContextItem = currentContextItem;
						}
					}
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return mostRecentWordItem;
		}


	// Constructor / deconstructor

	protected AdminSpecification( AdminItem adminItem )
		{
		String errorString = null;

		hasAlreadyFoundRelationContext_ = false;
		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		linkedGeneralizationWordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = null;
		userSpecificationItem_ = null;

		linkedGeneralizationWordItem_ = null;

		previousSpecificationString_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected assignment methods

	protected byte assignSelectionSpecification( SelectionItem assignmentSelectionItem )
		{
		if( assignmentSelectionItem != null )
			{
			if( assignSpecification( false, assignmentSelectionItem.isAssignedOrClear(), assignmentSelectionItem.isInactiveAssignment(), assignmentSelectionItem.isArchivedAssignment(), assignmentSelectionItem.isNegative(), false, assignmentSelectionItem.isPossessive(), assignmentSelectionItem.isSpecificationGeneralization(), assignmentSelectionItem.isUniqueUserRelation(), assignmentSelectionItem.assumptionLevel(), assignmentSelectionItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, assignmentSelectionItem.relationWordTypeNr(), assignmentSelectionItem.generalizationContextNr(), assignmentSelectionItem.specificationContextNr(), assignmentSelectionItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, assignmentSelectionItem.nContextRelations(), null, assignmentSelectionItem.generalizationWordItem(), assignmentSelectionItem.specificationWordItem(), assignmentSelectionItem.specificationString() ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to assign a specification" );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The given assignment selection item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem != null )
			{
			if( adminItem_.isSystemStartingUp() &&

			( generalizationWordItem.isNeedingAuthorizationForChanges() ||

			( specificationWordItem != null &&
			specificationWordItem.isNeedingAuthorizationForChanges() ) ) )
				{
				if( ( specificationResult = adminItem_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to assign generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + ( specificationWordItem == null ? specificationString : specificationWordItem.anyWordTypeString() ) + "\" with authorization" );
				}
			else
				{
				if( ( specificationResult = generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to assign generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + ( specificationWordItem == null ? specificationString : specificationWordItem.anyWordTypeString() ) + "\"" );
				}
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}


	// Protected specification methods

	protected void initializeLinkedWord()
		{
		linkedGeneralizationWordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;
		linkedGeneralizationWordItem_ = null;
		}

	protected void initializeAdminSpecificationVariables()
		{
		hasAlreadyFoundRelationContext_ = false;
		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = null;
		userSpecificationItem_ = null;

		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordItem_ = null;
		}

	protected byte addUserSpecifications( boolean isAction, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short imperativeParameter, short specificationWordParameter, int generalizationContextNr, int specificationContextNr, ReadItem generalizationReadItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		CollectionResultType collectionResult;
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		boolean isGeneralizationProperName;
		boolean isRelationWord;
		boolean isSpecificationWord;
		boolean hasFoundAction = false;
		boolean hasRelationWord = ( startRelationReadItem != null );
		boolean hasLinkedPossessiveDeterminer = false;
		boolean isExclusiveContext = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelection = ( selectionListNr != Constants.NO_LIST_NR );
		boolean isSkippingThisGeneralizationPart = false;
		boolean isSpecificationWordAlreadyAssignedByComparison = false;
		boolean isValueSpecificationWord = false;
		boolean isWaitingForRelation = false;
		boolean isWaitingForText = false;
		short firstSpecificationWordOrderNr;
		short generalizationWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		short valueGeneralizationWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		short linkedSpecificationWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		short currentSpecificationWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		short previousSpecificationWordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		int nContextRelations = 0;
		ReadItem currentReadItem;
		ReadItem firstRelationReadItem;
		ReadItem lastGeneralizationReadItem;
		WordItem generalizationWordItem;
		WordItem readWordItem;
		WordItem relationWordItem;
		WordItem tempSpecificationWordItem;
		WordItem compoundGeneralizationWordItem = null;
		WordItem currentSpecificationWordItem = null;
		WordItem linkedSpecificationWordItem = null;
		WordItem previousSpecificationWordItem = null;
		WordItem valueGeneralizationWordItem = null;
		String specificationString = null;

		previousSpecificationString_ = null;

		if( generalizationReadItem != null )
			{
			if( ( generalizationWordItem = generalizationReadItem.readWordItem() ) != null )
				{
				isGeneralizationProperName = generalizationReadItem.isProperName();
				generalizationWordTypeNr = generalizationReadItem.wordTypeNr();

				if( ( currentReadItem = startSpecificationReadItem ) != null )
					{
					if( endSpecificationReadItem != null )
						{
						lastGeneralizationReadItem = generalizationReadItem.nextReadItem();

						while( lastGeneralizationReadItem != null &&
						!lastGeneralizationReadItem.isGeneralizationWord() )
							{
							if( lastGeneralizationReadItem.isPossessiveDeterminer() )
								hasLinkedPossessiveDeterminer = true;

							lastGeneralizationReadItem = lastGeneralizationReadItem.nextReadItem();
							}

						firstSpecificationWordOrderNr = ( lastGeneralizationReadItem == null ? endSpecificationReadItem.wordOrderNr() : (short)( lastGeneralizationReadItem.wordOrderNr() + 1 ) );

						do	{
							isRelationWord = currentReadItem.isRelationWord();
							isSpecificationWord = currentReadItem.isSpecificationWord();

							if( isSkippingThisGeneralizationPart )
								{
								if( isWaitingForRelation )
									{
									if( isRelationWord ||
									currentReadItem.isText() )
										{
										isSkippingThisGeneralizationPart = false;
										isWaitingForRelation = false;
										isWaitingForText = false;
										}
									}
								else
									{
									if( generalizationReadItem.wordOrderNr() < currentReadItem.wordOrderNr() )
										isSkippingThisGeneralizationPart = false;
									}
								}
							else
								{
								if( doneSpecificationWordOrderNr_ > currentReadItem.wordOrderNr() )
									isSkippingThisGeneralizationPart = true;
								else
									{
									if( currentReadItem.isGeneralizationWord() )
										{
										isSkippingThisGeneralizationPart = true;
										isWaitingForRelation = true;
										}
									else
										{
										if( isSpecificationWord &&
										generalizationWordItem.isAdjectiveComparison() &&
										( tempSpecificationWordItem = currentReadItem.readWordItem() ) != null )
											{
											// Skip head and tail in the comparison
											if( !tempSpecificationWordItem.isNounHead() &&
											!tempSpecificationWordItem.isNounTail() )
												{
												isWaitingForText = true;
												isSpecificationWordAlreadyAssignedByComparison = true;
												currentSpecificationWordTypeNr = currentReadItem.wordTypeNr();
												currentSpecificationWordItem = tempSpecificationWordItem;
												}
											}
										}
									}
								}

							if( !isWaitingForText &&
							!isSkippingThisGeneralizationPart &&
							!currentReadItem.isNegative() &&
							// Skip numeral 'both'. Typically for English: in both ... and ...
							!currentReadItem.isNumeralBoth() )
								{
								readWordItem = currentReadItem.readWordItem();
								specificationString = null;

								if( currentReadItem.isText() )
									specificationString = currentReadItem.readString;

								if( isSpecificationWordAlreadyAssignedByComparison )
									isSpecificationWordAlreadyAssignedByComparison = false;
								else
									{
									currentSpecificationWordTypeNr = ( isQuestion && currentReadItem.isSingularOrPluralNounWordType() ? Constants.WORD_TYPE_NOUN_SINGULAR : currentReadItem.wordTypeNr() );
									currentSpecificationWordItem = readWordItem;
									}

								if( isPossessive &&
								currentReadItem.isNumeral() )
									nContextRelations = Integer.parseInt( currentReadItem.readWordTypeString() );
								else
									{
									if( isSpecificationWord &&
									startRelationReadItem != null &&
									!generalizationWordItem.isVerbImperativeLoginOrReadOrShow() )
										{
										if( ( contextResult = adminItem_.getRelationContext( isArchivedAssignment, isNegative, isPossessive, isQuestion, true, currentSpecificationWordTypeNr, nContextRelations, generalizationWordItem, currentSpecificationWordItem, null, startRelationReadItem ) ).result == Constants.RESULT_OK )
											{
											hasAlreadyFoundRelationContext_ = true;
											isExclusiveContext = contextResult.isExclusiveContext;
											userRelationContextNr_ = contextResult.contextNr;
											}
										else
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to get the multiple entry relation context" );
										}

									if( currentReadItem.isNumeral() ||
									currentReadItem.isText() ||

									( isSpecificationWord &&
									!currentReadItem.isPreposition() ) )
										{
										if( isValueSpecificationWord )
											{
											if( isSelection )
												{
												if( adminItem_.createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a value selection item" );
												}

											if( !hasRelationWord )
												{
												// Value, but no relation
												if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "Adding a value specification with relation isn't implemented" );
											}
										else
											{
											if( currentReadItem.isNounValue() )
												{
												isValueSpecificationWord = true;
												valueGeneralizationWordTypeNr = generalizationWordTypeNr;
												valueGeneralizationWordItem = generalizationWordItem;
												}
											else
												{
												if( selectionListNr == Constants.NO_LIST_NR )
													{
													if( previousSpecificationString_ == null &&
													previousSpecificationWordItem != null &&
													currentSpecificationWordItem != null &&
													currentReadItem.isMatchingReadWordTypeNr( previousSpecificationWordTypeNr ) )
														{
														if( ( collectionResult = adminItem_.collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) ).result == Constants.RESULT_OK )
															{
															if( collectionResult.isAmbiguousCollection )
																{
																if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, null, generalizationWordItem, previousSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
																	return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification with an ambiguous specification word to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to collect specification words" );
														}

													if( currentSpecificationWordTypeNr != Constants.WORD_TYPE_PREPOSITION )
														{
														previousSpecificationWordItem = currentSpecificationWordItem;
														previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
														previousSpecificationString_ = specificationString;
														}
													}
												else
													{
													// Create selection
													if( hasRelationWord )
														{
														// Selection, no value, with relation
														if( addUserSpecificationWithRelation( isAction, currentReadItem.isAdjectiveAssignedOrClear(), isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isExclusiveSpecification || isExclusiveContext ), isNegative, isNewStart, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, userAssumptionLevel, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem, specificationString ) != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a selection part with relation" );
														}
													else
														{
														// Selection, no value, no relation
														if( adminItem_.createSelectionPart( isAction, currentReadItem.isAdjectiveAssignedOrClear(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, generalizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a selection part" );
														}
													}
												}

											if( !currentReadItem.isAdjectiveAssignedOrClear() )
												{
												doneSpecificationWordOrderNr_ = currentReadItem.wordOrderNr();
												linkedSpecificationWordTypeNr = currentSpecificationWordTypeNr;
												linkedSpecificationWordItem = currentSpecificationWordItem;

												if( !currentReadItem.isNounValue() &&
												currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADVERB &&
												currentReadItem.wordOrderNr() <= firstSpecificationWordOrderNr )
													{
													// Login is already created by during startup
													if( generalizationWordItem.isVerbImperativeLoginOrReadOrShow() )
														hasFoundAction = true;
													else
														{
														if( hasRelationWord )
															{
															// No value, with relation
															if( addUserSpecificationWithRelation( false, false, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isExclusiveSpecification || isExclusiveContext ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, imperativeParameter, specificationWordParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem, specificationString ) == Constants.RESULT_OK )
																hasFoundAction = true;
															else
																return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a user specification with a relation to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														else
															{
															// No value, no relation
															if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, null, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result == Constants.RESULT_OK )
																hasFoundAction = true;
															else
																return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}

														if( !isNegative &&
														!isPartOf &&
														!isQuestion &&
														currentSpecificationWordItem != null &&
														adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) &&
														adminItem_.isSingularOrPluralNoun( currentSpecificationWordTypeNr ) )
															{
															// Generalizations with noun specifications - without relations
															if( !isPossessive )
																{
																if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, null ) == Constants.RESULT_OK )
																	{
																	if( !CommonVariables.hasShownWarning &&
																	// Definition specification
																	adminItem_.isSingularOrPluralNoun( generalizationWordTypeNr ) )
																		{
																		// Definition specification
																		if( ( specificationResult = adminItem_.drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result == Constants.RESULT_OK )
																			compoundGeneralizationWordItem = specificationResult.compoundGeneralizationWordItem;
																		else
																			return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a primary specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	}
																else
																	return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}

															if( !CommonVariables.hasShownWarning &&
															isGeneralizationProperName &&
															CommonVariables.nUserRelationWords == 1 &&
															startRelationReadItem != null &&
															( firstRelationReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null )
																{
																if( ( relationWordItem = firstRelationReadItem.readWordItem() ) != null )
																	{
																	if( adminItem_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, firstRelationReadItem.wordTypeNr(), specificationContextNr, userRelationContextNr_, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
																		return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with one relation word, from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															}
														}
													}
												}
											}
										}
									else
										{
										if( isPossessive &&
										currentReadItem.isArticle() )
											nContextRelations = 0;
										}

									if( !CommonVariables.hasShownWarning &&
									currentSpecificationWordItem != null )
										{
										if( currentReadItem.isVerb() )
											hasFoundAction = true;

										if( !isQuestion &&
										linkedSpecificationWordItem != null &&

										( isRelationWord ||
										isSpecificationWord ) &&

										( !hasRelationWord ||
										linkedSpecificationWordItem != currentSpecificationWordItem ) )
											{
											if( isGeneralizationProperName &&
											!isValueSpecificationWord &&
											// Skip adjectives
											adminItem_.isSingularOrPluralNoun( linkedSpecificationWordTypeNr ) )
												{
												if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationWord ? currentSpecificationWordItem : null ) ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
												}

											if( isRelationWord ||

											( isSpecificationWord &&
											hasLinkedPossessiveDeterminer ) )
												{
												if( !CommonVariables.hasShownWarning &&
												// Linked specification
												currentSpecificationWordItem == linkedSpecificationWordItem )
													{
													if( linkedGeneralizationWordItem_ == null )
														{
														linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
														linkedGeneralizationWordItem_ = generalizationWordItem;
														}
													else
														return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "Linked word \"" + linkedGeneralizationWordItem_.anyWordTypeString() + "\" is already assigned" );
													}

												if( !CommonVariables.hasShownWarning &&
												!isQuestion &&
												!isSpecificationGeneralization &&
												generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME &&
												userRelationContextNr_ > Constants.NO_CONTEXT_NR &&
												CommonVariables.nUserRelationWords > 1 )
													{
													if( adminItem_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, userRelationContextNr_, generalizationWordItem, linkedSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with multiple relation words, from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( selectionListNr == Constants.NO_LIST_NR &&
												currentReadItem.isAdjectiveAssigned() )
													{
													if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem.readWordTypeString(), Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, currentReadItem.wordOrderNr(), Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != Constants.RESULT_OK )
														return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
													}
												}
											}
										}
									}
								}
							}
						while( !CommonVariables.hasShownWarning &&
						currentReadItem != endSpecificationReadItem &&
						( currentReadItem = currentReadItem.nextReadItem() ) != null );

						if( hasFoundAction )
							{
							if( isGeneralizationProperName )
								{
								if( adminItem_.recalculateAssumptionsInAllTouchedWords() != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumptions in all words" );
								}
							}
						else
							return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I couldn't find anything to do" );
						}
					else
						return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given end specification read item is undefined" );
					}
				else
					return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given start specification read item is undefined" );
				}
			else
				return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given generalization read item has no read word" );
			}
		else
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given generalization read item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		ContextResultType contextResult = new ContextResultType();
		SpecificationResultType assignmentResult = new SpecificationResultType();
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isCompoundCollectionCollectedWithItself = false;
		boolean isDefinitionSpecification;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationContextAlreadyDefined = false;
		boolean isSelfGenerated = ( firstJustificationItem != null );
		SpecificationItem foundSpecificationItem;
		SpecificationItem foundAssignmentItem;
		SpecificationItem replacedAssignmentItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem createdAssignmentItem = null;
		SpecificationItem createdSpecificationItem = null;
		SpecificationItem existingSpecificationItem = null;

		if( generalizationWordItem != null )
			{
			if( generalizationWordItem != adminItem_ )
				{
				if( specificationWordItem != adminItem_ )
					{
					if( specificationWordItem != null &&
					relationWordItem != null &&
					!relationWordItem.isNumeralWordType() )
						{
						if( isPossessive &&
						specificationCollectionNr > Constants.NO_COLLECTION_NR &&
						specificationWordItem.isNounWordCollectedWithItself() &&
						specificationWordItem.isCompoundCollection( specificationCollectionNr ) )
							isCompoundCollectionCollectedWithItself = true;

						if( isPossessive &&
						!isSelfGenerated &&
						nContextRelations == 0 &&
						relationContextNr > Constants.NO_CONTEXT_NR )
							existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem );

						if( isArchivedAssignment ||
						isCompoundCollectionCollectedWithItself ||
						relationContextNr == Constants.NO_CONTEXT_NR ||

						( isPossessive &&
						!isSelfGenerated &&

						( ( nContextRelations == 0 &&
						existingSpecificationItem == null ) ||

						( !hasAlreadyFoundRelationContext_ &&
						existingSpecificationItem != null &&
						existingSpecificationItem.isUserSpecification() ) ) ) )
							{
							if( ( contextResult = adminItem_.getSpecificationRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionCollectedWithItself, isNegative, isPossessive, ( firstJustificationItem != null && firstJustificationItem.isAssumptionJustification() ), specificationCollectionNr, userRelationContextNr_, generalizationWordItem, specificationWordItem, relationWordItem ) ).result == Constants.RESULT_OK )
								relationContextNr = contextResult.contextNr;
							else
								adminItem_.addError( 1, moduleNameString_, "I failed to get a relation context" );
							}
						else
							isRelationContextAlreadyDefined = true;

						if( CommonVariables.result == Constants.RESULT_OK &&
						!isSelection )
							{
							if( relationWordItem.addContext( isCompoundCollectionCollectedWithItself, relationWordTypeNr, specificationWordTypeNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
							}
						}

					if( CommonVariables.result == Constants.RESULT_OK )
						{
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

						// Skip adding specification if already exists
						if( isAssignment ||
						!isRelationContextAlreadyDefined ||
						existingSpecificationItem == null ||
						!existingSpecificationItem.isActiveAssignment() )
							{
							// Check for an assignment or a specification with opposite negative indicator
							if( !isAssignment &&
							isArchivedAssignment_ &&
							generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, !isNegative, isPossessive, questionParameter, specificationWordItem ) != null )
								isAssignment = true;

							if( adminItem_.isSystemStartingUp() &&

							( generalizationWordItem.isNeedingAuthorizationForChanges() ||

							( specificationWordItem != null &&
							specificationWordItem.isNeedingAuthorizationForChanges() ) ) )
								{
								if( ( specificationResult = adminItem_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
									adminItem_.addError( 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with authorization" );
								}
							else
								{
								if( ( specificationResult = generalizationWordItem.addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
									adminItem_.addError( 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}

							if( CommonVariables.result == Constants.RESULT_OK &&
							!CommonVariables.hasShownWarning )
								{
								isArchivedAssignment_ = isArchivedAssignment;
								replacedAssignmentItem = specificationResult.replacedAssignmentItem;

								isDefinitionSpecification = ( relationWordItem == null &&
															adminItem_.isSingularOrPluralNoun( generalizationWordTypeNr ) &&
															adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) );

								if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) == null )
									{
									if( hasRelationWord &&
									!isSelfGenerated &&
									( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
										{
										userSpecificationItem_ = foundSpecificationItem;

										if( !isQuestion &&
										foundSpecificationItem.hasRelationContext() )
											simpleUserSpecificationItem_ = generalizationWordItem.firstSpecificationItem( isPossessive, false, Constants.NO_QUESTION_PARAMETER, specificationWordItem );
										}
									}
								else
									{
									if( !isSelfGenerated )
										{
										userSpecificationItem_ = createdSpecificationItem;

										if( !isQuestion )
											simpleUserSpecificationItem_ = createdSpecificationItem;
										}
									}

								if( !isExclusiveSpecification &&
								!isQuestion &&
								!isSelfGenerated &&
								specificationWordItem != null )
									{
									if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME )
										{
										if( !isSpecificationGeneralization &&
										createdSpecificationItem != null )
											{
											if( isNegative )
												{
												if( createdSpecificationItem.hasSpecificationCollection() )
													{
													if( adminItem_.makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
														adminItem_.addError( 1, moduleNameString_, "I failed to make an only-option-left assumption" );
													}
												}
											else
												{
												if( adminItem_.collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem ) == Constants.RESULT_OK )
													{
													if( !isPossessive )
														{
														if( adminItem_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) == Constants.RESULT_OK )
															{
															if( generalizationWordItem.hasFoundAnswerToQuestion() &&
															!specificationWordItem.isCompoundCollection( createdSpecificationItem.specificationCollectionNr() ) )
																{
																if( adminItem_.drawNegativeConclusionsFromAnsweredQuestions( createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
																	adminItem_.addError( 1, moduleNameString_, "I failed to draw negative conclusions from answered questions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														else
															adminItem_.addError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													}
												else
													adminItem_.addError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );
												}
											}
										}
									else
										{
										if( isDefinitionSpecification )
											{
											if( isSpecificationGeneralization )
												{
												if( isPartOf &&
												!isPossessive &&

												( !isAssignment ||
												isArchivedAssignment ) )
													{
													// Draw a possessive definition conclusion from a specification-generalization 'part of' sentence
													// Not yet explained in the design
													if( adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, false, false, isNegative, false, true, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
														adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( createdSpecificationItem == null )
													{
													if( !isConditional &&
													!isNegative )
														{
														// Check if primary specification already has an assignment
														foundAssignmentItem = generalizationWordItem.firstAssignmentItem( true, true, true, isPossessive, questionParameter, relationContextNr, specificationWordItem );

														if( isAssignment ||
														foundAssignmentItem != null )
															{
															if( generalizationWordItem.writeSelectedSpecification( true, ( foundAssignmentItem == null ? specificationResult.foundSpecificationItem : foundAssignmentItem ) ) == Constants.RESULT_OK )
																{
																if( CommonVariables.writeSentenceStringBuffer != null &&
																CommonVariables.writeSentenceStringBuffer.length() > 0 )
																	{
																	if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( foundAssignmentItem == null ? Constants.INTERFACE_LISTING_SENTENCE_ASSIGNMENT_CONFLICTS_WITH_DEFINITION_SPECIFICATION : Constants.INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) == Constants.RESULT_OK )
																		{
																		if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
																			adminItem_.startError( 1, moduleNameString_, "I failed to write a sentence" );
																		}
																	else
																		adminItem_.startError( 1, moduleNameString_, "I failed to write a header" );
																	}
																else
																	adminItem_.startError( 1, moduleNameString_, "Integrity error! I couldn't write the assignment with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
																}
															else
																adminItem_.addError( 1, moduleNameString_, "I failed to write a non-proper-name specification of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												else
													{
													if( isPossessive &&

													( !isAssignment ||
													isArchivedAssignment ) )
														{
														// Draw a specification-generalization 'part of' conclusion from a possessive definition sentence
														// Not yet explained in the design
														if( adminItem_.addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, false, false, isNegative, true, false, false, false, true, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
															adminItem_.addError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										}
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								!CommonVariables.hasShownWarning &&
								// Skip for example imperative verbs
								adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) &&

								( ( isAssignment &&
								// At this stage, selections must be stored - rather than executed. So, don't assign them
								!isSelection ) ||

								replacedAssignmentItem != null ) )
									{
									if( ( assignmentResult = assignSpecification( contextResult.isAmbiguousRelationContext, ( specificationWordItem != null && specificationWordItem.isAdjectiveAssignedOrClear() ), isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result == Constants.RESULT_OK )
										{
										if( ( createdAssignmentItem = assignmentResult.createdSpecificationItem ) != null )
											{
											if( !isArchivedAssignment_ ||
											specificationResult.createdSpecificationItem != null )
												specificationResult.createdSpecificationItem = createdAssignmentItem;

											if( replacedAssignmentItem != null )
												{
												if( generalizationWordItem.replaceOrDeleteSpecification( replacedAssignmentItem, createdAssignmentItem ) != Constants.RESULT_OK )
													adminItem_.addError( 1, moduleNameString_, "I failed to replace or delete an assignment in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}

											if( CommonVariables.result == Constants.RESULT_OK &&
											isNegative &&
											!isQuestion &&
											createdAssignmentItem.hasSpecificationCollection() &&
											generalizationWordItem.isUserGeneralizationWord )
												{
												if( adminItem_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem.specificationCollectionNr(), generalizationWordItem ) != Constants.RESULT_OK )
													adminItem_.addError( 1, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
												}
											}

										if( CommonVariables.result == Constants.RESULT_OK &&
										!isSelfGenerated &&
										( tempSpecificationItem = ( createdAssignmentItem == null ? assignmentResult.foundSpecificationItem : createdAssignmentItem ) ) != null )
											{
											userSpecificationItem_ = tempSpecificationItem;

											if( isArchivedAssignment_ &&
											!isQuestion )
												simpleUserSpecificationItem_ = tempSpecificationItem;
											}
										}
									else
										adminItem_.addError( 1, moduleNameString_, "I failed to assign the specification" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								!CommonVariables.hasShownWarning &&
								!isExclusiveSpecification &&
								!isSelfGenerated &&
								specificationWordItem != null )
									{
									if( isQuestion )
										{
										if( hasRelationWord &&

										( createdAssignmentItem != null ||

										( createdSpecificationItem != null &&
										!generalizationWordItem.hasShownMoreSpecificRelatedQuestion() ) ) )
											{
											if( adminItem_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, ( createdAssignmentItem == null ? createdSpecificationItem : createdAssignmentItem ), generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
												adminItem_.addError( 1, moduleNameString_, "I failed to make a suggestive question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										}
									else
										{
										if( isDefinitionSpecification &&
										!isSpecificationGeneralization &&
										userSpecificationItem_ != null )
											{
											if( adminItem_.drawSpecificationGeneralizationConclusion( isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
												adminItem_.addError( 1, moduleNameString_, "I failed to draw a specification generalization conclusion" );
											}
										}
									}
								}
							}
						}
					}
				else
					adminItem_.startError( 1, moduleNameString_, "The given specification word is the administrator" );
				}
			else
				adminItem_.startError( 1, moduleNameString_, "The given generalization word is the administrator" );
			}
		else
			adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationItem simpleUserSpecificationItem()
		{
		return simpleUserSpecificationItem_;
		}

	protected SpecificationItem userSpecificationItem()
		{
		return userSpecificationItem_;
		}
	};

/*************************************************************************
 *	"How amazing are the deeds of the Lord!
 *	All who delight in him should ponder them." (Psalm 111:2)
 *************************************************************************/
