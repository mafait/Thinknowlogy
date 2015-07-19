/*
 *	Class:			MultipleWordList
 *	Parent class:	List
 *	Purpose:		To store multiple word items
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

#include "MultipleWordItem.cpp"
#include "List.h"

class MultipleWordList : private List
	{
	friend class WordItem;


	// Private functions

	MultipleWordItem *firstActiveMultipleWordItem()
		{
		return (MultipleWordItem *)firstActiveItem();
		}

	bool hasFoundMultipleWordItem( unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		MultipleWordItem *searchItem = firstActiveMultipleWordItem();

		while( searchItem != NULL )
			{
			if( searchItem->wordTypeNr() == wordTypeNr &&
			searchItem->multipleWordItem() == multipleWordItem )
				return true;

			searchItem = searchItem->nextMultipleWordItem();
			}

		return false;
		}

	protected:
	// Constructor / deconstructor

	MultipleWordList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_MULTIPLE_WORD_LIST_SYMBOL, "MultipleWordList", commonVariables, myWordItem );
		}

	~MultipleWordList()
		{
		MultipleWordItem *deleteItem;
		MultipleWordItem *searchItem = firstActiveMultipleWordItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextMultipleWordItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class MultipleWordList has inactive items." );

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class MultipleWordList has archived items." );

		searchItem = (MultipleWordItem *)firstReplacedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextMultipleWordItem();
			delete deleteItem;
			}

		searchItem = (MultipleWordItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextMultipleWordItem();
			delete deleteItem;
			}
		}


	// Protected functions

	unsigned short matchingMultipleSingularNounWordParts( char *sentenceString )
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		MultipleWordItem *searchItem = firstActiveMultipleWordItem();
		WordItem *multipleWordItem;
		char *multipleWordString;

		if( sentenceString != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->isSingularNoun() &&
				searchItem->wordTypeLanguageNr() == currentLanguageNr &&
				( multipleWordItem = searchItem->multipleWordItem() ) != NULL )
					{
					if( ( multipleWordString = multipleWordItem->singularNounString() ) != NULL )
						{
						if( strncmp( sentenceString, multipleWordString, strlen( multipleWordString ) ) == 0 )
							return searchItem->nWordParts();
						}
					}

				searchItem = searchItem->nextMultipleWordItem();
				}
			}

		return 0;
		}

	ResultType checkWordItemForUsage( WordItem *unusedWordItem )
		{
		MultipleWordItem *searchItem = firstActiveMultipleWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->multipleWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The multiple word item is still in use" );

				searchItem = searchItem->nextMultipleWordItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given unused word item is undefined" );

		return RESULT_OK;
		}

	ResultType addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addMultipleWord";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( multipleWordItem != NULL )
				{
				if( commonVariables()->currentItemNr < MAX_ITEM_NR )
					{
					if( !hasFoundMultipleWordItem( wordTypeNr, multipleWordItem ) )
						{
						if( addItemToList( QUERY_ACTIVE_CHAR, new MultipleWordItem( nWordParts, commonVariables()->currentLanguageNr, wordTypeNr, multipleWordItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active multiple word item" );
						}
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given multiple word item is undefined" );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given word type number is undefined or out of bounds" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		MultipleWordItem *searchItem = firstActiveMultipleWordItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeMultipleWordItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store a multiple word item in the database" );
				}

			searchItem = searchItem->nextMultipleWordItem();
			}

		searchItem = firstReplacedMultipleWordItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeMultipleWordItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced multiple word item in the database" );
				}

			searchItem = searchItem->nextMultipleWordItem();
			}

		return RESULT_OK;
		}
*/	};

/*************************************************************************
 *	"The one thing I ask of the Lord -
 *	the thing I seek most -
 *	is to live in the house of the Lord all the days of my life,
 *	delighting in the Lord's perfections
 *	and meditating in his Temple." (Psalm 27:4)
 *************************************************************************/
