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

#include "AdminItem.h"
#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"
#include "SelectionItem.cpp"

class AdminSpecification
	{
	friend class AdminItem;

	// Private constructed variables

	bool isArchivedAssignment_ = false;
	bool isChineseCurrentLanguage_ = false;
	bool isSpanishCurrentLanguage_ = false;

	unsigned short doneSpecificationWordOrderNr_ = NO_ORDER_NR;
	unsigned short linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

	unsigned int nPreviousChineseUserRelationWords_ = 0;
	unsigned int userRelationContextNr_ = NO_CONTEXT_NR;

	ReadItem *previousChineseStartRelationReadItem_ = NULL;
	ReadItem *previousChineseEndRelationReadItem_ = NULL;

	SpecificationItem *simpleUserSpecificationItem_ = NULL;
	SpecificationItem *userSpecificationItem_ = NULL;

	WordItem *linkedGeneralizationWordItem_ = NULL;

//Java_private_final
	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminSpecification";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;


	// Private functions

	bool isAuthorizationRequiredForChanges( WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		return ( generalizationWordItem != NULL &&
				specificationWordItem != NULL &&
				adminItem_->isSystemStartingUp() &&

				( generalizationWordItem->isAuthorizationRequiredForChanges() ||
				specificationWordItem->isAuthorizationRequiredForChanges() ) );
		}

	bool isValidCollection( bool isPossessive, unsigned int relationCollectionNr, WordItem *generalizationWordItem )
		{
		WordItem *currentProperNounWordItem;

		if( relationCollectionNr == NO_COLLECTION_NR ||
		generalizationWordItem == NULL )
			return false;

		if( ( currentProperNounWordItem = globalVariables_->firstProperNounWordItem ) != NULL )
			{
			// Do for all specification words
			do	{
				if( currentProperNounWordItem != generalizationWordItem &&
				!currentProperNounWordItem->isUserGeneralizationWord &&
				currentProperNounWordItem->firstCollectionSpecificationItem( isPossessive, relationCollectionNr ) != NULL )
					return false;
				}
			while( ( currentProperNounWordItem = currentProperNounWordItem->nextProperNounWordItem ) != NULL );
			}

		return true;
		}

	unsigned int collectionNrByCollectionWord( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		unsigned int collectionNr;
		WordItem *currentCollectionWordItem;

		if( compoundGeneralizationWordItem != NULL &&
		( currentCollectionWordItem = globalVariables_->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( currentCollectionWordItem != compoundGeneralizationWordItem &&
				( collectionNr = currentCollectionWordItem->collectionNrByCompoundGeneralizationWordInWord( collectionWordTypeNr, compoundGeneralizationWordItem ) ) > NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	unsigned int collectionNrByExclusiveSpecification( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		unsigned int collectionNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;

		if( compoundGeneralizationWordItem != NULL &&
		( currentSpecificationItem = compoundGeneralizationWordItem->firstExclusiveSpecificationItem( false ) ) != NULL )
			{
			// Do for all exclusive specifications of this word (skip adjectives)
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
				( collectionNr = currentSpecificationWordItem->collectionNrByCompoundGeneralizationWordInWord( collectionWordTypeNr, compoundGeneralizationWordItem ) ) > NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextExclusiveSpecificationItem( false ) ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	signed char addUserSpecificationWithRelation( bool isAction, bool isAssignedOrClear, bool isAssignment, bool isCharacteristicFor, bool isChineseReasoning, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpanishCurrentLanguage, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, unsigned short imperativeVerbParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int nEnteredRelationWords, unsigned int nUserRelationWords, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isChineseReversedImperativeNoun;
		bool isFirstComparisonPart = ( selectionListNr == ADMIN_CONDITION_LIST );
		bool isFirstRun = true;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		unsigned short relationWordTypeNr;
		unsigned int specificationCollectionNr;
		unsigned int relationCollectionNr = NO_COLLECTION_NR;
		ReadItem *currentRelationReadItem = startRelationReadItem;
		SpecificationItem *chineseUserSpecificationItem;
		SpecificationItem *definitionSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *olderUserSpecificationItem;
		WordItem *mostRecentContextWord;
		WordItem *previousRelationWordItem = NULL;
		WordItem *relationWordItem;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addUserSpecificationWithRelation";

		if( startRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start relation read item is undefined" );

		if( endRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given end relation read item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		do	{
			if( currentRelationReadItem->isPreposition() )
				prepositionParameter = currentRelationReadItem->wordParameter();
			else
				{
				if( currentRelationReadItem->isRelationWord() )
					{
					if( ( relationWordItem = currentRelationReadItem->readWordItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined read word" );

					relationWordItem->addMyWordToTouchedDuringCurrentSentenceList();

					// Typical for Chinese: Relation before specification
					if( specificationWordItem != relationWordItem )
						{
						if( generalizationWordItem == relationWordItem )
							{
							if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_WARNING_WORD_IS_GENERALIZATION_AS_WELL_AS_RELATION_END ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
							}
						else
							{
							relationWordTypeNr = currentRelationReadItem->wordTypeNr();
							specificationCollectionNr = NO_COLLECTION_NR;

							if( nUserRelationWords == 1 &&
							( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationWordItem ) ) != NULL &&
							existingSpecificationItem->hasRelationWord() &&
							( mostRecentContextWord = existingSpecificationItem->mostRecentRelationWord() ) != NULL &&
							mostRecentContextWord != relationWordItem )
								{
								previousRelationWordItem = mostRecentContextWord;

								if( existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
								!existingSpecificationItem->isSpecificationWordSpanishAmbiguous() )
									specificationCollectionNr = existingSpecificationItem->specificationCollectionNr();
								}

							if( !isQuestion &&
							previousRelationWordItem != NULL &&

								// Avoid looping
								( relationWordItem->collectionNr( specificationWordItem ) == NO_COLLECTION_NR ||
								previousRelationWordItem->collectionNr( specificationWordItem ) == NO_COLLECTION_NR ) &&

							// Test file: "Paul has a parent, called John - Paul has a parent, called Anna"
							( ( olderUserSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, previousRelationWordItem ) ) == NULL ||
							!olderUserSpecificationItem->isOlderItem() ) )
								{
								// Collect relation words
								if( ( collectionResult = collectRelationWords( isExclusiveSpecification, false, relationWordTypeNr, specificationWordTypeNr, NO_COLLECTION_NR, NO_COLLECTION_NR, previousRelationWordItem, relationWordItem, specificationWordItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );

								if( ( relationCollectionNr = collectionResult.createdCollectionNr ) > NO_COLLECTION_NR &&
								( existingSpecificationItem = generalizationWordItem->bestMatchingRelationSpecificationItem( true, true, isNegative, isPossessive, specificationWordItem, previousRelationWordItem ) ) != NULL &&
								// Add relation collection number to found user specification
								generalizationWordItem->changeSpecification( existingSpecificationItem->isAnsweredQuestion(), existingSpecificationItem->isEveryGeneralization(), existingSpecificationItem->generalizationCollectionNr(), existingSpecificationItem->specificationCollectionNr(), relationCollectionNr, existingSpecificationItem->firstJustificationItem(), existingSpecificationItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a relation collection number to the existing user specification, having relation word \"", relationWordItem->anyWordTypeString(), "\"" );
								}

							if( selectionListNr == NO_LIST_NR )
								{
								if( !isChineseReasoning )
									{
									if( relationCollectionNr == NO_COLLECTION_NR &&
									isAssignment &&
									userSpecificationItem_ != NULL &&
									userSpecificationItem_->hasCompoundSpecificationCollection() &&
									!userSpecificationItem_->isOlderItem() &&
									!userSpecificationItem_->isSpecificationWordSpanishAmbiguous() )
										// Proper noun user assignment
										relationCollectionNr = userSpecificationItem_->relationCollectionNr();

									if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nEnteredRelationWords, NULL, generalizationWordItem, specificationWordItem, relationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification having relation word \"", relationWordItem->anyWordTypeString(), "\"" );

									if( !isQuestion &&
									!isSpanishCurrentLanguage &&
									( definitionSpecificationItem = specificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() ) != NULL &&

										// Conflicting sentence
										( userSpecificationItem_ == NULL ||

										// Specification already exists
										( ( existingSpecificationItem = generalizationWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&

											// Not first run
											( ( !isFirstRun &&
											!isAssignment &&
											userSpecificationItem_->hasNonCompoundSpecificationCollection() &&
											!userSpecificationItem_->isOlderItem() ) ||

											// First run
											( isFirstRun &&

												( ( ( !isAssignment ||
												// Typical for Chinese
												isChineseCurrentLanguage_ ) &&

												!definitionSpecificationItem->isAdjectiveSpecification() ) ||

												( ( !isAssignment ||
												isArchivedAssignment ||
												// Test file: "Paul is the son of John and Anna"
												definitionSpecificationItem->isExclusiveSpecification() ) &&

												!existingSpecificationItem->hasRelationCollection() ) ) ) ) ) ) &&

									// Draw specification substitution conclusion or ask a question
									adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									isFirstRun = false;
									}
								else	// Typical for Chinese
									{
									if( userSpecificationItem_->hasCompoundSpecificationCollection() )
										{
										// Make Chinese exclusive specification substitution assumption (non-possessive)
										if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, true, isExclusiveSpecification, isNegative, isPossessive, false, isUncountableGeneralizationNoun, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make a Chinese exclusive specification substitution assumption with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

										if( ( chineseUserSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, NO_COLLECTION_NR, specificationWordItem, NULL ) ) != NULL &&
										!chineseUserSpecificationItem->isOlderItem() &&
										// Make Chinese exclusive specification substitution assumption (possessive)
										adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, true, isExclusiveSpecification, false, !isPossessive, false, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make a Chinese exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								}
							else
								{
								// Typical for Chinese
								isChineseReversedImperativeNoun = ( isChineseCurrentLanguage_ &&
																	relationWordItem->isChineseReversedImperativeNoun() );

								if( adminItem_->createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem->isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, ( isChineseReversedImperativeNoun ? relationWordTypeNr : specificationWordTypeNr ), ( isChineseReversedImperativeNoun ? specificationWordTypeNr : relationWordTypeNr ), relationCollectionNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nEnteredRelationWords, generalizationWordItem, ( isChineseReversedImperativeNoun ? relationWordItem : specificationWordItem ), ( isChineseReversedImperativeNoun ? specificationWordItem : relationWordItem ), NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a selection part having relation word \"", relationWordItem->anyWordTypeString(), "\"" );

								isFirstComparisonPart = false;
								}

							previousRelationWordItem = relationWordItem;
							}
						}
					}
				}
			}
		while( currentRelationReadItem != endRelationReadItem &&
		( currentRelationReadItem = currentRelationReadItem->nextReadItem() ) != NULL &&
		// No conflict found
		!globalVariables_->hasDisplayedWarning );

		return RESULT_OK;
		}

	signed char collectGeneralizationWords( bool isExclusiveGeneralization, unsigned short generalizationWordTypeNr, unsigned short commonWordTypeNr, WordItem *previousGeneralizationWordItem, WordItem *currentGeneralizationWordItem, WordItem *newCommonWordItem )
		{
		bool hasFoundCollection = false;
		unsigned int generalizationCollectionNr = NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectGeneralizationWords";

		if( previousGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous generalization word item is undefined" );

		if( currentGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current generalization word item is undefined" );

		if( previousGeneralizationWordItem == currentGeneralizationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous and current generalization word items are the same word" );

		if( newCommonWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given new common word item is undefined" );

		if( ( generalizationCollectionNr = previousGeneralizationWordItem->collectionNr( newCommonWordItem ) ) == NO_COLLECTION_NR )
			generalizationCollectionNr = currentGeneralizationWordItem->collectionNr( newCommonWordItem );

		if( generalizationCollectionNr > NO_COLLECTION_NR )
			{
			if( ( boolResult = currentGeneralizationWordItem->findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" is collected with word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			// Each collection comes in pairs
			if( ( collectionResult = previousGeneralizationWordItem->createCollectionItem( true, isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, currentGeneralizationWordItem, newCommonWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" with current word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( generalizationCollectionNr == NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			// Collect current word
			if( currentGeneralizationWordItem->createCollectionItem( false, isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect current word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with previous word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			// Collect generalizations in previous word
			if( previousGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in previous word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			// Collect generalizations in current word
			if( currentGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in current word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		bool hasFoundCollection = false;
		bool isAllowingDifferentCommonWord;
		unsigned int specificationCollectionNr = NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectSpecificationWords";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( previousSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous specification word item is undefined" );

		if( currentSpecificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current specification word item is undefined" );

		if( previousSpecificationWordItem == currentSpecificationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous and current specification words are the same word" );

		if( compoundGeneralizationWordItem == NULL )
			{
			if( ( specificationCollectionNr = generalizationWordItem->userSpecificationCollectionNr() ) == NO_COLLECTION_NR &&

			( specificationWordTypeNr != WORD_TYPE_NOUN_PLURAL ||
			generalizationWordTypeNr != WORD_TYPE_NOUN_SINGULAR ) )
				specificationCollectionNr = currentSpecificationWordItem->collectionNr();
			}
		else
			{
			if( isExclusiveSpecification &&

				( generalizationWordItem == previousSpecificationWordItem ||
				!generalizationWordItem->isNounWordSpanishAmbiguous() ) )
				specificationCollectionNr = ( generalizationWordItem->isFeminineOrMasculineWord() ? collectionNrByExclusiveSpecification( specificationWordTypeNr, compoundGeneralizationWordItem ) :
																									collectionNrByCollectionWord( specificationWordTypeNr, compoundGeneralizationWordItem ) );
			}

		if( !isPossessive &&
		specificationCollectionNr > NO_COLLECTION_NR )
			{
			isAllowingDifferentCommonWord = ( previousSpecificationWordItem != generalizationWordItem );

			if( ( boolResult = previousSpecificationWordItem->findCollection( isAllowingDifferentCommonWord, currentSpecificationWordItem, generalizationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if word \"", currentSpecificationWordItem->anyWordTypeString(), "\" is collected with word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			// Each collection comes in pairs
			if( ( collectionResult = previousSpecificationWordItem->createCollectionItem( true, isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous word \"", previousSpecificationWordItem->anyWordTypeString(), "\" with current word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

			if( specificationCollectionNr == NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem->createCollectionItem( false, isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect current word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with previous word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		if( specificationCollectionNr > NO_COLLECTION_NR &&
		generalizationWordItem->collectSpecificationsInWord( isExclusiveSpecification, isQuestion, specificationCollectionNr ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect specifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		return RESULT_OK;
		}

	signed char displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords( bool isAssignment, bool isArchivedAssignment, WordItem *specificationWordItem )
		{
		SpecificationItem *hiddenSpanishSpecificationItem;
		WordItem *currentSpecificationWordItem;
		WordItem *hiddenSpanishSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords";

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) != NULL )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem->isProperNounWord() &&
				( hiddenSpanishSpecificationItem = currentSpecificationWordItem->firstUnhiddenSpanishSpecificationItem() ) != NULL )
					{
					if( !isAssignment )
						{
						currentSpecificationWordItem->hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = true;

						// Write Spanish assumption
						if( currentSpecificationWordItem->writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a Spanish assumption about word \"", currentSpecificationWordItem->anyWordTypeString(), "\" that is not hidden anymore" );
						}

					if( ( hiddenSpanishSpecificationWordItem = hiddenSpanishSpecificationItem->specificationWordItem() ) != NULL &&
					// Draw Spanish proper noun part-of conclusions
					adminItem_->drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, false, true, hiddenSpanishSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
					}
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char processJustification( bool hasRelationWordConfirmedSpecification, bool isOlderFoundSpecification, bool isUserGeneralizationWord, unsigned short justificationTypeNr, JustificationItem *createdOrFoundJustificationItem, JustificationItem *firstJustificationItem, SpecificationItem *foundSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundSpecificationCurrentCreationSentenceNr;
		bool isRemovingPreviousJustifications = false;
		JustificationItem *obsoleteJustificationItem = NULL;
		JustificationItem *questionJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "processJustification";

		if( createdOrFoundJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given created or found justification item is undefined" );

		if( firstJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given first justification item is undefined" );

		if( foundSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		if( primarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given primary specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( foundSpecificationItem->isReplacedOrDeletedItem() )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		hasFoundSpecificationCurrentCreationSentenceNr = foundSpecificationItem->hasCurrentCreationSentenceNr();

		if( relationWordItem != NULL )
			hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();

		switch( justificationTypeNr )
			{
			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
				if( isUserGeneralizationWord )
					{
					// Typical for Spanish
					if( foundSpecificationItem->isHiddenSpanishSpecification() &&
					!foundSpecificationItem->isOlderItem() )
						obsoleteJustificationItem = foundSpecificationItem->firstReplacedSecondarySpecificationJustificationItem();
					}

				break;

			case JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
				// Typical for Spanish
				if( foundSpecificationItem->isSpecificationWordSpanishAmbiguous() )
					{
					if( !hasRelationWordConfirmedSpecification &&
					primarySpecificationItem->isUserAssignment() )
						obsoleteJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem );
					}
				else
					{
					if( isOlderFoundSpecification &&
					!isUserGeneralizationWord &&

						( !hasFoundSpecificationCurrentCreationSentenceNr ||
						// Typical for Spanish
						isSpanishCurrentLanguage_ ) &&

					foundSpecificationItem->hasCompoundSpecificationCollection() &&
					foundSpecificationItem->hasOnlyOneRelationWord() )
						isRemovingPreviousJustifications = true;
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				if( foundSpecificationItem->hasAssumptionLevel() )
					{
					if( primarySpecificationItem->isCorrectedSpecification() )
						// Test files: "reasoning\family\Correcting invalidated assumption (by knowledge)",
						//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
						obsoleteJustificationItem = foundSpecificationItem->firstReplacedPrimarySpecificationJustificationItem();
					else
						{
						// Typical for Spanish
						if( primarySpecificationItem->isSpecificationWordSpanishAmbiguous() &&
						foundSpecificationItem->assumptionLevel() > createdOrFoundJustificationItem->justificationAssumptionLevel() &&
						firstJustificationItem->hasAdditionalDefinitionSpecification() )
							isRemovingPreviousJustifications = true;
						}
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION:
				if( isOlderFoundSpecification &&
				foundSpecificationItem->hasAssumptionLevel() &&

				// Typical for Spanish
				( hasFoundSpecificationCurrentCreationSentenceNr ||
				primarySpecificationItem->hasRelationWord() ) &&

				firstJustificationItem->isOlderItem() &&

				( firstJustificationItem->isPrimaryQuestionSpecification() ||
				firstJustificationItem->justificationAssumptionLevel() > createdOrFoundJustificationItem->justificationAssumptionLevel() ) )
					isRemovingPreviousJustifications = true;

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				// Adjusted question
				if( isOlderFoundSpecification &&
				secondarySpecificationItem != NULL &&
				!primarySpecificationItem->isPossessive() &&
				primarySpecificationItem->isUserSpecification() &&

				// Typical for Spanish
				( !hasFoundSpecificationCurrentCreationSentenceNr ||
				secondarySpecificationItem->isUserSpecification() ) )
					{
					if( generalizationWordItem->hasFeminineOrMasculineProperNounEnding() )
						{
						if( ( questionJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION ) ) != NULL &&
						( assumptionSpecificationItem = questionJustificationItem->primarySpecificationItem() ) != NULL &&
						assumptionSpecificationItem->assumptionLevel() > primarySpecificationItem->assumptionLevel() )
							// Remove less certain justification from question
							isRemovingPreviousJustifications = true;
						}
					else
						{
						// Remove obsolete justifications from adjusted compound question
						isRemovingPreviousJustifications = true;

						// Clear stored sentence buffer, because of adjusted question
						foundSpecificationItem->clearStoredSentenceString();
						}
					}
			}

		if( isRemovingPreviousJustifications )
			{
			if( hasFoundSpecificationCurrentCreationSentenceNr )
				{
				// Remove all previous justifications from found specification
				if( foundSpecificationItem->changeFirstJustification( false, createdOrFoundJustificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the found specification" );
				}
			else
				{
				// Copy and replace found specification, with different first justification item
				if( generalizationWordItem->changeSpecification( foundSpecificationItem->isAnsweredQuestion(), foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->generalizationCollectionNr(), foundSpecificationItem->specificationCollectionNr(), foundSpecificationItem->relationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy and replace the found specification, with a different first justification item" );

				foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();
				}

			if( justificationTypeNr == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION &&
			// Recalculate assumption level of found specification
			foundSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );
			}
		else
			{
			if( obsoleteJustificationItem == NULL )
				{
				// Attach created justification to the found specification
				if( foundSpecificationItem->attachJustificationToSpecification( createdOrFoundJustificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach a justification to the found specification" );
				}
			else
				{
				if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	CollectionResultType copyCollection( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short commonWordTypeNr, unsigned int currentRelationCollectionNr, WordItem *specificationWordItem, WordItem *previousRelationWordItem )
		{
		bool isForcingNewCollectionNr = true;
		unsigned int previousRelationCollectionNr = NO_COLLECTION_NR;
		unsigned int relationCollectionNr = NO_COLLECTION_NR;
		WordItem *currentCollectionWordItem;
		CollectionResultType collectionResult;
		CollectionResultType returnCollectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyCollection";

		if( specificationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( previousRelationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given previous relation word item is undefined" );

		if( ( currentCollectionWordItem = adminItem_->firstCollectionWordItem( currentRelationCollectionNr ) ) != NULL )
			{
			// Do for all collection words
			do	{
				if( previousRelationWordItem != currentCollectionWordItem )
					{
					// Collect relation words
						if( ( collectionResult = collectRelationWords( isExclusiveSpecification, isForcingNewCollectionNr, relationWordTypeNr, commonWordTypeNr, previousRelationCollectionNr, relationCollectionNr, previousRelationWordItem, currentCollectionWordItem, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", currentCollectionWordItem->anyWordTypeString(), "\"" );

					isForcingNewCollectionNr = false;
					relationCollectionNr = collectionResult.createdCollectionNr;
					previousRelationWordItem = currentCollectionWordItem;
					}
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->_nextCollectionWordItem( currentRelationCollectionNr ) ) != NULL );
			}

		returnCollectionResult.createdCollectionNr = relationCollectionNr;
		return returnCollectionResult;
		}

	CollectionResultType collectRelation( bool isAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isQuestion, bool isSpecificationWordSpanishAmbiguous, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int relationCollectionNr, SpecificationItem *existingSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *previousRelationWordItem, WordItem *relationWordItem )
		{
		bool hasExistingSpecificationCompoundCollection;
		bool hasExistingSpecificationCurrentCreationSentenceNr;
		bool hasExistingSpecificationUpdatedInformation;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasRelationWordConfirmedSpecification;
		bool hasUserSpecificationRelationCollection = false;
		bool isExistingHiddenSpanishSpecification;
		bool isExistingUserSpecification;
		bool isForcingNewCollectionNr = false;
		bool isOlderExistingSpecification;
		bool isPossessiveUserSpecification = false;
		bool isUserAssignment = false;
		bool isUserPreviousRelationWord;
		bool isUserRelationWord;
		bool isUserSpecificationWord;
		unsigned int currentRelationCollectionNr;
		unsigned int existingRelationCollectionNr;
		unsigned int previousRelationCollectionNr;
		WordItem *existingRelationWordItem;
		SpecificationItem *tempSpecificationItem = NULL;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectRelation";

		if( existingSpecificationItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given existing specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( previousRelationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given previous relation word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		hasExistingSpecificationCompoundCollection = existingSpecificationItem->hasCompoundSpecificationCollection();
		hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem->hasCurrentCreationSentenceNr();
		hasExistingSpecificationUpdatedInformation = existingSpecificationItem->hasUpdatedInformation();
		isExistingHiddenSpanishSpecification = existingSpecificationItem->isHiddenSpanishSpecification();
		isExistingUserSpecification = existingSpecificationItem->isUserSpecification();
		isOlderExistingSpecification = existingSpecificationItem->isOlderItem();
		existingRelationCollectionNr = existingSpecificationItem->relationCollectionNr();
		existingRelationWordItem = existingSpecificationItem->relationWordItem();

		if( userSpecificationItem_ != NULL )
			{
			hasUserSpecificationRelationCollection = userSpecificationItem_->hasRelationCollection();
			isUserAssignment = userSpecificationItem_->isAssignment();
			isPossessiveUserSpecification = userSpecificationItem_->isPossessive();
			}

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
		isUserRelationWord = generalizationWordItem->isUserRelationWord;
		isUserSpecificationWord = specificationWordItem->isUserSpecificationWord;

		isUserPreviousRelationWord = previousRelationWordItem->isUserRelationWord;
		previousRelationCollectionNr = previousRelationWordItem->collectionNr( specificationWordItem );

		hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();
		currentRelationCollectionNr = relationWordItem->collectionNr( specificationWordItem );

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
					currentRelationCollectionNr == NO_COLLECTION_NR ) ||

					// User relation word
					( isUserRelationWord &&

						// No current relation collection
						( ( currentRelationCollectionNr == NO_COLLECTION_NR &&

							// No existing relation collection
							( ( existingRelationCollectionNr == NO_COLLECTION_NR &&
							existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) &&

								// Test files: "reasoning\family\Complex (18)",
								//				"reasoning\family\Complex (19 - mixed)",
								//				"reasoning\family\Complex (19 - strange)"
								( !isExistingHiddenSpanishSpecification ||
								// Typical for Spanish
								// Spanish test file: "razonamiento\familia\Complejo (13)"
								previousRelationCollectionNr == NO_COLLECTION_NR ) ) ||

							// Existing relation collection
							// Test files: "reasoning\family\Complex (7 - Becky)",
							//				"reasoning\family\Complex (7 - Claudia)",
							//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
							( existingRelationCollectionNr > NO_COLLECTION_NR &&
							isPossessiveUserSpecification &&
							// Copy collection
							!isValidCollection( isPossessive, existingRelationCollectionNr, generalizationWordItem ) ) ) ) ||

						// Current relation collection
						// Test files: "reasoning\family\Complex (12)",
						//				"reasoning\family\Complex (16)"
						( currentRelationCollectionNr > NO_COLLECTION_NR &&
						hasRelationWordConfirmedSpecification &&
						!isExistingHiddenSpanishSpecification &&
						existingRelationCollectionNr == NO_COLLECTION_NR &&
						existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == NULL ) ) ) ) ) ||

				// Possessive
				( isPossessive &&
				currentRelationCollectionNr == NO_COLLECTION_NR &&
				userSpecificationItem_ != NULL &&
				userSpecificationItem_->hasCompoundSpecificationCollection() &&

					// Not user relation word
					( ( !isUserRelationWord &&
					hasUserSpecificationRelationCollection &&

						( existingRelationCollectionNr == NO_COLLECTION_NR &&

							// Test files: "reasoning\family\Complex (20)",
							//				"reasoning\family\John - Laura - John and Anna",
							//				"reasoning\family\John is a parent - Paul - Laura",
							//				"reasoning\family\John is the father - Paul - Laura"
							( !isUserAssignment ||
							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Luisa - José - Ana"
							previousRelationCollectionNr > NO_COLLECTION_NR ) ) ) ||

					// User relation word
					( isUserRelationWord &&
					// Typical for Spanish
					isSpecificationWordSpanishAmbiguous &&
					// Spanish test files: "razonamiento\familia\Complejo (7 - Becky)",
					//						"razonamiento\familia\Complejo (7 - Bonita)",
					//						"razonamiento\familia\Definición sencilla de la familia - Complejo (7 - Bonita)"
					isPossessiveUserSpecification &&
					!existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) ) ) ||

			// User specification word
			( isUserSpecificationWord &&
			currentRelationCollectionNr == NO_COLLECTION_NR &&

				// Existing specification is not user specification
				// Test file: "Paul has a father, called Anna"
				( ( ( !isExistingUserSpecification &&
				isPossessive &&
				userSpecificationItem_ == NULL ) ||

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
						existingRelationCollectionNr == NO_COLLECTION_NR ) ) ) ) ) ||

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
				( ( existingRelationCollectionNr == NO_COLLECTION_NR &&
				!isUserSpecificationWord &&

					// Non-possessive
					( ( !isPossessive &&
					isOlderExistingSpecification &&
					currentRelationCollectionNr > NO_COLLECTION_NR &&
					!isValidCollection( isPossessive, currentRelationCollectionNr, generalizationWordItem ) &&

						// Not user relation word
						// Test file: "reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
						( ( !isUserRelationWord &&
						( tempSpecificationItem = relationWordItem->firstSpecificationItem( false, true, false, specificationWordItem ) ) != NULL &&
						tempSpecificationItem->assumptionLevel() < existingSpecificationItem->assumptionLevel() ) ||

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
						( ( currentRelationCollectionNr == NO_COLLECTION_NR &&
						existingSpecificationItem->hasSecondaryUserSpecificationJustification() ) ||

						( currentRelationCollectionNr > NO_COLLECTION_NR &&
						// Typical for Spanish
						isSpecificationWordSpanishAmbiguous &&
						// Spanish test file: "razonamiento\familia\Complejo (19 - extraño)"
						!isExistingHiddenSpanishSpecification &&
						!isValidCollection( isPossessive, currentRelationCollectionNr, generalizationWordItem ) ) ) ) ) ) ||

				// Copy collection
				( existingRelationCollectionNr > NO_COLLECTION_NR &&
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
			existingRelationCollectionNr == NO_COLLECTION_NR &&
			previousRelationCollectionNr > NO_COLLECTION_NR &&

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
			if( existingRelationCollectionNr == NO_COLLECTION_NR )
				{
				if( ( collectionResult = collectRelationWords( isExclusiveSpecification, true, relationWordTypeNr, specificationWordTypeNr, previousRelationCollectionNr, relationCollectionNr, previousRelationWordItem, relationWordItem, specificationWordItem ) ).result != RESULT_OK )
					return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );

				// New collection number
				relationCollectionNr = collectionResult.createdCollectionNr;
				}
			else
				{
				// Copy collection
				if( ( collectionResult = copyCollection( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, existingRelationCollectionNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to copy a relation collection" );

				// New collection number
				relationCollectionNr = collectionResult.createdCollectionNr;
				}
			}

		if( ( ( relationCollectionNr == NO_COLLECTION_NR &&

			( previousRelationWordItem != relationWordItem ||

			( !hasExistingSpecificationUpdatedInformation &&
			!isExistingHiddenSpanishSpecification &&
			existingRelationCollectionNr > NO_COLLECTION_NR ) ) ) ||

		( !hasRelationWordConfirmedSpecification &&
		existingRelationCollectionNr > NO_COLLECTION_NR &&

			( previousRelationWordItem != relationWordItem ||
			// Typical for Spanish
			// Spanish test files: "respuestas a las preguntas\familia\Relación pregunta es subconjunto de relación de respuesta",
			//						"respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)"
			!hasExistingSpecificationCurrentCreationSentenceNr ) ) ) &&

		// Test file: "ambiguity\US presidents"
		!existingSpecificationItem->isExclusiveGeneralization() )
			{
			if( currentRelationCollectionNr > NO_COLLECTION_NR &&

				( !hasExistingSpecificationCurrentCreationSentenceNr ||
				!hasUserSpecificationRelationCollection ||
				isPossessiveUserSpecification ||
				existingRelationCollectionNr == NO_COLLECTION_NR ||

				( ( !isUserSpecificationWord ||
				previousRelationCollectionNr == existingRelationCollectionNr ) &&

					( isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) ||
					adminItem_->nRelationWords( previousRelationCollectionNr, previousRelationWordItem ) >= adminItem_->nRelationWords( currentRelationCollectionNr, relationWordItem ) ) ) ) )
				{
				relationCollectionNr = currentRelationCollectionNr;

				if( isUserRelationWord &&
				!isSpecificationWordSpanishAmbiguous &&
				existingRelationCollectionNr > NO_COLLECTION_NR &&
				previousRelationCollectionNr == relationCollectionNr &&
				existingRelationCollectionNr != relationCollectionNr )
					{
					previousRelationCollectionNr = existingRelationCollectionNr;
					relationCollectionNr = existingRelationCollectionNr;
					}
				}

			if( userSpecificationItem_ != NULL &&

				// No current relation collection
				( ( existingRelationCollectionNr == NO_COLLECTION_NR &&
				previousRelationCollectionNr > NO_COLLECTION_NR &&
				!isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) &&

					// Non-possessive
					( ( !isPossessive &&

						( hasExistingSpecificationCurrentCreationSentenceNr ||

						// Not user relation word
						( !isUserRelationWord &&
						// Typical for Spanish
						// Spanish test file: "razonamiento\familia\Luisa - José - Ana"
						existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) ||

						// User relation word
						( isUserRelationWord &&

							( ( isExistingHiddenSpanishSpecification &&

								// Spanish test files: "razonamiento\familia\Complejo (12)",
								//						"razonamiento\familia\Complejo (13)",
								//						"razonamiento\familia\Complejo (16)",
								//						"razonamiento\familia\Complejo (18)"
								( !isUserAssignment ||
								// Spanish test file: "razonamiento\familia\Complejo (19 - mezclado)"
								currentRelationCollectionNr == NO_COLLECTION_NR ) ) ||

							// Test files: "reasoning\family\Complex (4)",
							//				"reasoning\family\Complex (5)",
							//				"reasoning\family\Complex (6)",
							//				"reasoning\family\Paul - Anna - Joe - Laura - John",
							//				among others
							existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != NULL ) ) ) ) ||

					// Possessive
					( isPossessive &&
					isUserRelationWord &&

						( hasExistingSpecificationCurrentCreationSentenceNr ||
						!isUserAssignment ||
						isSpecificationWordSpanishAmbiguous ) ) ) ) ||

				// Current relation collection
				( currentRelationCollectionNr > NO_COLLECTION_NR &&

					// Not user specification word
					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				among others
					( ( !isUserSpecificationWord &&
					!hasExistingSpecificationCurrentCreationSentenceNr &&
					!isPossessive &&
					!isSpecificationWordSpanishAmbiguous &&
					existingRelationCollectionNr == NO_COLLECTION_NR &&

						( isUserAssignment ||
						// Test file: "reasoning\family\Joe has 2 parents - Anna - Paul"
						userSpecificationItem_->hasCompoundSpecificationCollection() ) ) ||

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
							existingRelationCollectionNr > NO_COLLECTION_NR ) ||

							( hasExistingSpecificationCurrentCreationSentenceNr &&
							existingRelationCollectionNr > NO_COLLECTION_NR &&

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

								( ( existingRelationCollectionNr > NO_COLLECTION_NR &&

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
					existingRelationCollectionNr > NO_COLLECTION_NR &&
					// Add relation collection number to found user specification
					generalizationWordItem->changeSpecification( existingSpecificationItem->isAnsweredQuestion(), existingSpecificationItem->isEveryGeneralization(), existingSpecificationItem->generalizationCollectionNr(), existingSpecificationItem->specificationCollectionNr(), relationCollectionNr, existingSpecificationItem->firstJustificationItem(), existingSpecificationItem ) != RESULT_OK )
						return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to add a relation collection number to the existing specification, having relation word \"", relationWordItem->anyWordTypeString(), "\"" );
					}

				// Existing specification is wrong. Find correct specification
				if( ( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedConclusionSpecificationItem( isNegative, isPossessive, isUserAssignment, specificationWordItem ) ) != NULL &&
				existingSpecificationItem != tempSpecificationItem )
					{
					if( hasRelationWordConfirmedSpecification &&
					tempSpecificationItem->relationWordItem() != previousRelationWordItem )
						// Test files: "reasoning\family\I know (3)",
						//				"reasoning\family\I know (10)",
						//				"reasoning\family\Laura - John and Anna - Joe",
						//				"reasoning\family\My assumptions that are confirmed (parents)",
						//				"reasoning\family\My assumptions that are confirmed (John and Anna)",
						//				"reasoning\family\My assumptions that are confirmed (John and Anna - reversed)",
						//				"reasoning\family\My conclusions that are confirmed (Joe has 2 parents)"
						previousRelationWordItem = tempSpecificationItem->relationWordItem();

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
					existingRelationCollectionNr = existingSpecificationItem->relationCollectionNr();
					}
				}

			// Not forcing new collection
			if( ( !isForcingNewCollectionNr &&
			!hasExistingSpecificationCurrentCreationSentenceNr &&
			!isExclusiveSpecification &&

				// No current relation collection
				( ( currentRelationCollectionNr == NO_COLLECTION_NR &&
				!isPossessive &&
				relationCollectionNr == NO_COLLECTION_NR &&

					// Existing specification is not user specification
					( ( !isExistingUserSpecification &&

						// Not user specification word
						// Test files: "reasoning\family\Joe has 2 parents - Joe is a child",
						//				"reasoning\family\Joe has 2 parents - Joe is a son",
						//				among others
						( ( !isUserSpecificationWord &&
						!isPossessiveUserSpecification &&
						!isUserRelationWord &&
						existingRelationCollectionNr == NO_COLLECTION_NR &&
						existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ||

						// User specification word
						( isUserSpecificationWord &&

							( ( !isUserRelationWord &&

								// Test files: "reasoning\family\Collect afterwards - Joe-Anna - Laura-John John-Anna-Paul-Joe-Laura",
								//				"reasoning\family\Context - Joe has 2 parents - John is a parent",
								//				"reasoning\family\Complex (17)",
								//				among others
								( ( existingRelationCollectionNr == NO_COLLECTION_NR &&
								existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ||

								// Typical for Spanish
								isSpanishCurrentLanguage_ ) ) ||

							// Test files: "question answering\family\Just a few questions (1)",
							//				"reasoning\family\Complex (18)",
							//				"reasoning\family\Complex (19 - mixed)",
							//				"reasoning\family\Complex (19 - strange)",
							//				among others
							( !isSpecificationWordSpanishAmbiguous &&
							!existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) &&
							existingSpecificationItem->hasNonCompoundSpecificationCollection() ) ) ) ) ) ||

					// Existing specification is user specification
					// Test file: "Negative and alternative answers"
					( isExistingUserSpecification &&
					isQuestion ) ) ) ||

				// Current relation collection
				( currentRelationCollectionNr > NO_COLLECTION_NR &&
				userSpecificationItem_ == NULL &&

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
					existingRelationCollectionNr == NO_COLLECTION_NR ||
					// Test file: "reasoning\family\I know (10)"
					previousRelationCollectionNr != currentRelationCollectionNr ||

					// Test file: "question answering\family\Positive answers"
					( existingRelationWordItem != NULL &&
					!existingRelationWordItem->isUserRelationWord ) ) ) ) ) ||

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
							!existingSpecificationItem->hasSpecificationCollection() ) ) ||

						// Typical for Spanish
						( isSpecificationWordSpanishAmbiguous &&
						!hasGeneralizationWordCurrentlyConfirmedSpecification &&
						existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != NULL &&
						// Question doesn't exist
						generalizationWordItem->firstSpecificationItem( false, false, true, specificationWordItem ) == NULL ) ) ) ||

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
				existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != NULL &&

					// Not confirmed relation word
					// Spanish test files: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)",
					//						"razonamiento\familia\Ana - Juan - Paz, José y Luisa",
					//						"razonamiento\familia\Complejo (19 - extraño)"
					( ( !hasRelationWordConfirmedSpecification &&
					isExistingHiddenSpanishSpecification &&
					generalizationWordItem->firstAssignmentOrSpecificationItem( false, true, NO_COLLECTION_NR, specificationWordItem, relationWordItem ) == NULL ) ||

					// Confirmed relation word
					( hasRelationWordConfirmedSpecification &&
					existingRelationCollectionNr > NO_COLLECTION_NR &&

						( currentRelationCollectionNr != existingRelationCollectionNr ||
						// Spanish test files: "conflicto\familia\José es una madre",
						//						"razonamiento\familia\Complejo (9)",
						//						"razonamiento\familia\Complejo (13)",
						//						"razonamiento\familia\Complejo (14)",
						//						"razonamiento\familia\Complejo (18)",
						//						among others
						isUserAssignment ) ) ) ) ) ) )
				// Only one relation word, no relation collection
				relationCollectionNr = NO_COLLECTION_NR;
			else
				{
				if( previousRelationWordItem != relationWordItem &&
				!existingSpecificationItem->isExclusiveSpecification() &&
				// Matching (past/present) tense
				existingSpecificationItem->isArchivedAssignment() == isArchivedAssignment &&

					// No current relation collection
					( ( currentRelationCollectionNr == NO_COLLECTION_NR &&
					relationCollectionNr == NO_COLLECTION_NR &&

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
						existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) ) ) ||

					// Current relation collection
					( currentRelationCollectionNr > NO_COLLECTION_NR &&

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
					if( ( collectionResult = collectRelationWords( isExclusiveSpecification, isForcingNewCollectionNr, relationWordTypeNr, specificationWordTypeNr, previousRelationCollectionNr, existingRelationCollectionNr, previousRelationWordItem, relationWordItem, specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );

					// New collection number
					relationCollectionNr = collectionResult.createdCollectionNr;
					}

				if( relationCollectionNr > NO_COLLECTION_NR &&
				relationCollectionNr != existingRelationCollectionNr )
					{
					// No existing relation collection
					if( ( existingRelationCollectionNr == NO_COLLECTION_NR &&

						// Test file: "reasoning\family\Paul - Joe - Laura - John and Anna (parents)"
						( isUserSpecificationWord ||

						// Test file (exclude): "reasoning\family\Joe has 2 parents - Anna - Paul"
						( ( !isForcingNewCollectionNr ||
						isPossessive ||
						!isUserRelationWord ||
						existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != NULL ||
						existingSpecificationItem->isHiddenSpanishSpecification() ) &&

						isValidCollection( isPossessive, relationCollectionNr, generalizationWordItem ) ) ||

						( isExistingHiddenSpanishSpecification &&
						hasExistingSpecificationCurrentCreationSentenceNr ) ) ) ||

					// Existing relation collection
					( existingRelationCollectionNr > NO_COLLECTION_NR &&
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
						( userSpecificationItem_ != NULL &&
						!userSpecificationItem_->isSpecificationWordSpanishAmbiguous() &&
						!isValidCollection( isPossessive, previousRelationCollectionNr, generalizationWordItem ) ) ) ) )
						{
						if( currentRelationCollectionNr > NO_COLLECTION_NR &&

							( existingRelationCollectionNr == NO_COLLECTION_NR ||

							// Typical for Spanish
							// Spanish test file: "razonamiento\familia\Complejo (18)"
							( isSpecificationWordSpanishAmbiguous &&
							!hasExistingSpecificationCurrentCreationSentenceNr ) ) )
								// Clear stored sentence buffer, because of changing relation collection
								existingSpecificationItem->clearStoredSentenceString();

						// Add relation collection number to found user specification
						if( generalizationWordItem->changeSpecification( existingSpecificationItem->isAnsweredQuestion(), existingSpecificationItem->isEveryGeneralization(), existingSpecificationItem->generalizationCollectionNr(), existingSpecificationItem->specificationCollectionNr(), relationCollectionNr, existingSpecificationItem->firstJustificationItem(), existingSpecificationItem ) != RESULT_OK )
							return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to add a relation collection number to the existing specification, having relation word \"", relationWordItem->anyWordTypeString(), "\"" );
						}
					}
				}
			}

		collectionResult.createdCollectionNr = relationCollectionNr;
		return collectionResult;
		}

	ContextResultType getRelationContext( bool isArchivedAssignment, bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationReadItem )
		{
		ReadItem *relationReadItem = NULL;
		SpecificationItem *foundSpecificationItem;
		WordItem *foundRelationWordItem;
		WordItem *relationWordItem;
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getRelationContext";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given start relation read item is undefined" );

		if( ( relationReadItem = startRelationReadItem->firstRelationWordReadItem() ) == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The read word of the first relation word is undefined" );

		if( ( relationWordItem = relationReadItem->readWordItem() ) == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "I couldn't find any relation word" );

		if( ( foundSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, NULL ) ) != NULL &&
		( foundRelationWordItem = foundSpecificationItem->relationWordItem() ) != NULL &&
		// Equal number (or more) relation words
		foundSpecificationItem->nRelationWords() >= globalVariables_->nUserRelationWords )
			{
			if( foundSpecificationItem->hasRelationCollection() )
				{
				if( foundSpecificationItem->isExclusiveSpecification() )
					// Already existing static (exclusive) semantic ambiguity
					contextResult.isAmbiguousRelationContext = true;
				}
			else
				{
				if( foundRelationWordItem != relationWordItem &&
				!foundSpecificationItem->isActiveAssignment() &&
				!foundRelationWordItem->hasCollection() )
					{
					// Static (exclusive) semantic ambiguity
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
						return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				}
			}

		return contextResult;
		}

	CreateAndAssignResultType addSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpanishCurrentLanguage, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isAmbiguousRelationContext = false;
		bool isAssignedOrClear;
		bool isDefinitionSpecification;
		bool isExistingOlderSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserGeneralizationWord;
		unsigned int assignmentSpecificationCollectionNr;
		JustificationItem *firstHiddenJustificationItem;
		SpecificationItem *createdAssignmentItem = NULL;
		SpecificationItem *createdOrFoundAssignmentItem;
		SpecificationItem *createdSpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *hiddenSpanishSpecificationItem;
		SpecificationItem *replacedAssignmentItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *previousRelationWordItem;
		char *writtenSentenceString;
		CollectionResultType collectionResult;
		CreateAndAssignResultType createAndAssignCheckResult;
		CreateAndAssignResultType createAndAssignReturnResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given specification word is the administrator" );

		isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

		if( isSpanishCurrentLanguage &&
		specificationWordItem != NULL )
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		if( ( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, false, specificationWordItem ) ) != NULL )
			{
			// Typical for Spanish
			if( isSpanishCurrentLanguage &&
			hasRelationWord &&
			!isAssignment &&
			!isQuestion &&
			relationWordItem != NULL &&
			userSpecificationItem_ != NULL )
				{
				if( existingSpecificationItem->hasRelationWord() )
					{
					if( isSpecificationWordSpanishAmbiguous &&
					existingSpecificationItem->hasCompoundSpecificationCollection() &&
					( tempSpecificationItem = generalizationWordItem->firstNonCompoundCollectionSpecificationItem( isNegative, isPossessive, existingSpecificationItem->specificationCollectionNr() ) ) != NULL &&
					tempSpecificationItem->relationWordItem() != relationWordItem &&
					
						( tempSpecificationItem->isOlderItem() ||
						relationWordItem->hasCurrentlyConfirmedSpecification() ||
						userSpecificationItem_->relationWordItem() == generalizationWordItem ) )
						existingSpecificationItem = tempSpecificationItem;
					}
				else
					{
					// Correct Spanish existing specification
					// Spanish test file: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)"
					if( isSpecificationWordSpanishAmbiguous &&
					( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, true, isNegative, isPossessive, true, specificationWordItem, NULL ) ) != NULL )
						existingSpecificationItem = tempSpecificationItem;
					}
				}

			isExistingOlderSpecification = existingSpecificationItem->isOlderItem();

			// Typical for Chinese
			if( isChineseCurrentLanguage_ &&
			!isAssignment &&
			isExistingOlderSpecification &&
			isPossessive &&
			relationWordItem != NULL &&
			( tempSpecificationItem = relationWordItem->firstAssignmentOrSpecificationItem( true, true, false, false, false, specificationWordItem ) ) != NULL &&
			tempSpecificationItem->isUserAssignment() &&
			tempSpecificationItem->hasOnlyOneRelationWord() )
				// Force Chinese assignment on confirmation
				isAssignment = true;
			}

		// Get relation context
		// Exceptions: Conditions and imperative verbs have no relation context
		if( !isConditional &&
		generalizationWordTypeNr != WORD_TYPE_VERB_SINGULAR &&
		specificationWordItem != NULL &&
		relationWordItem != NULL )
			{
			if( !isQuestion &&
			existingSpecificationItem != NULL &&
			existingSpecificationItem->isExclusiveGeneralization() &&
			existingSpecificationItem->hasRelationWord() &&
			existingSpecificationItem->relationWordItem() != relationWordItem &&
			// Inactive assignment already exists with same relation word
			generalizationWordItem->bestMatchingRelationSpecificationItem( false, false, false, true, true, false, isPossessive, false, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != NULL &&
			// Dynamic semantic ambiguity
			inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
				return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

			if( relationCollectionNr == NO_COLLECTION_NR &&
			existingSpecificationItem != NULL &&

				// Test file: "reasoning\Scientific challenge - Confirmation"
				( !isPossessive ||
				!existingSpecificationItem->isUserAssignment() ) &&

			( previousRelationWordItem = existingSpecificationItem->relationCollectionWordItem() ) != NULL &&

				( ( !isSpecificationWordSpanishAmbiguous &&

					// Test file: "reasoning\family\John - Peter - Anna"
					( !isAssignment ||
					isPossessive ||
					isQuestion ||
					userSpecificationItem_ != NULL ||
					previousRelationWordItem == relationWordItem ||
					!existingSpecificationItem->isSelfGeneratedSpecification() ||
					existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, previousRelationWordItem ) != NULL ) ) ||

				// Typical for Spanish
				( isSpecificationWordSpanishAmbiguous &&

					( userSpecificationItem_ == NULL ||
					// Matching (past/present) tense
					existingSpecificationItem->isArchivedAssignment() == userSpecificationItem_->isArchivedAssignment() ) ) ) )
				{
				// Collect relation words
				if( ( collectionResult = collectRelation( isAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isQuestion, isSpecificationWordSpanishAmbiguous, specificationWordTypeNr, relationWordTypeNr, relationCollectionNr, existingSpecificationItem, generalizationWordItem, specificationWordItem, previousRelationWordItem, relationWordItem ) ).result != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );

				relationCollectionNr = collectionResult.createdCollectionNr;
				}

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&
			isSelfGenerated &&
			!isUserGeneralizationWord &&
			userSpecificationItem_ != NULL &&
			userSpecificationItem_->hasRelationWord() &&
			specificationWordItem->isUserSpecificationWord &&
			( hiddenSpanishSpecificationItem = generalizationWordItem->firstUnhiddenSpanishSpecificationItem() ) != NULL &&
			( firstHiddenJustificationItem = hiddenSpanishSpecificationItem->firstJustificationItem() ) != NULL &&
			firstHiddenJustificationItem->isReversibleAssumptionOrConclusion() &&
			!firstHiddenJustificationItem->isOlderItem() )
				{
				if( firstHiddenJustificationItem->isPrimaryActiveAssignmentSpecification() &&
				// Write Spanish assumption that is not hidden anymore
				generalizationWordItem->writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				// Add reversible justifications after Spanish specification not hidden anymore
				if( adminItem_->addReversibleJustificationsAfterSpanishSpecificationNotHiddenAnymore( hiddenSpanishSpecificationItem, specificationWordItem ) != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to reversible justifications after Spanish specification that is not hidden anymore in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		// Check for an assignment or a specification with opposite negative indicator
		if( !isAssignment &&
		isNegative &&
		isPossessive &&
		isArchivedAssignment_ )
			isAssignment = true;

		if( adminItem_->isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			{
			if( ( createAndAssignReturnResult = adminItem_->addSpecificationWithAuthorization( isAssignment, isCharacteristicFor, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
				return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification to word \"", generalizationWordItem->anyWordTypeString(), "\" with authorization" );
			}
		else
			{
			if( ( createAndAssignReturnResult = generalizationWordItem->addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, NULL ) ).result != RESULT_OK )
				return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		// No conflict found
		if( !globalVariables_->hasDisplayedWarning )
			{
			isArchivedAssignment_ = isArchivedAssignment;
			foundSpecificationItem = createAndAssignReturnResult.foundSpecificationItem;
			replacedAssignmentItem = createAndAssignReturnResult.replacedAssignmentItem;

			isDefinitionSpecification = ( !hasRelationWord &&
										!isQuestion &&
										adminItem_->isNounWordType( generalizationWordTypeNr ) &&
										adminItem_->isNounWordType( specificationWordTypeNr ) );

			// Not created
			if( ( createdSpecificationItem = createAndAssignReturnResult.createdSpecificationItem ) == NULL )
				{
				if( hasRelationWord &&
				!isSelfGenerated &&
				foundSpecificationItem != NULL )
					{
					userSpecificationItem_ = foundSpecificationItem;

					if( !isQuestion &&
					foundSpecificationItem->hasRelationWord() )
						simpleUserSpecificationItem_ = generalizationWordItem->firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem );
					}
				}
			else	// Created
				{
				if( !isSelfGenerated )
					{
					userSpecificationItem_ = createdSpecificationItem;

					if( !isQuestion &&
					createdSpecificationItem->hasRelationWord() )
						simpleUserSpecificationItem_ = createdSpecificationItem;
					}
				}

			if( !isExclusiveSpecification &&
			!isQuestion &&
			!isSelfGenerated &&
			specificationWordItem != NULL &&
			!generalizationWordItem->hasCurrentlyConfirmedSpecification() )
				{
				if( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN )
					{
					if( createdSpecificationItem != NULL )
						{
						if( isNegative )
							{
							if( createdSpecificationItem->hasSpecificationCollection() &&
							generalizationWordItem->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) != NULL &&
							// Make only-option-left assumption
							adminItem_->makeOnlyOptionLeftAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
								return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make an only-option-left assumption" );
							}
						else
							{
							if( !isPossessive )
								{
								if( isAssignment &&
								hasRelationWord &&

									( !createdSpecificationItem->hasSpecificationCollection() ||
									// Typical for Spanish
									// Spanish test files: "respuestas a las preguntas\familia\Preguntas respondidas indirectamente (con la confirmación)",
									//						"razonamiento\familia\Ana - Juan - Paz, José y Luisa"
									isSpecificationWordSpanishAmbiguous ) &&

								// Collect generalization words of a proper noun specification
								collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationCollectionNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

								// Test file: "Indirectly answered questions - John is masculine"
								if( userSpecificationItem_->isAdjectiveSpecification() &&
								userSpecificationItem_->hasNonCompoundSpecificationCollection() &&
								// Make indirectly answered question assumption
								adminItem_->makeIndirectlyAnsweredQuestionAssumption( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, createdSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
							addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isExclusiveGeneralization, false, isNegative, false, true, false, isCharacteristicFor, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
								return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( createdSpecificationItem == NULL )
								{
								if( !isConditional &&
								!isNegative &&
								foundSpecificationItem != NULL &&

									// Check if primary specification already has an assignment
									( ( foundAssignmentItem = generalizationWordItem->firstAssignmentItem( isPossessive, isQuestion, specificationWordItem ) ) != NULL ||
									isAssignment ) )
									{
									// Test files: "conflict\Assignment in conflict with definition specification",
									//				"conflict\Definition specification is already assigned"
									if( generalizationWordItem->writeSelectedSpecification( false, true, ( foundAssignmentItem == NULL ? foundSpecificationItem : foundAssignmentItem ) ) != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a non proper noun specification of word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
									strlen( writtenSentenceString ) == 0 )
										return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "Integrity error! I couldn't write the assignment in word \"", generalizationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\". I guess, the implementation of my writing modules is insufficient to write this particular sentence structure" );

									if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( foundAssignmentItem == NULL ? INTERFACE_LISTING_SENTENCE_ASSIGNMENT_IN_CONFLICT_WITH_DEFINITION_SPECIFICATION : INTERFACE_LISTING_SENTENCE_DEFINITION_SPECIFICATION_IS_ALREADY_ASSIGNED ) ) != RESULT_OK )
										return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a header" );

									if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
										return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a sentence" );
									}
								}
							else	// Created specification
								{
								if( isEveryGeneralization &&
								!isAssignment &&
								isPossessive &&
								// Draw specification-generalization 'part of' conclusion from a possessive definition sentence
								// See Block 2 of the included challenge document, or https://www.mafait.org/block_2
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isSpecific, isExclusiveGeneralization, false, isNegative, !isSpecific, false, false, false, true, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							}
						}
					}
				}

			if( ( ( isAssignment &&
			// At this stage, selections must be stored - rather than executed. So, don't assign them
			!isSelection ) ||

			replacedAssignmentItem != NULL ) &&

			// Skip for example imperative verbs
			adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				isAssignedOrClear = ( specificationWordItem != NULL &&
									specificationWordItem->isAdjectiveAssignedOrEmpty() );

				// Typical for Spanish
				if( isSpanishCurrentLanguage &&
				isAssignment &&
				existingSpecificationItem != NULL &&
				existingSpecificationItem->hasCompoundSpecificationCollection() )
					{
					if( relationCollectionNr == NO_COLLECTION_NR )
						{
						// Spanish test files: "razonamiento\familia\Complejo (13)",
						//						"razonamiento\familia\Complejo (17)"
						if( isSpecificationWordSpanishAmbiguous &&
						existingSpecificationItem->hasRelationCollection() &&
						generalizationWordItem->hasCurrentlyConfirmedSpecification() )
							relationCollectionNr = existingSpecificationItem->relationCollectionNr();
						}
					else
						{
						// Spanish test files: "razonamiento\familia\Complejo (7 - Becky)",
						//						"razonamiento\familia\Complejo (7 - Bonita)",
						//						"razonamiento\familia\Definición sencilla de la familia - Complejo (7 - Bonita)"
						if( isExistingOlderSpecification &&
						isSelfGenerated )
							relationCollectionNr = NO_COLLECTION_NR;
						}
					}

				if( ( createAndAssignCheckResult = assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to assign the specification" );

				if( ( createdAssignmentItem = createAndAssignCheckResult.createdSpecificationItem ) != NULL )
					{
					// Test files: "ambiguity\US presidents",
					//				"reasoning\Scientific challenge - Confirmation",
					//				"reasoning\family\Complex (7 - Becky)",
					//				"reasoning\family\Complex (7 - Claudia)",
					//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
					if( !isArchivedAssignment_ ||
					// Past tense
					createAndAssignReturnResult.createdSpecificationItem != NULL )
						{
						createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;
						createAndAssignReturnResult.createdOrFoundSpecificationItem = createdAssignmentItem;
						}

					if( isNegative &&
					createdAssignmentItem->isExclusiveSpecification() &&
					// Draw only option left noun conclusion
					adminItem_->drawOnlyOptionLeftNounConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem->specificationCollectionNr(), generalizationContextNr, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw an only-option-left noun conclusion by negative assignment" );
					}

				if( !isSelfGenerated &&
				( createdOrFoundAssignmentItem = ( createdAssignmentItem == NULL ?
														createAndAssignCheckResult.foundSpecificationItem :
														createdAssignmentItem ) ) != NULL )
					{
					userSpecificationItem_ = createdOrFoundAssignmentItem;

					if( !isQuestion &&
					specificationWordItem != NULL )
						{
						if( createdOrFoundAssignmentItem->hasRelationWord() )
							simpleUserSpecificationItem_ = ( isArchivedAssignment_ ? createdOrFoundAssignmentItem :
																					generalizationWordItem->firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem ) );

						// Find answers to questions
						if( generalizationWordItem->findAnswersToQuestions( specificationWordItem->compoundCollectionNr(), createdOrFoundAssignmentItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to find answers to questions" );
						}
					}
				}

			if( !isExclusiveSpecification &&
			!isPossessive &&
			!isSpecificationGeneralization &&
			specificationWordItem != NULL &&
			userSpecificationItem_ != NULL )
				{
				if( hasRelationWord &&
				isSelfGenerated &&

					( ( !specificationWordItem->isUserSpecificationWord &&

						( foundSpecificationItem == NULL ||
						foundSpecificationItem->isOlderItem() ||

						( foundSpecificationItem->hasNonCompoundSpecificationCollection() &&
						!foundSpecificationItem->hasOnlyOneRelationWord() ) ) ) ||

					// Typical for Spanish
					( isSpanishCurrentLanguage &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Spanish test file: "razonamiento\familia\Complejo (17)"
						foundSpecificationItem == NULL ) ) ) &&

				// Draw reversible conclusions
				adminItem_->drawReversibleConclusions( ( isArchivedAssignment || isChineseCurrentLanguage_ ), isChineseCurrentLanguage_, true, isSpanishCurrentLanguage, userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

				if( isQuestion )
					{
					if( hasRelationWord &&
					createdAssignmentItem != NULL )
						{
						assignmentSpecificationCollectionNr = createdAssignmentItem->specificationCollectionNr();
									
						// Check if specification doesn't exist yet
						if( generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, assignmentSpecificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) == NULL &&
						// Make suggestive question assumption
						adminItem_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, assignmentSpecificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, createdAssignmentItem, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make a suggestive question assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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

					foundSpecificationItem == NULL &&
					!userSpecificationItem_->hasSpecificationCollection() &&
					!userSpecificationItem_->isExclusiveSpecification() &&
					// Draw specification-generalization conclusion
					adminItem_->drawSpecificationGeneralizationConclusion( isArchivedAssignment, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a specification-generalization conclusion" );
					}
				}
			}

		return createAndAssignReturnResult;
		}


	protected:
	// Constructor

	AdminSpecification( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected assignment functions

	signed char assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignSelectionSpecification";

		if( assignmentSelectionItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given assignment selection item is undefined" );

		if( assignSpecification( false, assignmentSelectionItem->isAssignedOrEmpty(), assignmentSelectionItem->isInactiveAssignment(), assignmentSelectionItem->isArchivedAssignment(), assignmentSelectionItem->isNegative(), assignmentSelectionItem->isPossessive(), assignmentSelectionItem->isSpecificationGeneralization(), assignmentSelectionItem->isUniqueUserRelation(), assignmentSelectionItem->assumptionLevel(), assignmentSelectionItem->prepositionParameter(), NO_QUESTION_PARAMETER, assignmentSelectionItem->relationWordTypeNr(), assignmentSelectionItem->relationCollectionNr(), assignmentSelectionItem->generalizationContextNr(), assignmentSelectionItem->specificationContextNr(), assignmentSelectionItem->relationContextNr(), assignmentSelectionItem->nEnteredRelationWords(), NULL, assignmentSelectionItem->generalizationWordItem(), assignmentSelectionItem->specificationWordItem(), assignmentSelectionItem->relationWordItem(), assignmentSelectionItem->specificationString() ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a specification" );

		return RESULT_OK;
		}

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( adminItem_->isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			return adminItem_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		return generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, relationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nEnteredRelationWords, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, NULL );
		}


	// Protected specification functions

	void initializeLinkedWord()
		{
		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;
		linkedGeneralizationWordItem_ = NULL;

		nPreviousChineseUserRelationWords_ = 0;
		previousChineseStartRelationReadItem_ = NULL;
		previousChineseEndRelationReadItem_ = NULL;
		}

	void initializeAdminSpecificationVariables()
		{
		isArchivedAssignment_ = false;
		isChineseCurrentLanguage_ = false;
		isSpanishCurrentLanguage_ = false;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		// Don't initialize this variable here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

		// Don't initialize this variable here. It is initialized by function initializeLinkedWord()
//		nPreviousChineseUserRelationWords_ = 0;
		userRelationContextNr_ = NO_CONTEXT_NR;

		// Don't initialize this variable here. It is initialized by function initializeLinkedWord()
//		previousChineseStartRelationReadItem_ = NULL;
//		previousChineseEndRelationReadItem_ = NULL;

		simpleUserSpecificationItem_ = NULL;
		userSpecificationItem_ = NULL;

		// Don't initialize this variable here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordItem_ = NULL;
		}

	signed char addUserSpecification( bool isAction, bool isAssignment, bool isCharacteristicFor, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short userAssumptionLevel, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationReadItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		bool hasAddedSpecification = false;
		bool hasAlreadyAddedChineseRelationWords = false;
		bool hasLinkedPossessiveDeterminer = false;
		bool hasNonCompoundUserSpecificationCollection = false;
		bool hasRelationWord = ( startRelationReadItem != NULL );
		bool isAmbiguousRelationContext = false;
		bool isChineseCurrentLanguage = adminItem_->isChineseCurrentLanguage();
		bool isChineseReasoning = false;
		bool isCurrentNounValue;
		bool isDrawingPossessiveReversibleConclusions;
		bool isGeneralizationProperNoun;
		bool isGeneralizationReasoningWordType;
		bool isNegativeSpecification;
		bool isNonChineseNumeral;
		bool isPreposition = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationWord;
		bool isSelection = ( selectionListNr != NO_LIST_NR );
		bool isSkippingThisGeneralizationPart = false;
		bool isSpanishAmbiguousSpecificationWord;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isSpecificationWord;
		bool isSpecificationWordAlreadyAssignedByComparison = false;
		bool isSystemStartingUp = adminItem_->isSystemStartingUp();
		bool isText;
		bool isValueSpecificationWord = false;
		bool isWaitingForRelation = false;
		bool isWaitingForText = false;
		unsigned short currentSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short generalizationWordTypeNr;
		unsigned short linkedSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short valueGeneralizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short wordOrderNr;
		unsigned int nEnteredRelationWords = 0;
		unsigned int nUserRelationWords = globalVariables_->nUserRelationWords;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
#ifdef _MSC_VER
		int intValue;		// Required by Visual Studio
#endif
		ReadItem *currentReadItem;
		ReadItem *firstRelationReadItem;
		ReadItem *lastGeneralizationReadItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		WordItem *adjectiveSpecificationWordItem;
		WordItem *collectionWordItem;
		WordItem *compoundGeneralizationWordItem = NULL;
		WordItem *currentSpecificationWordItem = NULL;
		WordItem *generalizationWordItem;
		WordItem *linkedSpecificationWordItem = NULL;
		WordItem *lookAheadSpanishRelationWordItem;
		WordItem *previousSpecificationWordItem = NULL;
		WordItem *readWordItem;
		WordItem *relationWordItem;
		WordItem *valueGeneralizationWordItem = NULL;
		char *readWordTypeString;
		char *specificationString = NULL;
		CreateAndAssignResultType createAndAssignResult;
		CompoundResultType compoundResult;
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addUserSpecification";

		if( generalizationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given generalization read item is undefined" );

		if( ( generalizationWordItem = generalizationReadItem->readWordItem() ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given generalization read item has no read word" );

		if( ( currentReadItem = startSpecificationReadItem ) == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given start specification read item is undefined" );

		if( endSpecificationReadItem == NULL )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "The given end specification read item is undefined" );

		isGeneralizationProperNoun = generalizationReadItem->isProperNoun();
		generalizationWordTypeNr = generalizationReadItem->wordTypeNr();
		lastGeneralizationReadItem = generalizationReadItem->nextReadItem();

		isChineseCurrentLanguage_ = isChineseCurrentLanguage;
		isSpanishCurrentLanguage_ = isSpanishCurrentLanguage;

		isGeneralizationReasoningWordType = adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr );

		while( lastGeneralizationReadItem != NULL &&
		!lastGeneralizationReadItem->isGeneralizationWord() )
			{
			if( lastGeneralizationReadItem->isPossessiveDeterminer() )
				hasLinkedPossessiveDeterminer = true;
			else
				{
				// Typical for Chinese: Relation before specification
				if( isChineseCurrentLanguage &&
				isGeneralizationProperNoun &&
				!isQuestion &&
				prepositionParameter > NO_PREPOSITION_PARAMETER &&
				lastGeneralizationReadItem->isSpecificationWord() )
					{
					linkedSpecificationWordTypeNr = lastGeneralizationReadItem->wordTypeNr();
					linkedSpecificationWordItem = lastGeneralizationReadItem->readWordItem();
					}
				}

			lastGeneralizationReadItem = lastGeneralizationReadItem->nextReadItem();
			}

		// Typical for Chinese: Relation first, then specification
		if( linkedSpecificationWordTypeNr > NO_WORD_TYPE_NR )
			{
			hasAddedSpecification = true;
			hasAlreadyAddedChineseRelationWords = true;

			if( nUserRelationWords == 0 )
				{
				hasRelationWord = true;
				isChineseReasoning = true;

				nUserRelationWords = nPreviousChineseUserRelationWords_;
				globalVariables_->nUserRelationWords = nUserRelationWords;

				startRelationReadItem = previousChineseStartRelationReadItem_;
				endRelationReadItem = previousChineseEndRelationReadItem_;
				}

			if( !isQuestion )
				{
				if( ( contextResult = getRelationContext( isArchivedAssignment, isPossessive, generalizationWordItem, linkedSpecificationWordItem, startRelationReadItem ) ).result != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to get a Chinese relation context" );

				isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
				userRelationContextNr_ = contextResult.contextNr;
				}

			// Remember variables below
			nPreviousChineseUserRelationWords_ = nUserRelationWords;
			previousChineseStartRelationReadItem_ = startRelationReadItem;
			previousChineseEndRelationReadItem_ = endRelationReadItem;

			// Add Chinese user specification with relation
			if( addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, NO_SELECTION_LEVEL, NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, nEnteredRelationWords, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, linkedSpecificationWordItem ) != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a Chinese user specification with a relation to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		do	{
			isNonChineseNumeral = currentReadItem->isNonChineseNumeral();
			isPreposition = currentReadItem->isPreposition();
			isRelationWord = currentReadItem->isRelationWord();
			isSpecificationWord = currentReadItem->isSpecificationWord();
			isText = currentReadItem->isText();
			wordOrderNr = currentReadItem->wordOrderNr();
			readWordItem = currentReadItem->readWordItem();

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
					if( generalizationReadItem->wordOrderNr() < wordOrderNr )
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
					prepositionParameter == NO_PREPOSITION_PARAMETER )
						{
						if( currentReadItem->isGeneralizationWord() )
							{
							isSkippingThisGeneralizationPart = true;
							isWaitingForRelation = true;
							}
						else
							{
							if( isSpecificationWord &&
							generalizationWordItem->isAdjectiveComparison() )
								{
								isWaitingForText = true;
								isSpecificationWordAlreadyAssignedByComparison = true;
								currentSpecificationWordTypeNr = currentReadItem->wordTypeNr();
								currentSpecificationWordItem = readWordItem;
								}
							}
						}
					}
				}

			if( !isWaitingForText &&
			!isSkippingThisGeneralizationPart &&
			!currentReadItem->isNegative() &&
			// Skip numeral 'both'. Typically for English: in both ... and ...
			!currentReadItem->isNumeralBoth() )
				{
				specificationString = NULL;

				if( isText )
					specificationString = currentReadItem->readString;

				if( isSpecificationWordAlreadyAssignedByComparison )
					isSpecificationWordAlreadyAssignedByComparison = false;
				else
					{
					currentSpecificationWordItem = readWordItem;
					currentSpecificationWordTypeNr = ( isQuestion &&
														currentReadItem->isNoun() ? WORD_TYPE_NOUN_SINGULAR :
																					currentReadItem->wordTypeNr() );
					}

				if( isNonChineseNumeral &&
				isPossessive &&
				!isRelationWord &&
				( readWordTypeString = currentReadItem->readWordTypeString() ) != NULL )
#ifdef _MSC_VER
					intValue = sscanf( readWordTypeString, "%u", &nEnteredRelationWords );
#endif
#ifndef _MSC_VER
					sscanf( readWordTypeString, "%u", &nEnteredRelationWords );
#endif
				else
					{
					if( !isQuestion &&
					!isSelection &&
					isSpecificationWord &&
					!hasAlreadyAddedChineseRelationWords &&
					startRelationReadItem != NULL &&
					!generalizationWordItem->isImperativeVerbDisplayLoginOrRead() )
						{
						if( ( contextResult = getRelationContext( isArchivedAssignment, isPossessive, generalizationWordItem, currentSpecificationWordItem, startRelationReadItem ) ).result != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to get a relation context" );

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
						prepositionParameter == WORD_PARAMETER_PREPOSITION_OF &&
						endRelationReadItem != NULL &&
						endRelationReadItem->isChineseReversedImperativeNoun() ) ) ) )
						{
						// Typical for Chinese
						if( isChineseCurrentLanguage &&
						!isValueSpecificationWord &&
						currentReadItem->isNounValueAhead() )
							{
							isValueSpecificationWord = true;
							valueGeneralizationWordTypeNr = generalizationWordTypeNr;
							valueGeneralizationWordItem = generalizationWordItem;
							}

						isCurrentNounValue = currentReadItem->isNounValue();

						if( isValueSpecificationWord )
							{
							// Skip Chinese value word, because it is already processed
							if( !isCurrentNounValue )
								{
								hasAddedSpecification = true;

								if( hasRelationWord )
									return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "Adding a value specification with relation isn't implemented" );

								if( adminItem_->createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nEnteredRelationWords, valueGeneralizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a value selection item" );

								// Value, but no relation
								if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nEnteredRelationWords, NULL, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a value specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
										if( addUserSpecificationWithRelation( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, selectionLevel, selectionListNr, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, nEnteredRelationWords, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else	// No relation word
										{
										// Selection, no value, no relation
										if( adminItem_->createSelectionPart( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nEnteredRelationWords, generalizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part" );
										}
									}
								else	// No selection
									{
									if( previousSpecificationWordItem != NULL &&
									currentSpecificationWordItem != NULL &&

										// Existing specification is user specification
										( ( existingSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, currentSpecificationWordItem ) ) == NULL ||
										existingSpecificationItem->isSpecificationGeneralization() ) &&

									// Collect specification words
									collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to collect specification words" );
									}
								}

							if( !isSelection ||
							!currentReadItem->isAdjectiveAssignedOrEmpty() )
								{
								doneSpecificationWordOrderNr_ = wordOrderNr;
								linkedSpecificationWordTypeNr = currentSpecificationWordTypeNr;
								linkedSpecificationWordItem = currentSpecificationWordItem;

								if( !isCurrentNounValue )
									{
									hasAddedSpecification = true;

									// Login is already created by during startup
									if( !generalizationWordItem->isImperativeVerbDisplayLoginOrRead() )
										{
										if( hasRelationWord )
											{
											if( ( !hasAlreadyAddedChineseRelationWords ||
											isChineseReasoning ) &&

											// No selection, no value, with relation
											addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, isChineseReasoning, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, NO_SELECTION_LEVEL, NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, nEnteredRelationWords, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a user specification with a relation to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else	// No relation word
											{
											isNegativeSpecification = ( isConditional ? false : isNegative );

											// No selection, no value, no relation
											if( ( createAndAssignResult = addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegativeSpecification, isPartOf, isPossessive, isSelection, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nEnteredRelationWords, NULL, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ) ).result != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a specification without relation to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

											if( currentSpecificationWordItem != NULL &&
											( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) != NULL )
												{
												if( !isNegative &&
												!isSpecificationGeneralization )
													{
													if( isExclusiveSpecification )
														{
														if( currentSpecificationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
														previousSpecificationWordItem != NULL &&
														// For now, limit the negative specification singular noun words to two
														globalVariables_->nUserSpecificationWords == 2 &&
														// Draw exclusive negative user conclusions
														adminItem_->drawExclusiveNegativeUserConclusions( isArchivedAssignment, createdSpecificationItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw an exclusive negative user conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
																if( currentSpecificationWordItem->firstNegativeSpecificationItem() != NULL &&
																// Draw simple negative conclusions
																adminItem_->drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, createdSpecificationItem, userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															else	// Definition specification
																{
																if( currentSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE &&
																// Feminine or masculine specification word
																currentSpecificationWordItem->isFeminineOrMasculineWord() &&
																( adjectiveSpecificationWordItem = adminItem_->predefinedWordItem( currentSpecificationWordItem->isFeminineWord() ? WORD_PARAMETER_ADJECTIVE_MASCULINE :
																																													WORD_PARAMETER_ADJECTIVE_FEMININE ) ) != NULL )
																	{
																	// For now, limit the negative specification adjective words
																	if( globalVariables_->nUserSpecificationWords > 1 )
																		return adminItem_->addError( functionNameString, moduleNameString_, "Multiple negative specification adjective words is not supported yet" );

																	// Draw negative user definition conclusions
																	if( adminItem_->drawNegativeUserDefinitionConclusions( isArchivedAssignment, createdSpecificationItem, generalizationWordItem, adjectiveSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																		return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative user definition conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
										currentSpecificationWordItem != NULL &&

											// Noun specification word
											( adminItem_->isNounWordType( currentSpecificationWordTypeNr ) ||

											// Adjective specification word
											( isEveryGeneralization &&
											currentSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE ) ) )
											{
											isSpanishAmbiguousSpecificationWord = currentSpecificationWordItem->isNounWordSpanishAmbiguous();

											if( userSpecificationItem_ != NULL )
												{
												hasNonCompoundUserSpecificationCollection = userSpecificationItem_->hasNonCompoundSpecificationCollection();
												userSpecificationCollectionNr = userSpecificationItem_->specificationCollectionNr();
												}

											if( isPossessive )
												{
												if( isSpanishAmbiguousSpecificationWord &&
												startRelationReadItem != NULL &&
												userSpecificationItem_ != NULL &&
												globalVariables_->nUserRelationWords == 1 &&
												( lookAheadSpanishRelationWordItem = startRelationReadItem->lookAheadRelationWordItem() ) != NULL &&
												!lookAheadSpanishRelationWordItem->isMasculineWord() &&
												// Mark relation word as masculine
												lookAheadSpanishRelationWordItem->markWordAsMasculine() != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark relation word \"", lookAheadSpanishRelationWordItem->anyWordTypeString(), "\" as masculine" );
												}
											else	// Non-possessive
												{
												if( userSpecificationItem_ != NULL )
													{
													if( isSpanishCurrentLanguage &&
													isGeneralizationProperNoun &&
													generalizationWordItem->isMasculineWord() &&
													// Display Spanish assumptions that are not hidden anymore
													displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords( isAssignment, isArchivedAssignment, currentSpecificationWordItem ) != RESULT_OK )
														return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to display Spanish assumptions that are not hidden anymore in context words" );

													if( currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE )
														{
														if( userSpecificationCollectionNr > NO_COLLECTION_NR &&

															// Proper noun specification
															( ( isGeneralizationProperNoun &&
															hasNonCompoundUserSpecificationCollection &&
															!isExclusiveSpecification ) ||

															// Definition specification
															( !isGeneralizationProperNoun &&
															!hasNonCompoundUserSpecificationCollection ) ||

															// Typical for Spanish
															isSpanishCurrentLanguage ) &&

														!userSpecificationItem_->isOlderItem() &&
														// Avoid idle call
														globalVariables_->firstUserDefinedProperNounWordItem != NULL &&
														// Draw proper noun part-of conclusions
														adminItem_->drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, isChineseCurrentLanguage, isSpanishCurrentLanguage, ( isGeneralizationProperNoun ? currentSpecificationWordItem : generalizationWordItem ), NULL ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

														if( !isSpecificationGeneralization &&
														generalizationWordItem != currentSpecificationWordItem &&
														userSpecificationItem_->hasSpecificationCollection() &&

															// Plural noun unknown
															// Test file: "reasoning\Knowledge of organisms (1)"
															( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||

															( isEveryGeneralization &&

																( !isExclusiveSpecification ||
																userSpecificationItem_->hasCompoundSpecificationCollection() ) ) ) &&

														// Make generalization assumption
														adminItem_->makeGeneralizationAssumption( isArchivedAssignment, isChineseCurrentLanguage, generalizationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make a generalization assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}

													// Existing specification is user specification
													if( ( existingSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, currentSpecificationWordItem ) ) != NULL &&

														( existingSpecificationItem->isExclusiveSpecification() ||

														( userSpecificationCollectionNr > NO_COLLECTION_NR &&
														!existingSpecificationItem->isAssignment() ) ) &&

													// Avoid idle call
													currentSpecificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() != NULL &&
													// Draw compound specification substitution conclusion
													adminItem_->drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, existingSpecificationItem, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
														return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												// Definition specification
												// Test file: "Knowledge of organisms (2)"
												if( ( ( !isGeneralizationProperNoun &&
												isEveryGeneralization &&
												!isExclusiveSpecification &&
												userSpecificationCollectionNr == NO_COLLECTION_NR &&
												userSpecificationItem_ != NULL &&
												!userSpecificationItem_->hasGeneralizationCollection() ) ||

												// Proper noun specification
												( isGeneralizationProperNoun &&

													( ( !hasRelationWord &&

														( !isAssignment ||

														( userSpecificationItem_ != NULL &&
														!userSpecificationItem_->hasRelationWord() ) ) ) ||

													( !isArchivedAssignment &&
													userSpecificationItem_ != NULL &&
													userSpecificationItem_->isAssignment() &&
													!userSpecificationItem_->isOlderItem() ) ||

													// Typical for Spanish
													isSpanishCurrentLanguage ) ) ) &&

												// Avoid idle call
												currentSpecificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() != NULL &&
												// Draw specification substitution conclusion or ask a question
												adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem, NULL ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( userSpecificationItem_ != NULL )
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
														simpleUserSpecificationItem_ != NULL &&
														( collectionWordItem = currentSpecificationWordItem->collectionWordItem( userSpecificationCollectionNr ) ) != NULL &&
														collectionWordItem->isNounWordSpanishAmbiguous() &&
														( additionalDefinitionSpecificationItem = collectionWordItem->firstSpecificationItem( false, false, false, collectionWordItem ) ) != NULL &&
														// Draw simple negative Spanish conclusions
														adminItem_->askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, isSpanishCurrentLanguage, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem_, additionalDefinitionSpecificationItem, simpleUserSpecificationItem_, simpleUserSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to simple negative Spanish conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													else	// Definition specification
														{
														// Avoid idle call
														if( generalizationWordItem->firstSpecificationGeneralizationItem( true ) != NULL )
															{
															// Draw definition specification substitution conclusion
															if( ( compoundResult = adminItem_->drawDefinitionSpecificationSubstitutionConclusion( isChineseCurrentLanguage, isSpanishCurrentLanguage, generalizationWordTypeNr, userSpecificationItem_, generalizationWordItem ) ).result != RESULT_OK )
																return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a definition specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

															compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
															}
														}
													}
												}

											// Avoid idle call
											if( !isSystemStartingUp &&
											isGeneralizationProperNoun &&
											!isUncountableGeneralizationNoun &&
											userSpecificationItem_ != NULL )
												{
												// No relation word
												if( ( ( startRelationReadItem == NULL &&
												// Typical for Spanish
												isSpanishCurrentLanguage &&
												!userSpecificationItem_->isOlderItem() ) ||

												// Relation word
												( startRelationReadItem != NULL &&

													( !userSpecificationItem_->isOlderItem() ||
													// Typical for Chinese
													// Chinese test file: "Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura"
													isChineseCurrentLanguage ) ) ) &&

												// Draw reversible conclusions
												adminItem_->drawReversibleConclusions( isAssignment, isChineseCurrentLanguage, true, isSpanishCurrentLanguage, userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( startRelationReadItem != NULL &&
												userSpecificationCollectionNr > NO_COLLECTION_NR &&
												// Including relation word of assignment
												globalVariables_->nUserRelationWords == 1 &&

													// Not older
													( ( !userSpecificationItem_->isOlderItem() &&

														( ( isPossessive &&
														userSpecificationItem_->hasCompoundSpecificationCollection() ) ||

														// Typical for Spanish
														isSpanishAmbiguousSpecificationWord ) ) ||

													// Typical for Chinese
													// Chinese test file: "Collect afterwards - John-Anna-Paul-Joe - John-Anna-Laura"
													( isChineseCurrentLanguage &&
													userSpecificationItem_->isOlderItem() &&
													userSpecificationItem_->hasUpdatedInformation() ) ) &&

												( firstRelationReadItem = startRelationReadItem->firstRelationWordReadItem() ) != NULL &&
												( relationWordItem = firstRelationReadItem->readWordItem() ) != NULL &&
												// Make an exclusive specification substitution assumption
												adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isChineseCurrentLanguage, isExclusiveSpecification, false, !isPossessive, isSpanishCurrentLanguage, false, firstRelationReadItem->wordTypeNr(), currentSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, currentSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
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

							( currentReadItem->isArticle() ||
							// Typical for Chinese
							currentReadItem->isConjunction() ) )
							nEnteredRelationWords = 0;
						}

					if( !isQuestion &&
					currentSpecificationWordItem != NULL &&
					linkedSpecificationWordItem != NULL &&
					userSpecificationItem_ != NULL &&

						( ( !hasRelationWord &&
						isSpecificationWord ) ||

						( isRelationWord &&
						linkedSpecificationWordItem != currentSpecificationWordItem ) ) )
						{
						relationWordItem = ( hasRelationWord ? currentSpecificationWordItem : NULL );

						if( !isEveryGeneralization &&
						isGeneralizationReasoningWordType &&
						!isNegative &&
						!isPreposition &&
						!isValueSpecificationWord &&
						linkedSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&

							// Definition specification
							// Test files: "reasoning\Boiling point",
							//				"reasoning\Condensation point"
							( ( !isGeneralizationProperNoun &&
							isPossessive &&

								( userSpecificationItem_->isUncountableGeneralizationNoun() ||
								// Typical for Spanish and French
								hasRelationWord ) ) ||

							// Proper noun specification
							( isGeneralizationProperNoun &&

								( ( ( relationWordItem != NULL ||
								!userSpecificationItem_->isOlderItem() ) &&

								userSpecificationItem_->hasSpecificationCollection() ) ||

								// Typical for Chinese
								// Test files: "reasoning\Boiling point",
								//				"reasoning\Condensation point"
								( isChineseCurrentLanguage &&
								isPossessive &&
								userSpecificationItem_->isUncountableGeneralizationNoun() ) ||

								// Typical for Spanish
								isSpanishCurrentLanguage ) ) ) &&

						// Avoid idle call
						generalizationWordItem->bestMatchingRelationSpecificationItem( true, true, false, isPossessive, linkedSpecificationWordItem, relationWordItem ) != NULL &&
						// Make an exclusive specification substitution assumption
						adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isChineseCurrentLanguage, isExclusiveSpecification, isNegative, isPossessive, isSpanishCurrentLanguage, isUncountableGeneralizationNoun, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, linkedSpecificationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );

						if( isRelationWord ||
						hasLinkedPossessiveDeterminer )
							{
							// Linked specification
							if( currentSpecificationWordItem == linkedSpecificationWordItem )
								{
								if( linkedGeneralizationWordItem_ != NULL )
									return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "Linked word \"", linkedGeneralizationWordItem_->anyWordTypeString(), "\" is already assigned" );

								linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
								linkedGeneralizationWordItem_ = generalizationWordItem;
								}

							if( isGeneralizationProperNoun &&
							hasRelationWord &&
							!isExclusiveSpecification &&
							userSpecificationItem_->hasUpdatedInformation() &&
							// Possessive user specification doesn't exist
							currentSpecificationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, linkedSpecificationWordItem ) == NULL )
								{
								isDrawingPossessiveReversibleConclusions = ( ( isAssignment &&
																			userSpecificationItem_->isOlderItem() ) ||

																			// Typical for Chinese
																			isChineseCurrentLanguage );

								if( ( isDrawingPossessiveReversibleConclusions ||

								// Non-assignment
								( !isAssignment &&
								isPossessive &&
								userSpecificationItem_->hasRelationCollection() ) ||

								// Typical for Spanish
								// Spanish test file: "razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
								( isSpanishCurrentLanguage &&
								isAssignment &&
								!isPossessive ) ) &&

								// Draw reversible conclusions
								adminItem_->drawReversibleConclusions( isAssignment, isChineseCurrentLanguage, isDrawingPossessiveReversibleConclusions, isSpanishCurrentLanguage, userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								if( userSpecificationItem_->hasSpecificationCollection() &&
								// Make an exclusive specification substitution assumption
								adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isChineseCurrentLanguage, false, false, !isPossessive, isSpanishCurrentLanguage, false, currentSpecificationWordTypeNr, linkedSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, currentSpecificationWordItem, linkedSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", currentSpecificationWordItem->anyWordTypeString(), "\" and specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );
								}
							}
						else
							{
							if( !isSelection &&
							currentReadItem->isAdjectiveAssigned() &&
							inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_START, currentReadItem->readWordTypeString(), INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_POSITION, wordOrderNr, INTERFACE_SENTENCE_WARNING_WORD_CAN_ONLY_BE_USED_IN_CONDITION_OF_SELECTION_END ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
							}
						}
					}
				}
			}
		while( currentReadItem != endSpecificationReadItem &&
		( currentReadItem = currentReadItem->nextReadItem() ) != NULL &&
		// No conflict found
		!globalVariables_->hasDisplayedWarning );

		if( !hasAddedSpecification )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I have not added any specification" );

		if( isGeneralizationProperNoun &&
		!isQuestion &&
		!isSelection )
			{
			if( hasRelationWord )
				{
				// Recalculate assumption levels of in touched words
				if( recalculateAssumptionLevelsInTouchedWords() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumption levels of specifications in all touched words" );
				}
			else
				{
				// Recalculate assumption levels of specifications in generalization word
				if( generalizationWordItem->recalculateAssumptionLevelsInWord( false ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumption levels of specifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char collectGeneralizationWordWithPreviousOne( bool isAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int relationCollectionNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isExclusiveGeneralization = false;
		bool isSpecificationWordSpanishAmbiguous;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *previousGeneralizationWordItem = NULL;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectGeneralizationWordWithPreviousOne";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationWordItem = globalVariables_->firstSpecificationWordItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The first specification word is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		// Do for all specification words
		do	{
			if( currentGeneralizationWordItem != generalizationWordItem &&
			currentGeneralizationWordItem->hasWordType( false, generalizationWordTypeNr ) &&
			// Try to find matching specification word
			( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, isPossessive, relationCollectionNr, specificationWordItem, relationWordItem ) ) != NULL )
				{
				// Languages and users
				if( ( isAssignment &&
				!isSpecificationWordSpanishAmbiguous ) ||

				foundSpecificationItem->isExclusiveGeneralization() )
					isExclusiveGeneralization = true;

				previousGeneralizationWordItem = currentGeneralizationWordItem;
				}
			}
		// Continue search to get the most recent generalization word
		while( ( currentGeneralizationWordItem = currentGeneralizationWordItem->nextSpecificationWordItem ) != NULL );

		if( previousGeneralizationWordItem != NULL &&
		collectGeneralizationWords( isExclusiveGeneralization, generalizationWordTypeNr, specificationWordTypeNr, previousGeneralizationWordItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalization words \"", previousGeneralizationWordItem->anyWordTypeString(), "\" and \"", generalizationWordItem->anyWordTypeString(), "\"" );

		return RESULT_OK;
		}

	signed char recalculateAssumptionLevelsInTouchedWords()
		{
		WordItem *currentTouchedWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptionLevelsInTouchedWords";

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				if( currentTouchedWordItem->recalculateAssumptionLevelsInWord( true ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption levels of specifications in word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	SpecificationItem *simpleUserSpecificationItem()
		{
		return simpleUserSpecificationItem_;
		}

	SpecificationItem *userSpecificationItem()
		{
		return userSpecificationItem_;
		}

	CollectionResultType collectRelationWords( bool isExclusiveSpecification, bool isForcingNewCollectionNr, unsigned short relationWordTypeNr, unsigned short commonWordTypeNr, unsigned int previousRelationCollectionNr, unsigned int relationCollectionNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *commonWordItem )
		{
		bool isCheckingForSameCollectionNumber;
		CollectionResultType collectionResult;
		CollectionResultType returnCollectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectRelationWords";

		if( previousRelationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given previous relation word item is undefined" );

		if( currentRelationWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given current relation word item is undefined" );

		if( commonWordItem == NULL )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given common word item is undefined" );

		if( previousRelationWordItem == currentRelationWordItem )
			return adminItem_->startCollectionResultError( functionNameString, moduleNameString_, "The given previous and current relation words are the same word" );

		if( !isForcingNewCollectionNr &&
		relationCollectionNr == NO_COLLECTION_NR )
			{
			// Get previous relation collection if not defined yet
			if( previousRelationCollectionNr == NO_COLLECTION_NR )
				previousRelationCollectionNr = previousRelationWordItem->collectionNr( commonWordItem );

			// Try to find existing relation collection number
			relationCollectionNr = ( previousRelationCollectionNr == NO_COLLECTION_NR ? currentRelationWordItem->collectionNr( commonWordItem ) : previousRelationCollectionNr );
			}

		isCheckingForSameCollectionNumber = ( !isForcingNewCollectionNr &&
											!commonWordItem->isNounWordSpanishAmbiguous() );

		// Each collection comes in pairs
		if( ( collectionResult = previousRelationWordItem->createCollectionItem( isCheckingForSameCollectionNumber, isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, relationCollectionNr, currentRelationWordItem, commonWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to collect previous word \"", previousRelationWordItem->anyWordTypeString(), "\" with current word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

		if( relationCollectionNr == NO_COLLECTION_NR )
			relationCollectionNr = collectionResult.createdCollectionNr;

		if( currentRelationWordItem->createCollectionItem( false, isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, relationCollectionNr, previousRelationWordItem, commonWordItem, NULL ).result != RESULT_OK )
			return adminItem_->addCollectionResultError( functionNameString, moduleNameString_, "I failed to collect current word \"", currentRelationWordItem->anyWordTypeString(), "\" with previous word \"", previousRelationWordItem->anyWordTypeString(), "\"" );

		returnCollectionResult.createdCollectionNr = relationCollectionNr;
		return returnCollectionResult;
		}

	CreateAndAssignResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNounEnding, bool isAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, unsigned short assumptionLevel, unsigned short justificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasDisplayedIntegrityWarning = false;
		bool hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion;
		bool hasRelationWordConfirmedSpecification = false;
		bool isEveryGeneralization;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isOlderFoundSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelectedUniqueUserRelation;
		bool isSpanishCurrentLanguage = isSpanishCurrentLanguage_;
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserGeneralizationWord;
		unsigned short calculatedAssumptionLevel;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *createdOrFoundJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *foundJustificationItem;
		SpecificationItem *createdSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSelfGeneratedSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( primarySpecificationItem != NULL &&
		primarySpecificationItem->isAnsweredQuestion() &&
		primarySpecificationItem->isSelfGeneratedQuestion() )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given primary specification item is obsolete. It is an answered, self-generated question" );

		// Check for existing justification
		if( ( foundJustificationItem = generalizationWordItem->foundJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) ) == NULL )
			{
			// Create justification
			if( ( justificationResult = generalizationWordItem->createJustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, NO_ORDER_NR, globalVariables_->currentSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to create a justification item" );

			createdJustificationItem = justificationResult.createdJustificationItem;
			}

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == NULL ? foundJustificationItem : createdJustificationItem ) ) == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I couldn't find or create a justification" );

		// No given assumption level
		if( assumptionLevel == NO_ASSUMPTION_LEVEL )
			{
			// Calculate assumption level
			if( ( calculatedAssumptionLevel = adminItem_->justificationAssumptionLevel( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem ) ) > MAX_LEVEL )
				return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "Assumption level overflow" );

			if( calculatedAssumptionLevel > NO_ASSUMPTION_LEVEL )
				assumptionLevel = calculatedAssumptionLevel;
			}

		hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
		isEveryGeneralization = ( !isGeneralizationProperNoun &&
								!isAssignment &&
								!isUncountableGeneralizationNoun );
		isSelectedUniqueUserRelation = ( justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION );

		if( ( createAndAssignResult = addSpecification( isAssignment, false, isArchivedAssignment, isCharacteristicFor, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpanishCurrentLanguage, isSpecificationGeneralization, isUncountableGeneralizationNoun, isSelectedUniqueUserRelation, false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification" );

		// No conflict found
		if( !globalVariables_->hasDisplayedWarning )
			{
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;

			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
				isOlderFoundSpecification = foundSpecificationItem->isOlderItem();

			if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
				{
				if( foundSpecificationItem == NULL )
					return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I couldn't find or create a specification" );

				if( foundSpecificationItem->isUserSpecification() )
					{
					hasDisplayedIntegrityWarning = true;

					// The result of this call is stored in a shared string. Hence, one call at the time.
					strcpy( errorString, "\nI found a user specification, whilst trying to create a self-generated specification:\n\tSpecificationItem: " );
					strcat( errorString, foundSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ";\n\tJustificationItem: " );
					strcat( errorString, createdOrFoundJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ".\n" );

					if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}

				if( !hasDisplayedIntegrityWarning )
					{
					if( createdJustificationItem != NULL &&
					createdJustificationItem->isReplacedOrDeletedItem() )
						return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The created justification item is replaced or deleted" );

					if( relationWordItem != NULL )
						hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();

					foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

					if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) == NULL )
						return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The found specification item has no justifications" );

					// A justification has been created or found, but the self-generated specification already exists
					// So, the justification needs to be added or replaced separately

					// Justification created
					if( createdJustificationItem != NULL &&
					// Justification doesn't exist yet in specification
					!foundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&

						( isUserGeneralizationWord ||
						assumptionLevel == NO_ASSUMPTION_LEVEL ||
						relationWordItem == NULL ||
						foundSpecificationItem->isSelfGeneratedAssumption() ||
						// Typical for Spanish
						// Spanish test files: "razonamiento\familia\Complejo (12)",
						//						"razonamiento\familia\Complejo (13)",
						//						"razonamiento\familia\Complejo (15)",
						//						among others
						isSpecificationWordSpanishAmbiguous ) )
						{
						// Process justification
						if( processJustification( hasRelationWordConfirmedSpecification, isOlderFoundSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, firstJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to process justifications of the found specification" );

						foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

						if( isOlderFoundSpecification &&
						!isPossessive &&
						!isQuestion &&
						relationWordItem == NULL &&
						foundSpecificationItem->isSelfGeneratedAssumption() &&

							// Definition specification
							( ( !isGeneralizationProperNoun &&
							primarySpecificationItem != NULL &&
							!primarySpecificationItem->isOlderItem() &&
							primarySpecificationItem->isAdjectiveSpecification() ) ||

							// Proper noun specification
							( isGeneralizationProperNoun &&

								// Test file: "reasoning\family\Correcting invalidated assumption (by knowledge)"
								( justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ||
								justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION ) ) ) &&

						// Recalculate assumption level of found assumption
						foundSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the assumption" );
						}
					}
				}

			if( !hasDisplayedIntegrityWarning &&
			!isNegative &&
			!isPartOf &&

			( createdJustificationItem != NULL ||
			createdSpecificationItem != NULL ||

			// Typical for Spanish
			( isSpanishCurrentLanguage &&
			foundSpecificationItem != NULL &&
			foundSpecificationItem->isHiddenSpanishSpecification() ) ) &&

			adminItem_->isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					createdSpecificationItem != NULL &&
					// Draw unique relation conclusion
					addSelfGeneratedSpecification( false, false, false, false, isExclusiveGeneralization, isExclusiveSpecification, true, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated unique specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					if( isGeneralizationProperNoun )
						{
						if( !isQuestion &&
						!isAssignment &&
						!isExclusiveSpecification &&
						relationWordItem == NULL &&

							( !isSpanishCurrentLanguage ||
							// Typical for Spanish
							createdSpecificationItem == NULL ||
							!createdSpecificationItem->isHiddenSpanishSpecification() ) &&

						// Avoid idle call
						specificationWordItem->firstNegativeSpecificationItem() != NULL &&
						// Draw simple negative conclusions
						adminItem_->drawSimpleNegativeConclusions( isArchivedAssignment, isSpanishCurrentLanguage, ( createdSpecificationItem == NULL ? foundSpecificationItem : createdSpecificationItem ), userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						// Non-exclusive specification
						if( ( ( !isExclusiveSpecification &&

							// Not created
							( ( createdSpecificationItem == NULL &&
							foundSpecificationItem != NULL &&

								// Not older
								( ( !foundSpecificationItem->isOlderItem() &&
								foundSpecificationItem->hasOnlyOneRelationWord() &&
								foundSpecificationItem->hasNonCompoundSpecificationCollection() &&

									( ( isUserGeneralizationWord &&
									userSpecificationItem_->isPossessive() ) ||

									( !isUserGeneralizationWord &&
									!foundSpecificationItem->hasJustificationWithAdditionalDefinitionSpecification() ) ||

									userSpecificationItem_->isActiveAssignment() ) ) ||

								// Older
								( foundSpecificationItem->isOlderItem() &&

									( ( foundSpecificationItem->hasRelationCollection() &&

										// Test file: "reasoning\family\John is the father - Paul - Laura"
										( isUserGeneralizationWord ||
										// Test files: "reasoning\family\Complex (4)",
										//				"reasoning\family\Complex (5)",
										//				"reasoning\family\Complex (6)"
										userSpecificationItem_->isActiveAssignment() ||
										// Test file: "reasoning\family\Paul - Joe - John and Anna"
										userSpecificationItem_->isPossessive() ||
										// Test file: "reasoning\family\Complex (13)"
										foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) == NULL ) ) ||

									// Typical for Spanish
									( isSpanishCurrentLanguage &&
									foundSpecificationItem->hasCompoundSpecificationCollection() ) ) ) ||

								// Typical for Spanish
								( isSpecificationWordSpanishAmbiguous &&
								!isAssignment ) ) ) ||

							// Created
							( createdSpecificationItem != NULL &&

								// Test files: "reasoning\family\Anna was the mother of Paul, Joe and Laura",
								//				"reasoning\family\John was the father of Paul, Joe and Laura",
								//				"reasoning\family\Past tense"
								( isArchivedAssignment ||

								// Test files: "conflict\family\Negative conflict - Pete is not a father",
								//				"conflict\family\No conflict in same sentence",
								//				"conflict\family\No conflict on multiple related specifications",
								//				among others
								!createdSpecificationItem->hasRelationWord() ||

								// Test files: "reasoning\family\Complex (2)",
								//				"reasoning\family\Complex (3)",
								//				"reasoning\family\Complex (4)",
								//				"reasoning\family\Complex (5)",
								//				"reasoning\family\Complex (6)",
								//				among others
								( !isUserGeneralizationWord &&
								specificationWordItem == userSpecificationItem_->specificationWordItem() ) ||

								// Typical for Spanish
								isSpanishCurrentLanguage ) &&

							( createdSpecificationItem->hasNonCompoundSpecificationCollection() ||

							// Typical for Spanish
							( isSpanishCurrentLanguage &&
							!isAssignment &&
							createdSpecificationItem->hasCompoundSpecificationCollection() ) ) ) ) ) ||

						// Exclusive specification
						// Typical for Spanish
						( isExclusiveSpecification &&
						isSpecificationWordSpanishAmbiguous ) ) &&

						// Draw proper noun part-of conclusions
						adminItem_->drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, isChineseCurrentLanguage_, isSpanishCurrentLanguage, specificationWordItem, ( isSpecificationWordSpanishAmbiguous ? relationWordItem : NULL ) ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
						}

					if( createdSpecificationItem != NULL )
						{
						// Proper noun specification
						if( isGeneralizationProperNoun )
							{
							// Test file: "reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
							if( hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion &&
							// Draw compound specification substitution conclusion
							adminItem_->drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else	// Definition specification
							{
							// Test file: "reasoning\Knowledge of organisms (1)"
							if( !isSpecificationGeneralization &&
							justificationTypeNr == JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION &&
							primarySpecificationItem != NULL &&
							primarySpecificationItem->isUserSpecification() &&
							createdSpecificationItem->hasSpecificationCollection() &&
							// Make generalization assumption
							adminItem_->makeGeneralizationAssumption( isArchivedAssignment, isChineseCurrentLanguage_, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make a generalization assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						}

					if( userSpecificationItem_ != NULL &&

						// Created specification
						( ( createdSpecificationItem != NULL &&

							// Not user generalization word
							( ( !isUserGeneralizationWord &&

								// Additional exclusive specification substitution conclusion
								( justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
								justificationTypeNr == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ||

								// Test files: "reasoning\Basic reasoning",
								//				"reasoning\family\Justification report",
								//				"reasoning\family\Simple family definition - Justification report"
								( isExclusiveSpecification &&
								!isQuestion &&
								!userSpecificationItem_->isExclusiveSpecification() ) ||

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
									justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) ) ) ) ||

							// User generalization word
							( isUserGeneralizationWord &&

								// Not found
								( ( foundSpecificationItem == NULL &&
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
								( foundSpecificationItem != NULL &&
								hasGeneralizationWordCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion ) ) ) ) ) ||

						// Found specification
						( foundSpecificationItem != NULL &&
						!isExclusiveSpecification &&
						isGeneralizationProperNoun &&
						!foundSpecificationItem->hasCompoundSpecificationCollection() &&

							// Not older
							( ( !isOlderFoundSpecification &&
							!isUserGeneralizationWord &&
							foundSpecificationItem->hasSpecificationCollection() &&

								( justificationTypeNr != JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ||
								// Test files: "reasoning\family\Complex (10)",
								//				"reasoning\family\My conclusions that are confirmed (without relation)",
								//				"reasoning\family\John has a daughter and 2 sons",
								//				"reasoning\family\This information is more specific"
								foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) == NULL ||

								// Typical for Chinese
								// Chinese test file: "This information is more specific"
								( isChineseCurrentLanguage_ &&
								userSpecificationItem_->isAssignment() ) ) ) ||

							// Older
							( isOlderFoundSpecification &&

								// Test files: "conflict\family\Joe is a mother",
								//				"question answering\family\Intelligent answering of questions",
								//				"reasoning\family\Avoid duplicate context",
								//				among others
								( ( foundSpecificationItem->hasCurrentCreationSentenceNr() &&
								userSpecificationItem_->hasRelationWord() ) ||

								// Spanish test files: "razonamiento\familia\Ana - Juan - Paz, José y Luisa",
								//						"razonamiento\familia\Complejo (8)",
								//						"razonamiento\familia\Complejo (16)",
								//						among others
								( isUserGeneralizationWord &&
								userSpecificationItem_->isSpecificationWordSpanishAmbiguous() ) ) ) ) ) ) &&

					// Avoid idle call
					specificationWordItem->firstNonNegativeNonPosessiveDefinitionSpecificationItem() != NULL &&
					// Draw specification substitution conclusion or ask a question
					adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, isSpanishCurrentLanguage, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			}

		createAndAssignResult.createdOrFoundSpecificationItem = createdSpecificationItem;

		if( foundSpecificationItem != NULL )
			{
			createAndAssignResult.foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

			if( createAndAssignResult.createdOrFoundSpecificationItem == NULL )
				createAndAssignResult.createdOrFoundSpecificationItem = foundSpecificationItem;
			}

		return createAndAssignResult;
		}
	};

/*************************************************************************
 *	"How amazing are the deeds of the Lord!
 *	All who delight in him should ponder them." (Psalm 111:2)
 *************************************************************************/
