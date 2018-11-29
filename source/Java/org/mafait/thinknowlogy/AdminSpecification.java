/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
 *	Version:		Thinknowlogy 2018r4 (New Science)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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

package org.mafait.thinknowlogy;

class AdminSpecification
	{
	// Private constructed variables

	private boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = false;
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

		if( !relationWordItem.hasContextInWord( relationContextNr, specificationWordItem ) &&
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

	private static int collectionNrByCompoundGeneralizationWordInCollectionWords( boolean isExclusiveSpecification, short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int collectionNr;
		WordItem currentCollectionWordItem;

		if( ( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
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
		boolean isCheckingExistingUserSpecification;
		boolean isChineseReversedImperativeNoun;
		boolean isFirstComparisonPart = ( selectionListNr == Constants.ADMIN_CONDITION_LIST );
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		short relationWordTypeNr;
		ReadItem currentRelationReadItem = startRelationReadItem;
		SpecificationItem chineseUserSpecificationItem;
		SpecificationItem existingSpecificationItem = null;
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

							if( nUserRelationWords == 1 &&
							( foundSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, ( questionParameter > Constants.NO_QUESTION_PARAMETER ), specificationWordItem ) ) != null &&
							foundSpecificationItem.hasRelationContext() &&
							( mostRecentContextWord = mostRecentContextWordInContextWords( foundSpecificationItem.relationContextNr() ) ) != null &&
							mostRecentContextWord != relationWordItem )
								previousRelationWordItem = mostRecentContextWord;

							if( previousRelationWordItem != null &&
							// Collect relation words
							collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

							if( selectionListNr == Constants.NO_LIST_NR )
								{
								// Typical for Chinese
								if( isChineseReasoning )
									{
									// Make a Chinese exclusive specification substitution assumption (non-possessive)
									if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

									if( ( chineseUserSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null &&
									!chineseUserSpecificationItem.isOlderItem() &&
									// Older possessive user specification doesn't exist
									relationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, specificationWordItem ) == null )
										{
										// Draw Chinese reversible conclusions
										if( adminItem_.drawReversibleConclusions( false, generalizationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to draw Chinese reversible conclusions in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

										if( chineseUserSpecificationItem.hasSpecificationCollection() &&
										// Make a Chinese exclusive specification substitution assumption (possessive)
										adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								else
									{
									if( isCheckingExistingUserSpecification )
										existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, specificationWordItem, relationWordItem );

									if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( existingSpecificationItem == null ? Constants.NO_COLLECTION_NR : existingSpecificationItem.specificationCollectionNr() ), generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, null, generalizationWordItem, specificationWordItem, relationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									!isAssignment &&
									!isPossessive &&
									generalizationWordItem.firstAssignmentItem( false, false, relationContextNr, specificationWordItem ) == null &&

									// Draw proper noun part-of conclusions
									adminItem_.drawProperNounPartOfConclusions( false, isArchivedAssignment, null, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
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
		while( !GlobalVariables.hasDisplayedWarning &&
		currentRelationReadItem != endRelationReadItem &&
		( currentRelationReadItem = currentRelationReadItem.nextReadItem() ) != null );

		return Constants.RESULT_OK;
		}

	private byte collectGeneralizationWords( boolean isExclusiveGeneralization, short generalizationWordTypeNr, short commonWordTypeNr, WordItem previousGeneralizationWordItem, WordItem newGeneralizationWordItem, WordItem newCommonWordItem )
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

		if( ( generalizationCollectionNr = previousGeneralizationWordItem.collectionNr( newCommonWordItem ) ) == Constants.NO_COLLECTION_NR )
			generalizationCollectionNr = newGeneralizationWordItem.collectionNr( newCommonWordItem );

		if( generalizationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			if( ( boolResult = newGeneralizationWordItem.findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find out if word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" is collected with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			if( ( collectionResult = previousGeneralizationWordItem.createCollection( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, newGeneralizationWordItem, newCommonWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( generalizationCollectionNr == Constants.NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			if( newGeneralizationWordItem.createCollection( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + newGeneralizationWordItem.anyWordTypeString() + "\" with word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( previousGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in previous word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( newGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in new word \"" + newGeneralizationWordItem.anyWordTypeString() + "\"" );
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
			if( ( collectionResult = previousRelationWordItem.createCollection( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousRelationWordItem.anyWordTypeString() + "\" with word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

			if( collectionNr == Constants.NO_COLLECTION_NR )
				collectionNr = collectionResult.createdCollectionNr;

			if( currentRelationWordItem.createCollection( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + currentRelationWordItem.anyWordTypeString() + "\" with word \"" + previousRelationWordItem.anyWordTypeString() + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte collectSpecificationWords( boolean isExclusiveSpecification, boolean isPossessive, boolean isQuestion, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem compoundGeneralizationWordItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		boolean hasFoundCollection = false;
		boolean isAllowingDifferentCommonWord;
		short collectionOrderNr = Constants.NO_ORDER_NR;
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
			if( ( specificationCollectionNr = previousSpecificationWordItem.collectionNr( generalizationWordItem ) ) == Constants.NO_COLLECTION_NR )
				{
				if( ( specificationCollectionNr = currentSpecificationWordItem.collectionNr( generalizationWordItem ) ) == Constants.NO_COLLECTION_NR &&

				( specificationWordTypeNr != Constants.WORD_TYPE_NOUN_PLURAL ||
				generalizationWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR ) &&

				( specificationCollectionNr = previousSpecificationWordItem.collectionNr( specificationWordTypeNr ) ) == Constants.NO_COLLECTION_NR )
					specificationCollectionNr = currentSpecificationWordItem.collectionNr( specificationWordTypeNr );
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
			// A collection comes in pairs
			if( collectionOrderNr >= Constants.MAX_ORDER_NR - 1 )
				return adminItem_.startSystemError( 1, moduleNameString_, "Collection order number overflow" );

			if( ( collectionResult = previousSpecificationWordItem.createCollection( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

			if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem.createCollection( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
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

		if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( ( currentContextItem = currentContextWordItem.contextItem( existingContextNr ) ) != null &&
				currentContextWordItem.addContext( currentContextItem.contextWordTypeNr(), currentContextItem.specificationWordTypeNr(), newContextNr, newSpanishAmbiguousCollectionNr, currentContextItem.specificationWordItem() ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a copied context to word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte displaySpanishAssumptionsThatAreNotHiddenAnymore( boolean isArchivedAssignment, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		SpecificationItem hiddenSpanishSpecificationItem;

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( hiddenSpanishSpecificationItem = generalizationWordItem.firstUnhiddenSpanishSpecificationItem() ) != null &&

		( hiddenSpanishSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) != null ||
		hiddenSpanishSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ) != null ) )
			{
			hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = true;

			if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, true, hiddenSpanishSpecificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"" + generalizationWordItem.anyWordTypeString() + "\" that is not hidden anymore" );

			if( !userSpecificationItem_.isPossessive() &&
			// Draw proper noun part-of conclusions
			adminItem_.drawProperNounPartOfConclusions( false, isArchivedAssignment, null, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
			}

		return Constants.RESULT_OK;
		}

	private byte displaySpanishAssumptionsThatAreNotHiddenAnymore( boolean hasRelationContext, boolean isAssignment, boolean isArchivedAssignment, WordItem specificationWordItem )
		{
		boolean isSpecificationWordSpanishAmbiguous;
		SpecificationItem hiddenSpanishSpecificationItem;
		WordItem currentContextWordItem;

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			// Do for all context words
			do	{
				if( ( hiddenSpanishSpecificationItem = currentContextWordItem.firstUnhiddenSpanishSpecificationItem() ) != null )
					{
					if( !isAssignment )
						{
						hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = true;

						if( !hiddenSpanishSpecificationItem.hasAssumptionLevel() &&
						hiddenSpanishSpecificationItem.isSelfGeneratedAssumption() &&
						// Assumption level of hidden Spanish specification isn't calculated. So, calculate now.
						// Test Spanish test file: "Complejo (14)"
						hiddenSpanishSpecificationItem.calculateAssumptionLevel() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to calculate an unhidden Spanish assumption level" );

						if( currentContextWordItem.writeUpdatedSpecification( true, false, false, false, true, hiddenSpanishSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write a Spanish assumption in word \"" + currentContextWordItem.anyWordTypeString() + "\" that is not hidden anymore" );

						if( ( hasRelationContext ||
						isSpecificationWordSpanishAmbiguous ) &&

						// Draw Spanish reversible conclusions
						adminItem_.drawReversibleConclusions( false, currentContextWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions in word \"" + currentContextWordItem.anyWordTypeString() + "\"" );
						}

					// Draw proper noun part-of conclusions
					if( adminItem_.drawProperNounPartOfConclusions( true, isArchivedAssignment, null, specificationWordItem, currentContextWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte processJustification( boolean hasFoundRelationContext, boolean isOlderFoundSpecification, boolean isUserGeneralizationWord, short justificationTypeNr, JustificationItem createdOrFoundJustificationItem, SpecificationItem foundSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasRelationWordConfirmedSpecification = false;
		boolean isAttachingJustification = true;
		boolean isRemovingPreviousJustifications = false;
		boolean isSpecificationWordSpanishAmbiguous;
		JustificationItem existingJustificationItem;
		JustificationItem obsoleteJustificationItem;
		SpecificationItem assumptionSpecificationItem;
		WordItem specificationWordItem;

		if( createdOrFoundJustificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given created or found justification item is undefined" );

		if( foundSpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given found specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given secondary specification item is undefined" );

		isSpecificationWordSpanishAmbiguous = foundSpecificationItem.isSpecificationWordSpanishAmbiguous();

		if( relationWordItem != null )
			hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();

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
							isAttachingJustification = false;
						}
					else
						{
						if( isUserGeneralizationWord &&

						( ( secondarySpecificationItem.isSelfGeneratedConclusion() &&
						( specificationWordItem = secondarySpecificationItem.specificationWordItem() ) != null &&
						( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem.isArchivedAssignment(), false, false, true, specificationWordItem, generalizationWordItem ) ) != null &&
						( existingJustificationItem = generalizationWordItem.secondarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != null ) ||

						( !isSpecificationWordSpanishAmbiguous &&
						primarySpecificationItem != null &&
						primarySpecificationItem.isSelfGeneratedConclusion() &&
						( specificationWordItem = primarySpecificationItem.specificationWordItem() ) != null &&
						( assumptionSpecificationItem = relationWordItem.firstSelfGeneratedCheckSpecificationItem( false, foundSpecificationItem.isArchivedAssignment(), false, false, true, specificationWordItem, generalizationWordItem ) ) != null &&
						( existingJustificationItem = generalizationWordItem.primarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != null ) ) )
							{
							if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to replace an existing justification item in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

							isAttachingJustification = false;
							}
						}
					}

				break;

			case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( hasFoundRelationContext &&
				isOlderFoundSpecification &&
				isUserGeneralizationWord &&
				primarySpecificationItem != null &&
				// Confirmation
				secondarySpecificationItem.isUserSpecification() &&
				( existingJustificationItem = generalizationWordItem.olderComplexJustificationItem( true, secondarySpecificationItem.isPossessive(), justificationTypeNr, secondarySpecificationItem.specificationCollectionNr(), primarySpecificationItem ) ) != null )
					{
					// Replace existing justification by the created justification
					if( generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to replace an existing justification by the created justification item in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					isAttachingJustification = false;
					}

				break;

			case Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION:
				if( !hasRelationWordConfirmedSpecification &&
				isOlderFoundSpecification &&
				!isUserGeneralizationWord &&
				foundSpecificationItem.isSelfGeneratedConclusion() )
					isAttachingJustification = false;

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( primarySpecificationItem != null &&
				( existingJustificationItem = foundSpecificationItem.differentAssumptionLevelSpecificationSubstitutionJustificationItem( primarySpecificationItem, secondarySpecificationItem ) ) != null )
					{
					isAttachingJustification = false;

					if( primarySpecificationItem.assumptionLevel() < existingJustificationItem.primarySpecificationAssumptionLevel() &&
					// Replace less certain existing justification by the created justification
					generalizationWordItem.replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION:
				if( isOlderFoundSpecification &&
				primarySpecificationItem != null &&
				!primarySpecificationItem.isPossessive() )
					{
					if( ( existingJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) != null &&

					( foundSpecificationItem.isConcludedAssumption() ||
					existingJustificationItem.primarySpecificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() ) )
						isRemovingPreviousJustifications = true;
					}

				break;

			case Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION:
				// Typical for Spanish
				if( isSpecificationWordSpanishAmbiguous )
					{
					if( foundSpecificationItem.secondarySpecificationJustificationItem( secondarySpecificationItem ) == null )
						{
						if( !hasRelationWordConfirmedSpecification )
							{
							if( secondarySpecificationItem.isUserAssignment() )
								{
								if( ( obsoleteJustificationItem = foundSpecificationItem.firstOlderJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) != null &&
								obsoleteJustificationItem.secondaryGeneralizationWordItem() == relationWordItem )
									{
									if( generalizationWordItem.replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to replace a reversible justification item without relation context in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									isAttachingJustification = false;
									}
								}
							else
								{
								if( ( obsoleteJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) != null &&
								obsoleteJustificationItem.secondaryGeneralizationWordItem() == relationWordItem )
									isAttachingJustification = false;
								}
							}
						}
					else
						isAttachingJustification = false;
					}
				else
					{
					if( foundSpecificationItem.hasOnlyOneRelationWord() )
						isRemovingPreviousJustifications = true;
					}

				break;

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				if( foundSpecificationItem.isConcludedAssumption() )
					// Remove obsolete justifications from concluded assumption
					isRemovingPreviousJustifications = true;

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				// Adjusted question
				if( isOlderFoundSpecification &&
				adminItem_.adjustedQuestionWordItem() == generalizationWordItem )
					{
					// Remove obsolete justifications from adjusted compound question
					isRemovingPreviousJustifications = true;
					// Adjusted question needs to be rewritten
					foundSpecificationItem.clearStoredSentenceStringBuffer();
					}
			}

		if( isRemovingPreviousJustifications )
			{
			if( foundSpecificationItem.hasCurrentCreationSentenceNr() )
				{
				if( foundSpecificationItem.changeFirstJustification( false, createdOrFoundJustificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to change the first justification of the found specification" );
				}
			else
				{
				// Copy and replace
				if( generalizationWordItem.copyAndReplaceSpecificationItem( foundSpecificationItem.isAnsweredQuestion(), foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.isExclusiveSpecification(), foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to copy and replace the found specification, with a different first justification item" );
				}
			}
		else
			{
			if( isAttachingJustification &&
			// Attach created justification to the found specification
			foundSpecificationItem.updatedSpecificationItem().attachJustificationToSpecificationAdvanced( createdOrFoundJustificationItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to attach a justification to the found specification" );
			}

		return Constants.RESULT_OK;
		}

	private byte recalculateAssumptionsInTouchedWords()
		{
		WordItem currentTouchedWordItem;

		if( ( currentTouchedWordItem = GlobalVariables.firstTouchedWordItem ) != null )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				if( currentTouchedWordItem.recalculateAssumptionsInWord() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumptions of word \"" + currentTouchedWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem.nextTouchedWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private static WordItem mostRecentContextWordInContextWords( int contextNr )
		{
		WordItem currentContextWordItem;
		WordItem mostRecentWordItem = null;

		if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem.contextItem( contextNr ) != null )
					mostRecentWordItem = currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return mostRecentWordItem;
		}

	private ContextResultType getRelationContext( boolean hasRelationWordConfirmedSpecification, boolean isAssignment, boolean isArchivedAssignment, boolean isPossessive, boolean isSelfGeneratedAssumption, boolean isSpecificationWordSpanishAmbiguous, int specificationCollectionNr, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasExistingSpecificationCompoundCollection = false;
		boolean hasExistingSpecificationCurrentCreationSentenceNr = false;
		boolean hasExistingSpecificationOnlyOneRelationWord = false;
		boolean hasGeneralizationWordAnsweredSelfGeneratedQuestion;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasNonCompoundUserSpecificationCollection = false;
		boolean hasRelationWordExistingRelationContext;
		boolean hasSpecificationCollection = ( specificationCollectionNr > Constants.NO_COLLECTION_NR );
		boolean hasUserSpecificationCompoundCollection = false;
		boolean isCompoundCollectionSpanishAmbiguous = false;
		boolean isExistingArchivedAssignment = false;
		boolean isExistingHiddenSpanishSpecification = false;
		boolean isExistingOlderSpecification = false;
		boolean isExistingSpecificationConcludedAssumption = false;
		boolean isExistingSpecificationSelfGeneratedAssumption = false;
		boolean isExistingSpecificationSelfGeneratedConclusion;
		boolean isExistingUserSpecification = false;
		boolean isGeneralizationWordUserRelationWord;
		boolean isUserArchivedAssignment = false;
		boolean isUserAssignment = false;
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
		WordItem foundWordItem;
		ContextResultType contextResult = new ContextResultType();

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( isSpecificationWordSpanishAmbiguous &&
		isPossessive &&
		specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		specificationWordItem.isCompoundCollection( specificationCollectionNr ) )
			{
			isCompoundCollectionSpanishAmbiguous = true;
			existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, false, isPossessive, false, specificationCollectionNr, Constants.NO_CONTEXT_NR, specificationWordItem );
			}
		else
			existingSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, isPossessive, specificationCollectionNr, specificationWordItem );

		hasGeneralizationWordAnsweredSelfGeneratedQuestion = generalizationWordItem.hasCurrentlyAnsweredSelfGeneratedQuestion();
		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();

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
			isExistingSpecificationConcludedAssumption = existingSpecificationItem.isConcludedAssumption();
			isExistingSpecificationSelfGeneratedAssumption = existingSpecificationItem.isSelfGeneratedAssumption();
			isExistingUserSpecification = existingSpecificationItem.isUserSpecification();
			existingSpecificationCollectionNr = existingSpecificationItem.specificationCollectionNr();
			existingRelationContextNr = existingSpecificationItem.relationContextNr();
			}

		if( userSpecificationItem != null )
			{
			if( userSpecificationItem.hasNonCompoundSpecificationCollection() )
				hasNonCompoundUserSpecificationCollection = true;
			else
				{
				if( userSpecificationItem.hasCompoundSpecificationCollection() )
					hasUserSpecificationCompoundCollection = true;
				}

			if( userSpecificationItem.isAssignment() )
				{
				isUserAssignment = true;

				if( userSpecificationItem.isArchivedAssignment() )
					isUserArchivedAssignment = true;
				}

			userSpecificationCollectionNr = userSpecificationItem.specificationCollectionNr();
			}

		// Try to find relation context with same number of relation words as in the user sentence
		if( nUserRelationWords > 0 )
			foundContextItem = relationWordItem.contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : Constants.NO_COLLECTION_NR ), specificationWordItem );

		if( foundContextItem == null )
			{
			isGeneralizationWordUserRelationWord = generalizationWordItem.isUserRelationWord;

			if( existingSpecificationItem == null )
				{
				if( ( foundContextNr = relationWordItem.contextNr( ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : Constants.NO_COLLECTION_NR ), specificationWordItem ) ) > Constants.NO_CONTEXT_NR &&

				// Typical for Chinese
				( generalizationWordItem.isFemale() ||

				( GlobalVariables.nUserGeneralizationWords > 1 &&

				( generalizationWordItem.hasFeminineOrMasculineProperNounEnding() ||
				contextUsedInUserSpecificationOfWordItem( isPossessive, specificationCollectionNr, foundContextNr, specificationWordItem ) == null ) ) ||

				// Generalization word is user relation word
				( isGeneralizationWordUserRelationWord &&

				( isUserAssignment ||

				( hasNonCompoundUserSpecificationCollection &&
				isPossessive ) ) &&

				adminItem_.nContextWords( foundContextNr, specificationWordItem ) == 1 ) ) )
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
					if( existingRelationContextNr > Constants.NO_CONTEXT_NR &&
					isExistingArchivedAssignment == isArchivedAssignment )
						foundRelationContextNr = existingRelationContextNr;
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
								contextResult.contextNr = ( foundContextNr > Constants.NO_CONTEXT_NR &&

															( !isExistingOlderSpecification ||
															existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) == null ) ? foundContextNr : existingRelationContextNr );
							}
						}
					}
				else
					{
					if( contextResult.contextNr == Constants.NO_CONTEXT_NR )
						{
						isExistingSpecificationSelfGeneratedConclusion = existingSpecificationItem.isSelfGeneratedConclusion();

						if( hasGeneralizationWordCurrentlyConfirmedSpecification )
							{
							if( !hasSpecificationCollection ||
							!isExistingOlderSpecification ||
							!isPossessive ||

							( hasUserSpecificationCompoundCollection &&
							!isExistingHiddenSpanishSpecification &&
							!isExistingSpecificationSelfGeneratedConclusion ) )
								{
								if( isValidContext( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) )
									contextResult.contextNr = existingRelationContextNr;
								else
									{
									if( isSpecificationWordSpanishAmbiguous )
										spanishAmbiguousCopyCollectionNr = specificationCollectionNr;

									// Need to copy context (1)
									contextResult.copiedRelationContextNr = existingRelationContextNr;
									}
								}
							else
								{
								// Typical for Spanish
								if( isExistingHiddenSpanishSpecification ||
								isExistingSpecificationConcludedAssumption )
									// Need to copy context (2)
									contextResult.copiedRelationContextNr = existingRelationContextNr;
								}
							}
						else
							{
							if( isValidContext( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) )
								{
								hasRelationWordExistingRelationContext = relationWordItem.hasContextInWord( existingRelationContextNr, specificationWordItem );

								// Check for copy context (of older existing specification)
								if( isExistingOlderSpecification &&

								// Condition part 1
								( ( !hasRelationWordExistingRelationContext &&
								isExistingUserSpecification &&
								!isSpecificationWordSpanishAmbiguous ) ||

								// Condition part 2
								( hasExistingSpecificationOnlyOneRelationWord &&
								isGeneralizationWordUserRelationWord &&
								isSelfGeneratedAssumption &&
								!isSpecificationWordSpanishAmbiguous &&
								isExistingSpecificationSelfGeneratedConclusion ) ||

								// Condition part 3
								// Typical for Spanish
								( isSpecificationWordSpanishAmbiguous &&
								!isExistingHiddenSpanishSpecification &&
								!hasRelationWordExistingRelationContext &&
								!isPossessive &&
								isSelfGeneratedAssumption ) ) )
									// Need to copy context (3)
									contextResult.copiedRelationContextNr = existingRelationContextNr;
								else
									{
									if( isSpecificationWordSpanishAmbiguous )
										{
										// Typical for Spanish
										if( hasExistingSpecificationCompoundCollection ||
										hasSpecificationCollection ||
										isExistingHiddenSpanishSpecification ||
										isExistingSpecificationSelfGeneratedAssumption )
											{
											// Typical for Spanish
											if( isExistingHiddenSpanishSpecification &&
											( foundWordItem = contextUsedInUserSpecificationOfWordItem( isPossessive, existingSpecificationCollectionNr, existingRelationContextNr, specificationWordItem ) ) != null &&
											( foundSpecificationItem = foundWordItem.firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, true, Constants.NO_QUESTION_PARAMETER, userSpecificationCollectionNr, Constants.NO_CONTEXT_NR, specificationWordItem ) ) != null )
												{
												if( hasSpecificationCollection )
													// Need to copy context (User)
													contextResult.copiedRelationContextNr = existingRelationContextNr;
												else
													contextResult.contextNr = foundSpecificationItem.relationContextNr();
												}
											else
												{
												// Typical for Spanish
												if( isPossessive &&
												!isExistingSpecificationSelfGeneratedAssumption &&
												!isGeneralizationWordUserRelationWord &&
												!isSelfGeneratedAssumption &&
												( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem.nonCompoundCollectionNr() ) > Constants.NO_COLLECTION_NR &&
												( foundSpecificationItem = generalizationWordItem.bestMatchingRelationContextNrSpecificationItem( false, true, nonCompoundSpanishSpecificationCollectionNr, specificationWordItem ) ) != null )
													// Need to copy context (4)
													contextResult.copiedRelationContextNr = foundSpecificationItem.relationContextNr();
												else
													{
													// Typical for Spanish
													if( !isCompoundCollectionSpanishAmbiguous )
														contextResult.contextNr = existingRelationContextNr;
													}
												}
											}
										}
									else
										{
										if( !hasRelationWordConfirmedSpecification &&

										( isPossessive ||
										isExistingSpecificationSelfGeneratedAssumption == isSelfGeneratedAssumption ) )
											{
											// Typical for Chinese: Skip on different specification words
											if( existingSpecificationItem.specificationWordItem() == specificationWordItem )
												contextResult.contextNr = foundRelationContextNr;
											}
										else
											{
											if( isExistingSpecificationConcludedAssumption )
												contextResult.contextNr = relationWordItem.contextNr( specificationWordItem );
											else
												{
												if( isPossessive ||

												( !isSelfGeneratedAssumption &&
												!hasRelationWordExistingRelationContext ) ||

												( isSelfGeneratedAssumption &&
												isGeneralizationWordUserRelationWord ) )
													contextResult.contextNr = existingRelationContextNr;
												}
											}
										}
									}
								}
							else
								{
								// Invalid context
								if( !isSpecificationWordSpanishAmbiguous ||
								// Typical for Spanish
								hasExistingSpecificationOnlyOneRelationWord ||
								isSelfGeneratedAssumption )
									// Need to copy context (Invalid Spanish context)
									contextResult.copiedRelationContextNr = existingRelationContextNr;
								else
									{
									if( relationWordItem.addContext( existingSpecificationItem.relationWordTypeNr(), existingSpecificationItem.specificationWordTypeNr(), existingRelationContextNr, Constants.NO_COLLECTION_NR, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to add a Spanish relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );
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
						if( ( hasSpecificationCollection &&

						( isSelfGeneratedAssumption ||
						// Typical for Spanish
						isCompoundCollectionSpanishAmbiguous ) ) ||

						// Collected afterwards
						( userSpecificationItem != null &&
						userSpecificationItem.isOlderItem() ) )
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

						( ( !hasExistingSpecificationOnlyOneRelationWord &&
						hasGeneralizationWordCurrentlyConfirmedSpecification ) ||

						( isSelfGeneratedAssumption &&

						( !isExistingHiddenSpanishSpecification ||
						// Typical for Spanish
						hasGeneralizationWordAnsweredSelfGeneratedQuestion ) ) ||

						( !hasSpecificationCollection &&
						isPossessive &&

						( hasExistingSpecificationOnlyOneRelationWord ||
						relationWordItem.hasContextInWord( existingRelationContextNr, specificationWordItem ) ) ) ) ) ||

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
								contextResult.contextNr = foundContextNr;
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
		boolean hasCurrentlyMoreSpecificNonExclusiveSpecification;
		boolean hasRelationWord = ( relationWordItem != null );
		boolean hasRelationWordConfirmedSpecification;
		boolean isAmbiguousRelationContext = false;
		boolean isAssignedOrClear;
		boolean isDefinitionSpecification;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationContextAlreadyDefined = false;
		boolean isSelfGenerated = ( firstJustificationItem != null );
		boolean isSelfGeneratedAssumption;
		boolean isSpanishCurrentLanguage = false;
		boolean isSpecificationWordSpanishAmbiguous = false;
		int copiedRelationContextNr = Constants.NO_CONTEXT_NR;
		int existingRelationContextNr = Constants.NO_CONTEXT_NR;
		SpecificationItem createdAssignmentItem = null;
		SpecificationItem createdSpecificationItem = null;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundAssignmentItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem replacedAssignmentItem;
		SpecificationItem tempSpecificationItem;
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
			existingRelationContextNr = existingSpecificationItem.relationContextNr();

			// Typical for Chinese
			if( isChineseCurrentLanguage_ &&
			!isAssignment &&
			isPossessive &&
			relationWordItem != null &&
			existingSpecificationItem.isOlderItem() &&
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
			relationWordItem.hasContextInWord( existingRelationContextNr, specificationWordItem ) ) ||

			// Existing specification is current
			( ( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||

			( !isSpanishCurrentLanguage &&
			isSelfGenerated ) ) &&

			!existingSpecificationItem.isOlderItem() ) ||

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
					isSelfGeneratedAssumption = ( firstJustificationItem != null && firstJustificationItem.isAssumptionJustification() );

					if( ( contextResult = getRelationContext( hasRelationWordConfirmedSpecification, isAssignment, isArchivedAssignment, isPossessive, isSelfGeneratedAssumption, isSpecificationWordSpanishAmbiguous, specificationCollectionNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to get the relation context" );

					isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
					copiedRelationContextNr = contextResult.copiedRelationContextNr;
					relationContextNr = contextResult.contextNr;

					if( !isValidContext( isPossessive, relationContextNr, specificationWordItem, relationWordItem ) )
						return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I found an invalid context in word \"" + relationWordItem.anyWordTypeString() + "\"" );
					}
				else
					isRelationContextAlreadyDefined = true;
				}

			if( isSpecificationWordSpanishAmbiguous &&
			specificationCollectionNr > Constants.NO_COLLECTION_NR &&
			!relationWordItem.hasContextInWord( relationContextNr, specificationWordItem ) &&
			// Check for the opposite collection context
			adminItem_.hasFoundContextInAllWords( relationContextNr, ( specificationWordItem.isCompoundCollection( specificationCollectionNr ) ? specificationWordItem.nonCompoundCollectionNr() : specificationWordItem.compoundCollectionNr() ), specificationWordItem ) )
				return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I found a Spanish ambiguous compound collection conflict in word \"" + relationWordItem.anyWordTypeString() + "\"" );

			if( relationWordItem.addContext( relationWordTypeNr, specificationWordTypeNr, relationContextNr, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : Constants.NO_COLLECTION_NR ), specificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a relation context to word \"" + relationWordItem.anyWordTypeString() + "\"" );

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&
			isSelfGenerated &&
			specificationWordItem.isUserSpecificationWord &&
			userSpecificationItem_.hasRelationContext() &&
			// Display Spanish assumptions that are not hidden anymore
			displaySpanishAssumptionsThatAreNotHiddenAnymore( isArchivedAssignment, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to display Spanish assumptions that are not hidden anymore in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
					if( hasRelationWord &&
					!isSelfGenerated &&
					foundSpecificationItem != null )
						{
						userSpecificationItem_ = foundSpecificationItem;

						if( !isQuestion &&
						foundSpecificationItem.hasRelationContext() )
							simpleUserSpecificationItem_ = generalizationWordItem.firstSpecificationItem( isPossessive, false, false, specificationWordItem );
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
				!generalizationWordItem.hasCurrentlyConfirmedSpecification() )
					{
					if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN )
						{
						if( createdSpecificationItem != null )
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
									// Collect generalization words of a proper noun specification
									adminItem_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

									if( adminItem_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									hasCurrentlyMoreSpecificNonExclusiveSpecification = generalizationWordItem.hasCurrentlyMoreSpecificNonExclusiveSpecification();

									if( generalizationWordItem.hasFoundAnswerToQuestion() &&

									( hasCurrentlyMoreSpecificNonExclusiveSpecification ||
									!specificationWordItem.isCompoundCollection( createdSpecificationItem.specificationCollectionNr() ) ) &&

									adminItem_.drawNegativeConclusionsFromAnsweredQuestions( hasCurrentlyMoreSpecificNonExclusiveSpecification, createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw negative conclusions from answered questions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, false, true, false, false, isCharacteristicFor, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, Constants.JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
									return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								if( createdSpecificationItem == null )
									{
									if( !isConditional &&
									!isNegative &&
									foundSpecificationItem != null &&
									!foundSpecificationItem.isExclusiveSpecification() &&

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
									// See Block 2 of the challenge document, or http://mafait.org/block2/
									addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isSpecific, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, !isSpecific, false, false, false, false, true, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"" + generalizationWordItem.anyWordTypeString() + "\" to specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					}

				if( !GlobalVariables.hasDisplayedWarning &&

				( ( isAssignment &&
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
							createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;

						if( replacedAssignmentItem != null &&
						generalizationWordItem.replaceOrDeleteSpecification( replacedAssignmentItem, createdAssignmentItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to replace or delete an assignment in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						if( isNegative &&
						generalizationWordItem.isUserGeneralizationWord &&
						createdAssignmentItem.hasSpecificationCollection() &&

						// Draw only option left conclusion
						adminItem_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem.specificationCollectionNr(), generalizationContextNr, generalizationWordItem ) != Constants.RESULT_OK )
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

				if( !GlobalVariables.hasDisplayedWarning &&
				!isExclusiveSpecification &&
				!isSpecificationGeneralization &&
				specificationWordItem != null &&
				userSpecificationItem_ != null )
					{
					if( hasRelationWord &&
					isSelfGenerated &&

					// Avoid looping of reversible conclusions
					( createdSpecificationItem != null ||

					( !isPossessive &&

					( !specificationWordItem.isUserSpecificationWord ||
					isSpanishCurrentLanguage ) ) ) &&

					// Draw reversible conclusions
					adminItem_.drawReversibleConclusions( ( createdSpecificationItem != null ), generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw reversible conclusions in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

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

							( createdAssignmentItem != null ||

							( createdSpecificationItem != null &&
							!generalizationWordItem.hasCurrentlyMoreSpecificQuestion() ) ) &&

							adminItem_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, ( createdAssignmentItem == null ? createdSpecificationItem : createdAssignmentItem ), generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make a suggestive question assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else
							{
							if( isDefinitionSpecification &&
							!isPossessive &&
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

	protected boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore()
		{
		return hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_;
		}

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
		hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = false;
		isArchivedAssignment_ = false;
		isChineseCurrentLanguage_ = false;

		doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		nPreviousChineseUserRelationWords_ = 0;
		userRelationContextNr_ = Constants.NO_CONTEXT_NR;

		// Don't initialize here. It is initialized by method initializeLinkedWord()
//		previousChineseStartRelationReadItem_ = null;
//		previousChineseEndRelationReadItem_ = null;

		simpleUserSpecificationItem_ = null;
		userSpecificationItem_ = null;

		// Don't initialize here. It is initialized by method initializeLinkedWord()
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
		boolean isNonChineseNumeral;
		boolean isPreposition = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isRelationWord;
		boolean isSelection = ( selectionListNr != Constants.NO_LIST_NR );
		boolean isSkippingThisGeneralizationPart = false;
		boolean isSpanishCurrentLanguage = adminItem_.isSpanishCurrentLanguage();
		boolean isSpecificationWord;
		boolean isSpecificationWordAlreadyAssignedByComparison = false;
		boolean isText;
		boolean isValueSpecificationWord = false;
		boolean isWaitingForRelation = false;
		boolean isWaitingForText = false;
		short currentSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short generalizationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short linkedSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short previousSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short valueGeneralizationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short wordOrderNr;
		int nContextRelations = 0;
		int nUserRelationWords = GlobalVariables.nUserRelationWords;
		ReadItem currentReadItem;
		ReadItem firstRelationReadItem;
		ReadItem lastGeneralizationReadItem;
		SpecificationItem existingUserSpecificationItem;
		SpecificationItem tempSpanishSpecificationItem;
		SpecificationItem tempUserSpecificationItem;
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
		if( linkedSpecificationWordTypeNr > Constants.NO_WORD_TYPE_NR )
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

			if( addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, linkedSpecificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a Chinese user specification with a relation to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			if( linkedSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
			// Make a specification substitution 'part of' assumption
			adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, generalizationWordItem, linkedSpecificationWordItem ) != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

			// Draw a specification substitution conclusion or ask a question
			if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, linkedSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, null ) != Constants.RESULT_OK )
				return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a Chinese specification substitution conclusion or ask a question for word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
														currentReadItem.isNoun() ? Constants.WORD_TYPE_NOUN_SINGULAR : currentReadItem.wordTypeNr() );
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

								if( isSelection &&
								adminItem_.createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a value selection item" );

								// Value, but no relation
								if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a value specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
								}
							}
						else
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

									( ( existingUserSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) == null ||
									!existingUserSpecificationItem.hasCompoundSpecificationCollection() ||
									existingUserSpecificationItem.isSpecificationGeneralization() ) &&

									// Collect specification words
									collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to collect specification words" );

									previousSpecificationWordItem = currentSpecificationWordItem;
									previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
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
										else
											{
											// No selection, no value, no relation
											if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nContextRelations, null, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a specification without relation to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}

										// Start reasoning
										if( isGeneralizationReasoningWordType &&
										!isNegative &&
										!isPartOf &&
										!isQuestion &&
										currentSpecificationWordItem != null &&
										adminItem_.isNounWordType( currentSpecificationWordTypeNr ) )
											{
											if( isPossessive )
												{
												if( isSpanishCurrentLanguage &&
												startRelationReadItem != null &&
												userSpecificationItem_ != null &&
												GlobalVariables.nUserRelationWords == 1 &&
												currentSpecificationWordItem.isNounWordSpanishAmbiguous() &&
												( lookAheadSpanishRelationWordItem = startRelationReadItem.lookAheadRelationWordItem() ) != null &&
												!lookAheadSpanishRelationWordItem.isMale() &&

												// Mark relation word as male
												lookAheadSpanishRelationWordItem.markWordAsMale() != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to mark relation word \"" + lookAheadSpanishRelationWordItem.anyWordTypeString() + "\" as male" );
												}
											else
												{
												if( isSpanishCurrentLanguage &&
												isGeneralizationProperNoun &&
												generalizationWordItem.isMale() &&
												// Display Spanish assumptions that are not hidden anymore
												displaySpanishAssumptionsThatAreNotHiddenAnymore( hasRelationWord, isAssignment, isArchivedAssignment, currentSpecificationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to display Spanish assumptions that are not hidden anymore" );

												// Draw proper noun part-of conclusions
												if( adminItem_.drawProperNounPartOfConclusions( false, isArchivedAssignment, ( isGeneralizationProperNoun ? generalizationWordItem : null ), ( isGeneralizationProperNoun ? currentSpecificationWordItem : generalizationWordItem ), null ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

												if( currentSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&
												// Make a specification substitution 'part of' assumption
												adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												// Draw a specification substitution conclusion or ask a question
												if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, null ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( !GlobalVariables.hasDisplayedWarning )
													{
													if( isGeneralizationProperNoun )
														{
														if( isSpanishCurrentLanguage &&
														!isExclusiveSpecification &&
														generalizationWordItem.isUserGeneralizationWord &&
														// Draw simple negative Spanish conclusions
														adminItem_.drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}
													else
														{
														// Definition specification
														if( ( compoundResult = adminItem_.drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a primary specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

														compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
														}
													}
												}

											if( isGeneralizationProperNoun &&
											!isUncountableGeneralizationNoun &&
											!GlobalVariables.hasDisplayedWarning &&
											GlobalVariables.nUserRelationWords == 1 &&
											startRelationReadItem != null &&
											( firstRelationReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null &&
											( relationWordItem = firstRelationReadItem.readWordItem() ) != null )
												{
												// Draw reversible conclusions
												if( adminItem_.drawReversibleConclusions( false, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( generalizationWordItem.isUserGeneralizationWord &&
												( tempUserSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, currentSpecificationWordItem ) ) != null &&
												tempUserSpecificationItem.hasSpecificationCollection() &&
												tempUserSpecificationItem.hasNewInformation() &&
												// Make an exclusive specification substitution assumption
												adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, firstRelationReadItem.wordTypeNr(), currentSpecificationWordTypeNr, generalizationWordTypeNr, relationWordItem, currentSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
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

					if( !GlobalVariables.hasDisplayedWarning &&
					!isQuestion &&
					currentSpecificationWordItem != null &&
					linkedSpecificationWordItem != null &&

					( isRelationWord ||
					isSpecificationWord ) &&

					( !hasRelationWord ||
					linkedSpecificationWordItem != currentSpecificationWordItem ) )
						{
						if( isGeneralizationReasoningWordType &&
						!isPreposition &&
						!isValueSpecificationWord &&
						linkedSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&

						( isGeneralizationProperNoun ||

						// Generalization word is noun
						( !isEveryGeneralization &&
						isPossessive ) ) &&

						// Make an exclusive specification substitution assumption
						adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationWord ? currentSpecificationWordItem : null ) ) != Constants.RESULT_OK )
							return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"" + linkedSpecificationWordItem.anyWordTypeString() + "\"" );

						if( isRelationWord ||
						hasLinkedPossessiveDeterminer )
							{
							if( !GlobalVariables.hasDisplayedWarning &&
							// Linked specification
							currentSpecificationWordItem == linkedSpecificationWordItem )
								{
								if( linkedGeneralizationWordItem_ != null )
									return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "Linked word \"" + linkedGeneralizationWordItem_.anyWordTypeString() + "\" is already assigned" );

								linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
								linkedGeneralizationWordItem_ = generalizationWordItem;
								}

							if( !GlobalVariables.hasDisplayedWarning &&
							isGeneralizationProperNoun &&
							userRelationContextNr_ > Constants.NO_CONTEXT_NR &&
							GlobalVariables.nUserRelationWords > 1 &&
							( tempUserSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, linkedSpecificationWordItem ) ) != null &&
							tempUserSpecificationItem.hasNewInformation() &&

							( currentSpecificationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, linkedSpecificationWordItem ) == null ||

							// Typical for Spanish. Test file: "Complejo (19 - extraño)"
							( isSpanishCurrentLanguage &&
							!tempUserSpecificationItem.isUserAssignment() &&
							( tempSpanishSpecificationItem = currentSpecificationWordItem.bestMatchingRelationContextNrSpecificationItem( true, true, false, true, linkedSpecificationWordItem, generalizationWordItem ) ) != null &&
							!tempSpanishSpecificationItem.isOlderItem() ) ) )
								{
								// Draw reversible conclusions
								if( adminItem_.drawReversibleConclusions( false, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( !isExclusiveSpecification &&
								// User specification doesn't exist
								tempUserSpecificationItem.hasSpecificationCollection() &&
								// Make an exclusive specification substitution assumption
								adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, currentSpecificationWordTypeNr, linkedSpecificationWordTypeNr, generalizationWordTypeNr, currentSpecificationWordItem, linkedSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
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
		while( !GlobalVariables.hasDisplayedWarning &&
		currentReadItem != endSpecificationReadItem &&
		( currentReadItem = currentReadItem.nextReadItem() ) != null );

		if( !hasAddedSpecification )
			return adminItem_.startErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I have not added any specification" );

		if( isGeneralizationProperNoun &&
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
					return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumptions of word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
			if( adminItem_.nContextWords( foundRelationContextNr, specificationWordItem ) >= GlobalVariables.nUserRelationWords )
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

	protected CreateAndAssignResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNounEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkipDrawingSpanishAmbiguousSubstitutionConclusion, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasDisplayedIntegrityWarning = false;
		boolean hasFoundRelationContext = false;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean isAssumption = adminItem_.isAssumption( assumptionJustificationTypeNr );
		boolean isOlderFoundSpecification;
		boolean isSelectedUniqueUserRelation;
		boolean isUserGeneralizationWord;
		short justificationTypeNr = assumptionJustificationTypeNr;
		short tempAssumptionLevel;
		JustificationItem createdJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem = null;
		JustificationItem foundJustificationItem = null;
		SpecificationItem createdSpecificationItem;
		SpecificationItem foundSpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		// Given assumption level has no value
		if( assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
			{
			// Calculate assumption level
			assumptionLevel = adminItem_.assumptionGrade( ( anotherPrimarySpecificationItem != null ), hasFeminineOrMasculineProperNounEnding, ( primarySpecificationItem != null && primarySpecificationItem.isGeneralizationProperNoun() && primarySpecificationItem.isPossessive() ), ( primarySpecificationItem != null && primarySpecificationItem.isQuestion() ), assumptionJustificationTypeNr );

			if( primarySpecificationItem != null &&
			( tempAssumptionLevel = primarySpecificationItem.assumptionLevel() ) > Constants.NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;

/*			if( anotherPrimarySpecificationItem != null &&
			( tempAssumptionLevel = anotherPrimarySpecificationItem.assumptionLevel() ) > Constants.NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;
*/
			if( secondarySpecificationItem != null &&
			( tempAssumptionLevel = secondarySpecificationItem.assumptionLevel() ) > Constants.NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;

			if( anotherSecondarySpecificationItem != null &&
			( tempAssumptionLevel = anotherSecondarySpecificationItem.assumptionLevel() ) > Constants.NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;

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

		if( ( justificationResult = generalizationWordItem.addJustification( hasFeminineOrMasculineProperNounEnding, isForcingNewJustification, false, justificationTypeNr, Constants.NO_ORDER_NR, GlobalVariables.currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a justification" );

		createdJustificationItem = justificationResult.createdJustificationItem;
		foundJustificationItem = justificationResult.foundJustificationItem;

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == null ? foundJustificationItem : createdJustificationItem ) ) == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I couldn't find or create a justification" );

		isSelectedUniqueUserRelation = ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION ||
										justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );

		if( ( createAndAssignResult = addSpecification( isAssignment, false, isArchivedAssignment, isCharacteristicFor, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isSelectedUniqueUserRelation, false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification" );

		if( !GlobalVariables.hasDisplayedWarning )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;
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
					if( createdJustificationItem != null &&
					createdJustificationItem.isReplacedOrDeletedItem() )
						return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The created justification item is replaced or deleted" );

					isOlderFoundSpecification = foundSpecificationItem.isOlderItem();

					// A justification has been created, but the self-generated specification already exists
					// So, the justification needs to be added or replaced separately

					// No justification is created
					if( ( createdJustificationItem == null &&

					( ( ( !isOlderFoundSpecification &&
					isPossessive ) ||

					// Typical for Spanish
					( isOlderFoundSpecification &&
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION ) ) &&

					!foundSpecificationItem.hasJustification( foundJustificationItem ) ) ) ||

					// Justification is created
					( createdJustificationItem != null &&

					( !isAssumption ||
					foundSpecificationItem.isSelfGeneratedAssumption() ||

					// Conclusion is forced to become an assumption
					( justificationTypeNr == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ||

					// Typical for Spanish (not possessive)
					( !isPossessive &&
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ||

					// Typical for Spanish (possessive)
					( isPossessive &&
					primarySpecificationItem != null &&

					( ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION &&
					foundSpecificationItem.hasCompoundSpecificationCollection() ) ||

					( justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
					hasGeneralizationWordCurrentlyConfirmedSpecification &&

					( primarySpecificationItem.isUserAssignment() ||
					foundSpecificationItem.hasOnlyOneRelationWord() ) ) ) ) ) ) ) )
						{
						if( processJustification( hasFoundRelationContext, isOlderFoundSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to process the created justification" );
						}
					}
				}
			else
				{
				if( hasFoundRelationContext &&
				!hasGeneralizationWordCurrentlyConfirmedSpecification &&
				foundSpecificationItem != null &&
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
					// Draw a unique relation conclusion
					addSelfGeneratedSpecification( false, false, false, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, false, true, false, true, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, null, null, createdSpecificationItem, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated unique specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					if( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN &&
					// Draw proper noun part-of conclusions
					adminItem_.drawProperNounPartOfConclusions( false, isArchivedAssignment, generalizationWordItem, specificationWordItem, ( !isUserGeneralizationWord && specificationWordItem.isNounWordSpanishAmbiguous() ? relationWordItem : null ) ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

					if( !isSkipDrawingSpanishAmbiguousSubstitutionConclusion )
						{
						// Make a specification substitution 'part of' assumption
						if( foundSpecificationItem == null &&
						createdSpecificationItem != null &&
						createdSpecificationItem.hasSpecificationCollection() &&
						adminItem_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						// Draw a specification substitution conclusion or ask a question
						if( adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
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
