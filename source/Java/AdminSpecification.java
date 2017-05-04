/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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


	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


	// Private methods

	private boolean isAuthorizationRequiredForChanges( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		return( generalizationWordItem != null &&
				specificationWordItem != null &&
				adminItem_.isSystemStartingUp() &&

				( generalizationWordItem.isAuthorizationRequiredForChanges() ||
				specificationWordItem.isAuthorizationRequiredForChanges() ) );
		}

	private static boolean isValidRelationContextInContextWords( boolean isPossessive, int existingRelationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		WordItem currentContextWordItem;

		if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&
		specificationWordItem != null &&
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem != relationWordItem &&
				!currentContextWordItem.isUserRelationWord &&
				currentContextWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, existingRelationContextNr, specificationWordItem ) != null )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return true;
		}

	private static int collectionNrByCompoundGeneralizationWordInCollectionWords( boolean isExclusiveSpecification, short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int collectionNr;
		WordItem currentCollectionWordItem;

		if( collectionWordTypeNr > Constants.NO_WORD_TYPE_NR &&
		compoundGeneralizationWordItem != null &&
		( currentCollectionWordItem = CommonVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( ( collectionNr = currentCollectionWordItem.collectionNrByCompoundGeneralizationWordInWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		return Constants.NO_COLLECTION_NR;
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
						return adminItem_.startError( 1, moduleNameString_, "I found an undefined read word" );

					if( specificationWordItem == relationWordItem )
						return adminItem_.startError( 1, moduleNameString_, "The relation word is the same as the specification word" );

					if( generalizationWordItem == relationWordItem )
						{
						if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_END ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
						}
					else
						{
						relationWordTypeNr = currentRelationReadItem.wordTypeNr();

						if( previousRelationWordItem == null &&
						CommonVariables.nUserRelationWords == 1 &&
						( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordItem ) ) != null &&
						foundSpecificationItem.hasRelationContext() &&
						( mostRecentContextWord = mostRecentContextWordInContextWords( foundSpecificationItem.relationContextNr() ) ) != null &&
						mostRecentContextWord != relationWordItem )
							previousRelationWordItem = mostRecentContextWord;

						if( previousRelationWordItem != null &&
						collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

						if( selectionListNr == Constants.NO_LIST_NR )
							{
							existingSpecificationItem = ( isCheckingExistingUserSpecification ? generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem, relationWordItem ) : null );

							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( existingSpecificationItem == null ? Constants.NO_COLLECTION_NR : existingSpecificationItem.specificationCollectionNr() ), generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, null, generalizationWordItem, specificationWordItem, relationWordItem, null ).result != Constants.RESULT_OK )
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

	private byte collectGeneralizationWords( boolean isExclusiveGeneralization, short generalizationWordTypeNr, short commonWordTypeNr, WordItem previousGeneralizationWordItem, WordItem newGeneralizationWordItem, WordItem previousCommonWordItem, WordItem newCommonWordItem )
		{
		boolean hasFoundCollection = false;
		int generalizationCollectionNr = Constants.NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;

		if( previousGeneralizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given previous generalization word item is undefined" );

		if( newGeneralizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given new generalization word item is undefined" );

		if( previousGeneralizationWordItem == newGeneralizationWordItem )
			return adminItem_.startError( 1, moduleNameString_, "The given previous and new generalization word items are the same word" );

		if( newCommonWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given new common word item is undefined" );

		if( ( generalizationCollectionNr = previousGeneralizationWordItem.collectionNr( generalizationWordTypeNr, newCommonWordItem ) ) == Constants.NO_COLLECTION_NR )
			generalizationCollectionNr = newGeneralizationWordItem.collectionNr( generalizationWordTypeNr, newCommonWordItem );

		if( generalizationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			if( ( boolResult = newGeneralizationWordItem.findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" is collected with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			if( ( collectionResult = previousGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, newGeneralizationWordItem, newCommonWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( generalizationCollectionNr == Constants.NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			if( newGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + newGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( previousGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in previous generalization word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( newCommonWordItem != previousCommonWordItem )
				{
				if( previousGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, newGeneralizationWordItem, previousCommonWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );

				if( newGeneralizationWordItem.addCollection( isExclusiveGeneralization, false, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, previousCommonWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + newGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );
				}

			if( newGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in new generalization word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte collectRelationWords( boolean isExclusiveSpecification, short relationWordTypeNr, short commonWordTypeNr, WordItem previousRelationWordItem, WordItem currentRelationWordItem, WordItem commonWordItem )
		{
		boolean hasFoundCollection = false;
		int collectionNr = Constants.NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;

		if( previousRelationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given previous relation word item is undefined" );

		if( currentRelationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given current relation word item is undefined" );

		if( previousRelationWordItem == currentRelationWordItem )
			return adminItem_.startError( 1, moduleNameString_, "The given previous and current relation words are the same word" );

		if( ( collectionNr = previousRelationWordItem.collectionNr( relationWordTypeNr, commonWordItem ) ) == Constants.NO_COLLECTION_NR )
			collectionNr = currentRelationWordItem.collectionNr( relationWordTypeNr, commonWordItem );

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			if( ( boolResult = previousRelationWordItem.findCollection( false, currentRelationWordItem, commonWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + previousRelationWordItem.anyWordTypeString() + "\" is collected with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			if( ( collectionResult = previousRelationWordItem.addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousRelationWordItem.anyWordTypeString() + "\" with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

			if( collectionNr == Constants.NO_COLLECTION_NR )
				collectionNr = collectionResult.createdCollectionNr;

			if( currentRelationWordItem.addCollection( isExclusiveSpecification, false, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + currentRelationWordItem.anyWordTypeString() + "\" with word \"" + previousRelationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte collectSpecificationWords( boolean isExclusiveSpecification, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem compoundGeneralizationWordItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		boolean hasFoundCollection = false;
		short existingPairCollectionOrderNr;
		short collectionOrderNr = Constants.NO_ORDER_NR;
		int specificationCollectionNr = Constants.NO_COLLECTION_NR;
		BoolResultType boolResult = new BoolResultType();
		CollectionResultType collectionResult;
		ShortResultType shortResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( previousSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given previous specification word item is undefined" );

		if( currentSpecificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given current specification word item is undefined" );

		if( previousSpecificationWordItem == currentSpecificationWordItem )
			return adminItem_.startError( 1, moduleNameString_, "The given previous and current specification words are the same word" );

		if( compoundGeneralizationWordItem == null )
			{
			if( ( specificationCollectionNr = previousSpecificationWordItem.collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == Constants.NO_COLLECTION_NR )
				{
				if( ( specificationCollectionNr = currentSpecificationWordItem.collectionNr( specificationWordTypeNr, generalizationWordItem ) ) == Constants.NO_COLLECTION_NR )
					{
					if( ( specificationWordTypeNr != Constants.WORD_TYPE_NOUN_PLURAL ||
					generalizationWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR ) &&

					( specificationCollectionNr = previousSpecificationWordItem.collectionNr( specificationWordTypeNr ) ) == Constants.NO_COLLECTION_NR )
						specificationCollectionNr = currentSpecificationWordItem.collectionNr( specificationWordTypeNr );
					}
				}
			else
				{
				// Collection already exists
				if( previousSpecificationWordItem.isOlderItem() &&
				currentSpecificationWordItem.isOlderItem() )
					{
					hasFoundCollection = true;
					specificationCollectionNr = Constants.NO_COLLECTION_NR;
					}
				}
			}
		else
			{
			if( !isExclusiveSpecification ||
			generalizationWordItem == previousSpecificationWordItem ||
			!generalizationWordItem.isNounWordSpanishAmbiguous() )
				specificationCollectionNr = collectionNrByCompoundGeneralizationWordInCollectionWords( isExclusiveSpecification, specificationWordTypeNr, compoundGeneralizationWordItem );
			}

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			if( ( shortResult = checkCollectionInCollectionWords( specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check the collection in all words" );

			if( ( existingPairCollectionOrderNr = shortResult.shortValue ) > Constants.NO_ORDER_NR &&
			( collectionOrderNr = adminItem_.highestCollectionOrderNrInCollectionWords( specificationCollectionNr ) ) > 1 &&
			// "- 1" because a collection comes in pairs
			existingPairCollectionOrderNr < collectionOrderNr - 1 )
				specificationCollectionNr = Constants.NO_COLLECTION_NR;
			}

		if( !isPossessive &&
		specificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			if( ( boolResult = previousSpecificationWordItem.findCollection( ( previousSpecificationWordItem != generalizationWordItem ), currentSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" is collected with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			// A collection comes in pairs
			if( collectionOrderNr >= Constants.MAX_ORDER_NR - 1 )
				return adminItem_.startSystemError( 1, moduleNameString_, "Collection order number overflow" );

			if( ( collectionResult = previousSpecificationWordItem.addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

			if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem.addCollection( isExclusiveSpecification, isSpecificationGeneralization, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		generalizationWordItem.collectSpecificationsInWord( isExclusiveSpecification, isQuestion, specificationCollectionNr ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to collect specifications in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		return Constants.RESULT_OK;
		}

	private byte copyContext( int existingContextNr, int newContextNr )
		{
		ContextItem currentContextItem;
		WordItem currentContextWordItem;

		if( ( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( ( currentContextItem = currentContextWordItem.contextItem( existingContextNr ) ) != null &&
				currentContextWordItem.addContext( currentContextItem.isCompoundCollectionSpanishAmbiguous(), currentContextItem.contextWordTypeNr(), currentContextItem.specificationWordTypeNr(), newContextNr, currentContextItem.specificationWordItem() ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a copied context to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte processJustification( boolean hasFoundRelationContext, boolean hasRelationWord, boolean hasRelationWordConfirmedSpecification, boolean isUserGeneralizationWord, short justificationTypeNr, JustificationItem createdOrFoundJustificationItem, SpecificationItem foundSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean isAttachingJustification = true;
		boolean isDeletingCreatedJustification = false;
		boolean isRemovingPreviousJustifications = false;
		boolean isSpecificationWordSpanishAmbiguous;
		JustificationResultType justificationResult;
		JustificationItem createdJustificationItem;
		JustificationItem obsoleteJustificationItem;
		JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem;
		JustificationItem existingJustificationItem = null;
		SpecificationItem assumptionSpecificationItem;
		SpecificationItem confirmedAllRelationWordsOfSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( createdOrFoundJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given created or found justification item is undefined" );

		if( foundSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given found specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		if( ( secondarySpecificationWordItem = secondarySpecificationItem.specificationWordItem() ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item has no specification word" );

		isSpecificationWordSpanishAmbiguous = foundSpecificationItem.isSpecificationWordSpanishAmbiguous();

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
						if( isUserGeneralizationWord )
							{
							if( isSpecificationWordSpanishAmbiguous )
								{
								if( !foundSpecificationItem.isOlderItem() &&
								foundSpecificationItem.isHiddenSpanishSpecification() &&
								createdOrFoundJustificationItem.hasFeminineOrMasculineProperNameEnding() )
									isAttachingJustification = false;
								}
							else
								{
								if( secondarySpecificationItem.isSelfGeneratedConclusion() &&
								( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPossessive(), true, secondarySpecificationWordItem, generalizationWordItem ) ) != null &&
								( existingJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != null )
									{
									if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace an existing justification item in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									isAttachingJustification = false;
									}
								}
							}
						}
					}

				break;

			case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( hasFoundRelationContext &&
				hasRelationWord &&
				isUserGeneralizationWord &&
				primarySpecificationItem != null &&
				// Confirmation
				secondarySpecificationItem.isUserSpecification() )
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
							if( ( justificationResult = generalizationWordItem.addJustification( false, false, false, justificationTypeNr, obsoleteJustificationItem.orderNr, obsoleteJustificationItem.originalSentenceNr(), null, null, secondarySpecificationItem, null, null ) ).result != Constants.RESULT_OK )
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

				break;

			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				if( !hasRelationWordConfirmedSpecification &&
				!isUserGeneralizationWord &&
				foundSpecificationItem.isOlderItem() &&
				foundSpecificationItem.isSelfGeneratedConclusion() )
					isDeletingCreatedJustification = true;

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
				else
					{
					if( hasRelationWordConfirmedSpecification &&
					foundSpecificationItem.isSelfGeneratedAssumption() &&
					( confirmedAllRelationWordsOfSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, false, false, false, foundSpecificationItem.specificationWordItem(), null ) ) != null )
						{
						if( confirmedAllRelationWordsOfSpecificationItem.attachJustificationToSpecification( createdOrFoundJustificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to attach the created justification to the conclusion specification" );

						isAttachingJustification = false;
						}
					}

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
				adminItem_.adjustedQuestionWordItem() == generalizationWordItem )
					{
					// Remove obsolete justifications from adjusted compound question
					isRemovingPreviousJustifications = true;
					// Adjusted question. Needs to be rewritten
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
				if( generalizationWordItem.copyAndReplaceSpecificationItem( foundSpecificationItem.isAnsweredQuestion(), foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.isExclusiveSpecification(), foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to copy and replace a specification with a different first justification item" );
				}
			else
				{
				if( isAttachingJustification )
					{
					if( primarySpecificationItem == null &&
					foundSpecificationItem.isSelfGeneratedAssumption() &&
					!foundSpecificationItem.hasAssumptionLevel() )
						foundSpecificationItem.incrementAssumptionLevel();

					// Attach created justification to the found specification
					if( generalizationWordItem.attachJustificationInWord( createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to attach a justification to a self-generated specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte recalculateAssumptionsInTouchedWords()
		{
		WordItem currentTouchedProperNameWordItem;

		if( ( currentTouchedProperNameWordItem = CommonVariables.firstTouchedProperNameWordItem ) != null )
			{
			// Do for all proper name words touched during current sentence
			do	{
				if( currentTouchedProperNameWordItem.recalculateAssumptionsInWord() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumptions of word \"" + currentTouchedProperNameWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedProperNameWordItem = currentTouchedProperNameWordItem.nextTouchedProperNameWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private static WordItem mostRecentContextWordInContextWords( int contextNr )
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

	private ContextResultType findPossessiveReversibleConclusionRelationContextOfInvolvedWords( boolean isPossessive, int nContextRelations, int relationContextNr, SpecificationItem foundSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		int currentRelationContextNr;
		int nContextWords;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem currentSpecificationItem;
		WordItem currentGeneralizationWordItem;

		if( relationContextNr <= Constants.NO_CONTEXT_NR )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given relation context number is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = generalizationWordItem.firstRelationGeneralizationItem() ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startContextResultError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( currentSpecificationItem = currentGeneralizationWordItem.firstRelationSpecificationItem() ) != null )
					{
					do	{
						if( currentSpecificationItem.isRelatedSpecification( false, false, isPossessive, specificationWordItem ) )
							{
							currentRelationContextNr = currentSpecificationItem.relationContextNr();

							if( !isPossessive ||
							currentRelationContextNr != relationContextNr )
								{
								nContextWords = adminItem_.nContextWordsInContextWords( currentRelationContextNr, specificationWordItem );

								if( currentSpecificationItem.isPossessiveReversibleConclusion() )
									{
									// Has found all relation words.
									// In other words: This relation word will be the last one
									if( nContextWords + 1 == nContextRelations )
										{
										contextResult.contextNr = relationContextNr;
										contextResult.confirmedAllRelationWordsOfSpecificationItem = foundSpecificationItem;
										}
									else
										contextResult.contextNr = currentRelationContextNr;
									}
								else
									{
									// Found equal number of relation words
									if( nContextWords == nContextRelations )
										{
										contextResult.contextNr = currentRelationContextNr;
										contextResult.confirmedAllRelationWordsOfSpecificationItem = foundSpecificationItem;
										}
									}
								}
							}
						}
					while( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
					( currentSpecificationItem = currentSpecificationItem.nextRelationSpecificationItem() ) != null );
					}
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextRelationGeneralizationItem() ) != null );
			}

		return contextResult;
		}

	private ContextResultType getRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCompoundCollectionSpanishAmbiguous, boolean isPossessive, boolean isSelfGeneratedAssumption, int specificationCollectionNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasGeneralizationWordAnsweredSelfGeneratedQuestion;
		boolean hasRelationWordConfirmedSpecification;
		boolean hasRelationWordExistingRelationContext;
		boolean isExistingSpecificationConcludedAssumption;
		boolean isExistingSpecificationSelfGeneratedConclusion;
		boolean isGeneralizationWordUserRelationWord;
		boolean isPossessiveUserSpecification;
		boolean isSpecificationWordSpanishAmbiguous;
		int foundContextNr;
		int existingCopyContextNr = Constants.NO_CONTEXT_NR;
		int existingRelationContextNr = Constants.NO_CONTEXT_NR;
		int foundRelationContextNr = Constants.NO_CONTEXT_NR;
		int nUserRelationWords = CommonVariables.nUserRelationWords;
		ContextItem foundContextItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem existingSpecificationItem = null;

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( isCompoundCollectionSpanishAmbiguous )
			existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, specificationWordItem );
		else
			existingSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, isArchivedAssignment, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationCollectionNr, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem );

		hasRelationWordConfirmedSpecification = relationWordItem.hasConfirmedSpecification();

		if( existingSpecificationItem != null )
			existingRelationContextNr = existingSpecificationItem.relationContextNr();

		// Try to find relation context with same number of relation words as in the user sentence
		if( ( foundContextItem = relationWordItem.contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, specificationWordItem ) ) == null )
			{
			isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			if( existingSpecificationItem == null ||

			( !hasRelationWordConfirmedSpecification &&
			isGeneralizationWordUserRelationWord &&
			!isSpecificationWordSpanishAmbiguous &&
			!existingSpecificationItem.hasRelationContext() ) )
				{
				if( nUserRelationWords > 1 &&

				( CommonVariables.nUserGeneralizationWords > 1 ||
				!relationWordItem.isOlderItem() ||
				// Typically for the Spanish language
				generalizationWordItem.isFemale() ) &&

				( foundContextNr = relationWordItem.contextNr( isCompoundCollectionSpanishAmbiguous, specificationWordItem ) ) > Constants.NO_CONTEXT_NR )
					contextResult.contextNr = foundContextNr;
				}
			else
				{
				isExistingSpecificationConcludedAssumption = existingSpecificationItem.isConcludedAssumption();

				// Not found yet
				if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
					{
					// Find specification with found context word as relation word
					if( isSpecificationWordSpanishAmbiguous )
						{
						if( ( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, false, isPossessive, Constants.NO_QUESTION_PARAMETER, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
							{
							if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&

							( !foundSpecificationItem.hasRelationContext() ||
							existingSpecificationItem.isHiddenSpanishSpecification() ) )
								foundRelationContextNr = existingRelationContextNr;
							else
								{
								if( specificationCollectionNr == Constants.NO_COLLECTION_NR ||
								foundSpecificationItem.isUserSpecification() ||
								foundSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
									foundRelationContextNr = foundSpecificationItem.relationContextNr();
								}
							}
						}
					else
						{
						if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&

						( !isExistingSpecificationConcludedAssumption ||
						isGeneralizationWordUserRelationWord ) &&

						existingSpecificationItem.isArchivedAssignment() == isArchivedAssignment )
							foundRelationContextNr = existingRelationContextNr;
						}

					// Cross-collected afterwards
					if( foundRelationContextNr > Constants.NO_CONTEXT_NR &&
					contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						isExistingSpecificationSelfGeneratedConclusion = existingSpecificationItem.isSelfGeneratedConclusion();

						if( generalizationWordItem.hasConfirmedSpecification() )
							{
							if( !isPossessive ||
							specificationCollectionNr == Constants.NO_COLLECTION_NR ||
							!existingSpecificationItem.isOlderItem() ||

							( userSpecificationItem != null &&
							userSpecificationItem.hasSpecificationCompoundCollection() &&

							( !isSpecificationWordSpanishAmbiguous ||

							// Typically for Spanish
							( !isExistingSpecificationSelfGeneratedConclusion &&
							userSpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ) )
								contextResult.contextNr = existingRelationContextNr;
							}
						else
							{
							hasRelationWordExistingRelationContext = relationWordItem.hasContextInWord( existingRelationContextNr, specificationWordItem );
							isPossessiveUserSpecification = ( userSpecificationItem != null &&
															userSpecificationItem.isPossessive() );

							// Check for copy context
							if( !hasRelationWordExistingRelationContext &&
							isGeneralizationWordUserRelationWord &&
							existingSpecificationItem.isOlderItem() &&

							( ( isExistingSpecificationConcludedAssumption &&
							isPossessiveUserSpecification ) ||

							( !isPossessive &&
							isExistingSpecificationSelfGeneratedConclusion &&
							isSelfGeneratedAssumption &&
							!isSpecificationWordSpanishAmbiguous &&

							( existingSpecificationItem.hasSpecificationCompoundCollection() ||

							( userSpecificationItem != null &&

							( userSpecificationItem.hasSpecificationCompoundCollection() ||
							!userSpecificationItem.hasSpecificationCollection() ) ) ) ) ||
							
							( !isExistingSpecificationConcludedAssumption &&
							!isValidRelationContextInContextWords( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) &&
							adminItem_.nContextWordsInContextWords( existingRelationContextNr, specificationWordItem ) > CommonVariables.nUserGeneralizationWords ) ) )
								// Need to copy context (1)
								existingCopyContextNr = existingRelationContextNr;
							else
								{
								if( isExistingSpecificationConcludedAssumption ||
								isSpecificationWordSpanishAmbiguous ||

								( isExistingSpecificationSelfGeneratedConclusion &&
								isPossessive &&
								isPossessiveUserSpecification ) )
									contextResult.contextNr = foundRelationContextNr;
								else
									{
									if( existingSpecificationItem.isSelfGeneratedAssumption() == isSelfGeneratedAssumption )
										{
										if( isPossessive ||
										isArchivedAssignment ||
										isSelfGeneratedAssumption )
											contextResult.contextNr = existingRelationContextNr;
										else
											{
											if( isExistingSpecificationSelfGeneratedConclusion )
												{
												if( isValidRelationContextInContextWords( false, existingRelationContextNr, specificationWordItem, relationWordItem ) )
													contextResult.contextNr = existingRelationContextNr;
												}
											else
												// Need to copy context (2)
												existingCopyContextNr = existingRelationContextNr;
											}
										}
									else
										{
										hasGeneralizationWordAnsweredSelfGeneratedQuestion = generalizationWordItem.hasAnsweredSelfGeneratedQuestion();

										// Possessive
										if( ( isPossessive &&

										( specificationCollectionNr == Constants.NO_COLLECTION_NR ||

										( hasGeneralizationWordAnsweredSelfGeneratedQuestion &&
										!isGeneralizationWordUserRelationWord ) ) ) ||

										// Not possessive
										( !isPossessive &&
										!hasRelationWordExistingRelationContext &&
										!isSelfGeneratedAssumption &&

										( isExistingSpecificationSelfGeneratedConclusion ||
										isValidRelationContextInContextWords( false, existingRelationContextNr, specificationWordItem, relationWordItem ) ) ) ||

										// Self-generated assumption
										( isGeneralizationWordUserRelationWord &&

										( isPossessive ||
										isSelfGeneratedAssumption ) &&

										( !hasGeneralizationWordAnsweredSelfGeneratedQuestion ||
										hasRelationWordExistingRelationContext ||

										( isPossessive &&
										!isSpecificationWordSpanishAmbiguous &&
										isValidRelationContextInContextWords( true, existingRelationContextNr, specificationWordItem, relationWordItem ) ) ) ) )
											contextResult.contextNr = existingRelationContextNr;
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
			if( hasRelationWordConfirmedSpecification &&
			isSelfGeneratedAssumption &&
			existingSpecificationItem != null &&
			existingSpecificationItem.hasRelationContext() &&
			existingRelationContextNr != contextResult.contextNr )
				{
				// Both existing specification and wanted specification are assumptions
				if( existingSpecificationItem.isSelfGeneratedAssumption() )
					contextResult.contextNr = existingRelationContextNr;
				else
					// Need to copy context (3)
					existingCopyContextNr = existingRelationContextNr;
				}
			else
				{
				foundContextNr = foundContextItem.contextNr();

				if( isAssignment &&
				!isInactiveAssignment &&
				!isArchivedAssignment &&
				isPossessive &&
				// Active assignment doesn't exist
				generalizationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, true, foundContextNr, specificationWordItem ) == null )
					{
					// Dynamic semantic ambiguity
					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != Constants.RESULT_OK )
						return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				else
					{
					if( isSelfGeneratedAssumption &&
					existingSpecificationItem != null &&

					( !isPossessive ||
					specificationCollectionNr == Constants.NO_COLLECTION_NR ) &&

					userSpecificationItem != null &&
					userSpecificationItem.isAssignment() &&
					existingSpecificationItem.hasRelationContext() &&
					existingSpecificationItem.isSelfGeneratedAssumption() )
						contextResult.contextNr = existingSpecificationItem.relationContextNr();
					else
						contextResult.contextNr = foundContextNr;
					}
				}
			}

		if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_.highestContextNrInAllContextWords() ) >= Constants.MAX_CONTEXT_NR )
				return adminItem_.startContextResultSystemError( 1, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;

			if( existingCopyContextNr > Constants.NO_CONTEXT_NR )
				{
				if( copyContext( existingCopyContextNr, contextResult.contextNr ) != Constants.RESULT_OK )
					return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to copied a context" );

				contextResult.copiedRelationContextNr = existingCopyContextNr;
				}
			}

		return contextResult;
		}

	private CreateAndAssignResultType addSpecification( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		boolean hasDisplayedMoreSpecificNonExclusiveSpecification;
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
		ContextResultType contextResult = new ContextResultType();
		CreateAndAssignResultType createAndAssignCheckResult = new CreateAndAssignResultType();
		CreateAndAssignResultType createAndAssignReturnResult = new CreateAndAssignResultType();

		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given specification word is the administrator" );

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

			if( relationContextNr == Constants.NO_CONTEXT_NR ||

			( isPossessive &&
			!isSpecificationWordSpanishAmbiguous &&
			specificationCollectionNr == Constants.NO_COLLECTION_NR &&

			( nContextRelations == 0 ||
			!generalizationWordItem.isOlderItem() ) &&

			// Possessive user specification
			( existingSpecificationItem == null ||

			( isAssignment &&
			!isArchivedAssignment ) ||

			// Ignore context in other words
			( existingSpecificationItem.hasRelationContext() &&
			existingSpecificationItem.relationContextNr() != relationContextNr &&

			( !relationWordItem.hasConfirmedSpecification() ||
			!existingSpecificationItem.hasSpecificationCollection() ||
			existingSpecificationItem.isSelfGeneratedAssumption() ) ) ) ) ||

			// User assignment, and existing self-generated specification
			( isAssignment &&
			!isPossessive &&
			userSpecificationItem_ != null &&
			!userSpecificationItem_.isOlderItem() &&

			( existingSpecificationItem == null ||
			existingSpecificationItem.isOlderItem() ) ) )
				{
				if( ( contextResult = getRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionSpanishAmbiguous, isPossessive, ( firstJustificationItem != null && firstJustificationItem.isAssumptionJustification() ), specificationCollectionNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to get the relation context" );

				relationContextNr = contextResult.contextNr;
				}
			else
				{
				isRelationContextAlreadyDefined = true;

				// User assignment, and existing user assignment
				if( isAssignment &&
				!isPossessive &&
				userSpecificationItem_ != null &&
				existingSpecificationItem != null &&
				existingSpecificationItem.isUserAssignment() &&
				existingSpecificationItem.relationContextNr() != relationContextNr )
					relationContextNr = existingSpecificationItem.relationContextNr();
				}

			if( relationWordItem.addContext( isCompoundCollectionSpanishAmbiguous, relationWordTypeNr, specificationWordTypeNr, relationContextNr, specificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
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

			if( adminItem_.isSystemStartingUp() &&
			isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
				{
				if( ( createAndAssignReturnResult = adminItem_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" with authorization" );
				}
			else
				{
				if( ( createAndAssignReturnResult = generalizationWordItem.addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, contextResult.copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add an assignment specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}

			if( !CommonVariables.hasDisplayedWarning )
				{
				isArchivedAssignment_ = isArchivedAssignment;
				replacedAssignmentItem = createAndAssignReturnResult.replacedAssignmentItem;

				isDefinitionSpecification = ( !hasRelationWord &&
											!isQuestion &&
											adminItem_.isNounWordType( generalizationWordTypeNr ) &&
											adminItem_.isNounWordType( specificationWordTypeNr ) );

				if( ( createdSpecificationItem = createAndAssignReturnResult.createdSpecificationItem ) == null )
					{
					if( hasRelationWord &&
					!isSelfGenerated &&
					( foundSpecificationItem = createAndAssignReturnResult.foundSpecificationItem ) != null )
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
								if( createdSpecificationItem.hasSpecificationCollection() &&
								adminItem_.makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make an only-option-left assumption" );
								}
							else
								{
								if( !isPossessive )
									{
									if( isAssignment &&
									hasRelationWord &&
									// Collect generalization words of a proper name specification
									adminItem_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

									if( adminItem_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									hasDisplayedMoreSpecificNonExclusiveSpecification = generalizationWordItem.hasDisplayedMoreSpecificNonExclusiveSpecification();

									if( generalizationWordItem.hasFoundAnswerToQuestion() &&

									( hasDisplayedMoreSpecificNonExclusiveSpecification ||
									!specificationWordItem.isCompoundCollection( createdSpecificationItem.specificationCollectionNr() ) ) &&

									adminItem_.drawNegativeConclusionsFromAnsweredQuestions( hasDisplayedMoreSpecificNonExclusiveSpecification, createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw negative conclusions from answered questions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
								isArchivedAssignment ) &&

								// Draw a possessive definition conclusion from a specification-generalization 'part of' sentence
								// Not yet explained in the design
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, false, true, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
									return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
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
											if( generalizationWordItem.writeSelectedSpecification( false, true, ( foundAssignmentItem == null ? createAndAssignReturnResult.foundSpecificationItem : foundAssignmentItem ) ) != Constants.RESULT_OK )
												return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a non-proper-name specification of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( CommonVariables.writtenSentenceStringBuffer == null ||
											CommonVariables.writtenSentenceStringBuffer.length() == 0 )
												return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "Integrity error! I couldn't write the assignment with generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );

											if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( foundAssignmentItem == null ? Constants.INTERFACE_LISTING_SENTENCE_ASSIGNMENT_CONFLICTS_WITH_DEFINITION_SPECIFICATION : Constants.INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) != Constants.RESULT_OK )
												return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a header" );

											if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, CommonVariables.writtenSentenceStringBuffer, CommonVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
												return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a sentence" );
											}
										}
									}
								else
									{
									if( isPossessive &&

									( !isAssignment ||
									isArchivedAssignment ) &&

									// Draw a specification-generalization 'part of' conclusion from a possessive definition sentence
									// Not yet explained in the design
									addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, true, false, false, false, true, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					}

				if( !CommonVariables.hasDisplayedWarning &&

				( ( isAssignment &&
				// At this stage, selections must be stored - rather than executed. So, don't assign them
				!isSelection ) ||

				replacedAssignmentItem != null ) &&

				// Skip for example imperative verbs
				adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
					{
					if( ( createAndAssignCheckResult = assignSpecification( contextResult.isAmbiguousRelationContext, ( specificationWordItem != null && specificationWordItem.isAdjectiveAssignedOrEmpty() ), isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to assign the specification" );

					if( ( createdAssignmentItem = createAndAssignCheckResult.createdSpecificationItem ) != null )
						{
						if( !isArchivedAssignment_ ||
						createAndAssignReturnResult.createdSpecificationItem != null )
							createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;

						if( replacedAssignmentItem != null &&
						generalizationWordItem.replaceOrDeleteSpecification( false, replacedAssignmentItem, createdAssignmentItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to replace or delete an assignment in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( isNegative &&
						!isQuestion &&
						createdAssignmentItem.hasSpecificationCollection() &&
						generalizationWordItem.isUserGeneralizationWord &&
						adminItem_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem.specificationCollectionNr(), generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
						}

					if( !isSelfGenerated &&
					( tempSpecificationItem = ( createdAssignmentItem == null ? createAndAssignCheckResult.foundSpecificationItem : createdAssignmentItem ) ) != null )
						{
						userSpecificationItem_ = tempSpecificationItem;

						if( isArchivedAssignment_ &&
						!isQuestion )
							simpleUserSpecificationItem_ = tempSpecificationItem;
						}
					}

				if( !CommonVariables.hasDisplayedWarning &&
				!isExclusiveSpecification &&
				!isSpecificationGeneralization &&
				replacedAssignmentItem == null &&
				specificationWordItem != null &&
				userSpecificationItem_ != null &&

				( !isSelfGenerated ||

				( isDefinitionSpecification &&
				isEveryGeneralization &&
				!isPossessive &&
				!userSpecificationItem_.hasSpecificationCollection() ) ) )
					{
					if( isQuestion )
						{
						if( hasRelationWord &&

						( createdAssignmentItem != null ||

						( createdSpecificationItem != null &&
						!generalizationWordItem.hasDisplayedMoreSpecificQuestion() ) ) )
							{
							if( adminItem_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, ( createdAssignmentItem == null ? createdSpecificationItem : createdAssignmentItem ), generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make a suggestive question assumption about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}
					else
						{
						if( isDefinitionSpecification &&
						adminItem_.drawSpecificationGeneralizationConclusion( isArchivedAssignment, isPossessive, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a specification generalization conclusion" );
						}
					}
				}
			}

		return createAndAssignReturnResult;
		}

	private ShortResultType checkCollectionInCollectionWords( int collectionNr, WordItem collectionWordItem, WordItem commonWordItem )
		{
		short existingPairCollectionOrderNr = Constants.NO_ORDER_NR;
		short foundCollectionOrderNr = Constants.NO_ORDER_NR;
		int nWords = 0;
		WordItem currentCollectionWordItem;
		ShortResultType shortResult = new ShortResultType();

		if( collectionNr <= Constants.NO_COLLECTION_NR )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given collection number is undefined" );

		if( collectionWordItem == null )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given collection word item is undefined" );

		if( ( currentCollectionWordItem = CommonVariables.firstCollectionWordItem ) == null )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given first collection word item is undefined" );

		// Do for all collection words
		do	{
			if( ( foundCollectionOrderNr = currentCollectionWordItem.collectionOrderNr( collectionNr, collectionWordItem, commonWordItem ) ) > Constants.NO_ORDER_NR )
				{
				// Found existing collection pair
				if( ++nWords == 2 )
					existingPairCollectionOrderNr = foundCollectionOrderNr;
				}
			}
		while( existingPairCollectionOrderNr == Constants.NO_ORDER_NR &&
		( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );

		shortResult.shortValue = existingPairCollectionOrderNr;
		return shortResult;
		}


	// Constructor

	protected AdminSpecification( AdminItem adminItem )
		{
		// Private constructed variables

		isArchivedAssignment_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		simpleUserSpecificationItem_ = null;
		userSpecificationItem_ = null;

		linkedGeneralizationWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected assignment methods

	protected byte assignSelectionSpecification( SelectionItem assignmentSelectionItem )
		{
		if( assignmentSelectionItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given assignment selection item is undefined" );

		if( assignSpecification( false, assignmentSelectionItem.isAssignedOrEmpty(), assignmentSelectionItem.isInactiveAssignment(), assignmentSelectionItem.isArchivedAssignment(), assignmentSelectionItem.isNegative(), false, assignmentSelectionItem.isPossessive(), assignmentSelectionItem.isSpecificationGeneralization(), assignmentSelectionItem.isUniqueUserRelation(), assignmentSelectionItem.assumptionLevel(), assignmentSelectionItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, assignmentSelectionItem.relationWordTypeNr(), assignmentSelectionItem.generalizationContextNr(), assignmentSelectionItem.specificationContextNr(), assignmentSelectionItem.relationContextNr(), Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, assignmentSelectionItem.nContextRelations(), null, assignmentSelectionItem.generalizationWordItem(), assignmentSelectionItem.specificationWordItem(), assignmentSelectionItem.specificationString() ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign a specification" );

		return Constants.RESULT_OK;
		}

	protected CreateAndAssignResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( adminItem_.isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			return adminItem_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

		return generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null );
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

	protected ContextResultType getGeneralizationContext( boolean isArchivedAssignment, boolean isPossessive, boolean isQuestion, boolean isUserSentence, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		ContextResultType contextResult = new ContextResultType();
		boolean hasFoundRelationContext;
		boolean hasFoundRelationWordInThisList;
		boolean hasSameNumberOrMoreRelationWords;
		boolean isSkippingThisContext = false;
		int currentRelationContextNr;
		int foundRelationContextNr;
		ContextItem currentRelationContextItem;
		ReadItem relationWordReadItem = null;
		SpecificationItem foundSpecificationItem;
		WordItem currentContextWordItem;
		WordItem currentRelationWordItem = relationWordItem;
		WordItem firstContextWordItem = CommonVariables.firstContextWordItem;

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem != null )
			{
			if( ( relationWordReadItem = startRelationReadItem.firstRelationWordReadItem() ) == null )
				return adminItem_.startContextResultError( 1, moduleNameString_, "The read word of the first relation word is undefined" );

			currentRelationWordItem = relationWordReadItem.readWordItem();
			}

		if( currentRelationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "I couldn't find any relation word" );

		if( ( currentRelationContextItem = currentRelationWordItem.firstActiveContextItem() ) != null )
			{
			// Do for all relation context items in the first relation context word
			do	{
				currentRelationContextNr = currentRelationContextItem.contextNr();

				if( currentRelationWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem ) )
					{
					hasFoundRelationWordInThisList = false;
					isSkippingThisContext = false;

					if( firstContextWordItem != null )
						{
						currentContextWordItem = firstContextWordItem;

						// Do for all context words, either in the current relation list or outside this list
						do	{
							foundSpecificationItem = ( isUserSentence ? null : generalizationWordItem.firstActiveAssignmentOrSpecificationItem( true, false, isPossessive, Constants.NO_QUESTION_PARAMETER, specificationWordItem ) );

							if( foundSpecificationItem == null ||
							!foundSpecificationItem.isSelfGeneratedConclusion() )
								{
								if( relationWordReadItem != null )
									hasFoundRelationWordInThisList = relationWordReadItem.hasFoundRelationWordInThisList( currentContextWordItem );

								hasFoundRelationContext = currentContextWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem );

								// Word is one of the relation words in this list, but doesn't have current context
								if( ( !hasFoundRelationContext &&
								hasFoundRelationWordInThisList ) ||

								// Word is in not current list of relation words, but has current context
								( hasFoundRelationContext &&
								!hasFoundRelationWordInThisList ) )
									isSkippingThisContext = true;
								}
							}
						while( !isSkippingThisContext &&
						( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
						}

					// The relation words in the list contain this context exclusively. (So, no other words)
					if( !isSkippingThisContext )
						contextResult.contextNr = currentRelationContextNr;
					}
				else
					{
					if( isUserSentence &&
					!currentRelationContextItem.isOlderItem() )
						contextResult.contextNr = currentRelationContextNr;
					}
				}
			while( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
			( currentRelationContextItem = currentRelationContextItem.nextContextItem() ) != null );
			}

		if( !isQuestion &&
		contextResult.contextNr == Constants.NO_CONTEXT_NR &&
		( foundSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, null ) ) != null )
			{
			if( ( foundRelationContextNr = foundSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR )
				{
				hasSameNumberOrMoreRelationWords = ( adminItem_.nContextWordsInContextWords( foundRelationContextNr, specificationWordItem ) >= CommonVariables.nUserRelationWords );

				if( hasSameNumberOrMoreRelationWords ||
				generalizationWordItem.isUserRelationWord )
					{
					if( foundSpecificationItem.isExclusiveSpecification() )
						{
						// Already existing static (exclusive) semantic ambiguity
						contextResult.isAmbiguousRelationContext = true;
						contextResult.contextNr = foundRelationContextNr;
						}
					else
						{
						if( isUserSentence )
							{
							if( !isSkippingThisContext )
								{
								contextResult.contextNr = foundRelationContextNr;

								if( !foundSpecificationItem.isActiveAssignment() )
									{
									// Static (exclusive) semantic ambiguity
									if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) != Constants.RESULT_OK )
										return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );

									contextResult.isAmbiguousRelationContext = true;
									}
								}
							}
						else
							{
							contextResult.contextNr = foundRelationContextNr;

							if( hasSameNumberOrMoreRelationWords &&
							// Try to find the relation context of a possessive reversible conclusion
							( contextResult = findPossessiveReversibleConclusionRelationContextOfInvolvedWords( isPossessive, nContextRelations, contextResult.contextNr, foundSpecificationItem, relationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to find a possessive reversible conclusion relation context of involved words" );
							}
						}
					}
				}
			}

		if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_.highestContextNrInAllContextWords() ) >= Constants.MAX_CONTEXT_NR )
				return adminItem_.startContextResultSystemError( 1, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		return contextResult;
		}

	protected byte addUserSpecifications( boolean isAction, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short imperativeVerbParameter, int generalizationContextNr, int specificationContextNr, ReadItem generalizationReadItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		boolean isGeneralizationProperName;
		boolean isGeneralizationReasoningWordType;
		boolean isRelationWord;
		boolean isSpecificationWord;
		boolean hasAddedSpecification = false;
		boolean hasRelationWord = ( startRelationReadItem != null );
		boolean hasLinkedPossessiveDeterminer = false;
		boolean isAmbiguousRelationContext = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
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
		CompoundResultType compoundResult;
		ContextResultType contextResult;

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
						if( ( contextResult = getGeneralizationContext( isArchivedAssignment, isPossessive, isQuestion, true, nContextRelations, generalizationWordItem, currentSpecificationWordItem, null, startRelationReadItem ) ).result != Constants.RESULT_OK )
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

							if( isSelection &&
							adminItem_.createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a value selection item" );

							// Value, but no relation
							if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
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
									if( previousSpecificationWordItem != null &&
									currentSpecificationWordItem != null &&
									currentReadItem.isMatchingReadWordTypeNr( previousSpecificationWordTypeNr ) &&

									// Collect specification words
									collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to collect specification words" );

									if( currentSpecificationWordTypeNr != Constants.WORD_TYPE_PREPOSITION )
										{
										previousSpecificationWordItem = currentSpecificationWordItem;
										previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
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
											if( addSpecification( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification without relation to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}

										// Start reasoning
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
												if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, ( currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE ), questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, null ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( !CommonVariables.hasDisplayedWarning )
													{
													if( isGeneralizationProperName )
														{
														if( isSpanishCurrentLanguage &&
														!isExclusiveSpecification &&
														generalizationWordItem.isUserGeneralizationWord &&

														// Draw simple negative Spanish conclusions
														adminItem_.drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														// Definition specification
														if( ( compoundResult = adminItem_.drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a primary specification substitution conclusion about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

														compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
														}
													}
												}

											if( isGeneralizationProperName &&
											!CommonVariables.hasDisplayedWarning &&
											CommonVariables.nUserRelationWords == 1 &&
											startRelationReadItem != null &&
											( firstRelationReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null &&
											( relationWordItem = firstRelationReadItem.readWordItem() ) != null &&

											adminItem_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, firstRelationReadItem.wordTypeNr(), specificationContextNr, userRelationContextNr_, generalizationWordItem, currentSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with one relation word, from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
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
					!isQuestion &&
					currentSpecificationWordItem != null &&
					linkedSpecificationWordItem != null &&

					( isRelationWord ||
					isSpecificationWord ) &&

					( !hasRelationWord ||
					linkedSpecificationWordItem != currentSpecificationWordItem ) )
						{
						if( isGeneralizationProperName &&
						!isValueSpecificationWord &&
						// Skip adjectives
						adminItem_.isNounWordType( linkedSpecificationWordTypeNr ) &&
						adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationWord ? currentSpecificationWordItem : null ) ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );

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
							CommonVariables.nUserRelationWords > 1 &&

							adminItem_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, specificationContextNr, userRelationContextNr_, generalizationWordItem, linkedSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a possessive reversible conclusion with multiple relation words, from generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							if( !isSelection &&
							currentReadItem.isAdjectiveAssigned() &&
							InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem.readWordTypeString(), Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, currentReadItem.wordOrderNr(), Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
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

		if( isGeneralizationProperName &&
		!isQuestion &&
		!isSelection )
			{
			if( hasRelationWord )
				{
				if( recalculateAssumptionsInTouchedWords() != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumptions of all touched words" );
				}
			else
				{
				if( generalizationWordItem.recalculateAssumptionsInWord() != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumptions of generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isExclusiveGeneralization = false;
		SpecificationItem foundSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem previousGeneralizationWordItem = null;
		WordItem previousSpecificationWordItem = null;
		BoolResultType boolResult;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
			specificationCollectionNr = specificationWordItem.compoundCollectionNr( specificationWordTypeNr );

		if( ( currentGeneralizationWordItem = CommonVariables.firstSpecificationWordItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The first specification word is undefined" );

		// Do for all specification words
		do	{
			if( currentGeneralizationWordItem != generalizationWordItem &&
			currentGeneralizationWordItem.hasWordType( false, generalizationWordTypeNr ) &&
			// Try to find matching specification word
			( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, false, isPossessive, Constants.NO_QUESTION_PARAMETER, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ) != null )
				{
				// Relation word of a generalization word: proper name
				if( ( boolResult = currentGeneralizationWordItem.findGeneralization( true, generalizationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find a generalization item" );

				// Generalization not found
				if( !boolResult.booleanValue )
					{
					// Languages and users
					if( foundSpecificationItem.isExclusiveGeneralization() ||

					( isAssignment &&
					!isPossessive &&
					!isSpecificationWordSpanishAmbiguous &&
					foundSpecificationItem.isActiveAssignment() ) )
						isExclusiveGeneralization = true;

					previousGeneralizationWordItem = currentGeneralizationWordItem;
					previousSpecificationWordItem = foundSpecificationItem.specificationWordItem();
					}
				}
			}
		// Continue search to get the most recent generalization word
		while( ( currentGeneralizationWordItem = currentGeneralizationWordItem.nextSpecificationWordItem ) != null );

		if( previousGeneralizationWordItem != null &&
		collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, specificationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalization words \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" and \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		return Constants.RESULT_OK;
		}

	protected SpecificationItem simpleUserSpecificationItem()
		{
		return simpleUserSpecificationItem_;
		}

	protected SpecificationItem userSpecificationItem()
		{
		return userSpecificationItem_;
		}

	protected CreateAndAssignResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNameEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkipDrawingSpanishAmbiguousSubstitutionConclusion, boolean isSpecificationGeneralization, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean isUserGeneralizationWord;
		boolean hasDisplayedIntegrityWarning = false;
		boolean hasFoundRelationContext = false;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasRelationWordConfirmedSpecification = false;
		boolean isAssumption = adminItem_.isAssumption( assumptionJustificationTypeNr );
		short justificationTypeNr = assumptionJustificationTypeNr;
		JustificationItem createdJustificationItem = null;
		JustificationItem foundJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();
		JustificationResultType justificationResult;

		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( primarySpecificationItem != null &&
		primarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given primary specification item is hidden" );

		if( anotherPrimarySpecificationItem != null &&
		anotherPrimarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given another primary specification item is hidden" );

		if( secondarySpecificationItem != null &&
		secondarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given secondary specification item is hidden" );

		if( anotherSecondarySpecificationItem != null &&
		anotherSecondarySpecificationItem.isHiddenSpanishSpecification() )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given another secondary specification item is hidden" );

		// Given assumption level has no value
		if( assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
			{
			// Calculate assumption level
			assumptionLevel = adminItem_.assumptionGrade( ( anotherPrimarySpecificationItem != null ), hasFeminineOrMasculineProperNameEnding, ( primarySpecificationItem != null && primarySpecificationItem.isGeneralizationProperName() && primarySpecificationItem.isPossessive() ), ( primarySpecificationItem != null && primarySpecificationItem.isQuestion() ), assumptionJustificationTypeNr );

			if( primarySpecificationItem != null )
				assumptionLevel += primarySpecificationItem.assumptionLevel();

			if( anotherPrimarySpecificationItem != null )
				assumptionLevel += anotherPrimarySpecificationItem.assumptionLevel();

			if( secondarySpecificationItem != null )
				assumptionLevel += secondarySpecificationItem.assumptionLevel();

			if( anotherSecondarySpecificationItem != null )
				assumptionLevel += anotherSecondarySpecificationItem.assumptionLevel();

			// No assumption level: So, it is a conclusion
			if( assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
				{
				if( !adminItem_.isConclusion( conclusionJustificationTypeNr ) )
					return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given conclusion justification type isn't a conclusion" );

				// Conclusion
				isAssumption = false;
				justificationTypeNr = conclusionJustificationTypeNr;
				}
			}

		if( ( justificationResult = generalizationWordItem.addJustification( hasFeminineOrMasculineProperNameEnding, isForcingNewJustification, false, justificationTypeNr, Constants.NO_ORDER_NR, CommonVariables.currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a justification" );

		createdJustificationItem = justificationResult.createdJustificationItem;
		foundJustificationItem = justificationResult.foundJustificationItem;

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == null ? foundJustificationItem : createdJustificationItem ) ) == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I couldn't find or create a justification" );

		if( ( createAndAssignResult = addSpecification( isAssignment, false, false, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION || justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION ), false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to create a specification" );

		if( !CommonVariables.hasDisplayedWarning )
			{
			createdSpecificationItem = createAndAssignResult.createdSpecificationItem;

			if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
				hasFoundRelationContext = foundSpecificationItem.hasRelationContext();

			if( createdSpecificationItem == null )
				{
				if( foundSpecificationItem == null )
					return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I couldn't find or create a specification" );

				if( foundSpecificationItem.isUserSpecification() )
					{
					hasDisplayedIntegrityWarning = true;
					if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a user specification, whilst trying to create a self-generated specification;\n\tSpecificationItem: " + foundSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + createdOrFoundJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
						return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I failed to write an interface warning" );
					}

				if( !hasDisplayedIntegrityWarning )
					{
					isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

					if( relationWordItem != null )
						hasRelationWordConfirmedSpecification = relationWordItem.hasConfirmedSpecification();

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

					( justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
					relationWordItem != null &&

					( ( isPossessive &&

					( ( !isUserGeneralizationWord &&
					!hasRelationWordConfirmedSpecification ) ||

					( isUserGeneralizationWord &&
					generalizationWordItem.hasConfirmedSpecification() &&
					foundSpecificationItem.hasOnlyOneRelationWord() ) ) ) ||

					( !isPossessive &&
					!isUserGeneralizationWord &&
					hasRelationWordConfirmedSpecification &&
					!foundSpecificationItem.hasOnlyOneRelationWord() ) ) ) ) ) ) ) )
						{
						if( processJustification( hasFoundRelationContext, hasRelationWord, hasRelationWordConfirmedSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to process the created justification" );
						}
					}
				}
			else
				{
				if( hasFoundRelationContext &&
				foundSpecificationItem != null &&
				!generalizationWordItem.hasConfirmedSpecification() &&
				foundSpecificationItem.relationContextNr() == createdSpecificationItem.relationContextNr() )
					return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "Integrity error! A specification was created in generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" while a specification was found as well with the same relation context" );
				}

			if( !hasDisplayedIntegrityWarning &&
			!isNegative &&
			!isPartOf &&
			adminItem_.isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					createdSpecificationItem != null &&

					// Draw a unique relation conclusion
					// Not yet explained in the design
					addSelfGeneratedSpecification( false, false, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, false, true, false, true, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, createdSpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated unique specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					if( ( createdJustificationItem != null ||
					createdSpecificationItem != null ) &&

					// Typically for the Spanish language
					!isSkipDrawingSpanishAmbiguousSubstitutionConclusion )
						{
						if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, false, isArchivedAssignment, isExclusiveSpecification, true, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		createAndAssignResult.createdJustificationItem = createdJustificationItem;
		return createAndAssignResult;
		}
	};

/*************************************************************************
 *	"How amazing are the deeds of the Lord!
 *	All who delight in him should ponder them." (Psalm 111:2)
 *************************************************************************/
