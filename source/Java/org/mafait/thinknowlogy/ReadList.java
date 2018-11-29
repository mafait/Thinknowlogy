/*	Class:			ReadList
 *	Parent class:	List
 *	Purpose:		To temporarily store read items
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

package org.mafait.thinknowlogy;

class ReadList extends List
	{
	// Private constructed variables

	private short lastActivatedWordOrderNr_ = Constants.NO_ORDER_NR;

	// Private methods

	private boolean hasFoundReadItem( short wordOrderNr, short wordParameter, short wordTypeNr, WordItem readWordItem )
		{
		ReadItem searchReadItem = firstActiveReadItem();

		while( searchReadItem != null )
			{
			if( searchReadItem.wordOrderNr() == wordOrderNr &&
			searchReadItem.wordParameter() == wordParameter &&
			searchReadItem.wordTypeNr() == wordTypeNr &&
			searchReadItem.readWordItem() == readWordItem )
				return true;

			searchReadItem = searchReadItem.nextReadItem();
			}

		return false;
		}

	// Constructor

	protected ReadList( WordItem myWordItem )
		{
		initializeListVariables( Constants.ADMIN_READ_LIST_SYMBOL, "ReadList", myWordItem );
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return true;
		}


	// Protected methods

	protected void clearLastActivatedWordOrderNr()
		{
		lastActivatedWordOrderNr_ = Constants.NO_ORDER_NR;
		}

	protected byte activateInactiveReadWords( short wordOrderNr )
		{
		ReadItem searchReadItem = firstInactiveReadItem();

		while( searchReadItem != null )
			{
			if( !searchReadItem.isPreposition() &&
			searchReadItem.wordOrderNr() > wordOrderNr )
				{
				if( activateItem( searchReadItem ) != Constants.RESULT_OK )
					return addError( 1, "I failed to activate an inactive item" );

				searchReadItem = nextReadListItem();
				}
			else
				searchReadItem = searchReadItem.nextReadItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte createReadItem( boolean isUncountableGeneralizationNoun, short wordOrderNr, short wordParameter, short wordTypeNr, int readStringLength, String readString, WordItem readWordItem )
		{
		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, "The given read word type number is undefined or out of bounds" );

		if( hasFoundReadItem( wordOrderNr, wordParameter, wordTypeNr, readWordItem ) )
			return startError( 1, "The given read item already exists" );

		if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new ReadItem( isUncountableGeneralizationNoun, wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add a read item" );

		return Constants.RESULT_OK;
		}

	protected byte deleteReadItemsWithNonMatchingMultipleWordPart( short wordOrderNr, String sentenceString )
		{
		ReadItem searchReadItem = firstActiveReadItem();
		WordItem readWordItem;

		if( sentenceString == null )
			return startError( 1, "The given sentence string is undefined" );

		while( searchReadItem != null )
			{
			if( searchReadItem.wordOrderNr() == wordOrderNr &&
			( readWordItem = searchReadItem.readWordItem() ) != null )
				{
				if( readWordItem.isMultipleWord() &&
				// No matching multiple word parts
				readWordItem.matchingMultipleWordParts( sentenceString ) == 0 )
					{
					if( deleteItem( searchReadItem ) != Constants.RESULT_OK )
						return addError( 1, "I failed to delete a read item" );

					searchReadItem = nextReadListItem();
					}
				else
					searchReadItem = searchReadItem.nextReadItem();
				}
			else
				searchReadItem = searchReadItem.nextReadItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte setGrammarParameter( boolean isValid, short startWordOrderNr, short endWordOrderNr, GrammarItem definitionGrammarItem )
		{
		boolean hasFound = false;
		boolean isMarked = true;
		short wordOrderNr;
		String definitionGrammarString;
		ReadItem searchReadItem = firstActiveReadItem();

		if( endWordOrderNr <= Constants.NO_ORDER_NR )
			return startError( 1, "The given end word order number is undefined" );

		if( startWordOrderNr >= endWordOrderNr )
			return startError( 1, "The given start word order number is equal or higher than the given end word order number" );

		if( definitionGrammarItem == null )
			return startError( 1, "The given grammar definition word item is undefined" );

		if( isValid )
			{
			while( isMarked &&
			searchReadItem != null &&
			( wordOrderNr = searchReadItem.wordOrderNr() ) <= endWordOrderNr )
				{
				if( !searchReadItem.isMarkedBySetGrammarParameter &&
				wordOrderNr > startWordOrderNr &&
				wordOrderNr <= endWordOrderNr )
					isMarked = false;

				searchReadItem = searchReadItem.nextReadItem();
				}

			searchReadItem = firstActiveReadItem();
			}

		while( searchReadItem != null &&
		( wordOrderNr = searchReadItem.wordOrderNr() ) <= endWordOrderNr )
			{
			if( wordOrderNr > startWordOrderNr &&
			wordOrderNr <= endWordOrderNr )
				{
				hasFound = true;

				if( isValid )
					{
					if( isMarked ||
					definitionGrammarItem.grammarParameter() > searchReadItem.grammarParameter )
						{
						searchReadItem.isMarkedBySetGrammarParameter = false;
						searchReadItem.grammarParameter = definitionGrammarItem.grammarParameter();
						searchReadItem.definitionGrammarItem = definitionGrammarItem;

						if( searchReadItem.readString == null &&
						( definitionGrammarString = definitionGrammarItem.grammarString() ) != null )
							searchReadItem.readString = definitionGrammarString;
						}
					}
				else
					searchReadItem.isMarkedBySetGrammarParameter = true;
				}

			searchReadItem = searchReadItem.nextReadItem();
			}

		if( !hasFound )
			return startError( 1, "I couldn't find any item between the given word order numbers" );

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

	protected BoolResultType findMoreInterpretations()
		{
		ReadItem activeReadItem = firstActiveReadItem();
		ReadItem inactiveReadItem = firstInactiveReadItem();
		BoolResultType boolResult = new BoolResultType();

		// Get last inactive item
		while( inactiveReadItem != null &&
		inactiveReadItem.nextReadItem() != null )
			inactiveReadItem = inactiveReadItem.nextReadItem();

		if( inactiveReadItem != null )
			{
			// Has found another interpretation
			boolResult.booleanValue = true;
			lastActivatedWordOrderNr_ = inactiveReadItem.wordOrderNr();

			if( activateItem( inactiveReadItem ) != Constants.RESULT_OK )
				return addBoolResultError( 1, "I failed to active an inactive item" );

			// Clear grammar parameters of all active read items
			while( activeReadItem != null )
				{
				activeReadItem.grammarParameter = Constants.NO_GRAMMAR_PARAMETER;
				activeReadItem = activeReadItem.nextReadItem();
				}
			}

		return boolResult;
		}

	protected BoolResultType selectMatchingWordType( short currentWordOrderNr, short wordParameter, short wordTypeNr )
		{
		boolean hasFoundMatchingWordType = false;
		ReadItem activeReadItem;
		ReadItem currentReadItem = firstActiveReadItem();
		BoolResultType boolResult = new BoolResultType();

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
						hasFoundMatchingWordType = true;

						// Inactivate read items that don't match
						while( activeReadItem != currentReadItem )
							{
							if( inactivateItem( activeReadItem ) != Constants.RESULT_OK )
								return addBoolResultError( 1, "I failed to inactive an active item" );

							activeReadItem = nextReadListItem();
							}
						}
					else
						currentReadItem = currentReadItem.nextReadItem();
					}
				}
			while( !hasFoundMatchingWordType &&
			currentReadItem != null &&
			// Only check this word position
			currentReadItem.wordOrderNr() == currentWordOrderNr + 1 &&
			currentReadItem.wordOrderNr() > lastActivatedWordOrderNr_ );
			}

		boolResult.booleanValue = hasFoundMatchingWordType;
		return boolResult;
		}

	protected ShortResultType getNumberOfReadWordReferences( short wordTypeNr, WordItem readWordItem )
		{
		short nReadWordReferences = 0;
		ReadItem searchReadItem = firstActiveReadItem();
		ShortResultType shortResult = new ShortResultType();

		if( readWordItem == null )
			return startShortResultError( 1, "The given read word is undefined" );

		while( searchReadItem != null )
			{
			if( searchReadItem.wordTypeNr() == wordTypeNr &&
			searchReadItem.readWordItem() == readWordItem )
				nReadWordReferences++;

			searchReadItem = searchReadItem.nextReadItem();
			}

		searchReadItem = firstInactiveReadItem();

		while( searchReadItem != null )
			{
			if( searchReadItem.wordTypeNr() == wordTypeNr &&
			searchReadItem.readWordItem() == readWordItem )
				nReadWordReferences++;

			searchReadItem = searchReadItem.nextReadItem();
			}

		shortResult.shortValue = nReadWordReferences;
		return shortResult;
		}
	};

/*************************************************************************
 *	"Everything he does reveals his glory and majesty.
 *	His righteousness never fails." (Psalm 111:3)
 *************************************************************************/
