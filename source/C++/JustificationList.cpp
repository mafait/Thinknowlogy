/*	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items of self-generated knowledge
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

	signed char checkForUnreferencedJustification( bool isActiveItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		WordItem *generalizationWordItem = myWordItem();
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForUnreferencedJustification";

		while( searchJustificationItem != NULL &&
		!commonVariables()->hasDisplayedIntegrityWarning )
			{
			if( !generalizationWordItem->isJustificationInUse( searchJustificationItem ) )
				{
				sprintf( errorString, "\nI found an unreferenced justification item:\n\tJustificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	signed char checkJustificationForUsage( bool isActiveItem, JustificationItem *unusedJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem == NULL )
			return startError( functionNameString, "The given unused justification item is undefined" );

		while( searchJustificationItem != NULL &&
		!commonVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->attachedJustificationItem() == unusedJustificationItem )
				{
				sprintf( errorString, "\nI found an attached justification item that is still in use:\n\tJustificationItem: %s;\n\tattachedJustificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ), unusedJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			if( searchJustificationItem->replacingJustificationItem == unusedJustificationItem )
				{
				sprintf( errorString, "\nI found a replacing justification item that is still in use:\n\tJustificationItem: %s;\n\tattachedJustificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ), unusedJustificationItem->itemToString( NO_WORD_TYPE_NR ) );

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
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSpecificationForUsage";

		if( unusedSpecificationItem == NULL )
			return startError( functionNameString, "The given unused specification item is undefined" );

		while( searchJustificationItem != NULL &&
		!commonVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->primarySpecificationItem() == unusedSpecificationItem )
				{
				sprintf( errorString, "\nI found a primary specification item that is still in use:\n\tJustificationItem: %s;\n\tprimarySpecificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ), unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			if( searchJustificationItem->anotherPrimarySpecificationItem() == unusedSpecificationItem )
				{
				sprintf( errorString, "\nI found an another primary specification item that is still in use:\n\tJustificationItem: %s;\n\tanotherPrimarySpecificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ), unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			if( searchJustificationItem->secondarySpecificationItem() == unusedSpecificationItem )
				{
				sprintf( errorString, "\nI found a secondary specification item that is still in use:\n\tJustificationItem: %s;\n\tsecondarySpecificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ), unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			if( searchJustificationItem->anotherSecondarySpecificationItem() == unusedSpecificationItem )
				{
				sprintf( errorString, "\nI found an another secondary specification item that is still in use:\n\tJustificationItem: %s;\n\tanotherSecondarySpecificationItem: %s.\n", searchJustificationItem->itemToString( NO_WORD_TYPE_NR ), unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	signed char replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationResultType justificationResult;
		JustificationItem *attachedJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustificationOfCorrectedAssumptionByOppositeQuestion";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, "The given obsolete justification item is undefined" );

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, "The given replacing justification item is undefined" );

		correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

		if( ( secondarySpecificationWordItem = replacingJustificationItem->secondarySpecificationWordItem() ) == NULL )
			return startError( functionNameString, "The given replacing justification item has no secondary specification word item" );

		if( ( foundSpecificationItem = myWordItem()->firstAssignmentOrSpecificationItem( false, false, false, false, NO_QUESTION_PARAMETER, secondarySpecificationWordItem ) ) != NULL )
			{
			if( foundSpecificationItem->attachJustificationToSpecification( replacingJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to attach the replacing justification item to the found specification" );

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

					if( ( justificationResult = copyJustification( false, attachedJustificationItem->primarySpecificationItem(), attachedJustificationItem->secondarySpecificationItem(), NULL, attachedJustificationItem ) ).result != RESULT_OK )
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

	signed char updateReplacedJustifications( JustificationItem *updateJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstReplacedJustificationItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "updateReplacedJustifications";

		if( updateJustificationItem == NULL )
			return startError( functionNameString, "The given update justification item is undefined" );

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, "The given replacing justification item is undefined" );

		if( replacingJustificationItem->isReplacedItem() )
			return startError( functionNameString, "The replaced justification item of the given update justification item is a replaced item" );

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

	JustificationItem *justificationItem( bool hasFeminineOrMasculineProperNameEnding, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem )
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

	JustificationItem *nextJustificationListItem()
		{
		return (JustificationItem *)nextListItem();
		}


	protected:
	// Constructor

	JustificationList( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;

		initializeListVariables( WORD_JUSTIFICATION_LIST_SYMBOL, "JustificationList", commonVariables, inputOutput, myWordItem );
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

	bool hasJustification( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->primarySpecificationItem() == primarySpecificationItem &&
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
				secondarySpecificationItem->hasSpecificationNonCompoundCollection() )
					return true;
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return false;
		}

	signed char attachJustification( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem )
		{
		ShortResultType shortResult;
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
			return startError( functionNameString, "The given attached justification item is undefined" );

		if( involvedSpecificationItem == NULL )
			return startError( functionNameString, "The given involved specification item is undefined" );

		if( !attachJustificationItem->isActiveItem() )
			return startError( functionNameString, "The given attached justification item isn't active" );

		if( involvedSpecificationItem->isReplacedOrDeletedItem() )
			return startError( functionNameString, "The given involved specification item is replaced or deleted" );

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

		if( ( firstJustificationItem = involvedSpecificationItem->firstJustificationItem() ) == NULL )
			return startError( functionNameString, "The given involved specification item has no justification item" );

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

				if( !hasInvolvedSpecificationRelationContext ||
				!involvedSpecificationItem->isOlderItem() )
					{
					previousAssumptionLevel = involvedSpecificationItem->assumptionLevel();

					if( ( shortResult = involvedSpecificationItem->recalculateAssumptionLevel() ).result != RESULT_OK )
						return addError( functionNameString, "I failed to recalculate the assumption level of the involved specification" );

					if( !hasInvolvedSpecificationRelationContext &&
					involvedSpecificationItem->isOlderItem() &&
					// Difference between recalculated assumption level and previous assumption level
					shortResult.shortValue != previousAssumptionLevel &&

					( previousAssumptionLevel < NUMBER_OF_ASSUMPTION_LEVELS ||
					// Recalculated assumption level is higher than 'maybe'
					// Skip writing meaningless adjustment (from 'maybe' to 'maybe')
					shortResult.shortValue < NUMBER_OF_ASSUMPTION_LEVELS ) )
						{
						// Recalculated assumption is conclusion
						if( shortResult.shortValue == NO_ASSUMPTION_LEVEL &&
						involvedSpecificationItem->markAsConcludedAssumption( false ) != RESULT_OK )
							return addError( functionNameString, "I failed to mark the involved specification as concluded assumption" );

						// Write adjusted specification
						if( generalizationWordItem->writeUpdatedSpecification( true, false, false, false, false, involvedSpecificationItem ) != RESULT_OK )
							return addError( functionNameString, "I failed to write an adjusted specification" );
						}
					}
				}
			else
				{
				if( !attachJustificationItem->hasAttachedJustification() &&
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

	signed char checkForUnreferencedJustification()
		{
		if( checkForUnreferencedJustification( true ) == RESULT_OK )
			return checkForUnreferencedJustification( false );

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

		return commonVariables()->result;
		}

	signed char checkJustificationForUsage( JustificationItem *unusedJustificationItem )
		{
		if( checkJustificationForUsage( true, unusedJustificationItem ) == RESULT_OK )
			{
			if( unusedJustificationItem != NULL &&
			unusedJustificationItem->isReplacedItem() )
				return checkJustificationForUsage( false, unusedJustificationItem );
			}

		return commonVariables()->result;
		}

	signed char checkSpecificationForUsage( SpecificationItem *unusedSpecificationItem )
		{
		if( checkSpecificationForUsage( true, unusedSpecificationItem ) == RESULT_OK )
			return checkSpecificationForUsage( false, unusedSpecificationItem );

		return commonVariables()->result;
		}

	signed char replaceJustification( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem, SpecificationItem *replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		RelatedResultType relatedResult;
		bool hasCorrectedAssumptionByKnowledge;
		bool hasCorrectedAssumptionByOppositeQuestion;
		JustificationItem *attachedJustificationItem;
		SpecificationItem *relatedSpecificationItem;
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceJustification";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, "The given obsolete justification item is undefined" );

		if( involvedSpecificationItem == NULL )
			return startError( functionNameString, "The given involved specification item is undefined" );

		if( obsoleteJustificationItem->replacingJustificationItem != NULL )
			return startError( functionNameString, "The given obsolete justification item is already replaced" );

		if( replacingJustificationItem != NULL &&
		replacingJustificationItem->replacingJustificationItem != NULL )
			return startError( functionNameString, "The given replacing justification item has a replacing justification item itself" );

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
			if( replacingJustificationItem->primarySpecificationWordItem() == involvedSpecificationItem->specificationWordItem() )
				{
				if( generalizationWordItem->writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, involvedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to write an adjusted specification" );

				// Corrected negative assumption
				if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, involvedSpecificationItem->isAssignment(), involvedSpecificationItem->isArchivedAssignment(), involvedSpecificationItem->isExclusiveSpecification(), true, involvedSpecificationItem->isPossessive(), NO_QUESTION_PARAMETER, involvedSpecificationItem->specificationCollectionNr(), involvedSpecificationItem->generalizationContextNr(), involvedSpecificationItem->specificationContextNr(), involvedSpecificationItem->relationContextNr(), involvedSpecificationItem->specificationWordItem() ) ).result != RESULT_OK )
					return addError( functionNameString, "I failed to find a negative related specification" );

				if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL )
					{
					if( generalizationWordItem->writeUpdatedSpecification( false, false, hasCorrectedAssumptionByKnowledge, hasCorrectedAssumptionByOppositeQuestion, false, relatedSpecificationItem ) != RESULT_OK )
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
		!replacingJustificationItem->hasAttachedJustification() )
			{
			if( replacingJustificationItem->changeAttachedJustification( attachedJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to change an attached justification item" );
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
					return addError( functionNameString, "I failed to replace an active justification item" );

				if( generalizationWordItem->updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to update a justification in specifications of my word" );

				if( replacingJustificationItem != NULL )
					{
					if( updateReplacedJustifications( obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to update the replacing justification items of the replaced justification items" );

					obsoleteJustificationItem->replacingJustificationItem = replacingJustificationItem;
					}
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
				if( involvedSpecificationItem->changeFirstJustification( attachedJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given obsolete justification item" );
				}
			else
				{
				if( hasCorrectedAssumptionByKnowledge ||
				hasCorrectedAssumptionByOppositeQuestion ||
				!generalizationWordItem->isJustificationInUse( attachedJustificationItem ) )
					{
					if( replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace the attached justification item" );
					}
				}
			}

		return RESULT_OK;
		}

	signed char replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteJustification";

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

	signed char replaceOrDeleteObsoleteJustifications()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "replaceOrDeleteObsoleteJustifications";

		while( searchJustificationItem != NULL )
			{
			if( generalizationWordItem->isJustificationInUse( searchJustificationItem ) )
				searchJustificationItem = searchJustificationItem->nextJustificationItem();
			else
				{
				if( replaceOrDeleteJustification( searchJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to replace an obsolete justification item" );

				searchJustificationItem = nextJustificationListItem();
				}
			}

		return RESULT_OK;
		}

		signed char updateSpecificationsInJustifications( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
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
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		hasCorrectedAssumption = generalizationWordItem->hasCorrectedAssumption();
		hasCorrectedAssumptionByKnowledge = generalizationWordItem->hasCorrectedAssumptionByKnowledge();
		hasObsoleteSpecificationItemRelationContext = obsoleteSpecificationItem->hasRelationContext();

		if( replacingSpecificationItem != NULL )
			{
			hasReplacingSpecificationItemRelationContext = replacingSpecificationItem->hasRelationContext();

			if( !obsoleteSpecificationItem->isQuestion() &&
			replacingSpecificationItem->isExclusiveNonPossessiveGeneralizationAssignment() )
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
						if( ( justificationResult = addJustification( searchJustificationItem->hasFeminineOrMasculineProperNameEnding(), ( isReplacingSpecificationWordSpanishAmbiguous && isReplacingSecondarySpecification && searchJustificationItem->isPossessiveReversibleAssumptionOrConclusion() ), false, searchJustificationItem->justificationTypeNr(), searchJustificationItem->orderNr, searchJustificationItem->originalSentenceNr(), ( isReplacingPrimarySpecification ? replacingSpecificationItem : primarySpecificationItem ), ( isReplacingAnotherPrimarySpecification ? replacingSpecificationItem : anotherPrimarySpecificationItem ), ( isReplacingSecondarySpecification ? replacingSpecificationItem : secondarySpecificationItem ), ( isReplacingAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem ), searchJustificationItem->attachedJustificationItem() ) ).result != RESULT_OK )
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

	signed char writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeRelatedJustificationSpecifications";

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

			// Primary specifications have different relation context
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

	JustificationResultType addJustification( bool hasFeminineOrMasculineProperNameEnding, bool isForcingNewJustification, bool isIncrementingOrderNr, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem )
		{
		JustificationResultType justificationResult;
		bool isExclusivePrimarySpecification = false;
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
			return startJustificationResultError( functionNameString, "Both the given primary specification item and the given secondary specification item are undefined" );

		// Skipping search, will force the creation of another justification item
		if( !isForcingNewJustification )
			foundJustificationItem = justificationItem( hasFeminineOrMasculineProperNameEnding, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem );

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
					// Justification exists without feminine or masculine proper name ending
					justificationItem( false, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem ) != NULL ) ) ) ) ) ) )
						orderNr++;
					}
				}

			if( orderNr <= NO_ORDER_NR )
				return startJustificationResultError( functionNameString, "The order number is undefined" );

			if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNameEnding, justificationTypeNr, orderNr, originalSentenceNr, updatedPrimarySpecificationItem, updatedAnotherPrimarySpecificationItem, updatedSecondarySpecificationItem, updatedAnotherSecondarySpecificationItem, attachedJustificationItem, commonVariables(), inputOutput(), this, myWordItem() ) ) == NULL )
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
		char functionNameString[FUNCTION_NAME_LENGTH] = "copyJustification";

		if( originalJustificationItem != NULL )
			return addJustification( originalJustificationItem->hasFeminineOrMasculineProperNameEnding(), isForcingNewJustification, false, originalJustificationItem->justificationTypeNr(), originalJustificationItem->orderNr, originalJustificationItem->originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem->anotherPrimarySpecificationItem(), newSecondarySpecificationItem, originalJustificationItem->anotherSecondarySpecificationItem(), newAttachedJustificationItem );

		return startJustificationResultError( functionNameString, "The given original justification item is undefined" );
		}
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
