/*
 *	Class:			WordWriteSentence
 *	Supports class:	WordItem
 *	Purpose:		To write specifications as sentences
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

class WordWriteSentence
	{
	// Private constructible variables

	private boolean hasFoundWordToWrite_;
	private boolean isSkipClearWriteLevel_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte clearContextWriteLevel( short currentWriteLevel, SpecificationItem clearSpecificationItem )
		{
		WordItem currentWordItem;
		if( clearSpecificationItem != null )
			{
			if( clearSpecificationItem.hasGeneralizationContext() &&
			( currentWordItem = CommonVariables.firstWordItem ) != null )
				{
				// Do for all words
				do	currentWordItem.clearGeneralizationContextWriteLevelInWord( currentWriteLevel, clearSpecificationItem.generalizationContextNr() );
				while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
				}

			if( clearSpecificationItem.hasSpecificationContext() &&
			( currentWordItem = CommonVariables.firstWordItem ) != null )
				{
				// Do for all words
				do	currentWordItem.clearSpecificationContextWriteLevelInWord( currentWriteLevel, clearSpecificationItem.specificationContextNr() );
				while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
				}

			if( clearSpecificationItem.hasRelationContext() &&
			( currentWordItem = CommonVariables.firstWordItem ) != null )
				{
				// Do for all words
				do	currentWordItem.clearRelationContextWriteLevelInWord( currentWriteLevel, clearSpecificationItem.relationContextNr() );
				while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given clear specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte clearWriteLevel( boolean isWritingCurrentSpecificationWordOnly, short currentWriteLevel, SpecificationItem clearSpecificationItem )
		{
		boolean isAnsweredQuestion;
		boolean isExclusiveSpecification;
		boolean isNegative;
		boolean isPossessive;
		boolean isSelfGenerated;
		short assumptionLevel;
		int specificationCollectionNr;
		int generalizationContextNr;
		int specificationContextNr;
		int relationContextNr;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;

		if( clearSpecificationItem != null )
			{
			if( CommonVariables.currentWriteLevel >= currentWriteLevel )
				{
				myWordItem_.clearGeneralizationWriteLevel( currentWriteLevel );

				isAnsweredQuestion = clearSpecificationItem.isAnsweredQuestion();

				// Clear contexts
				if( clearContextWriteLevel( currentWriteLevel, clearSpecificationItem ) == Constants.RESULT_OK )
					{
					// Clear specification
					if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAnsweredQuestion, clearSpecificationItem.isAssignment(), clearSpecificationItem.isInactiveAssignment(), clearSpecificationItem.isArchivedAssignment(), clearSpecificationItem.questionParameter() ) ) != null )
						{
						isExclusiveSpecification = clearSpecificationItem.isExclusiveSpecification();
						isNegative = clearSpecificationItem.isNegative();
						isPossessive = clearSpecificationItem.isPossessive();
						isSelfGenerated = clearSpecificationItem.isSelfGenerated();

						assumptionLevel = clearSpecificationItem.assumptionLevel();

						specificationCollectionNr = clearSpecificationItem.specificationCollectionNr();
						generalizationContextNr = clearSpecificationItem.generalizationContextNr();
						specificationContextNr = clearSpecificationItem.specificationContextNr();
						relationContextNr = clearSpecificationItem.relationContextNr();

						do	{
							if( currentSpecificationItem == clearSpecificationItem ||

							( !isWritingCurrentSpecificationWordOnly &&
							currentSpecificationItem.isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) )
								{
								if( ( currentSpecificationWordItem = currentSpecificationItem.specificationWordItem() ) == null )		// Specification string
									currentSpecificationItem.clearSpecificationStringWriteLevel( currentWriteLevel );
								else
									currentSpecificationWordItem.clearSpecificationWriteLevel( currentWriteLevel );
								}
							}
						while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != null );
						}
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I couldn't find any specification" );
					}
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to clear the current write level of the current specification item" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given current write level of this wordf is higher than the global current write level" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given clear specification item is undefined" );

		return Constants.RESULT_OK;
		}

	private byte cleanupWriteInfo( boolean isWritingCurrentSpecificationWordOnly, short startWriteLevel, int startWordPosition, SpecificationItem clearSpecificationItem )
		{
		if( CommonVariables.writeSentenceStringBuffer != null &&
		CommonVariables.writeSentenceStringBuffer.length() > startWordPosition )
			{
			CommonVariables.writeSentenceStringBuffer = ( startWordPosition > 0 ? new StringBuffer( CommonVariables.writeSentenceStringBuffer.substring( 0, startWordPosition ) ) : null );

			if( CommonVariables.currentWriteLevel > startWriteLevel )
				{
				if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, startWriteLevel, clearSpecificationItem ) == Constants.RESULT_OK )
					myWordItem_.initializeWordWriteWordsSpecificationVariables( clearSpecificationItem.isQuestion(), startWordPosition );
				else
					return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to cleanup the write levels of the write words" );
				}
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given start position is equal or smaller than the size of the write sentence string buffer" );

		return Constants.RESULT_OK;
		}

	private byte createWriteWord( boolean isSkipped, short grammarLevel, GrammarItem selectedGrammarItem )
		{

		if( !myWordItem_.isAdminWord() )
			{
			if( selectedGrammarItem != null )
				{
				if( myWordItem_.writeList == null )
					{
					// Create list
					if( ( myWordItem_.writeList = new WriteList( myWordItem_ ) ) != null )
						myWordItem_.wordListArray[Constants.WORD_WRITE_LIST] = myWordItem_.writeList;
					else
						return myWordItem_.startErrorInWord( 1, moduleNameString_, "I failed to create a write list" );
					}

				return myWordItem_.writeList.createWriteItem( isSkipped, grammarLevel, selectedGrammarItem );
				}

			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given grammar definition word item is undefined" );
			}

		return myWordItem_.startErrorInWord( 1, moduleNameString_, "The admin word item cannot have a write list" );
		}

	private WriteItem firstWriteItem()
		{
		if( myWordItem_.writeList != null )
			return myWordItem_.writeList.firstActiveWriteItem();

		return null;
		}


	// Constructor / deconstructor

	protected WordWriteSentence( WordItem myWordItem )
		{
		String errorString = null;

		hasFoundWordToWrite_ = false;
		isSkipClearWriteLevel_ = false;

		myWordItem_ = myWordItem;
		moduleNameString_ = this.getClass().getName();

		if( myWordItem_ == null )
			errorString = "The given my word is undefined";

		if( errorString != null )
			{
			if( myWordItem_ != null )
				myWordItem_.startSystemErrorInWord( 1, moduleNameString_, errorString );
			else
				{
				CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
				Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.PRESENTATION_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\t" + errorString + ".\n" );
				}
			}
		}


	// Protected methods

	protected byte selectGrammarToWriteSentence( boolean isIntegrityCheck, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		WriteResultType writeResult;
		boolean isChoice;
		boolean isOption;
		boolean stillSuccessful;
		boolean skipThisChoiceOrOptionPart;
		boolean skipNextChoiceOrOptionParts;
		short startWriteLevel = CommonVariables.currentWriteLevel;
		int startWordPosition = ( CommonVariables.writeSentenceStringBuffer == null ? 0 : CommonVariables.writeSentenceStringBuffer.length() );
		GrammarItem definitionGrammarItem = selectedGrammarItem;
		WriteItem currentWriteItem = null;

		hasFoundWordToWrite_ = false;

		if( grammarLevel < Constants.MAX_GRAMMAR_LEVEL )
			{
			if( selectedGrammarItem != null )
				{
				if( writeSpecificationItem != null )
					{
					if( grammarLevel == Constants.NO_GRAMMAR_LEVEL )	// Init
						{
						isSkipClearWriteLevel_ = false;
						CommonVariables.currentWriteLevel = Constants.NO_WRITE_LEVEL;

						CommonVariables.writeSentenceStringBuffer = null;

						myWordItem_.deleteTemporaryWriteList();
						myWordItem_.initializeWordWriteWordsVariables();
						}

					do	{
						if( definitionGrammarItem.isDefinitionStart() )
							{
							if( definitionGrammarItem.isNewStart() )	// Grammar word
								{
								if( ( writeResult = myWordItem_.writeWordsToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem ) ).result == Constants.RESULT_OK )
									{
									if( writeResult.hasFoundWordToWrite )
										hasFoundWordToWrite_ = true;

									isSkipClearWriteLevel_ = writeResult.isSkipClearWriteLevel;
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a word to a sentence" );
								}
							else										// Grammar definition
								{
								if( ( selectedGrammarItem = definitionGrammarItem.nextGrammarItem() ) != null )
									{
									isChoice = false;
									isOption = false;
									stillSuccessful = true;
									skipThisChoiceOrOptionPart = false;
									skipNextChoiceOrOptionParts = false;

									do	{
										if( selectedGrammarItem.isNewStart() )
											{
											if( isChoice ||
											isOption ||
											selectedGrammarItem.isChoiceStart() ||
											selectedGrammarItem.isOptionStart() )
												{
												currentWriteItem = firstWriteItem();

												while( currentWriteItem != null &&
												( currentWriteItem.grammarLevel() != grammarLevel ||
												currentWriteItem.startOfChoiceOrOptionGrammarItem() != selectedGrammarItem ) )
													currentWriteItem = currentWriteItem.nextWriteItem();

												if( isChoice ||
												isOption )	// End of old choice or option - new one starts
													{
													skipThisChoiceOrOptionPart = false;

													if( hasFoundWordToWrite_ )
														skipNextChoiceOrOptionParts = true;
													else
														{
														if( stillSuccessful &&
														currentWriteItem != null &&
														currentWriteItem.isSkipped )
															currentWriteItem.isSkipped = false;
														}

													if( currentWriteItem == null )
														{
														if( createWriteWord( ( !stillSuccessful || skipNextChoiceOrOptionParts ), grammarLevel, selectedGrammarItem ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a write word" );
														}
													else
														{
														if( currentWriteItem.startOfChoiceOrOptionGrammarItem().activeSentenceNr() == selectedGrammarItem.activeSentenceNr() &&
														currentWriteItem.startOfChoiceOrOptionGrammarItem().itemNr() == selectedGrammarItem.itemNr() )
															{
															if( currentWriteItem.grammarLevel() == grammarLevel )
																currentWriteItem = currentWriteItem.nextWriteItem();
															else
																return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the grammar level" );
															}
														else
															return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the selected grammar path" );
														}
													}
												else
													{
													if( selectedGrammarItem.isChoiceStart() )
														isChoice = true;
													else
														isOption = true;

													skipThisChoiceOrOptionPart = false;

													if( currentWriteItem == null )
														{
														if( createWriteWord( !stillSuccessful, grammarLevel, selectedGrammarItem ) != Constants.RESULT_OK )
															return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to create a start write word" );
														}
													else
														{
														if( currentWriteItem.startOfChoiceOrOptionGrammarItem().activeSentenceNr() == selectedGrammarItem.activeSentenceNr() &&
														currentWriteItem.startOfChoiceOrOptionGrammarItem().itemNr() == selectedGrammarItem.itemNr() )
															{
															if( currentWriteItem.grammarLevel() != grammarLevel )
																return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the grammar level at the start of a choice or option" );
															}
														else
															return myWordItem_.startErrorInWord( 1, moduleNameString_, "I lost track of the selected grammar path at the start of a choice or option" );
														}
													}
												}
											}

										if( stillSuccessful &&
										!skipThisChoiceOrOptionPart &&
										!skipNextChoiceOrOptionParts &&
										!selectedGrammarItem.isSkipOptionForWriting() )
											{
											if( grammarLevel + 1 < Constants.MAX_GRAMMAR_LEVEL )
												{
												if( selectGrammarToWriteSentence( isIntegrityCheck, isWritingCurrentSpecificationWordOnly, answerParameter, (short)( grammarLevel + 1 ), selectedGrammarItem.definitionGrammarItem, writeSpecificationItem ) == Constants.RESULT_OK )
													{
													if( !hasFoundWordToWrite_ )
														skipThisChoiceOrOptionPart = true;	// Failed, try next part
													}
												else
													return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to select the grammar for writing a sentence at grammar level reached: #" + ( grammarLevel + 1 ) );
												}
											else
												return myWordItem_.startErrorInWord( 1, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #" + ( grammarLevel + 1 ) );
											}

										if( selectedGrammarItem.isChoiceEnd ||
										selectedGrammarItem.isOptionEnd )
											{
											skipThisChoiceOrOptionPart = false;
											skipNextChoiceOrOptionParts = false;

											if( selectedGrammarItem.isChoiceEnd )
												isChoice = false;
											else
												{
												isOption = false;

												if( stillSuccessful )
													hasFoundWordToWrite_ = true;
												}
											}

										selectedGrammarItem = selectedGrammarItem.nextGrammarItem();

										if( !isChoice &&
										!isOption &&
										!hasFoundWordToWrite_ )
											stillSuccessful = false;
										}
									while( selectedGrammarItem != null &&
									!selectedGrammarItem.isDefinitionStart() );

									if( !hasFoundWordToWrite_ )
										{
										if( !isSkipClearWriteLevel_ &&
										CommonVariables.writeSentenceStringBuffer != null &&
										CommonVariables.writeSentenceStringBuffer.length() > startWordPosition )
											{
											if( cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != Constants.RESULT_OK )
												return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to cleanup the write info" );
											}
										}
									}
								else
									return myWordItem_.startErrorInWord( 1, moduleNameString_, "The grammar word item is undefined" );
								}

							definitionGrammarItem = definitionGrammarItem.nextDefinitionGrammarItem;
							}
						else
							return myWordItem_.startErrorInWord( 1, moduleNameString_, "The grammar definition word item isn't a definition start" );
						}
					while( !hasFoundWordToWrite_ &&
					definitionGrammarItem != null );

					if( grammarLevel == Constants.NO_GRAMMAR_LEVEL )
						{
						if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, Constants.NO_WRITE_LEVEL, writeSpecificationItem ) != Constants.RESULT_OK )
							return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to clear the write word levels in all words" );
						}
					}
				else
					return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given write specification item is undefined" );
				}
			else
				return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given selected grammar item is undefined" );
			}
		else
			return myWordItem_.startErrorInWord( 1, moduleNameString_, "The given grammar level is too high: #" + grammarLevel );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"Let the faithful rejoice that he honors them.
 *	Let them sing for joy as they lie on their beds." (Psalm 149:5)
 *************************************************************************/
