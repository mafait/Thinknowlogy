/*	Class:			MultipleWordList
 *	Parent class:	List
 *	Purpose:		To store multiple word items
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
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

class MultipleWordList extends List
	{
	// Private methods

	private boolean hasFoundMultipleWordItem( short wordTypeNr, WordItem multipleWordItem )
		{
		MultipleWordItem searchMultipleWordItem = firstActiveMultipleWordItem();

		while( searchMultipleWordItem != null )
			{
			if( searchMultipleWordItem.wordTypeNr() == wordTypeNr &&
			searchMultipleWordItem.multipleWordItem() == multipleWordItem )
				return true;

			searchMultipleWordItem = searchMultipleWordItem.nextMultipleWordItem();
			}

		return false;
		}

	private MultipleWordItem firstActiveMultipleWordItem()
		{
		return (MultipleWordItem)firstActiveItem();
		}

	// Constructor

	protected MultipleWordList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_MULTIPLE_WORD_LIST_SYMBOL, "MultipleWordList", myWordItem );
		}


	// Protected methods

	protected short matchingMultipleWordParts( String sentenceString )
		{
		MultipleWordItem searchMultipleWordItem = firstActiveMultipleWordItem();
		WordItem multipleWordItem;
		String multipleWordString;

		if( sentenceString != null )
			{
			while( searchMultipleWordItem != null )
				{
				if( ( multipleWordItem = searchMultipleWordItem.multipleWordItem() ) != null &&
				( multipleWordString = multipleWordItem.anyWordTypeString() ) != null &&
				sentenceString.startsWith( multipleWordString ) )
					return searchMultipleWordItem.nWordParts();

				searchMultipleWordItem = searchMultipleWordItem.nextMultipleWordItem();
				}
			}

		return 0;
		}

	protected byte addMultipleWord( short nWordParts, short wordTypeNr, WordItem multipleWordItem )
		{
		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return startError( 1, "The given word type number is undefined or out of bounds: " + wordTypeNr );

		if( multipleWordItem == null )
			return startError( 1, "The given multiple word item is undefined" );

		if( !hasFoundMultipleWordItem( wordTypeNr, multipleWordItem ) &&
		addItemToList( Constants.QUERY_ACTIVE_CHAR, new MultipleWordItem( nWordParts, GlobalVariables.currentLanguageNr, wordTypeNr, multipleWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
			return addError( 1, "I failed to add a multiple word item" );

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		MultipleWordItem searchMultipleWordItem = firstActiveMultipleWordItem();

		if( unusedWordItem == null )
			return startError( 1, "The given unused word item is undefined" );

		while( searchMultipleWordItem != null )
			{
			if( searchMultipleWordItem.multipleWordItem() == unusedWordItem )
				return startError( 1, "The multiple word item is still in use" );

			searchMultipleWordItem = searchMultipleWordItem.nextMultipleWordItem();
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"The one thing I ask of the Lord -
 *	the thing I seek most -
 *	is to live in the house of the Lord all the days of my life,
 *	delighting in the Lord's perfections
 *	and meditating in his Temple." (Psalm 27:4)
 *************************************************************************/
