/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		Storing and processing word information
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

// WordItem header

#ifndef WORDITEM
#define WORDITEM 1

#include "DuplicateResultType.cpp"
#include "GrammarResultType.cpp"
#include "JustificationResultType.cpp"
#include "RelatedResultType.cpp"
#include "WordResultType.cpp"

// Class declarations
class CollectionList;
class ContextItem;
class ContextList;
class GeneralizationItem;
class GeneralizationList;
class GrammarList;
class InterfaceList;
class JustificationList;
class MultipleWordList;
class SpecificationList;
class WordQuestion;
class WordSpecification;
class WordTypeList;
class WordWrite;
class WriteItem;
class WriteList;

class WordItem : protected Item
	{
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class AdminReasoning;
	friend class AdminSpecification;
	friend class AdminWrite;
	friend class CollectionItem;
	friend class CollectionList;
	friend class ContextItem;
	friend class ContextList;
	friend class GeneralizationItem;
	friend class GrammarItem;
	friend class GrammarList;
	friend class InputOutput;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class List;
	friend class MultipleWordItem;
	friend class MultipleWordList;
	friend class ReadItem;
	friend class ReadList;
	friend class SelectionItem;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordList;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordTypeItem;
	friend class WordTypeList;
	friend class WordWrite;

	// Private constructed variables

	bool isChineseLanguage_ = false;
	bool isSpanishLanguage_ = false;
	bool isNounWordSpanishAmbiguous_ = false;

	unsigned int highestSentenceNrInWord_ = NO_SENTENCE_NR;

	char *authorizationKey_ = NULL;

	// Word item supporting modules
	WordQuestion *wordQuestion_ = NULL;
	WordSpecification *wordSpecification_ = NULL;
	WordWrite *wordWrite_ = NULL;

	// Word item lists
	SpecificationList *assignmentList_ = NULL;
	CollectionList *collectionList_ = NULL;
	ContextList *contextList_ = NULL;
	GeneralizationList *generalizationList_ = NULL;
	GrammarList *grammarList_ = NULL;
	InterfaceList *interfaceList_ = NULL;
	JustificationList *justificationList_ = NULL;
	MultipleWordList *multipleWordList_ = NULL;
	SpecificationList *specificationList_ = NULL;
	WordTypeList *wordTypeList_ = NULL;
	WriteList *writeList_ = NULL;

	// Word item lists array
	List *wordListArray_[NUMBER_OF_WORD_LISTS];

	// This string is returned by a function. So, it must be "static".
	char wordItemNameString_[SENTENCE_STRING_LENGTH] = EMPTY_STRING;


	// Private initialized variables

	bool hasFeminineArticle_ = false;
	bool hasMasculineArticle_ = false;

	bool isFeminineWord_ = false;
	bool isMasculineWord_ = false;

	bool isLanguageWord_ = false;

	unsigned short wordParameter_ = NO_WORD_PARAMETER;


	// Private common functions

	unsigned short nUsers();

	char wordListChar( unsigned short wordListNr );

	char *selectedLanguageNameString( unsigned short languageNr );
	char *selectedUserNameString( unsigned short userNr );


	// Private error functions

	signed char addErrorWithWordListNr( unsigned short wordListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );

	BoolResultType startBoolResultErrorInWord( const char *functionNameString, const char *errorString );

	CollectionResultType addCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CollectionResultType startCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	GrammarResultType startGrammarResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	JustificationResultType startJustificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	WordEndingResultType startWordEndingResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	WordTypeResultType startWordTypeResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );


	// Private assignment functions

	signed char inactivateAssignment( bool isAmbiguousRelationContext, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	signed char inactivateCurrentAssignments();

	SpecificationItem *firstActiveNonQuestionAssignmentItem( unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString );


	// Private collection functions

	unsigned short highestCollectionOrderNrInWord( unsigned int collectionNr );

	unsigned int highestCollectionNrInWord();


	// Private context functions

	bool isContextSubsetInWord( unsigned int fullSetContextNr, unsigned int subsetContextNr );

	WordItem *_nextContextWordItem( bool isIncludingThisItem, unsigned int contextNr );


	// Private grammar functions

	void markGrammarAsChoiceEnd();
	void markGrammarAsOptionEnd();

	signed char checkForDuplicateGrammarDefinition();
	signed char checkGrammar();
	signed char linkLaterDefinedGrammarWords();
	signed char shrinkMergedWordsInWriteSentence();

	char *grammarString( unsigned short wordTypeNr );

	GrammarItem *firstReadingGrammarItem();
	GrammarItem *firstWritingGrammarItem( bool isQuestion );

	BoolResultType expandMergedWordsInReadSentence( char *readUserSentenceString );

	GrammarResultType createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem );
	GrammarResultType findGrammar( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString );

	WordEndingResultType analyzeWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString );


	// Private interface functions

	signed char checkInterface( unsigned short interfaceParameter, char *interfaceString );
	signed char createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString );

	const char *interfaceString_( unsigned short interfaceParameter );


	// Private specification functions

	bool isAuthorizedForChanges( char *authorizationKey );

	signed char checkSpecificationForUsageInWord( SpecificationItem *unusedSpecificationItem );

	SpecificationItem *firstNonNegativeUserSpecificationItem( unsigned short specificationWordTypeNr );


	// Private word type functions

	bool isSingularNounWord();


	protected:
	// Protected constructed variables

	bool hasDisplayedSpanishSpecificationsThatAreNotHiddenAnymore = false;

	bool isUserGeneralizationWord = false;
	bool isUserSpecificationWord = false;
	bool isUserRelationWord = false;

	bool isWordCheckedForSolving = false;

	unsigned short predefinedMultipleWordNr = 0;

	WordItem *nextAssignmentWordItem = NULL;
	WordItem *nextCollectionWordItem = NULL;
	WordItem *nextContextWordItem = NULL;
	WordItem *nextPossessiveNounWordItem = NULL;
	WordItem *nextSpecificationWordItem = NULL;
	WordItem *nextTouchedWordItem = NULL;
	WordItem *nextUserDefinedProperNounWordItem = NULL;


	// Protected error functions

	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	signed char startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char startSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	CreateAndAssignResultType addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CreateAndAssignResultType addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CreateAndAssignResultType addCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CreateAndAssignResultType startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CreateAndAssignResultType startCreateAndAssignResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CreateAndAssignResultType startCreateAndAssignResultSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	RelatedResultType addRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	RelatedResultType startRelatedResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	UserSpecificationResultType addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	UserSpecificationResultType addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	UserSpecificationResultType startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	UserSpecificationResultType startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );


	// Constructor

	WordItem();
	WordItem( bool isLanguageWord, unsigned short wordParameter, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList );
	~WordItem();


	// Protected virtual functions

	virtual void checkForUsage();

	virtual bool hasParameter( unsigned int queryParameter );
	virtual bool isSorted( Item *nextSortItem );

	virtual char *itemToString( unsigned short queryWordTypeNr );

	virtual BoolResultType findMatchingWordReferenceString( char *searchString );


	// Protected common functions

	void setCurrentLanguageAsChinese();
	void setHighestSentenceNr( unsigned int currentSentenceNr );

	bool isAdjectiveAssigned();
	bool isAdjectiveAssignedOrEmpty();
	bool isAdjectiveComparison();
	bool isAdjectiveComparisonEqual();
	bool isAdjectiveComparisonLess();
	bool isAdjectiveComparisonMore();
	bool isAdjectiveEven();
	bool isAdjectiveOdd();
	bool isAdjectiveOddOrEven();
	bool isAdjectiveFeminineOrMasculine();

	bool isAdminWord();
	bool isAuthorizationRequiredForChanges();
	bool isChineseCurrentLanguage();
	bool isSpanishCurrentLanguage();
	bool isFeminineWord();
	bool isMasculineWord();
	bool isFeminineOrMasculineWord();

	bool isChineseReversedImperativeNoun();
	bool isNounHead();
	bool isNounTail();
	bool isNounNumber();
	bool isNounPassword();
	bool isNounValue();

	bool isBasicVerb();
	bool isImperativeVerbDisplay();
	bool isImperativeVerbDisplayLoginOrRead();
	bool isImperativeVerbUndoOrRedo();

	unsigned short userNr( WordItem *userWordItem );
	unsigned short wordParameter();

	unsigned int highestSentenceNrInWord();

	signed char assignChangePermissions( char *authorizationKey );
	signed char checkStructuralIntegrityInWord();
	signed char markWordAsFeminine();
	signed char markWordAsMasculine();
	signed char relateSingularNounWithFeminineArticle();
	signed char relateSingularNounWithMasculineArticle();

	char *currentLanguageNameString();
	char *languageNameString( unsigned short languageNr );
	char *userNameString( unsigned short userNr );
	char *wordTypeNameString( unsigned short wordTypeNr );

	WordItem *languageWordItem( unsigned short languageNr );
	WordItem *nextWordItem();
	WordItem *predefinedWordItem( unsigned short wordParameter );


	// Protected assignment functions

	unsigned int nActiveAssignments();

	signed char archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem );
	signed char createNewAssignmentLevelInWord();
	signed char deleteAssignmentLevelInWord();
	signed char inactivateActiveAssignment( SpecificationItem *activeAssignmentItem );

	SpecificationItem *firstActiveNumeralAssignmentItem();
	SpecificationItem *firstActiveStringAssignmentItem();
	SpecificationItem *lastActiveNonQuestionAssignmentItem();

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );
	SpecificationItem *firstAssignmentItem( bool isPossessive, bool isQuestion, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstNonPossessiveActiveAssignmentItem( WordItem *relationWordItem );
	SpecificationItem *firstNonQuestionActiveAssignmentItem();
	SpecificationItem *firstNonQuestionAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem );
	SpecificationItem *firstQuestionAssignmentItem();

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *authorizationKey );


	// Protected cleanup functions

	void clearReplacingInfoInWord();
	void rebuildQuickAccessWordLists();

	unsigned int highestCurrentSentenceItemNrInWord( unsigned int currentSentenceNr, unsigned int currentSentenceItemNr );
	unsigned int highestFoundSentenceNrInWord( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestFoundSentenceNr, unsigned int maxSentenceNr );

	signed char decrementItemNrRangeInWord( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset );
	signed char decrementSentenceNrsInWord( unsigned int startSentenceNr );
	signed char deleteSentencesInWord( unsigned int lowestSentenceNr );
	signed char redoCurrentSentenceInWord();
	signed char removeFirstRangeOfDeletedItemsInWord();
	signed char undoCurrentSentenceInWord();


	// Protected collection functions

	bool hasCollection();
	bool hasCollectionNr( unsigned int collectionNr );

	bool isCompoundCollection( unsigned int collectionNr );
	bool isExclusiveCollection( unsigned int collectionNr );
	bool isNonCompoundCollection( unsigned int collectionNr );
	bool isNonExclusiveCollection( unsigned int collectionNr );
	bool isNounWordSpanishAmbiguous();

	unsigned short highestCollectionOrderNrInCollectionWords( unsigned int collectionNr );

	unsigned int collectionNr( unsigned short collectionWordTypeNr );
	unsigned int collectionNr( WordItem *commonWordItem );
	unsigned int collectionNrByCompoundGeneralizationWordInWord( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem );

	unsigned int compoundCollectionNr();
	unsigned int nonCompoundCollectionNr();
	unsigned int nonCompoundCollectionNrInWord( unsigned int compoundCollectionNr );
	unsigned int highestCollectionNrInCollectionWords();

	WordItem *collectionWordItem( unsigned int compoundCollectionNr );
	WordItem *commonWordItem( unsigned int collectionNr );
	WordItem *compoundGeneralizationWordItem( unsigned int compoundCollectionNr );
	WordItem *feminineCollectionWordItem();
	WordItem *masculineCollectionWordItem();

	BoolResultType findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem );

	CollectionResultType createCollectionItem( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem );


	// Protected context functions

	bool hasContextCurrentlyBeenUpdatedInWord( unsigned int contextNr );
	bool hasContextInWord( unsigned int contextNr );
	bool hasContextInWord( unsigned int contextNr, WordItem *specificationWordItem );
	bool isContextSimilarInWord( unsigned int firstContextNr, unsigned int secondContextNr );
	bool isContextSubsetInContextWords( unsigned int fullSetContextNr, unsigned int subsetContextNr );

	unsigned short contextWordTypeNr( unsigned int contextNr );

	unsigned int contextNr( WordItem *specificationWordItem );
	unsigned int contextNr( unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem );
	unsigned int highestContextNrInWord();
	unsigned int nContextWords( unsigned int contextNr );

	signed char addContext( unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem );

	ContextItem *firstActiveContextItem();
	ContextItem *contextItem( unsigned int contextNr );
	ContextItem *contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, unsigned int spanishAmbiguousCollectionNr, WordItem *specificationWordItem );

	WordItem *firstContextWordItem( unsigned int contextNr );
	WordItem *_nextContextWordItem( unsigned int contextNr );


	// Protected database connection functions

//	signed char storeChangesInWordInFutureDatabase();


	// Protected generalization functions

	signed char createGeneralizationItem( bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem );

	GeneralizationItem *firstGeneralizationItem();
	GeneralizationItem *firstNounSpecificationGeneralizationItem();
	GeneralizationItem *firstProperNounSpecificationGeneralizationItem();
	GeneralizationItem *firstSpecificationGeneralizationItem( bool isOnlySelectingCurrentLanguage );
	GeneralizationItem *firstRelationGeneralizationItem();

	BoolResultType findGeneralization( bool isRelation, WordItem *generalizationWordItem );


	// Protected grammar functions

	void markGrammarOfCurrentLanguageAsChoiceEnd();
	void markGrammarOfCurrentLanguageAsOptionEnd();

	bool isLanguageWord();

	unsigned short nLanguages();

	signed char checkForDuplicateGrammarDefinitionInCurrentLanguage();
	signed char checkGrammarOfCurrentLanguage();
	signed char linkLaterDefinedGrammarWordsInCurrentLanguage();
	signed char shrinkMergedWordsInWriteSentenceOfCurrentLanguage();

	GrammarItem *firstCurrentLanguageReadingGrammarItem();
	GrammarItem *firstCurrentLanguageWritingGrammarItem( bool isQuestion );

	GrammarResultType createGrammarItemForCurrentLanguage( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem );
	GrammarResultType findGrammarOfCurrentLanguage( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString );

	BoolResultType expandMergedWordsInReadSentenceOfCurrentLanguage( char *readUserSentenceString );
	WordEndingResultType analyzeWordEndingWithCurrentLanguage( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString );


	// Protected interface functions

	signed char checkInterfaceOfCurrentLanguage( unsigned short interfaceParameter, char *interfaceString );
	signed char createInterfaceForCurrentLanguage( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString );

	const char *interfaceString( unsigned short interfaceParameter );


	// Protected justification functions

	void checkSpecificationForUsageOfInvolvedWords( SpecificationItem *unusedSpecificationItem );
	void clearJustificationHasBeenWritten();

	bool hasJustification( SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem );
	bool hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();

	signed char cleanupUnreferencedJustifications();
	signed char replaceJustification( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem );
	signed char replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem );
	signed char updateSpecificationOfJustificationsInWord( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned short orderNr, unsigned int secondarySpecificationCollectionNr );

	JustificationResultType copyJustificationItem( SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem );
	JustificationResultType createJustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem, JustificationItem *attachedJustificationItem );

	JustificationItem *foundJustificationItem( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem );
	JustificationItem *primarySpecificationJustificationItem( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem );
	JustificationItem *primaryAndSecondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	JustificationItem *secondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem );

	SpecificationItem *suggestiveQuestionAssumptionSpecificationItem();


	// Protected multiple word functions

	bool isMultipleWord();

	unsigned short matchingMultipleWordParts( char *sentenceString );

	signed char addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem );


	// Protected query functions

	void countQuery();
	void clearQuerySelections();

	void itemQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr );
	void listQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryListString );
	void parameterQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned int queryParameter );
	void wordTypeQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, unsigned short queryWordTypeNr );

	bool hasActiveQuestionWithCompoundCollection();

	signed char displayQueryResultInWord( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth );
	signed char stringQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *queryString );
	signed char wordQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, char *wordReferenceNameString );
	signed char wordReferenceQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString );


	// Protected question functions

	signed char findAnswerToNewUserQuestion();
	signed char findAnswersToQuestions( unsigned int questionSpecificationCollectionNr, SpecificationItem *answerSpecificationItem );
	signed char writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem );


	// Protected specification functions

	void addMyWordToTouchedDuringCurrentSentenceList();
	void checkJustificationForUsageInWord( JustificationItem *unusedJustificationItem );
	void initializeVariablesInWord();

	bool hadOnceAnsweredSelfGeneratedQuestion();
	bool hasAnyUserSpecification();
	bool hasCurrentlyConfirmedSpecification();
	bool hasCurrentlyConfirmedSpecificationAndAtLeastOneRelation();
	bool hasCurrentlyConfirmedSpecificationButNoRelation();
	bool hasCurrentlyCorrectedAssumption();
	bool hasCurrentlyCorrectedAssumptionByKnowledge();
	bool hasCurrentlyCorrectedAssumptionByOppositeSuggestiveQuestion();
	bool hasCurrentlyMoreSpecificSpecification();
	bool hasMultipleSpecificationWordsWithSameSentenceNr( unsigned int creationSentenceNr, unsigned int skipThisItemNr, unsigned int specificationCollectionNr );
	bool hasNegativeSpecification();
	bool hasPartOfSpecification();
	bool hasPossiblyGapInKnowledge( unsigned int exclusiveSecondarySpecificationCollectionNr, unsigned int primarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem );
	bool hasRelationContextInSpecificationsInWord( unsigned int relationContextNr );

	bool isJustificationInUse( JustificationItem *unusedJustificationItem );

	unsigned int nRemainingSpecificationWordsForWriting( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGeneratedSpecification, unsigned short assumptionLevel, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, unsigned int creationSentenceNr );
	unsigned int userSpecificationCollectionNr();

	signed char checkForSpecificationConflict( bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isUserSpecificationWordSpanishAmbiguous, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	signed char clearStoredSentenceStringWithUnknownPluralNoun( const char *unknownPluralNounString, WordItem *specificationWordItem );
	signed char collectGeneralizations( bool isExclusiveGeneralization, unsigned int generalizationCollectionNr );
	signed char collectSpecificationsInWord( bool isExclusiveSpecification, bool isQuestion, unsigned int specificationCollectionNr );
	signed char confirmSpecificationButNotItsRelation( SpecificationItem *confirmationSpecificationItem );
	signed char recalculateAssumptionLevelsInWord();
	signed char removeObsoleteAssumptionJustificationsFromPartSpecificationsInWord();
	signed char replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char updateJustificationInSpecifications( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem );
	signed char updateSpecificationOfJustificationsOfInvolvedWords( bool isIncludingUpdateOfCurrentWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char writeConfirmedSpecification( unsigned short interfaceParameter, SpecificationItem *writeSpecificationItem );

	SpecificationItem *bestAssumptionLevelSpecificationItem( bool isPossessive, WordItem *specificationWordItem );

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingAnsweredQuestions, bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyGeneralizationContext, bool isAllowingEmptyRelationContext, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstActiveQuestionSpecificationItem();
	SpecificationItem *firstAdjectiveSpecificationItem( bool isNegative );

	SpecificationItem *firstAssignmentOrSpecificationItem( bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem );

	SpecificationItem *firstExclusiveSpecificationItem( bool isIncludingAdjectives );
	SpecificationItem *firstExclusiveSpecificationItem( WordItem *specificationWordItem );
	SpecificationItem *firstFeminineOrMasculineSpecificationItem();

	SpecificationItem *firstNegativeSpecificationItem();
	SpecificationItem *firstNonCollectedSpecificationItem( bool isSelfGenerated, WordItem *excludeSpecificcationWordItem );
	SpecificationItem *firstNonCompoundUserSpecificationItem();
	SpecificationItem *firstNonCompoundCollectionSpecificationItem( unsigned int specificationCollectionNr );
	SpecificationItem *firstNonExclusiveSpecificationItem( bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem );
	SpecificationItem *firstNonNegativeNonPossessiveDefinitionSpecificationItem();
	SpecificationItem *firstNonQuestionSpecificationItem();
	SpecificationItem *firstNonQuestionSpecificationItem( bool isAllowingEmptyRelationContext, bool isAssignment, bool isArchivedAssignment );
	SpecificationItem *firstOlderNonPossessiveNonQuestionSpecificationItem( bool isArchivedAssignment );

	SpecificationItem *firstPossessiveSpecificationItem();
	SpecificationItem *firstCandidateForQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification );

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, WordItem *specificationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyRelationContext, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );
	SpecificationItem *firstSpecificationItem( bool isIncludingAnsweredQuestions, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter );
	SpecificationItem *firstSpecificationItem( bool isNegative, bool isPossessive, bool isQuestion, WordItem *specificationWordItem );
	SpecificationItem *firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem );

	SpecificationItem *firstUnhiddenSpanishSpecificationItem();
	SpecificationItem *firstUserSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *noRelationContextSpecificationItem( bool isPossessive, bool isSelfGenerated, WordItem *specificationWordItem );
	SpecificationItem *partOfSpecificationItem( WordItem *specificationWordItem );
	SpecificationItem *sameUserQuestionSpecificationItem( unsigned short questionParameter );

	WordItem *feminineOrMasculineCommonWordItem( bool isFeminineWord );

	CollectionResultType collectExclusiveSpecificationWords( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem );

	CreateAndAssignResultType addSpecificationInWord( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isCharacteristicFor, bool isConditional, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *authorizationKey );
	CreateAndAssignResultType copyAndReplaceSpecification( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem );
	CreateAndAssignResultType createSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isCharacteristicFor, bool isConditional, bool isCorrectedSpecification, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *storedSentenceString, char *storedSentenceWithOnlyOneSpecificationString );

	RelatedResultType findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem );
	RelatedResultType findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );


	// Protected word type functions

	void clearGeneralizationWriteLevel( unsigned short currentWriteLevel );
	void clearSpecificationWriteLevel( unsigned short currentWriteLevel );
	void clearRelationWriteLevel( unsigned short currentWriteLevel );

	bool hasAnyWordType();
	bool hasFeminineAndMasculineArticle( unsigned short articleParameter );
	bool hasFeminineProperNounEnding();
	bool hasFeminineOrMasculineProperNounEnding();
	bool hasMasculineProperNounEnding();
	bool hasWordType( bool isAllowingDifferentNoun, unsigned short wordTypeNr );

	bool isSpecificationWordTypeAlreadyWritten( unsigned short specificationWordTypeNr );
	bool isRelationWordTypeAlreadyWritten( unsigned short relationWordTypeNr );

	bool isCorrectAdjective( unsigned short adjectiveParameter, unsigned short wordTypeNr );
	bool isCorrectDefiniteArticle( unsigned short definiteArticleParameter, unsigned short wordTypeNr );
	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *authorizationKey );
	bool isCorrectIndefiniteArticle( unsigned short indefiniteArticleParameter, unsigned short wordTypeNr );
	bool isNumeralWordType();
	bool isProperNounPrecededByDefiniteArticle( unsigned short definiteArticleParameter );
	bool isProperNounWord();

	signed char deleteAllWordTypesOfCurrentSentence();
	signed char deleteWordType( WordTypeItem *deleteWordTypeItem );
	signed char hideWordType( unsigned short wordTypeNr, char *authorizationKey );

	signed char markGeneralizationWordTypeAsWritten( unsigned short wordTypeNr );
	signed char markSpecificationWordTypeAsWritten( unsigned short wordTypeNr );
	signed char markRelationWordTypeAsWritten( unsigned short wordTypeNr );

	WordTypeItem *activeWordTypeItem( unsigned short wordTypeNr );
	WordTypeItem *activeWordTypeItem( bool isCheckingAllLanguages, unsigned short wordTypeNr );

	char *activeWordTypeString( unsigned short wordTypeNr );
	char *anyWordTypeString();
	char *singularNounString();
	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr );

	WordTypeResultType addWordType( bool isMultipleWord, bool isProperNounPrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString );
	WordResultType findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString );


	// Protected write functions

	void deleteTemporaryWriteList();

	signed char createWriteWord( bool isSkipped, unsigned short grammarLevel, GrammarItem *selectedGrammarItem );
	signed char writeJustificationSpecificationInWord( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem );
	signed char writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem );
	signed char writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem );
	signed char writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isJustification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem );
	signed char writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, WordItem *writeWordItem );
	signed char writeSpecificationSentence( bool isAssignment, bool isArchivedAssignment, bool isCheckingUserSentenceForIntegrity, bool isPossessive, bool isQuestion, bool isSpecificationGeneralization, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem );
	signed char writeUpdatedSpecification( bool isAdjustedSpecification, bool isCorrectedSpecificationByKnowledge, bool isCorrectedSpecificationByOppositeSuggestiveQuestion, bool isForcingResponseNotBeingFirstSpecification, bool isReplacedBySpecificationWithRelation, bool isWritingCurrentSpecificationWordOnly, bool wasHiddenSpanishSpecification, SpecificationItem *writeSpecificationItem );

	WriteItem *firstActiveWriteItem();
	};
#endif

/*************************************************************************
 *	"Those who look to him for help will be radiant with joy;
 *	no shadow of shame wil darken their faces." (Psalm 34:5)
 *************************************************************************/
