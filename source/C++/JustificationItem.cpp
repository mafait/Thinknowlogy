/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
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

	bool JustificationItem::isContextSimilarInContextWords( unsigned int firstContextNr, unsigned int secondContextNr )
		{
		WordItem *currentContextWordItem;

		if( firstContextNr > NO_CONTEXT_NR &&
		secondContextNr > NO_CONTEXT_NR &&
		firstContextNr != secondContextNr &&
		( currentContextWordItem = globalVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( !currentContextWordItem->isContextSimilarInWord( firstContextNr, secondContextNr ) )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return true;
		}

	bool JustificationItem::isSameJustificationType( JustificationItem *referenceJustificationItem )
		{
		return ( referenceJustificationItem != NULL &&
				orderNr == referenceJustificationItem->orderNr &&

				( justificationTypeNr_ == referenceJustificationItem->justificationTypeNr() ||

				( primarySpecificationItem_ == NULL &&
				referenceJustificationItem->primarySpecificationItem_ == NULL &&
				isReversibleAssumptionOrConclusion() == referenceJustificationItem->isReversibleAssumptionOrConclusion() ) ) );
		}


	// Constructor

	JustificationItem::JustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, unsigned short _orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "JustificationItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		hasFeminineOrMasculineProperNounEnding_ = hasFeminineOrMasculineProperNounEnding;

		justificationTypeNr_ = justificationTypeNr;

		primarySpecificationItem_ = primarySpecificationItem;
		anotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
		secondarySpecificationItem_ = secondarySpecificationItem;
		anotherSecondarySpecificationItem_ = anotherSecondarySpecificationItem;

		attachedJustificationItem_ = attachedJustificationItem;

		// Protected constructed variables

		orderNr = _orderNr;
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

		if( anotherPrimarySpecificationItem_ != NULL )
			anotherPrimarySpecificationItem_->isSelectedByJustificationQuery = true;

		if( secondarySpecificationItem_ != NULL )
			secondarySpecificationItem_->isSelectedByJustificationQuery = true;

		if( anotherSecondarySpecificationItem_ != NULL )
			anotherSecondarySpecificationItem_->isSelectedByJustificationQuery = true;
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

				( anotherPrimarySpecificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : anotherPrimarySpecificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : anotherPrimarySpecificationItem_->itemNr() == queryItemNr ) ) ||

				( secondarySpecificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : secondarySpecificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : secondarySpecificationItem_->itemNr() == queryItemNr ) ) ||

				( anotherSecondarySpecificationItem_ == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : anotherSecondarySpecificationItem_->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : anotherSecondarySpecificationItem_->itemNr() == queryItemNr ) ) ||

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

			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isOppositePossessiveConditionalSpecificationAssumption" );
				break;

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isExclusiveSpecificationSubstitutionAssumption" );
				break;

			case JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isIndirectlyAnsweredQuestionAssumption" );
				break;

			case JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSuggestiveQuestionAssumption" );
				break;

			case JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isOnlyOptionLeftAssumption" );
				break;

			case JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isReversibleAssumption" );
				break;

			case JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isDefinitionPartOfAssumption" );
				break;

			case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isNegativeAssumption" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationGeneralizationAssumption" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationSubstitutionAssumption" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationSubstitutionPartOfAssumption" );
				break;

			case JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isUniqueUserRelationAssumption" );
				break;

			case JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isOnlyOptionLeftConclusion" );
				break;

			case JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isReversibleConclusion" );
				break;

			case JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isDefinitionPartOfConclusion" );
				break;

			case JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isNegativeConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationGeneralizationConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationSubstitutionConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isSpecificationSubstitutionPartOfConclusion" );
				break;

			case JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isUniqueUserRelationConclusion" );
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

		sprintf( tempString, "%corderNr:%u", QUERY_SEPARATOR_CHAR, orderNr );
		strcat( queryString, tempString );

		if( primarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%cprimarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, primarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, primarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( anotherPrimarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%canotherPrimarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, anotherPrimarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, anotherPrimarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( secondarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%csecondarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, secondarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, secondarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		if( anotherSecondarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%canotherSecondarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, anotherSecondarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, anotherSecondarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
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

	bool JustificationItem::hasAttachedJustification()
		{
		return ( attachedJustificationItem_ != NULL );
		}

	bool JustificationItem::hasFeminineOrMasculineProperNounEnding()
		{
		return hasFeminineOrMasculineProperNounEnding_;
		}

	bool JustificationItem::hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem )
		{
		return ( primarySpecificationItem_ == primarySpecificationItem &&
				anotherPrimarySpecificationItem_ == anotherPrimarySpecificationItem &&
				secondarySpecificationItem_ == secondarySpecificationItem &&
				anotherSecondarySpecificationItem_ == anotherSecondarySpecificationItem );
		}

	bool JustificationItem::hasJustification( bool hasFeminineOrMasculineProperNounEnding, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem )
		{
		return ( hasFeminineOrMasculineProperNounEnding_ == hasFeminineOrMasculineProperNounEnding &&
				primarySpecificationItem_ == primarySpecificationItem &&
				anotherPrimarySpecificationItem_ == anotherPrimarySpecificationItem &&
				secondarySpecificationItem_ == secondarySpecificationItem &&
				anotherSecondarySpecificationItem_ == anotherSecondarySpecificationItem );
		}

	bool JustificationItem::hasNonPossessivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				!primarySpecificationItem_->isPossessive() );
		}

	bool JustificationItem::hasPossessivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isPossessive() );
		}

	bool JustificationItem::hasPossessiveSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != NULL &&
				secondarySpecificationItem_->isPossessive() );
		}

	bool JustificationItem::hasReplacedPrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isReplacedItem() );
		}

	bool JustificationItem::isPrimarySpecificationWordSpanishAmbiguous()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isSpecificationWordSpanishAmbiguous() );
		}

	bool JustificationItem::isUpdatedPrimarySpecificationWordSpanishAmbiguous()
		{
		WordItem *updatedPrimarySpecificationWordItem;

		return	( primarySpecificationItem_ != NULL &&
				( updatedPrimarySpecificationWordItem = primarySpecificationItem_->updatedSpecificationItem()->specificationWordItem() ) != NULL &&
				updatedPrimarySpecificationWordItem->isNounWordSpanishAmbiguous() );
		}

	bool JustificationItem::hasAnotherPrimarySpecification()
		{
		return ( anotherPrimarySpecificationItem_ != NULL );
		}

	bool JustificationItem::isAssumptionJustification()
		{
		return isAssumption( justificationTypeNr_ );
		}

	bool JustificationItem::isConclusionJustification()
		{
		return isConclusion( justificationTypeNr_ );
		}

	bool JustificationItem::isExclusiveSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	bool JustificationItem::isGeneralizationAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION );
		}

	bool JustificationItem::isNegativeAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
				justificationTypeNr_ == JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION );
		}

	bool JustificationItem::isOppositePossessiveConditionalSpecificationAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );
		}

	bool JustificationItem::isReversibleAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION );
		}

	bool JustificationItem::isReversibleConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION );
		}

	bool JustificationItem::isReversibleAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION ||
				justificationTypeNr_ == JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION );
		}

	bool JustificationItem::isQuestionJustification()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION );
		}

	bool JustificationItem::isSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	bool JustificationItem::isSuggestiveQuestionAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION );
		}

	unsigned short JustificationItem::justificationAssumptionGrade()
		{
		bool hasPossessivePrimarySpecification = false;
		bool hasPrimaryQuestionSpecification = false;

		if( primarySpecificationItem_ != NULL )
			{
			if( primarySpecificationItem_->isGeneralizationProperNoun() &&
			primarySpecificationItem_->isPossessive() )
				hasPossessivePrimarySpecification = true;

			if( primarySpecificationItem_->isQuestion() )
				hasPrimaryQuestionSpecification = true;
			}

		return assumptionGrade( ( anotherPrimarySpecificationItem_ != NULL ), hasFeminineOrMasculineProperNounEnding_, hasPossessivePrimarySpecification, hasPrimaryQuestionSpecification, justificationTypeNr_ );
		}

	unsigned short JustificationItem::justificationTypeNr()
		{
		return justificationTypeNr_;
		}

	unsigned short JustificationItem::updatedJustificationTypeNr( SpecificationItem *newSecondarySpecificationItem )
		{
		if( justificationTypeNr_ == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION &&
		newSecondarySpecificationItem != NULL &&
		!newSecondarySpecificationItem->isPossessive() &&
		newSecondarySpecificationItem->isUserSpecification() )
			return ( newSecondarySpecificationItem->hasAssumptionLevel() ? JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION : JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION );

		return justificationTypeNr_;
		}

	unsigned short JustificationItem::primarySpecificationAssumptionLevel()
		{
		return ( primarySpecificationItem_ != NULL ?
				primarySpecificationItem_->assumptionLevel() : NO_ASSUMPTION_LEVEL );
		}

	unsigned int JustificationItem::nJustificationContextRelations( unsigned int relationContextNr, unsigned int nRelationWords )
		{
		unsigned int primaryRelationContextNr;
		unsigned int secondaryRelationContextNr;

		if( relationContextNr > NO_CONTEXT_NR )
			{
			if( primarySpecificationItem_ != NULL &&
			( primaryRelationContextNr = primarySpecificationItem_->relationContextNr() ) > NO_CONTEXT_NR )
				return myWordItem()->nContextWords( primaryRelationContextNr, primarySpecificationItem_->specificationWordItem() );

			if( secondarySpecificationItem_ != NULL &&
			( secondaryRelationContextNr = secondarySpecificationItem_->relationContextNr() ) > NO_CONTEXT_NR )
				{
				if( anotherPrimarySpecificationItem_ != NULL )
					return myWordItem()->nContextWords( secondaryRelationContextNr, secondarySpecificationItem_->specificationWordItem() );

				if( isContextSimilarInContextWords( secondaryRelationContextNr, relationContextNr ) )
					return nRelationWords;
				}

			return 1;
			}

		return 0;
		}

	unsigned int JustificationItem::primarySpecificationCollectionNr()
		{
		return ( primarySpecificationItem_ != NULL ?
				primarySpecificationItem_->specificationCollectionNr() : NO_COLLECTION_NR );
		}

	unsigned int JustificationItem::secondarySpecificationCollectionNr()
		{
		return ( secondarySpecificationItem_ != NULL ?
				secondarySpecificationItem_->specificationCollectionNr() : NO_COLLECTION_NR );
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

		attachedJustificationItem_ = NULL;

		if( newAttachedJustificationItem != NULL &&
		!newAttachedJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given new attached justification item isn't active" );

		if( newAttachedJustificationItem == this )
			return startError( functionNameString, NULL, "The given new attached justification item is the same justification item as me" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( newAttachedJustificationItem == NULL ||
		!newAttachedJustificationItem->hasJustification( this ) )
			attachedJustificationItem_ = newAttachedJustificationItem;

		return RESULT_OK;
		}

	signed char JustificationItem::changePrimarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changePrimarySpecification";

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		primarySpecificationItem_ = replacingSpecificationItem;

		return RESULT_OK;
		}

	signed char JustificationItem::changeSecondarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "changeSecondarySpecification";

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		secondarySpecificationItem_ = replacingSpecificationItem;

		return RESULT_OK;
		}

	signed char JustificationItem::checkForDeletedSpecifications()
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForDeletedSpecifications";

		if( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isDeletedItem() )
			{
			strcpy( errorString, "\nI found a deleted primary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, primarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		if( anotherPrimarySpecificationItem_ != NULL &&
		anotherPrimarySpecificationItem_->isDeletedItem() )
			{
			strcpy( errorString, "\nI found a deleted another primary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, anotherPrimarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isDeletedItem() )
			{
			strcpy( errorString, "\nI found a deleted secondary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, secondarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		if( anotherSecondarySpecificationItem_ != NULL &&
		anotherSecondarySpecificationItem_->isDeletedItem() )
			{
			strcpy( errorString, "\nI found a deleted another secondary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, anotherSecondarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char JustificationItem::checkForReplacedOrDeletedSpecification()
		{
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForReplacedOrDeletedSpecification";

		if( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isReplacedOrDeletedItem() )
			{
			strcpy( errorString, "\nI found a replaced or deleted primary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, primarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		if( anotherPrimarySpecificationItem_ != NULL &&
		anotherPrimarySpecificationItem_->isReplacedOrDeletedItem() )
			{
			strcpy( errorString, "\nI found a replaced or deleted another primary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, anotherPrimarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isReplacedOrDeletedItem() )
			{
			strcpy( errorString, "\nI found a replaced or deleted secondary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, secondarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		if( anotherSecondarySpecificationItem_ != NULL &&
		anotherSecondarySpecificationItem_->isReplacedOrDeletedItem() )
			{
			strcpy( errorString, "\nI found a replaced or deleted another secondary specification item:\n\tSpecificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, anotherSecondarySpecificationItem_->itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ";\n\tJustificationItem: " );
			// The result of this call is stored in a shared string. Hence, one call at the time.
			strcat( errorString, itemToString( NO_WORD_TYPE_NR ) );
			strcat( errorString, ".\n" );

			if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
				return startError( functionNameString, NULL, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	JustificationItem *JustificationItem::attachedJustificationItem()
		{
		return attachedJustificationItem_;
		}

	JustificationItem *JustificationItem::attachedPredecessorOfOldJustificationItem( JustificationItem *obsoleteJustificationItem )
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
		if( ( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isSelfGeneratedQuestion() ) ||

		( primarySpecificationItem_ == primarySpecificationItem &&
		secondarySpecificationItem != NULL &&
		secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isSelfGenerated() &&
		secondarySpecificationItem_->generalizationWordItem() == secondarySpecificationItem->generalizationWordItem() ) )
			return this;

		// Recursive, do for all attached justification items
		if( attachedJustificationItem_ != NULL )
			return attachedJustificationItem_->obsoleteSpanishJustificationItem( primarySpecificationItem, secondarySpecificationItem );

		return NULL;
		}

	JustificationItem *JustificationItem::primarySpecificationWithoutRelationContextJustificationItem( WordItem *primarySpecificationWordItem )
		{
		if( primarySpecificationWordItem != NULL )
			{
			if( primarySpecificationItem_ != NULL &&

			( primarySpecificationItem_->isReplacedItem() ||

			( !primarySpecificationItem_->hasRelationContext() &&
			primarySpecificationItem_->specificationWordItem() == primarySpecificationWordItem ) ) )
				return this;

			// Recursive, do for all attached justification items
			if( attachedJustificationItem_ != NULL )
				return attachedJustificationItem_->primarySpecificationWithoutRelationContextJustificationItem( primarySpecificationWordItem );
			}

		return NULL;
		}

	JustificationItem *JustificationItem::secondarySpecificationQuestion()
		{
		JustificationItem *searchJustificationItem = this;
		SpecificationItem *secondarySpecificationItem;

		while( searchJustificationItem != NULL )
			{
			if( ( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem_ ) != NULL &&
			secondarySpecificationItem->isQuestion() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem_;
			}

		return NULL;
		}

	SpecificationItem *JustificationItem::primarySpecificationItem()
		{
		return primarySpecificationItem_;
		}

	SpecificationItem *JustificationItem::anotherPrimarySpecificationItem()
		{
		return anotherPrimarySpecificationItem_;
		}

	SpecificationItem *JustificationItem::secondarySpecificationItem()
		{
		return secondarySpecificationItem_;
		}

	SpecificationItem *JustificationItem::anotherSecondarySpecificationItem()
		{
		return anotherSecondarySpecificationItem_;
		}

	SpecificationItem *JustificationItem::updatedPrimarySpecificationItem()
		{
		return ( primarySpecificationItem_ != NULL ?
				primarySpecificationItem_->updatedSpecificationItem() : NULL );
		}

	SpecificationItem *JustificationItem::updatedSecondarySpecificationItem()
		{
		return ( secondarySpecificationItem_ != NULL ?
				secondarySpecificationItem_->updatedSpecificationItem() : NULL );
		}

	WordItem *JustificationItem::generalizationWordItem()
		{
		return myWordItem();
		}

	WordItem *JustificationItem::primarySpecificationWordItem()
		{
		return ( primarySpecificationItem_ != NULL ?
				primarySpecificationItem_->specificationWordItem() : NULL );
		}

	WordItem *JustificationItem::secondaryGeneralizationWordItem()
		{
		return ( secondarySpecificationItem_ != NULL ?
				secondarySpecificationItem_->generalizationWordItem() : NULL );
		}

	WordItem *JustificationItem::secondarySpecificationWordItem()
		{
		return ( secondarySpecificationItem_ != NULL ?
				secondarySpecificationItem_->specificationWordItem() : NULL );
		}

	ShortResultType JustificationItem::getCombinedAssumptionLevel()
		{
		unsigned int assumptionLevel = NO_ASSUMPTION_LEVEL;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getCombinedAssumptionLevel";

		if( primarySpecificationItem_ != NULL )
			assumptionLevel += primarySpecificationItem_->assumptionLevel();

		if( anotherPrimarySpecificationItem_ != NULL )
			assumptionLevel += anotherPrimarySpecificationItem_->assumptionLevel();

		if( secondarySpecificationItem_ != NULL )
			assumptionLevel += secondarySpecificationItem_->assumptionLevel();

		if( anotherSecondarySpecificationItem_ != NULL )
			assumptionLevel += anotherSecondarySpecificationItem_->assumptionLevel();

		if( assumptionLevel < MAX_LEVEL )
			shortResult.shortValue = (unsigned short)assumptionLevel;
		else
			shortResult.result = startSystemError( functionNameString, NULL, "Assumption level overflow" );

		return shortResult;
		}

/*************************************************************************
 *	"The voice of the Lord is powerful;
 *	the voice of the Lord is majestic." (Psalm 29:4)
 *************************************************************************/
