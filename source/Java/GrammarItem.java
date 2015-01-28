/*
 *	Class:			GrammarItem
 *	Parent class:	Item
 *	Purpose:		To store info about the grammar of a language, which
 *					will be used for reading as well as writing sentences
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

class GrammarItem extends Item
	{
	// Private loadable variables

	private boolean isDefinitionStart_;
	private boolean isNewStart_;
	private boolean isOptionStart_;
	private boolean isChoiceStart_;
	private boolean isSkipOptionForWriting_;

	private short grammarParameter_;
	private short grammarWordTypeNr_;

	private String grammarString_;


	// Protected constructible variables

	protected boolean isOptionEnd;
	protected boolean isChoiceEnd;
	protected boolean isGrammarItemInUse;

	protected GrammarItem nextDefinitionGrammarItem;

	protected String guideByGrammarString;


	// Protected loadable variables

	protected GrammarItem definitionGrammarItem;


	// Constructor / deconstructor

	protected GrammarItem( boolean isDefinitionStart, boolean isNewStart, boolean isOptionStart, boolean isChoiceStart, boolean isSkipOptionForWriting, short grammarWordTypeNr, short grammarParameter, int grammarStringLength, String grammarString, GrammarItem _grammarDefinitionWordItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

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
			startSystemErrorInItem( 1, null, null, "The given grammar string is undefined" );


		// Protected constructible variables

		isOptionEnd = false;
		isChoiceEnd = false;
		isGrammarItemInUse = false;

		nextDefinitionGrammarItem = null;

		guideByGrammarString = null;

		// Protected loadable variables

		definitionGrammarItem = _grammarDefinitionWordItem;
		}


	// Protected virtual methods

	protected void showString( boolean isReturnQueryToPosition )
		{
		if( CommonVariables.queryStringBuffer == null )
			CommonVariables.queryStringBuffer = new StringBuffer();

		if( grammarString_ != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			if( !isActiveItem() )	// Show status if not active
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( grammarString_ );
			}

		if( guideByGrammarString != null )
			{
			if( CommonVariables.hasFoundQuery )
				CommonVariables.queryStringBuffer.append( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING );

			if( !isActiveItem() )	// Show status if not active
				CommonVariables.queryStringBuffer.append( statusChar() );

			CommonVariables.hasFoundQuery = true;
			CommonVariables.queryStringBuffer.append( guideByGrammarString );
			}
		}

	protected boolean hasFoundParameter( int queryParameter )
		{
		return ( grammarParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				grammarParameter_ > Constants.NO_GRAMMAR_PARAMETER ) );
		}

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( definitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : definitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : definitionGrammarItem.itemNr() == queryItemNr ) ) ||

				( nextDefinitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : nextDefinitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : nextDefinitionGrammarItem.itemNr() == queryItemNr ) ) );
		}

	protected boolean hasFoundWordType( short queryWordTypeNr )
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

	protected String extraItemString()
		{
		return guideByGrammarString;
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		String grammarWordTypeString = myWordItem().wordTypeNameString( grammarWordTypeNr_ );
		baseToStringBuffer( queryWordTypeNr );

		if( isDefinitionStart_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isDefinitionStart" );

		if( isNewStart_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isNewStart" );

		if( isOptionStart_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOptionStart" );

		if( isOptionEnd )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isOptionEnd" );

		if( isChoiceStart_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isChoiceStart" );

		if( isChoiceEnd )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isChoiceEnd" );

		if( isSkipOptionForWriting_ )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSkipOptionForWriting" );
/*
		if( isGrammarItemInUse )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isGrammarItemInUse" );
*/
		if( grammarParameter_ > Constants.NO_GRAMMAR_PARAMETER )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarParameter:" + grammarParameter_ );

		if( grammarWordTypeNr_ > Constants.WORD_TYPE_UNDEFINED )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarWordType:" + ( grammarWordTypeString == null ? Constants.QUERY_WORD_TYPE_STRING + grammarWordTypeNr_ : grammarWordTypeString ) );

		if( grammarString_ != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarString:" + Constants.QUERY_STRING_START_CHAR + grammarString_ + Constants.QUERY_STRING_END_CHAR );

		if( guideByGrammarString != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "guideByGrammarString:" + Constants.QUERY_STRING_START_CHAR + guideByGrammarString + Constants.QUERY_STRING_END_CHAR );

		if( definitionGrammarItem != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "definitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + definitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + definitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		if( nextDefinitionGrammarItem != null )
			{
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nextDefinitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + nextDefinitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + nextDefinitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );
			}

		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

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

	protected boolean isWordEnding()
		{
		return ( grammarParameter_ == Constants.WORD_FEMININE_PROPER_NAME_ENDING ||
				grammarParameter_ == Constants.WORD_MASCULINE_PROPER_NAME_ENDING ||
				grammarParameter_ == Constants.WORD_FEMININE_SINGULAR_NOUN_ENDING ||
				grammarParameter_ == Constants.WORD_MASCULINE_SINGULAR_NOUN_ENDING ||
				grammarParameter_ == Constants.WORD_PLURAL_NOUN_ENDING );
		}

	protected boolean isGrammarDefinition()
		{
		return ( grammarParameter_ >= Constants.GRAMMAR_SENTENCE );
		}

	protected boolean isPredefinedWord()
		{
		return ( grammarWordTypeNr_ > Constants.WORD_TYPE_UNDEFINED &&
				grammarWordTypeNr_ < Constants.WORD_TYPE_TEXT &&

				grammarParameter_ > Constants.NO_GRAMMAR_PARAMETER );
		}

	protected boolean isUserDefinedWord()
		{
		return ( grammarWordTypeNr_ > Constants.WORD_TYPE_UNDEFINED &&
				grammarParameter_ == Constants.NO_GRAMMAR_PARAMETER );
		}

	protected boolean isGrammarStart()
		{
		return ( grammarParameter_ == Constants.GRAMMAR_SENTENCE );
		}

	protected boolean hasWordType()
		{
		return ( grammarWordTypeNr_ > Constants.WORD_TYPE_UNDEFINED );
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
		return ( nextItem == null );	// Last item in the list
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
		GrammarItem searchItem = this;

		if( checkGrammarItem != null &&
		( checkGrammarString = checkGrammarItem.grammarString() ) != null )
			{
			while( searchItem != null )
				{
				if( searchItem.isDefinitionStart_ &&
				searchItem != checkGrammarItem &&
				searchItem.grammarString_.equals( checkGrammarString ) )
					return false;

				searchItem = searchItem.nextGrammarItem();
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
