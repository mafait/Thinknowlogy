/*	Class:			WordItem
 *	Parent class:	Item
 *	Purpose:		To store and process word information
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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
class ShortResultType;
class SpecificationList;
class WordEndingResultType;
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
	friend class AdminReasoningNew;
	friend class AdminReasoningOld;
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

	bool isNounWordSpanishAmbiguous_;

	char *authorizationKey_;

	// Word item supporting modules
	WordQuestion *wordQuestion_;
	WordSpecification *wordSpecification_;
	WordWrite *wordWrite_;

	// Word item lists
	SpecificationList *assignmentList_;
	CollectionList *collectionList_;
	ContextList *contextList_;
	GeneralizationList *generalizationList_;
	GrammarList *grammarList_;
	InterfaceList *interfaceList_;
	JustificationList *justificationList_;
	MultipleWordList *multipleWordList_;
	SpecificationList *specificationList_;
	WordTypeList *wordTypeList_;
	WriteList *writeList_;

	// Word item lists array
	List *wordListArray_[NUMBER_OF_WORD_LISTS];

	// This string is returned by a function. So, it must be "static".
	char wordItemNameString_[MAX_SENTENCE_STRING_LENGTH];


	// Private initialized variables

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

	// Private error functions

	signed char addErrorWithWordListNr( unsigned short wordListNr, const char *functionNameString, const char *moduleNameString, const char *errorString );

	BoolResultType startBoolResultErrorInWord( const char *functionNameString, const char *errorString );

	CollectionResultType addCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CollectionResultType startCollectionResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	GrammarResultType startGrammarResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	JustificationResultType startJustificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	WordTypeResultType startWordTypeResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	// Private assignment functions

	signed char inactivateAssignment( bool isAmbiguousRelationContext, bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	signed char inactivateCurrentAssignments();

	SpecificationItem *firstActiveNonQuestionAssignmentItem( bool isDifferentRelationContext, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString );
	CreateAndAssignResultType assignSpecificationByValue( JustificationItem *firstJustificationItem, WordItem *specificationWordItem );

	// Private collection functions

	unsigned short highestCollectionOrderNrInWord( unsigned int collectionNr );

	unsigned int highestCollectionNrInWord();

	// Private context functions

	bool isContextSubsetInWord( unsigned int fullSetContextNr, unsigned int subsetContextNr );

	// Private grammar functions

	char *grammarStringInWord( unsigned short wordTypeNr );

	// Private specification functions

	bool isAuthorizedForChanges( char *authorizationKey );

	signed char checkSpecificationForUsageInWord( SpecificationItem *unusedSpecificationItem );

	SpecificationItem *firstRelationSpecificationItem( bool isInactiveAssignment, bool isArchivedAssignment, bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem );

	// Private word type functions

	bool isSingularNoun();


	protected:
	// Protected constructed variables

	bool isUserGeneralizationWord;
	bool isUserSpecificationWord;
	bool isUserRelationWord;

	bool isWordCheckedForSolving;

	unsigned short predefinedMultipleWordNr;

	WordItem *nextAssignmentWordItem;
	WordItem *nextCollectionWordItem;
	WordItem *nextContextWordItem;
	WordItem *nextPossessiveNounWordItem;
	WordItem *nextSpecificationWordItem;
	WordItem *nextTouchedProperNameWordItem;


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

	SpecificationResultType addSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	SpecificationResultType startSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );

	UserSpecificationResultType addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	UserSpecificationResultType addUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	UserSpecificationResultType startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString );
	UserSpecificationResultType startUserSpecificationResultErrorInWord( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );


	// Constructor

	WordItem();
	WordItem( bool isLanguageWord, unsigned short wordParameter, CommonVariables *commonVariables, InputOutput *inputOutput, List *myList );
	~WordItem();


	// Protected virtual functions

	virtual void checkForUsage();

	virtual bool hasParameter( unsigned int queryParameter );
	virtual bool isSorted( Item *nextSortItem );

	virtual char *itemToString( unsigned short queryWordTypeNr );

	virtual BoolResultType findMatchingWordReferenceString( char *searchString );


	// Protected common functions

	bool isAdjectiveAssigned();
	bool isAdjectiveAssignedOrEmpty();
	bool isAdjectiveNo();
	bool isAdjectiveComparison();
	bool isAdjectiveComparisonLess();
	bool isAdjectiveComparisonEqual();
	bool isAdjectiveComparisonMore();
	bool isAdjectiveOdd();
	bool isAdjectiveEven();
	bool isAdjectiveOddOrEven();

	bool isAdminWord();
	bool isAuthorizationRequiredForChanges();
	bool isFemale();
	bool isMale();
	bool isFemaleOrMale();

	bool isNounHead();
	bool isNounTail();
	bool isNounNumber();
	bool isNounPassword();
	bool isNounValue();

	bool isBasicVerb();
	bool isVerbImperativeDisplayOrLoginOrRead();
	bool isVerbUndoOrRedo();

	unsigned short userNr( WordItem *userWordItem );
	unsigned short wordParameter();

	signed char assignChangePermissions( char *authorizationKey );
	signed char checkStructuralIntegrityInWord();
	signed char markWordAsFemale();
	signed char markWordAsFeminine();
	signed char markWordAsMale();
	signed char markWordAsMasculine();

	char *languageNameString( unsigned short languageNr );
	char *userNameString( unsigned short userNr );
	char *wordTypeNameString( unsigned short wordTypeNr );

	WordItem *languageWordItemInWord( unsigned short languageNr );
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
	SpecificationItem *firstNonQuestionActiveAssignmentItem();
	SpecificationItem *lastActiveNonQuestionAssignmentItem();

	SpecificationItem *firstActiveAssignmentItem( bool isPossessive, unsigned short questionParameter, WordItem *relationWordItem );

	SpecificationItem *firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstNonQuestionAssignmentItem( bool isIncludingActiveAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstQuestionAssignmentItem();

	CreateAndAssignResultType assignSpecification( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *authorizationKey );


	// Protected cleanup functions

	void clearReplacingInfoInWord();
	void getHighestInUseSentenceNrInWord( bool isIncludingDeletedItems, bool isIncludingTemporaryLists, unsigned int highestSentenceNr );
	void rebuildQuickAccessWordLists();
	void setCurrentItemNrInWord();

	signed char decrementItemNrRangeInWord( unsigned int decrementSentenceNr, unsigned int decrementItemNr, unsigned int decrementOffset );
	signed char decrementSentenceNrsInWord( unsigned int startSentenceNr );
	signed char deleteSentencesInWord( unsigned int lowestSentenceNr );
	signed char redoCurrentSentenceInWord();
	signed char removeFirstRangeOfDeletedItemsInWord();
	signed char undoCurrentSentenceInWord();


	// Protected collection functions

	bool hasCollection();
	bool hasCollectionNr( unsigned int collectionNr );
	bool hasCollectionNr( unsigned int collectionNr, WordItem *commonWordItem );
	bool hasCollectionNr( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem );
	bool hasNonExclusiveCollection( unsigned int collectionNr );

	bool isCollectionSpanishAmbiguous( unsigned int collectionNr );
	bool isCompoundCollection( unsigned int collectionNr );
	bool isNonCompoundCollection( unsigned int collectionNr );

	bool isNounWordSpanishAmbiguous();
	bool isSpanishCurrentLanguage();

	unsigned short collectionOrderNr( unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem );
	unsigned short highestCollectionOrderNrInCollectionWords( unsigned int collectionNr );

	unsigned int collectionNr( unsigned short collectionWordTypeNr );
	unsigned int collectionNr( unsigned short collectionWordTypeNr, WordItem *commonWordItem );
	unsigned int collectionNrByCompoundGeneralizationWordInWord( bool isExclusiveSpecification, unsigned short collectionWordTypeNr, WordItem *compoundGeneralizationWordItem );

	unsigned int compoundCollectionNr( unsigned short collectionWordTypeNr );
	unsigned int nonCompoundCollectionNr( unsigned short collectionWordTypeNr );
	unsigned int nonCompoundCollectionNrInWord( unsigned int compoundCollectionNr );
	unsigned int highestCollectionNrInCollectionWords();

	WordItem *collectionWordItem( unsigned int compoundCollectionNr );
	WordItem *commonWordItem( unsigned int collectionNr );
	WordItem *compoundGeneralizationWordItem( unsigned int compoundCollectionNr );
	WordItem *feminineCollectionWordItem();

	BoolResultType findCollection( bool isAllowingDifferentCommonWord, WordItem *collectionWordItem, WordItem *commonWordItem );

	CollectionResultType addCollection( bool isExclusiveSpecification, bool isSpecificationGeneralization, unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, unsigned int collectionNr, WordItem *collectionWordItem, WordItem *commonWordItem, WordItem *compoundGeneralizationWordItem );


	// Protected context functions

	bool hasContextCurrentlyBeenUpdatedInWord( unsigned int contextNr, WordItem *specificationWordItem );
	bool hasContextInWord( unsigned int contextNr, WordItem *specificationWordItem );
	bool isContextSimilarInWord( unsigned int firstContextNr, unsigned int secondContextNr );
	bool isContextSubsetInContextWords( unsigned int fullSetContextNr, unsigned int subsetContextNr );

	unsigned short contextWordTypeNr( unsigned int contextNr );

	unsigned int contextNr( WordItem *specificationWordItem );
	unsigned int contextNr( bool isCompoundCollectionSpanishAmbiguous, WordItem *specificationWordItem );
	unsigned int highestContextNrInWord();
	unsigned int nContextWordsInContextWords( unsigned int contextNr, WordItem *specificationWordItem );

	signed char addContext( bool isCompoundCollectionSpanishAmbiguous, unsigned short contextWordTypeNr, unsigned short specificationWordTypeNr, unsigned int contextNr, WordItem *specificationWordItem );

	ContextItem *firstActiveContextItem();
	ContextItem *contextItem( unsigned int contextNr );
	ContextItem *contextItem( bool isCompoundCollectionSpanishAmbiguous, unsigned int nContextWords, WordItem *specificationWordItem );


	// Protected database connection functions

	signed char storeChangesInFutureDatabaseInWord();


	// Protected generalization functions

	signed char createGeneralizationItem( bool isRelation, unsigned short specificationWordTypeNr, unsigned short generalizationWordTypeNr, WordItem *generalizationWordItem );

	GeneralizationItem *firstGeneralizationItem();
	GeneralizationItem *firstNounSpecificationGeneralizationItem();
	GeneralizationItem *firstSpecificationGeneralizationItem( bool isOnlySelectingCurrentLanguage );
	GeneralizationItem *firstRelationGeneralizationItem();

	BoolResultType findGeneralization( bool isRelation, WordItem *generalizationWordItem );


	// Protected grammar functions

	void checkGrammarForUsageInWord( GrammarItem *unusedGrammarItem );
	void markAsChoiceEnd( unsigned int choiceEndItemNr );
	void markAsOptionEnd();

	bool isCheckingGrammarNeeded();
	bool isLanguageWithMergedWords();
	bool isLanguageWord();

	unsigned short nLanguages();

	signed char checkForDuplicateGrammarDefinition();
	signed char checkGrammar();
	signed char linkLaterDefinedGrammarWords();
	signed char expandMergedWordsInReadSentence( char *readUserSentenceString );
	signed char shrinkMergedWordsInWriteSentence();

	GrammarResultType createGrammarItem( bool isDefinitionStart, bool isNewStart, bool isOptionStart, bool isChoiceStart, bool isSkipOptionForWriting, unsigned short wordTypeNr, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString, GrammarItem *definitionGrammarItem );
	GrammarResultType findGrammar( bool isIgnoringGrammarParameter, unsigned short grammarParameter, size_t grammarStringLength, char *grammarString );

	WordEndingResultType analyzeWordEnding( unsigned short grammarParameter, size_t originalWordStringLength, char *originalWordString );

	GrammarItem *firstGrammarItem();


	// Protected interface functions

	signed char checkInterface( unsigned short interfaceParameter, char *interfaceString );
	signed char createInterfaceItem( unsigned short interfaceParameter, size_t interfaceStringLength, char *interfaceString );

	const char *interfaceString( unsigned short interfaceParameter );


	// Protected justification functions

	void checkSpecificationForUsageOfInvolvedWords( SpecificationItem *unusedSpecificationItem );
	void clearJustificationHasBeenWritten();

	bool hasJustification( unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem );
	bool hasQuestionJustificationWithNonCompoundSecondarySpecificationCollection();

	signed char attachJustificationInWord( JustificationItem *attachJustificationItem, SpecificationItem *involvedSpecificationItem );
	signed char replaceJustification( JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem, SpecificationItem *involvedSpecificationItem );
	signed char replaceOrDeleteJustification( JustificationItem *obsoleteJustificationItem );
	signed char replaceOrDeleteObsoleteJustifications();
	signed char updateSpecificationsInJustificationInWord( bool isMainWord, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char writeRelatedJustificationSpecifications( unsigned short justificationTypeNr, unsigned int specificationCollectionNr );

	JustificationResultType addJustification( bool hasFeminineOrMasculineProperNameEnding, bool isForcingNewJustification, bool isIncrementingOrderNr, unsigned short justificationTypeNr, unsigned short orderNr, unsigned int originalSentenceNr, SpecificationItem *primarySpecificationItem, SpecificationItem *anotherPrimarySpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *anotherSecondarySpecificationItem, JustificationItem *attachedJustificationItem );
	JustificationResultType copyJustification( bool isForcingNewJustification, SpecificationItem *newPrimarySpecificationItem, SpecificationItem *newSecondarySpecificationItem, JustificationItem *newAttachedJustificationItem, JustificationItem *originalJustificationItem );

	JustificationItem *correctedAssumptionByOppositeQuestionJustificationItem();
	JustificationItem *negativeAssumptionOrConclusionJustificationItem( SpecificationItem *secondarySpecificationItem );
	JustificationItem *olderComplexJustificationItem( bool hasSecondarySpecificationWithoutRelationContext, unsigned short justificationTypeNr, unsigned int secondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem );
	JustificationItem *possessiveReversibleAssumptionJustificationItem( WordItem *generalizationWordItem, WordItem *secondarySpecificationWordItem );
	JustificationItem *primarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem );
	JustificationItem *secondarySpecificationJustificationItem( bool isSelectingOlderItemOnly, unsigned short justificationTypeNr, SpecificationItem *secondarySpecificationItem );

	SpecificationItem *olderSuggestiveQuestionAssumptionSecondarySpecificationItem();


	// Protected multiple word functions

	bool isMultipleWord();

	unsigned short matchingMultipleSingularNounWordParts( char *sentenceString );

	signed char addMultipleWord( unsigned short nWordParts, unsigned short wordTypeNr, WordItem *multipleWordItem );


	// Protected query functions

	void countQuery();
	void clearQuerySelections();

	void itemQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr );
	void listQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString );
	void parameterQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter );
	void wordTypeQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr );

	bool hasActiveQuestionWithCompoundCollection();

	signed char displayQueryResultInWord( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth );
	signed char stringQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString );
	signed char wordQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *wordReferenceNameString );
	signed char wordReferenceQueryInWord( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString );


	// Protected question functions

	bool hasFoundAnswerToQuestion();

	signed char findAnswerToNewUserQuestion();
	signed char findAnswersToQuestions( unsigned int compoundSpecificationCollectionNr, SpecificationItem *answerSpecificationItem );
	signed char writeAnswerToQuestion( bool isNegativeAnswer, bool isPositiveAnswer, bool isUncertainAboutRelation, SpecificationItem *answerSpecificationItem );


	// Protected specification functions

	void checkJustificationForUsageInWord( JustificationItem *unusedJustificationItem );
	void initializeVariablesInWord();

	bool hasAnsweredSelfGeneratedQuestion();
	bool hasAnyUserSpecification();
	bool hasConfirmedSpecification();
	bool hasConfirmedSpecificationButNoRelation();
	bool hasContextInSpecificationsInWord( unsigned int contextNr );
	bool hasCorrectedAssumption();
	bool hasCorrectedAssumptionByKnowledge();
	bool hasCorrectedAssumptionByOppositeQuestion();
	bool hasDisplayedMoreSpecificSpecification();
	bool hasDisplayedMoreSpecificNonExclusiveSpecification();
	bool hasDisplayedMoreSpecificQuestion();
	bool hasNonPossessiveAndNonQuestionPartOfSpecification();
	bool hasPossessiveButNonQuestionSpecification();
	bool hasPossiblyGapInKnowledge( unsigned int exclusiveSecondarySpecificationCollectionNr, SpecificationItem *primarySpecificationItem );

	bool isJustificationInUse( JustificationItem *unusedJustificationItem );

	signed char changeJustificationOfNegativeAssumptions( SpecificationItem *secondarySpecificationItem );
	signed char checkForSpecificationConflict( bool isArchivedAssignment, bool isExclusiveSpecification, bool isGeneralizationProperName, bool isNegative, bool isPossessive, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	signed char clearLastWrittenSentenceStringWithUnknownPluralNoun( const char *unknownPluralNounString, WordItem *specificationWordItem );
	signed char collectGeneralizations( bool isExclusiveGeneralization, unsigned int generalizationCollectionNr );
	signed char collectSpecificationsInWord( bool isExclusiveSpecification, bool isQuestion, unsigned int specificationCollectionNr );
	signed char confirmSpecificationButNotItsRelation( SpecificationItem *confirmedSpecificationItem, SpecificationItem *confirmationSpecificationItem );
	signed char copyAndReplaceSpecificationItem( bool isNewAnsweredQuestion, bool isNewExclusiveGeneralization, bool isNewExclusiveSpecification, unsigned int newGeneralizationCollectionNr, unsigned int newSpecificationCollectionNr, JustificationItem *newFirstJustificationItem, SpecificationItem *originalSpecificationItem );
	signed char recalculateAssumptionsInWord();
	signed char replaceOrDeleteSpecification( bool isAllowingReplacedObsoleteSpecification, SpecificationItem *obsoleteSpecificationItem, SpecificationItem *replacingSpecificationItem );
	signed char updateJustificationInSpecifications( bool isExclusiveGeneralization, JustificationItem *obsoleteJustificationItem, JustificationItem *replacingJustificationItem );

	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *bestMatchingRelationContextNrSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingAnsweredQuestions, bool isIncludingAssignments, bool isIncludingInactiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *bestMatchingSpecificationWordSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingActiveAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstActiveQuestionSpecificationItem();
	SpecificationItem *firstActiveAssignmentOrSpecificationItem( bool isIncludingAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem );

	SpecificationItem *firstAnsweredQuestionAssignmentItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, WordItem *specificationWordItem );
	SpecificationItem *firstAssignmentOrSpecificationItem( bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *firstExclusiveNounSpecificationItem();
	SpecificationItem *firstExclusiveSpecificationItem();
	SpecificationItem *firstFeminineOrMasculineSpecificationItem();

	SpecificationItem *firstNonCompoundUserSpecificationItem();
	SpecificationItem *firstNonCompoundCollectionSpecificationItem( unsigned int specificationCollectionNr );
	SpecificationItem *firstNonPossessiveDefinitionSpecificationItem( bool isIncludingAdjectives );
	SpecificationItem *firstNonQuestionSpecificationItem();

	SpecificationItem *firstPossessiveSpecificationItem();
	SpecificationItem *firstPrimarySpecificationCandidateOfQuestionSpecificationItem( bool isAllowingSpanishPossessiveSpecification );
	SpecificationItem *firstRecentlyAnsweredQuestionSpecificationItem( unsigned short questionParameter );
	SpecificationItem *firstRelationSpecificationItem();

	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGeneratedAssumption, WordItem *specificationWordItem, WordItem *relationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isAllowingEmptyContextResult, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );
	SpecificationItem *firstSelfGeneratedCheckSpecificationItem( bool isExclusiveSpecification, bool isNegative, bool isPossessive, bool isSelfGenerated, unsigned short questionParameter, unsigned short specificationWordTypeNr, unsigned int specificationCollectionNr, WordItem *specificationWordItem );

	SpecificationItem *firstSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion );
	SpecificationItem *firstSpecificationItem( bool isIncludingAnsweredQuestions, bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, unsigned short questionParameter );

	SpecificationItem *firstSpecificationItem( bool isPossessive, bool isSpecificationGeneralization, unsigned short questionParameter, WordItem *specificationWordItem );

	SpecificationItem *firstUnhiddenSpecificationItem( unsigned int relationContextNr );
	SpecificationItem *firstUserSpecificationItem( bool isNegative, bool isPossessive, unsigned int specificationCollectionNr, unsigned int relationContextNr, WordItem *specificationWordItem );

	SpecificationItem *sameUserQuestionSpecificationItem( unsigned short questionParameter );

	WordItem *feminineOrMasculineCommonWordItem( bool isFeminineWord );

	CollectionResultType collectSpecifications( unsigned short collectionWordTypeNr, unsigned short commonWordTypeNr, WordItem *generalizationWordItem, WordItem *collectionWordItem );

	CreateAndAssignResultType addSpecificationInWord( bool isAssignment, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString, char *authorizationKey );
	CreateAndAssignResultType createSpecificationItem( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isAnsweredQuestion, bool isConcludedAssumption, bool isConditional, bool isCorrectedAssumption, bool isEveryGeneralization, bool isExclusiveGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assignmentLevel, unsigned short assumptionLevel, unsigned short languageNr, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *specificationWordItem, char *specificationString, char *writtenSentenceString );

	RelatedResultType findRelatedSpecification( bool isCheckingRelationContext, SpecificationItem *searchSpecificationItem );
	RelatedResultType findRelatedSpecification( bool isIgnoringNegative, bool isIncludingAssignments, bool isIncludingArchivedAssignments, bool isExclusiveSpecification, bool isNegative, bool isPossessive, unsigned short questionParameter, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, WordItem *specificationWordItem );


	// Protected word type functions

	void clearGeneralizationWriteLevel( unsigned short currentWriteLevel );
	void clearSpecificationWriteLevel( unsigned short currentWriteLevel );
	void clearRelationWriteLevel( unsigned short currentWriteLevel, unsigned int contextNr );

	bool hasAnyWordType();

	bool hasFeminineAndMasculineArticle( unsigned short articleParameter );
	bool hasFeminineProperNameEnding();
	bool hasFeminineOrMasculineProperNameEnding();
	bool hasMasculineProperNameEnding();

	bool hasWordType( bool isAllowingDifferentNoun, unsigned short wordTypeNr );

	bool isGeneralizationWordTypeAlreadyWritten( unsigned short generalizationWordTypeNr );
	bool isSpecificationWordTypeAlreadyWritten( unsigned short specificationWordTypeNr );
	bool isRelationWordTypeAlreadyWritten( unsigned short relationWordTypeNr );

	bool isCorrectAdjective( unsigned short adjectiveParameter, unsigned short wordTypeNr );
	bool isCorrectDefiniteArticle( unsigned short definiteArticleParameter, unsigned short wordTypeNr );
	bool isCorrectIndefiniteArticle( unsigned short indefiniteArticleParameter, unsigned short wordTypeNr );
	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *authorizationKey );
	bool isNumeralWordType();
	bool isProperNamePrecededByDefiniteArticle( unsigned short definiteArticleParameter );

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

	WordTypeResultType addWordType( bool isMultipleWord, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString );
	WordResultType findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString );


	// Protected write functions

	void deleteTemporaryWriteList();

	signed char createWriteWord( bool isSkipped, unsigned short grammarLevel, GrammarItem *selectedGrammarItem );
	signed char writeJustificationSpecification( bool isWritingCurrentSpecificationWordOnly, SpecificationItem *justificationSpecificationItem );
	signed char writeSelectedRelationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem );
	signed char writeSelectedSpecification( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, SpecificationItem *writeSpecificationItem );
	signed char writeSelectedSpecification( bool isAdjustedAssumption, bool isCheckingUserSentenceForIntegrity, bool isForcingResponseNotBeingAssignment, bool isForcingResponseNotBeingFirstSpecification, bool isWritingCurrentSentenceOnly, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, SpecificationItem *writeSpecificationItem );
	signed char writeSelectedSpecificationInfo( bool isAssignment, bool isInactiveAssignment, bool isArchivedAssignment, bool isQuestion, WordItem *writeWordItem );
	signed char writeSpecificationSentence( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem );
	signed char writeUpdatedSpecification( bool isAdjustedSpecification, bool isConcludedSpanishAmbiguousAssumption, bool isCorrectedAssumptionByKnowledge, bool isCorrectedAssumptionByOppositeQuestion, bool isReplacedBySpecificationWithRelation, SpecificationItem *writeSpecificationItem );

	WriteItem *firstActiveWriteItem();
	};
#endif

/*************************************************************************
 *	"Those who look to him for help will be radiant with joy;
 *	no shadow of shame wil darken their faces." (Psalm 34:5)
 *************************************************************************/
