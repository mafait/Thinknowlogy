/*
 *	Class:			ListCleanup
 *	Supports class:	List
 *	Purpose:		To cleanup unnecessary items in the lists
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

class ListCleanup
	{
	// Private constructible variables

	private List myList_;
	private String moduleNameString_;


	// Private methods

	private static void setCurrentItemNr( Item searchItem )
		{
		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() &&
			searchItem.itemNr() > CommonVariables.currentItemNr )
				CommonVariables.currentItemNr = searchItem.itemNr();

			searchItem = searchItem.nextItem;
			}
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
				if( searchItem.activeSentenceNr() > startSentenceNr )
					{
					if( searchItem.decrementActiveSentenceNr() != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to decrement the active sentence number of an item" );
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I found an item with an active sentence number equal to the given start sentence number " + startSentenceNr );
				}

			if( searchItem.inactiveSentenceNr() >= startSentenceNr )
				{
				if( searchItem.inactiveSentenceNr() > startSentenceNr )
					{
					if( searchItem.decrementInactiveSentenceNr() != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to decrement the inactive sentence number of an item" );
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I found an item with an inactive sentence number equal to the given start sentence number " + startSentenceNr );
				}

			if( searchItem.originalSentenceNr() >= startSentenceNr )
				{
				if( searchItem.originalSentenceNr() > startSentenceNr )
					{
					if( searchItem.decrementOriginalSentenceNr() != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to decrement the original sentence number of an item" );
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I found an item with an original sentence number equal to the given start sentence number " + startSentenceNr );
				}

			if( searchItem.creationSentenceNr() >= startSentenceNr )
				{
				if( searchItem.creationSentenceNr() > startSentenceNr )
					{
					if( searchItem.decrementCreationSentenceNr() != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to decrement the creation sentence number of an item" );
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I found an item with a creation sentence number equal to the given start sentence number " + startSentenceNr );
				}

			if( searchItem.archivedSentenceNr() >= startSentenceNr )
				{
				if( searchItem.archivedSentenceNr() > startSentenceNr )
					{
					if( searchItem.decrementArchivedSentenceNr() != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to decrement the archived sentence number of an item" );
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I found an item with an archived sentence number equal to the given start sentence number " + startSentenceNr );
				}

			if( searchItem.replacedSentenceNr() >= startSentenceNr )
				{
				if( searchItem.replacedSentenceNr() > startSentenceNr )
					{
					if( searchItem.decrementReplacedSentenceNr() != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to decrement the replaced sentence number of an item" );
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I found an item with a replaced sentence number equal to the given start sentence number " + startSentenceNr );
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
				if( searchItem.itemNr() > startDecrementItemNr )
					{
					if( searchItem.decrementItemNr( decrementOffset ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to decrement the item number of an item with a certain offset" );
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I found an item number equal to the given start item number" );
				}

			searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected ListCleanup( List myList )
		{
		myList_ = myList;
		moduleNameString_ = this.getClass().getName();

		if( myList_ == null )
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given my list is undefined.\n" );
		}


	// Protected methods

	protected void setCurrentItemNr()
		{
		setCurrentItemNr( myList_.firstActiveItem() );
		setCurrentItemNr( myList_.firstInactiveItem() );
		setCurrentItemNr( myList_.firstArchivedItem() );
		setCurrentItemNr( myList_.firstReplacedItem() );
		setCurrentItemNr( myList_.firstDeletedItem() );
		}

	protected void deleteRollbackInfo()
		{
		Item searchItem = myList_.firstDeletedItem();

		while( searchItem != null )
			{
			if( searchItem.isAvailableForRollbackAfterDelete &&
			searchItem.deletedSentenceNr() >= CommonVariables.currentSentenceNr )
				searchItem.isAvailableForRollbackAfterDelete = false;

			searchItem = searchItem.nextItem;
			}
		}

	protected int highestInUseSentenceNrInList( boolean isIncludingDeletedItems, int highestSentenceNr )
		{
		int tempSentenceNr;
		int highestInUseSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_.firstActiveItem() );

		if( ( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_.firstInactiveItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_.firstArchivedItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( ( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_.firstReplacedItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		if( isIncludingDeletedItems &&
		( tempSentenceNr = highestInUseSentenceNrInList( highestSentenceNr, myList_.firstDeletedItem() ) ) > highestInUseSentenceNr )
			highestInUseSentenceNr = tempSentenceNr;

		return highestInUseSentenceNr;
		}

	protected byte decrementSentenceNrs( int startSentenceNr )
		{
		if( startSentenceNr > Constants.NO_SENTENCE_NR )
			{
			if( decrementSentenceNrs( startSentenceNr, myList_.firstActiveItem() ) == Constants.RESULT_OK )
				{
				if( decrementSentenceNrs( startSentenceNr, myList_.firstInactiveItem() ) == Constants.RESULT_OK )
					{
					if( decrementSentenceNrs( startSentenceNr, myList_.firstArchivedItem() ) == Constants.RESULT_OK )
						{
						if( decrementSentenceNrs( startSentenceNr, myList_.firstReplacedItem() ) == Constants.RESULT_OK )
							return decrementSentenceNrs( startSentenceNr, myList_.firstDeletedItem() );
						}
					}
				}
			}
		else
			return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given start sentence number is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		if( decrementSentenceNr >= Constants.NO_SENTENCE_NR &&
		decrementSentenceNr < Constants.MAX_SENTENCE_NR )
			{
			if( startDecrementItemNr > Constants.NO_ITEM_NR )
				{
				if( decrementOffset > 0 )
					{
					if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_.firstActiveItem() ) == Constants.RESULT_OK )
						{
						if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_.firstInactiveItem() ) == Constants.RESULT_OK )
							{
							if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_.firstArchivedItem() ) == Constants.RESULT_OK )
								{
								if( decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_.firstReplacedItem() ) == Constants.RESULT_OK )
									return decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset, myList_.firstDeletedItem() );
								}
							}
						}
					}
				else
					return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given decrement offset is undefined" );
				}
			else
				return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given start item number is undefined" );
			}
		else
			return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given decrement sentence number is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte deleteSentences( boolean isAvailableForRollback, int lowestSentenceNr )
		{
		Item searchItem = myList_.firstActiveItem();

		if( lowestSentenceNr > Constants.NO_SENTENCE_NR )
			{
			while( searchItem != null )
				{
				if( searchItem.activeSentenceNr() >= lowestSentenceNr )
					{
					if( searchItem.creationSentenceNr() >= lowestSentenceNr )
						{
						if( myList_.deleteItem( isAvailableForRollback, searchItem ) == Constants.RESULT_OK )
							searchItem = myList_.nextListItem();
						else
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to delete an active item" );
						}
					else
						{
						if( searchItem.hasInactiveSentenceNr() )
							{
							if( myList_.inactivateItem( false, searchItem ) == Constants.RESULT_OK )
								searchItem = myList_.nextListItem();
							else
								return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate an active item" );
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
						if( myList_.deleteItem( isAvailableForRollback, searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to archive an inactive item" );
						}
					else
						{
						if( myList_.activateItem( false, searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate an inactive item" );
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
						if( myList_.deleteItem( isAvailableForRollback, searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to delete an archived item" );
						}
					else
						{
						searchItem.clearArchivedSentenceNr();

						if( searchItem.wasActiveBefore() )
							{
							if( myList_.activateItem( true, searchItem ) != Constants.RESULT_OK )
								return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate an archived item" );
							}
						else
							{
							if( myList_.inactivateItem( true, searchItem ) != Constants.RESULT_OK )
								return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate an archived item" );
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
						if( myList_.deleteItem( isAvailableForRollback, searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to delete a replaced item" );
						}
					else
						{
						searchItem.clearReplacedSentenceNr();

						if( searchItem.wasActiveBefore() )
							{
							if( myList_.activateItem( true, searchItem ) != Constants.RESULT_OK )
								return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate a replaced item" );
							}
						else
							{
							if( searchItem.wasInactiveBefore() )
								{
								if( myList_.inactivateItem( true, searchItem ) != Constants.RESULT_OK )
									return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate a replaced item" );
								}
							else
								{
								if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
									return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to archive a replaced item" );
								}
							}
						}

					searchItem = myList_.nextListItem();
					}
				else
					searchItem = searchItem.nextItem;
				}
			}
		else
			return myList_.startError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "The given lowest sentence number is undefined" );

		return Constants.RESULT_OK;
		}

	protected byte rollbackDeletedRedoInfo()
		{
		Item searchItem = myList_.firstDeletedItem();

		while( searchItem != null )
			{
			if( searchItem.isAvailableForRollbackAfterDelete &&
			searchItem.hasCurrentDeletedSentenceNr() )
				{
				searchItem.isAvailableForRollbackAfterDelete = false;

				if( myList_.removeItemFromList( searchItem ) == Constants.RESULT_OK )
					{
					if( searchItem.hasInactiveSentenceNr() )
						{
						if( searchItem.hasArchivedSentenceNr() )
							{
							if( myList_.addItemToList( Constants.QUERY_ARCHIVED_CHAR, searchItem ) == Constants.RESULT_OK )
								searchItem = myList_.nextListItem();
							else
								return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to add an item to the archive list" );
							}
						else
							{
							if( myList_.addItemToList( Constants.QUERY_INACTIVE_CHAR, searchItem ) == Constants.RESULT_OK )
								searchItem = myList_.nextListItem();
							else
								return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to add an item to the inactive list" );
							}
						}
					else
						{
						if( myList_.addItemToList( Constants.QUERY_ACTIVE_CHAR, searchItem ) == Constants.RESULT_OK )
							searchItem = myList_.nextListItem();
						else
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to add an item to the active list" );
						}
					}
				else
					return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to remove an item from the deleted list" );
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	protected byte undoCurrentSentence()
		{
		boolean isAssignment = myList_.isAssignmentList();
		Item searchItem = myList_.firstActiveItem();

		while( searchItem != null )
			{
			if( searchItem.activeSentenceNr() >= CommonVariables.currentSentenceNr )
				{
				if( searchItem.hasInactiveSentenceNr() &&
				searchItem.hasCurrentActiveSentenceNr() &&
				searchItem.inactiveSentenceNr() < searchItem.activeSentenceNr() )
					{
					if( myList_.inactivateItem( false, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate an active item" );
					}
				else
					{
					if( isAssignment )
						{
						if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to archive an active item" );
						}
					else
						{
						if( myList_.replaceItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to replace an active item" );
						}
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstInactiveItem();

		while( searchItem != null )
			{
			if( searchItem.inactiveSentenceNr() >= CommonVariables.currentSentenceNr )
				{
				if( searchItem.hasActiveSentenceNr() &&
				searchItem.hasCurrentInactiveSentenceNr() &&
				searchItem.activeSentenceNr() < searchItem.inactiveSentenceNr() )
					{
					if( myList_.activateItem( false, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate an inactive item" );
					}
				else
					{
					if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to archive an inactive item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstArchivedItem();

		while( searchItem != null )
			{
			if( ( searchItem.archivedSentenceNr() == CommonVariables.currentSentenceNr &&
			searchItem.creationSentenceNr() < CommonVariables.currentSentenceNr ) ||

			( searchItem.archivedSentenceNr() + 1 == CommonVariables.currentSentenceNr &&
			searchItem.creationSentenceNr() + 1 == CommonVariables.currentSentenceNr ) )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( myList_.activateItem( true, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate an archived item" );
					}
				else
					{
					if( myList_.inactivateItem( true, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate an archived item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstReplacedItem();

		while( searchItem != null )
			{
			if( ( searchItem.replacedSentenceNr() == CommonVariables.currentSentenceNr &&
			searchItem.creationSentenceNr() < CommonVariables.currentSentenceNr ) ||

			( searchItem.replacedSentenceNr() + 1 == CommonVariables.currentSentenceNr &&
			searchItem.creationSentenceNr() + 1 == CommonVariables.currentSentenceNr ) )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( myList_.activateItem( true, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate a replaced item" );
					}
				else
					{
					if( searchItem.wasInactiveBefore() )
						{
						if( myList_.inactivateItem( true, searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate a replaced item" );
						}
					else
						{
						if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to archive a replaced item" );
						}
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		return Constants.RESULT_OK;
		}

	protected byte redoCurrentSentence()
		{
		Item searchItem = myList_.firstActiveItem();

		while( searchItem != null )
			{
			if( ( searchItem.hasCurrentInactiveSentenceNr() ||
			searchItem.hasCurrentArchivedSentenceNr() ||
			searchItem.hasCurrentReplacedSentenceNr() ) &&

			searchItem.activeSentenceNr() < CommonVariables.currentSentenceNr )
				{
				if( searchItem.inactiveSentenceNr() > searchItem.archivedSentenceNr() )
					{
					if( myList_.inactivateItem( false, searchItem ) == Constants.RESULT_OK )
						searchItem = myList_.nextListItem();
					else
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate an active item" );
					}
				else
					{
					if( searchItem.hasCurrentArchivedSentenceNr() )
						{
						if( myList_.archiveItem( searchItem ) == Constants.RESULT_OK )
							searchItem = myList_.nextListItem();
						else
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to archive an active item" );
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
			if( ( searchItem.hasCurrentActiveSentenceNr() ||
			searchItem.hasCurrentArchivedSentenceNr() ||
			searchItem.hasCurrentReplacedSentenceNr() ) &&

			searchItem.inactiveSentenceNr() < CommonVariables.currentSentenceNr )
				{
				if( searchItem.inactiveSentenceNr() > searchItem.archivedSentenceNr() )
					{
					if( myList_.activateItem( false, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate an inactive item" );
					}
				else
					{
					if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to archive an inactive item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstArchivedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( myList_.activateItem( true, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate an archived item" );
					}
				else
					{
					if( myList_.inactivateItem( true, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate an archived item" );
					}

				searchItem = myList_.nextListItem();
				}
			else
				searchItem = searchItem.nextItem;
			}

		searchItem = myList_.firstReplacedItem();

		while( searchItem != null )
			{
			if( searchItem.hasCurrentCreationSentenceNr() )
				{
				if( searchItem.wasActiveBefore() )
					{
					if( myList_.activateItem( true, searchItem ) != Constants.RESULT_OK )
						return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to activate a replaced item" );
					}
				else
					{
					if( searchItem.wasInactiveBefore() )
						{
						if( myList_.inactivateItem( true, searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate a replaced item" );
						}
					else
						{
						if( myList_.archiveItem( searchItem ) != Constants.RESULT_OK )
							return myList_.addError( 1, moduleNameString_, myList_.myWordItem().anyWordTypeString(), "I failed to inactivate a replaced item" );
						}
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
 *
 *	"The winds blows, and we are gone -
 *	as though we had never been here.
 *	But the love of the Lord remains forever
 *	with those who fear him." (Psalm 103:16-17)
 *
 *************************************************************************/
