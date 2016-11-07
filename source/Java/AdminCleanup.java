/*	Class:			AdminCleanup
 *	Supports class:	AdminItem
 *	Purpose:		To cleanup obsolete items
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

class AdminCleanup
	{
	// Private constructed variables

	private boolean hasAnyChangeBeenMadeByThisSentence_;
	private boolean wasUndoOrRedoCommand_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private void decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		List currentAdminList;
		WordList wordList;

		if( CommonVariables.currentSentenceNr == decrementSentenceNr &&
		CommonVariables.currentItemNr > startDecrementItemNr )
			CommonVariables.currentItemNr -= decrementOffset;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				currentAdminList.decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset );
			}
		}

	private void decrementSentenceNrs( int startSentenceNr )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.decrementSentenceNrsInWordList( startSentenceNr );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				currentAdminList.decrementSentenceNrsInList( startSentenceNr );
			}
		}

	private static void rebuildQuickAccessListss()
		{
		WordItem currentWordItem;

		CommonVariables.firstAssignmentWordItem = null;
		CommonVariables.firstContextWordItem = null;
		CommonVariables.firstCollectionWordItem = null;
		CommonVariables.firstSpecificationWordItem = null;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.rebuildQuickAccessLists();
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		}

	private void removeFirstRangeOfDeletedItems()
		{
		short adminListNr = 0;
		List currentAdminList;
		WordList wordList;

		CommonVariables.nDeletedItems = 0;
		CommonVariables.removeSentenceNr = Constants.NO_SENTENCE_NR;
		CommonVariables.removeStartItemNr = Constants.NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.removeFirstRangeOfDeletedItemsInWordList();

		// Admin lists
		while( adminListNr < Constants.NUMBER_OF_ADMIN_LISTS &&
		CommonVariables.nDeletedItems == 0 )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				currentAdminList.removeFirstRangeOfDeletedItemsInList();

			adminListNr++;
			}
		}

	private int getHighestInUseSentenceNr( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		short adminListNr = 0;
		List currentAdminList;
		WordList wordList;

		CommonVariables.highestInUseSentenceNr = Constants.NO_SENTENCE_NR;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );

		// Admin lists
		while( adminListNr < Constants.NUMBER_OF_ADMIN_LISTS &&
		highestSentenceNr > CommonVariables.highestInUseSentenceNr )
			{
			currentAdminList = adminItem_.adminListArray[adminListNr];

			if( currentAdminList != null &&

			( isIncludingTemporaryLists ||
			!currentAdminList.isTemporaryList() ) )
				currentAdminList.getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			adminListNr++;
			}

		return CommonVariables.highestInUseSentenceNr;
		}

	private byte deleteAllWordTypesOfCurrentSentence( boolean isActiveItems )
		{
		ReadItem currentReadItem = ( isActiveItems ? adminItem_.firstActiveReadItem() : adminItem_.firstInactiveReadItem() );
		WordItem currentWordItem;
		WordList wordList;

		if( ( wordList = adminItem_.wordList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The word list isn't created yet" );

		while( currentReadItem != null )
			{
			currentWordItem = currentReadItem.readWordItem();

			// Skip text
			if( currentWordItem != null &&
			!currentWordItem.isDeletedItem() )
				{
				if( currentWordItem.deleteAllWordTypesOfCurrentSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete all word types of the current sentence in word \"" + currentWordItem.anyWordTypeString() + "\"" );

				if( !currentWordItem.hasAnyWordType() )
					{
					if( wordList.deleteItem( currentWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to delete word \"" + currentWordItem.anyWordTypeString() + "\"" );
					}
				}

			currentReadItem = currentReadItem.nextReadItem();
			}

		return Constants.RESULT_OK;
		}

	private byte deleteUnusedWordsAndWordTypes( boolean isActiveItems )
		{
		ReadResultType readResult;
		boolean isSameWordOrderNr;
		short nReadWordReferences;
		short previousWordOrderNr = Constants.NO_ORDER_NR;
		WordTypeItem currentWordTypeItem;
		ReadItem currentReadItem = ( isActiveItems ? adminItem_.firstActiveReadItem() : adminItem_.firstInactiveReadItem() );
		ReadList readList;
		WordItem currentReadWordItem;
		WordList wordList;

		if( ( readList = adminItem_.readList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The read list isn't created yet" );

		if( ( wordList = adminItem_.wordList ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The word list isn't created yet" );

		while( currentReadItem != null )
			{
			isSameWordOrderNr = ( currentReadItem.wordOrderNr() == previousWordOrderNr );
			previousWordOrderNr = currentReadItem.wordOrderNr();

			// Skip text
			if( ( currentReadWordItem = currentReadItem.readWordItem() ) != null )
				{
				if( !isActiveItems ||

				// More word types for this word number
				( isSameWordOrderNr &&
				!currentReadItem.isSingularNoun() ) )
					{
					if( ( currentWordTypeItem = currentReadItem.activeReadWordTypeItem() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't find the word type of an active read word" );

					if( currentWordTypeItem.hasCurrentCreationSentenceNr() )
						{
						if( ( readResult = readList.getNumberOfReadWordReferences( currentReadItem.wordTypeNr(), currentReadWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to get the number of read word references" );

						if( ( nReadWordReferences = readResult.nReadWordReferences ) < 1 )
							return adminItem_.startError( 1, moduleNameString_, "I have found an invalid number of read word references" );

						if( nReadWordReferences == 1 )
							{
							if( currentReadWordItem.deleteWordType( currentWordTypeItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to delete an unused word type item" );

							if( !currentReadWordItem.hasAnyWordType() &&
							!currentReadWordItem.isDeletedItem() &&
							currentReadWordItem.hasCurrentCreationSentenceNr() )
								{
								if( wordList.deleteItem( currentReadWordItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to delete an unused word item" );
								}
							}
						}
					}

				if( !isActiveItems ||
				isSameWordOrderNr )
					{
					if( readList.deleteItem( currentReadItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to delete an active read item" );

					currentReadItem = readList.nextReadListItem();
					}
				else
					currentReadItem = currentReadItem.nextReadItem();
				}
			else
				currentReadItem = currentReadItem.nextReadItem();
			}

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminCleanup( AdminItem adminItem )
		{
		String errorString = null;

		hasAnyChangeBeenMadeByThisSentence_ = false;
		wasUndoOrRedoCommand_ = false;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void checkForChangesMadeByThisSentence()
		{
		int currentSentenceNr = CommonVariables.currentSentenceNr;
		int highestInUseSentenceNr = getHighestInUseSentenceNr( false, false, currentSentenceNr );

		hasAnyChangeBeenMadeByThisSentence_ = ( highestInUseSentenceNr >= currentSentenceNr );
		}

	protected void cleanupDeletedItems()
		{
		int firstSentenceNrOfCurrentUser;
		int startRemoveSentenceNr = Constants.NO_SENTENCE_NR;

		if( ( CommonVariables.hasDisplayedWarning ||
		CommonVariables.result != Constants.RESULT_OK ) &&

		!adminItem_.hasClosedFileDueToError() )
			deleteSentences( CommonVariables.currentSentenceNr );

		do	{
			removeFirstRangeOfDeletedItems();

			if( CommonVariables.nDeletedItems > 0 )
				{
				decrementItemNrRange( CommonVariables.removeSentenceNr, CommonVariables.removeStartItemNr, CommonVariables.nDeletedItems );
				startRemoveSentenceNr = CommonVariables.removeSentenceNr;
				}
			}
		while( CommonVariables.nDeletedItems > 0 );

		if( CommonVariables.hasDisplayedWarning )
			CommonVariables.hasDisplayedWarning = false;
		else
			{
			if( startRemoveSentenceNr > Constants.NO_SENTENCE_NR &&
			// Previous deleted sentence might be empty
			startRemoveSentenceNr != CommonVariables.removeSentenceNr &&
			// All items of this sentence are deleted
			getHighestInUseSentenceNr( true, true, startRemoveSentenceNr ) < startRemoveSentenceNr )
				{
				// So, decrement all higher sentence numbers
				decrementSentenceNrs( startRemoveSentenceNr );

				if( CommonVariables.currentSentenceNr >= startRemoveSentenceNr )
					{
					firstSentenceNrOfCurrentUser = adminItem_.firstSentenceNrOfCurrentUser();

					// First user sentence
					if( startRemoveSentenceNr == firstSentenceNrOfCurrentUser )
						decrementCurrentSentenceNr();
					else
						{
						CommonVariables.currentSentenceNr = getHighestInUseSentenceNr( false, false, CommonVariables.currentSentenceNr );
						// Necessary after changing current sentence number
						setCurrentItemNr();
						}
					}
				}
			}
		}

	protected void clearTemporaryAdminLists()
		{
		wasUndoOrRedoCommand_ = false;

		adminItem_.deleteTemporaryReadList();
		adminItem_.deleteTemporaryScoreList();
		}

	protected void decrementCurrentSentenceNr()
		{
		if( CommonVariables.currentSentenceNr > Constants.NO_SENTENCE_NR )
			{
			CommonVariables.currentSentenceNr--;
			// Necessary after changing current sentence number
			setCurrentItemNr();
			}
		}

	protected void deleteSentences( int lowestSentenceNr )
		{
		List currentAdminList;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.deleteSentencesInWordList( lowestSentenceNr );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				currentAdminList.deleteSentencesInList( lowestSentenceNr );
			}

		rebuildQuickAccessListss();
		}

	protected void setCurrentItemNr()
		{
		List currentAdminList;
		WordList wordList;

		CommonVariables.currentItemNr = Constants.NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.setCurrentItemNrInWordList();

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
				currentAdminList.setCurrentItemNrInList();
			}
		}

	protected boolean hasAnyChangeBeenMadeByThisSentence()
		{
		return hasAnyChangeBeenMadeByThisSentence_;
		}

	protected boolean wasUndoOrRedoCommand()
		{
		return wasUndoOrRedoCommand_;
		}

	protected byte deleteAllWordTypesOfCurrentSentence()
		{
		if( deleteAllWordTypesOfCurrentSentence( true ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete all word types of the active read list" );

		if( deleteAllWordTypesOfCurrentSentence( false ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete all word types of the inactive read list" );

		return Constants.RESULT_OK;
		}

	protected byte deleteUnusedWordsAndWordTypes()
		{
		// Active read items
		if( deleteUnusedWordsAndWordTypes( true ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete unused word types in the active read list" );

		// Inactive read items
		if( deleteUnusedWordsAndWordTypes( false ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete unused word types in the inactive read list" );

		return Constants.RESULT_OK;
		}

	protected byte redoLastUndoneSentence()
		{
		List currentAdminList;
		WordItem currentWordItem;

		wasUndoOrRedoCommand_ = true;

		if( getHighestInUseSentenceNr( true, false, CommonVariables.currentSentenceNr ) == CommonVariables.currentSentenceNr )
			{
			// Important: Redo admin lists first, and the words after that.
			// Because redoing admin words list might redo words.
			for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
					{
					if( !currentAdminList.isTemporaryList() &&
					currentAdminList.redoCurrentSentenceInList() != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to redo the current sentence" );
					}
				}

			if( ( currentWordItem = CommonVariables.firstWordItem ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The first word item is undefined" );

			// Do for all words
			do	{
				if( currentWordItem.redoCurrentSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to redo the current sentence in word \"" + currentWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );

			if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, CommonVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I have redone' interface notification" );

			rebuildQuickAccessListss();
			}
		else
			{
			// No sentences found to redo
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'no sentences to redo' interface notification" );

			decrementCurrentSentenceNr();
			}

		return Constants.RESULT_OK;
		}

	protected byte undoLastSentence()
		{
		int firstSentenceNrOfCurrentUser = adminItem_.firstSentenceNrOfCurrentUser();
		List currentAdminList;
		WordItem currentWordItem;

		wasUndoOrRedoCommand_ = true;

		// Remove the deleted read items of this undo sentence
		if( CommonVariables.currentSentenceNr > firstSentenceNrOfCurrentUser )
			{
			decrementCurrentSentenceNr();

			if( ( currentWordItem = CommonVariables.firstWordItem ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The first word item is undefined" );

			// Do for all words
			do	{
				if( currentWordItem.undoCurrentSentence() != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to undo the current sentence in word \"" + currentWordItem.anyWordTypeString() + "\"" );
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );

			// Admin lists
			for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( ( currentAdminList = adminItem_.adminListArray[adminListNr] ) != null )
					{
					if( !currentAdminList.isTemporaryList() &&
					currentAdminList.undoCurrentSentenceInList() != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to undo the current sentence" );
					}
				}

			if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, CommonVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I have undone' interface notification" );

			rebuildQuickAccessListss();
			}
		else
			{
			// No sentences found to undo
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_UNDO ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'no sentences to undo' interface notification" );
			}

		if( CommonVariables.currentSentenceNr >= firstSentenceNrOfCurrentUser )
			decrementCurrentSentenceNr();

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"How great is the Lord,
 *	how deserving of praise,
 *	in the city of our God,
 *	which sits on his holy mountain!" (Psalm 48:1)
 *************************************************************************/
