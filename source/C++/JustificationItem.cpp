/*
 *	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

	bool JustificationItem::isSameJustificationType( JustificationItem *referenceJustificationItem )
		{
		return ( referenceJustificationItem != NULL &&
				orderNr == referenceJustificationItem->orderNr &&
				justificationTypeNr_ == referenceJustificationItem->justificationTypeNr() );
		}


	// Constructor / deconstructor

	JustificationItem::JustificationItem( unsigned short justificationTypeNr, unsigned short _orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( originalSentenceNr, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "JustificationItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		justificationTypeNr_ = justificationTypeNr;

		primarySpecificationItem_ = primarySpecificationItem;
		anotherPrimarySpecificationItem_ = anotherPrimarySpecificationItem;
		secondarySpecificationItem_ = secondarySpecificationItem;
		anotherSecondarySpecificationItem_ = anotherSecondarySpecificationItem;

		attachedJustificationItem_ = attachedJustificationItem;

		// Protected constructible variables

		hasBeenWritten = false;

		orderNr = _orderNr;

		replacingJustificationItem = NULL;
		}


	// Protected virtual functions

	bool JustificationItem::hasFoundWordType( unsigned short queryWordTypeNr )
		{
		return ( justificationTypeNr_ == queryWordTypeNr );
		}

	bool JustificationItem::hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
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
		Item::toString( queryWordTypeNr );

		switch( justificationTypeNr_ )
			{
			case JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isGeneralizationAssumption" );
				break;

			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isOppositePossessiveConditionalSpecificationAssumption" );
				break;

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isExclusiveSpecificationSubstitutionAssumption" );
				break;

			case JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isFeminineOrMasculineProperNameEndingAssumption" );
				break;

			case JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isIndirectlyAnsweredQuestionAssumption" );
				break;

			case JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSuggestiveQuestionAssumption" );
				break;

			case JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isOnlyOptionLeftAssumption" );
				break;

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isPossessiveReversibleAssumption" );
				break;

			case JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isDefinitionPartOfAssumption" );
				break;

			case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isNegativeAssumption" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSpecificationGeneralizationAssumption" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSpecificationSubstitutionAssumption" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSpecificationSubstitutionPartOfAssumption" );
				break;

			case JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isUniqueRelationAssumption" );
				break;

			case JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isOnlyOptionLeftConclusion" );
				break;

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isPossessiveReversibleConclusion" );
				break;

			case JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isDefinitionPartOfConclusion" );
				break;

			case JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isNegativeConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSpecificationGeneralizationConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSpecificationSubstitutionConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSpecificationSubstitutionPartOfConclusion" );
				break;

			case JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isUniqueRelationConclusion" );
				break;

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				strcat( commonVariables()->queryString, QUERY_SEPARATOR_STRING );
				strcat( commonVariables()->queryString, "isSpecificationSubstitutionQuestion" );
				break;

			default:
				sprintf( tempString, "%cjustificationType:%u", QUERY_SEPARATOR_CHAR, justificationTypeNr_ );
				strcat( commonVariables()->queryString, tempString );
			}

		sprintf( tempString, "%c%u", QUERY_WORD_TYPE_CHAR, justificationTypeNr_ );
		strcat( commonVariables()->queryString, tempString );

		sprintf( tempString, "%corderNr:%u", QUERY_SEPARATOR_CHAR, orderNr );
		strcat( commonVariables()->queryString, tempString );

		if( primarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%cprimarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, primarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, primarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( anotherPrimarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%canotherPrimarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, anotherPrimarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, anotherPrimarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( secondarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%csecondarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, secondarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, secondarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( anotherSecondarySpecificationItem_ != NULL )
			{
			sprintf( tempString, "%canotherSecondarySpecificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, anotherSecondarySpecificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, anotherSecondarySpecificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( attachedJustificationItem_ != NULL )
			{
			sprintf( tempString, "%cattachedJustificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, attachedJustificationItem_->creationSentenceNr(), QUERY_SEPARATOR_CHAR, attachedJustificationItem_->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		if( replacingJustificationItem != NULL )
			{
			sprintf( tempString, "%creplacingJustificationItem%c%u%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_REF_ITEM_START_CHAR, replacingJustificationItem->creationSentenceNr(), QUERY_SEPARATOR_CHAR, replacingJustificationItem->itemNr(), QUERY_REF_ITEM_END_CHAR );
			strcat( commonVariables()->queryString, tempString );
			}

		return commonVariables()->queryString;
		}


	// Protected functions

	bool JustificationItem::hasAttachedJustification()
		{
		return ( attachedJustificationItem_ != NULL );
		}

	bool JustificationItem::hasNewJustification()
		{
		JustificationItem *searchItem = this;

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				return true;

			searchItem = searchItem->attachedJustificationItem_;
			}

		return false;
/*
		// Recursive alternative:
		return ( hasCurrentCreationSentenceNr() ||

				( attachedJustificationItem_ != NULL &&
				attachedJustificationItem_->hasNewJustification() ) );
*/		}

	bool JustificationItem::hasFoundJustification( JustificationItem *existingJustificationItem )
		{
		JustificationItem *searchItem = this;

		while( searchItem != NULL )
			{
			if( searchItem == existingJustificationItem )
				return true;

			searchItem = searchItem->attachedJustificationItem_;
			}

		return false;
		}

	bool JustificationItem::hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition()
		{
		JustificationItem *searchItem = this;

		while( searchItem != NULL )
			{
			if( searchItem->hasPrimarySpecification() ||
			!searchItem->isExclusiveSpecificationSubstitutionAssumption() )
				return false;

			searchItem = searchItem->attachedJustificationItem_;
			}

		return true;
		}

	bool JustificationItem::hasPrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL );
		}

	bool JustificationItem::hasPrimaryQuestion()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isQuestion() );
		}

	bool JustificationItem::hasPrimarySpecificationWithRelationContext()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->hasRelationContext() );
		}

	bool JustificationItem::hasPrimaryUserSpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isUserSpecification() );
		}

	bool JustificationItem::hasHiddenPrimarySpecification()
		{
		WordItem *primarySpecificationWordItem;

		if( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isPossessive() &&
		primarySpecificationItem_->hasSpecificationCollection() &&
		( primarySpecificationWordItem = primarySpecificationItem_->specificationWordItem() ) != NULL )
			{
			if( primarySpecificationWordItem->isCollectedWithItself() ||
			primarySpecificationItem_->collectedWithItselfCommonWordItem() != NULL )
				return true;
			}

		return false;
		}

	bool JustificationItem::hasReplacedPrimarySpecification()
		{
		return ( primarySpecificationItem_ != NULL &&
				primarySpecificationItem_->isReplacedItem() );
		}

	bool JustificationItem::hasUpdatedPrimarySpecificationWordCollectedWithItself()
		{
		WordItem *updatedPrimarySpecificationWordItem;

		if( primarySpecificationItem_ != NULL &&
		( updatedPrimarySpecificationWordItem = primarySpecificationItem_->updatedSpecificationItem()->specificationWordItem() ) != NULL )
			return updatedPrimarySpecificationWordItem->isCollectedWithItself();

		return false;
		}

	bool JustificationItem::hasSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != NULL );
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

	bool JustificationItem::isFeminineOrMasculineProperNameEndingAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION );
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

	bool JustificationItem::isPossessiveReversibleAssumption()
		{
		return ( justificationTypeNr_ == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION );
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
		return assumptionGrade( justificationTypeNr_ );
		}

	unsigned short JustificationItem::justificationTypeNr()
		{
		return justificationTypeNr_;
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
				if( anotherPrimarySpecificationItem_ == NULL )
					{
					if( secondaryRelationContextNr == relationContextNr ||
					myWordItem()->isContextSimilarInAllWords( secondaryRelationContextNr, relationContextNr ) )
						return nSpecificationRelationWords;
					}
				else
					return myWordItem()->nContextWordsInAllWords( secondaryRelationContextNr, secondarySpecificationItem_->specificationWordItem() );
				}

			return 1;
			}

		return 0;
		}

	ResultType JustificationItem::attachJustification( JustificationItem *attachedJustificationItem, SpecificationItem *mySpecificationItem )
		{
		bool isMySpecification = false;
		JustificationItem *searchItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustification";

		if( attachedJustificationItem != NULL )
			{
			if( attachedJustificationItem != this )
				{
				if( mySpecificationItem != NULL )
					{
					if( hasCurrentCreationSentenceNr() )
						{
						if( attachedJustificationItem->isActiveItem() )
							{
							if( attachedJustificationItem_ == NULL )
								{
								if( ( searchItem = mySpecificationItem->firstJustificationItem() ) != NULL )
									{
									while( searchItem != NULL )
										{
										if( searchItem == this )
											isMySpecification = true;

										if( searchItem == attachedJustificationItem )
											return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item is already one of the attached justification items of my specification item" );

										searchItem = searchItem->attachedJustificationItem_;
										}

									if( isMySpecification )
										attachedJustificationItem_ = attachedJustificationItem;		// Add attached justification item
									else
										return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given my specification item isn't my specification item" );
									}
								else
									return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given my specification item has no justification item" );
								}
							else
								return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I already have an attached justification item" );
							}
						else
							return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item isn't active" );
						}
					else
						return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
					}
				else
					return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given my specification item is undefined" );
				}
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item is the same justification item as me" );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item is undefined" );

		return RESULT_OK;
		}

	ResultType JustificationItem::changeAttachedJustification( JustificationItem *newAttachedJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeAttachedJustification";

		attachedJustificationItem_ = NULL;

		if( newAttachedJustificationItem == NULL ||
		newAttachedJustificationItem->isActiveItem() )
			{
			if( newAttachedJustificationItem != this )
				{
				if( hasCurrentCreationSentenceNr() )
					{
					if( newAttachedJustificationItem == NULL ||

					( !hasFoundJustification( newAttachedJustificationItem ) &&
					!newAttachedJustificationItem->hasFoundJustification( this ) ) )
						attachedJustificationItem_ = newAttachedJustificationItem;
					}
				else
					return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given new attached justification item is the same justification item as me" );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given new attached justification item isn't active" );

		return RESULT_OK;
		}

	ResultType JustificationItem::changePrimarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changePrimarySpecification";

		if( replacingSpecificationItem != NULL )
			{
			if( replacingSpecificationItem->isActiveItem() )
				{
				if( hasCurrentCreationSentenceNr() )
					primarySpecificationItem_ = replacingSpecificationItem;
				else
					return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing specification item isn't active" );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing specification item is undefined" );

		return RESULT_OK;
		}

	ResultType JustificationItem::changeSecondarySpecification( SpecificationItem *replacingSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeSecondarySpecification";

		if( replacingSpecificationItem != NULL )
			{
			if( replacingSpecificationItem->isActiveItem() )
				{
				if( hasCurrentCreationSentenceNr() )
					secondarySpecificationItem_ = replacingSpecificationItem;
				else
					return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing specification item isn't active" );
			}
		else
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing specification item is undefined" );

		return RESULT_OK;
		}

	ResultType JustificationItem::checkForDeletedSpecifications()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDeletedSpecifications";

		if( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My primary specification is a deleted item" );

		if( anotherPrimarySpecificationItem_ != NULL &&
		anotherPrimarySpecificationItem_->isDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My another primary specification is a deleted item" );

		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My secondary specification is a deleted item" );

		if( anotherSecondarySpecificationItem_ != NULL &&
		anotherSecondarySpecificationItem_->isDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My another secondary specification is a deleted item" );

		return RESULT_OK;
		}

	ResultType JustificationItem::checkForReplacedOrDeletedSpecifications()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForReplacedOrDeletedSpecifications";

		if( primarySpecificationItem_ != NULL &&
		primarySpecificationItem_->isReplacedOrDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My primary specification is a replaced or a deleted item" );

		if( anotherPrimarySpecificationItem_ != NULL &&
		anotherPrimarySpecificationItem_->isReplacedOrDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My another primary specification is a replaced or a deleted item" );

		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isReplacedOrDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My secondary specification is a replaced or a deleted item" );

		if( anotherSecondarySpecificationItem_ != NULL &&
		anotherSecondarySpecificationItem_->isReplacedOrDeletedItem() )
			return startErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "My another secondary specification is a replaced or a deleted item" );

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

		if( commonVariables()->result == RESULT_OK &&
		anotherPrimarySpecificationItem_ != NULL &&
		( specificationResult = anotherPrimarySpecificationItem_->getAssumptionLevel() ).result == RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( commonVariables()->result == RESULT_OK &&
		secondarySpecificationItem_ != NULL &&
		( specificationResult = secondarySpecificationItem_->getAssumptionLevel() ).result == RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( combinedAssumptionLevel < MAX_LEVEL )
			specificationResult.combinedAssumptionLevel = (unsigned short)combinedAssumptionLevel;
		else
			specificationResult.result = startSystemErrorInItem( functionNameString, NULL, myWordItem()->anyWordTypeString(), "Assumption level overflow" );

		return specificationResult;
		}

	JustificationItem *JustificationItem::attachedJustificationItem()
		{
		return attachedJustificationItem_;
		}

	JustificationItem *JustificationItem::attachedPredecessorOfOldJustificationItem( JustificationItem *obsoleteJustificationItem )
		{
		JustificationItem *searchItem = this;

		while( searchItem != NULL )
			{
			if( searchItem->attachedJustificationItem_ == obsoleteJustificationItem )
				return searchItem;

			searchItem = searchItem->attachedJustificationItem_;
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
		JustificationItem *searchItem = attachedJustificationItem_;

		while( searchItem != NULL )
			{
			if( isSameJustificationType( searchItem ) )
				return searchItem;

			searchItem = searchItem->attachedJustificationItem_;
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

	JustificationItem *JustificationItem::primarySpecificationWithoutRelationContextJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		if( primarySpecificationItem != NULL )
			{
			if( primarySpecificationItem_ != NULL &&
			!primarySpecificationItem_->hasRelationContext() &&
			secondarySpecificationItem_ == secondarySpecificationItem &&
			primarySpecificationItem_->specificationWordItem() == primarySpecificationItem->specificationWordItem() )
				return this;

			if( attachedJustificationItem_ != NULL )
				return attachedJustificationItem_->primarySpecificationWithoutRelationContextJustificationItem( primarySpecificationItem, secondarySpecificationItem );
			}

		return NULL;
		}

	JustificationItem *JustificationItem::foundSecondarySpecificationQuestion()
		{
		if( secondarySpecificationItem_ != NULL &&
		secondarySpecificationItem_->isQuestion() )
			return this;

		if( attachedJustificationItem_ != NULL )
			return attachedJustificationItem_->foundSecondarySpecificationQuestion();

		return NULL;
		}

	JustificationItem *JustificationItem::selfGeneratedSecondarySpecificationJustificationItem( SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		if( secondarySpecificationItem != NULL &&
		secondarySpecificationItem_ != NULL )
			{
			if( primarySpecificationItem_ == primarySpecificationItem &&
			secondarySpecificationItem_->isSelfGenerated() &&
			secondarySpecificationItem_->specificationWordItem() == secondarySpecificationItem->specificationWordItem() )
				return this;

			if( attachedJustificationItem_ != NULL )
				return attachedJustificationItem_->selfGeneratedSecondarySpecificationJustificationItem( primarySpecificationItem, secondarySpecificationItem );
			}

		return NULL;
		}

	JustificationItem *JustificationItem::updatedJustificationItem()
		{
		JustificationItem *updatedJustificationItem;
		JustificationItem *searchItem = this;

		while( ( updatedJustificationItem = searchItem->replacingJustificationItem ) != NULL )
			searchItem = updatedJustificationItem;

		return searchItem;
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
