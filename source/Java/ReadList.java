/*
 *	Class:			ReadList
 *	Parent class:	List
 *	Purpose:		To temporarily store read items
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

class ReadList extends List
	{
	// Private constructible variables

	private short lastActivatedWordOrderNr_;

	// Constructor / deconstructor

	protected ReadList( WordItem myWordItem )
		{
		lastActivatedWordOrderNr_ = Constants.NO_ORDER_NR;

		initializeListVariables( Constants.ADMIN_READ_LIST_SYMBOL, myWordItem );
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return true;
		}

	protected ReferenceResultType findWordReference( WordItem referenceWordItem )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();
		ReadItem searchItem = firstActiveReadItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		searchItem != null &&
		!referenceResult.hasFoundWordReference )
			{
			if( ( referenceResult = searchItem.findWordReference( referenceWordItem ) ).result == Constants.RESULT_OK )
				searchItem = searchItem.nextReadItem();
			else
				addError( 1, null, null, "I failed to check for a reference word item in an active read item" );
			}

		searchItem = firstInactiveReadItem();

		while( CommonVariables.result == Constants.RESULT_OK &&
		searchItem != null &&
		!referenceResult.hasFoundWordReference )
			{
			if( ( referenceResult = searchItem.findWordReference( referenceWordItem ) ).result == Constants.RESULT_OK )
				searchItem = searchItem.nextReadItem();
			else
				addError( 1, null, null, "I failed to check for a reference word item in an inactive read item" );
			}

		return referenceResult;
		}

	protected byte deleteInvalidPartOfMultipleWordReadItems( short wordOrderNr, String sentenceString )
		{
		ReadItem searchItem = firstActiveReadItem();
		WordItem readWordItem;

		if( sentenceString != null )
			{
			while( searchItem != null )
				{
				if( searchItem.wordOrderNr() == wordOrderNr &&
				( readWordItem = searchItem.readWordItem() ) != null )
					{
					if( readWordItem.isMultipleWord() &&
					// No matching multiple word parts
					readWordItem.matchingMultipleSingularNounWordParts( sentenceString ) == 0 )
						{
						if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
							searchItem = nextReadListItem();
						else
							return addError( 1, null, null, "I failed to delete an active read item" );
						}
					else
						searchItem = searchItem.nextReadItem();
					}
				else
					searchItem = searchItem.nextReadItem();
				}
			}
		else
			return startError( 1, null, null, "The given sentence string is undefined" );

		return Constants.RESULT_OK;
		}


	// Protected methods

	protected void clearLastActivatedWordOrderNr()
		{
		lastActivatedWordOrderNr_ = Constants.NO_ORDER_NR;
		}

	protected boolean hasFoundReadItem( short wordOrderNr, short wordParameter, short wordTypeNr, String readString, WordItem readWordItem )
		{
		ReadItem searchItem = firstActiveReadItem();

		while( searchItem != null )
			{
			if( searchItem.wordOrderNr() == wordOrderNr &&
			searchItem.wordParameter() == wordParameter &&
			searchItem.wordTypeNr() == wordTypeNr &&

			( ( searchItem.readString == null &&
			readString == null ) ||

			( searchItem.readString != null &&
			readString != null &&
			searchItem.readString.equals( readString ) ) ) &&

			searchItem.readWordItem() == readWordItem )
				return true;

			searchItem = searchItem.nextReadItem();
			}

		return false;
		}

	protected boolean isImperativeSentence()
		{
		short previousWordOrderNr = Constants.NO_ORDER_NR;
		int nWords = 0;
		String readWordString;
		ReadItem startItem = null;
		ReadItem searchItem = firstActiveReadItem();

		CommonVariables.writeSentenceStringBuffer = new StringBuffer();

		while( searchItem != null )
			{
			if( ( nWords > 0 ||
			// Trigger
			searchItem.isSpecificationWord() ) &&

			// First appearance of new word
			searchItem.wordOrderNr() > previousWordOrderNr )
				{
				nWords++;
				previousWordOrderNr = searchItem.wordOrderNr();

				if( startItem == null )
					startItem = searchItem;
				}

			searchItem = searchItem.nextReadItem();
			}

		// Start creation of imperative sentence
		if( nWords > 2 )
			{
			previousWordOrderNr = Constants.NO_ORDER_NR;
			searchItem = startItem;
			CommonVariables.writeSentenceStringBuffer = new StringBuffer();

			while( searchItem != null )
				{
				if( searchItem.wordOrderNr() > previousWordOrderNr &&
				// Skip text
				searchItem.readWordItem() != null &&
				( readWordString = searchItem.readWordTypeString() ) != null )
					{
					if( previousWordOrderNr > Constants.NO_ORDER_NR &&
					// End of string (colon, question mark, etc)
					searchItem.grammarParameter != Constants.GRAMMAR_SENTENCE )
						CommonVariables.writeSentenceStringBuffer.append( Constants.SPACE_STRING );

					previousWordOrderNr = searchItem.wordOrderNr();
					CommonVariables.writeSentenceStringBuffer.append( readWordString );
					}

				searchItem = searchItem.nextReadItem();
				}
			}

		return ( nWords > 2 );
		}

	protected ReadResultType createReadItem( short wordOrderNr, short wordParameter, short wordTypeNr, int readStringLength, String readString, WordItem readWordItem )
		{
		ReadResultType readResult = new ReadResultType();

		if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		wordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
				{
				if( ( readResult.createdReadItem = new ReadItem( wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem, this, myWordItem() ) ) != null )
					{
					if( addItemToList( Constants.QUERY_ACTIVE_CHAR, readResult.createdReadItem ) != Constants.RESULT_OK )
						addError( 1, null, null, "I failed to add an active read item" );
					}
				else
					startError( 1, null, null, "I failed to create a read item" );
				}
			else
				startError( 1, null, null, "The current item number is undefined" );
			}
		else
			startError( 1, null, null, "The given read word type number is undefined or out of bounds" );

		readResult.result = CommonVariables.result;
		return readResult;
		}

	protected ReadResultType findMoreInterpretations()
		{
		ReadResultType readResult = new ReadResultType();
		ReadItem activeReadItem = firstActiveReadItem();
		ReadItem inactiveReadItem = firstInactiveReadItem();

		// Clear grammar parameters of all active read items
		while( activeReadItem != null )
			{
			activeReadItem.grammarParameter = Constants.NO_GRAMMAR_PARAMETER;
			activeReadItem = activeReadItem.nextReadItem();
			}

		// Get last inactive item
		while( inactiveReadItem != null &&
		inactiveReadItem.nextReadItem() != null )
			inactiveReadItem = inactiveReadItem.nextReadItem();

		if( inactiveReadItem != null )
			{
			readResult.hasFoundMoreInterpretations = true;
			lastActivatedWordOrderNr_ = inactiveReadItem.wordOrderNr();

			if( activateItem( inactiveReadItem ) != Constants.RESULT_OK )
				addError( 1, null, null, "I failed to active an inactive item" );
			}

		readResult.result = CommonVariables.result;
		return readResult;
		}

	protected ReadResultType getNumberOfReadWordReferences( short wordTypeNr, WordItem readWordItem )
		{
		ReadResultType readResult = new ReadResultType();
		ReadItem searchItem = firstActiveReadItem();

		if( readWordItem != null )
			{
			while( searchItem != null )
				{
				if( !searchItem.isUnusedReadItem &&
				searchItem.wordTypeNr() == wordTypeNr &&
				searchItem.readWordItem() == readWordItem )
					readResult.nReadWordReferences++;

				searchItem = searchItem.nextReadItem();
				}

			searchItem = firstInactiveReadItem();

			while( searchItem != null )
				{
				if( !searchItem.isUnusedReadItem &&
				searchItem.readWordItem() == readWordItem &&
				searchItem.wordTypeNr() == wordTypeNr )
					readResult.nReadWordReferences++;

				searchItem = searchItem.nextReadItem();
				}
			}
		else
			startError( 1, null, null, "The given read word is undefined" );

		readResult.result = CommonVariables.result;
		return readResult;
		}

	protected ReadResultType selectMatchingWordType( short currentWordOrderNr, short wordParameter, short wordTypeNr )
		{
		ReadResultType readResult = new ReadResultType();
		ReadItem activeReadItem;
		ReadItem currentReadItem = firstActiveReadItem();

		// Find current word position
		if( currentWordOrderNr > Constants.NO_ORDER_NR )
			{
			while( currentReadItem != null &&
			currentReadItem.wordOrderNr() <= currentWordOrderNr )
				currentReadItem = currentReadItem.nextReadItem();
			}

		if( ( activeReadItem = currentReadItem ) != null )
			{
			do	{
				if( currentReadItem != null )
					{
					if( currentReadItem.wordTypeNr() == wordTypeNr &&
					currentReadItem.wordParameter() == wordParameter )
						{
						readResult.hasFoundMatchingWordType = true;

						// Inactivate read items that don't match
						while( CommonVariables.result == Constants.RESULT_OK &&
						activeReadItem != currentReadItem )
							{
							if( inactivateItem( activeReadItem ) == Constants.RESULT_OK )
								activeReadItem = nextReadListItem();
							else
								addError( 1, null, null, "I failed to inactive an active item" );
							}
						}
					else
						currentReadItem = currentReadItem.nextReadItem();
					}
				}
			while( CommonVariables.result == Constants.RESULT_OK &&
			!readResult.hasFoundMatchingWordType &&
			currentReadItem != null &&
			// Only check this word position
			currentReadItem.wordOrderNr() == currentWordOrderNr + 1 &&
			currentReadItem.wordOrderNr() > lastActivatedWordOrderNr_ );
			}

		readResult.result = CommonVariables.result;
		return readResult;
		}

	protected byte activateInactiveReadWords( short wordOrderNr )
		{
		ReadItem searchItem = firstInactiveReadItem();

		while( searchItem != null )
			{
			if( !searchItem.isPreposition() &&
			searchItem.wordOrderNr() > wordOrderNr )
				{
				if( activateItem( searchItem ) == Constants.RESULT_OK )
					searchItem = nextReadListItem();
				else
					return addError( 1, null, null, "I failed to activate an inactive item" );
				}
			else
				searchItem = searchItem.nextReadItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteActiveReadWords()
		{
		ReadItem searchItem = firstActiveReadItem();

		while( searchItem != null )
			{
			if( deleteItem( false, searchItem ) == Constants.RESULT_OK )
				searchItem = nextReadListItem();
			else
				return addError( 1, null, null, "I failed to delete an active item" );
			}

		return Constants.RESULT_OK;
		}

	protected byte setGrammarParameter( boolean isValid, short startWordOrderNr, short endWordOrderNr, GrammarItem definitionGrammarItem )
		{
		boolean hasFound = false;
		boolean isMarked = true;
		String definitionGrammarString;
		ReadItem searchItem = firstActiveReadItem();

		if( endWordOrderNr > Constants.NO_ORDER_NR )
			{
			if( startWordOrderNr < endWordOrderNr )
				{
				if( definitionGrammarItem != null )
					{
					if( isValid )
						{
						while( isMarked &&
						searchItem != null &&
						searchItem.wordOrderNr() <= endWordOrderNr )
							{
							if( !searchItem.isMarkedBySetGrammarParameter &&
							searchItem.wordOrderNr() > startWordOrderNr &&
							searchItem.wordOrderNr() <= endWordOrderNr )
								isMarked = false;

							searchItem = searchItem.nextReadItem();
							}

						searchItem = firstActiveReadItem();
						}

					while( searchItem != null &&
					searchItem.wordOrderNr() <= endWordOrderNr )
						{
						if( searchItem.wordOrderNr() > startWordOrderNr &&
						searchItem.wordOrderNr() <= endWordOrderNr )
							{
							hasFound = true;

							if( isValid )
								{
								if( isMarked ||
								definitionGrammarItem.grammarParameter() > searchItem.grammarParameter )
									{
									searchItem.isMarkedBySetGrammarParameter = false;
									searchItem.grammarParameter = definitionGrammarItem.grammarParameter();
									searchItem.definitionGrammarItem = definitionGrammarItem;

									if( searchItem.readString == null &&
									( definitionGrammarString = definitionGrammarItem.grammarString() ) != null )
										{
										if( searchItem.readString != null )
											searchItem.readString = definitionGrammarString;
										else
											searchItem.readString = definitionGrammarString;
										}
									}
								}
							else
								searchItem.isMarkedBySetGrammarParameter = true;
							}

						searchItem = searchItem.nextReadItem();
						}

					if( !hasFound )
						return startError( 1, null, null, "I couldn't find any item between the given word order numbers" );
					}
				else
					return startError( 1, null, null, "The given grammar definition word item is undefined" );
				}
			else
				return startError( 1, null, null, "The given start word order number is equal or higher than the given end word order number" );
			}
		else
			return startError( 1, null, null, "The given end word order number is undefined" );

		return Constants.RESULT_OK;
		}

	protected ReadItem firstActiveReadItem()
		{
		return (ReadItem)firstActiveItem();
		}

	protected ReadItem firstInactiveReadItem()
		{
		return (ReadItem)firstInactiveItem();
		}

	protected ReadItem nextReadListItem()
		{
		return (ReadItem)nextListItem();
		}
	};

/*************************************************************************
 *	"Everything he does reveals his glory and majesty.
 *	His righteousness never fails." (Psalm 111:3)
 *************************************************************************/
