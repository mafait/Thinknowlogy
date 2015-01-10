/*
 *	Class:			AdminCleanup
 *	Supports class:	AdminItem
 *	Purpose:		To cleanup unnecessary items
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

class AdminCleanup
	{
	// Private constructible variables

	private boolean isDontIncrementCurrentSentenceNr_;
	private boolean hasFoundAnyChangeDuringThisSentence_;
	private boolean wasUndoOrRedo_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

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

	private void getHighestInUseSentenceNr( boolean isIncludingDeletedItems, boolean isIncludingLanguageAssignments, boolean isIncludingTemporaryLists, int highestSentenceNr )
		{
		short adminListNr = 0;

		CommonVariables.highestInUseSentenceNr = Constants.NO_SENTENCE_NR;

		if( adminItem_.wordList != null )							// Inside words
			adminItem_.wordList.getHighestInUseSentenceNrInWordList( isIncludingDeletedItems, isIncludingLanguageAssignments, isIncludingTemporaryLists, highestSentenceNr );

		while( CommonVariables.highestInUseSentenceNr < highestSentenceNr &&
		adminListNr < Constants.NUMBER_OF_ADMIN_LISTS )
			{
			if( adminItem_.adminListArray[adminListNr] != null &&	// Inside admin lists

			( isIncludingTemporaryLists ||
			!adminItem_.adminListArray[adminListNr].isTemporaryList() ) )
				adminItem_.adminListArray[adminListNr].getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			adminListNr++;
			}
		}

	private byte decrementSentenceNrs( int startSentenceNr )
		{
		if( adminItem_.wordList != null )							// Inside words
			{
			if( adminItem_.wordList.decrementSentenceNrsInWordList( startSentenceNr ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in my word list" );
			}

		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
				{
				if( adminItem_.adminListArray[adminListNr].decrementSentenceNrsInList( startSentenceNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte decrementCurrentSentenceNr()
		{
		int firstUserSentenceNr = adminItem_.firstUserSentenceNr();

		if( CommonVariables.currentSentenceNr > Constants.NO_SENTENCE_NR )
			{
			if( CommonVariables.currentSentenceNr - 1 >= firstUserSentenceNr )		// Is my sentence
				{
				getHighestInUseSentenceNr( false, true, false, --CommonVariables.currentSentenceNr );

				if( CommonVariables.highestInUseSentenceNr < CommonVariables.currentSentenceNr )
					CommonVariables.currentSentenceNr = ( CommonVariables.highestInUseSentenceNr > firstUserSentenceNr ? CommonVariables.highestInUseSentenceNr : firstUserSentenceNr );

				// Necessary after decrement of current sentence number
				setCurrentItemNr();
				}
			else
				isDontIncrementCurrentSentenceNr_ = true;
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current sentence number is undefined" );

		return Constants.RESULT_OK;
		}

	private byte decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		if( CommonVariables.currentSentenceNr == decrementSentenceNr &&
		CommonVariables.currentItemNr > startDecrementItemNr )
			CommonVariables.currentItemNr -= decrementOffset;

		if( adminItem_.wordList != null )							// Inside words
			{
			if( adminItem_.wordList.decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to decrement item number range in my word list" );
			}

		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
				{
				if( adminItem_.adminListArray[adminListNr].decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte deleteUnusedWordTypes( boolean isInactiveItems, boolean isDeleteAllActiveWordTypes )
		{
		ReadResultType readResult;
		short nReadWordReferences;
		String pluralNounString;
		WordTypeItem unusedWordTypeItem;
		WordTypeItem singularNounWordTypeItem;
		ReadItem previousReadItem = null;
		ReadItem unusedReadItem = ( isInactiveItems ? adminItem_.firstInactiveReadItem() : adminItem_.firstActiveReadItem() );
		WordItem unusedReadWordItem;

		if( adminItem_.readList != null )
			{
			while( unusedReadItem != null )
				{
				if( isInactiveItems ||
				isDeleteAllActiveWordTypes ||

				( previousReadItem != null &&
				!unusedReadItem.isSingularNoun() &&
				previousReadItem.wordOrderNr() == unusedReadItem.wordOrderNr() ) )		// More word types for this word number
					{
					if( ( unusedReadWordItem = unusedReadItem.readWordItem() ) != null )	// Skip text
						{
						if( ( unusedWordTypeItem = unusedReadItem.activeReadWordTypeItem() ) != null )
							{
							if( unusedWordTypeItem.hasCurrentCreationSentenceNr() )
								{
								if( !isDeleteAllActiveWordTypes &&
								unusedReadItem.isPluralNoun() &&	// Wrong assumption: This noun isn't plural but singular
								unusedReadWordItem.isUserDefinedWord() &&
								( pluralNounString = unusedWordTypeItem.itemString() ) != null &&
								( singularNounWordTypeItem = unusedReadWordItem.activeWordTypeItem( false, Constants.WORD_TYPE_NOUN_SINGULAR ) ) != null )
									{
									if( singularNounWordTypeItem.createNewWordTypeString( pluralNounString ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a new word string for a singular noun word type of an active read word" );
									}

								if( ( readResult = adminItem_.readList.getNumberOfReadWordReferences( unusedReadItem.wordTypeNr(), unusedReadWordItem ) ).result == Constants.RESULT_OK )
									{
									if( ( nReadWordReferences = readResult.nReadWordReferences ) >= 1 )
										{
										unusedReadItem.isUnusedReadItem = true;

										if( nReadWordReferences == 1 )
											{
											if( unusedReadWordItem.deleteWordType( unusedReadItem.wordTypeNr() ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an unused word type item" );
											}
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an invalid number of read word references" );
									}
								else
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to get the number of read word references" );
								}
							}
						else
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't find the word type of an active read word" );
						}
					}

				previousReadItem = unusedReadItem;
				unusedReadItem = unusedReadItem.nextReadItem();
				}
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The read list isn't created yet" );

		return Constants.RESULT_OK;
		}

	private byte deleteUnusedWordTypes( boolean isDeleteAllActiveWordTypes )
		{
		if( deleteUnusedWordTypes( false, isDeleteAllActiveWordTypes ) == Constants.RESULT_OK )		// From active read items
			{
			if( deleteUnusedWordTypes( true, isDeleteAllActiveWordTypes ) != Constants.RESULT_OK )	// From inactive read items
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the active unused word types" );
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the inactive unused word types" );

		return Constants.RESULT_OK;
		}

	private byte rollbackDeletedRedoInfo()
		{
		if( adminItem_.wordList != null )							// Inside words
			{
			if( adminItem_.wordList.rollbackDeletedRedoInfoInWordList() != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the current redo info of my words in my word list" );
			}

		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
				adminItem_.adminListArray[adminListNr].rollbackDeletedRedoInfoInList();
			}

		return Constants.RESULT_OK;
		}

	private byte removeFirstRangeOfDeletedItems()
		{
		short adminListNr = 0;

		CommonVariables.nDeletedItems = 0;
		CommonVariables.removeSentenceNr = Constants.NO_SENTENCE_NR;
		CommonVariables.removeStartItemNr = Constants.NO_ITEM_NR;

		if( adminItem_.wordList != null )							// Inside words
			{
			if( adminItem_.wordList.removeFirstRangeOfDeletedItemsInWordList() != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to remove the first deleted items in my word list" );
			}

		while( adminListNr < Constants.NUMBER_OF_ADMIN_LISTS &&
		CommonVariables.nDeletedItems == 0 )
			{
			if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
				{
				if( adminItem_.adminListArray[adminListNr].removeFirstRangeOfDeletedItemsInList() != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to remove the first deleted items" );
				}

			adminListNr++;
			}

		return Constants.RESULT_OK;
		}

	private byte undoCurrentSentence()
		{
		if( undoCurrentSentenceInAllWords() == Constants.RESULT_OK )			// Inside words
			{
			for( short adminListNr : Constants.AdminLists )
				{
				if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
					{
					if( adminItem_.adminListArray[adminListNr].undoCurrentSentenceInList() != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to undo the current sentence" );
					}
				}
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to undo the current sentence in my word list" );

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
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte redoCurrentSentence()
		{
		if( redoCurrentSentenceInAllWords() == Constants.RESULT_OK )			// Inside words
			{
			for( short adminListNr : Constants.AdminLists )
				{
				if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
					{
					if( adminItem_.adminListArray[adminListNr].redoCurrentSentenceInList() != Constants.RESULT_OK )
						return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to redo the current sentence" );
					}
				}
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to redo the current sentence in my word list" );

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
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first word item is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminCleanup( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		isDontIncrementCurrentSentenceNr_ = false;
		hasFoundAnyChangeDuringThisSentence_ = false;
		wasUndoOrRedo_ = false;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void checkForChangesDuringThisSentence()
		{
		hasFoundAnyChangeDuringThisSentence_ = true;

		getHighestInUseSentenceNr( false, false, false, CommonVariables.currentSentenceNr );

		if( CommonVariables.highestInUseSentenceNr < CommonVariables.currentSentenceNr )
			hasFoundAnyChangeDuringThisSentence_ = false;
		else
			clearDontIncrementCurrentSentenceNr();		// Found new knowledge
		}

	protected void clearDontIncrementCurrentSentenceNr()
		{
		isDontIncrementCurrentSentenceNr_ = false;
		}

	protected void deleteRollbackInfo()
		{
		if( adminItem_.wordList != null )							// Inside words
			adminItem_.wordList.deleteRollbackInfoInWordList();

		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
				adminItem_.adminListArray[adminListNr].deleteRollbackInfoInList();
			}
		}

	protected void setCurrentItemNr()
		{
		CommonVariables.currentItemNr = Constants.NO_ITEM_NR;

		if( adminItem_.wordList != null )							// Inside words
			adminItem_.wordList.setCurrentItemNrInWordList();

		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
				adminItem_.adminListArray[adminListNr].setCurrentItemNrInList();
			}
		}

	protected boolean isDontIncrementCurrentSentenceNr()
		{
		return isDontIncrementCurrentSentenceNr_;
		}

	protected boolean hasFoundAnyChangeDuringThisSentence()
		{
		return hasFoundAnyChangeDuringThisSentence_;
		}

	protected boolean wasUndoOrRedo()
		{
		return wasUndoOrRedo_;
		}

	protected int highestSentenceNr()
		{
		int tempSentenceNr;
		int highestSentenceNr = Constants.NO_SENTENCE_NR;

		if( adminItem_.wordList != null )							// Inside words
			highestSentenceNr = adminItem_.wordList.highestSentenceNrInWordList();

		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null &&	// Inside admin lists
			( tempSentenceNr = adminItem_.adminListArray[adminListNr].highestSentenceNrInList() ) > highestSentenceNr )
				highestSentenceNr = tempSentenceNr;
			}

		return highestSentenceNr;
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
				previousRemoveSentenceNr != CommonVariables.removeSentenceNr )		// Previous deleted sentence is may be empty
					{
					getHighestInUseSentenceNr( true, true, true, previousRemoveSentenceNr );

					if( CommonVariables.highestInUseSentenceNr < previousRemoveSentenceNr )	// All items of this sentence are deleted
						{																// So, decrement all higher sentence numbers
						if( decrementSentenceNrs( previousRemoveSentenceNr ) == Constants.RESULT_OK )
							{
							if( CommonVariables.removeSentenceNr > previousRemoveSentenceNr )
								CommonVariables.removeSentenceNr--;

							if( CommonVariables.currentSentenceNr >= previousRemoveSentenceNr )
								{
								if( decrementCurrentSentenceNr() != Constants.RESULT_OK )
									return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to decrement the current sentence number" );
								}
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number" );
						}
					}

				if( CommonVariables.nDeletedItems > 0 )
					{
					if( decrementItemNrRange( CommonVariables.removeSentenceNr, CommonVariables.removeStartItemNr, CommonVariables.nDeletedItems ) == Constants.RESULT_OK )
						previousRemoveSentenceNr = CommonVariables.removeSentenceNr;
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to decrement item number range" );
					}
				}
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to remove the first deleted items" );
			}
		while( CommonVariables.nDeletedItems > 0 );

//		Presentation.clearStatus();

		return Constants.RESULT_OK;
		}

	protected byte deleteAllTemporaryLists()
		{
		wasUndoOrRedo_ = false;

		adminItem_.deleteTemporaryReadList();				// Read list is a temporary list
		adminItem_.deleteTemporaryScoreList();				// Score list is a temporary list

		deleteWriteListsInAllWords();					// Response lists are temporary lists

		if( cleanupDeletedItems() != Constants.RESULT_OK )
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to cleanup the deleted items" );

		return Constants.RESULT_OK;
		}

	protected byte deleteUnusedInterpretations( boolean isDeleteAllActiveWordTypes )
		{
		ReferenceResultType referenceResult = new ReferenceResultType();
		short adminListNr;
		short previousWordOrderNr = Constants.NO_ORDER_NR;
		WordItem unusedWordItem;
		ReadItem unusedReadItem = adminItem_.firstActiveReadItem();

		if( adminItem_.readList != null )
			{
			if( adminItem_.wordList != null )
				{
				if( deleteUnusedWordTypes( isDeleteAllActiveWordTypes ) == Constants.RESULT_OK )	// From active read items
					{
					while( unusedReadItem != null )
						{
						if( unusedReadItem.isUnusedReadItem ||
						unusedReadItem.wordOrderNr() == previousWordOrderNr )
							{
							previousWordOrderNr = unusedReadItem.wordOrderNr();
							unusedWordItem = unusedReadItem.readWordItem();

							if( adminItem_.readList.deleteItem( false, unusedReadItem ) == Constants.RESULT_OK )
								{
								if( unusedWordItem != null &&	// Skip text
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
												return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to find a reference to an active word \"" + unusedWordItem.anyWordTypeString() + "\" in one of my lists" );
											}

										adminListNr++;
										}

									if( !referenceResult.hasFoundWordReference )
										{
										if( adminItem_.wordList.deleteItem( false, unusedWordItem ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete a word item" );
										}
									}

								unusedReadItem = adminItem_.readList.nextReadListItem();
								}
							else
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an active read item" );
							}
						else
							{
							previousWordOrderNr = unusedReadItem.wordOrderNr();
							unusedReadItem = unusedReadItem.nextReadItem();
							}
						}

					while( ( unusedReadItem = adminItem_.firstInactiveReadItem() ) != null )
						{
						if( adminItem_.readList.deleteItem( false, unusedReadItem ) == Constants.RESULT_OK )
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
											return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to find a reference to an inactive word \"" + unusedWordItem.anyWordTypeString() + "\" in one of my lists" );
										}

									adminListNr++;
									}

								if( !referenceResult.hasFoundWordReference )
									{
									if( adminItem_.wordList.deleteItem( false, unusedWordItem ) != Constants.RESULT_OK )
										return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete a word item" );
									}
								}
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete an unused (inactive) read word" );
						}
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the unused word types" );
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The word list isn't created yet" );
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteSentences( boolean isAvailableForRollback, int lowestSentenceNr )
		{
		if( adminItem_.wordList != null )							// Inside words
			{
			if( adminItem_.wordList.deleteSentencesInWordList( isAvailableForRollback, lowestSentenceNr ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete sentences in my word list" );
			}

		for( short adminListNr : Constants.AdminLists )
			{
			if( adminItem_.adminListArray[adminListNr] != null )	// Inside admin lists
				{
				if( adminItem_.adminListArray[adminListNr].deleteSentencesInList( isAvailableForRollback, lowestSentenceNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( adminItem_.adminListChar( adminListNr ), 1, moduleNameString_, "I failed to delete sentences in a list" );
				}
			}

		if( cleanupDeletedItems() != Constants.RESULT_OK )
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to cleanup the deleted items" );

		return Constants.RESULT_OK;
		}

	protected byte undoLastSentence()
		{
		boolean skipUndo = false;

		if( CommonVariables.currentSentenceNr > Constants.NO_SENTENCE_NR )
			{
			if( deleteAllTemporaryLists() == Constants.RESULT_OK )			// Deleted read words of the undo sentence
				{
				if( CommonVariables.currentSentenceNr > highestSentenceNr() )
					{
					if( CommonVariables.currentSentenceNr - 1 >= adminItem_.firstUserSentenceNr() )		// Is my sentence
						{
						CommonVariables.currentSentenceNr--;
						setCurrentItemNr();		// Necessary after decrement of current sentence number
						}
					else
						{
						if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_UNDO_SENTENCE_OF_ANOTHER_USER ) == Constants.RESULT_OK )
							{
							skipUndo = true;
							isDontIncrementCurrentSentenceNr_ = true;
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification about another user" );
						}
					}

				if( !skipUndo )
					{
					// Roll-back Undo sentence. Handle it as a command, not as a sentence
					rollbackDeletedRedoInfo();

					if( undoCurrentSentence() == Constants.RESULT_OK )
						{
						if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, CommonVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) == Constants.RESULT_OK )
							{
							wasUndoOrRedo_ = true;
							isDontIncrementCurrentSentenceNr_ = true;
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification about having undone a sentence" );
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to undo the current sentence" );
					}
				}
			else
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete all temporary lists" );
			}
		else
			{
			if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_LEFT_TO_UNDO ) != Constants.RESULT_OK )
				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification about no sentences left to undo" );
			}

		return Constants.RESULT_OK;
		}

	protected byte redoLastUndoneSentence()
		{
		if( deleteAllTemporaryLists() == Constants.RESULT_OK )	// Deleted read words of the undo sentence
			{
			if( !isDontIncrementCurrentSentenceNr_ &&
			CommonVariables.currentSentenceNr > highestSentenceNr() )
				{
				rollbackDeletedRedoInfo();

				if( redoCurrentSentence() == Constants.RESULT_OK )
					{
					if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, CommonVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) == Constants.RESULT_OK )
						wasUndoOrRedo_ = true;
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification about having redone a sentence" );
					}
				else
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to redo the current sentence" );
				}
			else
				{
				if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != Constants.RESULT_OK )
					return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification about no sentences to redo" );
				}
			}
		else
			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete all temporary lists" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"How great is the Lord,
 *	how deserving of praise,
 *	in the city of our God,
 *	which sits on his holy mountain!" (Psalm 48:1)
 *
 *************************************************************************/
