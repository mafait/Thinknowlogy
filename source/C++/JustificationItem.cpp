/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		Storing info need to write the justification reports
 *					for the self-generated knowledge
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

#include "SpecificationItem.cpp"

	// Private functions

	bool JustificationItem::hasJustification( JustificationItem *existingJustificationItem )
		{
		JustificationItem *searchJustificationItem = this;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem == existingJustificationItem )
				return true;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem_;
			}

		return false;
		}

	bool JustificationItem::hasSimilarRelationCollection( unsigned int firstCollectionNr, unsigned int secondCollectionNr, WordItem *firstRelationWordItem, WordItem *secondRelationWordItem )
		{
		WordItem *currentCollectionWordItem;

		if( firstRelationWordItem != NULL &&
		secondRelationWordItem != NULL &&

		( firstCollectionNr != secondCollectionNr ||
		firstRelationWordItem != secondRelationWordItem ) &&

		( currentCollectionWordItem = globalVariables()->firstCollectionWordItem ) != NULL )
			{
			// Do for all collection words
			do	{
				if( !currentCollectionWordItem->hasSimilarRelationCollection( firstCollectionNr, secondCollectionNr ) )
					return false;
				}
			while( ( currentCollectionWordItem = currentCollectionWordItem->nextCollectionWordItem ) != NULL );
			}

		return true;
		}

	bool JustificationItem::isSameJustificationType( JustificationItem *referenceJustificationItem )
		{
		return ( referenceJustificationItem != NULL &&
				referenceJustificationItem->justificationOrderNr == justificationOrderNr &&
				referenceJustificationItem->justificationTypeNr() == justificationTypeNr_ );
		}


	// Constructor

	JustificationItem::JustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, unsigned short _justificationOrderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, JustificationItem *attachedJustificationItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "JustificationItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		hasFeminineOrMasculineProperNounEnding_ = hasFeminineOrMasculineProperNounEnding;

		justificationTypeNr_ = justificationTypeNr;

		primarySpecificationItem_ = primarySpecificationItem;
		additionalDefinitionSpecificationItem_ = additionalDefinitionSpecificationItem;
		secondarySpecificationItem_ = secondarySpecificationItem;
		additionalProperNounSpecificationItem_ = additionalProperNounSpecificationItem;

		attachedJustificationItem_ = attachedJustificationItem;

		// Protected constructed variables

		justificationOrderNr = _justificationOrderNr;
		}


	// Protected virtual functions

	void JustificationItem::checkForUsage()
		{
		myWordItem()->checkJustificationForUsageInWord( this );
		}

	void JustificationItem::selectingJustificationSpecifications()
		{
		if( primarySpecificationItem_ != NULL )
			primarySpecificationItem_->isSelectedByJustificationQuery = true;

		if( additionalDefinitionSpecificationItem_ != NULL )
			additionalDefinitionSpecificationItem_->isSelectedByJustificationQuery = true;

		if( secondarySpecificationItem_ != NULL )
			secondarySpecificationItem_->isSelectedByJustificationQuery = true;

		if( additionalProperNounSpecificationItem_ != NULL )
			additionalProperNounSpecificationItem_->isSelectedByJustificationQuery = true;
		}

	bool JustificationItem::hasWordType( unsigned short queryWordTypeNr )
		{
		return ( justificationTypeNr_ == queryWordTypeNr );
		}

	bool JustificationItem::hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		return ( ( primarySpecificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : primarySpecificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : primarySpecificationItem_->itemNr() == queryItemNr ) ) ||

				( additionalDefinitionSpecificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : additionalDefinitionSpecificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : additionalDefinitionSpecificationItem_->itemNr() == queryItemNr ) ) ||

				( secondarySpecificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : secondarySpecificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : secondarySpecificationItem_->itemNr() == queryItemNr ) ) ||

				( additionalProperNounSpecificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : additionalProperNounSpecificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : additionalProperNounSpecificationItem_->itemNr() == queryItemNr ) ) ||

				( attachedJustificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : attachedJustificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : attachedJustificationItem_->itemNr() == queryItemNr ) ) );
		}

	char *JustificationItem::itemToString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( hasFeminineOrMasculineProperNounEnding_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "hasFeminineOrMasculineProperNounEnding" );
			}

		switch( justificationTypeNr_ )
			{
			case JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isGeneralizationAssumption" );
				break;

			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isOppositePossessiveSpecificationAssumption" );
				break;

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isExclusiveSpecificationSubstitutionAssumption" );
				break;

			case JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSuggestiveQuestionAssumption" );
				break;

			case JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isOnlyOptionLeftAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isReversibleAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isDefinitionPartOfAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isNegativeAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationGeneralizationAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationSubstitutionAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationSubstitutionPartOfAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isUniqueUserRelationAssumptionOrConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationSubstitutionQuestion" );
				break;

			default:
				sprintf( tempString, "%cjustificationType:%u", QUERY_SEPARATOR_CHAR, justificationTypeNr_ );
				strcat( queryString, tempString );
			}

		sprintf( tempString, "%c%u", QUERY_WORD_TYPE_CHAR, justificationTypeNr_ );
		strcat( queryString, tempString );

		sprintf( tempString, "%cjustificationOrderNr:%u", QUERY_SEPARATOR_CHAR, justificationOrderNr );
		strcat( queryString, tempString );

		if( primarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%cprimarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, primarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, primarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( additionalDefinitionSpecificationItem_ != NULL )
			{
			sprintf( tempString, "%cadditionalDefinitionSpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, additionalDefinitionSpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, additionalDefinitionSpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( secondarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%csecondarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, secondarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, secondarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( additionalProperNounSpecificationItem_ != NULL )
			{
			sprintf( tempString, "%cadditionalProperNounSpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, additionalProperNounSpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, additionalProperNounSpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( attachedJustificationItem_ != NULL )
			{
			sprintf( tempString, "%cattachedJustificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, attachedJustificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, attachedJustificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected functions

	void JustificationItem::clearReplacingInfo()
		{
		if( hasCurrentReplacedSentenceNr() )
			clearReplacedSentenceNr();
		}

	bool JustificationItem::hasAdditionalDefinitionSpecification()
		{
		return ( additionalDefinitionSpecificationItem_ != NULL );
		}

	bool JustificationItem::hasAssumptionLevel()
		{
		return ( justificationAssumptionLevel() > NO_ASSUMPTION_LEVEL );
		}

	bool JustificationItem::hasFeminineOrMasculineProperNounEnding()
		{
		return hasFeminineOrMasculineProperNounEnding_;
		}

	bool JustificationItem::hasJustification( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		return ( justificationTypeNr_ == justificationTypeNr &&
				hasFeminineOrMasculineProperNounEnding_ == hasFeminineOrMasculineProperNounEnding &&
				primarySpecificationItem_ == primarySpecificationItem &&
				additionalDefinitionSpecificationItem_ == additionalDefinitionSpecificationItem &&
				secondarySpecificationItem_ == secondarySpecificationItem );
//				additionalProperNounSpecificationItem_ == additionalProperNounSpecificationItem );
		}

	bool JustificationItem::hasPrimarySpecificationRelationCollection()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->hasRelationCollection() );
		}

	bool JustificationItem::hasPrimarySpecificationRelationWord()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->hasRelationWord() );
		}

	bool JustificationItem::hasPrimaryUserSpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isUserSpecification() );
		}

	bool JustificationItem::hasSecondaryCompoundCollectionSpecification()
		{
		return ( secondarySpecificationItem_ != NULL &&
				secondarySpecificationItem_->hasCompoundSpecificationCollection() );
		}

	bool JustificationItem::hasSecondaryUserSpecification()
		{
		return ( secondarySpecificationItem_ != NULL &&
				secondarySpecificationItem_->isUserSpecification() );
		}

	bool JustificationItem::isExclusivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isExclusiveSpecification() );
		}

	bool JustificationItem::isOlderPrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isOlderItem() );
		}

	bool JustificationItem::isPossessivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isPossessive() );
		}

	bool JustificationItem::isPossessiveSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != NULL &&
				secondarySpecificationItem_->isPossessive() );
		}

	bool JustificationItem::isPrimaryActiveAssignmentSpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isActiveAssignment() );
		}

	bool JustificationItem::isPrimaryNounSpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isNounSpecification() );
		}

	bool JustificationItem::isPrimaryQuestionSpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isQuestion() );
		}

	bool JustificationItem::isPrimarySpecificationWordSpanishAmbiguous()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isSpecificationWordSpanishAmbiguous() );
		}

	bool JustificationItem::isReplacedPrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isReplacedItem() );
		}

	bool JustificationItem::isReplacedSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != NULL &&
				secondarySpecificationItem_->isReplacedItem() );
		}

	bool JustificationItem::isExclusiveSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	bool JustificationItem::isNegativeAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION );
		}

	bool JustificationItem::isObsoleteAssumptionJustification( bool isIncludingOppositePossessiveSpecificationAssumption, bool isIncludingReversibleAssumptionOrConclusion, bool isIncludingSpecificationSubstitutionAssumptionOrConclusion, bool isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion, bool isOnlySelectingOlderJustifications, unsigned short assumptionLevel )
		{
		return 	( primarySpecificationItem_ != NULL &&

				( !isOnlySelectingOlderJustifications ||
				isOlderItem() ) &&

					( justificationAssumptionLevel() > assumptionLevel &&

					( ( justificationTypeNr_ == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION &&
					isIncludingOppositePossessiveSpecificationAssumption ) ||

					justificationTypeNr_ == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||

					( justificationTypeNr_ == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION &&
					isIncludingReversibleAssumptionOrConclusion ) ||

					( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION &&
					isIncludingSpecificationSubstitutionAssumptionOrConclusion ) ||

					( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION &&
					isIncludingSpecificationSubstitutionPartOfAssumptionOrConclusion ) ) ) );
		}

	bool JustificationItem::isOppositePossessiveSpecificationAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION );
		}

	bool JustificationItem::isQuestionJustification()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION );
		}

	bool JustificationItem::isReversibleAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION );
		}

	bool JustificationItem::isSpecificationSubstitutionAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION );
		}

	bool JustificationItem::isSpecificationSubstitutionPartOfAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION );
		}

	bool JustificationItem::isSuggestiveQuestionAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION );
		}

	bool JustificationItem::isSecondaryAssignmentSpecification()
		{
		return ( secondarySpecificationItem_ != NULL &&
				secondarySpecificationItem_->isAssignment() );
		}

	unsigned short JustificationItem::justificationAssumptionLevel()
		{
		return Item::justificationAssumptionLevel( hasFeminineOrMasculineProperNounEnding_, justificationTypeNr_, primarySpecificationItem_, additionalDefinitionSpecificationItem_, secondarySpecificationItem_, additionalProperNounSpecificationItem_ );
		}

	unsigned short JustificationItem::justificationTypeNr()
		{
		return justificationTypeNr_;
		}

	unsigned short JustificationItem::primarySpecificationWordTypeNr()
		{
		return ( primarySpecificationItem_ == NULL ? NO_WORD_TYPE_NR : primarySpecificationItem_->specificationWordTypeNr() );
		}

	unsigned int JustificationItem::nJustificationRelationWords( unsigned int relationCollectionNr, unsigned int nRelationWords, WordItem *relationWordItem )
		{
		WordItem *justificationRelationWordItem;

		if( relationWordItem != NULL )
			{
			// Only one relation word
			if( relationCollectionNr == NO_COLLECTION_NR )
				return 1;

			if( primarySpecificationItem_ != NULL &&
			( justificationRelationWordItem = primarySpecificationItem_->relationWordItem() ) != NULL )

				return ( isReversibleAssumptionOrConclusion() ? 1 : myWordItem()->nRelationWords( primarySpecificationItem_->relationCollectionNr(), justificationRelationWordItem ) );

			if( secondarySpecificationItem_ != NULL &&
			( justificationRelationWordItem = secondarySpecificationItem_->relationWordItem() ) != NULL )
				{
				if( additionalDefinitionSpecificationItem_ != NULL )
					return myWordItem()->nRelationWords( secondarySpecificationItem_->relationCollectionNr(), justificationRelationWordItem );

				return ( hasSimilarRelationCollection( secondarySpecificationItem_->relationCollectionNr(), relationCollectionNr, justificationRelationWordItem, relationWordItem ) ? nRelationWords : 1 );
				}
			}

		return 0;
		}

	unsigned int JustificationItem::primarySpecificationCollectionNr()
		{
		return ( primarySpecificationItem_ == NULL ? NO_COLLECTION_NR : primarySpecificationItem_->specificationCollectionNr() );
		}

	signed char JustificationItem::attachJustification( JustificationItem *attachedJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "attachJustification";

		if( attachedJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given attached justification item is undefined" );

		if( attachedJustificationItem == this )
			return startError( functionNameString, NULL, "The given attached justification item is the same justification item as me" );

		if( involvedSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given involved specification item is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( !attachedJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given attached justification item isn't active" );

		if( attachedJustificationItem_ != NULL )
			return startError( functionNameString, NULL, "I already have an attached justification item" );

		if( involvedSpecificationItem->checkJustification( attachedJustificationItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to check the given attached justification item against the given involved specification item" );

		// Add attached justification item
		attachedJustificationItem_ = attachedJustificationItem;

		return RESULT_OK;
		}

	signed char JustificationItem::changeAttachedJustification( JustificationItem *newAttachedJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changeAttachedJustification";

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		attachedJustificationItem_ = NULL;

		if( newAttachedJustificationItem == this )
			return startError( functionNameString, NULL, "The given new attached justification item is the same as me" );

		if( newAttachedJustificationItem != NULL )
			{
			if( !newAttachedJustificationItem->isActiveItem() )
				return startError( functionNameString, NULL, "The given new attached justification item isn't active" );

			if( newAttachedJustificationItem->hasJustification( this ) )
				return startError( functionNameString, NULL, "I am already linked to the given new attached justification item" );
			}

		attachedJustificationItem_ = newAttachedJustificationItem;
		return RESULT_OK;
		}

	signed char JustificationItem::checkSpecifications( bool isIncludingReplacedSpecifications )
		{
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkSpecifications";
		WordItem *_myWordItem = myWordItem();

		if( primarySpecificationItem_ != NULL )
			{
			if( primarySpecificationItem_->isReplacedItem() )
				{
				if( isIncludingReplacedSpecifications &&
				_myWordItem->replaceOrDeleteJustification( this ) != RESULT_OK )
					return startError( functionNameString, NULL, "I failed to replace or delete an unreferenced justification item with a replaced primary specification" );
				}
			else
				{
				if( primarySpecificationItem_->isDeletedItem() )
					{
					// The result of this call is stored in a shared string. Hence, one call at the time.
					strcpy( errorString, "\nI found a deleted primary specification item:\n\tSpecificationItem: " );
					strcat( errorString, primarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ";\n\tJustificationItem: " );
					strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ".\n" );

					if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return startError( functionNameString, NULL, "I failed to write an interface warning" );
					}
				}
			}

		if( additionalDefinitionSpecificationItem_ != NULL &&

			( additionalDefinitionSpecificationItem_->isDeletedItem() ||

			( isIncludingReplacedSpecifications &&
			additionalDefinitionSpecificationItem_->isReplacedItem() ) ) )
			{
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcpy( errorString, "\nI found a replaced or deleted additional definition specification item:\n\tSpecificationItem: " );
			strcat( errorString, additionalDefinitionSpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		if( secondarySpecificationItem_ != NULL )
			{
			if( secondarySpecificationItem_->isReplacedItem() )
				{
				if( isIncludingReplacedSpecifications &&
				// Replace or delete unreferenced justification from my word
				_myWordItem->replaceOrDeleteJustification( this ) != RESULT_OK )
					return startError( functionNameString, NULL, "I failed to replace or delete an unreferenced justification from my word" );
				}
			else
				{
				if( secondarySpecificationItem_->isDeletedItem() )
					{
					// The result of this call is stored in a shared string. Hence, one call at the time.
					strcpy( errorString, "\nI found a replaced or deleted secondary specification item:\n\tSpecificationItem: " );
					strcat( errorString, secondarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ";\n\tJustificationItem: " );
					strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
					strcat( errorString, ".\n" );

					// Write interface warning
					if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
						return startError( functionNameString, NULL, "I failed to write an interface warning" );
					}
				}
			}

		if( additionalProperNounSpecificationItem_ != NULL &&

			( additionalProperNounSpecificationItem_->isDeletedItem() ||

			( isIncludingReplacedSpecifications &&
			additionalProperNounSpecificationItem_->isReplacedItem() ) ) )
			{
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcpy( errorString, "\nI found a replaced or deleted additional proper noun specification item:\n\tSpecificationItem: " );
			strcat( errorString, additionalProperNounSpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			// Write interface warning
			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char JustificationItem::replaceSpecification( bool isReplacingPrimarySpecification, bool isReplacingSecondarySpecification, SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceSpecification";

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( isReplacingPrimarySpecification )
			primarySpecificationItem_ = replacingSpecificationItem;

//		if( isReplacingAdditionalDefinitionSpecification )
//			additionalDefinitionSpecificationItem_ = replacingSpecificationItem;

		if( isReplacingSecondarySpecification )
			secondarySpecificationItem_ = replacingSpecificationItem;

//		if( isReplacingAdditionalProperNounSpecification )
//			additionalProperNounSpecificationItem_ = replacingSpecificationItem;

		return RESULT_OK;
		}

	JustificationItem *JustificationItem::attachedJustificationItem()
		{
		return attachedJustificationItem_;
		}

	JustificationItem *JustificationItem::attachedPredecessorOfObsoleteJustificationItem( JustificationItem *obsoleteJustificationItem )
		{
		JustificationItem *searchJustificationItem = this;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->attachedJustificationItem_ == obsoleteJustificationItem )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem_;
			}

		return NULL;
		}

	JustificationItem *JustificationItem::nextJustificationItem()
		{
		return (JustificationItem *)nextItem;
		}

	JustificationItem *JustificationItem::nextJustificationItemWithSameTypeAndOrderNr()
		{
		JustificationItem *searchJustificationItem = attachedJustificationItem_;

		while( searchJustificationItem != NULL )
			{
			if( isSameJustificationType( searchJustificationItem ) )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem_;
			}

		return NULL;
		}

	JustificationItem *JustificationItem::nextJustificationItemWithDifferentTypeOrOrderNr( JustificationItem *firstJustificationItem )
		{
		JustificationItem *nextTypeJustificationItem = attachedJustificationItem_;
		JustificationItem *usedTypeJustificationItem;

		if( firstJustificationItem != NULL )
			{
			do	{
				// Find next occurrence with different type
				while( nextTypeJustificationItem != NULL &&
				isSameJustificationType( nextTypeJustificationItem ) )
					nextTypeJustificationItem = nextTypeJustificationItem->attachedJustificationItem_;

				if( nextTypeJustificationItem != NULL )
					{
					// Check if different type is already used
					usedTypeJustificationItem = firstJustificationItem;

					while( usedTypeJustificationItem != NULL &&
					!usedTypeJustificationItem->isSameJustificationType( nextTypeJustificationItem ) )
						usedTypeJustificationItem = usedTypeJustificationItem->attachedJustificationItem_;

					if( usedTypeJustificationItem == nextTypeJustificationItem )
						return nextTypeJustificationItem;
					}
				}
			while( nextTypeJustificationItem != NULL &&
			( nextTypeJustificationItem = nextTypeJustificationItem->attachedJustificationItem_ ) != NULL );
			}

		return NULL;
		}

	JustificationItem *JustificationItem::obsoleteSpanishJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		if( primarySpecificationItem_ == primarySpecificationItem &&
		secondarySpecificationItem != NULL &&
		secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isSelfGeneratedSpecification() &&
		secondarySpecificationItem_->generalizationWordItem() == secondarySpecificationItem->generalizationWordItem() )
			return this;

		// Recursive, do for all attached justification items
		if( attachedJustificationItem_ != NULL )
			return attachedJustificationItem_->obsoleteSpanishJustificationItem( primarySpecificationItem, secondarySpecificationItem );

		return NULL;
		}

	JustificationItem *JustificationItem::primarySpecificationWithoutRelationContextJustificationItem( WordItem *primarySpecificationWordItem )
		{
		if( primarySpecificationWordItem != NULL &&
		primarySpecificationItem_ != NULL )
			{
			if( primarySpecificationItem_->isReplacedOrDeletedItem() ||

			( !primarySpecificationItem_->hasRelationWord() &&
			primarySpecificationItem_->specificationWordItem() == primarySpecificationWordItem ) )
				return this;

			// Recursive, do for all attached justification items
			if( attachedJustificationItem_ != NULL )
				return attachedJustificationItem_->primarySpecificationWithoutRelationContextJustificationItem( primarySpecificationWordItem );
			}

		return NULL;
		}

	SpecificationItem *JustificationItem::primarySelfGeneratedSpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isSelfGeneratedSpecification() ? primarySpecificationItem_ : NULL );
		}

	SpecificationItem *JustificationItem::primarySpecificationItem()
		{
		return primarySpecificationItem_;
		}

	SpecificationItem *JustificationItem::additionalDefinitionSpecificationItem()
		{
		return additionalDefinitionSpecificationItem_;
		}

	SpecificationItem *JustificationItem::secondarySpecificationItem()
		{
		return secondarySpecificationItem_;
		}

	SpecificationItem *JustificationItem::additionalProperNounSpecificationItem()
	{
		return additionalProperNounSpecificationItem_;
	}

	SpecificationItem *JustificationItem::updatedPrimarySpecificationItem()
		{
		return ( primarySpecificationItem_ == NULL ? NULL : primarySpecificationItem_->updatedSpecificationItem() );
		}

	SpecificationItem *JustificationItem::updatedSecondarySpecificationItem()
		{
		return ( secondarySpecificationItem_ == NULL ? NULL : secondarySpecificationItem_->updatedSpecificationItem() );
		}

	WordItem *JustificationItem::generalizationWordItem()
		{
		return myWordItem();
		}

	WordItem *JustificationItem::primaryGeneralizationWordItem()
		{
		return ( primarySpecificationItem_ == NULL ? NULL : primarySpecificationItem_->generalizationWordItem() );
		}

	WordItem *JustificationItem::primarySpecificationWordItem()
		{
		return ( primarySpecificationItem_ == NULL ? NULL : primarySpecificationItem_->specificationWordItem() );
		}

	WordItem *JustificationItem::secondaryGeneralizationWordItem()
		{
		return ( secondarySpecificationItem_ == NULL ? NULL : secondarySpecificationItem_->generalizationWordItem() );
		}

/*************************************************************************
 *	"The voice of the Lord is powerful;
 *	the voice of the Lord is majestic." (Psalm 29:4)
 *************************************************************************/
