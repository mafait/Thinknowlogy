/*	Class:			MultipleWordItem
 *	Parent class:	Item
 *	Purpose:		To store info about multiple words
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

class MultipleWordItem extends Item
	{
	// Private initialized variables

	private short nWordParts_ = 0;
	private short wordTypeLanguageNr_ = Constants.NO_LANGUAGE_NR;
	private short wordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private WordItem multipleWordItem_ = null;


	// Constructor

	protected MultipleWordItem( short nWordParts, short wordTypeLanguageNr, short wordTypeNr, WordItem multipleWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		nWordParts_ = nWordParts;
		wordTypeLanguageNr_ = wordTypeLanguageNr;
		wordTypeNr_ = wordTypeNr;

		// Checking private initialized variables

		if( ( multipleWordItem_ = multipleWordItem ) == null )
			startSystemError( 1, null, null, "The given multiple word item is undefined" );
		}


	// Protected virtual methods

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( multipleWordItem_ != null &&
		( wordString = multipleWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( multipleWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : multipleWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : multipleWordItem_.itemNr() == queryItemNr ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		WordItem thisWordItem = myWordItem();
		String languageNameString = thisWordItem.languageNameString( wordTypeLanguageNr_ );
		StringBuffer queryStringBuffer;
		String wordString;
		String wordTypeString = thisWordItem.wordTypeNameString( wordTypeNr_ );

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nWordParts:" + nWordParts_ );

		if( wordTypeLanguageNr_ > Constants.NO_LANGUAGE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "wordTypeLanguageNr:" + ( languageNameString == null ? wordTypeLanguageNr_ : languageNameString ) );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "multipleWordType:" + ( wordTypeString == null ? Constants.EMPTY_STRING : wordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + wordTypeNr_ );

		if( multipleWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "multipleWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + multipleWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + multipleWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = multipleWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		return queryStringBuffer;
		}

	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		if( multipleWordItem_ != null )
			return multipleWordItem_.findMatchingWordReferenceString( queryString );

		return new BoolResultType();
		}


	// Protected methods

	protected short nWordParts()
		{
		return nWordParts_;
		}
/*
	protected short wordTypeLanguageNr()
		{
		return wordTypeLanguageNr_;
		}
*/
	protected short wordTypeNr()
		{
		return wordTypeNr_;
		}

	protected MultipleWordItem nextMultipleWordItem()
		{
		return (MultipleWordItem)nextItem;
		}

	protected WordItem multipleWordItem()
		{
		return multipleWordItem_;
		}
	};

/*************************************************************************
 *	"The Lord is my light and my salvation -
 *	so why should I be afraid?
 *	The Lord is my fortress, protecting me from danger,light and my salvation -
 *	so why should I tremble?" (Psalm 27:1)
 *************************************************************************/
