/*
 *	Class:			WordWriteSentence
 *	Supports class:	WordItem
 *	Purpose:		To write specifications as sentences
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

#include "SpecificationItem.cpp"
#include "WriteList.cpp"

class WordWriteSentence
	{
	friend class WordItem;

	// Private constructible variables

	bool hasFoundWordToWrite_;
	bool isSkipClearWriteLevel_;

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType clearContextWriteLevel( unsigned short currentWriteLevel, SpecificationItem *clearSpecificationItem )
		{
		WordItem *currentWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "clearContextWriteLevel";

		if( clearSpecificationItem != NULL )
			{
			if( clearSpecificationItem->hasGeneralizationContext() &&
			( currentWordItem = commonVariables_->firstWordItem ) != NULL )
				{
				// Do for all words
				do	currentWordItem->clearGeneralizationContextWriteLevelInWord( currentWriteLevel, clearSpecificationItem->generalizationContextNr() );
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
				}

			if( clearSpecificationItem->hasSpecificationContext() &&
			( currentWordItem = commonVariables_->firstWordItem ) != NULL )
				{
				// Do for all words
				do	currentWordItem->clearSpecificationContextWriteLevelInWord( currentWriteLevel, clearSpecificationItem->specificationContextNr() );
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
				}

			if( clearSpecificationItem->hasRelationContext() &&
			( currentWordItem = commonVariables_->firstWordItem ) != NULL )
				{
				// Do for all words
				do	currentWordItem->clearRelationContextWriteLevelInWord( currentWriteLevel, clearSpecificationItem->relationContextNr() );
				while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given clear specification item is undefined" );

		return RESULT_OK;
		}

	ResultType clearWriteLevel( bool isWritingCurrentSpecificationWordOnly, unsigned short currentWriteLevel, SpecificationItem *clearSpecificationItem )
		{
		bool isAnsweredQuestion;
		bool isExclusiveSpecification;
		bool isNegative;
		bool isPossessive;
		bool isSelfGenerated;
		unsigned short assumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "clearWriteLevel";

		if( clearSpecificationItem != NULL )
			{
			if( commonVariables_->currentWriteLevel >= currentWriteLevel )
				{
				myWordItem_->clearGeneralizationWriteLevel( currentWriteLevel );

				isAnsweredQuestion = clearSpecificationItem->isAnsweredQuestion();

				// Clear contexts
				if( clearContextWriteLevel( currentWriteLevel, clearSpecificationItem ) == RESULT_OK )
					{
					// Clear specification
					if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( true, clearSpecificationItem->isAssignment(), clearSpecificationItem->isInactiveAssignment(), clearSpecificationItem->isArchivedAssignment(), clearSpecificationItem->questionParameter() ) ) != NULL )
						{
						isExclusiveSpecification = clearSpecificationItem->isExclusiveSpecification();
						isNegative = clearSpecificationItem->isNegative();
						isPossessive = clearSpecificationItem->isPossessive();
						isSelfGenerated = clearSpecificationItem->isSelfGenerated();

						assumptionLevel = clearSpecificationItem->assumptionLevel();

						specificationCollectionNr = clearSpecificationItem->specificationCollectionNr();
						generalizationContextNr = clearSpecificationItem->generalizationContextNr();
						specificationContextNr = clearSpecificationItem->specificationContextNr();
						relationContextNr = clearSpecificationItem->relationContextNr();

						do	{
							if( currentSpecificationItem == clearSpecificationItem ||

							( !isWritingCurrentSpecificationWordOnly &&
							currentSpecificationItem->isRelatedSpecification( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) )
								{
								if( ( currentSpecificationWordItem = currentSpecificationItem->specificationWordItem() ) == NULL )		// Specification string
									currentSpecificationItem->clearSpecificationStringWriteLevel( currentWriteLevel );
								else
									currentSpecificationWordItem->clearSpecificationWriteLevel( currentWriteLevel );
								}
							}
						while( ( currentSpecificationItem = currentSpecificationItem->nextSpecificationItemWithSameQuestionParameter( isAnsweredQuestion ) ) != NULL );
						}
					else
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I couldn't find any specification" );
					}
				else
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to clear the current write level of the current specification item" );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given current write level of this wordf is higher than the global current write level" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given clear specification item is undefined" );

		return RESULT_OK;
		}

	ResultType cleanupWriteInfo( bool isWritingCurrentSpecificationWordOnly, unsigned short startWriteLevel, size_t startWordPosition, SpecificationItem *clearSpecificationItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "cleanupWriteInfo";
		if( strlen( commonVariables_->writeSentenceString ) > startWordPosition )
			{
			commonVariables_->writeSentenceString[startWordPosition] = NULL_CHAR;

			if( commonVariables_->currentWriteLevel > startWriteLevel )
				{
				if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, startWriteLevel, clearSpecificationItem ) == RESULT_OK )
					myWordItem_->initializeWordWriteWordsSpecificationVariables( clearSpecificationItem->isQuestion(), startWordPosition );
				else
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to cleanup the write levels of the write words" );
				}
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given start position is equal or smaller than the size of the write sentence string" );

		return RESULT_OK;
		}

	ResultType createWriteWord( bool isSkipped, unsigned short grammarLevel, GrammarItem *selectedGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWriteWord";

		if( !myWordItem_->isAdminWord() )
			{
			if( selectedGrammarItem != NULL )
				{
				if( myWordItem_->writeList == NULL )
					{
					// Create list
					if( ( myWordItem_->writeList = new WriteList( commonVariables_, myWordItem_ ) ) != NULL )
						myWordItem_->wordListArray[WORD_WRITE_LIST] = myWordItem_->writeList;
					else
						return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I failed to create a write list" );
					}

				return myWordItem_->writeList->createWriteItem( isSkipped, grammarLevel, selectedGrammarItem );
				}

			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar definition word item is undefined" );
			}

		return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The admin word item cannot have a write list" );
		}

	WriteItem *firstWriteItem()
		{
		if( myWordItem_->writeList != NULL )
			return myWordItem_->writeList->firstActiveWriteItem();

		return NULL;
		}


	protected:
	// Constructor / deconstructor

	WordWriteSentence( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundWordToWrite_ = false;
		isSkipClearWriteLevel_ = false;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordWriteSentence" );

		if( commonVariables_ != NULL )
			{
		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );
			}
		else
			strcpy( errorString, "The given common variables is undefined" );

		if( strlen( errorString ) > 0 )
			{
			if( myWordItem_ != NULL )
				myWordItem_->startSystemErrorInWord( PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			else
				{
			if( commonVariables_ != NULL )
				commonVariables_->result = RESULT_SYSTEM_ERROR;
				fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\t%s.\n", moduleNameString_, PRESENTATION_ERROR_CONSTRUCTOR_FUNCTION_NAME, errorString );
				}
			}
		}


	// Protected functions

	ResultType selectGrammarToWriteSentence( bool isIntegrityCheck, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		WriteResultType writeResult;
		bool isChoice;
		bool isOption;
		bool stillSuccessful;
		bool skipThisChoiceOrOptionPart;
		bool skipNextChoiceOrOptionParts;
		unsigned short startWriteLevel = commonVariables_->currentWriteLevel;
		size_t startWordPosition = strlen( commonVariables_->writeSentenceString );
		GrammarItem *definitionGrammarItem = selectedGrammarItem;
		WriteItem *currentWriteItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "selectGrammarToWriteSentence";

		hasFoundWordToWrite_ = false;

		if( grammarLevel < MAX_GRAMMAR_LEVEL )
			{
			if( selectedGrammarItem != NULL )
				{
				if( writeSpecificationItem != NULL )
					{
					if( grammarLevel == NO_GRAMMAR_LEVEL )	// Init
						{
						isSkipClearWriteLevel_ = false;
						commonVariables_->currentWriteLevel = NO_WRITE_LEVEL;

						strcpy( commonVariables_->writeSentenceString, EMPTY_STRING );

						myWordItem_->deleteTemporaryWriteList();
						myWordItem_->initializeWordWriteWordsVariables();
						}

					do	{
						if( definitionGrammarItem->isDefinitionStart() )
							{
							if( definitionGrammarItem->isNewStart() )	// Grammar word
								{
								if( ( writeResult = myWordItem_->writeWordsToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem ) ).result == RESULT_OK )
									{
									if( writeResult.hasFoundWordToWrite )
										hasFoundWordToWrite_ = true;

									isSkipClearWriteLevel_ = writeResult.isSkipClearWriteLevel;
									}
								else
									return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a word to a specification sentence" );
								}
							else										// Grammar definition
								{
								if( ( selectedGrammarItem = definitionGrammarItem->nextGrammarItem() ) != NULL )
									{
									isChoice = false;
									isOption = false;
									stillSuccessful = true;
									skipThisChoiceOrOptionPart = false;
									skipNextChoiceOrOptionParts = false;

									do	{
										if( selectedGrammarItem->isNewStart() )
											{
											if( isChoice ||
											isOption ||
											selectedGrammarItem->isChoiceStart() ||
											selectedGrammarItem->isOptionStart() )
												{
												currentWriteItem = firstWriteItem();

												while( currentWriteItem != NULL &&
												( currentWriteItem->grammarLevel() != grammarLevel ||
												currentWriteItem->startOfChoiceOrOptionGrammarItem() != selectedGrammarItem ) )
													currentWriteItem = currentWriteItem->nextWriteItem();

												if( isChoice ||
												isOption )	// End of old choice or option - new one starts
													{
													skipThisChoiceOrOptionPart = false;

													if( hasFoundWordToWrite_ )
														skipNextChoiceOrOptionParts = true;
													else
														{
														if( stillSuccessful &&
														currentWriteItem != NULL &&
														currentWriteItem->isSkipped )
															currentWriteItem->isSkipped = false;
														}

													if( currentWriteItem == NULL )
														{
														if( createWriteWord( ( !stillSuccessful || skipNextChoiceOrOptionParts ), grammarLevel, selectedGrammarItem ) != RESULT_OK )
															return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a write word" );
														}
													else
														{
														if( currentWriteItem->startOfChoiceOrOptionGrammarItem()->activeSentenceNr() == selectedGrammarItem->activeSentenceNr() &&
														currentWriteItem->startOfChoiceOrOptionGrammarItem()->itemNr() == selectedGrammarItem->itemNr() )
															{
															if( currentWriteItem->grammarLevel() == grammarLevel )
																currentWriteItem = currentWriteItem->nextWriteItem();
															else
																return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the grammar level" );
															}
														else
															return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the selected grammar path" );
														}
													}
												else
													{
													if( selectedGrammarItem->isChoiceStart() )
														isChoice = true;
													else
														isOption = true;

													skipThisChoiceOrOptionPart = false;

													if( currentWriteItem == NULL )
														{
														if( createWriteWord( !stillSuccessful, grammarLevel, selectedGrammarItem ) != RESULT_OK )
															return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a start write word" );
														}
													else
														{
														if( currentWriteItem->startOfChoiceOrOptionGrammarItem()->activeSentenceNr() == selectedGrammarItem->activeSentenceNr() &&
														currentWriteItem->startOfChoiceOrOptionGrammarItem()->itemNr() == selectedGrammarItem->itemNr() )
															{
															if( currentWriteItem->grammarLevel() != grammarLevel )
																return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the grammar level at the start of a choice or option" );
															}
														else
															return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the selected grammar path at the start of a choice or option" );
														}
													}
												}
											}

										if( stillSuccessful &&
										!skipThisChoiceOrOptionPart &&
										!skipNextChoiceOrOptionParts &&
										!selectedGrammarItem->isSkipOptionForWriting() )
											{
											if( grammarLevel + 1 < MAX_GRAMMAR_LEVEL )
												{
												if( selectGrammarToWriteSentence( isIntegrityCheck, isWritingCurrentSpecificationWordOnly, answerParameter, ( grammarLevel + 1 ), selectedGrammarItem->definitionGrammarItem, writeSpecificationItem ) == RESULT_OK )
													{
													if( !hasFoundWordToWrite_ )
														skipThisChoiceOrOptionPart = true;	// Failed, try next part
													}
												else
													return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to select the grammar for writing a sentence at grammar level reached: #", ( grammarLevel + 1 ) );
												}
											else
												return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #", ( grammarLevel + 1 ) );
											}

										if( selectedGrammarItem->isChoiceEnd ||
										selectedGrammarItem->isOptionEnd )
											{
											skipThisChoiceOrOptionPart = false;
											skipNextChoiceOrOptionParts = false;

											if( selectedGrammarItem->isChoiceEnd )
												isChoice = false;
											else
												{
												isOption = false;

												if( stillSuccessful )
													hasFoundWordToWrite_ = true;
												}
											}

										selectedGrammarItem = selectedGrammarItem->nextGrammarItem();

										if( !isChoice &&
										!isOption &&
										!hasFoundWordToWrite_ )
											stillSuccessful = false;
										}
									while( selectedGrammarItem != NULL &&
									!selectedGrammarItem->isDefinitionStart() );

									if( !hasFoundWordToWrite_ )
										{
										if( !isSkipClearWriteLevel_ &&
										strlen( commonVariables_->writeSentenceString ) > startWordPosition )
											{
											if( cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != RESULT_OK )
												return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to cleanup the write info" );
											}
										}
									}
								else
									return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The grammar word item is undefined" );
								}

							definitionGrammarItem = definitionGrammarItem->nextDefinitionGrammarItem;
							}
						else
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The grammar definition word item isn't a definition start" );
						}
					while( !hasFoundWordToWrite_ &&
					definitionGrammarItem != NULL );

					if( grammarLevel == NO_GRAMMAR_LEVEL )
						{
						if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, NO_WRITE_LEVEL, writeSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to clear the write word levels in all words" );
						}
					}
				else
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );
				}
			else
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given selected grammar item is undefined" );
			}
		else
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar level is too high: #", grammarLevel );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *
 *	"Let the faithful rejoice that he honors them.
 *	Let them sing for joy as they lie on their beds." (Psalm 149:5)
 *
 *************************************************************************/
