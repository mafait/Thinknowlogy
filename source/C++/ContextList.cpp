/*	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		To store context items
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

#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "InputOutput.cpp"
#include "List.h"

class ContextList : private List
	{
	friend class WordItem;

	// Private constructed variables

	bool isMarkedAsContextWord_ = false;


	// Private functions

	void deleteContextList( ContextItem *searchContextItem )
		{
		ContextItem *deleteContextItem;

		while( searchContextItem != NULL )
			{
			deleteContextItem = searchContextItem;
			searchContextItem = searchContextItem->nextContextItem();
			delete deleteContextItem;
			}
		}

	bool hasRelationContextInSpecificationsOfGeneralizationWords( unsigned int relationContextNr )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( relationContextNr > NO_CONTEXT_NR &&
		( currentGeneralizationItem = myWordItem()->firstGeneralizationItem() ) != NULL )
			{
			// Do for all generalization words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) != NULL &&
				currentGeneralizationWordItem->hasRelationContextInSpecificationsInWord( relationContextNr ) )
					return true;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
			}

		return false;
		}

	protected:
	// Constructor

	ContextList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( WORD_CONTEXT_LIST_SYMBOL, "ContextList", globalVariables, inputOutput, myWordItem );
		}

	~ContextList()
		{
		deleteContextList( firstActiveContextItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class ContextList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class ContextList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class ContextList has replaced items." );

		deleteContextList( (ContextItem *)firstDeletedItem() );
		}


	// Protected functions

	void addToContextWordQuickAccessList()
		{
		WordItem *lastContextWordItem = globalVariables()->lastContextWordItem;

		if( lastContextWordItem == NULL )
			globalVariables()->firstContextWordItem = myWordItem();
		else
			// Word order is important: Add word at end of context word list
			lastContextWordItem->nextContextWordItem = myWordItem();

		globalVariables()->lastContextWordItem = myWordItem();
		}

	bool hasContext( unsigned int contextNr )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->contextNr() == contextNr )
					return true;

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return false;
		}

	bool hasContext( unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->contextNr() == contextNr &&
				searchContextItem->specificationWordItem() == specificationWordItem )
					return true;

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return false;
		}

	bool hasContext( unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR &&
		specificationWordItem != NULL )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->contextNr() == contextNr &&
				searchContextItem->spanishAmbiguousCollectionNr() == spanishAmbiguousCollectionNr &&
				searchContextItem->specificationWordItem() == specificationWordItem )
					return true;

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return false;
		}

	bool hasContextCurrentlyBeenUpdated( unsigned int contextNr )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->hasCurrentCreationSentenceNr() &&
				searchContextItem->contextNr() == contextNr )
					return true;

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return false;
		}

	bool isContextSubset( unsigned int subsetContextNr, unsigned int fullSetContextNr )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		if( fullSetContextNr > NO_CONTEXT_NR &&
		subsetContextNr > NO_CONTEXT_NR )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->contextNr() == subsetContextNr &&
				hasContext( fullSetContextNr, searchContextItem->specificationWordItem() ) )
					return true;

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return false;
		}

	unsigned int contextNr( WordItem *specificationWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchContextItem != NULL )
			{
			if( searchContextItem->specificationWordItem() == specificationWordItem )
				return searchContextItem->contextNr();

			searchContextItem = searchContextItem->nextContextItem();
			}

		return NO_CONTEXT_NR;
		}

	unsigned int contextNr( unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		if( specificationWordItem != NULL )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->spanishAmbiguousCollectionNr() == spanishAmbiguousCollectionNr &&
				searchContextItem->specificationWordItem() == specificationWordItem )
					return searchContextItem->contextNr();

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return NO_CONTEXT_NR;
		}

	unsigned int highestContextNr()
		{
		unsigned int highestContextNr = NO_CONTEXT_NR;
		ContextItem *searchContextItem = firstActiveContextItem();

		while( searchContextItem != NULL )
			{
			if( searchContextItem->contextNr() > highestContextNr )
				highestContextNr = searchContextItem->contextNr();

			searchContextItem = searchContextItem->nextContextItem();
			}

		return highestContextNr;
		}

	signed char addContext( unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		ContextItem *foundContextItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addContext";

		if( contextNr <= NO_CONTEXT_NR )
			return startError( functionNameString, "The given context number is undefined" );

		if( ( foundContextItem = contextItem( contextNr ) ) == NULL )
			{
			if( contextWordTypeNr <= NO_WORD_TYPE_NR ||
			contextWordTypeNr >= NUMBER_OF_WORD_TYPES )
				return startError( functionNameString, "The given context word type is undefined or out of bounds: ", contextWordTypeNr );

			if( specificationWordItem != NULL &&

			( specificationWordTypeNr <= NO_WORD_TYPE_NR ||
			specificationWordTypeNr >= NUMBER_OF_WORD_TYPES ) )
				return startError( functionNameString, "The given specification word type is undefined or out of bounds: ", specificationWordTypeNr );

			if( !isMarkedAsContextWord_ )
				{
				isMarkedAsContextWord_ = true;
				addToContextWordQuickAccessList();
				}

			if( addItemToList( QUERY_ACTIVE_CHAR, new ContextItem( contextWordTypeNr, ( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? WORD_TYPE_NOUN_SINGULAR : specificationWordTypeNr ), contextNr, spanishAmbiguousCollectionNr, specificationWordItem, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
				return addError( functionNameString, "I failed to add a context item" );
			}
		else
			{
			if( foundContextItem->specificationWordItem() != specificationWordItem )
				{
				if( foundContextItem->specificationWordItem() == NULL )
					return startError( functionNameString, "The given context number is assigned to a pronoun context" );

				return startError( functionNameString, "The given context number is already assigned to another specification word: \"", foundContextItem->specificationWordItem()->anyWordTypeString(), "\"" );
				}
			}

		return RESULT_OK;
		}

	signed char checkForUnusedRelationContext()
		{
		ContextItem *searchContextItem = firstActiveContextItem();
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkForUnusedRelationContext";

		while( searchContextItem != NULL &&
		!globalVariables()->hasDisplayedIntegrityWarning )
			{
			if( !hasRelationContextInSpecificationsOfGeneralizationWords( searchContextItem->contextNr() ) )
				{
				sprintf( errorString, "\nI found unused relation context number: %u;\n\tContextItem: %s.\n", searchContextItem->contextNr(), searchContextItem->itemToString( NO_WORD_TYPE_NR ) );

				if( inputOutput()->writeDiacriticalText( INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, errorString ) != RESULT_OK )
					return startError( functionNameString, "I failed to write an interface warning" );
				}

			searchContextItem = searchContextItem->nextContextItem();
			}

		return RESULT_OK;
		}

	signed char checkWordItemForUsage( WordItem *unusedWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, "The given unused word item is undefined" );

		while( searchContextItem != NULL )
			{
			if( searchContextItem->specificationWordItem() == unusedWordItem )
				return startError( functionNameString, "The specification word item is still in use" );

			searchContextItem = searchContextItem->nextContextItem();
			}

		return RESULT_OK;
		}

	ContextItem *firstActiveContextItem()
		{
		return (ContextItem *)firstActiveItem();
		}

	ContextItem *contextItem( unsigned int contextNr )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->contextNr() == contextNr )
					return searchContextItem;

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return NULL;
		}

	ContextItem *contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem )
		{
		unsigned int searchSpanishAmbiguousCollectionNr;
		ContextItem *searchContextItem = firstActiveContextItem();
		WordItem *anyWordItem = myWordItem();

		if( nContextWords > 0 &&
		specificationWordItem != NULL )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->specificationWordItem() == specificationWordItem )
					{
					searchSpanishAmbiguousCollectionNr = searchContextItem->spanishAmbiguousCollectionNr();

					// No compound collection Spanish ambiguous
					if( ( !isCompoundCollectionSpanishAmbiguous &&

					( searchSpanishAmbiguousCollectionNr == NO_COLLECTION_NR ||
					// Typical for Spanish
					searchSpanishAmbiguousCollectionNr == spanishAmbiguousCollectionNr ) &&

					anyWordItem->nContextWords( searchContextItem->contextNr(), specificationWordItem ) == nContextWords ) ||

					// Typical for Spanish
					// Compound collection Spanish ambiguous
					( isCompoundCollectionSpanishAmbiguous &&
					searchSpanishAmbiguousCollectionNr == spanishAmbiguousCollectionNr ) )
						return searchContextItem;
					}

				searchContextItem = searchContextItem->nextContextItem();
				}
			}

		return NULL;
		}
	};

/*************************************************************************
 *	"O Lord my God, you have performed many wonders for us.
 *	Your plans for us are too numerous to list.
 *	You have no equal.
 *	I have tried to recite all your wonderful deeds,
 *	I would never come to the end of them." (Psalm 40:5)
 *************************************************************************/
