/*	Class:			AdminImperative
 *	Supports class:	AdminItem
 *	Purpose:		To execute imperative words
 *	Version:		Thinknowlogy 2017r2 (Science as it should be)
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

#include "AdminItem.h"
#include "ConditionResultType.cpp"
#include "FileResultType.cpp"
#include "ReadItem.cpp"
#include "ScoreList.cpp"

class AdminImperative
	{
	friend class AdminItem;

	// Private constructed variables

	bool hasRequestedRestart_;

	unsigned int oldGlobalSatisfiedScore_;
	unsigned int newGlobalSatisfiedScore_;
	unsigned int oldGlobalDissatisfiedScore_;
	unsigned int newGlobalDissatisfiedScore_;
	unsigned int oldGlobalNotBlockingScore_;
	unsigned int newGlobalNotBlockingScore_;
	unsigned int oldGlobalBlockingScore_;
	unsigned int newGlobalBlockingScore_;

	SpecificationItem *firstComparisonAssignmentItem_;
	SpecificationItem *secondComparisonAssignmentItem_;
	SpecificationItem *virtualListAssignmentItem_;

	char closedTestFileNameString_[MAX_SENTENCE_STRING_LENGTH];


	// Private initialized variables

	WordItem *predefinedAdjectiveBusyWordItem_;
	WordItem *predefinedAdjectiveDoneWordItem_;
	WordItem *predefinedAdjectiveInvertedWordItem_;
	WordItem *predefinedNounSolveLevelWordItem_;
	WordItem *predefinedNounSolveMethodWordItem_;
	WordItem *predefinedNounSolveStrategyWordItem_;

	char moduleNameString_[FUNCTION_NAME_LENGTH];

	AdminItem *adminItem_;
	CommonVariables *commonVariables_;
	InputOutput *inputOutput_;


	// Private functions

	bool isNumeralString( char *checkString )
		{
		size_t stringLength;
		size_t position = 0;

		if( checkString != NULL &&
		( stringLength = strlen( checkString ) ) > 0 )
			{
			while( position < stringLength &&
			isdigit( checkString[position] ) )
				position++;

			return ( position == stringLength );
			}

		return false;
		}

	signed char addWordToVirtualList( bool isSelection, unsigned short generalizationWordTypeNr, unsigned short specificationWordTypeNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "addWordToVirtualList";

		if( generalizationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem->addSpecificationInWord( true, false, false, false, false, false, false, false, false, false, false, isSelection, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, generalizationWordTypeNr, specificationWordTypeNr, NO_WORD_TYPE_NR, NO_COLLECTION_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, 0, NULL, specificationWordItem, NULL, NULL, NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a virtual list specification" );

		if( generalizationWordItem->assignSpecification( false, false, false, false, false, false, false, false, false, NO_ASSUMPTION_LEVEL, NO_PREPOSITION_PARAMETER, NO_QUESTION_PARAMETER, NO_WORD_TYPE_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_CONTEXT_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, NO_SENTENCE_NR, 0, NULL, specificationWordItem, NULL, NULL ).result != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a virtual list word" );

		return RESULT_OK;
		}

	signed char createNewAssignmentLevelInAssignmentWords()
		{
		WordItem *currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = commonVariables_->firstAssignmentWordItem ) != NULL )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem->createNewAssignmentLevelInWord() == RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem->nextAssignmentWordItem ) != NULL );
			}

		return commonVariables_->result;
		}

	signed char deleteAssignmentLevelInAssignmentWords()
		{
		WordItem *currentAssignmentWordItem;

		if( ( currentAssignmentWordItem = commonVariables_->firstAssignmentWordItem ) != NULL )
			{
			// Do for all assignment words
			do	;
			while( currentAssignmentWordItem->deleteAssignmentLevelInWord() == RESULT_OK &&
			( currentAssignmentWordItem = currentAssignmentWordItem->nextAssignmentWordItem ) != NULL );
			}

		return commonVariables_->result;
		}

	signed char executeImperativeDisplay( unsigned short executionNounWordParameter, unsigned short executionWordTypeNr, WordItem *imperativeVerbWordItem, WordItem *executionWordItem, char *executionString )
		{
		char *singularNounString;
		char *writtenSentenceString;
		FileResultType fileResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperativeDisplay";

		if( imperativeVerbWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given imperative verb word item is undefined" );

		if( executionString != NULL ||
		executionWordItem != NULL )
			{
			if( executionString != NULL &&
			executionWordTypeNr == WORD_TYPE_TEXT )
				{
				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL &&
				writeInfoTextWithPossibleQueryCommands( executionString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the execution string" );
				}
			else
				{
				adminItem_->initializeAdminWriteVariables();

				switch( executionNounWordParameter )
					{
					case WORD_PARAMETER_NOUN_JUSTIFICATION_REPORT:
						if( adminItem_->writeJustificationReport( executionWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write justification for a word" );

						break;

					default:
						// Try to read the info file with the specification name
						if( executionWordItem == NULL )
							return adminItem_->startError( functionNameString, moduleNameString_, "The specification word item is undefined" );

						if( ( fileResult = adminItem_->readInfoFile( false, executionWordItem->activeWordTypeString( executionWordTypeNr ) ) ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read the info file" );

						if( fileResult.createdFileItem == NULL &&
						executionWordTypeNr == WORD_TYPE_NOUN_PLURAL &&
						( singularNounString = executionWordItem->singularNounString() ) != NULL &&
						adminItem_->readInfoFile( false, singularNounString ).result != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to read the info file with a singular noun word" );

						// Try to display all knowledge about this specification
						if( adminItem_->writeInfoAboutWord( false, true, true, true, true, true, true, true, executionWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write info about a word" );
					}

				if( !commonVariables_->hasDisplayedMessage &&
				executionWordItem != NULL )
					{
					if( adminItem_->isImperativeSentence() &&
					( writtenSentenceString = commonVariables_->writtenSentenceString ) != NULL &&
					strlen( writtenSentenceString ) > 0 )
						{
						if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_I_HAVE_NO ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the 'I have no' interface notification" );

						if( inputOutput_->writeDiacriticalText( false, false, INPUT_OUTPUT_PROMPT_NOTIFICATION, writtenSentenceString ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a sentence" );
						}
					else
						{
						if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_HAVE_NO_INFO_ABOUT_START, executionWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_HAVE_NO_INFO_ABOUT_END ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
						}
					}
				}
			}
		else
			{
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char executeVirtualListImperative( unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, WordItem *imperativeVerbWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		bool hasFoundHeadOrTail = false;
		bool hasFoundVirtualListAction = false;
		SpecificationItem *localVirtualListAssignmentItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeVirtualListImperative";

		if( imperativeVerbWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given imperative verb word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		switch( imperativeVerbParameter )
			{
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
				if( !specificationWordItem->isNounHead() &&
				!specificationWordItem->isNounTail() )
					{
					if( addWordToVirtualList( false, relationWordTypeNr, specificationWordTypeNr, relationWordItem, specificationWordItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a word to a virtual list" );
					}
				else
					{
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
				switch( prepositionParameter )
					{
					// Head or tail
					case WORD_PARAMETER_PREPOSITION_OF:
						if( specificationWordItem->isNounHead() )
							{
							hasFoundHeadOrTail = true;

							if( ( virtualListAssignmentItem_ = relationWordItem->lastActiveNonQuestionAssignmentItem() ) == NULL )
								hasFoundVirtualListAction = true;
							else
								{
								if( relationWordItem->inactivateActiveAssignment( virtualListAssignmentItem_ ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the head of a virtual list" );
								}
							}
						else
							{
							if( specificationWordItem->isNounTail() )
								{
								hasFoundHeadOrTail = true;

								if( ( virtualListAssignmentItem_ = relationWordItem->firstNonQuestionActiveAssignmentItem() ) == NULL )
									hasFoundVirtualListAction = true;
								else
									{
									if( relationWordItem->inactivateActiveAssignment( virtualListAssignmentItem_ ) != RESULT_OK )
										return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the tail of a virtual list" );
									}
								}
							}

					// Don't insert a break statement here

					case WORD_PARAMETER_PREPOSITION_FROM:
						if( !hasFoundHeadOrTail )
							{
							if( ( virtualListAssignmentItem_ = relationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, specificationWordItem ) ) == NULL )
								hasFoundVirtualListAction = true;
							else
								{
								if( relationWordItem->inactivateActiveAssignment( virtualListAssignmentItem_ ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the word in a virtual list" );
								}
							}

						break;

					case WORD_PARAMETER_PREPOSITION_TO:
						// Global virtual assignment list
						if( virtualListAssignmentItem_ == NULL )
							{
							if( !hasFoundVirtualListAction &&
							inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
							}
						else
							{
							if( addWordToVirtualList( false, virtualListAssignmentItem_->generalizationWordTypeNr(), virtualListAssignmentItem_->specificationWordTypeNr(), relationWordItem, virtualListAssignmentItem_->specificationWordItem() ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to add a word to a virtual list" );
							}

						break;

					default:
						if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_PREPOSITION_NOT_USED_FOR_THIS_ACTION ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the preposition parameter" );
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				localVirtualListAssignmentItem = ( specificationWordItem->isNounHead() ? relationWordItem->lastActiveNonQuestionAssignmentItem() : ( specificationWordItem->isNounTail() ? relationWordItem->firstNonQuestionActiveAssignmentItem() : relationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, specificationWordItem ) ) );

				if( localVirtualListAssignmentItem != NULL &&
				relationWordItem->inactivateActiveAssignment( localVirtualListAssignmentItem ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to dectivate the head of a virtual list" );

				break;

			default:
				return adminItem_->startError( functionNameString, moduleNameString_, "The given imperative parameter isn't a virtual list action" );
			}

		return RESULT_OK;
		}

	signed char executeVirtualListImperative( unsigned short imperativeVerbParameter, unsigned short specificationWordTypeNr, WordItem *imperativeVerbWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem )
		{
		unsigned short prepositionParameter = NO_PREPOSITION_PARAMETER;
		ReadItem *currentReadItem = startRelationWordReadItem;
		WordItem *relationWordItem;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeVirtualListImperative";

		if( imperativeVerbWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given imperative verb word item is undefined" );

		if( specificationWordItem != NULL )
			{
			if( startRelationWordReadItem != NULL )
				{
				do	{
					if( currentReadItem->isPreposition() )
						prepositionParameter = currentReadItem->wordParameter();
					else
						{
						if( currentReadItem->isRelationWord() &&
						( relationWordItem = currentReadItem->readWordItem() ) != NULL &&
						executeVirtualListImperative( imperativeVerbParameter, prepositionParameter, specificationWordTypeNr, currentReadItem->wordTypeNr(), imperativeVerbWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
						}
					}
				while( currentReadItem != endRelationReadItem &&
				( currentReadItem = currentReadItem->nextReadItem() ) != NULL );
				}
			else
				{
				if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_NEEDS_A_LIST_TO_BE_SPECIFIED ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
				}
			}
		else
			{
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning about the add, move or remove imperative" );
			}

		return RESULT_OK;
		}

	signed char executeVirtualListSelectionImperative( unsigned short imperativeVerbParameter, unsigned short prepositionParameter, unsigned short specificationWordTypeNr, unsigned short relationWordTypeNr, WordItem *imperativeVerbWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeVirtualListSelectionImperative";

		if( imperativeVerbWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given imperative verb word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( executeVirtualListImperative( imperativeVerbParameter, prepositionParameter, specificationWordTypeNr, relationWordTypeNr, imperativeVerbWordItem, specificationWordItem, relationWordItem ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );

		return RESULT_OK;
		}

	signed char solveWord( unsigned short solveLevel, unsigned int currentSolveProgressLevel, unsigned int endSolveProgressLevel, WordItem *solveWordItem, SelectionItem *actionSelectionItem )
		{
		bool isInverted = false;
		unsigned int nPossibilities;
		unsigned int possibilityNumber = 0;
		unsigned int solveProgressStep;
		unsigned int nextSolveProgressLevel;
		ScoreItem *possibilityScoreItem;
		BoolResultType boolResult;
		SelectionResultType selectionResult;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "solveWord";

		if( solveWordItem == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given solve word is undefined" );

		// Word has no active assignments
		if( solveWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
			{
			if( commonVariables_->currentAssignmentLevel > solveLevel )
				return adminItem_->startError( functionNameString, moduleNameString_, "The given assignment level of ", commonVariables_->currentAssignmentLevel, " is higher than the given solve level ", solveLevel );

			if( adminItem_->assignSpecification( predefinedNounSolveMethodWordItem_, predefinedAdjectiveBusyWordItem_ ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the busy flag to the solve method at assignment level ", commonVariables_->currentAssignmentLevel );

			// There already is a scores list at the start
			if( adminItem_->hasScoreList() &&
			commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL &&
			adminItem_->deleteScores() != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the admin score list" );

			isInverted = ( predefinedNounSolveMethodWordItem_->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, predefinedAdjectiveInvertedWordItem_ ) != NULL );

			// Get solve strategy parameter
			if( ( shortResult = getAssignmentWordParameter( predefinedNounSolveStrategyWordItem_ ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve strategy at assignment level ", commonVariables_->currentAssignmentLevel );

			if( ( boolResult = findSelectionToSolveWord( true, ( commonVariables_->currentAssignmentLevel == solveLevel ), isInverted, ( commonVariables_->currentAssignmentLevel + 1 < solveLevel ), shortResult.shortValue, solveWordItem ) ).result != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to find a selection to solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

			// Has found possibility to solve word
			if( boolResult.booleanValue )
				{
				if( commonVariables_->currentAssignmentLevel < solveLevel )
					{
					if( ( nPossibilities = adminItem_->nPossibilities() ) <= 0 )
						return adminItem_->startError( functionNameString, moduleNameString_, "There are no possibilities at assignment level ", commonVariables_->currentAssignmentLevel );

					solveProgressStep = ( nPossibilities == 0 ? MAX_PROGRESS : ( ( endSolveProgressLevel - currentSolveProgressLevel ) / nPossibilities ) );

					if( solveLevel > 1 )
						inputOutput_->startProgress( INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_START, solveLevel, INTERFACE_CONSOLE_I_AM_EXECUTING_SELECTIONS_END, currentSolveProgressLevel, MAX_PROGRESS );

					if( ( possibilityScoreItem = adminItem_->firstPossibility() ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't find any possibility item at assignment level ", commonVariables_->currentAssignmentLevel );

					do	{
						// Copy solve action of NO_ASSIGNMENT_LEVEL to higher levels
						if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
							actionSelectionItem = possibilityScoreItem->referenceSelectionItem;

						if( createNewAssignmentLevelInAssignmentWords() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a new assignment level: ", commonVariables_->currentAssignmentLevel );

						commonVariables_->currentAssignmentLevel++;

						if( adminItem_->assignSelectionSpecification( possibilityScoreItem->referenceSelectionItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a selection specifcation at assignment level: ", commonVariables_->currentAssignmentLevel );

						nextSolveProgressLevel = currentSolveProgressLevel + solveProgressStep;

						if( executeSelection( ( currentSolveProgressLevel + solveProgressStep / 2L ), actionSelectionItem ) != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute a selection during the solving of word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

						// Word has active assignments
						if( solveWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
							{
							if( solveWord( solveLevel, currentSolveProgressLevel, nextSolveProgressLevel, solveWordItem, actionSelectionItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

							if( commonVariables_->currentAssignmentLevel == 1 &&
							adminItem_->changeAction( actionSelectionItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to change an action" );
							}
						else
							{
							// There is a winning or losing score
							isInverted = ( predefinedNounSolveMethodWordItem_->firstNonQuestionAssignmentItem( true, false, false, false, false, NO_CONTEXT_NR, predefinedAdjectiveInvertedWordItem_ ) != NULL );

							if( !isInverted &&
							commonVariables_->currentAssignmentLevel < solveLevel )
								{
								if( adminItem_->deleteScores() != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the scores with an assignment level higher than ", commonVariables_->currentAssignmentLevel );

								// Don't solve any deeper
								solveLevel = commonVariables_->currentAssignmentLevel;
								}

							// Create winning or losing score
							if( adminItem_->createScoreItem( false, NO_SCORE, ( isInverted ? NO_SCORE : WINNING_SCORE ), NO_SCORE, ( isInverted ? WINNING_SCORE : NO_SCORE ), NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, actionSelectionItem ) != RESULT_OK )
								return adminItem_->addError( functionNameString, moduleNameString_, "I failed to create a winning or losing score of solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );
							}

						currentSolveProgressLevel = nextSolveProgressLevel;

						if( deleteAssignmentLevelInAssignmentWords() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the assignments of level ", commonVariables_->currentAssignmentLevel );

						commonVariables_->currentAssignmentLevel--;
						possibilityScoreItem = possibilityScoreItem->nextPossibilityScoreItem();

						if( ++possibilityNumber <= nPossibilities )
							{
							if( possibilityScoreItem != NULL &&
							possibilityNumber == nPossibilities )
								return adminItem_->startError( functionNameString, moduleNameString_, "I found more possibility items than number of possibilities" );
							}
						else
							{
							if( possibilityScoreItem == NULL )
								return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the next possibility item before the number of possibilities is reached" );
							}
						}
					while( possibilityScoreItem != NULL );

					if( nPossibilities > 1 ||
					// Higher level has possibilities
					commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL )
						{
						if( adminItem_->deleteScores() != RESULT_OK )
							return adminItem_->addError( functionNameString, moduleNameString_, "I failed to delete the scores with assignment level ", commonVariables_->currentAssignmentLevel );
						}
					}

				currentSolveProgressLevel = endSolveProgressLevel;

				if( solveLevel > 1 )
					inputOutput_->displayProgress( currentSolveProgressLevel );

				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
					{
					// Get solve strategy parameter
					if( ( shortResult = getAssignmentWordParameter( predefinedNounSolveStrategyWordItem_ ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve strategy at assignment level ", commonVariables_->currentAssignmentLevel );

					// Get best selection
					if( ( selectionResult = adminItem_->getBestSelection( adminItem_->isCurrentlyTesting(), shortResult.shortValue ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the best selection action of solve word \"", solveWordItem->anyWordTypeString(), "\" at assignment level ", commonVariables_->currentAssignmentLevel );

					if( ( actionSelectionItem = selectionResult.selectionItem ) == NULL )
						return adminItem_->startError( functionNameString, moduleNameString_, "I couldn't get the best action selection item" );

					// Assign best selection
					if( adminItem_->assignSelectionSpecification( actionSelectionItem ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign a selection specification at assignment level ", commonVariables_->currentAssignmentLevel );

					// Set solve method to 'done'
					if( adminItem_->assignSpecification( predefinedNounSolveMethodWordItem_, predefinedAdjectiveDoneWordItem_ ) != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to assign the done flag to the solve method at assignment level ", commonVariables_->currentAssignmentLevel );
					}
				}
			else
				{
				// Display warning
				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL &&
				inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_START, solveWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_COULD_NOT_FIND_ANY_SELECTION_TO_SOLVE_INFO_END ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
				}
			}
		else
			{
			// Display warning
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_START, solveWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_WORD_ALREADY_SOLVED_END ) != RESULT_OK )
				return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return RESULT_OK;
		}

	signed char writeInfoTextWithPossibleQueryCommands( char *textString )
		{
		bool hasFoundNewLine = false;
		size_t textStringLength;
		size_t position = 0;
		char textChar = SYMBOL_QUESTION_MARK;
		char charString[2] = SPACE_STRING;
		char writeString[MAX_SENTENCE_STRING_LENGTH] = EMPTY_STRING;
		QueryResultType queryResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "writeInfoTextWithPossibleQueryCommands";

		if( textString == NULL )
			return adminItem_->startError( functionNameString, moduleNameString_, "The given text string is undefined" );

		textStringLength = strlen( textString );

		if( textString[0] == SYMBOL_DOUBLE_QUOTE )
			position++;

		while( position < textStringLength &&
		textString[position] != SYMBOL_DOUBLE_QUOTE )
			{
			if( textString[position] == QUERY_CHAR )
				{
				if( ++position >= textStringLength )
					return adminItem_->startError( functionNameString, moduleNameString_, "The text string ended with a query character" );

				if( ( queryResult = adminItem_->executeQuery( true, false, true, INPUT_OUTPUT_PROMPT_INFO, position, textString ) ).result != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute query \"", &textString[position], "\"" );

				position = queryResult.queryCommandStringPosition;
				}
			else
				{
				if( textString[position] == SYMBOL_BACK_SLASH )
					{
					if( ++position >= textStringLength )
						return adminItem_->startError( functionNameString, moduleNameString_, "The text string ended with a diacritical sign" );

					if( ( textChar = inputOutput_->convertDiacriticalChar( textString[position] ) ) == NEW_LINE_CHAR )
						hasFoundNewLine = true;
					}
				else
					textChar = textString[position];

				position++;
				charString[0] = textChar;
				strcat( writeString, charString );
				}

			if( hasFoundNewLine ||
			strlen( writeString ) + 1 == MAX_SENTENCE_STRING_LENGTH ||

			( position < textStringLength &&
			textString[position] != SYMBOL_DOUBLE_QUOTE &&
			textString[position] == QUERY_CHAR &&
			strlen( writeString ) > 0 ) )
				{
				if( inputOutput_->writeText( false, INPUT_OUTPUT_PROMPT_INFO, NO_CENTER_WIDTH, writeString ) != RESULT_OK )
					return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write a character" );

				hasFoundNewLine = false;
				strcpy( writeString, EMPTY_STRING );
				}
			}

		if( strlen( writeString ) > 0 &&
		inputOutput_->writeText( false, INPUT_OUTPUT_PROMPT_INFO, NO_CENTER_WIDTH, writeString ) != RESULT_OK )
			return adminItem_->addError( functionNameString, moduleNameString_, "I failed to write the last characters" );

		return RESULT_OK;
		}

	BoolResultType backTrackSelectionConditions( bool isInitializeScores, bool isInverted, bool isAllowingDuplicates, bool isPreparingSort, unsigned short executionLevel, unsigned short solveStrategyParameter, unsigned int conditionSentenceNr )
		{
		bool isNewStart;
		bool isWaitingForNewLevel;
		bool isWaitingForNewStart;
		bool hasFoundScore = false;
		unsigned short selectionLevel;
		unsigned short handledSelectionLevel;
		unsigned short previousSelectionLevel;
		unsigned short unhandledSelectionLevel;
		SelectionItem *conditionSelectionItem;
		SelectionItem *previousConditionSelectionItem;
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "backTrackSelectionConditions";

		adminItem_->clearConditionChecksForSolving( MAX_LEVEL, conditionSentenceNr );

		do	{
			isWaitingForNewLevel = false;
			isWaitingForNewStart = false;
			handledSelectionLevel = MAX_LEVEL;
			previousSelectionLevel = NO_SELECTION_LEVEL;
			unhandledSelectionLevel = MAX_LEVEL;
			previousConditionSelectionItem = NULL;

			if( ( conditionSelectionItem = adminItem_->firstConditionSelectionItem( conditionSentenceNr ) ) == NULL )
				return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "I couldn't get the first item of the condition with sentence number ", conditionSentenceNr );

			if( isInitializeScores )
				{
				oldGlobalSatisfiedScore_ = NO_SCORE;
				newGlobalSatisfiedScore_ = NO_SCORE;
				oldGlobalDissatisfiedScore_ = NO_SCORE;
				newGlobalDissatisfiedScore_ = NO_SCORE;
				oldGlobalNotBlockingScore_ = NO_SCORE;
				newGlobalNotBlockingScore_ = NO_SCORE;
				oldGlobalBlockingScore_ = NO_SCORE;
				newGlobalBlockingScore_ = NO_SCORE;
				}

			do	{
				isNewStart = conditionSelectionItem->isNewStart();
				selectionLevel = conditionSelectionItem->selectionLevel();

				if( conditionSelectionItem->isConditionCheckedForSolving )
					isWaitingForNewStart = true;
				else
					{
					if( isNewStart &&
					// Not first start item
					previousConditionSelectionItem != NULL )
						{
						isWaitingForNewStart = false;

						// Second branch on the same level
						if( selectionLevel == previousSelectionLevel )
							{
							if( !isWaitingForNewLevel )
								{
								if( handledSelectionLevel == MAX_LEVEL )
									{
									handledSelectionLevel = selectionLevel;

									// This is a new branch
									if( previousConditionSelectionItem->isConditionCheckedForSolving )
										conditionSelectionItem->isConditionCheckedForSolving = true;
									else
										{
										// The previous branch was a new branch and this one is unhandled
										isWaitingForNewLevel = true;
										unhandledSelectionLevel = selectionLevel;
										previousConditionSelectionItem->isConditionCheckedForSolving = true;
										}
									}
								else
									{
									isWaitingForNewLevel = true;

									// This branch isn't handled yet
									if( unhandledSelectionLevel == MAX_LEVEL )
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
						if( ( boolCheckResult = scoreSelectionCondition( isInverted, isAllowingDuplicates, isPreparingSort, solveStrategyParameter, conditionSelectionItem ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to score a selection condition" );

						if( boolCheckResult.booleanValue )
							boolReturnResult.booleanValue = true;
						}
					}

				previousSelectionLevel = selectionLevel;

				if( isNewStart )
					previousConditionSelectionItem = conditionSelectionItem;
				}
			while( ( conditionSelectionItem = conditionSelectionItem->nextConditionItem( executionLevel, conditionSentenceNr ) ) != NULL );

			if( previousSelectionLevel != executionLevel )
				return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "I couldn't reach the given execution level ", executionLevel, ". The highest reached level was ", handledSelectionLevel );

			// All branches on same level are done and there are branchs on a higher level unhandled,
			// so start again with the handled branches (by clearing their checks) until all paths are handled
			if( unhandledSelectionLevel < MAX_LEVEL &&
			handledSelectionLevel < unhandledSelectionLevel )
				adminItem_->clearConditionChecksForSolving( unhandledSelectionLevel, conditionSentenceNr );

			if( isAllowingDuplicates &&
			isInitializeScores &&
			commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL )
				{
				hasFoundScore = false;

				if( ( oldGlobalSatisfiedScore_ > NO_SCORE ||
				newGlobalSatisfiedScore_ > NO_SCORE ||
				oldGlobalDissatisfiedScore_ > NO_SCORE ||
				newGlobalDissatisfiedScore_ > NO_SCORE ||
				oldGlobalNotBlockingScore_ > NO_SCORE ||
				newGlobalNotBlockingScore_ > NO_SCORE ||
				oldGlobalBlockingScore_ > NO_SCORE ||
				newGlobalBlockingScore_ > NO_SCORE ) &&

				adminItem_->hasScoreList() )
					{
					if( ( boolCheckResult = adminItem_->checkScores( isInverted, solveStrategyParameter, oldGlobalSatisfiedScore_, newGlobalSatisfiedScore_, oldGlobalDissatisfiedScore_, newGlobalDissatisfiedScore_, oldGlobalNotBlockingScore_, newGlobalNotBlockingScore_, oldGlobalBlockingScore_, newGlobalBlockingScore_ ) ).result != RESULT_OK )
						return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to check the scores" );

					hasFoundScore = boolCheckResult.booleanValue;
					}

				if( !hasFoundScore &&
				adminItem_->createScoreItem( true, oldGlobalSatisfiedScore_, newGlobalSatisfiedScore_, oldGlobalDissatisfiedScore_, newGlobalDissatisfiedScore_, oldGlobalNotBlockingScore_, newGlobalNotBlockingScore_, oldGlobalBlockingScore_, newGlobalBlockingScore_, NULL ) != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create an empty solve score" );
				}
			}
		while( unhandledSelectionLevel < MAX_LEVEL );

		return boolReturnResult;
		}

	BoolResultType canWordBeSolved( WordItem *solveWordItem )
		{
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "canWordBeSolved";

		if( ( boolResult = canWordBeSolved( true, solveWordItem ) ).result != RESULT_OK )
			return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find out if a word can be solved by an action" );

		// Word can't be solved
		if( !boolResult.booleanValue &&
		!commonVariables_->hasDisplayedWarning &&
		( boolResult = canWordBeSolved( false, solveWordItem ) ).result != RESULT_OK )
			return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find out if a word can be solved by an alternative action" );

		return boolResult;
		}

	BoolResultType canWordBeSolved( bool isAction, WordItem *solveWordItem )
		{
		bool isWordCanBeSolved = false;
		SelectionItem *currentExecutionSelectionItem;
		WordItem *specificationWordItem;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "canWordBeSolved";

		if( solveWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given solve word item is undefined" );

		if( ( currentExecutionSelectionItem = adminItem_->firstSelectionItem( isAction, solveWordItem ) ) != NULL )
			{
			do	{
				if( ( specificationWordItem = currentExecutionSelectionItem->specificationWordItem() ) == NULL )
					return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The specification word of the current execution selection item is undefined" );

				if( specificationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
					{
					if( !currentExecutionSelectionItem->isNegative() &&
					!currentExecutionSelectionItem->isValueSpecification() )
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
			( currentExecutionSelectionItem = currentExecutionSelectionItem->nextSelectionItem( solveWordItem ) ) != NULL );
			}

		boolResult.booleanValue = isWordCanBeSolved;
		return boolResult;
		}

	BoolResultType checkComparison( bool isFirstComparisonPart, bool isNegative, bool isNumeralRelation, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem, char *specificationString )
		{
		int comparisonResult = 0;
		int firstNumeral = 0;
		int secondNumeral = 0;
		char *firstString = NULL;
		char *secondString = NULL;
		SpecificationItem *comparisonAssignmentItem;
		WordItem *firstSpecificationWordItem;
		WordItem *secondSpecificationWordItem;
		WordItem *comparisonAssignmentSpecificationWordItem = NULL;
		BoolResultType boolResult;
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkComparison";

		if( generalizationWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		// First part
		if( isFirstComparisonPart )
			{
			if( ( specificationResult = getComparisonAssignment( false, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
				return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed get the first comparison assignment" );

			// Allow the second part of the comparison to be checked
			boolResult.booleanValue = true;

			firstComparisonAssignmentItem_ = specificationResult.specificationItem;
			secondComparisonAssignmentItem_ = NULL;
			}
		else
			{
			if( !isNumeralRelation &&
			// Second part
			specificationString == NULL )
				{
				if( ( specificationResult = getComparisonAssignment( false, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed get a comparison assignment" );

				secondComparisonAssignmentItem_ = specificationResult.specificationItem;

				firstSpecificationWordItem = ( firstComparisonAssignmentItem_ == NULL ? NULL : firstComparisonAssignmentItem_->specificationWordItem() );
				secondSpecificationWordItem = ( secondComparisonAssignmentItem_ == NULL ? NULL : secondComparisonAssignmentItem_->specificationWordItem() );

				firstString = ( firstSpecificationWordItem == NULL ? NULL : firstSpecificationWordItem->anyWordTypeString() );
				secondString = ( secondSpecificationWordItem == NULL ? NULL : secondSpecificationWordItem->anyWordTypeString() );
				}
			else
				{
				// Numeral or specification string
				if( ( specificationResult = getComparisonAssignment( isNumeralRelation, specificationWordItem, relationWordItem ) ).result != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed get the first comparison assignment" );

				if( ( comparisonAssignmentItem = specificationResult.specificationItem ) != NULL )
					comparisonAssignmentSpecificationWordItem = comparisonAssignmentItem->specificationWordItem();

				firstString = ( comparisonAssignmentItem == NULL ? NULL : ( isNumeralRelation ? ( comparisonAssignmentSpecificationWordItem == NULL ? NULL : comparisonAssignmentSpecificationWordItem->anyWordTypeString() ) : comparisonAssignmentItem->specificationString() ) );
				secondString = ( isNumeralRelation ? ( relationWordItem == NULL ? NULL : relationWordItem->anyWordTypeString() ) : specificationString );
				}

			if( firstString == NULL ||
			secondString == NULL )
				comparisonResult = ( firstString == NULL && secondString == NULL ? 0 : ( firstString == NULL ? -1 : 1 ) );
			else
				{
				if( isNumeralString( firstString ) &&
				isNumeralString( secondString ) )
					{
					sscanf( firstString, "%d", &firstNumeral );
					sscanf( secondString, "%d", &secondNumeral );

					comparisonResult = ( firstNumeral == secondNumeral ? 0 : ( firstNumeral < secondNumeral ? -1 : 1 ) );
					}
				else
					comparisonResult = strcmp( firstString, secondString );
				}

			if( generalizationWordItem->isAdjectiveComparisonLess() )
				boolResult.booleanValue = ( comparisonResult < 0 ? !isNegative : isNegative );
			else
				{
				if( generalizationWordItem->isAdjectiveComparisonEqual() )
					boolResult.booleanValue = ( comparisonResult == 0 ? !isNegative : isNegative );
				else
					{
					if( !generalizationWordItem->isAdjectiveComparisonMore() )
						return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "Word \"", generalizationWordItem->anyWordTypeString(), "\" isn't comparison word" );

					boolResult.booleanValue = ( comparisonResult > 0 ? !isNegative : isNegative );
					}
				}
			}

		return boolResult;
		}

	BoolResultType checkOddOrEvenComparison( bool isNegative, WordItem *generalizationWordItem, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		unsigned int nAssignments;
		BoolResultType boolResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkOddOrEvenComparison";

		if( generalizationWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( relationWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given relation word item is undefined" );

		if( specificationWordItem->isNounNumber() )
			{
			nAssignments = relationWordItem->nActiveAssignments();

			if( generalizationWordItem->isAdjectiveOdd() )
				boolResult.booleanValue = ( nAssignments % 2 == 1 ? !isNegative : isNegative );
			else
				{
				if( !generalizationWordItem->isAdjectiveEven() )
					return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "Word \"", generalizationWordItem->anyWordTypeString(), "\" isn't about odd or even" );

				boolResult.booleanValue = ( nAssignments % 2 == 0 ? !isNegative : isNegative );
				}
			}
		else
			{
			if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
				return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to write an interface warning" );
			}

		return boolResult;
		}

	BoolResultType findSelectionToSolveWord( bool isInitializeScores, bool isAllowingDuplicates, bool isInverted, bool isPreparingSort, unsigned short solveStrategyParameter, WordItem *solveWordItem )
		{
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findSelectionToSolveWord";

		if( solveWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given solve word is undefined" );

		if( solveWordItem->isWordCheckedForSolving )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given solve word \"", solveWordItem->anyWordTypeString(), "\" is already checked" );

		solveWordItem->isWordCheckedForSolving = true;

		// Selection available
		if( commonVariables_->adminActionList != NULL )
			{
			// Check selection actions
			if( ( boolCheckResult = findSelectionToSolveWord( true, isInitializeScores, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, solveWordItem ) ).result != RESULT_OK )
				return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find a selection action to solve condition word \"", solveWordItem->anyWordTypeString(), "\"" );

			if( boolCheckResult.booleanValue )
				boolReturnResult.booleanValue = true;

			if( !commonVariables_->hasDisplayedWarning &&
			commonVariables_->adminAlternativeList != NULL )
				{
				// Check selection alternatives
				if( ( boolCheckResult = findSelectionToSolveWord( false, isInitializeScores, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, solveWordItem ) ).result != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find a selection alternative to solve condition word \"", solveWordItem->anyWordTypeString(), "\"" );

				if( boolCheckResult.booleanValue )
					boolReturnResult.booleanValue = true;
				}
			}

		solveWordItem->isWordCheckedForSolving = false;

		return boolReturnResult;
		}

	BoolResultType findSelectionToSolveWord( bool isAction, bool isInitializeScores, bool isAllowingDuplicates, bool isInverted, bool isPreparingSort, unsigned short solveStrategyParameter, WordItem *solveWordItem )
		{
		SelectionItem *currentExecutionSelectionItem;
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findSelectionToSolveWord";

		if( solveWordItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given solve word is undefined" );

		if( ( boolCheckResult = canWordBeSolved( isAction, solveWordItem ) ).result != RESULT_OK )
			return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find out if the given word \"", solveWordItem->anyWordTypeString(), "\" can be solved by action" );

		// Word can be solved
		if( boolCheckResult.booleanValue &&
		( currentExecutionSelectionItem = adminItem_->firstSelectionItem( isAction, solveWordItem ) ) != NULL )
			{
			do	{
				if( ( boolCheckResult = backTrackSelectionConditions( isInitializeScores, isInverted, isAllowingDuplicates, isPreparingSort, currentExecutionSelectionItem->selectionLevel(), solveStrategyParameter, currentExecutionSelectionItem->activeSentenceNr() ) ).result != RESULT_OK )
					return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to back-track the condition of the selection with sentence number ", currentExecutionSelectionItem->activeSentenceNr() );

				if( boolCheckResult.booleanValue )
					boolReturnResult.booleanValue = true;
				}
			while( ( currentExecutionSelectionItem = currentExecutionSelectionItem->nextSelectionItem( solveWordItem ) ) != NULL );
			}

		return boolReturnResult;
		}

	BoolResultType scoreSelectionCondition( bool isInverted, bool isAllowingDuplicates, bool isPreparingSort, unsigned short solveStrategyParameter, SelectionItem *conditionSelectionItem )
		{
		bool hasFoundScore = false;
		bool isAddingScoresOfConditionPart = true;
		WordItem *generalizationWordItem;
		WordItem *specificationWordItem;
		BoolResultType boolCheckResult;
		BoolResultType boolReturnResult;
		ConditionResultType conditionPartResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "scoreSelectionCondition";

		if( conditionSelectionItem == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The given condition selection item is undefined" );

		if( ( generalizationWordItem = conditionSelectionItem->generalizationWordItem() ) == NULL )
			return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The generalization word of the given condition selection item is undefined" );

		if( ( conditionPartResult = checkSelectionCondition( conditionSelectionItem ) ).result != RESULT_OK )
			return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to check the condition of a selection in word word \"", generalizationWordItem->anyWordTypeString(), "\"" );

		if( !conditionPartResult.isConditionPartSatisfied &&
		!generalizationWordItem->isWordCheckedForSolving )
			{
			if( conditionSelectionItem->isAssignedOrEmpty() )
				{
				if( !conditionSelectionItem->isNegative() &&
				// Word has no active assignments
				generalizationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
					{
					if( ( boolReturnResult = findSelectionToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) ).result != RESULT_OK )
						return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find a selection to solve condition word \"", generalizationWordItem->anyWordTypeString(), "\"" );

					// Has found possibility
					if( boolReturnResult.booleanValue )
						isAddingScoresOfConditionPart = false;
					}
				}
			else
				{
				if( ( specificationWordItem = conditionSelectionItem->specificationWordItem() ) == NULL )
					return adminItem_->startBoolResultError( functionNameString, moduleNameString_, "The specification word of the given score item is undefined" );

				if( !conditionSelectionItem->isNegative() &&
				!specificationWordItem->isWordCheckedForSolving &&
				// Word has no active assignments
				generalizationWordItem->firstNonQuestionActiveAssignmentItem() == NULL )
					{
					if( ( boolCheckResult = canWordBeSolved( specificationWordItem ) ).result != RESULT_OK )
						return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find out if specification word \"", specificationWordItem->anyWordTypeString(), "\" can be solved" );

					// Word can be solved
					if( boolCheckResult.booleanValue )
						{
						if( ( boolReturnResult = findSelectionToSolveWord( false, isAllowingDuplicates, isInverted, isPreparingSort, solveStrategyParameter, generalizationWordItem ) ).result != RESULT_OK )
							return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find a selection to solve condition word \"", generalizationWordItem->anyWordTypeString(), "\"" );

						// Has found possibility
						if( boolReturnResult.booleanValue )
							isAddingScoresOfConditionPart = false;
						}
					else
						{
						if( !isAllowingDuplicates &&
						adminItem_->hasScoreList() )
							{
							if( ( boolCheckResult = adminItem_->findScore( isPreparingSort, conditionSelectionItem ) ).result != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to find a score item" );

							hasFoundScore = boolCheckResult.booleanValue;
							}

						if( !hasFoundScore )
							{
							if( adminItem_->createScoreItem( ( commonVariables_->currentAssignmentLevel > NO_ASSIGNMENT_LEVEL ), NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, NO_SCORE, conditionSelectionItem ) != RESULT_OK )
								return adminItem_->addBoolResultError( functionNameString, moduleNameString_, "I failed to create an empty solve score" );

							// Has created score
							boolReturnResult.booleanValue = true;
							}
						}
					}
				}
			}

		if( isAddingScoresOfConditionPart &&
		!commonVariables_->hasDisplayedWarning )
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

	ConditionResultType checkSelectionCondition( SelectionItem *conditionSelectionItem )
		{
		bool isNegative;
		WordItem *generalizationWordItem;
		WordItem *specificationWordItem;
		BoolResultType boolResult;
		ConditionResultType conditionPartResult;
		ConditionResultType conditionReturnResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkSelectionCondition";

		if( conditionSelectionItem == NULL )
			return adminItem_->startConditionResultError( functionNameString, moduleNameString_, "The given condition selection item is undefined" );

		if( ( generalizationWordItem = conditionSelectionItem->generalizationWordItem() ) == NULL )
			return adminItem_->startConditionResultError( functionNameString, moduleNameString_, "The generalization word of the given condition selection item is undefined" );

		if( ( specificationWordItem = conditionSelectionItem->specificationWordItem() ) == NULL )
			return adminItem_->startConditionResultError( functionNameString, moduleNameString_, "The specification word of the given condition selection item is undefined" );

		isNegative = conditionSelectionItem->isNegative();

		if( conditionSelectionItem->isAssignedOrEmpty() )
			{
			if( specificationWordItem->isAdjectiveAssigned() )
				{
				// Adjective 'assigned'
				if( ( conditionPartResult = findBlockingScores( isNegative, generalizationWordItem ) ).result != RESULT_OK )
					return adminItem_->addConditionResultError( functionNameString, moduleNameString_, "I failed to find satisfying scores" );

				conditionReturnResult.isConditionPartSatisfied = ( conditionPartResult.isConditionPartSatisfied ? isNegative : !isNegative );
				}
			else
				{
				// Adjective 'empty'
				if( ( conditionPartResult = findBlockingScores( !isNegative, generalizationWordItem ) ).result != RESULT_OK )
					return adminItem_->addConditionResultError( functionNameString, moduleNameString_, "I failed to find satisfying scores" );

				conditionReturnResult.isConditionPartSatisfied = ( conditionPartResult.isConditionPartSatisfied ? !isNegative : isNegative );
				}

			conditionReturnResult.oldBlockingScore += conditionPartResult.oldBlockingScore;
			conditionReturnResult.newBlockingScore += conditionPartResult.newBlockingScore;
			conditionReturnResult.oldNotBlockingScore += conditionPartResult.oldNotBlockingScore;
			conditionReturnResult.newNotBlockingScore += conditionPartResult.newNotBlockingScore;
			}
		else
			{
			if( generalizationWordItem->isAdjectiveComparison() )
				{
				if( ( boolResult = checkComparison( conditionSelectionItem->isFirstComparisonPart(), isNegative, conditionSelectionItem->isNumeralRelation(), generalizationWordItem, specificationWordItem, conditionSelectionItem->relationWordItem(), conditionSelectionItem->specificationString() ) ).result != RESULT_OK )
					return adminItem_->addConditionResultError( functionNameString, moduleNameString_, "I failed to check a comparison" );

				conditionReturnResult.isConditionPartSatisfied = boolResult.booleanValue;
				}
			else
				{
				if( generalizationWordItem->isAdjectiveOddOrEven() )
					{
					if( ( boolResult = checkOddOrEvenComparison( isNegative, generalizationWordItem, specificationWordItem, conditionSelectionItem->relationWordItem() ) ).result != RESULT_OK )
						return adminItem_->addConditionResultError( functionNameString, moduleNameString_, "I failed to check for odd or even" );

					conditionReturnResult.isConditionPartSatisfied = boolResult.booleanValue;
					}
				else
					{
					if( conditionSelectionItem->isValueSpecification() )
						{
						if( ( conditionPartResult = checkValueConditionPart( isNegative, conditionSelectionItem->isPossessive(), generalizationWordItem, specificationWordItem ) ).result != RESULT_OK )
							return adminItem_->addConditionResultError( functionNameString, moduleNameString_, "I failed to check the condition of a specification by value" );

						conditionReturnResult.isConditionPartSatisfied = conditionPartResult.isConditionPartSatisfied;
						}
					else
						{
						if( ( conditionPartResult = findSatisfyingScores( conditionSelectionItem->isPossessive(), !isNegative, conditionSelectionItem->relationContextNr(), generalizationWordItem, specificationWordItem ) ).result != RESULT_OK )
							return adminItem_->addConditionResultError( functionNameString, moduleNameString_, "I failed to find satisfying scores" );

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

	ConditionResultType checkValueConditionPart( bool isNegative, bool isPossessive, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool isSatisfiedScore;
		SpecificationItem *foundAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		ConditionResultType conditionPartResult;
		ConditionResultType conditionReturnResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "checkValueConditionPart";

		conditionReturnResult.isConditionPartSatisfied = true;

		if( specificationWordItem == NULL )
			return adminItem_->startConditionResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentSpecificationItem = specificationWordItem->firstExclusiveSpecificationItem() ) != NULL )
			{
			do	{
				foundAssignmentItem = specificationWordItem->firstNonQuestionAssignmentItem( true, false, false, false, isPossessive, currentSpecificationItem->relationContextNr(), currentSpecificationItem->specificationWordItem() );
				isSatisfiedScore = ( isNegative == ( foundAssignmentItem == NULL || foundAssignmentItem->isNegative() ) );

				if( ( conditionPartResult = findSatisfyingScores( isPossessive, isSatisfiedScore, currentSpecificationItem->relationContextNr(), generalizationWordItem, currentSpecificationItem->specificationWordItem() ) ).result != RESULT_OK )
					return adminItem_->addConditionResultError( functionNameString, moduleNameString_, "I failed to find satisfying scores" );

				// Scoring assignment is unsatisfying
				if( conditionPartResult.isConditionPartSatisfied != isSatisfiedScore )
					conditionReturnResult.isConditionPartSatisfied = false;

				conditionReturnResult.oldSatisfiedScore += conditionPartResult.oldSatisfiedScore;
				conditionReturnResult.newSatisfiedScore += conditionPartResult.newSatisfiedScore;
				conditionReturnResult.oldDissatisfiedScore += conditionPartResult.oldDissatisfiedScore;
				conditionReturnResult.newDissatisfiedScore += conditionPartResult.newDissatisfiedScore;
				}
			while( ( currentSpecificationItem = currentSpecificationItem->nextExclusiveSpecificationItem() ) != NULL );
			}

		return conditionReturnResult;
		}

	ConditionResultType findBlockingScores( bool isBlocking, WordItem *generalizationWordItem )
		{
		bool hasFoundBlockingAssignment = false;
		SpecificationItem *currentAssignmentItem;
		ConditionResultType conditionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findBlockingScores";

		if( generalizationWordItem == NULL )
			return adminItem_->startConditionResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( ( currentAssignmentItem = generalizationWordItem->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			do	{
				if( !currentAssignmentItem->isNegative() )
					{
					if( isBlocking )
						{
						if( currentAssignmentItem->isOlderItem() )
							conditionResult.oldBlockingScore++;
						else
							conditionResult.newBlockingScore++;
						}
					else
						{
						if( currentAssignmentItem->isOlderItem() )
							conditionResult.oldNotBlockingScore++;
						else
							conditionResult.newNotBlockingScore++;
						}

					hasFoundBlockingAssignment = true;
					}
				}
			while( !hasFoundBlockingAssignment &&
			( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
			}

		conditionResult.isConditionPartSatisfied = !hasFoundBlockingAssignment;
		return conditionResult;
		}

	ConditionResultType findSatisfyingScores( bool isPossessive, bool isSatisfiedScore, unsigned int relationContextNr, WordItem *generalizationWordItem, WordItem *specificationWordItem )
		{
		bool hasFoundScoringAssignment = false;
		SpecificationItem *currentAssignmentItem;
		ConditionResultType conditionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "findSatisfyingScores";

		if( generalizationWordItem == NULL )
			return adminItem_->startConditionResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( specificationWordItem == NULL )
			return adminItem_->startConditionResultError( functionNameString, moduleNameString_, "The given specification word item is undefined" );

		if( ( currentAssignmentItem = generalizationWordItem->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			do	{
				if( currentAssignmentItem->isRelatedSpecification( false, isPossessive, relationContextNr, specificationWordItem ) )
					{
					hasFoundScoringAssignment = true;

					if( isSatisfiedScore )
						{
						if( currentAssignmentItem->isOlderItem() )
							conditionResult.oldSatisfiedScore++;
						else
							conditionResult.newSatisfiedScore++;
						}
					else
						{
						if( currentAssignmentItem->isOlderItem() )
							conditionResult.oldDissatisfiedScore++;
						else
							conditionResult.newDissatisfiedScore++;
						}
					}
				}
			while( !hasFoundScoringAssignment &&
			( currentAssignmentItem = currentAssignmentItem->nextSelectedSpecificationItem() ) != NULL );
			}

		conditionResult.isConditionPartSatisfied = hasFoundScoringAssignment;
		return conditionResult;
		}

	ShortResultType getAssignmentOrderNr( WordItem *generalizationWordItem )
		{
		unsigned short assignmentOrderNr = 0;
		unsigned short specificationNr = 0;
		SpecificationItem *orderAssignmentItem;
		SpecificationItem *currentSpecificationItem;
		WordItem *assignmentWordItem;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getAssignmentOrderNr";

		if( generalizationWordItem == NULL )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem->nActiveAssignments() > 1 )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "I have more than one assignment at assignment level ", commonVariables_->currentAssignmentLevel );

		orderAssignmentItem = generalizationWordItem->firstNonQuestionActiveAssignmentItem();

		if( orderAssignmentItem != NULL &&
		( assignmentWordItem = orderAssignmentItem->specificationWordItem() ) != NULL &&
		( currentSpecificationItem = generalizationWordItem->firstExclusiveSpecificationItem() ) != NULL )
			{
			do	{
				specificationNr++;

				if( currentSpecificationItem->specificationWordItem() == assignmentWordItem )
					assignmentOrderNr = specificationNr;
				}
			while( assignmentOrderNr == NO_ORDER_NR &&
			( currentSpecificationItem = currentSpecificationItem->nextExclusiveSpecificationItem() ) != NULL );
			}

		shortResult.shortValue = assignmentOrderNr;
		return shortResult;
		}

	ShortResultType getAssignmentWordParameter( WordItem *generalizationWordItem )
		{
		SpecificationItem *assignmentItem;
		WordItem *specificationWordItem;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getAssignmentWordParameter";

		if( generalizationWordItem == NULL )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "The given generalization word item is undefined" );

		if( generalizationWordItem->nActiveAssignments() > 1 )
			return adminItem_->startShortResultError( functionNameString, moduleNameString_, "I have more than one assignment at assignment level ", commonVariables_->currentAssignmentLevel );

		if( ( assignmentItem = generalizationWordItem->firstNonQuestionActiveAssignmentItem() ) != NULL )
			{
			if( ( specificationWordItem = assignmentItem->specificationWordItem() ) == NULL )
				return adminItem_->startShortResultError( functionNameString, moduleNameString_, "I found an undefined assignment word at assignment level ", commonVariables_->currentAssignmentLevel );

			shortResult.shortValue = specificationWordItem->wordParameter();
			}

		return shortResult;
		}

	SpecificationResultType getComparisonAssignment( bool isNumeralRelation, WordItem *specificationWordItem, WordItem *relationWordItem )
		{
		SpecificationResultType specificationResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "getComparisonAssignment";

		if( specificationWordItem == NULL )
			return adminItem_->startSpecificationResultError( functionNameString, moduleNameString_, NULL, "The given specification word item is undefined" );

		if( isNumeralRelation )
			specificationResult.specificationItem = specificationWordItem->firstActiveNumeralAssignmentItem();
		else
			{
			if( relationWordItem == NULL )
				specificationResult.specificationItem = specificationWordItem->firstActiveStringAssignmentItem();
			else
				{
				if( specificationWordItem->isNounHead() )
					specificationResult.specificationItem = relationWordItem->lastActiveNonQuestionAssignmentItem();
				else
					{
					if( specificationWordItem->isNounTail() )
						specificationResult.specificationItem = relationWordItem->firstNonQuestionActiveAssignmentItem();
					else
						{
						if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, specificationWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
							return adminItem_->addSpecificationResultError( functionNameString, moduleNameString_, NULL, "I failed to write an interface warning" );
						}
					}
				}
			}

		return specificationResult;
		}


	protected:
	// Constructor

	AdminImperative( AdminItem *adminItem, CommonVariables *commonVariables, InputOutput *inputOutput, WordItem *predefinedAdjectiveBusyWordItem, WordItem *predefinedAdjectiveDoneWordItem, WordItem *predefinedAdjectiveInvertedWordItem, WordItem *predefinedNounSolveLevelWordItem, WordItem *predefinedNounSolveMethodWordItem, WordItem *predefinedNounSolveStrategyWordItem )
		{
		char errorString[MAX_ERROR_STRING_LENGTH] = EMPTY_STRING;

		// Private constructed variables

		hasRequestedRestart_ = false;

		oldGlobalSatisfiedScore_ = NO_SCORE;
		newGlobalSatisfiedScore_ = NO_SCORE;
		oldGlobalDissatisfiedScore_ = NO_SCORE;
		newGlobalDissatisfiedScore_ = NO_SCORE;
		oldGlobalNotBlockingScore_ = NO_SCORE;
		newGlobalNotBlockingScore_ = NO_SCORE;
		oldGlobalBlockingScore_ = NO_SCORE;
		newGlobalBlockingScore_ = NO_SCORE;

		strcpy( closedTestFileNameString_, EMPTY_STRING );

		firstComparisonAssignmentItem_ = NULL;
		secondComparisonAssignmentItem_ = NULL;
		virtualListAssignmentItem_ = NULL;

		// Private initialized variables

		strcpy( moduleNameString_, "AdminImperative" );

		// Checking private initialized variables

		if( ( predefinedAdjectiveBusyWordItem_ = predefinedAdjectiveBusyWordItem ) == NULL )
			strcpy( errorString, "The given predefined adjective Busy word item is undefined" );

		if( ( predefinedAdjectiveDoneWordItem_ = predefinedAdjectiveDoneWordItem ) == NULL )
			strcpy( errorString, "The given predefined adjective Done word item is undefined" );

		if( ( predefinedAdjectiveInvertedWordItem_ = predefinedAdjectiveInvertedWordItem ) == NULL )
			strcpy( errorString, "The given predefined adjective Inverted word item is undefined" );

		if( ( predefinedNounSolveLevelWordItem_ = predefinedNounSolveLevelWordItem ) == NULL )
			strcpy( errorString, "The given predefined noun Solve Level word item is undefined" );

		if( ( predefinedNounSolveMethodWordItem_ = predefinedNounSolveMethodWordItem ) == NULL )
			strcpy( errorString, "The given predefined noun Solve Method word item is undefined" );

		if( ( predefinedNounSolveStrategyWordItem_ = predefinedNounSolveStrategyWordItem ) == NULL )
			strcpy( errorString, "The given predefined noun Solve Strategy word item is undefined" );

		if( ( adminItem_ = adminItem ) == NULL )
			{
			if( commonVariables != NULL )
				commonVariables->result = RESULT_SYSTEM_ERROR;

			fprintf( stderr, "\nClass:%s\nFunction:\t%s\nError:\t\tThe given admin item is undefined.\n", moduleNameString_, INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME );
			}
		else
			{
			if( ( commonVariables_ = commonVariables ) == NULL )
				strcpy( errorString, "The given common variables is undefined" );

			if( ( inputOutput_ = inputOutput ) == NULL )
				strcpy( errorString, "The given input-output is undefined" );

			if( strlen( errorString ) > 0 )
				adminItem_->startSystemError( INPUT_OUTPUT_ERROR_CONSTRUCTOR_FUNCTION_NAME, moduleNameString_, errorString );
			}
		}


	// Protected functions

	bool hasRequestedRestart()
		{
		return hasRequestedRestart_;
		}

	signed char executeImperative( bool isInitializeVariables, unsigned short executionListNr, unsigned short imperativeVerbParameter, unsigned short executionNounWordParameter, unsigned short specificationWordTypeNr, unsigned int endSolveProgressLevel, char *executionString, WordItem *imperativeVerbWordItem, WordItem *specificationWordItem, ReadItem *startRelationWordReadItem, ReadItem *endRelationReadItem, SelectionItem *executionSelectionItem, SelectionItem *actionSelectionItem )
		{
		bool isDisplayingRelationWarning = false;
		ShortResultType shortResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeImperative";

		if( isInitializeVariables )
			virtualListAssignmentItem_ = NULL;

		switch( imperativeVerbParameter )
			{
			// Selection
			case NO_IMPERATIVE_PARAMETER:
				if( adminItem_->assignSelectionSpecification( executionSelectionItem ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to assign an imperative selection specification at assignment level ", commonVariables_->currentAssignmentLevel );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_ADD:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_MOVE:
			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REMOVE:
				if( executionSelectionItem == NULL )
					{
					if( executeVirtualListImperative( imperativeVerbParameter, specificationWordTypeNr, imperativeVerbWordItem, specificationWordItem, startRelationWordReadItem, endRelationReadItem ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to execute a virtual list imperative" );
					}
				else
					{
					if( executeVirtualListSelectionImperative( imperativeVerbParameter, executionSelectionItem->prepositionParameter(), specificationWordTypeNr, executionSelectionItem->relationWordTypeNr(), imperativeVerbWordItem, specificationWordItem, executionSelectionItem->relationWordItem() ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to execute a selection virtual list imperative" );
					}

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_CLEAR:
				if( imperativeVerbWordItem == NULL )
					return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "The given imperative verb word item is undefined" );

				if( startRelationWordReadItem == NULL )
					{
					switch( executionNounWordParameter )
						{
						case WORD_PARAMETER_NOUN_MIND:
							adminItem_->deleteSentences( adminItem_->firstSentenceNrOfCurrentUser() );

							if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_NOTIFICATION, INTERFACE_IMPERATIVE_NOTIFICATION_MY_MIND_IS_CLEAR ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write the 'my mind is clear' interface notification about clearing my mind" );

							break;

						default:
							if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write an interface warning about clearing" );
						}
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_HELP:
				if( imperativeVerbWordItem == NULL )
					return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "The given imperative verb word item is undefined" );

				if( startRelationWordReadItem == NULL )
					{
					if( adminItem_->readInfoFile( true, imperativeVerbWordItem->anyWordTypeString() ).result != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to read the info help file" );
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_LOGIN:
				if( startRelationWordReadItem == NULL )
					{
					if( adminItem_->login( specificationWordItem ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to login" );
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_READ:
				if( startRelationWordReadItem == NULL )
					{
					switch( executionNounWordParameter )
						{
						case WORD_PARAMETER_NOUN_FILE:
							if( adminItem_->readExampleFile( executionString ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to read an example file" );

							break;

						case WORD_PARAMETER_NOUN_TEST_FILE:
							if( executionString == NULL ||
							strlen( executionString ) == 0 )
								return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "The given execution string is undefined or empty" );

							// Store file name before closing the test file
							strcpy( closedTestFileNameString_, executionString );

							// Postpone result check. Close test file first
							adminItem_->readTestFile( closedTestFileNameString_ );

							// Stop redirecting to test results
							inputOutput_->redirectOutputToTestFile( NULL );

							if( commonVariables_->result == RESULT_OK &&
							adminItem_->compareOutputFileAgainstReferenceFile( closedTestFileNameString_ ) != RESULT_OK )
								return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to compare the test file against the reference file" );

							// Continue redirecting to test results
							inputOutput_->redirectOutputToTestFile( adminItem_->currentWriteFile() );

							// Now check the result
							if( commonVariables_->result != RESULT_OK )
								{
								inputOutput_->clearProgress();
								return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to read a test file" );
								}

							break;

						default:
							if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
								return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: ", executionNounWordParameter );

							return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute imperative 'read'. Unknown specification parameter: ", executionNounWordParameter, " at assignment level ", commonVariables_->currentAssignmentLevel );
						}
					}
				else
					isDisplayingRelationWarning = true;

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_UNDO:
				if( adminItem_->undoLastSentence() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to undo the last sentence" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_REDO:
				if( adminItem_->redoLastUndoneSentence() != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to redo the last undone sentence" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_RESTART:
				hasRequestedRestart_ = true;
				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_DISPLAY:
				if( executeImperativeDisplay( executionNounWordParameter, specificationWordTypeNr, imperativeVerbWordItem, specificationWordItem, executionString ) != RESULT_OK )
					return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to execute the display imperative" );

				break;

			case WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE:
				if( executionSelectionItem != NULL ||

				( executionListNr == NO_LIST_NR &&
				specificationWordItem != NULL &&
				specificationWordItem->wordParameter() != WORD_PARAMETER_SINGULAR_VERB_IMPERATIVE_SOLVE ) )
					{
					adminItem_->deleteTemporaryScoreList();

					// Get solve level
					if( ( shortResult = getAssignmentOrderNr( predefinedNounSolveLevelWordItem_ ) ).result != RESULT_OK )
						return adminItem_->addError( functionNameString, moduleNameString_, "I failed to get the solve level" );

					if( solveWord( shortResult.shortValue, 0, endSolveProgressLevel, specificationWordItem, actionSelectionItem ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to solve a word at assignment level ", commonVariables_->currentAssignmentLevel );
					}
				else
					{
					if( inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_START, imperativeVerbWordItem->anyWordTypeString(), INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_HOW_TO_EXECUTE_IMPERATIVE_VERB_END ) != RESULT_OK )
						return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write an interface warning about solving" );
					}

				break;

			default:
				if( commonVariables_->currentAssignmentLevel == NO_ASSIGNMENT_LEVEL )
					return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute the imperative with word parameter: ", imperativeVerbParameter );

				return adminItem_->startErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I don't know how to execute the imperative with word parameter: ", imperativeVerbParameter, ", at assignment level ", commonVariables_->currentAssignmentLevel );
			}

		if( isDisplayingRelationWarning &&
		inputOutput_->writeInterfaceText( false, INPUT_OUTPUT_PROMPT_WARNING, INTERFACE_IMPERATIVE_WARNING_I_DONT_KNOW_TO_DO_WITH_RELATION ) != RESULT_OK )
			return adminItem_->addErrorWithAdminListNr( executionListNr, functionNameString, moduleNameString_, "I failed to write an interface warning" );

		return RESULT_OK;
		}

	signed char executeSelection( unsigned int endSolveProgressLevel, SelectionItem *actionSelectionItem )
		{
		bool hasDoneLastExecution;
		bool isConditionSatisfied;
		bool isInitializeVariables;
		bool isNewStart;
		bool isWaitingForNewStart;
		bool isWaitingForNewLevel;
		bool isWaitingForExecution;
		unsigned short executionLevel;
		unsigned short executionListNr;
		unsigned short selectionLevel;
		unsigned short nSelectionInterations = 0;
		unsigned int executionSentenceNr;
		SelectionItem *conditionSelectionItem;
		SelectionItem *executionSelectionItem;
		ConditionResultType conditionResult;
		char functionNameString[FUNCTION_NAME_LENGTH] = "executeSelection";

		do	{
			hasDoneLastExecution = false;
			commonVariables_->isAssignmentChanged = false;

			if( ( conditionSelectionItem = adminItem_->firstConditionItem() ) != NULL )
				{
				isConditionSatisfied = false;
				isWaitingForNewStart = false;
				isWaitingForNewLevel = false;
				isWaitingForExecution = false;
				executionLevel = conditionSelectionItem->selectionLevel();
				executionSentenceNr = conditionSelectionItem->activeSentenceNr();

				do	{
					if( conditionSelectionItem == NULL ||
					executionSentenceNr != conditionSelectionItem->activeSentenceNr() )
						{
						executionListNr = ( isConditionSatisfied ? ADMIN_ACTION_LIST : ADMIN_ALTERNATIVE_LIST );

						if( ( executionSelectionItem = adminItem_->executionStartEntry( isConditionSatisfied, executionLevel, executionSentenceNr ) ) != NULL )
							{
							isInitializeVariables = true;

							do	{
								if( adminItem_->executeImperative( isInitializeVariables, executionListNr, executionSelectionItem->imperativeVerbParameter(), NO_WORD_PARAMETER, executionSelectionItem->specificationWordTypeNr(), endSolveProgressLevel, executionSelectionItem->specificationString(), executionSelectionItem->generalizationWordItem(), executionSelectionItem->specificationWordItem(), NULL, NULL, executionSelectionItem, actionSelectionItem ) != RESULT_OK )
									return adminItem_->addError( functionNameString, moduleNameString_, "I failed to execute an imperative" );

								isInitializeVariables = false;
								}
							while( !hasRequestedRestart_ &&
							!commonVariables_->hasDisplayedWarning &&
							( executionSelectionItem = executionSelectionItem->nextExecutionItem( executionLevel, executionSentenceNr ) ) != NULL );
							}

						// Found new condition
						if( conditionSelectionItem != NULL )
							{
							isConditionSatisfied = false;
							isWaitingForNewStart = false;
							isWaitingForNewLevel = false;
							isWaitingForExecution = false;
							executionLevel = conditionSelectionItem->selectionLevel();
							}
						}

					if( conditionSelectionItem == NULL )
						hasDoneLastExecution = true;
					else
						{
						isNewStart = conditionSelectionItem->isNewStart();
						selectionLevel = conditionSelectionItem->selectionLevel();

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
							!isConditionSatisfied &&
							executionLevel == selectionLevel )
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
									isConditionSatisfied != conditionSelectionItem->isAction() )
										// Skip checking of this condition and wait for the next condition sentence number to come
										isWaitingForExecution = true;
									else
										{
										if( ( conditionResult = checkSelectionCondition( conditionSelectionItem ) ).result != RESULT_OK )
											return adminItem_->addError( functionNameString, moduleNameString_, "I failed to check the condition of a selection" );

										isConditionSatisfied = conditionResult.isConditionPartSatisfied;
										executionLevel = selectionLevel;
										executionSentenceNr = conditionSelectionItem->activeSentenceNr();
										}
									}
								}
							}

						conditionSelectionItem = conditionSelectionItem->nextSelectionItem();
						}
					}
				while( !hasDoneLastExecution &&
				!hasRequestedRestart_ &&
				!commonVariables_->hasDisplayedWarning );
				}
			}
		while( !hasRequestedRestart_ &&
		!commonVariables_->hasDisplayedWarning &&
		commonVariables_->isAssignmentChanged &&
		++nSelectionInterations < MAX_SELECTION_ITERATIONS );

		if( commonVariables_->isAssignmentChanged &&
		nSelectionInterations == MAX_SELECTION_ITERATIONS )
			return adminItem_->startError( functionNameString, moduleNameString_, "There is probably an endless loop in the selections" );

		return RESULT_OK;
		}
	};

/*************************************************************************
 *	"He has given me a new song to sing,
 *	a hymn of praise to our God.
 *	Many will see what he has done and be amazed.
 *	They will put their trust in the Lord." (Psalm 40:3)
 *************************************************************************/
