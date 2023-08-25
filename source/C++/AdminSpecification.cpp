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

		if( relationContextNr == NO_CONTEXT_NR ||
		specificationWordItem == NULL ||
		relationWordItem == NULL )
			return false;

		if( !relationWordItem->hasContextInWord( relationContextNr ) &&
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

	unsigned int collectionNrByCompoundGeneralizationWordInCollectionWords( bool isExclusiveSpecification, bool isTryingSmartSearch, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem )
		{
		unsigned int collectionNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentCollectionWordItem;
		WordItem *currentSpecificationWordItem;

		if( compoundGeneralizationWordItem != NULL )
			{
			// Try smart search first
			if( isTryingSmartSearch &&
			( currentSpecificationItem = compoundGeneralizationWordItem->firstExclusiveSpecificationItem( false ) ) != NULL )
				{
				// Do for all exclusive specifications of this word (skip adjectives)
				do	{
					if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) != NULL &&
					( collectionNr = currentSpecificationWordItem->collectionNrByCompoundGeneralizationWordInWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) ) > NO_COLLECTION_NR )
						return collectionNr;
					}
				while( ( currentSpecificationItem = currentSpecificationItem->nextExclusiveSpecificationItem( false ) ) != NULL );
				}

			// Not found. Do full search
			if( ( currentCollectionWordItem = globalVariables_->firstCollectionWordItem ) != NULL )
				{
				// Do for all collection words
				do	{
					if( currentCollectionWordItem != compoundGeneralizationWordItem &&
					( collectionNr = currentCollectionWordItem->collectionNrByCompoundGeneralizationWordInWord( isExclusiveSpecification, collectionWordTypeNr, compoundGeneralizationWordItem ) ) > NO_COLLECTION_NR )
						return collectionNr;
					}
				while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
				}
			}

		return NO_COLLECTION_NR;
		}

	WordItem *contextUsedInUserSpecificationOfWordItem( bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		// Specification collection number may be undefined

		if( specificationWordItem != NULL &&
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

	signed char addUserSpecificationWithRelation( bool isAction, bool isAssignedOrClear, bool isAssignment, bool isCharacteristicFor, bool isChineseReasoning, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpanishCurrentLanguage, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, unsigned short imperativeVerbParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, unsigned int nUserRelationWords, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isChineseReversedImperativeNoun;
		bool isFirstComparisonPart = ( selectionListNr == ADMIN_CONDITION_LIST );
		bool isFirstRun = true;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		unsigned short relationWordTypeNr;
		unsigned int specificationCollectionNr;
		ReadItem *currentRelationReadItem = startRelationReadItem;
		SpecificationItem *chineseUserSpecificationItem;
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
							( foundSpecificationItem = generalizationWordItem->firstAssignmentOrSpecificationItem( isAssignment, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationWordItem ) ) != NULL &&
							foundSpecificationItem->hasRelationContext() &&
							( mostRecentContextWord = mostRecentContextWordInContextWords( foundSpecificationItem->relationContextNr() ) ) != NULL &&
							mostRecentContextWord != relationWordItem )
								{
								previousRelationWordItem = mostRecentContextWord;

								if( foundSpecificationItem->hasNonCompoundSpecificationCollection() &&
								!foundSpecificationItem->isSpecificationWordSpanishAmbiguous() )
									specificationCollectionNr = foundSpecificationItem->specificationCollectionNr();
								}

							if( previousRelationWordItem != NULL &&
							// Collect relation words
							collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, relationWordItem, specificationWordItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous relation word \"", previousRelationWordItem->anyWordTypeString(), "\" to relation word \"", relationWordItem->anyWordTypeString(), "\"" );

							if( selectionListNr == NO_LIST_NR )
								{
								// Typical for Chinese
								if( isChineseReasoning )
									{
									if( userSpecificationItem_->hasCompoundSpecificationCollection() )
										{
										// Make a Chinese exclusive specification substitution assumption (non-possessive)
										if( adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

										if( ( chineseUserSpecificationItem = generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, true, isArchivedAssignment, false, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
										chineseUserSpecificationItem->hasSpecificationCollection() &&
										!chineseUserSpecificationItem->isOlderItem() &&
										// Older possessive user specification doesn't exist
										relationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem ) == NULL &&
										// Avoid idle call
										relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, specificationWordItem, generalizationWordItem ) != NULL &&
										// Make a Chinese exclusive specification substitution assumption (possessive)
										adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, relationWordTypeNr, specificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, specificationWordItem, generalizationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make an exclusive specification substitution assumption with relation word \"", relationWordItem->anyWordTypeString(), "\" and specification word \"", specificationWordItem->anyWordTypeString(), "\"" );
										}
									}
								else
									{
									if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, NULL, generalizationWordItem, specificationWordItem, relationWordItem, NULL ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a specification having relation word \"", relationWordItem->anyWordTypeString(), "\"" );

									if( !isQuestion &&
									!isSpanishCurrentLanguage &&

									( isFirstRun ||

									( userSpecificationItem_ != NULL &&
									userSpecificationItem_->hasNonCompoundSpecificationCollection() ) ) &&

									// Avoid idle call
									specificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL &&
									// Draw a specification substitution conclusion or ask a question
									adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
										return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"", generalizationWordItem->anyWordTypeString(), "\"" );

									isFirstRun = false;
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
			if( ( collectionResult = previousGeneralizationWordItem->createCollectionItem( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, currentGeneralizationWordItem, newCommonWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous word \"", previousGeneralizationWordItem->anyWordTypeString(), "\" with current word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( generalizationCollectionNr == NO_COLLECTION_NR )
				generalizationCollectionNr = collectionResult.createdCollectionNr;

			if( currentGeneralizationWordItem->createCollectionItem( isExclusiveGeneralization, generalizationWordTypeNr, commonWordTypeNr, generalizationCollectionNr, previousGeneralizationWordItem, newCommonWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect current word \"", currentGeneralizationWordItem->anyWordTypeString(), "\" with previous word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( previousGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in previous word \"", previousGeneralizationWordItem->anyWordTypeString(), "\"" );

			if( currentGeneralizationWordItem->collectGeneralizations( isExclusiveGeneralization, generalizationCollectionNr ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect generalizations in new word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );
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
			// Each collection comes in pairs
			if( ( collectionResult = previousRelationWordItem->createCollectionItem( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, currentRelationWordItem, commonWordItem, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous word \"", previousRelationWordItem->anyWordTypeString(), "\" with current word \"", currentRelationWordItem->anyWordTypeString(), "\"" );

			if( collectionNr == NO_COLLECTION_NR )
				collectionNr = collectionResult.createdCollectionNr;

			if( currentRelationWordItem->createCollectionItem( isExclusiveSpecification, relationWordTypeNr, commonWordTypeNr, collectionNr, previousRelationWordItem, commonWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect current word \"", currentRelationWordItem->anyWordTypeString(), "\" with previous word \"", previousRelationWordItem->anyWordTypeString(), "\"" );
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
			( specificationCollectionNr = previousSpecificationWordItem->collectionNr( generalizationWordItem ) ) == NO_COLLECTION_NR &&
			( specificationCollectionNr = currentSpecificationWordItem->collectionNr( generalizationWordItem ) ) == NO_COLLECTION_NR &&

			( specificationWordTypeNr != WORD_TYPE_NOUN_PLURAL ||
			generalizationWordTypeNr != WORD_TYPE_NOUN_SINGULAR ) &&

			( specificationCollectionNr = previousSpecificationWordItem->collectionNr( specificationWordTypeNr ) ) == NO_COLLECTION_NR )
				specificationCollectionNr = currentSpecificationWordItem->collectionNr( specificationWordTypeNr );
			}
		else
			{
			if( !isExclusiveSpecification ||
			generalizationWordItem == previousSpecificationWordItem ||
			!generalizationWordItem->isNounWordSpanishAmbiguous() )
				specificationCollectionNr = collectionNrByCompoundGeneralizationWordInCollectionWords( isExclusiveSpecification, ( isExclusiveSpecification && generalizationWordItem->isFeminineOrMasculineWord() ), specificationWordTypeNr, compoundGeneralizationWordItem );
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
			if( ( collectionResult = previousSpecificationWordItem->createCollectionItem( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, currentSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect previous word \"", previousSpecificationWordItem->anyWordTypeString(), "\" with current word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );

			if( specificationCollectionNr == NO_COLLECTION_NR )
				specificationCollectionNr = collectionResult.createdCollectionNr;

			if( currentSpecificationWordItem->createCollectionItem( isExclusiveSpecification, specificationWordTypeNr, generalizationWordTypeNr, specificationCollectionNr, previousSpecificationWordItem, generalizationWordItem, compoundGeneralizationWordItem ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect current word \"", currentSpecificationWordItem->anyWordTypeString(), "\" with previous word \"", previousSpecificationWordItem->anyWordTypeString(), "\"" );
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

		if( ( currentContextWordItem = adminItem_->firstContextWordItem( existingContextNr ) ) != NULL )
			{
			// Do for all context words with existing relation context
			do	{
				if( ( currentContextItem = currentContextWordItem->contextItem( existingContextNr ) ) != NULL &&
				// Add copied context
				currentContextWordItem->addContext( currentContextItem->contextWordTypeNr(), currentContextItem->specificationWordTypeNr(), newContextNr, newSpanishAmbiguousCollectionNr, currentContextItem->specificationWordItem() ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a copied context to word \"", currentContextWordItem->anyWordTypeString(), "\"" );
				}
			while( ( currentContextWordItem = currentContextWordItem->_nextContextWordItem( existingContextNr ) ) != NULL );
			}

		return RESULT_OK;
		}

	signed char displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords( bool isAssignment, bool isArchivedAssignment, WordItem *specificationWordItem )
		{
		SpecificationItem *hiddenSpanishSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *hiddenSpanishSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "displaySpanishAssumptionsThatAreNotHiddenAnymoreInContextWords";

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentContextWordItem = globalVariables_->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( ( hiddenSpanishSpecificationItem = currentContextWordItem->firstUnhiddenSpanishSpecificationItem() ) != NULL )
					{
					if( !isAssignment )
						{
						currentContextWordItem->hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = true;

						// Write Spanish assumption
						if( currentContextWordItem->writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a Spanish assumption about word \"", currentContextWordItem->anyWordTypeString(), "\" that is not hidden anymore" );
						}

					if( ( hiddenSpanishSpecificationWordItem = hiddenSpanishSpecificationItem->specificationWordItem() ) != NULL &&
					// Draw Spanish proper noun part-of conclusions
					adminItem_->drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, hiddenSpanishSpecificationWordItem, currentContextWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw Spanish proper noun 'part of' conclusions in proper noun words" );
					}
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return RESULT_OK;
		}

	signed char processJustification( bool hasRelationWordConfirmedSpecification, bool isOlderFoundSpecification, bool isUserGeneralizationWord, unsigned short justificationTypeNr, JustificationItem *createdOrFoundJustificationItem, JustificationItem *firstJustificationItem, SpecificationItem *foundSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundSpecificationCurrentCreationSentenceNr;
		bool isRemovingPreviousJustifications = false;
		bool isSpecificationWordSpanishAmbiguous;
		JustificationItem *obsoleteJustificationItem = NULL;
		JustificationItem *questionJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		WordItem *specificationWordItem;
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

		hasFoundSpecificationCurrentCreationSentenceNr = foundSpecificationItem->hasCurrentCreationSentenceNr();
		isSpecificationWordSpanishAmbiguous = foundSpecificationItem->isSpecificationWordSpanishAmbiguous();

		if( relationWordItem != NULL )
			hasRelationWordConfirmedSpecification = relationWordItem->hasCurrentlyConfirmedSpecification();

		switch( justificationTypeNr )
			{
			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
				if( isUserGeneralizationWord )
					{
					if( relationWordItem != NULL &&
					secondarySpecificationItem != NULL &&
					secondarySpecificationItem->isSelfGeneratedConclusion() &&
					( specificationWordItem = secondarySpecificationItem->specificationWordItem() ) != NULL &&
					( assumptionSpecificationItem = relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), false, false, true, specificationWordItem, generalizationWordItem ) ) != NULL )
						obsoleteJustificationItem = generalizationWordItem->secondarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION, assumptionSpecificationItem );
					else
						{
						// Typical for Spanish
						if( !foundSpecificationItem->isOlderItem() )
							obsoleteJustificationItem = foundSpecificationItem->firstReplacedSecondarySpecificationJustificationItem();
						}
					}

				break;

			case JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
				// Typical for Spanish
				if( isSpecificationWordSpanishAmbiguous )
					{
					if( !hasRelationWordConfirmedSpecification &&
					primarySpecificationItem->isUserAssignment() )
						obsoleteJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, relationWordItem );
					}
				else
					{
					if( isOlderFoundSpecification &&
					!isUserGeneralizationWord &&
					foundSpecificationItem->hasOnlyOneRelationWord() )
						isRemovingPreviousJustifications = true;
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				if( foundSpecificationItem->hasAssumptionLevel() )
					{
					if( primarySpecificationItem->isCorrectedSpecification() )
						// Test file: "Correcting invalidated assumption (by knowledge)"
						obsoleteJustificationItem = foundSpecificationItem->firstReplacedPrimarySpecificationJustificationItem();
					else
						{
						if( !hasFoundSpecificationCurrentCreationSentenceNr &&
						foundSpecificationItem->isGeneralizationProperNoun() &&
						foundSpecificationItem->assumptionLevel() > createdOrFoundJustificationItem->justificationAssumptionLevel() &&

						( !primarySpecificationItem->isSpecificationWordSpanishAmbiguous() ||
						// Typical for Spanish
						firstJustificationItem->hasAdditionalDefinitionSpecification() ) &&

						( foundSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL ||
						foundSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) != NULL ) )
							isRemovingPreviousJustifications = true;
						}
					}

				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION:
				if( isOlderFoundSpecification &&
				foundSpecificationItem->hasAssumptionLevel() &&

				// Typical for Spanish
				( hasFoundSpecificationCurrentCreationSentenceNr ||
				primarySpecificationItem->hasRelationContext() ) &&

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
						if( !hasFoundSpecificationCurrentCreationSentenceNr &&
						( questionJustificationItem = foundSpecificationItem->firstOlderJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION ) ) != NULL &&
						( assumptionSpecificationItem = questionJustificationItem->primarySpecificationItem() ) != NULL &&
						assumptionSpecificationItem->assumptionLevel() > primarySpecificationItem->assumptionLevel() )
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
				if( foundSpecificationItem->changeFirstJustification( false, createdOrFoundJustificationItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change the first justification of the found specification" );
				}
			else
				{
				// Copy and replace
				if( generalizationWordItem->copyAndReplaceSpecification( foundSpecificationItem->isAnsweredQuestion(), foundSpecificationItem->isEveryGeneralization(), foundSpecificationItem->generalizationCollectionNr(), foundSpecificationItem->specificationCollectionNr(), createdOrFoundJustificationItem, foundSpecificationItem ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to copy and replace the found specification, with a different first justification item" );

				foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();
				}

			if( !foundSpecificationItem->isQuestion() &&
			// Recalculate assumption level of found specification
			foundSpecificationItem->recalculateAssumptionLevel( false ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to recalculate the assumption level of the found specification" );

			// Clear stored sentence buffer, because of recalculated assumption level
			foundSpecificationItem->clearStoredSentenceString();
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

	WordItem *mostRecentContextWordInContextWords( unsigned int contextNr )
		{
		WordItem *currentContextWordItem;
		WordItem *mostRecentWordItem = NULL;

		if( ( currentContextWordItem = adminItem_->firstContextWordItem( contextNr ) ) != NULL )
			{
			// Do for all context words with given context
			do	mostRecentWordItem = currentContextWordItem;
			while( ( currentContextWordItem = currentContextWordItem->_nextContextWordItem( contextNr ) ) != NULL );
			}

		return mostRecentWordItem;
		}

	ContextResultType getRelationContextAdvanced( bool hasRelationWordConfirmedSpecification, bool isAssignment, bool isArchivedAssignment, bool isPossessive, bool isSelfGeneratedAssumption, bool isSpecificationWordSpanishAmbiguous, unsigned int specificationCollectionNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasCompoundUserSpecificationCollection = false;
		bool hasExistingSpecificationCompoundCollection = false;
		bool hasExistingSpecificationCurrentCreationSentenceNr = false;
		bool hasExistingSpecificationOnlyOneRelationWord = false;
		bool hasGeneralizationWordCurrentlyConfirmedSpecification;
		bool hasNonCompoundUserSpecificationCollection = false;
		bool hasRelationWordExistingRelationContext;
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isCompoundCollectionSpanishAmbiguous = false;
		bool isExistingArchivedAssignment = false;
		bool isExistingHiddenSpanishSpecification = false;
		bool isExistingOlderSpecification = false;
		bool isExistingSpecificationSelfGeneratedAssumption = false;
		bool isExistingUserSpecification = false;
		bool isOlderUserSpecification = false;
		bool isUserArchivedAssignment = false;
		bool isUserAssignment = false;
		bool isUserRelationWord;
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
		SpecificationItem *tempSpecificationItem;
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getRelationContextAdvanced";

		if( generalizationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startContextResultError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		hasGeneralizationWordCurrentlyConfirmedSpecification = generalizationWordItem->hasCurrentlyConfirmedSpecification();
		isUserRelationWord = generalizationWordItem->isUserRelationWord;

		if( userSpecificationItem_ != NULL )
			{
			if( userSpecificationItem_->hasNonCompoundSpecificationCollection() )
				hasNonCompoundUserSpecificationCollection = true;
			else
				{
				if( userSpecificationItem_->hasCompoundSpecificationCollection() )
					hasCompoundUserSpecificationCollection = true;
				}

			if( userSpecificationItem_->isAssignment() )
				{
				isUserAssignment = true;

				if( userSpecificationItem_->isArchivedAssignment() )
					isUserArchivedAssignment = true;
				}

			isOlderUserSpecification = userSpecificationItem_->isOlderItem();
			userSpecificationCollectionNr = userSpecificationItem_->specificationCollectionNr();
			}

		if( isSpecificationWordSpanishAmbiguous &&
		isPossessive &&
		specificationCollectionNr > NO_COLLECTION_NR &&
		specificationWordItem->isCompoundCollection( specificationCollectionNr ) )
			{
			isCompoundCollectionSpanishAmbiguous = true;
			existingSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, relationWordItem );
			}
		else
			{
			if( ( existingSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, isPossessive, specificationCollectionNr, specificationWordItem ) ) != NULL &&
			specificationCollectionNr > NO_COLLECTION_NR &&
			!existingSpecificationItem->hasRelationContext() &&
			( tempSpecificationItem = generalizationWordItem->firstSelfGeneratedCheckSpecificationItem( false, false, false, false, true, specificationWordItem, relationWordItem ) ) != NULL )
				existingSpecificationItem = tempSpecificationItem;
			}

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
			isExistingSpecificationSelfGeneratedAssumption = existingSpecificationItem->isSelfGeneratedAssumption();
			isExistingUserSpecification = existingSpecificationItem->isUserSpecification();
			existingSpecificationCollectionNr = existingSpecificationItem->specificationCollectionNr();
			existingRelationContextNr = existingSpecificationItem->relationContextNr();
			}

		// Try to find relation context with same number of relation words as in the user sentence
		if( nUserRelationWords > 0 )
			foundContextItem = relationWordItem->contextItem( isCompoundCollectionSpanishAmbiguous, nUserRelationWords, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : NO_COLLECTION_NR ), specificationWordItem );

		if( foundContextItem == NULL )
			{
			if( existingSpecificationItem == NULL )
				{
				if( ( foundContextNr = relationWordItem->contextNr( ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : NO_COLLECTION_NR ), specificationWordItem ) ) > NO_CONTEXT_NR &&

				// Typical for Chinese
				( generalizationWordItem->isFeminineWord() ||

				( globalVariables_->nUserGeneralizationWords > 1 &&

				( generalizationWordItem->hasFeminineOrMasculineProperNounEnding() ||
				contextUsedInUserSpecificationOfWordItem( isPossessive, specificationCollectionNr, foundContextNr, specificationWordItem ) == NULL ) ) ||

				// Generalization word is user relation word
				( isUserRelationWord &&

				( isUserAssignment ||

				( hasNonCompoundUserSpecificationCollection &&
				isPossessive ) ) &&

				adminItem_->nContextWords( foundContextNr ) == 1 ) ) )
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
					if( isExistingArchivedAssignment == isArchivedAssignment )
						{
						if( existingRelationContextNr == NO_CONTEXT_NR )
							{
							if( ( foundSpecificationItem = generalizationWordItem->firstAssignmentItem( false, false, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
							foundSpecificationItem->hasOnlyOneRelationWord() )
								// Need to copy context (1)
								// Typical for Spanish
								// Spanish test file: "Juan es el padre de Pablo - José tiene 2 padres"
								contextResult.copiedRelationContextNr = foundSpecificationItem->relationContextNr();
							}
						else
							foundRelationContextNr = existingRelationContextNr;
						}
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
								contextResult.contextNr = ( isExistingOlderSpecification ? existingRelationContextNr : foundContextNr );
							}
						}
					}
				else
					{
					if( contextResult.contextNr == NO_CONTEXT_NR )
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
								hasRelationWordExistingRelationContext = relationWordItem->hasContextInWord( existingRelationContextNr );

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
								existingSpecificationItem->isSelfGeneratedConclusion() ) ||

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

										( nonCompoundSpanishSpecificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&
										( foundSpecificationItem = generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( false, true, nonCompoundSpanishSpecificationCollectionNr, specificationWordItem ) ) != NULL )
											// Need to copy context (5)
											contextResult.copiedRelationContextNr = foundSpecificationItem->relationContextNr();
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
											if( existingSpecificationItem->specificationWordItem() == specificationWordItem )
												contextResult.contextNr = foundRelationContextNr;
											}
										else
											{
											if( isPossessive &&
											hasCompoundUserSpecificationCollection == isSelfGeneratedAssumption &&
											existingSpecificationItem->isSelfGeneratedConclusion() )
												contextResult.contextNr = relationWordItem->contextNr( specificationWordItem );
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
													( tempSpecificationItem = generalizationWordItem->bestAssumptionLevelSpecificationItem( isPossessive, specificationWordItem ) ) != NULL &&
													( foundContextNr = tempSpecificationItem->relationContextNr() ) != existingRelationContextNr )
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

						( isSelfGeneratedAssumption ||

						( !hasExistingSpecificationOnlyOneRelationWord &&
						hasGeneralizationWordCurrentlyConfirmedSpecification ) ||

						( !hasSpecificationCollection &&
						isPossessive &&

						( hasExistingSpecificationOnlyOneRelationWord ||
						relationWordItem->hasContextInWord( existingRelationContextNr ) ) ) ) ) ||

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
									contextResult.contextNr = foundContextNr;
								}
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
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasRelationWordConfirmedSpecification;
		bool isAmbiguousRelationContext = false;
		bool isAssignedOrClear;
		bool isDefinitionSpecification;
		bool isExistingOlderSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationContextAlreadyDefined = false;
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		bool isSelfGeneratedAssumption;
		bool isSpanishCurrentLanguage = false;
		bool isSpecificationWordSpanishAmbiguous = false;
		unsigned int assignmentSpecificationCollectionNr;
		unsigned int copiedRelationContextNr = NO_CONTEXT_NR;
		unsigned int existingRelationContextNr = NO_CONTEXT_NR;
		JustificationItem *firstHiddenJustificationItem;
		SpecificationItem *createdAssignmentItem = NULL;
		SpecificationItem *createdOrFoundAssignmentItem;
		SpecificationItem *createdSpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *hiddenSpanishSpecificationItem;
		SpecificationItem *replacedAssignmentItem;
		SpecificationItem *reverseSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *currentContextWordItem;
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
			isExistingOlderSpecification = existingSpecificationItem->isOlderItem();
			existingRelationContextNr = existingSpecificationItem->relationContextNr();

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
			relationWordItem->hasContextInWord( existingRelationContextNr ) ) ||

			// Existing specification is current
			( !isExistingOlderSpecification &&

			( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||

			( !isSpanishCurrentLanguage &&
			isSelfGenerated ) ) ) ||

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
					isSelfGeneratedAssumption = ( firstJustificationItem != NULL &&
												firstJustificationItem->hasAssumptionLevel() );

					// Get relation context (advanced)
					if( ( contextResult = getRelationContextAdvanced( hasRelationWordConfirmedSpecification, isAssignment, isArchivedAssignment, isPossessive, isSelfGeneratedAssumption, isSpecificationWordSpanishAmbiguous, specificationCollectionNr, generalizationWordItem, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to get the relation context (advanced)" );

					isAmbiguousRelationContext = contextResult.isAmbiguousRelationContext;
					copiedRelationContextNr = contextResult.copiedRelationContextNr;
					relationContextNr = contextResult.contextNr;

					if( !isValidContext( isPossessive, relationContextNr, specificationWordItem, relationWordItem ) )
						return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "I found an invalid context in word \"", relationWordItem->anyWordTypeString(), "\"" );
					}
				else
					isRelationContextAlreadyDefined = true;
				}

			// Add relation context
			if( relationWordItem->addContext( relationWordTypeNr, specificationWordTypeNr, relationContextNr, ( isSpecificationWordSpanishAmbiguous ? specificationCollectionNr : NO_COLLECTION_NR ), specificationWordItem ) != RESULT_OK )
				return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a relation context to word \"", relationWordItem->anyWordTypeString(), "\"" );

			// Typical for Spanish
			if( isSpecificationWordSpanishAmbiguous &&
			isSelfGenerated &&
			!generalizationWordItem->isUserGeneralizationWord &&
			specificationWordItem->isUserSpecificationWord &&
			userSpecificationItem_ != NULL &&
			userSpecificationItem_->hasRelationContext() &&

			( hiddenSpanishSpecificationItem = generalizationWordItem->firstUnhiddenSpanishSpecificationItem() ) != NULL &&
			hiddenSpanishSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != NULL &&

			// Typical for Spanish
			// Spanish test file: "Complejo (19 - extraño)"
			( ( existingSpecificationItem != NULL &&

			( !existingSpecificationItem->hasCurrentCreationSentenceNr() ||
			!existingSpecificationItem->hasRelationContextCurrentlyBeenUpdated() ) ) ||

			( ( firstHiddenJustificationItem = hiddenSpanishSpecificationItem->firstJustificationItem() ) != NULL &&
			!firstHiddenJustificationItem->isOppositePossessiveSpecificationAssumption() ) ) )
				{
				// Write Spanish assumption that is not hidden anymore
				if( generalizationWordItem->writeUpdatedSpecification( false, false, false, false, false, false, true, hiddenSpanishSpecificationItem ) != RESULT_OK )
					return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to write a Spanish assumption that is not hidden anymore in word \"", generalizationWordItem->anyWordTypeString(), "\" that is not hidden anymore" );

				generalizationWordItem->hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = true;

				if( isPossessive &&
				( currentContextWordItem = adminItem_->firstContextWordItem( relationContextNr ) ) != NULL )
					{
					// Do for all context words with relation context
					do	{
						if( currentContextWordItem != generalizationWordItem &&
						( reverseSpecificationItem = currentContextWordItem->firstSpecificationItem( false, false, false, specificationWordItem ) ) != NULL &&
						reverseSpecificationItem->isSelfGeneratedSpecification() &&
						// Justification doesn't already exist in specification
						!reverseSpecificationItem->hasPrimarySpecificationJustification( hiddenSpanishSpecificationItem ) &&
						// Justification has at least the same assumption level
						reverseSpecificationItem->assumptionLevel() >= hiddenSpanishSpecificationItem->assumptionLevel() &&
						// Add reversible justification to unhidden specification
						reverseSpecificationItem->addJustificationToSpecification( false, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, hiddenSpanishSpecificationItem, NULL, NULL ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a reversible specification justification to the reverse specification" );
						}
					while( ( currentContextWordItem = currentContextWordItem->_nextContextWordItem( relationContextNr ) ) != NULL );
					}
				}
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

				if( ( createdSpecificationItem = createAndAssignReturnResult.createdSpecificationItem ) == NULL )
					{
					// Not created
					if( hasRelationWord &&
					!isSelfGenerated &&
					foundSpecificationItem != NULL )
						{
						userSpecificationItem_ = foundSpecificationItem;

						if( !isQuestion &&
						foundSpecificationItem->hasRelationContext() )
							simpleUserSpecificationItem_ = generalizationWordItem->firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem );
						}
					}
				else
					{
					if( !isSelfGenerated )
						{
						userSpecificationItem_ = createdSpecificationItem;

						if( !isQuestion &&
						createdSpecificationItem->hasRelationContext() )
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
								adminItem_->makeOnlyOptionLeftAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, createdSpecificationItem, generalizationWordItem ) != RESULT_OK )
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

									// Test file: "Indirectly answered questions - John is masculine"
									if( userSpecificationItem_->isSpecificationAdjective() &&
									userSpecificationItem_->hasNonCompoundSpecificationCollection() &&
									// Make indirectly answered question assumption
									adminItem_->makeIndirectlyAnsweredQuestionAssumption( isArchivedAssignment, generalizationWordTypeNr, createdSpecificationItem, generalizationWordItem, specificationWordItem ) != RESULT_OK )
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

								// Draw a possessive definition conclusion from a specification-generalization 'part of' sentence
								addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, false, isEveryGeneralization, isExclusiveGeneralization, false, isNegative, false, true, false, isCharacteristicFor, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
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
									// See Block 2 of the included challenge document, or https://www.mafait.org/block_2
									addSelfGeneratedSpecification( false, isAssignment, isArchivedAssignment, isSpecific, isEveryGeneralization, isExclusiveGeneralization, false, isNegative, !isSpecific, false, false, false, true, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
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

					if( ( createAndAssignCheckResult = assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, userAssumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString ) ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to assign the specification" );

					if( ( createdAssignmentItem = createAndAssignCheckResult.createdSpecificationItem ) != NULL )
						{
						if( !isArchivedAssignment_ ||
						createAndAssignReturnResult.createdSpecificationItem != NULL )
							{
							createAndAssignReturnResult.createdSpecificationItem = createdAssignmentItem;
							createAndAssignReturnResult.createdOrFoundSpecificationItem = createdAssignmentItem;
							}

						if( replacedAssignmentItem != NULL &&
						generalizationWordItem->replaceOrDeleteSpecification( replacedAssignmentItem, createdAssignmentItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to replace or delete an assignment in word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					if( isNegative &&
					generalizationWordItem->isUserGeneralizationWord &&
					createdAssignmentItem->hasSpecificationCollection() &&
					// Draw only option left noun conclusion
					adminItem_->drawOnlyOptionLeftNounConclusion( isInactiveAssignment, isArchivedAssignment, createdAssignmentItem->specificationCollectionNr(), generalizationContextNr, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw an only-option-left noun conclusion by negative assignment" );
					}

				if( !isSelfGenerated )
					{
					if( ( createdOrFoundAssignmentItem = ( createdAssignmentItem == NULL ? createAndAssignCheckResult.foundSpecificationItem : createdAssignmentItem ) ) != NULL )
						{
						userSpecificationItem_ = createdOrFoundAssignmentItem;

						if( !isQuestion &&
						createdOrFoundAssignmentItem->hasRelationContext() )
							simpleUserSpecificationItem_ = ( isArchivedAssignment_ ? createdOrFoundAssignmentItem : generalizationWordItem->firstSpecificationItem( isNegative, isPossessive, false, specificationWordItem ) );
						}

						// Find answers to questions
						if( !isQuestion &&
						specificationWordItem != NULL &&
						( createdOrFoundAssignmentItem = createAndAssignCheckResult.createdOrFoundSpecificationItem ) != NULL &&
						generalizationWordItem->findAnswersToQuestions( specificationWordItem->compoundCollectionNr(), createdOrFoundAssignmentItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to find answers to questions" );
						}
					}

				if( !isExclusiveSpecification &&
				!isSpecificationGeneralization &&
				specificationWordItem != NULL &&
				userSpecificationItem_ != NULL )
					{
					if( hasRelationWord &&
					isSelfGenerated &&

					// Avoid looping of reversible conclusions
					( !isPossessive &&

					( !specificationWordItem->isUserSpecificationWord ||
					isSpanishCurrentLanguage ) ) &&

					// Draw reversible conclusions
					adminItem_->drawReversibleConclusions( userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

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
							createdAssignmentItem != NULL )
								{
								assignmentSpecificationCollectionNr = createdAssignmentItem->specificationCollectionNr();
									
								// Check if specification doesn't exist yet
								if( generalizationWordItem->bestMatchingSpecificationWordSpecificationItem( true, true, false, false, isNegative, isPossessive, assignmentSpecificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == NULL &&
								// Make suggestive question assumption
								adminItem_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, assignmentSpecificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, createdAssignmentItem, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
									return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make a suggestive question assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
								}
							}
						else
							{
							if( isDefinitionSpecification &&
							!isPossessive &&
							foundSpecificationItem == NULL &&
							!userSpecificationItem_->hasSpecificationCollection() &&
							generalizationWordItem->firstExclusiveSpecificationItem( true ) == NULL &&
							// Draw specification-generalization conclusion
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
		isArchivedAssignment_ = false;
		isChineseCurrentLanguage_ = false;

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
		bool hasRelationWord = ( startRelationReadItem != NULL );
		bool isAmbiguousRelationContext = false;
		bool isChineseCurrentLanguage = adminItem_->isChineseCurrentLanguage();
		bool isChineseReasoning = false;
		bool isCurrentNounValue;
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
		bool isText;
		bool isValueSpecificationWord = false;
		bool isWaitingForRelation = false;
		bool isWaitingForText = false;
		unsigned short currentSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short generalizationWordTypeNr;
		unsigned short linkedSpecificationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short valueGeneralizationWordTypeNr = NO_WORD_TYPE_NR;
		unsigned short wordOrderNr;
		unsigned int nContextRelations = 0;
		unsigned int nUserRelationWords = globalVariables_->nUserRelationWords;
		unsigned int userSpecificationCollectionNr;
#ifdef _MSC_VER
		int intValue;		// Required by Visual Studio
#endif
		ReadItem *currentReadItem;
		ReadItem *firstRelationReadItem;
		ReadItem *lastGeneralizationReadItem;
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *previousUserSpecificationItem = NULL;
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
		if( linkedSpecificationWordTypeNr > NO_WORD_TYPE_NR &&
		linkedSpecificationWordItem != NULL )
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

			// Add Chinese user specification with relation
			if( addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, NO_SELECTION_LEVEL, NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, linkedSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, linkedSpecificationWordItem ) != RESULT_OK )
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
					intValue = sscanf( readWordTypeString, "%u", &nContextRelations );
#endif
#ifndef _MSC_VER
				sscanf( readWordTypeString, "%u", &nContextRelations );
#endif
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

								if( adminItem_->createSelectionPart( isAction, false, isInactiveAssignment, isArchivedAssignment, false, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, true, userAssumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, valueGeneralizationWordItem, currentSpecificationWordItem, NULL, specificationString ) != RESULT_OK )
									return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a value selection item" );

								// Value, but no relation
								if( addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, ( isConditional ? false : isNegative ), isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, true, prepositionParameter, questionParameter, userAssumptionLevel, valueGeneralizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, valueGeneralizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ).result != RESULT_OK )
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
										if( addUserSpecificationWithRelation( isAction, currentReadItem->isAdjectiveAssignedOrEmpty(), isAssignment, isCharacteristicFor, false, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, selectionLevel, selectionListNr, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
											return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to create a selection part with relation" );
										}
									else	// No relation word
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

									// Existing user specification
									( ( existingSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, currentSpecificationWordItem ) ) == NULL ||
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
											addUserSpecificationWithRelation( false, false, isAssignment, isCharacteristicFor, isChineseReasoning, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, ( isAmbiguousRelationContext || isExclusiveSpecification ), ( isConditional ? false : isNegative ), false, isPartOf, isPossessive, isSpanishCurrentLanguage, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, questionParameter, NO_SELECTION_LEVEL, NO_LIST_NR, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, userRelationContextNr_, nContextRelations, nUserRelationWords, startRelationReadItem, endRelationReadItem, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
												return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to add a user specification with a relation to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
											}
										else	// No relation word
											{
											isNegativeSpecification = ( isConditional ? false : isNegative );

											// No selection, no value, no relation
											if( ( createAndAssignResult = addSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, false, isExclusiveSpecification, isNegativeSpecification, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, currentSpecificationWordTypeNr, linkedGeneralizationWordTypeNr_, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, nContextRelations, NULL, generalizationWordItem, currentSpecificationWordItem, linkedGeneralizationWordItem_, specificationString ) ).result != RESULT_OK )
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
													else
														{
														if( !isAssignment &&
														!isPossessive &&
														!isQuestion )
															{
															if( isGeneralizationProperNoun )
																{
																// Avoid idle call
																if( currentSpecificationWordItem->firstNegativeSpecificationItem() != NULL &&
																// Draw simple negative conclusions
																adminItem_->drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															else
																{
																if( currentSpecificationWordTypeNr == WORD_TYPE_ADJECTIVE &&
																// Feminine or masculine specification word
																currentSpecificationWordItem->isFeminineOrMasculineWord() &&
																( adjectiveSpecificationWordItem = adminItem_->predefinedWordItem( currentSpecificationWordItem->isFeminineWord() ? WORD_PARAMETER_ADJECTIVE_MASCULINE : WORD_PARAMETER_ADJECTIVE_FEMININE ) ) != NULL &&
																// For now, limit the negative specification adjective words
																globalVariables_->nUserSpecificationWords == 1 &&
																// Draw negative user conclusions
																adminItem_->drawNegativeUserConclusions( isArchivedAssignment, generalizationWordTypeNr, createdSpecificationItem, generalizationWordItem, adjectiveSpecificationWordItem, currentSpecificationWordItem ) != RESULT_OK )
																	return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw negative user conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );
																}
															}
														}
													}

												if( previousUserSpecificationItem != NULL &&
												!previousUserSpecificationItem->hasSpecificationCollection() &&
												// Collect previous specification
												previousUserSpecificationItem->collectSpecification( isExclusiveSpecification, createdSpecificationItem->specificationCollectionNr() ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to collect the previous specification of generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												previousSpecificationWordItem = currentSpecificationWordItem;
												previousUserSpecificationItem = createdSpecificationItem;
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
														if( ( ( isGeneralizationProperNoun &&
														userSpecificationItem_->hasNonCompoundSpecificationCollection() ) ||

														( ( !isGeneralizationProperNoun ||
														isSpanishCurrentLanguage ) &&

														userSpecificationItem_->hasSpecificationCollection() ) ) &&

														// Avoid idle call
														globalVariables_->firstUserDefinedProperNounWordItem != NULL &&
														// Draw proper noun part-of conclusions
														adminItem_->drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, ( isGeneralizationProperNoun ? currentSpecificationWordItem : generalizationWordItem ), NULL ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );

														if( !isSpecificationGeneralization &&
														generalizationWordItem != currentSpecificationWordItem &&

														( ( isEveryGeneralization &&

														( !isExclusiveSpecification ||
														userSpecificationItem_->hasCompoundSpecificationCollection() ) ) ||

														userSpecificationItem_->isSpecificationPluralNoun() ) &&

														// Make a specification substitution 'part of' assumption
														adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
														}

													// Avoid idle call
													if( currentSpecificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL &&
													// Existing user specification
													( existingSpecificationItem = generalizationWordItem->firstUserSpecificationItem( false, false, NO_COLLECTION_NR, NO_CONTEXT_NR, currentSpecificationWordItem ) ) != NULL &&

													( userSpecificationItem_->hasSpecificationCollection() ||
													existingSpecificationItem->isExclusiveSpecification() ) &&

													// Draw compound specification substitution conclusion
													adminItem_->drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, existingSpecificationItem, NULL, userSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
														return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );
													}

												if( ( !isEveryGeneralization ||

												( !isExclusiveSpecification &&
												userSpecificationItem_ != NULL &&
												!userSpecificationItem_->hasGeneralizationCollection() &&
												!userSpecificationItem_->hasSpecificationCollection() ) ) &&

												( !hasRelationWord ||
												isChineseCurrentLanguage ||
												isSpanishCurrentLanguage ||

												( !isArchivedAssignment &&
												userSpecificationItem_ != NULL &&
												userSpecificationItem_->isAssignment() ) ) &&

												// Avoid idle call
												currentSpecificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL &&
												// Draw a specification substitution conclusion or ask a question
												adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, currentSpecificationWordTypeNr, NO_WORD_TYPE_NR, generalizationContextNr, specificationContextNr, generalizationWordItem, currentSpecificationWordItem, NULL ) != RESULT_OK )
													return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a specification substitution conclusion or ask a question (normal) for word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( userSpecificationItem_ != NULL )
													{
													if( isGeneralizationProperNoun )
														{
														// Typical for Spanish
														// Spanish test file: "Definición sencilla de la familia - Juan y Ana - Pablo, José y Luisa"
														if( isSpanishCurrentLanguage &&
														!isSpanishAmbiguousSpecificationWord &&
														!isExclusiveSpecification &&
														simpleUserSpecificationItem_ != NULL &&
														simpleUserSpecificationItem_->hasNonCompoundSpecificationCollection() &&
														( userSpecificationCollectionNr = simpleUserSpecificationItem_->specificationCollectionNr() ) > NO_COLLECTION_NR &&
														( collectionWordItem = currentSpecificationWordItem->collectionWordItem( userSpecificationCollectionNr ) ) != NULL &&
														collectionWordItem->isNounWordSpanishAmbiguous() &&
														( additionalDefinitionSpecificationItem = collectionWordItem->firstSpecificationItem( false, false, false, collectionWordItem ) ) != NULL &&
														// Draw simple negative Spanish conclusions
														adminItem_->askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, currentSpecificationWordTypeNr, generalizationContextNr, specificationContextNr, simpleUserSpecificationItem_, additionalDefinitionSpecificationItem, simpleUserSpecificationItem_, generalizationWordItem, currentSpecificationWordItem ) != RESULT_OK )
															return adminItem_->addError( functionNameString, moduleNameString_, "I failed to simple negative Spanish conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\" with specification word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
														}
													else	// Definition
														{
														// Avoid idle call
														if( generalizationWordItem->firstSpecificationGeneralizationItem( true ) != NULL )
															{
															// Draw compound specification substitution conclusion
															if( ( compoundResult = adminItem_->drawCompoundSpecificationSubstitutionConclusion( generalizationWordTypeNr, userSpecificationItem_, generalizationWordItem ) ).result != RESULT_OK )
																return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

															compoundGeneralizationWordItem = compoundResult.compoundGeneralizationWordItem;
															}
														}
													}
												}

											if( isGeneralizationProperNoun &&
											!isUncountableGeneralizationNoun &&
											userSpecificationItem_ != NULL )
												{
												if( ( !isAssignment ||
												!userSpecificationItem_->isOlderItem() ||
												// Typical for Chinese
												isChineseCurrentLanguage ) &&

												// Avoid idle call
												globalVariables_->firstContextWordItem != NULL &&
												// Draw reversible conclusions
												adminItem_->drawReversibleConclusions( userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
													return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

												if( startRelationReadItem != NULL &&
												globalVariables_->nUserRelationWords == 1 &&
												userSpecificationItem_->hasSpecificationCollection() &&
												userSpecificationItem_->hasNewInformation() &&
												( firstRelationReadItem = startRelationReadItem->firstRelationWordReadItem() ) != NULL &&
												( relationWordItem = firstRelationReadItem->readWordItem() ) != NULL &&
												// Avoid idle call
												relationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, !isPossessive, currentSpecificationWordItem, generalizationWordItem ) != NULL &&
												// Make an exclusive specification substitution assumption
												adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, firstRelationReadItem->wordTypeNr(), currentSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, relationWordItem, currentSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
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

					if( !isQuestion &&
					currentSpecificationWordItem != NULL &&
					linkedSpecificationWordItem != NULL &&
					userSpecificationItem_ != NULL &&

					( isRelationWord ||
					isSpecificationWord ) &&

					( !hasRelationWord ||
					linkedSpecificationWordItem != currentSpecificationWordItem ) )
						{
						relationWordItem = ( hasRelationWord ? currentSpecificationWordItem : NULL );

						if( isGeneralizationReasoningWordType &&
						!isPreposition &&
						!isValueSpecificationWord &&
						linkedSpecificationWordTypeNr != WORD_TYPE_ADJECTIVE &&

						// Proper noun specification
						( ( isGeneralizationProperNoun &&
						!isNegative &&

						( userSpecificationItem_->hasSpecificationCollection() ||
						isSpanishCurrentLanguage ) ) ||

						// Test files: "Boiling point" and "Condensation point"
						( !isEveryGeneralization &&
						isPossessive &&

						( userSpecificationItem_->isUncountableGeneralizationNoun() ||
						// Typical for Spanish and French
						userSpecificationItem_->isAssignment() ) ) ) &&

						// Avoid idle call
						generalizationWordItem->bestMatchingRelationContextNrSpecificationItem( true, true, false, isPossessive, linkedSpecificationWordItem, relationWordItem ) != NULL &&
						// Make an exclusive specification substitution assumption
						adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, linkedSpecificationWordTypeNr, currentSpecificationWordTypeNr, userSpecificationItem_, generalizationWordItem, linkedSpecificationWordItem, relationWordItem ) != RESULT_OK )
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
							userRelationContextNr_ > NO_CONTEXT_NR &&
							userSpecificationItem_->hasNewInformation() &&
							// Possessive user specification doesn't exist
							currentSpecificationWordItem->firstUserSpecificationItem( false, true, NO_COLLECTION_NR, NO_CONTEXT_NR, linkedSpecificationWordItem ) == NULL )
								{
								if( ( isAssignment ||
								isPossessive ||
								// Typical for Chinese
								isChineseCurrentLanguage ) &&

								// Draw reversible conclusions
								adminItem_->drawReversibleConclusions( userSpecificationItem_, generalizationWordItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to draw reversible conclusions about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

								if( !isExclusiveSpecification &&
								// User specification doesn't exist
								userSpecificationItem_->hasSpecificationCollection() &&
								// Make an exclusive specification substitution assumption
								adminItem_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, false, !isPossessive, false, currentSpecificationWordTypeNr, linkedSpecificationWordTypeNr, generalizationWordTypeNr, userSpecificationItem_, currentSpecificationWordItem, linkedSpecificationWordItem, generalizationWordItem ) != RESULT_OK )
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
				if( generalizationWordItem->recalculateAssumptionLevelsInWord() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( selectionListNr, functionNameString, moduleNameString_, "I failed to recalculate the assumption levels of specifications in word \"", generalizationWordItem->anyWordTypeString(), "\"" );
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

	signed char recalculateAssumptionLevelsInTouchedWords()
		{
		WordItem *currentTouchedWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "recalculateAssumptionLevelsInTouchedWords";

		if( ( currentTouchedWordItem = globalVariables_->firstTouchedWordItem ) != NULL )
			{
			// Do for all proper noun words touched during current sentence
			do	{
				if( currentTouchedWordItem->recalculateAssumptionLevelsInWord() != RESULT_OK )
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
				// Specification word is required for Chinese
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
			if( adminItem_->nContextWords( foundRelationContextNr ) >= globalVariables_->nUserRelationWords )
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

	CreateAndAssignResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNounEnding, bool isAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, unsigned short assumptionLevel, unsigned short justificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasDisplayedIntegrityWarning = false;
		bool hasRelationWordConfirmedSpecification = false;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isOlderFoundSpecification;
		bool isSelectedUniqueUserRelation = ( justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION );
		bool isSpecificationWordSpanishAmbiguous;
		bool isUserGeneralizationWord;
		unsigned int calculatedAssumptionLevel;
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

		if( isEveryGeneralization &&
		isGeneralizationProperNoun )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word type is proper noun, while the specification is indicated as a definition" );

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
			if( ( calculatedAssumptionLevel = adminItem_->assumptionLevel( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem ) ) > MAX_LEVEL )
				return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "Assumption level overflow" );

			if( calculatedAssumptionLevel > NO_ASSUMPTION_LEVEL )
				assumptionLevel = (unsigned short)calculatedAssumptionLevel;
			}

		if( ( createAndAssignResult = addSpecification( isAssignment, false, isArchivedAssignment, isCharacteristicFor, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isSelectedUniqueUserRelation, false, prepositionParameter, questionParameter, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, createdOrFoundJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, NULL ) ).result != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification" );

		// No conflict found
		if( !globalVariables_->hasDisplayedWarning )
			{
			isUserGeneralizationWord = generalizationWordItem->isUserGeneralizationWord;
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			createdSpecificationItem = createAndAssignResult.createdSpecificationItem;
			foundSpecificationItem = createAndAssignResult.foundSpecificationItem;

			if( createdSpecificationItem == NULL )
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

					isOlderFoundSpecification = foundSpecificationItem->isOlderItem();

					// A justification has been created, but the self-generated specification already exists
					// So, the justification needs to be added or replaced separately

					if( relationWordItem == NULL ||

					// No justification is created
					// Test files: "Complex (6)" and "John is a parent - Paul is a child of John and Anna"
					( createdJustificationItem == NULL &&
					!isOlderFoundSpecification &&
					isPossessive &&
					!foundSpecificationItem->hasJustification( foundJustificationItem ) ) ||

					// Justification is created
					( createdJustificationItem != NULL &&

					// Typical for Spanish
					( ( isSpecificationWordSpanishAmbiguous &&
					justificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ||

					( foundSpecificationItem->isSelfGeneratedAssumption() &&
					!foundSpecificationItem->hasPrimaryAndSecondarySpecificationJustification( primarySpecificationItem, secondarySpecificationItem ) &&

					( justificationTypeNr != JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ||

					( secondarySpecificationItem != NULL &&

					( !secondarySpecificationItem->isSelfGeneratedAssumption() ||
					relationWordItem->firstSelfGeneratedCheckSpecificationItem( false, secondarySpecificationItem->isArchivedAssignment(), secondarySpecificationItem->isNegative(), secondarySpecificationItem->isPossessive(), false, secondarySpecificationItem->specificationWordItem(), generalizationWordItem ) == NULL ) ) ) ) ||

					( ( assumptionLevel == NO_ASSUMPTION_LEVEL ||

					( isUserGeneralizationWord &&
					justificationTypeNr == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) &&

					!foundSpecificationItem->hasPrimarySpecificationJustification( primarySpecificationItem ) ) ) ) )
						{
						// Process justification
						if( processJustification( hasRelationWordConfirmedSpecification, isOlderFoundSpecification, isUserGeneralizationWord, justificationTypeNr, createdOrFoundJustificationItem, firstJustificationItem, foundSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to process justifications of the found specification" );

						foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();
						}
					}
				}
			else
				{
				if( foundSpecificationItem != NULL &&
				foundSpecificationItem->hasRelationContext() &&
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
					createdSpecificationItem != NULL &&
					// Draw a unique relation conclusion
					addSelfGeneratedSpecification( false, false, false, false, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, true, false, true, false, false, false, false, NO_ASSUMPTION_LEVEL, JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, generalizationContextNr, specificationContextNr, NO_CONTEXT_NR, createdSpecificationItem, NULL, NULL, NULL, generalizationWordItem, specificationWordItem, NULL ).result != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a self-generated unique specification to word \"", generalizationWordItem->anyWordTypeString(), "\"" );
					}
				else
					{
					if( isGeneralizationProperNoun )
						{
						if( relationWordItem == NULL &&
						questionParameter == NO_QUESTION_PARAMETER &&
						// Avoid idle call
						specificationWordItem->firstNegativeSpecificationItem() != NULL &&
						// Draw simple negative conclusions
						adminItem_->drawSimpleNegativeConclusions( isArchivedAssignment, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw simple negative conclusions about generalization word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						if( ( !isAssignment ||
						isArchivedAssignment ) &&

						( !isExclusiveSpecification ||
						isSpecificationWordSpanishAmbiguous ||

						( createdSpecificationItem != NULL &&
						createdSpecificationItem->hasNonCompoundSpecificationCollection() ) ) &&

						// Draw proper noun part-of conclusions
						adminItem_->drawProperNounPartOfConclusionsInProperNounWords( isArchivedAssignment, specificationWordItem, ( isSpecificationWordSpanishAmbiguous ? relationWordItem : NULL ) ) != RESULT_OK )
							return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw proper noun 'part of' conclusions" );
						}

					if( !isExclusiveSpecification &&
					foundSpecificationItem == NULL &&
					generalizationWordItem != specificationWordItem &&
					simpleUserSpecificationItem_ == NULL &&
					createdSpecificationItem != NULL &&
					createdSpecificationItem->hasSpecificationCollection() &&
					// Make a specification substitution 'part of' assumption
					adminItem_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to make a specification substitution 'part of' assumption about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					// Test file: "Correcting invalidated assumption (by opposite suggestive question)"
					if( isGeneralizationProperNoun &&
					isUserGeneralizationWord &&
					createdSpecificationItem != NULL &&
					generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion() &&
					// Draw compound specification substitution conclusion
					adminItem_->drawCompoundSpecificationSubstitutionConclusion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, userSpecificationItem_, createdSpecificationItem, userSpecificationItem_, generalizationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to draw a compound specification substitution conclusion about word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					if( userSpecificationItem_ != NULL &&

					// Created specification
					( ( createdSpecificationItem != NULL &&

					( isExclusiveSpecification ||
					isSpecificationWordSpanishAmbiguous ||
					justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
					justificationTypeNr == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ||

					( isArchivedAssignment &&
					!isSpecificationGeneralization ) ||

					( isUserGeneralizationWord &&
					foundSpecificationItem == NULL &&

					( justificationTypeNr != JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||

					userSpecificationItem_->hasCompoundSpecificationCollection() ) ) ||

					( justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION &&
					adminItem_->isSpanishCurrentLanguage() ) ) ) ||

					// Found specification
					( foundSpecificationItem != NULL &&
					justificationTypeNr != JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION &&
					!foundSpecificationItem->hasCompoundSpecificationCollection() &&

					( ( !isUserGeneralizationWord &&
					foundSpecificationItem->hasSpecificationCollection() ) ||

					( foundSpecificationItem->isOlderItem() &&

					( ( foundSpecificationItem->hasCurrentCreationSentenceNr() &&
					userSpecificationItem_->hasRelationContext() ) ||

					userSpecificationItem_->isSpecificationWordSpanishAmbiguous() ) ) ) ) ) &&

					// Avoid idle call
					specificationWordItem->firstNonNegativeNonPossessiveDefinitionSpecificationItem() != NULL &&
					// Draw a specification substitution conclusion or ask a question
					adminItem_->drawSpecificationSubstitutionConclusionOrAskQuestion( isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
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
