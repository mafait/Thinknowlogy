/*
 *	Class:			AdminReadGrammar
 *	Supports class:	AdminItem
 *	Purpose:		To read and process grammar and interface files
 *	Version:		Thinknowlogy 2014r2a (George Boole)
 *
 *************************************************************************/
/*
 *	Thinknowlogy is grammar-based software,
 *	designed to utilize Natural Laws of Intelligence in grammar,
 *	in order to create intelligence through natural language in software,
 *	which is demonstrated by:
 *	- Programming in natural language;
 *	- Reasoning in natural language:
 *		- drawing conclusions (more advanced than scientific solutions),
 *		- making assumptions (with self-adjusting level of uncertainty),
 *		- asking questions (about gaps in the knowledge),
 *		- detecting conflicts in the knowledge;
 *	- Building semantics autonomously (no vocabularies):
 *		- detecting some cases of semantic ambiguity;
 *	- Multilingualism, proving: Natural Laws of Intelligence are universal.
 *
 *************************************************************************/
/*
 *	Copyright (C) 2009-2014, Menno Mafait
 *	Your additions, modifications, suggestions and bug reports
 *	are welcome at http://mafait.org
 *
 *************************************************************************/
/*
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *************************************************************************/

class AdminReadGrammar
	{
	// Private constructible variables

	private WordItem predefinedAdjectiveBusyWordItem_;
	private WordItem predefinedAdjectiveDoneWordItem_;
	private WordItem predefinedAdjectiveInvertedWordItem_;

	private WordItem predefinedNounExpertWordItem_;
	private WordItem predefinedNounInterfaceLanguageWordItem_;
	private WordItem predefinedNounPasswordWordItem_;
	private WordItem predefinedNounSolveLevelWordItem_;
	private WordItem predefinedNounSolveMethodWordItem_;
	private WordItem predefinedNounSolveStrategyWordItem_;
	private WordItem predefinedVerbLoginWordItem_;

	private AdminItem adminItem_;
	private WordItem myWordItem_;
	private String moduleNameString_;

	// Constructor / deconstructor

	protected AdminReadGrammar( AdminItem adminItem, WordItem myWordItem )
		{
		String errorString = null;

		predefinedAdjectiveBusyWordItem_ = null;
		predefinedAdjectiveDoneWordItem_ = null;
		predefinedAdjectiveInvertedWordItem_ = null;

		predefinedNounExpertWordItem_ = null;
		predefinedNounInterfaceLanguageWordItem_ = null;
		predefinedNounPasswordWordItem_ = null;
		predefinedNounSolveLevelWordItem_ = null;
		predefinedNounSolveMethodWordItem_ = null;
		predefinedNounSolveStrategyWordItem_ = null;
		predefinedVerbLoginWordItem_ = null;

		adminItem_ = adminItem;
		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ != null )
			{
			if( myWordItem_ == null )
				errorString = "The given my word is undefined";
			}
		else
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInItem( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected byte addGrammar( String grammarString )
		{
		GrammarResultType grammarResult;
		ReadResultType readResult;
		WordResultType wordResult;
		boolean hasFoundWordDefinitionInfo;
		boolean isMultipleWord;
		boolean hasCreatedInterface = false;
		boolean hasFoundChoiceAlternatives = false;
		boolean hasFoundOnlyOptions = true;
		boolean hasFoundParameter = false;
		boolean hasFoundPipe = false;
		boolean hasFoundWordTypeNr = false;
		boolean hasGrammarWords = false;
		boolean isChoice = false;
		boolean isChoiceCheck = false;
		boolean isChoiceStart = false;
		boolean isOption = false;
		boolean isOptionCheck = false;
		boolean isOptionStart = false;
		boolean isNewStart = true;
		boolean isSkipOptionForWriting = false;
		short grammarParameter = Constants.NO_GRAMMAR_PARAMETER;
		short wordTypeNr = Constants.WORD_TYPE_UNDEFINED;
		int firstCreationItemNr = Constants.NO_ITEM_NR;
		int endPosition;
		int grammarStringLength;
		int grammarPosition = 0;
		GrammarItem foundGrammarItem;
		GrammarItem grammarDefinitionItem = null;
		WordItem foundWordItem;
		WordItem createdWordItem;
		WordItem currentGrammarLanguageWordItem = CommonVariables.currentGrammarLanguageWordItem;

		if( grammarString != null )
			{
			if( ( grammarStringLength = grammarString.length() ) > 0 )
				{
				do	{
					if( ( readResult = adminItem_.readWordFromString( true, false, grammarPosition, 0, grammarString ) ).result == Constants.RESULT_OK )
						{
						hasFoundWordDefinitionInfo = false;

						switch( grammarString.charAt( grammarPosition ) )
							{
							case Constants.QUERY_WORD_TYPE_CHAR:
								if( !hasFoundWordTypeNr )
									{
									while( grammarPosition + 1 < grammarStringLength &&
									Character.isDigit( grammarString.charAt( grammarPosition + 1 ) ) )
										{
										hasFoundWordTypeNr = true;
										wordTypeNr = (short)( wordTypeNr * 10 + grammarString.charAt( ++grammarPosition ) - '0' );
										}

									if( !hasFoundWordTypeNr )
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to get the word type number from a grammar definition line" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found more than one word type parameters defined in a grammar definition line" );

								break;

							case Constants.QUERY_PARAMETER_CHAR:
								if( !hasFoundParameter )
									{
									// Get paramater from string
									while( grammarPosition + 1 < grammarStringLength &&
									Character.isDigit( grammarString.charAt( grammarPosition + 1 ) ) )
										{
										hasFoundParameter = true;
										grammarParameter = (short)( grammarParameter * 10 + grammarString.charAt( ++grammarPosition ) - '0' );
										}

									if( !hasFoundParameter )
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to get the grammar parameter from a grammar definition line" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found more than one values parameters defined in a grammar definition line" );

								break;

							case Constants.GRAMMAR_WORD_DEFINITION_CHAR:
								if( currentGrammarLanguageWordItem != null )
									{
									if( !hasCreatedInterface )
										{
										if( firstCreationItemNr == Constants.NO_ITEM_NR )		// Add grammar word or grammar definition word
											{
											if( ( readResult = adminItem_.readWordFromString( false, false, ++grammarPosition, 0, grammarString ) ).result == Constants.RESULT_OK )
												{
												if( ( grammarResult = currentGrammarLanguageWordItem.findGrammar( ( grammarParameter >= Constants.GRAMMAR_SENTENCE ), grammarParameter, readResult.wordLength, grammarString.substring( grammarPosition ) ) ).result == Constants.RESULT_OK )
													{
													hasFoundWordDefinitionInfo = true;

													if( !hasFoundWordTypeNr ||
													grammarParameter >= Constants.GRAMMAR_SENTENCE ||
													grammarResult.foundGrammarItem == null )
														{
														if( ( grammarResult = currentGrammarLanguageWordItem.createGrammar( true, ( hasFoundParameter && grammarParameter < Constants.GRAMMAR_SENTENCE ), false, false, false, wordTypeNr, grammarParameter, readResult.wordLength, grammarString.substring( grammarPosition ), null ) ).result == Constants.RESULT_OK )
															{
															firstCreationItemNr = CommonVariables.currentItemNr;
															grammarDefinitionItem = grammarResult.createdGrammarItem;
															}
														else
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a grammar definition word item" );
														}
													else
														{
														if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_START, grammarResult.foundGrammarItem.grammarParameter(), Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_MIDDLE, currentGrammarLanguageWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_END ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to write an interface notification" );
														}
													}
												else
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a grammar definition item" );
												}
											else
												return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a word definition from the grammar string" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar definition word must be the first word in the grammar definition" );
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current grammar language word item is undefined" );

								break;

							case Constants.GRAMMAR_OPTION_READ_NOT_WRITE_START:
								isSkipOptionForWriting = true;

								// Don't insert a break statement here

							case Constants.GRAMMAR_OPTION_START:
								if( !hasFoundWordTypeNr ||
								grammarParameter >= Constants.GRAMMAR_SENTENCE )
									{
									if( CommonVariables.currentItemNr > Constants.NO_ITEM_NR )
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
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar opion definition can not be nested" );
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar definition must start with a grammar definition word" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

								break;

							case Constants.GRAMMAR_OPTION_READ_NOT_WRITE_END:
								isSkipOptionForWriting = false;

								// Don't insert a break statement here

							case Constants.GRAMMAR_OPTION_END:
								if( currentGrammarLanguageWordItem != null )
									{
									if( isOption )
										{
										if( isChoiceCheck == isChoice )
											{
											if( !hasFoundPipe )
												{
												isOption = false;
												isNewStart = true;
												currentGrammarLanguageWordItem.markAsOptionEnd();
												}
											else
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an extra pipe character before a square bracket in the grammar definition" );
											}
										else
											{
											if( isChoice )
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "Choices are started within an option" );

											return myWordItem_.startErrorInItem( 1, moduleNameString_, "Choices are ended within an option" );
											}
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an extra square bracket character in the grammar definition" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current grammar language word item is undefined" );

								break;

							case Constants.GRAMMAR_CHOICE_START:
								if( !hasFoundWordTypeNr ||
								grammarParameter >= Constants.GRAMMAR_SENTENCE )
									{
									if( CommonVariables.currentItemNr > Constants.NO_ITEM_NR )
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
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "Nesting curved brackets isn't allowed" );
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar definition must start with a grammar definition word" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

								break;

							case Constants.GRAMMAR_CHOICE_END:
								if( currentGrammarLanguageWordItem != null )
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
													currentGrammarLanguageWordItem.markAsChoiceEnd( CommonVariables.currentItemNr );
													}
												else
													return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar definition choice must have alternatives" );
												}
											else
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an extra pipe character before a curved bracket in the grammar definition" );
											}
										else
											{
											if( isOption )
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "An option is started within choices" );

											return myWordItem_.startErrorInItem( 1, moduleNameString_, "An option is ended within choices" );
											}
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an extra curved bracket character in the grammar definition" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current grammar language word item is undefined" );

								break;

							case Constants.SYMBOL_PIPE:
								if( !hasFoundWordTypeNr ||
								grammarParameter >= Constants.GRAMMAR_SENTENCE )
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
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "Pipes are only allowed within grammar definition options or choices" );
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found an extra pipe character in the grammar definition" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

								break;

							case Constants.SYMBOL_DOUBLE_QUOTE:	// Interface definition or Guide by Grammar
								if( isNewStart &&
								!hasGrammarWords &&
								!isChoice &&
								!isOption )
									{
									if( grammarDefinitionItem == null )		// Interface definition
										{
										if( hasFoundParameter )
											{
											grammarPosition++;

											if( grammarPosition < grammarStringLength ||
											grammarString.charAt( grammarStringLength - 1 ) == Constants.SYMBOL_DOUBLE_QUOTE )
												{
												if( CommonVariables.currentInterfaceLanguageWordItem != null )
													{
													if( CommonVariables.currentInterfaceLanguageWordItem.checkInterface( grammarParameter, grammarString.substring( grammarPosition ) ) == Constants.RESULT_OK )
														{
														if( CommonVariables.currentInterfaceLanguageWordItem.createInterface( grammarParameter, ( grammarStringLength - grammarPosition - 1 ), grammarString.substring( grammarPosition ) ) == Constants.RESULT_OK )
															{
															hasCreatedInterface = true;
															grammarPosition = ( grammarStringLength - 1 );
															}
														else
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add an interface definition word item" );
														}
													else
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add an interface definition word item" );
													}
												else
													return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current interface language word item is undefined" );
												}
											else
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "I could a corrupte interface definition" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "An interface definition must have a parameter" );
										}
									else	// Guide by Grammar
										{
										endPosition = grammarPosition + 1;

										while( endPosition < grammarStringLength &&
										grammarString.charAt( endPosition ) != Constants.SYMBOL_DOUBLE_QUOTE )		// Find end of string
											endPosition++;

										if( endPosition < grammarStringLength )
											{
											if( ( grammarDefinitionItem.guideByGrammarString = grammarString.substring( grammarPosition + 1, endPosition ) ) == null )
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "I failed to create the Guide by Grammar string" );
											}
										else
											return myWordItem_.startErrorInItem( 1, moduleNameString_, "The Guide by Grammar string is corrupt" );
										}
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "Grammar definition and interface definitions can not be mixed" );

								break;

							default:
								if( !hasCreatedInterface )
									{
									if( CommonVariables.currentItemNr > Constants.NO_ITEM_NR )
										{
										if( hasFoundParameter &&
										grammarParameter < Constants.GRAMMAR_SENTENCE )
											{
											if( grammarParameter > Constants.NO_GRAMMAR_PARAMETER )
												{
												if( grammarParameter == Constants.WORD_PLURAL_NOUN_ENDING )
													{
													if( grammarDefinitionItem != null )
														{
														if( currentGrammarLanguageWordItem != null )
															{
															if( ( grammarResult = currentGrammarLanguageWordItem.createGrammar( false, false, false, false, false, Constants.WORD_TYPE_UNDEFINED, grammarParameter, readResult.wordLength, grammarString.substring( grammarPosition ), grammarDefinitionItem ) ).result == Constants.RESULT_OK )
																{
																if( grammarDefinitionItem.nextDefinitionGrammarItem == null )
																	grammarDefinitionItem.nextDefinitionGrammarItem = grammarResult.createdGrammarItem;
																else
																	return myWordItem_.startErrorInItem( 1, moduleNameString_, "The next definition grammar item is undefined" );
																}
															else
																return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a grammar definition word item" );
															}
														else
															return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current grammar language word item is undefined" );
														}
													else
														return myWordItem_.startErrorInItem( 1, moduleNameString_, "The grammar definition item is undefined" );
													}
												else
													{
													isMultipleWord = false;

													if( ( foundWordItem = myWordItem_.predefinedWordItem( grammarParameter ) ) != null )
														{
														if( foundWordItem.hasWordType( wordTypeNr ) )
															isMultipleWord = true;
														}

													if( isMultipleWord ||
													foundWordItem == null )
														{
														if( ( wordResult = adminItem_.addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, grammarParameter, wordTypeNr, readResult.wordLength, grammarString.substring( grammarPosition ) ) ).result == Constants.RESULT_OK )
															{
															if( ( createdWordItem = wordResult.createdWordItem ) != null )
																{
																if( isMultipleWord )
																	{
																	if( ( wordResult = adminItem_.addMultipleWord( grammarParameter, wordTypeNr, foundWordItem, createdWordItem, null ) ).result == Constants.RESULT_OK )
																		{
																		if( ( createdWordItem = wordResult.createdWordItem ) == null )
																			return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't create a multiple word" );
																		}
																	else
																		return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a multiple grammar word" );
																	}

																switch( grammarParameter )
																	{
																	case Constants.WORD_PARAMETER_ADJECTIVE_BUSY:
																		if( predefinedAdjectiveBusyWordItem_ == null )
																			predefinedAdjectiveBusyWordItem_ = createdWordItem;
																		else
																			{
																			if( predefinedAdjectiveBusyWordItem_ != createdWordItem )
																				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined busy adjective word" );
																			}

																		break;

																	case Constants.WORD_PARAMETER_ADJECTIVE_DONE:
																		if( predefinedAdjectiveDoneWordItem_ == null )
																			predefinedAdjectiveDoneWordItem_ = createdWordItem;
																		else
																			{
																			if( predefinedAdjectiveDoneWordItem_ != createdWordItem )
																				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined done adjective word" );
																			}

																		break;

																	case Constants.WORD_PARAMETER_ADJECTIVE_INVERTED:
																		if( predefinedAdjectiveInvertedWordItem_ == null )
																			predefinedAdjectiveInvertedWordItem_ = createdWordItem;
																		else
																			{
																			if( predefinedAdjectiveInvertedWordItem_ != createdWordItem )
																				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined inverted adjective word" );
																			}

																		break;

																	case Constants.WORD_PARAMETER_NOUN_EXPERT:
																		if( predefinedNounExpertWordItem_ == null )
																			{
																			predefinedNounExpertWordItem_ = createdWordItem;

																			if( adminItem_.authorizeWord( predefinedNounExpertWordItem_ ) != Constants.RESULT_OK )
																				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the predefined expert noun word" );
																			}
																		else
																			{
																			if( predefinedNounExpertWordItem_ != createdWordItem )
																				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined expert noun word" );
																			}

																		break;

																	case Constants.WORD_PARAMETER_NOUN_GRAMMAR_LANGUAGE:
																		if( CommonVariables.predefinedNounGrammarLanguageWordItem == null )
																			CommonVariables.predefinedNounGrammarLanguageWordItem = createdWordItem;
																		else
																			{
																			if( isMultipleWord )
																				CommonVariables.predefinedNounGrammarLanguageWordItem = createdWordItem;
																			else
																				{
																				if( CommonVariables.predefinedNounGrammarLanguageWordItem != createdWordItem )
																					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined grammar language noun word" );
																				}
																			}

																		if( adminItem_.authorizeLanguageWord( CommonVariables.predefinedNounGrammarLanguageWordItem ) == Constants.RESULT_OK )
																			{
																			if( adminItem_.createLanguageSpecification( currentGrammarLanguageWordItem, CommonVariables.predefinedNounGrammarLanguageWordItem ) != Constants.RESULT_OK )
																				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a grammar language specification" );
																			}
																		else
																			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the predefined grammar language noun word" );

																		break;

																	case Constants.WORD_PARAMETER_NOUN_INTERFACE_LANGUAGE:
																		if( predefinedNounInterfaceLanguageWordItem_ == null )
																			predefinedNounInterfaceLanguageWordItem_ = createdWordItem;
																		else
																			{
																			if( isMultipleWord )
																				predefinedNounInterfaceLanguageWordItem_ = createdWordItem;
																			else
																				{
																				if( predefinedNounInterfaceLanguageWordItem_ != createdWordItem )
																					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined interface language noun word" );
																				}
																			}

																		if( adminItem_.authorizeLanguageWord( predefinedNounInterfaceLanguageWordItem_ ) == Constants.RESULT_OK )
																			{
																			if( adminItem_.createLanguageSpecification( CommonVariables.currentInterfaceLanguageWordItem, predefinedNounInterfaceLanguageWordItem_ ) != Constants.RESULT_OK )
																				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create an interface language specification" );
																			}
																		else
																			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the predefined interface language noun word" );

																		break;

																	case Constants.WORD_PARAMETER_NOUN_PASSWORD:
																		if( predefinedNounPasswordWordItem_ == null )
																			{
																			predefinedNounPasswordWordItem_ = createdWordItem;

																			if( adminItem_.authorizeWord( predefinedNounPasswordWordItem_ ) != Constants.RESULT_OK )
																				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the predefined password noun word" );
																			}
																		else
																			{
																			if( predefinedNounPasswordWordItem_ != createdWordItem )
																				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined password noun word" );
																			}

																		break;

																	case Constants.WORD_PARAMETER_NOUN_SOLVE_LEVEL:
																		if( predefinedNounSolveLevelWordItem_ == null )
																			predefinedNounSolveLevelWordItem_ = createdWordItem;
																		else
																			{
																			if( isMultipleWord )
																				predefinedNounSolveLevelWordItem_ = createdWordItem;
																			else
																				{
																				if( predefinedNounSolveLevelWordItem_ != createdWordItem )
																					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined solve level noun word" );
																				}
																			}

																		break;

																	case Constants.WORD_PARAMETER_NOUN_SOLVE_METHOD:
																		if( predefinedNounSolveMethodWordItem_ == null )
																			predefinedNounSolveMethodWordItem_ = createdWordItem;
																		else
																			{
																			if( isMultipleWord )
																				predefinedNounSolveMethodWordItem_ = createdWordItem;
																			else
																				{
																				if( predefinedNounSolveMethodWordItem_ != createdWordItem )
																					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined solve method noun word" );
																				}
																			}

																		break;

																	case Constants.WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
																		if( predefinedNounSolveStrategyWordItem_ == null )
																			predefinedNounSolveStrategyWordItem_ = createdWordItem;
																		else
																			{
																			if( isMultipleWord )
																				predefinedNounSolveStrategyWordItem_ = createdWordItem;
																			else
																				{
																				if( predefinedNounSolveStrategyWordItem_ != createdWordItem )
																					return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined solve strategy noun word" );
																				}
																			}

																		break;

																	case Constants.WORD_PARAMETER_NOUN_USER:
																		if( CommonVariables.predefinedNounUserWordItem == null )
																			{
																			CommonVariables.predefinedNounUserWordItem = createdWordItem;

																			if( adminItem_.authorizeWord( CommonVariables.predefinedNounUserWordItem ) != Constants.RESULT_OK )
																				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the predefined user noun word" );
																			}
																		else
																			{
																			if( CommonVariables.predefinedNounUserWordItem != createdWordItem )
																				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined user noun word" );
																			}

																		break;

																	case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
																		if( predefinedVerbLoginWordItem_ == null )
																			{
																			predefinedVerbLoginWordItem_ = createdWordItem;

																			if( adminItem_.authorizeWord( predefinedVerbLoginWordItem_ ) != Constants.RESULT_OK )
																				return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to authorize the predefined login verb word" );
																			}
																		else
																			{
																			if( predefinedVerbLoginWordItem_ != createdWordItem )
																				return myWordItem_.startErrorInItem( 1, moduleNameString_, "I found a different predefined login verb word" );
																			}

																		break;
																	}
																}
															else
																return myWordItem_.startErrorInItem( 1, moduleNameString_, "I couldn't add a grammar word" );
															}
														else
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a grammar word" );
														}
													else
														{
														if( foundWordItem.addWordType( false, false, ( adminItem_.isAdjectiveParameter( grammarParameter ) ? grammarParameter : Constants.NO_ADJECTIVE_PARAMETER ), ( adminItem_.isDefiniteArticleParameter( grammarParameter ) ? grammarParameter : Constants.NO_DEFINITE_ARTICLE_PARAMETER ), ( adminItem_.isIndefiniteArticleParameter( grammarParameter ) ? grammarParameter : Constants.NO_INDEFINITE_ARTICLE_PARAMETER ), wordTypeNr, readResult.wordLength, grammarString.substring( grammarPosition ) ).result == Constants.RESULT_OK )
															{
															readResult.nextWordPosition = grammarStringLength;		// End of the line

															switch( grammarParameter )
																{
																case Constants.WORD_PARAMETER_NOUN_GRAMMAR_LANGUAGE:
																	if( CommonVariables.predefinedNounGrammarLanguageWordItem != null )
																		{
																		if( adminItem_.createLanguageSpecification( currentGrammarLanguageWordItem, CommonVariables.predefinedNounGrammarLanguageWordItem ) != Constants.RESULT_OK )
																			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create a grammar language specification" );
																		}
																	else
																		return myWordItem_.startErrorInItem( 1, moduleNameString_, "The predefined grammar language noun word item is undefined" );

																	break;

																case Constants.WORD_PARAMETER_NOUN_INTERFACE_LANGUAGE:
																	if( predefinedNounInterfaceLanguageWordItem_ != null )
																		{
																		if( adminItem_.createLanguageSpecification( CommonVariables.currentInterfaceLanguageWordItem, predefinedNounInterfaceLanguageWordItem_ ) != Constants.RESULT_OK )
																			return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to create an interface language specification" );
																		}
																	else
																		return myWordItem_.startErrorInItem( 1, moduleNameString_, "The predefined interface language noun word item is undefined" );
																}
															}
														else
															return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add another word type to grammar word \"" + foundWordItem.anyWordTypeString() + "\"" );
														}
													}
												}
											}
										else
											{
											if( currentGrammarLanguageWordItem != null )
												{
												// Get grammar identification
												if( ( grammarResult = currentGrammarLanguageWordItem.findGrammar( true, Constants.NO_GRAMMAR_PARAMETER, readResult.wordLength, grammarString.substring( grammarPosition ) ) ).result == Constants.RESULT_OK )
													{
													foundGrammarItem = grammarResult.foundGrammarItem;

													if( ( grammarResult = currentGrammarLanguageWordItem.createGrammar( false, ( hasFoundPipe || isNewStart ), isOptionStart, isChoiceStart, isSkipOptionForWriting, Constants.WORD_TYPE_UNDEFINED, Constants.NO_GRAMMAR_PARAMETER, readResult.wordLength, grammarString.substring( grammarPosition ), foundGrammarItem ) ).result == Constants.RESULT_OK )
														{
														isNewStart = false;
														isOptionStart = false;
														isChoiceStart = false;

														hasFoundPipe = false;
														hasGrammarWords = true;

														if( !isOptionStart )
															hasFoundOnlyOptions = false;

														if( foundGrammarItem != null &&
														!foundGrammarItem.hasCurrentCreationSentenceNr() )
															foundGrammarItem.isGrammarItemInUse = true;
														}
													else
														return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to add a grammar item" );
													}
												else
													return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to find a grammar definition word item" );
												}
											else
												return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current grammar language word item is undefined" );
											}
										}
									else
										return myWordItem_.startErrorInItem( 1, moduleNameString_, "The first grammar word in a grammar definition is the grammar definition word and must start with a grammar character" );
									}
								else
									return myWordItem_.startErrorInItem( 1, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );
							}

						if( !hasFoundWordDefinitionInfo &&
						!readResult.hasFoundGrammarDefinition &&
						readResult.nextWordPosition < grammarStringLength )
							{
							if( ( readResult = adminItem_.readWordFromString( false, false, grammarPosition, 0, grammarString ) ).result != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a new word from the grammar string" );
							}

						grammarPosition = readResult.nextWordPosition;
						}
					else
						return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to read a word from the grammar string" );
					}
				while( readResult.nextWordPosition < grammarStringLength );

				if( isOption )
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The grammar definition option isn't closed" );

				if( isChoice )
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The grammar definition choice isn't closed" );

				if( hasFoundPipe )
					return myWordItem_.startErrorInItem( 1, moduleNameString_, "The grammar definition ended with an open pipe" );

				if( grammarDefinitionItem != null )
					{
					if( hasFoundOnlyOptions &&

					( !hasFoundWordTypeNr ||
					grammarParameter >= Constants.GRAMMAR_SENTENCE ) )
						{
						if( hasGrammarWords )
							return myWordItem_.startErrorInItem( 1, moduleNameString_, "The grammar definition only exists of options" );

						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The grammar definition only exists of a grammar definition word" );
						}

					if( CommonVariables.currentGrammarLanguageWordItem != null )
						{
						// Remove possible duplicate grammar definition
						if( ( grammarResult = currentGrammarLanguageWordItem.checkForDuplicateGrammarDefinition() ).result == Constants.RESULT_OK )
							{
							if( currentGrammarLanguageWordItem.linkLaterDefinedGrammarWords() != Constants.RESULT_OK )
								return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to link later defined grammar words" );
							}
						else
							return myWordItem_.addErrorInItem( 1, moduleNameString_, "I failed to check for a duplicate grammar definition" );
						}
					else
						return myWordItem_.startErrorInItem( 1, moduleNameString_, "The current grammar language word item is undefined" );
					}
				}
			else
				return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given grammar string is empty" );
			}
		else
			return myWordItem_.startErrorInItem( 1, moduleNameString_, "The given grammar string is undefined" );

		return Constants.RESULT_OK;
		}

	protected WordItem predefinedAdjectiveBusyWordItem()
		{
		return predefinedAdjectiveBusyWordItem_;
		}

	protected WordItem predefinedAdjectiveDoneWordItem()
		{
		return predefinedAdjectiveDoneWordItem_;
		}

	protected WordItem predefinedAdjectiveInvertedWordItem()
		{
		return predefinedAdjectiveInvertedWordItem_;
		}

	protected WordItem predefinedNounExpertWordItem()
		{
		return predefinedNounExpertWordItem_;
		}

	protected WordItem predefinedNounInterfaceLanguageWordItem()
		{
		return predefinedNounInterfaceLanguageWordItem_;
		}

	protected WordItem predefinedNounPasswordWordItem()
		{
		return predefinedNounPasswordWordItem_;
		}

	protected WordItem predefinedNounSolveLevelWordItem()
		{
		return predefinedNounSolveLevelWordItem_;
		}

	protected WordItem predefinedNounSolveMethodWordItem()
		{
		return predefinedNounSolveMethodWordItem_;
		}

	protected WordItem predefinedNounSolveStrategyWordItem()
		{
		return predefinedNounSolveStrategyWordItem_;
		}

	protected WordItem predefinedVerbLoginWordItem()
		{
		return predefinedVerbLoginWordItem_;
		}
	};

/*************************************************************************
 *
 *	"The Lord is king!
 *	Let the nations tremble!
 *	He sits on his trone between the cherubim.
 *	Let the whole earth quake!" (Psalm 99:1)
 *
 *************************************************************************/
