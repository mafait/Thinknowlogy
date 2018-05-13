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

#include "SpecificationItem.cpp"

class JustificationList : private List
	{
	friend class WordItem;

	// Private constructed variables

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem_;


	// Private functions

	void clearReplacingInfo( JustificationItem *searchJustificationItem )
		{
		while( searchJustificationItem != NULL )
			{
			searchJustificationItem->clearReplacingInfo();
			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}
		}

	void deleteJustificationList( JustificationItem *searchJustificationItem )
		{
		JustificationItem *deleteJustificationItem;

		while( searchJustificationItem != NULL )
			{
			deleteJustificationItem = searchJustificationItem;
			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			delete deleteJustificationItem;
			}
		}

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

	unsigned short justificationOrderNr( bool hasFeminineOrMasculineProperNounEnding, bool hasPossessivePrimarySpecification, unsigned short justificationTypeNr, unsigned int primarySpecificationCollectionNr )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationCollectionNr > NO_COLLECTION_NR )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->hasFeminineOrMasculineProperNounEnding() == hasFeminineOrMasculineProperNounEnding &&
				searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem->hasPossessivePrimarySpecification() == hasPossessivePrimarySpecification &&
				searchJustificationItem->primarySpecificationCollectionNr() == primarySpecificationCollectionNr )
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
														primarySpecificationItem->hasCompoundSpecificationCollection() );

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

	signed char checkJustificationForUsage( bool isActiveItem, JustificationItem *unusedJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem == NULL )
			return startError( functionNameString, "The given unused justification item is undefined" );

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->attachedJustificationItem() == unusedJustificationItem )
				{
				strcpy( errorString, "\nI found an attached justification item that is still in use:\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tattachedJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, unusedJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	signed char checkSpecificationForUsage( bool isActiveItem, SpecificationItem *unusedSpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkSpecificationForUsage";

		if( unusedSpecificationItem == NULL )
			return startError( functionNameString, "The given unused specification item is undefined" );

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->primarySpecificationItem() == unusedSpecificationItem )
				{
				strcpy( errorString, "\nI found a primary specification item that is still in use:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			if( searchJustificationItem->anotherPrimarySpecificationItem() == unusedSpecificationItem )
				{
				strcpy( errorString, "\nI found an another primary specification item that is still in use:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			if( searchJustificationItem->secondarySpecificationItem() == unusedSpecificationItem )
				{
				strcpy( errorString, "\nI found a secondary primary specification item that is still in use:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			if( searchJustificationItem->anotherSecondarySpecificationItem() == unusedSpecificationItem )
				{
				strcpy( errorString, "\nI found an another secondary primary specification item that is still in use:\n\tSpecificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	signed char replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationItem *attachedJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceJustificationOfCorrectedAssumptionByOppositeQuestion";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, "The given obsolete justification item is undefined" );

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, "The given replacing justification item is undefined" );

		correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

		if( ( secondarySpecificationWordItem = replacingJustificationItem->secondarySpecificationWordItem() ) == NULL )
			return startError( functionNameString, "The given replacing justification item has no secondary specification word item" );

		if( ( foundSpecificationItem = myWordItem()->firstAssignmentOrSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem ) ) != NULL )
			{
			if( foundSpecificationItem->attachJustificationToSpecification( replacingJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to attach the replacing justification item to the found specification" );

			if( foundSpecificationItem->isReplacedItem() )
				foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

			if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL )
				{
				if( ( attachedJustificationItem = firstJustificationItem->attachedJustificationItem() ) != NULL )
					{
					// Remove the obsolete justification item from the replacing justification item
					if( firstJustificationItem->changeAttachedJustification( NULL ) != RESULT_OK )
						return addError( functionNameString, "I failed to remove the attached justification item of the replacing justification item" );

					if( replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace the attached justification item, without an attached justification item" );

					if( ( justificationResult = copyJustification( false, attachedJustificationItem->updatedPrimarySpecificationItem(), attachedJustificationItem->updatedSecondarySpecificationItem(), NULL, attachedJustificationItem ) ).result != RESULT_OK )
						return addError( functionNameString, "I failed to copy the attached justification item" );

					if( justificationResult.createdJustificationItem == NULL )
						return startError( functionNameString, "I couldn't find or create a justification" );

					if( foundSpecificationItem->attachJustificationToSpecification( justificationResult.createdJustificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to attach the created justification item to the found specification" );
					}
				}
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

	JustificationItem *justificationItem( bool hasFeminineOrMasculineProperNounEnding, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasJustification( hasFeminineOrMasculineProperNounEnding, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem ) )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationItem *nextJustificationListItem()
		{
		return (JustificationItem *)nextListItem();
		}


	protected:
	// Constructor

	JustificationList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;

		initializeListVariables( WORD_JUSTIFICATION_LIST_SYMBOL, "JustificationList", globalVariables, inputOutput, myWordItem );
		}

	~JustificationList()
		{
		deleteJustificationList( firstActiveJustificationItem() );
		deleteJustificationList( firstReplacedJustificationItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class JustificationList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class JustificationList has archived items." );

		deleteJustificationList( (JustificationItem *)firstDeletedItem() );
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

	void clearReplacingInfo()
		{
		JustificationItem *searchJustificationItem;

		if( ( searchJustificationItem = firstActiveJustificationItem() ) != NULL )
			clearReplacingInfo( searchJustificationItem );

		if( ( searchJustificationItem = firstReplacedJustificationItem() ) != NULL )
			clearReplacingInfo( searchJustificationItem );
		}

	void initializeJustificationVariables()
		{
		correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;
		}

	bool hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem->anotherPrimarySpecificationItem() == anotherPrimarySpecificationItem &&
			searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem )
				return true;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return false;
		}

	bool hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isQuestionJustification() )
				{
				secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem();

				if( secondarySpecificationItem != NULL &&
				secondarySpecificationItem->hasNonCompoundSpecificationCollection() )
					return true;
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return false;
		}

	signed char attachJustification( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
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
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "attachJustification";

		if( attachJustificationItem == NULL )
			return startError( functionNameString, "The given attached justification item is undefined" );

		if( !attachJustificationItem->isActiveItem() )
			return startError( functionNameString, "The given attached justification item isn't active" );

		if( involvedSpecificationItem == NULL )
			return startError( functionNameString, "The given involved specification item is undefined" );

		if( involvedSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, "The given involved specification item is replaced or deleted" );

		if( ( firstJustificationItem = involvedSpecificationItem->firstJustificationItem() ) == NULL )
			return startError( functionNameString, "The given involved specification item has no justification item" );

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
				return startSystemError( functionNameString, "Justification order number overflow" );

			attachJustificationItem->orderNr = ( highestOrderNr + 1 );
			}

		if( attachJustificationItem->secondarySpecificationQuestion() == NULL &&
		( obsoleteJustificationItem = firstJustificationItem->secondarySpecificationQuestion() ) == NULL )
			{
			if( attachedPrimarySpecificationItem != NULL &&
			!attachedPrimarySpecificationItem->isPossessive() &&
			attachedPrimarySpecificationItem->hasRelationContext() )
				obsoleteJustificationItem = firstJustificationItem->primarySpecificationWithoutRelationContextJustificationItem( attachedPrimarySpecificationItem->specificationWordItem() );
			else
				{
				// Typically for Spanish
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
							return addError( functionNameString, "I failed to replace the obsolete justification item" );

						attachJustificationItem->orderNr = obsoleteJustificationItem->orderNr;
						}
					}
				else
					{
					obsoleteJustificationItem = NULL;

					if( ( highestOrderNr = involvedSpecificationItem->highestAttachedJustificationOrderNr( justificationTypeNr ) ) >= MAX_ORDER_NR )
						return startSystemError( functionNameString, "Justification order number overflow" );

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
				if( lastAttachedJustificationItem->attachJustification( attachJustificationItem, involvedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to attach a justification item" );

				if( ( !hasInvolvedSpecificationRelationContext ||
				!involvedSpecificationItem->isOlderItem() ) &&

				!involvedSpecificationItem->isHiddenSpanishSpecification() )
					{
					previousAssumptionLevel = involvedSpecificationItem->assumptionLevel();

					if( ( shortResult = involvedSpecificationItem->calculateAssumptionLevel() ).result != RESULT_OK )
						return addError( functionNameString, "I failed to calculate the assumption level of the involved specification" );

					if( !hasInvolvedSpecificationRelationContext &&
					involvedSpecificationItem->isOlderItem() &&
					!involvedSpecificationItem->isPartOf() &&
					// Difference between recalculated assumption level and previous assumption level
					shortResult.shortValue != previousAssumptionLevel &&

					( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
					// Recalculated assumption level is higher than 'maybe'
					// Skip writing meaningless adjustment (from 'maybe' to 'maybe')
					shortResult.shortValue < NUMBER_OF_ASSUMPTION_LEVELS ) )
						{
						// Recalculated assumption is conclusion
						if( shortResult.shortValue == NO_ASSUMPTION_LEVEL &&
						involvedSpecificationItem->markAsConcludedAssumption() != RESULT_OK )
							return addError( functionNameString, "I failed to mark the involved specification as concluded assumption" );

						// Write adjusted specification
						if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, false, involvedSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to write an adjusted specification" );
						}
					}
				}
			else
				{
				if( !attachJustificationItem->hasAttachedJustification() &&
				// Attach justification to specification
				involvedSpecificationItem->attachJustificationToSpecification( attachJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to attach the justification item to the given involved specification item" );
				}
			}
		else
			{
			if( !isSkippingUpdateJustification &&
			generalizationWordItem->updateJustificationInSpecifications( false, obsoleteJustificationItem, attachJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to update a question justification item by a conclusion justification item" );
			}

		return RESULT_OK;
		}

	signed char checkForReplacedOrDeletedSpecification()
		{
		JustificationItem *searchJustificationItem;

		if( ( searchJustificationItem = firstActiveJustificationItem() ) != NULL )
			{
			do	;
			// Check for replaced and deleted specifications
			while( searchJustificationItem->checkForReplacedOrDeletedSpecification() == RESULT_OK &&
			( searchJustificationItem = searchJustificationItem->nextJustificationItem() ) != NULL );
			}

		if( ( searchJustificationItem = firstReplacedJustificationItem() ) != NULL )
			{
			do	;
			// Only check for deleted specifications
			while( searchJustificationItem->checkForDeletedSpecifications() == RESULT_OK &&
			( searchJustificationItem = searchJustificationItem->nextJustificationItem() ) != NULL );
			}

		return globalVariables()->result;
		}

	signed char checkForUnreferencedReplacedJustifications()
		{
		JustificationItem *searchJustificationItem = firstReplacedJustificationItem();
		WordItem *generalizationWordItem = myWordItem();
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForUnreferencedReplacedJustifications";

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( !generalizationWordItem->isJustificationInUse( searchJustificationItem ) )
				{
				sprintf( errorString, "\nI found an unreferenced replaced justification item:\n\tJustificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	signed char checkJustificationForUsage( JustificationItem *unusedJustificationItem )
		{
		if( checkJustificationForUsage( true, unusedJustificationItem ) == RESULT_OK )
			{
			if( unusedJustificationItem != NULL &&
			unusedJustificationItem->isReplacedItem() )
				return checkJustificationForUsage( false, unusedJustificationItem );
			}

		return globalVariables()->result;
		}

	signed char checkSpecificationForUsage( SpecificationItem *unusedSpecificationItem )
		{
		if( checkSpecificationForUsage( true, unusedSpecificationItem ) == RESULT_OK )
			return checkSpecificationForUsage( false, unusedSpecificationItem );

		return globalVariables()->result;
		}

	signed char replaceJustification( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem, SpecificationItem *replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		JustificationItem *attachedJustificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordItem *generalizationWordItem = myWordItem();
		RelatedResultType relatedResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceJustification";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, "The given obsolete justification item is undefined" );

		if( involvedSpecificationItem == NULL )
			return startError( functionNameString, "The given involved specification item is undefined" );

		if( obsoleteJustificationItem == replacingJustificationItem )
			return startError( functionNameString, "The given obsolete justification item and the given replacing justification item are the same justification item" );

		hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();
		hasCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCorrectedAssumptionByOppositeQuestion();

		// Corrected assumption
		if( ( hasCorrectedAssumptionByKnowledge ||
		hasCorrectedAssumptionByOppositeQuestion ) &&

		replacingJustificationItem != NULL &&
		replacingJustificationItem->isGeneralizationAssumption() )
			{
			if( !hasCorrectedAssumptionByOppositeQuestion &&
			involvedSpecificationItem->isReplacedItem() )
				involvedSpecificationItem = involvedSpecificationItem->updatedSpecificationItem();

			if( replacingJustificationItem->primarySpecificationWordItem() == involvedSpecificationItem->specificationWordItem() )
				{
				if( generalizationWordItem->writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, involvedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to write an adjusted specification" );

				// Corrected negative assumption
				if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, involvedSpecificationItem->isAssignment(), involvedSpecificationItem->isArchivedAssignment(), involvedSpecificationItem->isExclusiveSpecification(), true, involvedSpecificationItem->isPossessive(), NO_QUESTION_PARAMETER, involvedSpecificationItem->specificationCollectionNr(), involvedSpecificationItem->generalizationContextNr(), involvedSpecificationItem->specificationContextNr(), involvedSpecificationItem->relationContextNr(), involvedSpecificationItem->specificationWordItem() ) ).result != RESULT_OK )
					return addError( functionNameString, "I failed to find a negative related specification" );

				if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL )
					{
					if( generalizationWordItem->writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, false, relatedSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to write a corrected negative assumption" );

					if( hasCorrectedAssumptionByOppositeQuestion &&
					generalizationWordItem->replaceOrDeleteSpecification( false, relatedSpecificationItem, involvedSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace a corrected negative assumption" );
					}
				}

			if( ( attachedJustificationItem = replacingJustificationItem->attachedJustificationItem() ) != NULL &&

			( !hasCorrectedAssumptionByKnowledge ||
			!attachedJustificationItem->isUpdatedPrimarySpecificationWordSpanishAmbiguous() ) )
				{
				if( replaceOrDeleteJustification( replacingJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to delete a corrected justification" );

				replacingJustificationItem = NULL;
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
		!replacingJustificationItem->hasAttachedJustification() &&

		// Change attached justification
		replacingJustificationItem->changeAttachedJustification( attachedJustificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to change an attached justification item" );

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
					return addError( functionNameString, "I failed to replace an active justification item" );

				if( generalizationWordItem->updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to update a justification in specifications of my word" );
				}
			else
				{
				if( hasCorrectedAssumptionByOppositeQuestion &&
				replaceJustificationOfCorrectedAssumptionByOppositeQuestion( obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to replace the justification of a corrected assumption by opposite question" );
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
				if( involvedSpecificationItem->changeFirstJustification( !hasCorrectedAssumptionByKnowledge, attachedJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given obsolete justification item" );
				}
			else
				{
				if( ( hasCorrectedAssumptionByKnowledge ||
				hasCorrectedAssumptionByOppositeQuestion ||
				!generalizationWordItem->isJustificationInUse( attachedJustificationItem ) ) &&

				// Replace or delete justification
				replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to replace the attached justification item" );
				}
			}

		return RESULT_OK;
		}

	signed char replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteJustification";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, "The given obsolete justification item is undefined" );

		if( obsoleteJustificationItem->hasCurrentCreationSentenceNr() )
			{
			if( deleteItem( obsoleteJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to delete an obsolete justification" );
			}
		else
			{
			if( replaceItem( obsoleteJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to replace an obsolete justification" );
			}

		return RESULT_OK;
		}

	signed char replaceOrDeleteUnreferencedJustifications()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceOrDeleteUnreferencedJustifications";

		while( searchJustificationItem != NULL )
			{
			if( generalizationWordItem->isJustificationInUse( searchJustificationItem ) )
				searchJustificationItem = searchJustificationItem->nextJustificationItem();
			else
				{
				if( replaceOrDeleteJustification( searchJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to replace or delete an unreferenced justification item" );

				searchJustificationItem = nextJustificationListItem();
				}
			}

		return RESULT_OK;
		}

	signed char updateSpecificationsInJustifications( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool hasCorrectedAssumption;
		bool hasCorrectedAssumptionByKnowledge;
		bool isReplacingPrimarySpecification;
		bool isReplacingAnotherPrimarySpecification;
		bool isReplacingSecondarySpecification;
		bool isReplacingAnotherSecondarySpecification;
		bool isExclusiveGeneralization = false;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *primarySpecificationItem;
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *secondarySpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem;
		WordItem *generalizationWordItem = myWordItem();
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "updateSpecificationsInJustifications";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		hasCorrectedAssumption = generalizationWordItem->hasCorrectedAssumption();
		hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();

		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isExclusiveNonPossessiveGeneralizationAssignment() &&
		!obsoleteSpecificationItem->isQuestion() )
			isExclusiveGeneralization = true;

		while( searchJustificationItem != NULL )
			{
			primarySpecificationItem = searchJustificationItem->primarySpecificationItem();
			anotherPrimarySpecificationItem = searchJustificationItem->anotherPrimarySpecificationItem();
			secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem();
			anotherSecondarySpecificationItem = searchJustificationItem->anotherSecondarySpecificationItem();

			isReplacingPrimarySpecification = ( primarySpecificationItem == obsoleteSpecificationItem ||

												// Test file: "Laura has a father - Laura is a child - John and Anna"
												( !isMainWord &&
												replacingSpecificationItem != NULL &&
												searchJustificationItem->hasReplacedPrimarySpecification() &&
												searchJustificationItem->isOppositePossessiveConditionalSpecificationAssumption() &&
												obsoleteSpecificationItem->hasRelationContext() &&
												replacingSpecificationItem->isConcludedAssumption() &&
												obsoleteSpecificationItem->generalizationWordItem()->hasCurrentlyConfirmedSpecification() ) );

			isReplacingAnotherPrimarySpecification = ( anotherPrimarySpecificationItem == obsoleteSpecificationItem );

			isReplacingSecondarySpecification = ( secondarySpecificationItem == obsoleteSpecificationItem &&

												// Test file: "Complex (19)"
												( !isMainWord ||
												!searchJustificationItem->isPossessiveReversibleConclusion() ) );

			isReplacingAnotherSecondarySpecification = ( anotherSecondarySpecificationItem == obsoleteSpecificationItem );

			if( isReplacingPrimarySpecification ||
			isReplacingAnotherPrimarySpecification ||
			isReplacingSecondarySpecification ||
			isReplacingAnotherSecondarySpecification )
				{
				if( replacingSpecificationItem != NULL &&
				searchJustificationItem->hasCurrentCreationSentenceNr() )
					{
					if( isReplacingPrimarySpecification &&
					searchJustificationItem->changePrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to change the primary specificationItem item of a justification item" );

					if( isReplacingAnotherPrimarySpecification &&
					searchJustificationItem->changeAnotherPrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to change the another primary specificationItem item of a justification item" );

					if( isReplacingSecondarySpecification &&
					searchJustificationItem->changeSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to change the secondary specificationItem item of a justification item" );

					if( isReplacingAnotherSecondarySpecification &&
					searchJustificationItem->changeAnotherSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to change the another secondary specificationItem item of a justification item" );

					searchJustificationItem = searchJustificationItem->nextJustificationItem();
					}
				else
					{
					createdJustificationItem = NULL;

					if( replacingSpecificationItem != NULL &&

					( searchJustificationItem->isQuestionJustification() ||
					// Avoid creating unreferenced justifications
					generalizationWordItem->isJustificationInUse( searchJustificationItem ) ) )
						{
						if( ( justificationResult = addJustification( searchJustificationItem->hasFeminineOrMasculineProperNounEnding(), ( isReplacingSecondarySpecification && searchJustificationItem->isPossessiveReversibleAssumptionOrConclusion() ), false, searchJustificationItem->updatedJustificationTypeNr( replacingSpecificationItem ), searchJustificationItem->orderNr, searchJustificationItem->originalSentenceNr(), ( isReplacingPrimarySpecification ? replacingSpecificationItem : searchJustificationItem->updatedPrimarySpecificationItem() ), ( isReplacingAnotherPrimarySpecification ? replacingSpecificationItem : anotherPrimarySpecificationItem ), ( isReplacingSecondarySpecification ? replacingSpecificationItem : searchJustificationItem->updatedSecondarySpecificationItem() ), ( isReplacingAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem ), searchJustificationItem->attachedJustificationItem() ) ).result != RESULT_OK )
							return addError( functionNameString, "I failed to add a justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
							{
							if( justificationResult.foundJustificationItem == searchJustificationItem )
								return addError( functionNameString, "No justification item is created, and the found justification item is the same as the search item itself" );

							createdJustificationItem = justificationResult.foundJustificationItem;
							}
						}

					if( replaceJustification( isExclusiveGeneralization, searchJustificationItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCorrectedAssumptionByKnowledge ? replacingSpecificationItem : NULL ) ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace a justification item" );

					searchJustificationItem = ( hasCorrectedAssumption ? searchJustificationItem->nextJustificationItem() : firstActiveJustificationItem() );
					}
				}
			else
				searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	signed char writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeRelatedJustificationSpecifications";

		while( searchJustificationItem != NULL )
			{
			if( !searchJustificationItem->hasJustificationBeenWritten &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL &&
			!secondarySpecificationItem->isExclusiveSpecification() &&
			secondarySpecificationItem->specificationCollectionNr() == specificationCollectionNr )
				{
				if( ( generalizationWordItem = secondarySpecificationItem->generalizationWordItem() ) == NULL )
					return startError( functionNameString, "The generalization word of the given secondary specification item is undefined" );

				if( generalizationWordItem->writeJustificationSpecification( false, secondarySpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to write a secondary justification specification" );
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

	JustificationItem *olderComplexJustificationItem( bool hasSecondarySpecificationWithoutRelationContext, bool isPossessiveSecondarySpecification, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *searchPrimarySpecificationItem;
		SpecificationItem *searchSecondarySpecificationItem;

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			( searchSecondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL )
				{
				searchPrimarySpecificationItem = searchJustificationItem->primarySpecificationItem();

				if( primarySpecificationItem == NULL ||

				( primarySpecificationItem == searchPrimarySpecificationItem &&

				( !hasSecondarySpecificationWithoutRelationContext ||
				searchJustificationItem->isOlderItem() ) ) ||

				// Primary specifications have different relation context
				( searchPrimarySpecificationItem != NULL &&
				searchJustificationItem->isOlderItem() &&
				searchPrimarySpecificationItem->generalizationWordItem() == primarySpecificationItem->generalizationWordItem() &&
				searchPrimarySpecificationItem->isRelatedSpecification( primarySpecificationItem->isExclusiveSpecification(), primarySpecificationItem->isNegative(), primarySpecificationItem->isPossessive(), primarySpecificationItem->specificationWordItem() ) ) )
					{
					if( ( !hasSecondarySpecificationWithoutRelationContext ||
					!searchSecondarySpecificationItem->hasRelationContext() ) &&

					( secondarySpecificationCollectionNr == NO_COLLECTION_NR ||

					( !searchSecondarySpecificationItem->isConcludedAssumption() &&
					searchSecondarySpecificationItem->isPossessive() == isPossessiveSecondarySpecification &&
					searchSecondarySpecificationItem->specificationCollectionNr() == secondarySpecificationCollectionNr ) ) )
						return searchJustificationItem;
					}
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

	JustificationResultType addJustification( bool hasFeminineOrMasculineProperNounEnding, bool isForcingNewJustification, bool isIncrementingOrderNr, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		bool hasPrimarySpecificationRelationContext = false;
		bool isExclusivePrimarySpecification = false;
		bool isPrimarySpecificationWordSpanishAmbiguous = false;
		unsigned short foundJustificationOrderNr;
		unsigned int primarySpecificationCollectionNr;
		JustificationItem *existingJustificationItem;
		JustificationItem *foundJustificationItem = NULL;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addJustification";

		if( primarySpecificationItem == NULL &&
		secondarySpecificationItem == NULL )
			return startJustificationResultError( functionNameString, "Both the given primary specification item and the given secondary specification item are undefined" );

		if( primarySpecificationItem != NULL )
			{
			if( primarySpecificationItem->isReplacedItem() )
				return startJustificationResultError( functionNameString, "The given primary specification item is replaced" );

			if( primarySpecificationItem->isHiddenSpanishSpecification() )
				return startJustificationResultError( functionNameString, "The given primary specification item is hidden" );
			}

		if( anotherPrimarySpecificationItem != NULL )
			{
			if( anotherPrimarySpecificationItem->isReplacedItem() )
				return startJustificationResultError( functionNameString, "The given another primary specification item is replaced" );

			if( anotherPrimarySpecificationItem->isHiddenSpanishSpecification() )
				return startJustificationResultError( functionNameString, "The given another primary specification item is hidden" );
			}

		if( secondarySpecificationItem != NULL )
			{
			if( secondarySpecificationItem->isReplacedItem() )
				return startJustificationResultError( functionNameString, "The given secondary specification item is replaced" );

			if( secondarySpecificationItem->isHiddenSpanishSpecification() )
				return startJustificationResultError( functionNameString, "The given secondary specification item is hidden" );
			}

		if( anotherSecondarySpecificationItem != NULL )
			{
			if( anotherSecondarySpecificationItem->isReplacedItem() )
				return startJustificationResultError( functionNameString, "The given another secondary specification item is replaced" );

			if( anotherSecondarySpecificationItem->isHiddenSpanishSpecification() )
				return startJustificationResultError( functionNameString, "The given another secondary specification item is hidden" );
			}

		// Skipping search, will force the creation of another justification item
		if( !isForcingNewJustification )
			foundJustificationItem = justificationItem( hasFeminineOrMasculineProperNounEnding, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem );

		if( foundJustificationItem == NULL ||

		( attachedJustificationItem == NULL &&
		foundJustificationItem->isQuestionJustification() &&
		foundJustificationItem->justificationTypeNr() != justificationTypeNr ) ||

		( attachedJustificationItem != NULL &&
		foundJustificationItem->justificationTypeNr() == justificationTypeNr &&
		// Avoid looping
		foundJustificationItem->attachedJustificationItem() != attachedJustificationItem ) )
			{
			if( orderNr == NO_ORDER_NR )
				{
				if( foundJustificationItem == NULL &&
				primarySpecificationItem != NULL &&
				secondarySpecificationItem != NULL &&

				( justificationTypeNr != 16 ||
				anotherPrimarySpecificationItem != NULL ) &&

				( foundJustificationItem = olderComplexJustificationItem( false, secondarySpecificationItem->isPossessive(), justificationTypeNr, secondarySpecificationItem->specificationCollectionNr(), primarySpecificationItem ) ) != NULL )
					orderNr = ( foundJustificationItem->secondaryGeneralizationWordItem() == secondarySpecificationItem->generalizationWordItem() ? foundJustificationItem->orderNr : 1 );
				else
					{
					orderNr = ( justificationTypeNr == JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||
								justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
								justificationTypeNr == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ||
								justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION ||
								justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION ? 1 : highestJustificationOrderNr( justificationTypeNr ) );

					if( primarySpecificationItem != NULL )
						{
						hasPrimarySpecificationRelationContext = primarySpecificationItem->hasRelationContext();
						isPrimarySpecificationWordSpanishAmbiguous = primarySpecificationItem->isSpecificationWordSpanishAmbiguous();
						}

					if( orderNr > NO_ORDER_NR &&
					primarySpecificationItem != NULL &&
					secondarySpecificationItem != NULL )
						{
						// Group justification by selecting the order number
						switch( justificationTypeNr )
							{
							case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
								if( anotherPrimarySpecificationItem == NULL &&
								secondarySpecificationItem->isGeneralizationProperNoun() &&
								( existingJustificationItem = primarySpecificationJustificationItem( false, JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION, primarySpecificationItem ) ) != NULL )
									isIncrementingOrderNr = true;

								break;

							case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
								if( primarySpecificationItem->isExclusiveSpecification() )
									isExclusivePrimarySpecification = true;

								// Don't insert a break statement here

							case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
							case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
								// See case above
								if( ( isExclusivePrimarySpecification ||

								// Typically for Spanish
								( ( isPrimarySpecificationWordSpanishAmbiguous &&
								hasPrimarySpecificationRelationContext &&
								primarySpecificationItem->generalizationWordItem() == myWordItem() ) ||

								( hasFeminineOrMasculineProperNounEnding &&
								anotherPrimarySpecificationItem != NULL &&
								anotherPrimarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) ) ) &&

								( primarySpecificationCollectionNr = primarySpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
									{
									if( ( foundJustificationOrderNr = justificationOrderNr( hasFeminineOrMasculineProperNounEnding, primarySpecificationItem->isPossessive(), justificationTypeNr, primarySpecificationCollectionNr ) ) == NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}
								else
									{
									if( justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION )
										{
										if( ( isPrimarySpecificationWordSpanishAmbiguous &&
										hasPrimarySpecificationRelationContext &&
										orderNr == 1 ) ||

										( ( existingJustificationItem = primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, primarySpecificationItem ) ) != NULL &&
										existingJustificationItem->hasAnotherPrimarySpecification() ) )
											isIncrementingOrderNr = true;
										}
									}

								break;

							case JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
								// Typically for Spanish
								if( anotherPrimarySpecificationItem == NULL &&
								anotherSecondarySpecificationItem != NULL &&
								secondarySpecificationItem->hasRelationContext() )
										isIncrementingOrderNr = true;

								break;

							case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
								// Typically for Spanish
								if( anotherSecondarySpecificationItem == NULL )
									{
									if( ( foundJustificationOrderNr = questionJustificationOrderNr( primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem ) ) == NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}
							}
						}

					if( !isExclusivePrimarySpecification &&

					( isIncrementingOrderNr ||
					orderNr == NO_ORDER_NR ||
					justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

					// Typically for Spanish
					( primarySpecificationItem != NULL &&
					justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&

					// Skip self-generated question of corrected assumption by opposite question
					( hasPrimarySpecificationRelationContext ||
					anotherPrimarySpecificationItem == NULL ) ) ) )
						orderNr++;
					}
				}

			if( orderNr <= NO_ORDER_NR )
				return startJustificationResultError( functionNameString, "The order number is undefined" );

			if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, orderNr, originalSentenceNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, attachedJustificationItem, globalVariables(), inputOutput(), this, myWordItem() ) ) == NULL )
				return startJustificationResultError( functionNameString, "I failed to create a justification item" );

			// Add justification item
			if( addItemToList( QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != RESULT_OK )
				return addJustificationResultError( functionNameString, "I failed to add an active justification item" );
			}

		justificationResult.foundJustificationItem = foundJustificationItem;
		return justificationResult;
		}

	JustificationResultType copyJustification( bool isForcingNewJustification, SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyJustification";

		if( originalJustificationItem != NULL )
			return addJustification( originalJustificationItem->hasFeminineOrMasculineProperNounEnding(), isForcingNewJustification, false, originalJustificationItem->updatedJustificationTypeNr( newSecondarySpecificationItem ), originalJustificationItem->orderNr, originalJustificationItem->originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem->anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem->anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		return startJustificationResultError( functionNameString, "The given original justification item is undefined" );
		}
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
