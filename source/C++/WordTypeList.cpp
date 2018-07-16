/*	Class:			WordTypeList
 *	Parent class:	List
 *	Purpose:		To store word type items
 *	Version:		Thinknowlogy 2018r2 (Natural Intelligence)
 *************************************************************************/
/*	Copyright (C) 2009-2018, Menno Mafait. Your suggestions, modifications,
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
#include "WordEndingResultType.cpp"
#include "WordTypeItem.cpp"

class WordTypeList : private List
	{
	friend class WordItem;

	// Private constructed variables

	WordTypeItem *foundWordTypeItem_;


	// Private functions

	void deleteWordTypeList( WordTypeItem *searchWordTypeItem )
		{
		WordTypeItem *deleteWordTypeItem;

		while( searchWordTypeItem != NULL )
			{
			deleteWordTypeItem = searchWordTypeItem;
			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			delete deleteWordTypeItem;
			}
		}

	WordTypeItem *nextWordTypeListItem()
		{
		return (WordTypeItem *)nextListItem();
		}

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

	WordTypeItem *firstActiveCurrentLanguageWordTypeItem()
		{
		unsigned short currentLanguageNr = globalVariables()->currentLanguageNr;
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();

		while( searchWordTypeItem != NULL &&
		searchWordTypeItem->wordTypeLanguageNr() < currentLanguageNr )
			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();

		return ( searchWordTypeItem != NULL &&
				searchWordTypeItem->wordTypeLanguageNr() == currentLanguageNr ? searchWordTypeItem : NULL );
		}


	protected:
	// Constructor

	WordTypeList( GlobalVariables *globalVariables, InputOutput *inputOutput, WordItem *myWordItem )
		{
		// Private constructed variables

		foundWordTypeItem_ = NULL;

		initializeListVariables( WORD_TYPE_LIST_SYMBOL, "WordTypeList", globalVariables, inputOutput, myWordItem );
		}

	~WordTypeList()
		{
		deleteWordTypeList( firstActiveWordTypeItem() );

		if( firstInactiveItem() != NULL )
			fprintf( stderr, "\nError: Class WordTypeList has inactive items." );

		if( firstArchivedItem() != NULL )
			fprintf( stderr, "\nError: Class WordTypeList has archived items." );

		if( firstReplacedItem() != NULL )
			fprintf( stderr, "\nError: Class WordTypeList has replaced items." );

		deleteWordTypeList( (WordTypeItem *)firstDeletedItem() );
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

	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *authorizationKey )
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

	signed char checkWordTypesOnFeminineParameters()
		{
		unsigned short interfaceParameter;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordTypesOnFeminineParameters";

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

			if( interfaceParameter > NO_INTERFACE_PARAMETER &&
			inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter, searchWordTypeItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
				return addError( functionNameString, "I failed to write an interface notification about the use of an article with a feminine noun" );

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		return RESULT_OK;
		}

	signed char checkWordTypesOnMasculineParameters()
		{
		unsigned short interfaceParameter;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "checkWordTypesOnMasculineParameters";

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

			if( interfaceParameter > NO_INTERFACE_PARAMETER &&
			inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, interfaceParameter, searchWordTypeItem->itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
				return addError( functionNameString, "I failed to write an interface notification about the use of an article with a masculine noun" );

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		return RESULT_OK;
		}

	signed char deleteWordType( WordTypeItem *deleteWordTypeItem )
		{
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteWordType";

		if( deleteWordTypeItem == NULL )
			return startError( functionNameString, "The given delete word type item is undefined" );

		if( deleteItem( searchWordTypeItem ) != RESULT_OK )
			return addError( functionNameString, "I failed to delete a word type item" );

		return RESULT_OK;
		}

	signed char deleteAllWordTypesOfCurrentSentence()
		{
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "deleteAllWordTypesOfCurrentSentence";

		while( searchWordTypeItem != NULL )
			{
			if( searchWordTypeItem->hasCurrentCreationSentenceNr() )
				{
				if( deleteItem( searchWordTypeItem ) != RESULT_OK )
					return addError( functionNameString, "I failed to delete a word type item of this sentence" );

				searchWordTypeItem = nextWordTypeListItem();
				}
			else
				searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			}

		return RESULT_OK;
		}

	signed char hideWordTypeItem( unsigned short wordTypeNr, char *authorizationKey )
		{
		bool hasWordType = false;
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "hideWordTypeItem";

		while( searchWordTypeItem != NULL &&
		!hasWordType )
			{
			if( searchWordTypeItem->wordTypeNr() == wordTypeNr )
				{
				if( searchWordTypeItem->hideWordType( authorizationKey ) != RESULT_OK )
					return addError( functionNameString, "I failed to hide a word type" );

				hasWordType = true;
				}
			else
				searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		if( !hasWordType )
			return startError( functionNameString, "I coundn't find the given word type" );

		return RESULT_OK;
		}

	signed char markGeneralizationWordTypeAsWritten( bool isLanguageWord, unsigned short wordTypeNr )
		{
		WordTypeItem *searchWordTypeItem = ( isLanguageWord ? firstActiveWordTypeItem() : firstActiveCurrentLanguageWordTypeItem() );
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		// Search for required word type
		while( searchWordTypeItem != NULL &&
		searchWordTypeItem->wordTypeNr() != wordTypeNr )
			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();

		if( searchWordTypeItem == NULL )
			return startError( functionNameString, "I couldn't find the given word type number: ", wordTypeNr );

		if( searchWordTypeItem->markGeneralizationWordTypeAsWritten() != RESULT_OK )
			return addError( functionNameString, "I failed to mark a generalization word as written" );

		return RESULT_OK;
		}

	signed char markSpecificationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		bool hasFoundWordType = false;
		bool isNounWordTypeGiven = ( wordTypeNr == WORD_TYPE_NOUN_SINGULAR ||
									wordTypeNr == WORD_TYPE_NOUN_PLURAL );
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		while( !hasFoundWordType &&
		searchWordTypeItem != NULL )
			{
			if( searchWordTypeItem->wordTypeNr() == wordTypeNr )
				{
				if( searchWordTypeItem->markSpecificationWordTypeAsWritten() != RESULT_OK )
					return addError( functionNameString, "I failed to mark a specification word as written" );

				hasFoundWordType = true;
				}
			else
				{
				if( isNounWordTypeGiven )
					{
					// If plural noun is given, mark also the singular noun as written
					// If singular noun is given, mark also the plural noun as written
					if( searchWordTypeItem->markSpecificationWordTypeAsWritten() != RESULT_OK )
						return addError( functionNameString, "I failed to mark a plural noun specification word as written" );
					}
				}

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		if( !hasFoundWordType )
			return startError( functionNameString, "I couldn't find the given word type number: ", wordTypeNr );

		return RESULT_OK;
		}

	signed char markRelationWordTypeAsWritten( unsigned short wordTypeNr )
		{
		WordTypeItem *searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markRelationWordTypeAsWritten";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startError( functionNameString, "The given word type number is undefined or out of bounds: ", wordTypeNr );

		// Search for required word type
		while( searchWordTypeItem != NULL &&
		searchWordTypeItem->wordTypeNr() != wordTypeNr )
			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();

		if( searchWordTypeItem == NULL )
			return startError( functionNameString, "I couldn't find the given word type number: ", wordTypeNr );

		if( searchWordTypeItem->markRelationWordTypeAsWritten() != RESULT_OK )
			return addError( functionNameString, "I failed to mark a relation word as written" );

		return RESULT_OK;
		}

	char *wordTypeString( bool isCheckingAllLanguages, unsigned short wordTypeNr )
		{
		WordTypeItem *foundWordTypeItem = NULL;
		WordTypeItem *searchWordTypeItem;

		foundWordTypeItem_ = NULL;

		// Try to find word type of the current language
		if( ( searchWordTypeItem = firstActiveCurrentLanguageWordTypeItem() ) != NULL )
			foundWordTypeItem = wordTypeString( false, false, wordTypeNr, searchWordTypeItem );

		// Not found in current language. Now, try all languages
		if( isCheckingAllLanguages &&
		foundWordTypeItem == NULL &&
		( searchWordTypeItem = firstActiveWordTypeItem() ) != NULL )
			foundWordTypeItem = wordTypeString( false, true, wordTypeNr, searchWordTypeItem );

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
		if( ( foundWordTypeItem = wordTypeString( isAllowingDifferentNoun, false, wordTypeNr, firstActiveCurrentLanguageWordTypeItem() ) ) == NULL &&
		// Not found in current language. Now, try all languages
		isCheckingAllLanguages )
			foundWordTypeItem = wordTypeString( false, true, wordTypeNr, firstActiveWordTypeItem() );

		return foundWordTypeItem;
		}

	BoolResultType findMatchingWordReferenceString( char *searchString )
		{
		WordTypeItem *searchWordTypeItem = firstActiveWordTypeItem();
		char *itemString;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findMatchingWordReferenceString";

		while( !boolResult.booleanValue &&
		searchWordTypeItem != NULL )
			{
			if( ( itemString = searchWordTypeItem->itemString() ) != NULL &&
			( boolResult = compareStrings( searchString, itemString ) ).result != RESULT_OK )
				return addBoolResultError( functionNameString, "I failed to compare an active word type string with the query string" );

			searchWordTypeItem = searchWordTypeItem->nextWordTypeItem();
			}

		return boolResult;
		}

	WordTypeResultType addWordType( bool isLanguageWord, bool isMultipleWord, bool isProperNounPrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeNr, size_t wordLength, char *wordTypeString )
		{
		bool hasFeminineWordEnding = false;
		bool hasMasculineWordEnding = false;
		bool isSingularNoun;
		WordEndingResultType wordEndingResult;
		WordResultType wordResult;
		WordTypeResultType wordTypeResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addWordType";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return startWordTypeResultError( functionNameString, "The given word type number is undefined or out of bounds" );

		if( wordTypeString == NULL )
			return startWordTypeResultError( functionNameString, "The given wordTypeString is undefined" );

		// Check if word type already exists
		if( ( wordResult = findWordType( false, wordTypeNr, wordTypeString ) ).result != RESULT_OK )
			return addWordTypeResultError( functionNameString, "I failed to find the given word type" );

		// Add word type if it doesn't exist yet
		if( wordResult.foundWordTypeItem == NULL )
			{
			isSingularNoun = ( wordTypeNr == WORD_TYPE_NOUN_SINGULAR );

			if( isSingularNoun ||

			( !isLanguageWord &&
			wordTypeNr == WORD_TYPE_PROPER_NOUN ) )
				{
				// Check on feminine and masculine word ending
				if( ( wordEndingResult = myWordItem()->analyzeWordEndingWithCurrentLanguage( ( isSingularNoun ? WORD_FEMININE_SINGULAR_NOUN_ENDING : WORD_FEMININE_PROPER_NOUN_ENDING ), 0, wordTypeString ) ).result != RESULT_OK )
					return addWordTypeResultError( functionNameString, "I failed to check on feminine word ending" );

				if( wordEndingResult.hasFoundWordEnding )
					{
					hasFeminineWordEnding = true;

					if( isSingularNoun &&
					myWordItem()->markWordAsFeminine() != RESULT_OK )
						return addWordTypeResultError( functionNameString, "I failed to mark my word as feminine" );
					}
				else
					{
					if( ( wordEndingResult = myWordItem()->analyzeWordEndingWithCurrentLanguage( ( isSingularNoun ? WORD_MASCULINE_SINGULAR_NOUN_ENDING : WORD_MASCULINE_PROPER_NOUN_ENDING ), 0, wordTypeString ) ).result != RESULT_OK )
						return addWordTypeResultError( functionNameString, "I failed to check on masculine word ending" );

					if( wordEndingResult.hasFoundWordEnding )
						{
						hasMasculineWordEnding = true;

						if( isSingularNoun &&
						myWordItem()->markWordAsMasculine() != RESULT_OK )
							return addWordTypeResultError( functionNameString, "I failed to mark my word as masculine" );
						}
					}
				}

			if( ( wordTypeResult.wordTypeItem = new WordTypeItem( ( hasFeminineWordEnding && ( isSingularNoun || !isMultipleWord ) ), hasMasculineWordEnding, isProperNounPrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, globalVariables()->currentLanguageNr, wordTypeNr, wordLength, wordTypeString, globalVariables(), inputOutput(), this, myWordItem() ) ) == NULL )
				return startWordTypeResultError( functionNameString, "The created word type item is undefined" );

			if( addItemToList( QUERY_ACTIVE_CHAR, wordTypeResult.wordTypeItem ) != RESULT_OK )
				return addWordTypeResultError( functionNameString, "I failed to add a word type item" );
			}

		return wordTypeResult;
		}

	WordResultType findWordType( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString )
		{
		size_t currentWordTypeStringLength;
		size_t wordTypeStringLength;
		char *currentWordTypeString;
		WordTypeItem *currentWordTypeItem;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findWordType";

		if( wordTypeString == NULL )
			return startWordResultError( functionNameString, "The given word type string is undefined" );

		if( ( wordTypeStringLength = strlen( wordTypeString ) ) == 0 )
			return startWordResultError( functionNameString, "The given word type string is empty" );

		if( ( currentWordTypeItem = activeWordTypeItem( false, isCheckingAllLanguages, wordTypeNr ) ) != NULL )
			{
			do	{
				// Skip hidden word type
				if( ( currentWordTypeString = currentWordTypeItem->itemString() ) != NULL )
					{
					if( ( currentWordTypeStringLength = strlen( currentWordTypeString ) ) == 0 )
						return startWordResultError( functionNameString, "The active word type string is empty" );

					if( wordTypeStringLength == currentWordTypeStringLength &&
					strcmp( wordTypeString, currentWordTypeString ) == 0 )
						{
						wordResult.foundWordItem = myWordItem();
						wordResult.foundWordTypeItem = currentWordTypeItem;
						}
					}
				}
			while( wordResult.foundWordItem == NULL &&
			( currentWordTypeItem = currentWordTypeItem->nextCurrentLanguageWordTypeItem() ) != NULL );
			}

		return wordResult;
		}
	};

/*************************************************************************
 *	"He lifts the poor from the dust
 *	and needy from the garbage dump.
 *	He sets them among princes,
 *	even princes of his own people!" (Psalm 113:7-8)
 *************************************************************************/
