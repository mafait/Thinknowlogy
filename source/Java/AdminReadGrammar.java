/*	Class:			AdminReadGrammar
 *	Supports class:	AdminItem
 *	Purpose:		To read and process grammar and interface files
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

class AdminReadGrammar
	{
	// Private constructed variables

	private boolean hasUsedPredefinedMultipleWord_;

	private WordItem multipleWordItem_;
	private WordItem predefinedAdjectiveBusyWordItem_;
	private WordItem predefinedAdjectiveDoneWordItem_;
	private WordItem predefinedAdjectiveInvertedWordItem_;
	private WordItem predefinedNounDeveloperWordItem_;
	private WordItem predefinedNounExpertWordItem_;
	private WordItem predefinedNounPasswordWordItem_;
	private WordItem predefinedNounSolveLevelWordItem_;
	private WordItem predefinedNounSolveMethodWordItem_;
	private WordItem predefinedNounSolveStrategyWordItem_;
	private WordItem predefinedNounStartupLanguageWordItem_;
	private WordItem predefinedVerbLoginWordItem_;

	private AdminItem adminItem_;
	private String moduleNameString_;


	// Private methods

	private static void clearPredefinedMultipleWordNrInAllWords()
		{
		WordItem currentWordItem;

		if( ( currentWordItem = CommonVariables.firstWordItem ) != null )
			{
			// Do for all (grammar) words
			do	currentWordItem.predefinedMultipleWordNr = 0;
			while( ( currentWordItem = currentWordItem.nextWordItem() ) != null );
			}
		}

	private byte addPredefinedMultipleWord( short nMultipleWords, short wordParameter, short wordTypeNr, WordItem multipleWordItem )
		{
		WordResultType wordResult;
		short predefinedMultipleWordNr = 0;
		WordItem foundMultipleWordItem;
		WordTypeItem foundWordTypeItem;
		StringBuffer multipleWordStringBuffer = new StringBuffer();

		multipleWordItem_ = multipleWordItem;

		if( nMultipleWords <= 0 )
			return adminItem_.startError( 1, moduleNameString_, "The given number of multiple words is undefined" );

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
				return adminItem_.addError( 1, moduleNameString_, "I failed to add a predefined grammar word" );

			multipleWordItem_ = wordResult.createdWordItem;
			}
		else
			{
			if( multipleWordItem.addWordType( true, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, wordTypeNr, multipleWordStringBuffer.length(), multipleWordStringBuffer.toString() ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "The given multiple word item is undefined" );
			}

		if( multipleWordItem_ == null )
			return adminItem_.startError( 1, moduleNameString_, "The multiple word item is undefined" );

		predefinedMultipleWordNr = 0;

		while( predefinedMultipleWordNr < nMultipleWords &&
		( foundMultipleWordItem = predefinedMultipleWordItem( ++predefinedMultipleWordNr ) ) != null )
			{
			if( foundMultipleWordItem.addMultipleWord( nMultipleWords, wordTypeNr, multipleWordItem_ ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to add a multiple word" );
			}

		return Constants.RESULT_OK;
		}

	private byte assignPredefinedWord( short grammarParameter, WordItem createdWordItem )
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
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined busy adjective word" );
					}

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_DONE:
				if( predefinedAdjectiveDoneWordItem_ == null )
					predefinedAdjectiveDoneWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveDoneWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined done adjective word" );
					}

				break;

			case Constants.WORD_PARAMETER_ADJECTIVE_INVERTED:
				if( predefinedAdjectiveInvertedWordItem_ == null )
					predefinedAdjectiveInvertedWordItem_ = createdWordItem;
				else
					{
					if( predefinedAdjectiveInvertedWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined inverted adjective word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_DEVELOPER:
				if( predefinedNounDeveloperWordItem_ == null )
					{
					predefinedNounDeveloperWordItem_ = createdWordItem;

					if( adminItem_.authorizeWord( predefinedNounDeveloperWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined developer noun word" );
					}
				else
					{
					if( predefinedNounDeveloperWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined developer noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_EXPERT:
				if( predefinedNounExpertWordItem_ == null )
					{
					predefinedNounExpertWordItem_ = createdWordItem;

					if( adminItem_.authorizeWord( predefinedNounExpertWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined expert noun word" );
					}
				else
					{
					if( predefinedNounExpertWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined expert noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_LANGUAGE:
				if( CommonVariables.predefinedNounLanguageWordItem == null )
					{
					CommonVariables.predefinedNounLanguageWordItem = createdWordItem;

					// During the creation of the first language,
					// the predefined noun language word wasn't defined yet
					// So, create first language specification afterwards
					if( adminItem_.createLanguageSpecification( createdWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to create a language specification" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_PASSWORD:
				if( predefinedNounPasswordWordItem_ == null )
					{
					predefinedNounPasswordWordItem_ = createdWordItem;

					if( adminItem_.authorizeWord( predefinedNounPasswordWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined password noun word" );
					}
				else
					{
					if( predefinedNounPasswordWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined password noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_SOLVE_LEVEL:
				if( predefinedNounSolveLevelWordItem_ == null )
					predefinedNounSolveLevelWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveLevelWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined solve level noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_SOLVE_METHOD:
				if( predefinedNounSolveMethodWordItem_ == null )
					predefinedNounSolveMethodWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveMethodWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined solve method noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_SOLVE_STRATEGY:
				if( predefinedNounSolveStrategyWordItem_ == null )
					predefinedNounSolveStrategyWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounSolveStrategyWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined solve strategy noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_STARTUP_LANGUAGE:
				if( predefinedNounStartupLanguageWordItem_ == null )
					predefinedNounStartupLanguageWordItem_ = createdWordItem;
				else
					{
					if( predefinedNounStartupLanguageWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined startup language word" );
					}

				break;

			case Constants.WORD_PARAMETER_NOUN_USER:
				if( CommonVariables.predefinedNounUserWordItem == null )
					{
					CommonVariables.predefinedNounUserWordItem = createdWordItem;

					if( adminItem_.authorizeWord( CommonVariables.predefinedNounUserWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined user noun word" );
					}
				else
					{
					if( CommonVariables.predefinedNounUserWordItem != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined user noun word" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( predefinedVerbLoginWordItem_ == null )
					{
					predefinedVerbLoginWordItem_ = createdWordItem;

					if( adminItem_.authorizeWord( predefinedVerbLoginWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to authorize the predefined login verb word" );
					}
				else
					{
					if( predefinedVerbLoginWordItem_ != createdWordItem )
						return adminItem_.startError( 1, moduleNameString_, "I have found a different predefined login verb word" );
					}

				break;
			}

		return Constants.RESULT_OK;
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

	protected AdminReadGrammar( AdminItem adminItem )
		{
		String errorString = null;

		hasUsedPredefinedMultipleWord_ = false;

		multipleWordItem_ = null;
		predefinedAdjectiveBusyWordItem_ = null;
		predefinedAdjectiveDoneWordItem_ = null;
		predefinedAdjectiveInvertedWordItem_ = null;

		predefinedNounDeveloperWordItem_ = null;
		predefinedNounExpertWordItem_ = null;
		predefinedNounPasswordWordItem_ = null;
		predefinedNounSolveLevelWordItem_ = null;
		predefinedNounSolveMethodWordItem_ = null;
		predefinedNounSolveStrategyWordItem_ = null;
		predefinedNounStartupLanguageWordItem_ = null;
		predefinedVerbLoginWordItem_ = null;

		adminItem_ = adminItem;
		moduleNameString_ = this.getClass().getName();

		if( adminItem_ == null )
			errorString = "The given admin is undefined";

		if( errorString != null )
			{
			if( adminItem_ != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
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
		int endPosition;
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

		if( hasUsedPredefinedMultipleWord_ )
			{
			hasUsedPredefinedMultipleWord_ = false;
			clearPredefinedMultipleWordNrInAllWords();
			}

		do	{
			if( ( readResult = adminItem_.readWordFromString( true, isMergedWord, false, grammarPosition, 0, grammarString ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to read a word from the grammar string" );

			hasFoundWordDefinitionInfo = false;

			switch( grammarString.charAt( grammarPosition ) )
				{
				case Constants.QUERY_WORD_TYPE_CHAR:
					if( hasWordTypeNr )
						return adminItem_.startError( 1, moduleNameString_, "I have found more than one word type parameters defined in a grammar definition line" );

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
						return adminItem_.startError( 1, moduleNameString_, "I have found more than one values parameters defined in a grammar definition line" );

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

					if( ( readResult = adminItem_.readWordFromString( false, isMergedWord, false, ++grammarPosition, 0, grammarString ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to read a word definition from the grammar string" );

					if( ( grammarResult = currentLanguageWordItem.findGrammar( ( grammarParameter >= Constants.GRAMMAR_SENTENCE ), grammarParameter, readResult.wordLength, grammarString.substring( grammarPosition ) ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to find a grammar definition item" );

					hasFoundWordDefinitionInfo = true;

					if( !hasWordTypeNr ||
					grammarParameter >= Constants.GRAMMAR_SENTENCE ||
					grammarResult.foundGrammarItem == null )
						{
						if( ( grammarResult = currentLanguageWordItem.createGrammarItem( true, ( hasParameter && grammarParameter < Constants.GRAMMAR_SENTENCE ), false, false, false, wordTypeNr, grammarParameter, readResult.wordLength, grammarString.substring( grammarPosition ), null ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add a grammar definition word item" );

						firstCreationItemNr = CommonVariables.currentItemNr;
						definitionGrammarItem = grammarResult.createdGrammarItem;
						}
					else
						{
						if( Presentation.writeInterfaceText( Constants.PRESENTATION_PROMPT_NOTIFICATION, Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_START, grammarResult.foundGrammarItem.grammarParameter(), Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_MIDDLE, currentLanguageWordItem.anyWordTypeString(), Constants.INTERFACE_GRAMMAR_PARAMETER_DEFINED_MORE_THAN_ONCE_END ) != Constants.RESULT_OK )
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
						return adminItem_.startError( 1, moduleNameString_, "I have found an extra square bracket character in the grammar definition" );

					if( isChoiceCheck == isChoice )
						{
						if( hasFoundPipe )
							return adminItem_.startError( 1, moduleNameString_, "I have found an extra pipe character before a square bracket in the grammar definition" );

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
						return adminItem_.startError( 1, moduleNameString_, "I have found an extra curved bracket character in the grammar definition" );

					if( isOptionCheck == isOption )
						{
						if( hasFoundPipe )
							return adminItem_.startError( 1, moduleNameString_, "I have found an extra pipe character before a curved bracket in the grammar definition" );

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
						return adminItem_.startError( 1, moduleNameString_, "I have found an extra pipe character in the grammar definition" );

					if( !isOption &&
					!isChoice )
						return adminItem_.startError( 1, moduleNameString_, "Pipes are only allowed within grammar definition options or choices" );

					hasFoundPipe = true;

					if( isChoice )
						hasFoundChoiceAlternatives = true;

					break;

				// Either interface definition or Guide by Grammar
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
							return adminItem_.startError( 1, moduleNameString_, "I have found a corrupted interface definition" );

						if( currentLanguageWordItem.checkInterface( grammarParameter, grammarString.substring( grammarPosition ) ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add an interface definition word item" );

						if( currentLanguageWordItem.createInterfaceItem( grammarParameter, ( grammarStringLength - grammarPosition - 1 ), grammarString.substring( grammarPosition ) ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to add an interface definition word item" );

						hasCreatedInterface = true;
						grammarPosition = ( grammarStringLength - 1 );
						}
					else
						{
						// Guide by Grammar
						endPosition = grammarPosition + 1;

						while( endPosition < grammarStringLength &&
						// Find end of string
						grammarString.charAt( endPosition ) != Constants.SYMBOL_DOUBLE_QUOTE )
							endPosition++;

						if( endPosition >= grammarStringLength )
							return adminItem_.startError( 1, moduleNameString_, "The Guide by Grammar string is corrupt" );

						if( ( definitionGrammarItem.guideByGrammarString = grammarString.substring( grammarPosition + 1, endPosition ) ) == null )
							return adminItem_.startError( 1, moduleNameString_, "I failed to create the Guide by Grammar string" );
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

								if( ( grammarResult = currentLanguageWordItem.createGrammarItem( false, false, false, false, false, Constants.NO_WORD_TYPE_NR, grammarParameter, readResult.wordLength, grammarString.substring( grammarPosition ), definitionGrammarItem ) ).result != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to add a grammar definition word item" );

								if( definitionGrammarItem.nextDefinitionGrammarItem != null )
									return adminItem_.startError( 1, moduleNameString_, "The next definition grammar item is already defined" );

								definitionGrammarItem.nextDefinitionGrammarItem = grammarResult.createdGrammarItem;
								}
							else
								{
								foundWordItem = null;

								if( ( readResult.wordLength + grammarPosition ) < grammarStringLength )
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
									if( ( wordResult = adminItem_.addWord( false, false, Constants.NO_ADJECTIVE_PARAMETER, Constants.NO_DEFINITE_ARTICLE_PARAMETER, Constants.NO_INDEFINITE_ARTICLE_PARAMETER, grammarParameter, wordTypeNr, readResult.wordLength, grammarString.substring( grammarPosition ) ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add a grammar word" );

									if( ( createdWordItem = wordResult.createdWordItem ) == null )
										return adminItem_.startError( 1, moduleNameString_, "I couldn't create a grammar word" );

									if( isMultipleWord )
										{
										hasUsedPredefinedMultipleWord_ = true;
										createdWordItem.predefinedMultipleWordNr = ++predefinedMultipleWordNr;

										if( isLastPartOfMultipleWord )
											{
											if( addPredefinedMultipleWord( predefinedMultipleWordNr, grammarParameter, wordTypeNr, foundWordItem ) != Constants.RESULT_OK )
												return adminItem_.addError( 1, moduleNameString_, "I failed to add a predefined multiple grammar word" );

											if( ( createdWordItem = multipleWordItem_ ) == null )
												return adminItem_.startError( 1, moduleNameString_, "I couldn't create a predefined multiple grammar word" );
											}
										}

									if( isEndOfLine )
										{
										if( assignPredefinedWord( grammarParameter, createdWordItem ) != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to assign a predefined word" );
										}
									}
								else
									{
									if( foundWordItem.addWordType( false, false, ( adminItem_.isAdjectiveParameter( grammarParameter ) ? grammarParameter : Constants.NO_ADJECTIVE_PARAMETER ), ( adminItem_.isDefiniteArticleParameter( grammarParameter ) ? grammarParameter : Constants.NO_DEFINITE_ARTICLE_PARAMETER ), ( adminItem_.isIndefiniteArticleParameter( grammarParameter ) ? grammarParameter : Constants.NO_INDEFINITE_ARTICLE_PARAMETER ), wordTypeNr, readResult.wordLength, grammarString.substring( grammarPosition ) ).result != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to add another word type to grammar word \"" + foundWordItem.anyWordTypeString() + "\"" );
									}
								}
							}
						}
					else
						{
						// Get grammar identification
						if( ( grammarResult = currentLanguageWordItem.findGrammar( true, Constants.NO_GRAMMAR_PARAMETER, readResult.wordLength, grammarString.substring( grammarPosition ) ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to find a grammar definition word item" );

						foundGrammarItem = grammarResult.foundGrammarItem;

						if( ( grammarResult = currentLanguageWordItem.createGrammarItem( false, ( hasFoundPipe || isNewStart ), isOptionStart, isChoiceStart, isSkipOptionForWriting, Constants.NO_WORD_TYPE_NR, Constants.NO_GRAMMAR_PARAMETER, readResult.wordLength, grammarString.substring( grammarPosition ), foundGrammarItem ) ).result != Constants.RESULT_OK )
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
			!readResult.hasFoundGrammarDefinition &&
			readResult.nextWordPosition < grammarStringLength )
				{
				if( ( readResult = adminItem_.readWordFromString( false, false, false, grammarPosition, 0, grammarString ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to read a new word from the grammar string" );
				}

			grammarPosition = readResult.nextWordPosition;
			}
		while( readResult.nextWordPosition < grammarStringLength );

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

	protected WordItem predefinedNounDeveloperWordItem()
		{
		return predefinedNounDeveloperWordItem_;
		}

	protected WordItem predefinedNounExpertWordItem()
		{
		return predefinedNounExpertWordItem_;
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

	protected WordItem predefinedNounStartupLanguageWordItem()
		{
		return predefinedNounStartupLanguageWordItem_;
		}

	protected WordItem predefinedVerbLoginWordItem()
		{
		return predefinedVerbLoginWordItem_;
		}
	};

/*************************************************************************
 *	"The Lord is king!
 *	Let the nations tremble!
 *	He sits on his throne between the cherubim.
 *	Let the whole earth quake!" (Psalm 99:1)
 *************************************************************************/
