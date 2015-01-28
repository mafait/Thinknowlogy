/*
 *	Class:			AdminItem
 *	Parent class:	WordItem
 *	Grand parent:	Item
 *	Purpose:		To process tasks at administration level
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

// Admin Item header

#ifndef ADMINITEM
#define ADMINITEM 1

#include "WordItem.h"
#include "ContextResultType.cpp"
#include "FileResultType.cpp"
#include "ReadResultType.cpp"
#include "SelectionResultType.cpp"

// Class declarations needed by some compilers
class AdminAssumption;
class AdminAuthorization;
class AdminCleanup;
class AdminCollection;
class AdminConclusion;
class AdminContext;
class AdminImperative;
class AdminLanguage;
class AdminQuery;
class AdminReadCreateWords;
class AdminReadFile;
class AdminReadGrammar;
class AdminReadSentence;
class AdminSelection;
class AdminSolve;
class AdminSpecification;
class AdminWriteJustification;
class AdminWriteSpecification;
class FileList;
class List;
class ReadList;
class ScoreList;
class SelectionList;
class WordItem;
class WordList;

class AdminItem : private WordItem
	{
	friend class AdminAssumption;
	friend class AdminAuthorization;
	friend class AdminCleanup;
	friend class AdminCollection;
	friend class AdminConclusion;
	friend class AdminContext;
	friend class AdminImperative;
	friend class AdminLanguage;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class AdminSelection;
	friend class AdminSolve;
	friend class AdminSpecification;
	friend class AdminWriteSpecification;

	// Private constructible variables

	bool isSystemStartingUp_;

	AdminAssumption *adminAssumption_;
	AdminAuthorization *adminAuthorization_;
	AdminCleanup *adminCleanup_;
	AdminCollection *adminCollection_;
	AdminConclusion *adminConclusion_;
	AdminContext *adminContext_;
	AdminImperative *adminImperative_;
	AdminLanguage *adminLanguage_;
	AdminQuery *adminQuery_;
	AdminReadCreateWords *adminReadCreateWords_;
	AdminReadFile *adminReadFile_;
	AdminReadGrammar *adminReadGrammar_;
	AdminReadSentence *adminReadSentence_;
	AdminSelection *adminSelection_;
	AdminSolve *adminSolve_;
	AdminSpecification *adminSpecification_;
	AdminWriteJustification *adminWriteJustification_;
	AdminWriteSpecification *adminWriteSpecification_;


	// Private functions

	ResultType readStartupFile();
	ResultType startup();


	protected:
	// Protected constructible variables

	FileList *fileList;
	ReadList *readList;
	ScoreList *scoreList;
	WordList *wordList;
	SelectionList *conditionList;
	SelectionList *actionList;
	SelectionList *alternativeList;

	List *adminListArray[NUMBER_OF_ADMIN_LISTS];


	// Protected common functions

	bool isSystemStartingUp();

	char adminListChar( unsigned short adminListNr );


	// Protected assumption functions

	void initializeAdminAssumptionVariables();

	bool hasFoundSuggestiveQuestionAssumption();

	ResultType makeExclusiveSpecificationSubstitutionAssumption( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	ResultType makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short specificQuestionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType makeGeneralizationAssumptionBySpecification( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem );
	ResultType makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *createdSpecificationItem, WordItem *generalizationWordItem );
	ResultType makePossessiveConditionalSpecificationAssumption( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	ResultType makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );

	SpecificationResultType addSelfGeneratedSpecification( bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkipAdditionalConclusionOrAskQuestion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );


	// Protected authorization functions

	unsigned int firstUserSentenceNr();

	ResultType login( WordItem *specificationWordItem );
	ResultType authorizeWord( WordItem *authorizationWordItem );

	char *currentUserName();


	// Protected cleanup functions

	void clearDontIncrementCurrentSentenceNr();
	void checkForChangesDuringThisSentence();

	void deleteRollbackInfo();

	void setCurrentItemNr();

	bool isDontIncrementCurrentSentenceNr();
	bool wasUndoOrRedo();

	unsigned int highestSentenceNr();

	ResultType cleanupDeletedItems();

	ResultType deleteAllTemporaryLists();
	ResultType deleteUnusedInterpretations( bool isDeleteAllActiveWordTypes );
	ResultType deleteSentences( bool isAvailableForRollback, unsigned int lowestSentenceNr );

	ResultType undoLastSentence();
	ResultType redoLastUndoneSentence();


	// Protected collection functions

	CollectionResultType collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem );

	ResultType collectGeneralizationWordWithPreviousOne( bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType collectSpecificationStrings( bool isExclusiveSpecification, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, char *previousSpecificationString, char *currentSpecificationString );
	ResultType collectRelationWords( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *specificationWordItem );


	// Protected conclusion functions

	void initializeAdminConclusionVariables();

	ResultType drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem );
	ResultType drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem );
	ResultType drawPossessiveReversibleConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	ResultType drawSpecificationGeneralizationConclusion( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );

	SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( bool isNegative, bool isPartOf, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *createdFeminineOrMasculineSpecificationItem();
	SpecificationItem *foundFeminineOrMasculineSpecificationItem();


	// Protected context functions

	void initializeAdminContextVariables();

	unsigned int highestContextNr();

	ContextResultType addPronounContext( unsigned short contextWordTypeNr, WordItem *contextWordItem );
	ContextResultType getRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	ContextResultType getRelationContext( bool isAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem );


	// Protected database connection functions

//	ResultType storeChangesInFutureDatabase();


	// Protected imperative functions

	ResultType executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgress, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem );


	// Protected language functions

	ResultType authorizeLanguageWord( WordItem *authorizationWordItem );

	ResultType createGrammarLanguage( char *languageNameString );
	ResultType createInterfaceLanguage( char *languageNameString );

	ResultType createLanguageSpecification( WordItem *languageWordItem, WordItem *languageNounWordItem );

	ResultType assignGrammarAndInterfaceLanguage( unsigned short newLanguageNr );
	ResultType assignGrammarAndInterfaceLanguage( char *languageNameString );


	// Protected query functions

	void initializeQueryStringPosition();

	ResultType writeTextWithPossibleQueryCommands( unsigned short promptTypeNr, char *textString );
	ResultType executeQuery( bool isSuppressingMessage, bool returnToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, char *queryString );


	// Protected read create words functions

	void deleteTemporaryReadList();

	bool hasFoundDifferentParameter();
	bool hasPassedGrammarIntegrityCheck();
	bool isImperativeSentence();

	ReadResultType createReadWords( char *grammarString );
	ReadResultType readWordFromString( bool checkForGrammarDefinition, bool skipDoubleQuotes, size_t startWordPosition, size_t minimumStringLength, char *wordString );

	WordResultType addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordLength, char *wordString );

	ReadItem *firstActiveReadItem();
	ReadItem *firstInactiveReadItem();


	// Protected read file functions

	ResultType readExampleFile( char *exampleFileNameString );

	ResultType readAndExecute();
	ResultType getUserInput( bool isPassword, bool isQuestion, bool isText, char *promptInputString, char *readString );

	FileResultType readInfoFile( bool isReportErrorIfFileDoesNotExist, char *infoFileNameString );


	// Protected read grammar functions

	bool hasFoundAnyChangeDuringThisSentence();

	ResultType addGrammar( char *grammarString );

	WordItem *predefinedAdjectiveBusyWordItem();
	WordItem *predefinedAdjectiveDoneWordItem();
	WordItem *predefinedAdjectiveInvertedWordItem();

	WordItem *predefinedNounInterfaceLanguageWordItem();
	WordItem *predefinedNounPasswordWordItem();
	WordItem *predefinedNounSolveLevelWordItem();
	WordItem *predefinedNounSolveMethodWordItem();
	WordItem *predefinedNounSolveStrategyWordItem();
	WordItem *predefinedVerbLoginWordItem();


	// Protected read sentence functions

	void dontDeletedRollbackInfo();

	bool isExclusiveUserSpecification();
	bool isPossessiveUserSpecification();
	bool isUserSentenceMixOfFeminineAndMasculineSpecificationWords();

	ResultType processReadSentence( char *readString );

	WordResultType addMultipleWord( unsigned short wordParameter, unsigned short wordTypeNr, WordItem *firstWordItem, WordItem *secondWordItem, WordItem *existingMultipleWordItem );


	// Protected selection functions

	ResultType checkForDuplicateSelection();

	ResultType createSelectionTextPart( bool isAction, bool isNewStart, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, char *specificationString );
	ResultType createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short prepositionParameter, unsigned short specificationWordParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );

	ResultType executeSelections();
	ResultType executeSelection( unsigned int endSolveProgress, SelectionItem *actionSelectionItem );


	// Protected solve functions

	void clearCurrentSolveProgress();
	void deleteTemporaryScoreList();

	ResultType canWordBeSolved( WordItem *solveWordItem );
	ResultType findPossibilityToSolveWord( bool isAddScores, bool isInverted, bool duplicatesAllowed, bool prepareSort, unsigned short solveStrategyParameter, WordItem *solveWordItem );
	ResultType solveWord( unsigned int endSolveProgress, WordItem *solveWordItem, SelectionItem *actionSelectionItem );

	SelectionResultType checkCondition( SelectionItem *conditionSelectionItem );


	// Protected specification functions

	void initializeLinkedWord();
	void initializeAdminSpecificationVariables();

	ResultType addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, bool isUserSpecificationWordExclusivelyFeminineOrMasculine, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeParameter, unsigned short specificationWordParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationReadItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem );

	ResultType assignSelectionSpecification( SelectionItem *assignmentSelectionItem );
	ResultType assignSpecification( WordItem *generalizationWordItem, WordItem *specificationWordItem );

	SpecificationResultType addSpecification( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short userAssumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );
	SpecificationResultType addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueRelation, bool isUserSpecificationWordExclusivelyFeminineOrMasculine, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );
	SpecificationResultType assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString );

	SpecificationItem *userSpecificationItem();


	// Protected justification functions

	ResultType writeJustificationSpecification( char *justificationSentenceString, SpecificationItem *justificationSpecificationItem );


	// Protected write specification functions

	void initializeAdminWriteVariables();

	ResultType answerQuestions();
	ResultType checkForStoreAndRetrieveIntegrity( bool isQuestion, char *storeAndRetrieveIntegrityString );

	ResultType writeJustificationReport( WordItem *justificationWordItem );
	ResultType writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions );
	ResultType writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, char *storeAndRetrieveIntegrityString, WordItem *writeWordItem );


	// Public functions
	public:

	bool hasRequestedRestart();

	ResultType interact();


	// Constructor / deconstructor

	AdminItem();
	~AdminItem();
	};

#endif

/*************************************************************************
 *	"I will praise the Lord at all times.
 *	I will constantly speak his praises." (Psalm 34:1)
 *************************************************************************/
