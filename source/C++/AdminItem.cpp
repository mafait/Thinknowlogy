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

#include <time.h>
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
		( adminReadFile_ = new AdminReadFile( this, commonVariables(), this ) ) != NULL )
			return adminReadFile_->readStartupFile();

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin read module" );
		}

	ResultType AdminItem::startup()
		{
		char titleString[MAX_SENTENCE_STRING_LENGTH];
		char functionNameString[FUNCTION_NAME_LENGTH] = "startup";

		srand( (unsigned int)time( NULL ) );		// Initialize the random number generator with the time
		sprintf( titleString, "\n %s %s", PRODUCT_NAME, VERSION_NAME );

		if( commonVariables()->presentation->writeDiacriticalText( PRESENTATION_PROMPT_INFO, titleString ) == RESULT_OK )
			{
			if( readStartupFile() == RESULT_OK )
				{
				commonVariables()->presentation->clearStatus();

				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_START_MESSAGE ) == RESULT_OK )
					isSystemStartingUp_ = false;
				else
					return addErrorInItem( functionNameString, NULL, NULL, "I failed to write the start interface text" );
				}
			else
				return startSystemErrorInItem( functionNameString, NULL, NULL, "I failed to read the startup file" );
			}
		else
			return startSystemErrorInItem( functionNameString, NULL, NULL, "I failed to write the title text" );

		return RESULT_OK;
		}


	// Constructor / deconstructor

	AdminItem::AdminItem()
		{
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

		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			adminListArray[adminListNr] = NULL;

		// Initialization

		initializeItemVariables( "AdminItem", new CommonVariables(), this );

		if( ( commonVariables()->presentation = new Presentation( commonVariables() ) ) != NULL )
			{
			if( startup() != RESULT_OK )
				addErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to start the administrator" );
			}
		else
			startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the presentation module" );
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

	ResultType AdminItem::makeExclusiveSpecificationSubstitutionAssumption( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeExclusiveSpecificationSubstitutionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeExclusiveSpecificationSubstitutionAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short specificQuestionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumptionByGeneralization";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeGeneralizationAssumptionByGeneralization( isArchivedAssignment, isNegative, isPossessive, specificQuestionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeGeneralizationAssumptionBySpecification( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeGeneralizationAssumptionBySpecification";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeGeneralizationAssumptionBySpecification( isArchivedAssignment, isNegative, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, generalizationWordItem, specificationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeIndirectlyAnsweredQuestionAssumption";

		if( adminAssumption_ != NULL ||
		// Create supporting module
		( adminAssumption_ = new AdminAssumption( this, commonVariables(), this ) ) != NULL )
			return adminAssumption_->makeIndirectlyAnsweredQuestionAssumption( isAssignment, isInactiveAssignment, isArchivedAssignment, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, userSpecificationItem );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin assumption module" );
		}

	ResultType AdminItem::makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *createdSpecificationItem, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeOnlyOptionLeftAssumption( isArchivedAssignment, isPossessive, createdSpecificationItem, generalizationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makePossessiveConditionalSpecificationAssumption( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makePossessiveConditionalSpecificationAssumption( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, secondarySpecificationItem, userSpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	ResultType AdminItem::makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "makeSuggestiveQuestionAssumption";

		if( adminAssumption_ != NULL )
			return adminAssumption_->makeSuggestiveQuestionAssumption( isArchivedAssignment, isNegative, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		}

	SpecificationResultType AdminItem::addSelfGeneratedSpecification( bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isForcingNewJustification, bool isSkipAdditionalConclusionOrAskQuestion, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSelfGeneratedSpecification";

		if( adminAssumption_ != NULL )
			return adminAssumption_->addSelfGeneratedSpecification( isAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isUniqueUserRelation, isForcingNewJustification, isSkipAdditionalConclusionOrAskQuestion, assumptionLevel, assumptionJustificationTypeNr, conclusionJustificationTypeNr, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, primarySpecificationItem, anotherPrimarySpecificationItem, secondarySpecificationItem, anotherSecondarySpecificationItem, generalizationWordItem, specificationWordItem, relationWordItem );

		specificationResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin assumption module isn't created yet" );
		return specificationResult;
		}


	// Protected authorization functions

	unsigned int AdminItem::firstUserSentenceNr()
		{
		if( adminAuthorization_ != NULL )
			return adminAuthorization_->firstUserSentenceNr();

		return NO_SENTENCE_NR;
		}

	ResultType AdminItem::login( WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "login";

		if( adminAuthorization_ != NULL )
			return adminAuthorization_->login( specificationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin authorization module isn't created yet" );
		}

	ResultType AdminItem::authorizeWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "authorizeWord";

		if( adminAuthorization_ != NULL ||
		// Create supporting module
		( adminAuthorization_ = new AdminAuthorization( this, commonVariables(), this ) ) != NULL )
			return adminAuthorization_->authorizeWord( authorizationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin authorization module" );
		}

	char *AdminItem::currentUserName()
		{
		if( adminAuthorization_ != NULL )
			return adminAuthorization_->currentUserName();

		return NULL;	// During startup - before login
		}


	// Protected cleanup functions

	void AdminItem::clearDontIncrementCurrentSentenceNr()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->clearDontIncrementCurrentSentenceNr();
		}

	void AdminItem::checkForChangesDuringThisSentence()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->checkForChangesDuringThisSentence();
		}

	void AdminItem::deleteRollbackInfo()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->deleteRollbackInfo();
		}

	void AdminItem::setCurrentItemNr()
		{
		if( adminCleanup_ != NULL )
			adminCleanup_->setCurrentItemNr();
		}

	bool AdminItem::isDontIncrementCurrentSentenceNr()
		{
		if( adminCleanup_ != NULL )
			return adminCleanup_->isDontIncrementCurrentSentenceNr();

		return false;
		}

	bool AdminItem::hasFoundAnyChangeDuringThisSentence()
		{
		if( adminCleanup_ != NULL )
			return adminCleanup_->hasFoundAnyChangeDuringThisSentence();

		return true;	// Default when admin cleanup module isn't created yet
		}

	bool AdminItem::wasUndoOrRedo()
		{
		if( adminCleanup_ != NULL )
			return adminCleanup_->wasUndoOrRedo();

		return true;	// Default when admin cleanup module isn't created yet
		}

	unsigned int AdminItem::highestSentenceNr()
		{
		if( adminCleanup_ != NULL )
			return adminCleanup_->highestSentenceNr();

		return NO_SENTENCE_NR;
		}

	ResultType AdminItem::cleanupDeletedItems()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "cleanupDeletedItems";

		if( adminCleanup_ != NULL ||
		// Create supporting module
		( adminCleanup_ = new AdminCleanup( this, commonVariables(), this ) ) != NULL )
			return adminCleanup_->cleanupDeletedItems();

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin cleanup module" );
		}

	ResultType AdminItem::deleteAllTemporaryLists()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAllTemporaryLists";

		if( adminCleanup_ != NULL )
			return adminCleanup_->deleteAllTemporaryLists();

		return startErrorInItem( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}

	ResultType AdminItem::deleteUnusedInterpretations( bool isDeleteAllActiveWordTypes )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteUnusedInterpretations";

		if( adminCleanup_ != NULL )
			return adminCleanup_->deleteUnusedInterpretations( isDeleteAllActiveWordTypes );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}

	ResultType AdminItem::deleteSentences( bool isAvailableForRollback, unsigned int lowestSentenceNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteSentences";

		if( adminCleanup_ != NULL )
			return adminCleanup_->deleteSentences( isAvailableForRollback, lowestSentenceNr );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}

	ResultType AdminItem::undoLastSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "undoLastSentence";

		if( adminCleanup_ != NULL )
			return adminCleanup_->undoLastSentence();

		return startErrorInItem( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}

	ResultType AdminItem::redoLastUndoneSentence()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "redoLastUndoneSentence";

		if( adminCleanup_ != NULL )
			return adminCleanup_->redoLastUndoneSentence();

		return startErrorInItem( functionNameString, NULL, NULL, "The admin cleanup module isn't created yet" );
		}


	// Protected collection functions

	CollectionResultType AdminItem::collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem )
		{
		CollectionResultType collectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationWords";

		if( adminCollection_ != NULL )
			return adminCollection_->collectSpecificationWords( isExclusiveSpecification, isPossessive, isQuestion, isSpecificationGeneralization, generalizationWordTypeNr, specificationWordTypeNr, compoundGeneralizationWordItem, generalizationWordItem, previousSpecificationWordItem, currentSpecificationWordItem );

		collectionResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin collection module isn't created yet" );
		return collectionResult;
		}

	ResultType AdminItem::collectGeneralizationWordWithPreviousOne( bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationWords";

		if( adminCollection_ != NULL ||
		// Create supporting module
		( adminCollection_ = new AdminCollection( this, commonVariables(), this ) ) != NULL )
			return adminCollection_->collectGeneralizationWordWithPreviousOne( isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin collection module" );
		}

	ResultType AdminItem::collectSpecificationStrings( bool isExclusiveSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, char *previousSpecificationString, char *currentSpecificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectSpecificationStrings";

		if( adminCollection_ != NULL )
			return adminCollection_->collectSpecificationStrings( isExclusiveSpecification, generalizationWordTypeNr, specificationWordTypeNr, generalizationWordItem, previousSpecificationString, currentSpecificationString );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin collection module isn't created yet" );
		}

	ResultType AdminItem::collectRelationWords( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "collectRelationWords";

		if( adminCollection_ != NULL )
			return adminCollection_->collectRelationWords( isExclusiveSpecification, relationWordTypeNr, specificationWordTypeNr, previousRelationWordItem, currentRelationWordItem, specificationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin collection module isn't created yet" );
		}


	// Protected conclusion functions

	void AdminItem::initializeAdminConclusionVariables()
		{
		if( adminConclusion_ != NULL )
			adminConclusion_->initializeAdminConclusionVariables();
		}

	ResultType AdminItem::drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawOnlyOptionLeftConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawNegativeConclusionsFromAnsweredQuestions( userSpecificationItem, generalizationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawOnlyOptionLeftConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawOnlyOptionLeftConclusion( isInactiveAssignment, isArchivedAssignment, specificationCollectionNr, generalizationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawPossessiveReversibleConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawPossessiveReversibleConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawPossessiveReversibleConclusion( isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPossessive, isUniqueUserRelation, assumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawSpecificationGeneralizationConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawSpecificationGeneralizationConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawSpecificationGeneralizationConclusion( isInactiveAssignment, isArchivedAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, secondarySpecificationItem, generalizationWordItem, specificationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		}

	ResultType AdminItem::drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findSpecificationSubstitutionConclusion";

		if( adminConclusion_ != NULL ||
		// Create supporting module
		( adminConclusion_ = new AdminConclusion( this, commonVariables(), this ) ) != NULL )
			return adminConclusion_->drawSpecificationSubstitutionConclusionOrAskQuestion( isAssumption, isInactiveAssignment, isArchivedAssignment, isExclusiveSpecification, isNegative, isPartOf, isPossessive, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, generalizationContextNr, specificationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin conclusion module" );
		}

	SpecificationResultType AdminItem::drawCompoundSpecificationSubstitutionConclusion( bool isNegative, bool isPartOf, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "drawCompoundSpecificationSubstitutionConclusion";

		if( adminConclusion_ != NULL )
			return adminConclusion_->drawCompoundSpecificationSubstitutionConclusion( isNegative, isPartOf, isPossessive, specificationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, specificationWordItem );

		specificationResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin conclusion module isn't created yet" );
		return specificationResult;
		}


	// Protected context functions

	void AdminItem::initializeAdminContextVariables()
		{
		if( adminContext_ != NULL )
			adminContext_->initializeAdminContextVariables();
		}

	unsigned int AdminItem::highestContextNr()
		{
		if( adminContext_ != NULL )
			return adminContext_->highestContextNr();

		return NO_CONTEXT_NR;
		}

	ContextResultType AdminItem::addPronounContext( unsigned short contextWordTypeNr, WordItem *contextWordItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addPronounContext";

		if( adminContext_ != NULL ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, commonVariables(), this ) ) != NULL )
			return adminContext_->addPronounContext( contextWordTypeNr, contextWordItem );

		contextResult.result = startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin grammar module" );
		return contextResult;
		}

	ContextResultType AdminItem::getRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelationContext";

		if( adminContext_ != NULL ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, commonVariables(), this ) ) != NULL )
			return adminContext_->getRelationContext( isAssignment, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, questionParameter, specificationWordTypeNr, relationWordTypeNr, relationContextNr, generalizationWordItem, specificationWordItem, relationWordItem );

		contextResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin context module isn't created yet" );
		return contextResult;
		}

	ContextResultType AdminItem::getRelationContext( bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isUserSentence, unsigned short questionParameter, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem )
		{
		ContextResultType contextResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getRelationContext";

		if( adminContext_ != NULL ||
		// Create supporting module
		( adminContext_ = new AdminContext( this, commonVariables(), this ) ) != NULL )
			return adminContext_->getRelationContext( isAssignment, isExclusiveSpecification, isNegative, isPossessive, isUserSentence, questionParameter, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, startRelationReadItem );

		contextResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin context module isn't created yet" );
		return contextResult;
		}


	// Protected database connection functions
/*
	ResultType AdminItem::storeSentenceInFutureDataBase()
		{
		// Not fully implemented yet
		if( fileList != NULL )
			return fileList->storeChangesInFutureDataBase();

		// Read list and score list aren't stored, because these are temporary lists

		if( wordList != NULL )
			return wordList->storeChangesInFutureDataBase();

		if( conditionList != NULL )
			return conditionList->storeChangesInFutureDataBase();

		if( actionList != NULL )
			return actionList->storeChangesInFutureDataBase();

		if( alternativeList != NULL )
			return alternativeList->storeChangesInFutureDataBase();

		return RESULT_OK;
		}
*/


	// Protected imperative functions

	ResultType AdminItem::executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgress, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperative";

		if( adminImperative_ != NULL ||
		// Create supporting module
		( adminImperative_ = new AdminImperative( this, commonVariables(), this ) ) != NULL )
			return adminImperative_->executeImperative( isInitializeVariables, executionListNr, imperativeParameter, specificationWordParameter, specificationWordTypeNr, endSolveProgress, executionString, generalizationWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem, executionSelectionItem, actionSelectionItem );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin imperative module" );
		}


	// Protected language functions

	ResultType AdminItem::authorizeLanguageWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "authorizeLanguageWord";

		if( adminLanguage_ != NULL )
			return adminLanguage_->authorizeLanguageWord( authorizationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}

	ResultType AdminItem::createGrammarLanguage( char *languageNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createGrammarLanguage";

		if( adminLanguage_ != NULL )
			return adminLanguage_->createGrammarLanguage( languageNameString );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}

	ResultType AdminItem::createInterfaceLanguage( char *languageNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createInterfaceLanguage";

		if( adminLanguage_ != NULL ||
		// Create supporting module
		( adminLanguage_ = new AdminLanguage( this, commonVariables(), this ) ) != NULL )
			return adminLanguage_->createInterfaceLanguage( languageNameString );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin language module" );
		}

	ResultType AdminItem::createLanguageSpecification( WordItem *languageWordItem, WordItem *languageNounWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createLanguageSpecification";

		if( adminLanguage_ != NULL )
			return adminLanguage_->createLanguageSpecification( languageWordItem, languageNounWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}

	ResultType AdminItem::assignGrammarAndInterfaceLanguage( unsigned short newLanguageNr )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignGrammarAndInterfaceLanguage";

		if( adminLanguage_ != NULL )
			return adminLanguage_->assignGrammarAndInterfaceLanguage( newLanguageNr );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}

	ResultType AdminItem::assignGrammarAndInterfaceLanguage( char *languageNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignInterfaceLanguage";

		if( adminLanguage_ != NULL )
			return adminLanguage_->assignGrammarAndInterfaceLanguage( languageNameString );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
		}


	// Protected query functions

	void AdminItem::initializeQueryStringPosition()
		{
		if( adminQuery_ != NULL )
			adminQuery_->initializeQueryStringPosition();
		}

	ResultType AdminItem::writeTextWithPossibleQueryCommands( unsigned short promptTypeNr, char *textString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeTextWithPossibleQueryCommands";

		if( adminQuery_ != NULL ||
		// Create supporting module
		( adminQuery_ = new AdminQuery( this, commonVariables(), this ) ) != NULL )
			return adminQuery_->writeTextWithPossibleQueryCommands( promptTypeNr, textString );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin query module" );
		}

	ResultType AdminItem::executeQuery( bool isSuppressingMessage, bool returnToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, char *queryString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeQuery";

		if( adminQuery_ != NULL )
			return adminQuery_->executeQuery( isSuppressingMessage, returnToPosition, isWritingQueryResult, promptTypeNr, queryString );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin query module isn't created yet" );
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

	bool AdminItem::hasPassedGrammarIntegrityCheck()
		{
		if( readList != NULL )
			return readList->hasPassedGrammarIntegrityCheck();

		return false;
		}

	bool AdminItem::isImperativeSentence()
		{
		if( readList != NULL )
			return readList->isImperativeSentence();

		return false;
		}

	ReadResultType AdminItem::createReadWord( unsigned short wordOrderNr, unsigned short wordTypeNr, char *textString, WordItem *readWordItem )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWord";

		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->createReadWord( wordOrderNr, wordTypeNr, 0, textString, readWordItem );

		readResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin read create words module isn't created yet" );
		return readResult;
		}

	ReadResultType AdminItem::createReadWords( char *grammarString )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWords";

		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->createReadWords( grammarString );

		readResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin read create words module isn't created yet" );
		return readResult;
		}

	ReadResultType AdminItem::readWordFromString( bool checkForGrammarDefinition, bool skipDoubleQuotes, size_t startWordPosition, size_t minimumStringLength, char *wordString )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readWordFromString";

		if( adminReadCreateWords_ != NULL )
			return adminReadCreateWords_->readWordFromString( checkForGrammarDefinition, skipDoubleQuotes, startWordPosition, minimumStringLength, wordString );

		readResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin read create words module isn't created yet" );
		return readResult;
		}

	WordResultType AdminItem::addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordLength, char *wordString )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWord";

		if( adminReadCreateWords_ != NULL ||
		// Create supporting module
		( adminReadCreateWords_ = new AdminReadCreateWords( this, commonVariables(), this ) ) != NULL )
			return adminReadCreateWords_->addWord( isLanguageWord, isMultipleWord, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, wordParameter, wordTypeNr, wordLength, wordString );

		wordResult.result = startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin read create words module" );
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

	ResultType AdminItem::readExampleFile( char *exampleFileNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "readFile";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readExampleFile( exampleFileNameString );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	ResultType AdminItem::readAndExecute()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "interact";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readAndExecute();

		return startErrorInItem( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	ResultType AdminItem::getUserInput( bool isPassword, bool isQuestion, bool isText, char *promptInputString, char *readString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getUserInput";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readLine( isPassword, isQuestion, isText, NO_SENTENCE_NR, promptInputString, readString );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		}

	FileResultType AdminItem::readInfoFile( bool isReportErrorIfFileDoesNotExist, char *infoFileNameString )
		{
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readInfoFile";

		if( adminReadFile_ != NULL )
			return adminReadFile_->readInfoFile( isReportErrorIfFileDoesNotExist, infoFileNameString );

		fileResult.result = startErrorInItem( functionNameString, NULL, NULL, "The admin read module isn't created yet" );
		return fileResult;
		}


	// Protected read grammar functions

	ResultType AdminItem::addGrammar( char *grammarString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addGrammar";

		if( adminReadGrammar_ != NULL ||
		// Create supporting module
		( adminReadGrammar_ = new AdminReadGrammar( this, commonVariables(), this ) ) != NULL )
			return adminReadGrammar_->addGrammar( grammarString );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin read grammar module" );
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

	WordItem *AdminItem::predefinedNounInterfaceLanguageWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedNounInterfaceLanguageWordItem();

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

	WordItem *AdminItem::predefinedVerbLoginWordItem()
		{
		if( adminReadGrammar_ != NULL )
			return adminReadGrammar_->predefinedVerbLoginWordItem();

		return NULL;
		}


	// Protected read sentence functions

	void AdminItem::dontDeletedRollbackInfo()
		{
		if( adminReadSentence_ != NULL )
			adminReadSentence_->dontDeletedRollbackInfo();
		}

	bool AdminItem::isExclusiveUserSpecification()
		{
		if( adminReadSentence_ != NULL )
			return adminReadSentence_->isExclusiveSpecification();

		return false;
		}

	bool AdminItem::isUserSentencePossessive()
		{
		if( adminReadSentence_ != NULL )
			return adminReadSentence_->isUserSentencePossessive();

		return false;
		}

	ResultType AdminItem::processReadSentence( char *readString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "processReadSentence";

		if( adminReadSentence_ != NULL ||
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this, commonVariables(), this ) ) != NULL )
			return adminReadSentence_->processReadSentence( readString );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin read sentence module" );
		}

	WordResultType AdminItem::addMultipleWord( unsigned short wordParameter, unsigned short wordTypeNr, WordItem *firstWordItem, WordItem *secondWordItem, WordItem *existingMultipleWordItem )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "processReadSentence";

		if( adminReadSentence_ != NULL ||
		// Create supporting module
		( adminReadSentence_ = new AdminReadSentence( this, commonVariables(), this ) ) != NULL )
			return adminReadSentence_->addMultipleWord( wordParameter, wordTypeNr, wordTypeNr, firstWordItem, secondWordItem, existingMultipleWordItem );

		wordResult.result = startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin read sentence module" );
		return wordResult;
		}


	// Protected selection functions

	ResultType AdminItem::checkForDuplicateSelection()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkForDuplicateSelection";

		if( adminSelection_ != NULL )
			return adminSelection_->checkForDuplicateSelection();

		return startErrorInItem( functionNameString, NULL, NULL, "The admin selection module isn't created yet" );
		}

	ResultType AdminItem::createSelectionTextPart( bool isAction, bool isNewStart, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionTextPart";

		if( adminSelection_ != NULL )
			return adminSelection_->createSelectionPart( isAction, false, false, false, false, isNewStart, false, false, false, false, false, NO_ASSUMPTION_LEVEL, selectionLevel, selectionListNr, imperativeParameter, NO_PREPOSITION_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_TEXT, WORD_TYPE_TEXT, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, NULL, NULL, specificationString );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin selection module isn't created yet" );
		}

	ResultType AdminItem::createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short prepositionParameter, unsigned short specificationWordParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createSelectionPart";

		if( adminSelection_ != NULL ||
		// Create supporting module
		( adminSelection_ = new AdminSelection( this, commonVariables(), this ) ) != NULL )
			return adminSelection_->createSelectionPart( isAction, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isFirstComparisonPart, isNewStart, isNegative, isPossessive, isSpecificationGeneralization, isUniqueRelation, isValueSpecification, assumptionLevel, selectionLevel, selectionListNr, imperativeParameter, prepositionParameter, specificationWordParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin selection module" );
		}

	ResultType AdminItem::executeSelections()
		{
		if( adminSelection_ != NULL )
			return adminSelection_->executeSelection( MAX_PROGRESS, NULL );

		return RESULT_OK;
		}

	ResultType AdminItem::executeSelection( unsigned int endSolveProgress, SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeSelection";

		if( adminSelection_ != NULL )
			return adminSelection_->executeSelection( endSolveProgress, actionSelectionItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin selection module isn't created yet" );
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

	ResultType AdminItem::canWordBeSolved( WordItem *solveWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "canWordBeSolved";

		if( adminSolve_ != NULL )
			return adminSolve_->canWordBeSolved( solveWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin solve module isn't created yet" );
		}

	ResultType AdminItem::solveWord( unsigned int endSolveProgress, WordItem *solveWordItem, SelectionItem *actionSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "solveWord";

		if( adminSolve_ != NULL )
			return adminSolve_->solveWord( endSolveProgress, solveWordItem, actionSelectionItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin solve module isn't created yet" );
		}

	ResultType AdminItem::findPossibilityToSolveWord( bool isAddScores, bool isInverted, bool duplicatesAllowed, bool prepareSort, unsigned short solveStrategyParameter, WordItem *solveWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "findPossibilityToSolveWord";

		if( adminSolve_ != NULL )
			return adminSolve_->findPossibilityToSolveWord( isAddScores, isInverted, duplicatesAllowed, prepareSort, solveStrategyParameter, solveWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin solve module isn't created yet" );
		}

	SelectionResultType AdminItem::checkCondition( SelectionItem *conditionSelectionItem )
		{
		SelectionResultType selectionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkCondition";

		if( adminSolve_ != NULL ||
		// Create supporting module
		( adminSolve_ = new AdminSolve( this, commonVariables(), this ) ) != NULL )
			return adminSolve_->checkCondition( conditionSelectionItem );

		selectionResult.result = startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin solve module" );
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

	unsigned int AdminItem::mostRecentRelationContextNr()
		{
		if( adminSpecification_ != NULL )
			return adminSpecification_->mostRecentRelationContextNr();

		return NO_CONTEXT_NR;
		}

	ResultType AdminItem::addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isUserSpecificationExclusivelyFeminineOrMasculine, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationWordItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addUserSpecifications";

		if( adminSpecification_ != NULL )
			return adminSpecification_->addUserSpecifications( isAction, isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isNewStart, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, assumptionLevel, prepositionParameter, questionParameter, selectionLevel, selectionListNr, imperativeParameter, specificationWordParameter, generalizationContextNr, specificationContextNr, generalizationWordItem, startSpecificationReadItem, endSpecificationReadItem, startRelationReadItem, endRelationReadItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );
		}

	ResultType AdminItem::assignSelectionSpecification( SelectionItem *assignmentSelectionItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSelectionSpecification";

		if( adminSpecification_ != NULL )
			return adminSpecification_->assignSelectionSpecification( assignmentSelectionItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );
		}

	ResultType AdminItem::assignSpecification( WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignSpecification";

		if( adminSpecification_ != NULL )
			return adminSpecification_->assignSpecification( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_UNDEFINED, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, generalizationWordItem, specificationWordItem, NULL ).result;

		return startErrorInItem( functionNameString, NULL, NULL, "The admin specification module isn't created yet" );
		}

	SpecificationResultType AdminItem::addSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecification";

		if( adminSpecification_ != NULL ||
		// Create supporting module
		( adminSpecification_ = new AdminSpecification( this, commonVariables(), this ) ) != NULL )
			return adminSpecification_->addSpecification( isAssignment, false, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, false, isSpecificationGeneralization, isUniqueRelation, false, false, prepositionParameter, questionParameter, userAssumptionLevel, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, 0, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

		specificationResult.result = startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin specification module" );
		return specificationResult;
		}

	SpecificationResultType AdminItem::addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool isUserSpecificationExclusivelyFeminineOrMasculine, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( specificationWordItem == commonVariables()->predefinedNounGrammarLanguageWordItem ||
		specificationWordItem == predefinedNounInterfaceLanguageWordItem() )
			{
			if( adminLanguage_ != NULL )
				return adminLanguage_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startErrorInItem( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != NULL )
				return adminAuthorization_->addSpecificationWithAuthorization( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueRelation, isUserSpecificationExclusivelyFeminineOrMasculine, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, relationWordItem, specificationString );

			startErrorInItem( functionNameString, NULL, NULL, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = commonVariables()->result;
		return specificationResult;
		}

	SpecificationResultType AdminItem::assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addSpecificationWithAuthorization";

		if( specificationWordItem == commonVariables()->predefinedNounGrammarLanguageWordItem ||
		specificationWordItem == predefinedNounInterfaceLanguageWordItem() )
			{
			if( adminLanguage_ != NULL )
				return adminLanguage_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startErrorInItem( functionNameString, NULL, NULL, "The admin language module isn't created yet" );
			}
		else
			{
			if( adminAuthorization_ != NULL )
				return adminAuthorization_->assignSpecificationWithAuthorization( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, generalizationWordItem, specificationWordItem, specificationString );

			startErrorInItem( functionNameString, NULL, NULL, "The admin authorization module isn't created yet" );
			}

		specificationResult.result = commonVariables()->result;
		return specificationResult;
		}

	SpecificationItem *AdminItem::userSpecificationItem()
		{
		if( adminSpecification_ != NULL )
			return adminSpecification_->userSpecificationItem();

		return NULL;
		}


	// Protected write justification functions

	ResultType AdminItem::writeJustificationSpecification( char *justificationSentenceString, SpecificationItem *justificationSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationSpecification";

		if( adminWriteJustification_ != NULL ||
		// Create supporting module
		( adminWriteJustification_ = new AdminWriteJustification( commonVariables(), this ) ) != NULL )
			return adminWriteJustification_->writeJustificationSpecification( justificationSentenceString, justificationSpecificationItem );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin write justification module" );
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

		return startErrorInItem( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}

	ResultType AdminItem::checkForStoreAndRetrieveIntegrity( bool isQuestion, char *storeAndRetrieveIntegrityString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeWordsFailedForGrammarIntegrityCheck";

		if( adminWriteSpecification_ != NULL ||
		// Create supporting module
		( adminWriteSpecification_ = new AdminWriteSpecification( this, commonVariables(), this ) ) != NULL )
			return adminWriteSpecification_->checkForStoreAndRetrieveIntegrity( isQuestion, storeAndRetrieveIntegrityString );

		return startErrorInItem( functionNameString, NULL, NULL, "I failed to create the admin write specification module" );
		}

	ResultType AdminItem::writeJustificationReport( WordItem *justificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeJustificationReport";

		if( adminWriteSpecification_ != NULL )
			return adminWriteSpecification_->writeSelfGeneratedInfo( false, true, true, true, true, justificationWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}

	ResultType AdminItem::writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeSelfGeneratedInfo";

		if( adminWriteSpecification_ != NULL )
			return adminWriteSpecification_->writeSelfGeneratedInfo( isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingSelfGeneratedQuestions );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}

	ResultType AdminItem::writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, char *storeAndRetrieveIntegrityString, WordItem *writeWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoAboutWord";

		if( adminWriteSpecification_ != NULL )
			return adminWriteSpecification_->writeInfoAboutWord( isWritingCurrentSentenceOnly, isWritingUserSpecifications, isWritingSelfGeneratedConclusions, isWritingSelfGeneratedAssumptions, isWritingUserQuestions, isWritingSelfGeneratedQuestions, isWritingSpecificationInfo, isWritingRelatedInfo, storeAndRetrieveIntegrityString, writeWordItem );

		return startErrorInItem( functionNameString, NULL, NULL, "The admin write module isn't created yet" );
		}


	// Thinknowlogy functions

	bool AdminItem::hasRequestedRestart()
		{
		if( adminImperative_ != NULL )
			return adminImperative_->hasRequestedRestart();

		return false;
		}

	ResultType AdminItem::interact()
		{
		if( commonVariables() != NULL &&
		commonVariables()->result != RESULT_SYSTEM_ERROR )
			{
			readAndExecute();
			commonVariables()->result = RESULT_OK;	// Ignore RESULT_ERROR. Application will only exit on system error
			return RESULT_OK;						// Application will not exit on a normal error
			}

		return RESULT_SYSTEM_ERROR;
		}

/*************************************************************************
 *
 *	"Listen to me, all you in the distant lands!
 *	Pay attention, you who are far away!
 *	The Lord called me before my birth;
 *	from within the womb he called me by name." (Psalm 49:1)
 *
 *************************************************************************/
