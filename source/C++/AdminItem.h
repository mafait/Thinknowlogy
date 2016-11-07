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

// Admin Item header

#ifndef ADMINITEM
#define ADMINITEM 1

#include "ContextResultType.cpp"

// Some compilers need these class declarations
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
class ReadItem;
class ReadList;
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
	friend class AdminWriteJustification;
	friend class AdminWriteSpecification;

	// Private constructed variables

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
	// Protected constructed variables

	FileList *fileList;
	ReadList *readList;
	ScoreList *scoreList;
	WordList *wordList;
	SelectionList *conditionList;
	SelectionList *actionList;
	SelectionList *alternativeList;

	char *startupLanguageNameString;

	List *adminListArray[NUMBER_OF_ADMIN_LISTS];


	// Protected common functions

	void initializeAdminVariables();

	bool isSystemStartingUp();

	char adminListChar( unsigned short adminListNr );


	// Protected error functions

	ResultType addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 );
	ResultType addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ResultType addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );

	ResultType startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 );
	ResultType startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ResultType startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 );


	// Protected assumption functions

	bool hasSuggestiveQuestionAssumption();

	ResultType makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	ResultType makeGeneralizationAssumptionByGeneralization( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType makeIndirectlyAnsweredQuestionAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, SpecificationItem *userSpecificationItem );
	ResultType makeOnlyOptionLeftAssumption( bool isArchivedAssignment, bool isPossessive, SpecificationItem *createdSpecificationItem, WordItem *generalizationWordItem );
	ResultType makePossessiveConditionalSpecificationAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *secondarySpecificationItem, SpecificationItem *alternativeSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *relationWordItem );
	ResultType makeSpecificationSubstitutionPartOfAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );

	SpecificationItem *oppositePossessiveDefinitionSpecificationItem();


	// Protected authorization functions

	bool wasLoginCommand();

	unsigned int firstSentenceNrOfCurrentUser();

	ResultType login( WordItem *specificationWordItem );
	ResultType authorizeWord( WordItem *authorizationWordItem );

	char *currentUserName();


	// Protected cleanup functions

	void checkForChangesMadeByThisSentence();
	void clearTemporaryAdminLists();
	void decrementCurrentSentenceNr();
	void setCurrentItemNr();

	bool hasAnyChangeBeenMadeByThisSentence();
	bool wasUndoOrRedoCommand();

	ResultType cleanupDeletedItems();
	ResultType deleteAllWordTypesOfCurrentSentence();
	ResultType deleteUnusedWordsAndWordTypes();
	ResultType deleteSentences( unsigned int lowestSentenceNr );
	ResultType undoLastSentence();
	ResultType redoLastUndoneSentence();


	// Protected collection functions

	CollectionResultType collectSpecificationWords( bool isExclusiveSpecification, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *compoundGeneralizationWordItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem );

	ResultType collectGeneralizationWordWithPreviousOne( bool isAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType collectRelationWords( bool isExclusiveSpecification, unsigned short relationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *previousRelationWordItem, WordItem *currentRelationWordItem, WordItem *specificationWordItem );


	// Protected conclusion functions

	bool hasUnprocessedNegativeConclusion();

	ResultType drawNegativeConclusionsFromAnsweredQuestions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem );
	ResultType drawSimpleNegativeSpanishConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType drawOnlyOptionLeftConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, WordItem *generalizationWordItem );
	ResultType drawPossessiveReversibleConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, bool isPossessive, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	ResultType drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isPossessive, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	ResultType drawSpecificationSubstitutionConclusionOrAskQuestion( bool isAssumption, bool isInactiveAssignment, bool isArchivedAssignment, bool isExclusiveSpecification, bool isMakingPartOfAssumption, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );

	SpecificationResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *alternativeAnotherPrimarySpecificationItem();
	SpecificationItem *alternativeJustificationSpecificationItem( bool hasGeneralizationWordFeminineProperNameEnding, bool isCheckingForPossessiveSpecification, bool isCheckingForQuestion, bool isNegative, bool isPossessive, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	SpecificationItem *spanishAmbiguousCompoundPrimarySpecificationItem();
	SpecificationItem *spanishAmbiguousCompoundAnotherPrimarySpecificationItem();

	WordItem *adjustedQuestionWordItem();


	// Protected context functions

	unsigned int highestContextNrInAllContextWords();

	ContextResultType getRelationContext( bool isArchivedAssignment, bool isPossessive, bool isQuestion, bool isUserSentence, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem );
	ContextResultType getSpecificationRelationContext( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCompoundCollectionSpanishAmbiguous, bool isPossessive, bool isSelfGeneratedAssumption, unsigned int specificationCollectionNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );


	// Protected database connection functions

//	ResultType storeChangesInFutureDatabase();


	// Protected imperative functions

	ResultType executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeVerbParameter, unsigned short executionNounWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgress, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem );


	// Protected language functions

	ResultType authorizeLanguageWord( WordItem *authorizationWordItem );

	ResultType createLanguage( char *languageNameString );
	ResultType createLanguageSpecification( WordItem *languageNounWordItem );

	ResultType assignLanguage( unsigned short newLanguageNr );
	ResultType assignLanguage( char *languageNameString );


	// Protected query functions

	void initializeQueryStringPosition();

	ResultType writeInfoTextWithPossibleQueryCommands( char *textString );
	ResultType executeQuery( bool isSuppressingMessage, bool isReturningToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, char *queryString );


	// Protected read create words functions

	void deleteTemporaryReadList();

	bool hasFoundDifferentParameter();
	bool isImperativeSentence();

	unsigned short lastCreatedWordOrderNr();

	ReadResultType createReadWords( char *grammarString );
	ReadResultType readWordFromString( bool isCheckingForGrammarDefinition, bool isMergedWord, bool isSkippingTextString, size_t startWordPosition, size_t minimumStringLength, char *wordString );

	WordResultType addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordLength, char *wordString );
	WordResultType findWordTypeInAllWords( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString, WordItem *previousWordItem );

	ReadItem *firstActiveReadItem();
	ReadItem *firstInactiveReadItem();


	// Protected read file functions

	bool hasClosedFileDueToError();
	bool isCurrentlyTesting();

	ResultType compareOutputFileAgainstReferenceFile( char *testFileNameString );
	ResultType getUserInput( bool isPassword, bool isQuestion, char *promptInputString, char *readString );

	ResultType readAndExecute();
	ResultType readExampleFile( char *exampleFileNameString );
	ResultType readTestFile( char *testFileNameString );

	FileResultType readInfoFile( bool isReportingErrorIfFileDoesNotExist, char *infoFileNameString );

	FILE *currentWriteFile();


	// Protected read grammar functions

	ResultType addGrammar( char *grammarString );

	WordItem *predefinedAdjectiveBusyWordItem();
	WordItem *predefinedAdjectiveDoneWordItem();
	WordItem *predefinedAdjectiveInvertedWordItem();

	WordItem *predefinedNounPasswordWordItem();
	WordItem *predefinedNounSolveLevelWordItem();
	WordItem *predefinedNounSolveMethodWordItem();
	WordItem *predefinedNounSolveStrategyWordItem();
	WordItem *predefinedNounStartupLanguageWordItem();
	WordItem *predefinedVerbLoginWordItem();


	// Protected read sentence functions

	bool isActiveUserAssignment();
	bool isUserQuestion();
	bool wasPreviousCommandUndoOrRedo();

	ResultType processReadSentence( char *readString );


	// Protected selection functions

	ResultType checkForDuplicateSelection();
	ResultType createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );
	ResultType executeSelections();
	ResultType executeSelection( unsigned int endSolveProgress, SelectionItem *actionSelectionItem );


	// Protected solve functions

	void clearCurrentSolveProgress();
	void deleteTemporaryScoreList();

	ResultType solveWord( unsigned int endSolveProgress, WordItem *solveWordItem, SelectionItem *actionSelectionItem );


	// Protected specification functions

	void initializeLinkedWord();

	ResultType addUserSpecifications( bool isAction, bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationReadItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem );

	ResultType assignSelectionSpecification( SelectionItem *assignmentSelectionItem );
	ResultType assignSpecification( WordItem *generalizationWordItem, WordItem *specificationWordItem );

	SpecificationResultType addLanguageSpecification( WordItem *generalizationWordItem, WordItem *specificationWordItem );
	SpecificationResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNameEnding, bool isAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isForcingNewJustification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSkipAdditionalConclusionOrAskQuestion, bool isSpecificationGeneralization, unsigned short assumptionLevel, unsigned short assumptionJustificationTypeNr, unsigned short conclusionJustificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationResultType addSpecificationWithAuthorization( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );
	SpecificationResultType assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString );

	SpecificationItem *simpleUserSpecificationItem();
	SpecificationItem *userSpecificationItem();


	// Protected write justification functions

	ResultType writeJustificationSpecification( SpecificationItem *justificationSpecificationItem );


	// Protected write specification functions

	void initializeAdminWriteVariables();

	ResultType answerQuestions();

	ResultType checkIntegrityOfStoredUserSentence( char *readUserSentenceString );
	ResultType markWordsPassingIntegrityCheckOfStoredUserSentence( SpecificationItem *userSpecificationItem );

	ResultType writeJustificationReport( WordItem *justificationWordItem );
	ResultType writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions );
	ResultType writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, WordItem *writeWordItem );


	public:
	// Public functions

	bool hasRequestedRestart();

	char *currentLanguageNameString();

	ResultType interact();


	// Constructor

	AdminItem( char *startupLanguageNameString );
	~AdminItem();
	};
#endif

/*************************************************************************
 *	"I will praise the Lord at all times.
 *	I will constantly speak his praises." (Psalm 34:1)
 *************************************************************************/
