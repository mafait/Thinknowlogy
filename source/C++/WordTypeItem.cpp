/*	Class:			WordTypeItem
 *	Parent class:	Item
 *	Purpose:		Storing the word types of a word
 *	Version:		Thinknowlogy 2024 (Intelligent Origin)
 *************************************************************************/
/*	Copyright (C) 2024, Menno Mafait. Your suggestions, modifications,
 *	corrections and bug reports are welcome at https://mafait.org/contact
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

#ifndef WORDTYPEITEM
#define WORDTYPEITEM 1
#include "InputOutput.cpp"

class WordTypeItem : private Item
	{
	friend class AdminReadCreateWords;
	friend class AdminReadFile;
	friend class AdminReadSentence;
	friend class WordItem;
	friend class WordSpecification;
	friend class WordTypeList;

	// Private initialized variables

	bool hasFeminineWordEnding_ = false;
	bool hasMasculineWordEnding_ = false;
	bool isProperNounPrecededByDefiniteArticle_ = false;

	unsigned short adjectiveParameter_ = NO_ADJECTIVE_PARAMETER;
	unsigned short definiteArticleParameter_ = NO_DEFINITE_ARTICLE_PARAMETER;
	unsigned short indefiniteArticleParameter_ = NO_INDEFINITE_ARTICLE_PARAMETER;
	unsigned short wordTypeLanguageNr_ = NO_LANGUAGE_NR;
	unsigned short wordTypeNr_ = NO_WORD_TYPE_NR;


	// Private constructed variables

	unsigned short generalizationWriteLevel_ = NO_WRITE_LEVEL;
	unsigned short specificationWriteLevel_ = NO_WRITE_LEVEL;
	unsigned short relationWriteLevel_ = NO_WRITE_LEVEL;

	char *hideKey_ = NULL;
	char *wordTypeString_ = NULL;


	// Private functions

	bool isIndefinitePhoneticVowelArticle( unsigned short indefiniteArticleParameter )
		{
		unsigned short phoneticVowelIndefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem *phoneticVowelIndefiniteArticleWordItem;

		switch( indefiniteArticleParameter )
			{
			// Phonetic vowel article
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE:
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE:
				return true;

			// Plural article
			case WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_FEMININE:
				phoneticVowelIndefiniteArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE;
				break;

			case WORD_PARAMETER_ARTICLE_INDEFINITE_PLURAL_MASCULINE:
				phoneticVowelIndefiniteArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE;
				break;

			// Singular article
			case WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE:
				phoneticVowelIndefiniteArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE;
				break;

			case WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE:
				phoneticVowelIndefiniteArticleParameter = WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE;
				break;
			}

		return ( phoneticVowelIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
				( phoneticVowelIndefiniteArticleWordItem = myWordItem()->predefinedWordItem( phoneticVowelIndefiniteArticleParameter ) ) != NULL ?
					phoneticVowelIndefiniteArticleWordItem->hasWordType( false, WORD_TYPE_ARTICLE ) : false );
		}

	bool isIndefiniteArticlePhoneticVowelParameter( unsigned short wordParameter )
		{
		return ( wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE );
		}


	protected:
	// Constructor

	WordTypeItem( bool hasFeminineWordEnding, bool hasMasculineWordEnding, bool isProperNounPrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeLanguageNr, unsigned short wordTypeNr, size_t wordTypeStringLength, char *wordTypeString, GlobalVariables *globalVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WordTypeItem", globalVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		hasFeminineWordEnding_ = hasFeminineWordEnding;
		hasMasculineWordEnding_ = hasMasculineWordEnding;
		isProperNounPrecededByDefiniteArticle_ = isProperNounPrecededByDefiniteArticle;

		adjectiveParameter_ = adjectiveParameter;
		definiteArticleParameter_ = definiteArticleParameter;
		indefiniteArticleParameter_ = indefiniteArticleParameter;
		wordTypeLanguageNr_ = wordTypeLanguageNr;
		wordTypeNr_ = wordTypeNr;

		// Checking private initialized variables

		if( wordTypeString != NULL )
			{
			if( strlen( wordTypeString ) > 0 )
				{
				if( wordTypeStringLength > 0 )
					{
					if( wordTypeStringLength < SENTENCE_STRING_LENGTH &&
					strlen( wordTypeString ) < SENTENCE_STRING_LENGTH )
						{
						if( ( wordTypeString_ = new char[wordTypeStringLength + 1] ) != NULL )
							{
							strcpy( wordTypeString_, EMPTY_STRING );
							strncat( wordTypeString_, wordTypeString, wordTypeStringLength );

							if( isupper( wordTypeString[0] ) &&
							wordTypeNr != WORD_TYPE_LETTER_CAPITAL &&
							wordTypeNr != WORD_TYPE_PROPER_NOUN )
								wordTypeString_[0] = (char)tolower( wordTypeString_[0] );
							}
						else
							startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the word type string" );
						}
					else
						startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type is too long" );
					}
				else
					startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type string length is undefined" );
				}
			else
				startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type string is empty" );
			}
		else
			startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type string is undefined" );
		}

	~WordTypeItem()
		{
		if( wordTypeString_ != NULL )
			delete wordTypeString_;
		}


	// Protected virtual functions

	virtual void displayString( bool isReturnQueryToPosition )
		{
		statusString[0] = statusChar();

		if( itemString() != NULL )
			{
			if( globalVariables()->hasFoundQuery )
				strcat( globalVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( globalVariables()->queryString, statusString );

			globalVariables()->hasFoundQuery = true;
			strcat( globalVariables()->queryString, itemString() );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( adjectiveParameter_ == queryParameter ||
				definiteArticleParameter_ == queryParameter ||
				indefiniteArticleParameter_ == queryParameter ||
				wordTypeLanguageNr_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

					( adjectiveParameter_ > NO_ADJECTIVE_PARAMETER ||
					definiteArticleParameter_ > NO_DEFINITE_ARTICLE_PARAMETER ||
					indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER ||
					wordTypeLanguageNr_ > NO_LANGUAGE_NR ) ) );
		}

	virtual bool hasWordType( unsigned short queryWordTypeNr )
		{
		return ( wordTypeNr_ == queryWordTypeNr );
		}

	virtual bool isSorted( Item *nextSortItem )
		{
		return ( nextSortItem != NULL &&
				// Ascending wordTypeLanguageNr_
				wordTypeLanguageNr_ < ( (WordTypeItem *)nextSortItem )->wordTypeLanguageNr_ );
		}

	virtual char *itemString()
		{
		return ( hideKey_ == NULL ? wordTypeString_ : NULL );
		}

	virtual char *itemToString( unsigned short queryWordTypeNr )
		{
		WordItem *_myWordItem = myWordItem();
		char *queryString;
		char *wordString;
		char *wordTypeString = _myWordItem->wordTypeNameString( wordTypeNr_ );
		char *languageNameString = _myWordItem->languageNameString( wordTypeLanguageNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = globalVariables()->queryString;

		if( wordTypeLanguageNr_ > NO_LANGUAGE_NR )
			{
			if( languageNameString == NULL )
				sprintf( tempString, "%cwordTypeLanguageNr:%u", QUERY_SEPARATOR_CHAR, wordTypeLanguageNr_ );
			else
				sprintf( tempString, "%cwordTypeLanguageNr:%s", QUERY_SEPARATOR_CHAR, languageNameString );

			strcat( queryString, tempString );
			}

		if( hideKey_ != NULL )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isHidden" );
			}

		if( hasFeminineWordEnding_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "hasFeminineWordEnding" );
			}

		if( hasMasculineWordEnding_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "hasMasculineWordEnding" );
			}

		if( isProperNounPrecededByDefiniteArticle_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isProperNounPrecededByDefiniteArticle" );
			}

		if( adjectiveParameter_ > NO_DEFINITE_ARTICLE_PARAMETER )
			{
			sprintf( tempString, "%cadjectiveParameter:%u", QUERY_SEPARATOR_CHAR, adjectiveParameter_ );
			strcat( queryString, tempString );
			}

		if( definiteArticleParameter_ > NO_DEFINITE_ARTICLE_PARAMETER )
			{
			sprintf( tempString, "%cdefiniteArticleParameter:%u", QUERY_SEPARATOR_CHAR, definiteArticleParameter_ );
			strcat( queryString, tempString );
			}

		if( indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER )
			{
			sprintf( tempString, "%cindefiniteArticleParameter:%u", QUERY_SEPARATOR_CHAR, indefiniteArticleParameter_ );
			strcat( queryString, tempString );
			}

		if( wordTypeString == NULL )
			sprintf( tempString, "%cwordType:%c%u", QUERY_SEPARATOR_CHAR, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );
		else
			sprintf( tempString, "%cwordType:%s%c%u", QUERY_SEPARATOR_CHAR, wordTypeString, QUERY_WORD_TYPE_CHAR, wordTypeNr_ );

		strcat( queryString, tempString );

		if( ( wordString = itemString() ) != NULL )
			{
			sprintf( tempString, "%cwordString:%c%s%c", QUERY_SEPARATOR_CHAR, QUERY_STRING_START_CHAR, wordString, QUERY_STRING_END_CHAR );
			strcat( queryString, tempString );
			}

		return queryString;
		}


	// Protected functions

	void clearGeneralizationWriteLevel( unsigned short currentWriteLevel )
		{
		if( generalizationWriteLevel_ > currentWriteLevel )
			generalizationWriteLevel_ = NO_WRITE_LEVEL;
		}

	void clearSpecificationWriteLevel( unsigned short currentWriteLevel )
		{
		if( specificationWriteLevel_ > currentWriteLevel )
			specificationWriteLevel_ = NO_WRITE_LEVEL;
		}

	void clearRelationWriteLevel( unsigned short currentWriteLevel )
		{
		if( relationWriteLevel_ > currentWriteLevel )
			relationWriteLevel_ = NO_WRITE_LEVEL;
		}

	void clearIndefiniteArticleParameter()
		{
		indefiniteArticleParameter_ = NO_INDEFINITE_ARTICLE_PARAMETER;
		}

	bool hasAdjectiveParameter()
		{
		return ( adjectiveParameter_ > NO_ADJECTIVE_PARAMETER );
		}

	bool hasDefiniteArticleParameter()
		{
		return ( definiteArticleParameter_ > NO_DEFINITE_ARTICLE_PARAMETER );
		}

	bool hasIndefiniteArticleParameter()
		{
		return ( indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER );
		}

	bool hasFeminineWordEnding()
		{
		return hasFeminineWordEnding_;
		}

	bool hasFeminineOrMasculineWordEnding()
		{
		return ( hasFeminineWordEnding_ ||
				hasMasculineWordEnding_ );
		}

	bool hasMasculineWordEnding()
		{
		return hasMasculineWordEnding_;
		}

	bool hasFeminineDefiniteArticleParameter()
		{
		return isFeminineArticleParameter( definiteArticleParameter_ );
		}

	bool hasFeminineIndefiniteArticleParameter()
		{
		return isFeminineArticleParameter( indefiniteArticleParameter_ );
		}

	bool hasMasculineDefiniteArticleParameter()
		{
		return isMasculineArticleParameter( definiteArticleParameter_ );
		}

	bool hasMasculineIndefiniteArticleParameter()
		{
		return isMasculineArticleParameter( indefiniteArticleParameter_ );
		}

	bool hasFeminineAndMasculineDefiniteArticle()
		{
		return myWordItem()->hasFeminineAndMasculineArticle( definiteArticleParameter_ );
		}

	bool hasFeminineAndMasculineIndefiniteArticle()
		{
		return myWordItem()->hasFeminineAndMasculineArticle( indefiniteArticleParameter_ );
		}

	bool isAnswer()
		{
		return ( wordTypeNr_ == WORD_TYPE_ANSWER );
		}

	bool isCorrectAdjective( unsigned short adjectiveParameter )
		{
		return ( adjectiveParameter_ == NO_ADJECTIVE_PARAMETER ||
				adjectiveParameter_ == adjectiveParameter );
		}

	bool isCorrectDefiniteArticle( unsigned short definiteArticleParameter )
		{
		return ( definiteArticleParameter_ == NO_DEFINITE_ARTICLE_PARAMETER ||
				definiteArticleParameter_ == definiteArticleParameter );
		}

	bool isCorrectIndefiniteArticle( bool isCheckingForEqualParameters, unsigned short indefiniteArticleParameter )
		{
		bool hasIndefiniteArticleParameter = ( indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER );
		bool isStringStartingWithVowel;
		bool isVowelIndefiniteArticle;

		if( isCheckingForEqualParameters &&
		indefiniteArticleParameter_ == indefiniteArticleParameter )
			return true;

		// Typical for English ('a' or 'an')
		// If undefined, fall back to a simple phonetic vowel rule
		if( isIndefinitePhoneticVowelArticle( hasIndefiniteArticleParameter ? indefiniteArticleParameter_ : indefiniteArticleParameter ) )
			{
			isStringStartingWithVowel = doesStringStartWithPhoneticVowel( itemString() );
			isVowelIndefiniteArticle = isIndefiniteArticlePhoneticVowelParameter( indefiniteArticleParameter );

			return ( ( !isStringStartingWithVowel &&	// 'a'
					!isVowelIndefiniteArticle ) ||

					( isStringStartingWithVowel &&		// 'an'
					isVowelIndefiniteArticle ) );
			}

		if( hasIndefiniteArticleParameter )
			return !isCheckingForEqualParameters;

		return ( ( definiteArticleParameter_ != WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				definiteArticleParameter_ != WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE ) ||

				( definiteArticleParameter_ == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				indefiniteArticleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ) ||

				( definiteArticleParameter_ == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE &&
				indefiniteArticleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) );
		}

	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *hideKey )
		{
		return ( wordTypeNr_ == wordTypeNr &&
				hideKey_ == hideKey &&
				compareString != NULL &&
				strcmp( wordTypeString_, compareString ) == 0 );
		}

	bool isDefiniteArticle()
		{
		return isDefiniteArticleParameter( definiteArticleParameter_ );
		}

	bool isNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isProperNounPrecededByDefiniteArticle()
		{
		return isProperNounPrecededByDefiniteArticle_;
		}

	bool isProperNounPrecededByDefiniteArticle( unsigned short definiteArticleParameter )
		{
		return ( isProperNounPrecededByDefiniteArticle_ &&
				isCorrectDefiniteArticle( definiteArticleParameter ) );
		}

	bool isSingularNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}
/*
	bool isGeneralizationWordAlreadyWritten()
		{
		return ( generalizationWriteLevel_ > NO_WRITE_LEVEL );
		}
*/
	bool isSpecificationWordAlreadyWritten()
		{
		return ( specificationWriteLevel_ > NO_WRITE_LEVEL );
		}

	bool isRelationWordAlreadyWritten()
		{
		return ( relationWriteLevel_ > NO_WRITE_LEVEL );
		}

	unsigned short adjectiveParameter()
		{
		return adjectiveParameter_;
		}

	unsigned short definiteArticleParameter()
		{
		return definiteArticleParameter_;
		}

	unsigned short indefiniteArticleParameter()
		{
		return indefiniteArticleParameter_;
		}

	unsigned short wordTypeLanguageNr()
		{
		return wordTypeLanguageNr_;
		}

	unsigned short wordTypeNr()
		{
		return wordTypeNr_;
		}

	signed char markGeneralizationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( globalVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		if( generalizationWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );

		generalizationWriteLevel_ = ++globalVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	signed char markSpecificationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( globalVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		if( specificationWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );

		specificationWriteLevel_ = ++globalVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	signed char markRelationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "markRelationWordTypeAsWritten";

		if( globalVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		if( relationWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );

		relationWriteLevel_ = ++globalVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	signed char hideWordType( char *hideKey )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "hideWordType";

		if( hideKey_ != NULL )
			return startError( functionNameString, NULL, itemString(), "This word type is already hidden" );

		hideKey_ = hideKey;

		return RESULT_OK;
		}

	signed char setAdjectiveParameter( unsigned short newAdjectiveParameter )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setAdjectiveParameter";

		if( !isSingularNoun() )
			return startError( functionNameString, NULL, itemString(), "I am not a singular noun" );

		if( !isAdjectiveParameter( newAdjectiveParameter ) )
			return startError( functionNameString, NULL, itemString(), "The given adjective parameter is no adjective parameter" );

		if( adjectiveParameter_ == NO_ADJECTIVE_PARAMETER )
			{
			if( !hasCurrentCreationSentenceNr() )
				return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

			adjectiveParameter_ = newAdjectiveParameter;
			}
		else
			{
			if( adjectiveParameter_ != newAdjectiveParameter &&
			inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
				return addError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of a different adjective" );
			}

		return RESULT_OK;
		}

	signed char setDefiniteArticleParameter( unsigned short newDefiniteArticleParameter )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setDefiniteArticleParameter";

		if( !isSingularNoun() )
			return startError( functionNameString, NULL, itemString(), "I am not a singular noun" );

		if( !isDefiniteArticleParameter( newDefiniteArticleParameter ) )
			return startError( functionNameString, NULL, itemString(), "The given definite article parameter is no definite article parameter" );

		if( definiteArticleParameter_ == NO_DEFINITE_ARTICLE_PARAMETER )
			{
			// Need to be fixed by copy-and-replace of old word type item
//			if( !hasCurrentCreationSentenceNr() )
//				return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

			definiteArticleParameter_ = newDefiniteArticleParameter;
			}
		else
			{
			if( definiteArticleParameter_ != newDefiniteArticleParameter &&
			inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
				return addError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of a different defnite article" );
			}

		return RESULT_OK;
		}

	signed char setIndefiniteArticleParameter( unsigned short newIndefiniteArticleParameter )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setIndefiniteArticleParameter";

		if( !isSingularNoun() )
			return startError( functionNameString, NULL, itemString(), "I am not a singular noun" );

		if( !isIndefiniteArticleParameter( newIndefiniteArticleParameter ) )
			return startError( functionNameString, NULL, itemString(), "The given indefinite article parameter is no indefinite article parameter" );

		if( indefiniteArticleParameter_ == NO_INDEFINITE_ARTICLE_PARAMETER )
			{
			// Need to be fixed by copy-and-replace of old word type item
//			if( !hasCurrentCreationSentenceNr() )
//				return startError( functionNameString, NULL, "It isn't allowed to change an older item afterwards" );

			indefiniteArticleParameter_ = newIndefiniteArticleParameter;
			}
		else
			{
			if( indefiniteArticleParameter_ != newIndefiniteArticleParameter &&
			// Write interface notification about the use of an indefinite article
			inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
				return addError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of an indefinite article" );
			}

		return RESULT_OK;
		}

//Java_protected_final
	char *wordTypeString()
		{
		return wordTypeString_;
		}

	WordTypeItem *nextWordTypeItem()
		{
		return (WordTypeItem *)nextItem;
		}

	WordTypeItem *nextCurrentLanguageWordTypeItem()
		{
		WordTypeItem *nextCurrentLanguageWordTypeItem = nextWordTypeItem();

		return ( nextCurrentLanguageWordTypeItem != NULL &&
				nextCurrentLanguageWordTypeItem->wordTypeLanguageNr() == globalVariables()->currentLanguageNr ? nextCurrentLanguageWordTypeItem : NULL );
		}
	};
#endif

/*************************************************************************
 *	"The Lord gives his people strength.
 *	The Lord blesses them with peace." (Psalm 29:11)
 *************************************************************************/
