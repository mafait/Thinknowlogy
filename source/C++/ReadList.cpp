/*	Class:			ReadList
 *	Parent class:	List
 *	Purpose:		To temporarily store read items
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
 *************************************************************************/
/*	Copyright (C) 2009-2016, Menno Mafait. Your suggestions, modifications,
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

#ifndef READLIST
#define READLIST 1
#include "List.h"
#include "ReadItem.cpp"
#include "ReadResultType.cpp"

class ReadList : private List
	{
	friend class AdminCleanup;
	friend class AdminItem;
	friend class AdminReadCreateWords;
	friend class AdminReadSentence;

	// Private constructible variables

	unsigned short lastActivatedWordOrderNr_;

	protected:
	// Constructor / deconstructor

	ReadList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		lastActivatedWordOrderNr_ = NO_ORDER_NR;

		initializeListVariables( ADMIN_READ_LIST_SYMBOL, "ReadList", commonVariables, myWordItem );
		}

	~ReadList()
		{
		ReadItem *deleteItem;
		ReadItem *searchItem = firstActiveReadItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextReadItem();
			delete deleteItem;
			}

		searchItem = firstInactiveReadItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextReadItem();
			delete deleteItem;
			}

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class ReadList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class ReadList has replaced items." );

		searchItem = (ReadItem *)firstDeletedItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextReadItem();
			delete deleteItem;
			}
		}


	// Protected virtual functions

	bool isTemporaryList()
		{
		return true;
		}

	ReferenceResultType findWordReference( WordItem *referenceWordItem )
		{
		ReferenceResultType referenceResult;
		ReadItem *searchItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordReference";

		while( commonVariables()->result == RESULT_OK &&
		searchItem != NULL &&
		!referenceResult.hasFoundWordReference )
			{
			if( ( referenceResult = searchItem->findWordReference( referenceWordItem ) ).result == RESULT_OK )
				searchItem = searchItem->nextReadItem();
			else
				addError( functionNameString, NULL, "I failed to check for a reference word item in an active read item" );
			}

		searchItem = firstInactiveReadItem();

		while( commonVariables()->result == RESULT_OK &&
		searchItem != NULL &&
		!referenceResult.hasFoundWordReference )
			{
			if( ( referenceResult = searchItem->findWordReference( referenceWordItem ) ).result == RESULT_OK )
				searchItem = searchItem->nextReadItem();
			else
				addError( functionNameString, NULL, "I failed to check for a reference word item in an inactive read item" );
			}

		return referenceResult;
		}


	// Protected functions

	void clearLastActivatedWordOrderNr()
		{
		lastActivatedWordOrderNr_ = NO_ORDER_NR;
		}

	bool hasFoundReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, char *readString, WordItem *readWordItem )
		{
		ReadItem *searchItem = firstActiveReadItem();

		while( searchItem != NULL )
			{
			if( searchItem->wordOrderNr() == wordOrderNr &&
			searchItem->wordParameter() == wordParameter &&
			searchItem->wordTypeNr() == wordTypeNr &&

			( ( searchItem->readString == NULL &&
			readString == NULL ) ||

			( searchItem->readString != NULL &&
			readString != NULL &&
			strcmp( searchItem->readString, readString ) == 0 ) ) &&

			searchItem->readWordItem() == readWordItem )
				return true;

			searchItem = searchItem->nextReadItem();
			}

		return false;
		}

	bool isImperativeSentence()
		{
		unsigned short wordOrderNr;
		unsigned short previousWordOrderNr = NO_ORDER_NR;
		unsigned int nWords = 0;
		char *readWordString;
		char *writtenSentenceString;
		ReadItem *startItem = NULL;
		ReadItem *searchItem = firstActiveReadItem();

		while( searchItem != NULL )
			{
			if( ( nWords > 0 ||
			// Trigger
			searchItem->isSpecificationWord() ) &&

			// First appearance of new word
			( wordOrderNr = searchItem->wordOrderNr() ) > previousWordOrderNr )
				{
				nWords++;
				previousWordOrderNr = wordOrderNr;

				if( startItem == NULL )
					startItem = searchItem;
				}

			searchItem = searchItem->nextReadItem();
			}

		// Start creation of imperative sentence
		if( nWords > 2 )
			{
			previousWordOrderNr = NO_ORDER_NR;
			searchItem = startItem;
			strcpy( commonVariables()->writtenSentenceString, EMPTY_STRING );

			writtenSentenceString = commonVariables()->writtenSentenceString;

			while( searchItem != NULL )
				{
				if( ( wordOrderNr = searchItem->wordOrderNr() ) > previousWordOrderNr &&
				!searchItem->isText() &&
				( readWordString = searchItem->readWordTypeString() ) != NULL )
					{
					if( previousWordOrderNr > NO_ORDER_NR &&
					// End of string (colon, question mark, etc)
					searchItem->grammarParameter != GRAMMAR_SENTENCE )
						strcat( writtenSentenceString, SPACE_STRING );

					previousWordOrderNr = wordOrderNr;
					strcat( writtenSentenceString, readWordString );
					}

				searchItem = searchItem->nextReadItem();
				}

			return true;
			}

		return false;
		}

	ReadResultType createReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *readString, WordItem *readWordItem )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadItem";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( commonVariables()->currentItemNr < MAX_ITEM_NR )
				{
				if( ( readResult.createdReadItem = new ReadItem( wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem, commonVariables(), this, myWordItem() ) ) != NULL )
					{
					if( addItemToList( QUERY_ACTIVE_CHAR, readResult.createdReadItem ) != RESULT_OK )
						addError( functionNameString, NULL, "I failed to add an active read item" );
					}
				else
					startError( functionNameString, NULL, "I failed to create a read item" );
				}
			else
				startError( functionNameString, NULL, "The current item number is undefined" );
			}
		else
			startError( functionNameString, NULL, "The given read word type number is undefined or out of bounds" );

		readResult.result = commonVariables()->result;
		return readResult;
		}

	ReadResultType findMoreInterpretations()
		{
		ReadResultType readResult;
		ReadItem *activeReadItem = firstActiveReadItem();
		ReadItem *inactiveReadItem = firstInactiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMoreInterpretations";

		// Get last inactive item
		while( inactiveReadItem != NULL &&
		inactiveReadItem->nextReadItem() != NULL )
			inactiveReadItem = inactiveReadItem->nextReadItem();

		if( inactiveReadItem != NULL )
			{
			readResult.hasFoundMoreInterpretations = true;
			lastActivatedWordOrderNr_ = inactiveReadItem->wordOrderNr();

			if( activateItem( inactiveReadItem ) == RESULT_OK )
				{
				// Clear grammar parameters of all active read items
				while( activeReadItem != NULL )
					{
					activeReadItem->grammarParameter = NO_GRAMMAR_PARAMETER;
					activeReadItem = activeReadItem->nextReadItem();
					}
				}
			else
				addError( functionNameString, NULL, "I failed to active an inactive item" );
			}

		readResult.result = commonVariables()->result;
		return readResult;
		}

	ReadResultType getNumberOfReadWordReferences( unsigned short wordTypeNr, WordItem *readWordItem )
		{
		ReadResultType readResult;
		ReadItem *searchItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "getNumberOfReadWordReferences";

		if( readWordItem != NULL )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isUnusedReadItem &&
				searchItem->wordTypeNr() == wordTypeNr &&
				searchItem->readWordItem() == readWordItem )
					readResult.nReadWordReferences++;

				searchItem = searchItem->nextReadItem();
				}

			searchItem = firstInactiveReadItem();

			while( searchItem != NULL )
				{
				if( !searchItem->isUnusedReadItem &&
				searchItem->readWordItem() == readWordItem &&
				searchItem->wordTypeNr() == wordTypeNr )
					readResult.nReadWordReferences++;

				searchItem = searchItem->nextReadItem();
				}
			}
		else
			startError( functionNameString, NULL, "The given read word is undefined" );

		readResult.result = commonVariables()->result;
		return readResult;
		}

	ReadResultType selectMatchingWordType( unsigned short currentWordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr )
		{
		ReadResultType readResult;
		ReadItem *activeReadItem;
		ReadItem *currentReadItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "selectMatchingWordType";

		// Find current word position
		if( currentWordOrderNr > NO_ORDER_NR )
			{
			while( currentReadItem != NULL &&
			currentReadItem->wordOrderNr() <= currentWordOrderNr )
				currentReadItem = currentReadItem->nextReadItem();
			}

		if( ( activeReadItem = currentReadItem ) != NULL )
			{
			do	{
				if( currentReadItem != NULL )
					{
					if( currentReadItem->wordTypeNr() == wordTypeNr &&
					currentReadItem->wordParameter() == wordParameter )
						{
						readResult.hasFoundMatchingWordType = true;

						// Inactivate read items that don't match
						while( commonVariables()->result == RESULT_OK &&
						activeReadItem != currentReadItem )
							{
							if( inactivateItem( activeReadItem ) == RESULT_OK )
								activeReadItem = nextReadListItem();
							else
								addError( functionNameString, NULL, "I failed to inactive an active item" );
							}
						}
					else
						currentReadItem = currentReadItem->nextReadItem();
					}
				}
			while( commonVariables()->result == RESULT_OK &&
			!readResult.hasFoundMatchingWordType &&
			currentReadItem != NULL &&
			// Only check this word position
			currentReadItem->wordOrderNr() == currentWordOrderNr + 1 &&
			currentReadItem->wordOrderNr() > lastActivatedWordOrderNr_ );
			}

		readResult.result = commonVariables()->result;
		return readResult;
		}

	ResultType activateInactiveReadWords( unsigned short wordOrderNr )
		{
		ReadItem *searchItem = firstInactiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "activateInactiveReadWords";

		while( searchItem != NULL )
			{
			if( !searchItem->isPreposition() &&
			searchItem->wordOrderNr() > wordOrderNr )
				{
				if( activateItem( searchItem ) == RESULT_OK )
					searchItem = nextReadListItem();
				else
					return addError( functionNameString, NULL, "I failed to activate an inactive item" );
				}
			else
				searchItem = searchItem->nextReadItem();
			}

		return RESULT_OK;
		}

	ResultType deleteReadItemsWithNonMatchingMultipleWordPart( unsigned short wordOrderNr, char *sentenceString )
		{
		ReadItem *searchItem = firstActiveReadItem();
		WordItem *readWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteReadItemsWithNonMatchingMultipleWordPart";

		if( sentenceString != NULL )
			{
			while( searchItem != NULL )
				{
				if( searchItem->wordOrderNr() == wordOrderNr &&
				( readWordItem = searchItem->readWordItem() ) != NULL )
					{
					if( readWordItem->isMultipleWord() &&
					// No matching multiple word parts
					readWordItem->matchingMultipleSingularNounWordParts( sentenceString ) == 0 )
						{
						if( deleteItem( searchItem ) == RESULT_OK )
							searchItem = nextReadListItem();
						else
							return addError( functionNameString, NULL, "I failed to delete an active read item" );
						}
					else
						searchItem = searchItem->nextReadItem();
					}
				else
					searchItem = searchItem->nextReadItem();
				}
			}
		else
			return startError( functionNameString, NULL, "The given sentence string is undefined" );

		return RESULT_OK;
		}

	ResultType setGrammarParameter( bool isValid, unsigned short startWordOrderNr, unsigned short endWordOrderNr, GrammarItem *definitionGrammarItem )
		{
		bool hasFound = false;
		bool isMarked = true;
		unsigned short wordOrderNr;
		size_t definitionGrammarStringLength;
		char *definitionGrammarString;
		ReadItem *searchItem = firstActiveReadItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "setGrammarParameter";

		if( endWordOrderNr > NO_ORDER_NR )
			{
			if( startWordOrderNr < endWordOrderNr )
				{
				if( definitionGrammarItem != NULL )
					{
					if( isValid )
						{
						while( isMarked &&
						searchItem != NULL &&
						( wordOrderNr = searchItem->wordOrderNr() ) <= endWordOrderNr )
							{
							if( !searchItem->isMarkedBySetGrammarParameter &&
							wordOrderNr > startWordOrderNr &&
							wordOrderNr <= endWordOrderNr )
								isMarked = false;

							searchItem = searchItem->nextReadItem();
							}

						searchItem = firstActiveReadItem();
						}

					while( searchItem != NULL &&
					( wordOrderNr = searchItem->wordOrderNr() ) <= endWordOrderNr )
						{
						if( wordOrderNr > startWordOrderNr &&
						wordOrderNr <= endWordOrderNr )
							{
							hasFound = true;

							if( isValid )
								{
								if( isMarked ||
								definitionGrammarItem->grammarParameter() > searchItem->grammarParameter )
									{
									searchItem->isMarkedBySetGrammarParameter = false;
									searchItem->grammarParameter = definitionGrammarItem->grammarParameter();
									searchItem->definitionGrammarItem = definitionGrammarItem;

									if( searchItem->readString == NULL &&
									( definitionGrammarString = definitionGrammarItem->grammarString() ) != NULL )
										{
										if( searchItem->readString != NULL )
											strcpy( searchItem->readString, definitionGrammarString );
										else
											{
											if( ( definitionGrammarStringLength = strlen( definitionGrammarString ) ) < MAX_WORD_LENGTH )
												{
												if( searchItem->readString != NULL )
													delete searchItem->readString;

												if( ( searchItem->readString = new char[definitionGrammarStringLength + 1] ) != NULL )
													strcpy( searchItem->readString, definitionGrammarString );
												else
													return startError( functionNameString, NULL, "I failed to create a grammar string" );
												}
											else
												return startError( functionNameString, NULL, "The grammar definition string is too long" );
											}
										}
									}
								}
							else
								searchItem->isMarkedBySetGrammarParameter = true;
							}

						searchItem = searchItem->nextReadItem();
						}

					if( !hasFound )
						return startError( functionNameString, NULL, "I couldn't find any item between the given word order numbers" );
					}
				else
					return startError( functionNameString, NULL, "The given grammar definition word item is undefined" );
				}
			else
				return startError( functionNameString, NULL, "The given start word order number is equal or higher than the given end word order number" );
			}
		else
			return startError( functionNameString, NULL, "The given end word order number is undefined" );

		return RESULT_OK;
		}

	ReadItem *firstActiveReadItem()
		{
		return (ReadItem *)firstActiveItem();
		}

	ReadItem *firstInactiveReadItem()
		{
		return (ReadItem *)firstInactiveItem();
		}

	ReadItem *nextReadListItem()
		{
		return (ReadItem *)nextListItem();
		}
	};
#endif

/*************************************************************************
 *	"Everything he does reveals his glory and majesty.
 *	His righteousness never fails." (Psalm 111:3)
 *************************************************************************/
