/*	Class:			AdminSpecification
 *	Supports class:	AdminItem
 *	Purpose:		To create and assign specification structures
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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

#include "AdminItem.h"
#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "ReadItem.cpp"
#include "SelectionItem.cpp"

class AdminSpecification
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = false;
	bool isArchivedAssignment_ = false;
	bool isChineseCurrentLanguage_ = false;

	unsigned short doneSpecificationWordOrderNr_ = NO_ORDER_NR;
	unsigned short linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

	unsigned int nPreviousChineseUserRelationWords_ = 0;
	unsigned int userRelationContextNr_ = NO_CONTEXT_NR;

	ReadItem *previousChineseStartRelationReadItem_ = NULL;
	ReadItem *previousChineseEndRelationReadItem_ = NULL;

	SpecificationItem *simpleUserSpecificationItem_ = NULL;
	SpecificationItem *userSpecificationItem_ = NULL;

	WordItem *linkedGeneralizationWordItem_ = NULL;

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

	bool isValidContext( bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		WordItem *currentContextWordItem;

		if( relationContextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL &&
		relationWordItem != NULL &&
		!relationWordItem->hasContextInWord( relationContextNr, specificationWordItem ) &&
		( currentContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( !currentContextWordItem->isUserGeneralizationWord &&
				!currentContextWordItem->isUserRelationWord &&
				currentContextWordItem->firstAssignmentOrSpecificationItem( false, isPossessive, relationContextNr, specificationWordItem ) != NULL )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return true;
		}

	WordItem *contextUsedInUserSpecificationOfWordItem( bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		// Specification collection number may be undefined

		if( relationContextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL &&
		( currentGeneralizationItem = specificationWordItem->firstSpecificationGeneralizationItem( true ) ) != NULL )
			{
			// Do for all generalization specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL &&
				currentGeneralizationWordItem->firstUserSpecificationItem( false, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != NULL )
					return currentGeneralizationWordItem;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		return NULL;
		}

	unsigned int collectionNrByCompoundGeneralizationWordInCollectionWords( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		unsigned int collectionNr;
		WordItem *currentCollectionWordItem;

		if( collectionWordTypeNr > NO_WORD_TYPE_NR &&
		compoundGeneralizationWordItem != NULL &&
		( currentCollectionWordItem = globalVariables_->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( collectionNr = currentCollectionWordItem->collectionNrByCompoundGeneralizationWordInWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) ) > NO_COLLECTION_NR )
					return collectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	signed char addUserSpecificationWithRelation( bool isAction, bool isAssignedOrClear, bool isAssignment, bool isCharacteristicFor, bool isChineseReasoning, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpanishCurrentLanguage, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, unsigned short imperativeVerbParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, unsigned int nUserRelationWords, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isCheckingExistingUserSpecification;
		bool isChineseReversedImperativeNoun;
		bool isFirstComparisonPart = ( selectionListNr == ADMIN_CONDITION_LIST );
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		unsigned short relationWordTypeNr;
		ReadItem *currentRelationReadItem = startRelationReadItem;
		SpecificationItem *chineseUserSpecificationItem;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem;
		WordItem *mostRecentContextWord;
		WordItem *previousRelationWordItem = NULL;
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addUserSpecificationWithRelation";

		if( startRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given start relation read item is undefined" );

		if( endRelationReadItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given end relation read item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		isCheckingExistingUserSpecification = ( isPossessive &&
												nContextRelations == 0 &&
												!specificationWordItem->isNounWordSpanishAmbiguous() );

		do	{
			if( currentRelationReadItem->isPreposition() )
				prepositionParameter = currentRelationReadItem->wordParameter();
			else
				{
				if( currentRelationReadItem->isRelationWord() )
					{
					if( ( relationWordItem = currentRelationReadItem->readWordItem() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found an undefined read word" );

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

							if( nUserRelationWords == 1 &&
							( foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, ( questionParameter > NO_QUESTION_PARAMETER ), specificationWordItem ) ) != NULL &&
							foundSpecificationItem->hasRelationContext() &&
							( mostRecentContextWord = mostRecentContextWordInContextWords( foundSpecificationItem->relationContextNr() ) ) != NULL &&
							mostRecentContextWord != relationWordItem )
								previousRelationWordItem = mostRecentContextWord;

							if( previousRelationWordItem != NULL &&
							// Collect relation words
							collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );

							if( selectionListNr == NO_LIST_NR )
								{
								// Typical for Chinese
								if( isChineseReasoning )
									{
									// Make a Chinese exclusive specification substitution assumption (non-possessive)
									if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

									if( ( chineseUserSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
									!chineseUserSpecificationItem->isOlderItem() &&
									// Older possessive user specification doesn't exist
									relationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) == NULL )
										{
										// Draw Chinese reversible conclusions
										if( adminItem_->drawReversibleConclusions( false, generalizationWordItem ) != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw Chinese reversible conclusions in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

										if( chineseUserSpecificationItem->hasSpecificationCollection() &&
										// Make a Chinese exclusive specification substitution assumption (possessive)
										adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									{
									if( isCheckingExistingUserSpecification )
										existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, specificationWordItem, relationWordItem );

									if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, ( existingSpecificationItem == NULL ? NO_COLLECTION_NR : existingSpecificationItem->specificationCollectionNr() ), generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NULL, generalizationWordItem, specificationWordItem, relationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification having relation word \"", relationWordItem->anyWordTypeString(), "\"" );

									// Typical for Spanish
									if( isSpanishCurrentLanguage &&
									!isAssignment &&
									!isPossessive &&
									generalizationWordItem->firstAssignmentItem( false, false, relationContextNr, specificationWordItem ) == NULL &&

									// Draw proper noun part-of conclusions
									adminItem_->drawProperNounPartOfConclusions( false, isArchivedAssignment, NULL, specificationWordItem, relationWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
									}
								}
							else
								{
								// Typical for Chinese
								isChineseReversedImperativeNoun = ( isChineseCurrentLanguage_ &&
																	relationWordItem->isChineseReversedImperativeNoun() );

								if( adminItem_->createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, ( isFirstComparisonPart && !relationWordItem->isNumeralWordType() ), isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, ( isChineseReversedImperativeNoun ? relationWordTypeNr : specificationWordTypeNr ), ( isChineseReversedImperativeNoun ? specificationWordTypeNr : relationWordTypeNr ), generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, generalizationWordItem, ( isChineseReversedImperativeNoun ? relationWordItem : specificationWordItem ), ( isChineseReversedImperativeNoun ? specificationWordItem : relationWordItem ), NULL ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a selection part having relation word \"", relationWordItem->anyWordTypeString(), "\"" );

								isFirstComparisonPart = false;
								}

							previousRelationWordItem = relationWordItem;
							}
						}
					}
				}
			}
		while( !globalVariables_->hasDisplayedWarning &&
		currentRelationReadItem != endRelationReadItem &&
		( currentRelationReadItem = currentRelationReadItem->nextReadItem() ) != NULL );

		return RESULT_OK;
		}

	signed char collectGeneralizationWords( bool isExclusiveGeneralization, unsigned short generalizationWordTypeNr, unsigned short commonWordTypeNr, WordItem *previousGeneralizationWordItem, WordItem *newGeneralizationWordItem, WordItem *newCommonWordItem )
		{
		bool hasFoundCollection = false;
		unsigned int generalizationCollectionNr = NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectGeneralizationWords";

		if( previousGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous generalization word item is undefined" );

		if( newGeneralizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given new generalization word item is undefined" );

		if( previousGeneralizationWordItem == newGeneralizationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous and new generalization word items are the same word" );

		if( newCommonWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given new common word item is undefined" );

		if( ( generalizationCollectionNr = previousGeneralizationWordItem->collectionNr( newCommonWordItem ) ) == NO_COLLECTION_NR )
			generalizationCollectionNr = newGeneralizationWordItem->collectionNr( newCommonWordItem );

		if( generalizationCollectionNr > NO_COLLECTION_NR )
			{
			if( ( boolResult = newGeneralizationWordItem->findCollection( false, previousGeneralizationWordItem, newCommonWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" is collected with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			if( ( collectionResult = previousGeneralizationWordItem->createCollection( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, newGeneralizationWordItem, newCommonWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" with word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( generalizationCollectionNr == NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			if( newGeneralizationWordItem->createCollection( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", newGeneralizationWordItem->anyWordTypeString(), "\" with word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( previousGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in previous word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( newGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in new word \"", newGeneralizationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char collectRelationWords( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short commonWordTypeNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *commonWordItem )
		{
		bool hasFoundCollection = false;
		unsigned int collectionNr = NO_COLLECTION_NR;
		BoolResultType boolResult;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectRelationWords";

		if( previousRelationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous relation word item is undefined" );

		if( currentRelationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given current relation word item is undefined" );

		if( previousRelationWordItem == currentRelationWordItem )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given previous and current relation words are the same word" );

		if( ( collectionNr = previousRelationWordItem->collectionNr( commonWordItem ) ) == NO_COLLECTION_NR )
			collectionNr = currentRelationWordItem->collectionNr( commonWordItem );

		if( collectionNr > NO_COLLECTION_NR )
			{
			if( ( boolResult = previousRelationWordItem->findCollection( false, currentRelationWordItem, commonWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find out if word \"", previousRelationWordItem->anyWordTypeString(), "\" is collected with word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

			hasFoundCollection = boolResult.booleanValue;
			}

		if( !hasFoundCollection )
			{
			if( ( collectionResult = previousRelationWordItem->createCollection( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", previousRelationWordItem->anyWordTypeString(), "\" with word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

			if( collectionNr == NO_COLLECTION_NR )
				collectionNr = collectionResult.createdCollectionNr;

			if( currentRelationWordItem->createCollection( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", currentRelationWordItem->anyWordTypeString(), "\" with word \"", previousRelationWordItem->anyWordTypeString(), "\"" );
			}

		return RESULT_OK;
		}

	signed char collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		bool hasFoundCollection = false;
		bool isAllowingDifferentCommonWord;
		unsigned short collectionOrderNr = NO_ORDER_NR;
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
			if( ( specificationCollectionNr = previousSpecificationWordItem->collectionNr( generalizationWordItem ) ) == NO_COLLECTION_NR )
				{
				if( ( specificationCollectionNr = currentSpecificationWordItem->collectionNr( generalizationWordItem ) ) == NO_COLLECTION_NR &&

				( specificationWordTypeNr != WORD_TYPE_NOUN_PLURAL ||
				generalizationWordTypeNr != WORD_TYPE_NOUN_SINGULAR ) &&

				( specificationCollectionNr = previousSpecificationWordItem->collectionNr( specificationWordTypeNr ) ) == NO_COLLECTION_NR )
					specificationCollectionNr = currentSpecificationWordItem->collectionNr( specificationWordTypeNr );
				}
			else
				{
				// Collection already exists
				if( previousSpecificationWordItem->isOlderItem() &&
				currentSpecificationWordItem->isOlderItem() )
					{
					hasFoundCollection = true;
					specificationCollectionNr = NO_COLLECTION_NR;
					}
				}
			}
		else
			{
			if( !isExclusiveSpecification ||
			generalizationWordItem == previousSpecificationWordItem ||
			!generalizationWordItem->isNounWordSpanishAmbiguous() )
				specificationCollectionNr = collectionNrByCompoundGeneralizationWordInCollectionWords( isExclusiveSpecification, specificationWordTypeNr, compoundGeneralizationWordItem );
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
			// A collection comes in pairs
			if( collectionOrderNr >= MAX_ORDER_NR - 1 )
				return adminItem_->startSystemError( functionNameString, moduleNameString_, "Collection order number overflow" );

			if( ( collectionResult = previousSpecificationWordItem->createCollection( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", previousSpecificationWordItem->anyWordTypeString(), "\" with word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

			if( specificationCollectionNr == NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem->createCollection( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
			}

		if( specificationCollectionNr > NO_COLLECTION_NR &&
		generalizationWordItem->collectSpecificationsInWord( isExclusiveSpecification, isQuestion, specificationCollectionNr ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect specifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		return RESULT_OK;
		}

	signed char copyContext( unsigned int existingContextNr, unsigned int newSpanishAmbiguousCollectionNr, unsigned int newContextNr )
		{
		ContextItem *currentContextItem;
		WordItem *currentContextWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyContext";

		if( existingContextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( ( currentContextItem = currentContextWordItem->contextItem( existingContextNr ) ) != NULL &&
				currentContextWordItem->addContext( currentContextItem->contextWordTypeNr(), currentContextItem->specificationWordTypeNr(), newContextNr, newSpanishAmbiguousCollectionNr, currentContextItem->specificationWordItem() ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a copied context to word \"", currentContextWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char displaySpanishAssumptionsThatAreNotHiddenAnymore( bool isArchivedAssignment, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		SpecificationItem *hiddenSpanishSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displaySpanishAssumptionsThatAreNotHiddenAnymore";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( hiddenSpanishSpecificationItem = generalizationWordItem->firstUnhiddenSpanishSpecificationItem() ) != NULL &&

		( hiddenSpanishSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) != NULL ||
		hiddenSpanishSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ) != NULL ) )
			{
			hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = true;

			if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, true, hiddenSpanishSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"", generalizationWordItem->anyWordTypeString(), "\" that is not hidden anymore" );

			if( !userSpecificationItem_->isPossessive() &&
			// Draw proper noun part-of conclusions
			adminItem_->drawProperNounPartOfConclusions( false, isArchivedAssignment, NULL, specificationWordItem, generalizationWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
			}

		return RESULT_OK;
		}

	signed char displaySpanishAssumptionsThatAreNotHiddenAnymore( bool hasRelationContext, bool isAssignment, bool isArchivedAssignment, WordItem *specificationWordItem )
		{
		bool isSpecificationWordSpanishAmbiguous;
		SpecificationItem *hiddenSpanishSpecificationItem;
		WordItem *currentContextWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displaySpanishAssumptionsThatAreNotHiddenAnymore";

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			// Do for all context words
			do	{
				if( ( hiddenSpanishSpecificationItem = currentContextWordItem->firstUnhiddenSpanishSpecificationItem() ) != NULL )
					{
					if( !isAssignment )
						{
						hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = true;

						if( !hiddenSpanishSpecificationItem->hasAssumptionLevel() &&
						hiddenSpanishSpecificationItem->isSelfGeneratedAssumption() &&
						// Assumption level of hidden Spanish specification isn't calculated. So, calculate now.
						// Test Spanish test file: "Complejo (14)"
						hiddenSpanishSpecificationItem->calculateAssumptionLevel() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to calculate an unhidden Spanish assumption level" );

						if( currentContextWordItem->writeUpdatedSpecification( true, false, false, false, true, hiddenSpanishSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a Spanish assumption in word \"", currentContextWordItem->anyWordTypeString(), "\" that is not hidden anymore" );

						if( ( hasRelationContext ||
						isSpecificationWordSpanishAmbiguous ) &&

						// Draw Spanish reversible conclusions
						adminItem_->drawReversibleConclusions( false, currentContextWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions in word \"", currentContextWordItem->anyWordTypeString(), "\"" );
						}

					// Draw proper noun part-of conclusions
					if( adminItem_->drawProperNounPartOfConclusions( true, isArchivedAssignment, NULL, specificationWordItem, currentContextWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char processJustification( bool hasFoundRelationContext, bool isOlderFoundSpecification, bool isUserGeneralizationWord, unsigned short justificationTypeNr, JustificationItem *createdOrFoundJustificationItem, SpecificationItem *foundSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool hasRelationWordConfirmedSpecification = false;
		bool isAttachingJustification = true;
		bool isRemovingPreviousJustifications = false;
		bool isSpecificationWordSpanishAmbiguous;
		JustificationItem *existingJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "processJustification";

		if( createdOrFoundJustificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given created or found justification item is undefined" );

		if( foundSpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( secondarySpecificationItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given secondary specification item is undefined" );

		isSpecificationWordSpanishAmbiguous = foundSpecificationItem->isSpecificationWordSpanishAmbiguous();

		if( relationWordItem != NULL )
			hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();

		switch( justificationTypeNr )
			{
			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				if( relationWordItem != NULL )
					{
					if( secondarySpecificationItem->isSelfGeneratedAssumption() )
						{
						// Skip if justification with self-generated assumption is created,
						// while justification with self-generated conclusion already exists
						if( relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPossessive(), false, secondarySpecificationItem->specificationWordItem(), generalizationWordItem ) != NULL )
							isAttachingJustification = false;
						}
					else
						{
						if( isUserGeneralizationWord &&
						secondarySpecificationItem->isSelfGeneratedConclusion() &&
						( secondarySpecificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
						( assumptionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPossessive(), true, secondarySpecificationWordItem, generalizationWordItem ) ) != NULL &&
						( existingJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem ) ) != NULL )
							{
							if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an existing justification item in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

							isAttachingJustification = false;
							}
						}
					}

				break;

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( hasFoundRelationContext &&
				isOlderFoundSpecification &&
				isUserGeneralizationWord &&
				primarySpecificationItem != NULL &&
				// Confirmation
				secondarySpecificationItem->isUserSpecification() &&
				( existingJustificationItem = generalizationWordItem->olderComplexJustificationItem( true, secondarySpecificationItem->isPossessive(), justificationTypeNr, secondarySpecificationItem->specificationCollectionNr(), primarySpecificationItem ) ) != NULL )
					{
					// Replace existing justification by the created justification
					if( generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an existing justification by the created justification item in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					isAttachingJustification = false;
					}

				break;

			case JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION:
				if( !hasRelationWordConfirmedSpecification &&
				isOlderFoundSpecification &&
				!isUserGeneralizationWord &&
				foundSpecificationItem->isSelfGeneratedConclusion() )
					isAttachingJustification = false;

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				if( primarySpecificationItem != NULL &&
				( existingJustificationItem = foundSpecificationItem->differentAssumptionLevelSpecificationSubstitutionJustificationItem( primarySpecificationItem, secondarySpecificationItem ) ) != NULL )
					{
					isAttachingJustification = false;

					if( primarySpecificationItem->assumptionLevel() < existingJustificationItem->primarySpecificationAssumptionLevel() &&
					// Replace less certain existing justification by the created justification
					generalizationWordItem->replaceJustification( existingJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace an older generalization assumption by the created justification item in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION:
				if( isOlderFoundSpecification &&
				primarySpecificationItem != NULL &&
				!primarySpecificationItem->isPossessive() )
					{
					if( ( existingJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) != NULL &&

					( foundSpecificationItem->isConcludedAssumption() ||
					existingJustificationItem->primarySpecificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() ) )
						isRemovingPreviousJustifications = true;
					}

				break;

			case JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION:
				// Typical for Spanish
				if( isSpecificationWordSpanishAmbiguous )
					{
					if( foundSpecificationItem->secondarySpecificationJustificationItem( secondarySpecificationItem ) == NULL )
						{
						if( !hasRelationWordConfirmedSpecification )
							{
							if( secondarySpecificationItem->isUserAssignment() )
								{
								if( ( obsoleteJustificationItem = foundSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) != NULL &&
								obsoleteJustificationItem->secondaryGeneralizationWordItem() == relationWordItem )
									{
									if( generalizationWordItem->replaceJustification( obsoleteJustificationItem, createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to replace a reversible justification item without relation context in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									isAttachingJustification = false;
									}
								}
							else
								{
								if( ( obsoleteJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) ) != NULL &&
								obsoleteJustificationItem->secondaryGeneralizationWordItem() == relationWordItem )
									isAttachingJustification = false;
								}
							}
						}
					else
						isAttachingJustification = false;
					}
				else
					{
					if( foundSpecificationItem->hasOnlyOneRelationWord() )
						isRemovingPreviousJustifications = true;
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				// Adjusted question
				if( isOlderFoundSpecification &&
				adminItem_->adjustedQuestionWordItem() == generalizationWordItem )
					{
					// Remove obsolete justifications from adjusted compound question
					isRemovingPreviousJustifications = true;
					// Adjusted question needs to be rewritten
					foundSpecificationItem->clearStoredSentenceString();
					}
			}

		if( isRemovingPreviousJustifications )
			{
			if( generalizationWordItem->copyAndReplaceSpecificationItem( foundSpecificationItem->isAnsweredQuestion(), foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->isExclusiveSpecification(), foundSpecificationItem->generalizationCollectionNr(), foundSpecificationItem->specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy and replace a specification with a different first justification item" );
			}
		else
			{
			if( isAttachingJustification &&
			// Attach created justification to the found specification
			foundSpecificationItem->updatedSpecificationItem()->attachJustificationToSpecificationAdvanced( createdOrFoundJustificationItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to attach a justification to the found specification" );
			}

		return RESULT_OK;
		}

	signed char recalculateAssumptionsInTouchedWords()
		{
		WordItem *currentTouchedWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptionsInTouchedWords";

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				if( currentTouchedWordItem->recalculateAssumptionsInWord() != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumptions of word \"", currentTouchedWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentTouchedWordItem = currentTouchedWordItem->nextTouchedWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	WordItem *mostRecentContextWordInContextWords( unsigned int contextNr )
		{
		WordItem *currentContextWordItem;
		WordItem *mostRecentWordItem = NULL;

		if( contextNr > NO_CONTEXT_NR &&
		( currentContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( currentContextWordItem->contextItem( contextNr ) != NULL )
					mostRecentWordItem = currentContextWordItem;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return mostRecentWordItem;
		}

	ContextResultType getRelationContext( bool hasRelationWordConfirmedSpecification, bool isAssignment, bool isArchivedAssignment, bool isPossessive, bool isSelfGeneratedAssumption, bool isSpecificationWordSpanishAmbiguous, unsigned int specificationCollectionNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasExistingSpecificationCompoundCollection = false;
		bool hasExistingSpecificationCurrentCreationSentenceNr = false;
		bool hasExistingSpecificationOnlyOneRelationWord = false;
		bool hasGeneralizationWordAnsweredSelfGeneratedQuestion;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasNonCompoundUserSpecificationCollection = false;
		bool hasRelationWordExistingRelationContext;
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool hasUserSpecificationCompoundCollection = false;
		bool isCompoundCollectionSpanishAmbiguous = false;
		bool isExistingArchivedAssignment = false;
		bool isExistingHiddenSpanishSpecification = false;
		bool isExistingOlderSpecification = false;
		bool isExistingSpecificationConcludedAssumption = false;
		bool isExistingSpecificationSelfGeneratedAssumption = false;
		bool isExistingSpecificationSelfGeneratedConclusion;
		bool isExistingUserSpecification = false;
		bool isGeneralizationWordUserRelationWord;
		bool isUserArchivedAssignment = false;
		bool isUserAssignment = false;
		unsigned int existingRelationContextNr = NO_CONTEXT_NR;
		unsigned int existingSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int foundContextNr;
		unsigned int foundRelationContextNr = NO_CONTEXT_NR;
		unsigned int nonCompoundSpanishSpecificationCollectionNr;
		unsigned int nUserRelationWords = globalVariables_->nUserRelationWords;
		unsigned int spanishAmbiguousCopyCollectionNr = NO_COLLECTION_NR;
		unsigned int userSpecificationCollectionNr = NO_COLLECTION_NR;
		ContextItem *foundContextItem = NULL;
		SpecificationItem *existingSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem;
		WordItem *foundWordItem;
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getRelationContext";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( isSpecificationWordSpanishAmbiguous &&
		isPossessive &&
		specificationCollectionNr > NO_COLLECTION_NR &&
		specificationWordItem->isCompoundCollection( specificationCollectionNr ) )
			{
			isCompoundCollectionSpanishAmbiguous = true;
			existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, false, isPossessive, false, specificationCollectionNr, NO_CONTEXT_NR, specificationWordItem );
			}
		else
			existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, isPossessive, specificationCollectionNr, specificationWordItem );

		hasGeneralizationWordAnsweredSelfGeneratedQuestion = generalizationWordItem->hasCurrentlyAnsweredSelfGeneratedQuestion();
		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();

		if( isSpecificationWordSpanishAmbiguous &&
		existingSpecificationItem == NULL )
			existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, false, isPossessive, false, specificationWordItem );

		if( existingSpecificationItem != NULL )
			{
			hasExistingSpecificationCompoundCollection = existingSpecificationItem->hasCompoundSpecificationCollection();
			hasExistingSpecificationCurrentCreationSentenceNr = existingSpecificationItem->hasCurrentCreationSentenceNr();
			hasExistingSpecificationOnlyOneRelationWord = existingSpecificationItem->hasOnlyOneRelationWord();
			isExistingArchivedAssignment = existingSpecificationItem->isArchivedAssignment();
			isExistingHiddenSpanishSpecification = existingSpecificationItem->isHiddenSpanishSpecification();
			isExistingOlderSpecification = existingSpecificationItem->isOlderItem();
			isExistingSpecificationConcludedAssumption = existingSpecificationItem->isConcludedAssumption();
			isExistingSpecificationSelfGeneratedAssumption = existingSpecificationItem->isSelfGeneratedAssumption();
			isExistingUserSpecification = existingSpecificationItem->isUserSpecification();
			existingSpecificationCollectionNr = existingSpecificationItem->specificationCollectionNr();
			existingRelationContextNr = existingSpecificationItem->relationContextNr();
			}

		if( userSpecificationItem != NULL )
			{
			if( userSpecificationItem->hasNonCompoundSpecificationCollection() )
				hasNonCompoundUserSpecificationCollection = true;
			else
				{
				if( userSpecificationItem->hasCompoundSpecificationCollection() )
					hasUserSpecificationCompoundCollection = true;
				}

			if( userSpecificationItem->isAssignment() )
				{
				isUserAssignment = true;

				if( userSpecificationItem->isArchivedAssignment() )
					isUserArchivedAssignment = true;
				}

			userSpecificationCollectionNr = userSpecificationItem->specificationCollectionNr();
			}

		// Try to find relation context with same number of relation words as in the user sentence
		if( nUserRelationWords > 0 )
			foundContextItem = relationWordItem->contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : NO_COLLECTION_NR ), specificationWordItem );

		if( foundContextItem == NULL )
			{
			isGeneralizationWordUserRelationWord = generalizationWordItem->isUserRelationWord;

			if( existingSpecificationItem == NULL )
				{
				if( ( foundContextNr = relationWordItem->contextNr( ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : NO_COLLECTION_NR ), specificationWordItem ) ) > NO_CONTEXT_NR &&

				// Typical for Chinese
				( generalizationWordItem->isFemale() ||

				( globalVariables_->nUserGeneralizationWords > 1 &&

				( generalizationWordItem->hasFeminineOrMasculineProperNounEnding() ||
				contextUsedInUserSpecificationOfWordItem( isPossessive, specificationCollectionNr, foundContextNr, specificationWordItem ) == NULL ) ) ||

				// Generalization word is user relation word
				( isGeneralizationWordUserRelationWord &&

				( isUserAssignment ||

				( hasNonCompoundUserSpecificationCollection &&
				isPossessive ) ) &&

				adminItem_->nContextWords( foundContextNr, specificationWordItem ) == 1 ) ) )
					contextResult.contextNr = foundContextNr;
				}
			else
				{
				// Find specification with found context word as relation word
				if( isSpecificationWordSpanishAmbiguous )
					{
					if( ( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, false, isPossessive, specificationWordItem ) ) != NULL )
						{
						if( existingRelationContextNr > NO_CONTEXT_NR &&

						( isExistingHiddenSpanishSpecification ||
						!foundSpecificationItem->hasRelationContext() ) )
							foundRelationContextNr = existingRelationContextNr;
						else
							{
							if( !hasSpecificationCollection ||
							foundSpecificationItem->isUserSpecification() ||
							foundSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
								foundRelationContextNr = foundSpecificationItem->relationContextNr();
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
					if( existingRelationContextNr > NO_CONTEXT_NR &&
					isExistingArchivedAssignment == isArchivedAssignment )
						foundRelationContextNr = existingRelationContextNr;
					}

				if( foundRelationContextNr == NO_CONTEXT_NR &&
				contextResult.contextNr == NO_CONTEXT_NR )
					{
					if( isSpecificationWordSpanishAmbiguous )
						{
						// Typical for Spanish
						if( existingRelationContextNr == NO_CONTEXT_NR )
							{
							if( ( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, isArchivedAssignment, false, isPossessive, specificationWordItem, relationWordItem ) ) != NULL )
								contextResult.contextNr = foundSpecificationItem->relationContextNr();
							}
						else
							{
							foundContextNr = relationWordItem->contextNr( ( isExistingOlderSpecification ? NO_COLLECTION_NR : specificationCollectionNr ), specificationWordItem );

							// Typical for Spanish
							if( ( !hasExistingSpecificationCurrentCreationSentenceNr &&
							isExistingArchivedAssignment == isUserArchivedAssignment ) ||

							( !isExistingOlderSpecification &&
							foundContextNr > NO_CONTEXT_NR ) ||

							( isExistingSpecificationSelfGeneratedAssumption &&
							!isUserAssignment ) )
								contextResult.contextNr = ( foundContextNr > NO_CONTEXT_NR &&

															( !isExistingOlderSpecification ||
															existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ) == NULL ) ? foundContextNr : existingRelationContextNr );
							}
						}
					}
				else
					{
					if( contextResult.contextNr == NO_CONTEXT_NR )
						{
						isExistingSpecificationSelfGeneratedConclusion = existingSpecificationItem->isSelfGeneratedConclusion();

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
								hasRelationWordExistingRelationContext = relationWordItem->hasContextInWord( existingRelationContextNr, specificationWordItem );

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
											( foundWordItem = contextUsedInUserSpecificationOfWordItem( isPossessive, existingSpecificationCollectionNr, existingRelationContextNr, specificationWordItem ) ) != NULL &&
											( foundSpecificationItem = foundWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, true, NO_QUESTION_PARAMETER, userSpecificationCollectionNr, NO_CONTEXT_NR, specificationWordItem ) ) != NULL )
												{
												if( hasSpecificationCollection )
													// Need to copy context (User)
													contextResult.copiedRelationContextNr = existingRelationContextNr;
												else
													contextResult.contextNr = foundSpecificationItem->relationContextNr();
												}
											else
												{
												// Typical for Spanish
												if( isPossessive &&
												!isExistingSpecificationSelfGeneratedAssumption &&
												!isGeneralizationWordUserRelationWord &&
												!isSelfGeneratedAssumption &&
												( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&
												( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, true, nonCompoundSpanishSpecificationCollectionNr, specificationWordItem ) ) != NULL )
													// Need to copy context (4)
													contextResult.copiedRelationContextNr = foundSpecificationItem->relationContextNr();
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
											if( existingSpecificationItem->specificationWordItem() == specificationWordItem )
												contextResult.contextNr = foundRelationContextNr;
											}
										else
											{
											if( isExistingSpecificationConcludedAssumption )
												contextResult.contextNr = relationWordItem->contextNr( specificationWordItem );
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
									if( relationWordItem->addContext( existingSpecificationItem->relationWordTypeNr(), existingSpecificationItem->specificationWordTypeNr(), existingRelationContextNr, NO_COLLECTION_NR, specificationWordItem ) != RESULT_OK )
										return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to add a Spanish relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );
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
				foundContextNr = foundContextItem->contextNr();

				if( isAssignment &&
				!isArchivedAssignment )
					{
					// Dynamic semantic ambiguity
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, relationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_DYNAMIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
						return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to write the 'dynamic semantic ambiguity' interface notification" );

					contextResult.isAmbiguousRelationContext = true;
					}
				else
					{
					if( existingRelationContextNr == NO_CONTEXT_NR )
						{
						if( ( hasSpecificationCollection &&

						( isSelfGeneratedAssumption ||
						// Typical for Spanish
						isCompoundCollectionSpanishAmbiguous ) ) ||

						// Collected afterwards
						( userSpecificationItem != NULL &&
						userSpecificationItem->isOlderItem() ) )
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
						relationWordItem->hasContextInWord( existingRelationContextNr, specificationWordItem ) ) ) ) ) ||

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

		if( contextResult.contextNr == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;

			if( contextResult.copiedRelationContextNr > NO_CONTEXT_NR &&
			copyContext( contextResult.copiedRelationContextNr, spanishAmbiguousCopyCollectionNr, contextResult.contextNr ) != RESULT_OK )
				return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to copy a context" );
			}

		return contextResult;
		}

	CreateAndAssignResultType addSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		bool hasCurrentlyMoreSpecificNonExclusiveSpecification;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasRelationWordConfirmedSpecification;
		bool isAmbiguousRelationContext = false;
		bool isAssignedOrClear;
		bool isDefinitionSpecification;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationContextAlreadyDefined = false;
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		bool isSelfGeneratedAssumption;
		bool isSpanishCurrentLanguage = false;
		bool isSpecificationWordSpanishAmbiguous = false;
		unsigned int copiedRelationContextNr = NO_CONTEXT_NR;
		unsigned int existingRelationContextNr = NO_CONTEXT_NR;
		SpecificationItem *createdAssignmentItem = NULL;
		SpecificationItem *createdSpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *replacedAssignmentItem;
		SpecificationItem *tempSpecificationItem;
		char *writtenSentenceString;
		ContextResultType contextResult;
		CreateAndAssignResultType createAndAssignCheckResult;
		CreateAndAssignResultType createAndAssignReturnResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem == adminItem_ )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word is the administrator" );

		if( specificationWordItem == adminItem_ )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given specification word is the administrator" );

		if( ( existingSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( true, true, isNegative, isPossessive, false, specificationWordItem ) ) != NULL )
			{
			existingRelationContextNr = existingSpecificationItem->relationContextNr();

			// Typical for Chinese
			if( isChineseCurrentLanguage_ &&
			!isAssignment &&
			isPossessive &&
			relationWordItem != NULL &&
			existingSpecificationItem->isOlderItem() &&
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
			hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();
			isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();

			if( isSpanishCurrentLanguage )
				isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			if( existingRelationContextNr > NO_CONTEXT_NR &&
			existingSpecificationItem != NULL &&

			( existingRelationContextNr == relationContextNr ||

			// Confirmation
			( !isSpecificationWordSpanishAmbiguous &&
			hasRelationWordConfirmedSpecification &&
			existingSpecificationItem->hasOnlyOneRelationWord() &&
			relationWordItem->hasContextInWord( existingRelationContextNr, specificationWordItem ) ) ||

			// Existing specification is current
			( ( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||

			( !isSpanishCurrentLanguage &&
			isSelfGenerated ) ) &&

			!existingSpecificationItem->isOlderItem() ) ||

			// User specification
			( userSpecificationItem_ != NULL &&

			( ( !isSelfGenerated &&
			!isQuestion ) ||

			( userSpecificationItem_->isOlderItem() &&
			existingSpecificationItem->specificationCollectionNr() == specificationCollectionNr ) ) ) ) &&

			isValidContext( isPossessive, existingRelationContextNr, specificationWordItem, relationWordItem ) )
				{
				isRelationContextAlreadyDefined = true;
				relationContextNr = existingRelationContextNr;
				}
			else
				{
				if( relationContextNr == NO_CONTEXT_NR ||

				// Assignment
				( isAssignment &&
				userSpecificationItem_ != NULL &&
				!userSpecificationItem_->isOlderItem() ) ||

				// No assignment
				( !isAssignment &&
				isPossessive &&
				specificationCollectionNr == NO_COLLECTION_NR &&
				// Typical for Spanish
				!isSpecificationWordSpanishAmbiguous ) ||

				// Current relation context is invalid
				!isValidContext( isPossessive, relationContextNr, specificationWordItem, relationWordItem ) )
					{
					isSelfGeneratedAssumption = ( firstJustificationItem != NULL && firstJustificationItem->isAssumptionJustification() );

					if( ( contextResult = getRelationContext( hasRelationWordConfirmedSpecification, isAssignment, isArchivedAssignment, isPossessive, isSelfGeneratedAssumption, isSpecificationWordSpanishAmbiguous, specificationCollectionNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to get the relation context" );

					isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
					copiedRelationContextNr = contextResult.copiedRelationContextNr;
					relationContextNr = contextResult.contextNr;

					if( !isValidContext( isPossessive, relationContextNr, specificationWordItem, relationWordItem ) )
						return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I found an invalid context in word \"", relationWordItem->anyWordTypeString(), "\"" );
					}
				else
					isRelationContextAlreadyDefined = true;
				}

			if( isSpecificationWordSpanishAmbiguous &&
			specificationCollectionNr > NO_COLLECTION_NR &&
			!relationWordItem->hasContextInWord( relationContextNr, specificationWordItem ) &&
			// Check for the opposite collection context
			adminItem_->hasFoundContextInAllWords( relationContextNr, ( specificationWordItem->isCompoundCollection( specificationCollectionNr ) ? specificationWordItem->nonCompoundCollectionNr() : specificationWordItem->compoundCollectionNr() ), specificationWordItem ) )
				return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I found a Spanish ambiguous compound collection conflict in word \"", relationWordItem->anyWordTypeString(), "\"" );

			if( relationWordItem->addContext( relationWordTypeNr, specificationWordTypeNr, relationContextNr, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : NO_COLLECTION_NR ), specificationWordItem ) != RESULT_OK )
				return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&
			isSelfGenerated &&
			specificationWordItem->isUserSpecificationWord &&
			userSpecificationItem_->hasRelationContext() &&
			// Display Spanish assumptions that are not hidden anymore
			displaySpanishAssumptionsThatAreNotHiddenAnymore( isArchivedAssignment, generalizationWordItem, specificationWordItem ) != RESULT_OK )
				return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to display Spanish assumptions that are not hidden anymore in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
			}

		// Skip adding specification if already exists
		if( isAssignment ||
		!isRelationContextAlreadyDefined ||
		existingSpecificationItem == NULL ||
		!existingSpecificationItem->isActiveAssignment() )
			{
			// Check for an assignment or a specification with opposite negative indicator
			if( !isAssignment &&
			isNegative &&
			isPossessive &&
			isArchivedAssignment_ )
				isAssignment = true;

			if( adminItem_->isSystemStartingUp() &&
			isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
				{
				if( ( createAndAssignReturnResult = adminItem_->addSpecificationWithAuthorization( isAssignment, isCharacteristicFor, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString ) ).result != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification to word \"", generalizationWordItem->anyWordTypeString(), "\" with authorization" );
				}
			else
				{
				if( ( createAndAssignReturnResult = generalizationWordItem->addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, userAssumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, NULL ) ).result != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}

			if( !globalVariables_->hasDisplayedWarning )
				{
				isArchivedAssignment_ = isArchivedAssignment;
				foundSpecificationItem = createAndAssignReturnResult.foundSpecificationItem;
				replacedAssignmentItem = createAndAssignReturnResult.replacedAssignmentItem;

				isDefinitionSpecification = ( !hasRelationWord &&
											!isQuestion &&
											adminItem_->isNounWordType( generalizationWordTypeNr ) &&
											adminItem_->isNounWordType( specificationWordTypeNr ) );

				if( ( createdSpecificationItem = createAndAssignReturnResult.createdSpecificationItem ) == NULL )
					{
					if( hasRelationWord &&
					!isSelfGenerated &&
					foundSpecificationItem != NULL )
						{
						userSpecificationItem_ = foundSpecificationItem;

						if( !isQuestion &&
						foundSpecificationItem->hasRelationContext() )
							simpleUserSpecificationItem_ = generalizationWordItem->firstSpecificationItem( isPossessive, false, false, specificationWordItem );
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
								adminItem_->makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make an only-option-left assumption" );
								}
							else
								{
								if( !isPossessive )
									{
									if( isAssignment &&
									hasRelationWord &&
									// Collect generalization words of a proper noun specification
									adminItem_->collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

									if( adminItem_->makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, createdSpecificationItem ) != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make an indirectly answered question assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									hasCurrentlyMoreSpecificNonExclusiveSpecification = generalizationWordItem->hasCurrentlyMoreSpecificNonExclusiveSpecification();

									if( generalizationWordItem->hasFoundAnswerToQuestion() &&

									( hasCurrentlyMoreSpecificNonExclusiveSpecification ||
									!specificationWordItem->isCompoundCollection( createdSpecificationItem->specificationCollectionNr() ) ) &&

									adminItem_->drawNegativeConclusionsFromAnsweredQuestions( hasCurrentlyMoreSpecificNonExclusiveSpecification, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw negative conclusions from answered questions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, false, true, false, false, isCharacteristicFor, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION, JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
								}
							else
								{
								if( createdSpecificationItem == NULL )
									{
									if( !isConditional &&
									!isNegative &&
									foundSpecificationItem != NULL &&
									!foundSpecificationItem->isExclusiveSpecification() &&

									// Check if primary specification already has an assignment
									( ( foundAssignmentItem = generalizationWordItem->firstAssignmentItem( isPossessive, isQuestion, relationContextNr, specificationWordItem ) ) != NULL ||
									isAssignment ) )
										{
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
								else
									{
									if( !isAssignment &&
									isPossessive &&
									generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&

									// Draw a specification-generalization 'part of' conclusion from a possessive definition sentence
									// See Block 2 of the challenge document, or http://mafait.org/block2/
									addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isSpecific, isEveryGeneralization, isExclusiveGeneralization, false, false, isNegative, !isSpecific, false, false, false, false, true, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION, JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated reversible 'part of' specification to generalization word \"", generalizationWordItem->anyWordTypeString(), "\" to specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
									}
								}
							}
						}
					}

				if( !globalVariables_->hasDisplayedWarning &&

				( ( isAssignment &&
				// At this stage, selections must be stored - rather than executed. So, don't assign them
				!isSelection ) ||

				replacedAssignmentItem != NULL ) &&

				// Skip for example imperative verbs
				adminItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
					{
					isAssignedOrClear = ( specificationWordItem != NULL &&
										specificationWordItem->isAdjectiveAssignedOrEmpty() );

					if( ( createAndAssignCheckResult = assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to assign the specification" );

					if( ( createdAssignmentItem = createAndAssignCheckResult.createdSpecificationItem ) != NULL )
						{
						if( !isArchivedAssignment_ ||
						createAndAssignReturnResult.createdSpecificationItem != NULL )
							createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;

						if( replacedAssignmentItem != NULL &&
						generalizationWordItem->replaceOrDeleteSpecification( replacedAssignmentItem, createdAssignmentItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to replace or delete an assignment in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( isNegative &&
						generalizationWordItem->isUserGeneralizationWord &&
						createdAssignmentItem->hasSpecificationCollection() &&

						// Draw only option left conclusion
						adminItem_->drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem->specificationCollectionNr(), generalizationContextNr, generalizationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw an only-option-left conclusion by negative assignment" );
						}

					if( !isSelfGenerated &&
					( tempSpecificationItem = ( createdAssignmentItem == NULL ? createAndAssignCheckResult.foundSpecificationItem : createdAssignmentItem ) ) != NULL )
						{
						userSpecificationItem_ = tempSpecificationItem;

						if( isArchivedAssignment_ &&
						!isQuestion )
							simpleUserSpecificationItem_ = tempSpecificationItem;
						}
					}

				if( !globalVariables_->hasDisplayedWarning &&
				!isExclusiveSpecification &&
				!isSpecificationGeneralization &&
				specificationWordItem != NULL &&
				userSpecificationItem_ != NULL )
					{
					if( hasRelationWord &&
					isSelfGenerated &&

					// Avoid looping of reversible conclusions
					( createdSpecificationItem != NULL ||

					( !isPossessive &&

					( !specificationWordItem->isUserSpecificationWord ||
					isSpanishCurrentLanguage ) ) ) &&

					// Draw reversible conclusions
					adminItem_->drawReversibleConclusions( ( createdSpecificationItem != NULL ), generalizationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					if( replacedAssignmentItem == NULL &&

					( !isSelfGenerated ||

					( isDefinitionSpecification &&
					isEveryGeneralization &&
					!isPossessive &&
					!userSpecificationItem_->hasSpecificationCollection() ) ) )
						{
						if( isQuestion )
							{
							if( hasRelationWord &&

							( createdAssignmentItem != NULL ||

							( createdSpecificationItem != NULL &&
							!generalizationWordItem->hasCurrentlyMoreSpecificQuestion() ) ) &&

							adminItem_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, ( createdAssignmentItem == NULL ? createdSpecificationItem : createdAssignmentItem ), generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
								return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make a suggestive question assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
							}
						else
							{
							if( isDefinitionSpecification &&
							!isPossessive &&
							adminItem_->drawSpecificationGeneralizationConclusion( isArchivedAssignment, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a specification-generalization conclusion" );
							}
						}
					}
				}
			}

		return createAndAssignReturnResult;
		}


	protected:
	// Constructor

	AdminSpecification( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

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

	bool hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore()
		{
		return hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_;
		}

	signed char assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignSelectionSpecification";

		if( assignmentSelectionItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given assignment selection item is undefined" );

		if( assignSpecification( false, assignmentSelectionItem->isAssignedOrEmpty(), assignmentSelectionItem->isInactiveAssignment(), assignmentSelectionItem->isArchivedAssignment(), assignmentSelectionItem->isNegative(), assignmentSelectionItem->isPossessive(), assignmentSelectionItem->isSpecificationGeneralization(), assignmentSelectionItem->isUniqueUserRelation(), assignmentSelectionItem->assumptionLevel(), assignmentSelectionItem->prepositionParameter(), NO_QUESTION_PARAMETER, assignmentSelectionItem->relationWordTypeNr(), assignmentSelectionItem->generalizationContextNr(), assignmentSelectionItem->specificationContextNr(), assignmentSelectionItem->relationContextNr(), assignmentSelectionItem->nContextRelations(), NULL, assignmentSelectionItem->generalizationWordItem(), assignmentSelectionItem->specificationWordItem(), assignmentSelectionItem->specificationString() ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a specification" );

		return RESULT_OK;
		}

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short userAssumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( adminItem_->isSystemStartingUp() &&
		isAuthorizationRequiredForChanges( generalizationWordItem, specificationWordItem ) )
			return adminItem_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

		return generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, NULL );
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
		hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore_ = false;
		isArchivedAssignment_ = false;
		isChineseCurrentLanguage_ = false;

		doneSpecificationWordOrderNr_ = NO_ORDER_NR;
		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordTypeNr_ = NO_WORD_TYPE_NR;

		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		nPreviousChineseUserRelationWords_ = 0;
		userRelationContextNr_ = NO_CONTEXT_NR;

		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		previousChineseStartRelationReadItem_ = NULL;
//		previousChineseEndRelationReadItem_ = NULL;

		simpleUserSpecificationItem_ = NULL;
		userSpecificationItem_ = NULL;

		// Don't initialize here. It is initialized by function initializeLinkedWord()
//		linkedGeneralizationWordItem_ = NULL;
		}

	signed char addUserSpecification( bool isAction, bool isAssignment, bool isCharacteristicFor, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short userAssumptionLevel, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationReadItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		bool hasAddedSpecification = false;
		bool hasAlreadyAddedChineseRelationWords = false;
		bool hasLinkedPossessiveDeterminer = false;
		bool hasRelationWord = ( startRelationReadItem != NULL );
		bool isAmbiguousRelationContext = false;
		bool isChineseCurrentLanguage = adminItem_->isChineseCurrentLanguage();
		bool isChineseReasoning = false;
		bool isCurrentNounValue;
		bool isGeneralizationProperNoun;
		bool isGeneralizationReasoningWordType;
		bool isNonChineseNumeral;
		bool isPreposition = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationWord;
		bool isSelection = ( selectionListNr != NO_LIST_NR );
		bool isSkippingThisGeneralizationPart = false;
		bool isSpanishCurrentLanguage = adminItem_->isSpanishCurrentLanguage();
		bool isSpecificationWord;
		bool isSpecificationWordAlreadyAssignedByComparison = false;
		bool isText;
		bool isValueSpecificationWord = false;
		bool isWaitingForRelation = false;
		bool isWaitingForText = false;
		unsigned short currentSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short generalizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short linkedSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short previousSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short valueGeneralizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short wordOrderNr;
		unsigned int nContextRelations = 0;
		unsigned int nUserRelationWords = globalVariables_->nUserRelationWords;
		ReadItem *currentReadItem;
		ReadItem *firstRelationReadItem;
		ReadItem *lastGeneralizationReadItem;
		SpecificationItem *existingUserSpecificationItem;
		SpecificationItem *tempSpanishSpecificationItem;
		SpecificationItem *tempUserSpecificationItem;
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

			if( ( contextResult = getRelationContext( true, false, false, false, true, generalizationWordItem, linkedSpecificationWordItem, NULL, startRelationReadItem ) ).result != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to get a Chinese relation context" );

			isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
			userRelationContextNr_ = contextResult.contextNr;

			// Remember variables below
			nPreviousChineseUserRelationWords_ = nUserRelationWords;
			previousChineseStartRelationReadItem_ = startRelationReadItem;
			previousChineseEndRelationReadItem_ = endRelationReadItem;

			if( addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, NO_SELECTION_LEVEL, NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, linkedSpecificationWordItem ) != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a Chinese user specification with a relation to word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			if( linkedSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
			// Make a specification substitution 'part of' assumption
			adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, generalizationWordItem, linkedSpecificationWordItem ) != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

			// Draw a specification substitution conclusion or ask a question
			if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, linkedSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, linkedSpecificationWordItem, NULL ) != RESULT_OK )
				return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a Chinese specification substitution conclusion or ask a question for word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
														currentReadItem->isNoun() ? WORD_TYPE_NOUN_SINGULAR : currentReadItem->wordTypeNr() );
					}

				if( isNonChineseNumeral &&
				isPossessive &&
				!isRelationWord &&
				( readWordTypeString = currentReadItem->readWordTypeString() ) != NULL )
					sscanf( readWordTypeString, "%u", &nContextRelations );
				else
					{
					if( !isSelection &&
					isSpecificationWord &&
					!hasAlreadyAddedChineseRelationWords &&
					startRelationReadItem != NULL &&
					!generalizationWordItem->isImperativeVerbDisplayLoginOrRead() )
						{
						if( ( contextResult = getRelationContext( false, isArchivedAssignment, isPossessive, isQuestion, true, generalizationWordItem, currentSpecificationWordItem, NULL, startRelationReadItem ) ).result != RESULT_OK )
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

								if( isSelection &&
								adminItem_->createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a value selection item" );

								// Value, but no relation
								if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a value specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
										if( addUserSpecificationWithRelation( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, selectionLevel, selectionListNr, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else
										{
										// Selection, no value, no relation
										if( adminItem_->createSelectionPart( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, false, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, generalizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part" );
										}
									}
								else	// No selection
									{
									if( previousSpecificationWordItem != NULL &&
									currentSpecificationWordItem != NULL &&
									currentReadItem->isMatchingReadWordTypeNr( previousSpecificationWordTypeNr ) &&

									( ( existingUserSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, currentSpecificationWordItem ) ) == NULL ||
									!existingUserSpecificationItem->hasCompoundSpecificationCollection() ||
									existingUserSpecificationItem->isSpecificationGeneralization() ) &&

									// Collect specification words
									collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, generalizationWordTypeNr, currentSpecificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to collect specification words" );

									previousSpecificationWordItem = currentSpecificationWordItem;
									previousSpecificationWordTypeNr = currentSpecificationWordTypeNr;
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
											addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, isChineseReasoning, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, NO_SELECTION_LEVEL, NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a user specification with a relation to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else
											{
											// No selection, no value, no relation
											if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a specification without relation to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}

										// Start reasoning
										if( isGeneralizationReasoningWordType &&
										!isNegative &&
										!isPartOf &&
										!isQuestion &&
										currentSpecificationWordItem != NULL &&
										adminItem_->isNounWordType( currentSpecificationWordTypeNr ) )
											{
											if( isPossessive )
												{
												if( isSpanishCurrentLanguage &&
												startRelationReadItem != NULL &&
												userSpecificationItem_ != NULL &&
												globalVariables_->nUserRelationWords == 1 &&
												currentSpecificationWordItem->isNounWordSpanishAmbiguous() &&
												( lookAheadSpanishRelationWordItem = startRelationReadItem->lookAheadRelationWordItem() ) != NULL &&
												!lookAheadSpanishRelationWordItem->isMale() &&

												// Mark relation word as male
												lookAheadSpanishRelationWordItem->markWordAsMale() != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to mark relation word \"", lookAheadSpanishRelationWordItem->anyWordTypeString(), "\" as male" );
												}
											else
												{
												if( isSpanishCurrentLanguage &&
												isGeneralizationProperNoun &&
												generalizationWordItem->isMale() &&
												// Display Spanish assumptions that are not hidden anymore
												displaySpanishAssumptionsThatAreNotHiddenAnymore( hasRelationWord, isAssignment, isArchivedAssignment, currentSpecificationWordItem ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to display Spanish assumptions that are not hidden anymore" );

												// Draw proper noun part-of conclusions
												if( adminItem_->drawProperNounPartOfConclusions( false, isArchivedAssignment, ( isGeneralizationProperNoun ? generalizationWordItem : NULL ), ( isGeneralizationProperNoun ? currentSpecificationWordItem : generalizationWordItem ), NULL ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

												if( currentSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&
												// Make a specification substitution 'part of' assumption
												adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												// Draw a specification substitution conclusion or ask a question
												if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( false, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, NULL ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question for word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( !globalVariables_->hasDisplayedWarning )
													{
													if( isGeneralizationProperNoun )
														{
														if( isSpanishCurrentLanguage &&
														!isExclusiveSpecification &&
														generalizationWordItem->isUserGeneralizationWord &&
														// Draw simple negative Spanish conclusions
														adminItem_->drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a simple negative Spanish conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}
													else
														{
														// Definition specification
														if( ( compoundResult = adminItem_->drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, generalizationWordItem ) ).result != RESULT_OK )
															return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a primary specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

														compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
														}
													}
												}

											if( isGeneralizationProperNoun &&
											!isUncountableGeneralizationNoun &&
											!globalVariables_->hasDisplayedWarning &&
											globalVariables_->nUserRelationWords == 1 &&
											startRelationReadItem != NULL &&
											( firstRelationReadItem = startRelationReadItem->firstRelationWordReadItem() ) != NULL &&
											( relationWordItem = firstRelationReadItem->readWordItem() ) != NULL )
												{
												// Draw reversible conclusions
												if( adminItem_->drawReversibleConclusions( false, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( generalizationWordItem->isUserGeneralizationWord &&
												( tempUserSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, currentSpecificationWordItem ) ) != NULL &&
												tempUserSpecificationItem->hasSpecificationCollection() &&
												tempUserSpecificationItem->hasNewInformation() &&
												// Make an exclusive specification substitution assumption
												adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, firstRelationReadItem->wordTypeNr(), currentSpecificationWordTypeNr, generalizationWordTypeNr, relationWordItem, currentSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
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
							nContextRelations = 0;
						}

					if( !globalVariables_->hasDisplayedWarning &&
					!isQuestion &&
					currentSpecificationWordItem != NULL &&
					linkedSpecificationWordItem != NULL &&

					( isRelationWord ||
					isSpecificationWord ) &&

					( !hasRelationWord ||
					linkedSpecificationWordItem != currentSpecificationWordItem ) )
						{
						if( isGeneralizationReasoningWordType &&
						!isPreposition &&
						!isValueSpecificationWord &&
						linkedSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&

						( isGeneralizationProperNoun ||

						// Generalization word is noun
						( !isEveryGeneralization &&
						isPossessive ) ) &&

						// Make an exclusive specification substitution assumption
						adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, generalizationWordItem, linkedSpecificationWordItem, ( hasRelationWord ? currentSpecificationWordItem : NULL ) ) != RESULT_OK )
							return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"", linkedSpecificationWordItem->anyWordTypeString(), "\"" );

						if( isRelationWord ||
						hasLinkedPossessiveDeterminer )
							{
							if( !globalVariables_->hasDisplayedWarning &&
							// Linked specification
							currentSpecificationWordItem == linkedSpecificationWordItem )
								{
								if( linkedGeneralizationWordItem_ != NULL )
									return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "Linked word \"", linkedGeneralizationWordItem_->anyWordTypeString(), "\" is already assigned" );

								linkedGeneralizationWordTypeNr_ = generalizationWordTypeNr;
								linkedGeneralizationWordItem_ = generalizationWordItem;
								}

							if( !globalVariables_->hasDisplayedWarning &&
							isGeneralizationProperNoun &&
							userRelationContextNr_ > NO_CONTEXT_NR &&
							globalVariables_->nUserRelationWords > 1 &&
							( tempUserSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, linkedSpecificationWordItem ) ) != NULL &&
							tempUserSpecificationItem->hasNewInformation() &&

							( currentSpecificationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, linkedSpecificationWordItem ) == NULL ||

							// Typical for Spanish. Test file: "Complejo (19 - extraño)"
							( isSpanishCurrentLanguage &&
							!tempUserSpecificationItem->isUserAssignment() &&
							( tempSpanishSpecificationItem = currentSpecificationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, true, linkedSpecificationWordItem, generalizationWordItem ) ) != NULL &&
							!tempSpanishSpecificationItem->isOlderItem() ) ) )
								{
								// Draw reversible conclusions
								if( adminItem_->drawReversibleConclusions( false, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								if( !isExclusiveSpecification &&
								// User specification doesn't exist
								tempUserSpecificationItem->hasSpecificationCollection() &&
								// Make an exclusive specification substitution assumption
								adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, currentSpecificationWordTypeNr, linkedSpecificationWordTypeNr, generalizationWordTypeNr, currentSpecificationWordItem, linkedSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
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
		while( !globalVariables_->hasDisplayedWarning &&
		currentReadItem != endSpecificationReadItem &&
		( currentReadItem = currentReadItem->nextReadItem() ) != NULL );

		if( !hasAddedSpecification )
			return adminItem_->startErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I have not added any specification" );

		if( isGeneralizationProperNoun &&
		!isQuestion &&
		!isSelection )
			{
			if( hasRelationWord )
				{
				if( recalculateAssumptionsInTouchedWords() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumptions of all touched words" );
				}
			else
				{
				if( generalizationWordItem->recalculateAssumptionsInWord() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumptions of word \"", generalizationWordItem->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char collectGeneralizationWordWithPreviousOne( bool isAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
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
			( foundSpecificationItem = currentGeneralizationWordItem->firstAssignmentOrSpecificationItem( false, isPossessive, relationContextNr, specificationWordItem ) ) != NULL )
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

	SpecificationItem *simpleUserSpecificationItem()
		{
		return simpleUserSpecificationItem_;
		}

	SpecificationItem *userSpecificationItem()
		{
		return userSpecificationItem_;
		}

	ContextResultType getRelationContext( bool hasAlreadyAddedChineseRelationWords, bool isArchivedAssignment, bool isPossessive, bool isQuestion, bool isUserSentence, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
		{
		bool isSkippingThisContext = false;
		unsigned int currentRelationContextNr;
		unsigned int foundRelationContextNr;
		ContextItem *currentRelationContextItem;
		ReadItem *relationReadItem = NULL;
		SpecificationItem *foundSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *currentRelationWordItem = relationWordItem;
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getRelationContext";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( startRelationReadItem != NULL )
			{
			if( ( relationReadItem = startRelationReadItem->firstRelationWordReadItem() ) == NULL )
				return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The read word of the first relation word is undefined" );

			currentRelationWordItem = relationReadItem->readWordItem();
			}

		if( currentRelationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "I couldn't find any relation word" );

		if( ( currentRelationContextItem = currentRelationWordItem->firstActiveContextItem() ) != NULL )
			{
			// Do for all relation context items in the first relation context word
			do	{
				if( ( currentRelationContextNr = currentRelationContextItem->contextNr() ) > NO_CONTEXT_NR &&
				currentRelationWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem ) )
					{
					isSkippingThisContext = false;

					if( relationReadItem == NULL )
						isSkippingThisContext = true;
					else
						{
						if( ( currentContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
							{
							// Do for all context words, either in the current relation list or outside this list
							do	{
								if( relationReadItem->hasFoundRelationWordInThisList( currentContextWordItem ) != currentContextWordItem->hasContextInWord( currentRelationContextNr, specificationWordItem ) )
									isSkippingThisContext = true;
								}
							while( !isSkippingThisContext &&
							( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
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
					!currentRelationContextItem->isOlderItem() )
						contextResult.contextNr = currentRelationContextNr;
					}
				}
			while( contextResult.contextNr == NO_CONTEXT_NR &&
			( currentRelationContextItem = currentRelationContextItem->nextContextItem() ) != NULL );
			}

		if( !isQuestion &&
		contextResult.contextNr == NO_CONTEXT_NR &&
		( foundSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, isArchivedAssignment, false, isPossessive, isPossessive, specificationWordItem, NULL ) ) != NULL &&
		( foundRelationContextNr = foundSpecificationItem->relationContextNr() ) > NO_CONTEXT_NR )
			{
			// Equal number (or more) relation words
			if( adminItem_->nContextWords( foundRelationContextNr, specificationWordItem ) >= globalVariables_->nUserRelationWords )
				{
				if( foundSpecificationItem->isExclusiveSpecification() )
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

						if( !foundSpecificationItem->isActiveAssignment() )
							{
							// Static (exclusive) semantic ambiguity
							if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_NOTICED_SEMANTIC_AMBIGUITY_START, generalizationWordItem->anyWordTypeString(), INTERFACE_SENTENCE_NOTIFICATION_STATIC_SEMANTIC_AMBIGUITY_END ) != RESULT_OK )
								return adminItem_->addContextResultError( functionNameString, moduleNameString_, "I failed to write the 'static semantic ambiguity' interface notification" );

							contextResult.isAmbiguousRelationContext = true;
							}
						}
					}
				}
			}

		if( contextResult.contextNr == NO_CONTEXT_NR )
			{
			if( ( contextResult.contextNr = adminItem_->highestContextNrInAllContextWords() ) >= MAX_CONTEXT_NR )
				return adminItem_->startContextResultSystemError( functionNameString, moduleNameString_, "Context number overflow" );

			// Create new context number
			contextResult.contextNr++;
			}

		return contextResult;
		}

	CreateAndAssignResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNounEnding, bool isAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkipDrawingSpanishAmbiguousSubstitutionConclusion, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasDisplayedIntegrityWarning = false;
		bool hasFoundRelationContext = false;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool isAssumption = adminItem_->isAssumption( assumptionJustificationTypeNr );
		bool isOlderFoundSpecification;
		bool isSelectedUniqueUserRelation;
		bool isUserGeneralizationWord;
		unsigned short justificationTypeNr = assumptionJustificationTypeNr;
		unsigned short tempAssumptionLevel;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *createdOrFoundJustificationItem = NULL;
		JustificationItem *foundJustificationItem = NULL;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		JustificationResultType justificationResult;
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSelfGeneratedSpecification";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Given assumption level has no value
		if( assumptionLevel == NO_ASSUMPTION_LEVEL )
			{
			// Calculate assumption level
			assumptionLevel = adminItem_->assumptionGrade( ( anotherPrimarySpecificationItem != NULL ), hasFeminineOrMasculineProperNounEnding, ( primarySpecificationItem != NULL && primarySpecificationItem->isGeneralizationProperNoun() && primarySpecificationItem->isPossessive() ), ( primarySpecificationItem != NULL && primarySpecificationItem->isQuestion() ), assumptionJustificationTypeNr );

			if( primarySpecificationItem != NULL &&
			( tempAssumptionLevel = primarySpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;

/*			if( anotherPrimarySpecificationItem != NULL &&
			( tempAssumptionLevel = anotherPrimarySpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;
*/
			if( secondarySpecificationItem != NULL &&
			( tempAssumptionLevel = secondarySpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;

			if( anotherSecondarySpecificationItem != NULL &&
			( tempAssumptionLevel = anotherSecondarySpecificationItem->assumptionLevel() ) > NO_ASSUMPTION_LEVEL )
				assumptionLevel += tempAssumptionLevel;

			// No assumption level: So, it is a conclusion
			if( assumptionLevel == NO_ASSUMPTION_LEVEL )
				{
				if( !adminItem_->isConclusion( conclusionJustificationTypeNr ) )
					return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given conclusion justification type isn't a conclusion" );

				// Conclusion
				isAssumption = false;
				justificationTypeNr = conclusionJustificationTypeNr;
				}
			}

		if( ( justificationResult = generalizationWordItem->addJustification( hasFeminineOrMasculineProperNounEnding, isForcingNewJustification, false, justificationTypeNr, NO_ORDER_NR, globalVariables_->currentSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a justification" );

		createdJustificationItem = justificationResult.createdJustificationItem;
		foundJustificationItem = justificationResult.foundJustificationItem;

		if( ( createdOrFoundJustificationItem = ( createdJustificationItem == NULL ? foundJustificationItem : createdJustificationItem ) ) == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I couldn't find or create a justification" );

		isSelectedUniqueUserRelation = ( justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION ||
										justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );

		if( ( createAndAssignResult = addSpecification( isAssignment, false, isArchivedAssignment, isCharacteristicFor, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isSelectedUniqueUserRelation, false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification" );

		if( !globalVariables_->hasDisplayedWarning )
			{
			hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			createdSpecificationItem = createAndAssignResult.createdSpecificationItem;

			if( ( foundSpecificationItem = createAndAssignResult.foundSpecificationItem ) != NULL )
				hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

			if( createdSpecificationItem == NULL )
				{
				if( foundSpecificationItem == NULL )
					return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I couldn't find or create a specification" );

				if( foundSpecificationItem->isUserSpecification() )
					{
					hasDisplayedIntegrityWarning = true;

					strcpy( errorString, "\nI found a user specification, whilst trying to create a self-generated specification:\n\tSpecificationItem: " );
					// The result of this call is stored in a shared string. Hence, one call at the time.
					strcat( errorString, foundSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ";\n\tJustificationItem: " );
					// The result of this call is stored in a shared string. Hence, one call at the time.
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

					isOlderFoundSpecification = foundSpecificationItem->isOlderItem();

					// A justification has been created, but the self-generated specification already exists
					// So, the justification needs to be added or replaced separately

					// No justification is created
					if( ( createdJustificationItem == NULL &&

					( ( ( !isOlderFoundSpecification &&
					isPossessive ) ||

					// Typical for Spanish
					( isOlderFoundSpecification &&
					justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION ) ) &&

					!foundSpecificationItem->hasJustification( foundJustificationItem ) ) ) ||

					// Justification is created
					( createdJustificationItem != NULL &&

					( !isAssumption ||
					foundSpecificationItem->isSelfGeneratedAssumption() ||

					// Conclusion is forced to become an assumption
					( justificationTypeNr == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ||

					// Typical for Spanish (not possessive)
					( !isPossessive &&
					justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ||

					// Typical for Spanish (possessive)
					( isPossessive &&
					primarySpecificationItem != NULL &&

					( ( justificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION &&
					foundSpecificationItem->hasCompoundSpecificationCollection() ) ||

					( justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
					hasGeneralizationWordCurrentlyConfirmedSpecification &&

					( primarySpecificationItem->isUserAssignment() ||
					foundSpecificationItem->hasOnlyOneRelationWord() ) ) ) ) ) ) ) )
						{
						if( processJustification( hasFoundRelationContext, isOlderFoundSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to process the created justification" );
						}
					}
				}
			else
				{
				if( hasFoundRelationContext &&
				!hasGeneralizationWordCurrentlyConfirmedSpecification &&
				foundSpecificationItem != NULL &&
				foundSpecificationItem->relationContextNr() == createdSpecificationItem->relationContextNr() )
					return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "Integrity error! A specification was created in word \"", generalizationWordItem->anyWordTypeString(), "\" while a specification was found as well with the same relation context" );
				}

			if( !hasDisplayedIntegrityWarning &&
			!isNegative &&
			!isPartOf &&

			( createdJustificationItem != NULL ||
			createdSpecificationItem != NULL ||

			// Typical for Spanish
			( foundSpecificationItem != NULL &&
			foundSpecificationItem->isHiddenSpanishSpecification() ) ) &&

			adminItem_->isNounWordType( specificationWordTypeNr ) )
				{
				if( isPossessive )
					{
					if( isUniqueUserRelation &&
					// Draw a unique relation conclusion
					addSelfGeneratedSpecification( false, false, false, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, false, true, false, true, false, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION, JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, NULL, NULL, createdSpecificationItem, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated unique specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					if( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN &&
					// Draw proper noun part-of conclusions
					adminItem_->drawProperNounPartOfConclusions( false, isArchivedAssignment, generalizationWordItem, specificationWordItem, ( !isUserGeneralizationWord && specificationWordItem->isNounWordSpanishAmbiguous() ? relationWordItem : NULL ) ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

					if( !isSkipDrawingSpanishAmbiguousSubstitutionConclusion )
						{
						// Make a specification substitution 'part of' assumption
						if( adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						// Draw a specification substitution conclusion or ask a question
						if( adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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
