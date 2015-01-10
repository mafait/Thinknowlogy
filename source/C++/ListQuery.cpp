/*
 *	Class:			ListQuery
 *	Supports class:	List
 *	Purpose:		To process queries
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

#include "List.h"
#include "Presentation.cpp"

class ListQuery
	{
	friend class List;

	// Private constructible variables

	CommonVariables *commonVariables_;
	List *myList_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void itemQuery( bool isSelectOnFind, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( ( isReferenceQuery &&
			queryItem->hasFoundReferenceItemById( querySentenceNr, queryItemNr ) ) ||

			( !isReferenceQuery &&
			( querySentenceNr == NO_SENTENCE_NR ||
			querySentenceNr == queryItem->creationSentenceNr() ) &&

			( queryItemNr == NO_SENTENCE_NR ||
			queryItemNr == queryItem->itemNr() ) ) )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void listQuery( bool isSelectOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectOnFind )
				{
				if( !queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void wordTypeQuery( bool isSelectOnFind, unsigned short queryWordTypeNr, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasFoundWordType( queryWordTypeNr ) )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void parameterQuery( bool isSelectOnFind, unsigned int queryParameter, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( queryItem->hasFoundParameter( queryParameter ) )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void wordQuery( bool isSelectOnFind, Item *queryItem )
		{
		while( queryItem != NULL )
			{
			if( isSelectOnFind )
				{
				if( !queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}
		}

	void clearQuerySelections( Item *searchItem )
		{
		while( searchItem != NULL )
			{
			searchItem->isSelectedByQuery = false;
			searchItem = searchItem->nextItem;
			}
		}

	ResultType wordReferenceQuery( bool isSelectOnFind, char *wordReferenceNameString, Item *queryItem )
		{
		ReferenceResultType referenceResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQuery";

		while( queryItem != NULL )
			{
			if( ( referenceResult = queryItem->findMatchingWordReferenceString( wordReferenceNameString ) ).result == RESULT_OK )
				{
				if( referenceResult.hasFoundMatchingStrings )
					{
					if( isSelectOnFind &&
					!queryItem->isSelectedByQuery )
						{
						commonVariables_->hasFoundQuery = true;
						queryItem->isSelectedByQuery = true;
						}
					}
				else
					{
					if( !isSelectOnFind &&
					queryItem->isSelectedByQuery )
						queryItem->isSelectedByQuery = false;
					}

				queryItem = queryItem->nextItem;
				}
			else
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to check the word references" );
			}

		return RESULT_OK;
		}

	ResultType stringQuery( bool isSelectOnFind, char *wordString, Item *queryItem )
		{
		ReferenceResultType referenceResult;
		bool hasFoundString;
		char *itemString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQuery";

		while( queryItem != NULL )
			{
			hasFoundString = false;

			if( ( itemString = queryItem->itemString() ) != NULL )
				{
				if( ( referenceResult = compareStrings( wordString, itemString ) ).result == RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						hasFoundString = true;
					}
				else
					return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to compare two strings" );
				}

			if( !hasFoundString &&
			( itemString = queryItem->extraItemString() ) != NULL )
				{
				if( ( referenceResult = compareStrings( wordString, itemString ) ).result == RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						hasFoundString = true;
					}
				else
					return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to compare two strings" );
				}

			if( hasFoundString )
				{
				if( isSelectOnFind &&
				!queryItem->isSelectedByQuery )
					{
					commonVariables_->hasFoundQuery = true;
					queryItem->isSelectedByQuery = true;
					}
				}
			else
				{
				if( !isSelectOnFind &&
				queryItem->isSelectedByQuery )
					queryItem->isSelectedByQuery = false;
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}

	ResultType showQueryResult( bool showOnlyWords, bool showOnlyWordReferences, bool showOnlyStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth, Item *queryItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "showQueryResult";

		while( queryItem != NULL )
			{
			if( queryItem->isSelectedByQuery )
				{
				if( showOnlyWords )
					queryItem->showWords( isReturnQueryToPosition, queryWordTypeNr );
				else
					{
					if( showOnlyWordReferences )
						queryItem->showWordReferences( isReturnQueryToPosition );
					else
						{
						if( showOnlyStrings )
							queryItem->showString( isReturnQueryToPosition );
						else
							{
							if( commonVariables_->presentation->writeText( true, promptTypeNr, queryWidth, queryItem->toString( queryWordTypeNr ) ) != RESULT_OK )
								return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to show the info of an active item" );
							}
						}
					}
				}

			queryItem = queryItem->nextItem;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	ListQuery( CommonVariables *commonVariables, List *myList )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		commonVariables_ = commonVariables;
		myList_ = myList;
		strcpy( moduleNameString_, "ListQuery" );

		if( commonVariables_ != NULL )
			{
		if( myList_ == NULL )
			strcpy( errorString, "The given my list is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myList_ != NULL &&
			myList_->myWordItem() != NULL )
				myList_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	void countQuery()
		{
		Item *searchItem = myList_->firstActiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nActiveQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstInactiveItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nInactiveQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstArchivedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nArchivedQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstReplacedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nReplacedQueryItems++;

			searchItem = searchItem->nextItem;
			}

		searchItem = myList_->firstDeletedItem();

		while( searchItem != NULL )
			{
			if( searchItem->isSelectedByQuery )
				commonVariables_->nDeletedQueryItems++;

			searchItem = searchItem->nextItem;
			}
		}

	void clearQuerySelections()
		{
		clearQuerySelections( myList_->firstActiveItem() );
		clearQuerySelections( myList_->firstInactiveItem() );
		clearQuerySelections( myList_->firstArchivedItem() );
		clearQuerySelections( myList_->firstReplacedItem() );
		clearQuerySelections( myList_->firstDeletedItem() );
		}

	void itemQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, bool isReferenceQuery, unsigned int querySentenceNr, unsigned int queryItemNr )
		{
		if( isSelectActiveItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_->firstActiveItem() );

		if( isSelectInactiveItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_->firstInactiveItem() );

		if( isSelectArchivedItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_->firstArchivedItem() );

		if( isSelectReplacedItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_->firstReplacedItem() );

		if( isSelectDeletedItems )
			itemQuery( isSelectOnFind, isReferenceQuery, querySentenceNr, queryItemNr, myList_->firstDeletedItem() );
		}

	void listQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems )
		{
		if( isSelectActiveItems )
			listQuery( isSelectOnFind, myList_->firstActiveItem() );

		if( isSelectInactiveItems )
			listQuery( isSelectOnFind, myList_->firstInactiveItem() );

		if( isSelectArchivedItems )
			listQuery( isSelectOnFind, myList_->firstArchivedItem() );

		if( isSelectReplacedItems )
			listQuery( isSelectOnFind, myList_->firstReplacedItem() );

		if( isSelectDeletedItems )
			listQuery( isSelectOnFind, myList_->firstDeletedItem() );
		}

	void wordTypeQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned short queryWordTypeNr )
		{
		if( isSelectActiveItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_->firstActiveItem() );

		if( isSelectInactiveItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_->firstInactiveItem() );

		if( isSelectArchivedItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_->firstArchivedItem() );

		if( isSelectReplacedItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_->firstReplacedItem() );

		if( isSelectDeletedItems )
			wordTypeQuery( isSelectOnFind, queryWordTypeNr, myList_->firstDeletedItem() );
		}

	void parameterQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, unsigned int queryParameter )
		{
		if( isSelectActiveItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_->firstActiveItem() );

		if( isSelectInactiveItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_->firstInactiveItem() );

		if( isSelectArchivedItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_->firstArchivedItem() );

		if( isSelectReplacedItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_->firstReplacedItem() );

		if( isSelectDeletedItems )
			parameterQuery( isSelectOnFind, queryParameter, myList_->firstDeletedItem() );
		}

	void wordQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems )
		{
		if( isSelectActiveItems )
			wordQuery( isSelectOnFind, myList_->firstActiveItem() );

		if( isSelectInactiveItems )
			wordQuery( isSelectOnFind, myList_->firstInactiveItem() );

		if( isSelectArchivedItems )
			wordQuery( isSelectOnFind, myList_->firstArchivedItem() );

		if( isSelectReplacedItems )
			wordQuery( isSelectOnFind, myList_->firstReplacedItem() );

		if( isSelectDeletedItems )
			wordQuery( isSelectOnFind, myList_->firstDeletedItem() );
		}

	ReferenceResultType compareStrings( char *searchString, char *sourceString )
		{
		ReferenceResultType referenceResult;
		bool isStop;
		size_t searchStringPosition = 0;
		size_t sourceStringPosition = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "compareStrings";

		if( searchString != NULL )
			{
			if( sourceString != NULL )
				{
				if( searchString != sourceString )
					{
					referenceResult.hasFoundMatchingStrings = true;

					while( commonVariables_->result == RESULT_OK &&
					referenceResult.hasFoundMatchingStrings &&
					searchStringPosition < strlen( searchString ) &&
					sourceStringPosition < strlen( sourceString ) )
						{
						if( searchString[searchStringPosition] == SYMBOL_QUESTION_MARK ||
						searchString[searchStringPosition] == sourceString[sourceStringPosition] )
							{
							searchStringPosition++;
							sourceStringPosition++;
							}
						else
							{
							if( searchString[searchStringPosition] == SYMBOL_ASTERISK )
								{
								if( ++searchStringPosition < strlen( searchString ) )
									{
									isStop = false;

									while( !isStop &&
									sourceStringPosition < strlen( sourceString ) )
										{
										if( searchString[searchStringPosition] == sourceString[sourceStringPosition] )
											{
											// Check remaining strings
											if( ( referenceResult = compareStrings( &searchString[searchStringPosition], &sourceString[sourceStringPosition] ) ).result == RESULT_OK )
												{
												if( referenceResult.hasFoundMatchingStrings )
													{
													isStop = true;
													searchStringPosition++;
													}
												else
													referenceResult.hasFoundMatchingStrings = true;	// Reset indicator

												sourceStringPosition++;
												}
											else
												myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to compare the remaining strings" );
											}
										else
											sourceStringPosition++;					// Skip source characters when not equal
										}
									}
								else
									sourceStringPosition = strlen( sourceString );	// Empty source string after asterisk
								}
							else
								referenceResult.hasFoundMatchingStrings = false;
							}
						}

					if( commonVariables_->result == RESULT_OK )
						{
						if( referenceResult.hasFoundMatchingStrings &&
						sourceStringPosition == strlen( sourceString ) )
							{
							// Check search string for extra asterisks
							while( searchStringPosition < strlen( searchString ) &&
							searchString[searchStringPosition] == SYMBOL_ASTERISK )
								searchStringPosition++;		// Skip extra asterisks
							}

						if( searchStringPosition < strlen( searchString ) ||
						sourceStringPosition < strlen( sourceString ) )
							referenceResult.hasFoundMatchingStrings = false;
						}
					}
				else
					myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given strings are the same string" );
				}
			else
				myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given source string is undefined" );
			}
		else
			myList_->startError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "The given search string is undefined" );

		referenceResult.result = commonVariables_->result;
		return referenceResult;
		}

	ResultType wordReferenceQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordReferenceNameString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "wordReferenceQuery";

		if( isSelectActiveItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_->firstActiveItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to check the word references of an active word" );
			}

		if( isSelectInactiveItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_->firstInactiveItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to check the word references of an inactive word" );
			}

		if( isSelectArchivedItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_->firstArchivedItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to check the word references of an archived word" );
			}

		if( isSelectReplacedItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_->firstReplacedItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to check the word references of a replaced word" );
			}

		if( isSelectDeletedItems )
			{
			if( wordReferenceQuery( isSelectOnFind, wordReferenceNameString, myList_->firstDeletedItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to check the word references of a deleted word" );
			}

		return RESULT_OK;
		}

	ResultType stringQuery( bool isSelectOnFind, bool isSelectActiveItems, bool isSelectInactiveItems, bool isSelectArchivedItems, bool isSelectReplacedItems, bool isSelectDeletedItems, char *wordString )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "stringQuery";

		if( isSelectActiveItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_->firstActiveItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to string query an active word" );
			}

		if( isSelectInactiveItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_->firstInactiveItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to string query an inactive word" );
			}

		if( isSelectArchivedItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_->firstArchivedItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to string query an archived word" );
			}

		if( isSelectReplacedItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_->firstReplacedItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to string query a replaced word" );
			}

		if( isSelectDeletedItems )
			{
			if( stringQuery( isSelectOnFind, wordString, myList_->firstDeletedItem() ) != RESULT_OK )
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to string query a deleted word" );
			}

		return RESULT_OK;
		}

	ResultType showQueryResult( bool showOnlyWords, bool showOnlyWordReferences, bool showOnlyStrings, bool isReturnQueryToPosition, unsigned short promptTypeNr, unsigned short queryWordTypeNr, size_t queryWidth )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "showQueryResult";

		if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_->firstActiveItem() ) == RESULT_OK )
			{
			if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_->firstInactiveItem() ) == RESULT_OK )
				{
				if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_->firstArchivedItem() ) == RESULT_OK )
					{
					if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_->firstReplacedItem() ) == RESULT_OK )
						{
						if( showQueryResult( showOnlyWords, showOnlyWordReferences, showOnlyStrings, isReturnQueryToPosition, promptTypeNr, queryWordTypeNr, queryWidth, myList_->firstDeletedItem() ) != RESULT_OK )
							return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to show the query result of a deleted word" );
						}
					else
						return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to show the query result of a replaced word" );
					}
				else
					return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to show the query result of an archived word" );
				}
			else
				return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to show the query result of an inactive word" );
			}
		else
			return myList_->addError( functionNameString, moduleNameString_, myList_->myWordItem()->anyWordTypeString(), "I failed to show the query result of an active word" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"Your name, O Lord, endures forever;
 *	your name, O Lord, is known to every generation." (Psalm 135:13)
 *
 *************************************************************************/
