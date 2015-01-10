/*
 *	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		To store context items
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

#include "ContextItem.cpp"
#include "List.h"

class ContextList : private List
	{
	friend class WordItem;

	// Private deconstructor functions

	void deleteContextList( ContextItem *searchItem )
		{
		ContextItem *deleteItem;

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextContextItem();
			delete deleteItem;
			}
		}


	protected:
	// Constructor / deconstructor

	ContextList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		initializeListVariables( WORD_CONTEXT_LIST_SYMBOL, "ContextList", commonVariables, myWordItem );
		}

	~ContextList()
		{
		deleteContextList( firstActiveContextItem() );
		deleteContextList( (ContextItem *)firstReplacedItem() );
		deleteContextList( (ContextItem *)firstDeletedItem() );
		}


	// Protected functions

	void clearGeneralizationContextWriteLevel( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr )
					myWordItem()->clearGeneralizationWriteLevel( currentWriteLevel );

				searchItem = searchItem->nextContextItem();
				}
			}
		}

	void clearSpecificationContextWriteLevel( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr )
					myWordItem()->clearSpecificationWriteLevel( currentWriteLevel );

				searchItem = searchItem->nextContextItem();
				}
			}
		}

	void clearRelationContextWriteLevel( unsigned short currentWriteLevel, unsigned int contextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr )
					myWordItem()->clearRelationWriteLevel( currentWriteLevel );

				searchItem = searchItem->nextContextItem();
				}
			}
		}

	bool hasContext( unsigned int contextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	bool hasContext( unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr &&
				searchItem->specificationWordItem() == specificationWordItem )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	bool hasContextCurrentlyBeenUpdated( unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->hasCurrentCreationSentenceNr() &&
				searchItem->contextNr() == contextNr &&
				searchItem->specificationWordItem() == specificationWordItem )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	bool isContextSubset( unsigned int subsetContextNr, unsigned int fullSetContextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( fullSetContextNr > NO_CONTEXT_NR &&
		subsetContextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == subsetContextNr &&
				hasContext( fullSetContextNr, searchItem->specificationWordItem() ) )
					return true;

				searchItem = searchItem->nextContextItem();
				}
			}

		return false;
		}

	unsigned int contextNr( WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the specification word item will be undefined

		while( searchItem != NULL )
			{
			if( searchItem->specificationWordItem() == specificationWordItem )
				return searchItem->contextNr();

			searchItem = searchItem->nextContextItem();
			}

		return NO_CONTEXT_NR;
		}

	unsigned int contextNr( unsigned int nContextWords, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the specification word item will be undefined

		if( nContextWords > 0 )
			{
			while( searchItem != NULL )
				{
				if( searchItem->specificationWordItem() == specificationWordItem &&
				myWordItem()->nContextWordsInAllWords( searchItem->contextNr(), specificationWordItem ) == nContextWords )
					return searchItem->contextNr();

				searchItem = searchItem->nextContextItem();
				}
			}

		return NO_CONTEXT_NR;
		}

	unsigned int contextSentenceNr( unsigned int contextNr, WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the specification word item will be undefined

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr &&
				searchItem->specificationWordItem() == specificationWordItem )
					return searchItem->originalSentenceNr();

				searchItem = searchItem->nextContextItem();
				}
			}

		return NO_SENTENCE_NR;
		}

	unsigned int highestContextNr()
		{
		unsigned int highestContextNr = NO_CONTEXT_NR;
		ContextItem *searchItem = firstActiveContextItem();

		while( searchItem != NULL )
			{
			if( searchItem->contextNr() > highestContextNr )
				highestContextNr = searchItem->contextNr();

			searchItem = searchItem->nextContextItem();
			}

		return highestContextNr;
		}

	ResultType addContext( bool isQuestion, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addContext";

		if( contextNr > NO_CONTEXT_NR )
			{
			if( !hasContext( contextNr, specificationWordItem ) )
				{
				if( contextWordTypeNr > WORD_TYPE_UNDEFINED &&
				contextWordTypeNr < NUMBER_OF_WORD_TYPES )
					{
					if( specificationWordItem == NULL ||

					( specificationWordTypeNr > WORD_TYPE_UNDEFINED &&
					specificationWordTypeNr < NUMBER_OF_WORD_TYPES ) )
						{
						if( commonVariables()->currentItemNr < MAX_ITEM_NR )
							{
							if( addItemToList( QUERY_ACTIVE_CHAR, new ContextItem( isQuestion, contextWordTypeNr, specificationWordTypeNr, contextNr, specificationWordItem, commonVariables(), this, myWordItem() ) ) != RESULT_OK )
								return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active context item" );
							}
						else
							return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );
						}
					else
						return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given specification word type is undefined or out of bounds" );
					}
				else
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given context word type is undefined or out of bounds" );
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given context number is undefined" );

		return RESULT_OK;
		}

	ResultType checkWordItemForUsage( WordItem *unusedWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordItemForUsage";

		if( unusedWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->specificationWordItem() == unusedWordItem )
					return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The specification word item is still in use" );

				searchItem = searchItem->nextContextItem();
				}
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given unused word item is undefined" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDataBase()
		{
		// Not fully implemented yet
		ContextItem *searchItem = firstActiveContextItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDataBase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeContextItemInFutureDataBase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store a context item in the database" );
				}

			searchItem = searchItem->nextContextItem();
			}

		searchItem = firstReplacedContextItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeContextItemInFutureDataBase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced context item in the database" );
				}

			searchItem = searchItem->nextContextItem();
			}

		return RESULT_OK;
		}
*/
	ContextItem *firstActiveContextItem()
		{
		return (ContextItem *)firstActiveItem();
		}

	ContextItem *contextItem( unsigned int contextNr )
		{
		ContextItem *searchItem = firstActiveContextItem();

		if( contextNr > NO_CONTEXT_NR )
			{
			while( searchItem != NULL )
				{
				if( searchItem->contextNr() == contextNr )
					return searchItem;

				searchItem = searchItem->nextContextItem();
				}
			}

		return NULL;
		}

	ContextItem *contextItem( WordItem *specificationWordItem )
		{
		ContextItem *searchItem = firstActiveContextItem();

		// In case of a pronoun context, the specification word item will be undefined

		while( searchItem != NULL )
			{
			if( searchItem->specificationWordItem() == specificationWordItem )
				return searchItem;

			searchItem = searchItem->nextContextItem();
			}

		return NULL;
		}
	};

/*************************************************************************
 *
 *	"O Lord my God, you have performed many wonders for us.
 *	Your plans for us are too numerous to list.
 *	You have no equal.
 *	I have tried to recite all your wonderful deeds,
 *	I would never come to the end of them." (Psalm 40:5)
 *
 *************************************************************************/
