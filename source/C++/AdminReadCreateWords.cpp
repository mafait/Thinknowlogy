/*	Class:			AdminReadCreateWords
 *	Supports class:	AdminItem
 *	Purpose:		To create words of the read sentence
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

#include "AdminItem.h"
#include "CreateReadWordResultType.cpp"
#include "ReadList.cpp"
#include "WordEndingResultType.cpp"
#include "WordList.cpp"
#include "WordTypeItem.cpp"

class AdminReadCreateWords
	{
	friend class AdminItem;

	// Private constructed variables

	unsigned short lastCreatedWordOrderNr_;


	// Private initialized variables

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH];

	AdminItem *adminItem_;
	GlobalVariables *globalVariables_;
	InputOutput *inputOutput_;


	// Private functions

	bool isGrammarDefinitionSymbol( char character )
		{
		return ( character == QUERY_WORD_TYPE_CHAR ||
				character == QUERY_PARAMETER_CHAR ||
				character == GRAMMAR_WORD_DEFINITION_CHAR );
		}

	bool isSymbol( char character )
		{
		return ( character == SYMBOL_COMMA ||
				character == SYMBOL_COLON ||
				character == SYMBOL_SEMI_COLON ||
				character == SYMBOL_DOUBLE_COLON ||
				character == SYMBOL_EXCLAMATION_MARK ||
				character == SYMBOL_QUESTION_MARK ||
#ifdef _MSC_VER
				character == SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK ||
				character == SYMBOL_SPANISH_INVERTED_QUESTION_MARK ||
#endif
				character == SYMBOL_SLASH ||
				character == SYMBOL_BACK_SLASH ||
				// Don't add SYMBOL_QUOTE to avoid analyzing merged words
				// Don't add SYMBOL_DOUBLE_QUOTE to avoid analyzing text strings
				character == SYMBOL_OPEN_ROUNDED_BRACKET ||
				character == SYMBOL_CLOSE_ROUNDED_BRACKET );
		}

	CreateReadWordResultType createReadWord( bool isUncountableGeneralizationNoun, unsigned short wordOrderNr, unsigned short wordTypeNr, size_t textStringStartPosition, char *textString, WordItem *readWordItem )
		{
		unsigned short wordParameter = ( wordTypeNr == WORD_TYPE_NOUN_PLURAL ||
										readWordItem == NULL ? NO_WORD_PARAMETER : readWordItem->wordParameter() );
		CreateReadWordResultType createReadWordResult;
		ReadWordResultType readWordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createReadWord";

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return adminItem_->startCreateReadWordResultError( functionNameString, moduleNameString_, "The given word type number is undefined or out of bounds" );

		if( textString == NULL &&
		readWordItem == NULL )
			return adminItem_->startCreateReadWordResultError( functionNameString, moduleNameString_, "Both the given text string and the given read word item are undefined" );

		if( textString != NULL )
			{
			if( ( readWordResult = readWordFromString( false, false, true, 0, &textString[textStringStartPosition] ) ).result != RESULT_OK )
				return adminItem_->addCreateReadWordResultError( functionNameString, moduleNameString_, "I failed to read a word from a text string" );

			createReadWordResult.offset = readWordResult.offset;
			}

		if( adminItem_->createReadItem( isUncountableGeneralizationNoun, wordOrderNr, wordParameter, wordTypeNr, readWordResult.wordLength, ( textString == NULL ? NULL : &textString[textStringStartPosition + readWordResult.startWordPosition] ), readWordItem ) != RESULT_OK )
			return adminItem_->addCreateReadWordResultError( functionNameString, moduleNameString_, "I failed to create a read word item" );

		lastCreatedWordOrderNr_ = wordOrderNr;

		createReadWordResult.hasCreatedReadWord = true;
		return createReadWordResult;
		}

	ShortResultType getWordTypeNr( bool isCheckingProperNoun, size_t wordTypeStringLength, char *wordTypeString )
		{
		char wordTypeChar;
		size_t index = 0;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "getWordTypeNr";

		if( wordTypeString == NULL )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "The given word type string is undefined" );

		if( wordTypeStringLength <= 0 )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "The given word type string is empty" );

		wordTypeChar = wordTypeString[0];

		if( isalpha( wordTypeChar ) )
			{
			if( wordTypeStringLength == 1 )
				shortResult.shortValue = ( isupper( wordTypeChar ) ? WORD_TYPE_LETTER_CAPITAL : ( islower( wordTypeChar ) ? WORD_TYPE_LETTER_SMALL : WORD_TYPE_PROPER_NOUN ) );
			else
				{
				if( isCheckingProperNoun &&
				isupper( wordTypeChar ) )
					shortResult.shortValue = WORD_TYPE_PROPER_NOUN;
				}
			}
		else
			{
			// Check for number
			while( index < wordTypeStringLength &&
			isdigit( wordTypeString[index] ) )
				index++;

			if( index == wordTypeStringLength )
				shortResult.shortValue = WORD_TYPE_NUMERAL;
			}


		return shortResult;
		}


	protected:
	// Constructor

	AdminReadCreateWords( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		lastCreatedWordOrderNr_ = NO_ORDER_NR;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminReadCreateWords" );

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	unsigned short lastCreatedWordOrderNr()
		{
		return lastCreatedWordOrderNr_;
		}

	BoolResultType createReadWords( bool isChineseCurrentLanguage, char *readUserSentenceString )
		{
		bool hasFoundAdjectiveEvery = false;
		bool hasFoundChineseDefiniteArticle = false;
		bool hasFoundLowerCase;
		bool hasFoundProperNoun = false;
		bool hasFoundSingularNoun;
		bool hasFoundWordEnding;
		bool isAdjective = false;
		bool isAdverb;
		bool isArticle = false;
		bool isBasicVerb = false;
		bool isCapitalPluralVerb;
		bool isChineseImperativeVerbDisplay = false;
		bool isConjunction = false;
		bool isCreatedSingularNoun = false;
		bool isExactPluralNoun = false;
		bool isExactSingularNoun = false;
		bool isExactWord;
		bool isFirstFind;
		bool isFirstWord = true;
		bool isFrenchNumeral = false;
		bool isPartOfMultipleWord;
		bool isPossessiveDeterminer = false;
		bool isPossessivePronoun = false;
		bool isPreposition = false;
		bool isSelection = false;
		bool isSymbol = false;
		bool isUncountableGeneralizationNoun;
		bool isUndefinedWord = false;
		bool isUpperChar;
		bool wasPreviousWordAdjective;
		bool wasPreviousWordArticle;
		bool wasPreviousWordBasicVerb;
		bool wasPreviousWordChineseImperativeVerbDisplay;
		bool wasPreviousWordConjunction;
		bool wasPreviousWordCreatedSingularNoun;
		bool wasPreviousWordExactNoun;
		bool wasPreviousWordFrenchNumeral;
		bool wasPreviousWordPossessiveDeterminer;
		bool wasPreviousWordPreposition;
		bool wasPreviousWordSymbol;
		bool wasPreviousWordUndefined;
		unsigned short currentLanguageNr = globalVariables_->currentLanguageNr;
		unsigned short currentWordAdjectiveParameter = NO_ADJECTIVE_PARAMETER;
		unsigned short currentWordDefiniteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
		unsigned short currentWordIndefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;
		unsigned short currentWordOrderNr = NO_ORDER_NR;
		unsigned short foundWordTypeNr;
		unsigned short previousWordAdjectiveParameter;
		unsigned short previousWordDefiniteArticleParameter;
		unsigned short previousWordIndefiniteArticleParameter;
		unsigned short wordTypeNr = NO_WORD_TYPE_NR;
		char currentChar;
		size_t currentPosition = 0;
		size_t readUserSentenceStringLength;
		size_t wordStringLength;
		WordItem *createdWordItem;
		WordItem *foundWordItem;
		WordItem *pluralNounWordItem;
		WordItem *singularNounWordItem;
		WordTypeItem *foundWordTypeItem;
		WordTypeItem *tempWordTypeItem;
		char exactWordString[MAX_WORD_LENGTH] = EMPTY_STRING;
		char lowercaseWordString[MAX_WORD_LENGTH] = EMPTY_STRING;
		char singularNounWordString[MAX_WORD_LENGTH] = EMPTY_STRING;
		BoolResultType boolResult;
		CreateReadWordResultType createReadWordResult;
		ReadWordResultType readWordResult;
		ShortResultType shortResult;
		WordEndingResultType wordEndingResult;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createReadWords";

		// Set to pass while loop for the first time
		if( readUserSentenceString == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given read user sentence string is undefined" );

		if( ( readUserSentenceStringLength = strlen( readUserSentenceString ) ) == 0 )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given read user sentence string is empty" );

		do	{
			createReadWordResult.hasCreatedReadWord = false;

			if( ++currentWordOrderNr >= MAX_ORDER_NR )
				return adminItem_->startBoolResultSystemError( functionNameString, moduleNameString_, "Word order number overflow! I can't accept more words" );

			if( ( currentChar = readUserSentenceString[currentPosition] ) == QUERY_STRING_START_CHAR )
				{
				if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, WORD_TYPE_TEXT, currentPosition, readUserSentenceString, NULL ) ).result != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create a read word" );

				currentPosition += createReadWordResult.offset;
				}
			else
				{
				wasPreviousWordAdjective = isAdjective;
				wasPreviousWordArticle = isArticle;
				wasPreviousWordBasicVerb = isBasicVerb;
				wasPreviousWordChineseImperativeVerbDisplay = isChineseImperativeVerbDisplay;
				wasPreviousWordConjunction = isConjunction;
				wasPreviousWordCreatedSingularNoun = isCreatedSingularNoun;
				wasPreviousWordExactNoun = ( isExactSingularNoun ||
											isExactPluralNoun );
				wasPreviousWordFrenchNumeral = isFrenchNumeral;
				wasPreviousWordPossessiveDeterminer = isPossessiveDeterminer;
				wasPreviousWordPreposition = isPreposition;
				wasPreviousWordSymbol = isSymbol;
				wasPreviousWordUndefined = isUndefinedWord;

				previousWordAdjectiveParameter = currentWordAdjectiveParameter;
				previousWordDefiniteArticleParameter = currentWordDefiniteArticleParameter;
				previousWordIndefiniteArticleParameter = currentWordIndefiniteArticleParameter;

				hasFoundLowerCase = false;
				isAdjective = false;
				isAdverb = false;
				isArticle = false;
				isBasicVerb = false;
				isCapitalPluralVerb = false;
				isChineseImperativeVerbDisplay = false;
				isConjunction = false;
				isCreatedSingularNoun = false;
				isExactPluralNoun = false;
				isExactSingularNoun = false;
				isExactWord = false;
				isFrenchNumeral = false;
				isPartOfMultipleWord = true;
				isPossessiveDeterminer = false;
				isPossessivePronoun = false;
				isPreposition = false;
				isSymbol = false;
				isUndefinedWord = false;

				currentWordAdjectiveParameter = NO_ADJECTIVE_PARAMETER;
				currentWordDefiniteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
				currentWordIndefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;

				createdWordItem = NULL;
				foundWordItem = NULL;
				pluralNounWordItem = NULL;
				singularNounWordItem = NULL;

				if( ( readWordResult = readWordFromString( false, false, false, 0, &readUserSentenceString[currentPosition] ) ).result != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to read a word from the word string" );

				wordStringLength = readWordResult.wordLength;

				// Create exact word string
				strcpy( exactWordString, EMPTY_STRING );
				strncat( exactWordString, &readUserSentenceString[currentPosition], wordStringLength );

				// Create lowercase word string
				strcpy( lowercaseWordString, exactWordString );

				for( unsigned i = 0; i < wordStringLength; i++ )
					lowercaseWordString[i] = (char)tolower( exactWordString[i] );

				// Step 1: Find exact word types in all words
				do	{
					if( ( wordResult = findWordTypeInAllWords( false, NO_WORD_TYPE_NR, exactWordString, foundWordItem ) ).result != RESULT_OK )
						return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find an exact word type in all words" );

					// Found exact word
					if( ( foundWordItem = wordResult.foundWordItem ) != NULL &&
					( foundWordTypeItem = wordResult.foundWordTypeItem ) != NULL )
						{
						hasFoundSingularNoun = foundWordTypeItem->isSingularNoun();

						if( ( !isAdverb ||
						// Typical for Spanish: Skip answer, when an adverb was found too
						!foundWordTypeItem->isAnswer() ) &&

						( !isChineseCurrentLanguage ||
						// Typical for Chinese
						// Chinese test file: "Connect Four"
						// Chinese test file: "Acceptance - John is a person, John is tall and John is a boy"
						isFirstWord ||
						isSelection ||
						wordStringLength > 1 ||

						( !foundWordItem->isAdjectiveComparison() &&

						( hasFoundAdjectiveEvery ||
						!hasFoundSingularNoun ||
						!wasPreviousWordSymbol ) ) ) )
							{
							foundWordTypeNr = foundWordTypeItem->wordTypeNr();

							// Typical for Chinese: Possessive pronoun and possessive determiner are the same
							// Chinese test file: "Connect Four"
							if( isChineseCurrentLanguage &&
							foundWordTypeNr == WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR &&

							( hasFoundAdjectiveEvery ||
							hasFoundChineseDefiniteArticle ) &&

							( tempWordTypeItem = foundWordItem->activeWordTypeItem( WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR ) ) != NULL )
								{
								// Force Chinese possessive pronoun
								foundWordTypeNr = WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR;
								foundWordTypeItem = tempWordTypeItem;
								}

							if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, ( hasFoundSingularNoun && wasPreviousWordFrenchNumeral ? WORD_TYPE_NOUN_PLURAL : foundWordTypeNr ), 0, NULL, foundWordItem ) ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create another read word" );

							isExactWord = true;

							switch( foundWordTypeNr )
								{
								case WORD_TYPE_SYMBOL:
									isSymbol = true;
									break;

								case WORD_TYPE_NUMERAL:
									// Typical for French
									if( !isChineseCurrentLanguage )
										isFrenchNumeral = true;

									break;

								case WORD_TYPE_PROPER_NOUN:
									hasFoundProperNoun = true;
									break;

								case WORD_TYPE_ADJECTIVE:
									isAdjective = true;

									switch( foundWordItem->wordParameter() )
										{
										case WORD_PARAMETER_ADJECTIVE_NO:
											// Adjective 'no' can be used as article
											isArticle = true;
											break;

										case WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
											hasFoundAdjectiveEvery = true;
										}

									if( !wasPreviousWordArticle )
										currentWordAdjectiveParameter = foundWordTypeItem->adjectiveParameter();

									break;

								case WORD_TYPE_ADVERB:
									isAdverb = true;
									break;

								case WORD_TYPE_ARTICLE:
									isArticle = true;

									if( foundWordTypeItem->isDefiniteArticle() )
										{
										// Definite article
										hasFoundChineseDefiniteArticle = true;
										currentWordDefiniteArticleParameter = foundWordTypeItem->definiteArticleParameter();
										}
									else
										// Indefinite article
										currentWordIndefiniteArticleParameter = foundWordTypeItem->indefiniteArticleParameter();

									break;

								case WORD_TYPE_CONJUNCTION:
									isConjunction = true;
									break;

								case WORD_TYPE_NOUN_PLURAL:
									isExactPluralNoun = true;
									break;

								case WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR:
								case WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL:
									isPossessiveDeterminer = true;
									break;

								case WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR:
								case WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL:
									isPossessivePronoun = true;
									break;

								case WORD_TYPE_PREPOSITION:
									isPreposition = true;
									break;

								case WORD_TYPE_VERB_SINGULAR:
									// Multiple word ends at reaching a verb
									isPartOfMultipleWord = false;

									if( foundWordItem->isBasicVerb() )
										isBasicVerb = true;
									else
										{
										// Typical for Chinese
										if( isChineseCurrentLanguage &&
										foundWordItem->isImperativeVerbDisplay() )
											isChineseImperativeVerbDisplay = true;
										}

									break;

								case WORD_TYPE_SELECTION_WORD:
									isSelection = true;
								}

							if( hasFoundSingularNoun )
								{
								isExactSingularNoun = true;

								if( wasPreviousWordFrenchNumeral )
									{
									// Typical for French: Skip singular/plural noun mismatch of word 'fils'
									if( foundWordItem->addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ).result != RESULT_OK )
										return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add a plural noun word type item for word \"", foundWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( foundWordItem->isMultipleWord() )
										{
										if( wasPreviousWordBasicVerb )
											// Not the first part of a multiple word, but an adjective
											isExactWord = false;
										}
									else
										{
										isPartOfMultipleWord = false;

										if( previousWordAdjectiveParameter > NO_ADJECTIVE_PARAMETER )
											{
											if( !isChineseCurrentLanguage &&
											foundWordTypeItem->setAdjectiveParameter( previousWordAdjectiveParameter ) != RESULT_OK )
												return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to set the adjective parameter of a singular noun" );
											}
										else
											{
											if( previousWordDefiniteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER )
												{
												if( foundWordTypeItem->setDefiniteArticleParameter( previousWordDefiniteArticleParameter ) != RESULT_OK )
													return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to set the definite article parameter of a singular noun" );
												}
											else
												{
												if( previousWordIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
												foundWordTypeItem->setIndefiniteArticleParameter( previousWordIndefiniteArticleParameter ) != RESULT_OK )
													return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to set the indefinite article parameter of a singular noun" );
												}
											}
										}
									}
								}
							}
						}
					}
				// Allow multiple finds
				while( foundWordItem != NULL );

				if( !isBasicVerb &&
				!isPartOfMultipleWord &&
				!wasPreviousWordExactNoun &&

				// Delete obsolete read items, that where part of a mutliple word
				adminItem_->deleteReadItemsWithNonMatchingMultipleWordPart( currentWordOrderNr, &readUserSentenceString[currentPosition] ) != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to delete the read items with a non-matching multiple word part" );

				isUpperChar = isupper( currentChar );

				if( isFirstWord &&
				isUpperChar &&

				( !isExactWord ||
				wordStringLength == 1 ) )
					{
					if( ( shortResult = getWordTypeNr( false, wordStringLength, lowercaseWordString ) ).result != RESULT_OK )
						return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to get the word type number of a lowercase word" );

					if( ( wordTypeNr = shortResult.shortValue ) == NO_WORD_TYPE_NR )
						isUndefinedWord = true;

					// Step 2: Find word type with lowercase first letter in all words
					do	{
						if( ( wordResult = findWordTypeInAllWords( true, NO_WORD_TYPE_NR, lowercaseWordString, foundWordItem ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find a word type with difference case of the first letter in all words" );

						// Found word type with lowercase first letter
						if( ( foundWordItem = wordResult.foundWordItem ) != NULL &&
						( foundWordTypeItem = wordResult.foundWordTypeItem ) != NULL )
							{
							hasFoundLowerCase = true;
							foundWordTypeNr = foundWordTypeItem->wordTypeNr();

							if( foundWordTypeNr == wordTypeNr &&
							foundWordTypeItem->wordTypeLanguageNr() != currentLanguageNr &&

							// Create same word type for different language
							foundWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, wordStringLength, lowercaseWordString ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add a word type with lowercase first letter" );

							if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, foundWordTypeNr, 0, NULL, foundWordItem ) ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create a read word with a word type with difference case of the first letter" );

							switch( foundWordTypeNr )
								{
								case WORD_TYPE_ADJECTIVE:
									isAdjective = true;
									currentWordAdjectiveParameter = foundWordTypeItem->adjectiveParameter();

									if( foundWordItem->wordParameter() == WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL )
										hasFoundAdjectiveEvery = true;

									break;

								case WORD_TYPE_ARTICLE:
									isArticle = true;

									if( foundWordTypeItem->isDefiniteArticle() )
										// Definite article
										currentWordDefiniteArticleParameter = foundWordTypeItem->definiteArticleParameter();
									else
										// Indefinite article
										currentWordIndefiniteArticleParameter = foundWordTypeItem->indefiniteArticleParameter();

									break;

								case WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR:
								case WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL:
									isPossessiveDeterminer = true;
									break;

								case WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR:
								case WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL:
									isPossessivePronoun = true;
									break;

								case WORD_TYPE_VERB_SINGULAR:
								case WORD_TYPE_VERB_PLURAL:
									isCapitalPluralVerb = true;
									break;

								case WORD_TYPE_SELECTION_WORD:
									isSelection = true;
								}
							}
						}
					// Allow multiple finds
					while( foundWordItem != NULL );

					// One character
					if( !isUndefinedWord )
						{
						// Step 3: Typically for English: Find or create lowercase letter 'a' as first letter of a sentence.
						if( ( wordResult = findWordTypeInAllWords( false, wordTypeNr, lowercaseWordString, NULL ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find a lowercase letter" );

						if( wordResult.foundWordItem == NULL )
							{
							if( ( wordResult = addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, wordTypeNr, wordStringLength, lowercaseWordString ) ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add word with lowercase letter" );

							if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
								return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The created word with lowercase letter is undefined" );

							if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, wordTypeNr, 0, NULL, createdWordItem ) ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create a read word with lowercase letter" );
							}
						}
					}

				// Proper noun
				if( ( !isExactWord &&

				// Skip 'Undo' and 'Redo'
				( !isCapitalPluralVerb ||
				wordStringLength == 1 ) ) ||

				// Typical for Spanish: 'o' and 'y' are letters as well as conjunctions
				// Small letters, capital letters and numerals
				( isConjunction &&
				wasPreviousWordSymbol &&
				wordStringLength == 1 ) )
					{
					isFirstFind = true;

					// Step 4: Find exact word types in all words
					do	{
						if( ( wordResult = findWordTypeInAllWords( true, NO_WORD_TYPE_NR, exactWordString, foundWordItem ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find an exact word" );

						createdWordItem = NULL;
						foundWordItem = wordResult.foundWordItem;
						foundWordTypeItem = wordResult.foundWordTypeItem;

						if( isFirstFind ||
						// Skip if later runs have no result
						foundWordItem != NULL )
							{
							if( ( shortResult = getWordTypeNr( true, wordStringLength, exactWordString ) ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to get the word type number of an exact word" );

							if( ( wordTypeNr = shortResult.shortValue ) == NO_WORD_TYPE_NR )
								isUndefinedWord = true;
							else
								{
								// Not found
								if( foundWordItem == NULL )
									{
									// Small letter, capital letter, numeral, proper noun and singular noun
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, wordTypeNr, wordStringLength, exactWordString ) ).result != RESULT_OK )
										return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add an exact word" );

									if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
										return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "I couldn't create an exact word" );

									if( wordTypeNr == WORD_TYPE_PROPER_NOUN )
										hasFoundProperNoun = true;
									}
								else
									{
									if( foundWordTypeItem != NULL &&
									foundWordTypeItem->wordTypeNr() == wordTypeNr &&
									foundWordTypeItem->wordTypeLanguageNr() != currentLanguageNr )
										{
										// Create same word type for different language
										if( foundWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, wordStringLength, exactWordString ).result != RESULT_OK )
											return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add an exact word type" );

										createdWordItem = foundWordItem;
										}
									}

								if( createdWordItem != NULL )
									{
									if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, wordTypeNr, 0, NULL, createdWordItem ) ).result != RESULT_OK )
										return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create an exact read word" );

									isFirstFind = false;
									}
								}
							}
						}
					// Allow multiple finds
					while( foundWordItem != NULL );
					}

				// Step 5 and 6: Create singular noun and/or plural noun
				if( !isAdjective &&
				!isArticle &&
				!isBasicVerb &&
				!isCapitalPluralVerb &&
				!isConjunction &&
				!isPossessiveDeterminer &&
				!isPossessivePronoun &&
				!isSymbol &&

				( isUndefinedWord ||

				( wordStringLength > 1 &&

				( ( !isUpperChar &&
				wasPreviousWordArticle ) ||

				// Typical for English. Test files: "Boiling point" and "Condensation point"
				( hasFoundAdjectiveEvery &&
				isPreposition &&
				wasPreviousWordCreatedSingularNoun ) ) ) ) )
					{
					if( ( wordEndingResult = adminItem_->analyzeWordEndingWithCurrentLanguage( WORD_PLURAL_NOUN_ENDING, wordStringLength, exactWordString ) ).result != RESULT_OK )
						return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to check the plural noun ending" );

					hasFoundWordEnding = wordEndingResult.hasFoundWordEnding;

					// First word can be a singular noun (without article). Example: "Water ...".
					isUncountableGeneralizationNoun = ( isFirstWord &&
														isUndefinedWord );

					if( hasFoundProperNoun ||
					hasFoundWordEnding ||
					wasPreviousWordAdjective ||
					wasPreviousWordArticle ||
					wasPreviousWordCreatedSingularNoun ||
					wasPreviousWordPreposition ||

					// Typical for Chinese
					( isChineseCurrentLanguage && 

					( isUncountableGeneralizationNoun ||
					wasPreviousWordPossessiveDeterminer ||
					wasPreviousWordChineseImperativeVerbDisplay ) ) )
						{
						if( hasFoundWordEnding )
							{
							strcpy( singularNounWordString, wordEndingResult.wordString );

							if( !hasFoundLowerCase )
								singularNounWordString[0] = (char)tolower( singularNounWordString[0] );

							if( ( wordResult = findWordTypeInAllWords( false, WORD_TYPE_NOUN_SINGULAR, singularNounWordString, NULL ) ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find if a singular noun word already exists" );

							if( ( foundWordItem = wordResult.foundWordItem ) == NULL )
								{
								if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ) ).result != RESULT_OK )
									return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word" );

								if( ( pluralNounWordItem = wordResult.createdWordItem ) == NULL )
									return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The created word item is undefined" );

								if( pluralNounWordItem->addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, WORD_TYPE_NOUN_SINGULAR, wordEndingResult.wordStringLength, singularNounWordString ).result != RESULT_OK )
									return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word type item for plural noun word \"", pluralNounWordItem->anyWordTypeString(), "\"" );

								if( wordStringLength == wordEndingResult.wordStringLength &&
								strcmp( exactWordString, singularNounWordString ) == 0 )
									singularNounWordItem = pluralNounWordItem;
								}
							else
								{
								// Found singular noun
								if( !isExactPluralNoun )
									{
									if( foundWordItem->addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ).result != RESULT_OK )
										return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add a plural noun word type item for word \"", foundWordItem->anyWordTypeString(), "\"" );

									pluralNounWordItem = foundWordItem;
									}
								}
							}
						else
							{
							if( !isExactSingularNoun )
								{
								if( wasPreviousWordFrenchNumeral )
									{
									// Typical for French: Singular and plural noun 'fils' are the same
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ) ).result != RESULT_OK )
										return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word" );

									pluralNounWordItem = wordResult.createdWordItem;
									}
								else
									{
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, WORD_TYPE_NOUN_SINGULAR, wordStringLength, exactWordString ) ).result != RESULT_OK )
										return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word" );

									isCreatedSingularNoun = true;
									singularNounWordItem = wordResult.createdWordItem;
									}
								}
							}

						if( singularNounWordItem != NULL &&
						// Singular noun
						( createReadWordResult = createReadWord( isUncountableGeneralizationNoun, currentWordOrderNr, WORD_TYPE_NOUN_SINGULAR, 0, NULL, singularNounWordItem ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create a singular noun read word" );

						if( pluralNounWordItem != NULL &&
						// Plural noun
						( createReadWordResult = createReadWord( false, currentWordOrderNr, WORD_TYPE_NOUN_PLURAL, 0, NULL, pluralNounWordItem ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create a plural noun read word" );
						}

					// Step 7: Create an adjective
					if( isUndefinedWord &&
					!wasPreviousWordUndefined &&
					pluralNounWordItem == NULL &&

					( isFirstWord ||
					isSelection ||
					wasPreviousWordBasicVerb ||
					wasPreviousWordConjunction ||
					wasPreviousWordExactNoun ||
					wasPreviousWordSymbol ) )
						{
						if( ( wordResult = addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_ADJECTIVE, wordStringLength, lowercaseWordString ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to add an adjective word" );

						if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
							return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The last created adjective word is undefined" );

						if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, WORD_TYPE_ADJECTIVE, 0, NULL, createdWordItem ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create an adjective read word" );

						isAdjective = true;
						}
					}

				currentPosition += readWordResult.offset;
				}

			// Don't mark Spanish reversed question mark as first word
			if( !isSymbol )
				isFirstWord = false;
			}
		while( createReadWordResult.hasCreatedReadWord &&
		currentPosition < readUserSentenceStringLength );

		// Has created all read words
		boolResult.booleanValue = createReadWordResult.hasCreatedReadWord;
		return boolResult;
		}

	ReadWordResultType readWordFromString( bool isCheckingForGrammarDefinition, bool isMergedWord, bool isSkippingTextString, size_t minimumStringLength, char *wordString )
		{
		bool isText = false;
		char currentChar;
		size_t index = 0;
		size_t wordStringLength;
		ReadWordResultType readWordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readWordFromString";

		if( wordString == NULL )
			return adminItem_->startReadWordResultError( functionNameString, moduleNameString_, "The given word string is undefined" );

		if( ( wordStringLength = strlen( wordString ) ) <= 0 )
			return adminItem_->startReadWordResultError( functionNameString, moduleNameString_, "The given word string is empty" );

		currentChar = wordString[0];

		if( isSymbol( currentChar ) &&

		( !isCheckingForGrammarDefinition ||
		!isGrammarDefinitionSymbol( currentChar ) ) )
			{
			index++;
			readWordResult.wordLength++;
			}
		else
			{
			while( index < wordStringLength &&

			( isText ||
			readWordResult.wordLength < minimumStringLength ||

			( ( !isSymbol( currentChar ) ||

			( isCheckingForGrammarDefinition &&
			isGrammarDefinitionSymbol( currentChar ) ) ) &&

			( !isspace( currentChar ) ||

			// Typical for French: Include spaces in grammar merged word definition
			( isMergedWord &&
			currentChar == SPACE_CHAR ) ) ) ) )
				{
				if( isCheckingForGrammarDefinition &&
				isGrammarDefinitionSymbol( currentChar ) )
					readWordResult.hasFoundGrammarDefinition = true;

				if( currentChar == SYMBOL_DOUBLE_QUOTE &&

				( index == 0 ||
				// Skip escaped double quote character
				wordString[index - 1] != SYMBOL_BACK_SLASH ) )
					isText = !isText;

				readWordResult.wordLength++;

				if( ++index < wordStringLength )
					currentChar = wordString[index];
				}

			if( isSkippingTextString &&
			readWordResult.wordLength > 1 )
				readWordResult.wordLength--;

			if( isSkippingTextString &&
			index > 1 &&
			readWordResult.wordLength > 1 &&
			wordString[index - 1] == QUERY_STRING_END_CHAR )
				{
				readWordResult.wordLength--;
				readWordResult.startWordPosition++;
				}
			}

		// Skip spaces
		while( index < wordStringLength &&
		isspace( wordString[index] ) )
			index++;

		readWordResult.offset = index;
		return readWordResult;
		}

	WordResultType addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordTypeStringLength, char *wordTypeString )
		{
		bool isProperNounPrecededByDefiniteArticle;
		bool wasPreviousWordAdjective = ( previousWordAdjectiveParameter > NO_ADJECTIVE_PARAMETER );
		bool wasPreviousWordDefiniteArticle = ( previousWordDefiniteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER );
		bool wasPreviousWordIndefiniteArticle = ( previousWordIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER );
		unsigned short adjectiveParameter = NO_ADJECTIVE_PARAMETER;
		unsigned short definiteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
		unsigned short indefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem *createdWordItem;
		WordResultType wordResult;
		WordTypeResultType wordTypeResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addWord";

		if( wordTypeString == NULL )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given word type string is undefined" );

		if( wordTypeStringLength <= 0 ||
		strlen( wordTypeString ) == 0 )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given word type string is empty or has no words left anymore" );

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given word type number is undefined or out of bounds" );

		if( ( wordResult = findWordTypeInAllWords( false, wordTypeNr, wordTypeString, NULL ) ).result != RESULT_OK )
			return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to find a word type in all words" );

		if( wordResult.foundWordItem == NULL ||
		// Skip match with word part of multiple word
		wordResult.foundWordItem->wordParameter() != wordParameter )
			{
			if( ( wordResult = adminItem_->createWordItem( isLanguageWord, wordParameter ) ).result != RESULT_OK )
				return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to create a word item" );

			if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
				return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The last created word item is undefined" );

			if( globalVariables_->firstWordItem == NULL )
				// Remember the first word
				globalVariables_->firstWordItem = createdWordItem;

			if( wordParameter > NO_WORD_PARAMETER )
				{
				// Remember the first predefined word
				if( globalVariables_->firstPredefinedWordItem == NULL )
					globalVariables_->firstPredefinedWordItem = createdWordItem;

				// Remember the last predefined word
				globalVariables_->lastPredefinedWordItem = createdWordItem;
				}

			if( adminItem_->isAdjectiveParameter( wordParameter ) )
				adjectiveParameter = wordParameter;
			else
				{
				if( wasPreviousWordAdjective )
					adjectiveParameter = previousWordAdjectiveParameter;
				}

			if( adminItem_->isDefiniteArticleParameter( wordParameter ) )
				definiteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordDefiniteArticle )
					definiteArticleParameter = previousWordDefiniteArticleParameter;
				}

			if( adminItem_->isIndefiniteArticleParameter( wordParameter ) )
				indefiniteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordIndefiniteArticle )
					indefiniteArticleParameter = previousWordIndefiniteArticleParameter;
				}

			isProperNounPrecededByDefiniteArticle = ( wasPreviousWordDefiniteArticle &&
													  wordTypeNr == WORD_TYPE_PROPER_NOUN );

			if( ( wordTypeResult = createdWordItem->addWordType( isMultipleWord, isProperNounPrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordTypeStringLength, wordTypeString ) ).result != RESULT_OK )
				return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to add a word type to a new word" );

			if( indefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
			wordTypeResult.wordTypeItem != NULL &&
			adminItem_->isNounWordType( wordTypeNr ) &&
			!wordTypeResult.wordTypeItem->isCorrectIndefiniteArticle( false, indefiniteArticleParameter ) &&

			// Write 'different indefinite article used' notification
			inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, wordTypeString, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
				return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to write the 'different indefinite article used' interface notification" );
			}

		wordResult.foundWordItem = wordResult.foundWordItem;
		wordResult.foundWordTypeItem = wordResult.foundWordTypeItem;
		return wordResult;
		}

	WordResultType findWordTypeInAllWords( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString, WordItem *previousWordItem )
		{
		WordItem *currentWordItem;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findWordTypeInAllWords";

		if( ( currentWordItem = ( previousWordItem == NULL ? globalVariables_->firstWordItem : previousWordItem->nextWordItem() ) ) != NULL )
			{
			do	{
				if( ( wordResult = currentWordItem->findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString ) ).result != RESULT_OK )
					return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to find a word type in word \"", currentWordItem->anyWordTypeString(), "\"" );

				if( wordResult.foundWordItem != NULL )
					return wordResult;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return wordResult;
		}
	};

/*************************************************************************
 *	"How joyful are those who fear the Lord-
 *	all who follow his ways!" (Psalm 128:1)
 *************************************************************************/
