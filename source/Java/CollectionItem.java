/*
 *	Class:			CollectionItem
 *	Parent class:	List
 *	Purpose:		To store collections of a word
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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
	// Private loadable variables

	private boolean isExclusiveSpecification_;

	private short collectionOrderNr_;
	private short collectionWordTypeNr_;
	private short commonWordTypeNr_;

	private int collectionNr_;

	private WordItem collectionWordItem_;
	private WordItem commonWordItem_;
	private WordItem compoundGeneralizationWordItem_;


	// Constructor / deconstructor

	protected CollectionItem( boolean isExclusiveSpecification, short collectionOrderNr, short collectionWordTypeNr, short commonWordTypeNr, int collectionNr, WordItem collectionWordItem, WordItem commonWordItem, WordItem compoundGeneralizationWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

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

	protected void showWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( collectionWordItem_ != null &&
		( wordString = collectionWordItem_.wordTypeString( true, collectionWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}

		if( commonWordItem_ != null &&
		( wordString = commonWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery ||
			CommonVariables.queryStringBuffer.length() > 0 )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}

		if( compoundGeneralizationWordItem_ != null &&
		( wordString = compoundGeneralizationWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery ||
			CommonVariables.queryStringBuffer.length() > 0 )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( collectionOrderNr_ == queryParameter ||
				collectionNr_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&

				( collectionOrderNr_ > Constants.NO_ORDER_NR ||
				collectionNr_ > Constants.NO_COLLECTION_NR ) ) );
		}

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
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

	protected boolean hasFoundWordType( short queryWordTypeNr )
		{
		return ( collectionWordTypeNr_ == queryWordTypeNr ||
				commonWordTypeNr_ == queryWordTypeNr );
		}

	protected ReferenceResultType findMatchingWordReferenceString( String queryString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( collectionWordItem_ != null )
			{
			if( ( referenceResult = collectionWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				addError( 1, null, "I failed to find a matching word reference string for the collected word item" );
			}

		if( CommonVariables.result == Constants.RESULT_OK &&
		commonWordItem_ != null )
			{
			if( ( referenceResult = commonWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				addError( 1, null, "I failed to find a matching word reference string for the common word item" );
			}

		if( CommonVariables.result == Constants.RESULT_OK &&
		compoundGeneralizationWordItem_ != null )
			{
			if( ( referenceResult = compoundGeneralizationWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				addError( 1, null, "I failed to find a matching word reference string for the compound word item" );
			}

		referenceResult.result = CommonVariables.result;
		return referenceResult;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		String wordString;
		String collectionWordTypeString = myWordItem().wordTypeNameString( collectionWordTypeNr_ );
		String commonWordTypeString = myWordItem().wordTypeNameString( commonWordTypeNr_ );

		baseToStringBuffer( queryWordTypeNr );

		if( isExclusiveSpecification_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isExclusiveSpecification" );

		if( collectionNr_ > Constants.NO_COLLECTION_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionNr:" + collectionNr_ );

		if( collectionOrderNr_ > Constants.NO_ORDER_NR )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionOrderNr:" + collectionOrderNr_ );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionWordType:" + ( collectionWordTypeString == null ? Constants.EMPTY_STRING : collectionWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + collectionWordTypeNr_ );

		if( collectionWordItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "collectionWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + collectionWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + collectionWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = collectionWordItem_.wordTypeString( true, collectionWordTypeNr_ ) ) != null )
				CommonVariables.queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "commonWordType:" + ( commonWordTypeString == null ? Constants.EMPTY_STRING : commonWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + commonWordTypeNr_ );

		if( commonWordItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "commonWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + commonWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + commonWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = commonWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
				CommonVariables.queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		if( compoundGeneralizationWordItem_ != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "compoundGeneralizationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + compoundGeneralizationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + compoundGeneralizationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = compoundGeneralizationWordItem_.wordTypeString( true, commonWordTypeNr_ ) ) != null )
				CommonVariables.queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

	protected boolean hasFemaleCollectionWord()
		{
		return ( collectionWordItem_ != null &&
				collectionWordItem_.isFemale() );
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

	protected short commonWordTypeNr()
		{
		return commonWordTypeNr_;
		}

	protected int collectionNr()
		{
		return collectionNr_;
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

	protected CollectionItem nextCollectionItem()
		{
		return (CollectionItem)nextItem;
		}
	};

/*************************************************************************
 *	"Let them praise to Lord for his great love
 *	and for the wonderful things he has done for them." (Psalm 107:8)
 *************************************************************************/
