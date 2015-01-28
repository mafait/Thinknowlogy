/*
 *	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items for the
 *					self-generated knowledge
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

class JustificationList : private List
	{
	friend class WordItem;

	// Private constructible variables

	JustificationItem *adjustedQuestionReplacingJustificationItem_;
	JustificationItem *oldCorrectedAssumptionByOppositeQuestionJustificationItem_;


	// Private functions

	unsigned short highestJustificationOrderNr( unsigned short justificationTypeNr )
		{
		unsigned short highestOrderNr = NO_ORDER_NR;
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->orderNr > highestOrderNr &&
			searchItem->justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchItem->orderNr;

			searchItem = searchItem->nextJustificationItem();
			}

		return highestOrderNr;
		}

	ResultType checkForUnreferencedJustifications( bool isActiveItem )
		{
		JustificationItem *searchItem = firstJustificationItem( isActiveItem );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForUnreferencedJustifications";

		while( searchItem != NULL )
			{
			if( !myWordItem()->isJustificationInUse( searchItem ) )
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I found an unreferenced active justification item" );

			searchItem = searchItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	ResultType checkJustificationForUsage( bool isActiveItem, JustificationItem *unusedJustificationItem )
		{
		JustificationItem *searchItem = firstJustificationItem( isActiveItem );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->attachedJustificationItem() == unusedJustificationItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I found an attached justification item that is still in use" );

				if( searchItem->replacingJustificationItem == unusedJustificationItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I found a replacing justification item that is still in use" );

				searchItem = searchItem->nextJustificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given unused justification item is undefined" );

		return RESULT_OK;
		}

	ResultType checkSpecificationItemForUsage( bool isActiveItem, SpecificationItem *unusedSpecificationItem )
		{
		JustificationItem *searchItem = firstJustificationItem( isActiveItem );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationItemForUsage";

		if( unusedSpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->primarySpecificationItem() == unusedSpecificationItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The primary specification item is still in use" );

				if( searchItem->anotherPrimarySpecificationItem() == unusedSpecificationItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The another primary specification item is still in use" );

				if( searchItem->secondarySpecificationItem() == unusedSpecificationItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The secondary specification item is still in use" );

				if( searchItem->anotherSecondarySpecificationItem() == unusedSpecificationItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The another secondary specification item is still in use" );

				searchItem = searchItem->nextJustificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given unused specification item is undefined" );

		return RESULT_OK;
		}

	ResultType replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem *attachedJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustificationOfCorrectedAssumptionByOppositeQuestion";

		if( obsoleteJustificationItem != NULL )
			{
			if( replacingJustificationItem != NULL )
				{
				oldCorrectedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

				if( ( secondarySpecificationWordItem = replacingJustificationItem->secondarySpecificationWordItem() ) != NULL )
					{
					if( ( foundSpecificationItem = myWordItem()->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != NULL )
						{
						if( foundSpecificationItem->attachJustificationToSpecification( replacingJustificationItem ) == RESULT_OK )
							{
							foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

							if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL )
								{
								if( ( attachedJustificationItem = firstJustificationItem->attachedJustificationItem() ) != NULL )
									{
									// Remove the obsolete justification item from the replacing justification item
									if( firstJustificationItem->changeAttachedJustification( NULL ) == RESULT_OK )
										{
										if( replaceOrDeleteJustification( attachedJustificationItem ) == RESULT_OK )
											{
											if( ( justificationResult = copyJustification( false, attachedJustificationItem->primarySpecificationItem(), attachedJustificationItem->secondarySpecificationItem(), NULL, attachedJustificationItem ) ).result == RESULT_OK )
												{
												if( justificationResult.createdJustificationItem != NULL )
													{
													if( foundSpecificationItem->attachJustificationToSpecification( justificationResult.createdJustificationItem ) != RESULT_OK )
														return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to attach the created justification item to the found specification" );
													}
												else
													return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find or create a justification" );
												}
											else
												return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to copy the attached justification item" );
											}
										else
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the attached justification item, without an attached justification item" );
										}
									else
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the attached justification item of the replacing justification item" );
									}
								}
							}
						else
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to attach the replacing justification item to the found specification" );
						}
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item has no secondary specification word item" );
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item is undefined" );

		return RESULT_OK;
		}

	ResultType updateReplacedJustifications( JustificationItem *updateJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationItem *searchItem = firstReplacedJustificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateReplacedJustifications";

		if( updateJustificationItem != NULL )
			{
			if( replacingJustificationItem != NULL )
				{
				if( !replacingJustificationItem->isReplacedItem() )
					{
					while( searchItem != NULL )
						{
						if( searchItem->replacingJustificationItem == updateJustificationItem )
							searchItem->replacingJustificationItem = replacingJustificationItem;

						searchItem = searchItem->nextJustificationItem();
						}
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The replaced justification item of the given update justification item is a replaced item" );
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given update justification item is undefined" );

		return RESULT_OK;
		}

	JustificationItem *firstActiveJustificationItem()
		{
		return (JustificationItem *)firstActiveItem();
		}

	JustificationItem *firstReplacedJustificationItem()
		{
		return (JustificationItem *)firstReplacedItem();
		}

	JustificationItem *firstJustificationItem( bool isActiveItem )
		{
		return ( isActiveItem ? firstActiveJustificationItem() : firstReplacedJustificationItem() );
		}

	JustificationItem *justificationItem( bool isActiveItem, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchItem = firstJustificationItem( isActiveItem );

		while( searchItem != NULL )
			{
			if( searchItem->primarySpecificationItem() == primarySpecificationItem &&
			searchItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchItem->secondarySpecificationItem() == secondarySpecificationItem )
				return searchItem;

			searchItem = searchItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *primarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->justificationTypeNr() == justificationTypeNr &&
			searchItem->primarySpecificationItem() == primarySpecificationItem &&
			searchItem->anotherPrimarySpecificationItem() != NULL )
				return searchItem;

			searchItem = searchItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *secondaryAssumptionJustificationItem( bool isPossessive, WordItem *secondarySpecificationWordItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;

		if( secondarySpecificationWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				// Justification without primary specification
				if( !searchItem->hasPrimarySpecification() &&
				( secondarySpecificationItem = searchItem->secondarySpecificationItem() ) != NULL )
					{
					if( secondarySpecificationItem->hasAssumptionLevel() &&
					secondarySpecificationItem->isPossessive() == isPossessive &&
					secondarySpecificationItem->specificationWordItem() == secondarySpecificationWordItem &&
					secondarySpecificationItem->relationWordItem() == myWordItem() )
						return searchItem;
					}

				searchItem = searchItem->nextJustificationItem();
				}
			}

		return NULL;
		}


	protected:
	// Constructor / deconstructor

	JustificationList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		// Private constructible variables

		adjustedQuestionReplacingJustificationItem_ = NULL;
		oldCorrectedAssumptionByOppositeQuestionJustificationItem_ = NULL;

		initializeListVariables( WORD_JUSTIFICATION_LIST_SYMBOL, "JustificationList", commonVariables, myWordItem );
		}

	~JustificationList()
		{
		JustificationItem *deleteItem;
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextJustificationItem();
			delete deleteItem;
			}

		searchItem = firstReplacedJustificationItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextJustificationItem();
			delete deleteItem;
			}

		searchItem = (JustificationItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextJustificationItem();
			delete deleteItem;
			}
		}


	// Protected functions

	void clearJustificationHasBeenWritten()
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( searchItem != NULL )
			{
			searchItem->hasBeenWritten = false;
			searchItem = searchItem->nextJustificationItem();
			}
		}

	void initializeVariables()
		{
		adjustedQuestionReplacingJustificationItem_ = NULL;
		oldCorrectedAssumptionByOppositeQuestionJustificationItem_ = NULL;
		}

	JustificationResultType addJustification( bool isActiveItem, bool isForcingNewJustification, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem *foundJustificationItem = NULL;
		SpecificationItem *updatedPrimarySpecificationItem = ( primarySpecificationItem == NULL ? NULL : primarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedAnotherPrimarySpecificationItem = ( anotherPrimarySpecificationItem == NULL ? NULL : anotherPrimarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedSecondarySpecificationItem = ( secondarySpecificationItem == NULL ? NULL : secondarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedAnotherSecondarySpecificationItem = ( anotherSecondarySpecificationItem == NULL ? NULL : anotherSecondarySpecificationItem->updatedSpecificationItem() );
		char functionNameString[FUNCTION_NAME_LENGTH] = "addJustification";

		if( primarySpecificationItem != NULL ||
		secondarySpecificationItem != NULL )
			{
			if( !isForcingNewJustification )
				foundJustificationItem = justificationItem( isActiveItem, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem );

			if( foundJustificationItem == NULL ||

			// Found justification of wrong type (=question). Also create on concluded assumption
			( justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION &&
			foundJustificationItem->isQuestionJustification() ) ||

			// Exceptions
			( foundJustificationItem->justificationTypeNr() == justificationTypeNr &&

			( !isActiveItem ||

			( attachedJustificationItem != NULL &&
			foundJustificationItem->attachedJustificationItem() != attachedJustificationItem ) ) ) )
				{
				if( commonVariables()->currentItemNr < MAX_ITEM_NR )
					{
					if( orderNr == NO_ORDER_NR )
						{
						if( foundJustificationItem == NULL &&
						updatedAnotherPrimarySpecificationItem != NULL )
							foundJustificationItem = primarySpecificationJustificationItem( justificationTypeNr, updatedPrimarySpecificationItem );

						if( foundJustificationItem == NULL )
							{
							orderNr = highestJustificationOrderNr( justificationTypeNr );

							if( orderNr == NO_ORDER_NR ||
							justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
							justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

							// Skip self-generated question of corrected assumption by opposite question
							( justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
							updatedPrimarySpecificationItem != NULL &&
							!updatedPrimarySpecificationItem->isSelfGeneratedQuestion() ) )
								orderNr++;
							}
						else
							orderNr = foundJustificationItem->orderNr;
						}

					if( orderNr > NO_ORDER_NR )
						{
						if( ( justificationResult.createdJustificationItem = new JustificationItem( justificationTypeNr, orderNr, originalSentenceNr, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem, attachedJustificationItem, commonVariables(), this, myWordItem() ) ) != NULL )
							{
							if( isActiveItem )
								{
								// Add as active justification item
								if( addItemToList( QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != RESULT_OK )
									addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active justification item" );
								}
							else
								{
								// Add as replaced justification item
								if( addItemToList( QUERY_REPLACED_CHAR, justificationResult.createdJustificationItem ) != RESULT_OK )
									addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an archive justification item" );
								}
							}
						else
							startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to create a justification item" );
						}
					else
						startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The order number is undefined" );
					}
				else
					startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );
				}
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "None of the given specification items is defined" );

		justificationResult.foundJustificationItem = foundJustificationItem;
		justificationResult.result = commonVariables()->result;
		return justificationResult;
		}

	JustificationResultType copyJustification( bool isForcingNewJustification, SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyJustification";

		if( originalJustificationItem != NULL )
			return addJustification( true, isForcingNewJustification, originalJustificationItem->justificationTypeNr(), originalJustificationItem->orderNr, originalJustificationItem->originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem->anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem->anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		justificationResult.result = startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given original justification item is undefined" );
		return justificationResult;
		}

	ResultType attachJustification( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		bool isSkipUpdateJustification = false;
		unsigned short highestOrderNr;
		unsigned short justificationTypeNr;
		unsigned short previousAssumptionLevel;
		JustificationItem *attachedJustification;
		JustificationItem *createdJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *lastAttachedJustificationItem;
		JustificationItem *obsoleteJustificationItem = NULL;
		JustificationItem *previousAttachedJustificationItem = NULL;
		SpecificationItem *attachedPrimarySpecificationItem;
		SpecificationItem *attachedSecondarySpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustification";

		if( attachJustificationItem != NULL )
			{
			if( involvedSpecificationItem != NULL )
				{
				if( attachJustificationItem->isActiveItem() )
					{
					if( !involvedSpecificationItem->isReplacedOrDeletedItem() )
						{
						justificationTypeNr = attachJustificationItem->justificationTypeNr();

						if( !involvedSpecificationItem->hasFoundJustificationOfSameType( justificationTypeNr, attachJustificationItem->primarySpecificationItem(), attachJustificationItem->secondarySpecificationItem() ) )
							{
							if( ( highestOrderNr = involvedSpecificationItem->highestAttachedJustificationOrderNr( justificationTypeNr ) ) < MAX_ORDER_NR )
								attachJustificationItem->orderNr = ( highestOrderNr + 1 );
							else
								return startSystemError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "Justification order number overflow" );
							}

						if( ( firstJustificationItem = involvedSpecificationItem->firstJustificationItem() ) != NULL )
							{
							if( attachJustificationItem->foundSecondarySpecificationQuestion() == NULL &&
							( obsoleteJustificationItem = firstJustificationItem->foundSecondarySpecificationQuestion() ) == NULL )
								{
								attachedPrimarySpecificationItem = attachJustificationItem->primarySpecificationItem();

								if( attachedPrimarySpecificationItem != NULL &&
								attachedPrimarySpecificationItem->hasRelationContext() )
									obsoleteJustificationItem = firstJustificationItem->primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem, attachJustificationItem->secondarySpecificationItem() );
								else
									{
									attachedSecondarySpecificationItem = attachJustificationItem->secondarySpecificationItem();

									if( attachedSecondarySpecificationItem != NULL &&
									attachedSecondarySpecificationItem->isUserSpecification() )
										obsoleteJustificationItem = firstJustificationItem->selfGeneratedSecondarySpecificationJustificationItem( attachedPrimarySpecificationItem, attachJustificationItem->secondarySpecificationItem() );
									}

								if( obsoleteJustificationItem != NULL )
									{
									if( obsoleteJustificationItem->isOlderItem() )
										{
										if( attachedPrimarySpecificationItem != NULL &&
										attachedPrimarySpecificationItem->hasAssumptionLevel() &&
										!involvedSpecificationItem->hasRelationContext() )
											{
											if( involvedSpecificationItem->attachJustificationToSpecification( attachJustificationItem ) == RESULT_OK )
												isSkipUpdateJustification = true;
											else
												return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to attach the justification item to the given involved specification item" );
											}
										else
											{
											if( replaceJustification( false, false, false, obsoleteJustificationItem, attachJustificationItem, involvedSpecificationItem, NULL ) == RESULT_OK )
												attachJustificationItem->orderNr = obsoleteJustificationItem->orderNr;
											else
												return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the obsolete justification item" );
											}
										}
									else
										{
										obsoleteJustificationItem = NULL;

										if( ( highestOrderNr = involvedSpecificationItem->highestAttachedJustificationOrderNr( justificationTypeNr ) ) < MAX_ORDER_NR )
											attachJustificationItem->orderNr = ( highestOrderNr + 1 );
										else
											return startSystemError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "Justification order number overflow" );
										}
									}
								}

							if( obsoleteJustificationItem == NULL )
								{
								// Find a justification item with current sentence number, but without attached justification item
								lastAttachedJustificationItem = firstJustificationItem;

								while( lastAttachedJustificationItem->hasCurrentCreationSentenceNr() &&
								( attachedJustification = lastAttachedJustificationItem->attachedJustificationItem() ) != NULL )
									lastAttachedJustificationItem = attachedJustification;

								if( lastAttachedJustificationItem->hasCurrentCreationSentenceNr() )
									{
									if( ( specificationResult = involvedSpecificationItem->getAssumptionLevel() ).result == RESULT_OK )
										{
										previousAssumptionLevel = specificationResult.assumptionLevel;

										if( lastAttachedJustificationItem->attachJustification( attachJustificationItem, involvedSpecificationItem ) == RESULT_OK )
											{
											if( ( involvedSpecificationItem->isConcludedAssumption() ||

											( justificationTypeNr != JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION &&
											justificationTypeNr != JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ) ) &&

											// Skip on multiple relation context words
											( involvedSpecificationItem->isOlderItem() ||
											!involvedSpecificationItem->hasRelationContext() ||
											myWordItem()->nContextWordsInAllWords( involvedSpecificationItem->relationContextNr(), involvedSpecificationItem->specificationWordItem() ) == 1 ) )
												{
												if( ( specificationResult = involvedSpecificationItem->recalculateAssumptionLevel() ).result == RESULT_OK )
													{
													if( involvedSpecificationItem->isOlderItem() &&
													!involvedSpecificationItem->hasRelationContext() &&
													specificationResult.assumptionLevel != previousAssumptionLevel )
														{
														// Conclusion
														if( specificationResult.assumptionLevel == NO_ASSUMPTION_LEVEL )
															{
															if( involvedSpecificationItem->markAsConcludedAssumption( false ) != RESULT_OK )
																return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark the involved specification as concluded assumption" );
															}

														// Write adjusted specification
														if( myWordItem()->writeUpdatedSpecification( true, false, false, involvedSpecificationItem ) != RESULT_OK )
															return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an adjusted specification" );
														}
													}
												else
													return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to recalculate the assumption level" );
												}
											}
										else
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to attach a justification item" );
										}
									else
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to get the assumption level" );
									}
								else	// Otherwise, attach the justification item to the given specification item
									{
									if( attachJustificationItem->hasAttachedJustification() )
										{
										lastAttachedJustificationItem = firstJustificationItem;

										while( lastAttachedJustificationItem->hasCurrentCreationSentenceNr() &&
										( attachedJustification = lastAttachedJustificationItem->attachedJustificationItem() ) != NULL )
											{
											previousAttachedJustificationItem = lastAttachedJustificationItem;
											lastAttachedJustificationItem = attachedJustification;
											}

										// The last attached justification item has no attached justification item
										if( !lastAttachedJustificationItem->hasAttachedJustification() )
											{
											if( previousAttachedJustificationItem != NULL )
												{
												// Attach attachJustificationItem to a copy of the last attached justification item
												if( ( justificationResult = copyJustification( false, lastAttachedJustificationItem->primarySpecificationItem(), lastAttachedJustificationItem->secondarySpecificationItem(), attachJustificationItem, lastAttachedJustificationItem ) ).result == RESULT_OK )
													{
													if( ( createdJustificationItem = justificationResult.createdJustificationItem ) != NULL )
														{
														// Link the created justification item to the previous attached justification item
														if( previousAttachedJustificationItem->changeAttachedJustification( createdJustificationItem ) == RESULT_OK )
															{
															if( replaceJustification( false, false, false, lastAttachedJustificationItem, createdJustificationItem, involvedSpecificationItem, NULL ) == RESULT_OK )
																{
																if( myWordItem()->recalculateAssumptions() != RESULT_OK )
																	return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to recalculate my assumptions" );
																}
															else
																return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the last attached justification item" );
															}
														else
															return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the attached justification item of the previous justification item" );
														}
													else
														return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't create a justification item" );
													}
												else
													return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to copy the last attached justification item with a different first justification item" );
												}
											else
												return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The previous attached justification item is undefined. I have no solution implemented to solve this problem" );
											}
										else
											return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The last attached justification item isn't the only old one. I have no solution implemented to solve this problem" );
										}
									else
										{
										if( involvedSpecificationItem->attachJustificationToSpecification( attachJustificationItem ) != RESULT_OK )
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to attach the justification item to the given involved specification item" );
										}
									}
								}
							else
								{
								if( !isSkipUpdateJustification )
									{
									if( myWordItem()->updateJustificationInSpecifications( false, false, false, obsoleteJustificationItem, attachJustificationItem ) != RESULT_OK )
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to update a question justification item by a conclusion justification item" );
									}
								}
							}
						else
							return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given involved specification item has no justification item" );
						}
					else
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given involved specification item is replaced or deleted" );
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item isn't active" );
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given involved specification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given attached justification item is undefined" );

		return RESULT_OK;
		}

	ResultType checkForConfirmedJustifications( SpecificationItem *secondarySpecificationItem, SpecificationItem *involvedSpecificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem *secondarySpecificationJustificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForConfirmedJustifications";

		if( secondarySpecificationItem != NULL )
			{
			if( involvedSpecificationItem != NULL )
				{
				if( ( secondarySpecificationJustificationItem = secondaryAssumptionJustificationItem( !secondarySpecificationItem->isPossessive(), secondarySpecificationItem->specificationWordItem() ) ) == NULL )
					{
					if( secondarySpecificationItem->isUserSpecification() &&
					( secondarySpecificationJustificationItem = involvedSpecificationItem->oppositePossessiveConditionalSpecificationAssumptionJustificationItem( secondarySpecificationItem ) ) != NULL )
						{
						if( replaceJustification( true, false, false, secondarySpecificationJustificationItem, NULL, involvedSpecificationItem, NULL ) != RESULT_OK )
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace an opposite possessive conditional specification assumption justification item without primary specification" );
						}
					}
				else
					{
					if( ( justificationResult = addJustification( true, false, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, secondarySpecificationJustificationItem->orderNr, secondarySpecificationJustificationItem->originalSentenceNr(), NULL, NULL, secondarySpecificationItem, NULL, NULL ) ).result == RESULT_OK )
						{
						if( justificationResult.createdJustificationItem != NULL )
							{
							if( replaceJustification( true, false, false, secondarySpecificationJustificationItem, justificationResult.createdJustificationItem, involvedSpecificationItem, NULL ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace an old secondary justification item without primary specification" );
							}
						else
							return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find or create a justification without primary specification" );
						}
					else
						return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add a justification without primary specification" );
					}
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given involved justification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given secondary justification item is undefined" );

		return RESULT_OK;
		}

	ResultType checkForUnreferencedJustifications()
		{
		if( checkForUnreferencedJustifications( true ) == RESULT_OK )
			return checkForUnreferencedJustifications( false );

		return commonVariables()->result;
		}

	ResultType checkForReplacedOrDeletedSpecifications()
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( commonVariables()->result == RESULT_OK &&
		searchItem != NULL )
			{
			searchItem->checkForReplacedOrDeletedSpecifications();
			searchItem = searchItem->nextJustificationItem();
			}

		searchItem = firstReplacedJustificationItem();

		while( commonVariables()->result == RESULT_OK &&
		searchItem != NULL )
			{
			searchItem->checkForDeletedSpecifications();
			searchItem = searchItem->nextJustificationItem();
			}

		return commonVariables()->result;
		}

	ResultType checkJustificationForUsage( JustificationItem *unusedJustificationItem )
		{
		if( checkJustificationForUsage( true, unusedJustificationItem ) == RESULT_OK )
			{
			if( unusedJustificationItem != NULL &&
			unusedJustificationItem->isReplacedItem() )
				return checkJustificationForUsage( false, unusedJustificationItem );
			}

		return commonVariables()->result;
		}

	ResultType checkSpecificationItemForUsage( SpecificationItem *unusedSpecificationItem )
		{
		if( checkSpecificationItemForUsage( true, unusedSpecificationItem ) == RESULT_OK )
			return checkSpecificationItemForUsage( false, unusedSpecificationItem );

		return commonVariables()->result;
		}

	ResultType replaceJustification( bool hasConfirmedSpecification, bool isExclusiveGeneralization, bool isExclusiveSpecification, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem, SpecificationItem *replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		bool hasCorrectedAssumption;
		bool hasCorrectedAssumptionByKnowledge = myWordItem()->hasCorrectedAssumptionByKnowledge();
		bool hasCorrectedAssumptionByOppositeQuestion = myWordItem()->hasCorrectedAssumptionByOppositeQuestion();
		bool isUpdatedPrimarySpecificationWordCollectedWithItself = false;
		JustificationItem *attachedJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *relatedSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustification";

		if( obsoleteJustificationItem != NULL )
			{
			if( involvedSpecificationItem != NULL )
				{
				obsoleteJustificationItem = obsoleteJustificationItem->updatedJustificationItem();

				if( obsoleteJustificationItem->isOlderItem() )
					{
					if( obsoleteJustificationItem->replacingJustificationItem == NULL )
						{
						if( replacingJustificationItem == NULL ||
						replacingJustificationItem->replacingJustificationItem == NULL )
							{
							if( obsoleteJustificationItem != replacingJustificationItem )
								{
								hasCorrectedAssumption = ( hasCorrectedAssumptionByKnowledge ||
															hasCorrectedAssumptionByOppositeQuestion );

								// Corrected assumption
								if( hasCorrectedAssumption &&
								replacingJustificationItem != NULL &&
								replacingJustificationItem->isGeneralizationAssumption() )
									{
									if( replacingJustificationItem->primarySpecificationWordItem() == involvedSpecificationItem->specificationWordItem() )
										{
										if( myWordItem()->writeUpdatedSpecification( false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, involvedSpecificationItem ) == RESULT_OK )
											{
											// Corrected negative assumption
											if( ( specificationResult = myWordItem()->findRelatedSpecification( false, involvedSpecificationItem->isAssignment(), involvedSpecificationItem->isArchivedAssignment(), involvedSpecificationItem->isExclusiveSpecification(), true, involvedSpecificationItem->isPossessive(), NO_QUESTION_PARAMETER, involvedSpecificationItem->specificationCollectionNr(), involvedSpecificationItem->generalizationContextNr(), involvedSpecificationItem->specificationContextNr(), involvedSpecificationItem->relationContextNr(), involvedSpecificationItem->specificationWordItem(), NULL ) ).result == RESULT_OK )
												{
												if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL )
													{
													if( myWordItem()->writeUpdatedSpecification( false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, relatedSpecificationItem ) == RESULT_OK )
														{
														if( hasCorrectedAssumptionByOppositeQuestion )
															{
															if( myWordItem()->replaceOrDeleteSpecification( relatedSpecificationItem, involvedSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
																return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace a corrected negative assumption" );
															}
														}
													else
														return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write a corrected negative assumption" );
													}
												}
											else
												return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find a negative related specification" );
											}
										else
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an adjusted specification" );
										}

									if( ( attachedJustificationItem = replacingJustificationItem->attachedJustificationItem() ) != NULL )
										{
										if( hasCorrectedAssumptionByKnowledge )
											{
											isUpdatedPrimarySpecificationWordCollectedWithItself = attachedJustificationItem->hasUpdatedPrimarySpecificationWordCollectedWithItself();

											if( isUpdatedPrimarySpecificationWordCollectedWithItself &&
											attachedJustificationItem->hasReplacedPrimarySpecification() )
												{
												if( replaceJustification( hasConfirmedSpecification, isExclusiveGeneralization, isExclusiveSpecification, attachedJustificationItem, NULL, involvedSpecificationItem, NULL ) != RESULT_OK )
													return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the attached justification item with replaced primary specification" );
												}
											}

										if( !isUpdatedPrimarySpecificationWordCollectedWithItself )
											{
											if( replaceOrDeleteJustification( replacingJustificationItem ) == RESULT_OK )
												replacingJustificationItem = NULL;
											else
												return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to delete a corrected justification" );
											}
										}
									}

								attachedJustificationItem = obsoleteJustificationItem->attachedJustificationItem();

								if( attachedJustificationItem != NULL &&							// Old justification has an attached justification,
								attachedJustificationItem->isActiveItem() &&						// that is active,
								replacingJustificationItem != NULL &&
								replacingJustificationItem != attachedJustificationItem &&
								replacingJustificationItem->hasCurrentActiveSentenceNr() &&			// but the replacing one is current
								!replacingJustificationItem->hasAttachedJustification() )			// and it has no attached justifications
									{
									if( replacingJustificationItem->changeAttachedJustification( attachedJustificationItem ) != RESULT_OK )
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change an attached justification item" );
									}

								if( attachedJustificationItem == NULL ||
								!attachedJustificationItem->isActiveItem() ||

								// Replacing justification item has the same attached justification item
								( replacingJustificationItem != NULL &&

								( replacingJustificationItem == attachedJustificationItem ||
								replacingJustificationItem->attachedJustificationItem() == attachedJustificationItem ) ) )
									{
									if( obsoleteJustificationItem->isActiveItem() )
										{
										if( replaceOrDeleteJustification( obsoleteJustificationItem ) == RESULT_OK )
											{
											if( myWordItem()->updateJustificationInSpecifications( hasConfirmedSpecification, isExclusiveSpecification, isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
												{
												if( replacingJustificationItem != NULL )
													{
													if( adjustedQuestionReplacingJustificationItem_ != NULL &&
													!replacingJustificationItem->hasPrimaryUserSpecification() &&
													replacingJustificationItem->isQuestionJustification() )
														{
														if( replaceJustification( false, false, false, replacingJustificationItem, adjustedQuestionReplacingJustificationItem_, involvedSpecificationItem, NULL ) != RESULT_OK )
															return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the replacing justification item of an adjusted question" );
														}

													if( updateReplacedJustifications( obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
														obsoleteJustificationItem->replacingJustificationItem = replacingJustificationItem;
													else
														return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to update the replacing justification items of the replaced justification items" );
													}
												}
											else
												return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to update a justification in specifications of my word" );
											}
										else
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace an active justification item" );
										}
									else
										{
										if( hasCorrectedAssumptionByOppositeQuestion )
											{
											if( replaceJustificationOfCorrectedAssumptionByOppositeQuestion( obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
												return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the justification of a corrected assumption by opposite question" );
											}
										}
									}
								else
									{
									if( replacingCorrectedAssumptionByKnowledgeSpecificationItem != NULL )
										involvedSpecificationItem = replacingCorrectedAssumptionByKnowledgeSpecificationItem;

									if( involvedSpecificationItem->hasCurrentCreationSentenceNr() &&
									!involvedSpecificationItem->isReplacedItem() &&
									// Obsolete justification of adjusted question
									!attachedJustificationItem->isQuestionJustification() )
										{
										if( involvedSpecificationItem->changeFirstJustification( attachedJustificationItem ) != RESULT_OK )
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given obsolete justification item" );
										}
									else
										{
										if( !involvedSpecificationItem->isSelfGeneratedQuestion() &&
										( firstJustificationItem = involvedSpecificationItem->firstJustificationItem() ) != NULL )
											{
											if( !firstJustificationItem->isReplacedItem() )
												{
												if( replaceOrDeleteJustification( firstJustificationItem ) != RESULT_OK )
													return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the first justification item" );
												}

											if( hasCorrectedAssumption ||
											!myWordItem()->isJustificationInUse( attachedJustificationItem ) )
												{
												if( replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
													return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the attached justification item" );
												}
											}
										}
									}

								involvedSpecificationItem = involvedSpecificationItem->updatedSpecificationItem();

								// Check for concluded negative assumption
								if( involvedSpecificationItem->isNegative() &&
								!involvedSpecificationItem->isConcludedAssumption() &&
								obsoleteJustificationItem->isAssumptionJustification() &&
								replacingJustificationItem != NULL &&
								replacingJustificationItem->isConclusionJustification() )
									{
									if( involvedSpecificationItem->markAsConcludedAssumption( true ) == RESULT_OK )
										{
										if( myWordItem()->writeUpdatedSpecification( true, false, false, involvedSpecificationItem ) != RESULT_OK )
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write the updated involved specification" );
										}
									else
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark the involved specification as concluded negative assumption" );
									}
								}
							else
								return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item and the given replacing justification item are the same justification item" );
							}
						else
							return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item has a replacing justification item itself" );
						}
					else
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item is already replaced, because it has a replacing justification item" );
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item isn't old" );
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given involved specification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item is undefined" );

		return RESULT_OK;
		}

	ResultType replaceObsoleteJustificationsOfAdjustedQuestion( JustificationItem *replacingJustificationItem, SpecificationItem *adjustedQuestionSpecificationItem )
		{
		SpecificationResultType specificationResult;
		JustificationItem *attachedJustificationItem;
		JustificationItem *currentJustificationItem;
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *createdSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceObsoleteJustificationsOfAdjustedQuestion";

		adjustedQuestionReplacingJustificationItem_ = replacingJustificationItem;

		if( replacingJustificationItem != NULL )
			{
			if( adjustedQuestionSpecificationItem != NULL )
				{
				if( ( currentJustificationItem = adjustedQuestionSpecificationItem->firstJustificationItem() ) != NULL )
					{
					do	{
						attachedJustificationItem = currentJustificationItem->attachedJustificationItem();
						primarySpecificationItem = currentJustificationItem->primarySpecificationItem();

						if( primarySpecificationItem != NULL &&
						primarySpecificationItem->isOlderItem() )
							{
							if( replaceJustification( false, false, true, currentJustificationItem, replacingJustificationItem, adjustedQuestionSpecificationItem, NULL ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the current justification item of an adjusted question" );
							}
						}
					while( ( currentJustificationItem = attachedJustificationItem ) != NULL );

					if( !adjustedQuestionSpecificationItem->isReplacedItem() )
						{
						if( ( specificationResult = myWordItem()->copySpecification( adjustedQuestionSpecificationItem->isInactiveAssignment(), adjustedQuestionSpecificationItem->isArchivedAssignment(), adjustedQuestionSpecificationItem->isAnsweredQuestion(), adjustedQuestionSpecificationItem->isExclusiveSpecification(), adjustedQuestionSpecificationItem->assignmentLevel(), adjustedQuestionSpecificationItem->generalizationCollectionNr(), adjustedQuestionSpecificationItem->specificationCollectionNr(), adjustedQuestionSpecificationItem->relationContextNr(), replacingJustificationItem, adjustedQuestionSpecificationItem ) ).result == RESULT_OK )
							{
							if( ( createdSpecificationItem = specificationResult.createdSpecificationItem ) != NULL )
								{
								if( myWordItem()->replaceOrDeleteSpecification( adjustedQuestionSpecificationItem, createdSpecificationItem ) != RESULT_OK )
									return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace or delete a specification" );
								}
							else
								return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't create the adjusted question specification" );
							}
						else
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to copy a specification with a different first justification item" );
						}
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given adjusted question specification item isn't self-generated" );
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given adjusted question specification item is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item is undefined" );

		return RESULT_OK;
		}

	ResultType replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteJustification";

		if( obsoleteJustificationItem != NULL )
			{
			if( obsoleteJustificationItem->hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( false, obsoleteJustificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to delete an obsolete justification" );
				}
			else
				{
				if( replaceItem( obsoleteJustificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace an obsolete justification" );
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item is undefined" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		// Not fully implemented yet
		JustificationItem *searchItem = firstActiveJustificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeJustificationItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store a justification item in the database" );
				}

			searchItem = searchItem->nextJustificationItem();
			}

		searchItem = firstReplacedJustificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeJustificationItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced justification item in the database" );
				}

			searchItem = searchItem->nextJustificationItem();
			}

		return RESULT_OK;
		}
*/
	ResultType updateSpecificationsInJustifications( bool hasConfirmedSpecificationInMainWord, bool isActiveItem, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		JustificationResultType justificationResult;
		bool isReplacePrimarySpecification;
		bool isReplaceSecondarySpecification;
		bool isReplacingSpecificationWordCollectedWithItself = false;
		bool hasCorrectedAssumption = myWordItem()->hasCorrectedAssumption();
		bool hasCorrectedAssumptionByKnowledge = myWordItem()->hasCorrectedAssumptionByKnowledge();
		bool isExclusiveGeneralization = false;
		bool isExclusiveSpecification = ( replacingSpecificationItem == NULL ? false : replacingSpecificationItem->isExclusiveSpecification() );
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *searchItem = firstJustificationItem( isActiveItem );
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateSpecificationsInJustifications";

		if( obsoleteSpecificationItem != NULL )
			{
			if( replacingSpecificationItem != NULL )
				{
				if( !obsoleteSpecificationItem->isQuestion() &&
				replacingSpecificationItem->hasExclusiveGeneralizationCollection() )
					isExclusiveGeneralization = true;

				if( replacingSpecificationItem->isSpecificationWordCollectedWithItself() )
					isReplacingSpecificationWordCollectedWithItself = true;
				}

			while( searchItem != NULL )
				{
				primarySpecificationItem = searchItem->primarySpecificationItem();
				secondarySpecificationItem = searchItem->secondarySpecificationItem();

				isReplacePrimarySpecification = ( primarySpecificationItem == obsoleteSpecificationItem );
				isReplaceSecondarySpecification = ( secondarySpecificationItem == obsoleteSpecificationItem );

				if( isReplacePrimarySpecification ||
				isReplaceSecondarySpecification )
					{
					if( replacingSpecificationItem != NULL &&
					searchItem->hasCurrentCreationSentenceNr() )
						{
						if( isReplacePrimarySpecification )
							{
							if( searchItem->changePrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the primary specificationItem item of a justification item" );
							}

						if( isReplaceSecondarySpecification )
							{
							if( searchItem->changeSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the secondary specificationItem item of a justification item" );
							}

						searchItem = firstJustificationItem( isActiveItem );
						}
					else
						{
						createdJustificationItem = NULL;

						if( replacingSpecificationItem != NULL &&

						( !isActiveItem ||
						searchItem->isQuestionJustification() ||
						// Avoid creating unreferenced justifications
						myWordItem()->isJustificationInUse( searchItem ) ) )
							{
							if( ( justificationResult = addJustification( isActiveItem, ( isReplacingSpecificationWordCollectedWithItself && isReplaceSecondarySpecification && searchItem->isPossessiveReversibleAssumptionOrConclusion() ), searchItem->justificationTypeNr(), searchItem->orderNr, searchItem->originalSentenceNr(), ( isReplacePrimarySpecification ? replacingSpecificationItem : primarySpecificationItem ), searchItem->anotherPrimarySpecificationItem(), ( isReplaceSecondarySpecification ? replacingSpecificationItem : secondarySpecificationItem ), searchItem->anotherSecondarySpecificationItem(), searchItem->attachedJustificationItem() ) ).result == RESULT_OK )
								{
								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									{
									if( justificationResult.foundJustificationItem != searchItem )
										createdJustificationItem = justificationResult.foundJustificationItem;
									else
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "No justification item is created, and the found justificiation item is the same as the search item itself" );
									}
								else
									{
									if( !isActiveItem )
										searchItem->replacingJustificationItem = createdJustificationItem;
									}
								}
							else
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add a justification item" );
							}

						if( isActiveItem )
							{
							if( replaceJustification( hasConfirmedSpecificationInMainWord, isExclusiveGeneralization, isExclusiveSpecification, searchItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : NULL ) ) == RESULT_OK )
								searchItem = ( hasCorrectedAssumption ? searchItem->nextJustificationItem() : firstActiveJustificationItem() );
							else
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace a justification item" );
							}
						else
							searchItem = searchItem->nextJustificationItem();
						}
					}
				else
					searchItem = searchItem->nextJustificationItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete specification item is undefined" );

		return RESULT_OK;
		}

	ResultType writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelatedJustificationSpecifications";

		while( searchItem != NULL )
			{
			if( !searchItem->hasBeenWritten &&
			searchItem->justificationTypeNr() == justificationTypeNr &&
			( secondarySpecificationItem = searchItem->secondarySpecificationItem() ) != NULL )
				{
				if( !secondarySpecificationItem->isExclusiveSpecification() &&
				secondarySpecificationItem->specificationCollectionNr() == specificationCollectionNr )
					{
					if( ( generalizationWordItem = secondarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( generalizationWordItem->writeJustificationSpecification( false, secondarySpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write a secondary justification specification" );
						}
					else
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The secondary specification item has no generalization word" );
					}
				}

			searchItem = searchItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	JustificationItem *negativeAssumptionOrConclusionJustificationItem( SpecificationItem *anotherPrimarySpecificationItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		if( anotherPrimarySpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isNegativeAssumptionOrConclusion() &&
				searchItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem )
					return searchItem;

				searchItem = searchItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *obsoleteAssumptionJustificationItem()
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->isAssumptionJustification() &&
			!myWordItem()->isJustificationInUse( searchItem ) )
				return searchItem;

			searchItem = searchItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *oldCorrectedAssumptionByOppositeQuestionJustificationItem()
		{
		return oldCorrectedAssumptionByOppositeQuestionJustificationItem_;
		}

	JustificationItem *secondarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->justificationTypeNr() == justificationTypeNr &&
				searchItem->secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isSpecificationSubstitutionPartOfAssumption() &&
				searchItem->secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *oldSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		JustificationItem *searchItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;

		while( searchItem != NULL )
			{
			if( searchItem->isSuggestiveQuestionAssumption() &&
			( secondarySpecificationItem = searchItem->secondarySpecificationItem() ) != NULL )
				{
				if( secondarySpecificationItem->isOlderItem() )
					return secondarySpecificationItem;
				}

			searchItem = searchItem->nextJustificationItem();
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
