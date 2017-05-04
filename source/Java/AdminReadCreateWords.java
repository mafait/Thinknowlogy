/*	Class:			AdminReadCreateWords
 *	Supports class:	AdminItem
 *	Purpose:		To create words of the read sentence
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
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

class AdminReadCreateWords
	{
	// Private constructed variables

	private boolean hasFoundDifferentParameter_;

	private short lastCreatedWordOrderNr_;


	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


	// Private methods

	private static boolean isGrammarDefinitionSymbol( char character )
		{
		return ( character == Constants.QUERY_WORD_TYPE_CHAR ||
				character == Constants.QUERY_PARAMETER_CHAR ||
				character == Constants.GRAMMAR_WORD_DEFINITION_CHAR );
		}

	private static boolean isSymbol( char character )
		{
		return ( character == Constants.SYMBOL_COMMA ||
				character == Constants.SYMBOL_COLON ||
				character == Constants.SYMBOL_SEMI_COLON ||
				character == Constants.SYMBOL_DOUBLE_COLON ||
				character == Constants.SYMBOL_EXCLAMATION_MARK ||
				character == Constants.SYMBOL_EXCLAMATION_MARK_INVERTED ||
				character == Constants.SYMBOL_QUESTION_MARK ||
				character == Constants.SYMBOL_QUESTION_MARK_INVERTED ||
				character == Constants.SYMBOL_PLUS ||
				character == Constants.SYMBOL_EQUALITY ||
				character == Constants.SYMBOL_PIPE ||
				character == Constants.SYMBOL_AMPERSAND ||
				character == Constants.SYMBOL_ASTERISK ||
				character == Constants.SYMBOL_PERCENT ||
				character == Constants.SYMBOL_DOLLAR ||
				character == Constants.SYMBOL_SLASH ||
				character == Constants.SYMBOL_BACK_SLASH ||
				// Don't add Constants.SYMBOL_QUOTE to avoid analyzing compound words
				// Don't add Constants.SYMBOL_DOUBLE_QUOTE to avoid analyzing text strings
				character == Constants.SYMBOL_OPEN_ROUNDED_BRACKET ||
				character == Constants.SYMBOL_CLOSE_ROUNDED_BRACKET ||
				character == Constants.SYMBOL_OPEN_CURVED_BRACKET ||
				character == Constants.SYMBOL_CLOSE_CURVED_BRACKET ||
				character == Constants.SYMBOL_OPEN_HOOKED_BRACKET ||
				character == Constants.SYMBOL_CLOSE_HOOKED_BRACKET ||
				character == Constants.SYMBOL_OPEN_SQUARE_BRACKET ||
				character == Constants.SYMBOL_CLOSE_SQUARE_BRACKET );
		}

	private static int skipSpaces( int wordPosition, String wordString )
		{
		int wordStringLength;

		if( wordString != null &&
		( wordStringLength = wordString.length() ) > 0 )
			{
			while( wordPosition < wordStringLength &&
			Character.isWhitespace( wordString.charAt( wordPosition ) ) )
				wordPosition++;
			}

		return wordPosition;
		}

	private CreateReadWordResultType createReadWord( short wordOrderNr, short wordTypeNr, int textStringStartPosition, String textString, WordItem readWordItem )
		{
		CreateReadWordResultType createReadWordResult = new CreateReadWordResultType();
		ReadWordResultType readWordResult = new ReadWordResultType();
		short wordParameter = ( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||
										readWordItem == null ? Constants.NO_WORD_PARAMETER : readWordItem.wordParameter() );
		String readString = null;

		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return adminItem_.startCreateReadWordResultError( 1, moduleNameString_, "The given word type number is undefined or out of bounds" );

		if( textString == null &&
		readWordItem == null )
			return adminItem_.startCreateReadWordResultError( 1, moduleNameString_, "Both the given text string and the given read word item are undefined" );

		if( textString != null )
			{
			if( ( readWordResult = readWordFromString( false, false, true, textStringStartPosition, 0, textString ) ).result != Constants.RESULT_OK )
				return adminItem_.addCreateReadWordResultError( 1, moduleNameString_, "I failed to read a word from a text string" );

			if( readWordResult.startWordPosition >= textString.length() )
				return adminItem_.startCreateReadWordResultError( 1, moduleNameString_, "The found start word position is invalid" );

			createReadWordResult.nextTextStringPosition = readWordResult.nextWordPosition;
			readString = textString.substring( readWordResult.startWordPosition );
			}

		if( adminItem_.createReadItem( wordOrderNr, wordParameter, wordTypeNr, readWordResult.wordLength, readString, readWordItem ) != Constants.RESULT_OK )
			return adminItem_.addCreateReadWordResultError( 1, moduleNameString_, "I failed to create a read word item" );

		lastCreatedWordOrderNr_ = wordOrderNr;

		createReadWordResult.hasCreatedReadWord = true;
		return createReadWordResult;
		}

	private ShortResultType getWordTypeNr( boolean isCheckingProperName, int wordTypeStringLength, String wordTypeString )
		{
		int wordPosition = 0;
		ShortResultType shortResult = new ShortResultType();

		if( wordTypeString == null )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given word type string is undefined" );

		if( wordTypeStringLength <= 0 )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given word type string is empty" );

		if( Character.isLetter( wordTypeString.charAt( wordPosition ) ) )
			{
			if( wordTypeStringLength == 1 )
				shortResult.shortValue = ( Character.isUpperCase( wordTypeString.charAt( wordPosition ) ) ? Constants.WORD_TYPE_LETTER_CAPITAL : Constants.WORD_TYPE_LETTER_SMALL );
			else
				{
				if( isCheckingProperName &&
				Character.isUpperCase( wordTypeString.charAt( wordPosition ) ) )
					shortResult.shortValue = Constants.WORD_TYPE_PROPER_NAME;
				}
			}
		else
			{
			while( wordPosition < wordTypeStringLength &&
			Character.isDigit( wordTypeString.charAt( wordPosition ) ) )
				wordPosition++;

			if( wordPosition == wordTypeStringLength )
				shortResult.shortValue = Constants.WORD_TYPE_NUMERAL;
			}

		return shortResult;
		}


	// Constructor

	protected AdminReadCreateWords( AdminItem adminItem )
		{
		// Private constructed variables

		hasFoundDifferentParameter_ = false;

		lastCreatedWordOrderNr_ = Constants.NO_ORDER_NR;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected boolean hasFoundDifferentParameter()
		{
		return hasFoundDifferentParameter_;
		}

	protected short lastCreatedWordOrderNr()
		{
		return lastCreatedWordOrderNr_;
		}

	protected BoolResultType createReadWords( String readUserSentenceString )
		{
		boolean hasFoundSingularNoun;
		boolean isAdverb;
		boolean isCapitalSingularNoun;
		boolean isCapitalVerb;
		boolean isExactWord;
		boolean isFirstFind;
		boolean isPartOfMultipleWord;
		boolean wasPreviousWordAdjective;
		boolean wasPreviousWordArticle;
		boolean wasPreviousWordBasicVerb;
		boolean wasPreviousWordConjunction;
		boolean wasPreviousWordExactNoun;
		boolean wasPreviousWordNumeral;
		boolean wasPreviousWordPossessiveDeterminer;
		boolean wasPreviousWordPreposition;
		boolean wasPreviousWordProperName;
		boolean wasPreviousWordSymbol;
		boolean wasPreviousWordUndefined;
		boolean isAdjective = false;
		boolean isArticle = false;
		boolean isBasicVerb = false;
		boolean isConjunction = false;
		boolean isExactPluralNoun = false;
		boolean isExactSingularNoun = false;
		boolean isNumeral = false;
		boolean isPossessiveDeterminer = false;
		boolean isPossessivePronoun = false;
		boolean isPreposition = false;
		boolean isProperName = false;
		boolean isSymbol = false;
		boolean isUndefinedWord = false;
		boolean isFirstWord = true;
		short foundWordTypeNr;
		short previousWordAdjectiveParameter;
		short previousWordDefiniteArticleParameter;
		short previousWordIndefiniteArticleParameter;
		short currentWordAdjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
		short currentWordDefiniteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		short currentWordIndefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		short currentWordOrderNr = Constants.NO_ORDER_NR;
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		short wordTypeNr = Constants.NO_WORD_TYPE_NR;
		int readUserSentenceStringLength;
		int wordStringLength;
		int nextWordPosition = 0;
		int readPosition = 0;
		WordItem createdWordItem;
		WordItem currentLanguageWordItem;
		WordItem foundWordItem;
		WordItem pluralNounWordItem;
		WordItem singularNounWordItem;
		WordTypeItem foundWordTypeItem;
		String exactWordString = null;
		String lowercaseWordString = null;
		BoolResultType boolResult = new BoolResultType();
		CreateReadWordResultType createReadWordResult = new CreateReadWordResultType();
		ReadWordResultType readWordResult;
		ShortResultType shortResult;
		WordEndingResultType wordEndingResult;
		WordResultType wordResult;

		// Set to pass while loop for the first time
		hasFoundDifferentParameter_ = false;

		if( readUserSentenceString == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, null, "The given read user sentence string is undefined" );

		if( ( readUserSentenceStringLength = readUserSentenceString.length() ) == 0 )
			return adminItem_.startBoolResultError( 1, moduleNameString_, null, "The given read user sentence string is empty" );

		if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, null, "The current language word item is undefined" );

		do	{
			if( ++currentWordOrderNr >= Constants.MAX_ORDER_NR )
				return adminItem_.startBoolResultSystemError( 1, moduleNameString_, "Word order number overflow! I can't accept more words" );

			if( readUserSentenceString.charAt( readPosition ) == Constants.QUERY_STRING_START_CHAR )
				{
				if( ( createReadWordResult = createReadWord( currentWordOrderNr, Constants.WORD_TYPE_TEXT, readPosition, readUserSentenceString, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a read word" );

				nextWordPosition = createReadWordResult.nextTextStringPosition;
				}
			else
				{
				wasPreviousWordAdjective = isAdjective;
				wasPreviousWordArticle = isArticle;
				wasPreviousWordBasicVerb = isBasicVerb;
				wasPreviousWordConjunction = isConjunction;
				wasPreviousWordExactNoun = ( isExactSingularNoun ||
											isExactPluralNoun );
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
				isPartOfMultipleWord = true;
				isPossessiveDeterminer = false;
				isPossessivePronoun = false;
				isPreposition = false;
				isProperName = false;
				isSymbol = false;
				isUndefinedWord = false;

				currentWordAdjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
				currentWordDefiniteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
				currentWordIndefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;

				createdWordItem = null;
				foundWordItem = null;
				pluralNounWordItem = null;
				singularNounWordItem = null;

				if( ( readWordResult = readWordFromString( false, false, false, readPosition, 0, readUserSentenceString ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to read a word from the word string" );

				nextWordPosition = readWordResult.nextWordPosition;
				wordStringLength = readWordResult.wordLength;

				// Create exact word string
				exactWordString = readUserSentenceString.substring( readPosition, ( readPosition + readWordResult.wordLength ) );

				// Create lowercase word string
				lowercaseWordString = readUserSentenceString.substring( readPosition, ( readPosition + readWordResult.wordLength ) ).toLowerCase();

				// Step 1: Find exact word types in all words
				do	{
					if( ( wordResult = findWordTypeInAllWords( false, Constants.NO_WORD_TYPE_NR, exactWordString, foundWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find an exact word type in all words" );

					// Found exact word
					if( ( foundWordItem = wordResult.foundWordItem ) != null &&
					( foundWordTypeItem = wordResult.foundWordTypeItem ) != null &&

					// Typically for the Spanish language: Skip answer, when there was an adverb too
					( !isAdverb ||
					!foundWordTypeItem.isAnswer() ) )
						{
						hasFoundSingularNoun = foundWordTypeItem.isSingularNoun();

						if( ( createReadWordResult = createReadWord( currentWordOrderNr, ( hasFoundSingularNoun && wasPreviousWordNumeral ? Constants.WORD_TYPE_NOUN_PLURAL : foundWordTypeItem.wordTypeNr() ), 0, null, foundWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create another read word" );

						isExactWord = true;

						if( foundWordTypeItem.isAdjective() )
							{
							isAdjective = true;

							if( wordResult.foundWordItem != null &&
							// Adjective 'no' can be used as article
							wordResult.foundWordItem.isAdjectiveNo() )
								isArticle = true;

							if( !wasPreviousWordArticle )
								currentWordAdjectiveParameter = foundWordTypeItem.adjectiveParameter();
							}

						if( foundWordTypeItem.isAdverb() )
							isAdverb = true;

						if( foundWordTypeItem.isArticle() )
							{
							isArticle = true;

							if( foundWordTypeItem.isDefiniteArticle() )
								currentWordDefiniteArticleParameter = foundWordTypeItem.definiteArticleParameter();

							if( foundWordTypeItem.isIndefiniteArticle() )
								currentWordIndefiniteArticleParameter = foundWordTypeItem.indefiniteArticleParameter();
							}

						if( foundWordItem.isBasicVerb() )
							isBasicVerb = true;

						if( foundWordTypeItem.isConjunction() )
							isConjunction = true;

						if( foundWordTypeItem.isNumeral() )
							isNumeral = true;

						if( foundWordTypeItem.isVerb() )
							// Multiple word ends at reaching a verb
							isPartOfMultipleWord = false;

						if( hasFoundSingularNoun )
							{
							isExactSingularNoun = true;

							if( wasPreviousWordNumeral )
								{
								// Typically for French: Skip singular/plural noun mismatch of word 'fils'
								if( foundWordItem.addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ).result != Constants.RESULT_OK )
									return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a plural noun word type item for word \"" + foundWordItem.anyWordTypeString() + "\"" );
								}
							else
								{
								if( foundWordItem.isMultipleWord() )
									{
									if( wasPreviousWordBasicVerb )
										// Not the first part of a multiple word, but an adjective
										isExactWord = false;
									}
								else
									{
									isPartOfMultipleWord = false;

									if( previousWordAdjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER ||
									previousWordDefiniteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER ||
									previousWordIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
										{
										if( ( boolResult = foundWordTypeItem.setParametersOfSingularNoun( previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter ) ).result != Constants.RESULT_OK )
											return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to set the definite article parameter of a singular noun" );

										if( boolResult.booleanValue )
											hasFoundDifferentParameter_ = true;
										}
									}
								}
							}

						if( foundWordTypeItem.isPluralNoun() )
							isExactPluralNoun = true;

						if( foundWordTypeItem.isPossessiveDeterminer() )
							isPossessiveDeterminer = true;

						if( foundWordTypeItem.isPossessivePronoun() )
							isPossessivePronoun = true;

						if( foundWordTypeItem.isPreposition() )
							isPreposition = true;

						if( foundWordTypeItem.isSymbol() )
							isSymbol = true;
						}
					}
				// Allow multiple finds
				while( foundWordItem != null );

				if( !isBasicVerb &&
				!isPartOfMultipleWord &&
				!wasPreviousWordExactNoun &&
				adminItem_.deleteReadItemsWithNonMatchingMultipleWordPart( currentWordOrderNr, readUserSentenceString.substring( readPosition ) ) != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to delete the read items with a non-matching multiple word part" );

				if( isFirstWord &&
				Character.isUpperCase( readUserSentenceString.charAt( readPosition ) ) )
					{
					if( ( shortResult = getWordTypeNr( false, wordStringLength, lowercaseWordString ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to get the word type number of a lowercase word" );

					if( ( wordTypeNr = shortResult.shortValue ) == Constants.NO_WORD_TYPE_NR )
						isUndefinedWord = true;

					// Step 2: Find word type with lowercase first letter in all words
					do	{
						if( ( wordResult = findWordTypeInAllWords( true, Constants.NO_WORD_TYPE_NR, lowercaseWordString, foundWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a word type with difference case of the first letter in all words" );

						// Found word type with lowercase first letter
						if( ( foundWordItem = wordResult.foundWordItem ) != null &&
						( foundWordTypeItem = wordResult.foundWordTypeItem ) != null )
							{
							foundWordTypeNr = foundWordTypeItem.wordTypeNr();

							if( foundWordTypeNr == wordTypeNr &&
							foundWordTypeItem.wordTypeLanguageNr() != currentLanguageNr &&
							// Create same word type for different language
							foundWordItem.addWordType( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, wordStringLength, lowercaseWordString ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a word type with lowercase first letter" );

							if( ( createReadWordResult = createReadWord( currentWordOrderNr, foundWordTypeNr, 0, null, foundWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a read word with a word type with difference case of the first letter" );

							if( foundWordTypeItem.isAdjective() )
								{
								isAdjective = true;

								if( wordResult.foundWordItem != null &&
								// Adjective 'no' can be used as article
								wordResult.foundWordItem.isAdjectiveNo() )
									isArticle = true;

								if( !wasPreviousWordArticle )
									currentWordAdjectiveParameter = foundWordTypeItem.adjectiveParameter();
								}

							if( foundWordTypeItem.isArticle() )
								{
								isArticle = true;

								if( foundWordTypeItem.isDefiniteArticle() )
									currentWordDefiniteArticleParameter = foundWordTypeItem.definiteArticleParameter();

								if( foundWordTypeItem.isIndefiniteArticle() )
									currentWordIndefiniteArticleParameter = foundWordTypeItem.indefiniteArticleParameter();
								}

							if( foundWordTypeItem.isVerb() )
								isCapitalVerb = true;

							if( foundWordTypeItem.isSingularNoun() )
								isCapitalSingularNoun = true;

							if( foundWordTypeItem.isPossessiveDeterminer() )
								isPossessiveDeterminer = true;

							if( foundWordTypeItem.isPossessivePronoun() )
								isPossessivePronoun = true;
							}
						}
					// Allow multiple finds
					while( foundWordItem != null );

					if( !isUndefinedWord &&
					wordStringLength == 1 )
						{
						// Step 3: Typically for English: Find or create lowercase letter 'a' as first letter of a sentence.
						if( ( wordResult = findWordTypeInAllWords( false, wordTypeNr, lowercaseWordString, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a lowercase letter" );

						if( wordResult.foundWordItem == null )
							{
							if( ( wordResult = addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, wordTypeNr, wordStringLength, lowercaseWordString ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add word with lowercase letter" );

							if( ( createdWordItem = wordResult.createdWordItem ) == null )
								return adminItem_.startBoolResultError( 1, moduleNameString_, null, "The created word with lowercase letter is undefined" );

							if( ( createReadWordResult = createReadWord( currentWordOrderNr, wordTypeNr, 0, null, createdWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a read word with lowercase letter" );
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
						if( ( wordResult = findWordTypeInAllWords( true, Constants.NO_WORD_TYPE_NR, exactWordString, foundWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find an exact word" );

						createdWordItem = null;
						foundWordItem = wordResult.foundWordItem;
						foundWordTypeItem = wordResult.foundWordTypeItem;

						if( isFirstFind ||

						( foundWordItem != null &&
						// Skip if later runs have no result
						foundWordTypeItem != null ) )
							{
							if( ( shortResult = getWordTypeNr( true, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to get the word type number of an exact word" );

							if( ( wordTypeNr = shortResult.shortValue ) == Constants.NO_WORD_TYPE_NR )
								isUndefinedWord = true;
							else
								{
								if( foundWordItem == null )
									{
									// Small letters, capital letters, numerals and proper names
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, wordTypeNr, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add an exact word" );

									if( ( createdWordItem = wordResult.createdWordItem ) == null )
										return adminItem_.startBoolResultError( 1, moduleNameString_, null, "I couldn't create an exact word" );
									}
								else
									{
									if( foundWordTypeItem != null &&
									foundWordTypeItem.wordTypeNr() == wordTypeNr &&
									foundWordTypeItem.wordTypeLanguageNr() != currentLanguageNr )
										{
										// Create same word type for different language
										if( foundWordItem.addWordType( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, wordStringLength, exactWordString ).result != Constants.RESULT_OK )
											return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add an exact word type" );

										createdWordItem = foundWordItem;
										}
									}

								if( createdWordItem != null )
									{
									if( ( createReadWordResult = createReadWord( currentWordOrderNr, wordTypeNr, 0, null, createdWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create an exact read word" );

									isFirstFind = false;

									if( wordTypeNr == Constants.WORD_TYPE_PROPER_NAME )
										isProperName = true;
									}
								}
							}
						}
					// Allow multiple finds
					while( foundWordItem != null );
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
				!Character.isUpperCase( readUserSentenceString.charAt( readPosition ) ) ) )
					{
					if( ( wordEndingResult = currentLanguageWordItem.analyzeWordEnding( Constants.WORD_PLURAL_NOUN_ENDING, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to check the plural noun ending" );

					if( wasPreviousWordAdjective ||
					wasPreviousWordArticle ||
					wasPreviousWordBasicVerb ||
					wasPreviousWordConjunction ||
					wasPreviousWordNumeral ||
					wasPreviousWordPossessiveDeterminer ||
					wasPreviousWordPreposition ||
					wasPreviousWordProperName ||
					wasPreviousWordSymbol ||
					wordEndingResult.hasFoundWordEnding )
						{
						if( wordEndingResult.hasFoundWordEnding )
							{
							if( ( wordResult = findWordTypeInAllWords( false, Constants.WORD_TYPE_NOUN_SINGULAR, wordEndingResult.singularNounWordString, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find if a singular noun word already exists" );

							if( ( foundWordItem = wordResult.foundWordItem ) == null )
								{
								if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
									return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word" );

								if( ( pluralNounWordItem = wordResult.createdWordItem ) == null )
									return adminItem_.startBoolResultError( 1, moduleNameString_, null, "The created word item is undefined" );

								if( pluralNounWordItem.addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.WORD_TYPE_NOUN_SINGULAR, wordEndingResult.singularNounWordStringLength, wordEndingResult.singularNounWordString ).result != Constants.RESULT_OK )
									return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word type item for plural noun word \"" + pluralNounWordItem.anyWordTypeString() + "\"" );

								if( wordStringLength == wordEndingResult.singularNounWordStringLength &&
								exactWordString.equals( wordEndingResult.singularNounWordString ) )
									singularNounWordItem = pluralNounWordItem;
								}
							else
								{
								// Found singular noun
								if( !isExactPluralNoun )
									{
									if( foundWordItem.addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a plural noun word type item for word \"" + foundWordItem.anyWordTypeString() + "\"" );

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
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word" );

									pluralNounWordItem = wordResult.createdWordItem;
									}
								else
									{
									if( !isExactWord ||
									previousWordIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER )
										{
										if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
											return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word" );

										singularNounWordItem = wordResult.createdWordItem;
										}
									}
								}
							}

						if( singularNounWordItem != null &&
						// Singular noun
						( createReadWordResult = createReadWord( currentWordOrderNr, Constants.WORD_TYPE_NOUN_SINGULAR, 0, null, singularNounWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a singular noun read word" );

						if( pluralNounWordItem != null &&

						( wordEndingResult.hasFoundWordEnding ||
						wasPreviousWordNumeral ) )
							{
							// Plural noun
							if( ( createReadWordResult = createReadWord( currentWordOrderNr, Constants.WORD_TYPE_NOUN_PLURAL, 0, null, pluralNounWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a plural noun read word" );
							}
						}

					// Step 7: Create an adjective
					if( !isExactWord &&
					!wasPreviousWordUndefined &&

					( wasPreviousWordConjunction ||
					wasPreviousWordSymbol ||
					pluralNounWordItem == null ) )
						{
						if( ( wordResult = addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_ADJECTIVE, wordStringLength, lowercaseWordString ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add an adjective word" );

						if( ( createdWordItem = wordResult.createdWordItem ) == null )
							return adminItem_.startBoolResultError( 1, moduleNameString_, null, "The last created adjective word is undefined" );

						if( ( createReadWordResult = createReadWord( currentWordOrderNr, Constants.WORD_TYPE_ADJECTIVE, 0, null, createdWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create an adjective read word" );

						isAdjective = true;
						}
					}
				}

			// Don't mark Spanish reversed question mark as first word
			if( !isSymbol )
				isFirstWord = false;

			readPosition = nextWordPosition;
			}
		while( createReadWordResult.hasCreatedReadWord &&
		readPosition < readUserSentenceStringLength );

		// The read sentence isn't ended by a colon.
		// So, add the missing colon.
		if( !isSymbol &&
		createReadWord( ++currentWordOrderNr, Constants.WORD_TYPE_SYMBOL, 0, null, adminItem_.predefinedWordItem( Constants.WORD_PARAMETER_SYMBOL_COLON ) ).result != Constants.RESULT_OK )
			return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a read word for a missing colon" );

		// Has created all read words
		boolResult.booleanValue = createReadWordResult.hasCreatedReadWord;
		return boolResult;
		}

	protected ReadWordResultType readWordFromString( boolean isCheckingForGrammarDefinition, boolean isMergedWord, boolean isSkippingTextString, int startWordPosition, int minimumStringLength, String wordString )
		{
		ReadWordResultType readWordResult = new ReadWordResultType();
		boolean isText = false;
		boolean isWordStartingWithDoubleQuote = false;
		int wordStringLength;
		int wordPosition = startWordPosition;

		if( wordString == null )
			return adminItem_.startReadWordResultError( 1, moduleNameString_, "The given word string is undefined" );

		wordStringLength = wordString.length();

		if( wordPosition >= wordStringLength )
			return adminItem_.startReadWordResultError( 1, moduleNameString_, "The given start word position is invalid" );

		if( ( wordPosition = skipSpaces( wordPosition, wordString ) ) < wordStringLength )
			{
			readWordResult.startWordPosition = wordPosition;

			if( isSymbol( wordString.charAt( wordPosition ) ) &&

			( !isCheckingForGrammarDefinition ||
			!isGrammarDefinitionSymbol( wordString.charAt( wordPosition ) ) ) )
				{
				wordPosition++;
				readWordResult.wordLength++;
				}
			else
				{
				if( isSkippingTextString &&
				wordString.charAt( wordPosition ) == Constants.SYMBOL_DOUBLE_QUOTE )
					isWordStartingWithDoubleQuote = true;

				while( wordPosition < wordStringLength &&

				( isText ||
				readWordResult.wordLength < minimumStringLength ||

				( ( !Character.isWhitespace( wordString.charAt( wordPosition ) ) ||

				( isMergedWord &&
				// Typically for French: Include spaces in grammar compound word definition
				wordString.charAt( wordPosition ) == Constants.SPACE_CHAR ) ) &&

				( !isSymbol( wordString.charAt( wordPosition ) ) ||

				( isCheckingForGrammarDefinition &&
				isGrammarDefinitionSymbol( wordString.charAt( wordPosition ) ) ) ) ) ) )
					{
					if( isCheckingForGrammarDefinition &&
					isGrammarDefinitionSymbol( wordString.charAt( wordPosition ) ) )
						readWordResult.hasFoundGrammarDefinition = true;

					if( wordString.charAt( wordPosition ) == Constants.SYMBOL_DOUBLE_QUOTE &&

					( wordPosition == 0 ||
					// Skip escaped double quote character
					wordString.charAt( wordPosition - 1 ) != Constants.SYMBOL_BACK_SLASH ) )
						isText = !isText;

					wordPosition++;
					readWordResult.wordLength++;
					}

				if( isWordStartingWithDoubleQuote &&
				readWordResult.wordLength > 1 )
					readWordResult.wordLength--;

				if( isSkippingTextString &&
				wordPosition > 1 &&
				readWordResult.wordLength > 1 &&
				wordString.charAt( wordPosition - 1 ) == Constants.QUERY_STRING_END_CHAR )
					{
					readWordResult.wordLength--;
					readWordResult.startWordPosition++;
					}
				}

			wordPosition = skipSpaces( wordPosition, wordString );
			}

		readWordResult.nextWordPosition = wordPosition;
		return readWordResult;
		}

	protected WordResultType addWord( boolean isLanguageWord, boolean isMultipleWord, short previousWordAdjectiveParameter, short previousWordDefiniteArticleParameter, short previousWordIndefiniteArticleParameter, short wordParameter, short wordTypeNr, int wordTypeStringLength, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();
		WordTypeResultType wordTypeResult = new WordTypeResultType();
		boolean isProperName;
		boolean isProperNamePrecededByDefiniteArticle;
		boolean isNounWordType;
		boolean wasPreviousWordAdjective = ( previousWordAdjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER );
		boolean wasPreviousWordDefiniteArticle = ( previousWordDefiniteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER );
		boolean wasPreviousWordIndefiniteArticle = ( previousWordIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER );
		short adjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
		short definiteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		short indefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem createdWordItem;

		if( wordTypeString == null )
			return adminItem_.startWordResultError( 1, moduleNameString_, "The given word type string is undefined" );

		if( wordTypeStringLength <= 0 ||
		wordTypeString.length() == 0 )
			return adminItem_.startWordResultError( 1, moduleNameString_, "The given word type string is empty or has no words left anymore" );

		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return adminItem_.startWordResultError( 1, moduleNameString_, "The given word type number is undefined or out of bounds" );

		if( ( wordResult = findWordTypeInAllWords( false, wordTypeNr, wordTypeString, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to find a word type in all words" );

		if( wordResult.foundWordItem == null ||
		// Skip match with word part of multiple word
		wordResult.foundWordItem.wordParameter() != wordParameter )
			{
			if( ( wordResult = adminItem_.createWordItem( isLanguageWord, wordParameter ) ).result != Constants.RESULT_OK )
				return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to create a word item" );

			if( ( createdWordItem = wordResult.createdWordItem ) == null )
				return adminItem_.startWordResultError( 1, moduleNameString_, "The last created word item is undefined" );

			if( CommonVariables.firstWordItem == null )
				// Remember the first word
				CommonVariables.firstWordItem = createdWordItem;

			if( wordParameter > Constants.NO_WORD_PARAMETER )
				{
				// Remember the first predefined word
				if( CommonVariables.firstPredefinedWordItem == null )
					CommonVariables.firstPredefinedWordItem = createdWordItem;

				// Remember the last predefined word
				CommonVariables.lastPredefinedWordItem = createdWordItem;
				}

			isProperName = ( wordTypeNr == Constants.WORD_TYPE_PROPER_NAME );

			isProperNamePrecededByDefiniteArticle = ( isProperName &&
													wasPreviousWordDefiniteArticle );

			isNounWordType = adminItem_.isNounWordType( wordTypeNr );

			if( adminItem_.isAdjectiveParameter( wordParameter ) )
				adjectiveParameter = wordParameter;
			else
				{
				if( wasPreviousWordAdjective &&
				isNounWordType )
					adjectiveParameter = previousWordAdjectiveParameter;
				}

			if( adminItem_.isDefiniteArticleParameter( wordParameter ) )
				definiteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordDefiniteArticle &&

				( isProperName ||
				isNounWordType ) )
					definiteArticleParameter = previousWordDefiniteArticleParameter;
				}

			if( adminItem_.isIndefiniteArticleParameter( wordParameter ) )
				indefiniteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordIndefiniteArticle &&
				isNounWordType )
					indefiniteArticleParameter = previousWordIndefiniteArticleParameter;
				}

			if( ( wordTypeResult = createdWordItem.addWordType( isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordTypeStringLength, wordTypeString ) ).result != Constants.RESULT_OK )
				return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to add a word type to a new word" );

			if( isNounWordType &&
			indefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
			wordTypeResult.wordTypeItem != null &&
			!wordTypeResult.wordTypeItem.isCorrectIndefiniteArticle( false, indefiniteArticleParameter ) )
				{
				if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, wordTypeString, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
					return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to write the 'different indefinite article used' interface notification" );

				hasFoundDifferentParameter_ = true;
				}
			}

		wordResult.foundWordItem = wordResult.foundWordItem;
		wordResult.foundWordTypeItem = wordResult.foundWordTypeItem;
		return wordResult;
		}

	protected WordResultType findWordTypeInAllWords( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString, WordItem previousWordItem )
		{
		WordResultType wordResult = new WordResultType();
		WordItem currentWordItem;

		if( ( currentWordItem = ( previousWordItem == null ? CommonVariables.firstWordItem : previousWordItem.nextWordItem() ) ) != null )
			{
			do	{
				if( ( wordResult = currentWordItem.findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString ) ).result != Constants.RESULT_OK )
					return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to find a word type in word \"" + currentWordItem.anyWordTypeString() + "\"" );
				}
			while( wordResult.foundWordItem == null &&
			( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return wordResult;
		}
	};

/*************************************************************************
 *	"How joyful are those who fear the Lord-
 *	all who follow his ways!" (Psalm 128:1)
 *************************************************************************/
