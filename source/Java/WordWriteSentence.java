/*	Class:			WordWriteSentence
 *	Supports class:	WordItem
 *	Purpose:		To write specifications as sentences
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

class WordWriteSentence
	{
	// Private constructible variables

	private boolean hasFoundWordToWrite_;
	private boolean isSkippingClearWriteLevel_;

	private WordItem myWordItem_;
	private String moduleNameString_;


	// Private methods

	private byte clearWriteLevel( boolean isWritingCurrentSpecificationWordOnly, short currentWriteLevel, SpecificationItem clearSpecificationItem )
		{
		boolean isAnsweredQuestion;
		boolean isExclusiveSpecification = false;
		boolean isNegative = false;
		boolean isPossessive = false;
		boolean isSelfGenerated = false;
		short assumptionLevel = Constants.NO_ASSUMPTION_LEVEL;
		int specificationCollectionNr = Constants.NO_COLLECTION_NR;
		int generalizationContextNr = Constants.NO_CONTEXT_NR;
		int specificationContextNr = Constants.NO_CONTEXT_NR;
		int relationContextNr = Constants.NO_CONTEXT_NR;
		SpecificationItem currentSpecificationItem;
		WordItem currentSpecificationWordItem;
		WordItem currentWordItem;

		if( clearSpecificationItem != null )
			{
			if( CommonVariables.currentWriteLevel >= currentWriteLevel )
				{
				// Clear generalization write level
				myWordItem_.clearGeneralizationWriteLevel( currentWriteLevel );

				// Clear specification write level
				if( ( currentSpecificationWordItem = clearSpecificationItem.specificationWordItem() ) != null )
					{
					currentSpecificationWordItem.clearSpecificationWriteLevel( currentWriteLevel );
					relationContextNr = clearSpecificationItem.relationContextNr();

					if( !isWritingCurrentSpecificationWordOnly &&
					clearSpecificationItem.hasSpecificationCollection() )
						{
						isAnsweredQuestion = clearSpecificationItem.isAnsweredQuestion();
						isExclusiveSpecification = clearSpecificationItem.isExclusiveSpecification();
						isNegative = clearSpecificationItem.isNegative();
						isPossessive = clearSpecificationItem.isPossessive();
						isSelfGenerated = clearSpecificationItem.isSelfGenerated();

						assumptionLevel = clearSpecificationItem.assumptionLevel();

						specificationCollectionNr = clearSpecificationItem.specificationCollectionNr();
						generalizationContextNr = clearSpecificationItem.generalizationContextNr();
						specificationContextNr = clearSpecificationItem.specificationContextNr();

						if( ( currentSpecificationItem = myWordItem_.firstSelectedSpecificationItem( isAnsweredQuestion, clearSpecificationItem.isAssignment(), clearSpecificationItem.isInactiveAssignment(), clearSpecificationItem.isArchivedAssignment(), clearSpecificationItem.questionParameter() ) ) != null )
							{
							do	{
								if( ( currentSpecificationWordItem = currentSpecificationItem.relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, Constants.WORD_TYPE_UNDEFINED, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) != null )
									currentSpecificationWordItem.clearSpecificationWriteLevel( currentWriteLevel );
								}
							while( ( currentSpecificationItem = currentSpecificationItem.nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != null );
							}
						}

					// Clear relation context write level
					if( relationContextNr > Constants.NO_CONTEXT_NR &&
					( currentWordItem = CommonVariables.lastPredefinedWordItem ) != null )
						{
						// Do for all active words
						do	{
							if( currentWordItem.hasContextInWord( relationContextNr ) )
								currentWordItem.clearRelationWriteLevel( currentWriteLevel );
							}
						while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
						}
					}
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
		if( CommonVariables.writtenSentenceStringBuffer != null &&
		CommonVariables.writtenSentenceStringBuffer.length() > startWordPosition )
			{
			CommonVariables.writtenSentenceStringBuffer = new StringBuffer( startWordPosition > 0 ? CommonVariables.writtenSentenceStringBuffer.substring( 0, startWordPosition ) : Constants.EMPTY_STRING );

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
		WriteList writeList;

		if( ( writeList = myWordItem_.writeList ) != null )
			return writeList.firstActiveWriteItem();

		return null;
		}


	// Constructor / deconstructor

	protected WordWriteSentence( WordItem myWordItem )
		{
		String errorString = null;

		hasFoundWordToWrite_ = false;
		isSkippingClearWriteLevel_ = false;

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

	protected byte selectGrammarToWriteSentence( boolean isCheckingUserSentenceForIntegrity, boolean isWritingCurrentSpecificationWordOnly, short answerParameter, short grammarLevel, GrammarItem selectedGrammarItem, SpecificationItem writeSpecificationItem )
		{
		WriteResultType writeResult;
		boolean isChoice;
		boolean isOption;
		boolean isSkippingThisChoiceOrOptionPart;
		boolean isSkippingNextChoiceOrOptionParts;
		boolean isStillSuccessful;
		short startWriteLevel = CommonVariables.currentWriteLevel;
		// Take the current write sentence string buffer length as start position
		int startWordPosition = ( CommonVariables.writtenSentenceStringBuffer == null ? 0 : CommonVariables.writtenSentenceStringBuffer.length() );
		GrammarItem definitionGrammarItem = selectedGrammarItem;
		WordItem currentLanguageWordItem;
		WriteItem currentWriteItem = null;

		hasFoundWordToWrite_ = false;

		if( grammarLevel < Constants.MAX_GRAMMAR_LEVEL )
			{
			if( selectedGrammarItem != null )
				{
				if( writeSpecificationItem != null )
					{
					// Initialize
					if( grammarLevel == Constants.NO_GRAMMAR_LEVEL )
						{
						isSkippingClearWriteLevel_ = false;
						CommonVariables.currentWriteLevel = Constants.NO_WRITE_LEVEL;
						CommonVariables.writtenSentenceStringBuffer = new StringBuffer();

						myWordItem_.deleteTemporaryWriteList();
						myWordItem_.initializeWordWriteWordsVariables();
						}

					do	{
						if( definitionGrammarItem.isDefinitionStart() )
							{
							// Grammar word
							if( definitionGrammarItem.isNewStart() )
								{
								if( ( writeResult = myWordItem_.writeWordsToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem ) ).result == Constants.RESULT_OK )
									{
									if( writeResult.hasFoundWordToWrite )
										hasFoundWordToWrite_ = true;

									isSkippingClearWriteLevel_ = writeResult.isSkippingClearWriteLevel;
									}
								else
									return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to write a word to a sentence" );
								}
							else
								{
								// Grammar definition
								if( ( selectedGrammarItem = definitionGrammarItem.nextGrammarItem() ) != null )
									{
									isChoice = false;
									isOption = false;
									isStillSuccessful = true;
									isSkippingThisChoiceOrOptionPart = false;
									isSkippingNextChoiceOrOptionParts = false;

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
												// End of old choice or option - new one starts
												isOption )
													{
													isSkippingThisChoiceOrOptionPart = false;

													if( hasFoundWordToWrite_ )
														isSkippingNextChoiceOrOptionParts = true;
													else
														{
														if( isStillSuccessful &&
														currentWriteItem != null &&
														currentWriteItem.isSkipped )
															currentWriteItem.isSkipped = false;
														}

													if( currentWriteItem == null )
														{
														if( createWriteWord( ( !isStillSuccessful || isSkippingNextChoiceOrOptionParts ), grammarLevel, selectedGrammarItem ) != Constants.RESULT_OK )
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

													isSkippingThisChoiceOrOptionPart = false;

													if( currentWriteItem == null )
														{
														if( createWriteWord( !isStillSuccessful, grammarLevel, selectedGrammarItem ) != Constants.RESULT_OK )
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

										if( isStillSuccessful &&
										!isSkippingThisChoiceOrOptionPart &&
										!isSkippingNextChoiceOrOptionParts &&
										!selectedGrammarItem.isSkipOptionForWriting() )
											{
											if( grammarLevel + 1 < Constants.MAX_GRAMMAR_LEVEL )
												{
												if( selectGrammarToWriteSentence( false, isWritingCurrentSpecificationWordOnly, answerParameter, (short)( grammarLevel + 1 ), selectedGrammarItem.definitionGrammarItem, writeSpecificationItem ) == Constants.RESULT_OK )
													{
													if( !hasFoundWordToWrite_ )
														// Failed, try next part
														isSkippingThisChoiceOrOptionPart = true;
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
											isSkippingThisChoiceOrOptionPart = false;
											isSkippingNextChoiceOrOptionParts = false;

											if( selectedGrammarItem.isChoiceEnd )
												isChoice = false;
											else
												{
												isOption = false;

												if( isStillSuccessful )
													hasFoundWordToWrite_ = true;
												}
											}

										selectedGrammarItem = selectedGrammarItem.nextGrammarItem();

										if( !isChoice &&
										!isOption &&
										!hasFoundWordToWrite_ )
											isStillSuccessful = false;
										}
									while( selectedGrammarItem != null &&
									!selectedGrammarItem.isDefinitionStart() );

									if( !hasFoundWordToWrite_ &&
									!isSkippingClearWriteLevel_ &&
									// The sentence has grown
									CommonVariables.writtenSentenceStringBuffer != null &&
									CommonVariables.writtenSentenceStringBuffer.length() > startWordPosition )
										{
										if( cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != Constants.RESULT_OK )
											return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to cleanup the write info" );
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
						if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, Constants.NO_WRITE_LEVEL, writeSpecificationItem ) == Constants.RESULT_OK )
							{
							if( CommonVariables.writtenSentenceStringBuffer != null &&
							CommonVariables.writtenSentenceStringBuffer.length() > 0 )
								{
								if( isCheckingUserSentenceForIntegrity )
									CommonVariables.writtenUserSentenceStringBuffer = new StringBuffer( CommonVariables.writtenSentenceStringBuffer );

								// Typically for French: Compound words
								currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

								if( currentLanguageWordItem != null &&
								currentLanguageWordItem.isLanguageWithMergedWords() )
									{
									if( currentLanguageWordItem.shrinkMergedWordsInWriteSentence() != Constants.RESULT_OK )
										return myWordItem_.addErrorInWord( 1, moduleNameString_, "I failed to shrink the compound words in the write sentence string" );
									}
								}
							}
						else
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
