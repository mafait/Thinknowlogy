/*	Class:			ContextList
 *	Parent class:	List
 *	Purpose:		To store context items
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

package org.mafait.thinknowlogy;

class ContextList extends List
	{
	// Private constructed variables

	private boolean isMarkedAsContextWord_ = false;


	// Private methods

	private boolean hasRelationContextInSpecificationsOfGeneralizationWords( int relationContextNr )
		{
		GeneralizationItem currentGeneralizationItem;
		WordItem currentGeneralizationWordItem;

		if( ( currentGeneralizationItem = myWordItem().firstGeneralizationItem() ) != null )
			{
			// Do for all generalization words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) != null &&
				currentGeneralizationWordItem.hasRelationContextInSpecificationsInWord( relationContextNr ) )
					return true;
				}
			while( ( currentGeneralizationItem = currentGeneralizationItem.nextGeneralizationItem() ) != null );
			}

		return false;
		}

	// Constructor

	protected ContextList( WordItem myWordItem )
		{
		initializeListVariables( Constants.WORD_CONTEXT_LIST_SYMBOL, "ContextList", myWordItem );
		}


	// Protected methods

	protected void addToContextWordQuickAccessList()
		{
		WordItem lastContextWordItem = GlobalVariables.lastContextWordItem;

		if( lastContextWordItem == null )
			GlobalVariables.firstContextWordItem = myWordItem();
		else
			// Word order is important: Add word at end of context word list
			lastContextWordItem.nextContextWordItem = myWordItem();

		GlobalVariables.lastContextWordItem = myWordItem();
		}

	protected boolean hasContext( int contextNr )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.contextNr() == contextNr )
				return true;

			searchContextItem = searchContextItem.nextContextItem();
			}

		return false;
		}

	protected boolean hasContext( int contextNr, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.contextNr() == contextNr &&
			searchContextItem.specificationWordItem() == specificationWordItem )
				return true;

			searchContextItem = searchContextItem.nextContextItem();
			}

		return false;
		}

	protected boolean hasContext( int contextNr, int spanishAmbiguousCollectionNr, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.contextNr() == contextNr &&
			searchContextItem.spanishAmbiguousCollectionNr() == spanishAmbiguousCollectionNr &&
			searchContextItem.specificationWordItem() == specificationWordItem )
				return true;

			searchContextItem = searchContextItem.nextContextItem();
			}

		return false;
		}

	protected boolean hasContextCurrentlyBeenUpdated( int contextNr )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.hasCurrentCreationSentenceNr() &&
			searchContextItem.contextNr() == contextNr )
				return true;

			searchContextItem = searchContextItem.nextContextItem();
			}

		return false;
		}

	protected boolean isContextSubset( int subsetContextNr, int fullSetContextNr )
		{
		ContextItem searchContextItem = firstActiveContextItem();
		WordItem specificationWordItem;

		while( searchContextItem != null )
			{
			if( searchContextItem.contextNr() == subsetContextNr &&
			( specificationWordItem = searchContextItem.specificationWordItem() ) != null &&
			hasContext( fullSetContextNr, specificationWordItem ) )
				return true;

			searchContextItem = searchContextItem.nextContextItem();
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

	protected int contextNr( int spanishAmbiguousCollectionNr, WordItem specificationWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.spanishAmbiguousCollectionNr() == spanishAmbiguousCollectionNr &&
			searchContextItem.specificationWordItem() == specificationWordItem )
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

	protected byte addContext( short contextWordTypeNr, short specificationWordTypeNr, int contextNr, int spanishAmbiguousCollectionNr, WordItem specificationWordItem )
		{
		ContextItem foundContextItem;

		if( contextNr <= Constants.NO_CONTEXT_NR )
			return startError( 1, "The given context number is undefined" );

		if( ( foundContextItem = contextItem( contextNr ) ) == null )
			{
			if( contextWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
			contextWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
				return startError( 1, "The given context word type is undefined or out of bounds: " + contextWordTypeNr );

			if( specificationWordItem != null &&

			( specificationWordTypeNr <= Constants.NO_WORD_TYPE_NR ||
			specificationWordTypeNr >= Constants.NUMBER_OF_WORD_TYPES ) )
				return startError( 1, "The given specification word type is undefined or out of bounds: " + specificationWordTypeNr );

			if( !isMarkedAsContextWord_ )
				{
				isMarkedAsContextWord_ = true;
				addToContextWordQuickAccessList();
				}

			if( addItemToList( Constants.QUERY_ACTIVE_CHAR, new ContextItem( contextWordTypeNr, ( specificationWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ? Constants.WORD_TYPE_NOUN_SINGULAR : specificationWordTypeNr ), contextNr, spanishAmbiguousCollectionNr, specificationWordItem, this, myWordItem() ) ) != Constants.RESULT_OK )
				return addError( 1, "I failed to add a context item" );
			}
		else
			{
			if( foundContextItem.specificationWordItem() != specificationWordItem )
				{
				if( foundContextItem.specificationWordItem() == null )
					return startError( 1, "The given context number is assigned to a pronoun context" );

				return startError( 1, "The given context number is already assigned to another specification word: \"" + foundContextItem.specificationWordItem().anyWordTypeString() + "\"" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte checkForUnusedRelationContext()
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null &&
		!GlobalVariables.hasDisplayedIntegrityWarning )
			{
			if( !hasRelationContextInSpecificationsOfGeneralizationWords( searchContextItem.contextNr() ) &&
			InputOutput.writeDiacriticalText( Constants.INPUT_OUTPUT_PROMPT_WARNING_INTEGRITY, "\nI found unused relation context number: " + searchContextItem.contextNr() + ";\n\tContextItem: " + searchContextItem.itemToStringBuffer( Constants.NO_WORD_TYPE_NR ) + ".\n" ) != Constants.RESULT_OK )
				return startError( 1, "I failed to write an interface warning" );

			searchContextItem = searchContextItem.nextContextItem();
			}

		return Constants.RESULT_OK;
		}

	protected byte checkWordItemForUsage( WordItem unusedWordItem )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		if( unusedWordItem == null )
			return startError( 1, "The given unused word item is undefined" );

		while( searchContextItem != null )
			{
			if( searchContextItem.specificationWordItem() == unusedWordItem )
				return startError( 1, "The specification word item is still in use" );

			searchContextItem = searchContextItem.nextContextItem();
			}

		return Constants.RESULT_OK;
		}

	protected ContextItem firstActiveContextItem()
		{
		return (ContextItem)firstActiveItem();
		}

	protected ContextItem contextItem( int contextNr )
		{
		ContextItem searchContextItem = firstActiveContextItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.contextNr() == contextNr )
				return searchContextItem;

			searchContextItem = searchContextItem.nextContextItem();
			}

		return null;
		}

	protected ContextItem contextItem( boolean isCompoundCollectionSpanishAmbiguous, int nContextWords, int spanishAmbiguousCollectionNr, WordItem specificationWordItem )
		{
		int searchSpanishAmbiguousCollectionNr;
		ContextItem searchContextItem = firstActiveContextItem();
		WordItem anyWordItem = myWordItem();

		while( searchContextItem != null )
			{
			if( searchContextItem.specificationWordItem() == specificationWordItem )
				{
				searchSpanishAmbiguousCollectionNr = searchContextItem.spanishAmbiguousCollectionNr();

				// No compound collection Spanish ambiguous
				if( ( !isCompoundCollectionSpanishAmbiguous &&

				( searchSpanishAmbiguousCollectionNr == Constants.NO_COLLECTION_NR ||
				// Typical for Spanish
				searchSpanishAmbiguousCollectionNr == spanishAmbiguousCollectionNr ) &&

				anyWordItem.nContextWords( searchContextItem.contextNr(), specificationWordItem ) == nContextWords ) ||

				// Typical for Spanish
				// Compound collection Spanish ambiguous
				( isCompoundCollectionSpanishAmbiguous &&
				searchSpanishAmbiguousCollectionNr == spanishAmbiguousCollectionNr ) )
					return searchContextItem;
				}

			searchContextItem = searchContextItem.nextContextItem();
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
