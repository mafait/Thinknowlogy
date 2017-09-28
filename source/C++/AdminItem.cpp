/*	Class:			AdminItem
 *	Parent class:	WordItem
 *	Grand parent:	Item
 *	Purpose:		To process tasks at administration level
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

#include "AdminImperative.cpp"
#include "AdminQuery.cpp"
#include "AdminReadCreateWords.cpp"
#include "AdminReadFile.cpp"
#include "AdminReadSentence.cpp"
#include "AdminReasoningNew.cpp"
#include "AdminReasoningOld.cpp"
#include "AdminSpecification.cpp"
#include "AdminWrite.cpp"

	// Private functions

	void AdminItem::rebuildQuickAccessListsInAllWords()
		{
		WordItem *currentWordItem;

		// Initialize the four starting points of the quick access lists
		commonVariables()->firstAssignmentWordItem = NULL;
		commonVariables()->firstCollectionWordItem = NULL;
		commonVariables()->firstContextWordItem = NULL;
		commonVariables()->firstPossessiveNounWordItem = NULL;
		commonVariables()->firstSpecificationWordItem = NULL;
		commonVariables()->firstUserProperNameWordItem = NULL;

		commonVariables()->lastCollectionWordItem = NULL;
		commonVariables()->lastContextWordItem = NULL;

		// Now scan all words if they should be added to the quick access lists
		if( ( currentWordItem = commonVariables()->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->rebuildQuickAccessWordLists();
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		}

	signed char AdminItem::assignLanguage( char *languageNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignLanguage";

		if( adminReadFile_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->assignLanguage( languageNameString );
		}

	signed char AdminItem::readStartupFile()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readStartupFile";

		if( adminReadFile_ == NULL &&
		// Create supporting module
		( adminReadFile_ = new AdminReadFile( this, commonVariables(), inputOutput_ ) ) == NULL )
			return startError( functionNameString, NULL, NULL, "I failed to create the admin read module" );

		if( fileList_ == NULL )
			{
			// Create list
			if( ( fileList_ = new FileList( commonVariables(), inputOutput_, this ) ) == NULL )
				return startError( functionNameString, NULL, NULL, "I failed to create the file list" );

			adminListArray_[ADMIN_FILE_LIST] = fileList_;
			}

		return adminReadFile_->readStartupFile();
		}

	signed char AdminItem::startup()
		{
		char titleString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "startup";

		// Initialize the random number generator with the time
		srand( (unsigned int)time( NULL ) );
		sprintf( titleString, "\n %s %s", PRODUCT_NAME, VERSION_NAME );

		if( inputOutput_->writeDiacriticalText( INPUT_OUTPUT_PROMPT_INFO, titleString ) != RESULT_OK )
			return startSystemError( functionNameString, NULL, NULL, "I failed to write the title text" );

		if( readStartupFile() != RESULT_OK )
			return startSystemError( functionNameString, NULL, NULL, "I failed to read the startup file" );

		if( startupLanguageNameString_ != NULL )
			{
			if( strlen( startupLanguageNameString_ ) == 0 )
				// At the first start, assign the first language as the startup language
				strcpy( startupLanguageNameString_, languageNameString( 1 ) );

			if( assignLanguage( startupLanguageNameString_ ) != RESULT_OK )
				// No show stopper. So, no return. Just continue.
				addError( functionNameString, NULL, NULL, "I failed to assign the language" );
			}

		if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_INITIAL_MESSAGE ) != RESULT_OK )
			// No show stopper. So, no return. Just continue.
			addError( functionNameString, NULL, NULL, "I failed to write the start interface text" );

		// Try to login without password
		if( login( NULL ) != RESULT_OK )
			return startSystemError( functionNameString, NULL, NULL, "I failed to login" );

		if( commonVariables()->currentUserNr <= NO_USER_NR )
			return startSystemError( functionNameString, NULL, NULL, "No user is logged in" );

		isSystemStartingUp_ = false;

		return RESULT_OK;
		}


	// Constructor

	AdminItem::AdminItem( char *startupLanguageNameString )
		{
		CommonVariables *_commonVariables;

		// Private constructed variables

		isSystemStartingUp_ = true;
		wasUndoOrRedoCommand_ = false;

		// Admin item supporting modules
		adminImperative_ = NULL;
		adminQuery_ = NULL;
		adminReadFile_ = NULL;
		adminReadCreateWords_ = NULL;
		adminReadSentence_ = NULL;
		adminReasoningNew_ = NULL;
		adminReasoningOld_ = NULL;
		adminSpecification_ = NULL;
		adminWrite_ = NULL;

		// Admin item lists
		fileList_ = NULL;
		readList_ = NULL;
		scoreList_ = NULL;
		wordList_ = NULL;
		conditionList_ = NULL;
		actionList_ = NULL;
		alternativeList_ = NULL;

		// Admin item lists array
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			adminListArray_[adminListNr] = NULL;

		// Private initialized variables

		// Point to the given string (in class Thinknowlogy)
		startupLanguageNameString_ = startupLanguageNameString;

		// Checking private initialized variables

		if( ( _commonVariables = new CommonVariables() ) != NULL )
			{
			initializeItemVariables( "AdminItem", _commonVariables, this );

			if( ( inputOutput_ = new InputOutput( _commonVariables ) ) != NULL )
				{
				if( startup() != RESULT_OK )
					addError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to start the administrator" );
				}
			else
				startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the input-output module" );
			}
		else
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the common variables module" );
		}

	AdminItem::~AdminItem()
		{
		if( commonVariables() != NULL )
			delete commonVariables();
		if( inputOutput_ != NULL )
			delete inputOutput_;

		if( adminImperative_ != NULL )
			delete adminImperative_;
		if( adminQuery_ != NULL )
			delete adminQuery_;
		if( adminReadFile_ != NULL )
			delete adminReadFile_;
		if( adminReadCreateWords_ != NULL )
			delete adminReadCreateWords_;
		if( adminReadSentence_ != NULL )
			delete adminReadSentence_;
		if( adminReasoningNew_ != NULL )
			delete adminReasoningNew_;
		if( adminReasoningOld_ != NULL )
			delete adminReasoningOld_;
		if( adminSpecification_ != NULL )
			delete adminSpecification_;
		if( adminWrite_ != NULL )
			delete adminWrite_;

		if( fileList_ != NULL )
			delete fileList_;
		if( readList_ != NULL )
			delete readList_;
		if( scoreList_ != NULL )
			delete scoreList_;
		if( conditionList_ != NULL )
			delete conditionList_;
		if( actionList_ != NULL )
			delete actionList_;
		if( alternativeList_ != NULL )
			delete alternativeList_;
		if( wordList_ != NULL )
			delete wordList_;
		}


	// Protected common functions

	void AdminItem::clearTemporaryAdminLists()
		{
		wasUndoOrRedoCommand_ = false;

		deleteTemporaryReadList();
		deleteTemporaryScoreList();
		}

	void AdminItem::initializeAdminVariables()
		{
		if( adminReadFile_ != NULL )
			adminReadFile_->initializeAdminReadFileVariables();

		if( adminReasoningNew_ != NULL )
			adminReasoningNew_->initializeAdminReasoningNewVariables();

		if( adminReasoningOld_ != NULL )
			adminReasoningOld_->initializeAdminReasoningOldVariables();

		if( adminSpecification_ != NULL )
			adminSpecification_->initializeAdminSpecificationVariables();
		}

	bool AdminItem::isSystemStartingUp()
		{
		return isSystemStartingUp_;
		}

	bool AdminItem::wasUndoOrRedoCommand()
		{
		return wasUndoOrRedoCommand_;
		}

	unsigned int AdminItem::highestContextNrInAllContextWords()
		{
		unsigned int tempContextNr;
		unsigned int highestContextNr = NO_CONTEXT_NR;
		WordItem *currentContextWordItem;

		if( ( currentContextWordItem = commonVariables()->firstContextWordItem ) != NULL )
			{
			// Do for all context words
			do	{
				if( ( tempContextNr = currentContextWordItem->highestContextNrInWord() ) > highestContextNr )
					highestContextNr = tempContextNr;
				}
			while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
			}

		return highestContextNr;
		}

	unsigned int AdminItem::highestFoundSentenceNr( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int maxSentenceNr )
		{
		unsigned int highestFoundSentenceNr = NO_SENTENCE_NR;
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			highestFoundSentenceNr = wordList_->highestFoundSentenceNrInWordList( isIncludingDeletedItems, isIncludingTemporaryLists, maxSentenceNr );

		// Admin lists
		for( unsigned short adminListNr = 0; ( adminListNr < NUMBER_OF_ADMIN_LISTS && highestFoundSentenceNr < maxSentenceNr ); adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&

			( isIncludingTemporaryLists ||
			!currentAdminList->isTemporaryList() ) &&

			// For efficiency, only select lists with higher sentence number
			currentAdminList->highestSentenceNrInList() > highestFoundSentenceNr )
				highestFoundSentenceNr = currentAdminList->highestFoundSentenceNrInList( isIncludingDeletedItems, highestFoundSentenceNr, maxSentenceNr );
			}

		return highestFoundSentenceNr;
		}

	char AdminItem::adminListChar( unsigned short adminListNr )
		{
		switch( adminListNr )
			{
			case ADMIN_FILE_LIST:
				return ADMIN_FILE_LIST_SYMBOL;

			case ADMIN_READ_LIST:
				return ADMIN_READ_LIST_SYMBOL;

			case ADMIN_SCORE_LIST:
				return ADMIN_SCORE_LIST_SYMBOL;

			case ADMIN_WORD_LIST:
				return ADMIN_WORD_LIST_SYMBOL;

			case ADMIN_CONDITION_LIST:
				return ADMIN_CONDITION_LIST_SYMBOL;

			case ADMIN_ACTION_LIST:
				return ADMIN_ACTION_LIST_SYMBOL;

			case ADMIN_ALTERNATIVE_LIST:
				return ADMIN_ALTERNATIVE_LIST_SYMBOL;
			}

		return SYMBOL_QUESTION_MARK;
		}

	char *AdminItem::currentLanguageNameString()
		{
		if( commonVariables() != NULL )
			return languageNameString( commonVariables()->currentLanguageNr );

		return NULL;
		}

	char *AdminItem::startupLanguageNameString()
		{
		return startupLanguageNameString_;
		}


	// Protected error functions

	signed char AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		if( commonVariables() != NULL &&
		inputOutput_ != NULL )
			inputOutput_->displayError( adminListChar( adminListNr ), ( moduleNameString == NULL ? classNameString() : moduleNameString ), ( moduleNameString == NULL ? parentClassNameString() : NULL ), NULL, functionNameString, errorString );
		else
			fprintf( stderr, "\nClass:\t%s\nParent class:\t%s\nFunction:\t%s\nError:\t\t%s.\n", classNameString(), parentClassNameString(), functionNameString, errorString );

		return ( commonVariables() == NULL ? RESULT_ERROR : commonVariables()->result );
		}

	signed char AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		sprintf( tempString, "%s%u", errorString, number );
		return addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	signed char AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	signed char AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	signed char AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, errorString );

		if( commonVariables() != NULL )
		commonVariables()->result = RESULT_ERROR;

		return RESULT_ERROR;
		}

	signed char AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number )
		{
		sprintf( tempString, "%s%u", errorString, number );
		return startErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	signed char AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	signed char AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 )
		{
		sprintf( tempString, "%s%u%s%u", errorString1, number1, errorString2, number2 );
		return startErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}


	// Protected database connection functions
/*
	signed char AdminItem::storeChangesInFutureDatabase()
		{
		List *currentAdminList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		// Word lists
		if( wordList_ != NULL &&
		wordList_->storeChangesInFutureDatabaseInWordList() != RESULT_OK )
			return addError( functionNameString, NULL, NULL, "I failed to store changes of my words list in future database" );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentWordList = wordListArray_[wordListNr] ) != NULL &&
			// No need to store items of temporary lists
			!currentAdminList->isTemporaryList() &&
			currentAdminList->storeChangesInFutureDatabaseInList() != RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, functionNameString, NULL, "I failed to store changes in future database of an admin list" );
			}

		return RESULT_OK;
		}
*/

	// Protected imperative functions

	void AdminItem::clearConditionChecksForSolving( unsigned short selectionLevel, unsigned int conditionSentenceNr )
		{
		if( conditionList_ != NULL )
			conditionList_->clearConditionChecksForSolving( selectionLevel, conditionSentenceNr );
		}

	void AdminItem::deleteTemporaryScoreList()
		{
		if( scoreList_ != NULL )
			scoreList_->deleteTemporaryList();
		}

	bool AdminItem::hasRequestedRestart()
		{
		if( adminImperative_ != NULL )
			return adminImperative_->hasRequestedRestart();

		return false;
		}

	bool AdminItem::hasScoreList()
		{
		return( scoreList_ != NULL );
		}

	unsigned int AdminItem::nPossibilities()
		{
		if( scoreList_ != NULL )
			return scoreList_->nPossibilities();

		return 0;
		}

	signed char AdminItem::changeAction( SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "changeAction";

		if( scoreList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The score list isn't created yet" );

		return scoreList_->changeAction( actionSelectionItem );
		}

	signed char AdminItem::createScoreItem( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createScoreItem";

		if( scoreList_ == NULL )
			{
			// Create list
			if( ( scoreList_ = new ScoreList( commonVariables(), inputOutput_, this ) ) == NULL )
				return startError( functionNameString, NULL, NULL, "I failed to create the admin solve score list" );

			adminListArray_[ADMIN_SCORE_LIST] = scoreList_;
			}

		return scoreList_->createScoreItem( isChecked, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore, referenceSelectionItem );
		}

	signed char AdminItem::deleteScores()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteScores";

		if( scoreList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The score list isn't created yet" );

		return scoreList_->deleteScores();
		}

	signed char AdminItem::executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeVerbParameter, unsigned short executionNounWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgressLevel, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperative";

		if( adminImperative_ == NULL )
			{
			// Check before getting variables
			if( adminReadFile_ == NULL )
				return startError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

			// Create supporting module
			if( ( adminImperative_ = new AdminImperative( this, commonVariables(), inputOutput_, adminReadFile_->predefinedAdjectiveBusyWordItem(), adminReadFile_->predefinedAdjectiveDoneWordItem(), adminReadFile_->predefinedAdjectiveInvertedWordItem(), adminReadFile_->predefinedNounSolveLevelWordItem(), adminReadFile_->predefinedNounSolveMethodWordItem(), adminReadFile_->predefinedNounSolveStrategyWordItem() ) ) == NULL )
				return startError( functionNameString, NULL, NULL, "I failed to create the admin imperative module" );
			}

		return adminImperative_->executeImperative( isInitializeVariables, executionListNr, imperativeVerbParameter, executionNounWordParameter, specificationWordTypeNr, endSolveProgressLevel, executionString, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem, executionSelectionItem, actionSelectionItem );
		}

	signed char AdminItem::executeSelections()
		{
		if( adminImperative_ != NULL )
			return adminImperative_->executeSelection( MAX_PROGRESS, NULL );

		// There are no selections yet. Just return: Okay.
		return RESULT_OK;
		}

	signed char AdminItem::redoLastUndoneSentence()
		{
		List *currentAdminList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoLastUndoneSentence";

		wasUndoOrRedoCommand_ = true;

		if( highestFoundSentenceNr( true, false, commonVariables()->currentSentenceNr ) == commonVariables()->currentSentenceNr )
			{
			// Important: Redo admin lists first, and the words after that
			// Because redoing admin words list might redo words
			for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
				// No need to redo items of temporary lists
				!currentAdminList->isTemporaryList() &&
				currentAdminList->redoCurrentSentenceInList() != RESULT_OK )
					return addErrorWithAdminListNr( adminListNr, functionNameString, NULL, "I failed to redo the current sentence in an admin list" );
				}

			// Word lists
			if( wordList_ != NULL )
				wordList_->redoCurrentSentenceInWordList();

			if( inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_START, commonVariables()->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_REDONE_SENTENCE_NR_END ) != RESULT_OK )
				return addError( functionNameString, NULL, NULL, "I failed to write the 'I have redone' interface notification" );

			rebuildQuickAccessListsInAllWords();
			}
		else
			{
			// No sentences found to redo
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_REDO ) != RESULT_OK )
				return addError( functionNameString, NULL, NULL, "I failed to write the 'no sentences to redo' interface notification" );

			decrementCurrentSentenceNr();
			}

		return RESULT_OK;
		}

	signed char AdminItem::undoLastSentence()
		{
		unsigned int firstSentenceNr = firstSentenceNrOfCurrentUser();
		List *currentAdminList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoLastSentence";

		wasUndoOrRedoCommand_ = true;

		// Remove the deleted read items of this undo sentence
		if( commonVariables()->currentSentenceNr > firstSentenceNr )
			{
			decrementCurrentSentenceNr();

			// Word lists
			if( wordList_ != NULL )
				wordList_->undoCurrentSentenceInWordList();

			// Admin lists
			for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
				{
				if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
				// No need to redo items of temporary lists
				!currentAdminList->isTemporaryList() &&
				currentAdminList->undoCurrentSentenceInList() != RESULT_OK )
					return addErrorWithAdminListNr( adminListNr, functionNameString, NULL, "I failed to undo the current sentence" );
				}

			if( inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_START, commonVariables()->currentSentenceNr, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_UNDONE_SENTENCE_NR_END ) != RESULT_OK )
				return addError( functionNameString, NULL, NULL, "I failed to write the 'I have undone' interface notification" );

			rebuildQuickAccessListsInAllWords();
			}
		else
			{
			// No sentences found to undo
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_NO_SENTENCES_TO_UNDO ) != RESULT_OK )
				return addError( functionNameString, NULL, NULL, "I failed to write the 'no sentences to undo' interface notification" );
			}

		if( commonVariables()->currentSentenceNr >= firstSentenceNr )
			decrementCurrentSentenceNr();

		return RESULT_OK;
		}

	ScoreItem *AdminItem::firstPossibility()
		{
		if( scoreList_ != NULL )
			return scoreList_->firstPossibility();

		return NULL;
		}

	SelectionItem *AdminItem::executionStartEntry( bool isAction, unsigned short executionLevel, unsigned int executionSentenceNr )
		{
		if( isAction )
			{
			if( actionList_ != NULL )
				return actionList_->executionStartEntry( executionLevel, executionSentenceNr );
			}
		else
			{
			if( alternativeList_ != NULL )
				return alternativeList_->executionStartEntry( executionLevel, executionSentenceNr );
			}

		return NULL;
		}

	SelectionItem *AdminItem::firstConditionItem()
		{
		if( conditionList_ != NULL )
			return conditionList_->firstActiveSelectionItem();

		return NULL;
		}

	SelectionItem *AdminItem::firstConditionSelectionItem( unsigned int conditionSentenceNr )
		{
		if( conditionList_ != NULL )
			return conditionList_->firstConditionSelectionItem( conditionSentenceNr );

		return NULL;
		}

	SelectionItem *AdminItem::firstSelectionItem( bool isAction, WordItem *solveWordItem )
		{
		if( isAction )
			{
			if( actionList_ != NULL )
				return actionList_->firstSelectionItem( solveWordItem );
			}
		else
			{
			if( alternativeList_ != NULL )
				return alternativeList_->firstSelectionItem( solveWordItem );
			}

		return NULL;
		}

	BoolResultType AdminItem::checkScores( bool isInverted, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkScores";

		if( scoreList_ == NULL )
			return startBoolResultError( functionNameString, NULL, "The score list isn't created yet" );

		return scoreList_->checkScores( isInverted, solveStrategyParameter, oldSatisfiedScore, newSatisfiedScore, oldDissatisfiedScore, newDissatisfiedScore, oldNotBlockingScore, newNotBlockingScore, oldBlockingScore, newBlockingScore );
		}

	BoolResultType AdminItem::findScore( bool isPreparingSort, SelectionItem *findScoreItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findScore";

		if( scoreList_ == NULL )
			return startBoolResultError( functionNameString, NULL, "The score list isn't created yet" );

		return scoreList_->findScore( isPreparingSort, findScoreItem );
		}

	SelectionResultType AdminItem::getBestSelection( bool isCurrentlyTesting, unsigned short solveStrategyParameter )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getBestSelection";

		if( scoreList_ == NULL )
			return startSelectionResultError( functionNameString, "The score list isn't created yet" );

		return scoreList_->getBestSelection( isCurrentlyTesting, solveStrategyParameter );
		}


	// Protected query functions

	void AdminItem::clearQuerySelections()
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->clearQuerySelectionsInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL )
				currentAdminList->clearQuerySelectionsInList();
			}
		}

	void AdminItem::countQuery()
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->countQueryInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL )
				currentAdminList->countQueryInList();
			}
		}

	void AdminItem::itemQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->itemQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL )
				currentAdminList->itemQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr );
			}
		}

	void AdminItem::parameterQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter )
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->parameterQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL )
				currentAdminList->parameterQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryParameter );
			}
		}

	void AdminItem::listQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString )
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->listQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL )
				currentAdminList->listQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryListString );
			}
		}

	void AdminItem::wordTypeQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr )
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->wordTypeQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL )
				currentAdminList->wordTypeQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryWordTypeNr );
			}
		}

	signed char AdminItem::displayQueryResult( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		List *currentAdminList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "displayQueryResult";

		// Word lists
		if( wordList_ != NULL &&
		wordList_->displayQueryResultInWordList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
			return addError( functionNameString, NULL, NULL, "I failed to display the query result in my words list" );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
			currentAdminList->displayQueryResultInList( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, functionNameString, NULL, "I failed to display the query result of an admin list" );
			}

		return RESULT_OK;
		}

	signed char AdminItem::stringQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString )
		{
		List *currentAdminList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQuery";

		// Word lists
		if( wordList_ != NULL &&
		wordList_->stringQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != RESULT_OK )
			return addError( functionNameString, NULL, NULL, "I failed to query strings in my words list" );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
			currentAdminList->stringQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, queryString ) != RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, functionNameString, NULL, "I failed to query strings in an admin list" );
			}

		return RESULT_OK;
		}

	signed char AdminItem::wordQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQuery";

		// Word lists
		if( wordList_ != NULL &&
		wordList_->wordQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, wordNameString ) != RESULT_OK )
			return addError( functionNameString, NULL, NULL, "I failed to query the words in my words list" );

		return RESULT_OK;
		}

	signed char AdminItem::wordReferenceQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString )
		{
		List *currentAdminList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQuery";

		// Word lists
		if( wordList_ != NULL &&
		wordList_->wordReferenceQueryInWordList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, wordReferenceNameString ) != RESULT_OK )
			return addError( functionNameString, NULL, NULL, "I failed to query word references in my words list" );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
			currentAdminList->wordReferenceQueryInList( isSelectingOnFind, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, false, wordReferenceNameString ) != RESULT_OK )
				return addErrorWithAdminListNr( adminListNr, functionNameString, NULL, "I failed to query word references in an admin list" );
			}

		return RESULT_OK;
		}

	QueryResultType AdminItem::executeQuery( bool isSuppressingMessage, bool isReturningToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, size_t queryCommandStringStartPosition, char *queryString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeQuery";

		if( adminQuery_ == NULL &&
		// Create supporting module
		( adminQuery_ = new AdminQuery( this, commonVariables(), inputOutput_ ) ) == NULL )
			return startQueryResultError( functionNameString, NULL, "I failed to create the admin query module" );

		return adminQuery_->executeQuery( isSuppressingMessage, isReturningToPosition, isWritingQueryResult, promptTypeNr, queryCommandStringStartPosition, queryString );
		}


	// Protected read create words functions

	void AdminItem::deleteTemporaryReadList()
		{
		if( readList_ != NULL )
			readList_->deleteTemporaryList();
		}

	bool AdminItem::hasFoundDifferentParameter()
		{
		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->hasFoundDifferentParameter();

		return false;
		}

	bool AdminItem::isImperativeSentence()
		{
		if( readList_ != NULL )
			return readList_->isImperativeSentence();

		return false;
		}

	unsigned short AdminItem::lastCreatedWordOrderNr()
		{
		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->lastCreatedWordOrderNr();

		return NO_ORDER_NR;
		}

	signed char AdminItem::createReadItem( unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *readString, WordItem *readWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadItem";

		if( readList_ == NULL )
			{
			// Create list
			if( ( readList_ = new ReadList( commonVariables(), inputOutput_, this ) ) == NULL )
				return startError( functionNameString, NULL, "I failed to create the admin read list" );

			adminListArray_[ADMIN_READ_LIST] = readList_;
			}

		return readList_->createReadItem( wordOrderNr, wordParameter, wordTypeNr, readStringLength, readString, readWordItem );
		}

	signed char AdminItem::deleteReadItemsWithNonMatchingMultipleWordPart( unsigned short wordOrderNr, char *sentenceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteReadItemsWithNonMatchingMultipleWordPart";

		if( readList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The read list isn't created yet" );

		return readList_->deleteReadItemsWithNonMatchingMultipleWordPart( wordOrderNr, sentenceString );
		}

	ReadItem *AdminItem::firstActiveReadItem()
		{
		if( readList_ != NULL )
			return readList_->firstActiveReadItem();

		return NULL;
		}

	ReadItem *AdminItem::firstInactiveReadItem()
		{
		if( readList_ != NULL )
			return readList_->firstInactiveReadItem();

		return NULL;
		}

	ReadItem *AdminItem::nextReadListItem()
		{
		if( readList_ != NULL )
			return readList_->nextReadListItem();

		return NULL;
		}

	BoolResultType AdminItem::createReadWords( char *readUserSentenceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWords";

		if( adminReadCreateWords_ == NULL )
			return startBoolResultError( functionNameString, NULL, "The admin read create words module isn't created yet" );

		return adminReadCreateWords_->createReadWords( readUserSentenceString );
		}

	ReadWordResultType AdminItem::readWordFromString( bool isCheckingForGrammarDefinition, bool isMergedWord, bool isSkippingTextString, size_t startWordPosition, size_t minimumStringLength, char *wordString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readWordFromString";

		if( adminReadCreateWords_ == NULL )
			return startReadWordResultError( functionNameString, NULL, "The admin read create words module isn't created yet" );

		return adminReadCreateWords_->readWordFromString( isCheckingForGrammarDefinition, isMergedWord, isSkippingTextString, startWordPosition, minimumStringLength, wordString );
		}

	ShortResultType AdminItem::getNumberOfReadWordReferences( unsigned short wordTypeNr, WordItem *readWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getNumberOfReadWordReferences";

		if( readList_ == NULL )
			return startShortResultError( functionNameString, NULL, "The read list isn't created yet" );

		return readList_->getNumberOfReadWordReferences( wordTypeNr, readWordItem );
		}

	WordResultType AdminItem::addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordLength, char *wordString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWord";

		if( adminReadCreateWords_ == NULL &&
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this, commonVariables(), inputOutput_ ) ) == NULL )
			return startWordResultError( functionNameString, NULL, "I failed to create the admin read create words module" );

		return adminReadCreateWords_->addWord( isLanguageWord, isMultipleWord, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, wordParameter, wordTypeNr, wordLength, wordString );
		}

	WordResultType AdminItem::createWordItem( bool isLanguageWord, unsigned short wordParameter )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordItem";

		if( wordList_ == NULL )
			{
			// Create list
			if( ( wordList_ = new WordList( commonVariables(), inputOutput_, this ) ) == NULL )
				return startWordResultError( functionNameString, NULL, "I failed to create the word list" );

			adminListArray_[ADMIN_WORD_LIST] = wordList_;
			}

		return wordList_->createWordItem( isLanguageWord, wordParameter );
		}

	WordResultType AdminItem::findWordTypeInAllWords( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString, WordItem *previousWordItem )
		{
		WordResultType wordResult;

		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->findWordTypeInAllWords( isCheckingAllLanguages, wordTypeNr, wordTypeString, previousWordItem );

		return wordResult;
		}


	// Protected read file functions

	void AdminItem::decrementCurrentSentenceNr()
		{
		if( commonVariables()->currentSentenceNr > NO_SENTENCE_NR )
			{
			commonVariables()->currentSentenceNr--;
			// Necessary after changing current sentence number
			commonVariables()->currentSentenceItemNr = highestCurrentSentenceItemNr();
			}
		}

	void AdminItem::decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->decrementItemNrRangeInWordList( decrementSentenceNr, startDecrementItemNr, decrementOffset );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
			// For efficiency, only select lists with decrement sentence number of higher
			currentAdminList->highestSentenceNrInList() >= decrementSentenceNr )
				currentAdminList->decrementItemNrRangeInList( decrementSentenceNr, startDecrementItemNr, decrementOffset );
			}
		}

	void AdminItem::decrementSentenceNrs( unsigned int startSentenceNr )
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->decrementSentenceNrsInWordList( startSentenceNr );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
			// For efficiency, only select lists with start sentence number of higher
			currentAdminList->highestSentenceNrInList() >= startSentenceNr )
				currentAdminList->decrementSentenceNrsInList( startSentenceNr );
			}
		}

	void AdminItem::deleteSentences( unsigned int lowestSentenceNr )
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->deleteSentencesInWordList( lowestSentenceNr );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
			// For efficiency, only select lists with lowest sentence number of higher
			currentAdminList->highestSentenceNrInList() >= lowestSentenceNr )
				currentAdminList->deleteSentencesInList( lowestSentenceNr );
			}

		rebuildQuickAccessListsInAllWords();
		}

	void AdminItem::removeFirstRangeOfDeletedItems()
		{
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			wordList_->removeFirstRangeOfDeletedItemsInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; ( adminListNr < NUMBER_OF_ADMIN_LISTS && commonVariables()->nDeletedItems == 0 ); adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL )
				currentAdminList->removeFirstRangeOfDeletedItemsInList();
			}
		}

	bool AdminItem::isCurrentFileTestFile()
		{
		if( fileList_ != NULL )
			return fileList_->isCurrentFileTestFile();

		return false;
		}

	bool AdminItem::isCurrentlyTesting()
		{
		if( fileList_ != NULL )
			return fileList_->isCurrentlyTesting();

		return false;
		}

	bool AdminItem::isDisplayingLine()
		{
		if( !isSystemStartingUp_ &&
		fileList_ != NULL )
			return fileList_->isDisplayingLine();

		return false;
		}

	unsigned int AdminItem::currentFileSentenceNr()
		{
		if( fileList_ != NULL )
			return fileList_->currentFileSentenceNr();

		return NO_SENTENCE_NR;
		}

	unsigned int AdminItem::firstSentenceNrOfCurrentUser()
		{
		if( adminReadFile_ != NULL )
			return adminReadFile_->firstSentenceNrOfCurrentUser();

		return NO_SENTENCE_NR;
		}

	unsigned int AdminItem::highestCurrentSentenceItemNr()
		{
		unsigned int currentSentenceNr = commonVariables()->currentSentenceNr;
		unsigned int highestItemNr = NO_ITEM_NR;
		List *currentAdminList;

		// Word lists
		if( wordList_ != NULL )
			highestItemNr = wordList_->highestCurrentSentenceItemNrInWordList( currentSentenceNr, highestItemNr );

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( ( currentAdminList = adminListArray_[adminListNr] ) != NULL &&
			// For efficiency, only select lists with current sentence number or higher
			currentAdminList->highestSentenceNrInList() >= currentSentenceNr )
				highestItemNr = currentAdminList->highestCurrentSentenceItemNrInList( currentSentenceNr, highestItemNr );
			}

		return highestItemNr;
		}

	signed char AdminItem::closeCurrentFile( FileItem *closeFileItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "closeCurrentFile";

		if( fileList_ == NULL )
			return startError( functionNameString, NULL, "The file list isn't created yet" );

		return fileList_->closeCurrentFile( closeFileItem );
		}

	signed char AdminItem::compareOutputFileAgainstReferenceFile( char *testFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareOutputFileAgainstReferenceFile";

		if( adminReadFile_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->compareOutputFileAgainstReferenceFile( testFileNameString );
		}

	signed char AdminItem::interact()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "interact";

		if( commonVariables() == NULL ||
		commonVariables()->result == RESULT_SYSTEM_ERROR )
			return RESULT_SYSTEM_ERROR;

		if( adminReadFile_ == NULL )
			return startSystemError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		if( adminReadFile_->readAndExecute() == RESULT_SYSTEM_ERROR )
			return startSystemError( functionNameString, NULL, NULL, "I failed to read and execute" );

		// Ignore RESULT_ERROR. Application will only exit on system error
		commonVariables()->result = RESULT_OK;
		// Application will not exit on a normal error
		return RESULT_OK;
		}

	signed char AdminItem::login( WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "login";

		if( adminReadFile_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->login( specificationWordItem );
		}

	signed char AdminItem::readExampleFile( char *exampleFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readExampleFile";

		if( adminReadFile_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->readExampleFile( exampleFileNameString );
		}

	signed char AdminItem::readTestFile( char *testFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readTestFile";

		if( adminReadFile_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->readTestFile( testFileNameString );
		}

	FILE *AdminItem::currentReadFile()
		{
		if( fileList_ != NULL )
			return fileList_->currentReadFile();

		return NULL;
		}

	FILE *AdminItem::currentWriteFile()
		{
		if( fileList_ != NULL )
			return fileList_->currentWriteFile();

		return NULL;
		}

	CreateAndAssignResultType AdminItem::addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( adminReadFile_ == NULL )
			return startCreateAndAssignResultError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );
		}

	FileResultType AdminItem::openFile( bool isAddingSubPath, bool isInfoFile, bool isTestFile, bool isReportingErrorIfFileDoesNotExist, const char *defaultSubpathString, const char *fileNameString, const char *writeSubpathString, const char *referenceSubpathString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "openFile";

		if( fileList_ == NULL )
			return startFileResultError( functionNameString, NULL, "The file list isn't created yet" );

		return fileList_->openFile( isAddingSubPath, isInfoFile, isTestFile, isReportingErrorIfFileDoesNotExist, defaultSubpathString, fileNameString, writeSubpathString, referenceSubpathString );
		}

	FileResultType AdminItem::readInfoFile( bool isReportingErrorIfFileDoesNotExist, char *infoFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readInfoFile";

		if( adminReadFile_ == NULL )
			return startFileResultError( functionNameString, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->readInfoFile( isReportingErrorIfFileDoesNotExist, infoFileNameString );
		}


	// Protected read sentence functions

	void AdminItem::clearLastActivatedWordOrderNr()
		{
		if( readList_ != NULL )
			readList_->clearLastActivatedWordOrderNr();
		}

	bool AdminItem::hasAnyChangeBeenMadeByThisSentence()
		{
		if( adminReadSentence_ != NULL )
			return adminReadSentence_->hasAnyChangeBeenMadeByThisSentence();

		// Default if admin cleanup module isn't created yet
		return true;
		}

	bool AdminItem::hasFemaleUserSpecificationWord()
		{
		if( adminReadSentence_ != NULL )
			return adminReadSentence_->hasFemaleUserSpecificationWord();

		return false;
		}

	bool AdminItem::isUserQuestion()
		{
		if( adminReadSentence_ != NULL )
			return adminReadSentence_->isUserQuestion();

		return false;
		}

	bool AdminItem::wasPreviousCommandUndoOrRedo()
		{
		if( adminReadSentence_ != NULL )
			return adminReadSentence_->wasPreviousCommandUndoOrRedo();

		return false;
		}

	signed char AdminItem::activateInactiveReadWords( unsigned short wordOrderNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "activateInactiveReadWords";

		if( readList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The read list isn't created yet" );

		return readList_->activateInactiveReadWords( wordOrderNr );
		}

	signed char AdminItem::deleteReadItem( ReadItem *readItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteReadItem";

		if( readList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The read list isn't created yet" );

		return readList_->deleteItem( readItem );
		}

	signed char AdminItem::deleteWordItem( WordItem *wordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteWordItem";

		if( wordList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The word list isn't created yet" );

		return wordList_->deleteItem( wordItem );
		}

	signed char AdminItem::processReadSentence( char *readString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "processReadSentence";

		if( adminReadSentence_ == NULL &&
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this, commonVariables(), inputOutput_ ) ) == NULL )
			return startError( functionNameString, NULL, NULL, "I failed to create the admin read sentence module" );

		return adminReadSentence_->processReadSentence( readString );
		}

	signed char AdminItem::removeDuplicateSelection()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "removeDuplicateSelection";

		if( conditionList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The condition list isn't created yet" );

		if( actionList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The action list isn't created yet" );

		if( conditionList_->deleteActiveItemsWithCurrentSentenceNr() != RESULT_OK )
			return addError( functionNameString, NULL, NULL, "I failed to remove the condition of a selection" );

		if( actionList_->deleteActiveItemsWithCurrentSentenceNr() != RESULT_OK )
			return addError( functionNameString, NULL, NULL, "I failed to remove the action of a selection" );

		if( alternativeList_ != NULL &&
		alternativeList_->deleteActiveItemsWithCurrentSentenceNr() != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to remove the alternative of a selection" );

		return RESULT_OK;
		}

	signed char AdminItem::setGrammarParameter( bool isValid, unsigned short startWordOrderNr, unsigned short endWordOrderNr, GrammarItem *definitionGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "setGrammarParameter";

		if( readList_ == NULL )
			return startError( functionNameString, NULL, NULL, "The read list isn't created yet" );

		return readList_->setGrammarParameter( isValid, startWordOrderNr, endWordOrderNr, definitionGrammarItem );
		}

	BoolResultType AdminItem::findMoreInterpretations()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMoreInterpretations";

		if( readList_ == NULL )
			return startBoolResultError( functionNameString, NULL, "The read list isn't created yet" );

		return readList_->findMoreInterpretations();
		}

	BoolResultType AdminItem::selectMatchingWordType( unsigned short currentWordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "selectMatchingWordType";

		if( readList_ == NULL )
			return startBoolResultError( functionNameString, NULL, "The read list isn't created yet" );

		return readList_->selectMatchingWordType( currentWordOrderNr, wordParameter, wordTypeNr );
		}

	DuplicateResultType AdminItem::checkDuplicateCondition()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkDuplicateCondition";

		if( conditionList_ == NULL )
			return startDuplicateResultError( functionNameString, "The condition list isn't created yet" );

		return conditionList_->checkDuplicateCondition();
		}

	DuplicateResultType AdminItem::checkDuplicateSelectionPart( bool isAction, unsigned int duplicateConditionSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkDuplicateSelectionPart";

		if( isAction )
			{
			if( actionList_ == NULL )
				return startDuplicateResultError( functionNameString, "The action list isn't created yet" );

			return actionList_->checkDuplicateSelectionPart( duplicateConditionSentenceNr );
			}

		if( alternativeList_ == NULL )
			return startDuplicateResultError( functionNameString, "The alternative list isn't created yet" );

		return alternativeList_->checkDuplicateSelectionPart( duplicateConditionSentenceNr );
		}


	// Protected new reasoning functions

	signed char AdminItem::askQuestions()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "askQuestions";

		if( adminReasoningNew_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_->askQuestions();
		}

	signed char AdminItem::correctSuggestiveAssumptionsByOppositeQuestion( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusions";

		if( adminReasoningNew_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_->correctSuggestiveAssumptionsByOppositeQuestion( isArchivedAssignment, isNegative, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );
		}

	signed char AdminItem::drawPossessiveReversibleConclusions( WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusions";

		if( adminReasoningNew_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning new module isn't created yet" );

		return adminReasoningNew_->drawPossessiveReversibleConclusions( generalizationWordItem );
		}

	signed char AdminItem::drawProperNamePartOfConclusions( bool hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, bool isArchivedAssignment, WordItem *generalizationProperNameWordItem, WordItem *specificationWordItem, WordItem *spanishRelationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawProperNamePartOfConclusions";

		if( adminReasoningNew_ == NULL &&
		// Create supporting module
		( adminReasoningNew_ = new AdminReasoningNew( this, commonVariables() ) ) == NULL )
			return startError( functionNameString, NULL, NULL, "I failed to create the admin reasoning new module" );

		return adminReasoningNew_->drawProperNamePartOfConclusions( hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore, isArchivedAssignment, generalizationProperNameWordItem, specificationWordItem, spanishRelationWordItem );
		}

	WordItem *AdminItem::adjustedQuestionWordItem()
		{
		if( adminReasoningNew_ != NULL )
			return adminReasoningNew_->adjustedQuestionWordItem();

		return NULL;
		}


	// Protected old reasoning functions

	signed char AdminItem::checkForUnprocessedNegativeConclusion()
		{
		if( adminReasoningOld_ != NULL )
			return adminReasoningOld_->checkForUnprocessedNegativeConclusion();

		return RESULT_OK;
		}

	signed char AdminItem::drawNegativeConclusionsFromAnsweredQuestions( bool hasDisplayedMoreSpecificNonExclusiveSpecification, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusionsFromAnsweredQuestions";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->drawNegativeConclusionsFromAnsweredQuestions( hasDisplayedMoreSpecificNonExclusiveSpecification, userSpecificationItem, generalizationWordItem );
		}

	signed char AdminItem::drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawOnlyOptionLeftConclusion";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, specificationCollectionNr, generalizationWordItem );
		}

	signed char AdminItem::drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	signed char AdminItem::drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSimpleNegativeSpanishConclusion";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem );
		}

	signed char AdminItem::drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isPossessive, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->drawSpecificationGeneralizationConclusion( isArchivedAssignment, isPossessive, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );
		}

	signed char AdminItem::drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isMakingPartOfAssumption, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationSubstitutionConclusionOrAskQuestion";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isMakingPartOfAssumption, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	signed char AdminItem::makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	signed char AdminItem::makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

		if( adminReasoningOld_ == NULL &&
		// Create supporting module
		( adminReasoningOld_ = new AdminReasoningOld( this, commonVariables(), inputOutput_ ) ) == NULL )
			return startError( functionNameString, NULL, NULL, "I failed to create the admin reasoning module" );

		return adminReasoningOld_->makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, userSpecificationItem );
		}

	signed char AdminItem::makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *createdSpecificationItem, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem );
		}

	signed char AdminItem::makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminReasoningOld_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	SpecificationItem *AdminItem::oppositePossessiveDefinitionSpecificationItem()
		{
		if( adminReasoningOld_ != NULL )
			return adminReasoningOld_->oppositePossessiveDefinitionSpecificationItem();

		return NULL;
		}

	WordItem *AdminItem::possessiveDefinitionSpecificationWordItem()
		{
		if( adminReasoningOld_ != NULL )
			return adminReasoningOld_->possessiveDefinitionSpecificationWordItem();

		return NULL;
		}

	CompoundResultType AdminItem::drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( adminReasoningOld_ == NULL )
			return startCompoundResultError( functionNameString, NULL, "The admin reasoning old module isn't created yet" );

		return adminReasoningOld_->drawCompoundSpecificationSubstitutionConclusion( specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
		}


	// Protected specification functions

	void AdminItem::initializeLinkedWord()
		{
		if( adminSpecification_ != NULL )
			adminSpecification_->initializeLinkedWord();
		}

	bool AdminItem::hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore()
		{
		if( adminSpecification_ != NULL )
			return adminSpecification_->hasDisplaySpanishSpecificationsThatAreNotHiddenAnymore();

		return false;
		}

	signed char AdminItem::addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationWordItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecifications";

		if( adminSpecification_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );

		return adminSpecification_->addUserSpecifications( isAction, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, selectionLevel, selectionListNr, imperativeVerbParameter, generalizationContextNr, specificationContextNr, generalizationWordItem, startSpecificationReadItem, endSpecificationReadItem, startRelationReadItem, endRelationReadItem );
		}

	signed char AdminItem::assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSelectionSpecification";

		if( adminSpecification_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );

		return adminSpecification_->assignSelectionSpecification( assignmentSelectionItem );
		}

	signed char AdminItem::assignSpecification( WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( adminSpecification_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );

		return adminSpecification_->assignSpecification( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, generalizationWordItem, specificationWordItem, NULL ).result;
		}

	signed char AdminItem::collectGeneralizationWordWithPreviousOne( bool isAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationWords";

		if( adminSpecification_ == NULL &&
		// Create supporting module
		( adminSpecification_ = new AdminSpecification( this, commonVariables(), inputOutput_ ) ) == NULL )
			return startError( functionNameString, NULL, NULL, "I failed to create the admin specification module" );

		return adminSpecification_->collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem );
		}

	signed char AdminItem::createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionPart";

		switch( selectionListNr )
			{
			case ADMIN_CONDITION_LIST:
				if( conditionList_ == NULL )
					{
					// Create list
					if( ( conditionList_ = new SelectionList( ADMIN_CONDITION_LIST_SYMBOL, commonVariables(), inputOutput_, this ) ) == NULL )
						return startError( functionNameString, NULL, NULL, "I failed to create the condition list" );

					commonVariables()->adminConditionList = conditionList_;
					adminListArray_[ADMIN_CONDITION_LIST] = conditionList_;
					}

				return conditionList_->createSelectionItem( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			case ADMIN_ACTION_LIST:
				if( actionList_ == NULL )
					{
					// Create list
					if( ( actionList_ = new SelectionList( ADMIN_ACTION_LIST_SYMBOL, commonVariables(), inputOutput_, this ) ) == NULL )
						return startError( functionNameString, NULL, NULL, "I failed to create the action list" );

					commonVariables()->adminActionList = actionList_;
					adminListArray_[ADMIN_ACTION_LIST] = actionList_;
					}

				return actionList_->createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			case ADMIN_ALTERNATIVE_LIST:
				if( alternativeList_ == NULL )
					{
					// Create list
					if( ( alternativeList_ = new SelectionList( ADMIN_ALTERNATIVE_LIST_SYMBOL, commonVariables(), inputOutput_, this ) ) == NULL )
						return startError( functionNameString, NULL, NULL, "I failed to create the alternative list" );

					commonVariables()->adminAlternativeList = alternativeList_;
					adminListArray_[ADMIN_ALTERNATIVE_LIST] = alternativeList_;
					}

				return alternativeList_->createSelectionItem( false, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			default:
				return startError( functionNameString, NULL, NULL, "The given selection list number is undefined" );
			}
		}

	SpecificationItem *AdminItem::simpleUserSpecificationItem()
		{
		if( adminSpecification_ != NULL )
			return adminSpecification_->simpleUserSpecificationItem();

		return NULL;
		}

	SpecificationItem *AdminItem::userSpecificationItem()
		{
		if( adminSpecification_ != NULL )
			return adminSpecification_->userSpecificationItem();

		return NULL;
		}

	ContextResultType AdminItem::getGeneralizationContext( bool isArchivedAssignment, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getGeneralizationContext";

		if( adminSpecification_ == NULL )
			return startContextResultError( functionNameString, NULL, "The admin specification module isn't created yet" );

		return adminSpecification_->getGeneralizationContext( isArchivedAssignment, isPossessive, isQuestion, isUserSentence, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, startRelationReadItem );
		}

	CreateAndAssignResultType AdminItem::addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNameEnding, bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkipDrawingSpanishAmbiguousSubstitutionConclusion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSelfGeneratedSpecification";

		if( adminSpecification_ == NULL )
			return startCreateAndAssignResultError( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );

		return adminSpecification_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveGeneralization, isExclusiveSpecification, isForcingNewJustification, isNegative, isPartOf, isPossessive, isUniqueUserRelation, isSkipDrawingSpanishAmbiguousSubstitutionConclusion, isSpecificationGeneralization, assumptionLevel, assumptionJustificationTypeNr, conclusionJustificationTypeNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	CreateAndAssignResultType AdminItem::assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecificationWithAuthorization";

		if( adminReadFile_ == NULL )
			return startCreateAndAssignResultError( functionNameString, NULL, NULL, "The admin read file module isn't created yet" );

		return adminReadFile_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );
		}


	// Protected write functions

	void AdminItem::initializeAdminWriteVariables()
		{
		if( adminWrite_ != NULL )
			adminWrite_->initializeAdminWriteVariables();
		}

	signed char AdminItem::answerQuestions()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "answerQuestions";

		if( adminWrite_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );

		return adminWrite_->answerQuestions();
		}

	signed char AdminItem::checkIntegrityOfStoredUserSentence( char *readUserSentenceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkIntegrityOfStoredUserSentence";

		if( adminWrite_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );

		return adminWrite_->checkIntegrityOfStoredUserSentence( readUserSentenceString );
		}

	signed char AdminItem::markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem *userSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordsPassingIntegrityCheckOfStoredUserSentence";

		if( adminWrite_ == NULL &&
		// Create supporting module
		( adminWrite_ = new AdminWrite( this, commonVariables(), inputOutput_ ) ) == NULL )
			return startError( functionNameString, NULL, NULL, "I failed to create the admin write specification module" );

		return adminWrite_->markWordsPassingIntegrityCheckOfStoredUserSentence( userSpecificationItem );
		}

	signed char AdminItem::writeJustificationReport( WordItem *justificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationReport";

		if( adminWrite_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );

		return adminWrite_->writeSelfGeneratedInfo( false, true, true, true, true, justificationWordItem );
		}

	signed char AdminItem::writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		if( adminWrite_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );

		return adminWrite_->writeSelfGeneratedInfo( isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions );
		}

	signed char AdminItem::writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoAboutWord";

		if( adminWrite_ == NULL )
			return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );

		return adminWrite_->writeInfoAboutWord( isWritingCurrentSentenceOnly, isWritingUserSpecifications, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingUserQuestions, isWritingSelfGeneratedQuestions, isWritingSpecificationInfo, isWritingRelatedInfo, writeWordItem );
		}

/*************************************************************************
 *	"Listen to me, all you in the distant lands!
 *	Pay attention, you who are far away!
 *	The Lord called me before my birth;
 *	from within the womb he called me by name." (Psalm 49:1)
 *************************************************************************/
