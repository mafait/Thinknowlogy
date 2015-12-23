/*
 *	Class:			WordTypeList
 *	Parent class:	List
 *	Purpose:		To store word type items
 *	Version:		Thinknowlogy 2015r1 (Esperanza)
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

#include "List.h"
#include "WordTypeItem.cpp"

class WordTypeList : private List
	{
	friend class WordItem;
	friend class WordType;

	// Private constructible variables

	WordTypeItem *foundWordTypeItem_;


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

				// Don't show status with active items
				if( !searchItem->isActiveItem() )
					{
					statusString[0] = searchItem->statusChar();
					strcat( commonVariables()->queryString, statusString );
					}

				strcat( commonVariables()->queryString, wordTypeString );
				}

			searchItem = searchItem->nextWordTypeItem();
			}
		}

	WordTypeItem *wordTypeString( bool isAllowingDifferentNoun, bool isCheckingAllLanguages, unsigned short wordTypeNr, WordTypeItem *searchItem )
		{
		while( searchItem != NULL )
			{
			if( wordTypeNr == WORD_TYPE_UNDEFINED ||
			searchItem->wordTypeNr() == wordTypeNr ||

			( isAllowingDifferentNoun &&
			searchItem->isSingularOrPluralNounWordType() &&
			myWordItem()->isSingularOrPluralNoun( searchItem->wordTypeNr() ) ) )
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
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		WordTypeItem *searchItem = firstActiveWordTypeItem();

		while( searchItem != NULL &&
		searchItem->wordTypeLanguageNr() < currentLanguageNr )
			searchItem = searchItem->nextWordTypeItem();

		return ( searchItem != NULL && searchItem->wordTypeLanguageNr() == currentLanguageNr ? searchItem : NULL );
		}

	WordTypeItem *firstReplacedCurrentLanguageWordTypeItem()
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		WordTypeItem *searchItem = firstReplacedWordTypeItem();

		while( searchItem != NULL &&
		searchItem->wordTypeLanguageNr() < currentLanguageNr )
			searchItem = searchItem->nextWordTypeItem();

		return ( searchItem != NULL && searchItem->wordTypeLanguageNr() == currentLanguageNr ? searchItem : NULL );
		}

	WordTypeItem *firstDeletedCurrentLanguageWordTypeItem()
		{
		unsigned short currentLanguageNr = commonVariables()->currentLanguageNr;
		WordTypeItem *searchItem = firstDeletedWordTypeItem();

		while( searchItem != NULL &&
		searchItem->wordTypeLanguageNr() < currentLanguageNr )
			searchItem = searchItem->nextWordTypeItem();

		return ( searchItem != NULL && searchItem->wordTypeLanguageNr() == currentLanguageNr ? searchItem : NULL );
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
		WordTypeItem *deleteItem;
		WordTypeItem *searchItem = firstActiveWordTypeItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordTypeItem();
			delete deleteItem;
			}

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WordTypeList has inactive items." );

		if( firstArchivedItem() )
			fprintf( stderr, "\nError: Class WordTypeList has archived items." );

		searchItem = firstReplacedWordTypeItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordTypeItem();
			delete deleteItem;
			}

		searchItem = firstDeletedWordTypeItem();

		while( searchItem != NULL )
			{
			deleteItem = searchItem;
			searchItem = searchItem->nextWordTypeItem();
			delete deleteItem;
			}
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

	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, void *authorizationKey )
		{
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->isCorrectHiddenWordType( wordTypeNr, compareString, authorizationKey ) )
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
				interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchItem->hasFeminineIndefiniteArticleParameter() &&
				searchItem->hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > NO_INTERFACE_PARAMETER )
				{
				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to write an interface notification about the use of an article with a feminine noun" );
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
				interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START;
			else
				{
				if( searchItem->hasMasculineIndefiniteArticleParameter() &&
				searchItem->hasFeminineAndMasculineIndefiniteArticle() )
					interfaceParameter = INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START;
				}

			if( interfaceParameter > NO_INTERFACE_PARAMETER )
				{
				if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, interfaceParameter, searchItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to write an interface notification about the use of an article with a masculine noun" );
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
					return addError( functionNameString, NULL, "I failed to delete an active item" );
				}
			else
				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		if( !hasFoundWordType )
			return startError( functionNameString, NULL, "I coundn't find the given word type" );

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
					return addError( functionNameString, NULL, "I failed to hide a word type" );
				}
			else
				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		if( !hasFoundWordType )
			return startError( functionNameString, NULL, "I coundn't find the given word type" );

		return RESULT_OK;
		}

	ResultType markGeneralizationWordTypeAsWritten( bool isLanguageWord, unsigned short wordTypeNr )
		{
		bool hasFoundWordTypeNr = false;
		bool isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );
		char functionNameString[FUNCTION_NAME_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != NULL &&
			!isWordTypeAlreadyMarkedAsWritten )
				{
				if( searchItem->isGeneralizationWordAlreadyWritten() )
					isWordTypeAlreadyMarkedAsWritten = true;
				else
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
							return addError( functionNameString, NULL, "I failed to mark a word as written" );
						}
					}

				searchItem = ( isLanguageWord ? searchItem->nextWordTypeItem() : searchItem->nextCurrentLanguageWordTypeItem() );
				}

			if( !isWordTypeAlreadyMarkedAsWritten )
				{
				if( hasFoundWordTypeNr )
					{
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
					}
				else
					return startError( functionNameString, NULL, "I couldn't find the given word type number: ", wordTypeNr );
				}
			else
				return startError( functionNameString, NULL, "The given word type number is already marked as written: ", wordTypeNr );
			}
		else
			return startError( functionNameString, NULL, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		return RESULT_OK;
		}

	ResultType markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		bool hasFoundWordTypeNr = false;
		bool isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != NULL &&
			!isWordTypeAlreadyMarkedAsWritten )
				{
				if( searchItem->isSpecificationWordAlreadyWritten() )
					isWordTypeAlreadyMarkedAsWritten = true;
				else
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
							return addError( functionNameString, NULL, "I failed to mark a word as written" );
						}
					}

				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
				}

			if( !isWordTypeAlreadyMarkedAsWritten )
				{
				if( hasFoundWordTypeNr )
					{
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
					}
				else
					return startError( functionNameString, NULL, "I couldn't find the given word type number: ", wordTypeNr );
				}
			else
				return startError( functionNameString, NULL, "The given word type number is already marked as written: ", wordTypeNr );
			}
		else
			return startError( functionNameString, NULL, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		return RESULT_OK;
		}

	ResultType markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		bool hasFoundWordTypeNr = false;
		bool isWordTypeAlreadyMarkedAsWritten = false;
		WordTypeItem *pluralNounWordTypeItem = NULL;
		WordTypeItem *singularNounWordTypeItem = NULL;
		WordTypeItem *searchItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeNr > WORD_TYPE_UNDEFINED &&
		wordTypeNr < NUMBER_OF_WORD_TYPES )
			{
			while( searchItem != NULL &&
			!isWordTypeAlreadyMarkedAsWritten )
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
							return addError( functionNameString, NULL, "I failed to mark a word as written" );
						}
					}

				searchItem = searchItem->nextCurrentLanguageWordTypeItem();
				}

			if( !isWordTypeAlreadyMarkedAsWritten )
				{
				if( hasFoundWordTypeNr )
					{
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
					}
				else
					return startError( functionNameString, NULL, "I couldn't find the given word type number: ", wordTypeNr );
				}
			else
				return startError( functionNameString, NULL, "The given word type number is already marked as written: ", wordTypeNr );
			}
		else
			return startError( functionNameString, NULL, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		return RESULT_OK;
		}
/*
	ResultType storeChangesInFutureDatabase()
		{
		WordTypeItem *searchItem = firstActiveWordTypeItem();
		char functionNameString[FUNCTION_NAME_LENGTH] = "storeChangesInFutureDatabase";

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeWordTypeItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to store a word type item in the database" );
				}

			searchItem = searchItem->nextWordTypeItem();
			}

		searchItem = firstReplacedWordTypeItem();

		while( searchItem != NULL )
			{
			if( searchItem->hasCurrentCreationSentenceNr() )
				{
				if( searchItem->storeWordTypeItemInFutureDatabase() != RESULT_OK )
					return addError( functionNameString, NULL, "I failed to modify a replaced word type item in the database" );
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
					addError( functionNameString, NULL, "I failed to compare an active word type string with the query string" );
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
					addError( functionNameString, NULL, "I failed to compare a deleted word type string with the query string" );
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
					addError( functionNameString, NULL, "I failed to compare a deleted word type string with the query string" );
				}

			searchItem = searchItem->nextWordTypeItem();
			}

		referenceResult.result = commonVariables()->result;
		return referenceResult;
		}

	WordResultType createWordTypeItem( bool hasFeminineWordEnding, bool hasMasculineWordEnding, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
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
					if( ( wordResult.createdWordTypeItem = new WordTypeItem( hasFeminineWordEnding, hasMasculineWordEnding, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, commonVariables()->currentLanguageNr, wordTypeNr, wordLength, wordTypeString, commonVariables(), this, myWordItem() ) ) != NULL )
						{
						if( addItemToList( QUERY_ACTIVE_CHAR, wordResult.createdWordTypeItem ) != RESULT_OK )
							addError( functionNameString, NULL, "I failed to add an active word type item" );
						}
					else
						startError( functionNameString, NULL, "The created word type item is undefined" );
					}
				else
					startError( functionNameString, NULL, "The current item number is undefined" );
				}
			else
				startError( functionNameString, NULL, "The given wordTypeString is undefined" );
			}
		else
			startError( functionNameString, NULL, "The given word type number is undefined or out of bounds" );

		wordResult.result = commonVariables()->result;
		return wordResult;
		}

	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem;

		foundWordTypeItem_ = NULL;

		// Try to find word type of the current language
		if( ( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == NULL &&
		( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstReplacedCurrentLanguageWordTypeItem() ) ) == NULL &&
		( foundWordTypeItem = wordTypeString( false, false, wordTypeNr, firstDeletedCurrentLanguageWordTypeItem() ) ) == NULL )
			{
			// Not found in current language. Now, try all languages
			if( isCheckingAllLanguages &&
			( foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() ) ) == NULL )
				{
				if( ( foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstReplacedWordTypeItem() ) ) == NULL )
					foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstDeletedWordTypeItem() );
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
	};

/*************************************************************************
 *	"He lifts the poor from the dust
 *	and needy from the garbage dump.
 *	He sets them among princes,
 *	even princes of his own people!" (Psalm 113:7-8)
 *************************************************************************/
