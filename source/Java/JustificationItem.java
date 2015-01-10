/*
 *	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class JustificationItem extends Item
	{
	// Private loadable variables

	private short justificationTypeNr_;

	private SpecificationItem primarySpecificationItem_;
	private SpecificationItem anotherPrimarySpecificationItem_;
	private SpecificationItem secondarySpecificationItem_;
	private SpecificationItem anotherSecondarySpecificationItem_;

	private JustificationItem attachedJustificationItem_;


	// Protected constructible variables

	protected boolean hasBeenWritten;

	protected short orderNr;

	protected JustificationItem replacingJustificationItem;


	// Private methods

	private boolean isSameJustificationType( JustificationItem referenceJustificationItem )
		{
		return ( referenceJustificationItem != null &&
				orderNr == referenceJustificationItem.orderNr &&
				justificationTypeNr_ == referenceJustificationItem.justificationTypeNr() );
		}


	// Constructor / deconstructor

	protected JustificationItem( short justificationTypeNr, short _orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

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

		replacingJustificationItem = null;
		}


	// Protected virtual methods

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( primarySpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : primarySpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : primarySpecificationItem_.itemNr() == queryItemNr ) ) ||

				( anotherPrimarySpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : anotherPrimarySpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : anotherPrimarySpecificationItem_.itemNr() == queryItemNr ) ) ||

				( secondarySpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : secondarySpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : secondarySpecificationItem_.itemNr() == queryItemNr ) ) ||

				( anotherSecondarySpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : anotherSecondarySpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : anotherSecondarySpecificationItem_.itemNr() == queryItemNr ) ) ||

				( attachedJustificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : attachedJustificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : attachedJustificationItem_.itemNr() == queryItemNr ) ) ||

				( replacingJustificationItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : replacingJustificationItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : replacingJustificationItem.itemNr() == queryItemNr ) ) );
		}

	protected byte checkForUsage()
		{
		return myWordItem().checkJustificationForUsageInWord( this );
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		baseToStringBuffer( queryWordTypeNr );

		switch( justificationTypeNr_ )
			{
			case Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isGeneralizationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOppositePossessiveConditionalSpecificationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_BACK_FIRED_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isBackFiredPossessiveConditionalSpecificationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecificationSubstitutionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isFeminineOrMasculineProperNameEndingAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isIndirectlyAnsweredQuestionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSuggestiveQuestionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOnlyOptionLeftAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessiveReversibleAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isDefinitionPartOfAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegativeAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralizationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionPartOfAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueRelationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOnlyOptionLeftConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessiveReversibleConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isDefinitionPartOfConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegativeConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralizationConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionPartOfConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueRelationConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionQuestion" );
				break;

			default:
				CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "justificationType:" + justificationTypeNr_ );
			}

		CommonVariables.queryStringBuffer.append( Constants.EMPTY_STRING + Constants.QUERY_WORD_TYPE_CHAR + justificationTypeNr_ );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "orderNr:" + orderNr );

		if( primarySpecificationItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "primarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + primarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + primarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		if( anotherPrimarySpecificationItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "anotherPrimarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + anotherPrimarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + anotherPrimarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		if( secondarySpecificationItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "secondarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + secondarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + secondarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		if( anotherSecondarySpecificationItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "anotherSecondarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + anotherSecondarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + anotherSecondarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		if( attachedJustificationItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "attachedJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + attachedJustificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + attachedJustificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		if( replacingJustificationItem != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "replacingJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + replacingJustificationItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + replacingJustificationItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

	protected boolean hasAttachedJustification()
		{
		return ( attachedJustificationItem_ != null );
		}

	protected boolean hasNewJustification()
		{
		JustificationItem searchItem = this;

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				return true;

			searchItem = searchItem.attachedJustificationItem_;
			}

		return false;
/*
		// Recursive alternative:
		return ( hasCurrentCreationSentenceNr() ||

				( attachedJustificationItem_ != null &&
				attachedJustificationItem_.hasNewJustification() ) );
*/		}

	protected boolean hasFoundJustification( JustificationItem existingJustificationItem )
		{
		JustificationItem searchItem = this;

		while( searchItem != null )
			{
			if( searchItem == existingJustificationItem )
				return true;

			searchItem = searchItem.attachedJustificationItem_;
			}

		return false;
		}

	protected boolean hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition()
		{
		JustificationItem searchItem = this;

		while( searchItem != null )
			{
			if( searchItem.hasPrimarySpecification() ||
			!searchItem.isExclusiveSpecificationSubstitutionAssumption() )
				return false;

			searchItem = searchItem.attachedJustificationItem_;
			}

		return true;
		}

	protected boolean hasPrimarySpecification()
		{
		return ( primarySpecificationItem_ != null );
		}

	protected boolean hasPrimaryQuestion()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isQuestion() );
		}

	protected boolean hasPrimarySpecificationWithRelationContext()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.hasRelationContext() );
		}

	protected boolean hasPrimaryUserSpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isUserSpecification() );
		}

	protected boolean hasHiddenPrimarySpecification()
		{
		WordItem primarySpecificationWordItem;

		if( primarySpecificationItem_ != null &&
		primarySpecificationItem_.isPossessive() &&
		primarySpecificationItem_.hasSpecificationCollection() &&
		( primarySpecificationWordItem = primarySpecificationItem_.specificationWordItem() ) != null )
			{
			if( primarySpecificationWordItem.isCollectedWithItself() ||
			primarySpecificationItem_.collectedWithItselfCommonWordItem() != null )
				return true;
			}

		return false;
		}

	protected boolean hasReplacedPrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isReplacedItem() );
		}

	protected boolean hasUpdatedPrimarySpecificationWordCollectedWithItself()
		{
		WordItem updatedPrimarySpecificationWordItem;

		if( primarySpecificationItem_ != null &&
		( updatedPrimarySpecificationWordItem = primarySpecificationItem_.updatedSpecificationItem().specificationWordItem() ) != null )
			return updatedPrimarySpecificationWordItem.isCollectedWithItself();

		return false;
		}

	protected boolean hasSecondaryAssignment()
		{
		return ( secondarySpecificationItem_ != null &&
				secondarySpecificationItem_.isAssignment() );
		}

	protected boolean hasSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != null );
		}

	protected boolean hasSecondarySpecificationWithMultipleRelationContext()
		{
		return ( secondarySpecificationItem_ != null &&
				secondarySpecificationItem_.hasRelationContext() &&
				myWordItem().nContextWordsInAllWords( secondarySpecificationItem_.relationContextNr(), secondarySpecificationItem_.specificationWordItem() ) > 1 );
		}

	protected boolean hasSecondaryUserSpecification()
		{
		return ( secondarySpecificationItem_ != null &&
				secondarySpecificationItem_.isUserSpecification() );
		}

	protected boolean isAssumptionJustification()
		{
		return isAssumption( justificationTypeNr_ );
		}

	protected boolean isConclusionJustification()
		{
		return isConclusion( justificationTypeNr_ );
		}

	protected boolean isBackFiredPossessiveConditionalSpecificationAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_BACK_FIRED_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );
		}

	protected boolean isExclusiveSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	protected boolean isFeminineOrMasculineProperNameEndingAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION );
		}

	protected boolean isGeneralizationAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION );
		}

	protected boolean isNegativeAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
				justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION );
		}

	protected boolean isOppositePossessiveConditionalSpecificationAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION );
		}

	protected boolean isSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	protected boolean isSpecificationSubstitutionPartOfAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION );
		}

	protected boolean isSuggestiveQuestionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION );
		}

	protected boolean isPossessiveReversibleConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION );
		}

	protected boolean isQuestionJustification()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION );
		}

	protected short justificationAssumptionGrade()
		{
		return assumptionGrade( justificationTypeNr_ );
		}

	protected short justificationTypeNr()
		{
		return justificationTypeNr_;
		}

	protected int nJustificationContextRelations( int relationContextNr, int nSpecificationRelationWords )
		{
		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			if( secondarySpecificationItem_ != null &&
			secondarySpecificationItem_.hasRelationContext() &&
			myWordItem().isContextSimilarInAllWords( secondarySpecificationItem_.relationContextNr(), relationContextNr ) )
				return nSpecificationRelationWords;

			return 1;
			}

		return 0;
		}

	protected byte attachJustification( JustificationItem attachedJustificationItem, SpecificationItem mySpecificationItem )
		{
		boolean isMySpecification = false;
		JustificationItem searchItem;

		if( attachedJustificationItem != null )
			{
			if( attachedJustificationItem != this )
				{
				if( mySpecificationItem != null )
					{
					if( hasCurrentCreationSentenceNr() )
						{
						if( attachedJustificationItem.isActiveItem() )
							{
							if( attachedJustificationItem_ == null )
								{
								if( ( searchItem = mySpecificationItem.firstJustificationItem() ) != null )
									{
									while( searchItem != null )
										{
										if( searchItem == this )
											isMySpecification = true;

										if( searchItem == attachedJustificationItem )
											return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item is already one of the attached justification items of my specification item" );

										searchItem = searchItem.attachedJustificationItem_;
										}

									if( isMySpecification )
										attachedJustificationItem_ = attachedJustificationItem;		// Add attached justification item
									else
										return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given my specification item isn't my specification item" );
									}
								else
									return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given my specification item has no justification item" );
								}
							else
								return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I already have an attached justification item" );
							}
						else
							return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item isn't active" );
						}
					else
						return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
					}
				else
					return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given my specification item is undefined" );
				}
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item is the same justification item as me" );
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte changeAttachedJustification( JustificationItem newAttachedJustificationItem )
		{
		attachedJustificationItem_ = null;

		if( newAttachedJustificationItem == null ||
		newAttachedJustificationItem.isActiveItem() )
			{
			if( newAttachedJustificationItem != this )
				{
				if( hasCurrentCreationSentenceNr() )
					{
					if( newAttachedJustificationItem == null ||

					( !hasFoundJustification( newAttachedJustificationItem ) &&
					!newAttachedJustificationItem.hasFoundJustification( this ) ) )
						attachedJustificationItem_ = newAttachedJustificationItem;
					}
				else
					return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given new attached justification item is the same justification item as me" );
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given new attached justification item isn't active" );

		return Constants.RESULT_OK;
		}

	protected byte changePrimarySpecification( SpecificationItem replacingSpecificationItem )
		{
		if( replacingSpecificationItem != null )
			{
			if( replacingSpecificationItem.isActiveItem() )
				{
				if( hasCurrentCreationSentenceNr() )
					primarySpecificationItem_ = replacingSpecificationItem;
				else
					return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given replacing specification item isn't active" );
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given replacing specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte changeSecondarySpecification( SpecificationItem replacingSpecificationItem )
		{
		if( replacingSpecificationItem != null )
			{
			if( replacingSpecificationItem.isActiveItem() )
				{
				if( hasCurrentCreationSentenceNr() )
					secondarySpecificationItem_ = replacingSpecificationItem;
				else
					return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "It isn't allowed to change an older item afterwards" );
				}
			else
				return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given replacing specification item isn't active" );
			}
		else
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "The given replacing specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkForDeletedSpecifications()
		{
		if( primarySpecificationItem_ != null &&
		primarySpecificationItem_.isDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My primary specification is a deleted item" );

		if( anotherPrimarySpecificationItem_ != null &&
		anotherPrimarySpecificationItem_.isDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My another primary specification is a deleted item" );

		if( secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My secondary specification is a deleted item" );

		if( anotherSecondarySpecificationItem_ != null &&
		anotherSecondarySpecificationItem_.isDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My another secondary specification is a deleted item" );

		return Constants.RESULT_OK;
		}

	protected byte checkForReplacedOrDeletedSpecifications()
		{
		if( primarySpecificationItem_ != null &&
		primarySpecificationItem_.isReplacedOrDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My primary specification is a replaced or a deleted item" );

		if( anotherPrimarySpecificationItem_ != null &&
		anotherPrimarySpecificationItem_.isReplacedOrDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My another primary specification is a replaced or a deleted item" );

		if( secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isReplacedOrDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My secondary specification is a replaced or a deleted item" );

		if( anotherSecondarySpecificationItem_ != null &&
		anotherSecondarySpecificationItem_.isReplacedOrDeletedItem() )
			return startErrorInItem( 1, null, myWordItem().anyWordTypeString(), "My another secondary specification is a replaced or a deleted item" );

		return Constants.RESULT_OK;
		}

	protected SpecificationResultType getCombinedAssumptionLevel()
		{
		SpecificationResultType specificationResult = new SpecificationResultType();
		int combinedAssumptionLevel = Constants.NO_ASSUMPTION_LEVEL;

		if( primarySpecificationItem_ != null &&
		( specificationResult = primarySpecificationItem_.getAssumptionLevel() ).result == Constants.RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( anotherPrimarySpecificationItem_ != null &&
		( specificationResult = anotherPrimarySpecificationItem_.getAssumptionLevel() ).result == Constants.RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( CommonVariables.result == Constants.RESULT_OK &&
		secondarySpecificationItem_ != null &&
		( specificationResult = secondarySpecificationItem_.getAssumptionLevel() ).result == Constants.RESULT_OK )
			combinedAssumptionLevel += specificationResult.assumptionLevel;

		if( combinedAssumptionLevel < Constants.MAX_LEVEL )
			specificationResult.combinedAssumptionLevel = (short)combinedAssumptionLevel;
		else
			specificationResult.result = startSystemErrorInItem( 1, null, myWordItem().anyWordTypeString(), "Assumption level overflow" );

		return specificationResult;
		}

	protected JustificationItem attachedJustificationItem()
		{
		return attachedJustificationItem_;
		}

	protected JustificationItem attachedPredecessorOfOldJustificationItem( JustificationItem oldJustificationItem )
		{
		JustificationItem searchItem = this;

		while( searchItem != null )
			{
			if( searchItem.attachedJustificationItem_ == oldJustificationItem )
				return searchItem;

			searchItem = searchItem.attachedJustificationItem_;
			}

		return null;
/*
		// Recursive alternative:
		return ( attachedJustificationItem_ == null ? null : attachedJustificationItem_ == oldJustificationItem ? this : attachedJustificationItem_.attachedPredecessorOfOldJustificationItem( oldJustificationItem ) );
*/		}

	protected JustificationItem nextJustificationItem()
		{
		return (JustificationItem)nextItem;
		}

	protected JustificationItem nextJustificationItemWithSameTypeAndOrderNr()
		{
		JustificationItem searchItem = attachedJustificationItem_;

		while( searchItem != null )
			{
			if( isSameJustificationType( searchItem ) )
				return searchItem;

			searchItem = searchItem.attachedJustificationItem_;
			}

		return null;
		}

	protected JustificationItem nextJustificationItemWithDifferentTypeOrOrderNr( JustificationItem firstJustificationItem )
		{
		JustificationItem usedTypeJustificationItem;
		JustificationItem nextTypeJustificationItem = attachedJustificationItem_;

		if( firstJustificationItem != null )
			{
			do	{
				// Find next occurrence with different type
				while( nextTypeJustificationItem != null &&
				isSameJustificationType( nextTypeJustificationItem ) )
					nextTypeJustificationItem = nextTypeJustificationItem.attachedJustificationItem_;

				if( nextTypeJustificationItem != null )
					{
					// Check if different type is already used
					usedTypeJustificationItem = firstJustificationItem;

					while( usedTypeJustificationItem != null &&
					!usedTypeJustificationItem.isSameJustificationType( nextTypeJustificationItem ) )
						usedTypeJustificationItem = usedTypeJustificationItem.attachedJustificationItem_;

					if( usedTypeJustificationItem == nextTypeJustificationItem )
						return nextTypeJustificationItem;
					}
				}
			while( nextTypeJustificationItem != null &&
			( nextTypeJustificationItem = nextTypeJustificationItem.attachedJustificationItem_ ) != null );
			}

		return null;
		}

	protected JustificationItem primarySpecificationWithoutRelationContextJustificationItem( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		if( primarySpecificationItem != null )
			{
			if( primarySpecificationItem_ != null &&
			!primarySpecificationItem_.hasRelationContext() &&
			secondarySpecificationItem_ == secondarySpecificationItem &&
			primarySpecificationItem_.specificationWordItem() == primarySpecificationItem.specificationWordItem() )
				return this;

			if( attachedJustificationItem_ != null )
				return attachedJustificationItem_.primarySpecificationWithoutRelationContextJustificationItem( primarySpecificationItem, secondarySpecificationItem );
			}

		return null;
		}

	protected JustificationItem foundSecondarySpecificationQuestion()
		{
		if( secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isQuestion() )
			return this;

		if( attachedJustificationItem_ != null )
			return attachedJustificationItem_.foundSecondarySpecificationQuestion();

		return null;
		}

	protected JustificationItem selfGeneratedSecondarySpecificationJustificationItem( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		if( secondarySpecificationItem != null &&
		secondarySpecificationItem_ != null )
			{
			if( primarySpecificationItem_ == primarySpecificationItem &&
			secondarySpecificationItem_.isSelfGenerated() &&
			secondarySpecificationItem_.specificationWordItem() == secondarySpecificationItem.specificationWordItem() )
				return this;

			if( attachedJustificationItem_ != null )
				return attachedJustificationItem_.selfGeneratedSecondarySpecificationJustificationItem( primarySpecificationItem, secondarySpecificationItem );
			}

		return null;
		}

	protected JustificationItem updatedJustificationItem()
		{
		JustificationItem updatedJustificationItem;
		JustificationItem searchItem = this;

		while( ( updatedJustificationItem = searchItem.replacingJustificationItem ) != null )
			searchItem = updatedJustificationItem;

		return searchItem;
		}

	protected SpecificationItem primarySpecificationItem()
		{
		return primarySpecificationItem_;
		}

	protected SpecificationItem anotherPrimarySpecificationItem()
		{
		return anotherPrimarySpecificationItem_;
		}

	protected SpecificationItem secondarySpecificationItem()
		{
		return secondarySpecificationItem_;
		}

	protected SpecificationItem anotherSecondarySpecificationItem()
		{
		return anotherSecondarySpecificationItem_;
		}

	protected WordItem primarySpecificationWordItem()
		{
		if( primarySpecificationItem_ != null )
			return primarySpecificationItem_.specificationWordItem();

		return null;
		}

	protected WordItem secondarySpecificationWordItem()
		{
		if( secondarySpecificationItem_ != null )
			return secondarySpecificationItem_.specificationWordItem();

		return null;
		}
	};

/*************************************************************************
 *
 *	"The voice of the Lord is powerful;
 *	the voice of the Lord is majestic." (Psalm 29:4)
 *
 *************************************************************************/
