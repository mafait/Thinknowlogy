/*
 *	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
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

class AdminSpecification
	{
	// Private constructible variables

	private boolean isArchivedAssignment_;
	private boolean isFirstPartOfUserSentence_;

	private short doneSpecificationWordOrderNr_;
	private short linkedGeneralizationWordTypeNr_;

	private int missingRelationContextNr_;
	private int mostRecentRelationContextNr_;
	private int userRelationContextNr_;

	private SpecificationItem userSpecificationItem_;

	private WordItem linkedGeneralizationWordItem_;

	private String previousSpecificationString_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte addUserSpecificationWithRelation( boolean isAction, boolean isAssignedOrClear, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, short selectionLevel, short selectionListNr, short imperativeParameter, short specificationWordParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, ReadItem startRelationReadItem, ReadItem endRelationReadItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		boolean isFirstComparisonPart = ( selectionListNr == Constants.ADMIN_CONDITION_LIST );
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
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
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
													}

												if( selectionListNr == Constants.NO_LIST_NR )
													{
													if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, null, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ).result != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a specification having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
													}
												else
													{
													if( ( relationContextNr = relationWordItem.contextNrInWord( specificationWordItem ) ) == Constants.NO_CONTEXT_NR )
														{
														if( ( relationContextNr = adminItem_.highestContextNr() ) < Constants.MAX_CONTEXT_NR )
															{
															if( relationWordItem.addContext( isQuestion, relationWordTypeNr, specificationWordTypeNr, ++relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
															}
														else
															return myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
														}

													if( adminItem_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem.isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) == Constants.RESULT_OK )
														isFirstComparisonPart = false;
													else
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a selection part having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
													}

												previousRelationWordItem = relationWordItem;
												}
											else
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "The relation word is the same as the specification word" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "The relation word is the same as the generalization word" );
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an undefined read word" );
									}
								}
							}
						while( !isStop &&
						currentRelationReadItem != endRelationReadItem &&
						( currentRelationReadItem = currentRelationReadItem.nextReadItem() ) != null );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given end relation read item is undefined" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given start relation read item is undefined" );

		return Constants.RESULT_OK;
		}

	private SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, boolean isValueSpecification, short userAssumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem != null )
			{
			if( adminItem_.isSystemStartingUp() &&

			( generalizationWordItem.isNeedingAuthorizationForChanges() ||

			( specificationWordItem != null &&
			specificationWordItem.isNeedingAuthorizationForChanges() ) ) )
				return specificationResult = adminItem_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, mostRecentRelationContextNr_, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			if( !generalizationWordItem.isVerbImperativeLogin() )	// Already created by during startup
				return generalizationWordItem.addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, adminItem_.isUserSentenceMixOfFeminineAndMasculineSpecificationWords(), isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, mostRecentRelationContextNr_, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, null );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	private static WordItem mostRecentContextWordInAllWords( int contextNr )
		{
		ContextItem currentContextItem;
		ContextItem mostRecentContextItem = null;
		WordItem currentWordItem;
		WordItem mostRecentWordItem = null;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( currentContextItem = currentWordItem.contextItemInWord( contextNr ) ) != null )
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

	protected AdminSpecification( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		isArchivedAssignment_ = false;
		isFirstPartOfUserSentence_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		linkedGeneralizationWordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

		missingRelationContextNr_ = Constants.NO_CONTEXT_NR;
		mostRecentRelationContextNr_ = Constants.NO_CONTEXT_NR;
		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		userSpecificationItem_ = null;

		linkedGeneralizationWordItem_ = null;

		previousSpecificationString_ = null;

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


	// Protected assignment methods

	protected byte assignSelectionSpecification( SelectionItem assignmentSelectionItem )
		{
		if( assignmentSelectionItem != null )
			{
			if( assignSpecification( false, assignmentSelectionItem.isAssignedOrClear(), assignmentSelectionItem.isInactiveAssignment(), assignmentSelectionItem.isArchivedAssignment(), assignmentSelectionItem.isNegative(), false, assignmentSelectionItem.isPossessive(), assignmentSelectionItem.isSpecificationGeneralization(), assignmentSelectionItem.isUniqueRelation(), assignmentSelectionItem.assumptionLevel(), assignmentSelectionItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, assignmentSelectionItem.relationWordTypeNr(), assignmentSelectionItem.generalizationContextNr(), assignmentSelectionItem.specificationContextNr(), assignmentSelectionItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, assignmentSelectionItem.nContextRelations(), null, assignmentSelectionItem.generalizationWordItem(), assignmentSelectionItem.specificationWordItem(), assignmentSelectionItem.specificationString() ).result != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign a specification" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given assignment selection item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem != null )
			{
			if( adminItem_.isSystemStartingUp() &&

			( generalizationWordItem.isNeedingAuthorizationForChanges() ||

			( specificationWordItem != null &&
			specificationWordItem.isNeedingAuthorizationForChanges() ) ) )
				{
				if( ( specificationResult = adminItem_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + ( specificationWordItem == null ? specificationString : specificationWordItem.anyWordTypeString() ) + "\" with authorization" );
				}
			else
				{
				if( ( specificationResult = generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
					myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + ( specificationWordItem == null ? specificationString : specificationWordItem.anyWordTypeString() ) + "\"" );
				}
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

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
		isArchivedAssignment_ = false;
		isFirstPartOfUserSentence_ = true;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

		missingRelationContextNr_ = Constants.NO_CONTEXT_NR;
		mostRecentRelationContextNr_ = Constants.NO_CONTEXT_NR;
		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		userSpecificationItem_ = null;

		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordItem_ = null;
		}

	protected byte addUserSpecifications( boolean isAction, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, short userAssumptionLevel, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short imperativeParameter, short specificationWordParameter, int generalizationContextNr, int specificationContextNr, ReadItem generalizationReadItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		CollectionResultType collectionResult;
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		boolean isGeneralizationProperName;
		boolean isRelationWord;
		boolean isSpecificationWord;
		boolean hasFoundAction = false;
		boolean hasRelationContext = ( startRelationReadItem != null );
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
		WordItem currentWordItem;
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
							!currentReadItem.isNumeralBoth() )		// Skip numeral 'both'. Typically for English: in both ... and ...
								{
								readWordItem = currentReadItem.readWordItem();
								specificationString = null;

								if( currentReadItem.isText() )
									specificationString = currentReadItem.readString;

								if( isSpecificationWordAlreadyAssignedByComparison )
									isSpecificationWordAlreadyAssignedByComparison = false;
								else
									{
									currentSpecificationWordTypeNr = ( isQuestion && currentReadItem.isNoun() ? Constants.WORD_TYPE_NOUN_SINGULAR : currentReadItem.wordTypeNr() );
									currentSpecificationWordItem = readWordItem;
									}

								if( isPossessive &&
								currentReadItem.isNumeral() )
									nContextRelations = Integer.parseInt( currentReadItem.readWordTypeString() );
								else
									{
									if( isFirstPartOfUserSentence_ &&
									isSpecificationWord &&
									startRelationReadItem != null &&
									!generalizationWordItem.isVerbImperativeReadOrShow() )
										{
										if( ( contextResult = adminItem_.getRelationContext( isAssignment, isExclusiveSpecification, isNegative, isPossessive, isQuestion, true, nContextRelations, generalizationWordItem, currentSpecificationWordItem, null, startRelationReadItem ) ).result == Constants.RESULT_OK )
											{
											hasFoundAction = true;
											isExclusiveContext = contextResult.isExclusiveContext;
											userRelationContextNr_ = contextResult.contextNr;
											}
										else
											return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to get the relation context" );
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
												if( adminItem_.createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to create a value selection item" );
												}

											if( !hasRelationContext )
												{
												// Value, but no relation
												if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
												}
											else
												return myWordItem_.startErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "Adding a value specification with relation isn't implemented" );
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
													if( previousSpecificationString_ == null )
														{
														if( previousSpecificationWordItem != null &&
														currentSpecificationWordItem != null &&
														currentReadItem.isMatchingWordTypeNr( previousSpecificationWordTypeNr ) )
															{
															if( ( collectionResult = adminItem_.collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) ).result == Constants.RESULT_OK )
																{
																if( collectionResult.isAmbiguousCollection )
																	{
																	if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, null, generalizationWordItem, previousSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
																		return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to add a specification with an ambiguous specification word to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to collect specification words" );
															}
														}
													else
														{
														if( adminItem_.collectSpecificationStrings( ( isExclusiveSpecification || isPossessive ), generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationWordItem, previousSpecificationString_, specificationString ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to collect specification strings" );
														}

													if( currentSpecificationWordTypeNr != Constants.WORD_TYPE_PREPOSITION )
														{
														previousSpecificationWordItem = currentSpecificationWordItem;
														previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
														previousSpecificationString_ = specificationString;
														}
													}
												else	// Create selection
													{
													if( hasRelationContext )
														{
														// Selection, no value, with relation
														if( addUserSpecificationWithRelation( isAction, currentReadItem.isAdjectiveAssignedOrClear(), isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isExclusiveSpecification || isExclusiveContext ), isNegative, isNewStart, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, userAssumptionLevel, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem, specificationString ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to create a selection part with relation" );
														}
													else
														{
														// Selection, no value, no relation
														if( adminItem_.createSelectionPart( isAction, currentReadItem.isAdjectiveAssignedOrClear(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, generalizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to create a selection part" );
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
													if( generalizationWordItem.isVerbImperativeReadOrShow() )
														hasFoundAction = true;
													else
														{
														if( hasRelationContext )
															{
															// No value, with relation
															if( addUserSpecificationWithRelation( false, false, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isExclusiveSpecification || isExclusiveContext ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, imperativeParameter, specificationWordParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem, specificationString ) == Constants.RESULT_OK )
																hasFoundAction = true;
															else
																return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to add a user specification with a relation to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														else
															{
															// No value, no relation
															if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, null, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result == Constants.RESULT_OK )
																hasFoundAction = true;
															else
																return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}

														if( !isQuestion &&
														currentSpecificationWordItem != null &&
														adminItem_.isReasoningWordType( generalizationWordTypeNr ) &&
														adminItem_.isSingularOrPluralNoun( currentSpecificationWordTypeNr ) )
															{
															// Generalizations with noun specifications - without relations
															if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, null ) == Constants.RESULT_OK )
																{
																if( !CommonVariables.hasShownWarning )
																	{
																	if( adminItem_.isSingularOrPluralNoun( generalizationWordTypeNr ) )	// Definition specification
																		{
																		// Definition specification
																		if( ( specificationResult = adminItem_.drawCompoundSpecificationSubstitutionConclusion( isNegative, isPartOf, isPossessive, generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result == Constants.RESULT_OK )
																			compoundGeneralizationWordItem = specificationResult.compoundGeneralizationWordItem;
																		else
																			return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to draw a primary specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																		}
																	else
																		{
																		if( isGeneralizationProperName &&
																		CommonVariables.nUserRelationWords == 1 &&
																		startRelationReadItem != null &&
																		( firstRelationReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null )
																			{
																			if( ( relationWordItem = firstRelationReadItem.readWordItem() ) != null )
																				{
																				if( adminItem_.drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueRelation, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, firstRelationReadItem.wordTypeNr(), specificationContextNr, userRelationContextNr_, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
																					return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with one relation word, from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
																				}
																			}
																		}
																	}
																}
															else
																return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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

										( !hasRelationContext ||
										linkedSpecificationWordItem != currentSpecificationWordItem ) )
											{
											if( isGeneralizationProperName &&
											!isValueSpecificationWord &&
											// Skip adjectives
											adminItem_.isSingularOrPluralNoun( linkedSpecificationWordTypeNr ) )
												{
												if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationContext ? currentSpecificationWordItem : null ) ) != Constants.RESULT_OK )
													return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
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
														return myWordItem_.startErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "Linked word \"" + linkedGeneralizationWordItem_.anyWordTypeString() + "\" is already assigned" );
													}

												if( !CommonVariables.hasShownWarning &&
												!isQuestion &&
												userRelationContextNr_ > Constants.NO_CONTEXT_NR &&
												CommonVariables.nUserRelationWords > 1 )
													{
													if( adminItem_.drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueRelation, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, userRelationContextNr_, generalizationWordItem, linkedSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with multiple relation words, from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
													}
												}
											else
												{
												if( selectionListNr == Constants.NO_LIST_NR &&
												currentReadItem.isAdjectiveAssigned() )
													{
													if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem.readWordTypeString(), Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, currentReadItem.wordOrderNr(), Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != Constants.RESULT_OK )
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface warning" );
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
							isFirstPartOfUserSentence_ = false;

							if( isGeneralizationProperName &&
							( currentWordItem = CommonVariables.firstWordItem ) != null )
								{
								// Do for all words
								do	{
									if( currentWordItem.isWordTouchedDuringCurrentSentence )
										{
										// Recalculate the generalization word(s)
										if( currentWordItem.recalculateAssumptions() != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"" + currentWordItem.anyWordTypeString() + "\"" );
										}
									}
								while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
								}
							}
						else
							return myWordItem_.startErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "I couldn't find anything to do" );
						}
					else
						return myWordItem_.startErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "The given end specification read item is undefined" );
					}
				else
					return myWordItem_.startErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "The given start specification read item is undefined" );
				}
			else
				return myWordItem_.startErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "The given generalization read item has no read word" );
			}
		else
			return myWordItem_.startErrorInItem( adminItem_.adminListChar( selectionListNr ), 1, moduleNameString_, "The given generalization read item is undefined" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, boolean isValueSpecification, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		ContextResultType contextResult = new ContextResultType();
		SpecificationResultType assignmentResult = new SpecificationResultType();
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isDefinitionSpecification = ( relationWordItem == null &&
											adminItem_.isSingularOrPluralNoun( generalizationWordTypeNr ) &&
											adminItem_.isSingularOrPluralNoun( specificationWordTypeNr ) );
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationContextAlreadyDefined = false;
		boolean isSelfGenerated = ( firstJustificationItem != null );
		SpecificationItem createdFeminineOrMasculineSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundAssignmentItem;
		SpecificationItem foundFeminineOrMasculineSpecificationItem;
		SpecificationItem replacedAssignmentItem;
		SpecificationItem createdAssignmentItem = null;
		SpecificationItem createdSpecificationItem = null;
		WordItem foundWordItem;

		if( generalizationWordItem != null )
			{
			if( generalizationWordItem != myWordItem_ )
				{
				if( specificationWordItem != myWordItem_ )
					{
					if( specificationWordItem != null &&
					relationWordItem != null &&
					!relationWordItem.isNumeralWordType() )
						{
						if( relationContextNr == Constants.NO_CONTEXT_NR ||
						// If context number is already taken by the opposite word
						generalizationWordItem.contextItemInWord( relationContextNr ) != null )
							{
							if( ( contextResult = adminItem_.getRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, ( firstJustificationItem != null && firstJustificationItem.isAssumptionJustification() ), userRelationContextNr_, generalizationWordItem, specificationWordItem, relationWordItem ) ).result == Constants.RESULT_OK )
								relationContextNr = contextResult.contextNr;
							else
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to get a relation context" );
							}
						else
							{
							// Add missing relation context, if specification word is collected with itself
							if( specificationWordItem.isCollectedWithItself() &&
							specificationCollectionNr > Constants.NO_COLLECTION_NR &&
							specificationWordItem.isCompoundCollection( specificationCollectionNr ) &&
							( foundWordItem = myWordItem_.contextWordItemInAllWords( relationContextNr, specificationWordItem, null ) ) != null )
								{
								if( foundWordItem != relationWordItem )
									{
									if( missingRelationContextNr_ > Constants.NO_CONTEXT_NR &&
									relationWordItem.hasContextInWord( missingRelationContextNr_, specificationWordItem ) )
										relationContextNr = missingRelationContextNr_;
									else
										{
										if( ( relationContextNr = adminItem_.highestContextNr() ) < Constants.MAX_CONTEXT_NR )
											// Create new context number
											missingRelationContextNr_ = ++relationContextNr;
										else
											myWordItem_.startSystemErrorInItem( 1, moduleNameString_, "Context number overflow" );
										}
									}
								}
							else
								isRelationContextAlreadyDefined = true;
							}

						if( CommonVariables.result == Constants.RESULT_OK &&
						!isSelection )
							{
							if( relationWordItem.addContext( isQuestion, relationWordTypeNr, specificationWordTypeNr, relationContextNr, specificationWordItem ) == Constants.RESULT_OK )
								{
								if( relationWordItem.isUserRelationWord &&
								( foundFeminineOrMasculineSpecificationItem = adminItem_.foundFeminineOrMasculineSpecificationItem() ) != null )
									{
									if( foundFeminineOrMasculineSpecificationItem.relationContextNr() == relationContextNr &&
									( createdFeminineOrMasculineSpecificationItem = adminItem_.createdFeminineOrMasculineSpecificationItem() ) != null )
										{
										if( relationWordItem.addContext( false, relationWordTypeNr, specificationWordTypeNr, createdFeminineOrMasculineSpecificationItem.relationContextNr(), createdFeminineOrMasculineSpecificationItem.specificationWordItem() ) != Constants.RESULT_OK )
											myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a missing feminine or masculine relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							else
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
							}
						}

					if( CommonVariables.result == Constants.RESULT_OK )
						{
						mostRecentRelationContextNr_ = relationContextNr;
						existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

						// Skip adding specification if already exists
						if( isAssignment ||
						!isRelationContextAlreadyDefined ||
						existingSpecificationItem == null ||
						!existingSpecificationItem.isAssignment() )
							{
							// Check for an assignment or a specification with opposite negative indicator
							if( !isAssignment &&
							isArchivedAssignment_ &&
							generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, !isNegative, isPossessive, questionParameter, specificationWordItem ) != null )
								isAssignment = true;

							if( ( specificationResult = addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, hasUserSentenceExclusivelyFeminineOrMasculineSpecificationWords, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result == Constants.RESULT_OK )
								{
								if( !CommonVariables.hasShownWarning )
									{
									isArchivedAssignment_ = isArchivedAssignment;
									replacedAssignmentItem = specificationResult.replacedAssignmentItem;

									if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
										{
										if( !isQuestion &&
										!isSelfGenerated )
											userSpecificationItem_ = createdSpecificationItem;
										}

									if( !isExclusiveSpecification &&
									!isQuestion &&
									!isSelfGenerated &&
									!isSpecificationGeneralization &&
									specificationWordItem != null )
										{
										if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME )
											{
											if( createdSpecificationItem != null )
												{
												if( isNegative )
													{
													if( createdSpecificationItem.hasSpecificationCollection() )
														{
														if( adminItem_.makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
															myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an only-option-left assumption" );
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
																		myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw negative conclusions from answered questions in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
																}
															else
																myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make an indirectly answered question assumption in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );
													}
												}
											}
										else
											{
											if( isDefinitionSpecification )
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
																		if( Presentation.writeDiacriticalText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writeSentenceStringBuffer.toString() ) != Constants.RESULT_OK )
																			myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to write a sentence" );
																		}
																	else
																		myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to write a header" );
																	}
																else
																	myWordItem_.startErrorInItem( 1, moduleNameString_, "Integrity error! I couldn't write the assignment with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
																}
															else
																myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write a non-proper-name specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													}
												else
													{
													if( !isAssignment &&
													isPossessive )
														{
														// Draw a reversed "part of" conclusion
														// Not yet explained in the design
														if( adminItem_.addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, false, false, false, isNegative, true, false, false, false, true, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, Constants.WORD_PARAMETER_PREPOSITION_OF, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.WORD_TYPE_UNDEFINED, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
															myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a self-generated reversible part-of specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										}

									if( CommonVariables.result == Constants.RESULT_OK &&
									!CommonVariables.hasShownWarning &&
									// Skip for example imperative verbs
									adminItem_.isReasoningWordType( generalizationWordTypeNr ) &&

									( ( isAssignment &&
									// At this stage, selections must be stored - rather than executed. So, don't assign them
									!isSelection ) ||

									replacedAssignmentItem != null ) )
										{
										if( ( assignmentResult = assignSpecification( contextResult.isAmbiguousRelationContext, ( specificationWordItem != null && specificationWordItem.isAdjectiveAssignedOrClear() ), isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result == Constants.RESULT_OK )
											{
											if( ( createdAssignmentItem = assignmentResult.createdSpecificationItem ) != null )
												{
												specificationResult.createdSpecificationItem = createdAssignmentItem;

												if( isArchivedAssignment_ &&
												!isQuestion &&
												!isSelfGenerated )
													userSpecificationItem_ = createdAssignmentItem;

												if( replacedAssignmentItem != null )
													{
													if( generalizationWordItem.replaceOrDeleteSpecification( replacedAssignmentItem, createdAssignmentItem ) != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to replace or delete an assignment in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												if( CommonVariables.result == Constants.RESULT_OK &&
												!isExclusiveSpecification &&
												isQuestion &&
												relationWordItem != null )
													{
													if( adminItem_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, createdAssignmentItem.questionParameter(), generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, createdAssignmentItem, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to make a suggestive question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												if( CommonVariables.result == Constants.RESULT_OK &&
												isNegative &&
												!isQuestion &&
												createdAssignmentItem.hasSpecificationCollection() )
													{
													if( adminItem_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem.specificationCollectionNr(), generalizationWordItem ) != Constants.RESULT_OK )
														myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
													}
												}
											}
										else
											myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to assign the specification" );
										}

									if( CommonVariables.result == Constants.RESULT_OK &&
									!CommonVariables.hasShownWarning &&
									isDefinitionSpecification &&
									!isExclusiveSpecification &&
									!isQuestion &&
									!isSelfGenerated &&
									!isSpecificationGeneralization &&
									nContextRelations == 0 &&
									generalizationContextNr == Constants.NO_CONTEXT_NR &&
									specificationContextNr == Constants.NO_CONTEXT_NR &&
									specificationWordItem != null &&
									userSpecificationItem_ != null )
										{
										if( adminItem_.drawSpecificationGeneralizationConclusion( isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
											myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to draw a specification generalization conclusion" );
										}
									}
								}
							else
								myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					}
				else
					myWordItem_.startErrorInItem( 1, moduleNameString_, "The given specification word is the administrator" );
				}
			else
				myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word is the administrator" );
			}
		else
			myWordItem_.startErrorInItem( 1, moduleNameString_, "The given generalization word item is undefined" );

		specificationResult.result = CommonVariables.result;
		return specificationResult;
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
