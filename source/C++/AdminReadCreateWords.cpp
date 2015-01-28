/*
 *	Class:			AdminReadCreateWords
 *	Supports class:	AdminItem
 *	Purpose:		To create words of the read sentence
 *	Version:		Thinknowlogy 2014r2b (Laws of Thought)
 *************************************************************************/
/*	Copyright (C) 2009-2015, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
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

	// Private constructible variables

	bool hasCreatedReadWord_;
	bool hasFoundDifferentParameter_;

	unsigned short currentWordOrderNr_;
	unsigned short wordTypeNr_;

	size_t nextTextStringPosition_;

	char exactWordString_[MAX_WORD_LENGTH];
	char lowerCaseWordString_[MAX_WORD_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	bool isSymbol( char character )
		{
		return ( character == SYMBOL_COMMA ||
				character == SYMBOL_COLON ||
				character == SYMBOL_SEMI_COLON ||
				character == SYMBOL_DOUBLE_COLON ||
				character == SYMBOL_EXCLAMATION_MARK ||
				character == SYMBOL_EXCLAMATION_MARK_INVERTED ||
				character == SYMBOL_QUESTION_MARK ||
				character == SYMBOL_QUESTION_MARK_INVERTED ||
				character == SYMBOL_PIPE ||
				character == SYMBOL_AMPERSAND ||
				character == SYMBOL_ASTERISK ||
				character == SYMBOL_PERCENT ||
				character == SYMBOL_DOLLAR ||
				character == SYMBOL_SLASH ||
				character == SYMBOL_BACK_SLASH ||
				character == SYMBOL_QUOTE ||
				// Don't add SYMBOL_DOUBLE_QUOTE to avoid analyzing in text strings
				character == SYMBOL_OPEN_ROUNDED_BRACKET ||
				character == SYMBOL_CLOSE_ROUNDED_BRACKET ||
				character == SYMBOL_OPEN_CURVED_BRACKET ||
				character == SYMBOL_CLOSE_CURVED_BRACKET ||
				character == SYMBOL_OPEN_HOOKED_BRACKET ||
				character == SYMBOL_CLOSE_HOOKED_BRACKET ||
				character == SYMBOL_OPEN_SQUARE_BRACKET ||
				character == SYMBOL_CLOSE_SQUARE_BRACKET );
		}

	bool isGrammarDefinitionSymbol( char character )
		{
		return ( character == QUERY_WORD_TYPE_CHAR ||
				character == QUERY_PARAMETER_CHAR ||
				character == GRAMMAR_WORD_DEFINITION_CHAR );
		}

	ReadResultType createWordStrings( size_t startPosition, char *wordString )
		{
		ReadResultType readResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWordStrings";
		strcpy( exactWordString_, EMPTY_STRING );
		strcpy( lowerCaseWordString_, EMPTY_STRING );

		if( wordString != NULL )
			{
			if( ( readResult = readWordFromString( false, false, startPosition, 0, wordString ) ).result == RESULT_OK )
				{
				if( readResult.wordLength > 0 )
					{
					if( readResult.wordLength < MAX_WORD_LENGTH )
						{
						strncpy( exactWordString_, &wordString[startPosition], readResult.wordLength );
						exactWordString_[readResult.wordLength] = NULL_CHAR;

						strcpy( lowerCaseWordString_, exactWordString_ );

						for( unsigned i = 0; i < readResult.wordLength; i++ )
							lowerCaseWordString_[i] = (char)tolower( exactWordString_[i] );

						lowerCaseWordString_[readResult.wordLength] = NULL_CHAR;
						}
					else
						myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word string is too long" );
					}
				else
					myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word string is empty or has no words left anymore" );
				}
			else
				myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a word from the word string" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word string is undefined" );

		readResult.result = commonVariables_->result;
		return readResult;
		}

	ResultType getWordTypeNr( bool checkProperName, size_t wordTypeStringLength, char *wordTypeString )
		{
		size_t wordPosition = 0;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getWordTypeNr";

		wordTypeNr_ = WORD_TYPE_UNDEFINED;

		if( wordTypeString != NULL )
			{
			if( wordTypeStringLength > 0 )
				{
				if( isalpha( wordTypeString[wordPosition] ) )
					{
					if( wordTypeStringLength == 1 )
						wordTypeNr_ = ( isupper( wordTypeString[wordPosition] ) ? WORD_TYPE_LETTER_CAPITAL : WORD_TYPE_LETTER_SMALL );
					else
						{
						if( checkProperName &&
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
				}
			else
				return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word type string is empty" );
			}
		else
			return myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word type string is undefined" );

		return RESULT_OK;
		}


	protected:
	// Constructor / deconstructor

	AdminReadCreateWords( AdminItem *adminItem, CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasCreatedReadWord_ = false;
		hasFoundDifferentParameter_ = false;

		currentWordOrderNr_ = NO_ORDER_NR;
		wordTypeNr_ = WORD_TYPE_UNDEFINED;

		nextTextStringPosition_ = 0;

		strcpy( exactWordString_, EMPTY_STRING );
		strcpy( lowerCaseWordString_, EMPTY_STRING );

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "AdminReadCreateWords" );

		if( commonVariables_ != NULL )
			{
		if( adminItem_ != NULL )
			{
			if( myWordItem_ == NULL )
				strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given admin is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
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

	ReadResultType createReadWord( unsigned short wordOrderNr, unsigned short wordTypeNr, size_t textStringStartPosition, char *textString, WordItem *readWordItem )
		{
		ReadResultType readResult;
		unsigned short wordParameter = ( wordTypeNr == WORD_TYPE_NOUN_PLURAL || readWordItem == NULL ? NO_WORD_PARAMETER : readWordItem->wordParameter() );
		char *readString = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWord";

		hasCreatedReadWord_ = false;
		nextTextStringPosition_ = 0;

		if( wordTypeNr > WORD_TYPE_UNDEFINED )
			{
			if( textString != NULL ||
			readWordItem != NULL )
				{
				if( textString != NULL )
					{
					if( ( readResult = readWordFromString( false, true, textStringStartPosition, 0, textString ) ).result == RESULT_OK )
						{
						if( readResult.startWordPosition < strlen( textString ) )
							{
							nextTextStringPosition_ = readResult.nextWordPosition;
							readString = &textString[readResult.startWordPosition];
							}
						else
							myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The found start word position is invalid" );
						}
					else
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a word from a text string" );
					}

				if( commonVariables_->result == RESULT_OK )
					{
					if( adminItem_->readList == NULL )
						{
						// Create list
						if( ( adminItem_->readList = new ReadList( commonVariables_, myWordItem_ ) ) != NULL )
							adminItem_->adminListArray[ADMIN_READ_LIST] = adminItem_->readList;
						else
							myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to create a read list" );
						}
					else
						{
						// Find out if already exists
						if( adminItem_->readList->hasFoundReadItem( wordOrderNr, wordParameter, wordTypeNr, readString, readWordItem ) )
							myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given read item already exists" );
						}

					if( ( readResult = adminItem_->readList->createReadItem( wordOrderNr, wordParameter, wordTypeNr, readResult.wordLength, readString, readWordItem ) ).result == RESULT_OK )
						hasCreatedReadWord_ = true;
					else
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create an admin read words item" );
					}
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "Both the given text string and the given read word item are undefined" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word type number is undefined" );

		readResult.result = commonVariables_->result;
		return readResult;
		}

	ReadResultType createReadWords( char *readSentenceString )
		{
		GrammarResultType grammarResult;
		ReadResultType readResult;
		WordResultType wordResult;
		bool isCapitalSingularNoun;
		bool isCapitalVerb;
		bool isExactSingularNoun;
		bool isExactVerb;
		bool isExactWord;
		bool isFirstFind;
		bool isNotPartOfMultipleWord;
		bool isNumeral;
		bool isPartOfMultipleWord;
		bool isPluralNoun;
		bool wasPreviousWordAdjective;
		bool wasPreviousWordArticle;
		bool wasPreviousWordBasicVerb;
		bool wasPreviousWordConjunction;
		bool wasPreviousWordPossessiveDeterminer;
		bool wasPreviousWordPossessivePronoun;
		bool wasPreviousWordPreposition;
		bool wasPreviousWordSymbol;
		bool wasPreviousWordUndefined;
		bool isAdjective = false;
		bool isArticle = false;
		bool isBasicVerb = false;
		bool isConjunction = false;
		bool isPossessiveDeterminer = false;
		bool isPossessivePronoun = false;
		bool isPreposition = false;
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
		unsigned short currentGrammarLanguageNr = commonVariables_->currentGrammarLanguageNr;
		size_t readSentenceStringLength;
		size_t wordStringLength;
		size_t nextWordPosition = 0;
		size_t readPosition = 0;
		WordItem *createdWordItem;
		WordItem *currentGrammarLanguageWordItem;
		WordItem *foundWordItem;
		WordItem *pluralNounWordItem;
		WordItem *singularNounWordItem;
		WordTypeItem *foundWordTypeItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "createReadWords";

		hasCreatedReadWord_ = true;		// Set to pass while loop for the first time
		hasFoundDifferentParameter_ = false;
		currentWordOrderNr_ = NO_ORDER_NR;

		if( readSentenceString != NULL )
			{
			if( ( readSentenceStringLength = strlen( readSentenceString ) ) > 0 )
				{
				do	{
					if( ++currentWordOrderNr_ < MAX_ORDER_NR )
						{
						hasCreatedReadWord_ = false;

						if( readSentenceString[readPosition] == SYMBOL_DOUBLE_QUOTE )
							{
							if( createReadWord( currentWordOrderNr_, WORD_TYPE_TEXT, readPosition, readSentenceString, NULL ).result == RESULT_OK )
								nextWordPosition = nextTextStringPosition_;
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a read word" );
							}
						else
							{
							wasPreviousWordAdjective = isAdjective;
							wasPreviousWordArticle = isArticle;
							wasPreviousWordBasicVerb = isBasicVerb;
							wasPreviousWordConjunction = isConjunction;
							wasPreviousWordPossessiveDeterminer = isPossessiveDeterminer;
							wasPreviousWordPossessivePronoun = isPossessivePronoun;
							wasPreviousWordPreposition = isPreposition;
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
							isExactSingularNoun = false;
							isExactVerb = false;
							isExactWord = false;
							isNumeral = false;
							isPluralNoun = false;
							isPossessiveDeterminer = false;
							isPossessivePronoun = false;
							isPreposition = false;
							isSymbol = false;
							isUndefinedWord = false;

							currentWordAdjectiveParameter = NO_ADJECTIVE_PARAMETER;
							currentWordDefiniteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
							currentWordIndefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;

							createdWordItem = NULL;
							pluralNounWordItem = NULL;
							singularNounWordItem = NULL;

							if( ( readResult = createWordStrings( readPosition, readSentenceString ) ).result == RESULT_OK )
								{
								isNotPartOfMultipleWord = false;
								isPartOfMultipleWord = false;

								nextWordPosition = readResult.nextWordPosition;
								wordStringLength = readResult.wordLength;
								foundWordItem = NULL;

								// Step 1: Find exact word types in all words
								do	{
									if( ( wordResult = myWordItem_->findWordTypeInAllWords( false, WORD_TYPE_UNDEFINED, exactWordString_, foundWordItem ) ).result == RESULT_OK )
										{
										// Found exact word
										if( ( foundWordItem = wordResult.foundWordItem ) != NULL &&
										( foundWordTypeItem = wordResult.foundWordTypeItem ) != NULL )
											{
											if( foundWordItem->isMultipleWord() )
												isPartOfMultipleWord = true;
											else
												isNotPartOfMultipleWord = true;

											if( createReadWord( currentWordOrderNr_, foundWordTypeItem->wordTypeNr(), 0, NULL, foundWordItem ).result == RESULT_OK )
												{
												isExactWord = true;

												if( foundWordTypeItem->isAdjective() )
													{
													isAdjective = true;

													if( wordResult.foundWordItem != NULL &&
													// Adjective 'no' can be used as article
													wordResult.foundWordItem->isAdjectiveNo() )
														isArticle = true;

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

												if( foundWordItem->isBasicVerb() )
													isBasicVerb = true;

												if( foundWordTypeItem->isConjunction() )
													isConjunction = true;

												if( foundWordTypeItem->isVerb() )
													isExactVerb = true;

												if( foundWordTypeItem->isNumeral() )
													isNumeral = true;

												if( foundWordTypeItem->isSingularNoun() )
													{
													isExactSingularNoun = true;

													if( ( wordResult = foundWordTypeItem->setParameter( previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter ) ).result == RESULT_OK )
														{
														if( wordResult.hasFoundDifferentParameter )
															hasFoundDifferentParameter_ = true;
														}
													else
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to set a parameter of a singular noun" );
													}

												if( foundWordTypeItem->isPluralNoun() )
													isPluralNoun = true;

												if( foundWordTypeItem->isPossessiveDeterminer() )
													isPossessiveDeterminer = true;

												if( foundWordTypeItem->isPossessivePronoun() )
													isPossessivePronoun = true;

												if( foundWordTypeItem->isPreposition() )
													isPreposition = true;

												if( foundWordTypeItem->isSymbol() )
													isSymbol = true;
												}
											else
												myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create another read word" );
											}
										}
									else
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find an exact word type in all words" );
									}
								while( commonVariables_->result == RESULT_OK &&
								// Allow multiple finds
								foundWordItem != NULL );

								if( commonVariables_->result == RESULT_OK &&
								isExactSingularNoun &&
								isExactWord &&
								isNotPartOfMultipleWord &&
								isPartOfMultipleWord &&
								adminItem_->readList != NULL )
									{
									if( adminItem_->readList->deleteInvalidPartOfMultipleWordReadItems( currentWordOrderNr_, &readSentenceString[readPosition] ) != RESULT_OK )
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to delete the read items with part of multiple words" );
									}

								if( commonVariables_->result == RESULT_OK &&
								isFirstWord &&
								isupper( readSentenceString[readPosition] ) )
									{
									if( getWordTypeNr( false, wordStringLength, lowerCaseWordString_ ) == RESULT_OK )
										{
										foundWordItem = NULL;

										if( wordTypeNr_ == WORD_TYPE_UNDEFINED )
											isUndefinedWord = true;

										// Step 2: Find word type with lowercase first letter in all words
										do	{
											if( ( wordResult = myWordItem_->findWordTypeInAllWords( true, WORD_TYPE_UNDEFINED, lowerCaseWordString_, foundWordItem ) ).result == RESULT_OK )
												{
												// Found word type with lowercase first letter
												if( ( foundWordItem = wordResult.foundWordItem ) != NULL &&
												( foundWordTypeItem = wordResult.foundWordTypeItem ) != NULL )
													{
													foundWordTypeNr = foundWordTypeItem->wordTypeNr();

													if( foundWordItem->isMultipleWord() )
														isPartOfMultipleWord = true;
													else
														isNotPartOfMultipleWord = true;

													if( foundWordTypeNr == wordTypeNr_ &&
													foundWordTypeItem->wordTypeLanguageNr() != currentGrammarLanguageNr )
														{
														// Create same word type for different language
														if( foundWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr_, wordStringLength, lowerCaseWordString_ ).result != RESULT_OK )
															myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a word type with lowercase first letter" );
														}

													if( createReadWord( currentWordOrderNr_, foundWordTypeNr, 0, NULL, foundWordItem ).result == RESULT_OK )
														{
														if( foundWordTypeItem->isAdjective() )
															{
															isAdjective = true;

															if( wordResult.foundWordItem != NULL &&
															// Adjective 'no' can be used as article
															wordResult.foundWordItem->isAdjectiveNo() )
																isArticle = true;

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

														if( foundWordTypeItem->isPluralNoun() )
															isPluralNoun = true;

														if( foundWordTypeItem->isPossessiveDeterminer() )
															isPossessiveDeterminer = true;

														if( foundWordTypeItem->isPossessivePronoun() )
															isPossessivePronoun = true;
														}
													else
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a read word with a word type with difference case of the first letter" );
													}
												}
											else
												myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a word type with difference case of the first letter in all words" );
											}
										while( commonVariables_->result == RESULT_OK &&
										// Allow multiple finds
										foundWordItem != NULL );

										if( commonVariables_->result == RESULT_OK &&
										!isUndefinedWord &&
										wordStringLength == 1 )
											{
											// Step 3: Typically for English: Find or create lowercase letter 'a' as first letter of a sentence.
											if( ( wordResult = myWordItem_->findWordTypeInAllWords( false, wordTypeNr_, lowerCaseWordString_, NULL ) ).result == RESULT_OK )
												{
												if( wordResult.foundWordItem == NULL )
													{
													if( ( wordResult = addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, wordTypeNr_, wordStringLength, lowerCaseWordString_ ) ).result == RESULT_OK )
														{
														if( ( createdWordItem = wordResult.createdWordItem ) != NULL )
															{
															if( createReadWord( currentWordOrderNr_, wordTypeNr_, 0, NULL, createdWordItem ).result != RESULT_OK )
																myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a read word with lowercase letter" );
															}
														else
															myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The created word with lowercase letter is undefined" );
														}
													else
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add word with lowercase letter" );
													}
												}
											else
												myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a lowercase letter" );
											}
										}
									else
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get the word type number of a lowercase word" );
									}

								if( commonVariables_->result == RESULT_OK &&

								( !isExactWord ||

								( wasPreviousWordSymbol &&
								wordStringLength == 1 &&

								( isConjunction ||				// Typically for Spanish: 'o' and 'y' are letters as well as conjunctions
								isPossessivePronoun ) ) ) )		// Typically for Dutch: 'u' is a letter as well as a possessive pronoun
									{
									isFirstFind = true;
									foundWordItem = NULL;

									// Step 4: Find exact word types in all words
									do	{
										if( ( wordResult = myWordItem_->findWordTypeInAllWords( true, WORD_TYPE_UNDEFINED, exactWordString_, foundWordItem ) ).result == RESULT_OK )
											{
											createdWordItem = NULL;
											foundWordItem = wordResult.foundWordItem;
											foundWordTypeItem = wordResult.foundWordTypeItem;

											if( isFirstFind ||

											( foundWordItem != NULL &&
											foundWordTypeItem != NULL ) )	// Skip if later runs have no result
												{
												if( getWordTypeNr( true, wordStringLength, exactWordString_ ) == RESULT_OK )
													{
													if( wordTypeNr_ == WORD_TYPE_UNDEFINED )
														isUndefinedWord = true;
													else
														{
														if( foundWordItem == NULL )
															{
															// Small letters, capital letters, numerals and proper names
															if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, wordTypeNr_, wordStringLength, exactWordString_ ) ).result == RESULT_OK )
																{
																if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
																	myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't create an exact word" );
																}
															else
																myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add an exact word" );
															}
														else
															{
															if( foundWordTypeItem != NULL &&
															foundWordTypeItem->wordTypeNr() == wordTypeNr_ &&
															foundWordTypeItem->wordTypeLanguageNr() != currentGrammarLanguageNr )
																{
																// Create same word type for different language
																if( foundWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr_, wordStringLength, exactWordString_ ).result == RESULT_OK )
																	createdWordItem = foundWordItem;
																else
																	myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add an exact word type" );
																}
															}

														if( commonVariables_->result == RESULT_OK &&
														createdWordItem != NULL )
															{
															if( ( readResult = createReadWord( currentWordOrderNr_, wordTypeNr_, 0, NULL, createdWordItem ) ).result == RESULT_OK )
																isFirstFind = false;
															else
																myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create an exact read word" );
															}
														}
													}
												else
													myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to get the word type number of an exact word" );
												}
											}
										else
											myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find an exact word" );
										}
									while( commonVariables_->result == RESULT_OK &&
									// Allow multiple finds
									foundWordItem != NULL );
									}

								// Create a noun
								if( commonVariables_->result == RESULT_OK &&
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
								wasPreviousWordSymbol ||

								( !isExactVerb &&
								wasPreviousWordConjunction ) ) &&

								( isFirstWord ||
								!isupper( readSentenceString[readPosition] ) ) )
									{
									if( ( currentGrammarLanguageWordItem = commonVariables_->currentGrammarLanguageWordItem ) != NULL )
										{
										if( ( grammarResult = currentGrammarLanguageWordItem->checkOnWordEnding( WORD_PLURAL_NOUN_ENDING, wordStringLength, exactWordString_ ) ).result == RESULT_OK )
											{
											if( isFirstWord ||
											wasPreviousWordAdjective ||
											wasPreviousWordArticle ||
											wasPreviousWordBasicVerb ||
											wasPreviousWordConjunction ||
											wasPreviousWordPossessiveDeterminer ||
											wasPreviousWordPossessivePronoun ||
											wasPreviousWordPreposition ||
											wasPreviousWordSymbol ||
											grammarResult.hasFoundWordEnding )
												{
												if( grammarResult.hasFoundWordEnding )
													{
													if( ( wordResult = myWordItem_->findWordTypeInAllWords( false, WORD_TYPE_NOUN_SINGULAR, grammarResult.singularNounWordString, NULL ) ).result == RESULT_OK )
														{
														if( ( foundWordItem = wordResult.foundWordItem ) == NULL )
															{
															if( ( wordResult = addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ) ).result == RESULT_OK )
																{
																if( ( pluralNounWordItem = wordResult.createdWordItem ) != NULL )
																	{
																	if( pluralNounWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, WORD_TYPE_NOUN_SINGULAR, grammarResult.singularNounWordStringLength, grammarResult.singularNounWordString ).result == RESULT_OK )
																		{
																		if( wordStringLength == grammarResult.singularNounWordStringLength &&
																		strcmp( exactWordString_, grammarResult.singularNounWordString ) == 0 )
																			singularNounWordItem = pluralNounWordItem;
																		}
																	else
																		myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a singular noun word type item for plural noun word \"", pluralNounWordItem->anyWordTypeString(), "\"" );
																	}
																else
																	myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The created word item is undefined" );
																}
															else
																myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a singular noun word" );
															}
														else	// Found singular noun
															{
															if( !isPluralNoun )
																{
																if( foundWordItem->addWordType( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, WORD_TYPE_NOUN_PLURAL, wordStringLength, exactWordString_ ).result == RESULT_OK )
																	pluralNounWordItem = foundWordItem;
																else
																	myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a plural noun word type item for word \"", foundWordItem->anyWordTypeString(), "\"" );
																}
															}
														}
													else
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find if a singular noun word already exists" );
													}
												else
													{
													if( !isCapitalSingularNoun &&
													!isExactSingularNoun )
														{
														if( ( wordResult = addWord( false, false, previousWordAdjectiveParameter, previousWordDefiniteArticleParameter, previousWordIndefiniteArticleParameter, NO_WORD_PARAMETER, WORD_TYPE_NOUN_SINGULAR, wordStringLength, exactWordString_ ) ).result == RESULT_OK )
															singularNounWordItem = wordResult.createdWordItem;
														else
															myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a singular noun word" );
														}
													}

												if( commonVariables_->result == RESULT_OK &&
												singularNounWordItem != NULL )
													{
													if( createReadWord( currentWordOrderNr_, WORD_TYPE_NOUN_SINGULAR, 0, NULL, singularNounWordItem ).result != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a singular noun read word" );
													}

												if( commonVariables_->result == RESULT_OK &&
												grammarResult.hasFoundWordEnding &&
												pluralNounWordItem != NULL )
													{
													if( createReadWord( currentWordOrderNr_, WORD_TYPE_NOUN_PLURAL, 0, NULL, pluralNounWordItem ).result != RESULT_OK )
														myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a plural noun read word" );
													}
												}

											// Create an adjective
											if( commonVariables_->result == RESULT_OK &&
											!wasPreviousWordUndefined &&

											( !isExactWord ||

											( wasPreviousWordBasicVerb &&
											isExactSingularNoun ) ) &&

											( wasPreviousWordConjunction ||
											wasPreviousWordSymbol ||
											pluralNounWordItem == NULL ) )
												{
												if( ( wordResult = addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_ADJECTIVE, wordStringLength, lowerCaseWordString_ ) ).result == RESULT_OK )
													{
													if( ( createdWordItem = wordResult.createdWordItem ) != NULL )
														{
														if( createReadWord( currentWordOrderNr_, WORD_TYPE_ADJECTIVE, 0, NULL, createdWordItem ).result == RESULT_OK )
															isAdjective = true;
														else
															myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create an adjective read word" );
														}
													else
														myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The last created adjective word is undefined" );
													}
												else
													myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add an adjective word" );
												}
											}
										else
											myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check the plural noun ending" );
										}
									else
										myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The current grammar language word item is undefined" );
									}
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create the word strings" );
							}

						isFirstWord = false;
						readPosition = nextWordPosition;
						}
					else
						myWordItem_->startSystemErrorInItem( functionNameString, moduleNameString_, "Word order number overflow! I can't accept more words" );
					}
				while( commonVariables_->result == RESULT_OK &&
				hasCreatedReadWord_ &&
				readPosition < readSentenceStringLength );
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given read sentence string is empty" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given read sentence string is undefined" );

		readResult.hasCreatedAllReadWords = hasCreatedReadWord_;
		readResult.result = commonVariables_->result;
		return readResult;
		}

	ReadResultType readWordFromString( bool checkForGrammarDefinition, bool skipDoubleQuotes, size_t startWordPosition, size_t minimumStringLength, char *wordString )
		{
		ReadResultType readResult;
		bool isText = false;
		bool wordStartedWithDoubleQuote = false;
		size_t wordStringLength;
		size_t wordPosition = startWordPosition;
		char functionNameString[FUNCTION_NAME_LENGTH] = "readWordFromString";

		if( wordString != NULL )
			{
			wordStringLength = strlen( wordString );

			if( wordPosition < wordStringLength )
				{
				// Skip spaces
				while( wordPosition < wordStringLength &&
				isspace( wordString[wordPosition] ) )
					wordPosition++;

				if( wordPosition < wordStringLength )
					{
					readResult.startWordPosition = wordPosition;

					if( isSymbol( wordString[wordPosition] ) &&

					( !checkForGrammarDefinition ||
					!isGrammarDefinitionSymbol( wordString[wordPosition] ) ) )
						{
						wordPosition++;
						readResult.wordLength++;
						}
					else
						{
						if( skipDoubleQuotes &&
						wordString[wordPosition] == SYMBOL_DOUBLE_QUOTE )
							wordStartedWithDoubleQuote = true;

						while( wordPosition < wordStringLength &&

						( isText ||
						readResult.wordLength < minimumStringLength ||

						( !isspace( wordString[wordPosition] ) &&

						( !isSymbol( wordString[wordPosition] ) ||

						( checkForGrammarDefinition &&
						isGrammarDefinitionSymbol( wordString[wordPosition] ) ) ) ) ) )
							{
							if( checkForGrammarDefinition &&
							isGrammarDefinitionSymbol( wordString[wordPosition] ) )
								readResult.hasFoundGrammarDefinition = true;

							if( wordString[wordPosition] == QUERY_STRING_START_CHAR &&

							( wordPosition == 0 ||
							wordString[wordPosition - 1] != SYMBOL_BACK_SLASH ) )	// Skip escaped double quote character
								isText = !isText;

							wordPosition++;
							readResult.wordLength++;
							}

						if( wordStartedWithDoubleQuote &&
						readResult.wordLength > 1 )
							readResult.wordLength--;

						if( skipDoubleQuotes &&
						wordPosition > 1 &&
						readResult.wordLength > 1 &&
						wordString[wordPosition - 1] == SYMBOL_DOUBLE_QUOTE )
							{
							readResult.wordLength--;
							readResult.startWordPosition++;
							}
						}

					// Skip spaces
					while( wordPosition < wordStringLength &&
					isspace( wordString[wordPosition] ) )
						wordPosition++;
					}
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given start word position is invalid" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word string is undefined" );

		readResult.result = commonVariables_->result;
		readResult.nextWordPosition = wordPosition;
		return readResult;
		}

	WordResultType addWord( bool isLanguageWord, bool isMultipleWord, unsigned short previousWordAdjectiveParameter, unsigned short previousWordDefiniteArticleParameter, unsigned short previousWordIndefiniteArticleParameter, unsigned short wordParameter, unsigned short wordTypeNr, size_t wordTypeStringLength, char *wordTypeString )
		{
		WordResultType wordResult;
		WordResultType wordTypeResult;
		bool isProperName;
		bool isProperNamePrecededByDefiniteArticle;
		bool isSingularOrPluralNoun;
		bool wasPreviousWordAdjective = ( previousWordAdjectiveParameter > NO_ADJECTIVE_PARAMETER );
		bool wasPreviousWordDefiniteArticle = ( previousWordDefiniteArticleParameter > NO_DEFINITE_ARTICLE_PARAMETER );
		bool wasPreviousWordIndefiniteArticle = ( previousWordIndefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER );
		unsigned short adjectiveParameter = NO_ADJECTIVE_PARAMETER;
		unsigned short definiteArticleParameter = NO_DEFINITE_ARTICLE_PARAMETER;
		unsigned short indefiniteArticleParameter = NO_INDEFINITE_ARTICLE_PARAMETER;
		WordItem *createdWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWord";

		if( wordTypeString != NULL )
			{
			if( wordTypeStringLength > 0 &&
			strlen( wordTypeString ) > 0 )
				{
				if( wordTypeNr > WORD_TYPE_UNDEFINED )
					{
					if( adminItem_->wordList == NULL )
						{
						// Create list
						if( ( adminItem_->wordList = new WordList( commonVariables_, myWordItem_ ) ) != NULL )
							adminItem_->adminListArray[ADMIN_WORD_LIST] = adminItem_->wordList;
						else
							myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to create a word list" );
						}

					if( ( wordResult = myWordItem_->findWordTypeInAllWords( false, wordTypeNr, wordTypeString, NULL ) ).result == RESULT_OK )
						{
						if( wordResult.foundWordItem == NULL ||
						// Skip match with word part of multiple word
						wordResult.foundWordItem->wordParameter() != wordParameter )
							{
							if( ( wordResult = adminItem_->wordList->createWordItem( isLanguageWord, wordParameter ) ).result == RESULT_OK )
								{
								if( ( createdWordItem = wordResult.createdWordItem ) != NULL )
									{
									if( commonVariables_->firstWordItem == NULL )
										// Remember this word as the first word (linked list in reversed order)
										commonVariables_->firstWordItem = createdWordItem;

									isProperName = ( wordTypeNr == WORD_TYPE_PROPER_NAME );

									isProperNamePrecededByDefiniteArticle = ( isProperName &&
																			wasPreviousWordDefiniteArticle );

									isSingularOrPluralNoun = adminItem_->isSingularOrPluralNoun( wordTypeNr );

									if( adminItem_->isAdjectiveParameter( wordParameter ) )
										adjectiveParameter = wordParameter;
									else
										{
										if( wasPreviousWordAdjective &&
										isSingularOrPluralNoun )
											adjectiveParameter = previousWordAdjectiveParameter;
										}

									if( adminItem_->isDefiniteArticleParameter( wordParameter ) )
										definiteArticleParameter = wordParameter;
									else
										{
										if( wasPreviousWordDefiniteArticle &&

										( isProperName ||
										isSingularOrPluralNoun ) )
											definiteArticleParameter = previousWordDefiniteArticleParameter;
										}

									if( adminItem_->isIndefiniteArticleParameter( wordParameter ) )
										indefiniteArticleParameter = wordParameter;
									else
										{
										if( wasPreviousWordIndefiniteArticle &&
										isSingularOrPluralNoun )
											indefiniteArticleParameter = previousWordIndefiniteArticleParameter;
										}

									if( ( wordTypeResult = createdWordItem->addWordType( isMultipleWord, isProperNamePrecededByDefiniteArticle, adjectiveParameter, definiteArticleParameter, indefiniteArticleParameter, wordTypeNr, wordTypeStringLength, wordTypeString ) ).result == RESULT_OK )
										{
										if( isSingularOrPluralNoun &&
										indefiniteArticleParameter > NO_INDEFINITE_ARTICLE_PARAMETER &&
										wordTypeResult.createdWordTypeItem != NULL &&
										!wordTypeResult.createdWordTypeItem->isCorrectIndefiniteArticle( false, indefiniteArticleParameter ) )
											{
											if( commonVariables_->presentation->writeInterfaceText( false, PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_INDEFINITE_ARTICLE_WITH_NOUN_START, wordTypeString, INTERFACE_SENTENCE_NOTIFICATION_USED_INVALID_ADJECTIVE_OR_ARTICLE_WITH_NOUN_END ) == RESULT_OK )
												hasFoundDifferentParameter_ = true;
											else
												myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
											}
										}
									else
										myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a word type to a new word" );
									}
								else
									myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The last created word item is undefined" );
								}
							else
								myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a word item" );
							}
						}
					else
						myWordItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a word type in all words" );
					}
				else
					myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word type number is undefined" );
				}
			else
				myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word type string is empty or has no words left anymore" );
			}
		else
			myWordItem_->startErrorInItem( functionNameString, moduleNameString_, "The given word type string is undefined" );

		wordResult.result = commonVariables_->result;
		return wordResult;
		}
	};

/*************************************************************************
 *	"How joyful are those who fear the Lord-
 *	all who follow his ways!" (Psalm 128:1)
 *************************************************************************/
