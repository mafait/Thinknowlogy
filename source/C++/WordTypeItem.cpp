/*	Class:			WordTypeItem
 *	Parent class:	Item
 *	Purpose:		To store the word types of a word
 *	Version:		Thinknowlogy 2016r1 (Huguenot)
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

#ifndef WORDTYPEITEM
#define WORDTYPEITEM 1
#include "Presentation.cpp"

class WordTypeItem : private Item
	{
	friend class AdminCleanup;
	friend class AdminReadCreateWords;
	friend class AdminReadGrammar;
	friend class AdminReadSentence;
	friend class WordItem;
	friend class WordSpecification;
	friend class WordType;
	friend class WordTypeList;

	// Private loadable variables

	bool hasFeminineWordEnding_;
	bool hasMasculineWordEnding_;
	bool isProperNamePrecededByDefiniteArticle_;

	unsigned short adjectiveParameter_;
	unsigned short definiteArticleParameter_;
	unsigned short indefiniteArticleParameter_;
	unsigned short wordTypeLanguageNr_;
	unsigned short wordTypeNr_;


	// Private constructible variables

	unsigned short generalizationWriteLevel_;
	unsigned short specificationWriteLevel_;
	unsigned short relationWriteLevel_;

	char *wordTypeString_;

	void *hideKey_;


	// Private functions

	bool hasIndefinitePhoneticVowelArticle( unsigned short indefiniteArticleParameter )
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

	WordResultType setAdjectiveParameter( unsigned short adjectiveParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setAdjectiveParameter";

		if( isAdjectiveParameter( adjectiveParameter ) )
			{
			if( isSingularNoun() )
				{
				if( adjectiveParameter_ == NO_ADJECTIVE_PARAMETER )
					adjectiveParameter_ = adjectiveParameter;
				else
					{
					if( adjectiveParameter_ != adjectiveParameter )
						{
						if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == RESULT_OK )
							wordResult.hasFoundDifferentParameter = true;
						else
							addError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of a different adjective" );
						}
					}
				}
			else
				startError( functionNameString, NULL, itemString(), "I am not a singular noun" );
			}
		else
			startError( functionNameString, NULL, itemString(), "The given adjective parameter is not an adjective parameter" );

		wordResult.result = commonVariables()->result;
		return wordResult;
		}

	WordResultType setDefiniteArticleParameter( unsigned short definiteArticleParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setDefiniteArticleParameter";

		if( isDefiniteArticleParameter( definiteArticleParameter ) )
			{
			if( isSingularNoun() )
				{
				if( definiteArticleParameter_ == NO_DEFINITE_ARTICLE_PARAMETER )
					definiteArticleParameter_ = definiteArticleParameter;
				else
					{
					if( definiteArticleParameter_ != definiteArticleParameter )
						{
						if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_DEFINITE_ARTICLE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == RESULT_OK )
							wordResult.hasFoundDifferentParameter = true;
						else
							addError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of a different defnite article" );
						}
					}
				}
			else
				startError( functionNameString, NULL, itemString(), "I am not a singular noun" );
			}
		else
			startError( functionNameString, NULL, itemString(), "The given definite article parameter is not a definite article parameter" );

		wordResult.result = commonVariables()->result;
		return wordResult;
		}


	protected:
	// Constructor / deconstructor

	WordTypeItem( bool hasFeminineWordEnding, bool hasMasculineWordEnding, bool isProperNamePrecededByDefiniteArticle, unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter, unsigned short wordTypeLanguageNr, unsigned short wordTypeNr, size_t wordTypeStringLength, char *_wordTypeString, CommonVariables *commonVariables, List *myList, WordItem *myWordItem )
		{
		initializeItemVariables( NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, "WordTypeItem", commonVariables, myList, myWordItem );

		// Private loadable variables

		hasFeminineWordEnding_ = hasFeminineWordEnding;
		hasMasculineWordEnding_ = hasMasculineWordEnding;
		isProperNamePrecededByDefiniteArticle_ = isProperNamePrecededByDefiniteArticle;

		adjectiveParameter_ = adjectiveParameter;
		definiteArticleParameter_ = definiteArticleParameter;
		indefiniteArticleParameter_ = indefiniteArticleParameter;
		wordTypeLanguageNr_ = wordTypeLanguageNr;
		wordTypeNr_ = wordTypeNr;

		// Private constructible variables

		generalizationWriteLevel_ = NO_WRITE_LEVEL;
		specificationWriteLevel_ = NO_WRITE_LEVEL;
		relationWriteLevel_ = NO_WRITE_LEVEL;

		hideKey_ = NULL;
		wordTypeString_ = NULL;

		if( _wordTypeString != NULL )
			{
			if( strlen( _wordTypeString ) > 0 )
				{
				if( wordTypeStringLength > 0 )
					{
					if( wordTypeStringLength < MAX_SENTENCE_STRING_LENGTH &&
					strlen( _wordTypeString ) < MAX_SENTENCE_STRING_LENGTH )
						{
						if( ( wordTypeString_ = new char[wordTypeStringLength + 1] ) != NULL )
							{
							strcpy( wordTypeString_, EMPTY_STRING );
							strncat( wordTypeString_, _wordTypeString, wordTypeStringLength );

							if( isupper( _wordTypeString[0] ) &&
							wordTypeNr != WORD_TYPE_LETTER_CAPITAL &&
							wordTypeNr != WORD_TYPE_PROPER_NAME )
								wordTypeString_[0] = (char)tolower( wordTypeString_[0] );
							}
						else
							startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "I failed to create the word type string" );
						}
					else
						startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type is too long" );
					}
				else
					startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type string length is undefined" );
				}
			else
				startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type string is empty" );
			}
		else
			startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, NULL, NULL, "The given word type string is undefined" );
		}

	~WordTypeItem()
		{
		if( wordTypeString_ != NULL )
			delete wordTypeString_;
		}


	// Protected virtual functions

	virtual void showString( bool isReturnQueryToPosition )
		{
		statusString[0] = statusChar();

		if( itemString() != NULL )
			{
			if( commonVariables()->hasFoundQuery )
				strcat( commonVariables()->queryString, ( isReturnQueryToPosition ? NEW_LINE_STRING : QUERY_SEPARATOR_SPACE_STRING ) );

			// Show status if not active
			if( !isActiveItem() )
				strcat( commonVariables()->queryString, statusString );

			commonVariables()->hasFoundQuery = true;
			strcat( commonVariables()->queryString, itemString() );
			}
		}

	virtual bool hasFoundParameter( unsigned int queryParameter )
		{
		return ( adjectiveParameter_ == queryParameter ||
				definiteArticleParameter_ == queryParameter ||
				indefiniteArticleParameter_ == queryParameter ||
				generalizationWriteLevel_ == queryParameter ||
				specificationWriteLevel_ == queryParameter ||
				relationWriteLevel_ == queryParameter ||

				( queryParameter == MAX_QUERY_PARAMETER &&

				( adjectiveParameter_ > NO_ADJECTIVE_PARAMETER ||
				definiteArticleParameter_ > NO_DEFINITE_ARTICLE_PARAMETER ||
				indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER ||
				generalizationWriteLevel_ > NO_WRITE_LEVEL ||
				specificationWriteLevel_ > NO_WRITE_LEVEL ||
				relationWriteLevel_ > NO_WRITE_LEVEL ) ) );
		}

	virtual bool hasFoundWordType( unsigned short queryWordTypeNr )
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

	virtual char *toString( unsigned short queryWordTypeNr )
		{
		char *queryString;
		char *wordString;
		char *wordTypeString = myWordItem()->wordTypeNameString( wordTypeNr_ );
		char *languageNameString = myWordItem()->languageNameString( wordTypeLanguageNr_ );

		Item::toString( queryWordTypeNr );

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

	bool hasFeminineWordEnding()
		{
		return hasFeminineWordEnding_;
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
		bool hasIndefiniteArticleParameter = ( indefiniteArticleParameter_ > NO_INDEFINITE_ARTICLE_PARAMETER );

		if( isCheckingForEqualParameters &&
		indefiniteArticleParameter_ == indefiniteArticleParameter )
			return true;

		// Typically for English ('a' or 'an')
		// If undefined, fall back to a simple phonetic vowel rule
		if( hasIndefinitePhoneticVowelArticle( hasIndefiniteArticleParameter ? indefiniteArticleParameter_ : indefiniteArticleParameter ) )
			{
			isStringStartingWithVowel = isStartingWithPhoneticVowel( itemString() );
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

				( ( definiteArticleParameter_ == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_FEMININE &&
				indefiniteArticleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_FEMININE ) ||

				( definiteArticleParameter_ == WORD_PARAMETER_ARTICLE_DEFINITE_SINGULAR_MASCULINE &&
				indefiniteArticleParameter == WORD_PARAMETER_ARTICLE_INDEFINITE_SINGULAR_MASCULINE ) ) );
		}

	bool isProperNamePrecededByDefiniteArticle( unsigned short definiteArticleParameter )
		{
		return ( isProperNamePrecededByDefiniteArticle_ &&
				isCorrectDefiniteArticle( definiteArticleParameter ) );
		}

	bool isCorrectHiddenWordType( unsigned short wordTypeNr, char *compareString, void *hideKey )
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

	bool isSingularOrPluralNounWordType()
		{
		return isSingularOrPluralNoun( wordTypeNr_ );
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

	ResultType createNewWordTypeString( char *newWordTypeString )
		{
		size_t newWordTypeStringLength;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createNewWordTypeString";

		if( newWordTypeString != NULL )
			{
			if( ( newWordTypeStringLength = strlen( newWordTypeString ) ) > 0 )
				{
				if( newWordTypeStringLength < MAX_SENTENCE_STRING_LENGTH )
					{
					if( wordTypeString_ != NULL )
						delete wordTypeString_;

					if( ( wordTypeString_ = new char[newWordTypeStringLength + 1] ) != NULL )
						strcpy( wordTypeString_, newWordTypeString );
					else
						return startError( functionNameString, NULL, itemString(), "I failed to create the word type string" );
					}
				else
					return startError( functionNameString, NULL, itemString(), "The given word type is too long" );
				}
			else
				return startError( functionNameString, NULL, itemString(), "The given new word type string is empty" );
			}
		else
			return startError( functionNameString, NULL, itemString(), "The given new word type string is undefined" );

		return RESULT_OK;
		}

	ResultType markGeneralizationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markGeneralizationWordTypeAsWritten";

		if( commonVariables()->currentWriteLevel < MAX_LEVEL )
			{
			if( generalizationWriteLevel_ == NO_WRITE_LEVEL )
				generalizationWriteLevel_ = ++commonVariables()->currentWriteLevel;
			else
				return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );
			}
		else
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		return RESULT_OK;
		}

	ResultType markSpecificationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markSpecificationWordTypeAsWritten";

		if( commonVariables()->currentWriteLevel < MAX_LEVEL )
			{
			if( specificationWriteLevel_ == NO_WRITE_LEVEL )
				specificationWriteLevel_ = ++commonVariables()->currentWriteLevel;
			else
				return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );
			}
		else
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		return RESULT_OK;
		}

	ResultType markRelationWordTypeAsWritten()
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "markRelationWordTypeAsWritten";

		if( commonVariables()->currentWriteLevel < MAX_LEVEL )
			{
			if( relationWriteLevel_ == NO_WRITE_LEVEL )
				relationWriteLevel_ = ++commonVariables()->currentWriteLevel;
			else
				return startError( functionNameString, NULL, itemString(), "My write level is already assigned" );
			}
		else
			return startSystemError( functionNameString, NULL, itemString(), "Current write word level overflow" );

		return RESULT_OK;
		}

	ResultType hideWordType( void *hideKey )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "hideWordType";

		if( hideKey_ == NULL )
			hideKey_ = hideKey;
		else
			return startError( functionNameString, NULL, itemString(), "This word type is already hidden" );

		return RESULT_OK;
		}

	WordResultType setIndefiniteArticleParameter( unsigned short indefiniteArticleParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setIndefiniteArticleParameter";

		if( indefiniteArticleParameter == NO_INDEFINITE_ARTICLE_PARAMETER )
			indefiniteArticleParameter_ = NO_INDEFINITE_ARTICLE_PARAMETER;
		else
			{
			if( isIndefiniteArticleParameter( indefiniteArticleParameter ) )
				{
				if( isSingularNoun() )
					{
					if( indefiniteArticleParameter_ == NO_INDEFINITE_ARTICLE_PARAMETER )
						indefiniteArticleParameter_ = indefiniteArticleParameter;
					else
						{
						if( indefiniteArticleParameter_ != indefiniteArticleParameter )
							{
							if( commonVariables()->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, itemString(), INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == RESULT_OK )
								wordResult.hasFoundDifferentParameter = true;
							else
								addError( functionNameString, NULL, itemString(), "I failed to write an interface notification about the use of an indefinite article" );
							}
						}
					}
				else
					startError( functionNameString, NULL, itemString(), "I am not a singular noun" );
				}
			else
				startError( functionNameString, NULL, itemString(), "The given indefinite article parameter is not an indefinite article parameter" );
			}

		wordResult.result = commonVariables()->result;
		return wordResult;
		}

	WordResultType setParameter( unsigned short adjectiveParameter, unsigned short definiteArticleParameter, unsigned short indefiniteArticleParameter )
		{
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "setParameter";

		if( adjectiveParameter > NO_ADJECTIVE_PARAMETER )
			{
			if( ( wordResult = setAdjectiveParameter( adjectiveParameter ) ).result != RESULT_OK )
				addError( functionNameString, NULL, itemString(), "I failed to set my adjective parameter" );
			}
		else
			{
			if( definiteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER )
				{
				if( ( wordResult = setDefiniteArticleParameter( definiteArticleParameter ) ).result != RESULT_OK )
					addError( functionNameString, NULL, itemString(), "I failed to set my definite article parameter" );
				}
			else
				{
				if( indefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER )
					{
					if( ( wordResult = setIndefiniteArticleParameter( indefiniteArticleParameter ) ).result != RESULT_OK )
						addError( functionNameString, NULL, itemString(), "I failed to set the indefinite article parameter of a singular noun" );
					}
				}
			}

		return wordResult;
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
		WordTypeItem *searchItem = nextCurrentLanguageWordTypeItem();

		while( searchItem != NULL )
			{
			if( wordTypeNr == WORD_TYPE_UNDEFINED ||
			searchItem->wordTypeNr_ == wordTypeNr )
				return searchItem;

			searchItem = searchItem->nextCurrentLanguageWordTypeItem();
			}

		return NULL;
		}
	};
#endif

/*************************************************************************
 *	"The Lord gives his people strength.
 *	The Lord blesses them with peace." (Psalm 29:11)
 *************************************************************************/
