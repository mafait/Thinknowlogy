/*	Class:			AdminItem
 *	Parent class:	WordItem
 *	Grand parent:	Item
 *	Purpose:		To process tasks at administration level
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

import java.io.BufferedWriter;

class AdminItem extends WordItem
	{
	// Private constructed variables

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
	private AdminSelection adminSelection_;
	private AdminSolve adminSolve_;
	private AdminSpecification adminSpecification_;
	private AdminWriteJustification adminWriteJustification_;
	private AdminWriteSpecification adminWriteSpecification_;


	// Private methods

	private byte readStartupFile()
		{
		if( adminReadFile_ == null &&
		// Create supporting module
		( adminReadFile_ = new AdminReadFile( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin read module" );

		return adminReadFile_.readStartupFile();
		}

	private byte startup()
		{
		WordItem predefinedNounLanguageWordItem;

		if( readStartupFile() != Constants.RESULT_OK )
			return startSystemError( 1, null, null, "I failed to read the startup file" );

		if( ( predefinedNounLanguageWordItem = CommonVariables.predefinedNounLanguageWordItem ) == null )
			return startSystemError( 1, null, null, "The predefined noun language word item is undefined" );

		initializeQueryStringPosition();
		CommonVariables.queryStringBuffer = new StringBuffer();

		if( executeQuery( true, false, false, Constants.PRESENTATION_PROMPT_QUERY, ( Constants.EMPTY_STRING + Constants.QUERY_CHAR + Constants.QUERY_ACTIVE_CHAR + Constants.QUERY_WORD_REFERENCE_START_CHAR + predefinedNounLanguageWordItem.anyWordTypeString() + Constants.QUERY_WORD_REFERENCE_END_CHAR + Constants.QUERY_LIST_START_CHAR + Constants.WORD_SPECIFICATION_LIST_SYMBOL + Constants.QUERY_LIST_END_CHAR + Constants.QUERY_WORD_START_CHAR ) ) != Constants.RESULT_OK )
			// No show stopper
			addError( 1, null, null, "I failed to execute a query" );

		// More than one language available
		if( CommonVariables.queryStringBuffer.indexOf( Constants.QUERY_SEPARATOR_SPACE_STRING ) > 0 )
			CommonVariables.interfaceLanguageStringBuffer = new StringBuffer( CommonVariables.queryStringBuffer );

		if( startupLanguageNameString != null )
			{
			if( assignLanguage( startupLanguageNameString ) != Constants.RESULT_OK )
				// No show stopper
				addError( 1, null, null, "I failed to assign the language" );
			}

		// Try to login without password
		if( login( null ) != Constants.RESULT_OK )
			return startSystemError( 1, null, null, "I failed to login" );

		if( CommonVariables.currentUserNr <= Constants.NO_USER_NR )
			return startSystemError( 1, null, null, "No user is logged in" );

		isSystemStartingUp_ = false;

		return Constants.RESULT_OK;
		}


	// Constructor

	protected AdminItem( String _startupLanguageNameString )
		{
		// Private constructed variables

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
		adminSelection_ = null;
		adminSolve_ = null;
		adminSpecification_ = null;
		adminWriteJustification_ = null;
		adminWriteSpecification_ = null;

		// Protected constructed variables

		fileList = null;
		readList = null;
		scoreList = null;
		wordList = null;
		conditionList = null;
		actionList = null;
		alternativeList = null;

		startupLanguageNameString = _startupLanguageNameString;

		for( short adminListNr = 0; adminListNr < Constants.NUMBER_OF_ADMIN_LISTS; adminListNr++ )
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


	// Protected constructed variables

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

	protected void initializeAdminVariables()
		{
		if( adminAssumption_ != null )
			adminAssumption_.initializeAdminAssumptionVariables();

		if( adminAuthorization_ != null )
			adminAuthorization_.initializeAdminAuthorizationVariables();

		if( adminConclusion_ != null )
			adminConclusion_.initializeAdminConclusionVariables();

		if( adminSpecification_ != null )
			adminSpecification_.initializeAdminSpecificationVariables();
		}

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
		Presentation.displayError( adminListChar( adminListNr ), ( moduleNameString == null ? this.getClass().getName() : moduleNameString ), ( moduleNameString == null ? this.getClass().getSuperclass().getName() : null ), null, ( methodLevel + 1 ), errorString );
		return CommonVariables.result;
		}

	protected byte startErrorWithAdminListNr( short adminListNr, int methodLevel, String moduleNameString, String errorString )
		{
		addErrorWithAdminListNr( adminListNr, ( methodLevel + 1 ), moduleNameString, errorString );

		CommonVariables.result = Constants.RESULT_ERROR;

		return Constants.RESULT_ERROR;
		}


	// Protected assumption methods

	protected boolean hasSuggestiveQuestionAssumption()
		{
		if( adminAssumption_ != null )
			return adminAssumption_.hasSuggestiveQuestionAssumption();

		return false;
		}

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ == null )
			return startError( 1, null, null, "The admin assumption module isn't created yet" );

		return adminAssumption_.makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminAssumption_ == null )
			return startError( 1, null, null, "The admin assumption module isn't created yet" );

		return adminAssumption_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		if( adminAssumption_ == null &&
		// Create supporting module
		( adminAssumption_ = new AdminAssumption( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin assumption module" );

		return adminAssumption_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, userSpecificationItem );
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem createdSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminAssumption_ == null )
			return startError( 1, null, null, "The admin assumption module isn't created yet" );

		return adminAssumption_.makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem );
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, SpecificationItem secondarySpecificationItem, SpecificationItem alternativeSecondarySpecificationItem, WordItem generalizationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ == null )
			return startError( 1, null, null, "The admin assumption module isn't created yet" );

		return adminAssumption_.makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, alternativeSecondarySpecificationItem, generalizationWordItem, relationWordItem );
		}

	protected byte makeSpecificationSubstitutionPartOfAssumption( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminAssumption_ == null )
			return startError( 1, null, null, "The admin assumption module isn't created yet" );

		return adminAssumption_.makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ == null )
			return startError( 1, null, null, "The admin assumption module isn't created yet" );

		return adminAssumption_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected SpecificationItem oppositePossessiveDefinitionSpecificationItem()
		{
		if( adminAssumption_ != null )
			return adminAssumption_.oppositePossessiveDefinitionSpecificationItem();

		return null;
		}


	// Protected authorization methods

	protected boolean isCurrentUserDeveloper()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.isCurrentUserDeveloper();

		return false;
		}

	protected boolean isCurrentUserDeveloperOrExpert()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.isCurrentUserDeveloperOrExpert();

		return false;
		}

	protected boolean isCurrentUserExpert()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.isCurrentUserExpert();

		return false;
		}

	protected boolean wasLoginCommand()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.wasLoginCommand();

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
		if( adminAuthorization_ == null )
			return startError( 1, null, null, "The admin authorization module isn't created yet" );

		return adminAuthorization_.login( specificationWordItem );
		}

	protected byte authorizeWord( WordItem authorizationWordItem )
		{
		if( adminAuthorization_ == null &&
		// Create supporting module
		( adminAuthorization_ = new AdminAuthorization( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin authorization module" );

		return adminAuthorization_.authorizeWord( authorizationWordItem );
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

	protected void clearTemporaryAdminLists()
		{
		if( adminCleanup_ != null )
			adminCleanup_.clearTemporaryAdminLists();
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

	protected boolean hasAnyChangeBeenMadeByThisSentence()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.hasAnyChangeBeenMadeByThisSentence();

		// Default if admin cleanup module isn't created yet
		return true;
		}

	protected boolean wasUndoOrRedoCommand()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.wasUndoOrRedoCommand();

		// Default if admin cleanup module isn't created yet
		return true;
		}

	protected byte cleanupDeletedItems()
		{
		if( adminCleanup_ == null &&
		// Create supporting module
		( adminCleanup_ = new AdminCleanup( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin cleanup module" );

		adminCleanup_.cleanupDeletedItems();
		return Constants.RESULT_OK;
		}

	protected byte deleteAllWordTypesOfCurrentSentence()
		{
		if( adminCleanup_ == null )
			return startError( 1, null, null, "The admin cleanup module isn't created yet" );

		return adminCleanup_.deleteAllWordTypesOfCurrentSentence();
		}

	protected byte deleteUnusedWordsAndWordTypes()
		{
		if( adminCleanup_ == null )
			return startError( 1, null, null, "The admin cleanup module isn't created yet" );

		return adminCleanup_.deleteUnusedWordsAndWordTypes();
		}

	protected byte deleteSentences( int lowestSentenceNr )
		{
		if( adminCleanup_ == null )
			return startError( 1, null, null, "The admin cleanup module isn't created yet" );

		adminCleanup_.deleteSentences( lowestSentenceNr );
		return Constants.RESULT_OK;
		}

	protected byte undoLastSentence()
		{
		if( adminCleanup_ == null )
			return startError( 1, null, null, "The admin cleanup module isn't created yet" );

		return adminCleanup_.undoLastSentence();
		}

	protected byte redoLastUndoneSentence()
		{
		if( adminCleanup_ == null )
			return startError( 1, null, null, "The admin cleanup module isn't created yet" );

		return adminCleanup_.redoLastUndoneSentence();
		}


	// Protected collection methods

	protected CollectionResultType collectSpecificationWords( boolean isExclusiveSpecification, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem compoundGeneralizationWordItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		if( adminCollection_ == null )
			return startCollectionResultError( 1, null, null, "The admin collection module isn't created yet" );

		return adminCollection_.collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, specificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem );
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminCollection_ == null &&
		// Create supporting module
		( adminCollection_ = new AdminCollection( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin collection module" );

		return adminCollection_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected byte collectRelationWords( boolean isExclusiveSpecification, short relationWordTypeNr, short specificationWordTypeNr, WordItem previousRelationWordItem, WordItem currentRelationWordItem, WordItem specificationWordItem )
		{
		if( adminCollection_ == null )
			return startError( 1, null, null, "The admin collection module isn't created yet" );

		return adminCollection_.collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, currentRelationWordItem, specificationWordItem );
		}


	// Protected conclusion methods

	protected boolean hasUnprocessedNegativeConclusion()
		{
		if( adminConclusion_ != null )
			return adminConclusion_.hasUnprocessedNegativeConclusion();

		return false;
		}

	protected byte drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminConclusion_ == null )
			return startError( 1, null, null, "The admin conclusion module isn't created yet" );

		return adminConclusion_.drawNegativeConclusionsFromAnsweredQuestions( userSpecificationItem, generalizationWordItem );
		}

	protected byte drawSimpleNegativeSpanishConclusion( boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminConclusion_ == null )
			return startError( 1, null, null, "The admin conclusion module isn't created yet" );

		return adminConclusion_.drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem );
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, WordItem generalizationWordItem )
		{
		if( adminConclusion_ == null )
			return startError( 1, null, null, "The admin conclusion module isn't created yet" );

		return adminConclusion_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, specificationCollectionNr, generalizationWordItem );
		}

	protected byte drawPossessiveReversibleConclusion( boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isPossessive, boolean isUniqueUserRelation, short assumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminConclusion_ == null )
			return startError( 1, null, null, "The admin conclusion module isn't created yet" );

		return adminConclusion_.drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isArchivedAssignment, boolean isPossessive, boolean isSelfGenerated, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminConclusion_ == null )
			return startError( 1, null, null, "The admin conclusion module isn't created yet" );

		return adminConclusion_.drawSpecificationGeneralizationConclusion( isArchivedAssignment, isPossessive, isSelfGenerated, generalizationWordTypeNr, specificationWordTypeNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isMakingPartOfAssumption, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminConclusion_ == null &&
		// Create supporting module
		( adminConclusion_ = new AdminConclusion( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin conclusion module" );

		return adminConclusion_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isMakingPartOfAssumption, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		if( adminConclusion_ == null )
			return startSpecificationResultError( 1, null, null, "The admin conclusion module isn't created yet" );

		return adminConclusion_.drawCompoundSpecificationSubstitutionConclusion( specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );
		}

	protected SpecificationItem alternativeAnotherPrimarySpecificationItem()
		{
		if( adminConclusion_ != null )
			return adminConclusion_.alternativeAnotherPrimarySpecificationItem();

		return null;
		}

	protected SpecificationItem alternativeJustificationSpecificationItem( boolean hasGeneralizationWordFeminineProperNameEnding, boolean isCheckingForPossessiveSpecification, boolean isCheckingForQuestion, boolean isNegative, boolean isPossessive, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, isCheckingForPossessiveSpecification, isCheckingForQuestion, isNegative, isPossessive, userSpecificationItem, generalizationWordItem, specificationWordItem );

		return null;
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

	protected WordItem adjustedQuestionWordItem()
		{
		if( adminConclusion_ != null )
			return adminConclusion_.adjustedQuestionWordItem();

		return null;
		}


	// Protected context methods

	protected int highestContextNrInAllContextWords()
		{
		int tempContextNr;
		int highestContextNr = Constants.NO_CONTEXT_NR;
		WordItem currentContextWordItem;

		if( ( currentContextWordItem = CommonVariables.firstContextWordItem ) != null )
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

	protected ContextResultType getRelationContext( boolean isArchivedAssignment, boolean isPossessive, boolean isQuestion, boolean isUserSentence, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		if( adminContext_ == null &&
		// Create supporting module
		( adminContext_ = new AdminContext( this ) ) == null )
			return startContextResultError( 1, null, "I failed to create the admin context module" );

		return adminContext_.getRelationContext( isArchivedAssignment, isPossessive, isQuestion, isUserSentence, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, startRelationReadItem );
		}

	protected ContextResultType getSpecificationRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isCompoundCollectionSpanishAmbiguous, boolean isPossessive, boolean isSelfGeneratedAssumption, int specificationCollectionNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminContext_ == null &&
		// Create supporting module
		( adminContext_ = new AdminContext( this ) ) == null )
			return startContextResultError( 1, null, "I failed to create the admin context module" );

		return adminContext_.getSpecificationRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionSpanishAmbiguous, isPossessive, isSelfGeneratedAssumption, specificationCollectionNr, generalizationWordItem, specificationWordItem, relationWordItem );
		}


	// Protected database connection methods
/*
	protected byte storeSentenceInFutureDatabase()
		{
		if( fileList != null )
			return fileList.storeChangesInFutureDatabase();

		// Guide by Grammar list, read list and score list are temporary lists.
		// So, they are not stored.

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

	protected byte executeImperative( boolean isInitializeVariables, short executionListNr, short imperativeVerbParameter, short executionNounWordParameter, short specificationWordTypeNr, int endSolveProgress, String executionString, WordItem generalizationWordItem, WordItem specificationWordItem, ReadItem startRelationWordReadItem, ReadItem endRelationReadItem, SelectionItem executionSelectionItem, SelectionItem actionSelectionItem )
		{
		if( adminImperative_ == null &&
		// Create supporting module
		( adminImperative_ = new AdminImperative( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin imperative module" );

		return adminImperative_.executeImperative( isInitializeVariables, executionListNr, imperativeVerbParameter, executionNounWordParameter, specificationWordTypeNr, endSolveProgress, executionString, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem, executionSelectionItem, actionSelectionItem );
		}


	// Protected language methods

	protected byte authorizeLanguageWord( WordItem authorizationWordItem )
		{
		if( adminLanguage_ == null )
			return startError( 1, null, null, "The admin language module isn't created yet" );

		return adminLanguage_.authorizeLanguageWord( authorizationWordItem );
		}

	protected byte createLanguage( String languageNameString )
		{
		if( adminLanguage_ == null &&
		// Create supporting module
		( adminLanguage_ = new AdminLanguage( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin language module" );

		return adminLanguage_.createLanguage( languageNameString );
		}

	protected byte createLanguageSpecification( WordItem languageNounWordItem )
		{
		if( adminLanguage_ == null )
			return startError( 1, null, null, "The admin language module isn't created yet" );

		return adminLanguage_.createLanguageSpecification( languageNounWordItem );
		}

	protected byte assignLanguage( short newLanguageNr )
		{
		if( adminLanguage_ == null )
			return startError( 1, null, null, "The admin language module isn't created yet" );

		return adminLanguage_.assignLanguage( newLanguageNr );
		}

	protected byte assignLanguage( String languageNameString )
		{
		if( adminLanguage_ == null )
			return startError( 1, null, null, "The admin language module isn't created yet" );

		return adminLanguage_.assignLanguage( languageNameString );
		}


	// Protected query methods

	protected void initializeQueryStringPosition()
		{
		if( adminQuery_ != null )
			adminQuery_.initializeQueryStringPosition();
		}

	protected byte writeInfoTextWithPossibleQueryCommands( String textString )
		{
		if( adminQuery_ == null &&
		// Create supporting module
		( adminQuery_ = new AdminQuery( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin query module" );

		return adminQuery_.writeInfoTextWithPossibleQueryCommands( textString );
		}

	protected byte executeQuery( boolean isSuppressingMessage, boolean isReturningToPosition, boolean isWritingQueryResult, short promptTypeNr, String queryString )
		{
		if( adminQuery_ == null &&
		// Create supporting module
		( adminQuery_ = new AdminQuery( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin query module" );

		return adminQuery_.executeQuery( isSuppressingMessage, isReturningToPosition, isWritingQueryResult, promptTypeNr, queryString );
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
		if( adminReadCreateWords_ == null )
			return startReadResultError( 1, null, "The admin read create words module isn't created yet" );

		return adminReadCreateWords_.createReadWords( grammarString );
		}

	protected ReadResultType readWordFromString( boolean isCheckingForGrammarDefinition, boolean isMergedWord, boolean isSkippingTextString, int startWordPosition, int minimumStringLength, String wordString )
		{
		if( adminReadCreateWords_ == null )
			return startReadResultError( 1, null, "The admin read create words module isn't created yet" );

		return adminReadCreateWords_.readWordFromString( isCheckingForGrammarDefinition, isMergedWord, isSkippingTextString, startWordPosition, minimumStringLength, wordString );
		}

	protected WordResultType addWord( boolean isLanguageWord, boolean isMultipleWord, short previousWordAdjectiveParameter, short previousWordDefiniteArticleParameter, short previousWordIndefiniteArticleParameter, short wordParameter, short wordTypeNr, int wordLength, String wordString )
		{
		if( adminReadCreateWords_ == null )
			return startWordResultError( 1, null, "The admin read create words module isn't created yet" );

		return adminReadCreateWords_.addWord( isLanguageWord, isMultipleWord, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, wordParameter, wordTypeNr, wordLength, wordString );
		}

	protected WordResultType findWordTypeInAllWords( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString, WordItem previousWordItem )
		{
		if( adminReadCreateWords_ == null &&
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this ) ) == null )
			return startWordResultError( 1, null, "I failed to create the admin read create words module" );

		return adminReadCreateWords_.findWordTypeInAllWords( isCheckingAllLanguages, wordTypeNr, wordTypeString, previousWordItem );
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
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read module isn't created yet" );

		return adminReadFile_.compareOutputFileAgainstReferenceFile( testFileNameString );
		}

	protected byte getUserInput( boolean isGuideByGrammar, boolean isPassword, boolean isQuestion, boolean isText, String promptInputString, StringBuffer readStringBuffer )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read module isn't created yet" );

		return adminReadFile_.readLine( true, false, isGuideByGrammar, isPassword, isQuestion, isText, Constants.NO_SENTENCE_NR, promptInputString, readStringBuffer );
		}

	protected byte readAndExecute()
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read module isn't created yet" );

		return adminReadFile_.readAndExecute();
		}

	protected byte readExampleFile( String exampleFileNameString )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read module isn't created yet" );

		return adminReadFile_.readExampleFile( exampleFileNameString );
		}

	protected byte readTestFile( String testFileNameString )
		{
		if( adminReadFile_ == null )
			return startError( 1, null, null, "The admin read module isn't created yet" );

		return adminReadFile_.readTestFile( testFileNameString );
		}

	protected FileResultType readInfoFile( boolean isReportingErrorIfFileDoesNotExist, String infoFileNameString )
		{
		if( adminReadFile_ == null )
			return startFileResultError( 1, null, "The admin read module isn't created yet" );

		return adminReadFile_.readInfoFile( isReportingErrorIfFileDoesNotExist, infoFileNameString );
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
		if( adminReadGrammar_ == null &&
		// Create supporting module
		( adminReadGrammar_ = new AdminReadGrammar( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin read grammar module" );

		return adminReadGrammar_.addGrammar( grammarString );
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
		if( adminReadSentence_ == null &&
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin read sentence module" );

		return adminReadSentence_.processReadSentence( readString );
		}


	// Protected selection methods

	protected byte checkForDuplicateSelection()
		{
		if( adminSelection_ == null )
			return startError( 1, null, null, "The admin selection module isn't created yet" );

		return adminSelection_.checkForDuplicateSelection();
		}

	protected byte createSelectionPart( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short selectionListNr, short imperativeVerbParameter, short prepositionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		if( adminSelection_ == null &&
		// Create supporting module
		( adminSelection_ = new AdminSelection( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin selection module" );

		return adminSelection_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, selectionListNr, imperativeVerbParameter, prepositionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );
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
		if( adminSelection_ == null )
			return startError( 1, null, null, "The admin selection module isn't created yet" );

		return adminSelection_.executeSelection( endSolveProgress, actionSelectionItem );
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
		if( adminSolve_ == null &&
		// Create supporting module
		( adminSolve_ = new AdminSolve( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin solve module" );

		return adminSolve_.solveWord( endSolveProgress, solveWordItem, actionSelectionItem );
		}


	// Protected specification methods

	protected void initializeLinkedWord()
		{
		if( adminSpecification_ != null )
			adminSpecification_.initializeLinkedWord();
		}

	protected byte addUserSpecifications( boolean isAction, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short imperativeVerbParameter, int generalizationContextNr, int specificationContextNr, ReadItem generalizationWordItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		if( adminSpecification_ == null )
			return startError( 1, null, null, "The admin specification module isn't created yet" );

		return adminSpecification_.addUserSpecifications( isAction, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, selectionLevel, selectionListNr, imperativeVerbParameter, generalizationContextNr, specificationContextNr, generalizationWordItem, startSpecificationReadItem, endSpecificationReadItem, startRelationReadItem, endRelationReadItem );
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

		return adminSpecification_.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, generalizationWordItem, specificationWordItem, null ).result;
		}

	protected SpecificationResultType addLanguageSpecification( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminSpecification_ == null &&
		// Create supporting module
		( adminSpecification_ = new AdminSpecification( this ) ) == null )
			return startSpecificationResultError( 1, null, null, "I failed to create the admin specification module" );

		return adminSpecification_.addSpecification( true, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_ASSUMPTION_LEVEL, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, generalizationWordItem, specificationWordItem, null, null );
		}

	protected SpecificationResultType addSelfGeneratedSpecification( boolean hasFeminineOrMasculineProperNameEnding, boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isForcingNewJustification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isSkipAdditionalConclusionOrAskQuestion, boolean isSpecificationGeneralization, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminSpecification_ == null )
			return startSpecificationResultError( 1, null, null, "The admin specification module isn't created yet" );

		return adminSpecification_.addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isForcingNewJustification, isNegative, isPartOf, isPossessive, isUniqueUserRelation, isSkipAdditionalConclusionOrAskQuestion, isSpecificationGeneralization, assumptionLevel, assumptionJustificationTypeNr, conclusionJustificationTypeNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );
		}

	protected SpecificationResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		// Select language or authorization module
		if( specificationWordItem == CommonVariables.predefinedNounLanguageWordItem )
			{
			if( adminLanguage_ == null )
				return startSpecificationResultError( 1, null, null, "The admin language module isn't created yet" );

			return adminLanguage_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );
			}

		if( adminAuthorization_ == null )
			return startSpecificationResultError( 1, null, null, "The admin authorization module isn't created yet" );

		return adminAuthorization_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );
		}

	protected SpecificationResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		// Select language or authorization module
		if( specificationWordItem == CommonVariables.predefinedNounLanguageWordItem )
			{
			if( adminLanguage_ == null )
				return startSpecificationResultError( 1, null, null, "The admin language module isn't created yet" );

			return adminLanguage_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );
			}

		if( adminAuthorization_ == null )
			return startSpecificationResultError( 1, null, null, "The admin authorization module isn't created yet" );

		return adminAuthorization_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );
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
		if( adminWriteJustification_ == null &&
		// Create supporting module
		( adminWriteJustification_ = new AdminWriteJustification( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin write justification module" );

		return adminWriteJustification_.writeJustificationSpecification( justificationSpecificationItem );
		}


	// Protected write specification methods

	protected void initializeAdminWriteVariables()
		{
		if( adminWriteSpecification_ != null )
			adminWriteSpecification_.initializeAdminWriteVariables();
		}

	protected byte answerQuestions()
		{
		if( adminWriteSpecification_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWriteSpecification_.answerQuestions();
		}

	protected byte checkIntegrityOfStoredUserSentence( String readUserSentenceString )
		{
		if( adminWriteSpecification_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWriteSpecification_.checkIntegrityOfStoredUserSentence( readUserSentenceString );
		}

	protected byte markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem userSpecificationItem )
		{
		if( adminWriteSpecification_ == null &&
		// Create supporting module
		( adminWriteSpecification_ = new AdminWriteSpecification( this ) ) == null )
			return startError( 1, null, null, "I failed to create the admin write specification module" );

		return adminWriteSpecification_.markWordsPassingIntegrityCheckOfStoredUserSentence( userSpecificationItem );
		}

	protected byte writeJustificationReport( WordItem justificationWordItem )
		{
		if( adminWriteSpecification_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWriteSpecification_.writeSelfGeneratedInfo( false, true, true, true, true, justificationWordItem );
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions )
		{
		if( adminWriteSpecification_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWriteSpecification_.writeSelfGeneratedInfo( isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions );
		}

	protected byte writeInfoAboutWord( boolean isWritingCurrentSentenceOnly, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, boolean isWritingSpecificationInfo, boolean isWritingRelatedInfo, WordItem writeWordItem )
		{
		if( adminWriteSpecification_ == null )
			return startError( 1, null, null, "The admin write module isn't created yet" );

		return adminWriteSpecification_.writeInfoAboutWord( isWritingCurrentSentenceOnly, isWritingUserSpecifications, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingUserQuestions, isWritingSelfGeneratedQuestions, isWritingSpecificationInfo, isWritingRelatedInfo, writeWordItem );
		}


	// Thinknowlogy methods

	protected String currentLanguageNameString()
		{
		return languageNameString( CommonVariables.currentLanguageNr );
		}

	protected byte interact()
		{
		if( CommonVariables.result == Constants.RESULT_SYSTEM_ERROR )
			return Constants.RESULT_SYSTEM_ERROR;

		readAndExecute();
		// Ignore Constants.RESULT_ERROR. Application will only exit on system error
		CommonVariables.result = Constants.RESULT_OK;
		// Application will not exit on a normal error
		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Listen to me, all you in the distant lands!
 *	Pay attention, you who are far away!
 *	The Lord called me before my birth;
 *	from within the womb he called me by name." (Psalm 49:1)
 *************************************************************************/
