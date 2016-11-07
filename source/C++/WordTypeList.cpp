/*	Class:			WordTypeList
 *	Parent class:	List
 *	Purpose:		To store word type items
 *	Version:		Thinknowlogy 2016r2 (Restyle)
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
#include "WordTypeItem.cpp"

class WordTypeList : private List
	{
	friend class WordItem;
	friend class WordType;

	// Private constructed variables

	WordTypeItem *foundWordTypeItem_;


	// Private functions
	WordTypeItem *wordTypeString( bool isAllowingDifferentNoun, bool isCheckingAllLanguages, unsigned short wordTypeNr, WordTypeItem *searchWordTypeItem )
		{
		while( searchWordTypeItem != NULL )
			{
			if( wordTypeNr == NO_WORD_TYPE_NR ||
			searchWordTypeItem->wordTypeNr() == wordTypeNr ||

			( isAllowingDifferentNoun &&
			searchWordTypeItem->isNoun() ) )
				return searchWordTypeItem;

			if( foundWordTypeItem_ == NULL )
				foundWordTypeItem_ = searchWordTypeItem;

			searchWordTypeItem = ( isCheckingAllLanguages ? searchWordTypeItem->nextWordTypeItem() : searchWordTypeItem->nextCurrentLanguageWordTypeItem() );
			}

		return NULL;
		}

	WordTypeItem *firstActiveWordTypeItem()
		{
		return (WordTypeItem *)firstActiveItem();
		}

	WordTypeItem *firstDeletedWordTypeItem()
		{
		return (WordTypeItem *)firstDeletedItem();
		}

	WordTypeItem *firstActiveCurrentLanguageWordTypeItem()
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();

		while( searchWordTypeItem != NULL &&
		searchWordTypeItem->wordTypeLanguageNr() < currentLanguageNr )
			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();

		return ( searchWordTypeItem != NULL && searchWordTypeItem->wordTypeLanguageNr() == currentLanguageNr ? searchWordTypeItem : NULL );
		}

	WordTypeItem *firstDeletedCurrentLanguageWordTypeItem()
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		WordTypeItem *searchWordTypeItem = firstDeletedWordTypeItem();

		while( searchWordTypeItem != NULL &&
		searchWordTypeItem->wordTypeLanguageNr() < currentLanguageNr )
			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();

		return ( searchWordTypeItem != NULL && searchWordTypeItem->wordTypeLanguageNr() == currentLanguageNr ? searchWordTypeItem : NULL );
		}


	protected:
	// Constructor

	WordTypeList( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		foundWordTypeItem_ = NULL;

		initializeListVariables( WORD_TYPE_LIST_SYMBOL, "WordTypeList", commonVariables, myWordItem );
		}

	~WordTypeList()
		{
		WordTypeItem *deleteWordTypeItem;
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();

		while( searchWordTypeItem != NULL )
			{
			deleteWordTypeItem = searchWordTypeItem;
			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			delete deleteWordTypeItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WordTypeList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class WordTypeList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class WordTypeList has replaced items." );

		searchWordTypeItem = firstDeletedWordTypeItem();

		while( searchWordTypeItem != NULL )
			{
			deleteWordTypeItem = searchWordTypeItem;
			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			delete deleteWordTypeItem;
			}
		}


	// Protected functions

	void clearGeneralizationWriteLevel( bool isLanguageWord, unsigned short currentWriteLevel )
		{
		WordTypeItem *searchWordTypeItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );

		while( searchWordTypeItem != NULL )
			{
			searchWordTypeItem->clearGeneralizationWriteLevel( currentWriteLevel );
			searchWordTypeItem = ( isLanguageWord ? searchWordTypeItem->nextWordTypeItem() : searchWordTypeItem->nextCurrentLanguageWordTypeItem() );
			}
		}

	void clearSpecificationWriteLevel( unsigned short currentWriteLevel )
		{
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != NULL )
			{
			searchWordTypeItem->clearSpecificationWriteLevel( currentWriteLevel );
			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}
		}

	void clearRelationWriteLevel( unsigned short currentWriteLevel )
		{
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != NULL )
			{
			searchWordTypeItem->clearRelationWriteLevel( currentWriteLevel );
			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}
		}

	bool hasAnyWordType()
		{
		return ( firstActiveWordTypeItem() != NULL );
		}

	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, void *authorizationKey )
		{
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != NULL )
			{
			if( searchWordTypeItem->isCorrectHiddenWordType( wordTypeNr, compareString, authorizationKey ) )
				return true;

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		return false;
		}

	ResultType checkWordTypesOnFeminineParameters()
		{
		unsigned short interfaceParameter;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordTypesOnFeminineParameters";

		while( searchWordTypeItem != NULL )
			{
			interfaceParameter = NO_INTERFACE_PARAMETER;

			if( searchWordTypeItem->hasFeminineDefiniteArticleParameter() &&
			searchWordTypeItem->hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchWordTypeItem->hasFeminineIndefiniteArticleParameter() &&
				searchWordTypeItem->hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > NO_INTERFACE_PARAMETER )
				{
				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchWordTypeItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to write an interface notification about the use of an article with a feminine noun" );
				}

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		return RESULT_OK;
		}

	ResultType checkWordTypesOnMasculineParameters()
		{
		unsigned short interfaceParameter;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkWordTypesOnMasculineParameters";

		while( searchWordTypeItem != NULL )
			{
			interfaceParameter = NO_INTERFACE_PARAMETER;

			if( searchWordTypeItem->hasMasculineDefiniteArticleParameter() &&
			searchWordTypeItem->hasFeminineAndMasculineDefiniteArticle() )
				interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchWordTypeItem->hasMasculineIndefiniteArticleParameter() &&
				searchWordTypeItem->hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > NO_INTERFACE_PARAMETER )
				{
				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchWordTypeItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to write an interface notification about the use of an article with a masculine noun" );
				}

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		return RESULT_OK;
		}

	ResultType deleteWordType( WordTypeItem *deleteWordTypeItem )
		{
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteWordType";

		if( deleteWordTypeItem == NULL )
			return startError( functionNameString, NULL, "The given delete word type item is undefined" );

		if( deleteItem( searchWordTypeItem ) != RESULT_OK )
			return addError( functionNameString, NULL, "I failed to delete an active item" );

		return RESULT_OK;
		}

	ResultType deleteAllWordTypesOfCurrentSentence()
		{
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "deleteAllWordTypesOfCurrentSentence";

		while( searchWordTypeItem != NULL )
			{
			if( searchWordTypeItem->hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( searchWordTypeItem ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to delete a word type item of this sentence" );

				searchWordTypeItem = nextWordTypeListItem();
				}
			else
				searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			}

		return RESULT_OK;
		}

	ResultType hideWordTypeItem( unsigned short wordTypeNr, void *authorizationKey )
		{
		bool hasWordType = false;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "hideWordTypeItem";

		while( searchWordTypeItem != NULL &&
		!hasWordType )
			{
			if( searchWordTypeItem->wordTypeNr() == wordTypeNr )
				{
				if( searchWordTypeItem->hideWordType( authorizationKey ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to hide a word type" );

				hasWordType = true;
				}
			else
				searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		if( !hasWordType )
			return startError( functionNameString, NULL, "I coundn't find the given word type" );

		return RESULT_OK;
		}

	ResultType markGeneralizationWordTypeAsWritten( bool isLanguageWord, unsigned short wordTypeNr )
		{
		bool hasWordTypeNr = false;
		bool isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchWordTypeItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );
		char functionNameString[FUNCTION_NAME_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, NULL, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		while( searchWordTypeItem != NULL &&
		!isWordTypeAlreadyMarkedAsWritten )
			{
			if( searchWordTypeItem->isGeneralizationWordAlreadyWritten() )
				isWordTypeAlreadyMarkedAsWritten = true;
			else
				{
				if( searchWordTypeItem->isSingularNoun() )
					singularNounWordTypeItem = searchWordTypeItem;
				else
					{
					if( searchWordTypeItem->isPluralNoun() )
						pluralNounWordTypeItem = searchWordTypeItem;
					}

				if( searchWordTypeItem->wordTypeNr() == wordTypeNr )
					{
					if( searchWordTypeItem->markGeneralizationWordTypeAsWritten() != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to mark a word as written" );

					hasWordTypeNr = true;
					}
				}

			searchWordTypeItem = ( isLanguageWord ? searchWordTypeItem->nextWordTypeItem() : searchWordTypeItem->nextCurrentLanguageWordTypeItem() );
			}

		if( isWordTypeAlreadyMarkedAsWritten )
			return startError( functionNameString, NULL, "The given word type number is already marked as written: ", wordTypeNr );

		if( !hasWordTypeNr )
			return startError( functionNameString, NULL, "I couldn't find the given word type number: ", wordTypeNr );

		// If singular noun - also set plural noun
		if( wordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
		pluralNounWordTypeItem != NULL )
			{
			if( pluralNounWordTypeItem->markGeneralizationWordTypeAsWritten() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to mark a plural noun word as written" );
			}
		else
			{
			// If plural noun - also set singular noun
			if( wordTypeNr == WORD_TYPE_NOUN_PLURAL &&
			singularNounWordTypeItem != NULL )
				{
				if( singularNounWordTypeItem->markGeneralizationWordTypeAsWritten() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to mark a singular noun word as written" );
				}
			}

		return RESULT_OK;
		}

	ResultType markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		bool hasWordTypeNr = false;
		bool isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, NULL, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		while( searchWordTypeItem != NULL &&
		!isWordTypeAlreadyMarkedAsWritten )
			{
			if( searchWordTypeItem->isSpecificationWordAlreadyWritten() )
				isWordTypeAlreadyMarkedAsWritten = true;
			else
				{
				if( searchWordTypeItem->isSingularNoun() )
					singularNounWordTypeItem = searchWordTypeItem;
				else
					{
					if( searchWordTypeItem->isPluralNoun() )
						pluralNounWordTypeItem = searchWordTypeItem;
					}

				if( searchWordTypeItem->wordTypeNr() == wordTypeNr )
					{
					if( searchWordTypeItem->markSpecificationWordTypeAsWritten() != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to mark a word as written" );

					hasWordTypeNr = true;
					}
				}

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		if( isWordTypeAlreadyMarkedAsWritten )
			return startError( functionNameString, NULL, "The given word type number is already marked as written: ", wordTypeNr );

		if( !hasWordTypeNr )
			return startError( functionNameString, NULL, "I couldn't find the given word type number: ", wordTypeNr );

		// If singular noun - also set plural noun
		if( wordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
		pluralNounWordTypeItem != NULL )
			{
			if( pluralNounWordTypeItem->markSpecificationWordTypeAsWritten() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to mark a plural noun word as written" );
			}
		else
			{
			// If plural noun - also set singular noun
			if( wordTypeNr == WORD_TYPE_NOUN_PLURAL &&
			singularNounWordTypeItem != NULL )
				{
				if( singularNounWordTypeItem->markSpecificationWordTypeAsWritten() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to mark a singular noun word as written" );
				}
			}

		return RESULT_OK;
		}

	ResultType markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		bool hasWordTypeNr = false;
		bool isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, NULL, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		while( searchWordTypeItem != NULL &&
		!isWordTypeAlreadyMarkedAsWritten )
			{
			if( !searchWordTypeItem->isRelationWordAlreadyWritten() )
				{
				if( searchWordTypeItem->isSingularNoun() )
					singularNounWordTypeItem = searchWordTypeItem;
				else
					{
					if( searchWordTypeItem->isPluralNoun() )
						pluralNounWordTypeItem = searchWordTypeItem;
					}

				if( searchWordTypeItem->wordTypeNr() == wordTypeNr )
					{
					if( searchWordTypeItem->markRelationWordTypeAsWritten() != RESULT_OK )
						return addError( functionNameString, NULL, "I failed to mark a word as written" );

					hasWordTypeNr = true;
					}
				}

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		if( isWordTypeAlreadyMarkedAsWritten )
			return startError( functionNameString, NULL, "The given word type number is already marked as written: ", wordTypeNr );

		if( !hasWordTypeNr )
			return startError( functionNameString, NULL, "I couldn't find the given word type number: ", wordTypeNr );

		// If singular noun - also set plural noun
		if( wordTypeNr == WORD_TYPE_NOUN_SINGULAR &&
		pluralNounWordTypeItem != NULL )
			{
			if( pluralNounWordTypeItem->markRelationWordTypeAsWritten() != RESULT_OK )
				return addError( functionNameString, NULL, "I failed to mark a plural noun word as written" );
			}
		else
			{
			// If plural noun - also set singular noun
			if( wordTypeNr == WORD_TYPE_NOUN_PLURAL &&
			singularNounWordTypeItem != NULL )
				{
				if( singularNounWordTypeItem->markRelationWordTypeAsWritten() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to mark a singular noun word as written" );
				}
			}

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchWordTypeItem != NULL )
			{
			if( searchWordTypeItem->hasCurrentCreationSentenceNr() )
				{
				if( searchWordTypeItem->storeWordTypeItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a word type item in the database" );
				}

			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			}

		return RESULT_OK;
		}
*/
	StringResultType findMatchingWordReferenceString( char *searchString )
		{
		StringResultType stringResult;
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();
		char *itemString;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findMatchingWordReferenceString";

		while( !stringResult.hasFoundMatchingStrings &&
		searchWordTypeItem != NULL )
			{
			if( ( itemString = searchWordTypeItem->itemString() ) != NULL )
				{
				if( ( stringResult = compareStrings( searchString, itemString ) ).result != RESULT_OK )
					return addStringResultError( functionNameString, NULL, "I failed to compare an active word type string with the query string" );

				if( stringResult.hasFoundMatchingStrings )
					commonVariables()->matchingWordTypeNr = searchWordTypeItem->wordTypeNr();
				}

			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			}

		searchWordTypeItem = firstDeletedWordTypeItem();

		while( !stringResult.hasFoundMatchingStrings &&
		searchWordTypeItem != NULL )
			{
			if( ( itemString = searchWordTypeItem->itemString() ) != NULL )
				{
				if( ( stringResult = compareStrings( searchString, itemString ) ).result != RESULT_OK )
					return addStringResultError( functionNameString, NULL, "I failed to compare a deleted word type string with the query string" );

				if( stringResult.hasFoundMatchingStrings )
					commonVariables()->matchingWordTypeNr = searchWordTypeItem->wordTypeNr();
				}

			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			}

		return stringResult;
		}

	WordResultType createWordTypeItem( bool hasFeminineWordEnding, bool hasMasculineWordEnding, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordTypeItem";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startWordResultError( functionNameString, NULL, "The given word type number is undefined or out of bounds" );

		if( wordTypeString == NULL )
			return startWordResultError( functionNameString, NULL, "The given wordTypeString is undefined" );

		if( ( wordResult.createdWordTypeItem = new WordTypeItem( hasFeminineWordEnding, hasMasculineWordEnding, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, commonVariables()->currentLanguageNr, wordTypeNr, wordLength, wordTypeString, commonVariables(), this, myWordItem() ) ) == NULL )
			return startWordResultError( functionNameString, NULL, "The created word type item is undefined" );

		if( addItemToList( QUERY_ACTIVE_CHAR, wordResult.createdWordTypeItem ) != RESULT_OK )
			return addWordResultError( functionNameString, NULL, "I failed to add an active word type item" );

		return wordResult;
		}

	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		WordTypeItem *searchWordTypeItem;
		WordTypeItem *foundWordTypeItem = NULL;

		foundWordTypeItem_ = NULL;

		// Try to find word type of the current language
		if( ( searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem() ) != NULL )
			foundWordTypeItem = wordTypeString( false, false, wordTypeNr, searchWordTypeItem );

		if( foundWordTypeItem == NULL &&
		( searchWordTypeItem = firstDeletedCurrentLanguageWordTypeItem() ) != NULL )
			foundWordTypeItem = wordTypeString( false, false, wordTypeNr, searchWordTypeItem );

		// Not found in current language. Now, try all languages
		if( isCheckingAllLanguages &&
		foundWordTypeItem == NULL )
			{
			if( ( searchWordTypeItem = firstActiveWordTypeItem() ) != NULL )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, searchWordTypeItem );

			if( foundWordTypeItem == NULL &&
			( searchWordTypeItem = firstDeletedWordTypeItem() ) != NULL )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, searchWordTypeItem );
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

	WordTypeItem *activeWordTypeItem( bool isAllowingDifferentNoun, bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		// Try to find word type of the current language
		if( ( foundWordTypeItem = wordTypeString( isAllowingDifferentNoun, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == NULL )
			{
			// Not found in current language. Now, try all languages
			if( isCheckingAllLanguages )
				foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() );
			}

		return foundWordTypeItem;
		}

	WordTypeItem *nextWordTypeListItem()
		{
		return (WordTypeItem *)nextListItem();
		}
	};

/*************************************************************************
 *	"He lifts the poor from the dust
 *	and needy from the garbage dump.
 *	He sets them among princes,
 *	even princes of his own people!" (Psalm 113:7-8)
 *************************************************************************/
