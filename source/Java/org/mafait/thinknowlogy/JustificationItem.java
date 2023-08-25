/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		Storing info need to write the justification reports
 *					for the self-generated knowledge
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

package org.mafait.thinknowlogy;

class JustificationItem extends Item
	{
	// Private initialized variables

	private boolean hasFeminineOrMasculineProperNounEnding_ = false;

	private short justificationTypeNr_;

	private SpecificationItem primarySpecificationItem_ = null;
	private SpecificationItem additionalDefinitionSpecificationItem_ = null;
	private SpecificationItem secondarySpecificationItem_ = null;
	private SpecificationItem additionalProperNounSpecificationItem_ = null;

	private JustificationItem attachedJustificationItem_ = null;


	// Protected constructed variables

	protected boolean hasJustificationBeenWritten = false;

	protected short justificationOrderNr = Constants.NO_ORDER_NR;


	// Private methods

	private boolean hasJustification( JustificationItem existingJustificationItem )
		{
		JustificationItem searchJustificationItem = this;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem == existingJustificationItem )
				return true;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem_;
			}

		return false;
		}

	private static boolean isContextSimilarInContextWords( int firstContextNr, int secondContextNr )
		{
		WordItem currentContextWordItem;

		if( firstContextNr > Constants.NO_CONTEXT_NR &&
		secondContextNr > Constants.NO_CONTEXT_NR &&
		firstContextNr != secondContextNr &&
		( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( !currentContextWordItem.isContextSimilarInWord( firstContextNr, secondContextNr ) )
					return false;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return true;
		}

	private boolean isSameJustificationType( JustificationItem referenceJustificationItem )
		{
		return ( referenceJustificationItem != null &&
				referenceJustificationItem.justificationOrderNr == justificationOrderNr &&
				referenceJustificationItem.justificationTypeNr() == justificationTypeNr_ );
		}


	// Constructor

	protected JustificationItem( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, short _justificationOrderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem additionalProperNounSpecificationItem, JustificationItem attachedJustificationItem, List myList, WordItem myWordItem )
		{
		// Used for developer statistics
		GlobalVariables.nCreatedJustificationItems++;

		initializeItemVariables( originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

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


	// Protected virtual methods

	@Override
	protected void checkForUsage()
		{
		myWordItem().checkJustificationForUsageInWord( this );
		}

	@Override
	protected void selectingJustificationSpecifications()
		{
		if( primarySpecificationItem_ != null )
			primarySpecificationItem_.isSelectedByJustificationQuery = true;

		if( additionalDefinitionSpecificationItem_ != null )
			additionalDefinitionSpecificationItem_.isSelectedByJustificationQuery = true;

		if( secondarySpecificationItem_ != null )
			secondarySpecificationItem_.isSelectedByJustificationQuery = true;

		if( additionalProperNounSpecificationItem_ != null )
			additionalProperNounSpecificationItem_.isSelectedByJustificationQuery = true;
		}

	@Override
	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( justificationTypeNr_ == queryWordTypeNr );
		}

	@Override
	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( primarySpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : primarySpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : primarySpecificationItem_.itemNr() == queryItemNr ) ) ||

				( additionalDefinitionSpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : additionalDefinitionSpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : additionalDefinitionSpecificationItem_.itemNr() == queryItemNr ) ) ||

				( secondarySpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : secondarySpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : secondarySpecificationItem_.itemNr() == queryItemNr ) ) ||

				( additionalProperNounSpecificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : additionalProperNounSpecificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : additionalProperNounSpecificationItem_.itemNr() == queryItemNr ) ) ||

				( attachedJustificationItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : attachedJustificationItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : attachedJustificationItem_.itemNr() == queryItemNr ) ) );
		}

	@Override
	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( hasFeminineOrMasculineProperNounEnding_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasFeminineOrMasculineProperNounEnding" );

		switch( justificationTypeNr_ )
			{
			case Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isGeneralizationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOppositePossessiveSpecificationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecificationSubstitutionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isIndirectlyAnsweredQuestionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSuggestiveQuestionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOnlyOptionLeftAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isReversibleAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isDefinitionPartOfAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegativeAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralizationAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionPartOfAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION_OR_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueUserRelationAssumptionOrConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionQuestion" );
				break;

			default:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "justificationType:" + justificationTypeNr_ );
			}

		queryStringBuffer.append( Constants.EMPTY_STRING + Constants.QUERY_WORD_TYPE_CHAR + justificationTypeNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "justificationOrderNr:" + justificationOrderNr );

		if( primarySpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "primarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + primarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + primarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( additionalDefinitionSpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "additionalDefinitionSpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + additionalDefinitionSpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + additionalDefinitionSpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( secondarySpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "secondarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + secondarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + secondarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( additionalProperNounSpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "additionalProperNounSpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + additionalProperNounSpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + additionalProperNounSpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( attachedJustificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "attachedJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + attachedJustificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + attachedJustificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return queryStringBuffer;
		}


	// Protected methods

	protected void clearReplacingInfo()
		{
		if( hasCurrentReplacedSentenceNr() )
			clearReplacedSentenceNr();
		}

	protected boolean hasAdditionalDefinitionSpecification()
		{
		return ( additionalDefinitionSpecificationItem_ != null );
		}

	protected boolean hasAssumptionLevel()
		{
		return ( justificationAssumptionLevel() > Constants.NO_ASSUMPTION_LEVEL );
		}

	protected boolean hasFeminineOrMasculineProperNounEnding()
		{
		return hasFeminineOrMasculineProperNounEnding_;
		}

	protected boolean hasJustification( boolean hasFeminineOrMasculineProperNounEnding, short justificationTypeNr, SpecificationItem primarySpecificationItem, SpecificationItem additionalDefinitionSpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		return ( justificationTypeNr_ == justificationTypeNr &&
				hasFeminineOrMasculineProperNounEnding_ == hasFeminineOrMasculineProperNounEnding &&
				primarySpecificationItem_ == primarySpecificationItem &&
				additionalDefinitionSpecificationItem_ == additionalDefinitionSpecificationItem &&
				secondarySpecificationItem_ == secondarySpecificationItem );
//				additionalProperNounSpecificationItem_ == additionalProperNounSpecificationItem );
		}

	protected boolean hasPrimarySpecificationRelationContext()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.hasRelationContext() );
		}

	protected boolean isExclusivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isExclusiveSpecification() );
		}

	protected boolean isNegativePrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isNegative() );
		}

	protected boolean isPossessivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isPossessive() );
		}

	protected boolean isPrimaryAssignmentSpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isAssignment() );
		}

	protected boolean isPrimaryNounSpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isSpecificationNoun() );
		}

	protected boolean isPrimaryQuestionSpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isQuestion() );
		}

	protected boolean isPrimarySpecificationWordSpanishAmbiguous()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isSpecificationWordSpanishAmbiguous() );
		}

	protected boolean isReplacedPrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isReplacedItem() );
		}

	protected boolean isReplacedSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != null &&
				secondarySpecificationItem_.isReplacedItem() );
		}

	protected boolean hasCompoundCollectionSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != null &&
				secondarySpecificationItem_.hasCompoundSpecificationCollection() );
		}

	protected boolean isPossessiveSecondarySpecification()
		{
		return ( secondarySpecificationItem_ != null &&
				secondarySpecificationItem_.isPossessive() );
		}

	protected boolean isExclusiveSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	protected boolean isNegativeAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION );
		}

	protected boolean isObsoleteAssumptionJustification( boolean isIncludingNegativeAssumptionOrConclusion, boolean isIncludingReversibleAssumptionOrConclusion, boolean isIncludingSpecificationSubstitutionAssumptionOrConclusion, boolean isOnlySelectingOlderJustifications, short assumptionLevel )
		{
		return 	( primarySpecificationItem_ != null &&

				( !isOnlySelectingOlderJustifications ||
				isOlderItem() ) &&

				( justificationAssumptionLevel() > assumptionLevel &&

				( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ||
				justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||

				( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION &&
				isIncludingReversibleAssumptionOrConclusion ) ||

				( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION &&
				isIncludingNegativeAssumptionOrConclusion &&
				!primarySpecificationItem_.isSpecificationAdjective() ) ||

				( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION &&
				isIncludingSpecificationSubstitutionAssumptionOrConclusion ) ||

				justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION ) ) );
		}

	protected boolean isOppositePossessiveSpecificationAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION );
		}

	protected boolean isQuestionJustification()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION );
		}

	protected boolean isReversibleAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION );
		}

	protected boolean isReversibleConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION &&
				!hasAssumptionLevel() );
		}

	protected boolean isSpecificationSubstitutionAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION );
		}

	protected boolean isSpecificationSubstitutionPartOfAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION );
		}

	protected boolean isSuggestiveQuestionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION );
		}

	protected short justificationTypeNr()
		{
		return justificationTypeNr_;
		}

	protected short primarySpecificationWordTypeNr()
		{
		return ( primarySpecificationItem_ == null ? Constants.NO_WORD_TYPE_NR : primarySpecificationItem_.specificationWordTypeNr() );
		}

	protected int justificationAssumptionLevel()
		{
		return assumptionLevel( hasFeminineOrMasculineProperNounEnding_, justificationTypeNr_, primarySpecificationItem_, additionalDefinitionSpecificationItem_, secondarySpecificationItem_, additionalProperNounSpecificationItem_ );
		}

	protected int nJustificationContextRelations( int relationContextNr, int nRelationWords )
		{
		int primaryRelationContextNr;
		int secondaryRelationContextNr;

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			if( primarySpecificationItem_ != null &&
			( primaryRelationContextNr = primarySpecificationItem_.relationContextNr() ) > Constants.NO_CONTEXT_NR )

				return ( isReversibleAssumptionOrConclusion() ? 1 : myWordItem().nContextWords( primaryRelationContextNr ) );

			if( secondarySpecificationItem_ != null &&
			( secondaryRelationContextNr = secondarySpecificationItem_.relationContextNr() ) > Constants.NO_CONTEXT_NR )
				{
				if( additionalDefinitionSpecificationItem_ != null )
					return myWordItem().nContextWords( secondaryRelationContextNr );

				if( isContextSimilarInContextWords( secondaryRelationContextNr, relationContextNr ) )
					return nRelationWords;
				}

			return 1;
			}

		return 0;
		}

	protected int primarySpecificationCollectionNr()
		{
		return ( primarySpecificationItem_ == null ? Constants.NO_COLLECTION_NR : primarySpecificationItem_.specificationCollectionNr() );
		}

	protected int secondarySpecificationCollectionNr()
		{
		return ( secondarySpecificationItem_ == null ? Constants.NO_COLLECTION_NR : secondarySpecificationItem_.specificationCollectionNr() );
		}

	protected byte attachJustification( JustificationItem attachedJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		if( attachedJustificationItem == null )
			return startError( 1, null, "The given attached justification item is undefined" );

		if( attachedJustificationItem == this )
			return startError( 1, null, "The given attached justification item is the same justification item as me" );

		if( involvedSpecificationItem == null )
			return startError( 1, null, "The given involved specification item is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( !attachedJustificationItem.isActiveItem() )
			return startError( 1, null, "The given attached justification item isn't active" );

		if( attachedJustificationItem_ != null )
			return startError( 1, null, "I already have an attached justification item" );

		if( involvedSpecificationItem.checkJustification( attachedJustificationItem ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to check the given attached justification item against the given involved specification item" );

		// Add attached justification item
		attachedJustificationItem_ = attachedJustificationItem;

		return Constants.RESULT_OK;
		}

	protected byte changeAttachedJustification( JustificationItem newAttachedJustificationItem )
		{
		attachedJustificationItem_ = null;

		if( newAttachedJustificationItem == this )
			return startError( 1, null, "The given new attached justification item is the same as me" );

		if( newAttachedJustificationItem != null )
			{
			if( !newAttachedJustificationItem.isActiveItem() )
				return startError( 1, null, "The given new attached justification item isn't active" );

			if( newAttachedJustificationItem.hasJustification( this ) )
				return startError( 1, null, "I am already linked to the given new attached justification item" );
			}

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		attachedJustificationItem_ = newAttachedJustificationItem;
		return Constants.RESULT_OK;
		}

	protected byte checkSpecifications( boolean isIncludingReplacedSpecifications )
		{
		if( primarySpecificationItem_ != null )
			{
			if( primarySpecificationItem_.isReplacedItem() )
				{
				if( isIncludingReplacedSpecifications &&
				myWordItem().replaceOrDeleteJustification( this ) != Constants.RESULT_OK )
					return startError( 1, null, "I failed to replace or delete an unreferenced justification item with a replaced primary specification" );
				}
			else
				{
				if( primarySpecificationItem_.isDeletedItem() &&
				InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted primary specification item:\n\tSpecificationItem: " + primarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
					return startError( 1, null, "I failed to write an interface warning" );
				}
			}

		if( additionalDefinitionSpecificationItem_ != null &&

		( additionalDefinitionSpecificationItem_.isDeletedItem() ||

		( isIncludingReplacedSpecifications &&
		additionalDefinitionSpecificationItem_.isReplacedItem() ) ) &&

		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted additional definition specification item:\n\tSpecificationItem: " + additionalDefinitionSpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		if( secondarySpecificationItem_ != null )
			{
			if( secondarySpecificationItem_.isReplacedItem() )
				{
				if( isIncludingReplacedSpecifications &&
				myWordItem().replaceOrDeleteJustification( this ) != Constants.RESULT_OK )
					return startError( 1, null, "I failed to replace or delete an unreferenced justification item with a replaced secondary specification" );
				}
			else
				{
				if( secondarySpecificationItem_.isDeletedItem() )
					{
					if( InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted secondary specification item:\n\tSpecificationItem: " + secondarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
						return startError( 1, null, "I failed to write an interface warning" );
					}
				}
			}

		if( additionalProperNounSpecificationItem_ != null &&

		( additionalProperNounSpecificationItem_.isDeletedItem() ||

		( isIncludingReplacedSpecifications &&
		additionalProperNounSpecificationItem_.isReplacedItem() ) ) &&

		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted additional proper noun specification item:\n\tSpecificationItem: " + additionalProperNounSpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	protected byte replaceSpecification( boolean isReplacingPrimarySpecification, boolean isReplacingSecondarySpecification, SpecificationItem replacingSpecificationItem )
		{
		if( replacingSpecificationItem == null )
			return startError( 1, null, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, null, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( isReplacingPrimarySpecification )
			primarySpecificationItem_ = replacingSpecificationItem;

//		if( isReplacingAdditionalDefinitionSpecification )
//			additionalDefinitionSpecificationItem_ = replacingSpecificationItem;

		if( isReplacingSecondarySpecification )
			secondarySpecificationItem_ = replacingSpecificationItem;

//		if( isReplacingAdditionalProperNounSpecification )
//			additionalProperNounSpecificationItem_ = replacingSpecificationItem;

		return Constants.RESULT_OK;
		}

	protected JustificationItem attachedJustificationItem()
		{
		return attachedJustificationItem_;
		}

	protected JustificationItem attachedPredecessorOfObsoleteJustificationItem( JustificationItem obsoleteJustificationItem )
		{
		JustificationItem searchJustificationItem = this;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.attachedJustificationItem_ == obsoleteJustificationItem )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem_;
			}

		return null;
		}

	protected JustificationItem nextJustificationItem()
		{
		return (JustificationItem)nextItem;
		}

	protected JustificationItem nextJustificationItemWithSameTypeAndOrderNr()
		{
		JustificationItem searchJustificationItem = attachedJustificationItem_;

		while( searchJustificationItem != null )
			{
			if( isSameJustificationType( searchJustificationItem ) )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem_;
			}

		return null;
		}

	protected JustificationItem nextJustificationItemWithDifferentTypeOrOrderNr( JustificationItem firstJustificationItem )
		{
		JustificationItem nextTypeJustificationItem = attachedJustificationItem_;
		JustificationItem usedTypeJustificationItem;

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

	protected JustificationItem obsoleteSpanishJustificationItem( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		if( ( primarySpecificationItem_ != null &&
		primarySpecificationItem_.isSelfGeneratedQuestion() ) ||

		( primarySpecificationItem_ == primarySpecificationItem &&
		secondarySpecificationItem != null &&
		secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isSelfGeneratedSpecification() &&
		secondarySpecificationItem_.generalizationWordItem() == secondarySpecificationItem.generalizationWordItem() ) )
			return this;

		// Recursive, do for all attached justification items
		if( attachedJustificationItem_ != null )
			return attachedJustificationItem_.obsoleteSpanishJustificationItem( primarySpecificationItem, secondarySpecificationItem );

		return null;
		}

	protected JustificationItem primarySpecificationWithoutRelationContextJustificationItem( WordItem primarySpecificationWordItem )
		{
		if( primarySpecificationWordItem != null )
			{
			if( primarySpecificationItem_ != null &&

			( primarySpecificationItem_.isReplacedOrDeletedItem() ||

			( !primarySpecificationItem_.hasRelationContext() &&
			primarySpecificationItem_.specificationWordItem() == primarySpecificationWordItem ) ) )
				return this;

			// Recursive, do for all attached justification items
			if( attachedJustificationItem_ != null )
				return attachedJustificationItem_.primarySpecificationWithoutRelationContextJustificationItem( primarySpecificationWordItem );
			}

		return null;
		}

	protected SpecificationItem additionalDefinitionSpecificationItem()
		{
		return additionalDefinitionSpecificationItem_;
		}

		protected SpecificationItem additionalProperNounSpecificationItem()
	{
		return additionalProperNounSpecificationItem_;
	}

	protected SpecificationItem primarySelfGeneratedSpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isSelfGeneratedSpecification() ? primarySpecificationItem_ : null );
		}

	protected SpecificationItem primarySpecificationItem()
		{
		return primarySpecificationItem_;
		}

	protected SpecificationItem secondarySpecificationItem()
		{
		return secondarySpecificationItem_;
		}

	protected SpecificationItem updatedPrimarySpecificationItem()
		{
		return ( primarySpecificationItem_ == null ? null : primarySpecificationItem_.updatedSpecificationItem() );
		}

	protected SpecificationItem updatedSecondarySpecificationItem()
		{
		return ( secondarySpecificationItem_ == null ? null : secondarySpecificationItem_.updatedSpecificationItem() );
		}

	protected WordItem generalizationWordItem()
		{
		return myWordItem();
		}

	protected WordItem primaryGeneralizationWordItem()
		{
		return ( primarySpecificationItem_ == null ? null : primarySpecificationItem_.generalizationWordItem() );
		}

	protected WordItem primarySpecificationWordItem()
		{
		return ( primarySpecificationItem_ == null ? null : primarySpecificationItem_.specificationWordItem() );
		}

	protected WordItem secondaryGeneralizationWordItem()
		{
		return ( secondarySpecificationItem_ == null ? null : secondarySpecificationItem_.generalizationWordItem() );
		}
	}

/*************************************************************************
 *	"The voice of the Lord is powerful;
 *	the voice of the Lord is majestic." (Psalm 29:4)
 *************************************************************************/
