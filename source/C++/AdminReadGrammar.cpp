/*
 *	Class:			AdminReadGrammar
 *	Supports class:	AdminItem
 *	Purpose:		To read and process grammar and interface files
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

#include "AdminItem.h"
#include "GrammarItem.cpp"
#include "WordTypeItem.cpp"

class AdminReadGrammar
	{
	friend class AdminItem;

	// Private constructible variables

	WordItem *multipleWordItem_;
	WordItem *predefinedAdjectiveBusyWordItem_;
	WordItem *predefinedAdjectiveDoneWordItem_;
	WordItem *predefinedAdjectiveInvertedWordItem_;
	WordItem *predefinedNounPasswordWordItem_;
	WordItem *predefinedNounSolveLevelWordItem_;
	WordItem *predefinedNounSolveMethodWordItem_;
	WordItem *predefinedNounSolveStrategyWordItem_;
	WordItem *predefinedNounStartupLanguageWordItem_;
	WordItem *predefinedVerbLoginWordItem_;

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	void clearPredefinedMultipleWordNrInAllWords()
		{
		WordItem *currentWordItem;

		if( ( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	currentWordItem->predefinedMultipleWordNr = 0;
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}
		}

	ResultType addPredefinedMultipleWord( unsigned short nMultipleWords, unsigned short wordParameter, unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		WordResultType wordResult;
		unsigned short predefinedMultipleWordNr = 0;
		WordItem *foundMultipleWordItem;
		WordTypeItem *foundWordTypeItem;
		char multipleWordString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addPredefinedMultipleWord";

		multipleWordItem_ = multipleWordItem;

		if( nMultipleWords > 0 )
			{
			while( predefinedMultipleWordNr < nMultipleWords &&
			( foundMultipleWordItem = predefinedMultipleWordItem( ++predefinedMultipleWordNr ) ) != NULL )
				{
				if( ( foundWordTypeItem = foundMultipleWordItem->activeWordTypeItem( false, wordTypeNr ) ) != NULL )
					{
					if( predefinedMultipleWordNr > 1 )
						strcat( multipleWordString, SPACE_STRING );

					strcat( multipleWordString, foundWordTypeItem->itemString() );
					}
				}

			if( multipleWordItem == NULL ||
			multipleWordItem->creationSentenceNr() == commonVariables_->currentSentenceNr )
				{
				if( ( wordResult = adminItem_->addWord( false, true, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordParameter, wordTypeNr, strlen( multipleWordString ), multipleWordString ) ).result == RESULT_OK )
					multipleWordItem_ = wordResult.createdWordItem;
				else
					return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a predefined grammar word" );
				}
			else
				{
				if( multipleWordItem->addWordType( true, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, strlen( multipleWordString ), multipleWordString ).result != RESULT_OK )
					return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "The given multiple word item is undefined" );
				}

			if( multipleWordItem_ != NULL )
				{
				predefinedMultipleWordNr = 0;

				while( predefinedMultipleWordNr < nMultipleWords &&
				( foundMultipleWordItem = predefinedMultipleWordItem( ++predefinedMultipleWordNr ) ) != NULL )
					{
					if( foundMultipleWordItem->addMultipleWord( nMultipleWords, wordTypeNr, multipleWordItem_ ) != RESULT_OK )
						return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a multiple word" );
					}
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The multiple word item is undefined" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given number of multiple words is undefined" );

		return RESULT_OK;
		}

	ResultType assignPredefinedWord( unsigned short grammarParameter, WordItem *createdWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "assignPredefinedWord";

		if( createdWordItem != NULL )
			{
			switch( grammarParameter )
				{
				case WORD_PARAMETER_ADJECTIVE_BUSY:
					if( predefinedAdjectiveBusyWordItem_ == NULL )
						predefinedAdjectiveBusyWordItem_ = createdWordItem;
					else
						{
						if( predefinedAdjectiveBusyWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined busy adjective word" );
						}

					break;

				case WORD_PARAMETER_ADJECTIVE_DONE:
					if( predefinedAdjectiveDoneWordItem_ == NULL )
						predefinedAdjectiveDoneWordItem_ = createdWordItem;
					else
						{
						if( predefinedAdjectiveDoneWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined done adjective word" );
						}

					break;

				case WORD_PARAMETER_ADJECTIVE_INVERTED:
					if( predefinedAdjectiveInvertedWordItem_ == NULL )
						predefinedAdjectiveInvertedWordItem_ = createdWordItem;
					else
						{
						if( predefinedAdjectiveInvertedWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined inverted adjective word" );
						}

					break;

				case WORD_PARAMETER_NOUN_LANGUAGE:
					if( commonVariables_->predefinedNounLanguageWordItem == NULL )
						{
						commonVariables_->predefinedNounLanguageWordItem = createdWordItem;

						// During the creation of the first language,
						// the predefined noun language word wasn't defined yet
						// So, create first language specification afterwards
						if( adminItem_->createLanguageSpecification( createdWordItem ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to create a language specification" );
						}

					break;

				case WORD_PARAMETER_NOUN_PASSWORD:
					if( predefinedNounPasswordWordItem_ == NULL )
						{
						predefinedNounPasswordWordItem_ = createdWordItem;

						if( adminItem_->authorizeWord( predefinedNounPasswordWordItem_ ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to authorize the predefined password noun word" );
						}
					else
						{
						if( predefinedNounPasswordWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined password noun word" );
						}

					break;

				case WORD_PARAMETER_NOUN_SOLVE_LEVEL:
					if( predefinedNounSolveLevelWordItem_ == NULL )
						predefinedNounSolveLevelWordItem_ = createdWordItem;
					else
						{
						if( predefinedNounSolveLevelWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined solve level noun word" );
						}

					break;

				case WORD_PARAMETER_NOUN_SOLVE_METHOD:
					if( predefinedNounSolveMethodWordItem_ == NULL )
						predefinedNounSolveMethodWordItem_ = createdWordItem;
					else
						{
						if( predefinedNounSolveMethodWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined solve method noun word" );
						}

					break;

				case WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
					if( predefinedNounSolveStrategyWordItem_ == NULL )
						predefinedNounSolveStrategyWordItem_ = createdWordItem;
					else
						{
						if( predefinedNounSolveStrategyWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined solve strategy noun word" );
						}

					break;

				case WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
					if( predefinedNounStartupLanguageWordItem_ == NULL )
						predefinedNounStartupLanguageWordItem_ = createdWordItem;
					else
						{
						if( predefinedNounStartupLanguageWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined startup language word" );
						}

					break;

				case WORD_PARAMETER_NOUN_USER:
					if( commonVariables_->predefinedNounUserWordItem == NULL )
						{
						commonVariables_->predefinedNounUserWordItem = createdWordItem;

						if( adminItem_->authorizeWord( commonVariables_->predefinedNounUserWordItem ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to authorize the predefined user noun word" );
						}
					else
						{
						if( commonVariables_->predefinedNounUserWordItem != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined user noun word" );
						}

					break;

				case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
					if( predefinedVerbLoginWordItem_ == NULL )
						{
						predefinedVerbLoginWordItem_ = createdWordItem;

						if( adminItem_->authorizeWord( predefinedVerbLoginWordItem_ ) != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to authorize the predefined login verb word" );
						}
					else
						{
						if( predefinedVerbLoginWordItem_ != createdWordItem )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found a different predefined login verb word" );
						}

					break;
				}
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given created word item" );

		return RESULT_OK;
		}

	WordItem *predefinedMultipleWordItem( unsigned short predefinedMultipleWordNr )
		{
		WordItem *currentWordItem;

		if( predefinedMultipleWordNr > 0 &&
		( currentWordItem = commonVariables_->firstWordItem ) != NULL )
			{
			// Do for all words
			do	{
				if( currentWordItem->predefinedMultipleWordNr == predefinedMultipleWordNr )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NULL;
		}

	protected:
	// Constructor / deconstructor

	AdminReadGrammar( AdminItem *adminItem, CommonVariables *commonVariables )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		multipleWordItem_ = NULL;
		predefinedAdjectiveBusyWordItem_ = NULL;
		predefinedAdjectiveDoneWordItem_ = NULL;
		predefinedAdjectiveInvertedWordItem_ = NULL;

		predefinedNounPasswordWordItem_ = NULL;
		predefinedNounSolveLevelWordItem_ = NULL;
		predefinedNounSolveMethodWordItem_ = NULL;
		predefinedNounSolveStrategyWordItem_ = NULL;
		predefinedNounStartupLanguageWordItem_ = NULL;
		predefinedVerbLoginWordItem_ = NULL;

		adminItem_ = adminItem;
		commonVariables_ = commonVariables;
		strcpy( moduleNameString_, "AdminReadGrammar" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( adminItem_ == NULL )
			strcpy( errorString, "The given admin is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( adminItem_ != NULL )
				adminItem_->startSystemErrorInItem( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType addGrammar( char *grammarString )
		{
		GrammarResultType grammarResult;
		ReadResultType readResult;
		WordResultType wordResult;
		bool hasFoundWordDefinitionInfo;
		bool hasCreatedInterface = false;
		bool hasFoundChoiceAlternatives = false;
		bool hasFoundOnlyOptions = true;
		bool hasFoundParameter = false;
		bool hasFoundPipe = false;
		bool hasFoundWordTypeNr = false;
		bool hasGrammarWords = false;
		bool isChoice = false;
		bool isChoiceCheck = false;
		bool isChoiceStart = false;
		bool isEndOfLine = false;
		bool isLastPartOfMultipleWord = false;
		bool isMultipleWord = false;
		bool isNewStart = true;
		bool isOption = false;
		bool isOptionCheck = false;
		bool isOptionStart = false;
		bool isSkipOptionForWriting = false;
		unsigned short grammarParameter = NO_GRAMMAR_PARAMETER;
		unsigned short predefinedMultipleWordNr = 0;
		unsigned short wordTypeNr = WORD_TYPE_UNDEFINED;
		unsigned int firstCreationItemNr = NO_ITEM_NR;
		size_t endPosition;
		size_t tempStringLength;
		size_t grammarStringLength;
		size_t grammarPosition = 0;
		GrammarItem *foundGrammarItem;
		GrammarItem *grammarDefinitionItem = NULL;
		WordItem *foundWordItem;
		WordItem *createdWordItem;
		WordItem *currentLanguageWordItem = commonVariables_->currentLanguageWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "addGrammar";

		if( grammarString != NULL )
			{
			if( ( grammarStringLength = strlen( grammarString ) ) > 0 )
				{
				clearPredefinedMultipleWordNrInAllWords();

				do	{
					if( ( readResult = adminItem_->readWordFromString( true, false, grammarPosition, 0, grammarString ) ).result == RESULT_OK )
						{
						hasFoundWordDefinitionInfo = false;

						switch( grammarString[grammarPosition] )
							{
							case QUERY_WORD_TYPE_CHAR:
								if( !hasFoundWordTypeNr )
									{
									while( grammarPosition + 1 < grammarStringLength &&
									isdigit( grammarString[grammarPosition + 1] ) )
										{
										hasFoundWordTypeNr = true;
										wordTypeNr = ( wordTypeNr * 10 + grammarString[++grammarPosition] - '0' );
										}

									if( !hasFoundWordTypeNr )
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to get the word type number from a grammar definition line" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found more than one word type parameters defined in a grammar definition line" );

								break;

							case QUERY_PARAMETER_CHAR:
								if( !hasFoundParameter )
									{
									// Get paramater from string
									while( grammarPosition + 1 < grammarStringLength &&
									isdigit( grammarString[grammarPosition + 1] ) )
										{
										hasFoundParameter = true;
										grammarParameter = ( grammarParameter * 10 + grammarString[++grammarPosition] - '0' );
										}

									if( !hasFoundParameter )
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to get the grammar parameter from a grammar definition line" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found more than one values parameters defined in a grammar definition line" );

								break;

							case GRAMMAR_WORD_DEFINITION_CHAR:
								if( currentLanguageWordItem != NULL )
									{
									if( !hasCreatedInterface )
										{
										// Add grammar word or grammar definition word
										if( firstCreationItemNr == NO_ITEM_NR )
											{
											if( ( readResult = adminItem_->readWordFromString( false, false, ++grammarPosition, 0, grammarString ) ).result == RESULT_OK )
												{
												if( ( grammarResult = currentLanguageWordItem->findGrammar( ( grammarParameter >= GRAMMAR_SENTENCE ), grammarParameter, readResult.wordLength, &grammarString[grammarPosition] ) ).result == RESULT_OK )
													{
													hasFoundWordDefinitionInfo = true;

													if( !hasFoundWordTypeNr ||
													grammarParameter >= GRAMMAR_SENTENCE ||
													grammarResult.foundGrammarItem == NULL )
														{
														if( ( grammarResult = currentLanguageWordItem->createGrammarItem( true, ( hasFoundParameter && grammarParameter < GRAMMAR_SENTENCE ), false, false, false, wordTypeNr, grammarParameter, readResult.wordLength, &grammarString[grammarPosition], NULL ) ).result == RESULT_OK )
															{
															firstCreationItemNr = commonVariables_->currentItemNr;
															grammarDefinitionItem = grammarResult.createdGrammarItem;
															}
														else
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a grammar definition word item" );
														}
													else
														{
														if( commonVariables_->presentation->writeInterfaceText( PRESENTATION_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_START, grammarResult.foundGrammarItem->grammarParameter(), INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_MIDDLE, currentLanguageWordItem->anyWordTypeString(), INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_END ) != RESULT_OK )
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to write an interface notification" );
														}
													}
												else
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a grammar definition item" );
												}
											else
												return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a word definition from the grammar string" );
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar definition word must be the first word in the grammar definition" );
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );

								break;

							case GRAMMAR_OPTION_READ_NOT_WRITE_START:
								isSkipOptionForWriting = true;

								// Don't insert a break statement here

							case GRAMMAR_OPTION_START:
								if( !hasFoundWordTypeNr ||
								grammarParameter >= GRAMMAR_SENTENCE )
									{
									if( commonVariables_->currentItemNr > NO_ITEM_NR )
										{
										if( !isOption )
											{
											if( !hasFoundPipe )
												{
												isOption = true;
												isNewStart = true;
												isOptionStart = true;
												isChoiceCheck = isChoice;
												}
											else
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar opion definition can not be nested" );
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar definition must start with a grammar definition word" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

								break;

							case GRAMMAR_OPTION_READ_NOT_WRITE_END:
								isSkipOptionForWriting = false;

								// Don't insert a break statement here

							case GRAMMAR_OPTION_END:
								if( currentLanguageWordItem != NULL )
									{
									if( isOption )
										{
										if( isChoiceCheck == isChoice )
											{
											if( !hasFoundPipe )
												{
												isOption = false;
												isNewStart = true;
												currentLanguageWordItem->markAsOptionEnd();
												}
											else
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an extra pipe character before a square bracket in the grammar definition" );
											}
										else
											{
											if( isChoice )
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Choices are started within an option" );

											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Choices are ended within an option" );
											}
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an extra square bracket character in the grammar definition" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );

								break;

							case GRAMMAR_CHOICE_START:
								if( !hasFoundWordTypeNr ||
								grammarParameter >= GRAMMAR_SENTENCE )
									{
									if( commonVariables_->currentItemNr > NO_ITEM_NR )
										{
										if( !isChoice )
											{
											if( !hasFoundPipe )
												{
												isChoice = true;
												isNewStart = true;
												isChoiceStart = true;
												isOptionCheck = isOption;
												}
											else
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Nesting curved brackets isn't allowed" );
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar definition must start with a grammar definition word" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

								break;

							case GRAMMAR_CHOICE_END:
								if( currentLanguageWordItem != NULL )
									{
									if( isChoice )
										{
										if( isOptionCheck == isOption )
											{
											if( !hasFoundPipe )
												{
												if( hasFoundChoiceAlternatives )
													{
													isChoice = false;
													isNewStart = true;
													hasFoundChoiceAlternatives = false;
													currentLanguageWordItem->markAsChoiceEnd( commonVariables_->currentItemNr );
													}
												else
													return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar definition choice must have alternatives" );
												}
											else
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an extra pipe character before a curved bracket in the grammar definition" );
											}
										else
											{
											if( isOption )
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "An option is started within choices" );

											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "An option is ended within choices" );
											}
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an extra curved bracket character in the grammar definition" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );

								break;

							case SYMBOL_PIPE:
								if( !hasFoundWordTypeNr ||
								grammarParameter >= GRAMMAR_SENTENCE )
									{
									if( !hasFoundPipe )
										{
										if( isOption ||
										isChoice )
											{
											hasFoundPipe = true;

											if( isChoice )
												hasFoundChoiceAlternatives = true;
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Pipes are only allowed within grammar definition options or choices" );
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I've found an extra pipe character in the grammar definition" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

								break;

							// Either interface definition or Guide by Grammar
							case SYMBOL_DOUBLE_QUOTE:
								if( isNewStart &&
								!hasGrammarWords &&
								!isChoice &&
								!isOption )
									{
									// Interface definition
									if( grammarDefinitionItem == NULL )
										{
										if( hasFoundParameter )
											{
											grammarPosition++;

											if( grammarPosition < grammarStringLength ||
											grammarString[ grammarStringLength - 1 ] == SYMBOL_DOUBLE_QUOTE )
												{
												if( currentLanguageWordItem != NULL )
													{
													if( currentLanguageWordItem->checkInterface( grammarParameter, &grammarString[grammarPosition] ) == RESULT_OK )
														{
														if( currentLanguageWordItem->createInterfaceItem( grammarParameter, ( grammarStringLength - grammarPosition - 1 ), &grammarString[grammarPosition] ) == RESULT_OK )
															{
															hasCreatedInterface = true;
															grammarPosition = ( grammarStringLength - 1 );
															}
														else
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add an interface definition word item" );
														}
													else
														return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add an interface definition word item" );
													}
												else
													return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );
												}
											else
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I could a corrupte interface definition" );
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "An interface definition must have a parameter" );
										}
									else
										{
										// Guide by Grammar
										tempStringLength = 0;
										endPosition = grammarPosition + 1;

										while( endPosition < grammarStringLength &&
										// Find end of string
										grammarString[endPosition] != SYMBOL_DOUBLE_QUOTE )
											{
											endPosition++;
											tempStringLength++;
											}

										if( endPosition < grammarStringLength )
											{
											if( grammarDefinitionItem->guideByGrammarString != NULL )
												delete grammarDefinitionItem->guideByGrammarString;

											if( ( grammarDefinitionItem->guideByGrammarString = new char[tempStringLength + 1] ) != NULL )
												{
												strncpy( grammarDefinitionItem->guideByGrammarString, &grammarString[grammarPosition + 1], tempStringLength );
												grammarDefinitionItem->guideByGrammarString[tempStringLength] = NULL_CHAR;
												}
											else
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I failed to create the Guide by Grammar string" );
											}
										else
											return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The Guide by Grammar string is corrupt" );
										}
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Grammar definition and interface definitions can not be mixed" );

								break;

							default:
								if( !hasCreatedInterface )
									{
									if( commonVariables_->currentItemNr > NO_ITEM_NR )
										{
										if( hasFoundParameter &&
										grammarParameter < GRAMMAR_SENTENCE )
											{
											if( grammarParameter > NO_GRAMMAR_PARAMETER )
												{
												if( grammarParameter == WORD_PLURAL_NOUN_ENDING )
													{
													if( grammarDefinitionItem != NULL )
														{
														if( currentLanguageWordItem != NULL )
															{
															if( ( grammarResult = currentLanguageWordItem->createGrammarItem( false, false, false, false, false, WORD_TYPE_UNDEFINED, grammarParameter, readResult.wordLength, &grammarString[grammarPosition], grammarDefinitionItem ) ).result == RESULT_OK )
																{
																if( grammarDefinitionItem->nextDefinitionGrammarItem == NULL )
																	grammarDefinitionItem->nextDefinitionGrammarItem = grammarResult.createdGrammarItem;
																else
																	return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The next definition grammar item is undefined" );
																}
															else
																return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a grammar definition word item" );
															}
														else
															return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );
														}
													else
														return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The grammar definition item is undefined" );
													}
												else
													{
													foundWordItem = NULL;

													if( ( readResult.wordLength + grammarPosition ) < grammarStringLength )
														isMultipleWord = true;
													else
														{
														// End of line
														isEndOfLine = true;

														if( isMultipleWord )
															isLastPartOfMultipleWord = true;

														foundWordItem = adminItem_->predefinedWordItem( grammarParameter );
														}

													if( isLastPartOfMultipleWord ||
													foundWordItem == NULL )
														{
														if( ( wordResult = adminItem_->addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, grammarParameter, wordTypeNr, readResult.wordLength, &grammarString[grammarPosition] ) ).result == RESULT_OK )
															{
															if( ( createdWordItem = wordResult.createdWordItem ) != NULL )
																{
																if( isMultipleWord )
																	{
																	createdWordItem->predefinedMultipleWordNr = ++predefinedMultipleWordNr;

																	if( isLastPartOfMultipleWord )
																		{
																		if( addPredefinedMultipleWord( predefinedMultipleWordNr, grammarParameter, wordTypeNr, foundWordItem ) == RESULT_OK )
																			{
																			if( ( createdWordItem = multipleWordItem_ ) == NULL )
																				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't create a predefined multiple grammar word" );
																			}
																		else
																			return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a predefined multiple grammar word" );
																		}
																	}

																if( isEndOfLine )
																	{
																	if( assignPredefinedWord( grammarParameter, createdWordItem ) != RESULT_OK )
																		return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to assign a predefined word" );
																	}
																}
															else
																return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "I couldn't create a grammar word" );
															}
														else
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a grammar word" );
														}
													else
														{
														if( foundWordItem->addWordType( false, false, ( adminItem_->isAdjectiveParameter( grammarParameter ) ? grammarParameter : NO_ADJECTIVE_PARAMETER ), ( adminItem_->isDefiniteArticleParameter( grammarParameter ) ? grammarParameter : NO_DEFINITE_ARTICLE_PARAMETER ), ( adminItem_->isIndefiniteArticleParameter( grammarParameter ) ? grammarParameter : NO_INDEFINITE_ARTICLE_PARAMETER ), wordTypeNr, readResult.wordLength, &grammarString[grammarPosition] ).result != RESULT_OK )
															return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add another word type to grammar word \"", foundWordItem->anyWordTypeString(), "\"" );
														}
													}
												}
											}
										else
											{
											if( currentLanguageWordItem != NULL )
												{
												// Get grammar identification
												if( ( grammarResult = currentLanguageWordItem->findGrammar( true, NO_GRAMMAR_PARAMETER, readResult.wordLength, &grammarString[grammarPosition] ) ).result == RESULT_OK )
													{
													foundGrammarItem = grammarResult.foundGrammarItem;

													if( ( grammarResult = currentLanguageWordItem->createGrammarItem( false, ( hasFoundPipe || isNewStart ), isOptionStart, isChoiceStart, isSkipOptionForWriting, WORD_TYPE_UNDEFINED, NO_GRAMMAR_PARAMETER, readResult.wordLength, &grammarString[grammarPosition], foundGrammarItem ) ).result == RESULT_OK )
														{
														isNewStart = false;
														isOptionStart = false;
														isChoiceStart = false;

														hasFoundPipe = false;
														hasGrammarWords = true;

														if( !isOptionStart )
															hasFoundOnlyOptions = false;

														if( foundGrammarItem != NULL &&
														!foundGrammarItem->hasCurrentCreationSentenceNr() )
															foundGrammarItem->isGrammarItemInUse = true;
														}
													else
														return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to add a grammar item" );
													}
												else
													return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to find a grammar definition word item" );
												}
											else
												return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The current language word item is undefined" );
											}
										}
									else
										return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The first grammar word in a grammar definition is the grammar definition word and must start with a grammar character" );
									}
								else
									return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );
							}

						if( !hasFoundWordDefinitionInfo &&
						!readResult.hasFoundGrammarDefinition &&
						readResult.nextWordPosition < grammarStringLength )
							{
							if( ( readResult = adminItem_->readWordFromString( false, false, grammarPosition, 0, grammarString ) ).result != RESULT_OK )
								return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a new word from the grammar string" );
							}

						grammarPosition = readResult.nextWordPosition;
						}
					else
						return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to read a word from the grammar string" );
					}
				while( readResult.nextWordPosition < grammarStringLength );

				if( isOption )
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The grammar definition option isn't closed" );

				if( isChoice )
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The grammar definition choice isn't closed" );

				if( hasFoundPipe )
					return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The grammar definition ended with an open pipe" );

				if( grammarDefinitionItem != NULL )
					{
					if( hasFoundOnlyOptions &&

					( !hasFoundWordTypeNr ||
					grammarParameter >= GRAMMAR_SENTENCE ) )
						{
						if( hasGrammarWords )
							return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The grammar definition only exists of options" );

						return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The grammar definition only exists of a grammar definition word" );
						}

					// Remove possible duplicate grammar definition
					if( ( grammarResult = currentLanguageWordItem->checkForDuplicateGrammarDefinition() ).result == RESULT_OK )
						{
						if( currentLanguageWordItem->linkLaterDefinedGrammarWords() != RESULT_OK )
							return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to link later defined grammar words" );
						}
					else
						return adminItem_->addErrorInItem( functionNameString, moduleNameString_, "I failed to check for a duplicate grammar definition" );
					}
				}
			else
				return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given grammar string is empty" );
			}
		else
			return adminItem_->startErrorInItem( functionNameString, moduleNameString_, "The given grammar string is undefined" );

		return RESULT_OK;
		}

	WordItem *predefinedAdjectiveBusyWordItem()
		{
		return predefinedAdjectiveBusyWordItem_;
		}

	WordItem *predefinedAdjectiveDoneWordItem()
		{
		return predefinedAdjectiveDoneWordItem_;
		}

	WordItem *predefinedAdjectiveInvertedWordItem()
		{
		return predefinedAdjectiveInvertedWordItem_;
		}

	WordItem *predefinedNounPasswordWordItem()
		{
		return predefinedNounPasswordWordItem_;
		}

	WordItem *predefinedNounSolveLevelWordItem()
		{
		return predefinedNounSolveLevelWordItem_;
		}

	WordItem *predefinedNounSolveMethodWordItem()
		{
		return predefinedNounSolveMethodWordItem_;
		}

	WordItem *predefinedNounSolveStrategyWordItem()
		{
		return predefinedNounSolveStrategyWordItem_;
		}

	WordItem *predefinedNounStartupLanguageWordItem()
		{
		return predefinedNounStartupLanguageWordItem_;
		}

	WordItem *predefinedVerbLoginWordItem()
		{
		return predefinedVerbLoginWordItem_;
		}
	};

/*************************************************************************
 *	"The Lord is king!
 *	Let the nations tremble!
 *	He sits on his trone between the cherubim.
 *	Let the whole earth quake!" (Psalm 99:1)
 *************************************************************************/
