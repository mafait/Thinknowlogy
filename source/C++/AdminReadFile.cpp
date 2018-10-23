/*	Class:			AdminReadFile
 *	Supports class:	AdminItem
 *	Purpose:		To read the grammar, user-interface and example files
 *	Version:		Thinknowlogy 2018r3 (Deep Magic)
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

#include <time.h>
#include "AdminItem.h"
#include "FileList.cpp"
#include "GeneralizationItem.cpp"
#include "GrammarItem.cpp"
#include "ReadWordResultType.cpp"
#include "WordTypeItem.cpp"

class AdminReadFile
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasClosedFileDueToError_ = false;
	bool hasFoundDifferentTestResult_ = false;
	bool isPredefinedMultipleWord_ = false;
	bool wasLoginCommand_ = false;

	unsigned short testFileNr_ = 0;

	unsigned int firstSentenceNrOfCurrentUser_ = NO_SENTENCE_NR;

	clock_t startTime_ = 0;

	WordItem *currentUserWordItem_ = NULL;
	WordItem *predefinedAdjectiveBusyWordItem_ = NULL;
	WordItem *predefinedAdjectiveDoneWordItem_ = NULL;
	WordItem *predefinedAdjectiveInvertedWordItem_ = NULL;
	WordItem *predefinedNounLanguageWordItem_ = NULL;
	WordItem *predefinedNounPasswordWordItem_ = NULL;
	WordItem *predefinedNounSolveLevelWordItem_ = NULL;
	WordItem *predefinedNounSolveMethodWordItem_ = NULL;
	WordItem *predefinedNounSolveStrategyWordItem_ = NULL;
	WordItem *predefinedNounStartupLanguageWordItem_ = NULL;
	WordItem *predefinedNounUserWordItem_ = NULL;
	WordItem *predefinedVerbLoginWordItem_ = NULL;

	char moduleNameString_[FUNCTION_NAME_STRING_LENGTH] = "AdminReadFile";

	// Private initialized variables

	AdminItem *adminItem_ = NULL;
	GlobalVariables *globalVariables_ = NULL;
	InputOutput *inputOutput_ = NULL;


	// Private functions

	void cleanupDeletedItems()
		{
		unsigned int startRemoveSentenceNr = NO_SENTENCE_NR;

		if( !hasClosedFileDueToError_ &&

		( globalVariables_->hasDisplayedWarning ||
		globalVariables_->hasDisplayedIntegrityWarning ||
		globalVariables_->result != RESULT_OK ) )
			adminItem_->deleteSentences( globalVariables_->currentSentenceNr );

		do	{
			globalVariables_->nDeletedItems = 0;
			globalVariables_->removeSentenceNr = NO_SENTENCE_NR;
			globalVariables_->removeStartItemNr = NO_ITEM_NR;

			adminItem_->removeFirstRangeOfDeletedItems();

			if( globalVariables_->nDeletedItems > 0 )
				{
				decrementItemNrRange( globalVariables_->removeSentenceNr, globalVariables_->removeStartItemNr, globalVariables_->nDeletedItems );
				startRemoveSentenceNr = globalVariables_->removeSentenceNr;
				}
			}
		while( globalVariables_->nDeletedItems > 0 &&
		// Avoid triggering on deleted items in temporary lists
		// Less efficient alternative: Include deleted items in decrement sentence number and item number
		( firstSentenceNrOfCurrentUser() + 1 ) != startRemoveSentenceNr );

		if( globalVariables_->hasDisplayedWarning )
			globalVariables_->hasDisplayedWarning = false;
		else
			{
			if( startRemoveSentenceNr > NO_SENTENCE_NR &&
			// Previous deleted sentence might be empty
			startRemoveSentenceNr != globalVariables_->removeSentenceNr &&
			// All items of this sentence are deleted
			adminItem_->highestFoundSentenceNr( false, true, startRemoveSentenceNr ) < startRemoveSentenceNr )
				{
				// So, decrement all higher sentence numbers
				adminItem_->decrementSentenceNrs( startRemoveSentenceNr );

				// First user sentence
				if( firstSentenceNrOfCurrentUser() == startRemoveSentenceNr )
					adminItem_->decrementCurrentSentenceNr();
				else
					{
					globalVariables_->currentSentenceNr = adminItem_->highestFoundSentenceNr( false, false, globalVariables_->currentSentenceNr );
					// Necessary after changing current sentence number
					globalVariables_->currentSentenceItemNr = adminItem_->highestCurrentSentenceItemNr();
					}
				}
			}
		}

	void clearPredefinedMultipleWordNrInPredefinedWords()
		{
		WordItem *currentPredefinedWordItem;
		WordItem *lastPredefinedWordItem = globalVariables_->lastPredefinedWordItem;

		if( ( currentPredefinedWordItem = globalVariables_->firstPredefinedWordItem ) != NULL )
			{
			// Do for all (grammar) words
			do	currentPredefinedWordItem->predefinedMultipleWordNr = 0;
			while( currentPredefinedWordItem != lastPredefinedWordItem &&
			( currentPredefinedWordItem = currentPredefinedWordItem->nextWordItem() ) != NULL );
			}
		}

	void decrementItemNrRange( unsigned int decrementSentenceNr, unsigned int startDecrementItemNr, unsigned int decrementOffset )
		{
		if( globalVariables_->currentSentenceNr == decrementSentenceNr &&
		globalVariables_->currentSentenceItemNr > startDecrementItemNr )
			globalVariables_->currentSentenceItemNr -= decrementOffset;

		adminItem_->decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset );
		}

	bool hasAnyUserSpecification()
		{
		WordItem *currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = globalVariables_->firstSpecificationWordItem ) != NULL )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem->hasAnyUserSpecification() &&
				!currentSpecificationWordItem->isAuthorizationRequiredForChanges() )
					return true;
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem->nextSpecificationWordItem ) != NULL );
			}

		return false;
		}

	bool isGrammarChar( char grammarChar )
		{
		return ( grammarChar == QUERY_WORD_TYPE_CHAR ||
				grammarChar == QUERY_PARAMETER_CHAR ||
				grammarChar == GRAMMAR_WORD_DEFINITION_CHAR );
		}

	signed char addGrammar( char *grammarString )
		{
		bool hasCreatedInterface = false;
		bool hasFoundChoiceAlternatives = false;
		bool hasFoundOnlyOptions = true;
		bool hasFoundPipe = false;
		bool hasFoundWordDefinitionInfo;
		bool hasGrammarWords = false;
		bool hasParameter = false;
		bool hasWordTypeNr = false;
		bool isChoice = false;
		bool isChoiceCheck = false;
		bool isChoiceStart = false;
		bool isEndOfLine = false;
		bool isLastPartOfMultipleWord = false;
		bool isMergedWord = false;
		bool isMultipleWord = false;
		bool isNewStart = true;
		bool isOption = false;
		bool isOptionCheck = false;
		bool isOptionStart = false;
		bool isSkipOptionForWriting = false;
		unsigned short grammarParameter = NO_GRAMMAR_PARAMETER;
		unsigned short predefinedMultipleWordNr = 0;
		unsigned short wordTypeNr = NO_WORD_TYPE_NR;
		unsigned int firstCreationItemNr = NO_ITEM_NR;
		size_t grammarPosition = 0;
		size_t grammarStringLength;
		GrammarItem *definitionGrammarItem = NULL;
		GrammarItem *foundGrammarItem;
		WordItem *createdWordItem;
		WordItem *foundWordItem;
		GrammarResultType grammarResult;
		ReadWordResultType readWordResult;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addGrammar";

		if( grammarString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given grammar string is undefined" );

		if( ( grammarStringLength = strlen( grammarString ) ) == 0 )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given grammar string is empty" );

		if( isPredefinedMultipleWord_ )
			{
			isPredefinedMultipleWord_ = false;
			clearPredefinedMultipleWordNrInPredefinedWords();
			}

		do	{
			if( ( readWordResult = adminItem_->readWordFromString( true, isMergedWord, false, 0, &grammarString[grammarPosition] ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a word from the grammar string" );

			hasFoundWordDefinitionInfo = false;
			readWordResult.offset += grammarPosition;

			switch( grammarString[grammarPosition] )
				{
				case QUERY_WORD_TYPE_CHAR:
					if( hasWordTypeNr )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found more than one word type parameters defined in a grammar definition line" );

					while( grammarPosition + 1 < grammarStringLength &&
					isdigit( grammarString[grammarPosition + 1] ) )
						{
						hasWordTypeNr = true;
						wordTypeNr = ( wordTypeNr * 10 + grammarString[++grammarPosition] - '0' );
						}

					if( !hasWordTypeNr )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to get the word type number from a grammar definition line" );

					break;

				case QUERY_PARAMETER_CHAR:
					if( hasParameter )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found more than one values parameters defined in a grammar definition line" );

					// Get parameter from string
					while( grammarPosition + 1 < grammarStringLength &&
					isdigit( grammarString[grammarPosition + 1] ) )
						{
						hasParameter = true;
						grammarParameter = ( grammarParameter * 10 + grammarString[++grammarPosition] - '0' );

						if( grammarParameter == WORD_MERGED_WORD )
							isMergedWord = true;
						}

					if( !hasParameter )
						return adminItem_->startError( functionNameString, moduleNameString_, "I failed to get the grammar parameter from a grammar definition line" );

					break;

				case GRAMMAR_WORD_DEFINITION_CHAR:
					if( hasCreatedInterface )
						return adminItem_->startError( functionNameString, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );

					// Add grammar word or grammar definition word
					if( firstCreationItemNr != NO_ITEM_NR )
						return adminItem_->startError( functionNameString, moduleNameString_, "A grammar definition word must be the first word in the grammar definition" );

					if( ( readWordResult = adminItem_->readWordFromString( false, isMergedWord, false, 0, &grammarString[++grammarPosition] ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a word definition from the grammar string" );

					readWordResult.offset += grammarPosition;

					if( ( grammarResult = adminItem_->findGrammarOfCurrentLanguage( ( grammarParameter >= GRAMMAR_SENTENCE ), grammarParameter, readWordResult.wordLength, &grammarString[grammarPosition] ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a grammar definition item" );

					hasFoundWordDefinitionInfo = true;
					foundGrammarItem = grammarResult.grammarItem;

					if( !hasWordTypeNr ||
					foundGrammarItem == NULL ||
					grammarParameter >= GRAMMAR_SENTENCE )
						{
						if( ( grammarResult = adminItem_->createGrammarItemForCurrentLanguage( true, ( hasParameter && grammarParameter < GRAMMAR_SENTENCE ), false, false, false, wordTypeNr, grammarParameter, readWordResult.wordLength, &grammarString[grammarPosition], NULL ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a grammar definition word item" );

						firstCreationItemNr = globalVariables_->currentSentenceItemNr;
						definitionGrammarItem = grammarResult.grammarItem;
						}
					else
						{
						if( inputOutput_->writeInterfaceText( INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_START, foundGrammarItem->grammarParameter(), INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_MIDDLE, adminItem_->currentLanguageNameString(), INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_END ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'grammar parameter defined more than once' interface notification" );
						}

					break;

				case GRAMMAR_OPTION_READ_NOT_WRITE_START:
					isSkipOptionForWriting = true;

					// Don't insert a break statement here

				case GRAMMAR_OPTION_START:
					if( hasWordTypeNr &&
					grammarParameter < GRAMMAR_SENTENCE )
						return adminItem_->startError( functionNameString, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

					if( globalVariables_->currentSentenceItemNr <= NO_ITEM_NR )
						return adminItem_->startError( functionNameString, moduleNameString_, "A grammar definition must start with a grammar definition word" );

					if( isOption )
						return adminItem_->startError( functionNameString, moduleNameString_, "A grammar opion definition can not be nested" );

					if( hasFoundPipe )
						return adminItem_->startError( functionNameString, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );

					isOption = true;
					isNewStart = true;
					isOptionStart = true;
					isChoiceCheck = isChoice;

					break;

				case GRAMMAR_OPTION_READ_NOT_WRITE_END:
					isSkipOptionForWriting = false;

					// Don't insert a break statement here

				case GRAMMAR_OPTION_END:
					if( !isOption )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found an extra square bracket character in the grammar definition" );

					if( isChoiceCheck == isChoice )
						{
						if( hasFoundPipe )
							return adminItem_->startError( functionNameString, moduleNameString_, "I found an extra pipe character before a square bracket in the grammar definition" );

						isOption = false;
						isNewStart = true;
						adminItem_->markGrammarOfCurrentLanguageAsOptionEnd();
						}
					else
						{
						if( isChoice )
							return adminItem_->startError( functionNameString, moduleNameString_, "Choices are started within an option" );

						return adminItem_->startError( functionNameString, moduleNameString_, "Choices are ended within an option" );
						}

					break;

				case GRAMMAR_CHOICE_START:
					if( hasWordTypeNr &&
					grammarParameter < GRAMMAR_SENTENCE )
						return adminItem_->startError( functionNameString, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

					if( globalVariables_->currentSentenceItemNr <= NO_ITEM_NR )
						return adminItem_->startError( functionNameString, moduleNameString_, "A grammar definition must start with a grammar definition word" );

					if( isChoice )
						return adminItem_->startError( functionNameString, moduleNameString_, "Nesting curved brackets isn't allowed" );

					if( hasFoundPipe )
						return adminItem_->startError( functionNameString, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );

					isChoice = true;
					isNewStart = true;
					isChoiceStart = true;
					isOptionCheck = isOption;

					break;

				case GRAMMAR_CHOICE_END:
					if( !isChoice )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found an extra curved bracket character in the grammar definition" );

					if( isOptionCheck == isOption )
						{
						if( hasFoundPipe )
							return adminItem_->startError( functionNameString, moduleNameString_, "I found an extra pipe character before a curved bracket in the grammar definition" );

						if( !hasFoundChoiceAlternatives )
							return adminItem_->startError( functionNameString, moduleNameString_, "A grammar definition choice must have alternatives" );

						isChoice = false;
						isNewStart = true;
						hasFoundChoiceAlternatives = false;
						adminItem_->markGrammarOfCurrentLanguageAsChoiceEnd();
						}
					else
						{
						if( isOption )
							return adminItem_->startError( functionNameString, moduleNameString_, "An option is started within choices" );

						return adminItem_->startError( functionNameString, moduleNameString_, "An option is ended within choices" );
						}

					break;

				case SYMBOL_PIPE:
					if( hasWordTypeNr &&
					grammarParameter < GRAMMAR_SENTENCE )
						return adminItem_->startError( functionNameString, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

					if( hasFoundPipe )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found an extra pipe character in the grammar definition" );

					if( !isChoice &&
					!isOption )
						return adminItem_->startError( functionNameString, moduleNameString_, "Pipes are only allowed within grammar definition options or choices" );

					hasFoundPipe = true;

					if( isChoice )
						hasFoundChoiceAlternatives = true;

					break;

				// Interface definition
				case SYMBOL_DOUBLE_QUOTE:
					if( !isNewStart ||
					hasGrammarWords ||
					isChoice ||
					isOption )
						return adminItem_->startError( functionNameString, moduleNameString_, "Grammar definition and interface definitions can not be mixed" );

					// Interface definition
					if( definitionGrammarItem == NULL )
						{
						if( !hasParameter )
							return adminItem_->startError( functionNameString, moduleNameString_, "An interface definition must have a parameter" );

						grammarPosition++;

						if( grammarPosition >= grammarStringLength ||
						grammarString[ grammarStringLength - 1 ] != SYMBOL_DOUBLE_QUOTE )
							return adminItem_->startError( functionNameString, moduleNameString_, "I found a corrupted interface definition" );

						if( adminItem_->checkInterfaceOfCurrentLanguage( grammarParameter, &grammarString[grammarPosition] ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an interface definition word item" );

						if( adminItem_->createInterfaceForCurrentLanguage( grammarParameter, ( grammarStringLength - grammarPosition - 1 ), &grammarString[grammarPosition] ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add an interface definition word item" );

						hasCreatedInterface = true;
						grammarPosition = ( grammarStringLength - 1 );
						}

					break;

				default:
					if( hasCreatedInterface )
						return adminItem_->startError( functionNameString, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );

					if( globalVariables_->currentSentenceItemNr <= NO_ITEM_NR )
						return adminItem_->startError( functionNameString, moduleNameString_, "The first grammar word in a grammar definition is the grammar definition word and must start with a grammar character" );

					if( hasParameter &&
					grammarParameter < GRAMMAR_SENTENCE )
						{
						if( grammarParameter > NO_GRAMMAR_PARAMETER )
							{
							if( isMergedWord ||
							grammarParameter == WORD_PLURAL_NOUN_ENDING ||
							// Typical for Chinese
							grammarParameter == WORD_FEMININE_PROPER_NOUN_ENDING ||
							// Typical for Chinese
							grammarParameter == WORD_MASCULINE_PROPER_NOUN_ENDING )
								{
								if( definitionGrammarItem == NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "The grammar definition item is undefined" );

								if( ( grammarResult = adminItem_->createGrammarItemForCurrentLanguage( false, false, false, false, false, NO_WORD_TYPE_NR, grammarParameter, readWordResult.wordLength, &grammarString[grammarPosition], definitionGrammarItem ) ).result != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a grammar definition word item" );

								if( definitionGrammarItem->nextDefinitionGrammarItem != NULL )
									return adminItem_->startError( functionNameString, moduleNameString_, "The next definition grammar item is already defined" );

								definitionGrammarItem->nextDefinitionGrammarItem = grammarResult.grammarItem;
								}
							else
								{
								foundWordItem = NULL;

								if( ( readWordResult.wordLength + grammarPosition ) < grammarStringLength )
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
									if( ( wordResult = adminItem_->addWord( false, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, grammarParameter, wordTypeNr, readWordResult.wordLength, &grammarString[grammarPosition] ) ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a grammar word" );

									if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
										return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create a grammar word" );

									if( isMultipleWord )
										{
										isPredefinedMultipleWord_ = true;
										createdWordItem->predefinedMultipleWordNr = ++predefinedMultipleWordNr;

										if( isLastPartOfMultipleWord )
											{
											if( ( wordResult = addPredefinedMultipleWord( predefinedMultipleWordNr, grammarParameter, wordTypeNr, foundWordItem ) ).result != RESULT_OK )
												return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a predefined multiple grammar word" );

											if( ( createdWordItem = wordResult.createdWordItem ) == NULL )
												return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't create a predefined multiple grammar word" );
											}
										}

									if( isEndOfLine &&
									setPredefinedWord( grammarParameter, createdWordItem ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to set a predefined word" );
									}
								else
									{
									if( foundWordItem->addWordType( false, false, ( adminItem_->isAdjectiveParameter( grammarParameter ) ? grammarParameter : NO_ADJECTIVE_PARAMETER ), ( adminItem_->isDefiniteArticleParameter( grammarParameter ) ? grammarParameter : NO_DEFINITE_ARTICLE_PARAMETER ), ( adminItem_->isIndefiniteArticleParameter( grammarParameter ) ? grammarParameter : NO_INDEFINITE_ARTICLE_PARAMETER ), wordTypeNr, readWordResult.wordLength, &grammarString[grammarPosition] ).result != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add another word type to grammar word \"", foundWordItem->anyWordTypeString(), "\"" );

									if( grammarParameter == WORD_PARAMETER_SYMBOL_COLON &&
									grammarString[grammarPosition] != SYMBOL_COLON )
										adminItem_->setCurrentLanguageAsChinese();
									}
								}
							}
						}
					else
						{
						// Get grammar identification
						if( ( grammarResult = adminItem_->findGrammarOfCurrentLanguage( true, NO_GRAMMAR_PARAMETER, readWordResult.wordLength, &grammarString[grammarPosition] ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a grammar definition word item" );

						foundGrammarItem = grammarResult.grammarItem;

						if( adminItem_->createGrammarItemForCurrentLanguage( false, ( hasFoundPipe || isNewStart ), isOptionStart, isChoiceStart, isSkipOptionForWriting, NO_WORD_TYPE_NR, NO_GRAMMAR_PARAMETER, readWordResult.wordLength, &grammarString[grammarPosition], foundGrammarItem ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a grammar item" );

						isNewStart = false;
						isOptionStart = false;
						isChoiceStart = false;

						hasFoundOnlyOptions = false;
						hasFoundPipe = false;
						hasGrammarWords = true;

						if( foundGrammarItem != NULL &&
						!foundGrammarItem->hasCurrentCreationSentenceNr() )
							foundGrammarItem->isGrammarItemInUse = true;
						}
				}

			if( !hasFoundWordDefinitionInfo &&
			!readWordResult.hasFoundGrammarDefinition &&
			readWordResult.offset < grammarStringLength )
				{
				// Read next word from string
				if( ( readWordResult = adminItem_->readWordFromString( false, false, false, 0, &grammarString[grammarPosition] ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a new word from the grammar string" );

				readWordResult.offset += grammarPosition;
				}

			grammarPosition = readWordResult.offset;
			}
		while( readWordResult.offset < grammarStringLength );

		if( isOption )
			return adminItem_->startError( functionNameString, moduleNameString_, "The grammar definition option isn't closed" );

		if( isChoice )
			return adminItem_->startError( functionNameString, moduleNameString_, "The grammar definition choice isn't closed" );

		if( hasFoundPipe )
			return adminItem_->startError( functionNameString, moduleNameString_, "The grammar definition ended with an open pipe" );

		if( definitionGrammarItem != NULL )
			{
			if( hasFoundOnlyOptions &&

			( !hasWordTypeNr ||
			grammarParameter >= GRAMMAR_SENTENCE ) )
				{
				if( hasGrammarWords )
					return adminItem_->startError( functionNameString, moduleNameString_, "The grammar definition only exists of options" );

				return adminItem_->startError( functionNameString, moduleNameString_, "The grammar definition only exists of a grammar definition word" );
				}

			// Remove possible duplicate grammar definition
			if( adminItem_->checkForDuplicateGrammarDefinitionInCurrentLanguage() != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check for a duplicate grammar definition" );

			if( adminItem_->linkLaterDefinedGrammarWordsInCurrentLanguage() != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to link later defined grammar words" );
			}

		return RESULT_OK;
		}

	WordResultType addPredefinedMultipleWord( unsigned short nMultipleWords, unsigned short wordParameter, unsigned short wordTypeNr, WordItem *multipleWordItem )
		{
		unsigned short predefinedMultipleWordNr = 0;
		WordItem *foundMultipleWordItem;
		WordTypeItem *foundWordTypeItem;
		char multipleWordString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addPredefinedMultipleWord";

		wordResult.createdWordItem = multipleWordItem;

		if( nMultipleWords <= 0 )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given number of multiple words is undefined" );

		while( predefinedMultipleWordNr < nMultipleWords &&
		( foundMultipleWordItem = predefinedMultipleWordItem( ++predefinedMultipleWordNr ) ) != NULL )
			{
			if( ( foundWordTypeItem = foundMultipleWordItem->activeWordTypeItem( wordTypeNr ) ) != NULL )
				{
				if( predefinedMultipleWordNr > 1 )
					strcat( multipleWordString, SPACE_STRING );

				strcat( multipleWordString, foundWordTypeItem->itemString() );
				}
			}

		if( multipleWordItem != NULL )
			{
			if( multipleWordItem->hasCurrentCreationSentenceNr() )
				{
				if( ( wordResult = adminItem_->addWord( false, true, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordParameter, wordTypeNr, strlen( multipleWordString ), multipleWordString ) ).result != RESULT_OK )
					return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to add a predefined grammar word" );
				}
			else
				{
				if( multipleWordItem->addWordType( true, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, strlen( multipleWordString ), multipleWordString ).result != RESULT_OK )
					return adminItem_->addWordResultError( functionNameString, moduleNameString_, "The given multiple word item is undefined" );
				}
			}

		if( wordResult.createdWordItem == NULL )
			return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The multiple word item is undefined" );

		predefinedMultipleWordNr = 0;

		while( predefinedMultipleWordNr < nMultipleWords &&
		( foundMultipleWordItem = predefinedMultipleWordItem( ++predefinedMultipleWordNr ) ) != NULL )
			{
			if( foundMultipleWordItem->addMultipleWord( nMultipleWords, wordTypeNr, wordResult.createdWordItem ) != RESULT_OK )
				return adminItem_->addWordResultError( functionNameString, moduleNameString_, "I failed to add a multiple word" );
			}

		return wordResult;
		}

	signed char authorizeWord( WordItem *authorizationWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "authorizeWord";

		if( authorizationWordItem == NULL )
			return adminItem_->startSystemError( functionNameString, moduleNameString_, "The given authorization word item is undefined" );

		// Only accept authorization of words during startup
		if( !adminItem_->isSystemStartingUp() )
			return adminItem_->startError( functionNameString, moduleNameString_, "You are not authorized to authorize the given word" );

		if( authorizationWordItem->assignChangePermissions( moduleNameString_ ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign my authorization permissions to a word" );

		return RESULT_OK;
		}

	signed char closeCurrentFileItem( FileItem *closeFileItem )
		{
		bool isTestFile;
		unsigned int testFileSentenceNr;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "closeCurrentFileItem";

		// Get info before closing the file
		isTestFile = adminItem_->isCurrentFileTestFile();
		testFileSentenceNr = adminItem_->currentFileSentenceNr();

		if( adminItem_->closeCurrentFile( closeFileItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to close a file" );

		if( isTestFile )
			adminItem_->deleteSentences( testFileSentenceNr );
		else
			{
			if( globalVariables_->hasDisplayedWarning ||
			globalVariables_->result != RESULT_OK )
				{
				hasClosedFileDueToError_ = true;
				globalVariables_->hasDisplayedWarning = false;
				}
			}

		return RESULT_OK;
		}

	signed char createLanguageSpecification( WordItem *languageNounWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createLanguageSpecification";

		if( languageNounWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given language noun word item is undefined" );

		if( addSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_PROPER_NOUN, WORD_TYPE_NOUN_SINGULAR, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, globalVariables_->currentLanguageWordItem, languageNounWordItem, NULL, NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a new language specification" );

		return RESULT_OK;
		}

	signed char createLanguageWord( char *languageNameString )
		{
		WordItem *languageWordItem;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "createLanguageWord";

		if( ( wordResult = findLanguageByName( languageNameString ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the language" );

		if( wordResult.foundWordItem == NULL )
			{
			globalVariables_->currentLanguageNr++;

			if( ( wordResult = adminItem_->addWord( true, false, NO_ADJECTIVE_PARAMETER, NO_DEFINITE_ARTICLE_PARAMETER, NO_INDEFINITE_ARTICLE_PARAMETER, NO_WORD_PARAMETER, WORD_TYPE_PROPER_NOUN, strlen( languageNameString ), languageNameString ) ).result != RESULT_OK )
				{
				// On error, restore old language
				globalVariables_->currentLanguageNr--;
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a language word" );
				}

			if( ( languageWordItem = wordResult.createdWordItem ) == NULL )
				return adminItem_->startError( functionNameString, moduleNameString_, "The created language word item is undefined" );

			if( authorizeWord( languageWordItem ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to authorize the language word" );

			globalVariables_->currentLanguageWordItem = languageWordItem;
			}
		else
			globalVariables_->currentLanguageWordItem = wordResult.foundWordItem;

		return RESULT_OK;
		}

	signed char executeLine( char *readString )
		{
		bool hasSwitchedLanguage = false;
		bool wasQueryCommand = false;
		bool wasUndoOrRedoCommand;
		SpecificationItem *userSpecificationItem;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "executeLine";

		if( readString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read string is undefined" );

		// Skip empty line
		if( strlen( readString ) > 0 &&
		// and comment line
		readString[0] != COMMENT_CHAR )
			{
			globalVariables_->hasDisplayedIntegrityWarning = false;
			globalVariables_->hasDisplayedMessage = false;
			globalVariables_->hasDisplayedWarning = false;
			globalVariables_->isAssignmentChanged = false;
			globalVariables_->isConflictingQuestion = false;

			if( !adminItem_->wasPreviousCommandUndoOrRedo() )
				cleanupDeletedItems();

			hasClosedFileDueToError_ = false;

			// Reading grammar/interface file, changing language or a query
			if( readString[0] == QUERY_CHAR )
				{
				// Grammar/interface
				if( isalpha( readString[1] ) )
					{
					if( adminItem_->isSystemStartingUp() )
						{
						if( readGrammarFileAndUserInterfaceFile( &readString[1] ) != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to read the language" );
						}
					else
						{
						// Change language
						if( assignLanguage( &readString[1] ) == RESULT_OK )
							hasSwitchedLanguage = true;
						else
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language" );
						}
					}
				else
					{
					// Query
					if( readString[0] == QUERY_CHAR )
						{
						wasQueryCommand = true;

						if( adminItem_->executeQuery( false, true, true, INPUT_OUTPUT_PROMPT_QUERY, 1, readString ).result != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute query: \"", readString, "\"" );
						}
					}
				}
			else
				{
				// Sentence or grammar definition
				if( incrementCurrentSentenceNr() != RESULT_OK )
					adminItem_->addError( functionNameString, moduleNameString_, "I failed to increment the current sentence number" );

				// Grammar definition
				if( isGrammarChar( readString[0] ) )
					{
					if( addGrammar( readString ) != RESULT_OK )
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to add grammar: \"", readString, "\"" );
					}
				else
					{
					// Sentence
					if( adminItem_->processReadSentence( readString ) != RESULT_OK )
						adminItem_->addError( functionNameString, moduleNameString_, "I failed to process a read sentence" );
					}
				}

			if( globalVariables_->result != RESULT_SYSTEM_ERROR &&
			!adminItem_->hasRequestedRestart() )
				{
				wasUndoOrRedoCommand = adminItem_->wasUndoOrRedoCommand();

				if( globalVariables_->result == RESULT_OK &&
				!globalVariables_->hasDisplayedWarning )
					{
					if( adminItem_->hasAnyChangeBeenMadeByThisSentence() )
						{
						if( !hasSwitchedLanguage &&

						( wasUndoOrRedoCommand ||

						( !globalVariables_->hasDisplayedMessage &&
						( userSpecificationItem = adminItem_->userSpecificationItem() ) != NULL &&
						userSpecificationItem->isTriggeringExecutionOfSelections() ) ) )
							{
							if( adminItem_->executeSelections() != RESULT_OK )
								adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute selections after reading the sentence" );
							}
						}
					else
						{
						if( !hasSwitchedLanguage &&
						!wasLoginCommand_ &&
						!globalVariables_->hasDisplayedMessage &&

						inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != RESULT_OK )
							adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I know' interface notification" );
						}
					}

				adminItem_->clearTemporaryAdminLists();

				if( !wasQueryCommand &&
				!wasUndoOrRedoCommand )
					{
					cleanupDeletedItems();

					if( !adminItem_->isSystemStartingUp() )
						{
						// Check for empty sentence
						if( ( globalVariables_->currentSentenceItemNr = adminItem_->highestCurrentSentenceItemNr() ) == NO_ITEM_NR )
							adminItem_->decrementCurrentSentenceNr();
						}
					}
				}
			}

		return RESULT_OK;
		}

	signed char incrementCurrentSentenceNr()
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "incrementCurrentSentenceNr";

		if( globalVariables_->currentSentenceNr >= MAX_SENTENCE_NR )
			return adminItem_->startSystemError( functionNameString, moduleNameString_, "Sentence number overflow! I can't except anymore input" );

		globalVariables_->currentSentenceNr++;
		// Necessary after changing current sentence number
		globalVariables_->currentSentenceItemNr = adminItem_->highestCurrentSentenceItemNr();

		return RESULT_OK;
		}

	signed char readLanguageFile( bool isGrammarFile, char *languageNameString )
		{
		FileItem *openedLanguageFileItem;
		FileResultType fileResult;
		signed char originalResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readLanguageFile";

		if( languageNameString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given language name is undefined" );

		if( ( fileResult = adminItem_->openFile( true, false, false, false, ( isGrammarFile ? FILE_DATA_GRAMMAR_DIRECTORY_NAME_STRING : FILE_DATA_INTERFACE_DIRECTORY_NAME_STRING ), languageNameString, NULL, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, ( isGrammarFile ? "I failed to open the grammar file: \"" : "I failed to open the interface file: \"" ), languageNameString, "\"" );

		if( ( openedLanguageFileItem = fileResult.createdFileItem ) != NULL )
			{
			if( createLanguageWord( languageNameString ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create language word: \"", languageNameString, "\"" );

			// This isn't the first language
			if( predefinedNounLanguageWordItem_ != NULL &&
			createLanguageSpecification( predefinedNounLanguageWordItem_ ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a language specification" );

			if( readAndExecute() != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened language file" );

			originalResult = globalVariables_->result;

			if( closeCurrentFileItem( openedLanguageFileItem ) != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the language file item" );

			if( originalResult != RESULT_OK )
				globalVariables_->result = originalResult;
			}
		else
			return adminItem_->startError( functionNameString, moduleNameString_, ( isGrammarFile ? "I couldn't open the grammar file: \"" : "I couldn't open the interface file: \"" ), languageNameString, "\"" );

		return RESULT_OK;
		}

	signed char readGrammarFileAndUserInterfaceFile( char *readLanguageNameString )
		{
		WordItem *currentLanguageWordItem;
		char *startupLanguageNameString = adminItem_->startupLanguageNameString();
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readGrammarFileAndUserInterfaceFile";

		if( readLanguageNameString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given read language name string is undefined" );

		// Read the user-interface file
		if( readLanguageFile( false, readLanguageNameString ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read an interface file" );

		// Read the grammar file
		if( readLanguageFile( true, readLanguageNameString ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a grammar file" );

		// If the system is restarted
		if( startupLanguageNameString != NULL &&
		predefinedNounStartupLanguageWordItem_ != NULL &&
		( currentLanguageWordItem = globalVariables_->currentLanguageWordItem ) != NULL &&
		// The current language (of which the grammar file and the interface file were read above),
		// is the last selected language before the restart
		strcmp( startupLanguageNameString, adminItem_->currentLanguageNameString() ) == 0 )
			{
			// Create language specification for the startup language word
			if( addSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, WORD_TYPE_NOUN_SINGULAR, WORD_TYPE_PROPER_NOUN, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, predefinedNounStartupLanguageWordItem_, currentLanguageWordItem, NULL, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a predefined noun startup language specification" );

			// Assign the created startup language word to the current language
			if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, predefinedNounStartupLanguageWordItem_, currentLanguageWordItem, NULL ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the predefined noun startup language word" );
			}

		return RESULT_OK;
		}

	signed char readLine( bool isPassword, bool isQuestion, unsigned int promptSentenceNr, char *promptUserNameString, char *readString )
		{
		char promptString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readLine";

		// Don't show sentence number when asking for user name, password, and so on
		if( promptSentenceNr > NO_SENTENCE_NR )
			sprintf( promptString, "%u", promptSentenceNr );

		if( promptUserNameString != NULL )
			{
			if( strlen( promptString ) > 0 )
				strcat( promptString, QUERY_SEPARATOR_STRING );

			strcat( promptString, promptUserNameString );
			}

		if( inputOutput_->readLine( adminItem_->isDisplayingLine(), isPassword, isQuestion, promptString, readString, adminItem_->currentReadFile() ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a line from a file or from input" );

		return RESULT_OK;
		}

	signed char setPredefinedWord( unsigned short grammarParameter, WordItem *createdWordItem )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "setPredefinedWord";

		if( createdWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given created word item" );

		switch( grammarParameter )
			{
			case WORD_PARAMETER_ADJECTIVE_BUSY:
				if( predefinedAdjectiveBusyWordItem_ == NULL )
					predefinedAdjectiveBusyWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveBusyWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined busy adjective word" );
					}

				break;

			case WORD_PARAMETER_ADJECTIVE_DONE:
				if( predefinedAdjectiveDoneWordItem_ == NULL )
					predefinedAdjectiveDoneWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveDoneWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined done adjective word" );
					}

				break;

			case WORD_PARAMETER_ADJECTIVE_INVERTED:
				if( predefinedAdjectiveInvertedWordItem_ == NULL )
					predefinedAdjectiveInvertedWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveInvertedWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined inverted adjective word" );
					}

				break;

			case WORD_PARAMETER_NOUN_LANGUAGE:
				if( predefinedNounLanguageWordItem_ == NULL )
					{
					predefinedNounLanguageWordItem_ = createdWordItem;
					globalVariables_->predefinedNounLanguageWordItem = createdWordItem;

					// During the creation of the first language,
					// the predefined noun language word wasn't defined yet
					// So, create first language specification afterwards
					if( createLanguageSpecification( createdWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a language specification" );
					}

				break;

			case WORD_PARAMETER_NOUN_PASSWORD:
				if( predefinedNounPasswordWordItem_ == NULL )
					{
					predefinedNounPasswordWordItem_ = createdWordItem;

					if( authorizeWord( predefinedNounPasswordWordItem_ ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to authorize the predefined password noun word" );
					}
				else
					{
					if( predefinedNounPasswordWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined password noun word" );
					}

				break;

			case WORD_PARAMETER_NOUN_SOLVE_LEVEL:
				if( predefinedNounSolveLevelWordItem_ == NULL )
					predefinedNounSolveLevelWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveLevelWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined solve level noun word" );
					}

				break;

			case WORD_PARAMETER_NOUN_SOLVE_METHOD:
				if( predefinedNounSolveMethodWordItem_ == NULL )
					predefinedNounSolveMethodWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveMethodWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined solve method noun word" );
					}

				break;

			case WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
				if( predefinedNounSolveStrategyWordItem_ == NULL )
					predefinedNounSolveStrategyWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveStrategyWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined solve strategy noun word" );
					}

				break;

			case WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
				if( predefinedNounStartupLanguageWordItem_ == NULL )
					predefinedNounStartupLanguageWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounStartupLanguageWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined startup language word" );
					}

				break;

			case WORD_PARAMETER_NOUN_USER:
				if( predefinedNounUserWordItem_ == NULL )
					{
					predefinedNounUserWordItem_ = createdWordItem;
					globalVariables_->predefinedNounUserWordItem = createdWordItem;

					if( authorizeWord( predefinedNounUserWordItem_ ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to authorize the predefined user noun word" );
					}
				else
					{
					if( predefinedNounUserWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined user noun word" );
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( predefinedVerbLoginWordItem_ == NULL )
					{
					predefinedVerbLoginWordItem_ = createdWordItem;

					if( authorizeWord( predefinedVerbLoginWordItem_ ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to authorize the predefined login verb word" );
					}
				else
					{
					if( predefinedVerbLoginWordItem_ != createdWordItem )
						return adminItem_->startError( functionNameString, moduleNameString_, "I found a different predefined login verb word" );
					}

				break;
			}

		return RESULT_OK;
		}

	WordResultType findLanguageByName( char *languageNameString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "findLanguageByName";

		if( languageNameString != NULL )

			// Initially, the language name words are not linked to
			// the language defintion word. So, search in all words
			return adminItem_->findWordTypeInAllWords( true, WORD_TYPE_PROPER_NOUN, languageNameString, NULL );

		return adminItem_->startWordResultError( functionNameString, moduleNameString_, "The given language name string is undefined" );
		}

	WordItem *predefinedMultipleWordItem( unsigned short predefinedMultipleWordNr )
		{
		WordItem *currentWordItem;

		if( predefinedMultipleWordNr > 0 &&
		( currentWordItem = globalVariables_->firstWordItem ) != NULL )
			{
			// Do for all (grammar) words
			do	{
				if( currentWordItem->predefinedMultipleWordNr == predefinedMultipleWordNr )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem->nextWordItem() ) != NULL );
			}

		return NULL;
		}


	protected:
	// Constructor

	AdminReadFile( AdminItem *adminItem, GlobalVariables *globalVariables, InputOutput *inputOutput )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( globalVariables != NULL )
				globalVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( globalVariables_ = globalVariables ) == NULL )
				strcpy( errorString, "The given global variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	void initializeAdminReadFileVariables()
		{
		wasLoginCommand_ = false;
		}

	unsigned int firstSentenceNrOfCurrentUser()
		{
		return ( firstSentenceNrOfCurrentUser_ + 1 );
		}

	signed char assignLanguage( char *languageNameString )
		{
		bool hasFoundLanguage = false;
		SpecificationItem *languageSpecificationItem;
		WordItem *foundLanguageWordItem;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignLanguage";

		if( ( wordResult = findLanguageByName( languageNameString ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the language" );

		if( ( foundLanguageWordItem = wordResult.foundWordItem ) != NULL )
			{
			hasFoundLanguage = true;

			if( ( languageSpecificationItem = foundLanguageWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, NO_QUESTION_PARAMETER, NO_COLLECTION_NR, predefinedNounLanguageWordItem_ ) ) != NULL )
				globalVariables_->currentLanguageNr = languageSpecificationItem->languageNr();

			if( globalVariables_->currentLanguageWordItem != foundLanguageWordItem )
				{
				if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, foundLanguageWordItem, predefinedNounLanguageWordItem_, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the language" );

				globalVariables_->currentLanguageWordItem = foundLanguageWordItem;
				}

			if( !hasFoundLanguage )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given name isn't a grammar nor an language" );
			}
		else
			{
			if( !adminItem_->isSystemStartingUp() )
				return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find the requested language" );
			}

		return RESULT_OK;
		}

	signed char compareOutputFileAgainstReferenceFile( char *testFileNameString )
		{
		bool hasReadOutput;
		bool hasReadReference;
		bool isStop = false;
		unsigned int lineNr = 0;
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;
		char outputString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char referenceString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "compareOutputFileAgainstReferenceFile";

		hasFoundDifferentTestResult_ = false;

		if( ( fileResult = adminItem_->openFile( true, false, false, true, FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, testFileNameString, NULL, FILE_DATA_REGRESSION_TEST_REFERENCE_DIRECTORY_NAME_STRING ) ).result != RESULT_OK )
			return adminItem_->startError( functionNameString, moduleNameString_, "I failed to open a test file" );

		if( fileResult.outputFile == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The output file is undefined" );

		if( fileResult.referenceFile == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The reference file is undefined" );

		do	{
			isStop = true;
			hasFoundDifferentTestResult_ = true;

			if( inputOutput_->readLine( false, false, false, NULL, outputString, fileResult.outputFile ) != RESULT_OK )
				return adminItem_->startError( functionNameString, moduleNameString_, "I failed to read a line from the output file" );

			hasReadOutput = inputOutput_->hasReadLine();

			if( inputOutput_->readLine( false, false, false, NULL, referenceString, fileResult.referenceFile ) != RESULT_OK )
				return adminItem_->startError( functionNameString, moduleNameString_, "I failed to read a line from the reference file" );

			hasReadReference = inputOutput_->hasReadLine();

			if( hasReadOutput &&
			hasReadReference )
				{
				if( strcmp( outputString, referenceString ) != 0 )
					{
					sprintf( errorString, "Line number %u is different:\n* test result:\t\t\"%s\"\n* against reference:\t\"%s\"", lineNr, outputString, referenceString );
					return adminItem_->startError( functionNameString, moduleNameString_, errorString );
					}

				isStop = false;
				lineNr++;
				}
			else
				{
				if( hasReadOutput )
					return adminItem_->startError( functionNameString, moduleNameString_, "The output file is longer than the reference file" );

				if( hasReadReference )
					return adminItem_->startError( functionNameString, moduleNameString_, "The output file is shorter than the reference file" );

				hasFoundDifferentTestResult_ = false;
				}
			}
		while( !isStop );

			if( fileResult.outputFile != NULL )
				fclose( fileResult.outputFile );

			if( fileResult.referenceFile != NULL )
				fclose( fileResult.referenceFile );

		return RESULT_OK;
		}

	signed char login( WordItem *specificationWordItem )
		{
		bool isAlreadyLoggedInAsGivenUser = false;
		bool isCorrectPassword = false;
		bool isNoPasswordRequired = false;
		GeneralizationItem *currentGeneralizationItem;
		SpecificationItem *passwordAssignmentItem = NULL;
		WordItem *currentGeneralizationWordItem;
		WordItem *foundUserWordItem = NULL;
		WordItem *passwordSpecificationWordItem;
		char readPasswordString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char readUserNameString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		WordResultType wordResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "login";

		wasLoginCommand_ = false;

		if( predefinedNounPasswordWordItem_ == NULL )
			return adminItem_->startSystemError( functionNameString, moduleNameString_, "The predefined password noun word item is undefined" );

		if( predefinedNounUserWordItem_ == NULL )
			return adminItem_->startSystemError( functionNameString, moduleNameString_, "The predefined user noun word item is undefined" );

		// No user name is given
		if( specificationWordItem == NULL &&
		// Get first user without password
		( currentGeneralizationItem = predefinedNounPasswordWordItem_->firstSpecificationGeneralizationItem( true ) ) != NULL )
			{
			// Do for all generalization specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				// Select first user in the current language
				if( currentGeneralizationWordItem->activeWordTypeItem( false, WORD_TYPE_PROPER_NOUN ) != NULL )
					specificationWordItem = currentGeneralizationWordItem;
				}
			while( specificationWordItem == NULL &&
			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		// No user name is given
		// and no user without password is found
		if( specificationWordItem == NULL )
			{
			// Ask user name
			if( readLine( false, true, NO_SENTENCE_NR, predefinedNounUserWordItem_->singularNounString(), readUserNameString ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read to user name" );
			}
		else
			strcat( readUserNameString, specificationWordItem->anyWordTypeString() );

		if( strlen( readUserNameString ) > 0 &&
		// Find user word
		( currentGeneralizationItem = predefinedNounUserWordItem_->firstSpecificationGeneralizationItem( false ) ) != NULL )
			{
			// Do for all generalization specification words
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem->generalizationWordItem() ) == NULL )
					return adminItem_->startSystemError( functionNameString, moduleNameString_, "I found an undefined generalization word" );

				if( ( wordResult = currentGeneralizationWordItem->findWordType( true, WORD_TYPE_PROPER_NOUN, readUserNameString ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find the user name" );

				if( wordResult.foundWordTypeItem != NULL )
					foundUserWordItem = currentGeneralizationWordItem;
				}
			while( foundUserWordItem == NULL &&
			( currentGeneralizationItem = currentGeneralizationItem->nextSpecificationGeneralizationItem() ) != NULL );
			}

		if( foundUserWordItem != NULL &&
		foundUserWordItem == currentUserWordItem_ )
			{
			// Already logged in as given user
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_ALREADY_LOGGED_IN_START, readUserNameString, INTERFACE_CONSOLE_LOGIN_END ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'already logged in' interface notification" );

			isAlreadyLoggedInAsGivenUser = true;
			}
		else
			{
			if( foundUserWordItem != NULL &&
			( passwordAssignmentItem = predefinedNounPasswordWordItem_->firstNonPossessiveActiveAssignmentItem( foundUserWordItem ) ) == NULL )
				{
				// No password assignment found
				// Now, check explicitly if no password is required
				if( foundUserWordItem->bestMatchingSpecificationWordSpecificationItem( false, false, false, false, true, true, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, predefinedNounPasswordWordItem_ ) != NULL )
					isNoPasswordRequired = true;
				}
			else
				{
				// Ask password
				if( readLine( true, true, NO_SENTENCE_NR, predefinedNounPasswordWordItem_->singularNounString(), readPasswordString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read to password" );

				if( passwordAssignmentItem != NULL )
					{
					if( ( passwordSpecificationWordItem = passwordAssignmentItem->specificationWordItem() ) == NULL )
						return adminItem_->startSystemError( functionNameString, moduleNameString_, "The password assignment specification item is undefined" );

					if( passwordSpecificationWordItem->isCorrectHiddenWordType( passwordAssignmentItem->specificationWordTypeNr(), readPasswordString, moduleNameString_ ) )
						isCorrectPassword = true;
					}
				}

			if( ( isCorrectPassword ||
			isNoPasswordRequired ) &&

			foundUserWordItem != NULL )
				{
				if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, foundUserWordItem, predefinedNounUserWordItem_, NULL ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the user" );

				firstSentenceNrOfCurrentUser_ = globalVariables_->currentSentenceNr;
				currentUserWordItem_ = foundUserWordItem;
				globalVariables_->currentUserNr = predefinedNounUserWordItem_->userNr( foundUserWordItem );

				if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_WELCOME_MESSAGE_EXPERT ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the welcome message for experts" );
				}
			}

		if( ( isAlreadyLoggedInAsGivenUser ||
		isCorrectPassword ||
		isNoPasswordRequired ) &&

		foundUserWordItem != NULL )
			wasLoginCommand_ = true;
		else
			{
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_CONSOLE_LOGIN_FAILED ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'login failed' interface notification" );
			}

		return RESULT_OK;
		}

	signed char readAndExecute()
		{
		bool isLineExecuted;
		char readString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readAndExecute";

		hasFoundDifferentTestResult_ = false;

		do	{
			isLineExecuted = false;
			strcpy( readString, EMPTY_STRING );

			if( readLine( false, false, ( globalVariables_->currentSentenceNr + 1 ), ( currentUserWordItem_ != NULL ? currentUserWordItem_->anyWordTypeString() : NULL ), readString ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read a line" );

			if( inputOutput_->hasReadLine() )
				{
				if( executeLine( readString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute the read line" );

				isLineExecuted = true;
				}
			}
		while( isLineExecuted &&
		!hasFoundDifferentTestResult_ &&
		!adminItem_->hasRequestedRestart() &&

		// Ignore warnings during testing
		( adminItem_->isCurrentlyTesting() ||
		!globalVariables_->hasDisplayedWarning ) );

		if( adminItem_->isSystemStartingUp() &&
		globalVariables_->hasDisplayedWarning )
			globalVariables_->result = RESULT_SYSTEM_ERROR;

		return RESULT_OK;
		}

	signed char readExampleFile( char *exampleFileNameString )
		{
		FileItem *openedExampleFileItem;
		FileResultType fileResult;
		signed char originalResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readExampleFile";

		if( exampleFileNameString != NULL )
			{
			if( ( fileResult = adminItem_->openFile( !adminItem_->isSystemStartingUp(), false, false, true, FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, exampleFileNameString, NULL, NULL ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to open an example file" );

			if( ( openedExampleFileItem = fileResult.createdFileItem ) != NULL )
				{
				if( readAndExecute() != RESULT_OK )
					adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened example file" );

				originalResult = globalVariables_->result;

				if( closeCurrentFileItem( openedExampleFileItem ) != RESULT_OK )
					adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the example file item" );

				if( originalResult != RESULT_OK )
					globalVariables_->result = originalResult;
				}
			}
		else
			{
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char readStartupFile()
		{
		FileItem *openedStartupFileItem;
		FileResultType fileResult;
		signed char originalResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readStartupFile";

		if( ( fileResult = adminItem_->openFile( true, false, false, true, FILE_DATA_STARTUP_DIRECTORY_NAME_STRING, FILE_STARTUP_NAME_STRING, NULL, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to open an startup file" );

		if( ( openedStartupFileItem = fileResult.createdFileItem ) != NULL )
			{
			if( readAndExecute() != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened startup file" );

			originalResult = globalVariables_->result;

			if( closeCurrentFileItem( openedStartupFileItem ) != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the startup file item" );

			if( originalResult != RESULT_OK )
				globalVariables_->result = originalResult;
			}

		return RESULT_OK;
		}

	signed char readTestFile( char *testFileNameString )
		{
		bool isFirstTestFile = ( !adminItem_->isCurrentlyTesting() &&

								// Regression test button
								( adminItem_->currentFileSentenceNr() == NO_SENTENCE_NR ||
								// Regression test file is selected
								adminItem_->currentFileSentenceNr() == globalVariables_->currentSentenceNr ) );
		FileItem *testFileItem;
		char testString[MAX_SENTENCE_STRING_LENGTH];
		signed char originalResult;
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readTestFile";

		// Check if user has already entered sentences
		if( hasAnyUserSpecification() )
			return adminItem_->startError( functionNameString, moduleNameString_, "Some knowledge is already entered. It may disturb the test results. Please, use restart button before testing to ensure no knowledge is present" );

		if( isFirstTestFile )
			{
			testFileNr_ = 0;
			startTime_ = clock();
			}
		else
			testFileNr_++;

		if( ( fileResult = adminItem_->openFile( true, false, true, true, FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, testFileNameString, FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, NULL ) ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to open a test file" );

		if( ( testFileItem = fileResult.createdFileItem ) == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The last created file item is undefined" );

		sprintf( testString, "Test file #%u: %s.\n", testFileNr_, testFileItem->readFileNameString() );

		if( inputOutput_->writeText( true, true, INPUT_OUTPUT_PROMPT_NOTIFICATION, NO_CENTER_WIDTH, testString ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a text" );

		inputOutput_->redirectOutputToTestFile( testFileItem->writeFile() );

		if( readAndExecute() != RESULT_OK )
			adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened test file" );

		if( ( originalResult = globalVariables_->result ) != RESULT_OK )
			globalVariables_->result = RESULT_OK;

		if( closeCurrentFileItem( testFileItem ) != RESULT_OK )
			adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the test file item" );

		if( originalResult != RESULT_OK )
			globalVariables_->result = originalResult;
		else
			{
			if( isFirstTestFile )
				{
				sprintf( testString, "Done in: %.1f sec.\n", ( ( clock() - startTime_ ) / (double)CLOCKS_PER_SEC ) );

				if( inputOutput_->writeText( true, true, INPUT_OUTPUT_PROMPT_NOTIFICATION, NO_CENTER_WIDTH, testString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the test statistics text" );
				}
			}

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

	CreateAndAssignResultType addSpecificationWithAuthorization( bool isAssignment, bool isCharacteristicFor, bool isConditional, bool isInactiveAssignment, bool isArchivedAssignment, bool isEveryGeneralization, bool isExclusiveSpecification, bool isNegative, bool isPartOf, bool isPossessive, bool isSelection, bool isSpecific, bool isSpecificationGeneralization, bool isUncountableGeneralizationNoun, bool isUniqueUserRelation, bool isValueSpecification, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, unsigned int specificationCollectionNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int copiedRelationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		CreateAndAssignResultType createAndAssignResult;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "addSpecificationWithAuthorization";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem->isNounPassword() &&
		// Hide password
		specificationWordItem->hideWordType( specificationWordTypeNr, moduleNameString_ ) != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to hide a password" );

		if( ( createAndAssignResult = generalizationWordItem->addSpecificationInWord( isAssignment, isInactiveAssignment, isArchivedAssignment, isCharacteristicFor, isConditional, isEveryGeneralization, true, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecific, isSpecificationGeneralization, isUncountableGeneralizationNoun, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, moduleNameString_ ) ).result != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to add a specification with authorization" );

		// Collect current language with previous languages
		if( adminItem_->collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, relationContextNr, generalizationWordItem, specificationWordItem ) != RESULT_OK )
			return adminItem_->addCreateAndAssignResultError( functionNameString, moduleNameString_, "I failed to collect a generalization word with a previous one" );

		return createAndAssignResult;
		}

	CreateAndAssignResultType assignSpecificationWithAuthorization( bool isAmbiguousRelationContext, bool isAssignedOrClear, bool isInactiveAssignment, bool isArchivedAssignment, bool isNegative, bool isPossessive, bool isSpecificationGeneralization, bool isUniqueUserRelation, unsigned short assumptionLevel, unsigned short prepositionParameter, unsigned short questionParameter, unsigned short relationWordTypeNr, unsigned int generalizationContextNr, unsigned int specificationContextNr, unsigned int relationContextNr, unsigned int nContextRelations, JustificationItem *firstJustificationItem, WordItem *generalizationWordItem, WordItem *specificationWordItem, char *specificationString )
		{
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "assignSpecificationWithAuthorization";

		if( generalizationWordItem == NULL )
			return adminItem_->startCreateAndAssignResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		return generalizationWordItem->assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, moduleNameString_ );
		}

	FileResultType readInfoFile( bool isReportingErrorIfFileDoesNotExist, char *infoFileNameString )
		{
		FileItem *openedInfoFileItem;
		FileResultType fileResult;
		signed char originalResult;
		char infoPathString[MAX_SENTENCE_STRING_LENGTH] = FILE_DATA_INFO_DIRECTORY_NAME_STRING;
		char functionNameString[FUNCTION_NAME_STRING_LENGTH] = "readInfoFile";

		if( infoFileNameString == NULL )
			return adminItem_->startFileResultError( functionNameString, moduleNameString_, "The given info file name string is undefined" );

		strcat( infoPathString, adminItem_->currentLanguageNameString() );
		strcat( infoPathString, SLASH_STRING );

		if( ( fileResult = adminItem_->openFile( true, true, false, isReportingErrorIfFileDoesNotExist, infoPathString, infoFileNameString, NULL, NULL ) ).result != RESULT_OK )
			return adminItem_->addFileResultError( functionNameString, moduleNameString_, "I failed to open the info file" );

		if( ( openedInfoFileItem = fileResult.createdFileItem ) != NULL )
			{
			if( readAndExecute() != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to read and execute the opened info file" );

			originalResult = globalVariables_->result;

			if( closeCurrentFileItem( openedInfoFileItem ) != RESULT_OK )
				adminItem_->addError( functionNameString, moduleNameString_, "I failed to close the info file item" );

			if( originalResult != RESULT_OK )
				globalVariables_->result = originalResult;
			}

		return fileResult;
		}
	};

/*************************************************************************
 *	"Shout to the Lord, all the earth;
 *	break out in praise and sing for joy!" (Psalm 98:4)
 *************************************************************************/
