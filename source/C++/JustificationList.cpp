/*	Class:			JustificationList
 *	Parent class:	List
 *	Purpose:		Storing justification items of self-generated knowledge
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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
			if( searchJustificationItem->justificationOrderNr > highestOrderNr &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr )
				highestOrderNr = searchJustificationItem->justificationOrderNr;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return highestOrderNr;
		}

	unsigned short justificationOrderNr( bool hasPrimarySpecificationRelationContext, bool isExclusivePrimarySpecification, bool isPossessivePrimarySpecification, unsigned short justificationTypeNr, unsigned short primarySpecificationWordTypeNr, unsigned int primarySpecificationCollectionNr, SpecificationItem *secondarySpecificationItem, WordItem *primarySpecificationWordItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->isPossessivePrimarySpecification() == isPossessivePrimarySpecification &&

			// Primary specification
			( ( ( ( hasPrimarySpecificationRelationContext &&
			justificationTypeNr != JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION ) ||

			// Exclusive definition specification
			( isExclusivePrimarySpecification &&
			searchJustificationItem->isExclusivePrimarySpecification() ) ) &&

			( ( searchJustificationItem->primarySpecificationWordTypeNr() == primarySpecificationWordTypeNr &&
			searchJustificationItem->primarySpecificationCollectionNr() == primarySpecificationCollectionNr ) ||

			// Typical for Spanish
			// Spanish test file: "Complejo (2)"
			( searchJustificationItem->isPrimarySpecificationWordSpanishAmbiguous() &&
			searchJustificationItem->primarySpecificationWordItem() == primarySpecificationWordItem ) ) ) ||

			// Typical for Spanish
			searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem ) )
				return searchJustificationItem->justificationOrderNr;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NO_ORDER_NR;
		}

	unsigned short justificationOrderNr( bool hasAdditionalDefinitionSpecification, bool hasFeminineOrMasculineProperNounEnding, bool isPossessivePrimarySpecification, bool isPossessiveSecondarySpecification, bool isPrimarySpecificationWordSpanishAmbiguous, unsigned short justificationTypeNr, unsigned int primarySpecificationCollectionNr )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->hasFeminineOrMasculineProperNounEnding() == hasFeminineOrMasculineProperNounEnding &&
			searchJustificationItem->hasAdditionalDefinitionSpecification() == hasAdditionalDefinitionSpecification &&
			searchJustificationItem->isPossessivePrimarySpecification() == isPossessivePrimarySpecification &&
			searchJustificationItem->isPossessiveSecondarySpecification() == isPossessiveSecondarySpecification &&

			( searchJustificationItem->primarySpecificationCollectionNr() == primarySpecificationCollectionNr ||
			isPrimarySpecificationWordSpanishAmbiguous ||
			searchJustificationItem->isPrimarySpecificationWordSpanishAmbiguous() ) )
				return searchJustificationItem->justificationOrderNr;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NO_ORDER_NR;
		}

	unsigned short negativeJustificationOrderNr( bool isExclusivePrimarySpecification, bool isPossessivePrimarySpecification, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isNegativeAssumptionOrConclusion() &&
			searchJustificationItem->isPossessivePrimarySpecification() == isPossessivePrimarySpecification &&

			( searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem ||

			( isExclusivePrimarySpecification &&
			searchJustificationItem->isExclusivePrimarySpecification() ) ) )
				return searchJustificationItem->justificationOrderNr;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NO_ORDER_NR;
		}

	unsigned short reversibleJustificationOrderNr( bool isPossessivePrimarySpecification )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isReversibleAssumptionOrConclusion() &&
			searchJustificationItem->isPossessivePrimarySpecification() == isPossessivePrimarySpecification )
				return searchJustificationItem->justificationOrderNr;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NO_ORDER_NR;
		}

	unsigned short questionJustificationOrderNr( bool hasPrimarySpecificationRelationContext, bool hasAdditionalDefinitionSpecification, bool isExclusivePrimarySpecification, bool isPossessivePrimarySpecification, unsigned int primarySpecificationCollectionNr, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isQuestionJustification() &&
			searchJustificationItem->hasAdditionalDefinitionSpecification() == hasAdditionalDefinitionSpecification &&
			searchJustificationItem->isExclusivePrimarySpecification() == isExclusivePrimarySpecification &&
			searchJustificationItem->isPossessivePrimarySpecification() == isPossessivePrimarySpecification &&
			searchJustificationItem->hasPrimarySpecificationRelationContext() == hasPrimarySpecificationRelationContext &&

			( searchJustificationItem->primarySpecificationCollectionNr() == primarySpecificationCollectionNr ||
			// Typical for Spanish
			searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem ) )
				return searchJustificationItem->justificationOrderNr;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NO_ORDER_NR;
		}

	signed char checkJustificationForUsage( bool isActiveItem, JustificationItem *unusedJustificationItem )
		{
		JustificationItem *searchJustificationItem = firstJustificationItem( isActiveItem );
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkJustificationForUsage";

		if( unusedJustificationItem == NULL )
			return startError( functionNameString, "The given unused justification item is undefined" );

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->attachedJustificationItem() == unusedJustificationItem )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found an attached justification item that is still in use:\n\tJustificationItem: " );
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tattachedJustificationItem: " );
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
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkSpecificationForUsage";

		if( unusedSpecificationItem == NULL )
			return startError( functionNameString, "The given unused specification item is undefined" );

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( searchJustificationItem->primarySpecificationItem() == unusedSpecificationItem )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found a primary specification item that is still in use:\n\tSpecificationItem: " );
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write a primary specification interface warning" );
				}

			if( searchJustificationItem->additionalDefinitionSpecificationItem() == unusedSpecificationItem )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found an additional definition specification item that is still in use:\n\tSpecificationItem: " );
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write additional definition specification interface warning" );
				}

			if( searchJustificationItem->secondarySpecificationItem() == unusedSpecificationItem )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found a secondary primary specification item that is still in use:\n\tSpecificationItem: " );
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write secondary specification interface warning" );
				}

			if( searchJustificationItem->additionalProperNounSpecificationItem() == unusedSpecificationItem )
				{
				// The result of this call is stored in a shared string. Hence, one call at the time.
				strcpy( errorString, "\nI found an additional proper noun primary specification item that is still in use:\n\tSpecificationItem: " );
				strcat( errorString, unusedSpecificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ";\n\tJustificationItem: " );
				strcat( errorString, searchJustificationItem->itemToString( NO_WORD_TYPE_NR ) );
				strcat( errorString, ".\n" );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write additional proper noun specification interface warning" );
				}

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
		return ( isActiveItem ? firstActiveJustificationItem() :
								firstReplacedJustificationItem() );
		}

	JustificationItem *justificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->hasJustification( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem ) )
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

	bool hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem->additionalDefinitionSpecificationItem() == additionalDefinitionSpecificationItem &&
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

	signed char checkForReplacedOrDeletedSpecifications()
		{
		JustificationItem *searchJustificationItem;

		if( !globalVariables()->hasDisplayedIntegrityWarning &&
		( searchJustificationItem = firstActiveJustificationItem() ) != NULL )
			{
			do	;
			// Check for replaced and deleted specifications
			while( searchJustificationItem->checkSpecifications( true ) == RESULT_OK &&
			( searchJustificationItem = searchJustificationItem->nextJustificationItem() ) != NULL &&
			!searchJustificationItem->isReplacedOrDeletedItem() );
			}

		if( !globalVariables()->hasDisplayedIntegrityWarning &&
		( searchJustificationItem = firstReplacedJustificationItem() ) != NULL )
			{
			do	;
			// Only check for deleted specifications
			while( searchJustificationItem->checkSpecifications( false ) == RESULT_OK &&
			( searchJustificationItem = searchJustificationItem->nextJustificationItem() ) != NULL );
			}

		return globalVariables()->result;
		}

	signed char checkForUnreferencedReplacedJustifications()
		{
		JustificationItem *searchJustificationItem = firstReplacedJustificationItem();
		WordItem *_myWordItem = myWordItem();
		char errorString[ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForUnreferencedReplacedJustifications";

		while( searchJustificationItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( !_myWordItem->isJustificationInUse( searchJustificationItem ) )
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
		WordItem *_myWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "cleanupUnreferencedJustifications";

		while( searchJustificationItem != NULL )
			{
			if( _myWordItem->isJustificationInUse( searchJustificationItem ) )
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

	signed char replaceJustification( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem )
		{
		JustificationItem *attachedJustificationItem;
		WordItem *_myWordItem = myWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "replaceJustification";

		if( obsoleteJustificationItem == NULL )
			return startError( functionNameString, "The given obsolete justification item is undefined" );

		if( obsoleteJustificationItem == replacingJustificationItem )
			return startError( functionNameString, "The given obsolete justification item and the given replacing justification item are the same justification item" );

		if( obsoleteJustificationItem->isReplacedItem() )
			return startError( functionNameString, "The given obsolete justification item is already replaced" );

		// Old justification has an attached justification,
		if( ( attachedJustificationItem = obsoleteJustificationItem->attachedJustificationItem() ) != NULL &&
		// that is active,
		attachedJustificationItem->isActiveItem() &&
		replacingJustificationItem != NULL &&
		// but the replacing one no attached justifications
		replacingJustificationItem->attachedJustificationItem() == NULL &&
		// Change attached justification
		replacingJustificationItem->changeAttachedJustification( attachedJustificationItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to change an attached justification item" );

		if( attachedJustificationItem == NULL ||
		// Replacing justification item has the same attached justification item
		replacingJustificationItem != NULL )
			{
			// Replace or delete obsolete justification
			if( replaceOrDeleteJustification( obsoleteJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to replace the obsolete justification item" );

			if( _myWordItem->updateJustificationInSpecifications( isExclusiveGeneralization, obsoleteJustificationItem, replacingJustificationItem ) != RESULT_OK )
				return addError( functionNameString, "I failed to update a justification in specifications of my word" );
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

	signed char updateSpecificationOfJustifications( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem )
		{
		bool hasCurrentlyCorrectedAssumption;
		bool isExclusiveGeneralization = false;
		bool isReplacingAdditionalProperNounSpecification;
		bool isReplacingPrimarySpecification;
		bool isReplacingSecondarySpecification;
		JustificationItem *createdJustificationItem = NULL;
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *additionalDefinitionSpecificationItem;
		SpecificationItem *additionalProperNounSpecificationItem;
		SpecificationItem *selectedPrimarySpecificationItem;
		SpecificationItem *selectedSecondarySpecificationItem;
		WordItem *_myWordItem = myWordItem();
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "updateSpecificationOfJustifications";

		if( obsoleteSpecificationItem == NULL )
			return startError( functionNameString, "The given obsolete specification item is undefined" );

		hasCurrentlyCorrectedAssumption = _myWordItem->hasCurrentlyCorrectedAssumption();

		if( replacingSpecificationItem != NULL &&
		replacingSpecificationItem->isExclusiveNonPossessiveGeneralizationAssignment() )
			isExclusiveGeneralization = true;

		while( searchJustificationItem != NULL )
			{
			if( ( additionalDefinitionSpecificationItem = searchJustificationItem->additionalDefinitionSpecificationItem() ) == obsoleteSpecificationItem )
				return startError( functionNameString, "Replacing an obsolete additional definition specification item is not implemented" );

			isReplacingPrimarySpecification = ( searchJustificationItem->primarySpecificationItem() == obsoleteSpecificationItem );
			isReplacingSecondarySpecification = ( searchJustificationItem->secondarySpecificationItem() == obsoleteSpecificationItem );
			isReplacingAdditionalProperNounSpecification = ( ( additionalProperNounSpecificationItem = searchJustificationItem->additionalProperNounSpecificationItem() ) == obsoleteSpecificationItem );

			if( isReplacingPrimarySpecification ||
			isReplacingSecondarySpecification ||
			isReplacingAdditionalProperNounSpecification )
				{
				if( replacingSpecificationItem != NULL &&
				searchJustificationItem->hasCurrentCreationSentenceNr() )
					{
					if( isReplacingAdditionalProperNounSpecification )
						return startError( functionNameString, "Replacing an obsolete additional proper noun specification item is not implemented" );

					if( searchJustificationItem->replaceSpecification( isReplacingPrimarySpecification, isReplacingSecondarySpecification, replacingSpecificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace a specification item of the search justification item" );
					}
				else
					{
					createdJustificationItem = NULL;

					if( replacingSpecificationItem != NULL &&

					( searchJustificationItem->isQuestionJustification() ||
					// Avoid creating unreferenced justifications

					( ( !searchJustificationItem->isOppositePossessiveSpecificationAssumption() ||
					obsoleteSpecificationItem->isUserSpecification() ||
					replacingSpecificationItem->hasRelationContext() ||
					replacingSpecificationItem->isSelfGeneratedSpecification() ) &&

					// Avoid creating unreferenced justifications
					_myWordItem->isJustificationInUse( searchJustificationItem ) ) ) )
						{
						if( isReplacingAdditionalProperNounSpecification )
							return startError( functionNameString, "Replacing an obsolete additional proper noun specification item is not implemented" );

						selectedPrimarySpecificationItem = ( isReplacingPrimarySpecification ? replacingSpecificationItem :
																								searchJustificationItem->updatedPrimarySpecificationItem() );
						selectedSecondarySpecificationItem = ( isReplacingSecondarySpecification ? replacingSpecificationItem :
																								searchJustificationItem->updatedSecondarySpecificationItem() );

						// Create justification
						if( ( justificationResult = createJustificationItem( searchJustificationItem->hasFeminineOrMasculineProperNounEnding(), searchJustificationItem->justificationTypeNr(), searchJustificationItem->justificationOrderNr, searchJustificationItem->originalSentenceNr(), selectedPrimarySpecificationItem, additionalDefinitionSpecificationItem, selectedSecondarySpecificationItem, additionalProperNounSpecificationItem, searchJustificationItem->attachedJustificationItem() ) ).result != RESULT_OK )
							return addError( functionNameString, "I failed to create a justification item" );

						if( ( createdJustificationItem = justificationResult.createdJustificationItem ) == NULL )
							return startError( functionNameString, "No justification item is created" );
						}

					if( replaceJustification( isExclusiveGeneralization, searchJustificationItem, createdJustificationItem ) != RESULT_OK )
						return addError( functionNameString, "I failed to replace a justification item" );
					}
				}

			searchJustificationItem = ( !hasCurrentlyCorrectedAssumption &&					// First justification
										searchJustificationItem->isReplacedOrDeletedItem() ? firstActiveJustificationItem() :
																							// Next justification
																							searchJustificationItem->nextJustificationItem() );
			}

		return RESULT_OK;
		}

	signed char writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned short justificationOrderNr, unsigned int secondarySpecificationCollectionNr )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();
		SpecificationItem *secondarySpecificationItem;
		WordItem *generalizationWordItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "writeRelatedJustificationSpecifications";

		while( searchJustificationItem != NULL )
			{
			if( !searchJustificationItem->hasJustificationBeenWritten &&
			searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->justificationOrderNr == justificationOrderNr &&
			( secondarySpecificationItem = searchJustificationItem->secondarySpecificationItem() ) != NULL &&
			!secondarySpecificationItem->isExclusiveSpecification() &&
			secondarySpecificationItem->specificationCollectionNr() == secondarySpecificationCollectionNr )
				{
				if( ( generalizationWordItem = secondarySpecificationItem->generalizationWordItem() ) == NULL )
					return startError( functionNameString, "The generalization word of the given secondary specification item is undefined" );

				if( generalizationWordItem->writeJustificationSpecificationInWord( false, secondarySpecificationItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to write a secondary justification specification" );
				}

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return RESULT_OK;
		}

	JustificationItem *foundJustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		if( primarySpecificationItem != NULL &&

		( justificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION ||
		justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||

		( justificationTypeNr == JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION &&
		!primarySpecificationItem->isPossessive() &&

		( ( primarySpecificationItem->hasCompoundSpecificationCollection() &&
		!primarySpecificationItem->hasCurrentCreationSentenceNr() &&
		primarySpecificationItem->isSelfGeneratedSpecification() &&
		// Typical for Spanish
		// Spanish test file: "Complejo (17)"
		globalVariables()->nUserRelationWords > 0 ) ||

		( primarySpecificationItem->isOlderItem() &&
		primarySpecificationItem->isUserSpecification() &&
		!primarySpecificationItem->isAssignment() ) ) ) ||

		( justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION &&
		!primarySpecificationItem->isPossessive() &&
		primarySpecificationItem->isUserSpecification() ) ) )
			return justificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem );

		return NULL;
		}

	JustificationItem *primarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		if( primarySpecificationItem != NULL )
			{
			while( searchJustificationItem != NULL )
				{
				if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
				searchJustificationItem->primarySpecificationItem() == primarySpecificationItem )
					return searchJustificationItem;

				searchJustificationItem = searchJustificationItem->nextJustificationItem();
				}
			}

		return NULL;
		}

	JustificationItem *primaryAndSecondarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem )
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->justificationTypeNr() == justificationTypeNr &&
			searchJustificationItem->primarySpecificationItem() == primarySpecificationItem &&
			searchJustificationItem->secondarySpecificationItem() == secondarySpecificationItem )
				return searchJustificationItem;

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
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

	SpecificationItem *suggestiveQuestionAssumptionSpecificationItem()
		{
		JustificationItem *searchJustificationItem = firstActiveJustificationItem();

		while( searchJustificationItem != NULL )
			{
			if( searchJustificationItem->isSuggestiveQuestionAssumption() )
				return searchJustificationItem->primarySpecificationItem();

			searchJustificationItem = searchJustificationItem->nextJustificationItem();
			}

		return NULL;
		}

	JustificationResultType copyJustificationItem( SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "copyJustificationItem";

		if( originalJustificationItem == NULL )
			return startJustificationResultError( functionNameString, "The given original justification item is undefined" );

		return createJustificationItem( originalJustificationItem->hasFeminineOrMasculineProperNounEnding(), originalJustificationItem->justificationTypeNr(), originalJustificationItem->justificationOrderNr, originalJustificationItem->originalSentenceNr(), newPrimarySpecificationItem, originalJustificationItem->additionalDefinitionSpecificationItem(), newSecondarySpecificationItem, originalJustificationItem->additionalProperNounSpecificationItem(), newAttachedJustificationItem );
		}

	JustificationResultType createJustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, unsigned short _justificationOrderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, JustificationItem *attachedJustificationItem )
		{
		bool isIncrementingOrderNr = false;
		bool isOppositePossessiveSpecificationAssumption = ( justificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION );
		unsigned short foundJustificationOrderNr;
		JustificationResultType justificationResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createJustificationItem";

		if( primarySpecificationItem == NULL )
			return startJustificationResultError( functionNameString, "The given primary specification item is undefined" );

		if( primarySpecificationItem->isReplacedItem() )
			return startJustificationResultError( functionNameString, "The given primary specification item is replaced" );

		if( primarySpecificationItem->isHiddenSpanishSpecification() )
			return startJustificationResultError( functionNameString, "The given primary specification item is hidden" );

		if( isOppositePossessiveSpecificationAssumption )
			{
			if( primarySpecificationItem->isGeneralizationProperNoun() )
				return startJustificationResultError( functionNameString, "The given primary specification item is a proper noun specification" );

			if( primarySpecificationItem->isExclusiveSpecification() )
				return startJustificationResultError( functionNameString, "The given primary specification item is exclusive, while the justification type is non-exclusive" );
			}

		if( additionalDefinitionSpecificationItem != NULL )
			{
			if( additionalDefinitionSpecificationItem->isReplacedItem() )
				return startJustificationResultError( functionNameString, "The given additional definition specification item is replaced" );

			if( !additionalDefinitionSpecificationItem->isGeneralizationNoun() )
				return startJustificationResultError( functionNameString, "The given additional definition specification item is not a definition specification" );
			}

		if( secondarySpecificationItem != NULL )
			{
			if( secondarySpecificationItem->isReplacedItem() )
				return startJustificationResultError( functionNameString, "The given secondary specification item is replaced" );

			if( secondarySpecificationItem->isHiddenSpanishSpecification() )
				return startJustificationResultError( functionNameString, "The given secondary specification item is hidden" );

			if( isOppositePossessiveSpecificationAssumption &&
			secondarySpecificationItem->isExclusiveSpecification() &&
			// Definition
			secondarySpecificationItem->isGeneralizationNoun() )
				return startJustificationResultError( functionNameString, "The given secondary specification item is exclusive, while the justification type is non-exclusive" );
			}

		if( additionalProperNounSpecificationItem != NULL )
			{
			if( additionalProperNounSpecificationItem->isReplacedItem() )
				return startJustificationResultError( functionNameString, "The given additional proper noun specification item is replaced" );

			if( !additionalProperNounSpecificationItem->isGeneralizationProperNoun() )
				return startJustificationResultError( functionNameString, "The given additional definition specification item is not a proper noun specification" );
			}

		if( secondarySpecificationItem != NULL )
			{
			if( isOppositePossessiveSpecificationAssumption )
				{
				if( additionalDefinitionSpecificationItem == NULL )
					{
					if( primarySpecificationItem->specificationWordItem() != secondarySpecificationItem->specificationWordItem() &&

					( !primarySpecificationItem->isSpecificationWordSpanishAmbiguous() ||
					// Typical for Spanish
					secondarySpecificationItem->isSpecificationWordSpanishAmbiguous() ) )
						return startJustificationResultError( functionNameString, "The specification words of the given primary and secondary specification items are different" );
					}
				else
					{
					if( primarySpecificationItem->specificationWordItem() != additionalDefinitionSpecificationItem->generalizationWordItem() )
						return startJustificationResultError( functionNameString, "The specification word of the given primary and generalization word of the given additional definition specification are different" );

					if( additionalDefinitionSpecificationItem->specificationWordItem() != secondarySpecificationItem->specificationWordItem() )
						return startJustificationResultError( functionNameString, "The specification words of the given additional definition and secondary specification items are different" );
					}
				}
			else
				{
				if( justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION &&
				!primarySpecificationItem->isExclusiveSpecification() &&
				primarySpecificationItem->isGeneralizationProperNoun() &&
				!secondarySpecificationItem->isExclusiveSpecification() )
					return startJustificationResultError( functionNameString, "Both the given primary and secondary specification item are non-exclusive, while the justification type is exclusive" );
				}
			}

		if( _justificationOrderNr == NO_ORDER_NR )
			{
			if( ( _justificationOrderNr = highestJustificationOrderNr( justificationTypeNr ) ) > NO_ORDER_NR )
				{
				// Group justification by selecting the order number
				switch( justificationTypeNr )
					{
					case JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION:
					case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION_OR_CONCLUSION:
					case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION_OR_CONCLUSION:
						if( ( foundJustificationOrderNr = justificationOrderNr( primarySpecificationItem->hasRelationContext(), primarySpecificationItem->isExclusiveSpecification(), primarySpecificationItem->isPossessive(), justificationTypeNr, primarySpecificationItem->specificationWordTypeNr(), primarySpecificationItem->specificationCollectionNr(), secondarySpecificationItem, primarySpecificationItem->specificationWordItem() ) ) == NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_SPECIFICATION_ASSUMPTION:
					case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
						if( secondarySpecificationItem == NULL )
							return startJustificationResultError( functionNameString, "This justification type should have a secondary specification" );

						// Typical for Spanish
						if( ( foundJustificationOrderNr = justificationOrderNr( ( additionalDefinitionSpecificationItem != NULL ), hasFeminineOrMasculineProperNounEnding, primarySpecificationItem->isPossessive(), secondarySpecificationItem->isPossessive(), primarySpecificationItem->isSpecificationWordSpanishAmbiguous(), justificationTypeNr, primarySpecificationItem->specificationCollectionNr() ) ) == NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case JUSTIFICATION_TYPE_REVERSIBLE_ASSUMPTION_OR_CONCLUSION:
						if( ( foundJustificationOrderNr = reversibleJustificationOrderNr( primarySpecificationItem->isPossessive() ) ) == NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION_OR_CONCLUSION:
						if( ( foundJustificationOrderNr = negativeJustificationOrderNr( primarySpecificationItem->isExclusiveSpecification(), primarySpecificationItem->isPossessive(), secondarySpecificationItem ) ) == NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;

						break;

					case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_QUESTION:
						if( ( foundJustificationOrderNr = questionJustificationOrderNr( primarySpecificationItem->hasRelationContext(), ( additionalDefinitionSpecificationItem != NULL ), primarySpecificationItem->isExclusiveSpecification(), primarySpecificationItem->isPossessive(), primarySpecificationItem->specificationCollectionNr(), secondarySpecificationItem ) ) == NO_ORDER_NR )
							isIncrementingOrderNr = true;
						else
							_justificationOrderNr = foundJustificationOrderNr;
					}
				}

			if( isIncrementingOrderNr ||
			_justificationOrderNr == NO_ORDER_NR )
				{
				if( _justificationOrderNr >= MAX_ORDER_NR )
					return startJustificationResultError( functionNameString, "Justification order number overflow! I can't accept more justifications" );

				_justificationOrderNr++;
				}
			}

		if( _justificationOrderNr <= NO_ORDER_NR )
			return startJustificationResultError( functionNameString, "The order number is undefined" );

		// Create justification item
		if( ( justificationResult.createdJustificationItem = new JustificationItem( hasFeminineOrMasculineProperNounEnding, justificationTypeNr, _justificationOrderNr, originalSentenceNr, primarySpecificationItem, additionalDefinitionSpecificationItem, secondarySpecificationItem, additionalProperNounSpecificationItem, attachedJustificationItem, globalVariables(), inputOutput(), this, myWordItem() ) ) == NULL )
			return startJustificationResultError( functionNameString, "I failed to create a justification item" );

		// Add justification item to list
		if( addItemToList( QUERY_ACTIVE_CHAR, justificationResult.createdJustificationItem ) != RESULT_OK )
			return addJustificationResultError( functionNameString, "I failed to add a justification item" );

		return justificationResult;
		}
	};

/*************************************************************************
 *	"I will praise you every day;
 *	yes, I will praise you forever." (Psalm 145:2)
 *************************************************************************/
