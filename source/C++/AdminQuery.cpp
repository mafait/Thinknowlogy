/*	Class:			AdminQuery
 *	Supports class:	AdminItem
 *	Purpose:		To process queries
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

#include "AdminItem.h"

class AdminQuery
	{
	friend class AdminItem;

	// Private constructed variables

	unsigned int queryItemNr_;
	unsigned int querySentenceNr_;

	size_t queryCommandStringPosition_;

	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;


	// Private functions

	bool isDisplayingTotal()
		{
		unsigned int nFoundCategories = 0;

		if( commonVariables_->nActiveQueryItems > 0 )
			nFoundCategories++;

		if( commonVariables_->nInactiveQueryItems > 0 )
			nFoundCategories++;

		if( commonVariables_->nArchivedQueryItems > 0 )
			nFoundCategories++;

		if( commonVariables_->nReplacedQueryItems > 0 )
			nFoundCategories++;

		if( commonVariables_->nDeletedQueryItems > 0 )
			nFoundCategories++;

		return ( nFoundCategories > 1 );
		}

	bool isAdminListChar( char queryListChar )
		{
		return ( queryListChar == ADMIN_FILE_LIST_SYMBOL ||
				queryListChar == ADMIN_READ_LIST_SYMBOL ||
				queryListChar == ADMIN_SCORE_LIST_SYMBOL ||
				queryListChar == ADMIN_WORD_LIST_SYMBOL ||
				queryListChar == ADMIN_CONDITION_LIST_SYMBOL ||
				queryListChar == ADMIN_ACTION_LIST_SYMBOL ||
				queryListChar == ADMIN_ALTERNATIVE_LIST_SYMBOL );
		}

	bool isWordListChar( char queryListChar )
		{
		return ( queryListChar == WORD_ASSIGNMENT_LIST_SYMBOL ||
				queryListChar == WORD_COLLECTION_LIST_SYMBOL ||
				queryListChar == WORD_GENERALIZATION_LIST_SYMBOL ||
				queryListChar == WORD_INTERFACE_LIST_SYMBOL ||
				queryListChar == WORD_JUSTIFICATION_LIST_SYMBOL ||
				queryListChar == WORD_MULTIPLE_WORD_LIST_SYMBOL ||
				queryListChar == WORD_GRAMMAR_LIST_SYMBOL ||
				queryListChar == WORD_WRITE_LIST_SYMBOL ||
				queryListChar == WORD_SPECIFICATION_LIST_SYMBOL ||
				queryListChar == WORD_TYPE_LIST_SYMBOL ||
				queryListChar == WORD_CONTEXT_LIST_SYMBOL );
		}

	unsigned int nTotalCountQuery()
		{
		commonVariables_->nActiveQueryItems = 0;
		commonVariables_->nInactiveQueryItems = 0;
		commonVariables_->nArchivedQueryItems = 0;
		commonVariables_->nReplacedQueryItems = 0;
		commonVariables_->nDeletedQueryItems = 0;

		adminItem_->countQuery();

		return ( commonVariables_->nActiveQueryItems +
				commonVariables_->nInactiveQueryItems +
				commonVariables_->nArchivedQueryItems +
				commonVariables_->nReplacedQueryItems +
				commonVariables_->nDeletedQueryItems );
		}

	signed char getIdFromQuery( bool hasEndChar, char *queryCommandString, char startChar, char endChar )
		{
		size_t queryCommandStringLength;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getIdFromQuery";

		querySentenceNr_ = NO_SENTENCE_NR;
		queryItemNr_ = NO_ITEM_NR;

		if( queryCommandString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given query command string is undefined" );

		if( ( queryCommandStringLength = strlen( queryCommandString ) ) == 0 )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given query command string is empty" );

		if( queryCommandStringPosition_ <= 0 ||
		queryCommandStringPosition_ >= queryCommandStringLength )
			return adminItem_->startError( functionNameString, moduleNameString_, "The query command string position is not within the range of the given query command string" );

		if( queryCommandString[queryCommandStringPosition_] != startChar )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given command doesn't start with character '", startChar, "', but with '", queryCommandString[queryCommandStringPosition_], "'" );

		queryCommandStringPosition_++;

		if( queryCommandStringPosition_ < queryCommandStringLength &&
		queryCommandString[queryCommandStringPosition_] != endChar )
			{
			if( queryCommandString[queryCommandStringPosition_] == SYMBOL_ASTERISK )
				{
				queryCommandStringPosition_++;
				querySentenceNr_ = MAX_SENTENCE_NR;
				}
			else
				{
				if( queryCommandString[queryCommandStringPosition_] != QUERY_SEPARATOR_CHAR )
					{
					if( !isdigit( queryCommandString[queryCommandStringPosition_] ) )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find a number in the query string" );

					while( queryCommandStringPosition_ < queryCommandStringLength &&
					isdigit( queryCommandString[queryCommandStringPosition_] ) &&
					querySentenceNr_ <= MAX_SENTENCE_NR / 10 )
						{
						querySentenceNr_ = ( querySentenceNr_ * 10 ) + ( queryCommandString[queryCommandStringPosition_] - '0' );
						queryCommandStringPosition_++;
						}
					}

				if( hasEndChar &&
				queryCommandStringPosition_ < queryCommandStringLength &&
				queryCommandString[queryCommandStringPosition_] == QUERY_SEPARATOR_CHAR &&
				++queryCommandStringPosition_ < queryCommandStringLength &&
				queryCommandString[queryCommandStringPosition_] != endChar )
					{
					if( !isdigit( queryCommandString[queryCommandStringPosition_] ) )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the item number in the query string" );

					while( queryCommandStringPosition_ < queryCommandStringLength &&
					isdigit( queryCommandString[queryCommandStringPosition_] ) &&
					queryItemNr_ <= MAX_SENTENCE_NR / 10 )
						{
						queryItemNr_ = ( queryItemNr_ * 10 ) + ( queryCommandString[queryCommandStringPosition_] - '0' );
						queryCommandStringPosition_++;
						}
					}
				}
			}

		if( hasEndChar )
			{
			if( queryCommandStringPosition_ >= queryCommandStringLength ||
			queryCommandString[queryCommandStringPosition_] != endChar )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given query string is corrupt" );

			queryCommandStringPosition_++;
			}

		return RESULT_OK;
		}

	signed char getNameFromQuery( char *queryCommandString, char *nameString, char startChar, char endChar )
		{
		size_t queryCommandStringLength;
		size_t startSourceStringPosition;
		size_t nameLength = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getNameFromQuery";

		if( queryCommandString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given query command string is undefined" );

		if( nameString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given name string is undefined" );

		if( ( queryCommandStringLength = strlen( queryCommandString ) ) == 0 )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given query command string is empty" );

		if( queryCommandStringPosition_ <= 0 ||
		queryCommandStringPosition_ >= queryCommandStringLength )
			return adminItem_->startError( functionNameString, moduleNameString_, "The query command string position is not within the range of the given query command string" );

		if( queryCommandString[queryCommandStringPosition_] != startChar )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given name doesn't start with character '", startChar, "', but with '", queryCommandString[0], "'" );

		if( queryCommandStringPosition_ + 1 >= queryCommandStringLength )
			return adminItem_->startError( functionNameString, moduleNameString_, "The name in the given query string is corrupt" );

		queryCommandStringPosition_++;
		strcpy( nameString, EMPTY_STRING );

		if( queryCommandString[queryCommandStringPosition_] != endChar )
			{
			startSourceStringPosition = queryCommandStringPosition_;

			while( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
			nameLength <= MAX_SENTENCE_STRING_LENGTH - 1 &&
			queryCommandString[queryCommandStringPosition_] != endChar )
				{
				nameLength++;
				queryCommandStringPosition_++;
				}

			if( queryCommandString[queryCommandStringPosition_] != endChar )
				return adminItem_->startError( functionNameString, moduleNameString_, "The name in the given query string is corrupt" );

			strncat( nameString, &queryCommandString[startSourceStringPosition], nameLength );
			}

		queryCommandStringPosition_++;

		return RESULT_OK;
		}

	signed char itemQuery( bool isSelectingActiveItems, bool isSelectingInactiveItems, bool isSelectingArchivedItems, bool isSelectingReplacedItems, bool isSelectingDeletedItems, bool isSuppressingMessage, char *textString )
		{
		WordItem *currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQuery";

		if( textString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given text string is undefined" );

		adminItem_->itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, NO_SENTENCE_NR, NO_ITEM_NR );

		if( ( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The current language word item is undefined" );

		if( !isSuppressingMessage &&
		nTotalCountQuery() == 0 )
			strcat( textString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );

		return RESULT_OK;
		}

	protected:
	// Constructor

	AdminQuery( AdminItem *adminItem, CommonVariables *commonVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		queryItemNr_ = NO_ITEM_NR;
		querySentenceNr_ = NO_SENTENCE_NR;

		queryCommandStringPosition_ = 0;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminQuery" );

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( commonVariables != NULL )
				commonVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( commonVariables_ = commonVariables ) == NULL )
				strcpy( errorString, "The given common variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	QueryResultType executeQuery( bool isSuppressingMessage, bool isReturningToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, size_t queryCommandStringStartPosition, char *queryCommandString )
		{
		QueryResultType queryResult;
		bool isDisplayingCount = false;
		bool isEndOfQuery = false;
		bool isFirstInstruction = true;
		bool isInvalidChar = false;
		bool isOnlyDisplayingStrings = false;
		bool isOnlyDisplayingWords = false;
		bool isOnlyDisplayingWordReferences = false;
		bool isReturnQueryToPosition = isReturningToPosition;
		bool isSelectingActiveItems = true;
		bool isSelectingInactiveItems = true;
		bool isSelectingArchivedItems = true;
		bool isSelectingReplacedItems = true;
		bool isSelectingDeletedItems = true;
		bool isSelectingAttachedJustifications = false;
		bool isSelectingJustificationSpecifications = false;
		unsigned short queryWordTypeNr = NO_WORD_TYPE_NR;
		unsigned int nTotalCount = 0;
		size_t listStringPosition;
		size_t nameStringLength;
		size_t queryCommandStringLength;
		size_t queryWidth = NO_CENTER_WIDTH;
		WordItem *currentLanguageWordItem;
		char *queryString;
		char nameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeQuery";

		if( queryCommandString == NULL )
			return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "The given query command string is undefined" );

		if( ( queryCommandStringLength = strlen( queryCommandString ) ) == 0 )
			return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "The given query command string is empty" );

		if( queryCommandStringStartPosition <= 0 ||
		queryCommandStringStartPosition >= queryCommandStringLength )
			return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "The given query command string start position is not within the range of the given query command string" );

		if( ( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) == NULL )
			return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "The current language word item is undefined" );

		querySentenceNr_ = NO_SENTENCE_NR;
		queryItemNr_ = NO_ITEM_NR;

		queryCommandStringPosition_ = queryCommandStringStartPosition;

		strcpy( commonVariables_->queryString, EMPTY_STRING );
		queryString = commonVariables_->queryString;

		commonVariables_->hasFoundQuery = false;
		commonVariables_->matchingWordTypeNr = NO_WORD_TYPE_NR;

		adminItem_->clearQuerySelections();

		while( !isEndOfQuery &&
		strlen( commonVariables_->queryString ) == 0 &&
		queryCommandStringPosition_ < queryCommandStringLength )
			{
			switch( queryCommandString[queryCommandStringPosition_] )
				{
				case QUERY_ITEM_START_CHAR:
					if( getIdFromQuery( true, queryCommandString, QUERY_ITEM_START_CHAR, QUERY_ITEM_END_CHAR ) != RESULT_OK )
						return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get an identification from the item" );

					adminItem_->itemQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, querySentenceNr_, queryItemNr_ );

					isFirstInstruction = false;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_ITEM_WAS_FOUND ) );

					break;

				case QUERY_REF_ITEM_START_CHAR:
					if( getIdFromQuery( true, queryCommandString, QUERY_REF_ITEM_START_CHAR, QUERY_REF_ITEM_END_CHAR ) != RESULT_OK )
						return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get a reference identification from the item" );

					adminItem_->itemQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, true, querySentenceNr_, queryItemNr_ );

					isFirstInstruction = false;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_REFERENCE_ITEM_WAS_FOUND ) );

					break;

				case QUERY_LIST_START_CHAR:
					strcpy( nameString, EMPTY_STRING );

					if( getNameFromQuery( queryCommandString, nameString, QUERY_LIST_START_CHAR, QUERY_LIST_END_CHAR ) != RESULT_OK )
						return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get a list string from the text" );

					listStringPosition = 0;
					nameStringLength = strlen( nameString );

					// Check list characters for existence
					do	{
						if( nameStringLength > 0 &&
						!isWordListChar( nameString[listStringPosition] ) &&
						!isAdminListChar( nameString[listStringPosition] ) )
							{
							isInvalidChar = true;
							strcat( queryString, currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_INVALID_CHARACTER_IN_LIST ) );
							}
						}
					while( !isInvalidChar &&
					++listStringPosition < nameStringLength );

					// All list characters are valid
					if( !isInvalidChar )
						{
						adminItem_->listQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, nameString );

						isFirstInstruction = false;

						if( !isSuppressingMessage &&
						nTotalCountQuery() == 0 )
							strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_LIST_WAS_FOUND ) );
						}

					break;

				case QUERY_WORD_START_CHAR:
					if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
					queryCommandString[queryCommandStringPosition_ + 1] != QUERY_CHAR )
						{
						strcpy( nameString, EMPTY_STRING );

						if( getNameFromQuery( queryCommandString, nameString, QUERY_WORD_START_CHAR, QUERY_WORD_END_CHAR ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get a word name from the query specification" );

						if( strlen( nameString ) == 0 )
							{
							if( queryCommandStringPosition_ < queryCommandStringLength &&
							queryCommandString[queryCommandStringPosition_] != QUERY_CHAR )
								strcat( queryString, currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_EMPTY_WORD_SPECIFICATION ) );
							else
								{
								isOnlyDisplayingWords = true;
								isReturnQueryToPosition = true;
								}
							}
						else
							{
							if( adminItem_->wordQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, nameString ) != RESULT_OK )
								return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to query words" );

							isFirstInstruction = false;

							if( !isSuppressingMessage &&
							nTotalCountQuery() == 0 )
								strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_WAS_FOUND ) );
							}
						}
					else
						{
						isOnlyDisplayingWords = true;
						isReturnQueryToPosition = false;
						queryCommandStringPosition_++;
						}

					break;

				case QUERY_WORD_REFERENCE_START_CHAR:
					if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
					queryCommandString[queryCommandStringPosition_ + 1] != QUERY_CHAR )
						{
						strcpy( nameString, EMPTY_STRING );

						if( getNameFromQuery( queryCommandString, nameString, QUERY_WORD_REFERENCE_START_CHAR, QUERY_WORD_REFERENCE_END_CHAR ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get a word reference name from the query specification" );

						if( strlen( nameString ) == 0 )
							{
							if( queryCommandStringPosition_ < queryCommandStringLength &&
							queryCommandString[queryCommandStringPosition_] != QUERY_CHAR )
								strcat( queryString, currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_EMPTY_WORD_REFERENCE ) );
							else
								{
								isReturnQueryToPosition = true;
								isOnlyDisplayingWordReferences = true;
								}
							}
						else
							{
							if( adminItem_->wordReferenceQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSelectingAttachedJustifications, isSelectingJustificationSpecifications, nameString ) != RESULT_OK )
								return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to query word references" );

							isFirstInstruction = false;

							if( !isSuppressingMessage &&
							nTotalCountQuery() == 0 )
								strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_REFERENCE_WAS_FOUND ) );
							}
						}
					else
						{
						isReturnQueryToPosition = false;
						isOnlyDisplayingWordReferences = true;
						queryCommandStringPosition_++;
						}

					break;

				// Escape character for string
				case SYMBOL_BACK_SLASH:
					if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
					queryCommandString[queryCommandStringPosition_ + 1] != QUERY_CHAR )
						queryCommandStringPosition_++;

					// Don't insert a break statement here

				case QUERY_STRING_START_CHAR:
					if( queryCommandStringPosition_ + 1 < queryCommandStringLength &&
					queryCommandString[queryCommandStringPosition_ + 1] != QUERY_CHAR )
						{
						strcpy( nameString, EMPTY_STRING );

						if( getNameFromQuery( queryCommandString, nameString, QUERY_STRING_START_CHAR, QUERY_STRING_END_CHAR ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get a string from the query specification" );

						if( strlen( nameString ) == 0 )
							{
							if( queryCommandStringPosition_ < queryCommandStringLength &&
							queryCommandString[queryCommandStringPosition_] != QUERY_CHAR )
								strcat( queryString, currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_EMPTY_STRING_SPECIFICATION ) );
							else
								{
								isOnlyDisplayingStrings = true;
								isReturnQueryToPosition = true;
								}
							}
						else
							{
							if( adminItem_->stringQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, nameString ) != RESULT_OK )
								return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to query strings" );

							isFirstInstruction = false;
							queryWordTypeNr = commonVariables_->matchingWordTypeNr;

							if( !isSuppressingMessage &&
							nTotalCountQuery() == 0 )
								strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_STRING_WAS_FOUND ) );
							}
						}
					else
						{
						isOnlyDisplayingStrings = true;
						isReturnQueryToPosition = false;
						queryCommandStringPosition_++;
						}

					break;

				case QUERY_WORD_TYPE_CHAR:
					querySentenceNr_ = NO_SENTENCE_NR;

					if( getIdFromQuery( false, queryCommandString, QUERY_WORD_TYPE_CHAR, QUERY_WORD_TYPE_CHAR ) != RESULT_OK )
						return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get a word type" );

					if( queryItemNr_ != NO_ITEM_NR )
						return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "The given parameter is undefined" );

					adminItem_->wordTypeQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, (unsigned short)querySentenceNr_ );

					isFirstInstruction = false;
					// Remember given word type number
					queryWordTypeNr = (unsigned short)querySentenceNr_;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_TYPE_WAS_FOUND ) );

					break;

				case QUERY_PARAMETER_CHAR:
					querySentenceNr_ = NO_SENTENCE_NR;

					if( getIdFromQuery( false, queryCommandString, QUERY_PARAMETER_CHAR, QUERY_PARAMETER_CHAR ) != RESULT_OK )
						return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to get a parameter" );

					if( queryItemNr_ != NO_ITEM_NR )
						return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "The given parameter is undefined" );

					adminItem_->parameterQuery( isFirstInstruction, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, querySentenceNr_ );

					isFirstInstruction = false;

					if( !isSuppressingMessage &&
					nTotalCountQuery() == 0 )
						strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_PARAMETER_WAS_FOUND ) );

					break;

				case QUERY_ACTIVE_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems &&
					isSelectingDeletedItems )
						{
						isSelectingInactiveItems = false;
						isSelectingArchivedItems = false;
						isSelectingReplacedItems = false;
						isSelectingDeletedItems = false;
						}
					else
						isSelectingActiveItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString[queryCommandStringPosition_] == QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryString ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to execute an item query of active items" );
						}

					break;

				case QUERY_INACTIVE_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems &&
					isSelectingDeletedItems )
						{
						isSelectingActiveItems = false;
						isSelectingArchivedItems = false;
						isSelectingReplacedItems = false;
						isSelectingDeletedItems = false;
						}
					else
						isSelectingInactiveItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString[queryCommandStringPosition_] == QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryString ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to execute an item query of inactive items" );
						}

					break;

				case QUERY_ARCHIVED_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems &&
					isSelectingDeletedItems )
						{
						isSelectingActiveItems = false;
						isSelectingInactiveItems = false;
						isSelectingReplacedItems = false;
						isSelectingDeletedItems = false;
						}
					else
						isSelectingArchivedItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString[queryCommandStringPosition_] == QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryString ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to execute an item query of archived items" );
						}

					break;

				case QUERY_REPLACED_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems &&
					isSelectingDeletedItems )
						{
						isSelectingActiveItems = false;
						isSelectingInactiveItems = false;
						isSelectingArchivedItems = false;
						isSelectingDeletedItems = false;
						}
					else
						isSelectingReplacedItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString[queryCommandStringPosition_] == QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryString ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to execute an item query of archived items" );
						}

					break;

				case QUERY_DELETED_CHAR:
					// Initially
					if( isSelectingActiveItems &&
					isSelectingInactiveItems &&
					isSelectingArchivedItems &&
					isSelectingReplacedItems &&
					isSelectingDeletedItems )
						{
						isSelectingActiveItems = false;
						isSelectingInactiveItems = false;
						isSelectingArchivedItems = false;
						isSelectingReplacedItems = false;
						}
					else
						isSelectingDeletedItems = true;

					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString[queryCommandStringPosition_] == QUERY_CHAR )
						{
						if( itemQuery( isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, isSuppressingMessage, queryString ) != RESULT_OK )
							return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to execute an item query of deleted items" );
						}

					break;

				case QUERY_COUNT_CHAR:
					isDisplayingCount = true;
					queryCommandStringPosition_++;

					if( queryCommandStringPosition_ >= queryCommandStringLength ||
					// End of query
					queryCommandString[queryCommandStringPosition_] == QUERY_CHAR )
						adminItem_->itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, NO_SENTENCE_NR, NO_ITEM_NR );

					break;

				case QUERY_SELECTING_ATTACHED_JUSTIFICATIONS_CHAR:
					if( isSelectingAttachedJustifications )
						// Second asterisk
						isSelectingJustificationSpecifications = true;
					else
						// First asterisk
						isSelectingAttachedJustifications = true;

					queryCommandStringPosition_++;

					break;

				case QUERY_CHAR:
					isEndOfQuery = true;
					queryCommandStringPosition_++;

					break;

				default:
					// Set query width parameter
					if( !isdigit( queryCommandString[queryCommandStringPosition_] ) )
						return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "I found an illegal character '", queryCommandString[queryCommandStringPosition_], "' in the query" );

					while( queryCommandStringPosition_ < queryCommandStringLength &&
					isdigit( queryCommandString[queryCommandStringPosition_] ) &&
					queryWidth <= MAX_SENTENCE_NR / 10 )
						{
						queryWidth = ( queryWidth * 10 + queryCommandString[queryCommandStringPosition_] - '0' );
						queryCommandStringPosition_++;
						}

					if( queryWidth >= NUMBER_OF_CONSOLE_COLUMNS )
						return adminItem_->startQueryResultError( functionNameString, moduleNameString_, "The query width in the query is too high" );
				}
			}

		if( strlen( queryString ) == 0 )
			{
			// No query executed yet
			if( isFirstInstruction )
				adminItem_->itemQuery( true, isSelectingActiveItems, isSelectingInactiveItems, isSelectingArchivedItems, isSelectingReplacedItems, isSelectingDeletedItems, false, NO_SENTENCE_NR, NO_ITEM_NR );

			if( isDisplayingCount )
				{
				nTotalCount = nTotalCountQuery();

				if( isSuppressingMessage )
					sprintf( queryString, "%u", nTotalCount );
				else
					{
					if( nTotalCount == 0 )
						strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
					else
						{
						if( isDisplayingTotal() )
							sprintf( queryString, "total:%u", nTotalCount );

						if( commonVariables_->nActiveQueryItems > 0 )
							{
							if( strlen( queryString ) > 0 )
								strcat( queryString, QUERY_SEPARATOR_SPACE_STRING );

							sprintf( queryString, "%sactive:%u", queryString, commonVariables_->nActiveQueryItems );
							}

						if( commonVariables_->nInactiveQueryItems > 0 )
							{
							if( strlen( queryString ) > 0 )
								strcat( queryString, QUERY_SEPARATOR_SPACE_STRING );

							sprintf( queryString, "%sinactive:%u.\n", queryString, commonVariables_->nInactiveQueryItems );
							}

						if( commonVariables_->nArchivedQueryItems > 0 )
							{
							if( strlen( queryString ) > 0 )
								strcat( queryString, QUERY_SEPARATOR_SPACE_STRING );

							sprintf( queryString, "%sarchived:%u.\n", queryString, commonVariables_->nArchivedQueryItems );
							}

						if( commonVariables_->nReplacedQueryItems > 0 )
							{
							if( strlen( queryString ) > 0 )
								strcat( queryString, QUERY_SEPARATOR_SPACE_STRING );

							sprintf( queryString, "%sreplaced:%u.\n", queryString, commonVariables_->nReplacedQueryItems );
							}

						if( commonVariables_->nDeletedQueryItems > 0 )
							{
							if( strlen( queryString ) > 0 )
								strcat( queryString, QUERY_SEPARATOR_SPACE_STRING );

							sprintf( queryString, "%sdeleted:%u.\n", queryString, commonVariables_->nDeletedQueryItems );
							}
						}
					}
				}
			else
				{
				commonVariables_->hasFoundQuery = false;

				if( adminItem_->displayQueryResult( isOnlyDisplayingWords, isOnlyDisplayingWordReferences, isOnlyDisplayingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
					return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to display the query result" );

				if( !isSuppressingMessage &&
				!commonVariables_->hasFoundQuery &&
				strlen( queryString ) == 0 )
					{
					if( isOnlyDisplayingWords )
						strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORDS_WERE_FOUND ) );
					else
						{
						if( isOnlyDisplayingWordReferences )
							strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_REFERENCES_WERE_FOUND ) );
						else
							{
							if( isOnlyDisplayingStrings )
								strcat( queryString, currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_STRINGS_WERE_FOUND ) );
							}
						}
					}
				}
			}

		if( isDisplayingCount &&
		nTotalCount > 0 &&
		queryWidth == NO_CENTER_WIDTH &&
		!commonVariables_->hasFoundQuery )
			strcpy( commonVariables_->queryString, EMPTY_STRING );
		else
			{
			if( isWritingQueryResult &&

			( queryWidth > NO_CENTER_WIDTH ||
			strlen( queryString ) > 0 ) &&

			// Display comment on empty query
			inputOutput_->writeText( ( !isSuppressingMessage && !commonVariables_->hasFoundQuery && queryWidth == NO_CENTER_WIDTH ), promptTypeNr, queryWidth, queryString ) != RESULT_OK )
				return adminItem_->addQueryResultError( functionNameString, moduleNameString_, "I failed to write the query result" );
			}

		queryResult.queryCommandStringPosition = queryCommandStringPosition_;
		return queryResult;
		}
	};

/*************************************************************************
 *	"Honor the Lord, you heavenly beings;
 *	honor the Lord for his glory and strength." (Psalm 29:1)
 *************************************************************************/
