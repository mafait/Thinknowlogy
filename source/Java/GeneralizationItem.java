/*	Class:			GeneralizationItem
 *	Parent class:	Item
 *	Purpose:		To store info about generalizations of a word,
 *					which are the "parents" of that word,
 *					and is the opposite direction of its specifications
 *	Version:		Thinknowlogy 2018r1 (ShangDi 上帝)
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

class GeneralizationItem extends Item
	{
	// Private initialized variables

	private boolean isLanguageWord_;
	private boolean isRelation_;

	private short languageNr_;
	private short specificationWordTypeNr_;
	private short generalizationWordTypeNr_;

	private WordItem generalizationWordItem_;


	// Private methods

	private boolean isNoun()
		{
		return ( generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				generalizationWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}


	// Constructor

	protected GeneralizationItem( boolean isLanguageWord, boolean isRelation, short languageNr, short specificationWordTypeNr, short generalizationWordTypeNr, WordItem generalizationWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		isLanguageWord_ = isLanguageWord;
		isRelation_ = isRelation;

		languageNr_ = languageNr;

		specificationWordTypeNr_ = specificationWordTypeNr;
		generalizationWordTypeNr_ = generalizationWordTypeNr;

		// Checking private initialized variables

		if( ( generalizationWordItem_ = generalizationWordItem ) == null )
			startSystemError( 1, null, null, "The given generalization word item is undefined" );
		}


	// Protected virtual methods

	protected void displayWordReferences( boolean isReturnQueryToPosition )
		{
		String wordString;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		if( generalizationWordItem_ != null &&
		( wordString = generalizationWordItem_.wordTypeString( true, generalizationWordTypeNr_ ) ) != null )
			{
			if( GlobalVariables.hasFoundQuery )
				GlobalVariables.queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				GlobalVariables.queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			GlobalVariables.queryStringBuffer.append( wordString );
			}
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( generalizationWordItem_ == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : generalizationWordItem_.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : generalizationWordItem_.itemNr() == queryItemNr ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( specificationWordTypeNr_ == queryWordTypeNr ||
				generalizationWordTypeNr_ == queryWordTypeNr );
		}

	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		String generalizationWordTypeString = myWordItem().wordTypeNameString( generalizationWordTypeNr_ );
		String languageNameString = myWordItem().languageNameString( languageNr_ );
		StringBuffer queryStringBuffer;
		String specificationWordTypeString = myWordItem().wordTypeNameString( specificationWordTypeNr_ );
		String wordString;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

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

	protected BoolResultType findMatchingWordReferenceString( String queryString )
		{
		if( generalizationWordItem_ != null )
			return generalizationWordItem_.findMatchingWordReferenceString( queryString );

		return new BoolResultType();
		}


	// Protected methods

	protected boolean isRelation()
		{
		return isRelation_;
		}

	protected GeneralizationItem generalizationItem( boolean isIncludingThisItem, boolean isOnlySelectingCurrentLanguage, boolean isOnlySelectingNoun, boolean isRelation )
		{
		short currentLanguageNr = GlobalVariables.currentLanguageNr;
		GeneralizationItem searchGeneralizationItem = ( isIncludingThisItem ? this : nextGeneralizationItem() );

		while( searchGeneralizationItem != null )
			{
			if( searchGeneralizationItem.isRelation_ == isRelation &&

			( !isOnlySelectingCurrentLanguage ||
			searchGeneralizationItem.languageNr_ == currentLanguageNr ) &&

			( !isOnlySelectingNoun ||
			searchGeneralizationItem.isNoun() ) )
				return searchGeneralizationItem;

			searchGeneralizationItem = searchGeneralizationItem.nextGeneralizationItem();
			}

		return null;
		}

	protected GeneralizationItem nextGeneralizationItem()
		{
		return (GeneralizationItem)nextItem;
		}

	protected GeneralizationItem nextNounSpecificationGeneralizationItem()
		{
		return generalizationItem( false, false, true, false );
		}

	protected GeneralizationItem nextSpecificationGeneralizationItem()
		{
		return generalizationItem( false, false, false, false );
		}

	protected GeneralizationItem nextRelationGeneralizationItem()
		{
		return generalizationItem( false, false, false, true );
		}

	protected WordItem generalizationWordItem()
		{
		return generalizationWordItem_;
		}
	};

/*************************************************************************
 *	"Give thanks to him who made the heavens so skillfully.
 *	His faithful love endures forever." (Psalm 136:5)
 *************************************************************************/
