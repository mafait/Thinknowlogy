/*
 *	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items for the
 *					self-generated knowledge
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

class JustificationList extends List
	{
	// Private constructible variables

	private JustificationItem adjustedQuestionReplacingJustificationItem_;
	private JustificationItem oldCorrectedAssumptionByOppositeQuestionJustificationItem_;


	// Private methods

	private boolean hasPrimarySpecification( short justificationTypeNr, SpecificationItem primarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.justificationTypeNr() == justificationTypeNr &&
			searchItem.primarySpecificationItem() == primarySpecificationItem )
				return true;

			searchItem = searchItem.nextJustificationItem();
			}

		return false;
		}

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

	private byte checkJustificationForUsage( boolean isActiveItem, JustificationItem unusedJustificationItem )
		{
		JustificationItem searchItem = firstJustificationItem( isActiveItem );

		if( unusedJustificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.attachedJustificationItem() == unusedJustificationItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "I found an attached justification item that is still in use" );

				if( searchItem.replacingJustificationItem == unusedJustificationItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "I found a replacing justification item that is still in use" );

				searchItem = searchItem.nextJustificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused justification item is undefined" );

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
					return startError( 1, null, myWordItem().anyWordTypeString(), "The primary specification item is still in use" );

				if( searchItem.anotherPrimarySpecificationItem() == unusedSpecificationItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The another primary specification item is still in use" );

				if( searchItem.secondarySpecificationItem() == unusedSpecificationItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The secondary specification item is still in use" );

				if( searchItem.anotherSecondarySpecificationItem() == unusedSpecificationItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The another secondary specification item is still in use" );

				searchItem = searchItem.nextJustificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem oldJustificationItem, JustificationItem replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem attachedJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem foundSpecificationItem;
		WordItem secondarySpecificationWordItem;

		if( oldJustificationItem != null )
			{
			if( replacingJustificationItem != null )
				{
				oldCorrectedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

				if( ( secondarySpecificationWordItem = replacingJustificationItem.secondarySpecificationWordItem() ) != null )
					{
					if( ( foundSpecificationItem = myWordItem().firstAssignmentOrSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != null )
						{
						if( foundSpecificationItem.attachJustificationToSpecification( replacingJustificationItem ) == Constants.RESULT_OK )
							{
							foundSpecificationItem = foundSpecificationItem.updatedSpecificationItem();

							if( ( firstJustificationItem = foundSpecificationItem.firstJustificationItem() ) != null )
								{
								if( ( attachedJustificationItem = firstJustificationItem.attachedJustificationItem() ) != null )
									{
									// Remove the old justification item from the replacing justification item
									if( firstJustificationItem.changeAttachedJustification( null ) == Constants.RESULT_OK )
										{
										if( replaceOrDeleteJustification( attachedJustificationItem ) == Constants.RESULT_OK )
											{
											if( ( justificationResult = copyJustification( false, attachedJustificationItem.primarySpecificationItem(), null, attachedJustificationItem ) ).result == Constants.RESULT_OK )
												{
												if( justificationResult.createdJustificationItem != null )
													{
													if( foundSpecificationItem.attachJustificationToSpecification( justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
														return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to attach the created justification item to the found specification" );
													}
												else
													return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find or create a justification" );
												}
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy the attached justification item" );
											}
										else
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the attached justification item, without an attached justification item" );
										}
									else
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change the attached justification item of the replacing justification item" );
									}
								}
							}
						else
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to attach the replacing justification item to the found specification" );
						}
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing justification item has no secondary specification word item" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing justification item is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given old justification item is undefined" );

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
					return startError( 1, null, myWordItem().anyWordTypeString(), "The replaced justification item of the given update justification item is a replaced item" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing justification item is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given update justification item is undefined" );

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

	private JustificationItem justificationItem( boolean isActiveItem, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchItem = firstJustificationItem( isActiveItem );

		while( searchItem != null )
			{
			if( searchItem.primarySpecificationItem() == primarySpecificationItem &&
			searchItem.anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchItem.secondarySpecificationItem() == secondarySpecificationItem )
				return searchItem;

			searchItem = searchItem.nextJustificationItem();
			}

		return null;
		}

	private JustificationItem secondaryAssumptionJustificationItem( boolean isPossessive, WordItem secondarySpecificationWordItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;

		if( secondarySpecificationWordItem != null )
			{
			while( searchItem != null )
				{
				// Justification without primary specification
				if( !searchItem.hasPrimarySpecification() &&
				( secondarySpecificationItem = searchItem.secondarySpecificationItem() ) != null )
					{
					if( secondarySpecificationItem.hasAssumptionLevel() &&
					secondarySpecificationItem.isPossessive() == isPossessive &&
					secondarySpecificationItem.specificationWordItem() == secondarySpecificationWordItem &&
					secondarySpecificationItem.relationWordItem() == myWordItem() )
						return searchItem;
					}

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return null;
		}


	// Constructor / deconstructor

	protected JustificationList( WordItem myWordItem )
		{
		// Private constructible variables

		adjustedQuestionReplacingJustificationItem_ = null;
		oldCorrectedAssumptionByOppositeQuestionJustificationItem_ = null;

		initializeListVariables( Constants.WORD_JUSTIFICATION_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void clearJustificationHasBeenWritten()
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			searchItem.hasBeenWritten = false;
			searchItem = searchItem.nextJustificationItem();
			}
		}

	protected void initializeVariables()
		{
		adjustedQuestionReplacingJustificationItem_ = null;
		oldCorrectedAssumptionByOppositeQuestionJustificationItem_ = null;
		}

	protected JustificationResultType addJustification( boolean isActiveItem, boolean isForcingNewJustification, short justificationTypeNr, short orderNr, int originalSentenceNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, JustificationItem attachedJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();
		short createOrderNr = orderNr;
		JustificationItem foundJustificationItem = null;
		SpecificationItem updatedPrimarySpecificationItem = ( primarySpecificationItem == null ? null : primarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedAnotherPrimarySpecificationItem = ( anotherPrimarySpecificationItem == null ? null : anotherPrimarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedSecondarySpecificationItem = ( secondarySpecificationItem == null ? null : secondarySpecificationItem.updatedSpecificationItem() );
		SpecificationItem updatedAnotherSecondarySpecificationItem = ( anotherSecondarySpecificationItem == null ? null : anotherSecondarySpecificationItem.updatedSpecificationItem() );

		if( primarySpecificationItem != null ||
		secondarySpecificationItem != null )
			{
			foundJustificationItem = justificationItem( isActiveItem, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem );

			if( foundJustificationItem == null ||

			// Found justification of wrong type (=question). Also create on concluded assumption
			( justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION &&
			foundJustificationItem.isQuestionJustification() ) ||

			// Exceptions
			( foundJustificationItem.justificationTypeNr() == justificationTypeNr &&

			( !isActiveItem ||
			isForcingNewJustification ||

			( attachedJustificationItem != null &&
			foundJustificationItem.attachedJustificationItem() != attachedJustificationItem ) ) ) )
				{
				if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
					{
					if( orderNr == Constants.NO_ORDER_NR &&
					foundJustificationItem == null )
						{
						createOrderNr = highestJustificationOrderNr( justificationTypeNr );

						if( !myWordItem().hasCorrectedAssumptionByOppositeQuestion() &&

						( createOrderNr == Constants.NO_ORDER_NR ||
						justificationTypeNr == Constants.JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION ||
						justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
						justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
						justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||
						justificationTypeNr == Constants.JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION ||
						justificationTypeNr == Constants.JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION ||

						( myWordItem().hasConfirmedSpecificationButNoRelation() &&
						hasPrimarySpecification( Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, updatedPrimarySpecificationItem ) ) ) )
							createOrderNr++;
						}

					if( ( justificationResult.createdJustificationItem = new JustificationItem( justificationTypeNr, createOrderNr, originalSentenceNr, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem, attachedJustificationItem, this, myWordItem() ) ) != null )
						{
						if( isActiveItem )
							{
							if( addItemToList( Constants.QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
								addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active justification item" );
							}
						else	// Replaced item
							{
							if( addItemToList( Constants.QUERY_REPLACED_CHAR, justificationResult.createdJustificationItem ) != Constants.RESULT_OK )
								addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an archive justification item" );
							}
						}
					else
						startError( 1, null, myWordItem().anyWordTypeString(), "I failed to create a justification item" );
					}
				else
					startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );
				}
			}
		else
			startError( 1, null, myWordItem().anyWordTypeString(), "None of the given specification items is defined" );

		justificationResult.foundJustificationItem = foundJustificationItem;
		justificationResult.result = CommonVariables.result;
		return justificationResult;
		}

	protected JustificationResultType copyJustification( boolean isForcingNewJustification, SpecificationItem newPrimarySpecificationItem, JustificationItem newAttachedJustificationItem, JustificationItem originalJustificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();

		if( originalJustificationItem != null )
			return addJustification( true, isForcingNewJustification, originalJustificationItem.justificationTypeNr(), originalJustificationItem.orderNr, originalJustificationItem.originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem.anotherPrimarySpecificationItem(), originalJustificationItem.secondarySpecificationItem(), originalJustificationItem.anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		justificationResult.result = startError( 1, null, myWordItem().anyWordTypeString(), "The given original justification item is undefined" );
		return justificationResult;
		}

	protected byte attachJustification( JustificationItem attachJustificationItem, SpecificationItem involvedSpecificationItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		boolean isSkipUpdateJustification = false;
		short highestOrderNr;
		short justificationTypeNr;
		short previousAssumptionLevel;
		JustificationItem attachedJustification;
		JustificationItem createdJustificationItem;
		JustificationItem firstJustificationItem;
		JustificationItem lastAttachedJustificationItem;
		JustificationItem oldJustificationItem = null;
		JustificationItem previousAttachedJustificationItem = null;
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

						if( !involvedSpecificationItem.hasFoundJustificationOfSameType( justificationTypeNr, attachJustificationItem.primarySpecificationItem(), attachJustificationItem.secondarySpecificationItem() ) )
							{
							if( ( highestOrderNr = involvedSpecificationItem.highestAttachedJustificationOrderNr( justificationTypeNr ) ) < Constants.MAX_ORDER_NR )
								attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
							else
								return startSystemError( 1, null, myWordItem().anyWordTypeString(), "Justification order number overflow" );
							}

						if( ( firstJustificationItem = involvedSpecificationItem.firstJustificationItem() ) != null )
							{
							if( attachJustificationItem.foundSecondarySpecificationQuestion() == null &&
							( oldJustificationItem = firstJustificationItem.foundSecondarySpecificationQuestion() ) == null )
								{
								attachedPrimarySpecificationItem = attachJustificationItem.primarySpecificationItem();

								if( attachedPrimarySpecificationItem != null &&
								attachedPrimarySpecificationItem.hasRelationContext() )
									oldJustificationItem = firstJustificationItem.primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem, attachJustificationItem.secondarySpecificationItem() );
								else
									{
									attachedSecondarySpecificationItem = attachJustificationItem.secondarySpecificationItem();

									if( attachedSecondarySpecificationItem != null &&
									attachedSecondarySpecificationItem.isUserSpecification() )
										oldJustificationItem = firstJustificationItem.selfGeneratedSecondarySpecificationJustificationItem( attachedPrimarySpecificationItem, attachJustificationItem.secondarySpecificationItem() );
									}

								if( oldJustificationItem != null )
									{
									if( oldJustificationItem.isOlderItem() )
										{
										if( attachedPrimarySpecificationItem != null &&
										attachedPrimarySpecificationItem.hasAssumptionLevel() &&
										!involvedSpecificationItem.hasRelationContext() )
											{
											if( involvedSpecificationItem.attachJustificationToSpecification( attachJustificationItem ) == Constants.RESULT_OK )
												isSkipUpdateJustification = true;
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to attach the justification item to the given involved specification item" );
											}
										else
											{
											if( replaceJustification( false, false, false, oldJustificationItem, attachJustificationItem, involvedSpecificationItem, null ) == Constants.RESULT_OK )
												attachJustificationItem.orderNr = oldJustificationItem.orderNr;
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the old justification item" );
											}
										}
									else
										{
										oldJustificationItem = null;

										if( ( highestOrderNr = involvedSpecificationItem.highestAttachedJustificationOrderNr( justificationTypeNr ) ) < Constants.MAX_ORDER_NR )
											attachJustificationItem.orderNr = (short)( highestOrderNr + 1 );
										else
											return startSystemError( 1, null, myWordItem().anyWordTypeString(), "Justification order number overflow" );
										}
									}
								}

							if( oldJustificationItem == null )
								{
								// Find a justification item with current sentence number, but without attached justification item
								lastAttachedJustificationItem = firstJustificationItem;

								while( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() &&
								( attachedJustification = lastAttachedJustificationItem.attachedJustificationItem() ) != null )
									lastAttachedJustificationItem = attachedJustification;

								if( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() )
									{
									if( ( specificationResult = involvedSpecificationItem.getAssumptionLevel() ).result == Constants.RESULT_OK )
										{
										previousAssumptionLevel = specificationResult.assumptionLevel;

										if( lastAttachedJustificationItem.attachJustification( attachJustificationItem, involvedSpecificationItem ) == Constants.RESULT_OK )
											{
											if( ( specificationResult = involvedSpecificationItem.recalculateAssumptionLevel() ).result == Constants.RESULT_OK )
												{
												if( involvedSpecificationItem.isOlderItem() &&
												!involvedSpecificationItem.isNegative() &&
												!attachJustificationItem.isSpecificationSubstitutionPartOfAssumption() &&
												specificationResult.assumptionLevel != previousAssumptionLevel )
													{
													// Conclusion
													if( specificationResult.assumptionLevel == Constants.NO_ASSUMPTION_LEVEL )
														{
														if( involvedSpecificationItem.markAsConcludedAssumption( false ) != Constants.RESULT_OK )
															return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark the involved specification as concluded assumption" );
														}

													// Write adjusted specification
													if( myWordItem().writeUpdatedSpecification( true, false, false, involvedSpecificationItem ) != Constants.RESULT_OK )
														return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an adjusted specification" );
													}
												}
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to recalculate the assumption level" );
											}
										else
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to attach a justification item" );
										}
									else
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to get the assumption level" );
									}
								else	// Otherwise, attach the justification item to the given specification item
									{
									if( attachJustificationItem.hasAttachedJustification() )
										{
										lastAttachedJustificationItem = firstJustificationItem;

										while( lastAttachedJustificationItem.hasCurrentCreationSentenceNr() &&
										( attachedJustification = lastAttachedJustificationItem.attachedJustificationItem() ) != null )
											{
											previousAttachedJustificationItem = lastAttachedJustificationItem;
											lastAttachedJustificationItem = attachedJustification;
											}

										// The last attached justification item has no attached justification item
										if( !lastAttachedJustificationItem.hasAttachedJustification() )
											{
											if( previousAttachedJustificationItem != null )
												{
												// Attach attachJustificationItem to a copy of the last attached justification item
												if( ( justificationResult = copyJustification( false, lastAttachedJustificationItem.primarySpecificationItem(), attachJustificationItem, lastAttachedJustificationItem ) ).result == Constants.RESULT_OK )
													{
													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != null )
														{
														// Link the created justification item to the previous attached justification item
														if( previousAttachedJustificationItem.changeAttachedJustification( createdJustificationItem ) == Constants.RESULT_OK )
															{
															if( replaceJustification( false, false, false, lastAttachedJustificationItem, createdJustificationItem, involvedSpecificationItem, null ) == Constants.RESULT_OK )
																{
																if( myWordItem().recalculateAssumptions( false ) != Constants.RESULT_OK )
																	return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to recalculate my assumptions" );
																}
															else
																return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the last attached justification item" );
															}
														else
															return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change the attached justification item of the previous justification item" );
														}
													else
														return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't create a justification item" );
													}
												else
													return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy the last attached justification item with a different first justification item" );
												}
											else
												return startError( 1, null, myWordItem().anyWordTypeString(), "The previous attached justification item is undefined. I have no solution implemented to solve this problem" );
											}
										else
											return startError( 1, null, myWordItem().anyWordTypeString(), "The last attached justification item isn't the only old one. I have no solution implemented to solve this problem" );
										}
									else
										{
										if( involvedSpecificationItem.attachJustificationToSpecification( attachJustificationItem ) != Constants.RESULT_OK )
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to attach the justification item to the given involved specification item" );
										}
									}
								}
							else
								{
								if( !isSkipUpdateJustification )
									{
									if( myWordItem().updateJustificationInSpecifications( false, false, false, oldJustificationItem, attachJustificationItem ) != Constants.RESULT_OK )
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to update a question justification item by a conclusion justification item" );
									}
								}
							}
						else
							return startError( 1, null, myWordItem().anyWordTypeString(), "The given involved specification item has no justification item" );
						}
					else
						return startError( 1, null, myWordItem().anyWordTypeString(), "The given involved specification item is replaced or deleted" );
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item isn't active" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given involved specification item is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given attached justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkForConfirmedJustifications( SpecificationItem secondarySpecificationItem, SpecificationItem involvedSpecificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem secondarySpecificationJustificationItem;

		if( secondarySpecificationItem != null )
			{
			if( involvedSpecificationItem != null )
				{
				if( ( secondarySpecificationJustificationItem = secondaryAssumptionJustificationItem( !secondarySpecificationItem.isPossessive(), secondarySpecificationItem.specificationWordItem() ) ) != null )
					{
					if( ( justificationResult = addJustification( true, false, Constants.JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, secondarySpecificationJustificationItem.orderNr, secondarySpecificationJustificationItem.originalSentenceNr(), null, null, secondarySpecificationItem, null, null ) ).result == Constants.RESULT_OK )
						{
						if( justificationResult.createdJustificationItem != null )
							{
							if( replaceJustification( true, false, false, secondarySpecificationJustificationItem, justificationResult.createdJustificationItem, involvedSpecificationItem, null ) != Constants.RESULT_OK )
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace an old secondary justification item without primary specification" );
							}
						else
							return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't find or create a justification without primary specification" );
						}
					else
						return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add a justification without primary specification" );
					}
				else
					{
					if( secondarySpecificationItem.isUserSpecification() &&
					( secondarySpecificationJustificationItem = involvedSpecificationItem.oppositePossessiveConditionalSpecificationAssumptionJustificationItem( secondarySpecificationItem ) ) != null )
						{
						if( replaceJustification( true, false, false, secondarySpecificationJustificationItem, null, involvedSpecificationItem, null ) != Constants.RESULT_OK )
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace an opposite possessive conditional specification assumption justification item without primary specification" );
						}
					}
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given involved justification item is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given secondary justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte checkForUnreferencedJustifications()
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( !myWordItem().isJustificationInUse( searchItem ) )
				return startError( 1, null, myWordItem().anyWordTypeString(), "I found an unreferenced active justification item" );

			searchItem = searchItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
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
			return checkJustificationForUsage( false, unusedJustificationItem );

		return Constants.RESULT_OK;
		}

	protected byte checkSpecificationItemForUsage( SpecificationItem unusedSpecificationItem )
		{
		if( checkSpecificationItemForUsage( true, unusedSpecificationItem ) == Constants.RESULT_OK )
			return checkSpecificationItemForUsage( false, unusedSpecificationItem );

		return Constants.RESULT_OK;
		}

	protected byte replaceJustification( boolean hasConfirmedSpecification, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, JustificationItem oldJustificationItem, JustificationItem replacingJustificationItem, SpecificationItem involvedSpecificationItem, SpecificationItem replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		boolean hasCorrectedAssumption;
		boolean hasCorrectedAssumptionByKnowledge = myWordItem().hasCorrectedAssumptionByKnowledge();
		boolean hasCorrectedAssumptionByOppositeQuestion = myWordItem().hasCorrectedAssumptionByOppositeQuestion();
		boolean isUpdatedPrimarySpecificationWordCollectedWithItself = false;
		JustificationItem attachedJustificationItem;
		JustificationItem firstJustificationItem;
		SpecificationItem relatedSpecificationItem;

		if( oldJustificationItem != null )
			{
			if( involvedSpecificationItem != null )
				{
				oldJustificationItem = oldJustificationItem.updatedJustificationItem();

				if( oldJustificationItem.isOlderItem() )
					{
					if( oldJustificationItem.replacingJustificationItem == null )
						{
						if( replacingJustificationItem == null ||
						replacingJustificationItem.replacingJustificationItem == null )
							{
							if( oldJustificationItem != replacingJustificationItem )
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
										if( myWordItem().writeUpdatedSpecification( false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, involvedSpecificationItem ) == Constants.RESULT_OK )
											{
											// Corrected negative assumption
											if( ( specificationResult = myWordItem().findRelatedSpecification( false, involvedSpecificationItem.isAssignment(), involvedSpecificationItem.isInactiveAssignment(), involvedSpecificationItem.isArchivedAssignment(), involvedSpecificationItem.isExclusiveSpecification(), true, involvedSpecificationItem.isPossessive(), Constants.NO_QUESTION_PARAMETER, involvedSpecificationItem.specificationCollectionNr(), involvedSpecificationItem.generalizationContextNr(), involvedSpecificationItem.specificationContextNr(), involvedSpecificationItem.relationContextNr(), involvedSpecificationItem.specificationWordItem(), null ) ).result == Constants.RESULT_OK )
												{
												if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != null )
													{
													if( myWordItem().writeUpdatedSpecification( false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, relatedSpecificationItem ) == Constants.RESULT_OK )
														{
														if( hasCorrectedAssumptionByOppositeQuestion )
															{
															if( myWordItem().replaceOrDeleteSpecification( relatedSpecificationItem, involvedSpecificationItem.updatedSpecificationItem() ) != Constants.RESULT_OK )
																return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace a corrected negative assumption" );
															}
														}
													else
														return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write a corrected negative assumption" );
													}
												}
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to find a negative related specification" );
											}
										else
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write an adjusted specification" );
										}

									if( ( attachedJustificationItem = replacingJustificationItem.attachedJustificationItem() ) != null )
										{
										if( hasCorrectedAssumptionByKnowledge )
											{
											isUpdatedPrimarySpecificationWordCollectedWithItself = attachedJustificationItem.hasUpdatedPrimarySpecificationWordCollectedWithItself();

											if( isUpdatedPrimarySpecificationWordCollectedWithItself &&
											attachedJustificationItem.hasReplacedPrimarySpecification() )
												{
												if( replaceJustification( hasConfirmedSpecification, isExclusiveGeneralization, isExclusiveSpecification, attachedJustificationItem, null, involvedSpecificationItem, null ) != Constants.RESULT_OK )
													return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the attached justification item with replaced primary specification" );
												}
											}

										if( !isUpdatedPrimarySpecificationWordCollectedWithItself )
											{
											if( replaceOrDeleteJustification( replacingJustificationItem ) == Constants.RESULT_OK )
												replacingJustificationItem = null;
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete a corrected justification" );
											}
										}
									}

								attachedJustificationItem = oldJustificationItem.attachedJustificationItem();

								if( attachedJustificationItem != null &&							// Old justification has an attached justification,
								attachedJustificationItem.isActiveItem() &&						// that is active,
								replacingJustificationItem != null &&
								replacingJustificationItem != attachedJustificationItem &&
								replacingJustificationItem.hasCurrentActiveSentenceNr() &&			// but the replacing one is current
								!replacingJustificationItem.hasAttachedJustification() )			// and it has no attached justifications
									{
									if( replacingJustificationItem.changeAttachedJustification( attachedJustificationItem ) != Constants.RESULT_OK )
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change an attached justification item" );
									}

								if( attachedJustificationItem == null ||
								!attachedJustificationItem.isActiveItem() ||

								// Replacing justification item has the same attached justification item
								( replacingJustificationItem != null &&

								( replacingJustificationItem == attachedJustificationItem ||
								replacingJustificationItem.attachedJustificationItem() == attachedJustificationItem ) ) )
									{
									if( oldJustificationItem.isActiveItem() )
										{
										if( replaceOrDeleteJustification( oldJustificationItem ) == Constants.RESULT_OK )
											{
											if( myWordItem().updateJustificationInSpecifications( hasConfirmedSpecification, isExclusiveSpecification, isExclusiveGeneralization, oldJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
												{
												if( replacingJustificationItem != null )
													{
													if( adjustedQuestionReplacingJustificationItem_ != null &&
													!replacingJustificationItem.hasPrimaryUserSpecification() &&
													replacingJustificationItem.isQuestionJustification() )
														{
														if( replaceJustification( false, false, false, replacingJustificationItem, adjustedQuestionReplacingJustificationItem_, involvedSpecificationItem, null ) != Constants.RESULT_OK )
															return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the replacing justification item of an adjusted question" );
														}

													if( updateReplacedJustifications( oldJustificationItem, replacingJustificationItem ) == Constants.RESULT_OK )
														oldJustificationItem.replacingJustificationItem = replacingJustificationItem;
													else
														return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to update the replacing justification items of the replaced justification items" );
													}
												}
											else
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to update a justification in specifications of my word" );
											}
										else
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace an active justification item" );
										}
									else
										{
										if( hasCorrectedAssumptionByOppositeQuestion )
											{
											if( replaceJustificationOfCorrectedAssumptionByOppositeQuestion( oldJustificationItem, replacingJustificationItem ) != Constants.RESULT_OK )
												return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the justification of a corrected assumption by opposite question" );
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
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given old justification item" );
										}
									else
										{
										if( !involvedSpecificationItem.isSelfGeneratedQuestion() &&
										( firstJustificationItem = involvedSpecificationItem.firstJustificationItem() ) != null )
											{
											if( !firstJustificationItem.isReplacedItem() )
												{
												if( replaceOrDeleteJustification( firstJustificationItem ) != Constants.RESULT_OK )
													return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the first justification item" );
												}

											if( hasCorrectedAssumption ||
											!myWordItem().isJustificationInUse( attachedJustificationItem ) )
												{
												if( replaceOrDeleteJustification( attachedJustificationItem ) != Constants.RESULT_OK )
													return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the attached justification item" );
												}
											}
										}
									}

								involvedSpecificationItem = involvedSpecificationItem.updatedSpecificationItem();

								// Check for concluded negative assumption
								if( involvedSpecificationItem.isNegative() &&
								!involvedSpecificationItem.isConcludedAssumption() &&
								oldJustificationItem.isAssumptionJustification() &&
								replacingJustificationItem != null &&
								replacingJustificationItem.isConclusionJustification() )
									{
									if( involvedSpecificationItem.markAsConcludedAssumption( true ) == Constants.RESULT_OK )
										{
										if( myWordItem().writeUpdatedSpecification( true, false, false, involvedSpecificationItem ) != Constants.RESULT_OK )
											return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write the updated involved specification" );
										}
									else
										return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to mark the involved specification as concluded negative assumption" );
									}
								}
							else
								return startError( 1, null, myWordItem().anyWordTypeString(), "The given old justification item and the given replacing justification item are the same justification item" );
							}
						else
							return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing justification item has a replacing justification item itself" );
						}
					else
						return startError( 1, null, myWordItem().anyWordTypeString(), "The given old justification item is already replaced, because it has a replacing justification item" );
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "The given old justification item isn't old" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given involved specification item is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given old justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte replaceObsoleteJustificationsOfAdjustedQuestion( JustificationItem replacingJustificationItem, SpecificationItem adjustedQuestionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem attachedJustificationItem;
		JustificationItem currentJustificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem createdSpecificationItem;

		adjustedQuestionReplacingJustificationItem_ = replacingJustificationItem;

		if( replacingJustificationItem != null )
			{
			if( adjustedQuestionSpecificationItem != null )
				{
				if( ( currentJustificationItem = adjustedQuestionSpecificationItem.firstJustificationItem() ) != null )
					{
					do	{
						attachedJustificationItem = currentJustificationItem.attachedJustificationItem();
						primarySpecificationItem = currentJustificationItem.primarySpecificationItem();

						if( primarySpecificationItem != null &&
						primarySpecificationItem.isOlderItem() )
							{
							if( replaceJustification( false, false, true, currentJustificationItem, replacingJustificationItem, adjustedQuestionSpecificationItem, null ) != Constants.RESULT_OK )
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the current justification item of an adjusted question" );
							}
						}
					while( ( currentJustificationItem = attachedJustificationItem ) != null );

					if( !adjustedQuestionSpecificationItem.isReplacedItem() )
						{
						if( ( specificationResult = myWordItem().copySpecification( adjustedQuestionSpecificationItem.isInactiveAssignment(), adjustedQuestionSpecificationItem.isArchivedAssignment(), adjustedQuestionSpecificationItem.isAnsweredQuestion(), adjustedQuestionSpecificationItem.isExclusiveSpecification(), adjustedQuestionSpecificationItem.assignmentLevel(), adjustedQuestionSpecificationItem.generalizationCollectionNr(), adjustedQuestionSpecificationItem.specificationCollectionNr(), adjustedQuestionSpecificationItem.relationContextNr(), replacingJustificationItem, adjustedQuestionSpecificationItem ) ).result == Constants.RESULT_OK )
							{
							if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != null )
								{
								if( myWordItem().replaceOrDeleteSpecification( adjustedQuestionSpecificationItem, createdSpecificationItem ) != Constants.RESULT_OK )
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace or delete a specification" );
								}
							else
								return startError( 1, null, myWordItem().anyWordTypeString(), "I couldn't create the adjusted question specification" );
							}
						else
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to copy a specification with a different first justification item" );
						}
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "The given adjusted question specification item isn't self-generated" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given adjusted question specification item is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given replacing justification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte replaceOrDeleteJustification( JustificationItem obsoleteJustificationItem )
		{
		if( obsoleteJustificationItem != null )
			{
			if( obsoleteJustificationItem.hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( false, obsoleteJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to delete an obsolete justification" );
				}
			else
				{
				if( replaceItem( obsoleteJustificationItem ) != Constants.RESULT_OK )
					return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace an obsolete justification" );
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given obsolete justification item is undefined" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDataBase()
		{
		// Not fully implemented yet
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeJustificationItemInFutureDataBase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store a justification item in the database" );
				}

			searchItem = searchItem.nextJustificationItem();
			}

		searchItem = firstReplacedJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeJustificationItemInFutureDataBase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced justification item in the database" );
				}

			searchItem = searchItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected byte updateSpecificationsInJustifications( boolean hasConfirmedSpecificationInMainWord, boolean isActiveItem, SpecificationItem obsoleteSpecificationItem, SpecificationItem replacingSpecificationItem )
		{
		JustificationResultType justificationResult = new JustificationResultType();
		boolean isReplacePrimarySpecification;
		boolean isReplaceSecondarySpecification;
		boolean hasCorrectedAssumption = myWordItem().hasCorrectedAssumption();
		boolean hasCorrectedAssumptionByKnowledge = myWordItem().hasCorrectedAssumptionByKnowledge();
		boolean isExclusiveGeneralization = false;
		boolean isExclusiveSpecification = ( replacingSpecificationItem == null ? false : replacingSpecificationItem.isExclusiveSpecification() );
		SpecificationItem involvedSpecificationItem;
		SpecificationItem primarySpecificationItem;
		SpecificationItem secondarySpecificationItem;
		JustificationItem createdJustificationItem = null;
		JustificationItem searchItem = firstJustificationItem( isActiveItem );

		if( obsoleteSpecificationItem != null )
			{
			if( !obsoleteSpecificationItem.isQuestion() &&
			replacingSpecificationItem != null &&
			replacingSpecificationItem.hasExclusiveGeneralizationCollection() )
				isExclusiveGeneralization = true;

			while( searchItem != null )
				{
				primarySpecificationItem = searchItem.primarySpecificationItem();
				secondarySpecificationItem = searchItem.secondarySpecificationItem();

				isReplacePrimarySpecification = ( primarySpecificationItem == obsoleteSpecificationItem );
				isReplaceSecondarySpecification = ( secondarySpecificationItem == obsoleteSpecificationItem );

				if( isReplacePrimarySpecification ||
				isReplaceSecondarySpecification )
					{
					if( replacingSpecificationItem != null &&
					searchItem.hasCurrentCreationSentenceNr() )
						{
						if( isReplacePrimarySpecification )
							{
							if( primarySpecificationItem != null &&
							searchItem.isSpecificationSubstitutionAssumption() &&
							primarySpecificationItem.relationContextNr() != replacingSpecificationItem.relationContextNr() &&
							( involvedSpecificationItem = myWordItem().justificationSpecificationItem( replacingSpecificationItem.isAssignment(), replacingSpecificationItem.isInactiveAssignment(), replacingSpecificationItem.isArchivedAssignment(), searchItem ) ) != null )
								{
								if( replaceJustification( hasConfirmedSpecificationInMainWord, false, false, searchItem, null, involvedSpecificationItem, null ) != Constants.RESULT_OK )
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace the search justification item" );
								}
							else
								{
								if( searchItem.changePrimarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
									return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change the primary specificationItem item of a justification item" );
								}
							}

						if( isReplaceSecondarySpecification )
							{
							if( searchItem.changeSecondarySpecification( replacingSpecificationItem ) != Constants.RESULT_OK )
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to change the secondary specificationItem item of a justification item" );
							}

						searchItem = firstJustificationItem( isActiveItem );
						}
					else
						{
						createdJustificationItem = null;

						if( replacingSpecificationItem != null &&

						( !isActiveItem ||
						searchItem.isQuestionJustification() ||
						// Avoid creating unreferenced justifications
						myWordItem().isJustificationInUse( searchItem ) ) )
							{
							if( ( justificationResult = addJustification( isActiveItem, false, searchItem.justificationTypeNr(), searchItem.orderNr, searchItem.originalSentenceNr(), ( isReplacePrimarySpecification ? replacingSpecificationItem : primarySpecificationItem ), searchItem.anotherPrimarySpecificationItem(), ( isReplaceSecondarySpecification ? replacingSpecificationItem : secondarySpecificationItem ), searchItem.anotherSecondarySpecificationItem(), searchItem.attachedJustificationItem() ) ).result == Constants.RESULT_OK )
								{
								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == null )
									{
									if( justificationResult.foundJustificationItem != searchItem )
										createdJustificationItem = justificationResult.foundJustificationItem;
									else
										return addError( 1, null, myWordItem().anyWordTypeString(), "No justification item is created, and the found justificiation item is the same as the search item itself" );
									}
								else
									{
									if( !isActiveItem )
										searchItem.replacingJustificationItem = createdJustificationItem;
									}
								}
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add a justification item" );
							}

						if( isActiveItem )
							{
							if( replaceJustification( hasConfirmedSpecificationInMainWord, isExclusiveGeneralization, isExclusiveSpecification, searchItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : null ) ) == Constants.RESULT_OK )
								searchItem = ( hasCorrectedAssumption ? searchItem.nextJustificationItem() : firstActiveJustificationItem() );
							else
								return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to replace a justification item" );
							}
						else
							searchItem = searchItem.nextJustificationItem();
						}
					}
				else
					searchItem = searchItem.nextJustificationItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given obsolete specification item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte writeRelatedJustificationSpecifications( short justificationTypeNr, int specificationCollectionNr )
		{
		JustificationItem searchItem = firstActiveJustificationItem();
		SpecificationItem secondarySpecificationItem;
		WordItem generalizationWordItem;

		while( searchItem != null )
			{
			if( !searchItem.hasBeenWritten &&
			searchItem.justificationTypeNr() == justificationTypeNr &&
			( secondarySpecificationItem = searchItem.secondarySpecificationItem() ) != null )
				{
				if( !secondarySpecificationItem.isExclusiveSpecification() &&
				secondarySpecificationItem.specificationCollectionNr() == specificationCollectionNr )
					{
					if( ( generalizationWordItem = secondarySpecificationItem.generalizationWordItem() ) != null )
						{
						if( generalizationWordItem.writeJustificationSpecification( false, secondarySpecificationItem ) != Constants.RESULT_OK )
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to write a secondary justification specification" );
						}
					else
						return startError( 1, null, myWordItem().anyWordTypeString(), "The secondary specification item has no generalization word" );
					}
				}

			searchItem = searchItem.nextJustificationItem();
			}

		return Constants.RESULT_OK;
		}

	protected JustificationItem generalizationAssumptionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isGeneralizationAssumption() &&
				searchItem.secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem.nextJustificationItem();
				}
			}

		return null;
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

	protected JustificationItem obsoleteAssumptionJustificationItem()
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		while( searchItem != null )
			{
			if( searchItem.isAssumptionJustification() &&
			!myWordItem().isJustificationInUse( searchItem ) )
				return searchItem;

			searchItem = searchItem.nextJustificationItem();
			}

		return null;
		}

	protected JustificationItem oldCorrectedAssumptionByOppositeQuestionJustificationItem()
		{
		return oldCorrectedAssumptionByOppositeQuestionJustificationItem_;
		}

	protected JustificationItem questionJustificationItem( SpecificationItem secondarySpecificationItem )
		{
		JustificationItem searchItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isQuestionJustification() &&
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

	protected SpecificationItem oldSuggestiveQuestionAssumptionSecondarySpecificationItem()
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
 *
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *
 *************************************************************************/
