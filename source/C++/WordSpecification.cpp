/*	Class:			WordSpecification
 *	Supports class:	WordItem
 *	Purpose:		Creating specification structures
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
 *************************************************************************/
/*	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, eit	her version 2 of the License, or
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

	bool hasCurrentlyConfirmedSpecification_ = false;
	bool hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = false;
	bool hasCurrentlyConfirmedSpecificationButNoRelation_ = false;
	bool hasCurrentlyCorrectedAssumptionByKnowledge_ = false;
	bool hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ = false;
	bool hasCurrentlyMoreSpecificSpecification_ = false;
	bool isOnlyCheckingForConflicts_ = false;
	bool isWordTouchedDuringCurrentSentence_ = false;

	unsigned int compoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
	unsigned int userSpecificationCollectionNr_ = NO_COLLECTION_NR;

	SpecificationItem *correctedAssumptionReplacedSpecificationItem_ = NULL;
	SpecificationItem *previousConflictingSpecificationItem_ = NULL;
	SpecificationItem *replacedAssignmentItem_ = NULL;

	WordItem *spanishPreviousSpecificationWordItem_ = NULL;

//Java_private_final
	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "WordSpecification";

	// Private initialized variables

	GlobalVariables *globalVariables_;
	InputOutput *inputOutput_;
	WordItem *myWordItem_;


	// Private functions

	bool hasOnlyEitherFeminineOrMasculineSpecificationWords( bool isFeminineWord )
		{
		WordItem *currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) != NULL )
			{
			// Do for all words touched during current sentence
			do	{
				if( currentSpecificationWordItem->isUserSpecificationWord &&
				// Found difference
				currentSpecificationWordItem->isFeminineWord() != isFeminineWord )
					return false;
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
			}

		return true;
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

	signed char checkUserQuestion( bool hasRelationWord, bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, SpecificationItem *foundQuestionSpecificationItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationItem *relatedSpecificationItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkUserQuestion";

		if( foundQuestionSpecificationItem != NULL &&
		foundQuestionSpecificationItem->isAnsweredQuestion() &&
		foundQuestionSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification &&
		foundQuestionSpecificationItem->relationCollectionNr() == relationCollectionNr &&
		foundQuestionSpecificationItem->relationWordItem() == relationWordItem )
			globalVariables_->isQuestionAlreadyAnswered = true;

		if( specificationCollectionNr > NO_COLLECTION_NR &&

		( foundQuestionSpecificationItem == NULL ||
		!foundQuestionSpecificationItem->isQuestion() ) )
			{
			if( ( relatedResult = findRelatedSpecification( false, false, isAssignment, isAssignment, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related question" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
			!isExclusiveSpecification &&
			!isSpecificationGeneralization )
				{
				if( relatedSpecificationItem->isOlderItem() )
					{
					if( hasRelationWord )
						{
						if( correctedAssumptionReplacedSpecificationItem_ != NULL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The corrected assumption replaced specification item already assigned" );

						if( ( correctedAssumptionReplacedSpecificationItem_ = myWordItem_->firstSelfGeneratedCheckSpecificationItem( true, true, relatedSpecificationItem->isNegative(), relatedSpecificationItem->isPossessive(), true, NO_QUESTION_PARAMETER, relatedSpecificationItem->specificationCollectionNr(), relatedSpecificationItem->relationCollectionNr(), relatedSpecificationItem->specificationWordItem(), relatedSpecificationItem->relationWordItem() ) ) != NULL )
							{
							// Write assumption that needs to be corrected
							if( myWordItem_->writeUpdatedSpecification( false, false, true, false, false, false, false, correctedAssumptionReplacedSpecificationItem_ ) != RESULT_OK )
								return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a corrected assumption" );

							hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ = true;
							}
						}
					}
				else
					{
					if( relatedSpecificationItem->hasCompoundSpecificationCollection() )
						{
						// Current question in conflict with definition
						if( writeSpecificationInConflictWithDefinition( true, relatedSpecificationItem->specificationWordItem(), specificationWordItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write that the current question is in conflict with a definition" );

						globalVariables_->isConflictingQuestion = true;
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char collectSimpleSpecifications( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *collectionWordItem )
		{
		unsigned int specificationCollectionNr;
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundGeneralizationWordItem = NULL;
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "collectSimpleSpecifications";

		if( collectionWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given collection word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = myWordItem_->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startSystemErrorInWord( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( currentGeneralizationWordItem != collectionWordItem &&
				// Avoid idle call
				currentGeneralizationWordItem->firstExclusiveSpecificationItem( true ) != NULL )
					{
					// Collect specification words exclusively
					if( ( collectionResult = currentGeneralizationWordItem->collectExclusiveSpecificationWords( collectionWordTypeNr, commonWordTypeNr, myWordItem_, collectionWordItem ) ).result != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to exclusively collect specification words in generalization word \"", currentGeneralizationWordItem->anyWordTypeString(), "\"" );

					if( ( foundGeneralizationWordItem = collectionResult.foundGeneralizationWordItem ) != NULL &&
					( specificationCollectionNr = collectionWordItem->collectionNr( currentGeneralizationWordItem ) ) > NO_COLLECTION_NR &&
					// Collect by generalization
					foundGeneralizationWordItem->collectSpecificationsInWord( false, false, specificationCollectionNr ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to collect specifications in word \"", foundGeneralizationWordItem->anyWordTypeString(), "\"" );
					}
				}
			while( foundGeneralizationWordItem == NULL &&
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

	signed char replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteSpecification";

		if( obsoleteSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given obsolete specification item is undefined" );

		if( obsoleteSpecificationItem->isAssignment() )
			replacedAssignmentItem_ = obsoleteSpecificationItem;
		else
			{
			if( myWordItem_->replaceOrDeleteSpecification( obsoleteSpecificationItem, replacingSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a specification" );
			}

		return RESULT_OK;
		}

	signed char replaceObsoleteSpecification( bool isConfirmedSpecificationButNoneOfItsRelations, SpecificationItem *confirmedSpecificationItem, SpecificationItem *createdSpecificationItem, SpecificationItem *obsoleteSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceObsoleteSpecification";

		if( createdSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given created specification is undefined" );

		if( confirmedSpecificationItem != NULL &&
		// Replace confirmed specification
		replaceOrDeleteSpecification( confirmedSpecificationItem, createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the confirmed specification" );

		if( isConfirmedSpecificationButNoneOfItsRelations &&
		myWordItem_->confirmSpecificationButNotItsRelation( createdSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to confirm a specification, but not its relation(s)" );

		if( correctedAssumptionReplacedSpecificationItem_ != NULL )
			{
			if( replaceOrDeleteSpecification( correctedAssumptionReplacedSpecificationItem_, createdSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete a corrected assumption replaced specification" );

			correctedAssumptionReplacedSpecificationItem_ = NULL;
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

		if( myWordItem_->writeSelectedSpecification( false, false, olderSpecificationItem ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the given older specification" );

		if( ( writtenSentenceString = globalVariables_->writtenSentenceString ) == NULL ||
		strlen( writtenSentenceString ) == 0 )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't write the given older specification" );

		if( inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_NOTIFICATION, ( olderSpecificationItem->isQuestion() ? ( olderSpecificationItem->isSelfGeneratedSpecification() ? INTERFACE_LISTING_YOUR_QUESTION_IS_MORE_SPECIFIC_THAN_MY_QUESTION : INTERFACE_LISTING_THIS_QUESTION_IS_MORE_SPECIFIC_THAN_YOUR_QUESTION ) : ( olderSpecificationItem->isSelfGeneratedSpecification() ? ( olderSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_ASSUMPTION : INTERFACE_LISTING_YOUR_INFO_IS_MORE_SPECIFIC_THAN_MY_CONCLUSION ) : INTERFACE_LISTING_THIS_INFO_IS_MORE_SPECIFIC_THAN_YOUR_EARLIER_INFO ) ) ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface listing text" );

		if( inputOutput_->writeText( INPUT_OUTPUT_PROMPT_WRITE, writtenSentenceString, globalVariables_->learnedFromUserString ) != RESULT_OK )
			return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a sentence" );

		return RESULT_OK;
		}

	signed char writeSpecificationInConflictWithDefinition( bool isQuestion, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *definitionSpecificationItem;
		WordItem *currentGeneralizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeSpecificationInConflictWithDefinition";

		if( previousSpecificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given previous specification word item is undefined" );

		if( currentSpecificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given current specification word item is undefined" );

		// Get first generalization specification noun word
		if( ( currentGeneralizationItem = currentSpecificationWordItem->firstNounSpecificationGeneralizationItem() ) != NULL )
			{
			// Do for all generalization specification noun words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return myWordItem_->startSystemErrorInWord( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( definitionSpecificationItem = currentGeneralizationWordItem->firstExclusiveSpecificationItem( currentSpecificationWordItem ) ) != NULL &&
				currentGeneralizationWordItem->firstSpecificationItem( false, false, false, previousSpecificationWordItem ) != NULL &&
				// Write conflicting specification
				definitionSpecificationItem->writeSpecificationConflict( isQuestion, false ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an exclusive conflicting specification" );
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextNounSpecificationGeneralizationItem() ) != NULL );
			}

		return RESULT_OK;
		}

	RelatedResultType findRelatedSpecification( bool isCheckingRelationContext, bool isIgnoringExclusive, bool isIgnoringNegative, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool isFirstRelatedSpecification = true;
		bool isLastRelatedSpecification = false;
		unsigned int currentSpecificationSentenceNr;
		unsigned int highestSentenceNr = NO_SENTENCE_NR;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( specificationWordItem == NULL )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = myWordItem_->firstSpecificationItem( false, isAssignment, isInactiveAssignment, isArchivedAssignment, questionParameter ) ) != NULL )
			{
			do	{
				if( ( currentSpecificationWordItem = currentSpecificationItem->relatedSpecificationWordItem( isCheckingRelationContext, isIgnoringExclusive, isIgnoringNegative, isExclusiveSpecification, isNegative, isPossessive, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, relationWordItem ) ) != NULL )
					{
					currentSpecificationSentenceNr = currentSpecificationItem->creationSentenceNr();

					if( currentSpecificationWordItem == specificationWordItem &&
					currentSpecificationItem->relationCollectionNr() == relationCollectionNr )
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

	UserSpecificationResultType checkUserSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSpecificationWordSpanishAmbiguous, bool isValueSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, SpecificationItem *foundSpecificationItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundExclusiveSpecification;
		bool hasFoundRelationWord;
		bool hasRelationWord = ( relationWordItem != NULL );
		unsigned int foundRelationCollectionNr;
		JustificationItem *obsoleteJustificationItem;
		JustificationItem *predecessorObsoleteJustificationItem;
		SpecificationItem *assumptionSpecificationItem;
		SpecificationItem *oldUserSpecificationItem = NULL;
		SpecificationItem *relatedSpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *userSpecificationItem;
		WordItem *foundRelationWordItem;
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

		hasFoundExclusiveSpecification = foundSpecificationItem->isExclusiveSpecification();

		// No relation word
		if( !hasRelationWord &&
		hasFoundExclusiveSpecification &&
		!isAssignment &&
		!isExclusiveSpecification )
			{
			if( writeMoreSpecificSpecification( foundSpecificationItem ) != RESULT_OK )
				return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an notification about a more specific non-exclusive specification" );

			userSpecificationResult.isNonExclusiveSpecification = true;

			if( ( relatedResult = findRelatedSpecification( false, foundSpecificationItem ) ).result != RESULT_OK )
				return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find the related part of a more specific specification" );

			if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
			( obsoleteJustificationItem = myWordItem_->primarySpecificationJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION, relatedSpecificationItem ) ) != NULL &&
			( secondarySpecificationItem = obsoleteJustificationItem->secondarySpecificationItem() ) != NULL &&
			( assumptionSpecificationItem = myWordItem_->firstAssignmentOrSpecificationItem( true, true, false, false, false, secondarySpecificationItem->specificationWordItem() ) ) != NULL &&
			( predecessorObsoleteJustificationItem = assumptionSpecificationItem->firstJustificationItem() ) != NULL )
				{
				// Copy justification
				if( ( justificationResult = myWordItem_->copyJustificationItem( predecessorObsoleteJustificationItem->primarySpecificationItem(), predecessorObsoleteJustificationItem->secondarySpecificationItem(), NULL, predecessorObsoleteJustificationItem ) ).result != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to copy the predecessor obsolete justification item" );

				if( justificationResult.createdJustificationItem == NULL )
					return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The created justification item is undefined" );

				// Replace predecessor justification
				if( myWordItem_->replaceJustification( predecessorObsoleteJustificationItem, justificationResult.createdJustificationItem ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace a predecessor obsolete justification item" );

				// Replace obsolete justification
				if( myWordItem_->replaceJustification( obsoleteJustificationItem, NULL ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete justification item" );
				}
			}
		else
			{
			foundRelationWordItem = foundSpecificationItem->relationWordItem();
			hasFoundRelationWord = ( foundRelationWordItem != NULL );

			// Relation word
			if( hasRelationWord &&
			foundSpecificationItem->isOlderItem() )
				{
				if( foundSpecificationItem->isArchivedAssignment() == isArchivedAssignment &&
				// Check if negative specification exists
				myWordItem_->firstAssignmentOrSpecificationItem( true, true, true, isPossessive, false, specificationWordItem ) == NULL )
					{
					if( hasFoundRelationWord &&
					isPossessive &&
					relationWordItem != NULL &&
					specificationWordItem->isFeminineWord() != relationWordItem->isFeminineWord() &&
					specificationWordItem->isMasculineWord() != relationWordItem->isMasculineWord() &&
					// Check for specification conflict
					checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );

					if( !isInactiveAssignment &&

						( !hasFoundRelationWord ||

						( !foundSpecificationItem->isExclusiveGeneralization() &&
						// An self-generated assumption was found,
						// but there is also a user specification without relation word
						( oldUserSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, false, isPossessive, false, NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != NULL &&
						!oldUserSpecificationItem->hasRelationWord() ) ) )
						{
						if( writeMoreSpecificSpecification( oldUserSpecificationItem == NULL ? foundSpecificationItem : oldUserSpecificationItem ) != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an notification about a more specific related specification" );

						hasCurrentlyMoreSpecificSpecification_ = true;
						}
					}
				else
					{
					if( checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
					}
				}

			// No conflict found
			if( !globalVariables_->hasDisplayedWarning )
				{
				foundRelationCollectionNr = foundSpecificationItem->relationCollectionNr();

				if( foundRelationWordItem != NULL &&

					( ( !hasRelationWord &&
					foundRelationCollectionNr == NO_COLLECTION_NR ) ||

					// Current user specification has less relation words than the found specification
					( foundRelationCollectionNr > NO_COLLECTION_NR &&
					foundRelationCollectionNr != relationCollectionNr &&
					globalVariables_->nUserRelationWords < myWordItem_->nRelationWords( foundRelationCollectionNr, foundRelationWordItem ) ) ) )
					{
					if( !foundSpecificationItem->isHiddenSpanishSpecification() )
						{
						if( hasRelationWord )
							{
							hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation_ = true;

							if( writeConfirmedSpecification( ( foundSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_ASSUMPTION : INTERFACE_LISTING_CONFIRMED_SPECIFICATION_AND_AT_LEAST_ONE_RELATION_OF_MY_CONCLUSION ), foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed specification" );
							}
						else
							{
							if( myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, specificationWordItem ) == NULL )
								{
								hasCurrentlyConfirmedSpecificationButNoRelation_ = true;
								userSpecificationResult.isConfirmedSpecificationButNoneOfItsRelations = true;
								}
							}
						}
					}
				else
					{
					if( hasRelationWord )
						{
						if( !hasFoundRelationWord ||

						( ( relationCollectionNr == NO_COLLECTION_NR ||
						foundRelationCollectionNr == relationCollectionNr ) &&

						!foundSpecificationItem->isOlderItem() ) )
							// Confirmed relation words
							foundSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( hasFoundRelationWord, false, isNegative, isPossessive, true, NO_QUESTION_PARAMETER, specificationCollectionNr, foundRelationCollectionNr, specificationWordItem, foundRelationWordItem );
						}

					if( foundSpecificationItem == NULL )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						hasCurrentlyConfirmedSpecification_ &&
						( foundSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, false, false, isPossessive, true, specificationWordItem, NULL ) ) != NULL &&
						( userSpecificationItem = myWordItem_->firstSpecificationItem( false, isPossessive, false, specificationWordItem ) ) != NULL )
							{
							// Write confirmed specification
							if( writeConfirmedSpecification( INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_CONFIRMED, foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write another confirmed Spanish specification" );

							if( replaceOrDeleteSpecification( foundSpecificationItem, userSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete another confirmed Spanish specification" );
							}
						}
					else
						{
						// Confirmation: Replace a self-generated by a user-entered specification
						if( foundSpecificationItem->isSelfGeneratedSpecification() &&

							( foundRelationWordItem == NULL ||
							foundRelationWordItem == relationWordItem ||
							foundRelationWordItem->isUserRelationWord ) &&

						( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN ||
						// Typical for Spanish
						// Test file: "reasoning\family\I know (5)"
						!myWordItem_->isNounWordSpanishAmbiguous() ) )
							{
							hasCurrentlyConfirmedSpecification_ = true;
							globalVariables_->hasConfirmedAnySpecification = true;

							if( !foundSpecificationItem->isHiddenSpanishSpecification() &&
							// Write confirmed specification
							writeConfirmedSpecification( ( foundSpecificationItem->isSelfGeneratedAssumption() ? INTERFACE_LISTING_MY_ASSUMPTIONS_THAT_HAVE_BEEN_CONFIRMED : INTERFACE_LISTING_MY_CONCLUSIONS_THAT_HAVE_BEEN_CONFIRMED ), foundSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write the confirmed specification" );

							userSpecificationResult.confirmedSpecificationItem = foundSpecificationItem;
							}

						if( generalizationWordTypeNr == WORD_TYPE_NOUN_PLURAL ||

						( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
						foundSpecificationItem->isSingularNounSpecification() ) )
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

	UserSpecificationResultType checkUserSpecificationOrQuestion( bool hasRelationWord, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isSpecificationWordSpanishAmbiguous, bool isValueSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFeminineSingularNounEnding;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isSingularNounGeneralizationWord = ( generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR );
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordTypeItem *foundWordTypeItem;
		WordTypeItem *singularNounWordTypeItem;
		char *writtenSentenceString;
		RelatedResultType relatedResult;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkUserSpecificationOrQuestion";

		hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ = false;

		if( specificationWordItem == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// Check current assignment, specification or question (with relation)
		if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationSpecificationItem( true, true, true, true, true, isNegative, isPossessive, isQuestion, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) ) != NULL &&
		!isArchivedAssignment &&
		!isQuestion &&
		foundSpecificationItem->isArchivedAssignment() &&
		// Check for specification conflict
		// Test file: "Past tense - John was the father of Paul - John is a man"
		checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != RESULT_OK )
			return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );

		if( isQuestion &&
		foundSpecificationItem == NULL )
			foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, (unsigned short)NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem );

		if( foundSpecificationItem == NULL )
			{
			// Check current assignment or specification (without this relation)
			if( isQuestion )
				foundSpecificationItem = myWordItem_->firstUserSpecificationItem( isNegative, isPossessive, NO_COLLECTION_NR, specificationWordItem );
			else
				foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, true, isNegative, isPossessive, NO_COLLECTION_NR, NO_COLLECTION_NR, specificationWordItem, NULL );

			if( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN )
				{
				// Exclusive specifications are not conflicting
				if( !isExclusiveSpecification &&
				// Possessive specifications are not conflicting
				!isPossessive &&
				foundSpecificationItem == NULL )
					{
					if( !isNegative &&
					!myWordItem_->isFeminineOrMasculineWord() &&
					myWordItem_->hasFeminineProperNounEnding() != specificationWordItem->isFeminineWord() &&
					myWordItem_->hasMasculineProperNounEnding() != specificationWordItem->isMasculineWord() &&
					inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_PROPER_NOUN_TO_BE, myWordItem_->anyWordTypeString(), ( myWordItem_->hasFeminineProperNounEnding() ? INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_FEMININE : INTERFACE_SENTENCE_NOTIFICATION_I_EXPECTED_MASCULINE ) ) != RESULT_OK )
						return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface notification about a proper noun expected to be feminine or masculine" );

					if( specificationCollectionNr > NO_COLLECTION_NR )
						{
						if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
							return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

						relatedSpecificationItem = relatedResult.relatedSpecificationItem;

						if( hasRelationWord &&
						relatedSpecificationItem != NULL &&
						relatedSpecificationItem->isSelfGeneratedAssumption() &&
						specificationWordItem->isExclusiveCollection( specificationCollectionNr ) )
							{
							// Assumption needs to be corrected
							if( correctedAssumptionReplacedSpecificationItem_ != NULL )
								return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The corrected assumption specification item already assigned" );

							// Write assumption that needs to be corrected
							if( myWordItem_->writeUpdatedSpecification( false, true, false, false, false, false, false, relatedSpecificationItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an assumption that needs to be corrected" );

							hasCurrentlyCorrectedAssumptionByKnowledge_ = true;
							correctedAssumptionReplacedSpecificationItem_ = relatedSpecificationItem;
							}
						else
							{
							if( !isQuestion &&

								( relatedSpecificationItem == NULL ||

								( ( !relatedSpecificationItem->isOlderItem() ||
								specificationWordItem->isExclusiveCollection( specificationCollectionNr ) ) &&

								( !isSpecificationWordSpanishAmbiguous ||
								!relatedSpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) &&

							// Skip when question with the current specification word exists
							myWordItem_->firstSpecificationItem( false, false, true, specificationWordItem ) == NULL &&
							// Check for specification conflict
							checkForSpecificationConflict( isArchivedAssignment, isNegative, isPossessive, false, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, specificationWordItem, relationWordItem ) != RESULT_OK )
								return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check for a specification conflict" );
							}
						}
					}

				if( !hasRelationWord &&
				isAssignment &&
				!isArchivedAssignment &&
				!isValueSpecification &&
				specificationContextNr == NO_CONTEXT_NR &&
				// Write warning
				inputOutput_->writeInterfaceText( true, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_SENTENCE_WARNING_AMBIGUOUS_SENTENCE_MISSING_RELATION ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an interface warning about ambiguity" );
				}
			else
				{
				if( !isNegative &&
				isSingularNounGeneralizationWord &&
				!myWordItem_->isFeminineOrMasculineWord() &&
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
			if( !hasRelationWord &&
			isAssignment &&
			generalizationContextNr == NO_CONTEXT_NR &&
			foundSpecificationItem->hasRelationWord() )
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

		if( isQuestion )
			{
			if( checkUserQuestion( hasRelationWord, isAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, foundSpecificationItem, specificationWordItem, relationWordItem ) != RESULT_OK )
				return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the user question" );

			// Find self-generated question
			if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, true, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationCollectionNr, relationWordItem ) ).result != RESULT_OK )
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
				if( ( userSpecificationResult = findSameSimilarOrRelatedQuestion( isAssignment, isExclusiveSpecification, isNegative, isOnlyCheckingForConflicts_, isPossessive, false, isSpecificationGeneralization, assumptionLevel, questionParameter, specificationCollectionNr, relationCollectionNr, relationWordItem ) ).result != RESULT_OK )
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
				// Process previously unknown plural noun
				processPreviouslyUnknownPluralNoun( specificationWordItem ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to process a previously unknown plural noun" );
				}
			else
				{
				if( ( userSpecificationResult = checkUserSpecification( isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationWordSpanishAmbiguous, isValueSpecification, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, foundSpecificationItem, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to check the user specification" );
				}
			}

		return userSpecificationResult;
		}

	UserSpecificationResultType findSameSimilarOrRelatedQuestion( bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isOnlyCheckingForConflicts, bool isPossessive, bool isSelfGenerated, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, WordItem *relationWordItem )
		{
		bool hasFoundOlderSpecification = false;
		bool hasFoundQuestion;
		bool hasFoundSpecificationRelationWord = false;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isConflictingQuestion = false;
		bool isRelatedQuestion = false;
		bool isSameAssignment = false;
		bool isSameExclusiveSpecification = false;
		bool isSameRelation = false;
		bool isSameQuestion = true;
		bool isSimilarQuestion = false;
		bool isUserSpecificationWord;
		SpecificationItem *currentQuestionSpecificationItem;
		SpecificationItem *foundQuestionSpecificationItem = NULL;
		SpecificationItem *tempSpecificationItem;
		WordItem *currentWordItem;
		WordItem *tempSpecificationWordItem;
		char *writtenSentenceString;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findSameSimilarOrRelatedQuestion";

		if( ( currentWordItem = globalVariables_->firstWordItem ) == NULL )
			return myWordItem_->startUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "The first word item is undefined" );

		// Do for all words, including predefined words
		do	{
			hasFoundQuestion = false;
			isUserSpecificationWord = currentWordItem->isUserSpecificationWord;

			if( ( currentQuestionSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, currentWordItem ) ) == NULL )
				{
				// Find similar or more specific alternative question
				if( ( currentQuestionSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( !isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, specificationCollectionNr, currentWordItem ) ) == NULL &&
				// Find alternative question without specification collection
				( tempSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, questionParameter, NO_COLLECTION_NR, currentWordItem ) ) != NULL &&
				( tempSpecificationWordItem = tempSpecificationItem->specificationWordItem() ) != NULL &&
				tempSpecificationWordItem->isUserSpecificationWord )
					currentQuestionSpecificationItem = tempSpecificationItem;
				}
			else
				{
				if( hasRelationWord &&
				currentQuestionSpecificationItem->relationWordItem() != relationWordItem &&
				// Test file: "You had same-similar-related question before (with relation)"
				( tempSpecificationItem = myWordItem_->bestMatchingRelationSpecificationItem( true, true, true, true, true, isNegative, isPossessive, ( questionParameter > NO_QUESTION_PARAMETER ), specificationCollectionNr, relationCollectionNr, currentWordItem, relationWordItem ) ) != NULL &&
				tempSpecificationItem->hasRelationWord() )
					currentQuestionSpecificationItem = tempSpecificationItem;
				}

			if( currentQuestionSpecificationItem != NULL )
				{
				hasFoundQuestion = true;

				if( foundQuestionSpecificationItem == NULL ||
				// Similar or more specific alternative question
				foundQuestionSpecificationItem->isExclusiveSpecification() != currentQuestionSpecificationItem->isExclusiveSpecification() ||

				// Prefer question with user specification word
				( hasRelationWord &&
				( tempSpecificationWordItem = currentQuestionSpecificationItem->specificationWordItem() ) != NULL &&
				tempSpecificationWordItem->isUserSpecificationWord ) )
					{
					foundQuestionSpecificationItem = currentQuestionSpecificationItem;

					hasFoundOlderSpecification = foundQuestionSpecificationItem->isOlderItem();
					hasFoundSpecificationRelationWord = foundQuestionSpecificationItem->hasRelationWord();
					isSameAssignment = ( foundQuestionSpecificationItem->isAssignment() == isAssignment );
					isSameExclusiveSpecification = ( foundQuestionSpecificationItem->isExclusiveSpecification() == isExclusiveSpecification );
					isSameRelation = ( foundQuestionSpecificationItem->relationCollectionNr() == relationCollectionNr &&
										foundQuestionSpecificationItem->relationWordItem() == relationWordItem &&
										( tempSpecificationWordItem = foundQuestionSpecificationItem->specificationWordItem() ) != NULL &&
										tempSpecificationWordItem->isUserSpecificationWord );
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

					if( !hasFoundSpecificationRelationWord ||
					isSameRelation )
						{
						if( !isSelfGenerated &&

						( ( isSameRelation &&
						!isSameAssignment ) ||

						// Found user question with different exclusive specification
						foundQuestionSpecificationItem->isExclusiveSpecification() != isExclusiveSpecification ||
						// Found user question with different assumption level
						foundQuestionSpecificationItem->assumptionLevel() != assumptionLevel ) )
							isSameQuestion = false;
						else
							{
							if( !hasFoundSpecificationRelationWord &&
							!isExclusiveSpecification &&

								( !hasFoundOlderSpecification ||
								hasRelationWord ) )
								isConflictingQuestion = true;
							}
						}
					else
						{
						if( foundQuestionSpecificationItem->relationWordItem() == relationWordItem )
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
					( ( !hasFoundSpecificationRelationWord ||
					!isSameRelation ) &&

					foundQuestionSpecificationItem != NULL ) )
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
					}
				}
			else
				{
				if( hasFoundOlderSpecification &&
				!isOnlyCheckingForConflicts &&
				// Skip displaying announcement if question is in conflict with itself
				globalVariables_->nUserSpecificationWords == 1 &&
				// Write more specific specification
				writeMoreSpecificSpecification( foundQuestionSpecificationItem ) != RESULT_OK )
					return myWordItem_->addUserSpecificationResultErrorInWord( functionNameString, moduleNameString_, "I failed to write a more specific related question" );
				}
			}

		return userSpecificationResult;
		}


	protected:
	// Constructor

	WordSpecification( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		char errorString[ERROR_STRING_LENGTH] = EMPTY_STRING;

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
		hasCurrentlyCorrectedAssumptionByKnowledge_ = false;
		hasCurrentlyMoreSpecificSpecification_ = false;
		isOnlyCheckingForConflicts_ = false;
		isWordTouchedDuringCurrentSentence_ = false;

		compoundSpecificationCollectionNr_ = NO_COLLECTION_NR;
		userSpecificationCollectionNr_ = NO_COLLECTION_NR;

		previousConflictingSpecificationItem_ = NULL;

		spanishPreviousSpecificationWordItem_ = NULL;
		}

	void addMyWordToTouchedDuringCurrentSentenceList()
		{
		WordItem *lastTouchedWordItem;
		WordItem *tempTouchedWordItem;

		if( !isWordTouchedDuringCurrentSentence_ )
			{
			lastTouchedWordItem = globalVariables_->firstTouchedWordItem;

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

	bool hasCurrentlyCorrectedAssumption()
		{
		return ( hasCurrentlyCorrectedAssumptionByKnowledge_ ||
				hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ );
		}

	bool hasCurrentlyCorrectedAssumptionByKnowledge()
		{
		return hasCurrentlyCorrectedAssumptionByKnowledge_;
		}

	bool hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion()
		{
		return hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_;
		}

	bool hasCurrentlyMoreSpecificSpecification()
		{
		return hasCurrentlyMoreSpecificSpecification_;
		}

	unsigned int userSpecificationCollectionNr()
		{
		return userSpecificationCollectionNr_;
		}

	signed char checkForSpecificationConflict( bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isUserSpecificationWordSpanishAmbiguous, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasSpecificationCollection = ( specificationCollectionNr > NO_COLLECTION_NR );
		bool isSpecificationWordSpanishAmbiguous;
		unsigned int compoundSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int nonCompoundSpecificationCollectionNr;
		SpecificationItem *anotherConflictingSpecificationItem;
		SpecificationItem *conflictingSpecificationItem = NULL;
		SpecificationItem *foundSpecificationItem;
		SpecificationItem *negativeConflictingSpecificationItem = NULL;
		SpecificationItem *previousConflictingSpecificationItem = NULL;
		SpecificationItem *relatedSpecificationItem = NULL;
		SpecificationItem *tempConflictingSpecificationItem;
		WordItem *collectionWordItem;
		WordItem *relatedSpecificationWordItem;
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForSpecificationConflict";

		if( specificationWordItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();

		// Typical for Spanish
		if( previousConflictingSpecificationItem_ == NULL ||

		( ( collectionWordItem = specificationWordItem->collectionWordItem( specificationCollectionNr ) ) != NULL &&
		!collectionWordItem->isNounWordSpanishAmbiguous() ) )
			{
			if( ( negativeConflictingSpecificationItem = myWordItem_->firstAssignmentOrSpecificationItem( !isNegative, isPossessive, relationCollectionNr, specificationWordItem, relationWordItem ) ) != NULL &&
			negativeConflictingSpecificationItem->isOlderItem() )
				{
				// Write conflicting negative specification
				if( negativeConflictingSpecificationItem->writeSpecificationConflict( false, true ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write the conflicting negative specification" );

				previousConflictingSpecificationItem = negativeConflictingSpecificationItem;
				}

			// Derive conflict
			if( hasSpecificationCollection )
				{
				if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, NO_CONTEXT_NR, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

				relatedSpecificationItem = relatedResult.relatedSpecificationItem;
				}
			}

		if( relatedSpecificationItem == NULL )
			{
			// Check for past tense
			if( ( foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, false, false, (unsigned short)NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != NULL &&
			foundSpecificationItem->isArchivedAssignment() != isArchivedAssignment )
				conflictingSpecificationItem = foundSpecificationItem;
			else
				{
				if( !isSpecificationWordSpanishAmbiguous &&
				( compoundSpecificationCollectionNr = specificationWordItem->compoundCollectionNr() ) > NO_COLLECTION_NR )
					{
					if( !isNegative &&
					!isUserSpecificationWordSpanishAmbiguous &&
					( foundSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, true, isPossessive, (unsigned short)NO_QUESTION_PARAMETER, specificationCollectionNr, specificationWordItem ) ) != NULL &&
					compoundSpecificationCollectionNr != specificationCollectionNr )
						{
						// First check other conflict
						if( ( anotherConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, (unsigned short)NO_QUESTION_PARAMETER, compoundSpecificationCollectionNr, NULL ) ) != NULL &&
						// Write conflicting specification
						anotherConflictingSpecificationItem->writeSpecificationConflict( false, true ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write another conflicting specification" );
						}
					else
						{
						if( foundSpecificationItem != NULL &&
						foundSpecificationItem->isUserSpecification() )
							conflictingSpecificationItem = foundSpecificationItem;
						else
							{
							// Check for existing compound specification
							// No self-generated question available for this specification
							if( ( nonCompoundSpecificationCollectionNr = nonCompoundCollectionNrInCollectionWords( compoundSpecificationCollectionNr ) ) > NO_COLLECTION_NR &&
							( tempConflictingSpecificationItem = myWordItem_->bestMatchingSpecificationWordSpecificationItem( false, isNegative, isPossessive, (unsigned short)NO_QUESTION_PARAMETER, nonCompoundSpecificationCollectionNr, NULL ) ) != NULL )
								conflictingSpecificationItem = tempConflictingSpecificationItem;
							}
						}
					}
				}
			}
		else
			{
			if( ( conflictingSpecificationItem = ( isNegative ?
														myWordItem_->bestMatchingSpecificationWordSpecificationItem( true, true, false, (unsigned short)NO_QUESTION_PARAMETER, specificationCollectionNr, NULL ) :
														relatedSpecificationItem ) ) != NULL &&
			// No conflict found yet
			previousConflictingSpecificationItem_ == NULL )
				{
				if( !relatedSpecificationItem->hasRelationWord() &&
				conflictingSpecificationItem->isUserSpecification() )
					{
					// Opposite negative of conflicting specification word
					if( ( relatedResult = findRelatedSpecification( true, false, false, false, false, !isNegative, isPossessive, NO_QUESTION_PARAMETER, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, NO_CONTEXT_NR, conflictingSpecificationItem->specificationWordItem(), conflictingSpecificationItem->relationWordItem() ) ).result != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

					if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL &&
					relatedSpecificationItem->isOlderItem() &&
					( relatedSpecificationWordItem = relatedSpecificationItem->specificationWordItem() ) != NULL &&
					relatedSpecificationWordItem->isUserSpecificationWord )
						{
						previousConflictingSpecificationItem = relatedSpecificationItem;

						// Write opposite negative conflicting specification
						if( relatedSpecificationItem->writeSpecificationConflict( false, true ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write an opposite negative conflicting specification" );
						}
					}
				else
					{
					if( isNegative &&
					// Write conflicting negative specification
					relatedSpecificationItem->writeSpecificationConflict( false, true ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a negative conflicting specification" );
					}
				}
			}

		if( conflictingSpecificationItem != NULL )
			{
			if( conflictingSpecificationItem->isOlderItem() )
				{
				if( !isNegative &&
				conflictingSpecificationItem != negativeConflictingSpecificationItem &&
				conflictingSpecificationItem != previousConflictingSpecificationItem_ &&
				// Typical for Spanish: Manual conflict tests
				!conflictingSpecificationItem->isHiddenSpanishSpecification() &&

					( previousConflictingSpecificationItem == NULL ||
					!previousConflictingSpecificationItem->isSpecificationWordSpanishAmbiguous() ||
					globalVariables_->nUserRelationWords > 0 ) )
					{
					// Write conflicting specification
					if( conflictingSpecificationItem->writeSpecificationConflict( false, true ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a conflicting specification" );

					previousConflictingSpecificationItem_ = conflictingSpecificationItem;
					}
				}
			else
				{
				// Sentence can only be in conflict with itself if it has at least 2 specification words
				if( !conflictingSpecificationItem->hasNonExclusiveSpecificationCollection() &&
				conflictingSpecificationItem->isUserSpecification() &&
				// Current sentence in conflict with definition
				writeSpecificationInConflictWithDefinition( false, conflictingSpecificationItem->specificationWordItem(), specificationWordItem ) != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write that the current sentence is in conflict with a definition" );
				}
			}

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

	CreateAndAssignResultType addSpecificationInWord( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nEnteredRelationWords, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		bool hasFoundDifferentRelationWords = false;
		bool hasFoundExclusiveSpecification;
		bool hasRelationWord = ( relationWordItem != NULL );
		bool isConfirmedSpecificationButNoneOfItsRelations = false;
		bool isGeneralizationProperNoun = ( generalizationWordTypeNr == WORD_TYPE_PROPER_NOUN );
		bool isNonExclusiveSpecification = false;
		bool isQuestion = ( questionParameter > NO_QUESTION_PARAMETER );
		bool isRelationWordUserGeneralizationWord;
		bool isSameQuestionFromUser = false;
		bool isSelfGenerated = ( firstJustificationItem != NULL );
		bool isSpecificationFeminineWord;
		bool isSpecificationWordSpanishAmbiguous = false;
		bool isUserSpecificationWord;
		bool isUserRelationWord = myWordItem_->isUserRelationWord;
		unsigned int compoundSpecificationCollectionNr = NO_COLLECTION_NR;
		unsigned int nonCompoundSpecificationCollectionNr;
		unsigned int originalSentenceNr;
		JustificationItem *attachedJustificationItem;
		JustificationItem *foundJustificationItem;
		JustificationItem *tempJustificationItem;
		SpecificationItem *confirmedSpecificationItem = NULL;
		SpecificationItem *createdSpecificationItem = NULL;
		SpecificationItem *existingSpecificationItem;
		SpecificationItem *foundSpecificationItem = NULL;
		SpecificationItem *foundUserSpecificationItem;
		SpecificationItem *obsoleteSpecificationItem = NULL;
		SpecificationItem *relationOppositeSpecificationItem;
		SpecificationItem *tempSpecificationItem;
		WordItem *existingRelationWordItem;
		WordItem *foundRelationWordItem;
		CreateAndAssignResultType createAndAssignResult;
		UserSpecificationResultType userSpecificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSpecificationInWord";

		correctedAssumptionReplacedSpecificationItem_ = NULL;
		replacedAssignmentItem_ = NULL;

		// Check generalization word type
		if( !myWordItem_->hasWordType( true, generalizationWordTypeNr ) )
			return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The given generalization word type number doesn't exist for my word" );

		// Check relation variables:
		// 1) If relation context is defined,
		if( ( hasRelationWord ||
		// 2) or relation word type is defined
		relationWordTypeNr != NO_WORD_TYPE_NR ||
		// 3) or relation word item is defined
		relationWordItem != NULL ) &&

		( relationWordItem == NULL ||
		!relationWordItem->hasWordType( false, relationWordTypeNr ) ) )
			return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The relation variables aren't matching" );

		isRelationWordUserGeneralizationWord = ( relationWordItem == NULL ? false : relationWordItem->isUserGeneralizationWord );

		if( !isWordTouchedDuringCurrentSentence_ )
			addMyWordToTouchedDuringCurrentSentenceList();

		if( specificationWordItem != NULL )
			{
			// Check specification word type
			if( !specificationWordItem->hasWordType( true, specificationWordTypeNr ) &&
			// New created language
			!myWordItem_->isLanguageWord() )
				return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The given specification word doesn't have the given specification word type" );

			isSpecificationWordSpanishAmbiguous = specificationWordItem->isNounWordSpanishAmbiguous();
			isUserSpecificationWord = specificationWordItem->isUserSpecificationWord;

			// Collect definition specification without specification collection
			if( !isExclusiveSpecification &&
			!isQuestion &&
			generalizationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
			!specificationWordItem->hasCollection() &&

				// Collect new singular noun specification words
				( ( specificationWordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
				!specificationWordItem->isOlderItem() ) ||

				// Collect both predefined adjectives 'feminine' and 'masculine'
				specificationWordItem->isAdjectiveFeminineOrMasculine() ) &&

			// Collect simple specifications
			collectSimpleSpecifications( specificationWordTypeNr, generalizationWordTypeNr, specificationWordItem ) != RESULT_OK )
				return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect simple specifications, with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

			if( specificationCollectionNr == NO_COLLECTION_NR )
				{
				if( !isSelfGenerated &&
				userSpecificationCollectionNr_ > NO_COLLECTION_NR )
					// Use previous user specification collection
					specificationCollectionNr = userSpecificationCollectionNr_;
				else
					{
					compoundSpecificationCollectionNr = specificationWordItem->compoundCollectionNr();

					specificationCollectionNr = ( ( ( nonCompoundSpecificationCollectionNr = specificationWordItem->nonCompoundCollectionNr() ) > NO_COLLECTION_NR &&

													( ( compoundSpecificationCollectionNr == NO_COLLECTION_NR &&
													// Skip definition sentence with the same generalization and specification word
													!specificationWordItem->isUserGeneralizationWord ) ||

													( !isQuestion &&
													// Non-exclusive specification collection
													!isExclusiveSpecification &&

														( !isSpecificationWordSpanishAmbiguous ||

														( hasRelationWord &&

															( !isUserSpecificationWord ||
															nEnteredRelationWords > 1 ) ) ) ) ) ) ? nonCompoundSpecificationCollectionNr :
																									compoundSpecificationCollectionNr );

					// User specification / question
					if( !isSelfGenerated &&
					specificationCollectionNr != userSpecificationCollectionNr_ )
						// Remember this user specification collection
						userSpecificationCollectionNr_ = specificationCollectionNr;
					}
				}
			else
				{
				if( !specificationWordItem->hasCollectionNr( specificationCollectionNr ) )
					return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The given specification word doesn't have the given specification collection" );
				}

			// User specification / question
			if( !isSelfGenerated &&
			myWordItem_->isGeneralizationReasoningWordType( generalizationWordTypeNr ) )
				{
				// Check user specification / question
				if( ( userSpecificationResult = checkUserSpecificationOrQuestion( hasRelationWord, isAssignment, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isSpecificationGeneralization, isSpecificationWordSpanishAmbiguous, isValueSpecification, assumptionLevel, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to check a user specification or user question with specification word \"", specificationWordItem->anyWordTypeString(), "\"" );

				isConfirmedSpecificationButNoneOfItsRelations = userSpecificationResult.isConfirmedSpecificationButNoneOfItsRelations;
				isNonExclusiveSpecification = userSpecificationResult.isNonExclusiveSpecification;
				isSameQuestionFromUser = userSpecificationResult.isSameQuestionFromUser;

				// Confirmed specification
				confirmedSpecificationItem = userSpecificationResult.confirmedSpecificationItem;
				}

			// Try to find specification
			// Start with finding user specification
			if( ( foundUserSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, false, ( isGeneralizationProperNoun && isNegative ), isPossessive, false, questionParameter, specificationCollectionNr, ( isAssignment ? NO_COLLECTION_NR : relationCollectionNr ), specificationWordItem, ( isAssignment ? NULL : relationWordItem ) ) ) == NULL )
				{
				// User specification
				if( firstJustificationItem == NULL )
					{
					if( !isAssignment &&
					!isQuestion )
						// Obsolete user specification without specification
						// Test file: "reasoning\family\John - Anna (before family definition)"
						// Spanish test file: "razonamiento\Reto científico"
						obsoleteSpecificationItem = myWordItem_->firstSelfGeneratedCheckSpecificationItem( false, false, isNegative, isPossessive, false, questionParameter, NO_COLLECTION_NR, relationCollectionNr, specificationWordItem, relationWordItem );
					}
				else	// Self-generated
					{
					if( ( foundSpecificationItem = myWordItem_->bestMatchingRelationSpecificationItem( false, false, false, false, isArchivedAssignment, isNegative, isPossessive, isQuestion, specificationCollectionNr, ( isAssignment ? NO_COLLECTION_NR : relationCollectionNr ), specificationWordItem, ( isAssignment ? NULL : relationWordItem ) ) ) == NULL )
						{
						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						// Spanish test files: "razonamiento\familia\Complejo (5)",
						//						"razonamiento\familia\Complejo (6)",
						//						"razonamiento\familia\Complejo (11)",
						//						"razonamiento\familia\Complejo (19 - mezclado)"
						!isNegative &&
						isPossessive &&
						// Try to find alternative
						( tempSpecificationItem = myWordItem_->bestMatchingRelationSpecificationItem( false, false, false, true, specificationCollectionNr, specificationWordItem ) ) != NULL &&
						tempSpecificationItem->isSelfGeneratedConclusion() )
							foundSpecificationItem = tempSpecificationItem;
						}
					else
						{
						if( isExclusiveSpecification &&
						!foundSpecificationItem->isExclusiveSpecification() &&
						myWordItem_->firstExclusiveSpecificationItem( specificationWordItem ) != NULL )
							return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "An exclusive specification was requested. But I selected a non-exclusive specification instead" );
						}

					// Not found specification
					if( foundSpecificationItem == NULL )
						{
						if( !isQuestion &&
						( existingSpecificationItem = myWordItem_->bestMatchingRelationSpecificationItem( isArchivedAssignment, isNegative, isPossessive, specificationWordItem ) ) != NULL )
							{
							if( relationWordItem == NULL )
								{
								if( isNegative )
									// Test file: "reasoning\Scientific challenge"
									foundSpecificationItem = existingSpecificationItem;
								}
							else
								{
								if( !isSpecificationWordSpanishAmbiguous ||
								// Typical for Spanish
								!existingSpecificationItem->isHiddenSpanishSpecification() )
									{
									if( ( existingRelationWordItem = existingSpecificationItem->relationWordItem() ) != NULL &&
									// Typical for Spanish
									isSpecificationWordSpanishAmbiguous )
										{
										// Non-possessive
										if( !isPossessive )
											{
											if( existingSpecificationItem->hasNonCompoundSpecificationCollection() &&
											!existingSpecificationItem->hasOnlyOneRelationWord() )
												// Spanish test files: "razonamiento\familia\Complejo (2)",
												//						"razonamiento\familia\Complejo (12)",
												//						"razonamiento\familia\Complejo (15)"
												foundSpecificationItem = existingSpecificationItem;
											}
										else	// Possessive
											{
											// Efficiency
											if( isArchivedAssignment ||
											// Spanish test files: "conflicto\familia\José es una madre",
											//						"razonamiento\familia\Complejo (9)",
											//						"razonamiento\familia\Complejo (15)",
											//						"razonamiento\familia\Complejo (19 - extraño)",
											//						"razonamiento\familia\Complejo (20)",
											//						among others
											relationCollectionNr > NO_COLLECTION_NR ||

											( existingSpecificationItem->isOlderItem() &&

												// Same relation word
												( ( existingRelationWordItem == relationWordItem &&
												!existingSpecificationItem->hasPrimaryActiveAssignmentJustification() &&
												( foundJustificationItem = existingSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) != NULL &&

													// Spanish test files: "razonamiento\familia\Complejo (16)",
													//						"razonamiento\familia\Juan - Ana (antes de la definición de la familia)",
													//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
													//						"razonamiento\familia\Paz - Juan y Ana - José - Luisa"
													//						"razonamiento\familia\Paz es un hijo - Juan y Ana"
													( foundJustificationItem->isOlderItem() ||

													// Spanish test files: "razonamiento\familia\Complejo (6)",
													//						"razonamiento\familia\Complejo (11)",
													//						"razonamiento\familia\José tiene 2 padres - José es hijo"
													( ( attachedJustificationItem = foundJustificationItem->attachedJustificationItem() ) != NULL &&
													!attachedJustificationItem->isReversibleAssumptionOrConclusion() ) ) ) ||

												// Different relation word
												// Spanish test files: "conflicto\familia\José es una madre",
												//						"razonamiento\familia\Complejo (6)",
												//						"razonamiento\familia\Complejo (9)",
												//						"razonamiento\familia\Complejo (11)",
												//						"razonamiento\familia\Complejo (12)",
												//						"razonamiento\familia\Complejo (13)",
												//						"razonamiento\familia\Complejo (14)",
												//						among others
												( existingRelationWordItem != relationWordItem &&
												!existingSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) )
												foundSpecificationItem = existingSpecificationItem;
											}
										}	
									else
										{
										// Not user specification word
										// Test file: "reasoning\family\John - Anna (before family definition)"
										// Spanish test files: "razonamiento\familia\Complejo (18)",
										//						"razonamiento\familia\Complejo (19 - mezclado)",
										//						"razonamiento\familia\Complejo (19 - extraño)",
										if( ( !isUserSpecificationWord &&
										isRelationWordUserGeneralizationWord &&
										existingRelationWordItem != NULL ) ||

										// User specification word
										// Test file: "reasoning\family\John - Anna (before family definition)"
										( isUserSpecificationWord &&
										existingSpecificationItem->hasRelationCollection() ) )
											foundSpecificationItem = existingSpecificationItem;
										else
											{
											// Test file: "reasoning\family\Complex (9)",
											//				"reasoning\family\Complex (16)",
											//				"reasoning\family\I know (11)",
											//				"reasoning\family\John is a father - Paul has 2 parents",
											//				among others
											if( ( !isSpecificationWordSpanishAmbiguous ||

											// Typical for Spanish
											// Spanish test file: "razonamiento\familia\Sé (11)"
											!isPossessive ||

											// Spanish test file: "razonamiento\familia\Complejo (10)",
											//						"razonamiento\familia\Complejo (19 - mezclado)",
											//						"razonamiento\familia\Complejo (19 - extraño)",
											//						"razonamiento\familia\Mis conclusiones que se confirman (sin relación)",
											//						"razonamiento\familia\Mis suposiciones que se confirman (Paz, José y Luisa)"
											!isUserRelationWord ||

											// Spanish test file: "razonamiento\familia\Sé (11)"
											isUserSpecificationWord ) &&

											myWordItem_->noRelationWordSpecificationItem( isPossessive, true, specificationWordItem ) != NULL )
												{
												if( confirmedSpecificationItem != NULL )
													return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "The confirmed specification item is already defined" );

												// Write an assumption that has relation words now
												if( myWordItem_->writeUpdatedSpecification( isSpecificationWordSpanishAmbiguous, false, false, false, true, false, false, existingSpecificationItem ) != RESULT_OK )
													return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to write an assumption that has relation words now" );

												// Avoid replacing by specification with higher assumption level
												if( existingSpecificationItem->assumptionLevel() >= assumptionLevel )
													confirmedSpecificationItem = existingSpecificationItem;
												}
											}
										}
									}
								}
							}
						}
					else	// Found specification
						{
						if( isPossessive &&
						!isSpecificationWordSpanishAmbiguous &&
						hasCurrentlyConfirmedSpecification_ &&
						foundSpecificationItem->isSelfGeneratedConclusion() &&
						// Current user assignment
						myWordItem_->firstAssignmentItem( false, false, false ) != NULL )
							{
							// Conclusion becomes assumption
							// Test file: "reasoning\family\Joe has 2 parents - Joe is a child",
							//				"reasoning\family\Joe is a child - Joe has 2 parents",
							//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
							if( firstJustificationItem->changeAttachedJustification( foundSpecificationItem->firstJustificationItem() ) != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to change the attached justification item of the given first justification item" );

							obsoleteSpecificationItem = foundSpecificationItem;
							foundSpecificationItem = NULL;
							}
						else
							{
							if( relationWordItem != NULL &&
							foundSpecificationItem->isOlderItem() &&
							( foundRelationWordItem = foundSpecificationItem->relationWordItem() ) != NULL &&
							( foundJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL )
								{
								relationOppositeSpecificationItem = relationWordItem->firstSpecificationItem( isNegative, !isPossessive, isQuestion, specificationWordItem );

								// No relation collection
								if( ( relationCollectionNr == NO_COLLECTION_NR &&
								isRelationWordUserGeneralizationWord &&
								relationOppositeSpecificationItem != NULL &&

									// Same relation word
									// Test files: "reasoning\family\Complex (15)",
									//				"reasoning\family\Complex (16)",
									//				"reasoning\family\I know (3)",
									//				"reasoning\family\I know (10)",
									//				among others
									( ( foundRelationWordItem == relationWordItem &&
									foundSpecificationItem->hasRelationCollection() &&
									relationOppositeSpecificationItem->hasRelationWord() &&
									relationOppositeSpecificationItem->isUserSpecification() ) ||

									// Different relation word
									( foundRelationWordItem != relationWordItem &&

										// Non-possessive
										// Test files: "reasoning\family\Avoid duplicate context",
										//				"reasoning\family\Complex (7 - Becky)",
										//				"reasoning\family\Complex (7 - Claudia)",
										//				"reasoning\family\I know (4)",
										//				"reasoning\family\I know (3)",
										//				"reasoning\family\I know (10)",
										//				among others
										( ( !isPossessive &&
										!isSpecificationWordSpanishAmbiguous &&
										relationOppositeSpecificationItem->isUserSpecification() ) ||

										// Possessive
										// Test file: "Anna was the mother of Paul, Joe and Laura"
										( isPossessive &&
										isArchivedAssignment ) ) ) ) ) ||

								// Relation collection
								// Test file: "reasoning\family\Complex (4)"
								( relationCollectionNr > NO_COLLECTION_NR &&
								isPossessive &&
								foundRelationWordItem != relationWordItem &&
								!isSpecificationWordSpanishAmbiguous &&
								!isRelationWordUserGeneralizationWord &&
								relationOppositeSpecificationItem == NULL &&
								foundJustificationItem->isOlderItem() &&
								foundJustificationItem->isOppositePossessiveSpecificationAssumption() ) )
									hasFoundDifferentRelationWords = true;
								}
							}
						}
					}
				}
			else
				{
				if( isSpecificationGeneralization &&
				// Spanish ambiguous specification collection
				myWordItem_ == specificationWordItem )
					// Spanish test file: "razonamiento\Recopilar más tarde - Cada padre es un padre y cada madre es un padre"
					specificationCollectionNr = foundUserSpecificationItem->generalizationCollectionNr();
				else
					{
					if( !isSelfGenerated ||
					!foundUserSpecificationItem->isExclusiveSpecification() )
						{
						foundSpecificationItem = foundUserSpecificationItem;

						// Typical for Spanish
						if( isSpecificationWordSpanishAmbiguous &&
						hasRelationWord &&
						isQuestion &&
						foundUserSpecificationItem->hasRelationWord() )
							// Don't create question
							// Spanish test file: "respuestas a las preguntas\familia\	Relación pregunta es subconjunto de relación de respuesta"
							foundSpecificationItem = NULL;
						else
							{
							if( confirmedSpecificationItem != NULL &&
							!confirmedSpecificationItem->hasRelationWord() &&
							foundSpecificationItem->hasRelationWord() )
								{
								// Test file: "reasoning\family\Complex (3)"
								// Replace confirmed specification without relation word by found specification
								if( replaceOrDeleteSpecification( confirmedSpecificationItem, foundSpecificationItem ) != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace the confirmed specification without relation word by the found specification" );

								confirmedSpecificationItem = NULL;
								}
							else
								{
								// Typical for Spanish
								if( hasRelationWord &&
								!isPossessive &&
								!isSelfGenerated &&
								specificationCollectionNr > NO_COLLECTION_NR &&
								!foundSpecificationItem->hasSpecificationCollection() )
									{
									// Spanish test file: "razonamiento\familia\Juan - Ana (antes de la definición de la familia)"
									confirmedSpecificationItem = foundUserSpecificationItem;
									spanishPreviousSpecificationWordItem_ = specificationWordItem;
									}
								}
							}
						}
					else
						{
						if( compoundSpecificationCollectionNr_ == NO_COLLECTION_NR )
							{
							if( isExclusiveSpecification )
								{
								if( ( specificationCollectionNr = myWordItem_->highestCollectionNrInCollectionWords() ) >= MAX_COLLECTION_NR )
									return myWordItem_->startCreateAndAssignResultSystemErrorInWord( functionNameString, moduleNameString_, "Collection number overflow" );

								// Test files (incrementing specificationCollectionNr): "reasoning/The only option left - Boat",
								//														"reasoning/The only option left - Option"
								// and Spanish test files
								compoundSpecificationCollectionNr_ = ++specificationCollectionNr;

								// Typical for Spanish
								if( spanishPreviousSpecificationWordItem_ == NULL )
									spanishPreviousSpecificationWordItem_ = specificationWordItem;
								}
							else
								// Typical for Spanish
								foundSpecificationItem = myWordItem_->firstSpecificationItem( true, false, false, specificationWordItem );
							}
						else
							{
							// Typical for Spanish
							if( spanishPreviousSpecificationWordItem_ != NULL &&
							spanishPreviousSpecificationWordItem_ != specificationWordItem &&
							// Avoid looping
							specificationWordItem->collectionNr( myWordItem_ ) != compoundSpecificationCollectionNr_ )
								{
								specificationCollectionNr = compoundSpecificationCollectionNr_;

								// Each collection comes in pairs
								if( spanishPreviousSpecificationWordItem_->createCollectionItem( true, isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, specificationWordItem, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect previous word \"", spanishPreviousSpecificationWordItem_->anyWordTypeString(), "\" with curent word \"", specificationWordItem->anyWordTypeString(), "\"" );

								if( specificationWordItem->createCollectionItem( false, isExclusiveSpecification, specificationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, spanishPreviousSpecificationWordItem_, myWordItem_, myWordItem_ ).result != RESULT_OK )
									return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to collect current word \"", specificationWordItem->anyWordTypeString(), "\" with previous word \"", spanishPreviousSpecificationWordItem_->anyWordTypeString(), "\"" );
								}
							}
						}
					}
				}

			// Add generalization items
			if( hasRelationWord ||
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

		hasFoundExclusiveSpecification = ( foundSpecificationItem == NULL ? false : foundSpecificationItem->isExclusiveSpecification() );

		// No conflict found
		if( !globalVariables_->hasDisplayedWarning &&

			// Not found
			( foundSpecificationItem == NULL ||

			// Accept different relation words (e.g. ambiguous specification)
			// Test files: "reasoning\family\Anna was the mother of Paul, Joe and Laura",
			//				"reasoning\family\Avoid duplicate context",
			//				"reasoning\family\Complex (4)",
			//				among others
			hasFoundDifferentRelationWords ||

			// Test files: "conflict\family\Question in conflict with itself (with knowledge)",
			//				"reasoning\family\This information is more specific (non-exclusive)"
			isNonExclusiveSpecification ||

			// Test files: "reasoning\Knowledge of organisms (2)",
			//				"reasoning\family\Complex (4)",
			//				"reasoning\family\Complex (5)",
			//				"reasoning\family\Complex (6)",
			//				"reasoning\family\Complex (9)",
			//				"reasoning\family\Complex (18)",
			//				"reasoning\family\I know (10)",
			//				"reasoning\family\I know (11)",
			//				"reasoning\family\My assumptions that are confirmed (Paul and John)"
			confirmedSpecificationItem != NULL ||

			// Overwrite non-exclusive specification by exclusive one
			// Test files: "ambiguity\Boston",
			//				"ambiguity\Duplicates",
			//				"question answering\family\Same and similar questions",
			//				"question answering\family\You had same-similar question before (without relation)",
			//				"reasoning\John is a parent of Paul - John is a parent of Joe",
			//				"reasoning\Knowledge of organisms (1)",
			//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
			( !hasFoundExclusiveSpecification &&
			isExclusiveSpecification ) ||

			// Overwrite user questions by non-exclusive one
			// Test files: "question answering\This question is more specific (without relation)",
			//				"question answering\family\Same and similar questions",
			//				"question answering\family\This question is more specific (with relation)",
			//				"question answering\family\You had same-similar question before (without relation)"
			( hasFoundExclusiveSpecification &&
			!isAssignment &&
			isQuestion &&
			!isSelfGenerated &&
			!isSameQuestionFromUser ) ||

			// Overwrite non-conditional specification by conditional one
			// Test file: "programming\Connect-Four - Display information about the set"
			( isConditional &&
			!foundSpecificationItem->isConditional() ) ||

			( !isExclusiveSpecification &&
			relationCollectionNr > NO_COLLECTION_NR &&
			relationWordItem != NULL &&
			( foundRelationWordItem = foundSpecificationItem->relationWordItem() ) != NULL &&
			foundRelationWordItem != relationWordItem &&

				// User specification
				( ( foundSpecificationItem->isUserSpecification() &&

					// Test file: "reasoning\John is a parent of Paul - Joe has 2 parents"
					( isSelfGenerated ||

					// Test file: "reasoning\Paul has a parent, called John - Paul has a parent, called Anna"
					( isPossessive &&
					myWordItem_->bestMatchingRelationSpecificationItem( false, false, isNegative, isPossessive, specificationWordItem, relationWordItem ) == NULL ) ) ) ||

				// Self-generated specification
				( firstJustificationItem != NULL &&
				!firstJustificationItem->isOlderItem() &&
				foundSpecificationItem->isOlderItem() &&
				( foundJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL &&

					// Non-possessive
					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (3)",
					//				"reasoning\family\Complex (4)",
					//				"reasoning\family\Complex (5)",
					//				"reasoning\family\Complex (6)",
					//				among others
					( ( !isPossessive &&
					!isSpecificationWordSpanishAmbiguous &&
					myWordItem_->bestMatchingRelationSpecificationItem( false, false, isNegative, isPossessive, specificationWordItem, relationWordItem ) == NULL ) ||

					// Possessive
					( isPossessive &&
					// Relation opposite specification
					( relationOppositeSpecificationItem = relationWordItem->firstSpecificationItem( isNegative, false, isQuestion, specificationWordItem ) ) != NULL &&

						( ( !isSpecificationWordSpanishAmbiguous &&
						firstJustificationItem->isReversibleAssumptionOrConclusion() &&
						relationOppositeSpecificationItem->isSelfGeneratedSpecification() &&

							// Relation opposite specification is not older
							( ( !relationOppositeSpecificationItem->isOlderItem() &&

								// Relation word is not user generalization word
								// Test files: "reasoning\family\Complex (20)",
								//				"reasoning\family\John - Laura - John and Anna",
								//				"reasoning\family\John is a parent - Paul - Laura",
								//				"reasoning\family\John is the father - Paul - Laura"
								( ( !isRelationWordUserGeneralizationWord &&
								!foundSpecificationItem->hasRelationCollection() ) ||

								// Relation word is user generalization word
								( isRelationWordUserGeneralizationWord &&
								foundJustificationItem->isOlderItem() &&

									// Found justification is not current
									// Test files: "reasoning\family\Complex (11)",
									//				"reasoning\family\Laura - Paul - John and Anna",
									//				"reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul",
									//				"reasoning\family\Paul - Joe - John and Anna"
									( ( !foundJustificationItem->hasCurrentCreationSentenceNr() &&
									foundSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ||

									// Found justification is current
									( foundJustificationItem->hasCurrentCreationSentenceNr() &&

										// Test files: "reasoning\family\Complex (7 - Becky)",
										//				"reasoning\family\Complex (7 - Claudia)",
										//				"reasoning\family\Complex (18)",
										//				"reasoning\family\Complex (19 - mixed)",
										//				"reasoning\family\Complex (19 - strange)",
										//				"reasoning\family\Simple family definition - Complex (7 - Claudia)"
										( ( foundJustificationItem->isOppositePossessiveSpecificationAssumption() &&
										foundRelationWordItem->firstAdjectiveSpecificationItem( false, false ) != NULL ) ||

										// Reversible assumption or conclusion
										// Test file: "reasoning\family\Complex (2)"
										( foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) == NULL &&
										relationWordItem->firstAdjectiveSpecificationItem( false, false ) == NULL &&
										( tempJustificationItem = foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ) ) != NULL &&
										!tempJustificationItem->hasCurrentCreationSentenceNr() ) ) ) ) ) ) ) ||

							// Relation opposite specification is older
							// Test files: "reasoning\family\Complex (4)",
							//				"reasoning\family\Complex (5)",
							//				"reasoning\family\Complex (6)",
							//				"reasoning\family\John - Laura - John and Anna"
							( relationOppositeSpecificationItem->isOlderItem() &&
							foundSpecificationItem->hasOnlyOneRelationWord() &&
							foundSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) ||

						// Typical for Spanish
						( isSpecificationWordSpanishAmbiguous &&

							// Not hidden Spanish specification
							( ( !foundSpecificationItem->isHiddenSpanishSpecification() &&

								// Relation opposite self-generated specification
								// Spanish test files: "razonamiento\familia\Complejo (6)",
								//						"razonamiento\familia\Complejo (11)",
								//						"razonamiento\familia\Complejo (20)"
								( ( relationOppositeSpecificationItem->isSelfGeneratedSpecification() &&
								!foundSpecificationItem->hasRelationCollection() ) ||

								// Relation opposite user specification
								( relationOppositeSpecificationItem->isUserSpecification() &&
								// Spanish test files: "razonamiento\familia\Sé (3)",
								//						"razonamiento\familia\Mis suposiciones que se confirman (Paz, José y Luisa)",
								//						"razonamiento\familia\Paz es el hijo Juan y Ana"
								foundSpecificationItem->hasCompoundSpecificationCollection() &&
								!foundSpecificationItem->hasRelationCollection() ) ) ||

							// Hidden Spanish specification
							( foundSpecificationItem->isHiddenSpanishSpecification() &&
							// Opposite possessive specification assumption
							firstJustificationItem->hasSecondaryUserSpecification() &&

								// Spanish test file: "razonamiento\familia\Paz - Ana - José - Luisa - Juan"
								( !relationOppositeSpecificationItem->isHiddenSpanishSpecification() ||

								// Spanish test files: "razonamiento\familia\Complejo (14)",
								//						"razonamiento\familia\Sé (11)",
								//						"razonamiento\familia\Paz - Ana - José - Luisa - José - Juan - Paz"
								( !isRelationWordUserGeneralizationWord &&
								!foundSpecificationItem->hasCurrentCreationSentenceNr() &&
								foundSpecificationItem->firstJustificationItem( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) != NULL &&
								!relationOppositeSpecificationItem->hasOnlyOneJustificationType( JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION ) ) ) ) ) ) ) ) ) ) ) ) ) ) )
			{
			if( !isQuestion &&
			!isSelfGenerated &&
			foundSpecificationItem != NULL )
				{
				if( hasFoundExclusiveSpecification )
					// Test files: "programming\Connect-Four - Column is full",
					//				"programming\Connect-Four - Display information about the set",
					//				"programming\Connect-Four - Eager to win",
					//				"programming\Tower of Hanoi (even number of discs)",
					//				"programming\Tower of Hanoi (odd number of discs)"
					isExclusiveSpecification = true;

				if( foundSpecificationItem != confirmedSpecificationItem )
					// Test files: "ambiguity\Boston",
					//				"ambiguity\Duplicates",
					//				"conflict\family\Question in conflict with itself (with knowledge)",
					//				"programming\Connect-Four - Display information about the set",
					//				"reasoning\John is a parent of Paul - John is a parent of Joe",
					//				"reasoning\Paul has a parent, called John - Paul has a parent, called Anna",
					//				"reasoning\family\Complex (15)",
					//				among others
					obsoleteSpecificationItem = foundSpecificationItem;
				}

			if( isExclusiveSpecification &&
			isNonExclusiveSpecification )
				// Test files: "conflict\family\Question in conflict with itself (with knowledge)",
				//				"reasoning\family\This information is more specific (non-exclusive)"
				isExclusiveSpecification = false;

			originalSentenceNr = ( hasRelationWord ||
									isQuestion ||
									foundSpecificationItem == NULL ? globalVariables_->currentSentenceNr :
																	foundSpecificationItem->originalSentenceNr() );

			// Create the actual specification
			if( ( createAndAssignResult = myWordItem_->createSpecificationItem( false, false, false, false, isCharacteristicFor, isConditional, ( hasCurrentlyCorrectedAssumptionByKnowledge_ || hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion_ ), isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, ( isAssignment && !isSelfGenerated ? false : isNegative ), isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, NO_ASSIGNMENT_LEVEL, assumptionLevel, globalVariables_->currentLanguageNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, ( isAssignment ? NO_WORD_TYPE_NR : relationWordTypeNr ), specificationCollectionNr, ( isAssignment ? NO_COLLECTION_NR : relationCollectionNr ), generalizationContextNr, NO_CONTEXT_NR, ( isAssignment ? NO_CONTEXT_NR : relationContextNr ), originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, ( isAssignment ? 0 : nEnteredRelationWords ), firstJustificationItem, specificationWordItem, ( isAssignment ? NULL : relationWordItem ), specificationString, NULL, NULL ) ).result != RESULT_OK )
				return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to create a specification item" );

			if( ( createdSpecificationItem = createAndAssignResult.createdSpecificationItem ) == NULL )
				return myWordItem_->startCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I couldn't create a specification item" );

			// Test files: "reasoning\family\Anna - John - Paul, Joe and Laura",
			//				"reasoning\family\Complex (1)",
			//				"reasoning\family\Complex (9)",
			//				"reasoning\family\Complex (10)",
			//				"reasoning\family\Complex (16)",
			//				among others
			if( ( isConfirmedSpecificationButNoneOfItsRelations ||

			// Test files: "ambiguity\Boston",
			//				"ambiguity\Duplicates",
			//				"conflict\family\Question in conflict with itself (with knowledge)",
			//				"programming\Connect-Four - Display information about the set",
			//				"reasoning\John is a parent of Paul - John is a parent of Joe",
			//				"reasoning\Paul has a parent, called John - Paul has a parent, called Anna",
			//				"reasoning\family\Complex (15)",
			//				among others
			obsoleteSpecificationItem != NULL ||

			// Test files: "reasoning\family\Correcting invalidated assumption (by knowledge)",
			//				"reasoning\family\Correcting invalidated assumption (by opposite suggestive question)"
			correctedAssumptionReplacedSpecificationItem_ != NULL ||

			( confirmedSpecificationItem != NULL &&

				// Efficiency
				( !isSpecificationWordSpanishAmbiguous ||

				// Typical for Spanish
				// Spanish test files: "conflicto\familia\José es una madre",
				//						"respuestas a las preguntas\familia\Contestador inteligente de preguntas",
				//						"respuestas a las preguntas\familia\Sólo unas pocas preguntas (1)",
				//						"respuestas a las preguntas\familia\Tenías mismo-similar-relacionado pregunta antes (con relación)",
				//						"razonamiento\Reto científico",
				//						"razonamiento\familia\Complejo (1)",
				//						"razonamiento\familia\Complejo (2)",
				//						among others
				!createdSpecificationItem->isHiddenSpanishSpecification() ) ) ) &&

			// Now replace obsolete specifications by the created specification
			replaceObsoleteSpecification( isConfirmedSpecificationButNoneOfItsRelations, confirmedSpecificationItem, createdSpecificationItem, obsoleteSpecificationItem ) != RESULT_OK )
				return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace an obsolete specification" );

			if( isGeneralizationProperNoun &&
			firstJustificationItem != NULL &&
			foundSpecificationItem != NULL &&

				// Non-possessive
				// Test file: "reasoning\family\Paul - Anna - Joe - Laura - Joe - John - Paul"
				( ( !isPossessive &&
				foundSpecificationItem->isSelfGeneratedAssumption() &&
				firstJustificationItem->isSpecificationSubstitutionAssumptionOrConclusion() ) ||

				// Possessive
				( isPossessive &&

					// Test files: "reasoning\family\Complex (2)",
					//				"reasoning\family\Complex (4)",
					//				"reasoning\family\Complex (7 - Becky)",
					//				"reasoning\family\Complex (7 - Claudia)",
					//				among others
					( ( ( foundJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL &&
					foundJustificationItem->isOlderItem() ) ||

					// Typical for Spanish
					// Spanish test files: "razonamiento\familia\Complejo (6)",
					//						"razonamiento\familia\Complejo (11)",
					//						"razonamiento\familia\Juan - Paz - Luisa",
					( isSpecificationWordSpanishAmbiguous &&
					foundSpecificationItem->isSelfGeneratedConclusion() ) ) ) ) )
				{
				if( firstJustificationItem != foundSpecificationItem->firstJustificationItem() )
					{
					if( isPossessive &&
					foundSpecificationItem->hasCurrentCreationSentenceNr() )
						{
						// Test files: "reasoning\family\Complex (2)",
						//				"reasoning\family\Complex (7 - Becky)",
						//				"reasoning\family\Complex (7 - Claudia)",
						//				"reasoning\family\Complex (11)",
						//				"reasoning\family\Complex (18)",
						//				"reasoning\family\Complex (19 - mixed)",
						//				"reasoning\family\Complex (19 - strange)",
						//				among others
						if( createdSpecificationItem->changeFirstJustification( true, foundSpecificationItem->firstJustificationItem() ) != RESULT_OK )
							return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace the justifications of the created specification by the justifications of the found specification" );
						}
					else
						{
						// Change attached justification
						// Test files: "reasoning\family\Complex (4)",
						//				"reasoning\family\Complex (5)",
						//				"reasoning\family\Complex (6)",
						//				"reasoning\family\John - Laura - John and Anna",
						//				"reasoning\family\John is a parent - Paul - Laura",
						//				"reasoning\family\John is the father - Paul - Laura",
						if( firstJustificationItem->changeAttachedJustification( foundSpecificationItem->firstJustificationItem() ) != RESULT_OK )
							return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to attach the justifications of the found specification to the created specification" );
						}
					}

				// Replace or delete found specification with copied relation context
				// Test files: "reasoning\family\Complex (2)",
				//				"reasoning\family\Complex (4)",
				//				"reasoning\family\Complex (5)",
				//				"reasoning\family\Complex (6)",
				//				among others
				if( replaceOrDeleteSpecification( foundSpecificationItem, createdSpecificationItem ) != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to replace or delete the found specification with copied relation context" );
				}

			if( !isNegative &&
			!isPossessive &&
			!isQuestion &&
			specificationWordItem != NULL &&

				( !isSpecificationWordSpanishAmbiguous ||
				// Typical for Spanish
				!createdSpecificationItem->isHiddenSpanishSpecification() ) )
				{
				if( !isAssignment &&
				// Find answers to questions
				// Check user assignments after the assignment is created. Class: AdminSpecification, function: addSpecification()
				myWordItem_->findAnswersToQuestions( specificationWordItem->compoundCollectionNr(), createdSpecificationItem ) != RESULT_OK )
					return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to find answers to questions" );

				if( !isSelfGenerated &&
				!myWordItem_->isFeminineOrMasculineWord() &&
				specificationWordItem->isFeminineOrMasculineWord() )
					{
					isSpecificationFeminineWord = specificationWordItem->isFeminineWord();

					// Definition specification
					if( !isExclusiveSpecification ||
					hasOnlyEitherFeminineOrMasculineSpecificationWords( isSpecificationFeminineWord ) )
						{
						if( isSpecificationFeminineWord )
							{
							if( myWordItem_->markWordAsFeminine() != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as feminine" );
							}
						else
							{
							if( myWordItem_->markWordAsMasculine() != RESULT_OK )
								return myWordItem_->addCreateAndAssignResultErrorInWord( functionNameString, moduleNameString_, "I failed to mark my word as masculine" );
							}
						}
					}
				}
			}

		if( ( createAndAssignResult.createdOrFoundSpecificationItem = createdSpecificationItem ) == NULL )
			createAndAssignResult.createdOrFoundSpecificationItem = foundSpecificationItem;

		createAndAssignResult.foundSpecificationItem = foundSpecificationItem;
		createAndAssignResult.replacedAssignmentItem = replacedAssignmentItem_;
		return createAndAssignResult;
		}

	RelatedResultType findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( searchSpecificationItem == NULL )
			return myWordItem_->startRelatedResultErrorInWord( functionNameString, moduleNameString_, "The given search specification item is undefined" );

		return findRelatedSpecification( isCheckingRelationContext, false, false, searchSpecificationItem->isAssignment(), searchSpecificationItem->isInactiveAssignment(), searchSpecificationItem->isArchivedAssignment(), searchSpecificationItem->isExclusiveSpecification(), searchSpecificationItem->isNegative(), searchSpecificationItem->isPossessive(), searchSpecificationItem->questionParameter(), searchSpecificationItem->specificationWordTypeNr(), searchSpecificationItem->specificationCollectionNr(), searchSpecificationItem->relationCollectionNr(), searchSpecificationItem->generalizationContextNr(), searchSpecificationItem->specificationWordItem(), searchSpecificationItem->relationWordItem() );
		}

	RelatedResultType findRelatedSpecification( bool isIgnoringExclusive, bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationCollectionNr, unsigned int generalizationContextNr, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findRelatedSpecification";

		if( isIncludingAssignments )
			{
			if( ( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related assignment" );

			if( isIncludingArchivedAssignments &&
			relatedResult.relatedSpecificationItem == NULL &&
			// Archived assignments
			( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, true, false, true, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
				return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification as well" );
			}

		// Specifications
		if( relatedResult.relatedSpecificationItem == NULL &&
		( relatedResult = findRelatedSpecification( false, isIgnoringExclusive, isIgnoringNegative, false, false, false, isExclusiveSpecification, isNegative, isPossessive, questionParameter, specificationWordTypeNr, specificationCollectionNr, relationCollectionNr, generalizationContextNr, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
			return myWordItem_->addRelatedResultErrorInWord( functionNameString, moduleNameString_, "I failed to find a related specification" );

		return relatedResult;
		}
	};

/*************************************************************************
 *	"Yes, joyful are those who live like this!
 *	Joyful indeed are those whose God is the Lord." (Psalm 144:15)
 *************************************************************************/
