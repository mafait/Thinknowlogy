/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
 *	Version:		Thinknowlogy 2016r2 (Restyle)
 *************************************************************************/
/*	Copyright (C) 2009-2016, Menno Mafait. Your suggestions, modifications,
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

	bool JustificationItem::isContextSimilarInAllWords( unsigned int firstContextNr, unsigned int secondContextNr )
		{
		WordItem *currentContextWordItem;

		if( firstContextNr > NO_CONTEXT_NR &&
		secondContextNr > NO_CONTEXT_NR &&
		firstContextNr != secondContextNr &&
		( currentContextWordItem = commonVariables()->firstContextWordItem ) != NULL )
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

				( !hasPrimarySpecification() &&
				!referenceJustificationItem->hasPrimarySpecification() &&
				isPossessiveReversibleAssumptionOrConclusion() == referenceJustificationItem->isPossessiveReversibleAssumptionOrConclusion() ) ) );
		}


	// Constructor

	JustificationItem::JustificationItem( bool hasFeminineOrMasculineProperNameEnding, unsigned short justificationTypeNr, unsigned short _orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "JustificationItem", commonVariables, myList, myWordItem );

		// Private initialized variables

		hasFeminineOrMasculineProperNameEnding_ = hasFeminineOrMasculineProperNameEnding;

		justificationTypeNr_ = justificationTypeNr;

		primarySpecificationItem_ = primarySpecificationItem;
		anotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
		secondarySpecificationItem_ = secondarySpecificationItem;
		anotherSecondarySpecificationItem_ = anotherSecondarySpecificationItem;

		attachedJustificationItem_ = attachedJustificationItem;

		// Protected constructed variables

		hasJustificationBeenWritten = false;

		orderNr = _orderNr;

		replacingJustificationItem = NULL;
		}


	// Protected virtual functions

	void JustificationItem::clearReplacingInfo()
		{
		clearReplacedSentenceNr();
		replacingJustificationItem = NULL;
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
					( queryItemNr == NO_ITEM_NR ? true : attachedJustificationItem_->itemNr() == queryItemNr ) ) ||

				( replacingJustificationItem == NULL ? false :
					( querySentenceNr == NO_SENTENCE_NR ? true : replacingJustificationItem->creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == NO_ITEM_NR ? true : replacingJustificationItem->itemNr() == queryItemNr ) ) );
		}

	ResultType JustificationItem::checkForUsage()
		{
		return myWordItem()->checkJustificationForUsageInWord( this );
		}

	char *JustificationItem::toString( unsigned short queryWordTypeNr )
		{
		char *queryString;

		Item::toString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

		if( hasFeminineOrMasculineProperNameEnding_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "hasFeminineOrMasculineProperNameEnding" );
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

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isPossessiveReversibleAssumption" );
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

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION:
				strcat( queryString, QUERY_SEPARATOR_STRING );
				strcat( queryString, "isPossessiveReversibleConclusion" );
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

		if( replacingJustificationItem != NULL )
			{
			sprintf( tempString, "%creplacingJustificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, replacingJustificationItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, replacingJustificationItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected functions

	bool JustificationItem::hasAttachedJustification()
		{
		return ( attachedJustificationItem_ != NULL );
		}

	bool JustificationItem::hasFeminineOrMasculineProperNameEnding()
		{
		return hasFeminineOrMasculineProperNameEnding_;
		}

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

	bool JustificationItem::hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition()
		{
		JustificationItem *searchJustificationItem = this;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasPrimarySpecification() ||
			!searchJustificationItem->isExclusiveSpecificationSubstitutionAssumption() )
				return false;

			searchJustificationItem = searchJustificationItem->attachedJustificationItem_;
			}

		return true;
		}

	bool JustificationItem::hasHiddenPrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isHiddenSpanishSpecification() );
		}

	bool JustificationItem::hasPrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL );
		}

	bool JustificationItem::hasPrimaryAnsweredQuestion()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isAnsweredQuestion() );
		}

	bool JustificationItem::hasPrimaryQuestion()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isQuestion() );
		}

	bool JustificationItem::hasPrimaryUserSpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isUserSpecification() );
		}

	bool JustificationItem::hasPossessivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isPossessive() );
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

		if( primarySpecificationItem_ != NULL &&
		( updatedPrimarySpecificationWordItem = primarySpecificationItem_->updatedSpecificationItem()->specificationWordItem() ) != NULL )
			return updatedPrimarySpecificationWordItem->isNounWordSpanishAmbiguous();

		return false;
		}

	bool JustificationItem::hasAnotherPrimarySpecification()
		{
		return ( anotherPrimarySpecificationItem_ != NULL );
		}

	bool JustificationItem::hasReplacedSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != NULL &&
				secondarySpecificationItem_->isReplacedItem() );
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

	bool JustificationItem::isSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	bool JustificationItem::isSpecificationSubstitutionPartOfAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION );
		}

	bool JustificationItem::isSuggestiveQuestionAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION );
		}

	bool JustificationItem::isPossessiveReversibleAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION );
		}

	bool JustificationItem::isPossessiveReversibleConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION );
		}

	bool JustificationItem::isPossessiveReversibleAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||
				justificationTypeNr_ == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION );
		}

	bool JustificationItem::isQuestionJustification()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION );
		}

	unsigned short JustificationItem::justificationAssumptionGrade()
		{
		bool hasPossessivePrimarySpecification = false;
		bool hasPrimaryQuestionSpecification = false;

		if( primarySpecificationItem_ != NULL )
			{
			if( primarySpecificationItem_->isGeneralizationProperName() &&
			primarySpecificationItem_->isPossessive() )
				hasPossessivePrimarySpecification = true;

			if( primarySpecificationItem_->isQuestion() )
				hasPrimaryQuestionSpecification = true;
			}

		return assumptionGrade( ( anotherPrimarySpecificationItem_ != NULL ), hasFeminineOrMasculineProperNameEnding_, hasPossessivePrimarySpecification, hasPrimaryQuestionSpecification, justificationTypeNr_ );
		}

	unsigned short JustificationItem::justificationTypeNr()
		{
		return justificationTypeNr_;
		}

	unsigned short JustificationItem::primarySpecificationAssumptionLevel()
		{
		if( primarySpecificationItem_ != NULL )
			return primarySpecificationItem_->assumptionLevel();

		return NO_ASSUMPTION_LEVEL;
		}

	unsigned int JustificationItem::nJustificationContextRelations( unsigned int relationContextNr, unsigned int nSpecificationRelationWords )
		{
		unsigned int primaryRelationContextNr;
		unsigned int secondaryRelationContextNr;

		if( relationContextNr > NO_CONTEXT_NR )
			{
			if( primarySpecificationItem_ != NULL &&
			( primaryRelationContextNr = primarySpecificationItem_->relationContextNr() ) > NO_CONTEXT_NR )
				return myWordItem()->nContextWordsInAllWords( primaryRelationContextNr, primarySpecificationItem_->specificationWordItem() );

			if( secondarySpecificationItem_ != NULL &&
			( secondaryRelationContextNr = secondarySpecificationItem_->relationContextNr() ) > NO_CONTEXT_NR )
				{
				if( anotherPrimarySpecificationItem_ != NULL )
					return myWordItem()->nContextWordsInAllWords( secondaryRelationContextNr, secondarySpecificationItem_->specificationWordItem() );

				if( secondaryRelationContextNr == relationContextNr ||
				isContextSimilarInAllWords( secondaryRelationContextNr, relationContextNr ) )
					return nSpecificationRelationWords;
				}

			return 1;
			}

		return 0;
		}

	unsigned int JustificationItem::primarySpecificationCollectionNr()
		{
		if( primarySpecificationItem_ != NULL )
			return primarySpecificationItem_->specificationCollectionNr();

		return NO_COLLECTION_NR;
		}

	ResultType JustificationItem::attachJustification( JustificationItem *attachedJustificationItem, SpecificationItem *mySpecificationItem )
		{
		bool isMySpecification = false;
		JustificationItem *searchJustificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustification";

		if( attachedJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given attached justification item is undefined" );

		if( attachedJustificationItem == this )
			return startError( functionNameString, NULL, "The given attached justification item is the same justification item as me" );

		if( mySpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given my specification item is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( !attachedJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given attached justification item isn't active" );

		if( attachedJustificationItem_ != NULL )
			return startError( functionNameString, NULL, "I already have an attached justification item" );

		if( ( searchJustificationItem = mySpecificationItem->firstJustificationItem() ) == NULL )
			return startError( functionNameString, NULL, "The given my specification item has no justification item" );

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem == this )
				isMySpecification = true;

			if( searchJustificationItem == attachedJustificationItem )
				return startError( functionNameString, NULL, "The given attached justification item is already one of the attached justification items of my specification item" );

			searchJustificationItem = searchJustificationItem->attachedJustificationItem_;
			}

		if( !isMySpecification )
			return startError( functionNameString, NULL, "The given my specification item isn't my specification item" );

		// Add attached justification item
		attachedJustificationItem_ = attachedJustificationItem;

		return RESULT_OK;
		}

	ResultType JustificationItem::changeAttachedJustification( JustificationItem *newAttachedJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeAttachedJustification";

		attachedJustificationItem_ = NULL;

		if( newAttachedJustificationItem != NULL &&
		!newAttachedJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given new attached justification item isn't active" );

		if( newAttachedJustificationItem == this )
			return startError( functionNameString, NULL, "The given new attached justification item is the same justification item as me" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		if( newAttachedJustificationItem == NULL ||

		( !hasJustification( newAttachedJustificationItem ) &&
		!newAttachedJustificationItem->hasJustification( this ) ) )
			attachedJustificationItem_ = newAttachedJustificationItem;

		return RESULT_OK;
		}

	ResultType JustificationItem::changePrimarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changePrimarySpecification";

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		primarySpecificationItem_ = replacingSpecificationItem;

		return RESULT_OK;
		}

	ResultType JustificationItem::changeAnotherPrimarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeAnotherPrimarySpecification";

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		anotherPrimarySpecificationItem_ = replacingSpecificationItem;

		return RESULT_OK;
		}

	ResultType JustificationItem::changeSecondarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeSecondarySpecification";

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		secondarySpecificationItem_ = replacingSpecificationItem;

		return RESULT_OK;
		}

	ResultType JustificationItem::changeAnotherSecondarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeAnotherSecondarySpecification";

		if( replacingSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

		anotherSecondarySpecificationItem_ = replacingSpecificationItem;

		return RESULT_OK;
		}

	ResultType JustificationItem::checkForDeletedSpecifications()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDeletedSpecifications";

		if( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isDeletedItem() )
			return startError( functionNameString, NULL, "My primary specification is a deleted item" );

		if( anotherPrimarySpecificationItem_ != NULL &&
		anotherPrimarySpecificationItem_->isDeletedItem() )
			return startError( functionNameString, NULL, "My another primary specification is a deleted item" );

		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isDeletedItem() )
			return startError( functionNameString, NULL, "My secondary specification is a deleted item" );

		if( anotherSecondarySpecificationItem_ != NULL &&
		anotherSecondarySpecificationItem_->isDeletedItem() )
			return startError( functionNameString, NULL, "My another secondary specification is a deleted item" );

		return RESULT_OK;
		}

	ResultType JustificationItem::checkForReplacedOrDeletedSpecification()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForReplacedOrDeletedSpecification";

		if( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "My primary specification is a replaced or a deleted item" );

		if( anotherPrimarySpecificationItem_ != NULL &&
		anotherPrimarySpecificationItem_->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "My another primary specification is a replaced or a deleted item" );

		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "My secondary specification is a replaced or a deleted item" );

		if( anotherSecondarySpecificationItem_ != NULL &&
		anotherSecondarySpecificationItem_->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "My another secondary specification is a replaced or a deleted item" );

		return RESULT_OK;
		}

	SpecificationResultType JustificationItem::getCombinedAssumptionLevel()
		{
		SpecificationResultType specificationResult;
		unsigned int combinedAssumptionLevel = NO_ASSUMPTION_LEVEL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getCombinedAssumptionLevel";

		if( primarySpecificationItem_ != NULL &&
		( specificationResult = primarySpecificationItem_->getAssumptionLevel() ).result == RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( specificationResult.result == RESULT_OK &&
		anotherPrimarySpecificationItem_ != NULL &&
		( specificationResult = anotherPrimarySpecificationItem_->getAssumptionLevel() ).result == RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( specificationResult.result == RESULT_OK &&
		secondarySpecificationItem_ != NULL &&
		( specificationResult = secondarySpecificationItem_->getAssumptionLevel() ).result == RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( specificationResult.result == RESULT_OK &&
		anotherSecondarySpecificationItem_ != NULL &&
		( specificationResult = anotherSecondarySpecificationItem_->getAssumptionLevel() ).result == RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( combinedAssumptionLevel < MAX_LEVEL )
			specificationResult.combinedAssumptionLevel = (unsigned short)combinedAssumptionLevel;
		else
			specificationResult.result = startSystemError( functionNameString, NULL, "Assumption level overflow" );

		return specificationResult;
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
/*
		// Recursive alternative:
		return ( attachedJustificationItem_ == NULL ? NULL : attachedJustificationItem_ == obsoleteJustificationItem ? this : attachedJustificationItem_->attachedPredecessorOfOldJustificationItem( obsoleteJustificationItem ) );
*/		}

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
		JustificationItem *usedTypeJustificationItem;
		JustificationItem *nextTypeJustificationItem = attachedJustificationItem_;

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
		secondarySpecificationItem_->generalizationWordItem() == secondarySpecificationItem->generalizationWordItem() &&
		secondarySpecificationItem_->specificationWordItem() == secondarySpecificationItem->specificationWordItem() ) )
			return this;

		// Recursive, do for all attached justification items
		if( attachedJustificationItem_ != NULL )
			return attachedJustificationItem_->obsoleteSpanishJustificationItem( primarySpecificationItem, secondarySpecificationItem );

		return NULL;
		}

	JustificationItem *JustificationItem::primarySpecificationWithoutRelationContextJustificationItem( SpecificationItem *primarySpecificationItem )
		{
		if( primarySpecificationItem != NULL )
			{
			if( primarySpecificationItem_ != NULL &&

			( primarySpecificationItem_->isReplacedItem() ||

			( !primarySpecificationItem_->hasRelationContext() &&
			primarySpecificationItem_->specificationWordItem() == primarySpecificationItem->specificationWordItem() ) ) )
				return this;

			// Recursive, do for all attached justification items
			if( attachedJustificationItem_ != NULL )
				return attachedJustificationItem_->primarySpecificationWithoutRelationContextJustificationItem( primarySpecificationItem );
			}

		return NULL;
		}

	JustificationItem *JustificationItem::secondarySpecificationQuestion()
		{
		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isQuestion() )
			return this;

		// Recursive, do for all attached justification items
		if( attachedJustificationItem_ != NULL )
			return attachedJustificationItem_->secondarySpecificationQuestion();

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

	WordItem *JustificationItem::primarySpecificationWordItem()
		{
		if( primarySpecificationItem_ != NULL )
			return primarySpecificationItem_->specificationWordItem();

		return NULL;
		}

	WordItem *JustificationItem::secondarySpecificationWordItem()
		{
		if( secondarySpecificationItem_ != NULL )
			return secondarySpecificationItem_->specificationWordItem();

		return NULL;
		}

/*************************************************************************
 *	"The voice of the Lord is powerful;
 *	the voice of the Lord is majestic." (Psalm 29:4)
 *************************************************************************/
