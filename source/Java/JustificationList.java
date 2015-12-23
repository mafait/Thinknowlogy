/*
 *	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items for the
 *					self-generated knowledge
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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
	// Private constructible variables

	private JustificationItem correctedAssumptionByOppositeQuestionJustificationItem_;


	// Private methods

	private short highestJustificationOrderNr( short justificationTypeNr )
		{
		short highestOrderNr = Constants.NO_ORDER_NR;
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.orderNr > highestOrderNr &&
			searchItem.justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchItem.orderNr;

			searchItem = searchItem.nextJustificationItem();
			}

		return highestOrderNr;
		}

	private short justificationOrderNr( boolean hasFeminineOrMasculineProperNameEnding, boolean hasPossessivePrimarySpecification, short justificationTypeNr, int primarySpecificationCollectionNr )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( primarySpecificationCollectionNr > Constants.NO_COLLECTION_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.justificationTypeNr() == justificationTypeNr &&
				searchItem.hasPossessivePrimarySpecification() == hasPossessivePrimarySpecification &&
				searchItem.primarySpecificationCollectionNr() == primarySpecificationCollectionNr &&
				searchItem.hasFeminineOrMasculineProperNameEnding() == hasFeminineOrMasculineProperNameEnding )
					return searchItem.orderNr;

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return Constants.NO_ORDER_NR;
		}

	private short questionJustificationOrderNr( SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( primarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isQuestionJustification() &&
				searchItem.primarySpecificationItem() == primarySpecificationItem &&
				searchItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem )
					return searchItem.orderNr;

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return Constants.NO_ORDER_NR;
		}

	private byte checkForUnreferencedJustifications( boolean isActiveItem )
		{
		JustificationItem searchItem = firstJustificationItem( isActiveItem );

		while( searchItem != null )
			{
			if( !myWordItem().isJustificationInUse( searchItem ) )
				return startError( 1, null, "I have found an unreferenced active justification item" );

			searchItem = searchItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	private byte checkJustificationForUsage( boolean isActiveItem, JustificationItem unusedJustificationItem )
		{
		JustificationItem searchItem = firstJustificationItem( isActiveItem );

		if( unusedJustificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.attachedJustificationItem() == unusedJustificationItem )
					return startError( 1, null, "I have found an attached justification item that is still in use" );

				if( searchItem.replacingJustificationItem == unusedJustificationItem )
					return startError( 1, null, "I have found a replacing justification item that is still in use" );

				searchItem = searchItem.nextJustificationItem();
				}
			}
		else
			return startError( 1, null, "The given unused justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte checkSpecificationItemForUsage( boolean isActiveItem, SpecificationItem unusedSpecificationItem )
		{
		JustificationItem searchItem = firstJustificationItem( isActiveItem );

		if( unusedSpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.primarySpecificationItem() == unusedSpecificationItem )
					return startError( 1, null, "The primary specification item is still in use" );

				if( searchItem.anotherPrimarySpecificationItem() == unusedSpecificationItem )
					return startError( 1, null, "The another primary specification item is still in use" );

				if( searchItem.secondarySpecificationItem() == unusedSpecificationItem )
					return startError( 1, null, "The secondary specification item is still in use" );

				if( searchItem.anotherSecondarySpecificationItem() == unusedSpecificationItem )
					return startError( 1, null, "The another secondary specification item is still in use" );

				searchItem = searchItem.nextJustificationItem();
				}
			}
		else
			return startError( 1, null, "The given unused specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem obsoleteJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem attachedJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( obsoleteJustificationItem != null )
			{
			if( replacingJustificationItem != null )
				{
				correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

				if( ( secondarySpecificationWordItem = replacingJustificationItem.secondarySpecificationWordItem() ) != null )
					{
					if( ( foundSpecificationItem = myWordItem().firstAssignmentOrSpecificationItem( false, false, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != null )
						{
						if( foundSpecificationItem.attachJustificationToSpecification( replacingJustificationItem ) == Constants.RESULT_OK )
							{
							foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

							if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null )
								{
								if( ( attachedJustificationItem = firstJustificationItem.attachedJustificationItem() ) != null )
									{
									// Remove the obsolete justification item from the replacing justification item
									if( firstJustificationItem.changeAttachedJustification( null ) == Constants.RESULT_OK )
										{
										if( replaceOrDeleteJustification( attachedJustificationItem ) == Constants.RESULT_OK )
											{
											if( ( justificationResult = copyJustificationItem( attachedJustificationItem.primarySpecificationItem(), attachedJustificationItem.secondarySpecificationItem(), null, attachedJustificationItem ) ).result == Constants.RESULT_OK )
												{
												if( justificationResult.createdJustificationItem != null )
													{
													if( foundSpecificationItem.attachJustificationToSpecification( justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
														return addError( 1, null, "I failed to attach the created justification item to the found specification" );
													}
												else
													return startError( 1, null, "I couldn't find or create a justification" );
												}
											else
												return addError( 1, null, "I failed to copy the attached justification item" );
											}
										else
											return addError( 1, null, "I failed to replace the attached justification item, without an attached justification item" );
										}
									else
										return addError( 1, null, "I failed to remove the attached justification item of the replacing justification item" );
									}
								}
							}
						else
							return addError( 1, null, "I failed to attach the replacing justification item to the found specification" );
						}
					}
				else
					return startError( 1, null, "The given replacing justification item has no secondary specification word item" );
				}
			else
				return startError( 1, null, "The given replacing justification item is undefined" );
			}
		else
			return startError( 1, null, "The given obsolete justification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte updateReplacedJustifications( JustificationItem updateJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationItem searchItem = firstReplacedJustificationItem();

		if( updateJustificationItem != null )
			{
			if( replacingJustificationItem != null )
				{
				if( !replacingJustificationItem.isReplacedItem() )
					{
					while( searchItem != null )
						{
						if( searchItem.replacingJustificationItem == updateJustificationItem )
							searchItem.replacingJustificationItem = replacingJustificationItem;

						searchItem = searchItem.nextJustificationItem();
						}
					}
				else
					return startError( 1, null, "The replaced justification item of the given update justification item is a replaced item" );
				}
			else
				return startError( 1, null, "The given replacing justification item is undefined" );
			}
		else
			return startError( 1, null, "The given update justification item is undefined" );

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
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.primarySpecificationItem() == primarySpecificationItem &&
			searchItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchItem.secondarySpecificationItem() == secondarySpecificationItem &&
			searchItem.anotherSecondarySpecificationItem() == anotherSecondarySpecificationItem &&
			searchItem.hasFeminineOrMasculineProperNameEnding() == hasFeminineOrMasculineProperNameEnding )
				return searchItem;

			searchItem = searchItem.nextJustificationItem();
			}

		return null;
		}


	// Constructor / deconstructor

	protected JustificationList( WordItem myWordItem )
		{
		// Private constructible variables

		correctedAssumptionByOppositeQuestionJustificationItem_ = null;

		initializeListVariables( Constants.WORD_JUSTIFICATION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void clearJustificationHasBeenWritten()
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			searchItem.hasJustificationBeenWritten = false;
			searchItem = searchItem.nextJustificationItem();
			}
		}

	protected void initializeVariables()
		{
		correctedAssumptionByOppositeQuestionJustificationItem_ = null;
		}

	protected JustificationResultType addJustification( boolean hasFeminineOrMasculineProperNameEnding, boolean isForcingNewJustification, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();
		boolean isExclusivePrimarySpecification = false;
		boolean isNeedingIncrement = false;
		short foundJustificationOrderNr;
		int primarySpecificationCollectionNr;
		JustificationItem foundJustificationItem = null;
		SpecificationItem updatedPrimarySpecificationItem = ( primarySpecificationItem == null ? null : primarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedAnotherPrimarySpecificationItem = ( anotherPrimarySpecificationItem == null ? null : anotherPrimarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedSecondarySpecificationItem = ( secondarySpecificationItem == null ? null : secondarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedAnotherSecondarySpecificationItem = ( anotherSecondarySpecificationItem == null ? null : anotherSecondarySpecificationItem.updatedSpecificationItem() );

		if( primarySpecificationItem != null ||
		secondarySpecificationItem != null )
			{
			// Skipping search, forcing to create a new justification item
			if( !isForcingNewJustification )
				foundJustificationItem = getJustificationItem( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem );

			if( foundJustificationItem == null ||

			// Exceptions
			( foundJustificationItem.justificationTypeNr() == justificationTypeNr &&

			( ( attachedJustificationItem != null ||
			foundJustificationItem.isPossessiveReversibleConclusion() ) &&

			foundJustificationItem.attachedJustificationItem() != attachedJustificationItem ) ) )
				{
				if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
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

										( ( updatedPrimarySpecificationItem.isSpecificationWordCollectedWithItself() &&
										updatedPrimarySpecificationItem.hasRelationContext() &&
										updatedPrimarySpecificationItem.generalizationWordItem() == myWordItem() ) ||

										( hasFeminineOrMasculineProperNameEnding &&
										updatedAnotherPrimarySpecificationItem != null &&
										updatedAnotherPrimarySpecificationItem.isSpecificationWordCollectedWithItself() ) ) ) &&

										( primarySpecificationCollectionNr = updatedPrimarySpecificationItem.specificationCollectionNr() ) > Constants.NO_COLLECTION_NR )
											{
											if( ( foundJustificationOrderNr = justificationOrderNr( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem.isPossessive(), justificationTypeNr, primarySpecificationCollectionNr ) ) == Constants.NO_ORDER_NR )
												isNeedingIncrement = true;
											else
												orderNr = foundJustificationOrderNr;
											}

										break;

									case Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
										// Collected with itself
										if( updatedAnotherSecondarySpecificationItem == null )
											{
											if( ( foundJustificationOrderNr = questionJustificationOrderNr( updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem ) ) == Constants.NO_ORDER_NR )
												isNeedingIncrement = true;
											else
												orderNr = foundJustificationOrderNr;
											}
									}
								}

							if( CommonVariables.result == Constants.RESULT_OK &&
							!isExclusivePrimarySpecification &&

							( orderNr == Constants.NO_ORDER_NR ||
							isNeedingIncrement ||
							justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
							justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

							( updatedPrimarySpecificationItem != null &&

							( ( justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&

							// Skip self-generated question of corrected assumption by opposite question
							( updatedAnotherPrimarySpecificationItem == null ||
							updatedPrimarySpecificationItem.hasRelationContext() ) ) ||

							( updatedPrimarySpecificationItem.isSpecificationWordCollectedWithItself() &&

							( justificationTypeNr == Constants.JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||

							( hasFeminineOrMasculineProperNameEnding &&
							justificationTypeNr == Constants.JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION &&
							updatedPrimarySpecificationItem.isPossessive() &&
							// If justification without feminine or masculine proper name ending exists
							getJustificationItem( false, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem ) != null ) ) ) ) ) ) )
								orderNr++;
							}
						}

					if( CommonVariables.result == Constants.RESULT_OK )
						{
						if( orderNr > Constants.NO_ORDER_NR )
							{
							if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNameEnding, justificationTypeNr, orderNr, originalSentenceNr, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem, attachedJustificationItem, this, myWordItem() ) ) != null )
								{
								// Add justification item
								if( addItemToList( Constants.QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
									addError( 1, null, "I failed to add an active justification item" );
								}
							else
								startError( 1, null, "I failed to create a justification item" );
							}
						else
							startError( 1, null, "The order number is undefined" );
						}
					}
				else
					startError( 1, null, "The current item number is undefined" );
				}
			}
		else
			startError( 1, null, "Both the given primary specification item and the given secondary specification item are undefined" );

		justificationResult.foundJustificationItem = foundJustificationItem;
		justificationResult.result = CommonVariables.result;
		return justificationResult;
		}

	protected JustificationResultType copyJustificationItem( SpecificationItem newPrimarySpecificationItem, SpecificationItem newSecondarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		if( originalJustificationItem != null )
			return addJustification( originalJustificationItem.hasFeminineOrMasculineProperNameEnding(), false, originalJustificationItem.justificationTypeNr(), originalJustificationItem.orderNr, originalJustificationItem.originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem.anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem.anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		justificationResult.result = startError( 1, null, "The given original justification item is undefined" );
		return justificationResult;
		}

	protected byte attachJustification( JustificationItem attachJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		SpecificationResultType specificationResult;
		boolean hasPrimarySpecificationWordCollectedWithItself;
		boolean isSkipUpdateJustification = false;
		short highestOrderNr;
		short justificationTypeNr;
		short previousAssumptionLevel;
		JustificationItem attachedJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem lastAttachedJustificationItem;
		JustificationItem obsoleteJustificationItem = null;
		SpecificationItem attachedPrimarySpecificationItem;
		SpecificationItem attachedSecondarySpecificationItem;

		if( attachJustificationItem != null )
			{
			if( involvedSpecificationItem != null )
				{
				if( attachJustificationItem.isActiveItem() )
					{
					if( !involvedSpecificationItem.isReplacedOrDeletedItem() )
						{
						justificationTypeNr = attachJustificationItem.justificationTypeNr();
						hasPrimarySpecificationWordCollectedWithItself = attachJustificationItem.hasPrimarySpecificationWordCollectedWithItself();

						attachedPrimarySpecificationItem = attachJustificationItem.primarySpecificationItem();
						attachedSecondarySpecificationItem = attachJustificationItem.secondarySpecificationItem();

						if( attachedPrimarySpecificationItem != null &&

						( ( !involvedSpecificationItem.hasRelationContext() &&
						!involvedSpecificationItem.isExclusiveSpecification() &&
						!attachedPrimarySpecificationItem.isExclusiveSpecification() &&

						( attachedPrimarySpecificationItem.hasRelationContext() ||
						!attachedPrimarySpecificationItem.isPossessive() ) ) ||

						( ( !hasPrimarySpecificationWordCollectedWithItself ||

						( involvedSpecificationItem.isQuestion() &&
						attachedPrimarySpecificationItem.isPossessive() ) ) &&

						!involvedSpecificationItem.hasFoundJustificationOfSameType( justificationTypeNr, attachedPrimarySpecificationItem, attachedSecondarySpecificationItem ) ) ) )
							{
							if( ( highestOrderNr = involvedSpecificationItem.highestAttachedJustificationOrderNr( justificationTypeNr ) ) < Constants.MAX_ORDER_NR )
								attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
							else
								return startSystemError( 1, null, "Justification order number overflow" );
							}

						if( ( firstJustificationItem = involvedSpecificationItem.firstJustificationItem() ) != null )
							{
							if( attachJustificationItem.secondarySpecificationQuestion() == null &&
							( obsoleteJustificationItem = firstJustificationItem.secondarySpecificationQuestion() ) == null )
								{
								if( attachedPrimarySpecificationItem != null &&
								!attachedPrimarySpecificationItem.isPossessive() &&
								attachedPrimarySpecificationItem.hasRelationContext() )
									obsoleteJustificationItem = firstJustificationItem.primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem );
								else
									{
									if( attachedSecondarySpecificationItem != null &&
									attachedSecondarySpecificationItem.isUserSpecification() )
										obsoleteJustificationItem = firstJustificationItem.selfGeneratedSecondarySpecificationJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
									}

								if( obsoleteJustificationItem != null )
									{
									if( obsoleteJustificationItem.isOlderItem() )
										{
										if( !involvedSpecificationItem.hasRelationContext() &&

										( !hasPrimarySpecificationWordCollectedWithItself ||

										( attachedPrimarySpecificationItem != null &&
										attachedPrimarySpecificationItem.hasAssumptionLevel() ) ) )
											isSkipUpdateJustification = true;
										else
											{
											if( replaceJustification( false, obsoleteJustificationItem, attachJustificationItem, involvedSpecificationItem, null ) == Constants.RESULT_OK )
												attachJustificationItem.orderNr = obsoleteJustificationItem.orderNr;
											else
												return addError( 1, null, "I failed to replace the obsolete justification item" );
											}
										}
									else
										{
										obsoleteJustificationItem = null;

										if( ( highestOrderNr = involvedSpecificationItem.highestAttachedJustificationOrderNr( justificationTypeNr ) ) < Constants.MAX_ORDER_NR )
											attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
										else
											return startSystemError( 1, null, "Justification order number overflow" );
										}
									}
								}

							if( obsoleteJustificationItem == null )
								{
								// Find a justification item with current sentence number, but without attached justification item
								lastAttachedJustificationItem = firstJustificationItem;

								while( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() &&
								( attachedJustificationItem = lastAttachedJustificationItem.attachedJustificationItem() ) != null )
									lastAttachedJustificationItem = attachedJustificationItem;

								if( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() )
									{
									if( ( specificationResult = involvedSpecificationItem.getAssumptionLevel() ).result == Constants.RESULT_OK )
										{
										previousAssumptionLevel = specificationResult.assumptionLevel;

										if( lastAttachedJustificationItem.attachJustification( attachJustificationItem, involvedSpecificationItem ) == Constants.RESULT_OK )
											{
											if( ( involvedSpecificationItem.isConcludedAssumption() ||

											( justificationTypeNr != Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION &&
											justificationTypeNr != Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) &&

											// Skip on multiple relation context words
											( involvedSpecificationItem.isOlderItem() ||
											!involvedSpecificationItem.hasRelationContext() ||
											myWordItem().nContextWordsInAllWords( involvedSpecificationItem.relationContextNr(), involvedSpecificationItem.specificationWordItem() ) == 1 ) )
												{
												if( ( specificationResult = involvedSpecificationItem.recalculateAssumptionLevel() ).result == Constants.RESULT_OK )
													{
													if( involvedSpecificationItem.isOlderItem() &&
													!involvedSpecificationItem.hasRelationContext() &&
													specificationResult.assumptionLevel != previousAssumptionLevel &&

													// Avoid updates of high uncertainty mapping,
													// from 'maybe' of one level to 'maybe' of another level
													( previousAssumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ||
													specificationResult.assumptionLevel < Constants.NUMBER_OF_ASSUMPTION_LEVELS ) )
														{
														// Conclusion
														if( specificationResult.assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
															{
															if( involvedSpecificationItem.markAsConcludedAssumption() != Constants.RESULT_OK )
																return addError( 1, null, "I failed to mark the involved specification as concluded assumption" );
															}

														// Write adjusted specification
														if( myWordItem().writeUpdatedSpecification( true, false, false, false, involvedSpecificationItem ) != Constants.RESULT_OK )
															return addError( 1, null, "I failed to write an adjusted specification" );
														}
													}
												else
													return addError( 1, null, "I failed to recalculate the assumption level" );
												}
											}
										else
											return addError( 1, null, "I failed to attach a justification item" );
										}
									else
										return addError( 1, null, "I failed to get the assumption level" );
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
								if( !isSkipUpdateJustification )
									{
									if( myWordItem().updateJustificationInSpecifications( false, obsoleteJustificationItem, attachJustificationItem ) != Constants.RESULT_OK )
										return addError( 1, null, "I failed to update a question justification item by a conclusion justification item" );
									}
								}
							}
						else
							return startError( 1, null, "The given involved specification item has no justification item" );
						}
					else
						return startError( 1, null, "The given involved specification item is replaced or deleted" );
					}
				else
					return startError( 1, null, "The given attached justification item isn't active" );
				}
			else
				return startError( 1, null, "The given involved specification item is undefined" );
			}
		else
			return startError( 1, null, "The given attached justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkForUnreferencedJustifications()
		{
		if( checkForUnreferencedJustifications( true ) == Constants.RESULT_OK )
			return checkForUnreferencedJustifications( false );

		return CommonVariables.result;
		}

	protected byte checkForReplacedOrDeletedSpecifications()
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		searchItem != null )
			{
			searchItem.checkForReplacedOrDeletedSpecifications();
			searchItem = searchItem.nextJustificationItem();
			}

		searchItem = firstReplacedJustificationItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		searchItem != null )
			{
			searchItem.checkForDeletedSpecifications();
			searchItem = searchItem.nextJustificationItem();
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
		boolean hasCorrectedAssumption;
		boolean hasCorrectedAssumptionByKnowledge = myWordItem().hasCorrectedAssumptionByKnowledge();
		boolean hasCorrectedAssumptionByOppositeQuestion = myWordItem().hasCorrectedAssumptionByOppositeQuestion();
		JustificationItem attachedJustificationItem;
		SpecificationItem relatedSpecificationItem;

		if( obsoleteJustificationItem != null )
			{
			if( involvedSpecificationItem != null )
				{
				if( obsoleteJustificationItem.replacingJustificationItem == null )
					{
					if( replacingJustificationItem == null ||
					replacingJustificationItem.replacingJustificationItem == null )
						{
						if( obsoleteJustificationItem != replacingJustificationItem )
							{
							hasCorrectedAssumption = ( hasCorrectedAssumptionByKnowledge ||
														hasCorrectedAssumptionByOppositeQuestion );

							// Corrected assumption
							if( hasCorrectedAssumption &&
							replacingJustificationItem != null &&
							replacingJustificationItem.isGeneralizationAssumption() )
								{
								if( replacingJustificationItem.primarySpecificationWordItem() == involvedSpecificationItem.specificationWordItem() )
									{
									if( myWordItem().writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, involvedSpecificationItem ) == Constants.RESULT_OK )
										{
										// Corrected negative assumption
										if( ( specificationResult = myWordItem().findRelatedSpecification( false, involvedSpecificationItem.isAssignment(), involvedSpecificationItem.isArchivedAssignment(), involvedSpecificationItem.isExclusiveSpecification(), true, involvedSpecificationItem.isPossessive(), Constants.NO_QUESTION_PARAMETER, involvedSpecificationItem.specificationCollectionNr(), involvedSpecificationItem.generalizationContextNr(), involvedSpecificationItem.specificationContextNr(), involvedSpecificationItem.relationContextNr(), involvedSpecificationItem.specificationWordItem() ) ).result == Constants.RESULT_OK )
											{
											if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null )
												{
												if( myWordItem().writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, relatedSpecificationItem ) == Constants.RESULT_OK )
													{
													if( hasCorrectedAssumptionByOppositeQuestion )
														{
														if( myWordItem().replaceOrDeleteSpecification( relatedSpecificationItem, involvedSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
															return addError( 1, null, "I failed to replace a corrected negative assumption" );
														}
													}
												else
													return addError( 1, null, "I failed to write a corrected negative assumption" );
												}
											}
										else
											return addError( 1, null, "I failed to find a negative related specification" );
										}
									else
										return addError( 1, null, "I failed to write an adjusted specification" );
									}

								if( ( attachedJustificationItem = replacingJustificationItem.attachedJustificationItem() ) != null )
									{
									if( !hasCorrectedAssumptionByKnowledge ||
									!attachedJustificationItem.hasUpdatedPrimarySpecificationWordCollectedWithItself() )
										{
										if( replaceOrDeleteJustification( replacingJustificationItem ) == Constants.RESULT_OK )
											replacingJustificationItem = null;
										else
											return addError( 1, null, "I failed to delete a corrected justification" );
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
									if( replaceOrDeleteJustification( obsoleteJustificationItem ) == Constants.RESULT_OK )
										{
										if( myWordItem().updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
											{
											if( replacingJustificationItem != null )
												{
												if( updateReplacedJustifications( obsoleteJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
													obsoleteJustificationItem.replacingJustificationItem = replacingJustificationItem;
												else
													return addError( 1, null, "I failed to update the replacing justification items of the replaced justification items" );
												}
											}
										else
											return addError( 1, null, "I failed to update a justification in specifications of my word" );
										}
									else
										return addError( 1, null, "I failed to replace an active justification item" );
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
									if( hasCorrectedAssumption ||
									!myWordItem().isJustificationInUse( attachedJustificationItem ) )
										{
										if( replaceOrDeleteJustification( attachedJustificationItem ) != Constants.RESULT_OK )
											return addError( 1, null, "I failed to replace the attached justification item" );
										}
									}
								}
							}
						else
							return startError( 1, null, "The given obsolete justification item and the given replacing justification item are the same justification item" );
						}
					else
						return startError( 1, null, "The given replacing justification item has a replacing justification item itself" );
					}
				else
					return startError( 1, null, "The given obsolete justification item is already replaced" );
				}
			else
				return startError( 1, null, "The given involved specification item is undefined" );
			}
		else
			return startError( 1, null, "The given obsolete justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( obsoleteJustificationItem != null )
			{
			if( obsoleteJustificationItem.hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( false, obsoleteJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to delete an obsolete justification" );
				}
			else
				{
				if( replaceItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, "I failed to replace an obsolete justification" );
				}
			}
		else
			return startError( 1, null, "The given obsolete justification item is undefined" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeJustificationItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a justification item in the database" );
				}

			searchItem = searchItem.nextJustificationItem();
			}

		searchItem = firstReplacedJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeJustificationItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to modify a replaced justification item in the database" );
				}

			searchItem = searchItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected byte updateSpecificationsInJustifications( boolean isMainWord, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();
		boolean isReplacePrimarySpecification;
		boolean isReplaceAnotherPrimarySpecification;
		boolean isReplaceSecondarySpecification;
		boolean isReplaceAnotherSecondarySpecification;
		boolean hasObsoleteSpecificationItemRelationContext;
		boolean hasCorrectedAssumption = myWordItem().hasCorrectedAssumption();
		boolean hasCorrectedAssumptionByKnowledge = myWordItem().hasCorrectedAssumptionByKnowledge();
		boolean hasReplacingSpecificationItemRelationContext = false;
		boolean isExclusiveGeneralization = false;
		boolean isReplacingSpecificationWordCollectedWithItself = false;
		JustificationItem createdJustificationItem = null;
		JustificationItem searchItem = firstActiveJustificationItem();
		SpecificationItem primarySpecificationItem;
		SpecificationItem anotherPrimarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		SpecificationItem anotherSecondarySpecificationItem;

		if( obsoleteSpecificationItem != null )
			{
			hasObsoleteSpecificationItemRelationContext = obsoleteSpecificationItem.hasRelationContext();

			if( replacingSpecificationItem != null )
				{
				hasReplacingSpecificationItemRelationContext = replacingSpecificationItem.hasRelationContext();

				if( !obsoleteSpecificationItem.isQuestion() &&
				replacingSpecificationItem.hasExclusiveGeneralizationCollection() )
					isExclusiveGeneralization = true;

				if( replacingSpecificationItem.isSpecificationWordCollectedWithItself() )
					isReplacingSpecificationWordCollectedWithItself = true;
				}

			while( searchItem != null )
				{
				primarySpecificationItem = searchItem.primarySpecificationItem();
				anotherPrimarySpecificationItem = searchItem.anotherPrimarySpecificationItem();
				secondarySpecificationItem = searchItem.secondarySpecificationItem();
				anotherSecondarySpecificationItem = searchItem.anotherSecondarySpecificationItem();

				isReplacePrimarySpecification = ( primarySpecificationItem == obsoleteSpecificationItem );
				isReplaceAnotherPrimarySpecification = ( anotherPrimarySpecificationItem == obsoleteSpecificationItem );
				isReplaceSecondarySpecification = ( secondarySpecificationItem == obsoleteSpecificationItem );
				isReplaceAnotherSecondarySpecification = ( anotherSecondarySpecificationItem == obsoleteSpecificationItem );

				if( ( isReplacePrimarySpecification ||
				isReplaceAnotherPrimarySpecification ||
				isReplaceSecondarySpecification ||
				isReplaceAnotherSecondarySpecification ) &&

				( isMainWord ||
				hasReplacingSpecificationItemRelationContext ||
				!hasObsoleteSpecificationItemRelationContext ||
				replacingSpecificationItem == null ||
				searchItem.hasFeminineOrMasculineProperNameEnding() ) )
					{
					if( replacingSpecificationItem != null &&
					searchItem.hasCurrentCreationSentenceNr() )
						{
						if( isReplacePrimarySpecification )
							{
							if( searchItem.changePrimarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change the primary specificationItem item of a justification item" );
							}

						if( isReplaceAnotherPrimarySpecification )
							{
							if( searchItem.changeAnotherPrimarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change the another primary specificationItem item of a justification item" );
							}

						if( isReplaceSecondarySpecification )
							{
							if( searchItem.changeSecondarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change the secondary specificationItem item of a justification item" );
							}

						if( isReplaceAnotherSecondarySpecification )
							{
							if( searchItem.changeAnotherSecondarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, "I failed to change the another secondary specificationItem item of a justification item" );
							}

						searchItem = firstActiveJustificationItem();
						}
					else
						{
						createdJustificationItem = null;

						if( replacingSpecificationItem != null &&

						( searchItem.isQuestionJustification() ||
						// Avoid creating unreferenced justifications
						myWordItem().isJustificationInUse( searchItem ) ) )
							{
							if( ( justificationResult = addJustification( searchItem.hasFeminineOrMasculineProperNameEnding(), ( isReplacingSpecificationWordCollectedWithItself && isReplaceSecondarySpecification && searchItem.isPossessiveReversibleAssumptionOrConclusion() ), searchItem.justificationTypeNr(), searchItem.orderNr, searchItem.originalSentenceNr(), ( isReplacePrimarySpecification ? replacingSpecificationItem : primarySpecificationItem ), ( isReplaceAnotherPrimarySpecification ? replacingSpecificationItem : anotherPrimarySpecificationItem ), ( isReplaceSecondarySpecification ? replacingSpecificationItem : secondarySpecificationItem ), ( isReplaceAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem ), searchItem.attachedJustificationItem() ) ).result == Constants.RESULT_OK )
								{
								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
									{
									if( justificationResult.foundJustificationItem != searchItem )
										createdJustificationItem = justificationResult.foundJustificationItem;
									else
										return addError( 1, null, "No justification item is created, and the found justification item is the same as the search item itself" );
									}
								}
							else
								return addError( 1, null, "I failed to add a justification item" );
							}

						if( replaceJustification( isExclusiveGeneralization, searchItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : null ) ) == Constants.RESULT_OK )
							{
							searchItem = ( hasCorrectedAssumption ? searchItem.nextJustificationItem() : firstActiveJustificationItem() );

							if( hasCorrectedAssumptionByKnowledge &&
							searchItem != null &&
							searchItem.hasReplacedPrimarySpecification() )
								searchItem = firstActiveJustificationItem();
							}
						else
							return addError( 1, null, "I failed to replace a justification item" );
						}
					}
				else
					searchItem = searchItem.nextJustificationItem();
				}
			}
		else
			return startError( 1, null, "The given obsolete specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeRelatedJustificationSpecifications( short justificationTypeNr, int specificationCollectionNr )
		{
		JustificationItem searchItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;
		WordItem generalizationWordItem;

		while( searchItem != null )
			{
			if( !searchItem.hasJustificationBeenWritten &&
			searchItem.justificationTypeNr() == justificationTypeNr &&
			( secondarySpecificationItem = searchItem.secondarySpecificationItem() ) != null )
				{
				if( !secondarySpecificationItem.isExclusiveSpecification() &&
				secondarySpecificationItem.specificationCollectionNr() == specificationCollectionNr )
					{
					if( ( generalizationWordItem = secondarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( generalizationWordItem.writeJustificationSpecification( false, secondarySpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, "I failed to write a secondary justification specification" );
						}
					else
						return startError( 1, null, "The generalization word of the given secondary specification item is undefined" );
					}
				}

			searchItem = searchItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected JustificationItem correctedAssumptionByOppositeQuestionJustificationItem()
		{
		return correctedAssumptionByOppositeQuestionJustificationItem_;
		}

	protected JustificationItem negativeAssumptionOrConclusionJustificationItem( SpecificationItem anotherPrimarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( anotherPrimarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isNegativeAssumptionOrConclusion() &&
				searchItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem )
					return searchItem;

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem obsoleteJustificationItem()
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( !myWordItem().isJustificationInUse( searchItem ) )
				return searchItem;

			searchItem = searchItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationItem olderComplexJustificationItem( boolean hasSecondarySpecificationWithoutRelationContext, short justificationTypeNr, int secondarySpecificationCollectionNr, SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		while( searchItem != null )
			{
			if( searchItem.justificationTypeNr() == justificationTypeNr &&

			( searchItem.isOlderItem() ||

			( !hasSecondarySpecificationWithoutRelationContext &&
			searchItem.hasPrimaryUserSpecification() ) ) &&

			( primarySpecificationItem == null ||
			searchItem.primarySpecificationItem() == primarySpecificationItem ||

			// If primary specifications have different relation context
			( searchItem.primarySpecificationItem() != null &&
			searchItem.primarySpecificationItem().generalizationWordItem() == primarySpecificationItem.generalizationWordItem() &&
			searchItem.primarySpecificationItem().isRelatedSpecification( primarySpecificationItem.isExclusiveSpecification(), primarySpecificationItem.isNegative(), primarySpecificationItem.isPossessive(), primarySpecificationItem.specificationWordItem() ) ) ) &&

			( secondarySpecificationItem = searchItem.secondarySpecificationItem() ) != null )
				{
				if( ( !hasSecondarySpecificationWithoutRelationContext ||
				!secondarySpecificationItem.hasRelationContext() ) &&

				( secondarySpecificationCollectionNr == Constants.NO_COLLECTION_NR ||

				( !secondarySpecificationItem.isConcludedAssumption() &&
				secondarySpecificationItem.specificationCollectionNr() == secondarySpecificationCollectionNr ) ) )
					return searchItem;
				}

			searchItem = searchItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationItem possessiveReversibleAssumptionJustificationItem( WordItem secondarySpecificationWordItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( secondarySpecificationWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isPossessiveReversibleAssumption() &&
				searchItem.secondarySpecificationWordItem() == secondarySpecificationWordItem )
					return searchItem;
				searchItem = searchItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem primarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( primarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( ( !isSelectingOlderItemOnly ||
				searchItem.isOlderItem() ) &&

				searchItem.justificationTypeNr() == justificationTypeNr &&
				searchItem.primarySpecificationItem() == primarySpecificationItem )
					return searchItem;

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem secondarySpecificationJustificationItem( boolean isSelectingOlderItemOnly, short justificationTypeNr, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( ( !isSelectingOlderItemOnly ||
				searchItem.isOlderItem() ) &&

				searchItem.justificationTypeNr() == justificationTypeNr &&
				searchItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected JustificationItem specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isSpecificationSubstitutionPartOfAssumption() &&
				searchItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return null;
		}

	protected SpecificationItem olderSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		JustificationItem searchItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		while( searchItem != null )
			{
			if( searchItem.isSuggestiveQuestionAssumption() &&
			( secondarySpecificationItem = searchItem.secondarySpecificationItem() ) != null )
				{
				if( secondarySpecificationItem.isOlderItem() )
					return secondarySpecificationItem;
				}

			searchItem = searchItem.nextJustificationItem();
			}

		return null;
		}
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
