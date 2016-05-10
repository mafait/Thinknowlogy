/*	Class:			AdminItem
 *	Parent class:	WordItem
 *	Grand parent:	Item
 *	Purpose:		To process tasks at administration level
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

import java.io.BufferedWriter;

class AdminItem extends WordItem
	{
	// Private constructible variables

	private boolean isSystemStartingUp_;

	private AdminAssumption adminAssumption_;
	private AdminAuthorization adminAuthorization_;
	private AdminCleanup adminCleanup_;
	private AdminCollection adminCollection_;
	private AdminConclusion adminConclusion_;
	private AdminContext adminContext_;
	private AdminImperative adminImperative_;
	private AdminLanguage adminLanguage_;
	private AdminQuery adminQuery_;
	private AdminReadCreateWords adminReadCreateWords_;
	private AdminReadFile adminReadFile_;
	private AdminReadGrammar adminReadGrammar_;
	private AdminReadSentence adminReadSentence_;
	private AdminReasoning adminReasoning_;
	private AdminSelection adminSelection_;
	private AdminSolve adminSolve_;
	private AdminSpecification adminSpecification_;
	private AdminWriteJustification adminWriteJustification_;
	private AdminWriteSpecification adminWriteSpecification_;


	// Private methods

	private byte readStartupFile()
		{
		if( adminReadFile_ != null ||
		// Create supporting module
		( adminReadFile_ = new AdminReadFile( this ) ) != null )
			return adminReadFile_.readStartupFile();

		return startError( 1, null, null, "I failed to create the admin read module" );
		}

	private byte startup()
		{
		WordItem predefinedNounLanguageWordItem;

		if( readStartupFile() == Constants.RESULT_OK )
			{
			if( ( predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem ) != null )
				{
				initializeQueryStringPosition();
				CommonVariables.queryStringBuffer = new StringBuffer();

				if( executeQuery( true, false, false, Constants.PRESENTATION_PROMPT_QUERY, ( Constants.EMPTY_STRING + Constants.QUERY_CHAR + Constants.QUERY_ACTIVE_CHAR + Constants.QUERY_WORD_REFERENCE_START_CHAR + predefinedNounLanguageWordItem.anyWordTypeString() + Constants.QUERY_WORD_REFERENCE_END_CHAR + Constants.QUERY_LIST_START_CHAR + Constants.WORD_SPECIFICATION_LIST_SYMBOL + Constants.QUERY_LIST_END_CHAR + Constants.QUERY_WORD_START_CHAR ) ) == Constants.RESULT_OK )
					{
					// More than one language available
					if( CommonVariables.queryStringBuffer.indexOf( Constants.QUERY_SEPARATOR_SPACE_STRING ) > 0 )
						CommonVariables.interfaceLanguageStringBuffer = new StringBuffer( CommonVariables.queryStringBuffer );
					}
				else
					// No show stopper
					addError( 1, null, null, "I failed to execute a query" );

				if( startupLanguageNameString != null )
					{
					if( assignLanguage( startupLanguageNameString ) != Constants.RESULT_OK )
						// No show stopper
						addError( 1, null, null, "I failed to assign the language" );
					}

				// Try to login without password
				if( login( null ) == Constants.RESULT_OK )
					{
					if( CommonVariables.currentUserNr > Constants.NO_USER_NR )
						isSystemStartingUp_ = false;
					else
						return startSystemError( 1, null, null, "No user is logged in" );
					}
				else
					return startSystemError( 1, null, null, "I failed to login" );
				}
			else
				return startSystemError( 1, null, null, "The predefined noun language word item is undefined" );
			}
		else
			return startSystemError( 1, null, null, "I failed to read the startup file" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminItem( String _startupLanguageNameString )
		{
		// Private constructible variables

		isSystemStartingUp_ = true;

		adminAssumption_ = null;
		adminAuthorization_ = null;
		adminCleanup_ = null;
		adminCollection_ = null;
		adminConclusion_ = null;
		adminContext_ = null;
		adminImperative_ = null;
		adminLanguage_ = null;
		adminQuery_ = null;
		adminReadFile_ = null;
		adminReadGrammar_ = null;
		adminReadCreateWords_ = null;
		adminReadSentence_ = null;
		adminReasoning_ = null;
		adminSelection_ = null;
		adminSolve_ = null;
		adminSpecification_ = null;
		adminWriteJustification_ = null;
		adminWriteSpecification_ = null;

		// Protected constructible variables

		fileList = null;
		readList = null;
		scoreList = null;
		wordList = null;
		conditionList = null;
		actionList = null;
		alternativeList = null;

		startupLanguageNameString = _startupLanguageNameString;

		for( short adminListNr : Constants.AdminLists )
			adminListArray[adminListNr] = null;

		// Initialization

		// Initialize presentation after restart
		Presentation.initialize();
		// Initialize common variables after restart
		CommonVariables.initialize();
		// Initialize my own item variables after restart
		initializeItemVariables( this );

		if( startup() != Constants.RESULT_OK )
			addError( 1, null, null, "I failed to start the administrator" );
		}


	// Protected constructible variables

	protected FileList fileList;
	protected ReadList readList;
	protected ScoreList scoreList;
	protected WordList wordList;
	protected SelectionList conditionList;
	protected SelectionList actionList;
	protected SelectionList alternativeList;

	String startupLanguageNameString;

	protected List[] adminListArray = new List[Constants.NUMBER_OF_ADMIN_LISTS];


	// Protected common methods

	protected boolean isSystemStartingUp()
		{
		return isSystemStartingUp_;
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


	// Protected error methods

	protected byte addErrorWithAdminListNr( short adminListNr, int methodLevel, String moduleNameString, String errorString )
		{
		Presentation.showError( adminListChar( adminListNr ), ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), null, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}

	protected byte startErrorWithAdminListNr( short adminListNr, int methodLevel, String moduleNameString, String errorString )
		{
		addErrorWithAdminListNr( adminListNr, ( methodLevel + 1 ), moduleNameString, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;

		return Constants.RESULT_ERROR;
		}


	// Protected assumption methods

	protected void initializeAdminAssumptionVariables()
		{
		if( adminAssumption_ != null )
			adminAssumption_.initializeAdminAssumptionVariables();
		}

	protected boolean hasFoundSuggestiveQuestionAssumption()
		{
		if( adminAssumption_ != null )
			return adminAssumption_.hasFoundSuggestiveQuestionAssumption();

		return false;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startError( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		if( adminAssumption_ != null ||
		// Create supporting module
		( adminAssumption_ = new AdminAssumption( this ) ) != null )
			return adminAssumption_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, userSpecificationItem );

		return startError( 1, null, null, "I failed to create the admin assumption module" );
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem createdSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem );

		return startError( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, userSpecificationItem, generalizationWordItem, relationWordItem );

		return startError( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startError( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected SpecificationItem oppositePossessiveDefinitionSpecificationItem()
		{
		if( adminAssumption_ != null )
			return adminAssumption_.oppositePossessiveDefinitionSpecificationItem();

		return null;
		}


	// Protected authorization methods

	protected boolean isDeveloperUser()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.isDeveloperUser();

		return false;
		}

	protected boolean isExpertUser()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.isExpertUser();

		return false;
		}

	protected int firstSentenceNrOfCurrentUser()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.firstSentenceNrOfCurrentUser();

		return Constants.NO_SENTENCE_NR;
		}

	protected byte login( WordItem specificationWordItem )
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.login( specificationWordItem );

		return startError( 1, null, null, "The admin authorization module isn't created yet" );
		}

	protected byte authorizeWord( WordItem authorizationWordItem )
		{
		if( adminAuthorization_ != null ||
		// Create supporting module
		( adminAuthorization_ = new AdminAuthorization( this ) ) != null )
			return adminAuthorization_.authorizeWord( authorizationWordItem );

		return startError( 1, null, null, "I failed to create the admin authorization module" );
		}

	protected String currentUserName()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.currentUserName();

		// During startup, before login
		return null;
		}


	// Protected cleanup methods

	protected void checkForChangesMadeByThisSentence()
		{
		if( adminCleanup_ != null )
			adminCleanup_.checkForChangesMadeByThisSentence();
		}

	protected void clearAllTemporaryLists()
		{
		if( adminCleanup_ != null )
			adminCleanup_.clearAllTemporaryLists();
		}

	protected void decrementCurrentSentenceNr()
		{
		if( adminCleanup_ != null )
			adminCleanup_.decrementCurrentSentenceNr();
		}

	protected void setCurrentItemNr()
		{
		if( adminCleanup_ != null )
			adminCleanup_.setCurrentItemNr();
		}

	protected boolean hasFoundAnyChangeMadeByThisSentence()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.hasFoundAnyChangeMadeByThisSentence();

		// Default if admin cleanup module isn't created yet
		return true;
		}

	protected boolean wasCurrentCommandUndoOrRedo()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.wasCurrentCommandUndoOrRedo();

		// Default if admin cleanup module isn't created yet
		return true;
		}

	protected byte cleanupDeletedItems()
		{
		if( adminCleanup_ != null ||
		// Create supporting module
		( adminCleanup_ = new AdminCleanup( this ) ) != null )
			return adminCleanup_.cleanupDeletedItems();

		return startError( 1, null, null, "I failed to create the admin cleanup module" );
		}

	protected byte deleteUnusedInterpretations( boolean isDeletingAllActiveWordTypes )
		{
		if( adminCleanup_ != null )
			return adminCleanup_.deleteUnusedInterpretations( isDeletingAllActiveWordTypes );

		return startError( 1, null, null, "The admin cleanup module isn't created yet" );
		}

	protected byte deleteSentences( int lowestSentenceNr )
		{
		if( adminCleanup_ != null )
			return adminCleanup_.deleteSentences( lowestSentenceNr );

		return startError( 1, null, null, "The admin cleanup module isn't created yet" );
		}

	protected byte undoLastSentence()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.undoLastSentence();

		return startError( 1, null, null, "The admin cleanup module isn't created yet" );
		}

	protected byte redoLastUndoneSentence()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.redoLastUndoneSentence();

		return startError( 1, null, null, "The admin cleanup module isn't created yet" );
		}


	// Protected collection methods

	protected CollectionResultType collectSpecificationWords( boolean isExclusiveSpecification, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem compoundGeneralizationWordItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		if( adminCollection_ != null )
			return adminCollection_.collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, specificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem );

		collectionResult.result = startError( 1, null, null, "The admin collection module isn't created yet" );
		return collectionResult;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminCollection_ != null ||
		// Create supporting module
		( adminCollection_ = new AdminCollection( this ) ) != null )
			return adminCollection_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem );

		return startError( 1, null, null, "I failed to create the admin collection module" );
		}

	protected byte collectRelationWords( boolean isExclusiveSpecification, short relationWordTypeNr, short specificationWordTypeNr, WordItem previousRelationWordItem, WordItem currentRelationWordItem, WordItem specificationWordItem )
		{
		if( adminCollection_ != null )
			return adminCollection_.collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, currentRelationWordItem, specificationWordItem );

		return startError( 1, null, null, "The admin collection module isn't created yet" );
		}


	// Protected conclusion methods

	protected void initializeAdminConclusionVariables()
		{
		if( adminConclusion_ != null )
			adminConclusion_.initializeAdminConclusionVariables();
		}

	protected boolean hasFoundUnprocessedNegativeConclusion()
		{
		if( adminConclusion_ != null )
			return adminConclusion_.hasFoundUnprocessedNegativeConclusion();

		return false;
		}

	protected byte drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawNegativeConclusionsFromAnsweredQuestions( userSpecificationItem, generalizationWordItem );

		return startError( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawSimpleNegativeSpanishConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem );

		return startError( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, WordItem generalizationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, specificationCollectionNr, generalizationWordItem );

		return startError( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawPossessiveReversibleConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, boolean isUniqueUserRelation, short assumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawSpecificationGeneralizationConclusion( isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );

		return startError( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isMakingPartOfAssumption, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminConclusion_ != null ||
		// Create supporting module
		( adminConclusion_ = new AdminConclusion( this ) ) != null )
			return adminConclusion_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isMakingPartOfAssumption, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( 1, null, null, "I failed to create the admin conclusion module" );
		}

	protected SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( adminConclusion_ != null )
			return adminConclusion_.drawCompoundSpecificationSubstitutionConclusion( specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		specificationResult.result = startError( 1, null, null, "The admin conclusion module isn't created yet" );
		return specificationResult;
		}

	protected SpecificationItem spanishAmbiguousCompoundPrimarySpecificationItem()
		{
		if( adminConclusion_ != null )
			return adminConclusion_.spanishAmbiguousCompoundPrimarySpecificationItem();

		return null;
		}

	protected SpecificationItem spanishAmbiguousCompoundAnotherPrimarySpecificationItem()
		{
		if( adminConclusion_ != null )
			return adminConclusion_.spanishAmbiguousCompoundAnotherPrimarySpecificationItem();

		return null;
		}


	// Protected context methods

	protected int highestContextNrInAllWords()
		{
		int tempContextNr;
		int highestContextNr = Constants.NO_CONTEXT_NR;
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
			{
			// Do for all active words
			do	{
				if( ( tempContextNr = currentWordItem.highestContextNrInWord() ) > highestContextNr )
					highestContextNr = tempContextNr;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return highestContextNr;
		}

	protected ContextResultType getRelationContext( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, boolean isQuestion, boolean isUserSentence, short specificationWordTypeNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( adminContext_ != null ||
		// Create supporting module
		( adminContext_ = new AdminContext( this ) ) != null )
			return adminContext_.getRelationContext( isArchivedAssignment, isNegative, isPossessive, isQuestion, isUserSentence, specificationWordTypeNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, startRelationReadItem );

		contextResult.result = startError( 1, null, null, "I failed to create the admin context module" );
		return contextResult;
		}

	protected ContextResultType getSpecificationRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCompoundCollectionSpanishAmbiguous, boolean isNegative, boolean isPossessive, boolean isSelfGeneratedAssumption, int specificationCollectionNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( adminContext_ != null ||
		// Create supporting module
		( adminContext_ = new AdminContext( this ) ) != null )
			return adminContext_.getSpecificationRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionSpanishAmbiguous, isNegative, isPossessive, isSelfGeneratedAssumption, specificationCollectionNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		contextResult.result = startError( 1, null, null, "I failed to create the admin context module" );
		return contextResult;
		}


	// Protected database connection methods
/*
	protected byte storeSentenceInFutureDatabase()
		{
		if( fileList != null )
			return fileList.storeChangesInFutureDatabase();

		// Read list and score list aren't stored, because these are temporary lists

		if( wordList != null )
			return wordList.storeChangesInFutureDatabase();

		if( conditionList != null )
			return conditionList.storeChangesInFutureDatabase();

		if( actionList != null )
			return actionList.storeChangesInFutureDatabase();

		if( alternativeList != null )
			return alternativeList.storeChangesInFutureDatabase();

		return Constants.RESULT_OK;
		}
*/


	// Protected imperative methods

	protected boolean hasRequestedRestart()
		{
		if( adminImperative_ != null )
			return adminImperative_.hasRequestedRestart();

		return false;
		}

	protected byte executeImperative( boolean isInitializeVariables, short executionListNr, short imperativeParameter, short specificationWordParameter, short specificationWordTypeNr, int endSolveProgress, String executionString, WordItem generalizationWordItem, WordItem specificationWordItem, ReadItem startRelationWordReadItem, ReadItem endRelationReadItem, SelectionItem executionSelectionItem, SelectionItem actionSelectionItem )
		{
		if( adminImperative_ != null ||
		// Create supporting module
		( adminImperative_ = new AdminImperative( this ) ) != null )
			return adminImperative_.executeImperative( isInitializeVariables, executionListNr, imperativeParameter, specificationWordParameter, specificationWordTypeNr, endSolveProgress, executionString, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem, executionSelectionItem, actionSelectionItem );

		return startError( 1, null, null, "I failed to create the admin imperative module" );
		}


	// Protected language methods

	protected byte authorizeLanguageWord( WordItem authorizationWordItem )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.authorizeLanguageWord( authorizationWordItem );

		return startError( 1, null, null, "The admin language module isn't created yet" );
		}

	protected byte createLanguage( String languageNameString )
		{
		if( adminLanguage_ != null ||
		// Create supporting module
		( adminLanguage_ = new AdminLanguage( this ) ) != null )
			return adminLanguage_.createLanguage( languageNameString );

		return startError( 1, null, null, "I failed to create the admin language module" );
		}

	protected byte createLanguageSpecification( WordItem languageNounWordItem )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.createLanguageSpecification( languageNounWordItem );

		return startError( 1, null, null, "The admin language module isn't created yet" );
		}

	protected byte assignLanguage( short newLanguageNr )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.assignLanguage( newLanguageNr );

		return startError( 1, null, null, "The admin language module isn't created yet" );
		}

	protected byte assignLanguage( String languageNameString )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.assignLanguage( languageNameString );

		return startError( 1, null, null, "The admin language module isn't created yet" );
		}


	// Protected query methods

	protected void initializeQueryStringPosition()
		{
		if( adminQuery_ != null )
			adminQuery_.initializeQueryStringPosition();
		}

	protected byte writeInfoTextWithPossibleQueryCommands( String textString )
		{
		if( adminQuery_ != null ||
		// Create supporting module
		( adminQuery_ = new AdminQuery( this ) ) != null )
			return adminQuery_.writeInfoTextWithPossibleQueryCommands( textString );

		return startError( 1, null, null, "I failed to create the admin query module" );
		}

	protected byte executeQuery( boolean isSuppressingMessage, boolean isReturningToPosition, boolean isWritingQueryResult, short promptTypeNr, String queryString )
		{
		if( adminQuery_ != null ||
		// Create supporting module
		( adminQuery_ = new AdminQuery( this ) ) != null )
			return adminQuery_.executeQuery( isSuppressingMessage, isReturningToPosition, isWritingQueryResult, promptTypeNr, queryString );

		return startError( 1, null, null, "I failed to create the admin query module" );
		}


	// Protected read create words methods

	protected void deleteTemporaryReadList()
		{
		if( readList != null )
			readList.deleteTemporaryList();
		}

	protected boolean hasFoundDifferentParameter()
		{
		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.hasFoundDifferentParameter();

		return false;
		}

	protected boolean isImperativeSentence()
		{
		if( readList != null )
			return readList.isImperativeSentence();

		return false;
		}

	protected short lastCreatedWordOrderNr()
		{
		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.lastCreatedWordOrderNr();

		return Constants.NO_ORDER_NR;
		}

	protected ReadResultType createReadWords( String grammarString )
		{
		ReadResultType readResult = new ReadResultType();

		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.createReadWords( grammarString );

		readResult.result = startError( 1, null, null, "The admin read create words module isn't created yet" );
		return readResult;
		}

	protected ReadResultType readWordFromString( boolean isCheckingForGrammarDefinition, boolean isMergedWord, boolean isSkippingTextString, int startWordPosition, int minimumStringLength, String wordString )
		{
		ReadResultType readResult = new ReadResultType();

		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.readWordFromString( isCheckingForGrammarDefinition, isMergedWord, isSkippingTextString, startWordPosition, minimumStringLength, wordString );

		readResult.result = startError( 1, null, null, "The admin read create words module isn't created yet" );
		return readResult;
		}

	protected WordResultType addWord( boolean isLanguageWord, boolean isMultipleWord, short previousWordAdjectiveParameter, short previousWordDefiniteArticleParameter, short previousWordIndefiniteArticleParameter, short wordParameter, short wordTypeNr, int wordLength, String wordString )
		{
		WordResultType wordResult = new WordResultType();

		if( adminReadCreateWords_ != null ||
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this ) ) != null )
			return adminReadCreateWords_.addWord( isLanguageWord, isMultipleWord, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, wordParameter, wordTypeNr, wordLength, wordString );

		wordResult.result = startError( 1, null, null, "I failed to create the admin read create words module" );
		return wordResult;
		}

	protected WordResultType findWordTypeInAllWords( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString, WordItem previousWordItem )
		{
		WordResultType wordResult = new WordResultType();

		if( adminReadCreateWords_ != null ||
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this ) ) != null )
			return adminReadCreateWords_.findWordTypeInAllWords( isCheckingAllLanguages, wordTypeNr, wordTypeString, previousWordItem );

		wordResult.result = startError( 1, null, null, "I failed to create the admin read create words module" );
		return wordResult;
		}

	protected ReadItem firstActiveReadItem()
		{
		if( readList != null )
			return readList.firstActiveReadItem();

		return null;
		}

	protected ReadItem firstInactiveReadItem()
		{
		if( readList != null )
			return readList.firstInactiveReadItem();

		return null;
		}


	// Protected read file methods

	protected boolean hasClosedFileDueToError()
		{
		if( adminReadFile_ != null )
			return adminReadFile_.hasClosedFileDueToError();

		return false;
		}

	protected boolean isCurrentlyTesting()
		{
		if( fileList != null )
			return fileList.isCurrentlyTesting();

		return false;
		}

	protected byte compareOutputFileAgainstReferenceFile( String testFileNameString )
		{
		if( adminReadFile_ != null )
			return adminReadFile_.compareOutputFileAgainstReferenceFile( testFileNameString );

		return startError( 1, null, null, "The admin read module isn't created yet" );
		}

	protected byte getUserInput( boolean isGuideByGrammarPrompt, boolean isPassword, boolean isQuestion, boolean isText, String promptInputString, StringBuffer readStringBuffer )
		{
		if( adminReadFile_ != null )
			return adminReadFile_.readLine( true, false, isGuideByGrammarPrompt, isPassword, isQuestion, isText, Constants.NO_SENTENCE_NR, promptInputString, readStringBuffer );

		return startError( 1, null, null, "The admin read module isn't created yet" );
		}

	protected byte readAndExecute()
		{
		if( adminReadFile_ != null )
			return adminReadFile_.readAndExecute();

		return startError( 1, null, null, "The admin read module isn't created yet" );
		}

	protected byte readExampleFile( String exampleFileNameString )
		{
		if( adminReadFile_ != null )
			return adminReadFile_.readExampleFile( exampleFileNameString );

		return startError( 1, null, null, "The admin read module isn't created yet" );
		}

	protected byte readTestFile( String testFileNameString )
		{
		if( adminReadFile_ != null )
			return adminReadFile_.readTestFile( testFileNameString );

		return startError( 1, null, null, "The admin read module isn't created yet" );
		}

	protected FileResultType readInfoFile( boolean isReportingErrorIfFileDoesNotExist, String infoFileNameString )
		{
		FileResultType fileResult = new FileResultType();

		if( adminReadFile_ != null )
			return adminReadFile_.readInfoFile( isReportingErrorIfFileDoesNotExist, infoFileNameString );

		fileResult.result = startError( 1, null, null, "The admin read module isn't created yet" );
		return fileResult;
		}

	protected BufferedWriter currentWriteFile()
		{
		if( fileList != null )
			return fileList.currentWriteFile();

		return null;
		}


	// Protected read grammar methods

	protected byte addGrammar( String grammarString )
		{
		if( adminReadGrammar_ != null ||
		// Create supporting module
		( adminReadGrammar_ = new AdminReadGrammar( this ) ) != null )
			return adminReadGrammar_.addGrammar( grammarString );

		return startError( 1, null, null, "I failed to create the admin read grammar module" );
		}

	protected WordItem predefinedAdjectiveBusyWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedAdjectiveBusyWordItem();

		return null;
		}

	protected WordItem predefinedAdjectiveDoneWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedAdjectiveDoneWordItem();

		return null;
		}

	protected WordItem predefinedAdjectiveInvertedWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedAdjectiveInvertedWordItem();

		return null;
		}

	protected WordItem predefinedNounDeveloperWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounDeveloperWordItem();

		return null;
		}

	protected WordItem predefinedNounExpertWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounExpertWordItem();

		return null;
		}

	protected WordItem predefinedNounPasswordWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounPasswordWordItem();

		return null;
		}

	protected WordItem predefinedNounSolveLevelWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounSolveLevelWordItem();

		return null;
		}

	protected WordItem predefinedNounSolveMethodWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounSolveMethodWordItem();

		return null;
		}

	protected WordItem predefinedNounSolveStrategyWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounSolveStrategyWordItem();

		return null;
		}

	protected WordItem predefinedNounStartupLanguageWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounStartupLanguageWordItem();

		return null;
		}

	protected WordItem predefinedVerbLoginWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedVerbLoginWordItem();

		return null;
		}


	// Protected read sentence methods

	protected boolean isActiveUserAssignment()
		{
		if( adminReadSentence_ != null )
			return adminReadSentence_.isActiveUserAssignment();

		return false;
		}

	protected boolean isUserQuestion()
		{
		if( adminReadSentence_ != null )
			return adminReadSentence_.isUserQuestion();

		return false;
		}

	protected boolean wasPreviousCommandUndoOrRedo()
		{
		if( adminReadSentence_ != null )
			return adminReadSentence_.wasPreviousCommandUndoOrRedo();

		return false;
		}

	protected byte processReadSentence( String readString )
		{
		if( adminReadSentence_ != null ||
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this ) ) != null )
			return adminReadSentence_.processReadSentence( readString );

		return startError( 1, null, null, "I failed to create the admin read sentence module" );
		}


	// Protected reasoning methods

	protected void initializeAdminReasoningVariables()
		{
		if( adminReasoning_ != null )
			adminReasoning_.initializeAdminReasoningVariables();
		}

	protected byte askSpecificationSubstitutionQuestion( boolean isArchivedAssignment, boolean isExclusiveSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem existingQuestionSpecificationItem, SpecificationItem primarySpecificationItem, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminReasoning_ != null ||
		// Create supporting module
		( adminReasoning_ = new AdminReasoning( this ) ) != null )
			return adminReasoning_.askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, existingQuestionSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, specificationWordItem );

		return startError( 1, null, null, "I failed to create the admin reasoning module" );
		}

	protected SpecificationResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNameEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkipAdditionalConclusionOrAskQuestion, boolean isSpecificationGeneralization, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( adminReasoning_ != null ||
		// Create supporting module
		( adminReasoning_ = new AdminReasoning( this ) ) != null )
			return adminReasoning_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isForcingNewJustification, isNegative, isPartOf, isPossessive, isUniqueUserRelation, isSkipAdditionalConclusionOrAskQuestion, isSpecificationGeneralization, assumptionLevel, assumptionJustificationTypeNr, conclusionJustificationTypeNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		specificationResult.result = startError( 1, null, null, "I failed to create the admin reasoning module" );
		return specificationResult;
		}

	protected SpecificationItem alternativeAnotherPrimarySpecificationItem()
		{
		if( adminReasoning_ != null )
			return adminReasoning_.alternativeAnotherPrimarySpecificationItem();

		return null;
		}

	protected SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNameEnding, boolean isCheckingForPossessiveSpecification, boolean isCheckingForQuestion, boolean isNegative, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminReasoning_ != null )
			return adminReasoning_.alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, isCheckingForPossessiveSpecification, isCheckingForQuestion, isNegative, isPossessive, userSpecificationItem, generalizationWordItem, specificationWordItem );

		return null;
		}


	// Protected selection methods

	protected byte checkForDuplicateSelection()
		{
		if( adminSelection_ != null )
			return adminSelection_.checkForDuplicateSelection();

		return startError( 1, null, null, "The admin selection module isn't created yet" );
		}

	protected byte createSelectionPart( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short selectionListNr, short imperativeParameter, short prepositionParameter, short specificationWordParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		if( adminSelection_ != null ||
		// Create supporting module
		( adminSelection_ = new AdminSelection( this ) ) != null )
			return adminSelection_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		return startError( 1, null, null, "I failed to create the admin selection module" );
		}

	protected byte executeSelections()
		{
		if( adminSelection_ != null )
			return adminSelection_.executeSelection( Constants.MAX_PROGRESS, null );

		// If no selections exist, just return OK
		return Constants.RESULT_OK;
		}

	protected byte executeSelection( int endSolveProgress, SelectionItem actionSelectionItem )
		{
		if( adminSelection_ != null )
			return adminSelection_.executeSelection( endSolveProgress, actionSelectionItem );

		return startError( 1, null, null, "The admin selection module isn't created yet" );
		}


	// Protected solve methods

	protected void clearCurrentSolveProgress()
		{
		if( adminSolve_ != null )
			adminSolve_.clearCurrentSolveProgress();
		}

	protected void deleteTemporaryScoreList()
		{
		if( scoreList != null )
			scoreList.deleteTemporaryList();
		}

	protected byte solveWord( int endSolveProgress, WordItem solveWordItem, SelectionItem actionSelectionItem )
		{
		if( adminSolve_ != null )
			return adminSolve_.solveWord( endSolveProgress, solveWordItem, actionSelectionItem );

		return startError( 1, null, null, "The admin solve module isn't created yet" );
		}

	protected SelectionResultType checkCondition( SelectionItem conditionSelectionItem )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		if( adminSolve_ != null ||
		// Create supporting module
		( adminSolve_ = new AdminSolve( this ) ) != null )
			return adminSolve_.checkCondition( conditionSelectionItem );

		selectionResult.result = startError( 1, null, null, "I failed to create the admin solve module" );
		return selectionResult;
		}


	// Protected specification methods

	protected void initializeLinkedWord()
		{
		if( adminSpecification_ != null )
			adminSpecification_.initializeLinkedWord();
		}

	protected void initializeAdminSpecificationVariables()
		{
		if( adminSpecification_ != null )
			adminSpecification_.initializeAdminSpecificationVariables();
		}

	protected byte addUserSpecifications( boolean isAction, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short imperativeParameter, short specificationWordParameter, int generalizationContextNr, int specificationContextNr, ReadItem generalizationWordItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		if( adminSpecification_ != null )
			return adminSpecification_.addUserSpecifications( isAction, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, generalizationContextNr, specificationContextNr, generalizationWordItem, startSpecificationReadItem, endSpecificationReadItem, startRelationReadItem, endRelationReadItem );

		return startError( 1, null, null, "The admin specification module isn't created yet" );
		}

	protected byte assignSelectionSpecification( SelectionItem assignmentSelectionItem )
		{
		if( adminSpecification_ != null )
			return adminSpecification_.assignSelectionSpecification( assignmentSelectionItem );

		return startError( 1, null, null, "The admin specification module isn't created yet" );
		}

	protected byte assignSpecification( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminSpecification_ != null )
			return adminSpecification_.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, generalizationWordItem, specificationWordItem, null ).result;

		return startError( 1, null, null, "The admin specification module isn't created yet" );
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( adminSpecification_ != null ||
		// Create supporting module
		( adminSpecification_ = new AdminSpecification( this ) ) != null )
			return adminSpecification_.addSpecification( isAssignment, false, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		specificationResult.result = startError( 1, null, null, "I failed to create the admin specification module" );
		return specificationResult;
		}

	protected SpecificationResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( specificationWordItem == CommonVariables.predefinedNounLanguageWordItem )
			{
			if( adminLanguage_ != null )
				return adminLanguage_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startError( 1, null, null, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != null )
				return adminAuthorization_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startError( 1, null, null, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( specificationWordItem == CommonVariables.predefinedNounLanguageWordItem )
			{
			if( adminLanguage_ != null )
				return adminLanguage_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startError( 1, null, null, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != null )
				return adminAuthorization_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startError( 1, null, null, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationItem simpleUserSpecificationItem()
		{
		if( adminSpecification_ != null )
			return adminSpecification_.simpleUserSpecificationItem();

		return null;
		}

	protected SpecificationItem userSpecificationItem()
		{
		if( adminSpecification_ != null )
			return adminSpecification_.userSpecificationItem();

		return null;
		}


	// Protected write justification methods

	protected byte writeJustificationSpecification( SpecificationItem justificationSpecificationItem )
		{
		if( adminWriteJustification_ != null ||
		// Create supporting module
		( adminWriteJustification_ = new AdminWriteJustification( this ) ) != null )
			return adminWriteJustification_.writeJustificationSpecification( justificationSpecificationItem );

		return startError( 1, null, null, "I failed to create the admin write justification module" );
		}


	// Protected write specification methods

	protected void initializeAdminWriteVariables()
		{
		if( adminWriteSpecification_ != null )
			adminWriteSpecification_.initializeAdminWriteVariables();
		}

	protected byte answerQuestions()
		{
		if( adminWriteSpecification_ != null )
			return adminWriteSpecification_.answerQuestions();

		return startError( 1, null, null, "The admin write module isn't created yet" );
		}

	protected byte checkIntegrityOfStoredUserSentence( String readUserSentenceString )
		{
		if( adminWriteSpecification_ != null ||
		// Create supporting module
		( adminWriteSpecification_ = new AdminWriteSpecification( this ) ) != null )
			return adminWriteSpecification_.checkIntegrityOfStoredUserSentence( readUserSentenceString );

		return startError( 1, null, null, "I failed to create the admin write specification module" );
		}

	protected byte markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem userSpecificationItem )
		{
		if( adminWriteSpecification_ != null ||
		// Create supporting module
		( adminWriteSpecification_ = new AdminWriteSpecification( this ) ) != null )
			return adminWriteSpecification_.markWordsPassingIntegrityCheckOfStoredUserSentence( userSpecificationItem );

		return startError( 1, null, null, "I failed to create the admin write specification module" );
		}

	protected byte writeJustificationReport( WordItem justificationWordItem )
		{
		if( adminWriteSpecification_ != null )
			return adminWriteSpecification_.writeSelfGeneratedInfo( false, true, true, true, true, justificationWordItem );

		return startError( 1, null, null, "The admin write module isn't created yet" );
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions )
		{
		if( adminWriteSpecification_ != null )
			return adminWriteSpecification_.writeSelfGeneratedInfo( isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions );

		return startError( 1, null, null, "The admin write module isn't created yet" );
		}

	protected byte writeInfoAboutWord( boolean isWritingCurrentSentenceOnly, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, boolean isWritingSpecificationInfo, boolean isWritingRelatedInfo, WordItem writeWordItem )
		{
		if( adminWriteSpecification_ != null )
			return adminWriteSpecification_.writeInfoAboutWord( isWritingCurrentSentenceOnly, isWritingUserSpecifications, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingUserQuestions, isWritingSelfGeneratedQuestions, isWritingSpecificationInfo, isWritingRelatedInfo, writeWordItem );

		return startError( 1, null, null, "The admin write module isn't created yet" );
		}


	// Thinknowlogy methods

	protected String currentLanguageNameString()
		{
		return languageNameString( CommonVariables.currentLanguageNr );
		}

	protected byte interact()
		{
		if( CommonVariables.result != Constants.RESULT_SYSTEM_ERROR )
			{
			readAndExecute();
			// Ignore Constants.RESULT_ERROR. Application will only exit on system error
			CommonVariables.result = Constants.RESULT_OK;
			// Application will not exit on a normal error
			return Constants.RESULT_OK;
			}

		return Constants.RESULT_SYSTEM_ERROR;
		}
	};

/*************************************************************************
 *	"Listen to me, all you in the distant lands!
 *	Pay attention, you who are far away!
 *	The Lord called me before my birth;
 *	from within the womb he called me by name." (Psalm 49:1)
 *************************************************************************/
