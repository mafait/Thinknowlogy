/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		Creating specification and assignment structures
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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
	private boolean isSpanishCurrentLanguage_ = false;

	private short doneSpecificationWordOrderNr_ = Constants.NO_ORDER_NR;
	private short linkedGeneralizationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private int nPreviousChineseUserRelationWords_ = 0;
	private int userRelationContextNr_ = Constants.NO_CONTEXT_NR;

	private ReadItem previousChineseStartRelationReadItem_ = null;
	private ReadItem previousChineseEndRelationReadItem_ = null;

	private SpecificationItem simpleUserSpecificationItem_ = null;
	private SpecificationItem userSpecificationItem_ = null;

	private WordItem linkedGeneralizationWordItem_ = null;

	private final String moduleNameString_ = this.getClass().getName();

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

	private static boolean isValidCollection( boolean isPossessive, int relationCollectionNr, WordItem generalizationWordItem )
		{
		WordItem currentProperNounWordItem;

		if( relationCollectionNr == Constants.NO_COLLECTION_NR ||
		generalizationWordItem == null )
			return false;

		if( ( currentProperNounWordItem = GlobalVariables.firstProperNounWordItem ) != null )
			{
			// Do for all specification words
			do	{
				if( currentProperNounWordItem != generalizationWordItem &&
				!currentProperNounWordItem.isUserGeneralizationWord &&
				currentProperNounWordItem.firstCollectionSpecificationItem( isPossessive, relationCollectionNr ) != null )
					return false;
				}
			while( ( currentProperNounWordItem = currentProperNounWordItem.nextProperNounWordItem ) != null );
			}

		return true;
		}

	private static int collectionNrByCollectionWord( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int collectionNr;
		WordItem currentCollectionWordItem;

		if( compoundGeneralizationWordItem != null &&
		( currentCollectionWordItem = GlobalVariables.firstCollectionWordItem ) != null )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem != compoundGeneralizationWordItem &&
				( collectionNr = currentCollectionWordItem.collectionNrByCompoundGeneralizationWordInWord( collectionWordTypeNr, compoundGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem.nextCollectionWordItem ) != null );
			}

		return Constants.NO_COLLECTION_NR;
		}

	private static int collectionNrByExclusiveSpecification( short collectionWordTypeNr, WordItem compoundGeneralizationWordItem )
		{
		int collectionNr;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;

		if( compoundGeneralizationWordItem != null &&
		( currentSpecificationItem = compoundGeneralizationWordItem.firstExclusiveSpecificationItem( false ) ) != null )
			{
			// Do for all exclusive specifications of this word (skip adjectives)
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) != null &&
				( collectionNr = currentSpecificationWordItem.collectionNrByCompoundGeneralizationWordInWord( collectionWordTypeNr, compoundGeneralizationWordItem ) ) > Constants.NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextExclusiveSpecificationItem( false ) ) != null );
			}

		return Constants.NO_COLLECTION_NR;
		}

	private byte addUserSpecificationWithRelation( boolean isAction, boolean isAssignedOrClear, boolean isAssignment, boolean isCharacteristicFor, boolean isChineseReasoning, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpanishCurrentLanguage, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, short imperativeVerbParameter, short questionParameter, short selectionLevel, short selectionListNr, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int nEnteredRelationWords, int nUserRelationWords, ReadItem startRelationReadItem, ReadItem endRelationReadItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isChineseReversedImperativeNoun;
		boolean isFirstComparisonPart = ( selectionListNr == Constants.ADMIN_CONDITION_LIST );
		boolean isFirstRun = true;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		short relationWordTypeNr;
		int specificationCollectionNr;
		int relationCollectionNr = Constants.NO_COLLECTION_NR;
		ReadItem currentRelationReadItem = startRelationReadItem;
		SpecificationItem chineseUserSpecificationItem;
		SpecificationItem definitionSpecificationItem;
		SpecificationItem existingSpecificationItem;
		SpecificationItem olderUserSpecificationItem;
		WordItem mostRecentContextWord;
		WordItem previousRelationWordItem = null;
		WordItem relationWordItem;
		CollectionResultType collectionResult;

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
							( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationWordItem ) ) != null &&
							existingSpecificationItem.hasRelationWord() &&
							( mostRecentContextWord = existingSpecificationItem.mostRecentRelationWord() ) != null &&
							mostRecentContextWord != relationWordItem )
								{
								previousRelationWordItem = mostRecentContextWord;

								if( existingSpecificationItem.hasNonCompoundSpecificationCollection() &&
								!existingSpecificationItem.isSpecificationWordSpanishAmbiguous() )
									specificationCollectionNr = existingSpecificationItem.specificationCollectionNr();
								}

							if( !isQuestion &&
							previousRelationWordItem != null &&

								// Avoid looping
								( relationWordItem.collectionNr( specificationWordItem ) == Constants.NO_COLLECTION_NR ||
								previousRelationWordItem.collectionNr( specificationWordItem ) == Constants.NO_COLLECTION_NR ) &&

							// Test file: "Paul has a parent, called John - Paul has a parent, called Anna"
							( ( olderUserSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, previousRelationWordItem ) ) == null ||
							!olderUserSpecificationItem.isOlderItem() ) )
								{
								// Collect relation words
								if( ( collectionResult = collectRelationWords( isExclusiveSpecification, false, relationWordTypeNr, specificationWordTypeNr, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, previousRelationWordItem, relationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

								if( ( relationCollectionNr = collectionResult.createdCollectionNr ) > Constants.NO_COLLECTION_NR &&
								( existingSpecificationItem = generalizationWordItem.bestMatchingRelationSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, previousRelationWordItem ) ) != null &&
								// Add relation collection number to found user specification
								generalizationWordItem.changeSpecification( existingSpecificationItem.isAnsweredQuestion(), existingSpecificationItem.isEveryGeneralization(), existingSpecificationItem.generalizationCollectionNr(), existingSpecificationItem.specificationCollectionNr(), relationCollectionNr, existingSpecificationItem.firstJustificationItem(), existingSpecificationItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a relation collection number to the existing user specification, having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
								}

							if( selectionListNr == Constants.NO_LIST_NR )
								{
								if( !isChineseReasoning )
									{
									if( relationCollectionNr == Constants.NO_COLLECTION_NR &&
									isAssignment &&
									userSpecificationItem_ != null &&
									userSpecificationItem_.hasCompoundSpecificationCollection() &&
									!userSpecificationItem_.isOlderItem() &&
									!userSpecificationItem_.isSpecificationWordSpanishAmbiguous() )
										// Proper noun user assignment
										relationCollectionNr = userSpecificationItem_.relationCollectionNr();

									if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nEnteredRelationWords, null, generalizationWordItem, specificationWordItem, relationWordItem, null ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a specification having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

									if( !isQuestion &&
									!isSpanishCurrentLanguage &&
									( definitionSpecificationItem = specificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() ) != null &&

										// Conflicting sentence
										( userSpecificationItem_ == null ||

										// Specification already exists
										( ( existingSpecificationItem = generalizationWordItem.firstSpecificationItem( false, false, false, specificationWordItem ) ) != null &&

											// Not first run
											( ( !isFirstRun &&
											!isAssignment &&
											userSpecificationItem_.hasNonCompoundSpecificationCollection() &&
											!userSpecificationItem_.isOlderItem() ) ||

											// First run
											( isFirstRun &&

												( ( ( !isAssignment ||
												// Typical for Chinese
												isChineseCurrentLanguage_ ) &&

												!definitionSpecificationItem.isAdjectiveSpecification() ) ||

												( ( !isAssignment ||
												isArchivedAssignment ||
												// Test file: "Paul is the son of John and Anna"
												definitionSpecificationItem.isExclusiveSpecification() ) &&

												!existingSpecificationItem.hasRelationCollection() ) ) ) ) ) ) &&

									// Draw specification substitution conclusion or ask a question
									adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
										return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

									isFirstRun = false;
									}
								else	// Typical for Chinese
									{
									if( userSpecificationItem_.hasCompoundSpecificationCollection() )
										{
										// Make Chinese exclusive specification substitution assumption (non-possessive)
										if( adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, true, isExclusiveSpecification, isNegative, isPossessive, false, isUncountableGeneralizationNoun, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make a Chinese exclusive specification substitution assumption with specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );

										if( ( chineseUserSpecificationItem = generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, specificationWordItem, null ) ) != null &&
										!chineseUserSpecificationItem.isOlderItem() &&
										// Make Chinese exclusive specification substitution assumption (possessive)
										adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, true, isExclusiveSpecification, false, !isPossessive, false, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, specificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make a Chinese exclusive specification substitution assumption with relation word \"" + relationWordItem.anyWordTypeString() + "\" and specification word \"" + specificationWordItem.anyWordTypeString() + "\"" );
										}
									}
								}
							else
								{
								// Typical for Chinese
								isChineseReversedImperativeNoun = ( isChineseCurrentLanguage_ &&
																	relationWordItem.isChineseReversedImperativeNoun() );

								if( adminItem_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem.isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, ( isChineseReversedImperativeNoun ? relationWordTypeNr : specificationWordTypeNr ), ( isChineseReversedImperativeNoun ? specificationWordTypeNr : relationWordTypeNr ), relationCollectionNr, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nEnteredRelationWords, generalizationWordItem, ( isChineseReversedImperativeNoun ? relationWordItem : specificationWordItem ), ( isChineseReversedImperativeNoun ? specificationWordItem : relationWordItem ), null ) != Constants.RESULT_OK )
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
			if( ( collectionResult = previousGeneralizationWordItem.createCollectionItem( true, isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, currentGeneralizationWordItem, newCommonWordItem, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\" with current word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );

			if( generalizationCollectionNr == Constants.NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			// Collect current word
			if( currentGeneralizationWordItem.createCollectionItem( false, isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, null ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect current word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\" with previous word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			// Collect generalizations in previous word
			if( previousGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in previous word \"" + previousGeneralizationWordItem.anyWordTypeString() + "\"" );

			// Collect generalizations in current word
			if( currentGeneralizationWordItem.collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect generalizations in current word \"" + currentGeneralizationWordItem.anyWordTypeString() + "\"" );
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

			( specificationWordTypeNr != Constants.WORD_TYPE_NOUN_PLURAL ||
			generalizationWordTypeNr != Constants.WORD_TYPE_NOUN_SINGULAR ) )
				specificationCollectionNr = currentSpecificationWordItem.collectionNr();
			}
		else
			{
			if( isExclusiveSpecification &&

				( generalizationWordItem == previousSpecificationWordItem ||
				!generalizationWordItem.isNounWordSpanishAmbiguous() ) )
				specificationCollectionNr = ( generalizationWordItem.isFeminineOrMasculineWord() ? collectionNrByExclusiveSpecification( specificationWordTypeNr, compoundGeneralizationWordItem ) :
																									collectionNrByCollectionWord( specificationWordTypeNr, compoundGeneralizationWordItem ) );
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
			if( ( collectionResult = previousSpecificationWordItem.createCollectionItem( true, isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect previous word \"" + previousSpecificationWordItem.anyWordTypeString() + "\" with current word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );

			if( specificationCollectionNr == Constants.NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem.createCollectionItem( false, isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to collect current word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" with previous word \"" + previousSpecificationWordItem.anyWordTypeString() + "\"" );
			}

		if( specificationCollectionNr > Constants.NO_COLLECTION_NR &&
		generalizationWordItem.collectSpecificationsInWord( isExclusiveSpecification, isQuestion, specificationCollectionNr ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to collect specifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		return Constants.RESULT_OK;
		}

	private byte displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords( boolean isAssignment, boolean isArchivedAssignment, WordItem specificationWordItem )
		{
		SpecificationItem hiddenSpanishSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem hiddenSpanishSpecificationWordItem;

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationWordItem = GlobalVariables.firstSpecificationWordItem ) != null )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem.isProperNounWord() &&
				( hiddenSpanishSpecificationItem = currentSpecificationWordItem.firstUnhiddenSpanishSpecificationItem() ) != null )
					{
					if( !isAssignment )
						{
						currentSpecificationWordItem.hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = true;

						// Write Spanish assumption
						if( currentSpecificationWordItem.writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write a Spanish assumption about word \"" + currentSpecificationWordItem.anyWordTypeString() + "\" that is not hidden anymore" );
						}

					if( ( hiddenSpanishSpecificationWordItem = hiddenSpanishSpecificationItem.specificationWordItem() ) != null &&
					// Draw Spanish proper noun part-of conclusions
					adminItem_.drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, false, true, hiddenSpanishSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
					}
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
			}

		return Constants.RESULT_OK;
		}

	private byte processJustification( boolean hasRelationWordConfirmedSpecification, boolean isOlderFoundSpecification, boolean isUserGeneralizationWord, short justificationTypeNr, JustificationItem createdOrFoundJustificationItem, JustificationItem firstJustificationItem, SpecificationItem foundSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundSpecificationCurrentCreationSentenceNr;
		boolean isRemovingPreviousJustifications = false;
		JustificationItem obsoleteJustificationItem = null;
		JustificationItem questionJustificationItem;
		SpecificationItem assumptionSpecificationItem;

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

		if( foundSpecificationItem.isReplacedOrDeletedItem() )
			return adminItem_.startError( 1, moduleNameString_, "The given found specification item is undefined" );

		hasFoundSpecificationCurrentCreationSentenceNr = foundSpecificationItem.hasCurrentCreationSentenceNr();

		if( relationWordItem != null )
			hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();

		switch( justificationTypeNr )
			{
			case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
				if( isUserGeneralizationWord )
					{
					// Typical for Spanish
					if( foundSpecificationItem.isHiddenSpanishSpecification() &&
					!foundSpecificationItem.isOlderItem() )
						obsoleteJustificationItem = foundSpecificationItem.firstReplacedSecondarySpecificationJustificationItem();
					}

				break;

			case Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
				// Typical for Spanish
				if( foundSpecificationItem.isSpecificationWordSpanishAmbiguous() )
					{
					if( !hasRelationWordConfirmedSpecification &&
					primarySpecificationItem.isUserAssignment() )
						obsoleteJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem );
					}
				else
					{
					if( isOlderFoundSpecification &&
					!isUserGeneralizationWord &&

						( !hasFoundSpecificationCurrentCreationSentenceNr ||
						// Typical for Spanish
						isSpanishCurrentLanguage_ ) &&

					foundSpecificationItem.hasCompoundSpecificationCollection() &&
					foundSpecificationItem.hasOnlyOneRelationWord() )
						isRemovingPreviousJustifications = true;
					}

				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				if( foundSpecificationItem.hasAssumptionLevel() )
					{
					if( primarySpecificationItem.isCorrectedSpecification() )
						// Test files: "reasoning\family\Correcting invalidated assumption (by knowledge)",
						//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
						obsoleteJustificationItem = foundSpecificationItem.firstReplacedPrimarySpecificationJustificationItem();
					else
						{
						// Typical for Spanish
						if( primarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&
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
				primarySpecificationItem.hasRelationWord() ) &&

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
						if( ( questionJustificationItem = foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION ) ) != null &&
						( assumptionSpecificationItem = questionJustificationItem.primarySpecificationItem() ) != null &&
						assumptionSpecificationItem.assumptionLevel() > primarySpecificationItem.assumptionLevel() )
							// Remove less certain justification from question
							isRemovingPreviousJustifications = true;
						}
					else
						{
						// Remove obsolete justifications from adjusted compound question
						isRemovingPreviousJustifications = true;

						// Clear stored sentence buffer, because of adjusted question
						foundSpecificationItem.clearStoredSentenceStringBuffer();
						}
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
				// Copy and replace found specification, with different first justification item
				if( generalizationWordItem.changeSpecification( foundSpecificationItem.isAnsweredQuestion(), foundSpecificationItem.isEveryGeneralization(), foundSpecificationItem.generalizationCollectionNr(), foundSpecificationItem.specificationCollectionNr(), foundSpecificationItem.relationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to copy and replace the found specification, with a different first justification item" );

				foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();
				}

			if( justificationTypeNr == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION &&
			// Recalculate assumption level of found specification
			foundSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );
			}
		else
			{
			if( obsoleteJustificationItem == null )
				{
				// Attach created justification to the found specification
				if( foundSpecificationItem.attachJustificationToSpecification( createdOrFoundJustificationItem ) != Constants.RESULT_OK )
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

	private CollectionResultType copyCollection( boolean isExclusiveSpecification, short relationWordTypeNr, short commonWordTypeNr, int currentRelationCollectionNr, WordItem specificationWordItem, WordItem previousRelationWordItem )
		{
		boolean isForcingNewCollectionNr = true;
		int previousRelationCollectionNr = Constants.NO_COLLECTION_NR;
		int relationCollectionNr = Constants.NO_COLLECTION_NR;
		WordItem currentCollectionWordItem;
		CollectionResultType collectionResult;
		CollectionResultType returnCollectionResult = new CollectionResultType();

		if( specificationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( previousRelationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given previous relation word item is undefined" );

		if( ( currentCollectionWordItem = WordItem.firstCollectionWordItem( currentRelationCollectionNr ) ) != null )
			{
			// Do for all collection words
			do	{
				if( previousRelationWordItem != currentCollectionWordItem )
					{
					// Collect relation words
						if( ( collectionResult = collectRelationWords( isExclusiveSpecification, isForcingNewCollectionNr, relationWordTypeNr, commonWordTypeNr, previousRelationCollectionNr, relationCollectionNr, previousRelationWordItem, currentCollectionWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + currentCollectionWordItem.anyWordTypeString() + "\"" );

					isForcingNewCollectionNr = false;
					relationCollectionNr = collectionResult.createdCollectionNr;
					previousRelationWordItem = currentCollectionWordItem;
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem._nextCollectionWordItem( currentRelationCollectionNr ) ) != null );
			}

		returnCollectionResult.createdCollectionNr = relationCollectionNr;
		return returnCollectionResult;
		}

	private CollectionResultType collectRelation( boolean isAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isQuestion, boolean isSpecificationWordSpanishAmbiguous, short specificationWordTypeNr, short relationWordTypeNr, int relationCollectionNr, SpecificationItem existingSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem previousRelationWordItem, WordItem relationWordItem )
		{
		boolean hasExistingSpecificationCompoundCollection;
		boolean hasExistingSpecificationCurrentCreationSentenceNr;
		boolean hasExistingSpecificationUpdatedInformation;
		boolean hasGeneralizationWordCurrentlyConfirmedSpecification;
		boolean hasRelationWordConfirmedSpecification;
		boolean hasUserSpecificationRelationCollection = false;
		boolean isExistingHiddenSpanishSpecification;
		boolean isExistingUserSpecification;
		boolean isForcingNewCollectionNr = false;
		boolean isOlderExistingSpecification;
		boolean isPossessiveUserSpecification = false;
		boolean isUserAssignment = false;
		boolean isUserPreviousRelationWord;
		boolean isUserRelationWord;
		boolean isUserSpecificationWord;
		int currentRelationCollectionNr;
		int existingRelationCollectionNr;
		int previousRelationCollectionNr;
		WordItem existingRelationWordItem;
		SpecificationItem tempSpecificationItem = null;
		CollectionResultType collectionResult = new CollectionResultType();

		if( existingSpecificationItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given existing specification item is undefined" );

		if( generalizationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( previousRelationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given previous relation word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given relation word item is undefined" );

		hasExistingSpecificationCompoundCollection = existingSpecificationItem.hasCompoundSpecificationCollection();
		hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem.hasCurrentCreationSentenceNr();
		hasExistingSpecificationUpdatedInformation = existingSpecificationItem.hasUpdatedInformation();
		isExistingHiddenSpanishSpecification = existingSpecificationItem.isHiddenSpanishSpecification();
		isExistingUserSpecification = existingSpecificationItem.isUserSpecification();
		isOlderExistingSpecification = existingSpecificationItem.isOlderItem();
		existingRelationCollectionNr = existingSpecificationItem.relationCollectionNr();
		existingRelationWordItem = existingSpecificationItem.relationWordItem();

		if( userSpecificationItem_ != null )
			{
			hasUserSpecificationRelationCollection = userSpecificationItem_.hasRelationCollection();
			isUserAssignment = userSpecificationItem_.isAssignment();
			isPossessiveUserSpecification = userSpecificationItem_.isPossessive();
			}

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem.hasCurrentlyConfirmedSpecification();
		isUserRelationWord = generalizationWordItem.isUserRelationWord;
		isUserSpecificationWord = specificationWordItem.isUserSpecificationWord;

		isUserPreviousRelationWord = previousRelationWordItem.isUserRelationWord;
		previousRelationCollectionNr = previousRelationWordItem.collectionNr( specificationWordItem );

		hasRelationWordConfirmedSpecification = relationWordItem.hasCurrentlyConfirmedSpecification();
		currentRelationCollectionNr = relationWordItem.collectionNr( specificationWordItem );

		if( !isExclusiveSpecification &&
		previousRelationWordItem != relationWordItem &&

		// Existing specification is not current
		( ( !hasExistingSpecificationCurrentCreationSentenceNr &&

			// Not user specification word
			( ( !isUserSpecificationWord &&

				// Non-possessive
				( ( !isPossessive &&

					// Not user relation word
					// Test file: "reasoning\family\John - Anna (before family definition)"
					( ( !isUserRelationWord &&
					!hasGeneralizationWordCurrentlyConfirmedSpecification &&
					isExistingUserSpecification &&
					currentRelationCollectionNr == Constants.NO_COLLECTION_NR ) ||

					// User relation word
					( isUserRelationWord &&

						// No current relation collection
						( ( currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&

							// No existing relation collection
							( ( existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&
							existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) &&

								// Test files: "reasoning\family\Complex (18)",
								//				"reasoning\family\Complex (19 - mixed)",
								//				"reasoning\family\Complex (19 - strange)"
								( !isExistingHiddenSpanishSpecification ||
								// Typical for Spanish
								// Spanish test file: "razonamiento\familia\Complejo (13)"
								previousRelationCollectionNr == Constants.NO_COLLECTION_NR ) ) ||

							// Existing relation collection
							// Test files: "reasoning\family\Complex (7 - Becky)",
							//				"reasoning\family\Complex (7 - Claudia)",
							//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
							( existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&
							isPossessiveUserSpecification &&
							// Copy collection
							!isValidCollection( isPossessive, existingRelationCollectionNr, generalizationWordItem ) ) ) ) ||

						// Current relation collection
						// Test files: "reasoning\family\Complex (12)",
						//				"reasoning\family\Complex (16)"
						( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&
						hasRelationWordConfirmedSpecification &&
						!isExistingHiddenSpanishSpecification &&
						existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&
						existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == null ) ) ) ) ) ||

				// Possessive
				( isPossessive &&
				currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&
				userSpecificationItem_ != null &&
				userSpecificationItem_.hasCompoundSpecificationCollection() &&

					// Not user relation word
					( ( !isUserRelationWord &&
					hasUserSpecificationRelationCollection &&

						( existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&

							// Test files: "reasoning\family\Complex (20)",
							//				"reasoning\family\John - Laura - John and Anna",
							//				"reasoning\family\John is a parent - Paul - Laura",
							//				"reasoning\family\John is the father - Paul - Laura"
							( !isUserAssignment ||
							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Luisa - José - Ana"
							previousRelationCollectionNr > Constants.NO_COLLECTION_NR ) ) ) ||

					// User relation word
					( isUserRelationWord &&
					// Typical for Spanish
					isSpecificationWordSpanishAmbiguous &&
					// Spanish test files: "razonamiento\familia\Complejo (7 - Becky)",
					//						"razonamiento\familia\Complejo (7 - Bonita)",
					//						"razonamiento\familia\Definición sencilla de la familia - Complejo (7 - Bonita)"
					isPossessiveUserSpecification &&
					!existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) ) ) ||

			// User specification word
			( isUserSpecificationWord &&
			currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&

				// Existing specification is not user specification
				// Test file: "Paul has a father, called Anna"
				( ( ( !isExistingUserSpecification &&
				isPossessive &&
				userSpecificationItem_ == null ) ||

				// Existing specification is user specification
				( isExistingUserSpecification &&

					// Not user relation word
					// Test file: "Paul has a parent, called John - Paul has a parent, called Anna"
					( ( !isUserRelationWord &&
					isPossessive ) ||

					// User relation word
					( isUserRelationWord &&

						// Test files: "reasoning\family\Complex (7 - Becky)",
						//				"reasoning\family\Complex (7 - Claudia)",
						//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
						( isAssignment ||
						// Test file: "reasoning\John is a parent of Paul - Joe has 2 parents"
						existingRelationCollectionNr == Constants.NO_COLLECTION_NR ) ) ) ) ) ||

				// Typical for Spanish
				// Spanish test file: "Paz es el hijo Juan y Ana"
				( isSpecificationWordSpanishAmbiguous &&
				!isExistingHiddenSpanishSpecification &&
				isPossessive &&
				!isUserAssignment ) ) ) ) ) ||

		// Existing specification is current
		( hasExistingSpecificationCurrentCreationSentenceNr &&

			// Not confirmed generalization word
			( ( !hasGeneralizationWordCurrentlyConfirmedSpecification &&

				// Collect relation words
				( ( existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&
				!isUserSpecificationWord &&

					// Non-possessive
					( ( !isPossessive &&
					isOlderExistingSpecification &&
					currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&
					!isValidCollection( isPossessive, currentRelationCollectionNr, generalizationWordItem ) &&

						// Not user relation word
						// Test file: "reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
						( ( !isUserRelationWord &&
						( tempSpecificationItem = relationWordItem.firstSpecificationItem( false, true, false, specificationWordItem ) ) != null &&
						tempSpecificationItem.assumptionLevel() < existingSpecificationItem.assumptionLevel() ) ||

						// User relation word
						( isUserRelationWord &&
						// Typical for Spanish
						isSpecificationWordSpanishAmbiguous &&
						// Spanish test file: "razonamiento\familia\Complejo (3)"
						!isExistingHiddenSpanishSpecification ) ) ) ||

					// Possessive
					( isPossessive &&
					!isOlderExistingSpecification &&
					isUserAssignment &&
					!isUserRelationWord &&

						// No current relation collection
						// Test file: "reasoning\family\John - Anna (before family definition)"
						( ( currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&
						existingSpecificationItem.hasSecondaryUserSpecificationJustification() ) ||

						( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&
						// Typical for Spanish
						isSpecificationWordSpanishAmbiguous &&
						// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
						!isExistingHiddenSpanishSpecification &&
						!isValidCollection( isPossessive, currentRelationCollectionNr, generalizationWordItem ) ) ) ) ) ) ||

				// Copy collection
				( existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&
				!hasRelationWordConfirmedSpecification &&
				currentRelationCollectionNr != existingRelationCollectionNr &&

					( ( !isSpecificationWordSpanishAmbiguous &&

						// Test files: "reasoning\family\Complex (7 - Becky)",
						//				"reasoning\family\Complex (7 - Claudia)",
						//				"reasoning\family\Complex (19 - strange)",
						//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
						( isPossessive ||
						// Test file: "reasoning\family\Complex (19 - strange)"
						!isUserRelationWord ) &&

						!isValidCollection( isPossessive, existingRelationCollectionNr, generalizationWordItem ) ) ||

					// Typical for Spanish
					( isSpecificationWordSpanishAmbiguous &&
					// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
					!isExistingHiddenSpanishSpecification &&
					isUserRelationWord ) ) ) ) ) ||

			// Confirmed generalization word
			( hasGeneralizationWordCurrentlyConfirmedSpecification &&
			!isPossessiveUserSpecification &&
			existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&
			previousRelationCollectionNr > Constants.NO_COLLECTION_NR &&

				// Existing specification is not user specification
				( ( !isExistingUserSpecification &&
				// Typical for Spanish
				isSpecificationWordSpanishAmbiguous &&
				// Spanish test files: "razonamiento\familia\Complejo (6)",
				//						"razonamiento\familia\Complejo (11)"
				!isExistingHiddenSpanishSpecification ) ||

				// Existing specification is user specification
				( isExistingUserSpecification &&
				// Test file: "reasoning\family\I know (10)"
				previousRelationCollectionNr != currentRelationCollectionNr ) ) &&

			!isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) ) ) ) ) )
			{
			if( existingRelationCollectionNr == Constants.NO_COLLECTION_NR )
				{
				if( ( collectionResult = collectRelationWords( isExclusiveSpecification, true, relationWordTypeNr, specificationWordTypeNr, previousRelationCollectionNr, relationCollectionNr, previousRelationWordItem, relationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

				// New collection number
				relationCollectionNr = collectionResult.createdCollectionNr;
				}
			else
				{
				// Copy collection
				if( ( collectionResult = copyCollection( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, existingRelationCollectionNr, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to copy a relation collection" );

				// New collection number
				relationCollectionNr = collectionResult.createdCollectionNr;
				}
			}

		if( ( ( relationCollectionNr == Constants.NO_COLLECTION_NR &&

			( previousRelationWordItem != relationWordItem ||

			( !hasExistingSpecificationUpdatedInformation &&
			!isExistingHiddenSpanishSpecification &&
			existingRelationCollectionNr > Constants.NO_COLLECTION_NR ) ) ) ||

		( !hasRelationWordConfirmedSpecification &&
		existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&

			( previousRelationWordItem != relationWordItem ||
			// Typical for Spanish
			// Spanish test files: "respuestas a las preguntas\familia\Relación pregunta es subconjunto de relación de respuesta",
			//						"respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)"
			!hasExistingSpecificationCurrentCreationSentenceNr ) ) ) &&

		// Test file: "ambiguity\US presidents"
		!existingSpecificationItem.isExclusiveGeneralization() )
			{
			if( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&

				( !hasExistingSpecificationCurrentCreationSentenceNr ||
				!hasUserSpecificationRelationCollection ||
				isPossessiveUserSpecification ||
				existingRelationCollectionNr == Constants.NO_COLLECTION_NR ||

				( ( !isUserSpecificationWord ||
				previousRelationCollectionNr == existingRelationCollectionNr ) &&

					( isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) ||
					WordItem.nRelationWords( previousRelationCollectionNr, previousRelationWordItem ) >= WordItem.nRelationWords( currentRelationCollectionNr, relationWordItem ) ) ) ) )
				{
				relationCollectionNr = currentRelationCollectionNr;

				if( isUserRelationWord &&
				!isSpecificationWordSpanishAmbiguous &&
				existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&
				previousRelationCollectionNr == relationCollectionNr &&
				existingRelationCollectionNr != relationCollectionNr )
					{
					previousRelationCollectionNr = existingRelationCollectionNr;
					relationCollectionNr = existingRelationCollectionNr;
					}
				}

			if( userSpecificationItem_ != null &&

				// No current relation collection
				( ( existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&
				previousRelationCollectionNr > Constants.NO_COLLECTION_NR &&
				!isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) &&

					// Non-possessive
					( ( !isPossessive &&

						( hasExistingSpecificationCurrentCreationSentenceNr ||

						// Not user relation word
						( !isUserRelationWord &&
						// Typical for Spanish
						// Spanish test file: "razonamiento\familia\Luisa - José - Ana"
						existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) ||

						// User relation word
						( isUserRelationWord &&

							( ( isExistingHiddenSpanishSpecification &&

								// Spanish test files: "razonamiento\familia\Complejo (12)",
								//						"razonamiento\familia\Complejo (13)",
								//						"razonamiento\familia\Complejo (16)",
								//						"razonamiento\familia\Complejo (18)"
								( !isUserAssignment ||
								// Spanish test file: "razonamiento\familia\Complejo (19 - mezclado)"
								currentRelationCollectionNr == Constants.NO_COLLECTION_NR ) ) ||

							// Test files: "reasoning\family\Complex (4)",
							//				"reasoning\family\Complex (5)",
							//				"reasoning\family\Complex (6)",
							//				"reasoning\family\Paul - Anna - Joe - Laura - John",
							//				among others
							existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != null ) ) ) ) ||

					// Possessive
					( isPossessive &&
					isUserRelationWord &&

						( hasExistingSpecificationCurrentCreationSentenceNr ||
						!isUserAssignment ||
						isSpecificationWordSpanishAmbiguous ) ) ) ) ||

				// Current relation collection
				( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&

					// Not user specification word
					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				among others
					( ( !isUserSpecificationWord &&
					!hasExistingSpecificationCurrentCreationSentenceNr &&
					!isPossessive &&
					!isSpecificationWordSpanishAmbiguous &&
					existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&

						( isUserAssignment ||
						// Test file: "reasoning\family\Joe has 2 parents - Anna - Paul"
						userSpecificationItem_.hasCompoundSpecificationCollection() ) ) ||

					// User specification word
					( isUserSpecificationWord &&

						( ( isQuestion &&

							// Test file: "question answering\family\Just a few questions (1)"
							( !isExistingHiddenSpanishSpecification ||
							// Typical for Spanish
							// Spanish test file: "respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)"
							existingRelationCollectionNr != currentRelationCollectionNr ) ) ||

						( isUserRelationWord &&

							// Test files: "reasoning\family\Complex (15)",
							//				"reasoning\family\I know (12)",
							//				among others
							( ( hasExistingSpecificationCompoundCollection &&
							!isPossessive &&
							existingRelationCollectionNr > Constants.NO_COLLECTION_NR ) ||

							( hasExistingSpecificationCurrentCreationSentenceNr &&
							existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&

								// Test files: "reasoning\family\Complex (2)",
								//				"reasoning\family\Complex (3)",
								//				"reasoning\family\Complex (7 - Becky)",
								//				among others
								( !isPossessive ||
								// Typical for Spanish
								// Spanish test files: "conflicto\familia\José es una madre",
								//						"razonamiento\familia\Complejo (9)",
								//						"razonamiento\familia\Complejo (13)",
								//						"razonamiento\familia\Complejo (14)",
								//						"razonamiento\familia\Complejo (18)",
								//						among others
								isSpecificationWordSpanishAmbiguous ) ) ||

							( !hasRelationWordConfirmedSpecification &&

								( ( existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&

									// Test file: "reasoning\family\You have confirmed at least one relation word"
									( !isPossessive ||
									// Typical for Spanish
									// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
									isSpecificationWordSpanishAmbiguous ) ) ||

								// Typical for Spanish
								// Spanish test files: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)",
								//						"razonamiento\familia\Ana - Juan - Paz, José y Luisa"
								isSpecificationWordSpanishAmbiguous ) ) ) ) ) ) ) ) ) )
				{
				if( ( !hasExistingSpecificationUpdatedInformation &&

					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				among others
					( isUserAssignment ||
					// Test files: "reasoning\family\Complex (4)",
					//				"reasoning\family\Complex (5)",
					//				among others
					hasExistingSpecificationCurrentCreationSentenceNr ||
					// Test file: "reasoning\family\Complex (16)"
					hasRelationWordConfirmedSpecification ||
					existingRelationCollectionNr == relationCollectionNr ||

					// Test files: "reasoning\family\Complex (19 - strange)",
					//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
					( hasUserSpecificationRelationCollection &&
					isUserRelationWord ) ) ) ||

				// Typical for Spanish
				( isSpecificationWordSpanishAmbiguous &&

					// Typical for Spanish
					( isExistingHiddenSpanishSpecification ||

					( hasUserSpecificationRelationCollection &&
					existingRelationCollectionNr == relationCollectionNr ) ) ) )
					{
					// Correct collection doesn't exist yet
					isForcingNewCollectionNr = true;

					// Typical for Spanish
					if( isExistingHiddenSpanishSpecification &&
					existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&
					// Add relation collection number to found user specification
					generalizationWordItem.changeSpecification( existingSpecificationItem.isAnsweredQuestion(), existingSpecificationItem.isEveryGeneralization(), existingSpecificationItem.generalizationCollectionNr(), existingSpecificationItem.specificationCollectionNr(), relationCollectionNr, existingSpecificationItem.firstJustificationItem(), existingSpecificationItem ) != Constants.RESULT_OK )
						return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to add a relation collection number to the existing specification, having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
					}

				// Existing specification is wrong. Find correct specification
				if( ( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedConclusionSpecificationItem( isNegative, isPossessive, isUserAssignment, specificationWordItem ) ) != null &&
				existingSpecificationItem != tempSpecificationItem )
					{
					if( hasRelationWordConfirmedSpecification &&
					tempSpecificationItem.relationWordItem() != previousRelationWordItem )
						// Test files: "reasoning\family\I know (3)",
						//				"reasoning\family\I know (10)",
						//				"reasoning\family\Laura - John and Anna - Joe",
						//				"reasoning\family\My assumptions that are confirmed (parents)",
						//				"reasoning\family\My assumptions that are confirmed (John and Anna)",
						//				"reasoning\family\My assumptions that are confirmed (John and Anna - reversed)",
						//				"reasoning\family\My conclusions that are confirmed (Joe has 2 parents)"
						previousRelationWordItem = tempSpecificationItem.relationWordItem();

					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				"reasoning\family\I know (3)",
					//				"reasoning\family\I know (10)",
					//				"reasoning\family\Laura - John and Anna - Joe",
					//				"reasoning\family\My assumptions that are confirmed (parents)",
					//				"reasoning\family\My assumptions that are confirmed (John and Anna)",
					//				"reasoning\family\My assumptions that are confirmed (John and Anna - reversed)",
					//				"reasoning\family\My conclusions that are confirmed (Joe has 2 parents)"
					existingSpecificationItem = tempSpecificationItem;
					existingRelationCollectionNr = existingSpecificationItem.relationCollectionNr();
					}
				}

			// Not forcing new collection
			if( ( !isForcingNewCollectionNr &&
			!hasExistingSpecificationCurrentCreationSentenceNr &&
			!isExclusiveSpecification &&

				// No current relation collection
				( ( currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&
				!isPossessive &&
				relationCollectionNr == Constants.NO_COLLECTION_NR &&

					// Existing specification is not user specification
					( ( !isExistingUserSpecification &&

						// Not user specification word
						// Test files: "reasoning\family\Joe has 2 parents - Joe is a child",
						//				"reasoning\family\Joe has 2 parents - Joe is a son",
						//				among others
						( ( !isUserSpecificationWord &&
						!isPossessiveUserSpecification &&
						!isUserRelationWord &&
						existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&
						existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ||

						// User specification word
						( isUserSpecificationWord &&

							( ( !isUserRelationWord &&

								// Test files: "reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
								//				"reasoning\family\Context - Joe has 2 parents - John is a parent",
								//				"reasoning\family\Complex (17)",
								//				among others
								( ( existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&
								existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ||

								// Typical for Spanish
								isSpanishCurrentLanguage_ ) ) ||

							// Test files: "question answering\family\Just a few questions (1)",
							//				"reasoning\family\Complex (18)",
							//				"reasoning\family\Complex (19 - mixed)",
							//				"reasoning\family\Complex (19 - strange)",
							//				among others
							( !isSpecificationWordSpanishAmbiguous &&
							!existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) &&
							existingSpecificationItem.hasNonCompoundSpecificationCollection() ) ) ) ) ) ||

					// Existing specification is user specification
					// Test file: "Negative and alternative answers"
					( isExistingUserSpecification &&
					isQuestion ) ) ) ||

				// Current relation collection
				( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&
				userSpecificationItem_ == null &&

					// Test files: "question answering\family\Just a few questions (1)",
					//				"question answering\family\Positive answers",
					//				"question answering\family\Question relation is subset of answer relation",
					//				"You had same-similar-related question before (with relation)",
					//				"reasoning\family\You have confirmed at least one relation word"
					( !isUserPreviousRelationWord ||
					// Test files: "reasoning\family\Complex (12)",
					//				"reasoning\family\Complex (16)",
					//				"reasoning\family\Complex (20)",
					//				"reasoning\family\I know (11)"
					existingRelationCollectionNr == Constants.NO_COLLECTION_NR ||
					// Test file: "reasoning\family\I know (10)"
					previousRelationCollectionNr != currentRelationCollectionNr ||

					// Test file: "question answering\family\Positive answers"
					( existingRelationWordItem != null &&
					!existingRelationWordItem.isUserRelationWord ) ) ) ) ) ||

			// Forcing new collection
			( isForcingNewCollectionNr &&

				// Non-possessive
				( ( !isPossessive &&

					// Not user specification word
					( ( !isUserSpecificationWord &&

						( ( !hasExistingSpecificationCurrentCreationSentenceNr &&
						!hasRelationWordConfirmedSpecification &&

							// Test files: "reasoning\family\Complex (2)",
							//				"reasoning\family\Complex (13)",
							//				"reasoning\family\Joe has 2 parents - Joe is a child",
							//				"reasoning\family\Joe has 2 parents - Joe is a son",
							//				among others
							( ( hasGeneralizationWordCurrentlyConfirmedSpecification &&
							!isExistingHiddenSpanishSpecification &&
							// Typical for Chinese
							isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) ) ||

							// Test file: "reasoning\family\John - Anna (before family definition)"
							!existingSpecificationItem.hasSpecificationCollection() ) ) ||

						// Typical for Spanish
						( isSpecificationWordSpanishAmbiguous &&
						!hasGeneralizationWordCurrentlyConfirmedSpecification &&
						existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != null &&
						// Question doesn't exist
						generalizationWordItem.firstSpecificationItem( false, false, true, specificationWordItem ) == null ) ) ) ||

					// User specification word
					( isUserSpecificationWord &&
					!isUserPreviousRelationWord &&

						// Test files: "question answering\family\Just a few questions (1)",
						//				"reasoning\family\Complex (7 - Becky)",
						//				"reasoning\family\Complex (7 - Claudia)",
						//				"reasoning\family\Simple family definition - Complex (7 - Claudia)",
						//				among others
						( currentRelationCollectionNr == existingRelationCollectionNr ||

						// Not confirmed relation word
						( !hasRelationWordConfirmedSpecification &&
						// Typical for Spanish
						// Spanish test file: "respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)"
						isExistingHiddenSpanishSpecification &&
						!isUserRelationWord ) ||

						// Confirmed relation word
						( hasRelationWordConfirmedSpecification &&

							// Test files: "reasoning\family\Complex (4)",
							//				"reasoning\family\Complex (5)"
							( !hasExistingSpecificationCurrentCreationSentenceNr ||

							// Test file: "reasoning\family\Paul - Anna - Joe - Labura - Joe - John - Paul"
							( previousRelationCollectionNr != currentRelationCollectionNr &&
							relationWordItem == existingRelationWordItem &&
							isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) ) ) ) ) ) ) ) ||

				// Possessive
				( isPossessive &&
				// Typical for Spanish
				isSpecificationWordSpanishAmbiguous &&
				previousRelationWordItem != relationWordItem &&
				existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != null &&

					// Not confirmed relation word
					// Spanish test files: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)",
					//						"razonamiento\familia\Ana - Juan - Paz, José y Luisa",
					//						"razonamiento\familia\Complejo (19 - extraño)"
					( ( !hasRelationWordConfirmedSpecification &&
					isExistingHiddenSpanishSpecification &&
					generalizationWordItem.firstAssignmentOrSpecificationItem( false, true, Constants.NO_COLLECTION_NR, specificationWordItem, relationWordItem ) == null ) ||

					// Confirmed relation word
					( hasRelationWordConfirmedSpecification &&
					existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&

						( currentRelationCollectionNr != existingRelationCollectionNr ||
						// Spanish test files: "conflicto\familia\José es una madre",
						//						"razonamiento\familia\Complejo (9)",
						//						"razonamiento\familia\Complejo (13)",
						//						"razonamiento\familia\Complejo (14)",
						//						"razonamiento\familia\Complejo (18)",
						//						among others
						isUserAssignment ) ) ) ) ) ) )
				// Only one relation word, no relation collection
				relationCollectionNr = Constants.NO_COLLECTION_NR;
			else
				{
				if( previousRelationWordItem != relationWordItem &&
				!existingSpecificationItem.isExclusiveSpecification() &&
				// Matching (past/present) tense
				existingSpecificationItem.isArchivedAssignment() == isArchivedAssignment &&

					// No current relation collection
					( ( currentRelationCollectionNr == Constants.NO_COLLECTION_NR &&
					relationCollectionNr == Constants.NO_COLLECTION_NR &&

						( !isExistingHiddenSpanishSpecification ||

						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Complejo (5)",
						//						"razonamiento\familia\Complejo (6)",
						//						"razonamiento\familia\Complejo (11)",
						//						"razonamiento\familia\Complejo (19 - mezclado)"
						( hasExistingSpecificationCurrentCreationSentenceNr ||
						// Spanish test file: "Recopilar más tarde - José-Ana - Luisa-Juan Juan-Ana-Paz-José-Luisa"
						hasRelationWordConfirmedSpecification ||
						// Spanish test file: "razonamiento\familia\Complejo (18)"
						!isPossessive ||
						// Spanish test files: "razonamiento\familia\Complejo (14)",
						//						"razonamiento\familia\Luisa - Juan y Ana - José"
						!isUserSpecificationWord ||
						// Spanish test files: "razonamiento\familia\Complejo (13)",
						//						"razonamiento\familia\Luisa - José - Ana",
						//						"razonamiento\familia\Luisa - José - Juan-Ana",
						//						"razonamiento\familia\Luisa - José - Juan",
						//						"razonamiento\familia\Español ambigua - José - Paz - Juan"
						existingSpecificationItem.hasOnlyOneJustificationType( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) ) ) ||

					// Current relation collection
					( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&

						( isForcingNewCollectionNr ||

						( previousRelationCollectionNr != currentRelationCollectionNr &&

							// Test files: "reasoning\family\Complex (13)",
							//				"reasoning\family\Complex (18)",
							//				"reasoning\family\Complex (19 - mixed)",
							//				"reasoning\family\Complex (19 - strange)",
							//				among others
							( isOlderExistingSpecification ||
							// Test file: "reasoning\family\I know (10)"
							relationCollectionNr == currentRelationCollectionNr ) ) ) ) ) )
					{
					if( ( collectionResult = collectRelationWords( isExclusiveSpecification, isForcingNewCollectionNr, relationWordTypeNr, specificationWordTypeNr, previousRelationCollectionNr, existingRelationCollectionNr, previousRelationWordItem, relationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

					// New collection number
					relationCollectionNr = collectionResult.createdCollectionNr;
					}

				if( relationCollectionNr > Constants.NO_COLLECTION_NR &&
				relationCollectionNr != existingRelationCollectionNr )
					{
					// No existing relation collection
					if( ( existingRelationCollectionNr == Constants.NO_COLLECTION_NR &&

						// Test file: "reasoning\family\Paul - Joe - Laura - John and Anna (parents)"
						( isUserSpecificationWord ||

						// Test file (exclude): "reasoning\family\Joe has 2 parents - Anna - Paul"
						( ( !isForcingNewCollectionNr ||
						isPossessive ||
						!isUserRelationWord ||
						existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != null ||
						existingSpecificationItem.isHiddenSpanishSpecification() ) &&

						isValidCollection( isPossessive, relationCollectionNr, generalizationWordItem ) ) ||

						( isExistingHiddenSpanishSpecification &&
						hasExistingSpecificationCurrentCreationSentenceNr ) ) ) ||

					// Existing relation collection
					( existingRelationCollectionNr > Constants.NO_COLLECTION_NR &&
					!isExistingHiddenSpanishSpecification &&
					!hasRelationWordConfirmedSpecification &&
					!isExistingUserSpecification &&

						( isUserRelationWord ||
						( hasGeneralizationWordCurrentlyConfirmedSpecification &&

							( hasExistingSpecificationCurrentCreationSentenceNr ||
							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Complejo (18)"
							isSpecificationWordSpanishAmbiguous ) ) ) ) ||

					( hasExistingSpecificationCurrentCreationSentenceNr &&

						( !isSpecificationWordSpanishAmbiguous ||

						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Sé (3)",
						//						"razonamiento\familia\Sé (11)",
						//						"razonamiento\familia\Luisa - José - Juan y Ana - Paz",
						//						"razonamiento\familia\Luisa - José - Paz - Juan y Ana",
						//						"razonamiento\familia\Mis suposiciones que se confirman (Paz, José y Luisa)",
						//						"Saltarse las preguntas no válidos (1)"
						( userSpecificationItem_ != null &&
						!userSpecificationItem_.isSpecificationWordSpanishAmbiguous() &&
						!isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) ) ) ) )
						{
						if( currentRelationCollectionNr > Constants.NO_COLLECTION_NR &&

							( existingRelationCollectionNr == Constants.NO_COLLECTION_NR ||

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Complejo (18)"
							( isSpecificationWordSpanishAmbiguous &&
							!hasExistingSpecificationCurrentCreationSentenceNr ) ) )
								// Clear stored sentence buffer, because of changing relation collection
								existingSpecificationItem.clearStoredSentenceStringBuffer();

						// Add relation collection number to found user specification
						if( generalizationWordItem.changeSpecification( existingSpecificationItem.isAnsweredQuestion(), existingSpecificationItem.isEveryGeneralization(), existingSpecificationItem.generalizationCollectionNr(), existingSpecificationItem.specificationCollectionNr(), relationCollectionNr, existingSpecificationItem.firstJustificationItem(), existingSpecificationItem ) != Constants.RESULT_OK )
							return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to add a relation collection number to the existing specification, having relation word \"" + relationWordItem.anyWordTypeString() + "\"" );
						}
					}
				}
			}

		collectionResult.createdCollectionNr = relationCollectionNr;
		return collectionResult;
		}

	private ContextResultType getRelationContext( boolean isArchivedAssignment, boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem, ReadItem startRelationReadItem )
		{
		ReadItem relationReadItem = null;
		SpecificationItem foundSpecificationItem;
		WordItem foundRelationWordItem;
		WordItem relationWordItem;
		ContextResultType contextResult = new ContextResultType();

		if( generalizationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The given start relation read item is undefined" );

		if( ( relationReadItem = startRelationReadItem.firstRelationWordReadItem() ) == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "The read word of the first relation word is undefined" );

		if( ( relationWordItem = relationReadItem.readWordItem() ) == null )
			return adminItem_.startContextResultError( 1, moduleNameString_, "I couldn't find any relation word" );

		if( ( foundSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, null ) ) != null &&
		( foundRelationWordItem = foundSpecificationItem.relationWordItem() ) != null &&
		// Equal number (or more) relation words
		foundSpecificationItem.nRelationWords() >= GlobalVariables.nUserRelationWords )
			{
			if( foundSpecificationItem.hasRelationCollection() )
				{
				if( foundSpecificationItem.isExclusiveSpecification() )
					// Already existing static (exclusive) semantic ambiguity
					contextResult.isAmbiguousRelationContext = true;
				}
			else
				{
				if( foundRelationWordItem != relationWordItem &&
				!foundSpecificationItem.isActiveAssignment() &&
				!foundRelationWordItem.hasCollection() )
					{
					// Static (exclusive) semantic ambiguity
					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) != Constants.RESULT_OK )
						return adminItem_.addContextResultError( 1, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				}
			}

		return contextResult;
		}

	private CreateAndAssignResultType addSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecific, boolean isSpanishCurrentLanguage, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int relationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nEnteredRelationWords, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		boolean hasRelationWord = ( relationWordItem != null );
		boolean isAmbiguousRelationContext = false;
		boolean isAssignedOrClear;
		boolean isDefinitionSpecification;
		boolean isExistingOlderSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelfGenerated = ( firstJustificationItem != null );
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUserGeneralizationWord;
		int assignmentSpecificationCollectionNr;
		JustificationItem firstHiddenJustificationItem;
		SpecificationItem createdAssignmentItem = null;
		SpecificationItem createdOrFoundAssignmentItem;
		SpecificationItem createdSpecificationItem = null;
		SpecificationItem existingSpecificationItem;
		SpecificationItem foundAssignmentItem;
		SpecificationItem foundSpecificationItem;
		SpecificationItem hiddenSpanishSpecificationItem;
		SpecificationItem replacedAssignmentItem;
		SpecificationItem tempSpecificationItem;
		WordItem previousRelationWordItem;
		StringBuffer writtenSentenceStringBuffer;
		CollectionResultType collectionResult;
		CreateAndAssignResultType createAndAssignCheckResult;
		CreateAndAssignResultType createAndAssignReturnResult = new CreateAndAssignResultType();

		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given specification word is the administrator" );

		isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

		if( isSpanishCurrentLanguage &&
		specificationWordItem != null )
			isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

		if( ( existingSpecificationItem = generalizationWordItem.firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, false, specificationWordItem ) ) != null )
			{
			// Typical for Spanish
			if( isSpanishCurrentLanguage &&
			hasRelationWord &&
			!isAssignment &&
			!isQuestion &&
			relationWordItem != null &&
			userSpecificationItem_ != null )
				{
				if( existingSpecificationItem.hasRelationWord() )
					{
					if( isSpecificationWordSpanishAmbiguous &&
					existingSpecificationItem.hasCompoundSpecificationCollection() &&
					( tempSpecificationItem = generalizationWordItem.firstNonCompoundCollectionSpecificationItem( isNegative, isPossessive, existingSpecificationItem.specificationCollectionNr() ) ) != null &&
					tempSpecificationItem.relationWordItem() != relationWordItem &&
					
						( tempSpecificationItem.isOlderItem() ||
						relationWordItem.hasCurrentlyConfirmedSpecification() ||
						userSpecificationItem_.relationWordItem() == generalizationWordItem ) )
						existingSpecificationItem = tempSpecificationItem;
					}
				else
					{
					// Correct Spanish existing specification
					// Spanish test file: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)"
					if( isSpecificationWordSpanishAmbiguous &&
					( tempSpecificationItem = generalizationWordItem.firstSelfGeneratedCheckSpecificationItem( false, true, isNegative, isPossessive, true, specificationWordItem, null ) ) != null )
						existingSpecificationItem = tempSpecificationItem;
					}
				}

			isExistingOlderSpecification = existingSpecificationItem.isOlderItem();

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
			if( !isQuestion &&
			existingSpecificationItem != null &&
			existingSpecificationItem.isExclusiveGeneralization() &&
			existingSpecificationItem.hasRelationWord() &&
			existingSpecificationItem.relationWordItem() != relationWordItem &&
			// Inactive assignment already exists with same relation word
			generalizationWordItem.bestMatchingRelationSpecificationItem( false, false, false, true, true, false, isPossessive, false, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != null &&
			// Dynamic semantic ambiguity
			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem.anyWordTypeString(), Constants.INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != Constants.RESULT_OK )
				return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

			if( relationCollectionNr == Constants.NO_COLLECTION_NR &&
			existingSpecificationItem != null &&

				// Test file: "reasoning\Scientific challenge - Confirmation"
				( !isPossessive ||
				!existingSpecificationItem.isUserAssignment() ) &&

			( previousRelationWordItem = existingSpecificationItem.relationCollectionWordItem() ) != null &&

				( ( !isSpecificationWordSpanishAmbiguous &&

					// Test file: "reasoning\family\John - Peter - Anna"
					( !isAssignment ||
					isPossessive ||
					isQuestion ||
					userSpecificationItem_ != null ||
					previousRelationWordItem == relationWordItem ||
					!existingSpecificationItem.isSelfGeneratedSpecification() ||
					existingSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, previousRelationWordItem ) != null ) ) ||

				// Typical for Spanish
				( isSpecificationWordSpanishAmbiguous &&

					( userSpecificationItem_ == null ||
					// Matching (past/present) tense
					existingSpecificationItem.isArchivedAssignment() == userSpecificationItem_.isArchivedAssignment() ) ) ) )
				{
				// Collect relation words
				if( ( collectionResult = collectRelation( isAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isQuestion, isSpecificationWordSpanishAmbiguous, specificationWordTypeNr, relationWordTypeNr, relationCollectionNr, existingSpecificationItem, generalizationWordItem, specificationWordItem, previousRelationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to collect previous relation word \"" + previousRelationWordItem.anyWordTypeString() + "\" to relation word \"" + relationWordItem.anyWordTypeString() + "\"" );

				relationCollectionNr = collectionResult.createdCollectionNr;
				}

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&
			isSelfGenerated &&
			!isUserGeneralizationWord &&
			userSpecificationItem_ != null &&
			userSpecificationItem_.hasRelationWord() &&
			specificationWordItem.isUserSpecificationWord &&
			( hiddenSpanishSpecificationItem = generalizationWordItem.firstUnhiddenSpanishSpecificationItem() ) != null &&
			( firstHiddenJustificationItem = hiddenSpanishSpecificationItem.firstJustificationItem() ) != null &&
			firstHiddenJustificationItem.isReversibleAssumptionOrConclusion() &&
			!firstHiddenJustificationItem.isOlderItem() )
				{
				if( firstHiddenJustificationItem.isPrimaryActiveAssignmentSpecification() &&
				// Write Spanish assumption that is not hidden anymore
				generalizationWordItem.writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				// Add reversible justifications after Spanish specification not hidden anymore
				if( adminItem_.addReversibleJustificationsAfterSpanishSpecificationNotHiddenAnymore( hiddenSpanishSpecificationItem, specificationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to reversible justifications after Spanish specification that is not hidden anymore in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}

		// Check for an assignment or a specification with opposite negative indicator
		if( !isAssignment &&
		isNegative &&
		isPossessive &&
		isArchivedAssignment_ )
			isAssignment = true;

		if( adminItem_.isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			{
			if( ( createAndAssignReturnResult = adminItem_.addSpecificationWithAuthorization( isAssignment, isCharacteristicFor, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
				return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification to word \"" + generalizationWordItem.anyWordTypeString() + "\" with authorization" );
			}
		else
			{
			if( ( createAndAssignReturnResult = generalizationWordItem.addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, null ) ).result != Constants.RESULT_OK )
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
										Item.isNounWordType( generalizationWordTypeNr ) &&
										Item.isNounWordType( specificationWordTypeNr ) );

			// Not created
			if( ( createdSpecificationItem = createAndAssignReturnResult.createdSpecificationItem ) == null )
				{
				if( hasRelationWord &&
				!isSelfGenerated &&
				foundSpecificationItem != null )
					{
					userSpecificationItem_ = foundSpecificationItem;

					if( !isQuestion &&
					foundSpecificationItem.hasRelationWord() )
						simpleUserSpecificationItem_ = generalizationWordItem.firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem );
					}
				}
			else	// Created
				{
				if( !isSelfGenerated )
					{
					userSpecificationItem_ = createdSpecificationItem;

					if( !isQuestion &&
					createdSpecificationItem.hasRelationWord() )
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
							// Make only-option-left assumption
							adminItem_.makeOnlyOptionLeftAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, createdSpecificationItem, generalizationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make an only-option-left assumption" );
							}
						else
							{
							if( !isPossessive )
								{
								if( isAssignment &&
								hasRelationWord &&

									( !createdSpecificationItem.hasSpecificationCollection() ||
									// Typical for Spanish
									// Spanish test files: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)",
									//						"razonamiento\familia\Ana - Juan - Paz, José y Luisa"
									isSpecificationWordSpanishAmbiguous ) &&

								// Collect generalization words of a proper noun specification
								collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationCollectionNr, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

								// Test file: "Indirectly answered questions - John is masculine"
								if( userSpecificationItem_.isAdjectiveSpecification() &&
								userSpecificationItem_.hasNonCompoundSpecificationCollection() &&
								// Make indirectly answered question assumption
								adminItem_.makeIndirectlyAnsweredQuestionAssumption( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, createdSpecificationItem, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
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

							// Draw possessive definition conclusion from a specification-generalization 'part of' sentence
							addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isExclusiveGeneralization, false, isNegative, false, true, false, isCharacteristicFor, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
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
									( ( foundAssignmentItem = generalizationWordItem.firstAssignmentItem( isPossessive, isQuestion, specificationWordItem ) ) != null ||
									isAssignment ) )
									{
									// Test files: "conflict\Assignment in conflict with definition specification",
									//				"conflict\Definition specification is already assigned"
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
							else	// Created specification
								{
								if( isEveryGeneralization &&
								!isAssignment &&
								isPossessive &&
								// Draw specification-generalization 'part of' conclusion from a possessive definition sentence
								// See Block 2 of the included challenge document, or https://www.mafait.org/block_2
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isSpecific, isExclusiveGeneralization, false, isNegative, !isSpecific, false, false, false, true, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
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
			Item.isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				isAssignedOrClear = ( specificationWordItem != null &&
									specificationWordItem.isAdjectiveAssignedOrEmpty() );

				// Typical for Spanish
				if( isSpanishCurrentLanguage &&
				isAssignment &&
				existingSpecificationItem != null &&
				existingSpecificationItem.hasCompoundSpecificationCollection() )
					{
					if( relationCollectionNr == Constants.NO_COLLECTION_NR )
						{
						// Spanish test files: "razonamiento\familia\Complejo (13)",
						//						"razonamiento\familia\Complejo (17)"
						if( isSpecificationWordSpanishAmbiguous &&
						existingSpecificationItem.hasRelationCollection() &&
						generalizationWordItem.hasCurrentlyConfirmedSpecification() )
							relationCollectionNr = existingSpecificationItem.relationCollectionNr();
						}
					else
						{
						// Spanish test files: "razonamiento\familia\Complejo (7 - Becky)",
						//						"razonamiento\familia\Complejo (7 - Bonita)",
						//						"razonamiento\familia\Definición sencilla de la familia - Complejo (7 - Bonita)"
						if( isExistingOlderSpecification &&
						isSelfGenerated )
							relationCollectionNr = Constants.NO_COLLECTION_NR;
						}
					}

				if( ( createAndAssignCheckResult = assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to assign the specification" );

				if( ( createdAssignmentItem = createAndAssignCheckResult.createdSpecificationItem ) != null )
					{
					// Test files: "ambiguity\US presidents",
					//				"reasoning\Scientific challenge - Confirmation",
					//				"reasoning\family\Complex (7 - Becky)",
					//				"reasoning\family\Complex (7 - Claudia)",
					//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
					if( !isArchivedAssignment_ ||
					// Past tense
					createAndAssignReturnResult.createdSpecificationItem != null )
						{
						createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;
						createAndAssignReturnResult.createdOrFoundSpecificationItem = createdAssignmentItem;
						}

					if( isNegative &&
					createdAssignmentItem.isExclusiveSpecification() &&
					// Draw only option left noun conclusion
					adminItem_.drawOnlyOptionLeftNounConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem.specificationCollectionNr(), generalizationContextNr, generalizationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw an only-option-left noun conclusion by negative assignment" );
					}

				if( !isSelfGenerated &&
				( createdOrFoundAssignmentItem = ( createdAssignmentItem == null ?
														createAndAssignCheckResult.foundSpecificationItem :
														createdAssignmentItem ) ) != null )
					{
					userSpecificationItem_ = createdOrFoundAssignmentItem;

					if( !isQuestion &&
					specificationWordItem != null )
						{
						if( createdOrFoundAssignmentItem.hasRelationWord() )
							simpleUserSpecificationItem_ = ( isArchivedAssignment_ ? createdOrFoundAssignmentItem :
																					generalizationWordItem.firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem ) );

						// Find answers to questions
						if( generalizationWordItem.findAnswersToQuestions( specificationWordItem.compoundCollectionNr(), createdOrFoundAssignmentItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to find answers to questions" );
						}
					}
				}

			if( !isExclusiveSpecification &&
			!isPossessive &&
			!isSpecificationGeneralization &&
			specificationWordItem != null &&
			userSpecificationItem_ != null )
				{
				if( hasRelationWord &&
				isSelfGenerated &&

					( ( !specificationWordItem.isUserSpecificationWord &&

						( foundSpecificationItem == null ||
						foundSpecificationItem.isOlderItem() ||

						( foundSpecificationItem.hasNonCompoundSpecificationCollection() &&
						!foundSpecificationItem.hasOnlyOneRelationWord() ) ) ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Spanish test file: "razonamiento\familia\Complejo (17)"
						foundSpecificationItem == null ) ) ) &&

				// Draw reversible conclusions
				adminItem_.drawReversibleConclusions( ( isArchivedAssignment || isChineseCurrentLanguage_ ), isChineseCurrentLanguage_, true, isSpanishCurrentLanguage, userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
					return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

				if( isQuestion )
					{
					if( hasRelationWord &&
					createdAssignmentItem != null )
						{
						assignmentSpecificationCollectionNr = createdAssignmentItem.specificationCollectionNr();
									
						// Check if specification doesn't exist yet
						if( generalizationWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, assignmentSpecificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) == null &&
						// Make suggestive question assumption
						adminItem_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, assignmentSpecificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, createdAssignmentItem, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make a suggestive question assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
						}
					}
				else
					{
					if( isDefinitionSpecification &&

						// Test file: "reasoning\Collect afterwards - Past tense"
						( isArchivedAssignment ||

						( !isAssignment &&

							// Test file: "reasoning\Knowledge of organisms (2)"
							( isEveryGeneralization ||

							// Test files: "reasoning\Collect afterwards - Every father is a parent and every mother is a parent",
							//				"reasoning\Collect afterwards - Uncertainty",
							//				"reasoning\Specification-generalization - Mammals (plural)",
							//				"reasoning\Specification-generalization - Mammals (singular)"
							!isSelfGenerated ) ) ) &&

					foundSpecificationItem == null &&
					!userSpecificationItem_.hasSpecificationCollection() &&
					!userSpecificationItem_.isExclusiveSpecification() &&
					// Draw specification-generalization conclusion
					adminItem_.drawSpecificationGeneralizationConclusion( isArchivedAssignment, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a specification-generalization conclusion" );
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

		if( assignSpecification( false, assignmentSelectionItem.isAssignedOrEmpty(), assignmentSelectionItem.isInactiveAssignment(), assignmentSelectionItem.isArchivedAssignment(), assignmentSelectionItem.isNegative(), assignmentSelectionItem.isPossessive(), assignmentSelectionItem.isSpecificationGeneralization(), assignmentSelectionItem.isUniqueUserRelation(), assignmentSelectionItem.assumptionLevel(), assignmentSelectionItem.prepositionParameter(), Constants.NO_QUESTION_PARAMETER, assignmentSelectionItem.relationWordTypeNr(), assignmentSelectionItem.relationCollectionNr(), assignmentSelectionItem.generalizationContextNr(), assignmentSelectionItem.specificationContextNr(), assignmentSelectionItem.relationContextNr(), assignmentSelectionItem.nEnteredRelationWords(), null, assignmentSelectionItem.generalizationWordItem(), assignmentSelectionItem.specificationWordItem(), assignmentSelectionItem.relationWordItem(), assignmentSelectionItem.specificationString() ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign a specification" );

		return Constants.RESULT_OK;
		}

	protected CreateAndAssignResultType assignSpecification( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short userAssumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int relationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nEnteredRelationWords, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( adminItem_.isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			return adminItem_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		return generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, null );
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
		isSpanishCurrentLanguage_ = false;

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
		boolean hasNonCompoundUserSpecificationCollection = false;
		boolean hasRelationWord = ( startRelationReadItem != null );
		boolean isAmbiguousRelationContext = false;
		boolean isChineseCurrentLanguage = WordItem.isChineseCurrentLanguage();
		boolean isChineseReasoning = false;
		boolean isCurrentNounValue;
		boolean isDrawingPossessiveReversibleConclusions;
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
		boolean isSpanishCurrentLanguage = WordItem.isSpanishCurrentLanguage();
		boolean isSpecificationWord;
		boolean isSpecificationWordAlreadyAssignedByComparison = false;
		boolean isSystemStartingUp = adminItem_.isSystemStartingUp();
		boolean isText;
		boolean isValueSpecificationWord = false;
		boolean isWaitingForRelation = false;
		boolean isWaitingForText = false;
		short currentSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short generalizationWordTypeNr;
		short linkedSpecificationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short valueGeneralizationWordTypeNr = Constants.NO_WORD_TYPE_NR;
		short wordOrderNr;
		int nEnteredRelationWords = 0;
		int nUserRelationWords = GlobalVariables.nUserRelationWords;
		int userSpecificationCollectionNr = Constants.NO_COLLECTION_NR;
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
		isSpanishCurrentLanguage_ = isSpanishCurrentLanguage;

		isGeneralizationReasoningWordType = Item.isGeneralizationReasoningWordType( generalizationWordTypeNr );

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

			if( !isQuestion )
				{
				if( ( contextResult = getRelationContext( isArchivedAssignment, isPossessive, generalizationWordItem, linkedSpecificationWordItem, startRelationReadItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to get a Chinese relation context" );

				isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
				userRelationContextNr_ = contextResult.contextNr;
				}

			// Remember variables below
			nPreviousChineseUserRelationWords_ = nUserRelationWords;
			previousChineseStartRelationReadItem_ = startRelationReadItem;
			previousChineseEndRelationReadItem_ = endRelationReadItem;

			// Add Chinese user specification with relation
			if( addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, nEnteredRelationWords, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, linkedSpecificationWordItem ) != Constants.RESULT_OK )
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
						nEnteredRelationWords = Integer.parseInt( readWordTypeString );
					else
						// Typical for Chinese
						nEnteredRelationWords = AdminItem.convertChineseNumbers( readWordTypeString.charAt( 0 ) );
					}
				else
					{
					if( !isQuestion &&
					!isSelection &&
					isSpecificationWord &&
					!hasAlreadyAddedChineseRelationWords &&
					startRelationReadItem != null &&
					!generalizationWordItem.isImperativeVerbDisplayLoginOrRead() )
						{
						if( ( contextResult = getRelationContext( isArchivedAssignment, isPossessive, generalizationWordItem, currentSpecificationWordItem, startRelationReadItem ) ).result != Constants.RESULT_OK )
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

								if( adminItem_.createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nEnteredRelationWords, valueGeneralizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
									return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a value selection item" );

								// Value, but no relation
								if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nEnteredRelationWords, null, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != Constants.RESULT_OK )
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
										if( addUserSpecificationWithRelation( isAction, currentReadItem.isAdjectiveAssignedOrEmpty(), isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, selectionLevel, selectionListNr, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, nEnteredRelationWords, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else	// No relation word
										{
										// Selection, no value, no relation
										if( adminItem_.createSelectionPart( isAction, currentReadItem.isAdjectiveAssignedOrEmpty(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nEnteredRelationWords, generalizationWordItem, currentSpecificationWordItem, null, specificationString ) != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to create a selection part" );
										}
									}
								else	// No selection
									{
									if( previousSpecificationWordItem != null &&
									currentSpecificationWordItem != null &&

										// Existing specification is user specification
										( ( existingSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, currentSpecificationWordItem ) ) == null ||
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
											addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, isChineseReasoning, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, Constants.NO_SELECTION_LEVEL, Constants.NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, nEnteredRelationWords, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to add a user specification with a relation to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
											}
										else	// No relation word
											{
											isNegativeSpecification = ( isConditional ? false : isNegative );

											// No selection, no value, no relation
											if( ( createAndAssignResult = addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegativeSpecification, isPartOf, isPossessive, isSelection, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, Constants.NO_COLLECTION_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, nEnteredRelationWords, null, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ) ).result != Constants.RESULT_OK )
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
													else	// Non-exclusive
														{
														if( !isAssignment &&
														!isPossessive &&
														!isQuestion )
															{
															// Proper noun specification
															if( isGeneralizationProperNoun )
																{
																// Avoid idle call
																if( currentSpecificationWordItem.firstNegativeSpecificationItem() != null &&
																// Draw simple negative conclusions
																adminItem_.drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, createdSpecificationItem, userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
																	return adminItem_.addError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																}
															else	// Definition specification
																{
																if( currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE &&
																// Feminine or masculine specification word
																currentSpecificationWordItem.isFeminineOrMasculineWord() &&
																( adjectiveSpecificationWordItem = WordItem.predefinedWordItem( currentSpecificationWordItem.isFeminineWord() ? Constants.WORD_PARAMETER_ADJECTIVE_MASCULINE :
																																												Constants.WORD_PARAMETER_ADJECTIVE_FEMININE ) ) != null )
																	{
																	// For now, limit the negative specification adjective words
																	if( GlobalVariables.nUserSpecificationWords > 1 )
																		return adminItem_.addError( 1, moduleNameString_, "Multiple negative specification adjective words is not supported yet" );

																	// Draw negative user definition conclusions
																	if( adminItem_.drawNegativeUserDefinitionConclusions( isArchivedAssignment, createdSpecificationItem, generalizationWordItem, adjectiveSpecificationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
																		return adminItem_.addError( 1, moduleNameString_, "I failed to draw negative user definition conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
																	}
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
											( Item.isNounWordType( currentSpecificationWordTypeNr ) ||

											// Adjective specification word
											( isEveryGeneralization &&
											currentSpecificationWordTypeNr == Constants.WORD_TYPE_ADJECTIVE ) ) )
											{
											isSpanishAmbiguousSpecificationWord = currentSpecificationWordItem.isNounWordSpanishAmbiguous();

											if( userSpecificationItem_ != null )
												{
												hasNonCompoundUserSpecificationCollection = userSpecificationItem_.hasNonCompoundSpecificationCollection();
												userSpecificationCollectionNr = userSpecificationItem_.specificationCollectionNr();
												}

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
														if( userSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&

															// Proper noun specification
															( ( isGeneralizationProperNoun &&
															hasNonCompoundUserSpecificationCollection &&
															!isExclusiveSpecification ) ||

															// Definition specification
															( !isGeneralizationProperNoun &&
															!hasNonCompoundUserSpecificationCollection ) ||

															// Typical for Spanish
															isSpanishCurrentLanguage ) &&

														!userSpecificationItem_.isOlderItem() &&
														// Avoid idle call
														GlobalVariables.firstUserDefinedProperNounWordItem != null &&
														// Draw proper noun part-of conclusions
														adminItem_.drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, isChineseCurrentLanguage, isSpanishCurrentLanguage, ( isGeneralizationProperNoun ? currentSpecificationWordItem : generalizationWordItem ), null ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

														if( !isSpecificationGeneralization &&
														generalizationWordItem != currentSpecificationWordItem &&
														userSpecificationItem_.hasSpecificationCollection() &&

															// Plural noun unknown
															// Test file: "reasoning\Knowledge of organisms (1)"
															( generalizationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||

															( isEveryGeneralization &&

																( !isExclusiveSpecification ||
																userSpecificationItem_.hasCompoundSpecificationCollection() ) ) ) &&

														// Make generalization assumption
														adminItem_.makeGeneralizationAssumption( isArchivedAssignment, isChineseCurrentLanguage, generalizationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to make a generalization assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
														}

													// Existing specification is user specification
													if( ( existingSpecificationItem = generalizationWordItem.firstUserSpecificationItem( false, false, Constants.NO_COLLECTION_NR, currentSpecificationWordItem ) ) != null &&

														( existingSpecificationItem.isExclusiveSpecification() ||

														( userSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
														!existingSpecificationItem.isAssignment() ) ) &&

													// Avoid idle call
													currentSpecificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() != null &&
													// Draw compound specification substitution conclusion
													adminItem_.drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, existingSpecificationItem, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
														return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
													}

												// Definition specification
												// Test file: "Knowledge of organisms (2)"
												if( ( ( !isGeneralizationProperNoun &&
												isEveryGeneralization &&
												!isExclusiveSpecification &&
												userSpecificationCollectionNr == Constants.NO_COLLECTION_NR &&
												userSpecificationItem_ != null &&
												!userSpecificationItem_.hasGeneralizationCollection() ) ||

												// Proper noun specification
												( isGeneralizationProperNoun &&

													( ( !hasRelationWord &&

														( !isAssignment ||

														( userSpecificationItem_ != null &&
														!userSpecificationItem_.hasRelationWord() ) ) ) ||

													( !isArchivedAssignment &&
													userSpecificationItem_ != null &&
													userSpecificationItem_.isAssignment() &&
													!userSpecificationItem_.isOlderItem() ) ||

													// Typical for Spanish
													isSpanishCurrentLanguage ) ) ) &&

												// Avoid idle call
												currentSpecificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() != null &&
												// Draw specification substitution conclusion or ask a question
												adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, Constants.NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem, null ) != Constants.RESULT_OK )
													return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( userSpecificationItem_ != null )
													{
													// Proper noun specification
													if( isGeneralizationProperNoun )
														{
														// Typical for Spanish
														// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
														if( isSpanishCurrentLanguage &&
														hasNonCompoundUserSpecificationCollection &&
														!isExclusiveSpecification &&
														!isSpanishAmbiguousSpecificationWord &&
														simpleUserSpecificationItem_ != null &&
														( collectionWordItem = currentSpecificationWordItem.collectionWordItem( userSpecificationCollectionNr ) ) != null &&
														collectionWordItem.isNounWordSpanishAmbiguous() &&
														( additionalDefinitionSpecificationItem = collectionWordItem.firstSpecificationItem( false, false, false, collectionWordItem ) ) != null &&
														// Draw simple negative Spanish conclusions
														adminItem_.askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem_, additionalDefinitionSpecificationItem, simpleUserSpecificationItem_, simpleUserSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != Constants.RESULT_OK )
															return adminItem_.addError( 1, moduleNameString_, "I failed to simple negative Spanish conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\" with specification word \"" + currentSpecificationWordItem.anyWordTypeString() + "\"" );
														}
													else	// Definition specification
														{
														// Avoid idle call
														if( generalizationWordItem.firstSpecificationGeneralizationItem( true ) != null )
															{
															// Draw definition specification substitution conclusion
															if( ( compoundResult = adminItem_.drawDefinitionSpecificationSubstitutionConclusion( isChineseCurrentLanguage, isSpanishCurrentLanguage, generalizationWordTypeNr, userSpecificationItem_, generalizationWordItem ) ).result != Constants.RESULT_OK )
																return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to draw a definition specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

															compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
															}
														}
													}
												}

											// Avoid idle call
											if( !isSystemStartingUp &&
											isGeneralizationProperNoun &&
											!isUncountableGeneralizationNoun &&
											userSpecificationItem_ != null )
												{
												// No relation word
												if( ( ( startRelationReadItem == null &&
												// Typical for Spanish
												isSpanishCurrentLanguage &&
												!userSpecificationItem_.isOlderItem() ) ||

												// Relation word
												( startRelationReadItem != null &&

													( !userSpecificationItem_.isOlderItem() ||
													// Typical for Chinese
													// Chinese test file: "Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura"
													isChineseCurrentLanguage ) ) ) &&

												// Draw reversible conclusions
												adminItem_.drawReversibleConclusions( isAssignment, isChineseCurrentLanguage, true, isSpanishCurrentLanguage, userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
													return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

												if( startRelationReadItem != null &&
												userSpecificationCollectionNr > Constants.NO_COLLECTION_NR &&
												// Including relation word of assignment
												GlobalVariables.nUserRelationWords == 1 &&

													// Not older
													( ( !userSpecificationItem_.isOlderItem() &&

														( ( isPossessive &&
														userSpecificationItem_.hasCompoundSpecificationCollection() ) ||

														// Typical for Spanish
														isSpanishAmbiguousSpecificationWord ) ) ||

													// Typical for Chinese
													// Chinese test file: "Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura"
													( isChineseCurrentLanguage &&
													userSpecificationItem_.isOlderItem() &&
													userSpecificationItem_.hasUpdatedInformation() ) ) &&

												( firstRelationReadItem = startRelationReadItem.firstRelationWordReadItem() ) != null &&
												( relationWordItem = firstRelationReadItem.readWordItem() ) != null &&
												// Make an exclusive specification substitution assumption
												adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isChineseCurrentLanguage, isExclusiveSpecification, false, !isPossessive, isSpanishCurrentLanguage, false, firstRelationReadItem.wordTypeNr(), currentSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, currentSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
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
							nEnteredRelationWords = 0;
						}

					if( !isQuestion &&
					currentSpecificationWordItem != null &&
					linkedSpecificationWordItem != null &&
					userSpecificationItem_ != null &&

						( ( !hasRelationWord &&
						isSpecificationWord ) ||

						( isRelationWord &&
						linkedSpecificationWordItem != currentSpecificationWordItem ) ) )
						{
						relationWordItem = ( hasRelationWord ? currentSpecificationWordItem : null );

						if( !isEveryGeneralization &&
						isGeneralizationReasoningWordType &&
						!isNegative &&
						!isPreposition &&
						!isValueSpecificationWord &&
						linkedSpecificationWordTypeNr != Constants.WORD_TYPE_ADJECTIVE &&

							// Definition specification
							// Test files: "reasoning\Boiling point",
							//				"reasoning\Condensation point"
							( ( !isGeneralizationProperNoun &&
							isPossessive &&

								( userSpecificationItem_.isUncountableGeneralizationNoun() ||
								// Typical for Spanish and French
								hasRelationWord ) ) ||

							// Proper noun specification
							( isGeneralizationProperNoun &&

								( ( ( relationWordItem != null ||
								!userSpecificationItem_.isOlderItem() ) &&

								userSpecificationItem_.hasSpecificationCollection() ) ||

								// Typical for Chinese
								// Test files: "reasoning\Boiling point",
								//				"reasoning\Condensation point"
								( isChineseCurrentLanguage &&
								isPossessive &&
								userSpecificationItem_.isUncountableGeneralizationNoun() ) ||

								// Typical for Spanish
								isSpanishCurrentLanguage ) ) ) &&

						// Avoid idle call
						generalizationWordItem.bestMatchingRelationSpecificationItem( true, true, false, isPossessive, linkedSpecificationWordItem, relationWordItem ) != null &&
						// Make an exclusive specification substitution assumption
						adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isChineseCurrentLanguage, isExclusiveSpecification, isNegative, isPossessive, isSpanishCurrentLanguage, isUncountableGeneralizationNoun, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, linkedSpecificationWordItem, relationWordItem ) != Constants.RESULT_OK )
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
							hasRelationWord &&
							!isExclusiveSpecification &&
							userSpecificationItem_.hasUpdatedInformation() &&
							// Possessive user specification doesn't exist
							currentSpecificationWordItem.firstUserSpecificationItem( false, true, Constants.NO_COLLECTION_NR, linkedSpecificationWordItem ) == null )
								{
								isDrawingPossessiveReversibleConclusions = ( ( isAssignment &&
																			userSpecificationItem_.isOlderItem() ) ||

																			// Typical for Chinese
																			isChineseCurrentLanguage );

								if( ( isDrawingPossessiveReversibleConclusions ||

								// Non-assignment
								( !isAssignment &&
								isPossessive &&
								userSpecificationItem_.hasRelationCollection() ) ||

								// Typical for Spanish
								// Spanish test file: "razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
								( isSpanishCurrentLanguage &&
								isAssignment &&
								!isPossessive ) ) &&

								// Draw reversible conclusions
								adminItem_.drawReversibleConclusions( isAssignment, isChineseCurrentLanguage, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to draw reversible conclusions about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

								if( userSpecificationItem_.hasSpecificationCollection() &&
								// Make an exclusive specification substitution assumption
								adminItem_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isChineseCurrentLanguage, false, false, !isPossessive, isSpanishCurrentLanguage, false, currentSpecificationWordTypeNr, linkedSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, currentSpecificationWordItem, linkedSpecificationWordItem, generalizationWordItem ) != Constants.RESULT_OK )
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
				if( generalizationWordItem.recalculateAssumptionLevelsInWord( false ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( selectionListNr, 1, moduleNameString_, "I failed to recalculate the assumption levels of specifications in word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int relationCollectionNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
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
			( foundSpecificationItem = currentGeneralizationWordItem.firstAssignmentOrSpecificationItem( false, isPossessive, relationCollectionNr, specificationWordItem, relationWordItem ) ) != null )
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
				if( currentTouchedWordItem.recalculateAssumptionLevelsInWord( true ) != Constants.RESULT_OK )
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

	protected CollectionResultType collectRelationWords( boolean isExclusiveSpecification, boolean isForcingNewCollectionNr, short relationWordTypeNr, short commonWordTypeNr, int previousRelationCollectionNr, int relationCollectionNr, WordItem previousRelationWordItem, WordItem currentRelationWordItem, WordItem commonWordItem )
		{
		boolean isCheckingForSameCollectionNumber;
		CollectionResultType collectionResult;
		CollectionResultType returnCollectionResult = new CollectionResultType();

		if( previousRelationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given previous relation word item is undefined" );

		if( currentRelationWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given current relation word item is undefined" );

		if( commonWordItem == null )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given common word item is undefined" );

		if( previousRelationWordItem == currentRelationWordItem )
			return adminItem_.startCollectionResultError( 1, moduleNameString_, "The given previous and current relation words are the same word" );

		if( !isForcingNewCollectionNr &&
		relationCollectionNr == Constants.NO_COLLECTION_NR )
			{
			// Get previous relation collection if not defined yet
			if( previousRelationCollectionNr == Constants.NO_COLLECTION_NR )
				previousRelationCollectionNr = previousRelationWordItem.collectionNr( commonWordItem );

			// Try to find existing relation collection number
			relationCollectionNr = ( previousRelationCollectionNr == Constants.NO_COLLECTION_NR ? currentRelationWordItem.collectionNr( commonWordItem ) : previousRelationCollectionNr );
			}

		isCheckingForSameCollectionNumber = ( !isForcingNewCollectionNr &&
											!commonWordItem.isNounWordSpanishAmbiguous() );

		// Each collection comes in pairs
		if( ( collectionResult = previousRelationWordItem.createCollectionItem( isCheckingForSameCollectionNumber, isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, relationCollectionNr, currentRelationWordItem, commonWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to collect previous word \"" + previousRelationWordItem.anyWordTypeString() + "\" with current word \"" + currentRelationWordItem.anyWordTypeString() + "\"" );

		if( relationCollectionNr == Constants.NO_COLLECTION_NR )
			relationCollectionNr = collectionResult.createdCollectionNr;

		if( currentRelationWordItem.createCollectionItem( false, isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, relationCollectionNr, previousRelationWordItem, commonWordItem, null ).result != Constants.RESULT_OK )
			return adminItem_.addCollectionResultError( 1, moduleNameString_, "I failed to collect current word \"" + currentRelationWordItem.anyWordTypeString() + "\" with previous word \"" + previousRelationWordItem.anyWordTypeString() + "\"" );

		returnCollectionResult.createdCollectionNr = relationCollectionNr;
		return returnCollectionResult;
		}

	protected CreateAndAssignResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNounEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, short assumptionLevel, short justificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem additionalProperNounSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasDisplayedIntegrityWarning = false;
		boolean hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		boolean hasRelationWordConfirmedSpecification = false;
		boolean isEveryGeneralization;
		boolean isGeneralizationProperNoun = ( generalizationWordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );
		boolean isOlderFoundSpecification = false;
		boolean isQuestion = ( questionParameter > Constants.NO_QUESTION_PARAMETER );
		boolean isSelectedUniqueUserRelation;
		boolean isSpanishCurrentLanguage = isSpanishCurrentLanguage_;
		boolean isSpecificationWordSpanishAmbiguous = false;
		boolean isUserGeneralizationWord;
		short calculatedAssumptionLevel;
		JustificationItem createdJustificationItem = null;
		JustificationItem createdOrFoundJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem foundJustificationItem;
		SpecificationItem createdSpecificationItem = null;
		SpecificationItem foundSpecificationItem = null;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;

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
			if( ( calculatedAssumptionLevel = Item.justificationAssumptionLevel( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem ) ) > Constants.MAX_LEVEL )
				return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "Assumption level overflow" );

			if( calculatedAssumptionLevel > Constants.NO_ASSUMPTION_LEVEL )
				assumptionLevel = calculatedAssumptionLevel;
			}

		hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem.hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
		isEveryGeneralization = ( !isGeneralizationProperNoun &&
								!isAssignment &&
								!isUncountableGeneralizationNoun );
		isSelectedUniqueUserRelation = ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION );

		if( ( createAndAssignResult = addSpecification( isAssignment, false, isArchivedAssignment, isCharacteristicFor, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isSelectedUniqueUserRelation, false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification" );

		// No conflict found
		if( !GlobalVariables.hasDisplayedWarning )
			{
			isUserGeneralizationWord = generalizationWordItem.isUserGeneralizationWord;

			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem.isNounWordSpanishAmbiguous();

			if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != null )
				isOlderFoundSpecification = foundSpecificationItem.isOlderItem();

			if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == null )
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

					// A justification has been created or found, but the self-generated specification already exists
					// So, the justification needs to be added or replaced separately

					// Justification created
					if( createdJustificationItem != null &&
					// Justification doesn't exist yet in specification
					!foundSpecificationItem.hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&

						( isUserGeneralizationWord ||
						assumptionLevel == Constants.NO_ASSUMPTION_LEVEL ||
						relationWordItem == null ||
						foundSpecificationItem.isSelfGeneratedAssumption() ||
						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Complejo (12)",
						//						"razonamiento\familia\Complejo (13)",
						//						"razonamiento\familia\Complejo (15)",
						//						among others
						isSpecificationWordSpanishAmbiguous ) )
						{
						// Process justification
						if( processJustification( hasRelationWordConfirmedSpecification, isOlderFoundSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, firstJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to process justifications of the found specification" );

						foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

						if( isOlderFoundSpecification &&
						!isPossessive &&
						!isQuestion &&
						relationWordItem == null &&
						foundSpecificationItem.isSelfGeneratedAssumption() &&

							// Definition specification
							( ( !isGeneralizationProperNoun &&
							primarySpecificationItem != null &&
							!primarySpecificationItem.isOlderItem() &&
							primarySpecificationItem.isAdjectiveSpecification() ) ||

							// Proper noun specification
							( isGeneralizationProperNoun &&

								// Test file: "reasoning\family\Correcting invalidated assumption (by knowledge)"
								( justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ||
								justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION ) ) ) &&

						// Recalculate assumption level of found assumption
						foundSpecificationItem.recalculateAssumptionLevel( false ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to recalculate the assumption level of the assumption" );
						}
					}
				}

			if( !hasDisplayedIntegrityWarning &&
			!isNegative &&
			!isPartOf &&

			( createdJustificationItem != null ||
			createdSpecificationItem != null ||

			// Typical for Spanish
			( isSpanishCurrentLanguage &&
			foundSpecificationItem != null &&
			foundSpecificationItem.isHiddenSpanishSpecification() ) ) &&

			Item.isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					createdSpecificationItem != null &&
					// Draw unique relation conclusion
					addSelfGeneratedSpecification( false, false, false, false, isExclusiveGeneralization, isExclusiveSpecification, true, false, true, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, Constants.NO_CONTEXT_NR, createdSpecificationItem, null, null, null, generalizationWordItem, specificationWordItem, null ).result != Constants.RESULT_OK )
						return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a self-generated unique specification to word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
					}
				else
					{
					if( isGeneralizationProperNoun )
						{
						if( !isQuestion &&
						!isAssignment &&
						!isExclusiveSpecification &&
						relationWordItem == null &&

							( !isSpanishCurrentLanguage ||
							// Typical for Spanish
							createdSpecificationItem == null ||
							!createdSpecificationItem.isHiddenSpanishSpecification() ) &&

						// Avoid idle call
						specificationWordItem.firstNegativeSpecificationItem() != null &&
						// Draw simple negative conclusions
						adminItem_.drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, ( createdSpecificationItem == null ? foundSpecificationItem : createdSpecificationItem ), userSpecificationItem_, generalizationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						// Non-exclusive specification
						if( ( ( !isExclusiveSpecification &&

							// Not created
							( ( createdSpecificationItem == null &&
							foundSpecificationItem != null &&

								// Not older
								( ( !foundSpecificationItem.isOlderItem() &&
								foundSpecificationItem.hasOnlyOneRelationWord() &&
								foundSpecificationItem.hasNonCompoundSpecificationCollection() &&

									( ( isUserGeneralizationWord &&
									userSpecificationItem_.isPossessive() ) ||

									( !isUserGeneralizationWord &&
									!foundSpecificationItem.hasJustificationWithAdditionalDefinitionSpecification() ) ||

									userSpecificationItem_.isActiveAssignment() ) ) ||

								// Older
								( foundSpecificationItem.isOlderItem() &&

									( ( foundSpecificationItem.hasRelationCollection() &&

										// Test file: "reasoning\family\John is the father - Paul - Laura"
										( isUserGeneralizationWord ||
										// Test files: "reasoning\family\Complex (4)",
										//				"reasoning\family\Complex (5)",
										//				"reasoning\family\Complex (6)"
										userSpecificationItem_.isActiveAssignment() ||
										// Test file: "reasoning\family\Paul - Joe - John and Anna"
										userSpecificationItem_.isPossessive() ||
										// Test file: "reasoning\family\Complex (13)"
										foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == null ) ) ||

									// Typical for Spanish
									( isSpanishCurrentLanguage &&
									foundSpecificationItem.hasCompoundSpecificationCollection() ) ) ) ||

								// Typical for Spanish
								( isSpecificationWordSpanishAmbiguous &&
								!isAssignment ) ) ) ||

							// Created
							( createdSpecificationItem != null &&

								// Test files: "reasoning\family\Anna was the mother of Paul, Joe and Laura",
								//				"reasoning\family\John was the father of Paul, Joe and Laura",
								//				"reasoning\family\Past tense"
								( isArchivedAssignment ||

								// Test files: "conflict\family\Negative conflict - Pete is not a father",
								//				"conflict\family\No conflict in same sentence",
								//				"conflict\family\No conflict on multiple related specifications",
								//				among others
								!createdSpecificationItem.hasRelationWord() ||

								// Test files: "reasoning\family\Complex (2)",
								//				"reasoning\family\Complex (3)",
								//				"reasoning\family\Complex (4)",
								//				"reasoning\family\Complex (5)",
								//				"reasoning\family\Complex (6)",
								//				among others
								( !isUserGeneralizationWord &&
								specificationWordItem == userSpecificationItem_.specificationWordItem() ) ||

								// Typical for Spanish
								isSpanishCurrentLanguage ) &&

							( createdSpecificationItem.hasNonCompoundSpecificationCollection() ||

							// Typical for Spanish
							( isSpanishCurrentLanguage &&
							!isAssignment &&
							createdSpecificationItem.hasCompoundSpecificationCollection() ) ) ) ) ) ||

						// Exclusive specification
						// Typical for Spanish
						( isExclusiveSpecification &&
						isSpecificationWordSpanishAmbiguous ) ) &&

						// Draw proper noun part-of conclusions
						adminItem_.drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, isChineseCurrentLanguage_, isSpanishCurrentLanguage, specificationWordItem, ( isSpecificationWordSpanishAmbiguous ? relationWordItem : null ) ) != Constants.RESULT_OK )
							return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
						}

					if( createdSpecificationItem != null )
						{
						// Proper noun specification
						if( isGeneralizationProperNoun )
							{
							// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
							if( hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&
							// Draw compound specification substitution conclusion
							adminItem_.drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						else	// Definition specification
							{
							// Test file: "reasoning\Knowledge of organisms (1)"
							if( !isSpecificationGeneralization &&
							justificationTypeNr == Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION &&
							primarySpecificationItem != null &&
							primarySpecificationItem.isUserSpecification() &&
							createdSpecificationItem.hasSpecificationCollection() &&
							// Make generalization assumption
							adminItem_.makeGeneralizationAssumption( isArchivedAssignment, isChineseCurrentLanguage_, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
								return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to make a generalization assumption about word \"" + generalizationWordItem.anyWordTypeString() + "\"" );
							}
						}

					if( userSpecificationItem_ != null &&

						// Created specification
						( ( createdSpecificationItem != null &&

							// Not user generalization word
							( ( !isUserGeneralizationWord &&

								// Additional exclusive specification substitution conclusion
								( justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
								justificationTypeNr == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ||

								// Test files: "reasoning\Basic reasoning",
								//				"reasoning\family\Justification report",
								//				"reasoning\family\Simple family definition - Justification report"
								( isExclusiveSpecification &&
								!isQuestion &&
								!userSpecificationItem_.isExclusiveSpecification() ) ||

								// Test file: "reasoning\family\James was the father of Peter (before family definition)"
								( isArchivedAssignment &&
								!isSpecificationGeneralization ) ||

								// Typical for Spanish
								( isSpanishCurrentLanguage &&
								isGeneralizationProperNoun &&
								!isExclusiveSpecification &&

									// Spanish test files: "conflicto\familia\José es una madre",
									//						"conflicto\familia\Paz es una hija",
									//						"razonamiento\familia\Complejo (4).txt",
									//						"razonamiento\familia\Complejo (9).txt",
									//						among others
									( isSpecificationWordSpanishAmbiguous ||
									// Spanish test files: "conflicto\familia\José es una madre",
									//						"conflicto\familia\Paz es una hija",
									//						"respuestas a las preguntas\familia\Suposición a través pregunta sugestiva",
									//						among others
									justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) ) ) ) ||

							// User generalization word
							( isUserGeneralizationWord &&

								// Not found
								( ( foundSpecificationItem == null &&
								isGeneralizationProperNoun &&

									( !isExclusiveSpecification ||
									// Test files: "reasoning\family\Complex (10)",
									//				"reasoning\family\John has 2 sons and a daughter",
									//				"reasoning\family\John has a daughter and 2 sons",
									//				"reasoning\family\Person",
									//				among others
									isQuestion ) ) ||

								// Found
								// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
								( foundSpecificationItem != null &&
								hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ) ) ) ) ) ||

						// Found specification
						( foundSpecificationItem != null &&
						!isExclusiveSpecification &&
						isGeneralizationProperNoun &&
						!foundSpecificationItem.hasCompoundSpecificationCollection() &&

							// Not older
							( ( !isOlderFoundSpecification &&
							!isUserGeneralizationWord &&
							foundSpecificationItem.hasSpecificationCollection() &&

								( justificationTypeNr != Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ||
								// Test files: "reasoning\family\Complex (10)",
								//				"reasoning\family\My conclusions that are confirmed (without relation)",
								//				"reasoning\family\John has a daughter and 2 sons",
								//				"reasoning\family\This information is more specific"
								foundSpecificationItem.firstJustificationItem( Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) == null ||

								// Typical for Chinese
								// Chinese test file: "This information is more specific"
								( isChineseCurrentLanguage_ &&
								userSpecificationItem_.isAssignment() ) ) ) ||

							// Older
							( isOlderFoundSpecification &&

								// Test files: "conflict\family\Joe is a mother",
								//				"question answering\family\Intelligent answering of questions",
								//				"reasoning\family\Avoid duplicate context",
								//				among others
								( ( foundSpecificationItem.hasCurrentCreationSentenceNr() &&
								userSpecificationItem_.hasRelationWord() ) ||

								// Spanish test files: "razonamiento\familia\Ana - Juan - Paz, José y Luisa",
								//						"razonamiento\familia\Complejo (8)",
								//						"razonamiento\familia\Complejo (16)",
								//						among others
								( isUserGeneralizationWord &&
								userSpecificationItem_.isSpecificationWordSpanishAmbiguous() ) ) ) ) ) ) &&

					// Avoid idle call
					specificationWordItem.firstNonNegativeNonPosessiveDefinitionSpecificationItem() != null &&
					// Draw specification substitution conclusion or ask a question
					adminItem_.drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
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
