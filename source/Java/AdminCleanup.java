/*
 *	Class:			AdminCleanup
 *	Supports class:	AdminItem
 *	Purpose:		To cleanup obsolete items
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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
	// Private constructible variables

	private boolean hasFoundAnyChangeMadeByThisSentence_;
	private boolean wasUndoOrRedo_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private void decrementCurrentSentenceNr()
		{
		int firstSentenceNrOfCurrentUser = adminItem_.firstSentenceNrOfCurrentUser();

		if( CommonVariables.currentSentenceNr >= firstSentenceNrOfCurrentUser )
			{
			getHighestInUseSentenceNr( false, false, --CommonVariables.currentSentenceNr );

			if( CommonVariables.highestInUseSentenceNr < CommonVariables.currentSentenceNr )
				CommonVariables.currentSentenceNr = ( CommonVariables.highestInUseSentenceNr > firstSentenceNrOfCurrentUser ? CommonVariables.highestInUseSentenceNr : firstSentenceNrOfCurrentUser );

			// Necessary after changing current sentence number
			setCurrentItemNr();

			if( CommonVariables.currentItemNr == Constants.NO_ITEM_NR )
				{
				if( CommonVariables.currentSentenceNr > Constants.NO_SENTENCE_NR )
					{
					CommonVariables.currentSentenceNr--;
					// Necessary after changing current sentence number
					setCurrentItemNr();
					}
				}
			}
		else
			CommonVariables.isDontIncrementCurrentSentenceNr = true;
		}

	private static void deleteWriteListsInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.deleteTemporaryWriteList();
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		}

	private void getHighestInUseSentenceNr( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		short adminListNr = 0;
		WordList wordList;

		CommonVariables.highestInUseSentenceNr = Constants.NO_SENTENCE_NR;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, highestSentenceNr );

		// Admin lists
		while( CommonVariables.highestInUseSentenceNr < highestSentenceNr &&
		adminListNr < Constants.NUMBER_OF_ADMIN_LISTS )
			{
			if( adminItem_.adminListArray[adminListNr] != null &&

			( isIncludingTemporaryLists ||
			!adminItem_.adminListArray[adminListNr].isTemporaryList() ) )
				adminItem_.adminListArray[adminListNr].getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			adminListNr++;
			}
		}

	private int highestSentenceNr()
		{
		int tempSentenceNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			highestSentenceNr = wordList.highestSentenceNrInWordList();

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null &&
			( tempSentenceNr = adminItem_.adminListArray[adminListNr].highestSentenceNrInList() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;
			}

		return highestSentenceNr;
		}

	private byte decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		WordList wordList;

		if( CommonVariables.currentSentenceNr == decrementSentenceNr &&
		CommonVariables.currentItemNr > startDecrementItemNr )
			CommonVariables.currentItemNr -= decrementOffset;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to decrement item number range in my word list" );
			}

		for( short adminListNr : Constants.AdminLists )
			{
			// Admin lists
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte decrementSentenceNrs( int startSentenceNr )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.decrementSentenceNrsInWordList( startSentenceNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in my word list" );
			}

		for( short adminListNr : Constants.AdminLists )
			{
			// Admin lists
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].decrementSentenceNrsInList( startSentenceNr ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte deleteUnusedWordTypes( boolean isInactiveItems, boolean isDeletingAllActiveWordTypes )
		{
		ReadResultType readResult;
		short nReadWordReferences;
		String pluralNounString;
		WordTypeItem unusedWordTypeItem;
		WordTypeItem singularNounWordTypeItem;
		ReadItem previousReadItem = null;
		ReadItem unusedReadItem = ( isInactiveItems ? adminItem_.firstInactiveReadItem() : adminItem_.firstActiveReadItem() );
		WordItem unusedReadWordItem;
		ReadList readList;

		if( ( readList = adminItem_.readList ) != null )
			{
			while( unusedReadItem != null )
				{
				if( isInactiveItems ||
				isDeletingAllActiveWordTypes ||

				( previousReadItem != null &&
				!unusedReadItem.isSingularNoun() &&
				// More word types for this word number
				previousReadItem.wordOrderNr() == unusedReadItem.wordOrderNr() ) )
					{
					// Skip text
					if( ( unusedReadWordItem = unusedReadItem.readWordItem() ) != null )
						{
						if( ( unusedWordTypeItem = unusedReadItem.activeReadWordTypeItem() ) != null )
							{
							if( unusedWordTypeItem.hasCurrentCreationSentenceNr() )
								{
								if( !isDeletingAllActiveWordTypes &&
								// Wrong assumption: This noun isn't plural but singular
								unusedReadItem.isPluralNoun() &&
								unusedReadWordItem.isUserDefinedWord() &&
								( pluralNounString = unusedWordTypeItem.itemString() ) != null &&
								( singularNounWordTypeItem = unusedReadWordItem.activeWordTypeItem( Constants.WORD_TYPE_NOUN_SINGULAR ) ) != null )
									{
									if( singularNounWordTypeItem.createNewWordTypeString( pluralNounString ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to create a new word string for a singular noun word type of an active read word" );
									}

								if( ( readResult = readList.getNumberOfReadWordReferences( unusedReadItem.wordTypeNr(), unusedReadWordItem ) ).result == Constants.RESULT_OK )
									{
									if( ( nReadWordReferences = readResult.nReadWordReferences ) >= 1 )
										{
										unusedReadItem.isUnusedReadItem = true;

										if( nReadWordReferences == 1 )
											{
											if( unusedReadWordItem.deleteWordType( unusedReadItem.wordTypeNr() ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to delete an unused word type item" );
											}
										}
									else
										return adminItem_.startError( 1, moduleNameString_, "I have found an invalid number of read word references" );
									}
								else
									return adminItem_.addError( 1, moduleNameString_, "I failed to get the number of read word references" );
								}
							}
						else
							return adminItem_.startError( 1, moduleNameString_, "I couldn't find the word type of an active read word" );
						}
					}

				previousReadItem = unusedReadItem;
				unusedReadItem = unusedReadItem.nextReadItem();
				}
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The read list isn't created yet" );

		return Constants.RESULT_OK;
		}

	private byte deleteUnusedWordTypes( boolean isDeletingAllActiveWordTypes )
		{
		// From active read items
		if( deleteUnusedWordTypes( false, isDeletingAllActiveWordTypes ) == Constants.RESULT_OK )
			{
			// From inactive read items
			if( deleteUnusedWordTypes( true, isDeletingAllActiveWordTypes ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the active unused word types" );
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete the inactive unused word types" );

		return Constants.RESULT_OK;
		}

	private byte redoCurrentSentence()
		{
		// In words
		if( redoCurrentSentenceInAllWords() == Constants.RESULT_OK )
			{
			// Admin lists
			for( short adminListNr : Constants.AdminLists )
				{
				if( adminItem_.adminListArray[adminListNr] != null )
					{
					if( adminItem_.adminListArray[adminListNr].redoCurrentSentenceInList() != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to redo the current sentence" );
					}
				}
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to redo the current sentence in my word list" );

		return Constants.RESULT_OK;
		}

	private byte redoCurrentSentenceInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.redoCurrentSentence();
			while( CommonVariables.result == Constants.RESULT_OK &&
			( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The first word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte removeFirstRangeOfDeletedItems()
		{
		short adminListNr = 0;
		WordList wordList;

		CommonVariables.nDeletedItems = 0;
		CommonVariables.removeSentenceNr = Constants.NO_SENTENCE_NR;
		CommonVariables.removeStartItemNr = Constants.NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.removeFirstRangeOfDeletedItemsInWordList() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to remove the first deleted items in my word list" );
			}

		// Admin lists
		while( adminListNr < Constants.NUMBER_OF_ADMIN_LISTS &&
		CommonVariables.nDeletedItems == 0 )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].removeFirstRangeOfDeletedItemsInList() != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to remove the first deleted items" );
				}

			adminListNr++;
			}

		return Constants.RESULT_OK;
		}

	private byte rollbackDeletedRedoInfo()
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.rollbackDeletedRedoInfoInWordList() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the current redo info of my words in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				adminItem_.adminListArray[adminListNr].rollbackDeletedRedoInfoInList();
			}

		return Constants.RESULT_OK;
		}

	private byte undoCurrentSentence()
		{
		// In words
		if( undoCurrentSentenceInAllWords() == Constants.RESULT_OK )
			{
			// Admin lists
			for( short adminListNr : Constants.AdminLists )
				{
				if( adminItem_.adminListArray[adminListNr] != null )
					{
					if( adminItem_.adminListArray[adminListNr].undoCurrentSentenceInList() != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to undo the current sentence" );
					}
				}
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to undo the current sentence in my word list" );

		return Constants.RESULT_OK;
		}

	private byte undoCurrentSentenceInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.undoCurrentSentence();
			while( CommonVariables.result == Constants.RESULT_OK &&
			( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		else
			return adminItem_.startError( 1, moduleNameString_, "The first word item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminCleanup( AdminItem adminItem )
		{
		String errorString = null;

		hasFoundAnyChangeMadeByThisSentence_ = false;
		wasUndoOrRedo_ = false;

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
		hasFoundAnyChangeMadeByThisSentence_ = true;

		getHighestInUseSentenceNr( false, false, CommonVariables.currentSentenceNr );

		if( CommonVariables.highestInUseSentenceNr < CommonVariables.currentSentenceNr )
			hasFoundAnyChangeMadeByThisSentence_ = false;
		}

	protected void deleteRollbackInfo()
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.deleteRollbackInfoInWordList();

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				adminItem_.adminListArray[adminListNr].deleteRollbackInfoInList();
			}
		}

	protected void setCurrentItemNr()
		{
		WordList wordList;

		CommonVariables.currentItemNr = Constants.NO_ITEM_NR;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			wordList.setCurrentItemNrInWordList();

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				adminItem_.adminListArray[adminListNr].setCurrentItemNrInList();
			}
		}

	protected boolean hasFoundAnyChangeMadeByThisSentence()
		{
		return hasFoundAnyChangeMadeByThisSentence_;
		}

	protected boolean wasUndoOrRedo()
		{
		return wasUndoOrRedo_;
		}

	protected byte cleanupDeletedItems()
		{
		int previousRemoveSentenceNr = Constants.NO_SENTENCE_NR;
/*
		if( !adminItem_.isSystemStartingUp() )
			Presentation.showStatus( Constants.INTERFACE_CONSOLE_I_AM_CLEANING_UP_DELETED_ITEMS );
*/
		do	{
			if( removeFirstRangeOfDeletedItems() == Constants.RESULT_OK )
				{
				if( previousRemoveSentenceNr > Constants.NO_SENTENCE_NR &&
				// Previous deleted sentence is may be empty
				previousRemoveSentenceNr != CommonVariables.removeSentenceNr )
					{
					getHighestInUseSentenceNr( true, true, previousRemoveSentenceNr );

					// All items of this sentence are deleted
					if( CommonVariables.highestInUseSentenceNr < previousRemoveSentenceNr )
						{
						// So, decrement all higher sentence numbers
						if( decrementSentenceNrs( previousRemoveSentenceNr ) == Constants.RESULT_OK )
							{
							if( CommonVariables.removeSentenceNr > previousRemoveSentenceNr )
								CommonVariables.removeSentenceNr--;

							if( CommonVariables.currentSentenceNr >= previousRemoveSentenceNr )
								decrementCurrentSentenceNr();
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number" );
						}
					}

				if( CommonVariables.nDeletedItems > 0 )
					{
					if( decrementItemNrRange( CommonVariables.removeSentenceNr, CommonVariables.removeStartItemNr, CommonVariables.nDeletedItems ) == Constants.RESULT_OK )
						previousRemoveSentenceNr = CommonVariables.removeSentenceNr;
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to decrement item number range" );
					}
				}
			else
				return adminItem_.addError( 1, moduleNameString_, "I failed to remove the first deleted items" );
			}
		while( CommonVariables.nDeletedItems > 0 );

//		Presentation.clearStatus();

		return Constants.RESULT_OK;
		}

	protected byte deleteAllTemporaryLists()
		{
		wasUndoOrRedo_ = false;

		// Read list is a temporary list
		adminItem_.deleteTemporaryReadList();
		// Score list is a temporary list
		adminItem_.deleteTemporaryScoreList();
		// Response lists are temporary lists
		deleteWriteListsInAllWords();

		if( cleanupDeletedItems() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to cleanup the deleted items" );

		return Constants.RESULT_OK;
		}

	protected byte deleteUnusedInterpretations( boolean isDeletingAllActiveWordTypes )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();
		short adminListNr;
		short previousWordOrderNr = Constants.NO_ORDER_NR;
		WordItem unusedWordItem;
		ReadItem unusedReadItem = adminItem_.firstActiveReadItem();
		ReadList readList;
		WordList wordList;

		if( ( readList = adminItem_.readList ) != null )
			{
			if( ( wordList = adminItem_.wordList ) != null )
				{
				if( deleteUnusedWordTypes( isDeletingAllActiveWordTypes ) == Constants.RESULT_OK )
					{
					// Active read items
					while( unusedReadItem != null )
						{
						if( unusedReadItem.isUnusedReadItem ||
						unusedReadItem.wordOrderNr() == previousWordOrderNr )
							{
							previousWordOrderNr = unusedReadItem.wordOrderNr();
							unusedWordItem = unusedReadItem.readWordItem();

							if( readList.deleteItem( false, unusedReadItem ) == Constants.RESULT_OK )
								{
								// Skip text
								if( unusedWordItem != null &&
								!unusedWordItem.hasItems() &&
								!unusedWordItem.isDeletedItem() &&
								unusedWordItem.hasCurrentCreationSentenceNr() )
									{
									adminListNr = 0;
									referenceResult.hasFoundWordReference = false;

									while( !referenceResult.hasFoundWordReference &&
									adminListNr < Constants.NUMBER_OF_ADMIN_LISTS )
										{
										if( adminItem_.adminListArray[adminListNr] != null )
											{
											// Check the selection lists for a reference to this word
											if( ( referenceResult = adminItem_.adminListArray[adminListNr].findWordReference( unusedWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to find a reference to an active word \"" + unusedWordItem.anyWordTypeString() + "\" in one of my lists" );
											}

										adminListNr++;
										}

									if( !referenceResult.hasFoundWordReference )
										{
										if( wordList.deleteItem( false, unusedWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to delete a word item" );
										}
									}

								unusedReadItem = readList.nextReadListItem();
								}
							else
								return adminItem_.addError( 1, moduleNameString_, "I failed to delete an active read item" );
							}
						else
							{
							previousWordOrderNr = unusedReadItem.wordOrderNr();
							unusedReadItem = unusedReadItem.nextReadItem();
							}
						}

					// Inactive read items
					while( ( unusedReadItem = adminItem_.firstInactiveReadItem() ) != null )
						{
						if( readList.deleteItem( false, unusedReadItem ) == Constants.RESULT_OK )
							{
							unusedWordItem = unusedReadItem.readWordItem();

							if( unusedWordItem != null &&
							!unusedWordItem.hasItems() &&
							!unusedWordItem.isDeletedItem() &&
							unusedWordItem.hasCurrentCreationSentenceNr() )
								{
								adminListNr = 0;
								referenceResult.hasFoundWordReference = false;

								while( !referenceResult.hasFoundWordReference &&
								adminListNr < Constants.NUMBER_OF_ADMIN_LISTS )
									{
									if( adminItem_.adminListArray[adminListNr] != null )
										{
										// Check my lists for a reference to this word
										if( ( referenceResult = adminItem_.adminListArray[adminListNr].findWordReference( unusedWordItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to find a reference to an inactive word \"" + unusedWordItem.anyWordTypeString() + "\" in one of my lists" );
										}

									adminListNr++;
									}

								if( !referenceResult.hasFoundWordReference )
									{
									if( wordList.deleteItem( false, unusedWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to delete a word item" );
									}
								}
							}
						else
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete an unused (inactive) read word" );
						}
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to delete the unused word types" );
				}
			else
				return adminItem_.startError( 1, moduleNameString_, "The word list isn't created yet" );
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteSentences( boolean isAvailableForRollback, int lowestSentenceNr )
		{
		WordList wordList;

		// In words
		if( ( wordList = adminItem_.wordList ) != null )
			{
			if( wordList.deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete sentences in my word list" );
			}

		// Admin lists
		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )
				{
				if( adminItem_.adminListArray[adminListNr].deleteSentencesInList( isAvailableForRollback, lowestSentenceNr ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( adminListNr, 1, moduleNameString_, "I failed to delete sentences in a list" );
				}
			}

		if( cleanupDeletedItems() != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to cleanup the deleted items" );

		return Constants.RESULT_OK;
		}

	protected byte undoLastSentence()
		{
		int previousSentenceNr = CommonVariables.currentSentenceNr;

		// Remove the deleted read items of this undo sentence
		if( deleteAllTemporaryLists() == Constants.RESULT_OK )
			{
			// No sentences found to undo
			if( CommonVariables.currentSentenceNr <= adminItem_.firstSentenceNrOfCurrentUser() )
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_UNDO ) == Constants.RESULT_OK )
					{
					if( previousSentenceNr == CommonVariables.currentSentenceNr )
						CommonVariables.isDontIncrementCurrentSentenceNr = true;
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'no sentences to undo' interface notification" );
				}
			else
				{
				if( previousSentenceNr > Constants.NO_SENTENCE_NR &&
				previousSentenceNr == CommonVariables.currentSentenceNr )
					{
					CommonVariables.currentSentenceNr--;
					// Necessary after changing current sentence number
					setCurrentItemNr();
					}

				if( undoCurrentSentence() == Constants.RESULT_OK )
					{
					if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, CommonVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) == Constants.RESULT_OK )
						{
						wasUndoOrRedo_ = true;
						CommonVariables.isDontIncrementCurrentSentenceNr = true;
						}
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I have undone' interface notification" );
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to undo the current sentence" );
				}
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete all temporary lists" );

		return Constants.RESULT_OK;
		}

	protected byte redoLastUndoneSentence()
		{
		// Remove the deleted read items of this redo sentence
		if( deleteAllTemporaryLists() == Constants.RESULT_OK )
			{
			if( CommonVariables.currentSentenceNr > highestSentenceNr() )
				{
				// Roll-back deleted sentence. Process this sentence as a command, not as a sentence
				rollbackDeletedRedoInfo();

				if( redoCurrentSentence() == Constants.RESULT_OK )
					{
					if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, CommonVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) == Constants.RESULT_OK )
						wasUndoOrRedo_ = true;
					else
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I have redone' interface notification" );
					}
				else
					return adminItem_.addError( 1, moduleNameString_, "I failed to redo the current sentence" );
				}
			else
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'no sentences to redo' interface notification" );
				}
			}
		else
			return adminItem_.addError( 1, moduleNameString_, "I failed to delete all temporary lists" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"How great is the Lord,
 *	how deserving of praise,
 *	in the city of our God,
 *	which sits on his holy mountain!" (Psalm 48:1)
 *************************************************************************/
