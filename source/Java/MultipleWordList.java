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

class MultipleWordList extends List
	{
	// Private methods

	private MultipleWordItem firstActiveMultipleWordItem()
		{
		return (MultipleWordItem)firstActiveItem();
		}

	private boolean hasFoundMultipleWordItem( short wordTypeNr, WordItem multipleWordItem )
		{
		MultipleWordItem searchItem = firstActiveMultipleWordItem();

		while( searchItem != null )
			{
			if( searchItem.wordTypeNr() == wordTypeNr &&
			searchItem.multipleWordItem() == multipleWordItem )
				return true;

			searchItem = searchItem.nextMultipleWordItem();
			}

		return false;
		}

	// Constructor / deconstructor

	protected MultipleWordList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_MULTIPLE_WORD_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected short matchingMultipleSingularNounWordParts( String sentenceString )
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		MultipleWordItem searchItem = firstActiveMultipleWordItem();
		WordItem multipleWordItem;
		String multipleWordString;

		if( sentenceString != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isSingularNoun() &&
				searchItem.wordTypeLanguageNr() == currentLanguageNr &&
				( multipleWordItem = searchItem.multipleWordItem() ) != null )
					{
					if( ( multipleWordString = multipleWordItem.singularNounString() ) != null )
						{
						if( sentenceString.startsWith( multipleWordString ) )
							return searchItem.nWordParts();
						}
					}

				searchItem = searchItem.nextMultipleWordItem();
				}
			}

		return 0;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		MultipleWordItem searchItem = firstActiveMultipleWordItem();

		if( unusedWordItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.multipleWordItem() == unusedWordItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The multiple word item is still in use" );

				searchItem = searchItem.nextMultipleWordItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused word item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte addMultipleWord( short nWordParts, short wordTypeNr, WordItem multipleWordItem )
		{
		if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED &&
		wordTypeNr < Constants.NUMBER_OF_WORD_TYPES )
			{
			if( multipleWordItem != null )
				{
				if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
					{
					if( !hasFoundMultipleWordItem( wordTypeNr, multipleWordItem ) )
						{
						if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new MultipleWordItem( nWordParts, CommonVariables.currentLanguageNr, wordTypeNr, multipleWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
							return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active multiple word item" );
						}
					}
				else
					return startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );
				}
			else
				return startError( 1, null, myWordItem().anyWordTypeString(), "The given multiple word item is undefined" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given word type number is undefined or out of bounds" );

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		MultipleWordItem searchItem = firstActiveMultipleWordItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeMultipleWordItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to store a multiple word item in the database" );
				}

			searchItem = searchItem.nextMultipleWordItem();
			}

		searchItem = firstReplacedMultipleWordItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.storeMultipleWordItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, null, "I failed to modify a replaced multiple word item in the database" );
				}

			searchItem = searchItem.nextMultipleWordItem();
			}

		return Constants.RESULT_OK;
		}
*/	};

/*************************************************************************
 *	"The one thing I ask of the Lord -
 *	the thing I seek most -
 *	is to live in the house of the Lord all the days of my life,
 *	delighting in the Lord's perfections
 *	and meditating in his Temple." (Psalm 27:4)
 *************************************************************************/
