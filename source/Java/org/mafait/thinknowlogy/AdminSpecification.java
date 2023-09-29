/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		Creating specification and assignment structures
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

class AdminSpecification
	{
	// Private constructed variables

	private boolean isArchivedAssignment_ = false;
	private boolean isChineseCurrentLanguage_ = false;

	private short doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
	private short linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private int nPreviousChineseUserRelationWords_ = 0;
	private int userRelationContextNr_ = Constants.NO_CONTEXT_NR;

	private ReadItem previousChineseStartRelationReadItem_ = null;
	private ReadItem previousChineseEndRelationReadItem_ = null;

	private SpecificationItem simpleUserSpecificationItem_ = null;
	private SpecificationItem userSpecificationItem_ = null;

	private WordItem linkedGeneralizationWordItem_ = null;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private boolean isAuthorizationRequiredForChanges( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		return ( generalizationWordItem != null &&
				specificationWordItem != null &&
				adminItem_.isSystemStartingUp() &&

				( generalizationWordItem.isAuthorizationRequiredForChanges() ||
				specificationWordItem.isAuthorizationRequiredForChanges() ) );
		}

	private static boolean isValidContext( boolean isPossessive, int relationContextNr, WordItem specificationWordItem, WordItem relationWordItem )
		{
		WordItem currentContextWordItem;

		if( relationContextNr == Constants.NO_CONTEXT_NR ||
		specificationWordItem == null ||
		relationWordItem == null )
			return false;

		if( !relationWordItem.hasContextInWord( relationContextNr ) &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( !currentContextWordItem.isUserGeneralizationWord &&
				!currentContextWordItem.isUserRelationWord &&
				currentContextWordItem.firstAssignmentOrSpecificationItem( false, isPossessive, relationContextNr, specificationWordItem ) != null )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return true;
		}

	private static int collectionNrByCompoundGeneralizationWordInCollectionWords( boolean isExclusiveSpecification, boolean isTryingSmartSearch, short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int collectionNr;
		SpecificationItem currentSpecificationItem;
		WordItem currentCollectionWordItem;
		WordItem currentSpecificationWordItem;

		if( compoundGeneralizationWordItem != null )
			{
			// Try smart search first
			if( isTryingSmartSearch &&
			( currentSpecificationItem = compoundGeneralizationWordItem.firstExclusiveSpecificationItem( false ) ) != null )
				{
				// Do for all exclusive specifications of this word (skip adjectives)
				do	{
					if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
					( collectionNr = currentSpecificationWordItem.collectionNrByCompoundGeneralizationWordInWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
						return collectionNr;
					}
				while( ( currentSpecificationItem = currentSpecificationItem.nextExclusiveSpecificationItem( false ) ) != null );
				}

			// Not found. Do full search
			if( ( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
				{
				// Do for all collection words
				do	{
					if( currentCollectionWordItem != compoundGeneralizationWordItem &&
					( collectionNr = currentCollectionWordItem.collectionNrByCompoundGeneralizationWordInWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
						return collectionNr;
					}
				while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
				}
			}

		return Constants.NO_COLLECTION_NR;
		}

	private static WordItem contextUsedInUserSpecificationOfWordItem( boolean isPossessive, int specificationCollectionNr, int relationContextNr, WordItem specificationWordItem )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		// Specification collection number may be undefined

		if( specificationWordItem != null &&
		( currentGeneralizationItem = specificationWordItem.firstSpecificationGeneralizationItem( true ) ) != null )
			{
			// Do for all generalization specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null &&
				currentGeneralizationWordItem.firstUserSpecificationItem( false, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != null )
					return currentGeneralizationWordItem;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		return null;
		}

	private byte addUserSpecificationWithRelation( boolean isAction, boolean isAssignedOrClear, boolean isAssignment, boolean isCharacteristicFor, boolean isChineseReasoning, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpanishCurrentLanguage, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, short imperativeVerbParameter, short questionParameter, short selectionLevel, short selectionListNr, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, int nUserRelationWords, ReadItem startRelationReadItem, ReadItem endRelationReadItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isChineseReversedImperativeNoun;
		boolean isFirstComparisonPart = ( selectionListNr == Constants.ADMIN_CONDITION_LIST );
		boolean isFirstRun = true;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		short relationWordTypeNr;
		int specificationCollectionNr;
		ReadItem currentRelationReadItem = startRelationReadItem;
		SpecificationItem chineseUserSpecificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem mostRecentContextWord;
		WordItem previousRelationWordItem = null;
		WordItem relationWordItem;

		if( startRelationReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given start relation read item is undefined" );

		if( endRelationReadItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given end relation read item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		do	{
			if( currentRelationReadItem.isPreposition() )
				prepositionParameter = currentRelationReadItem.wordParameter();
			else
				{
				if( currentRelationReadItem.isRelationWord() )
					{
					if( ( relationWordItem = currentRelationReadItem.readWordItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I found an undefined read word" );

					relationWordItem.addMyWordToTouchedDuringCurrentSentenceList();

					// Typical for Chinese: Relation before specification
					if( specificationWordItem != relationWordItem )
						{
						if( generalizationWordItem == relationWordItem )
							{
							if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_END ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
							}
						else
							{
							relationWordTypeNr = currentRelationReadItem.wordTypeNr();
							specificationCollectionNr = Constants.NO_COLLECTION_NR;

							if( nUserRelationWords == 1 &&
							( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationWordItem ) ) != null &&
							foundSpecificationItem.hasRelationContext() &&
							( mostRecentContextWord = mostRecentContextWordInContextWords( foundSpecificationItem.relationContextNr() ) ) != null &&
							mostRecentContextWord != relationWordItem )
								{
								previousRelationWordItem = mostRecentContextWord;

								if( foundSpecificationItem.hasNonCompoundSpecificationCollection() &&
								!foundSpecificationItem.isSpecificationWordSpanishAmbiguous() )
									specificationCollectionNr = foundSpecificationItem.specificationCollectionNr();
								}

							if( previousRelationWordItem != null &&
							// Collect relation words
							collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

							if( selectionListNr == Constants.NO_LIST_NR )
								{
								// Typical for Chinese
								if( isChineseReasoning )
									{
									if( userSpecificationItem_.hasCompoundSpecificationCollection() )
										{
										// Make a Chinese exclusive specification substitution assumption (non-possessive)
										if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

										if( ( chineseUserSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
										!chineseUserSpecificationItem.isOlderItem() &&
										// Older possessive user specification doesn't exist
										relationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) == null &&
										// Avoid idle call
										relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, specificationWordItem, generalizationWordItem ) != null &&
										// Make a Chinese exclusive specification substitution assumption (possessive)
										adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									{
									if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, null, generalizationWordItem, specificationWordItem, relationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

									if( !isQuestion &&
									!isSpanishCurrentLanguage &&

									( isFirstRun ||

									( userSpecificationItem_ != null &&
									userSpecificationItem_.hasNonCompoundSpecificationCollection() ) ) &&

									// Avoid idle call
									specificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null &&
									// Draw a specification substitution conclusion or ask a question
									adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									isFirstRun = false;
									}
								}
							else
								{
								// Typical for Chinese
								isChineseReversedImperativeNoun = ( isChineseCurrentLanguage_ &&
																	relationWordItem.isChineseReversedImperativeNoun() );

								if( adminItem_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem.isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, ( isChineseReversedImperativeNoun ? relationWordTypeNr : specificationWordTypeNr ), ( isChineseReversedImperativeNoun ? specificationWordTypeNr : relationWordTypeNr ), generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, generalizationWordItem, ( isChineseReversedImperativeNoun ? relationWordItem : specificationWordItem ), ( isChineseReversedImperativeNoun ? specificationWordItem : relationWordItem ), null ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to create a selection part having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

								isFirstComparisonPart = false;
								}

							previousRelationWordItem = relationWordItem;
							}
						}
					}
				}
			}
		while( currentRelationReadItem != endRelationReadItem &&
		( currentRelationReadItem = currentRelationReadItem.nextReadItem() ) != null &&
		// No conflict found
		!GlobalVariables.hasDisplayedWarning );

		return Constants.RESULT_OK;
		}

	private byte collectGeneralizationWords( boolean isExclusiveGeneralization, short generalizationWordTypeNr, short commonWordTypeNr, WordItem previousGeneralizationWordItem, WordItem currentGeneralizationWordItem, WordItem newCommonWordItem )
		{
		boolean hasFoundCollection = false;
		int generalizationCollectionNr = Constants.NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;

		if( previousGeneralizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given previous generalization word item is undefined" );

		if( currentGeneralizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given current generalization word item is undefined" );

		if( previousGeneralizationWordItem == currentGeneralizationWordItem )
			return adminItem_.startError( 1, moduleNameString_, "The given previous and current generalization word items are the same word" );

		if( newCommonWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given new common word item is undefined" );

		if( ( generalizationCollectionNr = previousGeneralizationWordItem.collectionNr( newCommonWordItem ) ) == Constants.NO_COLLECTION_NR )
			generalizationCollectionNr = currentGeneralizationWordItem.collectionNr( newCommonWordItem );

		if( generalizationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			if( ( boolResult = currentGeneralizationWordItem.findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" is collected with word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			// Each collection comes in pairs
			if( ( collectionResult = previousGeneralizationWordItem.createCollectionItem( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, currentGeneralizationWordItem, newCommonWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with current word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( generalizationCollectionNr == Constants.NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			if( currentGeneralizationWordItem.createCollectionItem( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect current word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with previous word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( previousGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in previous word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( currentGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in new word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
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

		if( ( collectionNr = previousRelationWordItem.collectionNr( commonWordItem ) ) == Constants.NO_COLLECTION_NR )
			collectionNr = currentRelationWordItem.collectionNr( commonWordItem );

		if( collectionNr > Constants.NO_COLLECTION_NR )
			{
			if( ( boolResult = previousRelationWordItem.findCollection( false, currentRelationWordItem, commonWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + previousRelationWordItem.anyWordTypeString() + "\" is collected with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			// Each collection comes in pairs
			if( ( collectionResult = previousRelationWordItem.createCollectionItem( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous word \"" + previousRelationWordItem.anyWordTypeString() + "\" with current word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

			if( collectionNr == Constants.NO_COLLECTION_NR )
				collectionNr = collectionResult.createdCollectionNr;

			if( currentRelationWordItem.createCollectionItem( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect current word \"" + currentRelationWordItem.anyWordTypeString() + "\" with previous word \"" + previousRelationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte collectSpecificationWords( boolean isExclusiveSpecification, boolean isPossessive, boolean isQuestion, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem compoundGeneralizationWordItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		boolean hasFoundCollection = false;
		boolean isAllowingDifferentCommonWord;
		int specificationCollectionNr = Constants.NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;

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
			if( ( specificationCollectionNr = generalizationWordItem.userSpecificationCollectionNr() ) == Constants.NO_COLLECTION_NR &&
			( specificationCollectionNr = previousSpecificationWordItem.collectionNr( generalizationWordItem ) ) == Constants.NO_COLLECTION_NR &&
			( specificationCollectionNr = currentSpecificationWordItem.collectionNr( generalizationWordItem ) ) == Constants.NO_COLLECTION_NR &&

			( specificationWordTypeNr != Constants.WORD_TYPE_NOUN_PLURAL ||
			generalizationWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR ) &&

			( specificationCollectionNr = previousSpecificationWordItem.collectionNr( specificationWordTypeNr ) ) == Constants.NO_COLLECTION_NR )
				specificationCollectionNr = currentSpecificationWordItem.collectionNr( specificationWordTypeNr );
			}
		else
			{
			if( !isExclusiveSpecification ||
			generalizationWordItem == previousSpecificationWordItem ||
			!generalizationWordItem.isNounWordSpanishAmbiguous() )
				specificationCollectionNr = collectionNrByCompoundGeneralizationWordInCollectionWords( isExclusiveSpecification, ( isExclusiveSpecification && generalizationWordItem.isFeminineOrMasculineWord() ), specificationWordTypeNr, compoundGeneralizationWordItem );
			}

		if( !isPossessive &&
		specificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			isAllowingDifferentCommonWord = ( previousSpecificationWordItem != generalizationWordItem );

			if( ( boolResult = previousSpecificationWordItem.findCollection( isAllowingDifferentCommonWord, currentSpecificationWordItem, generalizationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" is collected with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			// Each collection comes in pairs
			if( ( collectionResult = previousSpecificationWordItem.createCollectionItem( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with current word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

			if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem.createCollectionItem( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect current word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with previous word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		generalizationWordItem.collectSpecificationsInWord( isExclusiveSpecification, isQuestion, specificationCollectionNr ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to collect specifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		return Constants.RESULT_OK;
		}

	private byte copyContext( int existingContextNr, int newSpanishAmbiguousCollectionNr, int newContextNr )
		{
		ContextItem currentContextItem;
		WordItem currentContextWordItem;

		if( ( currentContextWordItem = adminItem_.firstContextWordItem( existingContextNr ) ) != null )
			{
			// Do for all context words with existing relation context
			do	{
				if( ( currentContextItem = currentContextWordItem.contextItem( existingContextNr ) ) != null &&
				// Add copied context
				currentContextWordItem.addContext( currentContextItem.contextWordTypeNr(), currentContextItem.specificationWordTypeNr(), newContextNr, newSpanishAmbiguousCollectionNr, currentContextItem.specificationWordItem() ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a copied context to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( existingContextNr ) ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords( boolean isAssignment, boolean isArchivedAssignment, WordItem specificationWordItem )
		{
		SpecificationItem hiddenSpanishSpecificationItem;
		WordItem currentContextWordItem;
		WordItem hiddenSpanishSpecificationWordItem;

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( ( hiddenSpanishSpecificationItem = currentContextWordItem.firstUnhiddenSpanishSpecificationItem() ) != null )
					{
					if( !isAssignment )
						{
						currentContextWordItem.hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = true;

						// Write Spanish assumption
						if( currentContextWordItem.writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write a Spanish assumption about word \"" + currentContextWordItem.anyWordTypeString() + "\" that is not hidden anymore" );
						}

					if( ( hiddenSpanishSpecificationWordItem = hiddenSpanishSpecificationItem.specificationWordItem() ) != null &&
					// Draw Spanish proper noun part-of conclusions
					adminItem_.drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, hiddenSpanishSpecificationWordItem, currentContextWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte processJustification( boolean hasRelationWordConfirmedSpecification, boolean isOlderFoundSpecification, boolean isUserGeneralizationWord, short justificationTypeNr, JustificationItem createdOrFoundJustificationItem, JustificationItem firstJustificationItem, SpecificationItem foundSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundSpecificationCurrentCreationSentenceNr;
		boolean isRemovingPreviousJustifications = false;
		boolean isSpecificationWordSpanishAmbiguous;
		JustificationItem obsoleteJustificationItem = null;
		JustificationItem questionJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		WordItem specificationWordItem;

		if( createdOrFoundJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given created or found justification item is undefined" );

		if( firstJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given first justification item is undefined" );

		if( foundSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given found specification item is undefined" );

		if( primarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		hasFoundSpecificationCurrentCreationSentenceNr = foundSpecificationItem.hasCurrentCreationSentenceNr();
		isSpecificationWordSpanishAmbiguous = foundSpecificationItem.isSpecificationWordSpanishAmbiguous();

		if( relationWordItem != null )
			hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();

		switch( justificationTypeNr )
			{
			case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
				if( isUserGeneralizationWord )
					{
					if( relationWordItem != null &&
					generalizationWordItem.hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation() &&
					secondarySpecificationItem != null &&
					secondarySpecificationItem.isSelfGeneratedConclusion() &&
					( specificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
					( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), false, false, true, specificationWordItem, generalizationWordItem ) ) != null )
						// Test file: "You have confirmed at least one relation word"
						obsoleteJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem );
					else
						{
						// Typical for Spanish
						if( foundSpecificationItem.isHiddenSpanishSpecification() &&
						!foundSpecificationItem.isOlderItem() )
							obsoleteJustificationItem = foundSpecificationItem.firstReplacedSecondarySpecificationJustificationItem();
						}
					}

				break;

			case Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
				// Typical for Spanish
				if( isSpecificationWordSpanishAmbiguous )
					{
					if( !hasRelationWordConfirmedSpecification &&
					primarySpecificationItem.isUserAssignment() )
						obsoleteJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem );
					}
				else
					{
					if( isOlderFoundSpecification &&
					!isUserGeneralizationWord &&
					foundSpecificationItem.hasOnlyOneRelationWord() )
						isRemovingPreviousJustifications = true;
					}

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				if( foundSpecificationItem.hasAssumptionLevel() )
					{
					if( primarySpecificationItem.isCorrectedSpecification() )
						// Test file: "Correcting invalidated assumption (by knowledge)"
						obsoleteJustificationItem = foundSpecificationItem.firstReplacedPrimarySpecificationJustificationItem();
					else
						{
						if( !hasFoundSpecificationCurrentCreationSentenceNr &&
						// Typical for Spanish
						primarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&
						foundSpecificationItem.assumptionLevel() > createdOrFoundJustificationItem.justificationAssumptionLevel() &&
						firstJustificationItem.hasAdditionalDefinitionSpecification() )
							isRemovingPreviousJustifications = true;
						}
					}

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION:
				if( isOlderFoundSpecification &&
				foundSpecificationItem.hasAssumptionLevel() &&

				// Typical for Spanish
				( hasFoundSpecificationCurrentCreationSentenceNr ||
				primarySpecificationItem.hasRelationContext() ) &&

				firstJustificationItem.isOlderItem() &&

				( firstJustificationItem.isPrimaryQuestionSpecification() ||
				firstJustificationItem.justificationAssumptionLevel() > createdOrFoundJustificationItem.justificationAssumptionLevel() ) )
					isRemovingPreviousJustifications = true;

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				// Adjusted question
				if( isOlderFoundSpecification &&
				secondarySpecificationItem != null &&
				!primarySpecificationItem.isPossessive() &&
				primarySpecificationItem.isUserSpecification() &&

				// Typical for Spanish
				( !hasFoundSpecificationCurrentCreationSentenceNr ||
				secondarySpecificationItem.isUserSpecification() ) )
					{
					if( generalizationWordItem.hasFeminineOrMasculineProperNounEnding() )
						{
						if( !hasFoundSpecificationCurrentCreationSentenceNr &&
						( questionJustificationItem = foundSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION ) ) != null &&
						( assumptionSpecificationItem = questionJustificationItem.primarySpecificationItem() ) != null &&
						assumptionSpecificationItem.assumptionLevel() > primarySpecificationItem.assumptionLevel() )
							// Remove less certain justification from question
							isRemovingPreviousJustifications = true;
						}
					else
						// Remove obsolete justifications from adjusted compound question
						isRemovingPreviousJustifications = true;
					}
			}

		if( isRemovingPreviousJustifications )
			{
			if( hasFoundSpecificationCurrentCreationSentenceNr )
				{
				// Remove all previous justifications from found specification
				if( foundSpecificationItem.changeFirstJustification( false, createdOrFoundJustificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the found specification" );
				}
			else
				{
				// Copy and replace
				if( generalizationWordItem.copyAndReplaceSpecification( foundSpecificationItem.isAnsweredQuestion(), foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to copy and replace the found specification, with a different first justification item" );

				foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();
				}

			if( !foundSpecificationItem.isQuestion() &&
			// Recalculate assumption level of found specification
			foundSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );

			// Clear stored sentence buffer, because of recalculated assumption level
			foundSpecificationItem.clearStoredSentenceStringBuffer();
			}
		else
			{
			if( obsoleteJustificationItem == null )
				{
				if( !foundSpecificationItem.isReplacedOrDeletedItem() &&
				// Attach created justification to the found specification
				foundSpecificationItem.attachJustificationToSpecification( createdOrFoundJustificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to attach a justification to the found specification" );
				}
			else
				{
				if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to replace an obsolete justification item in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	private WordItem mostRecentContextWordInContextWords( int contextNr )
		{
		WordItem currentContextWordItem;
		WordItem mostRecentWordItem = null;

		if( ( currentContextWordItem = adminItem_.firstContextWordItem( contextNr ) ) != null )
			{
			// Do for all context words with given context
			do	mostRecentWordItem = currentContextWordItem;
			while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( contextNr ) ) != null );
			}

		return mostRecentWordItem;
		}

	private ContextResultType getRelationContextAdvanced( boolean hasRelationWordConfirmedSpecification, boolean isAssignment, boolean isArchivedAssignment, boolean isPossessive, boolean isSelfGeneratedAssumption, boolean isSpecificationWordSpanishAmbiguous, int specificationCollectionNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasCompoundUserSpecificationCollection = false;
		boolean hasExistingSpecificationCompoundCollection = false;
		boolean hasExistingSpecificationCurrentCreationSentenceNr = false;
		boolean hasExistingSpecificationOnlyOneRelationWord = false;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasNonCompoundUserSpecificationCollection = false;
		boolean hasRelationWordExistingRelationContext;
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean isCompoundCollectionSpanishAmbiguous = false;
		boolean isExistingArchivedAssignment = false;
		boolean isExistingHiddenSpanishSpecification = false;
		boolean isExistingOlderSpecification = false;
		boolean isExistingSpecificationSelfGeneratedAssumption = false;
		boolean isExistingUserSpecification = false;
		boolean isOlderUserSpecification = false;
		boolean isUserArchivedAssignment = false;
		boolean isUserAssignment = false;
		boolean isUserRelationWord;
		int existingRelationContextNr = Constants.NO_CONTEXT_NR;
		int existingSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		int foundContextNr;
		int foundRelationContextNr = Constants.NO_CONTEXT_NR;
		int nonCompoundSpanishSpecificationCollectionNr;
		int nUserRelationWords = GlobalVariables.nUserRelationWords;
		int spanishAmbiguousCopyCollectionNr = Constants.NO_COLLECTION_NR;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
		ContextItem foundContextItem = null;
		SpecificationItem existingSpecificationItem = null;
		SpecificationItem foundSpecificationItem;
		SpecificationItem tempSpecificationItem;
		ContextResultType contextResult = new ContextResultType();

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given relation word item is undefined" );

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
		isUserRelationWord = generalizationWordItem.isUserRelationWord;

		if( userSpecificationItem_ != null )
			{
			if( userSpecificationItem_.hasNonCompoundSpecificationCollection() )
				hasNonCompoundUserSpecificationCollection = true;
			else
				{
				if( userSpecificationItem_.hasCompoundSpecificationCollection() )
					hasCompoundUserSpecificationCollection = true;
				}

			if( userSpecificationItem_.isAssignment() )
				{
				isUserAssignment = true;

				if( userSpecificationItem_.isArchivedAssignment() )
					isUserArchivedAssignment = true;
				}

			isOlderUserSpecification = userSpecificationItem_.isOlderItem();
			userSpecificationCollectionNr = userSpecificationItem_.specificationCollectionNr();
			}

		if( isSpecificationWordSpanishAmbiguous &&
		isPossessive &&
		specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		specificationWordItem.isCompoundCollection( specificationCollectionNr ) )
			{
			isCompoundCollectionSpanishAmbiguous = true;
			existingSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, relationWordItem );
			}
		else
			{
			if( ( existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, isPossessive, specificationCollectionNr, specificationWordItem ) ) != null &&
			specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			!existingSpecificationItem.hasRelationContext() &&
			( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, relationWordItem ) ) != null )
				existingSpecificationItem = tempSpecificationItem;
			}

		if( isSpecificationWordSpanishAmbiguous &&
		existingSpecificationItem == null )
			existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, specificationWordItem );

		if( existingSpecificationItem != null )
			{
			hasExistingSpecificationCompoundCollection = existingSpecificationItem.hasCompoundSpecificationCollection();
			hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem.hasCurrentCreationSentenceNr();
			hasExistingSpecificationOnlyOneRelationWord = existingSpecificationItem.hasOnlyOneRelationWord();
			isExistingArchivedAssignment = existingSpecificationItem.isArchivedAssignment();
			isExistingHiddenSpanishSpecification = existingSpecificationItem.isHiddenSpanishSpecification();
			isExistingOlderSpecification = existingSpecificationItem.isOlderItem();
			isExistingSpecificationSelfGeneratedAssumption = existingSpecificationItem.isSelfGeneratedAssumption();
			isExistingUserSpecification = existingSpecificationItem.isUserSpecification();
			existingSpecificationCollectionNr = existingSpecificationItem.specificationCollectionNr();
			existingRelationContextNr = existingSpecificationItem.relationContextNr();
			}

		// Try to find relation context with same number of relation words as in the user sentence
		if( nUserRelationWords > 0 )
			foundContextItem = relationWordItem.contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : Constants.NO_COLLECTION_NR ), specificationWordItem );

		if( foundContextItem == null )
			{
			if( existingSpecificationItem == null )
				{
				if( ( foundContextNr = relationWordItem.contextNr( ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : Constants.NO_COLLECTION_NR ), specificationWordItem ) ) > Constants.NO_CONTEXT_NR &&

				// Typical for Chinese
				( generalizationWordItem.isFeminineWord() ||

				( GlobalVariables.nUserGeneralizationWords > 1 &&

				( generalizationWordItem.hasFeminineOrMasculineProperNounEnding() ||
				contextUsedInUserSpecificationOfWordItem( isPossessive, specificationCollectionNr, foundContextNr, specificationWordItem ) == null ) ) ||

				// Generalization word is user relation word
				( isUserRelationWord &&

				( isUserAssignment ||

				( hasNonCompoundUserSpecificationCollection &&
				isPossessive ) ) &&

				adminItem_.nContextWords( foundContextNr ) == 1 ) ) )
					contextResult.contextNr = foundContextNr;
				}
			else
				{
				// Find specification with found context word as relation word
				if( isSpecificationWordSpanishAmbiguous )
					{
					if( ( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, isPossessive, specificationWordItem ) ) != null )
						{
						if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&

						( isExistingHiddenSpanishSpecification ||
						!foundSpecificationItem.hasRelationContext() ) )
							foundRelationContextNr = existingRelationContextNr;
						else
							{
							if( !hasSpecificationCollection ||
							foundSpecificationItem.isUserSpecification() ||
							foundSpecificationItem.specificationCollectionNr() == specificationCollectionNr )
								foundRelationContextNr = foundSpecificationItem.relationContextNr();
							else
								{

								if( ( isSelfGeneratedAssumption ||
								existingSpecificationCollectionNr == specificationCollectionNr ) &&

								isExistingArchivedAssignment == isUserArchivedAssignment )
									contextResult.contextNr = existingRelationContextNr;
								}
							}
						}
					}
				else
					{
					if( isExistingArchivedAssignment == isArchivedAssignment )
						{
						if( existingRelationContextNr == Constants.NO_CONTEXT_NR )
							{
							if( ( foundSpecificationItem = generalizationWordItem.firstAssignmentItem( false, false, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
							foundSpecificationItem.hasOnlyOneRelationWord() )
								// Need to copy context (1)
								// Typical for Spanish
								// Spanish test file: "Juan es el padre de Pablo - José tiene 2 padres"
								contextResult.copiedRelationContextNr = foundSpecificationItem.relationContextNr();
							}
						else
							foundRelationContextNr = existingRelationContextNr;
						}
					}

				if( foundRelationContextNr == Constants.NO_CONTEXT_NR &&
				contextResult.contextNr == Constants.NO_CONTEXT_NR )
					{
					if( isSpecificationWordSpanishAmbiguous )
						{
						// Typical for Spanish
						if( existingRelationContextNr == Constants.NO_CONTEXT_NR )
							{
							if( ( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, isArchivedAssignment, false, isPossessive, specificationWordItem, relationWordItem ) ) != null )
								contextResult.contextNr = foundSpecificationItem.relationContextNr();
							}
						else
							{
							foundContextNr = relationWordItem.contextNr( ( isExistingOlderSpecification ? Constants.NO_COLLECTION_NR : specificationCollectionNr ), specificationWordItem );

							// Typical for Spanish
							if( ( !hasExistingSpecificationCurrentCreationSentenceNr &&
							isExistingArchivedAssignment == isUserArchivedAssignment ) ||

							( !isExistingOlderSpecification &&
							foundContextNr > Constants.NO_CONTEXT_NR ) ||

							( isExistingSpecificationSelfGeneratedAssumption &&
							!isUserAssignment ) )
								contextResult.contextNr = ( isExistingOlderSpecification ? existingRelationContextNr : foundContextNr );
							}
						}
					}
				else
					{
					if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						if( hasGeneralizationWordCurrentlyConfirmedSpecification )
							{
							if( !hasSpecificationCollection ||
							!isExistingOlderSpecification ||
							!isPossessive ||

							( hasCompoundUserSpecificationCollection &&
							!isExistingHiddenSpanishSpecification ) )
								{
								if( isValidContext( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) )
									contextResult.contextNr = existingRelationContextNr;
								else
									{
									if( isSpecificationWordSpanishAmbiguous )
										spanishAmbiguousCopyCollectionNr = specificationCollectionNr;

									// Need to copy context (2)
									contextResult.copiedRelationContextNr = existingRelationContextNr;
									}
								}
							else
								{
								// Typical for Spanish
								if( isExistingHiddenSpanishSpecification )
									// Need to copy context (3)
									contextResult.copiedRelationContextNr = existingRelationContextNr;
								}
							}
						else
							{
							if( isValidContext( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) )
								{
								hasRelationWordExistingRelationContext = relationWordItem.hasContextInWord( existingRelationContextNr );

								// Check for copy context (of older existing specification)
								if( isExistingOlderSpecification &&

								// Condition part 1
								( ( !hasRelationWordExistingRelationContext &&
								isExistingUserSpecification &&
								!isSpecificationWordSpanishAmbiguous ) ||

								// Condition part 2
								( hasExistingSpecificationOnlyOneRelationWord &&
								isUserRelationWord &&
								isSelfGeneratedAssumption &&
								!isSpecificationWordSpanishAmbiguous &&
								existingSpecificationItem.isSelfGeneratedConclusion() ) ||

								// Condition part 3
								// Typical for Spanish
								( isSpecificationWordSpanishAmbiguous &&
								!isExistingHiddenSpanishSpecification &&
								!hasRelationWordExistingRelationContext &&
								!isPossessive &&
								isSelfGeneratedAssumption ) ) )
									// Need to copy context (4)
									contextResult.copiedRelationContextNr = existingRelationContextNr;
								else
									{
									if( isSpecificationWordSpanishAmbiguous )
										{
										// Typical for Spanish
										if( isPossessive &&
										!isExistingHiddenSpanishSpecification &&
										!isExistingSpecificationSelfGeneratedAssumption &&
										!isSelfGeneratedAssumption &&

										( !isUserRelationWord ||

										( !hasSpecificationCollection &&
										!hasRelationWordExistingRelationContext ) ) &&

										( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
										( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, true, nonCompoundSpanishSpecificationCollectionNr, specificationWordItem ) ) != null )
											// Need to copy context (5)
											contextResult.copiedRelationContextNr = foundSpecificationItem.relationContextNr();
										else
											{
											// Typical for Spanish
											if( !isExistingHiddenSpanishSpecification ||
											!isCompoundCollectionSpanishAmbiguous )
												contextResult.contextNr = existingRelationContextNr;
											}
										}
									else
										{
										if( !hasRelationWordConfirmedSpecification &&

										( hasExistingSpecificationCurrentCreationSentenceNr ||
										isPossessive ||
										isExistingSpecificationSelfGeneratedAssumption == isSelfGeneratedAssumption ) )
											{
											// Typical for Chinese: Skip on different specification words
											if( existingSpecificationItem.specificationWordItem() == specificationWordItem )
												contextResult.contextNr = foundRelationContextNr;
											}
										else
											{
											if( isPossessive &&
											hasCompoundUserSpecificationCollection == isSelfGeneratedAssumption &&
											existingSpecificationItem.isSelfGeneratedConclusion() )
												contextResult.contextNr = relationWordItem.contextNr( specificationWordItem );
											else
												{
												if( isPossessive ||

												( !isSelfGeneratedAssumption &&
												!hasRelationWordExistingRelationContext ) ||

												( isSelfGeneratedAssumption &&
												isUserRelationWord ) )
													contextResult.contextNr = existingRelationContextNr;
													
												else
													{
													if( hasRelationWordConfirmedSpecification &&
													hasCompoundUserSpecificationCollection &&
													( tempSpecificationItem = generalizationWordItem.bestAssumptionLevelSpecificationItem( isPossessive, specificationWordItem ) ) != null &&
													( foundContextNr = tempSpecificationItem.relationContextNr() ) != existingRelationContextNr )
														contextResult.contextNr = foundContextNr;
													}
												}
											}
										}
									}
								}
							else
								// Need to copy context (Invalid context)
								contextResult.copiedRelationContextNr = existingRelationContextNr;
							}
						}
					}
				}
			}
		else
			{
			if( hasRelationWordConfirmedSpecification &&
			isSelfGeneratedAssumption )
				{
				// Both existing specification and wanted specification are assumptions
				if( isExistingSpecificationSelfGeneratedAssumption )
					contextResult.contextNr = existingRelationContextNr;
				else
					// Need to copy context (6)
					contextResult.copiedRelationContextNr = existingRelationContextNr;
				}
			else
				{
				foundContextNr = foundContextItem.contextNr();

				if( isAssignment &&
				!isArchivedAssignment )
					{
					// Dynamic semantic ambiguity
					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != Constants.RESULT_OK )
						return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				else
					{
					if( existingRelationContextNr == Constants.NO_CONTEXT_NR )
						{
						// Collected afterwards
						if( isOlderUserSpecification ||

						( hasSpecificationCollection &&

						( isSelfGeneratedAssumption ||
						// Typical for Spanish
						isCompoundCollectionSpanishAmbiguous ) ) )
							contextResult.contextNr = foundContextNr;
						}
					else
						{
						if( ( existingRelationContextNr == foundContextNr &&
						// Typical for Spanish
						!isSpecificationWordSpanishAmbiguous ) ||

						( !isExistingOlderSpecification &&
						existingSpecificationCollectionNr == specificationCollectionNr &&
						userSpecificationCollectionNr != specificationCollectionNr ) ||

						// Existing specification is self-generated assumption
						( isExistingSpecificationSelfGeneratedAssumption &&

						( ( isSelfGeneratedAssumption &&

						( !isSpecificationWordSpanishAmbiguous ||
						!hasExistingSpecificationOnlyOneRelationWord ||
						isExistingHiddenSpanishSpecification ||
						!isExistingOlderSpecification ) ) ||

						( !hasExistingSpecificationOnlyOneRelationWord &&
						hasGeneralizationWordCurrentlyConfirmedSpecification ) ||

						( !hasSpecificationCollection &&
						isPossessive &&

						( hasExistingSpecificationOnlyOneRelationWord ||
						relationWordItem.hasContextInWord( existingRelationContextNr ) ) ) ) ) ||

						// Existing specification isn't a self-generated assumption
						( !isExistingSpecificationSelfGeneratedAssumption &&

						( ( hasExistingSpecificationCompoundCollection &&
						!isExistingUserSpecification ) ||

						( isExistingHiddenSpanishSpecification &&
						!isCompoundCollectionSpanishAmbiguous &&
						isUserAssignment ) ) ) ||

						// Typical for Spanish
						( isSpecificationWordSpanishAmbiguous &&
						isPossessive &&
						!hasExistingSpecificationOnlyOneRelationWord &&
						!hasExistingSpecificationCurrentCreationSentenceNr ) )
							{
							if( isValidContext( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) )
								contextResult.contextNr = existingRelationContextNr;
							else
								// Need to copy context (7)
								contextResult.copiedRelationContextNr = existingRelationContextNr;
							}
						else
							{
							if( isExistingOlderSpecification ||
							userSpecificationCollectionNr == specificationCollectionNr )
								{
								// Typical for Spanish
								// Spanish test file: "Complejo (19 - extraño)"
								if( isExistingHiddenSpanishSpecification &&
								!hasExistingSpecificationOnlyOneRelationWord )
									contextResult.contextNr = existingRelationContextNr;
								else
									{
									if( !isUserAssignment &&
									isPossessive &&
									isSelfGeneratedAssumption &&
									specificationCollectionNr == Constants.NO_CONTEXT_NR )
										// Need to copy context (8)
										// Test file: "John is a parent - Paul - Laura"
										contextResult.copiedRelationContextNr = existingRelationContextNr;
									else
										contextResult.contextNr = foundContextNr;
									}
								}
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

			if( contextResult.copiedRelationContextNr > Constants.NO_CONTEXT_NR &&
			copyContext( contextResult.copiedRelationContextNr, spanishAmbiguousCopyCollectionNr, contextResult.contextNr ) != Constants.RESULT_OK )
				return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to copy a context" );
			}

		return contextResult;
		}

	private CreateAndAssignResultType addSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasRelationWordConfirmedSpecification;
		boolean isAmbiguousRelationContext = false;
		boolean isAssignedOrClear;
		boolean isDefinitionSpecification;
		boolean isExistingOlderSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationContextAlreadyDefined = false;
		boolean isSelfGenerated = ( firstJustificationItem != null );
		boolean isSelfGeneratedAssumption;
		boolean isSpanishCurrentLanguage = false;
		boolean isSpecificationWordSpanishAmbiguous = false;
		int assignmentSpecificationCollectionNr;
		int copiedRelationContextNr = Constants.NO_CONTEXT_NR;
		int existingRelationContextNr = Constants.NO_CONTEXT_NR;
		JustificationItem firstHiddenJustificationItem;
		SpecificationItem createdAssignmentItem = null;
		SpecificationItem createdOrFoundAssignmentItem;
		SpecificationItem createdSpecificationItem = null;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundAssignmentItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem hiddenSpanishSpecificationItem;
		SpecificationItem replacedAssignmentItem;
		SpecificationItem reverseSpecificationItem;
		SpecificationItem tempSpecificationItem;
		WordItem currentContextWordItem;
		StringBuffer writtenSentenceStringBuffer;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignCheckResult;
		CreateAndAssignResultType createAndAssignReturnResult = new CreateAndAssignResultType();

		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given specification word is the administrator" );

		if( ( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, false, specificationWordItem ) ) != null )
			{
			isExistingOlderSpecification = existingSpecificationItem.isOlderItem();
			existingRelationContextNr = existingSpecificationItem.relationContextNr();

			// Typical for Chinese
			if( isChineseCurrentLanguage_ &&
			!isAssignment &&
			isExistingOlderSpecification &&
			isPossessive &&
			relationWordItem != null &&
			( tempSpecificationItem = relationWordItem.firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != null &&
			tempSpecificationItem.isUserAssignment() &&
			tempSpecificationItem.hasOnlyOneRelationWord() )
				// Force Chinese assignment on confirmation
				isAssignment = true;
			}

		// Get relation context
		// Exceptions: Conditions and imperative verbs have no relation context
		if( !isConditional &&
		generalizationWordTypeNr != Constants.WORD_TYPE_VERB_SINGULAR &&
		specificationWordItem != null &&
		relationWordItem != null )
			{
			hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();
			isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&
			existingSpecificationItem != null &&

			( existingRelationContextNr == relationContextNr ||

			// Confirmation
			( !isSpecificationWordSpanishAmbiguous &&
			hasRelationWordConfirmedSpecification &&
			existingSpecificationItem.hasOnlyOneRelationWord() &&
			relationWordItem.hasContextInWord( existingRelationContextNr ) ) ||

			// Existing specification is current
			( !isExistingOlderSpecification &&

			( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||

			( !isSpanishCurrentLanguage &&
			isSelfGenerated ) ) ) ||

			// User specification
			( userSpecificationItem_ != null &&

			( ( !isSelfGenerated &&
			!isQuestion ) ||

			( userSpecificationItem_.isOlderItem() &&
			existingSpecificationItem.specificationCollectionNr() == specificationCollectionNr ) ) ) ) &&

			isValidContext( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) )
				{
				isRelationContextAlreadyDefined = true;
				relationContextNr = existingRelationContextNr;
				}
			else
				{
				if( relationContextNr == Constants.NO_CONTEXT_NR ||

				// Assignment
				( isAssignment &&
				userSpecificationItem_ != null &&
				!userSpecificationItem_.isOlderItem() ) ||

				// No assignment
				( !isAssignment &&
				isPossessive &&
				specificationCollectionNr == Constants.NO_COLLECTION_NR &&
				// Typical for Spanish
				!isSpecificationWordSpanishAmbiguous ) ||

				// Current relation context is invalid
				!isValidContext( isPossessive, relationContextNr, specificationWordItem, relationWordItem ) )
					{
					isSelfGeneratedAssumption = ( firstJustificationItem != null &&
												firstJustificationItem.hasAssumptionLevel() );

					// Get relation context (advanced)
					if( ( contextResult = getRelationContextAdvanced( hasRelationWordConfirmedSpecification, isAssignment, isArchivedAssignment, isPossessive, isSelfGeneratedAssumption, isSpecificationWordSpanishAmbiguous, specificationCollectionNr, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to get the relation context (advanced)" );

					isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
					copiedRelationContextNr = contextResult.copiedRelationContextNr;
					relationContextNr = contextResult.contextNr;

					if( !isValidContext( isPossessive, relationContextNr, specificationWordItem, relationWordItem ) )
						return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I found an invalid context in word \"" + relationWordItem.anyWordTypeString() + "\"" );
					}
				else
					isRelationContextAlreadyDefined = true;
				}

			// Add relation context
			if( relationWordItem.addContext( relationWordTypeNr, specificationWordTypeNr, relationContextNr, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : Constants.NO_COLLECTION_NR ), specificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&
			isSelfGenerated &&
			!generalizationWordItem.isUserGeneralizationWord &&
			specificationWordItem.isUserSpecificationWord &&
			userSpecificationItem_ != null &&
			userSpecificationItem_.hasRelationContext() &&

			( hiddenSpanishSpecificationItem = generalizationWordItem.firstUnhiddenSpanishSpecificationItem() ) != null &&
			hiddenSpanishSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != null &&

			// Typical for Spanish
			// Spanish test file: "Complejo (19 - extraño)"
			( ( existingSpecificationItem != null &&

			( !existingSpecificationItem.hasCurrentCreationSentenceNr() ||
			!existingSpecificationItem.hasRelationContextCurrentlyBeenUpdated() ) ) ||

			( ( firstHiddenJustificationItem = hiddenSpanishSpecificationItem.firstJustificationItem() ) != null &&
			!firstHiddenJustificationItem.isOppositePossessiveSpecificationAssumption() ) ) )
				{
				// Write Spanish assumption that is not hidden anymore
				if( generalizationWordItem.writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is not hidden anymore" );

				generalizationWordItem.hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = true;

				if( isPossessive &&
				( currentContextWordItem = adminItem_.firstContextWordItem( relationContextNr ) ) != null )
					{
					// Do for all context words with relation context
					do	{
						if( currentContextWordItem != generalizationWordItem &&
						( reverseSpecificationItem = currentContextWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&
						reverseSpecificationItem.isSelfGeneratedSpecification() &&
						// Justification doesn't already exist in specification
						!reverseSpecificationItem.hasPrimarySpecificationJustification( hiddenSpanishSpecificationItem ) &&
						// Justification has at least the same assumption level
						reverseSpecificationItem.assumptionLevel() >= hiddenSpanishSpecificationItem.assumptionLevel() &&
						// Add reversible justification to unhidden specification
						reverseSpecificationItem.addJustificationToSpecification( false, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, hiddenSpanishSpecificationItem, null, null ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a reversible specification justification to the reverse specification" );
						}
					while( ( currentContextWordItem = currentContextWordItem._nextContextWordItem( relationContextNr ) ) != null );
					}
				}
			}

		// Skip adding specification if already exists
		if( isAssignment ||
		!isRelationContextAlreadyDefined ||
		existingSpecificationItem == null ||
		!existingSpecificationItem.isActiveAssignment() )
			{
			// Check for an assignment or a specification with opposite negative indicator
			if( !isAssignment &&
			isNegative &&
			isPossessive &&
			isArchivedAssignment_ )
				isAssignment = true;

			if( adminItem_.isSystemStartingUp() &&
			isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
				{
				if( ( createAndAssignReturnResult = adminItem_.addSpecificationWithAuthorization( isAssignment, isCharacteristicFor, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification to word \"" + generalizationWordItem.anyWordTypeString() + "\" with authorization" );
				}
			else
				{
				if( ( createAndAssignReturnResult = generalizationWordItem.addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}

			// No conflict found
			if( !GlobalVariables.hasDisplayedWarning )
				{
				isArchivedAssignment_ = isArchivedAssignment;
				foundSpecificationItem = createAndAssignReturnResult.foundSpecificationItem;
				replacedAssignmentItem = createAndAssignReturnResult.replacedAssignmentItem;

				isDefinitionSpecification = ( !hasRelationWord &&
											!isQuestion &&
											adminItem_.isNounWordType( generalizationWordTypeNr ) &&
											adminItem_.isNounWordType( specificationWordTypeNr ) );

				if( ( createdSpecificationItem = createAndAssignReturnResult.createdSpecificationItem ) == null )
					{
					// Not created
					if( hasRelationWord &&
					!isSelfGenerated &&
					foundSpecificationItem != null )
						{
						userSpecificationItem_ = foundSpecificationItem;

						if( !isQuestion &&
						foundSpecificationItem.hasRelationContext() )
							simpleUserSpecificationItem_ = generalizationWordItem.firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem );
						}
					}
				else
					{
					if( !isSelfGenerated )
						{
						userSpecificationItem_ = createdSpecificationItem;

						if( !isQuestion &&
						createdSpecificationItem.hasRelationContext() )
							simpleUserSpecificationItem_ = createdSpecificationItem;
						}
					}

				if( !isExclusiveSpecification &&
				!isQuestion &&
				!isSelfGenerated &&
				specificationWordItem != null &&
				!generalizationWordItem.hasCurrentlyConfirmedSpecification() )
					{
					if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN )
						{
						if( createdSpecificationItem != null )
							{
							if( isNegative )
								{
								if( createdSpecificationItem.hasSpecificationCollection() &&
								generalizationWordItem.firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) != null &&
								adminItem_.makeOnlyOptionLeftAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make an only-option-left assumption" );
								}
							else
								{
								if( !isPossessive )
									{
									if( isAssignment &&
									hasRelationWord &&
									// Collect generalization words of a proper noun specification
									adminItem_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

									// Test file: "Indirectly answered questions - John is masculine"
									if( userSpecificationItem_.isSpecificationAdjective() &&
									userSpecificationItem_.hasNonCompoundSpecificationCollection() &&
									// Make indirectly answered question assumption
									adminItem_.makeIndirectlyAnsweredQuestionAssumption( isArchivedAssignment, generalizationWordTypeNr, createdSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
								if( ( isPartOf ||
								isCharacteristicFor ) &&

								// Draw a possessive definition conclusion from a specification-generalization 'part of' sentence
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isEveryGeneralization, isExclusiveGeneralization, false, isNegative, false, true, false, isCharacteristicFor, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
									return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								if( createdSpecificationItem == null )
									{
									if( !isConditional &&
									!isNegative &&
									foundSpecificationItem != null &&

									// Check if primary specification already has an assignment
									( ( foundAssignmentItem = generalizationWordItem.firstAssignmentItem( isPossessive, isQuestion, relationContextNr, specificationWordItem ) ) != null ||
									isAssignment ) )
										{
										if( generalizationWordItem.writeSelectedSpecification( false, true, ( foundAssignmentItem == null ? foundSpecificationItem : foundAssignmentItem ) ) != Constants.RESULT_OK )
											return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a non proper noun specification of word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

										if( ( writtenSentenceStringBuffer = GlobalVariables.writtenSentenceStringBuffer ) == null ||
										writtenSentenceStringBuffer.length() == 0 )
											return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "Integrity error! I couldn't write the assignment in word \"" + generalizationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );

										if( InputOutput.writeInterfaceText( true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, ( foundAssignmentItem == null ? Constants.INTERFACE_LISTING_SENTENCE_ASSIGNMENT_IN_CONFLICT_WITH_DEFINITION_SPECIFICATION : Constants.INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) != Constants.RESULT_OK )
											return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a header" );

										if( InputOutput.writeText( Constants.INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceStringBuffer, GlobalVariables.learnedFromUserStringBuffer ) != Constants.RESULT_OK )
											return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a sentence" );
										}
									}
								else
									{
									if( !isAssignment &&
									isPossessive &&
									generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
									// Draw a specification-generalization 'part of' conclusion from a possessive definition sentence
									// See Block 2 of the included challenge document, or https://www.mafait.org/block_2
									addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isSpecific, isEveryGeneralization, isExclusiveGeneralization, false, isNegative, !isSpecific, false, false, false, true, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					}

				if( ( ( isAssignment &&
				// At this stage, selections must be stored - rather than executed. So, don't assign them
				!isSelection ) ||

				replacedAssignmentItem != null ) &&

				// Skip for example imperative verbs
				adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
					{
					isAssignedOrClear = ( specificationWordItem != null &&
										specificationWordItem.isAdjectiveAssignedOrEmpty() );

					if( ( createAndAssignCheckResult = assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to assign the specification" );

					if( ( createdAssignmentItem = createAndAssignCheckResult.createdSpecificationItem ) != null )
						{
						if( !isArchivedAssignment_ ||
						createAndAssignReturnResult.createdSpecificationItem != null )
							{
							createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;
							createAndAssignReturnResult.createdOrFoundSpecificationItem = createdAssignmentItem;
							}

						if( replacedAssignmentItem != null &&
						generalizationWordItem.replaceOrDeleteSpecification( replacedAssignmentItem, createdAssignmentItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to replace or delete an assignment in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					if( isNegative &&
					generalizationWordItem.isUserGeneralizationWord &&
					createdAssignmentItem.hasSpecificationCollection() &&
					// Draw only option left noun conclusion
					adminItem_.drawOnlyOptionLeftNounConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem.specificationCollectionNr(), generalizationContextNr, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw an only-option-left noun conclusion by negative assignment" );
					}

				if( !isSelfGenerated )
					{
					if( ( createdOrFoundAssignmentItem = ( createdAssignmentItem == null ? createAndAssignCheckResult.foundSpecificationItem : createdAssignmentItem ) ) != null )
						{
						userSpecificationItem_ = createdOrFoundAssignmentItem;

						if( !isQuestion &&
						createdOrFoundAssignmentItem.hasRelationContext() )
							simpleUserSpecificationItem_ = ( isArchivedAssignment_ ? createdOrFoundAssignmentItem : generalizationWordItem.firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem ) );
						}

						// Find answers to questions
						if( !isQuestion &&
						specificationWordItem != null &&
						( createdOrFoundAssignmentItem = createAndAssignCheckResult.createdOrFoundSpecificationItem ) != null &&
						generalizationWordItem.findAnswersToQuestions( specificationWordItem.compoundCollectionNr(), createdOrFoundAssignmentItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to find answers to questions" );
						}
					}

				if( !isExclusiveSpecification &&
				!isSpecificationGeneralization &&
				specificationWordItem != null &&
				userSpecificationItem_ != null )
					{
					if( hasRelationWord &&
					isSelfGenerated &&

					// Avoid looping of reversible conclusions
					( !isPossessive &&

					( !specificationWordItem.isUserSpecificationWord ||
					isSpanishCurrentLanguage ) ) &&

					// Draw reversible conclusions
					adminItem_.drawReversibleConclusions( userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					if( replacedAssignmentItem == null &&

					( !isSelfGenerated ||

					( isDefinitionSpecification &&
					isEveryGeneralization &&
					!isPossessive &&
					!userSpecificationItem_.hasSpecificationCollection() ) ) )
						{
						if( isQuestion )
							{
							if( hasRelationWord &&
							createdAssignmentItem != null )
								{
								assignmentSpecificationCollectionNr = createdAssignmentItem.specificationCollectionNr();
									
								// Check if specification doesn't exist yet
								if( generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, false, false, isNegative, isPossessive, assignmentSpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == null &&
								// Make suggestive question assumption
								adminItem_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, assignmentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, createdAssignmentItem, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make a suggestive question assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						else
							{
							if( isDefinitionSpecification &&
							!isPossessive &&
							foundSpecificationItem == null &&
							!userSpecificationItem_.hasSpecificationCollection() &&
							generalizationWordItem.firstExclusiveSpecificationItem( true ) == null &&
							// Draw specification-generalization conclusion
							adminItem_.drawSpecificationGeneralizationConclusion( isArchivedAssignment, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a specification-generalization conclusion" );
							}
						}
					}
				}
			}

		return createAndAssignReturnResult;
		}


	// Constructor

	protected AdminSpecification( AdminItem adminItem )
		{
		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected assignment methods

	protected byte assignSelectionSpecification( SelectionItem assignmentSelectionItem )
		{
		if( assignmentSelectionItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given assignment selection item is undefined" );

		if( assignSpecification( false, assignmentSelectionItem.isAssignedOrEmpty(), assignmentSelectionItem.isInactiveAssignment(), assignmentSelectionItem.isArchivedAssignment(), assignmentSelectionItem.isNegative(), assignmentSelectionItem.isPossessive(), assignmentSelectionItem.isSpecificationGeneralization(), assignmentSelectionItem.isUniqueUserRelation(), assignmentSelectionItem.assumptionLevel(), assignmentSelectionItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, assignmentSelectionItem.relationWordTypeNr(), assignmentSelectionItem.generalizationContextNr(), assignmentSelectionItem.specificationContextNr(), assignmentSelectionItem.relationContextNr(), assignmentSelectionItem.nContextRelations(), null, assignmentSelectionItem.generalizationWordItem(), assignmentSelectionItem.specificationWordItem(), assignmentSelectionItem.specificationString() ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign a specification" );

		return Constants.RESULT_OK;
		}

	protected CreateAndAssignResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( adminItem_.isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			return adminItem_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

		return generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, null );
		}


	// Protected specification methods

	protected void initializeLinkedWord()
		{
		linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
		linkedGeneralizationWordItem_ = null;

		nPreviousChineseUserRelationWords_ = 0;
		previousChineseStartRelationReadItem_ = null;
		previousChineseEndRelationReadItem_ = null;
		}

	protected void initializeAdminSpecificationVariables()
		{
		isArchivedAssignment_ = false;
		isChineseCurrentLanguage_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		// Don't initialize this variable here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

		// Don't initialize this variable here. It is initialized by method initializeLinkedWord()
//		nPreviousChineseUserRelationWords_ = 0;
		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		// Don't initialize this variable here. It is initialized by method initializeLinkedWord()
//		previousChineseStartRelationReadItem_ = null;
//		previousChineseEndRelationReadItem_ = null;

		simpleUserSpecificationItem_ = null;
		userSpecificationItem_ = null;

		// Don't initialize this variable here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordItem_ = null;
		}

	protected byte addUserSpecification( boolean isAction, boolean isAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, short imperativeVerbParameter, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short userAssumptionLevel, int generalizationContextNr, int specificationContextNr, ReadItem generalizationReadItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		boolean hasAddedSpecification = false;
		boolean hasAlreadyAddedChineseRelationWords = false;
		boolean hasLinkedPossessiveDeterminer = false;
		boolean hasRelationWord = ( startRelationReadItem != null );
		boolean isAmbiguousRelationContext = false;
		boolean isChineseCurrentLanguage = adminItem_.isChineseCurrentLanguage();
		boolean isChineseReasoning = false;
		boolean isCurrentNounValue;
		boolean isGeneralizationProperNoun;
		boolean isGeneralizationReasoningWordType;
		boolean isNegativeSpecification;
		boolean isNonChineseNumeral;
		boolean isPreposition = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationWord;
		boolean isSelection = ( selectionListNr != Constants.NO_LIST_NR );
		boolean isSkippingThisGeneralizationPart = false;
		boolean isSpanishAmbiguousSpecificationWord;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isSpecificationWord;
		boolean isSpecificationWordAlreadyAssignedByComparison = false;
		boolean isText;
		boolean isValueSpecificationWord = false;
		boolean isWaitingForRelation = false;
		boolean isWaitingForText = false;
		short currentSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short generalizationWordTypeNr;
		short linkedSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short valueGeneralizationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short wordOrderNr;
		int nContextRelations = 0;
		int nUserRelationWords = GlobalVariables.nUserRelationWords;
		int userSpecificationCollectionNr;
		ReadItem currentReadItem;
		ReadItem firstRelationReadItem;
		ReadItem lastGeneralizationReadItem;
		SpecificationItem additionalDefinitionSpecificationItem;
		SpecificationItem createdSpecificationItem;
		SpecificationItem existingSpecificationItem;
		WordItem adjectiveSpecificationWordItem;
		WordItem collectionWordItem;
		WordItem compoundGeneralizationWordItem = null;
		WordItem currentSpecificationWordItem = null;
		WordItem generalizationWordItem;
		WordItem linkedSpecificationWordItem = null;
		WordItem lookAheadSpanishRelationWordItem;
		WordItem previousSpecificationWordItem = null;
		WordItem readWordItem;
		WordItem relationWordItem;
		WordItem valueGeneralizationWordItem = null;
		String readWordTypeString;
		String specificationString = null;
		CreateAndAssignResultType createAndAssignResult;
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

		isGeneralizationProperNoun = generalizationReadItem.isProperNoun();
		generalizationWordTypeNr = generalizationReadItem.wordTypeNr();
		lastGeneralizationReadItem = generalizationReadItem.nextReadItem();

		isChineseCurrentLanguage_ = isChineseCurrentLanguage;
		isGeneralizationReasoningWordType = adminItem_.isGeneralizationReasoningWordType( generalizationWordTypeNr );

		while( lastGeneralizationReadItem != null &&
		!lastGeneralizationReadItem.isGeneralizationWord() )
			{
			if( lastGeneralizationReadItem.isPossessiveDeterminer() )
				hasLinkedPossessiveDeterminer = true;
			else
				{
				// Typical for Chinese: Relation before specification
				if( isChineseCurrentLanguage &&
				isGeneralizationProperNoun &&
				!isQuestion &&
				prepositionParameter > Constants.NO_PREPOSITION_PARAMETER &&
				lastGeneralizationReadItem.isSpecificationWord() )
					{
					linkedSpecificationWordTypeNr = lastGeneralizationReadItem.wordTypeNr();
					linkedSpecificationWordItem = lastGeneralizationReadItem.readWordItem();
					}
				}

			lastGeneralizationReadItem = lastGeneralizationReadItem.nextReadItem();
			}

		// Typical for Chinese: Relation first, then specification
		if( linkedSpecificationWordTypeNr > Constants.NO_WORD_TYPE_NR &&
		linkedSpecificationWordItem != null )
			{
			hasAddedSpecification = true;
			hasAlreadyAddedChineseRelationWords = true;

			if( nUserRelationWords == 0 )
				{
				hasRelationWord = true;
				isChineseReasoning = true;

				nUserRelationWords = nPreviousChineseUserRelationWords_;
				GlobalVariables.nUserRelationWords = nUserRelationWords;

				startRelationReadItem = previousChineseStartRelationReadItem_;
				endRelationReadItem = previousChineseEndRelationReadItem_;
				}

			if( ( contextResult = getRelationContext( true, false, false, false, true, generalizationWordItem, linkedSpecificationWordItem, null, startRelationReadItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to get a Chinese relation context" );

			isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
			userRelationContextNr_ = contextResult.contextNr;

			// Remember variables below
			nPreviousChineseUserRelationWords_ = nUserRelationWords;
			previousChineseStartRelationReadItem_ = startRelationReadItem;
			previousChineseEndRelationReadItem_ = endRelationReadItem;

			// Add Chinese user specification with relation
			if( addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, linkedSpecificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a Chinese user specification with a relation to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
			}

		do	{
			isNonChineseNumeral = currentReadItem.isNonChineseNumeral();
			isPreposition = currentReadItem.isPreposition();
			isRelationWord = currentReadItem.isRelationWord();
			isSpecificationWord = currentReadItem.isSpecificationWord();
			isText = currentReadItem.isText();
			wordOrderNr = currentReadItem.wordOrderNr();
			readWordItem = currentReadItem.readWordItem();

			if( isSkippingThisGeneralizationPart )
				{
				if( isWaitingForRelation )
					{
					if( isRelationWord ||
					isText )
						{
						isSkippingThisGeneralizationPart = false;
						isWaitingForRelation = false;
						isWaitingForText = false;
						}
					}
				else
					{
					if( generalizationReadItem.wordOrderNr() < wordOrderNr )
						isSkippingThisGeneralizationPart = false;
					}
				}
			else
				{
				if( hasRelationWord &&
				isGeneralizationReasoningWordType &&
				!isRelationWord &&
				doneSpecificationWordOrderNr_ > wordOrderNr )
					isSkippingThisGeneralizationPart = true;
				else
					{
					if( isGeneralizationProperNoun ||
					prepositionParameter == Constants.NO_PREPOSITION_PARAMETER )
						{
						if( currentReadItem.isGeneralizationWord() )
							{
							isSkippingThisGeneralizationPart = true;
							isWaitingForRelation = true;
							}
						else
							{
							if( isSpecificationWord &&
							generalizationWordItem.isAdjectiveComparison() )
								{
								isWaitingForText = true;
								isSpecificationWordAlreadyAssignedByComparison = true;
								currentSpecificationWordTypeNr = currentReadItem.wordTypeNr();
								currentSpecificationWordItem = readWordItem;
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
				specificationString = null;

				if( isText )
					specificationString = currentReadItem.readString;

				if( isSpecificationWordAlreadyAssignedByComparison )
					isSpecificationWordAlreadyAssignedByComparison = false;
				else
					{
					currentSpecificationWordItem = readWordItem;
					currentSpecificationWordTypeNr = ( isQuestion &&
														currentReadItem.isNoun() ? Constants.WORD_TYPE_NOUN_SINGULAR :
																					currentReadItem.wordTypeNr() );
					}

				if( isNonChineseNumeral &&
				isPossessive &&
				!isRelationWord &&
				( readWordTypeString = currentReadItem.readWordTypeString() ) != null )
					{
					if( Character.isDigit( readWordTypeString.charAt( 0 ) ) )
						nContextRelations = Integer.parseInt( readWordTypeString );
					else
						// Typical for Chinese
						nContextRelations = adminItem_.convertChineseNumbers( readWordTypeString.charAt( 0 ) );
					}
				else
					{
					if( !isSelection &&
					isSpecificationWord &&
					!hasAlreadyAddedChineseRelationWords &&
					startRelationReadItem != null &&
					!generalizationWordItem.isImperativeVerbDisplayLoginOrRead() )
						{
						if( ( contextResult = getRelationContext( false, isArchivedAssignment, isPossessive, isQuestion, true, generalizationWordItem, currentSpecificationWordItem, null, startRelationReadItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to get a relation context" );

						isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
						userRelationContextNr_ = contextResult.contextNr;
						}

					if( isText ||

					( !isPreposition &&
					isSpecificationWord ) ||

					( isRelationWord &&

					( ( isNonChineseNumeral &&
					isSelection ) ||

					( isChineseCurrentLanguage &&
					prepositionParameter == Constants.WORD_PARAMETER_PREPOSITION_OF &&
					endRelationReadItem != null &&
					endRelationReadItem.isChineseReversedImperativeNoun() ) ) ) )
						{
						// Typical for Chinese
						if( isChineseCurrentLanguage &&
						!isValueSpecificationWord &&
						currentReadItem.isNounValueAhead() )
							{
							isValueSpecificationWord = true;
							valueGeneralizationWordTypeNr = generalizationWordTypeNr;
							valueGeneralizationWordItem = generalizationWordItem;
							}

						isCurrentNounValue = currentReadItem.isNounValue();

						if( isValueSpecificationWord )
							{
							// Skip Chinese value word, because it is already processed
							if( !isCurrentNounValue )
								{
								hasAddedSpecification = true;

								if( hasRelationWord )
									return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "Adding a value specification with relation isn't implemented" );

								if( adminItem_.createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a value selection item" );

								// Value, but no relation
								if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a value specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						else	// No value
							{
							if( isCurrentNounValue )
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
										if( addUserSpecificationWithRelation( isAction, currentReadItem.isAdjectiveAssignedOrEmpty(), isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, selectionLevel, selectionListNr, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else	// No relation word
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

									// Existing user specification
									( ( existingSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) == null ||
									existingSpecificationItem.isSpecificationGeneralization() ) &&

									// Collect specification words
									collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to collect specification words" );
									}
								}

							if( !isSelection ||
							!currentReadItem.isAdjectiveAssignedOrEmpty() )
								{
								doneSpecificationWordOrderNr_ = wordOrderNr;
								linkedSpecificationWordTypeNr = currentSpecificationWordTypeNr;
								linkedSpecificationWordItem = currentSpecificationWordItem;

								if( !isCurrentNounValue )
									{
									hasAddedSpecification = true;

									// Login is already created by during startup
									if( !generalizationWordItem.isImperativeVerbDisplayLoginOrRead() )
										{
										if( hasRelationWord )
											{
											if( ( !hasAlreadyAddedChineseRelationWords ||
											isChineseReasoning ) &&

											// No selection, no value, with relation
											addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, isChineseReasoning, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a user specification with a relation to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else	// No relation word
											{
											isNegativeSpecification = ( isConditional ? false : isNegative );

											// No selection, no value, no relation
											if( ( createAndAssignResult = addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegativeSpecification, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ) ).result != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification without relation to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

											if( currentSpecificationWordItem != null &&
											( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != null )
												{
												if( !isNegative &&
												!isSpecificationGeneralization )
													{
													if( isExclusiveSpecification )
														{
														if( currentSpecificationWordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR &&
														previousSpecificationWordItem != null &&
														// For now, limit the negative specification singular noun words to two
														GlobalVariables.nUserSpecificationWords == 2 &&
														// Draw exclusive negative user conclusions
														adminItem_.drawExclusiveNegativeUserConclusions( isArchivedAssignment, createdSpecificationItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to draw an exclusive negative user conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														if( !isAssignment &&
														!isPossessive &&
														!isQuestion )
															{
															if( isGeneralizationProperNoun )
																{
																// Avoid idle call
																if( currentSpecificationWordItem.firstNegativeSpecificationItem() != null &&
																// Draw simple negative conclusions
																adminItem_.drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																	return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															else
																{
																if( currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE &&
																// Feminine or masculine specification word
																currentSpecificationWordItem.isFeminineOrMasculineWord() &&
																( adjectiveSpecificationWordItem = adminItem_.predefinedWordItem( currentSpecificationWordItem.isFeminineWord() ? Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE : Constants.WORD_PARAMETER_ADJECTIVE_FEMININE ) ) != null &&
																// For now, limit the negative specification adjective words
																GlobalVariables.nUserSpecificationWords == 1 &&
																// Draw negative user definition conclusions
																adminItem_.drawNegativeUserDefinitionConclusions( isArchivedAssignment, createdSpecificationItem, generalizationWordItem, adjectiveSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																	return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative user definition conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															}
														}
													}

												previousSpecificationWordItem = currentSpecificationWordItem;
												}
											}

										if( isGeneralizationReasoningWordType &&
										!isNegative &&
										!isPartOf &&
										!isQuestion &&
										currentSpecificationWordItem != null &&

										// Noun specification word
										( adminItem_.isNounWordType( currentSpecificationWordTypeNr ) ||

										// Adjective specification word
										( isEveryGeneralization &&
										currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE ) ) )
											{
											isSpanishAmbiguousSpecificationWord = currentSpecificationWordItem.isNounWordSpanishAmbiguous();

											if( isPossessive )
												{
												if( isSpanishAmbiguousSpecificationWord &&
												startRelationReadItem != null &&
												userSpecificationItem_ != null &&
												GlobalVariables.nUserRelationWords == 1 &&
												( lookAheadSpanishRelationWordItem = startRelationReadItem.lookAheadRelationWordItem() ) != null &&
												!lookAheadSpanishRelationWordItem.isMasculineWord() &&
												// Mark relation word as masculine
												lookAheadSpanishRelationWordItem.markWordAsMasculine() != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to mark relation word \"" + lookAheadSpanishRelationWordItem.anyWordTypeString() + "\" as masculine" );
												}
											else	// Non-possessive
												{
												if( userSpecificationItem_ != null )
													{
													if( isSpanishCurrentLanguage &&
													isGeneralizationProperNoun &&
													generalizationWordItem.isMasculineWord() &&
													// Display Spanish assumptions that are not hidden anymore
													displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords( isAssignment, isArchivedAssignment, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to display Spanish assumptions that are not hidden anymore in context words" );

													if( currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE )
														{
														if( ( ( isGeneralizationProperNoun &&
														userSpecificationItem_.hasNonCompoundSpecificationCollection() ) ||

														( ( !isGeneralizationProperNoun ||
														isSpanishCurrentLanguage ) &&

														userSpecificationItem_.hasSpecificationCollection() ) ) &&

														// Avoid idle call
														GlobalVariables.firstUserDefinedProperNounWordItem != null &&
														// Draw proper noun part-of conclusions
														adminItem_.drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, ( isGeneralizationProperNoun ? currentSpecificationWordItem : generalizationWordItem ), null ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

														if( !isSpecificationGeneralization &&
														generalizationWordItem != currentSpecificationWordItem &&

														( ( isEveryGeneralization &&

														( !isExclusiveSpecification ||
														userSpecificationItem_.hasCompoundSpecificationCollection() ) ) ||

														userSpecificationItem_.isSpecificationPluralNoun() ) &&

														// Make a specification substitution 'part of' assumption
														adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}

													// Avoid idle call
													if( currentSpecificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null &&
													// Existing user specification
													( existingSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) != null &&

													( userSpecificationItem_.hasSpecificationCollection() ||
													existingSpecificationItem.isExclusiveSpecification() ) &&

													// Draw compound specification substitution conclusion
													adminItem_.drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, existingSpecificationItem, null, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												if( ( !isEveryGeneralization ||

												( !isExclusiveSpecification &&
												userSpecificationItem_ != null &&
												!userSpecificationItem_.hasGeneralizationCollection() &&
												!userSpecificationItem_.hasSpecificationCollection() ) ) &&

												( !hasRelationWord ||
												isChineseCurrentLanguage ||
												isSpanishCurrentLanguage ||

												( !isArchivedAssignment &&
												userSpecificationItem_ != null &&
												userSpecificationItem_.isAssignment() ) ) &&

												// Avoid idle call
												currentSpecificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null &&
												// Draw a specification substitution conclusion or ask a question
												adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, null ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( userSpecificationItem_ != null )
													{
													if( isGeneralizationProperNoun )
														{
														// Typical for Spanish
														// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
														if( isSpanishCurrentLanguage &&
														!isExclusiveSpecification &&
														!isSpanishAmbiguousSpecificationWord &&
														simpleUserSpecificationItem_ != null &&
														simpleUserSpecificationItem_.hasNonCompoundSpecificationCollection() &&
														( userSpecificationCollectionNr = simpleUserSpecificationItem_.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR &&
														( collectionWordItem = currentSpecificationWordItem.collectionWordItem( userSpecificationCollectionNr ) ) != null &&
														collectionWordItem.isNounWordSpanishAmbiguous() &&
														( additionalDefinitionSpecificationItem = collectionWordItem.firstSpecificationItem( false, false, false, collectionWordItem ) ) != null &&
														// Draw simple negative Spanish conclusions
														adminItem_.askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem_, additionalDefinitionSpecificationItem, simpleUserSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to simple negative Spanish conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													else	// Definition
														{
														// Avoid idle call
														if( generalizationWordItem.firstSpecificationGeneralizationItem( true ) != null )
															{
															// Draw compound specification substitution conclusion
															if( ( compoundResult = adminItem_.drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, userSpecificationItem_, generalizationWordItem ) ).result != Constants.RESULT_OK )
																return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

															compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
															}
														}
													}
												}

											if( isGeneralizationProperNoun &&
											!isUncountableGeneralizationNoun &&
											userSpecificationItem_ != null )
												{
												if( ( !isAssignment ||
												!userSpecificationItem_.isOlderItem() ||
												// Typical for Chinese
												isChineseCurrentLanguage ) &&

												// Avoid idle call
												GlobalVariables.firstContextWordItem != null &&
												// Draw reversible conclusions
												adminItem_.drawReversibleConclusions( userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( startRelationReadItem != null &&
												GlobalVariables.nUserRelationWords == 1 &&
												userSpecificationItem_.hasSpecificationCollection() &&
												userSpecificationItem_.hasNewInformation() &&
												( firstRelationReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null &&
												( relationWordItem = firstRelationReadItem.readWordItem() ) != null &&
												// Avoid idle call
												relationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, currentSpecificationWordItem, generalizationWordItem ) != null &&
												// Make an exclusive specification substitution assumption
												adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, firstRelationReadItem.wordTypeNr(), currentSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, currentSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
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

						( currentReadItem.isArticle() ||
						// Typical for Chinese
						currentReadItem.isConjunction() ) )
							nContextRelations = 0;
						}

					if( !isQuestion &&
					currentSpecificationWordItem != null &&
					linkedSpecificationWordItem != null &&
					userSpecificationItem_ != null &&

					( isRelationWord ||
					isSpecificationWord ) &&

					( !hasRelationWord ||
					linkedSpecificationWordItem != currentSpecificationWordItem ) )
						{
						relationWordItem = ( hasRelationWord ? currentSpecificationWordItem : null );

						if( isGeneralizationReasoningWordType &&
						!isPreposition &&
						!isValueSpecificationWord &&
						linkedSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&

						// Proper noun specification
						( ( isGeneralizationProperNoun &&
						!isNegative &&

						( userSpecificationItem_.hasSpecificationCollection() ||
						isSpanishCurrentLanguage ) ) ||

						// Test files: "Boiling point" and "Condensation point"
						( !isEveryGeneralization &&
						isPossessive &&

						( userSpecificationItem_.isUncountableGeneralizationNoun() ||
						// Typical for Spanish and French
						userSpecificationItem_.isAssignment() ) ) ) &&

						// Avoid idle call
						generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, linkedSpecificationWordItem, relationWordItem ) != null &&
						// Make an exclusive specification substitution assumption
						adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, linkedSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );

						if( isRelationWord ||
						hasLinkedPossessiveDeterminer )
							{
							// Linked specification
							if( currentSpecificationWordItem == linkedSpecificationWordItem )
								{
								if( linkedGeneralizationWordItem_ != null )
									return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "Linked word \"" + linkedGeneralizationWordItem_.anyWordTypeString() + "\" is already assigned" );

								linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
								linkedGeneralizationWordItem_ = generalizationWordItem;
								}

							if( isGeneralizationProperNoun &&
							userRelationContextNr_ > Constants.NO_CONTEXT_NR &&
							userSpecificationItem_.hasNewInformation() &&
							// Possessive user specification doesn't exist
							currentSpecificationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, linkedSpecificationWordItem ) == null )
								{
								if( ( isAssignment ||
								isPossessive ||
								// Typical for Chinese
								isChineseCurrentLanguage ) &&

								// Draw reversible conclusions
								adminItem_.drawReversibleConclusions( userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( !isExclusiveSpecification &&
								// User specification doesn't exist
								userSpecificationItem_.hasSpecificationCollection() &&
								// Make an exclusive specification substitution assumption
								adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, currentSpecificationWordTypeNr, linkedSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, currentSpecificationWordItem, linkedSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" and specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );
								}
							}
						else
							{
							if( !isSelection &&
							currentReadItem.isAdjectiveAssigned() &&
							InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem.readWordTypeString(), Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, wordOrderNr, Constants.INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			}
		while( currentReadItem != endSpecificationReadItem &&
		( currentReadItem = currentReadItem.nextReadItem() ) != null &&
		// No conflict found
		!GlobalVariables.hasDisplayedWarning );

		if( !hasAddedSpecification )
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I have not added any specification" );

		if( isGeneralizationProperNoun &&
		!isQuestion &&
		!isSelection )
			{
			if( hasRelationWord )
				{
				// Recalculate assumption levels of in touched words
				if( recalculateAssumptionLevelsInTouchedWords() != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumption levels of specifications in all touched words" );
				}
			else
				{
				// Recalculate assumption levels of specifications in generalization word
				if( generalizationWordItem.recalculateAssumptionLevelsInWord() != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumption levels of specifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isExclusiveGeneralization = false;
		boolean isSpecificationWordSpanishAmbiguous;
		SpecificationItem foundSpecificationItem;
		WordItem currentGeneralizationWordItem;
		WordItem previousGeneralizationWordItem = null;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationWordItem = GlobalVariables.firstSpecificationWordItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The first specification word is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		// Do for all specification words
		do	{
			if( currentGeneralizationWordItem != generalizationWordItem &&
			currentGeneralizationWordItem.hasWordType( false, generalizationWordTypeNr ) &&
			// Try to find matching specification word
			( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, isPossessive, relationContextNr, specificationWordItem ) ) != null )
				{
				// Languages and users
				if( ( isAssignment &&
				!isSpecificationWordSpanishAmbiguous ) ||

				foundSpecificationItem.isExclusiveGeneralization() )
					isExclusiveGeneralization = true;

				previousGeneralizationWordItem = currentGeneralizationWordItem;
				}
			}
		// Continue search to get the most recent generalization word
		while( ( currentGeneralizationWordItem = currentGeneralizationWordItem.nextSpecificationWordItem ) != null );

		if( previousGeneralizationWordItem != null &&
		collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalization words \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" and \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		return Constants.RESULT_OK;
		}

	protected byte recalculateAssumptionLevelsInTouchedWords()
		{
		WordItem currentTouchedWordItem;

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				if( currentTouchedWordItem.recalculateAssumptionLevelsInWord() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption levels of specifications in word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

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

	protected ContextResultType getRelationContext( boolean hasAlreadyAddedChineseRelationWords, boolean isArchivedAssignment, boolean isPossessive, boolean isQuestion, boolean isUserSentence, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		boolean isSkippingThisContext = false;
		int currentRelationContextNr;
		int foundRelationContextNr;
		ContextItem currentRelationContextItem;
		ReadItem relationReadItem = null;
		SpecificationItem foundSpecificationItem;
		WordItem currentContextWordItem;
		WordItem currentRelationWordItem = relationWordItem;
		ContextResultType contextResult = new ContextResultType();

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem != null )
			{
			if( ( relationReadItem = startRelationReadItem.firstRelationWordReadItem() ) == null )
				return adminItem_.startContextResultError( 1, moduleNameString_, "The read word of the first relation word is undefined" );

			currentRelationWordItem = relationReadItem.readWordItem();
			}

		if( currentRelationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "I couldn't find any relation word" );

		if( ( currentRelationContextItem = currentRelationWordItem.firstActiveContextItem() ) != null )
			{
			// Do for all relation context items in the first relation context word
			do	{
				if( ( currentRelationContextNr = currentRelationContextItem.contextNr() ) > Constants.NO_CONTEXT_NR &&
				// Specification word is required for Chinese
				currentRelationWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem ) )
					{
					isSkippingThisContext = false;

					if( relationReadItem == null )
						isSkippingThisContext = true;
					else
						{
						if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
							{
							// Do for all context words, either in the current relation list or outside this list
							do	{
								if( relationReadItem.hasFoundRelationWordInThisList( currentContextWordItem ) != currentContextWordItem.hasContextInWord( currentRelationContextNr, specificationWordItem ) )
									isSkippingThisContext = true;
								}
							while( !isSkippingThisContext &&
							( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
							}
						}

					// The relation words in the list contain this context exclusively. (So, no other words)
					if( !isSkippingThisContext )
						contextResult.contextNr = currentRelationContextNr;
					}
				else
					{
					if( isUserSentence &&
					!hasAlreadyAddedChineseRelationWords &&
					!currentRelationContextItem.isOlderItem() )
						contextResult.contextNr = currentRelationContextNr;
					}
				}
			while( contextResult.contextNr == Constants.NO_CONTEXT_NR &&
			( currentRelationContextItem = currentRelationContextItem.nextContextItem() ) != null );
			}

		if( !isQuestion &&
		contextResult.contextNr == Constants.NO_CONTEXT_NR &&
		( foundSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, null ) ) != null &&
		( foundRelationContextNr = foundSpecificationItem.relationContextNr() ) > Constants.NO_CONTEXT_NR )
			{
			// Equal number (or more) relation words
			if( adminItem_.nContextWords( foundRelationContextNr ) >= GlobalVariables.nUserRelationWords )
				{
				if( foundSpecificationItem.isExclusiveSpecification() )
					{
					// Already existing static (exclusive) semantic ambiguity
					contextResult.isAmbiguousRelationContext = true;
					contextResult.contextNr = foundRelationContextNr;
					}
				else
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

	protected CreateAndAssignResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNounEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, short assumptionLevel, short justificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem additionalProperNounSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasDisplayedIntegrityWarning = false;
		boolean hasRelationWordConfirmedSpecification = false;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isOlderFoundSpecification;
		boolean isSelectedUniqueUserRelation = ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION );
		boolean isSpecificationWordSpanishAmbiguous;
		boolean isUserGeneralizationWord;
		int calculatedAssumptionLevel;
		JustificationItem createdJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem foundJustificationItem;
		SpecificationItem createdSpecificationItem = null;
		SpecificationItem foundSpecificationItem = null;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( isEveryGeneralization &&
		isGeneralizationProperNoun )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word type is proper noun, while the specification is indicated as a definition" );

		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( primarySpecificationItem != null &&
		primarySpecificationItem.isAnsweredQuestion() &&
		primarySpecificationItem.isSelfGeneratedQuestion() )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given primary specification item is obsolete. It is an answered, self-generated question" );

		// Check for existing justification
		if( ( foundJustificationItem = generalizationWordItem.foundJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) ) == null )
			{
			// Create justification
			if( ( justificationResult = generalizationWordItem.createJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to create a justification item" );

			createdJustificationItem = justificationResult.createdJustificationItem;
			}

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == null ? foundJustificationItem : createdJustificationItem ) ) == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I couldn't find or create a justification" );

		// No given assumption level
		if( assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
			{
			// Calculate assumption level
			if( ( calculatedAssumptionLevel = adminItem_.justificationAssumptionLevel( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem ) ) > Constants.MAX_LEVEL )
				return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "Assumption level overflow" );

			if( calculatedAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL )
				assumptionLevel = (short)calculatedAssumptionLevel;
			}

		if( ( createAndAssignResult = addSpecification( isAssignment, false, isArchivedAssignment, isCharacteristicFor, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isSelectedUniqueUserRelation, false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification" );

		// No conflict found
		if( !GlobalVariables.hasDisplayedWarning )
			{
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
			foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

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
					if( createdJustificationItem != null &&
					createdJustificationItem.isReplacedOrDeletedItem() )
						return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The created justification item is replaced or deleted" );

					if( relationWordItem != null )
						hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();

					foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

					if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) == null )
						return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The found specification item has no justifications" );

					isOlderFoundSpecification = foundSpecificationItem.isOlderItem();

					// A justification has been created, but the self-generated specification already exists
					// So, the justification needs to be added or replaced separately

					if( relationWordItem == null ||

					// No justification is created
					// Test files: "Complex (6)" and "John is a parent - Paul is a child of John and Anna"
					( createdJustificationItem == null &&
					!isOlderFoundSpecification &&
					isPossessive &&
					!foundSpecificationItem.hasJustification( foundJustificationItem ) ) ||

					// Justification is created
					( createdJustificationItem != null &&

					// Typical for Spanish
					( ( isSpecificationWordSpanishAmbiguous &&
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ||

					( foundSpecificationItem.isSelfGeneratedAssumption() &&
					!foundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&

					( justificationTypeNr != Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ||

					( secondarySpecificationItem != null &&

					( !secondarySpecificationItem.isSelfGeneratedAssumption() ||
					relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), secondarySpecificationItem.isNegative(), secondarySpecificationItem.isPossessive(), false, secondarySpecificationItem.specificationWordItem(), generalizationWordItem ) == null ) ) ) ) ||

					( ( assumptionLevel == Constants.NO_ASSUMPTION_LEVEL ||

					( isUserGeneralizationWord &&
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) &&

					!foundSpecificationItem.hasPrimarySpecificationJustification( primarySpecificationItem ) ) ) ) )
						{
						// Process justification
						if( processJustification( hasRelationWordConfirmedSpecification, isOlderFoundSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, firstJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to process justifications of the found specification" );

						foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();
						}
					}
				}
			else
				{
				if( foundSpecificationItem != null &&
				foundSpecificationItem.hasRelationContext() &&
				foundSpecificationItem.relationContextNr() == createdSpecificationItem.relationContextNr() )
					return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "Integrity error! A specification was created in word \"" + generalizationWordItem.anyWordTypeString() + "\" while a specification was found as well with the same relation context" );
				}

			if( !hasDisplayedIntegrityWarning &&
			!isNegative &&
			!isPartOf &&

			( createdJustificationItem != null ||
			createdSpecificationItem != null ||

			// Typical for Spanish
			( foundSpecificationItem != null &&
			foundSpecificationItem.isHiddenSpanishSpecification() ) ) &&

			adminItem_.isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					createdSpecificationItem != null &&
					// Draw a unique relation conclusion
					addSelfGeneratedSpecification( false, false, false, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, true, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated unique specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					if( isGeneralizationProperNoun )
						{
						if( relationWordItem == null &&
						questionParameter == Constants.NO_QUESTION_PARAMETER &&
						// Avoid idle call
						specificationWordItem.firstNegativeSpecificationItem() != null &&
						// Draw simple negative conclusions
						adminItem_.drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( ( !isAssignment ||
						isArchivedAssignment ) &&

						( !isExclusiveSpecification ||
						isSpecificationWordSpanishAmbiguous ||

						( createdSpecificationItem != null &&
						createdSpecificationItem.hasNonCompoundSpecificationCollection() ) ) &&

						// Draw proper noun part-of conclusions
						adminItem_.drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, specificationWordItem, ( isSpecificationWordSpanishAmbiguous ? relationWordItem : null ) ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
						}

					if( !isExclusiveSpecification &&
					foundSpecificationItem == null &&
					createdSpecificationItem != null &&
					createdSpecificationItem.hasSpecificationCollection() &&
					// Typical for Spanish
					generalizationWordItem != specificationWordItem &&
					// Make a specification substitution 'part of' assumption
					adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					// Test file: "Correcting invalidated assumption (by opposite suggestive question)"
					if( isGeneralizationProperNoun &&
					isUserGeneralizationWord &&
					createdSpecificationItem != null &&
					generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
					// Draw compound specification substitution conclusion
					adminItem_.drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, createdSpecificationItem, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					if( userSpecificationItem_ != null &&

					// Created specification
					( ( createdSpecificationItem != null &&

					( isExclusiveSpecification ||
					isSpecificationWordSpanishAmbiguous ||
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ||

					( isArchivedAssignment &&
					!isSpecificationGeneralization ) ||

					( isUserGeneralizationWord &&
					foundSpecificationItem == null &&

					( justificationTypeNr != Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||

					userSpecificationItem_.hasCompoundSpecificationCollection() ) ) ||

					( justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION &&
					adminItem_.isSpanishCurrentLanguage() ) ) ) ||

					// Found specification
					( foundSpecificationItem != null &&
					justificationTypeNr != Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION &&
					!foundSpecificationItem.hasCompoundSpecificationCollection() &&

					( ( !isUserGeneralizationWord &&
					foundSpecificationItem.hasSpecificationCollection() ) ||

					( foundSpecificationItem.isOlderItem() &&

					( ( foundSpecificationItem.hasCurrentCreationSentenceNr() &&
					userSpecificationItem_.hasRelationContext() ) ||

					userSpecificationItem_.isSpecificationWordSpanishAmbiguous() ) ) ) ) ) &&

					// Avoid idle call
					specificationWordItem.firstNonNegativeNonPossessiveDefinitionSpecificationItem() != null &&
					// Draw a specification substitution conclusion or ask a question
					adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				}
			}

		createAndAssignResult.createdOrFoundSpecificationItem = createdSpecificationItem;

		if( foundSpecificationItem != null )
			{
			createAndAssignResult.foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

			if( createAndAssignResult.createdOrFoundSpecificationItem == null )
				createAndAssignResult.createdOrFoundSpecificationItem = foundSpecificationItem;
			}

		return createAndAssignResult;
		}
	}

/*************************************************************************
 *	"How amazing are the deeds of the Lord!
 *	All who delight in him should ponder them." (Psalm 111:2)
 *************************************************************************/
