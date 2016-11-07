/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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

class AdminSpecification
	{
	// Private constructed variables

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

	private boolean isAuthorizationRequiredForChanges( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		return( generalizationWordItem != null &&
				specificationWordItem != null &&
				adminItem_.isSystemStartingUp() &&

				( generalizationWordItem.isAuthorizationRequiredForChanges() ||
				specificationWordItem.isAuthorizationRequiredForChanges() ) );
		}

	private byte addUserSpecificationWithRelation( boolean isAction, boolean isAssignedOrClear, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short selectionLevel, short selectionListNr, short imperativeVerbParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, ReadItem startRelationReadItem, ReadItem endRelationReadItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isCheckingExistingUserSpecification;
		boolean isFirstComparisonPart = ( selectionListNr == Constants.ADMIN_CONDITION_LIST );
		boolean isStop = false;
		short relationWordTypeNr;
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		ReadItem currentRelationReadItem = startRelationReadItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem mostRecentContextWord;
		WordItem relationWordItem;
		WordItem previousRelationWordItem = null;

		if( startRelationReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given start relation read item is undefined" );

		if( endRelationReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given end relation read item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		isCheckingExistingUserSpecification = ( isPossessive &&
												nContextRelations == 0 &&
												!specificationWordItem.isNounWordSpanishAmbiguous() );

		do	{
			if( currentRelationReadItem.isPreposition() )
				prepositionParameter = currentRelationReadItem.wordParameter();
			else
				{
				if( currentRelationReadItem.isRelationWord() )
					{
					if( ( relationWordItem = currentRelationReadItem.readWordItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I have found an undefined read word" );

					if( specificationWordItem == relationWordItem )
						return adminItem_.startError( 1, moduleNameString_, "The relation word is the same as the specification word" );

					if( generalizationWordItem == relationWordItem )
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_END ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					else
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
							existingSpecificationItem = ( isCheckingExistingUserSpecification ? generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) : null );

							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( existingSpecificationItem == null ? Constants.NO_COLLECTION_NR : existingSpecificationItem.specificationCollectionNr() ), generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, null, generalizationWordItem, specificationWordItem, relationWordItem, null ).result != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							if( adminItem_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem.isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, null ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create a selection part having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

							isFirstComparisonPart = false;
							}

						previousRelationWordItem = relationWordItem;
						}
					}
				}
			}
		while( !isStop &&
		!CommonVariables.hasDisplayedWarning &&
		currentRelationReadItem != endRelationReadItem &&
		( currentRelationReadItem = currentRelationReadItem.nextReadItem() ) != null );

		return Constants.RESULT_OK;
		}

	private byte processJustification( boolean hasFoundRelationContext, boolean hasRelationWord, boolean isUserGeneralizationWord, short justificationTypeNr, JustificationItem createdOrFoundJustificationItem, SpecificationItem foundSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean isAttachingJustification = true;
		boolean isDeletingCreatedJustification = false;
		boolean isRecalculatingAssumptionsAfterAttachment = false;
		boolean isRemovingPreviousJustifications = false;
		boolean isSpecificationWordSpanishAmbiguous;
		JustificationResultType justificationResult;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem;
		JustificationItem existingJustificationItem = null;
		SpecificationItem assumptionSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( createdOrFoundJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given created or found justification item is undefined" );

		if( foundSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given found specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		isSpecificationWordSpanishAmbiguous = foundSpecificationItem.isSpecificationWordSpanishAmbiguous();
		secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem();

		switch( justificationTypeNr )
			{
			case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				if( relationWordItem != null )
					{
					if( secondarySpecificationItem.isSelfGeneratedAssumption() )
						{
						// Skip if justification with self-generated assumption is created,
						// while justification with self-generated conclusion already exists
						if( relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPossessive(), false, secondarySpecificationItem.specificationWordItem(), generalizationWordItem ) != null )
							isDeletingCreatedJustification = true;
						}
					else
						{
						if( secondarySpecificationItem.isSelfGeneratedConclusion() &&
						( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPossessive(), true, secondarySpecificationWordItem, generalizationWordItem ) ) != null )
							{
							if( ( existingJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != null )
								{
								if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace an existing justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								isAttachingJustification = false;
								}
							}
						else
							{
							if( isUserGeneralizationWord &&
							!foundSpecificationItem.isOlderItem() &&
							foundSpecificationItem.isHiddenSpanishSpecification() &&
							createdOrFoundJustificationItem.hasFeminineOrMasculineProperNameEnding() )
								isAttachingJustification = false;
							}
						}
					}

				break;

			case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( hasFoundRelationContext &&
				hasRelationWord &&
				primarySpecificationItem != null )
					{
					// Confirmation
					if( secondarySpecificationItem.isUserSpecification() )
						{
						if( isUserGeneralizationWord )
							{
							if( ( existingJustificationItem = generalizationWordItem.olderComplexJustificationItem( true, justificationTypeNr, secondarySpecificationItem.specificationCollectionNr(), primarySpecificationItem ) ) == null )
								{
								if( ( obsoleteJustificationItem = generalizationWordItem.possessiveReversibleAssumptionJustificationItem( relationWordItem, secondarySpecificationWordItem ) ) == null )
									{
									// Skip if justification already exists as an opposite possessive conditional specification assumption
									if( secondarySpecificationItem.isGeneralizationProperName() &&
									generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, secondarySpecificationItem ) != null )
										isDeletingCreatedJustification = true;
									}
								else
									{
									if( ( justificationResult = generalizationWordItem.addJustification( false, false, justificationTypeNr, obsoleteJustificationItem.orderNr, obsoleteJustificationItem.originalSentenceNr(), null, null, secondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a justification without primary specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't find or create a justification without primary specification" );

									if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification item without primary specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
									}
								}
							else
								{
								// Replace existing justification by the created justification
								if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to replace an existing justification by the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								isAttachingJustification = false;
								}
							}
						}
					else
						{
						if( !isUserGeneralizationWord &&
						!isSpecificationWordSpanishAmbiguous &&
						!foundSpecificationItem.isOlderItem() &&
						primarySpecificationItem.isOlderItem() )
							isRecalculatingAssumptionsAfterAttachment = true;
						}
					}

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( !hasRelationWord &&
				primarySpecificationItem != null &&
				( existingJustificationItem = foundSpecificationItem.differentAssumptionLevelSpecificationSubstitutionJustificationItem( primarySpecificationItem, secondarySpecificationItem ) ) != null )
					{
					isAttachingJustification = false;

					if( primarySpecificationItem.assumptionLevel() < existingJustificationItem.primarySpecificationAssumptionLevel() )
						{
						// Replace less certain existing justification by the created justification
						if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					else
						isDeletingCreatedJustification = true;
					}

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				if( !hasRelationWord &&
				primarySpecificationItem != null &&
				( specificationSubstitutionPartOfAssumptionJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) != null )
					{
					if( specificationSubstitutionPartOfAssumptionJustificationItem.hasHiddenPrimarySpecification() )
						{
						if( specificationSubstitutionPartOfAssumptionJustificationItem.changePrimarySpecification( primarySpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to replace a hidden primary specification of the attached justification item of the found specification substitution 'part of' assumption justification item" );

						isDeletingCreatedJustification = true;
						}
					}

				break;

			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION:
				if( ( !isSpecificationWordSpanishAmbiguous ||
				foundSpecificationItem.isConcludedAssumption() ) &&

				foundSpecificationItem.hasOnlyOneRelationWord() )
					isRemovingPreviousJustifications = true;

				break;

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				if( anotherSecondarySpecificationItem != null &&
				foundSpecificationItem.isConcludedAssumption() &&
				( obsoleteJustificationItem = generalizationWordItem.primarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION, secondarySpecificationItem ) ) != null )
					{
					if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to replace negative assumption justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					isAttachingJustification = false;
					}

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				// Generalization word has adjusted question
				if( foundSpecificationItem.isOlderItem() &&
				generalizationWordItem == adminItem_.adjustedQuestionWordItem() )
					{
					// Remove obsolete justifications from adjusted compound question
					isRemovingPreviousJustifications = true;
						foundSpecificationItem.lastWrittenSentenceStringBuffer = null;
					}
			}

		if( isDeletingCreatedJustification )
			{
			if( generalizationWordItem.replaceOrDeleteJustification( createdOrFoundJustificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the created justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}
		else
			{
			if( isRemovingPreviousJustifications )
				{
				if( generalizationWordItem.copyAndReplaceSpecificationItem( foundSpecificationItem.isAnsweredQuestion(), foundSpecificationItem.isExclusiveSpecification(), foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to copy and replace a specification with a different first justification item" );
				}
			else
				{
				if( isAttachingJustification )
					{
					// Attach created justification to the found specification
					if( generalizationWordItem.attachJustificationInWord( createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					if( isRecalculatingAssumptionsAfterAttachment )
						{
						if( generalizationWordItem.recalculateAssumptions() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private static WordItem mostRecentContextWordInAllWords( int contextNr )
		{
		ContextItem currentContextItem;
		ContextItem mostRecentContextItem = null;
		WordItem currentContextWordItem;
		WordItem mostRecentWordItem = null;

		if( contextNr > Constants.NO_CONTEXT_NR &&
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				currentContextItem = currentContextWordItem.contextItem( contextNr );

				if( currentContextItem != null &&

				( mostRecentContextItem == null ||
				currentContextItem.isMoreRecent( mostRecentContextItem ) ) )
					{
					mostRecentWordItem = currentContextWordItem;
					mostRecentContextItem = currentContextItem;
					}
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return mostRecentWordItem;
		}


	// Constructor

	protected AdminSpecification( AdminItem adminItem )
		{
		String errorString = null;

		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

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
		if( assignmentSelectionItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given assignment selection item is undefined" );

		if( assignSpecification( false, assignmentSelectionItem.isAssignedOrClear(), assignmentSelectionItem.isInactiveAssignment(), assignmentSelectionItem.isArchivedAssignment(), assignmentSelectionItem.isNegative(), false, assignmentSelectionItem.isPossessive(), assignmentSelectionItem.isSpecificationGeneralization(), assignmentSelectionItem.isUniqueUserRelation(), assignmentSelectionItem.assumptionLevel(), assignmentSelectionItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, assignmentSelectionItem.relationWordTypeNr(), assignmentSelectionItem.generalizationContextNr(), assignmentSelectionItem.specificationContextNr(), assignmentSelectionItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, assignmentSelectionItem.nContextRelations(), null, assignmentSelectionItem.generalizationWordItem(), assignmentSelectionItem.specificationWordItem(), assignmentSelectionItem.specificationString() ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign a specification" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( generalizationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			{
			if( ( specificationResult = adminItem_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
				return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to assign generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + ( specificationWordItem == null ? specificationString : specificationWordItem.anyWordTypeString() ) + "\" with authorization" );
			}
		else
			{
			if( ( specificationResult = generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to assign generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + ( specificationWordItem == null ? specificationString : specificationWordItem.anyWordTypeString() ) + "\"" );
			}

		return specificationResult;
		}


	// Protected specification methods

	protected void initializeLinkedWord()
		{
		linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
		linkedGeneralizationWordItem_ = null;
		}

	protected void initializeAdminSpecificationVariables()
		{
		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = null;
		userSpecificationItem_ = null;

		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordItem_ = null;
		}

	protected byte addUserSpecifications( boolean isAction, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short imperativeVerbParameter, int generalizationContextNr, int specificationContextNr, ReadItem generalizationReadItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		CollectionResultType collectionResult;
		ContextResultType contextResult;
		SpecificationResultType specificationResult;
		boolean isGeneralizationProperName;
		boolean isGeneralizationReasoningWordType;
		boolean isRelationWord;
		boolean isSpecificationWord;
		boolean hasAddedSpecification = false;
		boolean hasRelationWord = ( startRelationReadItem != null );
		boolean hasLinkedPossessiveDeterminer = false;
		boolean isAmbiguousRelationContext = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelection = ( selectionListNr != Constants.NO_LIST_NR );
		boolean isSkippingThisGeneralizationPart = false;
		boolean isSpecificationWordAlreadyAssignedByComparison = false;
		boolean isValueSpecificationWord = false;
		boolean isWaitingForRelation = false;
		boolean isWaitingForText = false;
		short firstSpecificationWordOrderNr;
		short generalizationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short valueGeneralizationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short linkedSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short currentSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short previousSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
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

		if( generalizationReadItem == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given generalization read item is undefined" );

		if( ( generalizationWordItem = generalizationReadItem.readWordItem() ) == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given generalization read item has no read word" );

		if( ( currentReadItem = startSpecificationReadItem ) == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given start specification read item is undefined" );

		if( endSpecificationReadItem == null )
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "The given end specification read item is undefined" );

		isGeneralizationProperName = generalizationReadItem.isProperName();
		generalizationWordTypeNr = generalizationReadItem.wordTypeNr();
		isGeneralizationReasoningWordType = adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr );

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
					if( hasRelationWord &&
					generalizationReadItem.wordOrderNr() < currentReadItem.wordOrderNr() )
						isSkippingThisGeneralizationPart = false;
					}
				}
			else
				{
				if( hasRelationWord &&
				doneSpecificationWordOrderNr_ > currentReadItem.wordOrderNr() )
					isSkippingThisGeneralizationPart = true;
				else
					{
					if( currentReadItem.isGeneralizationWord() )
						{
						if( hasRelationWord ||
						isSelection )
							{
							isSkippingThisGeneralizationPart = true;
							isWaitingForRelation = true;
							}
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
					currentSpecificationWordTypeNr = ( isQuestion && currentReadItem.isNoun() ? Constants.WORD_TYPE_NOUN_SINGULAR : currentReadItem.wordTypeNr() );
					currentSpecificationWordItem = readWordItem;
					}

				if( isPossessive &&
				currentReadItem.isNumeral() )
					nContextRelations = Integer.parseInt( currentReadItem.readWordTypeString() );
				else
					{
					if( !isSelection &&
					isSpecificationWord &&
					startRelationReadItem != null &&
					!generalizationWordItem.isVerbImperativeDisplayOrLoginOrRead() )
						{
						if( ( contextResult = adminItem_.getRelationContext( isArchivedAssignment, isPossessive, isQuestion, true, nContextRelations, generalizationWordItem, currentSpecificationWordItem, null, startRelationReadItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to get the multiple entry relation context" );

						isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
						userRelationContextNr_ = contextResult.contextNr;
						}

					if( currentReadItem.isNumeral() ||
					currentReadItem.isText() ||

					( isSpecificationWord &&
					!currentReadItem.isPreposition() ) )
						{
						if( isValueSpecificationWord )
							{
							hasAddedSpecification = true;

							if( hasRelationWord )
								return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "Adding a value specification with relation isn't implemented" );

							if( isSelection )
								{
								if( adminItem_.createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a value selection item" );
								}

							// Value, but no relation
							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a value specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
								if( isSelection )
									{
									hasAddedSpecification = true;

									if( hasRelationWord )
										{
										// Selection, no value, with relation
										if( addUserSpecificationWithRelation( isAction, currentReadItem.isAdjectiveAssignedOrEmpty(), isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, selectionLevel, selectionListNr, imperativeVerbParameter, userAssumptionLevel, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else
										{
										// Selection, no value, no relation
										if( adminItem_.createSelectionPart( isAction, currentReadItem.isAdjectiveAssignedOrEmpty(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, generalizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a selection part" );
										}
									}
								else	// No selection
									{
									if( previousSpecificationString_ == null &&
									previousSpecificationWordItem != null &&
									currentSpecificationWordItem != null &&
									currentReadItem.isMatchingReadWordTypeNr( previousSpecificationWordTypeNr ) )
										{
										if( ( collectionResult = adminItem_.collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to collect specification words" );

										if( collectionResult.isAmbiguousCollection )
											{
											if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, null, generalizationWordItem, previousSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification with an ambiguous specification word to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										}

									if( currentSpecificationWordTypeNr != Constants.WORD_TYPE_PREPOSITION )
										{
										previousSpecificationWordItem = currentSpecificationWordItem;
										previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
										previousSpecificationString_ = specificationString;
										}
									}
								}

							if( !isSelection ||
							!currentReadItem.isAdjectiveAssignedOrEmpty() )
								{
								doneSpecificationWordOrderNr_ = currentReadItem.wordOrderNr();
								linkedSpecificationWordTypeNr = currentSpecificationWordTypeNr;
								linkedSpecificationWordItem = currentSpecificationWordItem;

								if( !currentReadItem.isNounValue() &&
								currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADVERB &&
								currentReadItem.wordOrderNr() <= firstSpecificationWordOrderNr )
									{
									hasAddedSpecification = true;

									// Login is already created by during startup
									if( !generalizationWordItem.isVerbImperativeDisplayOrLoginOrRead() )
										{
										if( hasRelationWord )
											{
											// No selection, no value, with relation
											if( addUserSpecificationWithRelation( false, false, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, imperativeVerbParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a user specification with a relation to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else
											{
											// No selection, no value, no relation
											if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification without relation to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}

										if( isGeneralizationReasoningWordType &&
										!isNegative &&
										!isPartOf &&
										!isQuestion &&
										currentSpecificationWordItem != null &&
										adminItem_.isNounWordType( currentSpecificationWordTypeNr ) )
											{
											// Generalizations with noun specifications - without relations
											if( !isPossessive )
												{
												if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, true, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, null ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( !CommonVariables.hasDisplayedWarning )
													{
													if( isGeneralizationProperName )
														{
														if( !isExclusiveSpecification &&
														generalizationWordItem.isUserGeneralizationWord &&
														adminItem_.isSpanishCurrentLanguage() )
															{
															if( adminItem_.drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
															}
														}
													else
														{
														// Definition specification
														if( ( specificationResult = adminItem_.drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a primary specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

														compoundGeneralizationWordItem = specificationResult.compoundGeneralizationWordItem;
														}
													}
												}

											if( isGeneralizationProperName &&
											!CommonVariables.hasDisplayedWarning &&
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

					if( !CommonVariables.hasDisplayedWarning &&
					currentSpecificationWordItem != null )
						{
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
							adminItem_.isNounWordType( linkedSpecificationWordTypeNr ) )
								{
								if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationWord ? currentSpecificationWordItem : null ) ) != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
								}

							if( isRelationWord ||

							( isSpecificationWord &&
							hasLinkedPossessiveDeterminer ) )
								{
								if( !CommonVariables.hasDisplayedWarning &&
								// Linked specification
								currentSpecificationWordItem == linkedSpecificationWordItem )
									{
									if( linkedGeneralizationWordItem_ != null )
										return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "Linked word \"" + linkedGeneralizationWordItem_.anyWordTypeString() + "\" is already assigned" );

									linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
									linkedGeneralizationWordItem_ = generalizationWordItem;
									}

								if( !CommonVariables.hasDisplayedWarning &&
								isGeneralizationProperName &&
								!isQuestion &&
								!isSpecificationGeneralization &&
								userRelationContextNr_ > Constants.NO_CONTEXT_NR &&
								CommonVariables.nUserRelationWords > 1 )
									{
									if( adminItem_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, userRelationContextNr_, generalizationWordItem, linkedSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with multiple relation words, from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							else
								{
								if( !isSelection &&
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
		while( !CommonVariables.hasDisplayedWarning &&
		currentReadItem != endSpecificationReadItem &&
		( currentReadItem = currentReadItem.nextReadItem() ) != null );

		if( !hasAddedSpecification )
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I have not added any specification" );

		if( isGeneralizationProperName )
			{
			if( adminItem_.recalculateAssumptionsInAllTouchedWords() != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumptions in all words" );
			}

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNameEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkippingAdditionalConclusionOrAskQuestion, boolean isSpecificationGeneralization, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isUserGeneralizationWord;
		boolean hasFoundRelationContext = false;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isAssumption = adminItem_.isAssumption( assumptionJustificationTypeNr );
		boolean isGeneralizationProperName = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NAME );
		short justificationTypeNr = assumptionJustificationTypeNr;
		int createdRelationContextNr;
		JustificationItem createdJustificationItem = null;
		JustificationItem foundJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;

		if( generalizationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( primarySpecificationItem != null &&
		primarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( anotherPrimarySpecificationItem != null &&
		anotherPrimarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given another primary specification item is undefined" );

		if( secondarySpecificationItem != null &&
		secondarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( anotherSecondarySpecificationItem != null &&
		anotherSecondarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		// Check assumption
		if( isAssumption &&
		!hasFeminineOrMasculineProperNameEnding &&
		adminItem_.assumptionGrade( ( anotherPrimarySpecificationItem != null ), hasFeminineOrMasculineProperNameEnding, ( primarySpecificationItem != null && primarySpecificationItem.isGeneralizationProperName() && primarySpecificationItem.isPossessive() ), ( primarySpecificationItem != null && primarySpecificationItem.isQuestion() ), assumptionJustificationTypeNr ) == 0 &&

		( primarySpecificationItem == null ||

		( !primarySpecificationItem.hasAssumptionLevel() &&
		!primarySpecificationItem.isSelfGeneratedAssumption() ) ) &&

		( anotherPrimarySpecificationItem == null ||

		( !anotherPrimarySpecificationItem.hasAssumptionLevel() &&
		!anotherPrimarySpecificationItem.isSelfGeneratedAssumption() ) ) &&

		( secondarySpecificationItem == null ||

		( !secondarySpecificationItem.hasAssumptionLevel() &&
		!secondarySpecificationItem.isSelfGeneratedAssumption() ) ) )
			{
			// Conclusion
			if( !adminItem_.isConclusion( conclusionJustificationTypeNr ) )
				return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given conclusion justification type isn't a conclusion" );

			isAssumption = false;
			justificationTypeNr = conclusionJustificationTypeNr;
			}

		if( ( justificationResult = generalizationWordItem.addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, justificationTypeNr, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add a justification" );

		createdJustificationItem = justificationResult.createdJustificationItem;
		foundJustificationItem = justificationResult.foundJustificationItem;

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == null ? foundJustificationItem : createdJustificationItem ) ) == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "I couldn't find or create a justification" );

		if( ( specificationResult = addSpecification( isAssignment, false, false, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION || justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION ), false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to create a specification" );

		if( !CommonVariables.hasDisplayedWarning )
			{
			createdSpecificationItem = specificationResult.createdSpecificationItem;

			if( ( foundSpecificationItem = specificationResult.foundSpecificationItem ) != null )
				hasFoundRelationContext = foundSpecificationItem.hasRelationContext();

			if( createdSpecificationItem == null )
				{
				if( foundSpecificationItem == null )
					return adminItem_.startSpecificationResultError( 1, moduleNameString_, "I couldn't find or create a specification" );

				if( foundSpecificationItem.isUserSpecification() )
					return adminItem_.startSpecificationResultError( 1, moduleNameString_, "I have found a user specification, whilst trying to create a self-generated specification" );

				isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

				// A justification has been created, but the self-generated specification already exists
				// So, the justification needs to be added or replaced separately
				if( ( createdJustificationItem == null &&

				( !foundSpecificationItem.hasJustification( false, foundJustificationItem ) ||

				( !isPossessive &&
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ) ) ) ||

				( createdJustificationItem != null &&

				( !isAssumption ||
				foundSpecificationItem.isConcludedAssumption() ||
				foundSpecificationItem.isSelfGeneratedAssumption() ||

				// Conclusion becomes assumption
				( foundSpecificationItem.isSelfGeneratedConclusion() &&

				( justificationTypeNr == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||

				( !isUserGeneralizationWord &&
				justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
				relationWordItem != null &&

				( ( isPossessive &&
				!relationWordItem.hasConfirmedSpecification() ) ||

				( !isPossessive &&

				( relationWordItem.hasAnsweredSelfGeneratedQuestion() ||
				generalizationWordItem.hasAnsweredSelfGeneratedQuestion() ||

				( relationWordItem.hasConfirmedSpecification() &&
				foundSpecificationItem.hasSpecificationCompoundCollection() ) ) ) ) ) ) ) ) ) )
					{
					if( processJustification( hasFoundRelationContext, hasRelationWord, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to process the created justification" );
					}
				}
			else
				{
				createdRelationContextNr = createdSpecificationItem.relationContextNr();

				if( !hasRelationWord ||
				!hasFoundRelationContext ||
				generalizationWordItem.hasConfirmedSpecification() )
					{
					if( hasRelationWord &&
					isGeneralizationProperName )
						{
						if( adminItem_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, createdRelationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

						if( isAssumption &&
						!isExclusiveSpecification &&
						!createdSpecificationItem.isHiddenSpanishSpecification() )
							{
							// Check self-generated specification for integrity
							if( generalizationWordItem.writeSelectedSpecification( true, true, false, true, Constants.NO_ANSWER_PARAMETER, createdSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to write the self-generated specification in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to check the writing integrity" );

							if( CommonVariables.writtenSentenceStringBuffer == null ||
							CommonVariables.writtenSentenceStringBuffer.length() == 0 )
								return adminItem_.startSpecificationResultError( 1, moduleNameString_, "Integrity error! I couldn't write the self-generated specification with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );
							}
						}
					}
				else
					{
					if( foundSpecificationItem != null &&
					foundSpecificationItem.relationContextNr() == createdRelationContextNr )
						return adminItem_.startSpecificationResultError( 1, moduleNameString_, "Integrity error! A specification was created in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" while a specification was found as well with the same relation context" );
					}
				}

			if( !isNegative &&
			!isPartOf &&
			!isSkippingAdditionalConclusionOrAskQuestion &&
			adminItem_.isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					createdSpecificationItem != null )
						{
						// Draw a unique relation conclusion
						// Not yet explained in the design
						if( addSelfGeneratedSpecification( false, false, false, isEveryGeneralization, isExclusiveSpecification, false, true, false, true, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, createdSpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
							return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					{
					if( createdJustificationItem != null ||
					createdSpecificationItem != null )
						{
						if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, false, isArchivedAssignment, isExclusiveSpecification, true, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		specificationResult.createdJustificationItem = createdJustificationItem;
		return specificationResult;
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		ContextResultType contextResult = new ContextResultType();
		SpecificationResultType assignmentResult = new SpecificationResultType();
		SpecificationResultType specificationResult = new SpecificationResultType();
		boolean isAuthorizationRequired;
		boolean isDefinitionSpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isCompoundCollectionSpanishAmbiguous = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationContextAlreadyDefined = false;
		boolean isSelfGenerated = ( firstJustificationItem != null );
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem foundAssignmentItem;
		SpecificationItem replacedAssignmentItem;
		SpecificationItem tempSpecificationItem;
		SpecificationItem createdAssignmentItem = null;
		SpecificationItem createdSpecificationItem = null;

		if( generalizationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, "The given specification word is the administrator" );

		existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem );

		// Get relation context
		// Exceptions: Conditions and imperative verbs have no relation context
		if( !isConditional &&
		!isSelection &&
		generalizationWordTypeNr != Constants.WORD_TYPE_VERB_SINGULAR &&
		specificationWordItem != null &&
		relationWordItem != null )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			if( isSpecificationWordSpanishAmbiguous &&
			isPossessive &&
			specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			specificationWordItem.isCompoundCollection( specificationCollectionNr ) )
				isCompoundCollectionSpanishAmbiguous = true;

			if( isArchivedAssignment ||
			relationContextNr == Constants.NO_CONTEXT_NR ||

			( isPossessive &&
			!isSpecificationWordSpanishAmbiguous &&
			nContextRelations == 0 &&
			specificationCollectionNr == Constants.NO_COLLECTION_NR &&

			// Possessive user specification
			( !isSelfGenerated ||

			// Ignoring context in other words
			( existingSpecificationItem != null &&
			existingSpecificationItem.hasRelationContext() &&

			( !existingSpecificationItem.hasSpecificationCollection() ||
			existingSpecificationItem.relationContextNr() != relationContextNr ) &&

			( !relationWordItem.hasConfirmedSpecification() ||
			existingSpecificationItem.isSelfGeneratedAssumption() ) ) ) ) )
				{
				if( ( contextResult = adminItem_.getSpecificationRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionSpanishAmbiguous, isPossessive, ( firstJustificationItem != null && firstJustificationItem.isAssumptionJustification() ), specificationCollectionNr, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to get a relation context" );

				relationContextNr = contextResult.contextNr;
				}
			else
				isRelationContextAlreadyDefined = true;

			if( relationWordItem.addContext( isCompoundCollectionSpanishAmbiguous, relationWordTypeNr, specificationWordTypeNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
			}

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
			isAuthorizationRequired = isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem );

			if( isAuthorizationRequired )
				{
				if( ( specificationResult = adminItem_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with authorization" );
				}
			else
				{
				if( ( specificationResult = generalizationWordItem.addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add an assignment specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}

			if( !CommonVariables.hasDisplayedWarning )
				{
				isArchivedAssignment_ = isArchivedAssignment;
				replacedAssignmentItem = specificationResult.replacedAssignmentItem;

				isDefinitionSpecification = ( relationWordItem == null &&
											adminItem_.isNounWordType( generalizationWordTypeNr ) &&
											adminItem_.isNounWordType( specificationWordTypeNr ) );

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
				specificationWordItem != null &&
				!generalizationWordItem.hasConfirmedSpecification() )
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
										return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to make an only-option-left assumption" );
									}
								}
							else
								{
								if( adminItem_.collectGeneralizationWordWithPreviousOne( ( isAssignment || isAuthorizationRequired ), isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

								if( !isPossessive )
									{
									if( adminItem_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									if( generalizationWordItem.hasFoundAnswerToQuestion() &&
									!specificationWordItem.isCompoundCollection( createdSpecificationItem.specificationCollectionNr() ) )
										{
										if( adminItem_.drawNegativeConclusionsFromAnsweredQuestions( createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to draw negative conclusions from answered questions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
										}
									}
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
									if( addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, false, false, isNegative, false, true, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
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
											if( generalizationWordItem.writeSelectedSpecification( false, true, ( foundAssignmentItem == null ? specificationResult.foundSpecificationItem : foundAssignmentItem ) ) != Constants.RESULT_OK )
												return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to write a non-proper-name specification of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( CommonVariables.writtenSentenceStringBuffer == null ||
											CommonVariables.writtenSentenceStringBuffer.length() == 0 )
												return adminItem_.startSpecificationResultError( 1, moduleNameString_, "Integrity error! I couldn't write the assignment with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );

											if( Presentation.writeInterfaceText( true, Constants.PRESENTATION_PROMPT_NOTIFICATION, ( foundAssignmentItem == null ? Constants.INTERFACE_LISTING_SENTENCE_ASSIGNMENT_CONFLICTS_WITH_DEFINITION_SPECIFICATION : Constants.INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) != Constants.RESULT_OK )
												return adminItem_.startSpecificationResultError( 1, moduleNameString_, "I failed to write a header" );

											if( Presentation.writeText( Constants.PRESENTATION_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
												return adminItem_.startSpecificationResultError( 1, moduleNameString_, "I failed to write a sentence" );
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
										if( addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, false, false, isNegative, true, false, false, false, true, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
											return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							}
						}
					}

				if( !CommonVariables.hasDisplayedWarning &&
				// Skip for example imperative verbs
				adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) &&

				( ( isAssignment &&
				// At this stage, selections must be stored - rather than executed. So, don't assign them
				!isSelection ) ||

				replacedAssignmentItem != null ) )
					{
					if( ( assignmentResult = assignSpecification( contextResult.isAmbiguousRelationContext, ( specificationWordItem != null && specificationWordItem.isAdjectiveAssignedOrEmpty() ), isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
						return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to assign the specification" );

					if( ( createdAssignmentItem = assignmentResult.createdSpecificationItem ) != null )
						{
						if( !isArchivedAssignment_ ||
						specificationResult.createdSpecificationItem != null )
							specificationResult.createdSpecificationItem = createdAssignmentItem;

						if( replacedAssignmentItem != null )
							{
							if( generalizationWordItem.replaceOrDeleteSpecification( false, replacedAssignmentItem, createdAssignmentItem ) != Constants.RESULT_OK )
								return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to replace or delete an assignment in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}

						if( isNegative &&
						!isQuestion &&
						createdAssignmentItem.hasSpecificationCollection() &&
						generalizationWordItem.isUserGeneralizationWord )
							{
							if( adminItem_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem.specificationCollectionNr(), generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
							}
						}

					if( !isSelfGenerated &&
					( tempSpecificationItem = ( createdAssignmentItem == null ? assignmentResult.foundSpecificationItem : createdAssignmentItem ) ) != null )
						{
						userSpecificationItem_ = tempSpecificationItem;

						if( isArchivedAssignment_ &&
						!isQuestion )
							simpleUserSpecificationItem_ = tempSpecificationItem;
						}
					}

				if( !CommonVariables.hasDisplayedWarning &&
				!isExclusiveSpecification &&
				specificationWordItem != null &&
				userSpecificationItem_ != null &&

				( !isSelfGenerated ||

				( isDefinitionSpecification &&
				userSpecificationItem_.hasGeneralizationCollection() ) ) )
					{
					if( isQuestion )
						{
						if( hasRelationWord &&

						( createdAssignmentItem != null ||

						( createdSpecificationItem != null &&
						!generalizationWordItem.hasDisplayedMoreSpecificRelatedQuestion() ) ) )
							{
							if( adminItem_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, ( createdAssignmentItem == null ? createdSpecificationItem : createdAssignmentItem ), generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to make a suggestive question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						{
						if( isDefinitionSpecification &&
						!isSpecificationGeneralization )
							{
							if( adminItem_.drawSpecificationGeneralizationConclusion( isArchivedAssignment, isPossessive, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addSpecificationResultError( 1, moduleNameString_, "I failed to draw a specification generalization conclusion" );
							}
						}
					}
				}
			}

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
