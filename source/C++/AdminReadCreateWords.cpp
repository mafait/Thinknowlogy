/*	Class:			AdminReadCreateWords
 *	Supports class:	AdminItem
 *	Purpose:		To create words of the read sentence
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

#include "AdminItem.h"
#include "ReadList.cpp"
#include "WordList.cpp"
#include "WordTypeItem.cpp"

class AdminReadCreateWords
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasCreatedReadWord_;
	bool hasFoundDifferentParameter_;

	unsigned short currentWordOrderNr_;
	unsigned short lastCreatedWordOrderNr_;
	unsigned short wordTypeNr_;

	size_t nextTextStringPosition_;

	char exactWordString_[MAX_WORD_LENGTH];
	char lowerCaseWordString_[MAX_WORD_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


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
#ifdef _MSC_VER
				character == SYMBOL_EXCLAMATION_MARK_INVERTED ||
#endif
				character == SYMBOL_QUESTION_MARK ||
#ifdef _MSC_VER
				character == SYMBOL_QUESTION_MARK_INVERTED ||
#endif
				character == SYMBOL_PLUS ||
				character == SYMBOL_EQUALITY ||
				character == SYMBOL_PIPE ||
				character == SYMBOL_AMPERSAND ||
				character == SYMBOL_ASTERISK ||
				character == SYMBOL_PERCENT ||
				character == SYMBOL_DOLLAR ||
				character == SYMBOL_SLASH ||
				character == SYMBOL_BACK_SLASH ||
				// Don't add SYMBOL_QUOTE to avoid analyzing compound words
				// Don't add SYMBOL_DOUBLE_QUOTE to avoid analyzing text strings
				character == SYMBOL_OPEN_ROUNDED_BRACKET ||
				character == SYMBOL_CLOSE_ROUNDED_BRACKET ||
				character == SYMBOL_OPEN_CURVED_BRACKET ||
				character == SYMBOL_CLOSE_CURVED_BRACKET ||
				character == SYMBOL_OPEN_HOOKED_BRACKET ||
				character == SYMBOL_CLOSE_HOOKED_BRACKET ||
				character == SYMBOL_OPEN_SQUARE_BRACKET ||
				character == SYMBOL_CLOSE_SQUARE_BRACKET );
		}

	size_t skipSpaces( size_t wordPosition, char *wordString )
		{
		size_t wordStringLength;

		if( wordString != NULL &&
		( wordStringLength = strlen( wordString ) ) > 0 )
			{
			while( wordPosition < wordStringLength &&
			isspace( wordString[wordPosition] ) )
				wordPosition++;
			}

		return wordPosition;
		}

	ReadResultType createReadWord( unsigned short wordOrderNr, unsigned short wordTypeNr, size_t textStringStartPosition, char *textString, WordItem *readWordItem )
		{
		ReadResultType readResult;
		unsigned short wordParameter = ( wordTypeNr == WORD_TYPE_NOUN_PLURAL ||
										readWordItem == NULL ? NO_WORD_PARAMETER : readWordItem->wordParameter() );
		char *readString = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWord";

		hasCreatedReadWord_ = false;
		nextTextStringPosition_ = 0;

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given word type number is undefined or out of bounds" );

		if( textString == NULL &&
		readWordItem == NULL )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "Both the given text string and the given read word item are undefined" );

		if( textString != NULL )
			{
			if( ( readResult = readWordFromString( false, false, true, textStringStartPosition, 0, textString ) ).result != RESULT_OK )
				return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to read a word from a text string" );

			if( readResult.startWordPosition >= strlen( textString ) )
				return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The found start word position is invalid" );

			nextTextStringPosition_ = readResult.nextWordPosition;
			readString = &textString[readResult.startWordPosition];
			}

		if( adminItem_->readList == NULL )
			{
			// Create list
			if( ( adminItem_->readList = new ReadList( commonVariables_, adminItem_ ) ) == NULL )
				return adminItem_->startReadResultError( functionNameString, moduleNameString_, "I failed to create a read list" );

			adminItem_->adminListArray[ADMIN_READ_LIST] = adminItem_->readList;
			}
		else
			{
			// Find out if already exists
			if( adminItem_->readList->hasFoundReadItem( wordOrderNr, wordParameter, wordTypeNr, readString, readWordItem ) )
				return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given read item already exists" );
			}

		if( ( readResult = adminItem_->readList->createReadItem( wordOrderNr, wordParameter, wordTypeNr, readResult.wordLength, readString, readWordItem ) ).result != RESULT_OK )
			return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create an admin read words item" );

		hasCreatedReadWord_ = true;
		lastCreatedWordOrderNr_ = wordOrderNr;

		return readResult;
		}

	ReadResultType createWordStrings( size_t startPosition, char *wordString )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordStrings";
		strcpy( exactWordString_, EMPTY_STRING );
		strcpy( lowerCaseWordString_, EMPTY_STRING );

		if( wordString == NULL )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given word string is undefined" );

		if( ( readResult = readWordFromString( false, false, false, startPosition, 0, wordString ) ).result != RESULT_OK )
			return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to read a word from the word string" );

		if( readResult.wordLength <= 0 )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given word string is empty or has no words left anymore" );

		if( readResult.wordLength >= MAX_WORD_LENGTH )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given word string is too long" );

		strcpy( exactWordString_, EMPTY_STRING );
		strncat( exactWordString_, &wordString[startPosition], readResult.wordLength );

		strcpy( lowerCaseWordString_, exactWordString_ );

		for( unsigned i = 0; i < readResult.wordLength; i++ )
			lowerCaseWordString_[i] = (char)tolower( exactWordString_[i] );

		return readResult;
		}

	ResultType getWordTypeNr( bool isCheckingProperName, size_t wordTypeStringLength, char *wordTypeString )
		{
		size_t wordPosition = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getWordTypeNr";

		wordTypeNr_ = NO_WORD_TYPE_NR;

		if( wordTypeString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given word type string is undefined" );

		if( wordTypeStringLength <= 0 )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given word type string is empty" );

		if( isalpha( wordTypeString[wordPosition] ) )
			{
			if( wordTypeStringLength == 1 )
				wordTypeNr_ = ( isupper( wordTypeString[wordPosition] ) ? WORD_TYPE_LETTER_CAPITAL : WORD_TYPE_LETTER_SMALL );
			else
				{
				if( isCheckingProperName &&
				isupper( wordTypeString[wordPosition] ) )
					wordTypeNr_ = WORD_TYPE_PROPER_NAME;
				}
			}
		else
			{
			while( wordPosition < wordTypeStringLength &&
			isdigit( wordTypeString[wordPosition] ) )
				wordPosition++;

			if( wordPosition == wordTypeStringLength )
				wordTypeNr_ = WORD_TYPE_NUMERAL;
			}

		return RESULT_OK;
		}


	protected:
	// Constructor

	AdminReadCreateWords( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasCreatedReadWord_ = false;
		hasFoundDifferentParameter_ = false;

		currentWordOrderNr_ = NO_ORDER_NR;
		lastCreatedWordOrderNr_ = NO_ORDER_NR;
		wordTypeNr_ = NO_WORD_TYPE_NR;

		nextTextStringPosition_ = 0;

		strcpy( exactWordString_, EMPTY_STRING );
		strcpy( lowerCaseWordString_, EMPTY_STRING );

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminReadCreateWords" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemError( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	bool hasFoundDifferentParameter()
		{
		return hasFoundDifferentParameter_;
		}

	unsigned short lastCreatedWordOrderNr()
		{
		return lastCreatedWordOrderNr_;
		}

	ReadResultType createReadWords( char *readUserSentenceString )
		{
		GrammarResultType grammarResult;
		ReadResultType readResult;
		WordResultType wordResult;
		bool hasFoundSingularNoun;
		bool isAdverb;
		bool isCapitalSingularNoun;
		bool isCapitalVerb;
		bool isExactWord;
		bool isFirstFind;
		bool isPartOfMultipleWord;
		bool wasPreviousWordAdjective;
		bool wasPreviousWordArticle;
		bool wasPreviousWordBasicVerb;
		bool wasPreviousWordConjunction;
		bool wasPreviousWordExactNoun;
		bool wasPreviousWordNumeral;
		bool wasPreviousWordPossessiveDeterminer;
		bool wasPreviousWordPreposition;
		bool wasPreviousWordProperName;
		bool wasPreviousWordSymbol;
		bool wasPreviousWordUndefined;
		bool isAdjective = false;
		bool isArticle = false;
		bool isBasicVerb = false;
		bool isConjunction = false;
		bool isExactPluralNoun = false;
		bool isExactSingularNoun = false;
		bool isNumeral = false;
		bool isPossessiveDeterminer = false;
		bool isPossessivePronoun = false;
		bool isPreposition = false;
		bool isProperName = false;
		bool isSymbol = false;
		bool isUndefinedWord = false;
		bool isFirstWord = true;
		unsigned short foundWordTypeNr;
		unsigned short previousWordAdjectiveParameter;
		unsigned short previousWordDefiniteArticleParameter;
		unsigned short previousWordIndefiniteArticleParameter;
		unsigned short currentWordAdjectiveParameter = NO_ADJECTIVE_PARAMETER;
		unsigned short currentWordDefiniteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
		unsigned short currentWordIndefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;
		unsigned short currentLanguageNr = commonVariables_->currentLanguageNr;
		size_t readUserSentenceStringLength;
		size_t wordStringLength;
		size_t nextWordPosition = 0;
		size_t readPosition = 0;
		WordItem *createdWordItem;
		WordItem *currentLanguageWordItem;
		WordItem *foundWordItem;
		WordItem *pluralNounWordItem;
		WordItem *singularNounWordItem;
		WordTypeItem *foundWordTypeItem;
		ReadList *readList;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWords";

		// Set to pass while loop for the first time
		hasCreatedReadWord_ = true;
		hasFoundDifferentParameter_ = false;
		currentWordOrderNr_ = NO_ORDER_NR;

		if( readUserSentenceString == NULL )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given read user sentence string is undefined" );

		if( ( readUserSentenceStringLength = strlen( readUserSentenceString ) ) == 0 )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given read user sentence string is empty" );

		if( ( currentLanguageWordItem = commonVariables_->currentLanguageWordItem ) == NULL )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The current language word item is undefined" );

		do	{
			if( ++currentWordOrderNr_ >= MAX_ORDER_NR )
				return adminItem_->startReadResultSystemError( functionNameString, moduleNameString_, "Word order number overflow! I can't accept more words" );

			hasCreatedReadWord_ = false;

			if( readUserSentenceString[readPosition] == QUERY_STRING_START_CHAR )
				{
				if( createReadWord( currentWordOrderNr_, WORD_TYPE_TEXT, readPosition, readUserSentenceString, NULL ).result != RESULT_OK )
					return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create a read word" );

				nextWordPosition = nextTextStringPosition_;
				}
			else
				{
				wasPreviousWordAdjective = isAdjective;
				wasPreviousWordArticle = isArticle;
				wasPreviousWordBasicVerb = isBasicVerb;
				wasPreviousWordConjunction = isConjunction;
				wasPreviousWordExactNoun = ( isExactSingularNoun || isExactPluralNoun );
				wasPreviousWordNumeral = isNumeral;
				wasPreviousWordPossessiveDeterminer = isPossessiveDeterminer;
				wasPreviousWordPreposition = isPreposition;
				wasPreviousWordProperName = isProperName;
				wasPreviousWordSymbol = isSymbol;
				wasPreviousWordUndefined = isUndefinedWord;

				previousWordAdjectiveParameter = currentWordAdjectiveParameter;
				previousWordDefiniteArticleParameter = currentWordDefiniteArticleParameter;
				previousWordIndefiniteArticleParameter = currentWordIndefiniteArticleParameter;

				isAdjective = false;
				isAdverb = false;
				isArticle = false;
				isBasicVerb = false;
				isCapitalVerb = false;
				isCapitalSingularNoun = false;
				isConjunction = false;
				isExactPluralNoun = false;
				isExactSingularNoun = false;
				isExactWord = false;
				isNumeral = false;
				isPossessiveDeterminer = false;
				isPossessivePronoun = false;
				isPreposition = false;
				isProperName = false;
				isSymbol = false;
				isUndefinedWord = false;

				currentWordAdjectiveParameter = NO_ADJECTIVE_PARAMETER;
				currentWordDefiniteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
				currentWordIndefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;

				createdWordItem = NULL;
				pluralNounWordItem = NULL;
				singularNounWordItem = NULL;

				if( ( readResult = createWordStrings( readPosition, readUserSentenceString ) ).result != RESULT_OK )
					return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create the word strings" );

				isPartOfMultipleWord = true;

				nextWordPosition = readResult.nextWordPosition;
				wordStringLength = readResult.wordLength;
				foundWordItem = NULL;

				// Step 1: Find exact word types in all words
				do	{
					if( ( wordResult = findWordTypeInAllWords( false, NO_WORD_TYPE_NR, exactWordString_, foundWordItem ) ).result != RESULT_OK )
						return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to find an exact word type in all words" );

					// Found exact word
					if( ( foundWordItem = wordResult.foundWordItem ) != NULL &&
					( foundWordTypeItem = wordResult.foundWordTypeItem ) != NULL )
						{
						// Typically for the Spanish language: Skip answer, when there was an adverb too
						if( !isAdverb ||
						!foundWordTypeItem->isAnswer() )
							{
							hasFoundSingularNoun = foundWordTypeItem->isSingularNoun();

							if( createReadWord( currentWordOrderNr_, ( hasFoundSingularNoun && wasPreviousWordNumeral ? WORD_TYPE_NOUN_PLURAL : foundWordTypeItem->wordTypeNr() ), 0, NULL, foundWordItem ).result != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create another read word" );

							isExactWord = true;

							if( foundWordTypeItem->isAdjective() )
								{
								isAdjective = true;

								if( wordResult.foundWordItem != NULL &&
								// Adjective 'no' can be used as article
								wordResult.foundWordItem->isAdjectiveNo() )
									isArticle = true;

								if( !wasPreviousWordArticle )
									currentWordAdjectiveParameter = foundWordTypeItem->adjectiveParameter();
								}

							if( foundWordTypeItem->isAdverb() )
								isAdverb = true;

							if( foundWordTypeItem->isArticle() )
								{
								isArticle = true;

								if( foundWordTypeItem->isDefiniteArticle() )
									currentWordDefiniteArticleParameter = foundWordTypeItem->definiteArticleParameter();

								if( foundWordTypeItem->isIndefiniteArticle() )
									currentWordIndefiniteArticleParameter = foundWordTypeItem->indefiniteArticleParameter();
								}

							if( foundWordItem->isBasicVerb() )
								isBasicVerb = true;

							if( foundWordTypeItem->isConjunction() )
								isConjunction = true;

							if( foundWordTypeItem->isVerb() &&
							!foundWordItem->isMultipleWord() )
								isPartOfMultipleWord = false;

							if( foundWordTypeItem->isNumeral() )
								isNumeral = true;

							if( hasFoundSingularNoun )
								{
								isExactSingularNoun = true;

								if( wasPreviousWordNumeral )
									{
									// Typically for French: Skip singular/plural noun mismatch of word 'fils'
									if( foundWordItem->addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ).result != RESULT_OK )
										return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add a plural noun word type item for word \"", foundWordItem->anyWordTypeString(), "\"" );
									}
								else
									{
									if( !foundWordItem->isMultipleWord() )
										{
										isPartOfMultipleWord = false;

										if( previousWordAdjectiveParameter > NO_ADJECTIVE_PARAMETER ||
										previousWordDefiniteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER ||
										previousWordIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER )
											{
											if( ( wordResult = foundWordTypeItem->setParametersOfSingularNoun( previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter ) ).result != RESULT_OK )
												return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to set the definite article parameter of a singular noun" );

											if( wordResult.hasFoundDifferentParameter )
												hasFoundDifferentParameter_ = true;
											}
										}
									}
								}

							if( foundWordTypeItem->isPluralNoun() )
								isExactPluralNoun = true;

							if( foundWordTypeItem->isPossessiveDeterminer() )
								isPossessiveDeterminer = true;

							if( foundWordTypeItem->isPossessivePronoun() )
								isPossessivePronoun = true;

							if( foundWordTypeItem->isPreposition() )
								isPreposition = true;

							if( foundWordTypeItem->isSymbol() )
								isSymbol = true;
							}
						}
					}
				// Allow multiple finds
				while( foundWordItem != NULL );

				if( !isPartOfMultipleWord &&
				!wasPreviousWordExactNoun &&
				( readList = adminItem_->readList ) != NULL )
					{
					if( readList->deleteReadItemsWithNonMatchingMultipleWordPart( currentWordOrderNr_, &readUserSentenceString[readPosition] ) != RESULT_OK )
						return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to delete the read items with a non-matching multiple word part" );
					}

				if( isFirstWord &&
				isupper( readUserSentenceString[readPosition] ) )
					{
					if( getWordTypeNr( false, wordStringLength, lowerCaseWordString_ ) != RESULT_OK )
						return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to get the word type number of a lowercase word" );

					if( wordTypeNr_ == NO_WORD_TYPE_NR )
						isUndefinedWord = true;

					// Step 2: Find word type with lowercase first letter in all words
					do	{
						if( ( wordResult = findWordTypeInAllWords( true, NO_WORD_TYPE_NR, lowerCaseWordString_, foundWordItem ) ).result != RESULT_OK )
							return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to find a word type with difference case of the first letter in all words" );

						// Found word type with lowercase first letter
						if( ( foundWordItem = wordResult.foundWordItem ) != NULL &&
						( foundWordTypeItem = wordResult.foundWordTypeItem ) != NULL )
							{
							foundWordTypeNr = foundWordTypeItem->wordTypeNr();

							if( foundWordTypeNr == wordTypeNr_ &&
							foundWordTypeItem->wordTypeLanguageNr() != currentLanguageNr )
								{
								// Create same word type for different language
								if( foundWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr_, wordStringLength, lowerCaseWordString_ ).result != RESULT_OK )
									return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add a word type with lowercase first letter" );
								}

							if( createReadWord( currentWordOrderNr_, foundWordTypeNr, 0, NULL, foundWordItem ).result != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create a read word with a word type with difference case of the first letter" );

							if( foundWordTypeItem->isAdjective() )
								{
								isAdjective = true;

								if( wordResult.foundWordItem != NULL &&
								// Adjective 'no' can be used as article
								wordResult.foundWordItem->isAdjectiveNo() )
									isArticle = true;

								if( !wasPreviousWordArticle )
									currentWordAdjectiveParameter = foundWordTypeItem->adjectiveParameter();
								}

							if( foundWordTypeItem->isArticle() )
								{
								isArticle = true;

								if( foundWordTypeItem->isDefiniteArticle() )
									currentWordDefiniteArticleParameter = foundWordTypeItem->definiteArticleParameter();

								if( foundWordTypeItem->isIndefiniteArticle() )
									currentWordIndefiniteArticleParameter = foundWordTypeItem->indefiniteArticleParameter();
								}

							if( foundWordTypeItem->isVerb() )
								isCapitalVerb = true;

							if( foundWordTypeItem->isSingularNoun() )
								isCapitalSingularNoun = true;

							if( foundWordTypeItem->isPossessiveDeterminer() )
								isPossessiveDeterminer = true;

							if( foundWordTypeItem->isPossessivePronoun() )
								isPossessivePronoun = true;
							}
						}
					// Allow multiple finds
					while( foundWordItem != NULL );

					if( !isUndefinedWord &&
					wordStringLength == 1 )
						{
						// Step 3: Typically for English: Find or create lowercase letter 'a' as first letter of a sentence.
						if( ( wordResult = findWordTypeInAllWords( false, wordTypeNr_, lowerCaseWordString_, NULL ) ).result != RESULT_OK )
							return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to find a lowercase letter" );

						if( wordResult.foundWordItem == NULL )
							{
							if( ( wordResult = addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, wordTypeNr_, wordStringLength, lowerCaseWordString_ ) ).result != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add word with lowercase letter" );

							if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
								return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The created word with lowercase letter is undefined" );

							if( createReadWord( currentWordOrderNr_, wordTypeNr_, 0, NULL, createdWordItem ).result != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create a read word with lowercase letter" );
							}
						}
					}

				// Proper name
				if( ( ( !isExactWord &&

				// Skip 'Undo' and 'Redo'
				( !isCapitalVerb ||
				wordStringLength == 1 ) ) ||

				// Small letters, capital letters and numerals
				( wasPreviousWordSymbol &&
				wordStringLength == 1 &&

				// Typically for the Spanish language: 'o' and 'y' are letters as well as conjunctions
				( isConjunction ||
				// Typically for Dutch: 'u' is a letter as well as a possessive pronoun
				isPossessivePronoun ) ) ) )
					{
					isFirstFind = true;

					// Step 4: Find exact word types in all words
					do	{
						if( ( wordResult = findWordTypeInAllWords( true, NO_WORD_TYPE_NR, exactWordString_, foundWordItem ) ).result != RESULT_OK )
							return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to find an exact word" );

						createdWordItem = NULL;
						foundWordItem = wordResult.foundWordItem;
						foundWordTypeItem = wordResult.foundWordTypeItem;

						if( isFirstFind ||

						( foundWordItem != NULL &&
						// Skip if later runs have no result
						foundWordTypeItem != NULL ) )
							{
							if( getWordTypeNr( true, wordStringLength, exactWordString_ ) != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to get the word type number of an exact word" );

							if( wordTypeNr_ == NO_WORD_TYPE_NR )
								isUndefinedWord = true;
							else
								{
								if( foundWordItem == NULL )
									{
									// Small letters, capital letters, numerals and proper names
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, wordTypeNr_, wordStringLength, exactWordString_ ) ).result != RESULT_OK )
										return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add an exact word" );

									if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
										return adminItem_->startReadResultError( functionNameString, moduleNameString_, "I couldn't create an exact word" );
									}
								else
									{
									if( foundWordTypeItem != NULL &&
									foundWordTypeItem->wordTypeNr() == wordTypeNr_ &&
									foundWordTypeItem->wordTypeLanguageNr() != currentLanguageNr )
										{
										// Create same word type for different language
										if( foundWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr_, wordStringLength, exactWordString_ ).result != RESULT_OK )
											return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add an exact word type" );

										createdWordItem = foundWordItem;
										}
									}

								if( createdWordItem != NULL )
									{
									if( ( readResult = createReadWord( currentWordOrderNr_, wordTypeNr_, 0, NULL, createdWordItem ) ).result != RESULT_OK )
										return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create an exact read word" );

									isFirstFind = false;

									if( wordTypeNr_ == WORD_TYPE_PROPER_NAME )
										isProperName = true;
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
				!isCapitalVerb &&
				!isConjunction &&
				!isNumeral &&
				!isPossessiveDeterminer &&
				!isPossessivePronoun &&
				!isSymbol &&
				wordStringLength > 2 &&

				( isExactSingularNoun ||
				isUndefinedWord ||
				wasPreviousWordArticle ) &&

				( isFirstWord ||
				!isupper( readUserSentenceString[readPosition] ) ) )
					{
					if( ( grammarResult = currentLanguageWordItem->analyzeWordEnding( WORD_PLURAL_NOUN_ENDING, wordStringLength, exactWordString_ ) ).result != RESULT_OK )
						return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to check the plural noun ending" );

					if( wasPreviousWordAdjective ||
					wasPreviousWordArticle ||
					wasPreviousWordBasicVerb ||
					wasPreviousWordConjunction ||
					wasPreviousWordNumeral ||
					wasPreviousWordPossessiveDeterminer ||
					wasPreviousWordPreposition ||
					wasPreviousWordProperName ||
					wasPreviousWordSymbol ||
					grammarResult.hasFoundWordEnding )
						{
						if( grammarResult.hasFoundWordEnding )
							{
							if( ( wordResult = findWordTypeInAllWords( false, WORD_TYPE_NOUN_SINGULAR, grammarResult.singularNounWordString, NULL ) ).result != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to find if a singular noun word already exists" );

							if( ( foundWordItem = wordResult.foundWordItem ) == NULL )
								{
								if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ) ).result != RESULT_OK )
									return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word" );

								if( ( pluralNounWordItem = wordResult.createdWordItem ) == NULL )
									return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The created word item is undefined" );

								if( pluralNounWordItem->addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, WORD_TYPE_NOUN_SINGULAR, grammarResult.singularNounWordStringLength, grammarResult.singularNounWordString ).result != RESULT_OK )
									return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word type item for plural noun word \"", pluralNounWordItem->anyWordTypeString(), "\"" );

								if( wordStringLength == grammarResult.singularNounWordStringLength &&
								strcmp( exactWordString_, grammarResult.singularNounWordString ) == 0 )
									singularNounWordItem = pluralNounWordItem;
								}
							else
								{
								// Found singular noun
								if( !isExactPluralNoun )
									{
									if( foundWordItem->addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ).result != RESULT_OK )
										return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add a plural noun word type item for word \"", foundWordItem->anyWordTypeString(), "\"" );

									pluralNounWordItem = foundWordItem;
									}
								}
							}
						else
							{
							if( !isCapitalSingularNoun &&
							!isExactSingularNoun )
								{
								// Typically for French: Singular and plural noun 'fils' are the same
								if( wasPreviousWordNumeral )
									{
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ) ).result != RESULT_OK )
										return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word" );

									pluralNounWordItem = wordResult.createdWordItem;
									}
								else
									{
									if( !isExactWord ||
									previousWordIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER )
										{
										if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, WORD_TYPE_NOUN_SINGULAR, wordStringLength, exactWordString_ ) ).result != RESULT_OK )
											return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add a singular noun word" );

										singularNounWordItem = wordResult.createdWordItem;
										}
									}
								}
							}

						if( singularNounWordItem != NULL )
							{
							// Singular noun
							if( createReadWord( currentWordOrderNr_, WORD_TYPE_NOUN_SINGULAR, 0, NULL, singularNounWordItem ).result != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create a singular noun read word" );
							}

						if( pluralNounWordItem != NULL &&

						( grammarResult.hasFoundWordEnding ||
						wasPreviousWordNumeral ) )
							{
							// Plural noun
							if( createReadWord( currentWordOrderNr_, WORD_TYPE_NOUN_PLURAL, 0, NULL, pluralNounWordItem ).result != RESULT_OK )
								return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create a plural noun read word" );
							}
						}

					// Step 7: Create an adjective
					if( !isExactWord &&
					!wasPreviousWordUndefined &&

					( wasPreviousWordConjunction ||
					wasPreviousWordSymbol ||
					pluralNounWordItem == NULL ) )
						{
						if( ( wordResult = addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_ADJECTIVE, wordStringLength, lowerCaseWordString_ ) ).result != RESULT_OK )
							return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to add an adjective word" );

						if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
							return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The last created adjective word is undefined" );

						if( createReadWord( currentWordOrderNr_, WORD_TYPE_ADJECTIVE, 0, NULL, createdWordItem ).result != RESULT_OK )
							return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create an adjective read word" );

						isAdjective = true;
						}
					}
				}

			// Don't mark Spanish reversed question mark as first word
			if( !isSymbol )
				isFirstWord = false;

			readPosition = nextWordPosition;
			}
		while( hasCreatedReadWord_ &&
		readPosition < readUserSentenceStringLength );

		// The read sentence isn't ended by a colon.
		// So, add the missing colon.
		if( !isSymbol )
			{
			if( createReadWord( ++currentWordOrderNr_, WORD_TYPE_SYMBOL, 0, NULL, adminItem_->predefinedWordItem( WORD_PARAMETER_SYMBOL_COLON ) ).result != RESULT_OK )
				return adminItem_->addReadResultError( functionNameString, moduleNameString_, "I failed to create a read word for a missing colon" );
			}

		readResult.hasCreatedAllReadWords = hasCreatedReadWord_;
		return readResult;
		}

	ReadResultType readWordFromString( bool isCheckingForGrammarDefinition, bool isMergedWord, bool isSkippingTextString, size_t startWordPosition, size_t minimumStringLength, char *wordString )
		{
		ReadResultType readResult;
		bool isText = false;
		bool isWordStartingWithDoubleQuote = false;
		size_t wordStringLength;
		size_t wordPosition = startWordPosition;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readWordFromString";

		if( wordString == NULL )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given word string is undefined" );

		wordStringLength = strlen( wordString );

		if( wordPosition >= wordStringLength )
			return adminItem_->startReadResultError( functionNameString, moduleNameString_, "The given start word position is invalid" );

		if( ( wordPosition = skipSpaces( wordPosition, wordString ) ) < wordStringLength )
			{
			readResult.startWordPosition = wordPosition;

			if( isSymbol( wordString[wordPosition] ) &&

			( !isCheckingForGrammarDefinition ||
			!isGrammarDefinitionSymbol( wordString[wordPosition] ) ) )
				{
				wordPosition++;
				readResult.wordLength++;
				}
			else
				{
				if( isSkippingTextString &&
				wordString[wordPosition] == SYMBOL_DOUBLE_QUOTE )
					isWordStartingWithDoubleQuote = true;

				while( wordPosition < wordStringLength &&

				( isText ||
				readResult.wordLength < minimumStringLength ||

				( ( !isspace( wordString[wordPosition] ) ||

				( isMergedWord &&
				// Typically for French: Include spaces in grammar compound word definition
				wordString[wordPosition] == SPACE_CHAR ) ) &&

				( !isSymbol( wordString[wordPosition] ) ||

				( isCheckingForGrammarDefinition &&
				isGrammarDefinitionSymbol( wordString[wordPosition] ) ) ) ) ) )
					{
					if( isCheckingForGrammarDefinition &&
					isGrammarDefinitionSymbol( wordString[wordPosition] ) )
						readResult.hasFoundGrammarDefinition = true;

					if( wordString[wordPosition] == SYMBOL_DOUBLE_QUOTE &&

					( wordPosition == 0 ||
					// Skip escaped double quote character
					wordString[wordPosition - 1] != SYMBOL_BACK_SLASH ) )
						isText = !isText;

					wordPosition++;
					readResult.wordLength++;
					}

				if( isWordStartingWithDoubleQuote &&
				readResult.wordLength > 1 )
					readResult.wordLength--;

				if( isSkippingTextString &&
				wordPosition > 1 &&
				readResult.wordLength > 1 &&
				wordString[wordPosition - 1] == QUERY_STRING_END_CHAR )
					{
					readResult.wordLength--;
					readResult.startWordPosition++;
					}
				}

			wordPosition = skipSpaces( wordPosition, wordString );
			}

		readResult.nextWordPosition = wordPosition;
		return readResult;
		}

	WordResultType addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordTypeStringLength, char *wordTypeString )
		{
		WordResultType wordResult;
		WordResultType wordTypeResult;
		bool isProperName;
		bool isProperNamePrecededByDefiniteArticle;
		bool isNounWordType;
		bool wasPreviousWordAdjective = ( previousWordAdjectiveParameter > NO_ADJECTIVE_PARAMETER );
		bool wasPreviousWordDefiniteArticle = ( previousWordDefiniteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER );
		bool wasPreviousWordIndefiniteArticle = ( previousWordIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER );
		unsigned short adjectiveParameter = NO_ADJECTIVE_PARAMETER;
		unsigned short definiteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
		unsigned short indefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem *createdWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWord";

		if( wordTypeString == NULL )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given word type string is undefined" );

		if( wordTypeStringLength <= 0 ||
		strlen( wordTypeString ) == 0 )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given word type string is empty or has no words left anymore" );

		if( wordTypeNr <= NO_WORD_TYPE_NR ||
		wordTypeNr >= NUMBER_OF_WORD_TYPES )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given word type number is undefined or out of bounds" );

		if( adminItem_->wordList == NULL )
			{
			// Create list
			if( ( adminItem_->wordList = new WordList( commonVariables_, adminItem_ ) ) == NULL )
				return adminItem_->startWordResultError( functionNameString, moduleNameString_, "I failed to create a word list" );

			adminItem_->adminListArray[ADMIN_WORD_LIST] = adminItem_->wordList;
			}
		else
			{
			if( ( wordResult = findWordTypeInAllWords( false, wordTypeNr, wordTypeString, NULL ) ).result != RESULT_OK )
				return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to find a word type in all words" );
			}

		if( wordResult.foundWordItem == NULL ||
		// Skip match with word part of multiple word
		wordResult.foundWordItem->wordParameter() != wordParameter )
			{
			if( ( wordResult = adminItem_->wordList->createWordItem( isLanguageWord, wordParameter ) ).result != RESULT_OK )
				return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to create a word item" );

			if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
				return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The last created word item is undefined" );

			if( commonVariables_->firstWordItem == NULL )
				// Remember the first word
				commonVariables_->firstWordItem = createdWordItem;

			if( wordParameter > NO_WORD_PARAMETER )
				{
				// Remember the first predefined word
				if( commonVariables_->firstPredefinedWordItem == NULL )
					commonVariables_->firstPredefinedWordItem = createdWordItem;

				// Remember the last predefined word
				commonVariables_->lastPredefinedWordItem = createdWordItem;
				}

			isProperName = ( wordTypeNr == WORD_TYPE_PROPER_NAME );

			isProperNamePrecededByDefiniteArticle = ( isProperName &&
													wasPreviousWordDefiniteArticle );

			isNounWordType = adminItem_->isNounWordType( wordTypeNr );

			if( adminItem_->isAdjectiveParameter( wordParameter ) )
				adjectiveParameter = wordParameter;
			else
				{
				if( wasPreviousWordAdjective &&
				isNounWordType )
					adjectiveParameter = previousWordAdjectiveParameter;
				}

			if( adminItem_->isDefiniteArticleParameter( wordParameter ) )
				definiteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordDefiniteArticle &&

				( isProperName ||
				isNounWordType ) )
					definiteArticleParameter = previousWordDefiniteArticleParameter;
				}

			if( adminItem_->isIndefiniteArticleParameter( wordParameter ) )
				indefiniteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordIndefiniteArticle &&
				isNounWordType )
					indefiniteArticleParameter = previousWordIndefiniteArticleParameter;
				}

			if( ( wordTypeResult = createdWordItem->addWordType( isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordTypeStringLength, wordTypeString ) ).result != RESULT_OK )
				return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to add a word type to a new word" );

			if( isNounWordType &&
			indefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
			wordTypeResult.createdWordTypeItem != NULL &&
			!wordTypeResult.createdWordTypeItem->isCorrectIndefiniteArticle( false, indefiniteArticleParameter ) )
				{
				if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, wordTypeString, INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != RESULT_OK )
					return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to write the 'different indefinite article used' interface notification" );

				hasFoundDifferentParameter_ = true;
				}
			}

		return wordResult;
		}

	WordResultType findWordTypeInAllWords( bool isCheckingAllLanguages, unsigned short wordTypeNr, char *wordTypeString, WordItem *previousWordItem )
		{
		WordResultType wordResult;
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findWordTypeInAllWords";

		if( ( currentWordItem = ( previousWordItem == NULL ? commonVariables_->firstWordItem : previousWordItem->nextWordItem() ) ) != NULL )
			{
			do	{
				if( ( wordResult = currentWordItem->findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString ) ).result != RESULT_OK )
					return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to find a word type in word \"", currentWordItem->anyWordTypeString(), "\"" );
				}
			while( wordResult.foundWordItem == NULL &&
			( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return wordResult;
		}
	};

/*************************************************************************
 *	"How joyful are those who fear the Lord-
 *	all who follow his ways!" (Psalm 128:1)
 *************************************************************************/
