/*	Class:			AdminReadFile
 *	Supports class:	AdminItem
 *	Purpose:		To read the grammar, user-interface and example files
 *	Version:		Thinknowlogy 2017r1 (Bursts of Laughter)
 *************************************************************************/
/*	Copyright (C) 2009-2017, Menno Mafait. Your suggestions, modifications,
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

import java.io.IOException;
import java.util.Date;

class AdminReadFile
	{
	// Private constructed variables

	private boolean hasClosedFileDueToError_;
	private boolean hasFoundDifferentTestResult_;
	private boolean isDeveloperTheCurrentUser_;
	private boolean isExpertTheCurrentUser_;
	private boolean isPredefinedMultipleWord_;
	private boolean wasLoginCommand_;

	private short testFileNr_;

	private int firstSentenceNrOfCurrentUser_;

	long startTime_;

	private WordItem currentUserWordItem_;
	private WordItem predefinedAdjectiveBusyWordItem_;
	private WordItem predefinedAdjectiveDoneWordItem_;
	private WordItem predefinedAdjectiveInvertedWordItem_;
	private WordItem predefinedNounDeveloperWordItem_;
	private WordItem predefinedNounExpertWordItem_;
	private WordItem predefinedNounLanguageWordItem_;
	private WordItem predefinedNounPasswordWordItem_;
	private WordItem predefinedNounSolveLevelWordItem_;
	private WordItem predefinedNounSolveMethodWordItem_;
	private WordItem predefinedNounSolveStrategyWordItem_;
	private WordItem predefinedNounStartupLanguageWordItem_;
	private WordItem predefinedNounUserWordItem_;
	private WordItem predefinedVerbLoginWordItem_;


	// Private initialized variables

	private String moduleNameString_;

	private AdminItem adminItem_;


	// Private methods

	private void cleanupDeletedItems()
		{
		int firstSentenceNr;
		int startRemoveSentenceNr = Constants.NO_SENTENCE_NR;
		long startNanoTime = System.nanoTime();

		if( !hasClosedFileDueToError_ &&

		( CommonVariables.hasDisplayedWarning ||
		CommonVariables.hasDisplayedIntegrityWarning ||
		CommonVariables.result != Constants.RESULT_OK ) )
			adminItem_.deleteSentences( CommonVariables.currentSentenceNr );

		do	{
			CommonVariables.nDeletedItems = 0;
			CommonVariables.removeSentenceNr = Constants.NO_SENTENCE_NR;
			CommonVariables.removeStartItemNr = Constants.NO_ITEM_NR;

			adminItem_.removeFirstRangeOfDeletedItems();

			if( CommonVariables.nDeletedItems > 0 )
				{
				decrementItemNrRange( CommonVariables.removeSentenceNr, CommonVariables.removeStartItemNr, CommonVariables.nDeletedItems );
				startRemoveSentenceNr = CommonVariables.removeSentenceNr;
				}
			}
		while( CommonVariables.nDeletedItems > 0 );

		if( CommonVariables.hasDisplayedWarning )
			CommonVariables.hasDisplayedWarning = false;
		else
			{
			if( startRemoveSentenceNr > Constants.NO_SENTENCE_NR &&
			// Previous deleted sentence might be empty
			startRemoveSentenceNr != CommonVariables.removeSentenceNr &&
			// All items of this sentence are deleted
			adminItem_.highestInUseSentenceNr( true, true, startRemoveSentenceNr ) < startRemoveSentenceNr )
				{
				// So, decrement all higher sentence numbers
				adminItem_.decrementSentenceNrs( startRemoveSentenceNr );

				if( CommonVariables.currentSentenceNr >= startRemoveSentenceNr )
					{
					firstSentenceNr = firstSentenceNrOfCurrentUser();

					// First user sentence
					if( startRemoveSentenceNr == firstSentenceNr )
						adminItem_.decrementCurrentSentenceNr();
					else
						{
						CommonVariables.currentSentenceNr = adminItem_.highestInUseSentenceNr( false, false, CommonVariables.currentSentenceNr );
						// Necessary after changing current sentence number
						adminItem_.setCurrentItemNr();
						}
					}
				}
			}

		CommonVariables.cleaningTime += ( System.nanoTime() - startNanoTime );
		}

	private static void clearPredefinedMultipleWordNrInPredefinedWords()
		{
		WordItem currentPredefinedWordItem;
		WordItem lastPredefinedWordItem = CommonVariables.lastPredefinedWordItem;

		if( ( currentPredefinedWordItem = CommonVariables.firstPredefinedWordItem ) != null )
			{
			// Do for all (grammar) words
			do	currentPredefinedWordItem.predefinedMultipleWordNr = 0;
			while( currentPredefinedWordItem != lastPredefinedWordItem &&
			( currentPredefinedWordItem = currentPredefinedWordItem.nextWordItem() ) != null );
			}
		}

	private void decrementItemNrRange( int decrementSentenceNr, int startDecrementItemNr, int decrementOffset )
		{
		if( CommonVariables.currentSentenceNr == decrementSentenceNr &&
		CommonVariables.currentItemNr > startDecrementItemNr )
			CommonVariables.currentItemNr -= decrementOffset;

		adminItem_.decrementItemNrRange( decrementSentenceNr, startDecrementItemNr, decrementOffset );
		}

	private static boolean hasAnyUserSpecification()
		{
		WordItem currentSpecificationWordItem;

		if( ( currentSpecificationWordItem = CommonVariables.firstSpecificationWordItem ) != null )
			{
			// Do for all specification words
			do	{
				if( currentSpecificationWordItem.hasAnyUserSpecification() &&
				!currentSpecificationWordItem.isAuthorizationRequiredForChanges() )
					return true;
				}
			while( ( currentSpecificationWordItem = currentSpecificationWordItem.nextSpecificationWordItem ) != null );
			}

		return false;
		}

	private static boolean isGrammarChar( char grammarChar )
		{
		return ( grammarChar == Constants.QUERY_WORD_TYPE_CHAR ||
				grammarChar == Constants.QUERY_PARAMETER_CHAR ||
				grammarChar == Constants.GRAMMAR_WORD_DEFINITION_CHAR );
		}

	private byte addGrammar( String grammarString )
		{
		GrammarResultType grammarResult = new GrammarResultType();
		ReadWordResultType readWordResult;
		WordResultType wordResult;
		boolean hasFoundWordDefinitionInfo;
		boolean hasCreatedInterface = false;
		boolean hasFoundChoiceAlternatives = false;
		boolean hasFoundOnlyOptions = true;
		boolean hasParameter = false;
		boolean hasFoundPipe = false;
		boolean hasWordTypeNr = false;
		boolean hasGrammarWords = false;
		boolean isChoice = false;
		boolean isChoiceCheck = false;
		boolean isChoiceStart = false;
		boolean isMergedWord = false;
		boolean isEndOfLine = false;
		boolean isLastPartOfMultipleWord = false;
		boolean isMultipleWord = false;
		boolean isNewStart = true;
		boolean isOption = false;
		boolean isOptionCheck = false;
		boolean isOptionStart = false;
		boolean isSkipOptionForWriting = false;
		short grammarParameter = Constants.NO_GRAMMAR_PARAMETER;
		short predefinedMultipleWordNr = 0;
		short wordTypeNr = Constants.NO_WORD_TYPE_NR;
		int firstCreationItemNr = Constants.NO_ITEM_NR;
		int grammarStringLength;
		int grammarPosition = 0;
		GrammarItem foundGrammarItem;
		GrammarItem definitionGrammarItem = null;
		WordItem foundWordItem;
		WordItem createdWordItem;
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

		if( grammarString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given grammar string is undefined" );

		if( ( grammarStringLength = grammarString.length() ) == 0 )
			return adminItem_.startError( 1, moduleNameString_, "The given grammar string is empty" );

		if( currentLanguageWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The current language word item is undefined" );

		if( isPredefinedMultipleWord_ )
			{
			isPredefinedMultipleWord_ = false;
			clearPredefinedMultipleWordNrInPredefinedWords();
			}

		do	{
			if( ( readWordResult = adminItem_.readWordFromString( true, isMergedWord, false, grammarPosition, 0, grammarString ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to read a word from the grammar string" );

			hasFoundWordDefinitionInfo = false;

			switch( grammarString.charAt( grammarPosition ) )
				{
				case Constants.QUERY_WORD_TYPE_CHAR:
					if( hasWordTypeNr )
						return adminItem_.startError( 1, moduleNameString_, "I found more than one word type parameters defined in a grammar definition line" );

					while( grammarPosition + 1 < grammarStringLength &&
					Character.isDigit( grammarString.charAt( grammarPosition + 1 ) ) )
						{
						hasWordTypeNr = true;
						wordTypeNr = (short)( wordTypeNr * 10 + grammarString.charAt( ++grammarPosition ) - '0' );
						}

					if( !hasWordTypeNr )
						return adminItem_.startError( 1, moduleNameString_, "I failed to get the word type number from a grammar definition line" );

					break;

				case Constants.QUERY_PARAMETER_CHAR:
					if( hasParameter )
						return adminItem_.startError( 1, moduleNameString_, "I found more than one values parameters defined in a grammar definition line" );

					// Get parameter from string
					while( grammarPosition + 1 < grammarStringLength &&
					Character.isDigit( grammarString.charAt( grammarPosition + 1 ) ) )
						{
						hasParameter = true;
						grammarParameter = (short)( grammarParameter * 10 + grammarString.charAt( ++grammarPosition ) - '0' );

						if( grammarParameter == Constants.WORD_MERGED_WORD )
							isMergedWord = true;
						}

					if( !hasParameter )
						return adminItem_.startError( 1, moduleNameString_, "I failed to get the grammar parameter from a grammar definition line" );

					break;

				case Constants.GRAMMAR_WORD_DEFINITION_CHAR:
					if( hasCreatedInterface )
						return adminItem_.startError( 1, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );

					// Add grammar word or grammar definition word
					if( firstCreationItemNr != Constants.NO_ITEM_NR )
						return adminItem_.startError( 1, moduleNameString_, "A grammar definition word must be the first word in the grammar definition" );

					if( ( readWordResult = adminItem_.readWordFromString( false, isMergedWord, false, ++grammarPosition, 0, grammarString ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to read a word definition from the grammar string" );

					if( ( grammarResult = currentLanguageWordItem.findGrammar( ( grammarParameter >= Constants.GRAMMAR_SENTENCE ), grammarParameter, readWordResult.wordLength, grammarString.substring( grammarPosition ) ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a grammar definition item" );

					hasFoundWordDefinitionInfo = true;
					foundGrammarItem = grammarResult.grammarItem;

					if( !hasWordTypeNr ||
					foundGrammarItem == null ||
					grammarParameter >= Constants.GRAMMAR_SENTENCE )
						{
						if( ( grammarResult = currentLanguageWordItem.createGrammarItem( true, ( hasParameter && grammarParameter < Constants.GRAMMAR_SENTENCE ), false, false, false, wordTypeNr, grammarParameter, readWordResult.wordLength, grammarString.substring( grammarPosition ), null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a grammar definition word item" );

						firstCreationItemNr = CommonVariables.currentItemNr;
						definitionGrammarItem = grammarResult.grammarItem;
						}
					else
						{
						if( InputOutput.writeInterfaceText( Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_START, foundGrammarItem.grammarParameter(), Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_MIDDLE, currentLanguageWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_END ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'grammar parameter defined more than once' interface notification" );
						}

					break;

				case Constants.GRAMMAR_OPTION_READ_NOT_WRITE_START:
					isSkipOptionForWriting = true;

					// Don't insert a break statement here

				case Constants.GRAMMAR_OPTION_START:
					if( hasWordTypeNr &&
					grammarParameter < Constants.GRAMMAR_SENTENCE )
						return adminItem_.startError( 1, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

					if( CommonVariables.currentItemNr <= Constants.NO_ITEM_NR )
						return adminItem_.startError( 1, moduleNameString_, "A grammar definition must start with a grammar definition word" );

					if( isOption )
						return adminItem_.startError( 1, moduleNameString_, "A grammar opion definition can not be nested" );

					if( hasFoundPipe )
						return adminItem_.startError( 1, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );

					isOption = true;
					isNewStart = true;
					isOptionStart = true;
					isChoiceCheck = isChoice;

					break;

				case Constants.GRAMMAR_OPTION_READ_NOT_WRITE_END:
					isSkipOptionForWriting = false;

					// Don't insert a break statement here

				case Constants.GRAMMAR_OPTION_END:
					if( !isOption )
						return adminItem_.startError( 1, moduleNameString_, "I found an extra square bracket character in the grammar definition" );

					if( isChoiceCheck == isChoice )
						{
						if( hasFoundPipe )
							return adminItem_.startError( 1, moduleNameString_, "I found an extra pipe character before a square bracket in the grammar definition" );

						isOption = false;
						isNewStart = true;
						currentLanguageWordItem.markAsOptionEnd();
						}
					else
						{
						if( isChoice )
							return adminItem_.startError( 1, moduleNameString_, "Choices are started within an option" );

						return adminItem_.startError( 1, moduleNameString_, "Choices are ended within an option" );
						}

					break;

				case Constants.GRAMMAR_CHOICE_START:
					if( hasWordTypeNr &&
					grammarParameter < Constants.GRAMMAR_SENTENCE )
						return adminItem_.startError( 1, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

					if( CommonVariables.currentItemNr <= Constants.NO_ITEM_NR )
						return adminItem_.startError( 1, moduleNameString_, "A grammar definition must start with a grammar definition word" );

					if( isChoice )
						return adminItem_.startError( 1, moduleNameString_, "Nesting curved brackets isn't allowed" );

					if( hasFoundPipe )
						return adminItem_.startError( 1, moduleNameString_, "Pipes with different levels isn't allowed in the grammar definition" );

					isChoice = true;
					isNewStart = true;
					isChoiceStart = true;
					isOptionCheck = isOption;

					break;

				case Constants.GRAMMAR_CHOICE_END:
					if( !isChoice )
						return adminItem_.startError( 1, moduleNameString_, "I found an extra curved bracket character in the grammar definition" );

					if( isOptionCheck == isOption )
						{
						if( hasFoundPipe )
							return adminItem_.startError( 1, moduleNameString_, "I found an extra pipe character before a curved bracket in the grammar definition" );

						if( !hasFoundChoiceAlternatives )
							return adminItem_.startError( 1, moduleNameString_, "A grammar definition choice must have alternatives" );

						isChoice = false;
						isNewStart = true;
						hasFoundChoiceAlternatives = false;
						currentLanguageWordItem.markAsChoiceEnd( CommonVariables.currentItemNr );
						}
					else
						{
						if( isOption )
							return adminItem_.startError( 1, moduleNameString_, "An option is started within choices" );

						return adminItem_.startError( 1, moduleNameString_, "An option is ended within choices" );
						}

					break;

				case Constants.SYMBOL_PIPE:
					if( hasWordTypeNr &&
					grammarParameter < Constants.GRAMMAR_SENTENCE )
						return adminItem_.startError( 1, moduleNameString_, "A grammar definition can not have a value parameter lower than the grammar value" );

					if( hasFoundPipe )
						return adminItem_.startError( 1, moduleNameString_, "I found an extra pipe character in the grammar definition" );

					if( !isOption &&
					!isChoice )
						return adminItem_.startError( 1, moduleNameString_, "Pipes are only allowed within grammar definition options or choices" );

					hasFoundPipe = true;

					if( isChoice )
						hasFoundChoiceAlternatives = true;

					break;

				// Interface definition
				case Constants.SYMBOL_DOUBLE_QUOTE:
					if( !isNewStart ||
					hasGrammarWords ||
					isChoice ||
					isOption )
						return adminItem_.startError( 1, moduleNameString_, "Grammar definition and interface definitions can not be mixed" );

					// Interface definition
					if( definitionGrammarItem == null )
						{
						if( !hasParameter )
							return adminItem_.startError( 1, moduleNameString_, "An interface definition must have a parameter" );

						grammarPosition++;

						if( grammarPosition >= grammarStringLength ||
						grammarString.charAt( grammarStringLength - 1 ) != Constants.SYMBOL_DOUBLE_QUOTE )
							return adminItem_.startError( 1, moduleNameString_, "I found a corrupted interface definition" );

						if( currentLanguageWordItem.checkInterface( grammarParameter, grammarString.substring( grammarPosition ) ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add an interface definition word item" );

						if( currentLanguageWordItem.createInterfaceItem( grammarParameter, ( grammarStringLength - grammarPosition - 1 ), grammarString.substring( grammarPosition ) ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add an interface definition word item" );

						hasCreatedInterface = true;
						grammarPosition = ( grammarStringLength - 1 );
						}

					break;

				default:
					if( hasCreatedInterface )
						return adminItem_.startError( 1, moduleNameString_, "Interface definition and grammar definitions can not be mixed" );

					if( CommonVariables.currentItemNr <= Constants.NO_ITEM_NR )
						return adminItem_.startError( 1, moduleNameString_, "The first grammar word in a grammar definition is the grammar definition word and must start with a grammar character" );

					if( hasParameter &&
					grammarParameter < Constants.GRAMMAR_SENTENCE )
						{
						if( grammarParameter > Constants.NO_GRAMMAR_PARAMETER )
							{
							if( isMergedWord ||
							grammarParameter == Constants.WORD_PLURAL_NOUN_ENDING )
								{
								if( definitionGrammarItem == null )
									return adminItem_.startError( 1, moduleNameString_, "The grammar definition item is undefined" );

								if( ( grammarResult = currentLanguageWordItem.createGrammarItem( false, false, false, false, false, Constants.NO_WORD_TYPE_NR, grammarParameter, readWordResult.wordLength, grammarString.substring( grammarPosition ), definitionGrammarItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a grammar definition word item" );

								if( definitionGrammarItem.nextDefinitionGrammarItem != null )
									return adminItem_.startError( 1, moduleNameString_, "The next definition grammar item is already defined" );

								definitionGrammarItem.nextDefinitionGrammarItem = grammarResult.grammarItem;
								}
							else
								{
								foundWordItem = null;

								if( ( readWordResult.wordLength + grammarPosition ) < grammarStringLength )
									isMultipleWord = true;
								else
									{
									// End of line
									isEndOfLine = true;

									if( isMultipleWord )
										isLastPartOfMultipleWord = true;

									foundWordItem = adminItem_.predefinedWordItem( grammarParameter );
									}

								if( isLastPartOfMultipleWord ||
								foundWordItem == null )
									{
									if( ( wordResult = adminItem_.addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, grammarParameter, wordTypeNr, readWordResult.wordLength, grammarString.substring( grammarPosition ) ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a grammar word" );

									if( ( createdWordItem = wordResult.createdWordItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't create a grammar word" );

									if( isMultipleWord )
										{
										isPredefinedMultipleWord_ = true;
										createdWordItem.predefinedMultipleWordNr = ++predefinedMultipleWordNr;

										if( isLastPartOfMultipleWord )
											{
											if( ( wordResult = addPredefinedMultipleWord( predefinedMultipleWordNr, grammarParameter, wordTypeNr, foundWordItem ) ).result != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a predefined multiple grammar word" );

											if( ( createdWordItem = wordResult.createdWordItem ) == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't create a predefined multiple grammar word" );
											}
										}

									if( isEndOfLine &&
									setPredefinedWord( grammarParameter, createdWordItem ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to set a predefined word" );
									}
								else
									{
									if( foundWordItem.addWordType( false, false, ( adminItem_.isAdjectiveParameter( grammarParameter ) ? grammarParameter : Constants.NO_ADJECTIVE_PARAMETER ), ( adminItem_.isDefiniteArticleParameter( grammarParameter ) ? grammarParameter : Constants.NO_DEFINITE_ARTICLE_PARAMETER ), ( adminItem_.isIndefiniteArticleParameter( grammarParameter ) ? grammarParameter : Constants.NO_INDEFINITE_ARTICLE_PARAMETER ), wordTypeNr, readWordResult.wordLength, grammarString.substring( grammarPosition ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add another word type to grammar word \"" + foundWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					else
						{
						// Get grammar identification
						if( ( grammarResult = currentLanguageWordItem.findGrammar( true, Constants.NO_GRAMMAR_PARAMETER, readWordResult.wordLength, grammarString.substring( grammarPosition ) ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find a grammar definition word item" );

						foundGrammarItem = grammarResult.grammarItem;

						if( currentLanguageWordItem.createGrammarItem( false, ( hasFoundPipe || isNewStart ), isOptionStart, isChoiceStart, isSkipOptionForWriting, Constants.NO_WORD_TYPE_NR, Constants.NO_GRAMMAR_PARAMETER, readWordResult.wordLength, grammarString.substring( grammarPosition ), foundGrammarItem ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a grammar item" );

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
				}

			if( !hasFoundWordDefinitionInfo &&
			!readWordResult.hasFoundGrammarDefinition &&
			readWordResult.nextWordPosition < grammarStringLength &&
			( readWordResult = adminItem_.readWordFromString( false, false, false, grammarPosition, 0, grammarString ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to read a new word from the grammar string" );

			grammarPosition = readWordResult.nextWordPosition;
			}
		while( readWordResult.nextWordPosition < grammarStringLength );

		if( isOption )
			return adminItem_.startError( 1, moduleNameString_, "The grammar definition option isn't closed" );

		if( isChoice )
			return adminItem_.startError( 1, moduleNameString_, "The grammar definition choice isn't closed" );

		if( hasFoundPipe )
			return adminItem_.startError( 1, moduleNameString_, "The grammar definition ended with an open pipe" );

		if( definitionGrammarItem != null )
			{
			if( hasFoundOnlyOptions &&

			( !hasWordTypeNr ||
			grammarParameter >= Constants.GRAMMAR_SENTENCE ) )
				{
				if( hasGrammarWords )
					return adminItem_.startError( 1, moduleNameString_, "The grammar definition only exists of options" );

				return adminItem_.startError( 1, moduleNameString_, "The grammar definition only exists of a grammar definition word" );
				}

			// Remove possible duplicate grammar definition
			if( currentLanguageWordItem.checkForDuplicateGrammarDefinition() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to check for a duplicate grammar definition" );

			if( currentLanguageWordItem.linkLaterDefinedGrammarWords() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to link later defined grammar words" );
			}

		return Constants.RESULT_OK;
		}

	private WordResultType addPredefinedMultipleWord( short nMultipleWords, short wordParameter, short wordTypeNr, WordItem multipleWordItem )
		{
		short predefinedMultipleWordNr = 0;
		WordItem foundMultipleWordItem;
		WordTypeItem foundWordTypeItem;
		StringBuffer multipleWordStringBuffer = new StringBuffer();
		WordResultType wordResult = new WordResultType();

		wordResult.createdWordItem = multipleWordItem;

		if( nMultipleWords <= 0 )
			return adminItem_.startWordResultError( 1, moduleNameString_, "The given number of multiple words is undefined" );

		while( predefinedMultipleWordNr < nMultipleWords &&
		( foundMultipleWordItem = predefinedMultipleWordItem( ++predefinedMultipleWordNr ) ) != null )
			{
			if( ( foundWordTypeItem = foundMultipleWordItem.activeWordTypeItem( wordTypeNr ) ) != null )
				{
				if( predefinedMultipleWordNr > 1 )
					multipleWordStringBuffer.append( Constants.SPACE_STRING );

				multipleWordStringBuffer.append( foundWordTypeItem.itemString() );
				}
			}

		if( multipleWordItem == null ||
		multipleWordItem.hasCurrentCreationSentenceNr() )
			{
			if( ( wordResult = adminItem_.addWord( false, true, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordParameter, wordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ) ).result != Constants.RESULT_OK )
				return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to add a predefined grammar word" );
			}
		else
			{
			if( multipleWordItem.addWordType( true, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ).result != Constants.RESULT_OK )
				return adminItem_.addWordResultError( 1, moduleNameString_, "The given multiple word item is undefined" );
			}

		if( wordResult.createdWordItem == null )
			return adminItem_.startWordResultError( 1, moduleNameString_, "The multiple word item is undefined" );

		predefinedMultipleWordNr = 0;

		while( predefinedMultipleWordNr < nMultipleWords &&
		( foundMultipleWordItem = predefinedMultipleWordItem( ++predefinedMultipleWordNr ) ) != null )
			{
			if( foundMultipleWordItem.addMultipleWord( nMultipleWords, wordTypeNr, wordResult.createdWordItem ) != Constants.RESULT_OK )
				return adminItem_.addWordResultError( 1, moduleNameString_, "I failed to add a multiple word" );
			}

		return wordResult;
		}

	private byte authorizeWord( WordItem authorizationWordItem )
		{
		if( authorizationWordItem == null )
			return adminItem_.startSystemError( 1, moduleNameString_, "The given authorization word item is undefined" );

		// Only accept authorization of words during startup
		if( !adminItem_.isSystemStartingUp() )
			return adminItem_.startError( 1, moduleNameString_, "You are not authorized to authorize the given word" );

		if( authorizationWordItem.assignChangePermissions( moduleNameString_ ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign my authorization permissions to a word" );

		return Constants.RESULT_OK;
		}

	private byte closeCurrentFileItem( FileItem closeFileItem )
		{
		boolean isTestFile;
		int testFileSentenceNr;

		// Get info before closing the file
		isTestFile = adminItem_.isCurrentFileTestFile();
		testFileSentenceNr = adminItem_.currentFileSentenceNr();

		if( adminItem_.closeCurrentFile( closeFileItem ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to close a file" );

		if( isTestFile )
			adminItem_.deleteSentences( testFileSentenceNr );
		else
			{
			if( CommonVariables.hasDisplayedWarning ||
			CommonVariables.result != Constants.RESULT_OK )
				{
				hasClosedFileDueToError_ = true;
				CommonVariables.hasDisplayedWarning = false;
				}
			}

		return Constants.RESULT_OK;
		}

	private byte createLanguageSpecification( WordItem languageNounWordItem )
		{
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;

		if( languageNounWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given language noun word item is undefined" );

		if( currentLanguageWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The current language word item is undefined" );

		if( !currentLanguageWordItem.isLanguageWord() )
			return adminItem_.startError( 1, moduleNameString_, "The current language word isn't a language word" );

		if( addSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, currentLanguageWordItem, languageNounWordItem, null, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a new language specification" );

		return Constants.RESULT_OK;
		}

	private byte createLanguageWord( String languageNameString )
		{
		WordResultType wordResult;
		WordItem languageWordItem;

		if( ( wordResult = findLanguageByName( languageNameString ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find the language" );

		if( wordResult.foundWordItem == null )
			{
			CommonVariables.currentLanguageNr++;

			if( ( wordResult = adminItem_.addWord( true, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, Constants.NO_WORD_PARAMETER, Constants.WORD_TYPE_PROPER_NAME, languageNameString.length(), languageNameString ) ).result != Constants.RESULT_OK )
				{
				// On error, restore old language
				CommonVariables.currentLanguageNr--;
				return adminItem_.addError( 1, moduleNameString_, "I failed to add a language word" );
				}

			if( ( languageWordItem = wordResult.createdWordItem ) == null )
				return adminItem_.startError( 1, moduleNameString_, "The created language word item is undefined" );

			if( authorizeWord( languageWordItem ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the language word" );

			CommonVariables.currentLanguageWordItem = languageWordItem;
			}
		else
			CommonVariables.currentLanguageWordItem = wordResult.foundWordItem;

		return Constants.RESULT_OK;
		}

	private byte executeLine( StringBuffer readStringBuffer )
		{
		boolean wasQueryCommand = false;
		boolean hasSwitchedLanguage = false;
		boolean wasUndoOrRedoCommand;

		if( readStringBuffer == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read string buffer is undefined" );

		// Skip empty line
		if( readStringBuffer.length() > 0 &&
		// and comment line
		readStringBuffer.charAt( 0 ) != Constants.COMMENT_CHAR )
			{
			CommonVariables.hasDisplayedIntegrityWarning = false;
			CommonVariables.hasDisplayedMessage = false;
			CommonVariables.hasDisplayedWarning = false;
			CommonVariables.isAssignmentChanged = false;

			if( !adminItem_.wasPreviousCommandUndoOrRedo() )
				cleanupDeletedItems();

			hasClosedFileDueToError_ = false;

			// Reading grammar/interface file, changing language or a query
			if( readStringBuffer.charAt( 0 ) == Constants.QUERY_CHAR )
				{
				// Grammar/interface
				if( Character.isLetter( readStringBuffer.charAt( 1 ) ) )
					{
					if( adminItem_.isSystemStartingUp() )
						{
						if( readGrammarFileAndUserInterfaceFile( readStringBuffer.substring( 1 ) ) != Constants.RESULT_OK )
							adminItem_.addError( 1, moduleNameString_, "I failed to read the language" );
						}
					else
						{
						// Change language
						if( assignLanguage( readStringBuffer.substring( 1 ) ) == Constants.RESULT_OK )
							hasSwitchedLanguage = true;
						else
							adminItem_.addError( 1, moduleNameString_, "I failed to assign the language" );
						}
					}
				else
					{
					// Query
					if( readStringBuffer.charAt( 0 ) == Constants.QUERY_CHAR )
						{
						wasQueryCommand = true;

						if( adminItem_.executeQuery( false, true, true, Constants.INPUT_OUTPUT_PROMPT_QUERY, 1, readStringBuffer.toString() ).result != Constants.RESULT_OK )
							adminItem_.addError( 1, moduleNameString_, "I failed to execute query: \"" + readStringBuffer + "\"" );
						}
					}
				}
			else
				{
				// Sentence or grammar definition
				if( incrementCurrentSentenceNr() != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to increment the current sentence number" );

				// Grammar definition
				if( isGrammarChar( readStringBuffer.charAt( 0 ) ) )
					{
					if( addGrammar( readStringBuffer.toString() ) != Constants.RESULT_OK )
						adminItem_.addError( 1, moduleNameString_, "I failed to add grammar: \"" + readStringBuffer + "\"" );
					}
				else
					{
					// Sentence
					if( adminItem_.processReadSentence( readStringBuffer.toString() ) != Constants.RESULT_OK )
						adminItem_.addError( 1, moduleNameString_, "I failed to process a read sentence" );
					}
				}

			if( CommonVariables.result != Constants.RESULT_SYSTEM_ERROR &&
			!adminItem_.hasRequestedRestart() )
				{
				wasUndoOrRedoCommand = adminItem_.wasUndoOrRedoCommand();

				if( CommonVariables.result == Constants.RESULT_OK &&
				!CommonVariables.hasDisplayedWarning )
					{
					if( adminItem_.hasAnyChangeBeenMadeByThisSentence() )
						{
						if( !hasSwitchedLanguage &&

						( wasUndoOrRedoCommand ||
						!CommonVariables.hasDisplayedMessage ) )
							{
							if( adminItem_.executeSelections() != Constants.RESULT_OK )
								adminItem_.addError( 1, moduleNameString_, "I failed to execute selections after reading the sentence" );
							}
						}
					else
						{
						if( !hasSwitchedLanguage &&
						!wasLoginCommand_ &&
						!CommonVariables.hasDisplayedMessage &&
						!adminItem_.isSystemStartingUp() &&

						InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_SENTENCE_NOTIFICATION_I_KNOW ) != Constants.RESULT_OK )
							adminItem_.addError( 1, moduleNameString_, "I failed to write the 'I know' interface notification" );
						}
					}

				adminItem_.clearTemporaryAdminLists();

				if( !wasQueryCommand &&
				!wasUndoOrRedoCommand )
					{
					cleanupDeletedItems();

					if( !adminItem_.isSystemStartingUp() )
						{
						// Check for empty sentence
						adminItem_.setCurrentItemNr();

						if( CommonVariables.currentItemNr == Constants.NO_ITEM_NR )
							adminItem_.decrementCurrentSentenceNr();
						}
					}
				}
			}

		return Constants.RESULT_OK;
		}

	private byte incrementCurrentSentenceNr()
		{
		if( CommonVariables.currentSentenceNr >= Constants.MAX_SENTENCE_NR )
			return adminItem_.startSystemError( 1, moduleNameString_, "Sentence number overflow! I can't except anymore input" );

		CommonVariables.currentSentenceNr++;
		// Necessary after changing current sentence number
		adminItem_.setCurrentItemNr();

		return Constants.RESULT_OK;
		}

	private byte readLanguageFile( boolean isGrammarFile, String languageNameString )
		{
		FileResultType fileResult = new FileResultType();
		byte originalResult;
		FileItem openedLanguageFileItem;

		if( languageNameString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given language name is undefined" );

		Console.displayProgressStatus( languageNameString );

		if( ( fileResult = adminItem_.openFile( true, false, false, false, ( isGrammarFile ? Constants.FILE_DATA_GRAMMAR_DIRECTORY_NAME_STRING : Constants.FILE_DATA_INTERFACE_DIRECTORY_NAME_STRING ), languageNameString, null, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, ( isGrammarFile ? "I failed to open the grammar file: \"" : "I failed to open the interface file: \"" ) + languageNameString + "\"" );

		if( ( openedLanguageFileItem = fileResult.createdFileItem ) != null )
			{
			if( createLanguageWord( languageNameString ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to create language word: \"" + languageNameString + "\"" );

			// This isn't the first language
			if( predefinedNounLanguageWordItem_ != null &&
			createLanguageSpecification( predefinedNounLanguageWordItem_ ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to create a language specification" );

			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened language file" );

			originalResult = CommonVariables.result;

			if( closeCurrentFileItem( openedLanguageFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the language file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			}
		else
			{
			// When reading special characters goes wrong, the wrong Zip file may be downloaded
			// The Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and higher
			if( languageNameString.equals( Constants.FILE_WINDOWS_VERSION_STRING ) )
				return adminItem_.startSystemError( 1, moduleNameString_, "You have probably downloaded the Zip file for the wrong Windows version.\nPlease check if the downloaded file name matches your Windows version.\nThe Zip format of special characters up to Windows 7 is different from the Zip format of special characters from Windows 8 and higher" );

			return adminItem_.startError( 1, moduleNameString_, ( isGrammarFile ? "I couldn't open the grammar file: \"" : "I couldn't open the interface file: \"" ) + languageNameString + "\"" );
			}

		return Constants.RESULT_OK;
		}

	private byte readGrammarFileAndUserInterfaceFile( String readLanguageNameString )
		{
		WordItem currentLanguageWordItem;
		String startupLanguageNameString = adminItem_.startupLanguageNameString();

		if( readLanguageNameString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given read language name string is undefined" );

		// Read the user-interface file
		if( readLanguageFile( false, readLanguageNameString ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to read an interface file" );

		// Read the grammar file
		if( readLanguageFile( true, readLanguageNameString ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to read a grammar file" );

		// If the system is restarted
		if( startupLanguageNameString != null &&
		predefinedNounStartupLanguageWordItem_ != null &&
		( currentLanguageWordItem = CommonVariables.currentLanguageWordItem ) != null )
			{
			// The current language (of which the grammar file and the interface file were read above),
			// is the last selected language before the restart
			if( startupLanguageNameString.equals( currentLanguageWordItem.anyWordTypeString() ) )
				{
				// Create language specification for the startup language word
				if( addSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.WORD_TYPE_NOUN_SINGULAR, Constants.WORD_TYPE_PROPER_NAME, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, predefinedNounStartupLanguageWordItem_, currentLanguageWordItem, null, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to add a predefined noun startup language specification" );

				// Assign the created startup language word to the current language
				if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, predefinedNounStartupLanguageWordItem_, currentLanguageWordItem, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to assign the predefined noun startup language word" );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte readLine( boolean isClearInputField, boolean isFirstLine, boolean isPassword, boolean isQuestion, int promptSentenceNr, String promptUserNameString, StringBuffer readStringBuffer )
		{
		StringBuffer promptStringBuffer = new StringBuffer();

		if( promptSentenceNr > Constants.NO_SENTENCE_NR )
			promptStringBuffer.append( promptSentenceNr );

		if( promptUserNameString != null )
			{
			if( promptStringBuffer.length() > 0 )
				promptStringBuffer.append( Constants.QUERY_SEPARATOR_STRING );

			promptStringBuffer.append( promptUserNameString );
			}

		if( InputOutput.readLine( isClearInputField, adminItem_.isDisplayingLine(), isFirstLine, isPassword, isQuestion, promptStringBuffer.toString(), readStringBuffer, adminItem_.currentReadFile() ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to read a line from a file or from input" );

		return Constants.RESULT_OK;
		}

	private byte setPredefinedWord( short grammarParameter, WordItem createdWordItem )
		{
		if( createdWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given created word item" );

		switch( grammarParameter )
			{
			case Constants.WORD_PARAMETER_ADJECTIVE_BUSY:
				if( predefinedAdjectiveBusyWordItem_ == null )
					predefinedAdjectiveBusyWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveBusyWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined busy adjective word" );
					}

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_DONE:
				if( predefinedAdjectiveDoneWordItem_ == null )
					predefinedAdjectiveDoneWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveDoneWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined done adjective word" );
					}

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_INVERTED:
				if( predefinedAdjectiveInvertedWordItem_ == null )
					predefinedAdjectiveInvertedWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveInvertedWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined inverted adjective word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_DEVELOPER:
				if( predefinedNounDeveloperWordItem_ == null )
					{
					predefinedNounDeveloperWordItem_ = createdWordItem;

					if( authorizeWord( predefinedNounDeveloperWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined developer noun word" );
					}
				else
					{
					if( predefinedNounDeveloperWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined developer noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_EXPERT:
				if( predefinedNounExpertWordItem_ == null )
					{
					predefinedNounExpertWordItem_ = createdWordItem;

					if( authorizeWord( predefinedNounExpertWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined expert noun word" );
					}
				else
					{
					if( predefinedNounExpertWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined expert noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_LANGUAGE:
				if( predefinedNounLanguageWordItem_ == null )
					{
					predefinedNounLanguageWordItem_ = createdWordItem;
					CommonVariables.predefinedNounLanguageWordItem = createdWordItem;

					// During the creation of the first language,
					// the predefined noun language word wasn't defined yet
					// So, create first language specification afterwards
					if( createLanguageSpecification( createdWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to create a language specification" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_PASSWORD:
				if( predefinedNounPasswordWordItem_ == null )
					{
					predefinedNounPasswordWordItem_ = createdWordItem;

					if( authorizeWord( predefinedNounPasswordWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined password noun word" );
					}
				else
					{
					if( predefinedNounPasswordWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined password noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_SOLVE_LEVEL:
				if( predefinedNounSolveLevelWordItem_ == null )
					predefinedNounSolveLevelWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveLevelWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined solve level noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_SOLVE_METHOD:
				if( predefinedNounSolveMethodWordItem_ == null )
					predefinedNounSolveMethodWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveMethodWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined solve method noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
				if( predefinedNounSolveStrategyWordItem_ == null )
					predefinedNounSolveStrategyWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveStrategyWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined solve strategy noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
				if( predefinedNounStartupLanguageWordItem_ == null )
					predefinedNounStartupLanguageWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounStartupLanguageWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined startup language word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_USER:
				if( predefinedNounUserWordItem_ == null )
					{
					predefinedNounUserWordItem_ = createdWordItem;
					CommonVariables.predefinedNounUserWordItem = createdWordItem;

					if( authorizeWord( predefinedNounUserWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined user noun word" );
					}
				else
					{
					if( predefinedNounUserWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined user noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( predefinedVerbLoginWordItem_ == null )
					{
					predefinedVerbLoginWordItem_ = createdWordItem;

					if( authorizeWord( predefinedVerbLoginWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined login verb word" );
					}
				else
					{
					if( predefinedVerbLoginWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I found a different predefined login verb word" );
					}

				break;
			}

		return Constants.RESULT_OK;
		}

	private WordResultType findLanguageByName( String languageNameString )
		{
		// Initially, the language name words are not linked to
		// the language defintion word. So, search in all words
		return adminItem_.findWordTypeInAllWords( true, Constants.WORD_TYPE_PROPER_NAME, languageNameString.toString(), null );
		}

	private static WordItem predefinedMultipleWordItem( short predefinedMultipleWordNr )
		{
		WordItem currentWordItem;

		if( predefinedMultipleWordNr > 0 &&
		( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all (grammar) words
			do	{
				if( currentWordItem.predefinedMultipleWordNr == predefinedMultipleWordNr )
					return currentWordItem;
				}
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}

		return null;
		}


	// Constructor

	protected AdminReadFile( AdminItem adminItem )
		{
		// Private constructed variables

		hasClosedFileDueToError_ = false;
		hasFoundDifferentTestResult_ = false;
		isPredefinedMultipleWord_ = false;
		isDeveloperTheCurrentUser_ = false;
		isExpertTheCurrentUser_ = false;
		wasLoginCommand_ = false;

		testFileNr_ = 0;
		firstSentenceNrOfCurrentUser_ = Constants.NO_SENTENCE_NR;
		startTime_ = 0;

		currentUserWordItem_ = null;
		predefinedAdjectiveBusyWordItem_ = null;
		predefinedAdjectiveDoneWordItem_ = null;
		predefinedAdjectiveInvertedWordItem_ = null;
		predefinedNounDeveloperWordItem_ = null;
		predefinedNounExpertWordItem_ = null;
		predefinedNounLanguageWordItem_ = null;
		predefinedNounPasswordWordItem_ = null;
		predefinedNounSolveLevelWordItem_ = null;
		predefinedNounSolveMethodWordItem_ = null;
		predefinedNounSolveStrategyWordItem_ = null;
		predefinedNounStartupLanguageWordItem_ = null;
		predefinedNounUserWordItem_ = null;
		predefinedVerbLoginWordItem_ = null;

		// Private initialized variables

		moduleNameString_ = this.getClass().getName();

		// Checking private initialized variables

		if( ( adminItem_ = adminItem ) == null )
			{
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		}


	// Protected methods

	protected void initializeAdminReadFileVariables()
		{
		wasLoginCommand_ = false;
		}

	protected boolean isDeveloperTheCurrentUser()
		{
		return isDeveloperTheCurrentUser_;
		}

	protected boolean isDeveloperOrExpertTheCurrentUser()
		{
		return ( isDeveloperTheCurrentUser_ ||
				isExpertTheCurrentUser_ );
		}

	protected boolean isExpertTheCurrentUser()
		{
		return isExpertTheCurrentUser_;
		}

	protected int firstSentenceNrOfCurrentUser()
		{
		return ( firstSentenceNrOfCurrentUser_ + 1 );
		}

	protected byte assignLanguage( String languageNameString )
		{
		WordResultType wordResult;
		boolean hasFoundLanguage = false;
		SpecificationItem languageSpecificationItem;
		WordItem foundLanguageWordItem;

		if( ( wordResult = findLanguageByName( languageNameString ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to find the language" );

		if( ( foundLanguageWordItem = wordResult.foundWordItem ) != null )
			{
			hasFoundLanguage = true;

			if( ( languageSpecificationItem = foundLanguageWordItem.bestMatchingSpecificationWordSpecificationItem( true, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, predefinedNounLanguageWordItem_ ) ) != null )
				CommonVariables.currentLanguageNr = languageSpecificationItem.languageNr();

			if( CommonVariables.currentLanguageWordItem != foundLanguageWordItem )
				{
				if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, foundLanguageWordItem, predefinedNounLanguageWordItem_, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to assign the language" );

				CommonVariables.currentLanguageWordItem = foundLanguageWordItem;
				}

			if( !hasFoundLanguage )
				return adminItem_.startError( 1, moduleNameString_, "The given name isn't a grammar nor an language" );
			}
		else
			{
			if( !adminItem_.isSystemStartingUp() )
				return adminItem_.startError( 1, moduleNameString_, "I couldn't find the requested language" );
			}

		return Constants.RESULT_OK;
		}

	protected byte compareOutputFileAgainstReferenceFile( String testFileNameString )
		{
		FileResultType fileResult;
		boolean isStop = false;
		int lineNr = 0;
		String outputString = null;
		String referenceString = null;

		hasFoundDifferentTestResult_ = false;

		if( ( fileResult = adminItem_.openFile( true, false, false, true, Constants.FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, testFileNameString, null, Constants.FILE_DATA_REGRESSION_TEST_REFERENCE_DIRECTORY_NAME_STRING ) ).result != Constants.RESULT_OK )
			return adminItem_.startError( 1, moduleNameString_, "I failed to open a test file" );

		if( fileResult.outputFile == null )
			return adminItem_.startError( 1, moduleNameString_, "The output file is undefined" );

		if( fileResult.referenceFile == null )
			return adminItem_.startError( 1, moduleNameString_, "The reference file is undefined" );

		do	{
			isStop = true;
			hasFoundDifferentTestResult_ = true;

			try	{
				outputString = fileResult.outputFile.readLine();
				referenceString = fileResult.referenceFile.readLine();
				}
			catch( IOException exception )
				{
				return adminItem_.startError( 1, moduleNameString_, "I couldn't read from the output file or the reference file" );
				}

			if( outputString != null &&
			referenceString != null )
				{
				if( !outputString.equals( referenceString ) )
					return adminItem_.startError( 1, moduleNameString_, "Line number " + lineNr + " is different:\n* test result:\t\t\"" + outputString + "\"\n* against reference:\t\"" + referenceString + "\"" );

				isStop = false;
				lineNr++;
				}
			else
				{
				if( outputString != null )
					return adminItem_.startError( 1, moduleNameString_, "The output file is longer than the reference file" );

				if( referenceString != null )
					return adminItem_.startError( 1, moduleNameString_, "The output file is shorter than the reference file" );

				hasFoundDifferentTestResult_ = false;
				}
			}
		while( !isStop );

		try	{
			if( fileResult.outputFile != null )
				fileResult.outputFile.close();

			if( fileResult.referenceFile != null )
				fileResult.referenceFile.close();
			}
		catch( IOException exception )
			{
			return adminItem_.startError( 1, moduleNameString_, "I couldn't close the output file or the reference file" );
			}

		return Constants.RESULT_OK;
		}

	protected byte login( WordItem specificationWordItem )
		{
		WordResultType wordResult;
		boolean isAlreadyLoggedInAsGivenUser = false;
		boolean isCorrectPassword = false;
		boolean isNoPasswordRequired = false;
		GeneralizationItem currentGeneralizationItem;
		SpecificationItem passwordAssignmentItem = null;
		WordItem currentGeneralizationWordItem;
		WordItem passwordSpecificationWordItem;
		WordItem foundUserWordItem = null;
		StringBuffer readPasswordStringBuffer = new StringBuffer();
		StringBuffer readUserNameStringBuffer = new StringBuffer();

		wasLoginCommand_ = false;

		if( predefinedNounPasswordWordItem_ == null )
			return adminItem_.startSystemError( 1, moduleNameString_, "The predefined password noun word item is undefined" );

		if( predefinedNounUserWordItem_ == null )
			return adminItem_.startSystemError( 1, moduleNameString_, "The predefined user noun word item is undefined" );

		// No user name is given
		if( specificationWordItem == null &&
		// Get first user without password
		( currentGeneralizationItem = predefinedNounPasswordWordItem_.firstSpecificationGeneralizationItem( true ) ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				// Select first user in the current language
				if( currentGeneralizationWordItem.activeWordTypeItem( false, Constants.WORD_TYPE_PROPER_NAME ) != null )
					specificationWordItem = currentGeneralizationWordItem;
				}
			while( specificationWordItem == null &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		// No user name is given
		// and no user without password is found
		if( specificationWordItem == null )
			{
			// Ask user name
			if( readLine( true, false, false, true, Constants.NO_SENTENCE_NR, predefinedNounUserWordItem_.singularNounString(), readUserNameStringBuffer ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to read to user name" );
			}
		else
			readUserNameStringBuffer.append( specificationWordItem.anyWordTypeString() );

		if( readUserNameStringBuffer.length() > 0 &&
		// Find user word
		( currentGeneralizationItem = predefinedNounUserWordItem_.firstSpecificationGeneralizationItem( false ) ) != null )
			{
			do	{
				if( ( currentGeneralizationWordItem = currentGeneralizationItem.generalizationWordItem() ) == null )
					return adminItem_.startSystemError( 1, moduleNameString_, "I found an undefined generalization word" );

				if( ( wordResult = currentGeneralizationWordItem.findWordType( true, Constants.WORD_TYPE_PROPER_NAME, readUserNameStringBuffer.toString() ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to find the user name" );

				if( wordResult.foundWordTypeItem != null )
					foundUserWordItem = currentGeneralizationWordItem;
				}
			while( foundUserWordItem == null &&
			( currentGeneralizationItem = currentGeneralizationItem.nextSpecificationGeneralizationItem() ) != null );
			}

		if( foundUserWordItem != null &&
		foundUserWordItem == currentUserWordItem_ )
			{
			// Already logged in as given user
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_ALREADY_LOGGED_IN_START, readUserNameStringBuffer.toString(), Constants.INTERFACE_CONSOLE_LOGIN_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'already logged in' interface notification" );

			isAlreadyLoggedInAsGivenUser = true;
			}
		else
			{
			if( foundUserWordItem != null &&
			( passwordAssignmentItem = predefinedNounPasswordWordItem_.firstActiveAssignmentItem( false, Constants.NO_QUESTION_PARAMETER, foundUserWordItem ) ) == null )
				{
				// No password assignment found
				// Now, check explicitly if no password is required
				if( foundUserWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, true, true, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, predefinedNounPasswordWordItem_ ) != null )
					isNoPasswordRequired = true;
				}
			else
				{
				// Ask password
				if( readLine( true, false, true, true, Constants.NO_SENTENCE_NR, predefinedNounPasswordWordItem_.singularNounString(), readPasswordStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to read to password" );

				if( passwordAssignmentItem != null )
					{
					if( ( passwordSpecificationWordItem = passwordAssignmentItem.specificationWordItem() ) == null )
						return adminItem_.startSystemError( 1, moduleNameString_, "The password assignment specification item is undefined" );

					if( passwordSpecificationWordItem.isCorrectHiddenWordType( passwordAssignmentItem.specificationWordTypeNr(), readPasswordStringBuffer.toString(), moduleNameString_ ) )
						isCorrectPassword = true;
					}
				}

			if( ( isCorrectPassword ||
			isNoPasswordRequired ) &&

			foundUserWordItem != null )
				{
				if( assignSpecificationWithAuthorization( false, false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, Constants.NO_SENTENCE_NR, 0, null, foundUserWordItem, predefinedNounUserWordItem_, null ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to assign the user" );

				firstSentenceNrOfCurrentUser_ = CommonVariables.currentSentenceNr;
				currentUserWordItem_ = foundUserWordItem;
				CommonVariables.currentUserNr = predefinedNounUserWordItem_.userNr( foundUserWordItem );

				isDeveloperTheCurrentUser_ = ( predefinedNounDeveloperWordItem_ != null &&
												foundUserWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, predefinedNounDeveloperWordItem_ ) != null );

				isExpertTheCurrentUser_ = ( predefinedNounExpertWordItem_ != null &&
											foundUserWordItem.bestMatchingSpecificationWordSpecificationItem( false, false, false, false, false, Constants.NO_QUESTION_PARAMETER, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, predefinedNounExpertWordItem_ ) != null );

				if( isExpertTheCurrentUser_ &&
				InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_WELCOME_MESSAGE_EXPERT ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the welcome message for experts" );
				}
			}

		if( ( isAlreadyLoggedInAsGivenUser ||
		isCorrectPassword ||
		isNoPasswordRequired ) &&

		foundUserWordItem != null )
			wasLoginCommand_ = true;
		else
			{
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_CONSOLE_LOGIN_FAILED ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write the 'login failed' interface notification" );
			}

		return Constants.RESULT_OK;
		}

	protected byte readAndExecute()
		{
		boolean isLineExecuted;
		boolean isFirstLine = true;
		StringBuffer readStringBuffer = new StringBuffer();

		hasFoundDifferentTestResult_ = false;

		do	{
			isLineExecuted = false;
			readStringBuffer = new StringBuffer();

			if( readLine( false, isFirstLine, false, false, ( CommonVariables.currentSentenceNr + 1 ), ( currentUserWordItem_ == null ? null : currentUserWordItem_.anyWordTypeString() ), readStringBuffer ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to read a line" );

			if( InputOutput.hasReadLine() )
				{
				if( executeLine( readStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to execute the read line" );

				isFirstLine = false;
				isLineExecuted = true;
				}
			}
		while( isLineExecuted &&
		!hasFoundDifferentTestResult_ &&
		!adminItem_.hasRequestedRestart() &&
		!Console.isTestingCanceled() &&

		// Ignore warnings during testing
		( adminItem_.isCurrentlyTesting() ||
		!CommonVariables.hasDisplayedWarning ) );

		if( adminItem_.isSystemStartingUp() &&
		CommonVariables.hasDisplayedWarning )
			CommonVariables.result = Constants.RESULT_SYSTEM_ERROR;

		return Constants.RESULT_OK;
		}

	protected byte readExampleFile( String exampleFileNameString )
		{
		FileResultType fileResult;
		byte originalResult;
		FileItem openedExampleFileItem;

		if( exampleFileNameString != null )
			{
			if( adminItem_.isSystemStartingUp() )
				Console.displayProgressStatus( exampleFileNameString );

			if( ( fileResult = adminItem_.openFile( !adminItem_.isSystemStartingUp(), false, false, true, Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, exampleFileNameString, null, null ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to open an example file" );

			if( ( openedExampleFileItem = fileResult.createdFileItem ) != null )
				{
				if( readAndExecute() != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened example file" );

				originalResult = CommonVariables.result;

				if( closeCurrentFileItem( openedExampleFileItem ) != Constants.RESULT_OK )
					adminItem_.addError( 1, moduleNameString_, "I failed to close the example file item" );

				if( originalResult != Constants.RESULT_OK )
					CommonVariables.result = originalResult;
				}
			}
		else
			{
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_WHICH_FILE_TO_READ ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	protected byte readStartupFile()
		{
		FileResultType fileResult;
		byte originalResult;
		FileItem openedStartupFileItem;

		if( ( fileResult = adminItem_.openFile( true, false, false, true, Constants.FILE_DATA_STARTUP_DIRECTORY_NAME_STRING, Constants.FILE_STARTUP_NAME_STRING, null, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to open an startup file" );

		if( ( openedStartupFileItem = fileResult.createdFileItem ) != null )
			{
			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened startup file" );

			originalResult = CommonVariables.result;

			if( closeCurrentFileItem( openedStartupFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the startup file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			}

		return Constants.RESULT_OK;
		}

	protected byte readTestFile( String testFileNameString )
		{
		boolean isFirstTestFile = ( isDeveloperTheCurrentUser_ &&
								!adminItem_.isCurrentlyTesting() );
		byte originalResult;
		long totalTime;
		FileItem testFileItem;
		FileResultType fileResult;

		// Check if user has already entered sentences
		if( hasAnyUserSpecification() )
			return adminItem_.startError( 1, moduleNameString_, "Some knowledge is already entered. It may disturb the test results. Please, use restart button before testing to ensure no knowledge is present" );

		if( isFirstTestFile )
			{
			testFileNr_ = 0;
			startTime_ = new Date().getTime();
			CommonVariables.cleaningTime = 0;
			CommonVariables.parsingTime = 0;
			CommonVariables.solvingTime = 0;
			CommonVariables.writingTime = 0;
			}
		else
			testFileNr_++;

		if( ( fileResult = adminItem_.openFile( true, false, true, true, Constants.FILE_DATA_EXAMPLES_DIRECTORY_NAME_STRING, testFileNameString, Constants.FILE_DATA_REGRESSION_TEST_OUTPUT_DIRECTORY_NAME_STRING, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to open a test file" );

		if( ( testFileItem = fileResult.createdFileItem ) == null )
			return adminItem_.startError( 1, moduleNameString_, "The last created file item is undefined" );

			if( InputOutput.writeText( true, true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.NO_CENTER_WIDTH, "Test file #" + testFileNr_ + ": " + testFileItem.readFileNameString() + Constants.NEW_LINE_STRING ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write a text" );

			InputOutput.redirectOutputToTestFile( testFileItem.writeFile() );

			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened test file" );

			if( ( originalResult = CommonVariables.result ) != Constants.RESULT_OK )
				CommonVariables.result = Constants.RESULT_OK;

			if( closeCurrentFileItem( testFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the test file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			else
				{
				if( isFirstTestFile &&
				!Console.isTestingCanceled() )
					{
					totalTime = ( new Date().getTime() - startTime_ );

					if( InputOutput.writeText( true, true, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.NO_CENTER_WIDTH, "Done in: " + String.format( "%.1f", ( totalTime / 1e3 ) ) + " sec.\n- Parsing user sentences: " + String.format( "%.1f", ( CommonVariables.parsingTime / 1e9 ) ) + " sec. (" + String.format( "%.1f", ( CommonVariables.parsingTime / totalTime / 1e4 ) ) + "%);\n- Solving (Connect 4): " + String.format( "%.1f", ( CommonVariables.solvingTime / 1e9 ) ) + " sec. (" + String.format( "%.1f", ( CommonVariables.solvingTime / totalTime / 1e4 ) ) + "%);\n- Constructing new sentences: " + String.format( "%.1f", ( CommonVariables.writingTime / 1e9 ) ) + " sec. (" + String.format( "%.1f", ( CommonVariables.writingTime / totalTime / 1e4 ) ) + "%);\n- Cleaning up after sentences: " + String.format( "%.1f", ( CommonVariables.cleaningTime / 1e9 ) ) + " sec. (" + String.format( "%.1f", ( CommonVariables.cleaningTime / totalTime / 1e4 ) ) + "%).\n" ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write the test statistics text" );
					}
				}

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

	protected CreateAndAssignResultType addSpecificationWithAuthorization( boolean isAssignment, boolean isConditional, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isEveryGeneralization, boolean isExclusiveSpecification, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSelection, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, boolean isValueSpecification, short assumptionLevel, short prepositionParameter, short questionParameter, short generalizationWordTypeNr, short specificationWordTypeNr, short relationWordTypeNr, int specificationCollectionNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int copiedRelationContextNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		CreateAndAssignResultType createAndAssignResult = new CreateAndAssignResultType();

		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( generalizationWordItem.isNounPassword() &&
		// Hide password
		specificationWordItem.hideWordType( specificationWordTypeNr, moduleNameString_ ) != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to hide a password" );

		if( ( createAndAssignResult = generalizationWordItem.addSpecificationInWord( isAssignment, isConditional, isInactiveAssignment, isArchivedAssignment, isEveryGeneralization, true, isExclusiveSpecification, isNegative, isPartOf, isPossessive, isSelection, isSpecificationGeneralization, isUniqueUserRelation, isValueSpecification, assumptionLevel, prepositionParameter, questionParameter, generalizationWordTypeNr, specificationWordTypeNr, relationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, copiedRelationContextNr, nContextRelations, firstJustificationItem, specificationWordItem, relationWordItem, specificationString, moduleNameString_ ) ).result != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to add a specification with authorization" );

		// Collect current language with previous languages
		if( adminItem_.collectGeneralizationWordWithPreviousOne( isAssignment, isPossessive, generalizationWordTypeNr, specificationWordTypeNr, specificationCollectionNr, generalizationContextNr, specificationContextNr, relationContextNr, generalizationWordItem, specificationWordItem ) != Constants.RESULT_OK )
			return adminItem_.addCreateAndAssignResultError( 1, moduleNameString_, "I failed to collect a generalization word with a previous one" );

		return createAndAssignResult;
		}

	protected CreateAndAssignResultType assignSpecificationWithAuthorization( boolean isAmbiguousRelationContext, boolean isAssignedOrClear, boolean isInactiveAssignment, boolean isArchivedAssignment, boolean isNegative, boolean isPartOf, boolean isPossessive, boolean isSpecificationGeneralization, boolean isUniqueUserRelation, short assumptionLevel, short prepositionParameter, short questionParameter, short relationWordTypeNr, int generalizationContextNr, int specificationContextNr, int relationContextNr, int originalSentenceNr, int activeSentenceNr, int inactiveSentenceNr, int archivedSentenceNr, int nContextRelations, JustificationItem firstJustificationItem, WordItem generalizationWordItem, WordItem specificationWordItem, String specificationString )
		{
		if( generalizationWordItem == null )
			return adminItem_.startCreateAndAssignResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		return generalizationWordItem.assignSpecification( isAmbiguousRelationContext, isAssignedOrClear, isInactiveAssignment, isArchivedAssignment, isNegative, isPartOf, isPossessive, isSpecificationGeneralization, isUniqueUserRelation, assumptionLevel, prepositionParameter, questionParameter, relationWordTypeNr, generalizationContextNr, specificationContextNr, relationContextNr, originalSentenceNr, activeSentenceNr, inactiveSentenceNr, archivedSentenceNr, nContextRelations, firstJustificationItem, specificationWordItem, specificationString, moduleNameString_ );
		}

	protected FileResultType readInfoFile( boolean isReportingErrorIfFileDoesNotExist, String infoFileNameString )
		{
		FileResultType fileResult = new FileResultType();
		byte originalResult;
		FileItem openedInfoFileItem;
		WordItem currentLanguageWordItem = CommonVariables.currentLanguageWordItem;
		StringBuffer infoPathStringBuffer = new StringBuffer( Constants.FILE_DATA_INFO_DIRECTORY_NAME_STRING );

		if( infoFileNameString == null )
			return adminItem_.startFileResultError( 1, moduleNameString_, "The given info file name string is undefined" );

		if( currentLanguageWordItem != null )
			infoPathStringBuffer.append( currentLanguageWordItem.anyWordTypeString() + Constants.SYMBOL_SLASH );

		if( ( fileResult = adminItem_.openFile( true, true, false, isReportingErrorIfFileDoesNotExist, infoPathStringBuffer.toString(), infoFileNameString, null, null ) ).result != Constants.RESULT_OK )
			return adminItem_.addFileResultError( 1, moduleNameString_, "I failed to open the info file" );

		if( ( openedInfoFileItem = fileResult.createdFileItem ) != null )
			{
			if( readAndExecute() != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to read and execute the opened info file" );

			originalResult = CommonVariables.result;

			if( closeCurrentFileItem( openedInfoFileItem ) != Constants.RESULT_OK )
				adminItem_.addError( 1, moduleNameString_, "I failed to close the info file item" );

			if( originalResult != Constants.RESULT_OK )
				CommonVariables.result = originalResult;
			}

		return fileResult;
		}
	};

/*************************************************************************
 *	"Shout to the Lord, all the earth;
 *	break out in praise and sing for joy!" (Psalm 98:4)
 *************************************************************************/
