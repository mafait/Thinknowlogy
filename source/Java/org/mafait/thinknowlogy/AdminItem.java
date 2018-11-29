/*	Class:			AdminItem
 *	Parent class:	WordItem
 *	Grand parent:	Item
 *	Purpose:		To process tasks at administration level
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

import java.io.BufferedReader;
import java.io.BufferedWriter;

class AdminItem extends WordItem
	{
	// Private constructed variables

	private boolean isSystemStartingUp_ = true;
	private boolean wasUndoOrRedoCommand_ = false;

	// Admin item supporting modules
	private AdminImperative adminImperative_ = null;
	private AdminQuery adminQuery_ = null;
	private AdminReadCreateWords adminReadCreateWords_ = null;
	private AdminReadFile adminReadFile_ = null;
	private AdminReadSentence adminReadSentence_ = null;
	private AdminReasoningNew adminReasoningNew_ = null;
	private AdminReasoningOld adminReasoningOld_ = null;
	private AdminSpecification adminSpecification_ = null;
	private AdminWrite adminWrite_ = null;

	// Admin item lists
	private FileList fileList_ = null;
	private ReadList readList_ = null;
	private ScoreList scoreList_ = null;
	private SelectionList conditionList_ = null;
	private SelectionList actionList_ = null;
	private SelectionList alternativeList_ = null;
	private WordList wordList_ = null;

	// Admin item lists array
	private List[] adminListArray_ = new List[Constants.NUMBER_OF_ADMIN_LISTS];

	// Private initialized variables

	String startupLanguageNameString_ = null;


	// Private methods

	private static void rebuildQuickAccessListsInAllWords()
		{
		WordItem currentWordItem;

		// Initialize the four starting points of the quick access lists
		GlobalVariables.firstAssignmentWordItem = null;
		GlobalVariables.firstCollectionWordItem = null;
		GlobalVariables.firstContextWordItem = null;
		GlobalVariables.firstPossessiveNounWordItem = null;
		GlobalVariables.firstSpecificationWordItem = null;
		GlobalVariables.firstUserProperNounWordItem = null;

		GlobalVariables.lastCollectionWordItem = null;
		GlobalVariables.lastContextWordItem = null;

		// Rebuild quick access lists
		if( ( currentWordItem = GlobalVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	currentWordItem.rebuildQuickAccessWordLists();
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		}

	private byte assignLanguage( String languageNameString )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read file module isn't created yet" );

		return adminReadFile_.assignLanguage( languageNameString );
		}

	private byte initializeInterfaceLanguageStringBuffer()
		{
		WordItem predefinedNounLanguageWordItem;

		if( ( predefinedNounLanguageWordItem = GlobalVariables.predefinedNounLanguageWordItem ) == null )
			return startSystemError( 1, null, null, "The predefined noun language word item is undefined" );

		GlobalVariables.queryStringBuffer = new StringBuffer();

		if( executeQuery( true, false, false, Constants.INPUT_OUTPUT_PROMPT_QUERY, 1, ( Constants.EMPTY_STRING + Constants.QUERY_CHAR + Constants.QUERY_ACTIVE_CHAR + Constants.QUERY_WORD_REFERENCE_START_CHAR + predefinedNounLanguageWordItem.anyWordTypeString() + Constants.QUERY_WORD_REFERENCE_END_CHAR + Constants.QUERY_LIST_START_CHAR + Constants.WORD_SPECIFICATION_LIST_SYMBOL + Constants.QUERY_LIST_END_CHAR + Constants.QUERY_WORD_START_CHAR ) ).result != Constants.RESULT_OK )
			return addError( 1, null, null, "I failed to execute a query" );

		// More than one language available
		if( GlobalVariables.queryStringBuffer.indexOf( Constants.QUERY_SEPARATOR_SPACE_STRING ) > 0 )
			GlobalVariables.interfaceLanguageStringBuffer = GlobalVariables.queryStringBuffer;

		return Constants.RESULT_OK;
		}

	private byte readStartupFile()
		{
		if( adminReadFile_ == null &&
		// Create supporting module
		( adminReadFile_ = new AdminReadFile( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin read module" );

		if( fileList_ == null )
			{
			// Create list
			if( ( fileList_ = new FileList( this ) ) == null )
				return startError( 1, null, null, "I failed to create the file list" );

			adminListArray_[Constants.ADMIN_FILE_LIST] = fileList_;
			}

		return adminReadFile_.readStartupFile();
		}

	private byte startup()
		{
		if( readStartupFile() != Constants.RESULT_OK )
			return startSystemError( 1, null, null, "I failed to read the startup file" );

		if( initializeInterfaceLanguageStringBuffer() != Constants.RESULT_OK )
			// No show stopper. So, no return. Just continue.
			addError( 1, null, null, "I failed to initialize interface language string buffer" );

		if( startupLanguageNameString_ == null )
			// At the first start, assign the first language as the startup language
			startupLanguageNameString_ = languageNameString( (short)1 );

		if( assignLanguage( startupLanguageNameString_ ) != Constants.RESULT_OK )
			// No show stopper. So, no return. Just continue.
			addError( 1, null, null, "I failed to assign the language" );

		// Try to login without password
		if( login( null ) != Constants.RESULT_OK )
			return startSystemError( 1, null, null, "I failed to login" );

		if( GlobalVariables.currentUserNr <= Constants.NO_USER_NR )
			return startSystemError( 1, null, null, "No user is logged in" );

		isSystemStartingUp_ = false;

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminItem( String startupLanguageNameString )
		{
		// Private constructed variables

		// Admin item lists array
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			adminListArray_[adminListNr] = null;

		// Private initialized variables

		// Point to the given string (in class Thinknowlogy)
		startupLanguageNameString_ = startupLanguageNameString;

		// Initialize global variables after restart
		GlobalVariables.initialize();
		// Initialize input-output after restart
		InputOutput.initialize();
		// Initialize admin item variables after restart
		initializeItemVariables( this );

		// Now start the system
		if( startup() != Constants.RESULT_OK )
			addError( 1, null, null, "I failed to start the administrator" );
		}


	// Protected common methods

	protected void clearTemporaryAdminLists()
		{
		wasUndoOrRedoCommand_ = false;

		deleteTemporaryReadList();
		deleteTemporaryScoreList();
		}

	protected void initializeAdminVariables()
		{
		if( adminReadFile_ != null )
			adminReadFile_.initializeAdminReadFileVariables();

		if( adminReasoningNew_ != null )
			adminReasoningNew_.initializeAdminReasoningNewVariables();

		if( adminReasoningOld_ != null )
			adminReasoningOld_.initializeAdminReasoningOldVariables();

		if( adminSpecification_ != null )
			adminSpecification_.initializeAdminSpecificationVariables();
		}

	protected boolean isSystemStartingUp()
		{
		return isSystemStartingUp_;
		}

	protected boolean wasUndoOrRedoCommand()
		{
		return wasUndoOrRedoCommand_;
		}

	protected int highestContextNrInAllContextWords()
		{
		int highestContextNr = Constants.NO_CONTEXT_NR;
		int tempContextNr;
		WordItem currentContextWordItem;

		if( ( currentContextWordItem = GlobalVariables.firstContextWordItem ) != null )
			{
			// Do for all context words
			do	{
				if( ( tempContextNr = currentContextWordItem.highestContextNrInWord() ) > highestContextNr )
					highestContextNr = tempContextNr;
				}
			while( ( currentContextWordItem = currentContextWordItem.nextContextWordItem ) != null );
			}

		return highestContextNr;
		}

	protected int highestFoundSentenceNr( boolean isIncludingDeletedItems, boolean isIncludingTemporaryLists, int maxSentenceNr )
		{
		int highestFoundSentenceNr = Constants.NO_SENTENCE_NR;
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			highestFoundSentenceNr = wordList_.highestFoundSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, maxSentenceNr );

		// Admin lists
		for( short adminListNr = 0; ( adminListNr < Constants.NUMBER_OF_ADMIN_LISTS && highestFoundSentenceNr < maxSentenceNr ); adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&

			( isIncludingTemporaryLists ||
			!currentAdminList.isTemporaryList() ) &&

			// Efficiency: Only select lists with higher sentence number
			currentAdminList.highestSentenceNrInList() > highestFoundSentenceNr )
				highestFoundSentenceNr = currentAdminList.highestFoundSentenceNrInList( isIncludingDeletedItems, highestFoundSentenceNr, maxSentenceNr );
			}

		return highestFoundSentenceNr;
		}

	protected char adminListChar( short adminListNr )
		{
		switch( adminListNr )
			{
			case Constants.ADMIN_FILE_LIST:
				return Constants.ADMIN_FILE_LIST_SYMBOL;

			case Constants.ADMIN_READ_LIST:
				return Constants.ADMIN_READ_LIST_SYMBOL;

			case Constants.ADMIN_SCORE_LIST:
				return Constants.ADMIN_SCORE_LIST_SYMBOL;

			case Constants.ADMIN_WORD_LIST:
				return Constants.ADMIN_WORD_LIST_SYMBOL;

			case Constants.ADMIN_CONDITION_LIST:
				return Constants.ADMIN_CONDITION_LIST_SYMBOL;

			case Constants.ADMIN_ACTION_LIST:
				return Constants.ADMIN_ACTION_LIST_SYMBOL;

			case Constants.ADMIN_ALTERNATIVE_LIST:
				return Constants.ADMIN_ALTERNATIVE_LIST_SYMBOL;
			}

		return Constants.SYMBOL_QUESTION_MARK;
		}

	protected String currentLanguageNameString()
		{
		return languageNameString( GlobalVariables.currentLanguageNr );
		}

	protected String startupLanguageNameString()
		{
		return startupLanguageNameString_;
		}


	// Protected error methods

	protected byte addErrorWithAdminListNr( short adminListNr, int methodLevel, String moduleNameString, String errorString )
		{
		InputOutput.displayError( adminListChar( adminListNr ), ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), null, ( methodLevel + 1 ), errorString );
		return GlobalVariables.result;
		}

	protected byte startErrorWithAdminListNr( short adminListNr, int methodLevel, String moduleNameString, String errorString )
		{
		addErrorWithAdminListNr( adminListNr, ( methodLevel + 1 ), moduleNameString, errorString );

		GlobalVariables.result = Constants.RESULT_ERROR;

		return Constants.RESULT_ERROR;
		}


	// Protected database connection methods
/*
	protected byte storeChangesInFutureDatabase()
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null &&
		wordList_.storeChangesInFutureDatabaseInWordList() != Constants.RESULT_OK )
			return addError( 1, null, null, "I failed to store changes of my words list in future database" );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != null &&
			// No need to store items of temporary lists
			!currentAdminList.isTemporaryList() &&
			currentAdminList.storeChangesInFutureDatabaseInList() != Constants.RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, 1, null, "I failed to store changes in future database of an Admin list" );
			}

		return Constants.RESULT_OK;
		}
*/

	// Protected imperative methods

	protected void clearConditionChecksForSolving( short selectionLevel, int conditionSentenceNr )
		{
		if( conditionList_ != null )
			conditionList_.clearConditionChecksForSolving( selectionLevel, conditionSentenceNr );
		}

	protected void deleteTemporaryScoreList()
		{
		if( scoreList_ != null )
			scoreList_.deleteTemporaryList();
		}

	protected boolean hasRequestedRestart()
		{
		return ( adminImperative_ != null ?
				adminImperative_.hasRequestedRestart() : false );
		}

	protected boolean hasScoreList()
		{
		return ( scoreList_ != null );
		}

	protected int nPossibilities()
		{
		return ( scoreList_ != null ?
				scoreList_.nPossibilities() : 0 );
		}

	protected byte changeAction( SelectionItem actionSelectionItem )
		{
		if( scoreList_ == null )
			return startError( 1, null, null, "The score list isn't created yet" );

		return scoreList_.changeAction( actionSelectionItem );
		}

	protected byte createScoreItem( boolean isChecked, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore, SelectionItem referenceSelectionItem )
		{
		if( scoreList_ == null )
			{
			// Create list
			if( ( scoreList_ = new ScoreList( this ) ) == null )
				return startError( 1, null, null, "I failed to create the admin solve score list" );

			adminListArray_[Constants.ADMIN_SCORE_LIST] = scoreList_;
			}

		return scoreList_.createScoreItem( isChecked, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem );
		}

	protected byte deleteScores()
		{
		if( scoreList_ == null )
			return startError( 1, null, null, "The score list isn't created yet" );

		return scoreList_.deleteScores();
		}

	protected byte executeImperative( boolean isInitializeVariables, short executionListNr, short executionNounWordParameter, short imperativeVerbParameter, short specificationWordTypeNr, int endSolveProgressLevel, String executionString, WordItem generalizationWordItem, WordItem specificationWordItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem, SelectionItem executionSelectionItem, SelectionItem actionSelectionItem )
		{
		if( adminImperative_ == null )
			{
			// Check before getting variables
			if( adminReadFile_ == null )
				return startError( 1, null, null, "The admin read file module isn't created yet" );

			// Create supporting module
			if( ( adminImperative_ = new AdminImperative( this, adminReadFile_.predefinedAdjectiveBusyWordItem(), adminReadFile_.predefinedAdjectiveDoneWordItem(), adminReadFile_.predefinedAdjectiveInvertedWordItem(), adminReadFile_.predefinedNounSolveLevelWordItem(), adminReadFile_.predefinedNounSolveMethodWordItem(), adminReadFile_.predefinedNounSolveStrategyWordItem() ) ) == null )
				return startError( 1, null, null, "I failed to create the admin imperative module" );
			}

		return adminImperative_.executeImperative( isInitializeVariables, executionListNr, executionNounWordParameter, imperativeVerbParameter, specificationWordTypeNr, endSolveProgressLevel, executionString, generalizationWordItem, specificationWordItem, startRelationReadItem, endRelationReadItem, executionSelectionItem, actionSelectionItem );
		}

	protected byte executeSelections()
		{
		if( adminImperative_ != null )
			return adminImperative_.executeSelection( Constants.MAX_PROGRESS, null );

		// There are no selections yet. Just return: Okay.
		return Constants.RESULT_OK;
		}

	protected byte redoLastUndoneSentence()
		{
		List currentAdminList;

		wasUndoOrRedoCommand_ = true;

		if( highestFoundSentenceNr( true, false, GlobalVariables.currentSentenceNr ) == GlobalVariables.currentSentenceNr )
			{
			// Important: Redo admin lists first, and the words after that.
			// Because redoing admin words list might redo words.
			for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
				// No need to redo items of temporary lists
				!currentAdminList.isTemporaryList() &&
				currentAdminList.redoCurrentSentenceInList() != Constants.RESULT_OK )
					return addErrorWithAdminListNr( adminListNr, 1, null, "I failed to redo the current sentence in an Admin list" );
				}

			// Word lists
			if( wordList_ != null )
				wordList_.redoCurrentSentenceInWordList();

			if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, GlobalVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) != Constants.RESULT_OK )
				return addError( 1, null, null, "I failed to write the 'I have redone' interface notification" );

			rebuildQuickAccessListsInAllWords();
			}
		else
			{
			// No sentences found to redo
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != Constants.RESULT_OK )
				return addError( 1, null, null, "I failed to write the 'no sentences to redo' interface notification" );

			decrementCurrentSentenceNr();
			}

		return Constants.RESULT_OK;
		}

	protected byte undoLastSentence()
		{
		int firstSentenceNr = firstSentenceNrOfCurrentUser();
		List currentAdminList;

		wasUndoOrRedoCommand_ = true;

		// Remove the deleted read items of this undo sentence
		if( GlobalVariables.currentSentenceNr > firstSentenceNr )
			{
			decrementCurrentSentenceNr();

			// Word lists
			if( wordList_ != null )
				wordList_.undoCurrentSentenceInWordList();

			// Admin lists
			for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
				// No need to redo items of temporary lists
				!currentAdminList.isTemporaryList() &&
				currentAdminList.undoCurrentSentenceInList() != Constants.RESULT_OK )
					return addErrorWithAdminListNr( adminListNr, 1, null, "I failed to undo the current sentence" );
				}

			if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, GlobalVariables.currentSentenceNr, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) != Constants.RESULT_OK )
				return addError( 1, null, null, "I failed to write the 'I have undone' interface notification" );

			rebuildQuickAccessListsInAllWords();
			}
		else
			{
			// No sentences found to undo
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_UNDO ) != Constants.RESULT_OK )
				return addError( 1, null, null, "I failed to write the 'no sentences to undo' interface notification" );
			}

		if( GlobalVariables.currentSentenceNr >= firstSentenceNr )
			decrementCurrentSentenceNr();

		return Constants.RESULT_OK;
		}

	protected ScoreItem firstPossibility()
		{
		return ( scoreList_ != null ?
				scoreList_.firstPossibility() : null );
		}

	protected SelectionItem executionStartEntry( boolean isAction, short executionLevel, int executionSentenceNr )
		{
		if( isAction )
			return ( actionList_ != null ?
					actionList_.executionStartEntry( executionLevel, executionSentenceNr ) : null );

		return ( alternativeList_ != null ?
				alternativeList_.executionStartEntry( executionLevel, executionSentenceNr ) : null );
		}

	protected SelectionItem firstConditionItem()
		{
		return ( conditionList_ != null ?
				conditionList_.firstActiveSelectionItem() : null );
		}

	protected SelectionItem firstConditionSelectionItem( int conditionSentenceNr )
		{
		return ( conditionList_ != null ?
				conditionList_.firstConditionSelectionItem( conditionSentenceNr ) : null );
		}

	protected SelectionItem firstSelectionItem( boolean isAction, WordItem solveWordItem )
		{
		if( isAction )
			return ( actionList_ != null ?
					actionList_.firstSelectionItem( solveWordItem ) : null );

		return ( alternativeList_ != null ?
				alternativeList_.firstSelectionItem( solveWordItem ) : null );
		}

	protected BoolResultType checkScores( boolean isInverted, short solveStrategyParameter, int oldSatisfiedScore, int newSatisfiedScore, int oldDissatisfiedScore, int newDissatisfiedScore, int oldNotBlockingScore, int newNotBlockingScore, int oldBlockingScore, int newBlockingScore )
		{
		if( scoreList_ == null )
			return startBoolResultError( 1, null, "The score list isn't created yet" );

		return scoreList_.checkScores( isInverted, solveStrategyParameter, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore );
		}

	protected BoolResultType findScore( boolean isPreparingSort, SelectionItem findScoreItem )
		{
		if( scoreList_ == null )
			return startBoolResultError( 1, null, "The score list isn't created yet" );

		return scoreList_.findScore( isPreparingSort, findScoreItem );
		}

	protected SelectionResultType getBestSelection( boolean isCurrentlyTesting, short solveStrategyParameter )
		{
		if( scoreList_ == null )
			return startSelectionResultError( 1, "The score list isn't created yet" );

		return scoreList_.getBestSelection( isCurrentlyTesting, solveStrategyParameter );
		}


	// Protected query methods

	protected void clearQuerySelections()
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.clearQuerySelectionsInWordList();

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null )
				currentAdminList.clearQuerySelectionsInList();
			}
		}

	protected void countQuery()
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.countQueryInWordList();

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null )
				currentAdminList.countQueryInList();
			}
		}

	protected void itemQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isReferenceQuery, int querySentenceNr, int queryItemNr )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null )
				currentAdminList.itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			}
		}

	protected void listQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, StringBuffer queryListStringBuffer )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListStringBuffer );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null )
				currentAdminList.listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryListStringBuffer );
			}
		}

	protected void parameterQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, int queryParameter )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null )
				currentAdminList.parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryParameter );
			}
		}

	protected void wordTypeQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, short queryWordTypeNr )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null )
				currentAdminList.wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryWordTypeNr );
			}
		}

	protected byte displayQueryResult( boolean isOnlyDisplayingWords, boolean isOnlyDisplayingWordReferences, boolean isOnlyDisplayingStrings, boolean isReturnQueryToPosition, short promptTypeNr, short queryWordTypeNr, int queryWidth )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null &&
		wordList_.displayQueryResultInWordList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
			return addError( 1, null, null, "I failed to display the query result in my words list" );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
			currentAdminList.displayQueryResultInList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != Constants.RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, 1, null, "I failed to display the query result of an Admin list" );
			}

		return Constants.RESULT_OK;
		}

	protected byte stringQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, String queryString )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null &&
		wordList_.stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryString ) != Constants.RESULT_OK )
			return addError( 1, null, null, "I failed to query strings in my words list" );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
			currentAdminList.stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, queryString ) != Constants.RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, 1, null, "I failed to query strings in an Admin list" );
			}

		return Constants.RESULT_OK;
		}

	protected byte wordQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, String wordNameString )
		{
		// Word lists
		if( wordList_ != null &&
		wordList_.wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, wordNameString ) != Constants.RESULT_OK )
			return addError( 1, null, null, "I failed to query the words in my words list" );

		return Constants.RESULT_OK;
		}

	protected byte wordReferenceQuery( boolean isSelectingOnFind, boolean isSelectingActiveItems, boolean isSelectingInactiveItems, boolean isSelectingArchivedItems, boolean isSelectingReplacedItems, boolean isSelectingAttachedJustifications, boolean isSelectingJustificationSpecifications, String wordReferenceNameString )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null &&
		wordList_.wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != Constants.RESULT_OK )
			return addError( 1, null, null, "I failed to query word references in my words list" );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
			currentAdminList.wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, false, false, wordReferenceNameString ) != Constants.RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, 1, null, "I failed to query word references in an Admin list" );
			}

		return Constants.RESULT_OK;
		}

	protected QueryResultType executeQuery( boolean isSuppressingMessage, boolean isReturningToPosition, boolean isWritingQueryResult, short promptTypeNr, int queryCommandStringStartPosition, String queryString )
		{
		if( adminQuery_ == null &&
		// Create supporting module
		( adminQuery_ = new AdminQuery( this ) ) == null )
			return startQueryResultError( 1, null, "I failed to create the admin query module" );

		return adminQuery_.executeQuery( isSuppressingMessage, isReturningToPosition, isWritingQueryResult, promptTypeNr, queryCommandStringStartPosition, queryString );
		}


	// Protected read create words methods

	protected void deleteTemporaryReadList()
		{
		if( readList_ != null )
			readList_.deleteTemporaryList();
		}

	protected short lastCreatedWordOrderNr()
		{
		return ( adminReadCreateWords_ != null ?
				adminReadCreateWords_.lastCreatedWordOrderNr() : Constants.NO_ORDER_NR );
		}

	protected int convertChineseNumbers( char chineseChar )
		{
		return ( adminReadCreateWords_ != null ?
				adminReadCreateWords_.convertChineseNumbers( chineseChar ) : 0 );
		}

	protected byte createReadItem( boolean isUncountableGeneralizationNoun, short wordOrderNr, short wordParameter, short wordTypeNr, int readStringLength, String readString, WordItem readWordItem )
		{

		if( readList_ == null )
			{
			// Create list
			if( ( readList_ = new ReadList( this ) ) == null )
				return startError( 1, null, "I failed to create the admin read list" );

			adminListArray_[Constants.ADMIN_READ_LIST] = readList_;
			}

		return readList_.createReadItem( isUncountableGeneralizationNoun, wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem );
		}

	protected byte deleteReadItemsWithNonMatchingMultipleWordPart( short wordOrderNr, String sentenceString )
		{
		if( readList_ == null )
			return startError( 1, null, null, "The read list isn't created yet" );

		return readList_.deleteReadItemsWithNonMatchingMultipleWordPart( wordOrderNr, sentenceString );
		}

	protected ReadItem firstActiveReadItem()
		{
		return ( readList_ != null ?
				readList_.firstActiveReadItem() : null );
		}

	protected ReadItem firstInactiveReadItem()
		{
		return ( readList_ != null ?
				readList_.firstInactiveReadItem() : null );
		}

	protected ReadItem nextReadListItem()
		{
		return ( readList_ != null ?
				readList_.nextReadListItem() : null );
		}

	protected BoolResultType createReadWords( boolean isChineseCurrentLanguage, String readUserSentenceString )
		{
		if( adminReadCreateWords_ == null )
			return startBoolResultError( 1, null, "The admin read create words module isn't created yet" );

		return adminReadCreateWords_.createReadWords( isChineseCurrentLanguage, readUserSentenceString );
		}

	protected ReadWordResultType readWordFromString( boolean isCheckingForGrammarDefinition, boolean isChineseCurrentLanguage, boolean isMergedWord, boolean isSkippingTextString, int minimumStringLength, String wordString )
		{
		if( adminReadCreateWords_ == null )
			return startReadWordResultError( 1, null, "The admin read create words module isn't created yet" );

		return adminReadCreateWords_.readWordFromString( isCheckingForGrammarDefinition, isChineseCurrentLanguage, isMergedWord, isSkippingTextString, minimumStringLength, wordString );
		}

	protected ShortResultType getNumberOfReadWordReferences( short wordTypeNr, WordItem readWordItem )
		{
		if( readList_ == null )
			return startShortResultError( 1, null, "The read list isn't created yet" );

		return readList_.getNumberOfReadWordReferences( wordTypeNr, readWordItem );
		}

	protected WordResultType addWord( boolean isLanguageWord, boolean isMultipleWord, short previousWordAdjectiveParameter, short previousWordDefiniteArticleParameter, short previousWordIndefiniteArticleParameter, short wordParameter, short wordTypeNr, int wordLength, String wordString )
		{
		if( adminReadCreateWords_ == null &&
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this ) ) == null )
			return startWordResultError( 1, null, "I failed to create the admin read create words module" );

		return adminReadCreateWords_.addWord( isLanguageWord, isMultipleWord, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, wordParameter, wordTypeNr, wordLength, wordString );
		}

	protected WordResultType createWordItem( boolean isLanguageWord, short wordParameter )
		{
		if( wordList_ == null )
			{
			// Create list
			if( ( wordList_ = new WordList( this ) ) == null )
				return startWordResultError( 1, null, "I failed to create the word list" );

			adminListArray_[Constants.ADMIN_WORD_LIST] = wordList_;
			}

		return wordList_.createWordItem( isLanguageWord, wordParameter );
		}

	protected WordResultType findWordTypeInAllWords( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString, WordItem previousWordItem )
		{
		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.findWordTypeInAllWords( isCheckingAllLanguages, wordTypeNr, wordTypeString, previousWordItem );

		return new WordResultType();
		}


	// Protected read file methods

	protected void decrementCurrentSentenceNr()
		{
		if( GlobalVariables.currentSentenceNr > Constants.NO_SENTENCE_NR )
			{
			GlobalVariables.currentSentenceNr--;
			// Necessary after changing current sentence number
			GlobalVariables.currentSentenceItemNr = highestCurrentSentenceItemNr();
			}
		}

	protected void decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
			// Efficiency: Only select lists with decrement sentence number of higher
			currentAdminList.highestSentenceNrInList() >= decrementSentenceNr )
				currentAdminList.decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset );
			}
		}

	protected void decrementSentenceNrs( int startSentenceNr )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.decrementSentenceNrsInWordList( startSentenceNr );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
			// Efficiency: Only select lists with start sentence number of higher
			currentAdminList.highestSentenceNrInList() >= startSentenceNr )
				currentAdminList.decrementSentenceNrsInList( startSentenceNr );
			}
		}

	protected void deleteSentences( int lowestSentenceNr )
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.deleteSentencesInWordList( lowestSentenceNr );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
			// Efficiency: Only select lists with lowest sentence number of higher
			currentAdminList.highestSentenceNrInList() >= lowestSentenceNr )
				currentAdminList.deleteSentencesInList( lowestSentenceNr );
			}

		rebuildQuickAccessListsInAllWords();
		}

	protected void removeFirstRangeOfDeletedItems()
		{
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			wordList_.removeFirstRangeOfDeletedItemsInWordList();

		// Admin lists
		for( short adminListNr = 0; ( adminListNr < Constants.NUMBER_OF_ADMIN_LISTS && GlobalVariables.nDeletedItems == 0 ); adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null )
				currentAdminList.removeFirstRangeOfDeletedItemsInList();
			}
		}

	protected boolean isCurrentFileTestFile()
		{
		return ( fileList_ != null ?
				fileList_.isCurrentFileTestFile() : false );
		}

	protected boolean isCurrentlyTesting()
		{
		return ( fileList_ != null ?
				fileList_.isCurrentlyTesting() : false );
		}

	protected boolean isDeveloperTheCurrentUser()
		{
		return ( adminReadFile_ != null ?
				adminReadFile_.isDeveloperTheCurrentUser() : false );
		}

	protected boolean isDeveloperOrExpertTheCurrentUser()
		{
		return ( adminReadFile_ != null ?
				adminReadFile_.isDeveloperOrExpertTheCurrentUser() : false );
		}

	protected boolean isExpertTheCurrentUser()
		{
		return ( adminReadFile_ != null ?
				adminReadFile_.isExpertTheCurrentUser() : false );
		}

	protected boolean isDisplayingLine()
		{
		return ( !isSystemStartingUp_ &&
				fileList_ != null ?
				fileList_.isDisplayingLine() : false );
		}

	protected int currentFileSentenceNr()
		{
		return ( fileList_ != null ?
				fileList_.currentFileSentenceNr() : Constants.NO_SENTENCE_NR );
		}

	protected int firstSentenceNrOfCurrentUser()
		{
		return ( adminReadFile_ != null ?
				adminReadFile_.firstSentenceNrOfCurrentUser() : Constants.NO_SENTENCE_NR );
		}

	protected int highestCurrentSentenceItemNr()
		{
		int currentSentenceNr = GlobalVariables.currentSentenceNr;
		int highestItemNr = Constants.NO_ITEM_NR;
		List currentAdminList;

		// Word lists
		if( wordList_ != null )
			highestItemNr = wordList_.highestCurrentSentenceItemNrInWordList( currentSentenceNr, highestItemNr );

		// Admin lists
		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != null &&
			// Efficiency: Only select lists with current sentence number or higher
			currentAdminList.highestSentenceNrInList() >= currentSentenceNr )
				highestItemNr = currentAdminList.highestCurrentSentenceItemNrInList( currentSentenceNr, highestItemNr );
			}

		return highestItemNr;
		}

	protected byte closeCurrentFile( FileItem closeFileItem )
		{
		if( fileList_ == null )
			return startError( 1, null, "The file list isn't created yet" );

		return fileList_.closeCurrentFile( closeFileItem );
		}

	protected byte compareOutputFileAgainstReferenceFile( String testFileNameString )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read file module isn't created yet" );

		return adminReadFile_.compareOutputFileAgainstReferenceFile( testFileNameString );
		}

	protected byte interact()
		{
		if( GlobalVariables.result == Constants.RESULT_SYSTEM_ERROR )
			return Constants.RESULT_SYSTEM_ERROR;

		if( adminReadFile_ == null )
			return startSystemError( 1, null, null, "The admin read file module isn't created yet" );

		if( adminReadFile_.readAndExecute() == Constants.RESULT_SYSTEM_ERROR )
			return startSystemError( 1, null, null, "I failed to read and execute" );

		// Ignore Constants.RESULT_ERROR. Application will only exit on system error
		GlobalVariables.result = Constants.RESULT_OK;
		// Application will not exit on a normal error
		return Constants.RESULT_OK;
		}

	protected byte login( WordItem specificationWordItem )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read file module isn't created yet" );

		return adminReadFile_.login( specificationWordItem );
		}

	protected byte readExampleFile( String exampleFileNameString )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read file module isn't created yet" );

		return adminReadFile_.readExampleFile( exampleFileNameString );
		}

	protected byte readTestFile( String testFileNameString )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read file module isn't created yet" );

		return adminReadFile_.readTestFile( testFileNameString );
		}

	protected BufferedReader currentReadFile()
		{
		return ( fileList_ != null ?
				fileList_.currentReadFile() : null );
		}

	protected BufferedWriter currentWriteFile()
		{
		return ( fileList_ != null ?
				fileList_.currentWriteFile() : null );
		}

	protected CreateAndAssignResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		if( adminReadFile_ == null )
			return startCreateAndAssignResultError( 1, null, null, "The admin read file module isn't created yet" );

		return adminReadFile_.addSpecificationWithAuthorization( isAssignment, isCharacteristicFor, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );
		}

	protected FileResultType openFile( boolean isAddingSubPath, boolean isInfoFile, boolean isTestFile, boolean isReportingErrorIfFileDoesNotExist, String defaultSubpathString, String fileNameString, String writeSubpathString, String referenceSubpathString )
		{
		if( fileList_ == null )
			return startFileResultError( 1, null, "The file list isn't created yet" );

		return fileList_.openFile( isAddingSubPath, isInfoFile, isTestFile, isReportingErrorIfFileDoesNotExist, defaultSubpathString, fileNameString, writeSubpathString, referenceSubpathString );
		}

	protected FileResultType readInfoFile( boolean isReportingErrorIfFileDoesNotExist, String infoFileNameString )
		{
		if( adminReadFile_ == null )
			return startFileResultError( 1, null, "The admin read file module isn't created yet" );

		return adminReadFile_.readInfoFile( isReportingErrorIfFileDoesNotExist, infoFileNameString );
		}


	// Protected read sentence methods

	protected void clearLastActivatedWordOrderNr()
		{
		if( readList_ != null )
			readList_.clearLastActivatedWordOrderNr();
		}

	protected boolean hasAnyChangeBeenMadeByThisSentence()
		{
		return ( adminReadSentence_ != null ?
				// Default 'true' if admin read sentence module isn't created yet
				adminReadSentence_.hasAnyChangeBeenMadeByThisSentence() : true );
		}

	protected boolean hasFemaleUserSpecificationWord()
		{
		return ( adminReadSentence_ != null ?
				adminReadSentence_.hasFemaleUserSpecificationWord() : false );
		}

	protected boolean isUniqueUserRelation()
		{
		return ( adminReadSentence_ != null ?
				adminReadSentence_.isUniqueUserRelation() : false );
		}

	protected boolean isUserQuestion()
		{
		return ( adminReadSentence_ != null ?
				adminReadSentence_.isUserQuestion() : false );
		}

	protected boolean wasPreviousCommandUndoOrRedo()
		{
		return ( adminReadSentence_ != null ?
				adminReadSentence_.wasPreviousCommandUndoOrRedo() : false );
		}

	protected byte activateInactiveReadWords( short wordOrderNr )
		{
		if( readList_ == null )
			return startError( 1, null, null, "The read list isn't created yet" );

		return readList_.activateInactiveReadWords( wordOrderNr );
		}

	protected byte deleteReadItem( ReadItem readItem )
		{
		if( readList_ == null )
			return startError( 1, null, null, "The read list isn't created yet" );

		return readList_.deleteItem( readItem );
		}

	protected byte deleteWordItem( WordItem wordItem )
		{
		if( wordList_ == null )
			return startError( 1, null, null, "The word list isn't created yet" );

		return wordList_.deleteItem( wordItem );
		}

	protected byte processReadSentence( StringBuffer readStringBuffer )
		{
		if( adminReadSentence_ == null &&
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin read sentence module" );

		return adminReadSentence_.processReadSentence( readStringBuffer );
		}

	protected byte setGrammarParameter( boolean isValid, short startWordOrderNr, short endWordOrderNr, GrammarItem definitionGrammarItem )
		{
		if( readList_ == null )
			return startError( 1, null, null, "The read list isn't created yet" );

		return readList_.setGrammarParameter( isValid, startWordOrderNr, endWordOrderNr, definitionGrammarItem );
		}

	protected BoolResultType findMoreInterpretations()
		{
		if( readList_ == null )
			return startBoolResultError( 1, null, "The read list isn't created yet" );

		return readList_.findMoreInterpretations();
		}

	protected BoolResultType selectMatchingWordType( short currentWordOrderNr, short wordParameter, short wordTypeNr )
		{
		if( readList_ == null )
			return startBoolResultError( 1, null, "The read list isn't created yet" );

		return readList_.selectMatchingWordType( currentWordOrderNr, wordParameter, wordTypeNr );
		}

	protected DuplicateResultType checkForDuplicateCondition()
		{
		if( conditionList_ == null )
			return startDuplicateResultError( 1, "The condition list isn't created yet" );

		return conditionList_.checkForDuplicateCondition();
		}

	protected DuplicateResultType checkForDuplicateSelectionPart( boolean isAction, int duplicateConditionSentenceNr )
		{
		if( isAction )
			{
			if( actionList_ == null )
				return startDuplicateResultError( 1, "The action list isn't created yet" );

			return actionList_.checkForDuplicateSelectionPart( duplicateConditionSentenceNr );
			}

		if( alternativeList_ == null )
			return startDuplicateResultError( 1, "The alternative list isn't created yet" );

		return alternativeList_.checkForDuplicateSelectionPart( duplicateConditionSentenceNr );
		}


	// Protected new reasoning methods

	protected boolean hasSuggestiveQuestionAssumption()
		{
		return ( adminReasoningNew_ != null ?
				adminReasoningNew_.hasSuggestiveQuestionAssumption() : false );
		}

	protected byte askQuestions( boolean isCheckingForGapInKnowledge )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.askQuestions( isCheckingForGapInKnowledge );
		}

	protected byte addJustificationToSpecification( boolean hasFeminineOrMasculineProperNounEnding, boolean isForcingNewJustification, boolean isIncrementingOrderNr, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, SpecificationItem destinationSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.addJustificationToSpecification( hasFeminineOrMasculineProperNounEnding, false, isForcingNewJustification, isIncrementingOrderNr, assumptionJustificationTypeNr, conclusionJustificationTypeNr, destinationSpecificationItem, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, generalizationWordItem );
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, int generalizationContextNr, WordItem generalizationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, specificationCollectionNr, generalizationContextNr, generalizationWordItem );
		}

	protected byte drawProperNounPartOfConclusions( boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, boolean isArchivedAssignment, WordItem generalizationProperNounWordItem, WordItem specificationWordItem, WordItem spanishRelationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.drawProperNounPartOfConclusions( hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore, isArchivedAssignment, generalizationProperNounWordItem, specificationWordItem, spanishRelationWordItem );
		}

	protected byte drawReversibleConclusions( boolean hasCreatedSpecification, WordItem generalizationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.drawReversibleConclusions( hasCreatedSpecification, generalizationWordItem );
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isArchivedAssignment, boolean isSelfGenerated, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.drawSpecificationGeneralizationConclusion( isArchivedAssignment, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		if( adminReasoningNew_ == null &&
		// Create supporting module
		( adminReasoningNew_ = new AdminReasoningNew( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin reasoning new module" );

		return adminReasoningNew_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, userSpecificationItem );
		}

	protected byte makeGeneralizationAssumption( boolean hasCurrentlyCorrectedAssumptionByKnowledge, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.makeGeneralizationAssumption( hasCurrentlyCorrectedAssumptionByKnowledge, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, specificationWordItem );
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem createdSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem );
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startError( 1, null, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected CompoundResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		if( adminReasoningNew_ == null )
			return startCompoundResultError( 1, null, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_.drawCompoundSpecificationSubstitutionConclusion( specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
		}

	protected WordItem adjustedQuestionWordItem()
		{
		return ( adminReasoningNew_ != null ?
				adminReasoningNew_.adjustedQuestionWordItem() : null );
		}


	// Protected old reasoning methods

	protected byte checkForUnprocessedNegativeConclusion()
		{
		return ( adminReasoningOld_ != null ?
				adminReasoningOld_.checkForUnprocessedNegativeConclusion() : Constants.RESULT_OK );
		}

	protected byte drawNegativeConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem )
		{
		if( adminReasoningOld_ == null )
			return startError( 1, null, null, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_.drawNegativeConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem );
		}

	protected byte drawNegativeConclusionsFromAnsweredQuestions( boolean hasCurrentlyMoreSpecificNonExclusiveSpecification, SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminReasoningOld_ == null )
			return startError( 1, null, null, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_.drawNegativeConclusionsFromAnsweredQuestions( hasCurrentlyMoreSpecificNonExclusiveSpecification, userSpecificationItem, generalizationWordItem );
		}

	protected byte drawSimpleNegativeSpanishConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminReasoningOld_ == null )
			return startError( 1, null, null, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_.drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isArchivedAssignment, boolean isExclusiveSpecification, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminReasoningOld_ == null )
			return startError( 1, null, null, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isArchivedAssignment, isExclusiveSpecification, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUncountableGeneralizationNoun, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminReasoningOld_ == null )
			return startError( 1, null, null, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUncountableGeneralizationNoun, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		if( adminReasoningOld_ == null )
			return startError( 1, null, null, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_.makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, alternativeSecondarySpecificationItem, generalizationWordItem, relationWordItem );
		}

	protected byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminReasoningOld_ == null &&
		// Create supporting module
		( adminReasoningOld_ = new AdminReasoningOld( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin reasoning module" );

		return adminReasoningOld_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected SpecificationItem definitionSpecificationItem( WordItem specificationWordItem )
		{
		return ( adminReasoningOld_ != null ?
				adminReasoningOld_.definitionSpecificationItem( specificationWordItem ) : null );
		}

	protected WordItem oppositePossessiveDefinitionSpecificationWordItem( WordItem specificationWordItem )
		{
		return ( adminReasoningOld_ != null ?
				adminReasoningOld_.oppositePossessiveDefinitionSpecificationWordItem( specificationWordItem ) : null );
		}


	// Protected specification methods

	protected void initializeLinkedWord()
		{
		if( adminSpecification_ != null )
			adminSpecification_.initializeLinkedWord();
		}

	protected boolean hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore()
		{
		return ( adminSpecification_ != null ?
				adminSpecification_.hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore() : false );
		}

	protected byte addUserSpecification( boolean isAction, boolean isAssignment, boolean isCharacteristicFor, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, boolean isUniqueUserRelation, short imperativeVerbParameter, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short userAssumptionLevel, int generalizationContextNr, int specificationContextNr, ReadItem generalizationWordItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		if( adminSpecification_ == null )
			return startError( 1, null, null, "The admin specification module isn't created yet" );

		return adminSpecification_.addUserSpecification( isAction, isAssignment, isCharacteristicFor, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, imperativeVerbParameter, prepositionParameter, questionParameter, selectionLevel, selectionListNr, userAssumptionLevel, generalizationContextNr, specificationContextNr, generalizationWordItem, startSpecificationReadItem, endSpecificationReadItem, startRelationReadItem, endRelationReadItem );
		}

	protected byte assignSelectionSpecification( SelectionItem assignmentSelectionItem )
		{
		if( adminSpecification_ == null )
			return startError( 1, null, null, "The admin specification module isn't created yet" );

		return adminSpecification_.assignSelectionSpecification( assignmentSelectionItem );
		}

	protected byte assignSpecification( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminSpecification_ == null )
			return startError( 1, null, null, "The admin specification module isn't created yet" );

		return adminSpecification_.assignSpecification( false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, generalizationWordItem, specificationWordItem, null ).result;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminSpecification_ == null &&
		// Create supporting module
		( adminSpecification_ = new AdminSpecification( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin specification module" );

		return adminSpecification_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected byte createSelectionPart( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short selectionListNr, short imperativeVerbParameter, short prepositionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		switch( selectionListNr )
			{
			case Constants.ADMIN_CONDITION_LIST:
				if( conditionList_ == null )
					{
					// Create list
					if( ( conditionList_ = new SelectionList( Constants.ADMIN_CONDITION_LIST_SYMBOL, this ) ) == null )
						return startError( 1, null, null, "I failed to create the condition list" );

					GlobalVariables.adminConditionList = conditionList_;
					adminListArray_[Constants.ADMIN_CONDITION_LIST] = conditionList_;
					}

				return conditionList_.createSelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			case Constants.ADMIN_ACTION_LIST:
				if( actionList_ == null )
					{
					// Create list
					if( ( actionList_ = new SelectionList( Constants.ADMIN_ACTION_LIST_SYMBOL, this ) ) == null )
						return startError( 1, null, null, "I failed to create the action list" );

					GlobalVariables.adminActionList = actionList_;
					adminListArray_[Constants.ADMIN_ACTION_LIST] = actionList_;
					}

				return actionList_.createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			case Constants.ADMIN_ALTERNATIVE_LIST:
				if( alternativeList_ == null )
					{
					// Create list
					if( ( alternativeList_ = new SelectionList( Constants.ADMIN_ALTERNATIVE_LIST_SYMBOL, this ) ) == null )
						return startError( 1, null, null, "I failed to create the alternative list" );

					GlobalVariables.adminAlternativeList = alternativeList_;
					adminListArray_[Constants.ADMIN_ALTERNATIVE_LIST] = alternativeList_;
					}

				return alternativeList_.createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			default:
				return startError( 1, null, null, "The given selection list number is undefined" );
			}
		}

	protected SpecificationItem simpleUserSpecificationItem()
		{
		return ( adminSpecification_ != null ?
				adminSpecification_.simpleUserSpecificationItem() : null );
		}

	protected SpecificationItem userSpecificationItem()
		{
		return ( adminSpecification_ != null ?
				adminSpecification_.userSpecificationItem() : null );
		}

	protected ContextResultType getRelationContext( boolean isArchivedAssignment, boolean isPossessive, boolean isQuestion, boolean isUserSentence, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		if( adminSpecification_ == null )
			return startContextResultError( 1, null, "The admin specification module isn't created yet" );

		return adminSpecification_.getRelationContext( false, isArchivedAssignment, isPossessive, isQuestion, isUserSentence, generalizationWordItem, specificationWordItem, relationWordItem, startRelationReadItem );
		}

	protected CreateAndAssignResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNounEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isCharacteristicFor, boolean isEveryGeneralization, boolean isExclusiveGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkipDrawingSpanishAmbiguousSubstitutionConclusion, boolean isSpecific, boolean isSpecificationGeneralization, boolean isUncountableGeneralizationNoun, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminSpecification_ == null )
			return startCreateAndAssignResultError( 1, null, null, "The admin specification module isn't created yet" );

		return adminSpecification_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNounEnding, isAssignment, isArchivedAssignment, isCharacteristicFor, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isForcingNewJustification, isNegative, isPartOf, isPossessive, isUniqueUserRelation, isSkipDrawingSpanishAmbiguousSubstitutionConclusion, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, assumptionLevel, assumptionJustificationTypeNr, conclusionJustificationTypeNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected CreateAndAssignResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		if( adminReadFile_ == null )
			return startCreateAndAssignResultError( 1, null, null, "The admin read file module isn't created yet" );

		return adminReadFile_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );
		}


	// Protected write methods

	protected void initializeAdminWriteVariables()
		{
		if( adminWrite_ != null )
			adminWrite_.initializeAdminWriteVariables();
		}

	protected byte answerQuestions()
		{
		if( adminWrite_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWrite_.answerQuestions();
		}

	protected byte checkIntegrityOfStoredUserSentence( String readUserSentenceString )
		{
		if( adminWrite_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWrite_.checkIntegrityOfStoredUserSentence( readUserSentenceString );
		}

	protected byte prepareIntegrityCheckOfStoredUserSentence( boolean isChineseCurrentLanguage, SpecificationItem userSpecificationItem, String readUserSentenceString )
		{
		if( adminWrite_ == null &&
		// Create supporting module
		( adminWrite_ = new AdminWrite( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin write specification module" );

		return adminWrite_.prepareIntegrityCheckOfStoredUserSentence( isChineseCurrentLanguage, userSpecificationItem, readUserSentenceString );
		}

	protected byte writeJustificationReport( WordItem justificationWordItem )
		{
		if( adminWrite_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWrite_.writeSelfGeneratedInfo( false, true, true, true, true, justificationWordItem );
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions )
		{
		if( adminWrite_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWrite_.writeSelfGeneratedInfo( isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions );
		}

	protected byte writeInfoAboutWord( boolean isWritingCurrentSentenceOnly, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, boolean isWritingSpecificationInfo, boolean isWritingRelatedInfo, WordItem writeWordItem )
		{
		if( adminWrite_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWrite_.writeInfoAboutWord( isWritingCurrentSentenceOnly, isWritingUserSpecifications, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingUserQuestions, isWritingSelfGeneratedQuestions, isWritingSpecificationInfo, isWritingRelatedInfo, writeWordItem );
		}
	};

/*************************************************************************
 *	"Listen to me, all you in the distant lands!
 *	Pay attention, you who are far away!
 *	The Lord called me before my birth;
 *	from within the womb he called me by name." (Psalm 49:1)
 *************************************************************************/
