/*	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		To store context items
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

class ContextList extends List
	{
	// Private constructed variables

	private boolean isMarkedAsContextWord_;

	// Private methods

	private boolean hasContext( boolean isCompoundCollectionSpanishAmbiguous, int contextNr, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchContextItem != null )
				{
				if( searchContextItem.contextNr() == contextNr &&
				searchContextItem.specificationWordItem() == specificationWordItem &&
				searchContextItem.isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous )
					return true;

				searchContextItem = searchContextItem.nextContextItem();
				}
			}

		return false;
		}

	// Constructor

	protected ContextList( WordItem myWordItem )
		{
		isMarkedAsContextWord_ = false;

		initializeListVariables( Constants.WORD_CONTEXT_LIST_SYMBOL, myWordItem );
		}


	// Protected methods

	protected void addToContextWordQuickAccessList()
		{
		WordItem tempWordItem;
		WordItem lastContextWordItem = CommonVariables.firstContextWordItem;

		if( lastContextWordItem == null )
			CommonVariables.firstContextWordItem = myWordItem();
		else
			{
			// Word order is important: Add word at end of context word list
			while( ( tempWordItem = lastContextWordItem.nextContextWordItem ) != null )
				lastContextWordItem = tempWordItem;

			lastContextWordItem.nextContextWordItem = myWordItem();
			}
		}

	protected boolean hasContext( int contextNr )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchContextItem != null )
				{
				if( searchContextItem.contextNr() == contextNr )
					return true;

				searchContextItem = searchContextItem.nextContextItem();
				}
			}

		return false;
		}

	protected boolean hasContext( int contextNr, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchContextItem != null )
				{
				if( searchContextItem.contextNr() == contextNr &&
				searchContextItem.specificationWordItem() == specificationWordItem )
					return true;

				searchContextItem = searchContextItem.nextContextItem();
				}
			}

		return false;
		}

	protected boolean hasContextCurrentlyBeenUpdated( int contextNr, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchContextItem != null )
				{
				if( searchContextItem.hasCurrentCreationSentenceNr() &&
				searchContextItem.contextNr() == contextNr &&
				searchContextItem.specificationWordItem() == specificationWordItem )
					return true;

				searchContextItem = searchContextItem.nextContextItem();
				}
			}

		return false;
		}

	protected boolean isContextSubset( int subsetContextNr, int fullSetContextNr )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		if( fullSetContextNr > Constants.NO_CONTEXT_NR &&
		subsetContextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchContextItem != null )
				{
				if( searchContextItem.contextNr() == subsetContextNr &&
				hasContext( fullSetContextNr, searchContextItem.specificationWordItem() ) )
					return true;

				searchContextItem = searchContextItem.nextContextItem();
				}
			}

		return false;
		}

	protected int contextNr( WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchContextItem != null )
			{
			if( searchContextItem.specificationWordItem() == specificationWordItem )
				return searchContextItem.contextNr();

			searchContextItem = searchContextItem.nextContextItem();
			}

		return Constants.NO_CONTEXT_NR;
		}

	protected int contextNr( boolean isCompoundCollectionSpanishAmbiguous, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchContextItem != null )
			{
			if( searchContextItem.specificationWordItem() == specificationWordItem &&
			searchContextItem.isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous )
				return searchContextItem.contextNr();

			searchContextItem = searchContextItem.nextContextItem();
			}

		return Constants.NO_CONTEXT_NR;
		}

	protected int highestContextNr()
		{
		int highestContextNr = Constants.NO_CONTEXT_NR;
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.contextNr() > highestContextNr )
				highestContextNr = searchContextItem.contextNr();

			searchContextItem = searchContextItem.nextContextItem();
			}

		return highestContextNr;
		}

	protected byte addContext( boolean isCompoundCollectionSpanishAmbiguous, short contextWordTypeNr, short specificationWordTypeNr, int contextNr, WordItem specificationWordItem )
		{
		if( contextNr == Constants.NO_CONTEXT_NR )
			return startError( 1, null, "The given context number is undefined" );

		if( !hasContext( isCompoundCollectionSpanishAmbiguous, contextNr, specificationWordItem ) )
			{
			if( contextWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
			contextWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
				return startError( 1, null, "The given context word type is undefined or out of bounds: " + contextWordTypeNr );

			if( specificationWordItem != null &&

			( specificationWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
			specificationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES ) )
				return startError( 1, null, "The given specification word type is undefined or out of bounds: " + specificationWordTypeNr );

			if( !isMarkedAsContextWord_ )
				{
				isMarkedAsContextWord_ = true;
				addToContextWordQuickAccessList();
				}

			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new ContextItem( isCompoundCollectionSpanishAmbiguous, contextWordTypeNr, ( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ? Constants.WORD_TYPE_NOUN_SINGULAR : specificationWordTypeNr ), contextNr, specificationWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
				return addError( 1, null, "I failed to add an active context item" );
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForUnusedContext()
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( !myWordItem().hasContextInSpecificationsInAllWords( searchContextItem.contextNr() ) )
				return startError( 1, null, "I found unused context number: " + searchContextItem.contextNr() );

			searchContextItem = searchContextItem.nextContextItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		if( unusedWordItem == null )
			return startError( 1, null, "The given unused word item is undefined" );

		while( searchContextItem != null )
			{
			if( searchContextItem.specificationWordItem() == unusedWordItem )
				return startError( 1, null, "The specification word item is still in use" );

			searchContextItem = searchContextItem.nextContextItem();
			}

		return Constants.RESULT_OK;
		}
/*
	protected byte storeChangesInFutureDatabase()
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.hasCurrentCreationSentenceNr() )
				{
				if( searchContextItem.storeContextItemInFutureDatabase() != Constants.RESULT_OK )
					return addError( 1, null, "I failed to store a context item in the database" );
				}

			searchContextItem = searchContextItem.nextContextItem();
			}

		return Constants.RESULT_OK;
		}
*/
	protected ContextItem firstActiveContextItem()
		{
		return (ContextItem)firstActiveItem();
		}

	protected ContextItem contextItem( int contextNr )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		if( contextNr > Constants.NO_CONTEXT_NR )
			{
			while( searchContextItem != null )
				{
				if( searchContextItem.contextNr() == contextNr )
					return searchContextItem;

				searchContextItem = searchContextItem.nextContextItem();
				}
			}

		return null;
		}

	protected ContextItem contextItem( WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		// In case of a pronoun context, the given specification word item will be undefined

		while( searchContextItem != null )
			{
			if( searchContextItem.specificationWordItem() == specificationWordItem )
				return searchContextItem;

			searchContextItem = searchContextItem.nextContextItem();
			}

		return null;
		}

	protected ContextItem contextItem( boolean isCompoundCollectionSpanishAmbiguous, int nContextWords, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();
		WordItem anyWordItem = myWordItem();

		// In case of a pronoun context, the given specification word item will be undefined

		if( nContextWords > 0 )
			{
			while( searchContextItem != null )
				{
				if( searchContextItem.specificationWordItem() == specificationWordItem &&
				searchContextItem.isCompoundCollectionSpanishAmbiguous() == isCompoundCollectionSpanishAmbiguous &&
				anyWordItem.nContextWordsInAllWords( searchContextItem.contextNr(), specificationWordItem ) == nContextWords )
					return searchContextItem;

				searchContextItem = searchContextItem.nextContextItem();
				}
			}

		return null;
		}
	};

/*************************************************************************
 *	"O Lord my God, you have performed many wonders for us.
 *	Your plans for us are too numerous to list.
 *	You have no equal.
 *	I have tried to recite all your wonderful deeds,
 *	I would never come to the end of them." (Psalm 40:5)
 *************************************************************************/
