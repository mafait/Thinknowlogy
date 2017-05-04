/*	Class:			JustificationItem
 *	Parent class:	Item
 *	Purpose:		To store info need to write the justification reports
 *					for the self-generated knowledge
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

class JustificationItem extends Item
	{
	// Private initialized variables

	boolean hasFeminineOrMasculineProperNameEnding_;

	private short justificationTypeNr_;

	private SpecificationItem primarySpecificationItem_;
	private SpecificationItem anotherPrimarySpecificationItem_;
	private SpecificationItem secondarySpecificationItem_;
	private SpecificationItem anotherSecondarySpecificationItem_;

	private JustificationItem attachedJustificationItem_;


	// Protected constructed variables

	protected boolean hasJustificationBeenWritten;

	protected short orderNr;

	protected JustificationItem replacingJustificationItem;


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
		( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
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
				orderNr == referenceJustificationItem.orderNr &&

				( justificationTypeNr_ == referenceJustificationItem.justificationTypeNr() ||

				( primarySpecificationItem_ == null &&
				referenceJustificationItem.primarySpecificationItem_ == null &&
				isPossessiveReversibleAssumptionOrConclusion() == referenceJustificationItem.isPossessiveReversibleAssumptionOrConclusion() ) ) );
		}


	// Constructor

	protected JustificationItem( boolean hasFeminineOrMasculineProperNameEnding, short justificationTypeNr, short _orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( originalSentenceNr, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

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

		replacingJustificationItem = null;
		}


	// Protected virtual methods

	protected void checkForUsage()
		{
		myWordItem().checkJustificationForUsageInWord( this );
		}

	protected void selectingJustificationSpecifications()
		{
		if( primarySpecificationItem_ != null )
			primarySpecificationItem_.isSelectedByJustificationQuery = true;

		if( anotherPrimarySpecificationItem_ != null )
			anotherPrimarySpecificationItem_.isSelectedByJustificationQuery = true;

		if( secondarySpecificationItem_ != null )
			secondarySpecificationItem_.isSelectedByJustificationQuery = true;

		if( anotherSecondarySpecificationItem_ != null )
			anotherSecondarySpecificationItem_.isSelectedByJustificationQuery = true;
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( justificationTypeNr_ == queryWordTypeNr );
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
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

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( hasFeminineOrMasculineProperNameEnding_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "hasFeminineOrMasculineProperNameEnding" );

		switch( justificationTypeNr_ )
			{
			case Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isGeneralizationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOppositePossessiveConditionalSpecificationAssumption" );
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

			case Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOnlyOptionLeftAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessiveReversibleAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isDefinitionPartOfAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegativeAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralizationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionPartOfAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueUserRelationAssumption" );
				break;

			case Constants.JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOnlyOptionLeftConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isPossessiveReversibleConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isDefinitionPartOfConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNegativeConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationGeneralizationConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionPartOfConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isUniqueUserRelationConclusion" );
				break;

			case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSpecificationSubstitutionQuestion" );
				break;

			default:
				queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "justificationType:" + justificationTypeNr_ );
			}

		queryStringBuffer.append( Constants.EMPTY_STRING + Constants.QUERY_WORD_TYPE_CHAR + justificationTypeNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "orderNr:" + orderNr );

		if( primarySpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "primarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + primarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + primarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( anotherPrimarySpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "anotherPrimarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + anotherPrimarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + anotherPrimarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( secondarySpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "secondarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + secondarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + secondarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( anotherSecondarySpecificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "anotherSecondarySpecificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + anotherSecondarySpecificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + anotherSecondarySpecificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( attachedJustificationItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "attachedJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + attachedJustificationItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + attachedJustificationItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( replacingJustificationItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "replacingJustificationItem" + Constants.QUERY_REF_ITEM_START_CHAR + replacingJustificationItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + replacingJustificationItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return queryStringBuffer;
		}


	// Protected methods

	protected void clearReplacingInfo()
		{
		if( hasCurrentReplacedSentenceNr() ||

		( replacingJustificationItem != null &&
		replacingJustificationItem.hasCurrentOrNewerCreationSentenceNr() ) )
			{
			clearReplacedSentenceNr();
			replacingJustificationItem = null;
			}
		}

	protected boolean hasAttachedJustification()
		{
		return ( attachedJustificationItem_ != null );
		}

	protected boolean hasFeminineOrMasculineProperNameEnding()
		{
		return hasFeminineOrMasculineProperNameEnding_;
		}

	protected boolean hasOnlyExclusiveSpecificationSubstitutionAssumptionsWithoutDefinition()
		{
		JustificationItem searchJustificationItem = this;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.primarySpecificationItem_ != null ||
			!searchJustificationItem.isExclusiveSpecificationSubstitutionAssumption() )
				return false;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem_;
			}

		return true;
		}

	protected boolean hasHiddenPrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isHiddenSpanishSpecification() );
		}

	protected boolean hasPrimaryAnsweredQuestion()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isAnsweredQuestion() );
		}

	protected boolean hasPrimaryQuestion()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isQuestion() );
		}

	protected boolean hasPrimaryUserSpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isUserSpecification() );
		}

	protected boolean hasPossessivePrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isPossessive() );
		}

	protected boolean hasReplacedPrimarySpecification()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isReplacedItem() );
		}

	protected boolean isPrimarySpecificationWordSpanishAmbiguous()
		{
		return ( primarySpecificationItem_ != null &&
				primarySpecificationItem_.isSpecificationWordSpanishAmbiguous() );
		}

	protected boolean isUpdatedPrimarySpecificationWordSpanishAmbiguous()
		{
		WordItem updatedPrimarySpecificationWordItem;

		if( primarySpecificationItem_ != null &&
		( updatedPrimarySpecificationWordItem = primarySpecificationItem_.updatedSpecificationItem().specificationWordItem() ) != null )
			return updatedPrimarySpecificationWordItem.isNounWordSpanishAmbiguous();

		return false;
		}

	protected boolean hasAnotherPrimarySpecification()
		{
		return ( anotherPrimarySpecificationItem_ != null );
		}

	protected boolean isAssumptionJustification()
		{
		return isAssumption( justificationTypeNr_ );
		}

	protected boolean isConclusionJustification()
		{
		return isConclusion( justificationTypeNr_ );
		}

	protected boolean isExclusiveSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
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

	protected boolean isPossessiveReversibleAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION );
		}

	protected boolean isPossessiveReversibleConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION );
		}

	protected boolean isPossessiveReversibleAssumptionOrConclusion()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||
				justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION );
		}

	protected boolean isQuestionJustification()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION );
		}

	protected boolean isSpecificationSubstitutionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION );
		}

	protected boolean isSuggestiveQuestionAssumption()
		{
		return ( justificationTypeNr_ == Constants.JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION );
		}

	protected short justificationAssumptionGrade()
		{
		boolean hasPossessivePrimarySpecification = false;
		boolean hasPrimaryQuestionSpecification = false;

		if( primarySpecificationItem_ != null )
			{
			if( primarySpecificationItem_.isGeneralizationProperName() &&
			primarySpecificationItem_.isPossessive() )
				hasPossessivePrimarySpecification = true;

			if( primarySpecificationItem_.isQuestion() )
				hasPrimaryQuestionSpecification = true;
			}

		return assumptionGrade( ( anotherPrimarySpecificationItem_ != null ), hasFeminineOrMasculineProperNameEnding_, hasPossessivePrimarySpecification, hasPrimaryQuestionSpecification, justificationTypeNr_ );
		}

	protected short justificationTypeNr()
		{
		return justificationTypeNr_;
		}

	protected short primarySpecificationAssumptionLevel()
		{
		if( primarySpecificationItem_ != null )
			return primarySpecificationItem_.assumptionLevel();

		return Constants.NO_ASSUMPTION_LEVEL;
		}

	protected int nJustificationContextRelations( int relationContextNr, int nSpecificationRelationWords )
		{
		int primaryRelationContextNr;
		int secondaryRelationContextNr;

		if( relationContextNr > Constants.NO_CONTEXT_NR )
			{
			if( primarySpecificationItem_ != null &&
			( primaryRelationContextNr = primarySpecificationItem_.relationContextNr() ) > Constants.NO_CONTEXT_NR )
				return myWordItem().nContextWordsInContextWords( primaryRelationContextNr, primarySpecificationItem_.specificationWordItem() );

			if( secondarySpecificationItem_ != null &&
			( secondaryRelationContextNr = secondarySpecificationItem_.relationContextNr() ) > Constants.NO_CONTEXT_NR )
				{
				if( anotherPrimarySpecificationItem_ != null )
					return myWordItem().nContextWordsInContextWords( secondaryRelationContextNr, secondarySpecificationItem_.specificationWordItem() );

				if( secondaryRelationContextNr == relationContextNr ||
				isContextSimilarInContextWords( secondaryRelationContextNr, relationContextNr ) )
					return nSpecificationRelationWords;
				}

			return 1;
			}

		return 0;
		}

	protected int primarySpecificationCollectionNr()
		{
		if( primarySpecificationItem_ != null )
			return primarySpecificationItem_.specificationCollectionNr();

		return Constants.NO_COLLECTION_NR;
		}

	protected byte attachJustification( JustificationItem attachedJustificationItem, SpecificationItem mySpecificationItem )
		{
		boolean isMySpecification = false;
		JustificationItem searchJustificationItem;

		if( attachedJustificationItem == null )
			return startError( 1, null, "The given attached justification item is undefined" );

		if( attachedJustificationItem == this )
			return startError( 1, null, "The given attached justification item is the same justification item as me" );

		if( mySpecificationItem == null )
			return startError( 1, null, "The given my specification item is undefined" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( !attachedJustificationItem.isActiveItem() )
			return startError( 1, null, "The given attached justification item isn't active" );

		if( attachedJustificationItem_ != null )
			return startError( 1, null, "I already have an attached justification item" );

		if( ( searchJustificationItem = mySpecificationItem.firstJustificationItem() ) == null )
			return startError( 1, null, "The given my specification item has no justification item" );

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem == this )
				isMySpecification = true;

			if( searchJustificationItem == attachedJustificationItem )
				return startError( 1, null, "The given attached justification item is already one of the attached justification items of my specification item" );

			searchJustificationItem = searchJustificationItem.attachedJustificationItem_;
			}

		if( !isMySpecification )
			return startError( 1, null, "The given my specification item isn't my specification item" );

		// Add attached justification item
		attachedJustificationItem_ = attachedJustificationItem;

		return Constants.RESULT_OK;
		}

	protected byte changeAttachedJustification( JustificationItem newAttachedJustificationItem )
		{
		attachedJustificationItem_ = null;

		if( newAttachedJustificationItem != null &&
		!newAttachedJustificationItem.isActiveItem() )
			return startError( 1, null, "The given new attached justification item isn't active" );

		if( newAttachedJustificationItem == this )
			return startError( 1, null, "The given new attached justification item is the same justification item as me" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		if( newAttachedJustificationItem == null ||

		( !hasJustification( newAttachedJustificationItem ) &&
		!newAttachedJustificationItem.hasJustification( this ) ) )
			attachedJustificationItem_ = newAttachedJustificationItem;

		return Constants.RESULT_OK;
		}

	protected byte changePrimarySpecification( SpecificationItem replacingSpecificationItem )
		{
		if( replacingSpecificationItem == null )
			return startError( 1, null, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, null, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		primarySpecificationItem_ = replacingSpecificationItem;

		return Constants.RESULT_OK;
		}

	protected byte changeAnotherPrimarySpecification( SpecificationItem replacingSpecificationItem )
		{
		if( replacingSpecificationItem == null )
			return startError( 1, null, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, null, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		anotherPrimarySpecificationItem_ = replacingSpecificationItem;

		return Constants.RESULT_OK;
		}

	protected byte changeSecondarySpecification( SpecificationItem replacingSpecificationItem )
		{
		if( replacingSpecificationItem == null )
			return startError( 1, null, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, null, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		secondarySpecificationItem_ = replacingSpecificationItem;

		return Constants.RESULT_OK;
		}

	protected byte changeAnotherSecondarySpecification( SpecificationItem replacingSpecificationItem )
		{
		if( replacingSpecificationItem == null )
			return startError( 1, null, "The given replacing specification item is undefined" );

		if( replacingSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, null, "The given replacing specification item is replaced or deleted" );

		if( !hasCurrentCreationSentenceNr() )
			return startError( 1, null, "It isn't allowed to change an older item afterwards" );

		anotherSecondarySpecificationItem_ = replacingSpecificationItem;

		return Constants.RESULT_OK;
		}

	protected byte checkForDeletedSpecifications()
		{
		if( primarySpecificationItem_ != null &&
		primarySpecificationItem_.isDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted primary specification item:\n\tSpecificationItem: " + primarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		if( anotherPrimarySpecificationItem_ != null &&
		anotherPrimarySpecificationItem_.isDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted another primary specification item:\n\tSpecificationItem: " + anotherPrimarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		if( secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted secondary specification item:\n\tSpecificationItem: " + secondarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		if( anotherSecondarySpecificationItem_ != null &&
		anotherSecondarySpecificationItem_.isDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a deleted another secondary specification item:\n\tSpecificationItem: " + anotherSecondarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	protected byte checkForReplacedOrDeletedSpecification()
		{
		if( primarySpecificationItem_ != null &&
		primarySpecificationItem_.isReplacedOrDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted primary specification item:\n\tSpecificationItem: " + primarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		if( anotherPrimarySpecificationItem_ != null &&
		anotherPrimarySpecificationItem_.isReplacedOrDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted another primary specification item:\n\tSpecificationItem: " + anotherPrimarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		if( secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isReplacedOrDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted secondary specification item:\n\tSpecificationItem: " + secondarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		if( anotherSecondarySpecificationItem_ != null &&
		anotherSecondarySpecificationItem_.isReplacedOrDeletedItem() &&
		InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a replaced or deleted another secondary specification item:\n\tSpecificationItem: " + anotherSecondarySpecificationItem_.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tJustificationItem: " + itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
			return startError( 1, null, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	protected JustificationItem attachedJustificationItem()
		{
		return attachedJustificationItem_;
		}

	protected JustificationItem attachedPredecessorOfOldJustificationItem( JustificationItem obsoleteJustificationItem )
		{
		JustificationItem searchJustificationItem = this;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.attachedJustificationItem_ == obsoleteJustificationItem )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem_;
			}

		return null;
/*
		// Recursive alternative
		return ( attachedJustificationItem_ == null ? null : attachedJustificationItem_ == obsoleteJustificationItem ? this : attachedJustificationItem_.attachedPredecessorOfOldJustificationItem( obsoleteJustificationItem ) );
*/		}

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

	protected JustificationItem obsoleteSpanishJustificationItem( SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		if( ( primarySpecificationItem_ != null &&
		primarySpecificationItem_.isSelfGeneratedQuestion() ) ||

		( primarySpecificationItem_ == primarySpecificationItem &&
		secondarySpecificationItem != null &&
		secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isSelfGenerated() &&
		secondarySpecificationItem_.generalizationWordItem() == secondarySpecificationItem.generalizationWordItem() &&
		secondarySpecificationItem_.specificationWordItem() == secondarySpecificationItem.specificationWordItem() ) )
			return this;

		// Recursive, do for all attached justification items
		if( attachedJustificationItem_ != null )
			return attachedJustificationItem_.obsoleteSpanishJustificationItem( primarySpecificationItem, secondarySpecificationItem );

		return null;
		}

	protected JustificationItem primarySpecificationWithoutRelationContextJustificationItem( SpecificationItem primarySpecificationItem )
		{
		if( primarySpecificationItem != null )
			{
			if( primarySpecificationItem_ != null &&

			( primarySpecificationItem_.isReplacedItem() ||

			( !primarySpecificationItem_.hasRelationContext() &&
			primarySpecificationItem_.specificationWordItem() == primarySpecificationItem.specificationWordItem() ) ) )
				return this;

			// Recursive, do for all attached justification items
			if( attachedJustificationItem_ != null )
				return attachedJustificationItem_.primarySpecificationWithoutRelationContextJustificationItem( primarySpecificationItem );
			}

		return null;
		}

	protected JustificationItem secondarySpecificationQuestion()
		{
		JustificationItem searchJustificationItem = this;
		SpecificationItem secondarySpecificationItem;

		while( searchJustificationItem != null )
			{
			if( ( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem_ ) != null &&
			secondarySpecificationItem.isQuestion() )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.attachedJustificationItem_;
			}
/*
		// Recursive alternative
		if( secondarySpecificationItem_ != null &&
		secondarySpecificationItem_.isQuestion() )
			return this;

		// Do for all attached justification items
		if( attachedJustificationItem_ != null )
			return attachedJustificationItem_.secondarySpecificationQuestion();
*/
		return null;
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

	protected WordItem generalizationWordItem()
		{
		return myWordItem();
		}

	protected WordItem primarySpecificationWordItem()
		{
		if( primarySpecificationItem_ != null )
			return primarySpecificationItem_.specificationWordItem();

		return null;
		}

	protected WordItem secondaryGeneralizationWordItem()
		{
		if( secondarySpecificationItem_ != null )
			return secondarySpecificationItem_.generalizationWordItem();

		return null;
		}

	protected WordItem secondarySpecificationWordItem()
		{
		if( secondarySpecificationItem_ != null )
			return secondarySpecificationItem_.specificationWordItem();

		return null;
		}

	protected ShortResultType getCombinedAssumptionLevel()
		{
		ShortResultType shortResult = new ShortResultType();
		int assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;

		if( primarySpecificationItem_ != null )
			assumptionLevel += primarySpecificationItem_.assumptionLevel();

		if( anotherPrimarySpecificationItem_ != null )
			assumptionLevel += anotherPrimarySpecificationItem_.assumptionLevel();

		if( secondarySpecificationItem_ != null )
			assumptionLevel += secondarySpecificationItem_.assumptionLevel();

		if( anotherSecondarySpecificationItem_ != null )
			assumptionLevel += anotherSecondarySpecificationItem_.assumptionLevel();

		if( assumptionLevel < Constants.MAX_LEVEL )
			shortResult.shortValue = (short)assumptionLevel;
		else
			shortResult.result = startSystemError( 1, null, "Assumption level overflow" );

		return shortResult;
		}
	};

/*************************************************************************
 *	"The voice of the Lord is powerful;
 *	the voice of the Lord is majestic." (Psalm 29:4)
 *************************************************************************/
