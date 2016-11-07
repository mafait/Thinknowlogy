/*	Class:		Item
 *	Purpose:	Base class for the knowledge structure
 *	Version:	Thinknowlogy 2016r2 (Restyle)
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

// Item header

#ifndef ITEM
#define ITEM 1

#include <limits.h>
#include <stdio.h>
#include "CommonVariables.cpp"
#include "StringResultType.cpp"

// Some compilers need these class declarations
class CollectionResultType;
class ContextResultType;
class FileResultType;
class GrammarResultType;
class JustificationResultType;
class List;
class ReadResultType;
class SelectionResultType;
class SpecificationResultType;
class WordResultType;
class WriteResultType;

class Item
	{
	friend class List;
	friend class ListCleanup;
	friend class ListQuery;
	friend class SelectionList;

	// Private constructed variables

	unsigned short userNr_;

	unsigned int originalSentenceNr_;
	unsigned int creationSentenceNr_;

	unsigned int activeSentenceNr_;
	unsigned int inactiveSentenceNr_;
	unsigned int archivedSentenceNr_;
	unsigned int replacedSentenceNr_;
	unsigned int deletedSentenceNr_;

	unsigned int itemNr_;

	char statusChar_;

	char classNameString_[FUNCTION_NAME_LENGTH];
	char superClassNameString_[FUNCTION_NAME_LENGTH];


	// Private initialized variables

	CommonVariables *commonVariables_;
	List *myList_;
	WordItem *myWordItem_;


	// Private functions

	char *myWordTypeString( unsigned short queryWordTypeNr );


	protected:
	// Protected constructed variables

	bool isSelectedByQuery;
	bool isSelectedByJustificationQuery;

	char previousStatusChar;

	Item *previousItem;
	Item *nextItem;

	char statusString[2];
	char tempString[MAX_SENTENCE_STRING_LENGTH];


	// Constructor

	Item();
	virtual ~Item() {};

	// Protected error functions

	CollectionResultType addCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CollectionResultType addCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	CollectionResultType addCollectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	CollectionResultType startCollectionResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CollectionResultType startCollectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	CollectionResultType startCollectionResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	CollectionResultType startCollectionResultSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	ContextResultType addContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ContextResultType addContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ContextResultType startContextResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ContextResultType startContextResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	FileResultType addFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	FileResultType startFileResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	GrammarResultType startGrammarResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	JustificationResultType startJustificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	ReadResultType addReadResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ReadResultType addReadResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ReadResultType startReadResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ReadResultType startReadResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	ResultType addError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType addError( const char *functionNameString, const char *moduleNameString, char *wordItemString, const char *errorString );

	ResultType addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 );
	ResultType addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 );
	ResultType addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ResultType addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number1 );
	ResultType addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );

	ResultType addError( char listChar, const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType startError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2, const char *errorString3, unsigned int number3 );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2 );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2, char char2, const char *errorString3 );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number1, const char *errorString4, unsigned int number2, const char *errorString5, unsigned int number3 );

	ResultType startSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType startSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	SelectionResultType addSelectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	SelectionResultType startSelectionResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	SpecificationResultType addSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	SpecificationResultType addSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	SpecificationResultType addSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	SpecificationResultType addSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	SpecificationResultType startSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	SpecificationResultType startSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 );
	SpecificationResultType startSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	SpecificationResultType startSpecificationResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 );
	SpecificationResultType startSpecificationResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	SpecificationResultType startSpecificationResultSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	SpecificationResultType startSpecificationResultSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	StringResultType addStringResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	StringResultType addStringResultError( const char *functionNameString, const char *moduleNameString, char *wordItemString, const char *errorString );
	StringResultType startStringResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	WordResultType addWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	WordResultType addWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	WordResultType addWordResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	WordResultType startWordResultError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	WordResultType startWordResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );

	WriteResultType addWriteResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );
	WriteResultType startWriteResultError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString );


	// Protected virtual functions

	virtual void clearReplacingInfo();

	virtual void selectingAttachedJustifications( bool isSelectingJustificationSpecifications );
	virtual void selectingJustificationSpecifications();

	virtual void displayString( bool isReturnQueryToPosition );
	virtual void displayWordReferences( bool isReturnQueryToPosition );

	virtual bool hasParameter( unsigned int queryParameter );
	virtual bool hasReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr );
	virtual bool hasWordType( unsigned short queryWordTypeNr );

	virtual bool isSorted( Item *nextSortItem );

	virtual ResultType checkForUsage();

	virtual StringResultType findMatchingWordReferenceString( char *queryString );

	virtual char *itemString();
	virtual char *virtualGuideByGrammarString();
	virtual char *toString( unsigned short queryWordTypeNr );


	// Protected common functions

	void clearArchivedSentenceNr();
	void clearReplacedSentenceNr();

	void setActiveStatus();
	void setArchivedStatus();
	void setDeletedStatus();
	void setInactiveStatus();
	void setReplacedStatus();

	void setActiveSentenceNr();
	void setArchivedSentenceNr();
	void setDeletedSentenceNr();
	void setInactiveSentenceNr();
	void setReplacedSentenceNr();

	void displayWords( bool isReturnQueryToPosition, unsigned short queryWordTypeNr );

	// Strictly for initialization of AdminItem
	void initializeItemVariables( const char *classNameString, CommonVariables *commonVariables, WordItem *myWordItem );
	void initializeItemVariables( unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, const char *classNameString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem );

	bool hasActiveSentenceNr();
	bool hasInactiveSentenceNr();

	bool hasCurrentCreationSentenceNr();
	bool hasCurrentActiveSentenceNr();
	bool hasCurrentInactiveSentenceNr();
	bool hasCurrentArchivedSentenceNr();
	bool hasCurrentReplacedSentenceNr();

	bool hasSentenceNr( unsigned int sentenceNr );

	bool isOlderItem();

	bool isActiveItem();
	bool isInactiveItem();
	bool isArchivedItem();
	bool isReplacedItem();
	bool isDeletedItem();
	bool isReplacedOrDeletedItem();

	bool isMoreRecent( Item *checkItem );

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
	unsigned int deletedSentenceNr();

	unsigned int itemNr();

	ResultType decrementActiveSentenceNr();
	ResultType decrementInactiveSentenceNr();
	ResultType decrementOriginalSentenceNr();
	ResultType decrementCreationSentenceNr();
	ResultType decrementArchivedSentenceNr();
	ResultType decrementReplacedSentenceNr();

	ResultType decrementItemNr( unsigned int decrementOffset );

	char statusChar();

	char *classNameString();
	char *superClassNameString();

	CommonVariables *commonVariables();

	List *myList();

	Item *tailOfList();
	WordItem *myWordItem();


	// Protected definition functions

	bool doesStringStartWithPhoneticVowel( char *textString );

	bool isAssumption( unsigned short justificationTypeNr );
	bool isConclusion( unsigned short justificationTypeNr );

	bool isAdjectiveParameter( unsigned short checkParameter );
	bool isDefiniteArticleParameter( unsigned short checkParameter );
	bool isIndefiniteArticleParameter( unsigned short checkParameter );

	bool isFeminineArticleParameter( unsigned short articleParameter );
	bool isMasculineArticleParameter( unsigned short articleParameter );

	bool isGeneralizationReasoningWordType( unsigned short wordTypeNr );
	bool isMatchingWordType( unsigned short firstWordTypeNr, unsigned short secondWordTypeNr );
	bool isNounWordType( unsigned short wordTypeNr );

	unsigned short assumptionGrade( bool hasAnotherPrimarySpecification, bool hasFeminineOrMasculineProperNameEnding, bool hasPossessivePrimarySpecification, bool hasPrimaryQuestionSpecification, unsigned short justificationTypeNr );
	};
#endif

/*************************************************************************
 *	"I will boast only in the Lord;
 *	let all who are helpless take heart." (Psalm 34:2)
 *************************************************************************/
