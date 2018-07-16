/*	Class:			CollectionItem
 *	Parent class:	List
 *	Purpose:		To store collections of a word
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

class CollectionItem extends Item
	{
	// Private initialized variables

	private boolean isExclusiveSpecification_;

	private short collectionOrderNr_;
	private short collectionWordTypeNr_;
	private short commonWordTypeNr_;

	private int collectionNr_;

	private WordItem collectionWordItem_;
	private WordItem commonWordItem_;
	private WordItem compoundGeneralizationWordItem_;


	// Constructor

	protected CollectionItem( boolean isExclusiveSpecification, short collectionOrderNr, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		isExclusiveSpecification_ = isExclusiveSpecification;

		collectionOrderNr_ = collectionOrderNr;
		collectionWordTypeNr_ = collectionWordTypeNr;
		commonWordTypeNr_ = commonWordTypeNr;

		collectionNr_ = collectionNr;

		collectionWordItem_ = collectionWordItem;
		commonWordItem_ = commonWordItem;
		compoundGeneralizationWordItem_ = compoundGeneralizationWordItem;
		}


	// Protected virtual methods

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		StringBuffer queryStringBuffer;
		String wordString;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( collectionWordItem_ != null &&
		( wordString = collectionWordItem_.wordTypeString( true, collectionWordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			queryStringBuffer.append( wordString );
			}

		if( commonWordItem_ != null &&
		( wordString = commonWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery ||
			queryStringBuffer.length() > 0 )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			queryStringBuffer.append( wordString );
			}

		if( compoundGeneralizationWordItem_ != null &&
		( wordString = compoundGeneralizationWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery ||
			queryStringBuffer.length() > 0 )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( collectionOrderNr_ == queryParameter ||
				collectionNr_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( collectionOrderNr_ > Constants.NO_ORDER_NR ||
				collectionNr_ > Constants.NO_COLLECTION_NR ) ) );
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( collectionWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : collectionWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : collectionWordItem_.itemNr() == queryItemNr ) ) ||

				( commonWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : commonWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : commonWordItem_.itemNr() == queryItemNr ) ) ||

				( compoundGeneralizationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : compoundGeneralizationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : compoundGeneralizationWordItem_.itemNr() == queryItemNr ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( collectionWordTypeNr_ == queryWordTypeNr ||
				commonWordTypeNr_ == queryWordTypeNr );
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		String collectionWordTypeString = myWordItem().wordTypeNameString( collectionWordTypeNr_ );
		String commonWordTypeString = myWordItem().wordTypeNameString( commonWordTypeNr_ );
		StringBuffer queryStringBuffer;
		String wordString;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( isExclusiveSpecification_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecification" );

		if( collectionNr_ > Constants.NO_COLLECTION_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionNr:" + collectionNr_ );

		if( collectionOrderNr_ > Constants.NO_ORDER_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionOrderNr:" + collectionOrderNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionWordType:" + ( collectionWordTypeString == null ? Constants.EMPTY_STRING : collectionWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + collectionWordTypeNr_ );

		if( collectionWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + collectionWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + collectionWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = collectionWordItem_.wordTypeString( true, collectionWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "commonWordType:" + ( commonWordTypeString == null ? Constants.EMPTY_STRING : commonWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + commonWordTypeNr_ );

		if( commonWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "commonWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + commonWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + commonWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = commonWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( compoundGeneralizationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "compoundGeneralizationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + compoundGeneralizationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + compoundGeneralizationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = compoundGeneralizationWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		return queryStringBuffer;
		}

	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		BoolResultType boolResult = new BoolResultType();

		if( collectionWordItem_ != null &&
		// Collection word
		( boolResult = collectionWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
			return addBoolResultError( 1, null, null, "I failed to find a matching word reference string for the collected word item" );

		// No matching string
		if( !boolResult.booleanValue &&
		commonWordItem_ != null &&
		// Common word
		( boolResult = commonWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
			return addBoolResultError( 1, null, null, "I failed to find a matching word reference string for the common word item" );

		// No matching string
		if( !boolResult.booleanValue &&
		compoundGeneralizationWordItem_ != null &&
		// Compound generalization word
		( boolResult = compoundGeneralizationWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
			return addBoolResultError( 1, null, null, "I failed to find a matching word reference string for the compound word item" );

		return boolResult;
		}


	// Protected methods

	protected boolean hasFemaleCollectionWord()
		{
		return ( collectionWordItem_ != null &&
				collectionWordItem_.isFemale() );
		}

	protected boolean hasMaleCollectionWord()
		{
		return ( collectionWordItem_ != null &&
				collectionWordItem_.isMale() );
		}

	protected boolean isCompoundGeneralization()
		{
		return ( compoundGeneralizationWordItem_ != null );
		}

	protected boolean isExclusiveSpecification()
		{
		return isExclusiveSpecification_;
		}

	protected boolean isMatchingCollectionWordTypeNr( short collectionWordTypeNr )
		{
		return isMatchingWordType( collectionWordTypeNr_, collectionWordTypeNr );
		}

	protected short collectionOrderNr()
		{
		return collectionOrderNr_;
		}

	protected short collectionWordTypeNr()
		{
		return collectionWordTypeNr_;
		}

	protected int collectionNr()
		{
		return collectionNr_;
		}

	protected CollectionItem nextCollectionItem()
		{
		return (CollectionItem)nextItem;
		}

	protected WordItem collectionWordItem()
		{
		return collectionWordItem_;
		}

	protected WordItem commonWordItem()
		{
		return commonWordItem_;
		}

	protected WordItem compoundGeneralizationWordItem()
		{
		return compoundGeneralizationWordItem_;
		}
	};

/*************************************************************************
 *	"Let them praise to Lord for his great love
 *	and for the wonderful things he has done for them." (Psalm 107:8)
 *************************************************************************/
