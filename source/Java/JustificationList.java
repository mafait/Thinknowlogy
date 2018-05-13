/*	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items of self-generated knowledge
 *	Version:		Thinknowlogy 2018r1 (ShangDi 上帝)
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

class JustificationList extends List
	{
	// Private constructed variables

	private JustificationItem correctedAssumptionByOppositeQuestionJustificationItem_;


	// Private methods

	private static void clearReplacingInfo( JustificationItem searchJustificationItem )
		{
		while( searchJustificationItem != null )
			{
			searchJustificationItem.clearReplacingInfo();
			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}
		}

	private short highestJustificationOrderNr( short justificationTypeNr )
		{
		short highestOrderNr = Constants.NO_ORDER_NR;
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.orderNr > highestOrderNr &&
			searchJustificationItem.justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchJustificationItem.orderNr;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return highestOrderNr;
		}

	private short justificationOrderNr( boolean hasFeminineOrMasculineProperNounEnding, boolean hasPossessivePrimarySpecification, short justificationTypeNr, int primarySpecificationCollectionNr )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.hasFeminineOrMasculineProperNounEnding() == hasFeminineOrMasculineProperNounEnding &&
				searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem.hasPossessivePrimarySpecification() == hasPossessivePrimarySpecification &&
				searchJustificationItem.primarySpecificationCollectionNr() == primarySpecificationCollectionNr )
					return searchJustificationItem.orderNr;

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return Constants.NO_ORDER_NR;
		}

	private short questionJustificationOrderNr( SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		boolean isCheckingSpecificationCompoundCollection;
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem foundPrimarySpecificationItem;

		if( primarySpecificationItem != null &&
		secondarySpecificationItem != null )
			{
			isCheckingSpecificationCompoundCollection = ( anotherPrimarySpecificationItem == null &&
														primarySpecificationItem.hasCompoundSpecificationCollection() );

			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isQuestionJustification() )
					{
					if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem &&
					searchJustificationItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem )
						return searchJustificationItem.orderNr;

					if( isCheckingSpecificationCompoundCollection &&
					!searchJustificationItem.hasAnotherPrimarySpecification() &&
					( foundPrimarySpecificationItem = searchJustificationItem.primarySpecificationItem() ) != null )
						{
						if( foundPrimarySpecificationItem.specificationCollectionNr() == primarySpecificationItem.specificationCollectionNr() )
							return searchJustificationItem.orderNr;
						}
					}

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return Constants.NO_ORDER_NR;
		}

	private byte checkJustificationForUsage( boolean isActiveItem, JustificationItem unusedJustificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem( isActiveItem );

		if( unusedJustificationItem == null )
			return startError( 1, "The given unused justification item is undefined" );

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.attachedJustificationItem() == unusedJustificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an attached justification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tattachedJustificationItem: " + unusedJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte checkSpecificationForUsage( boolean isActiveItem, SpecificationItem unusedSpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem( isActiveItem );

		if( unusedSpecificationItem == null )
			return startError( 1, "The given unused specification item is undefined" );

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem.primarySpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a primary specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tprimarySpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			if( searchJustificationItem.anotherPrimarySpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an another primary specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tanotherPrimarySpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			if( searchJustificationItem.secondarySpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found a secondary specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tsecondarySpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			if( searchJustificationItem.anotherSecondarySpecificationItem() == unusedSpecificationItem &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an another secondary specification item that is still in use:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ";\n\tanotherSecondarySpecificationItem: " + unusedSpecificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationItem attachedJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem secondarySpecificationWordItem;
		JustificationResultType justificationResult;

		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		if( replacingJustificationItem == null )
			return startError( 1, "The given replacing justification item is undefined" );

		correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

		if( ( secondarySpecificationWordItem = replacingJustificationItem.secondarySpecificationWordItem() ) == null )
			return startError( 1, "The given replacing justification item has no secondary specification word item" );

		if( ( foundSpecificationItem = myWordItem().firstAssignmentOrSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem ) ) != null )
			{
			if( foundSpecificationItem.attachJustificationToSpecification( replacingJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to attach the replacing justification item to the found specification" );

			if( foundSpecificationItem.isReplacedItem() )
				foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

			if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null )
				{
				if( ( attachedJustificationItem = firstJustificationItem.attachedJustificationItem() ) != null )
					{
					// Remove the obsolete justification item from the replacing justification item
					if( firstJustificationItem.changeAttachedJustification( null ) != Constants.RESULT_OK )
						return addError( 1, "I failed to remove the attached justification item of the replacing justification item" );

					if( replaceOrDeleteJustification( attachedJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to replace the attached justification item, without an attached justification item" );

					if( ( justificationResult = copyJustification( false, attachedJustificationItem.updatedPrimarySpecificationItem(), attachedJustificationItem.updatedSecondarySpecificationItem(), null, attachedJustificationItem ) ).result != Constants.RESULT_OK )
						return addError( 1, "I failed to copy the attached justification item" );

					if( justificationResult.createdJustificationItem == null )
						return startError( 1, "I couldn't find or create a justification" );

					if( foundSpecificationItem.attachJustificationToSpecification( justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to attach the created justification item to the found specification" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private JustificationItem firstActiveJustificationItem()
		{
		return (JustificationItem)firstActiveItem();
		}

	private JustificationItem firstReplacedJustificationItem()
		{
		return (JustificationItem)firstReplacedItem();
		}

	private JustificationItem firstJustificationItem( boolean isActiveItem )
		{
		return ( isActiveItem ? firstActiveJustificationItem() : firstReplacedJustificationItem() );
		}

	private JustificationItem justificationItem( boolean hasFeminineOrMasculineProperNounEnding, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.hasJustification( hasFeminineOrMasculineProperNounEnding, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem ) )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}

	private JustificationItem nextJustificationListItem()
		{
		return (JustificationItem)nextListItem();
		}


	// Constructor

	protected JustificationList( WordItem myWordItem )
		{
		// Private constructed variables

		correctedAssumptionByOppositeQuestionJustificationItem_ = null;

		initializeListVariables( Constants.WORD_JUSTIFICATION_LIST_SYMBOL, "JustificationList", myWordItem );
		}


	// Protected methods

	protected void clearJustificationHasBeenWritten()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			searchJustificationItem.hasJustificationBeenWritten = false;
			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}
		}

	protected void clearReplacingInfo()
		{
		JustificationItem searchJustificationItem;

		if( ( searchJustificationItem = firstActiveJustificationItem() ) != null )
			clearReplacingInfo( searchJustificationItem );

		if( ( searchJustificationItem = firstReplacedJustificationItem() ) != null )
			clearReplacingInfo( searchJustificationItem );
		}

	protected void initializeJustificationVariables()
		{
		correctedAssumptionByOppositeQuestionJustificationItem_ = null;
		}

	protected boolean hasJustification( SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
				return true;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return false;
		}

	protected boolean hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isQuestionJustification() )
				{
				secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem();

				if( secondarySpecificationItem != null &&
				secondarySpecificationItem.hasNonCompoundSpecificationCollection() )
					return true;
				}

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return false;
		}

	protected byte attachJustification( JustificationItem attachJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		boolean hasInvolvedSpecificationRelationContext;
		boolean isPrimarySpecificationWordSpanishAmbiguous;
		boolean isSkippingUpdateJustification = false;
		short highestOrderNr;
		short justificationTypeNr;
		short previousAssumptionLevel;
		JustificationItem attachedJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem lastAttachedJustificationItem;
		JustificationItem obsoleteJustificationItem = null;
		SpecificationItem attachedPrimarySpecificationItem;
		SpecificationItem attachedSecondarySpecificationItem;
		WordItem generalizationWordItem = myWordItem();
		ShortResultType shortResult;

		if( attachJustificationItem == null )
			return startError( 1, "The given attached justification item is undefined" );

		if( !attachJustificationItem.isActiveItem() )
			return startError( 1, "The given attached justification item isn't active" );

		if( involvedSpecificationItem == null )
			return startError( 1, "The given involved specification item is undefined" );

		if( involvedSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, "The given involved specification item is replaced or deleted" );

		if( ( firstJustificationItem = involvedSpecificationItem.firstJustificationItem() ) == null )
			return startError( 1, "The given involved specification item has no justification item" );

		hasInvolvedSpecificationRelationContext = involvedSpecificationItem.hasRelationContext();
		isPrimarySpecificationWordSpanishAmbiguous = attachJustificationItem.isPrimarySpecificationWordSpanishAmbiguous();
		justificationTypeNr = attachJustificationItem.justificationTypeNr();

		attachedPrimarySpecificationItem = attachJustificationItem.primarySpecificationItem();
		attachedSecondarySpecificationItem = attachJustificationItem.secondarySpecificationItem();

		if( attachedPrimarySpecificationItem != null &&

		( ( !hasInvolvedSpecificationRelationContext &&
		!involvedSpecificationItem.isExclusiveSpecification() &&
		!attachedPrimarySpecificationItem.isExclusiveSpecification() &&

		( attachedPrimarySpecificationItem.hasRelationContext() ||
		!attachedPrimarySpecificationItem.isPossessive() ) ) ||

		( ( ( involvedSpecificationItem.isQuestion() &&
		attachedPrimarySpecificationItem.isPossessive() ) ||

		( !isPrimarySpecificationWordSpanishAmbiguous &&

		( attachedPrimarySpecificationItem.hasRelationContext() ||
		attachedPrimarySpecificationItem.isOlderItem() ) ) ) &&

		!involvedSpecificationItem.hasJustificationOfSameType( justificationTypeNr, attachedPrimarySpecificationItem, attachedSecondarySpecificationItem ) ) ) )
			{
			if( ( highestOrderNr = involvedSpecificationItem.highestAttachedJustificationOrderNr( justificationTypeNr ) ) >= Constants.MAX_ORDER_NR )
				return startSystemError( 1, "Justification order number overflow" );

			attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
			}

		if( attachJustificationItem.secondarySpecificationQuestion() == null &&
		( obsoleteJustificationItem = firstJustificationItem.secondarySpecificationQuestion() ) == null )
			{
			if( attachedPrimarySpecificationItem != null &&
			!attachedPrimarySpecificationItem.isPossessive() &&
			attachedPrimarySpecificationItem.hasRelationContext() )
				obsoleteJustificationItem = firstJustificationItem.primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem.specificationWordItem() );
			else
				{
				// Typically for Spanish
				if( attachedSecondarySpecificationItem != null &&
				attachedSecondarySpecificationItem.isUserSpecification() &&
				
				( involvedSpecificationItem.isQuestion() ||
				involvedSpecificationItem.isSpecificationWordSpanishAmbiguous() ) )
					obsoleteJustificationItem = firstJustificationItem.obsoleteSpanishJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
				}

			if( obsoleteJustificationItem != null )
				{
				if( obsoleteJustificationItem.isOlderItem() )
					{
					if( !hasInvolvedSpecificationRelationContext &&

					( !isPrimarySpecificationWordSpanishAmbiguous ||

					( attachedPrimarySpecificationItem != null &&
					attachedPrimarySpecificationItem.hasAssumptionLevel() ) ) )
						isSkippingUpdateJustification = true;
					else
						{
						if( replaceJustification( false, obsoleteJustificationItem, attachJustificationItem, involvedSpecificationItem, null ) != Constants.RESULT_OK )
							return addError( 1, "I failed to replace the obsolete justification item" );

						attachJustificationItem.orderNr = obsoleteJustificationItem.orderNr;
						}
					}
				else
					{
					obsoleteJustificationItem = null;

					if( ( highestOrderNr = involvedSpecificationItem.highestAttachedJustificationOrderNr( justificationTypeNr ) ) >= Constants.MAX_ORDER_NR )
						return startSystemError( 1, "Justification order number overflow" );

					attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
					}
				}
			}

		if( obsoleteJustificationItem == null )
			{
			// Find a justification item with current sentence number, but without attached justification item
			lastAttachedJustificationItem = firstJustificationItem;

			while( ( attachedJustificationItem = lastAttachedJustificationItem.attachedJustificationItem() ) != null )
				lastAttachedJustificationItem = attachedJustificationItem;

			if( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() )
				{
				if( lastAttachedJustificationItem.attachJustification( attachJustificationItem, involvedSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to attach a justification item" );

				if( ( !hasInvolvedSpecificationRelationContext ||
				!involvedSpecificationItem.isOlderItem() ) &&

				!involvedSpecificationItem.isHiddenSpanishSpecification() )
					{
					previousAssumptionLevel = involvedSpecificationItem.assumptionLevel();

					if( ( shortResult = involvedSpecificationItem.calculateAssumptionLevel() ).result != Constants.RESULT_OK )
						return addError( 1, "I failed to calculate the assumption level of the involved specification" );

					if( !hasInvolvedSpecificationRelationContext &&
					involvedSpecificationItem.isOlderItem() &&
					!involvedSpecificationItem.isPartOf() &&
					// Difference between recalculated assumption level and previous assumption level
					shortResult.shortValue != previousAssumptionLevel &&

					( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
					// Recalculated assumption level is higher than 'maybe'
					// Skip writing meaningless adjustment (from 'maybe' to 'maybe')
					shortResult.shortValue < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) )
						{
						// Recalculated assumption is conclusion
						if( shortResult.shortValue == Constants.NO_ASSUMPTION_LEVEL &&
						involvedSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
							return addError( 1, "I failed to mark the involved specification as concluded assumption" );

						// Write adjusted specification
						if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, involvedSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, "I failed to write an adjusted specification" );
						}
					}
				}
			else
				{
				if( !attachJustificationItem.hasAttachedJustification() &&
				// Attach justification to specification
				involvedSpecificationItem.attachJustificationToSpecification( attachJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to attach the justification item to the given involved specification item" );
				}
			}
		else
			{
			if( !isSkippingUpdateJustification &&
			generalizationWordItem.updateJustificationInSpecifications( false, obsoleteJustificationItem, attachJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to update a question justification item by a conclusion justification item" );
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForReplacedOrDeletedSpecification()
		{
		JustificationItem searchJustificationItem;

		if( ( searchJustificationItem = firstActiveJustificationItem() ) != null )
			{
			do	;
			// Check for replaced and deleted specifications
			while( searchJustificationItem.checkForReplacedOrDeletedSpecification() == Constants.RESULT_OK &&
			( searchJustificationItem = searchJustificationItem.nextJustificationItem() ) != null );
			}

		if( ( searchJustificationItem = firstReplacedJustificationItem() ) != null )
			{
			do	;
			// Only check for deleted specifications
			while( searchJustificationItem.checkForDeletedSpecifications() == Constants.RESULT_OK &&
			( searchJustificationItem = searchJustificationItem.nextJustificationItem() ) != null );
			}

		return GlobalVariables.result;
		}

	protected byte checkForUnreferencedReplacedJustifications()
		{
		JustificationItem searchJustificationItem = firstReplacedJustificationItem();
		WordItem generalizationWordItem = myWordItem();

		while( searchJustificationItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( !generalizationWordItem.isJustificationInUse( searchJustificationItem ) &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found an unreferenced replaced justification item:\n\tJustificationItem: " + searchJustificationItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkJustificationForUsage( JustificationItem unusedJustificationItem )
		{
		if( checkJustificationForUsage( true, unusedJustificationItem ) == Constants.RESULT_OK )
			{
			if( unusedJustificationItem != null &&
			unusedJustificationItem.isReplacedItem() )
				return checkJustificationForUsage( false, unusedJustificationItem );
			}

		return GlobalVariables.result;
		}

	protected byte checkSpecificationForUsage( SpecificationItem unusedSpecificationItem )
		{
		if( checkSpecificationForUsage( true, unusedSpecificationItem ) == Constants.RESULT_OK )
			return checkSpecificationForUsage( false, unusedSpecificationItem );

		return GlobalVariables.result;
		}

	protected byte replaceJustification( boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem, SpecificationItem involvedSpecificationItem, SpecificationItem replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		boolean hasCorrectedAssumptionByKnowledge;
		boolean hasCorrectedAssumptionByOppositeQuestion;
		JustificationItem attachedJustificationItem;
		SpecificationItem relatedSpecificationItem;
		WordItem generalizationWordItem = myWordItem();
		RelatedResultType relatedResult;

		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		if( involvedSpecificationItem == null )
			return startError( 1, "The given involved specification item is undefined" );

		if( obsoleteJustificationItem == replacingJustificationItem )
			return startError( 1, "The given obsolete justification item and the given replacing justification item are the same justification item" );

		hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();
		hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();

		// Corrected assumption
		if( ( hasCorrectedAssumptionByKnowledge ||
		hasCorrectedAssumptionByOppositeQuestion ) &&

		replacingJustificationItem != null &&
		replacingJustificationItem.isGeneralizationAssumption() )
			{
			if( !hasCorrectedAssumptionByOppositeQuestion &&
			involvedSpecificationItem.isReplacedItem() )
				involvedSpecificationItem = involvedSpecificationItem.updatedSpecificationItem();

			if( replacingJustificationItem.primarySpecificationWordItem() == involvedSpecificationItem.specificationWordItem() )
				{
				if( generalizationWordItem.writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, involvedSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to write an adjusted specification" );

				// Corrected negative assumption
				if( ( relatedResult = generalizationWordItem.findRelatedSpecification( false, involvedSpecificationItem.isAssignment(), involvedSpecificationItem.isArchivedAssignment(), involvedSpecificationItem.isExclusiveSpecification(), true, involvedSpecificationItem.isPossessive(), Constants.NO_QUESTION_PARAMETER, involvedSpecificationItem.specificationCollectionNr(), involvedSpecificationItem.generalizationContextNr(), involvedSpecificationItem.specificationContextNr(), involvedSpecificationItem.relationContextNr(), involvedSpecificationItem.specificationWordItem() ) ).result != Constants.RESULT_OK )
					return addError( 1, "I failed to find a negative related specification" );

				if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != null )
					{
					if( generalizationWordItem.writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, relatedSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to write a corrected negative assumption" );

					if( hasCorrectedAssumptionByOppositeQuestion &&
					generalizationWordItem.replaceOrDeleteSpecification( false, relatedSpecificationItem, involvedSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
						return addError( 1, "I failed to replace a corrected negative assumption" );
					}
				}

			if( ( attachedJustificationItem = replacingJustificationItem.attachedJustificationItem() ) != null &&

			( !hasCorrectedAssumptionByKnowledge ||
			!attachedJustificationItem.isUpdatedPrimarySpecificationWordSpanishAmbiguous() ) )
				{
				if( replaceOrDeleteJustification( replacingJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to delete a corrected justification" );

				replacingJustificationItem = null;
				}
			}

		attachedJustificationItem = obsoleteJustificationItem.attachedJustificationItem();

		// Old justification has an attached justification,
		if( attachedJustificationItem != null &&
		// that is active,
		attachedJustificationItem.isActiveItem() &&
		replacingJustificationItem != null &&
		replacingJustificationItem != attachedJustificationItem &&
		// but the replacing one is current
		replacingJustificationItem.hasCurrentActiveSentenceNr() &&
		// and it has no attached justifications
		!replacingJustificationItem.hasAttachedJustification() &&

		// Change attached justification
		replacingJustificationItem.changeAttachedJustification( attachedJustificationItem ) != Constants.RESULT_OK )
			return addError( 1, "I failed to change an attached justification item" );

		if( attachedJustificationItem == null ||
		!attachedJustificationItem.isActiveItem() ||

		// Replacing justification item has the same attached justification item
		( replacingJustificationItem != null &&

		( replacingJustificationItem == attachedJustificationItem ||
		replacingJustificationItem.attachedJustificationItem() == attachedJustificationItem ) ) )
			{
			if( obsoleteJustificationItem.isActiveItem() )
				{
				if( replaceOrDeleteJustification( obsoleteJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to replace an active justification item" );

				if( generalizationWordItem.updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to update a justification in specifications of my word" );
				}
			else
				{
				if( hasCorrectedAssumptionByOppositeQuestion &&
				replaceJustificationOfCorrectedAssumptionByOppositeQuestion( obsoleteJustificationItem, replacingJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to replace the justification of a corrected assumption by opposite question" );
				}
			}
		else
			{
			if( replacingCorrectedAssumptionByKnowledgeSpecificationItem != null )
				involvedSpecificationItem = replacingCorrectedAssumptionByKnowledgeSpecificationItem;

			if( involvedSpecificationItem.hasCurrentCreationSentenceNr() &&
			!involvedSpecificationItem.isReplacedItem() &&
			// Obsolete justification of adjusted question
			!attachedJustificationItem.isQuestionJustification() )
				{
				if( involvedSpecificationItem.changeFirstJustification( !hasCorrectedAssumptionByKnowledge, attachedJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given obsolete justification item" );
				}
			else
				{
				if( ( hasCorrectedAssumptionByKnowledge ||
				hasCorrectedAssumptionByOppositeQuestion ||
				!generalizationWordItem.isJustificationInUse( attachedJustificationItem ) ) &&

				// Replace or delete justification
				replaceOrDeleteJustification( attachedJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to replace the attached justification item" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( obsoleteJustificationItem == null )
			return startError( 1, "The given obsolete justification item is undefined" );

		if( obsoleteJustificationItem.hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to delete an obsolete justification" );
			}
		else
			{
			if( replaceItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, "I failed to replace an obsolete justification" );
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteUnreferencedJustifications()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		WordItem generalizationWordItem = myWordItem();

		while( searchJustificationItem != null )
			{
			if( generalizationWordItem.isJustificationInUse( searchJustificationItem ) )
				searchJustificationItem = searchJustificationItem.nextJustificationItem();
			else
				{
				if( replaceOrDeleteJustification( searchJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to replace or delete an unreferenced justification item" );

				searchJustificationItem = nextJustificationListItem();
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte updateSpecificationsInJustifications( boolean isMainWord, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		boolean hasCorrectedAssumption;
		boolean hasCorrectedAssumptionByKnowledge;
		boolean isReplacingPrimarySpecification;
		boolean isReplacingAnotherPrimarySpecification;
		boolean isReplacingSecondarySpecification;
		boolean isReplacingAnotherSecondarySpecification;
		boolean isExclusiveGeneralization = false;
		JustificationItem createdJustificationItem = null;
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem;
		WordItem generalizationWordItem = myWordItem();
		JustificationResultType justificationResult = new JustificationResultType();

		if( obsoleteSpecificationItem == null )
			return startError( 1, "The given obsolete specification item is undefined" );

		hasCorrectedAssumption = generalizationWordItem.hasCorrectedAssumption();
		hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();

		if( replacingSpecificationItem != null &&
		replacingSpecificationItem.isExclusiveNonPossessiveGeneralizationAssignment() &&
		!obsoleteSpecificationItem.isQuestion() )
			isExclusiveGeneralization = true;

		while( searchJustificationItem != null )
			{
			primarySpecificationItem = searchJustificationItem.primarySpecificationItem();
			anotherPrimarySpecificationItem = searchJustificationItem.anotherPrimarySpecificationItem();
			secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem();
			anotherSecondarySpecificationItem = searchJustificationItem.anotherSecondarySpecificationItem();

			isReplacingPrimarySpecification = ( primarySpecificationItem == obsoleteSpecificationItem ||

												// Test file: "Laura has a father - Laura is a child - John and Anna"
												( !isMainWord &&
												replacingSpecificationItem != null &&
												searchJustificationItem.hasReplacedPrimarySpecification() &&
												searchJustificationItem.isOppositePossessiveConditionalSpecificationAssumption() &&
												obsoleteSpecificationItem.hasRelationContext() &&
												replacingSpecificationItem.isConcludedAssumption() &&
												obsoleteSpecificationItem.generalizationWordItem().hasCurrentlyConfirmedSpecification() ) );

			isReplacingAnotherPrimarySpecification = ( anotherPrimarySpecificationItem == obsoleteSpecificationItem );

			isReplacingSecondarySpecification = ( secondarySpecificationItem == obsoleteSpecificationItem &&

												// Test file: "Complex (19)"
												( !isMainWord ||
												!searchJustificationItem.isPossessiveReversibleConclusion() ) );

			isReplacingAnotherSecondarySpecification = ( anotherSecondarySpecificationItem == obsoleteSpecificationItem );

			if( isReplacingPrimarySpecification ||
			isReplacingAnotherPrimarySpecification ||
			isReplacingSecondarySpecification ||
			isReplacingAnotherSecondarySpecification )
				{
				if( replacingSpecificationItem != null &&
				searchJustificationItem.hasCurrentCreationSentenceNr() )
					{
					if( isReplacingPrimarySpecification &&
					searchJustificationItem.changePrimarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to change the primary specificationItem item of a justification item" );

					if( isReplacingAnotherPrimarySpecification &&
					searchJustificationItem.changeAnotherPrimarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to change the another primary specificationItem item of a justification item" );

					if( isReplacingSecondarySpecification &&
					searchJustificationItem.changeSecondarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to change the secondary specificationItem item of a justification item" );

					if( isReplacingAnotherSecondarySpecification &&
					searchJustificationItem.changeAnotherSecondarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to change the another secondary specificationItem item of a justification item" );

					searchJustificationItem = searchJustificationItem.nextJustificationItem();
					}
				else
					{
					createdJustificationItem = null;

					if( replacingSpecificationItem != null &&

					( searchJustificationItem.isQuestionJustification() ||
					// Avoid creating unreferenced justifications
					generalizationWordItem.isJustificationInUse( searchJustificationItem ) ) )
						{
						if( ( justificationResult = addJustification( searchJustificationItem.hasFeminineOrMasculineProperNounEnding(), ( isReplacingSecondarySpecification && searchJustificationItem.isPossessiveReversibleAssumptionOrConclusion() ), false, searchJustificationItem.updatedJustificationTypeNr( replacingSpecificationItem ), searchJustificationItem.orderNr, searchJustificationItem.originalSentenceNr(), ( isReplacingPrimarySpecification ? replacingSpecificationItem : searchJustificationItem.updatedPrimarySpecificationItem() ), ( isReplacingAnotherPrimarySpecification ? replacingSpecificationItem : anotherPrimarySpecificationItem ), ( isReplacingSecondarySpecification ? replacingSpecificationItem : searchJustificationItem.updatedSecondarySpecificationItem() ), ( isReplacingAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem ), searchJustificationItem.attachedJustificationItem() ) ).result != Constants.RESULT_OK )
							return addError( 1, "I failed to add a justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
							{
							if( justificationResult.foundJustificationItem == searchJustificationItem )
								return addError( 1, "No justification item is created, and the found justification item is the same as the search item itself" );

							createdJustificationItem = justificationResult.foundJustificationItem;
							}
						}

					if( replaceJustification( isExclusiveGeneralization, searchJustificationItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : null ) ) != Constants.RESULT_OK )
						return addError( 1, "I failed to replace a justification item" );

					searchJustificationItem = ( hasCorrectedAssumption ? searchJustificationItem.nextJustificationItem() : firstActiveJustificationItem() );
					}
				}
			else
				searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte writeRelatedJustificationSpecifications( short justificationTypeNr, int specificationCollectionNr )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;
		WordItem generalizationWordItem;

		while( searchJustificationItem != null )
			{
			if( !searchJustificationItem.hasJustificationBeenWritten &&
			searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null &&
			!secondarySpecificationItem.isExclusiveSpecification() &&
			secondarySpecificationItem.specificationCollectionNr() == specificationCollectionNr )
				{
				if( ( generalizationWordItem = secondarySpecificationItem.generalizationWordItem() ) == null )
					return startError( 1, "The generalization word of the given secondary specification item is undefined" );

				if( generalizationWordItem.writeJustificationSpecification( false, secondarySpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to write a secondary justification specification" );
				}

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected JustificationItem correctedAssumptionByOppositeQuestionJustificationItem()
		{
		return correctedAssumptionByOppositeQuestionJustificationItem_;
		}

	protected JustificationItem negativeAssumptionOrConclusionJustificationItem( SpecificationItem anotherPrimarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( anotherPrimarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isNegativeAssumptionOrConclusion() &&
				searchJustificationItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem olderComplexJustificationItem( boolean hasSecondarySpecificationWithoutRelationContext, boolean isPossessiveSecondarySpecification, short justificationTypeNr, int secondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem searchPrimarySpecificationItem;
		SpecificationItem searchSecondarySpecificationItem;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
			( searchSecondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null )
				{
				searchPrimarySpecificationItem = searchJustificationItem.primarySpecificationItem();

				if( primarySpecificationItem == null ||

				( primarySpecificationItem == searchPrimarySpecificationItem &&

				( !hasSecondarySpecificationWithoutRelationContext ||
				searchJustificationItem.isOlderItem() ) ) ||

				// Primary specifications have different relation context
				( searchPrimarySpecificationItem != null &&
				searchJustificationItem.isOlderItem() &&
				searchPrimarySpecificationItem.generalizationWordItem() == primarySpecificationItem.generalizationWordItem() &&
				searchPrimarySpecificationItem.isRelatedSpecification( primarySpecificationItem.isExclusiveSpecification(), primarySpecificationItem.isNegative(), primarySpecificationItem.isPossessive(), primarySpecificationItem.specificationWordItem() ) ) )
					{
					if( ( !hasSecondarySpecificationWithoutRelationContext ||
					!searchSecondarySpecificationItem.hasRelationContext() ) &&

					( secondarySpecificationCollectionNr == Constants.NO_COLLECTION_NR ||

					( !searchSecondarySpecificationItem.isConcludedAssumption() &&
					searchSecondarySpecificationItem.isPossessive() == isPossessiveSecondarySpecification &&
					searchSecondarySpecificationItem.specificationCollectionNr() == secondarySpecificationCollectionNr ) ) )
						return searchJustificationItem;
					}
				}

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationItem possessiveReversibleAssumptionJustificationItem( WordItem generalizationWordItem, WordItem secondarySpecificationWordItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		if( generalizationWordItem != null &&
		secondarySpecificationWordItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isPossessiveReversibleAssumption() )
					{
					secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem();

					if( secondarySpecificationItem != null &&
					secondarySpecificationItem.generalizationWordItem() == generalizationWordItem &&
					secondarySpecificationItem.specificationWordItem() == secondarySpecificationWordItem )
						return searchJustificationItem;
					}

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem primarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( ( !isSelectingOlderItemOnly ||
				searchJustificationItem.isOlderItem() ) &&

				searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem.primarySpecificationItem() == primarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem secondarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( ( !isSelectingOlderItemOnly ||
				searchJustificationItem.isOlderItem() ) &&

				searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem olderSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.isSuggestiveQuestionAssumption() &&
			( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null )
				{
				if( secondarySpecificationItem.isOlderItem() )
					return secondarySpecificationItem;
				}

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationResultType addJustification( boolean hasFeminineOrMasculineProperNounEnding, boolean isForcingNewJustification, boolean isIncrementingOrderNr, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem )
		{
		boolean hasPrimarySpecificationRelationContext = false;
		boolean isExclusivePrimarySpecification = false;
		boolean isPrimarySpecificationWordSpanishAmbiguous = false;
		short foundJustificationOrderNr;
		int primarySpecificationCollectionNr;
		JustificationItem existingJustificationItem;
		JustificationItem foundJustificationItem = null;
		JustificationResultType justificationResult = new JustificationResultType();

		if( primarySpecificationItem == null &&
		secondarySpecificationItem == null )
			return startJustificationResultError( 1, "Both the given primary specification item and the given secondary specification item are undefined" );

		if( primarySpecificationItem != null )
			{
			if( primarySpecificationItem.isReplacedItem() )
				return startJustificationResultError( 1, "The given primary specification item is replaced" );

			if( primarySpecificationItem.isHiddenSpanishSpecification() )
				return startJustificationResultError( 1, "The given primary specification item is hidden" );
			}

		if( anotherPrimarySpecificationItem != null )
			{
			if( anotherPrimarySpecificationItem.isReplacedItem() )
				return startJustificationResultError( 1, "The given another primary specification item is replaced" );

			if( anotherPrimarySpecificationItem.isHiddenSpanishSpecification() )
				return startJustificationResultError( 1, "The given another primary specification item is hidden" );
			}

		if( secondarySpecificationItem != null )
			{
			if( secondarySpecificationItem.isReplacedItem() )
				return startJustificationResultError( 1, "The given secondary specification item is replaced" );

			if( secondarySpecificationItem.isHiddenSpanishSpecification() )
				return startJustificationResultError( 1, "The given secondary specification item is hidden" );
			}

		if( anotherSecondarySpecificationItem != null )
			{
			if( anotherSecondarySpecificationItem.isReplacedItem() )
				return startJustificationResultError( 1, "The given another secondary specification item is replaced" );

			if( anotherSecondarySpecificationItem.isHiddenSpanishSpecification() )
				return startJustificationResultError( 1, "The given another secondary specification item is hidden" );
			}

		// Skipping search, will force the creation of another justification item
		if( !isForcingNewJustification )
			foundJustificationItem = justificationItem( hasFeminineOrMasculineProperNounEnding, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem );

		if( foundJustificationItem == null ||

		( attachedJustificationItem == null &&
		foundJustificationItem.isQuestionJustification() &&
		foundJustificationItem.justificationTypeNr() != justificationTypeNr ) ||

		( attachedJustificationItem != null &&
		foundJustificationItem.justificationTypeNr() == justificationTypeNr &&
		// Avoid looping
		foundJustificationItem.attachedJustificationItem() != attachedJustificationItem ) )
			{
			if( orderNr == Constants.NO_ORDER_NR )
				{
				if( foundJustificationItem == null &&
				primarySpecificationItem != null &&
				secondarySpecificationItem != null &&

				( justificationTypeNr != 16 ||
				anotherPrimarySpecificationItem != null ) &&

				( foundJustificationItem = olderComplexJustificationItem( false, secondarySpecificationItem.isPossessive(), justificationTypeNr, secondarySpecificationItem.specificationCollectionNr(), primarySpecificationItem ) ) != null )
					orderNr = ( foundJustificationItem.secondaryGeneralizationWordItem() == secondarySpecificationItem.generalizationWordItem() ? foundJustificationItem.orderNr : 1 );
				else
					{
					orderNr = ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||
								justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
								justificationTypeNr == Constants.JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ||
								justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION ||
								justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION ? 1 : highestJustificationOrderNr( justificationTypeNr ) );

					if( primarySpecificationItem != null )
						{
						hasPrimarySpecificationRelationContext = primarySpecificationItem.hasRelationContext();
						isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem.isSpecificationWordSpanishAmbiguous();
						}

					if( orderNr > Constants.NO_ORDER_NR &&
					primarySpecificationItem != null &&
					secondarySpecificationItem != null )
						{
						// Group justification by selecting the order number
						switch( justificationTypeNr )
							{
							case Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
								if( anotherPrimarySpecificationItem == null &&
								secondarySpecificationItem.isGeneralizationProperNoun() &&
								( existingJustificationItem = primarySpecificationJustificationItem( false, Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, primarySpecificationItem ) ) != null )
									isIncrementingOrderNr = true;

								break;

							case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( primarySpecificationItem.isExclusiveSpecification() )
									isExclusivePrimarySpecification = true;

								// Don't insert a break statement here

							case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
							case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
								// See case above
								if( ( isExclusivePrimarySpecification ||

								// Typically for Spanish
								( ( isPrimarySpecificationWordSpanishAmbiguous &&
								hasPrimarySpecificationRelationContext &&
								primarySpecificationItem.generalizationWordItem() == myWordItem() ) ||

								( hasFeminineOrMasculineProperNounEnding &&
								anotherPrimarySpecificationItem != null &&
								anotherPrimarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ) &&

								( primarySpecificationCollectionNr = primarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
									{
									if( ( foundJustificationOrderNr = justificationOrderNr( hasFeminineOrMasculineProperNounEnding, primarySpecificationItem.isPossessive(), justificationTypeNr, primarySpecificationCollectionNr ) ) == Constants.NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}
								else
									{
									if( justificationTypeNr == Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION )
										{
										if( ( isPrimarySpecificationWordSpanishAmbiguous &&
										hasPrimarySpecificationRelationContext &&
										orderNr == 1 ) ||

										( ( existingJustificationItem = primarySpecificationJustificationItem( true, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, primarySpecificationItem ) ) != null &&
										existingJustificationItem.hasAnotherPrimarySpecification() ) )
											isIncrementingOrderNr = true;
										}
									}

								break;

							case Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
								// Typically for Spanish
								if( anotherPrimarySpecificationItem == null &&
								anotherSecondarySpecificationItem != null &&
								secondarySpecificationItem.hasRelationContext() )
										isIncrementingOrderNr = true;

								break;

							case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
								// Typically for Spanish
								if( anotherSecondarySpecificationItem == null )
									{
									if( ( foundJustificationOrderNr = questionJustificationOrderNr( primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem ) ) == Constants.NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}
							}
						}

					if( !isExclusivePrimarySpecification &&

					( isIncrementingOrderNr ||
					orderNr == Constants.NO_ORDER_NR ||
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

					// Typically for Spanish
					( primarySpecificationItem != null &&
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&

					// Skip self-generated question of corrected assumption by opposite question
					( hasPrimarySpecificationRelationContext ||
					anotherPrimarySpecificationItem == null ) ) ) )
						orderNr++;
					}
				}

			if( orderNr <= Constants.NO_ORDER_NR )
				return startJustificationResultError( 1, "The order number is undefined" );

			if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem, this, myWordItem() ) ) == null )
				return startJustificationResultError( 1, "I failed to create a justification item" );

			// Add justification item
			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
				return addJustificationResultError( 1, "I failed to add an active justification item" );
			}

		justificationResult.foundJustificationItem = foundJustificationItem;
		return justificationResult;
		}

	protected JustificationResultType copyJustification( boolean isForcingNewJustification, SpecificationItem newPrimarySpecificationItem, SpecificationItem newSecondarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		if( originalJustificationItem != null )
			return addJustification( originalJustificationItem.hasFeminineOrMasculineProperNounEnding(), isForcingNewJustification, false, originalJustificationItem.updatedJustificationTypeNr( newSecondarySpecificationItem ), originalJustificationItem.orderNr, originalJustificationItem.originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem.anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem.anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		return startJustificationResultError( 1, "The given original justification item is undefined" );
		}
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
