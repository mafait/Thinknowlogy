/*	Class:			GrammarItem
 *	Parent class:	Item
 *	Purpose:		Storing info about the grammar of a language, which
 *					will be used for reading as well as writing sentences
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

class GrammarItem extends Item
	{
	// Private initialized variables

	private boolean isDefinitionStart_ = false;
	private boolean isNewStart_ = false;
	private boolean isOptionStart_ = false;
	private boolean isChoiceStart_ = false;
	private boolean isSkipOptionForWriting_ = false;

	private short grammarParameter_ = Constants.NO_GRAMMAR_PARAMETER;
	private short grammarWordTypeNr_ = Constants.NO_WORD_TYPE_NR;

	private String grammarString_ = null;


	// Protected constructed variables

	protected boolean isOptionEnd;
	protected boolean isChoiceEnd;
	protected boolean isGrammarItemInUse;

	protected GrammarItem nextDefinitionGrammarItem;


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

		// Checking private initialized variables

		if( grammarString != null )
			grammarString_ = grammarString.substring( 0, grammarStringLength );
		else
			startSystemError( 1, null, null, "The given grammar string is undefined" );


		// Protected constructed variables

		isOptionEnd = false;
		isChoiceEnd = false;
		isGrammarItemInUse = false;

		nextDefinitionGrammarItem = null;

		// Protected initialized variables

		definitionGrammarItem = _definitionGrammarItem;
		}


	// Protected virtual methods

	@Override
	protected void displayString( boolean isReturnQueryToPosition )
		{
		StringBuffer queryStringBuffer;

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( grammarString_ != null )
			{
			if( GlobalVariables.hasFoundQuery )
				queryStringBuffer.append( ( isReturnQueryToPosition ? Constants.NEW_LINE_STRING : Constants.QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				queryStringBuffer.append( statusChar() );

			GlobalVariables.hasFoundQuery = true;
			queryStringBuffer.append( grammarString_ );
			}
		}

	@Override
	protected boolean hasParameter( int queryParameter )
		{
		return ( grammarParameter_ == queryParameter ||

				( queryParameter == Constants.MAX_QUERY_PARAMETER &&
				grammarParameter_ > Constants.NO_GRAMMAR_PARAMETER ) );
		}

	@Override
	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( ( definitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : definitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : definitionGrammarItem.itemNr() == queryItemNr ) ) ||

				( nextDefinitionGrammarItem == null ? false :
					( querySentenceNr == Constants.NO_SENTENCE_NR ? true : nextDefinitionGrammarItem.creationSentenceNr() == querySentenceNr ) &&
					( queryItemNr == Constants.NO_ITEM_NR ? true : nextDefinitionGrammarItem.itemNr() == queryItemNr ) ) );
		}

	@Override
	protected boolean hasWordType( short queryWordTypeNr )
		{
		return ( grammarWordTypeNr_ == queryWordTypeNr );
		}

	@Override
	protected String itemString()
		{
		return grammarString_;
		}

	@Override
	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;
		String grammarWordTypeString = myWordItem().wordTypeNameString( grammarWordTypeNr_ );

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

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

		if( definitionGrammarItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "definitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + definitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + definitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		if( nextDefinitionGrammarItem != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "nextDefinitionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + nextDefinitionGrammarItem.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + nextDefinitionGrammarItem.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return queryStringBuffer;
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

	protected boolean isGrammarDefinition()
		{
		return ( grammarParameter_ >= Constants.GRAMMAR_SENTENCE );
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

	protected boolean isUndefinedWord()
		{
		// Last item in the list
		return ( nextItem == null );
		}

	protected boolean isIdentical( GrammarItem checkGrammarItem )
		{
		return ( checkGrammarItem != null &&

				checkGrammarItem.isNewStart() == isNewStart_ &&
				checkGrammarItem.isOptionStart() == isOptionStart_ &&
				checkGrammarItem.isOptionEnd == isOptionEnd &&
				checkGrammarItem.isChoiceStart() == isChoiceStart_ &&
				checkGrammarItem.isChoiceEnd == isChoiceEnd &&
				checkGrammarItem.grammarParameter() == grammarParameter_ &&
				checkGrammarItem.grammarWordTypeNr() == grammarWordTypeNr_ &&
				checkGrammarItem.itemNr() == itemNr() &&

				grammarString_ != null &&
				checkGrammarItem.grammarString() != null &&
				grammarString_.equals( checkGrammarItem.grammarString() ) );
		}

	protected boolean isUsefulGrammarDefinition( boolean isAssignment, boolean isArchivedAssignment, boolean isChineseCurrentLanguage, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization )
		{		// Statement
		return ( ( !isQuestion &&

				( isArchivedAssignment ||
				grammarParameter_ < Constants.GRAMMAR_STATEMENT_ASSIGNMENT ||
				grammarParameter_ > Constants.GRAMMAR_STATEMENT_SPECIFICATION_GENERALIZATION ||

				// Generalization-specification
				( !isSpecificationGeneralization &&

				( ( !isAssignment &&
				grammarParameter_ == Constants.GRAMMAR_STATEMENT_SPECIFICATION ) ||

				( isAssignment &&

				( isPossessive ||
				isChineseCurrentLanguage ||
				grammarParameter_ == Constants.GRAMMAR_STATEMENT_ASSIGNMENT ) ) ) ) ||

				// Specification-generalization
				( isSpecificationGeneralization &&
				grammarParameter_ == Constants.GRAMMAR_STATEMENT_SPECIFICATION_GENERALIZATION ) ) ) ||

				// Question
				( isQuestion &&

				( grammarParameter_ < Constants.GRAMMAR_QUESTION_SPECIFICATION ||
				grammarParameter_ > Constants.GRAMMAR_QUESTION_SPECIFICATION_GENERALIZATION ||

				( !isSpecificationGeneralization &&
				grammarParameter_ == Constants.GRAMMAR_QUESTION_SPECIFICATION ) ||

				( isSpecificationGeneralization &&
				grammarParameter_ == Constants.GRAMMAR_QUESTION_SPECIFICATION_GENERALIZATION ) ) ) );
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
		GrammarItem nextEndingGrammarItem;

		return ( ( nextEndingGrammarItem = nextGrammarItem() ) != null &&
				nextEndingGrammarItem.grammarParameter() == grammarParameter_ ? nextEndingGrammarItem : null );
		}
	}

/*************************************************************************
 *	"For the Lord is good.
 *	His unfailing love continues forever,
 *	and his faithfulness continues to each generation." (Psalm 100:5)
 *************************************************************************/
