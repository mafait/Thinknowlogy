/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

#include "SpecificationList.cpp"
#include "UserSpecificationResultType.cpp"
#include "WordTypeItem.cpp"

class WordSpecification
	{
	friend class WordItem;

	// Private constructed variables

	bool hasCurrentlyConfirmedSpecification_;
	bool hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_;
	bool hasCurrentlyConfirmedSpecificationButNoRelation_;
	bool hasCorrectedAssumptionByKnowledge_;
	bool hasCorrectedAssumptionByOppositeQuestion_;
	bool hasDisplayedMoreSpecificSpecification_;
	bool hasDisplayedMoreSpecificNonExclusiveSpecification_;
	bool hasDisplayedMoreSpecificQuestion_;
	bool isOnlyCheckingForConflicts_;
	bool isWordTouchedDuringCurrentSentence_;

	unsigned int spanishCompoundSpecificationCollectionNr_;
	unsigned int userSpecificationCollectionNr_;

	SpecificationItem *correctedAssumptionReplacedSpecificationItem_;
	SpecificationItem *replacedAssignmentItem_;

	WordItem *spanishCompoundSpecificationWordItem_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH];

	GlobalVariables *globalVariables_;
	InputOutput *inputOutput_;
	WordItem *myWordItem_;


	// Private functions

	void addMyWordToTouchedDuringCurrentSentenceList()
		{
		WordItem *lastTouchedWordItem = globalVariables_->firstTouchedWordItem;
		WordItem *tempTouchedWordItem;

		if( !isWordTouchedDuringCurrentSentence_ )
			{
			isWordTouchedDuringCurrentSentence_ = true;

			if( lastTouchedWordItem == NULL )
				globalVariables_->firstTouchedWordItem = myWordItem_;
			else
				{
				// Word order is important: Add word at end of temporary touched word list
				while( ( tempTouchedWordItem = lastTouchedWordItem->nextTouchedWordItem ) != NULL )
					lastTouchedWordItem = tempTouchedWordItem;

				lastTouchedWordItem->nextTouchedWordItem = myWordItem_;
				}
			}
		}

	unsigned int nonCompoundCollectionNrInCollectionWords( unsigned int compoundCollectionNr )
		{
		unsigned int nonCompoundCollectionNr;
		WordItem *currentCollectionWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		( currentCollectionWordItem = globalVariables_->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( ( nonCompoundCollectionNr = currentCollectionWordItem->nonCompoundCollectionNrInWord( compoundCollectionNr ) ) > NO_COLLECTION_NR )
					return nonCompoundCollectionNr;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return NO_COLLECTION_NR;
		}

	signed char addGeneralization( bool isRelation, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *specificationWordItem )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addGeneralization";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( boolResult = specificationWordItem->findGeneralization( isRelation, myWordItem_ ) ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a generalization item" );

		// Generalization not found
		if( !boolResult.booleanValue &&
		// Specification word of a generalization word: noun
		specificationWordItem->createGeneralizationItem( isRelation, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_ ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a generalization item" );

		return RESULT_OK;
		}

	signed char checkUserQuestion( bool hasRelationContext, bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, SpecificationItem *foundQuestionSpecificationItem, WordItem *specificationWordItem )
		{
		SpecificationItem *relatedSpecificationItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkUserQuestion";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( foundQuestionSpecificationItem != NULL &&
		foundQuestionSpecificationItem->isAnsweredQuestion() &&
		foundQuestionSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification &&
		foundQuestionSpecificationItem->relationContextNr() == relationContextNr )
			globalVariables_->isQuestionAlreadyAnswered = true;

		if( foundQuestionSpecificationItem == NULL ||
		!foundQuestionSpecificationItem->isQuestion() )
			{
			if( specificationCollectionNr > NO_COLLECTION_NR &&
			( relatedResult = findRelatedSpecification( false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related question" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
			!isExclusiveSpecification &&
			!isSpecificationGeneralization )
				{
				if( relatedSpecificationItem->isOlderItem() )
					{
					if( hasRelationContext )
						{
						if( correctedAssumptionReplacedSpecificationItem_ != NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The corrected assumption replaced specification item already assigned" );

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_->firstSelfGeneratedCheckSpecificationItem( true, true, relatedSpecificationItem->isNegative(), relatedSpecificationItem->isPossessive(), true, NO_QUESTION_PARAMETER, relatedSpecificationItem->specificationCollectionNr(), relatedSpecificationItem->relationContextNr(), relatedSpecificationItem->specificationWordItem() ) ) != NULL )
							{
							if( myWordItem_->writeUpdatedSpecification( false, false, true, false, false, correctedAssumptionReplacedSpecificationItem_ ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a conflicting specification" );

							hasCorrectedAssumptionByOppositeQuestion_ = true;
							}
						}
					}
				else
					{
					if( relatedSpecificationItem->hasCompoundSpecificationCollection() )
						{
						// Current question in conflict with definition
						if( writeSpecificationInConflictWithDefinition( true, specificationWordItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write that the current question is in conflict with a definition" );

						globalVariables_->isConflictingQuestion = true;
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char collectSpecifications( bool isExclusiveGeneralization, bool isQuestion, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		unsigned int specificationCollectionNr;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundGeneralizationWordItem;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectSpecifications";

		if( collectionWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given collection word item is undefined" );

		if( ( currentGeneralizationItem = myWordItem_->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startSystemErrorInWord( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( collectionResult = currentGeneralizationWordItem->collectSpecifications( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem ) ).result != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to add a specification collection by exclusive specification in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

				foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem;

				if( foundGeneralizationWordItem != NULL &&
				( specificationCollectionNr = collectionWordItem->collectionNr( currentGeneralizationWordItem ) ) > NO_COLLECTION_NR &&
				// Collect by generalization
				foundGeneralizationWordItem->collectSpecificationsInWord( isExclusiveGeneralization, isQuestion, specificationCollectionNr ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to collect specifications in word \"", foundGeneralizationWordItem->anyWordTypeString(), "\"" );
				}
			while( collectionResult.foundGeneralizationWordItem == NULL &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char processPreviouslyUnknownPluralNoun( WordItem *specificationWordItem )
		{
		WordItem *currentSpecificationWordItem;
		WordTypeItem *pluralNounWordTypeItem;
		const char *unknownPluralNounString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "processPreviouslyUnknownPluralNoun";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( pluralNounWordTypeItem = specificationWordItem->activeWordTypeItem( WORD_TYPE_NOUN_PLURAL ) ) != NULL &&
		// Skip announcement for new occurrences
		!pluralNounWordTypeItem->isOlderItem() )
			{
			if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START, pluralNounWordTypeItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a notification text" );

			if( ( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) != NULL &&
			( unknownPluralNounString = myWordItem_->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) ) != NULL )
				{
				// Do for all specification words
				do	{
					if( currentSpecificationWordItem->clearStoredSentenceStringWithUnknownPluralNoun( unknownPluralNounString, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to clear the stored sentence string with specification word \"", specificationWordItem->anyWordTypeString(), "\" in word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *createdSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isAssignment() )
			replacedAssignmentItem_ = obsoleteSpecificationItem;
		else
			{
			if( myWordItem_->replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a specification" );
			}

		return RESULT_OK;
		}

	signed char replaceObsoleteSpecification( bool isConfirmedSpecificationButNotItsRelation, SpecificationItem *confirmedSpecificationItem, SpecificationItem *createdSpecificationItem, SpecificationItem *obsoleteAssumptionSpecificationItem, SpecificationItem *obsoleteHiddenSpanishSpecificationItem, SpecificationItem *obsoleteSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceObsoleteSpecification";

		if( createdSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given created specification is undefined" );

		if( confirmedSpecificationItem != NULL &&
		!createdSpecificationItem->isHiddenSpanishSpecification() &&
		// Replace confirmed specification
		replaceOrDeleteSpecification( confirmedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the confirmed specification" );

		if( isConfirmedSpecificationButNotItsRelation &&
		myWordItem_->confirmSpecificationButNotItsRelation( createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to confirm a specification, but not its relation(s)" );

		if( correctedAssumptionReplacedSpecificationItem_ != NULL )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a corrected assumption replaced specification" );

			correctedAssumptionReplacedSpecificationItem_ = NULL;
			}

		if( obsoleteAssumptionSpecificationItem != NULL )
			{
			if( !obsoleteAssumptionSpecificationItem->isConcludedAssumption() &&
			myWordItem_->attachJustificationInWord( obsoleteAssumptionSpecificationItem->firstJustificationItem(), createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to attach the first justification of the found assumption specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteAssumptionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete assumption specification" );
			}

		if( obsoleteHiddenSpanishSpecificationItem != NULL )
			{
			if( myWordItem_->attachJustificationInWord( obsoleteHiddenSpanishSpecificationItem->firstJustificationItem(), createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to attach the justifications of the found replaced hidden specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteHiddenSpanishSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete hidden Spanish specification" );
			}

		if( obsoleteSpecificationItem != NULL &&
		replaceOrDeleteSpecification( obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete specification" );

		return RESULT_OK;
		}

	signed char writeMoreSpecificSpecification( SpecificationItem *olderSpecificationItem )
		{
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeMoreSpecificSpecification";

		if( olderSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given older specification item is undefined" );

		if( !olderSpecificationItem->isOlderItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given older specification item isn't old" );

		if( myWordItem_->writeSelectedSpecification( false, !olderSpecificationItem->isExclusiveSpecification(), olderSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the given older specification" );

		if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
		strlen( writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given older specification" );

		if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( olderSpecificationItem->isQuestion() ? ( olderSpecificationItem->isSelfGenerated() ? INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem->isSelfGenerated() ? ( olderSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface listing text" );

		if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );

		return RESULT_OK;
		}

	signed char writeSpecificationInConflictWithDefinition( bool isQuestion, WordItem *specificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *definitionSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationInConflictWithDefinition";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentGeneralizationItem = specificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization noun specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startSystemErrorInWord( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( definitionSpecificationItem = currentGeneralizationWordItem->firstExclusiveSpecificationItem( specificationWordItem ) ) != NULL &&

				( !isQuestion ||
				definitionSpecificationItem->hasNonCompoundSpecificationCollection() ) &&

				// Write conflicting specification
				definitionSpecificationItem->writeSpecificationConflict( isQuestion ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an exclusive conflicting specification" );
				}
			while( isQuestion &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	RelatedResultType findRelatedSpecification( bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		bool isFirstRelatedSpecification = true;
		bool isLastRelatedSpecification = false;
		unsigned int currentSpecificationSentenceNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( specificationCollectionNr == NO_COLLECTION_NR )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given specification collection number is undefined" );

		if( specificationWordItem == NULL )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) ) != NULL )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem->relatedSpecificationWordItem( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, relationContextNr ) ) != NULL )
					{
					currentSpecificationSentenceNr = currentSpecificationItem->creationSentenceNr();

					if( currentSpecificationWordItem == specificationWordItem &&
					currentSpecificationItem->relationContextNr() == relationContextNr )
						{
						// Found the given specification item
						isFirstRelatedSpecification = false;

						if( currentSpecificationSentenceNr >= highestSentenceNr )
							{
							isLastRelatedSpecification = true;
							highestSentenceNr = currentSpecificationSentenceNr;
							}
						}
					else
						{
						if( currentSpecificationSentenceNr >= highestSentenceNr )
							{
							isLastRelatedSpecification = false;
							highestSentenceNr = currentSpecificationSentenceNr;
							}

						relatedResult.isFirstRelatedSpecification = isFirstRelatedSpecification;
						relatedResult.relatedSpecificationItem = currentSpecificationItem;
						}
					}
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedSpecificationItem() ) != NULL );

			relatedResult.isLastRelatedSpecification = isLastRelatedSpecification;
			}

		return relatedResult;
		}

	SpecificationResultType checkSpecificationWordForConflict( bool isArchivedAssignment, bool isGeneralizationProperNoun, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		bool hasFoundCompoundConflict = false;
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isSpecificationWordSpanishAmbiguous;
		unsigned int compoundSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int nonCompoundSpecificationCollectionNr;
		SpecificationItem *anotherConflictingSpecificationItem;
		SpecificationItem *conflictingSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *pastTenseAssignmentItem = NULL;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *tempConflictingSpecificationItem;
		RelatedResultType relatedResult;
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkSpecificationWordForConflict";

		if( specificationWordItem == NULL )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Derive conflict
		if( hasSpecificationCollection &&
		( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

		if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) == NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			// Check for looping
			if( isSpecificationWordSpanishAmbiguous ||
			( conflictingSpecificationItem = specificationWordItem->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, myWordItem_ ) ) == NULL )
				{
				// Check for past tense
				foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

				if( foundSpecificationItem != NULL &&
				foundSpecificationItem->isArchivedAssignment() != isArchivedAssignment )
					conflictingSpecificationItem = foundSpecificationItem;
				else
					{
					if( !isSpecificationWordSpanishAmbiguous &&
					( compoundSpecificationCollectionNr = specificationWordItem->compoundCollectionNr() ) > NO_COLLECTION_NR )
						{
						// Check for negative
						foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, true, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

						if( foundSpecificationItem != NULL &&
						!specificationWordItem->isUserSpecificationWord &&
						foundSpecificationItem->specificationCollectionNr() == specificationCollectionNr )
							hasFoundCompoundConflict = true;

						if( !isNegative &&
						foundSpecificationItem != NULL &&

						( hasFoundCompoundConflict ||
						compoundSpecificationCollectionNr != specificationCollectionNr ||
						foundSpecificationItem->isUserSpecification() ) )
							{
							// First check other conflict
							if( ( anotherConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, NULL ) ) != NULL &&
							// Write conflict
							anotherConflictingSpecificationItem->writeSpecificationConflict( false ) != RESULT_OK )
								return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write another conflicting specification" );

							conflictingSpecificationItem = foundSpecificationItem;
							}
						else
							{
							// Check for existing compound specification
							// No self-generated question available for this specification
							if( ( nonCompoundSpecificationCollectionNr = nonCompoundCollectionNrInCollectionWords( compoundSpecificationCollectionNr ) ) > NO_COLLECTION_NR &&
							( tempConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, NULL ) ) != NULL &&
							tempConflictingSpecificationItem->specificationWordItem() != specificationWordItem )
								conflictingSpecificationItem = tempConflictingSpecificationItem;
							}
						}
					}
				}
			}
		else
			{
			// Find for possible past tense assignment of relatedSpecificationItem
			pastTenseAssignmentItem = myWordItem_->firstNonQuestionAssignmentItem( false, false, true, isNegative, isPossessive, relatedSpecificationItem->specificationWordItem() );
			conflictingSpecificationItem = ( pastTenseAssignmentItem == NULL ? relatedSpecificationItem : pastTenseAssignmentItem );
			}

		if( !hasFoundCompoundConflict &&
		conflictingSpecificationItem != NULL &&
		!conflictingSpecificationItem->hasSpecificationBeenWrittenAsConflict() )
			{
			if( conflictingSpecificationItem->isOlderItem() )
				{
				if( ( !hasSpecificationCollection ||
				isGeneralizationProperNoun ) &&

				// Write conflicting specification
				conflictingSpecificationItem->writeSpecificationConflict( false ) != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the conflicting specification" );
				}
			else
				{
				// Sentence can only be in conflict with itself if it has at least 2 specification words
				if( globalVariables_->nUserSpecificationWords > 1 &&
				!conflictingSpecificationItem->hasNonExclusiveSpecificationCollection() &&
				conflictingSpecificationItem->isUserSpecification() &&

				// Current sentence in conflict with definition
				writeSpecificationInConflictWithDefinition( false, specificationWordItem ) != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write that the current sentence is in conflict with a definition" );
				}
			}

		specificationResult.specificationItem = conflictingSpecificationItem;
		return specificationResult;
		}

	UserSpecificationResultType checkUserSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationProperNoun, bool isNegative, bool isPossessive, bool isSelection, bool isSpecificationWordSpanishAmbiguous, bool isValueSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, SpecificationItem *foundSpecificationItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundRelationContext;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool isFoundSpecificationExclusive;
		unsigned int foundRelationContextNr;
		JustificationItem *nextJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *predecessorObsoleteJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *oldUserSpecificationItem = NULL;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkUserSpecification";

		if( foundSpecificationItem == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		if( specificationWordItem == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( foundSpecificationItem->isValueSpecification() != isValueSpecification )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "Value specification conflict! Specification word \"", specificationWordItem->anyWordTypeString(), "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );

		isFoundSpecificationExclusive = foundSpecificationItem->isExclusiveSpecification();

		if( !hasRelationContext &&
		!isAssignment &&
		!isExclusiveSpecification &&
		isFoundSpecificationExclusive )
			{
			if( writeMoreSpecificSpecification( foundSpecificationItem ) != RESULT_OK )
				return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );

			hasDisplayedMoreSpecificNonExclusiveSpecification_ = true;
			userSpecificationResult.isNonExclusiveSpecification = true;

			if( ( relatedResult = findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
				return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find the related part of a more specific specification" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
			( obsoleteJustificationItem = myWordItem_->primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, relatedSpecificationItem ) ) != NULL &&
			( secondarySpecificationItem = obsoleteJustificationItem->secondarySpecificationItem() ) != NULL &&
			( assumptionSpecificationItem = myWordItem_->firstAssignmentOrSpecificationItem( true, true, false, false, false, secondarySpecificationItem->specificationWordItem() ) ) != NULL &&
			( predecessorObsoleteJustificationItem = assumptionSpecificationItem->firstJustificationItem() ) != NULL )
				{
				while( ( nextJustificationItem = predecessorObsoleteJustificationItem->attachedJustificationItem() ) != NULL &&
				nextJustificationItem != obsoleteJustificationItem )
					predecessorObsoleteJustificationItem = nextJustificationItem;

				if( ( justificationResult = myWordItem_->copyJustification( true, predecessorObsoleteJustificationItem->primarySpecificationItem(), predecessorObsoleteJustificationItem->secondarySpecificationItem(), NULL, predecessorObsoleteJustificationItem ) ).result != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to copyh the predecessor obsolete justification item" );

				if( justificationResult.createdJustificationItem == NULL )
					return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The created justification item is undefined" );

				if( myWordItem_->replaceJustification( predecessorObsoleteJustificationItem, justificationResult.createdJustificationItem, assumptionSpecificationItem ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item" );

				if( myWordItem_->replaceJustification( obsoleteJustificationItem, NULL, assumptionSpecificationItem ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item" );
				}
			}
		else
			{
			hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

			if( !isSelection &&
			hasRelationContext &&
			foundSpecificationItem->isOlderItem() )
				{
				if( foundSpecificationItem->isArchivedAssignment() == isArchivedAssignment &&
				// Check if negative specification exists
				myWordItem_->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, specificationWordItem ) == NULL )
					{
					if( hasFoundRelationContext )
						{
						if( isPossessive &&
						relationWordItem != NULL &&
						specificationWordItem->isFemale() != relationWordItem->isFemale() &&
						specificationWordItem->isMale() != relationWordItem->isMale() &&

						// Check for specification conflict
						checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
						}

					if( !isInactiveAssignment &&

					( !hasFoundRelationContext ||

					( !foundSpecificationItem->isExclusiveGeneralization() &&
					// An self-generated assumption was found,
					// but there is also a user speciciation without relation word
					( oldUserSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, false, isPossessive, false, NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != NULL &&
					!oldUserSpecificationItem->hasRelationContext() ) ) )
						{
						if( writeMoreSpecificSpecification( oldUserSpecificationItem == NULL ? foundSpecificationItem : oldUserSpecificationItem ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an notification about a more specific related specification" );

						hasDisplayedMoreSpecificSpecification_ = true;
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			if( !globalVariables_->hasDisplayedWarning )
				{
				foundRelationContextNr = foundSpecificationItem->relationContextNr();

				if( hasFoundRelationContext &&
				foundRelationContextNr != relationContextNr &&

				( !hasRelationContext ||
				// Current user specification has less relation words than the found specification
				globalVariables_->nUserRelationWords < myWordItem_->nContextWords( foundRelationContextNr, specificationWordItem ) ) )
					{
					if( !foundSpecificationItem->isHiddenSpanishSpecification() &&

					// Has new relation context
					( hasRelationContext ||
					myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, relationContextNr, specificationWordItem ) == NULL ) )
						{
						if( !hasRelationContext )
							{
							hasCurrentlyConfirmedSpecificationButNoRelation_ = true;
							userSpecificationResult.isConfirmedSpecificationButNotItsRelation = true;
							}
						else
							{
							hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = true;

							if( writeConfirmedSpecification( ( foundSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION ), foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed specification" );
							}
						}
					}
				else
					{
					if( hasRelationContext )
						{
						if( !hasFoundRelationContext ||

						( foundRelationContextNr == relationContextNr &&
						!foundSpecificationItem->isOlderItem() ) )
							// Confirmed relation words
							foundSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( hasFoundRelationContext, false, isNegative, isPossessive, true, NO_QUESTION_PARAMETER, specificationCollectionNr, foundRelationContextNr, specificationWordItem );
						}

					if( foundSpecificationItem == NULL )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						hasCurrentlyConfirmedSpecification_ &&
						( foundSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, true, specificationWordItem, relationWordItem ) ) != NULL &&
						( userSpecificationItem = myWordItem_->firstSpecificationItem( false, isPossessive, false, specificationWordItem ) ) != NULL )
							{
							// Write confirmed specification
							if( writeConfirmedSpecification( INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED, foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write another confirmed Spanish specification" );

							if( replaceOrDeleteSpecification( foundSpecificationItem, userSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete another confirmed Spanish specification" );
							}
						}
					else
						{
						// Confirmation: Replace a self-generated by a user-entered specification
						if( ( hasRelationContext ||
						!isFoundSpecificationExclusive ) &&

						foundSpecificationItem->isSelfGenerated() )
							{
							hasCurrentlyConfirmedSpecification_ = true;

							if( isFoundSpecificationExclusive )
								userSpecificationResult.isConfirmedExclusive = true;

							if( !foundSpecificationItem->isHiddenSpanishSpecification() &&
							// Write confirmed specification
							writeConfirmedSpecification( ( foundSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ), foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed specification" );

							userSpecificationResult.confirmedSpecificationItem = foundSpecificationItem;
							}

						if( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||

						( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
						foundSpecificationItem->isSpecificationSingularNoun() ) )
							{
							if( processPreviouslyUnknownPluralNoun( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? myWordItem_ : specificationWordItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to process a previously unknown plural noun" );

							userSpecificationResult.confirmedSpecificationItem = foundSpecificationItem;
							}
						}
					}
				}
			}

		return userSpecificationResult;
		}

	UserSpecificationResultType checkUserSpecificationOrQuestion( bool hasRelationContext, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationProperNoun, bool isNegative, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isSpecificationWordSpanishAmbiguous, bool isValueSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFeminineSingularNounEnding;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSingularNounGeneralizationWord = ( generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR );
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordTypeItem *foundWordTypeItem;
		WordTypeItem *singularNounWordTypeItem;
		char *writtenSentenceString;
		BoolResultType boolResult;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkUserSpecificationOrQuestion";

		hasCorrectedAssumptionByOppositeQuestion_ = false;

		if( specificationWordItem == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Skip if definition (singular noun) is Spanish ambiguous
		if( isSingularNounGeneralizationWord &&
		specificationWordItem != myWordItem_ &&
		// Check specification in opposite direction
		( boolResult = myWordItem_->findGeneralization( false, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find out if word \"", specificationWordItem->anyWordTypeString(), "\" is one of my generalization words" );

		// Has found generalization
		// User specification is looping
		if( boolResult.booleanValue )
			{
			if( checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != RESULT_OK )
				return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification looping conflict" );
			}
		else
			{
			// Check current assignment, specification or question (with relation)
			if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( true, true, true, true, true, isNegative, isPossessive, isQuestion, specificationCollectionNr, ( isQuestion ? NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) != NULL )
				{
				if( !isQuestion &&
				isArchivedAssignment != foundSpecificationItem->isArchivedAssignment() &&

				// Check for specification conflict
				checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
				}

			if( isQuestion &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

			if( foundSpecificationItem == NULL )
				{
				// Check current assignment or specification (without this relation)
				if( isQuestion )
					foundSpecificationItem = myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );
				else
					foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isNegative, isPossessive, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, specificationWordItem );

				if( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN )
					{
					// Exclusive specifications are not conflicting
					if( !isExclusiveSpecification &&
					// Possessive specifications are not conflicting
					!isPossessive &&
					foundSpecificationItem == NULL )
						{
						if( !myWordItem_->isFemaleOrMale() &&
						myWordItem_->hasFeminineProperNounEnding() != specificationWordItem->isFemale() &&
						myWordItem_->hasMasculineProperNounEnding() != specificationWordItem->isMale() &&
						inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NOUN_TO_BE, myWordItem_->anyWordTypeString(), ( myWordItem_->hasFeminineProperNounEnding() ? INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a proper noun expected to be feminine or masculine" );

						if( specificationCollectionNr > NO_COLLECTION_NR )
							{
							if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

							relatedSpecificationItem = relatedResult.relatedSpecificationItem;

							if( hasRelationContext &&
							relatedSpecificationItem != NULL &&
							relatedSpecificationItem->isSelfGeneratedAssumption() )
								{
								// Assumption needs to be corrected
								if( correctedAssumptionReplacedSpecificationItem_ != NULL )
									return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The corrected assumption specification item already assigned" );

								if( myWordItem_->writeUpdatedSpecification( false, true, false, false, false, relatedSpecificationItem ) != RESULT_OK )
									return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a conflicting specification" );

								hasCorrectedAssumptionByKnowledge_ = true;
								correctedAssumptionReplacedSpecificationItem_ = relatedSpecificationItem;
								}
							else
								{
								if( !isQuestion &&
								myWordItem_->isUserGeneralizationWord &&

								( relatedSpecificationItem == NULL ||

								( !specificationWordItem->isNounWordSpanishAmbiguous() ||
								!relatedSpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) &&

								// Skip checking for conflicts if a question with the current specification word exists
								myWordItem_->firstSpecificationItem( false, false, true, specificationWordItem ) == NULL )
									{
									// Check for specification conflict
									if( checkForSpecificationConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
										return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
									}
								}
							}
						}

					if( !globalVariables_->hasDisplayedWarning &&
					!hasRelationContext &&
					isAssignment &&
					!isArchivedAssignment &&
					!isValueSpecification &&
					specificationContextNr == NO_CONTEXT_NR &&
					specificationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&

					// Write warning
					inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MISSING_RELATION ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning about ambiguity" );
					}
				else
					{
					if( isSingularNounGeneralizationWord &&
					!myWordItem_->isFemaleOrMale() &&
					( foundWordTypeItem = myWordItem_->activeWordTypeItem( WORD_TYPE_NOUN_SINGULAR ) ) != NULL &&
					foundWordTypeItem->hasFeminineOrMasculineWordEnding() )
						{
						hasFeminineSingularNounEnding = foundWordTypeItem->hasFeminineWordEnding();

						if( !myWordItem_->isCorrectIndefiniteArticle( ( hasFeminineSingularNounEnding ? WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE : WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ), generalizationWordTypeNr ) )
							{
							foundWordTypeItem->clearIndefiniteArticleParameter();

							if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_SINGULAR_NOUN_TO_BE, myWordItem_->anyWordTypeString(), ( hasFeminineSingularNounEnding ? INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a singular noun expected to be feminine or masculine" );
							}
						}
					}
				}
			else
				{
				if( !hasRelationContext &&
				isAssignment &&
				generalizationContextNr == NO_CONTEXT_NR &&
				foundSpecificationItem->hasRelationContext() )
					{
					if( myWordItem_->writeSelectedSpecification( false, true, foundSpecificationItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
					strlen( writtenSentenceString ) == 0 )
						return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I couldn't write a sentence with an assumption about the relation" );

					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a missing relation" );

					if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					// Force 'I know' notification
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'I know' interface notification" );
					}
				}

			if( !globalVariables_->hasDisplayedWarning )
				{
				if( isQuestion )
					{
					if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the user question" );

					// Find self-generated question
					if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationContextNr ) ).result != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting self-generated question" );

					if( !isOnlyCheckingForConflicts_ &&
					globalVariables_->nUserSpecificationWords > 1 &&
					myWordItem_->sameUserQuestionSpecificationItem( questionParameter ) != NULL )
						{
						if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write 'I had the same question before' interface notification" );

						userSpecificationResult.isSameQuestionFromUser = true;
						}
					else
						{
						// Find user question
						if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationContextNr ) ).result != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting user question" );
						}

					isOnlyCheckingForConflicts_ = true;
					}
				else
					{
					if( foundSpecificationItem == NULL )
						{
						if( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
						// Find singular word type
						( singularNounWordTypeItem = specificationWordItem->activeWordTypeItem( WORD_TYPE_NOUN_SINGULAR ) ) != NULL &&
						// Skip currently created singular noun
						singularNounWordTypeItem->isOlderItem() &&

						processPreviouslyUnknownPluralNoun( specificationWordItem ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to process a previously unknown plural noun" );
						}
					else
						{
						if( ( userSpecificationResult = checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperNoun, isNegative, isPossessive, isSelection, isSpecificationWordSpanishAmbiguous, isValueSpecification, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, relationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the user specification" );
						}
					}
				}
			}

		return userSpecificationResult;
		}

	UserSpecificationResultType findSameSimilarOrRelatedQuestion( bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isOnlyCheckingForConflicts, bool isPossessive, bool isSelfGenerated, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr )
		{
		bool hasFoundOlderSpecification = false;
		bool hasFoundQuestion;
		bool hasFoundSpecificationRelationContext = false;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isConflictingQuestion = false;
		bool isRelatedQuestion = false;
		bool isSameAssignment = false;
		bool isSameExclusiveSpecification = false;
		bool isSameRelationContext = false;
		bool isSameQuestion = true;
		bool isSimilarQuestion = false;
		bool isUserSpecificationWord;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *foundQuestionSpecificationItem = NULL;
		WordItem *currentWordItem;
		char *writtenSentenceString;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findSameSimilarOrRelatedQuestion";

		if( ( currentWordItem = globalVariables_->firstWordItem ) == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The first word item is undefined" );

		// Do for all words, including predefined words
		do	{
			hasFoundQuestion = false;
			isUserSpecificationWord = currentWordItem->isUserSpecificationWord;

			if( ( currentQuestionSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, currentWordItem ) ) != NULL )
				{
				hasFoundQuestion = true;

				if( foundQuestionSpecificationItem == NULL ||
				foundQuestionSpecificationItem->isExclusiveSpecification() != currentQuestionSpecificationItem->isExclusiveSpecification() )
					{
					foundQuestionSpecificationItem = currentQuestionSpecificationItem;

					hasFoundOlderSpecification = foundQuestionSpecificationItem->isOlderItem();
					hasFoundSpecificationRelationContext = foundQuestionSpecificationItem->hasRelationContext();
					isSameAssignment = ( foundQuestionSpecificationItem->isAssignment() == isAssignment );
					isSameRelationContext = ( foundQuestionSpecificationItem->relationContextNr() == relationContextNr );
					isSameExclusiveSpecification = ( foundQuestionSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification );
					}
				}

			if( hasFoundQuestion ||
			isUserSpecificationWord )
				{
				if( hasFoundQuestion &&
				isUserSpecificationWord &&
				foundQuestionSpecificationItem != NULL )
					{
					isSimilarQuestion = true;

					if( !hasFoundSpecificationRelationContext ||
					isSameRelationContext )
						{
						if( !isSelfGenerated &&

						( ( isSameRelationContext &&
						!isSameAssignment ) ||

						// Found user question with different exclusive specification
						foundQuestionSpecificationItem->isExclusiveSpecification() != isExclusiveSpecification ||
						// Found user question with different assumption level
						foundQuestionSpecificationItem->assumptionLevel() != assumptionLevel ) )
							isSameQuestion = false;
						else
							{
							if( !hasFoundSpecificationRelationContext &&
							!isExclusiveSpecification &&

							( !hasFoundOlderSpecification ||
							hasRelationContext ) )
								isConflictingQuestion = true;
							}
						}
					else
						{
						if( myWordItem_->isContextSubsetInContextWords( relationContextNr, foundQuestionSpecificationItem->relationContextNr() ) )
							isConflictingQuestion = true;
						else
							{
							isRelatedQuestion = true;
							isSameQuestion = false;
							}
						}
					}
				else
					{
					if( isUserSpecificationWord ||

					// Found question has specification words that aren't in the user question
					( hasSpecificationCollection &&
					isExclusiveSpecification ) )
						isSameQuestion = false;

					if( isSpecificationGeneralization ||

					// Prefer similar over related, if both occur
					( foundQuestionSpecificationItem != NULL &&

					( !hasFoundSpecificationRelationContext ||
					!isSameRelationContext ) ) )
						isSimilarQuestion = true;
					}
				}
			}
		while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );

		if( foundQuestionSpecificationItem != NULL )
			{
			if( !isConflictingQuestion &&
			!isOnlyCheckingForConflicts &&

			( isExclusiveSpecification ||
			isSameExclusiveSpecification ) )
				{
				if( isSameQuestion )
					{
					if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? INTERFACE_QUESTION_I_HAD_THE_SAME_QUESTION_BEFORE : INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about the same question" );
					}
				else
					{
					if( myWordItem_->writeSelectedSpecification( false, false, foundQuestionSpecificationItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the found specification" );

					if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
					strlen( writtenSentenceString ) == 0 )
						return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I couldn't write the found specification" );

					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isSimilarQuestion && !isRelatedQuestion ? INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE : INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE ) : ( isSimilarQuestion && !isRelatedQuestion ? INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE : INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE ) ) ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a related question" );

					if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

					userSpecificationResult.isSimilarOrRelatedQuestion = true;
					}
				}
			else
				{
				if( hasFoundOlderSpecification &&
				!isOnlyCheckingForConflicts )
					{
					hasDisplayedMoreSpecificQuestion_ = true;

					// Skip displaying announcement if question is in conflict with itself
					if( globalVariables_->nUserSpecificationWords == 1 &&
					// Write more specific specification
					writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a more specific related question" );
					}
				}
			}

		return userSpecificationResult;
		}


	protected:
	// Constructor

	WordSpecification( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasCurrentlyConfirmedSpecification_ = false;
		hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = false;
		hasCurrentlyConfirmedSpecificationButNoRelation_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasCorrectedAssumptionByOppositeQuestion_ = false;
		hasDisplayedMoreSpecificSpecification_ = false;
		hasDisplayedMoreSpecificNonExclusiveSpecification_ = false;
		hasDisplayedMoreSpecificQuestion_ = false;
		isOnlyCheckingForConflicts_ = false;
		isWordTouchedDuringCurrentSentence_ = false;

		spanishCompoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = NO_COLLECTION_NR;

		correctedAssumptionReplacedSpecificationItem_ = NULL;
		replacedAssignmentItem_ = NULL;

		spanishCompoundSpecificationWordItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "WordSpecification" );

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given my word item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				myWordItem_->startSystemErrorInWord( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeWordSpecificationVariables()
		{
		hasCurrentlyConfirmedSpecification_ = false;
		hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = false;
		hasCurrentlyConfirmedSpecificationButNoRelation_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasDisplayedMoreSpecificSpecification_ = false;
		hasDisplayedMoreSpecificNonExclusiveSpecification_ = false;
		hasDisplayedMoreSpecificQuestion_ = false;
		isOnlyCheckingForConflicts_ = false;
		isWordTouchedDuringCurrentSentence_ = false;

		spanishCompoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = NO_COLLECTION_NR;

		spanishCompoundSpecificationWordItem_ = NULL;
		}

	bool hasCurrentlyConfirmedSpecification()
		{
		return hasCurrentlyConfirmedSpecification_;
		}

	bool hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation()
		{
		return hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_;
		}

	bool hasCurrentlyConfirmedSpecificationButNoRelation()
		{
		return hasCurrentlyConfirmedSpecificationButNoRelation_;
		}

	bool hasCorrectedAssumption()
		{
		return ( hasCorrectedAssumptionByKnowledge_ ||
				hasCorrectedAssumptionByOppositeQuestion_ );
		}

	bool hasCorrectedAssumptionByKnowledge()
		{
		return hasCorrectedAssumptionByKnowledge_;
		}

	bool hasCorrectedAssumptionByOppositeQuestion()
		{
		return hasCorrectedAssumptionByOppositeQuestion_;
		}

	bool hasDisplayedMoreSpecificSpecification()
		{
		return hasDisplayedMoreSpecificSpecification_;
		}

	bool hasDisplayedMoreSpecificNonExclusiveSpecification()
		{
		return hasDisplayedMoreSpecificNonExclusiveSpecification_;
		}

	bool hasDisplayedMoreSpecificQuestion()
		{
		return hasDisplayedMoreSpecificQuestion_;
		}

	signed char checkForSpecificationConflict( bool isArchivedAssignment, bool isGeneralizationProperNoun, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *conflictingSpecificationItem;
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForSpecificationConflict";

		if( ( specificationResult = checkSpecificationWordForConflict( isArchivedAssignment, isGeneralizationProperNoun, isNegative, isPossessive, specificationCollectionNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );

		conflictingSpecificationItem = specificationResult.specificationItem;

		if( conflictingSpecificationItem != NULL &&
		conflictingSpecificationItem->isOlderItem() &&

		( isNegative ||
		!conflictingSpecificationItem->isNegative() ||
		conflictingSpecificationItem->hasCompoundSpecificationCollection() ) &&

		// Opposite negative of conflicting specification word
		checkSpecificationWordForConflict( isArchivedAssignment, isGeneralizationProperNoun, !isNegative, isPossessive, specificationCollectionNr, relationContextNr, conflictingSpecificationItem->specificationWordItem() ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for the opposite negative specification conflict" );

		return RESULT_OK;
		}

	signed char writeConfirmedSpecification( unsigned short interfaceParameter, SpecificationItem *writeSpecificationItem )
		{
		char *writtenSentenceString;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeConfirmedSpecification";

		if( interfaceParameter <= NO_INTERFACE_PARAMETER )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given interface parameter is undefined" );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( ( writtenSentenceString = writeSpecificationItem->storedSentenceString() ) == NULL ||
		strlen( writtenSentenceString ) <= 0 )
			{
			if( myWordItem_->writeSelectedSpecification( false, true, writeSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

			writtenSentenceString = globalVariables_->writtenSentenceString;
			}

		if( strlen( writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given write specification" );

		if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a confirmation" );

		if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

		return RESULT_OK;
		}

	CreateAndAssignResultType addSpecificationInWord( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		bool hasCopiedRelationContext = ( copiedRelationContextNr > NO_CONTEXT_NR );
		bool hasExistingSpecificationRelationContext;
		bool hasFoundSpecificationWithDifferentRelationContext = false;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool hasSpecificationCollection;
		bool isExistingHiddenSpanishSpecification;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserRelationWord = myWordItem_->isUserRelationWord;
		unsigned int compoundSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int originalSentenceNr;
		JustificationItem *duplicateJustificationItem;
		JustificationItem *foundJustificationItem;
		SpecificationItem *confirmedSpecificationItem = NULL;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *foundUserSpecificationItem;
		SpecificationItem *noRelationContextSpecificationItem;
		SpecificationItem *obsoleteAssumptionSpecificationItem = NULL;
		SpecificationItem *obsoleteHiddenSpanishSpecificationItem = NULL;
		SpecificationItem *obsoleteSpecificationItem = NULL;
		SpecificationItem *tempSpecificationItem;
		CreateAndAssignResultType createAndAssignResult;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSpecificationInWord";

		correctedAssumptionReplacedSpecificationItem_ = NULL;
		replacedAssignmentItem_ = NULL;

		// Check generalization word type
		if( !myWordItem_->hasWordType( true, generalizationWordTypeNr ) )
			return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		// Check relation variables
		if( ( hasRelationContext ||
		relationWordTypeNr != NO_WORD_TYPE_NR ||
		relationWordItem != NULL ) &&

		( ( !hasRelationContext &&
		// Exception: Conditions have no relation context
		!isConditional ) ||

		relationWordItem == NULL ||
		!relationWordItem->hasWordType( false, relationWordTypeNr ) ) )
			return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The relation variables aren't matching" );

		addMyWordToTouchedDuringCurrentSentenceList();

		if( specificationWordItem != NULL )
			{
			// Check specification word type
			if( !specificationWordItem->hasWordType( true, specificationWordTypeNr ) &&
			// New created language
			!myWordItem_->isLanguageWord() )
				return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The given specification word doesn't have the given specification word type" );

			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			// Collect definition specification without specification collection
			if( !isExclusiveSpecification &&
			!isQuestion &&
			generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
			specificationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
			!specificationWordItem->isOlderItem() &&
			!specificationWordItem->hasCollection() &&

			// Collect specifications
			collectSpecifications( isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ) != RESULT_OK )
				return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect myself with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

			if( specificationCollectionNr == NO_COLLECTION_NR )
				{
				if( !isSelfGenerated &&
				userSpecificationCollectionNr_ > NO_COLLECTION_NR )
					// Use previous user specification collection
					specificationCollectionNr = userSpecificationCollectionNr_;
				else
					{
					compoundSpecificationCollectionNr = specificationWordItem->compoundCollectionNr();
					nonCompoundSpecificationCollectionNr = specificationWordItem->nonCompoundCollectionNr();

					if( nonCompoundSpecificationCollectionNr > NO_COLLECTION_NR &&

					( ( compoundSpecificationCollectionNr == NO_COLLECTION_NR &&
					// Skip definition sentence with the same generalization and specification word
					!specificationWordItem->isUserGeneralizationWord ) ||

					( !isQuestion &&

					// Non-exclusive specification collection
					( !isExclusiveSpecification &&

					( !isSpecificationWordSpanishAmbiguous ||

					( hasRelationContext &&

					( nContextRelations > 1 ||
					!specificationWordItem->isUserSpecificationWord ) ) ) ) ) ) )
						specificationCollectionNr = nonCompoundSpecificationCollectionNr;
					else
						specificationCollectionNr = compoundSpecificationCollectionNr;
					}
				}
			else
				{
				if( !specificationWordItem->hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The given specification word doesn't have the given specification collection" );
				}

			hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );

			if( !isSelfGenerated &&
			myWordItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				if( hasSpecificationCollection &&

				( !isAssignment ||
				isGeneralizationProperNoun ) )
					userSpecificationCollectionNr_ = specificationCollectionNr;

				if( ( userSpecificationResult = checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperNoun, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isSpecificationWordSpanishAmbiguous, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to check a user specification or question with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

				confirmedSpecificationItem = userSpecificationResult.confirmedSpecificationItem;
				}

			if( !globalVariables_->hasDisplayedWarning )
				{
				// Try to find specification
				// Start with finding user specification
				if( ( foundUserSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == NULL )
					{
					// Self-generated
					if( firstJustificationItem != NULL )
						{
						if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationCollectionNr, ( isAssignment && !isArchivedAssignment ? NO_CONTEXT_NR : ( hasCopiedRelationContext ? copiedRelationContextNr : relationContextNr ) ), specificationWordItem ) ) == NULL &&
						// Try to find alternative
						!isArchivedAssignment &&
						( tempSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationCollectionNr, NO_CONTEXT_NR, specificationWordItem ) ) != NULL &&
						tempSpecificationItem->isSelfGeneratedConclusion() )
							foundSpecificationItem = tempSpecificationItem;

						if( foundSpecificationItem == NULL )
							{
							// Not found
							if( !isQuestion &&
							relationWordItem != NULL &&
							( existingSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isNegative, isPossessive, specificationWordItem ) ) != NULL )
								{
								if( !hasCopiedRelationContext &&
								compoundSpecificationCollectionNr > NO_COLLECTION_NR &&
								// Existing specification has no specification collection
								!existingSpecificationItem->hasSpecificationCollection() )
									// Obsolete not-collected specification
									obsoleteSpecificationItem = existingSpecificationItem;
								else
									{
									hasExistingSpecificationRelationContext = existingSpecificationItem->hasRelationContext();
									isExistingHiddenSpanishSpecification = existingSpecificationItem->isHiddenSpanishSpecification();

									if( isSpecificationWordSpanishAmbiguous &&
									hasExistingSpecificationRelationContext )
										{
										if( isPossessive )
											{
											if( !isExistingHiddenSpanishSpecification &&
											existingSpecificationItem->isSelfGenerated() &&
											existingSpecificationItem->relationContextNr() == relationContextNr )
												{
												if( existingSpecificationItem->hasSpecificationCollection() )
													foundSpecificationItem = existingSpecificationItem;
												else
													confirmedSpecificationItem = existingSpecificationItem;
												}
											}
										else
											{
											if( existingSpecificationItem->isSelfGeneratedConclusion() &&
											!existingSpecificationItem->hasOnlyOneRelationWord() )
												obsoleteHiddenSpanishSpecificationItem = existingSpecificationItem;
											}
										}
									else
										{
										if( existingSpecificationItem->isUserSpecification() )
											{
											if( isUserRelationWord &&
											isSpecificationWordSpanishAmbiguous )
												// More specific specification
												foundSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, specificationWordItem, relationWordItem );
											}
										else
											{
											if( existingSpecificationItem->relationContextNr() == relationContextNr )
												foundSpecificationItem = existingSpecificationItem;
											else
												{
												if( !isSpecificationWordSpanishAmbiguous &&
												( noRelationContextSpecificationItem = myWordItem_->noRelationContextSpecificationItem( isPossessive, true, specificationWordItem ) ) != NULL )
													{
													if( confirmedSpecificationItem != NULL )
														return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The confirmed specification item is already defined" );

													if( myWordItem_->writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, false, false, hasRelationContext, false, noRelationContextSpecificationItem ) != RESULT_OK )
														return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an assumption, which has relation words now" );

													confirmedSpecificationItem = noRelationContextSpecificationItem;
													}
												}
											}
										}
									}
								}
							}
						else
							{
							// Found specification has same relation context
							if( foundSpecificationItem->relationContextNr() == relationContextNr )
								{
								if( isGeneralizationProperNoun &&
								foundSpecificationItem->isOlderItem() )
									{
									if( firstJustificationItem->isConclusionJustification() )
										{
										if( foundSpecificationItem->isSelfGeneratedAssumption() &&

										( relationWordItem == NULL ||

										( isUserRelationWord &&
										!isSpecificationWordSpanishAmbiguous &&
										relationWordItem->isOlderItem() &&
										foundSpecificationItem->hasOnlyOneRelationWord() &&
										// Conclusion doesn't exist
										myWordItem_->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, NULL ) == NULL ) ) )
											{
											if( foundSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
												return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

											if( ( isUserRelationWord ||
											foundSpecificationItem->isPartOf() ||
											!myWordItem_->hasCurrentlyAnsweredSelfGeneratedQuestion() ) &&

											// Write concluded assumption
											myWordItem_->writeUpdatedSpecification( true, false, false, false, foundSpecificationItem->wasHiddenSpanishSpecification(), foundSpecificationItem ) != RESULT_OK )
												return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a concluded assumption" );
											}
										}
									else
										{
										if( hasRelationContext &&
										isPossessive &&
										!isUserRelationWord &&
										!isSpecificationWordSpanishAmbiguous )
											{
											if( foundSpecificationItem->isConcludedAssumption() &&
											!foundSpecificationItem->hasOnlyOneRelationWord() )
												{
												obsoleteAssumptionSpecificationItem = foundSpecificationItem;
												foundSpecificationItem = NULL;
												}
											else
												{
												if( ( foundJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) ) != NULL &&
												( duplicateJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) ) != NULL &&
												foundJustificationItem->secondarySpecificationItem() == duplicateJustificationItem->secondarySpecificationItem() &&

												// Remove an almost duplicate justification. Different justification type
												myWordItem_->replaceJustification( duplicateJustificationItem, foundJustificationItem, foundSpecificationItem ) != RESULT_OK )
													return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to remove an almost duplicate justification in my word" );
												}
											}
										}
									}
								}
							else	// Different relation context
								{
								if( hasRelationContext &&
								foundSpecificationItem->hasRelationContext() )
									{
									hasFoundSpecificationWithDifferentRelationContext = true;

									if( !isSpecificationWordSpanishAmbiguous &&
									firstJustificationItem->isPossessiveReversibleConclusion() &&

									( foundSpecificationItem->isSelfGeneratedConclusion() ||
									foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ) == NULL ) )
										obsoleteAssumptionSpecificationItem = foundSpecificationItem;
									}
								}
							}
						}
					}
				else
					{
					if( foundUserSpecificationItem->isSpecificationGeneralization() == isSpecificationGeneralization )
						{
						if( !isSelfGenerated ||
						foundUserSpecificationItem->hasRelationContext() ||
						!foundUserSpecificationItem->isExclusiveSpecification() ||
						foundUserSpecificationItem->isOlderItem() )
							{
							foundSpecificationItem = foundUserSpecificationItem;

							if( hasRelationContext &&
							isQuestion &&
							foundUserSpecificationItem->hasRelationContext() &&
							foundUserSpecificationItem->relationContextNr() != relationContextNr )
								// Question with different relation context
								foundSpecificationItem = NULL;
							else
								{
								if( specificationCollectionNr > NO_COLLECTION_NR &&
								relationWordItem != NULL &&
								foundUserSpecificationItem->hasRelationContext() &&
								!foundUserSpecificationItem->hasSpecificationCollection() )
									confirmedSpecificationItem = foundUserSpecificationItem;

								foundUserSpecificationItem = NULL;
								}
							}
						else	// Typical for compound collections in Spanish
							{
							if( spanishCompoundSpecificationCollectionNr_ == NO_COLLECTION_NR ||
							spanishCompoundSpecificationWordItem_ == NULL )
								{
								if( ( specificationCollectionNr = myWordItem_->highestCollectionNrInCollectionWords() ) >= MAX_COLLECTION_NR )
									return myWordItem_->startCreateAndAssignResultSystemErrorInWord( functionNameString, moduleNameString_, "Collection number overflow" );

								spanishCompoundSpecificationCollectionNr_ = ++specificationCollectionNr;
								spanishCompoundSpecificationWordItem_ = specificationWordItem;
								}
							else
								{
								specificationCollectionNr = spanishCompoundSpecificationCollectionNr_;

								if( spanishCompoundSpecificationWordItem_->createCollection( isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", spanishCompoundSpecificationWordItem_->anyWordTypeString(), "\" with word \"", specificationWordItem->anyWordTypeString(), "\"" );

								if( specificationWordItem->createCollection( isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishCompoundSpecificationWordItem_, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", specificationWordItem->anyWordTypeString(), "\" with word \"", spanishCompoundSpecificationWordItem_->anyWordTypeString(), "\"" );
								}
							}
						}
					else
						{
						if( foundUserSpecificationItem->isSpecificationWordSpanishAmbiguous() )
							specificationCollectionNr = myWordItem_->collectionNr( myWordItem_ );
						}
					}
				}

			// Add generalization items
			if( hasRelationContext ||
			foundSpecificationItem == NULL )
				{
				// Specification word
				if( addGeneralization( false, generalizationWordTypeNr, specificationWordTypeNr, specificationWordItem ) != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to add a specification word to the generalizations" );

				// Relation word
				if( relationWordItem != NULL &&
				addGeneralization( true, generalizationWordTypeNr, relationWordTypeNr, relationWordItem ) != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to add a relation word to the generalizations" );
				}
			}

		if( !globalVariables_->hasDisplayedWarning )
			{
			// Not found
			if( foundSpecificationItem == NULL ||
			// User specification has copied relation context
			hasCopiedRelationContext ||
			// Accept different relation context (e.g. ambiguous specification)
			hasFoundSpecificationWithDifferentRelationContext ||

			// Exceptions
			confirmedSpecificationItem != NULL ||
			userSpecificationResult.isNonExclusiveSpecification ||

			// Overwrite non-exclusive specification by exclusive one
			( isExclusiveSpecification &&
			!foundSpecificationItem->isExclusiveSpecification() ) ||

			// Overwrite user questions by non-exclusive one
			( !isAssignment &&
			isQuestion &&
			!isSelfGenerated &&
			!userSpecificationResult.isSameQuestionFromUser &&
			foundSpecificationItem->isExclusiveSpecification() ) ||

			// Overwrite non-conditional specification by conditional one
			( isConditional &&
			!foundSpecificationItem->isConditional() ) )
				{
				if( foundSpecificationItem != NULL )
					{
					if( !isExclusiveSpecification &&
					!isQuestion &&
					foundSpecificationItem->isExclusiveSpecification() )
						isExclusiveSpecification = true;

					if( !isQuestion &&
					!isSelfGenerated &&
					foundSpecificationItem != confirmedSpecificationItem )
						// Obsolete user specification
						obsoleteSpecificationItem = foundSpecificationItem;
					}

				if( isExclusiveSpecification )
					{
					if( userSpecificationResult.isNonExclusiveSpecification )
						isExclusiveSpecification = false;
					}
				else
					{
					if( userSpecificationResult.isConfirmedExclusive )
						isExclusiveSpecification = true;
					}

				originalSentenceNr = ( hasRelationContext ||
										isQuestion ||
										foundSpecificationItem == NULL ? ( obsoleteHiddenSpanishSpecificationItem == NULL ? globalVariables_->currentSentenceNr : obsoleteHiddenSpanishSpecificationItem->creationSentenceNr() ) : foundSpecificationItem->originalSentenceNr() );

				// Create the actual specification
				if( ( createAndAssignResult = myWordItem_->createSpecificationItem( false, false, false, false, isCharacteristicFor, false, isConditional, ( hasCorrectedAssumptionByKnowledge_ || hasCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, NO_ASSIGNMENT_LEVEL, assumptionLevel, globalVariables_->currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? NO_WORD_TYPE_NR : relationWordTypeNr ), specificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, ( isAssignment ? NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, NULL, NULL ) ).result != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to create a specification item" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
					return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I couldn't create a specification item" );

				// Now replaced obsolete specifications by the created specification
				if( replaceObsoleteSpecification( userSpecificationResult.isConfirmedSpecificationButNotItsRelation, confirmedSpecificationItem, createdSpecificationItem, obsoleteAssumptionSpecificationItem, obsoleteHiddenSpanishSpecificationItem, obsoleteSpecificationItem ) != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete specification" );

				if( hasCopiedRelationContext &&
				firstJustificationItem != NULL &&
				foundSpecificationItem != NULL &&

				( !foundSpecificationItem->hasSpecificationCollection() ||
				foundSpecificationItem->isSelfGeneratedAssumption() ||

				( isPossessive &&
				foundSpecificationItem->isSelfGeneratedConclusion() ) ) )
					{
					if( !firstJustificationItem->isOlderItem() &&
					firstJustificationItem != foundSpecificationItem->firstJustificationItem() )
						{
						if( isPossessive &&
						foundSpecificationItem->hasCurrentCreationSentenceNr() &&

						( !isSpecificationWordSpanishAmbiguous ||
						// Typical for Spanish
						!foundSpecificationItem->isHiddenSpanishSpecification() ) )
							{
							// Change first justification
							if( createdSpecificationItem->changeFirstJustification( true, foundSpecificationItem->firstJustificationItem() ) != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace the justifications of the created specification by the justifications of the found specification" );
							}
						else
							{
							// Change attached justification
							if( firstJustificationItem->changeAttachedJustification( foundSpecificationItem->firstJustificationItem() ) != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
							}
						}

					if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
						return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );
					}

				if( !isNegative &&
				!isPossessive &&
				!isQuestion &&
				specificationWordItem != NULL &&

				( !isSpecificationWordSpanishAmbiguous ||
				!createdSpecificationItem->isHiddenSpanishSpecification() ) )
					{
					if( myWordItem_->findAnswersToQuestions( specificationWordItem->compoundCollectionNr(), createdSpecificationItem ) != RESULT_OK )
						return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to find answers to questions" );

					if( !isSelfGenerated &&

					( isGeneralizationProperNoun ||
					generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR ) &&

					!myWordItem_->isFemaleOrMale() )
						{
						if( specificationWordItem->isFemale() )
							{
							if( myWordItem_->markWordAsFemale() != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as female" );
							}
						else
							{
							if( specificationWordItem->isMale() &&
							myWordItem_->markWordAsMale() != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as male" );
							}
						}
					}
				}
			}

		createAndAssignResult.foundSpecificationItem = foundSpecificationItem;
		createAndAssignResult.replacedAssignmentItem = replacedAssignmentItem_;
		return createAndAssignResult;
		}

	RelatedResultType findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( searchSpecificationItem == NULL )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given search specification item is undefined" );

		return findRelatedSpecification( isCheckingRelationContext, false, false, searchSpecificationItem->isAssignment(), searchSpecificationItem->isInactiveAssignment(), searchSpecificationItem->isArchivedAssignment(), searchSpecificationItem->isExclusiveSpecification(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPossessive(), searchSpecificationItem->questionParameter(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->specificationWordItem() );
		}

	RelatedResultType findRelatedSpecification( bool isIgnoringExclusive, bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( isIncludingAssignments )
			{
			if( ( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			relatedResult.relatedSpecificationItem == NULL &&
			// Archived assignments
			( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification as well" );
			}

		// Specifications
		if( relatedResult.relatedSpecificationItem == NULL &&
		( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

		return relatedResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
