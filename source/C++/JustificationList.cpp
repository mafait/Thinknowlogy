/*
 *	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items for the
 *					self-generated knowledge
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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
	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem_;


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
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I've found an unreferenced active justification item" );

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
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I've found an attached justification item that is still in use" );

				if( searchItem->replacingJustificationItem == unusedJustificationItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I've found a replacing justification item that is still in use" );

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
				correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

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
											if( ( justificationResult = copyJustificationItem( attachedJustificationItem->primarySpecificationItem(), attachedJustificationItem->secondarySpecificationItem(), NULL, attachedJustificationItem ) ).result == RESULT_OK )
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
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to remove the attached justification item of the replacing justification item" );
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

	JustificationItem *getJustificationItem( bool hasFeminineOrMasculineProperNameEnding, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *updatedAnotherSecondarySpecificationItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( searchItem != NULL )
			{
			if( searchItem->primarySpecificationItem() == primarySpecificationItem &&
			searchItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchItem->secondarySpecificationItem() == secondarySpecificationItem &&
			searchItem->anotherSecondarySpecificationItem() == updatedAnotherSecondarySpecificationItem &&
			searchItem->hasFeminineOrMasculineProperNameEnding() == hasFeminineOrMasculineProperNameEnding )
				return searchItem;

			searchItem = searchItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationResultType getRelatedQuestionJustificationOrderNr( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationResultType justificationResult;
		SpecificationResultType specificationResult;
		JustificationItem *searchItem = firstActiveJustificationItem();
		WordItem *primaryGeneralizationWord;
		WordItem *secondaryGeneralizationWord;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelatedQuestionJustificationOrderNr";

		if( primarySpecificationItem != NULL )
			{
			if( anotherPrimarySpecificationItem != NULL )
				{
				if( secondarySpecificationItem != NULL )
					{
					if( ( secondaryGeneralizationWord = secondarySpecificationItem->generalizationWordItem() ) != NULL )
						{
						if( ( specificationResult = secondaryGeneralizationWord->findRelatedSpecification( false, secondarySpecificationItem ) ).result == RESULT_OK )
							{
							if( specificationResult.isFirstRelatedSpecification )
								{
								if( ( primaryGeneralizationWord = primarySpecificationItem->generalizationWordItem() ) != NULL )
									{
									if( ( specificationResult = primaryGeneralizationWord->findRelatedSpecification( false, primarySpecificationItem ) ).result == RESULT_OK )
										{
										while( searchItem != NULL )
											{
											if( searchItem->isQuestionJustification() &&
											searchItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&

											( specificationResult.relatedSpecificationItem == NULL ||
											searchItem->primarySpecificationItem() == specificationResult.relatedSpecificationItem ) )
												{
												justificationResult.foundOrderNr = searchItem->orderNr;
												return justificationResult;
												}

											searchItem = searchItem->nextJustificationItem();
											}
										}
									else
										addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find a related specification of the given primary specification item" );
									}
								else
									startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The generalization word of the given primary specification item is undefined" );
								}
							}
						else
							addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to find a related specification of the given secondary specification item" );
						}
					else
						startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The generalization word of the given secondary specification item is undefined" );
					}
				else
					startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given secondary specification item is undefined" );
				}
			else
				startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given another primary specification item is undefined" );
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given primary specification item is undefined" );

		justificationResult.result = commonVariables()->result;
		return justificationResult;
		}

	JustificationResultType getSimilarSpecificationSubstitutionAssumptionOrderNr( SpecificationItem *primarySpecificationItem )
		{
		JustificationResultType justificationResult;
		unsigned int primarySpecificationCollectionNr;
		JustificationItem *searchItem = firstActiveJustificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "getSimilarSpecificationSubstitutionAssumptionOrderNr";

		if( primarySpecificationItem != NULL )
			{
			if( ( primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
				{
				while( searchItem != NULL )
					{
					if( searchItem->isSpecificationSubstitutionAssumption() &&
					searchItem->primarySpecificationCollectionNr() == primarySpecificationCollectionNr )
						{
						justificationResult.foundOrderNr = searchItem->orderNr;
						return justificationResult;
						}

					searchItem = searchItem->nextJustificationItem();
					}
				}
			else
				startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given primary specification item has no specification collection" );
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given primary specification item is undefined" );

		justificationResult.result = commonVariables()->result;
		return justificationResult;
		}


	protected:
	// Constructor / deconstructor

	JustificationList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		// Private constructible variables

		adjustedQuestionReplacingJustificationItem_ = NULL;
		correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;

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

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class JustificationList has inactive items." );

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class JustificationList has archived items." );

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
		correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;
		}

	JustificationResultType addJustification( bool hasFeminineOrMasculineProperNameEnding, bool isForcingNewJustification, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		JustificationResultType justificationResult;
		bool isSimilarQuestionJustification = false;
		JustificationItem *foundJustificationItem = NULL;
		SpecificationItem *updatedPrimarySpecificationItem = ( primarySpecificationItem == NULL ? NULL : primarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedAnotherPrimarySpecificationItem = ( anotherPrimarySpecificationItem == NULL ? NULL : anotherPrimarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedSecondarySpecificationItem = ( secondarySpecificationItem == NULL ? NULL : secondarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedAnotherSecondarySpecificationItem = ( anotherSecondarySpecificationItem == NULL ? NULL : anotherSecondarySpecificationItem->updatedSpecificationItem() );
		char functionNameString[FUNCTION_NAME_LENGTH] = "addJustification";

		if( primarySpecificationItem != NULL ||
		secondarySpecificationItem != NULL )
			{
			// Skipping search, forcing to create a new justification item
			if( !isForcingNewJustification )
				foundJustificationItem = getJustificationItem( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem );

			if( foundJustificationItem == NULL ||

			// Exceptions
			( foundJustificationItem->justificationTypeNr() == justificationTypeNr &&

			( attachedJustificationItem != NULL &&
			foundJustificationItem->attachedJustificationItem() != attachedJustificationItem ) ) )
				{
				if( commonVariables()->currentItemNr < MAX_ITEM_NR )
					{
					if( orderNr == NO_ORDER_NR )
						{
						if( foundJustificationItem == NULL &&
						updatedPrimarySpecificationItem != NULL &&
						updatedSecondarySpecificationItem != NULL &&
						( foundJustificationItem = olderPrimarySpecificationJustificationItem( ( updatedAnotherPrimarySpecificationItem != NULL || updatedPrimarySpecificationItem->isSpecificationWordCollectedWithItself() ), false, justificationTypeNr, updatedSecondarySpecificationItem->specificationCollectionNr(), updatedPrimarySpecificationItem ) ) != NULL )
							orderNr = foundJustificationItem->orderNr;
						else
							{
							orderNr = highestJustificationOrderNr( justificationTypeNr );

							if( orderNr > NO_ORDER_NR &&
							updatedPrimarySpecificationItem != NULL &&
							updatedSecondarySpecificationItem != NULL )
								{
								switch( justificationTypeNr )
									{
									case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
										if( updatedPrimarySpecificationItem->isExclusiveSpecification() )
											{
											if( ( justificationResult = getSimilarSpecificationSubstitutionAssumptionOrderNr( updatedPrimarySpecificationItem ) ).result == RESULT_OK )
												{
												if( justificationResult.foundOrderNr > NO_ORDER_NR )
													orderNr = justificationResult.foundOrderNr;
												}
											else
												addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to get a similar specification substitution assumption justification order number" );
											}

										break;

									case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
										if( updatedAnotherPrimarySpecificationItem != NULL )
											{
											isSimilarQuestionJustification = true;

											if( ( justificationResult = getRelatedQuestionJustificationOrderNr( updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem ) ).result == RESULT_OK )
												{
												if( justificationResult.foundOrderNr > NO_ORDER_NR )
													orderNr = justificationResult.foundOrderNr;
												}
											else
												addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to get a related question justification order number" );
											}

										break;
									}
								}

							if( commonVariables()->result == RESULT_OK &&
							justificationResult.foundOrderNr == NO_ORDER_NR &&

							( orderNr == NO_ORDER_NR ||

							isSimilarQuestionJustification ||
							justificationTypeNr == JUSTIFICATION_TYPE_FEMININE_OR_MASCULINE_PROPER_NAME_ENDING_ASSUMPTION ||
							justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
							justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

							( updatedPrimarySpecificationItem != NULL &&

							( ( justificationTypeNr == JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION &&
							updatedPrimarySpecificationItem->isSpecificationWordCollectedWithItself() ) ||

							( justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
							// Skip self-generated question of corrected assumption by opposite question
							!updatedPrimarySpecificationItem->isCorrectedAssumption() ) ) ) ) )
								orderNr++;
							}
						}

					if( commonVariables()->result == RESULT_OK )
						{
						if( orderNr > NO_ORDER_NR )
							{
							if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNameEnding, justificationTypeNr, orderNr, originalSentenceNr, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem, attachedJustificationItem, commonVariables(), this, myWordItem() ) ) != NULL )
								{
								// Add justification item
								if( addItemToList( QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != RESULT_OK )
									addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active justification item" );
								}
							else
								startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to create a justification item" );
							}
						else
							startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The order number is undefined" );
						}
					}
				else
					startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );
				}
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "Both the given primary specification item and the given secondary specification item are undefined" );

		justificationResult.foundJustificationItem = foundJustificationItem;
		justificationResult.result = commonVariables()->result;
		return justificationResult;
		}

	JustificationResultType copyJustificationItem( SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyJustificationItem";

		if( originalJustificationItem != NULL )
			return addJustification( originalJustificationItem->hasFeminineOrMasculineProperNameEnding(), false, originalJustificationItem->justificationTypeNr(), originalJustificationItem->orderNr, originalJustificationItem->originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem->anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem->anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		justificationResult.result = startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given original justification item is undefined" );
		return justificationResult;
		}

	ResultType attachJustification( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		SpecificationResultType specificationResult;
		bool isSkipUpdateJustification = false;
		unsigned short highestOrderNr;
		unsigned short justificationTypeNr;
		unsigned short previousAssumptionLevel;
		JustificationItem *attachedJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *lastAttachedJustificationItem;
		JustificationItem *obsoleteJustificationItem = NULL;
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
							if( attachJustificationItem->secondarySpecificationQuestion() == NULL &&
							( obsoleteJustificationItem = firstJustificationItem->secondarySpecificationQuestion() ) == NULL )
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
										if( !involvedSpecificationItem->hasRelationContext() &&

										( !attachJustificationItem->hasPrimarySpecificationWordCollectedWithItself() ||

										( attachedPrimarySpecificationItem != NULL &&
										attachedPrimarySpecificationItem->hasAssumptionLevel() ) ) )
											isSkipUpdateJustification = true;
										else
											{
											if( replaceJustification( false, obsoleteJustificationItem, attachJustificationItem, involvedSpecificationItem, NULL ) == RESULT_OK )
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
								( attachedJustificationItem = lastAttachedJustificationItem->attachedJustificationItem() ) != NULL )
									lastAttachedJustificationItem = attachedJustificationItem;

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
													specificationResult.assumptionLevel != previousAssumptionLevel &&

													// Avoid updates of high uncertainty mapping,
													// from 'maybe' of one level to 'maybe' of another level
													( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
													specificationResult.assumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ) )
														{
														// Conclusion
														if( specificationResult.assumptionLevel == NO_ASSUMPTION_LEVEL )
															{
															if( involvedSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
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
								else
									{
									if( !attachJustificationItem->hasAttachedJustification() )
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
									if( myWordItem()->updateJustificationInSpecifications( false, obsoleteJustificationItem, attachJustificationItem ) != RESULT_OK )
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

	ResultType replaceJustification( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem, SpecificationItem *replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		SpecificationResultType specificationResult;
		bool hasCorrectedAssumption;
		bool hasCorrectedAssumptionByKnowledge = myWordItem()->hasCorrectedAssumptionByKnowledge();
		bool hasCorrectedAssumptionByOppositeQuestion = myWordItem()->hasCorrectedAssumptionByOppositeQuestion();
		bool isUpdatedPrimarySpecificationWordCollectedWithItself = false;
		JustificationItem *attachedJustificationItem;
		SpecificationItem *relatedSpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustification";

		if( obsoleteJustificationItem != NULL )
			{
			if( involvedSpecificationItem != NULL )
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
										if( ( specificationResult = myWordItem()->findRelatedSpecification( false, involvedSpecificationItem->isAssignment(), involvedSpecificationItem->isArchivedAssignment(), involvedSpecificationItem->isExclusiveSpecification(), true, involvedSpecificationItem->isPossessive(), NO_QUESTION_PARAMETER, involvedSpecificationItem->specificationCollectionNr(), involvedSpecificationItem->generalizationContextNr(), involvedSpecificationItem->specificationContextNr(), involvedSpecificationItem->relationContextNr(), involvedSpecificationItem->specificationWordItem() ) ).result == RESULT_OK )
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
											if( replaceJustification( isExclusiveGeneralization, attachedJustificationItem, NULL, involvedSpecificationItem, NULL ) != RESULT_OK )
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

							// Old justification has an attached justification,
							if( attachedJustificationItem != NULL &&
							// that is active,
							attachedJustificationItem->isActiveItem() &&
							replacingJustificationItem != NULL &&
							replacingJustificationItem != attachedJustificationItem &&
							// but the replacing one is current
							replacingJustificationItem->hasCurrentActiveSentenceNr() &&
							// and it has no attached justifications
							!replacingJustificationItem->hasAttachedJustification() )
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
										if( myWordItem()->updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) == RESULT_OK )
											{
											if( replacingJustificationItem != NULL )
												{
												if( adjustedQuestionReplacingJustificationItem_ != NULL &&
												!replacingJustificationItem->hasPrimaryUserSpecification() &&
												replacingJustificationItem->isQuestionJustification() )
													{
													if( replaceJustification( false, replacingJustificationItem, adjustedQuestionReplacingJustificationItem_, involvedSpecificationItem, NULL ) != RESULT_OK )
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
									if( hasCorrectedAssumption ||
									!myWordItem()->isJustificationInUse( attachedJustificationItem ) )
										{
										if( replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
											return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the attached justification item" );
										}
									}
								}
							}
						else
							return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item and the given replacing justification item are the same justification item" );
						}
					else
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given replacing justification item has a replacing justification item itself" );
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given obsolete justification item is already replaced" );
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
							if( replaceJustification( false, currentJustificationItem, replacingJustificationItem, adjustedQuestionSpecificationItem, NULL ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace the current justification item of an adjusted question" );
							}
						}
					while( ( currentJustificationItem = attachedJustificationItem ) != NULL );

					if( !adjustedQuestionSpecificationItem->isReplacedItem() )
						{
						if( ( specificationResult = myWordItem()->copySpecificationItem( adjustedQuestionSpecificationItem->isInactiveAssignment(), adjustedQuestionSpecificationItem->isArchivedAssignment(), adjustedQuestionSpecificationItem->isAnsweredQuestion(), adjustedQuestionSpecificationItem->isExclusiveSpecification(), adjustedQuestionSpecificationItem->assignmentLevel(), adjustedQuestionSpecificationItem->generalizationCollectionNr(), adjustedQuestionSpecificationItem->specificationCollectionNr(), adjustedQuestionSpecificationItem->relationContextNr(), replacingJustificationItem, adjustedQuestionSpecificationItem ) ).result == RESULT_OK )
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
	ResultType updateSpecificationsInJustifications( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		JustificationResultType justificationResult;
		bool isReplacePrimarySpecification;
		bool isReplaceAnotherPrimarySpecification;
		bool isReplaceSecondarySpecification;
		bool isReplaceAnotherSecondarySpecification;
		bool hasObsoleteSpecificationItemRelationContext;
		bool hasCorrectedAssumption = myWordItem()->hasCorrectedAssumption();
		bool hasCorrectedAssumptionByKnowledge = myWordItem()->hasCorrectedAssumptionByKnowledge();
		bool hasReplacingSpecificationItemRelationContext = false;
		bool isExclusiveGeneralization = false;
		bool isReplacingSpecificationWordCollectedWithItself = false;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *searchItem = firstActiveJustificationItem();
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateSpecificationsInJustifications";

		if( obsoleteSpecificationItem != NULL )
			{
			hasObsoleteSpecificationItemRelationContext = obsoleteSpecificationItem->hasRelationContext();

			if( replacingSpecificationItem != NULL )
				{
				hasReplacingSpecificationItemRelationContext = replacingSpecificationItem->hasRelationContext();

				if( !obsoleteSpecificationItem->isQuestion() &&
				replacingSpecificationItem->hasExclusiveGeneralizationCollection() )
					isExclusiveGeneralization = true;

				if( replacingSpecificationItem->isSpecificationWordCollectedWithItself() )
					isReplacingSpecificationWordCollectedWithItself = true;
				}

			while( searchItem != NULL )
				{
				primarySpecificationItem = searchItem->primarySpecificationItem();
				anotherPrimarySpecificationItem = searchItem->anotherPrimarySpecificationItem();
				secondarySpecificationItem = searchItem->secondarySpecificationItem();
				anotherSecondarySpecificationItem = searchItem->anotherSecondarySpecificationItem();

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
				replacingSpecificationItem == NULL ||
				searchItem->hasFeminineOrMasculineProperNameEnding() ) )
					{
					if( replacingSpecificationItem != NULL &&
					searchItem->hasCurrentCreationSentenceNr() )
						{
						if( isReplacePrimarySpecification )
							{
							if( searchItem->changePrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the primary specificationItem item of a justification item" );
							}

						if( isReplaceAnotherPrimarySpecification )
							{
							if( searchItem->changeAnotherPrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the another primary specificationItem item of a justification item" );
							}

						if( isReplaceSecondarySpecification )
							{
							if( searchItem->changeSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the secondary specificationItem item of a justification item" );
							}

						if( isReplaceAnotherSecondarySpecification )
							{
							if( searchItem->changeAnotherSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to change the another secondary specificationItem item of a justification item" );
							}

						searchItem = firstActiveJustificationItem();
						}
					else
						{
						createdJustificationItem = NULL;

						if( replacingSpecificationItem != NULL &&

						( searchItem->isQuestionJustification() ||
						// Avoid creating unreferenced justifications
						myWordItem()->isJustificationInUse( searchItem ) ) )
							{
							if( ( justificationResult = addJustification( searchItem->hasFeminineOrMasculineProperNameEnding(), ( isReplacingSpecificationWordCollectedWithItself && isReplaceSecondarySpecification && searchItem->isPossessiveReversibleAssumptionOrConclusion() ), searchItem->justificationTypeNr(), searchItem->orderNr, searchItem->originalSentenceNr(), ( isReplacePrimarySpecification ? replacingSpecificationItem : primarySpecificationItem ), ( isReplaceAnotherPrimarySpecification ? replacingSpecificationItem : anotherPrimarySpecificationItem ), ( isReplaceSecondarySpecification ? replacingSpecificationItem : secondarySpecificationItem ), ( isReplaceAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem ), searchItem->attachedJustificationItem() ) ).result == RESULT_OK )
								{
								if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
									{
									if( justificationResult.foundJustificationItem != searchItem )
										createdJustificationItem = justificationResult.foundJustificationItem;
									else
										return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "No justification item is created, and the found justification item is the same as the search item itself" );
									}
								}
							else
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add a justification item" );
							}

						if( replaceJustification( isExclusiveGeneralization, searchItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : NULL ) ) == RESULT_OK )
							{
							searchItem = ( hasCorrectedAssumption ? searchItem->nextJustificationItem() : firstActiveJustificationItem() );

							if( hasCorrectedAssumptionByKnowledge &&
							searchItem != NULL &&
							searchItem->hasReplacedPrimarySpecification() )
								searchItem = firstActiveJustificationItem();
							}
						else
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to replace a justification item" );
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
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The generalization word of the given secondary specification item is undefined" );
					}
				}

			searchItem = searchItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem()
		{
		return correctedAssumptionByOppositeQuestionJustificationItem_;
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

	JustificationItem *obsoleteJustificationItem()
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		while( searchItem != NULL )
			{
			if( !myWordItem()->isJustificationInUse( searchItem ) )
				return searchItem;

			searchItem = searchItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *olderPrimarySpecificationJustificationItem( bool hasAnotherPrimarySpecification, bool hasSecondarySpecificationWithoutRelationContext, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;

		while( searchItem != NULL )
			{
			if( searchItem->isOlderItem() &&
			searchItem->justificationTypeNr() == justificationTypeNr &&

			( primarySpecificationItem == NULL ||
			searchItem->primarySpecificationItem() == primarySpecificationItem ) &&

			( secondarySpecificationItem = searchItem->secondarySpecificationItem() ) != NULL )
				{
				if( ( !hasAnotherPrimarySpecification ||
				searchItem->anotherPrimarySpecificationItem() != NULL ) &&

				( !hasSecondarySpecificationWithoutRelationContext ||
				!secondarySpecificationItem->hasRelationContext() ) &&

				( secondarySpecificationCollectionNr == NO_COLLECTION_NR ||
				secondarySpecificationItem->specificationCollectionNr() == secondarySpecificationCollectionNr ) )
					return searchItem;
				}

			searchItem = searchItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *olderSecondarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isOlderItem() &&
				searchItem->justificationTypeNr() == justificationTypeNr &&
				searchItem->secondarySpecificationItem() == secondarySpecificationItem )
					return searchItem;

				searchItem = searchItem->nextJustificationItem();
				}
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
