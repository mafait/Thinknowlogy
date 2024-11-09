/*	Class:			WriteItem
 *	Parent class:	Item
 *	Purpose:		Temporarily storing info about a word
 *					during the process of writing a sentence
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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

class WriteItem extends Item
	{
	// Private initialized variables

	private short grammarLevel_ = Constants.NO_GRAMMAR_LEVEL;

	private GrammarItem startOfChoiceOrOptionGrammarItem_ = null;


	// Protected initialized variables

	protected boolean isSkipped = false;


	// Constructor

	protected WriteItem( boolean _isSkipped, short grammarLevel, GrammarItem startOfChoiceOrOptionGrammarItem, List myList, WordItem myWordItem )
		{
		// Used for developer statistics
		GlobalVariables.nCreatedWriteItems++;

		initializeItemVariables( Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, myList, myWordItem );

		// Private initialized variables

		grammarLevel_ = grammarLevel;

		// Protected initialized variables

		isSkipped = _isSkipped;

		// Checking private initialized variables

		if( ( startOfChoiceOrOptionGrammarItem_ = startOfChoiceOrOptionGrammarItem ) == null )
			startSystemError( 1, null, null, "The given start of grammar choice or option grammar item is undefined" );
		}


	// Protected virtual methods

	@Override
	protected boolean hasReferenceItemById( int querySentenceNr, int queryItemNr )
		{
		return ( startOfChoiceOrOptionGrammarItem_ == null ? false :
				( querySentenceNr == Constants.NO_SENTENCE_NR ? true : startOfChoiceOrOptionGrammarItem_.creationSentenceNr() == querySentenceNr ) &&
				( queryItemNr == Constants.NO_ITEM_NR ? true : startOfChoiceOrOptionGrammarItem_.itemNr() == queryItemNr ) );
		}

	@Override
	protected StringBuffer itemToStringBuffer( short queryWordTypeNr )
		{
		StringBuffer queryStringBuffer;

		itemBaseToStringBuffer( queryWordTypeNr );

		if( GlobalVariables.queryStringBuffer == null )
			GlobalVariables.queryStringBuffer = new StringBuffer();

		queryStringBuffer = GlobalVariables.queryStringBuffer;

		if( isSkipped )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "isSkipped" );

		queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "grammarLevel:" + grammarLevel_ );

		if( startOfChoiceOrOptionGrammarItem_ != null )
			queryStringBuffer.append( Constants.QUERY_SEPARATOR_STRING + "startOfChoiceOrOptionGrammarItem" + Constants.QUERY_REF_ITEM_START_CHAR + startOfChoiceOrOptionGrammarItem_.creationSentenceNr() + Constants.QUERY_SEPARATOR_CHAR + startOfChoiceOrOptionGrammarItem_.itemNr() + Constants.QUERY_REF_ITEM_END_CHAR );

		return queryStringBuffer;
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
	}

/*************************************************************************
 *	"Those who are wise will take all this to heart;
 *	they will see in our history the faithful love
 *	of the Lord." (Psalm 107:43)
 *************************************************************************/
