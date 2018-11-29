/*	Class:			AdminReadCreateWords
 *	Supports class:	AdminItem
 *	Purpose:		To create words of the read sentence
 *	Version:		Thinknowlogy 2018r4 (New Science)
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

package org.mafait.thinknowlogy;

class AdminReadCreateWords
	{
	// Private constructed variables

	private short lastCreatedWordOrderNr_ = Constants.NO_ORDER_NR;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private AdminItem adminItem_ = null;


	// Private methods

	private static boolean isGrammarDefinitionSymbol( char character )
		{
		return ( character == Constants.QUERY_WORD_TYPE_CHAR ||
				character == Constants.QUERY_PARAMETER_CHAR ||
				character == Constants.GRAMMAR_WORD_DEFINITION_CHAR );
		}

	private static boolean isMatchingChineseWord( String wordString )
		{
		WordItem currentWordItem;

		// Do for all words
		if( wordString != null &&
		( currentWordItem = GlobalVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( currentWordItem.isMatchingChineseWord( wordString ) )
					return true;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return false;
		}

	private static boolean isSymbol( char character )
		{
		return ( character == Constants.SYMBOL_COMMA ||
				character == Constants.SYMBOL_COLON ||
				character == Constants.SYMBOL_SEMI_COLON ||
				character == Constants.SYMBOL_DOUBLE_COLON ||
				character == Constants.SYMBOL_EXCLAMATION_MARK ||
				character == Constants.SYMBOL_QUESTION_MARK ||
				character == Constants.SYMBOL_CHINESE_COMMA ||
				character == Constants.SYMBOL_CHINESE_COLON ||
				character == Constants.SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK ||
				character == Constants.SYMBOL_SPANISH_INVERTED_QUESTION_MARK ||
				character == Constants.SYMBOL_SLASH ||
				// Don't add Constants.SYMBOL_QUOTE to avoid analyzing merged words
				// Don't add Constants.SYMBOL_DOUBLE_QUOTE to avoid analyzing text strings
				character == Constants.SYMBOL_OPEN_ROUNDED_BRACKET ||
				character == Constants.SYMBOL_CLOSE_ROUNDED_BRACKET );
		}

	private static int bestMatchingChineseWordLength( String wordString )
		{
		int bestMatchingChineseWordLength = 0;
		int currentMatchingWordLength;
		WordItem currentWordItem;

		if( wordString != null &&
		( currentWordItem = GlobalVariables.firstWordItem ) != null )
			{
			// Do for all words
			do	{
				if( ( currentMatchingWordLength = currentWordItem.bestMatchingChineseWordLength( wordString ) ) > bestMatchingChineseWordLength )
					bestMatchingChineseWordLength = currentMatchingWordLength;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return bestMatchingChineseWordLength;
		}

	private CreateReadWordResultType createReadWord( boolean isUncountableGeneralizationNoun, short wordOrderNr, short wordTypeNr, int textStringStartPosition, String textString, WordItem readWordItem )
		{
		short wordParameter = ( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL ||
										readWordItem == null ? Constants.NO_WORD_PARAMETER : readWordItem.wordParameter() );
		CreateReadWordResultType createReadWordResult = new CreateReadWordResultType();
		ReadWordResultType readWordResult = new ReadWordResultType();

		if( wordTypeNr <= Constants.NO_WORD_TYPE_NR ||
		wordTypeNr >= Constants.NUMBER_OF_WORD_TYPES )
			return adminItem_.startCreateReadWordResultError( 1, moduleNameString_, "The given word type number is undefined or out of bounds" );

		if( textString == null &&
		readWordItem == null )
			return adminItem_.startCreateReadWordResultError( 1, moduleNameString_, "Both the given text string and the given read word item are undefined" );

		if( textString != null )
			{
			if( ( readWordResult = readWordFromString( false, false, false, true, 0, textString.substring( textStringStartPosition ) ) ).result != Constants.RESULT_OK )
				return adminItem_.addCreateReadWordResultError( 1, moduleNameString_, "I failed to read a word from a text string" );

			createReadWordResult.offset = readWordResult.offset;
			}

		if( adminItem_.createReadItem( isUncountableGeneralizationNoun, wordOrderNr, wordParameter, wordTypeNr, readWordResult.wordLength, ( textString != null ? textString.substring( textStringStartPosition + readWordResult.startWordPosition ) : null ), readWordItem ) != Constants.RESULT_OK )
			return adminItem_.addCreateReadWordResultError( 1, moduleNameString_, "I failed to create a read word item" );

		lastCreatedWordOrderNr_ = wordOrderNr;

		createReadWordResult.hasCreatedReadWord = true;
		return createReadWordResult;
		}

	private ShortResultType getWordTypeNr( boolean isCheckingProperNoun, boolean isForcingChineseProperNoun, int wordTypeStringLength, String wordTypeString )
		{
		char wordTypeChar;
		int index = 0;
		ShortResultType shortResult = new ShortResultType();

		if( wordTypeString == null )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given word type string is undefined" );

		if( wordTypeStringLength <= 0 )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given word type string is empty" );

		wordTypeChar = wordTypeString.charAt( 0 );

		if( Character.isLetter( wordTypeChar ) )
			{
			if( wordTypeStringLength == 1 )
				shortResult.shortValue = ( Character.isUpperCase( wordTypeChar ) ? Constants.WORD_TYPE_LETTER_CAPITAL : ( Character.isLowerCase( wordTypeChar ) ? Constants.WORD_TYPE_LETTER_SMALL : ( isCheckingProperNoun ? ( isForcingChineseProperNoun ? Constants.WORD_TYPE_PROPER_NOUN : Constants.WORD_TYPE_NOUN_SINGULAR ) : Constants.WORD_TYPE_NUMERAL ) ) );
			else
				{
				if( isCheckingProperNoun &&

				( isForcingChineseProperNoun ||
				Character.isUpperCase( wordTypeChar ) ) )
					shortResult.shortValue = Constants.WORD_TYPE_PROPER_NOUN;
				}
			}
		else
			{
			// Check for number
			while( index < wordTypeStringLength &&
			Character.isDigit( wordTypeString.charAt( index ) ) )
				index++;

			if( index == wordTypeStringLength )
				shortResult.shortValue = Constants.WORD_TYPE_NUMERAL;
			}


		return shortResult;
		}


	// Constructor

	protected AdminReadCreateWords( AdminItem adminItem )
		{
		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected short lastCreatedWordOrderNr()
		{
		return lastCreatedWordOrderNr_;
		}

	protected int convertChineseNumbers( char chineseChar )
		{
		switch( chineseChar )
			{
			case Constants.SYMBOL_CHINESE_ZERO:
				return 0;

			case Constants.SYMBOL_CHINESE_ONE:
				return 1;

			case Constants.SYMBOL_CHINESE_TWO:
				return 2;

			case Constants.SYMBOL_CHINESE_THREE:
				return 3;

			case Constants.SYMBOL_CHINESE_FOUR:
				return 4;

			case Constants.SYMBOL_CHINESE_FIVE:
				return 5;

			case Constants.SYMBOL_CHINESE_SIX:
				return 6;

			case Constants.SYMBOL_CHINESE_SEVEN:
				return 7;

			case Constants.SYMBOL_CHINESE_EIGHT:
				return 8;

			case Constants.SYMBOL_CHINESE_NINE:
				return 9;
			}

		return -1;
		}

	protected BoolResultType createReadWords( boolean isChineseCurrentLanguage, String readUserSentenceString )
		{
		boolean hasFoundAdjectiveEvery = false;
		boolean hasFoundChineseBasicVerb = false;
		boolean hasFoundChineseDefiniteArticle = false;
		boolean hasFoundChineseSingularNoun = false;
		boolean hasFoundLowerCase;
		boolean hasFoundProperNoun = false;
		boolean hasFoundSingularNoun;
		boolean hasFoundWordEnding;
		boolean isAdjective = false;
		boolean isAdverb;
		boolean isArticle = false;
		boolean isBasicVerb = false;
		boolean isCapitalPluralVerb;
		boolean isCheckingProperNoun;
		boolean isChineseAdjectiveCalled = false;
		boolean isChineseImperativeVerbDisplay = false;
		boolean isChineseProperNoun = false;
		boolean isConjunction = false;
		boolean isCreatedSingularNoun = false;
		boolean isExactPluralNoun = false;
		boolean isExactSingularNoun = false;
		boolean isExactWord;
		boolean isFirstFind;
		boolean isFirstWord = true;
		boolean isForcingChineseProperNoun;
		boolean isFrenchNumeral = false;
		boolean isPartOfMultipleWord;
		boolean isPossessiveDeterminer = false;
		boolean isPossessivePronoun = false;
		boolean isPreposition = false;
		boolean isSelection = false;
		boolean isSymbol = false;
		boolean isUncountableGeneralizationNoun;
		boolean isUndefinedWord = false;
		boolean isUpperChar;
		boolean wasPreviousWordAdjective;
		boolean wasPreviousWordArticle;
		boolean wasPreviousWordBasicVerb;
		boolean wasPreviousWordChineseAdjectiveCalled;
		boolean wasPreviousWordChineseImperativeVerbDisplay;
		boolean wasPreviousWordChineseProperNoun;
		boolean wasPreviousWordConjunction;
		boolean wasPreviousWordCreatedSingularNoun;
		boolean wasPreviousWordExactNoun;
		boolean wasPreviousWordFrenchNumeral;
		boolean wasPreviousWordPossessiveDeterminer;
		boolean wasPreviousWordPreposition;
		boolean wasPreviousWordSymbol;
		boolean wasPreviousWordUndefined;
		short currentLanguageNr = GlobalVariables.currentLanguageNr;
		short currentWordAdjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
		short currentWordDefiniteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		short currentWordIndefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		short currentWordOrderNr = Constants.NO_ORDER_NR;
		short foundWordTypeNr;
		short previousWordAdjectiveParameter;
		short previousWordDefiniteArticleParameter;
		short previousWordIndefiniteArticleParameter;
		short wordTypeNr = Constants.NO_WORD_TYPE_NR;
		char currentChar;
		char firstChar;
		int currentPosition = 0;
		int readUserSentenceStringLength;
		int wordStringLength;
		WordItem createdWordItem;
		WordItem foundWordItem;
		WordItem pluralNounWordItem;
		WordItem singularNounWordItem;
		WordTypeItem foundWordTypeItem;
		WordTypeItem tempWordTypeItem;
		String exactWordString = null;
		String lowerCaseWordString = null;
		String singularNounWordString = null;
		BoolResultType boolResult = new BoolResultType();
		CreateReadWordResultType createReadWordResult = new CreateReadWordResultType();
		ReadWordResultType readWordResult;
		ShortResultType shortResult;
		WordEndingResultType wordEndingResult;
		WordResultType wordResult;

		// Set to pass while loop for the first time
		if( readUserSentenceString == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given read user sentence string is undefined" );

		if( ( readUserSentenceStringLength = readUserSentenceString.length() ) == 0 )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given read user sentence string is empty" );

		if( !isChineseCurrentLanguage )
			{
			firstChar = readUserSentenceString.charAt( 0 );

			if( !Character.isUpperCase( firstChar ) &&
			firstChar != Constants.SYMBOL_SPANISH_INVERTED_QUESTION_MARK &&
			firstChar != Constants.SYMBOL_SPANISH_INVERTED_EXCLAMATION_MARK )
				isChineseCurrentLanguage = true;
			}

		do	{
			createReadWordResult.hasCreatedReadWord = false;

			if( ++currentWordOrderNr >= Constants.MAX_ORDER_NR )
				return adminItem_.startBoolResultSystemError( 1, moduleNameString_, "Word order number overflow! I can't accept more words" );

			if( ( currentChar = readUserSentenceString.charAt( currentPosition ) ) == Constants.QUERY_STRING_START_CHAR )
				{
				if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, Constants.WORD_TYPE_TEXT, currentPosition, readUserSentenceString, null ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a read word" );

				currentPosition += createReadWordResult.offset;
				}
			else
				{
				wasPreviousWordAdjective = isAdjective;
				wasPreviousWordArticle = isArticle;
				wasPreviousWordBasicVerb = isBasicVerb;
				wasPreviousWordChineseAdjectiveCalled = isChineseAdjectiveCalled;
				wasPreviousWordChineseImperativeVerbDisplay = isChineseImperativeVerbDisplay;
				wasPreviousWordChineseProperNoun = isChineseProperNoun;
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
				isChineseAdjectiveCalled = false;
				isChineseImperativeVerbDisplay = false;
				isChineseProperNoun = false;
				isConjunction = false;
				isCreatedSingularNoun = false;
				isExactPluralNoun = false;
				isExactSingularNoun = false;
				isExactWord = false;
				isForcingChineseProperNoun = false;
				isFrenchNumeral = false;
				isPartOfMultipleWord = true;
				isPossessiveDeterminer = false;
				isPossessivePronoun = false;
				isPreposition = false;
				isSymbol = false;
				isUndefinedWord = false;

				currentWordAdjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
				currentWordDefiniteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
				currentWordIndefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;

				createdWordItem = null;
				foundWordItem = null;
				pluralNounWordItem = null;
				singularNounWordItem = null;

				if( ( readWordResult = readWordFromString( false, isChineseCurrentLanguage, false, false, 0, readUserSentenceString.substring( currentPosition ) ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to read a word from the word string" );

				wordStringLength = readWordResult.wordLength;

				// Create exact word string
				exactWordString = readUserSentenceString.substring( currentPosition, ( currentPosition + wordStringLength ) );

				// Create lower-case word string
				lowerCaseWordString = readUserSentenceString.substring( currentPosition, ( currentPosition + wordStringLength ) ).toLowerCase();

				// Step 1: Find exact word types in all words
				do	{
					if( ( wordResult = findWordTypeInAllWords( false, Constants.NO_WORD_TYPE_NR, exactWordString, foundWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find an exact word type in all words" );

					// Found exact word
					if( ( foundWordItem = wordResult.foundWordItem ) != null &&
					( foundWordTypeItem = wordResult.foundWordTypeItem ) != null )
						{
						hasFoundSingularNoun = foundWordTypeItem.isSingularNoun();

						if( ( !isAdverb ||
						// Typical for Spanish: Skip answer, when an adverb was found too
						!foundWordTypeItem.isAnswer() ) &&

						( !isChineseCurrentLanguage ||
						// Typical for Chinese
						// Chinese test file: "Connect Four"
						// Chinese test file: "Acceptance - John is a person, John is tall and John is a boy"
						isFirstWord ||
						isSelection ||
						wordStringLength > 1 ||

						( !foundWordItem.isAdjectiveComparison() &&

						( hasFoundAdjectiveEvery ||
						!hasFoundSingularNoun ||
						!wasPreviousWordSymbol ||
						readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_QUESTION_MARK ) > 1 ) ) ) )
							{
							foundWordTypeNr = foundWordTypeItem.wordTypeNr();

							// Typical for Chinese: Possessive pronoun and possessive determiner are the same
							// Chinese test file: "Connect Four"
							if( isChineseCurrentLanguage &&
							foundWordTypeNr == Constants.WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR &&

							( hasFoundAdjectiveEvery ||
							hasFoundChineseDefiniteArticle ) &&

							( tempWordTypeItem = foundWordItem.activeWordTypeItem( Constants.WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR ) ) != null )
								{
								// Force Chinese possessive pronoun
								foundWordTypeNr = Constants.WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR;
								foundWordTypeItem = tempWordTypeItem;
								}

							if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, ( hasFoundSingularNoun && wasPreviousWordFrenchNumeral ? Constants.WORD_TYPE_NOUN_PLURAL : foundWordTypeNr ), 0, null, foundWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create another read word" );

							isExactWord = true;

							switch( foundWordTypeNr )
								{
								case Constants.WORD_TYPE_SYMBOL:
									isSymbol = true;
									break;

								case Constants.WORD_TYPE_NUMERAL:
									// Typical for French
									if( !isChineseCurrentLanguage )
										isFrenchNumeral = true;

									break;

								case Constants.WORD_TYPE_PROPER_NOUN:
									hasFoundProperNoun = true;
									break;

								case Constants.WORD_TYPE_ADJECTIVE:
									isAdjective = true;

									switch( foundWordItem.wordParameter() )
										{
										case Constants.WORD_PARAMETER_ADJECTIVE_NO:
											// Adjective 'no' can be used as article
											isArticle = true;
											break;

										case Constants.WORD_PARAMETER_ADJECTIVE_CALLED_NEUTRAL:
											isChineseAdjectiveCalled = true;
											break;

										case Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL:
											hasFoundAdjectiveEvery = true;
										}

									if( !wasPreviousWordArticle )
										currentWordAdjectiveParameter = foundWordTypeItem.adjectiveParameter();

									break;

								case Constants.WORD_TYPE_ADVERB:
									isAdverb = true;
									break;

								case Constants.WORD_TYPE_ARTICLE:
									isArticle = true;

									if( foundWordTypeItem.isDefiniteArticle() )
										{
										// Definite article
										hasFoundChineseDefiniteArticle = true;
										currentWordDefiniteArticleParameter = foundWordTypeItem.definiteArticleParameter();
										}
									else
										// Indefinite article
										currentWordIndefiniteArticleParameter = foundWordTypeItem.indefiniteArticleParameter();

									break;

								case Constants.WORD_TYPE_CONJUNCTION:
									isConjunction = true;
									break;

								case Constants.WORD_TYPE_NOUN_SINGULAR:
									hasFoundChineseSingularNoun = true;
									break;

								case Constants.WORD_TYPE_NOUN_PLURAL:
									isExactPluralNoun = true;
									break;

								case Constants.WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR:
								case Constants.WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL:
									isPossessiveDeterminer = true;
									break;

								case Constants.WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR:
								case Constants.WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL:
									isPossessivePronoun = true;
									break;

								case Constants.WORD_TYPE_PREPOSITION:
									isPreposition = true;
									break;

								case Constants.WORD_TYPE_VERB_SINGULAR:
									// Multiple word ends at reaching a verb
									isPartOfMultipleWord = false;

									if( foundWordItem.isBasicVerb() )
										{
										isBasicVerb = true;
										hasFoundChineseBasicVerb = true;
										}
									else
										{
										// Typical for Chinese
										if( isChineseCurrentLanguage &&
										foundWordItem.isImperativeVerbDisplay() )
											isChineseImperativeVerbDisplay = true;
										}

									break;

								case Constants.WORD_TYPE_SELECTION_WORD:
									isSelection = true;
								}

							if( hasFoundSingularNoun )
								{
								isExactSingularNoun = true;

								if( wasPreviousWordFrenchNumeral )
									{
									// Typical for French: Skip singular/plural noun mismatch of word 'fils'
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

										if( previousWordAdjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER )
											{
											if( !isChineseCurrentLanguage &&
											foundWordTypeItem.setAdjectiveParameter( previousWordAdjectiveParameter ) != Constants.RESULT_OK )
												return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to set the adjective parameter of a singular noun" );
											}
										else
											{
											if( previousWordDefiniteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER )
												{
												if( foundWordTypeItem.setDefiniteArticleParameter( previousWordDefiniteArticleParameter ) != Constants.RESULT_OK )
													return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to set the definite article parameter of a singular noun" );
												}
											else
												{
												if( previousWordIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
												foundWordTypeItem.setIndefiniteArticleParameter( previousWordIndefiniteArticleParameter ) != Constants.RESULT_OK )
													return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to set the indefinite article parameter of a singular noun" );
												}
											}
										}
									}
								}
							}
						}
					}
				// Allow multiple finds
				while( foundWordItem != null );

				if( !isBasicVerb &&
				!isPartOfMultipleWord &&
				!wasPreviousWordExactNoun &&

				// Delete obsolete read items, that where part of a mutliple word
				adminItem_.deleteReadItemsWithNonMatchingMultipleWordPart( currentWordOrderNr, readUserSentenceString.substring( currentPosition ) ) != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to delete the read items with a non-matching multiple word part" );

				isUpperChar = Character.isUpperCase( currentChar );

				if( isFirstWord &&
				isUpperChar &&

				( !isExactWord ||
				wordStringLength == 1 ) )
					{
					if( ( shortResult = getWordTypeNr( false, false, wordStringLength, lowerCaseWordString ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to get the word type number of a lower-case word" );

					if( ( wordTypeNr = shortResult.shortValue ) == Constants.NO_WORD_TYPE_NR )
						isUndefinedWord = true;

					// Step 2: Find word type with lower-case first letter in all words
					do	{
						if( ( wordResult = findWordTypeInAllWords( true, Constants.NO_WORD_TYPE_NR, lowerCaseWordString, foundWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a word type with difference case of the first letter in all words" );

						// Found word type with lower-case first letter
						if( ( foundWordItem = wordResult.foundWordItem ) != null &&
						( foundWordTypeItem = wordResult.foundWordTypeItem ) != null )
							{
							hasFoundLowerCase = true;
							foundWordTypeNr = foundWordTypeItem.wordTypeNr();

							if( foundWordTypeNr == wordTypeNr &&
							foundWordTypeItem.wordTypeLanguageNr() != currentLanguageNr &&

							// Create same word type for different language
							foundWordItem.addWordType( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, wordStringLength, lowerCaseWordString ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a word type with lower-case first letter" );

							if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, foundWordTypeNr, 0, null, foundWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a read word with a word type with difference case of the first letter" );

							switch( foundWordTypeNr )
								{
								case Constants.WORD_TYPE_ADJECTIVE:
									isAdjective = true;
									currentWordAdjectiveParameter = foundWordTypeItem.adjectiveParameter();

									if( foundWordItem.wordParameter() == Constants.WORD_PARAMETER_ADJECTIVE_EVERY_NEUTRAL )
										hasFoundAdjectiveEvery = true;

									break;

								case Constants.WORD_TYPE_ARTICLE:
									isArticle = true;

									if( foundWordTypeItem.isDefiniteArticle() )
										// Definite article
										currentWordDefiniteArticleParameter = foundWordTypeItem.definiteArticleParameter();
									else
										// Indefinite article
										currentWordIndefiniteArticleParameter = foundWordTypeItem.indefiniteArticleParameter();

									break;

								case Constants.WORD_TYPE_POSSESSIVE_DETERMINER_SINGULAR:
								case Constants.WORD_TYPE_POSSESSIVE_DETERMINER_PLURAL:
									isPossessiveDeterminer = true;
									break;

								case Constants.WORD_TYPE_POSSESSIVE_PRONOUN_SINGULAR:
								case Constants.WORD_TYPE_POSSESSIVE_PRONOUN_PLURAL:
									isPossessivePronoun = true;
									break;

								case Constants.WORD_TYPE_VERB_SINGULAR:
								case Constants.WORD_TYPE_VERB_PLURAL:
									isCapitalPluralVerb = true;
									break;

								case Constants.WORD_TYPE_SELECTION_WORD:
									isSelection = true;
								}
							}
						}
					// Allow multiple finds
					while( foundWordItem != null );

					// One character
					if( !isUndefinedWord )
						{
						// Step 3: Typically for English: Find or create lower-case letter 'a' as first letter of a sentence.
						if( ( wordResult = findWordTypeInAllWords( false, wordTypeNr, lowerCaseWordString, null ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a lower-case letter" );

						if( wordResult.foundWordItem == null )
							{
							if( ( wordResult = addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, wordTypeNr, wordStringLength, lowerCaseWordString ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add word with lower-case letter" );

							if( ( createdWordItem = wordResult.createdWordItem ) == null )
								return adminItem_.startBoolResultError( 1, moduleNameString_, "The created word with lower-case letter is undefined" );

							if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, wordTypeNr, 0, null, createdWordItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a read word with lower-case letter" );
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
						if( ( wordResult = findWordTypeInAllWords( true, Constants.NO_WORD_TYPE_NR, exactWordString, foundWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find an exact word" );

						createdWordItem = null;
						foundWordItem = wordResult.foundWordItem;
						foundWordTypeItem = wordResult.foundWordTypeItem;

						if( isFirstFind ||
						// Skip if later runs have no result
						foundWordItem != null )
							{
							isCheckingProperNoun = ( !isChineseCurrentLanguage ||
													// Typical for Chinese
													convertChineseNumbers( currentChar ) < 0 );

							// Typical for Chinese
							isForcingChineseProperNoun = ( isChineseCurrentLanguage &&

															// One character
															( ( wordStringLength == 1 &&

															( isFirstWord ||

															( hasFoundProperNoun &&
															!wasPreviousWordPreposition &&

															( ( hasFoundChineseBasicVerb &&
															wasPreviousWordSymbol ) ||

															( !hasFoundChineseBasicVerb &&

															( wasPreviousWordConjunction ||
															readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_ADJECTIVE_CALLED ) > 1 ) ) ||

															readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_PREPOSITION_OF ) > 1 ) ) ) ) ||

															// Multiple characters
															( wordStringLength > 1 &&

															( isFirstWord ||
															wasPreviousWordChineseAdjectiveCalled ||
															wasPreviousWordConjunction ) &&

															readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_COMMA ) < 0 &&
															readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_NUMERAL_ALL ) < 0 &&
															readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_VERB_HAS ) < 0 ) ) );

							if( ( shortResult = getWordTypeNr( isCheckingProperNoun, isForcingChineseProperNoun, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to get the word type number of an exact word" );

							wordTypeNr = shortResult.shortValue;

							// Typical for Chinese
							if( isChineseCurrentLanguage &&
							wordTypeNr == Constants.NO_WORD_TYPE_NR &&
							!isSelection &&
							wordStringLength > 1 )
								{
								if( hasFoundChineseSingularNoun )
									wordTypeNr = Constants.WORD_TYPE_NOUN_SINGULAR;
								else
									{
									if( wasPreviousWordChineseProperNoun ||

									// First word
									( isFirstWord &&

									( readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_VERB_HAS ) > 1 ||
									readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_CONJUNCTION_AND ) < 0 ) ) ||

									( wasPreviousWordBasicVerb &&
									readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_PREPOSITION_OF ) > 1 &&
									readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_QUESTION_MARK ) < 0 ) )
										wordTypeNr = Constants.WORD_TYPE_PROPER_NOUN;
									}
								}

							if( wordTypeNr == Constants.NO_WORD_TYPE_NR )
								isUndefinedWord = true;
							else
								{
								// Not found
								if( foundWordItem == null )
									{
									// Small letter, capital letter, numeral, proper noun and singular noun
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, wordTypeNr, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add an exact word" );

									if( ( createdWordItem = wordResult.createdWordItem ) == null )
										return adminItem_.startBoolResultError( 1, moduleNameString_, "I couldn't create an exact word" );

									if( wordTypeNr == Constants.WORD_TYPE_PROPER_NOUN )
										{
										hasFoundProperNoun = true;
										// Typical for Chinese
										isChineseProperNoun = true;
										}
									else
										{
										// Typical for Chinese
										if( wordTypeNr == Constants.WORD_TYPE_NOUN_SINGULAR )
											hasFoundChineseSingularNoun = true;
										}
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
									if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, wordTypeNr, 0, null, createdWordItem ) ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create an exact read word" );

									isFirstFind = false;
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
				wasPreviousWordCreatedSingularNoun ) ) ) ||

				// Typical for Chinese
				( isChineseCurrentLanguage &&
				isFirstWord &&
				wordTypeNr == Constants.WORD_TYPE_PROPER_NOUN &&

				( readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_COMMA ) > 1 ||
				readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_ADJECTIVE_EVERY ) > 1 ||
				readUserSentenceString.indexOf( Constants.SYMBOL_CHINESE_NUMERAL_ALL ) > 1 ) ) ) )
					{
					if( ( wordEndingResult = adminItem_.analyzeWordEndingWithCurrentLanguage( Constants.WORD_PLURAL_NOUN_ENDING, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to check the plural noun ending" );

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
							singularNounWordString = ( hasFoundLowerCase ? wordEndingResult.wordString : wordEndingResult.wordString.toLowerCase() );

							if( ( wordResult = findWordTypeInAllWords( false, Constants.WORD_TYPE_NOUN_SINGULAR, singularNounWordString, null ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find if a singular noun word already exists" );

							if( ( foundWordItem = wordResult.foundWordItem ) == null )
								{
								if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
									return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word" );

								if( ( pluralNounWordItem = wordResult.createdWordItem ) == null )
									return adminItem_.startBoolResultError( 1, moduleNameString_, "The created word item is undefined" );

								if( pluralNounWordItem.addWordType( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.WORD_TYPE_NOUN_SINGULAR, wordEndingResult.wordStringLength, singularNounWordString ).result != Constants.RESULT_OK )
									return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word type item for plural noun word \"" + pluralNounWordItem.anyWordTypeString() + "\"" );

								if( wordStringLength == wordEndingResult.wordStringLength &&
								exactWordString.equals( singularNounWordString ) )
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
							if( !isExactSingularNoun )
								{
								if( wasPreviousWordFrenchNumeral )
									{
									// Typical for French: Singular and plural noun 'fils' are the same
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word" );

									pluralNounWordItem = wordResult.createdWordItem;
									}
								else
									{
									if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, wordStringLength, exactWordString ) ).result != Constants.RESULT_OK )
										return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add a singular noun word" );

									isCreatedSingularNoun = true;
									singularNounWordItem = wordResult.createdWordItem;

									if( isUndefinedWord )
										hasFoundChineseSingularNoun = true;
									}
								}
							}

						if( singularNounWordItem != null &&
						// Singular noun
						( createReadWordResult = createReadWord( isUncountableGeneralizationNoun, currentWordOrderNr, Constants.WORD_TYPE_NOUN_SINGULAR, 0, null, singularNounWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a singular noun read word" );

						if( pluralNounWordItem != null &&
						// Plural noun
						( createReadWordResult = createReadWord( false, currentWordOrderNr, Constants.WORD_TYPE_NOUN_PLURAL, 0, null, pluralNounWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create a plural noun read word" );
						}

					// Step 7: Create an adjective
					if( isUndefinedWord &&
					!wasPreviousWordUndefined &&
					pluralNounWordItem == null &&

					( isFirstWord ||
					isSelection ||
					wasPreviousWordBasicVerb ||
					wasPreviousWordConjunction ||
					wasPreviousWordExactNoun ||
					wasPreviousWordSymbol ) )
						{
						if( ( wordResult = addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_ADJECTIVE, wordStringLength, lowerCaseWordString ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to add an adjective word" );

						if( ( createdWordItem = wordResult.createdWordItem ) == null )
							return adminItem_.startBoolResultError( 1, moduleNameString_, "The last created adjective word is undefined" );

						if( ( createReadWordResult = createReadWord( false, currentWordOrderNr, Constants.WORD_TYPE_ADJECTIVE, 0, null, createdWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create an adjective read word" );

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

	protected ReadWordResultType readWordFromString( boolean isCheckingForGrammarDefinition, boolean isChineseCurrentLanguage, boolean isMergedWord, boolean isSkippingTextString, int minimumStringLength, String wordString )
		{
		boolean isText = false;
		char currentChar;
		char previousChar = Constants.SPACE_CHAR;
		int bestMatchingChineseWordLength = 0;
		int index = 0;
		int wordStringLength;
		ReadWordResultType readWordResult = new ReadWordResultType();

		if( wordString == null )
			return adminItem_.startReadWordResultError( 1, moduleNameString_, "The given word string is undefined" );

		if( ( wordStringLength = wordString.length() ) <= 0 )
			return adminItem_.startReadWordResultError( 1, moduleNameString_, "The given word string is empty" );

		currentChar = wordString.charAt( 0 );

		if( isSymbol( currentChar ) &&

		( !isCheckingForGrammarDefinition ||
		!isGrammarDefinitionSymbol( currentChar ) ) )
			{
			index++;
			readWordResult.wordLength++;
			}
		else
			{
			// Typical for Chinese
			if( isChineseCurrentLanguage &&
			!Character.isLowerCase( currentChar ) &&
			!Character.isUpperCase( currentChar ) &&
			!Character.isDigit( currentChar ) &&

			// Chinese family definition
			( ( currentChar != Constants.SYMBOL_CHINESE_NOUN_MAN &&
			// Chinese family definition
			currentChar != Constants.SYMBOL_CHINESE_NOUN_WOMAN &&
			( bestMatchingChineseWordLength = bestMatchingChineseWordLength( wordString ) ) > 0 ) ||

			( bestMatchingChineseWordLength = bestMatchingChineseWordLength( wordString ) ) > 1 ) )
				{
				index = bestMatchingChineseWordLength;
				readWordResult.wordLength = bestMatchingChineseWordLength;
				}
			else
				{
				while( index < wordStringLength &&

				( isText ||
				readWordResult.wordLength < minimumStringLength ||

				// Grammar / interface file
				( ( !isSymbol( currentChar ) ||

				// Grammar / interface file
				( isCheckingForGrammarDefinition &&
				isGrammarDefinitionSymbol( currentChar ) ) ) &&

				// Typical for French: Include spaces in grammar merged word definition
				( ( isMergedWord &&
				currentChar == Constants.SPACE_CHAR ) ||

				( !Character.isWhitespace( currentChar ) &&

				( isCheckingForGrammarDefinition ||
				index < 1 ||

				// Grammar / interface file
				currentChar == Constants.SYMBOL_UNDERSCORE ||
				currentChar == Constants.SYMBOL_APOSTROPHE ||
				currentChar == Constants.SYMBOL_MINUS ||
				Character.isLowerCase( currentChar ) ||
				Character.isUpperCase( currentChar ) ||
				Character.isDigit( currentChar ) ||

				// Typical for Chinese
				( isChineseCurrentLanguage &&

				( previousChar == Constants.SYMBOL_CHINESE_DOT ||
				!isMatchingChineseWord( wordString.substring( index ) ) ) ) ) ) ) ) ) )
					{
					if( isCheckingForGrammarDefinition &&
					isGrammarDefinitionSymbol( currentChar ) )
						readWordResult.hasFoundGrammarDefinition = true;

					if( currentChar == Constants.SYMBOL_DOUBLE_QUOTE &&

					( index == 0 ||
					// Skip escaped double quote character
					previousChar != Constants.SYMBOL_BACK_SLASH ) )
						isText = !isText;

					readWordResult.wordLength++;

					// Get new word string character
					if( ++index < wordStringLength )
						{
						previousChar = currentChar;
						currentChar = wordString.charAt( index );
						}
					}
				}

			if( isSkippingTextString &&
			readWordResult.wordLength > 1 )
				readWordResult.wordLength--;

			if( isSkippingTextString &&
			index > 1 &&
			readWordResult.wordLength > 1 &&
			wordString.charAt( index - 1 ) == Constants.QUERY_STRING_END_CHAR )
				{
				readWordResult.wordLength--;
				readWordResult.startWordPosition++;
				}
			}

		// Skip spaces
		while( index < wordStringLength &&
		Character.isWhitespace( wordString.charAt( index ) ) )
			index++;

		readWordResult.offset = index;
		return readWordResult;
		}

	protected WordResultType addWord( boolean isLanguageWord, boolean isMultipleWord, short previousWordAdjectiveParameter, short previousWordDefiniteArticleParameter, short previousWordIndefiniteArticleParameter, short wordParameter, short wordTypeNr, int wordTypeStringLength, String wordTypeString )
		{
		boolean isProperNounPrecededByDefiniteArticle;
		boolean wasPreviousWordAdjective = ( previousWordAdjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER );
		boolean wasPreviousWordDefiniteArticle = ( previousWordDefiniteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER );
		boolean wasPreviousWordIndefiniteArticle = ( previousWordIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER );
		short adjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
		short definiteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		short indefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem createdWordItem;
		WordResultType wordResult;
		WordTypeResultType wordTypeResult;

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

			if( GlobalVariables.firstWordItem == null )
				// Remember the first word
				GlobalVariables.firstWordItem = createdWordItem;

			if( wordParameter > Constants.NO_WORD_PARAMETER )
				{
				// Remember the first predefined word
				if( GlobalVariables.firstPredefinedWordItem == null )
					GlobalVariables.firstPredefinedWordItem = createdWordItem;

				// Remember the last predefined word
				GlobalVariables.lastPredefinedWordItem = createdWordItem;
				}

			if( adminItem_.isAdjectiveParameter( wordParameter ) )
				adjectiveParameter = wordParameter;
			else
				{
				if( wasPreviousWordAdjective )
					adjectiveParameter = previousWordAdjectiveParameter;
				}

			if( adminItem_.isDefiniteArticleParameter( wordParameter ) )
				definiteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordDefiniteArticle )
					definiteArticleParameter = previousWordDefiniteArticleParameter;
				}

			if( adminItem_.isIndefiniteArticleParameter( wordParameter ) )
				indefiniteArticleParameter = wordParameter;
			else
				{
				if( wasPreviousWordIndefiniteArticle )
					indefiniteArticleParameter = previousWordIndefiniteArticleParameter;
				}

			isProperNounPrecededByDefiniteArticle = ( wasPreviousWordDefiniteArticle &&
													wordTypeNr == Constants.WORD_TYPE_PROPER_NOUN );

			if( ( wordTypeResult = createdWordItem.addWordType( isMultipleWord, isProperNounPrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordTypeStringLength, wordTypeString ) ).result != Constants.RESULT_OK )
				return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to add a word type to a new word" );

			if( indefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
			wordTypeResult.wordTypeItem != null &&
			adminItem_.isNounWordType( wordTypeNr ) &&
			!wordTypeResult.wordTypeItem.isCorrectIndefiniteArticle( false, indefiniteArticleParameter ) &&

			// Write 'different indefinite article used' notification
			InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_INDEFINITE_ARTICLE_WITH_NOUN_START, wordTypeString, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_DIFFERENT_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) != Constants.RESULT_OK )
				return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to write the 'different indefinite article used' interface notification" );
			}

		wordResult.foundWordItem = wordResult.foundWordItem;
		wordResult.foundWordTypeItem = wordResult.foundWordTypeItem;
		return wordResult;
		}

	protected WordResultType findWordTypeInAllWords( boolean isCheckingAllLanguages, short wordTypeNr, String wordTypeString, WordItem previousWordItem )
		{
		WordItem currentWordItem;
		WordResultType wordResult;

		if( ( currentWordItem = ( previousWordItem == null ? GlobalVariables.firstWordItem : previousWordItem.nextWordItem() ) ) != null )
			{
			do	{
				if( ( wordResult = currentWordItem.findWordType( isCheckingAllLanguages, wordTypeNr, wordTypeString ) ).result != Constants.RESULT_OK )
					return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to find a word type in word \"" + currentWordItem.anyWordTypeString() + "\"" );

				if( wordResult.foundWordItem != null )
					return wordResult;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return new WordResultType();
		}
	};

/*************************************************************************
 *	"How joyful are those who fear the Lord-
 *	all who follow his ways!" (Psalm 128:1)
 *************************************************************************/
