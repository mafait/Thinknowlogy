/*	Class:			GeneralizationItem
 *	Parent class:	Item
 *	Purpose:		To store info about generalizations of a word,
 *					which are the "parents" of that word,
 *					and is the opposite direction of its specifications
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

class GeneralizationItem extends Item
	{
	// Private loadable variables

	private boolean isLanguageWord_;
	private boolean isRelation_;

	private short languageNr_;
	private short specificationWordTypeNr_;
	private short generalizationWordTypeNr_;

	private WordItem generalizationWordItem_;


	// Constructor / deconstructor

	protected GeneralizationItem( boolean isLanguageWord, boolean isRelation, short languageNr, short specificationWordTypeNr, short generalizationWordTypeNr, WordItem generalizationWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

		isLanguageWord_ = isLanguageWord;
		isRelation_ = isRelation;

		languageNr_ = languageNr;

		specificationWordTypeNr_ = specificationWordTypeNr;
		generalizationWordTypeNr_ = generalizationWordTypeNr;

		generalizationWordItem_ = generalizationWordItem;

		if( generalizationWordItem_ == null )
			startSystemError( 1, null, null, "The given generalization word item is undefined" );
		}


	// Protected virtual methods

	protected void showWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( generalizationWordItem_ != null &&
		( wordString = generalizationWordItem_.wordTypeString( true, generalizationWordTypeNr_ ) ) != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( generalizationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : generalizationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : generalizationWordItem_.itemNr() == queryItemNr ) );
		}

	protected boolean hasFoundWordType( short queryWordTypeNr )
		{
		return ( specificationWordTypeNr_ == queryWordTypeNr ||
				generalizationWordTypeNr_ == queryWordTypeNr );
		}

	protected ReferenceResultType findMatchingWordReferenceString( String queryString )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();

		if( generalizationWordItem_ != null )
			{
			if( ( referenceResult = generalizationWordItem_.findMatchingWordReferenceString( queryString ) ).result != Constants.RESULT_OK )
				addError( 1, null, "I failed to find a matching word reference string for the generalization word" );
			}

		return referenceResult;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;
		String wordString;
		String languageNameString = myWordItem().languageNameString( languageNr_ );
		String generalizationWordTypeString = myWordItem().wordTypeNameString( generalizationWordTypeNr_ );
		String specificationWordTypeString = myWordItem().wordTypeNameString( specificationWordTypeNr_ );

		baseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( isLanguageWord_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isLanguageWord" );

		if( isRelation_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isRelation" );

		if( languageNr_ > Constants.NO_LANGUAGE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + ( languageNameString == null ? ( "languageNr:" + languageNr_ ) : ( "language:" + languageNameString ) ) );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "specificationWordType:" + ( specificationWordTypeString == null ? Constants.EMPTY_STRING : specificationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + specificationWordTypeNr_ );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationWordType:" + ( generalizationWordTypeString == null ? Constants.EMPTY_STRING : generalizationWordTypeString ) + Constants.QUERY_WORD_TYPE_STRING + generalizationWordTypeNr_ );

		if( generalizationWordItem_ != null )
			{
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "generalizationWordItem" + Constants.QUERY_REF_ITEM_START_CHAR + generalizationWordItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + generalizationWordItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

			if( ( wordString = generalizationWordItem_.wordTypeString( true, generalizationWordTypeNr_ ) ) != null )
				queryStringBuffer.append( Constants.QUERY_WORD_REFERENCE_START_CHAR + wordString + Constants.QUERY_WORD_REFERENCE_END_CHAR );
			}

		return queryStringBuffer;
		}


	// Protected methods

	protected boolean isRelation()
		{
		return isRelation_;
		}

	protected short generalizationWordTypeNr()
		{
		return generalizationWordTypeNr_;
		}

	protected short languageNr()
		{
		return languageNr_;
		}

	protected WordItem generalizationWordItem()
		{
		return generalizationWordItem_;
		}

	protected GeneralizationItem getGeneralizationItem( boolean isIncludingThisItem, boolean isOnlySelectingCurrentLanguage, boolean isOnlySelectingNoun, boolean isRelation )
		{
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		GeneralizationItem searchItem = ( isIncludingThisItem ? this : nextGeneralizationItem() );

		while( searchItem != null )
			{
			if( searchItem.isRelation_ == isRelation &&

			( !isOnlySelectingCurrentLanguage ||
			searchItem.languageNr_ == currentLanguageNr ) &&

			( !isOnlySelectingNoun ||
			isSingularOrPluralNoun( searchItem.generalizationWordTypeNr_ ) ) )
				return searchItem;

			searchItem = searchItem.nextGeneralizationItem();
			}

		return null;
		}

	protected GeneralizationItem nextGeneralizationItem()
		{
		return (GeneralizationItem)nextItem;
		}

	protected GeneralizationItem nextNounSpecificationGeneralizationItem()
		{
		return getGeneralizationItem( false, false, true, false );
		}

	protected GeneralizationItem nextSpecificationGeneralizationItem()
		{
		return getGeneralizationItem( false, false, false, false );
		}

	protected GeneralizationItem nextRelationGeneralizationItem()
		{
		return getGeneralizationItem( false, false, false, true );
		}
	};

/*************************************************************************
 *	"Give thanks to him who made the heavens so skillfully.
 *	His faithful love endures forever." (Psalm 136:5)
 *************************************************************************/
