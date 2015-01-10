/*
 *	Class:			WriteList
 *	Parent class:	List
 *	Purpose:		To temporarily store write items
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

class WriteList extends List
	{
	// Constructor / deconstructor

	protected WriteList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_WRITE_LIST_SYMBOL, myWordItem );
		}


	// Protected virtual methods

	protected boolean isTemporaryList()
		{
		return true;
		}


	// Protected methods

	protected byte checkGrammarItemForUsage( GrammarItem unusedGrammarItem )
		{
		WriteItem searchItem = firstActiveWriteItem();

		if( unusedGrammarItem != null )
			{
			while( searchItem != null )
				{
				if( searchItem.startOfChoiceOrOptionGrammarItem() == unusedGrammarItem )
					return startError( 1, null, myWordItem().anyWordTypeString(), "The start of choice or option grammar item is still in use" );

				searchItem = searchItem.nextWriteItem();
				}
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The given unused grammar item is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte createWriteItem( boolean isSkipped, short grammarLevel, GrammarItem startOfChoiceOrOptionGrammarItem )
		{
		if( CommonVariables.currentItemNr < Constants.MAX_ITEM_NR )
			{
			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new WriteItem( isSkipped, grammarLevel, startOfChoiceOrOptionGrammarItem, this, myWordItem() ) ) != Constants.RESULT_OK )
				return addError( 1, null, myWordItem().anyWordTypeString(), "I failed to add an active write item" );
			}
		else
			return startError( 1, null, myWordItem().anyWordTypeString(), "The current item number is undefined" );

		return Constants.RESULT_OK;
		}

	protected WriteItem firstActiveWriteItem()
		{
		return (WriteItem)firstActiveItem();
		}
	};

/*************************************************************************
 *
 *	"God says, "At the time I have planned,
 *	I will bring justice against the wicked.
 *	When the earth quackes and its people live in turmoil,
 *	I am the one that keeps its foundations firm." (Psalm 75:2-3)
 *
 *************************************************************************/
