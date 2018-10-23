/*	Class:			AdminImperative
 *	Supports class:	AdminItem
 *	Purpose:		To execute imperative words
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

class AdminImperative
	{
	// Private constructed variables

	private boolean hasRequestedRestart_ = false;

	private int oldGlobalSatisfiedScore_ = Constants.NO_SCORE;
	private int newGlobalSatisfiedScore_ = Constants.NO_SCORE;
	private int oldGlobalDissatisfiedScore_ = Constants.NO_SCORE;
	private int newGlobalDissatisfiedScore_ = Constants.NO_SCORE;
	private int oldGlobalNotBlockingScore_ = Constants.NO_SCORE;
	private int newGlobalNotBlockingScore_ = Constants.NO_SCORE;
	private int oldGlobalBlockingScore_ = Constants.NO_SCORE;
	private int newGlobalBlockingScore_ = Constants.NO_SCORE;

	private SpecificationItem firstComparisonAssignmentItem_ = null;
	private SpecificationItem secondComparisonAssignmentItem_ = null;
	private SpecificationItem virtualListAssignmentItem_ = null;

	private String moduleNameString_ = this.getClass().getName();

	// Private initialized variables

	private WordItem predefinedAdjectiveBusyWordItem_ = null;
	private WordItem predefinedAdjectiveDoneWordItem_ = null;
	private WordItem predefinedAdjectiveInvertedWordItem_ = null;
	private WordItem predefinedNounSolveLevelWordItem_ = null;
	private WordItem predefinedNounSolveMethodWordItem_ = null;
	private WordItem predefinedNounSolveStrategyWordItem_ = null;

	private AdminItem adminItem_ = null;


	// Private methods

	private static boolean isNumeralString( String checkString )
		{
		int position = 0;
		int stringLength;

		if( checkString != null &&
		( stringLength = checkString.length() ) > 0 )
			{
			while( position < stringLength &&
			Character.isDigit( checkString.charAt( position ) ) )
				position++;

			return ( position == stringLength );
			}

		return false;
		}

	private byte addWordToVirtualList( boolean isSelection, short generalizationWordTypeNr, short specificationWordTypeNr, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		if( generalizationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem.addSpecificationInWord( true, false, false, false, false, false, false, false, false, false, false, false, isSelection, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, Constants.NO_WORD_TYPE_NR, Constants.NO_COLLECTION_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, specificationWordItem, null, null, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to add a virtual list specification" );

		if( generalizationWordItem.assignSpecification( false, false, false, false, false, false, false, false, Constants.NO_ASSUMPTION_LEVEL, Constants.NO_PREPOSITION_PARAMETER, Constants.NO_QUESTION_PARAMETER, Constants.NO_WORD_TYPE_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, Constants.NO_CONTEXT_NR, 0, null, specificationWordItem, null, null ).result != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to assign a virtual list word" );

		return Constants.RESULT_OK;
		}

	private static byte createNewAssignmentLevelInAssignmentWords()
		{
		WordItem currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = GlobalVariables.firstAssignmentWordItem ) != null )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem.createNewAssignmentLevelInWord() == Constants.RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem.nextAssignmentWordItem ) != null );
			}

		return GlobalVariables.result;
		}

	private static byte deleteAssignmentLevelInAssignmentWords()
		{
		WordItem currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = GlobalVariables.firstAssignmentWordItem ) != null )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem.deleteAssignmentLevelInWord() == Constants.RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem.nextAssignmentWordItem ) != null );
			}

		return GlobalVariables.result;
		}

	private byte executeImperativeDisplay( short executionNounWordParameter, short executionWordTypeNr, WordItem imperativeVerbWordItem, WordItem executionWordItem, String executionString )
		{
		String singularNounString;
		FileResultType fileResult;

		if( imperativeVerbWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given imperative verb word item is undefined" );

		if( executionString != null ||
		executionWordItem != null )
			{
			if( executionString != null &&
			executionWordTypeNr == Constants.WORD_TYPE_TEXT )
				{
				if( GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL &&
				writeInfoTextWithPossibleQueryCommands( executionString ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write the execution string" );
				}
			else
				{
				adminItem_.initializeAdminWriteVariables();

				switch( executionNounWordParameter )
					{
					case Constants.WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
						if( adminItem_.writeJustificationReport( executionWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write justification for a word" );

						break;

					case Constants.WORD_PARAMETER_NOUN_INFORMATION:
						// Try to display all knowledge about this specification
						if( adminItem_.writeInfoAboutWord( false, true, true, true, true, true, true, true, executionWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write info about a word" );

						// Don't insert a break statement here

					default:
						// Try to read the info file with the given specification name
						if( executionWordItem == null )
							return adminItem_.startError( 1, moduleNameString_, "The specification word item is undefined" );

						if( ( fileResult = adminItem_.readInfoFile( false, executionWordItem.activeWordTypeString( executionWordTypeNr ) ) ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to read the info file" );

						if( fileResult.createdFileItem == null &&
						executionWordTypeNr == Constants.WORD_TYPE_NOUN_PLURAL &&
						( singularNounString = executionWordItem.singularNounString() ) != null &&
						// In case of a plural noun, also try a singular noun
						adminItem_.readInfoFile( false, singularNounString ).result != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to read the info file with a singular noun word" );
					}

				if( !GlobalVariables.hasDisplayedMessage &&
				InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_I_DONT_HAVE_THE_REQUESTED_INFO ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			}
		else
			{
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListImperativePart( short imperativeVerbParameter, short prepositionParameter, short specificationWordTypeNr, short relationWordTypeNr, WordItem imperativeVerbWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		boolean hasFoundHeadOrTail = false;
		boolean hasFoundVirtualListAction = false;
		SpecificationItem localVirtualListAssignmentItem;

		if( imperativeVerbWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given imperative verb word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given relation word item is undefined" );

		switch( imperativeVerbParameter )
			{
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
				if( !specificationWordItem.isNounHead() &&
				!specificationWordItem.isNounTail() )
					{
					if( addWordToVirtualList( false, relationWordTypeNr, specificationWordTypeNr, relationWordItem, specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to add a word to a virtual list" );
					}
				else
					{
					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
				switch( prepositionParameter )
					{
					// Head or tail
					case Constants.WORD_PARAMETER_PREPOSITION_OF:
						if( specificationWordItem.isNounHead() )
							{
							hasFoundHeadOrTail = true;

							if( ( virtualListAssignmentItem_ = relationWordItem.lastActiveNonQuestionAssignmentItem() ) == null )
								hasFoundVirtualListAction = true;
							else
								{
								if( relationWordItem.inactivateActiveAssignment( virtualListAssignmentItem_ ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to inactivate the head of a virtual list" );
								}
							}
						else
							{
							if( specificationWordItem.isNounTail() )
								{
								hasFoundHeadOrTail = true;

								if( ( virtualListAssignmentItem_ = relationWordItem.firstNonQuestionActiveAssignmentItem() ) == null )
									hasFoundVirtualListAction = true;
								else
									{
									if( relationWordItem.inactivateActiveAssignment( virtualListAssignmentItem_ ) != Constants.RESULT_OK )
										return adminItem_.addError( 1, moduleNameString_, "I failed to inactivate the tail of a virtual list" );
									}
								}
							}

					// Don't insert a break statement here

					case Constants.WORD_PARAMETER_PREPOSITION_FROM:
						if( !hasFoundHeadOrTail )
							{
							if( ( virtualListAssignmentItem_ = relationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, specificationWordItem ) ) == null )
								hasFoundVirtualListAction = true;
							else
								{
								if( relationWordItem.inactivateActiveAssignment( virtualListAssignmentItem_ ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to inactivate the word in a virtual list" );
								}
							}

						break;

					case Constants.WORD_PARAMETER_PREPOSITION_TO:
					// Typical for Chinese
					case Constants.WORD_PARAMETER_PREPOSITION_CHINESE_VERB_ADD:
					case Constants.WORD_PARAMETER_PREPOSITION_CHINESE_VERB_MOVE:
					case Constants.WORD_PARAMETER_PREPOSITION_CHINESE_VERB_REMOVE:
						// Global virtual assignment list
						if( virtualListAssignmentItem_ == null )
							{
							if( !hasFoundVirtualListAction &&
							InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
							}
						else
							{
							if( addWordToVirtualList( false, virtualListAssignmentItem_.generalizationWordTypeNr(), virtualListAssignmentItem_.specificationWordTypeNr(), relationWordItem, virtualListAssignmentItem_.specificationWordItem() ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to add a word to a virtual list" );
							}

						break;

					default:
						if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_PREPOSITION_NOT_USED_FOR_THIS_ACTION ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the preposition parameter" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				localVirtualListAssignmentItem = ( specificationWordItem.isNounHead() ? relationWordItem.lastActiveNonQuestionAssignmentItem() :
												( specificationWordItem.isNounTail() ? relationWordItem.firstNonQuestionActiveAssignmentItem() :
												relationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, false, specificationWordItem ) ) );

				if( localVirtualListAssignmentItem != null &&
				relationWordItem.inactivateActiveAssignment( localVirtualListAssignmentItem ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to inactivate the head of a virtual list" );

				break;

			default:
				return adminItem_.startError( 1, moduleNameString_, "The given imperative parameter isn't a virtual list action" );
			}

		return Constants.RESULT_OK;
		}

	private byte executeVirtualListImperative( short imperativeVerbParameter, short specificationWordTypeNr, WordItem imperativeVerbWordItem, WordItem specificationWordItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem )
		{
		short prepositionParameter = Constants.NO_PREPOSITION_PARAMETER;
		ReadItem currentReadItem = startRelationReadItem;
		WordItem relationWordItem;

		if( imperativeVerbWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given imperative verb word item is undefined" );

		if( specificationWordItem == null )
			{
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
			}
		else
			{
			if( startRelationReadItem == null )
				{
				if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
				}
			else
				{
				do	{
					if( currentReadItem.isPreposition() )
						prepositionParameter = currentReadItem.wordParameter();
					else
						{
						if( currentReadItem.isRelationWord() &&
						( relationWordItem = currentReadItem.readWordItem() ) != null &&

						executeVirtualListImperativePart( imperativeVerbParameter, prepositionParameter, specificationWordTypeNr, currentReadItem.wordTypeNr(), imperativeVerbWordItem, specificationWordItem, relationWordItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to execute a virtual list imperative part" );
						}
					}
				while( currentReadItem != endRelationReadItem &&
				( currentReadItem = currentReadItem.nextReadItem() ) != null );
				}
			}

		return Constants.RESULT_OK;
		}

	private byte solveWord( short solveLevel, int currentSolveProgressLevel, int endSolveProgressLevel, WordItem solveWordItem, SelectionItem actionSelectionItem )
		{
		boolean isAllowingDuplicates;
		boolean isInverted;
		boolean isPreparingSort;
		short currentAssignmentLevel = GlobalVariables.currentAssignmentLevel;
		int nPossibilities;
		int possibilityNumber = 0;
		int solveProgressStep;
		int nextSolveProgressLevel;
		ScoreItem possibilityScoreItem;
		BoolResultType boolResult;
		SelectionResultType selectionResult;
		ShortResultType shortResult;

		if( solveWordItem == null )
			return adminItem_.startError( 1, moduleNameString_, "The given solve word is undefined" );

		// Word has no active assignments
		if( solveWordItem.firstNonQuestionActiveAssignmentItem() == null )
			{
			if( currentAssignmentLevel > solveLevel )
				return adminItem_.startError( 1, moduleNameString_, "The given assignment level of " + currentAssignmentLevel + " is higher than the given solve level " + solveLevel );

			if( adminItem_.assignSpecification( predefinedNounSolveMethodWordItem_, predefinedAdjectiveBusyWordItem_ ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to assign the busy flag to the solve method at assignment level " + currentAssignmentLevel );

			// There already is a scores list at the start
			if( adminItem_.hasScoreList() &&
			currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL &&
			adminItem_.deleteScores() != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to delete the admin score list" );

			// Get solve strategy parameter
			if( ( shortResult = getAssignmentWordParameter( predefinedNounSolveStrategyWordItem_ ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve strategy at assignment level " + currentAssignmentLevel );

			isAllowingDuplicates = ( currentAssignmentLevel == solveLevel );
			isInverted = ( predefinedNounSolveMethodWordItem_.firstNonQuestionAssignmentItem( true, false, false, false, false, predefinedAdjectiveInvertedWordItem_ ) != null );
			isPreparingSort = ( currentAssignmentLevel + 1 < solveLevel );

			if( ( boolResult = findSelectionToSolveWord( true, isAllowingDuplicates, isInverted, isPreparingSort, shortResult.shortValue, solveWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to find a selection to solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + currentAssignmentLevel );

			// Has found possibility to solve word
			if( boolResult.booleanValue )
				{
				if( currentAssignmentLevel < solveLevel )
					{
					if( ( nPossibilities = adminItem_.nPossibilities() ) <= 0 )
						return adminItem_.startError( 1, moduleNameString_, "There are no possibilities at assignment level " + currentAssignmentLevel );

					solveProgressStep = ( nPossibilities == 0 ? Constants.MAX_PROGRESS : ( ( endSolveProgressLevel - currentSolveProgressLevel ) / nPossibilities ) );

					if( solveLevel > 1 )
						InputOutput.startProgress( Constants.INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_START, solveLevel, Constants.INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_END, currentSolveProgressLevel, Constants.MAX_PROGRESS );

					if( ( possibilityScoreItem = adminItem_.firstPossibility() ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't find any possibility item at assignment level " + currentAssignmentLevel );

					do	{
						// Copy solve action of Constants.NO_ASSIGNMENT_LEVEL to higher levels
						if( GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
							actionSelectionItem = possibilityScoreItem.referenceSelectionItem;

						if( createNewAssignmentLevelInAssignmentWords() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to create a new assignment level: " + GlobalVariables.currentAssignmentLevel );

						GlobalVariables.currentAssignmentLevel++;

						if( adminItem_.assignSelectionSpecification( possibilityScoreItem.referenceSelectionItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to assign a selection specifcation at assignment level: " + GlobalVariables.currentAssignmentLevel );

						nextSolveProgressLevel = currentSolveProgressLevel + solveProgressStep;

						if( executeSelection( (int)( currentSolveProgressLevel + solveProgressStep / 2L ), actionSelectionItem ) != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to execute a selection during the solving of word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + GlobalVariables.currentAssignmentLevel );

						// Word has active assignments
						if( solveWordItem.firstNonQuestionActiveAssignmentItem() == null )
							{
							if( solveWord( solveLevel, currentSolveProgressLevel, nextSolveProgressLevel, solveWordItem, actionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + GlobalVariables.currentAssignmentLevel );

							if( GlobalVariables.currentAssignmentLevel == 1 &&
							adminItem_.changeAction( actionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to change an action" );
							}
						else
							{
							// There is a winning or losing score
							isInverted = ( predefinedNounSolveMethodWordItem_.firstNonQuestionAssignmentItem( true, false, false, false, false, predefinedAdjectiveInvertedWordItem_ ) != null );

							if( !isInverted &&
							GlobalVariables.currentAssignmentLevel < solveLevel )
								{
								if( adminItem_.deleteScores() != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to delete the scores with an assignment level higher than " + GlobalVariables.currentAssignmentLevel );

								// Don't solve any deeper
								solveLevel = GlobalVariables.currentAssignmentLevel;
								}

							// Create winning or losing score
							if( adminItem_.createScoreItem( false, Constants.NO_SCORE, ( isInverted ? Constants.NO_SCORE : Constants.WINNING_SCORE ), Constants.NO_SCORE, ( isInverted ? Constants.WINNING_SCORE : Constants.NO_SCORE ), Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, actionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addError( 1, moduleNameString_, "I failed to create a winning or losing score of solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + GlobalVariables.currentAssignmentLevel );
							}

						currentSolveProgressLevel = nextSolveProgressLevel;

						if( deleteAssignmentLevelInAssignmentWords() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete the assignments of level " + GlobalVariables.currentAssignmentLevel );

						GlobalVariables.currentAssignmentLevel--;
						possibilityScoreItem = possibilityScoreItem.nextPossibilityScoreItem();

						if( ++possibilityNumber <= nPossibilities )
							{
							if( possibilityScoreItem != null &&
							possibilityNumber == nPossibilities )
								return adminItem_.startError( 1, moduleNameString_, "I found more possibility items than number of possibilities" );
							}
						else
							{
							if( possibilityScoreItem == null )
								return adminItem_.startError( 1, moduleNameString_, "I couldn't get the next possibility item before the number of possibilities is reached" );
							}
						}
					while( possibilityScoreItem != null );

					if( nPossibilities > 1 ||
					// Higher level has possibilities
					GlobalVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL )
						{
						if( adminItem_.deleteScores() != Constants.RESULT_OK )
							return adminItem_.addError( 1, moduleNameString_, "I failed to delete the scores with assignment level " + GlobalVariables.currentAssignmentLevel );
						}
					}

				currentSolveProgressLevel = endSolveProgressLevel;

				if( solveLevel > 1 )
					InputOutput.displayProgressBar( currentSolveProgressLevel );

				if( GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
					{
					// Get solve strategy parameter
					if( ( shortResult = getAssignmentWordParameter( predefinedNounSolveStrategyWordItem_ ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve strategy at assignment level " + GlobalVariables.currentAssignmentLevel );

					// Get best selection
					if( ( selectionResult = adminItem_.getBestSelection( adminItem_.isCurrentlyTesting(), shortResult.shortValue ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get the best selection action of solve word \"" + solveWordItem.anyWordTypeString() + "\" at assignment level " + GlobalVariables.currentAssignmentLevel );

					if( ( actionSelectionItem = selectionResult.selectionItem ) == null )
						return adminItem_.startError( 1, moduleNameString_, "I couldn't get the best action selection item" );

					// Assign best selection
					if( adminItem_.assignSelectionSpecification( actionSelectionItem ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to assign a selection specification at assignment level " + GlobalVariables.currentAssignmentLevel );

					// Set solve method to 'done'
					if( adminItem_.assignSpecification( predefinedNounSolveMethodWordItem_, predefinedAdjectiveDoneWordItem_ ) != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to assign the done flag to the solve method at assignment level " + GlobalVariables.currentAssignmentLevel );
					}
				}
			else
				{
				// Display warning
				if( currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL &&
				InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_START, solveWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_END ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
				}
			}
		else
			{
			// Display warning
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_START, solveWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_END ) != Constants.RESULT_OK )
				return adminItem_.addError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return Constants.RESULT_OK;
		}

	private byte writeInfoTextWithPossibleQueryCommands( String textString )
		{
		boolean hasFoundNewLine = false;
		int position = 0;
		int textStringLength;
		char textChar = Constants.SYMBOL_QUESTION_MARK;
		StringBuffer writeStringBuffer = new StringBuffer();
		QueryResultType queryResult;

		if( textString == null )
			return adminItem_.startError( 1, moduleNameString_, "The given text string is undefined" );

		textStringLength = textString.length();

		if( textString.charAt( 0 ) == Constants.SYMBOL_DOUBLE_QUOTE )
			position++;

		while( position < textStringLength &&
		textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE )
			{
			if( textString.charAt( position ) == Constants.QUERY_CHAR )
				{
				if( ++position >= textStringLength )
					return adminItem_.startError( 1, moduleNameString_, "The text string ended with a query character" );

				if( ( queryResult = adminItem_.executeQuery( true, false, true, Constants.INPUT_OUTPUT_PROMPT_INFO, position, textString ) ).result != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to execute query \"" + textString.substring( position ) + "\"" );

				position = queryResult.queryCommandStringPosition;
				}
			else
				{
				if( textString.charAt( position ) == Constants.SYMBOL_BACK_SLASH )
					{
					if( ++position >= textStringLength )
						return adminItem_.startError( 1, moduleNameString_, "The text string ended with a diacritical sign" );

					if( ( textChar = InputOutput.convertDiacriticalChar( textString.charAt( position ) ) ) == Constants.NEW_LINE_CHAR )
						hasFoundNewLine = true;
					}
				else
					textChar = textString.charAt( position );

				position++;
				writeStringBuffer.append( textChar );
				}

			if( hasFoundNewLine ||

			( position < textStringLength &&
			textString.charAt( position ) != Constants.SYMBOL_DOUBLE_QUOTE &&
			textString.charAt( position ) == Constants.QUERY_CHAR &&
			writeStringBuffer.length() > 0 ) )
				{
				if( InputOutput.writeText( false, Constants.INPUT_OUTPUT_PROMPT_INFO, Constants.NO_CENTER_WIDTH, writeStringBuffer ) != Constants.RESULT_OK )
					return adminItem_.addError( 1, moduleNameString_, "I failed to write a character" );

				hasFoundNewLine = false;
				writeStringBuffer = new StringBuffer();
				}
			}

		if( writeStringBuffer.length() > 0 &&
		InputOutput.writeText( false, Constants.INPUT_OUTPUT_PROMPT_INFO, Constants.NO_CENTER_WIDTH, writeStringBuffer ) != Constants.RESULT_OK )
			return adminItem_.addError( 1, moduleNameString_, "I failed to write the last characters" );

		return Constants.RESULT_OK;
		}

	private BoolResultType backTrackSelectionConditions( boolean isInitializeScores, boolean isInverted, boolean isAllowingDuplicates, boolean isPreparingSort, short executionLevel, short solveStrategyParameter, int conditionSentenceNr )
		{
		boolean hasFoundScore = false;
		boolean isNewStart;
		boolean isWaitingForNewLevel;
		boolean isWaitingForNewStart;
		short handledSelectionLevel;
		short previousSelectionLevel;
		short selectionLevel;
		short unhandledSelectionLevel;
		SelectionItem conditionSelectionItem;
		SelectionItem previousConditionSelectionItem;
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult = new BoolResultType();

		adminItem_.clearConditionChecksForSolving( Constants.MAX_LEVEL, conditionSentenceNr );

		do	{
			isWaitingForNewLevel = false;
			isWaitingForNewStart = false;
			handledSelectionLevel = Constants.MAX_LEVEL;
			previousSelectionLevel = Constants.NO_SELECTION_LEVEL;
			unhandledSelectionLevel = Constants.MAX_LEVEL;
			previousConditionSelectionItem = null;

			if( ( conditionSelectionItem = adminItem_.firstConditionSelectionItem( conditionSentenceNr ) ) == null )
				return adminItem_.startBoolResultError( 1, moduleNameString_, "I couldn't get the first item of the condition with sentence number " + conditionSentenceNr );

			if( isInitializeScores )
				{
				oldGlobalSatisfiedScore_ = Constants.NO_SCORE;
				newGlobalSatisfiedScore_ = Constants.NO_SCORE;
				oldGlobalDissatisfiedScore_ = Constants.NO_SCORE;
				newGlobalDissatisfiedScore_ = Constants.NO_SCORE;
				oldGlobalNotBlockingScore_ = Constants.NO_SCORE;
				newGlobalNotBlockingScore_ = Constants.NO_SCORE;
				oldGlobalBlockingScore_ = Constants.NO_SCORE;
				newGlobalBlockingScore_ = Constants.NO_SCORE;
				}

			do	{
				isNewStart = conditionSelectionItem.isNewStart();
				selectionLevel = conditionSelectionItem.selectionLevel();

				if( conditionSelectionItem.isConditionCheckedForSolving )
					isWaitingForNewStart = true;
				else
					{
					if( isNewStart &&
					// Not first start item
					previousConditionSelectionItem != null )
						{
						isWaitingForNewStart = false;

						// Second branch on the same level
						if( selectionLevel == previousSelectionLevel )
							{
							if( !isWaitingForNewLevel )
								{
								if( handledSelectionLevel == Constants.MAX_LEVEL )
									{
									handledSelectionLevel = selectionLevel;

									// This is a new branch
									if( previousConditionSelectionItem.isConditionCheckedForSolving )
										conditionSelectionItem.isConditionCheckedForSolving = true;
									else
										{
										// The previous branch was a new branch and this one is unhandled
										isWaitingForNewLevel = true;
										unhandledSelectionLevel = selectionLevel;
										previousConditionSelectionItem.isConditionCheckedForSolving = true;
										}
									}
								else
									{
									isWaitingForNewLevel = true;

									// This branch isn't handled yet
									if( unhandledSelectionLevel == Constants.MAX_LEVEL )
										unhandledSelectionLevel = selectionLevel;
									}
								}
							}
						else
							// New start on a new level
							isWaitingForNewLevel = false;
						}

					if( !isWaitingForNewLevel &&
					!isWaitingForNewStart )
						{
						if( ( boolCheckResult = scoreSelectionCondition( isInverted, isAllowingDuplicates, isPreparingSort, solveStrategyParameter, conditionSelectionItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to score a selection condition" );

						if( boolCheckResult.booleanValue )
							boolReturnResult.booleanValue = true;
						}
					}

				previousSelectionLevel = selectionLevel;

				if( isNewStart )
					previousConditionSelectionItem = conditionSelectionItem;
				}
			while( ( conditionSelectionItem = conditionSelectionItem.nextConditionItem( executionLevel, conditionSentenceNr ) ) != null );

			if( previousSelectionLevel != executionLevel )
				return adminItem_.startBoolResultError( 1, moduleNameString_, "I couldn't reach the given execution level " + executionLevel + ". The highest reached level was " + handledSelectionLevel );

			// All branches on same level are done and there are branchs on a higher level unhandled,
			// so start again with the handled branches (by clearing their checks) until all paths are handled
			if( unhandledSelectionLevel < Constants.MAX_LEVEL &&
			handledSelectionLevel < unhandledSelectionLevel )
				adminItem_.clearConditionChecksForSolving( unhandledSelectionLevel, conditionSentenceNr );

			if( isAllowingDuplicates &&
			isInitializeScores &&
			GlobalVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL )
				{
				hasFoundScore = false;

				if( ( oldGlobalSatisfiedScore_ > Constants.NO_SCORE ||
				newGlobalSatisfiedScore_ > Constants.NO_SCORE ||
				oldGlobalDissatisfiedScore_ > Constants.NO_SCORE ||
				newGlobalDissatisfiedScore_ > Constants.NO_SCORE ||
				oldGlobalNotBlockingScore_ > Constants.NO_SCORE ||
				newGlobalNotBlockingScore_ > Constants.NO_SCORE ||
				oldGlobalBlockingScore_ > Constants.NO_SCORE ||
				newGlobalBlockingScore_ > Constants.NO_SCORE ) &&

				adminItem_.hasScoreList() )
					{
					if( ( boolCheckResult = adminItem_.checkScores( isInverted, solveStrategyParameter, oldGlobalSatisfiedScore_, newGlobalSatisfiedScore_, oldGlobalDissatisfiedScore_, newGlobalDissatisfiedScore_, oldGlobalNotBlockingScore_, newGlobalNotBlockingScore_, oldGlobalBlockingScore_, newGlobalBlockingScore_ ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to check the scores" );

					hasFoundScore = boolCheckResult.booleanValue;
					}

				if( !hasFoundScore &&
				adminItem_.createScoreItem( true, oldGlobalSatisfiedScore_, newGlobalSatisfiedScore_, oldGlobalDissatisfiedScore_, newGlobalDissatisfiedScore_, oldGlobalNotBlockingScore_, newGlobalNotBlockingScore_, oldGlobalBlockingScore_, newGlobalBlockingScore_, null ) != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create an empty solve score" );
				}
			}
		while( unhandledSelectionLevel < Constants.MAX_LEVEL );

		return boolReturnResult;
		}

	private BoolResultType canWordBeSolved( WordItem solveWordItem )
		{
		BoolResultType boolResult;

		if( ( boolResult = canWordBeSolved( true, solveWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find out if a word can be solved by an action" );

		// Word can't be solved
		if( !boolResult.booleanValue &&
		!GlobalVariables.hasDisplayedWarning &&
		( boolResult = canWordBeSolved( false, solveWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find out if a word can be solved by an alternative action" );

		return boolResult;
		}

	private BoolResultType canWordBeSolved( boolean isAction, WordItem solveWordItem )
		{
		boolean isWordCanBeSolved = false;
		SelectionItem currentExecutionSelectionItem;
		WordItem specificationWordItem;
		BoolResultType boolResult = new BoolResultType();

		if( solveWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given solve word item is undefined" );

		if( ( currentExecutionSelectionItem = adminItem_.firstSelectionItem( isAction, solveWordItem ) ) != null )
			{
			do	{
				if( ( specificationWordItem = currentExecutionSelectionItem.specificationWordItem() ) == null )
					return adminItem_.startBoolResultError( 1, moduleNameString_, "The specification word of the current execution selection item is undefined" );

				if( specificationWordItem.firstNonQuestionActiveAssignmentItem() == null )
					{
					if( !currentExecutionSelectionItem.isNegative() &&
					!currentExecutionSelectionItem.isValueSpecification() )
						// Word can be solved
						isWordCanBeSolved = true;
					}
				else
					// Word has active assignments
					// Word can be solved
					isWordCanBeSolved = true;
				}
			// While word can't be solved
			while( !isWordCanBeSolved &&
			( currentExecutionSelectionItem = currentExecutionSelectionItem.nextSelectionItem( solveWordItem ) ) != null );
			}

		boolResult.booleanValue = isWordCanBeSolved;
		return boolResult;
		}

	private BoolResultType checkComparison( boolean isFirstComparisonPart, boolean isNegative, boolean isNumeralRelation, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem, String specificationString )
		{
		int comparisonResult = 0;
		int firstNumeral = 0;
		int secondNumeral = 0;
		String firstString = null;
		String secondString = null;
		SpecificationItem comparisonAssignmentItem;
		WordItem comparisonAssignmentSpecificationWordItem = null;
		WordItem firstSpecificationWordItem;
		WordItem secondSpecificationWordItem;
		BoolResultType boolResult = new BoolResultType();
		SpecificationResultType specificationResult;

		if( generalizationWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		// First part
		if( isFirstComparisonPart )
			{
			if( ( specificationResult = getComparisonAssignment( false, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed get the first comparison assignment" );

			// Allow the second part of the comparison to be checked
			boolResult.booleanValue = true;

			firstComparisonAssignmentItem_ = specificationResult.specificationItem;
			secondComparisonAssignmentItem_ = null;
			}
		else
			{
			if( !isNumeralRelation &&
			// Second part
			specificationString == null )
				{
				if( ( specificationResult = getComparisonAssignment( false, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed get a comparison assignment" );

				secondComparisonAssignmentItem_ = specificationResult.specificationItem;

				firstSpecificationWordItem = ( firstComparisonAssignmentItem_ != null ?
												firstComparisonAssignmentItem_.specificationWordItem() : null );
				secondSpecificationWordItem = ( secondComparisonAssignmentItem_ != null ?
												secondComparisonAssignmentItem_.specificationWordItem() : null );

				firstString = ( firstSpecificationWordItem != null ?
								firstSpecificationWordItem.anyWordTypeString() : null );
				secondString = ( secondSpecificationWordItem != null ?
								secondSpecificationWordItem.anyWordTypeString() : null );
				}
			else
				{
				// Numeral or specification string
				if( ( specificationResult = getComparisonAssignment( isNumeralRelation, specificationWordItem, relationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed get the first comparison assignment" );

				if( ( comparisonAssignmentItem = specificationResult.specificationItem ) != null )
					comparisonAssignmentSpecificationWordItem = comparisonAssignmentItem.specificationWordItem();

				firstString = ( comparisonAssignmentItem == null ? null :
								( isNumeralRelation ?
								( comparisonAssignmentSpecificationWordItem == null ? null : comparisonAssignmentSpecificationWordItem.anyWordTypeString() ) : comparisonAssignmentItem.specificationString() ) );
				secondString = ( isNumeralRelation ? ( relationWordItem == null ? null : relationWordItem.anyWordTypeString() ) : specificationString );
				}

			if( firstString == null ||
			secondString == null )
				comparisonResult = ( firstString == null && secondString == null ? 0 : ( firstString == null ? -1 : 1 ) );
			else
				{
				if( isNumeralString( firstString ) &&
				isNumeralString( secondString ) )
					{
					firstNumeral = Integer.parseInt( firstString );
					secondNumeral = Integer.parseInt( secondString );

					comparisonResult = ( firstNumeral == secondNumeral ? 0 : ( firstNumeral < secondNumeral ? -1 : 1 ) );
					}
				else
					comparisonResult = firstString.compareTo( secondString );
				}

			if( generalizationWordItem.isAdjectiveComparisonLess() )
				boolResult.booleanValue = ( comparisonResult < 0 ? !isNegative : isNegative );
			else
				{
				if( generalizationWordItem.isAdjectiveComparisonEqual() )
					boolResult.booleanValue = ( comparisonResult == 0 ? !isNegative : isNegative );
				else
					{
					if( !generalizationWordItem.isAdjectiveComparisonMore() )
						return adminItem_.startBoolResultError( 1, moduleNameString_, "Word \"" + generalizationWordItem.anyWordTypeString() + "\" isn't comparison word" );

					boolResult.booleanValue = ( comparisonResult > 0 ? !isNegative : isNegative );
					}
				}
			}

		return boolResult;
		}

	private BoolResultType checkOddOrEvenComparison( boolean isNegative, WordItem generalizationWordItem, WordItem specificationWordItem, WordItem relationWordItem )
		{
		int nAssignments;
		BoolResultType boolResult = new BoolResultType();

		if( generalizationWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given relation word item is undefined" );

		if( specificationWordItem.isNounNumber() )
			{
			nAssignments = relationWordItem.nActiveAssignments();

			if( generalizationWordItem.isAdjectiveOdd() )
				boolResult.booleanValue = ( nAssignments % 2 == 1 ? !isNegative : isNegative );
			else
				{
				if( !generalizationWordItem.isAdjectiveEven() )
					return adminItem_.startBoolResultError( 1, moduleNameString_, "Word \"" + generalizationWordItem.anyWordTypeString() + "\" isn't about odd or even" );

				boolResult.booleanValue = ( nAssignments % 2 == 0 ? !isNegative : isNegative );
				}
			}
		else
			{
			if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
				return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to write an interface warning" );
			}

		return boolResult;
		}

	private BoolResultType findSelectionToSolveWord( boolean isInitializeScores, boolean isAllowingDuplicates, boolean isInverted, boolean isPreparingSort, short solveStrategyParameter, WordItem solveWordItem )
		{
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult = new BoolResultType();

		if( solveWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given solve word is undefined" );

		if( solveWordItem.isWordCheckedForSolving )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given solve word \"" + solveWordItem.anyWordTypeString() + "\" is already checked" );

		solveWordItem.isWordCheckedForSolving = true;

		// Selection available
		if( GlobalVariables.adminActionList != null )
			{
			// Check selection actions
			if( ( boolCheckResult = findSelectionToSolveWord( true, isInitializeScores, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, solveWordItem ) ).result != Constants.RESULT_OK )
				return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a selection action to solve condition word \"" + solveWordItem.anyWordTypeString() + "\"" );

			if( boolCheckResult.booleanValue )
				boolReturnResult.booleanValue = true;

			if( !GlobalVariables.hasDisplayedWarning &&
			GlobalVariables.adminAlternativeList != null )
				{
				// Check selection alternatives
				if( ( boolCheckResult = findSelectionToSolveWord( false, isInitializeScores, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, solveWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a selection alternative to solve condition word \"" + solveWordItem.anyWordTypeString() + "\"" );

				if( boolCheckResult.booleanValue )
					boolReturnResult.booleanValue = true;
				}
			}

		solveWordItem.isWordCheckedForSolving = false;

		return boolReturnResult;
		}

	private BoolResultType findSelectionToSolveWord( boolean isAction, boolean isInitializeScores, boolean isAllowingDuplicates, boolean isInverted, boolean isPreparingSort, short solveStrategyParameter, WordItem solveWordItem )
		{
		SelectionItem currentExecutionSelectionItem;
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult = new BoolResultType();

		if( solveWordItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given solve word is undefined" );

		if( ( boolCheckResult = canWordBeSolved( isAction, solveWordItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find out if the given word \"" + solveWordItem.anyWordTypeString() + "\" can be solved by action" );

		// Word can be solved
		if( boolCheckResult.booleanValue &&
		( currentExecutionSelectionItem = adminItem_.firstSelectionItem( isAction, solveWordItem ) ) != null )
			{
			do	{
				if( ( boolCheckResult = backTrackSelectionConditions( isInitializeScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem.selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem.activeSentenceNr() ) ).result != Constants.RESULT_OK )
					return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to back-track the condition of the selection with sentence number " + currentExecutionSelectionItem.activeSentenceNr() );

				if( boolCheckResult.booleanValue )
					boolReturnResult.booleanValue = true;
				}
			while( ( currentExecutionSelectionItem = currentExecutionSelectionItem.nextSelectionItem( solveWordItem ) ) != null );
			}

		return boolReturnResult;
		}

	private BoolResultType scoreSelectionCondition( boolean isInverted, boolean isAllowingDuplicates, boolean isPreparingSort, short solveStrategyParameter, SelectionItem conditionSelectionItem )
		{
		boolean hasFoundScore = false;
		boolean isAddingScoresOfConditionPart = true;
		WordItem generalizationWordItem;
		WordItem specificationWordItem;
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult = new BoolResultType();
		ConditionResultType conditionPartResult;

		if( conditionSelectionItem == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The given condition selection item is undefined" );

		if( ( generalizationWordItem = conditionSelectionItem.generalizationWordItem() ) == null )
			return adminItem_.startBoolResultError( 1, moduleNameString_, "The generalization word of the given condition selection item is undefined" );

		if( ( conditionPartResult = checkSelectionCondition( conditionSelectionItem ) ).result != Constants.RESULT_OK )
			return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to check the condition of a selection in word word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

		if( !conditionPartResult.isConditionPartSatisfied &&
		!generalizationWordItem.isWordCheckedForSolving )
			{
			if( conditionSelectionItem.isAssignedOrEmpty() )
				{
				if( !conditionSelectionItem.isNegative() &&
				// Word has no active assignments
				generalizationWordItem.firstNonQuestionActiveAssignmentItem() == null )
					{
					if( ( boolReturnResult = findSelectionToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a selection to solve condition word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

					// Has found possibility
					if( boolReturnResult.booleanValue )
						isAddingScoresOfConditionPart = false;
					}
				}
			else
				{
				if( ( specificationWordItem = conditionSelectionItem.specificationWordItem() ) == null )
					return adminItem_.startBoolResultError( 1, moduleNameString_, "The specification word of the given score item is undefined" );

				if( !conditionSelectionItem.isNegative() &&
				!specificationWordItem.isWordCheckedForSolving &&
				// Word has no active assignments
				generalizationWordItem.firstNonQuestionActiveAssignmentItem() == null )
					{
					if( ( boolCheckResult = canWordBeSolved( specificationWordItem ) ).result != Constants.RESULT_OK )
						return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find out if specification word \"" + specificationWordItem.anyWordTypeString() + "\" can be solved" );

					// Word can be solved
					if( boolCheckResult.booleanValue )
						{
						if( ( boolReturnResult = findSelectionToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a selection to solve condition word \"" + generalizationWordItem.anyWordTypeString() + "\"" );

						// Has found possibility
						if( boolReturnResult.booleanValue )
							isAddingScoresOfConditionPart = false;
						}
					else
						{
						if( !isAllowingDuplicates &&
						adminItem_.hasScoreList() )
							{
							if( ( boolCheckResult = adminItem_.findScore( isPreparingSort, conditionSelectionItem ) ).result != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to find a score item" );

							hasFoundScore = boolCheckResult.booleanValue;
							}

						if( !hasFoundScore )
							{
							if( adminItem_.createScoreItem( ( GlobalVariables.currentAssignmentLevel > Constants.NO_ASSIGNMENT_LEVEL ), Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, Constants.NO_SCORE, conditionSelectionItem ) != Constants.RESULT_OK )
								return adminItem_.addBoolResultError( 1, moduleNameString_, "I failed to create an empty solve score" );

							// Has created score
							boolReturnResult.booleanValue = true;
							}
						}
					}
				}
			}

		if( isAddingScoresOfConditionPart &&
		!GlobalVariables.hasDisplayedWarning )
			{
			oldGlobalSatisfiedScore_ += conditionPartResult.oldSatisfiedScore;
			newGlobalSatisfiedScore_ += conditionPartResult.newSatisfiedScore;
			oldGlobalDissatisfiedScore_ += conditionPartResult.oldDissatisfiedScore;
			newGlobalDissatisfiedScore_ += conditionPartResult.newDissatisfiedScore;
			oldGlobalNotBlockingScore_ += conditionPartResult.oldNotBlockingScore;
			newGlobalNotBlockingScore_ += conditionPartResult.newNotBlockingScore;
			oldGlobalBlockingScore_ += conditionPartResult.oldBlockingScore;
			newGlobalBlockingScore_ += conditionPartResult.newBlockingScore;
			}

		return boolReturnResult;
		}

	private ConditionResultType checkSelectionCondition( SelectionItem conditionSelectionItem )
		{
		boolean isNegative;
		WordItem generalizationWordItem;
		WordItem specificationWordItem;
		BoolResultType boolResult;
		ConditionResultType conditionPartResult;
		ConditionResultType conditionReturnResult = new ConditionResultType();

		if( conditionSelectionItem == null )
			return adminItem_.startConditionResultError( 1, moduleNameString_, "The given condition selection item is undefined" );

		if( ( generalizationWordItem = conditionSelectionItem.generalizationWordItem() ) == null )
			return adminItem_.startConditionResultError( 1, moduleNameString_, "The generalization word of the given condition selection item is undefined" );

		if( ( specificationWordItem = conditionSelectionItem.specificationWordItem() ) == null )
			return adminItem_.startConditionResultError( 1, moduleNameString_, "The specification word of the given condition selection item is undefined" );

		isNegative = conditionSelectionItem.isNegative();

		if( conditionSelectionItem.isAssignedOrEmpty() )
			{
			if( specificationWordItem.isAdjectiveAssigned() )
				{
				// Adjective 'assigned'
				if( ( conditionPartResult = findBlockingScores( isNegative, generalizationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addConditionResultError( 1, moduleNameString_, "I failed to find satisfying scores" );

				conditionReturnResult.isConditionPartSatisfied = ( conditionPartResult.isConditionPartSatisfied ? isNegative : !isNegative );
				}
			else
				{
				// Adjective 'empty'
				if( ( conditionPartResult = findBlockingScores( !isNegative, generalizationWordItem ) ).result != Constants.RESULT_OK )
					return adminItem_.addConditionResultError( 1, moduleNameString_, "I failed to find satisfying scores" );

				conditionReturnResult.isConditionPartSatisfied = ( conditionPartResult.isConditionPartSatisfied ? !isNegative : isNegative );
				}

			conditionReturnResult.oldBlockingScore += conditionPartResult.oldBlockingScore;
			conditionReturnResult.newBlockingScore += conditionPartResult.newBlockingScore;
			conditionReturnResult.oldNotBlockingScore += conditionPartResult.oldNotBlockingScore;
			conditionReturnResult.newNotBlockingScore += conditionPartResult.newNotBlockingScore;
			}
		else
			{
			if( generalizationWordItem.isAdjectiveComparison() )
				{
				if( ( boolResult = checkComparison( conditionSelectionItem.isFirstComparisonPart(), isNegative, conditionSelectionItem.isNumeralRelation(), generalizationWordItem, specificationWordItem, conditionSelectionItem.relationWordItem(), conditionSelectionItem.specificationString() ) ).result != Constants.RESULT_OK )
					return adminItem_.addConditionResultError( 1, moduleNameString_, "I failed to check a comparison" );

				conditionReturnResult.isConditionPartSatisfied = boolResult.booleanValue;
				}
			else
				{
				if( generalizationWordItem.isAdjectiveOddOrEven() )
					{
					if( ( boolResult = checkOddOrEvenComparison( isNegative, generalizationWordItem, specificationWordItem, conditionSelectionItem.relationWordItem() ) ).result != Constants.RESULT_OK )
						return adminItem_.addConditionResultError( 1, moduleNameString_, "I failed to check for odd or even" );

					conditionReturnResult.isConditionPartSatisfied = boolResult.booleanValue;
					}
				else
					{
					if( conditionSelectionItem.isValueSpecification() )
						{
						if( ( conditionPartResult = checkValueConditionPart( isNegative, conditionSelectionItem.isPossessive(), generalizationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addConditionResultError( 1, moduleNameString_, "I failed to check the condition of a specification by value" );

						conditionReturnResult.isConditionPartSatisfied = conditionPartResult.isConditionPartSatisfied;
						}
					else
						{
						if( ( conditionPartResult = findSatisfyingScores( !isNegative, generalizationWordItem, specificationWordItem ) ).result != Constants.RESULT_OK )
							return adminItem_.addConditionResultError( 1, moduleNameString_, "I failed to find satisfying scores" );

						// Scoring assignment is satifying
						if( conditionPartResult.isConditionPartSatisfied != isNegative )
							conditionReturnResult.isConditionPartSatisfied = true;
						}

					conditionReturnResult.oldSatisfiedScore += conditionPartResult.oldSatisfiedScore;
					conditionReturnResult.newSatisfiedScore += conditionPartResult.newSatisfiedScore;
					conditionReturnResult.oldDissatisfiedScore += conditionPartResult.oldDissatisfiedScore;
					conditionReturnResult.newDissatisfiedScore += conditionPartResult.newDissatisfiedScore;
					}
				}
			}

		return conditionReturnResult;
		}

	private ConditionResultType checkValueConditionPart( boolean isNegative, boolean isPossessive, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean isSatisfiedScore;
		SpecificationItem currentSpecificationItem;
		SpecificationItem foundAssignmentItem;
		ConditionResultType conditionPartResult;
		ConditionResultType conditionReturnResult = new ConditionResultType();

		conditionReturnResult.isConditionPartSatisfied = true;

		if( specificationWordItem == null )
			return adminItem_.startConditionResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem.firstExclusiveSpecificationItem() ) != null )
			{
			do	{
				foundAssignmentItem = specificationWordItem.firstNonQuestionAssignmentItem( true, false, false, false, isPossessive, currentSpecificationItem.specificationWordItem() );
				isSatisfiedScore = ( isNegative == ( foundAssignmentItem == null || foundAssignmentItem.isNegative() ) );

				if( ( conditionPartResult = findSatisfyingScores( isSatisfiedScore, generalizationWordItem, currentSpecificationItem.specificationWordItem() ) ).result != Constants.RESULT_OK )
					return adminItem_.addConditionResultError( 1, moduleNameString_, "I failed to find satisfying scores" );

				// Scoring assignment is unsatisfying
				if( conditionPartResult.isConditionPartSatisfied != isSatisfiedScore )
					conditionReturnResult.isConditionPartSatisfied = false;

				conditionReturnResult.oldSatisfiedScore += conditionPartResult.oldSatisfiedScore;
				conditionReturnResult.newSatisfiedScore += conditionPartResult.newSatisfiedScore;
				conditionReturnResult.oldDissatisfiedScore += conditionPartResult.oldDissatisfiedScore;
				conditionReturnResult.newDissatisfiedScore += conditionPartResult.newDissatisfiedScore;
				}
			while( ( currentSpecificationItem = currentSpecificationItem.nextExclusiveSpecificationItem() ) != null );
			}

		return conditionReturnResult;
		}

	private ConditionResultType findBlockingScores( boolean isBlocking, WordItem generalizationWordItem )
		{
		boolean hasFoundBlockingAssignment = false;
		SpecificationItem currentAssignmentItem;
		ConditionResultType conditionResult = new ConditionResultType();

		if( generalizationWordItem == null )
			return adminItem_.startConditionResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentAssignmentItem = generalizationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
			{
			do	{
				if( !currentAssignmentItem.isNegative() )
					{
					if( currentAssignmentItem.isOlderItem() )
						{
						if( isBlocking )
							conditionResult.oldBlockingScore++;
						else
							conditionResult.oldNotBlockingScore++;
						}
					else
						{
						if( isBlocking )
							conditionResult.newBlockingScore++;
						else
							conditionResult.newNotBlockingScore++;
						}

					hasFoundBlockingAssignment = true;
					}
				}
			while( !hasFoundBlockingAssignment &&
			( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
			}

		conditionResult.isConditionPartSatisfied = !hasFoundBlockingAssignment;
		return conditionResult;
		}

	private ConditionResultType findSatisfyingScores( boolean isSatisfiedScore, WordItem generalizationWordItem, WordItem specificationWordItem )
		{
		boolean hasFoundScoringAssignment = false;
		SpecificationItem currentAssignmentItem;
		ConditionResultType conditionResult = new ConditionResultType();

		if( generalizationWordItem == null )
			return adminItem_.startConditionResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == null )
			return adminItem_.startConditionResultError( 1, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentAssignmentItem = generalizationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
			{
			do	{
				if( currentAssignmentItem.isRelatedSpecification( false, specificationWordItem ) )
					{
					if( currentAssignmentItem.isOlderItem() )
						{
						if( isSatisfiedScore )
							conditionResult.oldSatisfiedScore++;
						else
							conditionResult.oldDissatisfiedScore++;
						}
					else
						{
						if( isSatisfiedScore )
							conditionResult.newSatisfiedScore++;
						else
							conditionResult.newDissatisfiedScore++;
						}

					hasFoundScoringAssignment = true;
					}
				}
			while( !hasFoundScoringAssignment &&
			( currentAssignmentItem = currentAssignmentItem.nextSelectedSpecificationItem() ) != null );
			}

		conditionResult.isConditionPartSatisfied = hasFoundScoringAssignment;
		return conditionResult;
		}

	private ShortResultType getAssignmentOrderNr( WordItem generalizationWordItem )
		{
		short assignmentOrderNr = 0;
		short specificationNr = 0;
		SpecificationItem currentSpecificationItem;
		SpecificationItem orderAssignmentItem;
		WordItem assignmentWordItem;
		ShortResultType shortResult = new ShortResultType();

		if( generalizationWordItem == null )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem.nActiveAssignments() > 1 )
			return adminItem_.startShortResultError( 1, moduleNameString_, "I have more than one assignment at assignment level " + GlobalVariables.currentAssignmentLevel );

		orderAssignmentItem = generalizationWordItem.firstNonQuestionActiveAssignmentItem();

		if( orderAssignmentItem != null &&
		( assignmentWordItem = orderAssignmentItem.specificationWordItem() ) != null &&
		( currentSpecificationItem = generalizationWordItem.firstExclusiveSpecificationItem() ) != null )
			{
			do	{
				specificationNr++;

				if( currentSpecificationItem.specificationWordItem() == assignmentWordItem )
					assignmentOrderNr = specificationNr;
				}
			while( assignmentOrderNr == Constants.NO_ORDER_NR &&
			( currentSpecificationItem = currentSpecificationItem.nextExclusiveSpecificationItem() ) != null );
			}

		shortResult.shortValue = assignmentOrderNr;
		return shortResult;
		}

	private ShortResultType getAssignmentWordParameter( WordItem generalizationWordItem )
		{
		SpecificationItem assignmentItem;
		WordItem specificationWordItem;
		ShortResultType shortResult = new ShortResultType();

		if( generalizationWordItem == null )
			return adminItem_.startShortResultError( 1, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem.nActiveAssignments() > 1 )
			return adminItem_.startShortResultError( 1, moduleNameString_, "I have more than one assignment at assignment level " + GlobalVariables.currentAssignmentLevel );

		if( ( assignmentItem = generalizationWordItem.firstNonQuestionActiveAssignmentItem() ) != null )
			{
			if( ( specificationWordItem = assignmentItem.specificationWordItem() ) == null )
				return adminItem_.startShortResultError( 1, moduleNameString_, "I found an undefined assignment word at assignment level " + GlobalVariables.currentAssignmentLevel );

			shortResult.shortValue = specificationWordItem.wordParameter();
			}

		return shortResult;
		}

	private SpecificationResultType getComparisonAssignment( boolean isNumeralRelation, WordItem specificationWordItem, WordItem relationWordItem )
		{
		SpecificationResultType specificationResult = new SpecificationResultType();

		if( specificationWordItem == null )
			return adminItem_.startSpecificationResultError( 1, moduleNameString_, null, "The given specification word item is undefined" );

		if( isNumeralRelation )
			specificationResult.specificationItem = specificationWordItem.firstActiveNumeralAssignmentItem();
		else
			{
			if( relationWordItem == null )
				specificationResult.specificationItem = specificationWordItem.firstActiveStringAssignmentItem();
			else
				{
				if( specificationWordItem.isNounHead() )
					specificationResult.specificationItem = relationWordItem.lastActiveNonQuestionAssignmentItem();
				else
					{
					if( specificationWordItem.isNounTail() )
						specificationResult.specificationItem = relationWordItem.firstNonQuestionActiveAssignmentItem();
					else
						{
						if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
							return adminItem_.addSpecificationResultError( 1, moduleNameString_, null, "I failed to write an interface warning" );
						}
					}
				}
			}

		return specificationResult;
		}


	// Constructor

	protected AdminImperative( AdminItem adminItem, WordItem predefinedAdjectiveBusyWordItem, WordItem predefinedAdjectiveDoneWordItem, WordItem predefinedAdjectiveInvertedWordItem, WordItem predefinedNounSolveLevelWordItem, WordItem predefinedNounSolveMethodWordItem, WordItem predefinedNounSolveStrategyWordItem )
		{
		String errorString = null;

		// Checking private initialized variables

		if( ( predefinedAdjectiveBusyWordItem_ = predefinedAdjectiveBusyWordItem ) == null )
			errorString = "The given predefined adjective Busy word item is undefined";

		if( ( predefinedAdjectiveDoneWordItem_ = predefinedAdjectiveDoneWordItem ) == null )
			errorString = "The given predefined adjective Done word item is undefined";

		if( ( predefinedAdjectiveInvertedWordItem_ = predefinedAdjectiveInvertedWordItem ) == null )
			errorString = "The given predefined adjective Inverted word item is undefined";

		if( ( predefinedNounSolveLevelWordItem_ = predefinedNounSolveLevelWordItem ) == null )
			errorString = "The given predefined noun Solve Level word item is undefined";

		if( ( predefinedNounSolveMethodWordItem_ = predefinedNounSolveMethodWordItem ) == null )
			errorString = "The given predefined noun Solve Method word item is undefined";

		if( ( predefinedNounSolveStrategyWordItem_ = predefinedNounSolveStrategyWordItem ) == null )
			errorString = "The given predefined noun Solve Strategy word item is undefined";

		if( ( adminItem_ = adminItem ) == null )
			{
			GlobalVariables.result = Constants.RESULT_SYSTEM_ERROR;
			Console.addError( "\nClass:" + moduleNameString_ + "\nMethod:\t" + Constants.INPUT_OUTPUT_ERROR_CONSTRUCTOR_METHOD_NAME + "\nError:\t\tThe given admin item is undefined.\n" );
			}
		else
			{
			if( errorString != null )
				adminItem_.startSystemError( 1, moduleNameString_, errorString );
			}
		}


	// Protected methods

	protected boolean hasRequestedRestart()
		{
		return hasRequestedRestart_;
		}

	protected byte executeImperative( boolean isInitializeVariables, short executionListNr, short executionNounWordParameter, short imperativeVerbParameter, short specificationWordTypeNr, int endSolveProgressLevel, String executionString, WordItem imperativeVerbWordItem, WordItem specificationWordItem, ReadItem startRelationReadItem, ReadItem endRelationReadItem, SelectionItem executionSelectionItem, SelectionItem actionSelectionItem )
		{
		boolean isDisplayingRelationWarning = false;
		int firstSentenceNrOfCurrentUser;
		ShortResultType shortResult;

		if( isInitializeVariables )
			virtualListAssignmentItem_ = null;

		if( imperativeVerbWordItem == null )
			return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "The given imperative verb word item is undefined" );

		switch( imperativeVerbParameter )
			{
			// Selection
			case Constants.NO_IMPERATIVE_PARAMETER:
				if( adminItem_.assignSelectionSpecification( executionSelectionItem ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to assign an imperative selection specification at assignment level " + GlobalVariables.currentAssignmentLevel );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				if( executionSelectionItem == null )
					{
					if( executeVirtualListImperative( imperativeVerbParameter, specificationWordTypeNr, imperativeVerbWordItem, specificationWordItem, startRelationReadItem, endRelationReadItem ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to execute a virtual list imperative" );
					}
				else
					{
					if( executeVirtualListImperativePart( imperativeVerbParameter, executionSelectionItem.prepositionParameter(), specificationWordTypeNr, executionSelectionItem.relationWordTypeNr(), imperativeVerbWordItem, specificationWordItem, executionSelectionItem.relationWordItem() ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to execute a selection virtual list imperative" );
					}

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR:
				if( startRelationReadItem == null )
					{
					switch( executionNounWordParameter )
						{
						case Constants.WORD_PARAMETER_NOUN_MIND:
							firstSentenceNrOfCurrentUser = adminItem_.firstSentenceNrOfCurrentUser();
							adminItem_.deleteSentences( firstSentenceNrOfCurrentUser );
							GlobalVariables.currentSentenceNr = firstSentenceNrOfCurrentUser;

							if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_NOTIFICATION, Constants.INTERFACE_IMPERATIVE_NOTIFICATION_MY_MIND_IS_CLEAR ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write the 'my mind is clear' interface notification about clearing my mind" );

							break;

						default:
							if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write an interface warning about clearing" );
						}
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
				if( startRelationReadItem == null )
					{
					if( adminItem_.readInfoFile( true, imperativeVerbWordItem.anyWordTypeString() ).result != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to read the info help file" );
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( startRelationReadItem == null )
					{
					if( adminItem_.login( specificationWordItem ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to login" );
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
				if( startRelationReadItem == null )
					{
					switch( executionNounWordParameter )
						{
						case Constants.WORD_PARAMETER_NOUN_FILE:
							if( adminItem_.readExampleFile( executionString ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to read an example file" );

							break;

						case Constants.WORD_PARAMETER_NOUN_TEST_FILE:
							if( executionString == null ||
							executionString.length() == 0 )
								return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "The given execution string is undefined or empty" );

							// Postpone result check. Close test file first
							adminItem_.readTestFile( executionString );

							// Stop redirecting to test results
							InputOutput.redirectOutputToTestFile( null );

							if( GlobalVariables.result == Constants.RESULT_OK &&
							!Console.isTestingCanceled() &&
							adminItem_.compareOutputFileAgainstReferenceFile( executionString ) != Constants.RESULT_OK )
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to compare the test file against the reference file" );

							// Continue redirecting to test results
							InputOutput.redirectOutputToTestFile( adminItem_.currentWriteFile() );

							// Now check the result
							if( GlobalVariables.result != Constants.RESULT_OK )
								{
								InputOutput.clearProgress();
								return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to read a test file" );
								}

							break;

						default:
							if( GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
								return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: " + executionNounWordParameter );

							return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: " + executionNounWordParameter + " at assignment level " + GlobalVariables.currentAssignmentLevel );
						}
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO:
				if( adminItem_.undoLastSentence() != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to undo the last sentence" );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO:
				if( adminItem_.redoLastUndoneSentence() != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to redo the last undone sentence" );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_RESTART:
				hasRequestedRestart_ = true;
				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY:
				if( executeImperativeDisplay( executionNounWordParameter, specificationWordTypeNr, imperativeVerbWordItem, specificationWordItem, executionString ) != Constants.RESULT_OK )
					return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to execute the display imperative" );

				break;

			case Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE:
				if( executionSelectionItem != null ||

				( executionListNr == Constants.NO_LIST_NR &&
				specificationWordItem != null &&
				specificationWordItem.wordParameter() != Constants.WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE ) )
					{
					adminItem_.deleteTemporaryScoreList();

					// Get solve level
					if( ( shortResult = getAssignmentOrderNr( predefinedNounSolveLevelWordItem_ ) ).result != Constants.RESULT_OK )
						return adminItem_.addError( 1, moduleNameString_, "I failed to get the solve level" );

					if( solveWord( shortResult.shortValue, 0, endSolveProgressLevel, specificationWordItem, actionSelectionItem ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to solve a word at assignment level " + GlobalVariables.currentAssignmentLevel );
					}
				else
					{
					if( InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem.anyWordTypeString(), Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != Constants.RESULT_OK )
						return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write an interface warning about solving" );
					}

				break;

			default:
				if( GlobalVariables.currentAssignmentLevel == Constants.NO_ASSIGNMENT_LEVEL )
					return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute the imperative with word parameter: " + imperativeVerbParameter );

				return adminItem_.startErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I don't know how to execute the imperative with word parameter: " + imperativeVerbParameter + ", at assignment level " + GlobalVariables.currentAssignmentLevel );
			}

		if( isDisplayingRelationWarning &&
		InputOutput.writeInterfaceText( false, Constants.INPUT_OUTPUT_PROMPT_WARNING, Constants.INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_TO_DO_WITH_RELATION ) != Constants.RESULT_OK )
			return adminItem_.addErrorWithAdminListNr( executionListNr, 1, moduleNameString_, "I failed to write an interface warning" );

		return Constants.RESULT_OK;
		}

	protected byte executeSelection( int endSolveProgressLevel, SelectionItem actionSelectionItem )
		{
		boolean hasDoneLastExecution;
		boolean isConditionSatisfied;
		boolean isInitializeVariables;
		boolean isNewStart;
		boolean isWaitingForNewStart;
		boolean isWaitingForNewLevel;
		boolean isWaitingForExecution;
		short executionLevel;
		short nSelectionInterations = 0;
		short selectionLevel;
		int executionSentenceNr;
		SelectionItem conditionSelectionItem;
		SelectionItem executionSelectionItem;
		ConditionResultType conditionResult;

		do	{
			hasDoneLastExecution = false;
			GlobalVariables.isAssignmentChanged = false;

			if( ( conditionSelectionItem = adminItem_.firstConditionItem() ) != null )
				{
				isConditionSatisfied = false;
				isWaitingForNewStart = false;
				isWaitingForNewLevel = false;
				isWaitingForExecution = false;
				executionLevel = conditionSelectionItem.selectionLevel();
				executionSentenceNr = conditionSelectionItem.activeSentenceNr();

				do	{
					if( conditionSelectionItem == null ||
					executionSentenceNr != conditionSelectionItem.activeSentenceNr() )
						{
						if( ( executionSelectionItem = adminItem_.executionStartEntry( isConditionSatisfied, executionLevel, executionSentenceNr ) ) != null )
							{
							isInitializeVariables = true;

							do	{
								if( executeImperative( isInitializeVariables, ( isConditionSatisfied ? Constants.ADMIN_ACTION_LIST : Constants.ADMIN_ALTERNATIVE_LIST ), Constants.NO_WORD_PARAMETER, executionSelectionItem.imperativeVerbParameter(), executionSelectionItem.specificationWordTypeNr(), endSolveProgressLevel, executionSelectionItem.specificationString(), executionSelectionItem.generalizationWordItem(), executionSelectionItem.specificationWordItem(), null, null, executionSelectionItem, actionSelectionItem ) != Constants.RESULT_OK )
									return adminItem_.addError( 1, moduleNameString_, "I failed to execute an imperative" );

								isInitializeVariables = false;
								}
							while( !hasRequestedRestart_ &&
							!GlobalVariables.hasDisplayedWarning &&
							( executionSelectionItem = executionSelectionItem.nextExecutionItem( executionLevel, executionSentenceNr ) ) != null );
							}

						// Found new condition
						if( conditionSelectionItem == null )
							hasDoneLastExecution = true;
						else
							{
							isConditionSatisfied = false;
							isWaitingForNewStart = false;
							isWaitingForNewLevel = false;
							isWaitingForExecution = false;
							executionLevel = conditionSelectionItem.selectionLevel();
							}
						}

					if( conditionSelectionItem != null )
						{
						isNewStart = conditionSelectionItem.isNewStart();
						selectionLevel = conditionSelectionItem.selectionLevel();

						if( isNewStart &&
						isWaitingForNewStart &&
						// Found new start
						executionLevel == selectionLevel )
							isWaitingForNewStart = false;

						if( isWaitingForNewLevel &&
						// Found new level
						executionLevel != selectionLevel )
							isWaitingForNewLevel = false;

						if( !isWaitingForNewStart &&
						!isWaitingForNewLevel &&
						!isWaitingForExecution )
							{
							if( !isNewStart &&
							!isConditionSatisfied )
								// Skip checking of this condition part and wait for a new start to come on this level
								isWaitingForNewStart = true;
							else
								{
								if( isNewStart &&
								isConditionSatisfied &&
								executionLevel == selectionLevel )
									// Skip checking of this condition part and wait for a new level to come
									isWaitingForNewLevel = true;
								else
									{
									if( executionLevel != selectionLevel &&
									isConditionSatisfied != conditionSelectionItem.isAction() )
										// Skip checking of this condition and wait for the next condition sentence number to come
										isWaitingForExecution = true;
									else
										{
										if( ( conditionResult = checkSelectionCondition( conditionSelectionItem ) ).result != Constants.RESULT_OK )
											return adminItem_.addError( 1, moduleNameString_, "I failed to check the condition of a selection" );

										isConditionSatisfied = conditionResult.isConditionPartSatisfied;
										executionLevel = selectionLevel;
										executionSentenceNr = conditionSelectionItem.activeSentenceNr();
										}
									}
								}
							}

						conditionSelectionItem = conditionSelectionItem.nextSelectionItem();
						}
					}
				while( !hasDoneLastExecution &&
				!hasRequestedRestart_ &&
				!GlobalVariables.hasDisplayedWarning );
				}
			}
		while( !hasRequestedRestart_ &&
		!GlobalVariables.hasDisplayedWarning &&
		GlobalVariables.isAssignmentChanged &&
		++nSelectionInterations < Constants.MAX_SELECTION_ITERATIONS );

		if( GlobalVariables.isAssignmentChanged &&
		nSelectionInterations == Constants.MAX_SELECTION_ITERATIONS )
			return adminItem_.startError( 1, moduleNameString_, "There is probably an endless loop in the selections" );

		return Constants.RESULT_OK;
		}
	};

/*************************************************************************
 *	"He has given me a new song to sing,
 *	a hymn of praise to our God.
 *	Many will see what he has done and be amazed.
 *	They will put their trust in the Lord." (Psalm 40:3)
 *************************************************************************/
