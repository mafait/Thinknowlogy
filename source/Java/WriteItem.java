/*
 *	Class:			WriteItem
 *	Parent class:	Item
 *	Purpose:		To temporarily store info about a word
 *					during the process of writing a sentence
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class WriteItem extends Item
	{
	// Private loadable variables

	private short grammarLevel_;

	private GrammarItem startOfChoiceOrOptionGrammarItem_;


	// Protected loadable variables

	protected boolean isSkipped;


	// Constructor / deconstructor

	protected WriteItem( boolean _isSkipped, short grammarLevel, GrammarItem startOfChoiceOrOptionGrammarItem, List myList, WordItem myWordItem )
		{
		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private loadable variables

		grammarLevel_ = grammarLevel;

		// Protected loadable variables

		isSkipped = _isSkipped;

		if( ( startOfChoiceOrOptionGrammarItem_ = startOfChoiceOrOptionGrammarItem ) == null )
			startSystemErrorInItem( 1, null, null, "The given start of grammar choice or option grammar item is undefined" );
		}


	// Protected virtual methods

	protected boolean hasFoundReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( startOfChoiceOrOptionGrammarItem_ == null ? false :
				( querySentenceNr == Constants.NO_SENTENCE_NR ? true : startOfChoiceOrOptionGrammarItem_.creationSentenceNr() == querySentenceNr ) &&
				( queryItemNr == Constants.NO_ITEM_NR ? true : startOfChoiceOrOptionGrammarItem_.itemNr() == queryItemNr ) );
		}

	protected StringBuffer toStringBuffer( short queryWordTypeNr )
		{
		baseToStringBuffer( queryWordTypeNr );

		if( isSkipped )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSkipped" );

		CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarLevel:" + grammarLevel_ );

		if( startOfChoiceOrOptionGrammarItem_ != null )
			CommonVariables.queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "startOfChoiceOrOptionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + startOfChoiceOrOptionGrammarItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + startOfChoiceOrOptionGrammarItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return CommonVariables.queryStringBuffer;
		}


	// Protected methods

	protected short grammarLevel()
		{
		return grammarLevel_;
		}

	protected GrammarItem startOfChoiceOrOptionGrammarItem()
		{
		return startOfChoiceOrOptionGrammarItem_;
		}

	protected WriteItem nextWriteItem()
		{
		return (WriteItem)nextItem;
		}
	};

/*************************************************************************
 *
 *	"Those who are wise will take all this to heart;
 *	they will see in our history the faithful love
 *	of the Lord." (Psalm 107:43)
 *
 *************************************************************************/
