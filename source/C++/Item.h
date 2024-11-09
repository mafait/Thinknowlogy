/*	Class:		Item
 *	Purpose:	Base class for the knowledge structure
 *	Version:	Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
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

// Item header

#ifndef ITEM
#define ITEM 1

#include <limits.h>
#include <stdio.h>
#include "BoolResultType.cpp"
#include "CollectionResultType.cpp"
#include "GlobalVariables.cpp"
#include "QueryResultType.cpp"
#include "WordTypeResultType.cpp"

// Class declarations
class CompoundResultType;
class ConditionResultType;
class ContextResultType;
class CreateAndAssignResultType;
class CreateReadWordResultType;
class DuplicateResultType;
class FileResultType;
class FindSpecificationResultType;
class GrammarResultType;
class InputOutput;
class JustificationResultType;
class List;
class ReadWordResultType;
class RelatedResultType;
class SelectionResultType;
class ShortResultType;
class SpecificationItem;
class SpecificationResultType;
class UserSpecificationResultType;
class WordEndingResultType;
class WordResultType;
class WordTypeResultType;

class Item
	{
	friend class List;
	friend class SelectionList;

	// Private constructed variables

	unsigned short userNr_ = NO_USER_NR;

	unsigned int originalSentenceNr_ = NO_SENTENCE_NR;
	unsigned int creationSentenceNr_ = NO_SENTENCE_NR;

	unsigned int activeSentenceNr_ = NO_SENTENCE_NR;
	unsigned int inactiveSentenceNr_ = NO_SENTENCE_NR;
	unsigned int archivedSentenceNr_ = NO_SENTENCE_NR;
	unsigned int replacedSentenceNr_ = NO_SENTENCE_NR;

	unsigned int itemNr_ = NO_ITEM_NR;

	char statusChar_ = QUERY_ACTIVE_CHAR;

	char parentClassNameString_[FUNCTION_NAME_STRING_LENGTH] = "Item";

	// Private initialized variables

	char classNameString_[FUNCTION_NAME_STRING_LENGTH] = EMPTY_STRING;

	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;
	List *myList_ = NULL;
	WordItem *myWordItem_ = NULL;


	// Private functions

	unsigned short justificationAssumptionGrade( bool hasFeminineOrMasculineProperNounEnding, bool hasAdditionalDefinitionSpecification, bool isExclusivePrimarySpecification, bool isNegativePrimarySpecification, bool isNegativeSecondarySpecification, unsigned short justificationTypeNr );

	char *myWordTypeString( unsigned short queryWordTypeNr );


	protected:
	// Protected constructed variables

	bool isSelectedByQuery = false;
	bool isSelectedByJustificationQuery = false;

	char previousStatusChar = QUERY_ACTIVE_CHAR;

	Item *previousItem = NULL;
	Item *nextItem = NULL;

	char statusString[2] = SPACE_STRING;
	char tempString[EXTENDED_SENTENCE_STRING_LENGTH] = EMPTY_STRING;


	// Constructor

	Item();
	virtual ~Item() {};

	// Protected error functions

	signed char addError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char addError( const char *functionNameString, const char *moduleNameString, char *wordItemString, const char *errorString );
	signed char addError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 );
	signed char addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number );
	signed char addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	signed char addError( char listChar, const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char startError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2, const char *errorString3, unsigned int number3 );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2, char char2, const char *errorString3 );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	signed char startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number1, const char *errorString4, unsigned int number2, const char *errorString5, unsigned int number3 );
	signed char startSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	signed char startSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	BoolResultType addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	BoolResultType addBoolResultError( const char *functionNameString, const char *moduleNameString, char *wordItemString, const char *errorString );
	BoolResultType addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	BoolResultType addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	BoolResultType addBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	BoolResultType startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	BoolResultType startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );
	BoolResultType startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 );
	BoolResultType startBoolResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	BoolResultType startBoolResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	CollectionResultType addCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CollectionResultType addCollectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	CollectionResultType addCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CollectionResultType addCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CollectionResultType startCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CollectionResultType startCollectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	CompoundResultType addCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CompoundResultType addCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CompoundResultType addCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CompoundResultType startCompoundResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	ConditionResultType addConditionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ConditionResultType startConditionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	ContextResultType addContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ContextResultType addContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ContextResultType startContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ContextResultType startContextResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	CreateAndAssignResultType addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CreateAndAssignResultType addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CreateAndAssignResultType addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CreateAndAssignResultType addCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	CreateAndAssignResultType startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CreateAndAssignResultType startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	CreateAndAssignResultType startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CreateAndAssignResultType startCreateAndAssignResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	CreateAndAssignResultType startCreateAndAssignResultSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	CreateReadWordResultType addCreateReadWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CreateReadWordResultType startCreateReadWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	DuplicateResultType startDuplicateResultError( const char *functionNameString, const char *errorString );

	FileResultType addFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	FileResultType startFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	FindSpecificationResultType startFindSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	GrammarResultType startGrammarResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	JustificationResultType startJustificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	QueryResultType addQueryResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	QueryResultType startQueryResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	QueryResultType startQueryResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2 );

	ReadWordResultType startReadWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	RelatedResultType addRelatedResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	RelatedResultType startRelatedResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	SelectionResultType startSelectionResultError( const char *functionNameString, const char *errorString );

	ShortResultType startShortResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ShortResultType startShortResultError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int number );

	SpecificationResultType addSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	SpecificationResultType startSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	UserSpecificationResultType addUserSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	UserSpecificationResultType startUserSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	WordEndingResultType startWordEndingResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	WordResultType addWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	WordResultType addWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	WordResultType startWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	WordTypeResultType startWordTypeResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );


	// Protected virtual functions

	virtual void checkForUsage();
	virtual void displayString( bool isReturnQueryToPosition );
	virtual void displayWordReferences( bool isReturnQueryToPosition );
	virtual void selectingAttachedJustifications( bool isSelectingJustificationSpecifications );
	virtual void selectingJustificationSpecifications();

	virtual bool hasParameter( unsigned int queryParameter );
	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr );
	virtual bool hasWordType( unsigned short queryWordTypeNr );

	virtual bool isSorted( Item *nextSortItem );

	virtual char *itemString();
	virtual char *itemToString( unsigned short queryWordTypeNr );

	virtual BoolResultType findMatchingWordReferenceString( char *queryString );


	// Protected common functions

	void clearArchivedSentenceNr();
	void clearReplacedSentenceNr();

	void setActiveStatus();
	void setArchivedStatus();
	void setInactiveStatus();
	void setReplacedStatus();
	void setDeletedStatus();

	void setActiveSentenceNr();
	void setArchivedSentenceNr();
	void setInactiveSentenceNr();
	void setReplacedSentenceNr();

	void displayWords( bool isReturnQueryToPosition, unsigned short queryWordTypeNr );

	// Strictly for initialization of AdminItem
	void initializeItemVariables( const char *classNameString, GlobalVariables *globalVariables, WordItem *myWordItem );
	void initializeItemVariables( unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, const char *classNameString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem );

	bool hasActiveSentenceNr();
	bool hasInactiveSentenceNr();

	bool hasCurrentCreationSentenceNr();
	bool hasCurrentOrNewerCreationSentenceNr();
	bool hasCurrentActiveSentenceNr();
	bool hasCurrentInactiveSentenceNr();
	bool hasCurrentArchivedSentenceNr();
	bool hasCurrentReplacedSentenceNr();

	bool hasSentenceNr( unsigned int sentenceNr );
	bool hasUserNr();

	bool isOlderItem();

	bool isActiveItem();
	bool isInactiveItem();
	bool isArchivedItem();
	bool isReplacedItem();
	bool isDeletedItem();
	bool isReplacedOrDeletedItem();

	bool wasActiveBefore();
	bool wasInactiveBefore();
	bool wasArchivedBefore();

	unsigned short userNr();

	unsigned int activeSentenceNr();
	unsigned int inactiveSentenceNr();
	unsigned int originalSentenceNr();
	unsigned int creationSentenceNr();

	unsigned int archivedSentenceNr();
	unsigned int replacedSentenceNr();

	unsigned int itemNr();

	signed char decrementActiveSentenceNr();
	signed char decrementInactiveSentenceNr();
	signed char decrementOriginalSentenceNr();
	signed char decrementCreationSentenceNr();
	signed char decrementArchivedSentenceNr();
	signed char decrementReplacedSentenceNr();
	signed char decrementItemNr( unsigned int decrementOffset );

	char statusChar();

	char *classNameString();
	char *itemBaseToString( unsigned short queryWordTypeNr );
	char *parentClassNameString();

	GlobalVariables *globalVariables();

	InputOutput *inputOutput();

	List *myList();

	Item *tailOfList();
	WordItem *myWordItem();


	// Protected database connection functions

//	signed char storeChangesInFutureDatabase();


	// Protected definition functions

	bool doesStringStartWithPhoneticVowel( char *textString );

	bool isAdjectiveParameter( unsigned short checkParameter );
	bool isDefiniteArticleParameter( unsigned short checkParameter );
	bool isIndefiniteArticleParameter( unsigned short checkParameter );

	bool isFeminineArticleParameter( unsigned short articleParameter );
	bool isMasculineArticleParameter( unsigned short articleParameter );

	bool isGeneralizationReasoningWordType( unsigned short wordTypeNr );
	bool isSpecificationReasoningWordType( unsigned short wordTypeNr );

	bool isMatchingWordType( unsigned short firstWordTypeNr, unsigned short secondWordTypeNr );
	bool isNounWordType( unsigned short wordTypeNr );

	unsigned short justificationAssumptionLevel( bool hasFeminineOrMasculineProperNounEnding, unsigned short justificationTypeNr, SpecificationItem *primarySpecificationItem, SpecificationItem *additionalDefinitionSpecificationItem, SpecificationItem *secondarySpecificationItem, SpecificationItem *additionalProperNounSpecificationItem );
	};
#endif

/*************************************************************************
 *	"I will boast only in the Lord;
 *	let all who are helpless take heart." (Psalm 34:2)
 *************************************************************************/
