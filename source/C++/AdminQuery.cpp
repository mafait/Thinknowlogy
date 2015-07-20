/*
 *	Class:			AdminQuery
 *	Supports class:	AdminItem
 *	Purpose:		To process queries
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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
#include "Presentation.cpp"
#include "WordList.cpp"

class AdminQuery
	{
	friend class AdminItem;

	// Private constructible variables

	unsigned int queryItemNr_;
	unsigned int querySentenceNr_;
	size_t queryStringPosition_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void clearQuerySelections()
		{
		WordList *wordList;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->clearQuerySelectionsInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				adminItem_->adminListArray[adminListNr]->clearQuerySelectionsInList();
			}
		}

	bool isNeedingToShowTotal()
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

	unsigned int nTotalCount()
		{
		return ( commonVariables_->nActiveQueryItems +
				commonVariables_->nInactiveQueryItems +
				commonVariables_->nArchivedQueryItems +
				commonVariables_->nReplacedQueryItems +
				commonVariables_->nDeletedQueryItems );
		}

	unsigned int countQuery()
		{
		WordList *wordList;

		commonVariables_->nActiveQueryItems = 0;
		commonVariables_->nInactiveQueryItems = 0;
		commonVariables_->nArchivedQueryItems = 0;
		commonVariables_->nReplacedQueryItems = 0;
		commonVariables_->nDeletedQueryItems = 0;

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			wordList->countQueryInWordList();

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				adminItem_->adminListArray[adminListNr]->countQueryInList();
			}

		return nTotalCount();
		}

	ResultType itemQuery( bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, bool isSuppressingMessage, char *textString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQuery";

		if( textString != NULL )
			{
			if( itemQuery( true, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, NO_SENTENCE_NR, NO_ITEM_NR ) == RESULT_OK )
				{
				if( commonVariables_->currentLanguageWordItem != NULL )
					{
					if( !isSuppressingMessage &&
					countQuery() == 0 )
						strcat( textString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );
				}
			else
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query items" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given text string is undefined" );

		return RESULT_OK;
		}

	ResultType showQueryResult( bool isOnlyShowingWords, bool isOnlyShowingWordReferences, bool isOnlyShowingStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "showQueryResult";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->showQueryResultInWordList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, ( queryWordTypeNr == WORD_TYPE_UNDEFINED ? commonVariables_->matchingWordTypeNr : queryWordTypeNr ), queryWidth ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to show the query result in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->showQueryResultInList( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) != RESULT_OK )
					return adminItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to show the query result" );
				}
			}

		return RESULT_OK;
		}

	ResultType getIdFromQuery( bool hasEndChar, char *sourceString, char startChar, char endChar )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "getIdFromQuery";

		querySentenceNr_ = NO_SENTENCE_NR;
		queryItemNr_ = NO_ITEM_NR;

		if( sourceString != NULL )
			{
				if( queryStringPosition_ < strlen( sourceString ) )
					{
					if( sourceString[queryStringPosition_] == startChar )
						{
						queryStringPosition_++;

						if( queryStringPosition_ < strlen( sourceString ) &&
						sourceString[queryStringPosition_] != endChar )
							{
							if( sourceString[queryStringPosition_] == SYMBOL_ASTERISK )
								{
								queryStringPosition_++;
								querySentenceNr_ = MAX_SENTENCE_NR;
								}
							else
								{
								if( sourceString[queryStringPosition_] != QUERY_SEPARATOR_CHAR )
									{
									if( isdigit( sourceString[queryStringPosition_] ) )
										{
										while( queryStringPosition_ < strlen( sourceString ) &&
										isdigit( sourceString[queryStringPosition_] ) &&
										querySentenceNr_ <= MAX_SENTENCE_NR / 10 )
											{
											querySentenceNr_ = ( querySentenceNr_ * 10 ) + ( sourceString[queryStringPosition_] - '0' );
											queryStringPosition_++;
											}
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find a number in the query string" );
									}

								if( hasEndChar &&
								queryStringPosition_ < strlen( sourceString ) &&
								sourceString[queryStringPosition_] == QUERY_SEPARATOR_CHAR )
									{
									queryStringPosition_++;

									if( queryStringPosition_ < strlen( sourceString ) &&
									sourceString[queryStringPosition_] != endChar )
										{
										if( isdigit( sourceString[queryStringPosition_] ) )
											{
											while( queryStringPosition_ < strlen( sourceString ) &&
											isdigit( sourceString[queryStringPosition_] ) &&
											queryItemNr_ <= MAX_SENTENCE_NR / 10 )
												{
												queryItemNr_ = ( queryItemNr_ * 10 ) + ( sourceString[queryStringPosition_] - '0' );
												queryStringPosition_++;
												}
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't find the item number in the query string" );
										}
									}
								}
							}

						if( hasEndChar )
							{
							if( queryStringPosition_ < strlen( sourceString ) &&
							sourceString[queryStringPosition_] == endChar )
								queryStringPosition_++;
							else
								return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given query string is corrupt" );
							}
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given command doesn't start with character '", startChar, "', but with '", sourceString[queryStringPosition_], "'" );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given source string is empty or the given query source string position is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given source string is undefined" );

		return RESULT_OK;
		}

	ResultType getNameFromQuery( char *sourceString, char *nameString, char startChar, char endChar )
		{
		size_t nameLength = 0;
		size_t startSourceStringPosition;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getNameFromQuery";

		if( sourceString != NULL )
			{
			if( nameString != NULL )
				{
					if( queryStringPosition_ < strlen( sourceString ) )
						{
						if( sourceString[queryStringPosition_] == startChar )
							{
							if( queryStringPosition_ + 1 < strlen( sourceString ) )
								{
								queryStringPosition_++;

								if( sourceString[queryStringPosition_] != endChar )
									{
									startSourceStringPosition = queryStringPosition_;

									while( queryStringPosition_ + 1 < strlen( sourceString ) &&
									nameLength <= MAX_SENTENCE_STRING_LENGTH - 1 &&
									sourceString[queryStringPosition_] != endChar )
										{
										nameLength++;
										queryStringPosition_++;
										}

									if( sourceString[queryStringPosition_] == endChar )
										strncpy( nameString, &sourceString[startSourceStringPosition], nameLength );
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The name in the given query string is corrupt" );
									}

								nameString[nameLength] = NULL_CHAR;
								queryStringPosition_++;
								}
							else
								return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The name in the given query string is corrupt" );
							}
						else
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given name doesn't start with character '", startChar, "', but with '", sourceString[0], "'" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given source string is empty or the given query source string position is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given name string is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given source string is undefined" );

		return RESULT_OK;
		}

	ResultType itemQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "itemQuery";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->itemQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query item numbers in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->itemQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isReferenceQuery, querySentenceNr, queryItemNr ) != RESULT_OK )
					return adminItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to query item numbers in" );
				}
			}

		return RESULT_OK;
		}

	ResultType listQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *queryListString )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "listQuery";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->listQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListString ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->listQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryListString ) != RESULT_OK )
					return adminItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to do a list query module" );
				}
			}

		return RESULT_OK;
		}

	ResultType wordTypeQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned short queryWordTypeNr )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordTypeQuery";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->wordTypeQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query word types in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->wordTypeQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryWordTypeNr ) != RESULT_OK )
					return adminItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to word types" );
				}
			}

		return RESULT_OK;
		}

	ResultType parameterQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned int queryParameter )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "parameterQuery";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->parameterQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query parameters in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->parameterQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryParameter ) != RESULT_OK )
					return adminItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to parameters" );
				}
			}

		return RESULT_OK;
		}

	ResultType wordQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordNameString )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordQuery";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->wordQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordNameString ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query the words in my word list" );
			}

		return RESULT_OK;
		}

	ResultType wordReferenceQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordReferenceNameString )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQuery";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->wordReferenceQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query word references in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->wordReferenceQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, wordReferenceNameString ) != RESULT_OK )
					return adminItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to query word references" );
				}
			}

		return RESULT_OK;
		}

	ResultType stringQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *queryString )
		{
		WordList *wordList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQuery";

		// In words
		if( ( wordList = adminItem_->wordList ) != NULL )
			{
			if( wordList->stringQueryInWordList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString ) != RESULT_OK )
				return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query strings in my word list" );
			}

		// Admin lists
		for( unsigned short adminListNr = 0; adminListNr < NUMBER_OF_ADMIN_LISTS; adminListNr++ )
			{
			if( adminItem_->adminListArray[adminListNr] != NULL )
				{
				if( adminItem_->adminListArray[adminListNr]->stringQueryInList( isSelectOnFind, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, queryString ) != RESULT_OK )
					return adminItem_->addErrorInItem( adminItem_->adminListChar( adminListNr ), functionNameString, moduleNameString_, "I failed to query strings" );
				}
			}

		return RESULT_OK;
		}

	protected:
	// Constructor / deconstructor

	AdminQuery( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		queryItemNr_ = NO_ITEM_NR;
		querySentenceNr_ = NO_SENTENCE_NR;
		queryStringPosition_ = 1;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminQuery" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void initializeQueryStringPosition()
		{
		queryStringPosition_ = 1;
		}

	ResultType writeTextWithPossibleQueryCommands( unsigned short promptTypeNr, char *textString )
		{
		bool hasFoundNewLine = false;
		size_t previousPosition;
		size_t position = 0;
		char textChar = SYMBOL_QUESTION_MARK;
		char charString[2] = SPACE_STRING;
		char writeString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeTextWithPossibleQueryCommands";

		if( textString != NULL )
			{
			if( textString[0] == SYMBOL_DOUBLE_QUOTE )
				position++;

			while( position < strlen( textString ) &&
			textString[position] != SYMBOL_DOUBLE_QUOTE )
				{
				if( textString[position] == QUERY_CHAR )
					{
					if( ++position < strlen( textString ) )
						{
						previousPosition = position;
						queryStringPosition_ = position;

						if( executeQuery( true, false, true, promptTypeNr, textString ) == RESULT_OK )
							position = queryStringPosition_;
						else
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to execute query \"", &textString[previousPosition], "\"" );
						}
					else
						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The text string ended with a query character" );
					}
				else
					{
					if( textString[position] == TEXT_DIACRITICAL_CHAR )
						{
						if( ++position < strlen( textString ) )
							{
							if( ( textChar = commonVariables_->presentation->convertDiacriticalChar( textString[position] ) ) == NEW_LINE_CHAR )
								hasFoundNewLine = true;
							}
						else
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The text string ended with a diacritical sign" );
						}
					else
						textChar = textString[position];

					position++;
					charString[0] = textChar;
					strcat( writeString, charString );
					}

				if( hasFoundNewLine ||
				strlen( writeString ) + 1 == MAX_SENTENCE_STRING_LENGTH ||

				( position < strlen( textString ) &&
				textString[position] != SYMBOL_DOUBLE_QUOTE &&
				textString[position] == QUERY_CHAR &&
				strlen( writeString ) > 0 ) )
					{
					if( commonVariables_->presentation->writeText( false, promptTypeNr, NO_CENTER_WIDTH, writeString ) == RESULT_OK )
						{
						hasFoundNewLine = false;
						strcpy( writeString, EMPTY_STRING );
						}
					else
						return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write a character" );
					}
				}

			if( strlen( writeString ) > 0 )
				{
				if( commonVariables_->presentation->writeText( false, promptTypeNr, NO_CENTER_WIDTH, writeString ) != RESULT_OK )
					return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the last characters" );
				}
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given text string is undefined" );

		return RESULT_OK;
		}

	ResultType executeQuery( bool isSuppressingMessage, bool isReturningToPosition, bool isWritingQueryResult, unsigned short promptTypeNr, char *queryString )
		{
		bool isEndOfQuery = false;
		bool isFirstInstruction = true;
		bool isInvalidChar = false;
		bool isOnlyShowingStrings = false;
		bool isOnlyShowingWords = false;
		bool isOnlyShowingWordReferences = false;
		bool isReturnQueryToPosition = isReturningToPosition;
		bool isSelectActiveItems = true;
		bool isSelectInactiveItems = true;
		bool isSelectArchivedItems = true;
		bool isSelectReplacedItems = true;
		bool isSelectDeletedItems = true;
		bool isShowingCount = false;
		unsigned short queryWordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned int nTotal;
		size_t listStringPosition;
		size_t queryWidth = NO_CENTER_WIDTH;
		char nameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeQuery";

		strcpy( commonVariables_->queryString, EMPTY_STRING );

		if( queryString != NULL )
			{
			if( queryStringPosition_ > 0 &&
			queryStringPosition_ < strlen( queryString ) )
				{
				if( commonVariables_->currentLanguageWordItem != NULL )
					{
					clearQuerySelections();

					querySentenceNr_ = NO_SENTENCE_NR;
					queryItemNr_ = NO_ITEM_NR;

					commonVariables_->hasFoundQuery = false;
					commonVariables_->matchingWordTypeNr = WORD_TYPE_UNDEFINED;

					while( !isEndOfQuery &&
					strlen( commonVariables_->queryString ) == 0 &&
					queryStringPosition_ < strlen( queryString ) )
						{
						switch( queryString[queryStringPosition_] )
							{
							case QUERY_ITEM_START_CHAR:

								if( getIdFromQuery( true, queryString, QUERY_ITEM_START_CHAR, QUERY_ITEM_END_CHAR ) == RESULT_OK )
									{
									if( itemQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, querySentenceNr_, queryItemNr_ ) == RESULT_OK )
										{
										isFirstInstruction = false;

										if( !isSuppressingMessage &&
										countQuery() == 0 )
											strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_ITEM_WAS_FOUND ) );
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query items" );
									}
								else
									return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get an identification from the item" );

								break;

							case QUERY_REF_ITEM_START_CHAR:

								if( getIdFromQuery( true, queryString, QUERY_REF_ITEM_START_CHAR, QUERY_REF_ITEM_END_CHAR ) == RESULT_OK )
									{
									if( itemQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, true, querySentenceNr_, queryItemNr_ ) == RESULT_OK )
										{
										isFirstInstruction = false;

										if( !isSuppressingMessage &&
										countQuery() == 0 )
											strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_REFERENCE_ITEM_WAS_FOUND ) );
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query item references" );
									}
								else
									return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a reference identification from the item" );

								break;

							case QUERY_LIST_START_CHAR:
								strcpy( nameString, EMPTY_STRING );

								if( getNameFromQuery( queryString, nameString, QUERY_LIST_START_CHAR, QUERY_LIST_END_CHAR ) == RESULT_OK )
									{
									listStringPosition = 0;

									// Check list characters for existence
									do	{
										if( strlen( nameString ) > 0 &&
										!isWordListChar( nameString[listStringPosition] ) &&
										!isAdminListChar( nameString[listStringPosition] ) )
											{
											isInvalidChar = true;
											strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_INVALID_CHARACTER_IN_LIST ) );
											}
										}
									while( !isInvalidChar &&
									++listStringPosition < strlen( nameString ) );

									// All list characters are valid
									if( !isInvalidChar )
										{
										if( listQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameString ) == RESULT_OK )
											{
											isFirstInstruction = false;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_LIST_WAS_FOUND ) );
											}
										else
											return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query lists" );
										}
									}
								else
									return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a list string from the text" );

								break;

							case QUERY_WORD_START_CHAR:
								if( queryStringPosition_ + 1 < strlen( queryString ) &&
								queryString[queryStringPosition_ + 1] != QUERY_CHAR )
									{
									strcpy( nameString, EMPTY_STRING );

									if( getNameFromQuery( queryString, nameString, QUERY_WORD_START_CHAR, QUERY_WORD_END_CHAR ) == RESULT_OK )
										{
										if( strlen( nameString ) == 0 )
											{
											if( queryStringPosition_ < strlen( queryString ) &&
											queryString[queryStringPosition_] != QUERY_CHAR )
												strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_EMPTY_WORD_SPECIFICATION ) );
											else
												{
												isOnlyShowingWords = true;
												isReturnQueryToPosition = true;
												}
											}
										else
											{
											if( wordQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameString ) == RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_WAS_FOUND ) );
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query words" );
											}
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a word name from the query specification" );
									}
								else
									{
									isOnlyShowingWords = true;
									isReturnQueryToPosition = false;
									queryStringPosition_++;
									}

								break;

							case QUERY_WORD_REFERENCE_START_CHAR:
								if( queryStringPosition_ + 1 < strlen( queryString ) &&
								queryString[queryStringPosition_ + 1] != QUERY_CHAR )
									{
									strcpy( nameString, EMPTY_STRING );

									if( getNameFromQuery( queryString, nameString, QUERY_WORD_REFERENCE_START_CHAR, QUERY_WORD_REFERENCE_END_CHAR ) == RESULT_OK )
										{
										if( strlen( nameString ) == 0 )
											{
											if( queryStringPosition_ < strlen( queryString ) &&
											queryString[queryStringPosition_] != QUERY_CHAR )
												strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_EMPTY_WORD_REFERENCE ) );
											else
												{
												isReturnQueryToPosition = true;
												isOnlyShowingWordReferences = true;
												}
											}
										else
											{
											if( wordReferenceQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameString ) == RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_REFERENCE_WAS_FOUND ) );
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query word references" );
											}
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a word reference name from the query specification" );
									}
								else
									{
									isReturnQueryToPosition = false;
									isOnlyShowingWordReferences = true;
									queryStringPosition_++;
									}

								break;

							case SYMBOL_BACK_SLASH:		// Escape character for string
								if( queryStringPosition_ + 1 < strlen( queryString ) &&
								queryString[queryStringPosition_ + 1] != QUERY_CHAR )
									queryStringPosition_++;

								// Don't insert a break statement here

							case QUERY_STRING_START_CHAR:
								if( queryStringPosition_ + 1 < strlen( queryString ) &&
								queryString[queryStringPosition_ + 1] != QUERY_CHAR )
									{
									strcpy( nameString, EMPTY_STRING );

									if( getNameFromQuery( queryString, nameString, QUERY_STRING_START_CHAR, QUERY_STRING_END_CHAR ) == RESULT_OK )
										{
										if( strlen( nameString ) == 0 )
											{
											if( queryStringPosition_ < strlen( queryString ) &&
											queryString[queryStringPosition_] != QUERY_CHAR )
												strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( isSuppressingMessage ? INTERFACE_QUERY_ERROR : INTERFACE_QUERY_EMPTY_STRING_SPECIFICATION ) );
											else
												{
												isOnlyShowingStrings = true;
												isReturnQueryToPosition = true;
												}
											}
										else
											{
											if( stringQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, nameString ) == RESULT_OK )
												{
												isFirstInstruction = false;

												if( !isSuppressingMessage &&
												countQuery() == 0 )
													strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_STRING_WAS_FOUND ) );
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query strings" );
											}
										}
									else
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a string from the query specification" );
									}
								else
									{
									isOnlyShowingStrings = true;
									isReturnQueryToPosition = false;
									queryStringPosition_++;
									}

								break;

							case QUERY_WORD_TYPE_CHAR:

								querySentenceNr_ = NO_SENTENCE_NR;

								if( getIdFromQuery( false, queryString, QUERY_WORD_TYPE_CHAR, QUERY_WORD_TYPE_CHAR ) == RESULT_OK )
									{
									if( queryItemNr_ == NO_ITEM_NR )
										{
										if( wordTypeQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, (unsigned short)querySentenceNr_ ) == RESULT_OK )
											{
											isFirstInstruction = false;
											// Remember given word type number
											queryWordTypeNr = (unsigned short)querySentenceNr_;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_TYPE_WAS_FOUND ) );
											}
										else
											return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query word types" );
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given parameter is undefined" );
									}
								else
									return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a word type" );

								break;

							case QUERY_PARAMETER_CHAR:

								querySentenceNr_ = NO_SENTENCE_NR;

								if( getIdFromQuery( false, queryString, QUERY_PARAMETER_CHAR, QUERY_PARAMETER_CHAR ) == RESULT_OK )
									{
									if( queryItemNr_ == NO_ITEM_NR )
										{
										if( parameterQuery( isFirstInstruction, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, querySentenceNr_ ) == RESULT_OK )
											{
											isFirstInstruction = false;

											if( !isSuppressingMessage &&
											countQuery() == 0 )
												strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_PARAMETER_WAS_FOUND ) );
											}
										else
											return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query parameters" );
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given parameter is undefined" );
									}
								else
									return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get a parameter" );

								break;

							case QUERY_CHAR:

								isEndOfQuery = true;
								queryStringPosition_++;

								break;

							case QUERY_ACTIVE_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectInactiveItems = false;
									isSelectArchivedItems = false;
									isSelectReplacedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectActiveItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= strlen( queryString ) ||
								// End of query
								queryString[queryStringPosition_] == QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, commonVariables_->queryString ) != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to perform an item query of active items" );
									}

								break;

							case QUERY_INACTIVE_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectArchivedItems = false;
									isSelectReplacedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectInactiveItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= strlen( queryString ) ||
								// End of query
								queryString[queryStringPosition_] == QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, commonVariables_->queryString ) != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to perform an item query of inactive items" );
									}

								break;

							case QUERY_ARCHIVED_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectInactiveItems = false;
									isSelectReplacedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectArchivedItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= strlen( queryString ) ||
								// End of query
								queryString[queryStringPosition_] == QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, commonVariables_->queryString ) != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to perform an item query of archived items" );
									}

								break;

							case QUERY_REPLACED_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectInactiveItems = false;
									isSelectArchivedItems = false;
									isSelectDeletedItems = false;
									}
								else
									isSelectReplacedItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= strlen( queryString ) ||
								// End of query
								queryString[queryStringPosition_] == QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, commonVariables_->queryString ) != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to perform an item query of archived items" );
									}

								break;

							case QUERY_DELETED_CHAR:

								// Initially
								if( isSelectActiveItems &&
								isSelectInactiveItems &&
								isSelectArchivedItems &&
								isSelectReplacedItems &&
								isSelectDeletedItems )
									{
									isSelectActiveItems = false;
									isSelectInactiveItems = false;
									isSelectArchivedItems = false;
									isSelectReplacedItems = false;
									}
								else
									isSelectDeletedItems = true;

								queryStringPosition_++;

								if( queryStringPosition_ >= strlen( queryString ) ||
								// End of query
								queryString[queryStringPosition_] == QUERY_CHAR )
									{
									if( itemQuery( isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, isSuppressingMessage, commonVariables_->queryString ) != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to perform an item query of deleted items" );
									}

								break;

							case QUERY_COUNT_CHAR:

								isShowingCount = true;
								queryStringPosition_++;

								if( queryStringPosition_ >= strlen( queryString ) ||
								// End of query
								queryString[queryStringPosition_] == QUERY_CHAR )
									{
									if( itemQuery( true, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, NO_SENTENCE_NR, NO_ITEM_NR ) != RESULT_OK )
										return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query items" );
									}

								break;

							default:
								// Set query width parameter
								if( isdigit( queryString[queryStringPosition_] ) )
									{
									while( queryStringPosition_ < strlen( queryString ) &&
									isdigit( queryString[queryStringPosition_] ) &&
									queryWidth <= MAX_SENTENCE_NR / 10 )
										{
										queryWidth = ( queryWidth * 10 + queryString[queryStringPosition_] - '0' );
										queryStringPosition_++;
										}
									if( queryWidth >= NUMBER_OF_CONSOLE_COLUMNS )
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The query width in the query is too high" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an illegal character '", queryString[queryStringPosition_], "' in the query" );
							}
						}

					if( strlen( commonVariables_->queryString ) == 0 )
						{
						// No query performed yet
						if( isFirstInstruction )
							{
							if( itemQuery( true, isSelectActiveItems, isSelectInactiveItems, isSelectArchivedItems, isSelectReplacedItems, isSelectDeletedItems, false, NO_SENTENCE_NR, NO_ITEM_NR ) != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to query items" );
							}

						if( isShowingCount )
							{
							nTotal = countQuery();

							if( isSuppressingMessage )
								sprintf( commonVariables_->queryString, "%u", nTotal );
							else
								{
								if( nTotal == 0 )
									strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_ITEMS_WERE_FOUND ) );
								else
									{
									if( isNeedingToShowTotal() )
										sprintf( commonVariables_->queryString, "total:%u", nTotal );

									if( commonVariables_->nActiveQueryItems > 0 )
										{
										if( strlen( commonVariables_->queryString ) > 0 )
											strcat( commonVariables_->queryString, QUERY_SEPARATOR_SPACE_STRING );

										sprintf( commonVariables_->queryString, "%sactive:%u", commonVariables_->queryString, commonVariables_->nActiveQueryItems );
										}

									if( commonVariables_->nInactiveQueryItems > 0 )
										{
										if( strlen( commonVariables_->queryString ) > 0 )
											strcat( commonVariables_->queryString, QUERY_SEPARATOR_SPACE_STRING );

										sprintf( commonVariables_->queryString, "%sinactive:%u.\n", commonVariables_->queryString, commonVariables_->nInactiveQueryItems );
										}

									if( commonVariables_->nArchivedQueryItems > 0 )
										{
										if( strlen( commonVariables_->queryString ) > 0 )
											strcat( commonVariables_->queryString, QUERY_SEPARATOR_SPACE_STRING );

										sprintf( commonVariables_->queryString, "%sarchived:%u.\n", commonVariables_->queryString, commonVariables_->nArchivedQueryItems );
										}

									if( commonVariables_->nReplacedQueryItems > 0 )
										{
										if( strlen( commonVariables_->queryString ) > 0 )
											strcat( commonVariables_->queryString, QUERY_SEPARATOR_SPACE_STRING );

										sprintf( commonVariables_->queryString, "%sreplaced:%u.\n", commonVariables_->queryString, commonVariables_->nReplacedQueryItems );
										}

									if( commonVariables_->nDeletedQueryItems > 0 )
										{
										if( strlen( commonVariables_->queryString ) > 0 )
											strcat( commonVariables_->queryString, QUERY_SEPARATOR_SPACE_STRING );

										sprintf( commonVariables_->queryString, "%sdeleted:%u.\n", commonVariables_->queryString, commonVariables_->nDeletedQueryItems );
										}
									}
								}
							}
						else
							{
							commonVariables_->hasFoundQuery = false;

							if( showQueryResult( isOnlyShowingWords, isOnlyShowingWordReferences, isOnlyShowingStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth ) == RESULT_OK )
								{
								if( !isSuppressingMessage &&
								!commonVariables_->hasFoundQuery &&
								strlen( commonVariables_->queryString ) == 0 )
									{
									if( isOnlyShowingWords )
										strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORDS_WERE_FOUND ) );
									else
										{
										if( isOnlyShowingWordReferences )
											strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_WORD_REFERENCES_WERE_FOUND ) );
										else
											{
											if( isOnlyShowingStrings )
												strcat( commonVariables_->queryString, commonVariables_->currentLanguageWordItem->interfaceString( INTERFACE_QUERY_NO_STRINGS_WERE_FOUND ) );
											}
										}
									}
								}
							else
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to show the query result" );
							}
						}

					if( nTotalCount() == 0 ||	// Show comment on empty query
					commonVariables_->hasFoundQuery ||
					queryWidth > NO_CENTER_WIDTH )
						{
						if( isWritingQueryResult &&

						( queryWidth > NO_CENTER_WIDTH ||
						strlen( commonVariables_->queryString ) > 0 ) )
							{
							if( commonVariables_->presentation->writeText( ( !isSuppressingMessage && !commonVariables_->hasFoundQuery && queryWidth == NO_CENTER_WIDTH ), promptTypeNr, queryWidth, commonVariables_->queryString ) != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write the query result" );
							}
						}
					else
						strcpy( commonVariables_->queryString, EMPTY_STRING );
					}
				else
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given instruction string is empty or the given instruction string position is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given instruction string is undefined" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Honor the Lord, you heavenly beings;
 *	honor the Lord for his glory and strength." (Psalm 29:1)
 *************************************************************************/
