/*
 *	Class:			AdminItem
 *	Parent class:	WordItem
 *	Grand parent:	Item
 *	Purpose:		To process tasks at administration level
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
		( adminReadFile_ = new AdminReadFile( this, this ) ) != null )
			return adminReadFile_.readStartupFile();

		return startErrorInItem( 1, null, null, "I failed to create the admin read module" );
		}

	private byte startup()
		{
		if( readStartupFile() == Constants.RESULT_OK )
			{
			if( predefinedNounInterfaceLanguageWordItem() != null )
				{
				initializeQueryStringPosition();
				CommonVariables.queryStringBuffer = new StringBuffer();

				if( executeQuery( true, false, false, Constants.PRESENTATION_PROMPT_QUERY, ( Constants.EMPTY_STRING + Constants.QUERY_CHAR + Constants.QUERY_ACTIVE_CHAR + Constants.QUERY_WORD_REFERENCE_START_CHAR + predefinedNounInterfaceLanguageWordItem().anyWordTypeString() + Constants.QUERY_WORD_REFERENCE_END_CHAR + Constants.QUERY_LIST_START_CHAR + Constants.WORD_SPECIFICATION_LIST_SYMBOL + Constants.QUERY_LIST_END_CHAR + Constants.QUERY_WORD_START_CHAR ) ) == Constants.RESULT_OK )
					{
					isSystemStartingUp_ = false;

					if( CommonVariables.queryStringBuffer.indexOf( Constants.QUERY_SEPARATOR_SPACE_STRING ) > 0 )	// More than one interface language available
						CommonVariables.interfaceLanguageStringBuffer = new StringBuffer( CommonVariables.queryStringBuffer );
					}
				else
					return addErrorInItem( 1, null, null, "I failed to execute a query" );
				}
			else
				return startSystemErrorInItem( 1, null, null, "The predefined noun interface language word item is undefined" );
			}
		else
			return startSystemErrorInItem( 1, null, null, "I failed to read the startup file" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminItem( short startupGrammarLanguageNr )
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

		for( short adminListNr : Constants.AdminLists )
			adminListArray[adminListNr] = null;

		// Initialization

		Presentation.init();			// Init presentation after restart
		CommonVariables.init();		// Init common variables after restart
		initializeItemVariables( this );

		if( startup() == Constants.RESULT_OK )
			{
			if( startupGrammarLanguageNr > Constants.NO_LANGUAGE_NR &&
			startupGrammarLanguageNr <= nGrammarLanguages() )
				{
				if( assignGrammarAndInterfaceLanguage( startupGrammarLanguageNr ) != Constants.RESULT_OK )
					addErrorInItem( 1, null, null, "I failed to assign the grammar and interface language" );
				}
			}
		else
			addErrorInItem( 1, null, "I failed to start the administrator" );
		}


	// Protected constructible variables

	protected FileList fileList;
	protected ReadList readList;
	protected ScoreList scoreList;
	protected WordList wordList;
	protected SelectionList conditionList;
	protected SelectionList actionList;
	protected SelectionList alternativeList;

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

	protected byte makeExclusiveSpecificationSubstitutionAssumption( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeExclusiveSpecificationSubstitutionAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeGeneralizationAssumptionByGeneralization( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short specificQuestionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, isNegative, isPossessive, specificQuestionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startErrorInItem( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeGeneralizationAssumptionBySpecification( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeGeneralizationAssumptionBySpecification( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startErrorInItem( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeIndirectlyAnsweredQuestionAssumption( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, SpecificationItem userSpecificationItem )
		{
		if( adminAssumption_ != null ||
		// Create supporting module
		( adminAssumption_ = new AdminAssumption( this, this ) ) != null )
			return adminAssumption_.makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, userSpecificationItem );

		return startErrorInItem( 1, null, null, "I failed to create the admin assumption module" );
		}

	protected byte makeOnlyOptionLeftAssumption( boolean isArchivedAssignment, boolean isPossessive, SpecificationItem createdSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem );

		return startErrorInItem( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makePossessiveConditionalSpecificationAssumption( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, SpecificationItem secondarySpecificationItem, SpecificationItem userSpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makePossessiveConditionalSpecificationAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, userSpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected byte makeSuggestiveQuestionAssumption( boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminAssumption_ != null )
			return adminAssumption_.makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( 1, null, null, "The admin assumption module isn't created yet" );
		}

	protected SpecificationResultType addSelfGeneratedSpecification( boolean isAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isUniqueUserRelation, boolean isForcingNewJustification, boolean isSkipAdditionalConclusionOrAskQuestion, short assumptionLevel, short assumptionJustificationTypeNr, short conclusionJustificationTypeNr, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, SpecificationItem primarySpecificationItem, SpecificationItem anotherPrimarySpecificationItem, SpecificationItem secondarySpecificationItem, SpecificationItem anotherSecondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( adminAssumption_ != null )
			return adminAssumption_.addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isUniqueUserRelation, isForcingNewJustification, isSkipAdditionalConclusionOrAskQuestion, assumptionLevel, assumptionJustificationTypeNr, conclusionJustificationTypeNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		specificationResult.result = startErrorInItem( 1, null, null, "The admin assumption module isn't created yet" );
		return specificationResult;
		}


	// Protected authorization methods

	protected boolean isExpertUser()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.isExpertUser();

		return false;
		}

	protected int firstUserSentenceNr()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.firstUserSentenceNr();

		return Constants.NO_SENTENCE_NR;
		}

	protected byte login( WordItem specificationWordItem )
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.login( specificationWordItem );

		return startErrorInItem( 1, null, null, "The admin authorization module isn't created yet" );
		}

	protected byte authorizeWord( WordItem authorizationWordItem )
		{
		if( adminAuthorization_ != null ||
		// Create supporting module
		( adminAuthorization_ = new AdminAuthorization( this, this ) ) != null )
			return adminAuthorization_.authorizeWord( authorizationWordItem );

		return startErrorInItem( 1, null, null, "I failed to create the admin authorization module" );
		}

	protected String currentUserName()
		{
		if( adminAuthorization_ != null )
			return adminAuthorization_.currentUserName();

		return null;	// During startup - before login
		}


	// Protected cleanup methods

	protected void clearDontIncrementCurrentSentenceNr()
		{
		if( adminCleanup_ != null )
			adminCleanup_.clearDontIncrementCurrentSentenceNr();
		}

	protected void checkForChangesDuringThisSentence()
		{
		if( adminCleanup_ != null )
			adminCleanup_.checkForChangesDuringThisSentence();
		}

	protected void deleteRollbackInfo()
		{
		if( adminCleanup_ != null )
			adminCleanup_.deleteRollbackInfo();
		}

	protected void setCurrentItemNr()
		{
		if( adminCleanup_ != null )
			adminCleanup_.setCurrentItemNr();
		}

	protected boolean isDontIncrementCurrentSentenceNr()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.isDontIncrementCurrentSentenceNr();

		return false;
		}

	protected boolean hasFoundAnyChangeDuringThisSentence()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.hasFoundAnyChangeDuringThisSentence();

		return true;	// Default when admin cleanup module isn't created yet
		}

	protected boolean wasUndoOrRedo()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.wasUndoOrRedo();

		return true;	// Default when admin cleanup module isn't created yet
		}

	protected int highestSentenceNr()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.highestSentenceNr();

		return Constants.NO_SENTENCE_NR;
		}

	protected byte cleanupDeletedItems()
		{
		if( adminCleanup_ != null ||
		// Create supporting module
		( adminCleanup_ = new AdminCleanup( this, this ) ) != null )
			return adminCleanup_.cleanupDeletedItems();

		return startErrorInItem( 1, null, null, "I failed to create the admin cleanup module" );
		}

	protected byte deleteAllTemporaryLists()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.deleteAllTemporaryLists();

		return startErrorInItem( 1, null, null, "The admin cleanup module isn't created yet" );
		}

	protected byte deleteUnusedInterpretations( boolean isDeleteAllActiveWordTypes )
		{
		if( adminCleanup_ != null )
			return adminCleanup_.deleteUnusedInterpretations( isDeleteAllActiveWordTypes );

		return startErrorInItem( 1, null, null, "The admin cleanup module isn't created yet" );
		}

	protected byte deleteSentences( boolean isAvailableForRollback, int lowestSentenceNr )
		{
		if( adminCleanup_ != null )
			return adminCleanup_.deleteSentences( isAvailableForRollback, lowestSentenceNr );

		return startErrorInItem( 1, null, null, "The admin cleanup module isn't created yet" );
		}

	protected byte undoLastSentence()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.undoLastSentence();

		return startErrorInItem( 1, null, null, "The admin cleanup module isn't created yet" );
		}

	protected byte redoLastUndoneSentence()
		{
		if( adminCleanup_ != null )
			return adminCleanup_.redoLastUndoneSentence();

		return startErrorInItem( 1, null, null, "The admin cleanup module isn't created yet" );
		}


	// Protected collection methods

	protected CollectionResultType collectSpecificationWords( boolean isExclusiveSpecification, boolean isPossessive, boolean isQuestion, boolean isSpecificationGeneralization, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem compoundGeneralizationWordItem, WordItem generalizationWordItem, WordItem previousSpecificationWordItem, WordItem currentSpecificationWordItem )
		{
		CollectionResultType collectionResult = new CollectionResultType();

		if( adminCollection_ != null )
			return adminCollection_.collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, specificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem );

		collectionResult.result = startErrorInItem( 1, null, null, "The admin collection module isn't created yet" );
		return collectionResult;
		}

	protected byte collectGeneralizationWordWithPreviousOne( boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminCollection_ != null ||
		// Create supporting module
		( adminCollection_ = new AdminCollection( this, this ) ) != null )
			return adminCollection_.collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem );

		return startErrorInItem( 1, null, null, "I failed to create the admin collection module" );
		}

	protected byte collectSpecificationStrings( boolean isExclusiveSpecification, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem generalizationWordItem, String previousSpecificationString, String currentSpecificationString )
		{
		if( adminCollection_ != null )
			return adminCollection_.collectSpecificationStrings( isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationWordItem, previousSpecificationString, currentSpecificationString );

		return startErrorInItem( 1, null, null, "The admin collection module isn't created yet" );
		}

	protected byte collectRelationWords( boolean isExclusiveSpecification, short relationWordTypeNr, short specificationWordTypeNr, WordItem previousRelationWordItem, WordItem currentRelationWordItem, WordItem specificationWordItem )
		{
		if( adminCollection_ != null )
			return adminCollection_.collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, currentRelationWordItem, specificationWordItem );

		return startErrorInItem( 1, null, null, "The admin collection module isn't created yet" );
		}


	// Protected conclusion methods

	protected void initializeAdminConclusionVariables()
		{
		if( adminConclusion_ != null )
			adminConclusion_.initializeAdminConclusionVariables();
		}

	protected byte drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem userSpecificationItem, WordItem generalizationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawNegativeConclusionsFromAnsweredQuestions( userSpecificationItem, generalizationWordItem );

		return startErrorInItem( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawOnlyOptionLeftConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, int specificationCollectionNr, WordItem generalizationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, specificationCollectionNr, generalizationWordItem );

		return startErrorInItem( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawPossessiveReversibleConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUniqueUserRelation, short assumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationContextNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueUserRelation, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawSpecificationGeneralizationConclusion( boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isPossessive, short generalizationWordTypeNr, short specificationWordTypeNr, SpecificationItem secondarySpecificationItem, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminConclusion_ != null )
			return adminConclusion_.drawSpecificationGeneralizationConclusion( isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );

		return startErrorInItem( 1, null, null, "The admin conclusion module isn't created yet" );
		}

	protected byte drawSpecificationSubstitutionConclusionOrAskQuestion( boolean isAssumption, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		if( adminConclusion_ != null ||
		// Create supporting module
		( adminConclusion_ = new AdminConclusion( this, this ) ) != null )
			return adminConclusion_.drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( 1, null, null, "I failed to create the admin conclusion module" );
		}

	protected SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( boolean isNegative, boolean isPartOf, boolean isPossessive, short specificationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, WordItem specificationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( adminConclusion_ != null )
			return adminConclusion_.drawCompoundSpecificationSubstitutionConclusion( isNegative, isPartOf, isPossessive, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		specificationResult.result = startErrorInItem( 1, null, null, "The admin conclusion module isn't created yet" );
		return specificationResult;
		}


	// Protected context methods

	protected void initializeAdminContextVariables()
		{
		if( adminContext_ != null )
			adminContext_.initializeAdminContextVariables();
		}

	protected int highestContextNr()
		{
		if( adminContext_ != null )
			return adminContext_.highestContextNr();

		return Constants.NO_CONTEXT_NR;
		}

	protected ContextResultType addPronounContext( short contextWordTypeNr, WordItem contextWordItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( adminContext_ != null ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, this ) ) != null )
			return adminContext_.addPronounContext( contextWordTypeNr, contextWordItem );

		contextResult.result = startErrorInItem( 1, null, null, "I failed to create the admin grammar module" );
		return contextResult;
		}

	protected ContextResultType getRelationContext( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPossessive, short questionParameter, short specificationWordTypeNr, short relationWordTypeNr, int relationContextNr, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( adminContext_ != null ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, this ) ) != null )
			return adminContext_.getRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordTypeNr, relationWordTypeNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		contextResult.result = startErrorInItem( 1, null, null, "The admin context module isn't created yet" );
		return contextResult;
		}

	protected ContextResultType getRelationContext( boolean isAssignment, boolean isExclusiveSpecification, boolean isNegative, boolean isPossessive, boolean isUserSentence, short questionParameter, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, ReadItem startRelationReadItem )
		{
		ContextResultType contextResult = new ContextResultType();

		if( adminContext_ != null ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, this ) ) != null )
			return adminContext_.getRelationContext( isAssignment, isExclusiveSpecification, isNegative, isPossessive, isUserSentence, questionParameter, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, startRelationReadItem );

		contextResult.result = startErrorInItem( 1, null, null, "The admin context module isn't created yet" );
		return contextResult;
		}


	// Protected database connection methods
/*
	protected byte storeSentenceInFutureDataBase()
		{
		// Not fully implemented yet
		if( fileList != null )
			return fileList.storeChangesInFutureDataBase();

		// Read list and score list aren't stored, because these are temporary lists

		if( wordList != null )
			return wordList.storeChangesInFutureDataBase();

		if( conditionList != null )
			return conditionList.storeChangesInFutureDataBase();

		if( actionList != null )
			return actionList.storeChangesInFutureDataBase();

		if( alternativeList != null )
			return alternativeList.storeChangesInFutureDataBase();

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
		( adminImperative_ = new AdminImperative( this, this ) ) != null )
			return adminImperative_.executeImperative( isInitializeVariables, executionListNr, imperativeParameter, specificationWordParameter, specificationWordTypeNr, endSolveProgress, executionString, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem, executionSelectionItem, actionSelectionItem );

		return startErrorInItem( 1, null, null, "I failed to create the admin imperative module" );
		}


	// Protected language methods

	protected byte authorizeLanguageWord( WordItem authorizationWordItem )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.authorizeLanguageWord( authorizationWordItem );

		return startErrorInItem( 1, null, null, "The admin language module isn't created yet" );
		}

	protected byte createGrammarLanguage( String languageNameString )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.createGrammarLanguage( languageNameString );

		return startErrorInItem( 1, null, null, "The admin language module isn't created yet" );
		}

	protected byte createInterfaceLanguage( String languageNameString )
		{
		if( adminLanguage_ != null ||
		// Create supporting module
		( adminLanguage_ = new AdminLanguage( this, this ) ) != null )
			return adminLanguage_.createInterfaceLanguage( languageNameString );

		return startErrorInItem( 1, null, null, "I failed to create the admin language module" );
		}

	protected byte createLanguageSpecification( WordItem languageWordItem, WordItem languageNounWordItem )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.createLanguageSpecification( languageWordItem, languageNounWordItem );

		return startErrorInItem( 1, null, null, "The admin language module isn't created yet" );
		}

	protected byte assignGrammarAndInterfaceLanguage( short newLanguageNr )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.assignGrammarAndInterfaceLanguage( newLanguageNr );

		return startErrorInItem( 1, null, null, "The admin language module isn't created yet" );
		}

	protected byte assignGrammarAndInterfaceLanguage( String languageNameString )
		{
		if( adminLanguage_ != null )
			return adminLanguage_.assignGrammarAndInterfaceLanguage( languageNameString );

		return startErrorInItem( 1, null, null, "The admin language module isn't created yet" );
		}


	// Protected query methods

	protected void initializeQueryStringPosition()
		{
		if( adminQuery_ != null )
			adminQuery_.initializeQueryStringPosition();
		}

	protected byte writeTextWithPossibleQueryCommands( short promptTypeNr, String textString )
		{
		if( adminQuery_ != null ||
		// Create supporting module
		( adminQuery_ = new AdminQuery( this, this ) ) != null )
			return adminQuery_.writeTextWithPossibleQueryCommands( promptTypeNr, textString );

		return startErrorInItem( 1, null, null, "I failed to create the admin query module" );
		}

	protected byte executeQuery( boolean isSuppressingMessage, boolean returnToPosition, boolean isWritingQueryResult, short promptTypeNr, String queryString )
		{
		if( adminQuery_ != null )
			return adminQuery_.executeQuery( isSuppressingMessage, returnToPosition, isWritingQueryResult, promptTypeNr, queryString );

		return startErrorInItem( 1, null, null, "The admin query module isn't created yet" );
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

	protected boolean hasPassedGrammarIntegrityCheck()
		{
		if( readList != null )
			return readList.hasPassedGrammarIntegrityCheck();

		return false;
		}

	protected boolean isImperativeSentence()
		{
		if( readList != null )
			return readList.isImperativeSentence();

		return false;
		}

	protected ReadResultType createReadWord( short wordOrderNr, short wordTypeNr, String textString, WordItem readWordItem )
		{
		ReadResultType readResult = new ReadResultType();

		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.createReadWord( wordOrderNr, wordTypeNr, 0, textString, readWordItem );

		readResult.result = startErrorInItem( 1, null, null, "The admin read create words module isn't created yet" );
		return readResult;
		}

	protected ReadResultType createReadWords( String grammarString )
		{
		ReadResultType readResult = new ReadResultType();

		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.createReadWords( grammarString );

		readResult.result = startErrorInItem( 1, null, null, "The admin read create words module isn't created yet" );
		return readResult;
		}

	protected ReadResultType readWordFromString( boolean checkForGrammarDefinition, boolean skipDoubleQuotes, int startWordPosition, int minimumStringLength, String wordString )
		{
		ReadResultType readResult = new ReadResultType();

		if( adminReadCreateWords_ != null )
			return adminReadCreateWords_.readWordFromString( checkForGrammarDefinition, skipDoubleQuotes, startWordPosition, minimumStringLength, wordString );

		readResult.result = startErrorInItem( 1, null, null, "The admin read create words module isn't created yet" );
		return readResult;
		}

	protected WordResultType addWord( boolean isLanguageWord, boolean isMultipleWord, short previousWordAdjectiveParameter, short previousWordDefiniteArticleParameter, short previousWordIndefiniteArticleParameter, short wordParameter, short wordTypeNr, int wordLength, String wordString )
		{
		WordResultType wordResult = new WordResultType();

		if( adminReadCreateWords_ != null ||
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this, this ) ) != null )
			return adminReadCreateWords_.addWord( isLanguageWord, isMultipleWord, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, wordParameter, wordTypeNr, wordLength, wordString );

		wordResult.result = startErrorInItem( 1, null, null, "I failed to create the admin read create words module" );
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

	protected byte readExampleFile( String exampleFileNameString )
		{
		if( adminReadFile_ != null )
			return adminReadFile_.readExampleFile( exampleFileNameString );

		return startErrorInItem( 1, null, null, "The admin read module isn't created yet" );
		}

	protected byte readAndExecute()
		{
		if( adminReadFile_ != null )
			return adminReadFile_.readAndExecute();

		return startErrorInItem( 1, null, null, "The admin read module isn't created yet" );
		}

	protected byte getUserInput( boolean isGuideByGrammarPrompt, boolean isPassword, boolean isQuestion, boolean isText, String promptInputString, StringBuffer readStringBuffer )
		{
		if( adminReadFile_ != null )
			return adminReadFile_.readLine( true, false, isGuideByGrammarPrompt, isPassword, isQuestion, isText, Constants.NO_SENTENCE_NR, promptInputString, readStringBuffer );

		return startErrorInItem( 1, null, null, "The admin read module isn't created yet" );
		}

	protected FileResultType readInfoFile( boolean isReportErrorIfFileDoesNotExist, String infoFileNameString )
		{
		FileResultType fileResult = new FileResultType();

		if( adminReadFile_ != null )
			return adminReadFile_.readInfoFile( isReportErrorIfFileDoesNotExist, infoFileNameString );

		fileResult.result = startErrorInItem( 1, null, null, "The admin read module isn't created yet" );
		return fileResult;
		}


	// Protected read grammar methods

	protected byte addGrammar( String grammarString )
		{
		if( adminReadGrammar_ != null ||
		// Create supporting module
		( adminReadGrammar_ = new AdminReadGrammar( this, this ) ) != null )
			return adminReadGrammar_.addGrammar( grammarString );

		return startErrorInItem( 1, null, null, "I failed to create the admin read grammar module" );
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

	protected WordItem predefinedNounExpertWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounExpertWordItem();

		return null;
		}

	protected WordItem predefinedNounInterfaceLanguageWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedNounInterfaceLanguageWordItem();

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

	protected WordItem predefinedVerbLoginWordItem()
		{
		if( adminReadGrammar_ != null )
			return adminReadGrammar_.predefinedVerbLoginWordItem();

		return null;
		}


	// Protected read sentence methods

	protected void dontDeletedRollbackInfo()
		{
		if( adminReadSentence_ != null )
			adminReadSentence_.dontDeletedRollbackInfo();
		}

	protected boolean isExclusiveUserSpecification()
		{
		if( adminReadSentence_ != null )
			return adminReadSentence_.isExclusiveSpecification();

		return false;
		}

	protected boolean isUserSentencePossessive()
		{
		if( adminReadSentence_ != null )
			return adminReadSentence_.isUserSentencePossessive();

		return false;
		}

	protected byte processReadSentence( String readString )
		{
		if( adminReadSentence_ != null ||
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this, this ) ) != null )
			return adminReadSentence_.processReadSentence( readString );

		return startErrorInItem( 1, null, null, "I failed to create the admin read sentence module" );
		}

	protected WordResultType addMultipleWord( short wordParameter, short wordTypeNr, WordItem firstWordItem, WordItem secondWordItem, WordItem existingMultipleWordItem )
		{
		WordResultType wordResult = new WordResultType();

		if( adminReadSentence_ != null ||
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this, this ) ) != null )
			return adminReadSentence_.addMultipleWord( wordParameter, wordTypeNr, wordTypeNr, firstWordItem, secondWordItem, existingMultipleWordItem );

		wordResult.result = startErrorInItem( 1, null, null, "I failed to create the admin read sentence module" );
		return wordResult;
		}


	// Protected selection methods

	protected byte checkForDuplicateSelection()
		{
		if( adminSelection_ != null )
			return adminSelection_.checkForDuplicateSelection();

		return startErrorInItem( 1, null, null, "The admin selection module isn't created yet" );
		}

	protected byte createSelectionTextPart( boolean isAction, boolean isNewStart, short selectionLevel, short selectionListNr, short imperativeParameter, String specificationString )
		{
		if( adminSelection_ != null )
			return adminSelection_.createSelectionPart( isAction, false, false, false, false, isNewStart, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, selectionLevel, selectionListNr, imperativeParameter, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_TEXT, Constants.WORD_TYPE_TEXT, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, null, null, specificationString );

		return startErrorInItem( 1, null, null, "The admin selection module isn't created yet" );
		}

	protected byte createSelectionPart( boolean isAction, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isFirstComparisonPart, boolean isNewStart, boolean isNegative, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isValueSpecification, short assumptionLevel, short selectionLevel, short selectionListNr, short imperativeParameter, short prepositionParameter, short specificationWordParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		if( adminSelection_ != null ||
		// Create supporting module
		( adminSelection_ = new AdminSelection( this, this ) ) != null )
			return adminSelection_.createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		return startErrorInItem( 1, null, null, "I failed to create the admin selection module" );
		}

	protected byte executeSelections()
		{
		if( adminSelection_ != null )
			return adminSelection_.executeSelection( Constants.MAX_PROGRESS, null );

		return Constants.RESULT_OK;
		}

	protected byte executeSelection( int endSolveProgress, SelectionItem actionSelectionItem )
		{
		if( adminSelection_ != null )
			return adminSelection_.executeSelection( endSolveProgress, actionSelectionItem );

		return startErrorInItem( 1, null, null, "The admin selection module isn't created yet" );
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

	protected byte canWordBeSolved( WordItem solveWordItem )
		{
		if( adminSolve_ != null )
			return adminSolve_.canWordBeSolved( solveWordItem );

		return startErrorInItem( 1, null, null, "The admin solve module isn't created yet" );
		}

	protected byte solveWord( int endSolveProgress, WordItem solveWordItem, SelectionItem actionSelectionItem )
		{
		if( adminSolve_ != null )
			return adminSolve_.solveWord( endSolveProgress, solveWordItem, actionSelectionItem );

		return startErrorInItem( 1, null, null, "The admin solve module isn't created yet" );
		}

	protected byte findPossibilityToSolveWord( boolean isAddScores, boolean isInverted, boolean duplicatesAllowed, boolean prepareSort, short solveStrategyParameter, WordItem solveWordItem )
		{
		if( adminSolve_ != null )
			return adminSolve_.findPossibilityToSolveWord( isAddScores, isInverted, duplicatesAllowed, prepareSort, solveStrategyParameter, solveWordItem );

		return startErrorInItem( 1, null, null, "The admin solve module isn't created yet" );
		}

	protected SelectionResultType checkCondition( SelectionItem conditionSelectionItem )
		{
		SelectionResultType selectionResult = new SelectionResultType();

		if( adminSolve_ != null ||
		// Create supporting module
		( adminSolve_ = new AdminSolve( this, this ) ) != null )
			return adminSolve_.checkCondition( conditionSelectionItem );

		selectionResult.result = startErrorInItem( 1, null, null, "I failed to create the admin solve module" );
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

	protected int mostRecentRelationContextNr()
		{
		if( adminSpecification_ != null )
			return adminSpecification_.mostRecentRelationContextNr();

		return Constants.NO_CONTEXT_NR;
		}

	protected byte addUserSpecifications( boolean isAction, boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isNewStart, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isUserSpecificationExclusivelyFeminineOrMasculine, short assumptionLevel, short prepositionParameter, short questionParameter, short selectionLevel, short selectionListNr, short imperativeParameter, short specificationWordParameter, int generalizationContextNr, int specificationContextNr, ReadItem generalizationWordItem, ReadItem startSpecificationReadItem, ReadItem endSpecificationReadItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		if( adminSpecification_ != null )
			return adminSpecification_.addUserSpecifications( isAction, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, assumptionLevel, prepositionParameter, questionParameter, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, generalizationContextNr, specificationContextNr, generalizationWordItem, startSpecificationReadItem, endSpecificationReadItem, startRelationReadItem, endRelationReadItem );

		return startErrorInItem( 1, null, null, "The admin specification module isn't created yet" );
		}

	protected byte assignSelectionSpecification( SelectionItem assignmentSelectionItem )
		{
		if( adminSpecification_ != null )
			return adminSpecification_.assignSelectionSpecification( assignmentSelectionItem );

		return startErrorInItem( 1, null, null, "The admin specification module isn't created yet" );
		}

	protected byte assignSpecification( WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( adminSpecification_ != null )
			return adminSpecification_.assignSpecification( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_UNDEFINED, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, generalizationWordItem, specificationWordItem, null ).result;

		return startErrorInItem( 1, null, null, "The admin specification module isn't created yet" );
		}

	protected SpecificationResultType addSpecification( boolean isAssignment, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, short prepositionParameter, short questionParameter, short userAssumptionLevel, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( adminSpecification_ != null ||
		// Create supporting module
		( adminSpecification_ = new AdminSpecification( this, this ) ) != null )
			return adminSpecification_.addSpecification( isAssignment, false, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueRelation, false, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		specificationResult.result = startErrorInItem( 1, null, null, "I failed to create the admin specification module" );
		return specificationResult;
		}

	protected SpecificationResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueRelation, boolean isUserSpecificationExclusivelyFeminineOrMasculine, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( specificationWordItem == CommonVariables.predefinedNounGrammarLanguageWordItem ||
		specificationWordItem == predefinedNounInterfaceLanguageWordItem() )
			{
			if( adminLanguage_ != null )
				return adminLanguage_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startErrorInItem( 1, null, null, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != null )
				return adminAuthorization_.addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startErrorInItem( 1, null, null, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( specificationWordItem == CommonVariables.predefinedNounGrammarLanguageWordItem ||
		specificationWordItem == predefinedNounInterfaceLanguageWordItem() )
			{
			if( adminLanguage_ != null )
				return adminLanguage_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startErrorInItem( 1, null, null, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != null )
				return adminAuthorization_.assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startErrorInItem( 1, null, null, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = CommonVariables.result;
		return specificationResult;
		}

	protected SpecificationItem userSpecificationItem()
		{
		if( adminSpecification_ != null )
			return adminSpecification_.userSpecificationItem();

		return null;
		}


	// Protected write justification methods

	protected byte writeJustificationSpecification( String justificationSentenceString, SpecificationItem justificationSpecificationItem )
		{
		if( adminWriteJustification_ != null ||
		// Create supporting module
		( adminWriteJustification_ = new AdminWriteJustification( this ) ) != null )
			return adminWriteJustification_.writeJustificationSpecification( justificationSentenceString, justificationSpecificationItem );

		return startErrorInItem( 1, null, null, "I failed to create the admin write justification module" );
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

		return startErrorInItem( 1, null, null, "The admin write module isn't created yet" );
		}

	protected byte checkForStoreAndRetrieveIntegrity( boolean isQuestion, String storeAndRetrieveIntegrityString )
		{
		if( adminWriteSpecification_ != null ||
		// Create supporting module
		( adminWriteSpecification_ = new AdminWriteSpecification( this, this ) ) != null )
			return adminWriteSpecification_.checkForStoreAndRetrieveIntegrity( isQuestion, storeAndRetrieveIntegrityString );

		return startErrorInItem( 1, null, null, "I failed to create the admin write specification module" );
		}

	protected byte writeJustificationReport( WordItem justificationWordItem )
		{
		if( adminWriteSpecification_ != null )
			return adminWriteSpecification_.writeSelfGeneratedInfo( false, true, true, true, true, justificationWordItem );

		return startErrorInItem( 1, null, null, "The admin write module isn't created yet" );
		}

	protected byte writeSelfGeneratedInfo( boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingSelfGeneratedQuestions )
		{
		if( adminWriteSpecification_ != null )
			return adminWriteSpecification_.writeSelfGeneratedInfo( isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions );

		return startErrorInItem( 1, null, null, "The admin write module isn't created yet" );
		}

	protected byte writeInfoAboutWord( boolean isWritingCurrentSentenceOnly, boolean isWritingUserSpecifications, boolean isWritingSelfGeneratedConclusions, boolean isWritingSelfGeneratedAssumptions, boolean isWritingUserQuestions, boolean isWritingSelfGeneratedQuestions, boolean isWritingSpecificationInfo, boolean isWritingRelatedInfo, String storeAndRetrieveIntegrityString, WordItem writeWordItem )
		{
		if( adminWriteSpecification_ != null )
			return adminWriteSpecification_.writeInfoAboutWord( isWritingCurrentSentenceOnly, isWritingUserSpecifications, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingUserQuestions, isWritingSelfGeneratedQuestions, isWritingSpecificationInfo, isWritingRelatedInfo, storeAndRetrieveIntegrityString, writeWordItem );

		return startErrorInItem( 1, null, null, "The admin write module isn't created yet" );
		}


	// Thinknowlogy methods

	protected byte interact()
		{
		if( CommonVariables.result != Constants.RESULT_SYSTEM_ERROR )
			{
			readAndExecute();
			CommonVariables.result = Constants.RESULT_OK;	// Ignore Constants.RESULT_ERROR. Application will only exit on system error
			}

		return CommonVariables.result;
		}
	};

/*************************************************************************
 *
 *	"Listen to me, all you in the distant lands!
 *	Pay attention, you who are far away!
 *	The Lord called me before my birth;
 *	from within the womb he called me by name." (Psalm 49:1)
 *
 *************************************************************************/
