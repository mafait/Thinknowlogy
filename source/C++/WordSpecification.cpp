/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		To create specification structures
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

#include "ContextItem.cpp"
#include "SpecificationList.cpp"
#include "UserSpecificationResultType.cpp"
#include "WordTypeItem.cpp"

class WordSpecification
	{
	friend class WordItem;

	// Private constructed variables

	bool hasConfirmedSpecification_;
	bool hasConfirmedSpecificationButNoRelation_;
	bool hasCorrectedAssumptionByKnowledge_;
	bool hasCorrectedAssumptionByOppositeQuestion_;
	bool hasDisplayedMoreSpecificSpecification_;
	bool hasDisplayedMoreSpecificNonExclusiveSpecification_;
	bool hasDisplayedMoreSpecificQuestion_;
	bool isThisProperNameWordTouchedDuringCurrentSentence_;
	bool isOnlyCheckingForConflicts_;

	unsigned int spanishCompoundSpecificationCollectionNr_;
	unsigned int userSpecificationCollectionNr_;

	SpecificationItem *correctedAssumptionReplacedSpecificationItem_;
	SpecificationItem *firstConfirmedReplacedSpanishSpecificationItem_;
	SpecificationItem *replacedAssignmentItem_;

	WordItem *spanishCompoundSpecificationWordItem_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;
	WordItem *myWordItem_;


	// Private functions

	void addToTouchedDuringCurrentSentenceProperNameWordList()
		{
		WordItem *tempProperNameWordItem;
		WordItem *lastTouchedProperNameWordItem = commonVariables_->firstTouchedProperNameWordItem;

		if( lastTouchedProperNameWordItem == NULL )
			commonVariables_->firstTouchedProperNameWordItem = myWordItem_;
		else
			{
			// Word order is important: Add word at end of temporary touched word list
			while( ( tempProperNameWordItem = lastTouchedProperNameWordItem->nextTouchedProperNameWordItem ) != NULL )
				lastTouchedProperNameWordItem = tempProperNameWordItem;

			lastTouchedProperNameWordItem->nextTouchedProperNameWordItem = myWordItem_;
			}
		}

	unsigned int nonCompoundCollectionNrInCollectionWords( unsigned int compoundCollectionNr )
		{
		unsigned int nonCompoundCollectionNr;
		WordItem *currentCollectionWordItem;

		if( compoundCollectionNr > NO_COLLECTION_NR &&
		( currentCollectionWordItem = commonVariables_->firstCollectionWordItem ) != NULL )
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "addGeneralization";

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

	signed char checkUserQuestion( bool hasRelationContext, bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *foundQuestionSpecificationItem, WordItem *specificationWordItem )
		{
		SpecificationItem *relatedSpecificationItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkUserQuestion";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( foundQuestionSpecificationItem != NULL &&
		foundQuestionSpecificationItem->isAnsweredQuestion() &&
		foundQuestionSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification &&
		foundQuestionSpecificationItem->relationContextNr() == relationContextNr )
			commonVariables_->isQuestionAlreadyAnswered = true;

		if( foundQuestionSpecificationItem == NULL ||
		!foundQuestionSpecificationItem->isQuestion() )
			{
			if( specificationCollectionNr > NO_COLLECTION_NR &&
			( relatedResult = findRelatedSpecification( false, false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
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

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_->firstSelfGeneratedCheckSpecificationItem( true, true, true, relatedSpecificationItem->isNegative(), relatedSpecificationItem->isPossessive(), true, NO_QUESTION_PARAMETER, relatedSpecificationItem->specificationCollectionNr(), relatedSpecificationItem->relationContextNr(), relatedSpecificationItem->specificationWordItem() ) ) != NULL )
							{
							if( myWordItem_->writeUpdatedSpecification( false, false, false, true, false, correctedAssumptionReplacedSpecificationItem_ ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a conflicting specification" );

							hasCorrectedAssumptionByOppositeQuestion_ = true;
							}
						}
					}
				else
					{
					if( relatedSpecificationItem->hasSpecificationCompoundCollection() &&
					inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char collectSpecifications( bool isExclusiveGeneralization, bool isQuestion, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem )
		{
		CollectionResultType collectionResult;
		unsigned int specificationCollectionNr;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecifications";

		if( collectionWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given collection word item is undefined" );

		if( ( currentGeneralizationItem = myWordItem_->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != collectionWordItem )
					{
					if( ( collectionResult = currentGeneralizationWordItem->collectSpecifications( collectionWordTypeNr, commonWordTypeNr, generalizationWordItem, collectionWordItem ) ).result != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to add a specification collection by exclusive specification in word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

					foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem;

					if( foundGeneralizationWordItem != NULL &&
					( specificationCollectionNr = collectionWordItem->collectionNr( collectionWordTypeNr, currentGeneralizationWordItem ) ) > NO_COLLECTION_NR &&
					// Collect by generalization
					foundGeneralizationWordItem->collectSpecificationsInWord( isExclusiveGeneralization, isQuestion, specificationCollectionNr ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to collect specifications in word \"", foundGeneralizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( collectionResult.foundGeneralizationWordItem == NULL &&
			( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char displayAssumptionsThatAreNotHiddenAnymore()
		{
		ContextItem *currentContextItem;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *hiddenSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "displayAssumptionsThatAreNotHiddenAnymore";

		if( ( currentContextItem = myWordItem_->firstActiveContextItem() ) != NULL )
			{
			// Do for all context items in my word
			do	{
				if( ( currentGeneralizationItem = myWordItem_->firstGeneralizationItem() ) != NULL )
					{
					// Check all involved words
					do	{
						if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I found an undefined generalization word" );

						if( ( hiddenSpecificationItem = currentGeneralizationWordItem->firstUnhiddenSpecificationItem( currentContextItem->contextNr() ) ) != NULL &&
						currentGeneralizationWordItem->writeUpdatedSpecification( true, false, false, false, false, hiddenSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an assumption that is not hidden anymore" );
						}
					while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
					}
				}
			while( ( currentContextItem = currentContextItem->nextContextItem() ) != NULL );
			}

		return RESULT_OK;
		}

	signed char processPreviouslyUnknownPluralNoun( WordItem *specificationWordItem )
		{
		WordItem *currentLanguageWordItem;
		WordItem *currentSpecificationWordItem;
		WordTypeItem *pluralNounWordTypeItem;
		const char *unknownPluralNounString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "processPreviouslyUnknownPluralNoun";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		pluralNounWordTypeItem = specificationWordItem->activeWordTypeItem( WORD_TYPE_NOUN_PLURAL );

		if( pluralNounWordTypeItem != NULL &&
		// Skip announcement for new occurrences
		!pluralNounWordTypeItem->isOlderItem() )
			{
			if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_START, pluralNounWordTypeItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_YOU_ENTERED_UNKNOWN_PLURAL_OF_SINGULAR_NOUN_END ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a notification text" );

			currentLanguageWordItem = commonVariables_->currentLanguageWordItem;

			if( currentLanguageWordItem != NULL &&
			( currentSpecificationWordItem = commonVariables_->firstSpecificationWordItem ) != NULL &&
			( unknownPluralNounString = currentLanguageWordItem->interfaceString( INTERFACE_GRAMMAR_UNKNOWN_PLURAL_NOUN_START ) ) != NULL )
				{
				// Do for all specification words
				do	{
					if( currentSpecificationWordItem->clearLastWrittenSentenceStringWithUnknownPluralNoun( unknownPluralNounString, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to clear the last written sentence string with specification word \"", specificationWordItem->anyWordTypeString(), "\" in generalization word \"", currentSpecificationWordItem->anyWordTypeString(), "\"" );
					}
				while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
				}
			}

		return RESULT_OK;
		}

	signed char replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *createdSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isAssignment() )
			{
			if( replacedAssignmentItem_ == NULL &&
			firstConfirmedReplacedSpanishSpecificationItem_ != NULL &&
			myWordItem_->nContextWordsInContextWords( obsoleteSpecificationItem->relationContextNr(), obsoleteSpecificationItem->specificationWordItem() ) == commonVariables_->nUserRelationWords &&
			myWordItem_->replaceOrDeleteSpecification( true, firstConfirmedReplacedSpanishSpecificationItem_, obsoleteSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a confirmed specification" );

			replacedAssignmentItem_ = obsoleteSpecificationItem;
			}
		else
			{
			if( myWordItem_->replaceOrDeleteSpecification( false, obsoleteSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a specification" );

			if( firstConfirmedReplacedSpanishSpecificationItem_ == NULL &&
			obsoleteSpecificationItem->isReplacedItem() )
				firstConfirmedReplacedSpanishSpecificationItem_ = obsoleteSpecificationItem;
			}

		return RESULT_OK;
		}

	signed char replaceObsoleteSpecification( SpecificationItem *confirmedReplacedSpecificationItem, SpecificationItem *confirmedSpecificationButNoRelationReplacedSpecificationItem, SpecificationItem *createdSpecificationItem, SpecificationItem *foundUserSpecificationItem, SpecificationItem *obsoleteAssumptionSpecificationItem, SpecificationItem *obsoleteConcludedAssumptionSpecificationItem, SpecificationItem *obsoleteHiddenSpanishSpecificationItem, SpecificationItem *obsoleteNotCollectedSpecificationItem, SpecificationItem *obsoleteUserSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceObsoleteSpecification";

		if( createdSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given created specification is undefined" );

		if( confirmedReplacedSpecificationItem != NULL &&
		!createdSpecificationItem->isHiddenSpanishSpecification() )
			{
			if( replaceOrDeleteSpecification( confirmedReplacedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a confirmed replaced specification" );

			if( foundUserSpecificationItem == NULL )
				{
				// Clean-up obsolete justifications
				if( myWordItem_->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications" );
				}
			else
				{
				if( !foundUserSpecificationItem->isReplacedOrDeletedItem() &&
				replaceOrDeleteSpecification( foundUserSpecificationItem, createdSpecificationItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a duplicate user specification" );
				}
			}

		if( confirmedSpecificationButNoRelationReplacedSpecificationItem != NULL &&
		myWordItem_->confirmSpecificationButNotItsRelation( confirmedSpecificationButNoRelationReplacedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to confirm a specification, but no relation(s)" );

		if( correctedAssumptionReplacedSpecificationItem_ != NULL )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a corrected assumption replaced specification" );

			correctedAssumptionReplacedSpecificationItem_ = NULL;
			}

		if( obsoleteAssumptionSpecificationItem != NULL )
			{
			if( myWordItem_->attachJustificationInWord( obsoleteAssumptionSpecificationItem->firstJustificationItem(), createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to attach the first justification of the found assumption specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteAssumptionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete assumption specification" );
			}

		if( obsoleteConcludedAssumptionSpecificationItem != NULL &&
		replaceOrDeleteSpecification( obsoleteConcludedAssumptionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete concluded assumption specification" );

		if( obsoleteHiddenSpanishSpecificationItem != NULL )
			{
			if( myWordItem_->attachJustificationInWord( obsoleteHiddenSpanishSpecificationItem->firstJustificationItem(), createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to attach the justifications of the found replaced hidden specification to the created specification" );

			if( replaceOrDeleteSpecification( obsoleteHiddenSpanishSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete hidden Spanish specification" );
			}

		if( obsoleteNotCollectedSpecificationItem != NULL )
			{
			if( replaceOrDeleteSpecification( obsoleteNotCollectedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete not-collected specification" );

			// Clean-up obsolete justifications
			if( myWordItem_->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications" );
			}

		if( obsoleteUserSpecificationItem != NULL &&
		replaceOrDeleteSpecification( obsoleteUserSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete an obsolete user specification" );

		return RESULT_OK;
		}

	signed char writeConfirmedSpecification( unsigned short interfaceParameter, SpecificationItem *writeSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeConfirmedSpecification";

		if( interfaceParameter <= NO_INTERFACE_PARAMETER )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given interface parameter is undefined" );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		if( myWordItem_->writeSelectedSpecification( false, true, writeSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

		if( strlen( commonVariables_->writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given write specification" );

		if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a confirmation" );

		if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

		return RESULT_OK;
		}

	signed char writeMoreSpecificSpecification( SpecificationItem *olderSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeMoreSpecificSpecification";

		if( olderSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given older specification item is undefined" );

		if( !olderSpecificationItem->isOlderItem() )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given older specification item isn't old" );

		if( myWordItem_->writeSelectedSpecification( false, !olderSpecificationItem->isExclusiveSpecification(), olderSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the given older specification" );

		if( strlen( commonVariables_->writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given older specification" );

		if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( olderSpecificationItem->isQuestion() ? ( olderSpecificationItem->isSelfGenerated() ? INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem->isSelfGenerated() ? ( olderSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface listing text" );

		if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );

		return RESULT_OK;
		}

	RelatedResultType findRelatedSpecification( bool isCheckingAllQuestions, bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool isFirstRelatedSpecification = true;
		bool isLastRelatedSpecification = false;
		unsigned int currentSpecificationSentenceNr;
		unsigned int currentSpecificationItemNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;
		unsigned int highestItemNr = NO_ITEM_NR;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( specificationCollectionNr == NO_COLLECTION_NR )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given specification collection number is undefined" );

		if( specificationWordItem == NULL )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		currentSpecificationItem = ( isCheckingAllQuestions ? myWordItem_->firstSpecificationItem( isAssignment, isInactiveAssignment, isArchivedAssignment, true ) :
															myWordItem_->firstSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) );

		if( currentSpecificationItem != NULL )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem->relatedSpecificationWordItem( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) != NULL )
					{
					currentSpecificationSentenceNr = currentSpecificationItem->creationSentenceNr();
					currentSpecificationItemNr = currentSpecificationItem->itemNr();

					if( currentSpecificationWordItem == specificationWordItem &&
					currentSpecificationItem->relationContextNr() == relationContextNr )
						{
						// Found the given specification item
						isFirstRelatedSpecification = false;

						if( currentSpecificationSentenceNr > highestSentenceNr ||

						( currentSpecificationSentenceNr == highestSentenceNr &&
						currentSpecificationItemNr > highestItemNr ) )
							{
							isLastRelatedSpecification = true;

							highestSentenceNr = currentSpecificationSentenceNr;
							highestItemNr = currentSpecificationItemNr;
							}
						}
					else
						{
						if( currentSpecificationSentenceNr > highestSentenceNr ||

						( currentSpecificationSentenceNr == highestSentenceNr &&
						currentSpecificationItemNr > highestItemNr ) )
							{
							isLastRelatedSpecification = false;

							highestSentenceNr = currentSpecificationSentenceNr;
							highestItemNr = currentSpecificationItemNr;
							}

						// If the specification has no relation, select the oldest one (the first of a series)
						if( !hasRelationContext ||
						relatedResult.relatedSpecificationItem == NULL ||

						( currentSpecificationItem->specificationCollectionNr() == specificationCollectionNr &&
						currentSpecificationWordItem != specificationWordItem ) )
							{
							relatedResult.isFirstRelatedSpecification = isFirstRelatedSpecification;
							relatedResult.relatedSpecificationItem = currentSpecificationItem;
							}
						}
					}
				}
			while( ( currentSpecificationItem = ( isCheckingAllQuestions ? currentSpecificationItem->nextSelectedSpecificationItem() : currentSpecificationItem->nextSelectedSpecificationItem() ) ) != NULL );

			relatedResult.isLastRelatedSpecification = isLastRelatedSpecification;
			}

		return relatedResult;
		}

	SpecificationResultType checkSpecificationWordForConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationProperName, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		bool isSpecificationWordSpanishAmbiguous;
		bool hasFoundCompoundConflict = false;
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int compoundSpecificationCollectionNr = NO_COLLECTION_NR;
		SpecificationItem *anotherConflictingSpecificationItem;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *tempConflictingSpecificationItem;
		SpecificationItem *conflictingSpecificationItem = NULL;
		SpecificationItem *pastTenseAssignmentItem = NULL;
		RelatedResultType relatedResult;
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationWordForConflict";

		if( specificationWordItem == NULL )
			return myWordItem_->startSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Derive conflict
		if( hasSpecificationCollection &&
		( relatedResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

		if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) == NULL )
			{
			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

			// Check for looping
			if( isSpecificationWordSpanishAmbiguous ||
			( conflictingSpecificationItem = specificationWordItem->bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, relationContextNr, myWordItem_ ) ) == NULL )
				{
				// Check for past tense
				foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem != NULL &&
				foundSpecificationItem->isArchivedAssignment() != isArchivedAssignment )
					conflictingSpecificationItem = foundSpecificationItem;
				else
					{
					if( !isSpecificationWordSpanishAmbiguous &&
					( compoundSpecificationCollectionNr = specificationWordItem->compoundCollectionNr( specificationWordTypeNr ) ) > NO_COLLECTION_NR )
						{
						// Check for negative
						foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, NO_CONTEXT_NR, relationContextNr, specificationWordItem );

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
							if( ( anotherConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, NO_CONTEXT_NR, relationContextNr, NULL ) ) != NULL &&
							// Write conflict
							anotherConflictingSpecificationItem->writeSpecificationConflict() != RESULT_OK )
								return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write another conflicting specification" );

							conflictingSpecificationItem = foundSpecificationItem;
							}
						else
							{
							// Check for existing compound specification
							// No self-generated question available for this specification
							if( ( nonCompoundSpecificationCollectionNr = nonCompoundCollectionNrInCollectionWords( compoundSpecificationCollectionNr ) ) > NO_COLLECTION_NR )
								{
								tempConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, NO_CONTEXT_NR, relationContextNr, NULL );

								if( tempConflictingSpecificationItem == NULL )
									{
									tempConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, NO_CONTEXT_NR, relationContextNr, NULL );

									if( tempConflictingSpecificationItem != NULL &&
									!tempConflictingSpecificationItem->isOlderItem() &&
									tempConflictingSpecificationItem->specificationWordItem() != specificationWordItem )
										conflictingSpecificationItem = tempConflictingSpecificationItem;
									}
								else
									{
									if( tempConflictingSpecificationItem->specificationWordItem() != specificationWordItem &&
									!tempConflictingSpecificationItem->isHiddenSpanishSpecification() )
										conflictingSpecificationItem = tempConflictingSpecificationItem;
									}
								}
							}
						}
					}
				}
			}
		else
			{
			// Find for possible past tense assignment of relatedSpecificationItem
			pastTenseAssignmentItem = myWordItem_->firstNonQuestionAssignmentItem( false, false, true, isNegative, isPossessive, relationContextNr, relatedSpecificationItem->specificationWordItem() );
			conflictingSpecificationItem = ( pastTenseAssignmentItem == NULL ? relatedSpecificationItem : pastTenseAssignmentItem );
			}

		if( !hasFoundCompoundConflict &&
		!isExclusiveSpecification &&
		conflictingSpecificationItem != NULL &&
		!conflictingSpecificationItem->hasSpecificationBeenWrittenAsConflict() )
			{
			if( conflictingSpecificationItem->isOlderItem() )
				{
				if( ( !hasSpecificationCollection ||
				isGeneralizationProperName ) &&

				// Write conflicting specification
				conflictingSpecificationItem->writeSpecificationConflict() != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the conflicting specification" );
				}
			else
				{
				if( hasSpecificationCollection &&
				!conflictingSpecificationItem->hasSpecificationNonExclusiveCollection() &&

				// Sentence in conflict with itself
				inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_LISTING_SENTENCE_IN_CONFLICT_WITH_ITSELF ) != RESULT_OK )
					return myWordItem_->addSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			}

		specificationResult.specificationItem = conflictingSpecificationItem;
		return specificationResult;
		}

	UserSpecificationResultType checkUserSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationProperName, bool isNegative, bool isPossessive, bool isSelection, bool isValueSpecification, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, SpecificationItem *foundSpecificationItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundRelationContext;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		JustificationItem *nextJustificationItem;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *predecessorObsoleteJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		JustificationResultType justificationResult;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkUserSpecification";

		if( foundSpecificationItem == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given found specification item is undefined" );

		if( specificationWordItem == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( foundSpecificationItem->isValueSpecification() != isValueSpecification )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "Value specification conflict! Specification word \"", specificationWordItem->anyWordTypeString(), "\" is already a value specification or it is already a normal specification and you want to make it a value specification" );

		if( !hasRelationContext &&
		!isAssignment &&
		!isExclusiveSpecification &&
		foundSpecificationItem->isExclusiveSpecification() &&
		foundSpecificationItem->isUserSpecification() )
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
			( assumptionSpecificationItem = myWordItem_->firstAssignmentOrSpecificationItem( true, true, false, false, NO_QUESTION_PARAMETER, secondarySpecificationItem->specificationWordItem() ) ) != NULL &&
			( predecessorObsoleteJustificationItem = assumptionSpecificationItem->firstJustificationItem() ) != NULL )
				{
				while( ( nextJustificationItem = predecessorObsoleteJustificationItem->attachedJustificationItem() ) != obsoleteJustificationItem )
					predecessorObsoleteJustificationItem = nextJustificationItem;

				if( predecessorObsoleteJustificationItem != obsoleteJustificationItem )
					{
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
			}
		else
			{
			if( !isSelection &&
			hasRelationContext &&
			foundSpecificationItem->isOlderItem() )
				{
				if( foundSpecificationItem->isArchivedAssignment() == isArchivedAssignment &&
				// Check if negative specification exists
				myWordItem_->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, NO_QUESTION_PARAMETER, specificationWordItem ) == NULL )
					{
					if( foundSpecificationItem->hasRelationContext() )
						{
						if( isPossessive &&
						relationWordItem != NULL &&
						specificationWordItem->isFemale() != relationWordItem->isFemale() &&
						specificationWordItem->isMale() != relationWordItem->isMale() &&

						// Check for specification conflict
						checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
						}
					else
						{
						if( !isInactiveAssignment )
							{
							hasDisplayedMoreSpecificSpecification_ = true;

							if( writeMoreSpecificSpecification( foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an notification about a more specific related specification" );
							}
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				if( foundSpecificationItem->hasRelationContext() &&
				foundSpecificationItem->relationContextNr() != relationContextNr &&
				foundSpecificationItem->isInactiveAssignment() == isInactiveAssignment &&
				foundSpecificationItem->isArchivedAssignment() == isArchivedAssignment &&

				( !hasRelationContext ||
				// Current user specification has less relation words than the found specification
				commonVariables_->nUserRelationWords < myWordItem_->nContextWordsInContextWords( foundSpecificationItem->relationContextNr(), specificationWordItem ) ) )
					{
					// Has new relation context
					if( hasRelationContext ||
					myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, relationContextNr, specificationWordItem ) == NULL )
						{
						if( !hasRelationContext )
							{
							hasConfirmedSpecificationButNoRelation_ = true;
							userSpecificationResult.confirmedSpecificationButNoRelationReplacedSpecificationItem = foundSpecificationItem;
							}

						if( !foundSpecificationItem->isHiddenSpanishSpecification() &&
						writeConfirmedSpecification( ( foundSpecificationItem->isSelfGeneratedAssumption() ? ( hasRelationContext ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_ASSUMPTION_BUT_NO_RELATION ) : ( hasRelationContext ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_OF_MY_CONCLUSION_BUT_NO_RELATION ) ), foundSpecificationItem ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed specification" );
						}
					}
				else
					{
					if( hasRelationContext )
						{
						hasFoundRelationContext = foundSpecificationItem->hasRelationContext();

						if( !hasFoundRelationContext ||

						( !foundSpecificationItem->isOlderItem() &&
						foundSpecificationItem->relationContextNr() == relationContextNr ) )
							// Confirmed relation words
							foundSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( hasFoundRelationContext, false, false, isNegative, isPossessive, true, NO_QUESTION_PARAMETER, specificationCollectionNr, foundSpecificationItem->relationContextNr(), specificationWordItem );
						}

					if( foundSpecificationItem != NULL )
						{
						// Confirmation: Replace a self-generated by a user-entered specification
						if( foundSpecificationItem->isSelfGenerated() &&

						( hasRelationContext ||
						!foundSpecificationItem->isExclusiveSpecification() ) )
							{
							// Has already confirmed another specification
							if( hasConfirmedSpecification_ &&
							foundSpecificationItem->hasRelationContext() &&
							foundSpecificationItem->hasSpecificationNonCompoundCollection() &&
							foundSpecificationItem->relationContextNr() != relationContextNr &&
							( obsoleteJustificationItem = myWordItem_->secondarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, foundSpecificationItem ) ) != NULL &&

							// Replace justification of confirmed specification
							myWordItem_->replaceJustification( obsoleteJustificationItem, NULL, foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item" );

							if( foundSpecificationItem->isExclusiveSpecification() )
								userSpecificationResult.isConfirmedExclusive = true;

							if( foundSpecificationItem->isSelfGenerated() )
								hasConfirmedSpecification_ = true;

							userSpecificationResult.confirmedReplacedSpecificationItem = foundSpecificationItem;

							if( !foundSpecificationItem->isHiddenSpanishSpecification() &&
							writeConfirmedSpecification( ( foundSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_ARE_CONFIRMED : INTERFACE_LISTING_MY_CONCLUSIONS_THAT_ARE_CONFIRMED ), foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed specification" );
							}

						if( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
						foundSpecificationItem->specificationWordTypeNr() == WORD_TYPE_NOUN_SINGULAR )
							{
							if( processPreviouslyUnknownPluralNoun( specificationWordItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to process a previously unknown plural noun" );

							userSpecificationResult.confirmedReplacedSpecificationItem = foundSpecificationItem;
							}
						}
					}
				}
			}

		return userSpecificationResult;
		}

	UserSpecificationResultType checkUserSpecificationOrQuestion( bool hasRelationContext, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationProperName, bool isNegative, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isValueSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFeminineSingularNounEnding;
		bool isSingularNounGeneralizationWord = ( generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR );
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordTypeItem *foundWordTypeItem;
		WordTypeItem *singularNounWordTypeItem;
		UserSpecificationResultType userSpecificationResult;
		BoolResultType boolResult;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkUserSpecificationOrQuestion";

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
			if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != RESULT_OK )
				return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification looping conflict" );
			}
		else
			{
			// Check current assignment, specification or question (with relation)
			if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( true, true, true, true, true, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isQuestion ? NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) != NULL )
				{
				if( !isQuestion &&
				isArchivedAssignment != foundSpecificationItem->isArchivedAssignment() &&

				// Check for specification conflict
				checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationWordItem ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
				}

			if( isQuestion &&
			foundSpecificationItem == NULL )
				foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem );

			if( foundSpecificationItem == NULL )
				{
				// Check current assignment or specification (without this relation)
				if( isQuestion )
					foundSpecificationItem = myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, NO_CONTEXT_NR, specificationWordItem );

				if( foundSpecificationItem == NULL )
					foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, generalizationContextNr, NO_CONTEXT_NR, specificationWordItem );

				if( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME )
					{
					// Exclusive specifications are not conflicting
					if( !isExclusiveSpecification &&
					// Possessive specifications are not conflicting
					!isPossessive &&
					foundSpecificationItem == NULL )
						{
						if( !myWordItem_->isFemaleOrMale() &&
						myWordItem_->hasFeminineProperNameEnding() != specificationWordItem->isFemale() &&
						myWordItem_->hasMasculineProperNameEnding() != specificationWordItem->isMale() &&
						inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NAME_TO_BE, myWordItem_->anyWordTypeString(), ( myWordItem_->hasFeminineProperNameEnding() ? INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a proper name expected to be feminine or masculine" );

						if( specificationCollectionNr > NO_COLLECTION_NR )
							{
							if( ( relatedResult = findRelatedSpecification( false, true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

							relatedSpecificationItem = relatedResult.relatedSpecificationItem;

							if( hasRelationContext &&
							relatedSpecificationItem != NULL &&
							!relatedSpecificationItem->hasRelationContext() &&
							relatedSpecificationItem->isSelfGeneratedAssumption() )
								{
								// Assumption needs to be corrected
								if( correctedAssumptionReplacedSpecificationItem_ != NULL )
									return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The corrected assumption specification item already assigned" );

								if( myWordItem_->writeUpdatedSpecification( false, false, true, false, false, relatedSpecificationItem ) != RESULT_OK )
									return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a conflicting specification" );

								hasCorrectedAssumptionByKnowledge_ = true;
								correctedAssumptionReplacedSpecificationItem_ = relatedSpecificationItem;
								}
							else
								{
								if( !isPossessive &&
								!isQuestion &&
								myWordItem_->isUserGeneralizationWord &&

								( relatedSpecificationItem == NULL ||

								( ( !relatedSpecificationItem->hasRelationContext() ||
								relatedSpecificationItem->specificationWordItem() != specificationWordItem ) &&

								( !specificationWordItem->isNounWordSpanishAmbiguous() ||
								!relatedSpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) &&

								// Skip checking for conflicts if a question with the current specification word exists
								myWordItem_->firstSpecificationItem( false, false, WORD_PARAMETER_SINGULAR_VERB_IS, specificationWordItem ) == NULL )
									{
									// Check for specification conflict
									if( checkForSpecificationConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) != RESULT_OK )
										return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
									}
								}
							}
						}

					if( !commonVariables_->hasDisplayedWarning &&
					!hasRelationContext &&
					isAssignment &&
					!isInactiveAssignment &&
					!isArchivedAssignment &&
					!isNegative &&
					!isValueSpecification &&
					specificationContextNr == NO_CONTEXT_NR &&
					!myWordItem_->isLanguageWord() &&
					myWordItem_->isNounWordType( specificationWordTypeNr ) &&
					// Negative specification doesn't exist
					myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, true, isPossessive, NO_QUESTION_PARAMETER, specificationCollectionNr, generalizationContextNr, relationContextNr, specificationWordItem ) == NULL &&
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

							commonVariables_->hasDisplayedArticleNotification = true;
							}
						}
					}
				}
			else
				{
				if( !hasRelationContext &&
				isAssignment &&
				!isNegative &&
				generalizationContextNr == NO_CONTEXT_NR &&
				specificationContextNr == NO_CONTEXT_NR &&
				foundSpecificationItem->hasRelationContext() )
					{
					if( myWordItem_->writeSelectedSpecification( false, true, foundSpecificationItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					if( strlen( commonVariables_->writtenSentenceString ) == 0 )
						return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I couldn't write a sentence with an assumption about the relation" );

					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_MISSING_RELATION_I_ASSUME_YOU_MEAN ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a missing relation" );

					if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence with an assumption about the relation" );

					// Force 'I know' notification
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the 'I know' interface notification" );
					}
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				if( isQuestion )
					{
					if( checkUserQuestion( hasRelationContext, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, foundSpecificationItem, specificationWordItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the user question" );

					// Find self-generated question
					if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) ).result != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting self-generated question" );

					if( !isOnlyCheckingForConflicts_ &&
					commonVariables_->nUserSpecificationWords > 1 &&
					myWordItem_->sameUserQuestionSpecificationItem( questionParameter ) != NULL )
						{
						if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_QUESTION_YOU_HAD_THE_SAME_QUESTION_BEFORE ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write 'I had the same question before' interface notification" );

						userSpecificationResult.isSameQuestionFromUser = true;
						}
					else
						{
						// Find user question
						if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationContextNr ) ).result != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a same, similar, related, more specific and conflicting user question" );
						}

					isOnlyCheckingForConflicts_ = true;
					}
				else
					{
					if( foundSpecificationItem == NULL )
						{
						if( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL )
							{
							// Find singular word type
							singularNounWordTypeItem = specificationWordItem->activeWordTypeItem( WORD_TYPE_NOUN_SINGULAR );

							if( singularNounWordTypeItem != NULL &&
							// Skip currently created singular noun
							singularNounWordTypeItem->isOlderItem() &&
							processPreviouslyUnknownPluralNoun( specificationWordItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to process a previously unknown plural noun" );
							}
						}
					else
						{
						if( ( userSpecificationResult = checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, isSelection, isValueSpecification, specificationWordTypeNr, specificationCollectionNr, relationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the user specification" );
						}
					}
				}
			}

		return userSpecificationResult;
		}

	UserSpecificationResultType findSameSimilarOrRelatedQuestion( bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isOnlyCheckingForConflicts, bool isPossessive, bool isSelfGenerated, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr )
		{
		bool hasFoundQuestion;
		bool isUserSpecificationWord;
		bool hasFoundOlderSpecification = false;
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
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *foundQuestionSpecificationItem = NULL;
		WordItem *currentWordItem;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findSameSimilarOrRelatedQuestion";

		if( ( currentWordItem = commonVariables_->firstWordItem ) == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The first word item is undefined" );

		// Do for all words, including predefined words
		do	{
			hasFoundQuestion = false;
			isUserSpecificationWord = currentWordItem->isUserSpecificationWord;

			if( ( currentQuestionSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationWordTypeNr, specificationCollectionNr, currentWordItem ) ) != NULL )
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
				isUserSpecificationWord )
					{
					isSimilarQuestion = true;

					if( foundQuestionSpecificationItem != NULL )
						{
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
					}
				else
					{
					if( isUserSpecificationWord ||

					// Found question has specification words that aren't in the user question
					( hasSpecificationCollection &&
					isExclusiveSpecification &&
					currentWordItem->isCompoundCollection( specificationCollectionNr ) ) )
						isSameQuestion = false;

					if( isSpecificationGeneralization ||

					// Prefer similar over related, if both occur
					( foundQuestionSpecificationItem != NULL &&

					( !hasFoundSpecificationRelationContext ||
					!isSameRelationContext ) ) )
						{
						if( !hasRelationContext ||
						hasSpecificationCollection ||
						!isSameAssignment )
							isSimilarQuestion = true;
						else
							isConflictingQuestion = true;
						}
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

					if( strlen( commonVariables_->writtenSentenceString ) == 0 )
						return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I couldn't write the found specification" );

					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( isSelfGenerated ? ( isSimilarQuestion && !isRelatedQuestion ? INTERFACE_QUESTION_I_HAD_A_SIMILAR_QUESTION_BEFORE : INTERFACE_QUESTION_I_HAD_A_RELATED_QUESTION_BEFORE ) : ( isSimilarQuestion && !isRelatedQuestion ? INTERFACE_QUESTION_YOU_HAD_A_SIMILAR_QUESTION_BEFORE : INTERFACE_QUESTION_YOU_HAD_A_RELATED_QUESTION_BEFORE ) ) ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a related question" );

					if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, commonVariables_->writtenSentenceString, commonVariables_->learnedFromUserString ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence about the same, a similar or a relation question" );

					userSpecificationResult.isSimilarOrRelatedQuestion = true;
					}
				}
			else
				{
				if( hasFoundOlderSpecification )
					{
					if( !isOnlyCheckingForConflicts &&

					( !isSameExclusiveSpecification ||
					!isSameRelationContext ) )
						{
						hasDisplayedMoreSpecificQuestion_ = true;

						// Skip displaying announcement if question is in conflict with itself
						if( isExclusiveSpecification ||
						commonVariables_->nUserSpecificationWords == 1 )
							{
							if( writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a more specific related question" );
							}
						}
					}
				else
					{
					if( hasDisplayedMoreSpecificQuestion_ &&
					isConflictingQuestion &&
					inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_LISTING_QUESTION_IN_CONFLICT_WITH_ITSELF ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning" );
					}
				}
			}

		return userSpecificationResult;
		}


	protected:
	// Constructor

	WordSpecification( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasConfirmedSpecification_ = false;
		hasConfirmedSpecificationButNoRelation_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasCorrectedAssumptionByOppositeQuestion_ = false;
		hasDisplayedMoreSpecificSpecification_ = false;
		hasDisplayedMoreSpecificNonExclusiveSpecification_ = false;
		hasDisplayedMoreSpecificQuestion_ = false;
		isThisProperNameWordTouchedDuringCurrentSentence_ = false;
		isOnlyCheckingForConflicts_ = false;

		spanishCompoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = NO_COLLECTION_NR;

		correctedAssumptionReplacedSpecificationItem_ = NULL;
		firstConfirmedReplacedSpanishSpecificationItem_ = NULL;
		replacedAssignmentItem_ = NULL;

		spanishCompoundSpecificationWordItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "WordSpecification" );

		// Checking private initialized variables

		if( ( myWordItem_ = myWordItem ) == NULL )
			{
			if( commonVariables != NULL )
				commonVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given my word item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( commonVariables_ = commonVariables ) == NULL )
				strcpy( errorString, "The given common variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				myWordItem_->startSystemErrorInWord( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeWordSpecificationVariables()
		{
		hasConfirmedSpecification_ = false;
		hasConfirmedSpecificationButNoRelation_ = false;
		hasCorrectedAssumptionByKnowledge_ = false;
		hasDisplayedMoreSpecificSpecification_ = false;
		hasDisplayedMoreSpecificNonExclusiveSpecification_ = false;
		hasDisplayedMoreSpecificQuestion_ = false;
		isThisProperNameWordTouchedDuringCurrentSentence_ = false;
		isOnlyCheckingForConflicts_ = false;

		spanishCompoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = NO_COLLECTION_NR;

		firstConfirmedReplacedSpanishSpecificationItem_ = NULL;
		spanishCompoundSpecificationWordItem_ = NULL;
		}

	bool hasConfirmedSpecification()
		{
		return hasConfirmedSpecification_;
		}

	bool hasConfirmedSpecificationButNoRelation()
		{
		return hasConfirmedSpecificationButNoRelation_;
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

	signed char checkForSpecificationConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationProperName, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationItem *conflictingSpecificationItem;
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForSpecificationConflict";

		if( ( specificationResult = checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );

		conflictingSpecificationItem = specificationResult.specificationItem;

		if( conflictingSpecificationItem != NULL &&
		conflictingSpecificationItem->isOlderItem() &&

		( isNegative ||
		!conflictingSpecificationItem->isNegative() ||
		conflictingSpecificationItem->hasSpecificationCompoundCollection() ) &&

		// Opposite negative of conflicting specification word
		checkSpecificationWordForConflict( isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, !isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationContextNr, conflictingSpecificationItem->specificationWordItem() ).result != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to check for the opposite negative specification conflict" );

		return RESULT_OK;
		}

	CreateAndAssignResultType addSpecificationInWord( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		bool hasCopiedRelationContext = ( copiedRelationContextNr > NO_CONTEXT_NR );
		bool hasFoundSpecificationWithDifferentRelationContext = false;
		bool hasRelationContext = ( relationContextNr > NO_CONTEXT_NR );
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isGeneralizationProperName = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NAME );
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserRelationWord = myWordItem_->isUserRelationWord;
		unsigned int nonCompoundCollectionNr;
		unsigned int originalSentenceNr;
		unsigned int compoundCollectionNr = NO_COLLECTION_NR;
		SpecificationItem *createdSpecificationItem;
		SpecificationItem *replacingSpecificationItem;
		SpecificationItem *confirmedReplacedSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *foundUserSpecificationItem = NULL;
		SpecificationItem *obsoleteAssumptionSpecificationItem = NULL;
		SpecificationItem *obsoleteConcludedAssumptionSpecificationItem = NULL;
		SpecificationItem *obsoleteNotCollectedSpecificationItem = NULL;
		SpecificationItem *obsoleteHiddenSpanishSpecificationItem = NULL;
		SpecificationItem *obsoleteUserSpecificationItem = NULL;
		CreateAndAssignResultType createAndAssignResult;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationInWord";

		if( !isThisProperNameWordTouchedDuringCurrentSentence_ )
			{
			isThisProperNameWordTouchedDuringCurrentSentence_ = true;
			addToTouchedDuringCurrentSentenceProperNameWordList();
			}

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
			collectSpecifications( isAssignment, isQuestion, specificationWordTypeNr, generalizationWordTypeNr, myWordItem_, specificationWordItem ) != RESULT_OK )
				return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect myself with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

			if( hasSpecificationCollection )
				{
				if( !specificationWordItem->hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The given specification word doesn't have the given specification collection" );
				}
			else
				{
				if( !isSelfGenerated &&
				userSpecificationCollectionNr_ > NO_COLLECTION_NR )
					// Use previous user specification collection
					specificationCollectionNr = userSpecificationCollectionNr_;
				else
					{
					compoundCollectionNr = specificationWordItem->compoundCollectionNr( specificationWordTypeNr );
					nonCompoundCollectionNr = specificationWordItem->nonCompoundCollectionNr( specificationWordTypeNr );

					if( nonCompoundCollectionNr > NO_COLLECTION_NR &&

					( ( compoundCollectionNr == NO_COLLECTION_NR &&
					// Skip definition sentence with the same generalization and specification word
					!specificationWordItem->isUserGeneralizationWord ) ||

					( !isQuestion &&

					// Non-exclusive specification collection
					( ( !isExclusiveSpecification &&

					( !isSpecificationWordSpanishAmbiguous ||

					( hasRelationContext &&

					( nContextRelations > 1 ||
					!specificationWordItem->isUserSpecificationWord ) ) ) ) ||

					// Exclusive specification collection
					( isExclusiveSpecification &&
					!hasRelationContext &&
					!isPossessive &&
					!isSelfGenerated &&
					compoundCollectionNr > NO_COLLECTION_NR &&
					commonVariables_->nUserSpecificationWords > 2 ) ) ) ) )
						specificationCollectionNr = nonCompoundCollectionNr;
					else
						specificationCollectionNr = compoundCollectionNr;
					}
				}

			if( !isSelfGenerated &&
			myWordItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				if( specificationCollectionNr > NO_COLLECTION_NR &&

				( !isAssignment ||
				isGeneralizationProperName ) )
					userSpecificationCollectionNr_ = specificationCollectionNr;

				if( ( userSpecificationResult = checkUserSpecificationOrQuestion( hasRelationContext, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isGeneralizationProperName, isNegative, isPossessive, isSelection, isSpecificationGeneralization, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to check a user specification or question with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

				confirmedReplacedSpecificationItem = userSpecificationResult.confirmedReplacedSpecificationItem;
				}

			if( !commonVariables_->hasDisplayedWarning )
				{
				// Try to find specification
				// Start with finding user specification
				if( ( foundUserSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, true, false, ( isExclusiveSpecification && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? NO_CONTEXT_NR : relationContextNr ), specificationWordItem ) ) == NULL )
					{
					if( isSelfGenerated )
						{
						if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( false, false, isArchivedAssignment, false, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationCollectionNr, ( isAssignment && !isArchivedAssignment ? NO_CONTEXT_NR : ( hasCopiedRelationContext ? copiedRelationContextNr : relationContextNr ) ), specificationWordItem ) ) == NULL )
							{
							// Not found
							if( hasRelationContext &&
							!isQuestion &&
							( replacingSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( isArchivedAssignment, isArchivedAssignment, isNegative, isPossessive, NO_QUESTION_PARAMETER, relationContextNr, specificationWordItem ) ) != NULL )
								{
								if( !hasCopiedRelationContext &&
								compoundCollectionNr > NO_COLLECTION_NR &&
								!replacingSpecificationItem->hasSpecificationCollection() &&
								replacingSpecificationItem->isSelfGenerated() )
									obsoleteNotCollectedSpecificationItem = replacingSpecificationItem;
								else
									{
									if( isSpecificationWordSpanishAmbiguous &&
									replacingSpecificationItem->hasRelationContext() )
										{
										if( !isPossessive &&
										replacingSpecificationItem->isSelfGeneratedConclusion() &&
										replacingSpecificationItem->isHiddenSpanishSpecification() )
											obsoleteHiddenSpanishSpecificationItem = replacingSpecificationItem;
										}
									else
										{
										if( isUserRelationWord &&

										( !isSpecificationWordSpanishAmbiguous ||
										hasSpecificationCollection ) &&

										replacingSpecificationItem->isSelfGenerated() )
											{
											if( replacingSpecificationItem->relationContextNr() == relationContextNr )
												foundSpecificationItem = replacingSpecificationItem;
											else
												{
												// Find any specification
												replacingSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( true, true, isNegative, isPossessive, NO_QUESTION_PARAMETER, NO_CONTEXT_NR, specificationWordItem );

												if( replacingSpecificationItem != NULL &&
												!replacingSpecificationItem->hasRelationContext() )
													{
													if( confirmedReplacedSpecificationItem != NULL )
														return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The confirmed replaced specification item is already defined" );

													if( myWordItem_->writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, isSpecificationWordSpanishAmbiguous, false, false, !isSpecificationWordSpanishAmbiguous, replacingSpecificationItem ) != RESULT_OK )
														return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an assumption, which will have an added relation" );

													confirmedReplacedSpecificationItem = replacingSpecificationItem;
													}
												}
											}
										}
									}
								}
							}
						else
							{
							if( foundSpecificationItem->relationContextNr() == relationContextNr )
								{
								if( isGeneralizationProperName &&
								firstJustificationItem != NULL &&
								foundSpecificationItem->isOlderItem() )
									{
									if( firstJustificationItem->isConclusionJustification() )
										{
										if( foundSpecificationItem->isSelfGeneratedAssumption() &&

										( relationWordItem == NULL ||

										( relationWordItem->isOlderItem() &&

										( foundSpecificationItem->hasSpecificationCompoundCollection() ||

										( isUserRelationWord &&
										foundSpecificationItem->olderJustificationItem( JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ) != NULL &&

										( isSpecificationWordSpanishAmbiguous ||
										foundSpecificationItem->hasOnlyOneRelationWord() ) ) ) &&

										( ( !isSpecificationWordSpanishAmbiguous &&
										// Conclusion doesn't exist
										myWordItem_->firstSelfGeneratedCheckSpecificationItem( true, isArchivedAssignment, isNegative, isPossessive, false, specificationWordItem, NULL ) == NULL ) ||

										// Typically for the Spanish language
										( isSpecificationWordSpanishAmbiguous &&
										!foundSpecificationItem->isHiddenSpanishSpecification() ) ) ) ) )
											{
											if( foundSpecificationItem->markAsConcludedAssumption( false ) != RESULT_OK )
												return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark the found specification as concluded assumption" );

											if( isUserRelationWord ||
											foundSpecificationItem->isPartOf() ||

											( foundSpecificationItem->isNegative() &&
											!myWordItem_->hasAnsweredSelfGeneratedQuestion() ) )
												{
												if( myWordItem_->writeUpdatedSpecification( true, false, false, false, false, foundSpecificationItem ) != RESULT_OK )
													return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a concluded assumption" );
												}
											}
										}
									else
										{
										if( hasRelationContext &&
										!isSpecificationWordSpanishAmbiguous )
											{
											if( isPossessive )
												{
												if( !isUserRelationWord &&
												foundSpecificationItem->isConcludedAssumption() )
													obsoleteConcludedAssumptionSpecificationItem = foundSpecificationItem;
												}
											else
												{
												if( isUserRelationWord &&
												relationWordItem != NULL &&
												!relationWordItem->isOlderItem() &&
												!foundSpecificationItem->hasAssumptionLevel() &&
												!foundSpecificationItem->hasSpecificationCompoundCollection() )
													// Conclusion becomes assumption
													// No specification will be created
													foundSpecificationItem->incrementAssumptionLevel();
												}
											}
										}
									}
								}
							else
								{
								if( hasRelationContext &&
								foundSpecificationItem->hasRelationContext() )
									{
									hasFoundSpecificationWithDifferentRelationContext = true;

									if( !isSpecificationWordSpanishAmbiguous &&
									firstJustificationItem != NULL &&

									( ( foundSpecificationItem->isSelfGeneratedAssumption() &&
									firstJustificationItem->isOppositePossessiveConditionalSpecificationAssumption() ) ||

									( foundSpecificationItem->isSelfGeneratedConclusion() &&
									firstJustificationItem->isPossessiveReversibleConclusion() ) ) )
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

							if( isGeneralizationProperName &&
							specificationCollectionNr > NO_COLLECTION_NR &&

							( ( isAssignment &&
							confirmedReplacedSpecificationItem != NULL &&
							foundUserSpecificationItem->isUserAssignment() ) ||

							!foundUserSpecificationItem->hasSpecificationCollection() ) )
								// Prepare foundUserSpecificationItem for confirmation
								foundUserSpecificationItem = myWordItem_->firstSpecificationItem( isPossessive, false, NO_QUESTION_PARAMETER, specificationWordItem );
							else
								{
								if( !hasRelationContext ||
								!isQuestion ||
								!foundUserSpecificationItem->hasRelationContext() ||
								foundUserSpecificationItem->relationContextNr() == relationContextNr )
									foundUserSpecificationItem = NULL;
								else
									{
									// Question with different relation context
									foundSpecificationItem = NULL;
									foundUserSpecificationItem = myWordItem_->bestMatchingRelationContextNrSpecificationItem( isNegative, isPossessive, questionParameter, specificationWordItem, relationWordItem );

									if( foundUserSpecificationItem != NULL &&
									foundUserSpecificationItem->assumptionLevel() != assumptionLevel )
										foundUserSpecificationItem = NULL;
									}
								}
							}
						else	// Typically for compound collections in Spanish
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

								if( spanishCompoundSpecificationWordItem_->addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", spanishCompoundSpecificationWordItem_->anyWordTypeString(), "\" with word \"", specificationWordItem->anyWordTypeString(), "\"" );

								if( specificationWordItem->addCollection( isExclusiveSpecification, false, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishCompoundSpecificationWordItem_, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect word \"", specificationWordItem->anyWordTypeString(), "\" with word \"", spanishCompoundSpecificationWordItem_->anyWordTypeString(), "\"" );
								}
							}
						}
					else
						{
						if( foundUserSpecificationItem->isSpecificationWordSpanishAmbiguous() )
							specificationCollectionNr = myWordItem_->collectionNr( WORD_TYPE_NOUN_SINGULAR, myWordItem_ );
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

		if( !commonVariables_->hasDisplayedWarning )
			{
			if( foundSpecificationItem == NULL ||

			// Exceptions
			confirmedReplacedSpecificationItem != NULL ||
			correctedAssumptionReplacedSpecificationItem_ != NULL ||
			obsoleteConcludedAssumptionSpecificationItem != NULL ||
			userSpecificationResult.isNonExclusiveSpecification ||

			// Overwrite non-conditional specification by conditional one
			( isConditional &&
			!foundSpecificationItem->isConditional() ) ||

			// Overwrite non-exclusive specification by exclusive one
			( isExclusiveSpecification &&
			!foundSpecificationItem->isExclusiveSpecification() ) ||

			// Overwrite user questions by non-exclusive one
			( !isAssignment &&
			isQuestion &&
			!isSelfGenerated &&
			!userSpecificationResult.isSameQuestionFromUser &&
			foundSpecificationItem->isExclusiveSpecification() ) ||

			// Overwrite self-generated assignment by user assignment
			( isAssignment &&
			!isSelfGenerated &&
			foundSpecificationItem->isSelfGenerated() ) ||

			// User specification has copied relation context
			( hasCopiedRelationContext &&
			isSelfGenerated &&
			foundSpecificationItem->isUserSpecification() ) ||

			// Accept different relation context (e.g. ambiguous specification)
			( hasFoundSpecificationWithDifferentRelationContext &&
			!isArchivedAssignment &&
			isSelfGenerated ) )
				{
				if( foundSpecificationItem != NULL )
					{
					if( !isConditional &&
					foundSpecificationItem->isConditional() )
						isConditional = true;

					if( !isExclusiveSpecification &&
					!isQuestion &&
					foundSpecificationItem->isExclusiveSpecification() )
						isExclusiveSpecification = true;

					if( !isQuestion &&
					!isSelfGenerated &&
					foundSpecificationItem != confirmedReplacedSpecificationItem &&
					foundSpecificationItem->isUserSpecification() )
						obsoleteUserSpecificationItem = foundSpecificationItem;
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
									foundSpecificationItem == NULL ? commonVariables_->currentSentenceNr : foundSpecificationItem->originalSentenceNr() );

				// Create the actual specification
				if( ( createAndAssignResult = myWordItem_->createSpecificationItem( false, false, false, false, false, isConditional, ( hasCorrectedAssumptionByKnowledge_ || hasCorrectedAssumptionByOppositeQuestion_ ), isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, NO_ASSIGNMENT_LEVEL, assumptionLevel, commonVariables_->currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? NO_WORD_TYPE_NR : relationWordTypeNr ), specificationCollectionNr, generalizationContextNr, NO_CONTEXT_NR, ( isAssignment ? NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, ( isAssignment ? 0 : nContextRelations ), firstJustificationItem, specificationWordItem, specificationString, NULL ) ).result != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to create a specification item" );

				if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
					return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I couldn't create a specification item" );

				// Now replaced obsolete specifications by the created specification
				if( replaceObsoleteSpecification( confirmedReplacedSpecificationItem, userSpecificationResult.confirmedSpecificationButNoRelationReplacedSpecificationItem, createdSpecificationItem, foundUserSpecificationItem, obsoleteAssumptionSpecificationItem, obsoleteConcludedAssumptionSpecificationItem, obsoleteHiddenSpanishSpecificationItem, obsoleteNotCollectedSpecificationItem, obsoleteUserSpecificationItem ) != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete specification" );

				if( hasCopiedRelationContext &&
				firstJustificationItem != NULL &&
				foundSpecificationItem != NULL &&

				( isPossessive ||
				foundSpecificationItem->isSelfGeneratedAssumption() ||
				foundSpecificationItem->isUserSpecification() ) )
					{
					if( !firstJustificationItem->isOlderItem() &&

					( isSpecificationWordSpanishAmbiguous ||
					foundSpecificationItem->hasCurrentCreationSentenceNr() ||
					foundSpecificationItem->isConcludedAssumption() ) )
						{
						if( firstJustificationItem->changeAttachedJustification( foundSpecificationItem->firstJustificationItem() ) != RESULT_OK )
							return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
						}

					if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
						return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );

					if( foundSpecificationItem->isSelfGeneratedAssumption() &&
					!foundSpecificationItem->isSpecificationWordSpanishAmbiguous() &&

					// Clean-up obsolete justifications
					myWordItem_->replaceOrDeleteObsoleteJustifications() != RESULT_OK )
						return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete obsolete justifications" );
					}

				if( !isNegative &&
				!isPossessive &&
				!isQuestion &&
				specificationWordItem != NULL &&
				!userSpecificationResult.isSimilarOrRelatedQuestion &&

				( !isSpecificationWordSpanishAmbiguous ||
				!createdSpecificationItem->isHiddenSpanishSpecification() ) )
					{
					if( myWordItem_->findAnswersToQuestions( specificationWordItem->compoundCollectionNr( specificationWordTypeNr ), createdSpecificationItem ) != RESULT_OK )
						return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to find answers to questions" );

					if( !isSelfGenerated &&
					!myWordItem_->isFemaleOrMale() )
						{
						if( specificationWordItem->isFemale() )
							{
							if( myWordItem_->markWordAsFemale() != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as female" );
							}
						else
							{
							if( specificationWordItem->isMale() )
								{
								if( myWordItem_->markWordAsMale() != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as male" );

								if( !isAssignment &&
								isGeneralizationProperName &&
								myWordItem_->isSpanishCurrentLanguage() &&
								displayAssumptionsThatAreNotHiddenAnymore() != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to display assumptions that are not hidden anymore" );
								}
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( searchSpecificationItem == NULL )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given search specification item is undefined" );

		return findRelatedSpecification( false, isCheckingRelationContext, false, false, searchSpecificationItem->isAssignment(), searchSpecificationItem->isInactiveAssignment(), searchSpecificationItem->isArchivedAssignment(), searchSpecificationItem->isExclusiveSpecification(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPossessive(), searchSpecificationItem->questionParameter(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationContextNr(), searchSpecificationItem->relationContextNr(), searchSpecificationItem->specificationWordItem() );
		}

	RelatedResultType findRelatedSpecification( bool isCheckingAllQuestions, bool isIgnoringExclusive, bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findRelatedSpecification";

		if( isIncludingAssignments )
			{
			if( ( relatedResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			relatedResult.relatedSpecificationItem == NULL &&
			// Archived assignments
			( relatedResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification as well" );
			}

		// Specifications
		if( relatedResult.relatedSpecificationItem == NULL &&
		( relatedResult = findRelatedSpecification( isCheckingAllQuestions, false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

		return relatedResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
