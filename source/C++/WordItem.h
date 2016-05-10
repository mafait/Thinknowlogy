/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
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

// WordItem header

#ifndef WORDITEM
#define WORDITEM 1

#include "Item.h"
#include "CollectionResultType.cpp"
#include "GeneralizationResultType.cpp"
#include "GrammarResultType.cpp"
#include "JustificationResultType.cpp"
#include "SpecificationResultType.cpp"
#include "WordResultType.cpp"
#include "WriteResultType.cpp"

// Some compilers need these class declarations
class CollectionList;
class ContextItem;
class ContextList;
class GeneralizationItem;
class GeneralizationList;
class GrammarList;
class InterfaceList;
class JustificationList;
class MultipleWordList;
class SelectionItem;
class SpecificationList;
class WordAssignment;
class WordCleanup;
class WordCollection;
class WordQuestion;
class WordSpecification;
class WordType;
class WordTypeList;
class WordWrite;
class WordWriteSentence;
class WordWriteWords;
class WriteList;

class WordItem : protected Item
	{
	friend class AdminAssumption;
	friend class AdminAuthorization;
	friend class AdminCleanup;
	friend class AdminCollection;
	friend class AdminConclusion;
	friend class AdminContext;
	friend class AdminImperative;
	friend class AdminItem;
	friend class AdminLanguage;
	friend class AdminQuery;
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class AdminReasoning;
	friend class AdminSelection;
	friend class AdminSolve;
	friend class AdminSpecification;
	friend class AdminWriteJustification;
	friend class AdminWriteSpecification;
	friend class CollectionItem;
	friend class CollectionList;
	friend class ContextItem;
	friend class ContextList;
	friend class GeneralizationItem;
	friend class GrammarItem;
	friend class GrammarList;
	friend class Item;
	friend class JustificationItem;
	friend class JustificationList;
	friend class List;
	friend class MultipleWordItem;
	friend class MultipleWordList;
	friend class Presentation;
	friend class ReadItem;
	friend class ReadList;
	friend class SelectionItem;
	friend class SpecificationItem;
	friend class SpecificationList;
	friend class WordAssignment;
	friend class WordCleanup;
	friend class WordCollection;
	friend class WordList;
	friend class WordQuestion;
	friend class WordSpecification;
	friend class WordType;
	friend class WordTypeItem;
	friend class WordWrite;
	friend class WordWriteSentence;
	friend class WordWriteWords;


	// Private constructible variables

	bool isNounWordSpanishAmbiguous_;

	void *changeKey_;

	WordAssignment *wordAssignment_;
	WordCleanup *wordCleanup_;
	WordCollection *wordCollection_;
	WordQuestion *wordQuestion_;
	WordSpecification *wordSpecification_;
	WordType *wordType_;
	WordWrite *wordWrite_;
	WordWriteSentence *wordWriteSentence_;
	WordWriteWords *wordWriteWords_;


	// Private loadable variables

	bool isFemale_;
	bool isFeminineWord_;
	bool isMale_;
	bool isMasculineWord_;
	bool isLanguageWord_;

	unsigned short wordParameter_;


	// Private common functions

	bool isCorrectFeminineOrMasculineArticleWord( unsigned short articleParameter );

	unsigned short nUsers();

	char wordListChar( unsigned short wordListNr );

	char *languageNameStringInWord( unsigned short languageNr );
	char *userNameStringInWord( unsigned short userNr );

	WordItem *languageWordItemInWord( unsigned short languageNr );


	// Private assignment functions

	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );


	// Private collection functions

	unsigned short highestCollectionOrderNrInWord( unsigned int collectionNr );

	unsigned int highestCollectionNrInWord();
	unsigned int nonCompoundCollectionNrInWord( unsigned int compoundCollectionNr );


	// Private context functions

	bool hasContextCurrentlyBeenUpdatedInWord( unsigned int contextNr, WordItem *specificationWordItem );
	bool isContextSubsetInWord( unsigned int fullSetContextNr, unsigned int subsetContextNr );


	// Private grammar functions

	char *grammarStringInWord( unsigned short wordTypeNr );


	// Private specification functions

	ResultType checkSpecificationForUsageInWord( SpecificationItem *unusedSpecificationItem );


	protected:
	// Protected constructible variables

	bool isUserGeneralizationWord;
	bool isUserSpecificationWord;
	bool isUserRelationWord;

	bool isWordCheckedForSolving;
	bool isWordTouchedDuringCurrentSentence;

	unsigned short predefinedMultipleWordNr;

	SpecificationList *assignmentList;
	CollectionList *collectionList;
	ContextList *contextList;
	GeneralizationList *generalizationList;
	GrammarList *grammarList;
	InterfaceList *interfaceList;
	JustificationList *justificationList;
	MultipleWordList *multipleWordList;
	SpecificationList *specificationList;
	WordTypeList *wordTypeList;
	WriteList *writeList;

	List *wordListArray[NUMBER_OF_WORD_LISTS];


	// Protected functions

	ResultType addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 );
	ResultType addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ResultType addErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );

	ResultType addErrorWithWordListNr( unsigned short wordListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );

	ResultType startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 );
	ResultType startErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );

	ResultType startSystemErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );


	// Constructor / deconstructor

	WordItem();
	WordItem( bool isLanguageWord, unsigned short wordParameter, CommonVariables *commonVariables, List *myList );
	~WordItem();


	// Protected virtual functions

	virtual bool hasFoundParameter( unsigned int queryParameter );
	virtual bool isSorted( Item *nextSortItem );

	virtual ReferenceResultType findMatchingWordReferenceString( char *searchString );

	virtual ResultType checkForUsage();

	virtual char *toString( unsigned short queryWordTypeNr );


	// Protected common functions

	bool hasItems();

	bool isAdjectiveAssignedOrEmpty();
	bool isAdjectiveEmpty();
	bool isAdjectiveNo();
	bool isAdjectiveComparison();
	bool isAdjectiveComparisonLess();
	bool isAdjectiveComparisonEqual();
	bool isAdjectiveComparisonMore();
	bool isAdjectiveOdd();
	bool isAdjectiveEven();
	bool isAdjectiveOddOrEven();

	bool isAdminWord();
	bool isFemale();
	bool isMale();
	bool isFemaleOrMale();

	bool isNeedingAuthorizationForChanges();

	bool isNounHead();
	bool isNounTail();
	bool isNounNumber();
	bool isNounPassword();
	bool isNounUser();
	bool isNounValue();

	bool isBasicVerb();
	bool isVerbImperativeLoginOrReadOrShow();
	bool isVerbUndoOrRedo();
	bool isUserDefinedWord();

	unsigned short getUserNr( WordItem *userWordItem );
	unsigned short wordParameter();

	ResultType assignChangePermissions( void *changeKey );
	ResultType markWordAsFemale();
	ResultType markWordAsFeminine();
	ResultType markWordAsMale();
	ResultType markWordAsMasculine();

	char *languageNameString( unsigned short languageNr );
	char *userNameString( unsigned short userNr );
	char *wordTypeNameString( unsigned short wordTypeNr );

	WordItem *languageWordItem( unsigned short languageNr );
	WordItem *nextWordItem();
	WordItem *predefinedWordItem( unsigned short wordParameter );


	// Protected assignment functions

	unsigned int nActiveAssignments();

	ResultType createNewAssignmentLevelInWord();
	ResultType deleteAssignmentLevelInWord();
	ResultType inactivateActiveAssignment( SpecificationItem *activeAssignmentItem );
	ResultType archiveInactiveAssignment( SpecificationItem *inactiveAssignmentItem );

	SpecificationResultType getAssignmentOrderNr();
	SpecificationResultType getAssignmentWordParameter();

	SpecificationResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, void *changeKey );

	SpecificationItem *firstActiveNumeralAssignmentItem();
	SpecificationItem *firstActiveStringAssignmentItem();
	SpecificationItem *firstNonQuestionActiveAssignmentItem();
	SpecificationItem *lastActiveNonQuestionAssignmentItem();

	SpecificationItem *firstActiveAssignmentItem( unsigned short questionParameter );
	SpecificationItem *firstActiveAssignmentItem( bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem );
	SpecificationItem *firstActiveNonQuestionAssignmentItem( bool isDifferentRelationContext, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstNonQuestionAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstQuestionAssignmentItem();
	SpecificationItem *firstRecentlyReplacedAssignmentItem( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );


	// Protected cleanup functions

	void deleteTemporaryWriteList();
	void getHighestInUseSentenceNrInWord( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr );
	void setCurrentItemNrInWord();

	ResultType clearReplacedInfoInWord();
	ResultType decrementItemNrRangeInWord( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset );
	ResultType decrementSentenceNrsInWord( unsigned int startSentenceNr );
	ResultType deleteSentencesInWord( unsigned int lowestSentenceNr );
	ResultType redoCurrentSentence();
	ResultType removeFirstRangeOfDeletedItemsInWord();
	ResultType undoCurrentSentence();


	// Protected collection functions

	bool hasCollection();
	bool hasCollectionNr( unsigned int collectionNr );
	bool hasCollectionNr( unsigned int collectionNr, WordItem *commonWordItem );
	bool hasNonExclusiveCollection( unsigned int collectionNr );

	bool isCollectionSpanishAmbiguous( unsigned int collectionNr );
	bool isCompoundCollection( unsigned int collectionNr );
	bool isCompoundCollection( unsigned int collectionNr, WordItem *commonWordItem );
	bool isNonCompoundCollection( unsigned int collectionNr );

	bool isNounWordSpanishAmbiguous();
	bool isSpanishCurrentLanguage();

	unsigned short collectionOrderNr( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem );
	unsigned short highestCollectionOrderNrInAllWords( unsigned int collectionNr );

	unsigned int collectionNr( unsigned short collectionWordTypeNr );
	unsigned int collectionNr( unsigned short collectionWordTypeNr, WordItem *commonWordItem );
	unsigned int collectionNrByCompoundGeneralizationWordInWord( unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem );

	unsigned int compoundCollectionNr( unsigned short collectionWordTypeNr );
	unsigned int nonCompoundCollectionNr( unsigned short collectionWordTypeNr );
	unsigned int nonCompoundCollectionNrInAllWords( unsigned int compoundCollectionNr );
	unsigned int highestCollectionNrInAllWords();

	CollectionResultType addCollection( bool isExclusiveSpecification, bool isSpecificationGeneralization, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem );
	CollectionResultType addCollectionByGeneralization( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem );
	CollectionResultType findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem );

	ResultType addCollectionByGeneralization( bool isExclusiveGeneralization, bool isQuestion, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem );

	WordItem *collectionWordItem( unsigned int compoundCollectionNr, WordItem *notThisCommonWordItem );
	WordItem *commonWordItem( unsigned int collectionNr );
	WordItem *compoundGeneralizationWordItem( unsigned int compoundCollectionNr );
	WordItem *feminineCollectionWordItem();


	// Protected context functions

	bool hasContextCurrentlyBeenUpdatedInAllWords( unsigned int contextNr, WordItem *specificationWordItem );
	bool hasContextInWord( unsigned int contextNr );
	bool hasContextInWord( unsigned int contextNr, WordItem *specificationWordItem );
	bool isContextSimilarInWord( unsigned int firstContextNr, unsigned int secondContextNr );
	bool isContextSubsetInAllWords( unsigned int fullSetContextNr, unsigned int subsetContextNr );

	unsigned short contextWordTypeNrInWord( unsigned int contextNr );

	unsigned int contextNr( WordItem *specificationWordItem );
	unsigned int contextNr( bool isCompoundCollectionSpanishAmbiguous, WordItem *specificationWordItem );
	unsigned int highestContextNrInWord();
	unsigned int nContextWordsInAllWords( unsigned int contextNr, WordItem *specificationWordItem );

	ResultType addContext( bool isCompoundCollectionSpanishAmbiguous, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem );

	ContextItem *firstActiveContextItem();
	ContextItem *contextItem( unsigned int contextNr );
	ContextItem *contextItem( WordItem *specificationWordItem );
	ContextItem *contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, WordItem *specificationWordItem );

	WordItem *contextWordItemInAllWords( unsigned int contextNr, WordItem *specificationWordItem, WordItem *previousWordItem );


	// Protected database connection functions

//	ResultType storeWordItemInFutureDatabase();


	// Protected generalization functions

	GeneralizationResultType findGeneralization( bool isRelation, WordItem *generalizationWordItem );

	ResultType createGeneralizationItem( bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem );

	GeneralizationItem *firstGeneralizationItem();
	GeneralizationItem *firstNounSpecificationGeneralizationItem();
	GeneralizationItem *firstSpecificationGeneralizationItem( bool isOnlySelectingCurrentLanguage );
	GeneralizationItem *firstRelationGeneralizationItem();


	// Protected grammar functions

	void markAsChoiceEnd( unsigned int choiceEndItemNr );
	void markAsOptionEnd();

	bool isNeededToCheckGrammar();
	bool isLanguageWithMergedWords();
	bool isLanguageWord();

	unsigned short nLanguages();

	GrammarResultType analyzeWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString );
	GrammarResultType createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem );
	GrammarResultType findGrammar( bool ignoreValue, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString );
	GrammarResultType checkForDuplicateGrammarDefinition();

	ResultType checkGrammar();
	ResultType checkGrammarForUsageInWord( GrammarItem *unusedGrammarItem );
	ResultType linkLaterDefinedGrammarWords();

	ResultType expandMergedWordsInReadSentence( char *readUserSentenceString );
	ResultType shrinkMergedWordsInWriteSentence();

	GrammarItem *firstGrammarItem();


	// Protected interface functions

	ResultType checkInterface( unsigned short interfaceParameter, char *interfaceString );
	ResultType createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString );

	const char *interfaceString( unsigned short interfaceParameter );


	// Protected justification functions

	void clearJustificationHasBeenWritten();

	JustificationResultType addJustification( bool hasFeminineOrMasculineProperNameEnding, bool isForcingNewJustification, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem );
	JustificationResultType copyJustificationItem( SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem );

	ResultType attachJustification( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem );
	ResultType checkForUnreferencedJustifications();
	ResultType checkJustificationsForReplacedSpecifications();
	ResultType checkSpecificationForUsageOfInvolvedWords( SpecificationItem *unusedSpecificationItem );
	ResultType replaceJustification( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem );
	ResultType replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem );
	ResultType replaceOrDeleteObsoleteJustifications();
	ResultType updateSpecificationsInJustificationInWord( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	ResultType writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr );

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem();
	JustificationItem *negativeAssumptionOrConclusionJustificationItem( SpecificationItem *secondarySpecificationItem );
	JustificationItem *olderComplexJustificationItem( bool hasSecondarySpecificationWithoutRelationContext, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem );
	JustificationItem *possessiveReversibleAssumptionJustificationItem( WordItem *secondarySpecificationWordItem );
	JustificationItem *primarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem );
	JustificationItem *secondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem );
	JustificationItem *specificationSubstitutionPartOfAssumptionJustificationItem( SpecificationItem *secondarySpecificationItem );

	SpecificationItem *olderSuggestiveQuestionAssumptionSecondarySpecificationItem();


	// Protected multiple word functions

	bool isMultipleWord();

	unsigned short matchingMultipleSingularNounWordParts( char *sentenceString );

	ResultType addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem );


	// Protected query functions

	void countQuery();
	void clearQuerySelections();

	bool hasActiveQuestionWithCompoundCollection();

	ResultType itemQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr );
	ResultType listQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString );
	ResultType wordTypeQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr );
	ResultType parameterQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter );
	ResultType wordQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordReferenceNameString );
	ResultType wordReferenceQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString );
	ResultType stringQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString );

	ResultType showQueryResultInWord( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth );


	// Protected question functions

	bool hasFoundAnswerToQuestion();

	ResultType findAnswerToNewUserQuestion();
	ResultType findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem );
	ResultType writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem );

	SpecificationResultType findQuestionToBeAdjustedByCompoundCollection( bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCompoundCollectionNr, unsigned int relationContextNr, SpecificationItem *replacingSpecificationItem, WordItem *specificationWordItem );


	// Protected selection functions

	ResultType checkSelectionForUsageInWord( SelectionItem *unusedSelectionItem );


	// Protected specification functions

	void initializeVariablesInWord();

	bool hasAnsweredQuestion();
	bool hasConfirmedSpecification();
	bool hasCorrectedAssumption();
	bool hasCorrectedAssumptionByKnowledge();
	bool hasCorrectedAssumptionByOppositeQuestion();
	bool hasFoundAnyUserSpecification();
	bool hasNonPossessiveAndNonQuestionPartOfSpecification();
	bool hasPossessiveButNonQuestionSpecification();
	bool hasShownMoreSpecificRelatedQuestion();

	bool isAuthorizedForChanges( void *changeKey );
	bool isJustificationInUse( JustificationItem *unusedJustificationItem );

	ResultType changeJustificationOfNegativeAssumptions( SpecificationItem *secondarySpecificationItem );
	ResultType checkForSpecificationConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	ResultType checkJustificationForUsageInWord( JustificationItem *unusedJustificationItem );
	ResultType checkSpecificationsForReplacedOrDeletedJustifications( bool isAllowingNewerReplaceOrDeleteSentenceNr );
	ResultType clearLastWrittenSentenceStringWithUnknownPluralNoun( const char *unknownPluralNounString, WordItem *specificationWordItem );
	ResultType collectGeneralizationsOrSpecifications( bool isExclusiveGeneralization, bool isGeneralizationCollection, bool isQuestion, unsigned int collectionNr );
	ResultType confirmSpecificationButNotItsRelation( SpecificationItem *confirmedSpecificationItem, SpecificationItem *confirmationSpecificationItem );
	ResultType copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem );
	ResultType recalculateAssumptionsInAllTouchedWords();
	ResultType recalculateAssumptions();
	ResultType replaceOrDeleteSpecification( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	ResultType updateJustificationInSpecifications( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem );
	ResultType updateSpecificationsInJustificationsOfInvolvedWords( SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );

	SpecificationResultType addSpecification( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, void *changeKey );
	SpecificationResultType createSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveSpecification, bool isGeneralizationAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int generalizationCollectionNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *writtenSentenceString );

	SpecificationResultType findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem );
	SpecificationResultType findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingAnsweredQuestions, bool isIncludingAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstActiveQuestionSpecificationItem();
	SpecificationItem *firstActiveAssignmentOrSpecificationItem( bool isIncludingAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem );

	SpecificationItem *firstAnsweredQuestionAssignmentItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstFeminineOrMasculineSpecificationItem();

	SpecificationItem *firstNonCompoundCollectionSpecificationItem( unsigned int specificationCollectionNr );
	SpecificationItem *firstNonQuestionSpecificationItem();

	SpecificationItem *firstRecentlyAnsweredQuestionSpecificationItem( unsigned short questionParameter );
	SpecificationItem *firstRelationSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem );

	SpecificationItem *firstSelectedSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );
	SpecificationItem *firstSelectedSpecificationItem( bool isIncludingAnsweredQuestions, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter );

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, WordItem *specificationWordItem );

	SpecificationItem *firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem );

	SpecificationItem *firstUnhiddenSpecificationItem( unsigned int relationContextNr );
	SpecificationItem *firstUserSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *sameUserQuestionSpecificationItem( unsigned short questionParameter );

	WordItem *feminineOrMasculineCommonWordItem( bool isFeminineWord );


	// Protected word type functions

	void clearGeneralizationWriteLevel( unsigned short currentWriteLevel );
	void clearSpecificationWriteLevel( unsigned short currentWriteLevel );
	void clearRelationWriteLevel( unsigned short currentWriteLevel );

	bool hasFeminineAndMasculineArticle( unsigned short articleParameter );
	bool hasFeminineProperNameEnding();
	bool hasMasculineProperNameEnding();

	bool hasWordType( bool isAllowingDifferentNoun, unsigned short wordTypeNr );

	bool isGeneralizationWordTypeAlreadyWritten( unsigned short generalizationWordTypeNr );
	bool isSpecificationWordTypeAlreadyWritten( unsigned short specificationWordTypeNr );
	bool isRelationWordTypeAlreadyWritten( unsigned short relationWordTypeNr );

	bool isCorrectAdjective( unsigned short adjectiveParameter, unsigned short wordTypeNr );
	bool isCorrectDefiniteArticle( unsigned short definiteArticleParameter, unsigned short wordTypeNr );
	bool isCorrectIndefiniteArticle( unsigned short indefiniteArticleParameter, unsigned short wordTypeNr );
	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, void *authorizationKey );
	bool isNumeralWordType();
	bool isSingularNoun();
	bool isPluralNoun();
	bool isProperName();
	bool isProperNamePrecededByDefiniteArticle( unsigned short definiteArticleParameter );

	ResultType deleteWordType( unsigned short wordTypeNr );
	ResultType hideWordType( unsigned short wordTypeNr, void *authorizationKey );

	ResultType markGeneralizationWordTypeAsWritten( unsigned short wordTypeNr );
	ResultType markSpecificationWordTypeAsWritten( unsigned short wordTypeNr );
	ResultType markRelationWordTypeAsWritten( unsigned short wordTypeNr );

	WordResultType addWordType( bool isMultipleWord, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString );
	WordResultType findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString );

	WordTypeItem *activeWordTypeItem( unsigned short wordTypeNr );
	WordTypeItem *activeWordTypeItem( bool isCheckingAllLanguages, unsigned short wordTypeNr );

	char *anyWordTypeString();
	char *activeWordTypeString( unsigned short wordTypeNr );
	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr );

	char *singularNounString();


	// Protected write functions

	ResultType writeJustificationSpecification( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem );

	ResultType writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem );
	ResultType writeSelectedSpecification( bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem );

	ResultType writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem );
	ResultType writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem );

	ResultType writeUpdatedSpecification( bool isAdjustedSpecification, bool isConcludedAssumption, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, SpecificationItem *writeSpecificationItem );


	// Protected write sentence functions

	ResultType selectGrammarToWriteSentence( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem );


	// Protected write words functions

	void initializeWordWriteWordsVariables();
	void initializeWordWriteWordsSpecificationVariables( bool isQuestion, size_t startWordPosition );

	WriteResultType writeWordsToSentence( bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, GrammarItem *definitionGrammarItem, SpecificationItem *writeSpecificationItem );
	};
#endif

/*************************************************************************
 *	"For the angel of the Lord is a guard;
 *	he surrounds and defends all who fear him." (Psalm 34:7)
 *************************************************************************/
