/*	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		To store context items
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

#include "ContextItem.cpp"
#include "GeneralizationItem.cpp"
#include "InputOutput.cpp"
#include "List.h"

class ContextList : private List
	{
	friend class WordItem;

	// Private constructed variables

	bool isMarkedAsContextWord_;


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

	bool hasContextInSpecificationsOfGeneralizationWords( unsigned int contextNr )
		{
		GeneralizationItem *currentGeneralizationItem;
		WordItem *currentGeneralizationWordItem;

		if( contextNr > NO_CONTEXT_NR &&
		( currentGeneralizationItem = myWordItem()->firstGeneralizationItem() ) != NULL )
			{
			do	{
				currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem();

				if( currentGeneralizationWordItem != NULL &&
				currentGeneralizationWordItem->hasContextInSpecificationsInWord( contextNr ) )
					return true;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem->nextGeneralizationItem() ) != NULL );
			}

		return false;
		}

	protected:
	// Constructor

	ContextList( CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		isMarkedAsContextWord_ = false;

		initializeListVariables( WORD_CONTEXT_LIST_SYMBOL, "ContextList", commonVariables, inputOutput, myWordItem );
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
		WordItem *lastContextWordItem = commonVariables()->lastContextWordItem;

		if( lastContextWordItem == NULL )
			commonVariables()->firstContextWordItem = myWordItem();
		else
			// Word order is important: Add word at end of context word list
			lastContextWordItem->nextContextWordItem = myWordItem();

		commonVariables()->lastContextWordItem = myWordItem();
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

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
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

	bool hasContextCurrentlyBeenUpdated( unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->hasCurrentCreationSentenceNr() &&
				searchContextItem->contextNr() == contextNr &&
				searchContextItem->specificationWordItem() == specificationWordItem )
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

	unsigned int contextNr( bool isCompoundCollectionSpanishAmbiguous, WordItem *specificationWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchContextItem != NULL )
			{
			if( searchContextItem->specificationWordItem() == specificationWordItem &&
			searchContextItem->isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous )
				return searchContextItem->contextNr();

			searchContextItem = searchContextItem->nextContextItem();
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

	signed char addContext( bool isCompoundCollectionSpanishAmbiguous, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *foundContextItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addContext";

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

			if( addItemToList( QUERY_ACTIVE_CHAR, new ContextItem( isCompoundCollectionSpanishAmbiguous, contextWordTypeNr, ( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? WORD_TYPE_NOUN_SINGULAR : specificationWordTypeNr ), contextNr, specificationWordItem, commonVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
				return addError( functionNameString, "I failed to add an active context item" );
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

	signed char checkForUnusedContext()
		{
		ContextItem *searchContextItem = firstActiveContextItem();
		char errorString[MAX_ERROR_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForUnusedContext";

		while( searchContextItem != NULL &&
		!commonVariables()->hasDisplayedIntegrityWarning )
			{
			if( !hasContextInSpecificationsOfGeneralizationWords( searchContextItem->contextNr() ) )
				{
				sprintf( errorString, "\nI found unused context number: %u;\n\tContextItem: %s.\n", searchContextItem->contextNr(), searchContextItem->itemToString( NO_WORD_TYPE_NR ) );

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
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

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

	ContextItem *contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, WordItem *specificationWordItem )
		{
		ContextItem *searchContextItem = firstActiveContextItem();
		WordItem *anyWordItem = myWordItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( nContextWords > 0 )
			{
			while( searchContextItem != NULL )
				{
				if( searchContextItem->specificationWordItem() == specificationWordItem &&

				( ( isCompoundCollectionSpanishAmbiguous &&
				searchContextItem->isCompoundCollectionSpanishAmbiguous() ) ||

				( !isCompoundCollectionSpanishAmbiguous &&
				!searchContextItem->isCompoundCollectionSpanishAmbiguous() &&
				anyWordItem->nContextWordsInContextWords( searchContextItem->contextNr(), specificationWordItem ) == nContextWords ) ) )
					return searchContextItem;

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
