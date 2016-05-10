/*	Class:		Item
 *	Purpose:	Base class for the knowledge structure
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

#include "List.h"
#include "Presentation.cpp"

	// Private functions

	char *Item::myWordTypeString( unsigned short queryWordTypeNr )
		{
		char *wordTypeString = NULL;

		if( myList_ != NULL &&
		// Don't show my word string if the item is in an admin list
		!myList_->isAdminList() &&
		myWordItem_ != NULL &&
		( wordTypeString = myWordItem_->wordTypeString( true, queryWordTypeNr ) ) == NULL )
			wordTypeString = myWordItem_->anyWordTypeString();

		return wordTypeString;
		}


	// Constructor / deconstructor

	Item::Item()
		{
		// Private constructible variables

		userNr_ = NO_USER_NR;

		originalSentenceNr_ = NO_SENTENCE_NR;
		creationSentenceNr_ = NO_SENTENCE_NR;

		activeSentenceNr_ = NO_SENTENCE_NR;
		inactiveSentenceNr_ = NO_SENTENCE_NR;
		archivedSentenceNr_ = NO_SENTENCE_NR;
		replacedSentenceNr_ = NO_SENTENCE_NR;
		deletedSentenceNr_ = NO_SENTENCE_NR;

		itemNr_ = NO_ITEM_NR;

		statusChar_ = QUERY_ACTIVE_CHAR;

		// Private loadable variables

		myList_ = NULL;
		myWordItem_ = NULL;
		commonVariables_ = NULL;

		strcpy( superClassNameString_, "Item" );
		strcpy( classNameString_, EMPTY_STRING );

		// Protected constructible variables

		isSelectedByQuery = false;
		isSelectedByJustificationQuery = false;

		previousStatusChar = QUERY_ACTIVE_CHAR;

		nextItem = NULL;
		previousItem = NULL;

		strcpy( statusString, SPACE_STRING );
		strcpy( tempString, EMPTY_STRING );
		}


	// Protected error functions

	ResultType Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return addError( functionNameString, moduleNameString, ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), errorString );
		}

	ResultType Item::addError( const char *functionNameString, const char *moduleNameString, char *wordItemString, const char *errorString )
		{
		if( commonVariables_ != NULL &&
		commonVariables_->presentation != NULL )
			commonVariables_->presentation->showError( SYMBOL_QUESTION_MARK, ( moduleNameString == NULL ? classNameString_ : moduleNameString ), ( moduleNameString == NULL ? superClassNameString_ : NULL ), wordItemString, functionNameString, errorString );
		else
			fprintf( stderr, "\nClass:\t%s\nSubclass:\t%s\nFunction:\t%s\nWordItem:%s\nError:\t\t%s.\n", classNameString_, superClassNameString_, functionNameString, wordItemString, errorString );

		return ( commonVariables_ == NULL ? RESULT_ERROR : commonVariables_->result );
		}

	ResultType Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	ResultType Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 )
		{
		sprintf( tempString, "%s%u%s%u", errorString1, number1, errorString2, number2 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	ResultType Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	ResultType Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number1 )
		{
		sprintf( tempString, "%s%s%s%u", errorString1, errorString2, errorString3, number1 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	ResultType Item::addError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return addError( functionNameString, moduleNameString, NULL, tempString );
		}

	ResultType Item::addError( char listChar, const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		if( commonVariables_ != NULL &&
		commonVariables_->presentation != NULL )
			commonVariables_->presentation->showError( listChar, ( moduleNameString == NULL ? classNameString_ : moduleNameString ), ( moduleNameString == NULL ? superClassNameString_ : NULL ), wordNameString, functionNameString, errorString );
		else
			fprintf( stderr, "\nClass:\t%s\nSubclass:\t%s\nFunction:\t%s\nError:\t\t%s.\n", classNameString_, superClassNameString_, functionNameString, errorString );

		return ( commonVariables_ == NULL ? RESULT_ERROR : commonVariables_->result );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, NULL, errorString );

		if( commonVariables_ != NULL )
		commonVariables_->result = RESULT_ERROR;

		return RESULT_ERROR;
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		addError( functionNameString, moduleNameString, wordNameString, errorString );

		if( commonVariables_ != NULL )
		commonVariables_->result = RESULT_ERROR;

		return RESULT_ERROR;
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1 )
		{
		sprintf( tempString, "%s%u", errorString1, number1 );
		return startError( functionNameString, moduleNameString, NULL, tempString );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2 )
		{
		sprintf( tempString, "%s%u%s%u", errorString1, number1, errorString2, number2 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, unsigned int number1, const char *errorString2, unsigned int number2, const char *errorString3, unsigned int number3 )
		{
		sprintf( tempString, "%s%u%s%u%s%u", errorString1, number1, errorString2, number2, errorString3, number3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2 )
		{
		sprintf( tempString, "%s%c%s", errorString1, char1, errorString2 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, char char1, const char *errorString2, char char2, const char *errorString3 )
		{
		sprintf( tempString, "%s%c%s%c%s", errorString1, char1, errorString2, char2, errorString3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3 )
		{
		sprintf( tempString, "%s%s%s", errorString1, errorString2, errorString3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, const char *errorString4, const char *errorString5 )
		{
		sprintf( tempString, "%s%s%s%s%s", errorString1, errorString2, errorString3, errorString4, errorString5 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	ResultType Item::startError( const char *functionNameString, const char *moduleNameString, const char *errorString1, const char *errorString2, const char *errorString3, unsigned int number1, const char *errorString4, unsigned int number2, const char *errorString5, unsigned int number3 )
		{
		sprintf( tempString, "%s%s%s%u%s%u%s%u", errorString1, errorString2, errorString3, number1, errorString4, number2, errorString5, number3 );
		return startError( functionNameString, moduleNameString, tempString );
		}

	ResultType Item::startSystemError( const char *functionNameString, const char *moduleNameString, const char *errorString )
		{
		return startSystemError( functionNameString, moduleNameString, NULL, errorString );
		}

	ResultType Item::startSystemError( const char *functionNameString, const char *moduleNameString, char *wordNameString, const char *errorString )
		{
		char textChar;
		size_t tempStringPosition = 0;
		size_t errorStringPosition = 0;

		strcpy( tempString, EMPTY_STRING );

		while( errorStringPosition < strlen( errorString ) )
			{
			if( errorString[errorStringPosition] == SYMBOL_BACK_SLASH )
				{
				errorStringPosition++;

				if( errorStringPosition < strlen( errorString ) &&
				( textChar = commonVariables_->presentation->convertDiacriticalChar( errorString[errorStringPosition] ) ) != NEW_LINE_CHAR )
					tempString[tempStringPosition++] = textChar;
				}
			else
				tempString[tempStringPosition++] = errorString[errorStringPosition];

			errorStringPosition++;
			}

		addError( functionNameString, moduleNameString, wordNameString, tempString );

		if( commonVariables_ != NULL )
		commonVariables_->result = RESULT_SYSTEM_ERROR;

		return RESULT_SYSTEM_ERROR;
		}


	// Protected virtual functions

	void Item::clearReplacingItem()
		{
		// This is a virtual function. Therefore, it has no body.
		}

	void Item::selectingAttachedJustifications( bool isSelectingJustificationSpecifications )
		{
		// This is a virtual function. Therefore, it has no body, and the given variables are unreferenced.
		}

	void Item::selectingJustificationSpecifications()
		{
		// This is a virtual function. Therefore, it has no body.
		}

	void Item::showString( bool isReturnQueryToPosition )
		{
		// This is a virtual function. Therefore, it has no body, and the given variables are unreferenced.
		}

	void Item::showWordReferences( bool isReturnQueryToPosition )
		{
		// This is a virtual function. Therefore, it has no body, and the given variables are unreferenced.
		}

	bool Item::isSorted( Item *nextSortItem )
		{
		return ( nextSortItem != NULL &&
				// Descending creationSentenceNr_
				creationSentenceNr_ > nextSortItem->creationSentenceNr_ );
		}

	bool Item::hasFoundParameter( unsigned int queryParameter )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		return false;
		}

	bool Item::hasFoundWordType( unsigned short queryWordTypeNr )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		return false;
		}

	bool Item::hasFoundReferenceItemById( unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		return false;
		}

	ReferenceResultType Item::findMatchingWordReferenceString( char *queryString )
		{
		// This is a virtual function. Therefore, the given variables are unreferenced.
		ReferenceResultType referenceResult;
		return referenceResult;
		}

	ResultType Item::checkForUsage()
		{
		return RESULT_OK;
		}

	char *Item::itemString()
		{
		return NULL;
		}

	char *Item::virtualGuideByGrammarString()
		{
		return NULL;
		}


	char *Item::toString( unsigned short queryWordTypeNr )
		{
		char *queryString;
		char *myWordString = myWordTypeString( queryWordTypeNr );
		char *userNameString = ( myWordItem_ == NULL ? NULL : myWordItem_->userNameString( userNr_ ) );
		char statusString[2] = SPACE_STRING;

		statusString[0] = statusChar_;
		strcpy( commonVariables_->queryString, EMPTY_STRING );

		queryString = commonVariables_->queryString;

		// Show status if not active
		if( !isActiveItem() )
			strcat( queryString, statusString );

		if( myWordString != NULL )
			{
			sprintf( tempString, "%c%s%c", QUERY_WORD_START_CHAR, myWordString, QUERY_WORD_END_CHAR );
			strcat( queryString, tempString );
			}

		sprintf( tempString, "%c%c%c", QUERY_LIST_START_CHAR, ( myList_ == NULL ? QUERY_NO_LIST_CHAR : myList_->listChar() ), QUERY_LIST_END_CHAR );
		strcat( queryString, tempString );

		sprintf( tempString, "%c%u%c%u%c", QUERY_ITEM_START_CHAR, creationSentenceNr_, QUERY_SEPARATOR_CHAR, itemNr_, QUERY_ITEM_END_CHAR );
		strcat( queryString, tempString );
/*
		// Don't show: Always true during query
		if( isSelectedByQuery )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isSelectedByQuery" );
			}
*/
		if( isArchivedItem() ||
		isReplacedItem() )
			{
			sprintf( tempString, "%cpreviousStatusChar:%c", QUERY_SEPARATOR_CHAR, previousStatusChar );
			strcat( queryString, tempString );
			}

		if( userNr_ > NO_USER_NR )
			{
			if( userNameString != NULL )
				sprintf( tempString, "%cuser:%s", QUERY_SEPARATOR_CHAR, userNameString );
			else
				sprintf( tempString, "%cuser:%u", QUERY_SEPARATOR_CHAR, userNr_ );

			strcat( queryString, tempString );
			}

		if( originalSentenceNr_ > NO_SENTENCE_NR &&
		originalSentenceNr_ != creationSentenceNr_ )
			{
			sprintf( tempString, "%coriginalSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, originalSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( activeSentenceNr_ > NO_SENTENCE_NR &&
		activeSentenceNr_ != creationSentenceNr_ )
			{
			sprintf( tempString, "%cactiveSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, activeSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( inactiveSentenceNr_ > NO_SENTENCE_NR &&
		inactiveSentenceNr_ != creationSentenceNr_ )
			{
			sprintf( tempString, "%cinactiveSentence:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, inactiveSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( archivedSentenceNr_ > NO_SENTENCE_NR )
			{
			sprintf( tempString, "%carchivedSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, archivedSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( replacedSentenceNr_ > NO_SENTENCE_NR )
			{
			sprintf( tempString, "%creplacedSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, replacedSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		if( deletedSentenceNr_ > NO_SENTENCE_NR )
			{
			sprintf( tempString, "%cdeletedSentenceNr:%c%u%c", QUERY_SEPARATOR_CHAR, QUERY_ITEM_SENTENCE_NR_START_CHAR, deletedSentenceNr_, QUERY_ITEM_SENTENCE_NR_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected common functions

	void Item::setActiveStatus()
		{
		statusChar_ = QUERY_ACTIVE_CHAR;
		}

	void Item::setArchivedStatus()
		{
		statusChar_ = QUERY_ARCHIVED_CHAR;
		}

	void Item::setDeletedStatus()
		{
		statusChar_ = QUERY_DELETED_CHAR;
		}

	void Item::setInactiveStatus()
		{
		statusChar_ = QUERY_INACTIVE_CHAR;
		}

	void Item::setReplacedStatus()
		{
		statusChar_ = QUERY_REPLACED_CHAR;
		}

	void Item::setActiveSentenceNr()
		{
		if( activeSentenceNr_ == NO_SENTENCE_NR )
			activeSentenceNr_ = commonVariables_->currentSentenceNr;
		}

	void Item::setArchivedSentenceNr()
		{
		if( archivedSentenceNr_ == NO_SENTENCE_NR )
			archivedSentenceNr_ = commonVariables_->currentSentenceNr;
		}

	void Item::setDeletedSentenceNr()
		{
		deletedSentenceNr_ = commonVariables_->currentSentenceNr;
		}

	void Item::setInactiveSentenceNr()
		{
		if( inactiveSentenceNr_ == NO_SENTENCE_NR )
			inactiveSentenceNr_ = commonVariables_->currentSentenceNr;
		}

	void Item::setReplacedSentenceNr()
		{
		if( replacedSentenceNr_ == NO_SENTENCE_NR )
			replacedSentenceNr_ = commonVariables_->currentSentenceNr;
		}

	void Item::clearArchivedSentenceNr()
		{
		archivedSentenceNr_ = NO_SENTENCE_NR;
		}

	void Item::clearDeletedSentenceNr()
		{
		deletedSentenceNr_ = NO_SENTENCE_NR;
		}

	void Item::clearReplacedSentenceNr()
		{
		replacedSentenceNr_ = NO_SENTENCE_NR;
		}

	void Item::showWords( bool isReturnQueryToPosition, unsigned short queryWordTypeNr )
		{
		char *myWordString;

		statusString[0] = statusChar_;

		if( ( myWordString = myWordTypeString( queryWordTypeNr ) ) != NULL )
			{
			if( commonVariables_->hasFoundQuery )
				strcat( commonVariables_->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( commonVariables_->queryString, statusString );

			commonVariables_->hasFoundQuery = true;
			strcat( commonVariables_->queryString, myWordString );
			}
		}

	// Strictly for initialization of AdminItem
	void Item::initializeItemVariables( const char *classNameString, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructible variables

//		AdminItem has no constructible variables to be initialized

		// Private loadable variables

		commonVariables_ = commonVariables;
//		AdminItem has no myList_;
		myWordItem_ = myWordItem;

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( classNameString == NULL )
			strcpy( errorString, "The given my word is undefined" );
		else
			{
			if( classNameString != NULL )
				strcpy( classNameString_, classNameString );
			else
				strcpy( errorString, "The given class name string is undefined" );
			}

		if( strlen( errorString ) > 0 )
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), errorString );
		}

	void Item::initializeItemVariables( unsigned int originalSentenceNr, unsigned int activeSentenceNr, unsigned int inactiveSentenceNr, unsigned int archivedSentenceNr, const char *classNameString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private loadable variables

		commonVariables_ = commonVariables;
		myList_ = myList;
		myWordItem_ = myWordItem;

		// Private constructible variables

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );
		else
			{
		userNr_ = commonVariables_->currentUserNr;

		originalSentenceNr_ = ( originalSentenceNr == NO_SENTENCE_NR ? commonVariables_->currentSentenceNr : originalSentenceNr );
		creationSentenceNr_ = commonVariables_->currentSentenceNr;

		activeSentenceNr_ = ( originalSentenceNr == NO_SENTENCE_NR ? commonVariables_->currentSentenceNr : activeSentenceNr );
		inactiveSentenceNr_ = inactiveSentenceNr;
		archivedSentenceNr_ = archivedSentenceNr;

		itemNr_ = ++commonVariables_->currentItemNr;
			}

		if( myList_ == NULL )
			strcpy( errorString, "The given my list is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

		if( classNameString == NULL )
			strcpy( errorString, "The given my word is undefined" );
		else
			{
			if( classNameString != NULL )
				strcpy( classNameString_, classNameString );
			else
				strcpy( errorString, "The given class name string is undefined" );
			}

		if( strlen( errorString ) > 0 )
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, ( myWordItem_ == NULL || myWordItem_->isAdminWord() ? NULL : myWordItem_->anyWordTypeString() ), errorString );
		}

	bool Item::hasActiveSentenceNr()
		{
		return ( activeSentenceNr_ > NO_SENTENCE_NR );
		}

	bool Item::hasInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ > NO_SENTENCE_NR );
		}

	bool Item::hasCurrentOriginalSentenceNr()
		{
		return ( originalSentenceNr_ == commonVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentCreationSentenceNr()
		{
		return ( creationSentenceNr_ == commonVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentActiveSentenceNr()
		{
		return ( activeSentenceNr_ == commonVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentInactiveSentenceNr()
		{
		return ( inactiveSentenceNr_ == commonVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentArchivedSentenceNr()
		{
		return ( archivedSentenceNr_ == commonVariables_->currentSentenceNr );
		}

	bool Item::hasCurrentReplacedSentenceNr()
		{
		return ( replacedSentenceNr_ == commonVariables_->currentSentenceNr );
		}

	bool Item::hasSentenceNr( unsigned int sentenceNr )
		{
		return ( originalSentenceNr_ == sentenceNr ||
				creationSentenceNr_ == sentenceNr ||
				activeSentenceNr_ == sentenceNr ||
				inactiveSentenceNr_ == sentenceNr ||
				archivedSentenceNr_ == sentenceNr ||
				replacedSentenceNr_ == sentenceNr ||
				deletedSentenceNr_ == sentenceNr );
		}

	bool Item::isOlderItem()
		{
		return ( originalSentenceNr_ < commonVariables_->currentSentenceNr );
		}

	bool Item::isActiveItem()
		{
		return ( statusChar_ == QUERY_ACTIVE_CHAR );
		}

	bool Item::isInactiveItem()
		{
		return ( statusChar_ == QUERY_INACTIVE_CHAR );
		}

	bool Item::isArchivedItem()
		{
		return ( statusChar_ == QUERY_ARCHIVED_CHAR );
		}

	bool Item::isReplacedItem()
		{
		return ( statusChar_ == QUERY_REPLACED_CHAR );
		}

	bool Item::isDeletedItem()
		{
		return ( statusChar_ == QUERY_DELETED_CHAR );
		}

	bool Item::isReplacedOrDeletedItem()
		{
		return ( isReplacedItem() ||
				isDeletedItem() );
		}

	bool Item::isMoreRecent( Item *checkItem )
		{
		return ( checkItem != NULL &&

				( creationSentenceNr_ > checkItem->creationSentenceNr_ ||

				( creationSentenceNr_ == checkItem->creationSentenceNr_ &&
				itemNr_ > checkItem->itemNr_ ) ) );
		}

	bool Item::wasActiveBefore()
		{
		return ( previousStatusChar == QUERY_ACTIVE_CHAR );
		}

	bool Item::wasInactiveBefore()
		{
		return ( previousStatusChar == QUERY_INACTIVE_CHAR );
		}

	unsigned short Item::userNr()
		{
		return userNr_;
		}

	unsigned int Item::activeSentenceNr()
		{
		return activeSentenceNr_;
		}

	unsigned int Item::inactiveSentenceNr()
		{
		return inactiveSentenceNr_;
		}

	unsigned int Item::originalSentenceNr()
		{
		return originalSentenceNr_;
		}

	unsigned int Item::creationSentenceNr()
		{
		return creationSentenceNr_;
		}

	unsigned int Item::archivedSentenceNr()
		{
		return archivedSentenceNr_;
		}

	unsigned int Item::replacedSentenceNr()
		{
		return replacedSentenceNr_;
		}

	unsigned int Item::deletedSentenceNr()
		{
		return deletedSentenceNr_;
		}

	unsigned int Item::itemNr()
		{
		return itemNr_;
		}

	ResultType Item::decrementActiveSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementActiveSentenceNr";

		if( activeSentenceNr_ > NO_SENTENCE_NR )
			activeSentenceNr_--;
		else
			return startError( functionNameString, superClassNameString_, "The active sentence number is too low for a decrement" );

		return RESULT_OK;
		}

	ResultType Item::decrementInactiveSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementInactiveSentenceNr";

		if( inactiveSentenceNr_ > NO_SENTENCE_NR )
			inactiveSentenceNr_--;
		else
			return startError( functionNameString, superClassNameString_, "The inactive sentence number is too low for a decrement" );

		return RESULT_OK;
		}

	ResultType Item::decrementOriginalSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementOriginalSentenceNr";

		if( originalSentenceNr_ > NO_SENTENCE_NR )
			originalSentenceNr_--;
		else
			return startError( functionNameString, superClassNameString_, "The original sentence number is too low for a decrement" );

		return RESULT_OK;
		}

	ResultType Item::decrementCreationSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementCreationSentenceNr";

		if( creationSentenceNr_ > NO_SENTENCE_NR )
			creationSentenceNr_--;
		else
			return startError( functionNameString, superClassNameString_, "The creation sentence number is too low for a decrement" );

		return RESULT_OK;
		}

	ResultType Item::decrementArchivedSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementArchivedSentenceNr";

		if( archivedSentenceNr_ > NO_SENTENCE_NR )
			archivedSentenceNr_--;
		else
			return startError( functionNameString, superClassNameString_, "The archived sentence number is too low for a decrement" );

		return RESULT_OK;
		}

	ResultType Item::decrementReplacedSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementReplacedSentenceNr";

		if( replacedSentenceNr_ > NO_SENTENCE_NR )
			replacedSentenceNr_--;
		else
			return startError( functionNameString, superClassNameString_, "The replaced sentence number is too low for a decrement" );

		return RESULT_OK;
		}

	ResultType Item::decrementItemNr( unsigned int decrementOffset )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "decrementItemNr";

		if( itemNr_ > decrementOffset )
			itemNr_ -= decrementOffset;
		else
			return startError( functionNameString, superClassNameString_, "The given decrement offset is higher than the item number itself" );

		return RESULT_OK;
		}

	char Item::statusChar()
		{
		return statusChar_;
		}

	char *Item::classNameString()
		{
		return classNameString_;
		}

	char *Item::superClassNameString()
		{
		return superClassNameString_;
		}

	CommonVariables *Item::commonVariables()
		{
		return commonVariables_;
		}

	List *Item::myList()
		{
		return myList_;
		}

	WordItem *Item::myWordItem()
		{
		return myWordItem_;
		}


	// Protected definition functions

	bool Item::isAssumption( unsigned short justificationTypeNr )
		{
		return ( justificationTypeNr == JUSTIFICATION_TYPE_GENERALIZATION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_INDIRECTLY_ANSWERED_QUESTION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_SUGGESTIVE_QUESTION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION ||
				justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION );
		}

	bool Item::isConclusion( unsigned short justificationTypeNr )
		{
		return ( justificationTypeNr == JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_CONCLUSION ||
				justificationTypeNr == JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_CONCLUSION ||
				justificationTypeNr == JUSTIFICATION_TYPE_DEFINITION_PART_OF_CONCLUSION ||
				justificationTypeNr == JUSTIFICATION_TYPE_NEGATIVE_CONCLUSION ||
				justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_CONCLUSION ||
				justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_CONCLUSION ||
				justificationTypeNr == JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_CONCLUSION ||
				justificationTypeNr == JUSTIFICATION_TYPE_UNIQUE_RELATION_CONCLUSION );
		}

	bool Item::isAdjectiveParameter( unsigned short checkParameter )
		{
		return ( checkParameter == WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_EVERY_FEMININE_MASCULINE ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_PREVIOUS_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_PREVIOUS_FEMININE_MASCULINE ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_CURRENT_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_CURRENT_FEMININE_MASCULINE ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_NEW_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ADJECTIVE_NEW_FEMININE_MASCULINE );
//				checkParameter == WORD_PARAMETER_ADJECTIVE_NEXT_NEUTRAL ||
//				checkParameter == WORD_PARAMETER_ADJECTIVE_NEXT_FEMININE_MASCULINE );
		}

	bool Item::isDefiniteArticleParameter( unsigned short checkParameter )
		{
		return ( checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_NEUTRAL ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	bool Item::isIndefiniteArticleParameter( unsigned short checkParameter )
		{
		return ( checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||

				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||
				checkParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE );
		}

	bool Item::isFeminineArticleParameter( unsigned short articleParameter )
		{
		return ( articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ||

				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_FEMININE ||
				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE );
		}

	bool Item::isMasculineArticleParameter( unsigned short articleParameter )
		{
		return ( articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ||

				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_PLURAL_MASCULINE ||
				articleParameter == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE );
		}

	bool Item::isGeneralizationReasoningWordType( unsigned short wordTypeNr )
		{
		return ( wordTypeNr == WORD_TYPE_PROPER_NAME ||
				isSingularOrPluralNoun( wordTypeNr ) );
		}

	bool Item::isSingularOrPluralNoun( unsigned short wordTypeNr )
		{
		return ( wordTypeNr == WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr == WORD_TYPE_NOUN_PLURAL );
		}

	bool Item::isMatchingWordType( unsigned short firstWordTypeNr, unsigned short secondWordTypeNr )
		{
		return	( firstWordTypeNr == secondWordTypeNr ||

				( isSingularOrPluralNoun( firstWordTypeNr ) &&
				isSingularOrPluralNoun( secondWordTypeNr ) ) );
		}

	bool Item::isStartingWithPhoneticVowel( char *textString )
		{
		unsigned short index = 0;

		if( textString != NULL )
			{
			while( index < NUMBER_OF_PHONETIC_VOWELS )
				{
				if( textString[0] == PHONETIC_VOWEL[index] )
					return true;

				index++;
				}
			}

		return false;
		}

	unsigned short Item::assumptionGrade( bool hasAnotherPrimarySpecification, bool hasFeminineOrMasculineProperNameEnding, bool hasPossessivePrimarySpecification, bool hasPrimaryQuestionSpecification, unsigned short justificationTypeNr )
		{
		switch( justificationTypeNr )
			{
			case JUSTIFICATION_TYPE_ONLY_OPTION_LEFT_ASSUMPTION:
			case JUSTIFICATION_TYPE_DEFINITION_PART_OF_ASSUMPTION:
			case JUSTIFICATION_TYPE_SPECIFICATION_GENERALIZATION_SUBSTITUTION_ASSUMPTION:
			case JUSTIFICATION_TYPE_UNIQUE_RELATION_ASSUMPTION:
				return 0;

			case JUSTIFICATION_TYPE_OPPOSITE_POSSESSIVE_CONDITIONAL_SPECIFICATION_ASSUMPTION:
				return ( hasFeminineOrMasculineProperNameEnding ? 2 : 1 );

			case JUSTIFICATION_TYPE_EXCLUSIVE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				return ( hasAnotherPrimarySpecification && hasFeminineOrMasculineProperNameEnding ? 2 : 1 );

			case JUSTIFICATION_TYPE_POSSESSIVE_REVERSIBLE_ASSUMPTION:
				return ( hasFeminineOrMasculineProperNameEnding ? 1 : 0 );

			case JUSTIFICATION_TYPE_NEGATIVE_ASSUMPTION:
				return ( hasPossessivePrimarySpecification ? 1 : 0 );

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_PART_OF_ASSUMPTION:
				return ( hasPrimaryQuestionSpecification ? 1 : 0 );

			case JUSTIFICATION_TYPE_SPECIFICATION_SUBSTITUTION_ASSUMPTION:
				return ( hasAnotherPrimarySpecification ? 1 : 0 );

			default:
				return 1;
			}
		}

/*************************************************************************
 *	"Give thanks to him who made the heavenly lights-
 *		His faithful love endures forever.
 *	the sun to rule the day
 *		His faithful love endures forever.
 *	and the moon and stars to rule the night.
 *		His faithful love endures forever." (Psalm 136:7-9)
 *************************************************************************/
