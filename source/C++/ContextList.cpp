/*	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		Storing context items
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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

	ContextItem *firstActiveContextItem()
		{
		return (ContextItem *)firstActiveItem();
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
		WordItem *_myWordItem = myWordItem();

		if( lastContextWordItem == NULL )
			globalVariables()->firstContextWordItem = _myWordItem;
		else
			// Word order is important: Add word at end of context word list
			lastContextWordItem->nextContextWordItem = _myWordItem;

		globalVariables()->lastContextWordItem = _myWordItem;
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

	signed char addContext( unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *foundContextItem;
		WordItem *foundSpecificationWordItem;
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

			if( addItemToList( QUERY_ACTIVE_CHAR, new ContextItem( contextWordTypeNr, ( specificationWordTypeNr == WORD_TYPE_NOUN_PLURAL ? WORD_TYPE_NOUN_SINGULAR : specificationWordTypeNr ), contextNr, specificationWordItem, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
				return addError( functionNameString, "I failed to add a context item" );
			}
		else
			{
			if( ( foundSpecificationWordItem = foundContextItem->specificationWordItem() ) != specificationWordItem )
				{
				if( foundSpecificationWordItem == NULL )
					return startError( functionNameString, "The given context number is assigned to a pronoun context" );

				return startError( functionNameString, "The given context number is already assigned to another specification word: \"", foundSpecificationWordItem->anyWordTypeString(), "\"" );
				}
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

	ContextItem *contextItem( unsigned int contextNr )
		{
		ContextItem *searchContextItem = firstActiveContextItem();

		while( searchContextItem != NULL )
			{
			if( searchContextItem->contextNr() == contextNr )
				return searchContextItem;

			searchContextItem = searchContextItem->nextContextItem();
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
