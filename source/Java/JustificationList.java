/*	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items for the
 *					self-generated knowledge
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

class JustificationList extends List
	{
	// Private constructed variables

	private JustificationItem correctedAssumptionByOppositeQuestionJustificationItem_;


	// Private methods

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

	private short justificationOrderNr( boolean hasFeminineOrMasculineProperNameEnding, boolean hasPossessivePrimarySpecification, short justificationTypeNr, int primarySpecificationCollectionNr )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem.hasPossessivePrimarySpecification() == hasPossessivePrimarySpecification &&
				searchJustificationItem.primarySpecificationCollectionNr() == primarySpecificationCollectionNr &&
				searchJustificationItem.hasFeminineOrMasculineProperNameEnding() == hasFeminineOrMasculineProperNameEnding )
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
														primarySpecificationItem.hasSpecificationCompoundCollection() );

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

	private byte checkForUnreferencedJustification( boolean isActiveItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem( isActiveItem );
		WordItem generalizationWordItem = myWordItem();

		while( searchJustificationItem != null )
			{
			if( !generalizationWordItem.isJustificationInUse( searchJustificationItem ) )
				return startError( 1, null, "I have found an unreferenced justification item" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte checkJustificationForUsage( boolean isActiveItem, JustificationItem unusedJustificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem( isActiveItem );

		if( unusedJustificationItem == null )
			return startError( 1, null, "The given unused justification item is undefined" );

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.attachedJustificationItem() == unusedJustificationItem )
				return startError( 1, null, "I have found an attached justification item that is still in use" );

			if( searchJustificationItem.replacingJustificationItem == unusedJustificationItem )
				return startError( 1, null, "I have found a replacing justification item that is still in use" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte checkSpecificationItemForUsage( boolean isActiveItem, SpecificationItem unusedSpecificationItem )
		{
		JustificationItem searchJustificationItem = firstJustificationItem( isActiveItem );

		if( unusedSpecificationItem == null )
			return startError( 1, null, "The given unused specification item is undefined" );

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.primarySpecificationItem() == unusedSpecificationItem )
				return startError( 1, null, "The primary specification item is still in use" );

			if( searchJustificationItem.anotherPrimarySpecificationItem() == unusedSpecificationItem )
				return startError( 1, null, "The another primary specification item is still in use" );

			if( searchJustificationItem.secondarySpecificationItem() == unusedSpecificationItem )
				return startError( 1, null, "The secondary specification item is still in use" );

			if( searchJustificationItem.anotherSecondarySpecificationItem() == unusedSpecificationItem )
				return startError( 1, null, "The another secondary specification item is still in use" );

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem attachedJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( obsoleteJustificationItem == null )
			return startError( 1, null, "The given obsolete justification item is undefined" );

		if( replacingJustificationItem == null )
			return startError( 1, null, "The given replacing justification item is undefined" );

		correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

		if( ( secondarySpecificationWordItem = replacingJustificationItem.secondarySpecificationWordItem() ) == null )
			return startError( 1, null, "The given replacing justification item has no secondary specification word item" );

		if( ( foundSpecificationItem = myWordItem().firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != null )
			{
			if( foundSpecificationItem.attachJustificationToSpecification( replacingJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to attach the replacing justification item to the found specification" );

			foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

			if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null )
				{
				if( ( attachedJustificationItem = firstJustificationItem.attachedJustificationItem() ) != null )
					{
					// Remove the obsolete justification item from the replacing justification item
					if( firstJustificationItem.changeAttachedJustification( null ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to remove the attached justification item of the replacing justification item" );

					if( replaceOrDeleteJustification( attachedJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to replace the attached justification item, without an attached justification item" );

					if( ( justificationResult = copyJustificationItem( attachedJustificationItem.primarySpecificationItem(), attachedJustificationItem.secondarySpecificationItem(), null, attachedJustificationItem ) ).result != Constants.RESULT_OK )
						return addError( 1, null, "I failed to copy the attached justification item" );

					if( justificationResult.createdJustificationItem == null )
						return startError( 1, null, "I couldn't find or create a justification" );

					if( foundSpecificationItem.attachJustificationToSpecification( justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to attach the created justification item to the found specification" );
					}
				}
			}

		return Constants.RESULT_OK;
		}
/*
	private byte storeChangesInFutureDatabase( JustificationItem searchJustificationItem )
		{
		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.hasCurrentCreationSentenceNr() )
				{
				if( searchJustificationItem.storeJustificationItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a justification item in the database" );
				}

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}
*/
	private byte updateReplacedJustifications( JustificationItem updateJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationItem searchJustificationItem = firstReplacedJustificationItem();

		if( updateJustificationItem == null )
			return startError( 1, null, "The given update justification item is undefined" );

		if( replacingJustificationItem == null )
			return startError( 1, null, "The given replacing justification item is undefined" );

		if( replacingJustificationItem.isReplacedItem() )
			return startError( 1, null, "The replaced justification item of the given update justification item is a replaced item" );

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.replacingJustificationItem == updateJustificationItem )
				searchJustificationItem.replacingJustificationItem = replacingJustificationItem;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
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

	private JustificationItem getJustificationItem( boolean hasFeminineOrMasculineProperNameEnding, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchJustificationItem.secondarySpecificationItem() == secondarySpecificationItem &&
			searchJustificationItem.anotherSecondarySpecificationItem() == anotherSecondarySpecificationItem &&
			searchJustificationItem.hasFeminineOrMasculineProperNameEnding() == hasFeminineOrMasculineProperNameEnding )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}


	// Constructor

	protected JustificationList( WordItem myWordItem )
		{
		// Private constructed variables
		correctedAssumptionByOppositeQuestionJustificationItem_ = null;

		initializeListVariables( Constants.WORD_JUSTIFICATION_LIST_SYMBOL, myWordItem );
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

	protected void initializeJustificationVariables()
		{
		correctedAssumptionByOppositeQuestionJustificationItem_ = null;
		}

	protected JustificationResultType addJustification( boolean hasFeminineOrMasculineProperNameEnding, boolean isForcingNewJustification, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();
		boolean isExclusivePrimarySpecification = false;
		boolean isIncrementingOrderNr = false;
		short foundJustificationOrderNr;
		int primarySpecificationCollectionNr;
		JustificationItem foundJustificationItem = null;
		SpecificationItem updatedPrimarySpecificationItem = ( primarySpecificationItem == null ? null : primarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedAnotherPrimarySpecificationItem = ( anotherPrimarySpecificationItem == null ? null : anotherPrimarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedSecondarySpecificationItem = ( secondarySpecificationItem == null ? null : secondarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedAnotherSecondarySpecificationItem = ( anotherSecondarySpecificationItem == null ? null : anotherSecondarySpecificationItem.updatedSpecificationItem() );

		if( primarySpecificationItem == null &&
		secondarySpecificationItem == null )
			return startJustificationResultError( 1, null, "Both the given primary specification item and the given secondary specification item are undefined" );

		// Skipping search, forcing to create a new justification item
		if( !isForcingNewJustification )
			foundJustificationItem = getJustificationItem( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem );

		if( foundJustificationItem == null ||

		( attachedJustificationItem != null &&
		foundJustificationItem.justificationTypeNr() == justificationTypeNr &&
		// Avoid looping
		foundJustificationItem.attachedJustificationItem() != attachedJustificationItem ) )
			{
			if( orderNr == Constants.NO_ORDER_NR )
				{
				if( foundJustificationItem == null &&
				updatedPrimarySpecificationItem != null &&
				updatedSecondarySpecificationItem != null &&
				( foundJustificationItem = olderComplexJustificationItem( false, justificationTypeNr, updatedSecondarySpecificationItem.specificationCollectionNr(), updatedPrimarySpecificationItem ) ) != null )
					orderNr = foundJustificationItem.orderNr;
				else
					{
					orderNr = highestJustificationOrderNr( justificationTypeNr );

					if( orderNr > Constants.NO_ORDER_NR &&
					updatedPrimarySpecificationItem != null &&
					updatedSecondarySpecificationItem != null )
						{
						// Group justification by selecting the order number
						switch( justificationTypeNr )
							{
							case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( updatedPrimarySpecificationItem.isExclusiveSpecification() )
									isExclusivePrimarySpecification = true;

								// Don't insert a break statement here

							case Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
							case Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
								if( ( isExclusivePrimarySpecification ||

								( ( updatedPrimarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&
								updatedPrimarySpecificationItem.hasRelationContext() &&
								updatedPrimarySpecificationItem.generalizationWordItem() == myWordItem() ) ||

								( hasFeminineOrMasculineProperNameEnding &&
								updatedAnotherPrimarySpecificationItem != null &&
								updatedAnotherPrimarySpecificationItem.isSpecificationWordSpanishAmbiguous() ) ) ) &&

								( primarySpecificationCollectionNr = updatedPrimarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
									{
									if( ( foundJustificationOrderNr = justificationOrderNr( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem.isPossessive(), justificationTypeNr, primarySpecificationCollectionNr ) ) == Constants.NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}

								break;

							case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
								// Spanish ambiguous
								if( updatedAnotherSecondarySpecificationItem == null )
									{
									if( ( foundJustificationOrderNr = questionJustificationOrderNr( updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem ) ) == Constants.NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}
							}
						}

					if( !isExclusivePrimarySpecification &&

					( isIncrementingOrderNr ||
					orderNr == Constants.NO_ORDER_NR ||
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

					( updatedPrimarySpecificationItem != null &&

					( ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&

					// Skip self-generated question of corrected assumption by opposite question
					( updatedAnotherPrimarySpecificationItem == null ||
					updatedPrimarySpecificationItem.hasRelationContext() ) ) ||

					( updatedPrimarySpecificationItem.isSpecificationWordSpanishAmbiguous() &&

					( justificationTypeNr == Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||

					( hasFeminineOrMasculineProperNameEnding &&
					justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION &&
					updatedPrimarySpecificationItem.isPossessive() &&
					// If justification without feminine or masculine proper name ending exists
					getJustificationItem( false, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem ) != null ) ) ) ) ) ) )
						orderNr++;
					}
				}

			if( orderNr <= Constants.NO_ORDER_NR )
				return startJustificationResultError( 1, null, "The order number is undefined" );

			if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNameEnding, justificationTypeNr, orderNr, originalSentenceNr, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem, attachedJustificationItem, this, myWordItem() ) ) == null )
				return startJustificationResultError( 1, null, "I failed to create a justification item" );

			// Add justification item
			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
				return addJustificationResultError( 1, null, "I failed to add an active justification item" );
			}

		justificationResult.foundJustificationItem = foundJustificationItem;
		return justificationResult;
		}

	protected JustificationResultType copyJustificationItem( SpecificationItem newPrimarySpecificationItem, SpecificationItem newSecondarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		if( originalJustificationItem != null )
			return addJustification( originalJustificationItem.hasFeminineOrMasculineProperNameEnding(), false, originalJustificationItem.justificationTypeNr(), originalJustificationItem.orderNr, originalJustificationItem.originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem.anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem.anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		return startJustificationResultError( 1, null, "The given original justification item is undefined" );
		}

	protected byte attachJustification( JustificationItem attachJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		SpecificationResultType specificationResult;
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

		if( attachJustificationItem == null )
			return startError( 1, null, "The given attached justification item is undefined" );

		if( involvedSpecificationItem == null )
			return startError( 1, null, "The given involved specification item is undefined" );

		if( !attachJustificationItem.isActiveItem() )
			return startError( 1, null, "The given attached justification item isn't active" );

		if( involvedSpecificationItem.isReplacedOrDeletedItem() )
			return startError( 1, null, "The given involved specification item is replaced or deleted" );

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
				return startSystemError( 1, null, "Justification order number overflow" );

			attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
			}

		if( ( firstJustificationItem = involvedSpecificationItem.firstJustificationItem() ) == null )
			return startError( 1, null, "The given involved specification item has no justification item" );

		if( attachJustificationItem.secondarySpecificationQuestion() == null &&
		( obsoleteJustificationItem = firstJustificationItem.secondarySpecificationQuestion() ) == null )
			{
			if( attachedPrimarySpecificationItem != null &&
			!attachedPrimarySpecificationItem.isPossessive() &&
			attachedPrimarySpecificationItem.hasRelationContext() )
				obsoleteJustificationItem = firstJustificationItem.primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem );
			else
				{
				// Typically for the Spanish language
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
							return addError( 1, null, "I failed to replace the obsolete justification item" );

						attachJustificationItem.orderNr = obsoleteJustificationItem.orderNr;
						}
					}
				else
					{
					obsoleteJustificationItem = null;

					if( ( highestOrderNr = involvedSpecificationItem.highestAttachedJustificationOrderNr( justificationTypeNr ) ) >= Constants.MAX_ORDER_NR )
						return startSystemError( 1, null, "Justification order number overflow" );

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
				if( ( specificationResult = involvedSpecificationItem.getAssumptionLevel() ).result != Constants.RESULT_OK )
					return addError( 1, null, "I failed to get the assumption level" );

				previousAssumptionLevel = specificationResult.assumptionLevel;

				if( lastAttachedJustificationItem.attachJustification( attachJustificationItem, involvedSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to attach a justification item" );

				if( !hasInvolvedSpecificationRelationContext ||
				!involvedSpecificationItem.isOlderItem() )
					{
					if( ( specificationResult = involvedSpecificationItem.recalculateAssumptionLevel() ).result != Constants.RESULT_OK )
						return addError( 1, null, "I failed to recalculate the assumption level" );

					if( !hasInvolvedSpecificationRelationContext &&
					involvedSpecificationItem.isOlderItem() &&
					specificationResult.assumptionLevel != previousAssumptionLevel &&

					( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
					// Skip writing adjustments of uncertainty level higher than 'maybe'
					specificationResult.assumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) )
						{
						// Conclusion
						if( specificationResult.assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
							{
							if( involvedSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
								return addError( 1, null, "I failed to mark the involved specification as concluded assumption" );
							}

						// Write adjusted specification
						if( generalizationWordItem.writeUpdatedSpecification( true, false, false, false, false, false, involvedSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to write an adjusted specification" );
						}
					}
				}
			else
				{
				if( !attachJustificationItem.hasAttachedJustification() )
					{
					if( involvedSpecificationItem.attachJustificationToSpecification( attachJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to attach the justification item to the given involved specification item" );
					}
				}
			}
		else
			{
			if( !isSkippingUpdateJustification )
				{
				if( generalizationWordItem.updateJustificationInSpecifications( false, obsoleteJustificationItem, attachJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to update a question justification item by a conclusion justification item" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForUnreferencedJustification()
		{
		if( checkForUnreferencedJustification( true ) == Constants.RESULT_OK )
			return checkForUnreferencedJustification( false );

		return Constants.RESULT_OK;
		}

	protected byte checkForReplacedOrDeletedSpecification()
		{
		JustificationItem searchJustificationItem;

		if( ( searchJustificationItem = firstActiveJustificationItem() ) != null )
			{
			do	;
			while( searchJustificationItem.checkForReplacedOrDeletedSpecification() == Constants.RESULT_OK &&
			( searchJustificationItem = searchJustificationItem.nextJustificationItem() ) != null );
			}

		if( ( searchJustificationItem = firstReplacedJustificationItem() ) != null )
			{
			do	;
			while( searchJustificationItem.checkForDeletedSpecifications() == Constants.RESULT_OK &&
			( searchJustificationItem = searchJustificationItem.nextJustificationItem() ) != null );
			}

		return CommonVariables.result;
		}

	protected byte checkJustificationForUsage( JustificationItem unusedJustificationItem )
		{
		if( checkJustificationForUsage( true, unusedJustificationItem ) == Constants.RESULT_OK )
			{
			if( unusedJustificationItem != null &&
			unusedJustificationItem.isReplacedItem() )
				return checkJustificationForUsage( false, unusedJustificationItem );
			}

		return CommonVariables.result;
		}

	protected byte checkSpecificationItemForUsage( SpecificationItem unusedSpecificationItem )
		{
		if( checkSpecificationItemForUsage( true, unusedSpecificationItem ) == Constants.RESULT_OK )
			return checkSpecificationItemForUsage( false, unusedSpecificationItem );

		return CommonVariables.result;
		}

	protected byte replaceJustification( boolean isExclusiveGeneralization, JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem, SpecificationItem involvedSpecificationItem, SpecificationItem replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		boolean hasCorrectedAssumptionByKnowledge;
		boolean hasCorrectedAssumptionByOppositeQuestion;
		JustificationItem attachedJustificationItem;
		SpecificationItem relatedSpecificationItem;
		WordItem generalizationWordItem = myWordItem();

		if( obsoleteJustificationItem == null )
			return startError( 1, null, "The given obsolete justification item is undefined" );

		if( involvedSpecificationItem == null )
			return startError( 1, null, "The given involved specification item is undefined" );

		if( obsoleteJustificationItem.replacingJustificationItem != null )
			return startError( 1, null, "The given obsolete justification item is already replaced" );

		if( replacingJustificationItem != null &&
		replacingJustificationItem.replacingJustificationItem != null )
			return startError( 1, null, "The given replacing justification item has a replacing justification item itself" );

		if( obsoleteJustificationItem == replacingJustificationItem )
			return startError( 1, null, "The given obsolete justification item and the given replacing justification item are the same justification item" );

		hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();
		hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem.hasCorrectedAssumptionByOppositeQuestion();

		// Corrected assumption
		if( ( hasCorrectedAssumptionByKnowledge ||
		hasCorrectedAssumptionByOppositeQuestion ) &&

		replacingJustificationItem != null &&
		replacingJustificationItem.isGeneralizationAssumption() )
			{
			if( replacingJustificationItem.primarySpecificationWordItem() == involvedSpecificationItem.specificationWordItem() )
				{
				if( generalizationWordItem.writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, involvedSpecificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to write an adjusted specification" );

				// Corrected negative assumption
				if( ( specificationResult = generalizationWordItem.findRelatedSpecification( false, involvedSpecificationItem.isAssignment(), involvedSpecificationItem.isArchivedAssignment(), involvedSpecificationItem.isExclusiveSpecification(), true, involvedSpecificationItem.isPossessive(), Constants.NO_QUESTION_PARAMETER, involvedSpecificationItem.specificationCollectionNr(), involvedSpecificationItem.generalizationContextNr(), involvedSpecificationItem.specificationContextNr(), involvedSpecificationItem.relationContextNr(), involvedSpecificationItem.specificationWordItem() ) ).result != Constants.RESULT_OK )
					return addError( 1, null, "I failed to find a negative related specification" );

				if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null )
					{
					if( generalizationWordItem.writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, relatedSpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to write a corrected negative assumption" );

					if( hasCorrectedAssumptionByOppositeQuestion )
						{
						if( generalizationWordItem.replaceOrDeleteSpecification( false, relatedSpecificationItem, involvedSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to replace a corrected negative assumption" );
						}
					}
				}

			if( ( attachedJustificationItem = replacingJustificationItem.attachedJustificationItem() ) != null )
				{
				if( !hasCorrectedAssumptionByKnowledge ||
				!attachedJustificationItem.isUpdatedPrimarySpecificationWordSpanishAmbiguous() )
					{
					if( replaceOrDeleteJustification( replacingJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to delete a corrected justification" );

					replacingJustificationItem = null;
					}
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
		!replacingJustificationItem.hasAttachedJustification() )
			{
			if( replacingJustificationItem.changeAttachedJustification( attachedJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to change an attached justification item" );
			}

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
					return addError( 1, null, "I failed to replace an active justification item" );

				if( generalizationWordItem.updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to update a justification in specifications of my word" );

				if( replacingJustificationItem != null )
					{
					if( updateReplacedJustifications( obsoleteJustificationItem, replacingJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to update the replacing justification items of the replaced justification items" );

					obsoleteJustificationItem.replacingJustificationItem = replacingJustificationItem;
					}
				}
			else
				{
				if( hasCorrectedAssumptionByOppositeQuestion )
					{
					if( replaceJustificationOfCorrectedAssumptionByOppositeQuestion( obsoleteJustificationItem, replacingJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to replace the justification of a corrected assumption by opposite question" );
					}
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
				if( involvedSpecificationItem.changeFirstJustification( attachedJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given obsolete justification item" );
				}
			else
				{
				if( hasCorrectedAssumptionByKnowledge ||
				hasCorrectedAssumptionByOppositeQuestion ||
				!generalizationWordItem.isJustificationInUse( attachedJustificationItem ) )
					{
					if( replaceOrDeleteJustification( attachedJustificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to replace the attached justification item" );
					}
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( obsoleteJustificationItem == null )
			return startError( 1, null, "The given obsolete justification item is undefined" );

		if( obsoleteJustificationItem.hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to delete an obsolete justification" );
			}
		else
			{
			if( replaceItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to replace an obsolete justification" );
			}

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		if( storeChangesInFutureDatabase( firstActiveJustificationItem() ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to store active justification items in the database" );

		if( storeChangesInFutureDatabase( firstReplacedJustificationItem() ) != Constants.RESULT_OK )
			return addError( 1, null, "I failed to store replaced justification items in the database" );

		return Constants.RESULT_OK;
		}
*/
	protected byte updateSpecificationsInJustifications( boolean isMainWord, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();
		boolean hasCorrectedAssumption;
		boolean hasCorrectedAssumptionByKnowledge;
		boolean isReplacingPrimarySpecification;
		boolean isReplacingAnotherPrimarySpecification;
		boolean isReplacingSecondarySpecification;
		boolean isReplacingAnotherSecondarySpecification;
		boolean hasObsoleteSpecificationItemRelationContext;
		boolean hasReplacingSpecificationItemRelationContext = false;
		boolean isExclusiveGeneralization = false;
		boolean isReplacingSpecificationWordSpanishAmbiguous = false;
		JustificationItem createdJustificationItem = null;
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem;
		WordItem generalizationWordItem = myWordItem();

		if( obsoleteSpecificationItem == null )
			return startError( 1, null, "The given obsolete specification item is undefined" );

		hasCorrectedAssumption = generalizationWordItem.hasCorrectedAssumption();
		hasCorrectedAssumptionByKnowledge = generalizationWordItem.hasCorrectedAssumptionByKnowledge();
		hasObsoleteSpecificationItemRelationContext = obsoleteSpecificationItem.hasRelationContext();

		if( replacingSpecificationItem != null )
			{
			hasReplacingSpecificationItemRelationContext = replacingSpecificationItem.hasRelationContext();

			if( !obsoleteSpecificationItem.isQuestion() &&
			replacingSpecificationItem.hasExclusiveGeneralizationCollection() )
				isExclusiveGeneralization = true;

			if( replacingSpecificationItem.isSpecificationWordSpanishAmbiguous() )
				isReplacingSpecificationWordSpanishAmbiguous = true;
			}

		while( searchJustificationItem != null )
			{
			primarySpecificationItem = searchJustificationItem.primarySpecificationItem();
			anotherPrimarySpecificationItem = searchJustificationItem.anotherPrimarySpecificationItem();
			secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem();
			anotherSecondarySpecificationItem = searchJustificationItem.anotherSecondarySpecificationItem();

			isReplacingPrimarySpecification = ( primarySpecificationItem == obsoleteSpecificationItem );
			isReplacingAnotherPrimarySpecification = ( anotherPrimarySpecificationItem == obsoleteSpecificationItem );
			isReplacingSecondarySpecification = ( secondarySpecificationItem == obsoleteSpecificationItem );
			isReplacingAnotherSecondarySpecification = ( anotherSecondarySpecificationItem == obsoleteSpecificationItem );

			if( ( isReplacingPrimarySpecification ||
			isReplacingAnotherPrimarySpecification ||
			isReplacingSecondarySpecification ||
			isReplacingAnotherSecondarySpecification ) &&

			( isMainWord ||
			hasReplacingSpecificationItemRelationContext ||
			!hasObsoleteSpecificationItemRelationContext ||
			replacingSpecificationItem == null ) )
				{
				if( replacingSpecificationItem != null &&
				searchJustificationItem.hasCurrentCreationSentenceNr() )
					{
					if( isReplacingPrimarySpecification )
						{
						if( searchJustificationItem.changePrimarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to change the primary specificationItem item of a justification item" );
						}

					if( isReplacingAnotherPrimarySpecification )
						{
						if( searchJustificationItem.changeAnotherPrimarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to change the another primary specificationItem item of a justification item" );
						}

					if( isReplacingSecondarySpecification )
						{
						if( searchJustificationItem.changeSecondarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to change the secondary specificationItem item of a justification item" );
						}

					if( isReplacingAnotherSecondarySpecification )
						{
						if( searchJustificationItem.changeAnotherSecondarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to change the another secondary specificationItem item of a justification item" );
						}

					searchJustificationItem = firstActiveJustificationItem();
					}
				else
					{
					createdJustificationItem = null;

					if( replacingSpecificationItem != null &&

					( searchJustificationItem.isQuestionJustification() ||
					// Avoid creating unreferenced justifications
					generalizationWordItem.isJustificationInUse( searchJustificationItem ) ) )
						{
						if( ( justificationResult = addJustification( searchJustificationItem.hasFeminineOrMasculineProperNameEnding(), ( isReplacingSpecificationWordSpanishAmbiguous && isReplacingSecondarySpecification && searchJustificationItem.isPossessiveReversibleAssumptionOrConclusion() ), searchJustificationItem.justificationTypeNr(), searchJustificationItem.orderNr, searchJustificationItem.originalSentenceNr(), ( isReplacingPrimarySpecification ? replacingSpecificationItem : primarySpecificationItem ), ( isReplacingAnotherPrimarySpecification ? replacingSpecificationItem : anotherPrimarySpecificationItem ), ( isReplacingSecondarySpecification ? replacingSpecificationItem : secondarySpecificationItem ), ( isReplacingAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem ), searchJustificationItem.attachedJustificationItem() ) ).result != Constants.RESULT_OK )
							return addError( 1, null, "I failed to add a justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
							{
							if( justificationResult.foundJustificationItem == searchJustificationItem )
								return addError( 1, null, "No justification item is created, and the found justification item is the same as the search item itself" );

							createdJustificationItem = justificationResult.foundJustificationItem;
							}
						}

					if( replaceJustification( isExclusiveGeneralization, searchJustificationItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : null ) ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to replace a justification item" );

					searchJustificationItem = ( hasCorrectedAssumption ? searchJustificationItem.nextJustificationItem() : firstActiveJustificationItem() );

					if( hasCorrectedAssumptionByKnowledge &&
					searchJustificationItem != null &&
					searchJustificationItem.hasReplacedPrimarySpecification() )
						searchJustificationItem = firstActiveJustificationItem();
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
			( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null )
				{
				if( !secondarySpecificationItem.isExclusiveSpecification() &&
				secondarySpecificationItem.specificationCollectionNr() == specificationCollectionNr )
					{
					if( ( generalizationWordItem = secondarySpecificationItem.generalizationWordItem() ) == null )
						return startError( 1, null, "The generalization word of the given secondary specification item is undefined" );

					if( generalizationWordItem.writeJustificationSpecification( false, secondarySpecificationItem ) != Constants.RESULT_OK )
						return addError( 1, null, "I failed to write a secondary justification specification" );
					}
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

	protected JustificationItem obsoleteJustificationItem()
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		WordItem generalizationWordItem = myWordItem();

		while( searchJustificationItem != null )
			{
			if( !generalizationWordItem.isJustificationInUse( searchJustificationItem ) )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationItem olderComplexJustificationItem( boolean hasSecondarySpecificationWithoutRelationContext, short justificationTypeNr, int secondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		while( searchJustificationItem != null )
			{
			if( searchJustificationItem.justificationTypeNr() == justificationTypeNr &&

			( searchJustificationItem.isOlderItem() ||

			( !hasSecondarySpecificationWithoutRelationContext &&
			searchJustificationItem.hasPrimaryUserSpecification() ) ) &&

			( primarySpecificationItem == null ||
			searchJustificationItem.primarySpecificationItem() == primarySpecificationItem ||

			// If primary specifications have different relation context
			( searchJustificationItem.primarySpecificationItem() != null &&
			searchJustificationItem.primarySpecificationItem().generalizationWordItem() == primarySpecificationItem.generalizationWordItem() &&
			searchJustificationItem.primarySpecificationItem().isRelatedSpecification( primarySpecificationItem.isExclusiveSpecification(), primarySpecificationItem.isNegative(), primarySpecificationItem.isPossessive(), primarySpecificationItem.specificationWordItem() ) ) ) &&

			( secondarySpecificationItem = searchJustificationItem.secondarySpecificationItem() ) != null )
				{
				if( ( !hasSecondarySpecificationWithoutRelationContext ||
				!secondarySpecificationItem.hasRelationContext() ) &&

				( secondarySpecificationCollectionNr == Constants.NO_COLLECTION_NR ||

				( !secondarySpecificationItem.isConcludedAssumption() &&
				secondarySpecificationItem.specificationCollectionNr() == secondarySpecificationCollectionNr ) ) )
					return searchJustificationItem;
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

	protected JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchJustificationItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != null )
			{
			while( searchJustificationItem != null )
				{
				if( searchJustificationItem.isSpecificationSubstitutionPartOfAssumption() &&
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
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
