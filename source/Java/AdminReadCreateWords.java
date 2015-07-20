/*
 *	Class:			AdminReadCreateWords
 *	Supports class:	AdminItem
 *	Purpose:		To create words of the read sentence
 *	Version:		Thinknowlogy 2015r1beta (Corazón)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your suggestions, modifications and bug reports are welcome at
 *	http://mafait.org
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
	// Private constructible variables

	private boolean hasCreatedReadWord_;
	private boolean hasFoundDifferentParameter_;

	private short currentWordOrderNr_;
	private short lastCreatedWordOrderNr_;
	private short wordTypeNr_;

	private int nextTextStringPosition_;

	private String exactWordString_;
	private String lowerCaseWordString_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

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
				character == Constants.SYMBOL_PIPE ||
				character == Constants.SYMBOL_AMPERSAND ||
				character == Constants.SYMBOL_ASTERISK ||
				character == Constants.SYMBOL_PERCENT ||
				character == Constants.SYMBOL_DOLLAR ||
				character == Constants.SYMBOL_SLASH ||
				character == Constants.SYMBOL_BACK_SLASH ||
				character == Constants.SYMBOL_QUOTE ||
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

	private static boolean isGrammarDefinitionSymbol( char character )
		{
		return ( character == Constants.QUERY_WORD_TYPE_CHAR ||
				character == Constants.QUERY_PARAMETER_CHAR ||
				character == Constants.GRAMMAR_WORD_DEFINITION_CHAR );
		}

	private ReadResultType createWordStrings( int startPosition, String wordString )
		{
		ReadResultType readResult = new ReadResultType();
		exactWordString_ = null;
		lowerCaseWordString_ = null;

		if( wordString != null )
			{
			if( ( readResult = readWordFromString( false, false, startPosition, 0, wordString ) ).result == Constants.RESULT_OK )
				{
				if( readResult.wordLength > 0 )
					{
					exactWordString_ = wordString.substring( startPosition, ( startPosition + readResult.wordLength ) );
					lowerCaseWordString_ = wordString.substring( startPosition, ( startPosition + readResult.wordLength ) ).toLowerCase();
					}
				else
					adminItem_.startErrorInItem( 1, moduleNameString_, "The given word string is empty or has no words left anymore" );
				}
			else
				adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a word from the word string" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given word string is undefined" );

		readResult.result = CommonVariables.result;
		return readResult;
		}

	private byte getWordTypeNr( boolean isCheckingProperName, int wordTypeStringLength, String wordTypeString )
		{
		int wordPosition = 0;

		wordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

		if( wordTypeString != null )
			{
			if( wordTypeStringLength > 0 )
				{
				if( Character.isLetter( wordTypeString.charAt( wordPosition ) ) )
					{
					if( wordTypeStringLength == 1 )
						wordTypeNr_ = ( Character.isUpperCase( wordTypeString.charAt( wordPosition ) ) ? Constants.WORD_TYPE_LETTER_CAPITAL : Constants.WORD_TYPE_LETTER_SMALL );
					else
						{
						if( isCheckingProperName &&
						Character.isUpperCase( wordTypeString.charAt( wordPosition ) ) )
							wordTypeNr_ = Constants.WORD_TYPE_PROPER_NAME;
						}
					}
				else
					{
					while( wordPosition < wordTypeStringLength &&
					Character.isDigit( wordTypeString.charAt( wordPosition ) ) )
						wordPosition++;

					if( wordPosition == wordTypeStringLength )
						wordTypeNr_ = Constants.WORD_TYPE_NUMERAL;
					}
				}
			else
				return adminItem_.startErrorInItem( 1, moduleNameString_, "The given word type string is empty" );
			}
		else
			return adminItem_.startErrorInItem( 1, moduleNameString_, "The given word type string is undefined" );

		return Constants.RESULT_OK;
		}


	// Constructor / deconstructor

	protected AdminReadCreateWords( AdminItem adminItem )
		{
		String errorString = null;

		hasCreatedReadWord_ = false;
		hasFoundDifferentParameter_ = false;

		currentWordOrderNr_ = Constants.NO_ORDER_NR;
		lastCreatedWordOrderNr_ = Constants.NO_ORDER_NR;
		wordTypeNr_ = Constants.WORD_TYPE_UNDEFINED;

		nextTextStringPosition_ = 0;

		exactWordString_ = null;
		lowerCaseWordString_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
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

	protected ReadResultType createReadWord( short wordOrderNr, short wordTypeNr, int textStringStartPosition, String textString, WordItem readWordItem )
		{
		ReadResultType readResult = new ReadResultType();
		short wordParameter = ( wordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL || readWordItem == null ? Constants.NO_WORD_PARAMETER : readWordItem.wordParameter() );
		String readString = null;

		hasCreatedReadWord_ = false;
		nextTextStringPosition_ = 0;

		if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED )
			{
			if( textString != null ||
			readWordItem != null )
				{
				if( textString != null )
					{
					if( ( readResult = readWordFromString( false, true, textStringStartPosition, 0, textString ) ).result == Constants.RESULT_OK )
						{
						if( readResult.startWordPosition < textString.length() )
							{
							nextTextStringPosition_ = readResult.nextWordPosition;
							readString = textString.substring( readResult.startWordPosition );
							}
						else
							adminItem_.startErrorInItem( 1, moduleNameString_, "The found start word position is invalid" );
						}
					else
						adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a word from a text string" );
					}

				if( CommonVariables.result == Constants.RESULT_OK )
					{
					if( adminItem_.readList == null )
						{
						// Create list
						if( ( adminItem_.readList = new ReadList( adminItem_ ) ) != null )
							adminItem_.adminListArray[Constants.ADMIN_READ_LIST] = adminItem_.readList;
						else
							adminItem_.startErrorInItem( 1, moduleNameString_, "I failed to create a read list" );
						}
					else
						{
						// Find out if already exists
						if( adminItem_.readList.hasFoundReadItem( wordOrderNr, wordParameter, wordTypeNr, readString, readWordItem ) )
							adminItem_.startErrorInItem( 1, moduleNameString_, "The given read item already exists" );
						}

					if( ( readResult = adminItem_.readList.createReadItem( wordOrderNr, wordParameter, wordTypeNr, readResult.wordLength, readString, readWordItem ) ).result == Constants.RESULT_OK )
						{
						hasCreatedReadWord_ = true;
						lastCreatedWordOrderNr_ = wordOrderNr;
						}
					else
						adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create an admin read words item" );
					}
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "Both the given text string and the given read word item are undefined" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given word type number is undefined" );

		readResult.result = CommonVariables.result;
		return readResult;
		}

	protected ReadResultType createReadWords( String readSentenceString )
		{
		GrammarResultType grammarResult;
		ReadResultType readResult = new ReadResultType();
		WordResultType wordResult;
		boolean isCapitalSingularNoun;
		boolean isCapitalVerb;
		boolean isExactVerb;
		boolean isExactWord;
		boolean isFirstFind;
		boolean isNotPartOfMultipleWord;
		boolean isNumeral;
		boolean wasPreviousWordAdjective;
		boolean wasPreviousWordArticle;
		boolean wasPreviousWordBasicVerb;
		boolean wasPreviousWordConjunction;
		boolean wasPreviousWordExactNoun;
		boolean wasPreviousWordPossessiveDeterminer;
		boolean wasPreviousWordPossessivePronoun;
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
		short currentLanguageNr = CommonVariables.currentLanguageNr;
		int readSentenceStringLength;
		int wordStringLength;
		int nextWordPosition = 0;
		int readPosition = 0;
		WordItem createdWordItem;
		WordItem currentLanguageWordItem;
		WordItem foundWordItem;
		WordItem pluralNounWordItem;
		WordItem singularNounWordItem;
		WordTypeItem foundWordTypeItem;
		ReadList readList;

		// Set to pass while loop for the first time
		hasCreatedReadWord_ = true;
		hasFoundDifferentParameter_ = false;
		currentWordOrderNr_ = Constants.NO_ORDER_NR;

		if( readSentenceString != null )
			{
			if( ( readSentenceStringLength = readSentenceString.length() ) > 0 )
				{
				do	{
					if( ++currentWordOrderNr_ < Constants.MAX_ORDER_NR )
						{
						hasCreatedReadWord_ = false;

						if( readSentenceString.charAt( readPosition ) == Constants.SYMBOL_DOUBLE_QUOTE )
							{
							if( createReadWord( currentWordOrderNr_, Constants.WORD_TYPE_TEXT, readPosition, readSentenceString, null ).result == Constants.RESULT_OK )
								nextWordPosition = nextTextStringPosition_;
							else
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a read word" );
							}
						else
							{
							wasPreviousWordAdjective = isAdjective;
							wasPreviousWordArticle = isArticle;
							wasPreviousWordBasicVerb = isBasicVerb;
							wasPreviousWordConjunction = isConjunction;
							wasPreviousWordExactNoun = ( isExactSingularNoun || isExactPluralNoun );
							wasPreviousWordPossessiveDeterminer = isPossessiveDeterminer;
							wasPreviousWordPossessivePronoun = isPossessivePronoun;
							wasPreviousWordPreposition = isPreposition;
							wasPreviousWordProperName = isProperName;
							wasPreviousWordSymbol = isSymbol;
							wasPreviousWordUndefined = isUndefinedWord;

							previousWordAdjectiveParameter = currentWordAdjectiveParameter;
							previousWordDefiniteArticleParameter = currentWordDefiniteArticleParameter;
							previousWordIndefiniteArticleParameter = currentWordIndefiniteArticleParameter;

							isAdjective = false;
							isArticle = false;
							isBasicVerb = false;
							isCapitalVerb = false;
							isCapitalSingularNoun = false;
							isConjunction = false;
							isExactPluralNoun = false;
							isExactSingularNoun = false;
							isExactVerb = false;
							isExactWord = false;
							isNumeral = false;
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
							pluralNounWordItem = null;
							singularNounWordItem = null;

							if( ( readResult = createWordStrings( readPosition, readSentenceString ) ).result == Constants.RESULT_OK )
								{
								isNotPartOfMultipleWord = false;

								nextWordPosition = readResult.nextWordPosition;
								wordStringLength = readResult.wordLength;
								foundWordItem = null;

								// Step 1: Find exact word types in all words
								do	{
									if( ( wordResult = adminItem_.findWordTypeInAllWords( false, Constants.WORD_TYPE_UNDEFINED, exactWordString_, foundWordItem ) ).result == Constants.RESULT_OK )
										{
										// Found exact word
										if( ( foundWordItem = wordResult.foundWordItem ) != null &&
										( foundWordTypeItem = wordResult.foundWordTypeItem ) != null )
											{
											if( createReadWord( currentWordOrderNr_, foundWordTypeItem.wordTypeNr(), 0, null, foundWordItem ).result == Constants.RESULT_OK )
												{
												isExactWord = true;

												if( foundWordTypeItem.isAdjective() )
													{
													isAdjective = true;

													if( wordResult.foundWordItem != null &&
													// Adjective 'no' can be used as article
													wordResult.foundWordItem.isAdjectiveNo() )
														isArticle = true;

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

												if( foundWordItem.isBasicVerb() )
													isBasicVerb = true;

												if( foundWordTypeItem.isConjunction() )
													isConjunction = true;

												if( foundWordTypeItem.isVerb() )
													{
													isExactVerb = true;

													if( !foundWordItem.isMultipleWord() )
														isNotPartOfMultipleWord = true;
													}

												if( foundWordTypeItem.isNumeral() )
													isNumeral = true;

												if( foundWordTypeItem.isSingularNoun() )
													{
													isExactSingularNoun = true;

													if( !foundWordItem.isMultipleWord() )
														isNotPartOfMultipleWord = true;

													if( ( wordResult = foundWordTypeItem.setParameter( previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter ) ).result == Constants.RESULT_OK )
														{
														if( wordResult.hasFoundDifferentParameter )
															hasFoundDifferentParameter_ = true;
														}
													else
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to set a parameter of a singular noun" );
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
											else
												adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create another read word" );
											}
										}
									else
										adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find an exact word type in all words" );
									}
								while( CommonVariables.result == Constants.RESULT_OK &&
								// Allow multiple finds
								foundWordItem != null );

								if( CommonVariables.result == Constants.RESULT_OK &&
								isNotPartOfMultipleWord &&
								!wasPreviousWordExactNoun &&
								( readList = adminItem_.readList ) != null )
									{
									if( readList.deleteInvalidPartOfMultipleWordReadItems( currentWordOrderNr_, readSentenceString.substring( readPosition ) ) != Constants.RESULT_OK )
										adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to delete the read items with part of multiple words" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&
								isFirstWord &&
								Character.isUpperCase( readSentenceString.charAt( readPosition ) ) )
									{
									if( getWordTypeNr( false, wordStringLength, lowerCaseWordString_ ) == Constants.RESULT_OK )
										{
										foundWordItem = null;

										if( wordTypeNr_ == Constants.WORD_TYPE_UNDEFINED )
											isUndefinedWord = true;

										// Step 2: Find word type with lowercase first letter in all words
										do	{
											if( ( wordResult = adminItem_.findWordTypeInAllWords( true, Constants.WORD_TYPE_UNDEFINED, lowerCaseWordString_, foundWordItem ) ).result == Constants.RESULT_OK )
												{
												// Found word type with lowercase first letter
												if( ( foundWordItem = wordResult.foundWordItem ) != null &&
												( foundWordTypeItem = wordResult.foundWordTypeItem ) != null )
													{
													foundWordTypeNr = foundWordTypeItem.wordTypeNr();

													if( foundWordTypeNr == wordTypeNr_ &&
													foundWordTypeItem.wordTypeLanguageNr() != currentLanguageNr )
														{
														// Create same word type for different language
														if( foundWordItem.addWordType( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr_, wordStringLength, lowerCaseWordString_ ).result != Constants.RESULT_OK )
															adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a word type with lowercase first letter" );
														}

													if( createReadWord( currentWordOrderNr_, foundWordTypeNr, 0, null, foundWordItem ).result == Constants.RESULT_OK )
														{
														if( foundWordTypeItem.isAdjective() )
															{
															isAdjective = true;

															if( wordResult.foundWordItem != null &&
															// Adjective 'no' can be used as article
															wordResult.foundWordItem.isAdjectiveNo() )
																isArticle = true;

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
													else
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a read word with a word type with difference case of the first letter" );
													}
												}
											else
												adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a word type with difference case of the first letter in all words" );
											}
										while( CommonVariables.result == Constants.RESULT_OK &&
										// Allow multiple finds
										foundWordItem != null );

										if( CommonVariables.result == Constants.RESULT_OK &&
										!isUndefinedWord &&
										wordStringLength == 1 )
											{
											// Step 3: Typically for English: Find or create lowercase letter 'a' as first letter of a sentence.
											if( ( wordResult = adminItem_.findWordTypeInAllWords( false, wordTypeNr_, lowerCaseWordString_, null ) ).result == Constants.RESULT_OK )
												{
												if( wordResult.foundWordItem == null )
													{
													if( ( wordResult = addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, wordTypeNr_, wordStringLength, lowerCaseWordString_ ) ).result == Constants.RESULT_OK )
														{
														if( ( createdWordItem = wordResult.createdWordItem ) != null )
															{
															if( createReadWord( currentWordOrderNr_, wordTypeNr_, 0, null, createdWordItem ).result != Constants.RESULT_OK )
																adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a read word with lowercase letter" );
															}
														else
															adminItem_.startErrorInItem( 1, moduleNameString_, "The created word with lowercase letter is undefined" );
														}
													else
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add word with lowercase letter" );
													}
												}
											else
												adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a lowercase letter" );
											}
										}
									else
										adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get the word type number of a lowercase word" );
									}

								if( CommonVariables.result == Constants.RESULT_OK &&

								( !isExactWord ||

								( wasPreviousWordSymbol &&
								wordStringLength == 1 &&

								// Typically for Spanish: 'o' and 'y' are letters as well as conjunctions
								( isConjunction ||
								// Typically for Dutch: 'u' is a letter as well as a possessive pronoun
								isPossessivePronoun ) ) ) )
									{
									isFirstFind = true;
									foundWordItem = null;

									// Step 4: Find exact word types in all words
									do	{
										if( ( wordResult = adminItem_.findWordTypeInAllWords( true, Constants.WORD_TYPE_UNDEFINED, exactWordString_, foundWordItem ) ).result == Constants.RESULT_OK )
											{
											createdWordItem = null;
											foundWordItem = wordResult.foundWordItem;
											foundWordTypeItem = wordResult.foundWordTypeItem;

											if( isFirstFind ||

											( foundWordItem != null &&
											// Skip if later runs have no result
											foundWordTypeItem != null ) )
												{
												if( getWordTypeNr( true, wordStringLength, exactWordString_ ) == Constants.RESULT_OK )
													{
													if( wordTypeNr_ == Constants.WORD_TYPE_UNDEFINED )
														isUndefinedWord = true;
													else
														{
														if( foundWordItem == null )
															{
															// Small letters, capital letters, numerals and proper names
															if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, wordTypeNr_, wordStringLength, exactWordString_ ) ).result == Constants.RESULT_OK )
																{
																if( ( createdWordItem = wordResult.createdWordItem ) == null )
																	adminItem_.startErrorInItem( 1, moduleNameString_, "I couldn't create an exact word" );
																}
															else
																adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add an exact word" );
															}
														else
															{
															if( foundWordTypeItem != null &&
															foundWordTypeItem.wordTypeNr() == wordTypeNr_ &&
															foundWordTypeItem.wordTypeLanguageNr() != currentLanguageNr )
																{
																// Create same word type for different language
																if( foundWordItem.addWordType( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr_, wordStringLength, exactWordString_ ).result == Constants.RESULT_OK )
																	createdWordItem = foundWordItem;
																else
																	adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add an exact word type" );
																}
															}

														if( CommonVariables.result == Constants.RESULT_OK &&
														createdWordItem != null )
															{
															if( ( readResult = createReadWord( currentWordOrderNr_, wordTypeNr_, 0, null, createdWordItem ) ).result == Constants.RESULT_OK )
																{
																isFirstFind = false;

																if( wordTypeNr_ == Constants.WORD_TYPE_PROPER_NAME )
																	isProperName = true;
																}
															else
																adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create an exact read word" );
															}
														}
													}
												else
													adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to get the word type number of an exact word" );
												}
											}
										else
											adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find an exact word" );
										}
									while( CommonVariables.result == Constants.RESULT_OK &&
									// Allow multiple finds
									foundWordItem != null );
									}

								// Create a noun
								if( CommonVariables.result == Constants.RESULT_OK &&
								!isAdjective &&
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
								wasPreviousWordArticle ||

								( !isExactVerb &&
								wasPreviousWordConjunction ) ) &&

								( isFirstWord ||
								!Character.isUpperCase( readSentenceString.charAt( readPosition ) ) ) )
									{
									if( ( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
										{
										if( ( grammarResult = currentLanguageWordItem.checkOnWordEnding( Constants.WORD_PLURAL_NOUN_ENDING, wordStringLength, exactWordString_ ) ).result == Constants.RESULT_OK )
											{
											if( wasPreviousWordAdjective ||
											wasPreviousWordArticle ||
											wasPreviousWordBasicVerb ||
											wasPreviousWordConjunction ||
											wasPreviousWordPossessiveDeterminer ||
											wasPreviousWordPossessivePronoun ||
											wasPreviousWordPreposition ||
											wasPreviousWordProperName ||
											wasPreviousWordSymbol ||
											grammarResult.hasFoundWordEnding )
												{
												if( grammarResult.hasFoundWordEnding )
													{
													if( ( wordResult = adminItem_.findWordTypeInAllWords( false, Constants.WORD_TYPE_NOUN_SINGULAR, grammarResult.singularNounWordString, null ) ).result == Constants.RESULT_OK )
														{
														if( ( foundWordItem = wordResult.foundWordItem ) == null )
															{
															if( ( wordResult = addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ) ).result == Constants.RESULT_OK )
																{
																if( ( pluralNounWordItem = wordResult.createdWordItem ) != null )
																	{
																	if( pluralNounWordItem.addWordType( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, grammarResult.singularNounWordStringLength, grammarResult.singularNounWordString ).result == Constants.RESULT_OK )
																		{
																		if( wordStringLength == grammarResult.singularNounWordStringLength &&
																		exactWordString_.equals( grammarResult.singularNounWordString ) )
																			singularNounWordItem = pluralNounWordItem;
																		}
																	else
																		adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a singular noun word type item for plural noun word \"" + pluralNounWordItem.anyWordTypeString() + "\"" );
																	}
																else
																	adminItem_.startErrorInItem( 1, moduleNameString_, "The created word item is undefined" );
																}
															else
																adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a singular noun word" );
															}
														else
															{
															// Found singular noun
															if( !isExactPluralNoun )
																{
																if( foundWordItem.addWordType( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ).result == Constants.RESULT_OK )
																	pluralNounWordItem = foundWordItem;
																else
																	adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a plural noun word type item for word \"" + foundWordItem.anyWordTypeString() + "\"" );
																}
															}
														}
													else
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find if a singular noun word already exists" );
													}
												else
													{
													if( !isCapitalSingularNoun &&
													!isExactSingularNoun )
														{
														if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, wordStringLength, exactWordString_ ) ).result == Constants.RESULT_OK )
															singularNounWordItem = wordResult.createdWordItem;
														else
															adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a singular noun word" );
														}
													}

												if( CommonVariables.result == Constants.RESULT_OK &&
												singularNounWordItem != null )
													{
													if( createReadWord( currentWordOrderNr_, Constants.WORD_TYPE_NOUN_SINGULAR, 0, null, singularNounWordItem ).result != Constants.RESULT_OK )
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a singular noun read word" );
													}

												if( CommonVariables.result == Constants.RESULT_OK &&
												grammarResult.hasFoundWordEnding &&
												pluralNounWordItem != null )
													{
													if( createReadWord( currentWordOrderNr_, Constants.WORD_TYPE_NOUN_PLURAL, 0, null, pluralNounWordItem ).result != Constants.RESULT_OK )
														adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a plural noun read word" );
													}
												}

											// Create an adjective
											if( CommonVariables.result == Constants.RESULT_OK &&
											!wasPreviousWordUndefined &&

											( !isExactWord ||

											( wasPreviousWordBasicVerb &&
											isExactSingularNoun ) ) &&

											( wasPreviousWordConjunction ||
											wasPreviousWordSymbol ||
											pluralNounWordItem == null ) )
												{
												if( ( wordResult = addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_ADJECTIVE, wordStringLength, lowerCaseWordString_ ) ).result == Constants.RESULT_OK )
													{
													if( ( createdWordItem = wordResult.createdWordItem ) != null )
														{
														if( createReadWord( currentWordOrderNr_, Constants.WORD_TYPE_ADJECTIVE, 0, null, createdWordItem ).result == Constants.RESULT_OK )
															isAdjective = true;
														else
															adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create an adjective read word" );
														}
													else
														adminItem_.startErrorInItem( 1, moduleNameString_, "The last created adjective word is undefined" );
													}
												else
													adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add an adjective word" );
												}
											}
										else
											adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to check the plural noun ending" );
										}
									else
										adminItem_.startErrorInItem( 1, moduleNameString_, "The current language word item is undefined" );
									}
								}
							else
								adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create the word strings" );
							}

						// Don't mark Spanish reversed question mark as first word
						if( !isSymbol )
							isFirstWord = false;

						readPosition = nextWordPosition;
						}
					else
						adminItem_.startSystemErrorInItem( 1, moduleNameString_, "Word order number overflow! I can't accept more words" );
					}
				while( CommonVariables.result == Constants.RESULT_OK &&
				hasCreatedReadWord_ &&
				readPosition < readSentenceStringLength );
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "The given read sentence string is empty" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given read sentence string is undefined" );

		readResult.hasCreatedAllReadWords = hasCreatedReadWord_;
		readResult.result = CommonVariables.result;
		return readResult;
		}

	protected ReadResultType readWordFromString( boolean isCheckingForGrammarDefinition, boolean isSkippingDoubleQuotes, int startWordPosition, int minimumStringLength, String wordString )
		{
		ReadResultType readResult = new ReadResultType();
		boolean isText = false;
		boolean isWordStartingWithDoubleQuote = false;
		int wordStringLength;
		int wordPosition = startWordPosition;

		if( wordString != null )
			{
			wordStringLength = wordString.length();

			if( wordPosition < wordStringLength )
				{
				// Skip spaces
				while( wordPosition < wordStringLength &&
				Character.isWhitespace( wordString.charAt( wordPosition ) ) )
					wordPosition++;

				if( wordPosition < wordStringLength )
					{
					readResult.startWordPosition = wordPosition;

					if( isSymbol( wordString.charAt( wordPosition ) ) &&

					( !isCheckingForGrammarDefinition ||
					!isGrammarDefinitionSymbol( wordString.charAt( wordPosition ) ) ) )
						{
						wordPosition++;
						readResult.wordLength++;
						}
					else
						{
						if( isSkippingDoubleQuotes &&
						wordString.charAt( wordPosition ) == Constants.SYMBOL_DOUBLE_QUOTE )
							isWordStartingWithDoubleQuote = true;

						while( wordPosition < wordStringLength &&

						( isText ||
						readResult.wordLength < minimumStringLength ||

						( !Character.isWhitespace( wordString.charAt( wordPosition ) ) &&

						( !isSymbol( wordString.charAt( wordPosition ) ) ||

						( isCheckingForGrammarDefinition &&
						isGrammarDefinitionSymbol( wordString.charAt( wordPosition ) ) ) ) ) ) )
							{
							if( isCheckingForGrammarDefinition &&
							isGrammarDefinitionSymbol( wordString.charAt( wordPosition ) ) )
								readResult.hasFoundGrammarDefinition = true;

							if( wordString.charAt( wordPosition ) == Constants.QUERY_STRING_START_CHAR &&

							( wordPosition == 0 ||
							// Skip escaped double quote character
							wordString.charAt( wordPosition - 1 ) != Constants.SYMBOL_BACK_SLASH ) )
								isText = !isText;

							wordPosition++;
							readResult.wordLength++;
							}

						if( isWordStartingWithDoubleQuote &&
						readResult.wordLength > 1 )
							readResult.wordLength--;

						if( isSkippingDoubleQuotes &&
						wordPosition > 1 &&
						readResult.wordLength > 1 &&
						wordString.charAt( wordPosition - 1 ) == Constants.SYMBOL_DOUBLE_QUOTE )
							{
							readResult.wordLength--;
							readResult.startWordPosition++;
							}
						}

					// Skip spaces
					while( wordPosition < wordStringLength &&
					Character.isWhitespace( wordString.charAt( wordPosition ) ) )
						wordPosition++;
					}
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "The given start word position is invalid" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given word string is undefined" );

		readResult.result = CommonVariables.result;
		readResult.nextWordPosition = wordPosition;
		return readResult;
		}

	protected WordResultType addWord( boolean isLanguageWord, boolean isMultipleWord, short previousWordAdjectiveParameter, short previousWordDefiniteArticleParameter, short previousWordIndefiniteArticleParameter, short wordParameter, short wordTypeNr, int wordTypeStringLength, String wordTypeString )
		{
		WordResultType wordResult = new WordResultType();
		WordResultType wordTypeResult;
		boolean isProperName;
		boolean isProperNamePrecededByDefiniteArticle;
		boolean isSingularOrPluralNoun;
		boolean wasPreviousWordAdjective = ( previousWordAdjectiveParameter > Constants.NO_ADJECTIVE_PARAMETER );
		boolean wasPreviousWordDefiniteArticle = ( previousWordDefiniteArticleParameter > Constants.NO_DEFINITE_ARTICLE_PARAMETER );
		boolean wasPreviousWordIndefiniteArticle = ( previousWordIndefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER );
		short adjectiveParameter = Constants.NO_ADJECTIVE_PARAMETER;
		short definiteArticleParameter = Constants.NO_DEFINITE_ARTICLE_PARAMETER;
		short indefiniteArticleParameter = Constants.NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem createdWordItem;

		if( wordTypeString != null )
			{
			if( wordTypeStringLength > 0 &&
			wordTypeString.length() > 0 )
				{
				if( wordTypeNr > Constants.WORD_TYPE_UNDEFINED )
					{
					if( adminItem_.wordList == null )
						{
						// Create list
						if( ( adminItem_.wordList = new WordList( adminItem_ ) ) != null )
							adminItem_.adminListArray[Constants.ADMIN_WORD_LIST] = adminItem_.wordList;
						else
							adminItem_.startErrorInItem( 1, moduleNameString_, "I failed to create a word list" );
						}
					else
						{
						if( ( wordResult = adminItem_.findWordTypeInAllWords( false, wordTypeNr, wordTypeString, null ) ).result != Constants.RESULT_OK )
							adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a word type in all words" );
						}

					if( wordResult.foundWordItem == null ||
					// Skip match with word part of multiple word
					wordResult.foundWordItem.wordParameter() != wordParameter )
						{
						if( ( wordResult = adminItem_.wordList.createWordItem( isLanguageWord, wordParameter ) ).result == Constants.RESULT_OK )
							{
							if( ( createdWordItem = wordResult.createdWordItem ) != null )
								{
								if( CommonVariables.firstWordItem == null )
									// Remember the first word
									CommonVariables.firstWordItem = createdWordItem;

								if( wordParameter > Constants.NO_WORD_PARAMETER )
									// Remember the last predefined word
									CommonVariables.lastPredefinedWordItem = createdWordItem;

								isProperName = ( wordTypeNr == Constants.WORD_TYPE_PROPER_NAME );

								isProperNamePrecededByDefiniteArticle = ( isProperName &&
																		wasPreviousWordDefiniteArticle );

								isSingularOrPluralNoun = adminItem_.isSingularOrPluralNoun( wordTypeNr );

								if( adminItem_.isAdjectiveParameter( wordParameter ) )
									adjectiveParameter = wordParameter;
								else
									{
									if( wasPreviousWordAdjective &&
									isSingularOrPluralNoun )
										adjectiveParameter = previousWordAdjectiveParameter;
									}

								if( adminItem_.isDefiniteArticleParameter( wordParameter ) )
									definiteArticleParameter = wordParameter;
								else
									{
									if( wasPreviousWordDefiniteArticle &&

									( isProperName ||
									isSingularOrPluralNoun ) )
										definiteArticleParameter = previousWordDefiniteArticleParameter;
									}

								if( adminItem_.isIndefiniteArticleParameter( wordParameter ) )
									indefiniteArticleParameter = wordParameter;
								else
									{
									if( wasPreviousWordIndefiniteArticle &&
									isSingularOrPluralNoun )
										indefiniteArticleParameter = previousWordIndefiniteArticleParameter;
									}

								if( ( wordTypeResult = createdWordItem.addWordType( isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordTypeStringLength, wordTypeString ) ).result == Constants.RESULT_OK )
									{
									if( isSingularOrPluralNoun &&
									indefiniteArticleParameter > Constants.NO_INDEFINITE_ARTICLE_PARAMETER &&
									wordTypeResult.createdWordTypeItem != null &&
									!wordTypeResult.createdWordTypeItem.isCorrectIndefiniteArticle( false, indefiniteArticleParameter ) )
										{
										if( Presentation.writeInterfaceText( false, Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_INDEFINITE_ARTICLE_WITH_NOUN_START, wordTypeString, Constants.INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == Constants.RESULT_OK )
											hasFoundDifferentParameter_ = true;
										else
											adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
										}
									}
								else
									adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a word type to a new word" );
								}
							else
								adminItem_.startErrorInItem( 1, moduleNameString_, "The last created word item is undefined" );
							}
						else
							adminItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a word item" );
						}
					}
				else
					adminItem_.startErrorInItem( 1, moduleNameString_, "The given word type number is undefined" );
				}
			else
				adminItem_.startErrorInItem( 1, moduleNameString_, "The given word type string is empty or has no words left anymore" );
			}
		else
			adminItem_.startErrorInItem( 1, moduleNameString_, "The given word type string is undefined" );

		wordResult.result = CommonVariables.result;
		return wordResult;
		}
	};

/*************************************************************************
 *	"How joyful are those who fear the Lord-
 *	all who follow his ways!" (Psalm 128:1)
 *************************************************************************/
