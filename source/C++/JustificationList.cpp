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

#include "SpecificationItem.cpp"

class JustificationList : private List
	{
	friend class WordItem;

	// Private constructed variables

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem_;


	// Private functions

	unsigned short highestJustificationOrderNr( unsigned short justificationTypeNr )
		{
		unsigned short highestOrderNr = NO_ORDER_NR;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->orderNr > highestOrderNr &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchJustificationItem->orderNr;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return highestOrderNr;
		}

	unsigned short justificationOrderNr( bool hasFeminineOrMasculineProperNameEnding, bool hasPossessivePrimarySpecification, unsigned short justificationTypeNr, unsigned int primarySpecificationCollectionNr )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationCollectionNr > NO_COLLECTION_NR )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem->hasPossessivePrimarySpecification() == hasPossessivePrimarySpecification &&
				searchJustificationItem->primarySpecificationCollectionNr() == primarySpecificationCollectionNr &&
				searchJustificationItem->hasFeminineOrMasculineProperNameEnding() == hasFeminineOrMasculineProperNameEnding )
					return searchJustificationItem->orderNr;

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NO_ORDER_NR;
		}

	unsigned short questionJustificationOrderNr( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		bool isCheckingSpecificationCompoundCollection;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *foundPrimarySpecificationItem;

		if( primarySpecificationItem != NULL &&
		secondarySpecificationItem != NULL )
			{
			isCheckingSpecificationCompoundCollection = ( anotherPrimarySpecificationItem == NULL &&
														primarySpecificationItem->hasSpecificationCompoundCollection() );

			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isQuestionJustification() )
					{
					if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem &&
					searchJustificationItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem )
						return searchJustificationItem->orderNr;

					if( isCheckingSpecificationCompoundCollection &&
					!searchJustificationItem->hasAnotherPrimarySpecification() &&
					( foundPrimarySpecificationItem = searchJustificationItem->primarySpecificationItem() ) != NULL )
						{
						if( foundPrimarySpecificationItem->specificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() )
							return searchJustificationItem->orderNr;
						}
					}

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NO_ORDER_NR;
		}

	ResultType checkForUnreferencedJustification( bool isActiveItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForUnreferencedJustification";

		while( searchJustificationItem != NULL )
			{
			if( !generalizationWordItem->isJustificationInUse( searchJustificationItem ) )
				return startError( functionNameString, NULL, "I have found an unreferenced justification item" );

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	ResultType checkJustificationForUsage( bool isActiveItem, JustificationItem *unusedJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given unused justification item is undefined" );

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->attachedJustificationItem() == unusedJustificationItem )
				return startError( functionNameString, NULL, "I have found an attached justification item that is still in use" );

			if( searchJustificationItem->replacingJustificationItem == unusedJustificationItem )
				return startError( functionNameString, NULL, "I have found a replacing justification item that is still in use" );

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	ResultType checkSpecificationItemForUsage( bool isActiveItem, SpecificationItem *unusedSpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationItemForUsage";

		if( unusedSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given unused specification item is undefined" );

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->primarySpecificationItem() == unusedSpecificationItem )
				return startError( functionNameString, NULL, "The primary specification item is still in use" );

			if( searchJustificationItem->anotherPrimarySpecificationItem() == unusedSpecificationItem )
				return startError( functionNameString, NULL, "The another primary specification item is still in use" );

			if( searchJustificationItem->secondarySpecificationItem() == unusedSpecificationItem )
				return startError( functionNameString, NULL, "The secondary specification item is still in use" );

			if( searchJustificationItem->anotherSecondarySpecificationItem() == unusedSpecificationItem )
				return startError( functionNameString, NULL, "The another secondary specification item is still in use" );

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

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

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete justification item is undefined" );

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing justification item is undefined" );

		correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

		if( ( secondarySpecificationWordItem = replacingJustificationItem->secondarySpecificationWordItem() ) == NULL )
			return startError( functionNameString, NULL, "The given replacing justification item has no secondary specification word item" );

		if( ( foundSpecificationItem = myWordItem()->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != NULL )
			{
			if( foundSpecificationItem->attachJustificationToSpecification( replacingJustificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to attach the replacing justification item to the found specification" );

			foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

			if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL )
				{
				if( ( attachedJustificationItem = firstJustificationItem->attachedJustificationItem() ) != NULL )
					{
					// Remove the obsolete justification item from the replacing justification item
					if( firstJustificationItem->changeAttachedJustification( NULL ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to remove the attached justification item of the replacing justification item" );

					if( replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to replace the attached justification item, without an attached justification item" );

					if( ( justificationResult = copyJustificationItem( attachedJustificationItem->primarySpecificationItem(), attachedJustificationItem->secondarySpecificationItem(), NULL, attachedJustificationItem ) ).result != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to copy the attached justification item" );

					if( justificationResult.createdJustificationItem == NULL )
						return startError( functionNameString, NULL, "I couldn't find or create a justification" );

					if( foundSpecificationItem->attachJustificationToSpecification( justificationResult.createdJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to attach the created justification item to the found specification" );
					}
				}
			}

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase( JustificationItem *searchJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasCurrentCreationSentenceNr() )
				{
				if( searchJustificationItem->storeJustificationItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a justification item in the database" );
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}
*/
	ResultType updateReplacedJustifications( JustificationItem *updateJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstReplacedJustificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateReplacedJustifications";

		if( updateJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given update justification item is undefined" );

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given replacing justification item is undefined" );

		if( replacingJustificationItem->isReplacedItem() )
			return startError( functionNameString, NULL, "The replaced justification item of the given update justification item is a replaced item" );

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->replacingJustificationItem == updateJustificationItem )
				searchJustificationItem->replacingJustificationItem = replacingJustificationItem;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

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

	JustificationItem *getJustificationItem( bool hasFeminineOrMasculineProperNameEnding, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem &&
			searchJustificationItem->anotherSecondarySpecificationItem() == anotherSecondarySpecificationItem &&
			searchJustificationItem->hasFeminineOrMasculineProperNameEnding() == hasFeminineOrMasculineProperNameEnding )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NULL;
		}


	protected:
	// Constructor

	JustificationList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		// Private constructed variables
		correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;

		initializeListVariables( WORD_JUSTIFICATION_LIST_SYMBOL, "JustificationList", commonVariables, myWordItem );
		}

	~JustificationList()
		{
		JustificationItem *deleteJustificationItem;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			deleteJustificationItem = searchJustificationItem;
			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			delete deleteJustificationItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class JustificationList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class JustificationList has archived items." );

		searchJustificationItem = firstReplacedJustificationItem();

		while( searchJustificationItem != NULL )
			{
			deleteJustificationItem = searchJustificationItem;
			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			delete deleteJustificationItem;
			}

		searchJustificationItem = (JustificationItem *)firstDeletedItem();

		while( searchJustificationItem != NULL )
			{
			deleteJustificationItem = searchJustificationItem;
			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			delete deleteJustificationItem;
			}
		}


	// Protected functions

	void clearJustificationHasBeenWritten()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			searchJustificationItem->hasJustificationBeenWritten = false;
			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}
		}

	void initializeJustificationVariables()
		{
		correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;
		}

	JustificationResultType addJustification( bool hasFeminineOrMasculineProperNameEnding, bool isForcingNewJustification, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		JustificationResultType justificationResult;
		bool isExclusivePrimarySpecification = false;
		bool isIncrementingOrderNr = false;
		unsigned short foundJustificationOrderNr;
		unsigned int primarySpecificationCollectionNr;
		JustificationItem *foundJustificationItem = NULL;
		SpecificationItem *updatedPrimarySpecificationItem = ( primarySpecificationItem == NULL ? NULL : primarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedAnotherPrimarySpecificationItem = ( anotherPrimarySpecificationItem == NULL ? NULL : anotherPrimarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedSecondarySpecificationItem = ( secondarySpecificationItem == NULL ? NULL : secondarySpecificationItem->updatedSpecificationItem() );
		SpecificationItem *updatedAnotherSecondarySpecificationItem = ( anotherSecondarySpecificationItem == NULL ? NULL : anotherSecondarySpecificationItem->updatedSpecificationItem() );
		char functionNameString[FUNCTION_NAME_LENGTH] = "addJustification";

		if( primarySpecificationItem == NULL &&
		secondarySpecificationItem == NULL )
			return startJustificationResultError( functionNameString, NULL, "Both the given primary specification item and the given secondary specification item are undefined" );

		// Skipping search, forcing to create a new justification item
		if( !isForcingNewJustification )
			foundJustificationItem = getJustificationItem( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem );

		if( foundJustificationItem == NULL ||

		( attachedJustificationItem != NULL &&
		foundJustificationItem->justificationTypeNr() == justificationTypeNr &&
		// Avoid looping
		foundJustificationItem->attachedJustificationItem() != attachedJustificationItem ) )
			{
			if( orderNr == NO_ORDER_NR )
				{
				if( foundJustificationItem == NULL &&
				updatedPrimarySpecificationItem != NULL &&
				updatedSecondarySpecificationItem != NULL &&
				( foundJustificationItem = olderComplexJustificationItem( false, justificationTypeNr, updatedSecondarySpecificationItem->specificationCollectionNr(), updatedPrimarySpecificationItem ) ) != NULL )
					orderNr = foundJustificationItem->orderNr;
				else
					{
					orderNr = highestJustificationOrderNr( justificationTypeNr );

					if( orderNr > NO_ORDER_NR &&
					updatedPrimarySpecificationItem != NULL &&
					updatedSecondarySpecificationItem != NULL )
						{
						// Group justification by selecting the order number
						switch( justificationTypeNr )
							{
							case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( updatedPrimarySpecificationItem->isExclusiveSpecification() )
									isExclusivePrimarySpecification = true;

								// Don't insert a break statement here

							case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
							case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
								if( ( isExclusivePrimarySpecification ||

								( ( updatedPrimarySpecificationItem->isSpecificationWordSpanishAmbiguous() &&
								updatedPrimarySpecificationItem->hasRelationContext() &&
								updatedPrimarySpecificationItem->generalizationWordItem() == myWordItem() ) ||

								( hasFeminineOrMasculineProperNameEnding &&
								updatedAnotherPrimarySpecificationItem != NULL &&
								updatedAnotherPrimarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) &&

								( primarySpecificationCollectionNr = updatedPrimarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
									{
									if( ( foundJustificationOrderNr = justificationOrderNr( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem->isPossessive(), justificationTypeNr, primarySpecificationCollectionNr ) ) == NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}

								break;

							case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
								// Spanish ambiguous
								if( updatedAnotherSecondarySpecificationItem == NULL )
									{
									if( ( foundJustificationOrderNr = questionJustificationOrderNr( updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem ) ) == NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}
							}
						}

					if( !isExclusivePrimarySpecification &&

					( isIncrementingOrderNr ||
					orderNr == NO_ORDER_NR ||
					justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
					justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

					( updatedPrimarySpecificationItem != NULL &&

					( ( justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&

					// Skip self-generated question of corrected assumption by opposite question
					( updatedAnotherPrimarySpecificationItem == NULL ||
					updatedPrimarySpecificationItem->hasRelationContext() ) ) ||

					( updatedPrimarySpecificationItem->isSpecificationWordSpanishAmbiguous() &&

					( justificationTypeNr == JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||

					( hasFeminineOrMasculineProperNameEnding &&
					justificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION &&
					updatedPrimarySpecificationItem->isPossessive() &&
					// If justification without feminine or masculine proper name ending exists
					getJustificationItem( false, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem ) != NULL ) ) ) ) ) ) )
						orderNr++;
					}
				}

			if( orderNr <= NO_ORDER_NR )
				return startJustificationResultError( functionNameString, NULL, "The order number is undefined" );

			if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNameEnding, justificationTypeNr, orderNr, originalSentenceNr, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem, attachedJustificationItem, commonVariables(), this, myWordItem() ) ) == NULL )
				return startJustificationResultError( functionNameString, NULL, "I failed to create a justification item" );

			// Add justification item
			if( addItemToList( QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != RESULT_OK )
				return addJustificationResultError( functionNameString, NULL, "I failed to add an active justification item" );
			}

		justificationResult.foundJustificationItem = foundJustificationItem;
		return justificationResult;
		}

	JustificationResultType copyJustificationItem( SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyJustificationItem";

		if( originalJustificationItem != NULL )
			return addJustification( originalJustificationItem->hasFeminineOrMasculineProperNameEnding(), false, originalJustificationItem->justificationTypeNr(), originalJustificationItem->orderNr, originalJustificationItem->originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem->anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem->anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		return startJustificationResultError( functionNameString, NULL, "The given original justification item is undefined" );
		}

	ResultType attachJustification( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		SpecificationResultType specificationResult;
		bool hasInvolvedSpecificationRelationContext;
		bool isPrimarySpecificationWordSpanishAmbiguous;
		bool isSkippingUpdateJustification = false;
		unsigned short highestOrderNr;
		unsigned short justificationTypeNr;
		unsigned short previousAssumptionLevel;
		JustificationItem *attachedJustificationItem;
		JustificationItem *firstJustificationItem;
		JustificationItem *lastAttachedJustificationItem;
		JustificationItem *obsoleteJustificationItem = NULL;
		SpecificationItem *attachedPrimarySpecificationItem;
		SpecificationItem *attachedSecondarySpecificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "attachJustification";

		if( attachJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given attached justification item is undefined" );

		if( involvedSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given involved specification item is undefined" );

		if( !attachJustificationItem->isActiveItem() )
			return startError( functionNameString, NULL, "The given attached justification item isn't active" );

		if( involvedSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, NULL, "The given involved specification item is replaced or deleted" );

		hasInvolvedSpecificationRelationContext = involvedSpecificationItem->hasRelationContext();
		isPrimarySpecificationWordSpanishAmbiguous = attachJustificationItem->isPrimarySpecificationWordSpanishAmbiguous();
		justificationTypeNr = attachJustificationItem->justificationTypeNr();

		attachedPrimarySpecificationItem = attachJustificationItem->primarySpecificationItem();
		attachedSecondarySpecificationItem = attachJustificationItem->secondarySpecificationItem();

		if( attachedPrimarySpecificationItem != NULL &&

		( ( !hasInvolvedSpecificationRelationContext &&
		!involvedSpecificationItem->isExclusiveSpecification() &&
		!attachedPrimarySpecificationItem->isExclusiveSpecification() &&

		( attachedPrimarySpecificationItem->hasRelationContext() ||
		!attachedPrimarySpecificationItem->isPossessive() ) ) ||

		( ( ( involvedSpecificationItem->isQuestion() &&
		attachedPrimarySpecificationItem->isPossessive() ) ||

		( !isPrimarySpecificationWordSpanishAmbiguous &&

		( attachedPrimarySpecificationItem->hasRelationContext() ||
		attachedPrimarySpecificationItem->isOlderItem() ) ) ) &&

		!involvedSpecificationItem->hasJustificationOfSameType( justificationTypeNr, attachedPrimarySpecificationItem, attachedSecondarySpecificationItem ) ) ) )
			{
			if( ( highestOrderNr = involvedSpecificationItem->highestAttachedJustificationOrderNr( justificationTypeNr ) ) >= MAX_ORDER_NR )
				return startSystemError( functionNameString, NULL, "Justification order number overflow" );

			attachJustificationItem->orderNr = ( highestOrderNr + 1 );
			}

		if( ( firstJustificationItem = involvedSpecificationItem->firstJustificationItem() ) == NULL )
			return startError( functionNameString, NULL, "The given involved specification item has no justification item" );

		if( attachJustificationItem->secondarySpecificationQuestion() == NULL &&
		( obsoleteJustificationItem = firstJustificationItem->secondarySpecificationQuestion() ) == NULL )
			{
			if( attachedPrimarySpecificationItem != NULL &&
			!attachedPrimarySpecificationItem->isPossessive() &&
			attachedPrimarySpecificationItem->hasRelationContext() )
				obsoleteJustificationItem = firstJustificationItem->primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem );
			else
				{
				// Typically for the Spanish language
				if( attachedSecondarySpecificationItem != NULL &&
				attachedSecondarySpecificationItem->isUserSpecification() &&
				
				( involvedSpecificationItem->isQuestion() ||
				involvedSpecificationItem->isSpecificationWordSpanishAmbiguous() ) )
					obsoleteJustificationItem = firstJustificationItem->obsoleteSpanishJustificationItem( attachedPrimarySpecificationItem, attachedSecondarySpecificationItem );
				}

			if( obsoleteJustificationItem != NULL )
				{
				if( obsoleteJustificationItem->isOlderItem() )
					{
					if( !hasInvolvedSpecificationRelationContext &&

					( !isPrimarySpecificationWordSpanishAmbiguous ||

					( attachedPrimarySpecificationItem != NULL &&
					attachedPrimarySpecificationItem->hasAssumptionLevel() ) ) )
						isSkippingUpdateJustification = true;
					else
						{
						if( replaceJustification( false, obsoleteJustificationItem, attachJustificationItem, involvedSpecificationItem, NULL ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace the obsolete justification item" );

						attachJustificationItem->orderNr = obsoleteJustificationItem->orderNr;
						}
					}
				else
					{
					obsoleteJustificationItem = NULL;

					if( ( highestOrderNr = involvedSpecificationItem->highestAttachedJustificationOrderNr( justificationTypeNr ) ) >= MAX_ORDER_NR )
						return startSystemError( functionNameString, NULL, "Justification order number overflow" );

					attachJustificationItem->orderNr = ( highestOrderNr + 1 );
					}
				}
			}

		if( obsoleteJustificationItem == NULL )
			{
			// Find a justification item with current sentence number, but without attached justification item
			lastAttachedJustificationItem = firstJustificationItem;

			while( ( attachedJustificationItem = lastAttachedJustificationItem->attachedJustificationItem() ) != NULL )
				lastAttachedJustificationItem = attachedJustificationItem;

			if( lastAttachedJustificationItem->hasCurrentCreationSentenceNr() )
				{
				if( ( specificationResult = involvedSpecificationItem->getAssumptionLevel() ).result != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to get the assumption level" );

				previousAssumptionLevel = specificationResult.assumptionLevel;

				if( lastAttachedJustificationItem->attachJustification( attachJustificationItem, involvedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to attach a justification item" );

				if( !hasInvolvedSpecificationRelationContext ||
				!involvedSpecificationItem->isOlderItem() )
					{
					if( ( specificationResult = involvedSpecificationItem->recalculateAssumptionLevel() ).result != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to recalculate the assumption level" );

					if( !hasInvolvedSpecificationRelationContext &&
					involvedSpecificationItem->isOlderItem() &&
					specificationResult.assumptionLevel != previousAssumptionLevel &&

					( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
					// Skip writing adjustments of uncertainty level higher than 'maybe'
					specificationResult.assumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ) )
						{
						// Conclusion
						if( specificationResult.assumptionLevel == NO_ASSUMPTION_LEVEL )
							{
							if( involvedSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
								return addError( functionNameString, NULL, "I failed to mark the involved specification as concluded assumption" );
							}

						// Write adjusted specification
						if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, involvedSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to write an adjusted specification" );
						}
					}
				}
			else
				{
				if( !attachJustificationItem->hasAttachedJustification() )
					{
					if( involvedSpecificationItem->attachJustificationToSpecification( attachJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to attach the justification item to the given involved specification item" );
					}
				}
			}
		else
			{
			if( !isSkippingUpdateJustification )
				{
				if( generalizationWordItem->updateJustificationInSpecifications( false, obsoleteJustificationItem, attachJustificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to update a question justification item by a conclusion justification item" );
				}
			}

		return RESULT_OK;
		}

	ResultType checkForUnreferencedJustification()
		{
		if( checkForUnreferencedJustification( true ) == RESULT_OK )
			return checkForUnreferencedJustification( false );

		return RESULT_OK;
		}

	ResultType checkForReplacedOrDeletedSpecification()
		{
		JustificationItem *searchJustificationItem;

		if( ( searchJustificationItem = firstActiveJustificationItem() ) != NULL )
			{
			do	;
			while( searchJustificationItem->checkForReplacedOrDeletedSpecification() == RESULT_OK &&
			( searchJustificationItem = searchJustificationItem->nextJustificationItem() ) != NULL );
			}

		if( ( searchJustificationItem = firstReplacedJustificationItem() ) != NULL )
			{
			do	;
			while( searchJustificationItem->checkForDeletedSpecifications() == RESULT_OK &&
			( searchJustificationItem = searchJustificationItem->nextJustificationItem() ) != NULL );
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
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		JustificationItem *attachedJustificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustification";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete justification item is undefined" );

		if( involvedSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given involved specification item is undefined" );

		if( obsoleteJustificationItem->replacingJustificationItem != NULL )
			return startError( functionNameString, NULL, "The given obsolete justification item is already replaced" );

		if( replacingJustificationItem != NULL &&
		replacingJustificationItem->replacingJustificationItem != NULL )
			return startError( functionNameString, NULL, "The given replacing justification item has a replacing justification item itself" );

		if( obsoleteJustificationItem == replacingJustificationItem )
			return startError( functionNameString, NULL, "The given obsolete justification item and the given replacing justification item are the same justification item" );

		hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();
		hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();

		// Corrected assumption
		if( ( hasCorrectedAssumptionByKnowledge ||
		hasCorrectedAssumptionByOppositeQuestion ) &&

		replacingJustificationItem != NULL &&
		replacingJustificationItem->isGeneralizationAssumption() )
			{
			if( replacingJustificationItem->primarySpecificationWordItem() == involvedSpecificationItem->specificationWordItem() )
				{
				if( generalizationWordItem->writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, involvedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to write an adjusted specification" );

				// Corrected negative assumption
				if( ( specificationResult = generalizationWordItem->findRelatedSpecification( false, involvedSpecificationItem->isAssignment(), involvedSpecificationItem->isArchivedAssignment(), involvedSpecificationItem->isExclusiveSpecification(), true, involvedSpecificationItem->isPossessive(), NO_QUESTION_PARAMETER, involvedSpecificationItem->specificationCollectionNr(), involvedSpecificationItem->generalizationContextNr(), involvedSpecificationItem->specificationContextNr(), involvedSpecificationItem->relationContextNr(), involvedSpecificationItem->specificationWordItem() ) ).result != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to find a negative related specification" );

				if( ( relatedSpecificationItem = specificationResult.relatedSpecificationItem ) != NULL )
					{
					if( generalizationWordItem->writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, relatedSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to write a corrected negative assumption" );

					if( hasCorrectedAssumptionByOppositeQuestion )
						{
						if( generalizationWordItem->replaceOrDeleteSpecification( false, relatedSpecificationItem, involvedSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to replace a corrected negative assumption" );
						}
					}
				}

			if( ( attachedJustificationItem = replacingJustificationItem->attachedJustificationItem() ) != NULL )
				{
				if( !hasCorrectedAssumptionByKnowledge ||
				!attachedJustificationItem->isUpdatedPrimarySpecificationWordSpanishAmbiguous() )
					{
					if( replaceOrDeleteJustification( replacingJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to delete a corrected justification" );

					replacingJustificationItem = NULL;
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
				return addError( functionNameString, NULL, "I failed to change an attached justification item" );
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
				if( replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to replace an active justification item" );

				if( generalizationWordItem->updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to update a justification in specifications of my word" );

				if( replacingJustificationItem != NULL )
					{
					if( updateReplacedJustifications( obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to update the replacing justification items of the replaced justification items" );

					obsoleteJustificationItem->replacingJustificationItem = replacingJustificationItem;
					}
				}
			else
				{
				if( hasCorrectedAssumptionByOppositeQuestion )
					{
					if( replaceJustificationOfCorrectedAssumptionByOppositeQuestion( obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to replace the justification of a corrected assumption by opposite question" );
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
					return addError( functionNameString, NULL, "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given obsolete justification item" );
				}
			else
				{
				if( hasCorrectedAssumptionByKnowledge ||
				hasCorrectedAssumptionByOppositeQuestion ||
				!generalizationWordItem->isJustificationInUse( attachedJustificationItem ) )
					{
					if( replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to replace the attached justification item" );
					}
				}
			}

		return RESULT_OK;
		}

	ResultType replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteJustification";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete justification item is undefined" );

		if( obsoleteJustificationItem->hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteJustificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to delete an obsolete justification" );
			}
		else
			{
			if( replaceItem( obsoleteJustificationItem ) != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to replace an obsolete justification" );
			}

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		if( storeChangesInFutureDatabase( firstActiveJustificationItem() ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to store active justification items in the database" );

		if( storeChangesInFutureDatabase( firstReplacedJustificationItem() ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to store replaced justification items in the database" );

		return RESULT_OK;
		}
*/
	ResultType updateSpecificationsInJustifications( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		JustificationResultType justificationResult;
		bool hasCorrectedAssumption;
		bool hasCorrectedAssumptionByKnowledge;
		bool isReplacingPrimarySpecification;
		bool isReplacingAnotherPrimarySpecification;
		bool isReplacingSecondarySpecification;
		bool isReplacingAnotherSecondarySpecification;
		bool hasObsoleteSpecificationItemRelationContext;
		bool hasReplacingSpecificationItemRelationContext = false;
		bool isExclusiveGeneralization = false;
		bool isReplacingSpecificationWordSpanishAmbiguous = false;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateSpecificationsInJustifications";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, NULL, "The given obsolete specification item is undefined" );

		hasCorrectedAssumption = generalizationWordItem->hasCorrectedAssumption();
		hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();
		hasObsoleteSpecificationItemRelationContext = obsoleteSpecificationItem->hasRelationContext();

		if( replacingSpecificationItem != NULL )
			{
			hasReplacingSpecificationItemRelationContext = replacingSpecificationItem->hasRelationContext();

			if( !obsoleteSpecificationItem->isQuestion() &&
			replacingSpecificationItem->hasExclusiveGeneralizationCollection() )
				isExclusiveGeneralization = true;

			if( replacingSpecificationItem->isSpecificationWordSpanishAmbiguous() )
				isReplacingSpecificationWordSpanishAmbiguous = true;
			}

		while( searchJustificationItem != NULL )
			{
			primarySpecificationItem = searchJustificationItem->primarySpecificationItem();
			anotherPrimarySpecificationItem = searchJustificationItem->anotherPrimarySpecificationItem();
			secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem();
			anotherSecondarySpecificationItem = searchJustificationItem->anotherSecondarySpecificationItem();

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
			replacingSpecificationItem == NULL ) )
				{
				if( replacingSpecificationItem != NULL &&
				searchJustificationItem->hasCurrentCreationSentenceNr() )
					{
					if( isReplacingPrimarySpecification )
						{
						if( searchJustificationItem->changePrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change the primary specificationItem item of a justification item" );
						}

					if( isReplacingAnotherPrimarySpecification )
						{
						if( searchJustificationItem->changeAnotherPrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change the another primary specificationItem item of a justification item" );
						}

					if( isReplacingSecondarySpecification )
						{
						if( searchJustificationItem->changeSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change the secondary specificationItem item of a justification item" );
						}

					if( isReplacingAnotherSecondarySpecification )
						{
						if( searchJustificationItem->changeAnotherSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to change the another secondary specificationItem item of a justification item" );
						}

					searchJustificationItem = firstActiveJustificationItem();
					}
				else
					{
					createdJustificationItem = NULL;

					if( replacingSpecificationItem != NULL &&

					( searchJustificationItem->isQuestionJustification() ||
					// Avoid creating unreferenced justifications
					generalizationWordItem->isJustificationInUse( searchJustificationItem ) ) )
						{
						if( ( justificationResult = addJustification( searchJustificationItem->hasFeminineOrMasculineProperNameEnding(), ( isReplacingSpecificationWordSpanishAmbiguous && isReplacingSecondarySpecification && searchJustificationItem->isPossessiveReversibleAssumptionOrConclusion() ), searchJustificationItem->justificationTypeNr(), searchJustificationItem->orderNr, searchJustificationItem->originalSentenceNr(), ( isReplacingPrimarySpecification ? replacingSpecificationItem : primarySpecificationItem ), ( isReplacingAnotherPrimarySpecification ? replacingSpecificationItem : anotherPrimarySpecificationItem ), ( isReplacingSecondarySpecification ? replacingSpecificationItem : secondarySpecificationItem ), ( isReplacingAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem ), searchJustificationItem->attachedJustificationItem() ) ).result != RESULT_OK )
							return addError( functionNameString, NULL, "I failed to add a justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
							{
							if( justificationResult.foundJustificationItem == searchJustificationItem )
								return addError( functionNameString, NULL, "No justification item is created, and the found justification item is the same as the search item itself" );

							createdJustificationItem = justificationResult.foundJustificationItem;
							}
						}

					if( replaceJustification( isExclusiveGeneralization, searchJustificationItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : NULL ) ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to replace a justification item" );

					searchJustificationItem = ( hasCorrectedAssumption ? searchJustificationItem->nextJustificationItem() : firstActiveJustificationItem() );

					if( hasCorrectedAssumptionByKnowledge &&
					searchJustificationItem != NULL &&
					searchJustificationItem->hasReplacedPrimarySpecification() )
						searchJustificationItem = firstActiveJustificationItem();
					}
				}
			else
				searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	ResultType writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelatedJustificationSpecifications";

		while( searchJustificationItem != NULL )
			{
			if( !searchJustificationItem->hasJustificationBeenWritten &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL )
				{
				if( !secondarySpecificationItem->isExclusiveSpecification() &&
				secondarySpecificationItem->specificationCollectionNr() == specificationCollectionNr )
					{
					if( ( generalizationWordItem = secondarySpecificationItem->generalizationWordItem() ) == NULL )
						return startError( functionNameString, NULL, "The generalization word of the given secondary specification item is undefined" );

					if( generalizationWordItem->writeJustificationSpecification( false, secondarySpecificationItem ) != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to write a secondary justification specification" );
					}
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem()
		{
		return correctedAssumptionByOppositeQuestionJustificationItem_;
		}

	JustificationItem *negativeAssumptionOrConclusionJustificationItem( SpecificationItem *anotherPrimarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( anotherPrimarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isNegativeAssumptionOrConclusion() &&
				searchJustificationItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *obsoleteJustificationItem()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		WordItem *generalizationWordItem = myWordItem();

		while( searchJustificationItem != NULL )
			{
			if( !generalizationWordItem->isJustificationInUse( searchJustificationItem ) )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *olderComplexJustificationItem( bool hasSecondarySpecificationWithoutRelationContext, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&

			( searchJustificationItem->isOlderItem() ||

			( !hasSecondarySpecificationWithoutRelationContext &&
			searchJustificationItem->hasPrimaryUserSpecification() ) ) &&

			( primarySpecificationItem == NULL ||
			searchJustificationItem->primarySpecificationItem() == primarySpecificationItem ||

			// If primary specifications have different relation context
			( searchJustificationItem->primarySpecificationItem() != NULL &&
			searchJustificationItem->primarySpecificationItem()->generalizationWordItem() == primarySpecificationItem->generalizationWordItem() &&
			searchJustificationItem->primarySpecificationItem()->isRelatedSpecification( primarySpecificationItem->isExclusiveSpecification(), primarySpecificationItem->isNegative(), primarySpecificationItem->isPossessive(), primarySpecificationItem->specificationWordItem() ) ) ) &&

			( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL )
				{
				if( ( !hasSecondarySpecificationWithoutRelationContext ||
				!secondarySpecificationItem->hasRelationContext() ) &&

				( secondarySpecificationCollectionNr == NO_COLLECTION_NR ||

				( !secondarySpecificationItem->isConcludedAssumption() &&
				secondarySpecificationItem->specificationCollectionNr() == secondarySpecificationCollectionNr ) ) )
					return searchJustificationItem;
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *possessiveReversibleAssumptionJustificationItem( WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;

		if( generalizationWordItem != NULL &&
		secondarySpecificationWordItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isPossessiveReversibleAssumption() )
					{
					secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem();

					if( secondarySpecificationItem != NULL &&
					secondarySpecificationItem->generalizationWordItem() == generalizationWordItem &&
					secondarySpecificationItem->specificationWordItem() == secondarySpecificationWordItem )
						return searchJustificationItem;
					}

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *primarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( ( !isSelectingOlderItemOnly ||
				searchJustificationItem->isOlderItem() ) &&

				searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem->primarySpecificationItem() == primarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *secondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( ( !isSelectingOlderItemOnly ||
				searchJustificationItem->isOlderItem() ) &&

				searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( secondarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isSpecificationSubstitutionPartOfAssumption() &&
				searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	SpecificationItem *olderSuggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isSuggestiveQuestionAssumption() &&
			( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL )
				{
				if( secondarySpecificationItem->isOlderItem() )
					return secondarySpecificationItem;
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
