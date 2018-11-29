/*	Class:			MultipleWordList
 *	Parent class:	List
 *	Purpose:		To store multiple word items
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

#include "MultipleWordItem.cpp"
#include "List.h"

class MultipleWordList : private List
	{
	friend class WordItem;

	// Private functions

	void deleteMultipleWordList( MultipleWordItem *searchMultipleWordItem )
		{
		MultipleWordItem *deleteMultipleWordItem;

		while( searchMultipleWordItem != NULL )
			{
			deleteMultipleWordItem = searchMultipleWordItem;
			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			delete deleteMultipleWordItem;
			}
		}

	bool hasFoundMultipleWordItem( unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();

		while( searchMultipleWordItem != NULL )
			{
			if( searchMultipleWordItem->wordTypeNr() == wordTypeNr &&
			searchMultipleWordItem->multipleWordItem() == multipleWordItem )
				return true;

			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			}

		return false;
		}

	MultipleWordItem *firstActiveMultipleWordItem()
		{
		return (MultipleWordItem *)firstActiveItem();
		}

	protected:
	// Constructor

	MultipleWordList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		initializeListVariables( WORD_MULTIPLE_WORD_LIST_SYMBOL, "MultipleWordList", globalVariables, inputOutput, myWordItem );
		}

	~MultipleWordList()
		{
		deleteMultipleWordList( firstActiveMultipleWordItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class MultipleWordList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class MultipleWordList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class MultipleWordList has replaced items." );

		deleteMultipleWordList( (MultipleWordItem *)firstDeletedItem() );
		}


	// Protected functions

	unsigned short matchingMultipleWordParts( char *sentenceString )
		{
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();
		WordItem *multipleWordItem;
		char *multipleWordString;

		if( sentenceString != NULL )
			{
			while( searchMultipleWordItem != NULL )
				{
				if( ( multipleWordItem = searchMultipleWordItem->multipleWordItem() ) != NULL &&
				( multipleWordString = multipleWordItem->anyWordTypeString() ) != NULL &&
				strncmp( sentenceString, multipleWordString, strlen( multipleWordString ) ) == 0 )
					return searchMultipleWordItem->nWordParts();

				searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
				}
			}

		return 0;
		}

	signed char addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addMultipleWord";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		if( multipleWordItem == NULL )
			return startError( functionNameString, "The given multiple word item is undefined" );

		if( !hasFoundMultipleWordItem( wordTypeNr, multipleWordItem ) &&
		addItemToList( QUERY_ACTIVE_CHAR, new MultipleWordItem( nWordParts, globalVariables()->currentLanguageNr, wordTypeNr, multipleWordItem, globalVariables(), inputOutput(), this, myWordItem() ) ) != RESULT_OK )
			return addError( functionNameString, "I failed to add a multiple word item" );

		return RESULT_OK;
		}

	signed char checkWordItemForUsage( WordItem *unusedWordItem )
		{
		MultipleWordItem *searchMultipleWordItem = firstActiveMultipleWordItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem == NULL )
			return startError( functionNameString, "The given unused word item is undefined" );

		while( searchMultipleWordItem != NULL )
			{
			if( searchMultipleWordItem->multipleWordItem() == unusedWordItem )
				return startError( functionNameString, "The multiple word item is still in use" );

			searchMultipleWordItem = searchMultipleWordItem->nextMultipleWordItem();
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"The one thing I ask of the Lord -
 *	the thing I seek most -
 *	is to live in the house of the Lord all the days of my life,
 *	delighting in the Lord's perfections
 *	and meditating in his Temple." (Psalm 27:4)
 *************************************************************************/
