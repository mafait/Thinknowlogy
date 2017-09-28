/*	Class:			WordTypeItem
 *	Parent class:	Item
 *	Purpose:		To store the word types of a word
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
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

	bool hasFeminineWordEnding_;
	bool hasMasculineWordEnding_;
	bool isProperNamePrecededByDefiniteArticle_;

	unsigned short adjectiveParameter_;
	unsigned short definiteArticleParameter_;
	unsigned short indefiniteArticleParameter_;
	unsigned short wordTypeLanguageNr_;
	unsigned short wordTypeNr_;


	// Private constructed variables

	unsigned short generalizationWriteLevel_;
	unsigned short specificationWriteLevel_;
	unsigned short relationWriteLevel_;

	char *hideKey_;
	char *wordTypeString_;


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

		if( phoneticVowelIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
		( phoneticVowelIndefiniteArticleWordItem = myWordItem()->predefinedWordItem( phoneticVowelIndefiniteArticleParameter ) ) != NULL )
			return phoneticVowelIndefiniteArticleWordItem->hasWordType( false, WORD_TYPE_ARTICLE );

		return false;
		}

	bool isIndefiniteArticlePhoneticVowelParameter( unsigned short wordParameter )
		{
		return ( wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_FEMININE ||
				wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_PLURAL_MASCULINE ||
				wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_FEMININE ||
				wordParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_PHONETIC_VOWEL_SINGULAR_MASCULINE );
		}

	BoolResultType setAdjectiveParameter( unsigned short adjectiveParameter )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setAdjectiveParameter";

		if( !isAdjectiveParameter( adjectiveParameter ) )
			return startBoolResultError( functionNameString, NULL, itemString(), "The given adjective parameter is not an adjective parameter" );

		if( adjectiveParameter_ == NO_ADJECTIVE_PARAMETER )
			adjectiveParameter_ = adjectiveParameter;
		else
			{
			if( adjectiveParameter_ != adjectiveParameter )
				{
				if( inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addBoolResultError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of a different adjective" );

				// Has found different parameter
				boolResult.booleanValue = true;
				}
			}

		return boolResult;
		}

	BoolResultType setDefiniteArticleParameter( unsigned short definiteArticleParameter )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setDefiniteArticleParameter";

		if( !isDefiniteArticleParameter( definiteArticleParameter ) )
			return startBoolResultError( functionNameString, NULL, itemString(), "The given definite article parameter is not a definite article parameter" );

		if( definiteArticleParameter_ == NO_DEFINITE_ARTICLE_PARAMETER )
			definiteArticleParameter_ = definiteArticleParameter;
		else
			{
			if( definiteArticleParameter_ != definiteArticleParameter )
				{
				if( inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addBoolResultError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of a different defnite article" );

				// Has found different parameter
				boolResult.booleanValue = true;
				}
			}

		return boolResult;
		}

	BoolResultType setIndefiniteArticleParameter( unsigned short indefiniteArticleParameter )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setIndefiniteArticleParameter";

		if( !isIndefiniteArticleParameter( indefiniteArticleParameter ) )
			return startBoolResultError( functionNameString, NULL, itemString(), "The given indefinite article parameter is not an indefinite article parameter" );

		if( indefiniteArticleParameter_ == NO_INDEFINITE_ARTICLE_PARAMETER )
			indefiniteArticleParameter_ = indefiniteArticleParameter;
		else
			{
			if( indefiniteArticleParameter_ != indefiniteArticleParameter )
				{
				if( inputOutput()->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return addBoolResultError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of an indefinite article" );

				// Has found different parameter
				boolResult.booleanValue = true;
				}
			}

		return boolResult;
		}


	protected:
	// Constructor

	WordTypeItem( bool hasFeminineWordEnding, bool hasMasculineWordEnding, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeLanguageNr, unsigned short wordTypeNr, size_t wordTypeStringLength, char *wordTypeString, CommonVariables *commonVariables, InputOutput *inputOutput, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WordTypeItem", commonVariables, inputOutput, myList, myWordItem );

		// Private initialized variables

		hasFeminineWordEnding_ = hasFeminineWordEnding;
		hasMasculineWordEnding_ = hasMasculineWordEnding;
		isProperNamePrecededByDefiniteArticle_ = isProperNamePrecededByDefiniteArticle;

		adjectiveParameter_ = adjectiveParameter;
		definiteArticleParameter_ = definiteArticleParameter;
		indefiniteArticleParameter_ = indefiniteArticleParameter;
		wordTypeLanguageNr_ = wordTypeLanguageNr;
		wordTypeNr_ = wordTypeNr;

		// Private constructed variables

		generalizationWriteLevel_ = NO_WRITE_LEVEL;
		specificationWriteLevel_ = NO_WRITE_LEVEL;
		relationWriteLevel_ = NO_WRITE_LEVEL;

		hideKey_ = NULL;
		wordTypeString_ = NULL;

		// Checking private initialized variables

		if( wordTypeString != NULL )
			{
			if( strlen( wordTypeString ) > 0 )
				{
				if( wordTypeStringLength > 0 )
					{
					if( wordTypeStringLength < MAX_SENTENCE_STRING_LENGTH &&
					strlen( wordTypeString ) < MAX_SENTENCE_STRING_LENGTH )
						{
						if( ( wordTypeString_ = new char[wordTypeStringLength + 1] ) != NULL )
							{
							strcpy( wordTypeString_, EMPTY_STRING );
							strncat( wordTypeString_, wordTypeString, wordTypeStringLength );

							if( isupper( wordTypeString[0] ) &&
							wordTypeNr != WORD_TYPE_LETTER_CAPITAL &&
							wordTypeNr != WORD_TYPE_PROPER_NAME )
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
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Display status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, itemString() );
			}
		}

	virtual bool hasParameter( unsigned int queryParameter )
		{
		return ( adjectiveParameter_ == queryParameter ||
				definiteArticleParameter_ == queryParameter ||
				indefiniteArticleParameter_ == queryParameter ||
				generalizationWriteLevel_ == queryParameter ||
				specificationWriteLevel_ == queryParameter ||
				relationWriteLevel_ == queryParameter ||
				wordTypeLanguageNr_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( adjectiveParameter_ > NO_ADJECTIVE_PARAMETER ||
				definiteArticleParameter_ > NO_DEFINITE_ARTICLE_PARAMETER ||
				indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER ||
				generalizationWriteLevel_ > NO_WRITE_LEVEL ||
				specificationWriteLevel_ > NO_WRITE_LEVEL ||
				relationWriteLevel_ > NO_WRITE_LEVEL ||
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
		char *queryString;
		char *wordString;
		char *wordTypeString = myWordItem()->wordTypeNameString( wordTypeNr_ );
		char *languageNameString = myWordItem()->languageNameString( wordTypeLanguageNr_ );

		itemBaseToString( queryWordTypeNr );

		queryString = commonVariables()->queryString;

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

		if( isProperNamePrecededByDefiniteArticle_ )
			{
			strcat( queryString, QUERY_SEPARATOR_STRING );
			strcat( queryString, "isProperNamePrecededByDefiniteArticle" );
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
		bool isStringStartingWithVowel;
		bool isVowelIndefiniteArticle;
		bool isIndefiniteArticleParameter = ( indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER );

		if( isCheckingForEqualParameters &&
		indefiniteArticleParameter_ == indefiniteArticleParameter )
			return true;

		// Typically for English ('a' or 'an')
		// If undefined, fall back to a simple phonetic vowel rule
		if( isIndefinitePhoneticVowelArticle( isIndefiniteArticleParameter ? indefiniteArticleParameter_ : indefiniteArticleParameter ) )
			{
			isStringStartingWithVowel = doesStringStartWithPhoneticVowel( itemString() );
			isVowelIndefiniteArticle = isIndefiniteArticlePhoneticVowelParameter( indefiniteArticleParameter );

			return ( ( !isStringStartingWithVowel &&	// 'a'
					!isVowelIndefiniteArticle ) ||

					( isStringStartingWithVowel &&		// 'an'
					isVowelIndefiniteArticle ) );
			}

		if( isIndefiniteArticleParameter )
			return !isCheckingForEqualParameters;

		return ( ( definiteArticleParameter_ != WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				definiteArticleParameter_ != WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE ) ||

				( definiteArticleParameter_ == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				indefiniteArticleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ) ||

				( definiteArticleParameter_ == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE &&
				indefiniteArticleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) );
		}

	bool isProperNamePrecededByDefiniteArticle( unsigned short definiteArticleParameter )
		{
		return ( isProperNamePrecededByDefiniteArticle_ &&
				isCorrectDefiniteArticle( definiteArticleParameter ) );
		}

	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, char *hideKey )
		{
		if( wordTypeNr_ == wordTypeNr &&
		hideKey_ == hideKey &&
		compareString != NULL &&
		strcmp( wordTypeString_, compareString ) == 0 )
			return true;

		return false;
		}

	bool isAdjective()
		{
		return ( wordTypeNr_ == WORD_TYPE_ADJECTIVE );
		}

	bool isAdverb()
		{
		return ( wordTypeNr_ == WORD_TYPE_ADVERB );
		}

	bool isAnswer()
		{
		return ( wordTypeNr_ == WORD_TYPE_ANSWER );
		}

	bool isArticle()
		{
		return ( wordTypeNr_ == WORD_TYPE_ARTICLE );
		}

	bool isConjunction()
		{
		return ( wordTypeNr_ == WORD_TYPE_CONJUNCTION );
		}

	bool isDefiniteArticle()
		{
		// Filter on articles, because nouns also have a definiteArticleParameter
		return ( wordTypeNr_ == WORD_TYPE_ARTICLE &&
				isDefiniteArticleParameter( definiteArticleParameter_ ) );
		}

	bool isIndefiniteArticle()
		{
		// Filter on articles, because nouns also have an indefiniteArticleParameter
		return ( wordTypeNr_ == WORD_TYPE_ARTICLE &&
				isIndefiniteArticleParameter( indefiniteArticleParameter_ ) );
		}

	bool isNoun()
		{
		return isNounWordType( wordTypeNr_ );
		}

	bool isSingularNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_SINGULAR );
		}

	bool isPluralNoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_NOUN_PLURAL );
		}

	bool isPossessiveDeterminer()
		{
		return ( wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL );
		}

	bool isPossessivePronoun()
		{
		return ( wordTypeNr_ == WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL );
		}

	bool isPreposition()
		{
		return ( wordTypeNr_ == WORD_TYPE_PREPOSITION );
		}

	bool isSymbol()
		{
		return ( wordTypeNr_ == WORD_TYPE_SYMBOL );
		}

	bool isNumeral()
		{
		return ( wordTypeNr_ == WORD_TYPE_NUMERAL );
		}

	bool isVerb()
		{
		return ( wordTypeNr_ == WORD_TYPE_VERB_SINGULAR ||
				wordTypeNr_ == WORD_TYPE_VERB_PLURAL );
		}

	bool isGeneralizationWordAlreadyWritten()
		{
		return ( generalizationWriteLevel_ > NO_WRITE_LEVEL );
		}

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

	unsigned short wordTypeNr()
		{
		return wordTypeNr_;
		}

	unsigned short wordTypeLanguageNr()
		{
		return wordTypeLanguageNr_;
		}

	signed char markGeneralizationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( commonVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		if( generalizationWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );

		generalizationWriteLevel_ = ++commonVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	signed char markSpecificationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( commonVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		if( specificationWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );

		specificationWriteLevel_ = ++commonVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	signed char markRelationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markRelationWordTypeAsWritten";

		if( commonVariables()->currentWriteLevel >= MAX_LEVEL )
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		if( relationWriteLevel_ > NO_WRITE_LEVEL )
			return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );

		relationWriteLevel_ = ++commonVariables()->currentWriteLevel;

		return RESULT_OK;
		}

	signed char hideWordType( char *hideKey )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "hideWordType";

		if( hideKey_ != NULL )
			return startError( functionNameString, NULL, itemString(), "This word type is already hidden" );

		hideKey_ = hideKey;

		return RESULT_OK;
		}

	WordTypeItem *nextWordTypeItem()
		{
		return (WordTypeItem *)nextItem;
		}

	WordTypeItem *nextCurrentLanguageWordTypeItem()
		{
		WordTypeItem *nextCurrentLanguageWordTypeItem = nextWordTypeItem();

		return ( nextCurrentLanguageWordTypeItem != NULL &&
				nextCurrentLanguageWordTypeItem->wordTypeLanguageNr() == commonVariables()->currentLanguageNr ? nextCurrentLanguageWordTypeItem : NULL );
		}

	WordTypeItem *nextWordTypeItem( unsigned short wordTypeNr )
		{
		WordTypeItem *searchWordTypeItem = nextCurrentLanguageWordTypeItem();

		while( searchWordTypeItem != NULL )
			{
			if( wordTypeNr == NO_WORD_TYPE_NR ||
			searchWordTypeItem->wordTypeNr_ == wordTypeNr )
				return searchWordTypeItem;

			searchWordTypeItem = searchWordTypeItem->nextCurrentLanguageWordTypeItem();
			}

		return NULL;
		}

	BoolResultType setParametersOfSingularNoun( unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setParametersOfSingularNoun";

		if( !isSingularNoun() )
			return startBoolResultError( functionNameString, NULL, itemString(), "I am not a singular noun" );

		if( adjectiveParameter > NO_ADJECTIVE_PARAMETER )
			return setAdjectiveParameter( adjectiveParameter );

		if( definiteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER )
			return setDefiniteArticleParameter( definiteArticleParameter );

		if( indefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER )
			return setIndefiniteArticleParameter( indefiniteArticleParameter );

		return boolResult;
		}
	};
#endif

/*************************************************************************
 *	"The Lord gives his people strength.
 *	The Lord blesses them with peace." (Psalm 29:11)
 *************************************************************************/
