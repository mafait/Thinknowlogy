/*
 *	Class:			WordTypeList
 *	Parent class:	List
 *	Purpose:		To store word type items
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
#include "WordTypeItem.cpp"

class WordTypeList : private List
	{
	friend class WordItem;
	friend class WordType;

	// Private constructible variables

	WordTypeItem *foundWordTypeItem_;


	// Private deconstructor functions

	void deleteWordTypeList( WordTypeItem *searchItem )
		{
		WordTypeItem *deleteItem;

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordTypeItem();
			delete deleteItem;
			}
		}


	// Private functions

	void showWords( bool isReturnQueryToPosition, WordTypeItem *searchItem )
		{
		char *wordTypeString;
		char statusString[2] = SPACE_STRING;

		while( searchItem != NULL )
			{
			if( ( wordTypeString = searchItem->itemString() ) != NULL )
				{
				if( commonVariables()->hasFoundQuery ||
				strlen( commonVariables()->queryString ) > 0 )
					strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

				commonVariables()->hasFoundQuery = true;

				if( !searchItem->isActiveItem() )	// Don't show status with active items
					{
					statusString[0] = searchItem->statusChar();
					strcat( commonVariables()->queryString, statusString );
					}

				strcat( commonVariables()->queryString, wordTypeString );
				}

			searchItem = searchItem->nextWordTypeItem();
			}
		}

	WordTypeItem *wordTypeString( bool isAllowingSingularOrPluralNoun, bool isCheckingAllLanguages, unsigned short wordTypeNr, WordTypeItem *searchItem )
		{
		while( searchItem != NULL )
			{
			if( wordTypeNr == WORD_TYPE_UNDEFINED ||
			searchItem->wordTypeNr() == wordTypeNr ||

			( isAllowingSingularOrPluralNoun &&
			searchItem->isNoun() &&
			searchItem->isSingularOrPluralNoun( wordTypeNr ) ) )
				return searchItem;

			if( foundWordTypeItem_ == NULL )
				foundWordTypeItem_ = searchItem;

			searchItem = ( isCheckingAllLanguages ? searchItem->nextWordTypeItem() : searchItem->nextCurrentLanguageWordTypeItem() );
			}

		return NULL;
		}

	WordTypeItem *firstActiveWordTypeItem()
		{
		return (WordTypeItem *)firstActiveItem();
		}

	WordTypeItem *firstReplacedWordTypeItem()
		{
		return (WordTypeItem *)firstReplacedItem();
		}

	WordTypeItem *firstDeletedWordTypeItem()
		{
		return (WordTypeItem *)firstDeletedItem();
		}

	WordTypeItem *firstActiveCurrentLanguageWordTypeItem()
		{
		unsigned short currentGrammarLanguageNr = commonVariables()->currentGrammarLanguageNr;
		WordTypeItem *searchItem = firstActiveWordTypeItem();

		while( searchItem != NULL &&
		searchItem->wordTypeLanguageNr() < currentGrammarLanguageNr )
			searchItem = searchItem->nextWordTypeItem();

		return ( searchItem != NULL && searchItem->wordTypeLanguageNr() == currentGrammarLanguageNr ? searchItem : NULL );
		}

	WordTypeItem *firstReplacedCurrentLanguageWordTypeItem()
		{
		unsigned short currentGrammarLanguageNr = commonVariables()->currentGrammarLanguageNr;
		WordTypeItem *searchItem = firstReplacedWordTypeItem();

		while( searchItem != NULL &&
		searchItem->wordTypeLanguageNr() < currentGrammarLanguageNr )
			searchItem = searchItem->nextWordTypeItem();

		return ( searchItem != NULL && searchItem->wordTypeLanguageNr() == currentGrammarLanguageNr ? searchItem : NULL );
		}

	WordTypeItem *firstDeletedCurrentLanguageWordTypeItem()
		{
		unsigned short currentGrammarLanguageNr = commonVariables()->currentGrammarLanguageNr;
		WordTypeItem *searchItem = firstDeletedWordTypeItem();

		while( searchItem != NULL &&
		searchItem->wordTypeLanguageNr() < currentGrammarLanguageNr )
			searchItem = searchItem->nextWordTypeItem();

		return ( searchItem != NULL && searchItem->wordTypeLanguageNr() == currentGrammarLanguageNr ? searchItem : NULL );
		}


	protected:
	// Constructor / deconstructor

	WordTypeList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		foundWordTypeItem_ = NULL;

		initializeListVariables( WORD_TYPE_LIST_SYMBOL, "WordTypeList", commonVariables, myWordItem );
		}

	~WordTypeList()
		{
		deleteWordTypeList( firstActiveWordTypeItem() );
		deleteWordTypeList( firstReplacedWordTypeItem() );
		deleteWordTypeList( firstDeletedWordTypeItem() );
		}


	// Protected functions

	void showWords( bool isReturnQueryToPosition )
		{
		showWords( isReturnQueryToPosition, firstActiveWordTypeItem() );
		showWords( isReturnQueryToPosition, firstReplacedWordTypeItem() );
		showWords( isReturnQueryToPosition, firstDeletedWordTypeItem() );
		}

	void clearGeneralizationWriteLevel( bool isLanguageWord, unsigned short currentWriteLevel )
		{
		WordTypeItem *searchItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );

		while( searchItem != NULL )
			{
			searchItem->clearGeneralizationWriteLevel( currentWriteLevel );
			searchItem = ( isLanguageWord ? searchItem->nextWordTypeItem() : searchItem->nextCurrentLanguageWordTypeItem() );
			}
		}

	void clearSpecificationWriteLevel( unsigned short currentWriteLevel )
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			searchItem->clearSpecificationWriteLevel( currentWriteLevel );
			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}
		}

	void clearRelationWriteLevel( unsigned short currentWriteLevel )
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			searchItem->clearRelationWriteLevel( currentWriteLevel );
			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}
		}

	bool hasFeminineWordEnding( unsigned short wordTypeNr )
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasFeminineWordEnding() &&
			searchItem->wordTypeNr() == wordTypeNr )
				return true;

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	bool hasMasculineWordEnding( unsigned short wordTypeNr )
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasMasculineWordEnding() &&
			searchItem->wordTypeNr() == wordTypeNr )
				return true;

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	bool hasMultipleWordType()
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->isMultipleWord() )
				return true;

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, void *authorizationKey )
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->wordTypeNr() == wordTypeNr &&
			searchItem->isCorrectHiddenWordType( compareString, authorizationKey ) )
				return true;

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	bool isMultipleWord( unsigned short wordTypeNr )
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->isMultipleWord() &&
			searchItem->wordTypeNr() == wordTypeNr )
				return true;

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	ResultType checkWordTypesOnFeminineParameters()
		{
		unsigned short interfaceParameter;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordTypesOnFeminineParameters";

		while( searchItem != NULL )
			{
			interfaceParameter = NO_INTERFACE_PARAMETER;

			if( searchItem->hasFeminineDefiniteArticleParameter() &&
			searchItem->hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchItem->hasFeminineIndefiniteArticleParameter() &&
				searchItem->hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > NO_INTERFACE_PARAMETER )
				{
				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an interface notification" );
				}

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return RESULT_OK;
		}

	ResultType checkWordTypesOnMasculineParameters()
		{
		unsigned short interfaceParameter;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordTypesOnMasculineParameters";

		while( searchItem != NULL )
			{
			interfaceParameter = NO_INTERFACE_PARAMETER;

			if( searchItem->hasMasculineDefiniteArticleParameter() &&
			searchItem->hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchItem->hasMasculineIndefiniteArticleParameter() &&
				searchItem->hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > NO_INTERFACE_PARAMETER )
				{
				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to write an interface notification" );
				}

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return RESULT_OK;
		}

	ResultType deleteWordType( unsigned short wordTypeNr )
		{
		bool hasFoundWordType = false;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteWordType";

		while( searchItem != NULL &&
		!hasFoundWordType )
			{
			if( searchItem->wordTypeNr() == wordTypeNr )
				{
				if( deleteItem( false, searchItem ) == RESULT_OK )
					hasFoundWordType = true;
				else
					return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to delete an active item" );
				}
			else
				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		if( !hasFoundWordType )
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I coundn't find the given word type" );

		return RESULT_OK;
		}

	ResultType hideWordTypeItem( unsigned short wordTypeNr, void *authorizationKey )
		{
		bool hasFoundWordType = false;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "hideWordTypeItem";

		while( searchItem != NULL &&
		!hasFoundWordType )
			{
			if( searchItem->wordTypeNr() == wordTypeNr )
				{
				if( searchItem->hideWordType( authorizationKey ) == RESULT_OK )
					hasFoundWordType = true;
				else
					return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to hide a word type" );
				}
			else
				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		if( !hasFoundWordType )
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I coundn't find the given word type" );

		return RESULT_OK;
		}

	ResultType markGeneralizationWordTypeAsWritten( bool isLanguageWord, unsigned short wordTypeNr )
		{
		bool hasFoundWordTypeNr = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );
		char functionNameString[FUNCTION_NAME_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isGeneralizationWordAlreadyWritten() )
					{
					if( searchItem->isSingularNoun() )
						singularNounWordTypeItem = searchItem;
					else
						{
						if( searchItem->isPluralNoun() )
							pluralNounWordTypeItem = searchItem;
						}

					if( searchItem->wordTypeNr() == wordTypeNr )
						{
						if( searchItem->markGeneralizationWordTypeAsWritten() == RESULT_OK )
							hasFoundWordTypeNr = true;
						else
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a word as written" );
						}
					}

				searchItem = ( isLanguageWord ? searchItem->nextWordTypeItem() : searchItem->nextCurrentLanguageWordTypeItem() );
				}

			if( hasFoundWordTypeNr )
				{
				if( wordTypeNr == WORD_TYPE_NOUN_SINGULAR &&	// If singular noun - also set plural noun
				pluralNounWordTypeItem != NULL )
					{
					if( pluralNounWordTypeItem->markGeneralizationWordTypeAsWritten() != RESULT_OK )
						return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a plural noun word as written" );
					}
				else
					{
					if( wordTypeNr == WORD_TYPE_NOUN_PLURAL &&	// If plural noun - also set singular noun
					singularNounWordTypeItem != NULL )
						{
						if( singularNounWordTypeItem->markGeneralizationWordTypeAsWritten() != RESULT_OK )
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a singular noun word as written" );
						}
					}
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find the given word type number: ", wordTypeNr );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given word type number is undefined or out of bounds" );

		return RESULT_OK;
		}

	ResultType markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		bool hasFoundWordTypeNr = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isSpecificationWordAlreadyWritten() )
					{
					if( searchItem->isSingularNoun() )
						singularNounWordTypeItem = searchItem;
					else
						{
						if( searchItem->isPluralNoun() )
							pluralNounWordTypeItem = searchItem;
						}

					if( searchItem->wordTypeNr() == wordTypeNr )
						{
						if( searchItem->markSpecificationWordTypeAsWritten() == RESULT_OK )
							hasFoundWordTypeNr = true;
						else
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a word as written" );
						}
					}

				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
				}

			if( hasFoundWordTypeNr )
				{
				if( wordTypeNr == WORD_TYPE_NOUN_SINGULAR &&	// If singular noun - also set plural noun
				pluralNounWordTypeItem != NULL )
					{
					if( pluralNounWordTypeItem->markSpecificationWordTypeAsWritten() != RESULT_OK )
						return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a plural noun word as written" );
					}
				else
					{
					if( wordTypeNr == WORD_TYPE_NOUN_PLURAL &&	// If plural noun - also set singular noun
					singularNounWordTypeItem != NULL )
						{
						if( singularNounWordTypeItem->markSpecificationWordTypeAsWritten() != RESULT_OK )
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a singular noun word as written" );
						}
					}
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find the given word type number: ", wordTypeNr );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given word type number is undefined or out of bounds" );

		return RESULT_OK;
		}

	ResultType markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		bool hasFoundWordTypeNr = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != NULL )
				{
				if( !searchItem->isRelationWordAlreadyWritten() )
					{
					if( searchItem->isSingularNoun() )
						singularNounWordTypeItem = searchItem;
					else
						{
						if( searchItem->isPluralNoun() )
							pluralNounWordTypeItem = searchItem;
						}

					if( searchItem->wordTypeNr() == wordTypeNr )
						{
						if( searchItem->markRelationWordTypeAsWritten() == RESULT_OK )
							hasFoundWordTypeNr = true;
						else
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a word as written" );
						}
					}

				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
				}

			if( hasFoundWordTypeNr )
				{
				// If singular noun - also set plural noun
				if( wordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
				pluralNounWordTypeItem != NULL )
					{
					if( pluralNounWordTypeItem->markRelationWordTypeAsWritten() != RESULT_OK )
						return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a plural noun word as written" );
					}
				else
					{
					// If plural noun - also set singular noun
					if( wordTypeNr == WORD_TYPE_NOUN_PLURAL &&
					singularNounWordTypeItem != NULL )
						{
						if( singularNounWordTypeItem->markRelationWordTypeAsWritten() != RESULT_OK )
							return addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to mark a singular noun word as written" );
						}
					}
				}
			else
				return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I couldn't find the given word type number: ", wordTypeNr );
			}
		else
			return startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given word type number is undefined or out of bounds" );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDataBase()
		{
		// Not fully implemented yet
		WordTypeItem *searchItem = firstActiveWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDataBase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeWordTypeItemInFutureDataBase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to store a word type item in the database" );
				}

			searchItem = searchItem->nextWordTypeItem();
			}

		searchItem = firstReplacedWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeWordTypeItemInFutureDataBase() != RESULT_OK )
					return addError( functionNameString, NULL, NULL, "I failed to modify a replaced word type item in the database" );
				}

			searchItem = searchItem->nextWordTypeItem();
			}

		return RESULT_OK;
		}
*/
	ReferenceResultType findMatchingWordReferenceString( char *searchString )
		{
		ReferenceResultType referenceResult;
		WordTypeItem *searchItem = firstActiveWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		while( commonVariables()->result == RESULT_OK &&
		!referenceResult.hasFoundMatchingStrings &&
		searchItem != NULL )
			{
			if( searchItem->itemString() != NULL )
				{
				if( ( referenceResult = compareStrings( searchString, searchItem->itemString() ) ).result == RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						commonVariables()->matchingWordTypeNr = searchItem->wordTypeNr();
					}
				else
					addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to compare an active word type string with the query string" );
				}

			searchItem = searchItem->nextWordTypeItem();
			}

		searchItem = firstReplacedWordTypeItem();

		while( commonVariables()->result == RESULT_OK &&
		!referenceResult.hasFoundMatchingStrings &&
		searchItem != NULL )
			{
			if( searchItem->itemString() != NULL )
				{
				if( ( referenceResult = compareStrings( searchString, searchItem->itemString() ) ).result == RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						commonVariables()->matchingWordTypeNr = searchItem->wordTypeNr();
					}
				else
					addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to compare a deleted word type string with the query string" );
				}

			searchItem = searchItem->nextWordTypeItem();
			}

		searchItem = firstDeletedWordTypeItem();

		while( commonVariables()->result == RESULT_OK &&
		!referenceResult.hasFoundMatchingStrings &&
		searchItem != NULL )
			{
			if( searchItem->itemString() != NULL )
				{
				if( ( referenceResult = compareStrings( searchString, searchItem->itemString() ) ).result == RESULT_OK )
					{
					if( referenceResult.hasFoundMatchingStrings )
						commonVariables()->matchingWordTypeNr = searchItem->wordTypeNr();
					}
				else
					addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to compare a deleted word type string with the query string" );
				}

			searchItem = searchItem->nextWordTypeItem();
			}

		referenceResult.result = commonVariables()->result;
		return referenceResult;
		}

	WordResultType createWordTypeItem( bool hasFeminineWordEnding, bool hasMasculineWordEnding, bool isMultipleWord, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordTypeItem";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			if( wordTypeString != NULL )
				{
				if( commonVariables()->currentItemNr < MAX_ITEM_NR )
					{
					if( ( wordResult.createdWordTypeItem = new WordTypeItem( hasFeminineWordEnding, hasMasculineWordEnding, isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, commonVariables()->currentGrammarLanguageNr, wordTypeNr, wordLength, wordTypeString, commonVariables(), this, myWordItem() ) ) != NULL )
						{
						if( addItemToList( QUERY_ACTIVE_CHAR, wordResult.createdWordTypeItem ) != RESULT_OK )
							addError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "I failed to add an active word type item" );
						}
					else
						startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The created word type item is undefined" );
					}
				else
					startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The current item number is undefined" );
				}
			else
				startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given wordTypeString is undefined" );
			}
		else
			startError( functionNameString, NULL, myWordItem()->anyWordTypeString(), "The given word type number is undefined or out of bounds" );

		wordResult.result = commonVariables()->result;
		return wordResult;
		}

	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		foundWordTypeItem_ = NULL;

		// Try to find word type from the current language
		if( ( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == NULL )
			{
			if( ( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstReplacedCurrentLanguageWordTypeItem() ) ) == NULL )
				{
				if( ( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstDeletedCurrentLanguageWordTypeItem() ) ) == NULL )
					{
					// Not found in current language. Now, try all languages
					if( isCheckingAllLanguages &&
					( foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() ) ) == NULL )
						{
						if( ( foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstReplacedWordTypeItem() ) ) == NULL )
							foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstDeletedWordTypeItem() );
						}
					}
				}
			}

		return ( foundWordTypeItem == NULL ? ( foundWordTypeItem_ == NULL ? NULL : foundWordTypeItem_->itemString() ) : foundWordTypeItem->itemString() );
		}

	char *activeWordTypeString( bool isLanguageWord, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		if( ( foundWordTypeItem = activeWordTypeItem( false, isLanguageWord, wordTypeNr ) ) != NULL )
			return foundWordTypeItem->itemString();

		return NULL;
		}

	WordTypeItem *activeWordTypeItem( bool isAllowingSingularOrPluralNoun, bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		// Try to find word type from the current language
		if( ( foundWordTypeItem = wordTypeString( isAllowingSingularOrPluralNoun, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == NULL )
			{
			// Not found in current language. Now, try all languages
			if( isCheckingAllLanguages )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() );
			}

		return foundWordTypeItem;
		}
	};

/*************************************************************************
 *
 *	"He lifts the poor from the dust
 *	and needy from the garbage dump.
 *	He sets them among princes,
 *	even princes of his own people!" (Psalm 113:7-8)
 *
 *************************************************************************/
