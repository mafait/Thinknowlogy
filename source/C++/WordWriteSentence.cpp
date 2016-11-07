/*	Class:			WordWriteSentence
 *	Supports class:	WordItem
 *	Purpose:		To write specifications as sentences
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

#include "SpecificationItem.cpp"
#include "WriteList.cpp"

class WordWriteSentence
	{
	friend class WordItem;

	// Private constructed variables

	bool hasFoundWordToWrite_;
	bool isSkippingClearWriteLevel_;

	CommonVariables *commonVariables_;
	WordItem *myWordItem_;
	char moduleNameString_[FUNCTION_NAME_LENGTH];


	// Private functions

	ResultType clearWriteLevel( bool isWritingCurrentSpecificationWordOnly, unsigned short currentWriteLevel, SpecificationItem *clearSpecificationItem )
		{
		bool isAnsweredQuestion;
		bool isExclusiveSpecification = false;
		bool isNegative = false;
		bool isPossessive = false;
		bool isSelfGenerated = false;
		unsigned short assumptionLevel;
		unsigned int specificationCollectionNr;
		unsigned int generalizationContextNr;
		unsigned int specificationContextNr;
		unsigned int relationContextNr;
		SpecificationItem *currentSpecificationItem;
		WordItem *currentContextWordItem;
		WordItem *currentSpecificationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "clearWriteLevel";

		if( clearSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given clear specification item is undefined" );

		if( commonVariables_->currentWriteLevel < currentWriteLevel )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given current write level of this wordf is higher than the global current write level" );

		// Clear generalization write level
		myWordItem_->clearGeneralizationWriteLevel( currentWriteLevel );

		if( ( currentSpecificationWordItem = clearSpecificationItem->specificationWordItem() ) != NULL )
			{
			// Clear specification write level of current specification word
			currentSpecificationWordItem->clearSpecificationWriteLevel( currentWriteLevel );

			if( ( relationContextNr = clearSpecificationItem->relationContextNr() ) == NO_CONTEXT_NR )
				{
				if( !isWritingCurrentSpecificationWordOnly &&
				( specificationCollectionNr = clearSpecificationItem->specificationCollectionNr() ) > NO_COLLECTION_NR )
					{
					// Clear specification write level of related specification words
					isAnsweredQuestion = clearSpecificationItem->isAnsweredQuestion();
					isExclusiveSpecification = clearSpecificationItem->isExclusiveSpecification();
					isNegative = clearSpecificationItem->isNegative();
					isPossessive = clearSpecificationItem->isPossessive();
					isSelfGenerated = clearSpecificationItem->isSelfGenerated();

					assumptionLevel = clearSpecificationItem->assumptionLevel();

					generalizationContextNr = clearSpecificationItem->generalizationContextNr();
					specificationContextNr = clearSpecificationItem->specificationContextNr();

					if( ( currentSpecificationItem = myWordItem_->firstSelectedSpecificationItem( isAnsweredQuestion, clearSpecificationItem->isAssignment(), clearSpecificationItem->isInactiveAssignment(), clearSpecificationItem->isArchivedAssignment(), clearSpecificationItem->questionParameter() ) ) != NULL )
						{
						do	{
							if( currentSpecificationItem != clearSpecificationItem &&
							( currentSpecificationWordItem = currentSpecificationItem->relatedSpecificationWordItem( isExclusiveSpecification, isNegative, isPossessive, isSelfGenerated, assumptionLevel, NO_WORD_TYPE_NR, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr ) ) != NULL )
								currentSpecificationWordItem->clearSpecificationWriteLevel( currentWriteLevel );
							}
						while( ( currentSpecificationItem = currentSpecificationItem->nextSelectedQuestionParameterSpecificationItem( isAnsweredQuestion ) ) != NULL );
						}
					}
				}
			else
				{
				// Clear relation context write level
				if( ( currentContextWordItem = commonVariables_->firstContextWordItem ) != NULL )
					{
					// Do for all context words
					do	currentContextWordItem->clearRelationWriteLevel( currentWriteLevel, relationContextNr );
					while( ( currentContextWordItem = currentContextWordItem->nextContextWordItem ) != NULL );
					}
				}
			}

		return RESULT_OK;
		}

	ResultType cleanupWriteInfo( bool isWritingCurrentSpecificationWordOnly, unsigned short startWriteLevel, size_t startWordPosition, SpecificationItem *clearSpecificationItem )
		{
		char tempString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_LENGTH] = "cleanupWriteInfo";

		if( strlen( commonVariables_->writtenSentenceString ) <= startWordPosition )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given start position is equal or smaller than the size of the write sentence string" );

		strncat( tempString, commonVariables_->writtenSentenceString, startWordPosition );
		strcpy( commonVariables_->writtenSentenceString, tempString );

		if( commonVariables_->currentWriteLevel > startWriteLevel )
			{
			if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, startWriteLevel, clearSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to cleanup the write levels of the write words" );

			myWordItem_->initializeWordWriteWordsSpecificationVariables( clearSpecificationItem->isQuestion(), startWordPosition );
			}

		return RESULT_OK;
		}

	ResultType createWriteWord( bool isSkipped, unsigned short grammarLevel, GrammarItem *selectedGrammarItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "createWriteWord";

		if( selectedGrammarItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar definition word item is undefined" );

		if( myWordItem_->writeList == NULL )
			{
			// Create list
			if( ( myWordItem_->writeList = new WriteList( commonVariables_, myWordItem_ ) ) == NULL )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I failed to create a write list" );

			myWordItem_->wordListArray[WORD_WRITE_LIST] = myWordItem_->writeList;
			}

		return myWordItem_->writeList->createWriteItem( isSkipped, grammarLevel, selectedGrammarItem );
		}

	WriteItem *firstWriteItem()
		{
		WriteList *writeList;

		if( ( writeList = myWordItem_->writeList ) != NULL )
			return writeList->firstActiveWriteItem();

		return NULL;
		}


	protected:
	// Constructor

	WordWriteSentence( CommonVariables *commonVariables, WordItem *myWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		hasFoundWordToWrite_ = false;
		isSkippingClearWriteLevel_ = false;

		commonVariables_ = commonVariables;
		myWordItem_ = myWordItem;
		strcpy( moduleNameString_, "WordWriteSentence" );

		if( commonVariables_ == NULL )
			strcpy( errorString, "The given common variables is undefined" );

		if( myWordItem_ == NULL )
			strcpy( errorString, "The given my word is undefined" );

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

	ResultType selectGrammarToWriteSentence( bool isCheckingUserSentenceForIntegrity, bool isWritingCurrentSpecificationWordOnly, unsigned short answerParameter, unsigned short grammarLevel, GrammarItem *selectedGrammarItem, SpecificationItem *writeSpecificationItem )
		{
		WriteResultType writeResult;
		bool isChoice;
		bool isOption;
		bool isSkippingThisChoiceOrOptionPart;
		bool isSkippingNextChoiceOrOptionParts;
		bool isStillSuccessful;
		unsigned short startWriteLevel = commonVariables_->currentWriteLevel;
		// Take the current write sentence string length as start position
		size_t startWordPosition = strlen( commonVariables_->writtenSentenceString );
		GrammarItem *definitionGrammarItem = selectedGrammarItem;
		WordItem *currentLanguageWordItem;
		WriteItem *currentWriteItem = NULL;
		char functionNameString[FUNCTION_NAME_LENGTH] = "selectGrammarToWriteSentence";

		hasFoundWordToWrite_ = false;

		if( grammarLevel >= MAX_GRAMMAR_LEVEL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given grammar level is too high: #", grammarLevel );

		if( selectedGrammarItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given selected grammar item is undefined" );

		if( writeSpecificationItem == NULL )
			return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The given write specification item is undefined" );

		// Initialize
		if( grammarLevel == NO_GRAMMAR_LEVEL )
			{
			isSkippingClearWriteLevel_ = false;
			commonVariables_->currentWriteLevel = NO_WRITE_LEVEL;
			strcpy( commonVariables_->writtenSentenceString, EMPTY_STRING );

			myWordItem_->deleteTemporaryWriteList();
			myWordItem_->initializeWordWriteWordsVariables();
			}

		do	{
			if( !definitionGrammarItem->isDefinitionStart() )
				return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The grammar definition word item isn't a definition start" );

			// Grammar word
			if( definitionGrammarItem->isNewStart() )
				{
				if( ( writeResult = myWordItem_->writeWordsToSentence( isWritingCurrentSpecificationWordOnly, answerParameter, definitionGrammarItem, writeSpecificationItem ) ).result != RESULT_OK )
					return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to write a word to a sentence" );

				if( writeResult.hasFoundWordToWrite )
					hasFoundWordToWrite_ = true;

				isSkippingClearWriteLevel_ = writeResult.isSkippingClearWriteLevel;
				}
			else
				{
				// Grammar definition
				if( ( selectedGrammarItem = definitionGrammarItem->nextGrammarItem() ) == NULL )
					return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "The grammar word item is undefined" );

				isChoice = false;
				isOption = false;
				isStillSuccessful = true;
				isSkippingThisChoiceOrOptionPart = false;
				isSkippingNextChoiceOrOptionParts = false;

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
							// End of old choice or option - new one starts
							isOption )
								{
								isSkippingThisChoiceOrOptionPart = false;

								if( hasFoundWordToWrite_ )
									isSkippingNextChoiceOrOptionParts = true;
								else
									{
									if( isStillSuccessful &&
									currentWriteItem != NULL &&
									currentWriteItem->isSkipped )
										currentWriteItem->isSkipped = false;
									}

								if( currentWriteItem == NULL )
									{
									if( createWriteWord( ( !isStillSuccessful || isSkippingNextChoiceOrOptionParts ), grammarLevel, selectedGrammarItem ) != RESULT_OK )
										return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a write word" );
									}
								else
									{
									if( currentWriteItem->startOfChoiceOrOptionGrammarItem()->activeSentenceNr() != selectedGrammarItem->activeSentenceNr() ||
									currentWriteItem->startOfChoiceOrOptionGrammarItem()->itemNr() != selectedGrammarItem->itemNr() )
										return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the selected grammar path" );

									if( currentWriteItem->grammarLevel() != grammarLevel )
										return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the grammar level" );

									currentWriteItem = currentWriteItem->nextWriteItem();
									}
								}
							else
								{
								if( selectedGrammarItem->isChoiceStart() )
									isChoice = true;
								else
									isOption = true;

								isSkippingThisChoiceOrOptionPart = false;

								if( currentWriteItem == NULL )
									{
									if( createWriteWord( !isStillSuccessful, grammarLevel, selectedGrammarItem ) != RESULT_OK )
										return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to create a start write word" );
									}
								else
									{
									if( currentWriteItem->startOfChoiceOrOptionGrammarItem()->activeSentenceNr() != selectedGrammarItem->activeSentenceNr() ||
									currentWriteItem->startOfChoiceOrOptionGrammarItem()->itemNr() != selectedGrammarItem->itemNr() )
										return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the selected grammar path at the start of a choice or option" );

									if( currentWriteItem->grammarLevel() != grammarLevel )
										return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "I lost track of the grammar level at the start of a choice or option" );
									}
								}
							}
						}

					if( isStillSuccessful &&
					!isSkippingThisChoiceOrOptionPart &&
					!isSkippingNextChoiceOrOptionParts &&
					!selectedGrammarItem->isSkipOptionForWriting() )
						{
						if( grammarLevel + 1 >= MAX_GRAMMAR_LEVEL )
							return myWordItem_->startErrorInWord( functionNameString, moduleNameString_, "There is probably an endless loop in the grammar definitions, because the grammar level reached: #", ( grammarLevel + 1 ) );

						if( selectGrammarToWriteSentence( false, isWritingCurrentSpecificationWordOnly, answerParameter, ( grammarLevel + 1 ), selectedGrammarItem->definitionGrammarItem, writeSpecificationItem ) != RESULT_OK )
							return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to select the grammar for writing a sentence at grammar level reached: #", ( grammarLevel + 1 ) );

						if( !hasFoundWordToWrite_ )
							// Failed, try next part
							isSkippingThisChoiceOrOptionPart = true;
						}

					if( selectedGrammarItem->isChoiceEnd ||
					selectedGrammarItem->isOptionEnd )
						{
						isSkippingThisChoiceOrOptionPart = false;
						isSkippingNextChoiceOrOptionParts = false;

						if( selectedGrammarItem->isChoiceEnd )
							isChoice = false;
						else
							{
							isOption = false;

							if( isStillSuccessful )
								hasFoundWordToWrite_ = true;
							}
						}

					selectedGrammarItem = selectedGrammarItem->nextGrammarItem();

					if( !isChoice &&
					!isOption &&
					!hasFoundWordToWrite_ )
						isStillSuccessful = false;
					}
				while( selectedGrammarItem != NULL &&
				!selectedGrammarItem->isDefinitionStart() );

				if( !hasFoundWordToWrite_ &&
				!isSkippingClearWriteLevel_ &&
				// The sentence has grown
				strlen( commonVariables_->writtenSentenceString ) > startWordPosition )
					{
					if( cleanupWriteInfo( isWritingCurrentSpecificationWordOnly, startWriteLevel, startWordPosition, writeSpecificationItem ) != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to cleanup the write info" );
					}
				}

			definitionGrammarItem = definitionGrammarItem->nextDefinitionGrammarItem;
			}
		while( !hasFoundWordToWrite_ &&
		definitionGrammarItem != NULL );

		if( grammarLevel == NO_GRAMMAR_LEVEL )
			{
			// Cleanup
			myWordItem_->deleteTemporaryWriteList();

			if( clearWriteLevel( isWritingCurrentSpecificationWordOnly, NO_WRITE_LEVEL, writeSpecificationItem ) != RESULT_OK )
				return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to clear the write word levels in all words" );

			if( strlen( commonVariables_->writtenSentenceString ) > 0 )
				{
				if( isCheckingUserSentenceForIntegrity )
					strcpy( commonVariables_->writtenUserSentenceString, commonVariables_->writtenSentenceString );

				// Typically for French: Compound words
				currentLanguageWordItem = commonVariables_->currentLanguageWordItem;

				if( currentLanguageWordItem != NULL &&
				currentLanguageWordItem->isLanguageWithMergedWords() )
					{
					if( currentLanguageWordItem->shrinkMergedWordsInWriteSentence() != RESULT_OK )
						return myWordItem_->addErrorInWord( functionNameString, moduleNameString_, "I failed to shrink the compound words in the write sentence string" );
					}
				}
			}

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"Let the faithful rejoice that he honors them.
 *	Let them sing for joy as they lie on their beds." (Psalm 149:5)
 *************************************************************************/
