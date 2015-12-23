/*
 *	Class:			WordCleanup
 *	Supports class:	WordItem
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

class WordCleanup
	{
	// Private constructible variables

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Constructor / deconstructor

	protected WordCleanup( WordItem myWordItem )
		{
		String errorString = null;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected void deleteRollbackInfo()
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null )
				myWordItem_.wordListArray[wordListNr].deleteRollbackInfoInList();
			}
		}

	protected void getHighestInUseSentenceNr( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, boolean isLanguageWord, int highestSentenceNr )
		{
		short wordListNr = 0;

		while( wordListNr < Constants.NUMBER_OF_WORD_LISTS &&
		CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			{
			if( myWordItem_.wordListArray[wordListNr] != null &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) &&

			// Skip temporary lists
			( isIncludingTemporaryLists ||
			!myWordItem_.wordListArray[wordListNr].isTemporaryList() ) )
				myWordItem_.wordListArray[wordListNr].getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			wordListNr++;
			}
		}

	protected void setCurrentItemNr( boolean isLanguageWord )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				myWordItem_.wordListArray[wordListNr].setCurrentItemNrInList();
			}
		}

	protected byte decrementItemNrRange( boolean isLanguageWord, int decrementSentenceNr, int decrementItemNr, int decrementOffset )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( myWordItem_.wordListArray[wordListNr].decrementItemNrRangeInList( decrementSentenceNr, decrementItemNr, decrementOffset ) != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte decrementSentenceNrs( boolean isLanguageWord, int startSentenceNr )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( myWordItem_.wordListArray[wordListNr].decrementSentenceNrsInList( startSentenceNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteSentences( boolean isAvailableForRollback, boolean isLanguageWord, int lowestSentenceNr )
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( myWordItem_.wordListArray[wordListNr].deleteSentencesInList( isAvailableForRollback, lowestSentenceNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to delete sentences in one of my lists" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte redoCurrentSentence()
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null )
				{
				if( myWordItem_.wordListArray[wordListNr].redoCurrentSentenceInList() != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to redo the current sentence" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItems( boolean isLanguageWord )
		{
		short wordListNr = 0;

		while( wordListNr < Constants.NUMBER_OF_WORD_LISTS &&
		CommonVariables.nDeletedItems == 0 )
			{
			if( myWordItem_.wordListArray[wordListNr] != null &&

			( !isLanguageWord ||

			// To increase performance, skip organizing grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( myWordItem_.wordListArray[wordListNr].removeFirstRangeOfDeletedItemsInList() != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to remove the first deleted items" );
				}

			wordListNr++;
			}

		return Constants.RESULT_OK;
		}

	protected byte rollbackDeletedRedoInfo()
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null )
				{
				if( myWordItem_.wordListArray[wordListNr].rollbackDeletedRedoInfoInList() != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to rollback the deleted redo info in one of my lists" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte undoCurrentSentence()
		{
		for( short wordListNr : Constants.WordLists )
			{
			if( myWordItem_.wordListArray[wordListNr] != null )
				{
				if( myWordItem_.wordListArray[wordListNr].undoCurrentSentenceInList() != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to undo the current sentence" );
				}
			}

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"He has paid a full ransom for his people.
 *	He has guaranteed his convenant with them forever.
 *	What a holy, awe-inspiring name he has!" (Psalm 111:9)
 *************************************************************************/
