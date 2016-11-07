/*	Class:			GrammarItem
 *	Parent class:	Item
 *	Purpose:		To store info about the grammar of a language, which
 *					will be used for reading as well as writing sentences
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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

class GrammarItem extends Item
	{
	// Private initialized variables

	private boolean isDefinitionStart_;
	private boolean isNewStart_;
	private boolean isOptionStart_;
	private boolean isChoiceStart_;
	private boolean isSkipOptionForWriting_;

	private short grammarParameter_;
	private short grammarWordTypeNr_;

	private String grammarString_;


	// Protected constructed variables

	protected boolean isOptionEnd;
	protected boolean isChoiceEnd;
	protected boolean isGrammarItemInUse;

	protected GrammarItem nextDefinitionGrammarItem;

	protected String guideByGrammarString;


	// Protected initialized variables

	protected GrammarItem definitionGrammarItem;


	// Constructor

	protected GrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short grammarWordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem _definitionGrammarItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		isDefinitionStart_ = isDefinitionStart;
		isNewStart_ = isNewStart;
		isOptionStart_ = isOptionStart;
		isChoiceStart_ = isChoiceStart;
		isSkipOptionForWriting_ = isSkipOptionForWriting;

		grammarParameter_ = grammarParameter;
		grammarWordTypeNr_ = grammarWordTypeNr;

		grammarString_ = null;

		if( grammarString != null )
			grammarString_ = grammarString.substring( 0, grammarStringLength );
		else
			startSystemError( 1, null, null, "The given grammar string is undefined" );


		// Protected constructed variables

		isOptionEnd = false;
		isChoiceEnd = false;
		isGrammarItemInUse = false;

		nextDefinitionGrammarItem = null;

		guideByGrammarString = null;

		// Protected initialized variables

		definitionGrammarItem = _definitionGrammarItem;
		}


	// Protected virtual methods

	protected void displayString( boolean isReturnQueryToPosition )
		{
		StringBuffer queryStringBuffer;

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( grammarString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			queryStringBuffer.append( grammarString_ );
			}

		if( guideByGrammarString != null )
			{
			if( CommonVariables.hasFoundQuery )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			queryStringBuffer.append( guideByGrammarString );
			}
		}

	protected boolean hasParameter( int queryParameter )
		{
		return ( grammarParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				grammarParameter_ > Constants.NO_GRAMMAR_PARAMETER ) );
		}

	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( definitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : definitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : definitionGrammarItem.itemNr() == queryItemNr ) ) ||

				( nextDefinitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : nextDefinitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : nextDefinitionGrammarItem.itemNr() == queryItemNr ) ) );
		}

	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( grammarWordTypeNr_ == queryWordTypeNr );
		}

	protected byte checkForUsage()
		{
		return myWordItem().checkGrammarForUsageInWord( this );
		}

	protected String itemString()
		{
		return grammarString_;
		}

	protected String virtualGuideByGrammarString()
		{
		return guideByGrammarString;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;
		String grammarWordTypeString = myWordItem().wordTypeNameString( grammarWordTypeNr_ );

		baseToStringBuffer( queryWordTypeNr );

		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = CommonVariables.queryStringBuffer;

		if( isDefinitionStart_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isDefinitionStart" );

		if( isNewStart_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNewStart" );

		if( isOptionStart_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOptionStart" );

		if( isOptionEnd )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOptionEnd" );

		if( isChoiceStart_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isChoiceStart" );

		if( isChoiceEnd )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isChoiceEnd" );

		if( isSkipOptionForWriting_ )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSkipOptionForWriting" );
/*
		if( isGrammarItemInUse )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isGrammarItemInUse" );
*/
		if( grammarParameter_ > Constants.NO_GRAMMAR_PARAMETER )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarParameter:" + grammarParameter_ );

		if( grammarWordTypeNr_ > Constants.NO_WORD_TYPE_NR )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarWordType:" + ( grammarWordTypeString == null ? Constants.QUERY_WORD_TYPE_STRING + grammarWordTypeNr_ : grammarWordTypeString ) );

		if( grammarString_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarString:" + Constants.QUERY_STRING_START_CHAR + grammarString_ + Constants.QUERY_STRING_END_CHAR );

		if( guideByGrammarString != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "guideByGrammarString:" + Constants.QUERY_STRING_START_CHAR + guideByGrammarString + Constants.QUERY_STRING_END_CHAR );

		if( definitionGrammarItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "definitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + definitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + definitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( nextDefinitionGrammarItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nextDefinitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + nextDefinitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + nextDefinitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return queryStringBuffer;
		}


	// Protected methods

	protected boolean isAllowedToEnterNewWord()
		{
		return ( grammarWordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME ||
				grammarWordTypeNr_ == Constants.WORD_TYPE_ADJECTIVE ||
				grammarWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR ||
				grammarWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isDefinitionStart()
		{
		return isDefinitionStart_;
		}

	protected boolean isNewStart()
		{
		return isNewStart_;
		}

	protected boolean isOptionStart()
		{
		return isOptionStart_;
		}

	protected boolean isChoiceStart()
		{
		return isChoiceStart_;
		}

	protected boolean isSkipOptionForWriting()
		{
		return isSkipOptionForWriting_;
		}

	protected boolean isGrammarDefinition()
		{
		return ( grammarParameter_ >= Constants.GRAMMAR_SENTENCE );
		}

	protected boolean isPredefinedWord()
		{
		return ( grammarWordTypeNr_ > Constants.NO_WORD_TYPE_NR &&
				grammarWordTypeNr_ < Constants.WORD_TYPE_TEXT &&

				grammarParameter_ > Constants.NO_GRAMMAR_PARAMETER );
		}

	protected boolean isUserDefinedWord()
		{
		return ( grammarWordTypeNr_ > Constants.NO_WORD_TYPE_NR &&
				grammarParameter_ == Constants.NO_GRAMMAR_PARAMETER );
		}

	protected boolean isGrammarStart()
		{
		return ( grammarParameter_ == Constants.GRAMMAR_SENTENCE );
		}

	protected boolean hasWordType()
		{
		return ( grammarWordTypeNr_ > Constants.NO_WORD_TYPE_NR );
		}

	protected boolean isSymbol()
		{
		return ( grammarWordTypeNr_ == Constants.WORD_TYPE_SYMBOL );
		}

	protected boolean isNumeral()
		{
		return ( grammarWordTypeNr_ == Constants.WORD_TYPE_NUMERAL );
		}

	protected boolean isSingularNoun()
		{
		return ( grammarWordTypeNr_ == Constants.WORD_TYPE_NOUN_SINGULAR );
		}

	protected boolean isPluralNoun()
		{
		return ( grammarWordTypeNr_ == Constants.WORD_TYPE_NOUN_PLURAL );
		}

	protected boolean isText()
		{
		return ( grammarWordTypeNr_ == Constants.WORD_TYPE_TEXT );
		}

	protected boolean isUndefinedWord()
		{
		// Last item in the list
		return ( nextItem == null );
		}

	protected boolean isIdentical( GrammarItem checkGrammarItem )
		{
		return ( checkGrammarItem != null &&

				isNewStart_ == checkGrammarItem.isNewStart() &&
				isOptionStart_ == checkGrammarItem.isOptionStart() &&
				isOptionEnd == checkGrammarItem.isOptionEnd &&
				isChoiceStart_ == checkGrammarItem.isChoiceStart() &&
				isChoiceEnd == checkGrammarItem.isChoiceEnd &&
				grammarParameter_ == checkGrammarItem.grammarParameter() &&
				grammarWordTypeNr_ == checkGrammarItem.grammarWordTypeNr() &&
				itemNr() == checkGrammarItem.itemNr() &&

				grammarString_ != null &&
				checkGrammarItem.grammarString() != null &&
				grammarString_.equals( checkGrammarItem.grammarString() ) );
		}

	protected boolean isUniqueGrammarDefinition( GrammarItem checkGrammarItem )
		{
		String checkGrammarString;
		GrammarItem searchGrammarItem = this;

		if( checkGrammarItem != null &&
		( checkGrammarString = checkGrammarItem.grammarString() ) != null )
			{
			while( searchGrammarItem != null )
				{
				if( searchGrammarItem.isDefinitionStart_ &&
				searchGrammarItem != checkGrammarItem &&
				searchGrammarItem.grammarString_.equals( checkGrammarString ) )
					return false;

				searchGrammarItem = searchGrammarItem.nextGrammarItem();
				}
			}

		return true;
		}

	protected short adjectiveParameter()
		{
		if( isAdjectiveParameter( grammarParameter_ ) )
			return grammarParameter_;

		return Constants.NO_ADJECTIVE_PARAMETER;
		}

	protected short definiteArticleParameter()
		{
		if( isDefiniteArticleParameter( grammarParameter_ ) )
			return grammarParameter_;

		return Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		}

	protected short indefiniteArticleParameter()
		{
		if( isIndefiniteArticleParameter( grammarParameter_ ) )
			return grammarParameter_;

		return Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		}

	protected short grammarParameter()
		{
		return grammarParameter_;
		}

	protected short grammarWordTypeNr()
		{
		return grammarWordTypeNr_;
		}

	protected String grammarString()
		{
		return grammarString_;
		}

	protected GrammarItem nextGrammarItem()
		{
		return (GrammarItem)nextItem;
		}

	protected GrammarItem nextWordEndingGrammarItem()
		{
		GrammarItem nextEndingGrammarItem = nextGrammarItem();

		return ( nextEndingGrammarItem == null || nextEndingGrammarItem.grammarParameter() != grammarParameter_ ? null : nextEndingGrammarItem );
		}
	};

/*************************************************************************
 *	"For the Lord is good.
 *	His unfailing love continues forever,
 *	and his faithfulness continues to each generation." (Psalm 100:5)
 *************************************************************************/
