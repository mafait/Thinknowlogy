/*	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		To store justification items of self-generated knowledge
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem_ = NULL;


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

	unsigned short questionJustificationOrderNr( SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem )
		{
		bool isCheckingSpecificationCompoundCollection;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationItem != NULL )
			{
			isCheckingSpecificationCompoundCollection = ( anotherPrimarySpecificationItem == NULL &&
														primarySpecificationItem->hasCompoundSpecificationCollection() );

			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->isQuestionJustification() )
					{
					if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem )
						return searchJustificationItem->orderNr;

					if( isCheckingSpecificationCompoundCollection &&
					!searchJustificationItem->hasAnotherPrimarySpecification() &&
					searchJustificationItem->primarySpecificationCollectionNr() == primarySpecificationItem->specificationCollectionNr() )
						return searchJustificationItem->orderNr;
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

	signed char replaceJustificationOfCorrectedAssumptionByOppositeQuestion( JustificationItem *replacingJustificationItem )
		{
		JustificationItem *attachedJustificationItem;
		JustificationItem *firstJustificationItem;
		SpecificationItem *foundSpecificationItem;
		WordItem *secondarySpecificationWordItem;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceJustificationOfCorrectedAssumptionByOppositeQuestion";

		if( replacingJustificationItem == NULL )
			return startError( functionNameString, "The given replacing justification item is undefined" );

		correctedAssumptionByOppositeQuestionJustificationItem_ = replacingJustificationItem;

		if( ( secondarySpecificationWordItem = replacingJustificationItem->secondarySpecificationWordItem() ) == NULL )
			return startError( functionNameString, "The given replacing justification item has no secondary specification word item" );

		if( ( foundSpecificationItem = myWordItem()->firstAssignmentOrSpecificationItem( false, false, false, false, false, secondarySpecificationWordItem ) ) != NULL )
			{
			if( foundSpecificationItem->attachJustificationToSpecification( replacingJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to attach the replacing justification item to the found specification" );

			foundSpecificationItem = foundSpecificationItem->updatedSpecificationItem();

			if( ( firstJustificationItem = foundSpecificationItem->firstJustificationItem() ) != NULL &&
			( attachedJustificationItem = firstJustificationItem->attachedJustificationItem() ) != NULL )
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
			if( searchJustificationItem->isQuestionJustification() &&
			( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL &&
			secondarySpecificationItem->hasNonCompoundSpecificationCollection() )
				return true;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return false;
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
		if( checkJustificationForUsage( true, unusedJustificationItem ) == RESULT_OK &&
		unusedJustificationItem != NULL &&
		unusedJustificationItem->isReplacedItem() )
			return checkJustificationForUsage( false, unusedJustificationItem );

		return globalVariables()->result;
		}

	signed char checkSpecificationForUsage( SpecificationItem *unusedSpecificationItem )
		{
		if( checkSpecificationForUsage( true, unusedSpecificationItem ) == RESULT_OK )
			return checkSpecificationForUsage( false, unusedSpecificationItem );

		return globalVariables()->result;
		}

	signed char cleanupUnreferencedJustifications()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		WordItem *generalizationWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "cleanupUnreferencedJustifications";

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

	signed char replaceJustification( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem, SpecificationItem *replacingCorrectedAssumptionByKnowledgeSpecificationItem )
		{
		bool hasCurrentlyCorrectedAssumptionByKnowledge;
		bool hasCurrentlyCorrectedAssumptionByOppositeQuestion;
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

		hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge();
		hasCurrentlyCorrectedAssumptionByOppositeQuestion = generalizationWordItem->hasCurrentlyCorrectedAssumptionByOppositeQuestion();

		// Corrected assumption
		if( ( hasCurrentlyCorrectedAssumptionByKnowledge ||
		hasCurrentlyCorrectedAssumptionByOppositeQuestion ) &&

		replacingJustificationItem != NULL &&
		replacingJustificationItem->isGeneralizationAssumption() )
			{
			if( replacingJustificationItem->primarySpecificationWordItem() == involvedSpecificationItem->specificationWordItem() )
				{
				if( generalizationWordItem->writeUpdatedSpecification( false, hasCurrentlyCorrectedAssumptionByKnowledge, hasCurrentlyCorrectedAssumptionByOppositeQuestion, false, false, involvedSpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to write an adjusted specification" );

				// Corrected negative assumption
				if( ( relatedResult = generalizationWordItem->findRelatedSpecification( false, involvedSpecificationItem->isAssignment(), involvedSpecificationItem->isArchivedAssignment(), involvedSpecificationItem->isExclusiveSpecification(), true, involvedSpecificationItem->isPossessive(), NO_QUESTION_PARAMETER, involvedSpecificationItem->specificationCollectionNr(), involvedSpecificationItem->generalizationContextNr(), involvedSpecificationItem->relationContextNr(), involvedSpecificationItem->specificationWordItem() ) ).result != RESULT_OK )
					return addError( functionNameString, "I failed to find a negative related specification" );

				if( ( relatedSpecificationItem = relatedResult.relatedSpecificationItem ) != NULL )
					{
					if( generalizationWordItem->writeUpdatedSpecification( false, hasCurrentlyCorrectedAssumptionByKnowledge, hasCurrentlyCorrectedAssumptionByOppositeQuestion, false, false, relatedSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to write a corrected negative assumption" );

					if( hasCurrentlyCorrectedAssumptionByOppositeQuestion &&
					generalizationWordItem->replaceOrDeleteSpecification( relatedSpecificationItem, involvedSpecificationItem->updatedSpecificationItem() ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace a corrected negative assumption" );
					}
				}

			if( ( attachedJustificationItem = replacingJustificationItem->attachedJustificationItem() ) != NULL &&

			( !hasCurrentlyCorrectedAssumptionByKnowledge ||
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
		// but the replacing one no attached justifications
		!replacingJustificationItem->hasAttachedJustification() &&

		// Change attached justification
		replacingJustificationItem->changeAttachedJustification( attachedJustificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to change an attached justification item" );

		if( attachedJustificationItem == NULL ||
		!attachedJustificationItem->isActiveItem() ||
		// Replacing justification item has the same attached justification item
		replacingJustificationItem != NULL )
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
				if( hasCurrentlyCorrectedAssumptionByOppositeQuestion &&
				replaceJustificationOfCorrectedAssumptionByOppositeQuestion( replacingJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to replace the justification of a corrected assumption by opposite question" );
				}
			}
		else
			{
			if( replacingCorrectedAssumptionByKnowledgeSpecificationItem != NULL )
				involvedSpecificationItem = replacingCorrectedAssumptionByKnowledgeSpecificationItem;

			if( involvedSpecificationItem->hasCurrentCreationSentenceNr() )
				{
				if( involvedSpecificationItem->changeFirstJustification( !hasCurrentlyCorrectedAssumptionByKnowledge, attachedJustificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to change the first justification item of the given corrected specification item to the attached justification item of the given obsolete justification item" );
				}
			else
				{
				// Replace or delete justification
				if( replaceOrDeleteJustification( attachedJustificationItem ) != RESULT_OK )
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

	signed char updateSpecificationsInJustifications( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool hasCurrentlyCorrectedAssumption;
		bool hasCurrentlyCorrectedAssumptionByKnowledge;
		bool isForcingNewJustification;
		bool isReplacingPrimarySpecification;
		bool isReplacingSecondarySpecification;
		bool isReplacingAnotherSecondarySpecification;
		bool isExclusiveGeneralization = false;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *anotherPrimarySpecificationItem;
		SpecificationItem *anotherSecondarySpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		SpecificationItem *selectedAnotherSecondarySpecificationItem;
		WordItem *generalizationWordItem = myWordItem();
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "updateSpecificationsInJustifications";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		hasCurrentlyCorrectedAssumption = generalizationWordItem->hasCurrentlyCorrectedAssumption();
		hasCurrentlyCorrectedAssumptionByKnowledge = generalizationWordItem->hasCurrentlyCorrectedAssumptionByKnowledge();

		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isExclusiveNonPossessiveGeneralizationAssignment() )
			isExclusiveGeneralization = true;

		while( searchJustificationItem != NULL )
			{
			anotherPrimarySpecificationItem = searchJustificationItem->anotherPrimarySpecificationItem();
			anotherSecondarySpecificationItem = searchJustificationItem->anotherSecondarySpecificationItem();

			isReplacingPrimarySpecification = ( searchJustificationItem->primarySpecificationItem() == obsoleteSpecificationItem ||

												// Test file: "Laura has a father - Laura is a child - John and Anna"
												( !isMainWord &&
												replacingSpecificationItem != NULL &&
												searchJustificationItem->hasReplacedPrimarySpecification() &&
												searchJustificationItem->isOppositePossessiveConditionalSpecificationAssumption() &&
												obsoleteSpecificationItem->hasRelationContext() &&
												replacingSpecificationItem->isConcludedAssumption() &&
												obsoleteSpecificationItem->generalizationWordItem()->hasCurrentlyConfirmedSpecification() ) );

			isReplacingSecondarySpecification = ( searchJustificationItem->secondarySpecificationItem() == obsoleteSpecificationItem &&

												// Test file: "Complex (19)"
												( !isMainWord ||
												!searchJustificationItem->isReversibleConclusion() ) );

			isReplacingAnotherSecondarySpecification = ( anotherSecondarySpecificationItem == obsoleteSpecificationItem );

			if( isReplacingPrimarySpecification ||
			isReplacingSecondarySpecification ||
			isReplacingAnotherSecondarySpecification )
				{
				if( replacingSpecificationItem != NULL &&
				searchJustificationItem->hasCurrentCreationSentenceNr() )
					{
					if( isReplacingPrimarySpecification &&
					searchJustificationItem->changePrimarySpecification( replacingSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to change the primary specificationItem item of a justification item" );

					if( isReplacingSecondarySpecification &&
					searchJustificationItem->changeSecondarySpecification( replacingSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to change the secondary specificationItem item of a justification item" );

					if( isReplacingAnotherSecondarySpecification )
						return startError( functionNameString, "I didn't expect the another secondary specificationItem item needed to be replaced" );

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
						isForcingNewJustification = ( isReplacingSecondarySpecification &&
													searchJustificationItem->isReversibleAssumptionOrConclusion() );

						selectedPrimarySpecificationItem = ( isReplacingPrimarySpecification ? replacingSpecificationItem : searchJustificationItem->updatedPrimarySpecificationItem() );
						selectedSecondarySpecificationItem = ( isReplacingSecondarySpecification ? replacingSpecificationItem : searchJustificationItem->updatedSecondarySpecificationItem() );
						selectedAnotherSecondarySpecificationItem = ( isReplacingAnotherSecondarySpecification ? replacingSpecificationItem : anotherSecondarySpecificationItem );

						if( ( justificationResult = addJustification( searchJustificationItem->hasFeminineOrMasculineProperNounEnding(), isForcingNewJustification, false, searchJustificationItem->updatedJustificationTypeNr( replacingSpecificationItem ), searchJustificationItem->orderNr, searchJustificationItem->originalSentenceNr(), selectedPrimarySpecificationItem, anotherPrimarySpecificationItem, selectedSecondarySpecificationItem, selectedAnotherSecondarySpecificationItem, searchJustificationItem->attachedJustificationItem() ) ).result != RESULT_OK )
							return addError( functionNameString, "I failed to add a justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
							{
							if( justificationResult.foundJustificationItem == searchJustificationItem )
								return addError( functionNameString, "No justification item is created, and the found justification item is the same as the search item itself" );

							createdJustificationItem = justificationResult.foundJustificationItem;
							}
						}

					if( replaceJustification( isExclusiveGeneralization, searchJustificationItem, createdJustificationItem, obsoleteSpecificationItem, ( hasCurrentlyCorrectedAssumptionByKnowledge ? replacingSpecificationItem : NULL ) ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace a justification item" );

					searchJustificationItem = ( hasCurrentlyCorrectedAssumption ? searchJustificationItem->nextJustificationItem() : firstActiveJustificationItem() );
					}
				}
			else
				searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	signed char writeRelatedJustificationSpecifications( unsigned short justificationTypeNr )
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
			!secondarySpecificationItem->isExclusiveSpecification() )
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

		if( primarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
				( searchSecondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL )
					{
					searchPrimarySpecificationItem = searchJustificationItem->primarySpecificationItem();

					if( ( primarySpecificationItem == searchPrimarySpecificationItem &&

					( !hasSecondarySpecificationWithoutRelationContext ||
					searchJustificationItem->isOlderItem() ) ) ||

					// Primary specifications have different relation context
					( searchPrimarySpecificationItem != NULL &&
					searchJustificationItem->isOlderItem() &&
					searchPrimarySpecificationItem->generalizationWordItem() == primarySpecificationItem->generalizationWordItem() &&
					searchPrimarySpecificationItem->isRelatedSpecification( primarySpecificationItem->isExclusiveSpecification(), primarySpecificationItem->isPossessive(), primarySpecificationItem->specificationWordItem() ) ) )
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

	SpecificationItem *suggestiveQuestionAssumptionSecondarySpecificationItem()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isSuggestiveQuestionAssumption() )
				return searchJustificationItem->secondarySpecificationItem();

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
		attachedJustificationItem != NULL ||

		( foundJustificationItem->isQuestionJustification() &&
		foundJustificationItem->justificationTypeNr() != justificationTypeNr ) )
			{
			if( orderNr == NO_ORDER_NR )
				{
				if( foundJustificationItem == NULL &&
				primarySpecificationItem != NULL &&
				secondarySpecificationItem != NULL &&

				( anotherPrimarySpecificationItem != NULL ||
				justificationTypeNr != JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION ) &&

				( foundJustificationItem = olderComplexJustificationItem( false, secondarySpecificationItem->isPossessive(), justificationTypeNr, secondarySpecificationItem->specificationCollectionNr(), primarySpecificationItem ) ) != NULL )
					orderNr = ( foundJustificationItem->secondaryGeneralizationWordItem() == secondarySpecificationItem->generalizationWordItem() ? foundJustificationItem->orderNr : 1 );
				else
					{
					orderNr = ( justificationTypeNr == JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||
								justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
								justificationTypeNr == JUSTIFICATION_TYPE_REVERSIBLE_CONCLUSION ||
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
								if( isExclusivePrimarySpecification ||

								// Typical for Spanish
								( ( isPrimarySpecificationWordSpanishAmbiguous &&
								hasPrimarySpecificationRelationContext &&
								primarySpecificationItem->generalizationWordItem() == myWordItem() ) ||

								// Typical for Spanish
								( hasFeminineOrMasculineProperNounEnding &&
								anotherPrimarySpecificationItem != NULL ) ) )
									{
									if( ( foundJustificationOrderNr = justificationOrderNr( hasFeminineOrMasculineProperNounEnding, primarySpecificationItem->isPossessive(), justificationTypeNr, primarySpecificationItem->specificationCollectionNr() ) ) == NO_ORDER_NR )
										isIncrementingOrderNr = true;
									else
										orderNr = foundJustificationOrderNr;
									}
								else
									{
									if( justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
									( existingJustificationItem = primarySpecificationJustificationItem( true, JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION, primarySpecificationItem ) ) != NULL &&
									existingJustificationItem->hasAnotherPrimarySpecification() )
										isIncrementingOrderNr = true;
									}

								break;

							case JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION:
								// Typical for Spanish
								if( anotherPrimarySpecificationItem == NULL &&
								anotherSecondarySpecificationItem != NULL &&
								secondarySpecificationItem->hasRelationContext() )
										isIncrementingOrderNr = true;

								break;

							case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
								// Typical for Spanish
								if( ( foundJustificationOrderNr = questionJustificationOrderNr( primarySpecificationItem, anotherPrimarySpecificationItem ) ) == NO_ORDER_NR )
									isIncrementingOrderNr = true;
								else
									orderNr = foundJustificationOrderNr;
							}
						}

					if( !isExclusivePrimarySpecification &&

					( isIncrementingOrderNr ||
					orderNr == NO_ORDER_NR ||
					justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||

					// Typical for Spanish
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
				return addJustificationResultError( functionNameString, "I failed to add a justification item" );
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
