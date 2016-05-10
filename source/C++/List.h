/*	Class:		List
 *	Purpose:	Base class to store the items of the knowledge structure
 *	Version:	Thinknowlogy 2016r1 (Huguenot)
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

// List header

#ifndef LIST
#define LIST 1

#include <stdlib.h>
#include "Item.h"

// Some compilers need these class declarations
class ListCleanup;
class ListQuery;

class List
	{
	friend class AdminCleanup;
	friend class AdminQuery;
	friend class AdminReadSentence;
	friend class Item;
	friend class ListCleanup;
	friend class ListQuery;
	friend class SpecificationItem;
	friend class WordCleanup;
	friend class WordItem;

	// Private constructible variables

	char listChar_;

	Item *activeList_;
	Item *inactiveList_;
	Item *archivedList_;
	Item *replacedList_;
	Item *deletedList_;

	Item *nextListItem_;

	ListCleanup *listCleanup_;
	ListQuery *listQuery_;

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;

	char classNameString_[FUNCTION_NAME_LENGTH];
	char superClassNameString_[FUNCTION_NAME_LENGTH];


	private:
	// Private functions

	bool isAssignmentOrSpecificationList();
	bool isIncludingThisList( char *queryListString );

	ResultType removeItemFromList( Item *removeItem );

	Item *tailOfList( Item *searchItem );


	protected:
	// Constructor / deconstructor

	List();
	virtual ~List();


	// Protected error functions

	ResultType addError( const char *functionNameString, const char *moduleNameString, const char *errorString );

	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString, unsigned int sentenceNr );
	ResultType startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 );

	ResultType startSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString );


	// Protected virtual functions

	virtual bool isTemporaryList();

	virtual ReferenceResultType findWordReference( WordItem *referenceWordItem );


	// Protected common functions

	void initializeListVariables( char listChar, const char *classNameString, CommonVariables *commonVariables, WordItem *myWordItem );
	void deleteTemporaryList();

	bool hasItems();

	bool isAdminList();
	bool isAssignmentList();

	char listChar();

	ResultType addItemToList( char statusChar, Item *newItem );

	ResultType activateItem( Item *activateItem );
	ResultType inactivateItem( Item *inactivateItem );
	ResultType archiveItem( Item *archiveItem );
	ResultType replaceItem( Item *replaceItem );
	ResultType deleteItem( Item *deleteItem );

	ResultType deleteActiveItemsWithCurrentSentenceNr();

	ResultType removeFirstRangeOfDeletedItemsInList();

	Item *firstActiveItem();
	Item *firstInactiveItem();
	Item *firstArchivedItem();
	Item *firstReplacedItem();
	Item *firstDeletedItem();

	Item *nextListItem();

	CommonVariables *commonVariables();

	WordItem *myWordItem();


	// Protected cleanup functions

	void clearReplacedInfoInList();
	void getHighestInUseSentenceNrInList( bool isIncludingDeletedItems, unsigned int highestSentenceNr );
	void setCurrentItemNrInList();

	ResultType deleteSentencesInList( unsigned int lowestSentenceNr );

	ResultType decrementSentenceNrsInList( unsigned int startSentenceNr );
	ResultType decrementItemNrRangeInList( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset );

	ResultType undoCurrentSentenceInList();
	ResultType redoCurrentSentenceInList();


	// Protected query functions

	void countQueryInList();
	void clearQuerySelectionsInList();

	ReferenceResultType compareStrings( char *searchString, char *sourceString );

	ResultType itemQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr );
	ResultType listQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryListString );
	ResultType wordTypeQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned short queryWordTypeNr );
	ResultType parameterQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, unsigned int queryParameter );
	ResultType wordQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems );
	ResultType wordReferenceQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSelectingAttachedJustifications, bool isSelectingJustificationSpecifications, char *wordReferenceNameString );
	ResultType stringQueryInList( bool isSelectingOnFind, bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, char *queryString );
	ResultType showQueryResultInList( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth );
	};
#endif

/*************************************************************************
 *	"I prayed to the Lord, and he answered me.
 *	He freed me from all my fears." (Psalm 34:4)
 *************************************************************************/
