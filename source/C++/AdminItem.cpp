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

#include "AdminAssumption.cpp"
#include "AdminAuthorization.cpp"
#include "AdminCleanup.cpp"
#include "AdminCollection.cpp"
#include "AdminConclusion.cpp"
#include "AdminContext.cpp"
#include "AdminImperative.cpp"
#include "AdminLanguage.cpp"
#include "AdminQuery.cpp"
#include "AdminReadCreateWords.cpp"
#include "AdminReadFile.cpp"
#include "AdminReadGrammar.cpp"
#include "AdminReadSentence.cpp"
#include "AdminReasoning.cpp"
#include "AdminSelection.cpp"
#include "AdminSolve.cpp"
#include "AdminSpecification.cpp"
#include "AdminWriteJustification.cpp"
#include "AdminWriteSpecification.cpp"

	// Private functions

	ResultType AdminItem::readStartupFile()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readStartupFile";

		if( adminReadFile_ != NULL ||
		// Create supporting module
		( adminReadFile_ = new AdminReadFile( this, commonVariables() ) ) != NULL )
			return adminReadFile_->readStartupFile();

		return startError( functionNameString, NULL, NULL, "I failed to create the admin read module" );
		}

	ResultType AdminItem::startup()
		{
		char titleString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "startup";

		// Initialize the random number generator with the time
		srand( (unsigned int)time( NULL ) );
		sprintf( titleString, "\n %s %s", PRODUCT_NAME, VERSION_NAME );

		if( commonVariables()->presentation->writeDiacriticalText( PRESENTATION_PROMPT_INFO, titleString ) == RESULT_OK )
			{
			if( readStartupFile() == RESULT_OK )
				{
				if( startupLanguageNameString != NULL &&
				strlen( startupLanguageNameString ) > 0 )
					{
					if( assignLanguage( startupLanguageNameString ) != RESULT_OK )
						// No show stopper
						addError( functionNameString, NULL, NULL, "I failed to assign the language" );
					}

				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_START_MESSAGE ) != RESULT_OK )
					// No show stopper
					addError( functionNameString, NULL, NULL, "I failed to write the start interface text" );

				// Try to login without password
				if( login( NULL ) == RESULT_OK )
					{
					if( commonVariables()->currentUserNr > NO_USER_NR )
						isSystemStartingUp_ = false;
					else
						return startSystemError( functionNameString, NULL, NULL, "No user is logged in" );
					}
				else
					return startSystemError( functionNameString, NULL, NULL, "I failed to login" );
				}
			else
				return startSystemError( functionNameString, NULL, NULL, "I failed to read the startup file" );
			}
		else
			return startSystemError( functionNameString, NULL, NULL, "I failed to write the title text" );

		return RESULT_OK;
		}


	// Constructor / deconstructor

	AdminItem::AdminItem( char *_startupLanguageNameString )
		{
		CommonVariables *commonVariablesModule;

		// Private constructible variables

		isSystemStartingUp_ = true;

		adminAssumption_ = NULL;
		adminAuthorization_ = NULL;
		adminCleanup_ = NULL;
		adminCollection_ = NULL;
		adminConclusion_ = NULL;
		adminContext_ = NULL;
		adminImperative_ = NULL;
		adminLanguage_ = NULL;
		adminQuery_ = NULL;
		adminReadFile_ = NULL;
		adminReadGrammar_ = NULL;
		adminReadCreateWords_ = NULL;
		adminReadSentence_ = NULL;
		adminReasoning_ = NULL;
		adminSelection_ = NULL;
		adminSolve_ = NULL;
		adminSpecification_ = NULL;
		adminWriteJustification_ = NULL;
		adminWriteSpecification_ = NULL;

		// Protected constructible variables

		fileList = NULL;
		readList = NULL;
		scoreList = NULL;
		wordList = NULL;
		conditionList = NULL;
		actionList = NULL;
		alternativeList = NULL;

		startupLanguageNameString = _startupLanguageNameString;

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			adminListArray[adminListNr] = NULL;

		// Initialization

		if( ( commonVariablesModule = new CommonVariables() ) != NULL )
			{
			initializeItemVariables( "AdminItem", commonVariablesModule, this );

			if( ( commonVariablesModule->presentation = new Presentation( commonVariablesModule ) ) != NULL )
				{
				if( startup() != RESULT_OK )
					addError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to start the administrator" );
				}
			else
				startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the presentation module" );
			}
		else
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the common variables module" );
		}

	AdminItem::~AdminItem()
		{
		if( commonVariables() != NULL )
			{
			if( commonVariables()->presentation != NULL )
				delete commonVariables()->presentation;

			delete commonVariables();
			}

		if( adminAssumption_ != NULL )
			delete adminAssumption_;
		if( adminAuthorization_ != NULL )
			delete adminAuthorization_;
		if( adminCleanup_ != NULL )
			delete adminCleanup_;
		if( adminCollection_ != NULL )
			delete adminCollection_;
		if( adminConclusion_ != NULL )
			delete adminConclusion_;
		if( adminContext_ != NULL )
			delete adminContext_;
		if( adminImperative_ != NULL )
			delete adminImperative_;
		if( adminLanguage_ != NULL )
			delete adminLanguage_;
		if( adminQuery_ != NULL )
			delete adminQuery_;
		if( adminReadFile_ != NULL )
			delete adminReadFile_;
		if( adminReadGrammar_ != NULL )
			delete adminReadGrammar_;
		if( adminReadCreateWords_ != NULL )
			delete adminReadCreateWords_;
		if( adminReadSentence_ != NULL )
			delete adminReadSentence_;
		if( adminReasoning_ != NULL )
			delete adminReasoning_;
		if( adminSelection_ != NULL )
			delete adminSelection_;
		if( adminSolve_ != NULL )
			delete adminSolve_;
		if( adminSpecification_ != NULL )
			delete adminSpecification_;
		if( adminWriteJustification_ != NULL )
			delete adminWriteJustification_;
		if( adminWriteSpecification_ != NULL )
			delete adminWriteSpecification_;

		if( fileList != NULL )
			delete fileList;
		if( readList != NULL )
			delete readList;
		if( scoreList != NULL )
			delete scoreList;
		if( wordList != NULL )
			delete wordList;
		if( conditionList != NULL )
			delete conditionList;
		if( actionList != NULL )
			delete actionList;
		if( alternativeList != NULL )
			delete alternativeList;
		}


	// Protected common functions

	bool AdminItem::isSystemStartingUp()
		{
		return isSystemStartingUp_;
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


	// Protected error functions

	ResultType AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		if( commonVariables() != NULL &&
		commonVariables()->presentation != NULL )
			commonVariables()->presentation->showError( adminListChar( adminListNr ), ( moduleNameString == NULL ? classNameString() : moduleNameString ), ( moduleNameString == NULL ? superClassNameString() : NULL ), NULL, functionNameString, errorString );
		else
			fprintf( stderr, "\nClass:\t%s\nSubclass:\t%s\nFunction:\t%s\nError:\t\t%s.\n", classNameString(), superClassNameString(), functionNameString, errorString );

		return ( commonVariables() == NULL ? RESULT_ERROR : commonVariables()->result );
		}

	ResultType AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	ResultType AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	ResultType AdminItem::addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	ResultType AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		addErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, errorString );

		if( commonVariables() != NULL )
		commonVariables()->result = RESULT_ERROR;

		return RESULT_ERROR;
		}

	ResultType AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return startErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	ResultType AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}

	ResultType AdminItem::startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 )
		{
		sprintf( tempString, "%s%u%s%u", errorString1, number1, errorString2, number2 );
		return startErrorWithAdminListNr( adminListNr, functionNameString, moduleNameString, tempString );
		}


	// Protected assumption functions

	void AdminItem::initializeAdminAssumptionVariables()
		{
		if( adminAssumption_ != NULL )
			adminAssumption_->initializeAdminAssumptionVariables();
		}

	bool AdminItem::hasFoundSuggestiveQuestionAssumption()
		{
		if( adminAssumption_ != NULL )
			return adminAssumption_->hasFoundSuggestiveQuestionAssumption();

		return false;
		}

	ResultType AdminItem::makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeExclusiveSpecificationSubstitutionAssumption( isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumptionByGeneralization";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

		if( adminAssumption_ != NULL ||
		// Create supporting module
		( adminAssumption_ = new AdminAssumption( this, commonVariables() ) ) != NULL )
			return adminAssumption_->makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, userSpecificationItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin assumption module" );
		}

	ResultType AdminItem::makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *createdSpecificationItem, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makePossessiveConditionalSpecificationAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makePossessiveConditionalSpecificationAssumption( isArchivedAssignment, isExclusiveSpecification, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, secondarySpecificationItem, userSpecificationItem, generalizationWordItem, relationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeSpecificationSubstitutionPartOfAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSpecificationSubstitutionPartOfAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeSpecificationSubstitutionPartOfAssumption( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	SpecificationItem *AdminItem::oppositePossessiveDefinitionSpecificationItem()
		{
		if( adminAssumption_ != NULL )
			return adminAssumption_->oppositePossessiveDefinitionSpecificationItem();

		return NULL;
		}


	// Protected authorization functions

	unsigned int AdminItem::firstSentenceNrOfCurrentUser()
		{
		if( adminAuthorization_ != NULL )
			return adminAuthorization_->firstSentenceNrOfCurrentUser();

		return NO_SENTENCE_NR;
		}

	ResultType AdminItem::login( WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "login";

		if( adminAuthorization_ != NULL )
			return adminAuthorization_->login( specificationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin authorization module isn't created yet" );
		}

	ResultType AdminItem::authorizeWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "authorizeWord";

		if( adminAuthorization_ != NULL ||
		// Create supporting module
		( adminAuthorization_ = new AdminAuthorization( this, commonVariables() ) ) != NULL )
			return adminAuthorization_->authorizeWord( authorizationWordItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin authorization module" );
		}

	char *AdminItem::currentUserName()
		{
		if( adminAuthorization_ != NULL )
			return adminAuthorization_->currentUserName();

		// During startup, before login
		return NULL;
		}


	// Protected cleanup functions

	void AdminItem::checkForChangesMadeByThisSentence()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->checkForChangesMadeByThisSentence();
		}

	void AdminItem::clearAllTemporaryLists()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->clearAllTemporaryLists();
		}

	void AdminItem::decrementCurrentSentenceNr()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->decrementCurrentSentenceNr();
		}

	void AdminItem::setCurrentItemNr()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->setCurrentItemNr();
		}

	bool AdminItem::hasFoundAnyChangeMadeByThisSentence()
		{
		if( adminCleanup_ != NULL )
			return adminCleanup_->hasFoundAnyChangeMadeByThisSentence();

		// Default if admin cleanup module isn't created yet
		return true;
		}

	bool AdminItem::wasCurrentCommandUndoOrRedo()
		{
		if( adminCleanup_ != NULL )
			return adminCleanup_->wasCurrentCommandUndoOrRedo();

		// Default if admin cleanup module isn't created yet
		return true;
		}

	ResultType AdminItem::cleanupDeletedItems()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "cleanupDeletedItems";

		if( adminCleanup_ != NULL ||
		// Create supporting module
		( adminCleanup_ = new AdminCleanup( this, commonVariables() ) ) != NULL )
			return adminCleanup_->cleanupDeletedItems();

		return startError( functionNameString, NULL, NULL, "I failed to create the admin cleanup module" );
		}

	ResultType AdminItem::deleteUnusedInterpretations( bool isDeletingAllActiveWordTypes )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedInterpretations";

		if( adminCleanup_ != NULL )
			return adminCleanup_->deleteUnusedInterpretations( isDeletingAllActiveWordTypes );

		return startError( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}

	ResultType AdminItem::deleteSentences( unsigned int lowestSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		if( adminCleanup_ != NULL )
			return adminCleanup_->deleteSentences( lowestSentenceNr );

		return startError( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}

	ResultType AdminItem::undoLastSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoLastSentence";

		if( adminCleanup_ != NULL )
			return adminCleanup_->undoLastSentence();

		return startError( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}

	ResultType AdminItem::redoLastUndoneSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoLastUndoneSentence";

		if( adminCleanup_ != NULL )
			return adminCleanup_->redoLastUndoneSentence();

		return startError( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}


	// Protected collection functions

	CollectionResultType AdminItem::collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationWords";

		if( adminCollection_ != NULL )
			return adminCollection_->collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, specificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem );

		collectionResult.result = startError( functionNameString, NULL, NULL, "The admin collection module isn't created yet" );
		return collectionResult;
		}

	ResultType AdminItem::collectGeneralizationWordWithPreviousOne( bool isAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationWords";

		if( adminCollection_ != NULL ||
		// Create supporting module
		( adminCollection_ = new AdminCollection( this, commonVariables() ) ) != NULL )
			return adminCollection_->collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin collection module" );
		}

	ResultType AdminItem::collectRelationWords( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectRelationWords";

		if( adminCollection_ != NULL )
			return adminCollection_->collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, currentRelationWordItem, specificationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin collection module isn't created yet" );
		}


	// Protected conclusion functions

	void AdminItem::initializeAdminConclusionVariables()
		{
		if( adminConclusion_ != NULL )
			adminConclusion_->initializeAdminConclusionVariables();
		}

	bool AdminItem::hasFoundUnprocessedNegativeConclusion()
		{
		if( adminConclusion_ != NULL )
			return adminConclusion_->hasFoundUnprocessedNegativeConclusion();

		return false;
		}

	ResultType AdminItem::drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawNegativeConclusionsFromAnsweredQuestions";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawNegativeConclusionsFromAnsweredQuestions( userSpecificationItem, generalizationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSimpleNegativeSpanishConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawSimpleNegativeSpanishConclusion( isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawOnlyOptionLeftConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, specificationCollectionNr, generalizationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawPossessiveReversibleConclusion( isArchivedAssignment, isExclusiveSpecification, isPossessive, isUniqueUserRelation, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawSpecificationGeneralizationConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawSpecificationGeneralizationConclusion( isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isMakingPartOfAssumption, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findSpecificationSubstitutionConclusion";

		if( adminConclusion_ != NULL ||
		// Create supporting module
		( adminConclusion_ = new AdminConclusion( this, commonVariables() ) ) != NULL )
			return adminConclusion_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isMakingPartOfAssumption, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin conclusion module" );
		}

	SpecificationResultType AdminItem::drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawCompoundSpecificationSubstitutionConclusion( specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		specificationResult.result = startError( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		return specificationResult;
		}

	SpecificationItem *AdminItem::spanishAmbiguousCompoundPrimarySpecificationItem()
		{
		if( adminConclusion_ != NULL )
			return adminConclusion_->spanishAmbiguousCompoundPrimarySpecificationItem();

		return NULL;
		}

	SpecificationItem *AdminItem::spanishAmbiguousCompoundAnotherPrimarySpecificationItem()
		{
		if( adminConclusion_ != NULL )
			return adminConclusion_->spanishAmbiguousCompoundAnotherPrimarySpecificationItem();

		return NULL;
		}


	// Protected context functions

	unsigned int AdminItem::highestContextNrInAllWords()
		{
		unsigned int tempContextNr;
		unsigned int highestContextNr = NO_CONTEXT_NR;
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables()->lastPredefinedWordItem ) != NULL )
			{
			// Do for all active words
			do	{
				if( ( tempContextNr = currentWordItem->highestContextNrInWord() ) > highestContextNr )
					highestContextNr = tempContextNr;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return highestContextNr;
		}

	ContextResultType AdminItem::getRelationContext( bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned short specificationWordTypeNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelationContext";

		if( adminContext_ != NULL ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, commonVariables() ) ) != NULL )
			return adminContext_->getRelationContext( isArchivedAssignment, isNegative, isPossessive, isQuestion, isUserSentence, specificationWordTypeNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, startRelationReadItem );

		contextResult.result = startError( functionNameString, NULL, NULL, "I failed to create the admin context module" );
		return contextResult;
		}

	ContextResultType AdminItem::getSpecificationRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCompoundCollectionSpanishAmbiguous, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getSpecificationRelationContext";

		if( adminContext_ != NULL ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, commonVariables() ) ) != NULL )
			return adminContext_->getSpecificationRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isCompoundCollectionSpanishAmbiguous, isNegative, isPossessive, isSelfGeneratedAssumption, specificationCollectionNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		contextResult.result = startError( functionNameString, NULL, NULL, "I failed to create the admin context module" );
		return contextResult;
		}


	// Protected database connection functions
/*
	ResultType AdminItem::storeSentenceInFutureDatabase()
		{
		if( fileList != NULL )
			return fileList->storeChangesInFutureDatabase();

		// Read list and score list aren't stored, because these are temporary lists

		if( wordList != NULL )
			return wordList->storeChangesInFutureDatabase();

		if( conditionList != NULL )
			return conditionList->storeChangesInFutureDatabase();

		if( actionList != NULL )
			return actionList->storeChangesInFutureDatabase();

		if( alternativeList != NULL )
			return alternativeList->storeChangesInFutureDatabase();

		return RESULT_OK;
		}
*/


	// Protected imperative functions

	ResultType AdminItem::executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgress, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperative";

		if( adminImperative_ != NULL ||
		// Create supporting module
		( adminImperative_ = new AdminImperative( this, commonVariables() ) ) != NULL )
			return adminImperative_->executeImperative( isInitializeVariables, executionListNr, imperativeParameter, specificationWordParameter, specificationWordTypeNr, endSolveProgress, executionString, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem, executionSelectionItem, actionSelectionItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin imperative module" );
		}


	// Protected language functions

	ResultType AdminItem::authorizeLanguageWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "authorizeLanguageWord";

		if( adminLanguage_ != NULL )
			return adminLanguage_->authorizeLanguageWord( authorizationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}

	ResultType AdminItem::createLanguage( char *languageNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createLanguage";

		if( adminLanguage_ != NULL ||
		// Create supporting module
		( adminLanguage_ = new AdminLanguage( this, commonVariables() ) ) != NULL )
			return adminLanguage_->createLanguage( languageNameString );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin language module" );
		}

	ResultType AdminItem::createLanguageSpecification( WordItem *languageNounWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createLanguageSpecification";

		if( adminLanguage_ != NULL )
			return adminLanguage_->createLanguageSpecification( languageNounWordItem );

		return startError( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}

	ResultType AdminItem::assignLanguage( unsigned short newLanguageNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignLanguage";

		if( adminLanguage_ != NULL )
			return adminLanguage_->assignLanguage( newLanguageNr );

		return startError( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}

	ResultType AdminItem::assignLanguage( char *languageNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignInterfaceLanguage";

		if( adminLanguage_ != NULL )
			return adminLanguage_->assignLanguage( languageNameString );

		return startError( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}


	// Protected query functions

	void AdminItem::initializeQueryStringPosition()
		{
		if( adminQuery_ != NULL )
			adminQuery_->initializeQueryStringPosition();
		}

	ResultType AdminItem::writeInfoTextWithPossibleQueryCommands( char *textString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoTextWithPossibleQueryCommands";

		if( adminQuery_ != NULL ||
		// Create supporting module
		( adminQuery_ = new AdminQuery( this, commonVariables() ) ) != NULL )
			return adminQuery_->writeInfoTextWithPossibleQueryCommands( textString );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin query module" );
		}

	ResultType AdminItem::executeQuery( bool isSuppressingMessage, bool isReturningToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, char *queryString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeQuery";

		if( adminQuery_ != NULL ||
		// Create supporting module
		( adminQuery_ = new AdminQuery( this, commonVariables() ) ) != NULL )
			return adminQuery_->executeQuery( isSuppressingMessage, isReturningToPosition, isWritingQueryResult, promptTypeNr, queryString );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin query module" );
		}


	// Protected read create words functions

	void AdminItem::deleteTemporaryReadList()
		{
		if( readList != NULL )
			readList->deleteTemporaryList();
		}

	bool AdminItem::hasFoundDifferentParameter()
		{
		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->hasFoundDifferentParameter();

		return false;
		}

	bool AdminItem::isImperativeSentence()
		{
		if( readList != NULL )
			return readList->isImperativeSentence();

		return false;
		}

	unsigned short AdminItem::lastCreatedWordOrderNr()
		{
		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->lastCreatedWordOrderNr();

		return NO_ORDER_NR;
		}

	ReadResultType AdminItem::createReadWords( char *grammarString )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWords";

		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->createReadWords( grammarString );

		readResult.result = startError( functionNameString, NULL, NULL, "The admin read create words module isn't created yet" );
		return readResult;
		}

	ReadResultType AdminItem::readWordFromString( bool isCheckingForGrammarDefinition, bool isMergedWord, bool isSkippingTextString, size_t startWordPosition, size_t minimumStringLength, char *wordString )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readWordFromString";

		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->readWordFromString( isCheckingForGrammarDefinition, isMergedWord, isSkippingTextString, startWordPosition, minimumStringLength, wordString );

		readResult.result = startError( functionNameString, NULL, NULL, "The admin read create words module isn't created yet" );
		return readResult;
		}

	WordResultType AdminItem::addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordLength, char *wordString )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWord";

		if( adminReadCreateWords_ != NULL ||
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this, commonVariables() ) ) != NULL )
			return adminReadCreateWords_->addWord( isLanguageWord, isMultipleWord, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, wordParameter, wordTypeNr, wordLength, wordString );

		wordResult.result = startError( functionNameString, NULL, NULL, "I failed to create the admin read create words module" );
		return wordResult;
		}

	WordResultType AdminItem::findWordTypeInAllWords( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString, WordItem *previousWordItem )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordTypeInAllWords";

		if( adminReadCreateWords_ != NULL ||
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this, commonVariables() ) ) != NULL )
			return adminReadCreateWords_->findWordTypeInAllWords( isCheckingAllLanguages, wordTypeNr, wordTypeString, previousWordItem );

		wordResult.result = startError( functionNameString, NULL, NULL, "I failed to create the admin read create words module" );
		return wordResult;
		}

	ReadItem *AdminItem::firstActiveReadItem()
		{
		if( readList != NULL )
			return readList->firstActiveReadItem();

		return NULL;
		}

	ReadItem *AdminItem::firstInactiveReadItem()
		{
		if( readList != NULL )
			return readList->firstInactiveReadItem();

		return NULL;
		}


	// Protected read file functions

	bool AdminItem::hasClosedFileDueToError()
		{
		if( adminReadFile_ != NULL )
			return adminReadFile_->hasClosedFileDueToError();

		return false;
		}

	bool AdminItem::isCurrentlyTesting()
		{
		if( fileList != NULL )
			return fileList->isCurrentlyTesting();

		return false;
		}

	ResultType AdminItem::compareOutputFileAgainstReferenceFile( char *testFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareOutputFileAgainstReferenceFile";

		if( adminReadFile_ != NULL )
			return adminReadFile_->compareOutputFileAgainstReferenceFile( testFileNameString );

		return startError( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	ResultType AdminItem::getUserInput( bool isPassword, bool isQuestion, bool isText, char *promptInputString, char *readString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getUserInput";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readLine( isPassword, isQuestion, isText, NO_SENTENCE_NR, promptInputString, readString );

		return startError( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	ResultType AdminItem::readAndExecute()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "interact";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readAndExecute();

		return startError( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	ResultType AdminItem::readExampleFile( char *exampleFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readFile";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readExampleFile( exampleFileNameString );

		return startError( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	ResultType AdminItem::readTestFile( char *testFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readTestFile";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readTestFile( testFileNameString );

		return startError( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	FileResultType AdminItem::readInfoFile( bool isReportingErrorIfFileDoesNotExist, char *infoFileNameString )
		{
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readInfoFile";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readInfoFile( isReportingErrorIfFileDoesNotExist, infoFileNameString );

		fileResult.result = startError( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		return fileResult;
		}

	FILE *AdminItem::currentWriteFile()
		{
		if( fileList != NULL )
			return fileList->currentWriteFile();

		return NULL;
		}


	// Protected read grammar functions

	ResultType AdminItem::addGrammar( char *grammarString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addGrammar";

		if( adminReadGrammar_ != NULL ||
		// Create supporting module
		( adminReadGrammar_ = new AdminReadGrammar( this, commonVariables() ) ) != NULL )
			return adminReadGrammar_->addGrammar( grammarString );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin read grammar module" );
		}

	WordItem *AdminItem::predefinedAdjectiveBusyWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedAdjectiveBusyWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedAdjectiveDoneWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedAdjectiveDoneWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedAdjectiveInvertedWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedAdjectiveInvertedWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedNounPasswordWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedNounPasswordWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedNounSolveLevelWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedNounSolveLevelWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedNounSolveMethodWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedNounSolveMethodWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedNounSolveStrategyWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedNounSolveStrategyWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedNounStartupLanguageWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedNounStartupLanguageWordItem();

		return NULL;
		}

	WordItem *AdminItem::predefinedVerbLoginWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedVerbLoginWordItem();

		return NULL;
		}


	// Protected read sentence functions

	bool AdminItem::isActiveUserAssignment()
		{
		if( adminReadSentence_ != NULL )
			return adminReadSentence_->isActiveUserAssignment();

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

	ResultType AdminItem::processReadSentence( char *readString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "processReadSentence";

		if( adminReadSentence_ != NULL ||
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this, commonVariables() ) ) != NULL )
			return adminReadSentence_->processReadSentence( readString );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin read sentence module" );
		}


	// Protected reasoning functions

	void AdminItem::initializeAdminReasoningVariables()
		{
		if( adminReasoning_ != NULL )
			adminReasoning_->initializeAdminReasoningVariables();
		}

	ResultType AdminItem::askSpecificationSubstitutionQuestion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *existingQuestionSpecificationItem, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionPart";

		if( adminReasoning_ != NULL ||
		// Create supporting module
		( adminReasoning_ = new AdminReasoning( this, commonVariables() ) ) != NULL )
			return adminReasoning_->askSpecificationSubstitutionQuestion( isArchivedAssignment, isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, existingQuestionSpecificationItem, primarySpecificationItem, secondarySpecificationItem, generalizationWordItem, specificationWordItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin reasoning module" );
		}

	SpecificationResultType AdminItem::addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNameEnding, bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkipAdditionalConclusionOrAskQuestion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSelfGeneratedSpecification";

		if( adminReasoning_ != NULL ||
		// Create supporting module
		( adminReasoning_ = new AdminReasoning( this, commonVariables() ) ) != NULL )
			return adminReasoning_->addSelfGeneratedSpecification( hasFeminineOrMasculineProperNameEnding, isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isForcingNewJustification, isNegative, isPartOf, isPossessive, isUniqueUserRelation, isSkipAdditionalConclusionOrAskQuestion, isSpecificationGeneralization, assumptionLevel, assumptionJustificationTypeNr, conclusionJustificationTypeNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		specificationResult.result = startError( functionNameString, NULL, NULL, "I failed to create the admin reasoning module" );
		return specificationResult;
		}

	SpecificationItem *AdminItem::alternativeAnotherPrimarySpecificationItem()
		{
		if( adminReasoning_ != NULL )
			return adminReasoning_->alternativeAnotherPrimarySpecificationItem();

		return NULL;
		}

	SpecificationItem *AdminItem::alternativeJustificationSpecificationItem( bool hasGeneralizationWordFeminineProperNameEnding, bool isCheckingForPossessiveSpecification, bool isCheckingForQuestion, bool isNegative, bool isPossessive, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		if( adminReasoning_ != NULL )
			return adminReasoning_->alternativeJustificationSpecificationItem( hasGeneralizationWordFeminineProperNameEnding, isCheckingForPossessiveSpecification, isCheckingForQuestion, isNegative, isPossessive, userSpecificationItem, generalizationWordItem, specificationWordItem );

		return NULL;
		}


	// Protected selection functions

	ResultType AdminItem::checkForDuplicateSelection()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateSelection";

		if( adminSelection_ != NULL )
			return adminSelection_->checkForDuplicateSelection();

		return startError( functionNameString, NULL, NULL, "The admin selection module isn't created yet" );
		}

	ResultType AdminItem::createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short prepositionParameter, unsigned short specificationWordParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionPart";

		if( adminSelection_ != NULL ||
		// Create supporting module
		( adminSelection_ = new AdminSelection( this, commonVariables() ) ) != NULL )
			return adminSelection_->createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin selection module" );
		}

	ResultType AdminItem::executeSelections()
		{
		if( adminSelection_ != NULL )
			return adminSelection_->executeSelection( MAX_PROGRESS, NULL );

		// If no selections exist, just return OK
		return RESULT_OK;
		}

	ResultType AdminItem::executeSelection( unsigned int endSolveProgress, SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeSelection";

		if( adminSelection_ != NULL )
			return adminSelection_->executeSelection( endSolveProgress, actionSelectionItem );

		return startError( functionNameString, NULL, NULL, "The admin selection module isn't created yet" );
		}


	// Protected solve functions

	void AdminItem::clearCurrentSolveProgress()
		{
		if( adminSolve_ != NULL )
			adminSolve_->clearCurrentSolveProgress();
		}

	void AdminItem::deleteTemporaryScoreList()
		{
		if( scoreList != NULL )
			scoreList->deleteTemporaryList();
		}

	ResultType AdminItem::solveWord( unsigned int endSolveProgress, WordItem *solveWordItem, SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "solveWord";

		if( adminSolve_ != NULL )
			return adminSolve_->solveWord( endSolveProgress, solveWordItem, actionSelectionItem );

		return startError( functionNameString, NULL, NULL, "The admin solve module isn't created yet" );
		}

	SelectionResultType AdminItem::checkCondition( SelectionItem *conditionSelectionItem )
		{
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkCondition";

		if( adminSolve_ != NULL ||
		// Create supporting module
		( adminSolve_ = new AdminSolve( this, commonVariables() ) ) != NULL )
			return adminSolve_->checkCondition( conditionSelectionItem );

		selectionResult.result = startError( functionNameString, NULL, NULL, "I failed to create the admin solve module" );
		return selectionResult;
		}


	// Protected specification functions

	void AdminItem::initializeLinkedWord()
		{
		if( adminSpecification_ != NULL )
			adminSpecification_->initializeLinkedWord();
		}

	void AdminItem::initializeAdminSpecificationVariables()
		{
		if( adminSpecification_ != NULL )
			adminSpecification_->initializeAdminSpecificationVariables();
		}

	ResultType AdminItem::addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationWordItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecifications";

		if( adminSpecification_ != NULL )
			return adminSpecification_->addUserSpecifications( isAction, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, generalizationContextNr, specificationContextNr, generalizationWordItem, startSpecificationReadItem, endSpecificationReadItem, startRelationReadItem, endRelationReadItem );

		return startError( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );
		}

	ResultType AdminItem::assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSelectionSpecification";

		if( adminSpecification_ != NULL )
			return adminSpecification_->assignSelectionSpecification( assignmentSelectionItem );

		return startError( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );
		}

	ResultType AdminItem::assignSpecification( WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( adminSpecification_ != NULL )
			return adminSpecification_->assignSpecification( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, generalizationWordItem, specificationWordItem, NULL ).result;

		return startError( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );
		}

	SpecificationResultType AdminItem::addSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecification";

		if( adminSpecification_ != NULL ||
		// Create supporting module
		( adminSpecification_ = new AdminSpecification( this, commonVariables() ) ) != NULL )
			return adminSpecification_->addSpecification( isAssignment, false, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueUserRelation, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		specificationResult.result = startError( functionNameString, NULL, NULL, "I failed to create the admin specification module" );
		return specificationResult;
		}

	SpecificationResultType AdminItem::addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( specificationWordItem == commonVariables()->predefinedNounLanguageWordItem )
			{
			if( adminLanguage_ != NULL )
				return adminLanguage_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startError( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != NULL )
				return adminAuthorization_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startError( functionNameString, NULL, NULL, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = commonVariables()->result;
		return specificationResult;
		}

	SpecificationResultType AdminItem::assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( specificationWordItem == commonVariables()->predefinedNounLanguageWordItem )
			{
			if( adminLanguage_ != NULL )
				return adminLanguage_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startError( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != NULL )
				return adminAuthorization_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startError( functionNameString, NULL, NULL, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = commonVariables()->result;
		return specificationResult;
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


	// Protected write justification functions

	ResultType AdminItem::writeJustificationSpecification( SpecificationItem *justificationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecification";

		if( adminWriteJustification_ != NULL ||
		// Create supporting module
		( adminWriteJustification_ = new AdminWriteJustification( this, commonVariables() ) ) != NULL )
			return adminWriteJustification_->writeJustificationSpecification( justificationSpecificationItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin write justification module" );
		}


	// Protected write specification functions

	void AdminItem::initializeAdminWriteVariables()
		{
		if( adminWriteSpecification_ != NULL )
			adminWriteSpecification_->initializeAdminWriteVariables();
		}

	ResultType AdminItem::answerQuestions()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "answerQuestions";

		if( adminWriteSpecification_ != NULL )
			return adminWriteSpecification_->answerQuestions();

		return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}

	ResultType AdminItem::checkIntegrityOfStoredUserSentence( char *readUserSentenceString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkIntegrityOfStoredUserSentence";

		if( adminWriteSpecification_ != NULL ||
		// Create supporting module
		( adminWriteSpecification_ = new AdminWriteSpecification( this, commonVariables() ) ) != NULL )
			return adminWriteSpecification_->checkIntegrityOfStoredUserSentence( readUserSentenceString );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin write specification module" );
		}

	ResultType AdminItem::markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem *userSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markWordsPassingIntegrityCheckOfStoredUserSentence";

		if( adminWriteSpecification_ != NULL ||
		// Create supporting module
		( adminWriteSpecification_ = new AdminWriteSpecification( this, commonVariables() ) ) != NULL )
			return adminWriteSpecification_->markWordsPassingIntegrityCheckOfStoredUserSentence( userSpecificationItem );

		return startError( functionNameString, NULL, NULL, "I failed to create the admin write specification module" );
		}

	ResultType AdminItem::writeJustificationReport( WordItem *justificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationReport";

		if( adminWriteSpecification_ != NULL )
			return adminWriteSpecification_->writeSelfGeneratedInfo( false, true, true, true, true, justificationWordItem );

		return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}

	ResultType AdminItem::writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		if( adminWriteSpecification_ != NULL )
			return adminWriteSpecification_->writeSelfGeneratedInfo( isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions );

		return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}

	ResultType AdminItem::writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoAboutWord";

		if( adminWriteSpecification_ != NULL )
			return adminWriteSpecification_->writeInfoAboutWord( isWritingCurrentSentenceOnly, isWritingUserSpecifications, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingUserQuestions, isWritingSelfGeneratedQuestions, isWritingSpecificationInfo, isWritingRelatedInfo, writeWordItem );

		return startError( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}


	// Thinknowlogy functions

	bool AdminItem::hasRequestedRestart()
		{
		if( adminImperative_ != NULL )
			return adminImperative_->hasRequestedRestart();

		return false;
		}

	char *AdminItem::currentLanguageNameString()
		{
		if( commonVariables() != NULL )
			return languageNameString( commonVariables()->currentLanguageNr );

		return NULL;
		}

	ResultType AdminItem::interact()
		{
		if( commonVariables() != NULL &&
		commonVariables()->result != RESULT_SYSTEM_ERROR )
			{
			readAndExecute();
			// Ignore RESULT_ERROR. Application will only exit on system error
			commonVariables()->result = RESULT_OK;
			// Application will not exit on a normal error
			return RESULT_OK;
			}

		return RESULT_SYSTEM_ERROR;
		}

/*************************************************************************
 *	"Listen to me, all you in the distant lands!
 *	Pay attention, you who are far away!
 *	The Lord called me before my birth;
 *	from within the womb he called me by name." (Psalm 49:1)
 *************************************************************************/
