/*
 *	Class:			MultipleWordItem
 *	Parent class:	Item
 *	Purpose:		To store info about multiple words
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

class MultipleWordItem extends Item
	{
	// Private loadable variables

	private short wordTypeNr_;

	private WordItem multipleWordItem_;


	// Constructor / deconstructor

	protected MultipleWordItem( short wordTypeNr, WordItem multipleWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

		wordTypeNr_ = wordTypeNr;

		multipleWordItem_ = multipleWordItem;

		if( multipleWordItem_ == null )
			startSystemErrorInItem( 1, null, null, "The given multiple word item is undefined" );
		}


	// Protected virtual methods

	protected void showWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( multipleWordItem_ != null &&
		( wordString = multipleWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			if( !isActiveItem() )	// Show status if not active
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( multipleWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : multipleWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : multipleWordItem_.itemNr() == queryItemNr ) );
		}

	protected boolean hasFoundWordType( short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	protected ReferenceResultType findMatchingWordReferenceString( String queryString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( multipleWordItem_ != null )
			{
			if( ( referenceResult = multipleWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				addErrorInItem( 1, null, myWordItem().anyWordTypeString(), "I failed to find a matching word reference string for the multiple word word" );
			}

		return referenceResult;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		String wordString;
		String wordTypeString = myWordItem().wordTypeNameString( wordTypeNr_ );

		baseToStringBuffer( queryWordTypeNr );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "multipleWordType:" + ( wordTypeString == null ? Constants.EMPTY_STRING : wordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + wordTypeNr_ );

		if( multipleWordItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "multipleWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + multipleWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + multipleWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = multipleWordItem_.wordTypeString( true, wordTypeNr_ ) ) != null )
				CommonVariables.queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

	protected boolean isSingularNoun()
		{
		return ( wordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected short wordTypeNr()
		{
		return wordTypeNr_;
		}

	protected WordItem multipleWordItem()
		{
		return multipleWordItem_;
		}

	protected MultipleWordItem nextMultipleWordItem()
		{
		return (MultipleWordItem)nextItem;
		}
	};

/*************************************************************************
 *	"The Lord is my light and my salvation -
 *	so why should I be afraid?
 *	The Lord is my fortress, protecting me from danger,light and my salvation -
 *	so why should I tremble?" (Psalm 27:1)
 *************************************************************************/
