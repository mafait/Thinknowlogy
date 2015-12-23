/*
 *	Class:		Item
 *	Purpose:	Base class for the knowledge structure
 *	Version:	Thinknowlogy 2015r1 (Esperanza)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait. Your suggestions, modifications
 *	and bug reports are welcome at http://mafait.org
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
#include "ReferenceResultType.cpp"

// Some compilers need these class declarations
class List;

class Item
	{
	friend class List;
	friend class ListCleanup;
	friend class ListQuery;
	friend class SelectionList;
	friend class SpecificationItem;
	friend class WordItem;


	// Private constructible variables

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


	// Private loadable variables

	CommonVariables *commonVariables_;
	List *myList_;
	WordItem *myWordItem_;


	// Private functions

	char *myWordTypeString( unsigned short queryWordTypeNr );


	protected:
	// Protected constructible variables

	bool isAvailableForRollbackAfterDelete;
	bool isSelectedByQuery;
	bool isSelectedByJustificationQuery;

	char previousStatusChar;

	Item *previousItem;
	Item *nextItem;

	char tempString[MAX_SENTENCE_STRING_LENGTH];


	// Constructor / deconstructor

	Item();
	virtual ~Item() {};

	// Protected error functions

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


	// Protected virtual functions

	virtual void selectingAttachedJustifications( bool isSelectingJustificationSpecifications );
	virtual void selectingJustificationSpecifications();

	virtual void showString( bool isReturnQueryToPosition );
	virtual void showWordReferences( bool isReturnQueryToPosition );
	virtual void showWords( bool isReturnQueryToPosition, unsigned short queryWordTypeNr );

	virtual bool hasFoundParameter( unsigned int queryParameter );
	virtual bool hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr );
	virtual bool hasFoundWordType( unsigned short queryWordTypeNr );

	virtual bool isSorted( Item *nextSortItem );

	virtual ReferenceResultType findMatchingWordReferenceString( char *queryString );

	virtual ResultType checkForUsage();

	virtual char *itemString();
	virtual char *extraItemString();
	virtual char *toString( unsigned short queryWordTypeNr );


	// Protected common functions

	void setActiveStatus();
	void setInactiveStatus();
	void setArchivedStatus();
	void setReplacedStatus();
	void setDeletedStatus();

	void setActiveSentenceNr();
	void setInactiveSentenceNr();
	void setArchivedSentenceNr();
	void setReplacedSentenceNr();
	void setDeletedSentenceNr();

	void clearArchivedSentenceNr();
	void clearReplacedSentenceNr();

	// Strictly for initialization of AdminItem
	void initializeItemVariables( const char *classNameString, CommonVariables *commonVariables, WordItem *myWordItem );
	void initializeItemVariables( unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, const char *classNameString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem );

	bool hasActiveSentenceNr();
	bool hasInactiveSentenceNr();
	bool hasArchivedSentenceNr();

	bool hasCurrentCreationSentenceNr();

	bool hasCurrentActiveSentenceNr();
	bool hasCurrentInactiveSentenceNr();
	bool hasCurrentArchivedSentenceNr();
	bool hasCurrentReplacedSentenceNr();
	bool hasCurrentDeletedSentenceNr();

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

	List *myList();

	WordItem *myWordItem();

	CommonVariables *commonVariables();


	// Protected definition functions

	bool isAssumption( unsigned short justificationTypeNr );
	bool isConclusion( unsigned short justificationTypeNr );

	bool isAdjectiveParameter( unsigned short checkParameter );
	bool isDefiniteArticleParameter( unsigned short checkParameter );
	bool isIndefiniteArticleParameter( unsigned short checkParameter );

	bool isFeminineArticleParameter( unsigned short articleParameter );
	bool isMasculineArticleParameter( unsigned short articleParameter );

	bool isGeneralizationReasoningWordType( unsigned short wordTypeNr );

	bool isSingularOrPluralNoun( unsigned short wordTypeNr );
	bool isStartingWithPhoneticVowel( char *textString );

	bool isMatchingWordType( unsigned short firstWordTypeNr, unsigned short secondWordTypeNr );

	unsigned short assumptionGrade( bool hasAnotherPrimarySpecification, bool hasFeminineOrMasculineProperNameEnding, bool hasPossessivePrimarySpecification, bool hasPrimaryQuestionSpecification, unsigned short justificationTypeNr );
	};
#endif

/*************************************************************************
 *	"I will boast only in the Lord;
 *	let all who are helpless take heart." (Psalm 34:2)
 *************************************************************************/
