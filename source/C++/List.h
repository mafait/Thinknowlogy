/*	Class:		List
 *	Purpose:	Base class to store the items of the knowledge structure
 *	Version:	Thinknowlogy 2017r2 (Science as it should be)
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

// List header

#ifndef LIST
#define LIST 1

#include <stdlib.h>
#include "Item.h"

// Class declarations
class WordEndingResultType;

class List
	{
	friend class AdminItem;
	friend class Item;
	friend class SpecificationItem;
	friend class WordItem;

	// Private constructed variables

	unsigned int highestSentenceNrInList_;

	Item *activeList_;
	Item *inactiveList_;
	Item *archivedList_;
	Item *replacedList_;
	Item *deletedList_;

	Item *nextListItem_;

	// Private initialized variables

	char listChar_;
	char classNameString_[FUNCTION_NAME_LENGTH];
	char parentClassNameString_[FUNCTION_NAME_LENGTH];

	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;
	WordItem *myWordItem_;


	private:
	// Private functions

	bool isIncludingThisList( char *queryListString );

	// Private cleanup functions

	unsigned int highestCurrentSentenceItemNr( unsigned int currentSentenceNr, Item *searchItem );
	unsigned int highestFoundSentenceNrInList( unsigned int highestSentenceNr, Item *searchItem );

	signed char decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset, Item *searchItem );
	signed char decrementSentenceNrs( unsigned int startSentenceNr, Item *searchItem );
	signed char removeItemFromList( Item *removeItem );

	// Private database connection functions

//	signed char storeChangesInFutureDatabase( Item *searchItem );

	// Private query functions

	void clearQuerySelections( Item *searchItem );
	void itemQuery( bool isSelectingOnFind, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, Item *queryItem );
	void listQuery( bool isSelectingOnFind, Item *queryItem );
	void parameterQuery( bool isSelectingOnFind, unsigned int queryParameter, Item *queryItem );
	void wordQuery( bool isSelectingOnFind, Item *queryItem );
	void wordTypeQuery( bool isSelectingOnFind, unsigned short queryWordTypeNr, Item *queryItem );

	signed char displayQueryResult( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, Item *queryItem );
	signed char stringQuery( bool isSelectingOnFind, char *wordString, Item *queryItem );
	signed char wordReferenceQuery( bool isSelectingOnFind, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString, Item *queryItem );


	protected:
	// Constructor

	List();
	virtual ~List();


	// Protected error functions

	signed char addError( const char *functionNameString, const char *errorString );
	signed char addError( const char *functionNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char startError( const char *functionNameString, const char *errorString );
	signed char startError( const char *functionNameString, const char *errorString, unsigned int sentenceNr );
	signed char startError( const char *functionNameString, const char *errorString1, const char *errorString2, const char *errorString3 );
	signed char startSystemError( const char *functionNameString, const char *errorString );

	BoolResultType addBoolResultError( const char *functionNameString, const char *errorString );
	BoolResultType startBoolResultError( const char *functionNameString, const char *errorString );

	CollectionResultType addCollectionResultError( const char *functionNameString, const char *errorString );
	CollectionResultType startCollectionResultError( const char *functionNameString, const char *errorString );
	CollectionResultType startCollectionResultError( const char *functionNameString, const char *errorString, unsigned int number );
	CollectionResultType startCollectionResultSystemError( const char *functionNameString, const char *errorString );

	CreateAndAssignResultType addCreateAndAssignResultError( const char *functionNameString, const char *errorString );
	CreateAndAssignResultType startCreateAndAssignResultError( const char *functionNameString, const char *errorString );

	DuplicateResultType addDuplicateResultError( const char *functionNameString, const char *errorString );
	DuplicateResultType startDuplicateResultError( const char *functionNameString, const char *errorString );

	FileResultType addFileResultError( const char *functionNameString, const char *errorString );
	FileResultType startFileResultError( const char *functionNameString, const char *errorString );
	FileResultType startFileResultError( const char *functionNameString, const char *errorString1, const char *errorString2, const char *errorString3 );

	GrammarResultType addGrammarResultError( const char *functionNameString, const char *errorString );
	GrammarResultType startGrammarResultError( const char *functionNameString, const char *errorString );

	JustificationResultType addJustificationResultError( const char *functionNameString, const char *errorString );
	JustificationResultType startJustificationResultError( const char *functionNameString, const char *errorString );

	SelectionResultType addSelectionResultError( const char *functionNameString, const char *errorString );
	SelectionResultType startSelectionResultSystemError( const char *functionNameString, const char *errorString );

	ShortResultType startShortResultError( const char *functionNameString, const char *errorString );

	WordEndingResultType addWordEndingResultError( const char *functionNameString, const char *errorString );
	WordEndingResultType startWordEndingResultError( const char *functionNameString, const char *errorString );

	WordResultType addWordResultError( const char *functionNameString, const char *errorString );
	WordResultType startWordResultError( const char *functionNameString, const char *errorString );

	WordTypeResultType addWordTypeResultError( const char *functionNameString, const char *errorString );
	WordTypeResultType startWordTypeResultError( const char *functionNameString, const char *errorString );


	// Protected virtual functions

	virtual bool isTemporaryList();


	// Protected common functions

	void initializeListVariables( char listChar, const char *classNameString, CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *myWordItem );
	void deleteTemporaryList();

	bool hasActiveItems();

	bool isAdminList();
	bool isAssignmentList();

	unsigned int highestSentenceNrInList();

	char listChar();

	signed char addItemToList( char statusChar, Item *newItem );
	signed char activateItem( Item *activateItem );
	signed char inactivateItem( Item *inactivateItem );
	signed char archiveItem( Item *archiveItem );
	signed char replaceItem( Item *replaceItem );
	signed char deleteItem( Item *deleteItem );
	signed char deleteActiveItemsWithCurrentSentenceNr();
	signed char removeFirstRangeOfDeletedItemsInList();

	BoolResultType compareStrings( char *searchString, char *sourceString );

	Item *firstActiveItem();
	Item *firstInactiveItem();
	Item *firstArchivedItem();
	Item *firstReplacedItem();
	Item *firstDeletedItem();

	Item *nextListItem();

	CommonVariables *commonVariables();

	InputOutput *inputOutput();

	WordItem *myWordItem();


	// Protected cleanup functions

	unsigned int highestCurrentSentenceItemNrInList( unsigned int currentSentenceNr, unsigned int currentSentenceItemNr );
	unsigned int highestFoundSentenceNrInList( bool isIncludingDeletedItems, unsigned int highestFoundSentenceNr, unsigned int maxSentenceNr );

	signed char decrementSentenceNrsInList( unsigned int startSentenceNr );
	signed char decrementItemNrRangeInList( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset );
	signed char deleteSentencesInList( unsigned int lowestSentenceNr );
	signed char undoCurrentSentenceInList();
	signed char redoCurrentSentenceInList();


	// Protected database connection functions

//	signed char storeChangesInFutureDatabaseInList();


	// Protected query functions

	void clearQuerySelectionsInList();
	void countQueryInList();

	void itemQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr );
	void listQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString );
	void parameterQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter );
	void wordQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems );
	void wordTypeQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr );

	unsigned int highestFoundSentenceNrInList( bool isIncludingDeletedItems, unsigned int maxSentenceNr );

	signed char displayQueryResultInList( bool isOnlyDisplayingWords, bool isOnlyDisplayingWordReferences, bool isOnlyDisplayingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth );
	signed char stringQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString );
	signed char wordReferenceQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString );
	};
#endif

/*************************************************************************
 *	"I prayed to the Lord, and he answered me.
 *	He freed me from all my fears." (Psalm 34:4)
 *************************************************************************/
