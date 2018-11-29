/*	Class:			ContextItem
 *	Parent class:	Item
 *	Purpose:		To store the context info of a word
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

class ContextItem extends Item
	{
	// Private initialized variables

	private short contextWordTypeNr_ = Constants.NO_WORD_TYPE_NR;
	private short specificationWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private int contextNr_ = Constants.NO_CONTEXT_NR;
	private int spanishAmbiguousCollectionNr_ = Constants.NO_COLLECTION_NR;

	private WordItem specificationWordItem_ = null;


	// Constructor

	protected ContextItem( short contextWordTypeNr, short specificationWordTypeNr, int contextNr, int spanishAmbiguousCollectionNr, WordItem specificationWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		contextWordTypeNr_ = contextWordTypeNr;
		specificationWordTypeNr_ = specificationWordTypeNr;

		contextNr_ = contextNr;
		spanishAmbiguousCollectionNr_ = spanishAmbiguousCollectionNr;

		specificationWordItem_ = specificationWordItem;
		}


	// Protected virtual methods

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( specificationWordItem_ != null &&
		( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
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

	protected boolean hasParameter( int queryParameter )
		{
		return ( contextNr_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				contextNr_ > Constants.NO_CONTEXT_NR ) );
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( specificationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : specificationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : specificationWordItem_.itemNr() == queryItemNr ) ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( contextWordTypeNr_ == queryWordTypeNr ||
				specificationWordTypeNr_ == queryWordTypeNr );
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		WordItem thisWordItem = myWordItem();
		String wordString;
		StringBuffer queryStringBuffer;
		String contextWordTypeString = thisWordItem.wordTypeNameString( contextWordTypeNr_ );
		String specificationWordTypeString = thisWordItem.wordTypeNameString( specificationWordTypeNr_ );

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "contextWordType:" + ( contextWordTypeString == null ? Constants.EMPTY_STRING : contextWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + contextWordTypeNr_ );

		if( contextNr_ > Constants.NO_CONTEXT_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "contextNr:" + contextNr_ );

		if( spanishAmbiguousCollectionNr_ > Constants.NO_COLLECTION_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "spanishAmbiguousCollectionNr:" + spanishAmbiguousCollectionNr_ );

		if( specificationWordTypeNr_ > Constants.NO_WORD_TYPE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordType:" + ( specificationWordTypeString == null ? Constants.EMPTY_STRING : specificationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + specificationWordTypeNr_ );

		if( specificationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + specificationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + specificationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = specificationWordItem_.wordTypeString( true, specificationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		return queryStringBuffer;
		}

	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		if( specificationWordItem_ != null )
			return specificationWordItem_.findMatchingWordReferenceString( queryString );

		return new BoolResultType();
		}


	// Protected methods

	protected short contextWordTypeNr()
		{
		return contextWordTypeNr_;
		}

	protected short specificationWordTypeNr()
		{
		return specificationWordTypeNr_;
		}

	protected int contextNr()
		{
		return contextNr_;
		}

	protected int spanishAmbiguousCollectionNr()
		{
		return spanishAmbiguousCollectionNr_;
		}

	protected ContextItem nextContextItem()
		{
		return (ContextItem)nextItem;
		}

	protected WordItem specificationWordItem()
		{
		return specificationWordItem_;
		}
	};

/*************************************************************************
 *	"Enter his gates with thanksgiving;
 *	go into his courts with praise.
 *	Give thanks to him and praise his name." (Psalm 100:4)
 *************************************************************************/
