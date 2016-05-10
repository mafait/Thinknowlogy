/*	Class:			WordCleanup
 *	Supports class:	WordItem
 *	Purpose:		To cleanup obsolete items
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
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

	protected void clearReplacedInfo()
		{
		List currentWordList;

		for( short wordListNr : Constants.WordLists )
			{
			currentWordList = myWordItem_.wordListArray[wordListNr];

			if( currentWordList != null &&

			// Not needed to clear replaced info in grammar, interface and temporary lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			!currentWordList.isTemporaryList() ) )
				currentWordList.clearReplacedInfoInList();
			}
		}

	protected void getHighestInUseSentenceNr( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, boolean isLanguageWord, int highestSentenceNr )
		{
		short wordListNr = 0;
		List currentWordList;

		while( wordListNr < Constants.NUMBER_OF_WORD_LISTS &&
		CommonVariables.highestInUseSentenceNr < highestSentenceNr )
			{
			currentWordList = myWordItem_.wordListArray[wordListNr];

			if( currentWordList != null &&

			( !isLanguageWord ||

			// Not needed to organize grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) &&

			// Skip temporary lists
			( isIncludingTemporaryLists ||
			!currentWordList.isTemporaryList() ) )
				currentWordList.getHighestInUseSentenceNrInList( isIncludingDeletedItems, highestSentenceNr );

			wordListNr++;
			}
		}

	protected void setCurrentItemNr( boolean isLanguageWord )
		{
		List currentWordList;

		for( short wordListNr : Constants.WordLists )
			{
			currentWordList = myWordItem_.wordListArray[wordListNr];

			if( currentWordList != null &&

			( !isLanguageWord ||

			// Not needed to organize grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				currentWordList.setCurrentItemNrInList();
			}
		}

	protected byte decrementItemNrRange( boolean isLanguageWord, int decrementSentenceNr, int decrementItemNr, int decrementOffset )
		{
		List currentWordList;

		for( short wordListNr : Constants.WordLists )
			{
			currentWordList = myWordItem_.wordListArray[wordListNr];

			if( currentWordList != null &&

			( !isLanguageWord ||

			// Not needed to organize grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( currentWordList.decrementItemNrRangeInList( decrementSentenceNr, decrementItemNr, decrementOffset ) != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to decrement item number range" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte decrementSentenceNrs( boolean isLanguageWord, int startSentenceNr )
		{
		List currentWordList;

		for( short wordListNr : Constants.WordLists )
			{
			currentWordList = myWordItem_.wordListArray[wordListNr];

			if( currentWordList != null &&

			( !isLanguageWord ||

			// Not needed to organize grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( currentWordList.decrementSentenceNrsInList( startSentenceNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to decrement the sentence numbers from the current sentence number in one of my lists" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte deleteSentences( boolean isLanguageWord, int lowestSentenceNr )
		{
		List currentWordList;

		for( short wordListNr : Constants.WordLists )
			{
			currentWordList = myWordItem_.wordListArray[wordListNr];

			if( currentWordList != null &&

			( !isLanguageWord ||

			// Not needed to organize grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( currentWordList.deleteSentencesInList( lowestSentenceNr ) != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to delete sentences in one of my lists" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte redoCurrentSentence()
		{
		List currentWordList;

		for( short wordListNr : Constants.WordLists )
			{
			// Not needed to redo items in grammar and interface lists
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = myWordItem_.wordListArray[wordListNr] ) != null )
				{
				// Not needed to redo items in temporary lists
				if( !currentWordList.isTemporaryList() &&
				currentWordList.redoCurrentSentenceInList() != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to redo the current sentence" );
				}
			}

		return Constants.RESULT_OK;
		}

	protected byte removeFirstRangeOfDeletedItems( boolean isLanguageWord )
		{
		short wordListNr = 0;
		List currentWordList;

		while( wordListNr < Constants.NUMBER_OF_WORD_LISTS &&
		CommonVariables.nDeletedItems == 0 )
			{
			currentWordList = myWordItem_.wordListArray[wordListNr];

			if( currentWordList != null &&

			( !isLanguageWord ||

			// Not needed to organize grammar and interface lists
			( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST ) ) )
				{
				if( currentWordList.removeFirstRangeOfDeletedItemsInList() != Constants.RESULT_OK )
					return myWordItem_.addErrorWithWordListNr( wordListNr, 1, moduleNameString_, "I failed to remove the first deleted items" );
				}

			wordListNr++;
			}

		return Constants.RESULT_OK;
		}

	protected byte undoCurrentSentence()
		{
		List currentWordList;

		for( short wordListNr : Constants.WordLists )
			{
			// Not needed to undo items in grammar and interface lists
			if( wordListNr != Constants.WORD_GRAMMAR_LIST &&
			wordListNr != Constants.WORD_INTERFACE_LIST &&
			( currentWordList = myWordItem_.wordListArray[wordListNr] ) != null )
				{
				// Not needed to undo items in temporary lists
				if( !currentWordList.isTemporaryList() &&
				currentWordList.undoCurrentSentenceInList() != Constants.RESULT_OK )
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
