/*	Class:			ListCleanup
 *	Supports class:	List
 *	Purpose:		To cleanup obsolete items in the lists
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

class ListCleanup
	{
	// Private constructed variables

	private List myList_;
	private String moduleNameString_;


	// Private methods

	private static void clearReplacingInfo( Item searchItem )
		{
		int currentSentenceNr = CommonVariables.currentSentenceNr;

		while( searchItem != null )
			{
			if( searchItem.replacedSentenceNr() >= currentSentenceNr )
				searchItem.clearReplacingInfo();

			searchItem = searchItem.nextItem;
			}
		}

	private static void setCurrentItemNr( Item searchItem )
		{
		int tempItemNr;
		int currentItemNr = CommonVariables.currentItemNr;

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() &&
			( tempItemNr = searchItem.itemNr() ) > currentItemNr )
				currentItemNr = tempItemNr;

			searchItem = searchItem.nextItem;
			}

		CommonVariables.currentItemNr = currentItemNr;
		}

	private static int highestInUseSentenceNrInList( int highestSentenceNr, Item searchItem )
		{
		int tempSentenceNr;
		int highestInUseSentenceNr = Constants.NO_SENTENCE_NR;

		while( searchItem != null &&
		highestInUseSentenceNr < highestSentenceNr )
			{
			tempSentenceNr = searchItem.activeSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem.inactiveSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem.archivedSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			tempSentenceNr = searchItem.replacedSentenceNr();

			if( tempSentenceNr > highestInUseSentenceNr &&
			tempSentenceNr <= highestSentenceNr )
				highestInUseSentenceNr = tempSentenceNr;

			searchItem = searchItem.nextItem;
			}

		return highestInUseSentenceNr;
		}

	private byte decrementSentenceNrs( int startSentenceNr, Item searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.activeSentenceNr() >= startSentenceNr )
				{
				if( searchItem.activeSentenceNr() <= startSentenceNr )
					return myList_.startError( 1, moduleNameString_, "I have found an item with an active sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementActiveSentenceNr() != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to decrement the active sentence number of an item" );
				}

			if( searchItem.inactiveSentenceNr() >= startSentenceNr )
				{
				if( searchItem.inactiveSentenceNr() <= startSentenceNr )
					return myList_.startError( 1, moduleNameString_, "I have found an item with an inactive sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementInactiveSentenceNr() != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to decrement the inactive sentence number of an item" );
				}

			if( searchItem.originalSentenceNr() >= startSentenceNr )
				{
				if( searchItem.originalSentenceNr() <= startSentenceNr )
					return myList_.startError( 1, moduleNameString_, "I have found an item with an original sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementOriginalSentenceNr() != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to decrement the original sentence number of an item" );
				}

			if( searchItem.creationSentenceNr() >= startSentenceNr )
				{
				if( searchItem.creationSentenceNr() <= startSentenceNr )
					return myList_.startError( 1, moduleNameString_, "I have found an item with a creation sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementCreationSentenceNr() != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to decrement the creation sentence number of an item" );
				}

			if( searchItem.archivedSentenceNr() >= startSentenceNr )
				{
				if( searchItem.archivedSentenceNr() <= startSentenceNr )
					return myList_.startError( 1, moduleNameString_, "I have found an item with an archived sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementArchivedSentenceNr() != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to decrement the archived sentence number of an item" );
				}

			if( searchItem.replacedSentenceNr() >= startSentenceNr )
				{
				if( searchItem.replacedSentenceNr() <= startSentenceNr )
					return myList_.startError( 1, moduleNameString_, "I have found an item with a replaced sentence number equal to the given start sentence number " + startSentenceNr );

				if( searchItem.decrementReplacedSentenceNr() != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to decrement the replaced sentence number of an item" );
				}

			searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	private byte decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset, Item searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.creationSentenceNr() == decrementSentenceNr &&
			searchItem.itemNr() >= startDecrementItemNr )
				{
				if( searchItem.itemNr() <= startDecrementItemNr )
					return myList_.startError( 1, moduleNameString_, "I have found an item number equal to the given start item number" );

				if( searchItem.decrementItemNr( decrementOffset ) != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to decrement the item number of an item with a certain offset" );
				}

			searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected ListCleanup( List myList )
		{
		String errorString = null;

		myList_ = myList;
		moduleNameString_ = this.getClass().getName();

		if( myList_ == null )
			errorString = "The given my list is undefined";

		if( errorString != null )
			{
			if( myList_ != null &&
			myList_.myWordItem() != null )
				myList_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void clearReplacingInfo()
		{
		Item searchItem;

		if( ( searchItem = myList_.firstActiveItem() ) != null )
			clearReplacingInfo( searchItem );

		if( ( searchItem = myList_.firstInactiveItem() ) != null )
			clearReplacingInfo( searchItem );

		if( ( searchItem = myList_.firstArchivedItem() ) != null )
			clearReplacingInfo( searchItem );
		}

	protected void setCurrentItemNr()
		{
		Item searchItem;

		if( ( searchItem = myList_.firstActiveItem() ) != null )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_.firstInactiveItem() ) != null )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_.firstArchivedItem() ) != null )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_.firstReplacedItem() ) != null )
			setCurrentItemNr( searchItem );

		if( ( searchItem = myList_.firstDeletedItem() ) != null )
			setCurrentItemNr( searchItem );
		}

	protected int highestInUseSentenceNrInList( boolean isIncludingDeletedItems, int highestSentenceNr )
		{
		int tempSentenceNr;
		int highestInUseSentenceNr = Constants.NO_SENTENCE_NR;
		Item searchItem;

		if( ( searchItem = myList_.firstActiveItem() ) != null )
			highestInUseSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem );

		if( ( searchItem = myList_.firstInactiveItem() ) != null &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( searchItem = myList_.firstArchivedItem() ) != null &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( searchItem = myList_.firstReplacedItem() ) != null &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( isIncludingDeletedItems &&
		( searchItem = myList_.firstDeletedItem() ) != null &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, searchItem ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		return highestInUseSentenceNr;
		}

	protected byte decrementSentenceNrs( int startSentenceNr )
		{
		Item searchItem;

		if( startSentenceNr <= Constants.NO_SENTENCE_NR )
			return myList_.startError( 1, moduleNameString_, "The given start sentence number is undefined" );

		if( ( searchItem = myList_.firstActiveItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			decrementSentenceNrs( startSentenceNr, searchItem );

		return Constants.RESULT_OK;
		}

	protected byte decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		Item searchItem;

		if( decrementSentenceNr < Constants.NO_SENTENCE_NR ||
		decrementSentenceNr >= Constants.MAX_SENTENCE_NR )
			return myList_.startError( 1, moduleNameString_, "The given decrement sentence number is undefined" );

		if( startDecrementItemNr <= Constants.NO_ITEM_NR )
			return myList_.startError( 1, moduleNameString_, "The given start item number is undefined" );

		if( decrementOffset <= 0 )
			return myList_.startError( 1, moduleNameString_, "The given decrement offset is undefined" );

		if( ( searchItem = myList_.firstActiveItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstInactiveItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstArchivedItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstReplacedItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		if( CommonVariables.result == Constants.RESULT_OK &&
		( searchItem = myList_.firstDeletedItem() ) != null )
			decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, searchItem );

		// Clear error to be able to restart after justification error
		return Constants.RESULT_OK;
		}

	protected byte deleteSentences( int lowestSentenceNr )
		{
		boolean isResultOK = ( CommonVariables.result == Constants.RESULT_OK );
		Item searchItem = myList_.firstActiveItem();

		if( lowestSentenceNr <= Constants.NO_SENTENCE_NR )
			return myList_.startError( 1, moduleNameString_, "The given lowest sentence number is undefined" );

		while( searchItem != null )
			{
			if( searchItem.activeSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete an active item" );

					searchItem = myList_.nextListItem();
					}
				else
					{
					if( searchItem.wasInactiveBefore() )
						{
						if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to inactivate an active item" );

						searchItem = myList_.nextListItem();
						}
					else
						searchItem = searchItem.nextItem;
					}
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.inactiveSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete an inactive item" );
					}
				else
					{
					if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to activate an inactive item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.archivedSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete an archived item" );
					}
				else
					{
					searchItem.clearArchivedSentenceNr();

					if( searchItem.wasActiveBefore() )
						{
						if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to activate an archived item" );
						}
					else
						{
						if( !searchItem.wasInactiveBefore() )
							return myList_.startError( 1, moduleNameString_, "I don't know what to do with this archived item" );

						if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to inactivate an archived item" );
						}
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.replacedSentenceNr() >= lowestSentenceNr )
				{
				if( searchItem.creationSentenceNr() >= lowestSentenceNr )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete a replaced item" );
					}
				else
					{
					if( !isResultOK &&
					searchItem.hasCurrentReplacedSentenceNr() )
						searchItem.clearReplacingInfo();

					if( searchItem.wasActiveBefore() )
						{
						if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to activate a replaced item" );
						}
					else
						{
						if( searchItem.wasInactiveBefore() )
							{
							if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
								return myList_.addError( 1, moduleNameString_, "I failed to inactivate a replaced item" );
							}
						else
							{
							if( !searchItem.wasArchivedBefore() )
								return myList_.startError( 1, moduleNameString_, "I don't know what to do with this replaced item" );

							if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
								return myList_.addError( 1, moduleNameString_, "I failed to archive a replaced item" );
							}
						}
					}

				searchItem = myList_.nextListItem();
				}
			else
				{
				if( !isResultOK &&
				searchItem.replacedSentenceNr() == lowestSentenceNr - 1 )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete a replaced item" );

					searchItem = myList_.nextListItem();
					}
				else
					searchItem = searchItem.nextItem;
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte redoCurrentSentence()
		{
		Item searchItem = myList_.firstActiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentInactiveSentenceNr() )
				{
				if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to inactivate an active item" );

				searchItem = myList_.nextListItem();
				}
			else
				{
				if( searchItem.hasCurrentArchivedSentenceNr() )
					{
					if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to archive an active item" );

					searchItem = myList_.nextListItem();
					}
				else
					{
					if( searchItem.hasCurrentReplacedSentenceNr() )
						{
						if( myList_.replaceItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to replace an active item" );

						searchItem = myList_.nextListItem();
						}
					else
						searchItem = searchItem.nextItem;
					}
				}
			}

		searchItem = myList_.firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to activate an inactive item" );

				searchItem = myList_.nextListItem();
				}
			else
				{
				if( searchItem.hasCurrentArchivedSentenceNr() )
					{
					if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to archive an inactive item" );

					searchItem = myList_.nextListItem();
					}
				else
					searchItem = searchItem.nextItem;
				}
			}

		searchItem = myList_.firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to activate an archived item" );

				searchItem = myList_.nextListItem();
				}
			else
				{
				if( searchItem.hasCurrentInactiveSentenceNr() )
					{
					if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to inactivate an archived item" );

					searchItem = myList_.nextListItem();
					}
				else
					searchItem = searchItem.nextItem;
				}
			}

		searchItem = myList_.firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
					return myList_.addError( 1, moduleNameString_, "I failed to activate a replaced item" );

				searchItem = myList_.nextListItem();
				}
			else
				{
				if( searchItem.hasCurrentInactiveSentenceNr() )
					{
					if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to inactivate a replaced item" );

					searchItem = myList_.nextListItem();
					}
				else
					{
					if( searchItem.hasCurrentArchivedSentenceNr() )
						{
						if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to archive a replaced item" );

						searchItem = myList_.nextListItem();
						}
					else
						searchItem = searchItem.nextItem;
					}
				}
			}

		searchItem = myList_.firstDeletedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to activate a deleted item" );
					}
				else
					{
					if( searchItem.wasInactiveBefore() )
						{
						if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to inactivate a deleted item" );
						}
					else
						{
						if( !searchItem.wasArchivedBefore() )
							return myList_.startError( 1, moduleNameString_, "I don't know what to do with this deleted item" );

						if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to inactivate a deleted item" );
						}
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	protected byte undoCurrentSentence()
		{
		Item searchItem = myList_.firstActiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentActiveSentenceNr() )
				{
				if( searchItem.hasCurrentCreationSentenceNr() )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete an active item" );
					}
				else
					{
					if( !searchItem.hasInactiveSentenceNr() )
						return myList_.startError( 1, moduleNameString_, "I don't know what to do with this active item" );

					if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to inactivate an active item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentInactiveSentenceNr() )
				{
				if( searchItem.hasCurrentCreationSentenceNr() )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete an inactive item" );
					}
				else
					{
					if( !searchItem.hasActiveSentenceNr() )
						return myList_.startError( 1, moduleNameString_, "I don't know what to do with this inactive item" );

					if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to activate an inactive item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentArchivedSentenceNr() )
				{
				if( searchItem.hasCurrentCreationSentenceNr() )
					{
					if( myList_.deleteItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to delete an archived item" );
					}
				else
					{
					if( searchItem.wasActiveBefore() )
						{
						if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to activate an archived item" );
						}
					else
						{
						if( !searchItem.wasInactiveBefore() )
							return myList_.startError( 1, moduleNameString_, "I don't know what to do with this archived item" );

						if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, "I failed to inactivate an archived item" );
						}
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentReplacedSentenceNr() )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( myList_.activateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to activate a replaced item" );
					}
				else
					{
					if( !searchItem.wasInactiveBefore() )
						return myList_.startError( 1, moduleNameString_, "I don't know what to do with this replaced item" );

					if( myList_.inactivateItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, "I failed to inactivate a replaced item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"The winds blows, and we are gone -
 *	as though we had never been here.
 *	But the love of the Lord remains forever
 *	with those who fear him." (Psalm 103:16-17)
 *************************************************************************/
