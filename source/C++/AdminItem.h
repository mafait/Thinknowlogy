/*	Class:			AdminItem
 *	Parent class:	WordItem
 *	Grand parent:	Item
 *	Purpose:		Processing tasks at administration level
 *	Version:		Thinknowlogy 2023 (Shaking tree)
 *************************************************************************/
/*	Copyright (C) 2023, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

#include "CompoundResultType.cpp"
#include "ContextResultType.cpp"

// Class declarations
class AdminImperative;
class AdminQuery;
class AdminReadCreateWords;
class AdminReadFile;
class AdminReadSentence;
class AdminReasoning;
class AdminSpecification;
class AdminWrite;
class FileItem;
class FileList;
class ReadItem;
class ReadList;
class ScoreItem;
class ScoreList;
class SelectionItem;
class WordList;

class AdminItem : private WordItem
	{
	friend class AdminImperative;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class AdminReasoning;
	friend class AdminSpecification;
	friend class AdminWrite;

	// Private constructed variables

	bool isSystemStartingUp_ = true;
	bool wasUndoOrRedoCommand_ = false;

	// Admin item supporting modules
	AdminImperative *adminImperative_ = NULL;
	AdminQuery *adminQuery_ = NULL;
	AdminReadCreateWords *adminReadCreateWords_ = NULL;
	AdminReadFile *adminReadFile_ = NULL;
	AdminReadSentence *adminReadSentence_ = NULL;
	AdminReasoning *adminReasoning_ = NULL;
	AdminSpecification *adminSpecification_ = NULL;
	AdminWrite *adminWrite_ = NULL;

	// Admin item lists
	FileList *fileList_ = NULL;
	ReadList *readList_ = NULL;
	ScoreList *scoreList_ = NULL;
	SelectionList *conditionList_ = NULL;
	SelectionList *actionList_ = NULL;
	SelectionList *alternativeList_ = NULL;
	WordList *wordList_ = NULL;

	// Admin item lists array
	List *adminListArray_[NUMBER_OF_ADMIN_LISTS];

	// Private initialized variables

	char *startupLanguageNameString_ = NULL;

	InputOutput *inputOutput_ = NULL;


	// Private functions

	void rebuildQuickAccessListsInAllWords();

	signed char assignLanguage( char *languageNameString );
	signed char readStartupFile();
	signed char startup();


	protected:
	// Protected common functions

	void clearTemporaryAdminLists();
	void initializeAdminVariables();

	bool isSystemStartingUp();
	bool wasUndoOrRedoCommand();

	unsigned int highestContextNrInAllContextWords();
	unsigned int highestFoundSentenceNr( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int maxSentenceNr);

	char adminListChar( unsigned short adminListNr );
	char *startupLanguageNameString();


	// Protected error functions

	signed char addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char addErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );

	signed char startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char startErrorWithAdminListNr( unsigned short adminListNr, const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 );


	// Protected database connection functions

//	signed char storeChangesInFutureDatabase();


	// Protected imperative functions

	void clearConditionChecksForSolving( unsigned short selectionLevel, unsigned int conditionSentenceNr );
	void deleteTemporaryScoreList();

	bool hasScoreList();

	unsigned int nPossibilities();

	signed char changeAction( SelectionItem *actionSelectionItem );
	signed char createScoreItem( bool isChecked, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore, SelectionItem *referenceSelectionItem );
	signed char deleteScores();
	signed char executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short executionNounWordParameter, unsigned short imperativeVerbParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgressLevel, char *executionString, WordItem *generalizationWordItem, WordItem *specificationWordItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem );
	signed char executeSelections();
	signed char redoLastUndoneSentence();
	signed char undoLastSentence();

	ScoreItem *firstPossibility();

	SelectionItem *executionStartEntry( bool isAction, unsigned short executionLevel, unsigned int executionSentenceNr );
	SelectionItem *firstConditionItem();
	SelectionItem *firstConditionSelectionItem( unsigned int conditionSentenceNr );
	SelectionItem *firstSelectionItem( bool isAction, WordItem *solveWordItem );

	BoolResultType checkScores( bool isInverted, unsigned short solveStrategyParameter, unsigned int oldSatisfiedScore, unsigned int newSatisfiedScore, unsigned int oldDissatisfiedScore, unsigned int newDissatisfiedScore, unsigned int oldNotBlockingScore, unsigned int newNotBlockingScore, unsigned int oldBlockingScore, unsigned int newBlockingScore );
	BoolResultType findScore( bool isPreparingSort, SelectionItem *findScoreItem );

	SelectionResultType getBestSelection( bool isCurrentlyTesting, unsigned short solveStrategyParameter );


	// Protected query functions

	void clearQuerySelections();
	void countQuery();

	void itemQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr );
	void listQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString );
	void parameterQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter );
	void wordTypeQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr );

	signed char displayQueryResult( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth );
	signed char stringQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryString );
	signed char wordQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *wordNameString );
	signed char wordReferenceQuery( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString );

	QueryResultType executeQuery( bool isSuppressingMessage, bool isReturningToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, size_t queryCommandStringStartPosition, char *queryString );


	// Protected read create words functions

	void deleteTemporaryReadList();

	unsigned short lastCreatedWordOrderNr();

	signed char createReadItem( bool isUncountableGeneralizationNoun, unsigned short wordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr, size_t readStringLength, char *readString, WordItem *readWordItem );
	signed char deleteReadItemsWithNonMatchingMultipleWordPart( unsigned short wordOrderNr, char *sentenceString );

	ReadItem *firstActiveReadItem();
	ReadItem *firstInactiveReadItem();
	ReadItem *nextReadListItem();

	BoolResultType createReadWords( bool isChineseCurrentLanguage, char *readUserSentenceString );

	ReadWordResultType readWordFromString( bool isCheckingForGrammarDefinition, bool isMergedWord, bool isSkippingTextString, size_t minimumStringLength, char *wordString );

	ShortResultType getNumberOfReadWordReferences( unsigned short wordTypeNr, WordItem *readWordItem );

	WordResultType addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordLength, char *wordString );
	WordResultType createWordItem( bool isLanguageWord, unsigned short wordParameter );
	WordResultType findWordTypeInAllWords( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString, WordItem *previousWordItem );


	// Protected read file functions

	void decrementCurrentSentenceNr();
	void decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset );
	void decrementSentenceNrs( unsigned int startSentenceNr );
	void deleteSentences( unsigned int lowestSentenceNr );
	void removeFirstRangeOfDeletedItems();

	bool isCurrentFileTestFile();
	bool isCurrentlyTesting();
	bool isDisplayingLine();

	unsigned int currentFileSentenceNr();
	unsigned int firstSentenceNrOfCurrentUser();
	unsigned int highestCurrentSentenceItemNr();

	signed char closeCurrentFile( FileItem *closeFileItem );
	signed char compareOutputFileAgainstReferenceFile( char *testFileNameString );
	signed char login( WordItem *specificationWordItem );
	signed char readExampleFile( char *exampleFileNameString );
	signed char readTestFile( char *testFileNameString );

	FILE *currentReadFile();
	FILE *currentWriteFile();

	CreateAndAssignResultType addSpecificationWithAuthorization( bool isAssignment, bool isCharacteristicFor, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );
	CreateAndAssignResultType assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString );

	FileResultType openFile( bool isAddingSubPath, bool isInfoFile, bool isTestFile, bool isReportingErrorIfFileDoesNotExist, const char *defaultSubpathString, const char *fileNameString, const char *writeSubpathString, const char *referenceSubpathString );
	FileResultType readInfoFile( bool isReportingErrorIfFileDoesNotExist, char *infoFileNameString );


	// Protected read sentence functions

	void clearLastActivatedWordOrderNr();

	bool hasAnyChangeBeenMadeByThisSentence();
	bool isUniqueUserRelation();
	bool isUserQuestion();
	bool wasPreviousCommandUndoOrRedo();

	signed char activateInactiveReadWords( unsigned short wordOrderNr );
	signed char deleteReadItem( ReadItem *readItem );
	signed char deleteWordItem( WordItem *wordItem );
	signed char processReadSentence( char *readString );
	signed char setGrammarParameter( bool isValid, unsigned short startWordOrderNr, unsigned short endWordOrderNr, GrammarItem *definitionGrammarItem );

	BoolResultType findMoreInterpretations();
	BoolResultType selectMatchingWordType( unsigned short currentWordOrderNr, unsigned short wordParameter, unsigned short wordTypeNr );

	DuplicateResultType checkForDuplicateCondition();
	DuplicateResultType checkForDuplicateSelectionPart( bool isAction, unsigned int duplicateConditionSentenceNr );


	// Protected reasoning functions

	signed char askSpecificationSubstitutionQuestionOrDrawNegativeConclusion( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char drawCompoundSpecificationSubstitutionConclusion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *selectedPrimarySpecificationItem, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char drawExclusiveNegativeUserConclusions( bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *previousSpecificationWordItem, WordItem *currentSpecificationWordItem );
	signed char drawNegativeUserConclusions( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *adjectiveSpecificationWordItem, WordItem *currentSpecificationWordItem );
	signed char drawOnlyOptionLeftNounConclusion( bool isInactiveAssignment, bool isArchivedAssignment, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, WordItem *generalizationWordItem );
	signed char drawProperNounPartOfConclusionsInProperNounWords( bool isArchivedAssignment, WordItem *specificationWordItem, WordItem *spanishRelationWordItem );
	signed char drawReversibleConclusions( SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem );
	signed char drawSimpleNegativeConclusions( bool isArchivedAssignment, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char drawSpecificationGeneralizationConclusion( bool isArchivedAssignment, bool isSelfGenerated, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *secondarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char drawSpecificationSubstitutionConclusionOrAskQuestion( bool isArchivedAssignment, bool isExclusiveSpecification, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );

	signed char makeExclusiveSpecificationSubstitutionAssumption( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isUncountableGeneralizationNoun, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );
	signed char makeIndirectlyAnsweredQuestionAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, SpecificationItem *adjectiveSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char makeOnlyOptionLeftAssumption( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem );
	signed char makeSpecificationSubstitutionPartOfAssumption( bool isArchivedAssignment, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char makeSuggestiveQuestionAssumption( bool isArchivedAssignment, bool isNegative, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );

	CompoundResultType drawCompoundSpecificationSubstitutionConclusion( unsigned short specificationWordTypeNr, SpecificationItem *userSpecificationItem, WordItem *specificationWordItem );


	// Protected specification functions

	void initializeLinkedWord();

	signed char addUserSpecification( bool isAction, bool isAssignment, bool isCharacteristicFor, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isNewStart, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short userAssumptionLevel, unsigned int generalizationContextNr, unsigned int specificationContextNr, ReadItem *generalizationWordItem, ReadItem *startSpecificationReadItem, ReadItem *endSpecificationReadItem, ReadItem *startRelationReadItem, ReadItem *endRelationReadItem );
	signed char assignSelectionSpecification( SelectionItem *assignmentSelectionItem );
	signed char assignSpecification( WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char collectGeneralizationWordWithPreviousOne( bool isAssignment, bool isPossessive, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem );
	signed char createSelectionPart( bool isAction, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isFirstComparisonPart, bool isNewStart, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short selectionLevel, unsigned short selectionListNr, unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString );
	signed char recalculateAssumptionLevelsInTouchedWords();

	SpecificationItem *simpleUserSpecificationItem();
	SpecificationItem *userSpecificationItem();

	ContextResultType getRelationContext( bool isArchivedAssignment, bool isPossessive, bool isQuestion, bool isUserSentence, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, ReadItem *startRelationReadItem );

	CreateAndAssignResultType addSelfGeneratedSpecification( bool hasFeminineOrMasculineProperNounEnding, bool isAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isUniqueUserRelation, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, unsigned short assumptionLevel, unsigned short justificationTypeNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem );


	// Protected write functions

	void initializeAdminWriteVariables();

	signed char answerQuestions();
	signed char checkIntegrityOfStoredUserSentence( char *readUserSentenceString );
	signed char prepareIntegrityCheckOfStoredUserSentence( bool isChineseCurrentLanguage, SpecificationItem *userSpecificationItem, char *readUserSentenceString );
	signed char writeJustificationReport( WordItem *justificationWordItem );
	signed char writeSelfGeneratedInfo( bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingSelfGeneratedQuestions );
	signed char writeInfoAboutWord( bool isWritingCurrentSentenceOnly, bool isWritingUserSpecifications, bool isWritingSelfGeneratedConclusions, bool isWritingSelfGeneratedAssumptions, bool isWritingUserQuestions, bool isWritingSelfGeneratedQuestions, bool isWritingSpecificationInfo, bool isWritingRelatedInfo, WordItem *writeWordItem );


	public:
	// Public functions

	bool hasRequestedRestart();

	char *currentLanguageNameString();

	signed char interact();


	// Constructor

	AdminItem( char *startupLanguageNameString );
	~AdminItem();
	};
#endif

/*************************************************************************
 *	"I will praise the Lord at all times.
 *	I will constantly speak his praises." (Psalm 34:1)
 *************************************************************************/
